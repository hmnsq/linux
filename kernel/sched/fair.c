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
#ضمن <linux/energy_model.h>
#ضمن <linux/mmap_lock.h>
#ضمن <linux/hugetlb_inline.h>
#ضمن <linux/jiffies.h>
#ضمن <linux/math.h>
#ضمن <linux/mm_api.h>
#ضمن <linux/highmem.h>
#ضمن <linux/hrtimer.h>
#ضمن <linux/hrtimer_bases.h>
#ضمن <linux/spinlock_api.h>
#ضمن <linux/cpumask_api.h>
#ضمن <linux/lockdep_api.h>
#ضمن <linux/softirq.h>
#ضمن <linux/refcount_api.h>
#ضمن <linux/topology.h>
#ضمن <linux/sched/clock.h>
#ضمن <linux/sched/cond_resched.h>
#ضمن <linux/sched/cputime.h>
#ضمن <linux/sched/isolation.h>
#ضمن <linux/sched/nohz.h>
#ضمن <linux/sched/prio.h>
#ضمن <linux/static_call.h>

#ضمن <linux/cpuidle.h>
#ضمن <linux/interrupt.h>
#ضمن <linux/memory-tiers.h>
#ضمن <linux/mempolicy.h>
#ضمن <linux/mutex_api.h>
#ضمن <linux/profile.h>
#ضمن <linux/psi.h>
#ضمن <linux/ratelimit.h>
#ضمن <linux/task_work.h>
#ضمن <linux/rbtree_augmented.h>

#ضمن <asm/switch_to.h>

#ضمن <uapi/linux/sched/types.h>

#ضمن "sched.h"
#ضمن "stats.h"
#ضمن "autogroup.h"

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
بینښه صحيح sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
بینښه صحيح sysctl_sched_بنسټ_ټوټه			= 700000ULL;
ساكن بینښه صحيح normalized_sysctl_sched_بنسټ_ټوټه	= 700000ULL;

__read_mostly بینښه صحيح sysctl_sched_migration_لګښت	= 500000UL;

ساكن صحيح __init تیاری_sched_thermal_decay_ښويدنه(حرف *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	أرجع 1;
}
__setup("sched_thermal_decay_shift=", تیاری_sched_thermal_decay_ښويدنه);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
صحيح __weak arch_asym_cpu_لومړیتوب(صحيح cpu)
{
	أرجع -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#تعریف_کړه ځاییږي_ظرفیت(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#تعریف_کړه ظرفیت_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
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
ساكن بینښه صحيح sysctl_sched_cfs_bandwidth_ټوټه		= 5000UL;
#د_که_پای

#که_تعریف_شوی CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
ساكن بینښه صحيح sysctl_numa_balancing_promote_شرح_حد = 65536;
#د_که_پای

#که_تعریف_شوی CONFIG_SYSCTL
ساكن ثابت بنية ctl_جدول sched_fair_sysctls[] = {
#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.معلومات           = &sysctl_sched_cfs_bandwidth_ټوټه,
		.maxlen         = حجم(بینښه صحيح),
		.ډېرپېښ           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#د_که_پای
#که_تعریف_شوی CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.معلومات		= &sysctl_numa_balancing_promote_شرح_حد,
		.maxlen		= حجم(بینښه صحيح),
		.ډېرپېښ		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#د_که_پای /* CONFIG_NUMA_BALANCING */
};

ساكن صحيح __init sched_fair_sysctl_init(فراغ)
{
	ثبتوونکی_sysctl_init("kernel", sched_fair_sysctls);
	أرجع 0;
}
late_initcall(sched_fair_sysctl_init);
#د_که_پای /* CONFIG_SYSCTL */

ساكن مضمن فراغ update_بارول_جمع_کړه(بنية بارول_وزن *lw, بینښه اوږد inc)
{
	lw->وزن += inc;
	lw->inv_وزن = 0;
}

ساكن مضمن فراغ update_بارول_sub(بنية بارول_وزن *lw, بینښه اوږد dec)
{
	lw->وزن -= dec;
	lw->inv_وزن = 0;
}

ساكن مضمن فراغ update_بارول_سېټ(بنية بارول_وزن *lw, بینښه اوږد w)
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
ساكن بینښه صحيح get_update_sysctl_ضریب(فراغ)
{
	بینښه صحيح cpus = min_t(بینښه صحيح, عدد_online_cpus(), 8);
	بینښه صحيح ضریب;

	بدل (sysctl_sched_tunable_scaling) {
	حالة SCHED_TUNABLESCALING_NONE:
		ضریب = 1;
		اكسر;
	حالة SCHED_TUNABLESCALING_خطي:
		ضریب = cpus;
		اكسر;
	حالة SCHED_TUNABLESCALING_LOG:
	افتراضي:
		ضریب = 1 + ilog2(cpus);
		اكسر;
	}

	أرجع ضریب;
}

ساكن فراغ update_sysctl(فراغ)
{
	بینښه صحيح ضریب = get_update_sysctl_ضریب();

#تعریف_کړه سېټ_SYSCTL(name) \
	(sysctl_##name = (ضریب) * normalized_sysctl_##name)
	سېټ_SYSCTL(sched_بنسټ_ټوټه);
#تعریف_لغوه سېټ_SYSCTL
}

فراغ __init sched_init_granularity(فراغ)
{
	update_sysctl();
}

#که_نه_تعریف_شوی CONFIG_64_بټ
#تعریف_کړه WMULT_CONST	(~0U)
#تعریف_کړه WMULT_ښويدنه	32

ساكن فراغ __update_inv_weight(بنية بارول_وزن *lw)
{
	بینښه اوږد w;

	إذا (likely(lw->inv_وزن))
		أرجع;

	w = scale_بارول_ښکته(lw->وزن);

	إذا (بټونه_فی_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_وزن = 1;
	وإلا إذا (unlikely(!w))
		lw->inv_وزن = WMULT_CONST;
	وإلا
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
ساكن u64 __calc_delta(u64 delta_exec, بینښه اوږد وزن, بنية بارول_وزن *lw)
{
	u64 fact = scale_بارول_ښکته(وزن);
	u32 fact_hi = (u32)(fact >> 32);
	صحيح ښويدنه = WMULT_ښويدنه;
	صحيح fs;

	__update_inv_weight(lw);

	إذا (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		ښويدنه -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_وزن);

	fact_hi = (u32)(fact >> 32);
	إذا (fact_hi) {
		fs = fls(fact_hi);
		ښويدنه -= fs;
		fact >>= fs;
	}

	أرجع mul_u64_u32_shr(delta_exec, fact, ښويدنه);
}
#وإلا
ساكن u64 __calc_delta(u64 delta_exec, بینښه اوږد وزن, بنية بارول_وزن *lw)
{
	أرجع (delta_exec * وزن) / lw->وزن;
}
#د_که_پای

/*
 * delta /= w
 */
ساكن مضمن u64 calc_delta_fair(u64 delta, بنية sched_entity *se)
{
	إذا (se->h_بارول.وزن != NICE_0_بارول)
		delta = __calc_delta(delta, NICE_0_بارول, &se->h_بارول);

	أرجع delta;
}

ثابت بنية sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED

/* Walk up scheduling entities hierarchy */
#تعریف_کړه for_each_sched_entity(se) \
		لكل (; se; se = se->پلار)

ساكن مضمن bool list_جمع_کړه_ونې_پاڼه_cfs_rq(بنية cfs_rq *cfs_rq)
{
	بنية rq *rq = rq_د(cfs_rq);
	صحيح cpu = cpu_د(rq);

	إذا (cfs_rq->on_list)
		أرجع rq->tmp_alone_branch == &rq->ونې_پاڼه_cfs_rq_list;

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
	إذا (cfs_rq->tg->پلار &&
	    tg_cfs_rq(cfs_rq->tg->پلار, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_جمع_کړه_لکۍ_rcu(&cfs_rq->ونې_پاڼه_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->پلار, cpu)->ونې_پاڼه_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->ونې_پاڼه_cfs_rq_list;
		أرجع true;
	}

	إذا (!cfs_rq->tg->پلار) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_جمع_کړه_لکۍ_rcu(&cfs_rq->ونې_پاڼه_cfs_rq_list,
			&rq->ونې_پاڼه_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->ونې_پاڼه_cfs_rq_list;
		أرجع true;
	}

	/*
	 * The parent has not already been added so we want to
	 * make sure that it will be put after us.
	 * tmp_alone_branch points to the begin of the branch
	 * where we will add parent.
	 */
	list_جمع_کړه_rcu(&cfs_rq->ونې_پاڼه_cfs_rq_list, rq->tmp_alone_branch);
	/*
	 * update tmp_alone_branch to points to the new begin
	 * of the branch
	 */
	rq->tmp_alone_branch = &cfs_rq->ونې_پاڼه_cfs_rq_list;
	أرجع false;
}

ساكن مضمن فراغ list_del_ونې_پاڼه_cfs_rq(بنية cfs_rq *cfs_rq)
{
	إذا (cfs_rq->on_list) {
		بنية rq *rq = rq_د(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		إذا (rq->tmp_alone_branch == &cfs_rq->ونې_پاڼه_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->ونې_پاڼه_cfs_rq_list.prev;

		list_del_rcu(&cfs_rq->ونې_پاڼه_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

ساكن مضمن فراغ assert_list_ونې_پاڼه_cfs_rq(بنية rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->ونې_پاڼه_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#تعریف_کړه for_each_ونې_پاڼه_cfs_rq_خوندي(rq, cfs_rq, pos)			\
	list_for_each_ثبت_خوندي(cfs_rq, pos, &rq->ونې_پاڼه_cfs_rq_list,	\
				 ونې_پاڼه_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
ساكن مضمن بنية cfs_rq *
is_same_ډله(بنية sched_entity *se, بنية sched_entity *pse)
{
	إذا (se->cfs_rq == pse->cfs_rq)
		أرجع se->cfs_rq;

	أرجع عدم;
}

ساكن مضمن بنية sched_entity *پلار_entity(ثابت بنية sched_entity *se)
{
	أرجع se->پلار;
}

ساكن صحيح tg_is_idle(بنية task_ډله *tg)
{
	أرجع tg->idle > 0;
}

ساكن صحيح cfs_rq_is_idle(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_rq->idle > 0;
}

ساكن صحيح se_is_idle(بنية sched_entity *se)
{
	إذا (entity_is_task(se))
		أرجع task_has_idle_policy(task_د(se));
	أرجع cfs_rq_is_idle(ډله_cfs_rq(se));
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

#تعریف_کړه for_each_sched_entity(se) \
		لكل (; se; se = عدم)

ساكن مضمن bool list_جمع_کړه_ونې_پاڼه_cfs_rq(بنية cfs_rq *cfs_rq)
{
	أرجع true;
}

ساكن مضمن فراغ list_del_ونې_پاڼه_cfs_rq(بنية cfs_rq *cfs_rq)
{
}

ساكن مضمن فراغ assert_list_ونې_پاڼه_cfs_rq(بنية rq *rq)
{
}

#تعریف_کړه for_each_ونې_پاڼه_cfs_rq_خوندي(rq, cfs_rq, pos)	\
		لكل (cfs_rq = &rq->cfs, pos = عدم; cfs_rq; cfs_rq = pos)

ساكن مضمن بنية sched_entity *پلار_entity(بنية sched_entity *se)
{
	أرجع عدم;
}

ساكن مضمن صحيح tg_is_idle(بنية task_ډله *tg)
{
	أرجع 0;
}

ساكن صحيح cfs_rq_is_idle(بنية cfs_rq *cfs_rq)
{
	أرجع 0;
}

ساكن صحيح se_is_idle(بنية sched_entity *se)
{
	أرجع task_has_idle_policy(task_د(se));
}

#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

ساكن __always_inline
bool account_cfs_rq_runtime(بنية cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

خارجي فراغ __BUILD_BUG_vruntime_cmp(فراغ);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#تعریف_کړه vruntime_cmp(A, CMP_STR, B) ({				\
	صحيح __res = 0;						\
								\
	إذا (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} وإلا إذا (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} وإلا إذا (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} وإلا إذا (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} وإلا {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

خارجي فراغ __BUILD_BUG_vruntime_op(فراغ);

#تعریف_کړه vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	إذا (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} وإلا {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


ساكن مضمن __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	إذا (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	أرجع max_vruntime;
}

ساكن مضمن __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	إذا (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	أرجع min_vruntime;
}

ساكن مضمن bool entity_before(ثابت بنية sched_entity *a,
				 ثابت بنية sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	أرجع vruntime_cmp(a->deadline, "<", b->deadline);
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
ساكن مضمن s64 entity_کلی(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	أرجع vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#تعریف_کړه __node_2_se(غوټه) \
	rb_ثبت((غوټه), بنية sched_entity, پرلپسې_غوټه)

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
ساكن مضمن بینښه اوږد avg_vruntime_وزن(بنية cfs_rq *cfs_rq, بینښه اوږد w)
{
#که_تعریف_شوی CONFIG_64_بټ
	إذا (cfs_rq->مجموعه_ښويدنه)
		w = max(2UL, w >> cfs_rq->مجموعه_ښويدنه);
#د_که_پای
	أرجع w;
}

ساكن مضمن فراغ
__sum_w_vruntime_add(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	بینښه اوږد وزن = avg_vruntime_وزن(cfs_rq, se->h_بارول.وزن);
	s64 w_vruntime, کلی = entity_کلی(cfs_rq, se);

	w_vruntime = کلی * وزن;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->مجموعه_w_vruntime += w_vruntime;
	cfs_rq->مجموعه_وزن += وزن;
}

ساكن فراغ
مجموعه_w_vruntime_جمع_کړه_paranoid(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	بینښه اوږد وزن;
	s64 کلی, tmp;

again:
	وزن = avg_vruntime_وزن(cfs_rq, se->h_بارول.وزن);
	کلی = entity_کلی(cfs_rq, se);

	إذا (check_mul_ډکېدنه(کلی, وزن, &کلی))
		اذهب ډکېدنه;

	إذا (check_جمع_کړه_ډکېدنه(cfs_rq->مجموعه_w_vruntime, کلی, &tmp))
		اذهب ډکېدنه;

	cfs_rq->مجموعه_w_vruntime = tmp;
	cfs_rq->مجموعه_وزن += وزن;
	أرجع;

ډکېدنه:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->مجموعه_ښويدنه >= 10);
	cfs_rq->مجموعه_ښويدنه++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->مجموعه_w_vruntime = 0;
	cfs_rq->مجموعه_وزن = 0;

	لكل (بنية rb_غوټه *غوټه = cfs_rq->tasks_timeline.rb_leftmost;
	     غوټه; غوټه = rb_بل(غوټه))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(غوټه));

	اذهب again;
}

ساكن فراغ
مجموعه_w_vruntime_جمع_کړه(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	إذا (sched_feat(PARANOID_AVG))
		أرجع مجموعه_w_vruntime_جمع_کړه_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

ساكن فراغ
مجموعه_w_vruntime_sub(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	بینښه اوږد وزن = avg_vruntime_وزن(cfs_rq, se->h_بارول.وزن);
	s64 کلی = entity_کلی(cfs_rq, se);

	cfs_rq->مجموعه_w_vruntime -= کلی * وزن;
	cfs_rq->مجموعه_وزن -= وزن;
}

ساكن مضمن
فراغ update_zero_vruntime(بنية cfs_rq *cfs_rq, s64 delta)
{
	/*
	 * v' = v + d ==> sum_w_vruntime' = sum_w_vruntime - d*sum_weight
	 */
	cfs_rq->مجموعه_w_vruntime -= cfs_rq->مجموعه_وزن * delta;
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
u64 avg_vruntime(بنية cfs_rq *cfs_rq)
{
	بنية sched_entity *curr = cfs_rq->curr;
	اوږد وزن = cfs_rq->مجموعه_وزن;
	s64 delta = 0;

	إذا (curr && !curr->on_rq)
		curr = عدم;

	إذا (وزن) {
		s64 runtime = cfs_rq->مجموعه_w_vruntime;

		إذا (curr) {
			بینښه اوږد w = avg_vruntime_وزن(cfs_rq, curr->h_بارول.وزن);

			runtime += entity_کلی(cfs_rq, curr) * w;
			وزن += w;
		}

		/* sign flips effective floor / ceiling */
		إذا (runtime < 0)
			runtime -= (وزن - 1);

		delta = div64_long(runtime, وزن);
	} وإلا إذا (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	أرجع cfs_rq->zero_vruntime;
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
ساكن u64 ineligible_vruntime(بنية cfs_rq *cfs_rq)
{
	بنية sched_entity *curr = cfs_rq->curr;
	اوږد وزن = cfs_rq->مجموعه_وزن;
	s64 delta = 0;

	إذا (curr && !curr->on_rq)
		curr = عدم;

	إذا (وزن) {
		s64 runtime = cfs_rq->مجموعه_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		إذا (runtime < 0)
			runtime -= (وزن - 1);

		delta = div64_long(runtime, وزن);
	}

	أرجع cfs_rq->zero_vruntime + delta + 1;
}

ساكن مضمن u64 cfs_rq_max_ټوټه(بنية cfs_rq *cfs_rq);

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
ساكن s64 entity_lag(بنية cfs_rq *cfs_rq, بنية sched_entity *se, u64 avruntime)
{
	u64 max_ټوټه = cfs_rq_max_ټوټه(cfs_rq) + TICK_NSEC;
	s64 vlag, حد;

	vlag = avruntime - se->vruntime;
	حد = calc_delta_fair(max_ټوټه, se);

	أرجع clamp(vlag, -حد, حد);
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
ساكن __always_inline
bool update_entity_lag(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	إذا (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		إذا (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	أرجع avruntime - vlag != se->vruntime;
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
ساكن صحيح vruntime_eligible(بنية cfs_rq *cfs_rq, u64 vruntime)
{
	بنية sched_entity *curr = cfs_rq->curr;
	s64 کلی, avg = cfs_rq->مجموعه_w_vruntime;
	اوږد بارول = cfs_rq->مجموعه_وزن;

	إذا (curr && curr->on_rq) {
		بینښه اوږد وزن = avg_vruntime_وزن(cfs_rq, curr->h_بارول.وزن);

		avg += entity_کلی(cfs_rq, curr) * وزن;
		بارول += وزن;
	}

	کلی = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#که_تعریف_شوی CONFIG_64_بټ
#که_تعریف_شوی CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	أرجع avg >= (__int128)کلی * بارول;
#وإلا
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	إذا (check_mul_ډکېدنه(کلی, بارول, &rhs))
		أرجع کلی <= 0;

	أرجع avg >= rhs;
#د_که_پای
#وإلا /* 32bit */
	أرجع avg >= کلی * بارول;
#د_که_پای
}

صحيح entity_eligible(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	أرجع vruntime_eligible(cfs_rq, se->vruntime);
}

ساكن مضمن u64 cfs_rq_min_ټوټه(بنية cfs_rq *cfs_rq)
{
	بنية sched_entity *ریښه = __pick_root_entity(cfs_rq);
	بنية sched_entity *curr = cfs_rq->curr;
	u64 min_ټوټه = ~0ULL;

	إذا (curr && curr->on_rq)
		min_ټوټه = curr->ټوټه;

	إذا (ریښه)
		min_ټوټه = min(min_ټوټه, ریښه->min_ټوټه);

	أرجع min_ټوټه;
}

ساكن مضمن u64 cfs_rq_max_ټوټه(بنية cfs_rq *cfs_rq)
{
	بنية sched_entity *ریښه = __pick_root_entity(cfs_rq);
	بنية sched_entity *curr = cfs_rq->curr;
	u64 max_ټوټه = 0ULL;

	إذا (curr && curr->on_rq)
		max_ټوټه = curr->ټوټه;

	إذا (ریښه)
		max_ټوټه = max(max_ټوټه, ریښه->max_ټوټه);

	أرجع max_ټوټه;
}

ساكن مضمن bool __entity_less(بنية rb_غوټه *a, ثابت بنية rb_غوټه *b)
{
	أرجع entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
ساكن مضمن u64 __min_vruntime(u64 a, u64 b)
{
	أرجع vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(ساكن, min_vruntime_cb,
		     بنية sched_entity, پرلپسې_غوټه,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(ټوټه, min_ټوټه, min),
		     RB_AUG(ټوټه, max_ټوټه, max));

/*
 * Enqueue an entity into the rb-tree:
 */
ساكن فراغ __enqueue_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	WARN_ON_ONCE(&rq_د(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	مجموعه_w_vruntime_جمع_کړه(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_ټوټه = se->ټوټه;
	se->max_ټوټه = se->ټوټه;

	rb_جمع_کړه_augmented_cached(&se->پرلپسې_غوټه, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

ساكن فراغ __dequeue_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	WARN_ON_ONCE(&rq_د(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->پرلپسې_غوټه, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	مجموعه_w_vruntime_sub(cfs_rq, se);
}

بنية sched_entity *__pick_root_entity(بنية cfs_rq *cfs_rq)
{
	بنية rb_غوټه *ریښه = cfs_rq->tasks_timeline.rb_ریښه.rb_غوټه;

	إذا (!ریښه)
		أرجع عدم;

	أرجع __node_2_se(ریښه);
}

بنية sched_entity *__pick_first_entity(بنية cfs_rq *cfs_rq)
{
	بنية rb_غوټه *کیڼ = rb_لومړی_cached(&cfs_rq->tasks_timeline);

	إذا (!کیڼ)
		أرجع عدم;

	أرجع __node_2_se(کیڼ);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
ساكن مضمن فراغ سېټ_protect_ټوټه(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	u64 ټوټه = normalized_sysctl_sched_بنسټ_ټوټه;
	u64 vprot = se->deadline;

	إذا (sched_feat(پرلپسې_منزل_PARITY))
		ټوټه = cfs_rq_min_ټوټه(cfs_rq);

	ټوټه = min(ټوټه, se->ټوټه);

	/* If there are shorter slices than se's one */
	إذا (ټوټه != se->ټوټه) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(ټوټه, se));
		إذا (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

ساكن مضمن فراغ update_protect_ټوټه(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 ټوټه = cfs_rq_min_ټوټه(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(ټوټه, se));

	إذا (sched_feat(PREEMPT_SHORT) && ټوټه != se->ټوټه)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

ساكن مضمن bool protect_ټوټه(بنية sched_entity *se)
{
	أرجع vruntime_cmp(se->vruntime, "<", se->vprot);
}

ساكن مضمن فراغ cancel_protect_ټوټه(بنية sched_entity *se)
{
	إذا (protect_ټوټه(se))
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
ساكن بنية sched_entity *pick_eevdf(بنية cfs_rq *cfs_rq, bool protect)
{
	بنية rb_غوټه *غوټه = cfs_rq->tasks_timeline.rb_ریښه.rb_غوټه;
	بنية sched_entity *se = __pick_first_entity(cfs_rq);
	بنية sched_entity *curr = cfs_rq->curr;
	بنية sched_entity *غوره = عدم;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	إذا (cfs_rq->h_nr_queued == 1)
		أرجع curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	إذا (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->بل && entity_eligible(cfs_rq, cfs_rq->بل)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->بل->sched_delayed);
		أرجع cfs_rq->بل;
	}

	إذا (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = عدم;

	إذا (curr && protect && protect_ټوټه(curr))
		أرجع curr;

	/* Pick the leftmost entity if it's eligible */
	إذا (se && entity_eligible(cfs_rq, se)) {
		غوره = se;
		اذهب وموندل;
	}

	/* Heap search for the EEVD entity */
	طالما (غوټه) {
		بنية rb_غوټه *کیڼ = غوټه->rb_کیڼ;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		إذا (کیڼ && vruntime_eligible(cfs_rq,
					__node_2_se(کیڼ)->min_vruntime)) {
			غوټه = کیڼ;
			استمر;
		}

		se = __node_2_se(غوټه);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		إذا (entity_eligible(cfs_rq, se)) {
			غوره = se;
			اكسر;
		}

		غوټه = غوټه->rb_ښی;
	}
وموندل:
	إذا (!غوره || (curr && entity_before(curr, غوره)))
		غوره = curr;

	أرجع غوره;
}

بنية sched_entity *__pick_last_entity(بنية cfs_rq *cfs_rq)
{
	بنية rb_غوټه *وروستی = rb_وروستی(&cfs_rq->tasks_timeline.rb_ریښه);

	إذا (!وروستی)
		أرجع عدم;

	أرجع __node_2_se(وروستی);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
صحيح sched_update_scaling(فراغ)
{
	بینښه صحيح ضریب = get_update_sysctl_ضریب();

#تعریف_کړه WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (ضریب))
	WRT_SYSCTL(sched_بنسټ_ټوټه);
#تعریف_لغوه WRT_SYSCTL

	أرجع 0;
}

ساكن فراغ clear_buddies(بنية cfs_rq *cfs_rq, بنية sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
ساكن bool update_deadline(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	إذا (vruntime_cmp(se->vruntime, "<", se->deadline))
		أرجع false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	إذا (!se->custom_ټوټه)
		se->ټوټه = sysctl_sched_بنسټ_ټوټه;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->ټوټه, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	أرجع true;
}

#ضمن "pelt.h"

ساكن صحيح select_idle_sibling(بنية task_struct *p, صحيح prev_cpu, صحيح cpu);
ساكن بینښه اوږد task_h_بارول(بنية task_struct *p);
ساكن بینښه اوږد ظرفیت_د(صحيح cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
فراغ init_entity_runnable_اوسط(بنية sched_entity *se)
{
	بنية sched_avg *sa = &se->avg;

	memset(sa, 0, حجم(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	إذا (entity_is_task(se))
		sa->بارول_avg = scale_بارول_ښکته(se->بارول.وزن);

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
فراغ post_init_entity_util_avg(بنية task_struct *p)
{
	بنية sched_entity *se = &p->se;
	بنية cfs_rq *cfs_rq = cfs_rq_د(se);
	بنية sched_avg *sa = &se->avg;
	اوږد cpu_scale = arch_scale_cpu_ظرفیت(cpu_د(rq_د(cfs_rq)));
	اوږد cap = (اوږد)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	إذا (p->sched_class != &fair_sched_class) {
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
		se->avg.وروستی_update_وخت = cfs_rq_clock_pelt(cfs_rq);
		أرجع;
	}

	إذا (cap > 0) {
		إذا (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_وزن(se);
			sa->util_avg /= (cfs_rq->avg.بارول_avg + 1);

			إذا (sa->util_avg > cap)
				sa->util_avg = cap;
		} وإلا {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

ساكن مضمن فراغ account_mm_sched(بنية rq *rq, بنية task_struct *p, s64 delta_exec);

ساكن s64 update_se(بنية rq *rq, بنية sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_پیل;
	إذا (unlikely(delta_exec <= 0))
		أرجع delta_exec;

	se->exec_پیل = now;
	إذا (entity_is_task(se)) {
		بنية task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_پیل = now;
		running->se.مجموعه_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_ډله_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} وإلا {
		/* If not task, account the time against donor se  */
		se->مجموعه_exec_runtime += delta_exec;
	}

	إذا (schedstat_enabled()) {
		بنية sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	أرجع delta_exec;
}

#که_تعریف_شوی CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#تعریف_کړه EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#تعریف_کړه EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly بینښه صحيح llc_aggr_tolerance	= 1;
__read_mostly بینښه صحيح llc_epoch_period	= EPOCH_PERIOD;
__read_mostly بینښه صحيح llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly بینښه صحيح llc_imb_pct		= 20;
__read_mostly بینښه صحيح llc_overaggr_pct	= 50;

ساكن صحيح llc_id(صحيح cpu)
{
	إذا (cpu < 0)
		أرجع -1;

	أرجع فی_cpu(sd_llc_id, cpu);
}

ساكن مضمن صحيح get_sched_cache_scale(صحيح mul)
{
	بینښه صحيح tol = READ_ONCE(llc_aggr_tolerance);

	إذا (!tol)
		أرجع 0;

	إذا (tol >= 100)
		أرجع لوی_عدد;

	أرجع (1 + (tol - 1) * mul);
}

ساكن bool exceed_llc_ظرفیت(بنية sched_cache_ډله *grp, صحيح cpu)
{
#که_تعریف_شوی CONFIG_NUMA_BALANCING
	بینښه اوږد llc, footprint;
	بنية sched_domain *sd;
	صحيح scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	إذا (!sd)
		أرجع true;

	إذا (static_branch_likely(&sched_numa_balancing)) {
		/*
		 * TBD: RDT exclusive LLC ways reserved should be
		 * excluded.
		 */
		llc = sd->llc_بایټونه;
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
		إذا (scale == لوی_عدد)
			أرجع false;

		أرجع ((llc * (u64)scale) < (footprint * پاڼه_اندازه));
	}
#د_که_پای
	أرجع false;
}

ساكن bool invalid_llc_nr(بنية sched_cache_ډله *grp, بنية task_struct *p,
			   صحيح cpu)
{
	صحيح scale;

	إذا (get_nr_تارونه(p) <= 1)
		أرجع true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	إذا (scale == لوی_عدد)
		أرجع false;

	أرجع !ځاییږي_ظرفیت((READ_ONCE(grp->nr_running_avg) * cpu_smt_عدد_تارونه),
			(scale * فی_cpu(sd_llc_اندازه, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
ساكن bool task_pref_llc_runnable(بنية task_struct *p)
{
	أرجع p->pref_llc_queued && !p->se.sched_delayed;
}

ساكن فراغ pref_llc_running_inc(بنية rq *rq, بنية task_struct *p)
{
	إذا (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

ساكن فراغ pref_llc_running_dec(بنية rq *rq, بنية task_struct *p)
{
	إذا (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

ساكن فراغ account_llc_کتار_ته_ورزیاتول(بنية rq *rq, بنية task_struct *p)
{
	صحيح pref_llc, pref_llc_queued;
	بنية sched_domain *sd;

	pref_llc = p->preferred_llc;
	إذا (pref_llc < 0)
		أرجع;

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
	إذا (sd && (بینښه صحيح)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

ساكن فراغ account_llc_له_کتار_ایستل(بنية rq *rq, بنية task_struct *p)
{
	بنية sched_domain *sd;
	صحيح pref_llc;

	pref_llc = p->preferred_llc;
	إذا (pref_llc < 0)
		أرجع;

	rq->nr_llc_running--;
	إذا (p->pref_llc_queued) {
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
	إذا (sd && (بینښه صحيح)pref_llc < sd->llc_max) {
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
		إذا (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

صحيح mm_init_sched(بنية mm_struct *mm,
		  بنية sched_cache_وخت __percpu *_pcpu_sched)
{
	بنية sched_cache_ډله *grp;
	بینښه اوږد epoch = 0;
	صحيح i;

	grp = kzalloc_obj(*grp);
	إذا (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = عدم;
		أرجع -ENOMEM;
	}

	for_each_possible_cpu(i) {
		بنية sched_cache_وخت *pcpu_sched = فی_cpu_ptr(_pcpu_sched, i);
		بنية rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_قلف_init(&grp->قلف);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->بل_لټون = jiffies;
	grp->nr_running_avg = 0;
	grp->footprint = 0;
	refcount_سېټ(&grp->refcnt, 1);
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
	أرجع 0;
}

ساكن فراغ sched_cache_ډله_free_rcu(بنية rcu_سر *rcu)
{
	بنية sched_cache_ډله *grp =
		container_د(rcu, بنية sched_cache_ډله, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

ساكن فراغ sched_cache_ډله_put(بنية sched_cache_ډله *grp)
{
	إذا (!grp || !refcount_dec_and_test(&grp->refcnt))
		أرجع;

	call_rcu(&grp->rcu, sched_cache_ډله_free_rcu);
}

DEFINE_FREE(sched_cache_ډله_put, بنية sched_cache_ډله *,
	    sched_cache_ډله_put(_T));

#تعریف_کړه rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

ساكن بنية sched_cache_ډله *sched_cache_replace_grp(بنية task_struct *p,
							 بنية sched_cache_ډله *new)
{
	بنية sched_cache_ډله *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_ټاکل_اشاره_ګر(p->sched_cache_grp, new);

	أرجع old;
}

بنية sched_cache_ډله *sched_cache_ډله_get(بنية sched_cache_ډله *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	إذا (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = عدم;

	أرجع grp;
}

بنية sched_cache_ډله *task_cache_ډله_get(بنية task_struct *p)
{
	guard(rcu)();
	أرجع sched_cache_ډله_get(rcu_dereference(p->sched_cache_grp));
}

فراغ sched_cache_fork(بنية task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_اشاره_ګر(p->sched_cache_grp,
			 sched_cache_ډله_get(p->mm->sched_cache_grp));
}

فراغ sched_cache_fork_cleanup(بنية task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_ډله_put(rcu_access_اشاره_ګر(p->sched_cache_grp));
	RCU_INIT_اشاره_ګر(p->sched_cache_grp, عدم);
}

فراغ sched_cache_exec_mmap(بنية task_struct *p, بنية mm_struct *mm)
{
	بنية sched_cache_ډله *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_ډله_get(mm->sched_cache_grp));
	sched_cache_ډله_put(old);
}

فراغ sched_cache_exit_mm(بنية task_struct *p)
{
	بنية sched_cache_ډله *grp = sched_cache_replace_grp(p, عدم);

#که_تعریف_شوی CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	إذا (grp && p->ټول_numa_تېروتنې) {
		بینښه اوږد fp = READ_ONCE(grp->footprint);
		بینښه اوږد sub = min(fp, p->ټول_numa_تېروتنې);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#د_که_پای
	sched_cache_ډله_put(grp);
}

فراغ mm_destroy_sched(بنية mm_struct *mm)
{
	sched_cache_ډله_put(mm->sched_cache_grp);
	mm->sched_cache_grp = عدم;
}

/* because why would C be fully specified */
ساكن __always_inline فراغ __shr_u64(u64 *val, بینښه صحيح n)
{
	إذا (n >= 64) {
		*val = 0;
		أرجع;
	}
	*val >>= n;
}

ساكن مضمن فراغ __update_mm_sched(بنية rq *rq,
				     بنية sched_cache_وخت *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_قلف);

	بینښه صحيح period = max(READ_ONCE(llc_epoch_period), 1U);
	بینښه اوږد n, now = jiffies;
	اوږد delta = now - rq->cpu_epoch_بل;

	إذا (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_بل += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	إذا (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

ساكن بینښه اوږد fraction_mm_sched(بنية rq *rq,
				       بنية sched_cache_وخت *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_قلف);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	أرجع div64_u64(NICE_0_بارول * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

ساكن صحيح get_pref_llc(بنية task_struct *p, بنية sched_cache_ډله *grp)
{
	صحيح mm_sched_llc = -1, mm_sched_cpu;

	إذا (!grp)
		أرجع -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	إذا (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#که_تعریف_شوی CONFIG_NUMA_BALANCING
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
		إذا (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_منزل_غوټه(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#د_که_پای
	}

	أرجع mm_sched_llc;
}

ساكن بینښه صحيح task_running_on_cpu(صحيح cpu, بنية task_struct *p);

ساكن مضمن
فراغ account_mm_sched(بنية rq *rq, بنية task_struct *p, s64 delta_exec)
{
	بنية sched_cache_ډله *grp = rcu_dereference_all(p->sched_cache_grp);
	بنية sched_cache_وخت *pcpu_sched;
	صحيح mm_sched_llc = -1;
	بینښه اوږد epoch;

	إذا (!sched_cache_enabled())
		أرجع;

	إذا (p->sched_class != &fair_sched_class)
		أرجع;
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
	إذا (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		أرجع;

	pcpu_sched = فی_cpu_ptr(grp->pcpu_sched, cpu_د(rq));

	scoped_guard (raw_spinlock, &rq->cpu_epoch_قلف) {
		__update_mm_sched(rq, pcpu_sched);
		pcpu_sched->runtime += delta_exec;
		rq->cpu_runtime += delta_exec;
		epoch = rq->cpu_epoch;
	}

	/*
	 * If this process hasn't hit task_cache_work() for a while invalidate
	 * its preferred state.
	 */
	إذا ((اوږد)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_د(rq)) ||
	    exceed_llc_ظرفیت(grp, cpu_د(rq))) {
		إذا (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	إذا (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_له_کتار_ایستل(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_کتار_ته_ورزیاتول(rq, p);
	}
}

ساكن فراغ task_tick_cache(بنية rq *rq, بنية task_struct *p)
{
	بنية sched_cache_ډله *grp = rcu_dereference_all(p->sched_cache_grp);
	بنية callback_سر *work = &p->cache_work;
	بینښه اوږد epoch;

	إذا (!sched_cache_enabled())
		أرجع;

	إذا (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		أرجع;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	إذا (وخت_after_eq(grp->epoch, epoch))
		أرجع;

	guard(raw_spinlock)(&grp->قلف);

	إذا (work->بل == work) {
		task_work_جمع_کړه(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

ساكن فراغ get_لټون_cpumasks(cpumask_var_t cpus, بنية task_struct *p,
			      بنية sched_cache_ډله *grp)
{
#که_تعریف_شوی CONFIG_NUMA_BALANCING
	صحيح cpu, curr_cpu, nid, pref_nid;

	إذا (!static_branch_likely(&sched_numa_balancing))
		اذهب بهر;

	cpu = READ_ONCE(grp->cpu);
	إذا (cpu != -1)
		nid = cpu_منزل_غوټه(cpu);
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
	إذا (pref_nid == NUMA_NO_غوټه)
		اذهب بهر;

	cpumask_or(cpus, cpus, cpumask_د_غوټه(pref_nid));

	/* honor the task's preferred LLC CPU */
	إذا (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_غوټه)
		cpumask_or(cpus, cpus, cpumask_د_غوټه(nid));

	/* make sure the task's current running node is included */
	إذا (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_د_غوټه(cpu_منزل_غوټه(curr_cpu)));

	أرجع;

بهر:
#د_که_پای
	cpumask_copy(cpus, cpu_online_ماسک);
}

ساكن مضمن فراغ update_avg_scale(u64 *avg, u64 sample)
{
	صحيح ضریب = فی_cpu(sd_llc_اندازه, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 وېشونکی;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	وېشونکی = clamp_t(u32, (ضریب >> 2), 2, 8);
	*avg += div64_s64(diff, وېشونکی);
}

ساكن فراغ task_cache_work(بنية callback_سر *work)
{
	بنية sched_cache_ډله *grp __free(sched_cache_ډله_put) = عدم;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	صحيح cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	بینښه اوږد بل_لټون, now = jiffies;
	بنية task_struct *p = current, *cur;
	بینښه اوږد curr_m_a_occ = 0;
	بینښه اوږد m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->بل = work;

	إذا (p->flags & PF_EXITING)
		أرجع;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_ډله_get(p);
	إذا (!grp)
		أرجع;

	بل_لټون = READ_ONCE(grp->بل_لټون);
	إذا (وخت_before(now, بل_لټون))
		أرجع;

	/* only 1 thread is allowed to scan */
	إذا (!try_cmpxchg(&grp->بل_لټون, &بل_لټون,
			 now + max_t(بینښه اوږد,
				     READ_ONCE(llc_epoch_period), 1)))
		أرجع;

	curr_cpu = task_cpu(p);
	إذا (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_ظرفیت(grp, curr_cpu)) {
		إذا (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		أرجع;
	}

	إذا (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		أرجع;

	scoped_guard (cpus_read_قلف) {
		guard(rcu)();

		get_لټون_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			بنية sched_domain *sd = rcu_dereference_all(فی_cpu(sd_llc, cpu));
			بینښه اوږد occ, m_occ = 0, a_occ = 0;
			صحيح m_cpu = -1, i;

			إذا (!sd)
				استمر;

			for_each_cpu(i, sched_domain_پراختیا(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							فی_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				إذا (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				إذا (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
				    rcu_access_اشاره_ګر(cur->sched_cache_grp) == grp)
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
			إذا (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			إذا (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_پراختیا(sd));
		}
	}

	إذا (m_a_occ > (2 * curr_m_a_occ)) {
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

فراغ init_sched_mm(بنية task_struct *p)
{
	بنية callback_سر *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->بل = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_اشاره_ګر(p->sched_cache_grp, عدم);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#وإلا /* CONFIG_SCHED_CACHE */

ساكن مضمن فراغ account_mm_sched(بنية rq *rq, بنية task_struct *p,
				    s64 delta_exec) { }

فراغ init_sched_mm(بنية task_struct *p) { }

ساكن فراغ task_tick_cache(بنية rq *rq, بنية task_struct *p) { }

ساكن مضمن صحيح get_pref_llc(بنية task_struct *p,
			       بنية mm_struct *mm)
{
	أرجع -1;
}

ساكن فراغ account_llc_کتار_ته_ورزیاتول(بنية rq *rq, بنية task_struct *p) {}

ساكن فراغ account_llc_له_کتار_ایستل(بنية rq *rq, بنية task_struct *p) {}

ساكن فراغ pref_llc_running_inc(بنية rq *rq, بنية task_struct *p) {}

ساكن فراغ pref_llc_running_dec(بنية rq *rq, بنية task_struct *p) {}

#د_که_پای /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(بنية rq *rq)
{
	أرجع update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
ساكن فراغ update_curr(بنية cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	بنية sched_entity *curr = cfs_rq->h_curr;
	بنية rq *rq = rq_د(cfs_rq);
	s64 delta_exec;
	bool resched;

	إذا (unlikely(!curr))
		أرجع;

	delta_exec = update_se(rq, curr);
	إذا (unlikely(delta_exec <= 0))
		أرجع;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	إذا (!entity_is_task(curr))
		أرجع;

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

	إذا (cfs_rq->h_nr_queued == 1)
		أرجع;

	إذا (resched || !protect_ټوټه(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

ساكن فراغ update_curr_fair(بنية rq *rq)
{
	بنية sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_د(se));
}

ساكن مضمن فراغ
update_stats_wait_پیل_fair(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	بنية sched_statistics *stats;
	بنية task_struct *p = عدم;

	إذا (!schedstat_enabled())
		أرجع;

	stats = __schedstats_from_se(se);

	إذا (entity_is_task(se))
		p = task_د(se);

	__update_stats_wait_start(rq_د(cfs_rq), p, stats);
}

ساكن مضمن فراغ
update_stats_wait_پای_ټکی_fair(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	بنية sched_statistics *stats;
	بنية task_struct *p = عدم;

	إذا (!schedstat_enabled())
		أرجع;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	إذا (unlikely(!schedstat_val(stats->wait_پیل)))
		أرجع;

	إذا (entity_is_task(se))
		p = task_د(se);

	__update_stats_wait_end(rq_د(cfs_rq), p, stats);
}

ساكن مضمن فراغ
update_stats_کتار_ته_ورزیاتول_sleeper_fair(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	بنية sched_statistics *stats;
	بنية task_struct *tsk = عدم;

	إذا (!schedstat_enabled())
		أرجع;

	stats = __schedstats_from_se(se);

	إذا (entity_is_task(se))
		tsk = task_د(se);

	__update_stats_enqueue_sleeper(rq_د(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
ساكن مضمن فراغ
update_stats_کتار_ته_ورزیاتول_fair(بنية cfs_rq *cfs_rq, بنية sched_entity *se, صحيح flags)
{
	إذا (!schedstat_enabled())
		أرجع;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	إذا (se != cfs_rq->h_curr)
		update_stats_wait_پیل_fair(cfs_rq, se);

	إذا (flags & کتار_ته_ورزیاتول_WAKEUP)
		update_stats_کتار_ته_ورزیاتول_sleeper_fair(cfs_rq, se);
}

ساكن مضمن فراغ
update_stats_له_کتار_ایستل_fair(بنية cfs_rq *cfs_rq, بنية sched_entity *se, صحيح flags)
{

	إذا (!schedstat_enabled())
		أرجع;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	إذا (se != cfs_rq->h_curr)
		update_stats_wait_پای_ټکی_fair(cfs_rq, se);

	إذا ((flags & له_کتار_ایستل_SLEEP) && entity_is_task(se)) {
		بنية task_struct *tsk = task_د(se);
		بینښه صحيح state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		إذا (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_پیل,
				      rq_clock(rq_د(cfs_rq)));
		إذا (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.بلاک_پیل,
				      rq_clock(rq_د(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
ساكن مضمن فراغ
update_stats_curr_پیل(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_پیل = rq_clock_task(rq_د(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
ساكن مضمن bool is_core_idle(صحيح cpu)
{
	صحيح sibling;

	for_each_cpu(sibling, cpu_smt_ماسک(cpu)) {
		إذا (cpu == sibling)
			استمر;

		إذا (!idle_cpu(sibling))
			أرجع false;
	}

	أرجع true;
}

#که_تعریف_شوی CONFIG_NUMA
#تعریف_کړه NUMA_IMBALANCE_MIN 2

ساكن مضمن اوږد
adjust_numa_imbalance(صحيح imbalance, صحيح dst_running, صحيح imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	إذا (dst_running > imb_numa_nr)
		أرجع imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	إذا (imbalance <= NUMA_IMBALANCE_MIN)
		أرجع 0;

	أرجع imbalance;
}
#د_که_پای /* CONFIG_NUMA */

#که_تعریف_شوی CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
بینښه صحيح sysctl_numa_balancing_لټون_period_min = 1000;
بینښه صحيح sysctl_numa_balancing_لټون_period_max = 60000;

/* Portion of address space to scan in MB */
بینښه صحيح sysctl_numa_balancing_لټون_اندازه = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
بینښه صحيح sysctl_numa_balancing_لټون_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
بینښه صحيح sysctl_numa_balancing_hot_پوله = MSEC_فی_SEC;

بنية numa_ډله {
	refcount_t refcount;

	spinlock_t قلف; /* nr_tasks, tasks */
	صحيح nr_tasks;
	pid_t gid;
	صحيح فعال_nodes;

	بنية rcu_سر rcu;
	بینښه اوږد ټول_تېروتنې;
	بینښه اوږد max_تېروتنې_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	بینښه اوږد تېروتنې[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
ساكن بنية numa_ډله *deref_task_numa_ډله(بنية task_struct *p)
{
	أرجع rcu_dereference_check(p->numa_ډله, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

ساكن بنية numa_ډله *deref_curr_numa_ډله(بنية task_struct *p)
{
	أرجع rcu_dereference_protected(p->numa_ډله, p == current);
}

ساكن مضمن بینښه اوږد ډله_تېروتنې_priv(بنية numa_ډله *ng);
ساكن مضمن بینښه اوږد ډله_تېروتنې_shared(بنية numa_ډله *ng);

ساكن بینښه صحيح task_nr_لټون_windows(بنية task_struct *p)
{
	بینښه اوږد rss = 0;
	بینښه اوږد nr_لټون_پاڼې;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_لټون_پاڼې = MB_منزل_پاڼې(sysctl_numa_balancing_لټون_اندازه);
	rss = get_mm_rss(p->mm);
	إذا (!rss)
		rss = nr_لټون_پاڼې;

	rss = پړاو_پورته(rss, nr_لټون_پاڼې);
	أرجع rss / nr_لټون_پاڼې;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#تعریف_کړه MAX_لټون_کړکۍ 2560

ساكن بینښه صحيح task_لټون_min(بنية task_struct *p)
{
	بینښه صحيح لټون_اندازه = READ_ONCE(sysctl_numa_balancing_لټون_اندازه);
	بینښه صحيح لټون, floor;
	بینښه صحيح windows = 1;

	إذا (لټون_اندازه < MAX_لټون_کړکۍ)
		windows = MAX_لټون_کړکۍ / لټون_اندازه;
	floor = 1000 / windows;

	لټون = sysctl_numa_balancing_لټون_period_min / task_nr_لټون_windows(p);
	أرجع max_t(بینښه صحيح, floor, لټون);
}

ساكن بینښه صحيح task_لټون_پیل(بنية task_struct *p)
{
	بینښه اوږد smin = task_لټون_min(p);
	بینښه اوږد period = smin;
	بنية numa_ډله *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_قلف();
	ng = rcu_dereference_all(p->numa_ډله);
	إذا (ng) {
		بینښه اوږد shared = ډله_تېروتنې_shared(ng);
		بینښه اوږد خاص = ډله_تېروتنې_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= خاص + shared + 1;
	}
	rcu_read_unlock();

	أرجع max(smin, period);
}

ساكن بینښه صحيح task_لټون_max(بنية task_struct *p)
{
	بینښه اوږد smin = task_لټون_min(p);
	بینښه اوږد smax;
	بنية numa_ډله *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_لټون_period_max / task_nr_لټون_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_ډله(p);
	إذا (ng) {
		بینښه اوږد shared = ډله_تېروتنې_shared(ng);
		بینښه اوږد خاص = ډله_تېروتنې_priv(ng);
		بینښه اوږد period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= خاص + shared + 1;

		smax = max(smax, period);
	}

	أرجع max(smin, smax);
}

ساكن فراغ account_numa_کتار_ته_ورزیاتول(بنية rq *rq, بنية task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_غوټه);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_غوټه(p));
}

ساكن فراغ account_numa_له_کتار_ایستل(بنية rq *rq, بنية task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_غوټه);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_غوټه(p));
}

/* Shared or private faults. */
#تعریف_کړه NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#تعریف_کړه NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#تعریف_کړه NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_ډله_id(بنية task_struct *p)
{
	بنية numa_ډله *ng;
	pid_t gid = 0;

	rcu_read_قلف();
	ng = rcu_dereference_all(p->numa_ډله);
	إذا (ng)
		gid = ng->gid;
	rcu_read_unlock();

	أرجع gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
ساكن مضمن صحيح task_تېروتنې_idx(تعداد numa_تېروتنې_stats s, صحيح nid, صحيح priv)
{
	أرجع NR_NUMA_HINT_FAULT_TYPES * (s * nr_غوټه_ids + nid) + priv;
}

ساكن مضمن بینښه اوږد task_تېروتنې(بنية task_struct *p, صحيح nid)
{
	إذا (!p->numa_تېروتنې)
		أرجع 0;

	أرجع p->numa_تېروتنې[task_تېروتنې_idx(NUMA_MEM, nid, 0)] +
		p->numa_تېروتنې[task_تېروتنې_idx(NUMA_MEM, nid, 1)];
}

ساكن مضمن بینښه اوږد ډله_تېروتنې(بنية task_struct *p, صحيح nid)
{
	بنية numa_ډله *ng = deref_task_numa_ډله(p);

	إذا (!ng)
		أرجع 0;

	أرجع ng->تېروتنې[task_تېروتنې_idx(NUMA_MEM, nid, 0)] +
		ng->تېروتنې[task_تېروتنې_idx(NUMA_MEM, nid, 1)];
}

ساكن مضمن بینښه اوږد ډله_تېروتنې_cpu(بنية numa_ډله *ډله, صحيح nid)
{
	أرجع ډله->تېروتنې[task_تېروتنې_idx(NUMA_CPU, nid, 0)] +
		ډله->تېروتنې[task_تېروتنې_idx(NUMA_CPU, nid, 1)];
}

ساكن مضمن بینښه اوږد ډله_تېروتنې_priv(بنية numa_ډله *ng)
{
	بینښه اوږد تېروتنې = 0;
	صحيح غوټه;

	for_each_online_غوټه(غوټه) {
		تېروتنې += ng->تېروتنې[task_تېروتنې_idx(NUMA_MEM, غوټه, 1)];
	}

	أرجع تېروتنې;
}

ساكن مضمن بینښه اوږد ډله_تېروتنې_shared(بنية numa_ډله *ng)
{
	بینښه اوږد تېروتنې = 0;
	صحيح غوټه;

	for_each_online_غوټه(غوټه) {
		تېروتنې += ng->تېروتنې[task_تېروتنې_idx(NUMA_MEM, غوټه, 0)];
	}

	أرجع تېروتنې;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#تعریف_کړه فعال_غوټه_FRACTION 3

ساكن bool numa_is_فعال_غوټه(صحيح nid, بنية numa_ډله *ng)
{
	أرجع ډله_تېروتنې_cpu(ng, nid) * فعال_غوټه_FRACTION > ng->max_تېروتنې_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
ساكن بینښه اوږد score_nearby_nodes(بنية task_struct *p, صحيح nid,
					صحيح lim_dist, bool task)
{
	بینښه اوږد score = 0;
	صحيح غوټه, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	إذا (sched_numa_topology_type == NUMA_مستقیم)
		أرجع 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_واټن);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_غوټه(غوټه) {
		بینښه اوږد تېروتنې;
		صحيح dist = غوټه_واټن(nid, غوټه);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		إذا (dist >= max_dist || غوټه == nid)
			استمر;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		إذا (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			استمر;

		/* Add up the faults from nearby nodes. */
		إذا (task)
			تېروتنې = task_تېروتنې(p, غوټه);
		وإلا
			تېروتنې = ډله_تېروتنې(p, غوټه);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		إذا (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			تېروتنې *= (max_dist - dist);
			تېروتنې /= (max_dist - LOCAL_واټن);
		}

		score += تېروتنې;
	}

	أرجع score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
ساكن مضمن بینښه اوږد task_وزن(بنية task_struct *p, صحيح nid,
					صحيح dist)
{
	بینښه اوږد تېروتنې, ټول_تېروتنې;

	إذا (!p->numa_تېروتنې)
		أرجع 0;

	ټول_تېروتنې = p->ټول_numa_تېروتنې;

	إذا (!ټول_تېروتنې)
		أرجع 0;

	تېروتنې = task_تېروتنې(p, nid);
	تېروتنې += score_nearby_nodes(p, nid, dist, true);

	أرجع 1000 * تېروتنې / ټول_تېروتنې;
}

ساكن مضمن بینښه اوږد ډله_وزن(بنية task_struct *p, صحيح nid,
					 صحيح dist)
{
	بنية numa_ډله *ng = deref_task_numa_ډله(p);
	بینښه اوږد تېروتنې, ټول_تېروتنې;

	إذا (!ng)
		أرجع 0;

	ټول_تېروتنې = ng->ټول_تېروتنې;

	إذا (!ټول_تېروتنې)
		أرجع 0;

	تېروتنې = ډله_تېروتنې(p, nid);
	تېروتنې += score_nearby_nodes(p, nid, dist, false);

	أرجع 1000 * تېروتنې / ټول_تېروتنې;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
ساكن مضمن bool cpupid_valid(صحيح cpupid)
{
	أرجع cpupid_منزل_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
ساكن bool pgdat_free_space_enough(بنية pglist_معلومات *pgdat)
{
	صحيح z;
	بینښه اوږد enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> پاڼه_ښويدنه,
			   pgdat->غوټه_present_پاڼې >> 4);
	لكل (z = pgdat->nr_zones - 1; z >= 0; z--) {
		بنية zone *zone = pgdat->غوټه_zones + z;

		إذا (!populated_zone(zone))
			استمر;

		إذا (zone_watermark_ok(zone, 0,
				      promo_wmark_پاڼې(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			أرجع true;
	}
	أرجع false;
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
ساكن صحيح numa_hint_fault_latency(بنية folio *folio)
{
	صحيح وروستی_وخت, وخت;

	وخت = jiffies_منزل_msecs(jiffies);
	وروستی_وخت = folio_xchg_access_وخت(folio, وخت);

	أرجع (وخت - وروستی_وخت) & پاڼه_ACCESS_وخت_ماسک;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
ساكن bool numa_promotion_شرح_حد(بنية pglist_معلومات *pgdat,
				      بینښه اوږد شرح_حد, صحيح nr)
{
	بینښه اوږد nr_cand;
	بینښه صحيح now, پیل;

	now = jiffies_منزل_msecs(jiffies);
	mod_غوټه_پاڼه_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = غوټه_پاڼه_state(pgdat, PGPROMOTE_CANDIDATE);
	پیل = pgdat->nbp_rl_پیل;
	إذا (now - پیل > MSEC_فی_SEC &&
	    cmpxchg(&pgdat->nbp_rl_پیل, پیل, now) == پیل)
		pgdat->nbp_rl_nr_cand = nr_cand;
	إذا (nr_cand - pgdat->nbp_rl_nr_cand >= شرح_حد)
		أرجع true;
	أرجع false;
}

#تعریف_کړه NUMA_MIGRATION_ADJUST_ګامونه	16

ساكن فراغ numa_promotion_adjust_پوله(بنية pglist_معلومات *pgdat,
					    بینښه اوږد شرح_حد,
					    بینښه صحيح ref_th)
{
	بینښه صحيح now, پیل, th_period, unit_th, th;
	بینښه اوږد nr_cand, ref_cand, diff_cand;

	now = jiffies_منزل_msecs(jiffies);
	th_period = sysctl_numa_balancing_لټون_period_max;
	پیل = pgdat->nbp_th_پیل;
	إذا (now - پیل > th_period &&
	    cmpxchg(&pgdat->nbp_th_پیل, پیل, now) == پیل) {
		ref_cand = شرح_حد *
			sysctl_numa_balancing_لټون_period_max / MSEC_فی_SEC;
		nr_cand = غوټه_پاڼه_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_ګامونه;
		th = pgdat->nbp_پوله ? : ref_th;
		إذا (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		وإلا إذا (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_پوله = th;
	}
}

bool should_numa_migrate_حافظه(بنية task_struct *p, بنية folio *folio,
				صحيح src_nid, صحيح dst_cpu)
{
	بنية numa_ډله *ng = deref_curr_numa_ډله(p);
	صحيح dst_nid = cpu_منزل_غوټه(dst_cpu);
	صحيح وروستی_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	إذا (!غوټه_state(dst_nid, N_حافظه))
		أرجع false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	إذا (folio_use_access_وخت(folio)) {
		بنية pglist_معلومات *pgdat;
		بینښه اوږد شرح_حد;
		بینښه صحيح latency, th, def_th;
		اوږد nr = folio_nr_پاڼې(folio);

		pgdat = غوټه_معلومات(dst_nid);
		إذا (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_پوله = 0;
			mod_غوټه_پاڼه_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			أرجع true;
		}

		def_th = sysctl_numa_balancing_hot_پوله;
		شرح_حد = MB_منزل_پاڼې(sysctl_numa_balancing_promote_شرح_حد);
		numa_promotion_adjust_پوله(pgdat, شرح_حد, def_th);

		th = pgdat->nbp_پوله ? : def_th;
		latency = numa_hint_fault_latency(folio);
		إذا (latency >= th)
			أرجع false;

		أرجع !numa_promotion_شرح_حد(pgdat, شرح_حد, nr);
	}

	this_cpupid = cpu_pid_منزل_cpupid(dst_cpu, current->pid);
	وروستی_cpupid = folio_xchg_وروستی_cpupid(folio, this_cpupid);

	إذا (!(sysctl_numa_balancing_ډېرپېښ & NUMA_BALANCING_حافظه_TIERING) &&
	    !غوټه_is_toptier(src_nid) && !cpupid_valid(وروستی_cpupid))
		أرجع false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	إذا ((p->numa_preferred_nid == NUMA_NO_غوټه || p->numa_لټون_seq <= 4) &&
	    (cpupid_pid_unset(وروستی_cpupid) || cpupid_سمون_pid(p, وروستی_cpupid)))
		أرجع true;

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
	إذا (!cpupid_pid_unset(وروستی_cpupid) &&
				cpupid_منزل_nid(وروستی_cpupid) != dst_nid)
		أرجع false;

	/* Always allow migrate on private faults */
	إذا (cpupid_سمون_pid(p, وروستی_cpupid))
		أرجع true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	إذا (!ng)
		أرجع true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	إذا (ډله_تېروتنې_cpu(ng, dst_nid) > ډله_تېروتنې_cpu(ng, src_nid) *
					فعال_غوټه_FRACTION)
		أرجع true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	أرجع ډله_تېروتنې_cpu(ng, dst_nid) * ډله_تېروتنې(p, src_nid) * 3 >
	       ډله_تېروتنې_cpu(ng, src_nid) * ډله_تېروتنې(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
تعداد numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	غوټه_has_اضافي = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	غوټه_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	غوټه_overloaded
};

/* Cached statistics for all CPUs within a node */
بنية numa_stats {
	بینښه اوږد بارول;
	بینښه اوږد runnable;
	بینښه اوږد util;
	/* Total compute capacity of CPUs on a node */
	بینښه اوږد compute_ظرفیت;
	بینښه صحيح nr_running;
	بینښه صحيح وزن;
	تعداد numa_type غوټه_type;
	صحيح idle_cpu;
};

بنية task_numa_env {
	بنية task_struct *p;

	صحيح src_cpu, src_nid;
	صحيح dst_cpu, dst_nid;
	صحيح imb_numa_nr;

	بنية numa_stats src_stats, dst_stats;

	صحيح imbalance_pct;
	صحيح dist;

	بنية task_struct *غوره_task;
	اوږد غوره_imp;
	صحيح غوره_cpu;
};

ساكن بینښه اوږد cpu_بارول(بنية rq *rq);
ساكن بینښه اوږد cpu_runnable(بنية rq *rq);

ساكن مضمن تعداد
numa_type numa_classify(بینښه صحيح imbalance_pct,
			 بنية numa_stats *ns)
{
	إذا ((ns->nr_running > ns->وزن) &&
	    (((ns->compute_ظرفیت * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_ظرفیت * imbalance_pct) < (ns->runnable * 100))))
		أرجع غوټه_overloaded;

	إذا ((ns->nr_running < ns->وزن) ||
	    (((ns->compute_ظرفیت * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_ظرفیت * imbalance_pct) > (ns->runnable * 100))))
		أرجع غوټه_has_اضافي;

	أرجع غوټه_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
ساكن مضمن bool test_idle_cores(صحيح cpu);
ساكن مضمن صحيح numa_idle_core(صحيح idle_core, صحيح cpu)
{
	إذا (!sched_smt_فعال() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		أرجع idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	إذا (is_core_idle(cpu))
		idle_core = cpu;

	أرجع idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
ساكن فراغ update_numa_stats(بنية task_numa_env *env,
			      بنية numa_stats *ns, صحيح nid,
			      bool پیدا_کړه_idle)
{
	صحيح cpu, idle_core = -1;

	memset(ns, 0, حجم(*ns));
	ns->idle_cpu = -1;

	rcu_read_قلف();
	for_each_cpu(cpu, cpumask_د_غوټه(nid)) {
		بنية rq *rq = cpu_rq(cpu);

		ns->بارول += cpu_بارول(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_ظرفیت += ظرفیت_د(cpu);

		إذا (پیدا_کړه_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			إذا (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				استمر;

			إذا (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->وزن = cpumask_وزن(cpumask_د_غوټه(nid));

	ns->غوټه_type = numa_classify(env->imbalance_pct, ns);

	إذا (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

ساكن فراغ task_numa_ټاکل(بنية task_numa_env *env,
			     بنية task_struct *p, اوږد imp)
{
	بنية rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	إذا (env->غوره_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		صحيح cpu;
		صحيح پیل = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_د_غوټه(env->dst_nid), پیل + 1) {
			إذا (cpu == env->غوره_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				استمر;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			إذا (!xchg(&rq->numa_migrate_on, 1))
				اذهب ټاکل;
		}

		/* Failed to find an alternative idle CPU */
		أرجع;
	}

ټاکل:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	إذا (env->غوره_cpu != -1 && env->غوره_cpu != env->dst_cpu) {
		rq = cpu_rq(env->غوره_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	إذا (env->غوره_task)
		put_task_struct(env->غوره_task);
	إذا (p)
		get_task_struct(p);

	env->غوره_task = p;
	env->غوره_imp = imp;
	env->غوره_cpu = env->dst_cpu;
}

ساكن bool بارول_too_imbalanced(اوږد src_بارول, اوږد dst_بارول,
				بنية task_numa_env *env)
{
	اوږد imb, old_imb;
	اوږد orig_src_بارول, orig_dst_بارول;
	اوږد src_ظرفیت, dst_ظرفیت;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_ظرفیت = env->src_stats.compute_ظرفیت;
	dst_ظرفیت = env->dst_stats.compute_ظرفیت;

	imb = مطلق_قیمت(dst_بارول * src_ظرفیت - src_بارول * dst_ظرفیت);

	orig_src_بارول = env->src_stats.بارول;
	orig_dst_بارول = env->dst_stats.بارول;

	old_imb = مطلق_قیمت(orig_dst_بارول * src_ظرفیت - orig_src_بارول * dst_ظرفیت);

	/* Would this change make things worse? */
	أرجع (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#تعریف_کړه SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
ساكن bool task_numa_compare(بنية task_numa_env *env,
			      اوږد taskimp, اوږد groupimp, bool maymove)
{
	بنية numa_ډله *cur_ng, *p_ng = deref_curr_numa_ډله(env->p);
	بنية rq *dst_rq = cpu_rq(env->dst_cpu);
	اوږد imp = p_ng ? groupimp : taskimp;
	بنية task_struct *cur;
	اوږد src_بارول, dst_بارول;
	صحيح dist = env->dist;
	اوږد moveimp = imp;
	اوږد بارول;
	bool stopsearch = false;

	إذا (READ_ONCE(dst_rq->numa_migrate_on))
		أرجع false;

	rcu_read_قلف();
	cur = rcu_dereference_all(dst_rq->curr);
	إذا (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = عدم;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	إذا (cur == env->p) {
		stopsearch = true;
		اذهب unlock;
	}

	إذا (!cur) {
		إذا (maymove && moveimp >= env->غوره_imp)
			اذهب ټاکل;
		وإلا
			اذهب unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	إذا (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		اذهب unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	إذا (env->غوره_task &&
	    env->غوره_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		اذهب unlock;
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
	cur_ng = rcu_dereference_all(cur->numa_ډله);
	إذا (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		إذا (env->dst_stats.غوټه_type == غوټه_has_اضافي)
			اذهب unlock;

		imp = taskimp + task_وزن(cur, env->src_nid, dist) -
		      task_وزن(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		إذا (cur_ng)
			imp -= imp / 16;
	} وإلا {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		إذا (cur_ng && p_ng)
			imp += ډله_وزن(cur, env->src_nid, dist) -
			       ډله_وزن(cur, env->dst_nid, dist);
		وإلا
			imp += task_وزن(cur, env->src_nid, dist) -
			       task_وزن(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	إذا (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	إذا (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	إذا (maymove && moveimp > imp && moveimp > env->غوره_imp) {
		imp = moveimp;
		cur = عدم;
		اذهب ټاکل;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	إذا (env->غوره_task && cur->numa_preferred_nid == env->src_nid &&
	    env->غوره_task->numa_preferred_nid != env->src_nid) {
		اذهب ټاکل;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	إذا (imp < SMALLIMP || imp <= env->غوره_imp + SMALLIMP / 2)
		اذهب unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	بارول = task_h_بارول(env->p) - task_h_بارول(cur);
	إذا (!بارول)
		اذهب ټاکل;

	dst_بارول = env->dst_stats.بارول + بارول;
	src_بارول = env->src_stats.بارول - بارول;

	إذا (بارول_too_imbalanced(src_بارول, dst_بارول, env))
		اذهب unlock;

ټاکل:
	/* Evaluate an idle CPU for a task numa move. */
	إذا (!cur) {
		صحيح cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		إذا (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		إذا (!idle_cpu(cpu) && env->غوره_cpu >= 0 &&
		    idle_cpu(env->غوره_cpu)) {
			cpu = env->غوره_cpu;
		}

		env->dst_cpu = cpu;
	}

	task_numa_ټاکل(env, cur, imp);

	/*
	 * If a move to idle is allowed because there is capacity or load
	 * balance improves then stop the search. While a better swap
	 * candidate may exist, a search is not free.
	 */
	إذا (maymove && !cur && env->غوره_cpu >= 0 && idle_cpu(env->غوره_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	إذا (!maymove && env->غوره_task &&
	    env->غوره_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	أرجع stopsearch;
}

ساكن فراغ task_numa_پیدا_کړه_cpu(بنية task_numa_env *env,
				اوږد taskimp, اوږد groupimp)
{
	bool maymove = false;
	صحيح cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	إذا (env->dst_stats.غوټه_type == غوټه_has_اضافي) {
		بینښه صحيح imbalance;
		صحيح src_running, dst_running;

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
		إذا (!imbalance) {
			maymove = true;
			إذا (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_ټاکل(env, عدم, 0);
				أرجع;
			}
		}
	} وإلا {
		اوږد src_بارول, dst_بارول, بارول;
		/*
		 * If the improvement from just moving env->p direction is better
		 * than swapping tasks around, check if a move is possible.
		 */
		بارول = task_h_بارول(env->p);
		dst_بارول = env->dst_stats.بارول + بارول;
		src_بارول = env->src_stats.بارول - بارول;
		maymove = !بارول_too_imbalanced(src_بارول, dst_بارول, env);
	}

	/* Skip CPUs if the source task cannot migrate */
	for_each_cpu_and(cpu, cpumask_د_غوټه(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		إذا (task_numa_compare(env, taskimp, groupimp, maymove))
			اكسر;
	}
}

ساكن صحيح task_numa_migrate(بنية task_struct *p)
{
	بنية task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_غوټه(p),

		.imbalance_pct = 112,

		.غوره_task = عدم,
		.غوره_imp = 0,
		.غوره_cpu = -1,
	};
	بینښه اوږد taskweight, groupweight;
	بنية sched_domain *sd;
	اوږد taskimp, groupimp;
	بنية numa_ډله *ng;
	بنية rq *غوره_rq;
	صحيح nid, ret, dist;

	/*
	 * Pick the lowest SD_NUMA domain, as that would have the smallest
	 * imbalance and would be the first to start moving tasks about.
	 *
	 * And we want to avoid any moving of tasks about, as that would create
	 * random movement of tasks -- counter the numa conditions we're trying
	 * to satisfy here.
	 */
	rcu_read_قلف();
	sd = rcu_dereference_all(فی_cpu(sd_numa, env.src_cpu));
	إذا (sd) {
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
	إذا (unlikely(!sd)) {
		sched_setnuma(p, task_غوټه(p));
		أرجع -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = غوټه_واټن(env.src_nid, env.dst_nid);
	taskweight = task_وزن(p, env.src_nid, dist);
	groupweight = ډله_وزن(p, env.src_nid, dist);
	update_numa_stats(&env, &env.src_stats, env.src_nid, false);
	taskimp = task_وزن(p, env.dst_nid, dist) - taskweight;
	groupimp = ډله_وزن(p, env.dst_nid, dist) - groupweight;
	update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);

	/* Try to find a spot on the preferred nid. */
	task_numa_پیدا_کړه_cpu(&env, taskimp, groupimp);

	/*
	 * Look at other nodes in these cases:
	 * - there is no space available on the preferred_nid
	 * - the task is part of a numa_group that is interleaved across
	 *   multiple NUMA nodes; in order to better consolidate the group,
	 *   we need to check other locations.
	 */
	ng = deref_curr_numa_ډله(p);
	إذا (env.غوره_cpu == -1 || (ng && ng->فعال_nodes > 1)) {
		for_each_غوټه_state(nid, N_CPU) {
			إذا (nid == env.src_nid || nid == p->numa_preferred_nid)
				استمر;

			dist = غوټه_واټن(env.src_nid, env.dst_nid);
			إذا (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_وزن(p, env.src_nid, dist);
				groupweight = ډله_وزن(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_وزن(p, nid, dist) - taskweight;
			groupimp = ډله_وزن(p, nid, dist) - groupweight;
			إذا (taskimp < 0 && groupimp < 0)
				استمر;

			env.dist = dist;
			env.dst_nid = nid;
			update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);
			task_numa_پیدا_کړه_cpu(&env, taskimp, groupimp);
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
	إذا (ng) {
		إذا (env.غوره_cpu == -1)
			nid = env.src_nid;
		وإلا
			nid = cpu_منزل_غوټه(env.غوره_cpu);

		إذا (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	إذا (env.غوره_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, عدم, -1);
		أرجع -EAGAIN;
	}

	غوره_rq = cpu_rq(env.غوره_cpu);
	إذا (env.غوره_task == عدم) {
		ret = migrate_task_منزل(p, env.غوره_cpu);
		WRITE_ONCE(غوره_rq->numa_migrate_on, 0);
		إذا (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, عدم, env.غوره_cpu);
		أرجع ret;
	}

	ret = migrate_بدلون(p, env.غوره_task, env.غوره_cpu, env.src_cpu);
	WRITE_ONCE(غوره_rq->numa_migrate_on, 0);

	إذا (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.غوره_task, env.غوره_cpu);
	put_task_struct(env.غوره_task);
	أرجع ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
ساكن فراغ numa_migrate_preferred(بنية task_struct *p)
{
	بینښه اوږد interval = HZ;

	/* This task has no NUMA fault statistics yet */
	إذا (unlikely(p->numa_preferred_nid == NUMA_NO_غوټه || !p->numa_تېروتنې))
		أرجع;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_منزل_jiffies(p->numa_لټون_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	إذا (task_غوټه(p) == p->numa_preferred_nid)
		أرجع;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
ساكن فراغ numa_ډله_شمېر_فعال_nodes(بنية numa_ډله *numa_ډله)
{
	بینښه اوږد تېروتنې, max_تېروتنې = 0;
	صحيح nid, فعال_nodes = 0;

	for_each_غوټه_state(nid, N_CPU) {
		تېروتنې = ډله_تېروتنې_cpu(numa_ډله, nid);
		إذا (تېروتنې > max_تېروتنې)
			max_تېروتنې = تېروتنې;
	}

	for_each_غوټه_state(nid, N_CPU) {
		تېروتنې = ډله_تېروتنې_cpu(numa_ډله, nid);
		إذا (تېروتنې * فعال_غوټه_FRACTION > max_تېروتنې)
			فعال_nodes++;
	}

	numa_ډله->max_تېروتنې_cpu = max_تېروتنې;
	numa_ډله->فعال_nodes = فعال_nodes;
}

/*
 * When adapting the scan rate, the period is divided into NUMA_PERIOD_SLOTS
 * increments. The more local the fault statistics are, the higher the scan
 * period will be for the next scan window. If local/(local+remote) ratio is
 * below NUMA_PERIOD_THRESHOLD (where range of ratio is 1..NUMA_PERIOD_SLOTS)
 * the scan period will decrease. Aim for 70% local accesses.
 */
#تعریف_کړه NUMA_PERIOD_SLOTS 10
#تعریف_کړه NUMA_PERIOD_پوله 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
ساكن فراغ update_task_لټون_period(بنية task_struct *p,
			بینښه اوږد shared, بینښه اوږد خاص)
{
	بینښه صحيح period_خونه;
	صحيح lr_نسبت, ps_نسبت;
	صحيح diff;

	بینښه اوږد remote = p->numa_تېروتنې_locality[0];
	بینښه اوږد local = p->numa_تېروتنې_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	إذا (local + shared == 0 || p->numa_تېروتنې_locality[2]) {
		p->numa_لټون_period = min(p->numa_لټون_period_max,
			p->numa_لټون_period << 1);

		p->mm->numa_بل_لټون = jiffies +
			msecs_منزل_jiffies(p->numa_لټون_period);

		اذهب بهر;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_خونه = DIV_پړاو_پورته(p->numa_لټون_period, NUMA_PERIOD_SLOTS);
	lr_نسبت = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_نسبت = (خاص * NUMA_PERIOD_SLOTS) / (خاص + shared);

	إذا (ps_نسبت >= NUMA_PERIOD_پوله) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		صحيح خونه = ps_نسبت - NUMA_PERIOD_پوله;
		إذا (!خونه)
			خونه = 1;
		diff = خونه * period_خونه;
	} وإلا إذا (lr_نسبت >= NUMA_PERIOD_پوله) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		صحيح خونه = lr_نسبت - NUMA_PERIOD_پوله;
		إذا (!خونه)
			خونه = 1;
		diff = خونه * period_خونه;
	} وإلا {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		صحيح نسبت = max(lr_نسبت, ps_نسبت);
		diff = -(NUMA_PERIOD_پوله - نسبت) * period_خونه;
	}

	p->numa_لټون_period = clamp(p->numa_لټون_period + diff,
			task_لټون_min(p), task_لټون_max(p));

بهر:
	memset(p->numa_تېروتنې_locality, 0,
	       حجم(p->numa_تېروتنې_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
ساكن u64 numa_get_avg_runtime(بنية task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_پیل;
	runtime = p->se.مجموعه_exec_runtime;

	إذا (p->وروستی_task_numa_placement) {
		delta = runtime - p->وروستی_مجموعه_exec_runtime;
		*period = now - p->وروستی_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		إذا (unlikely((s64)*period < 0))
			*period = 0;
	} وإلا {
		delta = p->se.avg.بارول_مجموعه;
		*period = بارول_AVG_MAX;
	}

	p->وروستی_مجموعه_exec_runtime = runtime;
	p->وروستی_task_numa_placement = now;

	أرجع delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
ساكن صحيح preferred_ډله_nid(بنية task_struct *p, صحيح nid)
{
	nodemask_t nodes;
	صحيح dist;

	/* Direct connections between all NUMA nodes. */
	إذا (sched_numa_topology_type == NUMA_مستقیم)
		أرجع nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	إذا (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		بینښه اوږد score, max_score = 0;
		صحيح غوټه, max_غوټه = nid;

		dist = sched_max_numa_واټن;

		for_each_غوټه_state(غوټه, N_CPU) {
			score = ډله_وزن(p, غوټه, dist);
			إذا (score > max_score) {
				max_score = score;
				max_غوټه = غوټه;
			}
		}
		أرجع max_غوټه;
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
	nodes = غوټه_حالتونه[N_CPU];
	لكل (dist = sched_max_numa_واټن; dist > LOCAL_واټن; dist--) {
		بینښه اوږد max_تېروتنې = 0;
		nodemask_t max_ډله = غوټه_ماسک_NONE;
		صحيح a, b;

		/* Are there nodes at this distance from each other? */
		إذا (!پیدا_کړه_numa_واټن(dist))
			استمر;

		for_each_غوټه_ماسک(a, nodes) {
			بینښه اوږد تېروتنې = 0;
			nodemask_t this_ډله;
			nodes_clear(this_ډله);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_غوټه_ماسک(b, nodes) {
				إذا (غوټه_واټن(a, b) < dist) {
					تېروتنې += ډله_تېروتنې(p, b);
					غوټه_سېټ(b, this_ډله);
					غوټه_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			إذا (تېروتنې > max_تېروتنې) {
				max_تېروتنې = تېروتنې;
				max_ډله = this_ډله;
				/*
				 * subtle: at the smallest distance there is
				 * just one node left in each "group", the
				 * winner is the preferred nid.
				 */
				nid = a;
			}
		}
		/* Next round, evaluate the nodes within max_group. */
		إذا (!max_تېروتنې)
			اكسر;
		nodes = max_ډله;
	}
	أرجع nid;
}

ساكن فراغ task_numa_placement(بنية task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	بنية sched_cache_ډله __maybe_unused *grp;
	صحيح seq, nid, max_nid = NUMA_NO_غوټه;
	بینښه اوږد max_تېروتنې = 0;
	بینښه اوږد fault_types[2] = { 0, 0 };
	بینښه اوږد ټول_تېروتنې;
	u64 runtime, period;
	spinlock_t *ډله_قلف = عدم;
	اوږد __maybe_unused new_fp;
	بنية numa_ډله *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_لټون_seq);
	إذا (p->numa_لټون_seq == seq)
		أرجع;
	p->numa_لټون_seq = seq;
	p->numa_لټون_period_max = task_لټون_max(p);

	ټول_تېروتنې = p->numa_تېروتنې_locality[0] +
		       p->numa_تېروتنې_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_ډله(p);
	إذا (ng) {
		ډله_قلف = &ng->قلف;
		spin_قلف_irq(ډله_قلف);
	}

	/* Find the node with the highest number of faults */
	for_each_online_غوټه(nid) {
		/* Keep track of the offsets in numa_faults array */
		صحيح mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		بینښه اوږد تېروتنې = 0, ډله_تېروتنې = 0;
		صحيح priv;

		لكل (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			اوږد diff, f_diff, f_وزن;

			mem_idx = task_تېروتنې_idx(NUMA_MEM, nid, priv);
			membuf_idx = task_تېروتنې_idx(NUMA_MEMBUF, nid, priv);
			cpu_idx = task_تېروتنې_idx(NUMA_CPU, nid, priv);
			cpubuf_idx = task_تېروتنې_idx(NUMA_CPUBUF, nid, priv);

			/* Decay existing window, copy faults since last scan */
			diff = p->numa_تېروتنې[membuf_idx] - p->numa_تېروتنې[mem_idx] / 2;
			fault_types[priv] += p->numa_تېروتنې[membuf_idx];
			p->numa_تېروتنې[membuf_idx] = 0;

			/*
			 * Normalize the faults_from, so all tasks in a group
			 * count according to CPU use, instead of by the raw
			 * number of faults. Tasks with little runtime have
			 * little over-all impact on throughput, and thus their
			 * faults are less important.
			 */
			f_وزن = div64_u64(runtime << 16, period + 1);
			f_وزن = (f_وزن * p->numa_تېروتنې[cpubuf_idx]) /
				   (ټول_تېروتنې + 1);
			f_diff = f_وزن - p->numa_تېروتنې[cpu_idx] / 2;
			p->numa_تېروتنې[cpubuf_idx] = 0;

			p->numa_تېروتنې[mem_idx] += diff;
			p->numa_تېروتنې[cpu_idx] += f_diff;
			تېروتنې += p->numa_تېروتنې[mem_idx];
			p->ټول_numa_تېروتنې += diff;
			إذا (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->تېروتنې[mem_idx] += diff;
				ng->تېروتنې[cpu_idx] += f_diff;
				ng->ټول_تېروتنې += diff;
				ډله_تېروتنې += ng->تېروتنې[mem_idx];
			}
#که_تعریف_شوی CONFIG_SCHED_CACHE
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

				إذا (grp) {
					new_fp = (اوږد)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#د_که_پای
		}

		إذا (!ng) {
			إذا (تېروتنې > max_تېروتنې) {
				max_تېروتنې = تېروتنې;
				max_nid = nid;
			}
		} وإلا إذا (ډله_تېروتنې > max_تېروتنې) {
			max_تېروتنې = ډله_تېروتنې;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_تر_ټولو_نږدې_غوټه(max_nid, N_CPU);

	إذا (ng) {
		numa_ډله_شمېر_فعال_nodes(ng);
		spin_unlock_irq(ډله_قلف);
		max_nid = preferred_ډله_nid(p, max_nid);
	}

	إذا (max_تېروتنې) {
		/* Set the new preferred node */
		إذا (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_لټون_period(p, fault_types[0], fault_types[1]);
}

ساكن مضمن صحيح get_numa_ډله(بنية numa_ډله *grp)
{
	أرجع refcount_inc_not_zero(&grp->refcount);
}

ساكن مضمن فراغ put_numa_ډله(بنية numa_ډله *grp)
{
	إذا (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

ساكن فراغ task_numa_ډله(بنية task_struct *p, صحيح cpupid, صحيح flags,
			صحيح *priv)
{
	بنية numa_ډله *grp, *my_grp;
	بنية task_struct *tsk;
	bool join = false;
	صحيح cpu = cpupid_منزل_cpu(cpupid);
	صحيح i;

	إذا (unlikely(!deref_curr_numa_ډله(p))) {
		بینښه صحيح اندازه = حجم(بنية numa_ډله) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_غوټه_ids * حجم(بینښه اوږد);

		grp = kzalloc(اندازه, GFP_KERNEL | __GFP_NOWARN);
		إذا (!grp)
			أرجع;

		refcount_سېټ(&grp->refcount, 1);
		grp->فعال_nodes = 1;
		grp->max_تېروتنې_cpu = 0;
		spin_قلف_init(&grp->قلف);
		grp->gid = p->pid;

		لكل (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_غوټه_ids; i++)
			grp->تېروتنې[i] = p->numa_تېروتنې[i];

		grp->ټول_تېروتنې = p->ټول_numa_تېروتنې;

		grp->nr_tasks++;
		rcu_ټاکل_اشاره_ګر(p->numa_ډله, grp);
	}

	rcu_read_قلف();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	إذا (!cpupid_سمون_pid(tsk, cpupid))
		اذهب no_join;

	grp = rcu_dereference_all(tsk->numa_ډله);
	إذا (!grp)
		اذهب no_join;

	my_grp = deref_curr_numa_ډله(p);
	إذا (grp == my_grp)
		اذهب no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	إذا (my_grp->nr_tasks > grp->nr_tasks)
		اذهب no_join;

	/*
	 * Tie-break on the grp address.
	 */
	إذا (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		اذهب no_join;

	/* Always join threads in the same process. */
	إذا (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	إذا (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	إذا (join && !get_numa_ډله(grp))
		اذهب no_join;

	rcu_read_unlock();

	إذا (!join)
		أرجع;

	WARN_ON_ONCE(irqs_disabled());
	double_قلف_irq(&my_grp->قلف, &grp->قلف);

	لكل (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_غوټه_ids; i++) {
		my_grp->تېروتنې[i] -= p->numa_تېروتنې[i];
		grp->تېروتنې[i] += p->numa_تېروتنې[i];
	}
	my_grp->ټول_تېروتنې -= p->ټول_numa_تېروتنې;
	grp->ټول_تېروتنې += p->ټول_numa_تېروتنې;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->قلف);
	spin_unlock_irq(&grp->قلف);

	rcu_ټاکل_اشاره_ګر(p->numa_ډله, grp);

	put_numa_ډله(my_grp);
	أرجع;

no_join:
	rcu_read_unlock();
	أرجع;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
فراغ task_numa_free(بنية task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	بنية numa_ډله *grp = rcu_dereference_raw(p->numa_ډله);
	بینښه اوږد *numa_تېروتنې = p->numa_تېروتنې;
	بینښه اوږد flags;
	صحيح i;

	إذا (!numa_تېروتنې)
		أرجع;

	إذا (grp) {
		spin_قلف_irqsave(&grp->قلف, flags);
		لكل (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_غوټه_ids; i++)
			grp->تېروتنې[i] -= p->numa_تېروتنې[i];
		grp->ټول_تېروتنې -= p->ټول_numa_تېروتنې;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->قلف, flags);
		RCU_INIT_اشاره_ګر(p->numa_ډله, عدم);
		put_numa_ډله(grp);
	}

	إذا (final) {
		p->numa_تېروتنې = عدم;
		kfree(numa_تېروتنې);
	} وإلا {
		p->ټول_numa_تېروتنې = 0;
		لكل (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_غوټه_ids; i++)
			numa_تېروتنې[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
فراغ task_numa_fault(صحيح وروستی_cpupid, صحيح mem_غوټه, صحيح پاڼې, صحيح flags)
{
	بنية task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	صحيح cpu_غوټه = task_غوټه(current);
	صحيح local = !!(flags & TNF_FAULT_LOCAL);
	بنية numa_ډله *ng;
	صحيح priv;

	إذا (!static_branch_likely(&sched_numa_balancing))
		أرجع;

	/* for example, ksmd faulting in a user's mm */
	إذا (!p->mm)
		أرجع;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	إذا (!غوټه_is_toptier(mem_غوټه) &&
	    (sysctl_numa_balancing_ډېرپېښ & NUMA_BALANCING_حافظه_TIERING ||
	     !cpupid_valid(وروستی_cpupid)))
		أرجع;

	/* Allocate buffer to track faults on a per-node basis */
	إذا (unlikely(!p->numa_تېروتنې)) {
		صحيح اندازه = حجم(*p->numa_تېروتنې) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_غوټه_ids;

		p->numa_تېروتنې = kzalloc(اندازه, GFP_KERNEL|__GFP_NOWARN);
		إذا (!p->numa_تېروتنې)
			أرجع;

		p->ټول_numa_تېروتنې = 0;
		memset(p->numa_تېروتنې_locality, 0, حجم(p->numa_تېروتنې_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	إذا (unlikely(وروستی_cpupid == (-1 & وروستی_CPUPID_ماسک))) {
		priv = 1;
	} وإلا {
		priv = cpupid_سمون_pid(p, وروستی_cpupid);
		إذا (!priv && !(flags & TNF_NO_ډله))
			task_numa_ډله(p, وروستی_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_ډله(p);
	إذا (!priv && !local && ng && ng->فعال_nodes > 1 &&
				numa_is_فعال_غوټه(cpu_غوټه, ng) &&
				numa_is_فعال_غوټه(mem_غوټه, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	إذا (وخت_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	إذا (migrated)
		p->numa_پاڼې_migrated += پاڼې;
	إذا (flags & TNF_MIGRATE_FAIL)
		p->numa_تېروتنې_locality[2] += پاڼې;

	p->numa_تېروتنې[task_تېروتنې_idx(NUMA_MEMBUF, mem_غوټه, priv)] += پاڼې;
	p->numa_تېروتنې[task_تېروتنې_idx(NUMA_CPUBUF, cpu_غوټه, priv)] += پاڼې;
	p->numa_تېروتنې_locality[local] += پاڼې;
}

ساكن فراغ reset_ptenuma_لټون(بنية task_struct *p)
{
	/*
	 * We only did a read acquisition of the mmap sem, so
	 * p->mm->numa_scan_seq is written to without exclusive access
	 * and the update is not guaranteed to be atomic. That's not
	 * much of an issue though, since this is just used for
	 * statistical sampling. Use READ_ONCE/WRITE_ONCE, which are not
	 * expensive, to avoid any form of compiler optimizations:
	 */
	WRITE_ONCE(p->mm->numa_لټون_seq, READ_ONCE(p->mm->numa_لټون_seq) + 1);
	p->mm->numa_لټون_دننه_واټن = 0;
}

ساكن bool vma_is_accessed(بنية mm_struct *mm, بنية vm_area_struct *vma)
{
	بینښه اوږد pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	إذا ((READ_ONCE(current->mm->numa_لټون_seq) - vma->numab_state->پیل_لټون_seq) < 2)
		أرجع true;

	pids = vma->numab_state->pids_فعال[0] | vma->numab_state->pids_فعال[1];
	إذا (test_بټ(hash_32(current->pid, ilog2(بټونه_فی_LONG)), &pids))
		أرجع true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	إذا (mm->numa_لټون_دننه_واټن > vma->vm_پیل) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		أرجع true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	إذا (READ_ONCE(mm->numa_لټون_seq) >
	   (vma->numab_state->prev_لټون_seq + get_nr_تارونه(current)))
		أرجع true;

	أرجع false;
}

#تعریف_کړه VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_لټون_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
ساكن فراغ task_numa_work(بنية callback_سر *work)
{
	بینښه اوږد migrate, بل_لټون, now = jiffies;
	بنية task_struct *p = current;
	بنية mm_struct *mm = p->mm;
	u64 runtime = p->se.مجموعه_exec_runtime;
	بنية vm_area_struct *vma;
	بینښه اوږد پیل, پای_ټکی;
	بینښه اوږد nr_pte_updates = 0;
	اوږد پاڼې, virtpages;
	بنية vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_د(work, بنية task_struct, numa_work));

	work->بل = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	إذا (p->flags & PF_EXITING)
		أرجع;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	إذا (cpusets_enabled() && nodes_وزن(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		أرجع;
	}

	إذا (!mm->numa_بل_لټون) {
		mm->numa_بل_لټون = now +
			msecs_منزل_jiffies(sysctl_numa_balancing_لټون_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_بل_لټون;
	إذا (وخت_before(now, migrate))
		أرجع;

	إذا (p->numa_لټون_period == 0) {
		p->numa_لټون_period_max = task_لټون_max(p);
		p->numa_لټون_period = task_لټون_پیل(p);
	}

	بل_لټون = now + msecs_منزل_jiffies(p->numa_لټون_period);
	إذا (!try_cmpxchg(&mm->numa_بل_لټون, &migrate, بل_لټون))
		أرجع;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->غوټه_stamp += 2 * TICK_NSEC;

	پاڼې = sysctl_numa_balancing_لټون_اندازه;
	پاڼې <<= 20 - پاڼه_ښويدنه; /* MB in pages */
	virtpages = پاڼې * 8;	   /* Scan up to this much virtual space */
	إذا (!پاڼې)
		أرجع;


	إذا (!mmap_read_trylock(mm))
		أرجع;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	پیل = mm->numa_لټون_دننه_واټن;
	vma_iter_init(&vmi, mm, پیل);
	vma = vma_بل(&vmi);
	إذا (!vma) {
		reset_ptenuma_لټون(p);
		پیل = 0;
		vma_iter_سېټ(&vmi, پیل);
		vma = vma_بل(&vmi);
	}

	لكل (; vma; vma = vma_بل(&vmi)) {
		إذا (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_پاڼه(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			استمر;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		إذا (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			استمر;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		إذا (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			استمر;
		}

		/* Initialise new per-VMA NUMAB state. */
		إذا (!vma->numab_state) {
			بنية vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			إذا (!ptr)
				استمر;

			إذا (cmpxchg(&vma->numab_state, عدم, ptr)) {
				kfree(ptr);
				استمر;
			}

			vma->numab_state->پیل_لټون_seq = mm->numa_لټون_seq;

			vma->numab_state->بل_لټون = now +
				msecs_منزل_jiffies(sysctl_numa_balancing_لټون_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_فعال_reset =  vma->numab_state->بل_لټون +
				msecs_منزل_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->prev_لټون_seq = mm->numa_لټون_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		إذا (mm->numa_لټون_seq && وخت_before(jiffies,
						vma->numab_state->بل_لټون)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_لټون_DELAY);
			استمر;
		}

		/* RESET access PIDs regularly for old VMAs. */
		إذا (mm->numa_لټون_seq &&
				وخت_after(jiffies, vma->numab_state->pids_فعال_reset)) {
			vma->numab_state->pids_فعال_reset = vma->numab_state->pids_فعال_reset +
				msecs_منزل_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_فعال[0] = READ_ONCE(vma->numab_state->pids_فعال[1]);
			vma->numab_state->pids_فعال[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		إذا (vma->numab_state->prev_لټون_seq == mm->numa_لټون_seq) {
			mm->numa_لټون_دننه_واټن = vma->vm_پای_ټکی;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			استمر;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		إذا (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			استمر;
		}

		افعل {
			پیل = max(پیل, vma->vm_پیل);
			پای_ټکی = ALIGN(پیل + (پاڼې << پاڼه_ښويدنه), HPAGE_اندازه);
			پای_ټکی = min(پای_ټکی, vma->vm_پای_ټکی);
			nr_pte_updates = change_prot_numa(vma, پیل, پای_ټکی);

			/*
			 * Try to scan sysctl_numa_balancing_size worth of
			 * hpages that have at least one present PTE that
			 * is not already PTE-numa. If the VMA contains
			 * areas that are unused or already full of prot_numa
			 * PTEs, scan up to virtpages, to skip through those
			 * areas faster.
			 */
			إذا (nr_pte_updates)
				پاڼې -= (پای_ټکی - پیل) >> پاڼه_ښويدنه;
			virtpages -= (پای_ټکی - پیل) >> پاڼه_ښويدنه;

			پیل = پای_ټکی;
			إذا (پاڼې <= 0 || virtpages <= 0)
				اذهب بهر;

			cond_resched();
		} طالما (پای_ټکی != vma->vm_پای_ټکی);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->prev_لټون_seq = mm->numa_لټون_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		إذا (vma_pids_forced)
			اكسر;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	إذا (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		اذهب retry_pids;
	}

بهر:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	إذا (vma)
		mm->numa_لټون_دننه_واټن = پیل;
	وإلا
		reset_ptenuma_لټون(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	إذا (unlikely(p->se.مجموعه_exec_runtime != runtime)) {
		u64 diff = p->se.مجموعه_exec_runtime - runtime;
		p->غوټه_stamp += 32 * diff;
	}
}

فراغ init_numa_balancing(u64 clone_flags, بنية task_struct *p)
{
	صحيح mm_users = 0;
	بنية mm_struct *mm = p->mm;

	إذا (mm) {
		mm_users = atomic_read(&mm->mm_users);
		إذا (mm_users == 1) {
			mm->numa_بل_لټون = jiffies + msecs_منزل_jiffies(sysctl_numa_balancing_لټون_delay);
			mm->numa_لټون_seq = 0;
		}
	}
	p->غوټه_stamp			= 0;
	p->numa_لټون_seq		= mm ? mm->numa_لټون_seq : 0;
	p->numa_لټون_period		= sysctl_numa_balancing_لټون_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.بل		= &p->numa_work;
	p->numa_تېروتنې			= عدم;
	p->numa_پاڼې_migrated		= 0;
	p->ټول_numa_تېروتنې		= 0;
	RCU_INIT_اشاره_ګر(p->numa_ډله, عدم);
	p->وروستی_task_numa_placement	= 0;
	p->وروستی_مجموعه_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	إذا (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_غوټه;
		أرجع;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	إذا (mm) {
		بینښه صحيح delay;

		delay = min_t(بینښه صحيح, task_لټون_max(current),
			current->numa_لټون_period * mm_users * NSEC_فی_MSEC);
		delay += 2 * TICK_NSEC;
		p->غوټه_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
ساكن فراغ task_tick_numa(بنية rq *rq, بنية task_struct *curr)
{
	بنية callback_سر *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	إذا (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->بل != work)
		أرجع;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.مجموعه_exec_runtime;
	period = (u64)curr->numa_لټون_period * NSEC_فی_MSEC;

	إذا (now > curr->غوټه_stamp + period) {
		إذا (!curr->غوټه_stamp)
			curr->numa_لټون_period = task_لټون_پیل(curr);
		curr->غوټه_stamp += period;

		إذا (!وخت_before(jiffies, curr->mm->numa_بل_لټون))
			task_work_جمع_کړه(curr, work, TWA_RESUME);
	}
}

ساكن فراغ update_لټون_period(بنية task_struct *p, صحيح new_cpu)
{
	صحيح src_nid = cpu_منزل_غوټه(task_cpu(p));
	صحيح dst_nid = cpu_منزل_غوټه(new_cpu);

	إذا (!static_branch_likely(&sched_numa_balancing))
		أرجع;

	إذا (!p->mm || !p->numa_تېروتنې || (p->flags & PF_EXITING))
		أرجع;

	إذا (src_nid == dst_nid)
		أرجع;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	إذا (p->numa_لټون_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		إذا (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_غوټه &&
			src_nid != p->numa_preferred_nid))
			أرجع;
	}

	p->numa_لټون_period = task_لټون_پیل(p);
}

#وإلا /* !CONFIG_NUMA_BALANCING: */

ساكن فراغ task_tick_numa(بنية rq *rq, بنية task_struct *curr)
{
}

ساكن مضمن فراغ account_numa_کتار_ته_ورزیاتول(بنية rq *rq, بنية task_struct *p)
{
}

ساكن مضمن فراغ account_numa_له_کتار_ایستل(بنية rq *rq, بنية task_struct *p)
{
}

ساكن مضمن فراغ update_لټون_period(بنية task_struct *p, صحيح new_cpu)
{
}

#د_که_پای /* !CONFIG_NUMA_BALANCING */

ساكن فراغ
account_entity_کتار_ته_ورزیاتول(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_د(se));
	update_بارول_جمع_کړه(&cfs_rq->بارول, se->بارول.وزن);
	إذا (entity_is_task(se)) {
		بنية task_struct *p = task_د(se);
		بنية rq *rq = rq_د(cfs_rq);

		account_numa_کتار_ته_ورزیاتول(rq, p);
		account_llc_کتار_ته_ورزیاتول(rq, p);
		list_جمع_کړه(&se->ډله_غوټه, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

ساكن فراغ
account_entity_له_کتار_ایستل(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_د(se));
	update_بارول_sub(&cfs_rq->بارول, se->بارول.وزن);
	إذا (entity_is_task(se)) {
		بنية task_struct *p = task_د(se);
		بنية rq *rq = rq_د(cfs_rq);

		account_numa_له_کتار_ایستل(rq, p);
		account_llc_له_کتار_ایستل(rq, p);
		list_del_init(&se->ډله_غوټه);
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
#تعریف_کړه جمع_کړه_مثبت(_ptr, _val) افعل {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	إذا (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} طالما (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#تعریف_کړه lsub_مثبت(_ptr, _val) افعل {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} طالما (0)


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
#تعریف_کړه __update_sa(sa, name, delta_avg, delta_مجموعه) افعل {	\
	جمع_کړه_مثبت(&(sa)->name##_avg, delta_avg);		\
	جمع_کړه_مثبت(&(sa)->name##_sum, delta_مجموعه);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} طالما (0)

ساكن مضمن فراغ
کتار_ته_ورزیاتول_بارول_avg(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	__update_sa(&cfs_rq->avg, بارول, se->avg.بارول_avg,
		    se_وزن(se) * se->avg.بارول_مجموعه);
}

ساكن مضمن فراغ
له_کتار_ایستل_بارول_avg(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	__update_sa(&cfs_rq->avg, بارول, -se->avg.بارول_avg,
		    se_وزن(se) * -se->avg.بارول_مجموعه);
}

ساكن فراغ
rescale_entity(بنية sched_entity *se, بینښه اوږد وزن, bool rel_vprot)
{
	اوږد old_وزن = se->h_بارول.وزن;

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
	إذا (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_وزن, وزن);

	إذا (rel_vprot)
		se->vprot = div64_long(se->vprot * old_وزن, وزن);
}

ساكن فراغ reweight_eevdf(بنية cfs_rq *cfs_rq, بنية sched_entity *se,
			   بینښه اوږد وزن, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	إذا (se->h_بارول.وزن == وزن)
		أرجع;

	إذا (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		إذا (curr && protect_ټوټه(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		إذا (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, وزن, rel_vprot);

	update_بارول_سېټ(&se->h_بارول, وزن);

	إذا (on_rq) {
		إذا (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		إذا (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

ساكن فراغ reweight_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se,
			    بینښه اوږد وزن)
{
	إذا (se->بارول.وزن == وزن)
		أرجع;

	إذا (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_د(se));
		update_بارول_sub(&cfs_rq->بارول, se->بارول.وزن);
	}
	له_کتار_ایستل_بارول_avg(cfs_rq, se);

	update_بارول_سېټ(&se->بارول, وزن);

	افعل {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.بارول_avg = div_u64(se_وزن(se) * se->avg.بارول_مجموعه, divider);
	} طالما (0);

	کتار_ته_ورزیاتول_بارول_avg(cfs_rq, se);

	إذا (se->on_rq)
		update_بارول_جمع_کړه(&cfs_rq->بارول, se->بارول.وزن);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
ساكن __always_inline
بینښه اوږد __calc_prop_weight(بنية cfs_rq *cfs_rq, بنية sched_entity *se,
				 بینښه اوږد وزن)
{
	وزن *= se->بارول.وزن;
	إذا (پلار_entity(se))
		وزن /= cfs_rq->بارول.وزن;
	وإلا
		وزن /= NICE_0_بارول;

	أرجع max(وزن, MIN_SHARES);
}

ساكن فراغ reweight_task_fair(بنية rq *rq, بنية task_struct *p,
			       ثابت بنية بارول_وزن *lw)
{
	بنية sched_entity *se = &p->se;
	بینښه اوږد وزن = NICE_0_بارول;

	إذا (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_د(se), se, lw->وزن);
	se->بارول.inv_وزن = lw->inv_وزن;

	إذا (!se->on_rq)
		أرجع;

	for_each_sched_entity(se)
		وزن = __calc_prop_weight(cfs_rq_د(se), se, وزن);

	reweight_eevdf(&rq->cfs, &p->se, وزن, p->se.on_rq);
}

ساكن مضمن صحيح throttled_hierarchy(بنية cfs_rq *cfs_rq);

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
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
ساكن اوږد __calc_smp_shares(بنية cfs_rq *cfs_rq, اوږد tg_shares, اوږد shares_max)
{
	بنية task_ډله *tg = cfs_rq->tg;
	اوږد tg_وزن, بارول, shares;

	بارول = max(scale_بارول_ښکته(cfs_rq->بارول.وزن), cfs_rq->avg.بارول_avg);

	tg_وزن = atomic_long_read(&tg->بارول_avg);

	/* Ensure tg_weight >= load */
	tg_وزن -= cfs_rq->tg_بارول_avg_contrib;
	tg_وزن += بارول;

	shares = (tg_shares * بارول);
	إذا (tg_وزن)
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
	أرجع clamp_t(اوږد, shares, MIN_SHARES, shares_max);
}

ساكن صحيح tg_cpus(بنية task_ډله *tg)
{
	صحيح nr = عدد_online_cpus();

	إذا (cpusets_enabled()) {
		بنية cgroup *cgrp = tg->css.cgroup;
		إذا (cgrp)
			nr = cpuset_عدد_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	أرجع max(nr, 1);
}

ساكن مضمن صحيح tg_tasks(بنية task_ډله *tg)
{
	أرجع max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_ظرفیت_ښويدنه);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
ساكن اوږد calc_tasks_shares(بنية cfs_rq *cfs_rq)
{
	بنية task_ډله *tg = cfs_rq->tg;
	صحيح nr = tg_tasks(tg);
	اوږد tg_shares = READ_ONCE(tg->shares);
	أرجع __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
ساكن اوږد calc_max_shares(بنية cfs_rq *cfs_rq)
{
	بنية task_ډله *tg = cfs_rq->tg;
	صحيح nr = tg_cpus(tg);
	اوږد tg_shares = READ_ONCE(tg->shares);
	اوږد max_shares = scale_بارول(sched_prio_منزل_وزن[0]);
	أرجع __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
ساكن اوږد calc_concur_shares(بنية cfs_rq *cfs_rq)
{
	بنية task_ډله *tg = cfs_rq->tg;
	صحيح nr = min(tg_tasks(tg), tg_cpus(tg));
	اوږد tg_shares = READ_ONCE(tg->shares);
	أرجع __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
ساكن اوږد calc_smp_shares(بنية cfs_rq *cfs_rq)
{
	بنية task_ډله *tg = cfs_rq->tg;
	اوږد tg_shares = READ_ONCE(tg->shares);
	أرجع __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
ساكن اوږد calc_پورته_shares(بنية cfs_rq *cfs_rq)
{
	بنية task_ډله *tg = cfs_rq->tg;
	أرجع READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_ډله_shares, calc_concur_shares);

فراغ __sched_cgroup_mode_update(صحيح ډېرپېښ)
{
	اوږد (*func)(بنية cfs_rq *);
	بدل (ډېرپېښ) {
	حالة 0:
		func = &calc_پورته_shares;
		اكسر;
	حالة 1:
		func = &calc_smp_shares;
		اكسر;
	حالة 2:
	افتراضي:
		func = &calc_concur_shares;
		اكسر;
	حالة 3:
		func = &calc_max_shares;
		اكسر;
	حالة 4:
		func = &calc_tasks_shares;
		اكسر;
	}
	static_call_update(calc_ډله_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
ساكن فراغ update_cfs_ډله(بنية sched_entity *se)
{
	بنية cfs_rq *gcfs_rq = ډله_cfs_rq(se);
	اوږد shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	إذا (!gcfs_rq || !gcfs_rq->بارول.وزن)
		أرجع;

	shares = static_call(calc_ډله_shares)(gcfs_rq);
	reweight_entity(cfs_rq_د(se), se, shares);
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */
ساكن مضمن فراغ update_cfs_ډله(بنية sched_entity *se)
{
}
#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

ساكن مضمن فراغ cfs_rq_util_change(بنية cfs_rq *cfs_rq, صحيح flags)
{
	بنية rq *rq = rq_د(cfs_rq);

	إذا (&rq->cfs == cfs_rq) {
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

ساكن مضمن bool بارول_avg_is_decayed(بنية sched_avg *sa)
{
	إذا (sa->بارول_مجموعه)
		أرجع false;

	إذا (sa->util_مجموعه)
		أرجع false;

	إذا (sa->runnable_مجموعه)
		أرجع false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->بارول_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	أرجع true;
}

ساكن مضمن u64 cfs_rq_وروستی_update_وخت(بنية cfs_rq *cfs_rq)
{
	أرجع u64_u32_بارول_copy(cfs_rq->avg.وروستی_update_وخت,
				 cfs_rq->وروستی_update_وخت_copy);
}
#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
ساكن مضمن bool ماشوم_cfs_rq_on_list(بنية cfs_rq *cfs_rq)
{
	بنية cfs_rq *prev_cfs_rq;
	بنية list_سر *prev;
	بنية rq *rq = rq_د(cfs_rq);

	إذا (cfs_rq->on_list) {
		prev = cfs_rq->ونې_پاڼه_cfs_rq_list.prev;
	} وإلا {
		prev = rq->tmp_alone_branch;
	}

	إذا (prev == &rq->ونې_پاڼه_cfs_rq_list)
		أرجع false;

	prev_cfs_rq = container_د(prev, بنية cfs_rq, ونې_پاڼه_cfs_rq_list);

	أرجع (prev_cfs_rq->tg->پلار == cfs_rq->tg);
}

ساكن مضمن bool cfs_rq_is_decayed(بنية cfs_rq *cfs_rq)
{
	إذا (cfs_rq->بارول.وزن)
		أرجع false;

	إذا (!بارول_avg_is_decayed(&cfs_rq->avg))
		أرجع false;

	إذا (ماشوم_cfs_rq_on_list(cfs_rq))
		أرجع false;

	إذا (cfs_rq->tg_بارول_avg_contrib)
		أرجع false;

	أرجع true;
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
ساكن مضمن فراغ update_tg_بارول_avg(بنية cfs_rq *cfs_rq)
{
	اوږد dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	إذا (cfs_rq->tg == &ریښه_task_ډله)
		أرجع;

	/* rq has been offline and doesn't contribute to the share anymore: */
	إذا (!cpu_فعال(cpu_د(rq_د(cfs_rq))))
		أرجع;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_د(cfs_rq));
	إذا (now - cfs_rq->وروستی_update_tg_بارول_avg < NSEC_فی_MSEC)
		أرجع;

	dl = cfs_rq->avg.بارول_avg - cfs_rq->tg_بارول_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	إذا (مطلق_قیمت(dl) > cfs_rq->tg_بارول_avg_contrib / 64 ||
	    مطلق_قیمت(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_جمع_کړه(dl, &cfs_rq->tg->بارول_avg);
		atomic_long_جمع_کړه(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_بارول_avg_contrib = cfs_rq->avg.بارول_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->وروستی_update_tg_بارول_avg = now;
	}
}

ساكن مضمن فراغ clear_tg_بارول_avg(بنية cfs_rq *cfs_rq)
{
	اوږد dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	إذا (cfs_rq->tg == &ریښه_task_ډله)
		أرجع;

	now = rq_clock(rq_د(cfs_rq));
	dl = 0 - cfs_rq->tg_بارول_avg_contrib;
	dr = 0 - cfs_rq->tg_runnable_avg_contrib;
	atomic_long_جمع_کړه(dl, &cfs_rq->tg->بارول_avg);
	atomic_long_جمع_کړه(dr, &cfs_rq->tg->runnable_avg);
	cfs_rq->tg_بارول_avg_contrib = 0;
	cfs_rq->tg_runnable_avg_contrib = 0;
	cfs_rq->وروستی_update_tg_بارول_avg = now;
}

/* CPU offline callback: */
ساكن فراغ __maybe_unused clear_tg_offline_cfs_rqs(بنية rq *rq)
{
	بنية task_ډله *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_پیل_loop_update(rq);

	guard(rcu)();

	list_for_each_ثبت_rcu(tg, &task_groups, list) {
		بنية cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_د(rq));

		clear_tg_بارول_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
فراغ سېټ_task_rq_fair(بنية sched_entity *se,
		      بنية cfs_rq *prev, بنية cfs_rq *بل)
{
	u64 p_وروستی_update_وخت;
	u64 n_وروستی_update_وخت;

	إذا (!sched_feat(ATTACH_AGE_بارول))
		أرجع;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	إذا (!(se->avg.وروستی_update_وخت && prev))
		أرجع;

	p_وروستی_update_وخت = cfs_rq_وروستی_update_وخت(prev);
	n_وروستی_update_وخت = cfs_rq_وروستی_update_وخت(بل);

	__update_load_avg_blocked_se(p_وروستی_update_وخت, se);
	se->avg.وروستی_update_وخت = n_وروستی_update_وخت;
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
ساكن مضمن فراغ
update_tg_cfs_util(بنية cfs_rq *cfs_rq, بنية sched_entity *se, بنية cfs_rq *gcfs_rq)
{
	اوږد delta_مجموعه, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_مجموعه, divider;

	/* Nothing to update */
	إذا (!delta_avg)
		أرجع;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's utilization */
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	new_مجموعه = se->avg.util_avg * divider;
	delta_مجموعه = (اوږد)new_مجموعه - (اوږد)se->avg.util_مجموعه;
	se->avg.util_مجموعه = new_مجموعه;

	/* Update parent cfs_rq utilization */
	__update_sa(&cfs_rq->avg, util, delta_avg, delta_مجموعه);
}

ساكن مضمن فراغ
update_tg_cfs_runnable(بنية cfs_rq *cfs_rq, بنية sched_entity *se, بنية cfs_rq *gcfs_rq)
{
	اوږد delta_مجموعه, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_مجموعه;
	u32 divider;

	/* Nothing to update */
	إذا (!delta_avg)
		أرجع;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's runnable */
	se->avg.runnable_avg = gcfs_rq->avg.runnable_avg;
	new_مجموعه = (u64)se->avg.runnable_avg * divider;
	delta_مجموعه = (اوږد)new_مجموعه - (اوږد)se->avg.runnable_مجموعه;
	se->avg.runnable_مجموعه = new_مجموعه;

	/* Update parent cfs_rq runnable */
	__update_sa(&cfs_rq->avg, runnable, delta_avg, delta_مجموعه);
}

ساكن مضمن فراغ
update_tg_cfs_بارول(بنية cfs_rq *cfs_rq, بنية sched_entity *se, بنية cfs_rq *gcfs_rq)
{
	اوږد delta_avg, running_مجموعه, runnable_مجموعه = gcfs_rq->prop_runnable_مجموعه;
	بینښه اوږد بارول_avg;
	u64 بارول_مجموعه = 0;
	s64 delta_مجموعه;
	u32 divider;

	إذا (!runnable_مجموعه)
		أرجع;

	gcfs_rq->prop_runnable_مجموعه = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	إذا (runnable_مجموعه >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_مجموعه += se->avg.بارول_مجموعه;
		runnable_مجموعه = min_t(اوږد, runnable_مجموعه, divider);
	} وإلا {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		إذا (scale_بارول_ښکته(gcfs_rq->بارول.وزن)) {
			بارول_مجموعه = div_u64(gcfs_rq->avg.بارول_مجموعه,
				scale_بارول_ښکته(gcfs_rq->بارول.وزن));
		}

		/* But make sure to not inflate se's runnable */
		runnable_مجموعه = min(se->avg.بارول_مجموعه, بارول_مجموعه);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_مجموعه = se->avg.util_مجموعه >> SCHED_ظرفیت_ښويدنه;
	runnable_مجموعه = max(runnable_مجموعه, running_مجموعه);

	بارول_مجموعه = se_وزن(se) * runnable_مجموعه;
	بارول_avg = div_u64(بارول_مجموعه, divider);

	delta_avg = بارول_avg - se->avg.بارول_avg;
	إذا (!delta_avg)
		أرجع;

	delta_مجموعه = بارول_مجموعه - (s64)se_وزن(se) * se->avg.بارول_مجموعه;

	se->avg.بارول_مجموعه = runnable_مجموعه;
	se->avg.بارول_avg = بارول_avg;
	__update_sa(&cfs_rq->avg, بارول, delta_avg, delta_مجموعه);
}

ساكن مضمن فراغ جمع_کړه_tg_cfs_propagate(بنية cfs_rq *cfs_rq, اوږد runnable_مجموعه)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_مجموعه += runnable_مجموعه;
}

/* Update task and its cfs_rq load average */
ساكن مضمن صحيح propagate_entity_بارول_avg(بنية sched_entity *se)
{
	بنية cfs_rq *cfs_rq, *gcfs_rq;

	إذا (entity_is_task(se))
		أرجع 0;

	gcfs_rq = ډله_cfs_rq(se);
	إذا (!gcfs_rq->propagate)
		أرجع 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_د(se);

	جمع_کړه_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_مجموعه);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_بارول(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	أرجع 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
ساكن مضمن bool skip_blocked_update(بنية sched_entity *se)
{
	بنية cfs_rq *gcfs_rq = ډله_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	إذا (se->avg.بارول_avg || se->avg.util_avg)
		أرجع false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	إذا (gcfs_rq->propagate)
		أرجع false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	أرجع true;
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

ساكن مضمن فراغ update_tg_بارول_avg(بنية cfs_rq *cfs_rq) {}

ساكن مضمن فراغ clear_tg_offline_cfs_rqs(بنية rq *rq) {}

ساكن مضمن صحيح propagate_entity_بارول_avg(بنية sched_entity *se)
{
	أرجع 0;
}

ساكن مضمن فراغ جمع_کړه_tg_cfs_propagate(بنية cfs_rq *cfs_rq, اوږد runnable_مجموعه) {}

#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

#که_تعریف_شوی CONFIG_NO_HZ_COMMON
ساكن مضمن فراغ migrate_se_pelt_lag(بنية sched_entity *se)
{
	u64 throttled = 0, now, lut;
	بنية cfs_rq *cfs_rq;
	بنية rq *rq;
	bool is_idle;

	إذا (بارول_avg_is_decayed(&se->avg))
		أرجع;

	cfs_rq = cfs_rq_د(se);
	rq = rq_د(cfs_rq);

	rcu_read_قلف();
	is_idle = is_idle_task(rcu_dereference_all(rq->curr));
	rcu_read_unlock();

	/*
	 * The lag estimation comes with a cost we don't want to pay all the
	 * time. Hence, limiting to the case where the source CPU is idle and
	 * we know we are at the greatest risk to have an outdated clock.
	 */
	إذا (!is_idle)
		أرجع;

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

#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_بارول(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	إذا (throttled == U64_MAX)
		أرجع;
#د_که_پای
	now = u64_u32_بارول(rq->clock_pelt_idle);
	/*
	 * Paired with _update_idle_rq_clock_pelt(). It ensures at the worst case
	 * is observed the old clock_pelt_idle value and the new clock_idle,
	 * which lead to an underestimation. The opposite would lead to an
	 * overestimation.
	 */
	smp_rmb();
	lut = cfs_rq_وروستی_update_وخت(cfs_rq);

	now -= throttled;
	إذا (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	وإلا
		now += sched_clock_cpu(cpu_د(rq)) - u64_u32_بارول(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#وإلا /* !CONFIG_NO_HZ_COMMON: */
ساكن فراغ migrate_se_pelt_lag(بنية sched_entity *se) {}
#د_که_پای /* !CONFIG_NO_HZ_COMMON */

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
ساكن مضمن صحيح
update_cfs_rq_بارول_avg(u64 now, بنية cfs_rq *cfs_rq)
{
	بینښه اوږد removed_بارول = 0, removed_util = 0, removed_runnable = 0;
	بنية sched_avg *sa = &cfs_rq->avg;
	صحيح decayed = 0;

	إذا (cfs_rq->removed.nr) {
		بینښه اوږد r;
		u32 divider = get_pelt_divider(&cfs_rq->avg);

		raw_spin_قلف(&cfs_rq->removed.قلف);
		بدلون(cfs_rq->removed.util_avg, removed_util);
		بدلون(cfs_rq->removed.بارول_avg, removed_بارول);
		بدلون(cfs_rq->removed.runnable_avg, removed_runnable);
		cfs_rq->removed.nr = 0;
		raw_spin_unlock(&cfs_rq->removed.قلف);

		r = removed_بارول;
		__update_sa(sa, بارول, -r, -r*divider);

		r = removed_util;
		__update_sa(sa, util, -r, -r*divider);

		r = removed_runnable;
		__update_sa(sa, runnable, -r, -r*divider);

		/*
		 * removed_runnable is the unweighted version of removed_load so we
		 * can use it to estimate removed_load_sum.
		 */
		جمع_کړه_tg_cfs_propagate(cfs_rq,
			-(اوږد)(removed_runnable * divider) >> SCHED_ظرفیت_ښويدنه);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->وروستی_update_وخت,
			   cfs_rq->وروستی_update_وخت_copy,
			   sa->وروستی_update_وخت);
	أرجع decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
ساكن فراغ attach_entity_بارول_avg(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
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
	se->avg.وروستی_update_وخت = cfs_rq->avg.وروستی_update_وخت;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	/*
	 * Hell(o) Nasty stuff.. we need to recompute _sum based on the new
	 * period_contrib. This isn't strictly correct, but since we're
	 * entirely outside of the PELT hierarchy, nobody cares if we truncate
	 * _sum a little.
	 */
	se->avg.util_مجموعه = se->avg.util_avg * divider;

	se->avg.runnable_مجموعه = se->avg.runnable_avg * divider;

	se->avg.بارول_مجموعه = se->avg.بارول_avg * divider;
	إذا (se_وزن(se) < se->avg.بارول_مجموعه)
		se->avg.بارول_مجموعه = div_u64(se->avg.بارول_مجموعه, se_وزن(se));
	وإلا
		se->avg.بارول_مجموعه = 1;

	کتار_ته_ورزیاتول_بارول_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_مجموعه += se->avg.util_مجموعه;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_مجموعه += se->avg.runnable_مجموعه;

	جمع_کړه_tg_cfs_propagate(cfs_rq, se->avg.بارول_مجموعه);

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
ساكن فراغ detach_entity_بارول_avg(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	له_کتار_ایستل_بارول_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_مجموعه);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_مجموعه);

	جمع_کړه_tg_cfs_propagate(cfs_rq, -se->avg.بارول_مجموعه);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#تعریف_کړه UTIL_EST_MARGIN (SCHED_ظرفیت_SCALE / 100)

ساكن مضمن فراغ util_est_update(بنية sched_entity *se)
{
	بینښه صحيح ewma, dequeued, وروستی_ewma_diff;

	إذا (!sched_feat(UTIL_EST))
		أرجع;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	إذا (ewma & UTIL_AVG_UNCHANGED)
		أرجع;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	إذا (ewma <= dequeued) {
		ewma = dequeued;
		اذهب بشپړ;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	وروستی_ewma_diff = ewma - dequeued;
	إذا (وروستی_ewma_diff < UTIL_EST_MARGIN)
		اذهب بشپړ;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	إذا ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		اذهب بشپړ;

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
	ewma <<= UTIL_EST_وزن_ښويدنه;
	ewma  -= وروستی_ewma_diff;
	ewma >>= UTIL_EST_وزن_ښويدنه;
بشپړ:
	ewma |= UTIL_AVG_UNCHANGED;
	WRITE_ONCE(se->avg.util_est, ewma);

	trace_sched_util_est_se_tp(se);
}

/*
 * Optional action to be done while updating the load average
 */
#تعریف_کړه UPDATE_TG	0x01
#تعریف_کړه SKIP_AGE_بارول	0x02
#تعریف_کړه DO_ATTACH	0x04
#تعریف_کړه DO_DETACH	0x08
#تعریف_کړه UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
ساكن مضمن فراغ update_بارول_avg(بنية cfs_rq *cfs_rq, بنية sched_entity *se, صحيح flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	صحيح decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	إذا (se->avg.وروستی_update_وخت && !(flags & SKIP_AGE_بارول))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_بارول_avg(now, cfs_rq);
	decayed |= propagate_entity_بارول_avg(se);

	إذا (!se->avg.وروستی_update_وخت && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_بارول_avg(cfs_rq, se);
		update_tg_بارول_avg(cfs_rq);

	} وإلا إذا (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_بارول_avg(cfs_rq, se);
		update_tg_بارول_avg(cfs_rq);
	} وإلا إذا (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		إذا (flags & UPDATE_TG)
			update_tg_بارول_avg(cfs_rq);
	}

	إذا (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
ساكن فراغ sync_entity_بارول_avg(بنية sched_entity *se)
{
	بنية cfs_rq *cfs_rq = cfs_rq_د(se);
	u64 وروستی_update_وخت;

	وروستی_update_وخت = cfs_rq_وروستی_update_وخت(cfs_rq);
	__update_load_avg_blocked_se(وروستی_update_وخت, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
ساكن فراغ remove_entity_بارول_avg(بنية sched_entity *se)
{
	بنية cfs_rq *cfs_rq = cfs_rq_د(se);
	بینښه اوږد flags;

	/*
	 * tasks cannot exit without having gone through wake_up_new_task() ->
	 * enqueue_task_fair() which will have added things to the cfs_rq,
	 * so we can remove unconditionally.
	 */

	sync_entity_بارول_avg(se);

	raw_spin_قلف_irqsave(&cfs_rq->removed.قلف, flags);
	++cfs_rq->removed.nr;
	cfs_rq->removed.util_avg	+= se->avg.util_avg;
	cfs_rq->removed.بارول_avg	+= se->avg.بارول_avg;
	cfs_rq->removed.runnable_avg	+= se->avg.runnable_avg;
	raw_spin_unlock_irqrestore(&cfs_rq->removed.قلف, flags);
}

ساكن مضمن بینښه اوږد cfs_rq_runnable_avg(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_rq->avg.runnable_avg;
}

ساكن مضمن بینښه اوږد cfs_rq_بارول_avg(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_rq->avg.بارول_avg;
}

ساكن صحيح sched_balance_newidle(بنية rq *this_rq, بنية rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

ساكن مضمن بینښه اوږد task_util(بنية task_struct *p)
{
	أرجع READ_ONCE(p->se.avg.util_avg);
}

ساكن مضمن بینښه اوږد _task_util_est(بنية task_struct *p)
{
	أرجع READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

ساكن مضمن بینښه اوږد task_util_est(بنية task_struct *p)
{
	أرجع max(task_util(p), _task_util_est(p));
}

ساكن مضمن فراغ util_est_کتار_ته_ورزیاتول(بنية cfs_rq *cfs_rq,
				    بنية task_struct *p)
{
	بینښه صحيح enqueued;

	إذا (!sched_feat(UTIL_EST))
		أرجع;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ساكن مضمن فراغ util_est_له_کتار_ایستل(بنية cfs_rq *cfs_rq,
				    بنية task_struct *p)
{
	بینښه صحيح enqueued;

	إذا (!sched_feat(UTIL_EST))
		أرجع;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(بینښه صحيح, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ساكن مضمن بینښه اوږد get_actual_cpu_ظرفیت(صحيح cpu)
{
	بینښه اوږد ظرفیت = arch_scale_cpu_ظرفیت(cpu);

	ظرفیت -= max(hw_بارول_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	أرجع ظرفیت;
}

ساكن مضمن صحيح util_ځاییږي_cpu(بینښه اوږد util,
				بینښه اوږد uclamp_min,
				بینښه اوږد uclamp_max,
				صحيح cpu)
{
	بینښه اوږد ظرفیت = ظرفیت_د(cpu);
	بینښه اوږد ظرفیت_orig;
	bool ځاییږي, uclamp_max_ځاییږي;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	ځاییږي = ځاییږي_ظرفیت(util, ظرفیت);

	إذا (!uclamp_is_کارول_شوی())
		أرجع ځاییږي;

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
	ظرفیت_orig = arch_scale_cpu_ظرفیت(cpu);

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
	uclamp_max_ځاییږي = (ظرفیت_orig == SCHED_ظرفیت_SCALE) && (uclamp_max == SCHED_ظرفیت_SCALE);
	uclamp_max_ځاییږي = !uclamp_max_ځاییږي && (uclamp_max <= ظرفیت_orig);
	ځاییږي = ځاییږي || uclamp_max_ځاییږي;

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
	إذا (ځاییږي && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_ظرفیت(cpu)))
		أرجع -1;

	أرجع ځاییږي;
}

ساكن مضمن صحيح task_ځاییږي_cpu(بنية task_struct *p, صحيح cpu)
{
	بینښه اوږد uclamp_min = uclamp_eff_ارزښت(p, UCLAMP_MIN);
	بینښه اوږد uclamp_max = uclamp_eff_ارزښت(p, UCLAMP_MAX);
	بینښه اوږد util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	أرجع (util_ځاییږي_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

ساكن مضمن فراغ update_misfit_حالت(بنية task_struct *p, بنية rq *rq)
{
	صحيح cpu = cpu_د(rq);

	إذا (!sched_asym_cpucap_فعال())
		أرجع;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	إذا (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_ظرفیت(cpu) == p->max_allowed_ظرفیت) ||
	    task_ځاییږي_cpu(p, cpu)) {

		rq->misfit_task_بارول = 0;
		أرجع;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_بارول = max_t(بینښه اوږد, task_h_بارول(p), 1);
}

فراغ __setparam_fair(بنية task_struct *p, ثابت بنية sched_attr *attr)
{
	بنية sched_entity *se = &p->se;

	p->static_prio = NICE_منزل_PRIO(attr->sched_nice);
	إذا (attr->sched_runtime) {
		se->custom_ټوټه = 1;
		se->ټوټه = clamp_t(u64, attr->sched_runtime,
				      NSEC_فی_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_فی_MSEC*100); /* HZ=100  / 10 */
	} وإلا {
		se->custom_ټوټه = 0;
		se->ټوټه = sysctl_sched_بنسټ_ټوټه;
	}
}

ساكن فراغ
ځای_پرځای_کړه_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se, صحيح flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	بینښه صحيح nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	إذا (!se->custom_ټوټه)
		se->ټوټه = sysctl_sched_بنسټ_ټوټه;
	vslice = calc_delta_fair(se->ټوټه, se);

	إذا (flags & کتار_ته_ورزیاتول_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	إذا (sched_feat(ځای_پرځای_کړه_LAG) && nr_queued && se->vlag) {
		بنية sched_entity *curr = cfs_rq->curr;
		اوږد بارول, وزن;

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
		بارول = cfs_rq->مجموعه_وزن;
		إذا (curr && curr->on_rq)
			بارول += avg_vruntime_وزن(cfs_rq, curr->h_بارول.وزن);
		إذا (WARN_ON_ONCE(!بارول))
			بارول = 1;

		وزن = avg_vruntime_وزن(cfs_rq, se->h_بارول.وزن);

		/*
		 * Rewrite, because the product (W + w_i)*vl'_i might
		 * overflow s64:
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 *        = (W*vl'_i + w_i*vl'_i) / W
		 *        = vl'_i + w_i*vl'_i/W
		 */
		lag += div64_long(lag * وزن, بارول);

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
		إذا (وزن > بارول)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	إذا (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	إذا (sched_feat(ځای_پرځای_کړه_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		أرجع;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	إذا (sched_feat(ځای_پرځای_کړه_DEADLINE_INITIAL) && (flags & کتار_ته_ورزیاتول_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

ساكن فراغ check_کتار_ته_ورزیاتول_throttle(بنية cfs_rq *cfs_rq);
ساكن مضمن صحيح cfs_rq_throttled(بنية cfs_rq *cfs_rq);

ساكن فراغ
کتار_ته_ورزیاتول_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se, صحيح flags)
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
	update_بارول_avg(cfs_rq, se, UPDATE_TG | DO_ATTACH);
	se_update_runnable(se);
	/*
	 * XXX update_load_avg() above will have attached us to the pelt sum;
	 * but update_cfs_group() here will re-adjust the weight and have to
	 * undo/redo all that. Seems wasteful.
	 */
	update_cfs_ډله(se);

	account_entity_کتار_ته_ورزیاتول(cfs_rq, se);

	/* Entity has migrated, no longer consider this task hot */
	إذا (flags & کتار_ته_ورزیاتول_MIGRATED)
		se->exec_پیل = 0;

	check_schedstat_required();
	update_stats_کتار_ته_ورزیاتول_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	إذا (cfs_rq->nr_queued == 1) {
		check_کتار_ته_ورزیاتول_throttle(cfs_rq);
		list_جمع_کړه_ونې_پاڼه_cfs_rq(cfs_rq);
#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
		إذا (cfs_rq->pelt_clock_throttled) {
			بنية rq *rq = rq_د(cfs_rq);

			cfs_rq->throttled_clock_pelt_وخت += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#د_که_پای
	}
}

ساكن فراغ سېټ_بل_buddy(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	إذا (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		أرجع;
	إذا (se_is_idle(se))
		أرجع;
	cfs_rq->بل = se;
}

ساكن فراغ clear_buddies(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	إذا (cfs_rq->بل == se)
		cfs_rq->بل = عدم;
}

ساكن __always_inline فراغ return_cfs_rq_runtime(بنية cfs_rq *cfs_rq);

ساكن فراغ سېټ_delayed(بنية sched_entity *se)
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
	إذا (!entity_is_task(se)) {
		se->sched_delayed = 1;
		أرجع;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_د(cfs_rq_د(se)), task_د(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		بنية cfs_rq *cfs_rq = cfs_rq_د(se);

		cfs_rq->h_nr_runnable--;
	}
}

ساكن فراغ clear_delayed(بنية sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	إذا (!entity_is_task(se))
		أرجع;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_د(cfs_rq_د(se)), task_د(se));

	for_each_sched_entity(se) {
		بنية cfs_rq *cfs_rq = cfs_rq_د(se);

		cfs_rq->h_nr_runnable++;
	}
}

ساكن فراغ
له_کتار_ایستل_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se, صحيح flags)
{
	صحيح action = UPDATE_TG;

	إذا (entity_is_task(se)) {
		إذا (task_on_rq_migrating(task_د(se)))
			action |= DO_DETACH;

		إذا ((flags & له_کتار_ایستل_SLEEP) && !(flags & له_کتار_ایستل_DELAYED))
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
	update_بارول_avg(cfs_rq, se, action);
	se_update_runnable(se);

	update_stats_له_کتار_ایستل_fair(cfs_rq, se, flags);

	se->on_rq = 0;
	account_entity_له_کتار_ایستل(cfs_rq, se);

	/* return excess runtime on last dequeue */
	return_cfs_rq_runtime(cfs_rq);

	update_cfs_ډله(se);

	إذا (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
		إذا (throttled_hierarchy(cfs_rq)) {
			بنية rq *rq = rq_د(cfs_rq);

			list_del_ونې_پاڼه_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#د_که_پای
	}
}

ساكن فراغ
سېټ_بل_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	إذا (se->on_rq) {
		/*
		 * Any task has to be enqueued before it get to execute on
		 * a CPU. So account for the time it spent waiting on the
		 * runqueue.
		 */
		update_stats_wait_پای_ټکی_fair(cfs_rq, se);
		update_بارول_avg(cfs_rq, se, UPDATE_TG);
	}

	update_stats_curr_پیل(cfs_rq, se);
	WARN_ON_ONCE(cfs_rq->h_curr);
	cfs_rq->h_curr = se;

	/*
	 * Track our maximum slice length, if the CPU's load is at
	 * least twice that of our own weight (i.e. don't track it
	 * when there are only lesser-weight tasks around):
	 */
	إذا (schedstat_enabled() &&
	    rq_د(cfs_rq)->cfs.بارول.وزن >= 2*se->بارول.وزن) {
		بنية sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->ټوټه_max,
				max((u64)stats->ټوټه_max,
				    se->مجموعه_exec_runtime - se->prev_مجموعه_exec_runtime));
	}

	se->prev_مجموعه_exec_runtime = se->مجموعه_exec_runtime;
}

ساكن bool __dequeue_task(بنية rq *rq, بنية task_struct *p, صحيح flags);

ساكن بنية sched_entity *
pick_بل_entity(بنية rq *rq, bool protect)
{
	بنية cfs_rq *cfs_rq = &rq->cfs;
	بنية sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	إذا (se->sched_delayed) {
		__dequeue_task(rq, task_د(se), له_کتار_ایستل_SLEEP | له_کتار_ایستل_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		أرجع عدم;
	}
	أرجع se;
}

ساكن فراغ put_prev_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *prev)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	إذا (prev->on_rq)
		update_curr(cfs_rq);

	إذا (prev->on_rq) {
		update_stats_wait_پیل_fair(cfs_rq, prev);
		/* in !on_rq case, update occurred at dequeue */
		update_بارول_avg(cfs_rq, prev, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != prev);
	cfs_rq->h_curr = عدم;
}

ساكن فراغ
entity_tick(بنية cfs_rq *cfs_rq, بنية sched_entity *curr, صحيح queued)
{
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr(cfs_rq);

	/*
	 * Ensure that runnable average is periodically updated.
	 */
	update_بارول_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_ډله(curr);

#که_تعریف_شوی CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	إذا (queued) {
		resched_curr(rq_د(cfs_rq));
		أرجع;
	}
#د_که_پای
}


/**************************************************
 * CFS bandwidth control machinery
 */

#که_تعریف_شوی CONFIG_CFS_BANDWIDTH

#که_تعریف_شوی CONFIG_JUMP_LABEL
ساكن بنية static_کلی __cfs_bandwidth_used;

ساكن مضمن bool cfs_bandwidth_کارول_شوی(فراغ)
{
	أرجع static_کلی_false(&__cfs_bandwidth_used);
}

فراغ cfs_bandwidth_usage_inc(فراغ)
{
	static_کلی_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

فراغ cfs_bandwidth_usage_dec(فراغ)
{
	static_کلی_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#وإلا /* !CONFIG_JUMP_LABEL: */
ساكن bool cfs_bandwidth_کارول_شوی(فراغ)
{
	أرجع true;
}

فراغ cfs_bandwidth_usage_inc(فراغ) {}
فراغ cfs_bandwidth_usage_dec(فراغ) {}
#د_که_پای /* !CONFIG_JUMP_LABEL */

ساكن مضمن u64 sched_cfs_bandwidth_ټوټه(فراغ)
{
	أرجع (u64)sysctl_sched_cfs_bandwidth_ټوټه * NSEC_فی_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
فراغ __refill_cfs_bandwidth_runtime(بنية cfs_bandwidth *cfs_b)
{
	s64 runtime;

	إذا (unlikely(cfs_b->quota == RUNTIME_INF))
		أرجع;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	إذا (runtime > 0) {
		cfs_b->موده_وخت += runtime;
		cfs_b->nr_موده++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->موده);
	cfs_b->runtime_snap = cfs_b->runtime;
}

ساكن مضمن بنية cfs_bandwidth *tg_cfs_bandwidth(بنية task_ډله *tg)
{
	أرجع &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
ساكن صحيح __assign_cfs_rq_runtime(بنية cfs_bandwidth *cfs_b,
				   بنية cfs_rq *cfs_rq, u64 موخه_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->قلف);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = موخه_runtime - cfs_rq->runtime_پاتې_شوی;

	إذا (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	وإلا {
		پیل_cfs_bandwidth(cfs_b);

		إذا (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_پاتې_شوی += amount;

	أرجع cfs_rq->runtime_پاتې_شوی > 0;
}

ساكن bool throttle_cfs_rq(بنية cfs_rq *cfs_rq);

ساكن bool __account_cfs_rq_runtime(بنية cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_پاتې_شوی -= delta_exec;

	إذا (likely(cfs_rq->runtime_پاتې_شوی > 0))
		أرجع false;

	إذا (cfs_rq->throttled)
		أرجع true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	أرجع throttle_cfs_rq(cfs_rq);
}

ساكن __always_inline
bool account_cfs_rq_runtime(بنية cfs_rq *cfs_rq, u64 delta_exec)
{
	إذا (!cfs_bandwidth_کارول_شوی() || !cfs_rq->runtime_enabled)
		أرجع false;

	أرجع __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

ساكن مضمن صحيح cfs_rq_throttled(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_bandwidth_کارول_شوی() && cfs_rq->throttled;
}

ساكن مضمن bool cfs_rq_pelt_clock_throttled(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_bandwidth_کارول_شوی() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
ساكن مضمن صحيح throttled_hierarchy(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_bandwidth_کارول_شوی() && cfs_rq->throttle_شمېر;
}

ساكن مضمن صحيح lb_throttled_hierarchy(بنية task_struct *p, صحيح dst_cpu)
{
	أرجع throttled_hierarchy(tg_cfs_rq(task_ډله(p), dst_cpu));
}

ساكن مضمن bool task_is_throttled(بنية task_struct *p)
{
	أرجع cfs_bandwidth_کارول_شوی() && p->throttled;
}

ساكن bool له_کتار_ایستل_task_fair(بنية rq *rq, بنية task_struct *p, صحيح flags);
ساكن فراغ throttle_cfs_rq_work(بنية callback_سر *work)
{
	بنية task_struct *p = container_د(work, بنية task_struct, sched_throttle_work);
	بنية sched_entity *se;
	بنية cfs_rq *cfs_rq;
	بنية rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.بل = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	إذا ((p->flags & PF_EXITING))
		أرجع;

	scoped_guard(task_rq_قلف_2, p) {
		se = &p->se;
		cfs_rq = cfs_rq_د(se);

		/* Raced, forget */
		إذا (p->sched_class != &fair_sched_class)
			أرجع;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		إذا (!cfs_rq->throttle_شمېر)
			أرجع;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_تشه(&p->throttle_غوټه));
		له_کتار_ایستل_task_fair(rq, p, له_کتار_ایستل_SLEEP | له_کتار_ایستل_THROTTLE);
		list_جمع_کړه(&p->throttle_غوټه, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

فراغ init_cfs_throttle_work(بنية task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.بل = &p->sched_throttle_work;
	INIT_LIST_سر(&p->throttle_غوټه);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
ساكن فراغ detach_task_cfs_rq(بنية task_struct *p);
ساكن فراغ له_کتار_ایستل_throttled_task(بنية task_struct *p, صحيح flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_غوټه);

	/* task blocked after throttled */
	إذا (flags & له_کتار_ایستل_SLEEP) {
		p->throttled = false;
		أرجع;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	إذا (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

ساكن bool کتار_ته_ورزیاتول_throttled_task(بنية task_struct *p)
{
	بنية cfs_rq *cfs_rq = cfs_rq_د(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_تشه(&p->throttle_غوټه));

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
	إذا (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_د(cfs_rq), p)) {
		list_جمع_کړه(&p->throttle_غوټه, &cfs_rq->throttled_limbo_list);
		أرجع true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	أرجع false;
}

ساكن فراغ کتار_ته_ورزیاتول_task_fair(بنية rq *rq, بنية task_struct *p, صحيح flags);
ساكن صحيح tg_unthrottle_پورته(بنية task_ډله *tg, فراغ *معلومات)
{
	بنية rq *rq = معلومات;
	بنية cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_د(rq));
	بنية task_struct *p, *tmp;
	LIST_سر(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	إذا (--cfs_rq->throttle_شمېر)
		أرجع 0;

	إذا (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_وخت += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	إذا (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		إذا (WARN_ON_ONCE((s64)delta < 0))
			delta = 0;

		cfs_rq->throttled_clock_self_وخت += delta;
	}

	/*
	 * Move the tasks to a local list since an update_curr() during
	 * enqueue_task_fair() can throttle a higher cfs_rq, and it can
	 * see the "throttled_limbo_list" being non-empty in
	 * tg_throttle_down() if throttle_count turned 0 above.
	 */
	list_splice_init(&cfs_rq->throttled_limbo_list, &throttled_tasks);

	/* Re-enqueue the tasks that have been throttled at this level. */
	list_for_each_ثبت_خوندي(p, tmp, &throttled_tasks, throttle_غوټه) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		إذا (cfs_rq->throttle_شمېر)
			اكسر;

		list_del_init(&p->throttle_غوټه);
		p->throttled = false;
		کتار_ته_ورزیاتول_task_fair(rq, p, کتار_ته_ورزیاتول_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	إذا (!cfs_rq_is_decayed(cfs_rq))
		list_جمع_کړه_ونې_پاڼه_cfs_rq(cfs_rq);

	أرجع 0;
}

ساكن مضمن bool task_has_throttle_work(بنية task_struct *p)
{
	أرجع p->sched_throttle_work.بل != &p->sched_throttle_work;
}

ساكن مضمن فراغ task_throttle_تیاری_work(بنية task_struct *p)
{
	إذا (task_has_throttle_work(p))
		أرجع;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	إذا ((p->flags & (PF_EXITING | PF_KTHREAD)))
		أرجع;

	task_work_جمع_کړه(p, &p->sched_throttle_work, TWA_RESUME);
}

ساكن فراغ ریکارډ_throttle_clock(بنية cfs_rq *cfs_rq)
{
	بنية rq *rq = rq_د(cfs_rq);

	إذا (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	إذا (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

ساكن صحيح tg_throttle_ښکته(بنية task_ډله *tg, فراغ *معلومات)
{
	بنية rq *rq = معلومات;
	بنية cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_د(rq));

	إذا (cfs_rq->throttle_شمېر++)
		أرجع 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	إذا (!cfs_rq->nr_queued) {
		list_del_ونې_پاڼه_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_تشه(&cfs_rq->throttled_limbo_list));
	أرجع 0;
}

ساكن bool throttle_cfs_rq(بنية cfs_rq *cfs_rq)
{
	بنية cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	بنية sched_entity *curr = cfs_rq->h_curr;
	بنية rq *rq = rq_د(cfs_rq);

	scoped_guard(raw_spinlock, &cfs_b->قلف) {
		u64 موخه_runtime = 1;

		/*
		 * If cfs_rq->h_curr is still runnable, we are here from an
		 * update_curr(). Request sysctl_sched_cfs_bandwidth_slice
		 * worth of bandwidth to continue running.
		 *
		 * If the curr is not runnable, just request enough bandwidth
		 * to be runnable next time the pick selects this cfs_rq.
		 */
		إذا (curr && curr->on_rq)
			موخه_runtime = sched_cfs_bandwidth_ټوټه();

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
		إذا (__assign_cfs_rq_runtime(cfs_b, cfs_rq, موخه_runtime))
			أرجع false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_جمع_کړه_لکۍ_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_ونه_from(cfs_rq->tg, tg_throttle_ښکته, tg_nop, (فراغ *)rq);

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
	إذا (curr && curr->on_rq)
		task_throttle_تیاری_work(rq->donor);

	أرجع true;
}

فراغ unthrottle_cfs_rq(بنية cfs_rq *cfs_rq)
{
	بنية rq *rq = rq_د(cfs_rq);
	بنية cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	بنية sched_entity *se = cfs_rq_se(cfs_rq);

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
	إذا (cfs_rq->runtime_enabled && cfs_rq->runtime_پاتې_شوی <= 0)
		أرجع;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->قلف) {
		list_del_rcu(&cfs_rq->throttled_list);

		إذا (!cfs_rq->throttled_clock)
			اكسر;

		cfs_b->throttled_وخت += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_ونه_from(cfs_rq->tg, tg_nop, tg_unthrottle_پورته, (فراغ *)rq);

	إذا (!cfs_rq->بارول.وزن) {
		إذا (!cfs_rq->on_list)
			أرجع;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			إذا (list_جمع_کړه_ونې_پاڼه_cfs_rq(cfs_rq_د(se)))
				اكسر;
		}
	}

	assert_list_ونې_پاڼه_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	إذا (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

ساكن فراغ __cfsb_csd_unthrottle(فراغ *arg)
{
	بنية cfs_rq *cursor, *tmp;
	بنية rq *rq = arg;

	guard(rq_قلف)(rq);

	/*
	 * Iterating over the list can trigger several call to
	 * update_rq_clock() in unthrottle_cfs_rq().
	 * Do it once and skip the potential next ones.
	 */
	update_rq_clock(rq);
	rq_clock_پیل_loop_update(rq);

	/*
	 * Since we hold rq lock we're safe from concurrent manipulation of
	 * the CSD list. However, this RCU critical section annotates the
	 * fact that we pair with sched_free_group_rcu(), so that we cannot
	 * race with group being freed in the window between removing it
	 * from the list and advancing to the next entry in the list.
	 */
	guard(rcu)();

	list_for_each_ثبت_خوندي(cursor, tmp, &rq->cfsb_csd_list,
				 throttled_csd_list) {
		list_del_init(&cursor->throttled_csd_list);

		إذا (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

ساكن مضمن فراغ __unthrottle_cfs_rq_async(بنية cfs_rq *cfs_rq)
{
	بنية rq *rq = rq_د(cfs_rq);
	bool لومړی;

	إذا (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		أرجع;
	}

	/* Already enqueued */
	إذا (WARN_ON_ONCE(!list_تشه(&cfs_rq->throttled_csd_list)))
		أرجع;

	لومړی = list_تشه(&rq->cfsb_csd_list);
	list_جمع_کړه_لکۍ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	إذا (لومړی)
		smp_call_function_single_async(cpu_د(rq), &rq->cfsb_csd);
}

ساكن فراغ unthrottle_cfs_rq_async(بنية cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_د(cfs_rq));

	إذا (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_پاتې_شوی <= 0))
		أرجع;

	__unthrottle_cfs_rq_async(cfs_rq);
}

ساكن bool distribute_cfs_runtime(بنية cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	صحيح this_cpu = smp_processor_id();
	u64 runtime, پاتې_شوی = 1;
	بنية cfs_rq *cfs_rq;
	بنية rq *rq;

	guard(rcu)();

	list_for_each_ثبت_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_د(cfs_rq);

		إذا (!پاتې_شوی) {
			throttled = true;
			اكسر;
		}

		guard(rq_قلف_irqsave)(rq);

		إذا (!cfs_rq_throttled(cfs_rq))
			استمر;

		/* Already queued for async unthrottle */
		إذا (!list_تشه(&cfs_rq->throttled_csd_list))
			استمر;

		إذا (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_پاتې_شوی > 0);

		scoped_guard(raw_spinlock, &cfs_b->قلف) {
			runtime = -cfs_rq->runtime_پاتې_شوی + 1;
			إذا (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			پاتې_شوی = cfs_b->runtime;
		}

		cfs_rq->runtime_پاتې_شوی += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		إذا (cfs_rq->runtime_پاتې_شوی <= 0) {
			throttled = true;
			اكسر;
		}

		/* we check whether we're throttled above */
		إذا (cpu_د(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			استمر;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_تشه(&rq->cfsb_csd_list);
		list_جمع_کړه_لکۍ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	إذا (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	أرجع throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
ساكن صحيح do_sched_cfs_period_timer(بنية cfs_bandwidth *cfs_b, صحيح overrun, بینښه اوږد flags)
	__must_hold(&cfs_b->قلف)
{
	صحيح throttled;

	/* no need to continue the timer with no bandwidth constraint */
	إذا (cfs_b->quota == RUNTIME_INF)
		اذهب بهر_deactivate;

	throttled = !list_تشه(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	إذا (cfs_b->idle && !throttled)
		اذهب بهر_deactivate;

	إذا (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		أرجع 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	طالما (throttled && cfs_b->runtime > 0) {
		raw_spin_unlock_irqrestore(&cfs_b->قلف, flags);
		/* we can't nest cfs_b->lock while distributing bandwidth */
		throttled = distribute_cfs_runtime(cfs_b);
		raw_spin_قلف_irqsave(&cfs_b->قلف, flags);
	}

	/*
	 * While we are ensured activity in the period following an
	 * unthrottle, this also covers the case in which the new bandwidth is
	 * insufficient to cover the existing bandwidth deficit.  (Forcing the
	 * timer to remain active while there are any throttled entities.)
	 */
	cfs_b->idle = 0;

	أرجع 0;

بهر_deactivate:
	أرجع 1;
}

/* a cfs_rq won't donate quota below this amount */
ساكن ثابت u64 min_cfs_rq_runtime = 1 * NSEC_فی_MSEC;
/* minimum remaining period time to redistribute slack quota */
ساكن ثابت u64 min_bandwidth_expiration = 2 * NSEC_فی_MSEC;
/* how long we wait to gather additional slack before distributing */
ساكن ثابت u64 cfs_bandwidth_slack_period = 5 * NSEC_فی_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
ساكن صحيح runtime_refresh_within(بنية cfs_bandwidth *cfs_b, u64 min_expire)
{
	بنية hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 پاتې_شوی;

	/* if the call-back is running a quota refresh is already occurring */
	إذا (hrtimer_callback_running(refresh_timer))
		أرجع 1;

	/* is a quota refresh about to occur? */
	پاتې_شوی = ktime_منزل_ns(hrtimer_expires_پاتې_شوی(refresh_timer));
	إذا (پاتې_شوی < (s64)min_expire)
		أرجع 1;

	أرجع 0;
}

ساكن فراغ پیل_cfs_slack_bandwidth(بنية cfs_bandwidth *cfs_b)
{
	u64 min_کیڼ = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	إذا (runtime_refresh_within(cfs_b, min_کیڼ))
		أرجع;

	/* don't push forwards an existing deferred unthrottle */
	إذا (cfs_b->slack_started)
		أرجع;
	cfs_b->slack_started = true;

	hrtimer_پیل(&cfs_b->slack_timer,
			ns_منزل_ktime(cfs_bandwidth_slack_period),
			HRTIMER_ډېرپېښ_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
ساكن فراغ __return_cfs_rq_runtime(بنية cfs_rq *cfs_rq)
{
	بنية cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_پاتې_شوی - min_cfs_rq_runtime;

	إذا (slack_runtime <= 0)
		أرجع;

	guard(raw_spinlock)(&cfs_b->قلف);

	إذا (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		إذا (cfs_b->runtime > sched_cfs_bandwidth_ټوټه() &&
		    !list_تشه(&cfs_b->throttled_cfs_rq))
			پیل_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_پاتې_شوی -= slack_runtime;
}

ساكن __always_inline فراغ return_cfs_rq_runtime(بنية cfs_rq *cfs_rq)
{
	إذا (!cfs_bandwidth_کارول_شوی())
		أرجع;

	إذا (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		أرجع;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
ساكن فراغ do_sched_cfs_slack_timer(بنية cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->قلف) {
		u64 runtime = 0, ټوټه = sched_cfs_bandwidth_ټوټه();

		cfs_b->slack_started = false;

		إذا (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			أرجع;

		إذا (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > ټوټه)
			runtime = cfs_b->runtime;

		إذا (!runtime)
			أرجع;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
ساكن فراغ check_کتار_ته_ورزیاتول_throttle(بنية cfs_rq *cfs_rq)
{
	إذا (!cfs_bandwidth_کارول_شوی())
		أرجع;

	/* an active group must be handled by the update_curr() path */
	إذا (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		أرجع;

	/* ensure the group is not already throttled */
	إذا (cfs_rq_throttled(cfs_rq))
		أرجع;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

ساكن فراغ sync_throttle(بنية task_ډله *tg, صحيح cpu)
{
	بنية cfs_rq *pcfs_rq, *cfs_rq;

	إذا (!cfs_bandwidth_کارول_شوی())
		أرجع;

	إذا (!tg->پلار)
		أرجع;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->پلار, cpu);

	cfs_rq->throttle_شمېر = pcfs_rq->throttle_شمېر;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	إذا (cfs_rq->throttle_شمېر)
		cfs_rq->pelt_clock_throttled = 1;
}

ساكن تعداد hrtimer_restart sched_cfs_slack_timer(بنية hrtimer *timer)
{
	بنية cfs_bandwidth *cfs_b =
		container_د(timer, بنية cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	أرجع HRTIMER_NORESTART;
}

ساكن تعداد hrtimer_restart sched_cfs_period_timer(بنية hrtimer *timer)
{
	بنية cfs_bandwidth *cfs_b =
		container_د(timer, بنية cfs_bandwidth, period_timer);
	صحيح overrun;
	صحيح idle = 0;
	صحيح شمېر = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->قلف);

	لكل (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		إذا (!overrun)
			اكسر;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		إذا (++شمېر > 3) {
			u64 new, old = ktime_منزل_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			إذا (new < max_bw_quota_period_us * NSEC_فی_USEC) {
				cfs_b->period = ns_منزل_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->موده *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_فی_USEC),
					div_u64(cfs_b->quota, NSEC_فی_USEC));
			} وإلا {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_فی_USEC),
					div_u64(cfs_b->quota, NSEC_فی_USEC));
			}

			/* reset count so we don't come right back in here */
			شمېر = 0;
		}
	}

	إذا (idle) {
		cfs_b->period_فعال = 0;
		أرجع HRTIMER_NORESTART;
	}

	أرجع HRTIMER_RESTART;
}

فراغ init_cfs_bandwidth(بنية cfs_bandwidth *cfs_b, بنية cfs_bandwidth *پلار)
{
	raw_spin_قلف_init(&cfs_b->قلف);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_منزل_ktime(default_bw_period_us());
	cfs_b->موده = 0;
	cfs_b->پوړیز_quota = پلار ? پلار->پوړیز_quota : RUNTIME_INF;

	INIT_LIST_سر(&cfs_b->throttled_cfs_rq);
	hrtimer_تیاری(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_ډېرپېښ_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_سېټ_expires(&cfs_b->period_timer,
			    get_random_u32_لاندې(cfs_b->period));
	hrtimer_تیاری(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_ډېرپېښ_REL);
	cfs_b->slack_started = false;
}

ساكن فراغ init_cfs_rq_runtime(بنية cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_سر(&cfs_rq->throttled_list);
	INIT_LIST_سر(&cfs_rq->throttled_csd_list);
	INIT_LIST_سر(&cfs_rq->throttled_limbo_list);
}

فراغ پیل_cfs_bandwidth(بنية cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->قلف);

	إذا (cfs_b->period_فعال)
		أرجع;

	cfs_b->period_فعال = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_پیل_expires(&cfs_b->period_timer, HRTIMER_ډېرپېښ_ABS_PINNED);
}

ساكن فراغ destroy_cfs_bandwidth(بنية cfs_bandwidth *cfs_b)
{
	صحيح __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	إذا (!cfs_b->throttled_cfs_rq.بل)
		أرجع;

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
		بنية rq *rq = cpu_rq(i);

		إذا (list_تشه(&rq->cfsb_csd_list))
			استمر;

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
ساكن فراغ __maybe_unused update_runtime_enabled(بنية rq *rq)
{
	بنية task_ډله *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_ثبت_rcu(tg, &task_groups, list) {
		بنية cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		بنية cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_د(rq));

		scoped_guard(raw_spinlock, &cfs_b->قلف)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
ساكن فراغ __maybe_unused unthrottle_offline_cfs_rqs(بنية rq *rq)
{
	بنية task_ډله *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	إذا (cpumask_test_cpu(cpu_د(rq), cpu_فعال_ماسک))
		أرجع;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_پیل_loop_update(rq);

	guard(rcu)();

	list_for_each_ثبت_rcu(tg, &task_groups, list) {
		بنية cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_د(rq));

		إذا (!cfs_rq->runtime_enabled)
			استمر;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		إذا (!cfs_rq_throttled(cfs_rq))
			استمر;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_پاتې_شوی = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(بنية task_struct *p)
{
	بنية cfs_rq *cfs_rq = task_cfs_rq(p);

	إذا (!cfs_bandwidth_کارول_شوی())
		أرجع false;

	إذا (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->پوړیز_quota != RUNTIME_INF)
		أرجع true;

	أرجع false;
}

#که_تعریف_شوی CONFIG_NO_HZ_ډک
/* called from pick_next_task_fair() */
ساكن فراغ sched_fair_update_stop_tick(بنية rq *rq, بنية task_struct *p)
{
	صحيح cpu = cpu_د(rq);

	إذا (!cfs_bandwidth_کارول_شوی())
		أرجع;

	إذا (!tick_nohz_ډک_cpu(cpu))
		أرجع;

	إذا (rq->nr_running != 1)
		أرجع;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	إذا (cfs_task_bw_constrained(p))
		tick_nohz_dep_سېټ_cpu(cpu, TICK_DEP_بټ_SCHED);
}
#د_که_پای /* CONFIG_NO_HZ_FULL */

#وإلا /* !CONFIG_CFS_BANDWIDTH: */

ساكن bool account_cfs_rq_runtime(بنية cfs_rq *cfs_rq, u64 delta_exec) { أرجع false; }
ساكن فراغ check_کتار_ته_ورزیاتول_throttle(بنية cfs_rq *cfs_rq) {}
ساكن مضمن فراغ sync_throttle(بنية task_ډله *tg, صحيح cpu) {}
ساكن __always_inline فراغ return_cfs_rq_runtime(بنية cfs_rq *cfs_rq) {}
ساكن فراغ task_throttle_تیاری_work(بنية task_struct *p) {}
ساكن bool task_is_throttled(بنية task_struct *p) { أرجع false; }
ساكن فراغ له_کتار_ایستل_throttled_task(بنية task_struct *p, صحيح flags) {}
ساكن bool کتار_ته_ورزیاتول_throttled_task(بنية task_struct *p) { أرجع false; }
ساكن فراغ ریکارډ_throttle_clock(بنية cfs_rq *cfs_rq) {}

ساكن مضمن صحيح cfs_rq_throttled(بنية cfs_rq *cfs_rq)
{
	أرجع 0;
}

ساكن مضمن bool cfs_rq_pelt_clock_throttled(بنية cfs_rq *cfs_rq)
{
	أرجع false;
}

ساكن مضمن صحيح throttled_hierarchy(بنية cfs_rq *cfs_rq)
{
	أرجع 0;
}

ساكن مضمن صحيح lb_throttled_hierarchy(بنية task_struct *p, صحيح dst_cpu)
{
	أرجع 0;
}

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
فراغ init_cfs_bandwidth(بنية cfs_bandwidth *cfs_b, بنية cfs_bandwidth *پلار) {}
ساكن فراغ init_cfs_rq_runtime(بنية cfs_rq *cfs_rq) {}
#د_که_پای

ساكن مضمن بنية cfs_bandwidth *tg_cfs_bandwidth(بنية task_ډله *tg)
{
	أرجع عدم;
}
ساكن مضمن فراغ destroy_cfs_bandwidth(بنية cfs_bandwidth *cfs_b) {}
ساكن مضمن فراغ update_runtime_enabled(بنية rq *rq) {}
ساكن مضمن فراغ unthrottle_offline_cfs_rqs(بنية rq *rq) {}
#که_تعریف_شوی CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(بنية task_struct *p)
{
	أرجع false;
}
#د_که_پای
#د_که_پای /* !CONFIG_CFS_BANDWIDTH */

#إذا !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_ډک)
ساكن مضمن فراغ sched_fair_update_stop_tick(بنية rq *rq, بنية task_struct *p) {}
#د_که_پای

/**************************************************
 * CFS operations on tasks:
 */

#که_تعریف_شوی CONFIG_SCHED_HRTICK
ساكن فراغ hrtick_پیل_fair(بنية rq *rq, بنية task_struct *p)
{
	بنية sched_entity *se = &p->se;
	بینښه اوږد scale = 1024;
	بینښه اوږد util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	إذا (rq->cfs.h_nr_queued <= 1)
		أرجع;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	إذا ((s64)vdelta < 0) {
		إذا (task_current_donor(rq, p))
			resched_curr(rq);
		أرجع;
	}
	delta = (se->h_بارول.وزن * vdelta) / NICE_0_بارول;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	إذا (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_پیل(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
ساكن فراغ hrtick_update(بنية rq *rq)
{
	بنية task_struct *donor = rq->donor;

	إذا (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		أرجع;

	إذا (hrtick_فعال(rq))
		أرجع;

	hrtick_پیل_fair(rq, donor);
}
#وإلا /* !CONFIG_SCHED_HRTICK: */
ساكن مضمن فراغ
hrtick_پیل_fair(بنية rq *rq, بنية task_struct *p)
{
}

ساكن مضمن فراغ hrtick_update(بنية rq *rq)
{
}
#د_که_پای /* !CONFIG_SCHED_HRTICK */

ساكن مضمن bool cpu_overutilized(صحيح cpu)
{
	بینښه اوږد rq_util_max;

	إذا (!sched_energy_enabled())
		أرجع false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	أرجع !util_ځاییږي_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
ساكن مضمن bool is_rd_overutilized(بنية ریښه_domain *rd)
{
	أرجع !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

ساكن مضمن فراغ سېټ_rd_overutilized(بنية ریښه_domain *rd, bool flag)
{
	إذا (!sched_energy_enabled())
		أرجع;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

ساكن مضمن فراغ check_update_overutilized_حالت(بنية rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	إذا (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		سېټ_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
ساكن صحيح sched_idle_rq(بنية rq *rq)
{
	أرجع unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

ساكن صحيح ټاکنه_sched_idle_rq(بنية rq *rq, بنية task_struct *p)
{
	أرجع sched_idle_rq(rq) && !task_has_idle_policy(p);
}

ساكن صحيح ټاکنه_idle_cpu(صحيح cpu, بنية task_struct *p)
{
	أرجع شته_idle_cpu(cpu) ||
	       ټاکنه_sched_idle_rq(cpu_rq(cpu), p);
}

ساكن فراغ
requeue_delayed_entity(بنية cfs_rq *cfs_rq, بنية sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	إذا (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		إذا (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		ځای_پرځای_کړه_entity(cfs_rq, se, 0);
		إذا (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_بارول_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

ساكن بینښه اوږد کتار_ته_ورزیاتول_hierarchy(بنية task_struct *p, صحيح flags)
{
	بینښه اوږد وزن = NICE_0_بارول;
	صحيح task_new = !(flags & کتار_ته_ورزیاتول_WAKEUP);
	بنية sched_entity *se = &p->se;
	صحيح h_nr_idle = task_has_idle_policy(p);
	صحيح h_nr_runnable = 1;

	إذا (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		بنية cfs_rq *cfs_rq = cfs_rq_د(se);

		update_curr(cfs_rq);

		إذا (!se->on_rq) {
			کتار_ته_ورزیاتول_entity(cfs_rq, se, flags);
		} وإلا {
			update_بارول_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_ډله(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		إذا (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		وزن = __calc_prop_weight(cfs_rq, se, وزن);

		flags = کتار_ته_ورزیاتول_WAKEUP;
	}

	أرجع وزن;
}

/* Update curr's vruntime before placing entity or updating lag */
ساكن مضمن فراغ update_curr_eevdf(بنية cfs_rq *cfs_rq)
{
	إذا (!cfs_rq->curr)
		أرجع;

	update_curr(cfs_rq_د(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
ساكن فراغ
کتار_ته_ورزیاتول_task_fair(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	صحيح rq_h_nr_queued = rq->cfs.h_nr_queued;
	صحيح task_new = !(flags & کتار_ته_ورزیاتول_WAKEUP);
	بنية sched_entity *se = &p->se;
	بنية cfs_rq *cfs_rq = &rq->cfs;
	بینښه اوږد وزن;
	bool curr;

	إذا (task_is_throttled(p) && کتار_ته_ورزیاتول_throttled_task(p))
		أرجع;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	إذا (!p->se.sched_delayed || (flags & کتار_ته_ورزیاتول_DELAYED))
		util_est_کتار_ته_ورزیاتول(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	إذا (flags & کتار_ته_ورزیاتول_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		أرجع;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	إذا (p->دننه_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	إذا (curr)
		ځای_پرځای_کړه_entity(cfs_rq, se, flags);

	إذا (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	وزن = کتار_ته_ورزیاتول_hierarchy(p, flags);

	إذا (!curr) {
		reweight_eevdf(cfs_rq, se, وزن, false);
		ځای_پرځای_کړه_entity(cfs_rq, se, flags | کتار_ته_ورزیاتول_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	إذا (!rq_h_nr_queued && rq->cfs.h_nr_queued)
		dl_server_پیل(&rq->fair_server);

	/* At this point se is NULL and we are at root level*/
	جمع_کړه_nr_running(rq, 1);

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
	إذا (!task_new)
		check_update_overutilized_حالت(rq);

	assert_list_ونې_پاڼه_cfs_rq(rq);

	hrtick_update(rq);
}

ساكن فراغ له_کتار_ایستل_hierarchy(بنية task_struct *p, صحيح flags)
{
	بنية sched_entity *se = &p->se;
	bool task_sleep = flags & له_کتار_ایستل_SLEEP;
	bool task_delayed = flags & له_کتار_ایستل_DELAYED;
	bool task_throttled = flags & له_کتار_ایستل_THROTTLE;
	صحيح h_nr_runnable = 0;
	صحيح h_nr_idle = task_has_idle_policy(p);
	bool له_کتار_ایستل = true;

	إذا (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		بنية cfs_rq *cfs_rq = cfs_rq_د(se);

		update_curr(cfs_rq);

		إذا (له_کتار_ایستل) {
			له_کتار_ایستل_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			إذا (cfs_rq->بارول.وزن)
				له_کتار_ایستل = false;
		} وإلا {
			update_بارول_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_ډله(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		إذا (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		إذا (throttled_hierarchy(cfs_rq) && task_throttled)
			ریکارډ_throttle_clock(cfs_rq);

		flags |= له_کتار_ایستل_SLEEP;
		flags &= ~(له_کتار_ایستل_DELAYED | له_کتار_ایستل_SPECIAL);
	}
}

/*
 * The part of dequeue_task_fair() that is needed to dequeue delayed tasks.
 *
 * Returns:
 *   true  - dequeued
 *   false - delayed
 */
ساكن bool __dequeue_task(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	بنية sched_entity *se = &p->se;
	بنية cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & له_کتار_ایستل_SLEEP;
	bool task_delayed = flags & له_کتار_ایستل_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	إذا (flags & له_کتار_ایستل_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} وإلا {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		إذا (flags & (له_کتار_ایستل_SPECIAL | له_کتار_ایستل_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		إذا (sched_feat(DELAY_له_کتار_ایستل) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_بارول_avg(cfs_rq_د(se), se, UPDATE_UTIL_EST);
			سېټ_delayed(se);
			أرجع false;
		}
	}

	له_کتار_ایستل_hierarchy(p, flags);

	إذا (sched_feat(ځای_پرځای_کړه_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	إذا (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	إذا (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->بل_balance = jiffies;

	إذا (task_delayed) {
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

	أرجع true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
ساكن bool له_کتار_ایستل_task_fair(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	إذا (task_is_throttled(p)) {
		له_کتار_ایستل_throttled_task(p, flags);
		أرجع true;
	}

	إذا (!p->se.sched_delayed)
		util_est_له_کتار_ایستل(&rq->cfs, p);

	إذا (!__dequeue_task(rq, p, flags))
		أرجع false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	أرجع true;
}

ساكن مضمن بینښه صحيح cfs_h_nr_delayed(بنية rq *rq)
{
	أرجع (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
ساكن DEFINE_فی_CPU(cpumask_var_t, بارول_balance_ماسک);
ساكن DEFINE_فی_CPU(cpumask_var_t, select_rq_ماسک);
ساكن DEFINE_فی_CPU(cpumask_var_t, should_we_balance_tmpmask);

#که_تعریف_شوی CONFIG_NO_HZ_COMMON

ساكن بنية {
	cpumask_var_t idle_cpus_ماسک;
	صحيح has_blocked_بارول;		/* Idle CPUS has blocked load */
	صحيح needs_update;		/* Newly idle CPUs need their next_balance collated */
	بینښه اوږد بل_balance;     /* in jiffy units */
	بینښه اوږد بل_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#د_که_پای /* CONFIG_NO_HZ_COMMON */

ساكن بینښه اوږد cpu_بارول(بنية rq *rq)
{
	أرجع cfs_rq_بارول_avg(&rq->cfs);
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
ساكن بینښه اوږد cpu_بارول_without(بنية rq *rq, بنية task_struct *p)
{
	بنية cfs_rq *cfs_rq;
	بینښه صحيح بارول;

	/* Task has no contribution or is new */
	إذا (cpu_د(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.وروستی_update_وخت))
		أرجع cpu_بارول(rq);

	cfs_rq = &rq->cfs;
	بارول = READ_ONCE(cfs_rq->avg.بارول_avg);

	/* Discount task's util from CPU's util */
	lsub_مثبت(&بارول, task_h_بارول(p));

	أرجع بارول;
}

ساكن بینښه اوږد cpu_runnable(بنية rq *rq)
{
	أرجع cfs_rq_runnable_avg(&rq->cfs);
}

ساكن بینښه اوږد cpu_runnable_without(بنية rq *rq, بنية task_struct *p)
{
	بنية cfs_rq *cfs_rq;
	بینښه صحيح runnable;

	/* Task has no contribution or is new */
	إذا (cpu_د(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.وروستی_update_وخت))
		أرجع cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_مثبت(&runnable, p->se.avg.runnable_avg);

	أرجع runnable;
}

ساكن بینښه اوږد ظرفیت_د(صحيح cpu)
{
	أرجع cpu_rq(cpu)->cpu_ظرفیت;
}

ساكن فراغ ریکارډ_wakee(بنية task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	إذا (وخت_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	إذا (current->وروستی_wakee != p) {
		current->وروستی_wakee = p;
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
ساكن صحيح wake_wide(بنية task_struct *p)
{
	بینښه صحيح master = current->wakee_flips;
	بینښه صحيح slave = p->wakee_flips;
	صحيح ضریب = __this_cpu_read(sd_llc_اندازه);

	إذا (master < slave)
		بدلون(master, slave);
	إذا (slave < ضریب || master < slave * ضریب)
		أرجع 0;
	أرجع 1;
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
ساكن صحيح
wake_affine_idle(صحيح this_cpu, صحيح prev_cpu, صحيح sync)
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
	إذا (شته_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, prev_cpu))
		أرجع شته_idle_cpu(prev_cpu) ? prev_cpu : this_cpu;

	إذا (sync) {
		بنية rq *rq = cpu_rq(this_cpu);

		إذا ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			أرجع this_cpu;
	}

	إذا (شته_idle_cpu(prev_cpu))
		أرجع prev_cpu;

	أرجع nr_cpumask_بټونه;
}

ساكن صحيح
wake_affine_وزن(بنية sched_domain *sd, بنية task_struct *p,
		   صحيح this_cpu, صحيح prev_cpu, صحيح sync)
{
	s64 this_eff_بارول, prev_eff_بارول;
	بینښه اوږد task_بارول;

	this_eff_بارول = cpu_بارول(cpu_rq(this_cpu));

	إذا (sync) {
		بینښه اوږد current_بارول = task_h_بارول(current);

		إذا (current_بارول > this_eff_بارول)
			أرجع this_cpu;

		this_eff_بارول -= current_بارول;
	}

	task_بارول = task_h_بارول(p);

	this_eff_بارول += task_بارول;
	إذا (sched_feat(WA_BIAS))
		this_eff_بارول *= 100;
	this_eff_بارول *= ظرفیت_د(prev_cpu);

	prev_eff_بارول = cpu_بارول(cpu_rq(prev_cpu));
	prev_eff_بارول -= task_بارول;
	إذا (sched_feat(WA_BIAS))
		prev_eff_بارول *= 100 + (sd->imbalance_pct - 100) / 2;
	prev_eff_بارول *= ظرفیت_د(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	إذا (sync)
		prev_eff_بارول += 1;

	أرجع this_eff_بارول < prev_eff_بارول ? this_cpu : nr_cpumask_بټونه;
}

ساكن صحيح wake_affine(بنية sched_domain *sd, بنية task_struct *p,
		       صحيح this_cpu, صحيح prev_cpu, صحيح sync)
{
	صحيح موخه = nr_cpumask_بټونه;

	إذا (sched_feat(WA_IDLE))
		موخه = wake_affine_idle(this_cpu, prev_cpu, sync);

	إذا (sched_feat(WA_وزن) && موخه == nr_cpumask_بټونه)
		موخه = wake_affine_وزن(sd, p, this_cpu, prev_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	إذا (موخه != this_cpu)
		أرجع prev_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	أرجع موخه;
}

ساكن بنية sched_ډله *
sched_balance_پیدا_کړه_dst_ډله(بنية sched_domain *sd, بنية task_struct *p, صحيح this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
ساكن صحيح
sched_balance_پیدا_کړه_dst_ډله_cpu(بنية sched_ډله *ډله, بنية task_struct *p, صحيح this_cpu)
{
	بینښه اوږد بارول, min_بارول = ULONG_MAX;
	u64 min_exit_latency = U64_MAX;
	بینښه صحيح nr_candidates = 0;
	صحيح لږترلږه_loaded_cpu = this_cpu;
	صحيح shallowest_idle_cpu = -1;
	صحيح i;

	/* Check if we have any choice: */
	إذا (ډله->ډله_وزن == 1)
		أرجع cpumask_لومړی(sched_ډله_پراختیا(ډله));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_ډله_پراختیا(ډله), p->cpus_ptr) {
		بنية rq *rq = cpu_rq(i);

		إذا (!sched_core_cookie_سمون(rq, p))
			استمر;

		إذا (ټاکنه_sched_idle_rq(rq, p))
			أرجع i;

		إذا (شته_idle_cpu(i)) {
			بنية cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			إذا (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} وإلا إذا (exit_latency == min_exit_latency) {
				nr_candidates++;
				إذا (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} وإلا إذا (shallowest_idle_cpu == -1) {
			بارول = cpu_بارول(cpu_rq(i));
			إذا (بارول < min_بارول) {
				min_بارول = بارول;
				لږترلږه_loaded_cpu = i;
			}
		}
	}

	أرجع shallowest_idle_cpu != -1 ? shallowest_idle_cpu : لږترلږه_loaded_cpu;
}

ساكن مضمن صحيح sched_balance_پیدا_کړه_dst_cpu(بنية sched_domain *sd, بنية task_struct *p,
				  صحيح cpu, صحيح prev_cpu, صحيح sd_flag)
{
	صحيح new_cpu = cpu;

	إذا (!cpumask_intersects(sched_domain_پراختیا(sd), p->cpus_ptr))
		أرجع prev_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	إذا (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_بارول_avg(&p->se);

	طالما (sd) {
		بنية sched_ډله *ډله;
		بنية sched_domain *tmp;
		صحيح وزن;

		إذا (!(sd->flags & sd_flag)) {
			sd = sd->ماشوم;
			استمر;
		}

		ډله = sched_balance_پیدا_کړه_dst_ډله(sd, p, cpu);
		إذا (!ډله) {
			sd = sd->ماشوم;
			استمر;
		}

		new_cpu = sched_balance_پیدا_کړه_dst_ډله_cpu(ډله, p, cpu);
		إذا (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->ماشوم;
			استمر;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		وزن = sd->پراختیا_وزن;
		sd = عدم;
		for_each_domain(cpu, tmp) {
			إذا (وزن <= tmp->پراختیا_وزن)
				اكسر;
			إذا (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	أرجع new_cpu;
}

ساكن مضمن صحيح __select_idle_cpu(صحيح cpu, بنية task_struct *p)
{
	إذا (ټاکنه_idle_cpu(cpu, p) && sched_cpu_cookie_سمون(cpu_rq(cpu), p))
		أرجع cpu;

	أرجع -1;
}

DEFINE_STATIC_کلی_FALSE(sched_smt_present);
EXPORT_نښه_GPL(sched_smt_present);

ساكن مضمن فراغ سېټ_idle_cores(صحيح cpu, صحيح val)
{
	بنية sched_domain_shared *sds;

	sds = rcu_dereference_all(فی_cpu(sd_balance_shared, cpu));
	إذا (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

ساكن مضمن bool test_idle_cores(صحيح cpu)
{
	بنية sched_domain_shared *sds;

	sds = rcu_dereference_all(فی_cpu(sd_balance_shared, cpu));
	إذا (sds)
		أرجع READ_ONCE(sds->has_idle_cores);

	أرجع false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
ساكن مضمن صحيح select_idle_smt_cpu(بنية task_struct *p, صحيح cpu)
{
	بنية sched_domain *sd;
	صحيح غوره = cpu;
	صحيح sibling;

	إذا (!sched_smt_فعال())
		أرجع cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	إذا (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		أرجع cpu;

	for_each_cpu_and(sibling, sched_domain_پراختیا(sd), p->cpus_ptr) {
		إذا (sibling == غوره || !ټاکنه_idle_cpu(sibling, p))
			استمر;

		إذا (sched_asym_prefer(sibling, غوره))
			غوره = sibling;
	}

	أرجع غوره;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
فراغ __update_idle_core(بنية rq *rq)
{
	صحيح core = cpu_د(rq);
	صحيح cpu;

	rcu_read_قلف();
	إذا (test_idle_cores(core))
		اذهب unlock;

	for_each_cpu(cpu, cpu_smt_ماسک(core)) {
		إذا (cpu == core)
			استمر;

		إذا (!شته_idle_cpu(cpu))
			اذهب unlock;
	}

	سېټ_idle_cores(core, 1);
unlock:
	rcu_read_unlock();
}

/*
 * Scan the entire LLC domain for idle cores; this dynamically switches off if
 * there are no idle cores left in the system; tracked through
 * sd_balance_shared->has_idle_cores and enabled through update_idle_core()
 * above.
 */
ساكن صحيح select_idle_core(بنية task_struct *p, صحيح core, بنية cpumask *cpus, صحيح *idle_cpu)
{
	bool idle = true;
	صحيح cpu;

	for_each_cpu(cpu, cpu_smt_ماسک(core)) {
		إذا (!شته_idle_cpu(cpu)) {
			idle = false;
			إذا (*idle_cpu == -1) {
				إذا (ټاکنه_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					اكسر;
				}
				استمر;
			}
			اكسر;
		}
		إذا (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	إذا (idle)
		أرجع core;

	cpumask_andnot(cpus, cpus, cpu_smt_ماسک(core));
	أرجع -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
ساكن صحيح select_idle_smt(بنية task_struct *p, بنية sched_domain *sd, صحيح موخه)
{
	صحيح cpu;

	for_each_cpu_and(cpu, cpu_smt_ماسک(موخه), p->cpus_ptr) {
		إذا (cpu == موخه)
			استمر;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		إذا (!cpumask_test_cpu(cpu, sched_domain_پراختیا(sd)))
			استمر;
		إذا (ټاکنه_idle_cpu(cpu, p))
			أرجع cpu;
	}

	أرجع -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
ساكن صحيح select_idle_cpu(بنية task_struct *p, بنية sched_domain *sd, bool has_idle_core, صحيح موخه)
{
	بنية cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ماسک);
	صحيح i, cpu, idle_cpu = -1, nr = لوی_عدد;

	إذا (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Increment because !--nr is the condition to stop scan.
		 *
		 * Since "sd" is "sd_llc" for target CPU dereferenced in the
		 * caller, it is safe to directly dereference "sd->shared".
		 * Topology bits always ensure it assigned for "sd_llc" abd it
		 * cannot disappear as long as we have a RCU protected
		 * reference to one the associated "sd" here.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_لټون) + 1;
		/* overloaded LLC is unlikely to have idle cpu/core */
		إذا (nr == 1)
			أرجع -1;
	}

	إذا (!cpumask_and(cpus, sched_domain_پراختیا(sd), p->cpus_ptr))
		أرجع -1;

	إذا (static_branch_unlikely(&sched_cluster_فعال)) {
		بنية sched_ډله *sg = sd->groups;

		إذا (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_ډله_پراختیا(sg), موخه + 1) {
				إذا (!cpumask_test_cpu(cpu, cpus))
					استمر;

				إذا (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					إذا ((بینښه صحيح)i < nr_cpumask_بټونه)
						أرجع i;
				} وإلا {
					إذا (--nr <= 0)
						أرجع -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					إذا ((بینښه صحيح)idle_cpu < nr_cpumask_بټونه)
						أرجع idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_ډله_پراختیا(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, موخه + 1) {
		إذا (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			إذا ((بینښه صحيح)i < nr_cpumask_بټونه)
				أرجع i;

		} وإلا {
			إذا (--nr <= 0)
				أرجع -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			إذا ((بینښه صحيح)idle_cpu < nr_cpumask_بټونه)
				اكسر;
		}
	}

	إذا (has_idle_core)
		سېټ_idle_cores(موخه, false);

	أرجع idle_cpu;
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
تعداد asym_ځاییږي_state {
	ASYM_IDLE_UCLAMP_MISFIT = -4,
	ASYM_IDLE_COMPLETE_MISFIT,
	ASYM_IDLE_THREAD_ځاییږي,
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
ساكن صحيح
select_idle_ظرفیت(بنية task_struct *p, بنية sched_domain *sd, صحيح موخه)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_فعال() && test_idle_cores(موخه);
	بینښه اوږد task_util, util_min, util_max, غوره_cap = 0;
	صحيح ځاییږي, غوره_ځاییږي = ASYM_IDLE_THREAD_MISFIT;
	صحيح cpu, غوره_cpu = -1;
	بنية cpumask *cpus;
	صحيح nr = لوی_عدد;

	cpus = this_cpu_cpumask_var_ptr(select_rq_ماسک);
	cpumask_and(cpus, sched_domain_پراختیا(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_ارزښت(p, UCLAMP_MIN);
	util_max = uclamp_eff_ارزښت(p, UCLAMP_MAX);

	إذا (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_لټون) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		إذا (nr == 1)
			أرجع -1;
	}

	for_each_cpu_wrap(cpu, cpus, موخه) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		بینښه اوږد cpu_cap = ظرفیت_د(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		إذا (!has_idle_core && --nr <= 0)
			أرجع غوره_cpu;

		إذا (!ټاکنه_idle_cpu(cpu, p))
			استمر;

		ځاییږي = util_ځاییږي_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		إذا (ځاییږي > 0 && preferred_core)
			أرجع cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		وإلا إذا (ځاییږي < 0)
			cpu_cap = get_actual_cpu_ظرفیت(cpu);
		/*
		 * fits > 0 implies we are not on a preferred core, but the util
		 * fits CPU capacity. Set fits to ASYM_IDLE_THREAD_FITS
		 * so the effective range becomes
		 * [ASYM_IDLE_THREAD_FITS, ASYM_IDLE_THREAD_MISFIT], where:
		 *    ASYM_IDLE_THREAD_MISFIT - does not fit
		 *    ASYM_IDLE_THREAD_UCLAMP_MISFIT - fits with the exception of UCLAMP_MIN
		 *    ASYM_IDLE_THREAD_FITS - fits with the exception of preferred_core
		 */
		وإلا إذا (ځاییږي > 0)
			ځاییږي = ASYM_IDLE_THREAD_ځاییږي;

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
		إذا (preferred_core)
			ځاییږي += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		إذا ((ځاییږي < غوره_ځاییږي) ||
		    ((ځاییږي == غوره_ځاییږي) && (cpu_cap > غوره_cap))) {
			غوره_cap = cpu_cap;
			غوره_cpu = cpu;
			غوره_ځاییږي = ځاییږي;
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
	إذا (has_idle_core && غوره_ځاییږي > ASYM_IDLE_COMPLETE_MISFIT)
		سېټ_idle_cores(موخه, false);

	أرجع غوره_cpu;
}

ساكن مضمن bool asym_ځاییږي_cpu(بینښه اوږد util,
				 بینښه اوږد util_min,
				 بینښه اوږد util_max,
				 صحيح cpu)
{
	إذا (sched_asym_cpucap_فعال()) {
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
		أرجع (!sched_smt_فعال() || is_core_idle(cpu)) &&
		       (util_ځاییږي_cpu(util, util_min, util_max, cpu) > 0);
	}

	أرجع true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
ساكن صحيح select_idle_sibling(بنية task_struct *p, صحيح prev, صحيح موخه)
{
	bool has_idle_core = false;
	بنية sched_domain *sd;
	بینښه اوږد task_util, util_min, util_max;
	صحيح i, recent_کارول_شوی_cpu, prev_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	إذا (sched_asym_cpucap_فعال()) {
		sync_entity_بارول_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_ارزښت(p, UCLAMP_MIN);
		util_max = uclamp_eff_ارزښت(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	إذا (ټاکنه_idle_cpu(موخه, p) &&
	    asym_ځاییږي_cpu(task_util, util_min, util_max, موخه))
		اذهب select_smt_لومړیتوب;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	إذا (prev != موخه && cpus_share_cache(prev, موخه) &&
	    ټاکنه_idle_cpu(prev, p) &&
	    asym_ځاییږي_cpu(task_util, util_min, util_max, prev)) {

		إذا (!static_branch_unlikely(&sched_cluster_فعال) ||
		    cpus_share_resources(prev, موخه)) {
			موخه = prev;
			اذهب select_smt_لومړیتوب;
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
	إذا (is_فی_cpu_kthread(current) &&
	    دننه_task() &&
	    prev == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_ځاییږي_cpu(task_util, util_min, util_max, prev)) {
		موخه = prev;
		اذهب select_smt_لومړیتوب;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_کارول_شوی_cpu = p->recent_کارول_شوی_cpu;
	p->recent_کارول_شوی_cpu = prev;
	إذا (recent_کارول_شوی_cpu != prev &&
	    recent_کارول_شوی_cpu != موخه &&
	    cpus_share_cache(recent_کارول_شوی_cpu, موخه) &&
	    ټاکنه_idle_cpu(recent_کارول_شوی_cpu, p) &&
	    cpumask_test_cpu(recent_کارول_شوی_cpu, p->cpus_ptr) &&
	    asym_ځاییږي_cpu(task_util, util_min, util_max, recent_کارول_شوی_cpu)) {

		إذا (!static_branch_unlikely(&sched_cluster_فعال) ||
		    cpus_share_resources(recent_کارول_شوی_cpu, موخه)) {
			موخه = recent_کارول_شوی_cpu;
			اذهب select_smt_لومړیتوب;
		}

	} وإلا {
		recent_کارول_شوی_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	إذا (sched_asym_cpucap_فعال()) {
		sd = rcu_dereference_all(فی_cpu(sd_asym_cpucapacity, موخه));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		إذا (sd) {
			i = select_idle_ظرفیت(p, sd, موخه);
			إذا ((بینښه صحيح)i < nr_cpumask_بټونه) {
				موخه = i;
				اذهب select_smt_لومړیتوب;
			}
			أرجع موخه;
		}
	}

	sd = rcu_dereference_all(فی_cpu(sd_llc, موخه));
	إذا (!sd)
		أرجع موخه;

	إذا (sched_smt_فعال()) {
		has_idle_core = test_idle_cores(موخه);

		إذا (!has_idle_core && cpus_share_cache(prev, موخه)) {
			i = select_idle_smt(p, sd, prev);
			إذا ((بینښه صحيح)i < nr_cpumask_بټونه) {
				موخه = i;
				اذهب select_smt_لومړیتوب;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, موخه);
	إذا ((بینښه صحيح)i < nr_cpumask_بټونه) {
		موخه = i;
		اذهب select_smt_لومړیتوب;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	إذا ((بینښه صحيح)prev_aff < nr_cpumask_بټونه) {
		موخه = prev_aff;
		اذهب select_smt_لومړیتوب;
	}
	إذا ((بینښه صحيح)recent_کارول_شوی_cpu < nr_cpumask_بټونه) {
		موخه = recent_کارول_شوی_cpu;
		اذهب select_smt_لومړیتوب;
	}

	أرجع موخه;

select_smt_لومړیتوب:
	أرجع select_idle_smt_cpu(p, موخه);
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
ساكن بینښه اوږد
cpu_util(صحيح cpu, بنية task_struct *p, صحيح dst_cpu, صحيح boost)
{
	bool جمع_کړه_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	بنية cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	بینښه اوږد util = READ_ONCE(cfs_rq->avg.util_avg);
	بینښه اوږد runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	إذا (جمع_کړه_task)
		util += task_util(p);
	وإلا إذا (sub_task)
		lsub_مثبت(&util, task_util(p));

	إذا (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		إذا (جمع_کړه_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		وإلا إذا (sub_task)
			lsub_مثبت(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	إذا (sched_feat(UTIL_EST)) {
		بینښه اوږد util_est;

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
		إذا (dst_cpu == cpu)
			util_est += _task_util_est(p);
		وإلا إذا (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_مثبت(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	أرجع min(util, arch_scale_cpu_ظرفیت(cpu));
}

بینښه اوږد cpu_util_cfs(صحيح cpu)
{
	أرجع cpu_util(cpu, عدم, -1, 0);
}

بینښه اوږد cpu_util_cfs_boost(صحيح cpu)
{
	أرجع cpu_util(cpu, عدم, -1, 1);
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
ساكن بینښه اوږد cpu_util_without(صحيح cpu, بنية task_struct *p)
{
	/* Task has no contribution or is new */
	إذا (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.وروستی_update_وخت))
		p = عدم;

	أرجع cpu_util(cpu, p, -1, 0);
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
بینښه اوږد effective_cpu_util(صحيح cpu, بینښه اوږد util_cfs,
				 بینښه اوږد *min,
				 بینښه اوږد *max)
{
	بینښه اوږد util, irq, scale;
	بنية rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_ظرفیت(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	إذا (unlikely(irq >= scale)) {
		إذا (min)
			*min = scale;
		إذا (max)
			*max = scale;
		أرجع scale;
	}

	إذا (min) {
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
		إذا (!uclamp_is_کارول_شوی() && rt_rq_is_runnable(&rq->rt))
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
	إذا (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	إذا (util >= scale)
		أرجع scale;

	/*
	 * There is still idle time; further improve the number by using the
	 * IRQ metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              max - irq
	 *   U' = irq + --------- * U
	 *                 max
	 */
	util = scale_irq_ظرفیت(util, irq, scale);
	util += irq;

	أرجع min(scale, util);
}

بینښه اوږد sched_cpu_util(صحيح cpu)
{
	أرجع effective_cpu_util(cpu, cpu_util_cfs(cpu), عدم, عدم);
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
بنية energy_env {
	بینښه اوږد task_busy_وخت;
	بینښه اوږد pd_busy_وخت;
	بینښه اوږد cpu_cap;
	بینښه اوږد pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
ساكن مضمن فراغ eenv_task_busy_وخت(بنية energy_env *eenv,
				       بنية task_struct *p, صحيح prev_cpu)
{
	بینښه اوږد busy_وخت, max_cap = arch_scale_cpu_ظرفیت(prev_cpu);
	بینښه اوږد irq = cpu_util_irq(cpu_rq(prev_cpu));

	إذا (unlikely(irq >= max_cap))
		busy_وخت = max_cap;
	وإلا
		busy_وخت = scale_irq_ظرفیت(task_util_est(p), irq, max_cap);

	eenv->task_busy_وخت = busy_وخت;
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
ساكن مضمن فراغ eenv_pd_busy_وخت(بنية energy_env *eenv,
				     بنية cpumask *pd_cpus,
				     بنية task_struct *p)
{
	بینښه اوږد busy_وخت = 0;
	صحيح cpu;

	for_each_cpu(cpu, pd_cpus) {
		بینښه اوږد util = cpu_util(cpu, p, -1, 0);

		busy_وخت += effective_cpu_util(cpu, util, عدم, عدم);
	}

	eenv->pd_busy_وخت = min(eenv->pd_cap, busy_وخت);
}

/*
 * Compute the maximum utilization for compute_energy() when the task @p
 * is placed on the cpu @dst_cpu.
 *
 * Returns the maximum utilization among @eenv->cpus. This utilization can't
 * exceed @eenv->cpu_cap.
 */
ساكن مضمن بینښه اوږد
eenv_pd_max_util(بنية energy_env *eenv, بنية cpumask *pd_cpus,
		 بنية task_struct *p, صحيح dst_cpu)
{
	بینښه اوږد max_util = 0;
	صحيح cpu;

	for_each_cpu(cpu, pd_cpus) {
		بنية task_struct *tsk = (cpu == dst_cpu) ? p : عدم;
		بینښه اوږد util = cpu_util(cpu, p, dst_cpu, 1);
		بینښه اوږد eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		إذا (tsk && uclamp_is_کارول_شوی()) {
			min = max(min, uclamp_eff_ارزښت(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			إذا (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_ارزښت(p, UCLAMP_MAX);
			وإلا
				max = max(max, uclamp_eff_ارزښت(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	أرجع min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
ساكن مضمن بینښه اوږد
compute_energy(بنية energy_env *eenv, بنية perf_domain *pd,
	       بنية cpumask *pd_cpus, بنية task_struct *p, صحيح dst_cpu)
{
	بینښه اوږد max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	بینښه اوږد busy_وخت = eenv->pd_busy_وخت;
	بینښه اوږد energy;

	إذا (dst_cpu >= 0)
		busy_وخت = min(eenv->pd_cap, busy_وخت + eenv->task_busy_وخت);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_وخت, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_وخت);

	أرجع energy;
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
ساكن صحيح پیدا_کړه_energy_efficient_cpu(بنية task_struct *p, صحيح prev_cpu)
{
	بنية cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ماسک);
	بینښه اوږد prev_delta = ULONG_MAX, غوره_delta = ULONG_MAX;
	بینښه اوږد p_util_min = uclamp_is_کارول_شوی() ? uclamp_eff_ارزښت(p, UCLAMP_MIN) : 0;
	بینښه اوږد p_util_max = uclamp_is_کارول_شوی() ? uclamp_eff_ارزښت(p, UCLAMP_MAX) : 1024;
	بنية ریښه_domain *rd = this_rq()->rd;
	صحيح cpu, غوره_energy_cpu, موخه = -1;
	صحيح prev_ځاییږي = -1, غوره_ځاییږي = -1;
	بینښه اوږد غوره_actual_cap = 0;
	بینښه اوږد prev_actual_cap = 0;
	بنية sched_domain *sd;
	بنية perf_domain *pd;
	بنية energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	إذا (!pd)
		أرجع موخه;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	طالما (sd && !cpumask_test_cpu(prev_cpu, sched_domain_پراختیا(sd)))
		sd = sd->پلار;
	إذا (!sd)
		أرجع موخه;

	موخه = prev_cpu;

	sync_entity_بارول_avg(&p->se);
	إذا (!task_util_est(p) && p_util_min == 0)
		أرجع موخه;

	eenv_task_busy_وخت(&eenv, p, prev_cpu);

	لكل (; pd; pd = pd->بل) {
		بینښه اوږد util_min = p_util_min, util_max = p_util_max;
		بینښه اوږد cpu_cap, cpu_actual_cap, util;
		اوږد prev_اضافي_cap = -1, max_اضافي_cap = -1;
		بینښه اوږد rq_util_min, rq_util_max;
		بینښه اوږد cur_delta, بنسټ_energy;
		صحيح max_اضافي_cap_cpu = -1;
		صحيح ځاییږي, max_ځاییږي = -1;

		إذا (!cpumask_and(cpus, perf_domain_پراختیا(pd), cpu_online_ماسک))
			استمر;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_لومړی(cpus);
		cpu_actual_cap = get_actual_cpu_ظرفیت(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			بنية rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			إذا (!cpumask_test_cpu(cpu, sched_domain_پراختیا(sd)))
				استمر;

			إذا (!cpumask_test_cpu(cpu, p->cpus_ptr))
				استمر;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = ظرفیت_د(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			إذا (uclamp_is_کارول_شوی() && !uclamp_rq_is_idle(rq)) {
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

			ځاییږي = util_ځاییږي_cpu(util, util_min, util_max, cpu);
			إذا (!ځاییږي)
				استمر;

			lsub_مثبت(&cpu_cap, util);

			إذا (cpu == prev_cpu) {
				/* Always use prev_cpu as a candidate. */
				prev_اضافي_cap = cpu_cap;
				prev_ځاییږي = ځاییږي;
			} وإلا إذا ((ځاییږي > max_ځاییږي) ||
				   ((ځاییږي == max_ځاییږي) && ((اوږد)cpu_cap > max_اضافي_cap))) {
				/*
				 * Find the CPU with the maximum spare capacity
				 * among the remaining CPUs in the performance
				 * domain.
				 */
				max_اضافي_cap = cpu_cap;
				max_اضافي_cap_cpu = cpu;
				max_ځاییږي = ځاییږي;
			}
		}

		إذا (max_اضافي_cap_cpu < 0 && prev_اضافي_cap < 0)
			استمر;

		eenv_pd_busy_وخت(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		بنسټ_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		إذا (prev_اضافي_cap > -1) {
			prev_delta = compute_energy(&eenv, pd, cpus, p,
						    prev_cpu);
			/* CPU utilization has changed */
			إذا (prev_delta < بنسټ_energy)
				أرجع موخه;
			prev_delta -= بنسټ_energy;
			prev_actual_cap = cpu_actual_cap;
			غوره_delta = min(غوره_delta, prev_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		إذا (max_اضافي_cap_cpu >= 0 && max_اضافي_cap > prev_اضافي_cap) {
			/* Current best energy cpu fits better */
			إذا (max_ځاییږي < غوره_ځاییږي)
				استمر;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			إذا ((max_ځاییږي < 0) &&
			    (cpu_actual_cap <= غوره_actual_cap))
				استمر;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_اضافي_cap_cpu);
			/* CPU utilization has changed */
			إذا (cur_delta < بنسټ_energy)
				أرجع موخه;
			cur_delta -= بنسټ_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			إذا ((max_ځاییږي > 0) && (غوره_ځاییږي > 0) &&
			    (cur_delta >= غوره_delta))
				استمر;

			غوره_delta = cur_delta;
			غوره_energy_cpu = max_اضافي_cap_cpu;
			غوره_ځاییږي = max_ځاییږي;
			غوره_actual_cap = cpu_actual_cap;
		}
	}

	إذا ((غوره_ځاییږي > prev_ځاییږي) ||
	    ((غوره_ځاییږي > 0) && (غوره_delta < prev_delta)) ||
	    ((غوره_ځاییږي < 0) && (غوره_actual_cap > prev_actual_cap)))
		موخه = غوره_energy_cpu;

	أرجع موخه;
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
ساكن صحيح
select_task_rq_fair(بنية task_struct *p, صحيح prev_cpu, صحيح wake_flags)
{
	صحيح sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	بنية sched_domain *tmp, *sd = عدم;
	صحيح cpu = smp_processor_id();
	صحيح new_cpu = prev_cpu;
	صحيح want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	صحيح sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_قلف);
	إذا (wake_flags & WF_TTWU) {
		ریکارډ_wakee(p);

		إذا ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			أرجع cpu;

		إذا (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = پیدا_کړه_energy_efficient_cpu(p, prev_cpu);
			إذا (new_cpu >= 0)
				أرجع new_cpu;
			new_cpu = prev_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		إذا (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(prev_cpu, sched_domain_پراختیا(tmp))) {
			إذا (cpu != prev_cpu)
				new_cpu = wake_affine(tmp, p, cpu, prev_cpu, sync);

			sd = عدم; /* Prefer wake_affine over balance flags */
			اكسر;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		إذا (tmp->flags & sd_flag)
			sd = tmp;
		وإلا إذا (!want_affine)
			اكسر;
	}

	/* Slow path */
	إذا (unlikely(sd)) {
		new_cpu = sched_balance_پیدا_کړه_dst_cpu(sd, p, cpu, prev_cpu, sd_flag);
		أرجع select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	إذا (wake_flags & WF_TTWU)
		أرجع select_idle_sibling(p, prev_cpu, new_cpu);

	أرجع new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
ساكن فراغ migrate_task_rq_fair(بنية task_struct *p, صحيح new_cpu)
{
	بنية sched_entity *se = &p->se;

	إذا (!task_on_rq_migrating(p)) {
		remove_entity_بارول_avg(se);

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
	se->avg.وروستی_update_وخت = 0;

	update_لټون_period(p, new_cpu);
}

ساكن فراغ task_dead_fair(بنية task_struct *p)
{
	بنية sched_entity *se = &p->se;
	remove_entity_بارول_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
ساكن فراغ سېټ_task_max_allowed_ظرفیت(بنية task_struct *p)
{
	بنية asym_cap_معلومات *ثبت;

	إذا (!sched_asym_cpucap_فعال())
		أرجع;

	rcu_read_قلف();
	list_for_each_ثبت_rcu(ثبت, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_ظرفیت_پراختیا(ثبت);
		إذا (!cpumask_intersects(p->cpus_ptr, cpumask))
			استمر;

		p->max_allowed_ظرفیت = ثبت->ظرفیت;
		اكسر;
	}
	rcu_read_unlock();
}

ساكن فراغ سېټ_cpus_allowed_fair(بنية task_struct *p, بنية affinity_context *ctx)
{
	سېټ_cpus_allowed_common(p, ctx);
	سېټ_task_max_allowed_ظرفیت(p);
}

تعداد preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

ساكن مضمن bool سېټ_preempt_buddy(بنية cfs_rq *cfs_rq, بنية sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	إذا (cfs_rq->بل && entity_before(cfs_rq->بل, pse))
		أرجع false;

	سېټ_بل_buddy(cfs_rq, pse);
	أرجع true;
}

ساكن مضمن bool سېټ_short_buddy(بنية cfs_rq *cfs_rq, بنية sched_entity *pse)
{
	إذا (cfs_rq->بل) {
		إذا (cfs_rq->بل->ټوټه < pse->ټوټه)
			أرجع false;

		إذا (cfs_rq->بل->ټوټه == pse->ټوټه &&
		    entity_before(cfs_rq->بل, pse))
			أرجع false;
	}

	سېټ_بل_buddy(cfs_rq, pse);
	أرجع true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
ساكن مضمن تعداد preempt_wakeup_action
preempt_sync(بنية rq *rq, صحيح wake_flags,
	     بنية sched_entity *pse, بنية sched_entity *se)
{
	u64 پوله, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	پوله = sysctl_sched_migration_لګښت;
	delta = rq_clock_task(rq) - se->exec_پیل;
	إذا ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	إذا (wake_flags & WF_RQ_SELECTED)
		پوله >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	إذا (entity_before(pse, se) && delta >= پوله)
		أرجع PREEMPT_WAKEUP_RESCHED;

	أرجع PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ساكن فراغ wakeup_preempt_fair(بنية rq *rq, بنية task_struct *p, صحيح wake_flags)
{
	تعداد preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	بنية task_struct *donor = rq->donor;
	بنية sched_entity *nse, *se = &donor->se, *pse = &p->se;
	بنية cfs_rq *cfs_rq = &rq->cfs;
	صحيح cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	إذا (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		أرجع;

	إذا (unlikely(se == pse))
		أرجع;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	إذا (task_is_throttled(p))
		أرجع;

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
	إذا (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		أرجع;

	إذا (!sched_feat(WAKEUP_PREEMPTION))
		أرجع;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	إذا (cse_is_idle && !pse_is_idle)
		اذهب preempt;

	update_curr_fair(rq);

	إذا (cse_is_idle != pse_is_idle)
		اذهب update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	إذا (unlikely(!نورمال_policy(p->policy)))
		اذهب update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	إذا (pse->sched_delayed)
		اذهب update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	إذا (sched_feat(PREEMPT_SHORT) && (pse->ټوټه < se->ټوټه)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		اذهب pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	إذا (wake_flags & WF_FORK)
		اذهب update;

	/* Prefer picking wakee soon if appropriate. */
	إذا (sched_feat(بل_BUDDY) && سېټ_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		إذا (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	بدل (preempt_action) {
	حالة PREEMPT_WAKEUP_NONE:
		أرجع;
	حالة PREEMPT_WAKEUP_RESCHED:
		اذهب preempt;
	حالة PREEMPT_WAKEUP_SHORT:
		fallthrough;
	حالة PREEMPT_WAKEUP_PICK:
		اكسر;
	}

pick:
	إذا (cfs_rq->h_nr_queued) {
		nse = pick_بل_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		إذا (unlikely(!nse))
			اذهب pick;

		/* If @p has become the most eligible task, force preemption */
		إذا (nse == pse)
			اذهب preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	إذا (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		اذهب preempt;
update:
	إذا (sched_feat(پرلپسې_منزل_PARITY))
		update_protect_ټوټه(cfs_rq, se);

	أرجع;

preempt:
	cancel_protect_ټوټه(se);

	إذا (preempt_action == PREEMPT_WAKEUP_SHORT)
		سېټ_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

بنية task_struct *pick_task_fair(بنية rq *rq, بنية rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	بنية cfs_rq *cfs_rq = &rq->cfs;
	بنية sched_entity *se;
	بنية task_struct *p;
	صحيح new_tasks;

again:
	إذا (!cfs_rq->h_nr_queued)
		اذهب idle;

	/* Might not have done put_prev_entity() */
	إذا (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_بل_entity(rq, true);
	إذا (!se)
		اذهب again;

	p = task_د(se);
	أرجع p;

idle:
	إذا (sched_core_enabled(rq))
		أرجع عدم;

	new_tasks = sched_balance_newidle(rq, rf);
	إذا (new_tasks < 0)
		أرجع RETRY_TASK;
	إذا (new_tasks > 0)
		اذهب again;
	أرجع عدم;
}

ساكن بنية task_struct *
fair_server_pick_task(بنية sched_dl_entity *dl_se, بنية rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	أرجع pick_task_fair(dl_se->rq, rf);
}

فراغ fair_server_init(بنية rq *rq)
{
	بنية sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
ساكن فراغ put_prev_task_fair(بنية rq *rq, بنية task_struct *prev, بنية task_struct *بل)
{
	بنية sched_entity *se = &prev->se;
	بنية cfs_rq *cfs_rq = &rq->cfs;
	بنية sched_entity *nse = عدم;

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	إذا (بل && بل->sched_class == &fair_sched_class)
		nse = &بل->se;
#د_که_پای

	طالما (se) {
		cfs_rq = cfs_rq_د(se);
		إذا (!nse || cfs_rq->h_curr)
			put_prev_entity(cfs_rq, se);
#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
		إذا (nse) {
			إذا (is_same_ډله(se, nse))
				اكسر;

			صحيح d = nse->ژوروالی - se->ژوروالی;
			إذا (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = پلار_entity(nse);
				/* if nse is the deeper, do not ascend se */
				إذا (d > 0)
					استمر;
			}
		}
#د_که_پای
		se = پلار_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &prev->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = عدم;
	إذا (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
ساكن فراغ yield_task_fair(بنية rq *rq)
{
	بنية task_struct *curr = rq->donor;
	بنية sched_entity *se = &curr->se;
	بنية cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	إذا (unlikely(rq->nr_running == 1))
		أرجع;

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
	إذا (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

ساكن bool yield_منزل_task_fair(بنية rq *rq, بنية task_struct *p)
{
	بنية sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	إذا (!se->on_rq || se->sched_delayed)
		أرجع false;

	/* Tell the scheduler that we'd really like se to run next. */
	سېټ_بل_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	أرجع true;
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

ساكن بینښه اوږد __read_mostly max_بارول_balance_interval = HZ/10;

تعداد fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
تعداد ډله_type {
	/* The group has spare capacity that can be used to run more tasks.  */
	ډله_has_اضافي = 0,
	/*
	 * The group is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	ډله_fully_busy,
	/*
	 * One task doesn't fit with CPU's capacity and must be migrated to a
	 * more powerful CPU.
	 */
	ډله_misfit_task,
	/*
	 * Balance SMT group that's fully busy. Can benefit from migration
	 * a task on SMT with busy sibling to another CPU on idle core.
	 */
	ډله_smt_balance,
	/*
	 * SD_ASYM_PACKING only: One local CPU with higher capacity is available,
	 * and the task should be migrated to it instead of running on the
	 * current CPU.
	 */
	ډله_asym_packing,
	/*
	 * The tasks' affinity constraints previously prevented the scheduler
	 * from balancing the load across the system.
	 */
	ډله_imbalanced,
	/*
	 * There are tasks running on non-preferred LLC, possible to move
	 * them to their preferred LLC without creating too much imbalance.
	 * The priority of group_llc_balance is lower than that of
	 * group_overloaded and higher than that of all other group types.
	 * This is because group_llc_balance may exacerbate load imbalance.
	 * If the LLC balancing attempt fails, the nr_balance_failed
	 * mechanism will trigger other group types to rebalance the load.
	 */
	ډله_llc_balance,
	/*
	 * The CPU is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	ډله_overloaded
};

تعداد migration_type {
	migrate_بارول = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#تعریف_کړه LBF_ALL_PINNED	0x01
#تعریف_کړه LBF_NEED_BREAK	0x02
#تعریف_کړه LBF_DST_PINNED  0x04
#تعریف_کړه LBF_SOME_PINNED	0x08
#تعریف_کړه LBF_فعال_LB	0x10
#تعریف_کړه LBF_LLC_PINNED	0x20
#تعریف_کړه LBF_فعال_LB_LLC	0x40

بنية lb_env {
	بنية sched_domain	*sd;

	بنية rq		*src_rq;
	صحيح			src_cpu;

	صحيح			dst_cpu;
	بنية rq		*dst_rq;
	bool			dst_core_idle;

	بنية cpumask		*dst_grpmask;
	صحيح			new_dst_cpu;
	تعداد cpu_idle_type	idle;
	اوږد			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	بنية cpumask		*cpus;

	بینښه صحيح		flags;

	بینښه صحيح		loop;
	بینښه صحيح		loop_break;
	بینښه صحيح		loop_max;

	تعداد fbq_type		fbq_type;
	تعداد migration_type	migration_type;
	بنية list_سر	tasks;
};

/*
 * Is this task likely cache-hot:
 */
ساكن صحيح task_hot(بنية task_struct *p, بنية lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	إذا (p->sched_class != &fair_sched_class)
		أرجع 0;

	إذا (unlikely(task_has_idle_policy(p)))
		أرجع 0;

	/* SMT siblings share cache */
	إذا (env->sd->flags & SD_SHARE_CPUCAPACITY)
		أرجع 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	إذا (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_د(&p->se)->بل))
		أرجع 1;

	إذا (sysctl_sched_migration_لګښت == -1)
		أرجع 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	إذا (!sched_core_cookie_سمون(cpu_rq(env->dst_cpu), p))
		أرجع 1;

	إذا (sysctl_sched_migration_لګښت == 0)
		أرجع 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_پیل;

	أرجع delta < (s64)sysctl_sched_migration_لګښت;
}

#که_تعریف_شوی CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
ساكن اوږد migrate_degrades_locality(بنية task_struct *p, بنية lb_env *env)
{
	بنية numa_ډله *numa_ډله = rcu_dereference_all(p->numa_ډله);
	بینښه اوږد src_وزن, dst_وزن;
	صحيح src_nid, dst_nid, dist;

	إذا (!static_branch_likely(&sched_numa_balancing))
		أرجع 0;

	إذا (!p->numa_تېروتنې || !(env->sd->flags & SD_NUMA))
		أرجع 0;

	src_nid = cpu_منزل_غوټه(env->src_cpu);
	dst_nid = cpu_منزل_غوټه(env->dst_cpu);

	إذا (src_nid == dst_nid)
		أرجع 0;

	/* Migrating away from the preferred node is always bad. */
	إذا (src_nid == p->numa_preferred_nid) {
		إذا (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			أرجع 1;
		وإلا
			أرجع 0;
	}

	/* Encourage migration to the preferred node. */
	إذا (dst_nid == p->numa_preferred_nid)
		أرجع -1;

	/* Leaving a core idle is often worse than degrading locality. */
	إذا (env->idle == CPU_IDLE)
		أرجع 0;

	dist = غوټه_واټن(src_nid, dst_nid);
	إذا (numa_ډله) {
		src_وزن = ډله_وزن(p, src_nid, dist);
		dst_وزن = ډله_وزن(p, dst_nid, dist);
	} وإلا {
		src_وزن = task_وزن(p, src_nid, dist);
		dst_وزن = task_وزن(p, dst_nid, dist);
	}

	أرجع src_وزن - dst_وزن;
}

#وإلا /* !CONFIG_NUMA_BALANCING: */
ساكن مضمن اوږد migrate_degrades_locality(بنية task_struct *p,
					     بنية lb_env *env)
{
	أرجع 0;
}
#د_که_پای /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
ساكن مضمن صحيح task_is_ineligible_on_dst_cpu(بنية task_struct *p, صحيح dest_cpu)
{
	بنية cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	إذا (sched_feat(ځای_پرځای_کړه_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		أرجع 1;

	أرجع 0;
}

#که_تعریف_شوی CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
ساكن bool ځاییږي_llc_ظرفیت(بینښه اوږد util, بینښه اوږد max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	إذا (cpu_smt_عدد_تارونه == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	أرجع util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#تعریف_کړه util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

ساكن __maybe_unused bool get_llc_stats(صحيح cpu, بینښه اوږد *util,
					 بینښه اوږد *cap)
{
	بنية sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(فی_cpu(sd_llc_shared, cpu));
	إذا (!sd_share)
		أرجع false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->ظرفیت);

	أرجع true;
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
تعداد llc_mig {
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
ساكن تعداد llc_mig can_migrate_llc(صحيح src_cpu, صحيح dst_cpu,
				    بینښه اوږد tsk_util,
				    bool منزل_pref)
{
	بینښه اوږد src_util, dst_util, src_cap, dst_cap;

	إذا (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		أرجع mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	إذا (!ځاییږي_llc_ظرفیت(dst_util, dst_cap) &&
	    !ځاییږي_llc_ظرفیت(src_util, src_cap))
		أرجع mig_unrestricted;

	إذا (منزل_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		إذا (!ځاییږي_llc_ظرفیت(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			أرجع mig_forbid;
	} وإلا {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		إذا (ځاییږي_llc_ظرفیت(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			أرجع mig_forbid;
	}
	أرجع mig_llc;
}

ساكن مضمن bool task_misfits_asym_cpu(بنية lb_env *env, بنية task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	إذا ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_ځاییږي_cpu(p, env->dst_cpu) &&
	    task_ځاییږي_cpu(p, env->src_cpu))
		أرجع true;

	أرجع false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
ساكن تعداد llc_mig can_migrate_llc_task(بنية lb_env *env,
					 بنية task_struct *p)
{
	بنية sched_cache_ډله *grp;
	bool منزل_pref;
	صحيح cpu, src_cpu, dst_cpu;

	إذا (task_misfits_asym_cpu(env, p))
		أرجع mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	إذا (!grp)
		أرجع mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	إذا (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		أرجع mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	إذا (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_ظرفیت(grp, dst_cpu)) {
		إذا (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		أرجع mig_unrestricted;
	}

	إذا (cpus_share_cache(dst_cpu, cpu))
		منزل_pref = true;
	وإلا إذا (cpus_share_cache(src_cpu, cpu))
		منزل_pref = false;
	وإلا
		أرجع mig_unrestricted;

	أرجع can_migrate_llc(src_cpu, dst_cpu,
			       task_util(p), منزل_pref);
}

/*
 * Check if active load balance breaks LLC locality in
 * terms of cache aware load balance. The load level and
 * imbalance do not warrant breaking LLC preference per
 * the can_migrate_llc() policy. Here, the benefit of
 * LLC locality outweighs the power efficiency gained from
 * migrating the only runnable task away.
 */
ساكن مضمن bool
alb_break_llc(بنية lb_env *env)
{
	إذا (!sched_cache_enabled())
		أرجع false;

	إذا (cpus_share_cache(env->src_cpu, env->dst_cpu))
		أرجع false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	إذا (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		بینښه اوږد util = 0;
		بنية task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		إذا (env->migration_type == migrate_misfit)
			أرجع false;

		إذا (env->src_rq->nr_running <= 1)
			أرجع true;

		cur = rcu_dereference_all(env->src_rq->curr);
		إذا (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		إذا (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			أرجع true;
	}

	أرجع false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
ساكن مضمن bool
migrate_llc_task_wrong_dst(بنية task_struct *p, بنية lb_env *env)
{
	أرجع sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_فعال_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
ساكن bool migrate_degrades_llc(بنية task_struct *p, بنية lb_env *env)
{
	إذا (!sched_cache_enabled())
		أرجع false;

	إذا (task_has_sched_core(p))
		أرجع false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	إذا (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		أرجع false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	إذا (migrate_llc_task_wrong_dst(p, env))
		أرجع true;

	إذا (can_migrate_llc_task(env, p) != mig_forbid)
		أرجع false;

	أرجع true;
}

#وإلا
ساكن مضمن bool get_llc_stats(صحيح cpu, بینښه اوږد *util,
				 بینښه اوږد *cap)
{
	أرجع false;
}

ساكن مضمن bool
alb_break_llc(بنية lb_env *env)
{
	أرجع false;
}

ساكن مضمن bool
migrate_llc_task_wrong_dst(بنية task_struct *p, بنية lb_env *env)
{
	أرجع false;
}

ساكن مضمن bool
migrate_degrades_llc(بنية task_struct *p, بنية lb_env *env)
{
	أرجع false;
}
#د_که_پای
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
ساكن
صحيح can_migrate_task(بنية task_struct *p, بنية lb_env *env)
{
	اوږد degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	إذا (p->sched_task_hot)
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
	إذا ((p->se.sched_delayed) && (env->migration_type != migrate_بارول))
		أرجع 0;

	إذا (lb_throttled_hierarchy(p, env->dst_cpu))
		أرجع 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	إذا (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		أرجع 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	إذا (kthread_is_فی_cpu(p))
		أرجع 0;

	إذا (task_is_blocked(p))
		أرجع 0;

	إذا (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		صحيح cpu;

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
		إذا (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_فعال_LB))
			أرجع 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_لومړی_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		إذا (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		أرجع 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	إذا (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		أرجع 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	إذا (env->flags & LBF_فعال_LB)
		أرجع !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	إذا (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		إذا (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			إذا (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			أرجع 0;
		}

		hot = task_hot(p, env);
	} وإلا {
		hot = degrades > 0;
	}

	إذا (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		إذا (hot)
			p->sched_task_hot = 1;
		أرجع 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	أرجع 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
ساكن فراغ detach_task(بنية task_struct *p, بنية lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	إذا (p->sched_task_hot) {
		p->sched_task_hot = 0;
		schedstat_inc(env->sd->lb_hot_gained[env->idle]);
		schedstat_inc(p->stats.nr_forced_migrations);
	}

	WARN_ON(task_current(env->src_rq, p));
	WARN_ON(task_current_donor(env->src_rq, p));

	deactivate_task(env->src_rq, p, له_کتار_ایستل_NOCLOCK);
	سېټ_task_cpu(p, env->dst_cpu);
}

/*
 * detach_one_task() -- tries to dequeue exactly one task from env->src_rq, as
 * part of active balancing operations within "domain".
 *
 * Returns a task if successful and NULL otherwise.
 */
ساكن بنية task_struct *detach_one_task(بنية lb_env *env)
{
	بنية task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_ثبت_سرچپه(p,
			&env->src_rq->cfs_tasks, se.ډله_غوټه) {
		إذا (!can_migrate_task(p, env))
			استمر;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		أرجع p;
	}
	أرجع عدم;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
ساكن صحيح detach_tasks(بنية lb_env *env)
{
	بنية list_سر *tasks = &env->src_rq->cfs_tasks;
	بینښه اوږد util, بارول;
	بنية task_struct *p;
	صحيح detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	إذا (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		أرجع 0;
	}

	إذا (env->imbalance <= 0)
		أرجع 0;

	طالما (!list_تشه(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		إذا (env->idle && env->src_rq->nr_running <= 1)
			اكسر;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		إذا (env->loop > env->loop_max)
			اكسر;

		/* take a breather every nr_migrate tasks */
		إذا (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			اكسر;
		}

		p = list_وروستی_ثبت(tasks, بنية task_struct, se.ډله_غوټه);

		إذا (!can_migrate_task(p, env))
			اذهب بل;

		بدل (env->migration_type) {
		حالة migrate_بارول:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			بارول = max_t(بینښه اوږد, task_h_بارول(p), 1);

			إذا (sched_feat(LB_MIN) &&
			    بارول < 16 && !env->sd->nr_balance_failed)
				اذهب بل;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			إذا (shr_bound(بارول, env->sd->nr_balance_failed) > env->imbalance)
				اذهب بل;

			env->imbalance -= بارول;
			اكسر;

		حالة migrate_util:
			util = task_util_est(p);

			إذا (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				اذهب بل;

			env->imbalance -= util;
			اكسر;

		حالة migrate_task:
			env->imbalance--;
			اكسر;

		حالة migrate_misfit:
			/* This is not a misfit task */
			إذا (task_ځاییږي_cpu(p, env->src_cpu))
				اذهب بل;

			env->imbalance = 0;
			اكسر;

		حالة migrate_llc_task:
			env->imbalance--;
			اكسر;
		}

		detach_task(p, env);
		list_جمع_کړه(&p->se.ډله_غوټه, &env->tasks);

		detached++;

#که_تعریف_شوی CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		إذا (env->idle == CPU_NEWLY_IDLE)
			اكسر;
#د_که_پای

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		إذا (env->imbalance <= 0)
			اكسر;

		استمر;
بل:
		إذا (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.ډله_غوټه, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_جمع_کړه(env->sd->lb_gained[env->idle], detached);

	أرجع detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
ساكن فراغ attach_tasks(بنية lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#که_تعریف_شوی CONFIG_NO_HZ_COMMON
ساكن مضمن bool cfs_rq_has_blocked_بارول(بنية cfs_rq *cfs_rq)
{
	إذا (cfs_rq->avg.بارول_avg)
		أرجع true;

	إذا (cfs_rq->avg.util_avg)
		أرجع true;

	أرجع false;
}

ساكن مضمن bool others_have_blocked(بنية rq *rq)
{
	إذا (cpu_util_rt(rq))
		أرجع true;

	إذا (cpu_util_dl(rq))
		أرجع true;

	إذا (hw_بارول_avg(rq))
		أرجع true;

	إذا (cpu_util_irq(rq))
		أرجع true;

	أرجع false;
}

ساكن مضمن فراغ update_blocked_بارول_tick(بنية rq *rq)
{
	WRITE_ONCE(rq->وروستی_blocked_بارول_update_tick, jiffies);
}

ساكن مضمن فراغ update_has_blocked_بارول_حالت(بنية rq *rq, bool has_blocked_بارول)
{
	إذا (!has_blocked_بارول)
		rq->has_blocked_بارول = 0;
}
#وإلا /* !CONFIG_NO_HZ_COMMON: */
ساكن مضمن bool cfs_rq_has_blocked_بارول(بنية cfs_rq *cfs_rq) { أرجع false; }
ساكن مضمن bool others_have_blocked(بنية rq *rq) { أرجع false; }
ساكن مضمن فراغ update_blocked_بارول_tick(بنية rq *rq) {}
ساكن مضمن فراغ update_has_blocked_بارول_حالت(بنية rq *rq, bool has_blocked_بارول) {}
#د_که_پای /* !CONFIG_NO_HZ_COMMON */

ساكن bool __update_blocked_others(بنية rq *rq, bool *بشپړ)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_بارول_avgs(rq);

	إذا (others_have_blocked(rq))
		*بشپړ = false;

	أرجع updated;
}

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED

ساكن bool __update_blocked_fair(بنية rq *rq, bool *بشپړ)
{
	بنية cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_ونې_پاڼه_cfs_rq_خوندي(rq, cfs_rq, pos) {
		بنية sched_entity *se;

		إذا (update_cfs_rq_بارول_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_بارول_avg(cfs_rq);

			إذا (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			إذا (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		إذا (se && !skip_blocked_update(se))
			update_بارول_avg(cfs_rq_د(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		إذا (cfs_rq_is_decayed(cfs_rq))
			list_del_ونې_پاڼه_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		إذا (cfs_rq_has_blocked_بارول(cfs_rq))
			*بشپړ = false;
	}

	أرجع decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
ساكن فراغ update_cfs_rq_h_بارول(بنية cfs_rq *cfs_rq)
{
	بنية sched_entity *se = cfs_rq_se(cfs_rq);
	بینښه اوږد now = jiffies;
	بینښه اوږد بارول;

	إذا (cfs_rq->وروستی_h_بارول_update == now)
		أرجع;

	WRITE_ONCE(cfs_rq->h_بارول_بل, عدم);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_د(se);
		WRITE_ONCE(cfs_rq->h_بارول_بل, se);
		إذا (cfs_rq->وروستی_h_بارول_update == now)
			اكسر;
	}

	إذا (!se) {
		cfs_rq->h_بارول = cfs_rq_بارول_avg(cfs_rq);
		cfs_rq->وروستی_h_بارول_update = now;
	}

	طالما ((se = READ_ONCE(cfs_rq->h_بارول_بل)) != عدم) {
		بارول = cfs_rq->h_بارول;
		بارول = div64_ul(بارول * se->avg.بارول_avg,
				cfs_rq_بارول_avg(cfs_rq) + 1);
		cfs_rq = ډله_cfs_rq(se);
		cfs_rq->h_بارول = بارول;
		cfs_rq->وروستی_h_بارول_update = now;
	}
}

ساكن بینښه اوږد task_h_بارول(بنية task_struct *p)
{
	بنية cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_بارول(cfs_rq);
	أرجع div64_ul(p->se.avg.بارول_avg * cfs_rq->h_بارول,
			cfs_rq_بارول_avg(cfs_rq) + 1);
}
#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */
ساكن bool __update_blocked_fair(بنية rq *rq, bool *بشپړ)
{
	بنية cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_بارول_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	إذا (cfs_rq_has_blocked_بارول(cfs_rq))
		*بشپړ = false;

	أرجع decayed;
}

ساكن بینښه اوږد task_h_بارول(بنية task_struct *p)
{
	أرجع p->se.avg.بارول_avg;
}
#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

ساكن فراغ __sched_balance_update_blocked_averages(بنية rq *rq)
{
	bool decayed = false, بشپړ = true;

	update_blocked_بارول_tick(rq);

	decayed |= __update_blocked_others(rq, &بشپړ);
	decayed |= __update_blocked_fair(rq, &بشپړ);

	update_has_blocked_بارول_حالت(rq, !بشپړ);
	إذا (decayed)
		cpufreq_update_util(rq, 0);
}

ساكن فراغ sched_balance_update_blocked_averages(صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

	guard(rq_قلف_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
بنية sg_lb_stats {
	بینښه اوږد avg_بارول;			/* Avg load            over the CPUs of the group */
	بینښه اوږد ډله_بارول;		/* Total load          over the CPUs of the group */
	بینښه اوږد ډله_ظرفیت;		/* Capacity            over the CPUs of the group */
	بینښه اوږد ډله_util;		/* Total utilization   over the CPUs of the group */
	بینښه اوږد ډله_runnable;		/* Total runnable time over the CPUs of the group */
	بینښه صحيح مجموعه_nr_running;		/* Nr of all tasks running in the group */
	بینښه صحيح مجموعه_h_nr_running;		/* Nr of CFS tasks running in the group */
	بینښه صحيح idle_cpus;                 /* Nr of idle CPUs         in the group */
	بینښه صحيح ډله_وزن;
	تعداد ډله_type ډله_type;
	بینښه صحيح ډله_asym_packing;	/* Tasks should be moved to preferred CPU */
	بینښه صحيح ډله_smt_balance;		/* Task on busy SMT be moved */
	بینښه صحيح ډله_llc_balance;		/* Tasks should be moved to preferred LLC */
	بینښه اوږد ډله_misfit_task_بارول;	/* A CPU has a task too big for its capacity */
	بینښه صحيح ډله_overutilized;	/* At least one CPU is overutilized in the group */
#که_تعریف_شوی CONFIG_NUMA_BALANCING
	بینښه صحيح nr_numa_running;
	بینښه صحيح nr_preferred_running;
#د_که_پای
#که_تعریف_شوی CONFIG_SCHED_CACHE
	بینښه صحيح nr_pref_dst_llc;
#د_که_پای
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
بنية sd_lb_stats {
	بنية sched_ډله *busiest;		/* Busiest group in this sd */
	بنية sched_ډله *local;		/* Local group in this sd */
	بینښه اوږد ټول_بارول;		/* Total load of all groups in sd */
	بینښه اوږد ټول_ظرفیت;		/* Total capacity of all groups in sd */
	بینښه اوږد avg_بارول;			/* Average load across all groups in sd */
	بینښه صحيح prefer_sibling;		/* Tasks should go to sibling first */

	بنية sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	بنية sg_lb_stats local_stat;		/* Statistics of the local group */
};

ساكن مضمن فراغ init_sd_lb_stats(بنية sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (بنية sd_lb_stats){
		.busiest = عدم,
		.local = عدم,
		.ټول_بارول = 0UL,
		.ټول_ظرفیت = 0UL,
		.busiest_stat = {
			.idle_cpus = UINT_MAX,
			.ډله_type = ډله_has_اضافي,
		},
	};
}

ساكن بینښه اوږد scale_rt_ظرفیت(صحيح cpu)
{
	بینښه اوږد max = get_actual_cpu_ظرفیت(cpu);
	بنية rq *rq = cpu_rq(cpu);
	بینښه اوږد کارول_شوی, حرر;
	بینښه اوږد irq;

	irq = cpu_util_irq(rq);

	إذا (unlikely(irq >= max))
		أرجع 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	کارول_شوی = cpu_util_rt(rq);
	کارول_شوی += cpu_util_dl(rq);

	إذا (unlikely(کارول_شوی >= max))
		أرجع 1;

	حرر = max - کارول_شوی;

	أرجع scale_irq_ظرفیت(حرر, irq, max);
}

ساكن فراغ update_cpu_ظرفیت(بنية sched_domain *sd, صحيح cpu)
{
	بینښه اوږد ظرفیت = scale_rt_ظرفیت(cpu);
	بنية sched_ډله *sdg = sd->groups;

	إذا (!ظرفیت)
		ظرفیت = 1;

	cpu_rq(cpu)->cpu_ظرفیت = ظرفیت;
	trace_sched_cpu_ظرفیت_tp(cpu_rq(cpu));

	sdg->sgc->ظرفیت = ظرفیت;
	sdg->sgc->min_ظرفیت = ظرفیت;
	sdg->sgc->max_ظرفیت = ظرفیت;
}

فراغ update_ډله_ظرفیت(بنية sched_domain *sd, صحيح cpu)
{
	بنية sched_domain *ماشوم = sd->ماشوم;
	بنية sched_ډله *ډله, *sdg = sd->groups;
	بینښه اوږد ظرفیت, min_ظرفیت, max_ظرفیت;
	بینښه اوږد interval;

	interval = msecs_منزل_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_بارول_balance_interval);
	sdg->sgc->بل_update = jiffies + interval;

	إذا (!ماشوم) {
		update_cpu_ظرفیت(sd, cpu);
		أرجع;
	}

	ظرفیت = 0;
	min_ظرفیت = ULONG_MAX;
	max_ظرفیت = 0;

	إذا (ماشوم->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_ډله_پراختیا(sdg)) {
			بینښه اوږد cpu_cap = ظرفیت_د(cpu);

			ظرفیت += cpu_cap;
			min_ظرفیت = min(cpu_cap, min_ظرفیت);
			max_ظرفیت = max(cpu_cap, max_ظرفیت);
		}
	} وإلا  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		ډله = ماشوم->groups;
		افعل {
			بنية sched_ډله_ظرفیت *sgc = ډله->sgc;

			ظرفیت += sgc->ظرفیت;
			min_ظرفیت = min(sgc->min_ظرفیت, min_ظرفیت);
			max_ظرفیت = max(sgc->max_ظرفیت, max_ظرفیت);
			ډله = ډله->بل;
		} طالما (ډله != ماشوم->groups);
	}

	sdg->sgc->ظرفیت = ظرفیت;
	sdg->sgc->min_ظرفیت = min_ظرفیت;
	sdg->sgc->max_ظرفیت = max_ظرفیت;
}

/*
 * Check whether the capacity of the rq has been noticeably reduced by side
 * activity. The imbalance_pct is used for the threshold.
 * Return true is the capacity is reduced
 */
ساكن مضمن صحيح
check_cpu_ظرفیت(بنية rq *rq, بنية sched_domain *sd)
{
	أرجع ((rq->cpu_ظرفیت * sd->imbalance_pct) <
				(arch_scale_cpu_ظرفیت(cpu_د(rq)) * 100));
}

/* Check if the rq has a misfit task */
ساكن مضمن bool check_misfit_حالت(بنية rq *rq)
{
	أرجع rq->misfit_task_بارول;
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

ساكن مضمن صحيح sg_imbalanced(بنية sched_ډله *ډله)
{
	أرجع ډله->sgc->imbalance;
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
ساكن مضمن bool
ډله_has_ظرفیت(بینښه صحيح imbalance_pct, بنية sg_lb_stats *sgs)
{
	إذا (sgs->مجموعه_nr_running < sgs->ډله_وزن)
		أرجع true;

	إذا ((sgs->ډله_ظرفیت * imbalance_pct) <
			(sgs->ډله_runnable * 100))
		أرجع false;

	إذا ((sgs->ډله_ظرفیت * 100) >
			(sgs->ډله_util * imbalance_pct))
		أرجع true;

	أرجع false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
ساكن مضمن bool
ډله_is_overloaded(بینښه صحيح imbalance_pct, بنية sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	إذا (sched_energy_enabled() && !sgs->ډله_overutilized)
		أرجع false;

	إذا (sgs->مجموعه_nr_running <= sgs->ډله_وزن)
		أرجع false;

	إذا ((sgs->ډله_ظرفیت * 100) <
			(sgs->ډله_util * imbalance_pct))
		أرجع true;

	إذا ((sgs->ډله_ظرفیت * imbalance_pct) <
			(sgs->ډله_runnable * 100))
		أرجع true;

	أرجع false;
}

ساكن مضمن تعداد
ډله_type ډله_classify(بینښه صحيح imbalance_pct,
			  بنية sched_ډله *ډله,
			  بنية sg_lb_stats *sgs)
{
	إذا (ډله_is_overloaded(imbalance_pct, sgs))
		أرجع ډله_overloaded;

	إذا (sgs->ډله_llc_balance)
		أرجع ډله_llc_balance;

	إذا (sg_imbalanced(ډله))
		أرجع ډله_imbalanced;

	إذا (sgs->ډله_asym_packing)
		أرجع ډله_asym_packing;

	إذا (sgs->ډله_smt_balance)
		أرجع ډله_smt_balance;

	إذا (sgs->ډله_misfit_task_بارول)
		أرجع ډله_misfit_task;

	إذا (!ډله_has_ظرفیت(imbalance_pct, sgs))
		أرجع ډله_fully_busy;

	أرجع ډله_has_اضافي;
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
ساكن bool sched_use_asym_prio(بنية sched_domain *sd, صحيح cpu)
{
	إذا (!(sd->flags & SD_ASYM_PACKING))
		أرجع false;

	إذا (!sched_smt_فعال())
		أرجع true;

	أرجع sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

ساكن مضمن bool sched_asym(بنية sched_domain *sd, صحيح dst_cpu, صحيح src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	أرجع sched_use_asym_prio(sd, dst_cpu) &&
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
ساكن مضمن bool
sched_ډله_asym(بنية lb_env *env, بنية sg_lb_stats *sgs, بنية sched_ډله *ډله)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	إذا ((ډله->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->ډله_وزن - sgs->idle_cpus != 1))
		أرجع false;

	أرجع sched_asym(env->sd, env->dst_cpu, READ_ONCE(ډله->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
ساكن مضمن bool smt_vs_nonsmt_groups(بنية sched_ډله *sg1,
				    بنية sched_ډله *sg2)
{
	إذا (!sg1 || !sg2)
		أرجع false;

	أرجع (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

ساكن مضمن bool smt_balance(بنية lb_env *env, بنية sg_lb_stats *sgs,
			       بنية sched_ډله *ډله)
{
	إذا (!env->idle)
		أرجع false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	إذا (ډله->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->مجموعه_h_nr_running > 1)
		أرجع true;

	أرجع false;
}

ساكن مضمن اوږد sibling_imbalance(بنية lb_env *env,
				    بنية sd_lb_stats *sds,
				    بنية sg_lb_stats *busiest,
				    بنية sg_lb_stats *local)
{
	صحيح ncores_busiest, ncores_local;
	اوږد imbalance;

	إذا (!env->idle || !busiest->مجموعه_nr_running)
		أرجع 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	إذا (ncores_busiest == ncores_local) {
		imbalance = busiest->مجموعه_nr_running;
		lsub_مثبت(&imbalance, local->مجموعه_nr_running);
		أرجع imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->مجموعه_nr_running;
	lsub_مثبت(&imbalance, ncores_busiest * local->مجموعه_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	إذا (imbalance <= 1 && local->مجموعه_nr_running == 0 &&
	    busiest->مجموعه_nr_running > 1)
		imbalance = 2;

	أرجع imbalance;
}

ساكن مضمن bool
sched_reduced_ظرفیت(بنية rq *rq, بنية sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	إذا (rq->cfs.h_nr_runnable != 1)
		أرجع false;

	أرجع check_cpu_ظرفیت(rq, sd);
}

#که_تعریف_شوی CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
ساكن فراغ ریکارډ_sg_llc_stats(بنية lb_env *env,
				بنية sg_lb_stats *sgs,
				بنية sched_ډله *ډله)
{
	بنية sched_domain_shared *sd_share;
	صحيح cpu;

	إذا (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		أرجع;

	/* Only care about sched domain spanning multiple LLCs */
	إذا (env->sd->ماشوم != rcu_dereference_all(فی_cpu(sd_llc, env->dst_cpu)))
		أرجع;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	إذا ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_ډله_پراختیا(ډله), env->cpus))
		أرجع;

	/*
	 * At this point we know this group spans a LLC domain.
	 * Record the statistic of this group in its corresponding
	 * shared LLC domain.
	 * Note: sd_share cannot be obtained via sd->child->shared,
	 * because the latter refers to the domain that covers the
	 * local group. Instead, sd_share should be located using
	 * the first CPU of the LLC group.
	 */
	cpu = cpumask_لومړی(sched_ډله_پراختیا(ډله));
	sd_share = rcu_dereference_all(فی_cpu(sd_llc_shared, cpu));
	إذا (!sd_share)
		أرجع;

	إذا (READ_ONCE(sd_share->util_avg) != sgs->ډله_util)
		WRITE_ONCE(sd_share->util_avg, sgs->ډله_util);

	إذا (unlikely(READ_ONCE(sd_share->ظرفیت) != sgs->ډله_ظرفیت))
		WRITE_ONCE(sd_share->ظرفیت, sgs->ډله_ظرفیت);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
ساكن مضمن bool llc_balance(بنية lb_env *env, بنية sg_lb_stats *sgs,
			       بنية sched_ډله *ډله)
{
	إذا (!sched_cache_enabled())
		أرجع false;

	إذا (env->sd->flags & SD_SHARE_LLC)
		أرجع false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	إذا ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->ډله_misfit_task_بارول)
		أرجع false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	إذا (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		أرجع false;

	إذا (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_لومړی(sched_ډله_پراختیا(ډله)),
			    env->dst_cpu, 0, true) == mig_llc)
		أرجع true;

	أرجع false;
}

ساكن bool update_llc_busiest(بنية lb_env *env,
			       بنية sg_lb_stats *busiest,
			       بنية sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	أرجع sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#وإلا
ساكن مضمن فراغ ریکارډ_sg_llc_stats(بنية lb_env *env, بنية sg_lb_stats *sgs,
				       بنية sched_ډله *ډله)
{
}

ساكن مضمن bool llc_balance(بنية lb_env *env, بنية sg_lb_stats *sgs,
			       بنية sched_ډله *ډله)
{
	أرجع false;
}

ساكن bool update_llc_busiest(بنية lb_env *env,
			       بنية sg_lb_stats *busiest,
			       بنية sg_lb_stats *sgs)
{
	أرجع false;
}
#د_که_پای

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
ساكن مضمن فراغ update_sg_lb_stats(بنية lb_env *env,
				      بنية sd_lb_stats *sds,
				      بنية sched_ډله *ډله,
				      بنية sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	صحيح i, nr_running, local_ډله, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->پلار;

	memset(sgs, 0, حجم(*sgs));

	local_ډله = ډله == sds->local;

	for_each_cpu_and(i, sched_ډله_پراختیا(ډله), env->cpus) {
		بنية rq *rq = cpu_rq(i);
		بینښه اوږد بارول = cpu_بارول(rq);

		sgs->ډله_بارول += بارول;
		sgs->ډله_util += cpu_util_cfs(i);
		sgs->ډله_runnable += cpu_runnable(rq);
		sgs->مجموعه_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->مجموعه_nr_running += nr_running;

		إذا (cpu_overutilized(i))
			sgs->ډله_overutilized = 1;

#که_تعریف_شوی CONFIG_SCHED_CACHE
		إذا (sched_cache_enabled()) {
			بنية sched_domain *sd_tmp;
			صحيح dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			إذا (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				إذا (sd_tmp && (بینښه صحيح)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#د_که_پای

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		إذا (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			استمر;
		}

		/* Overload indicator is only updated at root domain */
		إذا (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#که_تعریف_شوی CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		إذا (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#د_که_پای
		إذا (local_ډله)
			استمر;

		إذا (sd_flags & SD_ASYM_CPUCAPACITY) {
			إذا (rq->misfit_task_بارول) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				إذا (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				إذا (ظرفیت_greater(ظرفیت_د(env->dst_cpu),
						     ډله->sgc->max_ظرفیت) &&
				    (sgs->ډله_misfit_task_بارول < rq->misfit_task_بارول))
					sgs->ډله_misfit_task_بارول = rq->misfit_task_بارول;
			}
		} وإلا إذا (env->idle && sched_reduced_ظرفیت(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			إذا (sgs->ډله_misfit_task_بارول < بارول)
				sgs->ډله_misfit_task_بارول = بارول;
		}
	}

	sgs->ډله_ظرفیت = ډله->sgc->ظرفیت;

	sgs->ډله_وزن = ډله->ډله_وزن;

	إذا (!local_ډله) {
		/* Check if dst CPU is idle and preferred to this group */
		إذا (env->idle && sgs->مجموعه_h_nr_running &&
		    sched_ډله_asym(env, sgs, ډله))
			sgs->ډله_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		إذا (smt_balance(env, sgs, ډله))
			sgs->ډله_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		إذا (llc_balance(env, sgs, ډله))
			sgs->ډله_llc_balance = 1;
	}

	sgs->ډله_type = ډله_classify(env->sd->imbalance_pct, ډله, sgs);

	ریکارډ_sg_llc_stats(env, sgs, ډله);
	/* Computing avg_load makes sense only when group is overloaded */
	إذا (sgs->ډله_type == ډله_overloaded)
		sgs->avg_بارول = (sgs->ډله_بارول * SCHED_ظرفیت_SCALE) /
				sgs->ډله_ظرفیت;
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
ساكن bool update_sd_pick_busiest(بنية lb_env *env,
				   بنية sd_lb_stats *sds,
				   بنية sched_ډله *sg,
				   بنية sg_lb_stats *sgs)
{
	بنية sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	إذا (!sgs->مجموعه_h_nr_running)
		أرجع false;

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
	إذا ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->ډله_type == ډله_misfit_task) &&
	    (!env->dst_core_idle ||
	     !ظرفیت_greater(ظرفیت_د(env->dst_cpu), sg->sgc->max_ظرفیت) ||
	     sds->local_stat.ډله_type != ډله_has_اضافي))
		أرجع false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	إذا ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->ډله_type <= ډله_fully_busy) &&
	    (ظرفیت_greater(sg->sgc->min_ظرفیت, ظرفیت_د(env->dst_cpu))))
		أرجع false;

	إذا (sgs->ډله_type > busiest->ډله_type)
		أرجع true;

	إذا (sgs->ډله_type < busiest->ډله_type)
		أرجع false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	بدل (sgs->ډله_type) {
	حالة ډله_overloaded:
		/* Select the overloaded group with highest avg_load. */
		أرجع sgs->avg_بارول > busiest->avg_بارول;

	حالة ډله_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		أرجع update_llc_busiest(env, busiest, sgs);

	حالة ډله_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		أرجع false;

	حالة ډله_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		أرجع sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	حالة ډله_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		أرجع sgs->ډله_misfit_task_بارول > busiest->ډله_misfit_task_بارول;

	حالة ډله_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		إذا (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			اذهب has_اضافي;

		fallthrough;

	حالة ډله_fully_busy:
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

		إذا (sgs->avg_بارول < busiest->avg_بارول)
			أرجع false;

		إذا (sgs->avg_بارول == busiest->avg_بارول) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			إذا (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				أرجع false;
		}

		اكسر;

	حالة ډله_has_اضافي:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		إذا (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			إذا (sg->flags & SD_SHARE_CPUCAPACITY && sgs->مجموعه_h_nr_running <= 1)
				أرجع false;
			وإلا
				أرجع true;
		}
has_اضافي:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		إذا (sgs->idle_cpus > busiest->idle_cpus)
			أرجع false;
		وإلا إذا ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->مجموعه_nr_running <= busiest->مجموعه_nr_running))
			أرجع false;

		اكسر;
	}

	أرجع true;
}

#که_تعریف_شوی CONFIG_NUMA_BALANCING
ساكن مضمن تعداد fbq_type fbq_classify_ډله(بنية sg_lb_stats *sgs)
{
	إذا (sgs->مجموعه_h_nr_running > sgs->nr_numa_running)
		أرجع regular;
	إذا (sgs->مجموعه_h_nr_running > sgs->nr_preferred_running)
		أرجع remote;
	أرجع all;
}

ساكن مضمن تعداد fbq_type fbq_classify_rq(بنية rq *rq)
{
	إذا (rq->nr_running > rq->nr_numa_running)
		أرجع regular;
	إذا (rq->nr_running > rq->nr_preferred_running)
		أرجع remote;
	أرجع all;
}
#وإلا /* !CONFIG_NUMA_BALANCING: */
ساكن مضمن تعداد fbq_type fbq_classify_ډله(بنية sg_lb_stats *sgs)
{
	أرجع all;
}

ساكن مضمن تعداد fbq_type fbq_classify_rq(بنية rq *rq)
{
	أرجع regular;
}
#د_که_پای /* !CONFIG_NUMA_BALANCING */


بنية sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

ساكن بینښه صحيح task_running_on_cpu(صحيح cpu, بنية task_struct *p)
{
	/* Task has no contribution or is new */
	إذا (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.وروستی_update_وخت))
		أرجع 0;

	إذا (task_on_rq_queued(p))
		أرجع 1;

	أرجع 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
ساكن صحيح idle_cpu_without(صحيح cpu, بنية task_struct *p)
{
	بنية rq *rq = cpu_rq(cpu);

	إذا (rq->curr != rq->idle && rq->curr != p)
		أرجع 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	إذا (rq->ttwu_pending)
		أرجع 0;

	أرجع 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
ساكن مضمن فراغ update_sg_wakeup_stats(بنية sched_domain *sd,
					  بنية sched_ډله *ډله,
					  بنية sg_lb_stats *sgs,
					  بنية task_struct *p)
{
	صحيح i, nr_running;

	memset(sgs, 0, حجم(*sgs));

	/* Assume that task can't fit any CPU of the group */
	إذا (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->ډله_misfit_task_بارول = 1;

	for_each_cpu_and(i, sched_ډله_پراختیا(ډله), p->cpus_ptr) {
		بنية rq *rq = cpu_rq(i);
		بینښه صحيح local;

		sgs->ډله_بارول += cpu_بارول_without(rq, p);
		sgs->ډله_util += cpu_util_without(i, p);
		sgs->ډله_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->مجموعه_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->مجموعه_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		إذا (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		إذا (sd->flags & SD_ASYM_CPUCAPACITY &&
		    sgs->ډله_misfit_task_بارول &&
		    task_ځاییږي_cpu(p, i))
			sgs->ډله_misfit_task_بارول = 0;

	}

	sgs->ډله_ظرفیت = ډله->sgc->ظرفیت;

	sgs->ډله_وزن = ډله->ډله_وزن;

	sgs->ډله_type = ډله_classify(sd->imbalance_pct, ډله, sgs);

	/*
	 * Computing avg_load makes sense only when group is fully busy or
	 * overloaded
	 */
	إذا (sgs->ډله_type == ډله_fully_busy ||
		sgs->ډله_type == ډله_overloaded)
		sgs->avg_بارول = (sgs->ډله_بارول * SCHED_ظرفیت_SCALE) /
				sgs->ډله_ظرفیت;
}

ساكن bool update_pick_idlest(بنية sched_ډله *idlest,
			       بنية sg_lb_stats *idlest_sgs,
			       بنية sched_ډله *ډله,
			       بنية sg_lb_stats *sgs)
{
	إذا (sgs->ډله_type < idlest_sgs->ډله_type)
		أرجع true;

	إذا (sgs->ډله_type > idlest_sgs->ډله_type)
		أرجع false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	بدل (sgs->ډله_type) {
	حالة ډله_overloaded:
	حالة ډله_fully_busy:
		/* Select the group with lowest avg_load. */
		إذا (idlest_sgs->avg_بارول <= sgs->avg_بارول)
			أرجع false;
		اكسر;

	حالة ډله_llc_balance:
	حالة ډله_imbalanced:
	حالة ډله_asym_packing:
	حالة ډله_smt_balance:
		/* Those types are not used in the slow wakeup path */
		أرجع false;

	حالة ډله_misfit_task:
		/* Select group with the highest max capacity */
		إذا (idlest->sgc->max_ظرفیت >= ډله->sgc->max_ظرفیت)
			أرجع false;
		اكسر;

	حالة ډله_has_اضافي:
		/* Select group with most idle CPUs */
		إذا (idlest_sgs->idle_cpus > sgs->idle_cpus)
			أرجع false;

		/* Select group with lowest group_util */
		إذا (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->ډله_util <= sgs->ډله_util)
			أرجع false;

		اكسر;
	}

	أرجع true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
ساكن بنية sched_ډله *
sched_balance_پیدا_کړه_dst_ډله(بنية sched_domain *sd, بنية task_struct *p, صحيح this_cpu)
{
	بنية sched_ډله *idlest = عدم, *local = عدم, *ډله = sd->groups;
	بنية sg_lb_stats local_sgs, tmp_sgs;
	بنية sg_lb_stats *sgs;
	بینښه اوږد imbalance;
	بنية sg_lb_stats idlest_sgs = {
			.avg_بارول = UINT_MAX,
			.ډله_type = ډله_overloaded,
	};

	افعل {
		صحيح local_ډله;

		/* Skip over this group if it has no CPUs allowed */
		إذا (!cpumask_intersects(sched_ډله_پراختیا(ډله),
					p->cpus_ptr))
			استمر;

		/* Skip over this group if no cookie matched */
		إذا (!sched_ډله_cookie_سمون(cpu_rq(this_cpu), p, ډله))
			استمر;

		local_ډله = cpumask_test_cpu(this_cpu,
					       sched_ډله_پراختیا(ډله));

		إذا (local_ډله) {
			sgs = &local_sgs;
			local = ډله;
		} وإلا {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, ډله, sgs, p);

		إذا (!local_ډله && update_pick_idlest(idlest, &idlest_sgs, ډله, sgs)) {
			idlest = ډله;
			idlest_sgs = *sgs;
		}

	} طالما (ډله = ډله->بل, ډله != sd->groups);


	/* There is no idlest group to push tasks to */
	إذا (!idlest)
		أرجع عدم;

	/* The local group has been skipped because of CPU affinity */
	إذا (!local)
		أرجع idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	إذا (local_sgs.ډله_type < idlest_sgs.ډله_type)
		أرجع عدم;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	إذا (local_sgs.ډله_type > idlest_sgs.ډله_type)
		أرجع idlest;

	بدل (local_sgs.ډله_type) {
	حالة ډله_overloaded:
	حالة ډله_fully_busy:

		/* Calculate allowed imbalance based on load */
		imbalance = scale_بارول_ښکته(NICE_0_بارول) *
				(sd->imbalance_pct-100) / 100;

		/*
		 * When comparing groups across NUMA domains, it's possible for
		 * the local domain to be very lightly loaded relative to the
		 * remote domains but "imbalance" skews the comparison making
		 * remote CPUs look much more favourable. When considering
		 * cross-domain, add imbalance to the load on the remote node
		 * and consider staying local.
		 */

		إذا ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_بارول + imbalance) >= local_sgs.avg_بارول))
			أرجع عدم;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		إذا (idlest_sgs.avg_بارول >= (local_sgs.avg_بارول + imbalance))
			أرجع عدم;

		إذا (100 * local_sgs.avg_بارول <= sd->imbalance_pct * idlest_sgs.avg_بارول)
			أرجع عدم;
		اكسر;

	حالة ډله_llc_balance:
	حالة ډله_imbalanced:
	حالة ډله_asym_packing:
	حالة ډله_smt_balance:
		/* Those type are not used in the slow wakeup path */
		أرجع عدم;

	حالة ډله_misfit_task:
		/* Select group with the highest max capacity */
		إذا (local->sgc->max_ظرفیت >= idlest->sgc->max_ظرفیت)
			أرجع عدم;
		اكسر;

	حالة ډله_has_اضافي:
#که_تعریف_شوی CONFIG_NUMA
		إذا (sd->flags & SD_NUMA) {
			صحيح imb_numa_nr = sd->imb_numa_nr;
#که_تعریف_شوی CONFIG_NUMA_BALANCING
			صحيح idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			إذا (cpu_منزل_غوټه(this_cpu) == p->numa_preferred_nid)
				أرجع عدم;

			idlest_cpu = cpumask_لومړی(sched_ډله_پراختیا(idlest));
			إذا (cpu_منزل_غوټه(idlest_cpu) == p->numa_preferred_nid)
				أرجع idlest;
#د_که_پای /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			إذا (p->nr_cpus_allowed != NR_CPUS) {
				بینښه صحيح w = cpumask_وزن_and(p->cpus_ptr,
								sched_ډله_پراختیا(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = مطلق_قیمت(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			إذا (!adjust_numa_imbalance(imbalance,
						   local_sgs.مجموعه_nr_running + 1,
						   imb_numa_nr)) {
				أرجع عدم;
			}
		}
#د_که_پای /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		إذا (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			أرجع عدم;
		اكسر;
	}

	أرجع idlest;
}

ساكن فراغ update_idle_cpu_لټون(بنية lb_env *env,
				 بینښه اوږد مجموعه_util)
{
	بنية sched_domain_shared *sd_share;
	بنية sched_domain *sd = env->sd;
	صحيح llc_وزن, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	إذا (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		أرجع;

	sd_share = sd->shared;
	إذا (!sd_share)
		أرجع;

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
	x = مجموعه_util;
	llc_وزن = sd->پراختیا_وزن;
	do_div(x, llc_وزن);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_ظرفیت_SCALE);
	tmp = min_t(اوږد, tmp, SCHED_ظرفیت_SCALE);
	y = SCHED_ظرفیت_SCALE - tmp;

	/* equation [2] */
	y *= llc_وزن;
	do_div(y, SCHED_ظرفیت_SCALE);
	إذا ((صحيح)y != sd_share->nr_idle_لټون)
		WRITE_ONCE(sd_share->nr_idle_لټون, (صحيح)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

ساكن مضمن فراغ update_sd_lb_stats(بنية lb_env *env, بنية sd_lb_stats *sds)
{
	بنية sched_ډله *sg = env->sd->groups;
	بنية sg_lb_stats *local = &sds->local_stat;
	بنية sg_lb_stats tmp_sgs;
	بینښه اوږد مجموعه_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_فعال() || is_core_idle(env->dst_cpu);

	افعل {
		بنية sg_lb_stats *sgs = &tmp_sgs;
		صحيح local_ډله;

		local_ډله = cpumask_test_cpu(env->dst_cpu, sched_ډله_پراختیا(sg));
		إذا (local_ډله) {
			sds->local = sg;
			sgs = local;

			إذا (env->idle != CPU_NEWLY_IDLE ||
			    وخت_after_eq(jiffies, sg->sgc->بل_update))
				update_ډله_ظرفیت(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		إذا (!local_ډله && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->ډله_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->ټول_بارول += sgs->ډله_بارول;
		sds->ټول_ظرفیت += sgs->ډله_ظرفیت;

		مجموعه_util += sgs->ډله_util;
		sg = sg->بل;
	} طالما (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	إذا (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	إذا (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_ډله(&sds->busiest_stat);

	إذا (!env->sd->پلار) {
		/* update overload indicator if we are at root domain */
		سېټ_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		سېټ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} وإلا إذا (sg_overutilized) {
		سېټ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_لټون(env, مجموعه_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
ساكن مضمن فراغ calculate_imbalance(بنية lb_env *env, بنية sd_lb_stats *sds)
{
	بنية sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	إذا (busiest->ډله_type == ډله_misfit_task) {
		إذا (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} وإلا {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_بارول;
			env->imbalance = busiest->ډله_misfit_task_بارول;
		}
		أرجع;
	}

	إذا (busiest->ډله_type == ډله_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->مجموعه_h_nr_running;
		أرجع;
	}

	إذا (busiest->ډله_type == ډله_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		أرجع;
	}

#که_تعریف_شوی CONFIG_SCHED_CACHE
	إذا (busiest->ډله_type == ډله_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		أرجع;
	}
#د_که_پای

	إذا (busiest->ډله_type == ډله_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		أرجع;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	إذا (local->ډله_type == ډله_has_اضافي) {
		إذا ((busiest->ډله_type > ډله_fully_busy) &&
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
			env->imbalance = max(local->ډله_ظرفیت, local->ډله_util) -
					 local->ډله_util;

			/*
			 * In some cases, the group's utilization is max or even
			 * higher than capacity because of migrations but the
			 * local CPU is (newly) idle. There is at least one
			 * waiting task in this overloaded busiest group. Let's
			 * try to pull it.
			 */
			إذا (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			أرجع;
		}

		إذا (busiest->ډله_وزن == 1 || sds->prefer_sibling) {
			/*
			 * When prefer sibling, evenly spread running tasks on
			 * groups.
			 */
			env->migration_type = migrate_task;
			env->imbalance = sibling_imbalance(env, sds, busiest, local);
		} وإلا {

			/*
			 * If there is no overload, we just want to even the number of
			 * idle CPUs.
			 */
			env->migration_type = migrate_task;
			env->imbalance = max_t(اوږد, 0,
					       (local->idle_cpus - busiest->idle_cpus));
		}

#که_تعریف_شوی CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		إذا (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->مجموعه_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#د_که_پای

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		أرجع;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	إذا (local->ډله_type < ډله_overloaded) {
		/*
		 * Local will become overloaded so the avg_load metrics are
		 * finally needed.
		 */

		local->avg_بارول = (local->ډله_بارول * SCHED_ظرفیت_SCALE) /
				  local->ډله_ظرفیت;

		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		إذا (local->avg_بارول >= busiest->avg_بارول) {
			env->imbalance = 0;
			أرجع;
		}

		sds->avg_بارول = (sds->ټول_بارول * SCHED_ظرفیت_SCALE) /
				sds->ټول_ظرفیت;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		إذا (local->avg_بارول >= sds->avg_بارول) {
			env->imbalance = 0;
			أرجع;
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
	env->migration_type = migrate_بارول;
	env->imbalance = min(
		(busiest->avg_بارول - sds->avg_بارول) * busiest->ډله_ظرفیت,
		(sds->avg_بارول - local->avg_بارول) * local->ډله_ظرفیت
	) / SCHED_ظرفیت_SCALE;
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
ساكن بنية sched_ډله *sched_balance_پیدا_کړه_src_ډله(بنية lb_env *env)
{
	بنية sg_lb_stats *local, *busiest;
	بنية sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	إذا (!sds.busiest)
		اذهب بهر_متوازن;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	إذا (busiest->ډله_type == ډله_misfit_task)
		اذهب force_balance;

	إذا (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		اذهب بهر_متوازن;

	/* ASYM feature bypasses nice load balance check */
	إذا (busiest->ډله_type == ډله_asym_packing)
		اذهب force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	إذا (busiest->ډله_type == ډله_imbalanced)
		اذهب force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	إذا (local->ډله_type > busiest->ډله_type)
		اذهب بهر_متوازن;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	إذا (local->ډله_type == ډله_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		إذا (local->avg_بارول >= busiest->avg_بارول)
			اذهب بهر_متوازن;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_بارول = (sds.ټول_بارول * SCHED_ظرفیت_SCALE) /
				sds.ټول_ظرفیت;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		إذا (local->avg_بارول >= sds.avg_بارول)
			اذهب بهر_متوازن;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		إذا (100 * busiest->avg_بارول <=
				env->sd->imbalance_pct * local->avg_بارول)
			اذهب بهر_متوازن;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	إذا (sds.prefer_sibling && local->ډله_type == ډله_has_اضافي &&
	    (busiest->ډله_type == ډله_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		اذهب force_balance;

	إذا (busiest->ډله_type != ډله_overloaded) {
		إذا (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			اذهب بهر_متوازن;
		}

		إذا (busiest->ډله_type == ډله_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			اذهب force_balance;
		}

		إذا (busiest->ډله_وزن > 1 &&
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
			اذهب بهر_متوازن;
		}

		إذا (busiest->مجموعه_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			اذهب بهر_متوازن;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	أرجع env->imbalance ? sds.busiest : عدم;

بهر_متوازن:
	env->imbalance = 0;
	أرجع عدم;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
ساكن بنية rq *sched_balance_پیدا_کړه_src_rq(بنية lb_env *env,
				     بنية sched_ډله *ډله)
{
	بنية rq *busiest = عدم, *rq;
	بینښه اوږد busiest_util = 0, busiest_بارول = 0, busiest_ظرفیت = 1;
	بینښه صحيح __maybe_unused busiest_pref_llc = 0;
	بنية sched_domain __maybe_unused *sd_tmp;
	بینښه صحيح busiest_nr = 0;
	صحيح __maybe_unused dst_llc;
	صحيح i;

	for_each_cpu_and(i, sched_ډله_پراختیا(ډله), env->cpus) {
		بینښه اوږد ظرفیت, بارول, util;
		بینښه صحيح nr_running;
		تعداد fbq_type rt;

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
		إذا (rt > env->fbq_type)
			استمر;

		nr_running = rq->cfs.h_nr_runnable;
		إذا (!nr_running)
			استمر;

		ظرفیت = ظرفیت_د(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		إذا (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    nr_running == 1) {
			bool cluster_equal_cap = static_branch_unlikely(&sched_cluster_فعال) &&
						 (get_actual_cpu_ظرفیت(env->dst_cpu) ==
						  get_actual_cpu_ظرفیت(i));
			bool smt_degraded_cap = sched_smt_فعال() && !is_core_idle(i);

			/*
			 * Busy SMT siblings reduce the capacity of CPU @i. Do
			 * not skip it in this case.
			 *
			 * CONFIG_SCHED_CLUSTER requires balancing load across
			 * clusters of identical capacity, accounting for
			 * hardware and cpufreq pressure.
			 */
			إذا (!smt_degraded_cap && !cluster_equal_cap &&
			    !ظرفیت_greater(ظرفیت_د(env->dst_cpu), ظرفیت))
				استمر;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		إذا (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			استمر;

		بدل (env->migration_type) {
		حالة migrate_بارول:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			بارول = cpu_بارول(rq);

			إذا (nr_running == 1 && بارول > env->imbalance &&
			    !check_cpu_ظرفیت(rq, env->sd))
				اكسر;

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
			إذا (بارول * busiest_ظرفیت > busiest_بارول * ظرفیت) {
				busiest_بارول = بارول;
				busiest_ظرفیت = ظرفیت;
				busiest = rq;
			}
			اكسر;

		حالة migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			إذا (nr_running <= 1)
				استمر;

			إذا (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			اكسر;

		حالة migrate_task:
			إذا (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			اكسر;

		حالة migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			إذا (rq->misfit_task_بارول > busiest_بارول) {
				busiest_بارول = rq->misfit_task_بارول;
				busiest = rq;
			}

			اكسر;

		حالة migrate_llc_task:
#که_تعریف_شوی CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			إذا (sd_tmp && (بینښه)dst_llc < sd_tmp->llc_max) {
				بینښه صحيح this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				إذا (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#د_که_پای
			اكسر;

		}
	}

	أرجع busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#تعریف_کړه MAX_PINNED_INTERVAL	512

ساكن مضمن bool
asym_فعال_balance(بنية lb_env *env)
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
	أرجع env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

ساكن مضمن bool
imbalanced_فعال_balance(بنية lb_env *env)
{
	بنية sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	إذا ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		أرجع 1;

	أرجع 0;
}

ساكن صحيح need_فعال_balance(بنية lb_env *env)
{
	بنية sched_domain *sd = env->sd;

	إذا (alb_break_llc(env))
		أرجع 0;

	إذا (asym_فعال_balance(env))
		أرجع 1;

	إذا (imbalanced_فعال_balance(env))
		أرجع 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	إذا (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		إذا ((check_cpu_ظرفیت(env->src_rq, sd)) &&
		    (ظرفیت_د(env->src_cpu)*sd->imbalance_pct < ظرفیت_د(env->dst_cpu)*100))
			أرجع 1;
	}

	إذا (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		أرجع 1;

	أرجع 0;
}

ساكن صحيح فعال_بارول_balance_cpu_stop(فراغ *معلومات);
ساكن صحيح فعال_بارول_balance_llc_cpu_stop(فراغ *معلومات);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
ساكن مضمن cpu_stop_fn_t alb_stop_fn(بنية lb_env *env)
{
	إذا (env->migration_type == migrate_llc_task)
		أرجع فعال_بارول_balance_llc_cpu_stop;

	أرجع فعال_بارول_balance_cpu_stop;
}

ساكن صحيح should_we_balance(بنية lb_env *env)
{
	بنية cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	بنية sched_ډله *sg = env->sd->groups;
	صحيح cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	إذا (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		أرجع 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	إذا (env->idle == CPU_NEWLY_IDLE) {
		إذا (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			أرجع 0;
		أرجع 1;
	}

	cpumask_copy(swb_cpus, ډله_balance_ماسک(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		إذا (!idle_cpu(cpu))
			استمر;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		إذا (sched_smt_فعال() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			إذا (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_ماسک(cpu));
			استمر;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		أرجع cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	إذا (idle_smt != -1)
		أرجع idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	أرجع ډله_balance_cpu(sg) == env->dst_cpu;
}

ساكن فراغ update_lb_imbalance_stat(بنية lb_env *env, بنية sched_domain *sd,
				     تعداد cpu_idle_type idle)
{
	إذا (!schedstat_enabled())
		أرجع;

	بدل (env->migration_type) {
	حالة migrate_بارول:
		__schedstat_add(sd->lb_imbalance_بارول[idle], env->imbalance);
		اكسر;
	حالة migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		اكسر;
	حالة migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		اكسر;
	حالة migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		اكسر;
	حالة migrate_llc_task:
		اكسر;
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
ساكن atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
ساكن صحيح sched_balance_rq(صحيح this_cpu, بنية rq *this_rq,
			بنية sched_domain *sd, تعداد cpu_idle_type idle,
			صحيح *continue_balancing)
{
	صحيح ld_moved, cur_ld_moved, فعال_balance = 0;
	بنية sched_domain *sd_پلار = sd->پلار;
	بنية sched_ډله *ډله;
	بنية rq *busiest;
	بنية rq_flags rf;
	بنية cpumask *cpus = this_cpu_cpumask_var_ptr(بارول_balance_ماسک);
	بنية lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = ډله_balance_ماسک(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_سر_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_پراختیا(sd), cpu_preferred_ماسک);

	schedstat_inc(sd->lb_شمېر[idle]);

redo:
	إذا (!should_we_balance(&env)) {
		*continue_balancing = 0;
		اذهب بهر_متوازن;
	}

	إذا (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		صحيح zero = 0;
		إذا (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			اذهب بهر_متوازن;

		need_unlock = true;
	}

	ډله = sched_balance_پیدا_کړه_src_ډله(&env);
	إذا (!ډله) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		اذهب بهر_متوازن;
	}

	busiest = sched_balance_پیدا_کړه_src_rq(&env, ډله);
	إذا (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		اذهب بهر_متوازن;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	إذا (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If sched_balance_find_src_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.loop_max  = min(sysctl_sched_nr_migrate, busiest->nr_running);

نور_balance:
		rq_قلف_irqsave(busiest, &rf);
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

		إذا (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		إذا (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			اذهب نور_balance;
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
		إذا ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

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
			اذهب نور_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		إذا (sd_پلار) {
			صحيح *ډله_imbalance = &sd_پلار->groups->sgc->imbalance;

			إذا ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*ډله_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		إذا (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_د(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			إذا (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				اذهب redo;
			}
			اذهب بهر_all_pinned;
		}
	}

	إذا (ld_moved) {
		sd->nr_balance_failed = 0;
		اذهب بهر_unbalanced;
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
	إذا (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	إذا (!need_فعال_balance(&env))
		اذهب بهر_unbalanced;

	scoped_guard (raw_spin_rq_قلف_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		إذا (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			اذهب بهر_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		إذا (busiest->فعال_balance)
			اذهب بهر_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		إذا (!busiest->curr->on_rq)
			اذهب بهر_unbalanced;

		busiest->فعال_balance = 1;
		busiest->ټېل_cpu = this_cpu;
		فعال_balance = 1;
		preempt_disable();
	}
	إذا (فعال_balance) {
		stop_one_cpu_nowait(cpu_د(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->فعال_balance_work);
	}
	preempt_enable();

بهر_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	اذهب بهر;

بهر_متوازن:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	إذا (sd_پلار && !(env.flags & LBF_ALL_PINNED)) {
		صحيح *ډله_imbalance = &sd_پلار->groups->sgc->imbalance;

		إذا (*ډله_imbalance)
			*ډله_imbalance = 0;
	}

بهر_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_متوازن[idle]);

	sd->nr_balance_failed = 0;

بهر_one_pinned:
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
	إذا (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		اذهب بهر;

	/* tune up the balancing interval */
	إذا ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
بهر:
	إذا (need_unlock)
		atomic_سېټ_release(&sched_balance_running, 0);

	أرجع ld_moved;
}

ساكن مضمن بینښه اوږد
get_sd_balance_interval(بنية sched_domain *sd, صحيح cpu_busy)
{
	بینښه اوږد interval = sd->balance_interval;

	إذا (cpu_busy)
		interval *= sd->busy_ضریب;

	/* scale ms to jiffies */
	interval = msecs_منزل_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	إذا (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_بارول_balance_interval);

	أرجع interval;
}

ساكن مضمن فراغ
update_بل_balance(بنية sched_domain *sd, بینښه اوږد *بل_balance)
{
	بینښه اوږد interval, بل;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	بل = sd->وروستی_balance + interval;

	إذا (وخت_after(*بل_balance, بل))
		*بل_balance = بل;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
ساكن صحيح __active_load_balance_cpu_stop(فراغ *معلومات, بینښه صحيح lb_flags)
{
	بنية rq *busiest_rq = معلومات;
	صحيح busiest_cpu = cpu_د(busiest_rq);
	صحيح موخه_cpu = busiest_rq->ټېل_cpu;
	بنية rq *موخه_rq = cpu_rq(موخه_cpu);
	بنية sched_domain *sd;
	بنية task_struct *p = عدم;
	بنية rq_flags rf;

	rq_قلف_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	إذا (!cpu_فعال(busiest_cpu) || !cpu_فعال(موخه_cpu))
		اذهب بهر_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	إذا (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->فعال_balance))
		اذهب بهر_unlock;

	/* Is there any task to move? */
	إذا (busiest_rq->nr_running <= 1)
		اذهب بهر_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == موخه_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_قلف();
	for_each_domain(موخه_cpu, sd) {
		إذا (cpumask_test_cpu(busiest_cpu, sched_domain_پراختیا(sd)))
			اكسر;
	}

	إذا (likely(sd)) {
		بنية lb_env env = {
			.sd		= sd,
			.dst_cpu	= موخه_cpu,
			.dst_rq		= موخه_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_فعال_LB | lb_flags,
		};

		schedstat_inc(sd->alb_شمېر);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		إذا (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} وإلا {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
بهر_unlock:
	busiest_rq->فعال_balance = 0;
	rq_unlock(busiest_rq, &rf);

	إذا (p)
		attach_one_task(موخه_rq, p);

	local_irq_enable();

	أرجع 0;
}

ساكن صحيح فعال_بارول_balance_cpu_stop(فراغ *معلومات)
{
	أرجع __active_load_balance_cpu_stop(معلومات, 0);
}

ساكن صحيح فعال_بارول_balance_llc_cpu_stop(فراغ *معلومات)
{
	أرجع __active_load_balance_cpu_stop(معلومات, LBF_فعال_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
فراغ update_max_interval(فراغ)
{
	max_بارول_balance_interval = HZ*عدد_online_cpus()/10;
}

ساكن مضمن فراغ update_newidle_stats(بنية sched_domain *sd, بینښه صحيح success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	إذا (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		صحيح نسبت = 0;

		إذا (delta < 0)
			delta = 0;

		إذا (sched_feat(NI_شرح)) {
			/*
			 * ratio  delta   freq
			 *
			 * 1024 -  4  s -  128 Hz
			 *  512 -  2  s -  256 Hz
			 *  256 -  1  s -  512 Hz
			 *  128 - .5  s - 1024 Hz
			 *   64 - .25 s - 2048 Hz
			 */
			نسبت = delta >> 22;
		}

		نسبت += sd->newidle_success;

		sd->newidle_نسبت = min(1024, نسبت);
		sd->newidle_call /= 2;
		sd->newidle_success /= 2;
	}
}

ساكن مضمن bool
update_newidle_لګښت(بنية sched_domain *sd, u64 لګښت, بینښه صحيح success)
{
	بینښه اوږد بل_decay = sd->وروستی_decay_max_lb_لګښت + HZ;
	بینښه اوږد now = jiffies;

	إذا (لګښت)
		update_newidle_stats(sd, success);

	إذا (لګښت > sd->max_newidle_lb_لګښت) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_لګښت = لګښت;
		sd->وروستی_decay_max_lb_لګښت = now;

	} وإلا إذا (وخت_after(now, بل_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_لګښت = (sd->max_newidle_lb_لګښت * 253) / 256;
		sd->وروستی_decay_max_lb_لګښت = now;
		أرجع true;
	}

	أرجع false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
ساكن فراغ sched_balance_domains(بنية rq *rq, تعداد cpu_idle_type idle)
{
	صحيح continue_balancing = 1;
	صحيح cpu = rq->cpu;
	صحيح busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	بینښه اوږد interval;
	بنية sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	بینښه اوږد بل_balance = jiffies + 60*HZ;
	صحيح update_بل_balance = 0;
	صحيح need_decay = 0;
	u64 max_لګښت = 0;

	rcu_read_قلف();
	for_each_domain(cpu, sd) {
		/*
		 * Decay the newidle max times here because this is a regular
		 * visit to all the domains.
		 */
		need_decay = update_newidle_لګښت(sd, 0, 0);
		max_لګښت += sd->max_newidle_lb_لګښت;

		/*
		 * Stop the load balance at this level. There is another
		 * CPU in our sched group which is doing load balancing more
		 * actively.
		 */
		إذا (!continue_balancing) {
			إذا (need_decay)
				استمر;
			اكسر;
		}

		interval = get_sd_balance_interval(sd, busy);
		إذا (وخت_after_eq(jiffies, sd->وروستی_balance + interval)) {
			إذا (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu);
				busy = !idle && !sched_idle_rq(rq);
			}
			sd->وروستی_balance = jiffies;
			interval = get_sd_balance_interval(sd, busy);
		}
		إذا (وخت_after(بل_balance, sd->وروستی_balance + interval)) {
			بل_balance = sd->وروستی_balance + interval;
			update_بل_balance = 1;
		}
	}
	إذا (need_decay) {
		/*
		 * Ensure the rq-wide value also decays but keep it at a
		 * reasonable floor to avoid funnies with rq->avg_idle.
		 */
		rq->max_idle_balance_لګښت =
			max((u64)sysctl_sched_migration_لګښت, max_لګښت);
	}
	rcu_read_unlock();

	/*
	 * next_balance will be updated only when there is a need.
	 * When the cpu is attached to null domain for ex, it will not be
	 * updated.
	 */
	إذا (likely(update_بل_balance))
		rq->بل_balance = بل_balance;

}

ساكن مضمن صحيح on_null_domain(بنية rq *rq)
{
	أرجع unlikely(!rcu_dereference_sched(rq->sd));
}

#که_تعریف_شوی CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
ساكن مضمن صحيح پیدا_کړه_new_ilb(فراغ)
{
	بنية cpumask *ilb_cpus;
	صحيح ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_ماسک);
	cpumask_and(ilb_cpus, nohz.idle_cpus_ماسک,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		إذا (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			إذا (sched_smt_فعال() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_ماسک(ilb_cpu));
			استمر;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		إذا (sched_smt_فعال() && !is_core_idle(ilb_cpu)) {
			إذا (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_ماسک(ilb_cpu));
			استمر;
		}

		أرجع ilb_cpu;
	}

	أرجع fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
ساكن فراغ kick_ilb(بینښه صحيح flags)
{
	صحيح ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	إذا (flags & NOHZ_BALANCE_KICK)
		nohz.بل_balance = jiffies+1;

	ilb_cpu = پیدا_کړه_new_ilb();
	إذا (ilb_cpu < 0)
		أرجع;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	إذا ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		أرجع;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	إذا (flags & NOHZ_KICK_ماسک)
		أرجع;

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
ساكن فراغ nohz_balancer_kick(بنية rq *rq)
{
	بینښه اوږد now = jiffies;
	بنية sched_domain_shared *sds;
	بنية sched_domain *sd;
	صحيح nr_busy, i, cpu = rq->cpu;
	بینښه صحيح flags = 0;

	إذا (unlikely(rq->idle_balance))
		أرجع;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	إذا (READ_ONCE(nohz.has_blocked_بارول) &&
	    وخت_after(now, READ_ONCE(nohz.بل_blocked)))
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
	إذا (وخت_before(now, nohz.بل_balance))
		اذهب بهر;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	إذا (unlikely(cpumask_تشه(nohz.idle_cpus_ماسک)))
		أرجع;

	إذا (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		اذهب بهر;
	}

	sd = rcu_dereference_all(rq->sd);
	إذا (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		إذا (rq->cfs.h_nr_runnable >= 1 && check_cpu_ظرفیت(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			اذهب بهر;
		}
	}

	sd = rcu_dereference_all(فی_cpu(sd_asym_packing, cpu));
	إذا (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_پراختیا(sd), nohz.idle_cpus_ماسک) {
			إذا (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				اذهب بهر;
			}
		}
	}

	sd = rcu_dereference_all(فی_cpu(sd_asym_cpucapacity, cpu));
	إذا (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		إذا (check_misfit_حالت(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		اذهب بهر;
	}

	sds = rcu_dereference_all(فی_cpu(sd_balance_shared, cpu));
	إذا (sds) {
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
		إذا (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
بهر:
	إذا (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_بل_KICK;

	إذا (flags)
		kick_ilb(flags);
}

ساكن فراغ سېټ_cpu_sd_state_busy(صحيح cpu)
{
	بنية sched_domain *sd;
	sd = rcu_dereference_all(فی_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	إذا (!sd || !sd->shared || !sd->nohz_idle)
		أرجع;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

فراغ nohz_balance_exit_idle(بنية rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	إذا (likely(!rq->nohz_tick_stopped))
		أرجع;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_ماسک);

	سېټ_cpu_sd_state_busy(rq->cpu);
}

ساكن فراغ سېټ_cpu_sd_state_idle(صحيح cpu)
{
	بنية sched_domain *sd;
	sd = rcu_dereference_all(فی_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	إذا (!sd || !sd->shared || sd->nohz_idle)
		أرجع;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
فراغ nohz_balance_enter_idle(صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	إذا (!cpu_فعال(cpu))
		أرجع;

	/*
	 * Can be set safely without rq->lock held
	 * If a clear happens, it will have evaluated last additions because
	 * rq->lock is held during the check and the clear
	 */
	rq->has_blocked_بارول = 1;

	/*
	 * The tick is still stopped but load could have been added in the
	 * meantime. We set the nohz.has_blocked_load flag to trig a check of the
	 * *_avg. The CPU is already part of nohz.idle_cpus_mask so the clear
	 * of nohz.has_blocked_load can only happen after checking the new load
	 */
	إذا (rq->nohz_tick_stopped)
		اذهب بهر;

	/* If we're a completely isolated CPU, we don't play: */
	إذا (on_null_domain(rq))
		أرجع;

	rq->nohz_tick_stopped = 1;

	cpumask_سېټ_cpu(cpu, nohz.idle_cpus_ماسک);

	/*
	 * Ensures that if nohz_idle_balance() fails to observe our
	 * @idle_cpus_mask store, it must observe the @has_blocked_load
	 * and @needs_update stores.
	 */
	smp_mb__after_atomic();

	سېټ_cpu_sd_state_idle(cpu);

	WRITE_ONCE(nohz.needs_update, 1);
بهر:
	/*
	 * Each time a cpu enter idle, we assume that it has blocked load and
	 * enable the periodic update of the load of idle CPUs
	 */
	WRITE_ONCE(nohz.has_blocked_بارول, 1);
}

ساكن bool update_nohz_stats(بنية rq *rq)
{
	بینښه صحيح cpu = rq->cpu;

	إذا (!rq->has_blocked_بارول)
		أرجع false;

	إذا (!cpumask_test_cpu(cpu, nohz.idle_cpus_ماسک))
		أرجع false;

	إذا (!وخت_after(jiffies, READ_ONCE(rq->وروستی_blocked_بارول_update_tick)))
		أرجع true;

	sched_balance_update_blocked_averages(cpu);

	أرجع rq->has_blocked_بارول;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
ساكن فراغ _nohz_idle_balance(بنية rq *this_rq, بینښه صحيح flags)
{
	/* Earliest time when we have to do rebalance again */
	بینښه اوږد now = jiffies;
	بینښه اوږد بل_balance = now + 60*HZ;
	bool has_blocked_بارول = false;
	صحيح update_بل_balance = 0;
	صحيح this_cpu = this_rq->cpu;
	صحيح balance_cpu;
	بنية rq *rq;

	WARN_ON_ONCE((flags & NOHZ_KICK_ماسک) == NOHZ_BALANCE_KICK);

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
	إذا (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_بارول, 0);
	إذا (flags & NOHZ_بل_KICK)
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
	for_each_cpu_wrap(balance_cpu,  nohz.idle_cpus_ماسک, this_cpu+1) {
		إذا (!idle_cpu(balance_cpu))
			استمر;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		إذا (!idle_cpu(this_cpu) && need_resched()) {
			إذا (flags & NOHZ_STATS_KICK)
				has_blocked_بارول = true;
			إذا (flags & NOHZ_بل_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			اذهب abort;
		}

		rq = cpu_rq(balance_cpu);

		إذا (flags & NOHZ_STATS_KICK)
			has_blocked_بارول |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		إذا (وخت_after_eq(jiffies, rq->بل_balance)) {
			بنية rq_flags rf;

			rq_قلف_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			إذا (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		إذا (وخت_after(بل_balance, rq->بل_balance)) {
			بل_balance = rq->بل_balance;
			update_بل_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	إذا (likely(update_بل_balance))
		nohz.بل_balance = بل_balance;

	إذا (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.بل_blocked,
			   now + msecs_منزل_jiffies(بارول_AVG_PERIOD));

abort:
	/* There is still blocked load, enable periodic update */
	إذا (has_blocked_بارول)
		WRITE_ONCE(nohz.has_blocked_بارول, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
ساكن bool nohz_idle_balance(بنية rq *this_rq, تعداد cpu_idle_type idle)
{
	بینښه صحيح flags = this_rq->nohz_idle_balance;

	إذا (!flags)
		أرجع false;

	this_rq->nohz_idle_balance = 0;

	إذا (idle != CPU_IDLE)
		أرجع false;

	_nohz_idle_balance(this_rq, flags);

	أرجع true;
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
فراغ nohz_پرلپسې_idle_balance(صحيح cpu)
{
	بینښه صحيح flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	إذا ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

ساكن فراغ nohz_newidle_balance(بنية rq *this_rq)
{
	صحيح this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	إذا (this_rq->avg_idle < sysctl_sched_migration_لګښت)
		أرجع;

	/* Don't need to update blocked load of idle CPUs*/
	إذا (!READ_ONCE(nohz.has_blocked_بارول) ||
	    وخت_before(jiffies, READ_ONCE(nohz.بل_blocked)))
		أرجع;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#وإلا /* !CONFIG_NO_HZ_COMMON: */
ساكن مضمن فراغ nohz_balancer_kick(بنية rq *rq) { }

ساكن مضمن bool nohz_idle_balance(بنية rq *this_rq, تعداد cpu_idle_type idle)
{
	أرجع false;
}

ساكن مضمن فراغ nohz_newidle_balance(بنية rq *this_rq) { }
#د_که_پای /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
ساكن صحيح sched_balance_newidle(بنية rq *this_rq, بنية rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	بینښه اوږد بل_balance = jiffies + HZ;
	صحيح this_cpu = this_rq->cpu;
	صحيح continue_balancing = 1;
	u64 t0, t1, curr_لګښت = 0;
	بنية sched_domain *sd;
	صحيح pulled_task = 0;

	update_misfit_حالت(عدم, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	إذا (this_rq->ttwu_pending)
		أرجع 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	إذا (!cpu_preferred(this_cpu))
		أرجع 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_قلف(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	إذا (!sd)
		اذهب بهر;

	إذا (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_لګښت) {

		update_بل_balance(sd, &بل_balance);
		اذهب بهر;
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
		u64 domain_لګښت;

		update_بل_balance(sd, &بل_balance);

		إذا (this_rq->avg_idle < curr_لګښت + sd->max_newidle_lb_لګښت)
			اكسر;

		إذا (sd->flags & SD_BALANCE_NEWIDLE) {
			بینښه صحيح وزن = 1;

			إذا (sched_feat(NI_RANDOM) && sd->newidle_نسبت < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				وزن = 1 + sd->newidle_نسبت;
				إذا (d1k > وزن) {
					update_newidle_stats(sd, 0);
					استمر;
				}
				وزن = (1024 + وزن/2) / وزن;
			}

			pulled_task = sched_balance_rq(this_cpu, this_rq,
						   sd, CPU_NEWLY_IDLE,
						   &continue_balancing);

			t1 = sched_clock_cpu(this_cpu);
			domain_لګښت = t1 - t0;
			curr_لګښت += domain_لګښت;
			t0 = t1;

			/*
			 * Track max cost of a domain to make sure to not delay the
			 * next wakeup on the CPU.
			 */
			update_newidle_لګښت(sd, domain_لګښت, وزن * !!pulled_task);
		}

		/*
		 * Stop searching for tasks to pull if there are
		 * now runnable tasks on this rq.
		 */
		إذا (pulled_task || !continue_balancing)
			اكسر;
	}

	raw_spin_rq_قلف(this_rq);

	إذا (curr_لګښت > this_rq->max_idle_balance_لګښت)
		this_rq->max_idle_balance_لګښت = curr_لګښت;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	إذا (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	إذا (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

بهر:
	/* Move the next balance forward */
	إذا (وخت_after(this_rq->بل_balance, بل_balance))
		this_rq->بل_balance = بل_balance;

	إذا (pulled_task)
		this_rq->idle_stamp = 0;
	وإلا
		nohz_newidle_balance(this_rq);

	rq_repin_قلف(this_rq, rf);

	أرجع pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
ساكن __latent_entropy فراغ sched_balance_softirq(فراغ)
{
	بنية rq *this_rq = this_rq();
	تعداد cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	إذا (nohz_idle_balance(this_rq, idle))
		أرجع;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
فراغ sched_balance_trigger(بنية rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	إذا (unlikely(on_null_domain(rq) || !cpu_فعال(cpu_د(rq))))
		أرجع;

	إذا (وخت_after_eq(jiffies, rq->بل_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

ساكن فراغ rq_online_fair(بنية rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

ساكن فراغ rq_offline_fair(بنية rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#که_تعریف_شوی CONFIG_SCHED_CORE
ساكن مضمن bool
__entity_slice_used(بنية sched_entity *se, صحيح min_nr_tasks)
{
	u64 rtime = se->مجموعه_exec_runtime - se->prev_مجموعه_exec_runtime;
	u64 ټوټه = se->ټوټه;

	أرجع (rtime * min_nr_tasks > ټوټه);
}

#تعریف_کړه MIN_NR_TASKS_DURING_FORCEIDLE	2
ساكن مضمن فراغ task_tick_core(بنية rq *rq, بنية task_struct *curr)
{
	إذا (!sched_core_enabled(rq))
		أرجع;

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
	إذا (rq->core->core_forceidle_شمېر && rq->cfs.h_nr_queued == 1 &&
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
ساكن فراغ se_fi_update(ثابت بنية sched_entity *se, بینښه صحيح fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		بنية cfs_rq *cfs_rq = cfs_rq_د(se);

		إذا (forceidle) {
			إذا (cfs_rq->forceidle_seq == fi_seq)
				اكسر;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

فراغ task_vruntime_update(بنية rq *rq, بنية task_struct *p, bool دننه_fi)
{
	بنية sched_entity *se = &p->se;

	إذا (p->sched_class != &fair_sched_class)
		أرجع;

	se_fi_update(se, rq->core->core_forceidle_seq, دننه_fi);
}

bool cfs_prio_less(ثابت بنية task_struct *a, ثابت بنية task_struct *b,
			bool دننه_fi)
{
	بنية rq *rq = task_rq(a);
	ثابت بنية sched_entity *sea = &a->se;
	ثابت بنية sched_entity *seb = &b->se;
	بنية cfs_rq *cfs_rqa;
	بنية cfs_rq *cfs_rqb;
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

	أرجع delta > 0;
}

ساكن صحيح task_is_throttled_fair(بنية task_struct *p, صحيح cpu)
{
	بنية cfs_rq *cfs_rq;

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	cfs_rq = tg_cfs_rq(task_ډله(p), cpu);
#وإلا
	cfs_rq = &cpu_rq(cpu)->cfs;
#د_که_پای
	أرجع throttled_hierarchy(cfs_rq);
}
#وإلا /* !CONFIG_SCHED_CORE: */
ساكن مضمن فراغ task_tick_core(بنية rq *rq, بنية task_struct *curr) {}
#د_که_پای /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ساكن فراغ task_tick_fair(بنية rq *rq, بنية task_struct *curr, صحيح queued)
{
	بنية sched_entity *se = &curr->se;

	إذا (se->on_rq) {
		بینښه اوږد وزن = NICE_0_بارول;
		بنية cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_د(se);
			entity_tick(cfs_rq, se, queued);

			وزن = __calc_prop_weight(cfs_rq, se, وزن);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, وزن, se->on_rq);
	}

	إذا (queued)
		أرجع;

	إذا (static_branch_unlikely(&sched_numa_balancing))
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
ساكن فراغ task_fork_fair(بنية task_struct *p)
{
	سېټ_task_max_allowed_ظرفیت(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
ساكن فراغ
prio_بدل_شوی_fair(بنية rq *rq, بنية task_struct *p, u64 oldprio)
{
	إذا (!task_on_rq_queued(p))
		أرجع;

	إذا (p->prio == oldprio)
		أرجع;

	إذا (rq->cfs.h_nr_queued == 1)
		أرجع;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	إذا (task_current_donor(rq, p)) {
		إذا (p->prio > oldprio)
			resched_curr(rq);
	} وإلا {
		wakeup_preempt(rq, p, 0);
	}
}

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
ساكن فراغ propagate_entity_cfs_rq(بنية sched_entity *se)
{
	بنية cfs_rq *cfs_rq = cfs_rq_د(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	إذا (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_جمع_کړه_ونې_پاڼه_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->پلار;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_د(se);

		update_بارول_avg(cfs_rq, se, UPDATE_TG);

		إذا (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_جمع_کړه_ونې_پاڼه_cfs_rq(cfs_rq);
	}

	assert_list_ونې_پاڼه_cfs_rq(rq_د(cfs_rq));
}
#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */
ساكن فراغ propagate_entity_cfs_rq(بنية sched_entity *se) { }
#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

ساكن فراغ detach_entity_cfs_rq(بنية sched_entity *se)
{
	بنية cfs_rq *cfs_rq = cfs_rq_د(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	إذا (!se->avg.وروستی_update_وخت)
		أرجع;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_بارول_avg(cfs_rq, se, 0);
	detach_entity_بارول_avg(cfs_rq, se);
	update_tg_بارول_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ساكن فراغ attach_entity_cfs_rq(بنية sched_entity *se)
{
	بنية cfs_rq *cfs_rq = cfs_rq_د(se);

	/* Synchronize entity with its cfs_rq */
	update_بارول_avg(cfs_rq, se, sched_feat(ATTACH_AGE_بارول) ? 0 : SKIP_AGE_بارول);
	attach_entity_بارول_avg(cfs_rq, se);
	update_tg_بارول_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ساكن فراغ detach_task_cfs_rq(بنية task_struct *p)
{
	بنية sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

ساكن فراغ attach_task_cfs_rq(بنية task_struct *p)
{
	بنية sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

ساكن فراغ switching_from_fair(بنية rq *rq, بنية task_struct *p)
{
	إذا (p->se.sched_delayed)
		له_کتار_ایستل_task(rq, p, له_کتار_ایستل_SLEEP | له_کتار_ایستل_DELAYED | له_کتار_ایستل_NOCLOCK);
}

ساكن فراغ switched_from_fair(بنية rq *rq, بنية task_struct *p)
{
	detach_task_cfs_rq(p);
}

ساكن فراغ switched_منزل_fair(بنية rq *rq, بنية task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	سېټ_task_max_allowed_ظرفیت(p);

	إذا (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		إذا (task_current_donor(rq, p))
			resched_curr(rq);
		وإلا
			wakeup_preempt(rq, p, 0);
	}
}

ساكن فراغ سېټ_بل_task_fair(بنية rq *rq, بنية task_struct *p, تعداد snt_e type)
{
	بنية sched_entity *se = &p->se;
	بنية cfs_rq *cfs_rq = &rq->cfs;
	بینښه اوږد وزن = NICE_0_بارول;
	bool لومړی = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	إذا (type == SNT_REPICK)
		اذهب repick;

	clear_buddies(cfs_rq, se);

	إذا (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_د(se);

		إذا (!IS_ENABLED(CONFIG_FAIR_ډله_SCHED) ||
		    !لومړی || !cfs_rq->h_curr)
			سېټ_بل_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		إذا (on_rq)
			وزن = __calc_prop_weight(cfs_rq, se, وزن);
	}

	إذا (throttled)
		task_throttle_تیاری_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	إذا (on_rq) {
		reweight_eevdf(cfs_rq, se, وزن, se->on_rq);
		إذا (لومړی)
			سېټ_protect_ټوټه(cfs_rq, se);
	}

	إذا (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->ډله_غوټه, &rq->cfs_tasks);
	}
	إذا (!لومړی)
		أرجع;

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
	إذا (hrtick_enabled_fair(rq))
		hrtick_پیل_fair(rq, p);
}

فراغ init_cfs_rq(بنية cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_ریښه_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_قلف_init(&cfs_rq->removed.قلف);
}

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
ساكن فراغ task_change_ډله_fair(بنية task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	إذا (READ_ONCE(p->__state) == TASK_NEW)
		أرجع;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.وروستی_update_وخت = 0;
	سېټ_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

فراغ free_fair_sched_ډله(بنية task_ډله *tg)
{
	free_percpu(tg->cfs_rq);
}

صحيح alloc_fair_sched_ډله(بنية task_ډله *tg, بنية task_ډله *پلار)
{
	بنية cfs_tg_state __percpu *state;
	بنية sched_entity *se;
	بنية cfs_rq *cfs_rq;
	صحيح i;

	state = alloc_percpu_gfp(بنية cfs_tg_state, GFP_KERNEL);
	إذا (!state)
		اذهب err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_بارول;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(پلار));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		إذا (!cfs_rq)
			اذهب err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_ثبت(tg, cfs_rq, se, i, tg_se(پلار, i));
		init_entity_runnable_اوسط(se);
	}

	أرجع 1;

err:
	أرجع 0;
}

فراغ online_fair_sched_ډله(بنية task_ډله *tg)
{
	بنية sched_entity *se;
	بنية rq_flags rf;
	بنية rq *rq;
	صحيح i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg_se(tg, i);
		rq_قلف_irq(rq, &rf);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		rq_unlock_irq(rq, &rf);
	}
}

فراغ unregister_fair_sched_ډله(بنية task_ډله *tg)
{
	صحيح cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		بنية cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		بنية sched_entity *se = tg_se(tg, cpu);
		بنية rq *rq = cpu_rq(cpu);

		إذا (se)
			remove_entity_بارول_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		إذا (cfs_rq->on_list) {
			guard(rq_قلف_irqsave)(rq);
			list_del_ونې_پاڼه_cfs_rq(cfs_rq);
		}
	}
}

فراغ init_tg_cfs_ثبت(بنية task_ډله *tg, بنية cfs_rq *cfs_rq,
			بنية sched_entity *se, صحيح cpu,
			بنية sched_entity *پلار)
{
	بنية rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	إذا (!se)
		أرجع;

	إذا (!پلار) {
		se->cfs_rq = &rq->cfs;
		se->ژوروالی = 0;
	} وإلا {
		se->cfs_rq = پلار->my_q;
		se->ژوروالی = پلار->ژوروالی + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_بارول_سېټ(&se->بارول, NICE_0_بارول);
	se->پلار = پلار;
}

ساكن DEFINE_MUTEX(shares_mutex);

ساكن صحيح __sched_group_set_shares(بنية task_ډله *tg, بینښه اوږد shares)
{
	صحيح i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	إذا (is_ریښه_task_ډله(tg))
		أرجع -EINVAL;

	shares = clamp(shares, scale_بارول(MIN_SHARES), scale_بارول(MAX_SHARES));

	إذا (tg->shares == shares)
		أرجع 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		بنية rq *rq = cpu_rq(i);
		بنية sched_entity *se = tg_se(tg, i);
		بنية rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_قلف_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_بارول_avg(cfs_rq_د(se), se, UPDATE_TG);
			update_cfs_ډله(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	أرجع 0;
}

صحيح sched_ډله_سېټ_shares(بنية task_ډله *tg, بینښه اوږد shares)
{
	صحيح ret;

	mutex_قلف(&shares_mutex);
	إذا (tg_is_idle(tg))
		ret = -EINVAL;
	وإلا
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	أرجع ret;
}

صحيح sched_ډله_سېټ_idle(بنية task_ډله *tg, اوږد idle)
{
	صحيح i;

	إذا (tg == &ریښه_task_ډله)
		أرجع -EINVAL;

	إذا (idle < 0 || idle > 1)
		أرجع -EINVAL;

	mutex_قلف(&shares_mutex);

	إذا (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		أرجع 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		بنية rq *rq = cpu_rq(i);
		بنية sched_entity *se = tg_se(tg, i);
		بنية cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		اوږد idle_task_delta;
		بنية rq_flags rf;

		rq_قلف_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		إذا (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			اذهب بل_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		إذا (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			بنية cfs_rq *cfs_rq = cfs_rq_د(se);

			إذا (!se->on_rq)
				اكسر;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			إذا (cfs_rq_is_idle(cfs_rq))
				اكسر;
		}

بل_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	إذا (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_بارول(وزن_IDLEPRIO));
	وإلا
		__sched_group_set_shares(tg, NICE_0_بارول);

	mutex_unlock(&shares_mutex);
	أرجع 0;
}

#د_که_پای /* CONFIG_FAIR_GROUP_SCHED */


ساكن بینښه صحيح get_rr_interval_fair(بنية rq *rq, بنية task_struct *task)
{
	بنية sched_entity *se = &task->se;
	بینښه صحيح rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	إذا (rq->cfs.بارول.وزن)
		rr_interval = NS_منزل_JIFFIES(se->ټوټه);

	أرجع rr_interval;
}

/*
 * All the scheduling class methods:
 */
DEFINE_SCHED_CLASS(fair) = {
	.کتار_ته_ورزیاتول_task		= کتار_ته_ورزیاتول_task_fair,
	.له_کتار_ایستل_task		= له_کتار_ایستل_task_fair,
	.yield_task		= yield_task_fair,
	.yield_منزل_task		= yield_منزل_task_fair,

	.wakeup_preempt		= wakeup_preempt_fair,

	.pick_task		= pick_task_fair,
	.put_prev_task		= put_prev_task_fair,
	.سېټ_بل_task          = سېټ_بل_task_fair,

	.select_task_rq		= select_task_rq_fair,
	.migrate_task_rq	= migrate_task_rq_fair,

	.rq_online		= rq_online_fair,
	.rq_offline		= rq_offline_fair,

	.task_dead		= task_dead_fair,
	.سېټ_cpus_allowed	= سېټ_cpus_allowed_fair,

	.task_tick		= task_tick_fair,
	.task_fork		= task_fork_fair,

	.reweight_task		= reweight_task_fair,
	.prio_بدل_شوی		= prio_بدل_شوی_fair,
	.switching_from		= switching_from_fair,
	.switched_from		= switched_from_fair,
	.switched_منزل		= switched_منزل_fair,

	.get_rr_interval	= get_rr_interval_fair,

	.update_curr		= update_curr_fair,

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	.task_change_ډله	= task_change_ډله_fair,
#د_که_پای

#که_تعریف_شوی CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#د_که_پای

#که_تعریف_شوی CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#د_که_پای
};

فراغ print_cfs_stats(بنية seq_file *m, صحيح cpu)
{
	بنية cfs_rq *cfs_rq, *pos;

	rcu_read_قلف();
	for_each_ونې_پاڼه_cfs_rq_خوندي(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#که_تعریف_شوی CONFIG_NUMA_BALANCING
فراغ show_numa_stats(بنية task_struct *p, بنية seq_file *m)
{
	صحيح غوټه;
	بینښه اوږد tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	بنية numa_ډله *ng;

	rcu_read_قلف();
	ng = rcu_dereference_all(p->numa_ډله);
	for_each_online_غوټه(غوټه) {
		إذا (p->numa_تېروتنې) {
			tsf = p->numa_تېروتنې[task_تېروتنې_idx(NUMA_MEM, غوټه, 0)];
			tpf = p->numa_تېروتنې[task_تېروتنې_idx(NUMA_MEM, غوټه, 1)];
		}
		إذا (ng) {
			gsf = ng->تېروتنې[task_تېروتنې_idx(NUMA_MEM, غوټه, 0)];
			gpf = ng->تېروتنې[task_تېروتنې_idx(NUMA_MEM, غوټه, 1)];
		}
		print_numa_stats(m, غوټه, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#د_که_پای /* CONFIG_NUMA_BALANCING */

__init فراغ init_sched_fair_class(فراغ)
{
	صحيح i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_غوټه(&فی_cpu(بارول_balance_ماسک, i), GFP_KERNEL, cpu_منزل_غوټه(i));
		zalloc_cpumask_var_غوټه(&فی_cpu(select_rq_ماسک,    i), GFP_KERNEL, cpu_منزل_غوټه(i));
		zalloc_cpumask_var_غوټه(&فی_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_منزل_غوټه(i));

#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_سر(&cpu_rq(i)->cfsb_csd_list);
#د_که_پای
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#که_تعریف_شوی CONFIG_NO_HZ_COMMON
	nohz.بل_balance = jiffies;
	nohz.بل_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_ماسک, GFP_NOWAIT);
#د_که_پای
}
