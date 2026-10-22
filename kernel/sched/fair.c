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
#समावेश <linux/energy_model.h>
#समावेश <linux/mmap_lock.h>
#समावेश <linux/hugetlb_inline.h>
#समावेश <linux/jiffies.h>
#समावेश <linux/math.h>
#समावेश <linux/mm_api.h>
#समावेश <linux/highmem.h>
#समावेश <linux/hrtimer.h>
#समावेश <linux/hrtimer_bases.h>
#समावेश <linux/spinlock_api.h>
#समावेश <linux/cpumask_api.h>
#समावेश <linux/lockdep_api.h>
#समावेश <linux/softirq.h>
#समावेश <linux/refcount_api.h>
#समावेश <linux/topology.h>
#समावेश <linux/sched/clock.h>
#समावेश <linux/sched/cond_resched.h>
#समावेश <linux/sched/cputime.h>
#समावेश <linux/sched/isolation.h>
#समावेश <linux/sched/nohz.h>
#समावेश <linux/sched/prio.h>
#समावेश <linux/static_call.h>

#समावेश <linux/cpuidle.h>
#समावेश <linux/interrupt.h>
#समावेश <linux/memory-tiers.h>
#समावेश <linux/mempolicy.h>
#समावेश <linux/mutex_api.h>
#समावेश <linux/profile.h>
#समावेश <linux/psi.h>
#समावेश <linux/ratelimit.h>
#समावेश <linux/task_work.h>
#समावेश <linux/rbtree_augmented.h>

#समावेश <asm/switch_to.h>

#समावेश <uapi/linux/sched/types.h>

#समावेश "sched.h"
#समावेश "stats.h"
#समावेश "autogroup.h"

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
अचिन्हित पूर्णांक sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
अचिन्हित पूर्णांक sysctl_sched_base_slice			= 700000ULL;
अटल अचिन्हित पूर्णांक normalized_sysctl_sched_base_slice	= 700000ULL;

__read_mostly अचिन्हित पूर्णांक sysctl_sched_migration_cost	= 500000UL;

अटल पूर्णांक __init setup_sched_thermal_decay_shift(अक्षर *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	वापस 1;
}
__setup("sched_thermal_decay_shift=", setup_sched_thermal_decay_shift);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
पूर्णांक __weak arch_asym_cpu_priority(पूर्णांक cpu)
{
	वापस -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#घोषणा fits_capacity(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#घोषणा capacity_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#अगर_घोषित CONFIG_CFS_BANDWIDTH
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
अटल अचिन्हित पूर्णांक sysctl_sched_cfs_bandwidth_slice		= 5000UL;
#पूर्ण_अगर

#अगर_घोषित CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
अटल अचिन्हित पूर्णांक sysctl_numa_balancing_promote_rate_limit = 65536;
#पूर्ण_अगर

#अगर_घोषित CONFIG_SYSCTL
अटल स्थिर काष्ठा ctl_table sched_fair_sysctls[] = {
#अगर_घोषित CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.तथ्य           = &sysctl_sched_cfs_bandwidth_slice,
		.maxlen         = माप(अचिन्हित पूर्णांक),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#पूर्ण_अगर
#अगर_घोषित CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.तथ्य		= &sysctl_numa_balancing_promote_rate_limit,
		.maxlen		= माप(अचिन्हित पूर्णांक),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#पूर्ण_अगर /* CONFIG_NUMA_BALANCING */
};

अटल पूर्णांक __init sched_fair_sysctl_init(व्योम)
{
	register_sysctl_init("kernel", sched_fair_sysctls);
	वापस 0;
}
late_initcall(sched_fair_sysctl_init);
#पूर्ण_अगर /* CONFIG_SYSCTL */

अटल अंतरभूत व्योम update_load_add(काष्ठा load_weight *lw, अचिन्हित दीर्घ inc)
{
	lw->weight += inc;
	lw->inv_weight = 0;
}

अटल अंतरभूत व्योम update_load_sub(काष्ठा load_weight *lw, अचिन्हित दीर्घ dec)
{
	lw->weight -= dec;
	lw->inv_weight = 0;
}

अटल अंतरभूत व्योम update_load_set(काष्ठा load_weight *lw, अचिन्हित दीर्घ w)
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
अटल अचिन्हित पूर्णांक get_update_sysctl_factor(व्योम)
{
	अचिन्हित पूर्णांक cpus = min_t(अचिन्हित पूर्णांक, अंक_online_cpus(), 8);
	अचिन्हित पूर्णांक factor;

	चयन (sysctl_sched_tunable_scaling) {
	हाल SCHED_TUNABLESCALING_NONE:
		factor = 1;
		अवरोध;
	हाल SCHED_TUNABLESCALING_LINEAR:
		factor = cpus;
		अवरोध;
	हाल SCHED_TUNABLESCALING_LOG:
	शेष:
		factor = 1 + ilog2(cpus);
		अवरोध;
	}

	वापस factor;
}

अटल व्योम update_sysctl(व्योम)
{
	अचिन्हित पूर्णांक factor = get_update_sysctl_factor();

#घोषणा SET_SYSCTL(name) \
	(sysctl_##name = (factor) * normalized_sysctl_##name)
	SET_SYSCTL(sched_base_slice);
#अघोषित SET_SYSCTL
}

व्योम __init sched_init_granularity(व्योम)
{
	update_sysctl();
}

#अगर_अघोषित CONFIG_64BIT
#घोषणा WMULT_CONST	(~0U)
#घोषणा WMULT_SHIFT	32

अटल व्योम __update_inv_weight(काष्ठा load_weight *lw)
{
	अचिन्हित दीर्घ w;

	अगर (likely(lw->inv_weight))
		वापस;

	w = scale_load_down(lw->weight);

	अगर (BITS_PER_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_weight = 1;
	अन्यथा अगर (unlikely(!w))
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
अटल u64 __calc_delta(u64 delta_exec, अचिन्हित दीर्घ weight, काष्ठा load_weight *lw)
{
	u64 fact = scale_load_down(weight);
	u32 fact_hi = (u32)(fact >> 32);
	पूर्णांक shift = WMULT_SHIFT;
	पूर्णांक fs;

	__update_inv_weight(lw);

	अगर (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		shift -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_weight);

	fact_hi = (u32)(fact >> 32);
	अगर (fact_hi) {
		fs = fls(fact_hi);
		shift -= fs;
		fact >>= fs;
	}

	वापस mul_u64_u32_shr(delta_exec, fact, shift);
}
#अन्यथा
अटल u64 __calc_delta(u64 delta_exec, अचिन्हित दीर्घ weight, काष्ठा load_weight *lw)
{
	वापस (delta_exec * weight) / lw->weight;
}
#पूर्ण_अगर

/*
 * delta /= w
 */
अटल अंतरभूत u64 calc_delta_fair(u64 delta, काष्ठा sched_entity *se)
{
	अगर (se->h_load.weight != NICE_0_LOAD)
		delta = __calc_delta(delta, NICE_0_LOAD, &se->h_load);

	वापस delta;
}

स्थिर काष्ठा sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED

/* Walk up scheduling entities hierarchy */
#घोषणा for_each_sched_entity(se) \
		क्रम (; se; se = se->parent)

अटल अंतरभूत bool list_add_leaf_cfs_rq(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा rq *rq = rq_of(cfs_rq);
	पूर्णांक cpu = cpu_of(rq);

	अगर (cfs_rq->on_list)
		वापस rq->tmp_alone_branch == &rq->leaf_cfs_rq_list;

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
	अगर (cfs_rq->tg->parent &&
	    tg_cfs_rq(cfs_rq->tg->parent, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_add_पुच्छ_rcu(&cfs_rq->leaf_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->parent, cpu)->leaf_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
		वापस true;
	}

	अगर (!cfs_rq->tg->parent) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_add_पुच्छ_rcu(&cfs_rq->leaf_cfs_rq_list,
			&rq->leaf_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
		वापस true;
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
	वापस false;
}

अटल अंतरभूत व्योम list_del_leaf_cfs_rq(काष्ठा cfs_rq *cfs_rq)
{
	अगर (cfs_rq->on_list) {
		काष्ठा rq *rq = rq_of(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		अगर (rq->tmp_alone_branch == &cfs_rq->leaf_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->leaf_cfs_rq_list.पूर्व;

		list_del_rcu(&cfs_rq->leaf_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

अटल अंतरभूत व्योम assert_list_leaf_cfs_rq(काष्ठा rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->leaf_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#घोषणा for_each_leaf_cfs_rq_safe(rq, cfs_rq, pos)			\
	list_for_each_entry_safe(cfs_rq, pos, &rq->leaf_cfs_rq_list,	\
				 leaf_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
अटल अंतरभूत काष्ठा cfs_rq *
is_same_group(काष्ठा sched_entity *se, काष्ठा sched_entity *pse)
{
	अगर (se->cfs_rq == pse->cfs_rq)
		वापस se->cfs_rq;

	वापस शून्य;
}

अटल अंतरभूत काष्ठा sched_entity *parent_entity(स्थिर काष्ठा sched_entity *se)
{
	वापस se->parent;
}

अटल पूर्णांक tg_is_idle(काष्ठा task_group *tg)
{
	वापस tg->idle > 0;
}

अटल पूर्णांक cfs_rq_is_idle(काष्ठा cfs_rq *cfs_rq)
{
	वापस cfs_rq->idle > 0;
}

अटल पूर्णांक se_is_idle(काष्ठा sched_entity *se)
{
	अगर (entity_is_task(se))
		वापस task_has_idle_policy(task_of(se));
	वापस cfs_rq_is_idle(group_cfs_rq(se));
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

#घोषणा for_each_sched_entity(se) \
		क्रम (; se; se = शून्य)

अटल अंतरभूत bool list_add_leaf_cfs_rq(काष्ठा cfs_rq *cfs_rq)
{
	वापस true;
}

अटल अंतरभूत व्योम list_del_leaf_cfs_rq(काष्ठा cfs_rq *cfs_rq)
{
}

अटल अंतरभूत व्योम assert_list_leaf_cfs_rq(काष्ठा rq *rq)
{
}

#घोषणा for_each_leaf_cfs_rq_safe(rq, cfs_rq, pos)	\
		क्रम (cfs_rq = &rq->cfs, pos = शून्य; cfs_rq; cfs_rq = pos)

अटल अंतरभूत काष्ठा sched_entity *parent_entity(काष्ठा sched_entity *se)
{
	वापस शून्य;
}

अटल अंतरभूत पूर्णांक tg_is_idle(काष्ठा task_group *tg)
{
	वापस 0;
}

अटल पूर्णांक cfs_rq_is_idle(काष्ठा cfs_rq *cfs_rq)
{
	वापस 0;
}

अटल पूर्णांक se_is_idle(काष्ठा sched_entity *se)
{
	वापस task_has_idle_policy(task_of(se));
}

#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

अटल __always_inline
bool account_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

बाह्य व्योम __BUILD_BUG_vruntime_cmp(व्योम);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#घोषणा vruntime_cmp(A, CMP_STR, B) ({				\
	पूर्णांक __res = 0;						\
								\
	अगर (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} अन्यथा अगर (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} अन्यथा अगर (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} अन्यथा अगर (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} अन्यथा {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

बाह्य व्योम __BUILD_BUG_vruntime_op(व्योम);

#घोषणा vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	अगर (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} अन्यथा {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


अटल अंतरभूत __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	अगर (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	वापस max_vruntime;
}

अटल अंतरभूत __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	अगर (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	वापस min_vruntime;
}

अटल अंतरभूत bool entity_before(स्थिर काष्ठा sched_entity *a,
				 स्थिर काष्ठा sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	वापस vruntime_cmp(a->deadline, "<", b->deadline);
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
अटल अंतरभूत s64 entity_कुंजी(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	वापस vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#घोषणा __node_2_se(गाँठ) \
	rb_entry((गाँठ), काष्ठा sched_entity, run_गाँठ)

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
अटल अंतरभूत अचिन्हित दीर्घ avg_vruntime_weight(काष्ठा cfs_rq *cfs_rq, अचिन्हित दीर्घ w)
{
#अगर_घोषित CONFIG_64BIT
	अगर (cfs_rq->योग_shift)
		w = max(2UL, w >> cfs_rq->योग_shift);
#पूर्ण_अगर
	वापस w;
}

अटल अंतरभूत व्योम
__sum_w_vruntime_add(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	अचिन्हित दीर्घ weight = avg_vruntime_weight(cfs_rq, se->h_load.weight);
	s64 w_vruntime, कुंजी = entity_कुंजी(cfs_rq, se);

	w_vruntime = कुंजी * weight;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->योग_w_vruntime += w_vruntime;
	cfs_rq->योग_weight += weight;
}

अटल व्योम
योग_w_vruntime_add_paranoid(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	अचिन्हित दीर्घ weight;
	s64 कुंजी, tmp;

again:
	weight = avg_vruntime_weight(cfs_rq, se->h_load.weight);
	कुंजी = entity_कुंजी(cfs_rq, se);

	अगर (check_mul_overflow(कुंजी, weight, &कुंजी))
		जाओ overflow;

	अगर (check_add_overflow(cfs_rq->योग_w_vruntime, कुंजी, &tmp))
		जाओ overflow;

	cfs_rq->योग_w_vruntime = tmp;
	cfs_rq->योग_weight += weight;
	वापस;

overflow:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->योग_shift >= 10);
	cfs_rq->योग_shift++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->योग_w_vruntime = 0;
	cfs_rq->योग_weight = 0;

	क्रम (काष्ठा rb_गाँठ *गाँठ = cfs_rq->tasks_timeline.rb_leftmost;
	     गाँठ; गाँठ = rb_अग्र(गाँठ))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(गाँठ));

	जाओ again;
}

अटल व्योम
योग_w_vruntime_add(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	अगर (sched_feat(PARANOID_AVG))
		वापस योग_w_vruntime_add_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

अटल व्योम
योग_w_vruntime_sub(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	अचिन्हित दीर्घ weight = avg_vruntime_weight(cfs_rq, se->h_load.weight);
	s64 कुंजी = entity_कुंजी(cfs_rq, se);

	cfs_rq->योग_w_vruntime -= कुंजी * weight;
	cfs_rq->योग_weight -= weight;
}

अटल अंतरभूत
व्योम update_zero_vruntime(काष्ठा cfs_rq *cfs_rq, s64 delta)
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
u64 avg_vruntime(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा sched_entity *curr = cfs_rq->curr;
	दीर्घ weight = cfs_rq->योग_weight;
	s64 delta = 0;

	अगर (curr && !curr->on_rq)
		curr = शून्य;

	अगर (weight) {
		s64 runtime = cfs_rq->योग_w_vruntime;

		अगर (curr) {
			अचिन्हित दीर्घ w = avg_vruntime_weight(cfs_rq, curr->h_load.weight);

			runtime += entity_कुंजी(cfs_rq, curr) * w;
			weight += w;
		}

		/* sign flips effective floor / ceiling */
		अगर (runtime < 0)
			runtime -= (weight - 1);

		delta = div64_long(runtime, weight);
	} अन्यथा अगर (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	वापस cfs_rq->zero_vruntime;
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
अटल u64 ineligible_vruntime(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा sched_entity *curr = cfs_rq->curr;
	दीर्घ weight = cfs_rq->योग_weight;
	s64 delta = 0;

	अगर (curr && !curr->on_rq)
		curr = शून्य;

	अगर (weight) {
		s64 runtime = cfs_rq->योग_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		अगर (runtime < 0)
			runtime -= (weight - 1);

		delta = div64_long(runtime, weight);
	}

	वापस cfs_rq->zero_vruntime + delta + 1;
}

अटल अंतरभूत u64 cfs_rq_max_slice(काष्ठा cfs_rq *cfs_rq);

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
अटल s64 entity_lag(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, u64 avruntime)
{
	u64 max_slice = cfs_rq_max_slice(cfs_rq) + TICK_NSEC;
	s64 vlag, limit;

	vlag = avruntime - se->vruntime;
	limit = calc_delta_fair(max_slice, se);

	वापस clamp(vlag, -limit, limit);
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
अटल __always_inline
bool update_entity_lag(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	अगर (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		अगर (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	वापस avruntime - vlag != se->vruntime;
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
अटल पूर्णांक vruntime_eligible(काष्ठा cfs_rq *cfs_rq, u64 vruntime)
{
	काष्ठा sched_entity *curr = cfs_rq->curr;
	s64 कुंजी, avg = cfs_rq->योग_w_vruntime;
	दीर्घ load = cfs_rq->योग_weight;

	अगर (curr && curr->on_rq) {
		अचिन्हित दीर्घ weight = avg_vruntime_weight(cfs_rq, curr->h_load.weight);

		avg += entity_कुंजी(cfs_rq, curr) * weight;
		load += weight;
	}

	कुंजी = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#अगर_घोषित CONFIG_64BIT
#अगर_घोषित CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	वापस avg >= (__int128)कुंजी * load;
#अन्यथा
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	अगर (check_mul_overflow(कुंजी, load, &rhs))
		वापस कुंजी <= 0;

	वापस avg >= rhs;
#पूर्ण_अगर
#अन्यथा /* 32bit */
	वापस avg >= कुंजी * load;
#पूर्ण_अगर
}

पूर्णांक entity_eligible(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	वापस vruntime_eligible(cfs_rq, se->vruntime);
}

अटल अंतरभूत u64 cfs_rq_min_slice(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा sched_entity *मूल = __pick_root_entity(cfs_rq);
	काष्ठा sched_entity *curr = cfs_rq->curr;
	u64 min_slice = ~0ULL;

	अगर (curr && curr->on_rq)
		min_slice = curr->slice;

	अगर (मूल)
		min_slice = min(min_slice, मूल->min_slice);

	वापस min_slice;
}

अटल अंतरभूत u64 cfs_rq_max_slice(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा sched_entity *मूल = __pick_root_entity(cfs_rq);
	काष्ठा sched_entity *curr = cfs_rq->curr;
	u64 max_slice = 0ULL;

	अगर (curr && curr->on_rq)
		max_slice = curr->slice;

	अगर (मूल)
		max_slice = max(max_slice, मूल->max_slice);

	वापस max_slice;
}

अटल अंतरभूत bool __entity_less(काष्ठा rb_गाँठ *a, स्थिर काष्ठा rb_गाँठ *b)
{
	वापस entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
अटल अंतरभूत u64 __min_vruntime(u64 a, u64 b)
{
	वापस vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(अटल, min_vruntime_cb,
		     काष्ठा sched_entity, run_गाँठ,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(slice, min_slice, min),
		     RB_AUG(slice, max_slice, max));

/*
 * Enqueue an entity into the rb-tree:
 */
अटल व्योम __enqueue_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	WARN_ON_ONCE(&rq_of(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	योग_w_vruntime_add(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_slice = se->slice;
	se->max_slice = se->slice;

	rb_add_augmented_cached(&se->run_गाँठ, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

अटल व्योम __dequeue_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	WARN_ON_ONCE(&rq_of(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->run_गाँठ, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	योग_w_vruntime_sub(cfs_rq, se);
}

काष्ठा sched_entity *__pick_root_entity(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा rb_गाँठ *मूल = cfs_rq->tasks_timeline.rb_मूल.rb_गाँठ;

	अगर (!मूल)
		वापस शून्य;

	वापस __node_2_se(मूल);
}

काष्ठा sched_entity *__pick_first_entity(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा rb_गाँठ *बायाँ = rb_first_cached(&cfs_rq->tasks_timeline);

	अगर (!बायाँ)
		वापस शून्य;

	वापस __node_2_se(बायाँ);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
अटल अंतरभूत व्योम set_protect_slice(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	u64 slice = normalized_sysctl_sched_base_slice;
	u64 vprot = se->deadline;

	अगर (sched_feat(RUN_TO_PARITY))
		slice = cfs_rq_min_slice(cfs_rq);

	slice = min(slice, se->slice);

	/* If there are shorter slices than se's one */
	अगर (slice != se->slice) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(slice, se));
		अगर (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

अटल अंतरभूत व्योम update_protect_slice(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 slice = cfs_rq_min_slice(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(slice, se));

	अगर (sched_feat(PREEMPT_SHORT) && slice != se->slice)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

अटल अंतरभूत bool protect_slice(काष्ठा sched_entity *se)
{
	वापस vruntime_cmp(se->vruntime, "<", se->vprot);
}

अटल अंतरभूत व्योम cancel_protect_slice(काष्ठा sched_entity *se)
{
	अगर (protect_slice(se))
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
अटल काष्ठा sched_entity *pick_eevdf(काष्ठा cfs_rq *cfs_rq, bool protect)
{
	काष्ठा rb_गाँठ *गाँठ = cfs_rq->tasks_timeline.rb_मूल.rb_गाँठ;
	काष्ठा sched_entity *se = __pick_first_entity(cfs_rq);
	काष्ठा sched_entity *curr = cfs_rq->curr;
	काष्ठा sched_entity *best = शून्य;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	अगर (cfs_rq->h_nr_queued == 1)
		वापस curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	अगर (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->अग्र && entity_eligible(cfs_rq, cfs_rq->अग्र)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->अग्र->sched_delayed);
		वापस cfs_rq->अग्र;
	}

	अगर (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = शून्य;

	अगर (curr && protect && protect_slice(curr))
		वापस curr;

	/* Pick the leftmost entity if it's eligible */
	अगर (se && entity_eligible(cfs_rq, se)) {
		best = se;
		जाओ found;
	}

	/* Heap search for the EEVD entity */
	जबतक (गाँठ) {
		काष्ठा rb_गाँठ *बायाँ = गाँठ->rb_बायाँ;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		अगर (बायाँ && vruntime_eligible(cfs_rq,
					__node_2_se(बायाँ)->min_vruntime)) {
			गाँठ = बायाँ;
			जारी;
		}

		se = __node_2_se(गाँठ);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		अगर (entity_eligible(cfs_rq, se)) {
			best = se;
			अवरोध;
		}

		गाँठ = गाँठ->rb_दायाँ;
	}
found:
	अगर (!best || (curr && entity_before(curr, best)))
		best = curr;

	वापस best;
}

काष्ठा sched_entity *__pick_last_entity(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा rb_गाँठ *last = rb_last(&cfs_rq->tasks_timeline.rb_मूल);

	अगर (!last)
		वापस शून्य;

	वापस __node_2_se(last);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
पूर्णांक sched_update_scaling(व्योम)
{
	अचिन्हित पूर्णांक factor = get_update_sysctl_factor();

#घोषणा WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (factor))
	WRT_SYSCTL(sched_base_slice);
#अघोषित WRT_SYSCTL

	वापस 0;
}

अटल व्योम clear_buddies(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
अटल bool update_deadline(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	अगर (vruntime_cmp(se->vruntime, "<", se->deadline))
		वापस false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	अगर (!se->custom_slice)
		se->slice = sysctl_sched_base_slice;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->slice, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	वापस true;
}

#समावेश "pelt.h"

अटल पूर्णांक select_idle_sibling(काष्ठा task_struct *p, पूर्णांक पूर्व_cpu, पूर्णांक cpu);
अटल अचिन्हित दीर्घ task_h_load(काष्ठा task_struct *p);
अटल अचिन्हित दीर्घ capacity_of(पूर्णांक cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
व्योम init_entity_runnable_average(काष्ठा sched_entity *se)
{
	काष्ठा sched_avg *sa = &se->avg;

	स_रखो(sa, 0, माप(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	अगर (entity_is_task(se))
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
व्योम post_init_entity_util_avg(काष्ठा task_struct *p)
{
	काष्ठा sched_entity *se = &p->se;
	काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);
	काष्ठा sched_avg *sa = &se->avg;
	दीर्घ cpu_scale = arch_scale_cpu_capacity(cpu_of(rq_of(cfs_rq)));
	दीर्घ cap = (दीर्घ)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	अगर (p->sched_class != &fair_sched_class) {
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
		वापस;
	}

	अगर (cap > 0) {
		अगर (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_weight(se);
			sa->util_avg /= (cfs_rq->avg.load_avg + 1);

			अगर (sa->util_avg > cap)
				sa->util_avg = cap;
		} अन्यथा {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

अटल अंतरभूत व्योम account_mm_sched(काष्ठा rq *rq, काष्ठा task_struct *p, s64 delta_exec);

अटल s64 update_se(काष्ठा rq *rq, काष्ठा sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_start;
	अगर (unlikely(delta_exec <= 0))
		वापस delta_exec;

	se->exec_start = now;
	अगर (entity_is_task(se)) {
		काष्ठा task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_start = now;
		running->se.योग_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_group_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} अन्यथा {
		/* If not task, account the time against donor se  */
		se->योग_exec_runtime += delta_exec;
	}

	अगर (schedstat_enabled()) {
		काष्ठा sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	वापस delta_exec;
}

#अगर_घोषित CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#घोषणा EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#घोषणा EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly अचिन्हित पूर्णांक llc_aggr_tolerance	= 1;
__read_mostly अचिन्हित पूर्णांक llc_epoch_period	= EPOCH_PERIOD;
__read_mostly अचिन्हित पूर्णांक llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly अचिन्हित पूर्णांक llc_imb_pct		= 20;
__read_mostly अचिन्हित पूर्णांक llc_overaggr_pct	= 50;

अटल पूर्णांक llc_id(पूर्णांक cpu)
{
	अगर (cpu < 0)
		वापस -1;

	वापस per_cpu(sd_llc_id, cpu);
}

अटल अंतरभूत पूर्णांक get_sched_cache_scale(पूर्णांक mul)
{
	अचिन्हित पूर्णांक tol = READ_ONCE(llc_aggr_tolerance);

	अगर (!tol)
		वापस 0;

	अगर (tol >= 100)
		वापस पूर्णांक_उच्च;

	वापस (1 + (tol - 1) * mul);
}

अटल bool exceed_llc_capacity(काष्ठा sched_cache_group *grp, पूर्णांक cpu)
{
#अगर_घोषित CONFIG_NUMA_BALANCING
	अचिन्हित दीर्घ llc, footprint;
	काष्ठा sched_domain *sd;
	पूर्णांक scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	अगर (!sd)
		वापस true;

	अगर (static_branch_likely(&sched_numa_balancing)) {
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
		अगर (scale == पूर्णांक_उच्च)
			वापस false;

		वापस ((llc * (u64)scale) < (footprint * PAGE_आकार));
	}
#पूर्ण_अगर
	वापस false;
}

अटल bool invalid_llc_nr(काष्ठा sched_cache_group *grp, काष्ठा task_struct *p,
			   पूर्णांक cpu)
{
	पूर्णांक scale;

	अगर (get_nr_threads(p) <= 1)
		वापस true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	अगर (scale == पूर्णांक_उच्च)
		वापस false;

	वापस !fits_capacity((READ_ONCE(grp->nr_running_avg) * cpu_smt_अंक_threads),
			(scale * per_cpu(sd_llc_आकार, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
अटल bool task_pref_llc_runnable(काष्ठा task_struct *p)
{
	वापस p->pref_llc_queued && !p->se.sched_delayed;
}

अटल व्योम pref_llc_running_inc(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अगर (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

अटल व्योम pref_llc_running_dec(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अगर (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

अटल व्योम account_llc_enqueue(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	पूर्णांक pref_llc, pref_llc_queued;
	काष्ठा sched_domain *sd;

	pref_llc = p->preferred_llc;
	अगर (pref_llc < 0)
		वापस;

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
	अगर (sd && (अचिन्हित पूर्णांक)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

अटल व्योम account_llc_dequeue(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	काष्ठा sched_domain *sd;
	पूर्णांक pref_llc;

	pref_llc = p->preferred_llc;
	अगर (pref_llc < 0)
		वापस;

	rq->nr_llc_running--;
	अगर (p->pref_llc_queued) {
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
	अगर (sd && (अचिन्हित पूर्णांक)pref_llc < sd->llc_max) {
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
		अगर (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

पूर्णांक mm_init_sched(काष्ठा mm_struct *mm,
		  काष्ठा sched_cache_time __percpu *_pcpu_sched)
{
	काष्ठा sched_cache_group *grp;
	अचिन्हित दीर्घ epoch = 0;
	पूर्णांक i;

	grp = kzalloc_obj(*grp);
	अगर (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = शून्य;
		वापस -ENOMEM;
	}

	for_each_possible_cpu(i) {
		काष्ठा sched_cache_time *pcpu_sched = per_cpu_ptr(_pcpu_sched, i);
		काष्ठा rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_lock_init(&grp->lock);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->अग्र_scan = jiffies;
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
	वापस 0;
}

अटल व्योम sched_cache_group_free_rcu(काष्ठा rcu_शीर्ष *rcu)
{
	काष्ठा sched_cache_group *grp =
		container_of(rcu, काष्ठा sched_cache_group, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

अटल व्योम sched_cache_group_put(काष्ठा sched_cache_group *grp)
{
	अगर (!grp || !refcount_dec_and_test(&grp->refcnt))
		वापस;

	call_rcu(&grp->rcu, sched_cache_group_free_rcu);
}

DEFINE_FREE(sched_cache_group_put, काष्ठा sched_cache_group *,
	    sched_cache_group_put(_T));

#घोषणा rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

अटल काष्ठा sched_cache_group *sched_cache_replace_grp(काष्ठा task_struct *p,
							 काष्ठा sched_cache_group *new)
{
	काष्ठा sched_cache_group *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_assign_pointer(p->sched_cache_grp, new);

	वापस old;
}

काष्ठा sched_cache_group *sched_cache_group_get(काष्ठा sched_cache_group *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	अगर (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = शून्य;

	वापस grp;
}

काष्ठा sched_cache_group *task_cache_group_get(काष्ठा task_struct *p)
{
	guard(rcu)();
	वापस sched_cache_group_get(rcu_dereference(p->sched_cache_grp));
}

व्योम sched_cache_fork(काष्ठा task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_POINTER(p->sched_cache_grp,
			 sched_cache_group_get(p->mm->sched_cache_grp));
}

व्योम sched_cache_fork_cleanup(काष्ठा task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_group_put(rcu_access_pointer(p->sched_cache_grp));
	RCU_INIT_POINTER(p->sched_cache_grp, शून्य);
}

व्योम sched_cache_exec_mmap(काष्ठा task_struct *p, काष्ठा mm_struct *mm)
{
	काष्ठा sched_cache_group *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_group_get(mm->sched_cache_grp));
	sched_cache_group_put(old);
}

व्योम sched_cache_exit_mm(काष्ठा task_struct *p)
{
	काष्ठा sched_cache_group *grp = sched_cache_replace_grp(p, शून्य);

#अगर_घोषित CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	अगर (grp && p->योग_numa_faults) {
		अचिन्हित दीर्घ fp = READ_ONCE(grp->footprint);
		अचिन्हित दीर्घ sub = min(fp, p->योग_numa_faults);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#पूर्ण_अगर
	sched_cache_group_put(grp);
}

व्योम mm_destroy_sched(काष्ठा mm_struct *mm)
{
	sched_cache_group_put(mm->sched_cache_grp);
	mm->sched_cache_grp = शून्य;
}

/* because why would C be fully specified */
अटल __always_inline व्योम __shr_u64(u64 *val, अचिन्हित पूर्णांक n)
{
	अगर (n >= 64) {
		*val = 0;
		वापस;
	}
	*val >>= n;
}

अटल अंतरभूत व्योम __update_mm_sched(काष्ठा rq *rq,
				     काष्ठा sched_cache_time *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_lock);

	अचिन्हित पूर्णांक period = max(READ_ONCE(llc_epoch_period), 1U);
	अचिन्हित दीर्घ n, now = jiffies;
	दीर्घ delta = now - rq->cpu_epoch_अग्र;

	अगर (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_अग्र += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	अगर (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

अटल अचिन्हित दीर्घ fraction_mm_sched(काष्ठा rq *rq,
				       काष्ठा sched_cache_time *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_lock);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	वापस div64_u64(NICE_0_LOAD * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

अटल पूर्णांक get_pref_llc(काष्ठा task_struct *p, काष्ठा sched_cache_group *grp)
{
	पूर्णांक mm_sched_llc = -1, mm_sched_cpu;

	अगर (!grp)
		वापस -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	अगर (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#अगर_घोषित CONFIG_NUMA_BALANCING
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
		अगर (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_to_गाँठ(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#पूर्ण_अगर
	}

	वापस mm_sched_llc;
}

अटल अचिन्हित पूर्णांक task_running_on_cpu(पूर्णांक cpu, काष्ठा task_struct *p);

अटल अंतरभूत
व्योम account_mm_sched(काष्ठा rq *rq, काष्ठा task_struct *p, s64 delta_exec)
{
	काष्ठा sched_cache_group *grp = rcu_dereference_all(p->sched_cache_grp);
	काष्ठा sched_cache_time *pcpu_sched;
	पूर्णांक mm_sched_llc = -1;
	अचिन्हित दीर्घ epoch;

	अगर (!sched_cache_enabled())
		वापस;

	अगर (p->sched_class != &fair_sched_class)
		वापस;
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
	अगर (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		वापस;

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
	अगर ((दीर्घ)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_of(rq)) ||
	    exceed_llc_capacity(grp, cpu_of(rq))) {
		अगर (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	अगर (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_dequeue(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_enqueue(rq, p);
	}
}

अटल व्योम task_tick_cache(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	काष्ठा sched_cache_group *grp = rcu_dereference_all(p->sched_cache_grp);
	काष्ठा callback_शीर्ष *work = &p->cache_work;
	अचिन्हित दीर्घ epoch;

	अगर (!sched_cache_enabled())
		वापस;

	अगर (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		वापस;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	अगर (time_after_eq(grp->epoch, epoch))
		वापस;

	guard(raw_spinlock)(&grp->lock);

	अगर (work->अग्र == work) {
		task_work_add(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

अटल व्योम get_scan_cpumasks(cpumask_var_t cpus, काष्ठा task_struct *p,
			      काष्ठा sched_cache_group *grp)
{
#अगर_घोषित CONFIG_NUMA_BALANCING
	पूर्णांक cpu, curr_cpu, nid, pref_nid;

	अगर (!static_branch_likely(&sched_numa_balancing))
		जाओ out;

	cpu = READ_ONCE(grp->cpu);
	अगर (cpu != -1)
		nid = cpu_to_गाँठ(cpu);
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
	अगर (pref_nid == NUMA_NO_गाँठ)
		जाओ out;

	cpumask_or(cpus, cpus, cpumask_of_गाँठ(pref_nid));

	/* honor the task's preferred LLC CPU */
	अगर (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_गाँठ)
		cpumask_or(cpus, cpus, cpumask_of_गाँठ(nid));

	/* make sure the task's current running node is included */
	अगर (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_of_गाँठ(cpu_to_गाँठ(curr_cpu)));

	वापस;

out:
#पूर्ण_अगर
	cpumask_copy(cpus, cpu_online_mask);
}

अटल अंतरभूत व्योम update_avg_scale(u64 *avg, u64 sample)
{
	पूर्णांक factor = per_cpu(sd_llc_आकार, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 भाजक;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	भाजक = clamp_t(u32, (factor >> 2), 2, 8);
	*avg += div64_s64(diff, भाजक);
}

अटल व्योम task_cache_work(काष्ठा callback_शीर्ष *work)
{
	काष्ठा sched_cache_group *grp __free(sched_cache_group_put) = शून्य;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	पूर्णांक cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	अचिन्हित दीर्घ अग्र_scan, now = jiffies;
	काष्ठा task_struct *p = current, *cur;
	अचिन्हित दीर्घ curr_m_a_occ = 0;
	अचिन्हित दीर्घ m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->अग्र = work;

	अगर (p->flags & PF_EXITING)
		वापस;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_group_get(p);
	अगर (!grp)
		वापस;

	अग्र_scan = READ_ONCE(grp->अग्र_scan);
	अगर (time_before(now, अग्र_scan))
		वापस;

	/* only 1 thread is allowed to scan */
	अगर (!try_cmpxchg(&grp->अग्र_scan, &अग्र_scan,
			 now + max_t(अचिन्हित दीर्घ,
				     READ_ONCE(llc_epoch_period), 1)))
		वापस;

	curr_cpu = task_cpu(p);
	अगर (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_capacity(grp, curr_cpu)) {
		अगर (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		वापस;
	}

	अगर (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		वापस;

	scoped_guard (cpus_read_lock) {
		guard(rcu)();

		get_scan_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			काष्ठा sched_domain *sd = rcu_dereference_all(per_cpu(sd_llc, cpu));
			अचिन्हित दीर्घ occ, m_occ = 0, a_occ = 0;
			पूर्णांक m_cpu = -1, i;

			अगर (!sd)
				जारी;

			for_each_cpu(i, sched_domain_span(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							per_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				अगर (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				अगर (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
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
			अगर (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			अगर (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_span(sd));
		}
	}

	अगर (m_a_occ > (2 * curr_m_a_occ)) {
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

व्योम init_sched_mm(काष्ठा task_struct *p)
{
	काष्ठा callback_शीर्ष *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->अग्र = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_POINTER(p->sched_cache_grp, शून्य);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#अन्यथा /* CONFIG_SCHED_CACHE */

अटल अंतरभूत व्योम account_mm_sched(काष्ठा rq *rq, काष्ठा task_struct *p,
				    s64 delta_exec) { }

व्योम init_sched_mm(काष्ठा task_struct *p) { }

अटल व्योम task_tick_cache(काष्ठा rq *rq, काष्ठा task_struct *p) { }

अटल अंतरभूत पूर्णांक get_pref_llc(काष्ठा task_struct *p,
			       काष्ठा mm_struct *mm)
{
	वापस -1;
}

अटल व्योम account_llc_enqueue(काष्ठा rq *rq, काष्ठा task_struct *p) {}

अटल व्योम account_llc_dequeue(काष्ठा rq *rq, काष्ठा task_struct *p) {}

अटल व्योम pref_llc_running_inc(काष्ठा rq *rq, काष्ठा task_struct *p) {}

अटल व्योम pref_llc_running_dec(काष्ठा rq *rq, काष्ठा task_struct *p) {}

#पूर्ण_अगर /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(काष्ठा rq *rq)
{
	वापस update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
अटल व्योम update_curr(काष्ठा cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	काष्ठा sched_entity *curr = cfs_rq->h_curr;
	काष्ठा rq *rq = rq_of(cfs_rq);
	s64 delta_exec;
	bool resched;

	अगर (unlikely(!curr))
		वापस;

	delta_exec = update_se(rq, curr);
	अगर (unlikely(delta_exec <= 0))
		वापस;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	अगर (!entity_is_task(curr))
		वापस;

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

	अगर (cfs_rq->h_nr_queued == 1)
		वापस;

	अगर (resched || !protect_slice(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

अटल व्योम update_curr_fair(काष्ठा rq *rq)
{
	काष्ठा sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_of(se));
}

अटल अंतरभूत व्योम
update_stats_wait_start_fair(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	काष्ठा sched_statistics *stats;
	काष्ठा task_struct *p = शून्य;

	अगर (!schedstat_enabled())
		वापस;

	stats = __schedstats_from_se(se);

	अगर (entity_is_task(se))
		p = task_of(se);

	__update_stats_wait_start(rq_of(cfs_rq), p, stats);
}

अटल अंतरभूत व्योम
update_stats_wait_end_fair(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	काष्ठा sched_statistics *stats;
	काष्ठा task_struct *p = शून्य;

	अगर (!schedstat_enabled())
		वापस;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	अगर (unlikely(!schedstat_val(stats->wait_start)))
		वापस;

	अगर (entity_is_task(se))
		p = task_of(se);

	__update_stats_wait_end(rq_of(cfs_rq), p, stats);
}

अटल अंतरभूत व्योम
update_stats_enqueue_sleeper_fair(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	काष्ठा sched_statistics *stats;
	काष्ठा task_struct *tsk = शून्य;

	अगर (!schedstat_enabled())
		वापस;

	stats = __schedstats_from_se(se);

	अगर (entity_is_task(se))
		tsk = task_of(se);

	__update_stats_enqueue_sleeper(rq_of(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
अटल अंतरभूत व्योम
update_stats_enqueue_fair(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, पूर्णांक flags)
{
	अगर (!schedstat_enabled())
		वापस;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	अगर (se != cfs_rq->h_curr)
		update_stats_wait_start_fair(cfs_rq, se);

	अगर (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_fair(cfs_rq, se);
}

अटल अंतरभूत व्योम
update_stats_dequeue_fair(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, पूर्णांक flags)
{

	अगर (!schedstat_enabled())
		वापस;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	अगर (se != cfs_rq->h_curr)
		update_stats_wait_end_fair(cfs_rq, se);

	अगर ((flags & DEQUEUE_SLEEP) && entity_is_task(se)) {
		काष्ठा task_struct *tsk = task_of(se);
		अचिन्हित पूर्णांक state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		अगर (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_start,
				      rq_clock(rq_of(cfs_rq)));
		अगर (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.block_start,
				      rq_clock(rq_of(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
अटल अंतरभूत व्योम
update_stats_curr_start(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_start = rq_clock_task(rq_of(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
अटल अंतरभूत bool is_core_idle(पूर्णांक cpu)
{
	पूर्णांक sibling;

	for_each_cpu(sibling, cpu_smt_mask(cpu)) {
		अगर (cpu == sibling)
			जारी;

		अगर (!idle_cpu(sibling))
			वापस false;
	}

	वापस true;
}

#अगर_घोषित CONFIG_NUMA
#घोषणा NUMA_IMBALANCE_MIN 2

अटल अंतरभूत दीर्घ
adjust_numa_imbalance(पूर्णांक imbalance, पूर्णांक dst_running, पूर्णांक imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	अगर (dst_running > imb_numa_nr)
		वापस imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	अगर (imbalance <= NUMA_IMBALANCE_MIN)
		वापस 0;

	वापस imbalance;
}
#पूर्ण_अगर /* CONFIG_NUMA */

#अगर_घोषित CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
अचिन्हित पूर्णांक sysctl_numa_balancing_scan_period_min = 1000;
अचिन्हित पूर्णांक sysctl_numa_balancing_scan_period_max = 60000;

/* Portion of address space to scan in MB */
अचिन्हित पूर्णांक sysctl_numa_balancing_scan_आकार = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
अचिन्हित पूर्णांक sysctl_numa_balancing_scan_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
अचिन्हित पूर्णांक sysctl_numa_balancing_hot_threshold = MSEC_PER_SEC;

काष्ठा numa_group {
	refcount_t refcount;

	spinlock_t lock; /* nr_tasks, tasks */
	पूर्णांक nr_tasks;
	pid_t gid;
	पूर्णांक active_nodes;

	काष्ठा rcu_शीर्ष rcu;
	अचिन्हित दीर्घ योग_faults;
	अचिन्हित दीर्घ max_faults_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	अचिन्हित दीर्घ faults[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
अटल काष्ठा numa_group *deref_task_numa_group(काष्ठा task_struct *p)
{
	वापस rcu_dereference_check(p->numa_group, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

अटल काष्ठा numa_group *deref_curr_numa_group(काष्ठा task_struct *p)
{
	वापस rcu_dereference_protected(p->numa_group, p == current);
}

अटल अंतरभूत अचिन्हित दीर्घ group_faults_priv(काष्ठा numa_group *ng);
अटल अंतरभूत अचिन्हित दीर्घ group_faults_shared(काष्ठा numa_group *ng);

अटल अचिन्हित पूर्णांक task_nr_scan_windows(काष्ठा task_struct *p)
{
	अचिन्हित दीर्घ rss = 0;
	अचिन्हित दीर्घ nr_scan_pages;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_scan_pages = MB_TO_PAGES(sysctl_numa_balancing_scan_आकार);
	rss = get_mm_rss(p->mm);
	अगर (!rss)
		rss = nr_scan_pages;

	rss = round_up(rss, nr_scan_pages);
	वापस rss / nr_scan_pages;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#घोषणा MAX_SCAN_WINDOW 2560

अटल अचिन्हित पूर्णांक task_scan_min(काष्ठा task_struct *p)
{
	अचिन्हित पूर्णांक scan_आकार = READ_ONCE(sysctl_numa_balancing_scan_आकार);
	अचिन्हित पूर्णांक scan, न्यूनमान;
	अचिन्हित पूर्णांक windows = 1;

	अगर (scan_आकार < MAX_SCAN_WINDOW)
		windows = MAX_SCAN_WINDOW / scan_आकार;
	न्यूनमान = 1000 / windows;

	scan = sysctl_numa_balancing_scan_period_min / task_nr_scan_windows(p);
	वापस max_t(अचिन्हित पूर्णांक, न्यूनमान, scan);
}

अटल अचिन्हित पूर्णांक task_scan_start(काष्ठा task_struct *p)
{
	अचिन्हित दीर्घ smin = task_scan_min(p);
	अचिन्हित दीर्घ period = smin;
	काष्ठा numa_group *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_lock();
	ng = rcu_dereference_all(p->numa_group);
	अगर (ng) {
		अचिन्हित दीर्घ shared = group_faults_shared(ng);
		अचिन्हित दीर्घ निजी = group_faults_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= निजी + shared + 1;
	}
	rcu_read_unlock();

	वापस max(smin, period);
}

अटल अचिन्हित पूर्णांक task_scan_max(काष्ठा task_struct *p)
{
	अचिन्हित दीर्घ smin = task_scan_min(p);
	अचिन्हित दीर्घ smax;
	काष्ठा numa_group *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_scan_period_max / task_nr_scan_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_group(p);
	अगर (ng) {
		अचिन्हित दीर्घ shared = group_faults_shared(ng);
		अचिन्हित दीर्घ निजी = group_faults_priv(ng);
		अचिन्हित दीर्घ period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= निजी + shared + 1;

		smax = max(smax, period);
	}

	वापस max(smin, smax);
}

अटल व्योम account_numa_enqueue(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_गाँठ);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_गाँठ(p));
}

अटल व्योम account_numa_dequeue(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_गाँठ);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_गाँठ(p));
}

/* Shared or private faults. */
#घोषणा NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#घोषणा NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#घोषणा NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_group_id(काष्ठा task_struct *p)
{
	काष्ठा numa_group *ng;
	pid_t gid = 0;

	rcu_read_lock();
	ng = rcu_dereference_all(p->numa_group);
	अगर (ng)
		gid = ng->gid;
	rcu_read_unlock();

	वापस gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
अटल अंतरभूत पूर्णांक task_faults_idx(क्रमागत numa_faults_stats s, पूर्णांक nid, पूर्णांक priv)
{
	वापस NR_NUMA_HINT_FAULT_TYPES * (s * nr_गाँठ_ids + nid) + priv;
}

अटल अंतरभूत अचिन्हित दीर्घ task_faults(काष्ठा task_struct *p, पूर्णांक nid)
{
	अगर (!p->numa_faults)
		वापस 0;

	वापस p->numa_faults[task_faults_idx(NUMA_MEM, nid, 0)] +
		p->numa_faults[task_faults_idx(NUMA_MEM, nid, 1)];
}

अटल अंतरभूत अचिन्हित दीर्घ group_faults(काष्ठा task_struct *p, पूर्णांक nid)
{
	काष्ठा numa_group *ng = deref_task_numa_group(p);

	अगर (!ng)
		वापस 0;

	वापस ng->faults[task_faults_idx(NUMA_MEM, nid, 0)] +
		ng->faults[task_faults_idx(NUMA_MEM, nid, 1)];
}

अटल अंतरभूत अचिन्हित दीर्घ group_faults_cpu(काष्ठा numa_group *group, पूर्णांक nid)
{
	वापस group->faults[task_faults_idx(NUMA_CPU, nid, 0)] +
		group->faults[task_faults_idx(NUMA_CPU, nid, 1)];
}

अटल अंतरभूत अचिन्हित दीर्घ group_faults_priv(काष्ठा numa_group *ng)
{
	अचिन्हित दीर्घ faults = 0;
	पूर्णांक गाँठ;

	for_each_online_गाँठ(गाँठ) {
		faults += ng->faults[task_faults_idx(NUMA_MEM, गाँठ, 1)];
	}

	वापस faults;
}

अटल अंतरभूत अचिन्हित दीर्घ group_faults_shared(काष्ठा numa_group *ng)
{
	अचिन्हित दीर्घ faults = 0;
	पूर्णांक गाँठ;

	for_each_online_गाँठ(गाँठ) {
		faults += ng->faults[task_faults_idx(NUMA_MEM, गाँठ, 0)];
	}

	वापस faults;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#घोषणा ACTIVE_गाँठ_FRACTION 3

अटल bool numa_is_active_गाँठ(पूर्णांक nid, काष्ठा numa_group *ng)
{
	वापस group_faults_cpu(ng, nid) * ACTIVE_गाँठ_FRACTION > ng->max_faults_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
अटल अचिन्हित दीर्घ score_nearby_nodes(काष्ठा task_struct *p, पूर्णांक nid,
					पूर्णांक lim_dist, bool task)
{
	अचिन्हित दीर्घ score = 0;
	पूर्णांक गाँठ, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	अगर (sched_numa_topology_type == NUMA_DIRECT)
		वापस 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_distance);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_गाँठ(गाँठ) {
		अचिन्हित दीर्घ faults;
		पूर्णांक dist = गाँठ_distance(nid, गाँठ);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		अगर (dist >= max_dist || गाँठ == nid)
			जारी;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		अगर (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			जारी;

		/* Add up the faults from nearby nodes. */
		अगर (task)
			faults = task_faults(p, गाँठ);
		अन्यथा
			faults = group_faults(p, गाँठ);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		अगर (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			faults *= (max_dist - dist);
			faults /= (max_dist - LOCAL_DISTANCE);
		}

		score += faults;
	}

	वापस score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
अटल अंतरभूत अचिन्हित दीर्घ task_weight(काष्ठा task_struct *p, पूर्णांक nid,
					पूर्णांक dist)
{
	अचिन्हित दीर्घ faults, योग_faults;

	अगर (!p->numa_faults)
		वापस 0;

	योग_faults = p->योग_numa_faults;

	अगर (!योग_faults)
		वापस 0;

	faults = task_faults(p, nid);
	faults += score_nearby_nodes(p, nid, dist, true);

	वापस 1000 * faults / योग_faults;
}

अटल अंतरभूत अचिन्हित दीर्घ group_weight(काष्ठा task_struct *p, पूर्णांक nid,
					 पूर्णांक dist)
{
	काष्ठा numa_group *ng = deref_task_numa_group(p);
	अचिन्हित दीर्घ faults, योग_faults;

	अगर (!ng)
		वापस 0;

	योग_faults = ng->योग_faults;

	अगर (!योग_faults)
		वापस 0;

	faults = group_faults(p, nid);
	faults += score_nearby_nodes(p, nid, dist, false);

	वापस 1000 * faults / योग_faults;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
अटल अंतरभूत bool cpupid_valid(पूर्णांक cpupid)
{
	वापस cpupid_to_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
अटल bool pgdat_free_space_enough(काष्ठा pglist_तथ्य *pgdat)
{
	पूर्णांक z;
	अचिन्हित दीर्घ enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> PAGE_SHIFT,
			   pgdat->गाँठ_present_pages >> 4);
	क्रम (z = pgdat->nr_zones - 1; z >= 0; z--) {
		काष्ठा zone *zone = pgdat->गाँठ_zones + z;

		अगर (!populated_zone(zone))
			जारी;

		अगर (zone_watermark_ok(zone, 0,
				      promo_wmark_pages(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			वापस true;
	}
	वापस false;
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
अटल पूर्णांक numa_hint_fault_latency(काष्ठा folio *folio)
{
	पूर्णांक last_time, समय;

	समय = jiffies_to_msecs(jiffies);
	last_time = folio_xchg_access_time(folio, समय);

	वापस (समय - last_time) & PAGE_ACCESS_TIME_MASK;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
अटल bool numa_promotion_rate_limit(काष्ठा pglist_तथ्य *pgdat,
				      अचिन्हित दीर्घ rate_limit, पूर्णांक nr)
{
	अचिन्हित दीर्घ nr_cand;
	अचिन्हित पूर्णांक now, start;

	now = jiffies_to_msecs(jiffies);
	mod_गाँठ_page_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = गाँठ_page_state(pgdat, PGPROMOTE_CANDIDATE);
	start = pgdat->nbp_rl_start;
	अगर (now - start > MSEC_PER_SEC &&
	    cmpxchg(&pgdat->nbp_rl_start, start, now) == start)
		pgdat->nbp_rl_nr_cand = nr_cand;
	अगर (nr_cand - pgdat->nbp_rl_nr_cand >= rate_limit)
		वापस true;
	वापस false;
}

#घोषणा NUMA_MIGRATION_ADJUST_STEPS	16

अटल व्योम numa_promotion_adjust_threshold(काष्ठा pglist_तथ्य *pgdat,
					    अचिन्हित दीर्घ rate_limit,
					    अचिन्हित पूर्णांक ref_th)
{
	अचिन्हित पूर्णांक now, start, th_period, unit_th, th;
	अचिन्हित दीर्घ nr_cand, ref_cand, diff_cand;

	now = jiffies_to_msecs(jiffies);
	th_period = sysctl_numa_balancing_scan_period_max;
	start = pgdat->nbp_th_start;
	अगर (now - start > th_period &&
	    cmpxchg(&pgdat->nbp_th_start, start, now) == start) {
		ref_cand = rate_limit *
			sysctl_numa_balancing_scan_period_max / MSEC_PER_SEC;
		nr_cand = गाँठ_page_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_STEPS;
		th = pgdat->nbp_threshold ? : ref_th;
		अगर (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		अन्यथा अगर (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_threshold = th;
	}
}

bool should_numa_migrate_memory(काष्ठा task_struct *p, काष्ठा folio *folio,
				पूर्णांक src_nid, पूर्णांक dst_cpu)
{
	काष्ठा numa_group *ng = deref_curr_numa_group(p);
	पूर्णांक dst_nid = cpu_to_गाँठ(dst_cpu);
	पूर्णांक last_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	अगर (!गाँठ_state(dst_nid, N_MEMORY))
		वापस false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	अगर (folio_use_access_time(folio)) {
		काष्ठा pglist_तथ्य *pgdat;
		अचिन्हित दीर्घ rate_limit;
		अचिन्हित पूर्णांक latency, th, def_th;
		दीर्घ nr = folio_nr_pages(folio);

		pgdat = गाँठ_तथ्य(dst_nid);
		अगर (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_threshold = 0;
			mod_गाँठ_page_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			वापस true;
		}

		def_th = sysctl_numa_balancing_hot_threshold;
		rate_limit = MB_TO_PAGES(sysctl_numa_balancing_promote_rate_limit);
		numa_promotion_adjust_threshold(pgdat, rate_limit, def_th);

		th = pgdat->nbp_threshold ? : def_th;
		latency = numa_hint_fault_latency(folio);
		अगर (latency >= th)
			वापस false;

		वापस !numa_promotion_rate_limit(pgdat, rate_limit, nr);
	}

	this_cpupid = cpu_pid_to_cpupid(dst_cpu, current->pid);
	last_cpupid = folio_xchg_last_cpupid(folio, this_cpupid);

	अगर (!(sysctl_numa_balancing_mode & NUMA_BALANCING_MEMORY_TIERING) &&
	    !गाँठ_is_toptier(src_nid) && !cpupid_valid(last_cpupid))
		वापस false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	अगर ((p->numa_preferred_nid == NUMA_NO_गाँठ || p->numa_scan_seq <= 4) &&
	    (cpupid_pid_unset(last_cpupid) || cpupid_match_pid(p, last_cpupid)))
		वापस true;

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
	अगर (!cpupid_pid_unset(last_cpupid) &&
				cpupid_to_nid(last_cpupid) != dst_nid)
		वापस false;

	/* Always allow migrate on private faults */
	अगर (cpupid_match_pid(p, last_cpupid))
		वापस true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	अगर (!ng)
		वापस true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	अगर (group_faults_cpu(ng, dst_nid) > group_faults_cpu(ng, src_nid) *
					ACTIVE_गाँठ_FRACTION)
		वापस true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	वापस group_faults_cpu(ng, dst_nid) * group_faults(p, src_nid) * 3 >
	       group_faults_cpu(ng, src_nid) * group_faults(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
क्रमागत numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	गाँठ_has_spare = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	गाँठ_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	गाँठ_overloaded
};

/* Cached statistics for all CPUs within a node */
काष्ठा numa_stats {
	अचिन्हित दीर्घ load;
	अचिन्हित दीर्घ runnable;
	अचिन्हित दीर्घ util;
	/* Total compute capacity of CPUs on a node */
	अचिन्हित दीर्घ compute_capacity;
	अचिन्हित पूर्णांक nr_running;
	अचिन्हित पूर्णांक weight;
	क्रमागत numa_type गाँठ_type;
	पूर्णांक idle_cpu;
};

काष्ठा task_numa_env {
	काष्ठा task_struct *p;

	पूर्णांक src_cpu, src_nid;
	पूर्णांक dst_cpu, dst_nid;
	पूर्णांक imb_numa_nr;

	काष्ठा numa_stats src_stats, dst_stats;

	पूर्णांक imbalance_pct;
	पूर्णांक dist;

	काष्ठा task_struct *best_task;
	दीर्घ best_imp;
	पूर्णांक best_cpu;
};

अटल अचिन्हित दीर्घ cpu_load(काष्ठा rq *rq);
अटल अचिन्हित दीर्घ cpu_runnable(काष्ठा rq *rq);

अटल अंतरभूत क्रमागत
numa_type numa_classify(अचिन्हित पूर्णांक imbalance_pct,
			 काष्ठा numa_stats *ns)
{
	अगर ((ns->nr_running > ns->weight) &&
	    (((ns->compute_capacity * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_capacity * imbalance_pct) < (ns->runnable * 100))))
		वापस गाँठ_overloaded;

	अगर ((ns->nr_running < ns->weight) ||
	    (((ns->compute_capacity * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_capacity * imbalance_pct) > (ns->runnable * 100))))
		वापस गाँठ_has_spare;

	वापस गाँठ_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
अटल अंतरभूत bool test_idle_cores(पूर्णांक cpu);
अटल अंतरभूत पूर्णांक numa_idle_core(पूर्णांक idle_core, पूर्णांक cpu)
{
	अगर (!sched_smt_active() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		वापस idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	अगर (is_core_idle(cpu))
		idle_core = cpu;

	वापस idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
अटल व्योम update_numa_stats(काष्ठा task_numa_env *env,
			      काष्ठा numa_stats *ns, पूर्णांक nid,
			      bool find_idle)
{
	पूर्णांक cpu, idle_core = -1;

	स_रखो(ns, 0, माप(*ns));
	ns->idle_cpu = -1;

	rcu_read_lock();
	for_each_cpu(cpu, cpumask_of_गाँठ(nid)) {
		काष्ठा rq *rq = cpu_rq(cpu);

		ns->load += cpu_load(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_capacity += capacity_of(cpu);

		अगर (find_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			अगर (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				जारी;

			अगर (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->weight = cpumask_weight(cpumask_of_गाँठ(nid));

	ns->गाँठ_type = numa_classify(env->imbalance_pct, ns);

	अगर (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

अटल व्योम task_numa_assign(काष्ठा task_numa_env *env,
			     काष्ठा task_struct *p, दीर्घ imp)
{
	काष्ठा rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	अगर (env->best_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		पूर्णांक cpu;
		पूर्णांक start = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_of_गाँठ(env->dst_nid), start + 1) {
			अगर (cpu == env->best_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				जारी;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			अगर (!xchg(&rq->numa_migrate_on, 1))
				जाओ assign;
		}

		/* Failed to find an alternative idle CPU */
		वापस;
	}

assign:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	अगर (env->best_cpu != -1 && env->best_cpu != env->dst_cpu) {
		rq = cpu_rq(env->best_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	अगर (env->best_task)
		put_task_struct(env->best_task);
	अगर (p)
		get_task_struct(p);

	env->best_task = p;
	env->best_imp = imp;
	env->best_cpu = env->dst_cpu;
}

अटल bool load_too_imbalanced(दीर्घ src_load, दीर्घ dst_load,
				काष्ठा task_numa_env *env)
{
	दीर्घ imb, old_imb;
	दीर्घ orig_src_load, orig_dst_load;
	दीर्घ src_capacity, dst_capacity;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_capacity = env->src_stats.compute_capacity;
	dst_capacity = env->dst_stats.compute_capacity;

	imb = असल(dst_load * src_capacity - src_load * dst_capacity);

	orig_src_load = env->src_stats.load;
	orig_dst_load = env->dst_stats.load;

	old_imb = असल(orig_dst_load * src_capacity - orig_src_load * dst_capacity);

	/* Would this change make things worse? */
	वापस (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#घोषणा SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
अटल bool task_numa_compare(काष्ठा task_numa_env *env,
			      दीर्घ taskimp, दीर्घ groupimp, bool maymove)
{
	काष्ठा numa_group *cur_ng, *p_ng = deref_curr_numa_group(env->p);
	काष्ठा rq *dst_rq = cpu_rq(env->dst_cpu);
	दीर्घ imp = p_ng ? groupimp : taskimp;
	काष्ठा task_struct *cur;
	दीर्घ src_load, dst_load;
	पूर्णांक dist = env->dist;
	दीर्घ moveimp = imp;
	दीर्घ load;
	bool stopsearch = false;

	अगर (READ_ONCE(dst_rq->numa_migrate_on))
		वापस false;

	rcu_read_lock();
	cur = rcu_dereference_all(dst_rq->curr);
	अगर (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = शून्य;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	अगर (cur == env->p) {
		stopsearch = true;
		जाओ unlock;
	}

	अगर (!cur) {
		अगर (maymove && moveimp >= env->best_imp)
			जाओ assign;
		अन्यथा
			जाओ unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	अगर (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		जाओ unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	अगर (env->best_task &&
	    env->best_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		जाओ unlock;
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
	अगर (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		अगर (env->dst_stats.गाँठ_type == गाँठ_has_spare)
			जाओ unlock;

		imp = taskimp + task_weight(cur, env->src_nid, dist) -
		      task_weight(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		अगर (cur_ng)
			imp -= imp / 16;
	} अन्यथा {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		अगर (cur_ng && p_ng)
			imp += group_weight(cur, env->src_nid, dist) -
			       group_weight(cur, env->dst_nid, dist);
		अन्यथा
			imp += task_weight(cur, env->src_nid, dist) -
			       task_weight(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	अगर (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	अगर (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	अगर (maymove && moveimp > imp && moveimp > env->best_imp) {
		imp = moveimp;
		cur = शून्य;
		जाओ assign;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	अगर (env->best_task && cur->numa_preferred_nid == env->src_nid &&
	    env->best_task->numa_preferred_nid != env->src_nid) {
		जाओ assign;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	अगर (imp < SMALLIMP || imp <= env->best_imp + SMALLIMP / 2)
		जाओ unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	load = task_h_load(env->p) - task_h_load(cur);
	अगर (!load)
		जाओ assign;

	dst_load = env->dst_stats.load + load;
	src_load = env->src_stats.load - load;

	अगर (load_too_imbalanced(src_load, dst_load, env))
		जाओ unlock;

assign:
	/* Evaluate an idle CPU for a task numa move. */
	अगर (!cur) {
		पूर्णांक cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		अगर (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		अगर (!idle_cpu(cpu) && env->best_cpu >= 0 &&
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
	अगर (maymove && !cur && env->best_cpu >= 0 && idle_cpu(env->best_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	अगर (!maymove && env->best_task &&
	    env->best_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	वापस stopsearch;
}

अटल व्योम task_numa_find_cpu(काष्ठा task_numa_env *env,
				दीर्घ taskimp, दीर्घ groupimp)
{
	bool maymove = false;
	पूर्णांक cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	अगर (env->dst_stats.गाँठ_type == गाँठ_has_spare) {
		अचिन्हित पूर्णांक imbalance;
		पूर्णांक src_running, dst_running;

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
		अगर (!imbalance) {
			maymove = true;
			अगर (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_assign(env, शून्य, 0);
				वापस;
			}
		}
	} अन्यथा {
		दीर्घ src_load, dst_load, load;
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
	for_each_cpu_and(cpu, cpumask_of_गाँठ(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		अगर (task_numa_compare(env, taskimp, groupimp, maymove))
			अवरोध;
	}
}

अटल पूर्णांक task_numa_migrate(काष्ठा task_struct *p)
{
	काष्ठा task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_गाँठ(p),

		.imbalance_pct = 112,

		.best_task = शून्य,
		.best_imp = 0,
		.best_cpu = -1,
	};
	अचिन्हित दीर्घ taskweight, groupweight;
	काष्ठा sched_domain *sd;
	दीर्घ taskimp, groupimp;
	काष्ठा numa_group *ng;
	काष्ठा rq *best_rq;
	पूर्णांक nid, ret, dist;

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
	अगर (sd) {
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
	अगर (unlikely(!sd)) {
		sched_setnuma(p, task_गाँठ(p));
		वापस -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = गाँठ_distance(env.src_nid, env.dst_nid);
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
	अगर (env.best_cpu == -1 || (ng && ng->active_nodes > 1)) {
		for_each_गाँठ_state(nid, N_CPU) {
			अगर (nid == env.src_nid || nid == p->numa_preferred_nid)
				जारी;

			dist = गाँठ_distance(env.src_nid, env.dst_nid);
			अगर (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_weight(p, env.src_nid, dist);
				groupweight = group_weight(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_weight(p, nid, dist) - taskweight;
			groupimp = group_weight(p, nid, dist) - groupweight;
			अगर (taskimp < 0 && groupimp < 0)
				जारी;

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
	अगर (ng) {
		अगर (env.best_cpu == -1)
			nid = env.src_nid;
		अन्यथा
			nid = cpu_to_गाँठ(env.best_cpu);

		अगर (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	अगर (env.best_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, शून्य, -1);
		वापस -EAGAIN;
	}

	best_rq = cpu_rq(env.best_cpu);
	अगर (env.best_task == शून्य) {
		ret = migrate_task_to(p, env.best_cpu);
		WRITE_ONCE(best_rq->numa_migrate_on, 0);
		अगर (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, शून्य, env.best_cpu);
		वापस ret;
	}

	ret = migrate_swap(p, env.best_task, env.best_cpu, env.src_cpu);
	WRITE_ONCE(best_rq->numa_migrate_on, 0);

	अगर (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.best_task, env.best_cpu);
	put_task_struct(env.best_task);
	वापस ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
अटल व्योम numa_migrate_preferred(काष्ठा task_struct *p)
{
	अचिन्हित दीर्घ interval = HZ;

	/* This task has no NUMA fault statistics yet */
	अगर (unlikely(p->numa_preferred_nid == NUMA_NO_गाँठ || !p->numa_faults))
		वापस;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_to_jiffies(p->numa_scan_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	अगर (task_गाँठ(p) == p->numa_preferred_nid)
		वापस;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
अटल व्योम numa_group_गिनती_active_nodes(काष्ठा numa_group *numa_group)
{
	अचिन्हित दीर्घ faults, max_faults = 0;
	पूर्णांक nid, active_nodes = 0;

	for_each_गाँठ_state(nid, N_CPU) {
		faults = group_faults_cpu(numa_group, nid);
		अगर (faults > max_faults)
			max_faults = faults;
	}

	for_each_गाँठ_state(nid, N_CPU) {
		faults = group_faults_cpu(numa_group, nid);
		अगर (faults * ACTIVE_गाँठ_FRACTION > max_faults)
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
#घोषणा NUMA_PERIOD_SLOTS 10
#घोषणा NUMA_PERIOD_THRESHOLD 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
अटल व्योम update_task_scan_period(काष्ठा task_struct *p,
			अचिन्हित दीर्घ shared, अचिन्हित दीर्घ निजी)
{
	अचिन्हित पूर्णांक period_slot;
	पूर्णांक lr_ratio, ps_ratio;
	पूर्णांक diff;

	अचिन्हित दीर्घ remote = p->numa_faults_locality[0];
	अचिन्हित दीर्घ local = p->numa_faults_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	अगर (local + shared == 0 || p->numa_faults_locality[2]) {
		p->numa_scan_period = min(p->numa_scan_period_max,
			p->numa_scan_period << 1);

		p->mm->numa_अग्र_scan = jiffies +
			msecs_to_jiffies(p->numa_scan_period);

		जाओ out;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_slot = DIV_ROUND_UP(p->numa_scan_period, NUMA_PERIOD_SLOTS);
	lr_ratio = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_ratio = (निजी * NUMA_PERIOD_SLOTS) / (निजी + shared);

	अगर (ps_ratio >= NUMA_PERIOD_THRESHOLD) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		पूर्णांक slot = ps_ratio - NUMA_PERIOD_THRESHOLD;
		अगर (!slot)
			slot = 1;
		diff = slot * period_slot;
	} अन्यथा अगर (lr_ratio >= NUMA_PERIOD_THRESHOLD) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		पूर्णांक slot = lr_ratio - NUMA_PERIOD_THRESHOLD;
		अगर (!slot)
			slot = 1;
		diff = slot * period_slot;
	} अन्यथा {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		पूर्णांक ratio = max(lr_ratio, ps_ratio);
		diff = -(NUMA_PERIOD_THRESHOLD - ratio) * period_slot;
	}

	p->numa_scan_period = clamp(p->numa_scan_period + diff,
			task_scan_min(p), task_scan_max(p));

out:
	स_रखो(p->numa_faults_locality, 0,
	       माप(p->numa_faults_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
अटल u64 numa_get_avg_runtime(काष्ठा task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_start;
	runtime = p->se.योग_exec_runtime;

	अगर (p->last_task_numa_placement) {
		delta = runtime - p->last_योग_exec_runtime;
		*period = now - p->last_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		अगर (unlikely((s64)*period < 0))
			*period = 0;
	} अन्यथा {
		delta = p->se.avg.load_योग;
		*period = LOAD_AVG_MAX;
	}

	p->last_योग_exec_runtime = runtime;
	p->last_task_numa_placement = now;

	वापस delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
अटल पूर्णांक preferred_group_nid(काष्ठा task_struct *p, पूर्णांक nid)
{
	nodemask_t nodes;
	पूर्णांक dist;

	/* Direct connections between all NUMA nodes. */
	अगर (sched_numa_topology_type == NUMA_DIRECT)
		वापस nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	अगर (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		अचिन्हित दीर्घ score, max_score = 0;
		पूर्णांक गाँठ, max_गाँठ = nid;

		dist = sched_max_numa_distance;

		for_each_गाँठ_state(गाँठ, N_CPU) {
			score = group_weight(p, गाँठ, dist);
			अगर (score > max_score) {
				max_score = score;
				max_गाँठ = गाँठ;
			}
		}
		वापस max_गाँठ;
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
	nodes = गाँठ_states[N_CPU];
	क्रम (dist = sched_max_numa_distance; dist > LOCAL_DISTANCE; dist--) {
		अचिन्हित दीर्घ max_faults = 0;
		nodemask_t max_group = गाँठ_MASK_NONE;
		पूर्णांक a, b;

		/* Are there nodes at this distance from each other? */
		अगर (!find_numa_distance(dist))
			जारी;

		for_each_गाँठ_mask(a, nodes) {
			अचिन्हित दीर्घ faults = 0;
			nodemask_t this_group;
			nodes_clear(this_group);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_गाँठ_mask(b, nodes) {
				अगर (गाँठ_distance(a, b) < dist) {
					faults += group_faults(p, b);
					गाँठ_set(b, this_group);
					गाँठ_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			अगर (faults > max_faults) {
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
		अगर (!max_faults)
			अवरोध;
		nodes = max_group;
	}
	वापस nid;
}

अटल व्योम task_numa_placement(काष्ठा task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	काष्ठा sched_cache_group __maybe_unused *grp;
	पूर्णांक seq, nid, max_nid = NUMA_NO_गाँठ;
	अचिन्हित दीर्घ max_faults = 0;
	अचिन्हित दीर्घ fault_types[2] = { 0, 0 };
	अचिन्हित दीर्घ योग_faults;
	u64 runtime, period;
	spinlock_t *group_lock = शून्य;
	दीर्घ __maybe_unused new_fp;
	काष्ठा numa_group *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_scan_seq);
	अगर (p->numa_scan_seq == seq)
		वापस;
	p->numa_scan_seq = seq;
	p->numa_scan_period_max = task_scan_max(p);

	योग_faults = p->numa_faults_locality[0] +
		       p->numa_faults_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_group(p);
	अगर (ng) {
		group_lock = &ng->lock;
		spin_lock_irq(group_lock);
	}

	/* Find the node with the highest number of faults */
	for_each_online_गाँठ(nid) {
		/* Keep track of the offsets in numa_faults array */
		पूर्णांक mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		अचिन्हित दीर्घ faults = 0, group_faults = 0;
		पूर्णांक priv;

		क्रम (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			दीर्घ diff, f_diff, f_weight;

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
				   (योग_faults + 1);
			f_diff = f_weight - p->numa_faults[cpu_idx] / 2;
			p->numa_faults[cpubuf_idx] = 0;

			p->numa_faults[mem_idx] += diff;
			p->numa_faults[cpu_idx] += f_diff;
			faults += p->numa_faults[mem_idx];
			p->योग_numa_faults += diff;
			अगर (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->faults[mem_idx] += diff;
				ng->faults[cpu_idx] += f_diff;
				ng->योग_faults += diff;
				group_faults += ng->faults[mem_idx];
			}
#अगर_घोषित CONFIG_SCHED_CACHE
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

				अगर (grp) {
					new_fp = (दीर्घ)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#पूर्ण_अगर
		}

		अगर (!ng) {
			अगर (faults > max_faults) {
				max_faults = faults;
				max_nid = nid;
			}
		} अन्यथा अगर (group_faults > max_faults) {
			max_faults = group_faults;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_nearest_गाँठ(max_nid, N_CPU);

	अगर (ng) {
		numa_group_गिनती_active_nodes(ng);
		spin_unlock_irq(group_lock);
		max_nid = preferred_group_nid(p, max_nid);
	}

	अगर (max_faults) {
		/* Set the new preferred node */
		अगर (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_scan_period(p, fault_types[0], fault_types[1]);
}

अटल अंतरभूत पूर्णांक get_numa_group(काष्ठा numa_group *grp)
{
	वापस refcount_inc_not_zero(&grp->refcount);
}

अटल अंतरभूत व्योम put_numa_group(काष्ठा numa_group *grp)
{
	अगर (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

अटल व्योम task_numa_group(काष्ठा task_struct *p, पूर्णांक cpupid, पूर्णांक flags,
			पूर्णांक *priv)
{
	काष्ठा numa_group *grp, *my_grp;
	काष्ठा task_struct *tsk;
	bool join = false;
	पूर्णांक cpu = cpupid_to_cpu(cpupid);
	पूर्णांक i;

	अगर (unlikely(!deref_curr_numa_group(p))) {
		अचिन्हित पूर्णांक आकार = माप(काष्ठा numa_group) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_गाँठ_ids * माप(अचिन्हित दीर्घ);

		grp = kzalloc(आकार, GFP_KERNEL | __GFP_NOWARN);
		अगर (!grp)
			वापस;

		refcount_set(&grp->refcount, 1);
		grp->active_nodes = 1;
		grp->max_faults_cpu = 0;
		spin_lock_init(&grp->lock);
		grp->gid = p->pid;

		क्रम (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_गाँठ_ids; i++)
			grp->faults[i] = p->numa_faults[i];

		grp->योग_faults = p->योग_numa_faults;

		grp->nr_tasks++;
		rcu_assign_pointer(p->numa_group, grp);
	}

	rcu_read_lock();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	अगर (!cpupid_match_pid(tsk, cpupid))
		जाओ no_join;

	grp = rcu_dereference_all(tsk->numa_group);
	अगर (!grp)
		जाओ no_join;

	my_grp = deref_curr_numa_group(p);
	अगर (grp == my_grp)
		जाओ no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	अगर (my_grp->nr_tasks > grp->nr_tasks)
		जाओ no_join;

	/*
	 * Tie-break on the grp address.
	 */
	अगर (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		जाओ no_join;

	/* Always join threads in the same process. */
	अगर (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	अगर (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	अगर (join && !get_numa_group(grp))
		जाओ no_join;

	rcu_read_unlock();

	अगर (!join)
		वापस;

	WARN_ON_ONCE(irqs_disabled());
	double_lock_irq(&my_grp->lock, &grp->lock);

	क्रम (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_गाँठ_ids; i++) {
		my_grp->faults[i] -= p->numa_faults[i];
		grp->faults[i] += p->numa_faults[i];
	}
	my_grp->योग_faults -= p->योग_numa_faults;
	grp->योग_faults += p->योग_numa_faults;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->lock);
	spin_unlock_irq(&grp->lock);

	rcu_assign_pointer(p->numa_group, grp);

	put_numa_group(my_grp);
	वापस;

no_join:
	rcu_read_unlock();
	वापस;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
व्योम task_numa_free(काष्ठा task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	काष्ठा numa_group *grp = rcu_dereference_raw(p->numa_group);
	अचिन्हित दीर्घ *numa_faults = p->numa_faults;
	अचिन्हित दीर्घ flags;
	पूर्णांक i;

	अगर (!numa_faults)
		वापस;

	अगर (grp) {
		spin_lock_irqsave(&grp->lock, flags);
		क्रम (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_गाँठ_ids; i++)
			grp->faults[i] -= p->numa_faults[i];
		grp->योग_faults -= p->योग_numa_faults;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->lock, flags);
		RCU_INIT_POINTER(p->numa_group, शून्य);
		put_numa_group(grp);
	}

	अगर (final) {
		p->numa_faults = शून्य;
		kfree(numa_faults);
	} अन्यथा {
		p->योग_numa_faults = 0;
		क्रम (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_गाँठ_ids; i++)
			numa_faults[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
व्योम task_numa_fault(पूर्णांक last_cpupid, पूर्णांक mem_गाँठ, पूर्णांक pages, पूर्णांक flags)
{
	काष्ठा task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	पूर्णांक cpu_गाँठ = task_गाँठ(current);
	पूर्णांक local = !!(flags & TNF_FAULT_LOCAL);
	काष्ठा numa_group *ng;
	पूर्णांक priv;

	अगर (!static_branch_likely(&sched_numa_balancing))
		वापस;

	/* for example, ksmd faulting in a user's mm */
	अगर (!p->mm)
		वापस;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	अगर (!गाँठ_is_toptier(mem_गाँठ) &&
	    (sysctl_numa_balancing_mode & NUMA_BALANCING_MEMORY_TIERING ||
	     !cpupid_valid(last_cpupid)))
		वापस;

	/* Allocate buffer to track faults on a per-node basis */
	अगर (unlikely(!p->numa_faults)) {
		पूर्णांक आकार = माप(*p->numa_faults) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_गाँठ_ids;

		p->numa_faults = kzalloc(आकार, GFP_KERNEL|__GFP_NOWARN);
		अगर (!p->numa_faults)
			वापस;

		p->योग_numa_faults = 0;
		स_रखो(p->numa_faults_locality, 0, माप(p->numa_faults_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	अगर (unlikely(last_cpupid == (-1 & LAST_CPUPID_MASK))) {
		priv = 1;
	} अन्यथा {
		priv = cpupid_match_pid(p, last_cpupid);
		अगर (!priv && !(flags & TNF_NO_GROUP))
			task_numa_group(p, last_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_group(p);
	अगर (!priv && !local && ng && ng->active_nodes > 1 &&
				numa_is_active_गाँठ(cpu_गाँठ, ng) &&
				numa_is_active_गाँठ(mem_गाँठ, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	अगर (time_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	अगर (migrated)
		p->numa_pages_migrated += pages;
	अगर (flags & TNF_MIGRATE_FAIL)
		p->numa_faults_locality[2] += pages;

	p->numa_faults[task_faults_idx(NUMA_MEMBUF, mem_गाँठ, priv)] += pages;
	p->numa_faults[task_faults_idx(NUMA_CPUBUF, cpu_गाँठ, priv)] += pages;
	p->numa_faults_locality[local] += pages;
}

अटल व्योम reset_ptenuma_scan(काष्ठा task_struct *p)
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

अटल bool vma_is_accessed(काष्ठा mm_struct *mm, काष्ठा vm_area_struct *vma)
{
	अचिन्हित दीर्घ pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	अगर ((READ_ONCE(current->mm->numa_scan_seq) - vma->numab_state->start_scan_seq) < 2)
		वापस true;

	pids = vma->numab_state->pids_active[0] | vma->numab_state->pids_active[1];
	अगर (test_bit(hash_32(current->pid, ilog2(BITS_PER_LONG)), &pids))
		वापस true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	अगर (mm->numa_scan_offset > vma->vm_start) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		वापस true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	अगर (READ_ONCE(mm->numa_scan_seq) >
	   (vma->numab_state->पूर्व_scan_seq + get_nr_threads(current)))
		वापस true;

	वापस false;
}

#घोषणा VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_scan_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
अटल व्योम task_numa_work(काष्ठा callback_शीर्ष *work)
{
	अचिन्हित दीर्घ migrate, अग्र_scan, now = jiffies;
	काष्ठा task_struct *p = current;
	काष्ठा mm_struct *mm = p->mm;
	u64 runtime = p->se.योग_exec_runtime;
	काष्ठा vm_area_struct *vma;
	अचिन्हित दीर्घ start, end;
	अचिन्हित दीर्घ nr_pte_updates = 0;
	दीर्घ pages, virtpages;
	काष्ठा vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_of(work, काष्ठा task_struct, numa_work));

	work->अग्र = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	अगर (p->flags & PF_EXITING)
		वापस;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	अगर (cpusets_enabled() && nodes_weight(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		वापस;
	}

	अगर (!mm->numa_अग्र_scan) {
		mm->numa_अग्र_scan = now +
			msecs_to_jiffies(sysctl_numa_balancing_scan_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_अग्र_scan;
	अगर (time_before(now, migrate))
		वापस;

	अगर (p->numa_scan_period == 0) {
		p->numa_scan_period_max = task_scan_max(p);
		p->numa_scan_period = task_scan_start(p);
	}

	अग्र_scan = now + msecs_to_jiffies(p->numa_scan_period);
	अगर (!try_cmpxchg(&mm->numa_अग्र_scan, &migrate, अग्र_scan))
		वापस;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->गाँठ_stamp += 2 * TICK_NSEC;

	pages = sysctl_numa_balancing_scan_आकार;
	pages <<= 20 - PAGE_SHIFT; /* MB in pages */
	virtpages = pages * 8;	   /* Scan up to this much virtual space */
	अगर (!pages)
		वापस;


	अगर (!mmap_read_trylock(mm))
		वापस;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	start = mm->numa_scan_offset;
	vma_iter_init(&vmi, mm, start);
	vma = vma_अग्र(&vmi);
	अगर (!vma) {
		reset_ptenuma_scan(p);
		start = 0;
		vma_iter_set(&vmi, start);
		vma = vma_अग्र(&vmi);
	}

	क्रम (; vma; vma = vma_अग्र(&vmi)) {
		अगर (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_page(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			जारी;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		अगर (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			जारी;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		अगर (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			जारी;
		}

		/* Initialise new per-VMA NUMAB state. */
		अगर (!vma->numab_state) {
			काष्ठा vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			अगर (!ptr)
				जारी;

			अगर (cmpxchg(&vma->numab_state, शून्य, ptr)) {
				kfree(ptr);
				जारी;
			}

			vma->numab_state->start_scan_seq = mm->numa_scan_seq;

			vma->numab_state->अग्र_scan = now +
				msecs_to_jiffies(sysctl_numa_balancing_scan_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_active_reset =  vma->numab_state->अग्र_scan +
				msecs_to_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->पूर्व_scan_seq = mm->numa_scan_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		अगर (mm->numa_scan_seq && time_before(jiffies,
						vma->numab_state->अग्र_scan)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SCAN_DELAY);
			जारी;
		}

		/* RESET access PIDs regularly for old VMAs. */
		अगर (mm->numa_scan_seq &&
				time_after(jiffies, vma->numab_state->pids_active_reset)) {
			vma->numab_state->pids_active_reset = vma->numab_state->pids_active_reset +
				msecs_to_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_active[0] = READ_ONCE(vma->numab_state->pids_active[1]);
			vma->numab_state->pids_active[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		अगर (vma->numab_state->पूर्व_scan_seq == mm->numa_scan_seq) {
			mm->numa_scan_offset = vma->vm_end;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			जारी;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		अगर (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			जारी;
		}

		करो {
			start = max(start, vma->vm_start);
			end = ALIGN(start + (pages << PAGE_SHIFT), HPAGE_आकार);
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
			अगर (nr_pte_updates)
				pages -= (end - start) >> PAGE_SHIFT;
			virtpages -= (end - start) >> PAGE_SHIFT;

			start = end;
			अगर (pages <= 0 || virtpages <= 0)
				जाओ out;

			cond_resched();
		} जबतक (end != vma->vm_end);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->पूर्व_scan_seq = mm->numa_scan_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		अगर (vma_pids_forced)
			अवरोध;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	अगर (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		जाओ retry_pids;
	}

out:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	अगर (vma)
		mm->numa_scan_offset = start;
	अन्यथा
		reset_ptenuma_scan(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	अगर (unlikely(p->se.योग_exec_runtime != runtime)) {
		u64 diff = p->se.योग_exec_runtime - runtime;
		p->गाँठ_stamp += 32 * diff;
	}
}

व्योम init_numa_balancing(u64 clone_flags, काष्ठा task_struct *p)
{
	पूर्णांक mm_users = 0;
	काष्ठा mm_struct *mm = p->mm;

	अगर (mm) {
		mm_users = atomic_read(&mm->mm_users);
		अगर (mm_users == 1) {
			mm->numa_अग्र_scan = jiffies + msecs_to_jiffies(sysctl_numa_balancing_scan_delay);
			mm->numa_scan_seq = 0;
		}
	}
	p->गाँठ_stamp			= 0;
	p->numa_scan_seq		= mm ? mm->numa_scan_seq : 0;
	p->numa_scan_period		= sysctl_numa_balancing_scan_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.अग्र		= &p->numa_work;
	p->numa_faults			= शून्य;
	p->numa_pages_migrated		= 0;
	p->योग_numa_faults		= 0;
	RCU_INIT_POINTER(p->numa_group, शून्य);
	p->last_task_numa_placement	= 0;
	p->last_योग_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	अगर (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_गाँठ;
		वापस;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	अगर (mm) {
		अचिन्हित पूर्णांक delay;

		delay = min_t(अचिन्हित पूर्णांक, task_scan_max(current),
			current->numa_scan_period * mm_users * NSEC_PER_MSEC);
		delay += 2 * TICK_NSEC;
		p->गाँठ_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
अटल व्योम task_tick_numa(काष्ठा rq *rq, काष्ठा task_struct *curr)
{
	काष्ठा callback_शीर्ष *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	अगर (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->अग्र != work)
		वापस;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.योग_exec_runtime;
	period = (u64)curr->numa_scan_period * NSEC_PER_MSEC;

	अगर (now > curr->गाँठ_stamp + period) {
		अगर (!curr->गाँठ_stamp)
			curr->numa_scan_period = task_scan_start(curr);
		curr->गाँठ_stamp += period;

		अगर (!time_before(jiffies, curr->mm->numa_अग्र_scan))
			task_work_add(curr, work, TWA_RESUME);
	}
}

अटल व्योम update_scan_period(काष्ठा task_struct *p, पूर्णांक new_cpu)
{
	पूर्णांक src_nid = cpu_to_गाँठ(task_cpu(p));
	पूर्णांक dst_nid = cpu_to_गाँठ(new_cpu);

	अगर (!static_branch_likely(&sched_numa_balancing))
		वापस;

	अगर (!p->mm || !p->numa_faults || (p->flags & PF_EXITING))
		वापस;

	अगर (src_nid == dst_nid)
		वापस;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	अगर (p->numa_scan_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		अगर (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_गाँठ &&
			src_nid != p->numa_preferred_nid))
			वापस;
	}

	p->numa_scan_period = task_scan_start(p);
}

#अन्यथा /* !CONFIG_NUMA_BALANCING: */

अटल व्योम task_tick_numa(काष्ठा rq *rq, काष्ठा task_struct *curr)
{
}

अटल अंतरभूत व्योम account_numa_enqueue(काष्ठा rq *rq, काष्ठा task_struct *p)
{
}

अटल अंतरभूत व्योम account_numa_dequeue(काष्ठा rq *rq, काष्ठा task_struct *p)
{
}

अटल अंतरभूत व्योम update_scan_period(काष्ठा task_struct *p, पूर्णांक new_cpu)
{
}

#पूर्ण_अगर /* !CONFIG_NUMA_BALANCING */

अटल व्योम
account_entity_enqueue(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_of(se));
	update_load_add(&cfs_rq->load, se->load.weight);
	अगर (entity_is_task(se)) {
		काष्ठा task_struct *p = task_of(se);
		काष्ठा rq *rq = rq_of(cfs_rq);

		account_numa_enqueue(rq, p);
		account_llc_enqueue(rq, p);
		list_add(&se->group_गाँठ, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

अटल व्योम
account_entity_dequeue(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_of(se));
	update_load_sub(&cfs_rq->load, se->load.weight);
	अगर (entity_is_task(se)) {
		काष्ठा task_struct *p = task_of(se);
		काष्ठा rq *rq = rq_of(cfs_rq);

		account_numa_dequeue(rq, p);
		account_llc_dequeue(rq, p);
		list_del_init(&se->group_गाँठ);
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
#घोषणा add_positive(_ptr, _val) करो {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	अगर (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} जबतक (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#घोषणा lsub_positive(_ptr, _val) करो {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} जबतक (0)


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
#घोषणा __update_sa(sa, name, delta_avg, delta_योग) करो {	\
	add_positive(&(sa)->name##_avg, delta_avg);		\
	add_positive(&(sa)->name##_sum, delta_योग);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} जबतक (0)

अटल अंतरभूत व्योम
enqueue_load_avg(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	__update_sa(&cfs_rq->avg, load, se->avg.load_avg,
		    se_weight(se) * se->avg.load_योग);
}

अटल अंतरभूत व्योम
dequeue_load_avg(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	__update_sa(&cfs_rq->avg, load, -se->avg.load_avg,
		    se_weight(se) * -se->avg.load_योग);
}

अटल व्योम
rescale_entity(काष्ठा sched_entity *se, अचिन्हित दीर्घ weight, bool rel_vprot)
{
	दीर्घ old_weight = se->h_load.weight;

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
	अगर (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_weight, weight);

	अगर (rel_vprot)
		se->vprot = div64_long(se->vprot * old_weight, weight);
}

अटल व्योम reweight_eevdf(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se,
			   अचिन्हित दीर्घ weight, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	अगर (se->h_load.weight == weight)
		वापस;

	अगर (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		अगर (curr && protect_slice(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		अगर (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, weight, rel_vprot);

	update_load_set(&se->h_load, weight);

	अगर (on_rq) {
		अगर (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		अगर (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

अटल व्योम reweight_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se,
			    अचिन्हित दीर्घ weight)
{
	अगर (se->load.weight == weight)
		वापस;

	अगर (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_of(se));
		update_load_sub(&cfs_rq->load, se->load.weight);
	}
	dequeue_load_avg(cfs_rq, se);

	update_load_set(&se->load, weight);

	करो {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.load_avg = div_u64(se_weight(se) * se->avg.load_योग, divider);
	} जबतक (0);

	enqueue_load_avg(cfs_rq, se);

	अगर (se->on_rq)
		update_load_add(&cfs_rq->load, se->load.weight);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
अटल __always_inline
अचिन्हित दीर्घ __calc_prop_weight(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se,
				 अचिन्हित दीर्घ weight)
{
	weight *= se->load.weight;
	अगर (parent_entity(se))
		weight /= cfs_rq->load.weight;
	अन्यथा
		weight /= NICE_0_LOAD;

	वापस max(weight, MIN_SHARES);
}

अटल व्योम reweight_task_fair(काष्ठा rq *rq, काष्ठा task_struct *p,
			       स्थिर काष्ठा load_weight *lw)
{
	काष्ठा sched_entity *se = &p->se;
	अचिन्हित दीर्घ weight = NICE_0_LOAD;

	अगर (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_of(se), se, lw->weight);
	se->load.inv_weight = lw->inv_weight;

	अगर (!se->on_rq)
		वापस;

	for_each_sched_entity(se)
		weight = __calc_prop_weight(cfs_rq_of(se), se, weight);

	reweight_eevdf(&rq->cfs, &p->se, weight, p->se.on_rq);
}

अटल अंतरभूत पूर्णांक throttled_hierarchy(काष्ठा cfs_rq *cfs_rq);

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
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
अटल दीर्घ __calc_smp_shares(काष्ठा cfs_rq *cfs_rq, दीर्घ tg_shares, दीर्घ shares_max)
{
	काष्ठा task_group *tg = cfs_rq->tg;
	दीर्घ tg_weight, load, shares;

	load = max(scale_load_down(cfs_rq->load.weight), cfs_rq->avg.load_avg);

	tg_weight = atomic_long_read(&tg->load_avg);

	/* Ensure tg_weight >= load */
	tg_weight -= cfs_rq->tg_load_avg_contrib;
	tg_weight += load;

	shares = (tg_shares * load);
	अगर (tg_weight)
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
	वापस clamp_t(दीर्घ, shares, MIN_SHARES, shares_max);
}

अटल पूर्णांक tg_cpus(काष्ठा task_group *tg)
{
	पूर्णांक nr = अंक_online_cpus();

	अगर (cpusets_enabled()) {
		काष्ठा cgroup *cgrp = tg->css.cgroup;
		अगर (cgrp)
			nr = cpuset_अंक_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	वापस max(nr, 1);
}

अटल अंतरभूत पूर्णांक tg_tasks(काष्ठा task_group *tg)
{
	वापस max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_CAPACITY_SHIFT);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
अटल दीर्घ calc_tasks_shares(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा task_group *tg = cfs_rq->tg;
	पूर्णांक nr = tg_tasks(tg);
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	वापस __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
अटल दीर्घ calc_max_shares(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा task_group *tg = cfs_rq->tg;
	पूर्णांक nr = tg_cpus(tg);
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	दीर्घ max_shares = scale_load(sched_prio_to_weight[0]);
	वापस __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
अटल दीर्घ calc_concur_shares(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा task_group *tg = cfs_rq->tg;
	पूर्णांक nr = min(tg_tasks(tg), tg_cpus(tg));
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	वापस __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
अटल दीर्घ calc_smp_shares(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा task_group *tg = cfs_rq->tg;
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	वापस __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
अटल दीर्घ calc_up_shares(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा task_group *tg = cfs_rq->tg;
	वापस READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_group_shares, calc_concur_shares);

व्योम __sched_cgroup_mode_update(पूर्णांक mode)
{
	दीर्घ (*func)(काष्ठा cfs_rq *);
	चयन (mode) {
	हाल 0:
		func = &calc_up_shares;
		अवरोध;
	हाल 1:
		func = &calc_smp_shares;
		अवरोध;
	हाल 2:
	शेष:
		func = &calc_concur_shares;
		अवरोध;
	हाल 3:
		func = &calc_max_shares;
		अवरोध;
	हाल 4:
		func = &calc_tasks_shares;
		अवरोध;
	}
	static_call_update(calc_group_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
अटल व्योम update_cfs_group(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *gcfs_rq = group_cfs_rq(se);
	दीर्घ shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	अगर (!gcfs_rq || !gcfs_rq->load.weight)
		वापस;

	shares = static_call(calc_group_shares)(gcfs_rq);
	reweight_entity(cfs_rq_of(se), se, shares);
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
अटल अंतरभूत व्योम update_cfs_group(काष्ठा sched_entity *se)
{
}
#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

अटल अंतरभूत व्योम cfs_rq_util_change(काष्ठा cfs_rq *cfs_rq, पूर्णांक flags)
{
	काष्ठा rq *rq = rq_of(cfs_rq);

	अगर (&rq->cfs == cfs_rq) {
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

अटल अंतरभूत bool load_avg_is_decayed(काष्ठा sched_avg *sa)
{
	अगर (sa->load_योग)
		वापस false;

	अगर (sa->util_योग)
		वापस false;

	अगर (sa->runnable_योग)
		वापस false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->load_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	वापस true;
}

अटल अंतरभूत u64 cfs_rq_last_update_time(काष्ठा cfs_rq *cfs_rq)
{
	वापस u64_u32_load_copy(cfs_rq->avg.last_update_time,
				 cfs_rq->last_update_time_copy);
}
#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
अटल अंतरभूत bool child_cfs_rq_on_list(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा cfs_rq *पूर्व_cfs_rq;
	काष्ठा list_शीर्ष *पूर्व;
	काष्ठा rq *rq = rq_of(cfs_rq);

	अगर (cfs_rq->on_list) {
		पूर्व = cfs_rq->leaf_cfs_rq_list.पूर्व;
	} अन्यथा {
		पूर्व = rq->tmp_alone_branch;
	}

	अगर (पूर्व == &rq->leaf_cfs_rq_list)
		वापस false;

	पूर्व_cfs_rq = container_of(पूर्व, काष्ठा cfs_rq, leaf_cfs_rq_list);

	वापस (पूर्व_cfs_rq->tg->parent == cfs_rq->tg);
}

अटल अंतरभूत bool cfs_rq_is_decayed(काष्ठा cfs_rq *cfs_rq)
{
	अगर (cfs_rq->load.weight)
		वापस false;

	अगर (!load_avg_is_decayed(&cfs_rq->avg))
		वापस false;

	अगर (child_cfs_rq_on_list(cfs_rq))
		वापस false;

	अगर (cfs_rq->tg_load_avg_contrib)
		वापस false;

	वापस true;
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
अटल अंतरभूत व्योम update_tg_load_avg(काष्ठा cfs_rq *cfs_rq)
{
	दीर्घ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	अगर (cfs_rq->tg == &मूल_task_group)
		वापस;

	/* rq has been offline and doesn't contribute to the share anymore: */
	अगर (!cpu_active(cpu_of(rq_of(cfs_rq))))
		वापस;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_of(cfs_rq));
	अगर (now - cfs_rq->last_update_tg_load_avg < NSEC_PER_MSEC)
		वापस;

	dl = cfs_rq->avg.load_avg - cfs_rq->tg_load_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	अगर (असल(dl) > cfs_rq->tg_load_avg_contrib / 64 ||
	    असल(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_add(dl, &cfs_rq->tg->load_avg);
		atomic_long_add(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_load_avg_contrib = cfs_rq->avg.load_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->last_update_tg_load_avg = now;
	}
}

अटल अंतरभूत व्योम clear_tg_load_avg(काष्ठा cfs_rq *cfs_rq)
{
	दीर्घ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	अगर (cfs_rq->tg == &मूल_task_group)
		वापस;

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
अटल व्योम __maybe_unused clear_tg_offline_cfs_rqs(काष्ठा rq *rq)
{
	काष्ठा task_group *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_start_loop_update(rq);

	guard(rcu)();

	list_for_each_entry_rcu(tg, &task_groups, list) {
		काष्ठा cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

		clear_tg_load_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
व्योम set_task_rq_fair(काष्ठा sched_entity *se,
		      काष्ठा cfs_rq *पूर्व, काष्ठा cfs_rq *अग्र)
{
	u64 p_last_update_time;
	u64 n_last_update_time;

	अगर (!sched_feat(ATTACH_AGE_LOAD))
		वापस;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	अगर (!(se->avg.last_update_time && पूर्व))
		वापस;

	p_last_update_time = cfs_rq_last_update_time(पूर्व);
	n_last_update_time = cfs_rq_last_update_time(अग्र);

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
अटल अंतरभूत व्योम
update_tg_cfs_util(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, काष्ठा cfs_rq *gcfs_rq)
{
	दीर्घ delta_योग, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_योग, divider;

	/* Nothing to update */
	अगर (!delta_avg)
		वापस;

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

अटल अंतरभूत व्योम
update_tg_cfs_runnable(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, काष्ठा cfs_rq *gcfs_rq)
{
	दीर्घ delta_योग, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_योग;
	u32 divider;

	/* Nothing to update */
	अगर (!delta_avg)
		वापस;

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

अटल अंतरभूत व्योम
update_tg_cfs_load(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, काष्ठा cfs_rq *gcfs_rq)
{
	दीर्घ delta_avg, running_योग, runnable_योग = gcfs_rq->prop_runnable_योग;
	अचिन्हित दीर्घ load_avg;
	u64 load_योग = 0;
	s64 delta_योग;
	u32 divider;

	अगर (!runnable_योग)
		वापस;

	gcfs_rq->prop_runnable_योग = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	अगर (runnable_योग >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_योग += se->avg.load_योग;
		runnable_योग = min_t(दीर्घ, runnable_योग, divider);
	} अन्यथा {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		अगर (scale_load_down(gcfs_rq->load.weight)) {
			load_योग = div_u64(gcfs_rq->avg.load_योग,
				scale_load_down(gcfs_rq->load.weight));
		}

		/* But make sure to not inflate se's runnable */
		runnable_योग = min(se->avg.load_योग, load_योग);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_योग = se->avg.util_योग >> SCHED_CAPACITY_SHIFT;
	runnable_योग = max(runnable_योग, running_योग);

	load_योग = se_weight(se) * runnable_योग;
	load_avg = div_u64(load_योग, divider);

	delta_avg = load_avg - se->avg.load_avg;
	अगर (!delta_avg)
		वापस;

	delta_योग = load_योग - (s64)se_weight(se) * se->avg.load_योग;

	se->avg.load_योग = runnable_योग;
	se->avg.load_avg = load_avg;
	__update_sa(&cfs_rq->avg, load, delta_avg, delta_योग);
}

अटल अंतरभूत व्योम add_tg_cfs_propagate(काष्ठा cfs_rq *cfs_rq, दीर्घ runnable_योग)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_योग += runnable_योग;
}

/* Update task and its cfs_rq load average */
अटल अंतरभूत पूर्णांक propagate_entity_load_avg(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *cfs_rq, *gcfs_rq;

	अगर (entity_is_task(se))
		वापस 0;

	gcfs_rq = group_cfs_rq(se);
	अगर (!gcfs_rq->propagate)
		वापस 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_of(se);

	add_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_योग);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_load(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	वापस 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
अटल अंतरभूत bool skip_blocked_update(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *gcfs_rq = group_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	अगर (se->avg.load_avg || se->avg.util_avg)
		वापस false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	अगर (gcfs_rq->propagate)
		वापस false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	वापस true;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

अटल अंतरभूत व्योम update_tg_load_avg(काष्ठा cfs_rq *cfs_rq) {}

अटल अंतरभूत व्योम clear_tg_offline_cfs_rqs(काष्ठा rq *rq) {}

अटल अंतरभूत पूर्णांक propagate_entity_load_avg(काष्ठा sched_entity *se)
{
	वापस 0;
}

अटल अंतरभूत व्योम add_tg_cfs_propagate(काष्ठा cfs_rq *cfs_rq, दीर्घ runnable_योग) {}

#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

#अगर_घोषित CONFIG_NO_HZ_COMMON
अटल अंतरभूत व्योम migrate_se_pelt_lag(काष्ठा sched_entity *se)
{
	u64 throttled = 0, now, lut;
	काष्ठा cfs_rq *cfs_rq;
	काष्ठा rq *rq;
	bool is_idle;

	अगर (load_avg_is_decayed(&se->avg))
		वापस;

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
	अगर (!is_idle)
		वापस;

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

#अगर_घोषित CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_load(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	अगर (throttled == U64_MAX)
		वापस;
#पूर्ण_अगर
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
	अगर (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	अन्यथा
		now += sched_clock_cpu(cpu_of(rq)) - u64_u32_load(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
अटल व्योम migrate_se_pelt_lag(काष्ठा sched_entity *se) {}
#पूर्ण_अगर /* !CONFIG_NO_HZ_COMMON */

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
अटल अंतरभूत पूर्णांक
update_cfs_rq_load_avg(u64 now, काष्ठा cfs_rq *cfs_rq)
{
	अचिन्हित दीर्घ removed_load = 0, removed_util = 0, removed_runnable = 0;
	काष्ठा sched_avg *sa = &cfs_rq->avg;
	पूर्णांक decayed = 0;

	अगर (cfs_rq->removed.nr) {
		अचिन्हित दीर्घ r;
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
			-(दीर्घ)(removed_runnable * divider) >> SCHED_CAPACITY_SHIFT);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->last_update_time,
			   cfs_rq->last_update_time_copy,
			   sa->last_update_time);
	वापस decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
अटल व्योम attach_entity_load_avg(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
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
	se->avg.util_योग = se->avg.util_avg * divider;

	se->avg.runnable_योग = se->avg.runnable_avg * divider;

	se->avg.load_योग = se->avg.load_avg * divider;
	अगर (se_weight(se) < se->avg.load_योग)
		se->avg.load_योग = div_u64(se->avg.load_योग, se_weight(se));
	अन्यथा
		se->avg.load_योग = 1;

	enqueue_load_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_योग += se->avg.util_योग;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_योग += se->avg.runnable_योग;

	add_tg_cfs_propagate(cfs_rq, se->avg.load_योग);

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
अटल व्योम detach_entity_load_avg(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	dequeue_load_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_योग);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_योग);

	add_tg_cfs_propagate(cfs_rq, -se->avg.load_योग);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#घोषणा UTIL_EST_MARGIN (SCHED_CAPACITY_SCALE / 100)

अटल अंतरभूत व्योम util_est_update(काष्ठा sched_entity *se)
{
	अचिन्हित पूर्णांक ewma, dequeued, last_ewma_diff;

	अगर (!sched_feat(UTIL_EST))
		वापस;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	अगर (ewma & UTIL_AVG_UNCHANGED)
		वापस;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	अगर (ewma <= dequeued) {
		ewma = dequeued;
		जाओ done;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	last_ewma_diff = ewma - dequeued;
	अगर (last_ewma_diff < UTIL_EST_MARGIN)
		जाओ done;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	अगर ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		जाओ done;

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
#घोषणा UPDATE_TG	0x01
#घोषणा SKIP_AGE_LOAD	0x02
#घोषणा DO_ATTACH	0x04
#घोषणा DO_DETACH	0x08
#घोषणा UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
अटल अंतरभूत व्योम update_load_avg(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, पूर्णांक flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	पूर्णांक decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	अगर (se->avg.last_update_time && !(flags & SKIP_AGE_LOAD))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_load_avg(now, cfs_rq);
	decayed |= propagate_entity_load_avg(se);

	अगर (!se->avg.last_update_time && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_load_avg(cfs_rq, se);
		update_tg_load_avg(cfs_rq);

	} अन्यथा अगर (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_load_avg(cfs_rq, se);
		update_tg_load_avg(cfs_rq);
	} अन्यथा अगर (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		अगर (flags & UPDATE_TG)
			update_tg_load_avg(cfs_rq);
	}

	अगर (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
अटल व्योम sync_entity_load_avg(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);
	u64 last_update_time;

	last_update_time = cfs_rq_last_update_time(cfs_rq);
	__update_load_avg_blocked_se(last_update_time, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
अटल व्योम remove_entity_load_avg(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);
	अचिन्हित दीर्घ flags;

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

अटल अंतरभूत अचिन्हित दीर्घ cfs_rq_runnable_avg(काष्ठा cfs_rq *cfs_rq)
{
	वापस cfs_rq->avg.runnable_avg;
}

अटल अंतरभूत अचिन्हित दीर्घ cfs_rq_load_avg(काष्ठा cfs_rq *cfs_rq)
{
	वापस cfs_rq->avg.load_avg;
}

अटल पूर्णांक sched_balance_newidle(काष्ठा rq *this_rq, काष्ठा rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

अटल अंतरभूत अचिन्हित दीर्घ task_util(काष्ठा task_struct *p)
{
	वापस READ_ONCE(p->se.avg.util_avg);
}

अटल अंतरभूत अचिन्हित दीर्घ _task_util_est(काष्ठा task_struct *p)
{
	वापस READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

अटल अंतरभूत अचिन्हित दीर्घ task_util_est(काष्ठा task_struct *p)
{
	वापस max(task_util(p), _task_util_est(p));
}

अटल अंतरभूत व्योम util_est_enqueue(काष्ठा cfs_rq *cfs_rq,
				    काष्ठा task_struct *p)
{
	अचिन्हित पूर्णांक enqueued;

	अगर (!sched_feat(UTIL_EST))
		वापस;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

अटल अंतरभूत व्योम util_est_dequeue(काष्ठा cfs_rq *cfs_rq,
				    काष्ठा task_struct *p)
{
	अचिन्हित पूर्णांक enqueued;

	अगर (!sched_feat(UTIL_EST))
		वापस;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(अचिन्हित पूर्णांक, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

अटल अंतरभूत अचिन्हित दीर्घ get_actual_cpu_capacity(पूर्णांक cpu)
{
	अचिन्हित दीर्घ capacity = arch_scale_cpu_capacity(cpu);

	capacity -= max(hw_load_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	वापस capacity;
}

अटल अंतरभूत पूर्णांक util_fits_cpu(अचिन्हित दीर्घ util,
				अचिन्हित दीर्घ uclamp_min,
				अचिन्हित दीर्घ uclamp_max,
				पूर्णांक cpu)
{
	अचिन्हित दीर्घ capacity = capacity_of(cpu);
	अचिन्हित दीर्घ capacity_orig;
	bool fits, uclamp_max_fits;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	fits = fits_capacity(util, capacity);

	अगर (!uclamp_is_used())
		वापस fits;

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
	अगर (fits && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_capacity(cpu)))
		वापस -1;

	वापस fits;
}

अटल अंतरभूत पूर्णांक task_fits_cpu(काष्ठा task_struct *p, पूर्णांक cpu)
{
	अचिन्हित दीर्घ uclamp_min = uclamp_eff_मूल्य(p, UCLAMP_MIN);
	अचिन्हित दीर्घ uclamp_max = uclamp_eff_मूल्य(p, UCLAMP_MAX);
	अचिन्हित दीर्घ util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	वापस (util_fits_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

अटल अंतरभूत व्योम update_misfit_status(काष्ठा task_struct *p, काष्ठा rq *rq)
{
	पूर्णांक cpu = cpu_of(rq);

	अगर (!sched_asym_cpucap_active())
		वापस;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	अगर (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_capacity(cpu) == p->max_allowed_capacity) ||
	    task_fits_cpu(p, cpu)) {

		rq->misfit_task_load = 0;
		वापस;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_load = max_t(अचिन्हित दीर्घ, task_h_load(p), 1);
}

व्योम __setparam_fair(काष्ठा task_struct *p, स्थिर काष्ठा sched_attr *attr)
{
	काष्ठा sched_entity *se = &p->se;

	p->static_prio = NICE_TO_PRIO(attr->sched_nice);
	अगर (attr->sched_runtime) {
		se->custom_slice = 1;
		se->slice = clamp_t(u64, attr->sched_runtime,
				      NSEC_PER_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_PER_MSEC*100); /* HZ=100  / 10 */
	} अन्यथा {
		se->custom_slice = 0;
		se->slice = sysctl_sched_base_slice;
	}
}

अटल व्योम
place_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, पूर्णांक flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	अचिन्हित पूर्णांक nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	अगर (!se->custom_slice)
		se->slice = sysctl_sched_base_slice;
	vslice = calc_delta_fair(se->slice, se);

	अगर (flags & ENQUEUE_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	अगर (sched_feat(PLACE_LAG) && nr_queued && se->vlag) {
		काष्ठा sched_entity *curr = cfs_rq->curr;
		दीर्घ load, weight;

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
		load = cfs_rq->योग_weight;
		अगर (curr && curr->on_rq)
			load += avg_vruntime_weight(cfs_rq, curr->h_load.weight);
		अगर (WARN_ON_ONCE(!load))
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
		अगर (weight > load)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	अगर (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	अगर (sched_feat(PLACE_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		वापस;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	अगर (sched_feat(PLACE_DEADLINE_INITIAL) && (flags & ENQUEUE_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

अटल व्योम check_enqueue_throttle(काष्ठा cfs_rq *cfs_rq);
अटल अंतरभूत पूर्णांक cfs_rq_throttled(काष्ठा cfs_rq *cfs_rq);

अटल व्योम
enqueue_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, पूर्णांक flags)
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
	अगर (flags & ENQUEUE_MIGRATED)
		se->exec_start = 0;

	check_schedstat_required();
	update_stats_enqueue_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	अगर (cfs_rq->nr_queued == 1) {
		check_enqueue_throttle(cfs_rq);
		list_add_leaf_cfs_rq(cfs_rq);
#अगर_घोषित CONFIG_CFS_BANDWIDTH
		अगर (cfs_rq->pelt_clock_throttled) {
			काष्ठा rq *rq = rq_of(cfs_rq);

			cfs_rq->throttled_clock_pelt_time += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#पूर्ण_अगर
	}
}

अटल व्योम set_अग्र_buddy(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	अगर (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		वापस;
	अगर (se_is_idle(se))
		वापस;
	cfs_rq->अग्र = se;
}

अटल व्योम clear_buddies(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	अगर (cfs_rq->अग्र == se)
		cfs_rq->अग्र = शून्य;
}

अटल __always_inline व्योम return_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq);

अटल व्योम set_delayed(काष्ठा sched_entity *se)
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
	अगर (!entity_is_task(se)) {
		se->sched_delayed = 1;
		वापस;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_of(cfs_rq_of(se)), task_of(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

		cfs_rq->h_nr_runnable--;
	}
}

अटल व्योम clear_delayed(काष्ठा sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	अगर (!entity_is_task(se))
		वापस;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_of(cfs_rq_of(se)), task_of(se));

	for_each_sched_entity(se) {
		काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

		cfs_rq->h_nr_runnable++;
	}
}

अटल व्योम
dequeue_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se, पूर्णांक flags)
{
	पूर्णांक action = UPDATE_TG;

	अगर (entity_is_task(se)) {
		अगर (task_on_rq_migrating(task_of(se)))
			action |= DO_DETACH;

		अगर ((flags & DEQUEUE_SLEEP) && !(flags & DEQUEUE_DELAYED))
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

	अगर (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#अगर_घोषित CONFIG_CFS_BANDWIDTH
		अगर (throttled_hierarchy(cfs_rq)) {
			काष्ठा rq *rq = rq_of(cfs_rq);

			list_del_leaf_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#पूर्ण_अगर
	}
}

अटल व्योम
set_अग्र_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	अगर (se->on_rq) {
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
	अगर (schedstat_enabled() &&
	    rq_of(cfs_rq)->cfs.load.weight >= 2*se->load.weight) {
		काष्ठा sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->slice_max,
				max((u64)stats->slice_max,
				    se->योग_exec_runtime - se->पूर्व_योग_exec_runtime));
	}

	se->पूर्व_योग_exec_runtime = se->योग_exec_runtime;
}

अटल bool __dequeue_task(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);

अटल काष्ठा sched_entity *
pick_अग्र_entity(काष्ठा rq *rq, bool protect)
{
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	काष्ठा sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	अगर (se->sched_delayed) {
		__dequeue_task(rq, task_of(se), DEQUEUE_SLEEP | DEQUEUE_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		वापस शून्य;
	}
	वापस se;
}

अटल व्योम put_पूर्व_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *पूर्व)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	अगर (पूर्व->on_rq)
		update_curr(cfs_rq);

	अगर (पूर्व->on_rq) {
		update_stats_wait_start_fair(cfs_rq, पूर्व);
		/* in !on_rq case, update occurred at dequeue */
		update_load_avg(cfs_rq, पूर्व, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != पूर्व);
	cfs_rq->h_curr = शून्य;
}

अटल व्योम
entity_tick(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *curr, पूर्णांक queued)
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

#अगर_घोषित CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	अगर (queued) {
		resched_curr(rq_of(cfs_rq));
		वापस;
	}
#पूर्ण_अगर
}


/**************************************************
 * CFS bandwidth control machinery
 */

#अगर_घोषित CONFIG_CFS_BANDWIDTH

#अगर_घोषित CONFIG_JUMP_LABEL
अटल काष्ठा static_कुंजी __cfs_bandwidth_used;

अटल अंतरभूत bool cfs_bandwidth_used(व्योम)
{
	वापस static_कुंजी_false(&__cfs_bandwidth_used);
}

व्योम cfs_bandwidth_usage_inc(व्योम)
{
	static_कुंजी_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

व्योम cfs_bandwidth_usage_dec(व्योम)
{
	static_कुंजी_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#अन्यथा /* !CONFIG_JUMP_LABEL: */
अटल bool cfs_bandwidth_used(व्योम)
{
	वापस true;
}

व्योम cfs_bandwidth_usage_inc(व्योम) {}
व्योम cfs_bandwidth_usage_dec(व्योम) {}
#पूर्ण_अगर /* !CONFIG_JUMP_LABEL */

अटल अंतरभूत u64 sched_cfs_bandwidth_slice(व्योम)
{
	वापस (u64)sysctl_sched_cfs_bandwidth_slice * NSEC_PER_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
व्योम __refill_cfs_bandwidth_runtime(काष्ठा cfs_bandwidth *cfs_b)
{
	s64 runtime;

	अगर (unlikely(cfs_b->quota == RUNTIME_INF))
		वापस;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	अगर (runtime > 0) {
		cfs_b->burst_time += runtime;
		cfs_b->nr_burst++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->burst);
	cfs_b->runtime_snap = cfs_b->runtime;
}

अटल अंतरभूत काष्ठा cfs_bandwidth *tg_cfs_bandwidth(काष्ठा task_group *tg)
{
	वापस &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
अटल पूर्णांक __assign_cfs_rq_runtime(काष्ठा cfs_bandwidth *cfs_b,
				   काष्ठा cfs_rq *cfs_rq, u64 target_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->lock);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = target_runtime - cfs_rq->runtime_remaining;

	अगर (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	अन्यथा {
		start_cfs_bandwidth(cfs_b);

		अगर (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_remaining += amount;

	वापस cfs_rq->runtime_remaining > 0;
}

अटल bool throttle_cfs_rq(काष्ठा cfs_rq *cfs_rq);

अटल bool __account_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_remaining -= delta_exec;

	अगर (likely(cfs_rq->runtime_remaining > 0))
		वापस false;

	अगर (cfs_rq->throttled)
		वापस true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	वापस throttle_cfs_rq(cfs_rq);
}

अटल __always_inline
bool account_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq, u64 delta_exec)
{
	अगर (!cfs_bandwidth_used() || !cfs_rq->runtime_enabled)
		वापस false;

	वापस __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

अटल अंतरभूत पूर्णांक cfs_rq_throttled(काष्ठा cfs_rq *cfs_rq)
{
	वापस cfs_bandwidth_used() && cfs_rq->throttled;
}

अटल अंतरभूत bool cfs_rq_pelt_clock_throttled(काष्ठा cfs_rq *cfs_rq)
{
	वापस cfs_bandwidth_used() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
अटल अंतरभूत पूर्णांक throttled_hierarchy(काष्ठा cfs_rq *cfs_rq)
{
	वापस cfs_bandwidth_used() && cfs_rq->throttle_गिनती;
}

अटल अंतरभूत पूर्णांक lb_throttled_hierarchy(काष्ठा task_struct *p, पूर्णांक dst_cpu)
{
	वापस throttled_hierarchy(tg_cfs_rq(task_group(p), dst_cpu));
}

अटल अंतरभूत bool task_is_throttled(काष्ठा task_struct *p)
{
	वापस cfs_bandwidth_used() && p->throttled;
}

अटल bool dequeue_task_fair(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);
अटल व्योम throttle_cfs_rq_work(काष्ठा callback_शीर्ष *work)
{
	काष्ठा task_struct *p = container_of(work, काष्ठा task_struct, sched_throttle_work);
	काष्ठा sched_entity *se;
	काष्ठा cfs_rq *cfs_rq;
	काष्ठा rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.अग्र = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	अगर ((p->flags & PF_EXITING))
		वापस;

	scoped_guard(task_rq_lock, p) {
		se = &p->se;
		cfs_rq = cfs_rq_of(se);

		/* Raced, forget */
		अगर (p->sched_class != &fair_sched_class)
			वापस;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		अगर (!cfs_rq->throttle_गिनती)
			वापस;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_empty(&p->throttle_गाँठ));
		dequeue_task_fair(rq, p, DEQUEUE_SLEEP | DEQUEUE_THROTTLE);
		list_add(&p->throttle_गाँठ, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

व्योम init_cfs_throttle_work(काष्ठा task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.अग्र = &p->sched_throttle_work;
	INIT_LIST_शीर्ष(&p->throttle_गाँठ);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
अटल व्योम detach_task_cfs_rq(काष्ठा task_struct *p);
अटल व्योम dequeue_throttled_task(काष्ठा task_struct *p, पूर्णांक flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_गाँठ);

	/* task blocked after throttled */
	अगर (flags & DEQUEUE_SLEEP) {
		p->throttled = false;
		वापस;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	अगर (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

अटल bool enqueue_throttled_task(काष्ठा task_struct *p)
{
	काष्ठा cfs_rq *cfs_rq = cfs_rq_of(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_empty(&p->throttle_गाँठ));

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
	अगर (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_of(cfs_rq), p)) {
		list_add(&p->throttle_गाँठ, &cfs_rq->throttled_limbo_list);
		वापस true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	वापस false;
}

अटल व्योम enqueue_task_fair(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);
अटल पूर्णांक tg_unthrottle_up(काष्ठा task_group *tg, व्योम *तथ्य)
{
	काष्ठा rq *rq = तथ्य;
	काष्ठा cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));
	काष्ठा task_struct *p, *tmp;
	LIST_शीर्ष(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	अगर (--cfs_rq->throttle_गिनती)
		वापस 0;

	अगर (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_time += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	अगर (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		अगर (WARN_ON_ONCE((s64)delta < 0))
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
	list_for_each_entry_safe(p, tmp, &throttled_tasks, throttle_गाँठ) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		अगर (cfs_rq->throttle_गिनती)
			अवरोध;

		list_del_init(&p->throttle_गाँठ);
		p->throttled = false;
		enqueue_task_fair(rq, p, ENQUEUE_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	अगर (!cfs_rq_is_decayed(cfs_rq))
		list_add_leaf_cfs_rq(cfs_rq);

	वापस 0;
}

अटल अंतरभूत bool task_has_throttle_work(काष्ठा task_struct *p)
{
	वापस p->sched_throttle_work.अग्र != &p->sched_throttle_work;
}

अटल अंतरभूत व्योम task_throttle_setup_work(काष्ठा task_struct *p)
{
	अगर (task_has_throttle_work(p))
		वापस;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	अगर ((p->flags & (PF_EXITING | PF_KTHREAD)))
		वापस;

	task_work_add(p, &p->sched_throttle_work, TWA_RESUME);
}

अटल व्योम record_throttle_clock(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा rq *rq = rq_of(cfs_rq);

	अगर (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	अगर (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

अटल पूर्णांक tg_throttle_down(काष्ठा task_group *tg, व्योम *तथ्य)
{
	काष्ठा rq *rq = तथ्य;
	काष्ठा cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

	अगर (cfs_rq->throttle_गिनती++)
		वापस 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	अगर (!cfs_rq->nr_queued) {
		list_del_leaf_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_empty(&cfs_rq->throttled_limbo_list));
	वापस 0;
}

अटल bool throttle_cfs_rq(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	काष्ठा sched_entity *curr = cfs_rq->h_curr;
	काष्ठा rq *rq = rq_of(cfs_rq);

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
		अगर (curr && curr->on_rq)
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
		अगर (__assign_cfs_rq_runtime(cfs_b, cfs_rq, target_runtime))
			वापस false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_add_पुच्छ_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_वृक्ष_from(cfs_rq->tg, tg_throttle_down, tg_nop, (व्योम *)rq);

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
	अगर (curr && curr->on_rq)
		task_throttle_setup_work(rq->donor);

	वापस true;
}

व्योम unthrottle_cfs_rq(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा rq *rq = rq_of(cfs_rq);
	काष्ठा cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	काष्ठा sched_entity *se = cfs_rq_se(cfs_rq);

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
	अगर (cfs_rq->runtime_enabled && cfs_rq->runtime_remaining <= 0)
		वापस;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->lock) {
		list_del_rcu(&cfs_rq->throttled_list);

		अगर (!cfs_rq->throttled_clock)
			अवरोध;

		cfs_b->throttled_time += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_वृक्ष_from(cfs_rq->tg, tg_nop, tg_unthrottle_up, (व्योम *)rq);

	अगर (!cfs_rq->load.weight) {
		अगर (!cfs_rq->on_list)
			वापस;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			अगर (list_add_leaf_cfs_rq(cfs_rq_of(se)))
				अवरोध;
		}
	}

	assert_list_leaf_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	अगर (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

अटल व्योम __cfsb_csd_unthrottle(व्योम *arg)
{
	काष्ठा cfs_rq *cursor, *tmp;
	काष्ठा rq *rq = arg;

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

		अगर (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

अटल अंतरभूत व्योम __unthrottle_cfs_rq_async(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा rq *rq = rq_of(cfs_rq);
	bool first;

	अगर (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		वापस;
	}

	/* Already enqueued */
	अगर (WARN_ON_ONCE(!list_empty(&cfs_rq->throttled_csd_list)))
		वापस;

	first = list_empty(&rq->cfsb_csd_list);
	list_add_पुच्छ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	अगर (first)
		smp_call_function_single_async(cpu_of(rq), &rq->cfsb_csd);
}

अटल व्योम unthrottle_cfs_rq_async(काष्ठा cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_of(cfs_rq));

	अगर (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_remaining <= 0))
		वापस;

	__unthrottle_cfs_rq_async(cfs_rq);
}

अटल bool distribute_cfs_runtime(काष्ठा cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	पूर्णांक this_cpu = smp_processor_id();
	u64 runtime, remaining = 1;
	काष्ठा cfs_rq *cfs_rq;
	काष्ठा rq *rq;

	guard(rcu)();

	list_for_each_entry_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_of(cfs_rq);

		अगर (!remaining) {
			throttled = true;
			अवरोध;
		}

		guard(rq_lock_irqsave)(rq);

		अगर (!cfs_rq_throttled(cfs_rq))
			जारी;

		/* Already queued for async unthrottle */
		अगर (!list_empty(&cfs_rq->throttled_csd_list))
			जारी;

		अगर (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_remaining > 0);

		scoped_guard(raw_spinlock, &cfs_b->lock) {
			runtime = -cfs_rq->runtime_remaining + 1;
			अगर (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			remaining = cfs_b->runtime;
		}

		cfs_rq->runtime_remaining += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		अगर (cfs_rq->runtime_remaining <= 0) {
			throttled = true;
			अवरोध;
		}

		/* we check whether we're throttled above */
		अगर (cpu_of(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			जारी;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_empty(&rq->cfsb_csd_list);
		list_add_पुच्छ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	अगर (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	वापस throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
अटल पूर्णांक do_sched_cfs_period_timer(काष्ठा cfs_bandwidth *cfs_b, पूर्णांक overrun, अचिन्हित दीर्घ flags)
	__must_hold(&cfs_b->lock)
{
	पूर्णांक throttled;

	/* no need to continue the timer with no bandwidth constraint */
	अगर (cfs_b->quota == RUNTIME_INF)
		जाओ out_deactivate;

	throttled = !list_empty(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	अगर (cfs_b->idle && !throttled)
		जाओ out_deactivate;

	अगर (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		वापस 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	जबतक (throttled && cfs_b->runtime > 0) {
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

	वापस 0;

out_deactivate:
	वापस 1;
}

/* a cfs_rq won't donate quota below this amount */
अटल स्थिर u64 min_cfs_rq_runtime = 1 * NSEC_PER_MSEC;
/* minimum remaining period time to redistribute slack quota */
अटल स्थिर u64 min_bandwidth_expiration = 2 * NSEC_PER_MSEC;
/* how long we wait to gather additional slack before distributing */
अटल स्थिर u64 cfs_bandwidth_slack_period = 5 * NSEC_PER_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
अटल पूर्णांक runtime_refresh_within(काष्ठा cfs_bandwidth *cfs_b, u64 min_expire)
{
	काष्ठा hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 remaining;

	/* if the call-back is running a quota refresh is already occurring */
	अगर (hrtimer_callback_running(refresh_timer))
		वापस 1;

	/* is a quota refresh about to occur? */
	remaining = ktime_to_ns(hrtimer_expires_remaining(refresh_timer));
	अगर (remaining < (s64)min_expire)
		वापस 1;

	वापस 0;
}

अटल व्योम start_cfs_slack_bandwidth(काष्ठा cfs_bandwidth *cfs_b)
{
	u64 min_बायाँ = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	अगर (runtime_refresh_within(cfs_b, min_बायाँ))
		वापस;

	/* don't push forwards an existing deferred unthrottle */
	अगर (cfs_b->slack_started)
		वापस;
	cfs_b->slack_started = true;

	hrtimer_start(&cfs_b->slack_timer,
			ns_to_ktime(cfs_bandwidth_slack_period),
			HRTIMER_MODE_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
अटल व्योम __return_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_remaining - min_cfs_rq_runtime;

	अगर (slack_runtime <= 0)
		वापस;

	guard(raw_spinlock)(&cfs_b->lock);

	अगर (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		अगर (cfs_b->runtime > sched_cfs_bandwidth_slice() &&
		    !list_empty(&cfs_b->throttled_cfs_rq))
			start_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_remaining -= slack_runtime;
}

अटल __always_inline व्योम return_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq)
{
	अगर (!cfs_bandwidth_used())
		वापस;

	अगर (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		वापस;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
अटल व्योम do_sched_cfs_slack_timer(काष्ठा cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->lock) {
		u64 runtime = 0, slice = sched_cfs_bandwidth_slice();

		cfs_b->slack_started = false;

		अगर (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			वापस;

		अगर (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > slice)
			runtime = cfs_b->runtime;

		अगर (!runtime)
			वापस;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
अटल व्योम check_enqueue_throttle(काष्ठा cfs_rq *cfs_rq)
{
	अगर (!cfs_bandwidth_used())
		वापस;

	/* an active group must be handled by the update_curr() path */
	अगर (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		वापस;

	/* ensure the group is not already throttled */
	अगर (cfs_rq_throttled(cfs_rq))
		वापस;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

अटल व्योम sync_throttle(काष्ठा task_group *tg, पूर्णांक cpu)
{
	काष्ठा cfs_rq *pcfs_rq, *cfs_rq;

	अगर (!cfs_bandwidth_used())
		वापस;

	अगर (!tg->parent)
		वापस;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->parent, cpu);

	cfs_rq->throttle_गिनती = pcfs_rq->throttle_गिनती;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	अगर (cfs_rq->throttle_गिनती)
		cfs_rq->pelt_clock_throttled = 1;
}

अटल क्रमागत hrtimer_restart sched_cfs_slack_timer(काष्ठा hrtimer *timer)
{
	काष्ठा cfs_bandwidth *cfs_b =
		container_of(timer, काष्ठा cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	वापस HRTIMER_NORESTART;
}

अटल क्रमागत hrtimer_restart sched_cfs_period_timer(काष्ठा hrtimer *timer)
{
	काष्ठा cfs_bandwidth *cfs_b =
		container_of(timer, काष्ठा cfs_bandwidth, period_timer);
	पूर्णांक overrun;
	पूर्णांक idle = 0;
	पूर्णांक गिनती = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->lock);

	क्रम (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		अगर (!overrun)
			अवरोध;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		अगर (++गिनती > 3) {
			u64 new, old = ktime_to_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			अगर (new < max_bw_quota_period_us * NSEC_PER_USEC) {
				cfs_b->period = ns_to_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->burst *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_PER_USEC),
					div_u64(cfs_b->quota, NSEC_PER_USEC));
			} अन्यथा {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_PER_USEC),
					div_u64(cfs_b->quota, NSEC_PER_USEC));
			}

			/* reset count so we don't come right back in here */
			गिनती = 0;
		}
	}

	अगर (idle) {
		cfs_b->period_active = 0;
		वापस HRTIMER_NORESTART;
	}

	वापस HRTIMER_RESTART;
}

व्योम init_cfs_bandwidth(काष्ठा cfs_bandwidth *cfs_b, काष्ठा cfs_bandwidth *parent)
{
	raw_spin_lock_init(&cfs_b->lock);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_to_ktime(default_bw_period_us());
	cfs_b->burst = 0;
	cfs_b->hierarchical_quota = parent ? parent->hierarchical_quota : RUNTIME_INF;

	INIT_LIST_शीर्ष(&cfs_b->throttled_cfs_rq);
	hrtimer_setup(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_set_expires(&cfs_b->period_timer,
			    get_random_u32_below(cfs_b->period));
	hrtimer_setup(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL);
	cfs_b->slack_started = false;
}

अटल व्योम init_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_शीर्ष(&cfs_rq->throttled_list);
	INIT_LIST_शीर्ष(&cfs_rq->throttled_csd_list);
	INIT_LIST_शीर्ष(&cfs_rq->throttled_limbo_list);
}

व्योम start_cfs_bandwidth(काष्ठा cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->lock);

	अगर (cfs_b->period_active)
		वापस;

	cfs_b->period_active = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_start_expires(&cfs_b->period_timer, HRTIMER_MODE_ABS_PINNED);
}

अटल व्योम destroy_cfs_bandwidth(काष्ठा cfs_bandwidth *cfs_b)
{
	पूर्णांक __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	अगर (!cfs_b->throttled_cfs_rq.अग्र)
		वापस;

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
		काष्ठा rq *rq = cpu_rq(i);

		अगर (list_empty(&rq->cfsb_csd_list))
			जारी;

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
अटल व्योम __maybe_unused update_runtime_enabled(काष्ठा rq *rq)
{
	काष्ठा task_group *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_entry_rcu(tg, &task_groups, list) {
		काष्ठा cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		काष्ठा cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

		scoped_guard(raw_spinlock, &cfs_b->lock)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
अटल व्योम __maybe_unused unthrottle_offline_cfs_rqs(काष्ठा rq *rq)
{
	काष्ठा task_group *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	अगर (cpumask_test_cpu(cpu_of(rq), cpu_active_mask))
		वापस;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_start_loop_update(rq);

	guard(rcu)();

	list_for_each_entry_rcu(tg, &task_groups, list) {
		काष्ठा cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

		अगर (!cfs_rq->runtime_enabled)
			जारी;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		अगर (!cfs_rq_throttled(cfs_rq))
			जारी;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_remaining = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(काष्ठा task_struct *p)
{
	काष्ठा cfs_rq *cfs_rq = task_cfs_rq(p);

	अगर (!cfs_bandwidth_used())
		वापस false;

	अगर (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->hierarchical_quota != RUNTIME_INF)
		वापस true;

	वापस false;
}

#अगर_घोषित CONFIG_NO_HZ_FULL
/* called from pick_next_task_fair() */
अटल व्योम sched_fair_update_stop_tick(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	पूर्णांक cpu = cpu_of(rq);

	अगर (!cfs_bandwidth_used())
		वापस;

	अगर (!tick_nohz_full_cpu(cpu))
		वापस;

	अगर (rq->nr_running != 1)
		वापस;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	अगर (cfs_task_bw_constrained(p))
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#पूर्ण_अगर /* CONFIG_NO_HZ_FULL */

#अन्यथा /* !CONFIG_CFS_BANDWIDTH: */

अटल bool account_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq, u64 delta_exec) { वापस false; }
अटल व्योम check_enqueue_throttle(काष्ठा cfs_rq *cfs_rq) {}
अटल अंतरभूत व्योम sync_throttle(काष्ठा task_group *tg, पूर्णांक cpu) {}
अटल __always_inline व्योम return_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq) {}
अटल व्योम task_throttle_setup_work(काष्ठा task_struct *p) {}
अटल bool task_is_throttled(काष्ठा task_struct *p) { वापस false; }
अटल व्योम dequeue_throttled_task(काष्ठा task_struct *p, पूर्णांक flags) {}
अटल bool enqueue_throttled_task(काष्ठा task_struct *p) { वापस false; }
अटल व्योम record_throttle_clock(काष्ठा cfs_rq *cfs_rq) {}

अटल अंतरभूत पूर्णांक cfs_rq_throttled(काष्ठा cfs_rq *cfs_rq)
{
	वापस 0;
}

अटल अंतरभूत bool cfs_rq_pelt_clock_throttled(काष्ठा cfs_rq *cfs_rq)
{
	वापस false;
}

अटल अंतरभूत पूर्णांक throttled_hierarchy(काष्ठा cfs_rq *cfs_rq)
{
	वापस 0;
}

अटल अंतरभूत पूर्णांक lb_throttled_hierarchy(काष्ठा task_struct *p, पूर्णांक dst_cpu)
{
	वापस 0;
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
व्योम init_cfs_bandwidth(काष्ठा cfs_bandwidth *cfs_b, काष्ठा cfs_bandwidth *parent) {}
अटल व्योम init_cfs_rq_runtime(काष्ठा cfs_rq *cfs_rq) {}
#पूर्ण_अगर

अटल अंतरभूत काष्ठा cfs_bandwidth *tg_cfs_bandwidth(काष्ठा task_group *tg)
{
	वापस शून्य;
}
अटल अंतरभूत व्योम destroy_cfs_bandwidth(काष्ठा cfs_bandwidth *cfs_b) {}
अटल अंतरभूत व्योम update_runtime_enabled(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम unthrottle_offline_cfs_rqs(काष्ठा rq *rq) {}
#अगर_घोषित CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(काष्ठा task_struct *p)
{
	वापस false;
}
#पूर्ण_अगर
#पूर्ण_अगर /* !CONFIG_CFS_BANDWIDTH */

#अगर !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_FULL)
अटल अंतरभूत व्योम sched_fair_update_stop_tick(काष्ठा rq *rq, काष्ठा task_struct *p) {}
#पूर्ण_अगर

/**************************************************
 * CFS operations on tasks:
 */

#अगर_घोषित CONFIG_SCHED_HRTICK
अटल व्योम hrtick_start_fair(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	काष्ठा sched_entity *se = &p->se;
	अचिन्हित दीर्घ scale = 1024;
	अचिन्हित दीर्घ util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	अगर (rq->cfs.h_nr_queued <= 1)
		वापस;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	अगर ((s64)vdelta < 0) {
		अगर (task_current_donor(rq, p))
			resched_curr(rq);
		वापस;
	}
	delta = (se->h_load.weight * vdelta) / NICE_0_LOAD;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	अगर (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_start(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
अटल व्योम hrtick_update(काष्ठा rq *rq)
{
	काष्ठा task_struct *donor = rq->donor;

	अगर (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		वापस;

	अगर (hrtick_active(rq))
		वापस;

	hrtick_start_fair(rq, donor);
}
#अन्यथा /* !CONFIG_SCHED_HRTICK: */
अटल अंतरभूत व्योम
hrtick_start_fair(काष्ठा rq *rq, काष्ठा task_struct *p)
{
}

अटल अंतरभूत व्योम hrtick_update(काष्ठा rq *rq)
{
}
#पूर्ण_अगर /* !CONFIG_SCHED_HRTICK */

अटल अंतरभूत bool cpu_overutilized(पूर्णांक cpu)
{
	अचिन्हित दीर्घ rq_util_max;

	अगर (!sched_energy_enabled())
		वापस false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	वापस !util_fits_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
अटल अंतरभूत bool is_rd_overutilized(काष्ठा मूल_domain *rd)
{
	वापस !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

अटल अंतरभूत व्योम set_rd_overutilized(काष्ठा मूल_domain *rd, bool flag)
{
	अगर (!sched_energy_enabled())
		वापस;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

अटल अंतरभूत व्योम check_update_overutilized_status(काष्ठा rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	अगर (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		set_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
अटल पूर्णांक sched_idle_rq(काष्ठा rq *rq)
{
	वापस unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

अटल पूर्णांक choose_sched_idle_rq(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	वापस sched_idle_rq(rq) && !task_has_idle_policy(p);
}

अटल पूर्णांक choose_idle_cpu(पूर्णांक cpu, काष्ठा task_struct *p)
{
	वापस available_idle_cpu(cpu) ||
	       choose_sched_idle_rq(cpu_rq(cpu), p);
}

अटल व्योम
requeue_delayed_entity(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	अगर (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		अगर (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		place_entity(cfs_rq, se, 0);
		अगर (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_load_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

अटल अचिन्हित दीर्घ enqueue_hierarchy(काष्ठा task_struct *p, पूर्णांक flags)
{
	अचिन्हित दीर्घ weight = NICE_0_LOAD;
	पूर्णांक task_new = !(flags & ENQUEUE_WAKEUP);
	काष्ठा sched_entity *se = &p->se;
	पूर्णांक h_nr_idle = task_has_idle_policy(p);
	पूर्णांक h_nr_runnable = 1;

	अगर (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

		update_curr(cfs_rq);

		अगर (!se->on_rq) {
			enqueue_entity(cfs_rq, se, flags);
		} अन्यथा {
			update_load_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_group(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		अगर (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		weight = __calc_prop_weight(cfs_rq, se, weight);

		flags = ENQUEUE_WAKEUP;
	}

	वापस weight;
}

/* Update curr's vruntime before placing entity or updating lag */
अटल अंतरभूत व्योम update_curr_eevdf(काष्ठा cfs_rq *cfs_rq)
{
	अगर (!cfs_rq->curr)
		वापस;

	update_curr(cfs_rq_of(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
अटल व्योम
enqueue_task_fair(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	पूर्णांक rq_h_nr_queued = rq->cfs.h_nr_queued;
	पूर्णांक task_new = !(flags & ENQUEUE_WAKEUP);
	काष्ठा sched_entity *se = &p->se;
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	अचिन्हित दीर्घ weight;
	bool curr;

	अगर (task_is_throttled(p) && enqueue_throttled_task(p))
		वापस;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	अगर (!p->se.sched_delayed || (flags & ENQUEUE_DELAYED))
		util_est_enqueue(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	अगर (flags & ENQUEUE_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		वापस;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	अगर (p->in_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	अगर (curr)
		place_entity(cfs_rq, se, flags);

	अगर (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	weight = enqueue_hierarchy(p, flags);

	अगर (!curr) {
		reweight_eevdf(cfs_rq, se, weight, false);
		place_entity(cfs_rq, se, flags | ENQUEUE_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	अगर (!rq_h_nr_queued && rq->cfs.h_nr_queued)
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
	अगर (!task_new)
		check_update_overutilized_status(rq);

	assert_list_leaf_cfs_rq(rq);

	hrtick_update(rq);
}

अटल व्योम dequeue_hierarchy(काष्ठा task_struct *p, पूर्णांक flags)
{
	काष्ठा sched_entity *se = &p->se;
	bool task_sleep = flags & DEQUEUE_SLEEP;
	bool task_delayed = flags & DEQUEUE_DELAYED;
	bool task_throttled = flags & DEQUEUE_THROTTLE;
	पूर्णांक h_nr_runnable = 0;
	पूर्णांक h_nr_idle = task_has_idle_policy(p);
	bool dequeue = true;

	अगर (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

		update_curr(cfs_rq);

		अगर (dequeue) {
			dequeue_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			अगर (cfs_rq->load.weight)
				dequeue = false;
		} अन्यथा {
			update_load_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_group(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		अगर (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		अगर (throttled_hierarchy(cfs_rq) && task_throttled)
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
अटल bool __dequeue_task(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	काष्ठा sched_entity *se = &p->se;
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & DEQUEUE_SLEEP;
	bool task_delayed = flags & DEQUEUE_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	अगर (flags & DEQUEUE_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} अन्यथा {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		अगर (flags & (DEQUEUE_SPECIAL | DEQUEUE_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		अगर (sched_feat(DELAY_DEQUEUE) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_load_avg(cfs_rq_of(se), se, UPDATE_UTIL_EST);
			set_delayed(se);
			वापस false;
		}
	}

	dequeue_hierarchy(p, flags);

	अगर (sched_feat(PLACE_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	अगर (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	अगर (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->अग्र_balance = jiffies;

	अगर (task_delayed) {
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

	वापस true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
अटल bool dequeue_task_fair(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	अगर (task_is_throttled(p)) {
		dequeue_throttled_task(p, flags);
		वापस true;
	}

	अगर (!p->se.sched_delayed)
		util_est_dequeue(&rq->cfs, p);

	अगर (!__dequeue_task(rq, p, flags))
		वापस false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	वापस true;
}

अटल अंतरभूत अचिन्हित पूर्णांक cfs_h_nr_delayed(काष्ठा rq *rq)
{
	वापस (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
अटल DEFINE_PER_CPU(cpumask_var_t, load_balance_mask);
अटल DEFINE_PER_CPU(cpumask_var_t, select_rq_mask);
अटल DEFINE_PER_CPU(cpumask_var_t, should_we_balance_tmpmask);

#अगर_घोषित CONFIG_NO_HZ_COMMON

अटल काष्ठा {
	cpumask_var_t idle_cpus_mask;
	पूर्णांक has_blocked_load;		/* Idle CPUS has blocked load */
	पूर्णांक needs_update;		/* Newly idle CPUs need their next_balance collated */
	अचिन्हित दीर्घ अग्र_balance;     /* in jiffy units */
	अचिन्हित दीर्घ अग्र_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#पूर्ण_अगर /* CONFIG_NO_HZ_COMMON */

अटल अचिन्हित दीर्घ cpu_load(काष्ठा rq *rq)
{
	वापस cfs_rq_load_avg(&rq->cfs);
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
अटल अचिन्हित दीर्घ cpu_load_without(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	काष्ठा cfs_rq *cfs_rq;
	अचिन्हित पूर्णांक load;

	/* Task has no contribution or is new */
	अगर (cpu_of(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		वापस cpu_load(rq);

	cfs_rq = &rq->cfs;
	load = READ_ONCE(cfs_rq->avg.load_avg);

	/* Discount task's util from CPU's util */
	lsub_positive(&load, task_h_load(p));

	वापस load;
}

अटल अचिन्हित दीर्घ cpu_runnable(काष्ठा rq *rq)
{
	वापस cfs_rq_runnable_avg(&rq->cfs);
}

अटल अचिन्हित दीर्घ cpu_runnable_without(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	काष्ठा cfs_rq *cfs_rq;
	अचिन्हित पूर्णांक runnable;

	/* Task has no contribution or is new */
	अगर (cpu_of(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		वापस cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_positive(&runnable, p->se.avg.runnable_avg);

	वापस runnable;
}

अटल अचिन्हित दीर्घ capacity_of(पूर्णांक cpu)
{
	वापस cpu_rq(cpu)->cpu_capacity;
}

अटल व्योम record_wakee(काष्ठा task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	अगर (time_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	अगर (current->last_wakee != p) {
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
अटल पूर्णांक wake_wide(काष्ठा task_struct *p)
{
	अचिन्हित पूर्णांक master = current->wakee_flips;
	अचिन्हित पूर्णांक slave = p->wakee_flips;
	पूर्णांक factor = __this_cpu_read(sd_llc_आकार);

	अगर (master < slave)
		swap(master, slave);
	अगर (slave < factor || master < slave * factor)
		वापस 0;
	वापस 1;
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
अटल पूर्णांक
wake_affine_idle(पूर्णांक this_cpu, पूर्णांक पूर्व_cpu, पूर्णांक sync)
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
	अगर (available_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, पूर्व_cpu))
		वापस available_idle_cpu(पूर्व_cpu) ? पूर्व_cpu : this_cpu;

	अगर (sync) {
		काष्ठा rq *rq = cpu_rq(this_cpu);

		अगर ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			वापस this_cpu;
	}

	अगर (available_idle_cpu(पूर्व_cpu))
		वापस पूर्व_cpu;

	वापस nr_cpumask_bits;
}

अटल पूर्णांक
wake_affine_weight(काष्ठा sched_domain *sd, काष्ठा task_struct *p,
		   पूर्णांक this_cpu, पूर्णांक पूर्व_cpu, पूर्णांक sync)
{
	s64 this_eff_load, पूर्व_eff_load;
	अचिन्हित दीर्घ task_load;

	this_eff_load = cpu_load(cpu_rq(this_cpu));

	अगर (sync) {
		अचिन्हित दीर्घ current_load = task_h_load(current);

		अगर (current_load > this_eff_load)
			वापस this_cpu;

		this_eff_load -= current_load;
	}

	task_load = task_h_load(p);

	this_eff_load += task_load;
	अगर (sched_feat(WA_BIAS))
		this_eff_load *= 100;
	this_eff_load *= capacity_of(पूर्व_cpu);

	पूर्व_eff_load = cpu_load(cpu_rq(पूर्व_cpu));
	पूर्व_eff_load -= task_load;
	अगर (sched_feat(WA_BIAS))
		पूर्व_eff_load *= 100 + (sd->imbalance_pct - 100) / 2;
	पूर्व_eff_load *= capacity_of(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	अगर (sync)
		पूर्व_eff_load += 1;

	वापस this_eff_load < पूर्व_eff_load ? this_cpu : nr_cpumask_bits;
}

अटल पूर्णांक wake_affine(काष्ठा sched_domain *sd, काष्ठा task_struct *p,
		       पूर्णांक this_cpu, पूर्णांक पूर्व_cpu, पूर्णांक sync)
{
	पूर्णांक target = nr_cpumask_bits;

	अगर (sched_feat(WA_IDLE))
		target = wake_affine_idle(this_cpu, पूर्व_cpu, sync);

	अगर (sched_feat(WA_WEIGHT) && target == nr_cpumask_bits)
		target = wake_affine_weight(sd, p, this_cpu, पूर्व_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	अगर (target != this_cpu)
		वापस पूर्व_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	वापस target;
}

अटल काष्ठा sched_group *
sched_balance_find_dst_group(काष्ठा sched_domain *sd, काष्ठा task_struct *p, पूर्णांक this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
अटल पूर्णांक
sched_balance_find_dst_group_cpu(काष्ठा sched_group *group, काष्ठा task_struct *p, पूर्णांक this_cpu)
{
	अचिन्हित दीर्घ load, min_load = अच_दीर्घ_उच्च;
	u64 min_exit_latency = U64_MAX;
	अचिन्हित पूर्णांक nr_candidates = 0;
	पूर्णांक least_loaded_cpu = this_cpu;
	पूर्णांक shallowest_idle_cpu = -1;
	पूर्णांक i;

	/* Check if we have any choice: */
	अगर (group->group_weight == 1)
		वापस cpumask_first(sched_group_span(group));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_group_span(group), p->cpus_ptr) {
		काष्ठा rq *rq = cpu_rq(i);

		अगर (!sched_core_cookie_match(rq, p))
			जारी;

		अगर (choose_sched_idle_rq(rq, p))
			वापस i;

		अगर (available_idle_cpu(i)) {
			काष्ठा cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			अगर (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} अन्यथा अगर (exit_latency == min_exit_latency) {
				nr_candidates++;
				अगर (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} अन्यथा अगर (shallowest_idle_cpu == -1) {
			load = cpu_load(cpu_rq(i));
			अगर (load < min_load) {
				min_load = load;
				least_loaded_cpu = i;
			}
		}
	}

	वापस shallowest_idle_cpu != -1 ? shallowest_idle_cpu : least_loaded_cpu;
}

अटल अंतरभूत पूर्णांक sched_balance_find_dst_cpu(काष्ठा sched_domain *sd, काष्ठा task_struct *p,
				  पूर्णांक cpu, पूर्णांक पूर्व_cpu, पूर्णांक sd_flag)
{
	पूर्णांक new_cpu = cpu;

	अगर (!cpumask_intersects(sched_domain_span(sd), p->cpus_ptr))
		वापस पूर्व_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	अगर (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_load_avg(&p->se);

	जबतक (sd) {
		काष्ठा sched_group *group;
		काष्ठा sched_domain *tmp;
		पूर्णांक weight;

		अगर (!(sd->flags & sd_flag)) {
			sd = sd->child;
			जारी;
		}

		group = sched_balance_find_dst_group(sd, p, cpu);
		अगर (!group) {
			sd = sd->child;
			जारी;
		}

		new_cpu = sched_balance_find_dst_group_cpu(group, p, cpu);
		अगर (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->child;
			जारी;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		weight = sd->span_weight;
		sd = शून्य;
		for_each_domain(cpu, tmp) {
			अगर (weight <= tmp->span_weight)
				अवरोध;
			अगर (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	वापस new_cpu;
}

अटल अंतरभूत पूर्णांक __select_idle_cpu(पूर्णांक cpu, काष्ठा task_struct *p)
{
	अगर (choose_idle_cpu(cpu, p) && sched_cpu_cookie_match(cpu_rq(cpu), p))
		वापस cpu;

	वापस -1;
}

DEFINE_STATIC_कुंजी_FALSE(sched_smt_present);
EXPORT_प्रतीक_GPL(sched_smt_present);

अटल अंतरभूत व्योम set_idle_cores(पूर्णांक cpu, पूर्णांक val)
{
	काष्ठा sched_domain_shared *sds;

	sds = rcu_dereference_all(per_cpu(sd_balance_shared, cpu));
	अगर (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

अटल अंतरभूत bool test_idle_cores(पूर्णांक cpu)
{
	काष्ठा sched_domain_shared *sds;

	sds = rcu_dereference_all(per_cpu(sd_balance_shared, cpu));
	अगर (sds)
		वापस READ_ONCE(sds->has_idle_cores);

	वापस false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
अटल अंतरभूत पूर्णांक select_idle_smt_cpu(काष्ठा task_struct *p, पूर्णांक cpu)
{
	काष्ठा sched_domain *sd;
	पूर्णांक best = cpu;
	पूर्णांक sibling;

	अगर (!sched_smt_active())
		वापस cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	अगर (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		वापस cpu;

	for_each_cpu_and(sibling, sched_domain_span(sd), p->cpus_ptr) {
		अगर (sibling == best || !choose_idle_cpu(sibling, p))
			जारी;

		अगर (sched_asym_prefer(sibling, best))
			best = sibling;
	}

	वापस best;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
व्योम __update_idle_core(काष्ठा rq *rq)
{
	पूर्णांक core = cpu_of(rq);
	पूर्णांक cpu;

	rcu_read_lock();
	अगर (test_idle_cores(core))
		जाओ unlock;

	for_each_cpu(cpu, cpu_smt_mask(core)) {
		अगर (cpu == core)
			जारी;

		अगर (!available_idle_cpu(cpu))
			जाओ unlock;
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
अटल पूर्णांक select_idle_core(काष्ठा task_struct *p, पूर्णांक core, काष्ठा cpumask *cpus, पूर्णांक *idle_cpu)
{
	bool idle = true;
	पूर्णांक cpu;

	for_each_cpu(cpu, cpu_smt_mask(core)) {
		अगर (!available_idle_cpu(cpu)) {
			idle = false;
			अगर (*idle_cpu == -1) {
				अगर (choose_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					अवरोध;
				}
				जारी;
			}
			अवरोध;
		}
		अगर (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	अगर (idle)
		वापस core;

	cpumask_andnot(cpus, cpus, cpu_smt_mask(core));
	वापस -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
अटल पूर्णांक select_idle_smt(काष्ठा task_struct *p, काष्ठा sched_domain *sd, पूर्णांक target)
{
	पूर्णांक cpu;

	for_each_cpu_and(cpu, cpu_smt_mask(target), p->cpus_ptr) {
		अगर (cpu == target)
			जारी;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		अगर (!cpumask_test_cpu(cpu, sched_domain_span(sd)))
			जारी;
		अगर (choose_idle_cpu(cpu, p))
			वापस cpu;
	}

	वापस -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
अटल पूर्णांक select_idle_cpu(काष्ठा task_struct *p, काष्ठा sched_domain *sd, bool has_idle_core, पूर्णांक target)
{
	काष्ठा cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	पूर्णांक i, cpu, idle_cpu = -1, nr = पूर्णांक_उच्च;

	अगर (sched_feat(SIS_UTIL) && sd->shared) {
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
		अगर (nr == 1)
			वापस -1;
	}

	अगर (!cpumask_and(cpus, sched_domain_span(sd), p->cpus_ptr))
		वापस -1;

	अगर (static_branch_unlikely(&sched_cluster_active)) {
		काष्ठा sched_group *sg = sd->groups;

		अगर (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_group_span(sg), target + 1) {
				अगर (!cpumask_test_cpu(cpu, cpus))
					जारी;

				अगर (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					अगर ((अचिन्हित पूर्णांक)i < nr_cpumask_bits)
						वापस i;
				} अन्यथा {
					अगर (--nr <= 0)
						वापस -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					अगर ((अचिन्हित पूर्णांक)idle_cpu < nr_cpumask_bits)
						वापस idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_group_span(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, target + 1) {
		अगर (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			अगर ((अचिन्हित पूर्णांक)i < nr_cpumask_bits)
				वापस i;

		} अन्यथा {
			अगर (--nr <= 0)
				वापस -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			अगर ((अचिन्हित पूर्णांक)idle_cpu < nr_cpumask_bits)
				अवरोध;
		}
	}

	अगर (has_idle_core)
		set_idle_cores(target, false);

	वापस idle_cpu;
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
क्रमागत asym_fits_state {
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
अटल पूर्णांक
select_idle_capacity(काष्ठा task_struct *p, काष्ठा sched_domain *sd, पूर्णांक target)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_active() && test_idle_cores(target);
	अचिन्हित दीर्घ task_util, util_min, util_max, best_cap = 0;
	पूर्णांक fits, best_fits = ASYM_IDLE_THREAD_MISFIT;
	पूर्णांक cpu, best_cpu = -1;
	काष्ठा cpumask *cpus;
	पूर्णांक nr = पूर्णांक_उच्च;

	cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	cpumask_and(cpus, sched_domain_span(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_मूल्य(p, UCLAMP_MIN);
	util_max = uclamp_eff_मूल्य(p, UCLAMP_MAX);

	अगर (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_scan) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		अगर (nr == 1)
			वापस -1;
	}

	for_each_cpu_wrap(cpu, cpus, target) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		अचिन्हित दीर्घ cpu_cap = capacity_of(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		अगर (!has_idle_core && --nr <= 0)
			वापस best_cpu;

		अगर (!choose_idle_cpu(cpu, p))
			जारी;

		fits = util_fits_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		अगर (fits > 0 && preferred_core)
			वापस cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		अन्यथा अगर (fits < 0)
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
		अन्यथा अगर (fits > 0)
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
		अगर (preferred_core)
			fits += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		अगर ((fits < best_fits) ||
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
	अगर (has_idle_core && best_fits > ASYM_IDLE_COMPLETE_MISFIT)
		set_idle_cores(target, false);

	वापस best_cpu;
}

अटल अंतरभूत bool asym_fits_cpu(अचिन्हित दीर्घ util,
				 अचिन्हित दीर्घ util_min,
				 अचिन्हित दीर्घ util_max,
				 पूर्णांक cpu)
{
	अगर (sched_asym_cpucap_active()) {
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
		वापस (!sched_smt_active() || is_core_idle(cpu)) &&
		       (util_fits_cpu(util, util_min, util_max, cpu) > 0);
	}

	वापस true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
अटल पूर्णांक select_idle_sibling(काष्ठा task_struct *p, पूर्णांक पूर्व, पूर्णांक target)
{
	bool has_idle_core = false;
	काष्ठा sched_domain *sd;
	अचिन्हित दीर्घ task_util, util_min, util_max;
	पूर्णांक i, recent_used_cpu, पूर्व_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	अगर (sched_asym_cpucap_active()) {
		sync_entity_load_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_मूल्य(p, UCLAMP_MIN);
		util_max = uclamp_eff_मूल्य(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	अगर (choose_idle_cpu(target, p) &&
	    asym_fits_cpu(task_util, util_min, util_max, target))
		जाओ select_smt_priority;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	अगर (पूर्व != target && cpus_share_cache(पूर्व, target) &&
	    choose_idle_cpu(पूर्व, p) &&
	    asym_fits_cpu(task_util, util_min, util_max, पूर्व)) {

		अगर (!static_branch_unlikely(&sched_cluster_active) ||
		    cpus_share_resources(पूर्व, target)) {
			target = पूर्व;
			जाओ select_smt_priority;
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
	अगर (is_per_cpu_kthread(current) &&
	    in_task() &&
	    पूर्व == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_fits_cpu(task_util, util_min, util_max, पूर्व)) {
		target = पूर्व;
		जाओ select_smt_priority;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_used_cpu = p->recent_used_cpu;
	p->recent_used_cpu = पूर्व;
	अगर (recent_used_cpu != पूर्व &&
	    recent_used_cpu != target &&
	    cpus_share_cache(recent_used_cpu, target) &&
	    choose_idle_cpu(recent_used_cpu, p) &&
	    cpumask_test_cpu(recent_used_cpu, p->cpus_ptr) &&
	    asym_fits_cpu(task_util, util_min, util_max, recent_used_cpu)) {

		अगर (!static_branch_unlikely(&sched_cluster_active) ||
		    cpus_share_resources(recent_used_cpu, target)) {
			target = recent_used_cpu;
			जाओ select_smt_priority;
		}

	} अन्यथा {
		recent_used_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	अगर (sched_asym_cpucap_active()) {
		sd = rcu_dereference_all(per_cpu(sd_asym_cpucapacity, target));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		अगर (sd) {
			i = select_idle_capacity(p, sd, target);
			अगर ((अचिन्हित पूर्णांक)i < nr_cpumask_bits) {
				target = i;
				जाओ select_smt_priority;
			}
			वापस target;
		}
	}

	sd = rcu_dereference_all(per_cpu(sd_llc, target));
	अगर (!sd)
		वापस target;

	अगर (sched_smt_active()) {
		has_idle_core = test_idle_cores(target);

		अगर (!has_idle_core && cpus_share_cache(पूर्व, target)) {
			i = select_idle_smt(p, sd, पूर्व);
			अगर ((अचिन्हित पूर्णांक)i < nr_cpumask_bits) {
				target = i;
				जाओ select_smt_priority;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, target);
	अगर ((अचिन्हित पूर्णांक)i < nr_cpumask_bits) {
		target = i;
		जाओ select_smt_priority;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	अगर ((अचिन्हित पूर्णांक)पूर्व_aff < nr_cpumask_bits) {
		target = पूर्व_aff;
		जाओ select_smt_priority;
	}
	अगर ((अचिन्हित पूर्णांक)recent_used_cpu < nr_cpumask_bits) {
		target = recent_used_cpu;
		जाओ select_smt_priority;
	}

	वापस target;

select_smt_priority:
	वापस select_idle_smt_cpu(p, target);
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
अटल अचिन्हित दीर्घ
cpu_util(पूर्णांक cpu, काष्ठा task_struct *p, पूर्णांक dst_cpu, पूर्णांक boost)
{
	bool add_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	काष्ठा cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	अचिन्हित दीर्घ util = READ_ONCE(cfs_rq->avg.util_avg);
	अचिन्हित दीर्घ runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	अगर (add_task)
		util += task_util(p);
	अन्यथा अगर (sub_task)
		lsub_positive(&util, task_util(p));

	अगर (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		अगर (add_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		अन्यथा अगर (sub_task)
			lsub_positive(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	अगर (sched_feat(UTIL_EST)) {
		अचिन्हित दीर्घ util_est;

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
		अगर (dst_cpu == cpu)
			util_est += _task_util_est(p);
		अन्यथा अगर (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_positive(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	वापस min(util, arch_scale_cpu_capacity(cpu));
}

अचिन्हित दीर्घ cpu_util_cfs(पूर्णांक cpu)
{
	वापस cpu_util(cpu, शून्य, -1, 0);
}

अचिन्हित दीर्घ cpu_util_cfs_boost(पूर्णांक cpu)
{
	वापस cpu_util(cpu, शून्य, -1, 1);
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
अटल अचिन्हित दीर्घ cpu_util_without(पूर्णांक cpu, काष्ठा task_struct *p)
{
	/* Task has no contribution or is new */
	अगर (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		p = शून्य;

	वापस cpu_util(cpu, p, -1, 0);
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
अचिन्हित दीर्घ effective_cpu_util(पूर्णांक cpu, अचिन्हित दीर्घ util_cfs,
				 अचिन्हित दीर्घ *min,
				 अचिन्हित दीर्घ *max)
{
	अचिन्हित दीर्घ util, irq, scale;
	काष्ठा rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_capacity(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	अगर (unlikely(irq >= scale)) {
		अगर (min)
			*min = scale;
		अगर (max)
			*max = scale;
		वापस scale;
	}

	अगर (min) {
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
		अगर (!uclamp_is_used() && rt_rq_is_runnable(&rq->rt))
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
	अगर (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	अगर (util >= scale)
		वापस scale;

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

	वापस min(scale, util);
}

अचिन्हित दीर्घ sched_cpu_util(पूर्णांक cpu)
{
	वापस effective_cpu_util(cpu, cpu_util_cfs(cpu), शून्य, शून्य);
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
काष्ठा energy_env {
	अचिन्हित दीर्घ task_busy_time;
	अचिन्हित दीर्घ pd_busy_time;
	अचिन्हित दीर्घ cpu_cap;
	अचिन्हित दीर्घ pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
अटल अंतरभूत व्योम eenv_task_busy_time(काष्ठा energy_env *eenv,
				       काष्ठा task_struct *p, पूर्णांक पूर्व_cpu)
{
	अचिन्हित दीर्घ busy_time, max_cap = arch_scale_cpu_capacity(पूर्व_cpu);
	अचिन्हित दीर्घ irq = cpu_util_irq(cpu_rq(पूर्व_cpu));

	अगर (unlikely(irq >= max_cap))
		busy_time = max_cap;
	अन्यथा
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
अटल अंतरभूत व्योम eenv_pd_busy_time(काष्ठा energy_env *eenv,
				     काष्ठा cpumask *pd_cpus,
				     काष्ठा task_struct *p)
{
	अचिन्हित दीर्घ busy_time = 0;
	पूर्णांक cpu;

	for_each_cpu(cpu, pd_cpus) {
		अचिन्हित दीर्घ util = cpu_util(cpu, p, -1, 0);

		busy_time += effective_cpu_util(cpu, util, शून्य, शून्य);
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
अटल अंतरभूत अचिन्हित दीर्घ
eenv_pd_max_util(काष्ठा energy_env *eenv, काष्ठा cpumask *pd_cpus,
		 काष्ठा task_struct *p, पूर्णांक dst_cpu)
{
	अचिन्हित दीर्घ max_util = 0;
	पूर्णांक cpu;

	for_each_cpu(cpu, pd_cpus) {
		काष्ठा task_struct *tsk = (cpu == dst_cpu) ? p : शून्य;
		अचिन्हित दीर्घ util = cpu_util(cpu, p, dst_cpu, 1);
		अचिन्हित दीर्घ eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		अगर (tsk && uclamp_is_used()) {
			min = max(min, uclamp_eff_मूल्य(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			अगर (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_मूल्य(p, UCLAMP_MAX);
			अन्यथा
				max = max(max, uclamp_eff_मूल्य(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	वापस min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
अटल अंतरभूत अचिन्हित दीर्घ
compute_energy(काष्ठा energy_env *eenv, काष्ठा perf_domain *pd,
	       काष्ठा cpumask *pd_cpus, काष्ठा task_struct *p, पूर्णांक dst_cpu)
{
	अचिन्हित दीर्घ max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	अचिन्हित दीर्घ busy_time = eenv->pd_busy_time;
	अचिन्हित दीर्घ energy;

	अगर (dst_cpu >= 0)
		busy_time = min(eenv->pd_cap, busy_time + eenv->task_busy_time);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_time, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_time);

	वापस energy;
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
अटल पूर्णांक find_energy_efficient_cpu(काष्ठा task_struct *p, पूर्णांक पूर्व_cpu)
{
	काष्ठा cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	अचिन्हित दीर्घ पूर्व_delta = अच_दीर्घ_उच्च, best_delta = अच_दीर्घ_उच्च;
	अचिन्हित दीर्घ p_util_min = uclamp_is_used() ? uclamp_eff_मूल्य(p, UCLAMP_MIN) : 0;
	अचिन्हित दीर्घ p_util_max = uclamp_is_used() ? uclamp_eff_मूल्य(p, UCLAMP_MAX) : 1024;
	काष्ठा मूल_domain *rd = this_rq()->rd;
	पूर्णांक cpu, best_energy_cpu, target = -1;
	पूर्णांक पूर्व_fits = -1, best_fits = -1;
	अचिन्हित दीर्घ best_actual_cap = 0;
	अचिन्हित दीर्घ पूर्व_actual_cap = 0;
	काष्ठा sched_domain *sd;
	काष्ठा perf_domain *pd;
	काष्ठा energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	अगर (!pd)
		वापस target;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	जबतक (sd && !cpumask_test_cpu(पूर्व_cpu, sched_domain_span(sd)))
		sd = sd->parent;
	अगर (!sd)
		वापस target;

	target = पूर्व_cpu;

	sync_entity_load_avg(&p->se);
	अगर (!task_util_est(p) && p_util_min == 0)
		वापस target;

	eenv_task_busy_time(&eenv, p, पूर्व_cpu);

	क्रम (; pd; pd = pd->अग्र) {
		अचिन्हित दीर्घ util_min = p_util_min, util_max = p_util_max;
		अचिन्हित दीर्घ cpu_cap, cpu_actual_cap, util;
		दीर्घ पूर्व_spare_cap = -1, max_spare_cap = -1;
		अचिन्हित दीर्घ rq_util_min, rq_util_max;
		अचिन्हित दीर्घ cur_delta, base_energy;
		पूर्णांक max_spare_cap_cpu = -1;
		पूर्णांक fits, max_fits = -1;

		अगर (!cpumask_and(cpus, perf_domain_span(pd), cpu_online_mask))
			जारी;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_first(cpus);
		cpu_actual_cap = get_actual_cpu_capacity(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			काष्ठा rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			अगर (!cpumask_test_cpu(cpu, sched_domain_span(sd)))
				जारी;

			अगर (!cpumask_test_cpu(cpu, p->cpus_ptr))
				जारी;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = capacity_of(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			अगर (uclamp_is_used() && !uclamp_rq_is_idle(rq)) {
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
			अगर (!fits)
				जारी;

			lsub_positive(&cpu_cap, util);

			अगर (cpu == पूर्व_cpu) {
				/* Always use prev_cpu as a candidate. */
				पूर्व_spare_cap = cpu_cap;
				पूर्व_fits = fits;
			} अन्यथा अगर ((fits > max_fits) ||
				   ((fits == max_fits) && ((दीर्घ)cpu_cap > max_spare_cap))) {
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

		अगर (max_spare_cap_cpu < 0 && पूर्व_spare_cap < 0)
			जारी;

		eenv_pd_busy_time(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		base_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		अगर (पूर्व_spare_cap > -1) {
			पूर्व_delta = compute_energy(&eenv, pd, cpus, p,
						    पूर्व_cpu);
			/* CPU utilization has changed */
			अगर (पूर्व_delta < base_energy)
				वापस target;
			पूर्व_delta -= base_energy;
			पूर्व_actual_cap = cpu_actual_cap;
			best_delta = min(best_delta, पूर्व_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		अगर (max_spare_cap_cpu >= 0 && max_spare_cap > पूर्व_spare_cap) {
			/* Current best energy cpu fits better */
			अगर (max_fits < best_fits)
				जारी;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			अगर ((max_fits < 0) &&
			    (cpu_actual_cap <= best_actual_cap))
				जारी;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_spare_cap_cpu);
			/* CPU utilization has changed */
			अगर (cur_delta < base_energy)
				वापस target;
			cur_delta -= base_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			अगर ((max_fits > 0) && (best_fits > 0) &&
			    (cur_delta >= best_delta))
				जारी;

			best_delta = cur_delta;
			best_energy_cpu = max_spare_cap_cpu;
			best_fits = max_fits;
			best_actual_cap = cpu_actual_cap;
		}
	}

	अगर ((best_fits > पूर्व_fits) ||
	    ((best_fits > 0) && (best_delta < पूर्व_delta)) ||
	    ((best_fits < 0) && (best_actual_cap > पूर्व_actual_cap)))
		target = best_energy_cpu;

	वापस target;
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
अटल पूर्णांक
select_task_rq_fair(काष्ठा task_struct *p, पूर्णांक पूर्व_cpu, पूर्णांक wake_flags)
{
	पूर्णांक sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	काष्ठा sched_domain *tmp, *sd = शून्य;
	पूर्णांक cpu = smp_processor_id();
	पूर्णांक new_cpu = पूर्व_cpu;
	पूर्णांक want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	पूर्णांक sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_lock);
	अगर (wake_flags & WF_TTWU) {
		record_wakee(p);

		अगर ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			वापस cpu;

		अगर (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = find_energy_efficient_cpu(p, पूर्व_cpu);
			अगर (new_cpu >= 0)
				वापस new_cpu;
			new_cpu = पूर्व_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		अगर (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(पूर्व_cpu, sched_domain_span(tmp))) {
			अगर (cpu != पूर्व_cpu)
				new_cpu = wake_affine(tmp, p, cpu, पूर्व_cpu, sync);

			sd = शून्य; /* Prefer wake_affine over balance flags */
			अवरोध;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		अगर (tmp->flags & sd_flag)
			sd = tmp;
		अन्यथा अगर (!want_affine)
			अवरोध;
	}

	/* Slow path */
	अगर (unlikely(sd)) {
		new_cpu = sched_balance_find_dst_cpu(sd, p, cpu, पूर्व_cpu, sd_flag);
		वापस select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	अगर (wake_flags & WF_TTWU)
		वापस select_idle_sibling(p, पूर्व_cpu, new_cpu);

	वापस new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
अटल व्योम migrate_task_rq_fair(काष्ठा task_struct *p, पूर्णांक new_cpu)
{
	काष्ठा sched_entity *se = &p->se;

	अगर (!task_on_rq_migrating(p)) {
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

अटल व्योम task_dead_fair(काष्ठा task_struct *p)
{
	काष्ठा sched_entity *se = &p->se;
	remove_entity_load_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
अटल व्योम set_task_max_allowed_capacity(काष्ठा task_struct *p)
{
	काष्ठा asym_cap_तथ्य *entry;

	अगर (!sched_asym_cpucap_active())
		वापस;

	rcu_read_lock();
	list_for_each_entry_rcu(entry, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_capacity_span(entry);
		अगर (!cpumask_intersects(p->cpus_ptr, cpumask))
			जारी;

		p->max_allowed_capacity = entry->capacity;
		अवरोध;
	}
	rcu_read_unlock();
}

अटल व्योम set_cpus_allowed_fair(काष्ठा task_struct *p, काष्ठा affinity_context *ctx)
{
	set_cpus_allowed_common(p, ctx);
	set_task_max_allowed_capacity(p);
}

क्रमागत preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

अटल अंतरभूत bool set_preempt_buddy(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	अगर (cfs_rq->अग्र && entity_before(cfs_rq->अग्र, pse))
		वापस false;

	set_अग्र_buddy(cfs_rq, pse);
	वापस true;
}

अटल अंतरभूत bool set_short_buddy(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *pse)
{
	अगर (cfs_rq->अग्र) {
		अगर (cfs_rq->अग्र->slice < pse->slice)
			वापस false;

		अगर (cfs_rq->अग्र->slice == pse->slice &&
		    entity_before(cfs_rq->अग्र, pse))
			वापस false;
	}

	set_अग्र_buddy(cfs_rq, pse);
	वापस true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
अटल अंतरभूत क्रमागत preempt_wakeup_action
preempt_sync(काष्ठा rq *rq, पूर्णांक wake_flags,
	     काष्ठा sched_entity *pse, काष्ठा sched_entity *se)
{
	u64 threshold, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	threshold = sysctl_sched_migration_cost;
	delta = rq_clock_task(rq) - se->exec_start;
	अगर ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	अगर (wake_flags & WF_RQ_SELECTED)
		threshold >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	अगर (entity_before(pse, se) && delta >= threshold)
		वापस PREEMPT_WAKEUP_RESCHED;

	वापस PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
अटल व्योम wakeup_preempt_fair(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक wake_flags)
{
	क्रमागत preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	काष्ठा task_struct *donor = rq->donor;
	काष्ठा sched_entity *nse, *se = &donor->se, *pse = &p->se;
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	पूर्णांक cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	अगर (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		वापस;

	अगर (unlikely(se == pse))
		वापस;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	अगर (task_is_throttled(p))
		वापस;

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
	अगर (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		वापस;

	अगर (!sched_feat(WAKEUP_PREEMPTION))
		वापस;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	अगर (cse_is_idle && !pse_is_idle)
		जाओ preempt;

	update_curr_fair(rq);

	अगर (cse_is_idle != pse_is_idle)
		जाओ update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	अगर (unlikely(!normal_policy(p->policy)))
		जाओ update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	अगर (pse->sched_delayed)
		जाओ update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	अगर (sched_feat(PREEMPT_SHORT) && (pse->slice < se->slice)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		जाओ pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	अगर (wake_flags & WF_FORK)
		जाओ update;

	/* Prefer picking wakee soon if appropriate. */
	अगर (sched_feat(अग्र_BUDDY) && set_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		अगर (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	चयन (preempt_action) {
	हाल PREEMPT_WAKEUP_NONE:
		वापस;
	हाल PREEMPT_WAKEUP_RESCHED:
		जाओ preempt;
	हाल PREEMPT_WAKEUP_SHORT:
		fallthrough;
	हाल PREEMPT_WAKEUP_PICK:
		अवरोध;
	}

pick:
	अगर (cfs_rq->h_nr_queued) {
		nse = pick_अग्र_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		अगर (unlikely(!nse))
			जाओ pick;

		/* If @p has become the most eligible task, force preemption */
		अगर (nse == pse)
			जाओ preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	अगर (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		जाओ preempt;
update:
	अगर (sched_feat(RUN_TO_PARITY))
		update_protect_slice(cfs_rq, se);

	वापस;

preempt:
	cancel_protect_slice(se);

	अगर (preempt_action == PREEMPT_WAKEUP_SHORT)
		set_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

काष्ठा task_struct *pick_task_fair(काष्ठा rq *rq, काष्ठा rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	काष्ठा sched_entity *se;
	काष्ठा task_struct *p;
	पूर्णांक new_tasks;

again:
	अगर (!cfs_rq->h_nr_queued)
		जाओ idle;

	/* Might not have done put_prev_entity() */
	अगर (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_अग्र_entity(rq, true);
	अगर (!se)
		जाओ again;

	p = task_of(se);
	वापस p;

idle:
	अगर (sched_core_enabled(rq))
		वापस शून्य;

	new_tasks = sched_balance_newidle(rq, rf);
	अगर (new_tasks < 0)
		वापस RETRY_TASK;
	अगर (new_tasks > 0)
		जाओ again;
	वापस शून्य;
}

अटल काष्ठा task_struct *
fair_server_pick_task(काष्ठा sched_dl_entity *dl_se, काष्ठा rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	वापस pick_task_fair(dl_se->rq, rf);
}

व्योम fair_server_init(काष्ठा rq *rq)
{
	काष्ठा sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
अटल व्योम put_पूर्व_task_fair(काष्ठा rq *rq, काष्ठा task_struct *पूर्व, काष्ठा task_struct *अग्र)
{
	काष्ठा sched_entity *se = &पूर्व->se;
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	काष्ठा sched_entity *nse = शून्य;

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	अगर (अग्र && अग्र->sched_class == &fair_sched_class)
		nse = &अग्र->se;
#पूर्ण_अगर

	जबतक (se) {
		cfs_rq = cfs_rq_of(se);
		अगर (!nse || cfs_rq->h_curr)
			put_पूर्व_entity(cfs_rq, se);
#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
		अगर (nse) {
			अगर (is_same_group(se, nse))
				अवरोध;

			पूर्णांक d = nse->depth - se->depth;
			अगर (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = parent_entity(nse);
				/* if nse is the deeper, do not ascend se */
				अगर (d > 0)
					जारी;
			}
		}
#पूर्ण_अगर
		se = parent_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &पूर्व->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = शून्य;
	अगर (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
अटल व्योम yield_task_fair(काष्ठा rq *rq)
{
	काष्ठा task_struct *curr = rq->donor;
	काष्ठा sched_entity *se = &curr->se;
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	अगर (unlikely(rq->nr_running == 1))
		वापस;

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
	अगर (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

अटल bool yield_to_task_fair(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	काष्ठा sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	अगर (!se->on_rq || se->sched_delayed)
		वापस false;

	/* Tell the scheduler that we'd really like se to run next. */
	set_अग्र_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	वापस true;
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

अटल अचिन्हित दीर्घ __read_mostly max_load_balance_interval = HZ/10;

क्रमागत fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
क्रमागत group_type {
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

क्रमागत migration_type {
	migrate_load = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#घोषणा LBF_ALL_PINNED	0x01
#घोषणा LBF_NEED_BREAK	0x02
#घोषणा LBF_DST_PINNED  0x04
#घोषणा LBF_SOME_PINNED	0x08
#घोषणा LBF_ACTIVE_LB	0x10
#घोषणा LBF_LLC_PINNED	0x20
#घोषणा LBF_ACTIVE_LB_LLC	0x40

काष्ठा lb_env {
	काष्ठा sched_domain	*sd;

	काष्ठा rq		*src_rq;
	पूर्णांक			src_cpu;

	पूर्णांक			dst_cpu;
	काष्ठा rq		*dst_rq;
	bool			dst_core_idle;

	काष्ठा cpumask		*dst_grpmask;
	पूर्णांक			new_dst_cpu;
	क्रमागत cpu_idle_type	idle;
	दीर्घ			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	काष्ठा cpumask		*cpus;

	अचिन्हित पूर्णांक		flags;

	अचिन्हित पूर्णांक		loop;
	अचिन्हित पूर्णांक		loop_break;
	अचिन्हित पूर्णांक		loop_max;

	क्रमागत fbq_type		fbq_type;
	क्रमागत migration_type	migration_type;
	काष्ठा list_शीर्ष	tasks;
};

/*
 * Is this task likely cache-hot:
 */
अटल पूर्णांक task_hot(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	अगर (p->sched_class != &fair_sched_class)
		वापस 0;

	अगर (unlikely(task_has_idle_policy(p)))
		वापस 0;

	/* SMT siblings share cache */
	अगर (env->sd->flags & SD_SHARE_CPUCAPACITY)
		वापस 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	अगर (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_of(&p->se)->अग्र))
		वापस 1;

	अगर (sysctl_sched_migration_cost == -1)
		वापस 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	अगर (!sched_core_cookie_match(cpu_rq(env->dst_cpu), p))
		वापस 1;

	अगर (sysctl_sched_migration_cost == 0)
		वापस 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_start;

	वापस delta < (s64)sysctl_sched_migration_cost;
}

#अगर_घोषित CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
अटल दीर्घ migrate_degrades_locality(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	काष्ठा numa_group *numa_group = rcu_dereference_all(p->numa_group);
	अचिन्हित दीर्घ src_weight, dst_weight;
	पूर्णांक src_nid, dst_nid, dist;

	अगर (!static_branch_likely(&sched_numa_balancing))
		वापस 0;

	अगर (!p->numa_faults || !(env->sd->flags & SD_NUMA))
		वापस 0;

	src_nid = cpu_to_गाँठ(env->src_cpu);
	dst_nid = cpu_to_गाँठ(env->dst_cpu);

	अगर (src_nid == dst_nid)
		वापस 0;

	/* Migrating away from the preferred node is always bad. */
	अगर (src_nid == p->numa_preferred_nid) {
		अगर (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			वापस 1;
		अन्यथा
			वापस 0;
	}

	/* Encourage migration to the preferred node. */
	अगर (dst_nid == p->numa_preferred_nid)
		वापस -1;

	/* Leaving a core idle is often worse than degrading locality. */
	अगर (env->idle == CPU_IDLE)
		वापस 0;

	dist = गाँठ_distance(src_nid, dst_nid);
	अगर (numa_group) {
		src_weight = group_weight(p, src_nid, dist);
		dst_weight = group_weight(p, dst_nid, dist);
	} अन्यथा {
		src_weight = task_weight(p, src_nid, dist);
		dst_weight = task_weight(p, dst_nid, dist);
	}

	वापस src_weight - dst_weight;
}

#अन्यथा /* !CONFIG_NUMA_BALANCING: */
अटल अंतरभूत दीर्घ migrate_degrades_locality(काष्ठा task_struct *p,
					     काष्ठा lb_env *env)
{
	वापस 0;
}
#पूर्ण_अगर /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
अटल अंतरभूत पूर्णांक task_is_ineligible_on_dst_cpu(काष्ठा task_struct *p, पूर्णांक dest_cpu)
{
	काष्ठा cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	अगर (sched_feat(PLACE_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		वापस 1;

	वापस 0;
}

#अगर_घोषित CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
अटल bool fits_llc_capacity(अचिन्हित दीर्घ util, अचिन्हित दीर्घ max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	अगर (cpu_smt_अंक_threads == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	वापस util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#घोषणा util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

अटल __maybe_unused bool get_llc_stats(पूर्णांक cpu, अचिन्हित दीर्घ *util,
					 अचिन्हित दीर्घ *cap)
{
	काष्ठा sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(per_cpu(sd_llc_shared, cpu));
	अगर (!sd_share)
		वापस false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->capacity);

	वापस true;
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
क्रमागत llc_mig {
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
अटल क्रमागत llc_mig can_migrate_llc(पूर्णांक src_cpu, पूर्णांक dst_cpu,
				    अचिन्हित दीर्घ tsk_util,
				    bool to_pref)
{
	अचिन्हित दीर्घ src_util, dst_util, src_cap, dst_cap;

	अगर (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		वापस mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	अगर (!fits_llc_capacity(dst_util, dst_cap) &&
	    !fits_llc_capacity(src_util, src_cap))
		वापस mig_unrestricted;

	अगर (to_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		अगर (!fits_llc_capacity(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			वापस mig_forbid;
	} अन्यथा {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		अगर (fits_llc_capacity(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			वापस mig_forbid;
	}
	वापस mig_llc;
}

अटल अंतरभूत bool task_misfits_asym_cpu(काष्ठा lb_env *env, काष्ठा task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	अगर ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_fits_cpu(p, env->dst_cpu) &&
	    task_fits_cpu(p, env->src_cpu))
		वापस true;

	वापस false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
अटल क्रमागत llc_mig can_migrate_llc_task(काष्ठा lb_env *env,
					 काष्ठा task_struct *p)
{
	काष्ठा sched_cache_group *grp;
	bool to_pref;
	पूर्णांक cpu, src_cpu, dst_cpu;

	अगर (task_misfits_asym_cpu(env, p))
		वापस mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	अगर (!grp)
		वापस mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	अगर (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		वापस mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	अगर (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_capacity(grp, dst_cpu)) {
		अगर (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		वापस mig_unrestricted;
	}

	अगर (cpus_share_cache(dst_cpu, cpu))
		to_pref = true;
	अन्यथा अगर (cpus_share_cache(src_cpu, cpu))
		to_pref = false;
	अन्यथा
		वापस mig_unrestricted;

	वापस can_migrate_llc(src_cpu, dst_cpu,
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
अटल अंतरभूत bool
alb_break_llc(काष्ठा lb_env *env)
{
	अगर (!sched_cache_enabled())
		वापस false;

	अगर (cpus_share_cache(env->src_cpu, env->dst_cpu))
		वापस false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	अगर (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		अचिन्हित दीर्घ util = 0;
		काष्ठा task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		अगर (env->migration_type == migrate_misfit)
			वापस false;

		अगर (env->src_rq->nr_running <= 1)
			वापस true;

		cur = rcu_dereference_all(env->src_rq->curr);
		अगर (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		अगर (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			वापस true;
	}

	वापस false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
अटल अंतरभूत bool
migrate_llc_task_wrong_dst(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	वापस sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_ACTIVE_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
अटल bool migrate_degrades_llc(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	अगर (!sched_cache_enabled())
		वापस false;

	अगर (task_has_sched_core(p))
		वापस false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	अगर (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		वापस false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	अगर (migrate_llc_task_wrong_dst(p, env))
		वापस true;

	अगर (can_migrate_llc_task(env, p) != mig_forbid)
		वापस false;

	वापस true;
}

#अन्यथा
अटल अंतरभूत bool get_llc_stats(पूर्णांक cpu, अचिन्हित दीर्घ *util,
				 अचिन्हित दीर्घ *cap)
{
	वापस false;
}

अटल अंतरभूत bool
alb_break_llc(काष्ठा lb_env *env)
{
	वापस false;
}

अटल अंतरभूत bool
migrate_llc_task_wrong_dst(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	वापस false;
}

अटल अंतरभूत bool
migrate_degrades_llc(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	वापस false;
}
#पूर्ण_अगर
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
अटल
पूर्णांक can_migrate_task(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	दीर्घ degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	अगर (p->sched_task_hot)
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
	अगर ((p->se.sched_delayed) && (env->migration_type != migrate_load))
		वापस 0;

	अगर (lb_throttled_hierarchy(p, env->dst_cpu))
		वापस 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	अगर (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		वापस 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	अगर (kthread_is_per_cpu(p))
		वापस 0;

	अगर (task_is_blocked(p))
		वापस 0;

	अगर (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		पूर्णांक cpu;

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
		अगर (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_ACTIVE_LB))
			वापस 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_first_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		अगर (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		वापस 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	अगर (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		वापस 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	अगर (env->flags & LBF_ACTIVE_LB)
		वापस !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	अगर (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		अगर (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			अगर (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			वापस 0;
		}

		hot = task_hot(p, env);
	} अन्यथा {
		hot = degrades > 0;
	}

	अगर (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		अगर (hot)
			p->sched_task_hot = 1;
		वापस 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	वापस 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
अटल व्योम detach_task(काष्ठा task_struct *p, काष्ठा lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	अगर (p->sched_task_hot) {
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
अटल काष्ठा task_struct *detach_one_task(काष्ठा lb_env *env)
{
	काष्ठा task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_entry_reverse(p,
			&env->src_rq->cfs_tasks, se.group_गाँठ) {
		अगर (!can_migrate_task(p, env))
			जारी;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		वापस p;
	}
	वापस शून्य;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
अटल पूर्णांक detach_tasks(काष्ठा lb_env *env)
{
	काष्ठा list_शीर्ष *tasks = &env->src_rq->cfs_tasks;
	अचिन्हित दीर्घ util, load;
	काष्ठा task_struct *p;
	पूर्णांक detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	अगर (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		वापस 0;
	}

	अगर (env->imbalance <= 0)
		वापस 0;

	जबतक (!list_empty(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		अगर (env->idle && env->src_rq->nr_running <= 1)
			अवरोध;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		अगर (env->loop > env->loop_max)
			अवरोध;

		/* take a breather every nr_migrate tasks */
		अगर (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			अवरोध;
		}

		p = list_last_entry(tasks, काष्ठा task_struct, se.group_गाँठ);

		अगर (!can_migrate_task(p, env))
			जाओ अग्र;

		चयन (env->migration_type) {
		हाल migrate_load:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			load = max_t(अचिन्हित दीर्घ, task_h_load(p), 1);

			अगर (sched_feat(LB_MIN) &&
			    load < 16 && !env->sd->nr_balance_failed)
				जाओ अग्र;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			अगर (shr_bound(load, env->sd->nr_balance_failed) > env->imbalance)
				जाओ अग्र;

			env->imbalance -= load;
			अवरोध;

		हाल migrate_util:
			util = task_util_est(p);

			अगर (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				जाओ अग्र;

			env->imbalance -= util;
			अवरोध;

		हाल migrate_task:
			env->imbalance--;
			अवरोध;

		हाल migrate_misfit:
			/* This is not a misfit task */
			अगर (task_fits_cpu(p, env->src_cpu))
				जाओ अग्र;

			env->imbalance = 0;
			अवरोध;

		हाल migrate_llc_task:
			env->imbalance--;
			अवरोध;
		}

		detach_task(p, env);
		list_add(&p->se.group_गाँठ, &env->tasks);

		detached++;

#अगर_घोषित CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		अगर (env->idle == CPU_NEWLY_IDLE)
			अवरोध;
#पूर्ण_अगर

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		अगर (env->imbalance <= 0)
			अवरोध;

		जारी;
अग्र:
		अगर (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.group_गाँठ, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_add(env->sd->lb_gained[env->idle], detached);

	वापस detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
अटल व्योम attach_tasks(काष्ठा lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#अगर_घोषित CONFIG_NO_HZ_COMMON
अटल अंतरभूत bool cfs_rq_has_blocked_load(काष्ठा cfs_rq *cfs_rq)
{
	अगर (cfs_rq->avg.load_avg)
		वापस true;

	अगर (cfs_rq->avg.util_avg)
		वापस true;

	वापस false;
}

अटल अंतरभूत bool others_have_blocked(काष्ठा rq *rq)
{
	अगर (cpu_util_rt(rq))
		वापस true;

	अगर (cpu_util_dl(rq))
		वापस true;

	अगर (hw_load_avg(rq))
		वापस true;

	अगर (cpu_util_irq(rq))
		वापस true;

	वापस false;
}

अटल अंतरभूत व्योम update_blocked_load_tick(काष्ठा rq *rq)
{
	WRITE_ONCE(rq->last_blocked_load_update_tick, jiffies);
}

अटल अंतरभूत व्योम update_has_blocked_load_status(काष्ठा rq *rq, bool has_blocked_load)
{
	अगर (!has_blocked_load)
		rq->has_blocked_load = 0;
}
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
अटल अंतरभूत bool cfs_rq_has_blocked_load(काष्ठा cfs_rq *cfs_rq) { वापस false; }
अटल अंतरभूत bool others_have_blocked(काष्ठा rq *rq) { वापस false; }
अटल अंतरभूत व्योम update_blocked_load_tick(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम update_has_blocked_load_status(काष्ठा rq *rq, bool has_blocked_load) {}
#पूर्ण_अगर /* !CONFIG_NO_HZ_COMMON */

अटल bool __update_blocked_others(काष्ठा rq *rq, bool *done)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_load_avgs(rq);

	अगर (others_have_blocked(rq))
		*done = false;

	वापस updated;
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED

अटल bool __update_blocked_fair(काष्ठा rq *rq, bool *done)
{
	काष्ठा cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_leaf_cfs_rq_safe(rq, cfs_rq, pos) {
		काष्ठा sched_entity *se;

		अगर (update_cfs_rq_load_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_load_avg(cfs_rq);

			अगर (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			अगर (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		अगर (se && !skip_blocked_update(se))
			update_load_avg(cfs_rq_of(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		अगर (cfs_rq_is_decayed(cfs_rq))
			list_del_leaf_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		अगर (cfs_rq_has_blocked_load(cfs_rq))
			*done = false;
	}

	वापस decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
अटल व्योम update_cfs_rq_h_load(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा sched_entity *se = cfs_rq_se(cfs_rq);
	अचिन्हित दीर्घ now = jiffies;
	अचिन्हित दीर्घ load;

	अगर (cfs_rq->last_h_load_update == now)
		वापस;

	WRITE_ONCE(cfs_rq->h_load_अग्र, शून्य);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);
		WRITE_ONCE(cfs_rq->h_load_अग्र, se);
		अगर (cfs_rq->last_h_load_update == now)
			अवरोध;
	}

	अगर (!se) {
		cfs_rq->h_load = cfs_rq_load_avg(cfs_rq);
		cfs_rq->last_h_load_update = now;
	}

	जबतक ((se = READ_ONCE(cfs_rq->h_load_अग्र)) != शून्य) {
		load = cfs_rq->h_load;
		load = div64_ul(load * se->avg.load_avg,
				cfs_rq_load_avg(cfs_rq) + 1);
		cfs_rq = group_cfs_rq(se);
		cfs_rq->h_load = load;
		cfs_rq->last_h_load_update = now;
	}
}

अटल अचिन्हित दीर्घ task_h_load(काष्ठा task_struct *p)
{
	काष्ठा cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_load(cfs_rq);
	वापस div64_ul(p->se.avg.load_avg * cfs_rq->h_load,
			cfs_rq_load_avg(cfs_rq) + 1);
}
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
अटल bool __update_blocked_fair(काष्ठा rq *rq, bool *done)
{
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_load_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	अगर (cfs_rq_has_blocked_load(cfs_rq))
		*done = false;

	वापस decayed;
}

अटल अचिन्हित दीर्घ task_h_load(काष्ठा task_struct *p)
{
	वापस p->se.avg.load_avg;
}
#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

अटल व्योम __sched_balance_update_blocked_averages(काष्ठा rq *rq)
{
	bool decayed = false, done = true;

	update_blocked_load_tick(rq);

	decayed |= __update_blocked_others(rq, &done);
	decayed |= __update_blocked_fair(rq, &done);

	update_has_blocked_load_status(rq, !done);
	अगर (decayed)
		cpufreq_update_util(rq, 0);
}

अटल व्योम sched_balance_update_blocked_averages(पूर्णांक cpu)
{
	काष्ठा rq *rq = cpu_rq(cpu);

	guard(rq_lock_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
काष्ठा sg_lb_stats {
	अचिन्हित दीर्घ avg_load;			/* Avg load            over the CPUs of the group */
	अचिन्हित दीर्घ group_load;		/* Total load          over the CPUs of the group */
	अचिन्हित दीर्घ group_capacity;		/* Capacity            over the CPUs of the group */
	अचिन्हित दीर्घ group_util;		/* Total utilization   over the CPUs of the group */
	अचिन्हित दीर्घ group_runnable;		/* Total runnable time over the CPUs of the group */
	अचिन्हित पूर्णांक योग_nr_running;		/* Nr of all tasks running in the group */
	अचिन्हित पूर्णांक योग_h_nr_running;		/* Nr of CFS tasks running in the group */
	अचिन्हित पूर्णांक idle_cpus;                 /* Nr of idle CPUs         in the group */
	अचिन्हित पूर्णांक group_weight;
	क्रमागत group_type group_type;
	अचिन्हित पूर्णांक group_asym_packing;	/* Tasks should be moved to preferred CPU */
	अचिन्हित पूर्णांक group_smt_balance;		/* Task on busy SMT be moved */
	अचिन्हित पूर्णांक group_llc_balance;		/* Tasks should be moved to preferred LLC */
	अचिन्हित दीर्घ group_misfit_task_load;	/* A CPU has a task too big for its capacity */
	अचिन्हित पूर्णांक group_overutilized;	/* At least one CPU is overutilized in the group */
#अगर_घोषित CONFIG_NUMA_BALANCING
	अचिन्हित पूर्णांक nr_numa_running;
	अचिन्हित पूर्णांक nr_preferred_running;
#पूर्ण_अगर
#अगर_घोषित CONFIG_SCHED_CACHE
	अचिन्हित पूर्णांक nr_pref_dst_llc;
#पूर्ण_अगर
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
काष्ठा sd_lb_stats {
	काष्ठा sched_group *busiest;		/* Busiest group in this sd */
	काष्ठा sched_group *local;		/* Local group in this sd */
	अचिन्हित दीर्घ योग_load;		/* Total load of all groups in sd */
	अचिन्हित दीर्घ योग_capacity;		/* Total capacity of all groups in sd */
	अचिन्हित दीर्घ avg_load;			/* Average load across all groups in sd */
	अचिन्हित पूर्णांक prefer_sibling;		/* Tasks should go to sibling first */

	काष्ठा sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	काष्ठा sg_lb_stats local_stat;		/* Statistics of the local group */
};

अटल अंतरभूत व्योम init_sd_lb_stats(काष्ठा sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (काष्ठा sd_lb_stats){
		.busiest = शून्य,
		.local = शून्य,
		.योग_load = 0UL,
		.योग_capacity = 0UL,
		.busiest_stat = {
			.idle_cpus = अच_पूर्णांक_उच्च,
			.group_type = group_has_spare,
		},
	};
}

अटल अचिन्हित दीर्घ scale_rt_capacity(पूर्णांक cpu)
{
	अचिन्हित दीर्घ max = get_actual_cpu_capacity(cpu);
	काष्ठा rq *rq = cpu_rq(cpu);
	अचिन्हित दीर्घ used, मुक्त;
	अचिन्हित दीर्घ irq;

	irq = cpu_util_irq(rq);

	अगर (unlikely(irq >= max))
		वापस 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	used = cpu_util_rt(rq);
	used += cpu_util_dl(rq);

	अगर (unlikely(used >= max))
		वापस 1;

	मुक्त = max - used;

	वापस scale_irq_capacity(मुक्त, irq, max);
}

अटल व्योम update_cpu_capacity(काष्ठा sched_domain *sd, पूर्णांक cpu)
{
	अचिन्हित दीर्घ capacity = scale_rt_capacity(cpu);
	काष्ठा sched_group *sdg = sd->groups;

	अगर (!capacity)
		capacity = 1;

	cpu_rq(cpu)->cpu_capacity = capacity;
	trace_sched_cpu_capacity_tp(cpu_rq(cpu));

	sdg->sgc->capacity = capacity;
	sdg->sgc->min_capacity = capacity;
	sdg->sgc->max_capacity = capacity;
}

व्योम update_group_capacity(काष्ठा sched_domain *sd, पूर्णांक cpu)
{
	काष्ठा sched_domain *child = sd->child;
	काष्ठा sched_group *group, *sdg = sd->groups;
	अचिन्हित दीर्घ capacity, min_capacity, max_capacity;
	अचिन्हित दीर्घ interval;

	interval = msecs_to_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_load_balance_interval);
	sdg->sgc->अग्र_update = jiffies + interval;

	अगर (!child) {
		update_cpu_capacity(sd, cpu);
		वापस;
	}

	capacity = 0;
	min_capacity = अच_दीर्घ_उच्च;
	max_capacity = 0;

	अगर (child->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_group_span(sdg)) {
			अचिन्हित दीर्घ cpu_cap = capacity_of(cpu);

			capacity += cpu_cap;
			min_capacity = min(cpu_cap, min_capacity);
			max_capacity = max(cpu_cap, max_capacity);
		}
	} अन्यथा  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		group = child->groups;
		करो {
			काष्ठा sched_group_capacity *sgc = group->sgc;

			capacity += sgc->capacity;
			min_capacity = min(sgc->min_capacity, min_capacity);
			max_capacity = max(sgc->max_capacity, max_capacity);
			group = group->अग्र;
		} जबतक (group != child->groups);
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
अटल अंतरभूत पूर्णांक
check_cpu_capacity(काष्ठा rq *rq, काष्ठा sched_domain *sd)
{
	वापस ((rq->cpu_capacity * sd->imbalance_pct) <
				(arch_scale_cpu_capacity(cpu_of(rq)) * 100));
}

/* Check if the rq has a misfit task */
अटल अंतरभूत bool check_misfit_status(काष्ठा rq *rq)
{
	वापस rq->misfit_task_load;
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

अटल अंतरभूत पूर्णांक sg_imbalanced(काष्ठा sched_group *group)
{
	वापस group->sgc->imbalance;
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
अटल अंतरभूत bool
group_has_capacity(अचिन्हित पूर्णांक imbalance_pct, काष्ठा sg_lb_stats *sgs)
{
	अगर (sgs->योग_nr_running < sgs->group_weight)
		वापस true;

	अगर ((sgs->group_capacity * imbalance_pct) <
			(sgs->group_runnable * 100))
		वापस false;

	अगर ((sgs->group_capacity * 100) >
			(sgs->group_util * imbalance_pct))
		वापस true;

	वापस false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
अटल अंतरभूत bool
group_is_overloaded(अचिन्हित पूर्णांक imbalance_pct, काष्ठा sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	अगर (sched_energy_enabled() && !sgs->group_overutilized)
		वापस false;

	अगर (sgs->योग_nr_running <= sgs->group_weight)
		वापस false;

	अगर ((sgs->group_capacity * 100) <
			(sgs->group_util * imbalance_pct))
		वापस true;

	अगर ((sgs->group_capacity * imbalance_pct) <
			(sgs->group_runnable * 100))
		वापस true;

	वापस false;
}

अटल अंतरभूत क्रमागत
group_type group_classify(अचिन्हित पूर्णांक imbalance_pct,
			  काष्ठा sched_group *group,
			  काष्ठा sg_lb_stats *sgs)
{
	अगर (group_is_overloaded(imbalance_pct, sgs))
		वापस group_overloaded;

	अगर (sgs->group_llc_balance)
		वापस group_llc_balance;

	अगर (sg_imbalanced(group))
		वापस group_imbalanced;

	अगर (sgs->group_asym_packing)
		वापस group_asym_packing;

	अगर (sgs->group_smt_balance)
		वापस group_smt_balance;

	अगर (sgs->group_misfit_task_load)
		वापस group_misfit_task;

	अगर (!group_has_capacity(imbalance_pct, sgs))
		वापस group_fully_busy;

	वापस group_has_spare;
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
अटल bool sched_use_asym_prio(काष्ठा sched_domain *sd, पूर्णांक cpu)
{
	अगर (!(sd->flags & SD_ASYM_PACKING))
		वापस false;

	अगर (!sched_smt_active())
		वापस true;

	वापस sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

अटल अंतरभूत bool sched_asym(काष्ठा sched_domain *sd, पूर्णांक dst_cpu, पूर्णांक src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	वापस sched_use_asym_prio(sd, dst_cpu) &&
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
अटल अंतरभूत bool
sched_group_asym(काष्ठा lb_env *env, काष्ठा sg_lb_stats *sgs, काष्ठा sched_group *group)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	अगर ((group->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->group_weight - sgs->idle_cpus != 1))
		वापस false;

	वापस sched_asym(env->sd, env->dst_cpu, READ_ONCE(group->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
अटल अंतरभूत bool smt_vs_nonsmt_groups(काष्ठा sched_group *sg1,
				    काष्ठा sched_group *sg2)
{
	अगर (!sg1 || !sg2)
		वापस false;

	वापस (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

अटल अंतरभूत bool smt_balance(काष्ठा lb_env *env, काष्ठा sg_lb_stats *sgs,
			       काष्ठा sched_group *group)
{
	अगर (!env->idle)
		वापस false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	अगर (group->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->योग_h_nr_running > 1)
		वापस true;

	वापस false;
}

अटल अंतरभूत दीर्घ sibling_imbalance(काष्ठा lb_env *env,
				    काष्ठा sd_lb_stats *sds,
				    काष्ठा sg_lb_stats *busiest,
				    काष्ठा sg_lb_stats *local)
{
	पूर्णांक ncores_busiest, ncores_local;
	दीर्घ imbalance;

	अगर (!env->idle || !busiest->योग_nr_running)
		वापस 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	अगर (ncores_busiest == ncores_local) {
		imbalance = busiest->योग_nr_running;
		lsub_positive(&imbalance, local->योग_nr_running);
		वापस imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->योग_nr_running;
	lsub_positive(&imbalance, ncores_busiest * local->योग_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	अगर (imbalance <= 1 && local->योग_nr_running == 0 &&
	    busiest->योग_nr_running > 1)
		imbalance = 2;

	वापस imbalance;
}

अटल अंतरभूत bool
sched_reduced_capacity(काष्ठा rq *rq, काष्ठा sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	अगर (rq->cfs.h_nr_runnable != 1)
		वापस false;

	वापस check_cpu_capacity(rq, sd);
}

#अगर_घोषित CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
अटल व्योम record_sg_llc_stats(काष्ठा lb_env *env,
				काष्ठा sg_lb_stats *sgs,
				काष्ठा sched_group *group)
{
	काष्ठा sched_domain_shared *sd_share;
	पूर्णांक cpu;

	अगर (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		वापस;

	/* Only care about sched domain spanning multiple LLCs */
	अगर (env->sd->child != rcu_dereference_all(per_cpu(sd_llc, env->dst_cpu)))
		वापस;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	अगर ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_group_span(group), env->cpus))
		वापस;

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
	अगर (!sd_share)
		वापस;

	अगर (READ_ONCE(sd_share->util_avg) != sgs->group_util)
		WRITE_ONCE(sd_share->util_avg, sgs->group_util);

	अगर (unlikely(READ_ONCE(sd_share->capacity) != sgs->group_capacity))
		WRITE_ONCE(sd_share->capacity, sgs->group_capacity);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
अटल अंतरभूत bool llc_balance(काष्ठा lb_env *env, काष्ठा sg_lb_stats *sgs,
			       काष्ठा sched_group *group)
{
	अगर (!sched_cache_enabled())
		वापस false;

	अगर (env->sd->flags & SD_SHARE_LLC)
		वापस false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	अगर ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->group_misfit_task_load)
		वापस false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	अगर (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		वापस false;

	अगर (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_first(sched_group_span(group)),
			    env->dst_cpu, 0, true) == mig_llc)
		वापस true;

	वापस false;
}

अटल bool update_llc_busiest(काष्ठा lb_env *env,
			       काष्ठा sg_lb_stats *busiest,
			       काष्ठा sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	वापस sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#अन्यथा
अटल अंतरभूत व्योम record_sg_llc_stats(काष्ठा lb_env *env, काष्ठा sg_lb_stats *sgs,
				       काष्ठा sched_group *group)
{
}

अटल अंतरभूत bool llc_balance(काष्ठा lb_env *env, काष्ठा sg_lb_stats *sgs,
			       काष्ठा sched_group *group)
{
	वापस false;
}

अटल bool update_llc_busiest(काष्ठा lb_env *env,
			       काष्ठा sg_lb_stats *busiest,
			       काष्ठा sg_lb_stats *sgs)
{
	वापस false;
}
#पूर्ण_अगर

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
अटल अंतरभूत व्योम update_sg_lb_stats(काष्ठा lb_env *env,
				      काष्ठा sd_lb_stats *sds,
				      काष्ठा sched_group *group,
				      काष्ठा sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	पूर्णांक i, nr_running, local_group, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->parent;

	स_रखो(sgs, 0, माप(*sgs));

	local_group = group == sds->local;

	for_each_cpu_and(i, sched_group_span(group), env->cpus) {
		काष्ठा rq *rq = cpu_rq(i);
		अचिन्हित दीर्घ load = cpu_load(rq);

		sgs->group_load += load;
		sgs->group_util += cpu_util_cfs(i);
		sgs->group_runnable += cpu_runnable(rq);
		sgs->योग_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->योग_nr_running += nr_running;

		अगर (cpu_overutilized(i))
			sgs->group_overutilized = 1;

#अगर_घोषित CONFIG_SCHED_CACHE
		अगर (sched_cache_enabled()) {
			काष्ठा sched_domain *sd_tmp;
			पूर्णांक dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			अगर (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				अगर (sd_tmp && (अचिन्हित पूर्णांक)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#पूर्ण_अगर

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		अगर (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			जारी;
		}

		/* Overload indicator is only updated at root domain */
		अगर (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#अगर_घोषित CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		अगर (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#पूर्ण_अगर
		अगर (local_group)
			जारी;

		अगर (sd_flags & SD_ASYM_CPUCAPACITY) {
			अगर (rq->misfit_task_load) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				अगर (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				अगर (capacity_greater(capacity_of(env->dst_cpu),
						     group->sgc->max_capacity) &&
				    (sgs->group_misfit_task_load < rq->misfit_task_load))
					sgs->group_misfit_task_load = rq->misfit_task_load;
			}
		} अन्यथा अगर (env->idle && sched_reduced_capacity(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			अगर (sgs->group_misfit_task_load < load)
				sgs->group_misfit_task_load = load;
		}
	}

	sgs->group_capacity = group->sgc->capacity;

	sgs->group_weight = group->group_weight;

	अगर (!local_group) {
		/* Check if dst CPU is idle and preferred to this group */
		अगर (env->idle && sgs->योग_h_nr_running &&
		    sched_group_asym(env, sgs, group))
			sgs->group_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		अगर (smt_balance(env, sgs, group))
			sgs->group_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		अगर (llc_balance(env, sgs, group))
			sgs->group_llc_balance = 1;
	}

	sgs->group_type = group_classify(env->sd->imbalance_pct, group, sgs);

	record_sg_llc_stats(env, sgs, group);
	/* Computing avg_load makes sense only when group is overloaded */
	अगर (sgs->group_type == group_overloaded)
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
अटल bool update_sd_pick_busiest(काष्ठा lb_env *env,
				   काष्ठा sd_lb_stats *sds,
				   काष्ठा sched_group *sg,
				   काष्ठा sg_lb_stats *sgs)
{
	काष्ठा sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	अगर (!sgs->योग_h_nr_running)
		वापस false;

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
	अगर ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->group_type == group_misfit_task) &&
	    (!env->dst_core_idle ||
	     !capacity_greater(capacity_of(env->dst_cpu), sg->sgc->max_capacity) ||
	     sds->local_stat.group_type != group_has_spare))
		वापस false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	अगर ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->group_type <= group_fully_busy) &&
	    (capacity_greater(sg->sgc->min_capacity, capacity_of(env->dst_cpu))))
		वापस false;

	अगर (sgs->group_type > busiest->group_type)
		वापस true;

	अगर (sgs->group_type < busiest->group_type)
		वापस false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	चयन (sgs->group_type) {
	हाल group_overloaded:
		/* Select the overloaded group with highest avg_load. */
		वापस sgs->avg_load > busiest->avg_load;

	हाल group_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		वापस update_llc_busiest(env, busiest, sgs);

	हाल group_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		वापस false;

	हाल group_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		वापस sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	हाल group_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		वापस sgs->group_misfit_task_load > busiest->group_misfit_task_load;

	हाल group_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		अगर (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			जाओ has_spare;

		fallthrough;

	हाल group_fully_busy:
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

		अगर (sgs->avg_load < busiest->avg_load)
			वापस false;

		अगर (sgs->avg_load == busiest->avg_load) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			अगर (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				वापस false;
		}

		अवरोध;

	हाल group_has_spare:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		अगर (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			अगर (sg->flags & SD_SHARE_CPUCAPACITY && sgs->योग_h_nr_running <= 1)
				वापस false;
			अन्यथा
				वापस true;
		}
has_spare:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		अगर (sgs->idle_cpus > busiest->idle_cpus)
			वापस false;
		अन्यथा अगर ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->योग_nr_running <= busiest->योग_nr_running))
			वापस false;

		अवरोध;
	}

	वापस true;
}

#अगर_घोषित CONFIG_NUMA_BALANCING
अटल अंतरभूत क्रमागत fbq_type fbq_classify_group(काष्ठा sg_lb_stats *sgs)
{
	अगर (sgs->योग_h_nr_running > sgs->nr_numa_running)
		वापस regular;
	अगर (sgs->योग_h_nr_running > sgs->nr_preferred_running)
		वापस remote;
	वापस all;
}

अटल अंतरभूत क्रमागत fbq_type fbq_classify_rq(काष्ठा rq *rq)
{
	अगर (rq->nr_running > rq->nr_numa_running)
		वापस regular;
	अगर (rq->nr_running > rq->nr_preferred_running)
		वापस remote;
	वापस all;
}
#अन्यथा /* !CONFIG_NUMA_BALANCING: */
अटल अंतरभूत क्रमागत fbq_type fbq_classify_group(काष्ठा sg_lb_stats *sgs)
{
	वापस all;
}

अटल अंतरभूत क्रमागत fbq_type fbq_classify_rq(काष्ठा rq *rq)
{
	वापस regular;
}
#पूर्ण_अगर /* !CONFIG_NUMA_BALANCING */


काष्ठा sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

अटल अचिन्हित पूर्णांक task_running_on_cpu(पूर्णांक cpu, काष्ठा task_struct *p)
{
	/* Task has no contribution or is new */
	अगर (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		वापस 0;

	अगर (task_on_rq_queued(p))
		वापस 1;

	वापस 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
अटल पूर्णांक idle_cpu_without(पूर्णांक cpu, काष्ठा task_struct *p)
{
	काष्ठा rq *rq = cpu_rq(cpu);

	अगर (rq->curr != rq->idle && rq->curr != p)
		वापस 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	अगर (rq->ttwu_pending)
		वापस 0;

	वापस 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
अटल अंतरभूत व्योम update_sg_wakeup_stats(काष्ठा sched_domain *sd,
					  काष्ठा sched_group *group,
					  काष्ठा sg_lb_stats *sgs,
					  काष्ठा task_struct *p)
{
	पूर्णांक i, nr_running;

	स_रखो(sgs, 0, माप(*sgs));

	/* Assume that task can't fit any CPU of the group */
	अगर (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->group_misfit_task_load = 1;

	for_each_cpu_and(i, sched_group_span(group), p->cpus_ptr) {
		काष्ठा rq *rq = cpu_rq(i);
		अचिन्हित पूर्णांक local;

		sgs->group_load += cpu_load_without(rq, p);
		sgs->group_util += cpu_util_without(i, p);
		sgs->group_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->योग_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->योग_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		अगर (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		अगर (sd->flags & SD_ASYM_CPUCAPACITY &&
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
	अगर (sgs->group_type == group_fully_busy ||
		sgs->group_type == group_overloaded)
		sgs->avg_load = (sgs->group_load * SCHED_CAPACITY_SCALE) /
				sgs->group_capacity;
}

अटल bool update_pick_idlest(काष्ठा sched_group *idlest,
			       काष्ठा sg_lb_stats *idlest_sgs,
			       काष्ठा sched_group *group,
			       काष्ठा sg_lb_stats *sgs)
{
	अगर (sgs->group_type < idlest_sgs->group_type)
		वापस true;

	अगर (sgs->group_type > idlest_sgs->group_type)
		वापस false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	चयन (sgs->group_type) {
	हाल group_overloaded:
	हाल group_fully_busy:
		/* Select the group with lowest avg_load. */
		अगर (idlest_sgs->avg_load <= sgs->avg_load)
			वापस false;
		अवरोध;

	हाल group_llc_balance:
	हाल group_imbalanced:
	हाल group_asym_packing:
	हाल group_smt_balance:
		/* Those types are not used in the slow wakeup path */
		वापस false;

	हाल group_misfit_task:
		/* Select group with the highest max capacity */
		अगर (idlest->sgc->max_capacity >= group->sgc->max_capacity)
			वापस false;
		अवरोध;

	हाल group_has_spare:
		/* Select group with most idle CPUs */
		अगर (idlest_sgs->idle_cpus > sgs->idle_cpus)
			वापस false;

		/* Select group with lowest group_util */
		अगर (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->group_util <= sgs->group_util)
			वापस false;

		अवरोध;
	}

	वापस true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
अटल काष्ठा sched_group *
sched_balance_find_dst_group(काष्ठा sched_domain *sd, काष्ठा task_struct *p, पूर्णांक this_cpu)
{
	काष्ठा sched_group *idlest = शून्य, *local = शून्य, *group = sd->groups;
	काष्ठा sg_lb_stats local_sgs, tmp_sgs;
	काष्ठा sg_lb_stats *sgs;
	अचिन्हित दीर्घ imbalance;
	काष्ठा sg_lb_stats idlest_sgs = {
			.avg_load = अच_पूर्णांक_उच्च,
			.group_type = group_overloaded,
	};

	करो {
		पूर्णांक local_group;

		/* Skip over this group if it has no CPUs allowed */
		अगर (!cpumask_intersects(sched_group_span(group),
					p->cpus_ptr))
			जारी;

		/* Skip over this group if no cookie matched */
		अगर (!sched_group_cookie_match(cpu_rq(this_cpu), p, group))
			जारी;

		local_group = cpumask_test_cpu(this_cpu,
					       sched_group_span(group));

		अगर (local_group) {
			sgs = &local_sgs;
			local = group;
		} अन्यथा {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, group, sgs, p);

		अगर (!local_group && update_pick_idlest(idlest, &idlest_sgs, group, sgs)) {
			idlest = group;
			idlest_sgs = *sgs;
		}

	} जबतक (group = group->अग्र, group != sd->groups);


	/* There is no idlest group to push tasks to */
	अगर (!idlest)
		वापस शून्य;

	/* The local group has been skipped because of CPU affinity */
	अगर (!local)
		वापस idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	अगर (local_sgs.group_type < idlest_sgs.group_type)
		वापस शून्य;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	अगर (local_sgs.group_type > idlest_sgs.group_type)
		वापस idlest;

	चयन (local_sgs.group_type) {
	हाल group_overloaded:
	हाल group_fully_busy:

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

		अगर ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_load + imbalance) >= local_sgs.avg_load))
			वापस शून्य;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		अगर (idlest_sgs.avg_load >= (local_sgs.avg_load + imbalance))
			वापस शून्य;

		अगर (100 * local_sgs.avg_load <= sd->imbalance_pct * idlest_sgs.avg_load)
			वापस शून्य;
		अवरोध;

	हाल group_llc_balance:
	हाल group_imbalanced:
	हाल group_asym_packing:
	हाल group_smt_balance:
		/* Those type are not used in the slow wakeup path */
		वापस शून्य;

	हाल group_misfit_task:
		/* Select group with the highest max capacity */
		अगर (local->sgc->max_capacity >= idlest->sgc->max_capacity)
			वापस शून्य;
		अवरोध;

	हाल group_has_spare:
#अगर_घोषित CONFIG_NUMA
		अगर (sd->flags & SD_NUMA) {
			पूर्णांक imb_numa_nr = sd->imb_numa_nr;
#अगर_घोषित CONFIG_NUMA_BALANCING
			पूर्णांक idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			अगर (cpu_to_गाँठ(this_cpu) == p->numa_preferred_nid)
				वापस शून्य;

			idlest_cpu = cpumask_first(sched_group_span(idlest));
			अगर (cpu_to_गाँठ(idlest_cpu) == p->numa_preferred_nid)
				वापस idlest;
#पूर्ण_अगर /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			अगर (p->nr_cpus_allowed != NR_CPUS) {
				अचिन्हित पूर्णांक w = cpumask_weight_and(p->cpus_ptr,
								sched_group_span(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = असल(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			अगर (!adjust_numa_imbalance(imbalance,
						   local_sgs.योग_nr_running + 1,
						   imb_numa_nr)) {
				वापस शून्य;
			}
		}
#पूर्ण_अगर /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		अगर (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			वापस शून्य;
		अवरोध;
	}

	वापस idlest;
}

अटल व्योम update_idle_cpu_scan(काष्ठा lb_env *env,
				 अचिन्हित दीर्घ योग_util)
{
	काष्ठा sched_domain_shared *sd_share;
	काष्ठा sched_domain *sd = env->sd;
	पूर्णांक llc_weight, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	अगर (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		वापस;

	sd_share = sd->shared;
	अगर (!sd_share)
		वापस;

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
	llc_weight = sd->span_weight;
	do_div(x, llc_weight);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_CAPACITY_SCALE);
	tmp = min_t(दीर्घ, tmp, SCHED_CAPACITY_SCALE);
	y = SCHED_CAPACITY_SCALE - tmp;

	/* equation [2] */
	y *= llc_weight;
	do_div(y, SCHED_CAPACITY_SCALE);
	अगर ((पूर्णांक)y != sd_share->nr_idle_scan)
		WRITE_ONCE(sd_share->nr_idle_scan, (पूर्णांक)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

अटल अंतरभूत व्योम update_sd_lb_stats(काष्ठा lb_env *env, काष्ठा sd_lb_stats *sds)
{
	काष्ठा sched_group *sg = env->sd->groups;
	काष्ठा sg_lb_stats *local = &sds->local_stat;
	काष्ठा sg_lb_stats tmp_sgs;
	अचिन्हित दीर्घ योग_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_active() || is_core_idle(env->dst_cpu);

	करो {
		काष्ठा sg_lb_stats *sgs = &tmp_sgs;
		पूर्णांक local_group;

		local_group = cpumask_test_cpu(env->dst_cpu, sched_group_span(sg));
		अगर (local_group) {
			sds->local = sg;
			sgs = local;

			अगर (env->idle != CPU_NEWLY_IDLE ||
			    time_after_eq(jiffies, sg->sgc->अग्र_update))
				update_group_capacity(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		अगर (!local_group && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->group_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->योग_load += sgs->group_load;
		sds->योग_capacity += sgs->group_capacity;

		योग_util += sgs->group_util;
		sg = sg->अग्र;
	} जबतक (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	अगर (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	अगर (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_group(&sds->busiest_stat);

	अगर (!env->sd->parent) {
		/* update overload indicator if we are at root domain */
		set_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		set_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} अन्यथा अगर (sg_overutilized) {
		set_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_scan(env, योग_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
अटल अंतरभूत व्योम calculate_imbalance(काष्ठा lb_env *env, काष्ठा sd_lb_stats *sds)
{
	काष्ठा sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	अगर (busiest->group_type == group_misfit_task) {
		अगर (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} अन्यथा {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_load;
			env->imbalance = busiest->group_misfit_task_load;
		}
		वापस;
	}

	अगर (busiest->group_type == group_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->योग_h_nr_running;
		वापस;
	}

	अगर (busiest->group_type == group_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		वापस;
	}

#अगर_घोषित CONFIG_SCHED_CACHE
	अगर (busiest->group_type == group_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		वापस;
	}
#पूर्ण_अगर

	अगर (busiest->group_type == group_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		वापस;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	अगर (local->group_type == group_has_spare) {
		अगर ((busiest->group_type > group_fully_busy) &&
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
			अगर (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			वापस;
		}

		अगर (busiest->group_weight == 1 || sds->prefer_sibling) {
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

#अगर_घोषित CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		अगर (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->योग_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#पूर्ण_अगर

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		वापस;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	अगर (local->group_type < group_overloaded) {
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
		अगर (local->avg_load >= busiest->avg_load) {
			env->imbalance = 0;
			वापस;
		}

		sds->avg_load = (sds->योग_load * SCHED_CAPACITY_SCALE) /
				sds->योग_capacity;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		अगर (local->avg_load >= sds->avg_load) {
			env->imbalance = 0;
			वापस;
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
अटल काष्ठा sched_group *sched_balance_find_src_group(काष्ठा lb_env *env)
{
	काष्ठा sg_lb_stats *local, *busiest;
	काष्ठा sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	अगर (!sds.busiest)
		जाओ out_balanced;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	अगर (busiest->group_type == group_misfit_task)
		जाओ force_balance;

	अगर (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		जाओ out_balanced;

	/* ASYM feature bypasses nice load balance check */
	अगर (busiest->group_type == group_asym_packing)
		जाओ force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	अगर (busiest->group_type == group_imbalanced)
		जाओ force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	अगर (local->group_type > busiest->group_type)
		जाओ out_balanced;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	अगर (local->group_type == group_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		अगर (local->avg_load >= busiest->avg_load)
			जाओ out_balanced;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_load = (sds.योग_load * SCHED_CAPACITY_SCALE) /
				sds.योग_capacity;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		अगर (local->avg_load >= sds.avg_load)
			जाओ out_balanced;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		अगर (100 * busiest->avg_load <=
				env->sd->imbalance_pct * local->avg_load)
			जाओ out_balanced;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	अगर (sds.prefer_sibling && local->group_type == group_has_spare &&
	    (busiest->group_type == group_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		जाओ force_balance;

	अगर (busiest->group_type != group_overloaded) {
		अगर (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			जाओ out_balanced;
		}

		अगर (busiest->group_type == group_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			जाओ force_balance;
		}

		अगर (busiest->group_weight > 1 &&
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
			जाओ out_balanced;
		}

		अगर (busiest->योग_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			जाओ out_balanced;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	वापस env->imbalance ? sds.busiest : शून्य;

out_balanced:
	env->imbalance = 0;
	वापस शून्य;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
अटल काष्ठा rq *sched_balance_find_src_rq(काष्ठा lb_env *env,
				     काष्ठा sched_group *group)
{
	काष्ठा rq *busiest = शून्य, *rq;
	अचिन्हित दीर्घ busiest_util = 0, busiest_load = 0, busiest_capacity = 1;
	अचिन्हित पूर्णांक __maybe_unused busiest_pref_llc = 0;
	काष्ठा sched_domain __maybe_unused *sd_tmp;
	अचिन्हित पूर्णांक busiest_nr = 0;
	पूर्णांक __maybe_unused dst_llc;
	पूर्णांक i;

	for_each_cpu_and(i, sched_group_span(group), env->cpus) {
		अचिन्हित दीर्घ capacity, load, util;
		अचिन्हित पूर्णांक nr_running;
		क्रमागत fbq_type rt;

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
		अगर (rt > env->fbq_type)
			जारी;

		nr_running = rq->cfs.h_nr_runnable;
		अगर (!nr_running)
			जारी;

		capacity = capacity_of(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		अगर (env->sd->flags & SD_ASYM_CPUCAPACITY &&
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
			अगर (!smt_degraded_cap && !cluster_equal_cap &&
			    !capacity_greater(capacity_of(env->dst_cpu), capacity))
				जारी;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		अगर (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			जारी;

		चयन (env->migration_type) {
		हाल migrate_load:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			load = cpu_load(rq);

			अगर (nr_running == 1 && load > env->imbalance &&
			    !check_cpu_capacity(rq, env->sd))
				अवरोध;

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
			अगर (load * busiest_capacity > busiest_load * capacity) {
				busiest_load = load;
				busiest_capacity = capacity;
				busiest = rq;
			}
			अवरोध;

		हाल migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			अगर (nr_running <= 1)
				जारी;

			अगर (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			अवरोध;

		हाल migrate_task:
			अगर (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			अवरोध;

		हाल migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			अगर (rq->misfit_task_load > busiest_load) {
				busiest_load = rq->misfit_task_load;
				busiest = rq;
			}

			अवरोध;

		हाल migrate_llc_task:
#अगर_घोषित CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			अगर (sd_tmp && (अचिन्हित)dst_llc < sd_tmp->llc_max) {
				अचिन्हित पूर्णांक this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				अगर (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#पूर्ण_अगर
			अवरोध;

		}
	}

	वापस busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#घोषणा MAX_PINNED_INTERVAL	512

अटल अंतरभूत bool
asym_active_balance(काष्ठा lb_env *env)
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
	वापस env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

अटल अंतरभूत bool
imbalanced_active_balance(काष्ठा lb_env *env)
{
	काष्ठा sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	अगर ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		वापस 1;

	वापस 0;
}

अटल पूर्णांक need_active_balance(काष्ठा lb_env *env)
{
	काष्ठा sched_domain *sd = env->sd;

	अगर (alb_break_llc(env))
		वापस 0;

	अगर (asym_active_balance(env))
		वापस 1;

	अगर (imbalanced_active_balance(env))
		वापस 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	अगर (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		अगर ((check_cpu_capacity(env->src_rq, sd)) &&
		    (capacity_of(env->src_cpu)*sd->imbalance_pct < capacity_of(env->dst_cpu)*100))
			वापस 1;
	}

	अगर (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		वापस 1;

	वापस 0;
}

अटल पूर्णांक active_load_balance_cpu_stop(व्योम *तथ्य);
अटल पूर्णांक active_load_balance_llc_cpu_stop(व्योम *तथ्य);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
अटल अंतरभूत cpu_stop_fn_t alb_stop_fn(काष्ठा lb_env *env)
{
	अगर (env->migration_type == migrate_llc_task)
		वापस active_load_balance_llc_cpu_stop;

	वापस active_load_balance_cpu_stop;
}

अटल पूर्णांक should_we_balance(काष्ठा lb_env *env)
{
	काष्ठा cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	काष्ठा sched_group *sg = env->sd->groups;
	पूर्णांक cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	अगर (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		वापस 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	अगर (env->idle == CPU_NEWLY_IDLE) {
		अगर (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			वापस 0;
		वापस 1;
	}

	cpumask_copy(swb_cpus, group_balance_mask(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		अगर (!idle_cpu(cpu))
			जारी;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		अगर (sched_smt_active() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			अगर (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_mask(cpu));
			जारी;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		वापस cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	अगर (idle_smt != -1)
		वापस idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	वापस group_balance_cpu(sg) == env->dst_cpu;
}

अटल व्योम update_lb_imbalance_stat(काष्ठा lb_env *env, काष्ठा sched_domain *sd,
				     क्रमागत cpu_idle_type idle)
{
	अगर (!schedstat_enabled())
		वापस;

	चयन (env->migration_type) {
	हाल migrate_load:
		__schedstat_add(sd->lb_imbalance_load[idle], env->imbalance);
		अवरोध;
	हाल migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		अवरोध;
	हाल migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		अवरोध;
	हाल migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		अवरोध;
	हाल migrate_llc_task:
		अवरोध;
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
अटल atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
अटल पूर्णांक sched_balance_rq(पूर्णांक this_cpu, काष्ठा rq *this_rq,
			काष्ठा sched_domain *sd, क्रमागत cpu_idle_type idle,
			पूर्णांक *continue_balancing)
{
	पूर्णांक ld_moved, cur_ld_moved, active_balance = 0;
	काष्ठा sched_domain *sd_parent = sd->parent;
	काष्ठा sched_group *group;
	काष्ठा rq *busiest;
	काष्ठा rq_flags rf;
	काष्ठा cpumask *cpus = this_cpu_cpumask_var_ptr(load_balance_mask);
	काष्ठा lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = group_balance_mask(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_शीर्ष_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_span(sd), cpu_preferred_mask);

	schedstat_inc(sd->lb_गिनती[idle]);

redo:
	अगर (!should_we_balance(&env)) {
		*continue_balancing = 0;
		जाओ out_balanced;
	}

	अगर (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		पूर्णांक zero = 0;
		अगर (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			जाओ out_balanced;

		need_unlock = true;
	}

	group = sched_balance_find_src_group(&env);
	अगर (!group) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		जाओ out_balanced;
	}

	busiest = sched_balance_find_src_rq(&env, group);
	अगर (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		जाओ out_balanced;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	अगर (busiest->nr_running > 1) {
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

		अगर (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		अगर (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			जाओ more_balance;
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
		अगर ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

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
			जाओ more_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		अगर (sd_parent) {
			पूर्णांक *group_imbalance = &sd_parent->groups->sgc->imbalance;

			अगर ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*group_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		अगर (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_of(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			अगर (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				जाओ redo;
			}
			जाओ out_all_pinned;
		}
	}

	अगर (ld_moved) {
		sd->nr_balance_failed = 0;
		जाओ out_unbalanced;
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
	अगर (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	अगर (!need_active_balance(&env))
		जाओ out_unbalanced;

	scoped_guard (raw_spin_rq_lock_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		अगर (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			जाओ out_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		अगर (busiest->active_balance)
			जाओ out_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		अगर (!busiest->curr->on_rq)
			जाओ out_unbalanced;

		busiest->active_balance = 1;
		busiest->धकेलो_cpu = this_cpu;
		active_balance = 1;
		preempt_disable();
	}
	अगर (active_balance) {
		stop_one_cpu_nowait(cpu_of(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->active_balance_work);
	}
	preempt_enable();

out_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	जाओ out;

out_balanced:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	अगर (sd_parent && !(env.flags & LBF_ALL_PINNED)) {
		पूर्णांक *group_imbalance = &sd_parent->groups->sgc->imbalance;

		अगर (*group_imbalance)
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
	अगर (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		जाओ out;

	/* tune up the balancing interval */
	अगर ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
out:
	अगर (need_unlock)
		atomic_set_release(&sched_balance_running, 0);

	वापस ld_moved;
}

अटल अंतरभूत अचिन्हित दीर्घ
get_sd_balance_interval(काष्ठा sched_domain *sd, पूर्णांक cpu_busy)
{
	अचिन्हित दीर्घ interval = sd->balance_interval;

	अगर (cpu_busy)
		interval *= sd->busy_factor;

	/* scale ms to jiffies */
	interval = msecs_to_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	अगर (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_load_balance_interval);

	वापस interval;
}

अटल अंतरभूत व्योम
update_अग्र_balance(काष्ठा sched_domain *sd, अचिन्हित दीर्घ *अग्र_balance)
{
	अचिन्हित दीर्घ interval, अग्र;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	अग्र = sd->last_balance + interval;

	अगर (time_after(*अग्र_balance, अग्र))
		*अग्र_balance = अग्र;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
अटल पूर्णांक __active_load_balance_cpu_stop(व्योम *तथ्य, अचिन्हित पूर्णांक lb_flags)
{
	काष्ठा rq *busiest_rq = तथ्य;
	पूर्णांक busiest_cpu = cpu_of(busiest_rq);
	पूर्णांक target_cpu = busiest_rq->धकेलो_cpu;
	काष्ठा rq *target_rq = cpu_rq(target_cpu);
	काष्ठा sched_domain *sd;
	काष्ठा task_struct *p = शून्य;
	काष्ठा rq_flags rf;

	rq_lock_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	अगर (!cpu_active(busiest_cpu) || !cpu_active(target_cpu))
		जाओ out_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	अगर (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->active_balance))
		जाओ out_unlock;

	/* Is there any task to move? */
	अगर (busiest_rq->nr_running <= 1)
		जाओ out_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == target_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_lock();
	for_each_domain(target_cpu, sd) {
		अगर (cpumask_test_cpu(busiest_cpu, sched_domain_span(sd)))
			अवरोध;
	}

	अगर (likely(sd)) {
		काष्ठा lb_env env = {
			.sd		= sd,
			.dst_cpu	= target_cpu,
			.dst_rq		= target_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_ACTIVE_LB | lb_flags,
		};

		schedstat_inc(sd->alb_गिनती);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		अगर (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} अन्यथा {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
out_unlock:
	busiest_rq->active_balance = 0;
	rq_unlock(busiest_rq, &rf);

	अगर (p)
		attach_one_task(target_rq, p);

	local_irq_enable();

	वापस 0;
}

अटल पूर्णांक active_load_balance_cpu_stop(व्योम *तथ्य)
{
	वापस __active_load_balance_cpu_stop(तथ्य, 0);
}

अटल पूर्णांक active_load_balance_llc_cpu_stop(व्योम *तथ्य)
{
	वापस __active_load_balance_cpu_stop(तथ्य, LBF_ACTIVE_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
व्योम update_max_interval(व्योम)
{
	max_load_balance_interval = HZ*अंक_online_cpus()/10;
}

अटल अंतरभूत व्योम update_newidle_stats(काष्ठा sched_domain *sd, अचिन्हित पूर्णांक success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	अगर (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		पूर्णांक ratio = 0;

		अगर (delta < 0)
			delta = 0;

		अगर (sched_feat(NI_RATE)) {
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

अटल अंतरभूत bool
update_newidle_cost(काष्ठा sched_domain *sd, u64 cost, अचिन्हित पूर्णांक success)
{
	अचिन्हित दीर्घ अग्र_decay = sd->last_decay_max_lb_cost + HZ;
	अचिन्हित दीर्घ now = jiffies;

	अगर (cost)
		update_newidle_stats(sd, success);

	अगर (cost > sd->max_newidle_lb_cost) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_cost = cost;
		sd->last_decay_max_lb_cost = now;

	} अन्यथा अगर (time_after(now, अग्र_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_cost = (sd->max_newidle_lb_cost * 253) / 256;
		sd->last_decay_max_lb_cost = now;
		वापस true;
	}

	वापस false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
अटल व्योम sched_balance_domains(काष्ठा rq *rq, क्रमागत cpu_idle_type idle)
{
	पूर्णांक continue_balancing = 1;
	पूर्णांक cpu = rq->cpu;
	पूर्णांक busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	अचिन्हित दीर्घ interval;
	काष्ठा sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	अचिन्हित दीर्घ अग्र_balance = jiffies + 60*HZ;
	पूर्णांक update_अग्र_balance = 0;
	पूर्णांक need_decay = 0;
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
		अगर (!continue_balancing) {
			अगर (need_decay)
				जारी;
			अवरोध;
		}

		interval = get_sd_balance_interval(sd, busy);
		अगर (time_after_eq(jiffies, sd->last_balance + interval)) {
			अगर (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
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
		अगर (time_after(अग्र_balance, sd->last_balance + interval)) {
			अग्र_balance = sd->last_balance + interval;
			update_अग्र_balance = 1;
		}
	}
	अगर (need_decay) {
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
	अगर (likely(update_अग्र_balance))
		rq->अग्र_balance = अग्र_balance;

}

अटल अंतरभूत पूर्णांक on_null_domain(काष्ठा rq *rq)
{
	वापस unlikely(!rcu_dereference_sched(rq->sd));
}

#अगर_घोषित CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
अटल अंतरभूत पूर्णांक find_new_ilb(व्योम)
{
	काष्ठा cpumask *ilb_cpus;
	पूर्णांक ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	cpumask_and(ilb_cpus, nohz.idle_cpus_mask,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		अगर (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			अगर (sched_smt_active() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_mask(ilb_cpu));
			जारी;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		अगर (sched_smt_active() && !is_core_idle(ilb_cpu)) {
			अगर (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_mask(ilb_cpu));
			जारी;
		}

		वापस ilb_cpu;
	}

	वापस fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
अटल व्योम kick_ilb(अचिन्हित पूर्णांक flags)
{
	पूर्णांक ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	अगर (flags & NOHZ_BALANCE_KICK)
		nohz.अग्र_balance = jiffies+1;

	ilb_cpu = find_new_ilb();
	अगर (ilb_cpu < 0)
		वापस;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	अगर ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		वापस;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	अगर (flags & NOHZ_KICK_MASK)
		वापस;

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
अटल व्योम nohz_balancer_kick(काष्ठा rq *rq)
{
	अचिन्हित दीर्घ now = jiffies;
	काष्ठा sched_domain_shared *sds;
	काष्ठा sched_domain *sd;
	पूर्णांक nr_busy, i, cpu = rq->cpu;
	अचिन्हित पूर्णांक flags = 0;

	अगर (unlikely(rq->idle_balance))
		वापस;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	अगर (READ_ONCE(nohz.has_blocked_load) &&
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
	अगर (time_before(now, nohz.अग्र_balance))
		जाओ out;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	अगर (unlikely(cpumask_empty(nohz.idle_cpus_mask)))
		वापस;

	अगर (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		जाओ out;
	}

	sd = rcu_dereference_all(rq->sd);
	अगर (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		अगर (rq->cfs.h_nr_runnable >= 1 && check_cpu_capacity(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			जाओ out;
		}
	}

	sd = rcu_dereference_all(per_cpu(sd_asym_packing, cpu));
	अगर (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_span(sd), nohz.idle_cpus_mask) {
			अगर (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				जाओ out;
			}
		}
	}

	sd = rcu_dereference_all(per_cpu(sd_asym_cpucapacity, cpu));
	अगर (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		अगर (check_misfit_status(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		जाओ out;
	}

	sds = rcu_dereference_all(per_cpu(sd_balance_shared, cpu));
	अगर (sds) {
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
		अगर (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
out:
	अगर (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_अग्र_KICK;

	अगर (flags)
		kick_ilb(flags);
}

अटल व्योम set_cpu_sd_state_busy(पूर्णांक cpu)
{
	काष्ठा sched_domain *sd;
	sd = rcu_dereference_all(per_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	अगर (!sd || !sd->shared || !sd->nohz_idle)
		वापस;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

व्योम nohz_balance_exit_idle(काष्ठा rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	अगर (likely(!rq->nohz_tick_stopped))
		वापस;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_mask);

	set_cpu_sd_state_busy(rq->cpu);
}

अटल व्योम set_cpu_sd_state_idle(पूर्णांक cpu)
{
	काष्ठा sched_domain *sd;
	sd = rcu_dereference_all(per_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	अगर (!sd || !sd->shared || sd->nohz_idle)
		वापस;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
व्योम nohz_balance_enter_idle(पूर्णांक cpu)
{
	काष्ठा rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	अगर (!cpu_active(cpu))
		वापस;

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
	अगर (rq->nohz_tick_stopped)
		जाओ out;

	/* If we're a completely isolated CPU, we don't play: */
	अगर (on_null_domain(rq))
		वापस;

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

अटल bool update_nohz_stats(काष्ठा rq *rq)
{
	अचिन्हित पूर्णांक cpu = rq->cpu;

	अगर (!rq->has_blocked_load)
		वापस false;

	अगर (!cpumask_test_cpu(cpu, nohz.idle_cpus_mask))
		वापस false;

	अगर (!time_after(jiffies, READ_ONCE(rq->last_blocked_load_update_tick)))
		वापस true;

	sched_balance_update_blocked_averages(cpu);

	वापस rq->has_blocked_load;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
अटल व्योम _nohz_idle_balance(काष्ठा rq *this_rq, अचिन्हित पूर्णांक flags)
{
	/* Earliest time when we have to do rebalance again */
	अचिन्हित दीर्घ now = jiffies;
	अचिन्हित दीर्घ अग्र_balance = now + 60*HZ;
	bool has_blocked_load = false;
	पूर्णांक update_अग्र_balance = 0;
	पूर्णांक this_cpu = this_rq->cpu;
	पूर्णांक balance_cpu;
	काष्ठा rq *rq;

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
	अगर (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_load, 0);
	अगर (flags & NOHZ_अग्र_KICK)
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
		अगर (!idle_cpu(balance_cpu))
			जारी;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		अगर (!idle_cpu(this_cpu) && need_resched()) {
			अगर (flags & NOHZ_STATS_KICK)
				has_blocked_load = true;
			अगर (flags & NOHZ_अग्र_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			जाओ पात;
		}

		rq = cpu_rq(balance_cpu);

		अगर (flags & NOHZ_STATS_KICK)
			has_blocked_load |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		अगर (time_after_eq(jiffies, rq->अग्र_balance)) {
			काष्ठा rq_flags rf;

			rq_lock_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			अगर (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		अगर (time_after(अग्र_balance, rq->अग्र_balance)) {
			अग्र_balance = rq->अग्र_balance;
			update_अग्र_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	अगर (likely(update_अग्र_balance))
		nohz.अग्र_balance = अग्र_balance;

	अगर (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.अग्र_blocked,
			   now + msecs_to_jiffies(LOAD_AVG_PERIOD));

पात:
	/* There is still blocked load, enable periodic update */
	अगर (has_blocked_load)
		WRITE_ONCE(nohz.has_blocked_load, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
अटल bool nohz_idle_balance(काष्ठा rq *this_rq, क्रमागत cpu_idle_type idle)
{
	अचिन्हित पूर्णांक flags = this_rq->nohz_idle_balance;

	अगर (!flags)
		वापस false;

	this_rq->nohz_idle_balance = 0;

	अगर (idle != CPU_IDLE)
		वापस false;

	_nohz_idle_balance(this_rq, flags);

	वापस true;
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
व्योम nohz_run_idle_balance(पूर्णांक cpu)
{
	अचिन्हित पूर्णांक flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	अगर ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

अटल व्योम nohz_newidle_balance(काष्ठा rq *this_rq)
{
	पूर्णांक this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	अगर (this_rq->avg_idle < sysctl_sched_migration_cost)
		वापस;

	/* Don't need to update blocked load of idle CPUs*/
	अगर (!READ_ONCE(nohz.has_blocked_load) ||
	    time_before(jiffies, READ_ONCE(nohz.अग्र_blocked)))
		वापस;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
अटल अंतरभूत व्योम nohz_balancer_kick(काष्ठा rq *rq) { }

अटल अंतरभूत bool nohz_idle_balance(काष्ठा rq *this_rq, क्रमागत cpu_idle_type idle)
{
	वापस false;
}

अटल अंतरभूत व्योम nohz_newidle_balance(काष्ठा rq *this_rq) { }
#पूर्ण_अगर /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
अटल पूर्णांक sched_balance_newidle(काष्ठा rq *this_rq, काष्ठा rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	अचिन्हित दीर्घ अग्र_balance = jiffies + HZ;
	पूर्णांक this_cpu = this_rq->cpu;
	पूर्णांक continue_balancing = 1;
	u64 t0, t1, curr_cost = 0;
	काष्ठा sched_domain *sd;
	पूर्णांक pulled_task = 0;

	update_misfit_status(शून्य, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	अगर (this_rq->ttwu_pending)
		वापस 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	अगर (!cpu_preferred(this_cpu))
		वापस 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_lock(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	अगर (!sd)
		जाओ out;

	अगर (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_cost) {

		update_अग्र_balance(sd, &अग्र_balance);
		जाओ out;
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

		अगर (this_rq->avg_idle < curr_cost + sd->max_newidle_lb_cost)
			अवरोध;

		अगर (sd->flags & SD_BALANCE_NEWIDLE) {
			अचिन्हित पूर्णांक weight = 1;

			अगर (sched_feat(NI_RANDOM) && sd->newidle_ratio < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				weight = 1 + sd->newidle_ratio;
				अगर (d1k > weight) {
					update_newidle_stats(sd, 0);
					जारी;
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
		अगर (pulled_task || !continue_balancing)
			अवरोध;
	}

	raw_spin_rq_lock(this_rq);

	अगर (curr_cost > this_rq->max_idle_balance_cost)
		this_rq->max_idle_balance_cost = curr_cost;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	अगर (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	अगर (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

out:
	/* Move the next balance forward */
	अगर (time_after(this_rq->अग्र_balance, अग्र_balance))
		this_rq->अग्र_balance = अग्र_balance;

	अगर (pulled_task)
		this_rq->idle_stamp = 0;
	अन्यथा
		nohz_newidle_balance(this_rq);

	rq_repin_lock(this_rq, rf);

	वापस pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
अटल __latent_entropy व्योम sched_balance_softirq(व्योम)
{
	काष्ठा rq *this_rq = this_rq();
	क्रमागत cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	अगर (nohz_idle_balance(this_rq, idle))
		वापस;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
व्योम sched_balance_trigger(काष्ठा rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	अगर (unlikely(on_null_domain(rq) || !cpu_active(cpu_of(rq))))
		वापस;

	अगर (time_after_eq(jiffies, rq->अग्र_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

अटल व्योम rq_online_fair(काष्ठा rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

अटल व्योम rq_offline_fair(काष्ठा rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#अगर_घोषित CONFIG_SCHED_CORE
अटल अंतरभूत bool
__entity_slice_used(काष्ठा sched_entity *se, पूर्णांक min_nr_tasks)
{
	u64 rtime = se->योग_exec_runtime - se->पूर्व_योग_exec_runtime;
	u64 slice = se->slice;

	वापस (rtime * min_nr_tasks > slice);
}

#घोषणा MIN_NR_TASKS_DURING_FORCEIDLE	2
अटल अंतरभूत व्योम task_tick_core(काष्ठा rq *rq, काष्ठा task_struct *curr)
{
	अगर (!sched_core_enabled(rq))
		वापस;

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
	अगर (rq->core->core_forceidle_गिनती && rq->cfs.h_nr_queued == 1 &&
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
अटल व्योम se_fi_update(स्थिर काष्ठा sched_entity *se, अचिन्हित पूर्णांक fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

		अगर (forceidle) {
			अगर (cfs_rq->forceidle_seq == fi_seq)
				अवरोध;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

व्योम task_vruntime_update(काष्ठा rq *rq, काष्ठा task_struct *p, bool in_fi)
{
	काष्ठा sched_entity *se = &p->se;

	अगर (p->sched_class != &fair_sched_class)
		वापस;

	se_fi_update(se, rq->core->core_forceidle_seq, in_fi);
}

bool cfs_prio_less(स्थिर काष्ठा task_struct *a, स्थिर काष्ठा task_struct *b,
			bool in_fi)
{
	काष्ठा rq *rq = task_rq(a);
	स्थिर काष्ठा sched_entity *sea = &a->se;
	स्थिर काष्ठा sched_entity *seb = &b->se;
	काष्ठा cfs_rq *cfs_rqa;
	काष्ठा cfs_rq *cfs_rqb;
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

	वापस delta > 0;
}

अटल पूर्णांक task_is_throttled_fair(काष्ठा task_struct *p, पूर्णांक cpu)
{
	काष्ठा cfs_rq *cfs_rq;

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	cfs_rq = tg_cfs_rq(task_group(p), cpu);
#अन्यथा
	cfs_rq = &cpu_rq(cpu)->cfs;
#पूर्ण_अगर
	वापस throttled_hierarchy(cfs_rq);
}
#अन्यथा /* !CONFIG_SCHED_CORE: */
अटल अंतरभूत व्योम task_tick_core(काष्ठा rq *rq, काष्ठा task_struct *curr) {}
#पूर्ण_अगर /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
अटल व्योम task_tick_fair(काष्ठा rq *rq, काष्ठा task_struct *curr, पूर्णांक queued)
{
	काष्ठा sched_entity *se = &curr->se;

	अगर (se->on_rq) {
		अचिन्हित दीर्घ weight = NICE_0_LOAD;
		काष्ठा cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_of(se);
			entity_tick(cfs_rq, se, queued);

			weight = __calc_prop_weight(cfs_rq, se, weight);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, weight, se->on_rq);
	}

	अगर (queued)
		वापस;

	अगर (static_branch_unlikely(&sched_numa_balancing))
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
अटल व्योम task_fork_fair(काष्ठा task_struct *p)
{
	set_task_max_allowed_capacity(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
अटल व्योम
prio_changed_fair(काष्ठा rq *rq, काष्ठा task_struct *p, u64 oldprio)
{
	अगर (!task_on_rq_queued(p))
		वापस;

	अगर (p->prio == oldprio)
		वापस;

	अगर (rq->cfs.h_nr_queued == 1)
		वापस;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	अगर (task_current_donor(rq, p)) {
		अगर (p->prio > oldprio)
			resched_curr(rq);
	} अन्यथा {
		wakeup_preempt(rq, p, 0);
	}
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
अटल व्योम propagate_entity_cfs_rq(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	अगर (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_add_leaf_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->parent;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);

		update_load_avg(cfs_rq, se, UPDATE_TG);

		अगर (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_add_leaf_cfs_rq(cfs_rq);
	}

	assert_list_leaf_cfs_rq(rq_of(cfs_rq));
}
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
अटल व्योम propagate_entity_cfs_rq(काष्ठा sched_entity *se) { }
#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

अटल व्योम detach_entity_cfs_rq(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	अगर (!se->avg.last_update_time)
		वापस;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_load_avg(cfs_rq, se, 0);
	detach_entity_load_avg(cfs_rq, se);
	update_tg_load_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

अटल व्योम attach_entity_cfs_rq(काष्ठा sched_entity *se)
{
	काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

	/* Synchronize entity with its cfs_rq */
	update_load_avg(cfs_rq, se, sched_feat(ATTACH_AGE_LOAD) ? 0 : SKIP_AGE_LOAD);
	attach_entity_load_avg(cfs_rq, se);
	update_tg_load_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

अटल व्योम detach_task_cfs_rq(काष्ठा task_struct *p)
{
	काष्ठा sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

अटल व्योम attach_task_cfs_rq(काष्ठा task_struct *p)
{
	काष्ठा sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

अटल व्योम switching_from_fair(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अगर (p->se.sched_delayed)
		dequeue_task(rq, p, DEQUEUE_SLEEP | DEQUEUE_DELAYED | DEQUEUE_NOCLOCK);
}

अटल व्योम switched_from_fair(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	detach_task_cfs_rq(p);
}

अटल व्योम switched_to_fair(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	set_task_max_allowed_capacity(p);

	अगर (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		अगर (task_current_donor(rq, p))
			resched_curr(rq);
		अन्यथा
			wakeup_preempt(rq, p, 0);
	}
}

अटल व्योम set_अग्र_task_fair(काष्ठा rq *rq, काष्ठा task_struct *p, क्रमागत snt_e type)
{
	काष्ठा sched_entity *se = &p->se;
	काष्ठा cfs_rq *cfs_rq = &rq->cfs;
	अचिन्हित दीर्घ weight = NICE_0_LOAD;
	bool first = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	अगर (type == SNT_REPICK)
		जाओ repick;

	clear_buddies(cfs_rq, se);

	अगर (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);

		अगर (!IS_ENABLED(CONFIG_FAIR_GROUP_SCHED) ||
		    !first || !cfs_rq->h_curr)
			set_अग्र_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		अगर (on_rq)
			weight = __calc_prop_weight(cfs_rq, se, weight);
	}

	अगर (throttled)
		task_throttle_setup_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	अगर (on_rq) {
		reweight_eevdf(cfs_rq, se, weight, se->on_rq);
		अगर (first)
			set_protect_slice(cfs_rq, se);
	}

	अगर (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->group_गाँठ, &rq->cfs_tasks);
	}
	अगर (!first)
		वापस;

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
	अगर (hrtick_enabled_fair(rq))
		hrtick_start_fair(rq, p);
}

व्योम init_cfs_rq(काष्ठा cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_मूल_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_lock_init(&cfs_rq->removed.lock);
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
अटल व्योम task_change_group_fair(काष्ठा task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	अगर (READ_ONCE(p->__state) == TASK_NEW)
		वापस;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.last_update_time = 0;
	set_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

व्योम free_fair_sched_group(काष्ठा task_group *tg)
{
	free_percpu(tg->cfs_rq);
}

पूर्णांक alloc_fair_sched_group(काष्ठा task_group *tg, काष्ठा task_group *parent)
{
	काष्ठा cfs_tg_state __percpu *state;
	काष्ठा sched_entity *se;
	काष्ठा cfs_rq *cfs_rq;
	पूर्णांक i;

	state = alloc_percpu_gfp(काष्ठा cfs_tg_state, GFP_KERNEL);
	अगर (!state)
		जाओ err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_LOAD;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(parent));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		अगर (!cfs_rq)
			जाओ err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_entry(tg, cfs_rq, se, i, tg_se(parent, i));
		init_entity_runnable_average(se);
	}

	वापस 1;

err:
	वापस 0;
}

व्योम online_fair_sched_group(काष्ठा task_group *tg)
{
	काष्ठा sched_entity *se;
	काष्ठा rq_flags rf;
	काष्ठा rq *rq;
	पूर्णांक i;

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

व्योम unregister_fair_sched_group(काष्ठा task_group *tg)
{
	पूर्णांक cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		काष्ठा cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		काष्ठा sched_entity *se = tg_se(tg, cpu);
		काष्ठा rq *rq = cpu_rq(cpu);

		अगर (se)
			remove_entity_load_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		अगर (cfs_rq->on_list) {
			guard(rq_lock_irqsave)(rq);
			list_del_leaf_cfs_rq(cfs_rq);
		}
	}
}

व्योम init_tg_cfs_entry(काष्ठा task_group *tg, काष्ठा cfs_rq *cfs_rq,
			काष्ठा sched_entity *se, पूर्णांक cpu,
			काष्ठा sched_entity *parent)
{
	काष्ठा rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	अगर (!se)
		वापस;

	अगर (!parent) {
		se->cfs_rq = &rq->cfs;
		se->depth = 0;
	} अन्यथा {
		se->cfs_rq = parent->my_q;
		se->depth = parent->depth + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_load_set(&se->load, NICE_0_LOAD);
	se->parent = parent;
}

अटल DEFINE_MUTEX(shares_mutex);

अटल पूर्णांक __sched_group_set_shares(काष्ठा task_group *tg, अचिन्हित दीर्घ shares)
{
	पूर्णांक i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	अगर (is_मूल_task_group(tg))
		वापस -EINVAL;

	shares = clamp(shares, scale_load(MIN_SHARES), scale_load(MAX_SHARES));

	अगर (tg->shares == shares)
		वापस 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		काष्ठा rq *rq = cpu_rq(i);
		काष्ठा sched_entity *se = tg_se(tg, i);
		काष्ठा rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_lock_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_load_avg(cfs_rq_of(se), se, UPDATE_TG);
			update_cfs_group(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	वापस 0;
}

पूर्णांक sched_group_set_shares(काष्ठा task_group *tg, अचिन्हित दीर्घ shares)
{
	पूर्णांक ret;

	mutex_lock(&shares_mutex);
	अगर (tg_is_idle(tg))
		ret = -EINVAL;
	अन्यथा
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	वापस ret;
}

पूर्णांक sched_group_set_idle(काष्ठा task_group *tg, दीर्घ idle)
{
	पूर्णांक i;

	अगर (tg == &मूल_task_group)
		वापस -EINVAL;

	अगर (idle < 0 || idle > 1)
		वापस -EINVAL;

	mutex_lock(&shares_mutex);

	अगर (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		वापस 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		काष्ठा rq *rq = cpu_rq(i);
		काष्ठा sched_entity *se = tg_se(tg, i);
		काष्ठा cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		दीर्घ idle_task_delta;
		काष्ठा rq_flags rf;

		rq_lock_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		अगर (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			जाओ अग्र_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		अगर (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			काष्ठा cfs_rq *cfs_rq = cfs_rq_of(se);

			अगर (!se->on_rq)
				अवरोध;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			अगर (cfs_rq_is_idle(cfs_rq))
				अवरोध;
		}

अग्र_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	अगर (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_load(WEIGHT_IDLEPRIO));
	अन्यथा
		__sched_group_set_shares(tg, NICE_0_LOAD);

	mutex_unlock(&shares_mutex);
	वापस 0;
}

#पूर्ण_अगर /* CONFIG_FAIR_GROUP_SCHED */


अटल अचिन्हित पूर्णांक get_rr_interval_fair(काष्ठा rq *rq, काष्ठा task_struct *task)
{
	काष्ठा sched_entity *se = &task->se;
	अचिन्हित पूर्णांक rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	अगर (rq->cfs.load.weight)
		rr_interval = NS_TO_JIFFIES(se->slice);

	वापस rr_interval;
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
	.put_पूर्व_task		= put_पूर्व_task_fair,
	.set_अग्र_task          = set_अग्र_task_fair,

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

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	.task_change_group	= task_change_group_fair,
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#पूर्ण_अगर

#अगर_घोषित CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#पूर्ण_अगर
};

व्योम print_cfs_stats(काष्ठा seq_file *m, पूर्णांक cpu)
{
	काष्ठा cfs_rq *cfs_rq, *pos;

	rcu_read_lock();
	for_each_leaf_cfs_rq_safe(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#अगर_घोषित CONFIG_NUMA_BALANCING
व्योम show_numa_stats(काष्ठा task_struct *p, काष्ठा seq_file *m)
{
	पूर्णांक गाँठ;
	अचिन्हित दीर्घ tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	काष्ठा numa_group *ng;

	rcu_read_lock();
	ng = rcu_dereference_all(p->numa_group);
	for_each_online_गाँठ(गाँठ) {
		अगर (p->numa_faults) {
			tsf = p->numa_faults[task_faults_idx(NUMA_MEM, गाँठ, 0)];
			tpf = p->numa_faults[task_faults_idx(NUMA_MEM, गाँठ, 1)];
		}
		अगर (ng) {
			gsf = ng->faults[task_faults_idx(NUMA_MEM, गाँठ, 0)];
			gpf = ng->faults[task_faults_idx(NUMA_MEM, गाँठ, 1)];
		}
		print_numa_stats(m, गाँठ, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#पूर्ण_अगर /* CONFIG_NUMA_BALANCING */

__init व्योम init_sched_fair_class(व्योम)
{
	पूर्णांक i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_गाँठ(&per_cpu(load_balance_mask, i), GFP_KERNEL, cpu_to_गाँठ(i));
		zalloc_cpumask_var_गाँठ(&per_cpu(select_rq_mask,    i), GFP_KERNEL, cpu_to_गाँठ(i));
		zalloc_cpumask_var_गाँठ(&per_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_to_गाँठ(i));

#अगर_घोषित CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_शीर्ष(&cpu_rq(i)->cfsb_csd_list);
#पूर्ण_अगर
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#अगर_घोषित CONFIG_NO_HZ_COMMON
	nohz.अग्र_balance = jiffies;
	nohz.अग्र_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_mask, GFP_NOWAIT);
#पूर्ण_अगर
}
