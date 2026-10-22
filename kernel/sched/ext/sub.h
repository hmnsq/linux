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

#ضمن "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

بنية scx_sched *scx_skip_subtree_pre(بنية scx_sched *pos, بنية scx_sched *ڕەگ);
بنية scx_sched *scx_next_descendant_pre(بنية scx_sched *pos, بنية scx_sched *ڕەگ);
فراغ scx_set_task_sched(بنية task_struct *p, بنية scx_sched *sch);
بنية cgroup *sch_cgroup(بنية scx_sched *sch);
فراغ set_cgroup_sched(بنية cgroup *cgrp, بنية scx_sched *sch);
فراغ scx_pstack_recursion_on_dispatch(بنية bpf_prog *prog);
فراغ scx_pstack_recursion_on_caps_updated(بنية bpf_prog *prog);
فراغ drain_descendants(بنية scx_sched *sch);
فراغ scx_sub_disable(بنية scx_sched *sch);
فراغ scx_sub_enable_workfn(بنية kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ثابت بنية bpf_prog_aux *aux);
فراغ scx_free_pshards(بنية scx_sched *sch);
s32 scx_alloc_pshards(بنية scx_sched *sch);
فراغ scx_init_ڕەگ_caps(بنية scx_sched *sch);
فراغ scx_process_sync_ecaps(بنية rq *rq, بنية task_struct *prev);
فراغ scx_unbypass_replay_ecaps(بنية rq *rq, بنية scx_sched *sch);
فراغ scx_online_ecaps(بنية rq *rq);
فراغ scx_offline_ecaps(بنية rq *rq);
فراغ scx_discard_ecaps_to_sync(s32 cpu, بنية scx_sched_pcpu *pcpu);
فراغ scx_discard_stale_ecaps_syncs(فراغ);
بنية scx_dispatch_q *scx_resolve_local_dsq(بنية scx_sched *sch, بنية rq *rq,
					     بنية task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(بنية rq *rq, بنية task_struct *p);
فراغ scx_reenq_reject(بنية rq *rq);
فراغ scx_rescue_charge(بنية rq *rq, s64 delta_exec);
فراغ scx_rescue_end(بنية rq *rq);
bool scx_rescue_keep(بنية rq *rq, بنية task_struct *p);
فراغ scx_rescue_flush(بنية rq *rq);
فراغ scx_rescue_dump(بنية seq_buf *s, بنية rq *rq);
فراغ scx_rescue_set_knobs(بنية scx_sched *sch);
فراغ scx_rescue_init(بنية rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
ساكن مضمن بنية scx_sched *scx_cgroup_sched(بنية cgroup *cgrp)
{
	أرجع rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

ساكن مضمن ثابت حرف *sch_cgrp_path(بنية scx_sched *sch)
{
	أرجع sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
ساكن مضمن فراغ scx_dec_has_subs(بنية scx_sched *sch)
{
	إذا (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#وإلا	/* CONFIG_EXT_SUB_SCHED */

ساكن مضمن بنية scx_sched *scx_next_descendant_pre(بنية scx_sched *pos, بنية scx_sched *ڕەگ) { أرجع pos ? عدم : ڕەگ; }
ساكن مضمن بنية scx_sched *scx_skip_subtree_pre(بنية scx_sched *pos, بنية scx_sched *ڕەگ) { أرجع عدم; }
ساكن مضمن فراغ scx_set_task_sched(بنية task_struct *p, بنية scx_sched *sch) {}
ساكن مضمن بنية cgroup *sch_cgroup(بنية scx_sched *sch) { أرجع عدم; }
ساكن مضمن ثابت حرف *sch_cgrp_path(بنية scx_sched *sch) { أرجع "/"; }
ساكن مضمن فراغ set_cgroup_sched(بنية cgroup *cgrp, بنية scx_sched *sch) {}
ساكن مضمن فراغ drain_descendants(بنية scx_sched *sch) { }
ساكن مضمن فراغ scx_sub_disable(بنية scx_sched *sch) { }
ساكن مضمن فراغ scx_free_pshards(بنية scx_sched *sch) {}
ساكن مضمن s32 scx_alloc_pshards(بنية scx_sched *sch) { أرجع 0; }
ساكن مضمن فراغ scx_init_ڕەگ_caps(بنية scx_sched *sch) {}
ساكن مضمن فراغ scx_process_sync_ecaps(بنية rq *rq, بنية task_struct *prev) {}
ساكن مضمن فراغ scx_unbypass_replay_ecaps(بنية rq *rq, بنية scx_sched *sch) {}
ساكن مضمن فراغ scx_online_ecaps(بنية rq *rq) {}
ساكن مضمن فراغ scx_offline_ecaps(بنية rq *rq) {}
ساكن مضمن فراغ scx_discard_ecaps_to_sync(s32 cpu, بنية scx_sched_pcpu *pcpu) {}
ساكن مضمن فراغ scx_discard_stale_ecaps_syncs(فراغ) {}
ساكن مضمن بنية scx_dispatch_q *scx_resolve_local_dsq(بنية scx_sched *sch, بنية rq *rq, بنية task_struct *p, u64 *enq_flags) { أرجع &rq->scx.local_dsq; }
ساكن مضمن bool scx_task_reenq_on_cap_revoke(بنية rq *rq, بنية task_struct *p) { أرجع false; }
ساكن مضمن فراغ scx_reenq_reject(بنية rq *rq) {}
ساكن مضمن فراغ scx_rescue_charge(بنية rq *rq, s64 delta_exec) {}
ساكن مضمن فراغ scx_rescue_end(بنية rq *rq) {}
ساكن مضمن bool scx_rescue_keep(بنية rq *rq, بنية task_struct *p) { أرجع false; }
ساكن مضمن فراغ scx_rescue_flush(بنية rq *rq) {}
ساكن مضمن فراغ scx_rescue_dump(بنية seq_buf *s, بنية rq *rq) {}
ساكن مضمن فراغ scx_rescue_set_knobs(بنية scx_sched *sch) {}
ساكن مضمن فراغ scx_rescue_init(بنية rq *rq) {}
ساكن مضمن فراغ scx_dec_has_subs(بنية scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, ڕەگ)					\
	لكل ((pos) = scx_next_descendant_pre(عدم, (ڕەگ)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (ڕەگ)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
ساكن مضمن u64 scx_missing_caps(بنية scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	إذا (!scx_has_subs())
		أرجع 0;

	/* root holds every cap on every cpu */
	إذا (!sch->level)
		أرجع 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	أرجع needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
ساكن مضمن u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	إذا (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		أرجع 0;
	إذا (enq_flags & SCX_ENQ_IMMED)
		أرجع SCX_CAP_ENQ_IMMED;
	أرجع SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
ساكن مضمن u64 scx_caps_for_task(بنية task_struct *p)
{
	إذا (p->scx.flags & SCX_TASK_IMMED)
		أرجع SCX_CAP_ENQ_IMMED;
	أرجع SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
ساكن مضمن u64 scx_caps_for_preempt(بنية scx_sched *sch, بنية rq *rq, u64 enq_flags)
{
	بنية task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	إذا (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		أرجع 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	إذا (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		أرجع 0;
	أرجع SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
ساكن مضمن u64 scx_caps_implied(u64 cap)
{
	بدل (cap) {
	حالة SCX_CAP_PREEMPT:
		أرجع SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	حالة SCX_CAP_ENQ:
		أرجع SCX_CAP_ENQ_IMMED;
	}
	أرجع 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
ساكن مضمن bool scx_task_can_stay_on_cpu(بنية rq *rq, بنية task_struct *p)
{
	إذا (!scx_has_subs())
		أرجع true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	إذا (unlikely(is_migration_disabled(p)))
		أرجع true;

	أرجع likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
ساكن مضمن بنية task_struct *scx_rescuee(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);

	إذا (!scx_has_subs())
		أرجع عدم;

	أرجع rq->scx.rescue.curr;
}

#وإلا	/* CONFIG_EXT_SUB_SCHED */

ساكن مضمن u64 scx_missing_caps(بنية scx_sched *sch, s32 cpu, u64 needed) { أرجع 0; }
ساكن مضمن u64 scx_caps_for_preempt(بنية scx_sched *sch, بنية rq *rq, u64 enq_flags) { أرجع 0; }
ساكن مضمن bool scx_task_can_stay_on_cpu(بنية rq *rq, بنية task_struct *p) { أرجع true; }
ساكن مضمن بنية task_struct *scx_rescuee(بنية rq *rq) { أرجع عدم; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
