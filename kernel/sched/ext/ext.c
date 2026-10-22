/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ಅನ್ತರ್ಭಾವ <linux/bitmap.h>
#ಅನ್ತರ್ಭಾವ <linux/btf_ids.h>
#ಅನ್ತರ್ಭಾವ <linux/rhashtable.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/isolation.h>
#ಅನ್ತರ್ಭಾವ <linux/suspend.h>
#ಅನ್ತರ್ಭಾವ <linux/sysrq.h>

#ಅನ್ತರ್ಭಾವ "../pelt.h"
#ಅನ್ತರ್ಭಾವ "internal.h"
#ಅನ್ತರ್ಭಾವ "cid.h"
#ಅನ್ತರ್ಭಾವ "arena.h"
#ಅನ್ತರ್ಭಾವ "idle.h"
#ಅನ್ತರ್ಭಾವ "sub.h"
#ಅನ್ತರ್ಭಾವ "inlines.h"

DEFINE_RAW_SPINLOCK(scx_sched_ಬೀಗ);

bool scx_allow_proxy_exec(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ true;
}

/*
 * NOTE: sched_ext is in the process of growing multiple scheduler support and
 * scx_root usage is in a transitional state. Naked dereferences are safe if the
 * caller is one of the tasks attached to SCX and explicit RCU dereference is
 * necessary otherwise. Naked scx_root dereferences trigger sparse warnings but
 * are used as temporary markers to indicate that the dereferences need to be
 * updated to point to the associated scheduler instances rather than scx_root.
 */
ರಚನೆ scx_sched __rcu *scx_ಬೇರು;

/*
 * All scheds, writers must hold both scx_enable_mutex and scx_sched_lock.
 * Readers can hold either or rcu_read_lock().
 */
LIST_ತಲೆ(scx_sched_all);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
ಸ್ಥಿರಾಂಕ ರಚನೆ rhashtable_params scx_sched_hash_params = {
	.ಕೀಲಿ_len		= sizeof_ಕ್ಷೇತ್ರ(ರಚನೆ scx_sched, ops.sub_cgroup_id),
	.ಕೀಲಿ_ಸ್ಥಾನಾಂತರ		= offsetof(ರಚನೆ scx_sched, ops.sub_cgroup_id),
	.ತಲೆ_ಸ್ಥಾನಾಂತರ		= offsetof(ರಚನೆ scx_sched, hash_ಗಂಟು),
	.insecure_elasticity	= true,	/* inserted under scx_sched_lock */
};

ರಚನೆ rhashtable scx_sched_hash;
#ಆದರೆ_ಅಂತ್ಯ

/* see SCX_OPS_TID_TO_TASK */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ rhashtable_params scx_tid_hash_params = {
	.ಕೀಲಿ_len		= sizeof_ಕ್ಷೇತ್ರ(ರಚನೆ sched_ext_entity, tid),
	.ಕೀಲಿ_ಸ್ಥಾನಾಂತರ		= offsetof(ರಚನೆ sched_ext_entity, tid),
	.ತಲೆ_ಸ್ಥಾನಾಂತರ		= offsetof(ರಚನೆ sched_ext_entity, tid_hash_ಗಂಟು),
	.insecure_elasticity	= true,	/* inserted/removed under scx_tasks_lock */
};
ಸ್ಥಿರ ರಚನೆ rhashtable scx_tid_hash;

/*
 * During exit, a task may schedule after losing its PIDs. When disabling the
 * BPF scheduler, we need to be able to iterate tasks in every state to
 * guarantee system safety. Maintain a dedicated task list which contains every
 * task between its fork and eventual free.
 */
ಸ್ಥಿರ DEFINE_RAW_SPINLOCK(scx_tasks_ಬೀಗ);
ಸ್ಥಿರ LIST_ತಲೆ(scx_tasks);

/* ops enable/disable */
DEFINE_MUTEX(scx_enable_mutex);
DEFINE_STATIC_ಕೀಲಿ_FALSE(__scx_enabled);
DEFINE_PERCPU_RWSEM(scx_fork_rwsem);
ಸ್ಥಿರ atomic_t scx_enable_state_var = ATOMIC_INIT(SCX_DISABLED);
ಸ್ಥಿರ DEFINE_RAW_SPINLOCK(scx_bypass_ಬೀಗ);
ಸ್ಥಿರ bool scx_init_task_enabled;
ಸ್ಥಿರ bool scx_switching_all;
DEFINE_STATIC_ಕೀಲಿ_FALSE(__scx_switched_all);
ಸ್ಥಿರ DEFINE_STATIC_ಕೀಲಿ_FALSE(__scx_tid_to_task_enabled);

/*
 * Gates cgroup ops delivery. Set at the end of the cgroup init phase of root
 * enable and cleared before root disable starts tearing down tasks, both under
 * scx_cgroup_lock(). Holding cgroup_lock() and seeing %true guarantees no race
 * against root tearing down tasks.
 */
bool scx_cgroup_enabled;

/*
 * True once SCX_OPS_TID_TO_TASK has been negotiated with the root scheduler
 * and the tid->task table is live. Wraps the static key so callers don't
 * take the address, and hints "likely enabled" for the common case where
 * the feature is in use.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_tid_ಗಮ್ಯ_task_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_likely(&__scx_tid_to_task_enabled);
}

ಸ್ಥಿರ atomic_long_t scx_nr_rejected = ATOMIC_LONG_INIT(0);
ಸ್ಥಿರ atomic_long_t scx_hotplug_seq = ATOMIC_LONG_INIT(0);

/* Global cursor for the per-CPU tid allocator. Starts at 1; tid 0 is reserved. */
ಸ್ಥಿರ atomic64_t scx_tid_cursor = ATOMIC64_INIT(1);

/* is @dsq synchronized by the containing rq lock instead of dsq->lock? */
ಸ್ಥಿರ bool dsq_is_rq_owned(ರಚನೆ scx_dispatch_q *dsq)
{
	ಆಯ್ಕೆ (dsq->id) {
	ಸ್ಥಿತಿ SCX_DSQ_LOCAL:
	ಸ್ಥಿತಿ SCX_DSQ_REJECT:
	ಸ್ಥಿತಿ SCX_DSQ_RESCUE:
		ಹಿಂತಿರುಗಿ true;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ false;
	}
}

/* Cursor for unique scx_sched instance ids. id 0 is reserved. */
ಸ್ಥಿರ atomic64_t scx_sched_id_cursor = ATOMIC64_INIT(0);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
/*
 * The sub sched being enabled. Used by scx_disable_and_exit_task() to exit
 * tasks for the sub-sched being enabled. Use a global variable instead of a
 * per-task field as all enables are serialized.
 */
ರಚನೆ scx_sched *scx_enabling_sub_sched;
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು scx_enabling_sub_sched	(ರಚನೆ scx_sched *)ಖಾಲಿ
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

/*
 * A monotonically increasing sequence number that is incremented every time a
 * scheduler is enabled. This can be used to check if any custom sched_ext
 * scheduler has ever been used in the system.
 */
ಸ್ಥಿರ atomic_long_t scx_enable_seq = ATOMIC_LONG_INIT(0);

/*
 * Watchdog interval. All scx_sched's share a single watchdog timer and the
 * interval is half of the shortest sch->watchdog_timeout.
 */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ scx_watchdog_interval;

/*
 * The last time the delayed work was run. This delayed work relies on
 * ksoftirqd being able to run to service timer interrupts, so it's possible
 * that this work itself could get wedged. To account for this, we check that
 * it's not stalled in the timer tick, and trigger an error if it is.
 */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ scx_watchdog_timestamp = INITIAL_JIFFIES;

ಸ್ಥಿರ ರಚನೆ delayed_work scx_watchdog_work;

/*
 * For %SCX_KICK_WAIT: Each CPU has a pointer to an array of kick_sync sequence
 * numbers. The arrays are allocated with kvzalloc() as size can exceed percpu
 * allocator limits on large machines. O(nr_cpu_ids^2) allocation, allocated
 * lazily when enabling and freed when disabling to avoid waste when sched_ext
 * isn't active.
 */
ರಚನೆ scx_kick_syncs {
	ರಚನೆ rcu_ತಲೆ		rcu;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		syncs[];
};

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ scx_kick_syncs __rcu *, scx_kick_syncs);

/*
 * Per-CPU buffered allocator state for p->scx.tid. Each CPU pulls a chunk of
 * SCX_TID_CHUNK ids from scx_tid_cursor and hands them out locally without
 * further synchronization. See scx_alloc_tid().
 */
ರಚನೆ scx_tid_alloc {
	u64	ಮುಂದಿನ;
	u64	ಕೊನೆ;
};
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ scx_tid_alloc, scx_tid_alloc);

/*
 * Direct dispatch marker.
 *
 * Non-NULL values are used for direct dispatch from enqueue path. A valid
 * pointer points to the task currently being enqueued. An ERR_PTR value is used
 * to indicate that direct dispatch has already happened.
 */
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ task_struct *, ನೇರ_dispatch_task);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ rhashtable_params dsq_hash_params = {
	.ಕೀಲಿ_len		= sizeof_ಕ್ಷೇತ್ರ(ರಚನೆ scx_dispatch_q, id),
	.ಕೀಲಿ_ಸ್ಥಾನಾಂತರ		= offsetof(ರಚನೆ scx_dispatch_q, id),
	.ತಲೆ_ಸ್ಥಾನಾಂತರ		= offsetof(ರಚನೆ scx_dispatch_q, hash_ಗಂಟು),
};

ಸ್ಥಿರ LLIST_ತಲೆ(dsqs_ಗಮ್ಯ_free);

/* ops debug dump */
ಸ್ಥಿರ DEFINE_RAW_SPINLOCK(scx_dump_ಬೀಗ);

ರಚನೆ scx_dump_ದತ್ತಾಂಶ {
	s32			cpu;
	bool			ಮೊದಲ;
	s32			cursor;
	ರಚನೆ seq_buf		*s;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ		*ಪೂರ್ವಪ್ರತ್ಯಯ;
	ರಚನೆ scx_bstr_buf	buf;
};

ಸ್ಥಿರ ರಚನೆ scx_dump_ದತ್ತಾಂಶ scx_dump_ದತ್ತಾಂಶ = {
	.cpu			= -1,
};

/* /sys/kernel/sched_ext interface */
ಸ್ಥಿರ ರಚನೆ kset *scx_kset;

/*
 * Parameters that can be adjusted through /sys/module/sched_ext/parameters.
 * There usually is no reason to modify these as normal scheduler operation
 * shouldn't be affected by them. The knobs are primarily for debugging.
 */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ scx_ತುಂಡು_bypass_us = SCX_ತುಂಡು_BYPASS / NSEC_ಪ್ರತಿ_USEC;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ scx_bypass_lb_intv_us = SCX_BYPASS_LB_DFL_INTV_US;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_ತುಂಡು_us(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val, ಸ್ಥಿರಾಂಕ ರಚನೆ kernel_param *kp)
{
	ಹಿಂತಿರುಗಿ param_ಗಣ_uint_minmax(val, kp, 100, 100 * USEC_ಪ್ರತಿ_MSEC);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ kernel_param_ops ತುಂಡು_us_param_ops = {
	.ಗಣ = ಗಣ_ತುಂಡು_us,
	.get = param_get_uint,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಗಣ_bypass_lb_intv_us(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val, ಸ್ಥಿರಾಂಕ ರಚನೆ kernel_param *kp)
{
	ಹಿಂತಿರುಗಿ param_ಗಣ_uint_minmax(val, kp, 0, 10 * USEC_ಪ್ರತಿ_SEC);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ kernel_param_ops bypass_lb_intv_us_param_ops = {
	.ಗಣ = ಗಣ_bypass_lb_intv_us,
	.get = param_get_uint,
};

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MODULE_PARAM_ಪೂರ್ವಪ್ರತ್ಯಯ
#ವ್ಯಾಖ್ಯಾನಿಸು MODULE_PARAM_ಪೂರ್ವಪ್ರತ್ಯಯ	"sched_ext."

module_param_cb(ತುಂಡು_bypass_us, &ತುಂಡು_us_param_ops, &scx_ತುಂಡು_bypass_us, 0600);
MODULE_PARM_DESC(ತುಂಡು_bypass_us, "bypass slice in microseconds, applied on [un]load (100us to 100ms)");
module_param_cb(bypass_lb_intv_us, &bypass_lb_intv_us_param_ops, &scx_bypass_lb_intv_us, 0600);
MODULE_PARM_DESC(bypass_lb_intv_us, "bypass load balance interval in microseconds (0 (disable) to 10s)");

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MODULE_PARAM_ಪೂರ್ವಪ್ರತ್ಯಯ

#ವ್ಯಾಖ್ಯಾನಿಸು CREATE_TRACE_POINTS
#ಅನ್ತರ್ಭಾವ <trace/events/sched_ext.h>

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_deferred(ರಚನೆ rq *rq);
ಸ್ಥಿರ bool task_dead_and_ಮುಗಿದ(ರಚನೆ task_struct *p);
ಸ್ಥಿರ ಶೂನ್ಯ scx_disable(ರಚನೆ scx_sched *sch, ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ);

__printf(5, 6) bool __scx_exit(ರಚನೆ scx_sched *sch,
			       ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ, s64 exit_ಸಂಕೇತ,
			       s32 exit_cpu, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
	va_list ವಾದಗಳು;
	bool ret;

	va_ಆರಂಭ(ವಾದಗಳು, fmt);
	ret = scx_vexit(sch, ಬಗೆ, exit_ಸಂಕೇತ, exit_cpu, fmt, ವಾದಗಳು);
	va_ಕೊನೆ(ವಾದಗಳು);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ದೀರ್ಘ jiffies_delta_msecs(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ at, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ now)
{
	ಆದರೆ (ಸಮಯ_after(at, now))
		ಹಿಂತಿರುಗಿ jiffies_ಗಮ್ಯ_msecs(at - now);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ -(ದೀರ್ಘ)jiffies_ಗಮ್ಯ_msecs(now - at);
}

ಸ್ಥಿರ bool u32_before(u32 a, u32 b)
{
	ಹಿಂತಿರುಗಿ (s32)(a - b) < 0;
}

/**
 * scx_is_descendant - Test whether sched is a descendant
 * @sch: sched to test
 * @ancestor: ancestor sched to test against
 *
 * Test whether @sch is a descendant of @ancestor.
 */
bool scx_is_descendant(ರಚನೆ scx_sched *sch, ರಚನೆ scx_sched *ancestor)
{
	ಆದರೆ (sch->level < ancestor->level)
		ಹಿಂತಿರುಗಿ false;
	ಹಿಂತಿರುಗಿ sch->ancestors[ancestor->level] == ancestor;
}

ಸ್ಥಿರ ರಚನೆ scx_dispatch_q *ಕಂಡುಹಿಡಿ_global_dsq(ರಚನೆ scx_sched *sch, s32 cpu)
{
	ಹಿಂತಿರುಗಿ &sch->pnode[cpu_ಗಮ್ಯ_ಗಂಟು(cpu)]->global_dsq;
}

ಸ್ಥಿರ ರಚನೆ scx_dispatch_q *ಕಂಡುಹಿಡಿ_user_dsq(ರಚನೆ scx_sched *sch, u64 dsq_id)
{
	ಹಿಂತಿರುಗಿ rhashtable_lookup(&sch->dsq_hash, &dsq_id, dsq_hash_params);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *scx_setscheduler_class(ರಚನೆ task_struct *p)
{
	ಆದರೆ (p->sched_class == &stop_sched_class)
		ಹಿಂತಿರುಗಿ &stop_sched_class;

	ಹಿಂತಿರುಗಿ __setscheduler_class(p->policy, p->prio);
}

ಸ್ಥಿರ ರಚನೆ scx_dispatch_q *bypass_enq_ಗುರಿ_dsq(ರಚನೆ scx_sched *sch, s32 cpu)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	/*
	 * If @sch is a sub-sched which is bypassing, its tasks should go into
	 * the bypass DSQs of the nearest ancestor which is not bypassing. The
	 * not-bypassing ancestor is responsible for scheduling all tasks from
	 * bypassing sub-trees. If all ancestors including root are bypassing,
	 * all tasks should go to the root's bypass DSQs.
	 *
	 * Whenever a sched starts bypassing, all runnable tasks in its subtree
	 * are re-enqueued after scx_bypassing() is turned on, guaranteeing that
	 * all tasks are transferred to the right DSQs.
	 */
	ಆಗ (scx_ಪೋಷಕ(sch) && scx_bypassing(sch, cpu))
		sch = scx_ಪೋಷಕ(sch);
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

	ಹಿಂತಿರುಗಿ scx_bypass_dsq(sch, cpu);
}

/**
 * rq_is_open - Is the rq available for immediate execution of an SCX task?
 * @rq: rq to test
 * @enq_flags: optional %SCX_ENQ_* of the task being enqueued
 *
 * Returns %true if @rq is currently open for executing an SCX task. After a
 * %false return, @rq is guaranteed to invoke SCX dispatch path at least once
 * before going to idle and not inserting a task into @rq's local DSQ after a
 * %false return doesn't cause @rq to stall.
 */
ಸ್ಥಿರ bool rq_is_open(ರಚನೆ rq *rq, u64 enq_flags)
{
	lockdep_assert_rq_held(rq);

	/*
	 * A higher-priority class task is either running or in the process of
	 * waking up on @rq.
	 */
	ಆದರೆ (sched_class_above(rq->ಮುಂದಿನ_class, &ext_sched_class))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * @rq is either in transition to or in idle and there is no
	 * higher-priority class task waking up on it.
	 */
	ಆದರೆ (sched_class_above(&ext_sched_class, rq->ಮುಂದಿನ_class))
		ಹಿಂತಿರುಗಿ true;

	/*
	 * @rq is either picking, in transition to, or running an SCX task.
	 */

	/*
	 * If we're in the dispatch path holding rq lock, $curr may or may not
	 * be ready depending on whether the on-going dispatch decides to extend
	 * $curr's slice. We say yes here and resolve it at the end of dispatch.
	 * See dispatch_one().
	 */
	ಆದರೆ (rq->scx.flags & SCX_RQ_ಒಳ_DISPATCH)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * %SCX_ENQ_PREEMPT clears $curr's slice if on SCX and kicks dispatch,
	 * so allow it to avoid spuriously triggering reenq on a combined
	 * PREEMPT|IMMED insertion.
	 */
	ಆದರೆ (enq_flags & SCX_ENQ_PREEMPT) {
		ರಚನೆ task_struct *curr = rq->curr;

		/*
		 * A protected slice refuses the preemption and the cpu stays
		 * occupied. See rq_owned_post_enq().
		 */
		ಹಿಂತಿರುಗಿ curr->sched_class != &ext_sched_class ||
			likely(!(curr->scx.flags & SCX_TASK_PROTECTED));
	}

	/*
	 * @rq is either in transition to or running an SCX task and can't go
	 * idle without another SCX dispatch cycle.
	 */
	ಹಿಂತಿರುಗಿ false;
}

/*
 * Track the rq currently locked.
 *
 * This allows kfuncs to safely operate on rq from any scx ops callback,
 * knowing which rq is already locked.
 */
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ rq *, scx_locked_rq_state);

/*
 * Under core scheduling, a pick that releases the rq lock invalidates the
 * core-wide selection it is part of. Count the releases so that the core-sched
 * pick can tell whether one happened across dispatch.
 */
ಸ್ಥಿರ ಶೂನ್ಯ scx_rq_ಬೀಗ_drop(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
	ಆದರೆ (sched_core_enabled(rq))
		rq->scx.ಬೀಗ_drop_seq++;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ switch_rq_ಬೀಗ(ರಚನೆ rq *from, ರಚನೆ rq *ಗಮ್ಯ)
{
	bool tracked = scx_locked_rq() == from;

	ಆದರೆ (tracked)
		update_locked_rq(ಖಾಲಿ);
	scx_rq_ಬೀಗ_drop(from);
	raw_spin_rq_unlock(from);
	raw_spin_rq_ಬೀಗ(ಗಮ್ಯ);
	ಆದರೆ (tracked)
		update_locked_rq(ಗಮ್ಯ);
}

/*
 * Flipped on enable per sch->is_cid_type. Declared in internal.h so
 * subsystem inlines can read it.
 */
DEFINE_STATIC_ಕೀಲಿ_FALSE(__scx_is_cid_type);

/**
 * scx_fill_cmask_scratch - Build this cpu's arena cmask from @cpumask
 * @sch: scx_sched whose scratch to fill
 * @cpumask: cpus to translate into cids
 *
 * The scratch lives in BPF-writable arena memory and its header can't be
 * trusted, so it is rewritten from kernel geometry rather than read. Caller
 * must hold an rq lock so this cpu is the sole kernel writer for as long as the
 * returned address is in use.
 */
ಸ್ಥಿರ ರಚನೆ scx_cmask *scx_fill_cmask_scratch(ರಚನೆ scx_sched *sch,
						ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpumask)
{
	ರಚನೆ scx_cmask *kern_va = *this_cpu_ptr(sch->ಗಣ_cmask_scratch);
	ರಚನೆ scx_cmask_ref ref;

	scx_cmask_ref_init_kern(sch, kern_va, 0, ಅಂಕೆ_possible_cpus(), &ref);
	scx_cmask_ref_from_cpumask(&ref, cpumask);
	ಹಿಂತಿರುಗಿ kern_va;
}

/**
 * scx_call_op_set_cpumask - Invoke the set_cpumask or set_cmask op for @task
 * @sch: scx_sched being invoked
 * @rq: rq to update as the currently-locked rq, or NULL
 * @task: task whose affinity is changing
 * @cpumask: new cpumask
 *
 * For cid-form schedulers, translate @cpumask to a cmask in the per-cpu scratch
 * and dispatch through the ops_cid union view. Caller must hold @rq's rq lock.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_call_op_ಗಣ_cpumask(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq,
					   ರಚನೆ task_struct *task,
					   ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpumask)
{
	ಆದರೆ (scx_is_cid_type())
		SCX_CALL_CID_OP_TASK(sch, ಗಣ_cmask, rq, task,
				     scx_fill_cmask_scratch(sch, cpumask));
	ಇಲ್ಲದಿದ್ದರೆ
		SCX_CALL_OP_TASK(sch, ಗಣ_cpumask, rq, task, cpumask);
}

ಎಣಿಕೆ scx_dsq_iter_flags {
	/* iterate in the reverse dispatch order */
	SCX_DSQ_ITER_REV		= 1U << 16,

	__SCX_DSQ_ITER_HAS_SLICE	= 1U << 30,
	__SCX_DSQ_ITER_HAS_VTIME	= 1U << 31,

	__SCX_DSQ_ITER_USER_FLAGS	= SCX_DSQ_ITER_REV,
	__SCX_DSQ_ITER_ALL_FLAGS	= __SCX_DSQ_ITER_USER_FLAGS |
					  __SCX_DSQ_ITER_HAS_SLICE |
					  __SCX_DSQ_ITER_HAS_VTIME,
};

/**
 * nldsq_next_task - Iterate to the next task in a non-local DSQ
 * @dsq: non-local dsq being iterated
 * @cur: current position, %NULL to start iteration
 * @rev: walk backwards
 *
 * Returns %NULL when iteration is finished.
 */
ಸ್ಥಿರ ರಚನೆ task_struct *nldsq_ಮುಂದಿನ_task(ರಚನೆ scx_dispatch_q *dsq,
					   ರಚನೆ task_struct *cur, bool rev)
{
	ರಚನೆ list_ತಲೆ *list_ಗಂಟು;
	ರಚನೆ scx_dsq_list_ಗಂಟು *dsq_lnode;

	lockdep_assert_held(&dsq->ಬೀಗ);

	ಆದರೆ (cur)
		list_ಗಂಟು = &cur->scx.dsq_list.ಗಂಟು;
	ಇಲ್ಲದಿದ್ದರೆ
		list_ಗಂಟು = &dsq->list;

	/* find the next task, need to skip BPF iteration cursors */
	ಮಾಡು {
		ಆದರೆ (rev)
			list_ಗಂಟು = list_ಗಂಟು->prev;
		ಇಲ್ಲದಿದ್ದರೆ
			list_ಗಂಟು = list_ಗಂಟು->ಮುಂದಿನ;

		ಆದರೆ (list_ಗಂಟು == &dsq->list)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;

		dsq_lnode = container_ದ(list_ಗಂಟು, ರಚನೆ scx_dsq_list_ಗಂಟು,
					 ಗಂಟು);
	} ಆಗ (dsq_lnode->flags & SCX_DSQ_LNODE_ITER_CURSOR);

	ಹಿಂತಿರುಗಿ container_ದ(dsq_lnode, ರಚನೆ task_struct, scx.dsq_list);
}

#ವ್ಯಾಖ್ಯಾನಿಸು nldsq_for_each_task(p, dsq)						\
	ಗಾಗಿ ((p) = nldsq_ಮುಂದಿನ_task((dsq), ಖಾಲಿ, false); (p);			\
	     (p) = nldsq_ಮುಂದಿನ_task((dsq), (p), false))

/**
 * nldsq_cursor_next_task - Iterate to the next task given a cursor in a non-local DSQ
 * @cursor: scx_dsq_list_node initialized with INIT_DSQ_LIST_CURSOR()
 * @dsq: non-local dsq being iterated
 *
 * Find the next task in a cursor based iteration. The caller must have
 * initialized @cursor using INIT_DSQ_LIST_CURSOR() and can release the DSQ lock
 * between the iteration steps.
 *
 * Only tasks which were queued before @cursor was initialized are visible. This
 * bounds the iteration and guarantees that vtime never jumps in the other
 * direction while iterating.
 */
ಸ್ಥಿರ ರಚನೆ task_struct *nldsq_cursor_ಮುಂದಿನ_task(ರಚನೆ scx_dsq_list_ಗಂಟು *cursor,
						  ರಚನೆ scx_dispatch_q *dsq)
{
	bool rev = cursor->flags & SCX_DSQ_ITER_REV;
	ರಚನೆ task_struct *p;

	lockdep_assert_held(&dsq->ಬೀಗ);
	BUG_ON(!(cursor->flags & SCX_DSQ_LNODE_ITER_CURSOR));

	ಆದರೆ (list_ಖಾಲಿಗಳು(&cursor->ಗಂಟು))
		p = ಖಾಲಿ;
	ಇಲ್ಲದಿದ್ದರೆ
		p = container_ದ(cursor, ರಚನೆ task_struct, scx.dsq_list);

	/* skip cursors and tasks that were queued after @cursor init */
	ಮಾಡು {
		p = nldsq_ಮುಂದಿನ_task(dsq, p, rev);
	} ಆಗ (p && unlikely(u32_before(cursor->priv, p->scx.dsq_seq)));

	ಆದರೆ (p) {
		ಆದರೆ (rev)
			list_move_ಬಾಲ(&cursor->ಗಂಟು, &p->scx.dsq_list.ಗಂಟು);
		ಇಲ್ಲದಿದ್ದರೆ
			list_move(&cursor->ಗಂಟು, &p->scx.dsq_list.ಗಂಟು);
	} ಇಲ್ಲದಿದ್ದರೆ {
		list_del_init(&cursor->ಗಂಟು);
	}

	ಹಿಂತಿರುಗಿ p;
}

/**
 * nldsq_cursor_lost_task - Test whether someone else took the task since iteration
 * @cursor: scx_dsq_list_node initialized with INIT_DSQ_LIST_CURSOR()
 * @rq: rq @p was on
 * @dsq: dsq @p was on
 * @p: target task
 *
 * @p is a task returned by nldsq_cursor_next_task(). The locks may have been
 * dropped and re-acquired inbetween. Verify that no one else took or is in the
 * process of taking @p from @dsq.
 *
 * On %false return, the caller can assume full ownership of @p.
 */
ಸ್ಥಿರ bool nldsq_cursor_ಕಳೆದುಹೋದ_task(ರಚನೆ scx_dsq_list_ಗಂಟು *cursor,
				   ರಚನೆ rq *rq, ರಚನೆ scx_dispatch_q *dsq,
				   ರಚನೆ task_struct *p)
{
	lockdep_assert_rq_held(rq);
	lockdep_assert_held(&dsq->ಬೀಗ);

	/*
	 * @p could have already left $src_dsq, got re-enqueud, or be in the
	 * process of being consumed by someone else.
	 */
	ಆದರೆ (unlikely(p->scx.dsq != dsq ||
		     u32_before(cursor->priv, p->scx.dsq_seq) ||
		     p->scx.holding_cpu >= 0))
		ಹಿಂತಿರುಗಿ true;

	/* if @p has stayed on @dsq, its rq couldn't have changed */
	ಆದರೆ (WARN_ON_ONCE(rq != task_rq(p)))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

/*
 * BPF DSQ iterator. Tasks in a non-local DSQ can be iterated in [reverse]
 * dispatch order. BPF-visible iterator is opaque and larger to allow future
 * changes without breaking backward compatibility. Can be used with
 * bpf_for_each(). See bpf_iter_scx_dsq_*().
 */
ರಚನೆ bpf_iter_scx_dsq_kern {
	ರಚನೆ scx_dsq_list_ಗಂಟು	cursor;
	ರಚನೆ scx_dispatch_q		*dsq;
	u64				ತುಂಡು;
	u64				vtime;
} __attribute__((aligned(8)));

ರಚನೆ bpf_iter_scx_dsq {
	u64				__opaque[6];
} __attribute__((aligned(8)));


u32 scx_get_task_state(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->scx.flags & SCX_TASK_STATE_ಮುಖವಾಡ;
}

ಶೂನ್ಯ scx_ಗಣ_task_state(ರಚನೆ task_struct *p, u32 state)
{
	u32 prev_state = scx_get_task_state(p);
	bool warn = false;

	ಆಯ್ಕೆ (state) {
	ಸ್ಥಿತಿ SCX_TASK_NONE:
		warn = prev_state == SCX_TASK_DEAD;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_TASK_INIT_BEGIN:
		warn = prev_state != SCX_TASK_NONE;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_TASK_INIT:
		warn = prev_state != SCX_TASK_INIT_BEGIN;
		p->scx.flags |= SCX_TASK_RESET_RUNNABLE_AT;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_TASK_READY:
		warn = !(prev_state == SCX_TASK_INIT ||
			 prev_state == SCX_TASK_ENABLED);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_TASK_ENABLED:
		warn = prev_state != SCX_TASK_READY;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_TASK_DEAD:
		warn = !(prev_state == SCX_TASK_NONE ||
			 prev_state == SCX_TASK_INIT_BEGIN);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		WARN_ONCE(1, "sched_ext: Invalid task state %d -> %d for %s[%d]",
			  prev_state, state, p->comm, p->pid);
		ಹಿಂತಿರುಗಿ;
	}

	WARN_ONCE(warn, "sched_ext: Invalid task state transition 0x%x -> 0x%x for %s[%d]",
		  prev_state, state, p->comm, p->pid);

	p->scx.flags &= ~SCX_TASK_STATE_ಮುಖವಾಡ;
	p->scx.flags |= state;
}

/**
 * scx_task_iter_start - Lock scx_tasks_lock and start a task iteration
 * @iter: iterator to init
 * @cgrp: Optional root of cgroup subhierarchy to iterate
 *
 * Initialize @iter. Once initialized, @iter must eventually be stopped with
 * scx_task_iter_stop().
 *
 * If @cgrp is %NULL, scx_tasks is used for iteration and this function returns
 * with scx_tasks_lock held and @iter->cursor inserted into scx_tasks.
 *
 * If @cgrp is not %NULL, @cgrp and its descendants' tasks are walked using
 * @iter->css_iter. The caller must be holding cgroup_lock() to prevent cgroup
 * task migrations.
 *
 * The two modes of iterations are largely independent and it's likely that
 * scx_tasks can be removed in favor of always using cgroup iteration if
 * CONFIG_SCHED_CLASS_EXT depends on CONFIG_CGROUPS.
 *
 * scx_tasks_lock and the rq lock may be released using scx_task_iter_unlock()
 * between this and the first next() call or between any two next() calls. If
 * the locks are released between two next() calls, the caller is responsible
 * for ensuring that the task being iterated remains accessible either through
 * RCU read lock or obtaining a reference count.
 *
 * All tasks which existed when the iteration started are guaranteed to be
 * visited as long as they are not dead.
 */
ಶೂನ್ಯ scx_task_iter_ಆರಂಭ(ರಚನೆ scx_task_iter *iter, ರಚನೆ cgroup *cgrp)
{
	memset(iter, 0, ಗಾತ್ರ(*iter));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಆದರೆ (cgrp) {
		lockdep_assert_held(&cgroup_mutex);
		iter->cgrp = cgrp;
		iter->css_pos = css_ಮುಂದಿನ_descendant_pre(ಖಾಲಿ, &iter->cgrp->self);
		css_task_iter_ಆರಂಭ(iter->css_pos, CSS_TASK_ITER_WITH_DEAD,
				    &iter->css_iter);
		ಹಿಂತಿರುಗಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ
	raw_spin_ಬೀಗ_irq(&scx_tasks_ಬೀಗ);

	iter->cursor = (ರಚನೆ sched_ext_entity){ .flags = SCX_TASK_CURSOR };
	list_ಕೂಡಿಸು(&iter->cursor.tasks_ಗಂಟು, &scx_tasks);
	iter->list_locked = true;
}

ಸ್ಥಿರ ಶೂನ್ಯ __scx_task_iter_rq_unlock(ರಚನೆ scx_task_iter *iter)
{
	ಆದರೆ (iter->locked_task) {
		__balance_callbacks(iter->rq, &iter->rf);
		task_rq_unlock(iter->rq, iter->locked_task, &iter->rf);
		iter->locked_task = ಖಾಲಿ;
	}
}

/**
 * scx_task_iter_unlock - Unlock rq and scx_tasks_lock held by a task iterator
 * @iter: iterator to unlock
 *
 * If @iter is in the middle of a locked iteration, it may be locking the rq of
 * the task currently being visited in addition to scx_tasks_lock. Unlock both.
 * This function can be safely called anytime during an iteration. The next
 * iterator operation will automatically restore the necessary locking.
 */
ಶೂನ್ಯ scx_task_iter_unlock(ರಚನೆ scx_task_iter *iter)
{
	__scx_task_iter_rq_unlock(iter);
	ಆದರೆ (iter->list_locked) {
		iter->list_locked = false;
		raw_spin_unlock_irq(&scx_tasks_ಬೀಗ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __scx_task_iter_maybe_relock(ರಚನೆ scx_task_iter *iter)
{
	ಆದರೆ (!iter->list_locked) {
		raw_spin_ಬೀಗ_irq(&scx_tasks_ಬೀಗ);
		iter->list_locked = true;
	}
}

/**
 * scx_task_iter_relock - Re-acquire scx_tasks_lock and, optionally, @p's rq
 * @iter: iterator to relock
 * @p: task whose rq to lock, or %NULL for scx_tasks_lock only
 *
 * Counterpart to scx_task_iter_unlock(). Locking @p's rq is optional. Once
 * re-acquired, both locks are managed by the iterator from here on.
 */
ಸ್ಥಿರ ಶೂನ್ಯ scx_task_iter_relock(ರಚನೆ scx_task_iter *iter,
				 ರಚನೆ task_struct *p)
{
	__scx_task_iter_maybe_relock(iter);
	ಆದರೆ (p) {
		iter->rq = task_rq_ಬೀಗ_2(p, &iter->rf);
		iter->locked_task = p;
	}
}

/**
 * scx_task_iter_stop - Stop a task iteration and unlock scx_tasks_lock
 * @iter: iterator to exit
 *
 * Exit a previously initialized @iter. Must be called with scx_tasks_lock held
 * which is released on return. If the iterator holds a task's rq lock, that rq
 * lock is also released. See scx_task_iter_start() for details.
 */
ಶೂನ್ಯ scx_task_iter_stop(ರಚನೆ scx_task_iter *iter)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಆದರೆ (iter->cgrp) {
		ಆದರೆ (iter->css_pos)
			css_task_iter_ಕೊನೆ(&iter->css_iter);
		__scx_task_iter_rq_unlock(iter);
		ಹಿಂತಿರುಗಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ
	__scx_task_iter_maybe_relock(iter);
	list_del_init(&iter->cursor.tasks_ಗಂಟು);
	scx_task_iter_unlock(iter);
}

/**
 * scx_task_iter_next - Next task
 * @iter: iterator to walk
 *
 * Visit the next task. See scx_task_iter_start() for details. Locks are dropped
 * and re-acquired every %SCX_TASK_ITER_BATCH iterations to avoid causing stalls
 * by holding scx_tasks_lock for too long.
 */
ಸ್ಥಿರ ರಚನೆ task_struct *scx_task_iter_ಮುಂದಿನ(ರಚನೆ scx_task_iter *iter)
{
	ರಚನೆ list_ತಲೆ *cursor = &iter->cursor.tasks_ಗಂಟು;
	ರಚನೆ sched_ext_entity *pos;

	ಆದರೆ (!(++iter->cnt % SCX_TASK_ITER_BATCH)) {
		scx_task_iter_unlock(iter);
		cond_resched();
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಆದರೆ (iter->cgrp) {
		ಆಗ (iter->css_pos) {
			ರಚನೆ task_struct *p;

			p = css_task_iter_ಮುಂದಿನ(&iter->css_iter);
			ಆದರೆ (p)
				ಹಿಂತಿರುಗಿ p;

			css_task_iter_ಕೊನೆ(&iter->css_iter);
			iter->css_pos = css_ಮುಂದಿನ_descendant_pre(iter->css_pos,
								&iter->cgrp->self);
			ಆದರೆ (iter->css_pos)
				css_task_iter_ಆರಂಭ(iter->css_pos, CSS_TASK_ITER_WITH_DEAD,
						    &iter->css_iter);
		}
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ
	__scx_task_iter_maybe_relock(iter);

	list_for_each_ನಮೂದು(pos, cursor, tasks_ಗಂಟು) {
		ಆದರೆ (&pos->tasks_ಗಂಟು == &scx_tasks)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		ಆದರೆ (!(pos->flags & SCX_TASK_CURSOR)) {
			list_move(cursor, &pos->tasks_ಗಂಟು);
			ಹಿಂತಿರುಗಿ container_ದ(pos, ರಚನೆ task_struct, scx);
		}
	}

	/* can't happen, should always terminate at scx_tasks above */
	BUG();
}

/**
 * scx_task_iter_next_locked - Next non-idle task with its rq locked
 * @iter: iterator to walk
 *
 * Visit the non-idle task with its rq lock held. Allows callers to specify
 * whether they would like to filter out dead tasks. See scx_task_iter_start()
 * for details.
 */
ರಚನೆ task_struct *scx_task_iter_ಮುಂದಿನ_locked(ರಚನೆ scx_task_iter *iter)
{
	ರಚನೆ task_struct *p;

	__scx_task_iter_rq_unlock(iter);

	ಆಗ ((p = scx_task_iter_ಮುಂದಿನ(iter))) {
		/*
		 * scx_task_iter is used to prepare and move tasks into SCX
		 * while loading the BPF scheduler and vice-versa while
		 * unloading. The init_tasks ("swappers") should be excluded
		 * from the iteration because:
		 *
		 * - It's unsafe to use __setscheduler_class() on an init_task
		 *   to determine the sched_class to use as it won't preserve
		 *   its idle_sched_class.
		 *
		 * - ops.init/exit_task() can easily be confused if called with
		 *   init_tasks as they, e.g., share PID 0.
		 *
		 * As init_tasks are never scheduled through SCX, they can be
		 * skipped safely. Note that is_idle_task() which tests %PF_IDLE
		 * doesn't work here:
		 *
		 * - %PF_IDLE may not be set for an init_task whose CPU hasn't
		 *   yet been onlined.
		 *
		 * - %PF_IDLE can be set on tasks that are not init_tasks. See
		 *   play_idle_precise() used by CONFIG_IDLE_INJECT.
		 *
		 * Test for idle_sched_class as only init_tasks are on it.
		 */
		ಆದರೆ (p->sched_class == &idle_sched_class)
			ಮುಂದುವರಿ;

		iter->rq = task_rq_ಬೀಗ_2(p, &iter->rf);
		iter->locked_task = p;

		/*
		 * cgroup_task_dead() removes the dead tasks from cset->tasks
		 * after sched_ext_dead() and cgroup iteration may see tasks
		 * which already finished sched_ext_dead(). %SCX_TASK_DEAD is
		 * set by sched_ext_dead() under @p's rq lock. Test it to
		 * avoid visiting tasks which are already dead from SCX POV.
		 */
		ಆದರೆ (scx_get_task_state(p) == SCX_TASK_DEAD) {
			__scx_task_iter_rq_unlock(iter);
			ಮುಂದುವರಿ;
		}

		ಹಿಂತಿರುಗಿ p;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/**
 * scx_dump_event - Dump an event 'kind' in 'events' to 's'
 * @s: output seq_buf
 * @events: event stats
 * @kind: a kind of event to dump
 */
#ವ್ಯಾಖ್ಯಾನಿಸು scx_dump_event(s, events, ಬಗೆ) ಮಾಡು {					\
	scx_dump_ಸಾಲು(&(s), "%40s: %16lld", #kind, (events)->ಬಗೆ);		\
} ಆಗ (0)


ಸ್ಥಿರ ಶೂನ್ಯ scx_read_events(ರಚನೆ scx_sched *sch,
			    ರಚನೆ scx_event_stats *events);

ಸ್ಥಿರ ಎಣಿಕೆ scx_enable_state scx_enable_state(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ atomic_read(&scx_enable_state_var);
}

ಸ್ಥಿರ ಎಣಿಕೆ scx_enable_state scx_ಗಣ_enable_state(ಎಣಿಕೆ scx_enable_state ಗಮ್ಯ)
{
	ಹಿಂತಿರುಗಿ atomic_xchg(&scx_enable_state_var, ಗಮ್ಯ);
}

ಸ್ಥಿರ bool scx_tryset_enable_state(ಎಣಿಕೆ scx_enable_state ಗಮ್ಯ,
				    ಎಣಿಕೆ scx_enable_state from)
{
	ಪೂರ್ಣಾಂಕ from_v = from;

	ಹಿಂತಿರುಗಿ atomic_try_cmpxchg(&scx_enable_state_var, &from_v, ಗಮ್ಯ);
}

/**
 * wait_ops_state - Busy-wait the specified ops state to end
 * @p: target task
 * @opss: state to wait the end of
 *
 * Busy-wait for @p to transition out of @opss. This can only be used when the
 * state part of @opss is %SCX_QUEUEING or %SCX_DISPATCHING. This function also
 * has load_acquire semantics to ensure that the caller can see the updates made
 * in the enqueueing and dispatching paths.
 */
ಸ್ಥಿರ ಶೂನ್ಯ wait_ops_state(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ opss)
{
	ಮಾಡು {
		cpu_relax();
	} ಆಗ (atomic_long_read_acquire(&p->scx.ops_state) == opss);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool __cpu_valid(s32 cpu)
{
	ಹಿಂತಿರುಗಿ likely(cpu >= 0 && cpu < nr_cpu_ids && cpu_possible(cpu));
}

/**
 * scx_cpu_valid - Verify a cpu number, to be used on ops input args
 * @sch: scx_sched to abort on error
 * @cpu: cpu number which came from a BPF ops
 * @where: extra information reported on error
 *
 * @cpu is a cpu number which came from the BPF scheduler and can be any value.
 * Verify that it is in range and one of the possible cpus. If invalid, trigger
 * an ops error.
 */
bool scx_cpu_valid(ರಚನೆ scx_sched *sch, s32 cpu, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *where)
{
	ಆದರೆ (__cpu_valid(cpu)) {
		ಹಿಂತಿರುಗಿ true;
	} ಇಲ್ಲದಿದ್ದರೆ {
		scx_error(sch, "invalid CPU %d%s%s", cpu, where ? " " : "", where ?: "");
		ಹಿಂತಿರುಗಿ false;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ deferred_bal_cb_workfn(ರಚನೆ rq *rq)
{
	ಸತತ_deferred(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ deferred_irq_workfn(ರಚನೆ irq_work *irq_work)
{
	ರಚನೆ rq *rq = container_ದ(irq_work, ರಚನೆ rq, scx.deferred_irq_work);

	raw_spin_rq_ಬೀಗ(rq);
	ಸತತ_deferred(rq);
	scx_rq_ಬೀಗ_drop(rq);
	raw_spin_rq_unlock(rq);
}

/**
 * schedule_deferred - Schedule execution of deferred actions on an rq
 * @rq: target rq
 *
 * Schedule execution of deferred actions on @rq. Deferred actions are executed
 * with @rq locked but unpinned, and thus can unlock @rq to e.g. migrate tasks
 * to other rqs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ವೇಳಾಪಟ್ಟಿ_deferred(ರಚನೆ rq *rq)
{
	/*
	 * This is the fallback when schedule_deferred_locked() can't use
	 * the cheaper balance callback or wakeup hook paths (the target
	 * CPU is not in dispatch or wakeup). Currently, this is primarily
	 * hit by reenqueue operations targeting a remote CPU.
	 *
	 * Queue on the target CPU. The deferred work can run from any CPU
	 * correctly - the _locked() path already processes remote rqs from
	 * the calling CPU - but targeting the owning CPU allows IPI delivery
	 * without waiting for the calling CPU to re-enable IRQs and is
	 * cheaper as the reenqueue runs locally.
	 */
	irq_work_ಸರತಿ_on(&rq->scx.deferred_irq_work, cpu_ದ(rq));
}

/**
 * schedule_deferred_locked - Schedule execution of deferred actions on an rq
 * @rq: target rq
 *
 * Schedule execution of deferred actions on @rq. Equivalent to
 * schedule_deferred() but requires @rq to be locked and can be more efficient.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ವೇಳಾಪಟ್ಟಿ_deferred_locked(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);

	/*
	 * If in the middle of waking up a task, task_woken_scx() will be called
	 * afterwards which will then run the deferred actions, no need to
	 * schedule anything.
	 */
	ಆದರೆ (rq->scx.flags & SCX_RQ_ಒಳ_WAKEUP)
		ಹಿಂತಿರುಗಿ;

	/* Don't do anything if there already is a deferred operation. */
	ಆದರೆ (rq->scx.flags & SCX_RQ_BAL_CB_PENDING)
		ಹಿಂತಿರುಗಿ;

	/*
	 * If in dispatch, the balance callbacks will be called before rq lock
	 * is released. Schedule one.
	 *
	 *
	 * We can't directly insert the callback into the
	 * rq's list: The call can drop its lock and make the pending balance
	 * callback visible to unrelated code paths that call rq_pin_lock().
	 *
	 * Just let dispatch_one() know that it must do it itself.
	 */
	ಆದರೆ (rq->scx.flags & SCX_RQ_ಒಳ_DISPATCH) {
		rq->scx.flags |= SCX_RQ_BAL_CB_PENDING;
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * No scheduler hooks available. Use the generic irq_work path. The
	 * above WAKEUP and DISPATCH paths should cover most of the cases and
	 * the time to IRQ re-enable shouldn't be long.
	 */
	ವೇಳಾಪಟ್ಟಿ_deferred(rq);
}

ಶೂನ್ಯ scx_proxy_reenqueue_retry(ರಚನೆ rq *rq, ರಚನೆ task_struct *ಮುಂದಿನ)
{
}

ಶೂನ್ಯ ವೇಳಾಪಟ್ಟಿ_dsq_reenq(ರಚನೆ scx_sched *sch, ರಚನೆ scx_dispatch_q *dsq,
			u64 reenq_flags, ರಚನೆ rq *locked_rq)
{
	ರಚನೆ rq *rq;

	/*
	 * Allowing reenqueues doesn't make sense while bypassing. This also
	 * blocks from new reenqueues to be scheduled on dead scheds.
	 */
	ಆದರೆ (unlikely(READ_ONCE(sch->bypass_ಆಳ)))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (dsq->id == SCX_DSQ_LOCAL) {
		rq = container_ದ(dsq, ರಚನೆ rq, scx.local_dsq);

		/*
		 * A sub-sched lacking baseline access on the target cid has no
		 * business triggering IPIs. The lockless test is fine: slipping
		 * through right after a revoke is harmless and a wrong denial
		 * can't happen - if the caller has seen its ownership, so does
		 * this test.
		 */
		ಆದರೆ (unlikely(scx_missing_caps(sch, cpu_ದ(rq), SCX_CAP_ತಳ))) {
			__scx_add_event(sch, SCX_EV_SUB_REENQ_DENIED, 1);
			ಹಿಂತಿರುಗಿ;
		}

		ರಚನೆ scx_sched_pcpu *sch_pcpu = ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu_ದ(rq));
		ರಚನೆ scx_deferred_reenq_local *drl = &sch_pcpu->deferred_reenq_local;

		/*
		 * Pairs with smp_mb() in process_deferred_reenq_locals() and
		 * guarantees that there is a reenq_local() afterwards.
		 */
		smp_mb();

		ಆದರೆ (list_ಖಾಲಿಗಳು(&drl->ಗಂಟು) ||
		    (READ_ONCE(drl->flags) & reenq_flags) != reenq_flags) {

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ಬೀಗ);

			ಆದರೆ (list_ಖಾಲಿಗಳು(&drl->ಗಂಟು))
				list_move_ಬಾಲ(&drl->ಗಂಟು, &rq->scx.deferred_reenq_locals);
			WRITE_ONCE(drl->flags, drl->flags | reenq_flags);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(dsq->id & SCX_DSQ_FLAG_BUILTIN)) {
		rq = this_rq();

		ರಚನೆ scx_dsq_pcpu *dsq_pcpu = ಪ್ರತಿ_cpu_ptr(dsq->pcpu, cpu_ದ(rq));
		ರಚನೆ scx_deferred_reenq_user *dru = &dsq_pcpu->deferred_reenq_user;

		/*
		 * Pairs with smp_mb() in process_deferred_reenq_users() and
		 * guarantees that there is a reenq_user() afterwards.
		 */
		smp_mb();

		ಆದರೆ (list_ಖಾಲಿಗಳು(&dru->ಗಂಟು) ||
		    (READ_ONCE(dru->flags) & reenq_flags) != reenq_flags) {

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ಬೀಗ);

			/* @dsq is being destroyed, see free_dsq_irq_workfn() */
			ಆದರೆ (unlikely(READ_ONCE(dsq->id) == SCX_DSQ_INVALID))
				ಹಿಂತಿರುಗಿ;

			ಆದರೆ (list_ಖಾಲಿಗಳು(&dru->ಗಂಟು))
				list_move_ಬಾಲ(&dru->ಗಂಟು, &rq->scx.deferred_reenq_users);
			WRITE_ONCE(dru->flags, dru->flags | reenq_flags);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		scx_error(sch, "DSQ 0x%llx not allowed for reenq", dsq->id);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (rq == locked_rq)
		ವೇಳಾಪಟ್ಟಿ_deferred_locked(rq);
	ಇಲ್ಲದಿದ್ದರೆ
		ವೇಳಾಪಟ್ಟಿ_deferred(rq);
}

/*
 * p->scx.slice_oob packs an out-of-band slice request into one atomic64. A zero
 * word means no request. Otherwise the fields are:
 *
 *   63      SCX_SLICE_OOB_PENDING, set on every request
 *   62-43   lower bits of issuing scheduler's id
 *   42-0    requested slice duration in nsecs
 *
 * A duration of SCX_SLICE_OOB_DUR_MASK means SCX_SLICE_INF. A finite dur
 * saturates at SCX_SLICE_OOB_DUR_MASK - 1. The id is used to detect and ignore
 * a request that outlived a task ownership change.
 *
 * Only the low 20 bits of sch->id are packed, which is enough to make
 * collisions practically impossible. A theoretical collision just lets a stale
 * request through once.
 */
ಎಣಿಕೆ scx_ತುಂಡು_oob_consts {
	SCX_ತುಂಡು_OOB_DUR_ಬಿಟ್ಟುಗಳು	= 43,
	SCX_ತುಂಡು_OOB_ID_ಬಿಟ್ಟುಗಳು	= 64 - SCX_ತುಂಡು_OOB_DUR_ಬಿಟ್ಟುಗಳು - 1,

	SCX_ತುಂಡು_OOB_DUR_ಮುಖವಾಡ	= (1LLU << SCX_ತುಂಡು_OOB_DUR_ಬಿಟ್ಟುಗಳು) - 1,
	SCX_ತುಂಡು_OOB_ID_ಸರಿಸು	= SCX_ತುಂಡು_OOB_DUR_ಬಿಟ್ಟುಗಳು,
	SCX_ತುಂಡು_OOB_ID_ಮುಖವಾಡ	= (1LLU << SCX_ತುಂಡು_OOB_ID_ಬಿಟ್ಟುಗಳು) - 1,
	SCX_ತುಂಡು_OOB_PENDING	= 1LLU << 63,
};

/*
 * Slice and dsq_vtime write rules
 *
 * While @p is running, sleeping or queued on an rq-owned DSQ, both fields are
 * protected by the rq lock. While running, the rq lock is required because
 * update_curr_scx() RMWs the slice and the cap check for slice extension is
 * only reliable under the rq lock.
 *
 * While @p is queued on a user DSQ or on the BPF side, the kernel neither
 * consumes nor decides on the fields. Synchronizing the writers is the BPF
 * scheduler's responsibility. An rq-locked scx_bpf_task_set_slice() write and a
 * concurrent DSQ insertion commit can race each other and whichever lands last
 * wins.
 *
 * A DSQ insert kfunc doesn't update the fields directly. The verdict carries
 * the values and apply_slice_vtime() commits them at the insertion.
 *
 * scx_bpf_task_set_slice() may be called from any context and writes directly
 * only if @p's rq lock is already held, otherwise it bounces through
 * p->scx.slice_oob, applied under @p's rq lock at the next slice consideration.
 *
 * While %SCX_TASK_PROTECTED is set, every scheduler-reachable slice update is
 * refused. See set_task_slice_keep_oob().
 *
 * dsq_vtime orders the next PRIQ insertion and has no running-side consumer, so
 * scx_bpf_task_set_dsq_vtime() writes it directly. Fork-time init and direct
 * BPF stores from non-cid-form schedulers are outside these rules.
 */

/* clear a pending slice request */
ಸ್ಥಿರ ಶೂನ್ಯ clear_task_ತುಂಡು_oob(ರಚನೆ task_struct *p)
{
	ಆದರೆ (unlikely(atomic64_read(&p->scx.ತುಂಡು_oob)))
		atomic64_ಗಣ(&p->scx.ತುಂಡು_oob, 0);
}

/**
 * dsq_insert_head - FIFO head insertion honoring %SCX_TASK_PROTECTED
 * @dsq: DSQ to insert into
 * @p: task being inserted
 *
 * A HEAD insert should land behind any leading protected tasks. Return %true
 * indicates whether @p became the first entry.
 */
ಸ್ಥಿರ bool dsq_ಸೇರಿಸು_ತಲೆ(ರಚನೆ scx_dispatch_q *dsq, ರಚನೆ task_struct *p)
{
	ರಚನೆ list_ತಲೆ *pos = &dsq->list;
	ರಚನೆ scx_dsq_list_ಗಂಟು *ಗಂಟು;

	/*
	 * Only rq-owned DSQs can hold protected tasks and the associated rq
	 * lock keeps their flags stable.
	 */
	ಆದರೆ (!dsq_is_rq_owned(dsq)) {
		list_ಕೂಡಿಸು(&p->scx.dsq_list.ಗಂಟು, &dsq->list);
		ಹಿಂತಿರುಗಿ true;
	}

	list_for_each_ನಮೂದು(ಗಂಟು, &dsq->list, ಗಂಟು) {
		ರಚನೆ task_struct *q;

		ಆದರೆ (WARN_ON_ONCE(ಗಂಟು->flags & SCX_DSQ_LNODE_ITER_CURSOR))
			ಮುಂದುವರಿ;

		q = container_ದ(ಗಂಟು, ರಚನೆ task_struct, scx.dsq_list);
		ಆದರೆ (!(q->scx.flags & SCX_TASK_PROTECTED))
			ನಿಲ್ಲಿಸು;

		pos = &ಗಂಟು->ಗಂಟು;
	}

	list_ಕೂಡಿಸು(&p->scx.dsq_list.ಗಂಟು, pos);

	ಹಿಂತಿರುಗಿ pos == &dsq->list;
}

/**
 * set_task_slice_keep_oob - Set @p's slice, leaving any pending oob request
 * @p: task of interest
 * @slice: slice to set
 *
 * While %SCX_TASK_PROTECTED is set, BPF schedulers may not modify the slice.
 * Refuse and return %false.
 */
ಸ್ಥಿರ bool ಗಣ_task_ತುಂಡು_keep_oob(ರಚನೆ task_struct *p, u64 ತುಂಡು)
{
	lockdep_assert_rq_held(task_rq(p));

	ಆದರೆ (unlikely(p->scx.flags & SCX_TASK_PROTECTED))
		ಹಿಂತಿರುಗಿ false;

	p->scx.ತುಂಡು = ತುಂಡು;
	ಹಿಂತಿರುಗಿ true;
}

/* set @p's slice, superseding any pending out-of-band request */
bool scx_ಗಣ_task_ತುಂಡು(ರಚನೆ task_struct *p, u64 ತುಂಡು)
{
	ಆದರೆ (!ಗಣ_task_ತುಂಡು_keep_oob(p, ತುಂಡು))
		ಹಿಂತಿರುಗಿ false;
	clear_task_ತುಂಡು_oob(p);
	ಹಿಂತಿರುಗಿ true;
}

/**
 * scx_task_slice_ended - @p's slice is consumed or given up
 * @rq: rq @p is on
 * @p: task of interest
 *
 * End what rides on the slice - the protection, and the rescue if @p is being
 * rescued.
 *
 * A dequeue normally ends the slice too. The exception is a save/restore pair
 * on the running task. Attribute changes like renice cycle the task through
 * dequeue and enqueue while it keeps executing, so the slice continues. A
 * queued task instead loses its DSQ position on any dequeue and the slice ends
 * with it.
 */
ಶೂನ್ಯ scx_task_ತುಂಡು_ended(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	lockdep_assert_rq_held(rq);

	p->scx.flags &= ~SCX_TASK_PROTECTED;
	ಆದರೆ (unlikely(p == scx_rescuee(rq)))
		scx_rescue_ಕೊನೆ(rq);
}

/* request @p's slice to be set to @slice, see the write rules above */
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_task_ತುಂಡು_oob(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p, u64 ತುಂಡು)
{
	u64 dur;

	ಆದರೆ (ತುಂಡು == SCX_ತುಂಡು_INF) {
		dur = SCX_ತುಂಡು_OOB_DUR_ಮುಖವಾಡ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(ತುಂಡು >= SCX_ತುಂಡು_OOB_DUR_ಮುಖವಾಡ)) {
		dur = SCX_ತುಂಡು_OOB_DUR_ಮುಖವಾಡ - 1;
		scx_ಕೂಡಿಸು_event(sch, SCX_EV_ತುಂಡು_CLAMPED, 1);
	} ಇಲ್ಲದಿದ್ದರೆ {
		dur = ತುಂಡು;
	}

	atomic64_ಗಣ(&p->scx.ತುಂಡು_oob, SCX_ತುಂಡು_OOB_PENDING |
		     ((sch->id & SCX_ತುಂಡು_OOB_ID_ಮುಖವಾಡ) << SCX_ತುಂಡು_OOB_ID_ಸರಿಸು) | dur);
}

/*
 * Apply a pending out-of-band slice request under @rq's lock. A request whose
 * packed id no longer matches @p's current owner is dropped. An extension needs
 * baseline cpu access on @p's cid, shortening is always allowed, and a
 * protected slice refuses both. %SCX_EV_SLICE_DENIED counts the denials. See
 * the write rules above.
 */
ಸ್ಥಿರ ಶೂನ್ಯ apply_task_ತುಂಡು_oob(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	u64 oob, dur, ತುಂಡು;

	lockdep_assert_rq_held(rq);

	ಆದರೆ (likely(!atomic64_read(&p->scx.ತುಂಡು_oob)))
		ಹಿಂತಿರುಗಿ;

	oob = atomic64_xchg(&p->scx.ತುಂಡು_oob, 0);
	ಆದರೆ (unlikely(!oob))
		ಹಿಂತಿರುಗಿ;

	/* the issuing scheduler no longer owns @p, drop the request */
	ಆದರೆ (unlikely(((oob >> SCX_ತುಂಡು_OOB_ID_ಸರಿಸು) & SCX_ತುಂಡು_OOB_ID_ಮುಖವಾಡ) !=
		     (scx_task_sched(p)->id & SCX_ತುಂಡು_OOB_ID_ಮುಖವಾಡ)))
		ಹಿಂತಿರುಗಿ;

	dur = oob & SCX_ತುಂಡು_OOB_DUR_ಮುಖವಾಡ;
	ತುಂಡು = dur == SCX_ತುಂಡು_OOB_DUR_ಮುಖವಾಡ ? SCX_ತುಂಡು_INF : dur;

	ಆದರೆ (ತುಂಡು > p->scx.ತುಂಡು &&
	    unlikely(scx_missing_caps(scx_task_sched(p), cpu_ದ(rq), SCX_CAP_ತಳ))) {
		__scx_add_event(scx_task_sched(p), SCX_EV_ತುಂಡು_DENIED, 1);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (unlikely(!ಗಣ_task_ತುಂಡು_keep_oob(p, ತುಂಡು)))
		__scx_add_event(scx_task_sched(p), SCX_EV_ತುಂಡು_DENIED, 1);
}

/*
 * A dsq insert kfunc doesn't write slice or dsq_vtime. The verdict carries them
 * and they are committed here, at the insertion. A zero @slice keeps the
 * current value, floored at 1 so the task isn't treated as expired.
 */
ಸ್ಥಿರ ಶೂನ್ಯ apply_ತುಂಡು_vtime(ರಚನೆ task_struct *p, u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	ಆದರೆ (ತುಂಡು) {
		p->scx.ತುಂಡು = ತುಂಡು;
		/*
		 * An explicit slice supersedes a pending oob request. A carried
		 * default refill is not an explicit request and must keep it.
		 */
		ಆದರೆ (!(enq_flags & SCX_ENQ_ತುಂಡು_DFL))
			clear_task_ತುಂಡು_oob(p);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!p->scx.ತುಂಡು) {
		p->scx.ತುಂಡು = 1;
	}

	ಆದರೆ (enq_flags & SCX_ENQ_DSQ_PRIQ)
		p->scx.dsq_vtime = vtime;
}

ಸ್ಥಿರ ಶೂನ್ಯ update_curr_scx(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *curr = rq->curr;
	s64 delta_exec;

	/* apply even on 0 delta_exec, callers may still act on the slice */
	apply_task_ತುಂಡು_oob(rq, curr);

	delta_exec = update_curr_common(rq);
	ಆದರೆ (unlikely(delta_exec <= 0))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (curr->scx.ತುಂಡು != SCX_ತುಂಡು_INF)
		curr->scx.ತುಂಡು -= min_t(u64, curr->scx.ತುಂಡು, delta_exec);

	ಆದರೆ (unlikely(curr == scx_rescuee(rq)))
		scx_rescue_charge(rq, delta_exec);

	dl_server_update(&rq->ext_server, delta_exec);
}

ಸ್ಥಿರ bool scx_dsq_priq_less(ರಚನೆ rb_ಗಂಟು *ಗಂಟು_a,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಗಂಟು *ಗಂಟು_b)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *a =
		container_ದ(ಗಂಟು_a, ರಚನೆ task_struct, scx.dsq_priq);
	ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *b =
		container_ದ(ಗಂಟು_b, ರಚನೆ task_struct, scx.dsq_priq);

	ಹಿಂತಿರುಗಿ ಸಮಯ_before64(a->scx.dsq_vtime, b->scx.dsq_vtime);
}

ಸ್ಥಿರ ಶೂನ್ಯ dsq_inc_nr(ರಚನೆ scx_dispatch_q *dsq, ರಚನೆ task_struct *p, u64 enq_flags)
{
	/* scx_bpf_dsq_nr_queued() reads ->nr without locking, use WRITE_ONCE() */
	WRITE_ONCE(dsq->nr, dsq->nr + 1);

	/*
	 * Once @p reaches a local DSQ, it can only leave it by being dispatched
	 * to the CPU or dequeued. In both cases, the only way @p can go back to
	 * the BPF sched is through enqueueing. If being inserted into a local
	 * DSQ with IMMED, persist the state until the next enqueueing event in
	 * scx_do_enqueue_task() so that we can maintain IMMED protection
	 * through e.g. SAVE/RESTORE cycles and slice extensions.
	 */
	ಆದರೆ (enq_flags & SCX_ENQ_IMMED) {
		ಆದರೆ (unlikely(dsq->id != SCX_DSQ_LOCAL)) {
			WARN_ON_ONCE(!(enq_flags & SCX_ENQ_GDSQ_FALLBACK));
			ಹಿಂತಿರುಗಿ;
		}
		p->scx.flags |= SCX_TASK_IMMED;
	}

	ಆದರೆ (p->scx.flags & SCX_TASK_IMMED) {
		ರಚನೆ rq *rq = container_ದ(dsq, ರಚನೆ rq, scx.local_dsq);

		ಆದರೆ (WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL))
			ಹಿಂತಿರುಗಿ;

		rq->scx.nr_immed++;

		/*
		 * If @rq already had other tasks or the current task is not
		 * done yet, @p can't go on the CPU immediately. Re-enqueue.
		 */
		ಆದರೆ (unlikely(dsq->nr > 1 || !rq_is_open(rq, enq_flags)))
			scx_ವೇಳಾಪಟ್ಟಿ_reenq_local(rq, 0);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ dsq_dec_nr(ರಚನೆ scx_dispatch_q *dsq, ರಚನೆ task_struct *p)
{
	/* see dsq_inc_nr() */
	WRITE_ONCE(dsq->nr, dsq->nr - 1);

	ಆದರೆ (p->scx.flags & SCX_TASK_IMMED) {
		ರಚನೆ rq *rq = container_ದ(dsq, ರಚನೆ rq, scx.local_dsq);

		ಆದರೆ (WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL) ||
		    WARN_ON_ONCE(rq->scx.nr_immed <= 0))
			ಹಿಂತಿರುಗಿ;

		rq->scx.nr_immed--;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ refill_task_ತುಂಡು_dfl(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p)
{
	/*
	 * A default refill is not an explicit request, so it must not drop a
	 * pending out-of-band one, which is applied when @p next runs.
	 */
	ಗಣ_task_ತುಂಡು_keep_oob(p, READ_ONCE(sch->ತುಂಡು_dfl));
	__scx_add_event(sch, SCX_EV_REFILL_ತುಂಡು_DFL, 1);
}

/*
 * Return true if @p is moving due to an internal SCX migration, false
 * otherwise.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_scx_migrating(ರಚನೆ task_struct *p)
{
	/*
	 * We only need to check sticky_cpu: it is set to the destination
	 * CPU in move_remote_task_to_local_dsq() before deactivate_task()
	 * and cleared when the task is enqueued on the destination, so it
	 * is only non-negative during an internal SCX migration.
	 */
	ಹಿಂತಿರುಗಿ p->scx.sticky_cpu >= 0;
}

/* Must be called under the lock serializing @p's custody transfers. */
ಸ್ಥಿರ bool task_leave_custody(ರಚನೆ task_struct *p)
{
	ಆದರೆ (!(p->scx.flags & SCX_TASK_ಒಳ_CUSTODY) || task_scx_migrating(p))
		ಹಿಂತಿರುಗಿ false;

	p->scx.flags &= ~SCX_TASK_ಒಳ_CUSTODY;
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ rq_owned_post_enq(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq,
			      ರಚನೆ scx_dispatch_q *dsq, ರಚನೆ task_struct *p,
			      u64 enq_flags)
{
	ಆದರೆ (task_leave_custody(p) && SCX_HAS_OP(sch, ಸರತಿಯಿಂದ_ತೆಗೆ))
		SCX_CALL_OP_TASK(sch, ಸರತಿಯಿಂದ_ತೆಗೆ, rq, p, 0);

	/*
	 * Only local inserts get the wakeup treatment below. Rejects kick the
	 * deferred reenq and rescue parks are paced by the rescue timer.
	 */
	ಆದರೆ (unlikely(dsq->id != SCX_DSQ_LOCAL)) {
		ಆದರೆ (dsq->id == SCX_DSQ_REJECT)
			ವೇಳಾಪಟ್ಟಿ_deferred_locked(rq);
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * Note that @rq's lock may be dropped between this enqueue and @p
	 * actually getting on CPU. This gives higher-class tasks (e.g. RT)
	 * an opportunity to wake up on @rq and prevent @p from running.
	 * Here are some concrete examples:
	 *
	 * Example 1:
	 *
	 * We dispatch two tasks from a single ops.dispatch():
	 * - First, a local task to this CPU's local DSQ;
	 * - Second, a local/remote task to a remote CPU's local DSQ.
	 * We must drop the local rq lock in order to finish the second
	 * dispatch. In that time, an RT task can wake up on the local rq.
	 *
	 * Example 2:
	 *
	 * We dispatch a local/remote task to a remote CPU's local DSQ.
	 * We must drop the remote rq lock before the dispatched task can run,
	 * which gives an RT task an opportunity to wake up on the remote rq.
	 *
	 * Both examples work the same if we replace dispatching with moving
	 * the tasks from a user-created DSQ.
	 *
	 * We must detect these wakeups so that we can re-enqueue IMMED tasks
	 * from @rq's local DSQ. scx_wakeup_preempt() serves exactly this
	 * purpose, but for it to be invoked, we must ensure that we bump
	 * @rq->next_class to &ext_sched_class if it's currently idle.
	 *
	 * wakeup_preempt() does the bumping, and since we only invoke it if
	 * @rq->next_class is below &ext_sched_class, it will also
	 * resched_curr(rq).
	 */
	ಆದರೆ (sched_class_above(p->sched_class, rq->ಮುಂದಿನ_class))
		wakeup_preempt(rq, p, 0);

	/*
	 * If @rq is in dispatch, the CPU is already vacant and looking for the
	 * next task to run. No need to preempt or trigger resched after moving
	 * @p into its local DSQ.
	 * Note that the wakeup_preempt() above may have already triggered
	 * a resched if @rq->next_class was idle. It's harmless, since
	 * need_resched is cleared immediately after task pick.
	 */
	ಆದರೆ (rq->scx.flags & SCX_RQ_ಒಳ_DISPATCH)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ ((enq_flags & SCX_ENQ_PREEMPT) && p != rq->curr &&
	    rq->curr->sched_class == &ext_sched_class) {
		ಆದರೆ (likely(scx_ಗಣ_task_ತುಂಡು(rq->curr, 0)))
			resched_curr(rq);
		ಇಲ್ಲದಿದ್ದರೆ
			__scx_add_event(sch, SCX_EV_ತುಂಡು_DENIED, 1);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq,
				 ರಚನೆ scx_dispatch_q *dsq, ರಚನೆ task_struct *p,
				 u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	bool is_rq_owned = false;

	ಆದರೆ (dsq->id == SCX_DSQ_LOCAL) {
		dsq = scx_resolve_local_dsq(sch, rq, p, &enq_flags);
		is_rq_owned = true;
	}

	WARN_ON_ONCE(p->scx.dsq || !list_ಖಾಲಿಗಳು(&p->scx.dsq_list.ಗಂಟು));
	WARN_ON_ONCE((p->scx.dsq_flags & SCX_TASK_DSQ_ON_PRIQ) ||
		     !RB_ಖಾಲಿಗಳು_ಗಂಟು(&p->scx.dsq_priq));

	ಆದರೆ (!is_rq_owned) {
		raw_spin_ಬೀಗ_nested(&dsq->ಬೀಗ,
			(enq_flags & SCX_ENQ_NESTED) ? SINGLE_ಆಳ_NESTING : 0);

		ಆದರೆ (unlikely(dsq->id == SCX_DSQ_INVALID)) {
			scx_error(sch, "attempting to dispatch to a destroyed dsq");
			/* fall back to the global dsq */
			raw_spin_unlock(&dsq->ಬೀಗ);
			dsq = ಕಂಡುಹಿಡಿ_global_dsq(sch, task_cpu(p));
			raw_spin_ಬೀಗ(&dsq->ಬೀಗ);
		}
	}

	ಆದರೆ (unlikely((dsq->id & SCX_DSQ_FLAG_BUILTIN) &&
		     (enq_flags & SCX_ENQ_DSQ_PRIQ))) {
		/*
		 * SCX_DSQ_LOCAL and SCX_DSQ_GLOBAL DSQs always consume from
		 * their FIFO queues. To avoid confusion and accidentally
		 * starving vtime-dispatched tasks by FIFO-dispatched tasks, we
		 * disallow any internal DSQ from doing vtime ordering of
		 * tasks.
		 */
		scx_error(sch, "cannot use vtime ordering for built-in DSQs");
		enq_flags &= ~SCX_ENQ_DSQ_PRIQ;
	}

	/*
	 * @dsq is locked and @enq_flags is sanitized. Commit the carried slice
	 * and vtime before the PRIQ insertion below reads the new dsq_vtime.
	 */
	ಆದರೆ (enq_flags & SCX_ENQ_APPLY_ತುಂಡು)
		apply_ತುಂಡು_vtime(p, ತುಂಡು, vtime, enq_flags);

	ಆದರೆ (enq_flags & SCX_ENQ_DSQ_PRIQ) {
		ರಚನೆ rb_ಗಂಟು *rbp;

		/*
		 * A PRIQ DSQ shouldn't be using FIFO enqueueing. As tasks are
		 * linked to both the rbtree and list on PRIQs, this can only be
		 * tested easily when adding the first task.
		 */
		ಆದರೆ (unlikely(RB_ಖಾಲಿಗಳು_ಬೇರು(&dsq->priq) &&
			     nldsq_ಮುಂದಿನ_task(dsq, ಖಾಲಿ, false)))
			scx_error(sch, "DSQ ID 0x%016llx already had FIFO-enqueued tasks",
				  dsq->id);

		p->scx.dsq_flags |= SCX_TASK_DSQ_ON_PRIQ;
		rb_ಕೂಡಿಸು(&p->scx.dsq_priq, &dsq->priq, scx_dsq_priq_less);

		/*
		 * Find the previous task and insert after it on the list so
		 * that @dsq->list is vtime ordered.
		 */
		rbp = rb_prev(&p->scx.dsq_priq);
		ಆದರೆ (rbp) {
			ರಚನೆ task_struct *prev =
				container_ದ(rbp, ರಚನೆ task_struct,
					     scx.dsq_priq);
			list_ಕೂಡಿಸು(&p->scx.dsq_list.ಗಂಟು, &prev->scx.dsq_list.ಗಂಟು);
			/* first task unchanged - no update needed */
		} ಇಲ್ಲದಿದ್ದರೆ {
			list_ಕೂಡಿಸು(&p->scx.dsq_list.ಗಂಟು, &dsq->list);
			/* not builtin and new task is at head - use fastpath */
			rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(dsq->ಮೊದಲ_task, p);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* a FIFO DSQ shouldn't be using PRIQ enqueuing */
		ಆದರೆ (unlikely(!RB_ಖಾಲಿಗಳು_ಬೇರು(&dsq->priq)))
			scx_error(sch, "DSQ ID 0x%016llx already had PRIQ-enqueued tasks",
				  dsq->id);

		ಆದರೆ (enq_flags & (SCX_ENQ_ತಲೆ | SCX_ENQ_PREEMPT)) {
			/* new task inserted at head - use fastpath */
			ಆದರೆ (dsq_ಸೇರಿಸು_ತಲೆ(dsq, p) && !(dsq->id & SCX_DSQ_FLAG_BUILTIN))
				rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(dsq->ಮೊದಲ_task, p);
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * dsq->list can contain parked BPF iterator cursors, so
			 * list_empty() here isn't a reliable proxy for "no real
			 * task in the DSQ". Test dsq->first_task directly.
			 */
			list_ಕೂಡಿಸು_ಬಾಲ(&p->scx.dsq_list.ಗಂಟು, &dsq->list);
			ಆದರೆ (!dsq->ಮೊದಲ_task && !(dsq->id & SCX_DSQ_FLAG_BUILTIN))
				rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(dsq->ಮೊದಲ_task, p);
		}
	}

	/* seq records the order tasks are queued, used by BPF DSQ iterator */
	WRITE_ONCE(dsq->seq, dsq->seq + 1);
	p->scx.dsq_seq = dsq->seq;

	dsq_inc_nr(dsq, p, enq_flags);
	p->scx.dsq = dsq;

	/*
	 * Update custody and call ops.dequeue() before clearing ops_state:
	 * once ops_state is cleared, waiters in ops_dequeue() can proceed
	 * and dequeue_task_scx() will RMW p->scx.flags. If we clear
	 * ops_state first, both sides would modify p->scx.flags
	 * concurrently in a non-atomic way.
	 */
	ಆದರೆ (is_rq_owned) {
		rq_owned_post_enq(sch, rq, dsq, p, enq_flags);
	} ಇಲ್ಲದಿದ್ದರೆ {
		bool call_ಸರತಿಯಿಂದ_ತೆಗೆ = false;

		/*
		 * Global and bypass DSQs are terminal - the task leaves the
		 * scheduler's custody, so ops.dequeue() fires. It can run
		 * without @p's rq lock (finish_dispatch() passes the dispatch
		 * rq); that's safe because dequeue_task_scx() waits on
		 * SCX_OPSS_DISPATCHING (see the ops_state note above) and so
		 * can't race it. A non-terminal DSQ keeps the task in custody.
		 * The custody transfer happens under @dsq->lock so that later
		 * consumers see the flag clear; the callback runs after
		 * @dsq->lock is dropped because it may lock a DSQ itself.
		 */
		ಆದರೆ (dsq->id == SCX_DSQ_GLOBAL || dsq->id == SCX_DSQ_BYPASS)
			call_ಸರತಿಯಿಂದ_ತೆಗೆ = task_leave_custody(p);
		ಇಲ್ಲದಿದ್ದರೆ
			p->scx.flags |= SCX_TASK_ಒಳ_CUSTODY;

		raw_spin_unlock(&dsq->ಬೀಗ);

		ಆದರೆ (call_ಸರತಿಯಿಂದ_ತೆಗೆ && SCX_HAS_OP(sch, ಸರತಿಯಿಂದ_ತೆಗೆ))
			SCX_CALL_OP_TASK(sch, ಸರತಿಯಿಂದ_ತೆಗೆ, rq, p, 0);
	}

	/*
	 * We're transitioning out of QUEUEING or DISPATCHING. store_release to
	 * match waiters' load_acquire.
	 */
	ಆದರೆ (enq_flags & SCX_ENQ_CLEAR_OPSS)
		atomic_long_ಗಣ_release(&p->scx.ops_state, SCX_OPSS_NONE);
}

ಶೂನ್ಯ scx_task_unlink_from_dsq(ರಚನೆ task_struct *p, ರಚನೆ scx_dispatch_q *dsq)
{
	WARN_ON_ONCE(list_ಖಾಲಿಗಳು(&p->scx.dsq_list.ಗಂಟು));

	ಆದರೆ (p->scx.dsq_flags & SCX_TASK_DSQ_ON_PRIQ) {
		rb_erase(&p->scx.dsq_priq, &dsq->priq);
		RB_CLEAR_ಗಂಟು(&p->scx.dsq_priq);
		p->scx.dsq_flags &= ~SCX_TASK_DSQ_ON_PRIQ;
	}

	list_del_init(&p->scx.dsq_list.ಗಂಟು);
	dsq_dec_nr(dsq, p);

	ಆದರೆ (!(dsq->id & SCX_DSQ_FLAG_BUILTIN) && rcu_access_ಸೂಚಕ(dsq->ಮೊದಲ_task) == p) {
		ರಚನೆ task_struct *ಮೊದಲ_task;

		ಮೊದಲ_task = nldsq_ಮುಂದಿನ_task(dsq, ಖಾಲಿ, false);
		rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(dsq->ಮೊದಲ_task, ಮೊದಲ_task);
	}
}

ಶೂನ್ಯ scx_dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ scx_dispatch_q *dsq = p->scx.dsq;
	bool is_rq_owned = dsq && dsq_is_rq_owned(dsq);

	lockdep_assert_rq_held(rq);

	ಆದರೆ (!dsq) {
		/*
		 * If !dsq && on-list, @p is on @rq's ddsp_deferred_locals.
		 * Unlinking is all that's needed to cancel.
		 */
		ಆದರೆ (unlikely(!list_ಖಾಲಿಗಳು(&p->scx.dsq_list.ಗಂಟು)))
			list_del_init(&p->scx.dsq_list.ಗಂಟು);

		/*
		 * When dispatching directly from the BPF scheduler to a local
		 * DSQ, the task isn't associated with any DSQ but
		 * @p->scx.holding_cpu may be set under the protection of
		 * %SCX_OPSS_DISPATCHING.
		 */
		ಆದರೆ (p->scx.holding_cpu >= 0)
			p->scx.holding_cpu = -1;

		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!is_rq_owned)
		raw_spin_ಬೀಗ(&dsq->ಬೀಗ);

	/*
	 * Now that we hold @dsq->lock, @p->holding_cpu and @p->scx.dsq_* can't
	 * change underneath us.
	*/
	ಆದರೆ (p->scx.holding_cpu < 0) {
		/* @p must still be on @dsq, dequeue */
		scx_task_unlink_from_dsq(p, dsq);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * We're racing against dispatch_to_local_dsq() which already
		 * removed @p from @dsq and set @p->scx.holding_cpu. Clear the
		 * holding_cpu which tells dispatch_to_local_dsq() that it lost
		 * the race.
		 */
		WARN_ON_ONCE(!list_ಖಾಲಿಗಳು(&p->scx.dsq_list.ಗಂಟು));
		p->scx.holding_cpu = -1;
	}
	p->scx.dsq = ಖಾಲಿ;

	ಆದರೆ (!is_rq_owned)
		raw_spin_unlock(&dsq->ಬೀಗ);
}

/*
 * Abbreviated version of scx_dispatch_dequeue() that can be used when both
 * @p's rq and dsq are locked.
 */
ಸ್ಥಿರ ಶೂನ್ಯ dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ_locked(ರಚನೆ task_struct *p,
				    ರಚನೆ scx_dispatch_q *dsq)
{
	lockdep_assert_rq_held(task_rq(p));
	lockdep_assert_held(&dsq->ಬೀಗ);

	scx_task_unlink_from_dsq(p, dsq);
	p->scx.dsq = ಖಾಲಿ;
}

ಸ್ಥಿರ ರಚನೆ scx_dispatch_q *ಕಂಡುಹಿಡಿ_dsq_for_dispatch(ರಚನೆ scx_sched *sch,
						    ರಚನೆ rq *rq, u64 dsq_id,
						    s32 tcpu)
{
	ರಚನೆ scx_dispatch_q *dsq;

	ಆದರೆ (dsq_id == SCX_DSQ_LOCAL)
		ಹಿಂತಿರುಗಿ &rq->scx.local_dsq;

	ಆದರೆ ((dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON) {
		s32 cpu = scx_cpu_ret(sch, dsq_id & SCX_DSQ_LOCAL_CPU_ಮುಖವಾಡ);

		ಆದರೆ (!scx_cpu_valid(sch, cpu, "in SCX_DSQ_LOCAL_ON dispatch verdict"))
			ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_global_dsq(sch, tcpu);

		ಹಿಂತಿರುಗಿ &cpu_rq(cpu)->scx.local_dsq;
	}

	ಆದರೆ (dsq_id == SCX_DSQ_GLOBAL)
		dsq = ಕಂಡುಹಿಡಿ_global_dsq(sch, tcpu);
	ಇಲ್ಲದಿದ್ದರೆ
		dsq = ಕಂಡುಹಿಡಿ_user_dsq(sch, dsq_id);

	/*
	 * Built-in DSQs are never inserted into dsq_hash, so REJECT and RESCUE
	 * hit the error below. They cannot be reached with an ID.
	 */
	ಆದರೆ (unlikely(!dsq)) {
		scx_error(sch, "non-existent DSQ 0x%llx", dsq_id);
		ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_global_dsq(sch, tcpu);
	}

	ಹಿಂತಿರುಗಿ dsq;
}

ಸ್ಥಿರ ಶೂನ್ಯ mark_ನೇರ_dispatch(ರಚನೆ scx_sched *sch,
				 ರಚನೆ task_struct *ddsp_task,
				 ರಚನೆ task_struct *p, u64 dsq_id,
				 u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	/*
	 * Mark that dispatch already happened from ops.select_cpu() or
	 * ops.enqueue() by spoiling direct_dispatch_task with a non-NULL value
	 * which can never match a valid task pointer.
	 */
	__this_cpu_write(ನೇರ_dispatch_task, ERR_PTR(-ESRCH));

	/* @p must match the task on the enqueue path */
	ಆದರೆ (unlikely(p != ddsp_task)) {
		ಆದರೆ (IS_ERR(ddsp_task))
			scx_error(sch, "%s[%d] already direct-dispatched",
				  p->comm, p->pid);
		ಇಲ್ಲದಿದ್ದರೆ
			scx_error(sch, "scheduling for %s[%d] but trying to direct-dispatch %s[%d]",
				  ddsp_task->comm, ddsp_task->pid,
				  p->comm, p->pid);
		ಹಿಂತಿರುಗಿ;
	}

	WARN_ON_ONCE(p->scx.ddsp_dsq_id != SCX_DSQ_INVALID);
	WARN_ON_ONCE(p->scx.ddsp_enq_flags);

	p->scx.ddsp_ತುಂಡು = ತುಂಡು;
	p->scx.ddsp_vtime = vtime;
	p->scx.ddsp_dsq_id = dsq_id;
	p->scx.ddsp_enq_flags = enq_flags;
}

/*
 * Clear @p direct dispatch state when leaving the scheduler.
 *
 * Direct dispatch state must be cleared in the following cases:
 *  - direct_dispatch(): cleared on the synchronous enqueue path, deferred
 *    dispatch keeps the state until consumed
 *  - process_ddsp_deferred_locals(): cleared after consuming deferred state,
 *  - scx_do_enqueue_task(): cleared on enqueue fallbacks where the dispatch
 *    verdict is ignored (local/global/bypass)
 *  - dequeue_task_scx(): cleared after scx_dispatch_dequeue(), covering
 *    deferred cancellation and holding_cpu races
 *  - scx_disable_task(): cleared for queued wakeup tasks, which are excluded by
 *    the scx_bypass() loop, so that stale state is not reused by a subsequent
 *    scheduler instance
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ clear_ನೇರ_dispatch(ರಚನೆ task_struct *p)
{
	p->scx.ddsp_dsq_id = SCX_DSQ_INVALID;
	p->scx.ddsp_enq_flags = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ನೇರ_dispatch(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p,
			    u64 enq_flags)
{
	ರಚನೆ rq *rq = task_rq(p);
	ರಚನೆ scx_dispatch_q *dsq =
		ಕಂಡುಹಿಡಿ_dsq_for_dispatch(sch, rq, p->scx.ddsp_dsq_id, task_cpu(p));
	u64 ddsp_enq_flags, ತುಂಡು, vtime;

	p->scx.ddsp_enq_flags |= enq_flags;

	/*
	 * We are in the enqueue path with @rq locked and pinned, and thus can't
	 * double lock a remote rq and enqueue to its local DSQ. For
	 * DSQ_LOCAL_ON verdicts targeting the local DSQ of a remote CPU, defer
	 * the enqueue so that it's executed when @rq can be unlocked.
	 */
	ಆದರೆ (dsq->id == SCX_DSQ_LOCAL && dsq != &rq->scx.local_dsq) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ opss;

		opss = atomic_long_read(&p->scx.ops_state) & SCX_OPSS_STATE_ಮುಖವಾಡ;

		ಆಯ್ಕೆ (opss & SCX_OPSS_STATE_ಮುಖವಾಡ) {
		ಸ್ಥಿತಿ SCX_OPSS_NONE:
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ SCX_OPSS_QUEUEING:
			/*
			 * As @p was never passed to the BPF side, _release is
			 * not strictly necessary. Still do it for consistency.
			 */
			atomic_long_ಗಣ_release(&p->scx.ops_state, SCX_OPSS_NONE);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			WARN_ONCE(true, "sched_ext: %s[%d] has invalid ops state 0x%lx in direct_dispatch()",
				  p->comm, p->pid, opss);
			atomic_long_ಗಣ_release(&p->scx.ops_state, SCX_OPSS_NONE);
			ನಿಲ್ಲಿಸು;
		}

		WARN_ON_ONCE(p->scx.dsq || !list_ಖಾಲಿಗಳು(&p->scx.dsq_list.ಗಂಟು));
		list_ಕೂಡಿಸು_ಬಾಲ(&p->scx.dsq_list.ಗಂಟು,
			      &rq->scx.ddsp_deferred_locals);
		ವೇಳಾಪಟ್ಟಿ_deferred_locked(rq);
		ಹಿಂತಿರುಗಿ;
	}

	ddsp_enq_flags = p->scx.ddsp_enq_flags;
	ತುಂಡು = p->scx.ddsp_ತುಂಡು;
	vtime = p->scx.ddsp_vtime;
	clear_ನೇರ_dispatch(p);

	scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, rq, dsq, p, ತುಂಡು, vtime,
			     ddsp_enq_flags | SCX_ENQ_APPLY_ತುಂಡು | SCX_ENQ_CLEAR_OPSS);
}

bool scx_rq_online(ರಚನೆ rq *rq)
{
	/*
	 * Test both cpu_active() and %SCX_RQ_ONLINE. %SCX_RQ_ONLINE indicates
	 * the online state as seen from the BPF scheduler. cpu_active() test
	 * guarantees that, if this function returns %true, %SCX_RQ_ONLINE will
	 * stay set until the current scheduling operation is complete even if
	 * we aren't locking @rq.
	 */
	ಹಿಂತಿರುಗಿ likely((rq->scx.flags & SCX_RQ_ONLINE) && cpu_ಸಕ್ರಿಯ(cpu_ದ(rq)));
}

ಶೂನ್ಯ scx_do_ಸರತಿಗೆ_ಸೇರಿಸು_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 enq_flags,
			 ಪೂರ್ಣಾಂಕ sticky_cpu)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);
	ರಚನೆ task_struct **ddsp_taskp;
	ರಚನೆ scx_dispatch_q *dsq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ qseq;

	WARN_ON_ONCE(!(p->scx.flags & SCX_TASK_QUEUED));

	/* internal movements - rq migration / RESTORE */
	ಆದರೆ (sticky_cpu == cpu_ದ(rq))
		ಹೋಗು local_norefill;

	/*
	 * Clear persistent TASK_IMMED for fresh enqueues, see dsq_inc_nr().
	 * Note that exiting and migration-disabled tasks that skip
	 * ops.enqueue() below will lose IMMED protection unless
	 * %SCX_OPS_ENQ_EXITING / %SCX_OPS_ENQ_MIGRATION_DISABLED are set.
	 */
	p->scx.flags &= ~SCX_TASK_IMMED;

	/*
	 * A task reenqueued too many times without running means the scheduler
	 * keeps re-deciding a placement it can't honor, e.g. re-inserting to a
	 * cid it lacks caps on. Eject the owning scheduler and strand the task
	 * to be picked up during sched exit.
	 */
	ಆದರೆ (enq_flags & SCX_ENQ_REENQ) {
		ಆದರೆ (++p->scx.reenq_cnt > 1)
			__scx_add_event(sch, SCX_EV_REENQ_REPEAT, 1);

		ಆದರೆ (unlikely(p->scx.reenq_cnt > SCX_REENQ_MAX_REPEAT)) {
			__scx_exit(sch, SCX_EXIT_ERROR_REENQ, 0, cpu_ದ(rq),
				   "%s[%d] reenqueued %u times without running",
				   p->comm, p->pid, p->scx.reenq_cnt);
			ಹಿಂತಿರುಗಿ;
		}
	}

	/*
	 * If !scx_rq_online(), we already told the BPF scheduler that the CPU
	 * is offline and are just running the hotplug path. Don't bother the
	 * BPF scheduler.
	 */
	ಆದರೆ (!scx_rq_online(rq))
		ಹೋಗು local;

	ಆದರೆ (scx_bypassing(sch, cpu_ದ(rq))) {
		__scx_add_event(sch, SCX_EV_BYPASS_DISPATCH, 1);
		ಹೋಗು bypass;
	}

	ಆದರೆ (p->scx.ddsp_dsq_id != SCX_DSQ_INVALID)
		ಹೋಗು ನೇರ;

	/* see %SCX_OPS_ENQ_EXITING */
	ಆದರೆ (!(sch->ops.flags & SCX_OPS_ENQ_EXITING) &&
	    unlikely(p->flags & PF_EXITING)) {
		__scx_add_event(sch, SCX_EV_ENQ_SKIP_EXITING, 1);
		enq_flags |= SCX_ENQ_RESCUE;	/* avoid looping on cap rejection */
		ಹೋಗು local;
	}

	/* see %SCX_OPS_ENQ_MIGRATION_DISABLED */
	ಆದರೆ (!(sch->ops.flags & SCX_OPS_ENQ_MIGRATION_DISABLED) &&
	    is_migration_disabled(p)) {
		__scx_add_event(sch, SCX_EV_ENQ_SKIP_MIGRATION_DISABLED, 1);
		ಹೋಗು local;
	}

	ಆದರೆ (unlikely(!SCX_HAS_OP(sch, ಸರತಿಗೆ_ಸೇರಿಸು)))
		ಹೋಗು global;

	/*
	 * DSQ bypass didn't trigger, enqueue on the BPF scheduler. Wrap the
	 * per-task qseq counter where the QSEQ field wraps and skip 0, which is
	 * what scx_bpf_dsq_insert() records for a task in NONE or DISPATCHING.
	 */
	p->scx.ops_qseq = ((p->scx.ops_qseq + 1) &
			   (SCX_OPSS_QSEQ_ಮುಖವಾಡ >> SCX_OPSS_QSEQ_ಸರಿಸು)) ?: 1;
	qseq = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)p->scx.ops_qseq << SCX_OPSS_QSEQ_ಸರಿಸು;

	WARN_ON_ONCE(atomic_long_read(&p->scx.ops_state) != SCX_OPSS_NONE);
	atomic_long_ಗಣ(&p->scx.ops_state, SCX_OPSS_QUEUEING | qseq);

	ddsp_taskp = this_cpu_ptr(&ನೇರ_dispatch_task);
	WARN_ON_ONCE(*ddsp_taskp);
	*ddsp_taskp = p;

	SCX_CALL_OP_TASK(sch, ಸರತಿಗೆ_ಸೇರಿಸು, rq, p, enq_flags);

	*ddsp_taskp = ಖಾಲಿ;
	ಆದರೆ (p->scx.ddsp_dsq_id != SCX_DSQ_INVALID)
		ಹೋಗು ನೇರ;

	/*
	 * Task is now in BPF scheduler's custody. Set %SCX_TASK_IN_CUSTODY
	 * so ops.dequeue() is called when it leaves custody.
	 */
	p->scx.flags |= SCX_TASK_ಒಳ_CUSTODY;

	/*
	 * If not directly dispatched, QUEUEING isn't clear yet and dispatch or
	 * dequeue may be waiting. The store_release matches their load_acquire.
	 */
	atomic_long_ಗಣ_release(&p->scx.ops_state, SCX_OPSS_QUEUED | qseq);
	ಹಿಂತಿರುಗಿ;

ನೇರ:
	ನೇರ_dispatch(sch, p, enq_flags);
	ಹಿಂತಿರುಗಿ;
local_norefill:
	scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, rq, &rq->scx.local_dsq, p, 0, 0, enq_flags);
	ಹಿಂತಿರುಗಿ;
local:
	dsq = &rq->scx.local_dsq;
	ಹೋಗು ಸರತಿಗೆ_ಸೇರಿಸು;
global:
	dsq = ಕಂಡುಹಿಡಿ_global_dsq(sch, task_cpu(p));
	ಹೋಗು ಸರತಿಗೆ_ಸೇರಿಸು;
bypass:
	dsq = bypass_enq_ಗುರಿ_dsq(sch, task_cpu(p));
	ಹೋಗು ಸರತಿಗೆ_ಸೇರಿಸು;

ಸರತಿಗೆ_ಸೇರಿಸು:
	refill_task_ತುಂಡು_dfl(sch, p);
	clear_ನೇರ_dispatch(p);
	scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, rq, dsq, p, 0, 0, enq_flags);
}

ಸ್ಥಿರ bool task_runnable(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ !list_ಖಾಲಿಗಳು(&p->scx.runnable_ಗಂಟು);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_task_runnable(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	lockdep_assert_rq_held(rq);

	ಆದರೆ (p->scx.flags & SCX_TASK_RESET_RUNNABLE_AT) {
		p->scx.runnable_at = jiffies;
		p->scx.flags &= ~SCX_TASK_RESET_RUNNABLE_AT;
	}

	/*
	 * list_add_tail() must be used. scx_bypass() and rq_offline_scx()
	 * depend on tasks being appended to the runnable_list.
	 */
	list_ಕೂಡಿಸು_ಬಾಲ(&p->scx.runnable_ಗಂಟು, &rq->scx.runnable_list);

	/*
	 * Record the rq @p is runnable on, maintained under the rq lock so it
	 * stays valid unlike task_cpu(), which a remote wakeup can move under
	 * pi_lock alone.
	 */
	WRITE_ONCE(p->scx.runnable_cpu, cpu_ದ(rq));
}

ಸ್ಥಿರ ಶೂನ್ಯ clr_task_runnable(ರಚನೆ task_struct *p, bool reset_runnable_at)
{
	list_del_init(&p->scx.runnable_ಗಂಟು);
	WRITE_ONCE(p->scx.runnable_cpu, -1);
	ಆದರೆ (reset_runnable_at) {
		p->scx.flags |= SCX_TASK_RESET_RUNNABLE_AT;
		p->scx.reenq_cnt = 0;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_task_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ core_enq_flags)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);
	ಪೂರ್ಣಾಂಕ sticky_cpu = p->scx.sticky_cpu;
	u64 enq_flags = core_enq_flags | rq->scx.remote_activate_enq_flags;

	/*
	 * An SCX-internal migration ends on arrival. Clear sticky_cpu so @p can
	 * leave custody when inserted into the destination DSQ.
	 */
	ಆದರೆ (sticky_cpu >= 0)
		p->scx.sticky_cpu = -1;

	/*
	 * SCX_RQ_IN_WAKEUP promises a task_woken_scx() call once this enqueue
	 * returns. Only the core's wakeup path delivers one. The flags stashed
	 * for a remote activation may carry the wakeup bit without it.
	 */
	ಆದರೆ (core_enq_flags & ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP)
		rq->scx.flags |= SCX_RQ_ಒಳ_WAKEUP;

	/*
	 * Restoring a running task will be immediately followed by
	 * set_next_task_scx() which expects the task to not be on the BPF
	 * scheduler as tasks can only start running through local DSQs. Force
	 * direct-dispatch into the local DSQ by setting the sticky_cpu. Mark
	 * IGNORE_CAPS to force entry into the local DSQ.
	 */
	ಆದರೆ (unlikely(enq_flags & ಸರತಿಗೆ_ಸೇರಿಸು_RESTORE) && task_current(rq, p)) {
		sticky_cpu = cpu_ದ(rq);
		enq_flags |= SCX_ENQ_IGNORE_CAPS;
	}

	ಆದರೆ (p->scx.flags & SCX_TASK_QUEUED) {
		WARN_ON_ONCE(!task_runnable(p));
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಗಣ_task_runnable(rq, p);
	p->scx.flags |= SCX_TASK_QUEUED;
	rq->scx.nr_running++;
	ಕೂಡಿಸು_nr_running(rq, 1);

	ಆದರೆ (SCX_HAS_OP(sch, runnable) && !task_on_rq_migrating(p))
		SCX_CALL_OP_TASK(sch, runnable, rq, p, enq_flags);

	/* Start dl_server if this is the first task being enqueued */
	ಆದರೆ (rq->scx.nr_running == 1)
		dl_server_ಆರಂಭ(&rq->ext_server);

	scx_do_ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, enq_flags, sticky_cpu);
ಹೊರಹೋಗು:
	rq->scx.flags &= ~SCX_RQ_ಒಳ_WAKEUP;

	ಆದರೆ ((enq_flags & SCX_ENQ_CPU_SELECTED) &&
	    unlikely(cpu_ದ(rq) != p->scx.selected_cpu))
		__scx_add_event(sch, SCX_EV_SELECT_CPU_FALLBACK, 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ ops_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 deq_flags)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ opss;

	/* dequeue is always temporary, don't reset runnable_at */
	clr_task_runnable(p, false);

retry:
	/* acquire ensures that we see the preceding updates on QUEUED */
	opss = atomic_long_read_acquire(&p->scx.ops_state);

	ಆಯ್ಕೆ (opss & SCX_OPSS_STATE_ಮುಖವಾಡ) {
	ಸ್ಥಿತಿ SCX_OPSS_NONE:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_OPSS_QUEUEING:
		/*
		 * QUEUEING is started and finished while holding @p's rq lock.
		 * As we're holding the rq lock now, we shouldn't see QUEUEING.
		 */
		BUG();
	ಸ್ಥಿತಿ SCX_OPSS_QUEUED:
		/*
		 * A queued task must always be in BPF scheduler's custody. If
		 * SCX_TASK_IN_CUSTODY is clear, finish_dispatch() on another
		 * CPU has already passed task_leave_custody() (which clears the
		 * flag), but has not yet written SCX_OPSS_NONE. That final
		 * store does not require this rq's lock, so retrying with
		 * cpu_relax() is bounded: we will observe NONE (or DISPATCHING,
		 * handled by the fallthrough) on a subsequent iteration.
		 */
		ಆದರೆ (unlikely(!(READ_ONCE(p->scx.flags) & SCX_TASK_ಒಳ_CUSTODY))) {
			cpu_relax();
			ಹೋಗು retry;
		}

		ಆದರೆ (atomic_long_try_cmpxchg(&p->scx.ops_state, &opss,
					    SCX_OPSS_NONE))
			ನಿಲ್ಲಿಸು;
		fallthrough;
	ಸ್ಥಿತಿ SCX_OPSS_DISPATCHING:
		/*
		 * If @p is being dispatched from the BPF scheduler to a DSQ,
		 * wait for the transfer to complete so that @p doesn't get
		 * added to its DSQ after dequeueing is complete.
		 *
		 * As we're waiting on DISPATCHING with the rq locked, the
		 * dispatching side shouldn't try to lock the rq while
		 * DISPATCHING is set. See dispatch_to_local_dsq().
		 *
		 * DISPATCHING shouldn't have qseq set and control can reach
		 * here with NONE @opss from the above QUEUED case block.
		 * Explicitly wait on %SCX_OPSS_DISPATCHING instead of @opss.
		 */
		wait_ops_state(p, SCX_OPSS_DISPATCHING);
		BUG_ON(atomic_long_read(&p->scx.ops_state) != SCX_OPSS_NONE);
		ನಿಲ್ಲಿಸು;
	}

	/*
	 * Call ops.dequeue() if the task is still in BPF custody.
	 *
	 * The code that clears ops_state to %SCX_OPSS_NONE does not always
	 * clear %SCX_TASK_IN_CUSTODY: in dispatch_to_local_dsq(), when
	 * we're moving a task that was in %SCX_OPSS_DISPATCHING to a
	 * remote CPU's local DSQ, we only set ops_state to %SCX_OPSS_NONE
	 * so that a concurrent dequeue can proceed, but we clear
	 * %SCX_TASK_IN_CUSTODY only when we later enqueue or move the
	 * task. So we can see NONE + IN_CUSTODY here and we must handle
	 * it. Similarly, after waiting on %SCX_OPSS_DISPATCHING we see
	 * NONE but the task may still have %SCX_TASK_IN_CUSTODY set until
	 * it is enqueued on the destination.
	 */
	ಆದರೆ (task_leave_custody(p) && SCX_HAS_OP(sch, ಸರತಿಯಿಂದ_ತೆಗೆ))
		SCX_CALL_OP_TASK(sch, ಸರತಿಯಿಂದ_ತೆಗೆ, rq, p, deq_flags);
}

ಸ್ಥಿರ bool ಸರತಿಯಿಂದ_ತೆಗೆ_task_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ core_deq_flags)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);
	u64 deq_flags = core_deq_flags;

	/*
	 * Set %SCX_DEQ_SCHED_CHANGE when the dequeue is due to a property
	 * change (not sleep).
	 */
	ಆದರೆ (!(deq_flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP))
		deq_flags |= SCX_DEQ_SCHED_CHANGE;

	ಆದರೆ (!(p->scx.flags & SCX_TASK_QUEUED)) {
		WARN_ON_ONCE(task_runnable(p));
		ಹಿಂತಿರುಗಿ true;
	}

	ops_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p, deq_flags);

	/*
	 * A currently running task which is going off @rq first gets dequeued
	 * and then stops running. As we want running <-> stopping transitions
	 * to be contained within runnable <-> quiescent transitions, trigger
	 * ->stopping() early here instead of in put_prev_task_scx().
	 *
	 * @p may go through multiple stopping <-> running transitions between
	 * here and put_prev_task_scx() if task attribute changes occur while
	 * dispatch_one() leaves @rq unlocked. However, they don't contain any
	 * information meaningful to the BPF scheduler and can be suppressed by
	 * skipping the callbacks if the task is !QUEUED.
	 */
	ಆದರೆ (task_current(rq, p) &&
	    (SCX_HAS_OP(sch, stopping) || unlikely(p == scx_rescuee(rq)))) {
		update_curr_scx(rq);
		ಆದರೆ (SCX_HAS_OP(sch, stopping))
			SCX_CALL_OP_TASK(sch, stopping, rq, p, false);
	}

	ಆದರೆ (SCX_HAS_OP(sch, quiescent) && !task_on_rq_migrating(p))
		SCX_CALL_OP_TASK(sch, quiescent, rq, p, deq_flags);

	ಆದರೆ (deq_flags & SCX_DEQ_SLEEP)
		p->scx.flags |= SCX_TASK_DEQD_FOR_SLEEP;
	ಇಲ್ಲದಿದ್ದರೆ
		p->scx.flags &= ~SCX_TASK_DEQD_FOR_SLEEP;

	p->scx.flags &= ~SCX_TASK_QUEUED;
	rq->scx.nr_running--;
	sub_nr_running(rq, 1);

	scx_dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p);

	/* see scx_task_slice_ended() for the save/restore exception */
	ಆದರೆ (!((deq_flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE) && task_current(rq, p)))
		scx_task_ತುಂಡು_ended(rq, p);

	clear_ನೇರ_dispatch(p);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ yield_task_scx(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *p = rq->donor;
	ರಚನೆ scx_sched *sch = scx_task_sched(p);

	/* a yield gives the slice up */
	scx_task_ತುಂಡು_ended(rq, p);

	ಆದರೆ (SCX_HAS_OP(sch, yield))
		SCX_CALL_OP_2TASKS_RET(sch, yield, rq, p, ಖಾಲಿ);
	ಇಲ್ಲದಿದ್ದರೆ
		scx_ಗಣ_task_ತುಂಡು(p, 0);
}

ಸ್ಥಿರ bool yield_ಗಮ್ಯ_task_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *ಗಮ್ಯ)
{
	ರಚನೆ task_struct *from = rq->donor;
	ರಚನೆ scx_sched *sch = scx_task_sched(from);

	/* like a plain yield, giving the slice up ends the protection */
	scx_task_ತುಂಡು_ended(rq, from);

	ಆದರೆ (SCX_HAS_OP(sch, yield) && sch == scx_task_sched(ಗಮ್ಯ))
		ಹಿಂತಿರುಗಿ SCX_CALL_OP_2TASKS_RET(sch, yield, rq, from, ಗಮ್ಯ);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ wakeup_preempt_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ wake_flags)
{
	/*
	 * Preemption between SCX tasks is implemented by resetting the victim
	 * task's slice to 0 and triggering reschedule on the target CPU.
	 * Nothing to do.
	 */
	ಆದರೆ (p->sched_class == &ext_sched_class)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Getting preempted by a higher-priority class. Reenqueue IMMED tasks.
	 * This captures all preemption cases including:
	 *
	 * - A SCX task is currently running.
	 *
	 * - @rq is waking from idle due to a SCX task waking to it.
	 *
	 * - A higher-priority wakes up while SCX dispatch is in progress.
	 */
	ಆದರೆ (rq->scx.nr_immed)
		scx_ವೇಳಾಪಟ್ಟಿ_reenq_local(rq, 0);
}

ಶೂನ್ಯ scx_move_local_task_ಗಮ್ಯ_local_dsq(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p,
				      u64 enq_flags, ರಚನೆ rq *dst_rq)
{
	ರಚನೆ scx_dispatch_q *dst_dsq;

	lockdep_assert_rq_held(dst_rq);

	WARN_ON_ONCE(p->scx.holding_cpu >= 0);
	scx_wait_dispatching(p);

	dst_dsq = scx_resolve_local_dsq(sch, dst_rq, p, &enq_flags);

	ಆದರೆ (enq_flags & (SCX_ENQ_ತಲೆ | SCX_ENQ_PREEMPT))
		dsq_ಸೇರಿಸು_ತಲೆ(dst_dsq, p);
	ಇಲ್ಲದಿದ್ದರೆ
		list_ಕೂಡಿಸು_ಬಾಲ(&p->scx.dsq_list.ಗಂಟು, &dst_dsq->list);

	dsq_inc_nr(dst_dsq, p, enq_flags);
	p->scx.dsq = dst_dsq;

	rq_owned_post_enq(sch, dst_rq, dst_dsq, p, enq_flags);
}

/**
 * move_remote_task_to_local_dsq - Move a task from a foreign rq to a local DSQ
 * @sch: scheduler placing @p
 * @p: task to move
 * @enq_flags: %SCX_ENQ_*
 * @src_rq: rq to move the task from, locked on entry, released on return
 * @dst_rq: rq to move the task into, locked on return
 *
 * Move @p which is currently on @src_rq to @dst_rq's local DSQ.
 */
ಸ್ಥಿರ ಶೂನ್ಯ move_remote_task_ಗಮ್ಯ_local_dsq(ರಚನೆ scx_sched *sch,
					  ರಚನೆ task_struct *p, u64 enq_flags,
					  ರಚನೆ rq *src_rq, ರಚನೆ rq *dst_rq)
{
	lockdep_assert_rq_held(src_rq);

	/*
	 * Set sticky_cpu before deactivate_task() to properly mark the
	 * beginning of an SCX-internal migration.
	 */
	p->scx.sticky_cpu = cpu_ದ(dst_rq);
	deactivate_task(src_rq, p, 0);
	ಗಣ_task_cpu(p, cpu_ದ(dst_rq));

	switch_rq_ಬೀಗ(src_rq, dst_rq);

	/*
	 * activate_task() below truncates enq_flags to 32 bits and re-derives
	 * @p's owner, dropping our scx flags and the placing @sch. We own @rq,
	 * so stash both across the call. The enqueue reads them back, keeping
	 * the scx flags and checking caps against the placer, not the owner.
	 */
	WARN_ON_ONCE(!cpumask_test_cpu(cpu_ದ(dst_rq), p->cpus_ptr));
	WARN_ON_ONCE(dst_rq->scx.remote_activate_enq_flags ||
		     dst_rq->scx.remote_activate_sch);
	dst_rq->scx.remote_activate_enq_flags = enq_flags;
	dst_rq->scx.remote_activate_sch = sch;
	activate_task(dst_rq, p, 0);
	dst_rq->scx.remote_activate_enq_flags = 0;
	dst_rq->scx.remote_activate_sch = ಖಾಲಿ;
}

/*
 * Similar to kernel/sched/core.c::is_cpu_allowed(). However, there are two
 * differences:
 *
 * - is_cpu_allowed() asks "Can this task run on this CPU?" while
 *   task_can_run_on_remote_rq() asks "Can the BPF scheduler migrate the task to
 *   this CPU?".
 *
 *   While migration is disabled, is_cpu_allowed() has to say "yes" as the task
 *   must be allowed to finish on the CPU that it's currently on regardless of
 *   the CPU state. However, task_can_run_on_remote_rq() must say "no" as the
 *   BPF scheduler shouldn't attempt to migrate a task which has migration
 *   disabled.
 *
 * - The BPF scheduler is bypassed while the rq is offline and we can always say
 *   no to the BPF scheduler initiated migrations while offline.
 *
 * The caller must ensure that @p and @rq are on different CPUs.
 * If enforce == true, caller must hold @p's rq lock.
 */
ಸ್ಥಿರ bool task_can_ಸತತ_on_remote_rq(ರಚನೆ scx_sched *sch,
				      ರಚನೆ task_struct *p, ರಚನೆ rq *rq,
				      bool enforce)
{
	s32 cpu = cpu_ದ(rq);

	/*
	 * To prevent races with @p still running on its old CPU while switching
	 * out, make sure we're holding @p's rq lock so as not to risk
	 * erroneously killing the BPF scheduler.
	 */
	ಆದರೆ (enforce)
		lockdep_assert_rq_held(task_rq(p));

	WARN_ON_ONCE(task_cpu(p) == cpu);

	/*
	 * If @p has migration disabled, @p->cpus_ptr is updated to contain only
	 * the pinned CPU in migrate_disable_switch() while @p is being switched
	 * out. However, put_prev_task_scx() is called before @p->cpus_ptr is
	 * updated and thus another CPU may see @p on a DSQ inbetween leading to
	 * @p passing the below task_allowed_on_cpu() check while migration is
	 * disabled.
	 *
	 * Test the migration disabled state first as the race window is narrow
	 * and the BPF scheduler failing to check migration disabled state can
	 * easily be masked if task_allowed_on_cpu() is done first.
	 */
	ಆದರೆ (unlikely(is_migration_disabled(p))) {
		ಆದರೆ (enforce)
			scx_error(sch, "SCX_DSQ_LOCAL[_ON] cannot move migration disabled %s[%d] from CPU %d to %d",
				  p->comm, p->pid, task_cpu(p), cpu);
		ಹಿಂತಿರುಗಿ false;
	}

	/*
	 * We don't require the BPF scheduler to avoid dispatching to offline
	 * CPUs mostly for convenience but also because CPUs can go offline
	 * between scx_bpf_dsq_insert() calls and here. Trigger error iff the
	 * picked CPU is outside the allowed mask.
	 */
	ಆದರೆ (!task_allowed_on_cpu(p, cpu)) {
		ಆದರೆ (enforce)
			scx_error(sch, "SCX_DSQ_LOCAL[_ON] target CPU %d not allowed for %s[%d]",
				  cpu, p->comm, p->pid);
		ಹಿಂತಿರುಗಿ false;
	}

	ಆದರೆ (!scx_rq_online(rq)) {
		ಆದರೆ (enforce)
			__scx_add_event(sch, SCX_EV_DISPATCH_LOCAL_DSQ_OFFLINE, 1);
		ಹಿಂತಿರುಗಿ false;
	}

	ಹಿಂತಿರುಗಿ true;
}

/**
 * unlink_dsq_and_switch_rq_lock() - Unlink task and switch to its rq lock
 * @p: target task
 * @dsq: locked DSQ @p is currently on
 * @locked_rq: currently locked rq
 * @src_rq: rq @p is currently on, stable with @dsq locked
 *
 * Called with @dsq and @locked_rq locked. We want to move @p to a different DSQ,
 * including any local DSQ, but are not locking @src_rq. Locking @src_rq is
 * required when transferring into a local DSQ. Even when transferring into a
 * non-local DSQ, it's better to use the same mechanism to protect against
 * dequeues and maintain the invariant that @p->scx.dsq can only change while
 * @src_rq is locked, which e.g. scx_dump_task() depends on.
 *
 * We want to grab @src_rq but that can deadlock if we try while locking @dsq,
 * so we want to unlink @p from @dsq, drop its lock and then lock @src_rq. As
 * this may race with dequeue, which can't drop the rq lock or fail, do a little
 * dancing from our side.
 *
 * @p->scx.holding_cpu is set to this CPU before @dsq is unlocked. If @p gets
 * dequeued after we unlock @dsq but before locking @src_rq, the holding_cpu
 * would be cleared to -1. While other cpus may have updated it to different
 * values afterwards, as this operation can't be preempted or recurse, the
 * holding_cpu can never become this CPU again before we're done. Thus, we can
 * tell whether we lost to dequeue by testing whether the holding_cpu still
 * points to this CPU. See scx_dispatch_dequeue() for the counterpart.
 *
 * On return, @dsq is unlocked and @src_rq is locked. Returns %true if @p is
 * still valid. %false if lost to dequeue.
 */
ಸ್ಥಿರ bool unlink_dsq_and_switch_rq_ಬೀಗ(ರಚನೆ task_struct *p,
					  ರಚನೆ scx_dispatch_q *dsq,
					  ರಚನೆ rq *locked_rq,
					  ರಚನೆ rq *src_rq)
{
	s32 cpu = raw_smp_processor_id();

	lockdep_assert_held(&dsq->ಬೀಗ);
	lockdep_assert_rq_held(locked_rq);

	WARN_ON_ONCE(p->scx.holding_cpu >= 0);
	scx_task_unlink_from_dsq(p, dsq);
	p->scx.holding_cpu = cpu;

	raw_spin_unlock(&dsq->ಬೀಗ);
	switch_rq_ಬೀಗ(locked_rq, src_rq);

	/* task_rq couldn't have changed if we're still the holding cpu */
	ಆದರೆ (likely(p->scx.holding_cpu == cpu) && !WARN_ON_ONCE(src_rq != task_rq(p))) {
		scx_wait_dispatching(p);
		ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool consume_remote_task(ರಚನೆ scx_sched *sch, ರಚನೆ rq *this_rq,
				ರಚನೆ task_struct *p, u64 enq_flags,
				ರಚನೆ scx_dispatch_q *dsq, ರಚನೆ rq *src_rq)
{
	ಆದರೆ (unlink_dsq_and_switch_rq_ಬೀಗ(p, dsq, this_rq, src_rq)) {
		move_remote_task_ಗಮ್ಯ_local_dsq(sch, p, enq_flags, src_rq, this_rq);
		ಹಿಂತಿರುಗಿ true;
	} ಇಲ್ಲದಿದ್ದರೆ {
		switch_rq_ಬೀಗ(src_rq, this_rq);
		ಹಿಂತಿರುಗಿ false;
	}
}

/**
 * move_task_between_dsqs() - Move a task from one DSQ to another
 * @sch: scx_sched being operated on
 * @p: target task
 * @enq_flags: %SCX_ENQ_*
 * @src_dsq: DSQ @p is currently on, must not be a local DSQ
 * @dst_dsq: DSQ @p is being moved to, can be any DSQ
 *
 * Must be called with @p's task_rq and @src_dsq locked. If @dst_dsq is a local
 * DSQ and @p is on a different CPU, @p will be migrated and thus its task_rq
 * will change. As @p's task_rq is locked, this function doesn't need to use the
 * holding_cpu mechanism.
 *
 * On return, @src_dsq is unlocked and only @p's new task_rq, which is the
 * return value, is locked.
 */
ಸ್ಥಿರ ರಚನೆ rq *move_task_between_dsqs(ರಚನೆ scx_sched *sch,
					 ರಚನೆ task_struct *p, u64 enq_flags,
					 ರಚನೆ scx_dispatch_q *src_dsq,
					 ರಚನೆ scx_dispatch_q *dst_dsq)
{
	ರಚನೆ rq *src_rq = task_rq(p), *dst_rq;

	BUG_ON(src_dsq->id == SCX_DSQ_LOCAL);
	lockdep_assert_held(&src_dsq->ಬೀಗ);
	lockdep_assert_rq_held(src_rq);

	scx_wait_dispatching(p);

	ಆದರೆ (dst_dsq->id == SCX_DSQ_LOCAL) {
		dst_rq = container_ದ(dst_dsq, ರಚನೆ rq, scx.local_dsq);
		ಆದರೆ (src_rq != dst_rq &&
		    unlikely(!task_can_ಸತತ_on_remote_rq(sch, p, dst_rq, true))) {
			dst_dsq = ಕಂಡುಹಿಡಿ_global_dsq(sch, task_cpu(p));
			dst_rq = src_rq;
			enq_flags |= SCX_ENQ_GDSQ_FALLBACK;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* no need to migrate if destination is a non-local DSQ */
		dst_rq = src_rq;
	}

	/*
	 * Move @p into $dst_dsq. If $dst_dsq is the local DSQ of a different
	 * CPU, @p will be migrated.
	 */
	ಆದರೆ (dst_dsq->id == SCX_DSQ_LOCAL) {
		/* @p is going from a non-local DSQ to a local DSQ */
		ಆದರೆ (src_rq == dst_rq) {
			scx_task_unlink_from_dsq(p, src_dsq);
			raw_spin_unlock(&src_dsq->ಬೀಗ);
			scx_move_local_task_ಗಮ್ಯ_local_dsq(sch, p, enq_flags, dst_rq);
		} ಇಲ್ಲದಿದ್ದರೆ {
			raw_spin_unlock(&src_dsq->ಬೀಗ);
			move_remote_task_ಗಮ್ಯ_local_dsq(sch, p, enq_flags, src_rq, dst_rq);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * @p is going from a non-local DSQ to a non-local DSQ. As
		 * $src_dsq is already locked, do an abbreviated dequeue.
		 */
		dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ_locked(p, src_dsq);
		raw_spin_unlock(&src_dsq->ಬೀಗ);

		scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, dst_rq, dst_dsq, p, 0, 0, enq_flags);
	}

	ಹಿಂತಿರುಗಿ dst_rq;
}

bool scx_consume_dispatch_q(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq,
			    ರಚನೆ scx_dispatch_q *dsq, u64 enq_flags)
{
	ರಚನೆ task_struct *p;
retry:
	/*
	 * The caller can't expect to successfully consume a task if the task's
	 * addition to @dsq isn't guaranteed to be visible somehow. Test
	 * @dsq->list without locking and skip if it seems empty.
	 */
	ಆದರೆ (list_ಖಾಲಿಗಳು(&dsq->list))
		ಹಿಂತಿರುಗಿ false;

	raw_spin_ಬೀಗ(&dsq->ಬೀಗ);

	nldsq_for_each_task(p, dsq) {
		ರಚನೆ rq *task_rq = task_rq(p);

		/*
		 * This loop can lead to multiple lockup scenarios, e.g. the BPF
		 * scheduler can put an enormous number of affinitized tasks into
		 * a contended DSQ, or the outer retry loop can repeatedly race
		 * against scx_bypass() dequeueing tasks from @dsq trying to put
		 * the system into the bypass mode. This can easily live-lock the
		 * machine. If aborting, exit from all non-bypass DSQs.
		 */
		ಆದರೆ (unlikely(READ_ONCE(sch->aborting)) && dsq->id != SCX_DSQ_BYPASS)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (rq == task_rq) {
			scx_task_unlink_from_dsq(p, dsq);
			raw_spin_unlock(&dsq->ಬೀಗ);
			scx_move_local_task_ಗಮ್ಯ_local_dsq(sch, p, enq_flags, rq);
			ಹಿಂತಿರುಗಿ true;
		}

		ಆದರೆ (task_can_ಸತತ_on_remote_rq(sch, p, rq, false)) {
			ಆದರೆ (likely(consume_remote_task(sch, rq, p, enq_flags, dsq, task_rq)))
				ಹಿಂತಿರುಗಿ true;
			ಹೋಗು retry;
		}
	}

	raw_spin_unlock(&dsq->ಬೀಗ);
	ಹಿಂತಿರುಗಿ false;
}

bool scx_consume_global_dsq(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq)
{
	ಪೂರ್ಣಾಂಕ ಗಂಟು = cpu_ಗಮ್ಯ_ಗಂಟು(cpu_ದ(rq));

	ಹಿಂತಿರುಗಿ scx_consume_dispatch_q(sch, rq, &sch->pnode[ಗಂಟು]->global_dsq, 0);
}

/**
 * dispatch_to_local_dsq - Dispatch a task to a local dsq
 * @sch: scx_sched being operated on
 * @rq: current rq which is locked
 * @dst_dsq: destination DSQ
 * @p: task to dispatch
 * @slice: slice carried by the insert verdict, 0 keeps the current value
 * @vtime: vtime carried by the insert verdict, committed on PRIQ inserts
 * @enq_flags: %SCX_ENQ_*
 *
 * We're holding @rq lock and want to dispatch @p to @dst_dsq which is a local
 * DSQ. This function performs all the synchronization dancing needed because
 * local DSQs are protected with rq locks.
 *
 * The caller must have exclusive ownership of @p (e.g. through
 * %SCX_OPSS_DISPATCHING).
 */
ಸ್ಥಿರ ಶೂನ್ಯ dispatch_ಗಮ್ಯ_local_dsq(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq,
				  ರಚನೆ scx_dispatch_q *dst_dsq, ರಚನೆ task_struct *p,
				  u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	ರಚನೆ rq *src_rq = task_rq(p);
	ರಚನೆ rq *dst_rq = container_ದ(dst_dsq, ರಚನೆ rq, scx.local_dsq);
	ರಚನೆ rq *locked_rq = rq;

	/*
	 * We're synchronized against dequeue through DISPATCHING. As @p can't
	 * be dequeued, its task_rq and cpus_allowed are stable too.
	 *
	 * If dispatching to @rq that @p is already on, no lock dancing needed.
	 */
	ಆದರೆ (rq == src_rq && rq == dst_rq) {
		scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, rq, dst_dsq, p, ತುಂಡು, vtime,
				     enq_flags | SCX_ENQ_APPLY_ತುಂಡು | SCX_ENQ_CLEAR_OPSS);
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * @p is on a possibly remote @src_rq which we need to lock to move the
	 * task. If dequeue is in progress, it'd be locking @src_rq and waiting
	 * on DISPATCHING, so we can't grab @src_rq lock while holding
	 * DISPATCHING.
	 *
	 * As DISPATCHING guarantees that @p is wholly ours, we can pretend that
	 * we're moving from a DSQ and use the same mechanism - mark the task
	 * under transfer with holding_cpu, release DISPATCHING and then follow
	 * the same protocol. See unlink_dsq_and_switch_rq_lock().
	 */
	p->scx.holding_cpu = raw_smp_processor_id();

	/* store_release ensures that dequeue sees the above */
	atomic_long_ಗಣ_release(&p->scx.ops_state, SCX_OPSS_NONE);

	/* switch to @src_rq lock */
	ಆದರೆ (locked_rq != src_rq) {
		switch_rq_ಬೀಗ(locked_rq, src_rq);
		locked_rq = src_rq;
	}

	/* task_rq couldn't have changed if we're still the holding cpu */
	ಆದರೆ (likely(p->scx.holding_cpu == raw_smp_processor_id()) &&
	    !WARN_ON_ONCE(src_rq != task_rq(p))) {
		bool fallback = false;
		/*
		 * If @p is staying on the same rq, there's no need to go
		 * through the full deactivate/activate cycle. Optimize by
		 * abbreviating move_remote_task_to_local_dsq().
		 */
		ಆದರೆ (src_rq == dst_rq) {
			p->scx.holding_cpu = -1;
			scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, dst_rq, &dst_rq->scx.local_dsq, p,
					     ತುಂಡು, vtime, enq_flags | SCX_ENQ_APPLY_ತುಂಡು);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(!task_can_ಸತತ_on_remote_rq(sch, p, dst_rq, true))) {
			p->scx.holding_cpu = -1;
			fallback = true;
			scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, src_rq, ಕಂಡುಹಿಡಿ_global_dsq(sch, task_cpu(p)),
					     p, ತುಂಡು, vtime,
					     enq_flags | SCX_ENQ_APPLY_ತುಂಡು |
					     SCX_ENQ_GDSQ_FALLBACK);
		} ಇಲ್ಲದಿದ್ದರೆ {
			apply_ತುಂಡು_vtime(p, ತುಂಡು, vtime, enq_flags);
			move_remote_task_ಗಮ್ಯ_local_dsq(sch, p, enq_flags, src_rq, dst_rq);
			/* task has been moved to dst_rq, which is now locked */
			locked_rq = dst_rq;
		}

		/* if the destination CPU is idle, wake it up */
		ಆದರೆ (!fallback && sched_class_above(p->sched_class, dst_rq->curr->sched_class))
			resched_curr(dst_rq);
	}

	/* switch back to @rq lock */
	ಆದರೆ (locked_rq != rq)
		switch_rq_ಬೀಗ(locked_rq, rq);
}

/**
 * finish_dispatch - Asynchronously finish dispatching a task
 * @sch: the scheduler
 * @rq: current rq which is locked
 * @p: task to finish dispatching
 * @qseq_at_dispatch: qseq when @p started getting dispatched
 * @dsq_id: destination DSQ ID
 * @slice: slice carried by the insert verdict, 0 keeps the current value
 * @vtime: vtime carried by the insert verdict, committed on PRIQ inserts
 * @enq_flags: %SCX_ENQ_*
 *
 * Dispatching to local DSQs may need to wait for queueing to complete or
 * require rq lock dancing. As we don't wanna do either while inside
 * ops.dispatch() to avoid locking order inversion, we split dispatching into
 * two parts. scx_bpf_dsq_insert() which is called by ops.dispatch() records the
 * task and its qseq. Once ops.dispatch() returns, this function is called to
 * finish up.
 *
 * There is no guarantee that @p is still valid for dispatching or even that it
 * was valid in the first place. Make sure that the task is still owned by the
 * BPF scheduler and claim the ownership before dispatching.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಮುಕ್ತಾಯ_dispatch(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ qseq_at_dispatch, u64 dsq_id,
			    u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	ರಚನೆ scx_dispatch_q *dsq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ opss;

retry:
	/*
	 * No need for _acquire here. @p is accessed only after a successful
	 * try_cmpxchg to DISPATCHING.
	 */
	opss = atomic_long_read(&p->scx.ops_state);

	ಆಯ್ಕೆ (opss & SCX_OPSS_STATE_ಮುಖವಾಡ) {
	ಸ್ಥಿತಿ SCX_OPSS_DISPATCHING:
	ಸ್ಥಿತಿ SCX_OPSS_NONE:
		/* someone else already got to it */
		ಹಿಂತಿರುಗಿ;
	ಸ್ಥಿತಿ SCX_OPSS_QUEUED:
		/*
		 * If qseq doesn't match, @p has gone through at least one
		 * dispatch/dequeue and re-enqueue cycle between
		 * scx_bpf_dsq_insert() and here and we have no claim on it.
		 */
		ಆದರೆ ((opss & SCX_OPSS_QSEQ_ಮುಖವಾಡ) != qseq_at_dispatch)
			ಹಿಂತಿರುಗಿ;

		/* see SCX_EV_INSERT_NOT_OWNED definition */
		ಆದರೆ (unlikely(!scx_task_on_sched(sch, p))) {
			__scx_add_event(sch, SCX_EV_ಸೇರಿಸು_NOT_OWNED, 1);
			ಹಿಂತಿರುಗಿ;
		}

		/*
		 * While we know @p is accessible, we don't yet have a claim on
		 * it - the BPF scheduler is allowed to dispatch tasks
		 * spuriously and there can be a racing dequeue attempt. Let's
		 * claim @p by atomically transitioning it from QUEUED to
		 * DISPATCHING.
		 */
		ಆದರೆ (likely(atomic_long_try_cmpxchg(&p->scx.ops_state, &opss,
						   SCX_OPSS_DISPATCHING)))
			ನಿಲ್ಲಿಸು;
		ಹೋಗು retry;
	ಸ್ಥಿತಿ SCX_OPSS_QUEUEING:
		/*
		 * scx_do_enqueue_task() is in the process of transferring the
		 * task to the BPF scheduler while holding @p's rq lock. As we
		 * aren't holding any kernel or BPF resource that the enqueue
		 * path may depend upon, it's safe to wait.
		 */
		wait_ops_state(p, opss);
		ಹೋಗು retry;
	}

	BUG_ON(!(p->scx.flags & SCX_TASK_QUEUED));

	dsq = ಕಂಡುಹಿಡಿ_dsq_for_dispatch(sch, rq, dsq_id, task_cpu(p));

	ಆದರೆ (dsq->id == SCX_DSQ_LOCAL)
		dispatch_ಗಮ್ಯ_local_dsq(sch, rq, dsq, p, ತುಂಡು, vtime, enq_flags);
	ಇಲ್ಲದಿದ್ದರೆ
		scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, rq, dsq, p, ತುಂಡು, vtime,
				     enq_flags | SCX_ENQ_APPLY_ತುಂಡು | SCX_ENQ_CLEAR_OPSS);
}

ಶೂನ್ಯ scx_flush_dispatch_buf(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq)
{
	ರಚನೆ scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	u32 u;

	ಗಾಗಿ (u = 0; u < dspc->cursor; u++) {
		ರಚನೆ scx_dsp_buf_ent *ent = &dspc->buf[u];

		ಮುಕ್ತಾಯ_dispatch(sch, rq, ent->task, ent->qseq, ent->dsq_id,
				ent->ತುಂಡು, ent->vtime, ent->enq_flags);
	}

	dspc->nr_tasks += dspc->cursor;
	dspc->cursor = 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ maybe_ಸರತಿ_balance_callback(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);

	ಆದರೆ (!(rq->scx.flags & SCX_RQ_BAL_CB_PENDING))
		ಹಿಂತಿರುಗಿ;

	ಸರತಿ_balance_callback(rq, &rq->scx.deferred_bal_cb,
				deferred_bal_cb_workfn);

	rq->scx.flags &= ~SCX_RQ_BAL_CB_PENDING;
}

ಸ್ಥಿರ ಎಣಿಕೆ scx_dsp_verdict dispatch_one(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev)
{
	ರಚನೆ scx_sched *ಬೇರು_sch = scx_ಬೇರು_protected_ಜೀವಂತ();
	ಎಣಿಕೆ scx_dsp_verdict verdict;
	s32 cpu = cpu_ದ(rq);

	lockdep_assert_rq_held(rq);
	rq->scx.flags |= SCX_RQ_ಒಳ_DISPATCH;

	scx_process_sync_ecaps(rq, prev);

	ಆದರೆ ((ಬೇರು_sch->ops.flags & SCX_OPS_HAS_CPU_PREEMPT) &&
	    unlikely(rq->scx.cpu_released)) {
		/*
		 * If the previous sched_class for the current CPU was not SCX,
		 * notify the BPF scheduler that it again has control of the
		 * core. This callback complements ->cpu_release(), which is
		 * emitted in switch_class().
		 */
		ಆದರೆ (ಬೇರು_sch->ops.cpu_acquire)
			SCX_CALL_OP(ಬೇರು_sch, cpu_acquire, rq, cpu, ಖಾಲಿ);
		rq->scx.cpu_released = false;
	}

	ಆದರೆ (prev->sched_class == &ext_sched_class) {
		update_curr_scx(rq);

		/*
		 * If @prev is runnable & has slice left, it has priority and
		 * fetching more just increases latency for the fetched tasks.
		 * Tell pick_task_scx() to keep running @prev. If the BPF
		 * scheduler wants to handle this explicitly, it should
		 * implement ->cpu_release().
		 *
		 * See scx_disable_workfn() for the explanation on the bypassing
		 * test.
		 */
		ಆದರೆ ((prev->scx.flags & SCX_TASK_QUEUED) && prev->scx.ತುಂಡು &&
		    !scx_bypassing(scx_task_sched(prev), cpu)) {
			verdict = SCX_DSP_PREV;
			ಹೋಗು has_tasks;
		}
	}

	/* if there already are tasks to run, nothing to do */
	ಆದರೆ (rq->scx.local_dsq.nr) {
		verdict = SCX_DSP_LOCAL;
		ಹೋಗು has_tasks;
	}

	verdict = scx_dispatch_sched(ಬೇರು_sch, rq, prev, false);
	ಆದರೆ (verdict != SCX_DSP_NONE)
		ಹೋಗು has_tasks;

	/*
	 * Didn't find another task to run. Keep running @prev unless its own
	 * scheduler set %SCX_OPS_ENQ_LAST and takes the enqueue instead, see
	 * put_prev_task_scx(). Read the scheduler here as the dispatch above
	 * may have dropped the rq lock while @prev changed class or scheduler.
	 */
	ಆದರೆ (prev->scx.flags & SCX_TASK_QUEUED) {
		ರಚನೆ scx_sched *prev_sch = scx_task_sched(prev);

		ಆದರೆ ((!(prev_sch->ops.flags & SCX_OPS_ENQ_ಕೊನೆಯ) ||
		     scx_bypassing(prev_sch, cpu)) && scx_task_can_stay_on_cpu(rq, prev)) {
			__scx_add_event(prev_sch, SCX_EV_DISPATCH_KEEP_ಕೊನೆಯ, 1);
			verdict = SCX_DSP_PREV;
			ಹೋಗು has_tasks;
		}
	}
	rq->scx.flags &= ~SCX_RQ_ಒಳ_DISPATCH;
	ಹಿಂತಿರುಗಿ SCX_DSP_NONE;

has_tasks:
	/*
	 * @rq may have extra IMMED tasks without reenq scheduled:
	 *
	 * - rq_is_open() can't reliably tell when and how slice is going to be
	 *   modified for $curr and allows IMMED tasks to be queued while
	 *   dispatch is in progress.
	 *
	 * - A non-IMMED HEAD task can get queued in front of an IMMED task
	 *   between the IMMED queueing and the subsequent scheduling event.
	 */
	ಆದರೆ (unlikely(rq->scx.local_dsq.nr > 1 && rq->scx.nr_immed))
		scx_ವೇಳಾಪಟ್ಟಿ_reenq_local(rq, 0);

	rq->scx.flags &= ~SCX_RQ_ಒಳ_DISPATCH;
	ಹಿಂತಿರುಗಿ verdict;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಮುಂದಿನ_task_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಎಣಿಕೆ snt_e type)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);

	ಆದರೆ (type == SNT_REPICK)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->scx.flags & SCX_TASK_QUEUED) {
		/*
		 * Core-sched might decide to execute @p before it is
		 * dispatched. Call ops_dequeue() to notify the BPF scheduler.
		 */
		ops_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p, SCX_DEQ_CORE_SCHED_EXEC);
		scx_dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p);
	}

	p->se.exec_ಆರಂಭ = rq_clock_task(rq);

	/* see dequeue_task_scx() on why we skip when !QUEUED */
	ಆದರೆ (SCX_HAS_OP(sch, running) && (p->scx.flags & SCX_TASK_QUEUED))
		SCX_CALL_OP_TASK(sch, running, rq, p);

	clr_task_runnable(p, true);

	/* apply any pending out-of-band slice request before the tick decision */
	apply_task_ತುಂಡು_oob(rq, p);

	/*
	 * @p is getting newly scheduled or got kicked after someone updated its
	 * slice. Update SCX_RQ_CAN_STOP_TICK to reflect whether the tick can be
	 * stopped. See scx_can_stop_tick().
	 *
	 * Moreover, refresh the load_avgs just when transitioning in and out of
	 * nohz. In the future, we might want to add a mechanism to update
	 * load_avgs periodically on tick-stopped CPUs.
	 */
	ಆದರೆ (p->scx.ತುಂಡು == SCX_ತುಂಡು_INF) {
		ಆದರೆ (!(rq->scx.flags & SCX_RQ_CAN_STOP_TICK)) {
			/*
			 * Bypass mode always assigns finite slices, so @p
			 * can't have an infinite slice while bypassing.
			 * Therefore, sched_update_tick_dependency() can safely
			 * evaluate the outgoing task.
			 */
			rq->scx.flags |= SCX_RQ_CAN_STOP_TICK;
			sched_update_tick_dependency(rq);

			update_other_ತುಂಬು_avgs(rq);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (rq->scx.flags & SCX_RQ_CAN_STOP_TICK) {
			rq->scx.flags &= ~SCX_RQ_CAN_STOP_TICK;
			update_other_ತುಂಬು_avgs(rq);
		}

		/*
		 * @rq still references the outgoing scheduling context. A finite
		 * slice is sufficient by itself to require the tick.
		 */
		ಆದರೆ (tick_nohz_ತುಂಬಿದ_cpu(cpu_ದ(rq)))
			tick_nohz_dep_ಗಣ_cpu(cpu_ದ(rq), TICK_DEP_ಬಿಟ್_SCHED);
	}
}

ಶೂನ್ಯ scx_proxy_donor_ಆರಂಭ(ರಚನೆ rq *rq)
{
}

ಸ್ಥಿರ ಎಣಿಕೆ scx_cpu_preempt_reason
preempt_reason_from_class(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class)
{
	ಆದರೆ (class == &stop_sched_class)
		ಹಿಂತಿರುಗಿ SCX_CPU_PREEMPT_STOP;
	ಆದರೆ (class == &dl_sched_class)
		ಹಿಂತಿರುಗಿ SCX_CPU_PREEMPT_DL;
	ಆದರೆ (class == &rt_sched_class)
		ಹಿಂತಿರುಗಿ SCX_CPU_PREEMPT_RT;
	ಹಿಂತಿರುಗಿ SCX_CPU_PREEMPT_UNKNOWN;
}

ಸ್ಥಿರ ಶೂನ್ಯ switch_class(ರಚನೆ rq *rq, ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ರಚನೆ scx_sched *sch = scx_ಬೇರು_protected_ಜೀವಂತ();
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *ಮುಂದಿನ_class = ಮುಂದಿನ->sched_class;

	ಆದರೆ (!(sch->ops.flags & SCX_OPS_HAS_CPU_PREEMPT))
		ಹಿಂತಿರುಗಿ;

	/*
	 * The callback is conceptually meant to convey that the CPU is no
	 * longer under the control of SCX. Therefore, don't invoke the callback
	 * if the next class is below SCX (in which case the BPF scheduler has
	 * actively decided not to schedule any tasks on the CPU).
	 */
	ಆದರೆ (sched_class_above(&ext_sched_class, ಮುಂದಿನ_class))
		ಹಿಂತಿರುಗಿ;

	/*
	 * At this point we know that SCX was preempted by a higher priority
	 * sched_class, so invoke the ->cpu_release() callback if we have not
	 * done so already. We only send the callback once between SCX being
	 * preempted, and it regaining control of the CPU.
	 *
	 * ->cpu_release() complements ->cpu_acquire(), which is emitted the
	 *  next time that dispatch_one() is invoked.
	 */
	ಆದರೆ (!rq->scx.cpu_released) {
		ಆದರೆ (sch->ops.cpu_release) {
			ರಚನೆ scx_cpu_release_ವಾದಗಳು ವಾದಗಳು = {
				.reason = preempt_reason_from_class(ಮುಂದಿನ_class),
				.task = ಮುಂದಿನ,
			};

			SCX_CALL_OP(sch, cpu_release, rq, cpu_ದ(rq), &ವಾದಗಳು);
		}
		rq->scx.cpu_released = true;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ put_prev_task_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			      ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);
	bool rescue_keep = false;

	/* see kick_sync_wait_bal_cb() */
	smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);

	update_curr_scx(rq);

	/*
	 * If the slice is consumed, protection ends with it. A rescuee
	 * preempted beforehand keeps going, see scx_rescue_keep().
	 */
	ಆದರೆ (!p->scx.ತುಂಡು) {
		ಆದರೆ (unlikely(p == scx_rescuee(rq)))
			rescue_keep = scx_rescue_keep(rq, p);
		ಆದರೆ (!rescue_keep)
			scx_task_ತುಂಡು_ended(rq, p);
	}

	/* see dequeue_task_scx() on why we skip when !QUEUED */
	ಆದರೆ (SCX_HAS_OP(sch, stopping) && (p->scx.flags & SCX_TASK_QUEUED))
		SCX_CALL_OP_TASK(sch, stopping, rq, p, true);

	ಆದರೆ (p->scx.flags & SCX_TASK_QUEUED) {
		ಗಣ_task_runnable(rq, p);

		/*
		 * If @p has slice left and is being put, @p is getting
		 * preempted by a higher priority scheduler class or core-sched
		 * forcing a different task. Leave it at the head of the local
		 * DSQ unless it was an IMMED task. IMMED tasks should not
		 * linger on a busy CPU, reenqueue them to the BPF scheduler.
		 *
		 * An open rescue must keep @p on the local DSQ even if the
		 * scheduler zeroed the slice in ops.stopping() above.
		 */
		ಆದರೆ ((p->scx.ತುಂಡು || unlikely(p == scx_rescuee(rq))) &&
		    !scx_bypassing(sch, cpu_ದ(rq))) {
			ಆದರೆ (p->scx.flags & SCX_TASK_IMMED) {
				p->scx.flags |= SCX_TASK_REENQ_PREEMPTED;
				scx_do_ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, SCX_ENQ_REENQ, -1);
				p->scx.flags &= ~SCX_TASK_REENQ_REASON_ಮುಖವಾಡ;
			} ಇಲ್ಲದಿದ್ದರೆ {
				u64 enq_flags = 0;

				/*
				 * Keep a preempted rescue going. If preempted
				 * by another SCX task, append to the local DSQ,
				 * see scx_rescue_keep().
				 */
				ಆದರೆ (unlikely(p == scx_rescuee(rq))) {
					enq_flags |= SCX_ENQ_IGNORE_CAPS;
					ಆದರೆ (!rescue_keep)
						enq_flags |= SCX_ENQ_ತಲೆ;
				} ಇಲ್ಲದಿದ್ದರೆ {
					enq_flags |= SCX_ENQ_ತಲೆ;
				}

				scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, rq, &rq->scx.local_dsq, p, 0, 0,
						     enq_flags);
			}
			ಹೋಗು switch_class;
		}

		/*
		 * If @p is runnable but we're about to enter a lower
		 * sched_class, %SCX_OPS_ENQ_LAST must be set. Tell
		 * ops.enqueue() that @p is the only one available for this cpu.
		 * This doesn't apply if the baseline access on the CPU is lost.
		 *
		 * Under core scheduling, a pick dispatches only when nothing is
		 * locally runnable and can legitimately go idle with @p still
		 * runnable (see do_pick_task_scx()).
		 */
		ಆದರೆ (ಮುಂದಿನ && sched_class_above(&ext_sched_class, ಮುಂದಿನ->sched_class) &&
		    scx_task_can_stay_on_cpu(rq, p)) {
			WARN_ON_ONCE(!sched_core_enabled(rq) &&
				     !(sch->ops.flags & SCX_OPS_ENQ_ಕೊನೆಯ));
			scx_do_ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, SCX_ENQ_ಕೊನೆಯ, -1);

			/*
			 * A task put on the local DSQ runs, as anywhere else.
			 * Here the insert can't reschedule on its own: the pick
			 * has settled on @next and @p is still curr, so
			 * resched_curr() would flag @p and __schedule() clears
			 * that right after the pick. Flag @next instead.
			 */
			ಆದರೆ (p->scx.dsq == &rq->scx.local_dsq)
				ಗಣ_tsk_need_resched(ಮುಂದಿನ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			scx_do_ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, 0, -1);
		}
	}

switch_class:
	ಆದರೆ (ಮುಂದಿನ && ಮುಂದಿನ->sched_class != &ext_sched_class)
		switch_class(rq, ಮುಂದಿನ);
}

ಸ್ಥಿರ ಶೂನ್ಯ kick_sync_wait_bal_cb(ರಚನೆ rq *rq)
{
	ರಚನೆ scx_kick_syncs __rcu *ks;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ksyncs;
	bool waited;
	s32 cpu;

	/*
	 * This callback is queued and normally flushed within @rq's own
	 * scheduling pass. However, dispatch can drop the rq lock while it sits
	 * queued, and lock takers in that window (the sched class change paths,
	 * the scx task iterator) flush pending balance callbacks on release,
	 * running this one on a foreign CPU whose snapshots are unrelated. The
	 * kicked CPUs are already on their way to advance the kick_syncs being
	 * waited on. Don't get in the way.
	 */
	ಆದರೆ (unlikely(cpu_ದ(rq) != smp_processor_id()))
		ಹಿಂತಿರುಗಿ;

	ks = __this_cpu_read(scx_kick_syncs);
	ksyncs = rcu_dereference_sched(ks)->syncs;

	/*
	 * Drop rq lock and enable IRQs while waiting. IRQs must be enabled
	 * — a target CPU may be waiting for us to process an IPI (e.g. TLB
	 * flush) while we wait for its kick_sync to advance.
	 *
	 * Also, keep advancing our own kick_sync so that new kick_sync waits
	 * targeting us, which can start after we drop the lock, cannot form
	 * cyclic dependencies.
	 */
retry:
	waited = false;
	for_each_cpu(cpu, rq->scx.cpus_ಗಮ್ಯ_sync) {
		/*
		 * smp_load_acquire() pairs with smp_store_release() on
		 * kick_sync updates on the target CPUs.
		 */
		ಆದರೆ (cpu == cpu_ದ(rq) ||
		    smp_ತುಂಬು_acquire(&cpu_rq(cpu)->scx.kick_sync) != ksyncs[cpu]) {
			cpumask_clear_cpu(cpu, rq->scx.cpus_ಗಮ್ಯ_sync);
			ಮುಂದುವರಿ;
		}

		scx_rq_ಬೀಗ_drop(rq);
		raw_spin_rq_unlock_irq(rq);
		ಆಗ (READ_ONCE(cpu_rq(cpu)->scx.kick_sync) == ksyncs[cpu]) {
			smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);
			cpu_relax();
		}
		raw_spin_rq_ಬೀಗ_irq(rq);
		waited = true;
	}

	ಆದರೆ (waited)
		ಹೋಗು retry;
}

ಸ್ಥಿರ ರಚನೆ task_struct *ಮೊದಲ_local_task(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ list_ಮೊದಲ_ನಮೂದು_or_null(&rq->scx.local_dsq.list,
					ರಚನೆ task_struct, scx.dsq_list.ಗಂಟು);
}

/*
 * Run dispatch and queue the follow-up work for a pick.
 */
ಸ್ಥಿರ ಎಣಿಕೆ scx_dsp_verdict dispatch_pick(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf,
					  ರಚನೆ task_struct *prev)
{
	ಎಣಿಕೆ scx_dsp_verdict verdict;

	rq_unpin_ಬೀಗ(rq, rf);
	verdict = dispatch_one(rq, prev);
	rq_repin_ಬೀಗ(rq, rf);
	maybe_ಸರತಿ_balance_callback(rq);

	/*
	 * Defer to a balance callback which can drop rq lock and enable IRQs.
	 * Waiting directly in the pick path would deadlock against CPUs sending
	 * us IPIs (e.g. TLB flushes) while we wait for them.
	 */
	ಆದರೆ (unlikely(rq->scx.kick_sync_pending)) {
		rq->scx.kick_sync_pending = false;
		ಸರತಿ_balance_callback(rq, &rq->scx.kick_sync_bal_cb,
				       kick_sync_wait_bal_cb);
	}

	ಹಿಂತಿರುಗಿ verdict;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
/*
 * Dispatch for a pick when core scheduling is enabled. The selection picks for
 * all SMT siblings and the rq_i->core_pick state it builds must stay atomic
 * throughout. If the dispatch released the rq lock, anything can have happened
 * in between - return %SCX_DSP_RETRY to restart the selection against current
 * state.
 */
ಸ್ಥಿರ ಎಣಿಕೆ scx_dsp_verdict dispatch_core_pick(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf,
					       ರಚನೆ task_struct *prev)
{
	ಎಣಿಕೆ scx_dsp_verdict verdict;
	u32 seq = rq->scx.ಬೀಗ_drop_seq;

	/* another dispatch is in flight on @rq, let that handle it */
	ಆದರೆ (rq->scx.flags & SCX_RQ_ಒಳ_DISPATCH)
		ಹಿಂತಿರುಗಿ SCX_DSP_NONE;

	rq_unpin_ಬೀಗ(rq, rf);

	verdict = dispatch_one(rq, prev);

	ಆದರೆ (cpu_ದ(rq) == smp_processor_id()) {
		maybe_ಸರತಿ_balance_callback(rq);

		/* see dispatch_pick() */
		ಆದರೆ (unlikely(rq->scx.kick_sync_pending)) {
			rq->scx.kick_sync_pending = false;
			ಸರತಿ_balance_callback(rq, &rq->scx.kick_sync_bal_cb,
					       kick_sync_wait_bal_cb);
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(rq->scx.flags & SCX_RQ_BAL_CB_PENDING)) {
		/*
		 * Balance callbacks must run in the context that queued them,
		 * so they can't be queued on another CPU's rq. Run the deferred
		 * work directly instead.
		 */
		rq->scx.flags &= ~SCX_RQ_BAL_CB_PENDING;
		ಸತತ_deferred(rq);
	}

	rq_repin_ಬೀಗ(rq, rf);

	/* if dispatch_one() released the rq lock, restart the selection */
	ಆದರೆ (rq->scx.ಬೀಗ_drop_seq != seq)
		ಹಿಂತಿರುಗಿ SCX_DSP_RETRY;

	ಹಿಂತಿರುಗಿ verdict;
}
#ಇಲ್ಲದಿದ್ದರೆ	/* CONFIG_SCHED_CORE */
ಸ್ಥಿರ ಎಣಿಕೆ scx_dsp_verdict dispatch_core_pick(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf,
					       ರಚನೆ task_struct *prev)
{
	ಹಿಂತಿರುಗಿ SCX_DSP_NONE;
}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_SCHED_CORE */

ಸ್ಥಿರ ರಚನೆ task_struct *
do_pick_task_scx(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf, bool force_scx)
{
	ರಚನೆ task_struct *prev = rq->curr;
	ಎಣಿಕೆ scx_dsp_verdict verdict;
	ರಚನೆ task_struct *p;

	/* see kick_sync_wait_bal_cb() */
	smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);

	rq_modified_begin(rq, &ext_sched_class);

	ಆದರೆ (sched_core_enabled(rq))
		verdict = dispatch_core_pick(rq, rf, prev);
	ಇಲ್ಲದಿದ್ದರೆ
		verdict = dispatch_pick(rq, rf, prev);

	ಆದರೆ (verdict == SCX_DSP_RETRY)
		ಹಿಂತಿರುಗಿ RETRY_TASK;

	/*
	 * If any higher-priority sched class enqueued a runnable task on this
	 * rq during dispatch_one(), abort and return RETRY_TASK, so that the
	 * scheduler loop can restart.
	 *
	 * If @force_scx is true, always try to pick a SCHED_EXT task,
	 * regardless of any higher-priority sched classes activity.
	 */
	ಆದರೆ (!force_scx && rq_modified_above(rq, &ext_sched_class))
		ಹಿಂತಿರುಗಿ RETRY_TASK;

	/*
	 * If we're keeping @prev, replenish slice if necessary and keep running
	 * @prev. Otherwise, pop the first one from the local DSQ.
	 */
	ಆದರೆ (verdict == SCX_DSP_PREV) {
		p = prev;
		ಆದರೆ (!p->scx.ತುಂಡು) {
			/* the slice is consumed, protection ends */
			scx_task_ತುಂಡು_ended(rq, p);
			refill_task_ತುಂಡು_dfl(scx_task_sched(p), p);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		p = ಮೊದಲ_local_task(rq);
		ಆದರೆ (!p)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;

		ಆದರೆ (unlikely(!p->scx.ತುಂಡು) && scx_task_can_stay_on_cpu(rq, p)) {
			ರಚನೆ scx_sched *sch = scx_task_sched(p);

			ಆದರೆ (!scx_bypassing(sch, cpu_ದ(rq)) &&
			    !sch->warned_zero_ತುಂಡು) {
				printk_deferred(KERN_WARNING "sched_ext: %s[%d] has zero slice in %s()\n",
						p->comm, p->pid, __func__);
				sch->warned_zero_ತುಂಡು = true;
			}
			refill_task_ತುಂಡು_dfl(sch, p);
		}
	}

	ಹಿಂತಿರುಗಿ p;
}

ಸ್ಥಿರ ರಚನೆ task_struct *pick_task_scx(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಹಿಂತಿರುಗಿ do_pick_task_scx(rq, rf, false);
}

/*
 * Select the next task to run from the ext scheduling class.
 *
 * Use do_pick_task_scx() directly with @force_scx enabled, since the
 * dl_server must always select a sched_ext task.
 */
ಸ್ಥಿರ ರಚನೆ task_struct *
ext_server_pick_task(ರಚನೆ sched_dl_entity *dl_se, ರಚನೆ rq_flags *rf)
{
	ಆದರೆ (!scx_enabled())
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ do_pick_task_scx(dl_se->rq, rf, true);
}

/*
 * Initialize the ext server deadline entity.
 */
ಶೂನ್ಯ ext_server_init(ರಚನೆ rq *rq)
{
	ರಚನೆ sched_dl_entity *dl_se = &rq->ext_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, ext_server_pick_task);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
/**
 * scx_prio_less - Task ordering for core-sched
 * @a: task A
 * @b: task B
 * @in_fi: in forced idle state
 *
 * Core-sched is implemented as an additional scheduling layer on top of the
 * usual sched_class'es and needs to find out the expected task ordering. For
 * SCX, core-sched calls this function to interrogate the task ordering.
 *
 * A pair of tasks owned by one scheduler is ordered by the owner's
 * ops.core_sched_before(). A pair spanning two schedulers is ordered by their
 * nearest common ancestor which implements the op - the one case where the op
 * is called on tasks that the scheduler delegated to its sub-schedulers and may
 * not be scheduling anymore.
 *
 * When neither applies, or the deciding scheduler is bypassing on either task's
 * CPU, the default ordering runs the task which has been waiting longer first.
 * A running task counts as the most recently serviced and orders after every
 * waiting task. Waiting tasks are compared by @p->scx.runnable_at.
 *
 * Return: %true if @a should run after @b.
 */
bool scx_prio_less(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *a, ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *b,
		   bool ಒಳ_fi)
{
	ರಚನೆ scx_sched *sch_a = scx_task_sched(a);
	ರಚನೆ scx_sched *sch_b = scx_task_sched(b);
	ರಚನೆ scx_sched *sch = ಖಾಲಿ;
	bool a_running, b_running;

	ಆದರೆ (sch_a == sch_b) {
		ಆದರೆ (SCX_HAS_OP(sch_a, core_sched_before))
			sch = sch_a;
	} ಇಲ್ಲದಿದ್ದರೆ {
		s32 level;

		ಗಾಗಿ (level = min(sch_a->level, sch_b->level); level >= 0; level--) {
			ರಚನೆ scx_sched *anc = sch_a->ancestors[level];

			ಆದರೆ (anc == sch_b->ancestors[level] &&
			    SCX_HAS_OP(anc, core_sched_before)) {
				sch = anc;
				ನಿಲ್ಲಿಸು;
			}
		}
	}

	/*
	 * scx_prio_less() returns whether @a should run after @b while
	 * ops.core_sched_before() returns whether its first argument should run
	 * before the second. Swap the arguments.
	 *
	 * The const qualifiers are dropped from task_struct pointers when
	 * calling ops.core_sched_before(). Accesses are controlled by the
	 * verifier.
	 */
	ಆದರೆ (sch && !scx_bypassing(sch, task_cpu(a)) && !scx_bypassing(sch, task_cpu(b)))
		ಹಿಂತಿರುಗಿ SCX_CALL_OP_2TASKS_RET(sch, core_sched_before, task_rq(a),
					      (ರಚನೆ task_struct *)b,
					      (ರಚನೆ task_struct *)a);

	/*
	 * runnable_at is refreshed only on enqueue, so a task which keeps
	 * occupying its CPU carries a stale stamp. A running task is the most
	 * recently serviced whatever its stamp says. Order it after every
	 * waiting task.
	 */
	a_running = a->on_cpu;
	b_running = b->on_cpu;
	ಆದರೆ (a_running != b_running)
		ಹಿಂತಿರುಗಿ a_running;

	ಹಿಂತಿರುಗಿ ಸಮಯ_after(a->scx.runnable_at, b->scx.runnable_at);
}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_SCHED_CORE */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_task_rq_scx(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prev_cpu, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);
	bool bypassing;

	/*
	 * sched_exec() calls with %WF_EXEC when @p is about to exec(2) as it
	 * can be a good migration opportunity with low cache and memory
	 * footprint. Returning a CPU different than @prev_cpu triggers
	 * immediate rq migration. However, for SCX, as the current rq
	 * association doesn't dictate where the task is going to run, this
	 * doesn't fit well. If necessary, we can later add a dedicated method
	 * which can decide to preempt self to force it through the regular
	 * scheduling path.
	 */
	ಆದರೆ (unlikely(wake_flags & WF_EXEC))
		ಹಿಂತಿರುಗಿ prev_cpu;

	bypassing = scx_bypassing(sch, task_cpu(p));
	ಆದರೆ (likely(SCX_HAS_OP(sch, select_cpu)) && !bypassing) {
		s32 cpu;
		ರಚನೆ task_struct **ddsp_taskp;

		ddsp_taskp = this_cpu_ptr(&ನೇರ_dispatch_task);
		WARN_ON_ONCE(*ddsp_taskp);
		*ddsp_taskp = p;

		this_rq()->scx.ಒಳ_select_cpu = true;
		cpu = SCX_CALL_OP_TASK_RET(sch, select_cpu, ಖಾಲಿ, p,
					   scx_cpu_arg(prev_cpu), wake_flags);
		cpu = scx_cpu_ret(sch, cpu);
		this_rq()->scx.ಒಳ_select_cpu = false;
		p->scx.selected_cpu = cpu;
		*ddsp_taskp = ಖಾಲಿ;
		ಆದರೆ (scx_cpu_valid(sch, cpu, "from ops.select_cpu()"))
			ಹಿಂತಿರುಗಿ cpu;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ prev_cpu;
	} ಇಲ್ಲದಿದ್ದರೆ {
		s32 cpu;

		/*
		 * While bypassing, the enqueue path routes @p to a bypass DSQ
		 * without consulting the direct-dispatch target, making the
		 * default selection pointless. It doesn't work anyway when the
		 * scheduler does its own idle tracking and the built-in idle
		 * cpumasks are not updated. Leave @p on @prev_cpu.
		 */
		ಆದರೆ (bypassing) {
			__scx_add_event(sch, SCX_EV_BYPASS_DISPATCH, 1);
			p->scx.selected_cpu = prev_cpu;
			ಹಿಂತಿರುಗಿ prev_cpu;
		}

		cpu = scx_select_cpu_dfl(p, prev_cpu, wake_flags, ಖಾಲಿ, 0);
		ಆದರೆ (cpu >= 0) {
			/*
			 * Carry the slice refill and let the insertion commit
			 * it under rq lock. See the write rules.
			 */
			__scx_add_event(sch, SCX_EV_REFILL_ತುಂಡು_DFL, 1);
			p->scx.ddsp_ತುಂಡು = READ_ONCE(sch->ತುಂಡು_dfl);
			p->scx.ddsp_enq_flags = SCX_ENQ_ತುಂಡು_DFL;
			p->scx.ddsp_dsq_id = SCX_DSQ_LOCAL;
		} ಇಲ್ಲದಿದ್ದರೆ {
			cpu = prev_cpu;
		}
		p->scx.selected_cpu = cpu;

		ಹಿಂತಿರುಗಿ cpu;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ task_woken_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಸತತ_deferred(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_cpus_allowed_scx(ರಚನೆ task_struct *p,
				 ರಚನೆ affinity_context *ac)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);

	ಗಣ_cpus_allowed_common(p, ac);

	ಆದರೆ (task_dead_and_ಮುಗಿದ(p))
		ಹಿಂತಿರುಗಿ;

	/*
	 * The effective cpumask is stored in @p->cpus_ptr which may temporarily
	 * differ from the configured one in @p->cpus_mask. Always tell the bpf
	 * scheduler the effective one.
	 *
	 * Fine-grained memory write control is enforced by BPF making the const
	 * designation pointless. Cast it away when calling the operation.
	 *
	 * The cid form receives the initial mask when the task is enabled and
	 * hears about changes only afterwards, see struct scx_enable_args.
	 */
	ಆದರೆ (SCX_HAS_OP(sch, ಗಣ_cpumask) &&
	    (!scx_is_cid_type() || scx_get_task_state(p) == SCX_TASK_ENABLED))
		scx_call_op_ಗಣ_cpumask(sch, task_rq(p), p, (ರಚನೆ cpumask *)p->cpus_ptr);
}

ಸ್ಥಿರ ಶೂನ್ಯ handle_hotplug(ರಚನೆ rq *rq, bool online)
{
	ರಚನೆ scx_sched *sch = scx_ಬೇರು_protected();
	s32 cpu = cpu_ದ(rq);
	s32 cpu_or_cid = cpu;

	atomic_long_inc(&scx_hotplug_seq);

	/*
	 * scx_root updates are protected by cpus_read_lock() and will stay
	 * stable here. Note that we can't depend on scx_enabled() test as the
	 * hotplug ops need to be enabled before __scx_enabled is set.
	 */
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (scx_enabled())
		scx_idle_update_selcpu_topology(&sch->ops);

	ಆದರೆ (online)
		scx_online_ecaps(rq);
	ಇಲ್ಲದಿದ್ದರೆ
		scx_offline_ecaps(rq);

	/*
	 * The tables can't be retired while this function is running as the
	 * retirement is inside cpus_read_lock. However, scx_cpu_arg() is
	 * awkward here as the tables can be NULL after root enable failure and
	 * lockdep would trigger without surrounding rcu_read_lock(). Open code
	 * the translation. If the table is NULL, the ops are also cleared and
	 * @cpu_or_cid goes unused.
	 */
	ಆದರೆ (scx_is_cid_type()) {
		s16 *tbl = rcu_dereference_check(scx_cpu_ಗಮ್ಯ_cid_tbl,
						 lockdep_is_cpus_held());

		ಆದರೆ (tbl) {
			ರಚನೆ scx_sched *pos;

			cpu_or_cid = tbl[cpu];

			guard(raw_spinlock_irqsave)(&scx_sched_ಬೀಗ);
			list_for_each_ನಮೂದು(pos, &scx_sched_all, all) {
				ರಚನೆ scx_cmask *ಮುಖವಾಡ = pos->online_cmask;

				ಆದರೆ (ಮುಖವಾಡ)
					__assign_bit(cpu_or_cid, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *)ಮುಖವಾಡ->ಬಿಟ್ಟುಗಳು,
						     online);
			}
		}
	}

	ಆದರೆ (online && SCX_HAS_OP(sch, cpu_online))
		SCX_CALL_OP(sch, cpu_online, ಖಾಲಿ, cpu_or_cid);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!online && SCX_HAS_OP(sch, cpu_offline))
		SCX_CALL_OP(sch, cpu_offline, ಖಾಲಿ, cpu_or_cid);
	ಇಲ್ಲದಿದ್ದರೆ
		scx_exit(sch, SCX_EXIT_UNREG_KERN,
			 SCX_ECODE_ACT_RESTART | SCX_ECODE_RSN_HOTPLUG,
			 "cpu %d going %s, exiting scheduler", cpu,
			 online ? "online" : "offline");
}

ಶೂನ್ಯ scx_rq_activate(ರಚನೆ rq *rq)
{
	handle_hotplug(rq, true);
}

ಶೂನ್ಯ scx_rq_deactivate(ರಚನೆ rq *rq)
{
	handle_hotplug(rq, false);
}

ಸ್ಥಿರ ಶೂನ್ಯ rq_online_scx(ರಚನೆ rq *rq)
{
	rq->scx.flags |= SCX_RQ_ONLINE;
}

ಸ್ಥಿರ ಶೂನ್ಯ rq_offline_scx(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *p, *n;

	rq->scx.flags &= ~SCX_RQ_ONLINE;

	/* sched domain rebuilds call rq_offline with the CPU staying alive */
	ಆದರೆ (cpu_ಸಕ್ರಿಯ(cpu_ದ(rq)))
		ಹಿಂತಿರುಗಿ;

	scx_rescue_flush(rq);

	/*
	 * An offline CPU no longer calls ops.dispatch(). Re-enqueue its tasks
	 * onto the local DSQ so that they run here and balance_push() moves
	 * them off.
	 */
	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ_ಹಿಮ್ಮುಖ(p, n, &rq->scx.runnable_list, scx.runnable_ಗಂಟು) {
		ಆದರೆ (p->scx.dsq == &rq->scx.local_dsq)
			ಮುಂದುವರಿ;
		guard(sched_change)(p, ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE | ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK);
	}
}

ಸ್ಥಿರ bool check_rq_for_timeouts(ರಚನೆ rq *rq)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ task_struct *p;
	ರಚನೆ rq_flags rf;
	bool timed_ಹೊರಹೋಗು = false;

	rq_ಬೀಗ_irqsave(rq, &rf);
	sch = rcu_dereference_bh(scx_ಬೇರು);
	ಆದರೆ (unlikely(!sch))
		ಹೋಗು ಹೊರಹೋಗು_unlock;

	list_for_each_ನಮೂದು(p, &rq->scx.runnable_list, scx.runnable_ಗಂಟು) {
		ರಚನೆ scx_sched *sch = scx_task_sched(p);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಕೊನೆಯ_runnable = p->scx.runnable_at;

		ಆದರೆ (unlikely(ಸಮಯ_after(jiffies,
					ಕೊನೆಯ_runnable + READ_ONCE(sch->watchdog_timeout)))) {
			ರಚನೆ scx_dispatch_q *dsq = READ_ONCE(p->scx.dsq);
			u32 dur_ms = jiffies_ಗಮ್ಯ_msecs(jiffies - ಕೊನೆಯ_runnable);

			/*
			 * A task can be stuck on a DSQ that a sched other than
			 * its owner is responsible for draining, e.g. an
			 * ancestor's bypass DSQ while the owner is bypassing.
			 * Blame the drainer. The local DSQ is consumed by the
			 * cpu itself and keeps blame on the owner.
			 */
			ಆದರೆ (dsq && dsq->sched && dsq->id != SCX_DSQ_LOCAL)
				sch = dsq->sched;

			__scx_exit(sch, SCX_EXIT_ERROR_STALL, 0, cpu_ದ(rq),
				   "%s[%d] failed to run for %u.%03us",
				   p->comm, p->pid, dur_ms / 1000,
				   dur_ms % 1000);
			timed_ಹೊರಹೋಗು = true;
			ನಿಲ್ಲಿಸು;
		}
	}
ಹೊರಹೋಗು_unlock:
	rq_unlock_irqrestore(rq, &rf);
	ಹಿಂತಿರುಗಿ timed_ಹೊರಹೋಗು;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_watchdog_workfn(ರಚನೆ work_struct *work)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ intv;
	ಪೂರ್ಣಾಂಕ cpu;

	WRITE_ONCE(scx_watchdog_timestamp, jiffies);

	for_each_online_cpu(cpu) {
		ಆದರೆ (unlikely(check_rq_for_timeouts(cpu_rq(cpu))))
			ನಿಲ್ಲಿಸು;

		cond_resched();
	}

	intv = READ_ONCE(scx_watchdog_interval);
	ಆದರೆ (intv < ULONG_MAX)
		ಸರತಿ_delayed_work(system_dfl_wq, ಗಮ್ಯ_delayed_work(work), intv);
}

ಶೂನ್ಯ scx_tick(ರಚನೆ rq *rq)
{
	ರಚನೆ scx_sched *ಬೇರು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಕೊನೆಯ_check;

	ಆದರೆ (!scx_enabled())
		ಹಿಂತಿರುಗಿ;

	ಬೇರು = rcu_dereference_bh(scx_ಬೇರು);
	ಆದರೆ (unlikely(!ಬೇರು))
		ಹಿಂತಿರುಗಿ;

	ಕೊನೆಯ_check = READ_ONCE(scx_watchdog_timestamp);
	ಆದರೆ (unlikely(ಸಮಯ_after(jiffies,
				ಕೊನೆಯ_check + READ_ONCE(ಬೇರು->watchdog_timeout)))) {
		u32 dur_ms = jiffies_ಗಮ್ಯ_msecs(jiffies - ಕೊನೆಯ_check);

		scx_exit(ಬೇರು, SCX_EXIT_ERROR_STALL, 0,
			 "watchdog failed to check in for %u.%03us",
			 dur_ms / 1000, dur_ms % 1000);
	}

	update_other_ತುಂಬು_avgs(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_tick_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr, ಪೂರ್ಣಾಂಕ queued)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(curr);

	update_curr_scx(rq);

	/*
	 * While disabling, always resched as we can't trust the slice
	 * management.
	 */
	ಆದರೆ (scx_bypassing(sch, cpu_ದ(rq)))
		scx_ಗಣ_task_ತುಂಡು(curr, 0);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (SCX_HAS_OP(sch, tick))
		SCX_CALL_OP_TASK(sch, tick, rq, curr);

	ಆದರೆ (!curr->scx.ತುಂಡು)
		resched_curr(rq);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
ಸ್ಥಿರ ರಚನೆ cgroup *tg_cgrp(ರಚನೆ task_ಗುಂಪು *tg)
{
	/*
	 * If CGROUP_SCHED is disabled, @tg is NULL. If @tg is an autogroup,
	 * @tg->css.cgroup is NULL. In both cases, @tg can be treated as the
	 * root cgroup.
	 */
	ಆದರೆ (tg && tg->css.cgroup)
		ಹಿಂತಿರುಗಿ tg->css.cgroup;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ &cgrp_dfl_ಬೇರು.cgrp;
}

#ವ್ಯಾಖ್ಯಾನಿಸು SCX_INIT_TASK_ವಾದಗಳು_CGROUP(cgrp)		.cgroup = (cgrp),

#ಇಲ್ಲದಿದ್ದರೆ	/* CONFIG_EXT_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿಸು SCX_INIT_TASK_ವಾದಗಳು_CGROUP(cgrp)

#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_GROUP_SCHED */

/**
 * __scx_init_task - Initialize a task for a sched
 * @sch: sched to initialize @p for
 * @p: task of interest
 * @cgrp: cgroup @p is joining, %NULL for @p's current task_group's cgroup
 * @fork: %true if @p is being forked
 *
 * Pre-commit cgroup migration passes @cgrp explicitly as @p's task_group
 * still reflects the source.
 *
 * Return 0 on success, -errno on failure.
 */
ಪೂರ್ಣಾಂಕ __scx_init_task(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p,
		    ರಚನೆ cgroup *cgrp, bool fork)
{
	ಪೂರ್ಣಾಂಕ ret;

	p->scx.disallow = false;

	ಆದರೆ (SCX_HAS_OP(sch, init_task)) {
		ರಚನೆ scx_init_task_ವಾದಗಳು ವಾದಗಳು = {
			SCX_INIT_TASK_ವಾದಗಳು_CGROUP(cgrp ?: tg_cgrp(task_ಗುಂಪು(p)))
			.fork = fork,
		};

		ret = SCX_CALL_OP_RET(sch, init_task, ಖಾಲಿ, p, &ವಾದಗಳು);
		ಆದರೆ (unlikely(ret)) {
			ret = scx_ops_sanitize_err(sch, "init_task", ret);
			ಹಿಂತಿರುಗಿ ret;
		}
	}

	ಆದರೆ (p->scx.disallow) {
		ಆದರೆ (unlikely(scx_ಪೋಷಕ(sch))) {
			scx_error(sch, "non-root ops.init_task() set task->scx.disallow for %s[%d]",
				  p->comm, p->pid);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(fork)) {
			scx_error(sch, "ops.init_task() set task->scx.disallow for %s[%d] during fork",
				  p->comm, p->pid);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(scx_enable_state() != SCX_ENABLING)) {
			scx_error(sch, "ops.init_task() set task->scx.disallow for %s[%d] outside the enable path",
				  p->comm, p->pid);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ rq *rq;
			ರಚನೆ rq_flags rf;

			rq = task_rq_ಬೀಗ_2(p, &rf);

			/*
			 * We're in the load path and @p->policy will be applied
			 * right after. Reverting @p->policy here and rejecting
			 * %SCHED_EXT transitions from scx_check_setscheduler()
			 * guarantees that if ops.init_task() sets @p->disallow,
			 * @p can never be in SCX.
			 */
			ಆದರೆ (p->policy == SCHED_EXT) {
				p->policy = SCHED_ಸಾಮಾನ್ಯ;
				atomic_long_inc(&scx_nr_rejected);
			}

			task_rq_unlock(rq, p, &rf);
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ __scx_enable_task(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p)
{
	ರಚನೆ rq *rq = task_rq(p);
	u32 ತೂಕ;

	lockdep_assert_rq_held(rq);

	/*
	 * Verify the task is not in BPF scheduler's custody. If flag
	 * transitions are consistent, the flag should always be clear
	 * here.
	 */
	WARN_ON_ONCE(p->scx.flags & SCX_TASK_ಒಳ_CUSTODY);

	/*
	 * Set the weight before calling ops.enable() so that the scheduler
	 * doesn't see a stale value if they inspect the task struct.
	 */
	ಆದರೆ (task_has_idle_policy(p))
		ತೂಕ = ತೂಕ_IDLEPRIO;
	ಇಲ್ಲದಿದ್ದರೆ
		ತೂಕ = sched_prio_ಗಮ್ಯ_ತೂಕ[p->static_prio - MAX_RT_PRIO];

	p->scx.ತೂಕ = sched_ತೂಕ_ಗಮ್ಯ_cgroup(ತೂಕ);

	ಆದರೆ (SCX_HAS_OP(sch, enable)) {
		ಆದರೆ (scx_is_cid_type()) {
			ರಚನೆ scx_cmask *cmask = scx_fill_cmask_scratch(sch, p->cpus_ptr);
			ರಚನೆ scx_enable_ವಾದಗಳು ವಾದಗಳು = {
				.cmask_arena_addr = scx_kaddr_ಗಮ್ಯ_arena(sch, cmask),
			};

			SCX_CALL_CID_OP_TASK(sch, enable, rq, p, &ವಾದಗಳು);
		} ಇಲ್ಲದಿದ್ದರೆ {
			SCX_CALL_OP_TASK(sch, enable, rq, p);
		}
	}

	/*
	 * The initial mask also goes out through set_cmask() so a scheduler can
	 * track affinity there alone, and before set_weight() so that the mask
	 * is in place when weight-dependent state is derived, see struct
	 * scx_enable_args.
	 */
	ಆದರೆ (scx_is_cid_type() && SCX_HAS_OP(sch, ಗಣ_cpumask))
		scx_call_op_ಗಣ_cpumask(sch, rq, p, p->cpus_ptr);

	ಆದರೆ (SCX_HAS_OP(sch, ಗಣ_ತೂಕ))
		SCX_CALL_OP_TASK(sch, ಗಣ_ತೂಕ, rq, p, p->scx.ತೂಕ);
}

ಶೂನ್ಯ scx_enable_task(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p)
{
	__scx_enable_task(sch, p);
	scx_ಗಣ_task_state(p, SCX_TASK_ENABLED);
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_disable_task(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p)
{
	ರಚನೆ rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(scx_get_task_state(p) != SCX_TASK_ENABLED);

	clear_ನೇರ_dispatch(p);

	ಆದರೆ (SCX_HAS_OP(sch, disable))
		SCX_CALL_OP_TASK(sch, disable, rq, p);
	scx_ಗಣ_task_state(p, SCX_TASK_READY);

	/*
	 * Reset the SCX-managed fields when @p leaves the BPF scheduler's
	 * control, after ops.disable() has observed their final values.
	 */
	p->scx.dsq_vtime = 0;
	scx_task_ತುಂಡು_ended(rq, p);
	scx_ಗಣ_task_ತುಂಡು(p, 0);
	p->scx.reenq_cnt = 0;

	/*
	 * Verify the task is not in BPF scheduler's custody. If flag
	 * transitions are consistent, the flag should always be clear
	 * here.
	 */
	WARN_ON_ONCE(p->scx.flags & SCX_TASK_ಒಳ_CUSTODY);
}

ಶೂನ್ಯ __scx_disable_and_exit_task(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p)
{
	ರಚನೆ scx_exit_task_ವಾದಗಳು ವಾದಗಳು = {
		.cancelled = false,
	};

	lockdep_assert_held(&p->pi_ಬೀಗ);
	lockdep_assert_rq_held(task_rq(p));

	ಆಯ್ಕೆ (scx_get_task_state(p)) {
	ಸ್ಥಿತಿ SCX_TASK_NONE:
		ಹಿಂತಿರುಗಿ;
	ಸ್ಥಿತಿ SCX_TASK_INIT:
		ವಾದಗಳು.cancelled = true;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_TASK_READY:
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_TASK_ENABLED:
		scx_disable_task(sch, p);
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		WARN_ON_ONCE(true);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (SCX_HAS_OP(sch, exit_task))
		SCX_CALL_OP_TASK(sch, exit_task, task_rq(p), p, &ವಾದಗಳು);
}

/*
 * Undo a completed __scx_init_task(sch, p, false) when scx_enable_task() never
 * ran. The task state has not been transitioned, so this mirrors the
 * SCX_TASK_INIT branch in __scx_disable_and_exit_task().
 */
ಶೂನ್ಯ scx_sub_init_cancel_task(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p)
{
	ರಚನೆ scx_exit_task_ವಾದಗಳು ವಾದಗಳು = { .cancelled = true };

	lockdep_assert_held(&p->pi_ಬೀಗ);
	lockdep_assert_rq_held(task_rq(p));

	/* @p was never associated with @sch, dispatch on the explicit @sch */
	ಆದರೆ (SCX_HAS_OP(sch, exit_task))
		__SCX_CALL_OP_TASK(sch, ops, exit_task, task_rq(p), p, &ವಾದಗಳು);
}

ಶೂನ್ಯ scx_disable_and_exit_task(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p)
{
	__scx_disable_and_exit_task(sch, p);

	/*
	 * If set, @p exited between __scx_init_task() and scx_enable_task() in
	 * scx_sub_enable() and is initialized for both the associated sched and
	 * its parent. Exit for the child too - scx_enable_task() never ran for
	 * it, so undo only init_task. The flag is only set on the sub-enable
	 * path, so it's always clear when @p arrives here in %SCX_TASK_NONE.
	 */
	ಆದರೆ (p->scx.flags & SCX_TASK_SUB_INIT) {
		ಆದರೆ (!WARN_ON_ONCE(!scx_enabling_sub_sched))
			scx_sub_init_cancel_task(scx_enabling_sub_sched, p);
		p->scx.flags &= ~SCX_TASK_SUB_INIT;
	}

	scx_ಗಣ_task_sched(p, ಖಾಲಿ);
	scx_ಗಣ_task_state(p, SCX_TASK_NONE);
}

ಶೂನ್ಯ init_scx_entity(ರಚನೆ sched_ext_entity *scx)
{
	memset(scx, 0, ಗಾತ್ರ(*scx));
	INIT_LIST_ತಲೆ(&scx->dsq_list.ಗಂಟು);
	RB_CLEAR_ಗಂಟು(&scx->dsq_priq);
	scx->sticky_cpu = -1;
	scx->holding_cpu = -1;
	scx->runnable_cpu = -1;
	INIT_LIST_ತಲೆ(&scx->runnable_ಗಂಟು);
	scx->runnable_at = jiffies;
	scx->ddsp_dsq_id = SCX_DSQ_INVALID;
	scx->ತುಂಡು = SCX_ತುಂಡು_DFL;
}

/* See scx_tid_alloc / scx_tid_cursor. */
ಸ್ಥಿರ u64 scx_alloc_tid(ಶೂನ್ಯ)
{
	ರಚನೆ scx_tid_alloc *ta;

	guard(preempt)();
	ta = this_cpu_ptr(&scx_tid_alloc);

	ಆದರೆ (unlikely(ta->ಮುಂದಿನ >= ta->ಕೊನೆ)) {
		ta->ಮುಂದಿನ = atomic64_fetch_ಕೂಡಿಸು(SCX_TID_CHUNK, &scx_tid_cursor);
		ta->ಕೊನೆ = ta->ಮುಂದಿನ + SCX_TID_CHUNK;
	}
	ಹಿಂತಿರುಗಿ ta->ಮುಂದಿನ++;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_tid_hash_ಸೇರಿಸು(ರಚನೆ task_struct *p)
{
	ಪೂರ್ಣಾಂಕ ret;

	lockdep_assert_held(&scx_tasks_ಬೀಗ);

	ret = rhashtable_lookup_ಸೇರಿಸು_fast(&scx_tid_hash,
					    &p->scx.tid_hash_ಗಂಟು,
					    scx_tid_hash_params);
	WARN_ON_ONCE(ret);
}

ಶೂನ್ಯ scx_pre_fork(ರಚನೆ task_struct *p)
{
	/*
	 * BPF scheduler enable/disable paths want to be able to iterate and
	 * update all tasks which can become complex when racing forks. As
	 * enable/disable are very cold paths, let's use a percpu_rwsem to
	 * exclude forks.
	 */
	percpu_ಕೆಳಕ್ಕೆ_read(&scx_fork_rwsem);
}

ಪೂರ್ಣಾಂಕ scx_fork(ರಚನೆ task_struct *p, ರಚನೆ kernel_clone_ವಾದಗಳು *kargs)
{
	s32 ret;

	percpu_rwsem_assert_held(&scx_fork_rwsem);

	p->scx.tid = scx_alloc_tid();

	ಆದರೆ (scx_init_task_enabled) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		ರಚನೆ scx_sched *sch = scx_cgroup_sched(kargs->cset->dfl_cgrp);
#ಇಲ್ಲದಿದ್ದರೆ
		ರಚನೆ scx_sched *sch = scx_ಬೇರು_protected_ಜೀವಂತ();
#ಆದರೆ_ಅಂತ್ಯ
		scx_ಗಣ_task_state(p, SCX_TASK_INIT_BEGIN);
		ret = __scx_init_task(sch, p, ಖಾಲಿ, true);
		ಆದರೆ (unlikely(ret)) {
			scx_ಗಣ_task_state(p, SCX_TASK_NONE);
			ಹಿಂತಿರುಗಿ ret;
		}
		scx_ಗಣ_task_state(p, SCX_TASK_INIT);
		scx_ಗಣ_task_sched(p, sch);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ scx_post_fork(ರಚನೆ task_struct *p)
{
	ಆದರೆ (scx_init_task_enabled) {
		scx_ಗಣ_task_state(p, SCX_TASK_READY);

		/*
		 * Enable the task immediately if it's running on sched_ext.
		 * Otherwise, it'll be enabled in switching_to_scx() if and
		 * when it's ever configured to run with a SCHED_EXT policy.
		 */
		ಆದರೆ (p->sched_class == &ext_sched_class) {
			ರಚನೆ rq_flags rf;
			ರಚನೆ rq *rq;

			rq = task_rq_ಬೀಗ_2(p, &rf);
			scx_enable_task(scx_task_sched(p), p);
			task_rq_unlock(rq, p, &rf);
		}
	}

	scoped_guard(raw_spinlock_irq, &scx_tasks_ಬೀಗ) {
		list_ಕೂಡಿಸು_ಬಾಲ(&p->scx.tasks_ಗಂಟು, &scx_tasks);
		ಆದರೆ (scx_tid_ಗಮ್ಯ_task_enabled())
			scx_tid_hash_ಸೇರಿಸು(p);
	}

	percpu_ಮೇಲೆ_read(&scx_fork_rwsem);
}

ಶೂನ್ಯ scx_cancel_fork(ರಚನೆ task_struct *p)
{
	ಆದರೆ (scx_init_task_enabled) {
		ರಚನೆ rq *rq;
		ರಚನೆ rq_flags rf;

		rq = task_rq_ಬೀಗ_2(p, &rf);
		WARN_ON_ONCE(scx_get_task_state(p) >= SCX_TASK_READY);
		scx_disable_and_exit_task(scx_task_sched(p), p);
		task_rq_unlock(rq, p, &rf);
	}

	percpu_ಮೇಲೆ_read(&scx_fork_rwsem);
}

/**
 * task_dead_and_done - Is a task dead and done running?
 * @p: target task
 *
 * Once sched_ext_dead() removes the dead task from scx_tasks and exits it, the
 * task no longer exists from SCX's POV. However, certain sched_class ops may be
 * invoked on these dead tasks leading to failures - e.g. sched_setscheduler()
 * may try to switch a task which finished sched_ext_dead() back into SCX
 * triggering invalid SCX task state transitions and worse.
 *
 * Once a task has finished the final switch, sched_ext_dead() is the only thing
 * that needs to happen on the task. Use this test to short-circuit sched_class
 * operations which may be called on dead tasks.
 */
ಸ್ಥಿರ bool task_dead_and_ಮುಗಿದ(ರಚನೆ task_struct *p)
{
	ರಚನೆ rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * In do_task_dead(), a dying task sets %TASK_DEAD with preemption
	 * disabled and __schedule(). If @p has %TASK_DEAD set and off CPU, @p
	 * won't ever run again.
	 */
	ಹಿಂತಿರುಗಿ unlikely(READ_ONCE(p->__state) == TASK_DEAD) &&
		!task_on_cpu(rq, p);
}

ಶೂನ್ಯ sched_ext_dead(ರಚನೆ task_struct *p)
{
	/*
	 * By the time control reaches here, @p has %TASK_DEAD set, switched out
	 * for the last time and then dropped the rq lock - task_dead_and_done()
	 * should be returning %true nullifying the straggling sched_class ops.
	 * Remove from scx_tasks and exit @p.
	 */
	scoped_guard(raw_spinlock_irqsave, &scx_tasks_ಬೀಗ) {
		list_del_init(&p->scx.tasks_ಗಂಟು);
		ಆದರೆ (scx_tid_ಗಮ್ಯ_task_enabled())
			rhashtable_remove_fast(&scx_tid_hash,
					       &p->scx.tid_hash_ಗಂಟು,
					       scx_tid_hash_params);
	}

	/*
	 * @p is off scx_tasks and wholly ours. scx_root_enable()'s READY ->
	 * ENABLED transitions can't race us. Disable ops for @p.
	 *
	 * %SCX_TASK_DEAD synchronizes against cgroup task iteration - see
	 * scx_task_iter_next_locked(). NONE tasks need no marking: cgroup
	 * iteration is only used from sub-sched paths, which require root
	 * enabled. Root enable transitions every live task to at least READY.
	 *
	 * %INIT_BEGIN means ops.init_task() is running for @p. Don't call
	 * into ops; transition to %DEAD so the post-init recheck unwinds
	 * via scx_sub_init_cancel_task().
	 */
	ಆದರೆ (scx_get_task_state(p) != SCX_TASK_NONE) {
		ರಚನೆ rq_flags rf;
		ರಚನೆ rq *rq;

		rq = task_rq_ಬೀಗ_2(p, &rf);
		ಆದರೆ (scx_get_task_state(p) != SCX_TASK_INIT_BEGIN)
			scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_ಗಣ_task_state(p, SCX_TASK_DEAD);
		task_rq_unlock(rq, p, &rf);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ reweight_task_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ ತುಂಬು_ತೂಕ *lw)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);

	lockdep_assert_rq_held(task_rq(p));

	ಆದರೆ (task_dead_and_ಮುಗಿದ(p))
		ಹಿಂತಿರುಗಿ;

	/*
	 * When switching sched_class away from SCX, reweight_task_scx()
	 * is called _after_ scx_disable_task(). Skip calling ops.set_weight()
	 * since the BPF scheduler may have already forgotten the task in
	 * ops.disable().
	 * p->scx.weight will be recalculated in scx_enable_task() if the task
	 * ever returns to SCX class.
	 */
	ಆದರೆ (scx_get_task_state(p) != SCX_TASK_ENABLED)
		ಹಿಂತಿರುಗಿ;

	p->scx.ತೂಕ = sched_ತೂಕ_ಗಮ್ಯ_cgroup(scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(lw->ತೂಕ));
	ಆದರೆ (SCX_HAS_OP(sch, ಗಣ_ತೂಕ))
		SCX_CALL_OP_TASK(sch, ಗಣ_ತೂಕ, rq, p, p->scx.ತೂಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ prio_ಬದಲಾಯಿತು_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 oldprio)
{
}

ಸ್ಥಿರ ಶೂನ್ಯ switching_ಗಮ್ಯ_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ scx_sched *sch = scx_task_sched(p);

	ಆದರೆ (task_dead_and_ಮುಗಿದ(p))
		ಹಿಂತಿರುಗಿ;

	scx_enable_task(sch, p);

	/*
	 * set_cpus_allowed_scx() is not called while @p is associated with a
	 * different scheduler class. Keep the BPF scheduler up-to-date. The cid
	 * form gets its mask from scx_enable_task().
	 */
	ಆದರೆ (!scx_is_cid_type() && SCX_HAS_OP(sch, ಗಣ_cpumask))
		scx_call_op_ಗಣ_cpumask(sch, rq, p, (ರಚನೆ cpumask *)p->cpus_ptr);
}

ಸ್ಥಿರ ಶೂನ್ಯ switched_from_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (task_dead_and_ಮುಗಿದ(p))
		ಹಿಂತಿರುಗಿ;

	/*
	 * %NONE means SCX is no longer tracking @p at the task level (e.g.
	 * scx_fail_parent() handed @p back to the parent at NONE pending the
	 * parent's own teardown). There is nothing to disable; calling
	 * scx_disable_task() would WARN on the non-%ENABLED state and trigger a
	 * NONE -> READY validation failure.
	 */
	ಆದರೆ (scx_get_task_state(p) == SCX_TASK_NONE)
		ಹಿಂತಿರುಗಿ;

	scx_disable_task(scx_task_sched(p), p);
}

ಸ್ಥಿರ ಶೂನ್ಯ switched_ಗಮ್ಯ_scx(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) {}

ಪೂರ್ಣಾಂಕ scx_check_setscheduler(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ policy)
{
	lockdep_assert_rq_held(task_rq(p));

	/* if disallow, reject transitioning into SCX */
	ಆದರೆ (scx_enabled() && READ_ONCE(p->scx.disallow) &&
	    p->policy != policy && policy == SCHED_EXT)
		ಹಿಂತಿರುಗಿ -EACCES;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ process_ddsp_deferred_locals(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *p;

	lockdep_assert_rq_held(rq);

	/*
	 * Now that @rq can be unlocked, execute the deferred enqueueing of
	 * tasks directly dispatched to the local DSQs of other CPUs. See
	 * direct_dispatch(). Keep popping from the head instead of using
	 * list_for_each_entry_safe() as dispatch_local_dsq() may unlock @rq
	 * temporarily.
	 */
	ಆಗ ((p = list_ಮೊದಲ_ನಮೂದು_or_null(&rq->scx.ddsp_deferred_locals,
				ರಚನೆ task_struct, scx.dsq_list.ಗಂಟು))) {
		ರಚನೆ scx_sched *sch = scx_task_sched(p);
		ರಚನೆ scx_dispatch_q *dsq;
		u64 dsq_id = p->scx.ddsp_dsq_id;
		u64 enq_flags = p->scx.ddsp_enq_flags;
		u64 ತುಂಡು = p->scx.ddsp_ತುಂಡು;
		u64 vtime = p->scx.ddsp_vtime;

		list_del_init(&p->scx.dsq_list.ಗಂಟು);
		clear_ನೇರ_dispatch(p);

		dsq = ಕಂಡುಹಿಡಿ_dsq_for_dispatch(sch, rq, dsq_id, task_cpu(p));
		ಆದರೆ (!WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL))
			dispatch_ಗಮ್ಯ_local_dsq(sch, rq, dsq, p, ತುಂಡು, vtime, enq_flags);
	}
}

/*
 * Determine whether @p should be reenqueued from a local DSQ.
 *
 * @reenq_flags is mutable and accumulates state across the DSQ walk:
 *
 * - %SCX_REENQ_TSR_NOT_FIRST: Set after the first task is visited. "First"
 *   tracks position in the DSQ list, not among IMMED tasks. A non-IMMED task at
 *   the head consumes the first slot.
 *
 * - %SCX_REENQ_TSR_RQ_OPEN: Set by reenq_local() before the walk if
 *   rq_is_open() is true.
 *
 * An IMMED task is kept (returns %false) only if it's the first task in the DSQ
 * AND the current task is done — i.e. it will execute immediately. All other
 * IMMED tasks are reenqueued. This means if a non-IMMED task sits at the head,
 * every IMMED task behind it gets reenqueued.
 *
 * Reenqueued tasks go through ops.enqueue() with %SCX_ENQ_REENQ |
 * %SCX_TASK_REENQ_IMMED. If the BPF scheduler dispatches back to the same local
 * DSQ with %SCX_ENQ_IMMED while the CPU is still unavailable, this triggers
 * another reenq cycle. Repetitions are bounded by %SCX_REENQ_MAX_REPEAT in
 * scx_do_enqueue_task(), which ejects the task's owning scheduler.
 */
ಸ್ಥಿರ bool local_task_should_reenq(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
				    u64 *reenq_flags, u32 *reason)
{
	bool ಮೊದಲ;

	ಮೊದಲ = !(*reenq_flags & SCX_REENQ_TSR_NOT_ಮೊದಲ);
	*reenq_flags |= SCX_REENQ_TSR_NOT_ಮೊದಲ;

	ಆದರೆ (unlikely((p->scx.flags & SCX_TASK_PROTECTED) || p == scx_rescuee(rq)))
		ಹಿಂತಿರುಗಿ false;

	*reason = SCX_TASK_REENQ_KFUNC;

	ಆದರೆ ((p->scx.flags & SCX_TASK_IMMED) &&
	    (!ಮೊದಲ || !(*reenq_flags & SCX_REENQ_TSR_RQ_OPEN))) {
		__scx_add_event(scx_task_sched(p), SCX_EV_REENQ_IMMED, 1);
		*reason = SCX_TASK_REENQ_IMMED;
		ಹಿಂತಿರುಗಿ true;
	}

	ಆದರೆ ((*reenq_flags & SCX_REENQ_CAP_REVOKE) &&
	    scx_task_reenq_on_cap_revoke(rq, p)) {
		*reason = SCX_TASK_REENQ_CAP;
		ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ *reenq_flags & SCX_REENQ_ANY;
}

/*
 * The dispatcher runs ops.dequeue() and stores the final ops_state after
 * dropping the DSQ lock, so @p can be found on a DSQ while still
 * %SCX_OPSS_DISPATCHING. Wait for the dispatch to finish before operating on
 * @p.
 */
ಶೂನ್ಯ scx_wait_dispatching(ರಚನೆ task_struct *p)
{
	ಆದರೆ (unlikely(atomic_long_read_acquire(&p->scx.ops_state) == SCX_OPSS_DISPATCHING))
		wait_ops_state(p, SCX_OPSS_DISPATCHING);
}

ಸ್ಥಿರ u32 reenq_local(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq, u64 reenq_flags)
{
	LIST_ತಲೆ(tasks);
	u32 nr_enqueued = 0;
	ರಚನೆ task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	ಆದರೆ (WARN_ON_ONCE(reenq_flags & __SCX_REENQ_TSR_MASK))
		reenq_flags &= ~__SCX_REENQ_TSR_MASK;
	ಆದರೆ (rq_is_open(rq, 0))
		reenq_flags |= SCX_REENQ_TSR_RQ_OPEN;

	/*
	 * The BPF scheduler may choose to dispatch tasks back to
	 * @rq->scx.local_dsq. Move all candidate tasks off to a private list
	 * first to avoid processing the same tasks repeatedly.
	 */
	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(p, n, &rq->scx.local_dsq.list,
				 scx.dsq_list.ಗಂಟು) {
		ರಚನೆ scx_sched *task_sch = scx_task_sched(p);
		u32 reason;

		/*
		 * If @p is being migrated, @p's current CPU may not agree with
		 * its allowed CPUs and the migration_cpu_stop is about to
		 * deactivate and re-activate @p anyway. Skip re-enqueueing.
		 *
		 * While racing sched property changes may also dequeue and
		 * re-enqueue a migrating task while its current CPU and allowed
		 * CPUs disagree, they use %ENQUEUE_RESTORE which is bypassed to
		 * the current local DSQ for running tasks and thus are not
		 * visible to the BPF scheduler.
		 */
		ಆದರೆ (p->migration_pending)
			ಮುಂದುವರಿ;

		ಆದರೆ (!scx_is_descendant(task_sch, sch))
			ಮುಂದುವರಿ;

		ಆದರೆ (!local_task_should_reenq(rq, p, &reenq_flags, &reason))
			ಮುಂದುವರಿ;

		scx_wait_dispatching(p);
		scx_dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p);

		ಆದರೆ (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_ಮುಖವಾಡ))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_ಮುಖವಾಡ;
		p->scx.flags |= reason;

		list_ಕೂಡಿಸು_ಬಾಲ(&p->scx.dsq_list.ಗಂಟು, &tasks);
	}

	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(p, n, &tasks, scx.dsq_list.ಗಂಟು) {
		list_del_init(&p->scx.dsq_list.ಗಂಟು);

		scx_do_ಸರತಿಗೆ_ಸೇರಿಸು_task(rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_ಮುಖವಾಡ;
		nr_enqueued++;
	}

	/*
	 * The revoke that scheduled this scan may have raced the pick: curr
	 * may be a now-capless task, either one that kept running or one
	 * promoted off the local DSQ between the ecaps sync and this scan.
	 * Zero the slice to evict it. The enqueue gate blocks new capless
	 * inserts, so no later pick can slip through after the scan.
	 */
	ಆದರೆ ((reenq_flags & SCX_REENQ_CAP_REVOKE) &&
	    rq->curr->sched_class == &ext_sched_class &&
	    scx_task_reenq_on_cap_revoke(rq, rq->curr)) {
		scx_ಗಣ_task_ತುಂಡು(rq->curr, 0);
		resched_curr(rq);
	}

	ಹಿಂತಿರುಗಿ nr_enqueued;
}

ಸ್ಥಿರ ಶೂನ್ಯ process_deferred_reenq_locals(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);

	/*
	 * A task can be re-queued within this loop when a reenqueued task
	 * bounces straight back to the local DSQ. That recursion is bounded by
	 * the per-task reenqueue cap in scx_do_enqueue_task().
	 */
	ಆಗ (true) {
		ರಚನೆ scx_sched *sch;
		u64 reenq_flags;

		scoped_guard (raw_spinlock, &rq->scx.deferred_reenq_ಬೀಗ) {
			ರಚನೆ scx_deferred_reenq_local *drl =
				list_ಮೊದಲ_ನಮೂದು_or_null(&rq->scx.deferred_reenq_locals,
							 ರಚನೆ scx_deferred_reenq_local,
							 ಗಂಟು);
			ರಚನೆ scx_sched_pcpu *sch_pcpu;

			ಆದರೆ (!drl)
				ಹಿಂತಿರುಗಿ;

			sch_pcpu = container_ದ(drl, ರಚನೆ scx_sched_pcpu,
						deferred_reenq_local);
			sch = sch_pcpu->sch;

			reenq_flags = drl->flags;
			WRITE_ONCE(drl->flags, 0);
			list_del_init(&drl->ಗಂಟು);
		}

		/* see schedule_dsq_reenq() */
		smp_mb();

		reenq_local(sch, rq, reenq_flags);
	}
}

ಸ್ಥಿರ bool user_task_should_reenq(ರಚನೆ task_struct *p, u64 reenq_flags, u32 *reason)
{
	*reason = SCX_TASK_REENQ_KFUNC;
	ಹಿಂತಿರುಗಿ reenq_flags & SCX_REENQ_ANY;
}

ಸ್ಥಿರ ಶೂನ್ಯ reenq_user(ರಚನೆ rq *rq, ರಚನೆ scx_dispatch_q *dsq, u64 reenq_flags)
{
	ರಚನೆ rq *locked_rq = rq;
	ರಚನೆ scx_sched *sch = dsq->sched;
	ರಚನೆ scx_dsq_list_ಗಂಟು cursor = INIT_DSQ_LIST_CURSOR(cursor, dsq, 0);
	ರಚನೆ task_struct *p;
	s32 nr_enqueued = 0;

	lockdep_assert_rq_held(rq);

	raw_spin_ಬೀಗ(&dsq->ಬೀಗ);

	ಆಗ (likely(!READ_ONCE(sch->bypass_ಆಳ))) {
		ರಚನೆ rq *task_rq;
		u32 reason;

		p = nldsq_cursor_ಮುಂದಿನ_task(&cursor, dsq);
		ಆದರೆ (!p)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (!user_task_should_reenq(p, reenq_flags, &reason))
			ಮುಂದುವರಿ;

		task_rq = task_rq(p);

		ಆದರೆ (locked_rq != task_rq) {
			ಆದರೆ (locked_rq) {
				scx_rq_ಬೀಗ_drop(locked_rq);
				raw_spin_rq_unlock(locked_rq);
			}
			ಆದರೆ (unlikely(!raw_spin_rq_trylock(task_rq))) {
				raw_spin_unlock(&dsq->ಬೀಗ);
				raw_spin_rq_ಬೀಗ(task_rq);
				raw_spin_ಬೀಗ(&dsq->ಬೀಗ);
			}
			locked_rq = task_rq;

			/* did we lose @p while switching locks? */
			ಆದರೆ (nldsq_cursor_ಕಳೆದುಹೋದ_task(&cursor, task_rq, dsq, p))
				ಮುಂದುವರಿ;
		}

		/* @p is on @dsq, its rq and @dsq are locked */
		scx_wait_dispatching(p);
		dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ_locked(p, dsq);
		raw_spin_unlock(&dsq->ಬೀಗ);

		ಆದರೆ (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_ಮುಖವಾಡ))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_ಮುಖವಾಡ;
		p->scx.flags |= reason;

		scx_do_ಸರತಿಗೆ_ಸೇರಿಸು_task(task_rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_ಮುಖವಾಡ;

		ಆದರೆ (!(++nr_enqueued % SCX_TASK_ITER_BATCH)) {
			scx_rq_ಬೀಗ_drop(locked_rq);
			raw_spin_rq_unlock(locked_rq);
			locked_rq = ಖಾಲಿ;
			cpu_relax();
		}

		raw_spin_ಬೀಗ(&dsq->ಬೀಗ);
	}

	list_del_init(&cursor.ಗಂಟು);
	raw_spin_unlock(&dsq->ಬೀಗ);

	ಆದರೆ (locked_rq != rq) {
		ಆದರೆ (locked_rq) {
			scx_rq_ಬೀಗ_drop(locked_rq);
			raw_spin_rq_unlock(locked_rq);
		}
		raw_spin_rq_ಬೀಗ(rq);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ process_deferred_reenq_users(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);

	ಆಗ (true) {
		ರಚನೆ scx_dispatch_q *dsq;
		u64 dsq_id, reenq_flags;

		scoped_guard (raw_spinlock, &rq->scx.deferred_reenq_ಬೀಗ) {
			ರಚನೆ scx_deferred_reenq_user *dru =
				list_ಮೊದಲ_ನಮೂದು_or_null(&rq->scx.deferred_reenq_users,
							 ರಚನೆ scx_deferred_reenq_user,
							 ಗಂಟು);
			ರಚನೆ scx_dsq_pcpu *dsq_pcpu;

			ಆದರೆ (!dru)
				ಹಿಂತಿರುಗಿ;

			dsq_pcpu = container_ದ(dru, ರಚನೆ scx_dsq_pcpu,
						deferred_reenq_user);
			dsq = dsq_pcpu->dsq;
			reenq_flags = dru->flags;
			WRITE_ONCE(dru->flags, 0);
			list_del_init(&dru->ಗಂಟು);
		}

		/* see schedule_dsq_reenq() */
		smp_mb();

		/* destroy_dsq() may have raced and invalidated @dsq, nothing to reenq */
		dsq_id = READ_ONCE(dsq->id);
		ಆದರೆ (unlikely(dsq_id == SCX_DSQ_INVALID))
			ಮುಂದುವರಿ;

		BUG_ON(dsq_id & SCX_DSQ_FLAG_BUILTIN);
		reenq_user(rq, dsq, reenq_flags);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸತತ_deferred(ರಚನೆ rq *rq)
{
	process_ddsp_deferred_locals(rq);

	ಆದರೆ (!list_ಖಾಲಿಗಳು(&rq->scx.deferred_reenq_locals))
		process_deferred_reenq_locals(rq);

	ಆದರೆ (!list_ಖಾಲಿಗಳು(&rq->scx.deferred_reenq_users))
		process_deferred_reenq_users(rq);

	scx_reenq_reject(rq);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_ತುಂಬಿದ
bool scx_can_stop_tick(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *p = rq->curr;
	ರಚನೆ scx_sched *sch = scx_task_sched(p);

	ಆದರೆ (p->sched_class != &ext_sched_class)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * @rq->curr may still reference an outgoing EXT task after it has been
	 * dequeued. If no EXT tasks are accounted on @rq, ignore its stale
	 * slice state. If another task is dispatched from a DSQ,
	 * set_next_task_scx() will update the dependency for the incoming task.
	 */
	ಆದರೆ (!rq->scx.nr_running)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (scx_bypassing(sch, cpu_ದ(rq)))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * A running rescuee's charging and expiry are tick-driven, see
	 * scx_rescue_charge(). Keep the tick while rescue is in progress.
	 */
	ಆದರೆ (unlikely(p == scx_rescuee(rq)))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * @rq can dispatch from different DSQs, so we can't tell whether it
	 * needs the tick or not by looking at nr_running. Allow stopping ticks
	 * iff the BPF scheduler indicated so. See set_next_task_scx().
	 */
	ಹಿಂತಿರುಗಿ rq->scx.flags & SCX_RQ_CAN_STOP_TICK;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED

DEFINE_STATIC_PERCPU_RWSEM(scx_cgroup_ops_rwsem);

ಶೂನ್ಯ scx_tg_init(ರಚನೆ task_ಗುಂಪು *tg)
{
	tg->scx.ತೂಕ = CGROUP_ತೂಕ_DFL;
	tg->scx.bw_period_us = default_bw_period_us();
	tg->scx.bw_quota_us = RUNTIME_INF;
	tg->scx.idle = false;
}

/**
 * scx_tg_sched - Resolve a task_group's sched
 * @tg: task_group of interest
 *
 * Return the sched that @tg's ops.cgroup_init() succeeded on, %NULL if @tg
 * isn't inited. An autogroup tg has no cgroup of its own and resolves to the
 * root sched.
 *
 * When a child sched exits, its task_groups are moved to the parent and
 * re-inited on it. A failed re-init fails the parent in turn and leaves the
 * task_group without a sched it's inited on, resolving to %NULL. See
 * scx_cgroup_return_subtree().
 *
 * Safe for callers read-locking the ops rwsem. tg->scx.sched rewrites
 * write-lock it, and tg on/offline can't overlap such callers as a css's files
 * are created after online and drained before offline.
 */
ಸ್ಥಿರ ರಚನೆ scx_sched *scx_tg_sched(ರಚನೆ task_ಗುಂಪು *tg)
{
	lockdep_assert(lockdep_is_held(&cgroup_mutex) ||
		       lockdep_is_held(&scx_cgroup_ops_rwsem));

	ಆದರೆ (!tg->css.cgroup)
		tg = &ಬೇರು_task_ಗುಂಪು;
	/* INITED means ops.cgroup_init() succeeded on @tg->scx.sched */
	ಹಿಂತಿರುಗಿ (tg->scx.flags & SCX_TG_INITED) ? tg->scx.sched : ಖಾಲಿ;
}

/**
 * scx_tg_knob_sched - Resolve the sched receiving a task_group's knob updates
 * @tg: task_group of interest
 *
 * Knobs of a cgroup belong to the parent. Deliver the set_* ops to the
 * parent task_group's sched, which equals @tg's own sched everywhere except
 * at a sub-scheduler attach point, where the sub's parent sched receives
 * them.
 *
 * Return %NULL if the parent task_group has no sched. That can happen when the
 * parent's ops.cgroup_init() fails while a sub-scheduler is being disabled.
 *
 * The callers sit in @tg's cgroup file writes holding the ops rwsem read
 * side. That extends scx_tg_sched()'s file-write argument to the parent's
 * sched read: a parent css outlives its children's files.
 */
ಸ್ಥಿರ ರಚನೆ scx_sched *scx_tg_knob_sched(ರಚನೆ task_ಗುಂಪು *tg)
{
	lockdep_assert(lockdep_is_held(&cgroup_mutex) ||
		       lockdep_is_held(&scx_cgroup_ops_rwsem));

	ಆದರೆ (!tg->css.cgroup || !tg->css.ಪೋಷಕ)
		ಹಿಂತಿರುಗಿ scx_tg_sched(&ಬೇರು_task_ಗುಂಪು);
	ಹಿಂತಿರುಗಿ scx_tg_sched(css_tg(tg->css.ಪೋಷಕ));
}

ಪೂರ್ಣಾಂಕ scx_tg_online(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	WARN_ON_ONCE(tg->scx.flags & (SCX_TG_ONLINE | SCX_TG_INITED));

	ಆದರೆ (scx_cgroup_enabled) {
		ರಚನೆ scx_sched *sch;

		/*
		 * The cgroup lifetime notifier populates cgrp->scx_sched before
		 * css_online, but only on the default hierarchy. Sub-scheds are
		 * attached to the cgroup2 hierarchy, so a cgroup1 task_group
		 * always belongs to the root sched.
		 */
		ಆದರೆ (cgroup_on_dfl(tg->css.cgroup))
			sch = scx_cgroup_sched(tg->css.cgroup);
		ಇಲ್ಲದಿದ್ದರೆ
			sch = scx_tg_sched(&ಬೇರು_task_ಗುಂಪು);

		ಆದರೆ (SCX_HAS_OP(sch, cgroup_init)) {
			ರಚನೆ scx_cgroup_init_ವಾದಗಳು ವಾದಗಳು =
				{ .ತೂಕ = tg->scx.ತೂಕ,
				  .bw_period_us = tg->scx.bw_period_us,
				  .bw_quota_us = tg->scx.bw_quota_us,
				  .bw_ಅವಧಿ_us = tg->scx.bw_ಅವಧಿ_us,
				  .sched_idle = tg->scx.idle };

			ret = SCX_CALL_OP_RET(sch, cgroup_init,
					      ಖಾಲಿ, tg->css.cgroup, &ವಾದಗಳು);
			ಆದರೆ (ret)
				ret = scx_ops_sanitize_err(sch, "cgroup_init", ret);
		}
		ಆದರೆ (ret == 0) {
			tg->scx.sched = sch;
			tg->scx.flags |= SCX_TG_ONLINE | SCX_TG_INITED;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		tg->scx.flags |= SCX_TG_ONLINE;
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಶೂನ್ಯ scx_tg_offline(ರಚನೆ task_ಗುಂಪು *tg)
{
	ರಚನೆ scx_sched *sch = tg->scx.sched;

	WARN_ON_ONCE(!(tg->scx.flags & SCX_TG_ONLINE));

	/* INITED implies non-NULL @sch, test before SCX_HAS_OP() derefs */
	ಆದರೆ (scx_cgroup_enabled && (tg->scx.flags & SCX_TG_INITED) &&
	    SCX_HAS_OP(sch, cgroup_exit))
		SCX_CALL_OP(sch, cgroup_exit, ಖಾಲಿ, tg->css.cgroup);
	tg->scx.sched = ಖಾಲಿ;
	tg->scx.flags &= ~(SCX_TG_ONLINE | SCX_TG_INITED);
}

/*
 * @p's sched for the cgroup migration paths. Stable as re-homes happen either
 * at CGROUP_TASK_MIGRATED of the same migration or under scx_cgroup_lock(),
 * both while holding cgroup_mutex.
 */
ಸ್ಥಿರ ರಚನೆ scx_sched *scx_cgroup_task_sched(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ rcu_dereference_protected(p->scx.sched, lockdep_is_held(&cgroup_mutex));
}

ಪೂರ್ಣಾಂಕ scx_cgroup_can_attach(ರಚನೆ cgroup_taskset *tset)
{
	ರಚನೆ cgroup_subsys_state *css;
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!scx_cgroup_enabled)
		ಹಿಂತಿರುಗಿ 0;

	cgroup_taskset_for_each(p, css, tset) {
		ರಚನೆ scx_sched *sch = scx_cgroup_task_sched(p);
		ರಚನೆ cgroup *from = tg_cgrp(task_ಗುಂಪು(p));
		ರಚನೆ cgroup *ಗಮ್ಯ = tg_cgrp(css_tg(css));

		WARN_ON_ONCE(p->scx.cgrp_moving_from);

		/*
		 * sched_move_task() omits identity migrations. Let's match the
		 * behavior so that ops.cgroup_prep_move() and ops.cgroup_move()
		 * always match one-to-one.
		 */
		ಆದರೆ (from == ಗಮ್ಯ)
			ಮುಂದುವರಿ;

		/*
		 * The cgroup_move ops are delivered to @p's sched, and only for
		 * moves that don't re-home @p. A re-homing move changes the dfl
		 * cgroup's sched and is reported through the
		 * exit_task/init_task pair that the re-homing generates.
		 */
		ಆದರೆ (!sch || sch != scx_cgroup_sched(task_css_ಗಣ(p)->mg_dst_cset->dfl_cgrp))
			ಮುಂದುವರಿ;

		ಆದರೆ (SCX_HAS_OP(sch, cgroup_prep_move)) {
			ret = SCX_CALL_OP_RET(sch, cgroup_prep_move, ಖಾಲಿ,
					      p, from, css->cgroup);
			ಆದರೆ (ret) {
				ret = scx_ops_sanitize_err(sch, "cgroup_prep_move", ret);
				ಹೋಗು err;
			}
		}

		p->scx.cgrp_moving_from = from;
	}

	ಹಿಂತಿರುಗಿ 0;

err:
	cgroup_taskset_for_each(p, css, tset) {
		ರಚನೆ scx_sched *sch = scx_cgroup_task_sched(p);

		/* cgrp_moving_from implies non-NULL @sch, test it first */
		ಆದರೆ (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_cancel_move))
			SCX_CALL_OP(sch, cgroup_cancel_move, ಖಾಲಿ,
				    p, p->scx.cgrp_moving_from, css->cgroup);
		p->scx.cgrp_moving_from = ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ ret;
}

ಶೂನ್ಯ scx_cgroup_move_task(ರಚನೆ task_struct *p)
{
	ರಚನೆ scx_sched *sch;

	ಆದರೆ (!scx_cgroup_enabled)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Migration keys off css rather than cgroup identity, so it can hand an
	 * unchanged-cgroup task here with cgrp_moving_from NULL. Nothing to
	 * report to the BPF scheduler then, so skip it and keep prep_move and
	 * move paired.
	 */
	sch = scx_cgroup_task_sched(p);
	ಆದರೆ (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_move))
		SCX_CALL_OP_TASK(sch, cgroup_move, task_rq(p),
				 p, p->scx.cgrp_moving_from,
				 tg_cgrp(task_ಗುಂಪು(p)));
	p->scx.cgrp_moving_from = ಖಾಲಿ;
}

ಶೂನ್ಯ scx_cgroup_cancel_attach(ರಚನೆ cgroup_taskset *tset)
{
	ರಚನೆ cgroup_subsys_state *css;
	ರಚನೆ task_struct *p;

	ಆದರೆ (!scx_cgroup_enabled)
		ಹಿಂತಿರುಗಿ;

	cgroup_taskset_for_each(p, css, tset) {
		ರಚನೆ scx_sched *sch = scx_cgroup_task_sched(p);

		/* cgrp_moving_from implies non-NULL @sch, test it first */
		ಆದರೆ (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_cancel_move))
			SCX_CALL_OP(sch, cgroup_cancel_move, ಖಾಲಿ,
				    p, p->scx.cgrp_moving_from, css->cgroup);
		p->scx.cgrp_moving_from = ಖಾಲಿ;
	}
}

ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_ತೂಕ(ರಚನೆ task_ಗುಂಪು *tg, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ)
{
	ರಚನೆ scx_sched *sch;

	percpu_ಕೆಳಕ್ಕೆ_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	ಆದರೆ (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_ಗಣ_ತೂಕ) &&
	    tg->scx.ತೂಕ != ತೂಕ)
		SCX_CALL_OP(sch, cgroup_ಗಣ_ತೂಕ, ಖಾಲಿ, tg_cgrp(tg), ತೂಕ);

	tg->scx.ತೂಕ = ತೂಕ;

	percpu_ಮೇಲೆ_read(&scx_cgroup_ops_rwsem);
}

ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_idle(ರಚನೆ task_ಗುಂಪು *tg, bool idle)
{
	ರಚನೆ scx_sched *sch;

	percpu_ಕೆಳಕ್ಕೆ_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	ಆದರೆ (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_ಗಣ_idle) &&
	    tg->scx.idle != idle)
		SCX_CALL_OP(sch, cgroup_ಗಣ_idle, ಖಾಲಿ, tg_cgrp(tg), idle);

	/* Update the task group's idle state */
	tg->scx.idle = idle;

	percpu_ಮೇಲೆ_read(&scx_cgroup_ops_rwsem);
}

ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_bandwidth(ರಚನೆ task_ಗುಂಪು *tg,
			     u64 period_us, u64 quota_us, u64 ಅವಧಿ_us)
{
	ರಚನೆ scx_sched *sch;

	percpu_ಕೆಳಕ್ಕೆ_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	ಆದರೆ (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_ಗಣ_bandwidth) &&
	    (tg->scx.bw_period_us != period_us ||
	     tg->scx.bw_quota_us != quota_us ||
	     tg->scx.bw_ಅವಧಿ_us != ಅವಧಿ_us))
		SCX_CALL_OP(sch, cgroup_ಗಣ_bandwidth, ಖಾಲಿ,
			    tg_cgrp(tg), period_us, quota_us, ಅವಧಿ_us);

	tg->scx.bw_period_us = period_us;
	tg->scx.bw_quota_us = quota_us;
	tg->scx.bw_ಅವಧಿ_us = ಅವಧಿ_us;

	percpu_ಮೇಲೆ_read(&scx_cgroup_ops_rwsem);
}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_GROUP_SCHED */

#ಆದರೆ defined(CONFIG_EXT_ಗುಂಪು_SCHED) || defined(CONFIG_EXT_SUB_SCHED)
ಸ್ಥಿರ ರಚನೆ cgroup *ಬೇರು_cgroup(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ &cgrp_dfl_ಬೇರು.cgrp;
}

/*
 * cgroup_lock() must nest outside the rwsem write side: a writer waiting
 * for cgroup_mutex deadlocks with cgroup teardown, which holds it while
 * draining a set_* file write blocked on the rwsem behind the writer.
 */
ಶೂನ್ಯ scx_cgroup_ಬೀಗ(ಶೂನ್ಯ)
{
	cgroup_ಬೀಗ();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	percpu_ಕೆಳಕ್ಕೆ_write(&scx_cgroup_ops_rwsem);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ scx_cgroup_unlock(ಶೂನ್ಯ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	percpu_ಮೇಲೆ_write(&scx_cgroup_ops_rwsem);
#ಆದರೆ_ಅಂತ್ಯ
	cgroup_unlock();
}
#ಇಲ್ಲದಿದ್ದರೆ	/* CONFIG_EXT_GROUP_SCHED || CONFIG_EXT_SUB_SCHED */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cgroup *ಬೇರು_cgroup(ಶೂನ್ಯ) { ಹಿಂತಿರುಗಿ ಖಾಲಿ; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_cgroup_ಬೀಗ(ಶೂನ್ಯ) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_cgroup_unlock(ಶೂನ್ಯ) {}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_GROUP_SCHED || CONFIG_EXT_SUB_SCHED */

/*
 * Omitted operations:
 *
 * - migrate_task_rq: Unnecessary as task to cpu mapping is transient.
 *
 * - task_fork/dead: We need fork/dead notifications for all tasks regardless of
 *   their current sched_class. Call them directly from sched core instead.
 */
DEFINE_SCHED_CLASS(ext) = {
	.ಸರತಿಗೆ_ಸೇರಿಸು_task		= ಸರತಿಗೆ_ಸೇರಿಸು_task_scx,
	.ಸರತಿಯಿಂದ_ತೆಗೆ_task		= ಸರತಿಯಿಂದ_ತೆಗೆ_task_scx,
	.yield_task		= yield_task_scx,
	.yield_ಗಮ್ಯ_task		= yield_ಗಮ್ಯ_task_scx,

	.wakeup_preempt		= wakeup_preempt_scx,

	.pick_task		= pick_task_scx,

	.put_prev_task		= put_prev_task_scx,
	.ಗಣ_ಮುಂದಿನ_task		= ಗಣ_ಮುಂದಿನ_task_scx,

	.select_task_rq		= select_task_rq_scx,
	.task_woken		= task_woken_scx,
	.ಗಣ_cpus_allowed	= ಗಣ_cpus_allowed_scx,

	.rq_online		= rq_online_scx,
	.rq_offline		= rq_offline_scx,

	.task_tick		= task_tick_scx,

	.switching_ಗಮ್ಯ		= switching_ಗಮ್ಯ_scx,
	.switched_from		= switched_from_scx,
	.switched_ಗಮ್ಯ		= switched_ಗಮ್ಯ_scx,
	.reweight_task		= reweight_task_scx,
	.prio_ಬದಲಾಯಿತು		= prio_ಬದಲಾಯಿತು_scx,

	.update_curr		= update_curr_scx,

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#ಆದರೆ_ಅಂತ್ಯ
};

s32 scx_init_dsq(ರಚನೆ scx_dispatch_q *dsq, u64 dsq_id, ರಚನೆ scx_sched *sch)
{
	s32 cpu;

	memset(dsq, 0, ಗಾತ್ರ(*dsq));

	raw_spin_ಬೀಗ_init(&dsq->ಬೀಗ);
	INIT_LIST_ತಲೆ(&dsq->list);
	dsq->id = dsq_id;
	dsq->sched = sch;

	dsq->pcpu = alloc_percpu(ರಚನೆ scx_dsq_pcpu);
	ಆದರೆ (!dsq->pcpu)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_dsq_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(dsq->pcpu, cpu);

		pcpu->dsq = dsq;
		INIT_LIST_ತಲೆ(&pcpu->deferred_reenq_user.ಗಂಟು);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ exit_dsq(ರಚನೆ scx_dispatch_q *dsq)
{
	s32 cpu;

	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_dsq_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(dsq->pcpu, cpu);
		ರಚನೆ scx_deferred_reenq_user *dru = &pcpu->deferred_reenq_user;
		ರಚನೆ rq *rq = cpu_rq(cpu);

		/*
		 * free_dsq_irq_workfn() cancelled the reenqs before the grace
		 * period and schedule_dsq_reenq() queues nothing after that.
		 */
		ಆದರೆ (WARN_ON_ONCE(!list_ಖಾಲಿಗಳು(&dru->ಗಂಟು))) {
			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ಬೀಗ);
			list_del_init(&dru->ಗಂಟು);
		}
	}

	free_percpu(dsq->pcpu);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_dsq_rcufn(ರಚನೆ rcu_ತಲೆ *rcu)
{
	ರಚನೆ scx_dispatch_q *dsq = container_ದ(rcu, ರಚನೆ scx_dispatch_q, rcu);

	exit_dsq(dsq);
	kfree(dsq);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_dsq_irq_workfn(ರಚನೆ irq_work *irq_work)
{
	ರಚನೆ llist_ಗಂಟು *ಗಮ್ಯ_free = llist_del_all(&dsqs_ಗಮ್ಯ_free);
	ರಚನೆ scx_dispatch_q *dsq, *tmp_dsq;

	llist_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(dsq, tmp_dsq, ಗಮ್ಯ_free, free_ಗಂಟು) {
		s32 cpu;

		/*
		 * Cancel pending reenqs. Only a run_deferred() that started
		 * before the grace period can hold one, and it keeps IRQs off,
		 * so the grace period waits for it. After this sweep,
		 * schedule_dsq_reenq() sees the invalid id under the same lock
		 * and queues nothing.
		 */
		for_each_possible_cpu(cpu) {
			ರಚನೆ scx_dsq_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(dsq->pcpu, cpu);
			ರಚನೆ rq *rq = cpu_rq(cpu);

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ಬೀಗ);
			list_del_init(&pcpu->deferred_reenq_user.ಗಂಟು);
		}

		call_rcu(&dsq->rcu, free_dsq_rcufn);
	}
}

ಸ್ಥಿರ DEFINE_IRQ_WORK(free_dsq_irq_work, free_dsq_irq_workfn);

ಸ್ಥಿರ ಶೂನ್ಯ destroy_dsq(ರಚನೆ scx_sched *sch, u64 dsq_id)
{
	ರಚನೆ scx_dispatch_q *dsq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	rcu_read_ಬೀಗ();

	dsq = ಕಂಡುಹಿಡಿ_user_dsq(sch, dsq_id);
	ಆದರೆ (!dsq)
		ಹೋಗು ಹೊರಹೋಗು_unlock_rcu;

	raw_spin_ಬೀಗ_irqsave(&dsq->ಬೀಗ, flags);

	ಆದರೆ (dsq->nr) {
		scx_error(sch, "attempting to destroy in-use dsq 0x%016llx (nr=%u)",
			  dsq->id, dsq->nr);
		ಹೋಗು ಹೊರಹೋಗು_unlock_dsq;
	}

	ಆದರೆ (rhashtable_remove_fast(&sch->dsq_hash, &dsq->hash_ಗಂಟು,
				   dsq_hash_params))
		ಹೋಗು ಹೊರಹೋಗು_unlock_dsq;

	/*
	 * Mark dead by invalidating ->id to prevent scx_dispatch_enqueue() from
	 * queueing more tasks. As this function can be called from anywhere,
	 * freeing is bounced through an irq work to avoid nesting RCU
	 * operations inside scheduler locks.
	 */
	dsq->id = SCX_DSQ_INVALID;
	ಆದರೆ (llist_ಕೂಡಿಸು(&dsq->free_ಗಂಟು, &dsqs_ಗಮ್ಯ_free))
		irq_work_ಸರತಿ(&free_dsq_irq_work);

ಹೊರಹೋಗು_unlock_dsq:
	raw_spin_unlock_irqrestore(&dsq->ಬೀಗ, flags);
ಹೊರಹೋಗು_unlock_rcu:
	rcu_read_unlock();
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
ಸ್ಥಿರ ಶೂನ್ಯ scx_cgroup_exit(ರಚನೆ scx_sched *sch)
{
	ರಚನೆ cgroup_subsys_state *css;

	/*
	 * scx_tg_on/offline() are excluded through cgroup_lock(). If we walk
	 * cgroups and exit all the inited ones, all online cgroups are exited.
	 */
	css_for_each_descendant_post(css, &ಬೇರು_task_ಗುಂಪು.css) {
		ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);

		/* also clear the sched of tgs whose ops.cgroup_init() failed */
		tg->scx.sched = ಖಾಲಿ;
		ಆದರೆ (tg->scx.flags & SCX_TG_INITED) {
			tg->scx.flags &= ~SCX_TG_INITED;
			ಆದರೆ (sch->ops.cgroup_exit)
				SCX_CALL_OP(sch, cgroup_exit, ಖಾಲಿ, css->cgroup);
		}
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scx_cgroup_init(ರಚನೆ scx_sched *sch)
{
	ರಚನೆ cgroup_subsys_state *css;
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * scx_tg_on/offline() are excluded through cgroup_lock(). If we walk
	 * cgroups and init, all online cgroups are initialized.
	 */
	css_for_each_descendant_pre(css, &ಬೇರು_task_ಗುಂಪು.css) {
		ರಚನೆ task_ಗುಂಪು *tg = css_tg(css);

		ಆದರೆ ((tg->scx.flags & (SCX_TG_ONLINE | SCX_TG_INITED)) != SCX_TG_ONLINE)
			ಮುಂದುವರಿ;

		ಆದರೆ (sch->ops.cgroup_init) {
			ರಚನೆ scx_cgroup_init_ವಾದಗಳು ವಾದಗಳು = {
				.ತೂಕ = tg->scx.ತೂಕ,
				.bw_period_us = tg->scx.bw_period_us,
				.bw_quota_us = tg->scx.bw_quota_us,
				.bw_ಅವಧಿ_us = tg->scx.bw_ಅವಧಿ_us,
				.sched_idle = tg->scx.idle,
			};

			ret = SCX_CALL_OP_RET(sch, cgroup_init, ಖಾಲಿ, css->cgroup, &ವಾದಗಳು);
			ಆದರೆ (ret) {
				scx_error(sch, "ops.cgroup_init() failed (%d)", ret);
				ಹಿಂತಿರುಗಿ ret;
			}
		}

		tg->scx.sched = sch;
		tg->scx.flags |= SCX_TG_INITED;
	}

	ಹಿಂತಿರುಗಿ 0;
}

#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಶೂನ್ಯ scx_cgroup_exit(ರಚನೆ scx_sched *sch) {}
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scx_cgroup_init(ರಚನೆ scx_sched *sch) { ಹಿಂತಿರುಗಿ 0; }
#ಆದರೆ_ಅಂತ್ಯ


/********************************************************************************
 * Sysfs interface and ops enable/disable.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು SCX_ATTR(_name)								\
	ಸ್ಥಿರ ರಚನೆ kobj_attribute scx_attr_##_name = {			\
		.attr = { .name = __stringify(_name), .ಬಹುಲಕ = 0444 },		\
		.show = scx_attr_##_name##_show,				\
	}

ಸ್ಥಿರ ssize_t scx_attr_state_show(ರಚನೆ kobject *kobj,
				   ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ಹಿಂತಿರುಗಿ sysfs_emit(buf, "%s\n", scx_enable_state_str[scx_enable_state()]);
}
SCX_ATTR(state);

ಸ್ಥಿರ ssize_t scx_attr_switch_all_show(ರಚನೆ kobject *kobj,
					ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ಹಿಂತಿರುಗಿ sysfs_emit(buf, "%d\n", READ_ONCE(scx_switching_all));
}
SCX_ATTR(switch_all);

ಸ್ಥಿರ ssize_t scx_attr_nr_rejected_show(ರಚನೆ kobject *kobj,
					 ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ಹಿಂತಿರುಗಿ sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_nr_rejected));
}
SCX_ATTR(nr_rejected);

ಸ್ಥಿರ ssize_t scx_attr_hotplug_seq_show(ರಚನೆ kobject *kobj,
					 ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ಹಿಂತಿರುಗಿ sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_hotplug_seq));
}
SCX_ATTR(hotplug_seq);

ಸ್ಥಿರ ssize_t scx_attr_enable_seq_show(ರಚನೆ kobject *kobj,
					ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ಹಿಂತಿರುಗಿ sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_enable_seq));
}
SCX_ATTR(enable_seq);

ಸ್ಥಿರ ರಚನೆ attribute *scx_global_attrs[] = {
	&scx_attr_state.attr,
	&scx_attr_switch_all.attr,
	&scx_attr_nr_rejected.attr,
	&scx_attr_hotplug_seq.attr,
	&scx_attr_enable_seq.attr,
	ಖಾಲಿ,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ attribute_ಗುಂಪು scx_global_attr_ಗುಂಪು = {
	.attrs = scx_global_attrs,
};

ಸ್ಥಿರ ಶೂನ್ಯ free_pnode(ರಚನೆ scx_sched_pnode *pnode);
ಸ್ಥಿರ ಶೂನ್ಯ free_exit_info(ರಚನೆ scx_exit_info *ei);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scx_exit_reason(ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ);
ಸ್ಥಿರ bool scx_claim_exit(ರಚನೆ scx_sched *sch, ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ);

s32 scx_alloc_kern_arena_objs(ರಚನೆ scx_sched *sch)
{
	ಗಾತ್ರ_t ಗಾತ್ರ_2 = struct_ಗಾತ್ರ_t(ರಚನೆ scx_cmask, ಬಿಟ್ಟುಗಳು,
				    SCX_CMASK_NR_ಪದಗಳು(ಅಂಕೆ_possible_cpus()));
	ರಚನೆ scx_cmask *online;
	ರಚನೆ scx_cmask_ref ref;
	ಪೂರ್ಣಾಂಕ cpu;

	/* hotplug stays excluded until the online mask is published */
	lockdep_assert_cpus_held();

	ಆದರೆ (!sch->is_cid_type || !sch->arena_pool)
		ಹಿಂತಿರುಗಿ 0;

	sch->ಗಣ_cmask_scratch = alloc_percpu(ರಚನೆ scx_cmask *);
	ಆದರೆ (!sch->ಗಣ_cmask_scratch)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_cmask **ಸ್ಥಾನ = ಪ್ರತಿ_cpu_ptr(sch->ಗಣ_cmask_scratch, cpu);

		*ಸ್ಥಾನ = scx_arena_alloc(sch, ಗಾತ್ರ_2);
		ಆದರೆ (!*ಸ್ಥಾನ)
			ಹಿಂತಿರುಗಿ -ENOMEM;
		scx_cmask_init(*ಸ್ಥಾನ, 0, ಅಂಕೆ_possible_cpus());
	}

	/* pack the online mask alongside the scratch masks */
	online = scx_arena_alloc(sch, ಗಾತ್ರ_2);
	ಆದರೆ (!online)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	scoped_guard(rcu) {
		scx_cmask_ref_init_kern(sch, online, 0, ಅಂಕೆ_possible_cpus(), &ref);
		scx_cmask_ref_from_cpumask(&ref, cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ);
	}
	sch->online_cmask = online;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_free_kern_arena_objs(ರಚನೆ scx_sched *sch)
{
	ಗಾತ್ರ_t ಗಾತ್ರ_2 = struct_ಗಾತ್ರ_t(ರಚನೆ scx_cmask, ಬಿಟ್ಟುಗಳು,
				    SCX_CMASK_NR_ಪದಗಳು(ಅಂಕೆ_possible_cpus()));
	ಪೂರ್ಣಾಂಕ cpu;

	scx_arena_free(sch, sch->online_cmask, ಗಾತ್ರ_2);
	ಆದರೆ (!sch->ಗಣ_cmask_scratch)
		ಹಿಂತಿರುಗಿ;

	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_cmask **ಸ್ಥಾನ = ಪ್ರತಿ_cpu_ptr(sch->ಗಣ_cmask_scratch, cpu);

		scx_arena_free(sch, *ಸ್ಥಾನ, ಗಾತ್ರ_2);
	}
	free_percpu(sch->ಗಣ_cmask_scratch);
	sch->ಗಣ_cmask_scratch = ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_sched_free_rcu_work(ರಚನೆ work_struct *work)
{
	ರಚನೆ rcu_work *rcu_work = ಗಮ್ಯ_rcu_work(work);
	ರಚನೆ scx_sched *sch = container_ದ(rcu_work, ರಚನೆ scx_sched, rcu_work);
	ರಚನೆ rhashtable_iter rht_iter;
	ರಚನೆ scx_dispatch_q *dsq;
	ಪೂರ್ಣಾಂಕ cpu, ಗಂಟು;

	irq_work_sync(&sch->propagate_exit_irq_work);
	irq_work_sync(&sch->disable_irq_work);
	kthread_destroy_ಕೆಲಸಗಾರ(sch->helper);
	timer_shutdown_sync(&sch->bypass_lb_timer);
	free_cpumask_var(sch->bypass_lb_donee_cpumask);
	free_cpumask_var(sch->bypass_lb_resched_cpumask);
	free_cpumask_var(sch->stall_cpus);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	kfree(sch->cgrp_ಮಾರ್ಗ);
	ಆದರೆ (sch_cgroup(sch))
		cgroup_put(sch_cgroup(sch));
	ಆದರೆ (sch->sub_kset)
		kobject_put(&sch->sub_kset->kobj);
	ಆದರೆ (scx_ಪೋಷಕ(sch))
		kobject_put(&scx_ಪೋಷಕ(sch)->kobj);
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_sched_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu);

		/*
		 * $sch would have entered bypass mode before the RCU grace
		 * period. As that blocks new deferrals, all
		 * deferred_reenq_local_node's must be off-list by now.
		 */
		WARN_ON_ONCE(!list_ಖಾಲಿಗಳು(&pcpu->deferred_reenq_local.ಗಂಟು));

		/* remove the queued ecaps sync so the pcpu can be freed */
		scx_discard_ecaps_ಗಮ್ಯ_sync(cpu, pcpu);

		/*
		 * Bypass blocks new kicks. Flush the kick irq_work so this
		 * pcpu's to_kick_node is off the list before it is freed.
		 */
		irq_work_sync(&cpu_rq(cpu)->scx.kick_cpus_irq_work);
		WARN_ON_ONCE(!list_ಖಾಲಿಗಳು(&pcpu->ಗಮ್ಯ_kick_ಗಂಟು));
		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_kick);
		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_kick_if_idle);
		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_preempt);
		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_wait);

		exit_dsq(scx_bypass_dsq(sch, cpu));
	}

	free_percpu(sch->pcpu);

	for_each_ಗಂಟು_state(ಗಂಟು, N_POSSIBLE)
		free_pnode(sch->pnode[ಗಂಟು]);
	kfree(sch->pnode);

	scx_free_pshards(sch);

	rhashtable_walk_enter(&sch->dsq_hash, &rht_iter);
	ಮಾಡು {
		rhashtable_walk_ಆರಂಭ(&rht_iter);

		ಆಗ (!IS_ERR_OR_NULL((dsq = rhashtable_walk_ಮುಂದಿನ(&rht_iter))))
			destroy_dsq(sch, dsq->id);

		rhashtable_walk_stop(&rht_iter);
	} ಆಗ (dsq == ERR_PTR(-EAGAIN));
	rhashtable_walk_exit(&rht_iter);

	rhashtable_free_and_destroy(&sch->dsq_hash, ಖಾಲಿ, ಖಾಲಿ);
	free_exit_info(sch->exit_info);
	scx_free_kern_arena_objs(sch);
	scx_arena_pool_destroy(sch);
	ಆದರೆ (sch->arena_map)
		bpf_map_put(sch->arena_map);

	/* @sch is completely inactive by now */
	scx_dec_has_subs(sch);

	kfree(sch);
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_kobj_release(ರಚನೆ kobject *kobj)
{
	ರಚನೆ scx_sched *sch = container_ದ(kobj, ರಚನೆ scx_sched, kobj);

	INIT_RCU_WORK(&sch->rcu_work, scx_sched_free_rcu_work);
	ಸರತಿ_rcu_work(system_dfl_wq, &sch->rcu_work);
}

ಸ್ಥಿರ ssize_t scx_attr_ops_show(ರಚನೆ kobject *kobj,
				 ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ರಚನೆ scx_sched *sch = container_ದ(kobj, ರಚನೆ scx_sched, kobj);

	ಹಿಂತಿರುಗಿ sysfs_emit(buf, "%s\n", sch->ops.name);
}
SCX_ATTR(ops);

#ವ್ಯಾಖ್ಯಾನಿಸು scx_attr_event_show(buf, at, events, ಬಗೆ) ({				\
	sysfs_emit_at(buf, at, "%s %llu\n", #kind, (events)->ಬಗೆ);		\
})

ಸ್ಥಿರ ssize_t scx_attr_events_show(ರಚನೆ kobject *kobj,
				    ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ರಚನೆ scx_sched *sch = container_ದ(kobj, ರಚನೆ scx_sched, kobj);
	ರಚನೆ scx_event_stats events;
	ಪೂರ್ಣಾಂಕ at = 0;

	scx_read_events(sch, &events);
#ವ್ಯಾಖ್ಯಾನಿಸು SCX_EVENT(name)	(at += scx_attr_event_show(buf, at, &events, name))
	SCX_EVENTS_LIST(SCX_EVENT);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCX_EVENT
	ಹಿಂತಿರುಗಿ at;
}
SCX_ATTR(events);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scx_cap_ಹೆಸರುಗಳು[__SCX_NR_CAPS] = {
	[__SCX_CAP_ENQ_IMMED]	= "enq_immed",
	[__SCX_CAP_ENQ]		= "enq",
	[__SCX_CAP_PREEMPT]	= "preempt",
	[__SCX_CAP_PERF]	= "perf",
};

ಸ್ಥಿರ ssize_t scx_attr_caps_show(ರಚನೆ kobject *kobj,
				  ರಚನೆ kobj_attribute *ka, ಅಕ್ಷರ *buf)
{
	ರಚನೆ scx_sched *sch = container_ದ(kobj, ರಚನೆ scx_sched, kobj);
	u32 npossible = ಅಂಕೆ_possible_cpus();
	ರಚನೆ scx_cmask *agg __free(kfree) =
		kzalloc_flex(*agg, ಬಿಟ್ಟುಗಳು, SCX_CMASK_NR_ಪದಗಳು(npossible));
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *agg_bm __free(bitmap) = bitmap_zalloc(npossible, GFP_KERNEL);
	ssize_t ಎಣಿಕೆ_2 = 0;
	s32 cap, si;

	ಆದರೆ (!agg || !agg_bm)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	ಗಾಗಿ (cap = 0; cap < __SCX_NR_CAPS; cap++) {
		SCX_CMASK_DEFINE(snap, 0, SCX_CID_SHARD_MAX_CPUS);

		scx_cmask_init(agg, 0, npossible);
		ಗಾಗಿ (si = 0; si < sch->nr_pshards; si++) {
			ರಚನೆ scx_cmask *cm = &sch->pshard[si]->caps[cap].cmask;

			scx_cmask_reframe(snap, cm->ತಳ, cm->nr_cids);
			scx_cmask_copy(snap, cm);
			scx_cmask_or(agg, snap);
		}
		/* %*pbl takes unsigned long bitmap layout, convert from u64 */
		bitmap_from_arr64(agg_bm, agg->ಬಿಟ್ಟುಗಳು, npossible);
		ಎಣಿಕೆ_2 += sysfs_emit_at(buf, ಎಣಿಕೆ_2, "%s: %*pbl\n",
				       scx_cap_ಹೆಸರುಗಳು[cap], npossible, agg_bm);
	}
	ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}
SCX_ATTR(caps);
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

ಸ್ಥಿರ ರಚನೆ attribute *scx_sched_attrs[] = {
	&scx_attr_ops.attr,
	&scx_attr_events.attr,
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	&scx_attr_caps.attr,
#ಆದರೆ_ಅಂತ್ಯ
	ಖಾಲಿ,
};
ATTRIBUTE_GROUPS(scx_sched);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ kobj_type scx_ktype = {
	.release = scx_kobj_release,
	.sysfs_ops = &kobj_sysfs_ops,
	.default_groups = scx_sched_groups,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scx_uevent(ಸ್ಥಿರಾಂಕ ರಚನೆ kobject *kobj, ರಚನೆ kobj_uevent_env *env)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ scx_sched *sch;

	/*
	 * scx_uevent() can be reached by both scx_sched kobjects (scx_ktype)
	 * and sub-scheduler kset kobjects (kset_ktype) through the parent
	 * chain walk. Filter out the latter to avoid invalid casts.
	 */
	ಆದರೆ (kobj->ktype != &scx_ktype)
		ಹಿಂತಿರುಗಿ 0;

	sch = container_ದ(kobj, ರಚನೆ scx_sched, kobj);

	ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_uevent_var(env, "SCXOPS=%s", sch->ops.name);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ kset_uevent_ops scx_uevent_ops = {
	.uevent = scx_uevent,
};

/*
 * Used by sched_fork() and __setscheduler_class() to pick the matching
 * sched_class. dl/rt are already handled.
 */
bool task_should_scx(ಪೂರ್ಣಾಂಕ policy)
{
	/* if disabled, nothing should be on it */
	ಆದರೆ (!scx_enabled())
		ಹಿಂತಿರುಗಿ false;

	/* scx is taking over all SCHED_OTHER and SCHED_EXT tasks */
	ಆದರೆ (READ_ONCE(scx_switching_all))
		ಹಿಂತಿರುಗಿ true;

	/*
	 * scx is tearing down - keep new SCHED_EXT tasks out.
	 *
	 * Must come after scx_switching_all test, which serves as a proxy
	 * for __scx_switched_all. While __scx_switched_all is set, we must
	 * return true via the branch above: a fork routed to fair would
	 * stall because next_active_class() skips fair.
	 *
	 * This can develop into a deadlock - scx holds scx_enable_mutex across
	 * kthread_create() in scx_alloc_and_add_sched(); if the new kthread is
	 * the stalled task, the disable path can never grab the mutex to clear
	 * scx_switching_all.
	 */
	ಆದರೆ (unlikely(scx_enable_state() == SCX_DISABLING))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ policy == SCHED_EXT;
}

bool scx_allow_ttwu_ಸರತಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ರಚನೆ scx_sched *sch;

	ಆದರೆ (!scx_enabled())
		ಹಿಂತಿರುಗಿ true;

	sch = scx_task_sched(p);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (sch->ops.flags & SCX_OPS_ALLOW_QUEUED_WAKEUP)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (unlikely(p->sched_class != &ext_sched_class))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

/**
 * handle_lockup - sched_ext common lockup handler
 * @exit_cpu: CPU to record in exit_info. Pass the stalled/hung CPU, not current.
 * @fmt: format string
 *
 * Called on system stall or lockup condition and initiates abort of sched_ext
 * if enabled, which may resolve the reported lockup.
 *
 * Returns %true if sched_ext is enabled and abort was initiated, which may
 * resolve the lockup. %false if sched_ext is not enabled or abort was already
 * initiated by someone else.
 */
ಸ್ಥಿರ __printf(2, 3) bool handle_lockup(ಪೂರ್ಣಾಂಕ exit_cpu, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
	ರಚನೆ scx_sched *sch;
	va_list ವಾದಗಳು;
	bool ret;

	guard(rcu)();

	sch = rcu_dereference(scx_ಬೇರು);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ false;

	ಆಯ್ಕೆ (scx_enable_state()) {
	ಸ್ಥಿತಿ SCX_ENABLING:
	ಸ್ಥಿತಿ SCX_ENABLED:
		va_ಆರಂಭ(ವಾದಗಳು, fmt);
		ret = scx_vexit(sch, SCX_EXIT_ERROR, 0, exit_cpu, fmt, ವಾದಗಳು);
		va_ಕೊನೆ(ವಾದಗಳು);
		ಹಿಂತಿರುಗಿ ret;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ false;
	}
}

/**
 * scx_rcu_cpu_stall - sched_ext RCU CPU stall handler
 * @stalled_mask: bit mask of stalled CPUs
 *
 * While there are various reasons why RCU CPU stalls can occur on a system
 * that may not be caused by the current BPF scheduler, try kicking out the
 * current scheduler in an attempt to recover the system to a good state before
 * issuing panics.
 *
 * Returns %true if sched_ext is enabled and abort was initiated, which may
 * resolve the reported RCU stall. %false if sched_ext is not enabled or someone
 * else already initiated abort.
 */
bool scx_rcu_cpu_stall(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *stalled_ಮುಖವಾಡ)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_exit_info *ei;
	ಪೂರ್ಣಾಂಕ exit_cpu;

	guard(rcu)();

	sch = rcu_dereference(scx_ಬೇರು);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ false;

	ಆಯ್ಕೆ (scx_enable_state()) {
	ಸ್ಥಿತಿ SCX_ENABLING:
	ಸ್ಥಿತಿ SCX_ENABLED:
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ false;
	}

	exit_cpu = cpumask_ಖಾಲಿಗಳು(stalled_ಮುಖವಾಡ) ? -1 : (ಪೂರ್ಣಾಂಕ)cpumask_ಮೊದಲ(stalled_ಮುಖವಾಡ);
	ei = sch->exit_info;

	guard(preempt)();

	ಆದರೆ (!scx_claim_exit(sch, SCX_EXIT_ERROR))
		ಹಿಂತಿರುಗಿ false;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_STACKTRACE
	ei->bt_len = ರಾಶಿ_trace_save(ei->bt, SCX_EXIT_BT_LEN, 1);
#ಆದರೆ_ಅಂತ್ಯ
	scnprintf(ei->ಸಂದೇಶ, SCX_EXIT_ಸಂದೇಶ_LEN, "RCU CPU stall on CPUs (%*pbl)",
		  cpumask_pr_ವಾದಗಳು(stalled_ಮುಖವಾಡ));
	ei->ಬಗೆ = SCX_EXIT_ERROR;
	ei->reason = scx_exit_reason(SCX_EXIT_ERROR);
	ei->exit_cpu = exit_cpu;
	cpumask_copy(sch->stall_cpus, stalled_ಮುಖವಾಡ);

	irq_work_ಸರತಿ(&sch->disable_irq_work);
	ಹಿಂತಿರುಗಿ true;
}

/**
 * scx_softlockup - sched_ext softlockup handler
 * @dur_s: number of seconds of CPU stuck due to soft lockup
 *
 * On some multi-socket setups (e.g. 2x Intel 8480c), the BPF scheduler can
 * live-lock the system by making many CPUs target the same DSQ to the point
 * where soft-lockup detection triggers. This function is called from
 * soft-lockup watchdog when the triggering point is close and tries to unjam
 * the system and aborting the BPF scheduler.
 */
ಶೂನ್ಯ scx_softlockup(u32 dur_s)
{
	ಪೂರ್ಣಾಂಕ cpu = smp_processor_id();

	ಆದರೆ (!handle_lockup(cpu, "soft lockup - CPU %d stuck for %us", cpu, dur_s))
		ಹಿಂತಿರುಗಿ;

	printk_deferred(KERN_ERR "sched_ext: Soft lockup - CPU %d stuck for %us, disabling BPF scheduler\n",
			cpu, dur_s);
}

/**
 * scx_hardlockup - sched_ext hardlockup handler
 * @cpu: the target CPU
 *
 * A poorly behaving BPF scheduler can trigger hard lockup by e.g. putting
 * numerous affinitized tasks in a single queue and directing all CPUs at it.
 * Try kicking out the current scheduler in an attempt to recover the system to
 * a good state before taking more drastic actions.
 *
 * Called from NMI. Aborting the scheduler sets ->aborting throughout the
 * hierarchy before returning, which is what breaks the dispatch-path live-locks
 * that can hard-lock CPUs.
 *
 * Returns %true if sched_ext is enabled and abort was initiated, which may
 * resolve the lockup. %false if sched_ext is not enabled or abort was already
 * initiated by someone else.
 */
bool scx_hardlockup(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (!handle_lockup(cpu, "hard lockup - CPU %d", cpu))
		ಹಿಂತಿರುಗಿ false;

	printk_deferred(KERN_ERR "sched_ext: Hard lockup - CPU %d, disabling BPF scheduler\n",
			cpu);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ u32 bypass_lb_cpu(ರಚನೆ scx_sched *sch, s32 donor,
			 ರಚನೆ cpumask *donee_ಮುಖವಾಡ, ರಚನೆ cpumask *resched_ಮುಖವಾಡ,
			 u32 nr_donor_ಗುರಿ, u32 nr_donee_ಗುರಿ)
{
	ರಚನೆ rq *donor_rq = cpu_rq(donor);
	ರಚನೆ scx_dispatch_q *donor_dsq = scx_bypass_dsq(sch, donor);
	ರಚನೆ task_struct *p, *n;
	ರಚನೆ scx_dsq_list_ಗಂಟು cursor = INIT_DSQ_LIST_CURSOR(cursor, donor_dsq, 0);
	s32 delta = READ_ONCE(donor_dsq->nr) - nr_donor_ಗುರಿ;
	u32 nr_ಸಮತೋಲಿತ = 0, min_delta_us;

	/*
	 * All we want to guarantee is reasonable forward progress. No reason to
	 * fine tune. Assuming every task on @donor_dsq runs their full slice,
	 * consider offloading iff the total queued duration is over the
	 * threshold.
	 */
	min_delta_us = READ_ONCE(scx_bypass_lb_intv_us) / SCX_BYPASS_LB_MIN_DELTA_DIV;
	ಆದರೆ (delta < DIV_ಸುತ್ತು_ಮೇಲೆ(min_delta_us, READ_ONCE(scx_ತುಂಡು_bypass_us)))
		ಹಿಂತಿರುಗಿ 0;

	raw_spin_rq_ಬೀಗ_irq(donor_rq);
	raw_spin_ಬೀಗ(&donor_dsq->ಬೀಗ);
	list_ಕೂಡಿಸು(&cursor.ಗಂಟು, &donor_dsq->list);
resume:
	n = container_ದ(&cursor, ರಚನೆ task_struct, scx.dsq_list);
	n = nldsq_ಮುಂದಿನ_task(donor_dsq, n, false);

	ಆಗ ((p = n)) {
		ರಚನೆ scx_dispatch_q *donee_dsq;
		ಪೂರ್ಣಾಂಕ donee;

		n = nldsq_ಮುಂದಿನ_task(donor_dsq, n, false);

		ಆದರೆ (donor_dsq->nr <= nr_donor_ಗುರಿ)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(donee_ಮುಖವಾಡ))
			ನಿಲ್ಲಿಸು;

		/*
		 * If an earlier pass placed @p on @donor_dsq from a different
		 * CPU and the donee hasn't consumed it yet, @p is still on the
		 * previous CPU and task_rq(@p) != @donor_rq. @p can't be moved
		 * without its rq locked. Skip.
		 */
		ಆದರೆ (task_rq(p) != donor_rq)
			ಮುಂದುವರಿ;

		donee = cpumask_any_and_distribute(donee_ಮುಖವಾಡ, p->cpus_ptr);
		ಆದರೆ (donee >= nr_cpu_ids)
			ಮುಂದುವರಿ;

		donee_dsq = scx_bypass_dsq(sch, donee);

		/*
		 * $p's rq is not locked but $p's DSQ lock protects its
		 * scheduling properties making this test safe.
		 */
		ಆದರೆ (!task_can_ಸತತ_on_remote_rq(sch, p, cpu_rq(donee), false))
			ಮುಂದುವರಿ;

		/*
		 * Moving $p from one non-local DSQ to another. The source rq
		 * and DSQ are already locked. Do an abbreviated dequeue and
		 * then perform enqueue without unlocking $donor_dsq.
		 *
		 * We don't want to drop and reacquire the lock on each
		 * iteration as @donor_dsq can be very long and potentially
		 * highly contended. Donee DSQs are less likely to be contended.
		 * The nested locking is safe as only this LB moves tasks
		 * between bypass DSQs.
		 */
		dispatch_ಸರತಿಯಿಂದ_ತೆಗೆ_locked(p, donor_dsq);
		scx_dispatch_ಸರತಿಗೆ_ಸೇರಿಸು(sch, cpu_rq(donee), donee_dsq, p, 0, 0, SCX_ENQ_NESTED);

		/*
		 * $donee might have been idle and need to be woken up. No need
		 * to be clever. Kick every CPU that receives tasks.
		 */
		cpumask_ಗಣ_cpu(donee, resched_ಮುಖವಾಡ);

		ಆದರೆ (READ_ONCE(donee_dsq->nr) >= nr_donee_ಗುರಿ)
			cpumask_clear_cpu(donee, donee_ಮುಖವಾಡ);

		nr_ಸಮತೋಲಿತ++;
		ಆದರೆ (!(nr_ಸಮತೋಲಿತ % SCX_BYPASS_LB_BATCH) && n) {
			list_move_ಬಾಲ(&cursor.ಗಂಟು, &n->scx.dsq_list.ಗಂಟು);
			raw_spin_unlock(&donor_dsq->ಬೀಗ);
			scx_rq_ಬೀಗ_drop(donor_rq);
			raw_spin_rq_unlock_irq(donor_rq);
			cpu_relax();
			raw_spin_rq_ಬೀಗ_irq(donor_rq);
			raw_spin_ಬೀಗ(&donor_dsq->ಬೀಗ);
			ಹೋಗು resume;
		}
	}

	list_del_init(&cursor.ಗಂಟು);
	raw_spin_unlock(&donor_dsq->ಬೀಗ);
	scx_rq_ಬೀಗ_drop(donor_rq);
	raw_spin_rq_unlock_irq(donor_rq);

	ಹಿಂತಿರುಗಿ nr_ಸಮತೋಲಿತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ bypass_lb_ಗಂಟು(ರಚನೆ scx_sched *sch, ಪೂರ್ಣಾಂಕ ಗಂಟು)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಗಂಟು_ಮುಖವಾಡ = cpumask_ದ_ಗಂಟು(ಗಂಟು);
	ರಚನೆ cpumask *donee_ಮುಖವಾಡ = sch->bypass_lb_donee_cpumask;
	ರಚನೆ cpumask *resched_ಮುಖವಾಡ = sch->bypass_lb_resched_cpumask;
	u32 nr_tasks = 0, nr_cpus = 0, nr_ಸಮತೋಲಿತ = 0;
	u32 nr_ಗುರಿ, nr_donor_ಗುರಿ;
	u32 before_min = U32_MAX, before_max = 0;
	u32 after_min = U32_MAX, after_max = 0;
	ಪೂರ್ಣಾಂಕ cpu;

	/* count the target tasks and CPUs */
	for_each_cpu_and(cpu, cpu_online_ಮುಖವಾಡ, ಗಂಟು_ಮುಖವಾಡ) {
		u32 nr = READ_ONCE(scx_bypass_dsq(sch, cpu)->nr);

		nr_tasks += nr;
		nr_cpus++;

		before_min = min(nr, before_min);
		before_max = max(nr, before_max);
	}

	ಆದರೆ (!nr_cpus)
		ಹಿಂತಿರುಗಿ;

	/*
	 * We don't want CPUs to have more than $nr_donor_target tasks and
	 * balancing to fill donee CPUs upto $nr_target. Once targets are
	 * calculated, find the donee CPUs.
	 */
	nr_ಗುರಿ = DIV_ಸುತ್ತು_ಮೇಲೆ(nr_tasks, nr_cpus);
	nr_donor_ಗುರಿ = DIV_ಸುತ್ತು_ಮೇಲೆ(nr_ಗುರಿ * SCX_BYPASS_LB_DONOR_PCT, 100);

	cpumask_clear(donee_ಮುಖವಾಡ);
	for_each_cpu_and(cpu, cpu_online_ಮುಖವಾಡ, ಗಂಟು_ಮುಖವಾಡ) {
		ಆದರೆ (READ_ONCE(scx_bypass_dsq(sch, cpu)->nr) < nr_ಗುರಿ)
			cpumask_ಗಣ_cpu(cpu, donee_ಮುಖವಾಡ);
	}

	/* iterate !donee CPUs and see if they should be offloaded */
	cpumask_clear(resched_ಮುಖವಾಡ);
	for_each_cpu_and(cpu, cpu_online_ಮುಖವಾಡ, ಗಂಟು_ಮುಖವಾಡ) {
		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(donee_ಮುಖವಾಡ))
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (cpumask_test_cpu(cpu, donee_ಮುಖವಾಡ))
			ಮುಂದುವರಿ;
		ಆದರೆ (READ_ONCE(scx_bypass_dsq(sch, cpu)->nr) <= nr_donor_ಗುರಿ)
			ಮುಂದುವರಿ;

		nr_ಸಮತೋಲಿತ += bypass_lb_cpu(sch, cpu, donee_ಮುಖವಾಡ, resched_ಮುಖವಾಡ,
					     nr_donor_ಗುರಿ, nr_ಗುರಿ);
	}

	for_each_cpu(cpu, resched_ಮುಖವಾಡ)
		resched_cpu(cpu);

	for_each_cpu_and(cpu, cpu_online_ಮುಖವಾಡ, ಗಂಟು_ಮುಖವಾಡ) {
		u32 nr = READ_ONCE(scx_bypass_dsq(sch, cpu)->nr);

		after_min = min(nr, after_min);
		after_max = max(nr, after_max);

	}

	trace_sched_ext_bypass_lb(ಗಂಟು, nr_cpus, nr_tasks, nr_ಸಮತೋಲಿತ,
				  before_min, before_max, after_min, after_max);
}

/*
 * In bypass mode, all tasks are put on the per-CPU bypass DSQs. If the machine
 * is over-saturated and the BPF scheduler skewed tasks into few CPUs, some
 * bypass DSQs can be overloaded. If there are enough tasks to saturate other
 * lightly loaded CPUs, such imbalance can lead to very high execution latency
 * on the overloaded CPUs and thus to hung tasks and RCU stalls. To avoid such
 * outcomes, a simple load balancing mechanism is implemented by the following
 * timer which runs periodically while bypass mode is in effect.
 */
ಸ್ಥಿರ ಶೂನ್ಯ scx_bypass_lb_timerfn(ರಚನೆ timer_list *timer)
{
	ರಚನೆ scx_sched *sch = container_ದ(timer, ರಚನೆ scx_sched, bypass_lb_timer);
	ಪೂರ್ಣಾಂಕ ಗಂಟು;
	u32 intv_us;

	ಆದರೆ (!scx_bypass_dsp_enabled(sch))
		ಹಿಂತಿರುಗಿ;

	for_each_ಗಂಟು_with_cpus(ಗಂಟು)
		bypass_lb_ಗಂಟು(sch, ಗಂಟು);

	intv_us = READ_ONCE(scx_bypass_lb_intv_us);
	ಆದರೆ (intv_us)
		mod_timer(timer, jiffies + usecs_ಗಮ್ಯ_jiffies(intv_us));
}

ಸ್ಥಿರ bool inc_bypass_ಆಳ(ರಚನೆ scx_sched *sch)
{
	lockdep_assert_held(&scx_bypass_ಬೀಗ);

	WARN_ON_ONCE(sch->bypass_ಆಳ < 0);
	WRITE_ONCE(sch->bypass_ಆಳ, sch->bypass_ಆಳ + 1);
	ಆದರೆ (sch->bypass_ಆಳ != 1)
		ಹಿಂತಿರುಗಿ false;

	WRITE_ONCE(sch->ತುಂಡು_dfl, READ_ONCE(scx_ತುಂಡು_bypass_us) * NSEC_ಪ್ರತಿ_USEC);
	sch->bypass_timestamp = ktime_get_ns();
	scx_ಕೂಡಿಸು_event(sch, SCX_EV_BYPASS_ACTIVATE, 1);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool dec_bypass_ಆಳ(ರಚನೆ scx_sched *sch)
{
	lockdep_assert_held(&scx_bypass_ಬೀಗ);

	WARN_ON_ONCE(sch->bypass_ಆಳ < 1);
	WRITE_ONCE(sch->bypass_ಆಳ, sch->bypass_ಆಳ - 1);
	ಆದರೆ (sch->bypass_ಆಳ != 0)
		ಹಿಂತಿರುಗಿ false;

	WRITE_ONCE(sch->ತುಂಡು_dfl, SCX_ತುಂಡು_DFL);
	scx_ಕೂಡಿಸು_event(sch, SCX_EV_BYPASS_DURATION,
		      ktime_get_ns() - sch->bypass_timestamp);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ enable_bypass_dsp(ರಚನೆ scx_sched *sch)
{
	ರಚನೆ scx_sched *host = scx_ಪೋಷಕ(sch) ?: sch;
	u32 intv_us = READ_ONCE(scx_bypass_lb_intv_us);
	s32 ret;

	/*
	 * @sch->bypass_depth transitioning from 0 to 1 triggers enabling.
	 * Shouldn't stagger.
	 */
	ಆದರೆ (WARN_ON_ONCE(test_and_ಗಣ_ಬಿಟ್(0, &sch->bypass_dsp_claim)))
		ಹಿಂತಿರುಗಿ;

	/*
	 * When a sub-sched bypasses, its tasks are queued on the bypass DSQs of
	 * the nearest non-bypassing ancestor or root. The parent hosts them
	 * unless it is bypassing too. Then they sit further up and the parent
	 * consumes only its own bypass DSQs, empty until it leaves bypass and
	 * becomes the host. Either way the nearest non-bypassing ancestor or
	 * root has bypass dispatch enabled while a descendant is bypassing,
	 * which is all that's required.
	 *
	 * scx_bypass_dsp_enabled() test is used to determine whether to enter
	 * the bypass dispatch handling path from both bypassing and hosting
	 * scheds. Bump enable depth on both @sch and bypass dispatch host.
	 */
	ret = atomic_inc_return(&sch->bypass_dsp_enable_ಆಳ);
	WARN_ON_ONCE(ret <= 0);

	ಆದರೆ (host != sch) {
		ret = atomic_inc_return(&host->bypass_dsp_enable_ಆಳ);
		WARN_ON_ONCE(ret <= 0);
	}

	/*
	 * The LB timer will stop running if bypass dispatch is disabled. Start
	 * after enabling bypass dispatch.
	 */
	ಆದರೆ (intv_us && !timer_pending(&host->bypass_lb_timer))
		mod_timer(&host->bypass_lb_timer,
			  jiffies + usecs_ಗಮ್ಯ_jiffies(intv_us));
}

/* may be called without holding scx_bypass_lock */
ಶೂನ್ಯ scx_disable_bypass_dsp(ರಚನೆ scx_sched *sch)
{
	s32 ret;

	ಆದರೆ (!test_and_clear_ಬಿಟ್(0, &sch->bypass_dsp_claim))
		ಹಿಂತಿರುಗಿ;

	ret = atomic_dec_return(&sch->bypass_dsp_enable_ಆಳ);
	WARN_ON_ONCE(ret < 0);

	ಆದರೆ (scx_ಪೋಷಕ(sch)) {
		ret = atomic_dec_return(&scx_ಪೋಷಕ(sch)->bypass_dsp_enable_ಆಳ);
		WARN_ON_ONCE(ret < 0);
	}
}

/**
 * unbypass_renotify_idle - Arm an idle re-notify for a sched leaving bypass
 * @rq: rq of the cpu leaving bypass
 * @pos: scheduler that just left bypass on @rq's cpu
 * @pcpu: @pos's per-cpu state for @rq's cpu
 *
 * A sched leaving bypass is owed the ops.update_idle() calls suppressed while
 * bypassing. A cpu that goes idle during the bypass window and stays idle won't
 * produce a notification. Arm a re-notify that scx_bypass()'s resched flushes
 * on the next idle pick.
 *
 * An acute case is ops.sub_attach(). If the parent grants the child cids while
 * attaching, when attach is complete and bypass is lifted, the child may hold
 * idle cids it never saw go idle.
 *
 * The root is no exception as bypass suppresses its notifications the same way.
 * However, the root uses a separate per-rq flag so its re-notify keeps working
 * even when !CONFIG_EXT_SUB_SCHED.
 */
ಸ್ಥಿರ ಶೂನ್ಯ unbypass_renotify_idle(ರಚನೆ rq *rq, ರಚನೆ scx_sched *pos,
				   ರಚನೆ scx_sched_pcpu *pcpu)
{
	ಆದರೆ (!pos->level) {
		rq->scx.flags |= SCX_RQ_ಬೇರು_IDLE_RENOTIFY;
		ಹಿಂತಿರುಗಿ;
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	pcpu->idle_renotify = true;
	rq->scx.flags |= SCX_RQ_SUB_IDLE_RENOTIFY;
#ಆದರೆ_ಅಂತ್ಯ
}

/**
 * scx_bypass - [Un]bypass scx_ops and guarantee forward progress
 * @sch: sched to bypass
 * @bypass: true for bypass, false for unbypass
 *
 * Bypassing guarantees that all runnable tasks make forward progress without
 * trusting the BPF scheduler. We can't grab any mutexes or rwsems as they might
 * be held by tasks that the BPF scheduler is forgetting to run, which
 * unfortunately also excludes toggling the static branches.
 *
 * Let's work around by overriding a couple ops and modifying behaviors based on
 * the DISABLING state and then cycling the queued tasks through dequeue/enqueue
 * to force global FIFO scheduling.
 *
 * - ops.select_cpu() is ignored and the default select_cpu() is used.
 *
 * - ops.enqueue() is ignored and tasks are queued in simple global FIFO order.
 *   %SCX_OPS_ENQ_LAST is also ignored.
 *
 * - ops.dispatch() is ignored.
 *
 * - dispatch_one() does not report %SCX_DSP_PREV on non-zero slice as slice
 *   can't be trusted. Whenever a tick triggers, the running task is rotated to
 *   the tail of the queue.
 *
 * - pick_next_task() suppresses zero slice warning.
 *
 * - scx_kick_cpu() is disabled to avoid irq_work malfunction during PM
 *   operations.
 *
 * - scx_prio_less() reverts to the default runnable_at order.
 */
ಶೂನ್ಯ scx_bypass(ರಚನೆ scx_sched *sch, bool bypass)
{
	ರಚನೆ scx_sched *pos;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	ಪೂರ್ಣಾಂಕ cpu;
	bool ಬದಲಾಯಿತು;

	raw_spin_ಬೀಗ_irqsave(&scx_bypass_ಬೀಗ, flags);

	ಆದರೆ (bypass) {
		ಬದಲಾಯಿತು = inc_bypass_ಆಳ(sch);
		ಆದರೆ (ಬದಲಾಯಿತು)
			enable_bypass_dsp(sch);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬದಲಾಯಿತು = dec_bypass_ಆಳ(sch);
	}

	/*
	 * Bypass state is propagated to all descendants - an scx_sched bypasses
	 * if itself or any of its ancestors are in bypass mode. A descendant's
	 * depth is the sum of its own bypasses and its ancestors', so every
	 * call propagates whether or not @sch changed state. The descendants'
	 * claims are released below with @sch's, after their tasks have moved.
	 */
	raw_spin_ಬೀಗ(&scx_sched_ಬೀಗ);
	scx_for_each_descendant_pre(pos, sch) {
		ಆದರೆ (pos == sch)
			ಮುಂದುವರಿ;
		ಆದರೆ (bypass) {
			ಆದರೆ (inc_bypass_ಆಳ(pos))
				enable_bypass_dsp(pos);
		} ಇಲ್ಲದಿದ್ದರೆ {
			dec_bypass_ಆಳ(pos);
		}
	}
	raw_spin_unlock(&scx_sched_ಬೀಗ);

	/*
	 * A descendant's depth is at least @sch's, so no bypass state changed
	 * unless @sch's did.
	 */
	ಆದರೆ (!ಬದಲಾಯಿತು)
		ಹೋಗು unlock;

	/*
	 * No task property is changing. We just need to make sure all currently
	 * queued tasks are re-queued according to the new scx_bypassing()
	 * state. As an optimization, walk each rq's runnable_list instead of
	 * the scx_tasks list.
	 *
	 * This function can't trust the scheduler and thus can't use
	 * cpus_read_lock(). Walk all possible CPUs instead of online.
	 */
	for_each_possible_cpu(cpu) {
		ರಚನೆ rq *rq = cpu_rq(cpu);
		ರಚನೆ task_struct *p, *n;

		raw_spin_rq_ಬೀಗ(rq);
		raw_spin_ಬೀಗ(&scx_sched_ಬೀಗ);

		scx_for_each_descendant_pre(pos, sch) {
			ರಚನೆ scx_sched_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(pos->pcpu, cpu);
			bool was_bypassing = pcpu->flags & SCX_SCHED_PCPU_BYPASSING;

			ಆದರೆ (pos->bypass_ಆಳ) {
				pcpu->flags |= SCX_SCHED_PCPU_BYPASSING;
			} ಇಲ್ಲದಿದ್ದರೆ {
				pcpu->flags &= ~SCX_SCHED_PCPU_BYPASSING;
				ಆದರೆ (was_bypassing) {
					unbypass_renotify_idle(rq, pos, pcpu);
					scx_unbypass_replay_ecaps(rq, pos);
				}
			}
		}

		raw_spin_unlock(&scx_sched_ಬೀಗ);

		/*
		 * We need to guarantee that no tasks are on the BPF scheduler
		 * while bypassing. Either we see enabled or the enable path
		 * sees scx_bypassing() before moving tasks to SCX.
		 */
		ಆದರೆ (!scx_enabled()) {
			scx_rq_ಬೀಗ_drop(rq);
			raw_spin_rq_unlock(rq);
			ಮುಂದುವರಿ;
		}

		/*
		 * The use of list_for_each_entry_safe_reverse() is required
		 * because each task is going to be removed from and added back
		 * to the runnable_list during iteration. Because they're added
		 * to the tail of the list, safe reverse iteration can still
		 * visit all nodes.
		 */
		list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ_ಹಿಮ್ಮುಖ(p, n, &rq->scx.runnable_list,
						 scx.runnable_ಗಂಟು) {
			ಆದರೆ (!scx_is_descendant(scx_task_sched(p), sch))
				ಮುಂದುವರಿ;

			/*
			 * Bypass trumps protection. Cycling clears for queued
			 * tasks but current task needs explicit stripping.
			 */
			ಆದರೆ (bypass && task_current(rq, p))
				scx_task_ತುಂಡು_ended(rq, p);

			/* cycling deq/enq is enough, see the function comment */
			scoped_guard (sched_change, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE) {
				/* nothing */ ;
			}
		}

		/* resched to restore ticks and idle state */
		ಆದರೆ (cpu_online(cpu) || cpu == smp_processor_id())
			resched_curr(rq);

		scx_rq_ಬೀಗ_drop(rq);
		raw_spin_rq_unlock(rq);
	}

	/* disarming must come after moving all tasks out of the bypass DSQs */
	ಆದರೆ (!bypass) {
		guard(raw_spinlock)(&scx_sched_ಬೀಗ);
		scx_for_each_descendant_pre(pos, sch)
			ಆದರೆ (!pos->bypass_ಆಳ)
				scx_disable_bypass_dsp(pos);
	}
unlock:
	raw_spin_unlock_irqrestore(&scx_bypass_ಬೀಗ, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ free_exit_info(ರಚನೆ scx_exit_info *ei)
{
	kvfree(ei->dump);
	kfree(ei->ಸಂದೇಶ);
	kfree(ei->bt);
	kfree(ei);
}

ಸ್ಥಿರ ರಚನೆ scx_exit_info *alloc_exit_info(ಗಾತ್ರ_t exit_dump_len)
{
	ರಚನೆ scx_exit_info *ei;

	ei = kzalloc_obj(*ei);
	ಆದರೆ (!ei)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ei->exit_cpu = -1;
	ei->bt = kzalloc_objs(ei->bt[0], SCX_EXIT_BT_LEN);
	ei->ಸಂದೇಶ = kzalloc(SCX_EXIT_ಸಂದೇಶ_LEN, GFP_KERNEL);
	ei->dump = kvzalloc(exit_dump_len, GFP_KERNEL);

	ಆದರೆ (!ei->bt || !ei->ಸಂದೇಶ || !ei->dump) {
		free_exit_info(ei);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ ei;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scx_exit_reason(ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ)
{
	ಆಯ್ಕೆ (ಬಗೆ) {
	ಸ್ಥಿತಿ SCX_EXIT_UNREG:
		ಹಿಂತಿರುಗಿ "unregistered from user space";
	ಸ್ಥಿತಿ SCX_EXIT_UNREG_BPF:
		ಹಿಂತಿರುಗಿ "unregistered from BPF";
	ಸ್ಥಿತಿ SCX_EXIT_UNREG_KERN:
		ಹಿಂತಿರುಗಿ "unregistered from the main kernel";
	ಸ್ಥಿತಿ SCX_EXIT_SYSRQ:
		ಹಿಂತಿರುಗಿ "disabled by sysrq-S";
	ಸ್ಥಿತಿ SCX_EXIT_ಪೋಷಕ:
		ಹಿಂತಿರುಗಿ "parent exiting";
	ಸ್ಥಿತಿ SCX_EXIT_ಪೋಷಕ_KILL:
		ಹಿಂತಿರುಗಿ "killed by parent scheduler";
	ಸ್ಥಿತಿ SCX_EXIT_ERROR:
		ಹಿಂತಿರುಗಿ "runtime error";
	ಸ್ಥಿತಿ SCX_EXIT_ERROR_BPF:
		ಹಿಂತಿರುಗಿ "scx_bpf_error";
	ಸ್ಥಿತಿ SCX_EXIT_ERROR_STALL:
		ಹಿಂತಿರುಗಿ "runnable task stall";
	ಸ್ಥಿತಿ SCX_EXIT_ERROR_REENQ:
		ಹಿಂತಿರುಗಿ "reenqueue limit";
	ಸ್ಥಿತಿ SCX_EXIT_ERROR_RESCUE:
		ಹಿಂತಿರುಗಿ "rescue bandwidth overload";
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ "<UNKNOWN>";
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ free_kick_syncs(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_kick_syncs __rcu **ksyncs = ಪ್ರತಿ_cpu_ptr(&scx_kick_syncs, cpu);
		ರಚನೆ scx_kick_syncs *ಗಮ್ಯ_free;

		/* flush the pending kick before freeing @ksyncs */
		irq_work_sync(&cpu_rq(cpu)->scx.kick_cpus_irq_work);
		ಗಮ್ಯ_free = rcu_replace_ಸೂಚಕ(*ksyncs, ಖಾಲಿ, true);
		ಆದರೆ (ಗಮ್ಯ_free)
			kvfree_rcu(ಗಮ್ಯ_free, rcu);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ refresh_watchdog(ಶೂನ್ಯ)
{
	ರಚನೆ scx_sched *sch;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ intv = ULONG_MAX;

	/* take the shortest timeout and use its half for watchdog interval */
	rcu_read_ಬೀಗ();
	list_for_each_ನಮೂದು_rcu(sch, &scx_sched_all, all)
		intv = max(min(intv, sch->watchdog_timeout / 2), 1);
	rcu_read_unlock();

	WRITE_ONCE(scx_watchdog_timestamp, jiffies);
	WRITE_ONCE(scx_watchdog_interval, intv);

	ಆದರೆ (intv < ULONG_MAX)
		mod_delayed_work(system_dfl_wq, &scx_watchdog_work, intv);
	ಇಲ್ಲದಿದ್ದರೆ
		cancel_delayed_work_sync(&scx_watchdog_work);
}

s32 scx_link_sched(ರಚನೆ scx_sched *sch)
{
	scoped_guard(raw_spinlock_irqsave, &scx_bypass_ಬೀಗ)	/* for the parent bypass check */
	scoped_guard(raw_spinlock, &scx_sched_ಬೀಗ) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		ರಚನೆ scx_sched *ಪೋಷಕ = scx_ಪೋಷಕ(sch);

		ಆದರೆ (ಪೋಷಕ) {
			s32 ret;

			/*
			 * Bypass state is spread across per-cpu flags and a
			 * depth count, so inheriting it is tricky and has no
			 * valid use case. Refuse it.
			 */
			ಆದರೆ (READ_ONCE(ಪೋಷಕ->bypass_ಆಳ)) {
				scx_error(sch, "parent bypassing (%d)", -EBUSY);
				ಹಿಂತಿರುಗಿ -EBUSY;
			}

			ret = rhashtable_lookup_ಸೇರಿಸು_fast(&scx_sched_hash,
					&sch->hash_ಗಂಟು, scx_sched_hash_params);
			ಆದರೆ (ret) {
				scx_error(sch, "failed to insert into scx_sched_hash (%d)",
					  ret);
				ಹಿಂತಿರುಗಿ ret;
			}

			list_ಕೂಡಿಸು_ಬಾಲ_rcu(&sch->sibling, &ಪೋಷಕ->children);

			/*
			 * Pairs with the mb after the ->aborting assertion in
			 * scx_claim_exit(). Either we see ->aborting and back
			 * out, or the exit path sees us and exits us.
			 */
			smp_mb();
			ಆದರೆ (unlikely(READ_ONCE(ಪೋಷಕ->aborting))) {
				rhashtable_remove_fast(&scx_sched_hash, &sch->hash_ಗಂಟು,
						       scx_sched_hash_params);
				list_del_rcu(&sch->sibling);
				scx_error(sch, "parent disabled (%d)", -ENOENT);
				ಹಿಂತಿರುಗಿ -ENOENT;
			}

			sch->linked = true;
		}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

		list_ಕೂಡಿಸು_ಬಾಲ_rcu(&sch->all, &scx_sched_all);
	}

	refresh_watchdog();
	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ scx_unlink_sched(ರಚನೆ scx_sched *sch)
{
	scoped_guard(raw_spinlock_irq, &scx_sched_ಬೀಗ) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		ಆದರೆ (sch->linked) {
			rhashtable_remove_fast(&scx_sched_hash, &sch->hash_ಗಂಟು,
					       scx_sched_hash_params);
			list_del_rcu(&sch->sibling);
			sch->linked = false;
		}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */
		list_del_rcu(&sch->all);
	}

	refresh_watchdog();
}

/*
 * Called to disable future dumps and wait for in-progress one while disabling
 * @sch. Once @sch becomes empty during disable, there's no point in dumping it.
 * This prevents calling dump ops on a dead sch.
 */
ಶೂನ್ಯ scx_disable_dump(ರಚನೆ scx_sched *sch)
{
	guard(raw_spinlock_irqsave)(&scx_dump_ಬೀಗ);
	sch->dump_disabled = true;
}

ಶೂನ್ಯ scx_log_sched_disable(ರಚನೆ scx_sched *sch)
{
	ರಚನೆ scx_exit_info *ei = sch->exit_info;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type = scx_ಪೋಷಕ(sch) ? "sub-scheduler" : "scheduler";

	ಆದರೆ (ei->ಬಗೆ >= SCX_EXIT_ERROR) {
		pr_err("sched_ext: BPF %s \"%s\" disabled (%s)\n", type,
		       sch->ops.name, ei->reason);

		ಆದರೆ (ei->ಸಂದೇಶ[0] != '\0')
			pr_err("sched_ext: %s: %s\n", sch->ops.name, ei->ಸಂದೇಶ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_STACKTRACE
		ರಾಶಿ_trace_print(ei->bt, ei->bt_len, 2);
#ಆದರೆ_ಅಂತ್ಯ
	} ಇಲ್ಲದಿದ್ದರೆ {
		pr_info("sched_ext: BPF %s \"%s\" disabled (%s)\n", type,
			sch->ops.name, ei->reason);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_ಬೇರು_disable(ರಚನೆ scx_sched *sch)
{
	ರಚನೆ scx_task_iter sti;
	ರಚನೆ task_struct *p;
	bool was_switched_all;
	ಪೂರ್ಣಾಂಕ cpu;

	/* guarantee forward progress and wait for descendants to be disabled */
	scx_bypass(sch, true);
	drain_descendants(sch);

	ಆಯ್ಕೆ (scx_ಗಣ_enable_state(SCX_DISABLING)) {
	ಸ್ಥಿತಿ SCX_DISABLING:
		WARN_ONCE(true, "sched_ext: duplicate disabling instance?");
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCX_DISABLED:
		pr_warn("sched_ext: ops error detected without ops (%s)\n",
			sch->exit_info->ಸಂದೇಶ);
		WARN_ON_ONCE(scx_ಗಣ_enable_state(SCX_DISABLED) != SCX_DISABLING);
		ಹೋಗು ಮುಗಿದ;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ನಿಲ್ಲಿಸು;
	}

	/*
	 * Here, every runnable task is guaranteed to make forward progress and
	 * we can safely use blocking synchronization constructs. Actually
	 * disable ops.
	 */
	mutex_ಬೀಗ(&scx_enable_mutex);

	was_switched_all = scx_switched_all();

	static_branch_disable(&__scx_switched_all);
	WRITE_ONCE(scx_switching_all, false);

	/*
	 * Shut down cgroup support before tasks so that the cgroup attach and
	 * migration paths don't race against scx_disable_and_exit_task().
	 */
	scx_cgroup_ಬೀಗ();
	scx_cgroup_enabled = false;
	scx_cgroup_exit(sch);
	scx_cgroup_unlock();

	/*
	 * The BPF scheduler is going away. All tasks including %TASK_DEAD ones
	 * must be switched out and exited synchronously.
	 */
	percpu_ಕೆಳಕ್ಕೆ_write(&scx_fork_rwsem);

	scx_init_task_enabled = false;

	scx_task_iter_ಆರಂಭ(&sti, ಖಾಲಿ);
	ಆಗ ((p = scx_task_iter_ಮುಂದಿನ_locked(&sti))) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸರತಿ_flags = ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE | ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK;
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *old_class = p->sched_class;
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *new_class = scx_setscheduler_class(p);

		update_rq_clock(task_rq(p));

		ಆದರೆ (old_class != new_class)
			ಸರತಿ_flags |= ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS;

		scoped_guard (sched_change, p, ಸರತಿ_flags) {
			p->sched_class = new_class;
		}

		scx_disable_and_exit_task(scx_task_sched(p), p);
	}
	scx_task_iter_stop(&sti);

	scx_disable_dump(sch);

	scx_cgroup_ಬೀಗ();
	ಗಣ_cgroup_sched(sch_cgroup(sch), ಖಾಲಿ);
	scx_cgroup_unlock();

	percpu_ಮೇಲೆ_write(&scx_fork_rwsem);

	/*
	 * Re-balance the dl_server bandwidth reservations: detach ext_server
	 * (no more sched_ext tasks) and reinstate fair_server if it was
	 * previously detached because we were running in full mode.
	 *
	 * Unlike the enable path, this runs on a recovery path that cannot
	 * fail, so we use dl_server_swap_bw() to atomically free ext_server's
	 * bandwidth and reclaim it for fair_server under the same dl_b lock.
	 *
	 * The swap can still fail with -EBUSY if someone bumped ext_server's
	 * runtime via debugfs between enable and disable; in that narrow case
	 * both servers end up detached and we just WARN.
	 */
	for_each_possible_cpu(cpu) {
		ರಚನೆ rq *rq = cpu_rq(cpu);

		scoped_guard(rq_ಬೀಗ_irqsave, rq) {
			update_rq_clock(rq);
			ಆದರೆ (was_switched_all) {
				ಆದರೆ (WARN_ON_ONCE(dl_server_ಅದಲುಬದಲು_bw(&rq->ext_server,
								   &rq->fair_server)))
					pr_warn("failed to re-attach fair_server on CPU %d\n", cpu);
			} ಇಲ್ಲದಿದ್ದರೆ {
				dl_server_detach_bw(&rq->ext_server);
			}
		}
	}

	/* no task is on scx, turn off all the switches and flush in-progress calls */
	static_branch_disable(&__scx_enabled);
	static_branch_disable(&__scx_is_cid_type);
	ಆದರೆ (sch->ops.flags & SCX_OPS_TID_ಗಮ್ಯ_TASK)
		static_branch_disable(&__scx_tid_to_task_enabled);
	bitmap_zero(sch->has_op, SCX_OPI_ಕೊನೆ);
	scx_idle_disable();
	synchronize_rcu();
	ಆದರೆ (sch->ops.flags & SCX_OPS_TID_ಗಮ್ಯ_TASK)
		rhashtable_free_and_destroy(&scx_tid_hash, ಖಾಲಿ, ಖಾಲಿ);

	scx_log_sched_disable(sch);

	ಆದರೆ (sch->ops.ನಿರ್ಗಮಿಸು)
		SCX_CALL_OP(sch, ನಿರ್ಗಮಿಸು, ಖಾಲಿ, sch->exit_info);

	/*
	 * @sch's non-ops programs such as timers and tracers can fire after
	 * ops.exit(). Now that exit is complete, stop scx_prog_sched() from
	 * resolving to @sch and drain in-flight resolvers.
	 */
	WRITE_ONCE(sch->dead, true);
	synchronize_rcu();

	scx_unlink_sched(sch);

	/*
	 * scx_root clearing and cid table retirement must be inside
	 * cpus_read_lock(). See handle_hotplug().
	 */
	cpus_read_ಬೀಗ();
	RCU_INIT_ಸೂಚಕ(scx_ಬೇರು, ಖಾಲಿ);
	scx_cid_retire_tables();
	cpus_read_unlock();

	/*
	 * Delete the kobject from the hierarchy synchronously. Otherwise, sysfs
	 * could observe an object of the same name still in the hierarchy when
	 * the next scheduler is loaded.
	 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಆದರೆ (sch->sub_kset)
		kobject_del(&sch->sub_kset->kobj);
#ಆದರೆ_ಅಂತ್ಯ
	/* not added if enable failed before scx_sched_sysfs_add() */
	ಆದರೆ (sch->kobj.state_ಒಳ_sysfs)
		kobject_del(&sch->kobj);

	free_kick_syncs();

	mutex_unlock(&scx_enable_mutex);

	WARN_ON_ONCE(scx_ಗಣ_enable_state(SCX_DISABLED) != SCX_DISABLING);
ಮುಗಿದ:
	scx_bypass(sch, false);
}

/**
 * scx_propagate_exit_irq_workfn - Claim SCX_EXIT_PARENT on the exiting subtree
 * @irq_work: &scx_sched.propagate_exit_irq_work
 *
 * Queued by scx_claim_exit() after a non-PARENT claim. Claims SCX_EXIT_PARENT
 * on each descendant, giving every one its own disable work - most of disabling
 * is serialized but ops.exit() can take arbitrarily long and running them in
 * separate helper kthreads parallelizes it. No recursion as only non-PARENT
 * claims propagate.
 */
ಸ್ಥಿರ ಶೂನ್ಯ scx_propagate_exit_irq_workfn(ರಚನೆ irq_work *irq_work)
{
	ರಚನೆ scx_sched *sch = container_ದ(irq_work, ರಚನೆ scx_sched,
					     propagate_exit_irq_work);
	ರಚನೆ scx_sched *pos;

	scoped_guard (raw_spinlock_irqsave, &scx_sched_ಬೀಗ) {
		scx_for_each_descendant_pre(pos, sch)
			scx_disable(pos, SCX_EXIT_ಪೋಷಕ);
	}
}

/*
 * Claim the exit on @sch. The caller must ensure that the helper kthread work
 * is kicked before the current task can be preempted. Once exit_kind is
 * claimed, scx_error() can no longer trigger, so if the current task gets
 * preempted and the BPF scheduler fails to schedule it back, the helper work
 * will never be kicked and the whole system can wedge.
 *
 * Lock-free and safe to call from any context including NMI.
 */
ಸ್ಥಿರ bool scx_claim_exit(ರಚನೆ scx_sched *sch, ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ)
{
	ಪೂರ್ಣಾಂಕ none = SCX_EXIT_NONE;

	lockdep_assert_preemption_disabled();

	ಆದರೆ (WARN_ON_ONCE(ಬಗೆ == SCX_EXIT_NONE || ಬಗೆ == SCX_EXIT_ಮುಗಿದ))
		ಬಗೆ = SCX_EXIT_ERROR;

	ಆದರೆ (!atomic_try_cmpxchg(&sch->exit_ಬಗೆ, &none, ಬಗೆ))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (ಬಗೆ == SCX_EXIT_ಪೋಷಕ) {
		/* an ancestor is already sweeping the subtree */
		WRITE_ONCE(sch->aborting, true);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ರಚನೆ scx_sched *pos;

		/*
		 * CPUs may be live-locked in the dispatch paths of @sch or its
		 * descendants, which ->aborting breaks. Sweep the subtree
		 * locklessly so that this works from NMI. smp_store_mb() orders
		 * each node's ->aborting store before its children are walked -
		 * either we see a racing scx_link_sched() on ->children or it
		 * sees ->aborting.
		 */
		scoped_guard (rcu) {
			scx_for_each_descendant_pre(pos, sch)
				smp_store_mb(pos->aborting, true);
		}

		irq_work_ಸರತಿ(&sch->propagate_exit_irq_work);
	}

	/* fired after ->aborting is set so callbacks can't delay recovery */
	trace_sched_ext_exit(sch, ಬಗೆ);

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_disable_workfn(ರಚನೆ kthread_work *work)
{
	ರಚನೆ scx_sched *sch = container_ದ(work, ರಚನೆ scx_sched, disable_work);
	ರಚನೆ scx_exit_info *ei = sch->exit_info;
	ಪೂರ್ಣಾಂಕ ಬಗೆ;

	ಬಗೆ = atomic_read(&sch->exit_ಬಗೆ);
	ಆಗ (true) {
		ಆದರೆ (ಬಗೆ == SCX_EXIT_ಮುಗಿದ)	/* already disabled? */
			ಹಿಂತಿರುಗಿ;
		WARN_ON_ONCE(ಬಗೆ == SCX_EXIT_NONE);
		ಆದರೆ (atomic_try_cmpxchg(&sch->exit_ಬಗೆ, &ಬಗೆ, SCX_EXIT_ಮುಗಿದ))
			ನಿಲ್ಲಿಸು;
	}
	ei->ಬಗೆ = ಬಗೆ;
	ei->reason = scx_exit_reason(ei->ಬಗೆ);

	ಆದರೆ (scx_ಪೋಷಕ(sch))
		scx_sub_disable(sch);
	ಇಲ್ಲದಿದ್ದರೆ
		scx_ಬೇರು_disable(sch);
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_disable(ರಚನೆ scx_sched *sch, ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ)
{
	guard(preempt)();
	ಆದರೆ (scx_claim_exit(sch, ಬಗೆ))
		irq_work_ಸರತಿ(&sch->disable_irq_work);
}

/**
 * scx_flush_disable_work - flush the disable work and wait for it to finish
 * @sch: the scheduler
 *
 * sch->disable_work might still not queued, causing kthread_flush_work()
 * as a noop. Syncing the irq_work first is required to guarantee the
 * kthread work has been queued before waiting for it.
 */
ಶೂನ್ಯ scx_flush_disable_work(ರಚನೆ scx_sched *sch)
{
	ಪೂರ್ಣಾಂಕ ಬಗೆ;

	ಮಾಡು {
		irq_work_sync(&sch->disable_irq_work);
		kthread_flush_work(&sch->disable_work);
		ಬಗೆ = atomic_read(&sch->exit_ಬಗೆ);
	} ಆಗ (ಬಗೆ != SCX_EXIT_NONE && ಬಗೆ != SCX_EXIT_ಮುಗಿದ);
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_newline(ರಚನೆ seq_buf *s)
{
	trace_sched_ext_dump("");

	/* @s may be zero sized and seq_buf triggers WARN if so */
	ಆದರೆ (s->ಗಾತ್ರ_2)
		seq_buf_putc(s, '\n');
}

__printf(2, 3) ಶೂನ್ಯ scx_dump_ಸಾಲು(ರಚನೆ seq_buf *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...)
{
	va_list ವಾದಗಳು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_TRACEPOINTS
	ಆದರೆ (trace_sched_ext_dump_enabled()) {
		/* protected by scx_dump_lock */
		ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_buf[SCX_EXIT_ಸಂದೇಶ_LEN];

		va_ಆರಂಭ(ವಾದಗಳು, fmt);
		vscnprintf(ಸಾಲು_buf, ಗಾತ್ರ(ಸಾಲು_buf), fmt, ವಾದಗಳು);
		va_ಕೊನೆ(ವಾದಗಳು);

		trace_call__sched_ext_dump(ಸಾಲು_buf);
	}
#ಆದರೆ_ಅಂತ್ಯ
	/* @s may be zero sized and seq_buf triggers WARN if so */
	ಆದರೆ (s->ಗಾತ್ರ_2) {
		va_ಆರಂಭ(ವಾದಗಳು, fmt);
		seq_buf_vprintf(s, fmt, ವಾದಗಳು);
		va_ಕೊನೆ(ವಾದಗಳು);

		seq_buf_putc(s, '\n');
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ dump_ರಾಶಿ_trace(ರಚನೆ seq_buf *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ,
			     ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *bt, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < len; i++)
		scx_dump_ಸಾಲು(s, "%s%pS", ಪೂರ್ವಪ್ರತ್ಯಯ, (ಶೂನ್ಯ *)bt[i]);
}

ಸ್ಥಿರ ಶೂನ್ಯ ops_dump_init(ರಚನೆ seq_buf *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ)
{
	ರಚನೆ scx_dump_ದತ್ತಾಂಶ *dd = &scx_dump_ದತ್ತಾಂಶ;

	lockdep_assert_irqs_disabled();

	dd->cpu = smp_processor_id();		/* allow scx_bpf_dump() */
	dd->ಮೊದಲ = true;
	dd->cursor = 0;
	dd->s = s;
	dd->ಪೂರ್ವಪ್ರತ್ಯಯ = ಪೂರ್ವಪ್ರತ್ಯಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ops_dump_flush(ಶೂನ್ಯ)
{
	ರಚನೆ scx_dump_ದತ್ತಾಂಶ *dd = &scx_dump_ದತ್ತಾಂಶ;
	ಅಕ್ಷರ *ಸಾಲು = dd->buf.ಸಾಲು;

	ಆದರೆ (!dd->cursor)
		ಹಿಂತಿರುಗಿ;

	/*
	 * There's something to flush and this is the first line. Insert a blank
	 * line to distinguish ops dump.
	 */
	ಆದರೆ (dd->ಮೊದಲ) {
		dump_newline(dd->s);
		dd->ಮೊದಲ = false;
	}

	/*
	 * There may be multiple lines in $line. Scan and emit each line
	 * separately.
	 */
	ಆಗ (true) {
		ಅಕ್ಷರ *ಕೊನೆ = ಸಾಲು;
		ಅಕ್ಷರ c;

		ಆಗ (*ಕೊನೆ != '\n' && *ಕೊನೆ != '\0')
			ಕೊನೆ++;

		/*
		 * If $line overflowed, it may not have newline at the end.
		 * Always emit with a newline.
		 */
		c = *ಕೊನೆ;
		*ಕೊನೆ = '\0';
		scx_dump_ಸಾಲು(dd->s, "%s%s", dd->ಪೂರ್ವಪ್ರತ್ಯಯ, ಸಾಲು);
		ಆದರೆ (c == '\0')
			ನಿಲ್ಲಿಸು;

		/* move to the next line */
		ಕೊನೆ++;
		ಆದರೆ (*ಕೊನೆ == '\0')
			ನಿಲ್ಲಿಸು;
		ಸಾಲು = ಕೊನೆ;
	}

	dd->cursor = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ops_dump_exit(ಶೂನ್ಯ)
{
	ops_dump_flush();
	scx_dump_ದತ್ತಾಂಶ.cpu = -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_dump_task(ರಚನೆ scx_sched *sch, ರಚನೆ seq_buf *s, ರಚನೆ scx_dump_ctx *dctx,
			  ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಅಕ್ಷರ marker)
{
	ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bt[SCX_EXIT_BT_LEN];
	ರಚನೆ scx_sched *task_sch = scx_task_sched(p);
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *own_marker;
	ಅಕ್ಷರ sch_id_buf[32];
	ಅಕ್ಷರ dsq_id_buf[19] = "(n/a)";
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ops_state = atomic_long_read(&p->scx.ops_state);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ bt_len = 0;

	own_marker = task_sch == sch ? "*" : "";

	ಆದರೆ (task_sch->level == 0)
		scnprintf(sch_id_buf, ಗಾತ್ರ(sch_id_buf), "root");
	ಇಲ್ಲದಿದ್ದರೆ
		scnprintf(sch_id_buf, ಗಾತ್ರ(sch_id_buf), "sub%d-%llu",
			  task_sch->level, task_sch->ops.sub_cgroup_id);

	ಆದರೆ (p->scx.dsq)
		scnprintf(dsq_id_buf, ಗಾತ್ರ(dsq_id_buf), "0x%llx",
			  (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)p->scx.dsq->id);

	dump_newline(s);
	scx_dump_ಸಾಲು(s, " %c%c %s[%d] %s%s %+ldms",
		      marker, task_state_ಗಮ್ಯ_char(p), p->comm, p->pid, own_marker, sch_id_buf,
		      jiffies_delta_msecs(p->scx.runnable_at, dctx->at_jiffies));
	scx_dump_ಸಾಲು(s, "      scx_state/flags=%u/0x%x dsq_flags=0x%x ops_state/qseq=%lu/%lu",
		      scx_get_task_state(p) >> SCX_TASK_STATE_ಸರಿಸು,
		      p->scx.flags & ~SCX_TASK_STATE_ಮುಖವಾಡ, p->scx.dsq_flags,
		      ops_state & SCX_OPSS_STATE_ಮುಖವಾಡ, ops_state >> SCX_OPSS_QSEQ_ಸರಿಸು);
	scx_dump_ಸಾಲು(s, "      sticky/holding_cpu=%d/%d dsq_id=%s",
		      p->scx.sticky_cpu, p->scx.holding_cpu, dsq_id_buf);
	scx_dump_ಸಾಲು(s, "      dsq_vtime=%llu slice=%llu weight=%u",
		      p->scx.dsq_vtime, p->scx.ತುಂಡು, p->scx.ತೂಕ);
	scx_dump_ಸಾಲು(s, "      cpus=%*pb no_mig=%u", cpumask_pr_ವಾದಗಳು(p->cpus_ptr),
		      p->migration_disabled);

	ಆದರೆ (SCX_HAS_OP(sch, dump_task)) {
		ops_dump_init(s, "    ");
		SCX_CALL_OP(sch, dump_task, rq, dctx, p);
		ops_dump_exit();
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_STACKTRACE
	bt_len = ರಾಶಿ_trace_save_tsk(p, bt, SCX_EXIT_BT_LEN, 1);
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (bt_len) {
		dump_newline(s);
		dump_ರಾಶಿ_trace(s, "    ", bt, bt_len);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_dump_cpu(ರಚನೆ scx_sched *sch, ರಚನೆ seq_buf *s,
			 ರಚನೆ scx_dump_ctx *dctx, ಪೂರ್ಣಾಂಕ cpu,
			 bool dump_all_tasks)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ರಚನೆ scx_sched_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu);
	ರಚನೆ rq_flags rf;
	ರಚನೆ task_struct *p;
	ರಚನೆ seq_buf ns;
	ಗಾತ್ರ_t avail, ಬಳಸಿದ;
	ಅಕ್ಷರ *buf;
	bool idle;

	rq_ಬೀಗ_irqsave(rq, &rf);

	idle = list_ಖಾಲಿಗಳು(&rq->scx.runnable_list) &&
		rq->curr->sched_class == &idle_sched_class;

	ಆದರೆ (idle && !SCX_HAS_OP(sch, dump_cpu))
		ಹೋಗು ಮುಂದಿನ;

	/*
	 * We don't yet know whether ops.dump_cpu() will produce output
	 * and we may want to skip the default CPU dump if it doesn't.
	 * Use a nested seq_buf to generate the standard dump so that we
	 * can decide whether to commit later.
	 */
	avail = seq_buf_get_buf(s, &buf);
	seq_buf_init(&ns, buf, avail);

	dump_newline(&ns);
	scx_dump_ಸಾಲು(&ns, "CPU %-4d: nr_run=%u flags=0x%x cpu_rel=%d ksync=%lu",
		      cpu, rq->scx.nr_running, rq->scx.flags, rq->scx.cpu_released,
		      rq->scx.kick_sync);
	scx_rescue_dump(&ns, rq);
	scx_dump_ಸಾಲು(&ns, "          curr=%s[%d] class=%ps",
		      rq->curr->comm, rq->curr->pid, rq->curr->sched_class);
	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(pcpu->cpus_ಗಮ್ಯ_kick))
		scx_dump_ಸಾಲು(&ns, "  cpus_to_kick   : %*pb",
			      cpumask_pr_ವಾದಗಳು(pcpu->cpus_ಗಮ್ಯ_kick));
	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(pcpu->cpus_ಗಮ್ಯ_kick_if_idle))
		scx_dump_ಸಾಲು(&ns, "  idle_to_kick   : %*pb",
			      cpumask_pr_ವಾದಗಳು(pcpu->cpus_ಗಮ್ಯ_kick_if_idle));
	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(pcpu->cpus_ಗಮ್ಯ_preempt))
		scx_dump_ಸಾಲು(&ns, "  cpus_to_preempt: %*pb",
			      cpumask_pr_ವಾದಗಳು(pcpu->cpus_ಗಮ್ಯ_preempt));
	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(pcpu->cpus_ಗಮ್ಯ_wait))
		scx_dump_ಸಾಲು(&ns, "  cpus_to_wait   : %*pb",
			      cpumask_pr_ವಾದಗಳು(pcpu->cpus_ಗಮ್ಯ_wait));
	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(rq->scx.cpus_ಗಮ್ಯ_sync))
		scx_dump_ಸಾಲು(&ns, "  cpus_to_sync   : %*pb",
			      cpumask_pr_ವಾದಗಳು(rq->scx.cpus_ಗಮ್ಯ_sync));

	ಬಳಸಿದ = seq_buf_ಬಳಸಿದ(&ns);
	ಆದರೆ (SCX_HAS_OP(sch, dump_cpu)) {
		ops_dump_init(&ns, "  ");
		SCX_CALL_OP(sch, dump_cpu, rq, dctx, scx_cpu_arg(cpu), idle);
		ops_dump_exit();
	}

	/*
	 * If idle && nothing generated by ops.dump_cpu(), there's
	 * nothing interesting. Skip.
	 */
	ಆದರೆ (idle && ಬಳಸಿದ == seq_buf_ಬಳಸಿದ(&ns))
		ಹೋಗು ಮುಂದಿನ;

	/*
	 * $s may already have overflowed when $ns was created. If so,
	 * calling commit on it will trigger BUG.
	 */
	ಆದರೆ (avail) {
		seq_buf_commit(s, seq_buf_ಬಳಸಿದ(&ns));
		ಆದರೆ (seq_buf_has_overflowed(&ns))
			seq_buf_ಗಣ_ಅತಿಪ್ರವಾಹ(s);
	}

	ಆದರೆ (rq->curr->sched_class == &ext_sched_class &&
	    (dump_all_tasks || scx_task_on_sched(sch, rq->curr)))
		scx_dump_task(sch, s, dctx, rq, rq->curr, '*');

	list_for_each_ನಮೂದು(p, &rq->scx.runnable_list, scx.runnable_ಗಂಟು)
		ಆದರೆ (dump_all_tasks || scx_task_on_sched(sch, p))
			scx_dump_task(sch, s, dctx, rq, p, ' ');
ಮುಂದಿನ:
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Dump scheduler state. If @dump_all_tasks is true, dump all tasks regardless
 * of which scheduler they belong to. If false, only dump tasks owned by @sch.
 * For SysRq-D dumps, @dump_all_tasks=false since all schedulers are dumped
 * separately. For error dumps, @dump_all_tasks=true since only the failing
 * scheduler is dumped.
 */
ಸ್ಥಿರ ಶೂನ್ಯ scx_dump_state(ರಚನೆ scx_sched *sch, ರಚನೆ scx_exit_info *ei,
			   ಗಾತ್ರ_t dump_len, bool dump_all_tasks)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ trunc_marker[] = "\n\n~~~~ TRUNCATED ~~~~\n";
	ರಚನೆ scx_dump_ctx dctx = {
		.ಬಗೆ = ei->ಬಗೆ,
		.exit_ಸಂಕೇತ = ei->exit_ಸಂಕೇತ,
		.reason = ei->reason,
		.at_ns = ktime_get_ns(),
		.at_jiffies = jiffies,
	};
	ರಚನೆ seq_buf s;
	ರಚನೆ scx_event_stats events;
	ಪೂರ್ಣಾಂಕ cpu;

	guard(raw_spinlock_irqsave)(&scx_dump_ಬೀಗ);

	ಆದರೆ (sch->dump_disabled)
		ಹಿಂತಿರುಗಿ;

	seq_buf_init(&s, ei->dump, dump_len);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಆದರೆ (sch->level == 0)
		scx_dump_ಸಾಲು(&s, "%s: root", sch->ops.name);
	ಇಲ್ಲದಿದ್ದರೆ
		scx_dump_ಸಾಲು(&s, "%s: sub%d-%llu %s",
			      sch->ops.name, sch->level, sch->ops.sub_cgroup_id,
			      sch->cgrp_ಮಾರ್ಗ);
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (ei->ಬಗೆ == SCX_EXIT_NONE) {
		scx_dump_ಸಾಲು(&s, "Debug dump triggered by %s", ei->reason);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ei->exit_cpu >= 0)
			scx_dump_ಸಾಲು(&s, "%s[%d] triggered exit kind %d on CPU %d:",
				      current->comm, current->pid, ei->ಬಗೆ,
				      ei->exit_cpu);
		ಇಲ್ಲದಿದ್ದರೆ
			scx_dump_ಸಾಲು(&s, "%s[%d] triggered exit kind %d:",
				      current->comm, current->pid, ei->ಬಗೆ);
		scx_dump_ಸಾಲು(&s, "  %s (%s)", ei->reason, ei->ಸಂದೇಶ);
		dump_newline(&s);
		scx_dump_ಸಾಲು(&s, "Backtrace:");
		dump_ರಾಶಿ_trace(&s, "  ", ei->bt, ei->bt_len);
	}

	ಆದರೆ (SCX_HAS_OP(sch, dump)) {
		ops_dump_init(&s, "");
		SCX_CALL_OP(sch, dump, ಖಾಲಿ, &dctx);
		ops_dump_exit();
	}

	dump_newline(&s);
	scx_dump_ಸಾಲು(&s, "CPU states");
	scx_dump_ಸಾಲು(&s, "----------");

	/*
	 * Dump stalled CPUs first so they aren't lost to dump truncation, then
	 * walk the rest in order. Fall back to exit_cpu if no stall mask set.
	 */
	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(sch->stall_cpus)) {
		for_each_cpu(cpu, sch->stall_cpus)
			scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		for_each_possible_cpu(cpu) {
			ಆದರೆ (!cpumask_test_cpu(cpu, sch->stall_cpus))
				scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ei->exit_cpu >= 0)
			scx_dump_cpu(sch, &s, &dctx, ei->exit_cpu, dump_all_tasks);
		for_each_possible_cpu(cpu) {
			ಆದರೆ (cpu != ei->exit_cpu)
				scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		}
	}

	dump_newline(&s);
	scx_dump_ಸಾಲು(&s, "Event counters");
	scx_dump_ಸಾಲು(&s, "--------------");

	scx_read_events(sch, &events);
#ವ್ಯಾಖ್ಯಾನಿಸು SCX_EVENT(name)	scx_dump_event(s, &events, name)
	SCX_EVENTS_LIST(SCX_EVENT);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCX_EVENT

	ಆದರೆ (seq_buf_has_overflowed(&s) && dump_len >= ಗಾತ್ರ(trunc_marker))
		ಸ್ಮರಣೆ_ನಕಲು(ei->dump + dump_len - ಗಾತ್ರ(trunc_marker),
		       trunc_marker, ಗಾತ್ರ(trunc_marker));
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_disable_irq_workfn(ರಚನೆ irq_work *irq_work)
{
	ರಚನೆ scx_sched *sch = container_ದ(irq_work, ರಚನೆ scx_sched, disable_irq_work);
	ರಚನೆ scx_exit_info *ei = sch->exit_info;

	ಆದರೆ (ei->ಬಗೆ >= SCX_EXIT_ERROR)
		scx_dump_state(sch, ei, sch->ops.exit_dump_len, true);

	kthread_ಸರತಿ_work(sch->helper, &sch->disable_work);
}

/* finish exit_info and kick the disable work, ei->msg must already be set */
ಸ್ಥಿರ ಶೂನ್ಯ scx_ಮುಕ್ತಾಯ_exit(ರಚನೆ scx_sched *sch, ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ,
			    s64 exit_ಸಂಕೇತ, s32 exit_cpu)
{
	ರಚನೆ scx_exit_info *ei = sch->exit_info;

	ei->exit_ಸಂಕೇತ = exit_ಸಂಕೇತ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_STACKTRACE
	/*
	 * stack_trace_save()'s NMI-safety is arch-dependent and undocumented.
	 * Skip the backtrace when exiting from NMI.
	 */
	ಆದರೆ (ಬಗೆ >= SCX_EXIT_ERROR && !ಒಳ_nmi())
		ei->bt_len = ರಾಶಿ_trace_save(ei->bt, SCX_EXIT_BT_LEN, 1);
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * Set ei->kind and ->reason for scx_dump_state(). They'll be set again
	 * in scx_disable_workfn().
	 */
	ei->ಬಗೆ = ಬಗೆ;
	ei->reason = scx_exit_reason(ei->ಬಗೆ);
	ei->exit_cpu = exit_cpu;

	irq_work_ಸರತಿ(&sch->disable_irq_work);
}

bool scx_vexit(ರಚನೆ scx_sched *sch,
	       ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ, s64 exit_ಸಂಕೇತ, s32 exit_cpu,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, va_list ವಾದಗಳು)
{
	ರಚನೆ scx_exit_info *ei = sch->exit_info;

	guard(preempt)();

	ಆದರೆ (!scx_claim_exit(sch, ಬಗೆ))
		ಹಿಂತಿರುಗಿ false;

	vscnprintf(ei->ಸಂದೇಶ, SCX_EXIT_ಸಂದೇಶ_LEN, fmt, ವಾದಗಳು);

	scx_ಮುಕ್ತಾಯ_exit(sch, ಬಗೆ, exit_ಸಂಕೇತ, exit_cpu);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ alloc_kick_syncs(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cpu;

	/*
	 * Allocate per-CPU arrays sized by nr_cpu_ids. Use kvzalloc as size
	 * can exceed percpu allocator limits on large machines.
	 */
	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_kick_syncs __rcu **ksyncs = ಪ್ರತಿ_cpu_ptr(&scx_kick_syncs, cpu);
		ರಚನೆ scx_kick_syncs *new_ksyncs;

		WARN_ON_ONCE(rcu_access_ಸೂಚಕ(*ksyncs));

		new_ksyncs = kvzalloc_ಗಂಟು(struct_ಗಾತ್ರ(new_ksyncs, syncs, nr_cpu_ids),
					   GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(cpu));
		ಆದರೆ (!new_ksyncs) {
			free_kick_syncs();
			ಹಿಂತಿರುಗಿ -ENOMEM;
		}

		rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(*ksyncs, new_ksyncs);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ free_pnode(ರಚನೆ scx_sched_pnode *pnode)
{
	ಆದರೆ (!pnode)
		ಹಿಂತಿರುಗಿ;
	exit_dsq(&pnode->global_dsq);
	kfree(pnode);
}

ಸ್ಥಿರ ರಚನೆ scx_sched_pnode *alloc_pnode(ರಚನೆ scx_sched *sch, ಪೂರ್ಣಾಂಕ ಗಂಟು)
{
	ರಚನೆ scx_sched_pnode *pnode;

	pnode = kzalloc_ಗಂಟು(ಗಾತ್ರ(*pnode), GFP_KERNEL, ಗಂಟು);
	ಆದರೆ (!pnode)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (scx_init_dsq(&pnode->global_dsq, SCX_DSQ_GLOBAL, sch)) {
		kfree(pnode);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ pnode;
}

/*
 * Allocate and initialize a new scx_sched. @cgrp's reference is always
 * consumed whether the function succeeds or fails.
 */
ರಚನೆ scx_sched *scx_alloc_and_ಕೂಡಿಸು_sched(ರಚನೆ scx_enable_cmd *cmd,
					  ರಚನೆ cgroup *cgrp,
					  ರಚನೆ scx_sched *ಪೋಷಕ)
{
	ರಚನೆ sched_ext_ops *ops = cmd->ops;
	ರಚನೆ scx_sched *sch;
	s32 level = ಪೋಷಕ ? ಪೋಷಕ->level + 1 : 0;
	s32 ಗಂಟು, cpu, ret, bypass_fail_cpu = nr_cpu_ids;

	sch = kzalloc_flex(*sch, ancestors, level + 1);
	ಆದರೆ (!sch) {
		ret = -ENOMEM;
		ಹೋಗು err_put_cgrp;
	}

	sch->exit_info = alloc_exit_info(ops->exit_dump_len);
	ಆದರೆ (!sch->exit_info) {
		ret = -ENOMEM;
		ಹೋಗು err_free_sch;
	}

	ret = rhashtable_init(&sch->dsq_hash, &dsq_hash_params);
	ಆದರೆ (ret < 0)
		ಹೋಗು err_free_ei;

	sch->pnode = kzalloc_objs(sch->pnode[0], nr_ಗಂಟು_ids);
	ಆದರೆ (!sch->pnode) {
		ret = -ENOMEM;
		ಹೋಗು err_free_hash;
	}

	for_each_ಗಂಟು_state(ಗಂಟು, N_POSSIBLE) {
		sch->pnode[ಗಂಟು] = alloc_pnode(sch, ಗಂಟು);
		ಆದರೆ (!sch->pnode[ಗಂಟು]) {
			ret = -ENOMEM;
			ಹೋಗು err_free_pnode;
		}
	}

	sch->dsp_max_batch = ops->dispatch_max_batch ?: SCX_DSP_DFL_MAX_BATCH;
	sch->pcpu = __alloc_percpu(struct_ಗಾತ್ರ_t(ರಚನೆ scx_sched_pcpu,
						 dsp_ctx.buf, sch->dsp_max_batch),
				   __alignof__(ರಚನೆ scx_sched_pcpu));
	ಆದರೆ (!sch->pcpu) {
		ret = -ENOMEM;
		ಹೋಗು err_free_pnode;
	}

	for_each_possible_cpu(cpu) {
		ret = scx_init_dsq(scx_bypass_dsq(sch, cpu), SCX_DSQ_BYPASS, sch);
		ಆದರೆ (ret) {
			bypass_fail_cpu = cpu;
			ಹೋಗು err_free_pcpu;
		}
	}

	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_sched_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu);

		ಗಂಟು = cpu_ಗಮ್ಯ_ಗಂಟು(cpu);
		pcpu->sch = sch;
		INIT_LIST_ತಲೆ(&pcpu->deferred_reenq_local.ಗಂಟು);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		init_llist_ಗಂಟು(&pcpu->ecaps_ಗಮ್ಯ_sync_ಗಂಟು);
#ಆದರೆ_ಅಂತ್ಯ
		INIT_LIST_ತಲೆ(&pcpu->ಗಮ್ಯ_kick_ಗಂಟು);
		ಆದರೆ (!zalloc_cpumask_var_ಗಂಟು(&pcpu->cpus_ಗಮ್ಯ_kick, GFP_KERNEL, ಗಂಟು) ||
		    !zalloc_cpumask_var_ಗಂಟು(&pcpu->cpus_ಗಮ್ಯ_kick_if_idle, GFP_KERNEL, ಗಂಟು) ||
		    !zalloc_cpumask_var_ಗಂಟು(&pcpu->cpus_ಗಮ್ಯ_preempt, GFP_KERNEL, ಗಂಟು) ||
		    !zalloc_cpumask_var_ಗಂಟು(&pcpu->cpus_ಗಮ್ಯ_wait, GFP_KERNEL, ಗಂಟು)) {
			ret = -ENOMEM;
			ಹೋಗು err_free_pcpu;
		}
	}

	sch->helper = kthread_ಸತತ_ಕೆಲಸಗಾರ(0, "sched_ext_helper");
	ಆದರೆ (IS_ERR(sch->helper)) {
		ret = PTR_ERR(sch->helper);
		ಹೋಗು err_free_pcpu;
	}

	sched_ಗಣ_fifo(sch->helper->task);

	ಆದರೆ (ಪೋಷಕ)
		ಸ್ಮರಣೆ_ನಕಲು(sch->ancestors, ಪೋಷಕ->ancestors,
		       level * ಗಾತ್ರ(ಪೋಷಕ->ancestors[0]));
	sch->ancestors[level] = sch;
	sch->level = level;
	sch->id = atomic64_inc_return(&scx_sched_id_cursor);

	ಆದರೆ (ops->timeout_ms)
		sch->watchdog_timeout = msecs_ಗಮ್ಯ_jiffies(ops->timeout_ms);
	ಇಲ್ಲದಿದ್ದರೆ
		sch->watchdog_timeout = SCX_WATCHDOG_MAX_TIMEOUT;

	sch->ತುಂಡು_dfl = SCX_ತುಂಡು_DFL;
	atomic_ಗಣ(&sch->exit_ಬಗೆ, SCX_EXIT_NONE);
	sch->disable_irq_work = IRQ_WORK_INIT_HARD(scx_disable_irq_workfn);
	sch->propagate_exit_irq_work = IRQ_WORK_INIT_HARD(scx_propagate_exit_irq_workfn);
	kthread_init_work(&sch->disable_work, scx_disable_workfn);
	timer_ಸಿದ್ಧತೆ(&sch->bypass_lb_timer, scx_bypass_lb_timerfn, 0);

	ಆದರೆ (!alloc_cpumask_var(&sch->bypass_lb_donee_cpumask, GFP_KERNEL)) {
		ret = -ENOMEM;
		ಹೋಗು err_stop_helper;
	}
	ಆದರೆ (!alloc_cpumask_var(&sch->bypass_lb_resched_cpumask, GFP_KERNEL)) {
		ret = -ENOMEM;
		ಹೋಗು err_free_lb_cpumask;
	}
	ಆದರೆ (!zalloc_cpumask_var(&sch->stall_cpus, GFP_KERNEL)) {
		ret = -ENOMEM;
		ಹೋಗು err_free_lb_resched_cpumask;
	}
	/*
	 * Copy ops through the right union view. For cid-form the source is
	 * struct sched_ext_ops_cid which lacks the trailing cpu_acquire/
	 * cpu_release; those stay zero from kzalloc.
	 */
	ಆದರೆ (cmd->is_cid_type) {
		sch->ops_cid = *cmd->ops_cid;
		sch->is_cid_type = true;
	} ಇಲ್ಲದಿದ್ದರೆ {
		sch->ops = *cmd->ops;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಅಕ್ಷರ *buf = kzalloc(ಮಾರ್ಗ_MAX, GFP_KERNEL);
	ಆದರೆ (!buf) {
		ret = -ENOMEM;
		ಹೋಗು err_free_lb_resched;
	}
	cgroup_ಮಾರ್ಗ(cgrp, buf, ಮಾರ್ಗ_MAX);
	sch->cgrp_ಮಾರ್ಗ = kstrdup(buf, GFP_KERNEL);
	kfree(buf);
	ಆದರೆ (!sch->cgrp_ಮಾರ್ಗ) {
		ret = -ENOMEM;
		ಹೋಗು err_free_lb_resched;
	}

	sch->cgrp = cgrp;
	INIT_LIST_ತಲೆ(&sch->children);
	INIT_LIST_ತಲೆ(&sch->sibling);
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

	/*
	 * Publishing makes @sch visible to scx_prog_sched() readers. Failure
	 * paths after this point must free @sch through kobject_put() whose
	 * release path defers the actual freeing by an RCU grace period.
	 */
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ops->priv, sch);

	sch->kobj.kset = scx_kset;
	INIT_LIST_ತಲೆ(&sch->all);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಆದರೆ (ಪೋಷಕ) {
		/*
		 * Pin @parent for @sch's lifetime. The kobject hierarchy pins
		 * it only via @parent->sub_kset, which is dropped during
		 * disable. Released in scx_sched_free_rcu_work().
		 */
		kobject_get(&ಪೋಷಕ->kobj);
	}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

	/*
	 * Init the kobj but don't add to sysfs yet. The enable path calls
	 * scx_sched_sysfs_add() once @sch's sysfs-visible state is initialized.
	 */
	kobject_init(&sch->kobj, &scx_ktype);

	/*
	 * Consume the arena_map ref bpf_scx_reg_cid() took. Defer to here so
	 * earlier failure paths leave cmd->arena_map set and bpf_scx_reg_cid
	 * drops the ref. After this point, sch owns the ref and any cleanup
	 * runs through scx_sched_free_rcu_work() which puts it.
	 */
	sch->arena_map = cmd->arena_map;
	/* BPF arena is only available on MMU && 64BIT */
#ಆದರೆ defined(CONFIG_MMU) && defined(CONFIG_64_ಬಿಟ್)
	ಆದರೆ (sch->arena_map)
		sch->arena_kern_ತಳ = bpf_arena_map_kern_vm_ಆರಂಭ(sch->arena_map);
#ಆದರೆ_ಅಂತ್ಯ
	cmd->arena_map = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ sch;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
err_free_lb_resched:
	free_cpumask_var(sch->stall_cpus);
#ಆದರೆ_ಅಂತ್ಯ
err_free_lb_resched_cpumask:
	free_cpumask_var(sch->bypass_lb_resched_cpumask);
err_free_lb_cpumask:
	free_cpumask_var(sch->bypass_lb_donee_cpumask);
err_stop_helper:
	kthread_destroy_ಕೆಲಸಗಾರ(sch->helper);
err_free_pcpu:
	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_sched_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu);

		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_kick);
		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_kick_if_idle);
		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_preempt);
		free_cpumask_var(pcpu->cpus_ಗಮ್ಯ_wait);
	}
	for_each_possible_cpu(cpu) {
		ಆದರೆ (cpu == bypass_fail_cpu)
			ನಿಲ್ಲಿಸು;
		exit_dsq(scx_bypass_dsq(sch, cpu));
	}
	free_percpu(sch->pcpu);
err_free_pnode:
	for_each_ಗಂಟು_state(ಗಂಟು, N_POSSIBLE)
		free_pnode(sch->pnode[ಗಂಟು]);
	kfree(sch->pnode);
err_free_hash:
	rhashtable_free_and_destroy(&sch->dsq_hash, ಖಾಲಿ, ಖಾಲಿ);
err_free_ei:
	free_exit_info(sch->exit_info);
err_free_sch:
	kfree(sch);
err_put_cgrp:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	cgroup_put(cgrp);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ ERR_PTR(ret);
}

/*
 * Add @sch's kobject to sysfs, and create its sub_kset if the scheduler
 * implements ops.sub_attach. Called by the enable workfns once @sch's
 * sysfs-visible state is initialized.
 */
ಪೂರ್ಣಾಂಕ scx_sched_sysfs_ಕೂಡಿಸು(ರಚನೆ scx_sched *sch)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ರಚನೆ scx_sched *ಪೋಷಕ = scx_ಪೋಷಕ(sch);
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (ಪೋಷಕ)
		ret = kobject_ಕೂಡಿಸು(&sch->kobj, &ಪೋಷಕ->sub_kset->kobj,
				  "sub-%llu", cgroup_id(sch_cgroup(sch)));
	ಇಲ್ಲದಿದ್ದರೆ
		ret = kobject_ಕೂಡಿಸು(&sch->kobj, ಖಾಲಿ, "root");
	ಆದರೆ (ret < 0)
		ಹಿಂತಿರುಗಿ ret;

	ಆದರೆ (sch->ops.sub_attach) {
		sch->sub_kset = kset_create_and_ಕೂಡಿಸು("sub", ಖಾಲಿ, &sch->kobj);
		ಆದರೆ (!sch->sub_kset)
			ಹಿಂತಿರುಗಿ -ENOMEM;
	}
	ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ kobject_ಕೂಡಿಸು(&sch->kobj, ಖಾಲಿ, "root");
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_hotplug_seq(ರಚನೆ scx_sched *sch,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ sched_ext_ops *ops)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ global_hotplug_seq;

	/*
	 * If a hotplug event has occurred between when a scheduler was
	 * initialized, and when we were able to attach, exit and notify user
	 * space about it.
	 */
	ಆದರೆ (ops->hotplug_seq) {
		global_hotplug_seq = atomic_long_read(&scx_hotplug_seq);
		ಆದರೆ (ops->hotplug_seq != global_hotplug_seq) {
			scx_exit(sch, SCX_EXIT_UNREG_KERN,
				 SCX_ECODE_ACT_RESTART | SCX_ECODE_RSN_HOTPLUG,
				 "expected hotplug seq %llu did not match actual %llu",
				 ops->hotplug_seq, global_hotplug_seq);
			ಹಿಂತಿರುಗಿ -EBUSY;
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ scx_validate_ops(ರಚನೆ scx_sched *sch, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_ext_ops *ops)
{
	/*
	 * It doesn't make sense to specify the SCX_OPS_ENQ_LAST flag if the
	 * ops.enqueue() callback isn't implemented.
	 */
	ಆದರೆ ((ops->flags & SCX_OPS_ENQ_ಕೊನೆಯ) && !ops->ಸರತಿಗೆ_ಸೇರಿಸು) {
		scx_error(sch, "SCX_OPS_ENQ_LAST requires ops.enqueue() to be implemented");
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	/*
	 * SCX_OPS_TID_TO_TASK is enabled by the root scheduler. A sub-sched
	 * may set it to declare a dependency; reject if the root hasn't
	 * enabled it.
	 */
	ಆದರೆ ((ops->flags & SCX_OPS_TID_ಗಮ್ಯ_TASK) && scx_ಪೋಷಕ(sch) &&
	    !(sch->ancestors[0]->ops.flags & SCX_OPS_TID_ಗಮ್ಯ_TASK)) {
		scx_error(sch, "SCX_OPS_TID_TO_TASK requires root scheduler to enable it");
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	/*
	 * SCX_OPS_BUILTIN_IDLE_PER_NODE requires built-in CPU idle
	 * selection policy to be enabled.
	 */
	ಆದರೆ ((ops->flags & SCX_OPS_BUILTIN_IDLE_ಪ್ರತಿ_ಗಂಟು) &&
	    (ops->update_idle && !(ops->flags & SCX_OPS_KEEP_BUILTIN_IDLE))) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE requires CPU idle selection enabled");
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	/*
	 * cid-form's struct is shorter and doesn't include the cpu_acquire /
	 * cpu_release tail; reading those fields off a cid-form @ops would
	 * run past the BPF allocation. Skip for cid-form.
	 */
	ಆದರೆ (!sch->is_cid_type && (ops->cpu_acquire || ops->cpu_release))
		pr_warn_ratelimited("ops->cpu_acquire/release() are deprecated, use sched_switch TP instead\n");

	/*
	 * Sub-scheduler support is tied to the cid-form struct_ops. A sub-sched
	 * attaches through a cid-form-only interface (sub_attach/sub_detach),
	 * and a root that accepts sub-scheds must expose cid-form state to
	 * them. Reject cpu-form schedulers on either side.
	 */
	ಆದರೆ (!sch->is_cid_type) {
		ಆದರೆ (scx_ಪೋಷಕ(sch)) {
			scx_error(sch, "sub-sched requires cid-form struct_ops");
			ಹಿಂತಿರುಗಿ -EINVAL;
		}
		ಆದರೆ (ops->sub_attach || ops->sub_detach) {
			scx_error(sch, "sub_attach/sub_detach requires cid-form struct_ops");
			ಹಿಂತಿರುಗಿ -EINVAL;
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_ಬೇರು_enable_workfn(ರಚನೆ kthread_work *work)
{
	ರಚನೆ scx_enable_cmd *cmd = container_ದ(work, ರಚನೆ scx_enable_cmd, work);
	ರಚನೆ sched_ext_ops *ops = cmd->ops;
	ರಚನೆ cgroup *cgrp = ಬೇರು_cgroup();
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_task_iter sti;
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ i, cpu, ret;

	mutex_ಬೀಗ(&scx_enable_mutex);

	ಆದರೆ (scx_enable_state() != SCX_DISABLED) {
		ret = -EBUSY;
		ಹೋಗು err_unlock;
	}

	/*
	 * @ops->priv binds @ops to its scx_sched instance. It is set here by
	 * scx_alloc_and_add_sched() and cleared at the tail of bpf_scx_unreg(),
	 * which runs after scx_root_disable() has dropped scx_enable_mutex. If
	 * it's still non-NULL here, a previous attachment on @ops has not
	 * finished tearing down; proceeding would let the in-flight unreg's
	 * RCU_INIT_POINTER(NULL) clobber the @ops->priv we are about to assign.
	 */
	ಆದರೆ (rcu_access_ಸೂಚಕ(ops->priv)) {
		ret = -EBUSY;
		ಹೋಗು err_unlock;
	}

	ret = alloc_kick_syncs();
	ಆದರೆ (ret)
		ಹೋಗು err_unlock;

	ಆದರೆ (ops->flags & SCX_OPS_TID_ಗಮ್ಯ_TASK) {
		ret = rhashtable_init(&scx_tid_hash, &scx_tid_hash_params);
		ಆದರೆ (ret)
			ಹೋಗು err_free_ksyncs;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	cgroup_get(cgrp);
#ಆದರೆ_ಅಂತ್ಯ
	/*
	 * Transition to ENABLING to arm the disable path. Allocation failure
	 * still unwinds locally. Full disabling on failure applies only after
	 * scx_alloc_and_add_sched() succeeds.
	 */
	WARN_ON_ONCE(scx_ಗಣ_enable_state(SCX_ENABLING) != SCX_DISABLED);
	WARN_ON_ONCE(scx_ಬೇರು);

	sch = scx_alloc_and_ಕೂಡಿಸು_sched(cmd, cgrp, ಖಾಲಿ);
	ಆದರೆ (IS_ERR(sch)) {
		ret = PTR_ERR(sch);
		WARN_ON_ONCE(scx_ಗಣ_enable_state(SCX_DISABLED) != SCX_ENABLING);
		ಹೋಗು err_free_tid_hash;
	}

	ಆದರೆ (sch->is_cid_type)
		static_branch_enable(&__scx_is_cid_type);

	atomic_long_ಗಣ(&scx_nr_rejected, 0);

	for_each_possible_cpu(cpu) {
		ರಚನೆ rq *rq = cpu_rq(cpu);

		rq->scx.local_dsq.sched = sch;
		rq->scx.cpuperf_ಗುರಿ = SCX_CPUPERF_ONE;
	}

	scx_discard_stale_ecaps_syncs();
	scx_rescue_ಗಣ_knobs(sch);

	/*
	 * Keep CPUs stable during enable so that the BPF scheduler can track
	 * online CPUs by watching ->on/offline_cpu() after ->init().
	 */
	cpus_read_ಬೀಗ();

	/*
	 * Build the cid mapping into a private under-construction set. It
	 * becomes visible to readers only through scx_cid_publish_tables() once
	 * ops.init_cids() has finalized the layout.
	 */
	ret = scx_cid_init(sch);
	ಆದರೆ (ret) {
		cpus_read_unlock();
		ಹೋಗು err_disable;
	}

	/*
	 * Make the scheduler instance visible. Must be inside cpus_read_lock().
	 * See handle_hotplug().
	 */
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(scx_ಬೇರು, sch);

	ret = scx_link_sched(sch);
	ಆದರೆ (ret) {
		cpus_read_unlock();
		ಹೋಗು err_disable;
	}

	scx_idle_enable(ops);

	/*
	 * A cid-form scheduler finalizes its cid layout in ops.init_cids(),
	 * which may call scx_bpf_cid_override(). Run it before the caps and
	 * shard state are built so the final layout is in effect.
	 */
	ಆದರೆ (sch->is_cid_type && sch->ops_cid.init_cids) {
		ret = SCX_CALL_OP_RET(sch, init_cids, ಖಾಲಿ);
		ಆದರೆ (ret) {
			ret = scx_ops_sanitize_err(sch, "init_cids", ret);
			cpus_read_unlock();
			scx_error(sch, "ops.init_cids() failed (%d)", ret);
			ಹೋಗು err_disable;
		}
	}

	/* the cid layout is final, expose it to readers */
	scx_cid_publish_tables();

	ret = scx_arena_pool_init(sch);
	ಆದರೆ (ret) {
		cpus_read_unlock();
		ಹೋಗು err_disable;
	}

	ret = scx_alloc_kern_arena_objs(sch);
	ಆದರೆ (ret) {
		cpus_read_unlock();
		ಹೋಗು err_disable;
	}

	ret = scx_alloc_pshards(sch);
	ಆದರೆ (ret) {
		cpus_read_unlock();
		ಹೋಗು err_disable;
	}

	scx_init_ಬೇರು_caps(sch);

	/* the cid caps and shards are live now, so ops.init() can query them */
	ಆದರೆ (sch->ops.init) {
		ret = SCX_CALL_OP_RET(sch, init, ಖಾಲಿ);
		ಆದರೆ (ret) {
			ret = scx_ops_sanitize_err(sch, "init", ret);
			cpus_read_unlock();
			scx_error(sch, "ops.init() failed (%d)", ret);
			ಹೋಗು err_disable;
		}
		sch->exit_info->flags |= SCX_EFLAG_INITIALIZED;
	}

	ret = scx_sched_sysfs_ಕೂಡಿಸು(sch);
	ಆದರೆ (ret) {
		cpus_read_unlock();
		ಹೋಗು err_disable;
	}

	ಗಾಗಿ (i = SCX_OPI_CPU_HOTPLUG_BEGIN; i < SCX_OPI_CPU_HOTPLUG_ಕೊನೆ; i++)
		ಆದರೆ (((ಶೂನ್ಯ (**)(ಶೂನ್ಯ))ops)[i])
			ಗಣ_ಬಿಟ್(i, sch->has_op);

	ret = check_hotplug_seq(sch, ops);
	ಆದರೆ (ret) {
		cpus_read_unlock();
		ಹೋಗು err_disable;
	}
	scx_idle_update_selcpu_topology(ops);

	cpus_read_unlock();

	ret = scx_validate_ops(sch, ops);
	ಆದರೆ (ret)
		ಹೋಗು err_disable;

	/*
	 * Attach the ext_server bandwidth reservation before anything is
	 * committed so that we can fail the enable if the root domain cannot
	 * accommodate it. The matching fair_server detach is deferred to the
	 * tail of this function, after the switch is fully committed and can no
	 * longer fail.
	 *
	 * On failure, err_disable funnels into scx_root_disable() which
	 * detaches ext_server, so partially-attached state is cleaned up
	 * automatically.
	 */
	for_each_possible_cpu(cpu) {
		ರಚನೆ rq *rq = cpu_rq(cpu);

		scoped_guard(rq_ಬೀಗ_irqsave, rq) {
			update_rq_clock(rq);
			ret = dl_server_attach_bw(&rq->ext_server);
		}
		ಆದರೆ (ret) {
			pr_warn("sched_ext: failed to attach ext_server on CPU %d (%d)\n",
				cpu, ret);
			ಹೋಗು err_disable;
		}
	}

	/*
	 * Once __scx_enabled is set, %current can be switched to SCX anytime.
	 * This can lead to stalls as some BPF schedulers (e.g. userspace
	 * scheduling) may not function correctly before all tasks are switched.
	 * Init in bypass mode to guarantee forward progress.
	 */
	scx_bypass(sch, true);

	ಗಾಗಿ (i = SCX_OPI_ಸಾಮಾನ್ಯ_BEGIN; i < SCX_OPI_ಸಾಮಾನ್ಯ_ಕೊನೆ; i++)
		ಆದರೆ (((ಶೂನ್ಯ (**)(ಶೂನ್ಯ))ops)[i])
			ಗಣ_ಬಿಟ್(i, sch->has_op);

	ಆದರೆ (sch->ops.cpu_acquire || sch->ops.cpu_release)
		sch->ops.flags |= SCX_OPS_HAS_CPU_PREEMPT;

	/*
	 * Lock out forks, cgroup on/offlining and moves before opening the
	 * floodgate so that they don't wander into the operations prematurely.
	 */
	percpu_ಕೆಳಕ್ಕೆ_write(&scx_fork_rwsem);

	WARN_ON_ONCE(scx_init_task_enabled);
	scx_init_task_enabled = true;

	/* flip under fork_rwsem; the iter below covers existing tasks */
	ಆದರೆ (ops->flags & SCX_OPS_TID_ಗಮ್ಯ_TASK)
		static_branch_enable(&__scx_tid_to_task_enabled);

	/*
	 * Enable ops for every task. Fork is excluded by scx_fork_rwsem
	 * preventing new tasks from being added. No need to exclude tasks
	 * leaving as sched_ext_dead() can handle both prepped and enabled
	 * tasks. Prep all tasks first and then enable them with preemption
	 * disabled.
	 *
	 * All cgroups should be initialized before scx_init_task() so that the
	 * BPF scheduler can reliably track each task's cgroup membership from
	 * scx_init_task(). Lock out cgroup on/offlining and task migrations
	 * while tasks are being initialized so that scx_cgroup_can_attach()
	 * never sees uninitialized tasks.
	 */
	scx_cgroup_ಬೀಗ();
	ಗಣ_cgroup_sched(sch_cgroup(sch), sch);
	ret = scx_cgroup_init(sch);
	ಆದರೆ (ret)
		ಹೋಗು err_disable_unlock_all;

	WARN_ON_ONCE(scx_cgroup_enabled);
	scx_cgroup_enabled = true;

	scx_task_iter_ಆರಂಭ(&sti, ಖಾಲಿ);
	ಆಗ ((p = scx_task_iter_ಮುಂದಿನ_locked(&sti))) {
		/*
		 * @p is in scx_tasks under scx_tasks_lock, and SCX_TASK_DEAD
		 * tasks are filtered by scx_task_iter_next_locked().
		 * sched_ext_dead() removes @p from scx_tasks under the same
		 * lock before put_task_struct_rcu_user() runs, so @p->usage
		 * is guaranteed > 0 here.
		 */
		get_task_struct(p);

		/*
		 * Set %INIT_BEGIN under the iter's rq lock so that a concurrent
		 * sched_ext_dead() does not call ops.exit_task() on @p while
		 * ops.init_task() is running. If sched_ext_dead() runs before
		 * this store, it has already removed @p from scx_tasks and the
		 * iter won't visit @p; if it runs after, it observes
		 * %INIT_BEGIN and transitions to %DEAD without calling ops,
		 * leaving the post-init recheck below to unwind.
		 */
		scx_ಗಣ_task_state(p, SCX_TASK_INIT_BEGIN);
		scx_task_iter_unlock(&sti);

		ret = __scx_init_task(sch, p, ಖಾಲಿ, false);

		scx_task_iter_relock(&sti, p);

		ಆದರೆ (unlikely(ret)) {
			ಆದರೆ (scx_get_task_state(p) != SCX_TASK_DEAD)
				scx_ಗಣ_task_state(p, SCX_TASK_NONE);
			scx_task_iter_stop(&sti);
			scx_error(sch, "ops.init_task() failed (%d) for %s[%d]",
				  ret, p->comm, p->pid);
			put_task_struct(p);
			ಹೋಗು err_disable_unlock_all;
		}

		ಆದರೆ (scx_get_task_state(p) == SCX_TASK_DEAD) {
			/*
			 * sched_ext_dead() observed %INIT_BEGIN and set %DEAD.
			 * ops.exit_task() is owed to the sched __scx_init_task()
			 * ran against; call it now.
			 */
			scx_sub_init_cancel_task(sch, p);
		} ಇಲ್ಲದಿದ್ದರೆ {
			scx_ಗಣ_task_state(p, SCX_TASK_INIT);
			scx_ಗಣ_task_sched(p, sch);
			scx_ಗಣ_task_state(p, SCX_TASK_READY);
		}

		/*
		 * Insert into the tid hash. scx_tasks_lock is held by the iter;
		 * list_empty() guards against sched_ext_dead() having taken @p
		 * off the list while init ran unlocked.
		 */
		ಆದರೆ (scx_tid_ಗಮ್ಯ_task_enabled() && !list_ಖಾಲಿಗಳು(&p->scx.tasks_ಗಂಟು))
			scx_tid_hash_ಸೇರಿಸು(p);

		put_task_struct(p);
	}
	scx_task_iter_stop(&sti);
	scx_cgroup_unlock();
	percpu_ಮೇಲೆ_write(&scx_fork_rwsem);

	/*
	 * All tasks are READY. It's safe to turn on scx_enabled() and switch
	 * all eligible tasks.
	 */
	WRITE_ONCE(scx_switching_all, !(ops->flags & SCX_OPS_SWITCH_PARTIAL));
	static_branch_enable(&__scx_enabled);

	/*
	 * We're fully committed and can't fail. The task READY -> ENABLED
	 * transitions here are synchronized against sched_ext_dead() through
	 * scx_tasks_lock.
	 */
	percpu_ಕೆಳಕ್ಕೆ_write(&scx_fork_rwsem);
	scx_task_iter_ಆರಂಭ(&sti, ಖಾಲಿ);
	ಆಗ ((p = scx_task_iter_ಮುಂದಿನ_locked(&sti))) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸರತಿ_flags = ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE;
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *old_class = p->sched_class;
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *new_class = scx_setscheduler_class(p);

		ಆದರೆ (scx_get_task_state(p) != SCX_TASK_READY)
			ಮುಂದುವರಿ;

		ಆದರೆ (old_class != new_class)
			ಸರತಿ_flags |= ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS;

		scoped_guard (sched_change, p, ಸರತಿ_flags) {
			scx_ಗಣ_task_ತುಂಡು(p, READ_ONCE(sch->ತುಂಡು_dfl));
			p->sched_class = new_class;
		}
	}
	scx_task_iter_stop(&sti);
	percpu_ಮೇಲೆ_write(&scx_fork_rwsem);

	scx_bypass(sch, false);

	ಆದರೆ (!scx_tryset_enable_state(SCX_ENABLED, SCX_ENABLING)) {
		WARN_ON_ONCE(atomic_read(&sch->exit_ಬಗೆ) == SCX_EXIT_NONE);
		ret = -EBUSY;
		ಹೋಗು err_disable;
	}

	ಆದರೆ (!(ops->flags & SCX_OPS_SWITCH_PARTIAL))
		static_branch_enable(&__scx_switched_all);

	/*
	 * Detach the fair_server bandwidth reservation now that the switch
	 * is fully committed. In full mode (!SCX_OPS_SWITCH_PARTIAL) no
	 * task will ever run in the fair class, so give that bandwidth
	 * back to the RT class. The matching ext_server attach already
	 * happened earlier; this only releases bandwidth and cannot fail.
	 *
	 * In partial mode keep fair_server attached.
	 */
	ಆದರೆ (scx_switched_all()) {
		for_each_possible_cpu(cpu) {
			ರಚನೆ rq *rq = cpu_rq(cpu);

			guard(rq_ಬೀಗ_irqsave)(rq);
			update_rq_clock(rq);
			dl_server_detach_bw(&rq->fair_server);
		}
	}

	pr_info("sched_ext: BPF scheduler \"%s\" enabled%s\n",
		sch->ops.name, scx_switched_all() ? "" : " (partial)");
	kobject_uevent(&sch->kobj, KOBJ_ಕೂಡಿಸು);
	mutex_unlock(&scx_enable_mutex);

	atomic_long_inc(&scx_enable_seq);

	cmd->ret = 0;
	ಹಿಂತಿರುಗಿ;

err_free_tid_hash:
	ಆದರೆ (ops->flags & SCX_OPS_TID_ಗಮ್ಯ_TASK)
		rhashtable_free_and_destroy(&scx_tid_hash, ಖಾಲಿ, ಖಾಲಿ);
err_free_ksyncs:
	free_kick_syncs();
err_unlock:
	mutex_unlock(&scx_enable_mutex);
	cmd->ret = ret;
	ಹಿಂತಿರುಗಿ;

err_disable_unlock_all:
	scx_cgroup_unlock();
	percpu_ಮೇಲೆ_write(&scx_fork_rwsem);
	/* we'll soon enter disable path, keep bypass on */
err_disable:
	mutex_unlock(&scx_enable_mutex);
	/*
	 * Returning an error code here would not pass all the error information
	 * to userspace. Record errno using scx_error() for cases scx_error()
	 * wasn't already invoked and exit indicating success so that the error
	 * is notified through ops.exit() with all the details.
	 *
	 * Flush scx_disable_work to ensure that error is reported before init
	 * completion. sch's base reference will be put by bpf_scx_unreg().
	 */
	scx_error(sch, "scx_root_enable() failed (%d)", ret);
	scx_flush_disable_work(sch);
	cmd->ret = 0;
}

ಸ್ಥಿರ s32 scx_enable(ರಚನೆ scx_enable_cmd *cmd, ರಚನೆ bpf_link *link)
{
	ಸ್ಥಿರ ರಚನೆ kthread_ಕೆಲಸಗಾರ *helper;
	ಸ್ಥಿರ DEFINE_MUTEX(helper_mutex);

	ಆದರೆ (housekeeping_enabled(HK_TYPE_DOMAIN_BOOT)) {
		pr_err("sched_ext: Not compatible with \"isolcpus=\" domain isolation\n");
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (!READ_ONCE(helper)) {
		mutex_ಬೀಗ(&helper_mutex);
		ಆದರೆ (!helper) {
			ರಚನೆ kthread_ಕೆಲಸಗಾರ *w =
				kthread_ಸತತ_ಕೆಲಸಗಾರ(0, "scx_enable_helper");
			ಆದರೆ (IS_ERR_OR_NULL(w)) {
				mutex_unlock(&helper_mutex);
				ಹಿಂತಿರುಗಿ -ENOMEM;
			}
			sched_ಗಣ_fifo(w->task);
			WRITE_ONCE(helper, w);
		}
		mutex_unlock(&helper_mutex);
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಆದರೆ (cmd->ops->sub_cgroup_id > 1)
		kthread_init_work(&cmd->work, scx_sub_enable_workfn);
	ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */
		kthread_init_work(&cmd->work, scx_ಬೇರು_enable_workfn);

	kthread_ಸರತಿ_work(READ_ONCE(helper), &cmd->work);
	kthread_flush_work(&cmd->work);
	ಹಿಂತಿರುಗಿ cmd->ret;
}


/********************************************************************************
 * bpf_struct_ops plumbing.
 */
#ಅನ್ತರ್ಭಾವ <linux/bpf_verifier.h>
#ಅನ್ತರ್ಭಾವ <linux/bpf.h>
#ಅನ್ತರ್ಭಾವ <linux/btf.h>

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_type *task_struct_type;

ಸ್ಥಿರ bool bpf_scx_is_valid_access(ಪೂರ್ಣಾಂಕ off, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2,
				    ಎಣಿಕೆ bpf_access_type type,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog *prog,
				    ರಚನೆ bpf_insn_access_aux *info)
{
	ಆದರೆ (type != BPF_READ)
		ಹಿಂತಿರುಗಿ false;
	ಆದರೆ (off < 0 || off >= ಗಾತ್ರ(__u64) * MAX_BPF_FUNC_ವಾದಗಳು)
		ಹಿಂತಿರುಗಿ false;
	ಆದರೆ (off % ಗಾತ್ರ_2 != 0)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ btf_ctx_access(off, ಗಾತ್ರ_2, type, prog, info);
}

/* common to both forms: only scx.disallow is writable */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_btf_struct_access_common(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_reg_state *reg,
					    ಪೂರ್ಣಾಂಕ off, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ btf_type *t;

	t = btf_type_by_id(reg->btf, reg->btf_id);
	ಆದರೆ (t == task_struct_type &&
	    off >= offsetof(ರಚನೆ task_struct, scx.disallow) &&
	    off + ಗಾತ್ರ_2 <= offsetofend(ರಚನೆ task_struct, scx.disallow))
		ಹಿಂತಿರುಗಿ SCALAR_ಮೌಲ್ಯ;

	ಹಿಂತಿರುಗಿ -EACCES;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_btf_struct_access(ರಚನೆ bpf_verifier_log *ಲಘುಗಣಕ,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_reg_state *reg, ಪೂರ್ಣಾಂಕ off,
				     ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ btf_type *t;

	t = btf_type_by_id(reg->btf, reg->btf_id);
	ಆದರೆ (t == task_struct_type) {
		ಆದರೆ ((off >= offsetof(ರಚನೆ task_struct, scx.ತುಂಡು) &&
		     off + ಗಾತ್ರ_2 <= offsetofend(ರಚನೆ task_struct, scx.ತುಂಡು)) ||
		    (off >= offsetof(ರಚನೆ task_struct, scx.dsq_vtime) &&
		     off + ಗಾತ್ರ_2 <= offsetofend(ರಚನೆ task_struct, scx.dsq_vtime)))
			ಹಿಂತಿರುಗಿ SCALAR_ಮೌಲ್ಯ;
	}

	ಹಿಂತಿರುಗಿ bpf_scx_btf_struct_access_common(reg, off, ಗಾತ್ರ_2);
}

/* cid-form rejects direct slice and dsq_vtime writes in favor of the kfuncs */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_cid_btf_struct_access(ರಚನೆ bpf_verifier_log *ಲಘುಗಣಕ,
					 ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_reg_state *reg, ಪೂರ್ಣಾಂಕ off,
					 ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
	ಹಿಂತಿರುಗಿ bpf_scx_btf_struct_access_common(reg, off, ಗಾತ್ರ_2);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_verifier_ops bpf_scx_verifier_ops = {
	.get_func_proto = bpf_ತಳ_func_proto,
	.is_valid_access = bpf_scx_is_valid_access,
	.btf_struct_access = bpf_scx_btf_struct_access,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_verifier_ops bpf_scx_cid_verifier_ops = {
	.get_func_proto = bpf_ತಳ_func_proto,
	.is_valid_access = bpf_scx_is_valid_access,
	.btf_struct_access = bpf_scx_cid_btf_struct_access,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_init_member(ಸ್ಥಿರಾಂಕ ರಚನೆ btf_type *t,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ btf_member *member,
			       ಶೂನ್ಯ *kdata, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *udata)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_ext_ops *uops = udata;
	ರಚನೆ sched_ext_ops *ops = kdata;
	u32 moff = __btf_member_bit_offset(t, member) / 8;
	ಪೂರ್ಣಾಂಕ ret;

	ಆಯ್ಕೆ (moff) {
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, dispatch_max_batch):
		ಆದರೆ (*(u32 *)(udata + moff) > ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
			ಹಿಂತಿರುಗಿ -E2BIG;
		ops->dispatch_max_batch = *(u32 *)(udata + moff);
		ಹಿಂತಿರುಗಿ 1;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, flags):
		ಆದರೆ (*(u64 *)(udata + moff) & ~SCX_OPS_ALL_FLAGS)
			ಹಿಂತಿರುಗಿ -EINVAL;
		ops->flags = *(u64 *)(udata + moff);
		ಹಿಂತಿರುಗಿ 1;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, name):
		ret = bpf_obj_name_cpy(ops->name, uops->name,
				       ಗಾತ್ರ(ops->name));
		ಆದರೆ (ret < 0)
			ಹಿಂತಿರುಗಿ ret;
		ಆದರೆ (ret == 0)
			ಹಿಂತಿರುಗಿ -EINVAL;
		ಹಿಂತಿರುಗಿ 1;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, timeout_ms):
		ಆದರೆ (msecs_ಗಮ್ಯ_jiffies(*(u32 *)(udata + moff)) >
		    SCX_WATCHDOG_MAX_TIMEOUT)
			ಹಿಂತಿರುಗಿ -E2BIG;
		ops->timeout_ms = *(u32 *)(udata + moff);
		ಹಿಂತಿರುಗಿ 1;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, exit_dump_len):
		ops->exit_dump_len =
			*(u32 *)(udata + moff) ?: SCX_EXIT_DUMP_DFL_LEN;
		ಹಿಂತಿರುಗಿ 1;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, hotplug_seq):
		ops->hotplug_seq = *(u64 *)(udata + moff);
		ಹಿಂತಿರುಗಿ 1;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, cid_shard_ಗಾತ್ರ):
		ops->cid_shard_ಗಾತ್ರ = *(u32 *)(udata + moff);
		ಹಿಂತಿರುಗಿ 1;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, rescue_bandwidth_ppt): {
		u32 bw_ppt = *(u32 *)(udata + moff);

		ಆದರೆ (bw_ppt > SCX_RESCUE_MAX_BW_PPT && bw_ppt != SCX_RESCUE_DISABLE)
			ಹಿಂತಿರುಗಿ -E2BIG;
		ops->rescue_bandwidth_ppt = bw_ppt;
		ಹಿಂತಿರುಗಿ 1;
	}
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, rescue_ಕಾಲಾಂಶ_us): {
		u32 ಕಾಲಾಂಶ_us = *(u32 *)(udata + moff);

		ಆದರೆ (ಕಾಲಾಂಶ_us > SCX_RESCUE_MAX_ಕಾಲಾಂಶ_US)
			ಹಿಂತಿರುಗಿ -E2BIG;
		ಆದರೆ (ಕಾಲಾಂಶ_us && ಕಾಲಾಂಶ_us < SCX_RESCUE_MIN_ಕಾಲಾಂಶ_US)
			ಹಿಂತಿರುಗಿ -EINVAL;
		ops->rescue_ಕಾಲಾಂಶ_us = ಕಾಲಾಂಶ_us;
		ಹಿಂತಿರುಗಿ 1;
	}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, sub_cgroup_id):
		ops->sub_cgroup_id = *(u64 *)(udata + moff);
		ಹಿಂತಿರುಗಿ 1;
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_check_member(ಸ್ಥಿರಾಂಕ ರಚನೆ btf_type *t,
				ಸ್ಥಿರಾಂಕ ರಚನೆ btf_member *member,
				ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog *prog)
{
	u32 moff = __btf_member_bit_offset(t, member) / 8;

	ಆಯ್ಕೆ (moff) {
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, init_task):
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, cgroup_init):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, cgroup_exit):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, cgroup_prep_move):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, cgroup_ಗಣ_bandwidth):
#ಆದರೆ_ಅಂತ್ಯ
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, cpu_online):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, cpu_offline):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, init_cids):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, init):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, ನಿರ್ಗಮಿಸು):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, sub_attach):
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, sub_detach):
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಆದರೆ (prog->sleepable)
			ಹಿಂತಿರುಗಿ -EINVAL;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	/*
	 * Enable private stack for operations that can nest along the
	 * hierarchy.
	 *
	 * XXX - Ideally, we should only do this for scheds that allow
	 * sub-scheds and sub-scheds themselves but I don't know how to access
	 * struct_ops from here.
	 */
	ಆಯ್ಕೆ (moff) {
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, dispatch):
		prog->aux->priv_ರಾಶಿ_requested = true;
		prog->aux->recursion_detected = scx_pstack_recursion_on_dispatch;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ offsetof(ರಚನೆ sched_ext_ops, sub_caps_updated):
		prog->aux->priv_ರಾಶಿ_requested = true;
		prog->aux->recursion_detected = scx_pstack_recursion_on_caps_updated;
		ನಿಲ್ಲಿಸು;
	}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_reg(ಶೂನ್ಯ *kdata, ರಚನೆ bpf_link *link)
{
	ರಚನೆ scx_enable_cmd cmd = { .ops = kdata };

	ಹಿಂತಿರುಗಿ scx_enable(&cmd, link);
}

ರಚನೆ scx_arena_ಪರಿಶೀಲನೆ {
	ರಚನೆ bpf_map	*arena;
	ಪೂರ್ಣಾಂಕ		err;
};

/*
 * The verifier enforces one arena per BPF program, so each struct_ops
 * member prog contributes at most one arena via bpf_prog_arena().
 * Require all non-NULL contributions to match.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scx_arena_ಪರಿಶೀಲನೆ_prog(ರಚನೆ bpf_prog *prog, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ scx_arena_ಪರಿಶೀಲನೆ *s = ದತ್ತಾಂಶ;
	ರಚನೆ bpf_map *arena = ಖಾಲಿ;

	/* arena.o, which defines these, is built only on MMU && 64BIT */
#ಆದರೆ defined(CONFIG_MMU) && defined(CONFIG_64_ಬಿಟ್)
	arena = bpf_prog_arena(prog);
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (!arena)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (s->arena && s->arena != arena) {
		s->err = -EINVAL;
		ಹಿಂತಿರುಗಿ 1;
	}
	s->arena = arena;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_reg_cid(ಶೂನ್ಯ *kdata, ರಚನೆ bpf_link *link)
{
	ರಚನೆ scx_enable_cmd cmd = { .ops_cid = kdata, .is_cid_type = true };
	ರಚನೆ scx_arena_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ = {};
	ಪೂರ್ಣಾಂಕ ret;

	bpf_struct_ops_for_each_prog(kdata, scx_arena_ಪರಿಶೀಲನೆ_prog, &ಪರಿಶೀಲನೆ);
	ಆದರೆ (ಪರಿಶೀಲನೆ.err) {
		pr_err("sched_ext: cid-form scheduler uses multiple arena maps\n");
		ಹಿಂತಿರುಗಿ ಪರಿಶೀಲನೆ.err;
	}
	ಆದರೆ (!ಪರಿಶೀಲನೆ.arena) {
		pr_err("sched_ext: cid-form scheduler must use a BPF arena map\n");
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	bpf_map_inc(ಪರಿಶೀಲನೆ.arena);
	cmd.arena_map = ಪರಿಶೀಲನೆ.arena;
	ret = scx_enable(&cmd, link);
	ಆದರೆ (cmd.arena_map)		/* not consumed by scx_alloc_and_add_sched() */
		bpf_map_put(cmd.arena_map);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ bpf_scx_unreg(ಶೂನ್ಯ *kdata, ರಚನೆ bpf_link *link)
{
	ರಚನೆ sched_ext_ops *ops = kdata;
	ರಚನೆ scx_sched *sch = rcu_dereference_protected(ops->priv, true);

	scx_disable(sch, SCX_EXIT_UNREG);
	scx_flush_disable_work(sch);
	RCU_INIT_ಸೂಚಕ(ops->priv, ಖಾಲಿ);
	kobject_put(&sch->kobj);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_init(ರಚನೆ btf *btf)
{
	task_struct_type = btf_type_by_id(btf, btf_tracing_ids[BTF_TRACING_TYPE_TASK]);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_update(ಶೂನ್ಯ *kdata, ಶೂನ್ಯ *old_kdata, ರಚನೆ bpf_link *link)
{
	/*
	 * sched_ext does not support updating the actively-loaded BPF
	 * scheduler, as registering a BPF scheduler can always fail if the
	 * scheduler returns an error code for e.g. ops.init(), ops.init_task(),
	 * etc. Similarly, we can always race with unregistration happening
	 * elsewhere, such as with sysrq.
	 */
	ಹಿಂತಿರುಗಿ -EOPNOTSUPP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ bpf_scx_validate(ಶೂನ್ಯ *kdata)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ s32 sched_ext_ops__select_cpu(ರಚನೆ task_struct *p, s32 prev_cpu, u64 wake_flags) { ಹಿಂತಿರುಗಿ -EINVAL; }
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ task_struct *p, u64 enq_flags) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ task_struct *p, u64 enq_flags) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__dispatch(s32 prev_cpu, ರಚನೆ task_struct *prev__nullable) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__tick(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__runnable(ರಚನೆ task_struct *p, u64 enq_flags) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__running(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__stopping(ರಚನೆ task_struct *p, bool runnable) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__quiescent(ರಚನೆ task_struct *p, u64 deq_flags) {}
ಸ್ಥಿರ bool sched_ext_ops__yield(ರಚನೆ task_struct *from, ರಚನೆ task_struct *ಗಮ್ಯ_nullable) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ bool sched_ext_ops__core_sched_before(ರಚನೆ task_struct *a, ರಚನೆ task_struct *b) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_ಗಣ_ತೂಕ(ರಚನೆ task_struct *p, u32 ತೂಕ) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_ಗಣ_cpumask(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಮುಖವಾಡ) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__update_idle(s32 cpu, bool idle) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__cpu_acquire(s32 cpu, ರಚನೆ scx_cpu_acquire_ವಾದಗಳು *ವಾದಗಳು) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__cpu_release(s32 cpu, ರಚನೆ scx_cpu_release_ವಾದಗಳು *ವಾದಗಳು) {}
ಸ್ಥಿರ s32 sched_ext_ops__init_task(ರಚನೆ task_struct *p, ರಚನೆ scx_init_task_ವಾದಗಳು *ವಾದಗಳು) { ಹಿಂತಿರುಗಿ -EINVAL; }
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__exit_task(ರಚನೆ task_struct *p, ರಚನೆ scx_exit_task_ವಾದಗಳು *ವಾದಗಳು) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__enable(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__disable(ರಚನೆ task_struct *p) {}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
ಸ್ಥಿರ s32 sched_ext_ops__cgroup_init(ರಚನೆ cgroup *cgrp, ರಚನೆ scx_cgroup_init_ವಾದಗಳು *ವಾದಗಳು) { ಹಿಂತಿರುಗಿ -EINVAL; }
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__cgroup_exit(ರಚನೆ cgroup *cgrp) {}
ಸ್ಥಿರ s32 sched_ext_ops__cgroup_prep_move(ರಚನೆ task_struct *p, ರಚನೆ cgroup *from, ರಚನೆ cgroup *ಗಮ್ಯ) { ಹಿಂತಿರುಗಿ -EINVAL; }
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__cgroup_move(ರಚನೆ task_struct *p, ರಚನೆ cgroup *from, ರಚನೆ cgroup *ಗಮ್ಯ) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__cgroup_cancel_move(ರಚನೆ task_struct *p, ರಚನೆ cgroup *from, ರಚನೆ cgroup *ಗಮ್ಯ) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_cgroup_ಗಣ_ತೂಕ(ರಚನೆ cgroup *cgrp, u32 ತೂಕ) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_cgroup_ಗಣ_bandwidth(ರಚನೆ cgroup *cgrp, u64 period_us, u64 quota_us, u64 ಅವಧಿ_us) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_cgroup_ಗಣ_idle(ರಚನೆ cgroup *cgrp, bool idle) {}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_GROUP_SCHED */
ಸ್ಥಿರ s32 sched_ext_ops__sub_attach(ರಚನೆ scx_sub_attach_ವಾದಗಳು *ವಾದಗಳು) { ಹಿಂತಿರುಗಿ -EINVAL; }
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__sub_detach(ರಚನೆ scx_sub_detach_ವಾದಗಳು *ವಾದಗಳು) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__cpu_online(s32 cpu) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__cpu_offline(s32 cpu) {}
ಸ್ಥಿರ s32 sched_ext_ops__init_cids(ಶೂನ್ಯ) { ಹಿಂತಿರುಗಿ -EINVAL; }
ಸ್ಥಿರ s32 sched_ext_ops__init(ಶೂನ್ಯ) { ಹಿಂತಿರುಗಿ -EINVAL; }
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__exit(ರಚನೆ scx_exit_info *info) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__dump(ರಚನೆ scx_dump_ctx *ctx) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__dump_cpu(ರಚನೆ scx_dump_ctx *ctx, s32 cpu, bool idle) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__dump_task(ರಚನೆ scx_dump_ctx *ctx, ರಚನೆ task_struct *p) {}

ಸ್ಥಿರ ರಚನೆ sched_ext_ops __bpf_ops_sched_ext_ops = {
	.select_cpu		= sched_ext_ops__select_cpu,
	.ಸರತಿಗೆ_ಸೇರಿಸು		= sched_ext_ops_ಸರತಿಗೆ_ಸೇರಿಸು,
	.ಸರತಿಯಿಂದ_ತೆಗೆ		= sched_ext_ops_ಸರತಿಯಿಂದ_ತೆಗೆ,
	.dispatch		= sched_ext_ops__dispatch,
	.tick			= sched_ext_ops__tick,
	.runnable		= sched_ext_ops__runnable,
	.running		= sched_ext_ops__running,
	.stopping		= sched_ext_ops__stopping,
	.quiescent		= sched_ext_ops__quiescent,
	.yield			= sched_ext_ops__yield,
	.core_sched_before	= sched_ext_ops__core_sched_before,
	.ಗಣ_ತೂಕ		= sched_ext_ops_ಗಣ_ತೂಕ,
	.ಗಣ_cpumask		= sched_ext_ops_ಗಣ_cpumask,
	.update_idle		= sched_ext_ops__update_idle,
	.cpu_acquire		= sched_ext_ops__cpu_acquire,
	.cpu_release		= sched_ext_ops__cpu_release,
	.init_task		= sched_ext_ops__init_task,
	.exit_task		= sched_ext_ops__exit_task,
	.enable			= sched_ext_ops__enable,
	.disable		= sched_ext_ops__disable,
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	.cgroup_init		= sched_ext_ops__cgroup_init,
	.cgroup_exit		= sched_ext_ops__cgroup_exit,
	.cgroup_prep_move	= sched_ext_ops__cgroup_prep_move,
	.cgroup_move		= sched_ext_ops__cgroup_move,
	.cgroup_cancel_move	= sched_ext_ops__cgroup_cancel_move,
	.cgroup_ಗಣ_ತೂಕ	= sched_ext_ops_cgroup_ಗಣ_ತೂಕ,
	.cgroup_ಗಣ_bandwidth	= sched_ext_ops_cgroup_ಗಣ_bandwidth,
	.cgroup_ಗಣ_idle	= sched_ext_ops_cgroup_ಗಣ_idle,
#ಆದರೆ_ಅಂತ್ಯ
	.sub_attach		= sched_ext_ops__sub_attach,
	.sub_detach		= sched_ext_ops__sub_detach,
	.cpu_online		= sched_ext_ops__cpu_online,
	.cpu_offline		= sched_ext_ops__cpu_offline,
	.init_cids		= sched_ext_ops__init_cids,
	.init			= sched_ext_ops__init,
	.ನಿರ್ಗಮಿಸು			= sched_ext_ops__exit,
	.dump			= sched_ext_ops__dump,
	.dump_cpu		= sched_ext_ops__dump_cpu,
	.dump_task		= sched_ext_ops__dump_task,
};

ಸ್ಥಿರ ರಚನೆ bpf_struct_ops bpf_sched_ext_ops = {
	.verifier_ops = &bpf_scx_verifier_ops,
	.reg = bpf_scx_reg,
	.unreg = bpf_scx_unreg,
	.check_member = bpf_scx_check_member,
	.init_member = bpf_scx_init_member,
	.init = bpf_scx_init,
	.update = bpf_scx_update,
	.validate = bpf_scx_validate,
	.name = "sched_ext_ops",
	.owner = THIS_MODULE,
	.cfi_stubs = &__bpf_ops_sched_ext_ops
};

/*
 * cid-form cfi stubs. Stubs whose signatures match the cpu-form (param types
 * identical, only param names differ across structs) are reused. Some need
 * fresh stubs, set_cmask and enable due to argument differences and the
 * sub-sched notifiers because no cpu-form stub exists to reuse.
 */
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_cid_ಗಣ_cmask(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *cmask__arena) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops_cid__enable(ರಚನೆ task_struct *p, ರಚನೆ scx_enable_ವಾದಗಳು *ವಾದಗಳು) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__sub_caps_updated(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *cmask__arena, u64 caps) {}
ಸ್ಥಿರ ಶೂನ್ಯ sched_ext_ops__sub_ecaps_updated(s32 cid, u64 before, u64 after) {}

ಸ್ಥಿರ ರಚನೆ sched_ext_ops_cid __bpf_ops_sched_ext_ops_cid = {
	.select_cid		= sched_ext_ops__select_cpu,
	.ಸರತಿಗೆ_ಸೇರಿಸು		= sched_ext_ops_ಸರತಿಗೆ_ಸೇರಿಸು,
	.ಸರತಿಯಿಂದ_ತೆಗೆ		= sched_ext_ops_ಸರತಿಯಿಂದ_ತೆಗೆ,
	.dispatch		= sched_ext_ops__dispatch,
	.tick			= sched_ext_ops__tick,
	.runnable		= sched_ext_ops__runnable,
	.running		= sched_ext_ops__running,
	.stopping		= sched_ext_ops__stopping,
	.quiescent		= sched_ext_ops__quiescent,
	.yield			= sched_ext_ops__yield,
	.core_sched_before	= sched_ext_ops__core_sched_before,
	.ಗಣ_ತೂಕ		= sched_ext_ops_ಗಣ_ತೂಕ,
	.ಗಣ_cmask		= sched_ext_ops_cid_ಗಣ_cmask,
	.update_idle		= sched_ext_ops__update_idle,
	.init_task		= sched_ext_ops__init_task,
	.exit_task		= sched_ext_ops__exit_task,
	.enable			= sched_ext_ops_cid__enable,
	.disable		= sched_ext_ops__disable,
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	.cpuctl_init		= sched_ext_ops__cgroup_init,
	.cpuctl_exit		= sched_ext_ops__cgroup_exit,
	.cpuctl_prep_move	= sched_ext_ops__cgroup_prep_move,
	.cpuctl_move		= sched_ext_ops__cgroup_move,
	.cpuctl_cancel_move	= sched_ext_ops__cgroup_cancel_move,
	.cpuctl_ಗಣ_ತೂಕ	= sched_ext_ops_cgroup_ಗಣ_ತೂಕ,
	.cpuctl_ಗಣ_bandwidth	= sched_ext_ops_cgroup_ಗಣ_bandwidth,
	.cpuctl_ಗಣ_idle	= sched_ext_ops_cgroup_ಗಣ_idle,
#ಆದರೆ_ಅಂತ್ಯ
	.sub_attach		= sched_ext_ops__sub_attach,
	.sub_detach		= sched_ext_ops__sub_detach,
	.sub_caps_updated	= sched_ext_ops__sub_caps_updated,
	.sub_ecaps_updated	= sched_ext_ops__sub_ecaps_updated,
	.cid_online		= sched_ext_ops__cpu_online,
	.cid_offline		= sched_ext_ops__cpu_offline,
	.init_cids		= sched_ext_ops__init_cids,
	.init			= sched_ext_ops__init,
	.ನಿರ್ಗಮಿಸು			= sched_ext_ops__exit,
	.dump			= sched_ext_ops__dump,
	.dump_cid		= sched_ext_ops__dump_cpu,
	.dump_task		= sched_ext_ops__dump_task,
};

/*
 * The cid-form struct_ops shares all bpf_struct_ops hooks with the cpu form.
 * init_member, check_member, reg, unreg, etc. process kdata as the byte block
 * verified to match by the BUILD_BUG_ON checks in scx_init().
 */
ಸ್ಥಿರ ರಚನೆ bpf_struct_ops bpf_sched_ext_ops_cid = {
	.verifier_ops = &bpf_scx_cid_verifier_ops,
	.reg = bpf_scx_reg_cid,
	.unreg = bpf_scx_unreg,
	.check_member = bpf_scx_check_member,
	.init_member = bpf_scx_init_member,
	.init = bpf_scx_init,
	.update = bpf_scx_update,
	.validate = bpf_scx_validate,
	.name = "sched_ext_ops_cid",
	.owner = THIS_MODULE,
	.cfi_stubs = &__bpf_ops_sched_ext_ops_cid
};


/********************************************************************************
 * System integration and init.
 */

ಸ್ಥಿರ ಶೂನ್ಯ sysrq_handle_sched_ext_reset(u8 ಕೀಲಿ)
{
	ರಚನೆ scx_sched *sch;

	sch = rcu_dereference(scx_ಬೇರು);
	ಆದರೆ (likely(sch))
		scx_disable(sch, SCX_EXIT_SYSRQ);
	ಇಲ್ಲದಿದ್ದರೆ
		pr_info("sched_ext: BPF schedulers not loaded\n");
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ sysrq_ಕೀಲಿ_op sysrq_sched_ext_reset_op = {
	.handler	= sysrq_handle_sched_ext_reset,
	.help_ಸಂದೇಶ	= "reset-sched-ext(S)",
	.action_ಸಂದೇಶ	= "Disable sched_ext and revert all tasks to CFS",
	.enable_ಮುಖವಾಡ	= SYSRQ_ENABLE_RTNICE,
};

ಸ್ಥಿರ ಶೂನ್ಯ sysrq_handle_sched_ext_dump(u8 ಕೀಲಿ)
{
	ರಚನೆ scx_exit_info ei = {
		.ಬಗೆ		= SCX_EXIT_NONE,
		.exit_cpu	= -1,
		.reason		= "SysRq-D",
	};
	ರಚನೆ scx_sched *sch;

	list_for_each_ನಮೂದು_rcu(sch, &scx_sched_all, all)
		scx_dump_state(sch, &ei, 0, false);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ sysrq_ಕೀಲಿ_op sysrq_sched_ext_dump_op = {
	.handler	= sysrq_handle_sched_ext_dump,
	.help_ಸಂದೇಶ	= "dump-sched-ext(D)",
	.action_ಸಂದೇಶ	= "Trigger sched_ext debug dump",
	.enable_ಮುಖವಾಡ	= SYSRQ_ENABLE_RTNICE,
};

ಸ್ಥಿರ bool can_skip_idle_kick(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);

	/*
	 * We can skip idle kicking if @rq is going to go through at least one
	 * full SCX scheduling cycle before going idle. Just checking whether
	 * curr is not idle is insufficient because we could be racing
	 * dispatch_one() trying to pull the next task from a remote rq, which
	 * may fail, and @rq may become idle afterwards.
	 *
	 * The race window is small and we don't and can't guarantee that @rq is
	 * only kicked while idle anyway. Skip only when sure.
	 */
	ಹಿಂತಿರುಗಿ !is_idle_task(rq->curr) && !(rq->scx.flags & SCX_RQ_ಒಳ_DISPATCH);
}

ಸ್ಥಿರ bool kick_one_cpu(s32 cpu, ರಚನೆ scx_sched_pcpu *pcpu, ರಚನೆ rq *this_rq,
			 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ksyncs)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ರಚನೆ scx_rq *this_scx = &this_rq->scx;
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *cur_class;
	bool should_wait = false;
	bool kickable;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	raw_spin_rq_ಬೀಗ_irqsave(rq, flags);
	cur_class = rq->curr->sched_class;

	/*
	 * During CPU hotplug, a CPU may depend on kicking itself to make
	 * forward progress. Allow kicking self regardless of online state. If
	 * @cpu is running a higher class task, we have no control over @cpu.
	 * Skip kicking. A sub-sched lacking baseline access on @cid has no
	 * business forcing a reschedule there - skip. This is the authoritative
	 * cap check: ecaps is read here under @rq's lock.
	 */
	kickable = (cpu_online(cpu) || cpu == cpu_ದ(this_rq)) &&
		   !sched_class_above(cur_class, &ext_sched_class);

	ಆದರೆ (kickable && !scx_missing_caps(pcpu->sch, cpu, SCX_CAP_ತಳ)) {
		ಆದರೆ (cpumask_test_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_preempt)) {
			ಆದರೆ (cur_class == &ext_sched_class) {
				u64 caps = scx_caps_for_preempt(pcpu->sch, rq, 0);

				ಆದರೆ (unlikely(scx_missing_caps(pcpu->sch, cpu, caps)))
					__scx_add_event(pcpu->sch, SCX_EV_SUB_PREEMPT_DENIED, 1);
				ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(!scx_ಗಣ_task_ತುಂಡು(rq->curr, 0)))
					__scx_add_event(pcpu->sch, SCX_EV_ತುಂಡು_DENIED, 1);
			}
			cpumask_clear_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_preempt);
		}

		ಆದರೆ (cpumask_test_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_wait)) {
			ಆದರೆ (cur_class == &ext_sched_class) {
				cpumask_ಗಣ_cpu(cpu, this_scx->cpus_ಗಮ್ಯ_sync);
				ksyncs[cpu] = rq->scx.kick_sync;
				should_wait = true;
			}
			cpumask_clear_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_wait);
		}

		resched_curr(rq);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* a kickable cpu was skipped solely for the missing caps */
		ಆದರೆ (kickable)
			__scx_add_event(pcpu->sch, SCX_EV_SUB_KICK_DENIED, 1);
		cpumask_clear_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_preempt);
		cpumask_clear_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_wait);
	}

	scx_rq_ಬೀಗ_drop(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	ಹಿಂತಿರುಗಿ should_wait;
}

ಸ್ಥಿರ ಶೂನ್ಯ kick_one_cpu_if_idle(s32 cpu, ರಚನೆ scx_sched_pcpu *pcpu,
				 ರಚನೆ rq *this_rq)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	raw_spin_rq_ಬೀಗ_irqsave(rq, flags);

	/* idle kicks need baseline access too, see kick_one_cpu() */
	ಆದರೆ (!can_skip_idle_kick(rq) &&
	    (cpu_online(cpu) || cpu == cpu_ದ(this_rq))) {
		ಆದರೆ (likely(!scx_missing_caps(pcpu->sch, cpu, SCX_CAP_ತಳ)))
			resched_curr(rq);
		ಇಲ್ಲದಿದ್ದರೆ
			__scx_add_event(pcpu->sch, SCX_EV_SUB_KICK_DENIED, 1);
	}

	scx_rq_ಬೀಗ_drop(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ kick_cpus_irq_workfn(ರಚನೆ irq_work *irq_work)
{
	ರಚನೆ rq *this_rq = this_rq();
	ರಚನೆ scx_rq *this_scx = &this_rq->scx;
	ರಚನೆ scx_kick_syncs __rcu *ksyncs_pcpu = __this_cpu_read(scx_kick_syncs);
	ರಚನೆ scx_sched_pcpu *pcpu, *tmp;
	bool should_wait = false;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ksyncs;
	s32 cpu;

	/* can race with free_kick_syncs() during scheduler disable */
	ಆದರೆ (unlikely(!ksyncs_pcpu))
		ಹಿಂತಿರುಗಿ;

	ksyncs = rcu_dereference_bh(ksyncs_pcpu)->syncs;

	/*
	 * Walk scheds with pending kicks on this cpu. scx_kick_cpu() adds to
	 * the list under local_irq_save() and only this irq_work consumes it.
	 * A plain list without locking is sufficient.
	 */
	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(pcpu, tmp, &this_scx->sched_pcpus_ಗಮ್ಯ_kick, ಗಮ್ಯ_kick_ಗಂಟು) {
		list_del_init(&pcpu->ಗಮ್ಯ_kick_ಗಂಟು);

		for_each_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_kick) {
			should_wait |= kick_one_cpu(cpu, pcpu, this_rq, ksyncs);
			cpumask_clear_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_kick);
			cpumask_clear_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_kick_if_idle);
		}

		for_each_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_kick_if_idle) {
			kick_one_cpu_if_idle(cpu, pcpu, this_rq);
			cpumask_clear_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_kick_if_idle);
		}
	}

	/*
	 * Can't wait in hardirq — kick_sync can't advance, deadlocking if
	 * CPUs wait for each other. Defer to kick_sync_wait_bal_cb().
	 */
	ಆದರೆ (should_wait) {
		raw_spin_rq_ಬೀಗ(this_rq);
		this_scx->kick_sync_pending = true;
		resched_curr(this_rq);
		scx_rq_ಬೀಗ_drop(this_rq);
		raw_spin_rq_unlock(this_rq);
	}
}

/**
 * print_scx_info - print out sched_ext scheduler state
 * @log_lvl: the log level to use when printing
 * @p: target task
 *
 * If a sched_ext scheduler is enabled, print the name and state of the
 * scheduler. If @p is on sched_ext, print further information about the task.
 *
 * This function can be safely called on any task as long as the task_struct
 * itself is accessible. While safe, this function isn't synchronized and may
 * print out mixups or garbages of limited length.
 */
ಶೂನ್ಯ print_scx_info(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *log_lvl, ರಚನೆ task_struct *p)
{
	ರಚನೆ scx_sched *sch;
	ಎಣಿಕೆ scx_enable_state state = scx_enable_state();
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *all = READ_ONCE(scx_switching_all) ? "+all" : "";
	ಅಕ್ಷರ runnable_at_buf[22] = "?";
	ರಚನೆ sched_class *class;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ runnable_at;

	guard(rcu)();

	sch = scx_task_sched_rcu(p);

	ಆದರೆ (!sch)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Carefully check if the task was running on sched_ext, and then
	 * carefully copy the time it's been runnable, and its state.
	 */
	ಆದರೆ (copy_from_kernel_nofault(&class, &p->sched_class, ಗಾತ್ರ(class)) ||
	    class != &ext_sched_class) {
		printk("%sSched_ext: %s (%s%s)", log_lvl, sch->ops.name,
		       scx_enable_state_str[state], all);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!copy_from_kernel_nofault(&runnable_at, &p->scx.runnable_at,
				      ಗಾತ್ರ(runnable_at)))
		scnprintf(runnable_at_buf, ಗಾತ್ರ(runnable_at_buf), "%+ldms",
			  jiffies_delta_msecs(runnable_at, jiffies));

	/* print everything onto one line to conserve console space */
	printk("%sSched_ext: %s (%s%s), task: runnable_at=%s",
	       log_lvl, sch->ops.name, scx_enable_state_str[state], all,
	       runnable_at_buf);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scx_pm_handler(ರಚನೆ notifier_ಖಂಡ *nb, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ event, ಶೂನ್ಯ *ptr)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ಬೇರು);
	ಆದರೆ (!sch)
		ಹಿಂತಿರುಗಿ NOTIFY_OK;

	/*
	 * SCX schedulers often have userspace components which are sometimes
	 * involved in critial scheduling paths. PM operations involve freezing
	 * userspace which can lead to scheduling misbehaviors including stalls.
	 * Let's bypass while PM operations are in progress.
	 */
	ಆಯ್ಕೆ (event) {
	ಸ್ಥಿತಿ PM_HIBERNATION_PREPARE:
	ಸ್ಥಿತಿ PM_SUSPEND_PREPARE:
	ಸ್ಥಿತಿ PM_RESTORE_PREPARE:
		scx_bypass(sch, true);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ PM_POST_HIBERNATION:
	ಸ್ಥಿತಿ PM_POST_SUSPEND:
	ಸ್ಥಿತಿ PM_POST_RESTORE:
		scx_bypass(sch, false);
		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ NOTIFY_OK;
}

ಸ್ಥಿರ ರಚನೆ notifier_ಖಂಡ scx_pm_notifier = {
	.notifier_call = scx_pm_handler,
};

ಶೂನ್ಯ __init init_sched_ext_class(ಶೂನ್ಯ)
{
	s32 cpu, v;

	/*
	 * The following is to prevent the compiler from optimizing out the enum
	 * definitions so that BPF scheduler implementations can use them
	 * through the generated vmlinux.h.
	 */
	WRITE_ONCE(v, SCX_ENQ_WAKEUP | SCX_DEQ_SLEEP | SCX_KICK_PREEMPT |
		   SCX_TG_ONLINE);

	scx_idle_init_masks();

	for_each_possible_cpu(cpu) {
		ರಚನೆ rq *rq = cpu_rq(cpu);
		ಪೂರ್ಣಾಂಕ  n = cpu_ಗಮ್ಯ_ಗಂಟು(cpu);

		/* local_dsq's sch will be set during scx_root_enable() */
		BUG_ON(scx_init_dsq(&rq->scx.local_dsq, SCX_DSQ_LOCAL, ಖಾಲಿ));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		BUG_ON(scx_init_dsq(&rq->scx.reject_dsq, SCX_DSQ_REJECT, ಖಾಲಿ));
		scx_rescue_init(rq);
#ಆದರೆ_ಅಂತ್ಯ

		INIT_LIST_ತಲೆ(&rq->scx.runnable_list);
		INIT_LIST_ತಲೆ(&rq->scx.ddsp_deferred_locals);

		BUG_ON(!zalloc_cpumask_var_ಗಂಟು(&rq->scx.cpus_ಗಮ್ಯ_sync, GFP_KERNEL, n));
		INIT_LIST_ತಲೆ(&rq->scx.sched_pcpus_ಗಮ್ಯ_kick);
		raw_spin_ಬೀಗ_init(&rq->scx.deferred_reenq_ಬೀಗ);
		INIT_LIST_ತಲೆ(&rq->scx.deferred_reenq_locals);
		INIT_LIST_ತಲೆ(&rq->scx.deferred_reenq_users);
		rq->scx.deferred_irq_work = IRQ_WORK_INIT_HARD(deferred_irq_workfn);
		rq->scx.kick_cpus_irq_work = IRQ_WORK_INIT_HARD(kick_cpus_irq_workfn);

		ಆದರೆ (cpu_online(cpu))
			cpu_rq(cpu)->scx.flags |= SCX_RQ_ONLINE;
	}

	ರಿಜಿಸ್ಟರ್_sysrq_ಕೀಲಿ('S', &sysrq_sched_ext_reset_op);
	ರಿಜಿಸ್ಟರ್_sysrq_ಕೀಲಿ('D', &sysrq_sched_ext_dump_op);
	INIT_DELAYED_WORK(&scx_watchdog_work, scx_watchdog_workfn);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	BUG_ON(rhashtable_init(&scx_sched_hash, &scx_sched_hash_params));
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */
}


/********************************************************************************
 * Helpers that can be called from the BPF scheduler.
 */
ಸ್ಥಿರ bool scx_vet_enq_flags(ರಚನೆ scx_sched *sch, u64 dsq_id, u64 *enq_flags)
{
	bool is_local = dsq_id == SCX_DSQ_LOCAL ||
		(dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON;

	ಆದರೆ (unlikely(*enq_flags & __SCX_ENQ_INTERNAL_MASK)) {
		scx_error(sch, "invalid enq_flags 0x%llx", *enq_flags);
		ಹಿಂತಿರುಗಿ false;
	}

	ಆದರೆ (*enq_flags & SCX_ENQ_IMMED) {
		ಆದರೆ (unlikely(!is_local)) {
			scx_error(sch, "SCX_ENQ_IMMED on a non-local DSQ 0x%llx", dsq_id);
			ಹಿಂತಿರುಗಿ false;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((sch->ops.flags & SCX_OPS_ALWAYS_ENQ_IMMED) && is_local) {
		*enq_flags |= SCX_ENQ_IMMED;
	}

	ಆದರೆ (unlikely((*enq_flags & SCX_ENQ_RESCUE) && !is_local)) {
		scx_error(sch, "SCX_ENQ_RESCUE on a non-local DSQ 0x%llx", dsq_id);
		ಹಿಂತಿರುಗಿ false;
	}

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool scx_dsq_ಸೇರಿಸು_preamble(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p,
				    u64 dsq_id, u64 *enq_flags)
{
	lockdep_assert_irqs_disabled();

	ಆದರೆ (unlikely(!p)) {
		scx_error(sch, "called with NULL task");
		ಹಿಂತಿರುಗಿ false;
	}

	/* see SCX_EV_INSERT_NOT_OWNED definition */
	ಆದರೆ (unlikely(!scx_task_on_sched(sch, p))) {
		__scx_add_event(sch, SCX_EV_ಸೇರಿಸು_NOT_OWNED, 1);
		ಹಿಂತಿರುಗಿ false;
	}

	ಆದರೆ (!scx_vet_enq_flags(sch, dsq_id, enq_flags))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_dsq_ಸೇರಿಸು_commit(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p,
				  u64 dsq_id, u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	ರಚನೆ scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	ರಚನೆ task_struct *ddsp_task;

	ddsp_task = __this_cpu_read(ನೇರ_dispatch_task);
	ಆದರೆ (ddsp_task) {
		mark_ನೇರ_dispatch(sch, ddsp_task, p, dsq_id, ತುಂಡು, vtime, enq_flags);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (unlikely(dspc->cursor >= sch->dsp_max_batch)) {
		scx_error(sch, "dispatch buffer overflow");
		ಹಿಂತಿರುಗಿ;
	}

	dspc->buf[dspc->cursor++] = (ರಚನೆ scx_dsp_buf_ent){
		.task = p,
		.qseq = atomic_long_read(&p->scx.ops_state) & SCX_OPSS_QSEQ_ಮುಖವಾಡ,
		.dsq_id = dsq_id,
		.ತುಂಡು = ತುಂಡು,
		.vtime = vtime,
		.enq_flags = enq_flags,
	};
}

__bpf_kfunc_start_defs();

/**
 * scx_bpf_dsq_insert___v2 - Insert a task into the FIFO queue of a DSQ
 * @p: task_struct to insert
 * @dsq_id: DSQ to insert into
 * @slice: duration @p can run for in nsecs, 0 to keep the current value
 * @enq_flags: SCX_ENQ_*
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Insert @p into the FIFO queue of the DSQ identified by @dsq_id. It is safe to
 * call this function spuriously. Can be called from ops.enqueue(),
 * ops.select_cpu(), and ops.dispatch().
 *
 * When called from ops.select_cpu() or ops.enqueue(), it's for direct dispatch
 * and @p must match the task being enqueued.
 *
 * When called from ops.select_cpu(), @enq_flags and @dsq_id are stored, and @p
 * will be directly inserted into the corresponding dispatch queue after
 * ops.select_cpu() returns. If @p is inserted into SCX_DSQ_LOCAL, it will be
 * inserted into the local DSQ of the CPU returned by ops.select_cpu().
 * @enq_flags are OR'd with the enqueue flags on the enqueue path before the
 * task is inserted.
 *
 * When called from ops.dispatch(), there are no restrictions on @p or @dsq_id
 * and this function can be called upto ops.dispatch_max_batch times to insert
 * multiple tasks. scx_bpf_dispatch_nr_slots() returns the number of the
 * remaining slots. scx_bpf_dsq_move_to_local() flushes the batch and resets the
 * counter.
 *
 * This function doesn't have any locking restrictions and may be called under
 * BPF locks (in the future when BPF introduces more flexible locking).
 *
 * @p is allowed to run for @slice. The scheduling path is triggered on slice
 * exhaustion. If zero, the current residual slice is maintained. If
 * %SCX_SLICE_INF, @p never expires and the BPF scheduler must kick the CPU with
 * scx_bpf_kick_cpu() to trigger scheduling.
 *
 * Returns %true on successful insertion, %false on failure. On the root
 * scheduler, %false return triggers scheduler abort and the caller doesn't need
 * to check the return value.
 */
__bpf_kfunc bool scx_bpf_dsq_ಸೇರಿಸು_v2(ರಚನೆ task_struct *p, u64 dsq_id,
					 u64 ತುಂಡು, u64 enq_flags,
					 ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (!scx_dsq_ಸೇರಿಸು_preamble(sch, p, dsq_id, &enq_flags))
		ಹಿಂತಿರುಗಿ false;

	scx_dsq_ಸೇರಿಸು_commit(sch, p, dsq_id, ತುಂಡು, 0, enq_flags);

	ಹಿಂತಿರುಗಿ true;
}

/*
 * COMPAT: Will be removed in v6.23 along with the ___v2 suffix.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_dsq_ಸೇರಿಸು(ರಚನೆ task_struct *p, u64 dsq_id,
				    u64 ತುಂಡು, u64 enq_flags,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	scx_bpf_dsq_ಸೇರಿಸು_v2(p, dsq_id, ತುಂಡು, enq_flags, aux);
}

ಸ್ಥಿರ bool scx_dsq_ಸೇರಿಸು_vtime(ರಚನೆ scx_sched *sch, ರಚನೆ task_struct *p,
				 u64 dsq_id, u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	ಆದರೆ (!scx_dsq_ಸೇರಿಸು_preamble(sch, p, dsq_id, &enq_flags))
		ಹಿಂತಿರುಗಿ false;

	scx_dsq_ಸೇರಿಸು_commit(sch, p, dsq_id, ತುಂಡು, vtime, enq_flags | SCX_ENQ_DSQ_PRIQ);

	ಹಿಂತಿರುಗಿ true;
}

ರಚನೆ scx_bpf_dsq_ಸೇರಿಸು_vtime_ವಾದಗಳು {
	/* @p can't be packed together as KF_RCU is not transitive */
	u64			dsq_id;
	u64			ತುಂಡು;
	u64			vtime;
	u64			enq_flags;
};

/**
 * __scx_bpf_dsq_insert_vtime - Arg-wrapped vtime DSQ insertion
 * @p: task_struct to insert
 * @args: struct containing the rest of the arguments
 *       @args->dsq_id: DSQ to insert into
 *       @args->slice: duration @p can run for in nsecs, 0 to keep the current value
 *       @args->vtime: @p's ordering inside the vtime-sorted queue of the target DSQ
 *       @args->enq_flags: SCX_ENQ_*
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Wrapper kfunc that takes arguments via struct to work around BPF's 5 argument
 * limit. BPF programs should use scx_bpf_dsq_insert_vtime() which is provided
 * as an inline wrapper in common.bpf.h.
 *
 * Insert @p into the vtime priority queue of the DSQ identified by
 * @args->dsq_id. Tasks queued into the priority queue are ordered by
 * @args->vtime. All other aspects are identical to scx_bpf_dsq_insert().
 *
 * @args->vtime ordering is according to time_before64() which considers
 * wrapping. A numerically larger vtime may indicate an earlier position in the
 * ordering and vice-versa.
 *
 * A DSQ can only be used as a FIFO or priority queue at any given time and this
 * function must not be called on a DSQ which already has one or more FIFO tasks
 * queued and vice-versa. Also, the built-in DSQs (SCX_DSQ_LOCAL and
 * SCX_DSQ_GLOBAL) cannot be used as priority queues.
 *
 * Returns %true on successful insertion, %false on failure. On the root
 * scheduler, %false return triggers scheduler abort and the caller doesn't need
 * to check the return value.
 */
__bpf_kfunc bool
__scx_bpf_dsq_insert_vtime(ರಚನೆ task_struct *p,
			   ರಚನೆ scx_bpf_dsq_ಸೇರಿಸು_vtime_ವಾದಗಳು *ವಾದಗಳು,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ scx_dsq_ಸೇರಿಸು_vtime(sch, p, ವಾದಗಳು->dsq_id, ವಾದಗಳು->ತುಂಡು,
				    ವಾದಗಳು->vtime, ವಾದಗಳು->enq_flags);
}

/*
 * COMPAT: Will be removed in v6.23.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_dsq_ಸೇರಿಸು_vtime(ರಚನೆ task_struct *p, u64 dsq_id,
					  u64 ತುಂಡು, u64 vtime, u64 enq_flags)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ಬೇರು);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	/*
	 * Disallow if any sub-scheds are attached. There is no way to tell
	 * which scheduler called us, so error out @p's scheduler -- read it
	 * under RCU as @p's locks aren't necessarily held here. @p may be a
	 * task past sched_ext_dead() or an idle task, in which case its
	 * scheduler can't be determined and there is nothing obviously wrong
	 * to report; just refuse the call.
	 */
	ಆದರೆ (unlikely(!list_ಖಾಲಿಗಳು(&sch->children))) {
		ರಚನೆ scx_sched *tsch = scx_task_sched_rcu(p);

		ಆದರೆ (tsch)
			scx_error(tsch, "__scx_bpf_dsq_insert_vtime() must be used");
		ಹಿಂತಿರುಗಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ

	scx_dsq_ಸೇರಿಸು_vtime(sch, p, dsq_id, ತುಂಡು, vtime, enq_flags);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_ಸರತಿಗೆ_ಸೇರಿಸು_dispatch)
BTF_ID_FLAGS(func, scx_bpf_dsq_ಸೇರಿಸು, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_ಸೇರಿಸು_v2, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, __scx_bpf_dsq_insert_vtime, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_ಸೇರಿಸು_vtime, KF_RCU)
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_ಸರತಿಗೆ_ಸೇರಿಸು_dispatch)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_kfunc_id_ಗಣ scx_kfunc_ಗಣ_ಸರತಿಗೆ_ಸೇರಿಸು_dispatch = {
	.owner			= THIS_MODULE,
	.ಗಣ			= &scx_kfunc_ids_ಸರತಿಗೆ_ಸೇರಿಸು_dispatch,
	.filter			= scx_kfunc_context_filter,
};

ಸ್ಥಿರ bool scx_dsq_move(ರಚನೆ bpf_iter_scx_dsq_kern *kit,
			 ರಚನೆ task_struct *p, u64 dsq_id, u64 enq_flags,
			 bool priq)
{
	ರಚನೆ scx_dispatch_q *src_dsq = kit->dsq, *dst_dsq;
	ರಚನೆ scx_sched *sch;
	ರಚನೆ rq *p_rq, *src_rq, *locked_rq;
	bool dispatched = false;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	/*
	 * The verifier considers an iterator slot initialized on any
	 * KF_ITER_NEW return, so a BPF program may legally reach here after
	 * bpf_iter_scx_dsq_new() failed and left @kit->dsq NULL.
	 */
	ಆದರೆ (unlikely(!src_dsq))
		ಹಿಂತಿರುಗಿ false;

	sch = src_dsq->sched;

	ಆದರೆ (!scx_vet_enq_flags(sch, dsq_id, &enq_flags))
		ಹಿಂತಿರುಗಿ false;

	/* internal bit, can only go in after @enq_flags is vetted */
	ಆದರೆ (priq)
		enq_flags |= SCX_ENQ_DSQ_PRIQ;

	/*
	 * If the BPF scheduler keeps calling this function repeatedly, it can
	 * cause similar live-lock conditions as scx_consume_dispatch_q().
	 */
	ಆದರೆ (unlikely(READ_ONCE(sch->aborting)))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Can be called from either ops.dispatch() holding the dispatched rq's
	 * lock or any context where no rq lock is held. If latter, lock @p's
	 * task_rq which we'll likely need anyway.
	 */
	src_rq = task_rq(p);

	local_irq_save(flags);

	/*
	 * Under core scheduling, dispatch can run for a sibling rq, so the
	 * locked rq is not necessarily this CPU's.
	 */
	locked_rq = scx_locked_rq();

	ಆದರೆ (locked_rq) {
		ಆದರೆ (locked_rq != src_rq)
			switch_rq_ಬೀಗ(locked_rq, src_rq);
	} ಇಲ್ಲದಿದ್ದರೆ {
		raw_spin_rq_ಬೀಗ(src_rq);
	}

	p_rq = src_rq;
	raw_spin_ಬೀಗ(&src_dsq->ಬೀಗ);

	/* did someone else get to it while we dropped the locks? */
	ಆದರೆ (nldsq_cursor_ಕಳೆದುಹೋದ_task(&kit->cursor, src_rq, src_dsq, p)) {
		raw_spin_unlock(&src_dsq->ಬೀಗ);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * @p has been on $src_dsq and can't move anymore. If @p is not on @sch,
	 * the caller didn't have authority over @p at the time of the call.
	 */
	ಆದರೆ (unlikely(!scx_task_on_sched(sch, p))) {
		scx_error(sch, "scx_bpf_dsq_move[_vtime]() on %s[%d] but the task belongs to a different scheduler",
			  p->comm, p->pid);
		raw_spin_unlock(&src_dsq->ಬೀಗ);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* @p is still on $src_dsq and stable, determine the destination */
	dst_dsq = ಕಂಡುಹಿಡಿ_dsq_for_dispatch(sch, locked_rq ?: this_rq(), dsq_id, task_cpu(p));

	/*
	 * Apply vtime and slice updates before moving. @p is still on $src_dsq
	 * with both $src_dsq and its task_rq locked, satisfying the write
	 * rules, and the PRIQ insertion into $dst_dsq reads the new vtime.
	 */
	ಆದರೆ (kit->cursor.flags & __SCX_DSQ_ITER_HAS_VTIME)
		p->scx.dsq_vtime = kit->vtime;
	ಆದರೆ (kit->cursor.flags & __SCX_DSQ_ITER_HAS_SLICE)
		scx_ಗಣ_task_ತುಂಡು(p, kit->ತುಂಡು);

	/* execute move */
	p_rq = move_task_between_dsqs(sch, p, enq_flags, src_dsq, dst_dsq);
	dispatched = true;
ಹೊರಹೋಗು:
	ಆದರೆ (locked_rq) {
		ಆದರೆ (locked_rq != p_rq)
			switch_rq_ಬೀಗ(p_rq, locked_rq);
	} ಇಲ್ಲದಿದ್ದರೆ {
		scx_rq_ಬೀಗ_drop(p_rq);
		raw_spin_rq_unlock_irqrestore(p_rq, flags);
	}

	kit->cursor.flags &= ~(__SCX_DSQ_ITER_HAS_SLICE |
			       __SCX_DSQ_ITER_HAS_VTIME);
	ಹಿಂತಿರುಗಿ dispatched;
}

__bpf_kfunc_start_defs();

/**
 * scx_bpf_dispatch_nr_slots - Return the number of remaining dispatch slots
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Can only be called from ops.dispatch().
 */
__bpf_kfunc u32 scx_bpf_dispatch_nr_slots(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ sch->dsp_max_batch - __this_cpu_read(sch->pcpu->dsp_ctx.cursor);
}

/**
 * scx_bpf_dispatch_cancel - Cancel the latest dispatch
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Cancel the latest dispatch. Can be called multiple times to cancel further
 * dispatches. Can only be called from ops.dispatch().
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_dispatch_cancel(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_dsp_ctx *dspc;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;

	dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;

	ಆದರೆ (dspc->cursor > 0)
		dspc->cursor--;
	ಇಲ್ಲದಿದ್ದರೆ
		scx_error(sch, "dispatch buffer underflow");
}

/**
 * scx_bpf_dsq_move_to_local___v2 - move a task from a DSQ to the current CPU's local DSQ
 * @dsq_id: DSQ to move task from. Must be a user-created DSQ
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 * @enq_flags: %SCX_ENQ_*
 *
 * Move a task from the non-local DSQ identified by @dsq_id to the current CPU's
 * local DSQ for execution with @enq_flags applied. Can only be called from
 * ops.dispatch().
 *
 * Built-in DSQs (%SCX_DSQ_GLOBAL and %SCX_DSQ_LOCAL*) are not supported as
 * sources. Local DSQs support reenqueueing (a task can be picked up for
 * execution, dequeued for property changes, or reenqueued), but the BPF
 * scheduler cannot directly iterate or move tasks from them. %SCX_DSQ_GLOBAL
 * is similar but also doesn't support reenqueueing, as it maps to multiple
 * per-node DSQs making the scope difficult to define; this may change in the
 * future.
 *
 * This function flushes the in-flight dispatches from scx_bpf_dsq_insert()
 * before trying to move from the specified DSQ. It may also grab rq locks and
 * thus can't be called under any BPF locks.
 *
 * Returns %true if a task has been moved, %false if there isn't any task to
 * move.
 */
__bpf_kfunc bool scx_bpf_dsq_move_ಗಮ್ಯ_local_v2(u64 dsq_id, u64 enq_flags,
						ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_dispatch_q *dsq;
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_dsp_ctx *dspc;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (!scx_vet_enq_flags(sch, SCX_DSQ_LOCAL, &enq_flags))
		ಹಿಂತಿರುಗಿ false;

	dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;

	scx_flush_dispatch_buf(sch, dspc->rq);

	dsq = ಕಂಡುಹಿಡಿ_user_dsq(sch, dsq_id);
	ಆದರೆ (unlikely(!dsq)) {
		scx_error(sch, "invalid DSQ ID 0x%016llx", dsq_id);
		ಹಿಂತಿರುಗಿ false;
	}

	ಆದರೆ (scx_consume_dispatch_q(sch, dspc->rq, dsq, enq_flags)) {
		/*
		 * A successfully consumed task can be dequeued before it starts
		 * running while the CPU is trying to migrate other dispatched
		 * tasks. Bump nr_tasks to tell dispatch_one() to retry on empty
		 * local DSQ.
		 */
		dspc->nr_tasks++;
		ಹಿಂತಿರುಗಿ true;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ false;
	}
}

/*
 * COMPAT: ___v2 was introduced in v7.1. Remove this and ___v2 tag in the future.
 */
__bpf_kfunc bool scx_bpf_dsq_move_ಗಮ್ಯ_local(u64 dsq_id, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ಹಿಂತಿರುಗಿ scx_bpf_dsq_move_ಗಮ್ಯ_local_v2(dsq_id, 0, aux);
}

/**
 * scx_bpf_dsq_move_set_slice - Override slice when moving between DSQs
 * @it__iter: DSQ iterator in progress
 * @slice: duration the moved task can run for in nsecs
 *
 * Override the slice of the next task that will be moved from @it__iter using
 * scx_bpf_dsq_move[_vtime](). If this function is not called, the previous
 * slice duration is kept.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_dsq_move_ಗಣ_ತುಂಡು(ರಚನೆ bpf_iter_scx_dsq *it__iter,
					    u64 ತುಂಡು)
{
	ರಚನೆ bpf_iter_scx_dsq_kern *kit = (ಶೂನ್ಯ *)it__iter;

	kit->ತುಂಡು = ತುಂಡು;
	kit->cursor.flags |= __SCX_DSQ_ITER_HAS_SLICE;
}

/**
 * scx_bpf_dsq_move_set_vtime - Override vtime when moving between DSQs
 * @it__iter: DSQ iterator in progress
 * @vtime: task's ordering inside the vtime-sorted queue of the target DSQ
 *
 * Override the vtime of the next task that will be moved from @it__iter using
 * scx_bpf_dsq_move_vtime(). If this function is not called, the previous slice
 * vtime is kept. If scx_bpf_dsq_move() is used to dispatch the next task, the
 * override is ignored and cleared.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_dsq_move_ಗಣ_vtime(ರಚನೆ bpf_iter_scx_dsq *it__iter,
					    u64 vtime)
{
	ರಚನೆ bpf_iter_scx_dsq_kern *kit = (ಶೂನ್ಯ *)it__iter;

	kit->vtime = vtime;
	kit->cursor.flags |= __SCX_DSQ_ITER_HAS_VTIME;
}

/**
 * scx_bpf_dsq_move - Move a task from DSQ iteration to a DSQ
 * @it__iter: DSQ iterator in progress
 * @p: task to transfer
 * @dsq_id: DSQ to move @p to
 * @enq_flags: SCX_ENQ_*
 *
 * Transfer @p which is on the DSQ currently iterated by @it__iter to the DSQ
 * specified by @dsq_id. All DSQs - local DSQs, global DSQ and user DSQs - can
 * be the destination.
 *
 * For the transfer to be successful, @p must still be on the DSQ and have been
 * queued before the DSQ iteration started. This function doesn't care whether
 * @p was obtained from the DSQ iteration. @p just has to be on the DSQ and have
 * been queued before the iteration started.
 *
 * @p's slice is kept by default. Use scx_bpf_dsq_move_set_slice() to update.
 *
 * Can be called from ops.dispatch() or any BPF context which doesn't hold a rq
 * lock (e.g. BPF timers or SYSCALL programs).
 *
 * Returns %true if @p has been consumed, %false if @p had already been
 * consumed, dequeued, or, for sub-scheds, @dsq_id points to a disallowed local
 * DSQ.
 */
__bpf_kfunc bool scx_bpf_dsq_move(ರಚನೆ bpf_iter_scx_dsq *it__iter,
				  ರಚನೆ task_struct *p, u64 dsq_id,
				  u64 enq_flags)
{
	ಹಿಂತಿರುಗಿ scx_dsq_move((ರಚನೆ bpf_iter_scx_dsq_kern *)it__iter,
			    p, dsq_id, enq_flags, false);
}

/**
 * scx_bpf_dsq_move_vtime - Move a task from DSQ iteration to a PRIQ DSQ
 * @it__iter: DSQ iterator in progress
 * @p: task to transfer
 * @dsq_id: DSQ to move @p to
 * @enq_flags: SCX_ENQ_*
 *
 * Transfer @p which is on the DSQ currently iterated by @it__iter to the
 * priority queue of the DSQ specified by @dsq_id. The destination must be a
 * user DSQ as only user DSQs support priority queue.
 *
 * @p's slice and vtime are kept by default. Use scx_bpf_dsq_move_set_slice()
 * and scx_bpf_dsq_move_set_vtime() to update.
 *
 * All other aspects are identical to scx_bpf_dsq_move(). See
 * scx_bpf_dsq_insert_vtime() for more information on @vtime.
 */
__bpf_kfunc bool scx_bpf_dsq_move_vtime(ರಚನೆ bpf_iter_scx_dsq *it__iter,
					ರಚನೆ task_struct *p, u64 dsq_id,
					u64 enq_flags)
{
	ಹಿಂತಿರುಗಿ scx_dsq_move((ರಚನೆ bpf_iter_scx_dsq_kern *)it__iter,
			    p, dsq_id, enq_flags, true);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_dispatch)
BTF_ID_FLAGS(func, scx_bpf_dispatch_nr_slots, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_dispatch_cancel, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ಗಮ್ಯ_local, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ಗಮ್ಯ_local_v2, KF_IMPLICIT_ವಾದಗಳು)
/* scx_bpf_dsq_move*() also in scx_kfunc_ids_unlocked: callable from unlocked contexts */
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ಗಣ_ತುಂಡು, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ಗಣ_vtime, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_vtime, KF_RCU)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
BTF_ID_FLAGS(func, scx_bpf_sub_dispatch, KF_IMPLICIT_ವಾದಗಳು)
#ಆದರೆ_ಅಂತ್ಯ
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_dispatch)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_kfunc_id_ಗಣ scx_kfunc_ಗಣ_dispatch = {
	.owner			= THIS_MODULE,
	.ಗಣ			= &scx_kfunc_ids_dispatch,
	.filter			= scx_kfunc_context_filter,
};

__bpf_kfunc_start_defs();

/**
 * scx_bpf_reenqueue_local - Re-enqueue tasks on a local DSQ
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Iterate over all of the tasks currently enqueued on the local DSQ of the
 * caller's CPU, and re-enqueue them in the BPF scheduler. Returns the number of
 * processed tasks. Can only be called from ops.cpu_release().
 */
__bpf_kfunc u32 scx_bpf_reenqueue_local(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ rq *rq;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ 0;

	rq = cpu_rq(smp_processor_id());
	lockdep_assert_rq_held(rq);

	ಹಿಂತಿರುಗಿ reenq_local(sch, rq, SCX_REENQ_ANY);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_cpu_release)
BTF_ID_FLAGS(func, scx_bpf_reenqueue_local, KF_IMPLICIT_ವಾದಗಳು)
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_cpu_release)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_kfunc_id_ಗಣ scx_kfunc_ಗಣ_cpu_release = {
	.owner			= THIS_MODULE,
	.ಗಣ			= &scx_kfunc_ids_cpu_release,
	.filter			= scx_kfunc_context_filter,
};

__bpf_kfunc_start_defs();

/**
 * scx_bpf_create_dsq - Create a custom DSQ
 * @dsq_id: DSQ to create
 * @node: NUMA node to allocate from
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Create a custom DSQ identified by @dsq_id. Can be called from any sleepable
 * scx callback, and any BPF_PROG_TYPE_SYSCALL prog.
 */
__bpf_kfunc s32 scx_bpf_create_dsq(u64 dsq_id, s32 ಗಂಟು, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_dispatch_q *dsq;
	ರಚನೆ scx_sched *sch;
	s32 ret;

	ಆದರೆ (unlikely(ಗಂಟು >= (ಪೂರ್ಣಾಂಕ)nr_ಗಂಟು_ids ||
		     (ಗಂಟು < 0 && ಗಂಟು != NUMA_NO_ಗಂಟು)))
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಆದರೆ (unlikely(dsq_id & SCX_DSQ_FLAG_BUILTIN))
		ಹಿಂತಿರುಗಿ -EINVAL;

	dsq = kmalloc_ಗಂಟು(ಗಾತ್ರ(*dsq), GFP_KERNEL, ಗಂಟು);
	ಆದರೆ (!dsq)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	/*
	 * scx_init_dsq() must be called in GFP_KERNEL context. Init it with
	 * NULL @sch and update afterwards.
	 */
	ret = scx_init_dsq(dsq, dsq_id, ಖಾಲಿ);
	ಆದರೆ (ret) {
		kfree(dsq);
		ಹಿಂತಿರುಗಿ ret;
	}

	rcu_read_ಬೀಗ();

	sch = scx_prog_sched(aux);
	ಆದರೆ (sch) {
		dsq->sched = sch;
		ret = rhashtable_lookup_ಸೇರಿಸು_fast(&sch->dsq_hash, &dsq->hash_ಗಂಟು,
						    dsq_hash_params);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = -ENODEV;
	}

	rcu_read_unlock();
	ಆದರೆ (ret) {
		exit_dsq(dsq);
		kfree(dsq);
	}
	ಹಿಂತಿರುಗಿ ret;
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_unlocked)
BTF_ID_FLAGS(func, scx_bpf_create_dsq, KF_IMPLICIT_ವಾದಗಳು | KF_SLEEPABLE)
/* also in scx_kfunc_ids_dispatch: also callable from ops.dispatch() */
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ಗಣ_ತುಂಡು, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ಗಣ_vtime, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_vtime, KF_RCU)
/* also in scx_kfunc_ids_select_cpu: also callable from ops.select_cpu()/ops.enqueue() */
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_unlocked)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_kfunc_id_ಗಣ scx_kfunc_ಗಣ_unlocked = {
	.owner			= THIS_MODULE,
	.ಗಣ			= &scx_kfunc_ids_unlocked,
	.filter			= scx_kfunc_context_filter,
};

__bpf_kfunc_start_defs();

/**
 * scx_bpf_task_set_slice - Set task's time slice
 * @p: task of interest
 * @slice: time slice to set in nsecs
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Set @p's time slice. @p must be on the calling scheduler. The value is
 * applied whether or not the caller holds @p's rq lock - see the slice write
 * rules above for the ownership model.
 *
 * Raising the slice is honored only while the scheduler holds %SCX_CAP_BASE on
 * @p's cpu, otherwise it is counted in %SCX_EV_SLICE_DENIED. Shortening is
 * always allowed. On the stashed path the slice is packed into an atomic64_t
 * with the scheduler id and a flag bit, so a slice too large to fit is clamped
 * and counted in %SCX_EV_SLICE_CLAMPED. %SCX_SLICE_INF is preserved.
 *
 * Return %true on success, %false if @p is not on the calling scheduler.
 */
__bpf_kfunc bool scx_bpf_task_ಗಣ_ತುಂಡು(ರಚನೆ task_struct *p, u64 ತುಂಡು,
					ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ rq *locked_rq;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch || !scx_task_on_sched(sch, p)))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Directly write only when we hold the lock of the rq @p is queued or
	 * running on. See the write rules above.
	 *
	 * While @p is queued on a user DSQ or in the BPF scheduler,
	 * synchronization is the scheduler's responsibility. This write can
	 * race a concurrent dispatch's commit, see apply_slice_vtime().
	 *
	 * Making this kfunc always go through the oob stash would leave the
	 * commit as the only direct writer and close the race, but that would
	 * require two more oob application points - the dispatch keep-prev test
	 * and the tick-time expiry check.
	 */
	locked_rq = scx_locked_rq();
	ಆದರೆ (!locked_rq ||
	    (READ_ONCE(p->scx.runnable_cpu) != cpu_ದ(locked_rq) &&
	     !task_current(locked_rq, p))) {
		ಗಣ_task_ತುಂಡು_oob(sch, p, ತುಂಡು);
		ಹಿಂತಿರುಗಿ true;
	}

	/* under the rq lock: apply now, extensions gated on baseline access */
	ಆದರೆ (ತುಂಡು > p->scx.ತುಂಡು &&
	    unlikely(scx_missing_caps(sch, cpu_ದ(locked_rq), SCX_CAP_ತಳ))) {
		__scx_add_event(sch, SCX_EV_ತುಂಡು_DENIED, 1);
		ಹಿಂತಿರುಗಿ true;
	}

	ಆದರೆ (unlikely(!scx_ಗಣ_task_ತುಂಡು(p, ತುಂಡು)))
		__scx_add_event(sch, SCX_EV_ತುಂಡು_DENIED, 1);

	ಹಿಂತಿರುಗಿ true;
}

/**
 * scx_bpf_task_set_dsq_vtime - Set task's virtual time for DSQ ordering
 * @p: task of interest
 * @vtime: virtual time to set
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Set @p's virtual time to @vtime. Returns %true on success, %false if the
 * calling scheduler doesn't have authority over @p.
 */
__bpf_kfunc bool scx_bpf_task_ಗಣ_dsq_vtime(ರಚನೆ task_struct *p, u64 vtime,
					    ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch || !scx_task_on_sched(sch, p)))
		ಹಿಂತಿರುಗಿ false;

	p->scx.dsq_vtime = vtime;
	ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ scx_kick_cpu(ರಚನೆ scx_sched *sch, s32 cpu, u64 flags)
{
	ರಚನೆ scx_sched_pcpu *pcpu;
	ರಚನೆ rq *this_rq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ irq_flags;

	/*
	 * The per-cpu kick list is guarded only by local_irq_save(), which does
	 * not mask NMIs, so kicking from NMI could corrupt it and is unsupported.
	 */
	ಆದರೆ (unlikely(ಒಳ_nmi())) {
		scx_error(sch, "scx_bpf_kick_cpu() called from NMI");
		ಹಿಂತಿರುಗಿ;
	}

	local_irq_save(irq_flags);

	this_rq = this_rq();
	pcpu = this_cpu_ptr(sch->pcpu);

	/*
	 * While bypassing for PM ops, IRQ handling may not be online which can
	 * lead to irq_work_queue() malfunction such as infinite busy wait for
	 * IRQ status update. Suppress kicking.
	 */
	ಆದರೆ (scx_bypassing(sch, cpu_ದ(this_rq)))
		ಹೋಗು ಹೊರಹೋಗು;

	/*
	 * Actual kicking is bounced to kick_cpus_irq_workfn() to avoid nesting
	 * rq locks. We can probably be smarter and avoid bouncing if called
	 * from ops which don't hold a rq lock.
	 *
	 * The kick masks are owned by @sch->pcpu, so that a preempt kick can be
	 * attributed to @sch.
	 */
	ಆದರೆ (flags & SCX_KICK_IDLE) {
		ರಚನೆ rq *ಗುರಿ_rq = cpu_rq(cpu);

		ಆದರೆ (unlikely(flags & (SCX_KICK_PREEMPT | SCX_KICK_WAIT)))
			scx_error(sch, "PREEMPT/WAIT cannot be used with SCX_KICK_IDLE");

		ಆದರೆ (raw_spin_rq_trylock(ಗುರಿ_rq)) {
			ಆದರೆ (can_skip_idle_kick(ಗುರಿ_rq)) {
				scx_rq_ಬೀಗ_drop(ಗುರಿ_rq);
				raw_spin_rq_unlock(ಗುರಿ_rq);
				ಹೋಗು ಹೊರಹೋಗು;
			}
			scx_rq_ಬೀಗ_drop(ಗುರಿ_rq);
			raw_spin_rq_unlock(ಗುರಿ_rq);
		}
		cpumask_ಗಣ_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_kick_if_idle);
	} ಇಲ್ಲದಿದ್ದರೆ {
		cpumask_ಗಣ_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_kick);

		ಆದರೆ (flags & SCX_KICK_PREEMPT)
			cpumask_ಗಣ_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_preempt);
		ಆದರೆ (flags & SCX_KICK_WAIT)
			cpumask_ಗಣ_cpu(cpu, pcpu->cpus_ಗಮ್ಯ_wait);
	}

	ಆದರೆ (list_ಖಾಲಿಗಳು(&pcpu->ಗಮ್ಯ_kick_ಗಂಟು))
		list_ಕೂಡಿಸು_ಬಾಲ(&pcpu->ಗಮ್ಯ_kick_ಗಂಟು, &this_rq->scx.sched_pcpus_ಗಮ್ಯ_kick);
	irq_work_ಸರತಿ(&this_rq->scx.kick_cpus_irq_work);
ಹೊರಹೋಗು:
	local_irq_restore(irq_flags);
}

/**
 * scx_bpf_kick_cpu - Trigger reschedule on a CPU
 * @cpu: cpu to kick
 * @flags: %SCX_KICK_* flags
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Kick @cpu into rescheduling. This can be used to wake up an idle CPU or
 * trigger rescheduling on a busy CPU. This can be called from any online
 * scx_ops operation and the actual kicking is performed asynchronously through
 * an irq work.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_kick_cpu(s32 cpu, u64 flags, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ಆದರೆ (likely(sch) && scx_cpu_valid(sch, cpu, ಖಾಲಿ))
		scx_kick_cpu(sch, cpu, flags);
}

/**
 * scx_bpf_kick_cid - Trigger reschedule on the CPU mapped to @cid
 * @cid: cid to kick
 * @flags: %SCX_KICK_* flags
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_kick_cpu(). An invalid @cid aborts the
 * scheduler via scx_cid_to_cpu(). Caps are enforced on the delivery path: a
 * kick is dropped if the caller lacks baseline access on @cid, and a
 * %SCX_KICK_PREEMPT degrades to a plain reschedule if the caller lacks
 * %SCX_CAP_PREEMPT for a task outside its subtree.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_kick_cid(s32 cid, u64 flags, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	s32 cpu;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;
	cpu = scx_cid_ಗಮ್ಯ_cpu(sch, cid);
	ಆದರೆ (cpu < 0)
		ಹಿಂತಿರುಗಿ;
	scx_kick_cpu(sch, cpu, flags);
}

/**
 * scx_bpf_dsq_nr_queued - Return the number of queued tasks
 * @dsq_id: id of the DSQ
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Return the number of tasks in the DSQ matching @dsq_id. If not found,
 * -%ENOENT is returned.
 *
 * %SCX_DSQ_LOCAL resolves to the local DSQ of the rq the current scheduler
 * operation is locked to - e.g. the rq being dispatched for in ops.dispatch() -
 * or the calling CPU's when no rq is locked.
 */
__bpf_kfunc s32 scx_bpf_dsq_nr_queued(u64 dsq_id, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_dispatch_q *dsq;
	s32 ret;

	preempt_disable();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch)) {
		ret = -ENODEV;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (dsq_id == SCX_DSQ_LOCAL) {
		ret = READ_ONCE((scx_locked_rq() ?: this_rq())->scx.local_dsq.nr);
		ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON) {
		s32 cpu = scx_cpu_ret(sch, dsq_id & SCX_DSQ_LOCAL_CPU_ಮುಖವಾಡ);

		ಆದರೆ (scx_cpu_valid(sch, cpu, ಖಾಲಿ)) {
			ret = READ_ONCE(cpu_rq(cpu)->scx.local_dsq.nr);
			ಹೋಗು ಹೊರಹೋಗು;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		dsq = ಕಂಡುಹಿಡಿ_user_dsq(sch, dsq_id);
		ಆದರೆ (dsq) {
			ret = READ_ONCE(dsq->nr);
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}
	ret = -ENOENT;
ಹೊರಹೋಗು:
	preempt_enable();
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * scx_bpf_destroy_dsq - Destroy a custom DSQ
 * @dsq_id: DSQ to destroy
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Destroy the custom DSQ identified by @dsq_id. Only DSQs created with
 * scx_bpf_create_dsq() can be destroyed. The caller must ensure that the DSQ is
 * empty and no further tasks are dispatched to it. Ignored if called on a DSQ
 * which doesn't exist. Can be called from any online scx_ops operations.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_destroy_dsq(u64 dsq_id, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ಆದರೆ (sch)
		destroy_dsq(sch, dsq_id);
}

/**
 * bpf_iter_scx_dsq_new - Create a DSQ iterator
 * @it: iterator to initialize
 * @dsq_id: DSQ to iterate
 * @flags: %SCX_DSQ_ITER_*
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Initialize BPF iterator @it which can be used with bpf_for_each() to walk
 * tasks in the DSQ specified by @dsq_id. Iteration using @it only includes
 * tasks which are already queued when this function is invoked.
 */
__bpf_kfunc ಪೂರ್ಣಾಂಕ bpf_iter_scx_dsq_new(ರಚನೆ bpf_iter_scx_dsq *it, u64 dsq_id,
				     u64 flags, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ bpf_iter_scx_dsq_kern *kit = (ಶೂನ್ಯ *)it;
	ರಚನೆ scx_sched *sch;

	BUILD_BUG_ON(ಗಾತ್ರ(ರಚನೆ bpf_iter_scx_dsq_kern) >
		     ಗಾತ್ರ(ರಚನೆ bpf_iter_scx_dsq));
	BUILD_BUG_ON(__alignof__(ರಚನೆ bpf_iter_scx_dsq_kern) !=
		     __alignof__(ರಚನೆ bpf_iter_scx_dsq));
	BUILD_BUG_ON(__SCX_DSQ_ITER_ALL_FLAGS &
		     ((1U << __SCX_DSQ_LNODE_PRIV_SHIFT) - 1));

	/*
	 * next() and destroy() will be called regardless of the return value.
	 * Always clear $kit->dsq.
	 */
	kit->dsq = ಖಾಲಿ;

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ -ENODEV;

	ಆದರೆ (flags & ~__SCX_DSQ_ITER_USER_FLAGS)
		ಹಿಂತಿರುಗಿ -EINVAL;

	kit->dsq = ಕಂಡುಹಿಡಿ_user_dsq(sch, dsq_id);
	ಆದರೆ (!kit->dsq)
		ಹಿಂತಿರುಗಿ -ENOENT;

	kit->cursor = INIT_DSQ_LIST_CURSOR(kit->cursor, kit->dsq, flags);

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * bpf_iter_scx_dsq_next - Progress a DSQ iterator
 * @it: iterator to progress
 *
 * Return the next task. See bpf_iter_scx_dsq_new().
 */
__bpf_kfunc ರಚನೆ task_struct *bpf_iter_scx_dsq_ಮುಂದಿನ(ರಚನೆ bpf_iter_scx_dsq *it)
{
	ರಚನೆ bpf_iter_scx_dsq_kern *kit = (ಶೂನ್ಯ *)it;

	ಆದರೆ (!kit->dsq)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	guard(raw_spinlock_irqsave)(&kit->dsq->ಬೀಗ);

	ಹಿಂತಿರುಗಿ nldsq_cursor_ಮುಂದಿನ_task(&kit->cursor, kit->dsq);
}

/**
 * bpf_iter_scx_dsq_destroy - Destroy a DSQ iterator
 * @it: iterator to destroy
 *
 * Undo bpf_iter_scx_dsq_new().
 */
__bpf_kfunc ಶೂನ್ಯ bpf_iter_scx_dsq_destroy(ರಚನೆ bpf_iter_scx_dsq *it)
{
	ರಚನೆ bpf_iter_scx_dsq_kern *kit = (ಶೂನ್ಯ *)it;

	ಆದರೆ (!kit->dsq)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!list_ಖಾಲಿಗಳು(&kit->cursor.ಗಂಟು)) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

		raw_spin_ಬೀಗ_irqsave(&kit->dsq->ಬೀಗ, flags);
		list_del_init(&kit->cursor.ಗಂಟು);
		raw_spin_unlock_irqrestore(&kit->dsq->ಬೀಗ, flags);
	}
	kit->dsq = ಖಾಲಿ;
}

/**
 * scx_bpf_dsq_peek - Lockless peek at the first element.
 * @dsq_id: DSQ to examine.
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Read the first element in the DSQ. This is semantically equivalent to using
 * the DSQ iterator, but is lockfree. Of course, like any lockless operation,
 * this provides only a point-in-time snapshot, and the contents may change
 * by the time any subsequent locking operation reads the queue.
 *
 * Returns the pointer, or NULL indicates an empty queue OR internal error.
 */
__bpf_kfunc ರಚನೆ task_struct *scx_bpf_dsq_peek(u64 dsq_id,
						 ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_dispatch_q *dsq;

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (unlikely(dsq_id & SCX_DSQ_FLAG_BUILTIN)) {
		scx_error(sch, "peek disallowed on builtin DSQ 0x%llx", dsq_id);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	dsq = ಕಂಡುಹಿಡಿ_user_dsq(sch, dsq_id);
	ಆದರೆ (unlikely(!dsq)) {
		scx_error(sch, "peek on non-existent DSQ 0x%llx", dsq_id);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ rcu_dereference(dsq->ಮೊದಲ_task);
}

/**
 * scx_bpf_dsq_reenq - Re-enqueue tasks on a DSQ
 * @dsq_id: DSQ to re-enqueue
 * @reenq_flags: %SCX_RENQ_*
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Iterate over all of the tasks currently enqueued on the DSQ identified by
 * @dsq_id, and re-enqueue them in the BPF scheduler. The following DSQs are
 * supported:
 *
 * - Local DSQs (%SCX_DSQ_LOCAL or %SCX_DSQ_LOCAL_ON | $cpu)
 * - User DSQs
 *
 * Re-enqueues are performed asynchronously. Can be called from anywhere.
 *
 * %SCX_DSQ_LOCAL resolves to the local DSQ of the rq the current scheduler
 * operation is locked to - e.g. the rq being dispatched for in ops.dispatch() -
 * or the calling CPU's when no rq is locked.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_dsq_reenq(u64 dsq_id, u64 reenq_flags,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ rq *locked_rq = scx_locked_rq();
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_dispatch_q *dsq;

	guard(preempt)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (unlikely(reenq_flags & ~__SCX_REENQ_USER_MASK)) {
		scx_error(sch, "invalid SCX_REENQ flags 0x%llx", reenq_flags);
		ಹಿಂತಿರುಗಿ;
	}

	/* not specifying any filter bits is the same as %SCX_REENQ_ANY */
	ಆದರೆ (!(reenq_flags & __SCX_REENQ_FILTER_MASK))
		reenq_flags |= SCX_REENQ_ANY;

	dsq = ಕಂಡುಹಿಡಿ_dsq_for_dispatch(sch, locked_rq ?: this_rq(), dsq_id, smp_processor_id());
	ವೇಳಾಪಟ್ಟಿ_dsq_reenq(sch, dsq, reenq_flags, locked_rq);
}

/**
 * scx_bpf_reenqueue_local___v2 - Re-enqueue tasks on a local DSQ
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Iterate over all of the tasks currently enqueued on the local DSQ of the
 * caller's CPU, and re-enqueue them in the BPF scheduler. Can be called from
 * anywhere.
 *
 * This is now a special case of scx_bpf_dsq_reenq() and may be removed in the
 * future.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_reenqueue_local___v2(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	scx_bpf_dsq_reenq(SCX_DSQ_LOCAL, 0, aux);
}

__bpf_kfunc_end_defs();

__printf(5, 0)
ಸ್ಥಿರ s32 __bstr_format(ರಚನೆ scx_sched *sch, u64 *ದತ್ತಾಂಶ_buf, ಅಕ್ಷರ *ಸಾಲು_buf,
			 ಗಾತ್ರ_t ಸಾಲು_ಗಾತ್ರ, ಅಕ್ಷರ *fmt, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ *ದತ್ತಾಂಶ,
			 u32 ದತ್ತಾಂಶ_sz)
{
	ರಚನೆ bpf_bprintf_ದತ್ತಾಂಶ bprintf_ದತ್ತಾಂಶ = { .get_bin_ವಾದಗಳು = true };
	s32 ret;

	ಆದರೆ (ದತ್ತಾಂಶ_sz % 8 || ದತ್ತಾಂಶ_sz > MAX_BPRINTF_VARARGS * 8 ||
	    (ದತ್ತಾಂಶ_sz && !ದತ್ತಾಂಶ)) {
		scx_error(sch, "invalid data=%p and data__sz=%u", (ಶೂನ್ಯ *)ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_sz);
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ret = copy_from_kernel_nofault(ದತ್ತಾಂಶ_buf, ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_sz);
	ಆದರೆ (ret < 0) {
		scx_error(sch, "failed to read data fields (%d)", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = bpf_bprintf_prepare(fmt, UINT_MAX, ದತ್ತಾಂಶ_buf, ದತ್ತಾಂಶ_sz / 8,
				  &bprintf_ದತ್ತಾಂಶ);
	ಆದರೆ (ret < 0) {
		scx_error(sch, "format preparation failed (%d)", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = bstr_printf(ಸಾಲು_buf, ಸಾಲು_ಗಾತ್ರ, fmt,
			  bprintf_ದತ್ತಾಂಶ.bin_ವಾದಗಳು);
	bpf_bprintf_cleanup(&bprintf_ದತ್ತಾಂಶ);
	ಆದರೆ (ret < 0) {
		scx_error(sch, "(\"%s\", %p, %u) failed to format", fmt, ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_sz);
		ಹಿಂತಿರುಗಿ ret;
	}

	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Exit @sch with the reason formatted from a BPF-supplied bstr format. The exit
 * is claimed first and the reason is formatted directly into the winner-owned
 * exit_info buffer, which allows use from any context including NMI.
 *
 * @fmt_blame is the sched blamed for formatting failures through the
 * scx_error() calls in __bstr_format() and differs from @sch when a parent
 * supplies the kill reason for a child. A formatting failure doesn't revert the
 * claim - @sch still exits with the claimed kind and a fallback message.
 */
__printf(5, 0)
bool scx_exit_bstr(ರಚನೆ scx_sched *sch, ಎಣಿಕೆ scx_exit_ಬಗೆ ಬಗೆ,
		   s64 exit_ಸಂಕೇತ, ರಚನೆ scx_sched *fmt_blame, ಅಕ್ಷರ *fmt,
		   ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ *ದತ್ತಾಂಶ, u32 ದತ್ತಾಂಶ_sz)
{
	ರಚನೆ scx_exit_info *ei = sch->exit_info;
	u64 ದತ್ತಾಂಶ_buf[MAX_BPRINTF_VARARGS];
	s32 ret;

	guard(preempt)();

	ಆದರೆ (!scx_claim_exit(sch, ಬಗೆ))
		ಹಿಂತಿರುಗಿ false;

	ret = __bstr_format(fmt_blame, ದತ್ತಾಂಶ_buf, ei->ಸಂದೇಶ, SCX_EXIT_ಸಂದೇಶ_LEN,
			    fmt, ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_sz);
	ಆದರೆ (ret < 0)
		scnprintf(ei->ಸಂದೇಶ, SCX_EXIT_ಸಂದೇಶ_LEN,
			  "exit message formatting failed (%d)", ret);

	scx_ಮುಕ್ತಾಯ_exit(sch, ಬಗೆ, exit_ಸಂಕೇತ, raw_smp_processor_id());
	ಹಿಂತಿರುಗಿ true;
}

__bpf_kfunc_start_defs();

/**
 * scx_bpf_exit_bstr - Gracefully exit the BPF scheduler.
 * @exit_code: Exit value to pass to user space via struct scx_exit_info.
 * @fmt: error message format string
 * @data: format string parameters packaged using ___bpf_fill() macro
 * @data__sz: @data len, must end in '__sz' for the verifier
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Indicate that the BPF scheduler wants to exit gracefully, and initiate ops
 * disabling.
 */
__printf(2, 0)
__bpf_kfunc ಶೂನ್ಯ scx_bpf_exit_bstr(s64 exit_ಸಂಕೇತ, ಅಕ್ಷರ *fmt,
				   ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ *ದತ್ತಾಂಶ, u32 ದತ್ತಾಂಶ_sz,
				   ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (likely(sch))
		scx_exit_bstr(sch, SCX_EXIT_UNREG_BPF, exit_ಸಂಕೇತ, sch, fmt,
			      ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_sz);
}

/**
 * scx_bpf_error_bstr - Indicate fatal error
 * @fmt: error message format string
 * @data: format string parameters packaged using ___bpf_fill() macro
 * @data__sz: @data len, must end in '__sz' for the verifier
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Indicate that the BPF scheduler encountered a fatal error and initiate ops
 * disabling.
 */
__printf(1, 0)
__bpf_kfunc ಶೂನ್ಯ scx_bpf_error_bstr(ಅಕ್ಷರ *fmt, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ *ದತ್ತಾಂಶ,
				    u32 ದತ್ತಾಂಶ_sz, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (likely(sch))
		scx_exit_bstr(sch, SCX_EXIT_ERROR_BPF, 0, sch, fmt, ದತ್ತಾಂಶ,
			      ದತ್ತಾಂಶ_sz);
}

/**
 * scx_bpf_dump_bstr - Generate extra debug dump specific to the BPF scheduler
 * @fmt: format string
 * @data: format string parameters packaged using ___bpf_fill() macro
 * @data__sz: @data len, must end in '__sz' for the verifier
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * To be called through scx_bpf_dump() helper from ops.dump(), dump_cpu() and
 * dump_task() to generate extra debug dump specific to the BPF scheduler.
 *
 * The extra dump may be multiple lines. A single line may be split over
 * multiple calls. The last line is automatically terminated.
 */
__printf(1, 0)
__bpf_kfunc ಶೂನ್ಯ scx_bpf_dump_bstr(ಅಕ್ಷರ *fmt, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ *ದತ್ತಾಂಶ,
				   u32 ದತ್ತಾಂಶ_sz, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_dump_ದತ್ತಾಂಶ *dd = &scx_dump_ದತ್ತಾಂಶ;
	ರಚನೆ scx_bstr_buf *buf = &dd->buf;
	s32 ret;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (raw_smp_processor_id() != dd->cpu) {
		scx_error(sch, "scx_bpf_dump() must only be called from ops.dump() and friends");
		ಹಿಂತಿರುಗಿ;
	}

	/* append the formatted string to the line buf */
	ret = __bstr_format(sch, buf->ದತ್ತಾಂಶ, buf->ಸಾಲು + dd->cursor,
			    ಗಾತ್ರ(buf->ಸಾಲು) - dd->cursor, fmt, ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_sz);
	ಆದರೆ (ret < 0) {
		scx_dump_ಸಾಲು(dd->s, "%s[!] (\"%s\", %p, %u) failed to format (%d)",
			      dd->ಪೂರ್ವಪ್ರತ್ಯಯ, fmt, ದತ್ತಾಂಶ, ದತ್ತಾಂಶ_sz, ret);
		ಹಿಂತಿರುಗಿ;
	}

	dd->cursor += ret;
	dd->cursor = min_t(s32, dd->cursor, ಗಾತ್ರ(buf->ಸಾಲು));

	ಆದರೆ (!dd->cursor)
		ಹಿಂತಿರುಗಿ;

	/*
	 * If the line buf overflowed or ends in a newline, flush it into the
	 * dump. This is to allow the caller to generate a single line over
	 * multiple calls. As ops_dump_flush() can also handle multiple lines in
	 * the line buf, the only case which can lead to an unexpected
	 * truncation is when the caller keeps generating newlines in the middle
	 * instead of the end consecutively. Don't do that.
	 */
	ಆದರೆ (dd->cursor >= ಗಾತ್ರ(buf->ಸಾಲು) || buf->ಸಾಲು[dd->cursor - 1] == '\n')
		ops_dump_flush();
}

/**
 * scx_bpf_cpuperf_cap - Query the maximum relative capacity of a CPU
 * @cpu: CPU of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Return the maximum relative capacity of @cpu in relation to the most
 * performant CPU in the system. The return value is in the range [1,
 * %SCX_CPUPERF_ONE]. See scx_bpf_cpuperf_cur().
 */
__bpf_kfunc u32 scx_bpf_cpuperf_cap(s32 cpu, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (likely(sch) && scx_cpu_valid(sch, cpu, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ SCX_CPUPERF_ONE;
}

/**
 * scx_bpf_cidperf_cap - Query the maximum relative capacity of the CPU at @cid
 * @cid: cid of the CPU to query
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpuperf_cap().
 */
__bpf_kfunc u32 scx_bpf_cidperf_cap(s32 cid, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ SCX_CPUPERF_ONE;
	cpu = scx_cid_ಗಮ್ಯ_cpu(sch, cid);
	ಆದರೆ (cpu < 0)
		ಹಿಂತಿರುಗಿ SCX_CPUPERF_ONE;
	ಹಿಂತಿರುಗಿ arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);
}

/**
 * scx_bpf_cpuperf_cur - Query the current relative performance of a CPU
 * @cpu: CPU of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Return the current relative performance of @cpu in relation to its maximum.
 * The return value is in the range [1, %SCX_CPUPERF_ONE].
 *
 * The current performance level of a CPU in relation to the maximum performance
 * available in the system can be calculated as follows:
 *
 *   scx_bpf_cpuperf_cap() * scx_bpf_cpuperf_cur() / %SCX_CPUPERF_ONE
 *
 * The result is in the range [1, %SCX_CPUPERF_ONE].
 */
__bpf_kfunc u32 scx_bpf_cpuperf_cur(s32 cpu, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (likely(sch) && scx_cpu_valid(sch, cpu, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ arch_scale_freq_ಸಾಮರ್ಥ್ಯ(cpu);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ SCX_CPUPERF_ONE;
}

/**
 * scx_bpf_cidperf_cur - Query the current performance of the CPU at @cid
 * @cid: cid of the CPU to query
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpuperf_cur().
 */
__bpf_kfunc u32 scx_bpf_cidperf_cur(s32 cid, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ SCX_CPUPERF_ONE;
	cpu = scx_cid_ಗಮ್ಯ_cpu(sch, cid);
	ಆದರೆ (cpu < 0)
		ಹಿಂತಿರುಗಿ SCX_CPUPERF_ONE;
	ಹಿಂತಿರುಗಿ arch_scale_freq_ಸಾಮರ್ಥ್ಯ(cpu);
}

/* validate and apply a cpuperf target, see scx_bpf_cpuperf_set() */
ಸ್ಥಿರ s32 scx_cpuperf_ಗಣ(ರಚನೆ scx_sched *sch, s32 cpu, u32 perf)
{
	ರಚನೆ rq *rq, *locked_rq;
	ರಚನೆ rq_flags rf;
	s32 ret;

	ಆದರೆ (unlikely(perf > SCX_CPUPERF_ONE)) {
		scx_error(sch, "Invalid cpuperf target %u for CPU %d", perf, cpu);
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (!scx_cpu_valid(sch, cpu, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ -EINVAL;

	rq = cpu_rq(cpu);
	locked_rq = scx_locked_rq();

	/*
	 * When called with an rq lock held, restrict the operation to the
	 * corresponding CPU to prevent ABBA deadlocks.
	 */
	ಆದರೆ (locked_rq && rq != locked_rq) {
		scx_error(sch, "Invalid target CPU %d", cpu);
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	/*
	 * If no rq lock is held, allow to operate on any CPU by acquiring
	 * the corresponding rq lock.
	 */
	ಆದರೆ (!locked_rq) {
		rq_ಬೀಗ_irqsave(rq, &rf);
		update_rq_clock(rq);
	}

	/*
	 * ecaps updates are folded under the rq lock, making this test
	 * authoritative: a write can never land after a revoke has taken
	 * effect on @cpu.
	 */
	ಆದರೆ (likely(!scx_missing_caps(sch, cpu, SCX_CAP_PERF))) {
		rq->scx.cpuperf_ಗುರಿ = perf;
		cpufreq_update_util(rq, 0);
		ret = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		__scx_add_event(sch, SCX_EV_SUB_CIDPERF_DENIED, 1);
		ret = -EACCES;
	}

	ಆದರೆ (!locked_rq)
		rq_unlock_irqrestore(rq, &rf);

	ಹಿಂತಿರುಗಿ ret;
}

/**
 * scx_bpf_cpuperf_set - Set the relative performance target of a CPU
 * @cpu: CPU of interest
 * @perf: target performance level [0, %SCX_CPUPERF_ONE]
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Set the target performance level of @cpu to @perf. @perf is in linear
 * relative scale between 0 and %SCX_CPUPERF_ONE. This determines how the
 * schedutil cpufreq governor chooses the target frequency.
 *
 * The actual performance level chosen, CPU grouping, and the overhead and
 * latency of the operations are dependent on the hardware and cpufreq driver in
 * use. Consult hardware and cpufreq documentation for more information. The
 * current performance level can be monitored using scx_bpf_cpuperf_cur().
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_cpuperf_ಗಣ(s32 cpu, u32 perf, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;

	scx_cpuperf_ಗಣ(sch, cpu, perf);
}

/**
 * scx_bpf_cidperf_set - Set the performance target of the CPU at @cid
 * @cid: cid of the CPU to target
 * @perf: target performance level [0, %SCX_CPUPERF_ONE]
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpuperf_set(). A sub-sched needs
 * SCX_CAP_PERF on @cid. Returns 0 if the target was applied, -%EACCES if
 * the write was denied for missing caps, other -errnos if @cid didn't
 * resolve.
 */
__bpf_kfunc s32 scx_bpf_cidperf_ಗಣ(s32 cid, u32 perf,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ -ENODEV;
	cpu = scx_cid_ಗಮ್ಯ_cpu(sch, cid);
	ಆದರೆ (cpu < 0)
		ಹಿಂತಿರುಗಿ cpu;

	ಹಿಂತಿರುಗಿ scx_cpuperf_ಗಣ(sch, cpu, perf);
}

/**
 * scx_bpf_nr_node_ids - Return the number of possible node IDs
 *
 * All valid node IDs in the system are smaller than the returned value.
 */
__bpf_kfunc u32 scx_bpf_nr_ಗಂಟು_ids(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ nr_ಗಂಟು_ids;
}

/**
 * scx_bpf_nr_cpu_ids - Return the number of possible CPU IDs
 *
 * All valid CPU IDs in the system are smaller than the returned value.
 */
__bpf_kfunc u32 scx_bpf_nr_cpu_ids(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ nr_cpu_ids;
}

/**
 * scx_bpf_nr_cids - Return the size of the cid space
 *
 * Equals num_possible_cpus(). All valid cids are in [0, return value).
 */
__bpf_kfunc u32 scx_bpf_nr_cids(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ ಅಂಕೆ_possible_cpus();
}

/**
 * scx_bpf_nr_online_cids - Return current count of online CPUs in cid space
 *
 * Return num_online_cpus(). The standard model restarts the scheduler on
 * hotplug, which lets schedulers treat [0, nr_online_cids) as the online
 * range. Schedulers that prefer to handle hotplug without a restart should
 * install a custom mapping via scx_bpf_cid_override() and track onlining
 * through the ops.cid_online / ops.cid_offline callbacks, starting from the
 * mask scx_bpf_online_cmask() returns.
 */
__bpf_kfunc u32 scx_bpf_nr_online_cids(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ ಅಂಕೆ_online_cpus();
}

/**
 * scx_bpf_online_cmask - Return the online cid mask in the scheduler arena
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Return a kernel-maintained cmask covering [0, scx_bpf_nr_cids()), or NULL if
 * the calling program is not associated with a live cid-form scheduler or the
 * mask is not allocated yet, as in ops.init_cids(). Treat the mask as read-only
 * even though arena memory stays writable by the BPF scheduler. The mask
 * follows the SCX hotplug notifications: a cid's bit is updated before
 * ops.cid_online/offline() runs for it. The pointer is valid from ops.init()
 * through ops.exit(). Root ops.init() runs with hotplug excluded. Other
 * contexts can observe concurrent updates.
 */
__bpf_kfunc ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *scx_bpf_online_cmask(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_cmask *online;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	online = sch->online_cmask;
	ಆದರೆ (unlikely(!online))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *)scx_kaddr_ಗಮ್ಯ_arena(sch, online);
}

/**
 * scx_bpf_this_cid - Return the cid of the CPU this program is running on
 *
 * cid-addressed equivalent of bpf_get_smp_processor_id() for scx programs.
 * The current cpu is trivially valid, so this is just a table lookup. Return
 * -EINVAL if called before any scheduler has ever published its cid tables.
 */
__bpf_kfunc s32 scx_bpf_this_cid(ಶೂನ್ಯ)
{
	s16 *tbl;

	guard(rcu)();

	tbl = rcu_dereference(scx_cpu_ಗಮ್ಯ_cid_tbl);
	ಆದರೆ (!tbl)
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಹಿಂತಿರುಗಿ tbl[raw_smp_processor_id()];
}

/**
 * scx_bpf_get_possible_cpumask - Get a referenced kptr to cpu_possible_mask
 */
__bpf_kfunc ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *scx_bpf_get_possible_cpumask(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ cpu_possible_ಮುಖವಾಡ;
}

/**
 * scx_bpf_get_online_cpumask - Get a referenced kptr to cpu_online_mask
 */
__bpf_kfunc ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *scx_bpf_get_online_cpumask(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ cpu_online_ಮುಖವಾಡ;
}

/**
 * scx_bpf_put_cpumask - Release a possible/online cpumask
 * @cpumask: cpumask to release
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_put_cpumask(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpumask)
{
	/*
	 * Empty function body because we aren't actually acquiring or releasing
	 * a reference to a global cpumask, which is read-only in the caller and
	 * is never released. The acquire / release semantics here are just used
	 * to make the cpumask is a trusted pointer in the caller.
	 */
}

/**
 * scx_bpf_task_running - Is task currently running?
 * @p: task of interest
 */
__bpf_kfunc bool scx_bpf_task_running(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ task_rq(p)->curr == p;
}

/**
 * scx_bpf_task_cpu - CPU a task is currently associated with
 * @p: task of interest
 */
__bpf_kfunc s32 scx_bpf_task_cpu(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ task_cpu(p);
}

/**
 * scx_bpf_task_cid - cid a task is currently associated with
 * @p: task of interest
 *
 * cid-addressed equivalent of scx_bpf_task_cpu(). task_cpu(p) is always a
 * valid cpu, so this is just a table lookup. Return -EINVAL if called before
 * any scheduler has ever published its cid tables.
 */
__bpf_kfunc s32 scx_bpf_task_cid(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	s16 *tbl;

	/* KF_RCU covers only @p - a sleepable program holds no RCU lock */
	guard(rcu)();

	tbl = rcu_dereference(scx_cpu_ಗಮ್ಯ_cid_tbl);
	ಆದರೆ (!tbl)
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಹಿಂತಿರುಗಿ tbl[task_cpu(p)];
}

/**
 * scx_bpf_locked_rq - Return the rq currently locked by SCX
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns the rq if a rq lock is currently held by SCX.
 * Otherwise emits an error and returns NULL.
 */
__bpf_kfunc ರಚನೆ rq *scx_bpf_locked_rq(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ rq *rq;

	guard(preempt)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	rq = scx_locked_rq();
	ಆದರೆ (!rq) {
		scx_error(sch, "accessing rq without holding rq lock");
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ rq;
}

/**
 * scx_bpf_cpu_curr - Return remote CPU's curr task
 * @cpu: CPU of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Callers must hold RCU read lock (KF_RCU).
 */
__bpf_kfunc ರಚನೆ task_struct *scx_bpf_cpu_curr(s32 cpu, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (!scx_cpu_valid(sch, cpu, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ rcu_dereference(cpu_rq(cpu)->curr);
}

/**
 * scx_bpf_cid_curr - Return the curr task on the CPU at @cid
 * @cid: cid of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpu_curr(). Callers must hold RCU
 * read lock (KF_RCU).
 */
__bpf_kfunc ರಚನೆ task_struct *scx_bpf_cid_curr(s32 cid, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	cpu = scx_cid_ಗಮ್ಯ_cpu(sch, cid);
	ಆದರೆ (cpu < 0)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ rcu_dereference(cpu_rq(cpu)->curr);
}

/**
 * scx_bpf_tid_to_task - Look up a task by its scx tid
 * @tid: task ID previously read from p->scx.tid
 *
 * Returns the task with the given tid, or NULL if no such task exists. The
 * returned pointer is valid until the end of the current RCU read section
 * (KF_RCU_PROTECTED). Requires SCX_OPS_TID_TO_TASK to be set on the root
 * scheduler; otherwise an error is raised and NULL returned.
 */
__bpf_kfunc ರಚನೆ task_struct *scx_bpf_tid_ಗಮ್ಯ_task(u64 tid)
{
	ರಚನೆ sched_ext_entity *scx;

	ಆದರೆ (!scx_tid_ಗಮ್ಯ_task_enabled()) {
		ರಚನೆ scx_sched *sch = rcu_dereference(scx_ಬೇರು);

		ಆದರೆ (sch)
			scx_error(sch, "scx_bpf_tid_to_task() called without SCX_OPS_TID_TO_TASK");
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	scx = rhashtable_lookup(&scx_tid_hash, &tid, scx_tid_hash_params);
	ಆದರೆ (!scx)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ container_ದ(scx, ರಚನೆ task_struct, scx);
}

u64 __scx_bpf_now(ರಚನೆ rq *rq)
{
	/* the caller must be on @rq's cpu or hold its lock */
	lockdep_assert((rq == this_rq() && !preemptible()) ||
		       lockdep_is_held(__rq_lockp(rq)));

	ಆದರೆ (smp_ತುಂಬು_acquire(&rq->scx.flags) & SCX_RQ_CLK_VALID) {
		/* if the rq clock is valid, use the cached rq clock */
		ಹಿಂತಿರುಗಿ READ_ONCE(rq->scx.clock);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Otherwise, return a fresh rq clock.
		 *
		 * The rq clock is updated outside of the rq lock.
		 * In this case, keep the updated rq clock invalid so the next
		 * read outside the rq lock gets a fresh rq clock.
		 */
		ಹಿಂತಿರುಗಿ sched_clock_cpu(cpu_ದ(rq));
	}
}

/**
 * scx_bpf_now - Returns a high-performance monotonically non-decreasing
 * clock for the current CPU. The clock returned is in nanoseconds.
 *
 * It provides the following properties:
 *
 * 1) High performance: Many BPF schedulers call bpf_ktime_get_ns() frequently
 *  to account for execution time and track tasks' runtime properties.
 *  Unfortunately, in some hardware platforms, bpf_ktime_get_ns() -- which
 *  eventually reads a hardware timestamp counter -- is neither performant nor
 *  scalable. scx_bpf_now() aims to provide a high-performance clock by
 *  using the rq clock in the scheduler core whenever possible.
 *
 * 2) High enough resolution for the BPF scheduler use cases: In most BPF
 *  scheduler use cases, the required clock resolution is lower than the most
 *  accurate hardware clock (e.g., rdtsc in x86). scx_bpf_now() basically
 *  uses the rq clock in the scheduler core whenever it is valid. It considers
 *  that the rq clock is valid from the time the rq clock is updated
 *  (update_rq_clock) until the rq is unlocked (rq_unpin_lock).
 *
 * 3) Monotonically non-decreasing clock for the same CPU: scx_bpf_now()
 *  guarantees the clock never goes backward when comparing them in the same
 *  CPU. On the other hand, when comparing clocks in different CPUs, there
 *  is no such guarantee -- the clock can go backward. It provides a
 *  monotonically *non-decreasing* clock so that it would provide the same
 *  clock values in two different scx_bpf_now() calls in the same CPU
 *  during the same period of when the rq clock is valid.
 */
__bpf_kfunc u64 scx_bpf_now(ಶೂನ್ಯ)
{
	/*
	 * Note that scx_bpf_now() is re-entrant between a process context and
	 * an interrupt context (e.g., timer interrupt). However, we don't need
	 * to consider the race between them because such race is not observable
	 * from a caller.
	 */
	guard(preempt)();
	ಹಿಂತಿರುಗಿ __scx_bpf_now(this_rq());
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_read_events(ರಚನೆ scx_sched *sch, ರಚನೆ scx_event_stats *events)
{
	ಪೂರ್ಣಾಂಕ cpu;

	/* Aggregate per-CPU event counters into @events. */
	memset(events, 0, ಗಾತ್ರ(*events));
	for_each_possible_cpu(cpu) {
		ರಚನೆ scx_event_stats *e_cpu = &ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu)->event_stats;
#ವ್ಯಾಖ್ಯಾನಿಸು SCX_EVENT(name)	(events->name += READ_ONCE(e_cpu->name))
		SCX_EVENTS_LIST(SCX_EVENT);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCX_EVENT
	}
}

/**
 * scx_bpf_events - Read the event counters of the calling scheduler
 * @events: output buffer from a BPF program
 * @events__sz: @events len, must end in '__sz' for the verifier
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Read the event counters of the scheduler associated with the calling program.
 * @events is zeroed when no scheduler can be resolved.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_events(ರಚನೆ scx_event_stats *events, ಗಾತ್ರ_t events__sz,
				ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;
	ರಚನೆ scx_event_stats e_sys;

	rcu_read_ಬೀಗ();
	sch = scx_prog_sched(aux);
	ಆದರೆ (sch)
		scx_read_events(sch, &e_sys);
	ಇಲ್ಲದಿದ್ದರೆ
		memset(&e_sys, 0, ಗಾತ್ರ(e_sys));
	rcu_read_unlock();

	/*
	 * We cannot entirely trust a BPF-provided size since a BPF program
	 * might be compiled against a different vmlinux.h, of which
	 * scx_event_stats would be larger (a newer vmlinux.h) or smaller
	 * (an older vmlinux.h). Hence, we use the smaller size to avoid
	 * memory corruption.
	 */
	events__sz = min(events__sz, ಗಾತ್ರ(*events));
	ಸ್ಮರಣೆ_ನಕಲು(events, &e_sys, events__sz);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
/**
 * scx_bpf_task_cgroup - Return the sched cgroup of a task
 * @p: task of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * @p->sched_task_group->css.cgroup represents the cgroup @p is associated with
 * from the scheduler's POV. SCX operations should use this function to
 * determine @p's current cgroup as, unlike following @p->cgroups,
 * @p->sched_task_group is stable for the duration of the SCX op. See
 * SCX_CALL_OP_TASK() for details.
 */
__bpf_kfunc ರಚನೆ cgroup *scx_bpf_task_cgroup(ರಚನೆ task_struct *p,
					       ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ task_ಗುಂಪು *tg = p->sched_task_ಗುಂಪು;
	ರಚನೆ cgroup *cgrp = &cgrp_dfl_ಬೇರು.cgrp;
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!scx_kf_arg_task_ok(sch, p))
		ಹೋಗು ಹೊರಹೋಗು;

	cgrp = tg_cgrp(tg);

ಹೊರಹೋಗು:
	cgroup_get(cgrp);
	ಹಿಂತಿರುಗಿ cgrp;
}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_CGROUP_SCHED */

__bpf_kfunc_end_defs();

BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_any)
BTF_ID_FLAGS(func, scx_bpf_task_ಗಣ_ತುಂಡು, KF_IMPLICIT_ವಾದಗಳು | KF_RCU);
BTF_ID_FLAGS(func, scx_bpf_task_ಗಣ_dsq_vtime, KF_IMPLICIT_ವಾದಗಳು | KF_RCU);
BTF_ID_FLAGS(func, scx_bpf_kick_cpu, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_kick_cid, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_dsq_nr_queued, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_destroy_dsq, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_dsq_peek, KF_IMPLICIT_ವಾದಗಳು | KF_RCU_PROTECTED | KF_RET_NULL)
BTF_ID_FLAGS(func, scx_bpf_dsq_reenq, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_reenqueue_local___v2, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_new, KF_IMPLICIT_ವಾದಗಳು | KF_ITER_NEW | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_ಮುಂದಿನ, KF_ITER_ಮುಂದಿನ | KF_RET_NULL)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_destroy, KF_ITER_DESTROY)
BTF_ID_FLAGS(func, scx_bpf_exit_bstr, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_error_bstr, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_dump_bstr, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cap, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cur, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_ಗಣ, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cidperf_cap, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cidperf_cur, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cidperf_ಗಣ, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_nr_ಗಂಟು_ids)
BTF_ID_FLAGS(func, scx_bpf_nr_cpu_ids)
BTF_ID_FLAGS(func, scx_bpf_nr_cids)
BTF_ID_FLAGS(func, scx_bpf_nr_online_cids)
BTF_ID_FLAGS(func, scx_bpf_online_cmask, KF_IMPLICIT_ವಾದಗಳು | KF_ARENA_RET)
BTF_ID_FLAGS(func, scx_bpf_this_cid)
BTF_ID_FLAGS(func, scx_bpf_get_possible_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_online_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_task_running, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_task_cpu, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_task_cid, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_locked_rq, KF_IMPLICIT_ವಾದಗಳು | KF_RET_NULL)
BTF_ID_FLAGS(func, scx_bpf_cpu_curr, KF_IMPLICIT_ವಾದಗಳು | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_cid_curr, KF_IMPLICIT_ವಾದಗಳು | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_tid_ಗಮ್ಯ_task, KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_now)
BTF_ID_FLAGS(func, scx_bpf_events, KF_IMPLICIT_ವಾದಗಳು)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
BTF_ID_FLAGS(func, scx_bpf_task_cgroup, KF_IMPLICIT_ವಾದಗಳು | KF_RCU | KF_ACQUIRE)
#ಆದರೆ_ಅಂತ್ಯ
BTF_ID_FLAGS(func, scx_bpf_sub_grant, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_sub_revoke, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_sub_caps, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_sub_kill_bstr, KF_IMPLICIT_ವಾದಗಳು)
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_any)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_kfunc_id_ಗಣ scx_kfunc_ಗಣ_any = {
	.owner			= THIS_MODULE,
	.ಗಣ			= &scx_kfunc_ids_any,
	.filter			= scx_kfunc_context_filter,
};

/*
 * cpu-form kfuncs that are forbidden from cid-form schedulers
 * (bpf_sched_ext_ops_cid). Programs targeting the cid struct_ops type must
 * use the cid-form alternative (cid/cmask kfuncs).
 *
 * Membership overlaps with scx_kfunc_ids_{any,idle,select_cpu}; the filter
 * tests this set independently and rejects matches before the per-op
 * allow-list check runs.
 *
 * pahole/resolve_btfids scans every BTF_ID_FLAGS() at build time and
 * intersects flags across duplicate entries, so each entry must carry the
 * same flags as the kfunc's primary declaration; otherwise the flags get
 * dropped globally.
 */
BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_cpu_only)
BTF_ID_FLAGS(func, scx_bpf_kick_cpu, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_task_cpu, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_cpu_curr, KF_IMPLICIT_ವಾದಗಳು | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_cpu_ಗಂಟು, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cap, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cur, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_ಗಣ, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_get_possible_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_online_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask, KF_IMPLICIT_ವಾದಗಳು | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask_ಗಂಟು, KF_IMPLICIT_ವಾದಗಳು | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask, KF_IMPLICIT_ವಾದಗಳು | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask_ಗಂಟು, KF_IMPLICIT_ವಾದಗಳು | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_idle_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_test_and_clear_cpu_idle, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu_ಗಂಟು, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu_ಗಂಟು, KF_IMPLICIT_ವಾದಗಳು | KF_RCU)
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_cpu_only)

/*
 * Per-op kfunc allow flags. Each bit corresponds to a context-sensitive kfunc
 * group; an op may permit zero or more groups, with the union expressed in
 * scx_kf_allow_flags[]. The verifier-time filter (scx_kfunc_context_filter())
 * consults this table to decide whether a context-sensitive kfunc is callable
 * from a given SCX op.
 */
ಎಣಿಕೆ scx_kf_allow_flags {
	SCX_KF_ALLOW_UNLOCKED		= 1 << 0,
	SCX_KF_ALLOW_INIT_CIDS		= 1 << 1,
	SCX_KF_ALLOW_CPU_RELEASE	= 1 << 2,
	SCX_KF_ALLOW_DISPATCH		= 1 << 3,
	SCX_KF_ALLOW_ಸರತಿಗೆ_ಸೇರಿಸು		= 1 << 4,
	SCX_KF_ALLOW_SELECT_CPU		= 1 << 5,
};

/*
 * Map each SCX op to the union of kfunc groups it permits, indexed by
 * SCX_OP_IDX(op). Ops not listed only permit kfuncs that are not
 * context-sensitive.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u32 scx_kf_allow_flags[] = {
	[SCX_OP_IDX(select_cpu)]	= SCX_KF_ALLOW_SELECT_CPU | SCX_KF_ALLOW_ಸರತಿಗೆ_ಸೇರಿಸು,
	[SCX_OP_IDX(ಸರತಿಗೆ_ಸೇರಿಸು)]		= SCX_KF_ALLOW_SELECT_CPU | SCX_KF_ALLOW_ಸರತಿಗೆ_ಸೇರಿಸು,
	[SCX_OP_IDX(dispatch)]		= SCX_KF_ALLOW_ಸರತಿಗೆ_ಸೇರಿಸು | SCX_KF_ALLOW_DISPATCH,
	[SCX_OP_IDX(cpu_release)]	= SCX_KF_ALLOW_CPU_RELEASE,
	[SCX_OP_IDX(init_task)]		= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(dump)]		= SCX_KF_ALLOW_UNLOCKED,
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	[SCX_OP_IDX(cgroup_init)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_exit)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_prep_move)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_cancel_move)] = SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_ಗಣ_ತೂಕ)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_ಗಣ_bandwidth)] = SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_ಗಣ_idle)]	= SCX_KF_ALLOW_UNLOCKED,
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_GROUP_SCHED */
	[SCX_OP_IDX(sub_attach)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(sub_detach)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(sub_ecaps_updated)]	= SCX_KF_ALLOW_ಸರತಿಗೆ_ಸೇರಿಸು | SCX_KF_ALLOW_DISPATCH,
	[SCX_OP_IDX(cpu_online)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cpu_offline)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(init_cids)]		= SCX_KF_ALLOW_UNLOCKED | SCX_KF_ALLOW_INIT_CIDS,
	[SCX_OP_IDX(init)]		= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(ನಿರ್ಗಮಿಸು)]		= SCX_KF_ALLOW_UNLOCKED,
};

/*
 * Verifier-time filter for SCX kfuncs. Registered via the .filter field on
 * each per-group btf_kfunc_id_set. The BPF core invokes this for every kfunc
 * call in the registered hook (BPF_PROG_TYPE_STRUCT_OPS or
 * BPF_PROG_TYPE_SYSCALL), regardless of which set originally introduced the
 * kfunc - so the filter must short-circuit on kfuncs it doesn't govern by
 * falling through to "allow" when none of the SCX sets contain the kfunc.
 */
ಪೂರ್ಣಾಂಕ scx_kfunc_context_filter(ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog *prog, u32 kfunc_id)
{
	bool ಒಳ_unlocked = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_unlocked, kfunc_id);
	bool ಒಳ_init_cids = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_init_cids, kfunc_id);
	bool ಒಳ_select_cpu = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_select_cpu, kfunc_id);
	bool ಒಳ_ಸರತಿಗೆ_ಸೇರಿಸು = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_ಸರತಿಗೆ_ಸೇರಿಸು_dispatch, kfunc_id);
	bool ಒಳ_dispatch = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_dispatch, kfunc_id);
	bool ಒಳ_cpu_release = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_cpu_release, kfunc_id);
	bool ಒಳ_idle = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_idle, kfunc_id);
	bool ಒಳ_any = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_any, kfunc_id);
	bool ಒಳ_cpu_only = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_cpu_only, kfunc_id);
	bool ಒಳ_cid = btf_id_set8_ಒಳಗೊಂಡಿದೆ(&scx_kfunc_ids_cid, kfunc_id);
	u32 moff, flags;

	/* Not an SCX kfunc - allow. */
	ಆದರೆ (!(ಒಳ_unlocked || ಒಳ_init_cids || ಒಳ_select_cpu || ಒಳ_ಸರತಿಗೆ_ಸೇರಿಸು || ಒಳ_dispatch ||
	      ಒಳ_cpu_release || ಒಳ_idle || ಒಳ_any || ಒಳ_cid))
		ಹಿಂತಿರುಗಿ 0;

	/* SYSCALL progs (e.g. BPF test_run()) may call unlocked and select_cpu kfuncs. */
	ಆದರೆ (prog->type == BPF_PROG_TYPE_SYSCALL)
		ಹಿಂತಿರುಗಿ (ಒಳ_unlocked || ಒಳ_select_cpu || ಒಳ_idle || ಒಳ_any || ಒಳ_cid) ? 0 : -EACCES;

	ಆದರೆ (prog->type != BPF_PROG_TYPE_STRUCT_OPS)
		ಹಿಂತಿರುಗಿ (ಒಳ_any || ಒಳ_idle || ಒಳ_cid) ? 0 : -EACCES;

	/*
	 * add_subprog_and_kfunc() collects all kfunc calls, including dead code
	 * guarded by bpf_ksym_exists(), before check_attach_btf_id() sets
	 * prog->aux->st_ops. Allow all kfuncs when st_ops is not yet set;
	 * do_check_main() re-runs the filter with st_ops set and enforces the
	 * actual restrictions.
	 */
	ಆದರೆ (!prog->aux->st_ops)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * Non-SCX struct_ops: SCX kfuncs are not permitted.
	 *
	 * Both bpf_sched_ext_ops (cpu-form) and bpf_sched_ext_ops_cid
	 * (cid-form) are valid SCX struct_ops. Member offsets match between
	 * the two (verified by BUILD_BUG_ON in scx_init()), so the shared
	 * scx_kf_allow_flags[] table indexed by SCX_MOFF_IDX(moff) applies to
	 * both.
	 */
	ಆದರೆ (prog->aux->st_ops != &bpf_sched_ext_ops &&
	    prog->aux->st_ops != &bpf_sched_ext_ops_cid)
		ಹಿಂತಿರುಗಿ -EACCES;

	/*
	 * cid-form schedulers must use cid/cmask kfuncs. cid and cpu are both
	 * small s32s and trivially confused, so cpu-only kfuncs are rejected at
	 * load time. The reverse (cpu-form calling cid-form kfuncs) is
	 * intentionally permissive to ease gradual cpumask -> cid migration.
	 */
	ಆದರೆ (prog->aux->st_ops == &bpf_sched_ext_ops_cid && ಒಳ_cpu_only)
		ಹಿಂತಿರುಗಿ -EACCES;

	/* SCX struct_ops: check the per-op allow list. */
	ಆದರೆ (ಒಳ_any || ಒಳ_idle || ಒಳ_cid)
		ಹಿಂತಿರುಗಿ 0;

	moff = prog->aux->attach_st_ops_member_off;
	flags = scx_kf_allow_flags[SCX_MOFF_IDX(moff)];

	ಆದರೆ ((flags & SCX_KF_ALLOW_UNLOCKED) && ಒಳ_unlocked)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((flags & SCX_KF_ALLOW_INIT_CIDS) && ಒಳ_init_cids)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((flags & SCX_KF_ALLOW_CPU_RELEASE) && ಒಳ_cpu_release)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((flags & SCX_KF_ALLOW_DISPATCH) && ಒಳ_dispatch)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((flags & SCX_KF_ALLOW_ಸರತಿಗೆ_ಸೇರಿಸು) && ಒಳ_ಸರತಿಗೆ_ಸೇರಿಸು)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ ((flags & SCX_KF_ALLOW_SELECT_CPU) && ಒಳ_select_cpu)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ -EACCES;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init scx_init(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ ret;

	/*
	 * sched_ext_ops_cid mirrors sched_ext_ops up to and including @priv.
	 * Both bpf_scx_init_member() and bpf_scx_check_member() use offsets
	 * from struct sched_ext_ops; sched_ext_ops_cid relies on those offsets
	 * matching for the shared fields. Catch any drift at boot.
	 */
#ವ್ಯಾಖ್ಯಾನಿಸು CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cpu_ಕ್ಷೇತ್ರ, cid_ಕ್ಷೇತ್ರ)					\
	BUILD_BUG_ON(offsetof(ರಚನೆ sched_ext_ops, cpu_ಕ್ಷೇತ್ರ) !=		\
		     offsetof(ರಚನೆ sched_ext_ops_cid, cid_ಕ್ಷೇತ್ರ))
	/* data fields used by bpf_scx_init_member() */
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(dispatch_max_batch, dispatch_max_batch);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(flags, flags);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(name, name);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(timeout_ms, timeout_ms);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(exit_dump_len, exit_dump_len);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(hotplug_seq, hotplug_seq);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cid_shard_ಗಾತ್ರ, cid_shard_ಗಾತ್ರ);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(rescue_bandwidth_ppt, rescue_bandwidth_ppt);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(rescue_ಕಾಲಾಂಶ_us, rescue_ಕಾಲಾಂಶ_us);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(sub_cgroup_id, sub_cgroup_id);
	/* shared callbacks: the union view requires byte-for-byte offset match */
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(ಸರತಿಗೆ_ಸೇರಿಸು, ಸರತಿಗೆ_ಸೇರಿಸು);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(ಸರತಿಯಿಂದ_ತೆಗೆ, ಸರತಿಯಿಂದ_ತೆಗೆ);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(dispatch, dispatch);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(tick, tick);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(runnable, runnable);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(running, running);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(stopping, stopping);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(quiescent, quiescent);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(yield, yield);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(core_sched_before, core_sched_before);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(ಗಣ_ತೂಕ, ಗಣ_ತೂಕ);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(update_idle, update_idle);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(init_task, init_task);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(exit_task, exit_task);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(enable, enable);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(disable, disable);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(dump, dump);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(dump_task, dump_task);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(sub_attach, sub_attach);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(sub_detach, sub_detach);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(sub_caps_updated, sub_caps_updated);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(sub_ecaps_updated, sub_ecaps_updated);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(init_cids, init_cids);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(init, init);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(ನಿರ್ಗಮಿಸು, ನಿರ್ಗಮಿಸು);
	/* renamed callbacks must occupy the same slot as their cpu-form sibling */
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(select_cpu, select_cid);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(ಗಣ_cpumask, ಗಣ_cmask);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cpu_online, cid_online);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cpu_offline, cid_offline);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(dump_cpu, dump_cid);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_init, cpuctl_init);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_exit, cpuctl_exit);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_prep_move, cpuctl_prep_move);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_move, cpuctl_move);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_cancel_move, cpuctl_cancel_move);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_ಗಣ_ತೂಕ, cpuctl_ಗಣ_ತೂಕ);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_ಗಣ_bandwidth, cpuctl_ಗಣ_bandwidth);
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(cgroup_ಗಣ_idle, cpuctl_ಗಣ_idle);
#ಆದರೆ_ಅಂತ್ಯ
	/* @priv tail must align since both share the same data block */
	CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ(priv, priv);
	/*
	 * cid-form must end exactly at @priv - scx_validate_ops() skips
	 * cpu_acquire/cpu_release for cid-form because reading those fields
	 * past the BPF allocation would be UB.
	 */
	BUILD_BUG_ON(offsetof(ರಚನೆ sched_ext_ops_cid, __end) !=
		     offsetofend(ರಚನೆ sched_ext_ops, priv));
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CID_ಸ್ಥಾನಾಂತರ_ಹೊಂದಿಕೆ

	/*
	 * kfunc registration can't be done from init_sched_ext_class() as
	 * register_btf_kfunc_id_set() needs most of the system to be up.
	 *
	 * Some kfuncs are context-sensitive and can only be called from
	 * specific SCX ops. They are grouped into per-context BTF sets, each
	 * registered with scx_kfunc_context_filter as its .filter callback. The
	 * BPF core dedups identical filter pointers per hook
	 * (btf_populate_kfunc_set()), so the filter is invoked exactly once per
	 * kfunc lookup; it consults scx_kf_allow_flags[] to enforce per-op
	 * restrictions at verify time.
	 */
	ಆದರೆ ((ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ಗಣ_ಸರತಿಗೆ_ಸೇರಿಸು_dispatch)) ||
	    (ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ಗಣ_dispatch)) ||
	    (ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ಗಣ_cpu_release)) ||
	    (ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ಗಣ_unlocked)) ||
	    (ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_SYSCALL,
					     &scx_kfunc_ಗಣ_unlocked)) ||
	    (ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ಗಣ_any)) ||
	    (ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_TRACING,
					     &scx_kfunc_ಗಣ_any)) ||
	    (ret = ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_SYSCALL,
					     &scx_kfunc_ಗಣ_any))) {
		pr_err("sched_ext: Failed to register kfunc sets (%d)\n", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = scx_idle_init();
	ಆದರೆ (ret) {
		pr_err("sched_ext: Failed to initialize idle tracking (%d)\n", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = scx_cid_kfunc_init();
	ಆದರೆ (ret) {
		pr_err("sched_ext: Failed to register cid kfuncs (%d)\n", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = ರಿಜಿಸ್ಟರ್_bpf_struct_ops(&bpf_sched_ext_ops, sched_ext_ops);
	ಆದರೆ (ret) {
		pr_err("sched_ext: Failed to register struct_ops (%d)\n", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = ರಿಜಿಸ್ಟರ್_bpf_struct_ops(&bpf_sched_ext_ops_cid, sched_ext_ops_cid);
	ಆದರೆ (ret) {
		pr_err("sched_ext: Failed to register cid struct_ops (%d)\n", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = ರಿಜಿಸ್ಟರ್_pm_notifier(&scx_pm_notifier);
	ಆದರೆ (ret) {
		pr_err("sched_ext: Failed to register PM notifier (%d)\n", ret);
		ಹಿಂತಿರುಗಿ ret;
	}

	scx_kset = kset_create_and_ಕೂಡಿಸು("sched_ext", &scx_uevent_ops, kernel_kobj);
	ಆದರೆ (!scx_kset) {
		pr_err("sched_ext: Failed to create /sys/kernel/sched_ext\n");
		ಹಿಂತಿರುಗಿ -ENOMEM;
	}

	ret = sysfs_create_ಗುಂಪು(&scx_kset->kobj, &scx_global_attr_ಗುಂಪು);
	ಆದರೆ (ret < 0) {
		pr_err("sched_ext: Failed to add global attributes\n");
		ಹಿಂತಿರುಗಿ ret;
	}

	ಹಿಂತಿರುಗಿ 0;
}
__initcall(scx_init);

/*
 * Compatibility markers for userspace. Existence of a marker function
 * represents that the kernel supports that sched-ext feature.
 */

/*
 * scx_compat_marker_cgroup_set_bandwidth_may_sleep: advertises that
 * ops.cgroup_set_bandwidth() may be implemented as a sleepable callback.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
DEFINE_SCX_COMPAT_MARKER(cgroup_ಗಣ_bandwidth_may_sleep);
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_GROUP_SCHED */
