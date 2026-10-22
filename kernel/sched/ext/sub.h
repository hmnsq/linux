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

#अन्तर्भाव "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

संरचना scx_sched *scx_skip_subtree_pre(संरचना scx_sched *pos, संरचना scx_sched *मूळ);
संरचना scx_sched *scx_next_descendant_pre(संरचना scx_sched *pos, संरचना scx_sched *मूळ);
शून्य scx_set_task_sched(संरचना task_struct *p, संरचना scx_sched *sch);
संरचना cgroup *sch_cgroup(संरचना scx_sched *sch);
शून्य set_cgroup_sched(संरचना cgroup *cgrp, संरचना scx_sched *sch);
शून्य scx_pstack_recursion_on_dispatch(संरचना bpf_prog *prog);
शून्य scx_pstack_recursion_on_caps_updated(संरचना bpf_prog *prog);
शून्य drain_descendants(संरचना scx_sched *sch);
शून्य scx_sub_disable(संरचना scx_sched *sch);
शून्य scx_sub_enable_workfn(संरचना kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, ध्रुव संरचना bpf_prog_aux *aux);
शून्य scx_free_pshards(संरचना scx_sched *sch);
s32 scx_alloc_pshards(संरचना scx_sched *sch);
शून्य scx_init_मूळ_caps(संरचना scx_sched *sch);
शून्य scx_process_sync_ecaps(संरचना rq *rq, संरचना task_struct *prev);
शून्य scx_unbypass_replay_ecaps(संरचना rq *rq, संरचना scx_sched *sch);
शून्य scx_online_ecaps(संरचना rq *rq);
शून्य scx_offline_ecaps(संरचना rq *rq);
शून्य scx_discard_ecaps_to_sync(s32 cpu, संरचना scx_sched_pcpu *pcpu);
शून्य scx_discard_stale_ecaps_syncs(शून्य);
संरचना scx_dispatch_q *scx_resolve_local_dsq(संरचना scx_sched *sch, संरचना rq *rq,
					     संरचना task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(संरचना rq *rq, संरचना task_struct *p);
शून्य scx_reenq_reject(संरचना rq *rq);
शून्य scx_rescue_charge(संरचना rq *rq, s64 delta_exec);
शून्य scx_rescue_end(संरचना rq *rq);
bool scx_rescue_keep(संरचना rq *rq, संरचना task_struct *p);
शून्य scx_rescue_flush(संरचना rq *rq);
शून्य scx_rescue_dump(संरचना seq_buf *s, संरचना rq *rq);
शून्य scx_rescue_set_knobs(संरचना scx_sched *sch);
शून्य scx_rescue_init(संरचना rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
स्थिर अन्तःस्थ संरचना scx_sched *scx_cgroup_sched(संरचना cgroup *cgrp)
{
	प्रत्यागम rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

स्थिर अन्तःस्थ ध्रुव वर्ण *sch_cgrp_path(संरचना scx_sched *sch)
{
	प्रत्यागम sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
स्थिर अन्तःस्थ शून्य scx_dec_has_subs(संरचना scx_sched *sch)
{
	यदि (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#अन्यथा	/* CONFIG_EXT_SUB_SCHED */

स्थिर अन्तःस्थ संरचना scx_sched *scx_next_descendant_pre(संरचना scx_sched *pos, संरचना scx_sched *मूळ) { प्रत्यागम pos ? रिक्त : मूळ; }
स्थिर अन्तःस्थ संरचना scx_sched *scx_skip_subtree_pre(संरचना scx_sched *pos, संरचना scx_sched *मूळ) { प्रत्यागम रिक्त; }
स्थिर अन्तःस्थ शून्य scx_set_task_sched(संरचना task_struct *p, संरचना scx_sched *sch) {}
स्थिर अन्तःस्थ संरचना cgroup *sch_cgroup(संरचना scx_sched *sch) { प्रत्यागम रिक्त; }
स्थिर अन्तःस्थ ध्रुव वर्ण *sch_cgrp_path(संरचना scx_sched *sch) { प्रत्यागम "/"; }
स्थिर अन्तःस्थ शून्य set_cgroup_sched(संरचना cgroup *cgrp, संरचना scx_sched *sch) {}
स्थिर अन्तःस्थ शून्य drain_descendants(संरचना scx_sched *sch) { }
स्थिर अन्तःस्थ शून्य scx_sub_disable(संरचना scx_sched *sch) { }
स्थिर अन्तःस्थ शून्य scx_free_pshards(संरचना scx_sched *sch) {}
स्थिर अन्तःस्थ s32 scx_alloc_pshards(संरचना scx_sched *sch) { प्रत्यागम 0; }
स्थिर अन्तःस्थ शून्य scx_init_मूळ_caps(संरचना scx_sched *sch) {}
स्थिर अन्तःस्थ शून्य scx_process_sync_ecaps(संरचना rq *rq, संरचना task_struct *prev) {}
स्थिर अन्तःस्थ शून्य scx_unbypass_replay_ecaps(संरचना rq *rq, संरचना scx_sched *sch) {}
स्थिर अन्तःस्थ शून्य scx_online_ecaps(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_offline_ecaps(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_discard_ecaps_to_sync(s32 cpu, संरचना scx_sched_pcpu *pcpu) {}
स्थिर अन्तःस्थ शून्य scx_discard_stale_ecaps_syncs(शून्य) {}
स्थिर अन्तःस्थ संरचना scx_dispatch_q *scx_resolve_local_dsq(संरचना scx_sched *sch, संरचना rq *rq, संरचना task_struct *p, u64 *enq_flags) { प्रत्यागम &rq->scx.local_dsq; }
स्थिर अन्तःस्थ bool scx_task_reenq_on_cap_revoke(संरचना rq *rq, संरचना task_struct *p) { प्रत्यागम false; }
स्थिर अन्तःस्थ शून्य scx_reenq_reject(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_rescue_charge(संरचना rq *rq, s64 delta_exec) {}
स्थिर अन्तःस्थ शून्य scx_rescue_end(संरचना rq *rq) {}
स्थिर अन्तःस्थ bool scx_rescue_keep(संरचना rq *rq, संरचना task_struct *p) { प्रत्यागम false; }
स्थिर अन्तःस्थ शून्य scx_rescue_flush(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_rescue_dump(संरचना seq_buf *s, संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_rescue_set_knobs(संरचना scx_sched *sch) {}
स्थिर अन्तःस्थ शून्य scx_rescue_init(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_dec_has_subs(संरचना scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, मूळ)					\
	प्रति ((pos) = scx_next_descendant_pre(रिक्त, (मूळ)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (मूळ)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
स्थिर अन्तःस्थ u64 scx_missing_caps(संरचना scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	यदि (!scx_has_subs())
		प्रत्यागम 0;

	/* root holds every cap on every cpu */
	यदि (!sch->level)
		प्रत्यागम 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	प्रत्यागम needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
स्थिर अन्तःस्थ u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	यदि (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		प्रत्यागम 0;
	यदि (enq_flags & SCX_ENQ_IMMED)
		प्रत्यागम SCX_CAP_ENQ_IMMED;
	प्रत्यागम SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
स्थिर अन्तःस्थ u64 scx_caps_for_task(संरचना task_struct *p)
{
	यदि (p->scx.flags & SCX_TASK_IMMED)
		प्रत्यागम SCX_CAP_ENQ_IMMED;
	प्रत्यागम SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
स्थिर अन्तःस्थ u64 scx_caps_for_preempt(संरचना scx_sched *sch, संरचना rq *rq, u64 enq_flags)
{
	संरचना task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	यदि (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		प्रत्यागम 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	यदि (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		प्रत्यागम 0;
	प्रत्यागम SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
स्थिर अन्तःस्थ u64 scx_caps_implied(u64 cap)
{
	विकल्प (cap) {
	पक्ष SCX_CAP_PREEMPT:
		प्रत्यागम SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	पक्ष SCX_CAP_ENQ:
		प्रत्यागम SCX_CAP_ENQ_IMMED;
	}
	प्रत्यागम 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
स्थिर अन्तःस्थ bool scx_task_can_stay_on_cpu(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (!scx_has_subs())
		प्रत्यागम true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	यदि (unlikely(is_migration_disabled(p)))
		प्रत्यागम true;

	प्रत्यागम likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
स्थिर अन्तःस्थ संरचना task_struct *scx_rescuee(संरचना rq *rq)
{
	lockdep_assert_rq_held(rq);

	यदि (!scx_has_subs())
		प्रत्यागम रिक्त;

	प्रत्यागम rq->scx.rescue.curr;
}

#अन्यथा	/* CONFIG_EXT_SUB_SCHED */

स्थिर अन्तःस्थ u64 scx_missing_caps(संरचना scx_sched *sch, s32 cpu, u64 needed) { प्रत्यागम 0; }
स्थिर अन्तःस्थ u64 scx_caps_for_preempt(संरचना scx_sched *sch, संरचना rq *rq, u64 enq_flags) { प्रत्यागम 0; }
स्थिर अन्तःस्थ bool scx_task_can_stay_on_cpu(संरचना rq *rq, संरचना task_struct *p) { प्रत्यागम true; }
स्थिर अन्तःस्थ संरचना task_struct *scx_rescuee(संरचना rq *rq) { प्रत्यागम रिक्त; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
