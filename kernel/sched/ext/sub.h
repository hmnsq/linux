/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Sub-scheduler hierarchy support.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#اگر_غیر_تعریف_شدہ _KERNEL_SCHED_EXT_SUB_H
#تعریف_کرو _KERNEL_SCHED_EXT_SUB_H

#شامل "internal.h"

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED

ساخت scx_sched *scx_skip_subtree_pre(ساخت scx_sched *pos, ساخت scx_sched *جڑ);
ساخت scx_sched *scx_اگلا_descendant_pre(ساخت scx_sched *pos, ساخت scx_sched *جڑ);
خالی scx_سیٹ_task_sched(ساخت task_struct *p, ساخت scx_sched *sch);
ساخت cgroup *sch_cgroup(ساخت scx_sched *sch);
خالی سیٹ_cgroup_sched(ساخت cgroup *cgrp, ساخت scx_sched *sch);
خالی scx_pstack_recursion_on_dispatch(ساخت bpf_prog *prog);
خالی scx_pstack_recursion_on_caps_updated(ساخت bpf_prog *prog);
خالی drain_descendants(ساخت scx_sched *sch);
خالی scx_sub_disable(ساخت scx_sched *sch);
خالی scx_sub_enable_workfn(ساخت kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ثابت ساخت bpf_prog_aux *aux);
خالی scx_free_pshards(ساخت scx_sched *sch);
s32 scx_alloc_pshards(ساخت scx_sched *sch);
خالی scx_init_جڑ_caps(ساخت scx_sched *sch);
خالی scx_process_sync_ecaps(ساخت rq *rq, ساخت task_struct *پچھلا);
خالی scx_unbypass_replay_ecaps(ساخت rq *rq, ساخت scx_sched *sch);
خالی scx_online_ecaps(ساخت rq *rq);
خالی scx_offline_ecaps(ساخت rq *rq);
خالی scx_discard_ecaps_مقصد_sync(s32 cpu, ساخت scx_sched_pcpu *pcpu);
خالی scx_discard_stale_ecaps_syncs(خالی);
ساخت scx_dispatch_q *scx_resolve_local_dsq(ساخت scx_sched *sch, ساخت rq *rq,
					     ساخت task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(ساخت rq *rq, ساخت task_struct *p);
خالی scx_reenq_reject(ساخت rq *rq);
خالی scx_rescue_charge(ساخت rq *rq, s64 delta_exec);
خالی scx_rescue_انتہا(ساخت rq *rq);
bool scx_rescue_keep(ساخت rq *rq, ساخت task_struct *p);
خالی scx_rescue_flush(ساخت rq *rq);
خالی scx_rescue_dump(ساخت seq_buf *s, ساخت rq *rq);
خالی scx_rescue_سیٹ_knobs(ساخت scx_sched *sch);
خالی scx_rescue_init(ساخت rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
ساکن اندرونی ساخت scx_sched *scx_cgroup_sched(ساخت cgroup *cgrp)
{
	واپس rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

ساکن اندرونی ثابت حرف *sch_cgrp_راستہ(ساخت scx_sched *sch)
{
	واپس sch->cgrp_راستہ;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
ساکن اندرونی خالی scx_dec_has_subs(ساخت scx_sched *sch)
{
	اگر (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#ورنہ	/* CONFIG_EXT_SUB_SCHED */

ساکن اندرونی ساخت scx_sched *scx_اگلا_descendant_pre(ساخت scx_sched *pos, ساخت scx_sched *جڑ) { واپس pos ? تہی : جڑ; }
ساکن اندرونی ساخت scx_sched *scx_skip_subtree_pre(ساخت scx_sched *pos, ساخت scx_sched *جڑ) { واپس تہی; }
ساکن اندرونی خالی scx_سیٹ_task_sched(ساخت task_struct *p, ساخت scx_sched *sch) {}
ساکن اندرونی ساخت cgroup *sch_cgroup(ساخت scx_sched *sch) { واپس تہی; }
ساکن اندرونی ثابت حرف *sch_cgrp_راستہ(ساخت scx_sched *sch) { واپس "/"; }
ساکن اندرونی خالی سیٹ_cgroup_sched(ساخت cgroup *cgrp, ساخت scx_sched *sch) {}
ساکن اندرونی خالی drain_descendants(ساخت scx_sched *sch) { }
ساکن اندرونی خالی scx_sub_disable(ساخت scx_sched *sch) { }
ساکن اندرونی خالی scx_free_pshards(ساخت scx_sched *sch) {}
ساکن اندرونی s32 scx_alloc_pshards(ساخت scx_sched *sch) { واپس 0; }
ساکن اندرونی خالی scx_init_جڑ_caps(ساخت scx_sched *sch) {}
ساکن اندرونی خالی scx_process_sync_ecaps(ساخت rq *rq, ساخت task_struct *پچھلا) {}
ساکن اندرونی خالی scx_unbypass_replay_ecaps(ساخت rq *rq, ساخت scx_sched *sch) {}
ساکن اندرونی خالی scx_online_ecaps(ساخت rq *rq) {}
ساکن اندرونی خالی scx_offline_ecaps(ساخت rq *rq) {}
ساکن اندرونی خالی scx_discard_ecaps_مقصد_sync(s32 cpu, ساخت scx_sched_pcpu *pcpu) {}
ساکن اندرونی خالی scx_discard_stale_ecaps_syncs(خالی) {}
ساکن اندرونی ساخت scx_dispatch_q *scx_resolve_local_dsq(ساخت scx_sched *sch, ساخت rq *rq, ساخت task_struct *p, u64 *enq_flags) { واپس &rq->scx.local_dsq; }
ساکن اندرونی bool scx_task_reenq_on_cap_revoke(ساخت rq *rq, ساخت task_struct *p) { واپس false; }
ساکن اندرونی خالی scx_reenq_reject(ساخت rq *rq) {}
ساکن اندرونی خالی scx_rescue_charge(ساخت rq *rq, s64 delta_exec) {}
ساکن اندرونی خالی scx_rescue_انتہا(ساخت rq *rq) {}
ساکن اندرونی bool scx_rescue_keep(ساخت rq *rq, ساخت task_struct *p) { واپس false; }
ساکن اندرونی خالی scx_rescue_flush(ساخت rq *rq) {}
ساکن اندرونی خالی scx_rescue_dump(ساخت seq_buf *s, ساخت rq *rq) {}
ساکن اندرونی خالی scx_rescue_سیٹ_knobs(ساخت scx_sched *sch) {}
ساکن اندرونی خالی scx_rescue_init(ساخت rq *rq) {}
ساکن اندرونی خالی scx_dec_has_subs(ساخت scx_sched *sch) {}

#اگر_ختم	/* CONFIG_EXT_SUB_SCHED */

/**
 * scx_for_each_descendant_pre - pre-order walk of a sched's descendants
 * @pos: iteration cursor
 * @root: sched to walk the descendants of
 *
 * Walk @root's descendants. @root is included in the iteration and the first
 * node to be visited. Must be called with scx_enable_mutex, scx_sched_lock, or
 * RCU read lock.
 */
#تعریف_کرو scx_for_each_descendant_pre(pos, جڑ)					\
	برائے ((pos) = scx_اگلا_descendant_pre(تہی, (جڑ)); (pos);		\
	     (pos) = scx_اگلا_descendant_pre((pos), (جڑ)))

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
ساکن اندرونی u64 scx_missing_caps(ساخت scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	اگر (!scx_has_subs())
		واپس 0;

	/* root holds every cap on every cpu */
	اگر (!sch->level)
		واپس 0;

	ecaps = READ_ONCE(فی_cpu_ptr(sch->pcpu, cpu)->ecaps);

	واپس needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
ساکن اندرونی u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	اگر (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		واپس 0;
	اگر (enq_flags & SCX_ENQ_IMMED)
		واپس SCX_CAP_ENQ_IMMED;
	واپس SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
ساکن اندرونی u64 scx_caps_for_task(ساخت task_struct *p)
{
	اگر (p->scx.flags & SCX_TASK_IMMED)
		واپس SCX_CAP_ENQ_IMMED;
	واپس SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
ساکن اندرونی u64 scx_caps_for_preempt(ساخت scx_sched *sch, ساخت rq *rq, u64 enq_flags)
{
	ساخت task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	اگر (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		واپس 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	اگر (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		واپس 0;
	واپس SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
ساکن اندرونی u64 scx_caps_implied(u64 cap)
{
	انتخاب (cap) {
	حالت SCX_CAP_PREEMPT:
		واپس SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	حالت SCX_CAP_ENQ:
		واپس SCX_CAP_ENQ_IMMED;
	}
	واپس 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
ساکن اندرونی bool scx_task_can_stay_on_cpu(ساخت rq *rq, ساخت task_struct *p)
{
	اگر (!scx_has_subs())
		واپس true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	اگر (unlikely(is_migration_disabled(p)))
		واپس true;

	واپس likely(!scx_missing_caps(scx_task_sched(p), cpu_کا(rq), SCX_CAP_بنیاد));
}

/* the task admitted for rescue on @rq, NULL if none */
ساکن اندرونی ساخت task_struct *scx_rescuee(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);

	اگر (!scx_has_subs())
		واپس تہی;

	واپس rq->scx.rescue.curr;
}

#ورنہ	/* CONFIG_EXT_SUB_SCHED */

ساکن اندرونی u64 scx_missing_caps(ساخت scx_sched *sch, s32 cpu, u64 needed) { واپس 0; }
ساکن اندرونی u64 scx_caps_for_preempt(ساخت scx_sched *sch, ساخت rq *rq, u64 enq_flags) { واپس 0; }
ساکن اندرونی bool scx_task_can_stay_on_cpu(ساخت rq *rq, ساخت task_struct *p) { واپس true; }
ساکن اندرونی ساخت task_struct *scx_rescuee(ساخت rq *rq) { واپس تہی; }

#اگر_ختم	/* CONFIG_EXT_SUB_SCHED */

#اگر_ختم /* _KERNEL_SCHED_EXT_SUB_H */
