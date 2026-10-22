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

#ଅନ୍ତର୍ଭାଵ "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

ସଂରଚନା scx_sched *scx_skip_subtree_pre(ସଂରଚନା scx_sched *pos, ସଂରଚନା scx_sched *ମୂଳ);
ସଂରଚନା scx_sched *scx_next_descendant_pre(ସଂରଚନା scx_sched *pos, ସଂରଚନା scx_sched *ମୂଳ);
ଶୂନ୍ୟ scx_set_task_sched(ସଂରଚନା task_struct *p, ସଂରଚନା scx_sched *sch);
ସଂରଚନା cgroup *sch_cgroup(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ set_cgroup_sched(ସଂରଚନା cgroup *cgrp, ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_pstack_recursion_on_dispatch(ସଂରଚନା bpf_prog *prog);
ଶୂନ୍ୟ scx_pstack_recursion_on_caps_updated(ସଂରଚନା bpf_prog *prog);
ଶୂନ୍ୟ drain_descendants(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_sub_disable(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_sub_enable_workfn(ସଂରଚନା kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ଅଚଳ ସଂରଚନା bpf_prog_aux *aux);
ଶୂନ୍ୟ scx_free_pshards(ସଂରଚନା scx_sched *sch);
s32 scx_alloc_pshards(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_init_ମୂଳ_caps(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_process_sync_ecaps(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev);
ଶୂନ୍ୟ scx_unbypass_replay_ecaps(ସଂରଚନା rq *rq, ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_online_ecaps(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_offline_ecaps(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_discard_ecaps_to_sync(s32 cpu, ସଂରଚନା scx_sched_pcpu *pcpu);
ଶୂନ୍ୟ scx_discard_stale_ecaps_syncs(ଶୂନ୍ୟ);
ସଂରଚନା scx_dispatch_q *scx_resolve_local_dsq(ସଂରଚନା scx_sched *sch, ସଂରଚନା rq *rq,
					     ସଂରଚନା task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p);
ଶୂନ୍ୟ scx_reenq_reject(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_rescue_charge(ସଂରଚନା rq *rq, s64 delta_exec);
ଶୂନ୍ୟ scx_rescue_end(ସଂରଚନା rq *rq);
bool scx_rescue_keep(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p);
ଶୂନ୍ୟ scx_rescue_flush(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_rescue_dump(ସଂରଚନା seq_buf *s, ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_rescue_set_knobs(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_rescue_init(ସଂରଚନା rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା scx_sched *scx_cgroup_sched(ସଂରଚନା cgroup *cgrp)
{
	ଫେରନ୍ତୁ rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *sch_cgrp_path(ସଂରଚନା scx_sched *sch)
{
	ଫେରନ୍ତୁ sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_dec_has_subs(ସଂରଚନା scx_sched *sch)
{
	ଯଦି (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#ଅନ୍ୟଥା	/* CONFIG_EXT_SUB_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା scx_sched *scx_next_descendant_pre(ସଂରଚନା scx_sched *pos, ସଂରଚନା scx_sched *ମୂଳ) { ଫେରନ୍ତୁ pos ? ଖାଲି : ମୂଳ; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା scx_sched *scx_skip_subtree_pre(ସଂରଚନା scx_sched *pos, ସଂରଚନା scx_sched *ମୂଳ) { ଫେରନ୍ତୁ ଖାଲି; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_set_task_sched(ସଂରଚନା task_struct *p, ସଂରଚନା scx_sched *sch) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cgroup *sch_cgroup(ସଂରଚନା scx_sched *sch) { ଫେରନ୍ତୁ ଖାଲି; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର *sch_cgrp_path(ସଂରଚନା scx_sched *sch) { ଫେରନ୍ତୁ "/"; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ set_cgroup_sched(ସଂରଚନା cgroup *cgrp, ସଂରଚନା scx_sched *sch) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ drain_descendants(ସଂରଚନା scx_sched *sch) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_sub_disable(ସଂରଚନା scx_sched *sch) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_free_pshards(ସଂରଚନା scx_sched *sch) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ s32 scx_alloc_pshards(ସଂରଚନା scx_sched *sch) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_init_ମୂଳ_caps(ସଂରଚନା scx_sched *sch) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_process_sync_ecaps(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_unbypass_replay_ecaps(ସଂରଚନା rq *rq, ସଂରଚନା scx_sched *sch) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_online_ecaps(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_offline_ecaps(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_discard_ecaps_to_sync(s32 cpu, ସଂରଚନା scx_sched_pcpu *pcpu) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_discard_stale_ecaps_syncs(ଶୂନ୍ୟ) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା scx_dispatch_q *scx_resolve_local_dsq(ସଂରଚନା scx_sched *sch, ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, u64 *enq_flags) { ଫେରନ୍ତୁ &rq->scx.local_dsq; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool scx_task_reenq_on_cap_revoke(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p) { ଫେରନ୍ତୁ false; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_reenq_reject(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rescue_charge(ସଂରଚନା rq *rq, s64 delta_exec) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rescue_end(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool scx_rescue_keep(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p) { ଫେରନ୍ତୁ false; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rescue_flush(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rescue_dump(ସଂରଚନା seq_buf *s, ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rescue_set_knobs(ସଂରଚନା scx_sched *sch) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rescue_init(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_dec_has_subs(ସଂରଚନା scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, ମୂଳ)					\
	ପାଇଁ ((pos) = scx_next_descendant_pre(ଖାଲି, (ମୂଳ)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (ମୂଳ)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 scx_missing_caps(ସଂରଚନା scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	ଯଦି (!scx_has_subs())
		ଫେରନ୍ତୁ 0;

	/* root holds every cap on every cpu */
	ଯଦି (!sch->level)
		ଫେରନ୍ତୁ 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	ଫେରନ୍ତୁ needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	ଯଦି (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ଫେରନ୍ତୁ 0;
	ଯଦି (enq_flags & SCX_ENQ_IMMED)
		ଫେରନ୍ତୁ SCX_CAP_ENQ_IMMED;
	ଫେରନ୍ତୁ SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 scx_caps_for_task(ସଂରଚନା task_struct *p)
{
	ଯଦି (p->scx.flags & SCX_TASK_IMMED)
		ଫେରନ୍ତୁ SCX_CAP_ENQ_IMMED;
	ଫେରନ୍ତୁ SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 scx_caps_for_preempt(ସଂରଚନା scx_sched *sch, ସଂରଚନା rq *rq, u64 enq_flags)
{
	ସଂରଚନା task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	ଯଦି (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		ଫେରନ୍ତୁ 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	ଯଦି (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 scx_caps_implied(u64 cap)
{
	ବାଛ (cap) {
	ସ୍ଥିତି SCX_CAP_PREEMPT:
		ଫେରନ୍ତୁ SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	ସ୍ଥିତି SCX_CAP_ENQ:
		ଫେରନ୍ତୁ SCX_CAP_ENQ_IMMED;
	}
	ଫେରନ୍ତୁ 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool scx_task_can_stay_on_cpu(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଯଦି (!scx_has_subs())
		ଫେରନ୍ତୁ true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	ଯଦି (unlikely(is_migration_disabled(p)))
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *scx_rescuee(ସଂରଚନା rq *rq)
{
	lockdep_assert_rq_held(rq);

	ଯଦି (!scx_has_subs())
		ଫେରନ୍ତୁ ଖାଲି;

	ଫେରନ୍ତୁ rq->scx.rescue.curr;
}

#ଅନ୍ୟଥା	/* CONFIG_EXT_SUB_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 scx_missing_caps(ସଂରଚନା scx_sched *sch, s32 cpu, u64 needed) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 scx_caps_for_preempt(ସଂରଚନା scx_sched *sch, ସଂରଚନା rq *rq, u64 enq_flags) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool scx_task_can_stay_on_cpu(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p) { ଫେରନ୍ତୁ true; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *scx_rescuee(ସଂରଚନା rq *rq) { ଫେରନ୍ତୁ ଖାଲି; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
