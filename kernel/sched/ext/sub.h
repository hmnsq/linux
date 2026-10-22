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

#அந்தர்பாவ "internal.h"

#ifdef CONFIG_EXT_SUB_SCHED

கட்டமைப்பு scx_sched *scx_skip_subtree_pre(கட்டமைப்பு scx_sched *pos, கட்டமைப்பு scx_sched *வேர்);
கட்டமைப்பு scx_sched *scx_next_descendant_pre(கட்டமைப்பு scx_sched *pos, கட்டமைப்பு scx_sched *வேர்);
வெறுமை scx_set_task_sched(கட்டமைப்பு task_struct *p, கட்டமைப்பு scx_sched *sch);
கட்டமைப்பு cgroup *sch_cgroup(கட்டமைப்பு scx_sched *sch);
வெறுமை set_cgroup_sched(கட்டமைப்பு cgroup *cgrp, கட்டமைப்பு scx_sched *sch);
வெறுமை scx_pstack_recursion_on_dispatch(கட்டமைப்பு bpf_prog *prog);
வெறுமை scx_pstack_recursion_on_caps_updated(கட்டமைப்பு bpf_prog *prog);
வெறுமை drain_descendants(கட்டமைப்பு scx_sched *sch);
வெறுமை scx_sub_disable(கட்டமைப்பு scx_sched *sch);
வெறுமை scx_sub_enable_workfn(கட்டமைப்பு kthread_work *work);
bool scx_bpf_sub_dispatch(u64 cgroup_id, மாறிலி கட்டமைப்பு bpf_prog_aux *aux);
வெறுமை scx_free_pshards(கட்டமைப்பு scx_sched *sch);
s32 scx_alloc_pshards(கட்டமைப்பு scx_sched *sch);
வெறுமை scx_init_வேர்_caps(கட்டமைப்பு scx_sched *sch);
வெறுமை scx_process_sync_ecaps(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev);
வெறுமை scx_unbypass_replay_ecaps(கட்டமைப்பு rq *rq, கட்டமைப்பு scx_sched *sch);
வெறுமை scx_online_ecaps(கட்டமைப்பு rq *rq);
வெறுமை scx_offline_ecaps(கட்டமைப்பு rq *rq);
வெறுமை scx_discard_ecaps_to_sync(s32 cpu, கட்டமைப்பு scx_sched_pcpu *pcpu);
வெறுமை scx_discard_stale_ecaps_syncs(வெறுமை);
கட்டமைப்பு scx_dispatch_q *scx_resolve_local_dsq(கட்டமைப்பு scx_sched *sch, கட்டமைப்பு rq *rq,
					     கட்டமைப்பு task_struct *p, u64 *enq_flags);
bool scx_task_reenq_on_cap_revoke(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p);
வெறுமை scx_reenq_reject(கட்டமைப்பு rq *rq);
வெறுமை scx_rescue_charge(கட்டமைப்பு rq *rq, s64 delta_exec);
வெறுமை scx_rescue_end(கட்டமைப்பு rq *rq);
bool scx_rescue_keep(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p);
வெறுமை scx_rescue_flush(கட்டமைப்பு rq *rq);
வெறுமை scx_rescue_dump(கட்டமைப்பு seq_buf *s, கட்டமைப்பு rq *rq);
வெறுமை scx_rescue_set_knobs(கட்டமைப்பு scx_sched *sch);
வெறுமை scx_rescue_init(கட்டமைப்பு rq *rq);

/*
 * cgrp->scx_sched is written by root/sub enable/disable under all of
 * scx_enable_mutex, scx_fork_rwsem and cgroup_mutex. A new cgroup inherits the
 * parent's sched under just cgroup_mutex but is not yet reachable by the other
 * two lock holders. Any one of the three locks stabilizes the association.
 */
நிலையான உள்வரி கட்டமைப்பு scx_sched *scx_cgroup_sched(கட்டமைப்பு cgroup *cgrp)
{
	திரும்பு rcu_dereference_check(cgrp->scx_sched,
				     lockdep_is_held(&cgroup_mutex) ||
				     percpu_rwsem_is_held(&scx_fork_rwsem) ||
				     lockdep_is_held(&scx_enable_mutex));
}

நிலையான உள்வரி மாறிலி எழுத்து *sch_cgrp_path(கட்டமைப்பு scx_sched *sch)
{
	திரும்பு sch->cgrp_path;
}

/* a dying sub's hot-path influence ends in scx_sched_free_rcu_work() */
நிலையான உள்வரி வெறுமை scx_dec_has_subs(கட்டமைப்பு scx_sched *sch)
{
	எனில் (sch->level)
		static_branch_dec(&__scx_has_subs);
}

#இல்லையெனில்	/* CONFIG_EXT_SUB_SCHED */

நிலையான உள்வரி கட்டமைப்பு scx_sched *scx_next_descendant_pre(கட்டமைப்பு scx_sched *pos, கட்டமைப்பு scx_sched *வேர்) { திரும்பு pos ? வெற்று : வேர்; }
நிலையான உள்வரி கட்டமைப்பு scx_sched *scx_skip_subtree_pre(கட்டமைப்பு scx_sched *pos, கட்டமைப்பு scx_sched *வேர்) { திரும்பு வெற்று; }
நிலையான உள்வரி வெறுமை scx_set_task_sched(கட்டமைப்பு task_struct *p, கட்டமைப்பு scx_sched *sch) {}
நிலையான உள்வரி கட்டமைப்பு cgroup *sch_cgroup(கட்டமைப்பு scx_sched *sch) { திரும்பு வெற்று; }
நிலையான உள்வரி மாறிலி எழுத்து *sch_cgrp_path(கட்டமைப்பு scx_sched *sch) { திரும்பு "/"; }
நிலையான உள்வரி வெறுமை set_cgroup_sched(கட்டமைப்பு cgroup *cgrp, கட்டமைப்பு scx_sched *sch) {}
நிலையான உள்வரி வெறுமை drain_descendants(கட்டமைப்பு scx_sched *sch) { }
நிலையான உள்வரி வெறுமை scx_sub_disable(கட்டமைப்பு scx_sched *sch) { }
நிலையான உள்வரி வெறுமை scx_free_pshards(கட்டமைப்பு scx_sched *sch) {}
நிலையான உள்வரி s32 scx_alloc_pshards(கட்டமைப்பு scx_sched *sch) { திரும்பு 0; }
நிலையான உள்வரி வெறுமை scx_init_வேர்_caps(கட்டமைப்பு scx_sched *sch) {}
நிலையான உள்வரி வெறுமை scx_process_sync_ecaps(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev) {}
நிலையான உள்வரி வெறுமை scx_unbypass_replay_ecaps(கட்டமைப்பு rq *rq, கட்டமைப்பு scx_sched *sch) {}
நிலையான உள்வரி வெறுமை scx_online_ecaps(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_offline_ecaps(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_discard_ecaps_to_sync(s32 cpu, கட்டமைப்பு scx_sched_pcpu *pcpu) {}
நிலையான உள்வரி வெறுமை scx_discard_stale_ecaps_syncs(வெறுமை) {}
நிலையான உள்வரி கட்டமைப்பு scx_dispatch_q *scx_resolve_local_dsq(கட்டமைப்பு scx_sched *sch, கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, u64 *enq_flags) { திரும்பு &rq->scx.local_dsq; }
நிலையான உள்வரி bool scx_task_reenq_on_cap_revoke(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p) { திரும்பு false; }
நிலையான உள்வரி வெறுமை scx_reenq_reject(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_rescue_charge(கட்டமைப்பு rq *rq, s64 delta_exec) {}
நிலையான உள்வரி வெறுமை scx_rescue_end(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி bool scx_rescue_keep(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p) { திரும்பு false; }
நிலையான உள்வரி வெறுமை scx_rescue_flush(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_rescue_dump(கட்டமைப்பு seq_buf *s, கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_rescue_set_knobs(கட்டமைப்பு scx_sched *sch) {}
நிலையான உள்வரி வெறுமை scx_rescue_init(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_dec_has_subs(கட்டமைப்பு scx_sched *sch) {}

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
#define scx_for_each_descendant_pre(pos, வேர்)					\
	க்கு ((pos) = scx_next_descendant_pre(வெற்று, (வேர்)); (pos);		\
	     (pos) = scx_next_descendant_pre((pos), (வேர்)))

#ifdef CONFIG_EXT_SUB_SCHED

/**
 * scx_missing_caps - The caps in @needed that @sch lacks on @cpu
 * @sch: sched to test
 * @cpu: cpu to test on
 * @needed: bitmask of SCX_CAP_* values
 *
 * Return the caps in @needed that @sch lacks for @cpu, 0 if it holds them all.
 */
நிலையான உள்வரி u64 scx_missing_caps(கட்டமைப்பு scx_sched *sch, s32 cpu, u64 needed)
{
	u64 ecaps;

	/* no sub-scheds, no missing caps */
	எனில் (!scx_has_subs())
		திரும்பு 0;

	/* root holds every cap on every cpu */
	எனில் (!sch->level)
		திரும்பு 0;

	ecaps = READ_ONCE(per_cpu_ptr(sch->pcpu, cpu)->ecaps);

	திரும்பு needed & ~ecaps;
}

/*
 * Cap semantics: which caps an action requires, and which caps a cap implies.
 * Keep all such mappings collected here.
 */

/* map @enq_flags to the SCX_CAP_* bit required for the local-DSQ insert */
நிலையான உள்வரி u64 scx_caps_for_enq(u64 enq_flags)
{
	/* a restored task must be put into the local DSQ regardless of caps */
	எனில் (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		திரும்பு 0;
	எனில் (enq_flags & SCX_ENQ_IMMED)
		திரும்பு SCX_CAP_ENQ_IMMED;
	திரும்பு SCX_CAP_ENQ;
}

/* map queued @p to the SCX_CAP_* bit required to stay on its local DSQ */
நிலையான உள்வரி u64 scx_caps_for_task(கட்டமைப்பு task_struct *p)
{
	எனில் (p->scx.flags & SCX_TASK_IMMED)
		திரும்பு SCX_CAP_ENQ_IMMED;
	திரும்பு SCX_CAP_ENQ;
}

/* the cap @sch needs to preempt @rq's current task, 0 if none */
நிலையான உள்வரி u64 scx_caps_for_preempt(கட்டமைப்பு scx_sched *sch, கட்டமைப்பு rq *rq, u64 enq_flags)
{
	கட்டமைப்பு task_struct *curr = rq->curr;

	/* a kernel-forced placement preempts regardless of caps */
	எனில் (unlikely(enq_flags & SCX_ENQ_IGNORE_CAPS))
		திரும்பு 0;
	/* a non-ext task can't be preempted by ext, own-subtree needs no cap */
	எனில் (curr->sched_class != &ext_sched_class ||
	    scx_is_descendant(scx_task_sched(curr), sch))
		திரும்பு 0;
	திரும்பு SCX_CAP_PREEMPT;
}

/* caps implied by holding @cap */
நிலையான உள்வரி u64 scx_caps_implied(u64 cap)
{
	தேர்வு (cap) {
	நிலை SCX_CAP_PREEMPT:
		திரும்பு SCX_CAP_ENQ | SCX_CAP_ENQ_IMMED;
	நிலை SCX_CAP_ENQ:
		திரும்பு SCX_CAP_ENQ_IMMED;
	}
	திரும்பு 0;
}

/* may @p keep running on @rq's cpu? requires baseline cpu access */
நிலையான உள்வரி bool scx_task_can_stay_on_cpu(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	எனில் (!scx_has_subs())
		திரும்பு true;

	/* a migration-disabled task is let in without caps, keep it likewise */
	எனில் (unlikely(is_migration_disabled(p)))
		திரும்பு true;

	திரும்பு likely(!scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE));
}

/* the task admitted for rescue on @rq, NULL if none */
நிலையான உள்வரி கட்டமைப்பு task_struct *scx_rescuee(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);

	எனில் (!scx_has_subs())
		திரும்பு வெற்று;

	திரும்பு rq->scx.rescue.curr;
}

#இல்லையெனில்	/* CONFIG_EXT_SUB_SCHED */

நிலையான உள்வரி u64 scx_missing_caps(கட்டமைப்பு scx_sched *sch, s32 cpu, u64 needed) { திரும்பு 0; }
நிலையான உள்வரி u64 scx_caps_for_preempt(கட்டமைப்பு scx_sched *sch, கட்டமைப்பு rq *rq, u64 enq_flags) { திரும்பு 0; }
நிலையான உள்வரி bool scx_task_can_stay_on_cpu(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p) { திரும்பு true; }
நிலையான உள்வரி கட்டமைப்பு task_struct *scx_rescuee(கட்டமைப்பு rq *rq) { திரும்பு வெற்று; }

#endif	/* CONFIG_EXT_SUB_SCHED */

#endif /* _KERNEL_SCHED_EXT_SUB_H */
