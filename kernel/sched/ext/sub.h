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

#અન્તર્ભાવ "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

સંરચના scx_sched *scx_skip_subtree_pre(સંરચના scx_sched *pos, સંરચના scx_sched *મૂળ);
સંરચના scx_sched *scx_next_descendant_pre(સંરચના scx_sched *pos, સંરચના scx_sched *મૂળ);
શૂન્ય scx_set_task_sched(સંરચના task_struct *p, સંરચના scx_sched *sch);
સંરચના cgroup *sch_cgroup(સંરચના scx_sched *sch);
શૂન્ય set_cgroup_sched(સંરચના cgroup *cgrp, સંરચના scx_sched *sch);
શૂન્ય scx_pstack_recursion_on_dispatch(સંરચના bpf_prog *prog);
શૂન્ય scx_pstack_recursion_on_caps_updated(સંરચના bpf_prog *prog);
શૂન્ય drain_descendants(સંરચના scx_sched *sch);
શૂન્ય scx_sub_disable(સંરચના scx_sched *sch);
શૂન્ય scx_sub_enable_workfn(સંરચના kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, અચલ સંરચના bpf_prog_aux *aux);
શૂન્ય scx_free_pshards(સંરચના scx_sched *sch);
s32 scx_alloc_pshards(સંરચના scx_sched *sch);
શૂન્ય scx_init_મૂળ_caps(સંરચના scx_sched *sch);
શૂન્ય scx_process_sync_ecaps(સંરચના rq *rq, સંરચના task_struct *prev);
શૂન્ય scx_unbypass_replay_ecaps(સંરચના rq *rq, સંરચના scx_sched *sch);
શૂન્ય scx_online_ecaps(સંરચના rq *rq);
શૂન્ય scx_offline_ecaps(સંરચના rq *rq);
શૂન્ય scx_discard_ecaps_to_sync(s32 cpu, સંરચના scx_sched_pcpu *pcpu);
શૂન્ય scx_discard_stale_ecaps_syncs(શૂન્ય);
સંરચના scx_dispatch_q *scx_resolve_local_dsq(સંરચના scx_sched *sch, સંરચના rq *rq,
					     સંરચના task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(સંરચના rq *rq, સંરચના task_struct *p);
શૂન્ય scx_reenq_reject(સંરચના rq *rq);
શૂન્ય scx_rescue_charge(સંરચના rq *rq, s64 delta_exec);
શૂન્ય scx_rescue_end(સંરચના rq *rq);
bool scx_rescue_keep(સંરચના rq *rq, સંરચના task_struct *p);
શૂન્ય scx_rescue_flush(સંરચના rq *rq);
શૂન્ય scx_rescue_dump(સંરચના seq_buf *s, સંરચના rq *rq);
શૂન્ય scx_rescue_set_knobs(સંરચના scx_sched *sch);
શૂન્ય scx_rescue_init(સંરચના rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
સ્થિર અંતઃરેખ સંરચના scx_sched *scx_cgroup_sched(સંરચના cgroup *cgrp)
{
	પાછા rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

સ્થિર અંતઃરેખ અચલ અક્ષર *sch_cgrp_path(સંરચના scx_sched *sch)
{
	પાછા sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
સ્થિર અંતઃરેખ શૂન્ય scx_dec_has_subs(સંરચના scx_sched *sch)
{
	જો (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#નહીંતર	/* CONFIG_EXT_SUB_SCHED */

સ્થિર અંતઃરેખ સંરચના scx_sched *scx_next_descendant_pre(સંરચના scx_sched *pos, સંરચના scx_sched *મૂળ) { પાછા pos ? ખાલી : મૂળ; }
સ્થિર અંતઃરેખ સંરચના scx_sched *scx_skip_subtree_pre(સંરચના scx_sched *pos, સંરચના scx_sched *મૂળ) { પાછા ખાલી; }
સ્થિર અંતઃરેખ શૂન્ય scx_set_task_sched(સંરચના task_struct *p, સંરચના scx_sched *sch) {}
સ્થિર અંતઃરેખ સંરચના cgroup *sch_cgroup(સંરચના scx_sched *sch) { પાછા ખાલી; }
સ્થિર અંતઃરેખ અચલ અક્ષર *sch_cgrp_path(સંરચના scx_sched *sch) { પાછા "/"; }
સ્થિર અંતઃરેખ શૂન્ય set_cgroup_sched(સંરચના cgroup *cgrp, સંરચના scx_sched *sch) {}
સ્થિર અંતઃરેખ શૂન્ય drain_descendants(સંરચના scx_sched *sch) { }
સ્થિર અંતઃરેખ શૂન્ય scx_sub_disable(સંરચના scx_sched *sch) { }
સ્થિર અંતઃરેખ શૂન્ય scx_free_pshards(સંરચના scx_sched *sch) {}
સ્થિર અંતઃરેખ s32 scx_alloc_pshards(સંરચના scx_sched *sch) { પાછા 0; }
સ્થિર અંતઃરેખ શૂન્ય scx_init_મૂળ_caps(સંરચના scx_sched *sch) {}
સ્થિર અંતઃરેખ શૂન્ય scx_process_sync_ecaps(સંરચના rq *rq, સંરચના task_struct *prev) {}
સ્થિર અંતઃરેખ શૂન્ય scx_unbypass_replay_ecaps(સંરચના rq *rq, સંરચના scx_sched *sch) {}
સ્થિર અંતઃરેખ શૂન્ય scx_online_ecaps(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_offline_ecaps(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_discard_ecaps_to_sync(s32 cpu, સંરચના scx_sched_pcpu *pcpu) {}
સ્થિર અંતઃરેખ શૂન્ય scx_discard_stale_ecaps_syncs(શૂન્ય) {}
સ્થિર અંતઃરેખ સંરચના scx_dispatch_q *scx_resolve_local_dsq(સંરચના scx_sched *sch, સંરચના rq *rq, સંરચના task_struct *p, u64 *enq_flags) { પાછા &rq->scx.local_dsq; }
સ્થિર અંતઃરેખ bool scx_task_reenq_on_cap_revoke(સંરચના rq *rq, સંરચના task_struct *p) { પાછા false; }
સ્થિર અંતઃરેખ શૂન્ય scx_reenq_reject(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_rescue_charge(સંરચના rq *rq, s64 delta_exec) {}
સ્થિર અંતઃરેખ શૂન્ય scx_rescue_end(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ bool scx_rescue_keep(સંરચના rq *rq, સંરચના task_struct *p) { પાછા false; }
સ્થિર અંતઃરેખ શૂન્ય scx_rescue_flush(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_rescue_dump(સંરચના seq_buf *s, સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_rescue_set_knobs(સંરચના scx_sched *sch) {}
સ્થિર અંતઃરેખ શૂન્ય scx_rescue_init(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_dec_has_subs(સંરચના scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, મૂળ)					\
	માટે ((pos) = scx_next_descendant_pre(ખાલી, (મૂળ)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (મૂળ)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
સ્થિર અંતઃરેખ u64 scx_missing_caps(સંરચના scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	જો (!scx_has_subs())
		પાછા 0;

	/* root holds every cap on every cpu */
	જો (!sch->level)
		પાછા 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	પાછા needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
સ્થિર અંતઃરેખ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	જો (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		પાછા 0;
	જો (enq_flags & SCX_ENQ_IMMED)
		પાછા SCX_CAP_ENQ_IMMED;
	પાછા SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
સ્થિર અંતઃરેખ u64 scx_caps_for_task(સંરચના task_struct *p)
{
	જો (p->scx.flags & SCX_TASK_IMMED)
		પાછા SCX_CAP_ENQ_IMMED;
	પાછા SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
સ્થિર અંતઃરેખ u64 scx_caps_for_preempt(સંરચના scx_sched *sch, સંરચના rq *rq, u64 enq_flags)
{
	સંરચના task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	જો (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		પાછા 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	જો (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		પાછા 0;
	પાછા SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
સ્થિર અંતઃરેખ u64 scx_caps_implied(u64 cap)
{
	પસંદગી (cap) {
	સ્થિતિ SCX_CAP_PREEMPT:
		પાછા SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	સ્થિતિ SCX_CAP_ENQ:
		પાછા SCX_CAP_ENQ_IMMED;
	}
	પાછા 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
સ્થિર અંતઃરેખ bool scx_task_can_stay_on_cpu(સંરચના rq *rq, સંરચના task_struct *p)
{
	જો (!scx_has_subs())
		પાછા true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	જો (unlikely(is_migration_disabled(p)))
		પાછા true;

	પાછા likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
સ્થિર અંતઃરેખ સંરચના task_struct *scx_rescuee(સંરચના rq *rq)
{
	lockdep_assert_rq_held(rq);

	જો (!scx_has_subs())
		પાછા ખાલી;

	પાછા rq->scx.rescue.curr;
}

#નહીંતર	/* CONFIG_EXT_SUB_SCHED */

સ્થિર અંતઃરેખ u64 scx_missing_caps(સંરચના scx_sched *sch, s32 cpu, u64 needed) { પાછા 0; }
સ્થિર અંતઃરેખ u64 scx_caps_for_preempt(સંરચના scx_sched *sch, સંરચના rq *rq, u64 enq_flags) { પાછા 0; }
સ્થિર અંતઃરેખ bool scx_task_can_stay_on_cpu(સંરચના rq *rq, સંરચના task_struct *p) { પાછા true; }
સ્થિર અંતઃરેખ સંરચના task_struct *scx_rescuee(સંરચના rq *rq) { પાછા ખાલી; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
