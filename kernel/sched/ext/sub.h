/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Sub-scheduler hierarchy support.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#నిర్వచితం_కాకపోతే _KERNEL_SCHED_EXT_SUB_H
#నిర్వచించు _KERNEL_SCHED_EXT_SUB_H

#అన్తర్భావ "internal.h"

#నిర్వచితమైతే CONFIG_EXT_SUB_SCHED

నిర్మాణం scx_sched *scx_skip_subtree_pre(నిర్మాణం scx_sched *pos, నిర్మాణం scx_sched *మూలం);
నిర్మాణం scx_sched *scx_తదుపరి_descendant_pre(నిర్మాణం scx_sched *pos, నిర్మాణం scx_sched *మూలం);
శూన్య scx_సమితి_task_sched(నిర్మాణం task_struct *p, నిర్మాణం scx_sched *sch);
నిర్మాణం cgroup *sch_cgroup(నిర్మాణం scx_sched *sch);
శూన్య సమితి_cgroup_sched(నిర్మాణం cgroup *cgrp, నిర్మాణం scx_sched *sch);
శూన్య scx_pstack_recursion_on_dispatch(నిర్మాణం bpf_prog *prog);
శూన్య scx_pstack_recursion_on_caps_updated(నిర్మాణం bpf_prog *prog);
శూన్య drain_descendants(నిర్మాణం scx_sched *sch);
శూన్య scx_sub_disable(నిర్మాణం scx_sched *sch);
శూన్య scx_sub_enable_workfn(నిర్మాణం kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux);
శూన్య scx_free_pshards(నిర్మాణం scx_sched *sch);
s32 scx_alloc_pshards(నిర్మాణం scx_sched *sch);
శూన్య scx_init_మూలం_caps(నిర్మాణం scx_sched *sch);
శూన్య scx_process_sync_ecaps(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev);
శూన్య scx_unbypass_replay_ecaps(నిర్మాణం rq *rq, నిర్మాణం scx_sched *sch);
శూన్య scx_online_ecaps(నిర్మాణం rq *rq);
శూన్య scx_offline_ecaps(నిర్మాణం rq *rq);
శూన్య scx_discard_ecaps_గమ్యం_sync(s32 cpu, నిర్మాణం scx_sched_pcpu *pcpu);
శూన్య scx_discard_stale_ecaps_syncs(శూన్య);
నిర్మాణం scx_dispatch_q *scx_resolve_local_dsq(నిర్మాణం scx_sched *sch, నిర్మాణం rq *rq,
					     నిర్మాణం task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(నిర్మాణం rq *rq, నిర్మాణం task_struct *p);
శూన్య scx_reenq_reject(నిర్మాణం rq *rq);
శూన్య scx_rescue_charge(నిర్మాణం rq *rq, s64 delta_exec);
శూన్య scx_rescue_చివర(నిర్మాణం rq *rq);
bool scx_rescue_keep(నిర్మాణం rq *rq, నిర్మాణం task_struct *p);
శూన్య scx_rescue_flush(నిర్మాణం rq *rq);
శూన్య scx_rescue_dump(నిర్మాణం seq_buf *s, నిర్మాణం rq *rq);
శూన్య scx_rescue_సమితి_knobs(నిర్మాణం scx_sched *sch);
శూన్య scx_rescue_init(నిర్మాణం rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
స్థిర అంతఃవరుస నిర్మాణం scx_sched *scx_cgroup_sched(నిర్మాణం cgroup *cgrp)
{
	తిరిగి rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *sch_cgrp_మార్గం(నిర్మాణం scx_sched *sch)
{
	తిరిగి sch->cgrp_మార్గం;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
స్థిర అంతఃవరుస శూన్య scx_dec_has_subs(నిర్మాణం scx_sched *sch)
{
	ఒకవేళ (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#లేకపోతే	/* CONFIG_EXT_SUB_SCHED */

స్థిర అంతఃవరుస నిర్మాణం scx_sched *scx_తదుపరి_descendant_pre(నిర్మాణం scx_sched *pos, నిర్మాణం scx_sched *మూలం) { తిరిగి pos ? ఖాళీ : మూలం; }
స్థిర అంతఃవరుస నిర్మాణం scx_sched *scx_skip_subtree_pre(నిర్మాణం scx_sched *pos, నిర్మాణం scx_sched *మూలం) { తిరిగి ఖాళీ; }
స్థిర అంతఃవరుస శూన్య scx_సమితి_task_sched(నిర్మాణం task_struct *p, నిర్మాణం scx_sched *sch) {}
స్థిర అంతఃవరుస నిర్మాణం cgroup *sch_cgroup(నిర్మాణం scx_sched *sch) { తిరిగి ఖాళీ; }
స్థిర అంతఃవరుస స్థిరరాశి అక్షరం *sch_cgrp_మార్గం(నిర్మాణం scx_sched *sch) { తిరిగి "/"; }
స్థిర అంతఃవరుస శూన్య సమితి_cgroup_sched(నిర్మాణం cgroup *cgrp, నిర్మాణం scx_sched *sch) {}
స్థిర అంతఃవరుస శూన్య drain_descendants(నిర్మాణం scx_sched *sch) { }
స్థిర అంతఃవరుస శూన్య scx_sub_disable(నిర్మాణం scx_sched *sch) { }
స్థిర అంతఃవరుస శూన్య scx_free_pshards(నిర్మాణం scx_sched *sch) {}
స్థిర అంతఃవరుస s32 scx_alloc_pshards(నిర్మాణం scx_sched *sch) { తిరిగి 0; }
స్థిర అంతఃవరుస శూన్య scx_init_మూలం_caps(నిర్మాణం scx_sched *sch) {}
స్థిర అంతఃవరుస శూన్య scx_process_sync_ecaps(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev) {}
స్థిర అంతఃవరుస శూన్య scx_unbypass_replay_ecaps(నిర్మాణం rq *rq, నిర్మాణం scx_sched *sch) {}
స్థిర అంతఃవరుస శూన్య scx_online_ecaps(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_offline_ecaps(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_discard_ecaps_గమ్యం_sync(s32 cpu, నిర్మాణం scx_sched_pcpu *pcpu) {}
స్థిర అంతఃవరుస శూన్య scx_discard_stale_ecaps_syncs(శూన్య) {}
స్థిర అంతఃవరుస నిర్మాణం scx_dispatch_q *scx_resolve_local_dsq(నిర్మాణం scx_sched *sch, నిర్మాణం rq *rq, నిర్మాణం task_struct *p, u64 *enq_flags) { తిరిగి &rq->scx.local_dsq; }
స్థిర అంతఃవరుస bool scx_task_reenq_on_cap_revoke(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) { తిరిగి false; }
స్థిర అంతఃవరుస శూన్య scx_reenq_reject(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_rescue_charge(నిర్మాణం rq *rq, s64 delta_exec) {}
స్థిర అంతఃవరుస శూన్య scx_rescue_చివర(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస bool scx_rescue_keep(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) { తిరిగి false; }
స్థిర అంతఃవరుస శూన్య scx_rescue_flush(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_rescue_dump(నిర్మాణం seq_buf *s, నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_rescue_సమితి_knobs(నిర్మాణం scx_sched *sch) {}
స్థిర అంతఃవరుస శూన్య scx_rescue_init(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_dec_has_subs(నిర్మాణం scx_sched *sch) {}

#అయితే_ముగింపు	/* CONFIG_EXT_SUB_SCHED */

/**
 * scx_for_each_descendant_pre - pre-order walk of a sched's descendants
 * @pos: iteration cursor
 * @root: sched to walk the descendants of
 *
 * Walk @root's descendants. @root is included in the iteration and the first
 * node to be visited. Must be called with scx_enable_mutex, scx_sched_lock, or
 * RCU read lock.
 */
#నిర్వచించు scx_for_each_descendant_pre(pos, మూలం)					\
	కొరకు ((pos) = scx_తదుపరి_descendant_pre(ఖాళీ, (మూలం)); (pos);		\
	     (pos) = scx_తదుపరి_descendant_pre((pos), (మూలం)))

#నిర్వచితమైతే CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
స్థిర అంతఃవరుస u64 scx_missing_caps(నిర్మాణం scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	ఒకవేళ (!scx_has_subs())
		తిరిగి 0;

	/* root holds every cap on every cpu */
	ఒకవేళ (!sch->level)
		తిరిగి 0;

	ecaps = READ_ONCE(చొప్పున_cpu_ptr(sch->pcpu, cpu)->ecaps);

	తిరిగి needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
స్థిర అంతఃవరుస u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	ఒకవేళ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		తిరిగి 0;
	ఒకవేళ (enq_flags & SCX_ENQ_IMMED)
		తిరిగి SCX_CAP_ENQ_IMMED;
	తిరిగి SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
స్థిర అంతఃవరుస u64 scx_caps_for_task(నిర్మాణం task_struct *p)
{
	ఒకవేళ (p->scx.flags & SCX_TASK_IMMED)
		తిరిగి SCX_CAP_ENQ_IMMED;
	తిరిగి SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
స్థిర అంతఃవరుస u64 scx_caps_for_preempt(నిర్మాణం scx_sched *sch, నిర్మాణం rq *rq, u64 enq_flags)
{
	నిర్మాణం task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	ఒకవేళ (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		తిరిగి 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	ఒకవేళ (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		తిరిగి 0;
	తిరిగి SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
స్థిర అంతఃవరుస u64 scx_caps_implied(u64 cap)
{
	ఎంపిక (cap) {
	స్థితి SCX_CAP_PREEMPT:
		తిరిగి SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	స్థితి SCX_CAP_ENQ:
		తిరిగి SCX_CAP_ENQ_IMMED;
	}
	తిరిగి 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
స్థిర అంతఃవరుస bool scx_task_can_stay_on_cpu(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (!scx_has_subs())
		తిరిగి true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	ఒకవేళ (unlikely(is_migration_disabled(p)))
		తిరిగి true;

	తిరిగి likely(!scx_missing_caps(scx_task_sched(p), cpu_యొక్క(rq), SCX_CAP_పునాది));
}

/* the task admitted for rescue on @rq, NULL if none */
స్థిర అంతఃవరుస నిర్మాణం task_struct *scx_rescuee(నిర్మాణం rq *rq)
{
	lockdep_assert_rq_held(rq);

	ఒకవేళ (!scx_has_subs())
		తిరిగి ఖాళీ;

	తిరిగి rq->scx.rescue.curr;
}

#లేకపోతే	/* CONFIG_EXT_SUB_SCHED */

స్థిర అంతఃవరుస u64 scx_missing_caps(నిర్మాణం scx_sched *sch, s32 cpu, u64 needed) { తిరిగి 0; }
స్థిర అంతఃవరుస u64 scx_caps_for_preempt(నిర్మాణం scx_sched *sch, నిర్మాణం rq *rq, u64 enq_flags) { తిరిగి 0; }
స్థిర అంతఃవరుస bool scx_task_can_stay_on_cpu(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) { తిరిగి true; }
స్థిర అంతఃవరుస నిర్మాణం task_struct *scx_rescuee(నిర్మాణం rq *rq) { తిరిగి ఖాళీ; }

#అయితే_ముగింపు	/* CONFIG_EXT_SUB_SCHED */

#అయితే_ముగింపు /* _KERNEL_SCHED_EXT_SUB_H */
