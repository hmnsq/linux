// SPDX-License-Identifier: GPL-2.0
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Sub-scheduler hierarchy support.
 *
 * A sub-scheduler is an scx_sched attached to a cgroup subtree under another
 * scx_sched. This file holds the sub-scheduler implementation: the scheduler
 * tree walk, capability delegation, per-shard cap state and its sync, and the
 * sub-scheduler enable/disable paths. The core dispatch/enqueue machinery it
 * builds on lives in ext.c.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#அந்தர்பாவ <linux/rhashtable.h>
#அந்தர்பாவ "internal.h"
#அந்தர்பாவ "cid.h"
#அந்தர்பாவ "arena.h"
#அந்தர்பாவ "sub.h"
#அந்தர்பாவ "inlines.h"

#ifdef CONFIG_EXT_SUB_SCHED

/*
 * On while any sub-scheduler exists so that a root-only system doesn't pay for
 * the sub-sched portions of hot paths. See scx_has_subs().
 */
DEFINE_STATIC_சாவி_FALSE(__scx_has_subs);

/* latched at root enable before any rescue runs */
நிலையான s32 scx_rescue_bw_1024;
நிலையான s64 scx_rescue_quantum_ns;
நிலையான s64 scx_rescue_sat_delta_ns;
நிலையான unsigned long scx_rescue_decay_halflife;
நிலையான unsigned long scx_rescue_overload_after;

/**
 * scx_skip_subtree_pre - Skip @pos's subtree in a pre-order walk
 * @pos: current position
 * @root: walk root
 *
 * In a walk started by scx_next_descendant_pre(), continue past @pos's subtree:
 * return @pos's next sibling, or the closest ancestor's next sibling, or NULL
 * if @pos's subtree is the last under @root. Same locking rules.
 */
கட்டமைப்பு scx_sched *scx_skip_subtree_pre(கட்டமைப்பு scx_sched *pos, கட்டமைப்பு scx_sched *வேர்)
{
	கட்டமைப்பு scx_sched *next;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_lock) ||
		       rcu_read_lock_any_held());

	போது (pos != வேர்) {
		next = list_next_or_null_rcu(&scx_parent(pos)->children, &pos->sibling,
					     கட்டமைப்பு scx_sched, sibling);
		எனில் (next)
			திரும்பு next;
		pos = scx_parent(pos);
	}
	திரும்பு வெற்று;
}

/**
 * scx_next_descendant_pre - find the next descendant for pre-order walk
 * @pos: the current position (%NULL to initiate traversal)
 * @root: sched whose descendants to walk
 *
 * To be used by scx_for_each_descendant_pre(). Find the next descendant to
 * visit for pre-order traversal of @root's descendants. @root is included in
 * the iteration and the first node to be visited.
 */
கட்டமைப்பு scx_sched *scx_next_descendant_pre(கட்டமைப்பு scx_sched *pos, கட்டமைப்பு scx_sched *வேர்)
{
	கட்டமைப்பு scx_sched *next;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_lock) ||
		       rcu_read_lock_any_held());

	/* if first iteration, visit @root */
	எனில் (!pos)
		திரும்பு வேர்;

	/* visit the first child if exists */
	next = list_first_or_null_rcu(&pos->children, கட்டமைப்பு scx_sched, sibling);
	எனில் (next)
		திரும்பு next;

	/* no child, visit my or the closest ancestor's next sibling */
	திரும்பு scx_skip_subtree_pre(pos, வேர்);
}

நிலையான கட்டமைப்பு scx_sched *scx_find_sub_sched(u64 cgroup_id)
{
	திரும்பு rhashtable_lookup(&scx_sched_hash, &cgroup_id,
				 scx_sched_hash_params);
}

வெறுமை scx_set_task_sched(கட்டமைப்பு task_struct *p, கட்டமைப்பு scx_sched *sch)
{
	rcu_assign_pointer(p->scx.sched, sch);
}

கட்டமைப்பு cgroup *sch_cgroup(கட்டமைப்பு scx_sched *sch)
{
	திரும்பு sch->cgrp;
}

/* for each descendant of @cgrp including self, set ->scx_sched to @sch */
வெறுமை set_cgroup_sched(கட்டமைப்பு cgroup *cgrp, கட்டமைப்பு scx_sched *sch)
{
	கட்டமைப்பு cgroup *pos;
	கட்டமைப்பு cgroup_subsys_state *css;

	cgroup_for_each_live_descendant_pre(pos, css, cgrp)
		rcu_assign_pointer(pos->scx_sched, sch);
}

நிலையான வெறுமை free_pshard(கட்டமைப்பு scx_pshard *pshard)
{
	கட்டமைப்பு scx_caps_updated *cu;

	எனில் (!pshard)
		திரும்பு;
	cu = &pshard->caps_updated;
	எனில் (cu->cmask_arena_out)
		scx_arena_free(pshard->sch, cu->cmask_arena_out,
			       struct_size_t(கட்டமைப்பு scx_cmask, bits,
					     SCX_CMASK_NR_சொற்கள்(pshard->nr_cids)));
	kfree(pshard);
}

வெறுமை scx_free_pshards(கட்டமைப்பு scx_sched *sch)
{
	s32 si;

	எனில் (!sch->pshard)
		திரும்பு;
	க்கு (si = 0; si < sch->nr_pshards; si++)
		free_pshard(sch->pshard[si]);
	kfree(sch->pshard);
}

நிலையான கட்டமைப்பு scx_pshard *alloc_pshard(கட்டமைப்பு scx_sched *sch, s32 shard_idx, s32 முனை)
{
	மாறிலி கட்டமைப்பு scx_cid_shard *shard =
		&rcu_dereference_protected(scx_cid_shard_ranges,
					   lockdep_is_held(&scx_enable_mutex))[shard_idx];
	size_t cmask_size = struct_size_t(கட்டமைப்பு scx_cmask, bits,
					  SCX_CMASK_NR_சொற்கள்(shard->nr_cids));
	கட்டமைப்பு scx_pshard *pshard;
	கட்டமைப்பு scx_caps_updated *cu;
	s32 i;

	pshard = kzalloc_முனை(அளவு(*pshard), GFP_KERNEL, முனை);
	எனில் (!pshard)
		திரும்பு வெற்று;

	raw_spin_lock_init(&pshard->lock);
	pshard->sch = sch;
	pshard->base = shard->base_cid;
	pshard->nr_cids = shard->nr_cids;

	க்கு (i = 0; i < __SCX_NR_CAPS; i++)
		scx_cmask_init(&pshard->caps[i].cmask, shard->base_cid, shard->nr_cids);

	cu = &pshard->caps_updated;
	raw_spin_lock_init(&cu->lock);
	INIT_LIST_HEAD(&cu->முனை_in_flight);
	__scx_cmask_init(&cu->cmask, shard->base_cid, shard->nr_cids, SCX_CID_SHARD_MAX_CPUS);

	cu->cmask_arena_out = scx_arena_alloc(sch, cmask_size);
	எனில் (!cu->cmask_arena_out) {
		free_pshard(pshard);
		திரும்பு வெற்று;
	}

	scx_cmask_init(cu->cmask_arena_out, shard->base_cid, shard->nr_cids);

	திரும்பு pshard;
}

s32 scx_alloc_pshards(கட்டமைப்பு scx_sched *sch)
{
	கட்டமைப்பு scx_pshard **pshard;
	s32 *shard_முனை;
	s32 si;

	எனில் (!sch->is_cid_type || !sch->arena_pool)
		திரும்பு 0;

	shard_முனை = rcu_dereference_protected(scx_shard_முனை,
					       lockdep_is_held(&scx_enable_mutex));

	pshard = kzalloc_objs(pshard[0], scx_nr_cid_shards);
	எனில் (!pshard)
		திரும்பு -ENOMEM;

	க்கு (si = 0; si < scx_nr_cid_shards; si++) {
		pshard[si] = alloc_pshard(sch, si, shard_முனை[si]);
		எனில் (!pshard[si]) {
			போது (--si >= 0)
				free_pshard(pshard[si]);
			kfree(pshard);
			திரும்பு -ENOMEM;
		}
	}

	sch->nr_pshards = scx_nr_cid_shards;
	/*
	 * Publish only after every entry is built so a reader observing
	 * @sch->pshard never sees a partially-filled array or unpublished cid
	 * tables. Pair the store with a barrier and an acquire load on the
	 * read side.
	 */
	smp_wmb();
	WRITE_ONCE(sch->pshard, pshard);
	திரும்பு 0;
}

/*
 * Seed the root's caps fully. Root owns all cids on all caps at enable time.
 * Children acquire caps via scx_bpf_sub_grant().
 */
வெறுமை scx_init_வேர்_caps(கட்டமைப்பு scx_sched *sch)
{
	s32 si, i;

	க்கு (si = 0; si < sch->nr_pshards; si++) {
		கட்டமைப்பு scx_pshard *ps = sch->pshard[si];

		க்கு (i = 0; i < __SCX_NR_CAPS; i++)
			scx_cmask_fill(&ps->caps[i].cmask);
	}
}

/* unserved remainder of @rq's rescuee's admitted slice, 0 once fully served */
நிலையான s64 scx_rescue_slice_remaining(கட்டமைப்பு rq *rq)
{
	s64 served = rq->scx.rescue.curr->se.sum_exec_runtime - rq->scx.rescue.exec_snap;

	திரும்பு max(rq->scx.rescue.slice - served, 0);
}

/*
 * Decay @pcpu's rescue usage average in place, halving per the knob-derived
 * halflife, see scx_rescue_set_knobs(). The timestamp advances only by whole
 * halflives.
 */
நிலையான u64 scx_rescue_decay_avg(கட்டமைப்பு scx_sched_pcpu *pcpu)
{
	unsigned long halflife = scx_rescue_decay_halflife;
	u64 n = div_u64(get_jiffies_64() - pcpu->rescue_avg_at, halflife);

	எனில் (n) {
		pcpu->rescue_avg = n < 64 ? pcpu->rescue_avg >> n : 0;
		pcpu->rescue_avg_at += n * halflife;
	}
	திரும்பு pcpu->rescue_avg;
}

/**
 * scx_rescue_charge - Charge the rescuee's runtime
 * @rq: rq the rescuee is running on
 * @delta_exec: runtime being charged
 *
 * Also ends the rescue once the admitted slice has been served in full. Ending
 * on served time rather than slice exhaustion bounds both the rescue and the
 * charging when a scheduler extends the rescuee's slice.
 */
வெறுமை scx_rescue_charge(கட்டமைப்பு rq *rq, s64 delta_exec)
{
	கட்டமைப்பு scx_sched_pcpu *pcpu;

	lockdep_assert_rq_held(rq);

	/*
	 * A rescue slice is bounded by one quantum and tick-driven expiry can
	 * overshoot by up to a tick. Clamp to avoid wild over-charges on VMs.
	 */
	delta_exec = min_t(s64, delta_exec, scx_rescue_quantum_ns + TICK_NSEC);

	rq->scx.rescue.budget -= delta_exec;

	/* per-cpu usage average feeds the overload victim pick */
	pcpu = per_cpu_ptr(scx_task_sched(rq->curr)->pcpu, cpu_of(rq));
	pcpu->rescue_avg = scx_rescue_decay_avg(pcpu) + delta_exec;

	எனில் (!scx_rescue_slice_remaining(rq))
		scx_task_slice_ended(rq, rq->scx.rescue.curr);
}

/**
 * scx_rescue_end - End the rescue execution on @rq
 * @rq: rq of interest
 *
 * When no rescuee is left pending, the session is over and the balance above
 * one quantum dies with it - it would otherwise become a banked license to
 * preempt the cid owner long after the starvation ended. While waiters remain,
 * the accrued deficit belongs to the queue and carries into the next rescue.
 */
வெறுமை scx_rescue_end(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);

	rq->scx.rescue.curr = வெற்று;
	எனில் (list_empty(&rq->scx.rescue.dsq.list))
		rq->scx.rescue.budget = min(rq->scx.rescue.budget, scx_rescue_quantum_ns);
}

/**
 * scx_rescue_keep - Keep the rescue going for a preempted-out rescuee
 * @rq: rq @p is running on
 * @p: task under rescue whose slice is exhausted
 *
 * Called from put_prev_task_scx() to decide what an exhausted slice means for
 * the rescuee. scx_rescue_charge() ends the rescue the moment the admitted
 * slice is fully served, so arriving here with the rescue still open means @p
 * was preempted. Restore the unserved remainder and return %true - @p stays the
 * rescuee and the caller reinserts it at the tail of the local DSQ, behind
 * whatever preempted the rescuee.
 *
 * Return %false to end the rescue instead - the slice is already fully served,
 * @p is leaving the rq or bypass is dismantling rescues.
 */
bool scx_rescue_keep(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	s64 remaining = scx_rescue_slice_remaining(rq);

	lockdep_assert_rq_held(rq);

	எனில் (!remaining || !(p->scx.flags & SCX_TASK_QUEUED) ||
	    scx_bypassing(scx_task_sched(p), cpu_of(rq)))
		திரும்பு false;

	scx_set_task_slice(p, remaining);
	திரும்பு true;
}

/**
 * scx_rescue_accrue - Accrue budget at the configured fraction of elapsed time
 * @rq: rq of interest
 *
 * A session spans from the first arrival until no rescuee is left, pending or
 * admitted. While one is active the cap is three quanta and the balance drives
 * escalation, see scx_rescue_timerfn(). Outside a session the cap is one
 * quantum, so an idle gap funds the next arrival's admission but never an
 * escalation.
 */
நிலையான வெறுமை scx_rescue_accrue(கட்டமைப்பு rq *rq)
{
	bool in_session = rq->scx.rescue.curr || !list_empty(&rq->scx.rescue.dsq.list);
	s64 cap = in_session ? 3 * scx_rescue_quantum_ns : scx_rescue_quantum_ns;
	s64 delta;
	u64 now;

	lockdep_assert_rq_held(rq);

	/* not every path here holds an updated rq clock, use __scx_bpf_now() */
	now = __scx_bpf_now(rq);
	delta = now - rq->scx.rescue.clock;
	rq->scx.rescue.clock = now;

	/*
	 * Avoid multiplication overflows by taking a shortcut when the gap is
	 * large enough to fill the budget.
	 */
	எனில் (delta >= scx_rescue_sat_delta_ns)
		rq->scx.rescue.budget = cap;
	இல்லையெனில்
		rq->scx.rescue.budget =
			min(cap, rq->scx.rescue.budget +
			    ((delta * scx_rescue_bw_1024) >> SCHED_CAPACITY_SHIFT));
}

/*
 * The slice for the next admission - the quantum divided across the stranded
 * tasks so that a crowded queue round-robins on shorter slices.
 */
நிலையான s64 scx_rescue_next_slice(கட்டமைப்பு rq *rq)
{
	s64 min_slice = max_t(s64, SCX_RESCUE_MIN_SLICE_US * NSEC_PER_USEC, TICK_NSEC);
	u32 depth = rq->scx.rescue.dsq.nr ?: 1;

	திரும்பு clamp(div_s64(scx_rescue_quantum_ns, depth), min_slice, scx_rescue_quantum_ns);
}

நிலையான வெறுமை scx_rescue_timer_arm(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு timer_list *timer = &rq->scx.rescue.timer;
	s64 delay = scx_rescue_quantum_ns / 4;	/* should be granular enough */

	எனில் (timer_pending(timer))
		திரும்பு;

	/*
	 * While the head waiter can't be admitted because the bucket is short
	 * of a full quantum, stretch to the full funding delay.
	 */
	எனில் (!rq->scx.rescue.curr && rq->scx.rescue.budget < scx_rescue_quantum_ns) {
		s64 deficit = scx_rescue_quantum_ns - rq->scx.rescue.budget;

		delay = max(delay,
			    div_s64(deficit << SCHED_CAPACITY_SHIFT, scx_rescue_bw_1024));
	}

	/* +1 rounds up so the beat is due by the time the timer fires */
	timer->expires = jiffies + nsecs_to_jiffies(delay) + 1;
	add_timer_on(timer, cpu_of(rq));
}

/**
 * scx_rescue_admit - Start rescuing @p on @rq
 * @rq: rq @p is being admitted on
 * @p: task being admitted, off any DSQ
 * @slice: CPU time to grant
 *
 * The schedulers keep their normal control over @p and may preempt or reslice
 * it. @slice is measured on served CPU time against the snapshot taken here, so
 * neither shortens the rescue, see scx_rescue_charge() and scx_rescue_keep().
 * Prolonged denial escalates into protected execution, see
 * scx_rescue_timerfn().
 */
நிலையான வெறுமை scx_rescue_admit(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, s64 slice)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->scx.rescue.curr);

	rq->scx.rescue.curr = p;
	rq->scx.rescue.slice = slice;
	rq->scx.rescue.exec_snap = p->se.sum_exec_runtime;
	scx_set_task_slice(p, slice);
	scx_rescue_timer_arm(rq);
}

/**
 * scx_rescue_try_admit - Try to admit a freshly stranded task
 * @rq: rq @p is being inserted on
 * @p: stranded task being diverted to rescue
 *
 * One rescue at a time and earlier arrivals go first. Admission needs a full
 * quantum of budget, spent as the rescue runs. Return %true if @p was admitted
 * and should be inserted at the tail of @rq's local DSQ, %false if it has to
 * park on the rescue DSQ, with the timer armed to admit it later.
 */
நிலையான bool scx_rescue_try_admit(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	scx_rescue_accrue(rq);

	எனில் (!rq->scx.rescue.curr && list_empty(&rq->scx.rescue.dsq.list) &&
	    rq->scx.rescue.budget >= scx_rescue_quantum_ns) {
		scx_rescue_admit(rq, p, scx_rescue_quantum_ns);
		திரும்பு true;
	}

	scx_rescue_timer_arm(rq);
	திரும்பு false;
}

/**
 * scx_rescue_check_overload - Eject the top rescue consumer on a stuck rescue
 * @rq: rq whose rescue timer fired
 *
 * If the oldest waiter on @rq's rescue DSQ has been queued for too long, rescue
 * demand on this cpu persistently exceeds the configured bandwidth. Eject the
 * sub with the highest recent rescue consumption instead of letting the
 * scheduler stall path blame the waiter's owner, who may just be crowded out.
 */
நிலையான வெறுமை scx_rescue_check_overload(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு scx_sched *victim = வெற்று, *pos;
	கட்டமைப்பு task_struct *p;
	முழு cpu = cpu_of(rq);
	u64 max_avg = 0;
	u32 dur_ms;

	lockdep_assert_rq_held(rq);

	p = list_first_entry_or_null(&rq->scx.rescue.dsq.list, கட்டமைப்பு task_struct,
				     scx.dsq_list.முனை);
	எனில் (!p)
		திரும்பு;

	/* has the head waiter been queued for longer than the threshold? */
	எனில் (time_before(jiffies, p->scx.rescue_at + scx_rescue_overload_after))
		திரும்பு;

	/*
	 * Grace period after the last ejection on this cpu - the freed
	 * bandwidth gets one threshold's worth of time to drain the backlog
	 * before another sub is judged.
	 */
	எனில் (time_before64(get_jiffies_64(), rq->scx.rescue.kill_at +
			  scx_rescue_overload_after))
		திரும்பு;

	list_for_each_entry_rcu(pos, &scx_sched_all, all) {
		u64 avg = scx_rescue_decay_avg(per_cpu_ptr(pos->pcpu, cpu));

		/* skip an already-exiting sub, else the ejection is wasted */
		எனில் (pos->level && avg > max_avg &&
		    atomic_read(&pos->exit_kind) == SCX_EXIT_NONE) {
			max_avg = avg;
			victim = pos;
		}
	}
	எனில் (!victim)
		திரும்பு;

	rq->scx.rescue.kill_at = get_jiffies_64();
	dur_ms = jiffies_to_msecs(jiffies - p->scx.rescue_at);
	__scx_exit(victim, SCX_EXIT_ERROR_RESCUE, 0, cpu,
		   "used too much rescue CPU time (%llums) while %s[%d] waited %u.%03us to be rescued",
		   div_u64(max_avg, NSEC_PER_MSEC), p->comm, p->pid, dur_ms / 1000,
		   dur_ms % 1000);
}

/**
 * scx_rescue_timerfn - Drive and pace rescue execution
 * @timer: rq->scx.rescue.timer
 *
 * Runs every quarter quantum while a rescuee exists, pending or admitted, see
 * scx_rescue_timer_arm(). The head waiter is admitted once the bucket holds a
 * full quantum and granted its slice, see scx_rescue_next_slice(). A session
 * whose budget accumulates over two quanta with the admitted rescuee still
 * waiting escalates - the rescuee's remaining slice turns into protected
 * execution and it preempts the current task. An overloaded rescue queue ejects
 * the top consumer, see scx_rescue_check_overload().
 */
நிலையான வெறுமை scx_rescue_timerfn(கட்டமைப்பு timer_list *timer)
{
	கட்டமைப்பு rq *rq = timer_container_of(rq, timer, scx.rescue.timer);
	கட்டமைப்பு task_struct *p;

	guard(rq_lock_irqsave)(rq);

	p = rq->scx.rescue.curr;
	எனில் (!p && list_empty(&rq->scx.rescue.dsq.list))
		திரும்பு;

	scx_rescue_accrue(rq);
	scx_rescue_check_overload(rq);

	எனில் (!p) {
		s64 slice = scx_rescue_next_slice(rq);

		/* no rescue in progress */
		எனில் (rq->scx.rescue.budget < scx_rescue_quantum_ns)
			செல் out_arm;

		/* there's enough budget to start rescuing the next one */
		p = list_first_entry(&rq->scx.rescue.dsq.list, கட்டமைப்பு task_struct,
				     scx.dsq_list.முனை);
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_rescue_admit(rq, p, slice);
		scx_move_local_task_to_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
		எனில் (sched_class_above(&ext_sched_class, rq->curr->sched_class))
			resched_curr(rq);
	} இல்லையெனில் எனில் (p->scx.dsq && rq->scx.rescue.budget > 2 * scx_rescue_quantum_ns) {
		/*
		 * The rescuee waited for the CPU for too long. Escalate - grant
		 * the unserved remainder, protect it from the schedulers and
		 * preempt the current task. The slice is set before the
		 * protection. Repeat beats only repeat the head move - the
		 * slice write is refused on a protected task.
		 */
		scx_set_task_slice(p, scx_rescue_slice_remaining(rq));
		p->scx.flags |= SCX_TASK_PROTECTED;
		scx_task_unlink_from_dsq(p, &rq->scx.local_dsq);
		scx_move_local_task_to_local_dsq(scx_task_sched(p), p,
					SCX_ENQ_HEAD | SCX_ENQ_PREEMPT | SCX_ENQ_IGNORE_CAPS,
					rq);
	}
out_arm:
	scx_rescue_timer_arm(rq);
}

/* flush out tasks waiting for rescue before a CPU goes down */
வெறுமை scx_rescue_flush(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	/* end the current rescue */
	எனில் (rq->scx.rescue.curr)
		scx_task_slice_ended(rq, rq->scx.rescue.curr);

	/* and flush out all pending ones */
	list_for_each_entry_safe(p, n, &rq->scx.rescue.dsq.list, scx.dsq_list.முனை) {
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_move_local_task_to_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
	}

	timer_delete(&rq->scx.rescue.timer);
}

வெறுமை scx_rescue_dump(கட்டமைப்பு seq_buf *s, கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு task_struct *p = rq->scx.rescue.curr;

	scx_dump_வரி(s, "          rescue=%u budget=%lldus rescuing=%s[%d]",
		      rq->scx.rescue.dsq.nr,
		      div_s64(rq->scx.rescue.budget, NSEC_PER_USEC),
		      p ? p->comm : "none", p ? p->pid : -1);
}

/*
 * A scheduler whose stall watchdog is shorter than the overload threshold gets
 * stall-killed over its parked waiters before the overload check can eject the
 * actual top consumer. The root's knobs set the threshold, warn on any
 * scheduler that doesn't fit it.
 */
நிலையான வெறுமை scx_rescue_check_timeout(கட்டமைப்பு scx_sched *sch)
{
	எனில் (!scx_rescue_bw_1024 || sch->watchdog_timeout > scx_rescue_overload_after)
		திரும்பு;

	pr_warn("sched_ext: %s: watchdog timeout %ums <= rescue overload threshold %ums\n",
		sch->ops.name, jiffies_to_msecs(sch->watchdog_timeout),
		jiffies_to_msecs(scx_rescue_overload_after));
}

/* latch the rescue parameters on root scheduler enable */
வெறுமை scx_rescue_set_knobs(கட்டமைப்பு scx_sched *sch)
{
	s32 bw_ppt = sch->ops.rescue_bandwidth_ppt ?: SCX_RESCUE_DFL_BW_PPT;
	s64 quantum_us = sch->ops.rescue_quantum_us ?: SCX_RESCUE_DFL_QUANTUM_US;
	s64 period_ns;

	எனில் (sch->ops.rescue_bandwidth_ppt == SCX_RESCUE_DISABLE) {
		scx_rescue_bw_1024 = 0;
		திரும்பு;
	}

	scx_rescue_bw_1024 = bw_ppt * SCHED_CAPACITY_SCALE / 1000;
	scx_rescue_quantum_ns = max(quantum_us * NSEC_PER_USEC, TICK_NSEC);
	scx_rescue_sat_delta_ns =
		div_s64((4 * scx_rescue_quantum_ns + TICK_NSEC) << SCHED_CAPACITY_SHIFT,
			scx_rescue_bw_1024);

	/*
	 * The overload threshold and the decay halflife scale with the funding
	 * period - the time the bucket takes to fund one full quantum.
	 */
	period_ns = div_s64(scx_rescue_quantum_ns << SCHED_CAPACITY_SHIFT, scx_rescue_bw_1024);
	scx_rescue_overload_after =
		clamp(nsecs_to_jiffies(SCX_RESCUE_OVERLOAD_MULT * period_ns),
		      msecs_to_jiffies(SCX_RESCUE_MIN_OVERLOAD_MS),
		      msecs_to_jiffies(SCX_RESCUE_MAX_OVERLOAD_MS));
	scx_rescue_decay_halflife = scx_rescue_overload_after / 4;

	/* a single in-budget wait must not cross the overload trigger */
	எனில் (nsecs_to_jiffies(period_ns) > scx_rescue_overload_after / 2)
		pr_warn("sched_ext: %s: rescue funding period %lldms > overload threshold %ums / 2\n",
			sch->ops.name, div_s64(period_ns, NSEC_PER_MSEC),
			jiffies_to_msecs(scx_rescue_overload_after));

	scx_rescue_check_timeout(sch);
}

வெறுமை scx_rescue_init(கட்டமைப்பு rq *rq)
{
	BUG_ON(scx_init_dsq(&rq->scx.rescue.dsq, SCX_DSQ_RESCUE, வெற்று));
	timer_setup(&rq->scx.rescue.timer, scx_rescue_timerfn, TIMER_PINNED);
	rq->scx.rescue.kill_at = get_jiffies_64();
}

/**
 * scx_resolve_local_dsq - Pick the local, rescue or reject DSQ for an insert
 * @sch: enqueuing sub-sched
 * @rq: rq whose local DSQ @p targets
 * @p: task being inserted
 * @enq_flags: in/out, unhonored flags are cleared
 *
 * Return @rq's local DSQ if @sch holds the required caps on @rq's cid.
 * Otherwise, return @rq's rescue DSQ if the insert carries %SCX_ENQ_RESCUE and
 * rescue is enabled, or @rq's reject DSQ after recording the reenq reason on
 * @p.
 *
 * %SCX_ENQ_IMMED, %SCX_ENQ_PREEMPT and %SCX_ENQ_HEAD are cleared when diverting
 * to rescue or reject. %SCX_ENQ_PREEMPT is also cleared on a fallback
 * migration-disabled admission.
 *
 * Bypass doesn't need special-casing as a bypassing sched's tasks are enqueued
 * to and run by its nearest non-bypassing ancestor. If root is bypassing, it
 * always holds all caps.
 */
கட்டமைப்பு scx_dispatch_q *scx_resolve_local_dsq(கட்டமைப்பு scx_sched *sch, கட்டமைப்பு rq *rq,
					     கட்டமைப்பு task_struct *p, u64 *enq_flags)
{
	எனில் (!scx_has_subs())
		திரும்பு &rq->scx.local_dsq;

	s32 cid = __scx_cpu_to_cid(cpu_of(rq));
	கட்டமைப்பு scx_sched *asch = rq->scx.remote_activate_sch ?: sch;
	u64 needed = scx_caps_for_enq(*enq_flags);
	u64 missing;

	/*
	 * On a remote activation the scheduling sched (@asch) differs from
	 * @p's owner (@sch). Check caps against the scheduling sched.
	 */
	எனில் (*enq_flags & SCX_ENQ_PREEMPT)
		needed |= scx_caps_for_preempt(asch, rq, *enq_flags);
	missing = scx_missing_caps(asch, cpu_of(rq), needed);

	/* requirements met */
	எனில் (likely(!missing))
		திரும்பு &rq->scx.local_dsq;

	/*
	 * The task must run on this CPU regardless of caps: the rq is draining
	 * offline (BPF scheduler bypassed), the task is migration-disabled, or a
	 * migration is pending. Admit despite the missing caps and count it.
	 * Refuse preemptions.
	 */
	எனில் (unlikely(!scx_rq_online(rq) || is_migration_disabled(p) ||
		     p->migration_pending)) {
		__scx_add_event(sch, SCX_EV_SUB_FORCED_ADMIT, 1);
		*enq_flags &= ~SCX_ENQ_PREEMPT;
		திரும்பு &rq->scx.local_dsq;
	}

	/*
	 * Diverting to rescue or reject, neither of which honors IMMED, PREEMPT
	 * or HEAD - a diversion has no priority and IMMED is not allowed on
	 * non-local DSQs. Strip the enq and task flags along with the slice.
	 */
	*enq_flags &= ~(SCX_ENQ_IMMED | SCX_ENQ_PREEMPT | SCX_ENQ_HEAD |
			SCX_ENQ_APPLY_SLICE | SCX_ENQ_SLICE_DFL);
	p->scx.flags &= ~SCX_TASK_IMMED;

	/* the enqueuer opted for rescue instead of rejection and reenqueue */
	எனில் ((*enq_flags & SCX_ENQ_RESCUE) && likely(scx_rescue_bw_1024)) {
		__scx_add_event(sch, SCX_EV_SUB_RESCUE, 1);
		எனில் (scx_rescue_try_admit(rq, p))
			திரும்பு &rq->scx.local_dsq;

		/* queueing, the overload trigger measures the wait from here */
		p->scx.rescue_at = jiffies;
		திரும்பு &rq->scx.rescue.dsq;
	}

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = cid;

	திரும்பு &rq->scx.reject_dsq;
}

/* @p lost the caps needed to stay on @rq's local DSQ? Record reason if so. */
bool scx_task_reenq_on_cap_revoke(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	u64 missing;

	/* migration-disabled tasks and the rescuee are admitted capless */
	எனில் (is_migration_disabled(p) || p == scx_rescuee(rq))
		திரும்பு false;

	missing = scx_missing_caps(scx_task_sched(p), cpu_of(rq), scx_caps_for_task(p));
	எனில் (likely(!missing))
		திரும்பு false;

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = __scx_cpu_to_cid(cpu_of(rq));
	திரும்பு true;
}

/*
 * Drain @rq->scx.reject_dsq, reenqueueing each task so the BPF re-decides
 * from p->scx.reenq_reason_*.
 *
 * A task can be re-rejected repeatedly. The reenqueue is bounded per task in
 * scx_do_enqueue_task(), which ejects the owning sub past SCX_REENQ_MAX_REPEAT.
 * Rejection can't happen for root.
 */
வெறுமை scx_reenq_reject(கட்டமைப்பு rq *rq)
{
	LIST_HEAD(tasks);
	கட்டமைப்பு task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	எனில் (!scx_has_subs() || list_empty(&rq->scx.reject_dsq.list))
		திரும்பு;

	/*
	 * Move to a private list so a task re-rejected by the
	 * scx_do_enqueue_task() below isn't revisited this round.
	 */
	list_for_each_entry_safe(p, n, &rq->scx.reject_dsq.list, scx.dsq_list.முனை) {
		/* migration_pending tasks should have bypassed to local DSQ */
		எனில் (WARN_ON_ONCE(p->migration_pending))
			தொடர்;

		scx_wait_dispatching(p);
		scx_dispatch_dequeue(rq, p);

		எனில் (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_MASK))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
		p->scx.flags |= SCX_TASK_REENQ_CAP;

		list_add_tail(&p->scx.dsq_list.முனை, &tasks);
	}

	list_for_each_entry_safe(p, n, &tasks, scx.dsq_list.முனை) {
		list_del_init(&p->scx.dsq_list.முனை);

		scx_do_enqueue_task(rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
	}
}

/* record a caps change, see struct scx_caps_updated */
நிலையான வெறுமை caps_updated_record(கட்டமைப்பு scx_pshard *ps, மாறிலி கட்டமைப்பு scx_cmask *cids, u64 caps,
				கட்டமைப்பு list_head *to_deliver)
{
	கட்டமைப்பு scx_caps_updated *cu = &ps->caps_updated;

	guard(raw_spinlock)(&cu->lock);
	scx_cmask_or(&cu->cmask, cids);
	cu->caps |= caps;
	எனில் (list_empty(&cu->முனை_in_flight))
		list_add_tail(&cu->முனை_in_flight, to_deliver);
}

/* deliver queued caps_updated callbacks, see struct scx_caps_updated */
நிலையான வெறுமை caps_updated_deliver(கட்டமைப்பு list_head *to_deliver)
{
	கட்டமைப்பு scx_caps_updated *cu, *tmp;

	list_for_each_entry_safe(cu, tmp, to_deliver, முனை_in_flight) {
		கட்டமைப்பு scx_pshard *ps = container_of(cu, கட்டமைப்பு scx_pshard, caps_updated);
		கட்டமைப்பு scx_sched *sch = ps->sch;

		போது (true) {
			u64 caps = 0;

			/*
			 * During enable, has_op is set after ops.sub_attach(),
			 * so !has_op means the op is absent or the sched isn't
			 * live yet - e.g. caps grant from ops.sub_attach().
			 * Either way don't consume - leave for
			 * scx_sub_seed_caps() to deliver once live.
			 */
			scoped_guard (raw_spinlock, &cu->lock) {
				எனில் (cu->caps && SCX_HAS_OP(sch, sub_caps_updated) &&
				    likely(!READ_ONCE(sch->aborting))) {
					கட்டமைப்பு scx_cmask_ref ref;

					caps = cu->caps;
					scx_cmask_ref_init_kern(sch, cu->cmask_arena_out,
								ps->base, ps->nr_cids, &ref);
					scx_cmask_ref_copy(&ref, &cu->cmask);
					scx_cmask_clear(&cu->cmask);
					cu->caps = 0;
				} இல்லையெனில் {
					list_del_init(&cu->முனை_in_flight);
				}
			}
			எனில் (!caps)
				நிறுத்து;

			/* caps != 0 only when deliverable (has_op, above) */
			SCX_CALL_OP(sch, sub_caps_updated, வெற்று, cu->cmask_arena_out, caps);
		}
	}
}

/*
 * Deliver caps owed to @sch that couldn't be delivered earlier (e.g. a grant
 * taken during its sub_attach(), before has_op was set). Called once @sch is
 * enabled.
 */
நிலையான வெறுமை scx_sub_seed_caps(கட்டமைப்பு scx_sched *sch)
{
	LIST_HEAD(to_deliver);
	s32 si;

	guard(irqsave)();

	க்கு (si = 0; si < sch->nr_pshards; si++) {
		கட்டமைப்பு scx_pshard *ps = sch->pshard[si];
		கட்டமைப்பு scx_caps_updated *cu = &ps->caps_updated;

		scoped_guard (raw_spinlock, &cu->lock) {
			எனில் (cu->caps && list_empty(&cu->முனை_in_flight))
				list_add_tail(&cu->முனை_in_flight, &to_deliver);
		}
	}
	caps_updated_deliver(&to_deliver);
}

நிலையான u64 calc_effective_caps(கட்டமைப்பு scx_pshard *ps, s32 cid)
{
	u64 ecaps = 0;
	u32 cap_bit;

	க்கு (cap_bit = 0; cap_bit < __SCX_NR_CAPS; cap_bit++)
		எனில் (scx_cmask_test(cid, &ps->caps[cap_bit].cmask))
			ecaps |= BIT_U64(cap_bit) | scx_caps_implied(BIT_U64(cap_bit));
	திரும்பு ecaps;
}

/**
 * queue_sync_ecaps - Queue ecaps update for a (sch, cid) pair
 * @sch: sched to update
 * @cid: cid to update
 *
 * Queue an ecaps update for @sch's @cid and kick the cpu so that it syncs in
 * dispatch_one().
 */
நிலையான வெறுமை queue_sync_ecaps(கட்டமைப்பு scx_sched *sch, s32 cid)
{
	s32 cpu = __scx_cid_to_cpu(cid);
	கட்டமைப்பு scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);

	/*
	 * Pairs with smp_mb() in scx_process_sync_ecaps(). Either the check
	 * below sees the node off the list and queues it, or the in-flight sync
	 * sees the caps[] update made before this call.
	 */
	smp_mb();

	/* @cid's pshard->lock excludes concurrent queueing attempts */
	எனில் (llist_on_list(&pcpu->ecaps_to_sync_முனை))
		திரும்பு;
	எனில் (llist_add(&pcpu->ecaps_to_sync_முனை, &cpu_rq(cpu)->scx.ecaps_to_sync))
		scx_kick_cpu(sch->ancestors[0], cpu, 0);
}

/* discard @rq's queued ecaps syncs */
நிலையான வெறுமை discard_queued_syncs(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு llist_முனை *pos, *tmp;

	lockdep_assert_rq_held(rq);

	llist_for_each_safe(pos, tmp, llist_del_all(&rq->scx.ecaps_to_sync))
		init_llist_முனை(pos);
}

/**
 * scx_process_sync_ecaps - Sync this cpu's ecaps to pshard->caps[]
 * @rq: the cid's cpu rq
 * @prev: @rq's previous task from the in-progress dispatch
 *
 * pshard->caps[] is the target configuration. pcpu->ecaps is the effective
 * transposed copy owned by the cid's cpu and written only here under @rq's
 * lock.
 *
 * A sched that newly gains baseline access here is owed an update_idle() so it
 * learns the cid's idle state. Such a gain arms the per-rq
 * %SCX_RQ_SUB_IDLE_RENOTIFY gate so the next idle pick delivers it.
 */
வெறுமை scx_process_sync_ecaps(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev)
{
	s32 cpu = cpu_of(rq);
	s32 cid, shard;
	கட்டமைப்பு llist_முனை *batch, *pos, *tmp;
	u64 lost_all = 0;

	lockdep_assert_rq_held(rq);

	எனில் (!scx_has_subs() || likely(llist_empty(&rq->scx.ecaps_to_sync)))
		திரும்பு;

	/*
	 * ecaps are zeroed while the cpu is inactive and must stay zero.
	 * Discard queued syncs instead of processing them - the
	 * scx_online_ecaps() reseed re-syncs every sched on activation.
	 * cpu_active() clears before the offline zeroing and sets before the
	 * reseed is queued, so this test can neither miss a racing sync nor
	 * eat the reseed.
	 */
	எனில் (unlikely(!cpu_active(cpu))) {
		discard_queued_syncs(rq);
		திரும்பு;
	}

	/* @cid is valid here: the cpu is active with queued syncs */
	cid = __scx_cpu_to_cid(cpu);
	shard = rcu_dereference_all(scx_cid_to_shard)[cid];

	batch = llist_del_all(&rq->scx.ecaps_to_sync);
	llist_for_each_safe(pos, tmp, batch) {
		கட்டமைப்பு scx_sched_pcpu *pcpu =
			container_of(pos, கட்டமைப்பு scx_sched_pcpu, ecaps_to_sync_முனை);
		கட்டமைப்பு scx_pshard *ps = pcpu->sch->pshard[shard];
		u64 old, ecaps, lost, gained;

		init_llist_முனை(pos);

		/* pairs with smp_mb() in queue_sync_ecaps(), see there */
		smp_mb();

		old = READ_ONCE(pcpu->ecaps);
		ecaps = calc_effective_caps(ps, cid);
		WRITE_ONCE(pcpu->ecaps, ecaps);

		lost = old & ~ecaps;
		gained = ecaps & ~old;
		lost_all |= lost;

		/*
		 * Tell the sched its effective caps on this cid changed. The
		 * invocation is equivalent to the dispatch path and may drop
		 * and re-acquire the rq lock temporarily while the rest of
		 * @batch is held privately, see scx_discard_ecaps_to_sync().
		 * The dispatch kfuncs resolve their context on the executing
		 * cpu, which under core scheduling can differ from @rq's cpu,
		 * so the context is set up there. The rq recorded in it keeps
		 * the dispatches targeting @rq.
		 */
		எனில் (ecaps != pcpu->reported_ecaps &&
		    SCX_HAS_OP(pcpu->sch, sub_ecaps_updated) &&
		    !scx_bypassing(pcpu->sch, cpu)) {
			கட்டமைப்பு scx_dsp_ctx *dspc = &this_cpu_ptr(pcpu->sch->pcpu)->dsp_ctx;

			dspc->rq = rq;
			/* stash @prev so nested dispatches can access it */
			rq->scx.sub_dispatch_prev = prev;
			SCX_CALL_OP(pcpu->sch, sub_ecaps_updated, rq, scx_cpu_arg(cpu),
				    pcpu->reported_ecaps, ecaps);
			rq->scx.sub_dispatch_prev = வெற்று;
			scx_flush_dispatch_buf(pcpu->sch, rq);
			pcpu->reported_ecaps = ecaps;
		}

		/*
		 * Gaining baseline access owes an update_idle() so the sched
		 * learns the cpu's idle state. Arm the per-rq gate so the next
		 * idle pick flushes it. Losing access drops any pending notify.
		 */
		எனில் (gained & SCX_CAP_BASE) {
			pcpu->idle_renotify = true;
			rq->scx.flags |= SCX_RQ_SUB_IDLE_RENOTIFY;
		} இல்லையெனில் எனில் (lost & SCX_CAP_BASE) {
			pcpu->idle_renotify = false;
		}
	}

	/*
	 * Losing a cap can strand already-queued tasks. Schedule a reenq scan
	 * to move the now-capless ones off the local DSQ. The scan tests
	 * against the effective caps and thus must come after the ecaps sync.
	 */
	எனில் (lost_all & SCX_CAPS_REENQ_ON_LOSS)
		scx_schedule_reenq_local(rq, SCX_REENQ_CAP_REVOKE);
}

/**
 * scx_unbypass_replay_ecaps - Replay a bypass-suppressed ecaps notification
 * @rq: rq of the cpu leaving bypass
 * @sch: scheduler that just left bypass on @rq's cpu
 *
 * scx_process_sync_ecaps() consumes syncs while bypassing without delivering
 * ops.sub_ecaps_updated(), leaving reported_ecaps stale. Nothing re-queues a
 * sync when bypass lifts, so without a replay a cid that never changes again
 * would never be notified. The attach-time initial grants are the acute case
 * as they are consumed during the enable bypass window. Re-queue a sync for
 * any undelivered delta so the next dispatch delivers it.
 */
வெறுமை scx_unbypass_replay_ecaps(கட்டமைப்பு rq *rq, கட்டமைப்பு scx_sched *sch)
{
	s32 cpu = cpu_of(rq);
	கட்டமைப்பு scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);
	கட்டமைப்பு scx_pshard *ps;
	s32 cid;

	lockdep_assert_rq_held(rq);

	/* root holds every cap and never uses ecaps */
	எனில் (!sch->level)
		திரும்பு;

	எனில் (READ_ONCE(pcpu->ecaps) == pcpu->reported_ecaps)
		திரும்பு;

	cid = __scx_cpu_to_cid(cpu);
	ps = sch->pshard[rcu_dereference_all(scx_cid_to_shard)[cid]];

	guard(raw_spinlock)(&ps->lock);
	queue_sync_ecaps(sch, cid);
}

/*
 * A cpu came back. Re-seed each sub-sched's ecaps on the cpu's cid. The sync
 * recomputes effective caps from the pshard and fires ops.sub_ecaps_updated()
 * only on a real change since offline.
 */
வெறுமை scx_online_ecaps(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு scx_sched *வேர், *pos;
	s32 cid, shard;

	/*
	 * Only a live hierarchy can have ecaps to reseed. This also keeps the
	 * table reads below away from an enable that failed before publishing
	 * the tables. A concurrent disable can't retire them, see
	 * handle_hotplug().
	 */
	எனில் (!scx_enabled())
		திரும்பு;

	guard(rq_lock_irqsave)(rq);

	வேர் = scx_வேர்_protected();
	cid = __scx_cpu_to_cid(cpu_of(rq));
	shard = rcu_dereference_all(scx_cid_to_shard)[cid];

	scx_for_each_descendant_pre(pos, வேர்) {
		கட்டமைப்பு scx_pshard *ps;

		/* root holds every cap and never uses ecaps */
		எனில் (!pos->level)
			தொடர்;

		ps = pos->pshard[shard];
		guard(raw_spinlock)(&ps->lock);
		queue_sync_ecaps(pos, cid);
	}
}

/*
 * A cpu is going down. Zero each sub-sched's in-effect ecaps so cap checks
 * treat the cpu as capless while offline. Pending and late-queued syncs are
 * discarded at consumption by scx_process_sync_ecaps() while the cpu is
 * inactive. Leave reported_ecaps. Ownership is unchanged, so the
 * scx_online_ecaps() reseed reports only a genuine delta. No callback fires
 * here.
 */
வெறுமை scx_offline_ecaps(கட்டமைப்பு rq *rq)
{
	s32 cpu = cpu_of(rq);
	கட்டமைப்பு scx_sched *வேர், *pos;

	guard(rq_lock_irqsave)(rq);

	வேர் = scx_வேர்_protected();

	scx_for_each_descendant_pre(pos, வேர்) {
		/* root holds every cap and never uses ecaps */
		எனில் (!pos->level)
			தொடர்;

		WRITE_ONCE(per_cpu_ptr(pos->pcpu, cpu)->ecaps, 0);
	}
}

/*
 * @pcpu's sched was unhashed before the grace period, so nothing re-queues its
 * sync node. Remove the node from @rq's pending list so the pcpu can be freed.
 */
வெறுமை scx_discard_ecaps_to_sync(s32 cpu, கட்டமைப்பு scx_sched_pcpu *pcpu)
{
	கட்டமைப்பு rq *rq = cpu_rq(cpu);
	கட்டமைப்பு llist_முனை *head = வெற்று, *tail = வெற்று;
	கட்டமைப்பு llist_முனை *pos, *tmp;

	/*
	 * llist can't unlink a single node. Take all queued nodes, drop @pcpu's
	 * and resplice the rest. Nodes in the taken batch read as on-list
	 * throughout, so queue_sync_ecaps() stays correct.
	 */
	எனில் (llist_on_list(&pcpu->ecaps_to_sync_முனை)) {
		scoped_guard (rq_lock_irqsave, rq) {
			llist_for_each_safe(pos, tmp, llist_del_all(&rq->scx.ecaps_to_sync)) {
				எனில் (pos == &pcpu->ecaps_to_sync_முனை) {
					init_llist_முனை(pos);
				} இல்லையெனில் {
					pos->next = head;
					head = pos;
					எனில் (!tail)
						tail = pos;
				}
			}
			எனில் (head)
				llist_add_batch(head, tail, &rq->scx.ecaps_to_sync);
		}
	}

	/*
	 * An in-flight scx_process_sync_ecaps() batch may still hold the node
	 * privately across dispatch-induced rq unlocks, reading as on-list.
	 *
	 * Because a bypassing sched gets no op call, init_llist_node() and all
	 * @pcpu accesses share one contiguous lock hold, off-list under the rq
	 * lock means @pcpu won't be accessed again.
	 */
	போது (true) {
		scoped_guard (rq_lock_irqsave, rq) {
			எனில் (!llist_on_list(&pcpu->ecaps_to_sync_முனை))
				திரும்பு;
		}
		cpu_relax();
	}
}

/**
 * scx_discard_stale_ecaps_syncs - Discard ecaps syncs from earlier schedulers
 *
 * To be called during root enable before the scheduler goes live. An earlier
 * root's sub-sched may not have gone through its RCU free path yet (e.g. a
 * still-open link fd defers it) and can leave queued ecaps syncs behind.
 * Processing them would decode the dead sched's pshards with the current cid
 * layout. Discard them instead. The backing scx_sched_pcpu's are still
 * allocated as the free path removes ecaps_to_sync_node before freeing.
 */
வெறுமை scx_discard_stale_ecaps_syncs(வெறுமை)
{
	s32 cpu;

	for_each_possible_cpu(cpu) {
		கட்டமைப்பு rq *rq = cpu_rq(cpu);

		guard(rq_lock_irqsave)(rq);
		discard_queued_syncs(rq);
	}
}

நிலையான DECLARE_WAIT_QUEUE_HEAD(scx_unlink_waitq);

வெறுமை drain_descendants(கட்டமைப்பு scx_sched *sch)
{
	/*
	 * Child scheds that finished the critical part of disabling will take
	 * themselves off @sch->children. Wait for it to drain. As propagation
	 * is recursive, empty @sch->children means that all proper descendant
	 * scheds reached unlinking stage.
	 */
	wait_event(scx_unlink_waitq, list_empty(&sch->children));
}

/**
 * scx_rehome_task - Move a task to a sched it has been initialized for
 * @to: sched taking over @p, @p's init on it already complete
 * @p: task to re-home
 *
 * Exit @p from its current sched and switch it over to @to, overriding the
 * state to %SCX_TASK_READY to account for the already completed init. A task
 * on a non-ext class, possible under an %SCX_OPS_SWITCH_PARTIAL root, stays
 * %READY and is enabled by switching_to_scx() if it switches over.
 */
நிலையான வெறுமை scx_rehome_task(கட்டமைப்பு scx_sched *to, கட்டமைப்பு task_struct *p)
{
	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(task_rq(p));

	scoped_guard (sched_change, p, DEQUEUE_SAVE | DEQUEUE_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_set_task_state(p, SCX_TASK_INIT_BEGIN);
		scx_set_task_state(p, SCX_TASK_INIT);
		scx_set_task_sched(p, to);
		scx_set_task_state(p, SCX_TASK_READY);
		எனில் (p->sched_class == &ext_sched_class)
			scx_enable_task(to, p);
	}
}

/**
 * scx_punt_task - Hand a task to a failed sched without initialization
 * @to: failed and bypassed sched taking custody of @p
 * @p: task to punt
 *
 * Take @p off its current sched and put it on @to at %SCX_TASK_NONE. @to is
 * dying and its teardown will re-home @p properly.
 *
 * Used when @to must take over @p but failed to initialize it. Bypass keeps
 * scheduling decisions away from @to but @p can still trigger its task ops,
 * which may confuse the BPF side. @to is dying anyway. The exit paths skip
 * %NONE tasks (see __scx_disable_and_exit_task() and switched_from_scx()).
 */
நிலையான வெறுமை scx_punt_task(கட்டமைப்பு scx_sched *to, கட்டமைப்பு task_struct *p)
{
	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(task_rq(p));
	WARN_ON_ONCE(!READ_ONCE(to->bypass_depth));

	scoped_guard (sched_change, p, DEQUEUE_SAVE | DEQUEUE_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_set_task_sched(p, to);
	}
}

நிலையான வெறுமை scx_fail_parent(கட்டமைப்பு scx_sched *sch,
			    கட்டமைப்பு task_struct *failed, s32 fail_code)
{
	கட்டமைப்பு scx_sched *parent = scx_parent(sch);
	கட்டமைப்பு scx_task_iter sti;
	கட்டமைப்பு task_struct *p;

	scx_error(parent, "ops.init_task() failed (%d) for %s[%d] while disabling a sub-scheduler",
		  fail_code, failed->comm, failed->pid);

	/*
	 * Once $parent is bypassed, tasks can be punted into it. This may
	 * cause downstream failures on the BPF side but $parent is dying
	 * anyway.
	 */
	scx_bypass(parent, true);

	scx_task_iter_start(&sti, sch->cgrp);
	போது ((p = scx_task_iter_next_locked(&sti))) {
		எனில் (scx_task_on_sched(parent, p))
			தொடர்;

		scx_punt_task(parent, p);
	}
	scx_task_iter_stop(&sti);
}

#ifdef CONFIG_EXT_GROUP_SCHED
/**
 * scx_cgroup_claim_subtree - Claim the subtree's cgroups for an enabling sub
 * @sch: sub-scheduler being enabled
 *
 * Called while enabling @sch, after the subtree's cgrp->scx_sched's are pointed
 * at @sch and before any task is claimed. This mirrors root enable's
 * cgroups-before-tasks order. The ops.init_task() args are task_group-granular
 * and can still reference a cgroup outside the handed-over set when the cpu
 * controller is coarser than the sub topology or mounted on cgroup1.
 *
 * First init each of the parent sched's subtree cgroups on @sch, and only then
 * exit them from the parent, so that a failed init can be unwound with the
 * parent untouched. The both-inited transient is invisible outside
 * scx_cgroup_lock(). %SCX_TG_SUB_INIT tracks the first pass's progress.
 * %SCX_TG_INITED stays set throughout, except for a task_group whose
 * ops.cgroup_init() failed on the parent (see scx_cgroup_return_subtree()):
 * there is nothing to exit from the parent and %SCX_TG_INITED is set back with
 * the transfer.
 *
 * Dying but not yet offlined task_groups are included: a removed cgroup keeps
 * hosting scheduling events until its dying tasks finish their final context
 * switches, so it still needs to be inited on a sched, and its offline-time
 * ops.cgroup_exit() follows the last of those events.
 *
 * Return 0 on success, -errno on failure. On failure, @sch has been
 * scx_error()'d and is left with no cgroups.
 */
நிலையான s32 scx_cgroup_claim_subtree(கட்டமைப்பு scx_sched *sch)
{
	கட்டமைப்பு cgroup *sub_cgrp = sch_cgroup(sch);
	கட்டமைப்பு cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	கட்டமைப்பு scx_sched *parent = scx_parent(sch);
	கட்டமைப்பு cgroup_subsys_state *css;
	முழு ret;

	css_for_each_descendant_pre(css, ecss) {
		கட்டமைப்பு task_group *tg = css_tg(css);
		கட்டமைப்பு scx_cgroup_init_args args = {
			.weight = tg->scx.weight,
			.bw_period_us = tg->scx.bw_period_us,
			.bw_quota_us = tg->scx.bw_quota_us,
			.bw_burst_us = tg->scx.bw_burst_us,
			.sched_idle = tg->scx.idle,
		};

		எனில் (tg->scx.sched != parent ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			தொடர்;

		எனில் (SCX_HAS_OP(sch, cgroup_init)) {
			ret = SCX_CALL_OP_RET(sch, cgroup_init, வெற்று, css->cgroup, &args);
			எனில் (ret) {
				scx_error(sch, "ops.cgroup_init() failed (%d)", ret);
				செல் err;
			}
		}
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_post(css, ecss) {
		கட்டமைப்பு task_group *tg = css_tg(css);

		/*
		 * SUB_INIT is pass 1's progress mark: pass 2 and the err path
		 * must visit exactly the tgs pass 1 inited.
		 */
		எனில் (!(tg->scx.flags & SCX_TG_SUB_INIT))
			தொடர்;

		/* skip the exit if the parent's ops.cgroup_init() failed */
		எனில் ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(parent, cgroup_exit))
			SCX_CALL_OP(parent, cgroup_exit, வெற்று, css->cgroup);
		tg->scx.sched = sch;
		tg->scx.flags |= SCX_TG_INITED;
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}

	திரும்பு 0;

err:
	css_for_each_descendant_post(css, ecss) {
		கட்டமைப்பு task_group *tg = css_tg(css);

		எனில் (!(tg->scx.flags & SCX_TG_SUB_INIT))
			தொடர்;

		எனில் (SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, வெற்று, css->cgroup);
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}
	திரும்பு ret;
}

/**
 * scx_cgroup_return_subtree - Return the subtree's cgroups to the parent sched
 * @sch: sub-scheduler being disabled
 *
 * Called while disabling @sch, after the subtree's cgrp->scx_sched's are reset
 * to the parent sched and before tasks are re-homed, mirroring root disable's
 * cgroups-before-tasks teardown order. The reverse of
 * scx_cgroup_claim_subtree(): exit @sch's cgroups from @sch, then init them on
 * the parent with the current tg->scx.* values, resyncing settings that changed
 * while @sch had them.
 *
 * When an init on the parent fails, the parent is failed - the same policy as
 * task re-homing. The remaining task_groups are punted: they move to the parent
 * anyway with %SCX_TG_INITED cleared, as ops.cgroup_init() failed or never ran
 * for them. A punted task_group gets no cgroup ops. The dying parent's own
 * disable moves it one sched up, initing it there. Root ends the chain: root
 * teardown drops cgroup ops entirely and the next enable's bulk init re-inits
 * every online task_group.
 *
 * The task re-home that follows still delivers ops.init_task() to the dying
 * parent, including for tasks in punted cgroups it never inited - tolerated
 * like the downstream failures of task punting (see scx_punt_task()).
 */
நிலையான வெறுமை scx_cgroup_return_subtree(கட்டமைப்பு scx_sched *sch)
{
	கட்டமைப்பு cgroup *sub_cgrp = sch_cgroup(sch);
	கட்டமைப்பு cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	கட்டமைப்பு scx_sched *parent = scx_parent(sch);
	கட்டமைப்பு cgroup_subsys_state *css;
	bool parent_failed = false;
	முழு ret;

	css_for_each_descendant_post(css, ecss) {
		கட்டமைப்பு task_group *tg = css_tg(css);

		எனில் (tg->scx.sched != sch ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			தொடர்;

		/* skip the exit if @sch's ops.cgroup_init() failed for the tg */
		எனில் ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, வெற்று, css->cgroup);
		tg->scx.sched = parent;
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_pre(css, ecss) {
		கட்டமைப்பு task_group *tg = css_tg(css);
		கட்டமைப்பு scx_cgroup_init_args args = {
			.weight = tg->scx.weight,
			.bw_period_us = tg->scx.bw_period_us,
			.bw_quota_us = tg->scx.bw_quota_us,
			.bw_burst_us = tg->scx.bw_burst_us,
			.sched_idle = tg->scx.idle,
		};

		/* the first pass must have transferred everything */
		WARN_ON_ONCE(tg->scx.sched == sch);

		/*
		 * SUB_INIT distinguishes the tgs pass 1 moved. The sched test
		 * can't: a tg punted to the parent by an earlier failure would
		 * also match.
		 */
		எனில் (!(tg->scx.flags & SCX_TG_SUB_INIT))
			தொடர்;
		tg->scx.flags &= ~(SCX_TG_SUB_INIT | SCX_TG_INITED);

		/*
		 * A re-init on $parent failed. The task_groups from here on are
		 * punted: they stay on the dying $parent with INITED clear and
		 * move onward when it disables.
		 */
		எனில் (parent_failed)
			தொடர்;

		எனில் (SCX_HAS_OP(parent, cgroup_init)) {
			ret = SCX_CALL_OP_RET(parent, cgroup_init, வெற்று, css->cgroup, &args);
			எனில் (ret) {
				scx_error(parent, "ops.cgroup_init() failed (%d) while disabling a sub-scheduler",
					  ret);
				parent_failed = true;
				தொடர்;
			}
		}
		tg->scx.flags |= SCX_TG_INITED;
	}
}
#இல்லையெனில்
நிலையான உள்வரி s32 scx_cgroup_claim_subtree(கட்டமைப்பு scx_sched *sch) { திரும்பு 0; }
நிலையான உள்வரி வெறுமை scx_cgroup_return_subtree(கட்டமைப்பு scx_sched *sch) {}
#endif

வெறுமை scx_sub_disable(கட்டமைப்பு scx_sched *sch)
{
	கட்டமைப்பு scx_sched *parent = scx_parent(sch);
	கட்டமைப்பு scx_task_iter sti;
	கட்டமைப்பு task_struct *p;
	முழு ret;

	/*
	 * Guarantee forward progress and wait for descendants to be disabled.
	 * To limit disruptions, $parent is not bypassed. Tasks are fully
	 * prepped and then inserted back into $parent.
	 */
	scx_bypass(sch, true);
	drain_descendants(sch);

	/*
	 * Here, every runnable task is guaranteed to make forward progress and
	 * we can safely use blocking synchronization constructs. Actually
	 * disable ops.
	 */
	mutex_lock(&scx_enable_mutex);
	percpu_down_write(&scx_fork_rwsem);
	scx_cgroup_lock();

	/*
	 * An enable that failed before scx_link_sched() succeeded never owned a
	 * cgroup or task and won't be waited on by an ancestor's
	 * drain_descendants(). Nothing to reparent and walking the tasks can
	 * misbehave as the task ownership invariant (either owned by self or
	 * parent) does not hold. ->sibling can't identify this case - an undone
	 * link leaves it non-empty.
	 */
	எனில் (!sch->linked)
		செல் dump;

	set_cgroup_sched(sch_cgroup(sch), parent);

	/*
	 * Return the subtree's cgroups before re-homing tasks so that any
	 * ops.init_task() on $parent only sees cgroups it has initialized.
	 */
	scx_cgroup_return_subtree(sch);

	scx_task_iter_start(&sti, sch->cgrp);
	போது ((p = scx_task_iter_next_locked(&sti))) {
		கட்டமைப்பு rq *rq;
		கட்டமைப்பு rq_flags rf;

		/* filter out duplicate visits */
		எனில் (scx_task_on_sched(parent, p))
			தொடர்;

		/*
		 * By the time control reaches here, all linked descendant
		 * schedulers should have been disabled.
		 */
		WARN_ON_ONCE(!scx_task_on_sched(sch, p));

		/*
		 * @p is pinned by the iter: css_task_iter_next() takes a
		 * reference and holds it until the next iter_next() call, so
		 * @p->usage is guaranteed > 0.
		 */
		get_task_struct(p);

		scx_task_iter_unlock(&sti);

		/*
		 * $p is READY or ENABLED on @sch. Initialize for $parent,
		 * disable and exit from @sch, and then switch over to $parent.
		 *
		 * If a task fails to initialize for $parent, the only available
		 * action is disabling $parent too. While this allows disabling
		 * of a child sched to cause the parent scheduler to fail, the
		 * failure can only originate from ops.init_task() of the
		 * parent. A child can't directly affect the parent through its
		 * own failures.
		 */
		ret = __scx_init_task(parent, p, வெற்று, false);
		எனில் (ret) {
			scx_fail_parent(sch, p, ret);
			put_task_struct(p);
			நிறுத்து;
		}

		rq = task_rq_lock(p, &rf);

		எனில் (scx_get_task_state(p) == SCX_TASK_DEAD) {
			/*
			 * sched_ext_dead() raced us between __scx_init_task()
			 * and this rq lock and ran exit_task() on @sch (the
			 * sched @p was on at that point), not on $parent.
			 * $parent's just-completed init is owed an exit_task()
			 * and we issue it here.
			 */
			scx_sub_init_cancel_task(parent, p);
			task_rq_unlock(rq, p, &rf);
			put_task_struct(p);
			தொடர்;
		}

		scx_rehome_task(parent, p);

		task_rq_unlock(rq, p, &rf);
		put_task_struct(p);
	}
	scx_task_iter_stop(&sti);

dump:
	scx_disable_dump(sch);

	scx_cgroup_unlock();
	percpu_up_write(&scx_fork_rwsem);

	/*
	 * All tasks are moved off of @sch but there may still be on-going
	 * operations (e.g. ops.select_cpu()). Drain them by flushing RCU. Use
	 * the expedited version as ancestors may be waiting in bypass mode.
	 * Also, tell the parent that there is no need to keep running bypass
	 * DSQs for us.
	 */
	synchronize_rcu_expedited();
	scx_disable_bypass_dsp(sch);

	scx_unlink_sched(sch);

	mutex_unlock(&scx_enable_mutex);

	/*
	 * @sch is now unlinked from the parent's children list. Notify and call
	 * ops.sub_detach/exit(). Note that ops.sub_detach/exit() must be called
	 * after unlinking and releasing all locks. See scx_claim_exit().
	 */
	wake_up_all(&scx_unlink_waitq);

	எனில் (parent->ops.sub_detach && sch->sub_attached) {
		கட்டமைப்பு scx_sub_detach_args sub_detach_args = {
			.ops = &sch->ops,
			.cgroup_path = sch->cgrp_path,
		};
		SCX_CALL_OP(parent, sub_detach, வெற்று,
			    &sub_detach_args);
	}

	scx_log_sched_disable(sch);

	எனில் (sch->ops.exit)
		SCX_CALL_OP(sch, exit, வெற்று, sch->exit_info);

	/*
	 * @sch's non-ops programs such as timers and tracers can fire after
	 * ops.exit(). Now that exit is complete, stop scx_prog_sched() from
	 * resolving to @sch and drain in-flight resolvers.
	 */
	WRITE_ONCE(sch->dead, true);
	synchronize_rcu();

	எனில் (sch->sub_kset)
		kobject_del(&sch->sub_kset->kobj);
	/* not added if enable failed before scx_sched_sysfs_add() */
	எனில் (sch->kobj.state_in_sysfs)
		kobject_del(&sch->kobj);
}

/* verify that a scheduler can be attached to @cgrp and return the parent */
நிலையான கட்டமைப்பு scx_sched *find_parent_sched(கட்டமைப்பு cgroup *cgrp)
{
	கட்டமைப்பு scx_sched *parent = scx_cgroup_sched(cgrp);
	கட்டமைப்பு scx_sched *pos;

	lockdep_assert_held(&scx_sched_lock);

	/* can't attach twice to the same cgroup */
	எனில் (parent->cgrp == cgrp)
		திரும்பு ERR_PTR(-EBUSY);

	/* does $parent allow sub-scheds? */
	எனில் (!parent->ops.sub_attach)
		திரும்பு ERR_PTR(-EOPNOTSUPP);

	/* can't insert between $parent and its exiting children */
	list_for_each_entry(pos, &parent->children, sibling)
		எனில் (cgroup_is_descendant(pos->cgrp, cgrp))
			திரும்பு ERR_PTR(-EBUSY);

	திரும்பு parent;
}

நிலையான bool assert_task_ready_or_enabled(கட்டமைப்பு task_struct *p)
{
	u32 state = scx_get_task_state(p);

	தேர்வு (state) {
	நிலை SCX_TASK_READY:
	நிலை SCX_TASK_ENABLED:
		திரும்பு true;
	இயல்பு:
		WARN_ONCE(true, "sched_ext: Invalid task state %d for %s[%d] during enabling sub sched",
			  state, p->comm, p->pid);
		திரும்பு false;
	}
}

வெறுமை scx_sub_enable_workfn(கட்டமைப்பு kthread_work *work)
{
	கட்டமைப்பு scx_enable_cmd *cmd = container_of(work, கட்டமைப்பு scx_enable_cmd, work);
	கட்டமைப்பு sched_ext_ops *ops = cmd->ops;
	கட்டமைப்பு cgroup *cgrp;
	கட்டமைப்பு scx_sched *parent, *sch;
	கட்டமைப்பு scx_task_iter sti;
	கட்டமைப்பு task_struct *p;
	s32 i, ret;

	mutex_lock(&scx_enable_mutex);

	எனில் (!scx_enabled()) {
		ret = -ENODEV;
		செல் out_unlock;
	}

	/* See scx_root_enable_workfn() for the @ops->priv check. */
	எனில் (rcu_access_pointer(ops->priv)) {
		ret = -EBUSY;
		செல் out_unlock;
	}

	cgrp = cgroup_get_from_id(ops->sub_cgroup_id);
	எனில் (IS_ERR(cgrp)) {
		ret = PTR_ERR(cgrp);
		செல் out_unlock;
	}

	raw_spin_lock_irq(&scx_sched_lock);
	parent = find_parent_sched(cgrp);
	எனில் (IS_ERR(parent)) {
		raw_spin_unlock_irq(&scx_sched_lock);
		ret = PTR_ERR(parent);
		செல் out_put_cgrp;
	}
	kobject_get(&parent->kobj);
	raw_spin_unlock_irq(&scx_sched_lock);

	/*
	 * Flip the hot-path gates before ops->priv is published - the sub's
	 * programs can e.g. kick cpus from that point on. The matching dec is
	 * at the end of scx_sched_free_rcu_work().
	 */
	static_branch_inc(&__scx_has_subs);

	/* scx_alloc_and_add_sched() consumes @cgrp whether it succeeds or not */
	sch = scx_alloc_and_add_sched(cmd, cgrp, parent);
	kobject_put(&parent->kobj);
	எனில் (IS_ERR(sch)) {
		static_branch_dec(&__scx_has_subs);
		ret = PTR_ERR(sch);
		செல் out_unlock;
	}

	/*
	 * Validate before scx_link_sched() publishes @sch, so an invalid sub
	 * never becomes visible with an unallocated pshard.
	 */
	ret = scx_validate_ops(sch, ops);
	எனில் (ret)
		செல் err_disable;

	scx_rescue_check_timeout(sch);

	/*
	 * Allocate pshard[] before scx_link_sched() publishes @sch into the
	 * parent's RCU children list. A concurrent revoke walking the tree
	 * would otherwise dereference sch->pshard[si] while it's still NULL.
	 * Unlike the root path, the cid shard layout is stable at this point.
	 *
	 * scx_alloc_pshards() skips allocation when @sch's arena pool isn't
	 * initialized, so scx_arena_pool_init() must run first.
	 */
	ret = scx_arena_pool_init(sch);
	எனில் (ret)
		செல் err_disable;

	ret = scx_alloc_pshards(sch);
	எனில் (ret)
		செல் err_disable;

	ret = scx_link_sched(sch);
	எனில் (ret)
		செல் err_disable;

	ret = scx_sched_sysfs_add(sch);
	எனில் (ret)
		செல் err_disable;

	எனில் (sch->level >= SCX_SUB_MAX_DEPTH) {
		scx_error(sch, "max nesting depth %d violated",
			  SCX_SUB_MAX_DEPTH);
		ret = -EINVAL;
		செல் err_disable;
	}

	scoped_guard(cpus_read_lock) {
		ret = scx_alloc_kern_arena_objs(sch);
		எனில் (ret)
			செல் err_disable;
	}

	எனில் (sch->ops.init) {
		ret = SCX_CALL_OP_RET(sch, init, வெற்று);
		எனில் (ret) {
			ret = scx_ops_sanitize_err(sch, "init", ret);
			scx_error(sch, "ops.init() failed (%d)", ret);
			செல் err_disable;
		}
		sch->exit_info->flags |= SCX_EFLAG_INITIALIZED;
	}

	கட்டமைப்பு scx_sub_attach_args sub_attach_args = {
		.ops = &sch->ops,
		.cgroup_path = sch->cgrp_path,
	};

	ret = SCX_CALL_OP_RET(parent, sub_attach, வெற்று,
			      &sub_attach_args);
	எனில் (ret) {
		ret = scx_ops_sanitize_err(sch, "sub_attach", ret);
		scx_error(sch, "parent rejected (%d)", ret);
		செல் err_disable;
	}
	sch->sub_attached = true;

	scx_bypass(sch, true);

	க்கு (i = SCX_OPI_BEGIN; i < SCX_OPI_END; i++)
		எனில் (((வெறுமை (**)(வெறுமை))ops)[i])
			set_bit(i, sch->has_op);

	percpu_down_write(&scx_fork_rwsem);
	scx_cgroup_lock();

	/*
	 * Set cgroup->scx_sched's and check CSS_ONLINE. Either we see
	 * !CSS_ONLINE or scx_cgroup_lifetime_notify() sees and shoots us down.
	 */
	set_cgroup_sched(sch_cgroup(sch), sch);
	எனில் (!(cgrp->self.flags & CSS_ONLINE)) {
		scx_error(sch, "cgroup is not online");
		ret = -ENODEV;
		செல் err_unlock_and_disable;
	}

	/*
	 * Take over the subtree's cgroups before any task is claimed,
	 * mirroring root enable's cgroups-before-tasks order.
	 */
	ret = scx_cgroup_claim_subtree(sch);
	எனில் (ret)
		செல் err_unlock_and_disable;

	/*
	 * Initialize tasks for the new child $sch without exiting them for
	 * $parent so that the tasks can always be reverted back to $parent
	 * sched on child init failure.
	 */
	WARN_ON_ONCE(scx_enabling_sub_sched);
	scx_enabling_sub_sched = sch;

	scx_task_iter_start(&sti, sch->cgrp);
	போது ((p = scx_task_iter_next_locked(&sti))) {
		கட்டமைப்பு rq *rq;
		கட்டமைப்பு rq_flags rf;

		/*
		 * Task iteration may visit the same task twice when racing
		 * against exiting. Use %SCX_TASK_SUB_INIT to mark tasks which
		 * finished __scx_init_task() and skip if set.
		 *
		 * A task may exit and get freed between __scx_init_task()
		 * completion and scx_enable_task(). In such cases,
		 * scx_disable_and_exit_task() must exit the task for both the
		 * parent and child scheds.
		 */
		எனில் (p->scx.flags & SCX_TASK_SUB_INIT)
			தொடர்;

		/* @p is pinned by the iter; see scx_sub_disable() */
		get_task_struct(p);

		எனில் (!assert_task_ready_or_enabled(p)) {
			ret = -EINVAL;
			செல் abort;
		}

		scx_task_iter_unlock(&sti);

		/*
		 * As $p is still on $parent, it can't be transitioned to INIT.
		 * Let's worry about task state later. Use __scx_init_task().
		 */
		ret = __scx_init_task(sch, p, வெற்று, false);
		எனில் (ret)
			செல் abort;

		rq = task_rq_lock(p, &rf);

		எனில் (scx_get_task_state(p) == SCX_TASK_DEAD) {
			/*
			 * sched_ext_dead() raced us between __scx_init_task()
			 * and this rq lock and ran exit_task() on $parent (the
			 * sched @p was on at that point), not on @sch. @sch's
			 * just-completed init is owed an exit_task() and we
			 * issue it here.
			 */
			scx_sub_init_cancel_task(sch, p);
			task_rq_unlock(rq, p, &rf);
			put_task_struct(p);
			தொடர்;
		}

		p->scx.flags |= SCX_TASK_SUB_INIT;
		task_rq_unlock(rq, p, &rf);

		put_task_struct(p);
	}
	scx_task_iter_stop(&sti);

	/*
	 * All tasks are prepped. Disable/exit tasks for $parent and enable for
	 * the new @sch.
	 */
	scx_task_iter_start(&sti, sch->cgrp);
	போது ((p = scx_task_iter_next_locked(&sti))) {
		/*
		 * Use clearing of %SCX_TASK_SUB_INIT to detect and skip
		 * duplicate iterations.
		 */
		எனில் (!(p->scx.flags & SCX_TASK_SUB_INIT))
			தொடர்;

		scoped_guard (sched_change, p, DEQUEUE_SAVE | DEQUEUE_MOVE) {
			/*
			 * $p must be either READY or ENABLED. If ENABLED,
			 * __scx_disabled_and_exit_task() first disables and
			 * makes it READY. However, after exiting $p, it will
			 * leave $p as READY.
			 */
			assert_task_ready_or_enabled(p);
			__scx_disable_and_exit_task(parent, p);

			/*
			 * $p is now only initialized for @sch and READY, which
			 * is what we want. Assign it to @sch and, if it's on
			 * the ext class, enable. A non-ext task, possible under
			 * an %SCX_OPS_SWITCH_PARTIAL root, stays READY and is
			 * enabled by switching_to_scx() if it switches over.
			 */
			scx_set_task_sched(p, sch);
			எனில் (p->sched_class == &ext_sched_class)
				scx_enable_task(sch, p);

			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);

	scx_enabling_sub_sched = வெற்று;

	scx_cgroup_unlock();
	percpu_up_write(&scx_fork_rwsem);

	scx_bypass(sch, false);

	/* @sch is enabled; deliver any caps owed since its sub_attach() */
	scx_sub_seed_caps(sch);

	pr_info("sched_ext: BPF sub-scheduler \"%s\" enabled\n", sch->ops.name);
	kobject_uevent(&sch->kobj, KOBJ_ADD);
	ret = 0;
	செல் out_unlock;

out_put_cgrp:
	cgroup_put(cgrp);
out_unlock:
	mutex_unlock(&scx_enable_mutex);
	cmd->ret = ret;
	திரும்பு;

abort:
	put_task_struct(p);
	scx_task_iter_stop(&sti);

	/*
	 * Undo __scx_init_task() for tasks we marked. scx_enable_task() never
	 * ran for @sch on them, so calling scx_disable_task() here would invoke
	 * ops.disable() without a matching ops.enable(). scx_enabling_sub_sched
	 * must stay set until SUB_INIT is cleared from every marked task -
	 * scx_disable_and_exit_task() reads it when a task exits concurrently.
	 */
	scx_task_iter_start(&sti, sch->cgrp);
	போது ((p = scx_task_iter_next_locked(&sti))) {
		எனில் (p->scx.flags & SCX_TASK_SUB_INIT) {
			scx_sub_init_cancel_task(sch, p);
			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);
	scx_enabling_sub_sched = வெற்று;
err_unlock_and_disable:
	/* we'll soon enter disable path, keep bypass on */
	scx_cgroup_unlock();
	percpu_up_write(&scx_fork_rwsem);
err_disable:
	mutex_unlock(&scx_enable_mutex);
	/*
	 * Some enable failures only return an errno (e.g. -ENOMEM from an
	 * allocation) without calling scx_error(). Record it so
	 * scx_flush_disable_work() runs the disable and ops.exit() fires.
	 */
	scx_error(sch, "scx_sub_enable() failed (%d)", ret);
	scx_flush_disable_work(sch);
	cmd->ret = 0;
}

/**
 * scx_cgroup_task_migrating - Prepare a task for a cgroup migration
 * @ctx: migration being prepared
 *
 * A task's sched must match its cgroup's owner, so a migration that crosses a
 * sched boundary re-homes the task once committed. Run the fallible part here,
 * before the migration commits: initialize the task for the destination sched.
 * A rejection fails the cgroup.procs write.
 */
நிலையான s32 scx_cgroup_task_migrating(கட்டமைப்பு cgroup_task_migrate_ctx *ctx)
{
	கட்டமைப்பு task_struct *p = ctx->task;
	கட்டமைப்பு scx_sched *to;
	முழு ret;

	/*
	 * Cleared under scx_cgroup_lock() before root disable starts tearing
	 * down tasks. As cgroup_mutex is held, a set flag guarantees that the
	 * teardown loop is not running concurrently.
	 */
	எனில் (!scx_cgroup_enabled)
		திரும்பு NOTIFY_OK;

	to = scx_cgroup_sched(ctx->dst_dcgrp);
	எனில் (scx_task_on_sched(to, p))
		திரும்பு NOTIFY_OK;

	ret = __scx_init_task(to, p, ctx->dst_dcgrp, false);
	எனில் (ret)
		திரும்பு notifier_from_errno(ret);

	திரும்பு NOTIFY_OK;
}

/**
 * scx_cgroup_task_migrated - Re-home a task that changed cgroups
 * @ctx: committed migration
 *
 * Move the task to its new cgroup's sched, which scx_cgroup_task_migrating()
 * already initialized it for. Can't fail.
 *
 * This is safe against all phases of the destination sched's destruction. A
 * disable resets cgroup ownership to the parent and re-homes tasks in one
 * scx_cgroup_lock() section. If that section already ran, the destination would
 * be the parent. Otherwise, the re-home loop is still ahead and guaranteed to
 * visit the task, now in the destination cgroup.
 */
நிலையான வெறுமை scx_cgroup_task_migrated(கட்டமைப்பு cgroup_task_migrate_ctx *ctx)
{
	கட்டமைப்பு task_struct *p = ctx->task;
	கட்டமைப்பு scx_sched *to;
	கட்டமைப்பு rq *rq;
	கட்டமைப்பு rq_flags rf;

	எனில் (!scx_cgroup_enabled)
		திரும்பு;

	to = scx_cgroup_sched(ctx->dst_dcgrp);
	எனில் (scx_task_on_sched(to, p))
		திரும்பு;

	rq = task_rq_lock(p, &rf);
	scx_rehome_task(to, p);
	task_rq_unlock(rq, p, &rf);
}

/**
 * scx_cgroup_task_migrate_canceled - Undo migration preparation
 * @ctx: canceled migration
 *
 * The migration failed after scx_cgroup_task_migrating() initialized the task
 * for the destination sched. The task stays on its current sched in the source
 * cgroup. Undo the destination's init.
 */
நிலையான வெறுமை scx_cgroup_task_migrate_canceled(கட்டமைப்பு cgroup_task_migrate_ctx *ctx)
{
	கட்டமைப்பு task_struct *p = ctx->task;
	கட்டமைப்பு scx_sched *to;
	கட்டமைப்பு rq *rq;
	கட்டமைப்பு rq_flags rf;

	எனில் (!scx_cgroup_enabled)
		திரும்பு;

	to = scx_cgroup_sched(ctx->dst_dcgrp);
	எனில் (scx_task_on_sched(to, p))
		திரும்பு;

	rq = task_rq_lock(p, &rf);
	scx_sub_init_cancel_task(to, p);
	task_rq_unlock(rq, p, &rf);
}

நிலையான s32 scx_cgroup_lifetime_notify(கட்டமைப்பு notifier_block *nb,
				      unsigned long action, வெறுமை *data)
{
	கட்டமைப்பு cgroup *cgrp = data;
	கட்டமைப்பு cgroup *parent = cgroup_parent(cgrp);
	கட்டமைப்பு scx_sched *sch;

	எனில் (!cgroup_on_dfl(cgrp))
		திரும்பு NOTIFY_OK;

	தேர்வு (action) {
	நிலை CGROUP_LIFETIME_ONLINE:
		/* inherit ->scx_sched from $parent */
		எனில் (parent)
			rcu_assign_pointer(cgrp->scx_sched, scx_cgroup_sched(parent));
		நிறுத்து;
	நிலை CGROUP_LIFETIME_OFFLINE:
		/* if there is a sched attached, shoot it down */
		sch = scx_cgroup_sched(cgrp);
		எனில் (sch && sch->cgrp == cgrp)
			scx_exit(sch, SCX_EXIT_UNREG_KERN,
				 SCX_ECODE_RSN_CGROUP_OFFLINE,
				 "cgroup %llu going offline", cgroup_id(cgrp));
		நிறுத்து;
	}

	திரும்பு NOTIFY_OK;
}

நிலையான கட்டமைப்பு notifier_block scx_cgroup_lifetime_nb = {
	.notifier_call = scx_cgroup_lifetime_notify,
};

நிலையான s32 scx_cgroup_task_notify(கட்டமைப்பு notifier_block *nb,
				  unsigned long action, வெறுமை *data)
{
	கட்டமைப்பு cgroup_task_migrate_ctx *ctx = data;

	தேர்வு (action) {
	நிலை CGROUP_TASK_MIGRATING:
		திரும்பு scx_cgroup_task_migrating(ctx);
	நிலை CGROUP_TASK_MIGRATED:
		scx_cgroup_task_migrated(ctx);
		நிறுத்து;
	நிலை CGROUP_TASK_MIGRATE_CANCELED:
		scx_cgroup_task_migrate_canceled(ctx);
		நிறுத்து;
	}

	திரும்பு NOTIFY_OK;
}

நிலையான கட்டமைப்பு notifier_block scx_cgroup_task_nb = {
	.notifier_call = scx_cgroup_task_notify,
};

நிலையான s32 __init scx_cgroup_notifier_init(வெறுமை)
{
	s32 ret;

	ret = blocking_notifier_chain_register(&cgroup_lifetime_notifier,
					       &scx_cgroup_lifetime_nb);
	எனில் (ret)
		திரும்பு ret;

	திரும்பு blocking_notifier_chain_register(&cgroup_task_notifier,
						&scx_cgroup_task_nb);
}
core_initcall(scx_cgroup_notifier_init);

நிலையான வெறுமை scx_pstack_recursion(கட்டமைப்பு bpf_prog *prog, மாறிலி எழுத்து *op)
{
	கட்டமைப்பு scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(prog->aux);
	எனில் (unlikely(!sch))
		திரும்பு;

	scx_error(sch, "%s recursion detected", op);
}

வெறுமை scx_pstack_recursion_on_dispatch(கட்டமைப்பு bpf_prog *prog)
{
	scx_pstack_recursion(prog, "dispatch");
}

வெறுமை scx_pstack_recursion_on_caps_updated(கட்டமைப்பு bpf_prog *prog)
{
	scx_pstack_recursion(prog, "sub_caps_updated");
}

__bpf_kfunc_start_defs();

/**
 * scx_bpf_sub_dispatch - Trigger dispatching on a child scheduler
 * @cgroup_id: cgroup ID of the child scheduler to dispatch
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Allows a parent scheduler to trigger dispatching on one of its direct
 * child schedulers. The child scheduler runs its dispatch operation to
 * move tasks from dispatch queues to the local runqueue.
 *
 * Returns: true on success, false if cgroup_id is invalid, not a direct
 * child, or caller lacks dispatch permission.
 */
__bpf_kfunc bool scx_bpf_sub_dispatch(u64 cgroup_id, மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	கட்டமைப்பு rq *rq = scx_locked_rq();
	கட்டமைப்பு scx_sched *parent, *child;

	guard(rcu)();
	parent = scx_prog_sched(aux);
	எனில் (unlikely(!parent))
		திரும்பு false;

	child = scx_find_sub_sched(cgroup_id);

	எனில் (unlikely(!child))
		திரும்பு false;

	எனில் (unlikely(scx_parent(child) != parent)) {
		scx_error(parent, "trying to dispatch a distant sub-sched on cgroup %llu",
			  cgroup_id);
		திரும்பு false;
	}

	/*
	 * Skip a child that does not effectively hold the base cap on this cpu:
	 * its inserts would only be rejected. ecaps are synced at the top of
	 * dispatch_one() before dispatch, so this reflects the in-effect state.
	 */
	எனில் (scx_missing_caps(child, cpu_of(rq), SCX_CAP_BASE))
		திரும்பு false;

	திரும்பு scx_dispatch_sched(child, rq, rq->scx.sub_dispatch_prev, true) !=
		SCX_DSP_NONE;
}

/* Validate common inputs. On success, *parent_out and *child_out are set. */
நிலையான s32 sub_cap_preamble(u64 cgroup_id, u64 caps, மாறிலி கட்டமைப்பு bpf_prog_aux *aux,
			    கட்டமைப்பு scx_sched **parent_out, கட்டமைப்பு scx_sched **child_out)
{
	கட்டமைப்பு scx_sched *parent, *child;

	parent = scx_prog_sched(aux);
	எனில் (unlikely(!parent))
		திரும்பு -ENODEV;

	எனில் (!scx_is_cid_type()) {
		scx_error(parent, "sub-cap kfuncs require a cid-form scheduler");
		திரும்பு -EOPNOTSUPP;
	}

	child = scx_find_sub_sched(cgroup_id);
	எனில் (unlikely(!child))
		திரும்பு -ENODEV;

	எனில் (unlikely(scx_parent(child) != parent)) {
		scx_error(parent, "%s: sub-%llu is not a direct child",
			  parent->cgrp_path, cgroup_id);
		திரும்பு -EINVAL;
	}

	எனில் (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(parent, "invalid caps 0x%llx", caps);
		திரும்பு -EINVAL;
	}

	*parent_out = parent;
	*child_out = child;
	திரும்பு 0;
}

/**
 * scx_bpf_sub_grant - Grant @caps on a cmask's cids to a direct child
 * @cgroup_id: cgroup id of the direct child sub-sched
 * @caps: bitmask of SCX_CAP_* to grant
 * @cmask__arena: cid cmask to grant @caps on
 * @denied_out__arena__nullable: optional cmask accumulating refused cids
 * @aux: implicit BPF argument
 *
 * A cid in @cmask__arena is granted to the child only if the parent holds every
 * requested cap on it. Refused cids are OR'd into the denied mask when
 * provided. Refusals outside the denied mask's range are not recorded.
 *
 * All-or-nothing keeps the caller-visible result binary per cid, so the denied
 * mask is one mask to interpret rather than a per-cap matrix.
 *
 * Return 0 on full success, -EPERM if any cid was refused, or a negative
 * errno on other failures.
 */
__bpf_kfunc s32 scx_bpf_sub_grant(u64 cgroup_id, u64 caps,
				  மாறிலி கட்டமைப்பு scx_cmask *cmask__arena,
				  கட்டமைப்பு scx_cmask *denied_out__arena__nullable,
				  மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	கட்டமைப்பு scx_cmask_ref ref, denied_ref;
	கட்டமைப்பு scx_sched *parent, *child;
	bool any_denied = false;
	LIST_HEAD(to_deliver);
	s32 si, ret;

	guard(irqsave)();

	ret = sub_cap_preamble(cgroup_id, caps, aux, &parent, &child);
	எனில் (ret)
		திரும்பு ret;

	ret = scx_cmask_ref_init(parent, cmask__arena, &ref);
	எனில் (ret) {
		scx_error(parent, "invalid cmask (%d)", ret);
		திரும்பு ret;
	}

	எனில் (denied_out__arena__nullable) {
		ret = scx_cmask_ref_init(parent, denied_out__arena__nullable, &denied_ref);
		எனில் (ret) {
			scx_error(parent, "invalid denied_out (%d)", ret);
			திரும்பு ret;
		}
	}

	/* apply the grant one shard at a time */
	க்கு (si = ref.shard_first; si < ref.shard_end; si++) {
		SCX_CMASK_DEFINE_SHARD(slice, 0, SCX_CID_SHARD_MAX_CPUS);
		கட்டமைப்பு scx_pshard *pps = parent->pshard[si];
		கட்டமைப்பு scx_pshard *cps = child->pshard[si];
		u64 granted_caps = 0;
		u32 cap_bit;

		scx_cmask_ref_shard(&ref, si, slice);
		எனில் (scx_cmask_empty(slice))
			தொடர்;

		SCX_CMASK_DEFINE_SHARD(granted_cids, slice->base, slice->nr_cids);
		SCX_CMASK_DEFINE_SHARD(changed_cids, slice->base, slice->nr_cids);
		SCX_CMASK_DEFINE_SHARD(delta, slice->base, slice->nr_cids);

		scx_cmask_copy(granted_cids, slice);

		scoped_guard (raw_spinlock, &pps->lock) {
			guard(raw_spinlock_nested)(&cps->lock);

			/*
			 * Narrow granted_cids to cids the parent holds every
			 * requested cap on. All-or-nothing per cid.
			 */
			scx_for_each_cap_bit(cap_bit, caps)
				scx_cmask_and(granted_cids, &pps->caps[cap_bit].cmask);

			/*
			 * For each requested cap, fold the newly-set cids into
			 * the child and accumulate the delta.
			 */
			scx_for_each_cap_bit(cap_bit, caps) {
				கட்டமைப்பு scx_cmask *ccm = &cps->caps[cap_bit].cmask;

				scx_cmask_copy(delta, granted_cids);
				scx_cmask_andnot(delta, ccm);
				எனில் (scx_cmask_empty(delta))
					தொடர்;

				scx_cmask_or(ccm, delta);
				scx_cmask_or(changed_cids, delta);
				granted_caps |= BIT_U64(cap_bit);
			}

			எனில் (granted_caps) {
				s32 cid;

				caps_updated_record(cps, changed_cids, granted_caps,
						    &to_deliver);
				/*
				 * The sync arms an update_idle() re-notify if
				 * the cid gains baseline access, so the holder
				 * learns of an already-idle cid.
				 */
				scx_cmask_for_each_cid(cid, changed_cids)
					queue_sync_ecaps(child, cid);
			}
		}

		/* record cids that didn't make it into the denied mask */
		எனில் (!scx_cmask_subset(slice, granted_cids)) {
			any_denied = true;
			எனில் (denied_out__arena__nullable) {
				SCX_CMASK_DEFINE_SHARD(denied, slice->base, slice->nr_cids);

				scx_cmask_copy(denied, slice);
				scx_cmask_andnot(denied, granted_cids);
				scx_cmask_ref_or(&denied_ref, denied);
			}
		}
	}

	caps_updated_deliver(&to_deliver);

	திரும்பு any_denied ? -EPERM : 0;
}

/**
 * scx_bpf_sub_revoke - Revoke @caps on a cmask's cids from a direct child
 * @cgroup_id: cgroup id of the direct child sub-sched
 * @caps: bitmask of SCX_CAP_* to revoke
 * @cmask__arena: cid cmask to revoke @caps on
 * @aux: implicit BPF argument
 *
 * Clear @caps bits on @cmask__arena from the child named by @cgroup_id and all
 * its descendants. The origin parent's pshard lock is held across the subtree
 * walk so a concurrent grant from the origin parent observes the revoked state.
 */
__bpf_kfunc வெறுமை scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    மாறிலி கட்டமைப்பு scx_cmask *cmask__arena,
				    மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	கட்டமைப்பு scx_cmask_ref ref;
	கட்டமைப்பு scx_sched *parent, *child, *pos;
	LIST_HEAD(to_deliver);
	s32 si, ret;

	guard(irqsave)();

	எனில் (sub_cap_preamble(cgroup_id, caps, aux, &parent, &child))
		திரும்பு;

	ret = scx_cmask_ref_init(parent, cmask__arena, &ref);
	எனில் (ret) {
		scx_error(parent, "invalid cmask (%d)", ret);
		திரும்பு;
	}

	/* per-shard, walk child's subtree and clear @caps */
	க்கு (si = ref.shard_first; si < ref.shard_end; si++) {
		SCX_CMASK_DEFINE_SHARD(slice, 0, SCX_CID_SHARD_MAX_CPUS);

		scx_cmask_ref_shard(&ref, si, slice);
		எனில் (scx_cmask_empty(slice))
			தொடர்;

		/*
		 * Pre-order with subtree skip: a descendant that cleared
		 * nothing means no descendant of it can hold @caps on these
		 * cids either.
		 */
		guard(raw_spinlock)(&parent->pshard[si]->lock);
		pos = scx_next_descendant_pre(வெற்று, child);
		போது (pos) {
			கட்டமைப்பு scx_pshard *ps = pos->pshard[si];
			SCX_CMASK_DEFINE_SHARD(changed_cids, slice->base, slice->nr_cids);
			SCX_CMASK_DEFINE_SHARD(delta, slice->base, slice->nr_cids);
			u64 revoked_caps = 0;
			u32 cap_bit;

			scoped_guard (raw_spinlock_nested, &ps->lock) {
				/*
				 * For each cap, clear lost cids and accumulate
				 * the per-cap diff for notification.
				 */
				scx_for_each_cap_bit(cap_bit, caps) {
					கட்டமைப்பு scx_cmask *cm = &ps->caps[cap_bit].cmask;

					scx_cmask_copy(delta, cm);
					scx_cmask_and(delta, slice);
					எனில் (scx_cmask_empty(delta))
						தொடர்;

					scx_cmask_andnot(cm, delta);
					scx_cmask_or(changed_cids, delta);
					revoked_caps |= BIT_U64(cap_bit);
				}

				எனில் (revoked_caps) {
					s32 cid;

					caps_updated_record(ps, changed_cids, revoked_caps,
							    &to_deliver);
					scx_cmask_for_each_cid(cid, changed_cids)
						queue_sync_ecaps(pos, cid);
				}
			}

			எனில் (revoked_caps)
				pos = scx_next_descendant_pre(pos, child);
			இல்லையெனில்
				pos = scx_skip_subtree_pre(pos, child);
		}
	}

	caps_updated_deliver(&to_deliver);
}

/**
 * scx_bpf_sub_caps - Read self's or a direct child's cap cmasks
 * @cgroup_id: 0 for self, or a direct child's cgroup id
 * @caps: one or more SCX_CAP_* bits
 * @out__arena: cmask to receive the union of @caps within its range
 * @aux: implicit BPF argument
 *
 * Read the cap cmasks granted on each cid for self (@cgroup_id 0) or a direct
 * child - the literal granted set. A sched can read only itself or a direct
 * child.
 *
 * Return 0, -ENODEV if @cgroup_id names no direct child, or -EINVAL on bad
 * inputs.
 */
__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, கட்டமைப்பு scx_cmask *out__arena,
				 மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	கட்டமைப்பு scx_cmask_ref ref;
	கட்டமைப்பு scx_sched *sch, *target;
	கட்டமைப்பு scx_pshard **pshard;
	s32 si, ret;

	guard(irqsave)();

	sch = scx_prog_sched(aux);
	எனில் (unlikely(!sch))
		திரும்பு -ENODEV;

	எனில் (!scx_is_cid_type()) {
		scx_error(sch, "sub-cap kfuncs require a cid-form scheduler");
		திரும்பு -EOPNOTSUPP;
	}

	எனில் (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(sch, "invalid caps 0x%llx", caps);
		திரும்பு -EINVAL;
	}

	/* @cgroup_id 0 reads self, otherwise a direct child */
	எனில் (cgroup_id) {
		target = scx_find_sub_sched(cgroup_id);
		எனில் (unlikely(!target))
			திரும்பு -ENODEV;
		எனில் (unlikely(scx_parent(target) != sch)) {
			scx_error(sch, "%s: sub-%llu is not a direct child",
				  sch->cgrp_path, cgroup_id);
			திரும்பு -EINVAL;
		}
	} இல்லையெனில் {
		target = sch;
	}

	/*
	 * The target's caps storage may not be set up yet (e.g. a self-read
	 * during ops.init_cids()). Pairs with the publish in
	 * scx_alloc_pshards(): a non-NULL pshard has every element set and the
	 * acquire also orders the cid table reads below against it.
	 */
	pshard = smp_load_acquire(&target->pshard);
	எனில் (unlikely(!pshard)) {
		scx_error(sch, "scx_bpf_sub_caps() called before caps storage is initialized");
		திரும்பு -ENODEV;
	}

	ret = scx_cmask_ref_init(sch, out__arena, &ref);
	எனில் (ret) {
		scx_error(sch, "invalid out (%d)", ret);
		திரும்பு ret;
	}

	க்கு (si = ref.shard_first; si < ref.shard_end; si++) {
		மாறிலி கட்டமைப்பு scx_cid_shard *shard =
			&rcu_dereference_all(scx_cid_shard_ranges)[si];
		SCX_CMASK_DEFINE_SHARD(local_out, shard->base_cid, shard->nr_cids);
		u32 cap_bit;

		scx_for_each_cap_bit(cap_bit, caps)
			scx_cmask_or(local_out, &pshard[si]->caps[cap_bit].cmask);
		scx_cmask_ref_copy(&ref, local_out);
	}
	திரும்பு 0;
}

/**
 * scx_bpf_sub_kill_bstr - Kill a direct child sub-scheduler
 * @cgroup_id: cgroup id of the direct child to kill
 * @fmt: reason message format string
 * @data: format string parameters packaged using ___bpf_fill() macro
 * @data__sz: @data len, must end in '__sz' for the verifier
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Evict a direct child sub-scheduler, disabling it with the supplied reason.
 * The child and its subtree are torn down asynchronously through the usual
 * disable path.
 *
 * Unlike scx_bpf_exit(), no exit code is taken: the child is a separate
 * scheduler with its own exit-code semantics, so a code chosen by the parent
 * would have no defined meaning. The reason string carries the intent.
 *
 * Return 0 on success or -ENODEV if @cgroup_id names no sub-scheduler, which
 * can race with the child detaching on its own and so is not a scheduler error.
 * Naming a sched that exists but is not a direct child aborts the parent.
 */
__printf(2, 0)
__bpf_kfunc s32 scx_bpf_sub_kill_bstr(u64 cgroup_id, எழுத்து *fmt,
				      unsigned long long *data, u32 data__sz,
				      மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	கட்டமைப்பு scx_sched *parent, *child;

	guard(rcu)();

	parent = scx_prog_sched(aux);
	எனில் (unlikely(!parent))
		திரும்பு -ENODEV;

	எனில் (!scx_is_cid_type()) {
		scx_error(parent, "sub-cap kfuncs require a cid-form scheduler");
		திரும்பு -EOPNOTSUPP;
	}

	child = scx_find_sub_sched(cgroup_id);
	எனில் (unlikely(!child))
		திரும்பு -ENODEV;

	எனில் (unlikely(scx_parent(child) != parent)) {
		scx_error(parent, "%s: sub-%llu is not a direct child",
			  parent->cgrp_path, cgroup_id);
		திரும்பு -EINVAL;
	}

	scx_exit_bstr(child, SCX_EXIT_PARENT_KILL, 0, parent, fmt, data, data__sz);
	திரும்பு 0;
}

__bpf_kfunc_end_defs();

#இல்லையெனில்	/* !CONFIG_EXT_SUB_SCHED */

__bpf_kfunc_start_defs();

__bpf_kfunc s32 scx_bpf_sub_grant(u64 cgroup_id, u64 caps,
				  மாறிலி கட்டமைப்பு scx_cmask *cmask__arena,
				  கட்டமைப்பு scx_cmask *denied_out__arena__nullable,
				  மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	திரும்பு -EOPNOTSUPP;
}

__bpf_kfunc வெறுமை scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    மாறிலி கட்டமைப்பு scx_cmask *cmask__arena,
				    மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
}

__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, கட்டமைப்பு scx_cmask *out__arena,
				 மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	திரும்பு -EOPNOTSUPP;
}

__bpf_kfunc s32 scx_bpf_sub_kill_bstr(u64 cgroup_id, எழுத்து *fmt,
				      unsigned long long *data, u32 data__sz,
				      மாறிலி கட்டமைப்பு bpf_prog_aux *aux)
{
	திரும்பு -EOPNOTSUPP;
}

__bpf_kfunc_end_defs();

#endif	/* CONFIG_EXT_SUB_SCHED */
