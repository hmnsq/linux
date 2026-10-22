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

#ಅನ್ತರ್ಭಾವ "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

ಸಂರಚನಾ scx_sched *scx_skip_subtree_pre(ಸಂರಚನಾ scx_sched *pos, ಸಂರಚನಾ scx_sched *ಬೇರು);
ಸಂರಚನಾ scx_sched *scx_next_descendant_pre(ಸಂರಚನಾ scx_sched *pos, ಸಂರಚನಾ scx_sched *ಬೇರು);
ಶೂನ್ಯ scx_set_task_sched(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ scx_sched *sch);
ಸಂರಚನಾ cgroup *sch_cgroup(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ set_cgroup_sched(ಸಂರಚನಾ cgroup *cgrp, ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_pstack_recursion_on_dispatch(ಸಂರಚನಾ bpf_prog *prog);
ಶೂನ್ಯ scx_pstack_recursion_on_caps_updated(ಸಂರಚನಾ bpf_prog *prog);
ಶೂನ್ಯ drain_descendants(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_sub_disable(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_sub_enable_workfn(ಸಂರಚನಾ kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ಧ್ರುವ ಸಂರಚನಾ bpf_prog_aux *aux);
ಶೂನ್ಯ scx_free_pshards(ಸಂರಚನಾ scx_sched *sch);
s32 scx_alloc_pshards(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_init_ಬೇರು_caps(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_process_sync_ecaps(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *prev);
ಶೂನ್ಯ scx_unbypass_replay_ecaps(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_online_ecaps(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_offline_ecaps(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_discard_ecaps_to_sync(s32 cpu, ಸಂರಚನಾ scx_sched_pcpu *pcpu);
ಶೂನ್ಯ scx_discard_stale_ecaps_syncs(ಶೂನ್ಯ);
ಸಂರಚನಾ scx_dispatch_q *scx_resolve_local_dsq(ಸಂರಚನಾ scx_sched *sch, ಸಂರಚನಾ rq *rq,
					     ಸಂರಚನಾ task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p);
ಶೂನ್ಯ scx_reenq_reject(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_rescue_charge(ಸಂರಚನಾ rq *rq, s64 delta_exec);
ಶೂನ್ಯ scx_rescue_end(ಸಂರಚನಾ rq *rq);
bool scx_rescue_keep(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p);
ಶೂನ್ಯ scx_rescue_flush(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_rescue_dump(ಸಂರಚನಾ seq_buf *s, ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_rescue_set_knobs(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_rescue_init(ಸಂರಚನಾ rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ scx_sched *scx_cgroup_sched(ಸಂರಚನಾ cgroup *cgrp)
{
	ಪ್ರತ್ಯಾಗಮ rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *sch_cgrp_path(ಸಂರಚನಾ scx_sched *sch)
{
	ಪ್ರತ್ಯಾಗಮ sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_dec_has_subs(ಸಂರಚನಾ scx_sched *sch)
{
	ಯದಿ (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#ಅನ್ಯಥಾ	/* CONFIG_EXT_SUB_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ scx_sched *scx_next_descendant_pre(ಸಂರಚನಾ scx_sched *pos, ಸಂರಚನಾ scx_sched *ಬೇರು) { ಪ್ರತ್ಯಾಗಮ pos ? ರಿಕ್ತ : ಬೇರು; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ scx_sched *scx_skip_subtree_pre(ಸಂರಚನಾ scx_sched *pos, ಸಂರಚನಾ scx_sched *ಬೇರು) { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_set_task_sched(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ scx_sched *sch) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cgroup *sch_cgroup(ಸಂರಚನಾ scx_sched *sch) { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ *sch_cgrp_path(ಸಂರಚನಾ scx_sched *sch) { ಪ್ರತ್ಯಾಗಮ "/"; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ set_cgroup_sched(ಸಂರಚನಾ cgroup *cgrp, ಸಂರಚನಾ scx_sched *sch) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ drain_descendants(ಸಂರಚನಾ scx_sched *sch) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_sub_disable(ಸಂರಚನಾ scx_sched *sch) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_free_pshards(ಸಂರಚನಾ scx_sched *sch) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ s32 scx_alloc_pshards(ಸಂರಚನಾ scx_sched *sch) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_init_ಬೇರು_caps(ಸಂರಚನಾ scx_sched *sch) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_process_sync_ecaps(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *prev) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_unbypass_replay_ecaps(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ scx_sched *sch) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_online_ecaps(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_offline_ecaps(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_discard_ecaps_to_sync(s32 cpu, ಸಂರಚನಾ scx_sched_pcpu *pcpu) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_discard_stale_ecaps_syncs(ಶೂನ್ಯ) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ scx_dispatch_q *scx_resolve_local_dsq(ಸಂರಚನಾ scx_sched *sch, ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, u64 *enq_flags) { ಪ್ರತ್ಯಾಗಮ &rq->scx.local_dsq; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool scx_task_reenq_on_cap_revoke(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p) { ಪ್ರತ್ಯಾಗಮ false; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_reenq_reject(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rescue_charge(ಸಂರಚನಾ rq *rq, s64 delta_exec) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rescue_end(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool scx_rescue_keep(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p) { ಪ್ರತ್ಯಾಗಮ false; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rescue_flush(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rescue_dump(ಸಂರಚನಾ seq_buf *s, ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rescue_set_knobs(ಸಂರಚನಾ scx_sched *sch) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rescue_init(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_dec_has_subs(ಸಂರಚನಾ scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, ಬೇರು)					\
	ಪ್ರತಿ ((pos) = scx_next_descendant_pre(ರಿಕ್ತ, (ಬೇರು)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (ಬೇರು)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 scx_missing_caps(ಸಂರಚನಾ scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	ಯದಿ (!scx_has_subs())
		ಪ್ರತ್ಯಾಗಮ 0;

	/* root holds every cap on every cpu */
	ಯದಿ (!sch->level)
		ಪ್ರತ್ಯಾಗಮ 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	ಪ್ರತ್ಯಾಗಮ needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	ಯದಿ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಯದಿ (enq_flags & SCX_ENQ_IMMED)
		ಪ್ರತ್ಯಾಗಮ SCX_CAP_ENQ_IMMED;
	ಪ್ರತ್ಯಾಗಮ SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 scx_caps_for_task(ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (p->scx.flags & SCX_TASK_IMMED)
		ಪ್ರತ್ಯಾಗಮ SCX_CAP_ENQ_IMMED;
	ಪ್ರತ್ಯಾಗಮ SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 scx_caps_for_preempt(ಸಂರಚನಾ scx_sched *sch, ಸಂರಚನಾ rq *rq, u64 enq_flags)
{
	ಸಂರಚನಾ task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	ಯದಿ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ಪ್ರತ್ಯಾಗಮ 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	ಯದಿ (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 scx_caps_implied(u64 cap)
{
	ವಿಕಲ್ಪ (cap) {
	ಪಕ್ಷ SCX_CAP_PREEMPT:
		ಪ್ರತ್ಯಾಗಮ SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	ಪಕ್ಷ SCX_CAP_ENQ:
		ಪ್ರತ್ಯಾಗಮ SCX_CAP_ENQ_IMMED;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool scx_task_can_stay_on_cpu(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (!scx_has_subs())
		ಪ್ರತ್ಯಾಗಮ true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	ಯದಿ (unlikely(is_migration_disabled(p)))
		ಪ್ರತ್ಯಾಗಮ true;

	ಪ್ರತ್ಯಾಗಮ likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_struct *scx_rescuee(ಸಂರಚನಾ rq *rq)
{
	lockdep_assert_rq_held(rq);

	ಯದಿ (!scx_has_subs())
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಪ್ರತ್ಯಾಗಮ rq->scx.rescue.curr;
}

#ಅನ್ಯಥಾ	/* CONFIG_EXT_SUB_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 scx_missing_caps(ಸಂರಚನಾ scx_sched *sch, s32 cpu, u64 needed) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 scx_caps_for_preempt(ಸಂರಚನಾ scx_sched *sch, ಸಂರಚನಾ rq *rq, u64 enq_flags) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool scx_task_can_stay_on_cpu(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p) { ಪ್ರತ್ಯಾಗಮ true; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_struct *scx_rescuee(ಸಂರಚನಾ rq *rq) { ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
