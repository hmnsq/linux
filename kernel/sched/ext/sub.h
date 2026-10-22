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

#অন্তর্ভাৱ "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

সংরচনা scx_sched *scx_skip_subtree_pre(সংরচনা scx_sched *pos, সংরচনা scx_sched *মূল);
সংরচনা scx_sched *scx_next_descendant_pre(সংরচনা scx_sched *pos, সংরচনা scx_sched *মূল);
শূন্য scx_set_task_sched(সংরচনা task_struct *p, সংরচনা scx_sched *sch);
সংরচনা cgroup *sch_cgroup(সংরচনা scx_sched *sch);
শূন্য set_cgroup_sched(সংরচনা cgroup *cgrp, সংরচনা scx_sched *sch);
শূন্য scx_pstack_recursion_on_dispatch(সংরচনা bpf_prog *prog);
শূন্য scx_pstack_recursion_on_caps_updated(সংরচনা bpf_prog *prog);
শূন্য drain_descendants(সংরচনা scx_sched *sch);
শূন্য scx_sub_disable(সংরচনা scx_sched *sch);
শূন্য scx_sub_enable_workfn(সংরচনা kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ধ্রুৱ সংরচনা bpf_prog_aux *aux);
শূন্য scx_free_pshards(সংরচনা scx_sched *sch);
s32 scx_alloc_pshards(সংরচনা scx_sched *sch);
শূন্য scx_init_মূল_caps(সংরচনা scx_sched *sch);
শূন্য scx_process_sync_ecaps(সংরচনা rq *rq, সংরচনা task_struct *prev);
শূন্য scx_unbypass_replay_ecaps(সংরচনা rq *rq, সংরচনা scx_sched *sch);
শূন্য scx_online_ecaps(সংরচনা rq *rq);
শূন্য scx_offline_ecaps(সংরচনা rq *rq);
শূন্য scx_discard_ecaps_to_sync(s32 cpu, সংরচনা scx_sched_pcpu *pcpu);
শূন্য scx_discard_stale_ecaps_syncs(শূন্য);
সংরচনা scx_dispatch_q *scx_resolve_local_dsq(সংরচনা scx_sched *sch, সংরচনা rq *rq,
					     সংরচনা task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(সংরচনা rq *rq, সংরচনা task_struct *p);
শূন্য scx_reenq_reject(সংরচনা rq *rq);
শূন্য scx_rescue_charge(সংরচনা rq *rq, s64 delta_exec);
শূন্য scx_rescue_end(সংরচনা rq *rq);
bool scx_rescue_keep(সংরচনা rq *rq, সংরচনা task_struct *p);
শূন্য scx_rescue_flush(সংরচনা rq *rq);
শূন্য scx_rescue_dump(সংরচনা seq_buf *s, সংরচনা rq *rq);
শূন্য scx_rescue_set_knobs(সংরচনা scx_sched *sch);
শূন্য scx_rescue_init(সংরচনা rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
স্থির অন্তঃস্থ সংরচনা scx_sched *scx_cgroup_sched(সংরচনা cgroup *cgrp)
{
	ঘূৰাই rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

স্থির অন্তঃস্থ ধ্রুৱ ৱর্ণ *sch_cgrp_path(সংরচনা scx_sched *sch)
{
	ঘূৰাই sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
স্থির অন্তঃস্থ শূন্য scx_dec_has_subs(সংরচনা scx_sched *sch)
{
	যদি (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#অন্যথা	/* CONFIG_EXT_SUB_SCHED */

স্থির অন্তঃস্থ সংরচনা scx_sched *scx_next_descendant_pre(সংরচনা scx_sched *pos, সংরচনা scx_sched *মূল) { ঘূৰাই pos ? রিক্ত : মূল; }
স্থির অন্তঃস্থ সংরচনা scx_sched *scx_skip_subtree_pre(সংরচনা scx_sched *pos, সংরচনা scx_sched *মূল) { ঘূৰাই রিক্ত; }
স্থির অন্তঃস্থ শূন্য scx_set_task_sched(সংরচনা task_struct *p, সংরচনা scx_sched *sch) {}
স্থির অন্তঃস্থ সংরচনা cgroup *sch_cgroup(সংরচনা scx_sched *sch) { ঘূৰাই রিক্ত; }
স্থির অন্তঃস্থ ধ্রুৱ ৱর্ণ *sch_cgrp_path(সংরচনা scx_sched *sch) { ঘূৰাই "/"; }
স্থির অন্তঃস্থ শূন্য set_cgroup_sched(সংরচনা cgroup *cgrp, সংরচনা scx_sched *sch) {}
স্থির অন্তঃস্থ শূন্য drain_descendants(সংরচনা scx_sched *sch) { }
স্থির অন্তঃস্থ শূন্য scx_sub_disable(সংরচনা scx_sched *sch) { }
স্থির অন্তঃস্থ শূন্য scx_free_pshards(সংরচনা scx_sched *sch) {}
স্থির অন্তঃস্থ s32 scx_alloc_pshards(সংরচনা scx_sched *sch) { ঘূৰাই 0; }
স্থির অন্তঃস্থ শূন্য scx_init_মূল_caps(সংরচনা scx_sched *sch) {}
স্থির অন্তঃস্থ শূন্য scx_process_sync_ecaps(সংরচনা rq *rq, সংরচনা task_struct *prev) {}
স্থির অন্তঃস্থ শূন্য scx_unbypass_replay_ecaps(সংরচনা rq *rq, সংরচনা scx_sched *sch) {}
স্থির অন্তঃস্থ শূন্য scx_online_ecaps(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_offline_ecaps(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_discard_ecaps_to_sync(s32 cpu, সংরচনা scx_sched_pcpu *pcpu) {}
স্থির অন্তঃস্থ শূন্য scx_discard_stale_ecaps_syncs(শূন্য) {}
স্থির অন্তঃস্থ সংরচনা scx_dispatch_q *scx_resolve_local_dsq(সংরচনা scx_sched *sch, সংরচনা rq *rq, সংরচনা task_struct *p, u64 *enq_flags) { ঘূৰাই &rq->scx.local_dsq; }
স্থির অন্তঃস্থ bool scx_task_reenq_on_cap_revoke(সংরচনা rq *rq, সংরচনা task_struct *p) { ঘূৰাই false; }
স্থির অন্তঃস্থ শূন্য scx_reenq_reject(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_rescue_charge(সংরচনা rq *rq, s64 delta_exec) {}
স্থির অন্তঃস্থ শূন্য scx_rescue_end(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ bool scx_rescue_keep(সংরচনা rq *rq, সংরচনা task_struct *p) { ঘূৰাই false; }
স্থির অন্তঃস্থ শূন্য scx_rescue_flush(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_rescue_dump(সংরচনা seq_buf *s, সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_rescue_set_knobs(সংরচনা scx_sched *sch) {}
স্থির অন্তঃস্থ শূন্য scx_rescue_init(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_dec_has_subs(সংরচনা scx_sched *sch) {}

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
	বাবে ((pos) = scx_next_descendant_pre(রিক্ত, (মূল)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (মূল)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
স্থির অন্তঃস্থ u64 scx_missing_caps(সংরচনা scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	যদি (!scx_has_subs())
		ঘূৰাই 0;

	/* root holds every cap on every cpu */
	যদি (!sch->level)
		ঘূৰাই 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	ঘূৰাই needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
স্থির অন্তঃস্থ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	যদি (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ঘূৰাই 0;
	যদি (enq_flags & SCX_ENQ_IMMED)
		ঘূৰাই SCX_CAP_ENQ_IMMED;
	ঘূৰাই SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
স্থির অন্তঃস্থ u64 scx_caps_for_task(সংরচনা task_struct *p)
{
	যদি (p->scx.flags & SCX_TASK_IMMED)
		ঘূৰাই SCX_CAP_ENQ_IMMED;
	ঘূৰাই SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
স্থির অন্তঃস্থ u64 scx_caps_for_preempt(সংরচনা scx_sched *sch, সংরচনা rq *rq, u64 enq_flags)
{
	সংরচনা task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	যদি (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ঘূৰাই 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	যদি (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		ঘূৰাই 0;
	ঘূৰাই SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
স্থির অন্তঃস্থ u64 scx_caps_implied(u64 cap)
{
	ৱিকল্প (cap) {
	পক্ষ SCX_CAP_PREEMPT:
		ঘূৰাই SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	পক্ষ SCX_CAP_ENQ:
		ঘূৰাই SCX_CAP_ENQ_IMMED;
	}
	ঘূৰাই 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
স্থির অন্তঃস্থ bool scx_task_can_stay_on_cpu(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	যদি (!scx_has_subs())
		ঘূৰাই true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	যদি (unlikely(is_migration_disabled(p)))
		ঘূৰাই true;

	ঘূৰাই likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
স্থির অন্তঃস্থ সংরচনা task_struct *scx_rescuee(সংরচনা rq *rq)
{
	lockdep_assert_rq_held(rq);

	যদি (!scx_has_subs())
		ঘূৰাই রিক্ত;

	ঘূৰাই rq->scx.rescue.curr;
}

#অন্যথা	/* CONFIG_EXT_SUB_SCHED */

স্থির অন্তঃস্থ u64 scx_missing_caps(সংরচনা scx_sched *sch, s32 cpu, u64 needed) { ঘূৰাই 0; }
স্থির অন্তঃস্থ u64 scx_caps_for_preempt(সংরচনা scx_sched *sch, সংরচনা rq *rq, u64 enq_flags) { ঘূৰাই 0; }
স্থির অন্তঃস্থ bool scx_task_can_stay_on_cpu(সংরচনা rq *rq, সংরচনা task_struct *p) { ঘূৰাই true; }
স্থির অন্তঃস্থ সংরচনা task_struct *scx_rescuee(সংরচনা rq *rq) { ঘূৰাই রিক্ত; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
