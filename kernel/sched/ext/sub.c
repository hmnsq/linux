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
#شامل <linux/rhashtable.h>
#شامل "internal.h"
#شامل "cid.h"
#شامل "arena.h"
#شامل "sub.h"
#شامل "inlines.h"

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED

/*
 * On while any sub-scheduler exists so that a root-only system doesn't pay for
 * the sub-sched portions of hot paths. See scx_has_subs().
 */
DEFINE_STATIC_کلید_FALSE(__scx_has_subs);

/* latched at root enable before any rescue runs */
ساکن s32 scx_rescue_bw_1024;
ساکن s64 scx_rescue_وقتی_حصہ_ns;
ساکن s64 scx_rescue_sat_delta_ns;
ساکن غیرعلامتی طویل scx_rescue_decay_halflife;
ساکن غیرعلامتی طویل scx_rescue_overload_after;

/**
 * scx_skip_subtree_pre - Skip @pos's subtree in a pre-order walk
 * @pos: current position
 * @root: walk root
 *
 * In a walk started by scx_next_descendant_pre(), continue past @pos's subtree:
 * return @pos's next sibling, or the closest ancestor's next sibling, or NULL
 * if @pos's subtree is the last under @root. Same locking rules.
 */
ساخت scx_sched *scx_skip_subtree_pre(ساخت scx_sched *pos, ساخت scx_sched *جڑ)
{
	ساخت scx_sched *اگلا;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_تالا) ||
		       rcu_read_تالا_any_held());

	جب (pos != جڑ) {
		اگلا = list_اگلا_or_null_rcu(&scx_والد(pos)->children, &pos->sibling,
					     ساخت scx_sched, sibling);
		اگر (اگلا)
			واپس اگلا;
		pos = scx_والد(pos);
	}
	واپس تہی;
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
ساخت scx_sched *scx_اگلا_descendant_pre(ساخت scx_sched *pos, ساخت scx_sched *جڑ)
{
	ساخت scx_sched *اگلا;

	lockdep_assert(lockdep_is_held(&scx_enable_mutex) ||
		       lockdep_is_held(&scx_sched_تالا) ||
		       rcu_read_تالا_any_held());

	/* if first iteration, visit @root */
	اگر (!pos)
		واپس جڑ;

	/* visit the first child if exists */
	اگلا = list_پہلا_or_null_rcu(&pos->children, ساخت scx_sched, sibling);
	اگر (اگلا)
		واپس اگلا;

	/* no child, visit my or the closest ancestor's next sibling */
	واپس scx_skip_subtree_pre(pos, جڑ);
}

ساکن ساخت scx_sched *scx_ڈھونڈو_sub_sched(u64 cgroup_id)
{
	واپس rhashtable_lookup(&scx_sched_hash, &cgroup_id,
				 scx_sched_hash_params);
}

خالی scx_سیٹ_task_sched(ساخت task_struct *p, ساخت scx_sched *sch)
{
	rcu_تفویض_اشارہ_گر(p->scx.sched, sch);
}

ساخت cgroup *sch_cgroup(ساخت scx_sched *sch)
{
	واپس sch->cgrp;
}

/* for each descendant of @cgrp including self, set ->scx_sched to @sch */
خالی سیٹ_cgroup_sched(ساخت cgroup *cgrp, ساخت scx_sched *sch)
{
	ساخت cgroup *pos;
	ساخت cgroup_subsys_state *css;

	cgroup_for_each_زندہ_descendant_pre(pos, css, cgrp)
		rcu_تفویض_اشارہ_گر(pos->scx_sched, sch);
}

ساکن خالی free_pshard(ساخت scx_pshard *pshard)
{
	ساخت scx_caps_updated *cu;

	اگر (!pshard)
		واپس;
	cu = &pshard->caps_updated;
	اگر (cu->cmask_arena_برآمد)
		scx_arena_free(pshard->sch, cu->cmask_arena_برآمد,
			       struct_سائز_t(ساخت scx_cmask, بٹ,
					     SCX_CMASK_NR_الفاظ(pshard->nr_cids)));
	kfree(pshard);
}

خالی scx_free_pshards(ساخت scx_sched *sch)
{
	s32 si;

	اگر (!sch->pshard)
		واپس;
	برائے (si = 0; si < sch->nr_pshards; si++)
		free_pshard(sch->pshard[si]);
	kfree(sch->pshard);
}

ساکن ساخت scx_pshard *alloc_pshard(ساخت scx_sched *sch, s32 shard_idx, s32 گرہ)
{
	ثابت ساخت scx_cid_shard *shard =
		&rcu_dereference_protected(scx_cid_shard_ranges,
					   lockdep_is_held(&scx_enable_mutex))[shard_idx];
	سائز_t cmask_سائز = struct_سائز_t(ساخت scx_cmask, بٹ,
					  SCX_CMASK_NR_الفاظ(shard->nr_cids));
	ساخت scx_pshard *pshard;
	ساخت scx_caps_updated *cu;
	s32 i;

	pshard = kzalloc_گرہ(حجم(*pshard), GFP_KERNEL, گرہ);
	اگر (!pshard)
		واپس تہی;

	raw_spin_تالا_init(&pshard->تالا);
	pshard->sch = sch;
	pshard->بنیاد = shard->بنیاد_cid;
	pshard->nr_cids = shard->nr_cids;

	برائے (i = 0; i < __SCX_NR_CAPS; i++)
		scx_cmask_init(&pshard->caps[i].cmask, shard->بنیاد_cid, shard->nr_cids);

	cu = &pshard->caps_updated;
	raw_spin_تالا_init(&cu->تالا);
	INIT_LIST_سر(&cu->گرہ_اندر_flight);
	__scx_cmask_init(&cu->cmask, shard->بنیاد_cid, shard->nr_cids, SCX_CID_SHARD_MAX_CPUS);

	cu->cmask_arena_برآمد = scx_arena_alloc(sch, cmask_سائز);
	اگر (!cu->cmask_arena_برآمد) {
		free_pshard(pshard);
		واپس تہی;
	}

	scx_cmask_init(cu->cmask_arena_برآمد, shard->بنیاد_cid, shard->nr_cids);

	واپس pshard;
}

s32 scx_alloc_pshards(ساخت scx_sched *sch)
{
	ساخت scx_pshard **pshard;
	s32 *shard_گرہ;
	s32 si;

	اگر (!sch->is_cid_type || !sch->arena_pool)
		واپس 0;

	shard_گرہ = rcu_dereference_protected(scx_shard_گرہ,
					       lockdep_is_held(&scx_enable_mutex));

	pshard = kzalloc_objs(pshard[0], scx_nr_cid_shards);
	اگر (!pshard)
		واپس -ENOMEM;

	برائے (si = 0; si < scx_nr_cid_shards; si++) {
		pshard[si] = alloc_pshard(sch, si, shard_گرہ[si]);
		اگر (!pshard[si]) {
			جب (--si >= 0)
				free_pshard(pshard[si]);
			kfree(pshard);
			واپس -ENOMEM;
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
	واپس 0;
}

/*
 * Seed the root's caps fully. Root owns all cids on all caps at enable time.
 * Children acquire caps via scx_bpf_sub_grant().
 */
خالی scx_init_جڑ_caps(ساخت scx_sched *sch)
{
	s32 si, i;

	برائے (si = 0; si < sch->nr_pshards; si++) {
		ساخت scx_pshard *ps = sch->pshard[si];

		برائے (i = 0; i < __SCX_NR_CAPS; i++)
			scx_cmask_fill(&ps->caps[i].cmask);
	}
}

/* unserved remainder of @rq's rescuee's admitted slice, 0 once fully served */
ساکن s64 scx_rescue_قاش_بقایا(ساخت rq *rq)
{
	s64 served = rq->scx.rescue.curr->se.مجموع_exec_runtime - rq->scx.rescue.exec_snap;

	واپس max(rq->scx.rescue.قاش - served, 0);
}

/*
 * Decay @pcpu's rescue usage average in place, halving per the knob-derived
 * halflife, see scx_rescue_set_knobs(). The timestamp advances only by whole
 * halflives.
 */
ساکن u64 scx_rescue_decay_avg(ساخت scx_sched_pcpu *pcpu)
{
	غیرعلامتی طویل halflife = scx_rescue_decay_halflife;
	u64 n = div_u64(get_jiffies_64() - pcpu->rescue_avg_at, halflife);

	اگر (n) {
		pcpu->rescue_avg = n < 64 ? pcpu->rescue_avg >> n : 0;
		pcpu->rescue_avg_at += n * halflife;
	}
	واپس pcpu->rescue_avg;
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
خالی scx_rescue_charge(ساخت rq *rq, s64 delta_exec)
{
	ساخت scx_sched_pcpu *pcpu;

	lockdep_assert_rq_held(rq);

	/*
	 * A rescue slice is bounded by one quantum and tick-driven expiry can
	 * overshoot by up to a tick. Clamp to avoid wild over-charges on VMs.
	 */
	delta_exec = min_t(s64, delta_exec, scx_rescue_وقتی_حصہ_ns + TICK_NSEC);

	rq->scx.rescue.budget -= delta_exec;

	/* per-cpu usage average feeds the overload victim pick */
	pcpu = فی_cpu_ptr(scx_task_sched(rq->curr)->pcpu, cpu_کا(rq));
	pcpu->rescue_avg = scx_rescue_decay_avg(pcpu) + delta_exec;

	اگر (!scx_rescue_قاش_بقایا(rq))
		scx_task_قاش_ended(rq, rq->scx.rescue.curr);
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
خالی scx_rescue_انتہا(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);

	rq->scx.rescue.curr = تہی;
	اگر (list_خالی_خانے(&rq->scx.rescue.dsq.list))
		rq->scx.rescue.budget = min(rq->scx.rescue.budget, scx_rescue_وقتی_حصہ_ns);
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
bool scx_rescue_keep(ساخت rq *rq, ساخت task_struct *p)
{
	s64 بقایا = scx_rescue_قاش_بقایا(rq);

	lockdep_assert_rq_held(rq);

	اگر (!بقایا || !(p->scx.flags & SCX_TASK_QUEUED) ||
	    scx_bypassing(scx_task_sched(p), cpu_کا(rq)))
		واپس false;

	scx_سیٹ_task_قاش(p, بقایا);
	واپس true;
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
ساکن خالی scx_rescue_accrue(ساخت rq *rq)
{
	bool اندر_session = rq->scx.rescue.curr || !list_خالی_خانے(&rq->scx.rescue.dsq.list);
	s64 cap = اندر_session ? 3 * scx_rescue_وقتی_حصہ_ns : scx_rescue_وقتی_حصہ_ns;
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
	اگر (delta >= scx_rescue_sat_delta_ns)
		rq->scx.rescue.budget = cap;
	ورنہ
		rq->scx.rescue.budget =
			min(cap, rq->scx.rescue.budget +
			    ((delta * scx_rescue_bw_1024) >> SCHED_استعداد_سرکاؤ));
}

/*
 * The slice for the next admission - the quantum divided across the stranded
 * tasks so that a crowded queue round-robins on shorter slices.
 */
ساکن s64 scx_rescue_اگلا_قاش(ساخت rq *rq)
{
	s64 min_قاش = max_t(s64, SCX_RESCUE_MIN_قاش_US * NSEC_فی_USEC, TICK_NSEC);
	u32 گہرائی = rq->scx.rescue.dsq.nr ?: 1;

	واپس clamp(div_s64(scx_rescue_وقتی_حصہ_ns, گہرائی), min_قاش, scx_rescue_وقتی_حصہ_ns);
}

ساکن خالی scx_rescue_timer_arm(ساخت rq *rq)
{
	ساخت timer_list *timer = &rq->scx.rescue.timer;
	s64 delay = scx_rescue_وقتی_حصہ_ns / 4;	/* should be granular enough */

	اگر (timer_pending(timer))
		واپس;

	/*
	 * While the head waiter can't be admitted because the bucket is short
	 * of a full quantum, stretch to the full funding delay.
	 */
	اگر (!rq->scx.rescue.curr && rq->scx.rescue.budget < scx_rescue_وقتی_حصہ_ns) {
		s64 deficit = scx_rescue_وقتی_حصہ_ns - rq->scx.rescue.budget;

		delay = max(delay,
			    div_s64(deficit << SCHED_استعداد_سرکاؤ, scx_rescue_bw_1024));
	}

	/* +1 rounds up so the beat is due by the time the timer fires */
	timer->expires = jiffies + nsecs_مقصد_jiffies(delay) + 1;
	جوڑو_timer_on(timer, cpu_کا(rq));
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
ساکن خالی scx_rescue_admit(ساخت rq *rq, ساخت task_struct *p, s64 قاش)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->scx.rescue.curr);

	rq->scx.rescue.curr = p;
	rq->scx.rescue.قاش = قاش;
	rq->scx.rescue.exec_snap = p->se.مجموع_exec_runtime;
	scx_سیٹ_task_قاش(p, قاش);
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
ساکن bool scx_rescue_try_admit(ساخت rq *rq, ساخت task_struct *p)
{
	scx_rescue_accrue(rq);

	اگر (!rq->scx.rescue.curr && list_خالی_خانے(&rq->scx.rescue.dsq.list) &&
	    rq->scx.rescue.budget >= scx_rescue_وقتی_حصہ_ns) {
		scx_rescue_admit(rq, p, scx_rescue_وقتی_حصہ_ns);
		واپس true;
	}

	scx_rescue_timer_arm(rq);
	واپس false;
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
ساکن خالی scx_rescue_check_overload(ساخت rq *rq)
{
	ساخت scx_sched *victim = تہی, *pos;
	ساخت task_struct *p;
	صحیح cpu = cpu_کا(rq);
	u64 max_avg = 0;
	u32 dur_ms;

	lockdep_assert_rq_held(rq);

	p = list_پہلا_اندراج_or_null(&rq->scx.rescue.dsq.list, ساخت task_struct,
				     scx.dsq_list.گرہ);
	اگر (!p)
		واپس;

	/* has the head waiter been queued for longer than the threshold? */
	اگر (وقت_before(jiffies, p->scx.rescue_at + scx_rescue_overload_after))
		واپس;

	/*
	 * Grace period after the last ejection on this cpu - the freed
	 * bandwidth gets one threshold's worth of time to drain the backlog
	 * before another sub is judged.
	 */
	اگر (وقت_before64(get_jiffies_64(), rq->scx.rescue.kill_at +
			  scx_rescue_overload_after))
		واپس;

	list_for_each_اندراج_rcu(pos, &scx_sched_all, all) {
		u64 avg = scx_rescue_decay_avg(فی_cpu_ptr(pos->pcpu, cpu));

		/* skip an already-exiting sub, else the ejection is wasted */
		اگر (pos->level && avg > max_avg &&
		    atomic_read(&pos->exit_قسم) == SCX_EXIT_NONE) {
			max_avg = avg;
			victim = pos;
		}
	}
	اگر (!victim)
		واپس;

	rq->scx.rescue.kill_at = get_jiffies_64();
	dur_ms = jiffies_مقصد_msecs(jiffies - p->scx.rescue_at);
	__scx_exit(victim, SCX_EXIT_ERROR_RESCUE, 0, cpu,
		   "used too much rescue CPU time (%llums) while %s[%d] waited %u.%03us to be rescued",
		   div_u64(max_avg, NSEC_فی_MSEC), p->comm, p->pid, dur_ms / 1000,
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
ساکن خالی scx_rescue_timerfn(ساخت timer_list *timer)
{
	ساخت rq *rq = timer_container_کا(rq, timer, scx.rescue.timer);
	ساخت task_struct *p;

	guard(rq_تالا_irqsave)(rq);

	p = rq->scx.rescue.curr;
	اگر (!p && list_خالی_خانے(&rq->scx.rescue.dsq.list))
		واپس;

	scx_rescue_accrue(rq);
	scx_rescue_check_overload(rq);

	اگر (!p) {
		s64 قاش = scx_rescue_اگلا_قاش(rq);

		/* no rescue in progress */
		اگر (rq->scx.rescue.budget < scx_rescue_وقتی_حصہ_ns)
			جاؤ برآمد_arm;

		/* there's enough budget to start rescuing the next one */
		p = list_پہلا_اندراج(&rq->scx.rescue.dsq.list, ساخت task_struct,
				     scx.dsq_list.گرہ);
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_rescue_admit(rq, p, قاش);
		scx_move_local_task_مقصد_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
		اگر (sched_class_above(&ext_sched_class, rq->curr->sched_class))
			resched_curr(rq);
	} ورنہ اگر (p->scx.dsq && rq->scx.rescue.budget > 2 * scx_rescue_وقتی_حصہ_ns) {
		/*
		 * The rescuee waited for the CPU for too long. Escalate - grant
		 * the unserved remainder, protect it from the schedulers and
		 * preempt the current task. The slice is set before the
		 * protection. Repeat beats only repeat the head move - the
		 * slice write is refused on a protected task.
		 */
		scx_سیٹ_task_قاش(p, scx_rescue_قاش_بقایا(rq));
		p->scx.flags |= SCX_TASK_PROTECTED;
		scx_task_unlink_from_dsq(p, &rq->scx.local_dsq);
		scx_move_local_task_مقصد_local_dsq(scx_task_sched(p), p,
					SCX_ENQ_سر | SCX_ENQ_PREEMPT | SCX_ENQ_IGNORE_CAPS,
					rq);
	}
برآمد_arm:
	scx_rescue_timer_arm(rq);
}

/* flush out tasks waiting for rescue before a CPU goes down */
خالی scx_rescue_flush(ساخت rq *rq)
{
	ساخت task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	/* end the current rescue */
	اگر (rq->scx.rescue.curr)
		scx_task_قاش_ended(rq, rq->scx.rescue.curr);

	/* and flush out all pending ones */
	list_for_each_اندراج_محفوظ(p, n, &rq->scx.rescue.dsq.list, scx.dsq_list.گرہ) {
		scx_task_unlink_from_dsq(p, &rq->scx.rescue.dsq);
		scx_move_local_task_مقصد_local_dsq(scx_task_sched(p), p,
						 SCX_ENQ_IGNORE_CAPS, rq);
	}

	timer_delete(&rq->scx.rescue.timer);
}

خالی scx_rescue_dump(ساخت seq_buf *s, ساخت rq *rq)
{
	ساخت task_struct *p = rq->scx.rescue.curr;

	scx_dump_سطر(s, "          rescue=%u budget=%lldus rescuing=%s[%d]",
		      rq->scx.rescue.dsq.nr,
		      div_s64(rq->scx.rescue.budget, NSEC_فی_USEC),
		      p ? p->comm : "none", p ? p->pid : -1);
}

/*
 * A scheduler whose stall watchdog is shorter than the overload threshold gets
 * stall-killed over its parked waiters before the overload check can eject the
 * actual top consumer. The root's knobs set the threshold, warn on any
 * scheduler that doesn't fit it.
 */
ساکن خالی scx_rescue_check_timeout(ساخت scx_sched *sch)
{
	اگر (!scx_rescue_bw_1024 || sch->watchdog_timeout > scx_rescue_overload_after)
		واپس;

	pr_warn("sched_ext: %s: watchdog timeout %ums <= rescue overload threshold %ums\n",
		sch->ops.name, jiffies_مقصد_msecs(sch->watchdog_timeout),
		jiffies_مقصد_msecs(scx_rescue_overload_after));
}

/* latch the rescue parameters on root scheduler enable */
خالی scx_rescue_سیٹ_knobs(ساخت scx_sched *sch)
{
	s32 bw_ppt = sch->ops.rescue_bandwidth_ppt ?: SCX_RESCUE_DFL_BW_PPT;
	s64 وقتی_حصہ_us = sch->ops.rescue_وقتی_حصہ_us ?: SCX_RESCUE_DFL_وقتی_حصہ_US;
	s64 period_ns;

	اگر (sch->ops.rescue_bandwidth_ppt == SCX_RESCUE_DISABLE) {
		scx_rescue_bw_1024 = 0;
		واپس;
	}

	scx_rescue_bw_1024 = bw_ppt * SCHED_استعداد_SCALE / 1000;
	scx_rescue_وقتی_حصہ_ns = max(وقتی_حصہ_us * NSEC_فی_USEC, TICK_NSEC);
	scx_rescue_sat_delta_ns =
		div_s64((4 * scx_rescue_وقتی_حصہ_ns + TICK_NSEC) << SCHED_استعداد_سرکاؤ,
			scx_rescue_bw_1024);

	/*
	 * The overload threshold and the decay halflife scale with the funding
	 * period - the time the bucket takes to fund one full quantum.
	 */
	period_ns = div_s64(scx_rescue_وقتی_حصہ_ns << SCHED_استعداد_سرکاؤ, scx_rescue_bw_1024);
	scx_rescue_overload_after =
		clamp(nsecs_مقصد_jiffies(SCX_RESCUE_OVERLOAD_MULT * period_ns),
		      msecs_مقصد_jiffies(SCX_RESCUE_MIN_OVERLOAD_MS),
		      msecs_مقصد_jiffies(SCX_RESCUE_MAX_OVERLOAD_MS));
	scx_rescue_decay_halflife = scx_rescue_overload_after / 4;

	/* a single in-budget wait must not cross the overload trigger */
	اگر (nsecs_مقصد_jiffies(period_ns) > scx_rescue_overload_after / 2)
		pr_warn("sched_ext: %s: rescue funding period %lldms > overload threshold %ums / 2\n",
			sch->ops.name, div_s64(period_ns, NSEC_فی_MSEC),
			jiffies_مقصد_msecs(scx_rescue_overload_after));

	scx_rescue_check_timeout(sch);
}

خالی scx_rescue_init(ساخت rq *rq)
{
	BUG_ON(scx_init_dsq(&rq->scx.rescue.dsq, SCX_DSQ_RESCUE, تہی));
	timer_تیاری(&rq->scx.rescue.timer, scx_rescue_timerfn, TIMER_PINNED);
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
ساخت scx_dispatch_q *scx_resolve_local_dsq(ساخت scx_sched *sch, ساخت rq *rq,
					     ساخت task_struct *p, u64 *enq_flags)
{
	اگر (!scx_has_subs())
		واپس &rq->scx.local_dsq;

	s32 cid = __scx_cpu_to_cid(cpu_کا(rq));
	ساخت scx_sched *asch = rq->scx.remote_activate_sch ?: sch;
	u64 needed = scx_caps_for_enq(*enq_flags);
	u64 missing;

	/*
	 * On a remote activation the scheduling sched (@asch) differs from
	 * @p's owner (@sch). Check caps against the scheduling sched.
	 */
	اگر (*enq_flags & SCX_ENQ_PREEMPT)
		needed |= scx_caps_for_preempt(asch, rq, *enq_flags);
	missing = scx_missing_caps(asch, cpu_کا(rq), needed);

	/* requirements met */
	اگر (likely(!missing))
		واپس &rq->scx.local_dsq;

	/*
	 * The task must run on this CPU regardless of caps: the rq is draining
	 * offline (BPF scheduler bypassed), the task is migration-disabled, or a
	 * migration is pending. Admit despite the missing caps and count it.
	 * Refuse preemptions.
	 */
	اگر (unlikely(!scx_rq_online(rq) || is_migration_disabled(p) ||
		     p->migration_pending)) {
		__scx_add_event(sch, SCX_EV_SUB_FORCED_ADMIT, 1);
		*enq_flags &= ~SCX_ENQ_PREEMPT;
		واپس &rq->scx.local_dsq;
	}

	/*
	 * Diverting to rescue or reject, neither of which honors IMMED, PREEMPT
	 * or HEAD - a diversion has no priority and IMMED is not allowed on
	 * non-local DSQs. Strip the enq and task flags along with the slice.
	 */
	*enq_flags &= ~(SCX_ENQ_IMMED | SCX_ENQ_PREEMPT | SCX_ENQ_سر |
			SCX_ENQ_APPLY_قاش | SCX_ENQ_قاش_DFL);
	p->scx.flags &= ~SCX_TASK_IMMED;

	/* the enqueuer opted for rescue instead of rejection and reenqueue */
	اگر ((*enq_flags & SCX_ENQ_RESCUE) && likely(scx_rescue_bw_1024)) {
		__scx_add_event(sch, SCX_EV_SUB_RESCUE, 1);
		اگر (scx_rescue_try_admit(rq, p))
			واپس &rq->scx.local_dsq;

		/* queueing, the overload trigger measures the wait from here */
		p->scx.rescue_at = jiffies;
		واپس &rq->scx.rescue.dsq;
	}

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = cid;

	واپس &rq->scx.reject_dsq;
}

/* @p lost the caps needed to stay on @rq's local DSQ? Record reason if so. */
bool scx_task_reenq_on_cap_revoke(ساخت rq *rq, ساخت task_struct *p)
{
	u64 missing;

	/* migration-disabled tasks and the rescuee are admitted capless */
	اگر (is_migration_disabled(p) || p == scx_rescuee(rq))
		واپس false;

	missing = scx_missing_caps(scx_task_sched(p), cpu_کا(rq), scx_caps_for_task(p));
	اگر (likely(!missing))
		واپس false;

	p->scx.reenq_reason_caps = missing;
	p->scx.reenq_reason_cid = __scx_cpu_to_cid(cpu_کا(rq));
	واپس true;
}

/*
 * Drain @rq->scx.reject_dsq, reenqueueing each task so the BPF re-decides
 * from p->scx.reenq_reason_*.
 *
 * A task can be re-rejected repeatedly. The reenqueue is bounded per task in
 * scx_do_enqueue_task(), which ejects the owning sub past SCX_REENQ_MAX_REPEAT.
 * Rejection can't happen for root.
 */
خالی scx_reenq_reject(ساخت rq *rq)
{
	LIST_سر(tasks);
	ساخت task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	اگر (!scx_has_subs() || list_خالی_خانے(&rq->scx.reject_dsq.list))
		واپس;

	/*
	 * Move to a private list so a task re-rejected by the
	 * scx_do_enqueue_task() below isn't revisited this round.
	 */
	list_for_each_اندراج_محفوظ(p, n, &rq->scx.reject_dsq.list, scx.dsq_list.گرہ) {
		/* migration_pending tasks should have bypassed to local DSQ */
		اگر (WARN_ON_ONCE(p->migration_pending))
			جاری;

		scx_wait_dispatching(p);
		scx_dispatch_قطار_سے_نکالو(rq, p);

		اگر (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_نقاب))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_نقاب;
		p->scx.flags |= SCX_TASK_REENQ_CAP;

		list_جوڑو_دم(&p->scx.dsq_list.گرہ, &tasks);
	}

	list_for_each_اندراج_محفوظ(p, n, &tasks, scx.dsq_list.گرہ) {
		list_del_init(&p->scx.dsq_list.گرہ);

		scx_do_قطار_میں_ڈالو_task(rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_نقاب;
	}
}

/* record a caps change, see struct scx_caps_updated */
ساکن خالی caps_updated_ریکارڈ(ساخت scx_pshard *ps, ثابت ساخت scx_cmask *cids, u64 caps,
				ساخت list_سر *مقصد_deliver)
{
	ساخت scx_caps_updated *cu = &ps->caps_updated;

	guard(raw_spinlock)(&cu->تالا);
	scx_cmask_or(&cu->cmask, cids);
	cu->caps |= caps;
	اگر (list_خالی_خانے(&cu->گرہ_اندر_flight))
		list_جوڑو_دم(&cu->گرہ_اندر_flight, مقصد_deliver);
}

/* deliver queued caps_updated callbacks, see struct scx_caps_updated */
ساکن خالی caps_updated_deliver(ساخت list_سر *مقصد_deliver)
{
	ساخت scx_caps_updated *cu, *tmp;

	list_for_each_اندراج_محفوظ(cu, tmp, مقصد_deliver, گرہ_اندر_flight) {
		ساخت scx_pshard *ps = container_کا(cu, ساخت scx_pshard, caps_updated);
		ساخت scx_sched *sch = ps->sch;

		جب (true) {
			u64 caps = 0;

			/*
			 * During enable, has_op is set after ops.sub_attach(),
			 * so !has_op means the op is absent or the sched isn't
			 * live yet - e.g. caps grant from ops.sub_attach().
			 * Either way don't consume - leave for
			 * scx_sub_seed_caps() to deliver once live.
			 */
			scoped_guard (raw_spinlock, &cu->تالا) {
				اگر (cu->caps && SCX_HAS_OP(sch, sub_caps_updated) &&
				    likely(!READ_ONCE(sch->aborting))) {
					ساخت scx_cmask_ref ref;

					caps = cu->caps;
					scx_cmask_ref_init_kern(sch, cu->cmask_arena_برآمد,
								ps->بنیاد, ps->nr_cids, &ref);
					scx_cmask_ref_copy(&ref, &cu->cmask);
					scx_cmask_clear(&cu->cmask);
					cu->caps = 0;
				} ورنہ {
					list_del_init(&cu->گرہ_اندر_flight);
				}
			}
			اگر (!caps)
				توڑو;

			/* caps != 0 only when deliverable (has_op, above) */
			SCX_CALL_OP(sch, sub_caps_updated, تہی, cu->cmask_arena_برآمد, caps);
		}
	}
}

/*
 * Deliver caps owed to @sch that couldn't be delivered earlier (e.g. a grant
 * taken during its sub_attach(), before has_op was set). Called once @sch is
 * enabled.
 */
ساکن خالی scx_sub_بیج_caps(ساخت scx_sched *sch)
{
	LIST_سر(مقصد_deliver);
	s32 si;

	guard(irqsave)();

	برائے (si = 0; si < sch->nr_pshards; si++) {
		ساخت scx_pshard *ps = sch->pshard[si];
		ساخت scx_caps_updated *cu = &ps->caps_updated;

		scoped_guard (raw_spinlock, &cu->تالا) {
			اگر (cu->caps && list_خالی_خانے(&cu->گرہ_اندر_flight))
				list_جوڑو_دم(&cu->گرہ_اندر_flight, &مقصد_deliver);
		}
	}
	caps_updated_deliver(&مقصد_deliver);
}

ساکن u64 calc_effective_caps(ساخت scx_pshard *ps, s32 cid)
{
	u64 ecaps = 0;
	u32 cap_ثنائی_ہندسہ;

	برائے (cap_ثنائی_ہندسہ = 0; cap_ثنائی_ہندسہ < __SCX_NR_CAPS; cap_ثنائی_ہندسہ++)
		اگر (scx_cmask_test(cid, &ps->caps[cap_ثنائی_ہندسہ].cmask))
			ecaps |= ثنائی_ہندسہ_U64(cap_ثنائی_ہندسہ) | scx_caps_implied(ثنائی_ہندسہ_U64(cap_ثنائی_ہندسہ));
	واپس ecaps;
}

/**
 * queue_sync_ecaps - Queue ecaps update for a (sch, cid) pair
 * @sch: sched to update
 * @cid: cid to update
 *
 * Queue an ecaps update for @sch's @cid and kick the cpu so that it syncs in
 * dispatch_one().
 */
ساکن خالی قطار_sync_ecaps(ساخت scx_sched *sch, s32 cid)
{
	s32 cpu = __scx_cid_to_cpu(cid);
	ساخت scx_sched_pcpu *pcpu = فی_cpu_ptr(sch->pcpu, cpu);

	/*
	 * Pairs with smp_mb() in scx_process_sync_ecaps(). Either the check
	 * below sees the node off the list and queues it, or the in-flight sync
	 * sees the caps[] update made before this call.
	 */
	smp_mb();

	/* @cid's pshard->lock excludes concurrent queueing attempts */
	اگر (llist_on_list(&pcpu->ecaps_مقصد_sync_گرہ))
		واپس;
	اگر (llist_جوڑو(&pcpu->ecaps_مقصد_sync_گرہ, &cpu_rq(cpu)->scx.ecaps_مقصد_sync))
		scx_kick_cpu(sch->ancestors[0], cpu, 0);
}

/* discard @rq's queued ecaps syncs */
ساکن خالی discard_queued_syncs(ساخت rq *rq)
{
	ساخت llist_گرہ *pos, *tmp;

	lockdep_assert_rq_held(rq);

	llist_for_each_محفوظ(pos, tmp, llist_del_all(&rq->scx.ecaps_مقصد_sync))
		init_llist_گرہ(pos);
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
خالی scx_process_sync_ecaps(ساخت rq *rq, ساخت task_struct *پچھلا)
{
	s32 cpu = cpu_کا(rq);
	s32 cid, shard;
	ساخت llist_گرہ *batch, *pos, *tmp;
	u64 گمشدہ_all = 0;

	lockdep_assert_rq_held(rq);

	اگر (!scx_has_subs() || likely(llist_خالی_خانے(&rq->scx.ecaps_مقصد_sync)))
		واپس;

	/*
	 * ecaps are zeroed while the cpu is inactive and must stay zero.
	 * Discard queued syncs instead of processing them - the
	 * scx_online_ecaps() reseed re-syncs every sched on activation.
	 * cpu_active() clears before the offline zeroing and sets before the
	 * reseed is queued, so this test can neither miss a racing sync nor
	 * eat the reseed.
	 */
	اگر (unlikely(!cpu_فعال(cpu))) {
		discard_queued_syncs(rq);
		واپس;
	}

	/* @cid is valid here: the cpu is active with queued syncs */
	cid = __scx_cpu_to_cid(cpu);
	shard = rcu_dereference_all(scx_cid_مقصد_shard)[cid];

	batch = llist_del_all(&rq->scx.ecaps_مقصد_sync);
	llist_for_each_محفوظ(pos, tmp, batch) {
		ساخت scx_sched_pcpu *pcpu =
			container_کا(pos, ساخت scx_sched_pcpu, ecaps_مقصد_sync_گرہ);
		ساخت scx_pshard *ps = pcpu->sch->pshard[shard];
		u64 old, ecaps, گمشدہ, gained;

		init_llist_گرہ(pos);

		/* pairs with smp_mb() in queue_sync_ecaps(), see there */
		smp_mb();

		old = READ_ONCE(pcpu->ecaps);
		ecaps = calc_effective_caps(ps, cid);
		WRITE_ONCE(pcpu->ecaps, ecaps);

		گمشدہ = old & ~ecaps;
		gained = ecaps & ~old;
		گمشدہ_all |= گمشدہ;

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
		اگر (ecaps != pcpu->reported_ecaps &&
		    SCX_HAS_OP(pcpu->sch, sub_ecaps_updated) &&
		    !scx_bypassing(pcpu->sch, cpu)) {
			ساخت scx_dsp_ctx *dspc = &this_cpu_ptr(pcpu->sch->pcpu)->dsp_ctx;

			dspc->rq = rq;
			/* stash @prev so nested dispatches can access it */
			rq->scx.sub_dispatch_پچھلا = پچھلا;
			SCX_CALL_OP(pcpu->sch, sub_ecaps_updated, rq, scx_cpu_arg(cpu),
				    pcpu->reported_ecaps, ecaps);
			rq->scx.sub_dispatch_پچھلا = تہی;
			scx_flush_dispatch_buf(pcpu->sch, rq);
			pcpu->reported_ecaps = ecaps;
		}

		/*
		 * Gaining baseline access owes an update_idle() so the sched
		 * learns the cpu's idle state. Arm the per-rq gate so the next
		 * idle pick flushes it. Losing access drops any pending notify.
		 */
		اگر (gained & SCX_CAP_بنیاد) {
			pcpu->idle_renotify = true;
			rq->scx.flags |= SCX_RQ_SUB_IDLE_RENOTIFY;
		} ورنہ اگر (گمشدہ & SCX_CAP_بنیاد) {
			pcpu->idle_renotify = false;
		}
	}

	/*
	 * Losing a cap can strand already-queued tasks. Schedule a reenq scan
	 * to move the now-capless ones off the local DSQ. The scan tests
	 * against the effective caps and thus must come after the ecaps sync.
	 */
	اگر (گمشدہ_all & SCX_CAPS_REENQ_ON_LOSS)
		scx_جدول_وقت_reenq_local(rq, SCX_REENQ_CAP_REVOKE);
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
خالی scx_unbypass_replay_ecaps(ساخت rq *rq, ساخت scx_sched *sch)
{
	s32 cpu = cpu_کا(rq);
	ساخت scx_sched_pcpu *pcpu = فی_cpu_ptr(sch->pcpu, cpu);
	ساخت scx_pshard *ps;
	s32 cid;

	lockdep_assert_rq_held(rq);

	/* root holds every cap and never uses ecaps */
	اگر (!sch->level)
		واپس;

	اگر (READ_ONCE(pcpu->ecaps) == pcpu->reported_ecaps)
		واپس;

	cid = __scx_cpu_to_cid(cpu);
	ps = sch->pshard[rcu_dereference_all(scx_cid_مقصد_shard)[cid]];

	guard(raw_spinlock)(&ps->تالا);
	قطار_sync_ecaps(sch, cid);
}

/*
 * A cpu came back. Re-seed each sub-sched's ecaps on the cpu's cid. The sync
 * recomputes effective caps from the pshard and fires ops.sub_ecaps_updated()
 * only on a real change since offline.
 */
خالی scx_online_ecaps(ساخت rq *rq)
{
	ساخت scx_sched *جڑ, *pos;
	s32 cid, shard;

	/*
	 * Only a live hierarchy can have ecaps to reseed. This also keeps the
	 * table reads below away from an enable that failed before publishing
	 * the tables. A concurrent disable can't retire them, see
	 * handle_hotplug().
	 */
	اگر (!scx_enabled())
		واپس;

	guard(rq_تالا_irqsave)(rq);

	جڑ = scx_جڑ_protected();
	cid = __scx_cpu_to_cid(cpu_کا(rq));
	shard = rcu_dereference_all(scx_cid_مقصد_shard)[cid];

	scx_for_each_descendant_pre(pos, جڑ) {
		ساخت scx_pshard *ps;

		/* root holds every cap and never uses ecaps */
		اگر (!pos->level)
			جاری;

		ps = pos->pshard[shard];
		guard(raw_spinlock)(&ps->تالا);
		قطار_sync_ecaps(pos, cid);
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
خالی scx_offline_ecaps(ساخت rq *rq)
{
	s32 cpu = cpu_کا(rq);
	ساخت scx_sched *جڑ, *pos;

	guard(rq_تالا_irqsave)(rq);

	جڑ = scx_جڑ_protected();

	scx_for_each_descendant_pre(pos, جڑ) {
		/* root holds every cap and never uses ecaps */
		اگر (!pos->level)
			جاری;

		WRITE_ONCE(فی_cpu_ptr(pos->pcpu, cpu)->ecaps, 0);
	}
}

/*
 * @pcpu's sched was unhashed before the grace period, so nothing re-queues its
 * sync node. Remove the node from @rq's pending list so the pcpu can be freed.
 */
خالی scx_discard_ecaps_مقصد_sync(s32 cpu, ساخت scx_sched_pcpu *pcpu)
{
	ساخت rq *rq = cpu_rq(cpu);
	ساخت llist_گرہ *سر = تہی, *دم = تہی;
	ساخت llist_گرہ *pos, *tmp;

	/*
	 * llist can't unlink a single node. Take all queued nodes, drop @pcpu's
	 * and resplice the rest. Nodes in the taken batch read as on-list
	 * throughout, so queue_sync_ecaps() stays correct.
	 */
	اگر (llist_on_list(&pcpu->ecaps_مقصد_sync_گرہ)) {
		scoped_guard (rq_تالا_irqsave, rq) {
			llist_for_each_محفوظ(pos, tmp, llist_del_all(&rq->scx.ecaps_مقصد_sync)) {
				اگر (pos == &pcpu->ecaps_مقصد_sync_گرہ) {
					init_llist_گرہ(pos);
				} ورنہ {
					pos->اگلا = سر;
					سر = pos;
					اگر (!دم)
						دم = pos;
				}
			}
			اگر (سر)
				llist_جوڑو_batch(سر, دم, &rq->scx.ecaps_مقصد_sync);
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
	جب (true) {
		scoped_guard (rq_تالا_irqsave, rq) {
			اگر (!llist_on_list(&pcpu->ecaps_مقصد_sync_گرہ))
				واپس;
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
خالی scx_discard_stale_ecaps_syncs(خالی)
{
	s32 cpu;

	for_each_possible_cpu(cpu) {
		ساخت rq *rq = cpu_rq(cpu);

		guard(rq_تالا_irqsave)(rq);
		discard_queued_syncs(rq);
	}
}

ساکن DECLARE_WAIT_قطار_سر(scx_unlink_waitq);

خالی drain_descendants(ساخت scx_sched *sch)
{
	/*
	 * Child scheds that finished the critical part of disabling will take
	 * themselves off @sch->children. Wait for it to drain. As propagation
	 * is recursive, empty @sch->children means that all proper descendant
	 * scheds reached unlinking stage.
	 */
	wait_event(scx_unlink_waitq, list_خالی_خانے(&sch->children));
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
ساکن خالی scx_rehome_task(ساخت scx_sched *مقصد, ساخت task_struct *p)
{
	lockdep_assert_held(&p->pi_تالا);
	lockdep_assert_rq_held(task_rq(p));

	scoped_guard (sched_change, p, قطار_سے_نکالو_SAVE | قطار_سے_نکالو_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_سیٹ_task_state(p, SCX_TASK_INIT_BEGIN);
		scx_سیٹ_task_state(p, SCX_TASK_INIT);
		scx_سیٹ_task_sched(p, مقصد);
		scx_سیٹ_task_state(p, SCX_TASK_READY);
		اگر (p->sched_class == &ext_sched_class)
			scx_enable_task(مقصد, p);
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
ساکن خالی scx_punt_task(ساخت scx_sched *مقصد, ساخت task_struct *p)
{
	lockdep_assert_held(&p->pi_تالا);
	lockdep_assert_rq_held(task_rq(p));
	WARN_ON_ONCE(!READ_ONCE(مقصد->bypass_گہرائی));

	scoped_guard (sched_change, p, قطار_سے_نکالو_SAVE | قطار_سے_نکالو_MOVE) {
		scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_سیٹ_task_sched(p, مقصد);
	}
}

ساکن خالی scx_fail_والد(ساخت scx_sched *sch,
			    ساخت task_struct *failed, s32 fail_رمز)
{
	ساخت scx_sched *والد = scx_والد(sch);
	ساخت scx_task_iter sti;
	ساخت task_struct *p;

	scx_error(والد, "ops.init_task() failed (%d) for %s[%d] while disabling a sub-scheduler",
		  fail_رمز, failed->comm, failed->pid);

	/*
	 * Once $parent is bypassed, tasks can be punted into it. This may
	 * cause downstream failures on the BPF side but $parent is dying
	 * anyway.
	 */
	scx_bypass(والد, true);

	scx_task_iter_آغاز(&sti, sch->cgrp);
	جب ((p = scx_task_iter_اگلا_locked(&sti))) {
		اگر (scx_task_on_sched(والد, p))
			جاری;

		scx_punt_task(والد, p);
	}
	scx_task_iter_stop(&sti);
}

#اگر_تعریف_شدہ CONFIG_EXT_گروہ_SCHED
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
ساکن s32 scx_cgroup_claim_subtree(ساخت scx_sched *sch)
{
	ساخت cgroup *sub_cgrp = sch_cgroup(sch);
	ساخت cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	ساخت scx_sched *والد = scx_والد(sch);
	ساخت cgroup_subsys_state *css;
	صحیح ret;

	css_for_each_descendant_pre(css, ecss) {
		ساخت task_گروہ *tg = css_tg(css);
		ساخت scx_cgroup_init_دلائل دلائل = {
			.وزن = tg->scx.وزن,
			.bw_period_us = tg->scx.bw_period_us,
			.bw_quota_us = tg->scx.bw_quota_us,
			.bw_مدت_us = tg->scx.bw_مدت_us,
			.sched_idle = tg->scx.idle,
		};

		اگر (tg->scx.sched != والد ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			جاری;

		اگر (SCX_HAS_OP(sch, cgroup_init)) {
			ret = SCX_CALL_OP_RET(sch, cgroup_init, تہی, css->cgroup, &دلائل);
			اگر (ret) {
				scx_error(sch, "ops.cgroup_init() failed (%d)", ret);
				جاؤ err;
			}
		}
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_post(css, ecss) {
		ساخت task_گروہ *tg = css_tg(css);

		/*
		 * SUB_INIT is pass 1's progress mark: pass 2 and the err path
		 * must visit exactly the tgs pass 1 inited.
		 */
		اگر (!(tg->scx.flags & SCX_TG_SUB_INIT))
			جاری;

		/* skip the exit if the parent's ops.cgroup_init() failed */
		اگر ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(والد, cgroup_exit))
			SCX_CALL_OP(والد, cgroup_exit, تہی, css->cgroup);
		tg->scx.sched = sch;
		tg->scx.flags |= SCX_TG_INITED;
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}

	واپس 0;

err:
	css_for_each_descendant_post(css, ecss) {
		ساخت task_گروہ *tg = css_tg(css);

		اگر (!(tg->scx.flags & SCX_TG_SUB_INIT))
			جاری;

		اگر (SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, تہی, css->cgroup);
		tg->scx.flags &= ~SCX_TG_SUB_INIT;
	}
	واپس ret;
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
ساکن خالی scx_cgroup_return_subtree(ساخت scx_sched *sch)
{
	ساخت cgroup *sub_cgrp = sch_cgroup(sch);
	ساخت cgroup_subsys_state *ecss = cgroup_e_css(sub_cgrp, &cpu_cgrp_subsys);
	ساخت scx_sched *والد = scx_والد(sch);
	ساخت cgroup_subsys_state *css;
	bool والد_failed = false;
	صحیح ret;

	css_for_each_descendant_post(css, ecss) {
		ساخت task_گروہ *tg = css_tg(css);

		اگر (tg->scx.sched != sch ||
		    !cgroup_is_descendant(css->cgroup, sub_cgrp))
			جاری;

		/* skip the exit if @sch's ops.cgroup_init() failed for the tg */
		اگر ((tg->scx.flags & SCX_TG_INITED) && SCX_HAS_OP(sch, cgroup_exit))
			SCX_CALL_OP(sch, cgroup_exit, تہی, css->cgroup);
		tg->scx.sched = والد;
		tg->scx.flags |= SCX_TG_SUB_INIT;
	}

	css_for_each_descendant_pre(css, ecss) {
		ساخت task_گروہ *tg = css_tg(css);
		ساخت scx_cgroup_init_دلائل دلائل = {
			.وزن = tg->scx.وزن,
			.bw_period_us = tg->scx.bw_period_us,
			.bw_quota_us = tg->scx.bw_quota_us,
			.bw_مدت_us = tg->scx.bw_مدت_us,
			.sched_idle = tg->scx.idle,
		};

		/* the first pass must have transferred everything */
		WARN_ON_ONCE(tg->scx.sched == sch);

		/*
		 * SUB_INIT distinguishes the tgs pass 1 moved. The sched test
		 * can't: a tg punted to the parent by an earlier failure would
		 * also match.
		 */
		اگر (!(tg->scx.flags & SCX_TG_SUB_INIT))
			جاری;
		tg->scx.flags &= ~(SCX_TG_SUB_INIT | SCX_TG_INITED);

		/*
		 * A re-init on $parent failed. The task_groups from here on are
		 * punted: they stay on the dying $parent with INITED clear and
		 * move onward when it disables.
		 */
		اگر (والد_failed)
			جاری;

		اگر (SCX_HAS_OP(والد, cgroup_init)) {
			ret = SCX_CALL_OP_RET(والد, cgroup_init, تہی, css->cgroup, &دلائل);
			اگر (ret) {
				scx_error(والد, "ops.cgroup_init() failed (%d) while disabling a sub-scheduler",
					  ret);
				والد_failed = true;
				جاری;
			}
		}
		tg->scx.flags |= SCX_TG_INITED;
	}
}
#ورنہ
ساکن اندرونی s32 scx_cgroup_claim_subtree(ساخت scx_sched *sch) { واپس 0; }
ساکن اندرونی خالی scx_cgroup_return_subtree(ساخت scx_sched *sch) {}
#اگر_ختم

خالی scx_sub_disable(ساخت scx_sched *sch)
{
	ساخت scx_sched *والد = scx_والد(sch);
	ساخت scx_task_iter sti;
	ساخت task_struct *p;
	صحیح ret;

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
	mutex_تالا(&scx_enable_mutex);
	percpu_نیچے_کو_write(&scx_fork_rwsem);
	scx_cgroup_تالا();

	/*
	 * An enable that failed before scx_link_sched() succeeded never owned a
	 * cgroup or task and won't be waited on by an ancestor's
	 * drain_descendants(). Nothing to reparent and walking the tasks can
	 * misbehave as the task ownership invariant (either owned by self or
	 * parent) does not hold. ->sibling can't identify this case - an undone
	 * link leaves it non-empty.
	 */
	اگر (!sch->linked)
		جاؤ dump;

	سیٹ_cgroup_sched(sch_cgroup(sch), والد);

	/*
	 * Return the subtree's cgroups before re-homing tasks so that any
	 * ops.init_task() on $parent only sees cgroups it has initialized.
	 */
	scx_cgroup_return_subtree(sch);

	scx_task_iter_آغاز(&sti, sch->cgrp);
	جب ((p = scx_task_iter_اگلا_locked(&sti))) {
		ساخت rq *rq;
		ساخت rq_flags rf;

		/* filter out duplicate visits */
		اگر (scx_task_on_sched(والد, p))
			جاری;

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
		ret = __scx_init_task(والد, p, تہی, false);
		اگر (ret) {
			scx_fail_والد(sch, p, ret);
			put_task_struct(p);
			توڑو;
		}

		rq = task_rq_تالا_2(p, &rf);

		اگر (scx_get_task_state(p) == SCX_TASK_DEAD) {
			/*
			 * sched_ext_dead() raced us between __scx_init_task()
			 * and this rq lock and ran exit_task() on @sch (the
			 * sched @p was on at that point), not on $parent.
			 * $parent's just-completed init is owed an exit_task()
			 * and we issue it here.
			 */
			scx_sub_init_cancel_task(والد, p);
			task_rq_unlock(rq, p, &rf);
			put_task_struct(p);
			جاری;
		}

		scx_rehome_task(والد, p);

		task_rq_unlock(rq, p, &rf);
		put_task_struct(p);
	}
	scx_task_iter_stop(&sti);

dump:
	scx_disable_dump(sch);

	scx_cgroup_unlock();
	percpu_اوپر_write(&scx_fork_rwsem);

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
	wake_اوپر_all(&scx_unlink_waitq);

	اگر (والد->ops.sub_detach && sch->sub_attached) {
		ساخت scx_sub_detach_دلائل sub_detach_دلائل = {
			.ops = &sch->ops,
			.cgroup_راستہ = sch->cgrp_راستہ,
		};
		SCX_CALL_OP(والد, sub_detach, تہی,
			    &sub_detach_دلائل);
	}

	scx_log_sched_disable(sch);

	اگر (sch->ops.باہر_نکلو)
		SCX_CALL_OP(sch, باہر_نکلو, تہی, sch->exit_info);

	/*
	 * @sch's non-ops programs such as timers and tracers can fire after
	 * ops.exit(). Now that exit is complete, stop scx_prog_sched() from
	 * resolving to @sch and drain in-flight resolvers.
	 */
	WRITE_ONCE(sch->dead, true);
	synchronize_rcu();

	اگر (sch->sub_kset)
		kobject_del(&sch->sub_kset->kobj);
	/* not added if enable failed before scx_sched_sysfs_add() */
	اگر (sch->kobj.state_اندر_sysfs)
		kobject_del(&sch->kobj);
}

/* verify that a scheduler can be attached to @cgrp and return the parent */
ساکن ساخت scx_sched *ڈھونڈو_والد_sched(ساخت cgroup *cgrp)
{
	ساخت scx_sched *والد = scx_cgroup_sched(cgrp);
	ساخت scx_sched *pos;

	lockdep_assert_held(&scx_sched_تالا);

	/* can't attach twice to the same cgroup */
	اگر (والد->cgrp == cgrp)
		واپس ERR_PTR(-EBUSY);

	/* does $parent allow sub-scheds? */
	اگر (!والد->ops.sub_attach)
		واپس ERR_PTR(-EOPNOTSUPP);

	/* can't insert between $parent and its exiting children */
	list_for_each_اندراج(pos, &والد->children, sibling)
		اگر (cgroup_is_descendant(pos->cgrp, cgrp))
			واپس ERR_PTR(-EBUSY);

	واپس والد;
}

ساکن bool assert_task_ready_or_enabled(ساخت task_struct *p)
{
	u32 state = scx_get_task_state(p);

	انتخاب (state) {
	حالت SCX_TASK_READY:
	حالت SCX_TASK_ENABLED:
		واپس true;
	طےشدہ:
		WARN_ONCE(true, "sched_ext: Invalid task state %d for %s[%d] during enabling sub sched",
			  state, p->comm, p->pid);
		واپس false;
	}
}

خالی scx_sub_enable_workfn(ساخت kthread_work *work)
{
	ساخت scx_enable_cmd *cmd = container_کا(work, ساخت scx_enable_cmd, work);
	ساخت sched_ext_ops *ops = cmd->ops;
	ساخت cgroup *cgrp;
	ساخت scx_sched *والد, *sch;
	ساخت scx_task_iter sti;
	ساخت task_struct *p;
	s32 i, ret;

	mutex_تالا(&scx_enable_mutex);

	اگر (!scx_enabled()) {
		ret = -ENODEV;
		جاؤ برآمد_unlock;
	}

	/* See scx_root_enable_workfn() for the @ops->priv check. */
	اگر (rcu_access_اشارہ_گر(ops->priv)) {
		ret = -EBUSY;
		جاؤ برآمد_unlock;
	}

	cgrp = cgroup_get_from_id(ops->sub_cgroup_id);
	اگر (IS_ERR(cgrp)) {
		ret = PTR_ERR(cgrp);
		جاؤ برآمد_unlock;
	}

	raw_spin_تالا_irq(&scx_sched_تالا);
	والد = ڈھونڈو_والد_sched(cgrp);
	اگر (IS_ERR(والد)) {
		raw_spin_unlock_irq(&scx_sched_تالا);
		ret = PTR_ERR(والد);
		جاؤ برآمد_put_cgrp;
	}
	kobject_get(&والد->kobj);
	raw_spin_unlock_irq(&scx_sched_تالا);

	/*
	 * Flip the hot-path gates before ops->priv is published - the sub's
	 * programs can e.g. kick cpus from that point on. The matching dec is
	 * at the end of scx_sched_free_rcu_work().
	 */
	static_branch_inc(&__scx_has_subs);

	/* scx_alloc_and_add_sched() consumes @cgrp whether it succeeds or not */
	sch = scx_alloc_and_جوڑو_sched(cmd, cgrp, والد);
	kobject_put(&والد->kobj);
	اگر (IS_ERR(sch)) {
		static_branch_dec(&__scx_has_subs);
		ret = PTR_ERR(sch);
		جاؤ برآمد_unlock;
	}

	/*
	 * Validate before scx_link_sched() publishes @sch, so an invalid sub
	 * never becomes visible with an unallocated pshard.
	 */
	ret = scx_validate_ops(sch, ops);
	اگر (ret)
		جاؤ err_disable;

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
	اگر (ret)
		جاؤ err_disable;

	ret = scx_alloc_pshards(sch);
	اگر (ret)
		جاؤ err_disable;

	ret = scx_link_sched(sch);
	اگر (ret)
		جاؤ err_disable;

	ret = scx_sched_sysfs_جوڑو(sch);
	اگر (ret)
		جاؤ err_disable;

	اگر (sch->level >= SCX_SUB_MAX_گہرائی) {
		scx_error(sch, "max nesting depth %d violated",
			  SCX_SUB_MAX_گہرائی);
		ret = -EINVAL;
		جاؤ err_disable;
	}

	scoped_guard(cpus_read_تالا) {
		ret = scx_alloc_kern_arena_objs(sch);
		اگر (ret)
			جاؤ err_disable;
	}

	اگر (sch->ops.init) {
		ret = SCX_CALL_OP_RET(sch, init, تہی);
		اگر (ret) {
			ret = scx_ops_sanitize_err(sch, "init", ret);
			scx_error(sch, "ops.init() failed (%d)", ret);
			جاؤ err_disable;
		}
		sch->exit_info->flags |= SCX_EFLAG_INITIALIZED;
	}

	ساخت scx_sub_attach_دلائل sub_attach_دلائل = {
		.ops = &sch->ops,
		.cgroup_راستہ = sch->cgrp_راستہ,
	};

	ret = SCX_CALL_OP_RET(والد, sub_attach, تہی,
			      &sub_attach_دلائل);
	اگر (ret) {
		ret = scx_ops_sanitize_err(sch, "sub_attach", ret);
		scx_error(sch, "parent rejected (%d)", ret);
		جاؤ err_disable;
	}
	sch->sub_attached = true;

	scx_bypass(sch, true);

	برائے (i = SCX_OPI_BEGIN; i < SCX_OPI_انتہا; i++)
		اگر (((خالی (**)(خالی))ops)[i])
			سیٹ_ثنائی_ہندسہ(i, sch->has_op);

	percpu_نیچے_کو_write(&scx_fork_rwsem);
	scx_cgroup_تالا();

	/*
	 * Set cgroup->scx_sched's and check CSS_ONLINE. Either we see
	 * !CSS_ONLINE or scx_cgroup_lifetime_notify() sees and shoots us down.
	 */
	سیٹ_cgroup_sched(sch_cgroup(sch), sch);
	اگر (!(cgrp->self.flags & CSS_ONLINE)) {
		scx_error(sch, "cgroup is not online");
		ret = -ENODEV;
		جاؤ err_unlock_and_disable;
	}

	/*
	 * Take over the subtree's cgroups before any task is claimed,
	 * mirroring root enable's cgroups-before-tasks order.
	 */
	ret = scx_cgroup_claim_subtree(sch);
	اگر (ret)
		جاؤ err_unlock_and_disable;

	/*
	 * Initialize tasks for the new child $sch without exiting them for
	 * $parent so that the tasks can always be reverted back to $parent
	 * sched on child init failure.
	 */
	WARN_ON_ONCE(scx_enabling_sub_sched);
	scx_enabling_sub_sched = sch;

	scx_task_iter_آغاز(&sti, sch->cgrp);
	جب ((p = scx_task_iter_اگلا_locked(&sti))) {
		ساخت rq *rq;
		ساخت rq_flags rf;

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
		اگر (p->scx.flags & SCX_TASK_SUB_INIT)
			جاری;

		/* @p is pinned by the iter; see scx_sub_disable() */
		get_task_struct(p);

		اگر (!assert_task_ready_or_enabled(p)) {
			ret = -EINVAL;
			جاؤ abort;
		}

		scx_task_iter_unlock(&sti);

		/*
		 * As $p is still on $parent, it can't be transitioned to INIT.
		 * Let's worry about task state later. Use __scx_init_task().
		 */
		ret = __scx_init_task(sch, p, تہی, false);
		اگر (ret)
			جاؤ abort;

		rq = task_rq_تالا_2(p, &rf);

		اگر (scx_get_task_state(p) == SCX_TASK_DEAD) {
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
			جاری;
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
	scx_task_iter_آغاز(&sti, sch->cgrp);
	جب ((p = scx_task_iter_اگلا_locked(&sti))) {
		/*
		 * Use clearing of %SCX_TASK_SUB_INIT to detect and skip
		 * duplicate iterations.
		 */
		اگر (!(p->scx.flags & SCX_TASK_SUB_INIT))
			جاری;

		scoped_guard (sched_change, p, قطار_سے_نکالو_SAVE | قطار_سے_نکالو_MOVE) {
			/*
			 * $p must be either READY or ENABLED. If ENABLED,
			 * __scx_disabled_and_exit_task() first disables and
			 * makes it READY. However, after exiting $p, it will
			 * leave $p as READY.
			 */
			assert_task_ready_or_enabled(p);
			__scx_disable_and_exit_task(والد, p);

			/*
			 * $p is now only initialized for @sch and READY, which
			 * is what we want. Assign it to @sch and, if it's on
			 * the ext class, enable. A non-ext task, possible under
			 * an %SCX_OPS_SWITCH_PARTIAL root, stays READY and is
			 * enabled by switching_to_scx() if it switches over.
			 */
			scx_سیٹ_task_sched(p, sch);
			اگر (p->sched_class == &ext_sched_class)
				scx_enable_task(sch, p);

			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);

	scx_enabling_sub_sched = تہی;

	scx_cgroup_unlock();
	percpu_اوپر_write(&scx_fork_rwsem);

	scx_bypass(sch, false);

	/* @sch is enabled; deliver any caps owed since its sub_attach() */
	scx_sub_بیج_caps(sch);

	pr_info("sched_ext: BPF sub-scheduler \"%s\" enabled\n", sch->ops.name);
	kobject_uevent(&sch->kobj, KOBJ_جوڑو);
	ret = 0;
	جاؤ برآمد_unlock;

برآمد_put_cgrp:
	cgroup_put(cgrp);
برآمد_unlock:
	mutex_unlock(&scx_enable_mutex);
	cmd->ret = ret;
	واپس;

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
	scx_task_iter_آغاز(&sti, sch->cgrp);
	جب ((p = scx_task_iter_اگلا_locked(&sti))) {
		اگر (p->scx.flags & SCX_TASK_SUB_INIT) {
			scx_sub_init_cancel_task(sch, p);
			p->scx.flags &= ~SCX_TASK_SUB_INIT;
		}
	}
	scx_task_iter_stop(&sti);
	scx_enabling_sub_sched = تہی;
err_unlock_and_disable:
	/* we'll soon enter disable path, keep bypass on */
	scx_cgroup_unlock();
	percpu_اوپر_write(&scx_fork_rwsem);
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
ساکن s32 scx_cgroup_task_migrating(ساخت cgroup_task_migrate_ctx *ctx)
{
	ساخت task_struct *p = ctx->task;
	ساخت scx_sched *مقصد;
	صحیح ret;

	/*
	 * Cleared under scx_cgroup_lock() before root disable starts tearing
	 * down tasks. As cgroup_mutex is held, a set flag guarantees that the
	 * teardown loop is not running concurrently.
	 */
	اگر (!scx_cgroup_enabled)
		واپس NOTIFY_OK;

	مقصد = scx_cgroup_sched(ctx->dst_dcgrp);
	اگر (scx_task_on_sched(مقصد, p))
		واپس NOTIFY_OK;

	ret = __scx_init_task(مقصد, p, ctx->dst_dcgrp, false);
	اگر (ret)
		واپس notifier_from_errno(ret);

	واپس NOTIFY_OK;
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
ساکن خالی scx_cgroup_task_migrated(ساخت cgroup_task_migrate_ctx *ctx)
{
	ساخت task_struct *p = ctx->task;
	ساخت scx_sched *مقصد;
	ساخت rq *rq;
	ساخت rq_flags rf;

	اگر (!scx_cgroup_enabled)
		واپس;

	مقصد = scx_cgroup_sched(ctx->dst_dcgrp);
	اگر (scx_task_on_sched(مقصد, p))
		واپس;

	rq = task_rq_تالا_2(p, &rf);
	scx_rehome_task(مقصد, p);
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
ساکن خالی scx_cgroup_task_migrate_canceled(ساخت cgroup_task_migrate_ctx *ctx)
{
	ساخت task_struct *p = ctx->task;
	ساخت scx_sched *مقصد;
	ساخت rq *rq;
	ساخت rq_flags rf;

	اگر (!scx_cgroup_enabled)
		واپس;

	مقصد = scx_cgroup_sched(ctx->dst_dcgrp);
	اگر (scx_task_on_sched(مقصد, p))
		واپس;

	rq = task_rq_تالا_2(p, &rf);
	scx_sub_init_cancel_task(مقصد, p);
	task_rq_unlock(rq, p, &rf);
}

ساکن s32 scx_cgroup_lifetime_notify(ساخت notifier_بلاک *nb,
				      غیرعلامتی طویل action, خالی *ڈیٹا)
{
	ساخت cgroup *cgrp = ڈیٹا;
	ساخت cgroup *والد = cgroup_والد(cgrp);
	ساخت scx_sched *sch;

	اگر (!cgroup_on_dfl(cgrp))
		واپس NOTIFY_OK;

	انتخاب (action) {
	حالت CGROUP_LIFETIME_ONLINE:
		/* inherit ->scx_sched from $parent */
		اگر (والد)
			rcu_تفویض_اشارہ_گر(cgrp->scx_sched, scx_cgroup_sched(والد));
		توڑو;
	حالت CGROUP_LIFETIME_OFFLINE:
		/* if there is a sched attached, shoot it down */
		sch = scx_cgroup_sched(cgrp);
		اگر (sch && sch->cgrp == cgrp)
			scx_exit(sch, SCX_EXIT_UNREG_KERN,
				 SCX_ECODE_RSN_CGROUP_OFFLINE,
				 "cgroup %llu going offline", cgroup_id(cgrp));
		توڑو;
	}

	واپس NOTIFY_OK;
}

ساکن ساخت notifier_بلاک scx_cgroup_lifetime_nb = {
	.notifier_call = scx_cgroup_lifetime_notify,
};

ساکن s32 scx_cgroup_task_notify(ساخت notifier_بلاک *nb,
				  غیرعلامتی طویل action, خالی *ڈیٹا)
{
	ساخت cgroup_task_migrate_ctx *ctx = ڈیٹا;

	انتخاب (action) {
	حالت CGROUP_TASK_MIGRATING:
		واپس scx_cgroup_task_migrating(ctx);
	حالت CGROUP_TASK_MIGRATED:
		scx_cgroup_task_migrated(ctx);
		توڑو;
	حالت CGROUP_TASK_MIGRATE_CANCELED:
		scx_cgroup_task_migrate_canceled(ctx);
		توڑو;
	}

	واپس NOTIFY_OK;
}

ساکن ساخت notifier_بلاک scx_cgroup_task_nb = {
	.notifier_call = scx_cgroup_task_notify,
};

ساکن s32 __init scx_cgroup_notifier_init(خالی)
{
	s32 ret;

	ret = blocking_notifier_chain_رجسٹر(&cgroup_lifetime_notifier,
					       &scx_cgroup_lifetime_nb);
	اگر (ret)
		واپس ret;

	واپس blocking_notifier_chain_رجسٹر(&cgroup_task_notifier,
						&scx_cgroup_task_nb);
}
core_initcall(scx_cgroup_notifier_init);

ساکن خالی scx_pstack_recursion(ساخت bpf_prog *prog, ثابت حرف *op)
{
	ساخت scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(prog->aux);
	اگر (unlikely(!sch))
		واپس;

	scx_error(sch, "%s recursion detected", op);
}

خالی scx_pstack_recursion_on_dispatch(ساخت bpf_prog *prog)
{
	scx_pstack_recursion(prog, "dispatch");
}

خالی scx_pstack_recursion_on_caps_updated(ساخت bpf_prog *prog)
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
__bpf_kfunc bool scx_bpf_sub_dispatch(u64 cgroup_id, ثابت ساخت bpf_prog_aux *aux)
{
	ساخت rq *rq = scx_locked_rq();
	ساخت scx_sched *والد, *بچہ;

	guard(rcu)();
	والد = scx_prog_sched(aux);
	اگر (unlikely(!والد))
		واپس false;

	بچہ = scx_ڈھونڈو_sub_sched(cgroup_id);

	اگر (unlikely(!بچہ))
		واپس false;

	اگر (unlikely(scx_والد(بچہ) != والد)) {
		scx_error(والد, "trying to dispatch a distant sub-sched on cgroup %llu",
			  cgroup_id);
		واپس false;
	}

	/*
	 * Skip a child that does not effectively hold the base cap on this cpu:
	 * its inserts would only be rejected. ecaps are synced at the top of
	 * dispatch_one() before dispatch, so this reflects the in-effect state.
	 */
	اگر (scx_missing_caps(بچہ, cpu_کا(rq), SCX_CAP_بنیاد))
		واپس false;

	واپس scx_dispatch_sched(بچہ, rq, rq->scx.sub_dispatch_پچھلا, true) !=
		SCX_DSP_NONE;
}

/* Validate common inputs. On success, *parent_out and *child_out are set. */
ساکن s32 sub_cap_preamble(u64 cgroup_id, u64 caps, ثابت ساخت bpf_prog_aux *aux,
			    ساخت scx_sched **والد_برآمد, ساخت scx_sched **بچہ_برآمد)
{
	ساخت scx_sched *والد, *بچہ;

	والد = scx_prog_sched(aux);
	اگر (unlikely(!والد))
		واپس -ENODEV;

	اگر (!scx_is_cid_type()) {
		scx_error(والد, "sub-cap kfuncs require a cid-form scheduler");
		واپس -EOPNOTSUPP;
	}

	بچہ = scx_ڈھونڈو_sub_sched(cgroup_id);
	اگر (unlikely(!بچہ))
		واپس -ENODEV;

	اگر (unlikely(scx_والد(بچہ) != والد)) {
		scx_error(والد, "%s: sub-%llu is not a direct child",
			  والد->cgrp_راستہ, cgroup_id);
		واپس -EINVAL;
	}

	اگر (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(والد, "invalid caps 0x%llx", caps);
		واپس -EINVAL;
	}

	*والد_برآمد = والد;
	*بچہ_برآمد = بچہ;
	واپس 0;
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
				  ثابت ساخت scx_cmask *cmask__arena,
				  ساخت scx_cmask *denied_برآمد_arena_nullable,
				  ثابت ساخت bpf_prog_aux *aux)
{
	ساخت scx_cmask_ref ref, denied_ref;
	ساخت scx_sched *والد, *بچہ;
	bool any_denied = false;
	LIST_سر(مقصد_deliver);
	s32 si, ret;

	guard(irqsave)();

	ret = sub_cap_preamble(cgroup_id, caps, aux, &والد, &بچہ);
	اگر (ret)
		واپس ret;

	ret = scx_cmask_ref_init(والد, cmask__arena, &ref);
	اگر (ret) {
		scx_error(والد, "invalid cmask (%d)", ret);
		واپس ret;
	}

	اگر (denied_برآمد_arena_nullable) {
		ret = scx_cmask_ref_init(والد, denied_برآمد_arena_nullable, &denied_ref);
		اگر (ret) {
			scx_error(والد, "invalid denied_out (%d)", ret);
			واپس ret;
		}
	}

	/* apply the grant one shard at a time */
	برائے (si = ref.shard_پہلا; si < ref.shard_انتہا; si++) {
		SCX_CMASK_DEFINE_SHARD(قاش, 0, SCX_CID_SHARD_MAX_CPUS);
		ساخت scx_pshard *pps = والد->pshard[si];
		ساخت scx_pshard *cps = بچہ->pshard[si];
		u64 granted_caps = 0;
		u32 cap_ثنائی_ہندسہ;

		scx_cmask_ref_shard(&ref, si, قاش);
		اگر (scx_cmask_خالی_خانے(قاش))
			جاری;

		SCX_CMASK_DEFINE_SHARD(granted_cids, قاش->بنیاد, قاش->nr_cids);
		SCX_CMASK_DEFINE_SHARD(بدلا_cids, قاش->بنیاد, قاش->nr_cids);
		SCX_CMASK_DEFINE_SHARD(delta, قاش->بنیاد, قاش->nr_cids);

		scx_cmask_copy(granted_cids, قاش);

		scoped_guard (raw_spinlock, &pps->تالا) {
			guard(raw_spinlock_nested)(&cps->تالا);

			/*
			 * Narrow granted_cids to cids the parent holds every
			 * requested cap on. All-or-nothing per cid.
			 */
			scx_for_each_cap_ثنائی_ہندسہ(cap_ثنائی_ہندسہ, caps)
				scx_cmask_and(granted_cids, &pps->caps[cap_ثنائی_ہندسہ].cmask);

			/*
			 * For each requested cap, fold the newly-set cids into
			 * the child and accumulate the delta.
			 */
			scx_for_each_cap_ثنائی_ہندسہ(cap_ثنائی_ہندسہ, caps) {
				ساخت scx_cmask *ccm = &cps->caps[cap_ثنائی_ہندسہ].cmask;

				scx_cmask_copy(delta, granted_cids);
				scx_cmask_andnot(delta, ccm);
				اگر (scx_cmask_خالی_خانے(delta))
					جاری;

				scx_cmask_or(ccm, delta);
				scx_cmask_or(بدلا_cids, delta);
				granted_caps |= ثنائی_ہندسہ_U64(cap_ثنائی_ہندسہ);
			}

			اگر (granted_caps) {
				s32 cid;

				caps_updated_ریکارڈ(cps, بدلا_cids, granted_caps,
						    &مقصد_deliver);
				/*
				 * The sync arms an update_idle() re-notify if
				 * the cid gains baseline access, so the holder
				 * learns of an already-idle cid.
				 */
				scx_cmask_for_each_cid(cid, بدلا_cids)
					قطار_sync_ecaps(بچہ, cid);
			}
		}

		/* record cids that didn't make it into the denied mask */
		اگر (!scx_cmask_subset(قاش, granted_cids)) {
			any_denied = true;
			اگر (denied_برآمد_arena_nullable) {
				SCX_CMASK_DEFINE_SHARD(denied, قاش->بنیاد, قاش->nr_cids);

				scx_cmask_copy(denied, قاش);
				scx_cmask_andnot(denied, granted_cids);
				scx_cmask_ref_or(&denied_ref, denied);
			}
		}
	}

	caps_updated_deliver(&مقصد_deliver);

	واپس any_denied ? -EPERM : 0;
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
__bpf_kfunc خالی scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    ثابت ساخت scx_cmask *cmask__arena,
				    ثابت ساخت bpf_prog_aux *aux)
{
	ساخت scx_cmask_ref ref;
	ساخت scx_sched *والد, *بچہ, *pos;
	LIST_سر(مقصد_deliver);
	s32 si, ret;

	guard(irqsave)();

	اگر (sub_cap_preamble(cgroup_id, caps, aux, &والد, &بچہ))
		واپس;

	ret = scx_cmask_ref_init(والد, cmask__arena, &ref);
	اگر (ret) {
		scx_error(والد, "invalid cmask (%d)", ret);
		واپس;
	}

	/* per-shard, walk child's subtree and clear @caps */
	برائے (si = ref.shard_پہلا; si < ref.shard_انتہا; si++) {
		SCX_CMASK_DEFINE_SHARD(قاش, 0, SCX_CID_SHARD_MAX_CPUS);

		scx_cmask_ref_shard(&ref, si, قاش);
		اگر (scx_cmask_خالی_خانے(قاش))
			جاری;

		/*
		 * Pre-order with subtree skip: a descendant that cleared
		 * nothing means no descendant of it can hold @caps on these
		 * cids either.
		 */
		guard(raw_spinlock)(&والد->pshard[si]->تالا);
		pos = scx_اگلا_descendant_pre(تہی, بچہ);
		جب (pos) {
			ساخت scx_pshard *ps = pos->pshard[si];
			SCX_CMASK_DEFINE_SHARD(بدلا_cids, قاش->بنیاد, قاش->nr_cids);
			SCX_CMASK_DEFINE_SHARD(delta, قاش->بنیاد, قاش->nr_cids);
			u64 revoked_caps = 0;
			u32 cap_ثنائی_ہندسہ;

			scoped_guard (raw_spinlock_nested, &ps->تالا) {
				/*
				 * For each cap, clear lost cids and accumulate
				 * the per-cap diff for notification.
				 */
				scx_for_each_cap_ثنائی_ہندسہ(cap_ثنائی_ہندسہ, caps) {
					ساخت scx_cmask *cm = &ps->caps[cap_ثنائی_ہندسہ].cmask;

					scx_cmask_copy(delta, cm);
					scx_cmask_and(delta, قاش);
					اگر (scx_cmask_خالی_خانے(delta))
						جاری;

					scx_cmask_andnot(cm, delta);
					scx_cmask_or(بدلا_cids, delta);
					revoked_caps |= ثنائی_ہندسہ_U64(cap_ثنائی_ہندسہ);
				}

				اگر (revoked_caps) {
					s32 cid;

					caps_updated_ریکارڈ(ps, بدلا_cids, revoked_caps,
							    &مقصد_deliver);
					scx_cmask_for_each_cid(cid, بدلا_cids)
						قطار_sync_ecaps(pos, cid);
				}
			}

			اگر (revoked_caps)
				pos = scx_اگلا_descendant_pre(pos, بچہ);
			ورنہ
				pos = scx_skip_subtree_pre(pos, بچہ);
		}
	}

	caps_updated_deliver(&مقصد_deliver);
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
__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, ساخت scx_cmask *برآمد_arena,
				 ثابت ساخت bpf_prog_aux *aux)
{
	ساخت scx_cmask_ref ref;
	ساخت scx_sched *sch, *ہدف;
	ساخت scx_pshard **pshard;
	s32 si, ret;

	guard(irqsave)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		واپس -ENODEV;

	اگر (!scx_is_cid_type()) {
		scx_error(sch, "sub-cap kfuncs require a cid-form scheduler");
		واپس -EOPNOTSUPP;
	}

	اگر (unlikely(caps & ~__SCX_CAP_ALL)) {
		scx_error(sch, "invalid caps 0x%llx", caps);
		واپس -EINVAL;
	}

	/* @cgroup_id 0 reads self, otherwise a direct child */
	اگر (cgroup_id) {
		ہدف = scx_ڈھونڈو_sub_sched(cgroup_id);
		اگر (unlikely(!ہدف))
			واپس -ENODEV;
		اگر (unlikely(scx_والد(ہدف) != sch)) {
			scx_error(sch, "%s: sub-%llu is not a direct child",
				  sch->cgrp_راستہ, cgroup_id);
			واپس -EINVAL;
		}
	} ورنہ {
		ہدف = sch;
	}

	/*
	 * The target's caps storage may not be set up yet (e.g. a self-read
	 * during ops.init_cids()). Pairs with the publish in
	 * scx_alloc_pshards(): a non-NULL pshard has every element set and the
	 * acquire also orders the cid table reads below against it.
	 */
	pshard = smp_بھرو_acquire(&ہدف->pshard);
	اگر (unlikely(!pshard)) {
		scx_error(sch, "scx_bpf_sub_caps() called before caps storage is initialized");
		واپس -ENODEV;
	}

	ret = scx_cmask_ref_init(sch, برآمد_arena, &ref);
	اگر (ret) {
		scx_error(sch, "invalid out (%d)", ret);
		واپس ret;
	}

	برائے (si = ref.shard_پہلا; si < ref.shard_انتہا; si++) {
		ثابت ساخت scx_cid_shard *shard =
			&rcu_dereference_all(scx_cid_shard_ranges)[si];
		SCX_CMASK_DEFINE_SHARD(local_برآمد, shard->بنیاد_cid, shard->nr_cids);
		u32 cap_ثنائی_ہندسہ;

		scx_for_each_cap_ثنائی_ہندسہ(cap_ثنائی_ہندسہ, caps)
			scx_cmask_or(local_برآمد, &pshard[si]->caps[cap_ثنائی_ہندسہ].cmask);
		scx_cmask_ref_copy(&ref, local_برآمد);
	}
	واپس 0;
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
				      غیرعلامتی طویل طویل *ڈیٹا, u32 ڈیٹا_sz,
				      ثابت ساخت bpf_prog_aux *aux)
{
	ساخت scx_sched *والد, *بچہ;

	guard(rcu)();

	والد = scx_prog_sched(aux);
	اگر (unlikely(!والد))
		واپس -ENODEV;

	اگر (!scx_is_cid_type()) {
		scx_error(والد, "sub-cap kfuncs require a cid-form scheduler");
		واپس -EOPNOTSUPP;
	}

	بچہ = scx_ڈھونڈو_sub_sched(cgroup_id);
	اگر (unlikely(!بچہ))
		واپس -ENODEV;

	اگر (unlikely(scx_والد(بچہ) != والد)) {
		scx_error(والد, "%s: sub-%llu is not a direct child",
			  والد->cgrp_راستہ, cgroup_id);
		واپس -EINVAL;
	}

	scx_exit_bstr(بچہ, SCX_EXIT_والد_KILL, 0, والد, fmt, ڈیٹا, ڈیٹا_sz);
	واپس 0;
}

__bpf_kfunc_end_defs();

#ورنہ	/* !CONFIG_EXT_SUB_SCHED */

__bpf_kfunc_start_defs();

__bpf_kfunc s32 scx_bpf_sub_grant(u64 cgroup_id, u64 caps,
				  ثابت ساخت scx_cmask *cmask__arena,
				  ساخت scx_cmask *denied_برآمد_arena_nullable,
				  ثابت ساخت bpf_prog_aux *aux)
{
	واپس -EOPNOTSUPP;
}

__bpf_kfunc خالی scx_bpf_sub_revoke(u64 cgroup_id, u64 caps,
				    ثابت ساخت scx_cmask *cmask__arena,
				    ثابت ساخت bpf_prog_aux *aux)
{
}

__bpf_kfunc s32 scx_bpf_sub_caps(u64 cgroup_id, u64 caps, ساخت scx_cmask *برآمد_arena,
				 ثابت ساخت bpf_prog_aux *aux)
{
	واپس -EOPNOTSUPP;
}

__bpf_kfunc s32 scx_bpf_sub_kill_bstr(u64 cgroup_id, حرف *fmt,
				      غیرعلامتی طویل طویل *ڈیٹا, u32 ڈیٹا_sz,
				      ثابت ساخت bpf_prog_aux *aux)
{
	واپس -EOPNOTSUPP;
}

__bpf_kfunc_end_defs();

#اگر_ختم	/* CONFIG_EXT_SUB_SCHED */
