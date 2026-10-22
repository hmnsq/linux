/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Sub-scheduler hierarchy support.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_EXT_SUB_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_EXT_SUB_H

#ਅਨ੍ਤਰ੍ਭਾਵ "internal.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED

ਬਣਤਰ scx_sched *scx_skip_subtree_pre(ਬਣਤਰ scx_sched *pos, ਬਣਤਰ scx_sched *ਜੜ੍ਹ);
ਬਣਤਰ scx_sched *scx_ਅਗਲਾ_descendant_pre(ਬਣਤਰ scx_sched *pos, ਬਣਤਰ scx_sched *ਜੜ੍ਹ);
ਖਾਲੀ scx_ਸੈੱਟ_task_sched(ਬਣਤਰ task_struct *p, ਬਣਤਰ scx_sched *sch);
ਬਣਤਰ cgroup *sch_cgroup(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ ਸੈੱਟ_cgroup_sched(ਬਣਤਰ cgroup *cgrp, ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_pstack_recursion_on_dispatch(ਬਣਤਰ bpf_prog *prog);
ਖਾਲੀ scx_pstack_recursion_on_caps_updated(ਬਣਤਰ bpf_prog *prog);
ਖਾਲੀ drain_descendants(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_sub_disable(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_sub_enable_workfn(ਬਣਤਰ kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux);
ਖਾਲੀ scx_free_pshards(ਬਣਤਰ scx_sched *sch);
s32 scx_alloc_pshards(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_init_ਜੜ੍ਹ_caps(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_process_sync_ecaps(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev);
ਖਾਲੀ scx_unbypass_replay_ecaps(ਬਣਤਰ rq *rq, ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_online_ecaps(ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_offline_ecaps(ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_discard_ecaps_ਮੰਜ਼ਿਲ_sync(s32 cpu, ਬਣਤਰ scx_sched_pcpu *pcpu);
ਖਾਲੀ scx_discard_stale_ecaps_syncs(ਖਾਲੀ);
ਬਣਤਰ scx_dispatch_q *scx_resolve_local_dsq(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq,
					     ਬਣਤਰ task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p);
ਖਾਲੀ scx_reenq_reject(ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_rescue_charge(ਬਣਤਰ rq *rq, s64 delta_exec);
ਖਾਲੀ scx_rescue_ਸਿਰਾ(ਬਣਤਰ rq *rq);
bool scx_rescue_keep(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p);
ਖਾਲੀ scx_rescue_flush(ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_rescue_dump(ਬਣਤਰ seq_buf *s, ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_rescue_ਸੈੱਟ_knobs(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_rescue_init(ਬਣਤਰ rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ scx_sched *scx_cgroup_sched(ਬਣਤਰ cgroup *cgrp)
{
	ਵਾਪਸ rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *sch_cgrp_ਰਸਤਾ(ਬਣਤਰ scx_sched *sch)
{
	ਵਾਪਸ sch->cgrp_ਰਸਤਾ;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_dec_has_subs(ਬਣਤਰ scx_sched *sch)
{
	ਜੇ (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#ਨਹੀਂਤਾਂ	/* CONFIG_EXT_SUB_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ scx_sched *scx_ਅਗਲਾ_descendant_pre(ਬਣਤਰ scx_sched *pos, ਬਣਤਰ scx_sched *ਜੜ੍ਹ) { ਵਾਪਸ pos ? ਨਿਰਾ : ਜੜ੍ਹ; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ scx_sched *scx_skip_subtree_pre(ਬਣਤਰ scx_sched *pos, ਬਣਤਰ scx_sched *ਜੜ੍ਹ) { ਵਾਪਸ ਨਿਰਾ; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_ਸੈੱਟ_task_sched(ਬਣਤਰ task_struct *p, ਬਣਤਰ scx_sched *sch) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cgroup *sch_cgroup(ਬਣਤਰ scx_sched *sch) { ਵਾਪਸ ਨਿਰਾ; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ *sch_cgrp_ਰਸਤਾ(ਬਣਤਰ scx_sched *sch) { ਵਾਪਸ "/"; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_cgroup_sched(ਬਣਤਰ cgroup *cgrp, ਬਣਤਰ scx_sched *sch) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ drain_descendants(ਬਣਤਰ scx_sched *sch) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_sub_disable(ਬਣਤਰ scx_sched *sch) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_free_pshards(ਬਣਤਰ scx_sched *sch) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ s32 scx_alloc_pshards(ਬਣਤਰ scx_sched *sch) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_init_ਜੜ੍ਹ_caps(ਬਣਤਰ scx_sched *sch) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_process_sync_ecaps(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_unbypass_replay_ecaps(ਬਣਤਰ rq *rq, ਬਣਤਰ scx_sched *sch) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_online_ecaps(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_offline_ecaps(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_discard_ecaps_ਮੰਜ਼ਿਲ_sync(s32 cpu, ਬਣਤਰ scx_sched_pcpu *pcpu) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_discard_stale_ecaps_syncs(ਖਾਲੀ) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ scx_dispatch_q *scx_resolve_local_dsq(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 *enq_flags) { ਵਾਪਸ &rq->scx.local_dsq; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_task_reenq_on_cap_revoke(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) { ਵਾਪਸ false; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_reenq_reject(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rescue_charge(ਬਣਤਰ rq *rq, s64 delta_exec) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rescue_ਸਿਰਾ(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_rescue_keep(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) { ਵਾਪਸ false; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rescue_flush(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rescue_dump(ਬਣਤਰ seq_buf *s, ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rescue_ਸੈੱਟ_knobs(ਬਣਤਰ scx_sched *sch) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rescue_init(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_dec_has_subs(ਬਣਤਰ scx_sched *sch) {}

#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

/**
 * scx_for_each_descendant_pre - pre-order walk of a sched's descendants
 * @pos: iteration cursor
 * @root: sched to walk the descendants of
 *
 * Walk @root's descendants. @root is included in the iteration and the first
 * node to be visited. Must be called with scx_enable_mutex, scx_sched_lock, or
 * RCU read lock.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_for_each_descendant_pre(pos, ਜੜ੍ਹ)					\
	ਲਈ ((pos) = scx_ਅਗਲਾ_descendant_pre(ਨਿਰਾ, (ਜੜ੍ਹ)); (pos);		\
	     (pos) = scx_ਅਗਲਾ_descendant_pre((pos), (ਜੜ੍ਹ)))

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 scx_missing_caps(ਬਣਤਰ scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	ਜੇ (!scx_has_subs())
		ਵਾਪਸ 0;

	/* root holds every cap on every cpu */
	ਜੇ (!sch->level)
		ਵਾਪਸ 0;

	ecaps = READ_ONCE(ਪ੍ਰਤੀ_cpu_ptr(sch->pcpu, cpu)->ecaps);

	ਵਾਪਸ needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	ਜੇ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ਵਾਪਸ 0;
	ਜੇ (enq_flags & SCX_ENQ_IMMED)
		ਵਾਪਸ SCX_CAP_ENQ_IMMED;
	ਵਾਪਸ SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 scx_caps_for_task(ਬਣਤਰ task_struct *p)
{
	ਜੇ (p->scx.flags & SCX_TASK_IMMED)
		ਵਾਪਸ SCX_CAP_ENQ_IMMED;
	ਵਾਪਸ SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 scx_caps_for_preempt(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq, u64 enq_flags)
{
	ਬਣਤਰ task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	ਜੇ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ਵਾਪਸ 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	ਜੇ (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		ਵਾਪਸ 0;
	ਵਾਪਸ SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 scx_caps_implied(u64 cap)
{
	ਚੋਣ (cap) {
	ਹਾਲਤ SCX_CAP_PREEMPT:
		ਵਾਪਸ SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	ਹਾਲਤ SCX_CAP_ENQ:
		ਵਾਪਸ SCX_CAP_ENQ_IMMED;
	}
	ਵਾਪਸ 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_task_can_stay_on_cpu(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (!scx_has_subs())
		ਵਾਪਸ true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	ਜੇ (unlikely(is_migration_disabled(p)))
		ਵਾਪਸ true;

	ਵਾਪਸ likely(!scx_missing_caps(scx_task_sched(p), cpu_ਦਾ(rq), SCX_CAP_ਅਧਾਰ));
}

/* the task admitted for rescue on @rq, NULL if none */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *scx_rescuee(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);

	ਜੇ (!scx_has_subs())
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ rq->scx.rescue.curr;
}

#ਨਹੀਂਤਾਂ	/* CONFIG_EXT_SUB_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 scx_missing_caps(ਬਣਤਰ scx_sched *sch, s32 cpu, u64 needed) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 scx_caps_for_preempt(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq, u64 enq_flags) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_task_can_stay_on_cpu(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) { ਵਾਪਸ true; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *scx_rescuee(ਬਣਤਰ rq *rq) { ਵਾਪਸ ਨਿਰਾ; }

#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_EXT_SUB_H */
