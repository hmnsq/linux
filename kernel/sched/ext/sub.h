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

#അന്തര്ഭാവ "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

ഘടന scx_sched *scx_skip_subtree_pre(ഘടന scx_sched *pos, ഘടന scx_sched *വേര്);
ഘടന scx_sched *scx_next_descendant_pre(ഘടന scx_sched *pos, ഘടന scx_sched *വേര്);
ശൂന്യം scx_set_task_sched(ഘടന task_struct *p, ഘടന scx_sched *sch);
ഘടന cgroup *sch_cgroup(ഘടന scx_sched *sch);
ശൂന്യം set_cgroup_sched(ഘടന cgroup *cgrp, ഘടന scx_sched *sch);
ശൂന്യം scx_pstack_recursion_on_dispatch(ഘടന bpf_prog *prog);
ശൂന്യം scx_pstack_recursion_on_caps_updated(ഘടന bpf_prog *prog);
ശൂന്യം drain_descendants(ഘടന scx_sched *sch);
ശൂന്യം scx_sub_disable(ഘടന scx_sched *sch);
ശൂന്യം scx_sub_enable_workfn(ഘടന kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, സ്ഥിരം ഘടന bpf_prog_aux *aux);
ശൂന്യം scx_free_pshards(ഘടന scx_sched *sch);
s32 scx_alloc_pshards(ഘടന scx_sched *sch);
ശൂന്യം scx_init_വേര്_caps(ഘടന scx_sched *sch);
ശൂന്യം scx_process_sync_ecaps(ഘടന rq *rq, ഘടന task_struct *prev);
ശൂന്യം scx_unbypass_replay_ecaps(ഘടന rq *rq, ഘടന scx_sched *sch);
ശൂന്യം scx_online_ecaps(ഘടന rq *rq);
ശൂന്യം scx_offline_ecaps(ഘടന rq *rq);
ശൂന്യം scx_discard_ecaps_to_sync(s32 cpu, ഘടന scx_sched_pcpu *pcpu);
ശൂന്യം scx_discard_stale_ecaps_syncs(ശൂന്യം);
ഘടന scx_dispatch_q *scx_resolve_local_dsq(ഘടന scx_sched *sch, ഘടന rq *rq,
					     ഘടന task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(ഘടന rq *rq, ഘടന task_struct *p);
ശൂന്യം scx_reenq_reject(ഘടന rq *rq);
ശൂന്യം scx_rescue_charge(ഘടന rq *rq, s64 delta_exec);
ശൂന്യം scx_rescue_end(ഘടന rq *rq);
bool scx_rescue_keep(ഘടന rq *rq, ഘടന task_struct *p);
ശൂന്യം scx_rescue_flush(ഘടന rq *rq);
ശൂന്യം scx_rescue_dump(ഘടന seq_buf *s, ഘടന rq *rq);
ശൂന്യം scx_rescue_set_knobs(ഘടന scx_sched *sch);
ശൂന്യം scx_rescue_init(ഘടന rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
സ്ഥിതം വരിയിൽ ഘടന scx_sched *scx_cgroup_sched(ഘടന cgroup *cgrp)
{
	തിരികെ rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *sch_cgrp_path(ഘടന scx_sched *sch)
{
	തിരികെ sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
സ്ഥിതം വരിയിൽ ശൂന്യം scx_dec_has_subs(ഘടന scx_sched *sch)
{
	എങ്കിൽ (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#അല്ലെങ്കിൽ	/* CONFIG_EXT_SUB_SCHED */

സ്ഥിതം വരിയിൽ ഘടന scx_sched *scx_next_descendant_pre(ഘടന scx_sched *pos, ഘടന scx_sched *വേര്) { തിരികെ pos ? ശൂന്യ : വേര്; }
സ്ഥിതം വരിയിൽ ഘടന scx_sched *scx_skip_subtree_pre(ഘടന scx_sched *pos, ഘടന scx_sched *വേര്) { തിരികെ ശൂന്യ; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_set_task_sched(ഘടന task_struct *p, ഘടന scx_sched *sch) {}
സ്ഥിതം വരിയിൽ ഘടന cgroup *sch_cgroup(ഘടന scx_sched *sch) { തിരികെ ശൂന്യ; }
സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം *sch_cgrp_path(ഘടന scx_sched *sch) { തിരികെ "/"; }
സ്ഥിതം വരിയിൽ ശൂന്യം set_cgroup_sched(ഘടന cgroup *cgrp, ഘടന scx_sched *sch) {}
സ്ഥിതം വരിയിൽ ശൂന്യം drain_descendants(ഘടന scx_sched *sch) { }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_sub_disable(ഘടന scx_sched *sch) { }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_free_pshards(ഘടന scx_sched *sch) {}
സ്ഥിതം വരിയിൽ s32 scx_alloc_pshards(ഘടന scx_sched *sch) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_init_വേര്_caps(ഘടന scx_sched *sch) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_process_sync_ecaps(ഘടന rq *rq, ഘടന task_struct *prev) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_unbypass_replay_ecaps(ഘടന rq *rq, ഘടന scx_sched *sch) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_online_ecaps(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_offline_ecaps(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_discard_ecaps_to_sync(s32 cpu, ഘടന scx_sched_pcpu *pcpu) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_discard_stale_ecaps_syncs(ശൂന്യം) {}
സ്ഥിതം വരിയിൽ ഘടന scx_dispatch_q *scx_resolve_local_dsq(ഘടന scx_sched *sch, ഘടന rq *rq, ഘടന task_struct *p, u64 *enq_flags) { തിരികെ &rq->scx.local_dsq; }
സ്ഥിതം വരിയിൽ bool scx_task_reenq_on_cap_revoke(ഘടന rq *rq, ഘടന task_struct *p) { തിരികെ false; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_reenq_reject(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rescue_charge(ഘടന rq *rq, s64 delta_exec) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rescue_end(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ bool scx_rescue_keep(ഘടന rq *rq, ഘടന task_struct *p) { തിരികെ false; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rescue_flush(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rescue_dump(ഘടന seq_buf *s, ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rescue_set_knobs(ഘടന scx_sched *sch) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rescue_init(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_dec_has_subs(ഘടന scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, വേര്)					\
	വേണ്ടി ((pos) = scx_next_descendant_pre(ശൂന്യ, (വേര്)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (വേര്)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
സ്ഥിതം വരിയിൽ u64 scx_missing_caps(ഘടന scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	എങ്കിൽ (!scx_has_subs())
		തിരികെ 0;

	/* root holds every cap on every cpu */
	എങ്കിൽ (!sch->level)
		തിരികെ 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	തിരികെ needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
സ്ഥിതം വരിയിൽ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	എങ്കിൽ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		തിരികെ 0;
	എങ്കിൽ (enq_flags & SCX_ENQ_IMMED)
		തിരികെ SCX_CAP_ENQ_IMMED;
	തിരികെ SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
സ്ഥിതം വരിയിൽ u64 scx_caps_for_task(ഘടന task_struct *p)
{
	എങ്കിൽ (p->scx.flags & SCX_TASK_IMMED)
		തിരികെ SCX_CAP_ENQ_IMMED;
	തിരികെ SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
സ്ഥിതം വരിയിൽ u64 scx_caps_for_preempt(ഘടന scx_sched *sch, ഘടന rq *rq, u64 enq_flags)
{
	ഘടന task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	എങ്കിൽ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		തിരികെ 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	എങ്കിൽ (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		തിരികെ 0;
	തിരികെ SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
സ്ഥിതം വരിയിൽ u64 scx_caps_implied(u64 cap)
{
	തിരഞ്ഞെടുപ്പ് (cap) {
	അവസ്ഥ SCX_CAP_PREEMPT:
		തിരികെ SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	അവസ്ഥ SCX_CAP_ENQ:
		തിരികെ SCX_CAP_ENQ_IMMED;
	}
	തിരികെ 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
സ്ഥിതം വരിയിൽ bool scx_task_can_stay_on_cpu(ഘടന rq *rq, ഘടന task_struct *p)
{
	എങ്കിൽ (!scx_has_subs())
		തിരികെ true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	എങ്കിൽ (unlikely(is_migration_disabled(p)))
		തിരികെ true;

	തിരികെ likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
സ്ഥിതം വരിയിൽ ഘടന task_struct *scx_rescuee(ഘടന rq *rq)
{
	lockdep_assert_rq_held(rq);

	എങ്കിൽ (!scx_has_subs())
		തിരികെ ശൂന്യ;

	തിരികെ rq->scx.rescue.curr;
}

#അല്ലെങ്കിൽ	/* CONFIG_EXT_SUB_SCHED */

സ്ഥിതം വരിയിൽ u64 scx_missing_caps(ഘടന scx_sched *sch, s32 cpu, u64 needed) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ u64 scx_caps_for_preempt(ഘടന scx_sched *sch, ഘടന rq *rq, u64 enq_flags) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ bool scx_task_can_stay_on_cpu(ഘടന rq *rq, ഘടന task_struct *p) { തിരികെ true; }
സ്ഥിതം വരിയിൽ ഘടന task_struct *scx_rescuee(ഘടന rq *rq) { തിരികെ ശൂന്യ; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
