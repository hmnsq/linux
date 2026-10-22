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

#شامل "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

ساختار scx_sched *scx_skip_subtree_pre(ساختار scx_sched *pos, ساختار scx_sched *ریشه);
ساختار scx_sched *scx_next_descendant_pre(ساختار scx_sched *pos, ساختار scx_sched *ریشه);
تهی scx_set_task_sched(ساختار task_struct *p, ساختار scx_sched *sch);
ساختار cgroup *sch_cgroup(ساختار scx_sched *sch);
تهی set_cgroup_sched(ساختار cgroup *cgrp, ساختار scx_sched *sch);
تهی scx_pstack_recursion_on_dispatch(ساختار bpf_prog *prog);
تهی scx_pstack_recursion_on_caps_updated(ساختار bpf_prog *prog);
تهی drain_descendants(ساختار scx_sched *sch);
تهی scx_sub_disable(ساختار scx_sched *sch);
تهی scx_sub_enable_workfn(ساختار kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ثابت ساختار bpf_prog_aux *aux);
تهی scx_free_pshards(ساختار scx_sched *sch);
s32 scx_alloc_pshards(ساختار scx_sched *sch);
تهی scx_init_ریشه_caps(ساختار scx_sched *sch);
تهی scx_process_sync_ecaps(ساختار rq *rq, ساختار task_struct *prev);
تهی scx_unbypass_replay_ecaps(ساختار rq *rq, ساختار scx_sched *sch);
تهی scx_online_ecaps(ساختار rq *rq);
تهی scx_offline_ecaps(ساختار rq *rq);
تهی scx_discard_ecaps_to_sync(s32 cpu, ساختار scx_sched_pcpu *pcpu);
تهی scx_discard_stale_ecaps_syncs(تهی);
ساختار scx_dispatch_q *scx_resolve_local_dsq(ساختار scx_sched *sch, ساختار rq *rq,
					     ساختار task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(ساختار rq *rq, ساختار task_struct *p);
تهی scx_reenq_reject(ساختار rq *rq);
تهی scx_rescue_charge(ساختار rq *rq, s64 delta_exec);
تهی scx_rescue_end(ساختار rq *rq);
bool scx_rescue_keep(ساختار rq *rq, ساختار task_struct *p);
تهی scx_rescue_flush(ساختار rq *rq);
تهی scx_rescue_dump(ساختار seq_buf *s, ساختار rq *rq);
تهی scx_rescue_set_knobs(ساختار scx_sched *sch);
تهی scx_rescue_init(ساختار rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
ایستا درونخطی ساختار scx_sched *scx_cgroup_sched(ساختار cgroup *cgrp)
{
	بازگشت rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

ایستا درونخطی ثابت نویسه *sch_cgrp_path(ساختار scx_sched *sch)
{
	بازگشت sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
ایستا درونخطی تهی scx_dec_has_subs(ساختار scx_sched *sch)
{
	اگر (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#وگرنه	/* CONFIG_EXT_SUB_SCHED */

ایستا درونخطی ساختار scx_sched *scx_next_descendant_pre(ساختار scx_sched *pos, ساختار scx_sched *ریشه) { بازگشت pos ? پوچ : ریشه; }
ایستا درونخطی ساختار scx_sched *scx_skip_subtree_pre(ساختار scx_sched *pos, ساختار scx_sched *ریشه) { بازگشت پوچ; }
ایستا درونخطی تهی scx_set_task_sched(ساختار task_struct *p, ساختار scx_sched *sch) {}
ایستا درونخطی ساختار cgroup *sch_cgroup(ساختار scx_sched *sch) { بازگشت پوچ; }
ایستا درونخطی ثابت نویسه *sch_cgrp_path(ساختار scx_sched *sch) { بازگشت "/"; }
ایستا درونخطی تهی set_cgroup_sched(ساختار cgroup *cgrp, ساختار scx_sched *sch) {}
ایستا درونخطی تهی drain_descendants(ساختار scx_sched *sch) { }
ایستا درونخطی تهی scx_sub_disable(ساختار scx_sched *sch) { }
ایستا درونخطی تهی scx_free_pshards(ساختار scx_sched *sch) {}
ایستا درونخطی s32 scx_alloc_pshards(ساختار scx_sched *sch) { بازگشت 0; }
ایستا درونخطی تهی scx_init_ریشه_caps(ساختار scx_sched *sch) {}
ایستا درونخطی تهی scx_process_sync_ecaps(ساختار rq *rq, ساختار task_struct *prev) {}
ایستا درونخطی تهی scx_unbypass_replay_ecaps(ساختار rq *rq, ساختار scx_sched *sch) {}
ایستا درونخطی تهی scx_online_ecaps(ساختار rq *rq) {}
ایستا درونخطی تهی scx_offline_ecaps(ساختار rq *rq) {}
ایستا درونخطی تهی scx_discard_ecaps_to_sync(s32 cpu, ساختار scx_sched_pcpu *pcpu) {}
ایستا درونخطی تهی scx_discard_stale_ecaps_syncs(تهی) {}
ایستا درونخطی ساختار scx_dispatch_q *scx_resolve_local_dsq(ساختار scx_sched *sch, ساختار rq *rq, ساختار task_struct *p, u64 *enq_flags) { بازگشت &rq->scx.local_dsq; }
ایستا درونخطی bool scx_task_reenq_on_cap_revoke(ساختار rq *rq, ساختار task_struct *p) { بازگشت false; }
ایستا درونخطی تهی scx_reenq_reject(ساختار rq *rq) {}
ایستا درونخطی تهی scx_rescue_charge(ساختار rq *rq, s64 delta_exec) {}
ایستا درونخطی تهی scx_rescue_end(ساختار rq *rq) {}
ایستا درونخطی bool scx_rescue_keep(ساختار rq *rq, ساختار task_struct *p) { بازگشت false; }
ایستا درونخطی تهی scx_rescue_flush(ساختار rq *rq) {}
ایستا درونخطی تهی scx_rescue_dump(ساختار seq_buf *s, ساختار rq *rq) {}
ایستا درونخطی تهی scx_rescue_set_knobs(ساختار scx_sched *sch) {}
ایستا درونخطی تهی scx_rescue_init(ساختار rq *rq) {}
ایستا درونخطی تهی scx_dec_has_subs(ساختار scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, ریشه)					\
	برای ((pos) = scx_next_descendant_pre(پوچ, (ریشه)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (ریشه)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
ایستا درونخطی u64 scx_missing_caps(ساختار scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	اگر (!scx_has_subs())
		بازگشت 0;

	/* root holds every cap on every cpu */
	اگر (!sch->level)
		بازگشت 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	بازگشت needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
ایستا درونخطی u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	اگر (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		بازگشت 0;
	اگر (enq_flags & SCX_ENQ_IMMED)
		بازگشت SCX_CAP_ENQ_IMMED;
	بازگشت SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
ایستا درونخطی u64 scx_caps_for_task(ساختار task_struct *p)
{
	اگر (p->scx.flags & SCX_TASK_IMMED)
		بازگشت SCX_CAP_ENQ_IMMED;
	بازگشت SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
ایستا درونخطی u64 scx_caps_for_preempt(ساختار scx_sched *sch, ساختار rq *rq, u64 enq_flags)
{
	ساختار task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	اگر (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		بازگشت 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	اگر (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		بازگشت 0;
	بازگشت SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
ایستا درونخطی u64 scx_caps_implied(u64 cap)
{
	انتخاب (cap) {
	حالت SCX_CAP_PREEMPT:
		بازگشت SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	حالت SCX_CAP_ENQ:
		بازگشت SCX_CAP_ENQ_IMMED;
	}
	بازگشت 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
ایستا درونخطی bool scx_task_can_stay_on_cpu(ساختار rq *rq, ساختار task_struct *p)
{
	اگر (!scx_has_subs())
		بازگشت true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	اگر (unlikely(is_migration_disabled(p)))
		بازگشت true;

	بازگشت likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
ایستا درونخطی ساختار task_struct *scx_rescuee(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);

	اگر (!scx_has_subs())
		بازگشت پوچ;

	بازگشت rq->scx.rescue.curr;
}

#وگرنه	/* CONFIG_EXT_SUB_SCHED */

ایستا درونخطی u64 scx_missing_caps(ساختار scx_sched *sch, s32 cpu, u64 needed) { بازگشت 0; }
ایستا درونخطی u64 scx_caps_for_preempt(ساختار scx_sched *sch, ساختار rq *rq, u64 enq_flags) { بازگشت 0; }
ایستا درونخطی bool scx_task_can_stay_on_cpu(ساختار rq *rq, ساختار task_struct *p) { بازگشت true; }
ایستا درونخطی ساختار task_struct *scx_rescuee(ساختار rq *rq) { بازگشت پوچ; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
