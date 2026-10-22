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
#ضمن <linux/rhashtable.h>
#ضمن "internal.h"
#ضمن "cid.h"
#ضمن "arena.h"
#ضمن "sub.h"
#ضمن "inlines.h"

#ifdef CONFIG_EXT_SUB_SCHED

/*
 * On while any sub-scheduler exists so that a root-only system doesn't pay for
 * the sub-sched portions of hot paths. See scx_has_subs().
 */
DEFINE_STATIC_مفتاح_FALSE(__scx_has_subs);

/* latched at root enable before any rescue runs */
ساكن s32 scx_rescue_bw_1024;
ساكن s64 scx_rescue_quantum_ns;
ساكن s64 scx_rescue_sat_delta_ns;
ساكن unsigned long scx_rescue_decay_halflife;
ساكن unsigned long scx_rescue_overload_after;

/**
 * scx_skip_subtree_pre - Skip @pos's subtree in a pre-order walk
 * @pos: current position
 * @root: walk root
 *
 * In a walk started by scx_next_descendant_pre(), continue past @pos's subtree:
 * return @pos's next sibling, or the closest ancestor's next sibling, or NULL
 * if @pos's subtree is the last under @root. Same locking rules.
 */
بنية scx_sched *scx_skip_subtree_pre(بنية scx_sched *pos, بنية scx_sched *جذر)
{
	بنية scx_sched *next;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_lock) ||
		       rcu_read_lock_any_held());

	بينما (pos != جذر) {
		next = list_next_or_null_rcu(&scx_parent(pos)->children, &pos->sibling,
					     بنية scx_sched, sibling);
		إذا (next)
			إرجاع next;
		pos = scx_parent(pos);
	}
	إرجاع عدم;
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
بنية scx_sched *scx_next_descendant_pre(بنية scx_sched *pos, بنية scx_sched *جذر)
{
	بنية scx_sched *next;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_lock) ||
		       rcu_read_lock_any_held());

	/* if first iteration, visit @root */
	إذا (!pos)
		إرجاع جذر;

	/* visit the first child if exists */
	next = list_first_or_null_rcu(&pos->children, بنية scx_sched, sibling);
	إذا (next)
		إرجاع next;

	/* no child, visit my or the closest ancestor's next sibling */
	إرجاع scx_skip_subtree_pre(pos, جذر);
}

ساكن بنية scx_sched *scx_find_sub_sched(u64 cgroup_id)
{
	إرجاع rhashtable_lookup(&scx_sched_hash, &cgroup_id,
				 scx_sched_hash_params);
}

فراغ scx_set_task_sched(بنية task_struct *p, بنية scx_sched *sch)
{
	rcu_assign_pointer(p->scx.sched, sch);
}

بنية cgroup *sch_cgroup(بنية scx_sched *sch)
{
	إرجاع sch->cgrp;
}

/* for each descendant of @cgrp including self, set ->scx_sched to @sch */
فراغ set_cgroup_sched(بنية cgroup *cgrp, بنية scx_sched *sch)
{
	بنية cgroup *pos;
	بنية cgroup_subsys_state *css;

	cgroup_for_each_live_descendant_pre(pos, css, cgrp)
		rcu_assign_pointer(pos->scx_sched, sch);
}

ساكن فراغ free_pshard(بنية scx_pshard *pshard)
{
	بنية scx_caps_updated *cu;

	إذا (!pshard)
		إرجاع;
	cu = &pshard->caps_updated;
	إذا (cu->cmask_arena_out)
		scx_arena_free(pshard->sch, cu->cmask_arena_out,
			       struct_size_t(بنية scx_cmask, bits,
					     SCX_CMASK_NR_كلمات(pshard->nr_cids)));
	kfree(pshard);
}

فراغ scx_free_pshards(بنية scx_sched *sch)
{
	s32 si;

	إذا (!sch->pshard)
		إرجاع;
	لكل (si = 0; si < sch->nr_pshards; si++)
		free_pshard(sch->pshard[si]);
	kfree(sch->pshard);
}

ساكن بنية scx_pshard *alloc_pshard(بنية scx_sched *sch, s32 shard_idx, s32 عقدة)
{
	ثابت بنية scx_cid_shard *shard =
		&rcu_dereference_protected(scx_cid_shard_ranges,
					   lockdep_is_held(&scx_enable_mutex))[shard_idx];
	size_t cmask_size = struct_size_t(بنية scx_cmask, bits,
					  SCX_CMASK_NR_كلمات(shard->nr_cids));
	بنية scx_pshard *pshard;
	بنية scx_caps_updated *cu;
	s32 i;

	pshard = kzalloc_عقدة(حجم(*pshard), GFP_KERNEL, عقدة);
	إذا (!pshard)
		إرجاع عدم;

	raw_spin_lock_init(&pshard->lock);
	pshard->sch = sch;
	pshard->base = shard->base_cid;
	pshard->nr_cids = shard->nr_cids;

	لكل (i = 0; i < __SCX_NR_CAPS; i++)
		scx_cmask_init(&pshard->caps[i].cmask, shard->base_cid, shard->nr_cids);

	cu = &pshard->caps_updated;
	raw_spin_lock_init(&cu->lock);
	INIT_LIST_HEAD(&cu->عقدة_in_flight);
	__scx_cmask_init(&cu->cmask, shard->base_cid, shard->nr_cids, SCX_CID_SHARD_MAX_CPUS);

	cu->cmask_arena_out = scx_arena_alloc(sch, cmask_size);
	إذا (!cu->cmask_arena_out) {
		free_pshard(pshard);
		إرجاع عدم;
	}

	scx_cmask_init(cu->cmask_arena_out, shard->base_cid, shard->nr_cids);

	إرجاع pshard;
}

s32 scx_alloc_pshards(بنية scx_sched *sch)
{
	بنية scx_pshard **pshard;
	s32 *shard_عقدة;
	s32 si;

	إذا (!sch->is_cid_type || !sch->arena_pool)
		إرجاع 0;

	shard_عقدة = rcu_dereference_protected(scx_shard_عقدة,
					       lockdep_is_held(&scx_enable_mutex));

	pshard = kzalloc_objs(pshard[0], scx_nr_cid_shards);
	إذا (!pshard)
		إرجاع -ENOMEM;

	لكل (si = 0; si < scx_nr_cid_shards; si++) {
		pshard[si] = alloc_pshard(sch, si, shard_عقدة[si]);
		إذا (!pshard[si]) {
			بينما (--si >= 0)
				free_pshard(pshard[si]);
			kfree(pshard);
			إرجاع -ENOMEM;
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
	إرجاع 0;
}

/*
 * Seed the root's caps fully. Root owns all cids on all caps at enable time.
 * Children acquire caps via scx_bpf_sub_grant().
 */
فراغ scx_init_جذر_caps(بنية scx_sched *sch)
{
	s32 si, i;

	لكل (si = 0; si < sch->nr_pshards; si++) {
		بنية scx_pshard *ps = sch->pshard[si];

		لكل (i = 0; i < __SCX_NR_CAPS; i++)
			scx_cmask_fill(&ps->caps[i].cmask);
	}
}

/* unserved remainder of @rq's rescuee's admitted slice, 0 once fully served */
ساكن s64 scx_rescue_slice_remaining(بنية rq *rq)
{
	s64 served = rq->scx.rescue.curr->se.sum_exec_runtime - rq->scx.rescue.exec_snap;

	إرجاع max(rq->scx.rescue.slice - served, 0);
}

/*
 * Decay @pcpu's rescue usage average in place, halving per the knob-derived
 * halflife, see scx_rescue_set_knobs(). The timestamp advances only by whole
 * halflives.
 */
ساكن u64 scx_rescue_decay_avg(بنية scx_sched_pcpu *pcpu)
{
	unsigned long halflife = scx_rescue_decay_halflife;
	u64 n = div_u64(get_jiffies_64() - pcpu->rescue_avg_at, halflife);

	إذا (n) {
		pcpu->rescue_avg = n < 64 ? pcpu->rescue_avg >> n : 0;
		pcpu->rescue_avg_at += n * halflife;
	}
	إرجاع pcpu->rescue_avg;
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
فراغ scx_rescue_charge(بنية rq *rq, s64 delta_exec)
{
	بنية scx_sched_pcpu *pcpu;

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

	إذا (!scx_rescue_slice_remaining(rq))
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
فراغ scx_rescue_end(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);

	rq->scx.rescue.curr = عدم;
	إذا (list_empty(&rq->scx.rescue.dsq.list))
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
bool scx_rescue_keep(بنية rq *rq, بنية task_struct *p)
{
	s64 remaining = scx_rescue_slice_remaining(rq);

	lockdep_assert_rq_held(rq);

	إذا (!remaining || !(p->scx.flags & SCX_TASK_QUEUED) ||
	    scx_bypassing(scx_task_sched(p), cpu_of(rq)))
		إرجاع false;

	scx_set_task_slice(p, remaining);
	إرجاع true;
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
ساكن فراغ scx_rescue_accrue(بنية rq *rq)
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
	إذا (delta >= scx_rescue_sat_delta_ns)
		rq->scx.rescue.budget = cap;
	وإلا
		rq->scx.rescue.budget =
			min(cap, rq->scx.rescue.budget +
			    ((delta * scx_rescue_bw_1024) >> SCHED_CAPACITY_SHIFT));
}

/*
 * The slice for the next admission - the quantum divided across the stranded
 * tasks so that a crowded queue round-robins on shorter slices.
 */
ساكن s64 scx_rescue_next_slice(بنية rq *rq)
{
	s64 min_slice = max_t(s64, SCX_RESCUE_MIN_SLICE_US * NSEC_PER_USEC, TICK_NSEC);
	u32 depth = rq->scx.rescue.dsq.nr ?: 1;

	إرجاع clamp(div_s64(scx_rescue_quantum_ns, depth), min_slice, scx_rescue_quantum_ns);
}

ساكن فراغ scx_rescue_timer_arm(بنية rq *rq)
{
	بنية timer_list *timer = &rq->scx.rescue.timer;
	s64 delay = scx_rescue_quantum_ns / 4;	/* should be granular enough */

	إذا (timer_pending(timer))
		إرجاع;

	/*
	 * While the head waiter can't be admitted because the bucket is short
	 * of a full quantum, stretch to the full funding delay.
	 */
	إذا (!rq->scx.rescue.curr && rq->scx.rescue.budget < scx_rescue_quantum_ns) {
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
ساكن فراغ scx_rescue_admit(بنية rq *rq, بنية task_struct *p, s64 slice)
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
ساكن bool scx_rescue_try_admit(بنية rq *rq, بنية task_struct *p)
{
	scx_rescue_accrue(rq);

	إذا (!rq->scx.rescue.curr && list_empty(&rq->scx.rescue.dsq.list) &&
	    rq->scx.rescue.budget >= scx_rescue_quantum_ns) {
		scx_rescue_admit(rq, p, scx_rescue_quantum_ns);
		إرجاع true;
	}

	scx_rescue_timer_arm(rq);
	إرجاع false;
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
ساكن فراغ scx_rescue_check_overload(بنية rq *rq)
{
	بنية scx_sched *victim = عدم, *pos;
	بنية task_struct *p;
	صحيح cpu = cpu_of(rq);
	u64 max_avg = 0;
	u32 dur_ms;

	lockdep_assert_rq_held(rq);

	p = list_first_entry_or_null(&rq->scx.rescue.dsq.list, بنية task_struct,
				     scx.dsq_list.عقدة);
	إذا (!p)
		إرجاع;

	/* has the head waiter been queued for longer than the threshold? */
	إذا (time_before(jiffies, p->scx.rescue_at + scx_rescue_overload_after))
		إرجاع;

	/*
	 * Grace period after the last ejection on this cpu - the freed
	 * bandwidth gets one threshold's worth of time to drain the backlog
	 * before another sub is judged.
	 */
	إذا (time_before64(get_jiffies_64(), rq->scx.rescue.kill_at +
			  scx_rescue_overload_after))
		إرجاع;

	list_for_each_entry_rcu(pos, &scx_sched_all, all) {
		u64 avg = scx_rescue_decay_avg(per_cpu_ptr(pos->pcpu, cpu));

		/* skip an already-exiting sub, else the ejection is wasted */
		إذا (pos->level && avg > max_avg &&
		    atomic_read(&pos->exit_kind) == SCX_EXIT_NONE) {
			max_avg = avg;
			victim = pos;
		}
	}
	إذا (!victim)
		إرجاع;

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
ساكن فراغ scx_rescue_timerfn(بنية timer_list *timer)
{
	بنية rq *rq = timer_container_of(rq, timer, scx.rescue.timer);
	بنية task_struct *p;

	guard(rq_lock_irqsave)(rq);

	p = rq->scx.rescue.curr;
	إذا (!p && list_empty(&rq->scx.rescue.dsq.list))
		إرجاع;

	scx_rescue_accrue(rq);
	scx_rescue_check_overload(rq);

	إذا (!p) {
		s64 slice = scx_rescue_next_slice(rq);

		/* no rescue in progress */
		إذا (rq->scx.rescue.budget < scx_rescue_quantum_ns)
			اذهب out_arm;

		/* there's enough budget to start rescuing the next one */
		p = list_first_entry(&rq->scx.rescue.dsq.list, بنية task_struct,
				     scx.dsq_list.عقدة);
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_rescue_admit(rq, p, slice);
		scx_move_local_task_to_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
		إذا (sched_class_above(&ext_sched_class, rq->curr->sched_class))
			resched_curr(rq);
	} وإلا إذا (p->scx.dsq && rq->scx.rescue.budget > 2 * scx_rescue_quantum_ns) {
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
فراغ scx_rescue_flush(بنية rq *rq)
{
	بنية task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	/* end the current rescue */
	إذا (rq->scx.rescue.curr)
		scx_task_slice_ended(rq, rq->scx.rescue.curr);

	/* and flush out all pending ones */
	list_for_each_entry_safe(p, n, &rq->scx.rescue.dsq.list, scx.dsq_list.عقدة) {
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_move_local_task_to_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
	}

	timer_delete(&rq->scx.rescue.timer);
}

فراغ scx_rescue_dump(بنية seq_buf *s, بنية rq *rq)
{
	بنية task_struct *p = rq->scx.rescue.curr;

	scx_dump_سطر(s, "          rescue=%u budget=%lldus rescuing=%s[%d]",
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
ساكن فراغ scx_rescue_check_timeout(بنية scx_sched *sch)
{
	إذا (!scx_rescue_bw_1024 || sch->watchdog_timeout > scx_rescue_overload_after)
		إرجاع;

	pr_warn("sched_ext: %s: watchdog timeout %ums <= rescue overload threshold %ums\n",
		sch->ops.name, jiffies_to_msecs(sch->watchdog_timeout),
		jiffies_to_msecs(scx_rescue_overload_after));
}

/* latch the rescue parameters on root scheduler enable */
فراغ scx_rescue_set_knobs(بنية scx_sched *sch)
{
	s32 bw_ppt = sch->ops.rescue_bandwidth_ppt ?: SCX_RESCUE_DFL_BW_PPT;
	s64 quantum_us = sch->ops.rescue_quantum_us ?: SCX_RESCUE_DFL_QUANTUM_US;
	s64 period_ns;

	إذا (sch->ops.rescue_bandwidth_ppt == SCX_RESCUE_DISABLE) {
		scx_rescue_bw_1024 = 0;
		إرجاع;
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
	إذا (nsecs_to_jiffies(period_ns) > scx_rescue_overload_after / 2)
		pr_warn("sched_ext: %s: rescue funding period %lldms > overload threshold %ums / 2\n",
			sch->ops.name, div_s64(period_ns, NSEC_PER_MSEC),
			jiffies_to_msecs(scx_rescue_overload_after));

	scx_rescue_check_timeout(sch);
}

فراغ scx_rescue_init(بنية rq *rq)
{
	BUG_ON(scx_init_dsq(&rq->scx.rescue.dsq, SCX_DSQ_RESCUE, عدم));
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
بنية scx_dispatch_q *scx_resolve_local_dsq(بنية scx_sched *sch, بنية rq *rq,
					     بنية task_struct *p, u64 *enq_flags)
{
	إذا (!scx_has_subs())
		إرجاع &rq->scx.local_dsq;

	s32 cid = __scx_cpu_to_cid(cpu_of(rq));
	بنية scx_sched *asch = rq->scx.remote_activate_sch ?: sch;
	u64 needed = scx_caps_for_enq(*enq_flags);
	u64 missing;

	/*
	 * On a remote activation the scheduling sched (@asch) differs from
	 * @p's owner (@sch). Check caps against the scheduling sched.
	 */
	إذا (*enq_flags & SCX_ENQ_PREEMPT)
		needed |= scx_caps_for_preempt(asch, rq, *enq_flags);
	missing = scx_missing_caps(asch, cpu_of(rq), needed);

	/* requirements met */
	إذا (likely(!missing))
		إرجاع &rq->scx.local_dsq;

	/*
	 * The task must run on this CPU regardless of caps: the rq is draining
	 * offline (BPF scheduler bypassed), the task is migration-disabled, or a
	 * migration is pending. Admit despite the missing caps and count it.
	 * Refuse preemptions.
	 */
	إذا (unlikely(!scx_rq_online(rq) || is_migration_disabled(p) ||
		     p->migration_pending)) {
		__scx_add_event(sch, SCX_EV_SUB_FORCED_ADMIT, 1);
		*enq_flags &= ~SCX_ENQ_PREEMPT;
		إرجاع &rq->scx.local_dsq;
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
	إذا ((*enq_flags & SCX_ENQ_RESCUE) && likely(scx_rescue_bw_1024)) {
		__scx_add_event(sch, SCX_EV_SUB_RESCUE, 1);
		إذا (scx_rescue_try_admit(rq, p))
			إرجاع &rq->scx.local_dsq;

		/* queueing, the overload trigger measures the wait from here */
		p->scx.rescue_at = jiffies;
		إرجاع &rq->scx.rescue.dsq;
	}

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = cid;

	إرجاع &rq->scx.reject_dsq;
}

/* @p lost the caps needed to stay on @rq's local DSQ? Record reason if so. */
bool scx_task_reenq_on_cap_revoke(بنية rq *rq, بنية task_struct *p)
{
	u64 missing;

	/* migration-disabled tasks and the rescuee are admitted capless */
	إذا (is_migration_disabled(p) || p == scx_rescuee(rq))
		إرجاع false;

	missing = scx_missing_caps(scx_task_sched(p), cpu_of(rq), scx_caps_for_task(p));
	إذا (likely(!missing))
		إرجاع false;

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = __scx_cpu_to_cid(cpu_of(rq));
	إرجاع true;
}

/*
 * Drain @rq->scx.reject_dsq, reenqueueing each task so the BPF re-decides
 * from p->scx.reenq_reason_*.
 *
 * A task can be re-rejected repeatedly. The reenqueue is bounded per task in
 * scx_do_enqueue_task(), which ejects the owning sub past SCX_REENQ_MAX_REPEAT.
 * Rejection can't happen for root.
 */
فراغ scx_reenq_reject(بنية rq *rq)
{
	LIST_HEAD(tasks);
	بنية task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	إذا (!scx_has_subs() || list_empty(&rq->scx.reject_dsq.list))
		إرجاع;

	/*
	 * Move to a private list so a task re-rejected by the
	 * scx_do_enqueue_task() below isn't revisited this round.
	 */
	list_for_each_entry_safe(p, n, &rq->scx.reject_dsq.list, scx.dsq_list.عقدة) {
		/* migration_pending tasks should have bypassed to local DSQ */
		إذا (WARN_ON_ONCE(p->migration_pending))
			استمر;

		scx_wait_dispatching(p);
		scx_dispatch_dequeue(rq, p);

		إذا (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_MASK))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
		p->scx.flags |= SCX_TASK_REENQ_CAP;

		list_add_tail(&p->scx.dsq_list.عقدة, &tasks);
	}

	list_for_each_entry_safe(p, n, &tasks, scx.dsq_list.عقدة) {
		list_del_init(&p->scx.dsq_list.عقدة);

		scx_do_enqueue_task(rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
	}
}

/* record a caps change, see struct scx_caps_updated */
ساكن فراغ caps_updated_record(بنية scx_pshard *ps, ثابت بنية scx_cmask *cids, u64 caps,
				بنية list_head *to_deliver)
{
	بنية scx_caps_updated *cu = &ps->caps_updated;

	guard(raw_spinlock)(&cu->lock);
	scx_cmask_or(&cu->cmask, cids);
	cu->caps |= caps;
	إذا (list_empty(&cu->عقدة_in_flight))
		list_add_tail(&cu->عقدة_in_flight, to_deliver);
}

/* deliver queued caps_updated callbacks, see struct scx_caps_updated */
ساكن فراغ caps_updated_deliver(بنية list_head *to_deliver)
{
	بنية scx_caps_updated *cu, *tmp;

	list_for_each_entry_safe(cu, tmp, to_deliver, عقدة_in_flight) {
		بنية scx_pshard *ps = container_of(cu, بنية scx_pshard, caps_updated);
		بنية scx_sched *sch = ps->sch;

		بينما (true) {
			u64 caps = 0;

			/*
			 * During enable, has_op is set after ops.sub_attach(),
			 * so !has_op means the op is absent or the sched isn't
			 * live yet - e.g. caps grant from ops.sub_attach().
			 * Either way don't consume - leave for
			 * scx_sub_seed_caps() to deliver once live.
			 */
			scoped_guard (raw_spinlock, &cu->lock) {
				إذا (cu->caps && SCX_HAS_OP(sch, sub_caps_updated) &&
				    likely(!READ_ONCE(sch->aborting))) {
					بنية scx_cmask_ref ref;

					caps = cu->caps;
					scx_cmask_ref_init_kern(sch, cu->cmask_arena_out,
								ps->base, ps->nr_cids, &ref);
					scx_cmask_ref_copy(&ref, &cu->cmask);
					scx_cmask_clear(&cu->cmask);
					cu->caps = 0;
				} وإلا {
					list_del_init(&cu->عقدة_in_flight);
				}
			}
			إذا (!caps)
				اكسر;

			/* caps != 0 only when deliverable (has_op, above) */
			SCX_CALL_OP(sch, sub_caps_updated, عدم, cu->cmask_arena_out, caps);
		}
	}
}

/*
 * Deliver caps owed to @sch that couldn't be delivered earlier (e.g. a grant
 * taken during its sub_attach(), before has_op was set). Called once @sch is
 * enabled.
 */
ساكن فراغ scx_sub_seed_caps(بنية scx_sched *sch)
{
	LIST_HEAD(to_deliver);
	s32 si;

	guard(irqsave)();

	لكل (si = 0; si < sch->nr_pshards; si++) {
		بنية scx_pshard *ps = sch->pshard[si];
		بنية scx_caps_updated *cu = &ps->caps_updated;

		scoped_guard (raw_spinlock, &cu->lock) {
			إذا (cu->caps && list_empty(&cu->عقدة_in_flight))
				list_add_tail(&cu->عقدة_in_flight, &to_deliver);
		}
	}
	caps_updated_deliver(&to_deliver);
}

ساكن u64 calc_effective_caps(بنية scx_pshard *ps, s32 cid)
{
	u64 ecaps = 0;
	u32 cap_bit;

	لكل (cap_bit = 0; cap_bit < __SCX_NR_CAPS; cap_bit++)
		إذا (scx_cmask_test(cid, &ps->caps[cap_bit].cmask))
			ecaps |= BIT_U64(cap_bit) | scx_caps_implied(BIT_U64(cap_bit));
	إرجاع ecaps;
}

/**
 * queue_sync_ecaps - Queue ecaps update for a (sch, cid) pair
 * @sch: sched to update
 * @cid: cid to update
 *
 * Queue an ecaps update for @sch's @cid and kick the cpu so that it syncs in
 * dispatch_one().
 */
ساكن فراغ queue_sync_ecaps(بنية scx_sched *sch, s32 cid)
{
	s32 cpu = __scx_cid_to_cpu(cid);
	بنية scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);

	/*
	 * Pairs with smp_mb() in scx_process_sync_ecaps(). Either the check
	 * below sees the node off the list and queues it, or the in-flight sync
	 * sees the caps[] update made before this call.
	 */
	smp_mb();

	/* @cid's pshard->lock excludes concurrent queueing attempts */
	إذا (llist_on_list(&pcpu->ecaps_to_sync_عقدة))
		إرجاع;
	إذا (llist_add(&pcpu->ecaps_to_sync_عقدة, &cpu_rq(cpu)->scx.ecaps_to_sync))
		scx_kick_cpu(sch->ancestors[0], cpu, 0);
}

/* discard @rq's queued ecaps syncs */
ساكن فراغ discard_queued_syncs(بنية rq *rq)
{
	بنية llist_عقدة *pos, *tmp;

	lockdep_assert_rq_held(rq);

	llist_for_each_safe(pos, tmp, llist_del_all(&rq->scx.ecaps_to_sync))
		init_llist_عقدة(pos);
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
فراغ scx_process_sync_ecaps(بنية rq *rq, بنية task_struct *prev)
{
	s32 cpu = cpu_of(rq);
	s32 cid, shard;
	بنية llist_عقدة *batch, *pos, *tmp;
	u64 lost_all = 0;

	lockdep_assert_rq_held(rq);

	إذا (!scx_has_subs() || likely(llist_empty(&rq->scx.ecaps_to_sync)))
		إرجاع;

	/*
	 * ecaps are zeroed while the cpu is inactive and must stay zero.
	 * Discard queued syncs instead of processing them - the
	 * scx_online_ecaps() reseed re-syncs every sched on activation.
	 * cpu_active() clears before the offline zeroing and sets before the
	 * reseed is queued, so this test can neither miss a racing sync nor
	 * eat the reseed.
	 */
	إذا (unlikely(!cpu_active(cpu))) {
		discard_queued_syncs(rq);
		إرجاع;
	}

	/* @cid is valid here: the cpu is active with queued syncs */
	cid = __scx_cpu_to_cid(cpu);
	shard = rcu_dereference_all(scx_cid_to_shard)[cid];

	batch = llist_del_all(&rq->scx.ecaps_to_sync);
	llist_for_each_safe(pos, tmp, batch) {
		بنية scx_sched_pcpu *pcpu =
			container_of(pos, بنية scx_sched_pcpu, ecaps_to_sync_عقدة);
		بنية scx_pshard *ps = pcpu->sch->pshard[shard];
		u64 old, ecaps, lost, gained;

		init_llist_عقدة(pos);

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
		إذا (ecaps != pcpu->reported_ecaps &&
		    SCX_HAS_OP(pcpu->sch, sub_ecaps_updated) &&
		    !scx_bypassing(pcpu->sch, cpu)) {
			بنية scx_dsp_ctx *dspc = &this_cpu_ptr(pcpu->sch->pcpu)->dsp_ctx;

			dspc->rq = rq;
			/* stash @prev so nested dispatches can access it */
			rq->scx.sub_dispatch_prev = prev;
			SCX_CALL_OP(pcpu->sch, sub_ecaps_updated, rq, scx_cpu_arg(cpu),
				    pcpu->reported_ecaps, ecaps);
			rq->scx.sub_dispatch_prev = عدم;
			scx_flush_dispatch_buf(pcpu->sch, rq);
			pcpu->reported_ecaps = ecaps;
		}

		/*
		 * Gaining baseline access owes an update_idle() so the sched
		 * learns the cpu's idle state. Arm the per-rq gate so the next
		 * idle pick flushes it. Losing access drops any pending notify.
		 */
		إذا (gained & SCX_CAP_BASE) {
			pcpu->idle_renotify = true;
			rq->scx.flags |= SCX_RQ_SUB_IDLE_RENOTIFY;
		} وإلا إذا (lost & SCX_CAP_BASE) {
			pcpu->idle_renotify = false;
		}
	}

	/*
	 * Losing a cap can strand already-queued tasks. Schedule a reenq scan
	 * to move the now-capless ones off the local DSQ. The scan tests
	 * against the effective caps and thus must come after the ecaps sync.
	 */
	إذا (lost_all & SCX_CAPS_REENQ_ON_LOSS)
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
فراغ scx_unbypass_replay_ecaps(بنية rq *rq, بنية scx_sched *sch)
{
	s32 cpu = cpu_of(rq);
	بنية scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);
	بنية scx_pshard *ps;
	s32 cid;

	lockdep_assert_rq_held(rq);

	/* root holds every cap and never uses ecaps */
	إذا (!sch->level)
		إرجاع;

	إذا (READ_ONCE(pcpu->ecaps) == pcpu->reported_ecaps)
		إرجاع;

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
فراغ scx_online_ecaps(بنية rq *rq)
{
	بنية scx_sched *جذر, *pos;
	s32 cid, shard;

	/*
	 * Only a live hierarchy can have ecaps to reseed. This also keeps the
	 * table reads below away from an enable that failed before publishing
	 * the tables. A concurrent disable can't retire them, see
	 * handle_hotplug().
	 */
	إذا (!scx_enabled())
		إرجاع;

	guard(rq_lock_irqsave)(rq);

	جذر = scx_جذر_protected();
	cid = __scx_cpu_to_cid(cpu_of(rq));
	shard = rcu_dereference_all(scx_cid_to_shard)[cid];

	scx_for_each_descendant_pre(pos, جذر) {
		بنية scx_pshard *ps;

		/* root holds every cap and never uses ecaps */
		إذا (!pos->level)
			استمر;

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
فراغ scx_offline_ecaps(بنية rq *rq)
{
	s32 cpu = cpu_of(rq);
	بنية scx_sched *جذر, *pos;

	guard(rq_lock_irqsave)(rq);

	جذر = scx_جذر_protected();

	scx_for_each_descendant_pre(pos, جذر) {
		/* root holds every cap and never uses ecaps */
		إذا (!pos->level)
			استمر;

		WRITE_ONCE(per_cpu_ptr(pos->pcpu, cpu)->ecaps, 0);
	}
}

/*
 * @pcpu's sched was unhashed before the grace period, so nothing re-queues its
 * sync node. Remove the node from @rq's pending list so the pcpu can be freed.
 */
فراغ scx_discard_ecaps_to_sync(s32 cpu, بنية scx_sched_pcpu *pcpu)
{
	بنية rq *rq = cpu_rq(cpu);
	بنية llist_عقدة *head = عدم, *tail = عدم;
	بنية llist_عقدة *pos, *tmp;

	/*
	 * llist can't unlink a single node. Take all queued nodes, drop @pcpu's
	 * and resplice the rest. Nodes in the taken batch read as on-list
	 * throughout, so queue_sync_ecaps() stays correct.
	 */
	إذا (llist_on_list(&pcpu->ecaps_to_sync_عقدة)) {
		scoped_guard (rq_lock_irqsave, rq) {
			llist_for_each_safe(pos, tmp, llist_del_all(&rq->scx.ecaps_to_sync)) {
				إذا (pos == &pcpu->ecaps_to_sync_عقدة) {
					init_llist_عقدة(pos);
				} وإلا {
					pos->next = head;
					head = pos;
					إذا (!tail)
						tail = pos;
				}
			}
			إذا (head)
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
	بينما (true) {
		scoped_guard (rq_lock_irqsave, rq) {
			إذا (!llist_on_list(&pcpu->ecaps_to_sync_عقدة))
				إرجاع;
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
فراغ scx_discard_stale_ecaps_syncs(فراغ)
{
	s32 cpu;

	for_each_possible_cpu(cpu) {
		بنية rq *rq = cpu_rq(cpu);

		guard(rq_lock_irqsave)(rq);
		discard_queued_syncs(rq);
	}
}

ساكن DECLARE_WAIT_QUEUE_HEAD(scx_unlink_waitq);

فراغ drain_descendants(بنية scx_sched *sch)
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
ساكن فراغ scx_rehome_task(بنية scx_sched *to, بنية task_struct *p)
{
	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(task_rq(p));

	scoped_guard (sched_change, p, DEQUEUE_SAVE | DEQUEUE_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_set_task_state(p, SCX_TASK_INIT_BEGIN);
		scx_set_task_state(p, SCX_TASK_INIT);
		scx_set_task_sched(p, to);
		scx_set_task_state(p, SCX_TASK_READY);
		إذا (p->sched_class == &ext_sched_class)
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
ساكن فراغ scx_punt_task(بنية scx_sched *to, بنية task_struct *p)
{
	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(task_rq(p));
	WARN_ON_ONCE(!READ_ONCE(to->bypass_depth));

	scoped_guard (sched_change, p, DEQUEUE_SAVE | DEQUEUE_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_set_task_sched(p, to);
	}
}

ساكن فراغ scx_fail_parent(بنية scx_sched *sch,
			    بنية task_struct *failed, s32 fail_code)
{
	بنية scx_sched *parent = scx_parent(sch);
	بنية scx_task_iter sti;
	بنية task_struct *p;

	scx_error(parent, "ops.init_task() failed (%d) for %s[%d] while disabling a sub-scheduler",
		  fail_code, failed->comm, failed->pid);

	/*
	 * Once $parent is bypassed, tasks can be punted into it. This may
	 * cause downstream failures on the BPF side but $parent is dying
	 * anyway.
	 */
	scx_bypass(parent, true);

	scx_task_iter_start(&sti, sch->cgrp);
	بينما ((p = scx_task_iter_next_locked(&sti))) {
		إذا (scx_task_on_sched(parent, p))
			استمر;

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
ساكن s32 scx_cgroup_claim_subtree(بنية scx_sched *sch)
{
	بنية cgroup *sub_cgrp = sch_cgroup(sch);
	بنية cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	بنية scx_sched *parent = scx_parent(sch);
	بنية cgroup_subsys_state *css;
	صحيح ret;

	css_for_each_descendant_pre(css, ecss) {
		بنية task_group *tg = css_tg(css);
		بنية scx_cgroup_init_args args = {
			.weight = tg->scx.weight,
			.bw_period_us = tg->scx.bw_period_us,
			.bw_quota_us = tg->scx.bw_quota_us,
			.bw_burst_us = tg->scx.bw_burst_us,
			.sched_idle = tg->scx.idle,
		};

		إذا (tg->scx.sched != parent ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			استمر;

		إذا (SCX_HAS_OP(sch, cgroup_init)) {
			ret = SCX_CALL_OP_RET(sch, cgroup_init, عدم, css->cgroup, &args);
			إذا (ret) {
				scx_error(sch, "ops.cgroup_init() failed (%d)", ret);
				اذهب err;
			}
		}
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_post(css, ecss) {
		بنية task_group *tg = css_tg(css);

		/*
		 * SUB_INIT is pass 1's progress mark: pass 2 and the err path
		 * must visit exactly the tgs pass 1 inited.
		 */
		إذا (!(tg->scx.flags & SCX_TG_SUB_INIT))
			استمر;

		/* skip the exit if the parent's ops.cgroup_init() failed */
		إذا ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(parent, cgroup_exit))
			SCX_CALL_OP(parent, cgroup_exit, عدم, css->cgroup);
		tg->scx.sched = sch;
		tg->scx.flags |= SCX_TG_INITED;
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}

	إرجاع 0;

err:
	css_for_each_descendant_post(css, ecss) {
		بنية task_group *tg = css_tg(css);

		إذا (!(tg->scx.flags & SCX_TG_SUB_INIT))
			استمر;

		إذا (SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, عدم, css->cgroup);
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}
	إرجاع ret;
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
ساكن فراغ scx_cgroup_return_subtree(بنية scx_sched *sch)
{
	بنية cgroup *sub_cgrp = sch_cgroup(sch);
	بنية cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	بنية scx_sched *parent = scx_parent(sch);
	بنية cgroup_subsys_state *css;
	bool parent_failed = false;
	صحيح ret;

	css_for_each_descendant_post(css, ecss) {
		بنية task_group *tg = css_tg(css);

		إذا (tg->scx.sched != sch ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			استمر;

		/* skip the exit if @sch's ops.cgroup_init() failed for the tg */
		إذا ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, عدم, css->cgroup);
		tg->scx.sched = parent;
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_pre(css, ecss) {
		بنية task_group *tg = css_tg(css);
		بنية scx_cgroup_init_args args = {
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
		إذا (!(tg->scx.flags & SCX_TG_SUB_INIT))
			استمر;
		tg->scx.flags &= ~(SCX_TG_SUB_INIT | SCX_TG_INITED);

		/*
		 * A re-init on $parent failed. The task_groups from here on are
		 * punted: they stay on the dying $parent with INITED clear and
		 * move onward when it disables.
		 */
		إذا (parent_failed)
			استمر;

		إذا (SCX_HAS_OP(parent, cgroup_init)) {
			ret = SCX_CALL_OP_RET(parent, cgroup_init, عدم, css->cgroup, &args);
			إذا (ret) {
				scx_error(parent, "ops.cgroup_init() failed (%d) while disabling a sub-scheduler",
					  ret);
				parent_failed = true;
				استمر;
			}
		}
		tg->scx.flags |= SCX_TG_INITED;
	}
}
#وإلا
ساكن مضمن s32 scx_cgroup_claim_subtree(بنية scx_sched *sch) { إرجاع 0; }
ساكن مضمن فراغ scx_cgroup_return_subtree(بنية scx_sched *sch) {}
#endif

فراغ scx_sub_disable(بنية scx_sched *sch)
{
	بنية scx_sched *parent = scx_parent(sch);
	بنية scx_task_iter sti;
	بنية task_struct *p;
	صحيح ret;

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
	إذا (!sch->linked)
		اذهب dump;

	set_cgroup_sched(sch_cgroup(sch), parent);

	/*
	 * Return the subtree's cgroups before re-homing tasks so that any
	 * ops.init_task() on $parent only sees cgroups it has initialized.
	 */
	scx_cgroup_return_subtree(sch);

	scx_task_iter_start(&sti, sch->cgrp);
	بينما ((p = scx_task_iter_next_locked(&sti))) {
		بنية rq *rq;
		بنية rq_flags rf;

		/* filter out duplicate visits */
		إذا (scx_task_on_sched(parent, p))
			استمر;

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
		ret = __scx_init_task(parent, p, عدم, false);
		إذا (ret) {
			scx_fail_parent(sch, p, ret);
			put_task_struct(p);
			اكسر;
		}

		rq = task_rq_lock(p, &rf);

		إذا (scx_get_task_state(p) == SCX_TASK_DEAD) {
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
			استمر;
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

	إذا (parent->ops.sub_detach && sch->sub_attached) {
		بنية scx_sub_detach_args sub_detach_args = {
			.ops = &sch->ops,
			.cgroup_path = sch->cgrp_path,
		};
		SCX_CALL_OP(parent, sub_detach, عدم,
			    &sub_detach_args);
	}

	scx_log_sched_disable(sch);

	إذا (sch->ops.exit)
		SCX_CALL_OP(sch, exit, عدم, sch->exit_info);

	/*
	 * @sch's non-ops programs such as timers and tracers can fire after
	 * ops.exit(). Now that exit is complete, stop scx_prog_sched() from
	 * resolving to @sch and drain in-flight resolvers.
	 */
	WRITE_ONCE(sch->dead, true);
	synchronize_rcu();

	إذا (sch->sub_kset)
		kobject_del(&sch->sub_kset->kobj);
	/* not added if enable failed before scx_sched_sysfs_add() */
	إذا (sch->kobj.state_in_sysfs)
		kobject_del(&sch->kobj);
}

/* verify that a scheduler can be attached to @cgrp and return the parent */
ساكن بنية scx_sched *find_parent_sched(بنية cgroup *cgrp)
{
	بنية scx_sched *parent = scx_cgroup_sched(cgrp);
	بنية scx_sched *pos;

	lockdep_assert_held(&scx_sched_lock);

	/* can't attach twice to the same cgroup */
	إذا (parent->cgrp == cgrp)
		إرجاع ERR_PTR(-EBUSY);

	/* does $parent allow sub-scheds? */
	إذا (!parent->ops.sub_attach)
		إرجاع ERR_PTR(-EOPNOTSUPP);

	/* can't insert between $parent and its exiting children */
	list_for_each_entry(pos, &parent->children, sibling)
		إذا (cgroup_is_descendant(pos->cgrp, cgrp))
			إرجاع ERR_PTR(-EBUSY);

	إرجاع parent;
}

ساكن bool assert_task_ready_or_enabled(بنية task_struct *p)
{
	u32 state = scx_get_task_state(p);

	بدل (state) {
	حالة SCX_TASK_READY:
	حالة SCX_TASK_ENABLED:
		إرجاع true;
	افتراضي:
		WARN_ONCE(true, "sched_ext: Invalid task state %d for %s[%d] during enabling sub sched",
			  state, p->comm, p->pid);
		إرجاع false;
	}
}

فراغ scx_sub_enable_workfn(بنية kthread_work *work)
{
	بنية scx_enable_cmd *cmd = container_of(work, بنية scx_enable_cmd, work);
	بنية sched_ext_ops *ops = cmd->ops;
	بنية cgroup *cgrp;
	بنية scx_sched *parent, *sch;
	بنية scx_task_iter sti;
	بنية task_struct *p;
	s32 i, ret;

	mutex_lock(&scx_enable_mutex);

	إذا (!scx_enabled()) {
		ret = -ENODEV;
		اذهب out_unlock;
	}

	/* See scx_root_enable_workfn() for the @ops->priv check. */
	إذا (rcu_access_pointer(ops->priv)) {
		ret = -EBUSY;
		اذهب out_unlock;
	}

	cgrp = cgroup_get_from_id(ops->sub_cgroup_id);
	إذا (IS_ERR(cgrp)) {
		ret = PTR_ERR(cgrp);
		اذهب out_unlock;
	}

	raw_spin_lock_irq(&scx_sched_lock);
	parent = find_parent_sched(cgrp);
	إذا (IS_ERR(parent)) {
		raw_spin_unlock_irq(&scx_sched_lock);
		ret = PTR_ERR(parent);
		اذهب out_put_cgrp;
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
	إذا (IS_ERR(sch)) {
		static_branch_dec(&__scx_has_subs);
		ret = PTR_ERR(sch);
		اذهب out_unlock;
	}

	/*
	 * Validate before scx_link_sched() publishes @sch, so an invalid sub
	 * never becomes visible with an unallocated pshard.
	 */
	ret = scx_validate_ops(sch, ops);
	إذا (ret)
		اذهب err_disable;

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
	إذا (ret)
		اذهب err_disable;

	ret = scx_alloc_pshards(sch);
	إذا (ret)
		اذهب err_disable;

	ret = scx_link_sched(sch);
	إذا (ret)
		اذهب err_disable;

	ret = scx_sched_sysfs_add(sch);
	إذا (ret)
		اذهب err_disable;

	إذا (sch->level >= SCX_SUB_MAX_DEPTH) {
		scx_error(sch, "max nesting depth %d violated",
			  SCX_SUB_MAX_DEPTH);
		ret = -EINVAL;
		اذهب err_disable;
	}

	scoped_guard(cpus_read_lock) {
		ret = scx_alloc_kern_arena_objs(sch);
		إذا (ret)
			اذهب err_disable;
	}

	إذا (sch->ops.init) {
		ret = SCX_CALL_OP_RET(sch, init, عدم);
		إذا (ret) {
			ret = scx_ops_sanitize_err(sch, "init", ret);
			scx_error(sch, "ops.init() failed (%d)", ret);
			اذهب err_disable;
		}
		sch->exit_info->flags |= SCX_EFLAG_INITIALIZED;
	}

	بنية scx_sub_attach_args sub_attach_args = {
		.ops = &sch->ops,
		.cgroup_path = sch->cgrp_path,
	};

	ret = SCX_CALL_OP_RET(parent, sub_attach, عدم,
			      &sub_attach_args);
	إذا (ret) {
		ret = scx_ops_sanitize_err(sch, "sub_attach", ret);
		scx_error(sch, "parent rejected (%d)", ret);
		اذهب err_disable;
	}
	sch->sub_attached = true;

	scx_bypass(sch, true);

	لكل (i = SCX_OPI_BEGIN; i < SCX_OPI_END; i++)
		إذا (((فراغ (**)(فراغ))ops)[i])
			set_bit(i, sch->has_op);

	percpu_down_write(&scx_fork_rwsem);
	scx_cgroup_lock();

	/*
	 * Set cgroup->scx_sched's and check CSS_ONLINE. Either we see
	 * !CSS_ONLINE or scx_cgroup_lifetime_notify() sees and shoots us down.
	 */
	set_cgroup_sched(sch_cgroup(sch), sch);
	إذا (!(cgrp->self.flags & CSS_ONLINE)) {
		scx_error(sch, "cgroup is not online");
		ret = -ENODEV;
		اذهب err_unlock_and_disable;
	}

	/*
	 * Take over the subtree's cgroups before any task is claimed,
	 * mirroring root enable's cgroups-before-tasks order.
	 */
	ret = scx_cgroup_claim_subtree(sch);
	إذا (ret)
		اذهب err_unlock_and_disable;

	/*
	 * Initialize tasks for the new child $sch without exiting them for
	 * $parent so that the tasks can always be reverted back to $parent
	 * sched on child init failure.
	 */
	WARN_ON_ONCE(scx_enabling_sub_sched);
	scx_enabling_sub_sched = sch;

	scx_task_iter_start(&sti, sch->cgrp);
	بينما ((p = scx_task_iter_next_locked(&sti))) {
		بنية rq *rq;
		بنية rq_flags rf;

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
		إذا (p->scx.flags & SCX_TASK_SUB_INIT)
			استمر;

		/* @p is pinned by the iter; see scx_sub_disable() */
		get_task_struct(p);

		إذا (!assert_task_ready_or_enabled(p)) {
			ret = -EINVAL;
			اذهب abort;
		}

		scx_task_iter_unlock(&sti);

		/*
		 * As $p is still on $parent, it can't be transitioned to INIT.
		 * Let's worry about task state later. Use __scx_init_task().
		 */
		ret = __scx_init_task(sch, p, عدم, false);
		إذا (ret)
			اذهب abort;

		rq = task_rq_lock(p, &rf);

		إذا (scx_get_task_state(p) == SCX_TASK_DEAD) {
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
			استمر;
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
	بينما ((p = scx_task_iter_next_locked(&sti))) {
		/*
		 * Use clearing of %SCX_TASK_SUB_INIT to detect and skip
		 * duplicate iterations.
		 */
		إذا (!(p->scx.flags & SCX_TASK_SUB_INIT))
			استمر;

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
			إذا (p->sched_class == &ext_sched_class)
				scx_enable_task(sch, p);

			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);

	scx_enabling_sub_sched = عدم;

	scx_cgroup_unlock();
	percpu_up_write(&scx_fork_rwsem);

	scx_bypass(sch, false);

	/* @sch is enabled; deliver any caps owed since its sub_attach() */
	scx_sub_seed_caps(sch);

	pr_info("sched_ext: BPF sub-scheduler \"%s\" enabled\n", sch->ops.name);
	kobject_uevent(&sch->kobj, KOBJ_ADD);
	ret = 0;
	اذهب out_unlock;

out_put_cgrp:
	cgroup_put(cgrp);
out_unlock:
	mutex_unlock(&scx_enable_mutex);
	cmd->ret = ret;
	إرجاع;

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
	بينما ((p = scx_task_iter_next_locked(&sti))) {
		إذا (p->scx.flags & SCX_TASK_SUB_INIT) {
			scx_sub_init_cancel_task(sch, p);
			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);
	scx_enabling_sub_sched = عدم;
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
ساكن s32 scx_cgroup_task_migrating(بنية cgroup_task_migrate_ctx *ctx)
{
	بنية task_struct *p = ctx->task;
	بنية scx_sched *to;
	صحيح ret;

	/*
	 * Cleared under scx_cgroup_lock() before root disable starts tearing
	 * down tasks. As cgroup_mutex is held, a set flag guarantees that the
	 * teardown loop is not running concurrently.
	 */
	إذا (!scx_cgroup_enabled)
		إرجاع NOTIFY_OK;

	to = scx_cgroup_sched(ctx->dst_dcgrp);
	إذا (scx_task_on_sched(to, p))
		إرجاع NOTIFY_OK;

	ret = __scx_init_task(to, p, ctx->dst_dcgrp, false);
	إذا (ret)
		إرجاع notifier_from_errno(ret);

	إرجاع NOTIFY_OK;
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
ساكن فراغ scx_cgroup_task_migrated(بنية cgroup_task_migrate_ctx *ctx)
{
	بنية task_struct *p = ctx->task;
	بنية scx_sched *to;
	بنية rq *rq;
	بنية rq_flags rf;

	إذا (!scx_cgroup_enabled)
		إرجاع;

	to = scx_cgroup_sched(ctx->dst_dcgrp);
	إذا (scx_task_on_sched(to, p))
		إرجاع;

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
ساكن فراغ scx_cgroup_task_migrate_canceled(بنية cgroup_task_migrate_ctx *ctx)
{
	بنية task_struct *p = ctx->task;
	بنية scx_sched *to;
	بنية rq *rq;
	بنية rq_flags rf;

	إذا (!scx_cgroup_enabled)
		إرجاع;

	to = scx_cgroup_sched(ctx->dst_dcgrp);
	إذا (scx_task_on_sched(to, p))
		إرجاع;

	rq = task_rq_lock(p, &rf);
	scx_sub_init_cancel_task(to, p);
	task_rq_unlock(rq, p, &rf);
}

ساكن s32 scx_cgroup_lifetime_notify(بنية notifier_block *nb,
				      unsigned long action, فراغ *data)
{
	بنية cgroup *cgrp = data;
	بنية cgroup *parent = cgroup_parent(cgrp);
	بنية scx_sched *sch;

	إذا (!cgroup_on_dfl(cgrp))
		إرجاع NOTIFY_OK;

	بدل (action) {
	حالة CGROUP_LIFETIME_ONLINE:
		/* inherit ->scx_sched from $parent */
		إذا (parent)
			rcu_assign_pointer(cgrp->scx_sched, scx_cgroup_sched(parent));
		اكسر;
	حالة CGROUP_LIFETIME_OFFLINE:
		/* if there is a sched attached, shoot it down */
		sch = scx_cgroup_sched(cgrp);
		إذا (sch && sch->cgrp == cgrp)
			scx_exit(sch, SCX_EXIT_UNREG_KERN,
				 SCX_ECODE_RSN_CGROUP_OFFLINE,
				 "cgroup %llu going offline", cgroup_id(cgrp));
		اكسر;
	}

	إرجاع NOTIFY_OK;
}

ساكن بنية notifier_block scx_cgroup_lifetime_nb = {
	.notifier_call = scx_cgroup_lifetime_notify,
};

ساكن s32 scx_cgroup_task_notify(بنية notifier_block *nb,
				  unsigned long action, فراغ *data)
{
	بنية cgroup_task_migrate_ctx *ctx = data;

	بدل (action) {
	حالة CGROUP_TASK_MIGRATING:
		إرجاع scx_cgroup_task_migrating(ctx);
	حالة CGROUP_TASK_MIGRATED:
		scx_cgroup_task_migrated(ctx);
		اكسر;
	حالة CGROUP_TASK_MIGRATE_CANCELED:
		scx_cgroup_task_migrate_canceled(ctx);
		اكسر;
	}

	إرجاع NOTIFY_OK;
}

ساكن بنية notifier_block scx_cgroup_task_nb = {
	.notifier_call = scx_cgroup_task_notify,
};

ساكن s32 __init scx_cgroup_notifier_init(فراغ)
{
	s32 ret;

	ret = blocking_notifier_chain_register(&cgroup_lifetime_notifier,
					       &scx_cgroup_lifetime_nb);
	إذا (ret)
		إرجاع ret;

	إرجاع blocking_notifier_chain_register(&cgroup_task_notifier,
						&scx_cgroup_task_nb);
}
core_initcall(scx_cgroup_notifier_init);

ساكن فراغ scx_pstack_recursion(بنية bpf_prog *prog, ثابت حرف *op)
{
	بنية scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(prog->aux);
	إذا (unlikely(!sch))
		إرجاع;

	scx_error(sch, "%s recursion detected", op);
}

فراغ scx_pstack_recursion_on_dispatch(بنية bpf_prog *prog)
{
	scx_pstack_recursion(prog, "dispatch");
}

فراغ scx_pstack_recursion_on_caps_updated(بنية bpf_prog *prog)
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
__bpf_kfunc bool scx_bpf_sub_dispatch(u64 cgroup_id, ثابت بنية bpf_prog_aux *aux)
{
	بنية rq *rq = scx_locked_rq();
	بنية scx_sched *parent, *child;

	guard(rcu)();
	parent = scx_prog_sched(aux);
	إذا (unlikely(!parent))
		إرجاع false;

	child = scx_find_sub_sched(cgroup_id);

	إذا (unlikely(!child))
		إرجاع false;

	إذا (unlikely(scx_parent(child) != parent)) {
		scx_error(parent, "trying to dispatch a distant sub-sched on cgroup %llu",
			  cgroup_id);
		إرجاع false;
	}

	/*
	 * Skip a child that does not effectively hold the base cap on this cpu:
	 * its inserts would only be rejected. ecaps are synced at the top of
	 * dispatch_one() before dispatch, so this reflects the in-effect state.
	 */
	إذا (scx_missing_caps(child, cpu_of(rq), SCX_CAP_BASE))
		إرجاع false;

	إرجاع scx_dispatch_sched(child, rq, rq->scx.sub_dispatch_prev, true) !=
		SCX_DSP_NONE;
}

/* Validate common inputs. On success, *parent_out and *child_out are set. */
ساكن s32 sub_cap_preamble(u64 cgroup_id, u64 caps, ثابت بنية bpf_prog_aux *aux,
			    بنية scx_sched **parent_out, بنية scx_sched **child_out)
{
	بنية scx_sched *parent, *child;

	parent = scx_prog_sched(aux);
	إذا (unlikely(!parent))
		إرجاع -ENODEV;

	إذا (!scx_is_cid_type()) {
		scx_error(parent, "sub-cap kfuncs require a cid-form scheduler");
		إرجاع -EOPNOTSUPP;
	}

	child = scx_find_sub_sched(cgroup_id);
	إذا (unlikely(!child))
		إرجاع -ENODEV;

	إذا (unlikely(scx_parent(child) != parent)) {
		scx_error(parent, "%s: sub-%llu is not a direct child",
			  parent->cgrp_path, cgroup_id);
		إرجاع -EINVAL;
	}

	إذا (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(parent, "invalid caps 0x%llx", caps);
		إرجاع -EINVAL;
	}

	*parent_out = parent;
	*child_out = child;
	إرجاع 0;
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
				  ثابت بنية scx_cmask *cmask__arena,
				  بنية scx_cmask *denied_out__arena__nullable,
				  ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_cmask_ref ref, denied_ref;
	بنية scx_sched *parent, *child;
	bool any_denied = false;
	LIST_HEAD(to_deliver);
	s32 si, ret;

	guard(irqsave)();

	ret = sub_cap_preamble(cgroup_id, caps, aux, &parent, &child);
	إذا (ret)
		إرجاع ret;

	ret = scx_cmask_ref_init(parent, cmask__arena, &ref);
	إذا (ret) {
		scx_error(parent, "invalid cmask (%d)", ret);
		إرجاع ret;
	}

	إذا (denied_out__arena__nullable) {
		ret = scx_cmask_ref_init(parent, denied_out__arena__nullable, &denied_ref);
		إذا (ret) {
			scx_error(parent, "invalid denied_out (%d)", ret);
			إرجاع ret;
		}
	}

	/* apply the grant one shard at a time */
	لكل (si = ref.shard_first; si < ref.shard_end; si++) {
		SCX_CMASK_DEFINE_SHARD(slice, 0, SCX_CID_SHARD_MAX_CPUS);
		بنية scx_pshard *pps = parent->pshard[si];
		بنية scx_pshard *cps = child->pshard[si];
		u64 granted_caps = 0;
		u32 cap_bit;

		scx_cmask_ref_shard(&ref, si, slice);
		إذا (scx_cmask_empty(slice))
			استمر;

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
				بنية scx_cmask *ccm = &cps->caps[cap_bit].cmask;

				scx_cmask_copy(delta, granted_cids);
				scx_cmask_andnot(delta, ccm);
				إذا (scx_cmask_empty(delta))
					استمر;

				scx_cmask_or(ccm, delta);
				scx_cmask_or(changed_cids, delta);
				granted_caps |= BIT_U64(cap_bit);
			}

			إذا (granted_caps) {
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
		إذا (!scx_cmask_subset(slice, granted_cids)) {
			any_denied = true;
			إذا (denied_out__arena__nullable) {
				SCX_CMASK_DEFINE_SHARD(denied, slice->base, slice->nr_cids);

				scx_cmask_copy(denied, slice);
				scx_cmask_andnot(denied, granted_cids);
				scx_cmask_ref_or(&denied_ref, denied);
			}
		}
	}

	caps_updated_deliver(&to_deliver);

	إرجاع any_denied ? -EPERM : 0;
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
__bpf_kfunc فراغ scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    ثابت بنية scx_cmask *cmask__arena,
				    ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_cmask_ref ref;
	بنية scx_sched *parent, *child, *pos;
	LIST_HEAD(to_deliver);
	s32 si, ret;

	guard(irqsave)();

	إذا (sub_cap_preamble(cgroup_id, caps, aux, &parent, &child))
		إرجاع;

	ret = scx_cmask_ref_init(parent, cmask__arena, &ref);
	إذا (ret) {
		scx_error(parent, "invalid cmask (%d)", ret);
		إرجاع;
	}

	/* per-shard, walk child's subtree and clear @caps */
	لكل (si = ref.shard_first; si < ref.shard_end; si++) {
		SCX_CMASK_DEFINE_SHARD(slice, 0, SCX_CID_SHARD_MAX_CPUS);

		scx_cmask_ref_shard(&ref, si, slice);
		إذا (scx_cmask_empty(slice))
			استمر;

		/*
		 * Pre-order with subtree skip: a descendant that cleared
		 * nothing means no descendant of it can hold @caps on these
		 * cids either.
		 */
		guard(raw_spinlock)(&parent->pshard[si]->lock);
		pos = scx_next_descendant_pre(عدم, child);
		بينما (pos) {
			بنية scx_pshard *ps = pos->pshard[si];
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
					بنية scx_cmask *cm = &ps->caps[cap_bit].cmask;

					scx_cmask_copy(delta, cm);
					scx_cmask_and(delta, slice);
					إذا (scx_cmask_empty(delta))
						استمر;

					scx_cmask_andnot(cm, delta);
					scx_cmask_or(changed_cids, delta);
					revoked_caps |= BIT_U64(cap_bit);
				}

				إذا (revoked_caps) {
					s32 cid;

					caps_updated_record(ps, changed_cids, revoked_caps,
							    &to_deliver);
					scx_cmask_for_each_cid(cid, changed_cids)
						queue_sync_ecaps(pos, cid);
				}
			}

			إذا (revoked_caps)
				pos = scx_next_descendant_pre(pos, child);
			وإلا
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
__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, بنية scx_cmask *out__arena,
				 ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_cmask_ref ref;
	بنية scx_sched *sch, *target;
	بنية scx_pshard **pshard;
	s32 si, ret;

	guard(irqsave)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		إرجاع -ENODEV;

	إذا (!scx_is_cid_type()) {
		scx_error(sch, "sub-cap kfuncs require a cid-form scheduler");
		إرجاع -EOPNOTSUPP;
	}

	إذا (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(sch, "invalid caps 0x%llx", caps);
		إرجاع -EINVAL;
	}

	/* @cgroup_id 0 reads self, otherwise a direct child */
	إذا (cgroup_id) {
		target = scx_find_sub_sched(cgroup_id);
		إذا (unlikely(!target))
			إرجاع -ENODEV;
		إذا (unlikely(scx_parent(target) != sch)) {
			scx_error(sch, "%s: sub-%llu is not a direct child",
				  sch->cgrp_path, cgroup_id);
			إرجاع -EINVAL;
		}
	} وإلا {
		target = sch;
	}

	/*
	 * The target's caps storage may not be set up yet (e.g. a self-read
	 * during ops.init_cids()). Pairs with the publish in
	 * scx_alloc_pshards(): a non-NULL pshard has every element set and the
	 * acquire also orders the cid table reads below against it.
	 */
	pshard = smp_load_acquire(&target->pshard);
	إذا (unlikely(!pshard)) {
		scx_error(sch, "scx_bpf_sub_caps() called before caps storage is initialized");
		إرجاع -ENODEV;
	}

	ret = scx_cmask_ref_init(sch, out__arena, &ref);
	إذا (ret) {
		scx_error(sch, "invalid out (%d)", ret);
		إرجاع ret;
	}

	لكل (si = ref.shard_first; si < ref.shard_end; si++) {
		ثابت بنية scx_cid_shard *shard =
			&rcu_dereference_all(scx_cid_shard_ranges)[si];
		SCX_CMASK_DEFINE_SHARD(local_out, shard->base_cid, shard->nr_cids);
		u32 cap_bit;

		scx_for_each_cap_bit(cap_bit, caps)
			scx_cmask_or(local_out, &pshard[si]->caps[cap_bit].cmask);
		scx_cmask_ref_copy(&ref, local_out);
	}
	إرجاع 0;
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
__bpf_kfunc s32 scx_bpf_sub_kill_bstr(u64 cgroup_id, حرف *fmt,
				      unsigned long long *data, u32 data__sz,
				      ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *parent, *child;

	guard(rcu)();

	parent = scx_prog_sched(aux);
	إذا (unlikely(!parent))
		إرجاع -ENODEV;

	إذا (!scx_is_cid_type()) {
		scx_error(parent, "sub-cap kfuncs require a cid-form scheduler");
		إرجاع -EOPNOTSUPP;
	}

	child = scx_find_sub_sched(cgroup_id);
	إذا (unlikely(!child))
		إرجاع -ENODEV;

	إذا (unlikely(scx_parent(child) != parent)) {
		scx_error(parent, "%s: sub-%llu is not a direct child",
			  parent->cgrp_path, cgroup_id);
		إرجاع -EINVAL;
	}

	scx_exit_bstr(child, SCX_EXIT_PARENT_KILL, 0, parent, fmt, data, data__sz);
	إرجاع 0;
}

__bpf_kfunc_end_defs();

#وإلا	/* !CONFIG_EXT_SUB_SCHED */

__bpf_kfunc_start_defs();

__bpf_kfunc s32 scx_bpf_sub_grant(u64 cgroup_id, u64 caps,
				  ثابت بنية scx_cmask *cmask__arena,
				  بنية scx_cmask *denied_out__arena__nullable,
				  ثابت بنية bpf_prog_aux *aux)
{
	إرجاع -EOPNOTSUPP;
}

__bpf_kfunc فراغ scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    ثابت بنية scx_cmask *cmask__arena,
				    ثابت بنية bpf_prog_aux *aux)
{
}

__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, بنية scx_cmask *out__arena,
				 ثابت بنية bpf_prog_aux *aux)
{
	إرجاع -EOPNOTSUPP;
}

__bpf_kfunc s32 scx_bpf_sub_kill_bstr(u64 cgroup_id, حرف *fmt,
				      unsigned long long *data, u32 data__sz,
				      ثابت بنية bpf_prog_aux *aux)
{
	إرجاع -EOPNOTSUPP;
}

__bpf_kfunc_end_defs();

#endif	/* CONFIG_EXT_SUB_SCHED */
