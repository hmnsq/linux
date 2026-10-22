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
#అన్తర్భావ <linux/rhashtable.h>
#అన్తర్భావ "internal.h"
#అన్తర్భావ "cid.h"
#అన్తర్భావ "arena.h"
#అన్తర్భావ "sub.h"
#అన్తర్భావ "inlines.h"

#నిర్వచితమైతే CONFIG_EXT_SUB_SCHED

/*
 * On while any sub-scheduler exists so that a root-only system doesn't pay for
 * the sub-sched portions of hot paths. See scx_has_subs().
 */
DEFINE_STATIC_కీలకం_FALSE(__scx_has_subs);

/* latched at root enable before any rescue runs */
స్థిర s32 scx_rescue_bw_1024;
స్థిర s64 scx_rescue_కాలాంశం_ns;
స్థిర s64 scx_rescue_sat_delta_ns;
స్థిర గుర్తులేని దీర్ఘ scx_rescue_decay_halflife;
స్థిర గుర్తులేని దీర్ఘ scx_rescue_overload_after;

/**
 * scx_skip_subtree_pre - Skip @pos's subtree in a pre-order walk
 * @pos: current position
 * @root: walk root
 *
 * In a walk started by scx_next_descendant_pre(), continue past @pos's subtree:
 * return @pos's next sibling, or the closest ancestor's next sibling, or NULL
 * if @pos's subtree is the last under @root. Same locking rules.
 */
నిర్మాణం scx_sched *scx_skip_subtree_pre(నిర్మాణం scx_sched *pos, నిర్మాణం scx_sched *మూలం)
{
	నిర్మాణం scx_sched *తదుపరి;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_తాళం) ||
		       rcu_read_తాళం_any_held());

	అయితే (pos != మూలం) {
		తదుపరి = list_తదుపరి_or_null_rcu(&scx_మాతృ(pos)->children, &pos->sibling,
					     నిర్మాణం scx_sched, sibling);
		ఒకవేళ (తదుపరి)
			తిరిగి తదుపరి;
		pos = scx_మాతృ(pos);
	}
	తిరిగి ఖాళీ;
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
నిర్మాణం scx_sched *scx_తదుపరి_descendant_pre(నిర్మాణం scx_sched *pos, నిర్మాణం scx_sched *మూలం)
{
	నిర్మాణం scx_sched *తదుపరి;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_తాళం) ||
		       rcu_read_తాళం_any_held());

	/* if first iteration, visit @root */
	ఒకవేళ (!pos)
		తిరిగి మూలం;

	/* visit the first child if exists */
	తదుపరి = list_మొదటి_or_null_rcu(&pos->children, నిర్మాణం scx_sched, sibling);
	ఒకవేళ (తదుపరి)
		తిరిగి తదుపరి;

	/* no child, visit my or the closest ancestor's next sibling */
	తిరిగి scx_skip_subtree_pre(pos, మూలం);
}

స్థిర నిర్మాణం scx_sched *scx_కనుగొను_sub_sched(u64 cgroup_id)
{
	తిరిగి rhashtable_lookup(&scx_sched_hash, &cgroup_id,
				 scx_sched_hash_params);
}

శూన్య scx_సమితి_task_sched(నిర్మాణం task_struct *p, నిర్మాణం scx_sched *sch)
{
	rcu_కేటాయించు_సూచిక(p->scx.sched, sch);
}

నిర్మాణం cgroup *sch_cgroup(నిర్మాణం scx_sched *sch)
{
	తిరిగి sch->cgrp;
}

/* for each descendant of @cgrp including self, set ->scx_sched to @sch */
శూన్య సమితి_cgroup_sched(నిర్మాణం cgroup *cgrp, నిర్మాణం scx_sched *sch)
{
	నిర్మాణం cgroup *pos;
	నిర్మాణం cgroup_subsys_state *css;

	cgroup_for_each_సజీవ_descendant_pre(pos, css, cgrp)
		rcu_కేటాయించు_సూచిక(pos->scx_sched, sch);
}

స్థిర శూన్య free_pshard(నిర్మాణం scx_pshard *pshard)
{
	నిర్మాణం scx_caps_updated *cu;

	ఒకవేళ (!pshard)
		తిరిగి;
	cu = &pshard->caps_updated;
	ఒకవేళ (cu->cmask_arena_బహిర్గతం)
		scx_arena_free(pshard->sch, cu->cmask_arena_బహిర్గతం,
			       struct_కొలత_t(నిర్మాణం scx_cmask, బిట్లు,
					     SCX_CMASK_NR_పదాలు(pshard->nr_cids)));
	kfree(pshard);
}

శూన్య scx_free_pshards(నిర్మాణం scx_sched *sch)
{
	s32 si;

	ఒకవేళ (!sch->pshard)
		తిరిగి;
	కొరకు (si = 0; si < sch->nr_pshards; si++)
		free_pshard(sch->pshard[si]);
	kfree(sch->pshard);
}

స్థిర నిర్మాణం scx_pshard *alloc_pshard(నిర్మాణం scx_sched *sch, s32 shard_idx, s32 కణుపు)
{
	స్థిరరాశి నిర్మాణం scx_cid_shard *shard =
		&rcu_dereference_protected(scx_cid_shard_ranges,
					   lockdep_is_held(&scx_enable_mutex))[shard_idx];
	కొలత_t cmask_కొలత = struct_కొలత_t(నిర్మాణం scx_cmask, బిట్లు,
					  SCX_CMASK_NR_పదాలు(shard->nr_cids));
	నిర్మాణం scx_pshard *pshard;
	నిర్మాణం scx_caps_updated *cu;
	s32 i;

	pshard = kzalloc_కణుపు(పరిమాణం(*pshard), GFP_KERNEL, కణుపు);
	ఒకవేళ (!pshard)
		తిరిగి ఖాళీ;

	raw_spin_తాళం_init(&pshard->తాళం);
	pshard->sch = sch;
	pshard->పునాది = shard->పునాది_cid;
	pshard->nr_cids = shard->nr_cids;

	కొరకు (i = 0; i < __SCX_NR_CAPS; i++)
		scx_cmask_init(&pshard->caps[i].cmask, shard->పునాది_cid, shard->nr_cids);

	cu = &pshard->caps_updated;
	raw_spin_తాళం_init(&cu->తాళం);
	INIT_LIST_తల(&cu->కణుపు_లోపల_flight);
	__scx_cmask_init(&cu->cmask, shard->పునాది_cid, shard->nr_cids, SCX_CID_SHARD_MAX_CPUS);

	cu->cmask_arena_బహిర్గతం = scx_arena_alloc(sch, cmask_కొలత);
	ఒకవేళ (!cu->cmask_arena_బహిర్గతం) {
		free_pshard(pshard);
		తిరిగి ఖాళీ;
	}

	scx_cmask_init(cu->cmask_arena_బహిర్గతం, shard->పునాది_cid, shard->nr_cids);

	తిరిగి pshard;
}

s32 scx_alloc_pshards(నిర్మాణం scx_sched *sch)
{
	నిర్మాణం scx_pshard **pshard;
	s32 *shard_కణుపు;
	s32 si;

	ఒకవేళ (!sch->is_cid_type || !sch->arena_pool)
		తిరిగి 0;

	shard_కణుపు = rcu_dereference_protected(scx_shard_కణుపు,
					       lockdep_is_held(&scx_enable_mutex));

	pshard = kzalloc_objs(pshard[0], scx_nr_cid_shards);
	ఒకవేళ (!pshard)
		తిరిగి -ENOMEM;

	కొరకు (si = 0; si < scx_nr_cid_shards; si++) {
		pshard[si] = alloc_pshard(sch, si, shard_కణుపు[si]);
		ఒకవేళ (!pshard[si]) {
			అయితే (--si >= 0)
				free_pshard(pshard[si]);
			kfree(pshard);
			తిరిగి -ENOMEM;
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
	తిరిగి 0;
}

/*
 * Seed the root's caps fully. Root owns all cids on all caps at enable time.
 * Children acquire caps via scx_bpf_sub_grant().
 */
శూన్య scx_init_మూలం_caps(నిర్మాణం scx_sched *sch)
{
	s32 si, i;

	కొరకు (si = 0; si < sch->nr_pshards; si++) {
		నిర్మాణం scx_pshard *ps = sch->pshard[si];

		కొరకు (i = 0; i < __SCX_NR_CAPS; i++)
			scx_cmask_fill(&ps->caps[i].cmask);
	}
}

/* unserved remainder of @rq's rescuee's admitted slice, 0 once fully served */
స్థిర s64 scx_rescue_ముక్క_మిగిలిన(నిర్మాణం rq *rq)
{
	s64 served = rq->scx.rescue.curr->se.కూడిక_exec_runtime - rq->scx.rescue.exec_snap;

	తిరిగి max(rq->scx.rescue.ముక్క - served, 0);
}

/*
 * Decay @pcpu's rescue usage average in place, halving per the knob-derived
 * halflife, see scx_rescue_set_knobs(). The timestamp advances only by whole
 * halflives.
 */
స్థిర u64 scx_rescue_decay_avg(నిర్మాణం scx_sched_pcpu *pcpu)
{
	గుర్తులేని దీర్ఘ halflife = scx_rescue_decay_halflife;
	u64 n = div_u64(get_jiffies_64() - pcpu->rescue_avg_at, halflife);

	ఒకవేళ (n) {
		pcpu->rescue_avg = n < 64 ? pcpu->rescue_avg >> n : 0;
		pcpu->rescue_avg_at += n * halflife;
	}
	తిరిగి pcpu->rescue_avg;
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
శూన్య scx_rescue_charge(నిర్మాణం rq *rq, s64 delta_exec)
{
	నిర్మాణం scx_sched_pcpu *pcpu;

	lockdep_assert_rq_held(rq);

	/*
	 * A rescue slice is bounded by one quantum and tick-driven expiry can
	 * overshoot by up to a tick. Clamp to avoid wild over-charges on VMs.
	 */
	delta_exec = min_t(s64, delta_exec, scx_rescue_కాలాంశం_ns + TICK_NSEC);

	rq->scx.rescue.budget -= delta_exec;

	/* per-cpu usage average feeds the overload victim pick */
	pcpu = చొప్పున_cpu_ptr(scx_task_sched(rq->curr)->pcpu, cpu_యొక్క(rq));
	pcpu->rescue_avg = scx_rescue_decay_avg(pcpu) + delta_exec;

	ఒకవేళ (!scx_rescue_ముక్క_మిగిలిన(rq))
		scx_task_ముక్క_ended(rq, rq->scx.rescue.curr);
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
శూన్య scx_rescue_చివర(నిర్మాణం rq *rq)
{
	lockdep_assert_rq_held(rq);

	rq->scx.rescue.curr = ఖాళీ;
	ఒకవేళ (list_ఖాళీలు(&rq->scx.rescue.dsq.list))
		rq->scx.rescue.budget = min(rq->scx.rescue.budget, scx_rescue_కాలాంశం_ns);
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
bool scx_rescue_keep(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	s64 మిగిలిన = scx_rescue_ముక్క_మిగిలిన(rq);

	lockdep_assert_rq_held(rq);

	ఒకవేళ (!మిగిలిన || !(p->scx.flags & SCX_TASK_QUEUED) ||
	    scx_bypassing(scx_task_sched(p), cpu_యొక్క(rq)))
		తిరిగి false;

	scx_సమితి_task_ముక్క(p, మిగిలిన);
	తిరిగి true;
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
స్థిర శూన్య scx_rescue_accrue(నిర్మాణం rq *rq)
{
	bool లోపల_session = rq->scx.rescue.curr || !list_ఖాళీలు(&rq->scx.rescue.dsq.list);
	s64 cap = లోపల_session ? 3 * scx_rescue_కాలాంశం_ns : scx_rescue_కాలాంశం_ns;
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
	ఒకవేళ (delta >= scx_rescue_sat_delta_ns)
		rq->scx.rescue.budget = cap;
	లేకపోతే
		rq->scx.rescue.budget =
			min(cap, rq->scx.rescue.budget +
			    ((delta * scx_rescue_bw_1024) >> SCHED_సామర్థ్యం_జరుపు));
}

/*
 * The slice for the next admission - the quantum divided across the stranded
 * tasks so that a crowded queue round-robins on shorter slices.
 */
స్థిర s64 scx_rescue_తదుపరి_ముక్క(నిర్మాణం rq *rq)
{
	s64 min_ముక్క = max_t(s64, SCX_RESCUE_MIN_ముక్క_US * NSEC_చొప్పున_USEC, TICK_NSEC);
	u32 లోతు = rq->scx.rescue.dsq.nr ?: 1;

	తిరిగి clamp(div_s64(scx_rescue_కాలాంశం_ns, లోతు), min_ముక్క, scx_rescue_కాలాంశం_ns);
}

స్థిర శూన్య scx_rescue_timer_arm(నిర్మాణం rq *rq)
{
	నిర్మాణం timer_list *timer = &rq->scx.rescue.timer;
	s64 delay = scx_rescue_కాలాంశం_ns / 4;	/* should be granular enough */

	ఒకవేళ (timer_pending(timer))
		తిరిగి;

	/*
	 * While the head waiter can't be admitted because the bucket is short
	 * of a full quantum, stretch to the full funding delay.
	 */
	ఒకవేళ (!rq->scx.rescue.curr && rq->scx.rescue.budget < scx_rescue_కాలాంశం_ns) {
		s64 deficit = scx_rescue_కాలాంశం_ns - rq->scx.rescue.budget;

		delay = max(delay,
			    div_s64(deficit << SCHED_సామర్థ్యం_జరుపు, scx_rescue_bw_1024));
	}

	/* +1 rounds up so the beat is due by the time the timer fires */
	timer->expires = jiffies + nsecs_గమ్యం_jiffies(delay) + 1;
	కూడు_timer_on(timer, cpu_యొక్క(rq));
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
స్థిర శూన్య scx_rescue_admit(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, s64 ముక్క)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->scx.rescue.curr);

	rq->scx.rescue.curr = p;
	rq->scx.rescue.ముక్క = ముక్క;
	rq->scx.rescue.exec_snap = p->se.కూడిక_exec_runtime;
	scx_సమితి_task_ముక్క(p, ముక్క);
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
స్థిర bool scx_rescue_try_admit(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	scx_rescue_accrue(rq);

	ఒకవేళ (!rq->scx.rescue.curr && list_ఖాళీలు(&rq->scx.rescue.dsq.list) &&
	    rq->scx.rescue.budget >= scx_rescue_కాలాంశం_ns) {
		scx_rescue_admit(rq, p, scx_rescue_కాలాంశం_ns);
		తిరిగి true;
	}

	scx_rescue_timer_arm(rq);
	తిరిగి false;
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
స్థిర శూన్య scx_rescue_check_overload(నిర్మాణం rq *rq)
{
	నిర్మాణం scx_sched *victim = ఖాళీ, *pos;
	నిర్మాణం task_struct *p;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);
	u64 max_avg = 0;
	u32 dur_ms;

	lockdep_assert_rq_held(rq);

	p = list_మొదటి_నమోదు_or_null(&rq->scx.rescue.dsq.list, నిర్మాణం task_struct,
				     scx.dsq_list.కణుపు);
	ఒకవేళ (!p)
		తిరిగి;

	/* has the head waiter been queued for longer than the threshold? */
	ఒకవేళ (సమయం_before(jiffies, p->scx.rescue_at + scx_rescue_overload_after))
		తిరిగి;

	/*
	 * Grace period after the last ejection on this cpu - the freed
	 * bandwidth gets one threshold's worth of time to drain the backlog
	 * before another sub is judged.
	 */
	ఒకవేళ (సమయం_before64(get_jiffies_64(), rq->scx.rescue.kill_at +
			  scx_rescue_overload_after))
		తిరిగి;

	list_for_each_నమోదు_rcu(pos, &scx_sched_all, all) {
		u64 avg = scx_rescue_decay_avg(చొప్పున_cpu_ptr(pos->pcpu, cpu));

		/* skip an already-exiting sub, else the ejection is wasted */
		ఒకవేళ (pos->level && avg > max_avg &&
		    atomic_read(&pos->exit_రకం) == SCX_EXIT_NONE) {
			max_avg = avg;
			victim = pos;
		}
	}
	ఒకవేళ (!victim)
		తిరిగి;

	rq->scx.rescue.kill_at = get_jiffies_64();
	dur_ms = jiffies_గమ్యం_msecs(jiffies - p->scx.rescue_at);
	__scx_exit(victim, SCX_EXIT_ERROR_RESCUE, 0, cpu,
		   "used too much rescue CPU time (%llums) while %s[%d] waited %u.%03us to be rescued",
		   div_u64(max_avg, NSEC_చొప్పున_MSEC), p->comm, p->pid, dur_ms / 1000,
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
స్థిర శూన్య scx_rescue_timerfn(నిర్మాణం timer_list *timer)
{
	నిర్మాణం rq *rq = timer_container_యొక్క(rq, timer, scx.rescue.timer);
	నిర్మాణం task_struct *p;

	guard(rq_తాళం_irqsave)(rq);

	p = rq->scx.rescue.curr;
	ఒకవేళ (!p && list_ఖాళీలు(&rq->scx.rescue.dsq.list))
		తిరిగి;

	scx_rescue_accrue(rq);
	scx_rescue_check_overload(rq);

	ఒకవేళ (!p) {
		s64 ముక్క = scx_rescue_తదుపరి_ముక్క(rq);

		/* no rescue in progress */
		ఒకవేళ (rq->scx.rescue.budget < scx_rescue_కాలాంశం_ns)
			వెళ్ళు బహిర్గతం_arm;

		/* there's enough budget to start rescuing the next one */
		p = list_మొదటి_నమోదు(&rq->scx.rescue.dsq.list, నిర్మాణం task_struct,
				     scx.dsq_list.కణుపు);
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_rescue_admit(rq, p, ముక్క);
		scx_move_local_task_గమ్యం_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
		ఒకవేళ (sched_class_above(&ext_sched_class, rq->curr->sched_class))
			resched_curr(rq);
	} లేకపోతే ఒకవేళ (p->scx.dsq && rq->scx.rescue.budget > 2 * scx_rescue_కాలాంశం_ns) {
		/*
		 * The rescuee waited for the CPU for too long. Escalate - grant
		 * the unserved remainder, protect it from the schedulers and
		 * preempt the current task. The slice is set before the
		 * protection. Repeat beats only repeat the head move - the
		 * slice write is refused on a protected task.
		 */
		scx_సమితి_task_ముక్క(p, scx_rescue_ముక్క_మిగిలిన(rq));
		p->scx.flags |= SCX_TASK_PROTECTED;
		scx_task_unlink_from_dsq(p, &rq->scx.local_dsq);
		scx_move_local_task_గమ్యం_local_dsq(scx_task_sched(p), p,
					SCX_ENQ_తల | SCX_ENQ_PREEMPT | SCX_ENQ_IGNORE_CAPS,
					rq);
	}
బహిర్గతం_arm:
	scx_rescue_timer_arm(rq);
}

/* flush out tasks waiting for rescue before a CPU goes down */
శూన్య scx_rescue_flush(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	/* end the current rescue */
	ఒకవేళ (rq->scx.rescue.curr)
		scx_task_ముక్క_ended(rq, rq->scx.rescue.curr);

	/* and flush out all pending ones */
	list_for_each_నమోదు_సురక్షిత(p, n, &rq->scx.rescue.dsq.list, scx.dsq_list.కణుపు) {
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_move_local_task_గమ్యం_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
	}

	timer_delete(&rq->scx.rescue.timer);
}

శూన్య scx_rescue_dump(నిర్మాణం seq_buf *s, నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *p = rq->scx.rescue.curr;

	scx_dump_పంక్తి(s, "          rescue=%u budget=%lldus rescuing=%s[%d]",
		      rq->scx.rescue.dsq.nr,
		      div_s64(rq->scx.rescue.budget, NSEC_చొప్పున_USEC),
		      p ? p->comm : "none", p ? p->pid : -1);
}

/*
 * A scheduler whose stall watchdog is shorter than the overload threshold gets
 * stall-killed over its parked waiters before the overload check can eject the
 * actual top consumer. The root's knobs set the threshold, warn on any
 * scheduler that doesn't fit it.
 */
స్థిర శూన్య scx_rescue_check_timeout(నిర్మాణం scx_sched *sch)
{
	ఒకవేళ (!scx_rescue_bw_1024 || sch->watchdog_timeout > scx_rescue_overload_after)
		తిరిగి;

	pr_warn("sched_ext: %s: watchdog timeout %ums <= rescue overload threshold %ums\n",
		sch->ops.name, jiffies_గమ్యం_msecs(sch->watchdog_timeout),
		jiffies_గమ్యం_msecs(scx_rescue_overload_after));
}

/* latch the rescue parameters on root scheduler enable */
శూన్య scx_rescue_సమితి_knobs(నిర్మాణం scx_sched *sch)
{
	s32 bw_ppt = sch->ops.rescue_bandwidth_ppt ?: SCX_RESCUE_DFL_BW_PPT;
	s64 కాలాంశం_us = sch->ops.rescue_కాలాంశం_us ?: SCX_RESCUE_DFL_కాలాంశం_US;
	s64 period_ns;

	ఒకవేళ (sch->ops.rescue_bandwidth_ppt == SCX_RESCUE_DISABLE) {
		scx_rescue_bw_1024 = 0;
		తిరిగి;
	}

	scx_rescue_bw_1024 = bw_ppt * SCHED_సామర్థ్యం_SCALE / 1000;
	scx_rescue_కాలాంశం_ns = max(కాలాంశం_us * NSEC_చొప్పున_USEC, TICK_NSEC);
	scx_rescue_sat_delta_ns =
		div_s64((4 * scx_rescue_కాలాంశం_ns + TICK_NSEC) << SCHED_సామర్థ్యం_జరుపు,
			scx_rescue_bw_1024);

	/*
	 * The overload threshold and the decay halflife scale with the funding
	 * period - the time the bucket takes to fund one full quantum.
	 */
	period_ns = div_s64(scx_rescue_కాలాంశం_ns << SCHED_సామర్థ్యం_జరుపు, scx_rescue_bw_1024);
	scx_rescue_overload_after =
		clamp(nsecs_గమ్యం_jiffies(SCX_RESCUE_OVERLOAD_MULT * period_ns),
		      msecs_గమ్యం_jiffies(SCX_RESCUE_MIN_OVERLOAD_MS),
		      msecs_గమ్యం_jiffies(SCX_RESCUE_MAX_OVERLOAD_MS));
	scx_rescue_decay_halflife = scx_rescue_overload_after / 4;

	/* a single in-budget wait must not cross the overload trigger */
	ఒకవేళ (nsecs_గమ్యం_jiffies(period_ns) > scx_rescue_overload_after / 2)
		pr_warn("sched_ext: %s: rescue funding period %lldms > overload threshold %ums / 2\n",
			sch->ops.name, div_s64(period_ns, NSEC_చొప్పున_MSEC),
			jiffies_గమ్యం_msecs(scx_rescue_overload_after));

	scx_rescue_check_timeout(sch);
}

శూన్య scx_rescue_init(నిర్మాణం rq *rq)
{
	BUG_ON(scx_init_dsq(&rq->scx.rescue.dsq, SCX_DSQ_RESCUE, ఖాళీ));
	timer_అమరిక(&rq->scx.rescue.timer, scx_rescue_timerfn, TIMER_PINNED);
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
నిర్మాణం scx_dispatch_q *scx_resolve_local_dsq(నిర్మాణం scx_sched *sch, నిర్మాణం rq *rq,
					     నిర్మాణం task_struct *p, u64 *enq_flags)
{
	ఒకవేళ (!scx_has_subs())
		తిరిగి &rq->scx.local_dsq;

	s32 cid = __scx_cpu_to_cid(cpu_యొక్క(rq));
	నిర్మాణం scx_sched *asch = rq->scx.remote_activate_sch ?: sch;
	u64 needed = scx_caps_for_enq(*enq_flags);
	u64 missing;

	/*
	 * On a remote activation the scheduling sched (@asch) differs from
	 * @p's owner (@sch). Check caps against the scheduling sched.
	 */
	ఒకవేళ (*enq_flags & SCX_ENQ_PREEMPT)
		needed |= scx_caps_for_preempt(asch, rq, *enq_flags);
	missing = scx_missing_caps(asch, cpu_యొక్క(rq), needed);

	/* requirements met */
	ఒకవేళ (likely(!missing))
		తిరిగి &rq->scx.local_dsq;

	/*
	 * The task must run on this CPU regardless of caps: the rq is draining
	 * offline (BPF scheduler bypassed), the task is migration-disabled, or a
	 * migration is pending. Admit despite the missing caps and count it.
	 * Refuse preemptions.
	 */
	ఒకవేళ (unlikely(!scx_rq_online(rq) || is_migration_disabled(p) ||
		     p->migration_pending)) {
		__scx_add_event(sch, SCX_EV_SUB_FORCED_ADMIT, 1);
		*enq_flags &= ~SCX_ENQ_PREEMPT;
		తిరిగి &rq->scx.local_dsq;
	}

	/*
	 * Diverting to rescue or reject, neither of which honors IMMED, PREEMPT
	 * or HEAD - a diversion has no priority and IMMED is not allowed on
	 * non-local DSQs. Strip the enq and task flags along with the slice.
	 */
	*enq_flags &= ~(SCX_ENQ_IMMED | SCX_ENQ_PREEMPT | SCX_ENQ_తల |
			SCX_ENQ_APPLY_ముక్క | SCX_ENQ_ముక్క_DFL);
	p->scx.flags &= ~SCX_TASK_IMMED;

	/* the enqueuer opted for rescue instead of rejection and reenqueue */
	ఒకవేళ ((*enq_flags & SCX_ENQ_RESCUE) && likely(scx_rescue_bw_1024)) {
		__scx_add_event(sch, SCX_EV_SUB_RESCUE, 1);
		ఒకవేళ (scx_rescue_try_admit(rq, p))
			తిరిగి &rq->scx.local_dsq;

		/* queueing, the overload trigger measures the wait from here */
		p->scx.rescue_at = jiffies;
		తిరిగి &rq->scx.rescue.dsq;
	}

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = cid;

	తిరిగి &rq->scx.reject_dsq;
}

/* @p lost the caps needed to stay on @rq's local DSQ? Record reason if so. */
bool scx_task_reenq_on_cap_revoke(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	u64 missing;

	/* migration-disabled tasks and the rescuee are admitted capless */
	ఒకవేళ (is_migration_disabled(p) || p == scx_rescuee(rq))
		తిరిగి false;

	missing = scx_missing_caps(scx_task_sched(p), cpu_యొక్క(rq), scx_caps_for_task(p));
	ఒకవేళ (likely(!missing))
		తిరిగి false;

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = __scx_cpu_to_cid(cpu_యొక్క(rq));
	తిరిగి true;
}

/*
 * Drain @rq->scx.reject_dsq, reenqueueing each task so the BPF re-decides
 * from p->scx.reenq_reason_*.
 *
 * A task can be re-rejected repeatedly. The reenqueue is bounded per task in
 * scx_do_enqueue_task(), which ejects the owning sub past SCX_REENQ_MAX_REPEAT.
 * Rejection can't happen for root.
 */
శూన్య scx_reenq_reject(నిర్మాణం rq *rq)
{
	LIST_తల(tasks);
	నిర్మాణం task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	ఒకవేళ (!scx_has_subs() || list_ఖాళీలు(&rq->scx.reject_dsq.list))
		తిరిగి;

	/*
	 * Move to a private list so a task re-rejected by the
	 * scx_do_enqueue_task() below isn't revisited this round.
	 */
	list_for_each_నమోదు_సురక్షిత(p, n, &rq->scx.reject_dsq.list, scx.dsq_list.కణుపు) {
		/* migration_pending tasks should have bypassed to local DSQ */
		ఒకవేళ (WARN_ON_ONCE(p->migration_pending))
			కొనసాగు;

		scx_wait_dispatching(p);
		scx_dispatch_వరుస_నుండి_తీయి(rq, p);

		ఒకవేళ (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_ముసుగు))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_ముసుగు;
		p->scx.flags |= SCX_TASK_REENQ_CAP;

		list_కూడు_తోక(&p->scx.dsq_list.కణుపు, &tasks);
	}

	list_for_each_నమోదు_సురక్షిత(p, n, &tasks, scx.dsq_list.కణుపు) {
		list_del_init(&p->scx.dsq_list.కణుపు);

		scx_do_వరుసలో_చేర్చు_task(rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_ముసుగు;
	}
}

/* record a caps change, see struct scx_caps_updated */
స్థిర శూన్య caps_updated_రికార్డు(నిర్మాణం scx_pshard *ps, స్థిరరాశి నిర్మాణం scx_cmask *cids, u64 caps,
				నిర్మాణం list_తల *గమ్యం_deliver)
{
	నిర్మాణం scx_caps_updated *cu = &ps->caps_updated;

	guard(raw_spinlock)(&cu->తాళం);
	scx_cmask_or(&cu->cmask, cids);
	cu->caps |= caps;
	ఒకవేళ (list_ఖాళీలు(&cu->కణుపు_లోపల_flight))
		list_కూడు_తోక(&cu->కణుపు_లోపల_flight, గమ్యం_deliver);
}

/* deliver queued caps_updated callbacks, see struct scx_caps_updated */
స్థిర శూన్య caps_updated_deliver(నిర్మాణం list_తల *గమ్యం_deliver)
{
	నిర్మాణం scx_caps_updated *cu, *tmp;

	list_for_each_నమోదు_సురక్షిత(cu, tmp, గమ్యం_deliver, కణుపు_లోపల_flight) {
		నిర్మాణం scx_pshard *ps = container_యొక్క(cu, నిర్మాణం scx_pshard, caps_updated);
		నిర్మాణం scx_sched *sch = ps->sch;

		అయితే (true) {
			u64 caps = 0;

			/*
			 * During enable, has_op is set after ops.sub_attach(),
			 * so !has_op means the op is absent or the sched isn't
			 * live yet - e.g. caps grant from ops.sub_attach().
			 * Either way don't consume - leave for
			 * scx_sub_seed_caps() to deliver once live.
			 */
			scoped_guard (raw_spinlock, &cu->తాళం) {
				ఒకవేళ (cu->caps && SCX_HAS_OP(sch, sub_caps_updated) &&
				    likely(!READ_ONCE(sch->aborting))) {
					నిర్మాణం scx_cmask_ref ref;

					caps = cu->caps;
					scx_cmask_ref_init_kern(sch, cu->cmask_arena_బహిర్గతం,
								ps->పునాది, ps->nr_cids, &ref);
					scx_cmask_ref_copy(&ref, &cu->cmask);
					scx_cmask_clear(&cu->cmask);
					cu->caps = 0;
				} లేకపోతే {
					list_del_init(&cu->కణుపు_లోపల_flight);
				}
			}
			ఒకవేళ (!caps)
				ఆపు;

			/* caps != 0 only when deliverable (has_op, above) */
			SCX_CALL_OP(sch, sub_caps_updated, ఖాళీ, cu->cmask_arena_బహిర్గతం, caps);
		}
	}
}

/*
 * Deliver caps owed to @sch that couldn't be delivered earlier (e.g. a grant
 * taken during its sub_attach(), before has_op was set). Called once @sch is
 * enabled.
 */
స్థిర శూన్య scx_sub_విత్తనం_caps(నిర్మాణం scx_sched *sch)
{
	LIST_తల(గమ్యం_deliver);
	s32 si;

	guard(irqsave)();

	కొరకు (si = 0; si < sch->nr_pshards; si++) {
		నిర్మాణం scx_pshard *ps = sch->pshard[si];
		నిర్మాణం scx_caps_updated *cu = &ps->caps_updated;

		scoped_guard (raw_spinlock, &cu->తాళం) {
			ఒకవేళ (cu->caps && list_ఖాళీలు(&cu->కణుపు_లోపల_flight))
				list_కూడు_తోక(&cu->కణుపు_లోపల_flight, &గమ్యం_deliver);
		}
	}
	caps_updated_deliver(&గమ్యం_deliver);
}

స్థిర u64 calc_effective_caps(నిర్మాణం scx_pshard *ps, s32 cid)
{
	u64 ecaps = 0;
	u32 cap_బిట్టు;

	కొరకు (cap_బిట్టు = 0; cap_బిట్టు < __SCX_NR_CAPS; cap_బిట్టు++)
		ఒకవేళ (scx_cmask_test(cid, &ps->caps[cap_బిట్టు].cmask))
			ecaps |= బిట్టు_U64(cap_బిట్టు) | scx_caps_implied(బిట్టు_U64(cap_బిట్టు));
	తిరిగి ecaps;
}

/**
 * queue_sync_ecaps - Queue ecaps update for a (sch, cid) pair
 * @sch: sched to update
 * @cid: cid to update
 *
 * Queue an ecaps update for @sch's @cid and kick the cpu so that it syncs in
 * dispatch_one().
 */
స్థిర శూన్య వరుస_sync_ecaps(నిర్మాణం scx_sched *sch, s32 cid)
{
	s32 cpu = __scx_cid_to_cpu(cid);
	నిర్మాణం scx_sched_pcpu *pcpu = చొప్పున_cpu_ptr(sch->pcpu, cpu);

	/*
	 * Pairs with smp_mb() in scx_process_sync_ecaps(). Either the check
	 * below sees the node off the list and queues it, or the in-flight sync
	 * sees the caps[] update made before this call.
	 */
	smp_mb();

	/* @cid's pshard->lock excludes concurrent queueing attempts */
	ఒకవేళ (llist_on_list(&pcpu->ecaps_గమ్యం_sync_కణుపు))
		తిరిగి;
	ఒకవేళ (llist_కూడు(&pcpu->ecaps_గమ్యం_sync_కణుపు, &cpu_rq(cpu)->scx.ecaps_గమ్యం_sync))
		scx_kick_cpu(sch->ancestors[0], cpu, 0);
}

/* discard @rq's queued ecaps syncs */
స్థిర శూన్య discard_queued_syncs(నిర్మాణం rq *rq)
{
	నిర్మాణం llist_కణుపు *pos, *tmp;

	lockdep_assert_rq_held(rq);

	llist_for_each_సురక్షిత(pos, tmp, llist_del_all(&rq->scx.ecaps_గమ్యం_sync))
		init_llist_కణుపు(pos);
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
శూన్య scx_process_sync_ecaps(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev)
{
	s32 cpu = cpu_యొక్క(rq);
	s32 cid, shard;
	నిర్మాణం llist_కణుపు *batch, *pos, *tmp;
	u64 కోల్పోయిన_all = 0;

	lockdep_assert_rq_held(rq);

	ఒకవేళ (!scx_has_subs() || likely(llist_ఖాళీలు(&rq->scx.ecaps_గమ్యం_sync)))
		తిరిగి;

	/*
	 * ecaps are zeroed while the cpu is inactive and must stay zero.
	 * Discard queued syncs instead of processing them - the
	 * scx_online_ecaps() reseed re-syncs every sched on activation.
	 * cpu_active() clears before the offline zeroing and sets before the
	 * reseed is queued, so this test can neither miss a racing sync nor
	 * eat the reseed.
	 */
	ఒకవేళ (unlikely(!cpu_చురుకైన(cpu))) {
		discard_queued_syncs(rq);
		తిరిగి;
	}

	/* @cid is valid here: the cpu is active with queued syncs */
	cid = __scx_cpu_to_cid(cpu);
	shard = rcu_dereference_all(scx_cid_గమ్యం_shard)[cid];

	batch = llist_del_all(&rq->scx.ecaps_గమ్యం_sync);
	llist_for_each_సురక్షిత(pos, tmp, batch) {
		నిర్మాణం scx_sched_pcpu *pcpu =
			container_యొక్క(pos, నిర్మాణం scx_sched_pcpu, ecaps_గమ్యం_sync_కణుపు);
		నిర్మాణం scx_pshard *ps = pcpu->sch->pshard[shard];
		u64 old, ecaps, కోల్పోయిన, gained;

		init_llist_కణుపు(pos);

		/* pairs with smp_mb() in queue_sync_ecaps(), see there */
		smp_mb();

		old = READ_ONCE(pcpu->ecaps);
		ecaps = calc_effective_caps(ps, cid);
		WRITE_ONCE(pcpu->ecaps, ecaps);

		కోల్పోయిన = old & ~ecaps;
		gained = ecaps & ~old;
		కోల్పోయిన_all |= కోల్పోయిన;

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
		ఒకవేళ (ecaps != pcpu->reported_ecaps &&
		    SCX_HAS_OP(pcpu->sch, sub_ecaps_updated) &&
		    !scx_bypassing(pcpu->sch, cpu)) {
			నిర్మాణం scx_dsp_ctx *dspc = &this_cpu_ptr(pcpu->sch->pcpu)->dsp_ctx;

			dspc->rq = rq;
			/* stash @prev so nested dispatches can access it */
			rq->scx.sub_dispatch_prev = prev;
			SCX_CALL_OP(pcpu->sch, sub_ecaps_updated, rq, scx_cpu_arg(cpu),
				    pcpu->reported_ecaps, ecaps);
			rq->scx.sub_dispatch_prev = ఖాళీ;
			scx_flush_dispatch_buf(pcpu->sch, rq);
			pcpu->reported_ecaps = ecaps;
		}

		/*
		 * Gaining baseline access owes an update_idle() so the sched
		 * learns the cpu's idle state. Arm the per-rq gate so the next
		 * idle pick flushes it. Losing access drops any pending notify.
		 */
		ఒకవేళ (gained & SCX_CAP_పునాది) {
			pcpu->idle_renotify = true;
			rq->scx.flags |= SCX_RQ_SUB_IDLE_RENOTIFY;
		} లేకపోతే ఒకవేళ (కోల్పోయిన & SCX_CAP_పునాది) {
			pcpu->idle_renotify = false;
		}
	}

	/*
	 * Losing a cap can strand already-queued tasks. Schedule a reenq scan
	 * to move the now-capless ones off the local DSQ. The scan tests
	 * against the effective caps and thus must come after the ecaps sync.
	 */
	ఒకవేళ (కోల్పోయిన_all & SCX_CAPS_REENQ_ON_LOSS)
		scx_కాలపట్టిక_reenq_local(rq, SCX_REENQ_CAP_REVOKE);
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
శూన్య scx_unbypass_replay_ecaps(నిర్మాణం rq *rq, నిర్మాణం scx_sched *sch)
{
	s32 cpu = cpu_యొక్క(rq);
	నిర్మాణం scx_sched_pcpu *pcpu = చొప్పున_cpu_ptr(sch->pcpu, cpu);
	నిర్మాణం scx_pshard *ps;
	s32 cid;

	lockdep_assert_rq_held(rq);

	/* root holds every cap and never uses ecaps */
	ఒకవేళ (!sch->level)
		తిరిగి;

	ఒకవేళ (READ_ONCE(pcpu->ecaps) == pcpu->reported_ecaps)
		తిరిగి;

	cid = __scx_cpu_to_cid(cpu);
	ps = sch->pshard[rcu_dereference_all(scx_cid_గమ్యం_shard)[cid]];

	guard(raw_spinlock)(&ps->తాళం);
	వరుస_sync_ecaps(sch, cid);
}

/*
 * A cpu came back. Re-seed each sub-sched's ecaps on the cpu's cid. The sync
 * recomputes effective caps from the pshard and fires ops.sub_ecaps_updated()
 * only on a real change since offline.
 */
శూన్య scx_online_ecaps(నిర్మాణం rq *rq)
{
	నిర్మాణం scx_sched *మూలం, *pos;
	s32 cid, shard;

	/*
	 * Only a live hierarchy can have ecaps to reseed. This also keeps the
	 * table reads below away from an enable that failed before publishing
	 * the tables. A concurrent disable can't retire them, see
	 * handle_hotplug().
	 */
	ఒకవేళ (!scx_enabled())
		తిరిగి;

	guard(rq_తాళం_irqsave)(rq);

	మూలం = scx_మూలం_protected();
	cid = __scx_cpu_to_cid(cpu_యొక్క(rq));
	shard = rcu_dereference_all(scx_cid_గమ్యం_shard)[cid];

	scx_for_each_descendant_pre(pos, మూలం) {
		నిర్మాణం scx_pshard *ps;

		/* root holds every cap and never uses ecaps */
		ఒకవేళ (!pos->level)
			కొనసాగు;

		ps = pos->pshard[shard];
		guard(raw_spinlock)(&ps->తాళం);
		వరుస_sync_ecaps(pos, cid);
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
శూన్య scx_offline_ecaps(నిర్మాణం rq *rq)
{
	s32 cpu = cpu_యొక్క(rq);
	నిర్మాణం scx_sched *మూలం, *pos;

	guard(rq_తాళం_irqsave)(rq);

	మూలం = scx_మూలం_protected();

	scx_for_each_descendant_pre(pos, మూలం) {
		/* root holds every cap and never uses ecaps */
		ఒకవేళ (!pos->level)
			కొనసాగు;

		WRITE_ONCE(చొప్పున_cpu_ptr(pos->pcpu, cpu)->ecaps, 0);
	}
}

/*
 * @pcpu's sched was unhashed before the grace period, so nothing re-queues its
 * sync node. Remove the node from @rq's pending list so the pcpu can be freed.
 */
శూన్య scx_discard_ecaps_గమ్యం_sync(s32 cpu, నిర్మాణం scx_sched_pcpu *pcpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	నిర్మాణం llist_కణుపు *తల = ఖాళీ, *తోక = ఖాళీ;
	నిర్మాణం llist_కణుపు *pos, *tmp;

	/*
	 * llist can't unlink a single node. Take all queued nodes, drop @pcpu's
	 * and resplice the rest. Nodes in the taken batch read as on-list
	 * throughout, so queue_sync_ecaps() stays correct.
	 */
	ఒకవేళ (llist_on_list(&pcpu->ecaps_గమ్యం_sync_కణుపు)) {
		scoped_guard (rq_తాళం_irqsave, rq) {
			llist_for_each_సురక్షిత(pos, tmp, llist_del_all(&rq->scx.ecaps_గమ్యం_sync)) {
				ఒకవేళ (pos == &pcpu->ecaps_గమ్యం_sync_కణుపు) {
					init_llist_కణుపు(pos);
				} లేకపోతే {
					pos->తదుపరి = తల;
					తల = pos;
					ఒకవేళ (!తోక)
						తోక = pos;
				}
			}
			ఒకవేళ (తల)
				llist_కూడు_batch(తల, తోక, &rq->scx.ecaps_గమ్యం_sync);
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
	అయితే (true) {
		scoped_guard (rq_తాళం_irqsave, rq) {
			ఒకవేళ (!llist_on_list(&pcpu->ecaps_గమ్యం_sync_కణుపు))
				తిరిగి;
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
శూన్య scx_discard_stale_ecaps_syncs(శూన్య)
{
	s32 cpu;

	for_each_possible_cpu(cpu) {
		నిర్మాణం rq *rq = cpu_rq(cpu);

		guard(rq_తాళం_irqsave)(rq);
		discard_queued_syncs(rq);
	}
}

స్థిర DECLARE_WAIT_వరుస_తల(scx_unlink_waitq);

శూన్య drain_descendants(నిర్మాణం scx_sched *sch)
{
	/*
	 * Child scheds that finished the critical part of disabling will take
	 * themselves off @sch->children. Wait for it to drain. As propagation
	 * is recursive, empty @sch->children means that all proper descendant
	 * scheds reached unlinking stage.
	 */
	wait_event(scx_unlink_waitq, list_ఖాళీలు(&sch->children));
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
స్థిర శూన్య scx_rehome_task(నిర్మాణం scx_sched *గమ్యం, నిర్మాణం task_struct *p)
{
	lockdep_assert_held(&p->pi_తాళం);
	lockdep_assert_rq_held(task_rq(p));

	scoped_guard (sched_change, p, వరుస_నుండి_తీయి_SAVE | వరుస_నుండి_తీయి_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_సమితి_task_state(p, SCX_TASK_INIT_BEGIN);
		scx_సమితి_task_state(p, SCX_TASK_INIT);
		scx_సమితి_task_sched(p, గమ్యం);
		scx_సమితి_task_state(p, SCX_TASK_READY);
		ఒకవేళ (p->sched_class == &ext_sched_class)
			scx_enable_task(గమ్యం, p);
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
స్థిర శూన్య scx_punt_task(నిర్మాణం scx_sched *గమ్యం, నిర్మాణం task_struct *p)
{
	lockdep_assert_held(&p->pi_తాళం);
	lockdep_assert_rq_held(task_rq(p));
	WARN_ON_ONCE(!READ_ONCE(గమ్యం->bypass_లోతు));

	scoped_guard (sched_change, p, వరుస_నుండి_తీయి_SAVE | వరుస_నుండి_తీయి_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_సమితి_task_sched(p, గమ్యం);
	}
}

స్థిర శూన్య scx_fail_మాతృ(నిర్మాణం scx_sched *sch,
			    నిర్మాణం task_struct *failed, s32 fail_సంకేతం)
{
	నిర్మాణం scx_sched *మాతృ = scx_మాతృ(sch);
	నిర్మాణం scx_task_iter sti;
	నిర్మాణం task_struct *p;

	scx_error(మాతృ, "ops.init_task() failed (%d) for %s[%d] while disabling a sub-scheduler",
		  fail_సంకేతం, failed->comm, failed->pid);

	/*
	 * Once $parent is bypassed, tasks can be punted into it. This may
	 * cause downstream failures on the BPF side but $parent is dying
	 * anyway.
	 */
	scx_bypass(మాతృ, true);

	scx_task_iter_ప్రారంభం(&sti, sch->cgrp);
	అయితే ((p = scx_task_iter_తదుపరి_locked(&sti))) {
		ఒకవేళ (scx_task_on_sched(మాతృ, p))
			కొనసాగు;

		scx_punt_task(మాతృ, p);
	}
	scx_task_iter_stop(&sti);
}

#నిర్వచితమైతే CONFIG_EXT_సమూహం_SCHED
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
స్థిర s32 scx_cgroup_claim_subtree(నిర్మాణం scx_sched *sch)
{
	నిర్మాణం cgroup *sub_cgrp = sch_cgroup(sch);
	నిర్మాణం cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	నిర్మాణం scx_sched *మాతృ = scx_మాతృ(sch);
	నిర్మాణం cgroup_subsys_state *css;
	పూర్ణాంకం ret;

	css_for_each_descendant_pre(css, ecss) {
		నిర్మాణం task_సమూహం *tg = css_tg(css);
		నిర్మాణం scx_cgroup_init_వాదనలు వాదనలు = {
			.బరువు = tg->scx.బరువు,
			.bw_period_us = tg->scx.bw_period_us,
			.bw_quota_us = tg->scx.bw_quota_us,
			.bw_వ్యవధి_us = tg->scx.bw_వ్యవధి_us,
			.sched_idle = tg->scx.idle,
		};

		ఒకవేళ (tg->scx.sched != మాతృ ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			కొనసాగు;

		ఒకవేళ (SCX_HAS_OP(sch, cgroup_init)) {
			ret = SCX_CALL_OP_RET(sch, cgroup_init, ఖాళీ, css->cgroup, &వాదనలు);
			ఒకవేళ (ret) {
				scx_error(sch, "ops.cgroup_init() failed (%d)", ret);
				వెళ్ళు err;
			}
		}
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_post(css, ecss) {
		నిర్మాణం task_సమూహం *tg = css_tg(css);

		/*
		 * SUB_INIT is pass 1's progress mark: pass 2 and the err path
		 * must visit exactly the tgs pass 1 inited.
		 */
		ఒకవేళ (!(tg->scx.flags & SCX_TG_SUB_INIT))
			కొనసాగు;

		/* skip the exit if the parent's ops.cgroup_init() failed */
		ఒకవేళ ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(మాతృ, cgroup_exit))
			SCX_CALL_OP(మాతృ, cgroup_exit, ఖాళీ, css->cgroup);
		tg->scx.sched = sch;
		tg->scx.flags |= SCX_TG_INITED;
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}

	తిరిగి 0;

err:
	css_for_each_descendant_post(css, ecss) {
		నిర్మాణం task_సమూహం *tg = css_tg(css);

		ఒకవేళ (!(tg->scx.flags & SCX_TG_SUB_INIT))
			కొనసాగు;

		ఒకవేళ (SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, ఖాళీ, css->cgroup);
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}
	తిరిగి ret;
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
స్థిర శూన్య scx_cgroup_return_subtree(నిర్మాణం scx_sched *sch)
{
	నిర్మాణం cgroup *sub_cgrp = sch_cgroup(sch);
	నిర్మాణం cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	నిర్మాణం scx_sched *మాతృ = scx_మాతృ(sch);
	నిర్మాణం cgroup_subsys_state *css;
	bool మాతృ_failed = false;
	పూర్ణాంకం ret;

	css_for_each_descendant_post(css, ecss) {
		నిర్మాణం task_సమూహం *tg = css_tg(css);

		ఒకవేళ (tg->scx.sched != sch ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			కొనసాగు;

		/* skip the exit if @sch's ops.cgroup_init() failed for the tg */
		ఒకవేళ ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, ఖాళీ, css->cgroup);
		tg->scx.sched = మాతృ;
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_pre(css, ecss) {
		నిర్మాణం task_సమూహం *tg = css_tg(css);
		నిర్మాణం scx_cgroup_init_వాదనలు వాదనలు = {
			.బరువు = tg->scx.బరువు,
			.bw_period_us = tg->scx.bw_period_us,
			.bw_quota_us = tg->scx.bw_quota_us,
			.bw_వ్యవధి_us = tg->scx.bw_వ్యవధి_us,
			.sched_idle = tg->scx.idle,
		};

		/* the first pass must have transferred everything */
		WARN_ON_ONCE(tg->scx.sched == sch);

		/*
		 * SUB_INIT distinguishes the tgs pass 1 moved. The sched test
		 * can't: a tg punted to the parent by an earlier failure would
		 * also match.
		 */
		ఒకవేళ (!(tg->scx.flags & SCX_TG_SUB_INIT))
			కొనసాగు;
		tg->scx.flags &= ~(SCX_TG_SUB_INIT | SCX_TG_INITED);

		/*
		 * A re-init on $parent failed. The task_groups from here on are
		 * punted: they stay on the dying $parent with INITED clear and
		 * move onward when it disables.
		 */
		ఒకవేళ (మాతృ_failed)
			కొనసాగు;

		ఒకవేళ (SCX_HAS_OP(మాతృ, cgroup_init)) {
			ret = SCX_CALL_OP_RET(మాతృ, cgroup_init, ఖాళీ, css->cgroup, &వాదనలు);
			ఒకవేళ (ret) {
				scx_error(మాతృ, "ops.cgroup_init() failed (%d) while disabling a sub-scheduler",
					  ret);
				మాతృ_failed = true;
				కొనసాగు;
			}
		}
		tg->scx.flags |= SCX_TG_INITED;
	}
}
#లేకపోతే
స్థిర అంతఃవరుస s32 scx_cgroup_claim_subtree(నిర్మాణం scx_sched *sch) { తిరిగి 0; }
స్థిర అంతఃవరుస శూన్య scx_cgroup_return_subtree(నిర్మాణం scx_sched *sch) {}
#అయితే_ముగింపు

శూన్య scx_sub_disable(నిర్మాణం scx_sched *sch)
{
	నిర్మాణం scx_sched *మాతృ = scx_మాతృ(sch);
	నిర్మాణం scx_task_iter sti;
	నిర్మాణం task_struct *p;
	పూర్ణాంకం ret;

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
	mutex_తాళం(&scx_enable_mutex);
	percpu_కిందికి_write(&scx_fork_rwsem);
	scx_cgroup_తాళం();

	/*
	 * An enable that failed before scx_link_sched() succeeded never owned a
	 * cgroup or task and won't be waited on by an ancestor's
	 * drain_descendants(). Nothing to reparent and walking the tasks can
	 * misbehave as the task ownership invariant (either owned by self or
	 * parent) does not hold. ->sibling can't identify this case - an undone
	 * link leaves it non-empty.
	 */
	ఒకవేళ (!sch->linked)
		వెళ్ళు dump;

	సమితి_cgroup_sched(sch_cgroup(sch), మాతృ);

	/*
	 * Return the subtree's cgroups before re-homing tasks so that any
	 * ops.init_task() on $parent only sees cgroups it has initialized.
	 */
	scx_cgroup_return_subtree(sch);

	scx_task_iter_ప్రారంభం(&sti, sch->cgrp);
	అయితే ((p = scx_task_iter_తదుపరి_locked(&sti))) {
		నిర్మాణం rq *rq;
		నిర్మాణం rq_flags rf;

		/* filter out duplicate visits */
		ఒకవేళ (scx_task_on_sched(మాతృ, p))
			కొనసాగు;

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
		ret = __scx_init_task(మాతృ, p, ఖాళీ, false);
		ఒకవేళ (ret) {
			scx_fail_మాతృ(sch, p, ret);
			put_task_struct(p);
			ఆపు;
		}

		rq = task_rq_తాళం_2(p, &rf);

		ఒకవేళ (scx_get_task_state(p) == SCX_TASK_DEAD) {
			/*
			 * sched_ext_dead() raced us between __scx_init_task()
			 * and this rq lock and ran exit_task() on @sch (the
			 * sched @p was on at that point), not on $parent.
			 * $parent's just-completed init is owed an exit_task()
			 * and we issue it here.
			 */
			scx_sub_init_cancel_task(మాతృ, p);
			task_rq_unlock(rq, p, &rf);
			put_task_struct(p);
			కొనసాగు;
		}

		scx_rehome_task(మాతృ, p);

		task_rq_unlock(rq, p, &rf);
		put_task_struct(p);
	}
	scx_task_iter_stop(&sti);

dump:
	scx_disable_dump(sch);

	scx_cgroup_unlock();
	percpu_పైకి_write(&scx_fork_rwsem);

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
	wake_పైకి_all(&scx_unlink_waitq);

	ఒకవేళ (మాతృ->ops.sub_detach && sch->sub_attached) {
		నిర్మాణం scx_sub_detach_వాదనలు sub_detach_వాదనలు = {
			.ops = &sch->ops,
			.cgroup_మార్గం = sch->cgrp_మార్గం,
		};
		SCX_CALL_OP(మాతృ, sub_detach, ఖాళీ,
			    &sub_detach_వాదనలు);
	}

	scx_log_sched_disable(sch);

	ఒకవేళ (sch->ops.నిష్క్రమించు)
		SCX_CALL_OP(sch, నిష్క్రమించు, ఖాళీ, sch->exit_info);

	/*
	 * @sch's non-ops programs such as timers and tracers can fire after
	 * ops.exit(). Now that exit is complete, stop scx_prog_sched() from
	 * resolving to @sch and drain in-flight resolvers.
	 */
	WRITE_ONCE(sch->dead, true);
	synchronize_rcu();

	ఒకవేళ (sch->sub_kset)
		kobject_del(&sch->sub_kset->kobj);
	/* not added if enable failed before scx_sched_sysfs_add() */
	ఒకవేళ (sch->kobj.state_లోపల_sysfs)
		kobject_del(&sch->kobj);
}

/* verify that a scheduler can be attached to @cgrp and return the parent */
స్థిర నిర్మాణం scx_sched *కనుగొను_మాతృ_sched(నిర్మాణం cgroup *cgrp)
{
	నిర్మాణం scx_sched *మాతృ = scx_cgroup_sched(cgrp);
	నిర్మాణం scx_sched *pos;

	lockdep_assert_held(&scx_sched_తాళం);

	/* can't attach twice to the same cgroup */
	ఒకవేళ (మాతృ->cgrp == cgrp)
		తిరిగి ERR_PTR(-EBUSY);

	/* does $parent allow sub-scheds? */
	ఒకవేళ (!మాతృ->ops.sub_attach)
		తిరిగి ERR_PTR(-EOPNOTSUPP);

	/* can't insert between $parent and its exiting children */
	list_for_each_నమోదు(pos, &మాతృ->children, sibling)
		ఒకవేళ (cgroup_is_descendant(pos->cgrp, cgrp))
			తిరిగి ERR_PTR(-EBUSY);

	తిరిగి మాతృ;
}

స్థిర bool assert_task_ready_or_enabled(నిర్మాణం task_struct *p)
{
	u32 state = scx_get_task_state(p);

	ఎంపిక (state) {
	స్థితి SCX_TASK_READY:
	స్థితి SCX_TASK_ENABLED:
		తిరిగి true;
	అప్రమేయ:
		WARN_ONCE(true, "sched_ext: Invalid task state %d for %s[%d] during enabling sub sched",
			  state, p->comm, p->pid);
		తిరిగి false;
	}
}

శూన్య scx_sub_enable_workfn(నిర్మాణం kthread_work *work)
{
	నిర్మాణం scx_enable_cmd *cmd = container_యొక్క(work, నిర్మాణం scx_enable_cmd, work);
	నిర్మాణం sched_ext_ops *ops = cmd->ops;
	నిర్మాణం cgroup *cgrp;
	నిర్మాణం scx_sched *మాతృ, *sch;
	నిర్మాణం scx_task_iter sti;
	నిర్మాణం task_struct *p;
	s32 i, ret;

	mutex_తాళం(&scx_enable_mutex);

	ఒకవేళ (!scx_enabled()) {
		ret = -ENODEV;
		వెళ్ళు బహిర్గతం_unlock;
	}

	/* See scx_root_enable_workfn() for the @ops->priv check. */
	ఒకవేళ (rcu_access_సూచిక(ops->priv)) {
		ret = -EBUSY;
		వెళ్ళు బహిర్గతం_unlock;
	}

	cgrp = cgroup_get_from_id(ops->sub_cgroup_id);
	ఒకవేళ (IS_ERR(cgrp)) {
		ret = PTR_ERR(cgrp);
		వెళ్ళు బహిర్గతం_unlock;
	}

	raw_spin_తాళం_irq(&scx_sched_తాళం);
	మాతృ = కనుగొను_మాతృ_sched(cgrp);
	ఒకవేళ (IS_ERR(మాతృ)) {
		raw_spin_unlock_irq(&scx_sched_తాళం);
		ret = PTR_ERR(మాతృ);
		వెళ్ళు బహిర్గతం_put_cgrp;
	}
	kobject_get(&మాతృ->kobj);
	raw_spin_unlock_irq(&scx_sched_తాళం);

	/*
	 * Flip the hot-path gates before ops->priv is published - the sub's
	 * programs can e.g. kick cpus from that point on. The matching dec is
	 * at the end of scx_sched_free_rcu_work().
	 */
	static_branch_inc(&__scx_has_subs);

	/* scx_alloc_and_add_sched() consumes @cgrp whether it succeeds or not */
	sch = scx_alloc_and_కూడు_sched(cmd, cgrp, మాతృ);
	kobject_put(&మాతృ->kobj);
	ఒకవేళ (IS_ERR(sch)) {
		static_branch_dec(&__scx_has_subs);
		ret = PTR_ERR(sch);
		వెళ్ళు బహిర్గతం_unlock;
	}

	/*
	 * Validate before scx_link_sched() publishes @sch, so an invalid sub
	 * never becomes visible with an unallocated pshard.
	 */
	ret = scx_validate_ops(sch, ops);
	ఒకవేళ (ret)
		వెళ్ళు err_disable;

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
	ఒకవేళ (ret)
		వెళ్ళు err_disable;

	ret = scx_alloc_pshards(sch);
	ఒకవేళ (ret)
		వెళ్ళు err_disable;

	ret = scx_link_sched(sch);
	ఒకవేళ (ret)
		వెళ్ళు err_disable;

	ret = scx_sched_sysfs_కూడు(sch);
	ఒకవేళ (ret)
		వెళ్ళు err_disable;

	ఒకవేళ (sch->level >= SCX_SUB_MAX_లోతు) {
		scx_error(sch, "max nesting depth %d violated",
			  SCX_SUB_MAX_లోతు);
		ret = -EINVAL;
		వెళ్ళు err_disable;
	}

	scoped_guard(cpus_read_తాళం) {
		ret = scx_alloc_kern_arena_objs(sch);
		ఒకవేళ (ret)
			వెళ్ళు err_disable;
	}

	ఒకవేళ (sch->ops.init) {
		ret = SCX_CALL_OP_RET(sch, init, ఖాళీ);
		ఒకవేళ (ret) {
			ret = scx_ops_sanitize_err(sch, "init", ret);
			scx_error(sch, "ops.init() failed (%d)", ret);
			వెళ్ళు err_disable;
		}
		sch->exit_info->flags |= SCX_EFLAG_INITIALIZED;
	}

	నిర్మాణం scx_sub_attach_వాదనలు sub_attach_వాదనలు = {
		.ops = &sch->ops,
		.cgroup_మార్గం = sch->cgrp_మార్గం,
	};

	ret = SCX_CALL_OP_RET(మాతృ, sub_attach, ఖాళీ,
			      &sub_attach_వాదనలు);
	ఒకవేళ (ret) {
		ret = scx_ops_sanitize_err(sch, "sub_attach", ret);
		scx_error(sch, "parent rejected (%d)", ret);
		వెళ్ళు err_disable;
	}
	sch->sub_attached = true;

	scx_bypass(sch, true);

	కొరకు (i = SCX_OPI_BEGIN; i < SCX_OPI_చివర; i++)
		ఒకవేళ (((శూన్య (**)(శూన్య))ops)[i])
			సమితి_బిట్టు(i, sch->has_op);

	percpu_కిందికి_write(&scx_fork_rwsem);
	scx_cgroup_తాళం();

	/*
	 * Set cgroup->scx_sched's and check CSS_ONLINE. Either we see
	 * !CSS_ONLINE or scx_cgroup_lifetime_notify() sees and shoots us down.
	 */
	సమితి_cgroup_sched(sch_cgroup(sch), sch);
	ఒకవేళ (!(cgrp->self.flags & CSS_ONLINE)) {
		scx_error(sch, "cgroup is not online");
		ret = -ENODEV;
		వెళ్ళు err_unlock_and_disable;
	}

	/*
	 * Take over the subtree's cgroups before any task is claimed,
	 * mirroring root enable's cgroups-before-tasks order.
	 */
	ret = scx_cgroup_claim_subtree(sch);
	ఒకవేళ (ret)
		వెళ్ళు err_unlock_and_disable;

	/*
	 * Initialize tasks for the new child $sch without exiting them for
	 * $parent so that the tasks can always be reverted back to $parent
	 * sched on child init failure.
	 */
	WARN_ON_ONCE(scx_enabling_sub_sched);
	scx_enabling_sub_sched = sch;

	scx_task_iter_ప్రారంభం(&sti, sch->cgrp);
	అయితే ((p = scx_task_iter_తదుపరి_locked(&sti))) {
		నిర్మాణం rq *rq;
		నిర్మాణం rq_flags rf;

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
		ఒకవేళ (p->scx.flags & SCX_TASK_SUB_INIT)
			కొనసాగు;

		/* @p is pinned by the iter; see scx_sub_disable() */
		get_task_struct(p);

		ఒకవేళ (!assert_task_ready_or_enabled(p)) {
			ret = -EINVAL;
			వెళ్ళు abort;
		}

		scx_task_iter_unlock(&sti);

		/*
		 * As $p is still on $parent, it can't be transitioned to INIT.
		 * Let's worry about task state later. Use __scx_init_task().
		 */
		ret = __scx_init_task(sch, p, ఖాళీ, false);
		ఒకవేళ (ret)
			వెళ్ళు abort;

		rq = task_rq_తాళం_2(p, &rf);

		ఒకవేళ (scx_get_task_state(p) == SCX_TASK_DEAD) {
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
			కొనసాగు;
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
	scx_task_iter_ప్రారంభం(&sti, sch->cgrp);
	అయితే ((p = scx_task_iter_తదుపరి_locked(&sti))) {
		/*
		 * Use clearing of %SCX_TASK_SUB_INIT to detect and skip
		 * duplicate iterations.
		 */
		ఒకవేళ (!(p->scx.flags & SCX_TASK_SUB_INIT))
			కొనసాగు;

		scoped_guard (sched_change, p, వరుస_నుండి_తీయి_SAVE | వరుస_నుండి_తీయి_MOVE) {
			/*
			 * $p must be either READY or ENABLED. If ENABLED,
			 * __scx_disabled_and_exit_task() first disables and
			 * makes it READY. However, after exiting $p, it will
			 * leave $p as READY.
			 */
			assert_task_ready_or_enabled(p);
			__scx_disable_and_exit_task(మాతృ, p);

			/*
			 * $p is now only initialized for @sch and READY, which
			 * is what we want. Assign it to @sch and, if it's on
			 * the ext class, enable. A non-ext task, possible under
			 * an %SCX_OPS_SWITCH_PARTIAL root, stays READY and is
			 * enabled by switching_to_scx() if it switches over.
			 */
			scx_సమితి_task_sched(p, sch);
			ఒకవేళ (p->sched_class == &ext_sched_class)
				scx_enable_task(sch, p);

			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);

	scx_enabling_sub_sched = ఖాళీ;

	scx_cgroup_unlock();
	percpu_పైకి_write(&scx_fork_rwsem);

	scx_bypass(sch, false);

	/* @sch is enabled; deliver any caps owed since its sub_attach() */
	scx_sub_విత్తనం_caps(sch);

	pr_info("sched_ext: BPF sub-scheduler \"%s\" enabled\n", sch->ops.name);
	kobject_uevent(&sch->kobj, KOBJ_కూడు);
	ret = 0;
	వెళ్ళు బహిర్గతం_unlock;

బహిర్గతం_put_cgrp:
	cgroup_put(cgrp);
బహిర్గతం_unlock:
	mutex_unlock(&scx_enable_mutex);
	cmd->ret = ret;
	తిరిగి;

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
	scx_task_iter_ప్రారంభం(&sti, sch->cgrp);
	అయితే ((p = scx_task_iter_తదుపరి_locked(&sti))) {
		ఒకవేళ (p->scx.flags & SCX_TASK_SUB_INIT) {
			scx_sub_init_cancel_task(sch, p);
			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);
	scx_enabling_sub_sched = ఖాళీ;
err_unlock_and_disable:
	/* we'll soon enter disable path, keep bypass on */
	scx_cgroup_unlock();
	percpu_పైకి_write(&scx_fork_rwsem);
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
స్థిర s32 scx_cgroup_task_migrating(నిర్మాణం cgroup_task_migrate_ctx *ctx)
{
	నిర్మాణం task_struct *p = ctx->task;
	నిర్మాణం scx_sched *గమ్యం;
	పూర్ణాంకం ret;

	/*
	 * Cleared under scx_cgroup_lock() before root disable starts tearing
	 * down tasks. As cgroup_mutex is held, a set flag guarantees that the
	 * teardown loop is not running concurrently.
	 */
	ఒకవేళ (!scx_cgroup_enabled)
		తిరిగి NOTIFY_OK;

	గమ్యం = scx_cgroup_sched(ctx->dst_dcgrp);
	ఒకవేళ (scx_task_on_sched(గమ్యం, p))
		తిరిగి NOTIFY_OK;

	ret = __scx_init_task(గమ్యం, p, ctx->dst_dcgrp, false);
	ఒకవేళ (ret)
		తిరిగి notifier_from_errno(ret);

	తిరిగి NOTIFY_OK;
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
స్థిర శూన్య scx_cgroup_task_migrated(నిర్మాణం cgroup_task_migrate_ctx *ctx)
{
	నిర్మాణం task_struct *p = ctx->task;
	నిర్మాణం scx_sched *గమ్యం;
	నిర్మాణం rq *rq;
	నిర్మాణం rq_flags rf;

	ఒకవేళ (!scx_cgroup_enabled)
		తిరిగి;

	గమ్యం = scx_cgroup_sched(ctx->dst_dcgrp);
	ఒకవేళ (scx_task_on_sched(గమ్యం, p))
		తిరిగి;

	rq = task_rq_తాళం_2(p, &rf);
	scx_rehome_task(గమ్యం, p);
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
స్థిర శూన్య scx_cgroup_task_migrate_canceled(నిర్మాణం cgroup_task_migrate_ctx *ctx)
{
	నిర్మాణం task_struct *p = ctx->task;
	నిర్మాణం scx_sched *గమ్యం;
	నిర్మాణం rq *rq;
	నిర్మాణం rq_flags rf;

	ఒకవేళ (!scx_cgroup_enabled)
		తిరిగి;

	గమ్యం = scx_cgroup_sched(ctx->dst_dcgrp);
	ఒకవేళ (scx_task_on_sched(గమ్యం, p))
		తిరిగి;

	rq = task_rq_తాళం_2(p, &rf);
	scx_sub_init_cancel_task(గమ్యం, p);
	task_rq_unlock(rq, p, &rf);
}

స్థిర s32 scx_cgroup_lifetime_notify(నిర్మాణం notifier_ఖండం *nb,
				      గుర్తులేని దీర్ఘ action, శూన్య *దత్తాంశం)
{
	నిర్మాణం cgroup *cgrp = దత్తాంశం;
	నిర్మాణం cgroup *మాతృ = cgroup_మాతృ(cgrp);
	నిర్మాణం scx_sched *sch;

	ఒకవేళ (!cgroup_on_dfl(cgrp))
		తిరిగి NOTIFY_OK;

	ఎంపిక (action) {
	స్థితి CGROUP_LIFETIME_ONLINE:
		/* inherit ->scx_sched from $parent */
		ఒకవేళ (మాతృ)
			rcu_కేటాయించు_సూచిక(cgrp->scx_sched, scx_cgroup_sched(మాతృ));
		ఆపు;
	స్థితి CGROUP_LIFETIME_OFFLINE:
		/* if there is a sched attached, shoot it down */
		sch = scx_cgroup_sched(cgrp);
		ఒకవేళ (sch && sch->cgrp == cgrp)
			scx_exit(sch, SCX_EXIT_UNREG_KERN,
				 SCX_ECODE_RSN_CGROUP_OFFLINE,
				 "cgroup %llu going offline", cgroup_id(cgrp));
		ఆపు;
	}

	తిరిగి NOTIFY_OK;
}

స్థిర నిర్మాణం notifier_ఖండం scx_cgroup_lifetime_nb = {
	.notifier_call = scx_cgroup_lifetime_notify,
};

స్థిర s32 scx_cgroup_task_notify(నిర్మాణం notifier_ఖండం *nb,
				  గుర్తులేని దీర్ఘ action, శూన్య *దత్తాంశం)
{
	నిర్మాణం cgroup_task_migrate_ctx *ctx = దత్తాంశం;

	ఎంపిక (action) {
	స్థితి CGROUP_TASK_MIGRATING:
		తిరిగి scx_cgroup_task_migrating(ctx);
	స్థితి CGROUP_TASK_MIGRATED:
		scx_cgroup_task_migrated(ctx);
		ఆపు;
	స్థితి CGROUP_TASK_MIGRATE_CANCELED:
		scx_cgroup_task_migrate_canceled(ctx);
		ఆపు;
	}

	తిరిగి NOTIFY_OK;
}

స్థిర నిర్మాణం notifier_ఖండం scx_cgroup_task_nb = {
	.notifier_call = scx_cgroup_task_notify,
};

స్థిర s32 __init scx_cgroup_notifier_init(శూన్య)
{
	s32 ret;

	ret = blocking_notifier_chain_రిజిస్టరు(&cgroup_lifetime_notifier,
					       &scx_cgroup_lifetime_nb);
	ఒకవేళ (ret)
		తిరిగి ret;

	తిరిగి blocking_notifier_chain_రిజిస్టరు(&cgroup_task_notifier,
						&scx_cgroup_task_nb);
}
core_initcall(scx_cgroup_notifier_init);

స్థిర శూన్య scx_pstack_recursion(నిర్మాణం bpf_prog *prog, స్థిరరాశి అక్షరం *op)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(prog->aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి;

	scx_error(sch, "%s recursion detected", op);
}

శూన్య scx_pstack_recursion_on_dispatch(నిర్మాణం bpf_prog *prog)
{
	scx_pstack_recursion(prog, "dispatch");
}

శూన్య scx_pstack_recursion_on_caps_updated(నిర్మాణం bpf_prog *prog)
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
__bpf_kfunc bool scx_bpf_sub_dispatch(u64 cgroup_id, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం rq *rq = scx_locked_rq();
	నిర్మాణం scx_sched *మాతృ, *బిడ్డ;

	guard(rcu)();
	మాతృ = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!మాతృ))
		తిరిగి false;

	బిడ్డ = scx_కనుగొను_sub_sched(cgroup_id);

	ఒకవేళ (unlikely(!బిడ్డ))
		తిరిగి false;

	ఒకవేళ (unlikely(scx_మాతృ(బిడ్డ) != మాతృ)) {
		scx_error(మాతృ, "trying to dispatch a distant sub-sched on cgroup %llu",
			  cgroup_id);
		తిరిగి false;
	}

	/*
	 * Skip a child that does not effectively hold the base cap on this cpu:
	 * its inserts would only be rejected. ecaps are synced at the top of
	 * dispatch_one() before dispatch, so this reflects the in-effect state.
	 */
	ఒకవేళ (scx_missing_caps(బిడ్డ, cpu_యొక్క(rq), SCX_CAP_పునాది))
		తిరిగి false;

	తిరిగి scx_dispatch_sched(బిడ్డ, rq, rq->scx.sub_dispatch_prev, true) !=
		SCX_DSP_NONE;
}

/* Validate common inputs. On success, *parent_out and *child_out are set. */
స్థిర s32 sub_cap_preamble(u64 cgroup_id, u64 caps, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux,
			    నిర్మాణం scx_sched **మాతృ_బహిర్గతం, నిర్మాణం scx_sched **బిడ్డ_బహిర్గతం)
{
	నిర్మాణం scx_sched *మాతృ, *బిడ్డ;

	మాతృ = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!మాతృ))
		తిరిగి -ENODEV;

	ఒకవేళ (!scx_is_cid_type()) {
		scx_error(మాతృ, "sub-cap kfuncs require a cid-form scheduler");
		తిరిగి -EOPNOTSUPP;
	}

	బిడ్డ = scx_కనుగొను_sub_sched(cgroup_id);
	ఒకవేళ (unlikely(!బిడ్డ))
		తిరిగి -ENODEV;

	ఒకవేళ (unlikely(scx_మాతృ(బిడ్డ) != మాతృ)) {
		scx_error(మాతృ, "%s: sub-%llu is not a direct child",
			  మాతృ->cgrp_మార్గం, cgroup_id);
		తిరిగి -EINVAL;
	}

	ఒకవేళ (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(మాతృ, "invalid caps 0x%llx", caps);
		తిరిగి -EINVAL;
	}

	*మాతృ_బహిర్గతం = మాతృ;
	*బిడ్డ_బహిర్గతం = బిడ్డ;
	తిరిగి 0;
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
				  స్థిరరాశి నిర్మాణం scx_cmask *cmask__arena,
				  నిర్మాణం scx_cmask *denied_బహిర్గతం_arena_nullable,
				  స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_cmask_ref ref, denied_ref;
	నిర్మాణం scx_sched *మాతృ, *బిడ్డ;
	bool any_denied = false;
	LIST_తల(గమ్యం_deliver);
	s32 si, ret;

	guard(irqsave)();

	ret = sub_cap_preamble(cgroup_id, caps, aux, &మాతృ, &బిడ్డ);
	ఒకవేళ (ret)
		తిరిగి ret;

	ret = scx_cmask_ref_init(మాతృ, cmask__arena, &ref);
	ఒకవేళ (ret) {
		scx_error(మాతృ, "invalid cmask (%d)", ret);
		తిరిగి ret;
	}

	ఒకవేళ (denied_బహిర్గతం_arena_nullable) {
		ret = scx_cmask_ref_init(మాతృ, denied_బహిర్గతం_arena_nullable, &denied_ref);
		ఒకవేళ (ret) {
			scx_error(మాతృ, "invalid denied_out (%d)", ret);
			తిరిగి ret;
		}
	}

	/* apply the grant one shard at a time */
	కొరకు (si = ref.shard_మొదటి; si < ref.shard_చివర; si++) {
		SCX_CMASK_DEFINE_SHARD(ముక్క, 0, SCX_CID_SHARD_MAX_CPUS);
		నిర్మాణం scx_pshard *pps = మాతృ->pshard[si];
		నిర్మాణం scx_pshard *cps = బిడ్డ->pshard[si];
		u64 granted_caps = 0;
		u32 cap_బిట్టు;

		scx_cmask_ref_shard(&ref, si, ముక్క);
		ఒకవేళ (scx_cmask_ఖాళీలు(ముక్క))
			కొనసాగు;

		SCX_CMASK_DEFINE_SHARD(granted_cids, ముక్క->పునాది, ముక్క->nr_cids);
		SCX_CMASK_DEFINE_SHARD(మారింది_cids, ముక్క->పునాది, ముక్క->nr_cids);
		SCX_CMASK_DEFINE_SHARD(delta, ముక్క->పునాది, ముక్క->nr_cids);

		scx_cmask_copy(granted_cids, ముక్క);

		scoped_guard (raw_spinlock, &pps->తాళం) {
			guard(raw_spinlock_nested)(&cps->తాళం);

			/*
			 * Narrow granted_cids to cids the parent holds every
			 * requested cap on. All-or-nothing per cid.
			 */
			scx_for_each_cap_బిట్టు(cap_బిట్టు, caps)
				scx_cmask_and(granted_cids, &pps->caps[cap_బిట్టు].cmask);

			/*
			 * For each requested cap, fold the newly-set cids into
			 * the child and accumulate the delta.
			 */
			scx_for_each_cap_బిట్టు(cap_బిట్టు, caps) {
				నిర్మాణం scx_cmask *ccm = &cps->caps[cap_బిట్టు].cmask;

				scx_cmask_copy(delta, granted_cids);
				scx_cmask_andnot(delta, ccm);
				ఒకవేళ (scx_cmask_ఖాళీలు(delta))
					కొనసాగు;

				scx_cmask_or(ccm, delta);
				scx_cmask_or(మారింది_cids, delta);
				granted_caps |= బిట్టు_U64(cap_బిట్టు);
			}

			ఒకవేళ (granted_caps) {
				s32 cid;

				caps_updated_రికార్డు(cps, మారింది_cids, granted_caps,
						    &గమ్యం_deliver);
				/*
				 * The sync arms an update_idle() re-notify if
				 * the cid gains baseline access, so the holder
				 * learns of an already-idle cid.
				 */
				scx_cmask_for_each_cid(cid, మారింది_cids)
					వరుస_sync_ecaps(బిడ్డ, cid);
			}
		}

		/* record cids that didn't make it into the denied mask */
		ఒకవేళ (!scx_cmask_subset(ముక్క, granted_cids)) {
			any_denied = true;
			ఒకవేళ (denied_బహిర్గతం_arena_nullable) {
				SCX_CMASK_DEFINE_SHARD(denied, ముక్క->పునాది, ముక్క->nr_cids);

				scx_cmask_copy(denied, ముక్క);
				scx_cmask_andnot(denied, granted_cids);
				scx_cmask_ref_or(&denied_ref, denied);
			}
		}
	}

	caps_updated_deliver(&గమ్యం_deliver);

	తిరిగి any_denied ? -EPERM : 0;
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
__bpf_kfunc శూన్య scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    స్థిరరాశి నిర్మాణం scx_cmask *cmask__arena,
				    స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_cmask_ref ref;
	నిర్మాణం scx_sched *మాతృ, *బిడ్డ, *pos;
	LIST_తల(గమ్యం_deliver);
	s32 si, ret;

	guard(irqsave)();

	ఒకవేళ (sub_cap_preamble(cgroup_id, caps, aux, &మాతృ, &బిడ్డ))
		తిరిగి;

	ret = scx_cmask_ref_init(మాతృ, cmask__arena, &ref);
	ఒకవేళ (ret) {
		scx_error(మాతృ, "invalid cmask (%d)", ret);
		తిరిగి;
	}

	/* per-shard, walk child's subtree and clear @caps */
	కొరకు (si = ref.shard_మొదటి; si < ref.shard_చివర; si++) {
		SCX_CMASK_DEFINE_SHARD(ముక్క, 0, SCX_CID_SHARD_MAX_CPUS);

		scx_cmask_ref_shard(&ref, si, ముక్క);
		ఒకవేళ (scx_cmask_ఖాళీలు(ముక్క))
			కొనసాగు;

		/*
		 * Pre-order with subtree skip: a descendant that cleared
		 * nothing means no descendant of it can hold @caps on these
		 * cids either.
		 */
		guard(raw_spinlock)(&మాతృ->pshard[si]->తాళం);
		pos = scx_తదుపరి_descendant_pre(ఖాళీ, బిడ్డ);
		అయితే (pos) {
			నిర్మాణం scx_pshard *ps = pos->pshard[si];
			SCX_CMASK_DEFINE_SHARD(మారింది_cids, ముక్క->పునాది, ముక్క->nr_cids);
			SCX_CMASK_DEFINE_SHARD(delta, ముక్క->పునాది, ముక్క->nr_cids);
			u64 revoked_caps = 0;
			u32 cap_బిట్టు;

			scoped_guard (raw_spinlock_nested, &ps->తాళం) {
				/*
				 * For each cap, clear lost cids and accumulate
				 * the per-cap diff for notification.
				 */
				scx_for_each_cap_బిట్టు(cap_బిట్టు, caps) {
					నిర్మాణం scx_cmask *cm = &ps->caps[cap_బిట్టు].cmask;

					scx_cmask_copy(delta, cm);
					scx_cmask_and(delta, ముక్క);
					ఒకవేళ (scx_cmask_ఖాళీలు(delta))
						కొనసాగు;

					scx_cmask_andnot(cm, delta);
					scx_cmask_or(మారింది_cids, delta);
					revoked_caps |= బిట్టు_U64(cap_బిట్టు);
				}

				ఒకవేళ (revoked_caps) {
					s32 cid;

					caps_updated_రికార్డు(ps, మారింది_cids, revoked_caps,
							    &గమ్యం_deliver);
					scx_cmask_for_each_cid(cid, మారింది_cids)
						వరుస_sync_ecaps(pos, cid);
				}
			}

			ఒకవేళ (revoked_caps)
				pos = scx_తదుపరి_descendant_pre(pos, బిడ్డ);
			లేకపోతే
				pos = scx_skip_subtree_pre(pos, బిడ్డ);
		}
	}

	caps_updated_deliver(&గమ్యం_deliver);
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
__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, నిర్మాణం scx_cmask *బహిర్గతం_arena,
				 స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_cmask_ref ref;
	నిర్మాణం scx_sched *sch, *లక్ష్యం;
	నిర్మాణం scx_pshard **pshard;
	s32 si, ret;

	guard(irqsave)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

	ఒకవేళ (!scx_is_cid_type()) {
		scx_error(sch, "sub-cap kfuncs require a cid-form scheduler");
		తిరిగి -EOPNOTSUPP;
	}

	ఒకవేళ (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(sch, "invalid caps 0x%llx", caps);
		తిరిగి -EINVAL;
	}

	/* @cgroup_id 0 reads self, otherwise a direct child */
	ఒకవేళ (cgroup_id) {
		లక్ష్యం = scx_కనుగొను_sub_sched(cgroup_id);
		ఒకవేళ (unlikely(!లక్ష్యం))
			తిరిగి -ENODEV;
		ఒకవేళ (unlikely(scx_మాతృ(లక్ష్యం) != sch)) {
			scx_error(sch, "%s: sub-%llu is not a direct child",
				  sch->cgrp_మార్గం, cgroup_id);
			తిరిగి -EINVAL;
		}
	} లేకపోతే {
		లక్ష్యం = sch;
	}

	/*
	 * The target's caps storage may not be set up yet (e.g. a self-read
	 * during ops.init_cids()). Pairs with the publish in
	 * scx_alloc_pshards(): a non-NULL pshard has every element set and the
	 * acquire also orders the cid table reads below against it.
	 */
	pshard = smp_ఎక్కించు_acquire(&లక్ష్యం->pshard);
	ఒకవేళ (unlikely(!pshard)) {
		scx_error(sch, "scx_bpf_sub_caps() called before caps storage is initialized");
		తిరిగి -ENODEV;
	}

	ret = scx_cmask_ref_init(sch, బహిర్గతం_arena, &ref);
	ఒకవేళ (ret) {
		scx_error(sch, "invalid out (%d)", ret);
		తిరిగి ret;
	}

	కొరకు (si = ref.shard_మొదటి; si < ref.shard_చివర; si++) {
		స్థిరరాశి నిర్మాణం scx_cid_shard *shard =
			&rcu_dereference_all(scx_cid_shard_ranges)[si];
		SCX_CMASK_DEFINE_SHARD(local_బహిర్గతం, shard->పునాది_cid, shard->nr_cids);
		u32 cap_బిట్టు;

		scx_for_each_cap_బిట్టు(cap_బిట్టు, caps)
			scx_cmask_or(local_బహిర్గతం, &pshard[si]->caps[cap_బిట్టు].cmask);
		scx_cmask_ref_copy(&ref, local_బహిర్గతం);
	}
	తిరిగి 0;
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
__bpf_kfunc s32 scx_bpf_sub_kill_bstr(u64 cgroup_id, అక్షరం *fmt,
				      గుర్తులేని దీర్ఘ దీర్ఘ *దత్తాంశం, u32 దత్తాంశం_sz,
				      స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *మాతృ, *బిడ్డ;

	guard(rcu)();

	మాతృ = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!మాతృ))
		తిరిగి -ENODEV;

	ఒకవేళ (!scx_is_cid_type()) {
		scx_error(మాతృ, "sub-cap kfuncs require a cid-form scheduler");
		తిరిగి -EOPNOTSUPP;
	}

	బిడ్డ = scx_కనుగొను_sub_sched(cgroup_id);
	ఒకవేళ (unlikely(!బిడ్డ))
		తిరిగి -ENODEV;

	ఒకవేళ (unlikely(scx_మాతృ(బిడ్డ) != మాతృ)) {
		scx_error(మాతృ, "%s: sub-%llu is not a direct child",
			  మాతృ->cgrp_మార్గం, cgroup_id);
		తిరిగి -EINVAL;
	}

	scx_exit_bstr(బిడ్డ, SCX_EXIT_మాతృ_KILL, 0, మాతృ, fmt, దత్తాంశం, దత్తాంశం_sz);
	తిరిగి 0;
}

__bpf_kfunc_end_defs();

#లేకపోతే	/* !CONFIG_EXT_SUB_SCHED */

__bpf_kfunc_start_defs();

__bpf_kfunc s32 scx_bpf_sub_grant(u64 cgroup_id, u64 caps,
				  స్థిరరాశి నిర్మాణం scx_cmask *cmask__arena,
				  నిర్మాణం scx_cmask *denied_బహిర్గతం_arena_nullable,
				  స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	తిరిగి -EOPNOTSUPP;
}

__bpf_kfunc శూన్య scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    స్థిరరాశి నిర్మాణం scx_cmask *cmask__arena,
				    స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
}

__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, నిర్మాణం scx_cmask *బహిర్గతం_arena,
				 స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	తిరిగి -EOPNOTSUPP;
}

__bpf_kfunc s32 scx_bpf_sub_kill_bstr(u64 cgroup_id, అక్షరం *fmt,
				      గుర్తులేని దీర్ఘ దీర్ఘ *దత్తాంశం, u32 దత్తాంశం_sz,
				      స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	తిరిగి -EOPNOTSUPP;
}

__bpf_kfunc_end_defs();

#అయితే_ముగింపు	/* CONFIG_EXT_SUB_SCHED */
