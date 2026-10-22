/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Sub-scheduler hierarchy support.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#अगर_अघोषित _KERNEL_SCHED_EXT_SUB_H
#घोषणा _KERNEL_SCHED_EXT_SUB_H

#समावेश "internal.h"

#अगर_घोषित CONFIG_EXT_SUB_SCHED

काष्ठा scx_sched *scx_skip_subtree_pre(काष्ठा scx_sched *pos, काष्ठा scx_sched *मूल);
काष्ठा scx_sched *scx_अग्र_descendant_pre(काष्ठा scx_sched *pos, काष्ठा scx_sched *मूल);
व्योम scx_set_task_sched(काष्ठा task_struct *p, काष्ठा scx_sched *sch);
काष्ठा cgroup *sch_cgroup(काष्ठा scx_sched *sch);
व्योम set_cgroup_sched(काष्ठा cgroup *cgrp, काष्ठा scx_sched *sch);
व्योम scx_pstack_recursion_on_dispatch(काष्ठा bpf_prog *prog);
व्योम scx_pstack_recursion_on_caps_updated(काष्ठा bpf_prog *prog);
व्योम drain_descendants(काष्ठा scx_sched *sch);
व्योम scx_sub_disable(काष्ठा scx_sched *sch);
व्योम scx_sub_enable_workfn(काष्ठा kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, स्थिर काष्ठा bpf_prog_aux *aux);
व्योम scx_free_pshards(काष्ठा scx_sched *sch);
s32 scx_alloc_pshards(काष्ठा scx_sched *sch);
व्योम scx_init_मूल_caps(काष्ठा scx_sched *sch);
व्योम scx_process_sync_ecaps(काष्ठा rq *rq, काष्ठा task_struct *पूर्व);
व्योम scx_unbypass_replay_ecaps(काष्ठा rq *rq, काष्ठा scx_sched *sch);
व्योम scx_online_ecaps(काष्ठा rq *rq);
व्योम scx_offline_ecaps(काष्ठा rq *rq);
व्योम scx_discard_ecaps_to_sync(s32 cpu, काष्ठा scx_sched_pcpu *pcpu);
व्योम scx_discard_stale_ecaps_syncs(व्योम);
काष्ठा scx_dispatch_q *scx_resolve_local_dsq(काष्ठा scx_sched *sch, काष्ठा rq *rq,
					     काष्ठा task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(काष्ठा rq *rq, काष्ठा task_struct *p);
व्योम scx_reenq_reject(काष्ठा rq *rq);
व्योम scx_rescue_charge(काष्ठा rq *rq, s64 delta_exec);
व्योम scx_rescue_end(काष्ठा rq *rq);
bool scx_rescue_keep(काष्ठा rq *rq, काष्ठा task_struct *p);
व्योम scx_rescue_flush(काष्ठा rq *rq);
व्योम scx_rescue_dump(काष्ठा seq_buf *s, काष्ठा rq *rq);
व्योम scx_rescue_set_knobs(काष्ठा scx_sched *sch);
व्योम scx_rescue_init(काष्ठा rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
अटल अंतरभूत काष्ठा scx_sched *scx_cgroup_sched(काष्ठा cgroup *cgrp)
{
	वापस rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

अटल अंतरभूत स्थिर अक्षर *sch_cgrp_path(काष्ठा scx_sched *sch)
{
	वापस sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
अटल अंतरभूत व्योम scx_dec_has_subs(काष्ठा scx_sched *sch)
{
	अगर (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#अन्यथा	/* CONFIG_EXT_SUB_SCHED */

अटल अंतरभूत काष्ठा scx_sched *scx_अग्र_descendant_pre(काष्ठा scx_sched *pos, काष्ठा scx_sched *मूल) { वापस pos ? शून्य : मूल; }
अटल अंतरभूत काष्ठा scx_sched *scx_skip_subtree_pre(काष्ठा scx_sched *pos, काष्ठा scx_sched *मूल) { वापस शून्य; }
अटल अंतरभूत व्योम scx_set_task_sched(काष्ठा task_struct *p, काष्ठा scx_sched *sch) {}
अटल अंतरभूत काष्ठा cgroup *sch_cgroup(काष्ठा scx_sched *sch) { वापस शून्य; }
अटल अंतरभूत स्थिर अक्षर *sch_cgrp_path(काष्ठा scx_sched *sch) { वापस "/"; }
अटल अंतरभूत व्योम set_cgroup_sched(काष्ठा cgroup *cgrp, काष्ठा scx_sched *sch) {}
अटल अंतरभूत व्योम drain_descendants(काष्ठा scx_sched *sch) { }
अटल अंतरभूत व्योम scx_sub_disable(काष्ठा scx_sched *sch) { }
अटल अंतरभूत व्योम scx_free_pshards(काष्ठा scx_sched *sch) {}
अटल अंतरभूत s32 scx_alloc_pshards(काष्ठा scx_sched *sch) { वापस 0; }
अटल अंतरभूत व्योम scx_init_मूल_caps(काष्ठा scx_sched *sch) {}
अटल अंतरभूत व्योम scx_process_sync_ecaps(काष्ठा rq *rq, काष्ठा task_struct *पूर्व) {}
अटल अंतरभूत व्योम scx_unbypass_replay_ecaps(काष्ठा rq *rq, काष्ठा scx_sched *sch) {}
अटल अंतरभूत व्योम scx_online_ecaps(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_offline_ecaps(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_discard_ecaps_to_sync(s32 cpu, काष्ठा scx_sched_pcpu *pcpu) {}
अटल अंतरभूत व्योम scx_discard_stale_ecaps_syncs(व्योम) {}
अटल अंतरभूत काष्ठा scx_dispatch_q *scx_resolve_local_dsq(काष्ठा scx_sched *sch, काष्ठा rq *rq, काष्ठा task_struct *p, u64 *enq_flags) { वापस &rq->scx.local_dsq; }
अटल अंतरभूत bool scx_task_reenq_on_cap_revoke(काष्ठा rq *rq, काष्ठा task_struct *p) { वापस false; }
अटल अंतरभूत व्योम scx_reenq_reject(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_rescue_charge(काष्ठा rq *rq, s64 delta_exec) {}
अटल अंतरभूत व्योम scx_rescue_end(काष्ठा rq *rq) {}
अटल अंतरभूत bool scx_rescue_keep(काष्ठा rq *rq, काष्ठा task_struct *p) { वापस false; }
अटल अंतरभूत व्योम scx_rescue_flush(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_rescue_dump(काष्ठा seq_buf *s, काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_rescue_set_knobs(काष्ठा scx_sched *sch) {}
अटल अंतरभूत व्योम scx_rescue_init(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_dec_has_subs(काष्ठा scx_sched *sch) {}

#पूर्ण_अगर	/* CONFIG_EXT_SUB_SCHED */

/**
 * scx_for_each_descendant_pre - pre-order walk of a sched's descendants
 * @pos: iteration cursor
 * @root: sched to walk the descendants of
 *
 * Walk @root's descendants. @root is included in the iteration and the first
 * node to be visited. Must be called with scx_enable_mutex, scx_sched_lock, or
 * RCU read lock.
 */
#घोषणा scx_for_each_descendant_pre(pos, मूल)					\
	क्रम ((pos) = scx_अग्र_descendant_pre(शून्य, (मूल)); (pos);		\
	     (pos) = scx_अग्र_descendant_pre((pos), (मूल)))

#अगर_घोषित CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
अटल अंतरभूत u64 scx_missing_caps(काष्ठा scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	अगर (!scx_has_subs())
		वापस 0;

	/* root holds every cap on every cpu */
	अगर (!sch->level)
		वापस 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	वापस needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
अटल अंतरभूत u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	अगर (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		वापस 0;
	अगर (enq_flags & SCX_ENQ_IMMED)
		वापस SCX_CAP_ENQ_IMMED;
	वापस SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
अटल अंतरभूत u64 scx_caps_for_task(काष्ठा task_struct *p)
{
	अगर (p->scx.flags & SCX_TASK_IMMED)
		वापस SCX_CAP_ENQ_IMMED;
	वापस SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
अटल अंतरभूत u64 scx_caps_for_preempt(काष्ठा scx_sched *sch, काष्ठा rq *rq, u64 enq_flags)
{
	काष्ठा task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	अगर (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		वापस 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	अगर (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		वापस 0;
	वापस SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
अटल अंतरभूत u64 scx_caps_implied(u64 cap)
{
	चयन (cap) {
	हाल SCX_CAP_PREEMPT:
		वापस SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	हाल SCX_CAP_ENQ:
		वापस SCX_CAP_ENQ_IMMED;
	}
	वापस 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
अटल अंतरभूत bool scx_task_can_stay_on_cpu(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अगर (!scx_has_subs())
		वापस true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	अगर (unlikely(is_migration_disabled(p)))
		वापस true;

	वापस likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
अटल अंतरभूत काष्ठा task_struct *scx_rescuee(काष्ठा rq *rq)
{
	lockdep_assert_rq_held(rq);

	अगर (!scx_has_subs())
		वापस शून्य;

	वापस rq->scx.rescue.curr;
}

#अन्यथा	/* CONFIG_EXT_SUB_SCHED */

अटल अंतरभूत u64 scx_missing_caps(काष्ठा scx_sched *sch, s32 cpu, u64 needed) { वापस 0; }
अटल अंतरभूत u64 scx_caps_for_preempt(काष्ठा scx_sched *sch, काष्ठा rq *rq, u64 enq_flags) { वापस 0; }
अटल अंतरभूत bool scx_task_can_stay_on_cpu(काष्ठा rq *rq, काष्ठा task_struct *p) { वापस true; }
अटल अंतरभूत काष्ठा task_struct *scx_rescuee(काष्ठा rq *rq) { वापस शून्य; }

#पूर्ण_अगर	/* CONFIG_EXT_SUB_SCHED */

#पूर्ण_अगर /* _KERNEL_SCHED_EXT_SUB_H */
