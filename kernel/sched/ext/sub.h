/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Sub-scheduler hierarchy support.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_SUB_H
#define _KERNEL_SCHED_EXT_SUB_H

#অন্তর্ভাব "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

কাঠামো scx_sched *scx_skip_subtree_pre(কাঠামো scx_sched *pos, কাঠামো scx_sched *মূল);
কাঠামো scx_sched *scx_পরবর্তী_descendant_pre(কাঠামো scx_sched *pos, কাঠামো scx_sched *মূল);
শূন্য scx_set_task_sched(কাঠামো task_struct *p, কাঠামো scx_sched *sch);
কাঠামো cgroup *sch_cgroup(কাঠামো scx_sched *sch);
শূন্য set_cgroup_sched(কাঠামো cgroup *cgrp, কাঠামো scx_sched *sch);
শূন্য scx_pstack_recursion_on_dispatch(কাঠামো bpf_prog *prog);
শূন্য scx_pstack_recursion_on_caps_updated(কাঠামো bpf_prog *prog);
শূন্য drain_descendants(কাঠামো scx_sched *sch);
শূন্য scx_sub_disable(কাঠামো scx_sched *sch);
শূন্য scx_sub_enable_workfn(কাঠামো kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ধ্রুবক কাঠামো bpf_prog_aux *aux);
শূন্য scx_free_pshards(কাঠামো scx_sched *sch);
s32 scx_alloc_pshards(কাঠামো scx_sched *sch);
শূন্য scx_init_মূল_caps(কাঠামো scx_sched *sch);
শূন্য scx_process_sync_ecaps(কাঠামো rq *rq, কাঠামো task_struct *পূর্ব);
শূন্য scx_unbypass_replay_ecaps(কাঠামো rq *rq, কাঠামো scx_sched *sch);
শূন্য scx_online_ecaps(কাঠামো rq *rq);
শূন্য scx_offline_ecaps(কাঠামো rq *rq);
শূন্য scx_discard_ecaps_to_sync(s32 cpu, কাঠামো scx_sched_pcpu *pcpu);
শূন্য scx_discard_stale_ecaps_syncs(শূন্য);
কাঠামো scx_dispatch_q *scx_resolve_local_dsq(কাঠামো scx_sched *sch, কাঠামো rq *rq,
					     কাঠামো task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(কাঠামো rq *rq, কাঠামো task_struct *p);
শূন্য scx_reenq_reject(কাঠামো rq *rq);
শূন্য scx_rescue_charge(কাঠামো rq *rq, s64 delta_exec);
শূন্য scx_rescue_end(কাঠামো rq *rq);
bool scx_rescue_keep(কাঠামো rq *rq, কাঠামো task_struct *p);
শূন্য scx_rescue_flush(কাঠামো rq *rq);
শূন্য scx_rescue_dump(কাঠামো seq_buf *s, কাঠামো rq *rq);
শূন্য scx_rescue_set_knobs(কাঠামো scx_sched *sch);
শূন্য scx_rescue_init(কাঠামো rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
স্থির অন্তঃরেখ কাঠামো scx_sched *scx_cgroup_sched(কাঠামো cgroup *cgrp)
{
	ফেরত rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

স্থির অন্তঃরেখ ধ্রুবক অক্ষর *sch_cgrp_path(কাঠামো scx_sched *sch)
{
	ফেরত sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
স্থির অন্তঃরেখ শূন্য scx_dec_has_subs(কাঠামো scx_sched *sch)
{
	যদি (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#অন্যথা	/* CONFIG_EXT_SUB_SCHED */

স্থির অন্তঃরেখ কাঠামো scx_sched *scx_পরবর্তী_descendant_pre(কাঠামো scx_sched *pos, কাঠামো scx_sched *মূল) { ফেরত pos ? ফাঁকা : মূল; }
স্থির অন্তঃরেখ কাঠামো scx_sched *scx_skip_subtree_pre(কাঠামো scx_sched *pos, কাঠামো scx_sched *মূল) { ফেরত ফাঁকা; }
স্থির অন্তঃরেখ শূন্য scx_set_task_sched(কাঠামো task_struct *p, কাঠামো scx_sched *sch) {}
স্থির অন্তঃরেখ কাঠামো cgroup *sch_cgroup(কাঠামো scx_sched *sch) { ফেরত ফাঁকা; }
স্থির অন্তঃরেখ ধ্রুবক অক্ষর *sch_cgrp_path(কাঠামো scx_sched *sch) { ফেরত "/"; }
স্থির অন্তঃরেখ শূন্য set_cgroup_sched(কাঠামো cgroup *cgrp, কাঠামো scx_sched *sch) {}
স্থির অন্তঃরেখ শূন্য drain_descendants(কাঠামো scx_sched *sch) { }
স্থির অন্তঃরেখ শূন্য scx_sub_disable(কাঠামো scx_sched *sch) { }
স্থির অন্তঃরেখ শূন্য scx_free_pshards(কাঠামো scx_sched *sch) {}
স্থির অন্তঃরেখ s32 scx_alloc_pshards(কাঠামো scx_sched *sch) { ফেরত 0; }
স্থির অন্তঃরেখ শূন্য scx_init_মূল_caps(কাঠামো scx_sched *sch) {}
স্থির অন্তঃরেখ শূন্য scx_process_sync_ecaps(কাঠামো rq *rq, কাঠামো task_struct *পূর্ব) {}
স্থির অন্তঃরেখ শূন্য scx_unbypass_replay_ecaps(কাঠামো rq *rq, কাঠামো scx_sched *sch) {}
স্থির অন্তঃরেখ শূন্য scx_online_ecaps(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_offline_ecaps(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_discard_ecaps_to_sync(s32 cpu, কাঠামো scx_sched_pcpu *pcpu) {}
স্থির অন্তঃরেখ শূন্য scx_discard_stale_ecaps_syncs(শূন্য) {}
স্থির অন্তঃরেখ কাঠামো scx_dispatch_q *scx_resolve_local_dsq(কাঠামো scx_sched *sch, কাঠামো rq *rq, কাঠামো task_struct *p, u64 *enq_flags) { ফেরত &rq->scx.local_dsq; }
স্থির অন্তঃরেখ bool scx_task_reenq_on_cap_revoke(কাঠামো rq *rq, কাঠামো task_struct *p) { ফেরত false; }
স্থির অন্তঃরেখ শূন্য scx_reenq_reject(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_rescue_charge(কাঠামো rq *rq, s64 delta_exec) {}
স্থির অন্তঃরেখ শূন্য scx_rescue_end(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ bool scx_rescue_keep(কাঠামো rq *rq, কাঠামো task_struct *p) { ফেরত false; }
স্থির অন্তঃরেখ শূন্য scx_rescue_flush(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_rescue_dump(কাঠামো seq_buf *s, কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_rescue_set_knobs(কাঠামো scx_sched *sch) {}
স্থির অন্তঃরেখ শূন্য scx_rescue_init(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_dec_has_subs(কাঠামো scx_sched *sch) {}

#endif	/* CONFIG_EXT_SUB_SCHED */

/**
 * scx_for_each_descendant_pre - pre-order walk of a sched's descendants
 * @pos: iteration cursor
 * @root: sched to walk the descendants of
 *
 * Walk @root's descendants. @root is included in the iteration and the first
 * node to be visited. Must be called with scx_enable_mutex, scx_sched_lock, or
 * RCU read lock.
 */
#define scx_for_each_descendant_pre(pos, মূল)					\
	জন্য ((pos) = scx_পরবর্তী_descendant_pre(ফাঁকা, (মূল)); (pos);		\
	     (pos) = scx_পরবর্তী_descendant_pre((pos), (মূল)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
স্থির অন্তঃরেখ u64 scx_missing_caps(কাঠামো scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	যদি (!scx_has_subs())
		ফেরত 0;

	/* root holds every cap on every cpu */
	যদি (!sch->level)
		ফেরত 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	ফেরত needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
স্থির অন্তঃরেখ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	যদি (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ফেরত 0;
	যদি (enq_flags & SCX_ENQ_IMMED)
		ফেরত SCX_CAP_ENQ_IMMED;
	ফেরত SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
স্থির অন্তঃরেখ u64 scx_caps_for_task(কাঠামো task_struct *p)
{
	যদি (p->scx.flags & SCX_TASK_IMMED)
		ফেরত SCX_CAP_ENQ_IMMED;
	ফেরত SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
স্থির অন্তঃরেখ u64 scx_caps_for_preempt(কাঠামো scx_sched *sch, কাঠামো rq *rq, u64 enq_flags)
{
	কাঠামো task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	যদি (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ফেরত 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	যদি (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		ফেরত 0;
	ফেরত SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
স্থির অন্তঃরেখ u64 scx_caps_implied(u64 cap)
{
	নির্বাচন (cap) {
	ক্ষেত্র SCX_CAP_PREEMPT:
		ফেরত SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	ক্ষেত্র SCX_CAP_ENQ:
		ফেরত SCX_CAP_ENQ_IMMED;
	}
	ফেরত 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
স্থির অন্তঃরেখ bool scx_task_can_stay_on_cpu(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	যদি (!scx_has_subs())
		ফেরত true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	যদি (unlikely(is_migration_disabled(p)))
		ফেরত true;

	ফেরত likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
স্থির অন্তঃরেখ কাঠামো task_struct *scx_rescuee(কাঠামো rq *rq)
{
	lockdep_assert_rq_held(rq);

	যদি (!scx_has_subs())
		ফেরত ফাঁকা;

	ফেরত rq->scx.rescue.curr;
}

#অন্যথা	/* CONFIG_EXT_SUB_SCHED */

স্থির অন্তঃরেখ u64 scx_missing_caps(কাঠামো scx_sched *sch, s32 cpu, u64 needed) { ফেরত 0; }
স্থির অন্তঃরেখ u64 scx_caps_for_preempt(কাঠামো scx_sched *sch, কাঠামো rq *rq, u64 enq_flags) { ফেরত 0; }
স্থির অন্তঃরেখ bool scx_task_can_stay_on_cpu(কাঠামো rq *rq, কাঠামো task_struct *p) { ফেরত true; }
স্থির অন্তঃরেখ কাঠামো task_struct *scx_rescuee(কাঠামো rq *rq) { ফেরত ফাঁকা; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
