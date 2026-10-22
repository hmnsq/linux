/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Sub-scheduler hierarchy support.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_EXT_SUB_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_EXT_SUB_H

#ಅನ್ತರ್ಭಾವ "internal.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED

ರಚನೆ scx_sched *scx_skip_subtree_pre(ರಚನೆ scx_sched *pos, ರಚನೆ scx_sched *ಬೇರು);
ರಚನೆ scx_sched *scx_ಮುಂದಿನ_descendant_pre(ರಚನೆ scx_sched *pos, ರಚನೆ scx_sched *ಬೇರು);
ಶೂನ್ಯ scx_ಗಣ_task_sched(ರಚನೆ task_struct *p, ರಚನೆ scx_sched *sch);
ರಚನೆ cgroup *sch_cgroup(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ ಗಣ_cgroup_sched(ರಚನೆ cgroup *cgrp, ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_pstack_recursion_on_dispatch(ರಚನೆ bpf_prog *prog);
ಶೂನ್ಯ scx_pstack_recursion_on_caps_updated(ರಚನೆ bpf_prog *prog);
ಶೂನ್ಯ drain_descendants(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_sub_disable(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_sub_enable_workfn(ರಚನೆ kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux);
ಶೂನ್ಯ scx_free_pshards(ರಚನೆ scx_sched *sch);
s32 scx_alloc_pshards(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_init_ಬೇರು_caps(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_process_sync_ecaps(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev);
ಶೂನ್ಯ scx_unbypass_replay_ecaps(ರಚನೆ rq *rq, ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_online_ecaps(ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_offline_ecaps(ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_discard_ecaps_ಗಮ್ಯ_sync(s32 cpu, ರಚನೆ scx_sched_pcpu *pcpu);
ಶೂನ್ಯ scx_discard_stale_ecaps_syncs(ಶೂನ್ಯ);
ರಚನೆ scx_dispatch_q *scx_resolve_local_dsq(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq,
					     ರಚನೆ task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(ರಚನೆ rq *rq, ರಚನೆ task_struct *p);
ಶೂನ್ಯ scx_reenq_reject(ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_rescue_charge(ರಚನೆ rq *rq, s64 delta_exec);
ಶೂನ್ಯ scx_rescue_ಕೊನೆ(ರಚನೆ rq *rq);
bool scx_rescue_keep(ರಚನೆ rq *rq, ರಚನೆ task_struct *p);
ಶೂನ್ಯ scx_rescue_flush(ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_rescue_dump(ರಚನೆ seq_buf *s, ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_rescue_ಗಣ_knobs(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_rescue_init(ರಚನೆ rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ scx_sched *scx_cgroup_sched(ರಚನೆ cgroup *cgrp)
{
	ಹಿಂತಿರುಗಿ rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sch_cgrp_ಮಾರ್ಗ(ರಚನೆ scx_sched *sch)
{
	ಹಿಂತಿರುಗಿ sch->cgrp_ಮಾರ್ಗ;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_dec_has_subs(ರಚನೆ scx_sched *sch)
{
	ಆದರೆ (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#ಇಲ್ಲದಿದ್ದರೆ	/* CONFIG_EXT_SUB_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ scx_sched *scx_ಮುಂದಿನ_descendant_pre(ರಚನೆ scx_sched *pos, ರಚನೆ scx_sched *ಬೇರು) { ಹಿಂತಿರುಗಿ pos ? ಖಾಲಿ : ಬೇರು; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ scx_sched *scx_skip_subtree_pre(ರಚನೆ scx_sched *pos, ರಚನೆ scx_sched *ಬೇರು) { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_ಗಣ_task_sched(ರಚನೆ task_struct *p, ರಚನೆ scx_sched *sch) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cgroup *sch_cgroup(ರಚನೆ scx_sched *sch) { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sch_cgrp_ಮಾರ್ಗ(ರಚನೆ scx_sched *sch) { ಹಿಂತಿರುಗಿ "/"; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_cgroup_sched(ರಚನೆ cgroup *cgrp, ರಚನೆ scx_sched *sch) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ drain_descendants(ರಚನೆ scx_sched *sch) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_sub_disable(ರಚನೆ scx_sched *sch) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_free_pshards(ರಚನೆ scx_sched *sch) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ s32 scx_alloc_pshards(ರಚನೆ scx_sched *sch) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_init_ಬೇರು_caps(ರಚನೆ scx_sched *sch) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_process_sync_ecaps(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_unbypass_replay_ecaps(ರಚನೆ rq *rq, ರಚನೆ scx_sched *sch) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_online_ecaps(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_offline_ecaps(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_discard_ecaps_ಗಮ್ಯ_sync(s32 cpu, ರಚನೆ scx_sched_pcpu *pcpu) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_discard_stale_ecaps_syncs(ಶೂನ್ಯ) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ scx_dispatch_q *scx_resolve_local_dsq(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 *enq_flags) { ಹಿಂತಿರುಗಿ &rq->scx.local_dsq; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_task_reenq_on_cap_revoke(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_reenq_reject(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rescue_charge(ರಚನೆ rq *rq, s64 delta_exec) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rescue_ಕೊನೆ(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_rescue_keep(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rescue_flush(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rescue_dump(ರಚನೆ seq_buf *s, ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rescue_ಗಣ_knobs(ರಚನೆ scx_sched *sch) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rescue_init(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_dec_has_subs(ರಚನೆ scx_sched *sch) {}

#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

/**
 * scx_for_each_descendant_pre - pre-order walk of a sched's descendants
 * @pos: iteration cursor
 * @root: sched to walk the descendants of
 *
 * Walk @root's descendants. @root is included in the iteration and the first
 * node to be visited. Must be called with scx_enable_mutex, scx_sched_lock, or
 * RCU read lock.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು scx_for_each_descendant_pre(pos, ಬೇರು)					\
	ಗಾಗಿ ((pos) = scx_ಮುಂದಿನ_descendant_pre(ಖಾಲಿ, (ಬೇರು)); (pos);		\
	     (pos) = scx_ಮುಂದಿನ_descendant_pre((pos), (ಬೇರು)))

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 scx_missing_caps(ರಚನೆ scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	ಆದರೆ (!scx_has_subs())
		ಹಿಂತಿರುಗಿ 0;

	/* root holds every cap on every cpu */
	ಆದರೆ (!sch->level)
		ಹಿಂತಿರುಗಿ 0;

	ecaps = READ_ONCE(ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu)->ecaps);

	ಹಿಂತಿರುಗಿ needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	ಆದರೆ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (enq_flags & SCX_ENQ_IMMED)
		ಹಿಂತಿರುಗಿ SCX_CAP_ENQ_IMMED;
	ಹಿಂತಿರುಗಿ SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 scx_caps_for_task(ರಚನೆ task_struct *p)
{
	ಆದರೆ (p->scx.flags & SCX_TASK_IMMED)
		ಹಿಂತಿರುಗಿ SCX_CAP_ENQ_IMMED;
	ಹಿಂತಿರುಗಿ SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 scx_caps_for_preempt(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq, u64 enq_flags)
{
	ರಚನೆ task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	ಆದರೆ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ಹಿಂತಿರುಗಿ 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	ಆದರೆ (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 scx_caps_implied(u64 cap)
{
	ಆಯ್ಕೆ (cap) {
	ಸ್ಥಿತಿ SCX_CAP_PREEMPT:
		ಹಿಂತಿರುಗಿ SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	ಸ್ಥಿತಿ SCX_CAP_ENQ:
		ಹಿಂತಿರುಗಿ SCX_CAP_ENQ_IMMED;
	}
	ಹಿಂತಿರುಗಿ 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_task_can_stay_on_cpu(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (!scx_has_subs())
		ಹಿಂತಿರುಗಿ true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	ಆದರೆ (unlikely(is_migration_disabled(p)))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ likely(!scx_missing_caps(scx_task_sched(p), cpu_ದ(rq), SCX_CAP_ತಳ));
}

/* the task admitted for rescue on @rq, NULL if none */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *scx_rescuee(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);

	ಆದರೆ (!scx_has_subs())
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ rq->scx.rescue.curr;
}

#ಇಲ್ಲದಿದ್ದರೆ	/* CONFIG_EXT_SUB_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 scx_missing_caps(ರಚನೆ scx_sched *sch, s32 cpu, u64 needed) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 scx_caps_for_preempt(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq, u64 enq_flags) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_task_can_stay_on_cpu(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ true; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *scx_rescuee(ರಚನೆ rq *rq) { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }

#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_EXT_SUB_H */
