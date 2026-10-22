/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/bitmap.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/btf_ids.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rhashtable.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/isolation.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/suspend.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sysrq.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "../pelt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cid.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "arena.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "idle.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sub.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "inlines.h"

DEFINE_RAW_SPINLOCK(scx_sched_ਤਾਲਾ);

bool scx_allow_proxy_exec(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ true;
}

/*
 * NOTE: sched_ext is in the process of growing multiple scheduler support and
 * scx_root usage is in a transitional state. Naked dereferences are safe if the
 * caller is one of the tasks attached to SCX and explicit RCU dereference is
 * necessary otherwise. Naked scx_root dereferences trigger sparse warnings but
 * are used as temporary markers to indicate that the dereferences need to be
 * updated to point to the associated scheduler instances rather than scx_root.
 */
ਬਣਤਰ scx_sched __rcu *scx_ਜੜ੍ਹ;

/*
 * All scheds, writers must hold both scx_enable_mutex and scx_sched_lock.
 * Readers can hold either or rcu_read_lock().
 */
LIST_ਸਿਰ(scx_sched_all);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
ਅਚਲ ਬਣਤਰ rhashtable_params scx_sched_hash_params = {
	.ਕੁੰਜੀ_len		= sizeof_ਖੇਤਰ(ਬਣਤਰ scx_sched, ops.sub_cgroup_id),
	.ਕੁੰਜੀ_ਵਿੱਥ		= offsetof(ਬਣਤਰ scx_sched, ops.sub_cgroup_id),
	.ਸਿਰ_ਵਿੱਥ		= offsetof(ਬਣਤਰ scx_sched, hash_ਗੰਢ),
	.insecure_elasticity	= true,	/* inserted under scx_sched_lock */
};

ਬਣਤਰ rhashtable scx_sched_hash;
#ਜੇ_ਅੰਤ

/* see SCX_OPS_TID_TO_TASK */
ਸਥਿਰ ਅਚਲ ਬਣਤਰ rhashtable_params scx_tid_hash_params = {
	.ਕੁੰਜੀ_len		= sizeof_ਖੇਤਰ(ਬਣਤਰ sched_ext_entity, tid),
	.ਕੁੰਜੀ_ਵਿੱਥ		= offsetof(ਬਣਤਰ sched_ext_entity, tid),
	.ਸਿਰ_ਵਿੱਥ		= offsetof(ਬਣਤਰ sched_ext_entity, tid_hash_ਗੰਢ),
	.insecure_elasticity	= true,	/* inserted/removed under scx_tasks_lock */
};
ਸਥਿਰ ਬਣਤਰ rhashtable scx_tid_hash;

/*
 * During exit, a task may schedule after losing its PIDs. When disabling the
 * BPF scheduler, we need to be able to iterate tasks in every state to
 * guarantee system safety. Maintain a dedicated task list which contains every
 * task between its fork and eventual free.
 */
ਸਥਿਰ DEFINE_RAW_SPINLOCK(scx_tasks_ਤਾਲਾ);
ਸਥਿਰ LIST_ਸਿਰ(scx_tasks);

/* ops enable/disable */
DEFINE_MUTEX(scx_enable_mutex);
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(__scx_enabled);
DEFINE_PERCPU_RWSEM(scx_fork_rwsem);
ਸਥਿਰ atomic_t scx_enable_state_var = ATOMIC_INIT(SCX_DISABLED);
ਸਥਿਰ DEFINE_RAW_SPINLOCK(scx_bypass_ਤਾਲਾ);
ਸਥਿਰ bool scx_init_task_enabled;
ਸਥਿਰ bool scx_switching_all;
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(__scx_switched_all);
ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_FALSE(__scx_tid_to_task_enabled);

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
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_tid_ਮੰਜ਼ਿਲ_task_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_likely(&__scx_tid_to_task_enabled);
}

ਸਥਿਰ atomic_long_t scx_nr_rejected = ATOMIC_LONG_INIT(0);
ਸਥਿਰ atomic_long_t scx_hotplug_seq = ATOMIC_LONG_INIT(0);

/* Global cursor for the per-CPU tid allocator. Starts at 1; tid 0 is reserved. */
ਸਥਿਰ atomic64_t scx_tid_cursor = ATOMIC64_INIT(1);

/* is @dsq synchronized by the containing rq lock instead of dsq->lock? */
ਸਥਿਰ bool dsq_is_rq_owned(ਬਣਤਰ scx_dispatch_q *dsq)
{
	ਚੋਣ (dsq->id) {
	ਹਾਲਤ SCX_DSQ_LOCAL:
	ਹਾਲਤ SCX_DSQ_REJECT:
	ਹਾਲਤ SCX_DSQ_RESCUE:
		ਵਾਪਸ true;
	ਮੂਲ:
		ਵਾਪਸ false;
	}
}

/* Cursor for unique scx_sched instance ids. id 0 is reserved. */
ਸਥਿਰ atomic64_t scx_sched_id_cursor = ATOMIC64_INIT(0);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
/*
 * The sub sched being enabled. Used by scx_disable_and_exit_task() to exit
 * tasks for the sub-sched being enabled. Use a global variable instead of a
 * per-task field as all enables are serialized.
 */
ਬਣਤਰ scx_sched *scx_enabling_sub_sched;
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_enabling_sub_sched	(ਬਣਤਰ scx_sched *)ਨਿਰਾ
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

/*
 * A monotonically increasing sequence number that is incremented every time a
 * scheduler is enabled. This can be used to check if any custom sched_ext
 * scheduler has ever been used in the system.
 */
ਸਥਿਰ atomic_long_t scx_enable_seq = ATOMIC_LONG_INIT(0);

/*
 * Watchdog interval. All scx_sched's share a single watchdog timer and the
 * interval is half of the shortest sch->watchdog_timeout.
 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ scx_watchdog_interval;

/*
 * The last time the delayed work was run. This delayed work relies on
 * ksoftirqd being able to run to service timer interrupts, so it's possible
 * that this work itself could get wedged. To account for this, we check that
 * it's not stalled in the timer tick, and trigger an error if it is.
 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ scx_watchdog_timestamp = INITIAL_JIFFIES;

ਸਥਿਰ ਬਣਤਰ delayed_work scx_watchdog_work;

/*
 * For %SCX_KICK_WAIT: Each CPU has a pointer to an array of kick_sync sequence
 * numbers. The arrays are allocated with kvzalloc() as size can exceed percpu
 * allocator limits on large machines. O(nr_cpu_ids^2) allocation, allocated
 * lazily when enabling and freed when disabling to avoid waste when sched_ext
 * isn't active.
 */
ਬਣਤਰ scx_kick_syncs {
	ਬਣਤਰ rcu_ਸਿਰ		rcu;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		syncs[];
};

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ scx_kick_syncs __rcu *, scx_kick_syncs);

/*
 * Per-CPU buffered allocator state for p->scx.tid. Each CPU pulls a chunk of
 * SCX_TID_CHUNK ids from scx_tid_cursor and hands them out locally without
 * further synchronization. See scx_alloc_tid().
 */
ਬਣਤਰ scx_tid_alloc {
	u64	ਅਗਲਾ;
	u64	ਸਿਰਾ;
};
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ scx_tid_alloc, scx_tid_alloc);

/*
 * Direct dispatch marker.
 *
 * Non-NULL values are used for direct dispatch from enqueue path. A valid
 * pointer points to the task currently being enqueued. An ERR_PTR value is used
 * to indicate that direct dispatch has already happened.
 */
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ task_struct *, ਸਿੱਧਾ_dispatch_task);

ਸਥਿਰ ਅਚਲ ਬਣਤਰ rhashtable_params dsq_hash_params = {
	.ਕੁੰਜੀ_len		= sizeof_ਖੇਤਰ(ਬਣਤਰ scx_dispatch_q, id),
	.ਕੁੰਜੀ_ਵਿੱਥ		= offsetof(ਬਣਤਰ scx_dispatch_q, id),
	.ਸਿਰ_ਵਿੱਥ		= offsetof(ਬਣਤਰ scx_dispatch_q, hash_ਗੰਢ),
};

ਸਥਿਰ LLIST_ਸਿਰ(dsqs_ਮੰਜ਼ਿਲ_free);

/* ops debug dump */
ਸਥਿਰ DEFINE_RAW_SPINLOCK(scx_dump_ਤਾਲਾ);

ਬਣਤਰ scx_dump_ਅੰਕੜੇ {
	s32			cpu;
	bool			ਪਹਿਲਾ;
	s32			cursor;
	ਬਣਤਰ seq_buf		*s;
	ਅਚਲ ਅੱਖਰ		*ਅਗੇਤਰ;
	ਬਣਤਰ scx_bstr_buf	buf;
};

ਸਥਿਰ ਬਣਤਰ scx_dump_ਅੰਕੜੇ scx_dump_ਅੰਕੜੇ = {
	.cpu			= -1,
};

/* /sys/kernel/sched_ext interface */
ਸਥਿਰ ਬਣਤਰ kset *scx_kset;

/*
 * Parameters that can be adjusted through /sys/module/sched_ext/parameters.
 * There usually is no reason to modify these as normal scheduler operation
 * shouldn't be affected by them. The knobs are primarily for debugging.
 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scx_ਟੁਕੜਾ_bypass_us = SCX_ਟੁਕੜਾ_BYPASS / NSEC_ਪ੍ਰਤੀ_USEC;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ scx_bypass_lb_intv_us = SCX_BYPASS_LB_DFL_INTV_US;

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_ਟੁਕੜਾ_us(ਅਚਲ ਅੱਖਰ *val, ਅਚਲ ਬਣਤਰ kernel_param *kp)
{
	ਵਾਪਸ param_ਸੈੱਟ_uint_minmax(val, kp, 100, 100 * USEC_ਪ੍ਰਤੀ_MSEC);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ kernel_param_ops ਟੁਕੜਾ_us_param_ops = {
	.ਸੈੱਟ = ਸੈੱਟ_ਟੁਕੜਾ_us,
	.get = param_get_uint,
};

ਸਥਿਰ ਪੂਰਨ ਸੈੱਟ_bypass_lb_intv_us(ਅਚਲ ਅੱਖਰ *val, ਅਚਲ ਬਣਤਰ kernel_param *kp)
{
	ਵਾਪਸ param_ਸੈੱਟ_uint_minmax(val, kp, 0, 10 * USEC_ਪ੍ਰਤੀ_SEC);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ kernel_param_ops bypass_lb_intv_us_param_ops = {
	.ਸੈੱਟ = ਸੈੱਟ_bypass_lb_intv_us,
	.get = param_get_uint,
};

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MODULE_PARAM_ਅਗੇਤਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MODULE_PARAM_ਅਗੇਤਰ	"sched_ext."

module_param_cb(ਟੁਕੜਾ_bypass_us, &ਟੁਕੜਾ_us_param_ops, &scx_ਟੁਕੜਾ_bypass_us, 0600);
MODULE_PARM_DESC(ਟੁਕੜਾ_bypass_us, "bypass slice in microseconds, applied on [un]load (100us to 100ms)");
module_param_cb(bypass_lb_intv_us, &bypass_lb_intv_us_param_ops, &scx_bypass_lb_intv_us, 0600);
MODULE_PARM_DESC(bypass_lb_intv_us, "bypass load balance interval in microseconds (0 (disable) to 10s)");

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MODULE_PARAM_ਅਗੇਤਰ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CREATE_TRACE_POINTS
#ਅਨ੍ਤਰ੍ਭਾਵ <trace/events/sched_ext.h>

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_deferred(ਬਣਤਰ rq *rq);
ਸਥਿਰ bool task_dead_and_ਪੂਰਾ(ਬਣਤਰ task_struct *p);
ਸਥਿਰ ਖਾਲੀ scx_disable(ਬਣਤਰ scx_sched *sch, ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ);

__printf(5, 6) bool __scx_exit(ਬਣਤਰ scx_sched *sch,
			       ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ, s64 exit_ਕੋਡ,
			       s32 exit_cpu, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
	va_list ਦਲੀਲਾਂ;
	bool ret;

	va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
	ret = scx_vexit(sch, ਕਿਸਮ, exit_ਕੋਡ, exit_cpu, fmt, ਦਲੀਲਾਂ);
	va_ਸਿਰਾ(ਦਲੀਲਾਂ);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਲੰਮਾ jiffies_delta_msecs(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ at, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ now)
{
	ਜੇ (ਸਮਾਂ_after(at, now))
		ਵਾਪਸ jiffies_ਮੰਜ਼ਿਲ_msecs(at - now);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ -(ਲੰਮਾ)jiffies_ਮੰਜ਼ਿਲ_msecs(now - at);
}

ਸਥਿਰ bool u32_before(u32 a, u32 b)
{
	ਵਾਪਸ (s32)(a - b) < 0;
}

/**
 * scx_is_descendant - Test whether sched is a descendant
 * @sch: sched to test
 * @ancestor: ancestor sched to test against
 *
 * Test whether @sch is a descendant of @ancestor.
 */
bool scx_is_descendant(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ scx_sched *ancestor)
{
	ਜੇ (sch->level < ancestor->level)
		ਵਾਪਸ false;
	ਵਾਪਸ sch->ancestors[ancestor->level] == ancestor;
}

ਸਥਿਰ ਬਣਤਰ scx_dispatch_q *ਲੱਭੋ_global_dsq(ਬਣਤਰ scx_sched *sch, s32 cpu)
{
	ਵਾਪਸ &sch->pnode[cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu)]->global_dsq;
}

ਸਥਿਰ ਬਣਤਰ scx_dispatch_q *ਲੱਭੋ_user_dsq(ਬਣਤਰ scx_sched *sch, u64 dsq_id)
{
	ਵਾਪਸ rhashtable_lookup(&sch->dsq_hash, &dsq_id, dsq_hash_params);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ sched_class *scx_setscheduler_class(ਬਣਤਰ task_struct *p)
{
	ਜੇ (p->sched_class == &stop_sched_class)
		ਵਾਪਸ &stop_sched_class;

	ਵਾਪਸ __setscheduler_class(p->policy, p->prio);
}

ਸਥਿਰ ਬਣਤਰ scx_dispatch_q *bypass_enq_ਨਿਸ਼ਾਨਾ_dsq(ਬਣਤਰ scx_sched *sch, s32 cpu)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
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
	ਜਦੋਂ (scx_ਮਾਪਾ(sch) && scx_bypassing(sch, cpu))
		sch = scx_ਮਾਪਾ(sch);
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

	ਵਾਪਸ scx_bypass_dsq(sch, cpu);
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
ਸਥਿਰ bool rq_is_open(ਬਣਤਰ rq *rq, u64 enq_flags)
{
	lockdep_assert_rq_held(rq);

	/*
	 * A higher-priority class task is either running or in the process of
	 * waking up on @rq.
	 */
	ਜੇ (sched_class_above(rq->ਅਗਲਾ_class, &ext_sched_class))
		ਵਾਪਸ false;

	/*
	 * @rq is either in transition to or in idle and there is no
	 * higher-priority class task waking up on it.
	 */
	ਜੇ (sched_class_above(&ext_sched_class, rq->ਅਗਲਾ_class))
		ਵਾਪਸ true;

	/*
	 * @rq is either picking, in transition to, or running an SCX task.
	 */

	/*
	 * If we're in the dispatch path holding rq lock, $curr may or may not
	 * be ready depending on whether the on-going dispatch decides to extend
	 * $curr's slice. We say yes here and resolve it at the end of dispatch.
	 * See dispatch_one().
	 */
	ਜੇ (rq->scx.flags & SCX_RQ_ਵਿੱਚ_DISPATCH)
		ਵਾਪਸ true;

	/*
	 * %SCX_ENQ_PREEMPT clears $curr's slice if on SCX and kicks dispatch,
	 * so allow it to avoid spuriously triggering reenq on a combined
	 * PREEMPT|IMMED insertion.
	 */
	ਜੇ (enq_flags & SCX_ENQ_PREEMPT) {
		ਬਣਤਰ task_struct *curr = rq->curr;

		/*
		 * A protected slice refuses the preemption and the cpu stays
		 * occupied. See rq_owned_post_enq().
		 */
		ਵਾਪਸ curr->sched_class != &ext_sched_class ||
			likely(!(curr->scx.flags & SCX_TASK_PROTECTED));
	}

	/*
	 * @rq is either in transition to or running an SCX task and can't go
	 * idle without another SCX dispatch cycle.
	 */
	ਵਾਪਸ false;
}

/*
 * Track the rq currently locked.
 *
 * This allows kfuncs to safely operate on rq from any scx ops callback,
 * knowing which rq is already locked.
 */
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ rq *, scx_locked_rq_state);

/*
 * Under core scheduling, a pick that releases the rq lock invalidates the
 * core-wide selection it is part of. Count the releases so that the core-sched
 * pick can tell whether one happened across dispatch.
 */
ਸਥਿਰ ਖਾਲੀ scx_rq_ਤਾਲਾ_drop(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
	ਜੇ (sched_core_enabled(rq))
		rq->scx.ਤਾਲਾ_drop_seq++;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ switch_rq_ਤਾਲਾ(ਬਣਤਰ rq *from, ਬਣਤਰ rq *ਮੰਜ਼ਿਲ)
{
	bool tracked = scx_locked_rq() == from;

	ਜੇ (tracked)
		update_locked_rq(ਨਿਰਾ);
	scx_rq_ਤਾਲਾ_drop(from);
	raw_spin_rq_unlock(from);
	raw_spin_rq_ਤਾਲਾ(ਮੰਜ਼ਿਲ);
	ਜੇ (tracked)
		update_locked_rq(ਮੰਜ਼ਿਲ);
}

/*
 * Flipped on enable per sch->is_cid_type. Declared in internal.h so
 * subsystem inlines can read it.
 */
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(__scx_is_cid_type);

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
ਸਥਿਰ ਬਣਤਰ scx_cmask *scx_fill_cmask_scratch(ਬਣਤਰ scx_sched *sch,
						ਅਚਲ ਬਣਤਰ cpumask *cpumask)
{
	ਬਣਤਰ scx_cmask *kern_va = *this_cpu_ptr(sch->ਸੈੱਟ_cmask_scratch);
	ਬਣਤਰ scx_cmask_ref ref;

	scx_cmask_ref_init_kern(sch, kern_va, 0, ਅੰਕ_possible_cpus(), &ref);
	scx_cmask_ref_from_cpumask(&ref, cpumask);
	ਵਾਪਸ kern_va;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_call_op_ਸੈੱਟ_cpumask(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq,
					   ਬਣਤਰ task_struct *task,
					   ਅਚਲ ਬਣਤਰ cpumask *cpumask)
{
	ਜੇ (scx_is_cid_type())
		SCX_CALL_CID_OP_TASK(sch, ਸੈੱਟ_cmask, rq, task,
				     scx_fill_cmask_scratch(sch, cpumask));
	ਨਹੀਂਤਾਂ
		SCX_CALL_OP_TASK(sch, ਸੈੱਟ_cpumask, rq, task, cpumask);
}

ਗਿਣਤੀ scx_dsq_iter_flags {
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
ਸਥਿਰ ਬਣਤਰ task_struct *nldsq_ਅਗਲਾ_task(ਬਣਤਰ scx_dispatch_q *dsq,
					   ਬਣਤਰ task_struct *cur, bool rev)
{
	ਬਣਤਰ list_ਸਿਰ *list_ਗੰਢ;
	ਬਣਤਰ scx_dsq_list_ਗੰਢ *dsq_lnode;

	lockdep_assert_held(&dsq->ਤਾਲਾ);

	ਜੇ (cur)
		list_ਗੰਢ = &cur->scx.dsq_list.ਗੰਢ;
	ਨਹੀਂਤਾਂ
		list_ਗੰਢ = &dsq->list;

	/* find the next task, need to skip BPF iteration cursors */
	ਕਰੋ {
		ਜੇ (rev)
			list_ਗੰਢ = list_ਗੰਢ->prev;
		ਨਹੀਂਤਾਂ
			list_ਗੰਢ = list_ਗੰਢ->ਅਗਲਾ;

		ਜੇ (list_ਗੰਢ == &dsq->list)
			ਵਾਪਸ ਨਿਰਾ;

		dsq_lnode = container_ਦਾ(list_ਗੰਢ, ਬਣਤਰ scx_dsq_list_ਗੰਢ,
					 ਗੰਢ);
	} ਜਦੋਂ (dsq_lnode->flags & SCX_DSQ_LNODE_ITER_CURSOR);

	ਵਾਪਸ container_ਦਾ(dsq_lnode, ਬਣਤਰ task_struct, scx.dsq_list);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ nldsq_for_each_task(p, dsq)						\
	ਲਈ ((p) = nldsq_ਅਗਲਾ_task((dsq), ਨਿਰਾ, false); (p);			\
	     (p) = nldsq_ਅਗਲਾ_task((dsq), (p), false))

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
ਸਥਿਰ ਬਣਤਰ task_struct *nldsq_cursor_ਅਗਲਾ_task(ਬਣਤਰ scx_dsq_list_ਗੰਢ *cursor,
						  ਬਣਤਰ scx_dispatch_q *dsq)
{
	bool rev = cursor->flags & SCX_DSQ_ITER_REV;
	ਬਣਤਰ task_struct *p;

	lockdep_assert_held(&dsq->ਤਾਲਾ);
	BUG_ON(!(cursor->flags & SCX_DSQ_LNODE_ITER_CURSOR));

	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&cursor->ਗੰਢ))
		p = ਨਿਰਾ;
	ਨਹੀਂਤਾਂ
		p = container_ਦਾ(cursor, ਬਣਤਰ task_struct, scx.dsq_list);

	/* skip cursors and tasks that were queued after @cursor init */
	ਕਰੋ {
		p = nldsq_ਅਗਲਾ_task(dsq, p, rev);
	} ਜਦੋਂ (p && unlikely(u32_before(cursor->priv, p->scx.dsq_seq)));

	ਜੇ (p) {
		ਜੇ (rev)
			list_move_ਪੂਛ(&cursor->ਗੰਢ, &p->scx.dsq_list.ਗੰਢ);
		ਨਹੀਂਤਾਂ
			list_move(&cursor->ਗੰਢ, &p->scx.dsq_list.ਗੰਢ);
	} ਨਹੀਂਤਾਂ {
		list_del_init(&cursor->ਗੰਢ);
	}

	ਵਾਪਸ p;
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
ਸਥਿਰ bool nldsq_cursor_ਗੁਆਚਾ_task(ਬਣਤਰ scx_dsq_list_ਗੰਢ *cursor,
				   ਬਣਤਰ rq *rq, ਬਣਤਰ scx_dispatch_q *dsq,
				   ਬਣਤਰ task_struct *p)
{
	lockdep_assert_rq_held(rq);
	lockdep_assert_held(&dsq->ਤਾਲਾ);

	/*
	 * @p could have already left $src_dsq, got re-enqueud, or be in the
	 * process of being consumed by someone else.
	 */
	ਜੇ (unlikely(p->scx.dsq != dsq ||
		     u32_before(cursor->priv, p->scx.dsq_seq) ||
		     p->scx.holding_cpu >= 0))
		ਵਾਪਸ true;

	/* if @p has stayed on @dsq, its rq couldn't have changed */
	ਜੇ (WARN_ON_ONCE(rq != task_rq(p)))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

/*
 * BPF DSQ iterator. Tasks in a non-local DSQ can be iterated in [reverse]
 * dispatch order. BPF-visible iterator is opaque and larger to allow future
 * changes without breaking backward compatibility. Can be used with
 * bpf_for_each(). See bpf_iter_scx_dsq_*().
 */
ਬਣਤਰ bpf_iter_scx_dsq_kern {
	ਬਣਤਰ scx_dsq_list_ਗੰਢ	cursor;
	ਬਣਤਰ scx_dispatch_q		*dsq;
	u64				ਟੁਕੜਾ;
	u64				vtime;
} __attribute__((aligned(8)));

ਬਣਤਰ bpf_iter_scx_dsq {
	u64				__opaque[6];
} __attribute__((aligned(8)));


u32 scx_get_task_state(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->scx.flags & SCX_TASK_STATE_ਨਕਾਬ;
}

ਖਾਲੀ scx_ਸੈੱਟ_task_state(ਬਣਤਰ task_struct *p, u32 state)
{
	u32 prev_state = scx_get_task_state(p);
	bool warn = false;

	ਚੋਣ (state) {
	ਹਾਲਤ SCX_TASK_NONE:
		warn = prev_state == SCX_TASK_DEAD;
		ਰੋਕੋ;
	ਹਾਲਤ SCX_TASK_INIT_BEGIN:
		warn = prev_state != SCX_TASK_NONE;
		ਰੋਕੋ;
	ਹਾਲਤ SCX_TASK_INIT:
		warn = prev_state != SCX_TASK_INIT_BEGIN;
		p->scx.flags |= SCX_TASK_RESET_RUNNABLE_AT;
		ਰੋਕੋ;
	ਹਾਲਤ SCX_TASK_READY:
		warn = !(prev_state == SCX_TASK_INIT ||
			 prev_state == SCX_TASK_ENABLED);
		ਰੋਕੋ;
	ਹਾਲਤ SCX_TASK_ENABLED:
		warn = prev_state != SCX_TASK_READY;
		ਰੋਕੋ;
	ਹਾਲਤ SCX_TASK_DEAD:
		warn = !(prev_state == SCX_TASK_NONE ||
			 prev_state == SCX_TASK_INIT_BEGIN);
		ਰੋਕੋ;
	ਮੂਲ:
		WARN_ONCE(1, "sched_ext: Invalid task state %d -> %d for %s[%d]",
			  prev_state, state, p->comm, p->pid);
		ਵਾਪਸ;
	}

	WARN_ONCE(warn, "sched_ext: Invalid task state transition 0x%x -> 0x%x for %s[%d]",
		  prev_state, state, p->comm, p->pid);

	p->scx.flags &= ~SCX_TASK_STATE_ਨਕਾਬ;
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
ਖਾਲੀ scx_task_iter_ਸ਼ੁਰੂ(ਬਣਤਰ scx_task_iter *iter, ਬਣਤਰ cgroup *cgrp)
{
	memset(iter, 0, ਆਕਾਰ(*iter));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਜੇ (cgrp) {
		lockdep_assert_held(&cgroup_mutex);
		iter->cgrp = cgrp;
		iter->css_pos = css_ਅਗਲਾ_descendant_pre(ਨਿਰਾ, &iter->cgrp->self);
		css_task_iter_ਸ਼ੁਰੂ(iter->css_pos, CSS_TASK_ITER_WITH_DEAD,
				    &iter->css_iter);
		ਵਾਪਸ;
	}
#ਜੇ_ਅੰਤ
	raw_spin_ਤਾਲਾ_irq(&scx_tasks_ਤਾਲਾ);

	iter->cursor = (ਬਣਤਰ sched_ext_entity){ .flags = SCX_TASK_CURSOR };
	list_ਜੋੜੋ(&iter->cursor.tasks_ਗੰਢ, &scx_tasks);
	iter->list_locked = true;
}

ਸਥਿਰ ਖਾਲੀ __scx_task_iter_rq_unlock(ਬਣਤਰ scx_task_iter *iter)
{
	ਜੇ (iter->locked_task) {
		__balance_callbacks(iter->rq, &iter->rf);
		task_rq_unlock(iter->rq, iter->locked_task, &iter->rf);
		iter->locked_task = ਨਿਰਾ;
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
ਖਾਲੀ scx_task_iter_unlock(ਬਣਤਰ scx_task_iter *iter)
{
	__scx_task_iter_rq_unlock(iter);
	ਜੇ (iter->list_locked) {
		iter->list_locked = false;
		raw_spin_unlock_irq(&scx_tasks_ਤਾਲਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ __scx_task_iter_maybe_relock(ਬਣਤਰ scx_task_iter *iter)
{
	ਜੇ (!iter->list_locked) {
		raw_spin_ਤਾਲਾ_irq(&scx_tasks_ਤਾਲਾ);
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
ਸਥਿਰ ਖਾਲੀ scx_task_iter_relock(ਬਣਤਰ scx_task_iter *iter,
				 ਬਣਤਰ task_struct *p)
{
	__scx_task_iter_maybe_relock(iter);
	ਜੇ (p) {
		iter->rq = task_rq_ਤਾਲਾ_2(p, &iter->rf);
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
ਖਾਲੀ scx_task_iter_stop(ਬਣਤਰ scx_task_iter *iter)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਜੇ (iter->cgrp) {
		ਜੇ (iter->css_pos)
			css_task_iter_ਸਿਰਾ(&iter->css_iter);
		__scx_task_iter_rq_unlock(iter);
		ਵਾਪਸ;
	}
#ਜੇ_ਅੰਤ
	__scx_task_iter_maybe_relock(iter);
	list_del_init(&iter->cursor.tasks_ਗੰਢ);
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
ਸਥਿਰ ਬਣਤਰ task_struct *scx_task_iter_ਅਗਲਾ(ਬਣਤਰ scx_task_iter *iter)
{
	ਬਣਤਰ list_ਸਿਰ *cursor = &iter->cursor.tasks_ਗੰਢ;
	ਬਣਤਰ sched_ext_entity *pos;

	ਜੇ (!(++iter->cnt % SCX_TASK_ITER_BATCH)) {
		scx_task_iter_unlock(iter);
		cond_resched();
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਜੇ (iter->cgrp) {
		ਜਦੋਂ (iter->css_pos) {
			ਬਣਤਰ task_struct *p;

			p = css_task_iter_ਅਗਲਾ(&iter->css_iter);
			ਜੇ (p)
				ਵਾਪਸ p;

			css_task_iter_ਸਿਰਾ(&iter->css_iter);
			iter->css_pos = css_ਅਗਲਾ_descendant_pre(iter->css_pos,
								&iter->cgrp->self);
			ਜੇ (iter->css_pos)
				css_task_iter_ਸ਼ੁਰੂ(iter->css_pos, CSS_TASK_ITER_WITH_DEAD,
						    &iter->css_iter);
		}
		ਵਾਪਸ ਨਿਰਾ;
	}
#ਜੇ_ਅੰਤ
	__scx_task_iter_maybe_relock(iter);

	list_for_each_ਇੰਦਰਾਜ(pos, cursor, tasks_ਗੰਢ) {
		ਜੇ (&pos->tasks_ਗੰਢ == &scx_tasks)
			ਵਾਪਸ ਨਿਰਾ;
		ਜੇ (!(pos->flags & SCX_TASK_CURSOR)) {
			list_move(cursor, &pos->tasks_ਗੰਢ);
			ਵਾਪਸ container_ਦਾ(pos, ਬਣਤਰ task_struct, scx);
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
ਬਣਤਰ task_struct *scx_task_iter_ਅਗਲਾ_locked(ਬਣਤਰ scx_task_iter *iter)
{
	ਬਣਤਰ task_struct *p;

	__scx_task_iter_rq_unlock(iter);

	ਜਦੋਂ ((p = scx_task_iter_ਅਗਲਾ(iter))) {
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
		ਜੇ (p->sched_class == &idle_sched_class)
			ਜਾਰੀ;

		iter->rq = task_rq_ਤਾਲਾ_2(p, &iter->rf);
		iter->locked_task = p;

		/*
		 * cgroup_task_dead() removes the dead tasks from cset->tasks
		 * after sched_ext_dead() and cgroup iteration may see tasks
		 * which already finished sched_ext_dead(). %SCX_TASK_DEAD is
		 * set by sched_ext_dead() under @p's rq lock. Test it to
		 * avoid visiting tasks which are already dead from SCX POV.
		 */
		ਜੇ (scx_get_task_state(p) == SCX_TASK_DEAD) {
			__scx_task_iter_rq_unlock(iter);
			ਜਾਰੀ;
		}

		ਵਾਪਸ p;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

/**
 * scx_dump_event - Dump an event 'kind' in 'events' to 's'
 * @s: output seq_buf
 * @events: event stats
 * @kind: a kind of event to dump
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_dump_event(s, events, ਕਿਸਮ) ਕਰੋ {					\
	scx_dump_ਸਤਰ(&(s), "%40s: %16lld", #kind, (events)->ਕਿਸਮ);		\
} ਜਦੋਂ (0)


ਸਥਿਰ ਖਾਲੀ scx_read_events(ਬਣਤਰ scx_sched *sch,
			    ਬਣਤਰ scx_event_stats *events);

ਸਥਿਰ ਗਿਣਤੀ scx_enable_state scx_enable_state(ਖਾਲੀ)
{
	ਵਾਪਸ atomic_read(&scx_enable_state_var);
}

ਸਥਿਰ ਗਿਣਤੀ scx_enable_state scx_ਸੈੱਟ_enable_state(ਗਿਣਤੀ scx_enable_state ਮੰਜ਼ਿਲ)
{
	ਵਾਪਸ atomic_xchg(&scx_enable_state_var, ਮੰਜ਼ਿਲ);
}

ਸਥਿਰ bool scx_tryset_enable_state(ਗਿਣਤੀ scx_enable_state ਮੰਜ਼ਿਲ,
				    ਗਿਣਤੀ scx_enable_state from)
{
	ਪੂਰਨ from_v = from;

	ਵਾਪਸ atomic_try_cmpxchg(&scx_enable_state_var, &from_v, ਮੰਜ਼ਿਲ);
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
ਸਥਿਰ ਖਾਲੀ wait_ops_state(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ opss)
{
	ਕਰੋ {
		cpu_relax();
	} ਜਦੋਂ (atomic_long_read_acquire(&p->scx.ops_state) == opss);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool __cpu_valid(s32 cpu)
{
	ਵਾਪਸ likely(cpu >= 0 && cpu < nr_cpu_ids && cpu_possible(cpu));
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
bool scx_cpu_valid(ਬਣਤਰ scx_sched *sch, s32 cpu, ਅਚਲ ਅੱਖਰ *where)
{
	ਜੇ (__cpu_valid(cpu)) {
		ਵਾਪਸ true;
	} ਨਹੀਂਤਾਂ {
		scx_error(sch, "invalid CPU %d%s%s", cpu, where ? " " : "", where ?: "");
		ਵਾਪਸ false;
	}
}

ਸਥਿਰ ਖਾਲੀ deferred_bal_cb_workfn(ਬਣਤਰ rq *rq)
{
	ਲੜੀਵਾਰ_deferred(rq);
}

ਸਥਿਰ ਖਾਲੀ deferred_irq_workfn(ਬਣਤਰ irq_work *irq_work)
{
	ਬਣਤਰ rq *rq = container_ਦਾ(irq_work, ਬਣਤਰ rq, scx.deferred_irq_work);

	raw_spin_rq_ਤਾਲਾ(rq);
	ਲੜੀਵਾਰ_deferred(rq);
	scx_rq_ਤਾਲਾ_drop(rq);
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
ਸਥਿਰ ਖਾਲੀ ਸਮਾਂ_ਸਾਰਣੀ_deferred(ਬਣਤਰ rq *rq)
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
	irq_work_ਕਤਾਰ_on(&rq->scx.deferred_irq_work, cpu_ਦਾ(rq));
}

/**
 * schedule_deferred_locked - Schedule execution of deferred actions on an rq
 * @rq: target rq
 *
 * Schedule execution of deferred actions on @rq. Equivalent to
 * schedule_deferred() but requires @rq to be locked and can be more efficient.
 */
ਸਥਿਰ ਖਾਲੀ ਸਮਾਂ_ਸਾਰਣੀ_deferred_locked(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);

	/*
	 * If in the middle of waking up a task, task_woken_scx() will be called
	 * afterwards which will then run the deferred actions, no need to
	 * schedule anything.
	 */
	ਜੇ (rq->scx.flags & SCX_RQ_ਵਿੱਚ_WAKEUP)
		ਵਾਪਸ;

	/* Don't do anything if there already is a deferred operation. */
	ਜੇ (rq->scx.flags & SCX_RQ_BAL_CB_PENDING)
		ਵਾਪਸ;

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
	ਜੇ (rq->scx.flags & SCX_RQ_ਵਿੱਚ_DISPATCH) {
		rq->scx.flags |= SCX_RQ_BAL_CB_PENDING;
		ਵਾਪਸ;
	}

	/*
	 * No scheduler hooks available. Use the generic irq_work path. The
	 * above WAKEUP and DISPATCH paths should cover most of the cases and
	 * the time to IRQ re-enable shouldn't be long.
	 */
	ਸਮਾਂ_ਸਾਰਣੀ_deferred(rq);
}

ਖਾਲੀ scx_proxy_reenqueue_retry(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
}

ਖਾਲੀ ਸਮਾਂ_ਸਾਰਣੀ_dsq_reenq(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ scx_dispatch_q *dsq,
			u64 reenq_flags, ਬਣਤਰ rq *locked_rq)
{
	ਬਣਤਰ rq *rq;

	/*
	 * Allowing reenqueues doesn't make sense while bypassing. This also
	 * blocks from new reenqueues to be scheduled on dead scheds.
	 */
	ਜੇ (unlikely(READ_ONCE(sch->bypass_ਡੂੰਘਾਈ)))
		ਵਾਪਸ;

	ਜੇ (dsq->id == SCX_DSQ_LOCAL) {
		rq = container_ਦਾ(dsq, ਬਣਤਰ rq, scx.local_dsq);

		/*
		 * A sub-sched lacking baseline access on the target cid has no
		 * business triggering IPIs. The lockless test is fine: slipping
		 * through right after a revoke is harmless and a wrong denial
		 * can't happen - if the caller has seen its ownership, so does
		 * this test.
		 */
		ਜੇ (unlikely(scx_missing_caps(sch, cpu_ਦਾ(rq), SCX_CAP_ਅਧਾਰ))) {
			__scx_add_event(sch, SCX_EV_SUB_REENQ_DENIED, 1);
			ਵਾਪਸ;
		}

		ਬਣਤਰ scx_sched_pcpu *sch_pcpu = ਪ੍ਰਤੀ_cpu_ptr(sch->pcpu, cpu_ਦਾ(rq));
		ਬਣਤਰ scx_deferred_reenq_local *drl = &sch_pcpu->deferred_reenq_local;

		/*
		 * Pairs with smp_mb() in process_deferred_reenq_locals() and
		 * guarantees that there is a reenq_local() afterwards.
		 */
		smp_mb();

		ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&drl->ਗੰਢ) ||
		    (READ_ONCE(drl->flags) & reenq_flags) != reenq_flags) {

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ਤਾਲਾ);

			ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&drl->ਗੰਢ))
				list_move_ਪੂਛ(&drl->ਗੰਢ, &rq->scx.deferred_reenq_locals);
			WRITE_ONCE(drl->flags, drl->flags | reenq_flags);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (!(dsq->id & SCX_DSQ_FLAG_BUILTIN)) {
		rq = this_rq();

		ਬਣਤਰ scx_dsq_pcpu *dsq_pcpu = ਪ੍ਰਤੀ_cpu_ptr(dsq->pcpu, cpu_ਦਾ(rq));
		ਬਣਤਰ scx_deferred_reenq_user *dru = &dsq_pcpu->deferred_reenq_user;

		/*
		 * Pairs with smp_mb() in process_deferred_reenq_users() and
		 * guarantees that there is a reenq_user() afterwards.
		 */
		smp_mb();

		ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&dru->ਗੰਢ) ||
		    (READ_ONCE(dru->flags) & reenq_flags) != reenq_flags) {

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ਤਾਲਾ);

			/* @dsq is being destroyed, see free_dsq_irq_workfn() */
			ਜੇ (unlikely(READ_ONCE(dsq->id) == SCX_DSQ_INVALID))
				ਵਾਪਸ;

			ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&dru->ਗੰਢ))
				list_move_ਪੂਛ(&dru->ਗੰਢ, &rq->scx.deferred_reenq_users);
			WRITE_ONCE(dru->flags, dru->flags | reenq_flags);
		}
	} ਨਹੀਂਤਾਂ {
		scx_error(sch, "DSQ 0x%llx not allowed for reenq", dsq->id);
		ਵਾਪਸ;
	}

	ਜੇ (rq == locked_rq)
		ਸਮਾਂ_ਸਾਰਣੀ_deferred_locked(rq);
	ਨਹੀਂਤਾਂ
		ਸਮਾਂ_ਸਾਰਣੀ_deferred(rq);
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
ਗਿਣਤੀ scx_ਟੁਕੜਾ_oob_consts {
	SCX_ਟੁਕੜਾ_OOB_DUR_ਬਿੱਟ	= 43,
	SCX_ਟੁਕੜਾ_OOB_ID_ਬਿੱਟ	= 64 - SCX_ਟੁਕੜਾ_OOB_DUR_ਬਿੱਟ - 1,

	SCX_ਟੁਕੜਾ_OOB_DUR_ਨਕਾਬ	= (1LLU << SCX_ਟੁਕੜਾ_OOB_DUR_ਬਿੱਟ) - 1,
	SCX_ਟੁਕੜਾ_OOB_ID_ਖਿਸਕਾਓ	= SCX_ਟੁਕੜਾ_OOB_DUR_ਬਿੱਟ,
	SCX_ਟੁਕੜਾ_OOB_ID_ਨਕਾਬ	= (1LLU << SCX_ਟੁਕੜਾ_OOB_ID_ਬਿੱਟ) - 1,
	SCX_ਟੁਕੜਾ_OOB_PENDING	= 1LLU << 63,
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
ਸਥਿਰ ਖਾਲੀ clear_task_ਟੁਕੜਾ_oob(ਬਣਤਰ task_struct *p)
{
	ਜੇ (unlikely(atomic64_read(&p->scx.ਟੁਕੜਾ_oob)))
		atomic64_ਸੈੱਟ(&p->scx.ਟੁਕੜਾ_oob, 0);
}

/**
 * dsq_insert_head - FIFO head insertion honoring %SCX_TASK_PROTECTED
 * @dsq: DSQ to insert into
 * @p: task being inserted
 *
 * A HEAD insert should land behind any leading protected tasks. Return %true
 * indicates whether @p became the first entry.
 */
ਸਥਿਰ bool dsq_ਪਾਓ_ਸਿਰ(ਬਣਤਰ scx_dispatch_q *dsq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ list_ਸਿਰ *pos = &dsq->list;
	ਬਣਤਰ scx_dsq_list_ਗੰਢ *ਗੰਢ;

	/*
	 * Only rq-owned DSQs can hold protected tasks and the associated rq
	 * lock keeps their flags stable.
	 */
	ਜੇ (!dsq_is_rq_owned(dsq)) {
		list_ਜੋੜੋ(&p->scx.dsq_list.ਗੰਢ, &dsq->list);
		ਵਾਪਸ true;
	}

	list_for_each_ਇੰਦਰਾਜ(ਗੰਢ, &dsq->list, ਗੰਢ) {
		ਬਣਤਰ task_struct *q;

		ਜੇ (WARN_ON_ONCE(ਗੰਢ->flags & SCX_DSQ_LNODE_ITER_CURSOR))
			ਜਾਰੀ;

		q = container_ਦਾ(ਗੰਢ, ਬਣਤਰ task_struct, scx.dsq_list);
		ਜੇ (!(q->scx.flags & SCX_TASK_PROTECTED))
			ਰੋਕੋ;

		pos = &ਗੰਢ->ਗੰਢ;
	}

	list_ਜੋੜੋ(&p->scx.dsq_list.ਗੰਢ, pos);

	ਵਾਪਸ pos == &dsq->list;
}

/**
 * set_task_slice_keep_oob - Set @p's slice, leaving any pending oob request
 * @p: task of interest
 * @slice: slice to set
 *
 * While %SCX_TASK_PROTECTED is set, BPF schedulers may not modify the slice.
 * Refuse and return %false.
 */
ਸਥਿਰ bool ਸੈੱਟ_task_ਟੁਕੜਾ_keep_oob(ਬਣਤਰ task_struct *p, u64 ਟੁਕੜਾ)
{
	lockdep_assert_rq_held(task_rq(p));

	ਜੇ (unlikely(p->scx.flags & SCX_TASK_PROTECTED))
		ਵਾਪਸ false;

	p->scx.ਟੁਕੜਾ = ਟੁਕੜਾ;
	ਵਾਪਸ true;
}

/* set @p's slice, superseding any pending out-of-band request */
bool scx_ਸੈੱਟ_task_ਟੁਕੜਾ(ਬਣਤਰ task_struct *p, u64 ਟੁਕੜਾ)
{
	ਜੇ (!ਸੈੱਟ_task_ਟੁਕੜਾ_keep_oob(p, ਟੁਕੜਾ))
		ਵਾਪਸ false;
	clear_task_ਟੁਕੜਾ_oob(p);
	ਵਾਪਸ true;
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
ਖਾਲੀ scx_task_ਟੁਕੜਾ_ended(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	lockdep_assert_rq_held(rq);

	p->scx.flags &= ~SCX_TASK_PROTECTED;
	ਜੇ (unlikely(p == scx_rescuee(rq)))
		scx_rescue_ਸਿਰਾ(rq);
}

/* request @p's slice to be set to @slice, see the write rules above */
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_task_ਟੁਕੜਾ_oob(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p, u64 ਟੁਕੜਾ)
{
	u64 dur;

	ਜੇ (ਟੁਕੜਾ == SCX_ਟੁਕੜਾ_INF) {
		dur = SCX_ਟੁਕੜਾ_OOB_DUR_ਨਕਾਬ;
	} ਨਹੀਂਤਾਂ ਜੇ (unlikely(ਟੁਕੜਾ >= SCX_ਟੁਕੜਾ_OOB_DUR_ਨਕਾਬ)) {
		dur = SCX_ਟੁਕੜਾ_OOB_DUR_ਨਕਾਬ - 1;
		scx_ਜੋੜੋ_event(sch, SCX_EV_ਟੁਕੜਾ_CLAMPED, 1);
	} ਨਹੀਂਤਾਂ {
		dur = ਟੁਕੜਾ;
	}

	atomic64_ਸੈੱਟ(&p->scx.ਟੁਕੜਾ_oob, SCX_ਟੁਕੜਾ_OOB_PENDING |
		     ((sch->id & SCX_ਟੁਕੜਾ_OOB_ID_ਨਕਾਬ) << SCX_ਟੁਕੜਾ_OOB_ID_ਖਿਸਕਾਓ) | dur);
}

/*
 * Apply a pending out-of-band slice request under @rq's lock. A request whose
 * packed id no longer matches @p's current owner is dropped. An extension needs
 * baseline cpu access on @p's cid, shortening is always allowed, and a
 * protected slice refuses both. %SCX_EV_SLICE_DENIED counts the denials. See
 * the write rules above.
 */
ਸਥਿਰ ਖਾਲੀ apply_task_ਟੁਕੜਾ_oob(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	u64 oob, dur, ਟੁਕੜਾ;

	lockdep_assert_rq_held(rq);

	ਜੇ (likely(!atomic64_read(&p->scx.ਟੁਕੜਾ_oob)))
		ਵਾਪਸ;

	oob = atomic64_xchg(&p->scx.ਟੁਕੜਾ_oob, 0);
	ਜੇ (unlikely(!oob))
		ਵਾਪਸ;

	/* the issuing scheduler no longer owns @p, drop the request */
	ਜੇ (unlikely(((oob >> SCX_ਟੁਕੜਾ_OOB_ID_ਖਿਸਕਾਓ) & SCX_ਟੁਕੜਾ_OOB_ID_ਨਕਾਬ) !=
		     (scx_task_sched(p)->id & SCX_ਟੁਕੜਾ_OOB_ID_ਨਕਾਬ)))
		ਵਾਪਸ;

	dur = oob & SCX_ਟੁਕੜਾ_OOB_DUR_ਨਕਾਬ;
	ਟੁਕੜਾ = dur == SCX_ਟੁਕੜਾ_OOB_DUR_ਨਕਾਬ ? SCX_ਟੁਕੜਾ_INF : dur;

	ਜੇ (ਟੁਕੜਾ > p->scx.ਟੁਕੜਾ &&
	    unlikely(scx_missing_caps(scx_task_sched(p), cpu_ਦਾ(rq), SCX_CAP_ਅਧਾਰ))) {
		__scx_add_event(scx_task_sched(p), SCX_EV_ਟੁਕੜਾ_DENIED, 1);
		ਵਾਪਸ;
	}

	ਜੇ (unlikely(!ਸੈੱਟ_task_ਟੁਕੜਾ_keep_oob(p, ਟੁਕੜਾ)))
		__scx_add_event(scx_task_sched(p), SCX_EV_ਟੁਕੜਾ_DENIED, 1);
}

/*
 * A dsq insert kfunc doesn't write slice or dsq_vtime. The verdict carries them
 * and they are committed here, at the insertion. A zero @slice keeps the
 * current value, floored at 1 so the task isn't treated as expired.
 */
ਸਥਿਰ ਖਾਲੀ apply_ਟੁਕੜਾ_vtime(ਬਣਤਰ task_struct *p, u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	ਜੇ (ਟੁਕੜਾ) {
		p->scx.ਟੁਕੜਾ = ਟੁਕੜਾ;
		/*
		 * An explicit slice supersedes a pending oob request. A carried
		 * default refill is not an explicit request and must keep it.
		 */
		ਜੇ (!(enq_flags & SCX_ENQ_ਟੁਕੜਾ_DFL))
			clear_task_ਟੁਕੜਾ_oob(p);
	} ਨਹੀਂਤਾਂ ਜੇ (!p->scx.ਟੁਕੜਾ) {
		p->scx.ਟੁਕੜਾ = 1;
	}

	ਜੇ (enq_flags & SCX_ENQ_DSQ_PRIQ)
		p->scx.dsq_vtime = vtime;
}

ਸਥਿਰ ਖਾਲੀ update_curr_scx(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *curr = rq->curr;
	s64 delta_exec;

	/* apply even on 0 delta_exec, callers may still act on the slice */
	apply_task_ਟੁਕੜਾ_oob(rq, curr);

	delta_exec = update_curr_common(rq);
	ਜੇ (unlikely(delta_exec <= 0))
		ਵਾਪਸ;

	ਜੇ (curr->scx.ਟੁਕੜਾ != SCX_ਟੁਕੜਾ_INF)
		curr->scx.ਟੁਕੜਾ -= min_t(u64, curr->scx.ਟੁਕੜਾ, delta_exec);

	ਜੇ (unlikely(curr == scx_rescuee(rq)))
		scx_rescue_charge(rq, delta_exec);

	dl_server_update(&rq->ext_server, delta_exec);
}

ਸਥਿਰ bool scx_dsq_priq_less(ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ_a,
			      ਅਚਲ ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ_b)
{
	ਅਚਲ ਬਣਤਰ task_struct *a =
		container_ਦਾ(ਗੰਢ_a, ਬਣਤਰ task_struct, scx.dsq_priq);
	ਅਚਲ ਬਣਤਰ task_struct *b =
		container_ਦਾ(ਗੰਢ_b, ਬਣਤਰ task_struct, scx.dsq_priq);

	ਵਾਪਸ ਸਮਾਂ_before64(a->scx.dsq_vtime, b->scx.dsq_vtime);
}

ਸਥਿਰ ਖਾਲੀ dsq_inc_nr(ਬਣਤਰ scx_dispatch_q *dsq, ਬਣਤਰ task_struct *p, u64 enq_flags)
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
	ਜੇ (enq_flags & SCX_ENQ_IMMED) {
		ਜੇ (unlikely(dsq->id != SCX_DSQ_LOCAL)) {
			WARN_ON_ONCE(!(enq_flags & SCX_ENQ_GDSQ_FALLBACK));
			ਵਾਪਸ;
		}
		p->scx.flags |= SCX_TASK_IMMED;
	}

	ਜੇ (p->scx.flags & SCX_TASK_IMMED) {
		ਬਣਤਰ rq *rq = container_ਦਾ(dsq, ਬਣਤਰ rq, scx.local_dsq);

		ਜੇ (WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL))
			ਵਾਪਸ;

		rq->scx.nr_immed++;

		/*
		 * If @rq already had other tasks or the current task is not
		 * done yet, @p can't go on the CPU immediately. Re-enqueue.
		 */
		ਜੇ (unlikely(dsq->nr > 1 || !rq_is_open(rq, enq_flags)))
			scx_ਸਮਾਂ_ਸਾਰਣੀ_reenq_local(rq, 0);
	}
}

ਸਥਿਰ ਖਾਲੀ dsq_dec_nr(ਬਣਤਰ scx_dispatch_q *dsq, ਬਣਤਰ task_struct *p)
{
	/* see dsq_inc_nr() */
	WRITE_ONCE(dsq->nr, dsq->nr - 1);

	ਜੇ (p->scx.flags & SCX_TASK_IMMED) {
		ਬਣਤਰ rq *rq = container_ਦਾ(dsq, ਬਣਤਰ rq, scx.local_dsq);

		ਜੇ (WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL) ||
		    WARN_ON_ONCE(rq->scx.nr_immed <= 0))
			ਵਾਪਸ;

		rq->scx.nr_immed--;
	}
}

ਸਥਿਰ ਖਾਲੀ refill_task_ਟੁਕੜਾ_dfl(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p)
{
	/*
	 * A default refill is not an explicit request, so it must not drop a
	 * pending out-of-band one, which is applied when @p next runs.
	 */
	ਸੈੱਟ_task_ਟੁਕੜਾ_keep_oob(p, READ_ONCE(sch->ਟੁਕੜਾ_dfl));
	__scx_add_event(sch, SCX_EV_REFILL_ਟੁਕੜਾ_DFL, 1);
}

/*
 * Return true if @p is moving due to an internal SCX migration, false
 * otherwise.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_scx_migrating(ਬਣਤਰ task_struct *p)
{
	/*
	 * We only need to check sticky_cpu: it is set to the destination
	 * CPU in move_remote_task_to_local_dsq() before deactivate_task()
	 * and cleared when the task is enqueued on the destination, so it
	 * is only non-negative during an internal SCX migration.
	 */
	ਵਾਪਸ p->scx.sticky_cpu >= 0;
}

/* Must be called under the lock serializing @p's custody transfers. */
ਸਥਿਰ bool task_leave_custody(ਬਣਤਰ task_struct *p)
{
	ਜੇ (!(p->scx.flags & SCX_TASK_ਵਿੱਚ_CUSTODY) || task_scx_migrating(p))
		ਵਾਪਸ false;

	p->scx.flags &= ~SCX_TASK_ਵਿੱਚ_CUSTODY;
	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ rq_owned_post_enq(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq,
			      ਬਣਤਰ scx_dispatch_q *dsq, ਬਣਤਰ task_struct *p,
			      u64 enq_flags)
{
	ਜੇ (task_leave_custody(p) && SCX_HAS_OP(sch, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ))
		SCX_CALL_OP_TASK(sch, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ, rq, p, 0);

	/*
	 * Only local inserts get the wakeup treatment below. Rejects kick the
	 * deferred reenq and rescue parks are paced by the rescue timer.
	 */
	ਜੇ (unlikely(dsq->id != SCX_DSQ_LOCAL)) {
		ਜੇ (dsq->id == SCX_DSQ_REJECT)
			ਸਮਾਂ_ਸਾਰਣੀ_deferred_locked(rq);
		ਵਾਪਸ;
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
	ਜੇ (sched_class_above(p->sched_class, rq->ਅਗਲਾ_class))
		wakeup_preempt(rq, p, 0);

	/*
	 * If @rq is in dispatch, the CPU is already vacant and looking for the
	 * next task to run. No need to preempt or trigger resched after moving
	 * @p into its local DSQ.
	 * Note that the wakeup_preempt() above may have already triggered
	 * a resched if @rq->next_class was idle. It's harmless, since
	 * need_resched is cleared immediately after task pick.
	 */
	ਜੇ (rq->scx.flags & SCX_RQ_ਵਿੱਚ_DISPATCH)
		ਵਾਪਸ;

	ਜੇ ((enq_flags & SCX_ENQ_PREEMPT) && p != rq->curr &&
	    rq->curr->sched_class == &ext_sched_class) {
		ਜੇ (likely(scx_ਸੈੱਟ_task_ਟੁਕੜਾ(rq->curr, 0)))
			resched_curr(rq);
		ਨਹੀਂਤਾਂ
			__scx_add_event(sch, SCX_EV_ਟੁਕੜਾ_DENIED, 1);
	}
}

ਸਥਿਰ ਖਾਲੀ scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq,
				 ਬਣਤਰ scx_dispatch_q *dsq, ਬਣਤਰ task_struct *p,
				 u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	bool is_rq_owned = false;

	ਜੇ (dsq->id == SCX_DSQ_LOCAL) {
		dsq = scx_resolve_local_dsq(sch, rq, p, &enq_flags);
		is_rq_owned = true;
	}

	WARN_ON_ONCE(p->scx.dsq || !list_ਖਾਲੀ_ਥਾਵਾਂ(&p->scx.dsq_list.ਗੰਢ));
	WARN_ON_ONCE((p->scx.dsq_flags & SCX_TASK_DSQ_ON_PRIQ) ||
		     !RB_ਖਾਲੀ_ਥਾਵਾਂ_ਗੰਢ(&p->scx.dsq_priq));

	ਜੇ (!is_rq_owned) {
		raw_spin_ਤਾਲਾ_nested(&dsq->ਤਾਲਾ,
			(enq_flags & SCX_ENQ_NESTED) ? SINGLE_ਡੂੰਘਾਈ_NESTING : 0);

		ਜੇ (unlikely(dsq->id == SCX_DSQ_INVALID)) {
			scx_error(sch, "attempting to dispatch to a destroyed dsq");
			/* fall back to the global dsq */
			raw_spin_unlock(&dsq->ਤਾਲਾ);
			dsq = ਲੱਭੋ_global_dsq(sch, task_cpu(p));
			raw_spin_ਤਾਲਾ(&dsq->ਤਾਲਾ);
		}
	}

	ਜੇ (unlikely((dsq->id & SCX_DSQ_FLAG_BUILTIN) &&
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
	ਜੇ (enq_flags & SCX_ENQ_APPLY_ਟੁਕੜਾ)
		apply_ਟੁਕੜਾ_vtime(p, ਟੁਕੜਾ, vtime, enq_flags);

	ਜੇ (enq_flags & SCX_ENQ_DSQ_PRIQ) {
		ਬਣਤਰ rb_ਗੰਢ *rbp;

		/*
		 * A PRIQ DSQ shouldn't be using FIFO enqueueing. As tasks are
		 * linked to both the rbtree and list on PRIQs, this can only be
		 * tested easily when adding the first task.
		 */
		ਜੇ (unlikely(RB_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(&dsq->priq) &&
			     nldsq_ਅਗਲਾ_task(dsq, ਨਿਰਾ, false)))
			scx_error(sch, "DSQ ID 0x%016llx already had FIFO-enqueued tasks",
				  dsq->id);

		p->scx.dsq_flags |= SCX_TASK_DSQ_ON_PRIQ;
		rb_ਜੋੜੋ(&p->scx.dsq_priq, &dsq->priq, scx_dsq_priq_less);

		/*
		 * Find the previous task and insert after it on the list so
		 * that @dsq->list is vtime ordered.
		 */
		rbp = rb_prev(&p->scx.dsq_priq);
		ਜੇ (rbp) {
			ਬਣਤਰ task_struct *prev =
				container_ਦਾ(rbp, ਬਣਤਰ task_struct,
					     scx.dsq_priq);
			list_ਜੋੜੋ(&p->scx.dsq_list.ਗੰਢ, &prev->scx.dsq_list.ਗੰਢ);
			/* first task unchanged - no update needed */
		} ਨਹੀਂਤਾਂ {
			list_ਜੋੜੋ(&p->scx.dsq_list.ਗੰਢ, &dsq->list);
			/* not builtin and new task is at head - use fastpath */
			rcu_ਸੌਂਪੋ_ਸੂਚਕ(dsq->ਪਹਿਲਾ_task, p);
		}
	} ਨਹੀਂਤਾਂ {
		/* a FIFO DSQ shouldn't be using PRIQ enqueuing */
		ਜੇ (unlikely(!RB_ਖਾਲੀ_ਥਾਵਾਂ_ਜੜ੍ਹ(&dsq->priq)))
			scx_error(sch, "DSQ ID 0x%016llx already had PRIQ-enqueued tasks",
				  dsq->id);

		ਜੇ (enq_flags & (SCX_ENQ_ਸਿਰ | SCX_ENQ_PREEMPT)) {
			/* new task inserted at head - use fastpath */
			ਜੇ (dsq_ਪਾਓ_ਸਿਰ(dsq, p) && !(dsq->id & SCX_DSQ_FLAG_BUILTIN))
				rcu_ਸੌਂਪੋ_ਸੂਚਕ(dsq->ਪਹਿਲਾ_task, p);
		} ਨਹੀਂਤਾਂ {
			/*
			 * dsq->list can contain parked BPF iterator cursors, so
			 * list_empty() here isn't a reliable proxy for "no real
			 * task in the DSQ". Test dsq->first_task directly.
			 */
			list_ਜੋੜੋ_ਪੂਛ(&p->scx.dsq_list.ਗੰਢ, &dsq->list);
			ਜੇ (!dsq->ਪਹਿਲਾ_task && !(dsq->id & SCX_DSQ_FLAG_BUILTIN))
				rcu_ਸੌਂਪੋ_ਸੂਚਕ(dsq->ਪਹਿਲਾ_task, p);
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
	ਜੇ (is_rq_owned) {
		rq_owned_post_enq(sch, rq, dsq, p, enq_flags);
	} ਨਹੀਂਤਾਂ {
		bool call_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ = false;

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
		ਜੇ (dsq->id == SCX_DSQ_GLOBAL || dsq->id == SCX_DSQ_BYPASS)
			call_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ = task_leave_custody(p);
		ਨਹੀਂਤਾਂ
			p->scx.flags |= SCX_TASK_ਵਿੱਚ_CUSTODY;

		raw_spin_unlock(&dsq->ਤਾਲਾ);

		ਜੇ (call_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ && SCX_HAS_OP(sch, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ))
			SCX_CALL_OP_TASK(sch, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ, rq, p, 0);
	}

	/*
	 * We're transitioning out of QUEUEING or DISPATCHING. store_release to
	 * match waiters' load_acquire.
	 */
	ਜੇ (enq_flags & SCX_ENQ_CLEAR_OPSS)
		atomic_long_ਸੈੱਟ_release(&p->scx.ops_state, SCX_OPSS_NONE);
}

ਖਾਲੀ scx_task_unlink_from_dsq(ਬਣਤਰ task_struct *p, ਬਣਤਰ scx_dispatch_q *dsq)
{
	WARN_ON_ONCE(list_ਖਾਲੀ_ਥਾਵਾਂ(&p->scx.dsq_list.ਗੰਢ));

	ਜੇ (p->scx.dsq_flags & SCX_TASK_DSQ_ON_PRIQ) {
		rb_erase(&p->scx.dsq_priq, &dsq->priq);
		RB_CLEAR_ਗੰਢ(&p->scx.dsq_priq);
		p->scx.dsq_flags &= ~SCX_TASK_DSQ_ON_PRIQ;
	}

	list_del_init(&p->scx.dsq_list.ਗੰਢ);
	dsq_dec_nr(dsq, p);

	ਜੇ (!(dsq->id & SCX_DSQ_FLAG_BUILTIN) && rcu_access_ਸੂਚਕ(dsq->ਪਹਿਲਾ_task) == p) {
		ਬਣਤਰ task_struct *ਪਹਿਲਾ_task;

		ਪਹਿਲਾ_task = nldsq_ਅਗਲਾ_task(dsq, ਨਿਰਾ, false);
		rcu_ਸੌਂਪੋ_ਸੂਚਕ(dsq->ਪਹਿਲਾ_task, ਪਹਿਲਾ_task);
	}
}

ਖਾਲੀ scx_dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ scx_dispatch_q *dsq = p->scx.dsq;
	bool is_rq_owned = dsq && dsq_is_rq_owned(dsq);

	lockdep_assert_rq_held(rq);

	ਜੇ (!dsq) {
		/*
		 * If !dsq && on-list, @p is on @rq's ddsp_deferred_locals.
		 * Unlinking is all that's needed to cancel.
		 */
		ਜੇ (unlikely(!list_ਖਾਲੀ_ਥਾਵਾਂ(&p->scx.dsq_list.ਗੰਢ)))
			list_del_init(&p->scx.dsq_list.ਗੰਢ);

		/*
		 * When dispatching directly from the BPF scheduler to a local
		 * DSQ, the task isn't associated with any DSQ but
		 * @p->scx.holding_cpu may be set under the protection of
		 * %SCX_OPSS_DISPATCHING.
		 */
		ਜੇ (p->scx.holding_cpu >= 0)
			p->scx.holding_cpu = -1;

		ਵਾਪਸ;
	}

	ਜੇ (!is_rq_owned)
		raw_spin_ਤਾਲਾ(&dsq->ਤਾਲਾ);

	/*
	 * Now that we hold @dsq->lock, @p->holding_cpu and @p->scx.dsq_* can't
	 * change underneath us.
	*/
	ਜੇ (p->scx.holding_cpu < 0) {
		/* @p must still be on @dsq, dequeue */
		scx_task_unlink_from_dsq(p, dsq);
	} ਨਹੀਂਤਾਂ {
		/*
		 * We're racing against dispatch_to_local_dsq() which already
		 * removed @p from @dsq and set @p->scx.holding_cpu. Clear the
		 * holding_cpu which tells dispatch_to_local_dsq() that it lost
		 * the race.
		 */
		WARN_ON_ONCE(!list_ਖਾਲੀ_ਥਾਵਾਂ(&p->scx.dsq_list.ਗੰਢ));
		p->scx.holding_cpu = -1;
	}
	p->scx.dsq = ਨਿਰਾ;

	ਜੇ (!is_rq_owned)
		raw_spin_unlock(&dsq->ਤਾਲਾ);
}

/*
 * Abbreviated version of scx_dispatch_dequeue() that can be used when both
 * @p's rq and dsq are locked.
 */
ਸਥਿਰ ਖਾਲੀ dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_locked(ਬਣਤਰ task_struct *p,
				    ਬਣਤਰ scx_dispatch_q *dsq)
{
	lockdep_assert_rq_held(task_rq(p));
	lockdep_assert_held(&dsq->ਤਾਲਾ);

	scx_task_unlink_from_dsq(p, dsq);
	p->scx.dsq = ਨਿਰਾ;
}

ਸਥਿਰ ਬਣਤਰ scx_dispatch_q *ਲੱਭੋ_dsq_for_dispatch(ਬਣਤਰ scx_sched *sch,
						    ਬਣਤਰ rq *rq, u64 dsq_id,
						    s32 tcpu)
{
	ਬਣਤਰ scx_dispatch_q *dsq;

	ਜੇ (dsq_id == SCX_DSQ_LOCAL)
		ਵਾਪਸ &rq->scx.local_dsq;

	ਜੇ ((dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON) {
		s32 cpu = scx_cpu_ret(sch, dsq_id & SCX_DSQ_LOCAL_CPU_ਨਕਾਬ);

		ਜੇ (!scx_cpu_valid(sch, cpu, "in SCX_DSQ_LOCAL_ON dispatch verdict"))
			ਵਾਪਸ ਲੱਭੋ_global_dsq(sch, tcpu);

		ਵਾਪਸ &cpu_rq(cpu)->scx.local_dsq;
	}

	ਜੇ (dsq_id == SCX_DSQ_GLOBAL)
		dsq = ਲੱਭੋ_global_dsq(sch, tcpu);
	ਨਹੀਂਤਾਂ
		dsq = ਲੱਭੋ_user_dsq(sch, dsq_id);

	/*
	 * Built-in DSQs are never inserted into dsq_hash, so REJECT and RESCUE
	 * hit the error below. They cannot be reached with an ID.
	 */
	ਜੇ (unlikely(!dsq)) {
		scx_error(sch, "non-existent DSQ 0x%llx", dsq_id);
		ਵਾਪਸ ਲੱਭੋ_global_dsq(sch, tcpu);
	}

	ਵਾਪਸ dsq;
}

ਸਥਿਰ ਖਾਲੀ mark_ਸਿੱਧਾ_dispatch(ਬਣਤਰ scx_sched *sch,
				 ਬਣਤਰ task_struct *ddsp_task,
				 ਬਣਤਰ task_struct *p, u64 dsq_id,
				 u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	/*
	 * Mark that dispatch already happened from ops.select_cpu() or
	 * ops.enqueue() by spoiling direct_dispatch_task with a non-NULL value
	 * which can never match a valid task pointer.
	 */
	__this_cpu_write(ਸਿੱਧਾ_dispatch_task, ERR_PTR(-ESRCH));

	/* @p must match the task on the enqueue path */
	ਜੇ (unlikely(p != ddsp_task)) {
		ਜੇ (IS_ERR(ddsp_task))
			scx_error(sch, "%s[%d] already direct-dispatched",
				  p->comm, p->pid);
		ਨਹੀਂਤਾਂ
			scx_error(sch, "scheduling for %s[%d] but trying to direct-dispatch %s[%d]",
				  ddsp_task->comm, ddsp_task->pid,
				  p->comm, p->pid);
		ਵਾਪਸ;
	}

	WARN_ON_ONCE(p->scx.ddsp_dsq_id != SCX_DSQ_INVALID);
	WARN_ON_ONCE(p->scx.ddsp_enq_flags);

	p->scx.ddsp_ਟੁਕੜਾ = ਟੁਕੜਾ;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ clear_ਸਿੱਧਾ_dispatch(ਬਣਤਰ task_struct *p)
{
	p->scx.ddsp_dsq_id = SCX_DSQ_INVALID;
	p->scx.ddsp_enq_flags = 0;
}

ਸਥਿਰ ਖਾਲੀ ਸਿੱਧਾ_dispatch(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p,
			    u64 enq_flags)
{
	ਬਣਤਰ rq *rq = task_rq(p);
	ਬਣਤਰ scx_dispatch_q *dsq =
		ਲੱਭੋ_dsq_for_dispatch(sch, rq, p->scx.ddsp_dsq_id, task_cpu(p));
	u64 ddsp_enq_flags, ਟੁਕੜਾ, vtime;

	p->scx.ddsp_enq_flags |= enq_flags;

	/*
	 * We are in the enqueue path with @rq locked and pinned, and thus can't
	 * double lock a remote rq and enqueue to its local DSQ. For
	 * DSQ_LOCAL_ON verdicts targeting the local DSQ of a remote CPU, defer
	 * the enqueue so that it's executed when @rq can be unlocked.
	 */
	ਜੇ (dsq->id == SCX_DSQ_LOCAL && dsq != &rq->scx.local_dsq) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ opss;

		opss = atomic_long_read(&p->scx.ops_state) & SCX_OPSS_STATE_ਨਕਾਬ;

		ਚੋਣ (opss & SCX_OPSS_STATE_ਨਕਾਬ) {
		ਹਾਲਤ SCX_OPSS_NONE:
			ਰੋਕੋ;
		ਹਾਲਤ SCX_OPSS_QUEUEING:
			/*
			 * As @p was never passed to the BPF side, _release is
			 * not strictly necessary. Still do it for consistency.
			 */
			atomic_long_ਸੈੱਟ_release(&p->scx.ops_state, SCX_OPSS_NONE);
			ਰੋਕੋ;
		ਮੂਲ:
			WARN_ONCE(true, "sched_ext: %s[%d] has invalid ops state 0x%lx in direct_dispatch()",
				  p->comm, p->pid, opss);
			atomic_long_ਸੈੱਟ_release(&p->scx.ops_state, SCX_OPSS_NONE);
			ਰੋਕੋ;
		}

		WARN_ON_ONCE(p->scx.dsq || !list_ਖਾਲੀ_ਥਾਵਾਂ(&p->scx.dsq_list.ਗੰਢ));
		list_ਜੋੜੋ_ਪੂਛ(&p->scx.dsq_list.ਗੰਢ,
			      &rq->scx.ddsp_deferred_locals);
		ਸਮਾਂ_ਸਾਰਣੀ_deferred_locked(rq);
		ਵਾਪਸ;
	}

	ddsp_enq_flags = p->scx.ddsp_enq_flags;
	ਟੁਕੜਾ = p->scx.ddsp_ਟੁਕੜਾ;
	vtime = p->scx.ddsp_vtime;
	clear_ਸਿੱਧਾ_dispatch(p);

	scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, rq, dsq, p, ਟੁਕੜਾ, vtime,
			     ddsp_enq_flags | SCX_ENQ_APPLY_ਟੁਕੜਾ | SCX_ENQ_CLEAR_OPSS);
}

bool scx_rq_online(ਬਣਤਰ rq *rq)
{
	/*
	 * Test both cpu_active() and %SCX_RQ_ONLINE. %SCX_RQ_ONLINE indicates
	 * the online state as seen from the BPF scheduler. cpu_active() test
	 * guarantees that, if this function returns %true, %SCX_RQ_ONLINE will
	 * stay set until the current scheduling operation is complete even if
	 * we aren't locking @rq.
	 */
	ਵਾਪਸ likely((rq->scx.flags & SCX_RQ_ONLINE) && cpu_ਸਰਗਰਮ(cpu_ਦਾ(rq)));
}

ਖਾਲੀ scx_do_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 enq_flags,
			 ਪੂਰਨ sticky_cpu)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
	ਬਣਤਰ task_struct **ddsp_taskp;
	ਬਣਤਰ scx_dispatch_q *dsq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ qseq;

	WARN_ON_ONCE(!(p->scx.flags & SCX_TASK_QUEUED));

	/* internal movements - rq migration / RESTORE */
	ਜੇ (sticky_cpu == cpu_ਦਾ(rq))
		ਜਾਓ local_norefill;

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
	ਜੇ (enq_flags & SCX_ENQ_REENQ) {
		ਜੇ (++p->scx.reenq_cnt > 1)
			__scx_add_event(sch, SCX_EV_REENQ_REPEAT, 1);

		ਜੇ (unlikely(p->scx.reenq_cnt > SCX_REENQ_MAX_REPEAT)) {
			__scx_exit(sch, SCX_EXIT_ERROR_REENQ, 0, cpu_ਦਾ(rq),
				   "%s[%d] reenqueued %u times without running",
				   p->comm, p->pid, p->scx.reenq_cnt);
			ਵਾਪਸ;
		}
	}

	/*
	 * If !scx_rq_online(), we already told the BPF scheduler that the CPU
	 * is offline and are just running the hotplug path. Don't bother the
	 * BPF scheduler.
	 */
	ਜੇ (!scx_rq_online(rq))
		ਜਾਓ local;

	ਜੇ (scx_bypassing(sch, cpu_ਦਾ(rq))) {
		__scx_add_event(sch, SCX_EV_BYPASS_DISPATCH, 1);
		ਜਾਓ bypass;
	}

	ਜੇ (p->scx.ddsp_dsq_id != SCX_DSQ_INVALID)
		ਜਾਓ ਸਿੱਧਾ;

	/* see %SCX_OPS_ENQ_EXITING */
	ਜੇ (!(sch->ops.flags & SCX_OPS_ENQ_EXITING) &&
	    unlikely(p->flags & PF_EXITING)) {
		__scx_add_event(sch, SCX_EV_ENQ_SKIP_EXITING, 1);
		enq_flags |= SCX_ENQ_RESCUE;	/* avoid looping on cap rejection */
		ਜਾਓ local;
	}

	/* see %SCX_OPS_ENQ_MIGRATION_DISABLED */
	ਜੇ (!(sch->ops.flags & SCX_OPS_ENQ_MIGRATION_DISABLED) &&
	    is_migration_disabled(p)) {
		__scx_add_event(sch, SCX_EV_ENQ_SKIP_MIGRATION_DISABLED, 1);
		ਜਾਓ local;
	}

	ਜੇ (unlikely(!SCX_HAS_OP(sch, ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ)))
		ਜਾਓ global;

	/*
	 * DSQ bypass didn't trigger, enqueue on the BPF scheduler. Wrap the
	 * per-task qseq counter where the QSEQ field wraps and skip 0, which is
	 * what scx_bpf_dsq_insert() records for a task in NONE or DISPATCHING.
	 */
	p->scx.ops_qseq = ((p->scx.ops_qseq + 1) &
			   (SCX_OPSS_QSEQ_ਨਕਾਬ >> SCX_OPSS_QSEQ_ਖਿਸਕਾਓ)) ?: 1;
	qseq = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)p->scx.ops_qseq << SCX_OPSS_QSEQ_ਖਿਸਕਾਓ;

	WARN_ON_ONCE(atomic_long_read(&p->scx.ops_state) != SCX_OPSS_NONE);
	atomic_long_ਸੈੱਟ(&p->scx.ops_state, SCX_OPSS_QUEUEING | qseq);

	ddsp_taskp = this_cpu_ptr(&ਸਿੱਧਾ_dispatch_task);
	WARN_ON_ONCE(*ddsp_taskp);
	*ddsp_taskp = p;

	SCX_CALL_OP_TASK(sch, ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ, rq, p, enq_flags);

	*ddsp_taskp = ਨਿਰਾ;
	ਜੇ (p->scx.ddsp_dsq_id != SCX_DSQ_INVALID)
		ਜਾਓ ਸਿੱਧਾ;

	/*
	 * Task is now in BPF scheduler's custody. Set %SCX_TASK_IN_CUSTODY
	 * so ops.dequeue() is called when it leaves custody.
	 */
	p->scx.flags |= SCX_TASK_ਵਿੱਚ_CUSTODY;

	/*
	 * If not directly dispatched, QUEUEING isn't clear yet and dispatch or
	 * dequeue may be waiting. The store_release matches their load_acquire.
	 */
	atomic_long_ਸੈੱਟ_release(&p->scx.ops_state, SCX_OPSS_QUEUED | qseq);
	ਵਾਪਸ;

ਸਿੱਧਾ:
	ਸਿੱਧਾ_dispatch(sch, p, enq_flags);
	ਵਾਪਸ;
local_norefill:
	scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, rq, &rq->scx.local_dsq, p, 0, 0, enq_flags);
	ਵਾਪਸ;
local:
	dsq = &rq->scx.local_dsq;
	ਜਾਓ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ;
global:
	dsq = ਲੱਭੋ_global_dsq(sch, task_cpu(p));
	ਜਾਓ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ;
bypass:
	dsq = bypass_enq_ਨਿਸ਼ਾਨਾ_dsq(sch, task_cpu(p));
	ਜਾਓ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ;

ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ:
	refill_task_ਟੁਕੜਾ_dfl(sch, p);
	clear_ਸਿੱਧਾ_dispatch(p);
	scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, rq, dsq, p, 0, 0, enq_flags);
}

ਸਥਿਰ bool task_runnable(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ !list_ਖਾਲੀ_ਥਾਵਾਂ(&p->scx.runnable_ਗੰਢ);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_task_runnable(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	lockdep_assert_rq_held(rq);

	ਜੇ (p->scx.flags & SCX_TASK_RESET_RUNNABLE_AT) {
		p->scx.runnable_at = jiffies;
		p->scx.flags &= ~SCX_TASK_RESET_RUNNABLE_AT;
	}

	/*
	 * list_add_tail() must be used. scx_bypass() and rq_offline_scx()
	 * depend on tasks being appended to the runnable_list.
	 */
	list_ਜੋੜੋ_ਪੂਛ(&p->scx.runnable_ਗੰਢ, &rq->scx.runnable_list);

	/*
	 * Record the rq @p is runnable on, maintained under the rq lock so it
	 * stays valid unlike task_cpu(), which a remote wakeup can move under
	 * pi_lock alone.
	 */
	WRITE_ONCE(p->scx.runnable_cpu, cpu_ਦਾ(rq));
}

ਸਥਿਰ ਖਾਲੀ clr_task_runnable(ਬਣਤਰ task_struct *p, bool reset_runnable_at)
{
	list_del_init(&p->scx.runnable_ਗੰਢ);
	WRITE_ONCE(p->scx.runnable_cpu, -1);
	ਜੇ (reset_runnable_at) {
		p->scx.flags |= SCX_TASK_RESET_RUNNABLE_AT;
		p->scx.reenq_cnt = 0;
	}
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ core_enq_flags)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
	ਪੂਰਨ sticky_cpu = p->scx.sticky_cpu;
	u64 enq_flags = core_enq_flags | rq->scx.remote_activate_enq_flags;

	/*
	 * An SCX-internal migration ends on arrival. Clear sticky_cpu so @p can
	 * leave custody when inserted into the destination DSQ.
	 */
	ਜੇ (sticky_cpu >= 0)
		p->scx.sticky_cpu = -1;

	/*
	 * SCX_RQ_IN_WAKEUP promises a task_woken_scx() call once this enqueue
	 * returns. Only the core's wakeup path delivers one. The flags stashed
	 * for a remote activation may carry the wakeup bit without it.
	 */
	ਜੇ (core_enq_flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP)
		rq->scx.flags |= SCX_RQ_ਵਿੱਚ_WAKEUP;

	/*
	 * Restoring a running task will be immediately followed by
	 * set_next_task_scx() which expects the task to not be on the BPF
	 * scheduler as tasks can only start running through local DSQs. Force
	 * direct-dispatch into the local DSQ by setting the sticky_cpu. Mark
	 * IGNORE_CAPS to force entry into the local DSQ.
	 */
	ਜੇ (unlikely(enq_flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_RESTORE) && task_current(rq, p)) {
		sticky_cpu = cpu_ਦਾ(rq);
		enq_flags |= SCX_ENQ_IGNORE_CAPS;
	}

	ਜੇ (p->scx.flags & SCX_TASK_QUEUED) {
		WARN_ON_ONCE(!task_runnable(p));
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਸੈੱਟ_task_runnable(rq, p);
	p->scx.flags |= SCX_TASK_QUEUED;
	rq->scx.nr_running++;
	ਜੋੜੋ_nr_running(rq, 1);

	ਜੇ (SCX_HAS_OP(sch, runnable) && !task_on_rq_migrating(p))
		SCX_CALL_OP_TASK(sch, runnable, rq, p, enq_flags);

	/* Start dl_server if this is the first task being enqueued */
	ਜੇ (rq->scx.nr_running == 1)
		dl_server_ਸ਼ੁਰੂ(&rq->ext_server);

	scx_do_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, enq_flags, sticky_cpu);
ਬਾਹਰਲਾ:
	rq->scx.flags &= ~SCX_RQ_ਵਿੱਚ_WAKEUP;

	ਜੇ ((enq_flags & SCX_ENQ_CPU_SELECTED) &&
	    unlikely(cpu_ਦਾ(rq) != p->scx.selected_cpu))
		__scx_add_event(sch, SCX_EV_SELECT_CPU_FALLBACK, 1);
}

ਸਥਿਰ ਖਾਲੀ ops_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 deq_flags)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ opss;

	/* dequeue is always temporary, don't reset runnable_at */
	clr_task_runnable(p, false);

retry:
	/* acquire ensures that we see the preceding updates on QUEUED */
	opss = atomic_long_read_acquire(&p->scx.ops_state);

	ਚੋਣ (opss & SCX_OPSS_STATE_ਨਕਾਬ) {
	ਹਾਲਤ SCX_OPSS_NONE:
		ਰੋਕੋ;
	ਹਾਲਤ SCX_OPSS_QUEUEING:
		/*
		 * QUEUEING is started and finished while holding @p's rq lock.
		 * As we're holding the rq lock now, we shouldn't see QUEUEING.
		 */
		BUG();
	ਹਾਲਤ SCX_OPSS_QUEUED:
		/*
		 * A queued task must always be in BPF scheduler's custody. If
		 * SCX_TASK_IN_CUSTODY is clear, finish_dispatch() on another
		 * CPU has already passed task_leave_custody() (which clears the
		 * flag), but has not yet written SCX_OPSS_NONE. That final
		 * store does not require this rq's lock, so retrying with
		 * cpu_relax() is bounded: we will observe NONE (or DISPATCHING,
		 * handled by the fallthrough) on a subsequent iteration.
		 */
		ਜੇ (unlikely(!(READ_ONCE(p->scx.flags) & SCX_TASK_ਵਿੱਚ_CUSTODY))) {
			cpu_relax();
			ਜਾਓ retry;
		}

		ਜੇ (atomic_long_try_cmpxchg(&p->scx.ops_state, &opss,
					    SCX_OPSS_NONE))
			ਰੋਕੋ;
		fallthrough;
	ਹਾਲਤ SCX_OPSS_DISPATCHING:
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
		ਰੋਕੋ;
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
	ਜੇ (task_leave_custody(p) && SCX_HAS_OP(sch, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ))
		SCX_CALL_OP_TASK(sch, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ, rq, p, deq_flags);
}

ਸਥਿਰ bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ core_deq_flags)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
	u64 deq_flags = core_deq_flags;

	/*
	 * Set %SCX_DEQ_SCHED_CHANGE when the dequeue is due to a property
	 * change (not sleep).
	 */
	ਜੇ (!(deq_flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP))
		deq_flags |= SCX_DEQ_SCHED_CHANGE;

	ਜੇ (!(p->scx.flags & SCX_TASK_QUEUED)) {
		WARN_ON_ONCE(task_runnable(p));
		ਵਾਪਸ true;
	}

	ops_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p, deq_flags);

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
	ਜੇ (task_current(rq, p) &&
	    (SCX_HAS_OP(sch, stopping) || unlikely(p == scx_rescuee(rq)))) {
		update_curr_scx(rq);
		ਜੇ (SCX_HAS_OP(sch, stopping))
			SCX_CALL_OP_TASK(sch, stopping, rq, p, false);
	}

	ਜੇ (SCX_HAS_OP(sch, quiescent) && !task_on_rq_migrating(p))
		SCX_CALL_OP_TASK(sch, quiescent, rq, p, deq_flags);

	ਜੇ (deq_flags & SCX_DEQ_SLEEP)
		p->scx.flags |= SCX_TASK_DEQD_FOR_SLEEP;
	ਨਹੀਂਤਾਂ
		p->scx.flags &= ~SCX_TASK_DEQD_FOR_SLEEP;

	p->scx.flags &= ~SCX_TASK_QUEUED;
	rq->scx.nr_running--;
	sub_nr_running(rq, 1);

	scx_dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p);

	/* see scx_task_slice_ended() for the save/restore exception */
	ਜੇ (!((deq_flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE) && task_current(rq, p)))
		scx_task_ਟੁਕੜਾ_ended(rq, p);

	clear_ਸਿੱਧਾ_dispatch(p);
	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ yield_task_scx(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *p = rq->donor;
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);

	/* a yield gives the slice up */
	scx_task_ਟੁਕੜਾ_ended(rq, p);

	ਜੇ (SCX_HAS_OP(sch, yield))
		SCX_CALL_OP_2TASKS_RET(sch, yield, rq, p, ਨਿਰਾ);
	ਨਹੀਂਤਾਂ
		scx_ਸੈੱਟ_task_ਟੁਕੜਾ(p, 0);
}

ਸਥਿਰ bool yield_ਮੰਜ਼ਿਲ_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *ਮੰਜ਼ਿਲ)
{
	ਬਣਤਰ task_struct *from = rq->donor;
	ਬਣਤਰ scx_sched *sch = scx_task_sched(from);

	/* like a plain yield, giving the slice up ends the protection */
	scx_task_ਟੁਕੜਾ_ended(rq, from);

	ਜੇ (SCX_HAS_OP(sch, yield) && sch == scx_task_sched(ਮੰਜ਼ਿਲ))
		ਵਾਪਸ SCX_CALL_OP_2TASKS_RET(sch, yield, rq, from, ਮੰਜ਼ਿਲ);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ wakeup_preempt_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ wake_flags)
{
	/*
	 * Preemption between SCX tasks is implemented by resetting the victim
	 * task's slice to 0 and triggering reschedule on the target CPU.
	 * Nothing to do.
	 */
	ਜੇ (p->sched_class == &ext_sched_class)
		ਵਾਪਸ;

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
	ਜੇ (rq->scx.nr_immed)
		scx_ਸਮਾਂ_ਸਾਰਣੀ_reenq_local(rq, 0);
}

ਖਾਲੀ scx_move_local_task_ਮੰਜ਼ਿਲ_local_dsq(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p,
				      u64 enq_flags, ਬਣਤਰ rq *dst_rq)
{
	ਬਣਤਰ scx_dispatch_q *dst_dsq;

	lockdep_assert_rq_held(dst_rq);

	WARN_ON_ONCE(p->scx.holding_cpu >= 0);
	scx_wait_dispatching(p);

	dst_dsq = scx_resolve_local_dsq(sch, dst_rq, p, &enq_flags);

	ਜੇ (enq_flags & (SCX_ENQ_ਸਿਰ | SCX_ENQ_PREEMPT))
		dsq_ਪਾਓ_ਸਿਰ(dst_dsq, p);
	ਨਹੀਂਤਾਂ
		list_ਜੋੜੋ_ਪੂਛ(&p->scx.dsq_list.ਗੰਢ, &dst_dsq->list);

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
ਸਥਿਰ ਖਾਲੀ move_remote_task_ਮੰਜ਼ਿਲ_local_dsq(ਬਣਤਰ scx_sched *sch,
					  ਬਣਤਰ task_struct *p, u64 enq_flags,
					  ਬਣਤਰ rq *src_rq, ਬਣਤਰ rq *dst_rq)
{
	lockdep_assert_rq_held(src_rq);

	/*
	 * Set sticky_cpu before deactivate_task() to properly mark the
	 * beginning of an SCX-internal migration.
	 */
	p->scx.sticky_cpu = cpu_ਦਾ(dst_rq);
	deactivate_task(src_rq, p, 0);
	ਸੈੱਟ_task_cpu(p, cpu_ਦਾ(dst_rq));

	switch_rq_ਤਾਲਾ(src_rq, dst_rq);

	/*
	 * activate_task() below truncates enq_flags to 32 bits and re-derives
	 * @p's owner, dropping our scx flags and the placing @sch. We own @rq,
	 * so stash both across the call. The enqueue reads them back, keeping
	 * the scx flags and checking caps against the placer, not the owner.
	 */
	WARN_ON_ONCE(!cpumask_test_cpu(cpu_ਦਾ(dst_rq), p->cpus_ptr));
	WARN_ON_ONCE(dst_rq->scx.remote_activate_enq_flags ||
		     dst_rq->scx.remote_activate_sch);
	dst_rq->scx.remote_activate_enq_flags = enq_flags;
	dst_rq->scx.remote_activate_sch = sch;
	activate_task(dst_rq, p, 0);
	dst_rq->scx.remote_activate_enq_flags = 0;
	dst_rq->scx.remote_activate_sch = ਨਿਰਾ;
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
ਸਥਿਰ bool task_can_ਲੜੀਵਾਰ_on_remote_rq(ਬਣਤਰ scx_sched *sch,
				      ਬਣਤਰ task_struct *p, ਬਣਤਰ rq *rq,
				      bool enforce)
{
	s32 cpu = cpu_ਦਾ(rq);

	/*
	 * To prevent races with @p still running on its old CPU while switching
	 * out, make sure we're holding @p's rq lock so as not to risk
	 * erroneously killing the BPF scheduler.
	 */
	ਜੇ (enforce)
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
	ਜੇ (unlikely(is_migration_disabled(p))) {
		ਜੇ (enforce)
			scx_error(sch, "SCX_DSQ_LOCAL[_ON] cannot move migration disabled %s[%d] from CPU %d to %d",
				  p->comm, p->pid, task_cpu(p), cpu);
		ਵਾਪਸ false;
	}

	/*
	 * We don't require the BPF scheduler to avoid dispatching to offline
	 * CPUs mostly for convenience but also because CPUs can go offline
	 * between scx_bpf_dsq_insert() calls and here. Trigger error iff the
	 * picked CPU is outside the allowed mask.
	 */
	ਜੇ (!task_allowed_on_cpu(p, cpu)) {
		ਜੇ (enforce)
			scx_error(sch, "SCX_DSQ_LOCAL[_ON] target CPU %d not allowed for %s[%d]",
				  cpu, p->comm, p->pid);
		ਵਾਪਸ false;
	}

	ਜੇ (!scx_rq_online(rq)) {
		ਜੇ (enforce)
			__scx_add_event(sch, SCX_EV_DISPATCH_LOCAL_DSQ_OFFLINE, 1);
		ਵਾਪਸ false;
	}

	ਵਾਪਸ true;
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
ਸਥਿਰ bool unlink_dsq_and_switch_rq_ਤਾਲਾ(ਬਣਤਰ task_struct *p,
					  ਬਣਤਰ scx_dispatch_q *dsq,
					  ਬਣਤਰ rq *locked_rq,
					  ਬਣਤਰ rq *src_rq)
{
	s32 cpu = raw_smp_processor_id();

	lockdep_assert_held(&dsq->ਤਾਲਾ);
	lockdep_assert_rq_held(locked_rq);

	WARN_ON_ONCE(p->scx.holding_cpu >= 0);
	scx_task_unlink_from_dsq(p, dsq);
	p->scx.holding_cpu = cpu;

	raw_spin_unlock(&dsq->ਤਾਲਾ);
	switch_rq_ਤਾਲਾ(locked_rq, src_rq);

	/* task_rq couldn't have changed if we're still the holding cpu */
	ਜੇ (likely(p->scx.holding_cpu == cpu) && !WARN_ON_ONCE(src_rq != task_rq(p))) {
		scx_wait_dispatching(p);
		ਵਾਪਸ true;
	}

	ਵਾਪਸ false;
}

ਸਥਿਰ bool consume_remote_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *this_rq,
				ਬਣਤਰ task_struct *p, u64 enq_flags,
				ਬਣਤਰ scx_dispatch_q *dsq, ਬਣਤਰ rq *src_rq)
{
	ਜੇ (unlink_dsq_and_switch_rq_ਤਾਲਾ(p, dsq, this_rq, src_rq)) {
		move_remote_task_ਮੰਜ਼ਿਲ_local_dsq(sch, p, enq_flags, src_rq, this_rq);
		ਵਾਪਸ true;
	} ਨਹੀਂਤਾਂ {
		switch_rq_ਤਾਲਾ(src_rq, this_rq);
		ਵਾਪਸ false;
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
ਸਥਿਰ ਬਣਤਰ rq *move_task_between_dsqs(ਬਣਤਰ scx_sched *sch,
					 ਬਣਤਰ task_struct *p, u64 enq_flags,
					 ਬਣਤਰ scx_dispatch_q *src_dsq,
					 ਬਣਤਰ scx_dispatch_q *dst_dsq)
{
	ਬਣਤਰ rq *src_rq = task_rq(p), *dst_rq;

	BUG_ON(src_dsq->id == SCX_DSQ_LOCAL);
	lockdep_assert_held(&src_dsq->ਤਾਲਾ);
	lockdep_assert_rq_held(src_rq);

	scx_wait_dispatching(p);

	ਜੇ (dst_dsq->id == SCX_DSQ_LOCAL) {
		dst_rq = container_ਦਾ(dst_dsq, ਬਣਤਰ rq, scx.local_dsq);
		ਜੇ (src_rq != dst_rq &&
		    unlikely(!task_can_ਲੜੀਵਾਰ_on_remote_rq(sch, p, dst_rq, true))) {
			dst_dsq = ਲੱਭੋ_global_dsq(sch, task_cpu(p));
			dst_rq = src_rq;
			enq_flags |= SCX_ENQ_GDSQ_FALLBACK;
		}
	} ਨਹੀਂਤਾਂ {
		/* no need to migrate if destination is a non-local DSQ */
		dst_rq = src_rq;
	}

	/*
	 * Move @p into $dst_dsq. If $dst_dsq is the local DSQ of a different
	 * CPU, @p will be migrated.
	 */
	ਜੇ (dst_dsq->id == SCX_DSQ_LOCAL) {
		/* @p is going from a non-local DSQ to a local DSQ */
		ਜੇ (src_rq == dst_rq) {
			scx_task_unlink_from_dsq(p, src_dsq);
			raw_spin_unlock(&src_dsq->ਤਾਲਾ);
			scx_move_local_task_ਮੰਜ਼ਿਲ_local_dsq(sch, p, enq_flags, dst_rq);
		} ਨਹੀਂਤਾਂ {
			raw_spin_unlock(&src_dsq->ਤਾਲਾ);
			move_remote_task_ਮੰਜ਼ਿਲ_local_dsq(sch, p, enq_flags, src_rq, dst_rq);
		}
	} ਨਹੀਂਤਾਂ {
		/*
		 * @p is going from a non-local DSQ to a non-local DSQ. As
		 * $src_dsq is already locked, do an abbreviated dequeue.
		 */
		dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_locked(p, src_dsq);
		raw_spin_unlock(&src_dsq->ਤਾਲਾ);

		scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, dst_rq, dst_dsq, p, 0, 0, enq_flags);
	}

	ਵਾਪਸ dst_rq;
}

bool scx_consume_dispatch_q(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq,
			    ਬਣਤਰ scx_dispatch_q *dsq, u64 enq_flags)
{
	ਬਣਤਰ task_struct *p;
retry:
	/*
	 * The caller can't expect to successfully consume a task if the task's
	 * addition to @dsq isn't guaranteed to be visible somehow. Test
	 * @dsq->list without locking and skip if it seems empty.
	 */
	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&dsq->list))
		ਵਾਪਸ false;

	raw_spin_ਤਾਲਾ(&dsq->ਤਾਲਾ);

	nldsq_for_each_task(p, dsq) {
		ਬਣਤਰ rq *task_rq = task_rq(p);

		/*
		 * This loop can lead to multiple lockup scenarios, e.g. the BPF
		 * scheduler can put an enormous number of affinitized tasks into
		 * a contended DSQ, or the outer retry loop can repeatedly race
		 * against scx_bypass() dequeueing tasks from @dsq trying to put
		 * the system into the bypass mode. This can easily live-lock the
		 * machine. If aborting, exit from all non-bypass DSQs.
		 */
		ਜੇ (unlikely(READ_ONCE(sch->aborting)) && dsq->id != SCX_DSQ_BYPASS)
			ਰੋਕੋ;

		ਜੇ (rq == task_rq) {
			scx_task_unlink_from_dsq(p, dsq);
			raw_spin_unlock(&dsq->ਤਾਲਾ);
			scx_move_local_task_ਮੰਜ਼ਿਲ_local_dsq(sch, p, enq_flags, rq);
			ਵਾਪਸ true;
		}

		ਜੇ (task_can_ਲੜੀਵਾਰ_on_remote_rq(sch, p, rq, false)) {
			ਜੇ (likely(consume_remote_task(sch, rq, p, enq_flags, dsq, task_rq)))
				ਵਾਪਸ true;
			ਜਾਓ retry;
		}
	}

	raw_spin_unlock(&dsq->ਤਾਲਾ);
	ਵਾਪਸ false;
}

bool scx_consume_global_dsq(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq)
{
	ਪੂਰਨ ਗੰਢ = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu_ਦਾ(rq));

	ਵਾਪਸ scx_consume_dispatch_q(sch, rq, &sch->pnode[ਗੰਢ]->global_dsq, 0);
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
ਸਥਿਰ ਖਾਲੀ dispatch_ਮੰਜ਼ਿਲ_local_dsq(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq,
				  ਬਣਤਰ scx_dispatch_q *dst_dsq, ਬਣਤਰ task_struct *p,
				  u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	ਬਣਤਰ rq *src_rq = task_rq(p);
	ਬਣਤਰ rq *dst_rq = container_ਦਾ(dst_dsq, ਬਣਤਰ rq, scx.local_dsq);
	ਬਣਤਰ rq *locked_rq = rq;

	/*
	 * We're synchronized against dequeue through DISPATCHING. As @p can't
	 * be dequeued, its task_rq and cpus_allowed are stable too.
	 *
	 * If dispatching to @rq that @p is already on, no lock dancing needed.
	 */
	ਜੇ (rq == src_rq && rq == dst_rq) {
		scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, rq, dst_dsq, p, ਟੁਕੜਾ, vtime,
				     enq_flags | SCX_ENQ_APPLY_ਟੁਕੜਾ | SCX_ENQ_CLEAR_OPSS);
		ਵਾਪਸ;
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
	atomic_long_ਸੈੱਟ_release(&p->scx.ops_state, SCX_OPSS_NONE);

	/* switch to @src_rq lock */
	ਜੇ (locked_rq != src_rq) {
		switch_rq_ਤਾਲਾ(locked_rq, src_rq);
		locked_rq = src_rq;
	}

	/* task_rq couldn't have changed if we're still the holding cpu */
	ਜੇ (likely(p->scx.holding_cpu == raw_smp_processor_id()) &&
	    !WARN_ON_ONCE(src_rq != task_rq(p))) {
		bool fallback = false;
		/*
		 * If @p is staying on the same rq, there's no need to go
		 * through the full deactivate/activate cycle. Optimize by
		 * abbreviating move_remote_task_to_local_dsq().
		 */
		ਜੇ (src_rq == dst_rq) {
			p->scx.holding_cpu = -1;
			scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, dst_rq, &dst_rq->scx.local_dsq, p,
					     ਟੁਕੜਾ, vtime, enq_flags | SCX_ENQ_APPLY_ਟੁਕੜਾ);
		} ਨਹੀਂਤਾਂ ਜੇ (unlikely(!task_can_ਲੜੀਵਾਰ_on_remote_rq(sch, p, dst_rq, true))) {
			p->scx.holding_cpu = -1;
			fallback = true;
			scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, src_rq, ਲੱਭੋ_global_dsq(sch, task_cpu(p)),
					     p, ਟੁਕੜਾ, vtime,
					     enq_flags | SCX_ENQ_APPLY_ਟੁਕੜਾ |
					     SCX_ENQ_GDSQ_FALLBACK);
		} ਨਹੀਂਤਾਂ {
			apply_ਟੁਕੜਾ_vtime(p, ਟੁਕੜਾ, vtime, enq_flags);
			move_remote_task_ਮੰਜ਼ਿਲ_local_dsq(sch, p, enq_flags, src_rq, dst_rq);
			/* task has been moved to dst_rq, which is now locked */
			locked_rq = dst_rq;
		}

		/* if the destination CPU is idle, wake it up */
		ਜੇ (!fallback && sched_class_above(p->sched_class, dst_rq->curr->sched_class))
			resched_curr(dst_rq);
	}

	/* switch back to @rq lock */
	ਜੇ (locked_rq != rq)
		switch_rq_ਤਾਲਾ(locked_rq, rq);
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
ਸਥਿਰ ਖਾਲੀ ਅੰਤ_dispatch(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ qseq_at_dispatch, u64 dsq_id,
			    u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	ਬਣਤਰ scx_dispatch_q *dsq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ opss;

retry:
	/*
	 * No need for _acquire here. @p is accessed only after a successful
	 * try_cmpxchg to DISPATCHING.
	 */
	opss = atomic_long_read(&p->scx.ops_state);

	ਚੋਣ (opss & SCX_OPSS_STATE_ਨਕਾਬ) {
	ਹਾਲਤ SCX_OPSS_DISPATCHING:
	ਹਾਲਤ SCX_OPSS_NONE:
		/* someone else already got to it */
		ਵਾਪਸ;
	ਹਾਲਤ SCX_OPSS_QUEUED:
		/*
		 * If qseq doesn't match, @p has gone through at least one
		 * dispatch/dequeue and re-enqueue cycle between
		 * scx_bpf_dsq_insert() and here and we have no claim on it.
		 */
		ਜੇ ((opss & SCX_OPSS_QSEQ_ਨਕਾਬ) != qseq_at_dispatch)
			ਵਾਪਸ;

		/* see SCX_EV_INSERT_NOT_OWNED definition */
		ਜੇ (unlikely(!scx_task_on_sched(sch, p))) {
			__scx_add_event(sch, SCX_EV_ਪਾਓ_NOT_OWNED, 1);
			ਵਾਪਸ;
		}

		/*
		 * While we know @p is accessible, we don't yet have a claim on
		 * it - the BPF scheduler is allowed to dispatch tasks
		 * spuriously and there can be a racing dequeue attempt. Let's
		 * claim @p by atomically transitioning it from QUEUED to
		 * DISPATCHING.
		 */
		ਜੇ (likely(atomic_long_try_cmpxchg(&p->scx.ops_state, &opss,
						   SCX_OPSS_DISPATCHING)))
			ਰੋਕੋ;
		ਜਾਓ retry;
	ਹਾਲਤ SCX_OPSS_QUEUEING:
		/*
		 * scx_do_enqueue_task() is in the process of transferring the
		 * task to the BPF scheduler while holding @p's rq lock. As we
		 * aren't holding any kernel or BPF resource that the enqueue
		 * path may depend upon, it's safe to wait.
		 */
		wait_ops_state(p, opss);
		ਜਾਓ retry;
	}

	BUG_ON(!(p->scx.flags & SCX_TASK_QUEUED));

	dsq = ਲੱਭੋ_dsq_for_dispatch(sch, rq, dsq_id, task_cpu(p));

	ਜੇ (dsq->id == SCX_DSQ_LOCAL)
		dispatch_ਮੰਜ਼ਿਲ_local_dsq(sch, rq, dsq, p, ਟੁਕੜਾ, vtime, enq_flags);
	ਨਹੀਂਤਾਂ
		scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, rq, dsq, p, ਟੁਕੜਾ, vtime,
				     enq_flags | SCX_ENQ_APPLY_ਟੁਕੜਾ | SCX_ENQ_CLEAR_OPSS);
}

ਖਾਲੀ scx_flush_dispatch_buf(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	u32 u;

	ਲਈ (u = 0; u < dspc->cursor; u++) {
		ਬਣਤਰ scx_dsp_buf_ent *ent = &dspc->buf[u];

		ਅੰਤ_dispatch(sch, rq, ent->task, ent->qseq, ent->dsq_id,
				ent->ਟੁਕੜਾ, ent->vtime, ent->enq_flags);
	}

	dspc->nr_tasks += dspc->cursor;
	dspc->cursor = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ maybe_ਕਤਾਰ_balance_callback(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);

	ਜੇ (!(rq->scx.flags & SCX_RQ_BAL_CB_PENDING))
		ਵਾਪਸ;

	ਕਤਾਰ_balance_callback(rq, &rq->scx.deferred_bal_cb,
				deferred_bal_cb_workfn);

	rq->scx.flags &= ~SCX_RQ_BAL_CB_PENDING;
}

ਸਥਿਰ ਗਿਣਤੀ scx_dsp_verdict dispatch_one(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev)
{
	ਬਣਤਰ scx_sched *ਜੜ੍ਹ_sch = scx_ਜੜ੍ਹ_protected_ਜੀਵਤ();
	ਗਿਣਤੀ scx_dsp_verdict verdict;
	s32 cpu = cpu_ਦਾ(rq);

	lockdep_assert_rq_held(rq);
	rq->scx.flags |= SCX_RQ_ਵਿੱਚ_DISPATCH;

	scx_process_sync_ecaps(rq, prev);

	ਜੇ ((ਜੜ੍ਹ_sch->ops.flags & SCX_OPS_HAS_CPU_PREEMPT) &&
	    unlikely(rq->scx.cpu_released)) {
		/*
		 * If the previous sched_class for the current CPU was not SCX,
		 * notify the BPF scheduler that it again has control of the
		 * core. This callback complements ->cpu_release(), which is
		 * emitted in switch_class().
		 */
		ਜੇ (ਜੜ੍ਹ_sch->ops.cpu_acquire)
			SCX_CALL_OP(ਜੜ੍ਹ_sch, cpu_acquire, rq, cpu, ਨਿਰਾ);
		rq->scx.cpu_released = false;
	}

	ਜੇ (prev->sched_class == &ext_sched_class) {
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
		ਜੇ ((prev->scx.flags & SCX_TASK_QUEUED) && prev->scx.ਟੁਕੜਾ &&
		    !scx_bypassing(scx_task_sched(prev), cpu)) {
			verdict = SCX_DSP_PREV;
			ਜਾਓ has_tasks;
		}
	}

	/* if there already are tasks to run, nothing to do */
	ਜੇ (rq->scx.local_dsq.nr) {
		verdict = SCX_DSP_LOCAL;
		ਜਾਓ has_tasks;
	}

	verdict = scx_dispatch_sched(ਜੜ੍ਹ_sch, rq, prev, false);
	ਜੇ (verdict != SCX_DSP_NONE)
		ਜਾਓ has_tasks;

	/*
	 * Didn't find another task to run. Keep running @prev unless its own
	 * scheduler set %SCX_OPS_ENQ_LAST and takes the enqueue instead, see
	 * put_prev_task_scx(). Read the scheduler here as the dispatch above
	 * may have dropped the rq lock while @prev changed class or scheduler.
	 */
	ਜੇ (prev->scx.flags & SCX_TASK_QUEUED) {
		ਬਣਤਰ scx_sched *prev_sch = scx_task_sched(prev);

		ਜੇ ((!(prev_sch->ops.flags & SCX_OPS_ENQ_ਆਖਰੀ) ||
		     scx_bypassing(prev_sch, cpu)) && scx_task_can_stay_on_cpu(rq, prev)) {
			__scx_add_event(prev_sch, SCX_EV_DISPATCH_KEEP_ਆਖਰੀ, 1);
			verdict = SCX_DSP_PREV;
			ਜਾਓ has_tasks;
		}
	}
	rq->scx.flags &= ~SCX_RQ_ਵਿੱਚ_DISPATCH;
	ਵਾਪਸ SCX_DSP_NONE;

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
	ਜੇ (unlikely(rq->scx.local_dsq.nr > 1 && rq->scx.nr_immed))
		scx_ਸਮਾਂ_ਸਾਰਣੀ_reenq_local(rq, 0);

	rq->scx.flags &= ~SCX_RQ_ਵਿੱਚ_DISPATCH;
	ਵਾਪਸ verdict;
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਅਗਲਾ_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਗਿਣਤੀ snt_e type)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);

	ਜੇ (type == SNT_REPICK)
		ਵਾਪਸ;

	ਜੇ (p->scx.flags & SCX_TASK_QUEUED) {
		/*
		 * Core-sched might decide to execute @p before it is
		 * dispatched. Call ops_dequeue() to notify the BPF scheduler.
		 */
		ops_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p, SCX_DEQ_CORE_SCHED_EXEC);
		scx_dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p);
	}

	p->se.exec_ਸ਼ੁਰੂ = rq_clock_task(rq);

	/* see dequeue_task_scx() on why we skip when !QUEUED */
	ਜੇ (SCX_HAS_OP(sch, running) && (p->scx.flags & SCX_TASK_QUEUED))
		SCX_CALL_OP_TASK(sch, running, rq, p);

	clr_task_runnable(p, true);

	/* apply any pending out-of-band slice request before the tick decision */
	apply_task_ਟੁਕੜਾ_oob(rq, p);

	/*
	 * @p is getting newly scheduled or got kicked after someone updated its
	 * slice. Update SCX_RQ_CAN_STOP_TICK to reflect whether the tick can be
	 * stopped. See scx_can_stop_tick().
	 *
	 * Moreover, refresh the load_avgs just when transitioning in and out of
	 * nohz. In the future, we might want to add a mechanism to update
	 * load_avgs periodically on tick-stopped CPUs.
	 */
	ਜੇ (p->scx.ਟੁਕੜਾ == SCX_ਟੁਕੜਾ_INF) {
		ਜੇ (!(rq->scx.flags & SCX_RQ_CAN_STOP_TICK)) {
			/*
			 * Bypass mode always assigns finite slices, so @p
			 * can't have an infinite slice while bypassing.
			 * Therefore, sched_update_tick_dependency() can safely
			 * evaluate the outgoing task.
			 */
			rq->scx.flags |= SCX_RQ_CAN_STOP_TICK;
			sched_update_tick_dependency(rq);

			update_other_ਲੱਦੋ_avgs(rq);
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (rq->scx.flags & SCX_RQ_CAN_STOP_TICK) {
			rq->scx.flags &= ~SCX_RQ_CAN_STOP_TICK;
			update_other_ਲੱਦੋ_avgs(rq);
		}

		/*
		 * @rq still references the outgoing scheduling context. A finite
		 * slice is sufficient by itself to require the tick.
		 */
		ਜੇ (tick_nohz_ਭਰਿਆ_cpu(cpu_ਦਾ(rq)))
			tick_nohz_dep_ਸੈੱਟ_cpu(cpu_ਦਾ(rq), TICK_DEP_ਦੋਆਧਾਰੀ_ਅੰਕ_SCHED);
	}
}

ਖਾਲੀ scx_proxy_donor_ਸ਼ੁਰੂ(ਬਣਤਰ rq *rq)
{
}

ਸਥਿਰ ਗਿਣਤੀ scx_cpu_preempt_reason
preempt_reason_from_class(ਅਚਲ ਬਣਤਰ sched_class *class)
{
	ਜੇ (class == &stop_sched_class)
		ਵਾਪਸ SCX_CPU_PREEMPT_STOP;
	ਜੇ (class == &dl_sched_class)
		ਵਾਪਸ SCX_CPU_PREEMPT_DL;
	ਜੇ (class == &rt_sched_class)
		ਵਾਪਸ SCX_CPU_PREEMPT_RT;
	ਵਾਪਸ SCX_CPU_PREEMPT_UNKNOWN;
}

ਸਥਿਰ ਖਾਲੀ switch_class(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਬਣਤਰ scx_sched *sch = scx_ਜੜ੍ਹ_protected_ਜੀਵਤ();
	ਅਚਲ ਬਣਤਰ sched_class *ਅਗਲਾ_class = ਅਗਲਾ->sched_class;

	ਜੇ (!(sch->ops.flags & SCX_OPS_HAS_CPU_PREEMPT))
		ਵਾਪਸ;

	/*
	 * The callback is conceptually meant to convey that the CPU is no
	 * longer under the control of SCX. Therefore, don't invoke the callback
	 * if the next class is below SCX (in which case the BPF scheduler has
	 * actively decided not to schedule any tasks on the CPU).
	 */
	ਜੇ (sched_class_above(&ext_sched_class, ਅਗਲਾ_class))
		ਵਾਪਸ;

	/*
	 * At this point we know that SCX was preempted by a higher priority
	 * sched_class, so invoke the ->cpu_release() callback if we have not
	 * done so already. We only send the callback once between SCX being
	 * preempted, and it regaining control of the CPU.
	 *
	 * ->cpu_release() complements ->cpu_acquire(), which is emitted the
	 *  next time that dispatch_one() is invoked.
	 */
	ਜੇ (!rq->scx.cpu_released) {
		ਜੇ (sch->ops.cpu_release) {
			ਬਣਤਰ scx_cpu_release_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
				.reason = preempt_reason_from_class(ਅਗਲਾ_class),
				.task = ਅਗਲਾ,
			};

			SCX_CALL_OP(sch, cpu_release, rq, cpu_ਦਾ(rq), &ਦਲੀਲਾਂ);
		}
		rq->scx.cpu_released = true;
	}
}

ਸਥਿਰ ਖਾਲੀ put_prev_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			      ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
	bool rescue_keep = false;

	/* see kick_sync_wait_bal_cb() */
	smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);

	update_curr_scx(rq);

	/*
	 * If the slice is consumed, protection ends with it. A rescuee
	 * preempted beforehand keeps going, see scx_rescue_keep().
	 */
	ਜੇ (!p->scx.ਟੁਕੜਾ) {
		ਜੇ (unlikely(p == scx_rescuee(rq)))
			rescue_keep = scx_rescue_keep(rq, p);
		ਜੇ (!rescue_keep)
			scx_task_ਟੁਕੜਾ_ended(rq, p);
	}

	/* see dequeue_task_scx() on why we skip when !QUEUED */
	ਜੇ (SCX_HAS_OP(sch, stopping) && (p->scx.flags & SCX_TASK_QUEUED))
		SCX_CALL_OP_TASK(sch, stopping, rq, p, true);

	ਜੇ (p->scx.flags & SCX_TASK_QUEUED) {
		ਸੈੱਟ_task_runnable(rq, p);

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
		ਜੇ ((p->scx.ਟੁਕੜਾ || unlikely(p == scx_rescuee(rq))) &&
		    !scx_bypassing(sch, cpu_ਦਾ(rq))) {
			ਜੇ (p->scx.flags & SCX_TASK_IMMED) {
				p->scx.flags |= SCX_TASK_REENQ_PREEMPTED;
				scx_do_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, SCX_ENQ_REENQ, -1);
				p->scx.flags &= ~SCX_TASK_REENQ_REASON_ਨਕਾਬ;
			} ਨਹੀਂਤਾਂ {
				u64 enq_flags = 0;

				/*
				 * Keep a preempted rescue going. If preempted
				 * by another SCX task, append to the local DSQ,
				 * see scx_rescue_keep().
				 */
				ਜੇ (unlikely(p == scx_rescuee(rq))) {
					enq_flags |= SCX_ENQ_IGNORE_CAPS;
					ਜੇ (!rescue_keep)
						enq_flags |= SCX_ENQ_ਸਿਰ;
				} ਨਹੀਂਤਾਂ {
					enq_flags |= SCX_ENQ_ਸਿਰ;
				}

				scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, rq, &rq->scx.local_dsq, p, 0, 0,
						     enq_flags);
			}
			ਜਾਓ switch_class;
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
		ਜੇ (ਅਗਲਾ && sched_class_above(&ext_sched_class, ਅਗਲਾ->sched_class) &&
		    scx_task_can_stay_on_cpu(rq, p)) {
			WARN_ON_ONCE(!sched_core_enabled(rq) &&
				     !(sch->ops.flags & SCX_OPS_ENQ_ਆਖਰੀ));
			scx_do_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, SCX_ENQ_ਆਖਰੀ, -1);

			/*
			 * A task put on the local DSQ runs, as anywhere else.
			 * Here the insert can't reschedule on its own: the pick
			 * has settled on @next and @p is still curr, so
			 * resched_curr() would flag @p and __schedule() clears
			 * that right after the pick. Flag @next instead.
			 */
			ਜੇ (p->scx.dsq == &rq->scx.local_dsq)
				ਸੈੱਟ_tsk_need_resched(ਅਗਲਾ);
		} ਨਹੀਂਤਾਂ {
			scx_do_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, 0, -1);
		}
	}

switch_class:
	ਜੇ (ਅਗਲਾ && ਅਗਲਾ->sched_class != &ext_sched_class)
		switch_class(rq, ਅਗਲਾ);
}

ਸਥਿਰ ਖਾਲੀ kick_sync_wait_bal_cb(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ scx_kick_syncs __rcu *ks;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ksyncs;
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
	ਜੇ (unlikely(cpu_ਦਾ(rq) != smp_processor_id()))
		ਵਾਪਸ;

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
	for_each_cpu(cpu, rq->scx.cpus_ਮੰਜ਼ਿਲ_sync) {
		/*
		 * smp_load_acquire() pairs with smp_store_release() on
		 * kick_sync updates on the target CPUs.
		 */
		ਜੇ (cpu == cpu_ਦਾ(rq) ||
		    smp_ਲੱਦੋ_acquire(&cpu_rq(cpu)->scx.kick_sync) != ksyncs[cpu]) {
			cpumask_clear_cpu(cpu, rq->scx.cpus_ਮੰਜ਼ਿਲ_sync);
			ਜਾਰੀ;
		}

		scx_rq_ਤਾਲਾ_drop(rq);
		raw_spin_rq_unlock_irq(rq);
		ਜਦੋਂ (READ_ONCE(cpu_rq(cpu)->scx.kick_sync) == ksyncs[cpu]) {
			smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);
			cpu_relax();
		}
		raw_spin_rq_ਤਾਲਾ_irq(rq);
		waited = true;
	}

	ਜੇ (waited)
		ਜਾਓ retry;
}

ਸਥਿਰ ਬਣਤਰ task_struct *ਪਹਿਲਾ_local_task(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ list_ਪਹਿਲਾ_ਇੰਦਰਾਜ_or_null(&rq->scx.local_dsq.list,
					ਬਣਤਰ task_struct, scx.dsq_list.ਗੰਢ);
}

/*
 * Run dispatch and queue the follow-up work for a pick.
 */
ਸਥਿਰ ਗਿਣਤੀ scx_dsp_verdict dispatch_pick(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf,
					  ਬਣਤਰ task_struct *prev)
{
	ਗਿਣਤੀ scx_dsp_verdict verdict;

	rq_unpin_ਤਾਲਾ(rq, rf);
	verdict = dispatch_one(rq, prev);
	rq_repin_ਤਾਲਾ(rq, rf);
	maybe_ਕਤਾਰ_balance_callback(rq);

	/*
	 * Defer to a balance callback which can drop rq lock and enable IRQs.
	 * Waiting directly in the pick path would deadlock against CPUs sending
	 * us IPIs (e.g. TLB flushes) while we wait for them.
	 */
	ਜੇ (unlikely(rq->scx.kick_sync_pending)) {
		rq->scx.kick_sync_pending = false;
		ਕਤਾਰ_balance_callback(rq, &rq->scx.kick_sync_bal_cb,
				       kick_sync_wait_bal_cb);
	}

	ਵਾਪਸ verdict;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
/*
 * Dispatch for a pick when core scheduling is enabled. The selection picks for
 * all SMT siblings and the rq_i->core_pick state it builds must stay atomic
 * throughout. If the dispatch released the rq lock, anything can have happened
 * in between - return %SCX_DSP_RETRY to restart the selection against current
 * state.
 */
ਸਥਿਰ ਗਿਣਤੀ scx_dsp_verdict dispatch_core_pick(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf,
					       ਬਣਤਰ task_struct *prev)
{
	ਗਿਣਤੀ scx_dsp_verdict verdict;
	u32 seq = rq->scx.ਤਾਲਾ_drop_seq;

	/* another dispatch is in flight on @rq, let that handle it */
	ਜੇ (rq->scx.flags & SCX_RQ_ਵਿੱਚ_DISPATCH)
		ਵਾਪਸ SCX_DSP_NONE;

	rq_unpin_ਤਾਲਾ(rq, rf);

	verdict = dispatch_one(rq, prev);

	ਜੇ (cpu_ਦਾ(rq) == smp_processor_id()) {
		maybe_ਕਤਾਰ_balance_callback(rq);

		/* see dispatch_pick() */
		ਜੇ (unlikely(rq->scx.kick_sync_pending)) {
			rq->scx.kick_sync_pending = false;
			ਕਤਾਰ_balance_callback(rq, &rq->scx.kick_sync_bal_cb,
					       kick_sync_wait_bal_cb);
		}
	} ਨਹੀਂਤਾਂ ਜੇ (unlikely(rq->scx.flags & SCX_RQ_BAL_CB_PENDING)) {
		/*
		 * Balance callbacks must run in the context that queued them,
		 * so they can't be queued on another CPU's rq. Run the deferred
		 * work directly instead.
		 */
		rq->scx.flags &= ~SCX_RQ_BAL_CB_PENDING;
		ਲੜੀਵਾਰ_deferred(rq);
	}

	rq_repin_ਤਾਲਾ(rq, rf);

	/* if dispatch_one() released the rq lock, restart the selection */
	ਜੇ (rq->scx.ਤਾਲਾ_drop_seq != seq)
		ਵਾਪਸ SCX_DSP_RETRY;

	ਵਾਪਸ verdict;
}
#ਨਹੀਂਤਾਂ	/* CONFIG_SCHED_CORE */
ਸਥਿਰ ਗਿਣਤੀ scx_dsp_verdict dispatch_core_pick(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf,
					       ਬਣਤਰ task_struct *prev)
{
	ਵਾਪਸ SCX_DSP_NONE;
}
#ਜੇ_ਅੰਤ	/* CONFIG_SCHED_CORE */

ਸਥਿਰ ਬਣਤਰ task_struct *
do_pick_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf, bool force_scx)
{
	ਬਣਤਰ task_struct *prev = rq->curr;
	ਗਿਣਤੀ scx_dsp_verdict verdict;
	ਬਣਤਰ task_struct *p;

	/* see kick_sync_wait_bal_cb() */
	smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);

	rq_modified_begin(rq, &ext_sched_class);

	ਜੇ (sched_core_enabled(rq))
		verdict = dispatch_core_pick(rq, rf, prev);
	ਨਹੀਂਤਾਂ
		verdict = dispatch_pick(rq, rf, prev);

	ਜੇ (verdict == SCX_DSP_RETRY)
		ਵਾਪਸ RETRY_TASK;

	/*
	 * If any higher-priority sched class enqueued a runnable task on this
	 * rq during dispatch_one(), abort and return RETRY_TASK, so that the
	 * scheduler loop can restart.
	 *
	 * If @force_scx is true, always try to pick a SCHED_EXT task,
	 * regardless of any higher-priority sched classes activity.
	 */
	ਜੇ (!force_scx && rq_modified_above(rq, &ext_sched_class))
		ਵਾਪਸ RETRY_TASK;

	/*
	 * If we're keeping @prev, replenish slice if necessary and keep running
	 * @prev. Otherwise, pop the first one from the local DSQ.
	 */
	ਜੇ (verdict == SCX_DSP_PREV) {
		p = prev;
		ਜੇ (!p->scx.ਟੁਕੜਾ) {
			/* the slice is consumed, protection ends */
			scx_task_ਟੁਕੜਾ_ended(rq, p);
			refill_task_ਟੁਕੜਾ_dfl(scx_task_sched(p), p);
		}
	} ਨਹੀਂਤਾਂ {
		p = ਪਹਿਲਾ_local_task(rq);
		ਜੇ (!p)
			ਵਾਪਸ ਨਿਰਾ;

		ਜੇ (unlikely(!p->scx.ਟੁਕੜਾ) && scx_task_can_stay_on_cpu(rq, p)) {
			ਬਣਤਰ scx_sched *sch = scx_task_sched(p);

			ਜੇ (!scx_bypassing(sch, cpu_ਦਾ(rq)) &&
			    !sch->warned_zero_ਟੁਕੜਾ) {
				printk_deferred(KERN_WARNING "sched_ext: %s[%d] has zero slice in %s()\n",
						p->comm, p->pid, __func__);
				sch->warned_zero_ਟੁਕੜਾ = true;
			}
			refill_task_ਟੁਕੜਾ_dfl(sch, p);
		}
	}

	ਵਾਪਸ p;
}

ਸਥਿਰ ਬਣਤਰ task_struct *pick_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਵਾਪਸ do_pick_task_scx(rq, rf, false);
}

/*
 * Select the next task to run from the ext scheduling class.
 *
 * Use do_pick_task_scx() directly with @force_scx enabled, since the
 * dl_server must always select a sched_ext task.
 */
ਸਥਿਰ ਬਣਤਰ task_struct *
ext_server_pick_task(ਬਣਤਰ sched_dl_entity *dl_se, ਬਣਤਰ rq_flags *rf)
{
	ਜੇ (!scx_enabled())
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ do_pick_task_scx(dl_se->rq, rf, true);
}

/*
 * Initialize the ext server deadline entity.
 */
ਖਾਲੀ ext_server_init(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ sched_dl_entity *dl_se = &rq->ext_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, ext_server_pick_task);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
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
bool scx_prio_less(ਅਚਲ ਬਣਤਰ task_struct *a, ਅਚਲ ਬਣਤਰ task_struct *b,
		   bool ਵਿੱਚ_fi)
{
	ਬਣਤਰ scx_sched *sch_a = scx_task_sched(a);
	ਬਣਤਰ scx_sched *sch_b = scx_task_sched(b);
	ਬਣਤਰ scx_sched *sch = ਨਿਰਾ;
	bool a_running, b_running;

	ਜੇ (sch_a == sch_b) {
		ਜੇ (SCX_HAS_OP(sch_a, core_sched_before))
			sch = sch_a;
	} ਨਹੀਂਤਾਂ {
		s32 level;

		ਲਈ (level = min(sch_a->level, sch_b->level); level >= 0; level--) {
			ਬਣਤਰ scx_sched *anc = sch_a->ancestors[level];

			ਜੇ (anc == sch_b->ancestors[level] &&
			    SCX_HAS_OP(anc, core_sched_before)) {
				sch = anc;
				ਰੋਕੋ;
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
	ਜੇ (sch && !scx_bypassing(sch, task_cpu(a)) && !scx_bypassing(sch, task_cpu(b)))
		ਵਾਪਸ SCX_CALL_OP_2TASKS_RET(sch, core_sched_before, task_rq(a),
					      (ਬਣਤਰ task_struct *)b,
					      (ਬਣਤਰ task_struct *)a);

	/*
	 * runnable_at is refreshed only on enqueue, so a task which keeps
	 * occupying its CPU carries a stale stamp. A running task is the most
	 * recently serviced whatever its stamp says. Order it after every
	 * waiting task.
	 */
	a_running = a->on_cpu;
	b_running = b->on_cpu;
	ਜੇ (a_running != b_running)
		ਵਾਪਸ a_running;

	ਵਾਪਸ ਸਮਾਂ_after(a->scx.runnable_at, b->scx.runnable_at);
}
#ਜੇ_ਅੰਤ	/* CONFIG_SCHED_CORE */

ਸਥਿਰ ਪੂਰਨ select_task_rq_scx(ਬਣਤਰ task_struct *p, ਪੂਰਨ prev_cpu, ਪੂਰਨ wake_flags)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
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
	ਜੇ (unlikely(wake_flags & WF_EXEC))
		ਵਾਪਸ prev_cpu;

	bypassing = scx_bypassing(sch, task_cpu(p));
	ਜੇ (likely(SCX_HAS_OP(sch, select_cpu)) && !bypassing) {
		s32 cpu;
		ਬਣਤਰ task_struct **ddsp_taskp;

		ddsp_taskp = this_cpu_ptr(&ਸਿੱਧਾ_dispatch_task);
		WARN_ON_ONCE(*ddsp_taskp);
		*ddsp_taskp = p;

		this_rq()->scx.ਵਿੱਚ_select_cpu = true;
		cpu = SCX_CALL_OP_TASK_RET(sch, select_cpu, ਨਿਰਾ, p,
					   scx_cpu_arg(prev_cpu), wake_flags);
		cpu = scx_cpu_ret(sch, cpu);
		this_rq()->scx.ਵਿੱਚ_select_cpu = false;
		p->scx.selected_cpu = cpu;
		*ddsp_taskp = ਨਿਰਾ;
		ਜੇ (scx_cpu_valid(sch, cpu, "from ops.select_cpu()"))
			ਵਾਪਸ cpu;
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ prev_cpu;
	} ਨਹੀਂਤਾਂ {
		s32 cpu;

		/*
		 * While bypassing, the enqueue path routes @p to a bypass DSQ
		 * without consulting the direct-dispatch target, making the
		 * default selection pointless. It doesn't work anyway when the
		 * scheduler does its own idle tracking and the built-in idle
		 * cpumasks are not updated. Leave @p on @prev_cpu.
		 */
		ਜੇ (bypassing) {
			__scx_add_event(sch, SCX_EV_BYPASS_DISPATCH, 1);
			p->scx.selected_cpu = prev_cpu;
			ਵਾਪਸ prev_cpu;
		}

		cpu = scx_select_cpu_dfl(p, prev_cpu, wake_flags, ਨਿਰਾ, 0);
		ਜੇ (cpu >= 0) {
			/*
			 * Carry the slice refill and let the insertion commit
			 * it under rq lock. See the write rules.
			 */
			__scx_add_event(sch, SCX_EV_REFILL_ਟੁਕੜਾ_DFL, 1);
			p->scx.ddsp_ਟੁਕੜਾ = READ_ONCE(sch->ਟੁਕੜਾ_dfl);
			p->scx.ddsp_enq_flags = SCX_ENQ_ਟੁਕੜਾ_DFL;
			p->scx.ddsp_dsq_id = SCX_DSQ_LOCAL;
		} ਨਹੀਂਤਾਂ {
			cpu = prev_cpu;
		}
		p->scx.selected_cpu = cpu;

		ਵਾਪਸ cpu;
	}
}

ਸਥਿਰ ਖਾਲੀ task_woken_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਲੜੀਵਾਰ_deferred(rq);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_cpus_allowed_scx(ਬਣਤਰ task_struct *p,
				 ਬਣਤਰ affinity_context *ac)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);

	ਸੈੱਟ_cpus_allowed_common(p, ac);

	ਜੇ (task_dead_and_ਪੂਰਾ(p))
		ਵਾਪਸ;

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
	ਜੇ (SCX_HAS_OP(sch, ਸੈੱਟ_cpumask) &&
	    (!scx_is_cid_type() || scx_get_task_state(p) == SCX_TASK_ENABLED))
		scx_call_op_ਸੈੱਟ_cpumask(sch, task_rq(p), p, (ਬਣਤਰ cpumask *)p->cpus_ptr);
}

ਸਥਿਰ ਖਾਲੀ handle_hotplug(ਬਣਤਰ rq *rq, bool online)
{
	ਬਣਤਰ scx_sched *sch = scx_ਜੜ੍ਹ_protected();
	s32 cpu = cpu_ਦਾ(rq);
	s32 cpu_or_cid = cpu;

	atomic_long_inc(&scx_hotplug_seq);

	/*
	 * scx_root updates are protected by cpus_read_lock() and will stay
	 * stable here. Note that we can't depend on scx_enabled() test as the
	 * hotplug ops need to be enabled before __scx_enabled is set.
	 */
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;

	ਜੇ (scx_enabled())
		scx_idle_update_selcpu_topology(&sch->ops);

	ਜੇ (online)
		scx_online_ecaps(rq);
	ਨਹੀਂਤਾਂ
		scx_offline_ecaps(rq);

	/*
	 * The tables can't be retired while this function is running as the
	 * retirement is inside cpus_read_lock. However, scx_cpu_arg() is
	 * awkward here as the tables can be NULL after root enable failure and
	 * lockdep would trigger without surrounding rcu_read_lock(). Open code
	 * the translation. If the table is NULL, the ops are also cleared and
	 * @cpu_or_cid goes unused.
	 */
	ਜੇ (scx_is_cid_type()) {
		s16 *tbl = rcu_dereference_check(scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl,
						 lockdep_is_cpus_held());

		ਜੇ (tbl) {
			ਬਣਤਰ scx_sched *pos;

			cpu_or_cid = tbl[cpu];

			guard(raw_spinlock_irqsave)(&scx_sched_ਤਾਲਾ);
			list_for_each_ਇੰਦਰਾਜ(pos, &scx_sched_all, all) {
				ਬਣਤਰ scx_cmask *ਨਕਾਬ = pos->online_cmask;

				ਜੇ (ਨਕਾਬ)
					__assign_bit(cpu_or_cid, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *)ਨਕਾਬ->ਬਿੱਟ,
						     online);
			}
		}
	}

	ਜੇ (online && SCX_HAS_OP(sch, cpu_online))
		SCX_CALL_OP(sch, cpu_online, ਨਿਰਾ, cpu_or_cid);
	ਨਹੀਂਤਾਂ ਜੇ (!online && SCX_HAS_OP(sch, cpu_offline))
		SCX_CALL_OP(sch, cpu_offline, ਨਿਰਾ, cpu_or_cid);
	ਨਹੀਂਤਾਂ
		scx_exit(sch, SCX_EXIT_UNREG_KERN,
			 SCX_ECODE_ACT_RESTART | SCX_ECODE_RSN_HOTPLUG,
			 "cpu %d going %s, exiting scheduler", cpu,
			 online ? "online" : "offline");
}

ਖਾਲੀ scx_rq_activate(ਬਣਤਰ rq *rq)
{
	handle_hotplug(rq, true);
}

ਖਾਲੀ scx_rq_deactivate(ਬਣਤਰ rq *rq)
{
	handle_hotplug(rq, false);
}

ਸਥਿਰ ਖਾਲੀ rq_online_scx(ਬਣਤਰ rq *rq)
{
	rq->scx.flags |= SCX_RQ_ONLINE;
}

ਸਥਿਰ ਖਾਲੀ rq_offline_scx(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *p, *n;

	rq->scx.flags &= ~SCX_RQ_ONLINE;

	/* sched domain rebuilds call rq_offline with the CPU staying alive */
	ਜੇ (cpu_ਸਰਗਰਮ(cpu_ਦਾ(rq)))
		ਵਾਪਸ;

	scx_rescue_flush(rq);

	/*
	 * An offline CPU no longer calls ops.dispatch(). Re-enqueue its tasks
	 * onto the local DSQ so that they run here and balance_push() moves
	 * them off.
	 */
	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ_ਉਲਟ(p, n, &rq->scx.runnable_list, scx.runnable_ਗੰਢ) {
		ਜੇ (p->scx.dsq == &rq->scx.local_dsq)
			ਜਾਰੀ;
		guard(sched_change)(p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK);
	}
}

ਸਥਿਰ bool check_rq_for_timeouts(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ task_struct *p;
	ਬਣਤਰ rq_flags rf;
	bool timed_ਬਾਹਰਲਾ = false;

	rq_ਤਾਲਾ_irqsave(rq, &rf);
	sch = rcu_dereference_bh(scx_ਜੜ੍ਹ);
	ਜੇ (unlikely(!sch))
		ਜਾਓ ਬਾਹਰਲਾ_unlock;

	list_for_each_ਇੰਦਰਾਜ(p, &rq->scx.runnable_list, scx.runnable_ਗੰਢ) {
		ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਆਖਰੀ_runnable = p->scx.runnable_at;

		ਜੇ (unlikely(ਸਮਾਂ_after(jiffies,
					ਆਖਰੀ_runnable + READ_ONCE(sch->watchdog_timeout)))) {
			ਬਣਤਰ scx_dispatch_q *dsq = READ_ONCE(p->scx.dsq);
			u32 dur_ms = jiffies_ਮੰਜ਼ਿਲ_msecs(jiffies - ਆਖਰੀ_runnable);

			/*
			 * A task can be stuck on a DSQ that a sched other than
			 * its owner is responsible for draining, e.g. an
			 * ancestor's bypass DSQ while the owner is bypassing.
			 * Blame the drainer. The local DSQ is consumed by the
			 * cpu itself and keeps blame on the owner.
			 */
			ਜੇ (dsq && dsq->sched && dsq->id != SCX_DSQ_LOCAL)
				sch = dsq->sched;

			__scx_exit(sch, SCX_EXIT_ERROR_STALL, 0, cpu_ਦਾ(rq),
				   "%s[%d] failed to run for %u.%03us",
				   p->comm, p->pid, dur_ms / 1000,
				   dur_ms % 1000);
			timed_ਬਾਹਰਲਾ = true;
			ਰੋਕੋ;
		}
	}
ਬਾਹਰਲਾ_unlock:
	rq_unlock_irqrestore(rq, &rf);
	ਵਾਪਸ timed_ਬਾਹਰਲਾ;
}

ਸਥਿਰ ਖਾਲੀ scx_watchdog_workfn(ਬਣਤਰ work_struct *work)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ intv;
	ਪੂਰਨ cpu;

	WRITE_ONCE(scx_watchdog_timestamp, jiffies);

	for_each_online_cpu(cpu) {
		ਜੇ (unlikely(check_rq_for_timeouts(cpu_rq(cpu))))
			ਰੋਕੋ;

		cond_resched();
	}

	intv = READ_ONCE(scx_watchdog_interval);
	ਜੇ (intv < ULONG_MAX)
		ਕਤਾਰ_delayed_work(system_dfl_wq, ਮੰਜ਼ਿਲ_delayed_work(work), intv);
}

ਖਾਲੀ scx_tick(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ scx_sched *ਜੜ੍ਹ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਆਖਰੀ_check;

	ਜੇ (!scx_enabled())
		ਵਾਪਸ;

	ਜੜ੍ਹ = rcu_dereference_bh(scx_ਜੜ੍ਹ);
	ਜੇ (unlikely(!ਜੜ੍ਹ))
		ਵਾਪਸ;

	ਆਖਰੀ_check = READ_ONCE(scx_watchdog_timestamp);
	ਜੇ (unlikely(ਸਮਾਂ_after(jiffies,
				ਆਖਰੀ_check + READ_ONCE(ਜੜ੍ਹ->watchdog_timeout)))) {
		u32 dur_ms = jiffies_ਮੰਜ਼ਿਲ_msecs(jiffies - ਆਖਰੀ_check);

		scx_exit(ਜੜ੍ਹ, SCX_EXIT_ERROR_STALL, 0,
			 "watchdog failed to check in for %u.%03us",
			 dur_ms / 1000, dur_ms % 1000);
	}

	update_other_ਲੱਦੋ_avgs(rq);
}

ਸਥਿਰ ਖਾਲੀ task_tick_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr, ਪੂਰਨ queued)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(curr);

	update_curr_scx(rq);

	/*
	 * While disabling, always resched as we can't trust the slice
	 * management.
	 */
	ਜੇ (scx_bypassing(sch, cpu_ਦਾ(rq)))
		scx_ਸੈੱਟ_task_ਟੁਕੜਾ(curr, 0);
	ਨਹੀਂਤਾਂ ਜੇ (SCX_HAS_OP(sch, tick))
		SCX_CALL_OP_TASK(sch, tick, rq, curr);

	ਜੇ (!curr->scx.ਟੁਕੜਾ)
		resched_curr(rq);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
ਸਥਿਰ ਬਣਤਰ cgroup *tg_cgrp(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	/*
	 * If CGROUP_SCHED is disabled, @tg is NULL. If @tg is an autogroup,
	 * @tg->css.cgroup is NULL. In both cases, @tg can be treated as the
	 * root cgroup.
	 */
	ਜੇ (tg && tg->css.cgroup)
		ਵਾਪਸ tg->css.cgroup;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ &cgrp_dfl_ਜੜ੍ਹ.cgrp;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCX_INIT_TASK_ਦਲੀਲਾਂ_CGROUP(cgrp)		.cgroup = (cgrp),

#ਨਹੀਂਤਾਂ	/* CONFIG_EXT_GROUP_SCHED */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCX_INIT_TASK_ਦਲੀਲਾਂ_CGROUP(cgrp)

#ਜੇ_ਅੰਤ	/* CONFIG_EXT_GROUP_SCHED */

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
ਪੂਰਨ __scx_init_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p,
		    ਬਣਤਰ cgroup *cgrp, bool fork)
{
	ਪੂਰਨ ret;

	p->scx.disallow = false;

	ਜੇ (SCX_HAS_OP(sch, init_task)) {
		ਬਣਤਰ scx_init_task_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
			SCX_INIT_TASK_ਦਲੀਲਾਂ_CGROUP(cgrp ?: tg_cgrp(task_ਸਮੂਹ(p)))
			.fork = fork,
		};

		ret = SCX_CALL_OP_RET(sch, init_task, ਨਿਰਾ, p, &ਦਲੀਲਾਂ);
		ਜੇ (unlikely(ret)) {
			ret = scx_ops_sanitize_err(sch, "init_task", ret);
			ਵਾਪਸ ret;
		}
	}

	ਜੇ (p->scx.disallow) {
		ਜੇ (unlikely(scx_ਮਾਪਾ(sch))) {
			scx_error(sch, "non-root ops.init_task() set task->scx.disallow for %s[%d]",
				  p->comm, p->pid);
		} ਨਹੀਂਤਾਂ ਜੇ (unlikely(fork)) {
			scx_error(sch, "ops.init_task() set task->scx.disallow for %s[%d] during fork",
				  p->comm, p->pid);
		} ਨਹੀਂਤਾਂ ਜੇ (unlikely(scx_enable_state() != SCX_ENABLING)) {
			scx_error(sch, "ops.init_task() set task->scx.disallow for %s[%d] outside the enable path",
				  p->comm, p->pid);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ rq *rq;
			ਬਣਤਰ rq_flags rf;

			rq = task_rq_ਤਾਲਾ_2(p, &rf);

			/*
			 * We're in the load path and @p->policy will be applied
			 * right after. Reverting @p->policy here and rejecting
			 * %SCHED_EXT transitions from scx_check_setscheduler()
			 * guarantees that if ops.init_task() sets @p->disallow,
			 * @p can never be in SCX.
			 */
			ਜੇ (p->policy == SCHED_EXT) {
				p->policy = SCHED_ਸਾਧਾਰਨ;
				atomic_long_inc(&scx_nr_rejected);
			}

			task_rq_unlock(rq, p, &rf);
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ __scx_enable_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ rq *rq = task_rq(p);
	u32 ਭਾਰ;

	lockdep_assert_rq_held(rq);

	/*
	 * Verify the task is not in BPF scheduler's custody. If flag
	 * transitions are consistent, the flag should always be clear
	 * here.
	 */
	WARN_ON_ONCE(p->scx.flags & SCX_TASK_ਵਿੱਚ_CUSTODY);

	/*
	 * Set the weight before calling ops.enable() so that the scheduler
	 * doesn't see a stale value if they inspect the task struct.
	 */
	ਜੇ (task_has_idle_policy(p))
		ਭਾਰ = ਭਾਰ_IDLEPRIO;
	ਨਹੀਂਤਾਂ
		ਭਾਰ = sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[p->static_prio - MAX_RT_PRIO];

	p->scx.ਭਾਰ = sched_ਭਾਰ_ਮੰਜ਼ਿਲ_cgroup(ਭਾਰ);

	ਜੇ (SCX_HAS_OP(sch, enable)) {
		ਜੇ (scx_is_cid_type()) {
			ਬਣਤਰ scx_cmask *cmask = scx_fill_cmask_scratch(sch, p->cpus_ptr);
			ਬਣਤਰ scx_enable_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
				.cmask_arena_addr = scx_kaddr_ਮੰਜ਼ਿਲ_arena(sch, cmask),
			};

			SCX_CALL_CID_OP_TASK(sch, enable, rq, p, &ਦਲੀਲਾਂ);
		} ਨਹੀਂਤਾਂ {
			SCX_CALL_OP_TASK(sch, enable, rq, p);
		}
	}

	/*
	 * The initial mask also goes out through set_cmask() so a scheduler can
	 * track affinity there alone, and before set_weight() so that the mask
	 * is in place when weight-dependent state is derived, see struct
	 * scx_enable_args.
	 */
	ਜੇ (scx_is_cid_type() && SCX_HAS_OP(sch, ਸੈੱਟ_cpumask))
		scx_call_op_ਸੈੱਟ_cpumask(sch, rq, p, p->cpus_ptr);

	ਜੇ (SCX_HAS_OP(sch, ਸੈੱਟ_ਭਾਰ))
		SCX_CALL_OP_TASK(sch, ਸੈੱਟ_ਭਾਰ, rq, p, p->scx.ਭਾਰ);
}

ਖਾਲੀ scx_enable_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p)
{
	__scx_enable_task(sch, p);
	scx_ਸੈੱਟ_task_state(p, SCX_TASK_ENABLED);
}

ਸਥਿਰ ਖਾਲੀ scx_disable_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(scx_get_task_state(p) != SCX_TASK_ENABLED);

	clear_ਸਿੱਧਾ_dispatch(p);

	ਜੇ (SCX_HAS_OP(sch, disable))
		SCX_CALL_OP_TASK(sch, disable, rq, p);
	scx_ਸੈੱਟ_task_state(p, SCX_TASK_READY);

	/*
	 * Reset the SCX-managed fields when @p leaves the BPF scheduler's
	 * control, after ops.disable() has observed their final values.
	 */
	p->scx.dsq_vtime = 0;
	scx_task_ਟੁਕੜਾ_ended(rq, p);
	scx_ਸੈੱਟ_task_ਟੁਕੜਾ(p, 0);
	p->scx.reenq_cnt = 0;

	/*
	 * Verify the task is not in BPF scheduler's custody. If flag
	 * transitions are consistent, the flag should always be clear
	 * here.
	 */
	WARN_ON_ONCE(p->scx.flags & SCX_TASK_ਵਿੱਚ_CUSTODY);
}

ਖਾਲੀ __scx_disable_and_exit_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ scx_exit_task_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
		.cancelled = false,
	};

	lockdep_assert_held(&p->pi_ਤਾਲਾ);
	lockdep_assert_rq_held(task_rq(p));

	ਚੋਣ (scx_get_task_state(p)) {
	ਹਾਲਤ SCX_TASK_NONE:
		ਵਾਪਸ;
	ਹਾਲਤ SCX_TASK_INIT:
		ਦਲੀਲਾਂ.cancelled = true;
		ਰੋਕੋ;
	ਹਾਲਤ SCX_TASK_READY:
		ਰੋਕੋ;
	ਹਾਲਤ SCX_TASK_ENABLED:
		scx_disable_task(sch, p);
		ਰੋਕੋ;
	ਮੂਲ:
		WARN_ON_ONCE(true);
		ਵਾਪਸ;
	}

	ਜੇ (SCX_HAS_OP(sch, exit_task))
		SCX_CALL_OP_TASK(sch, exit_task, task_rq(p), p, &ਦਲੀਲਾਂ);
}

/*
 * Undo a completed __scx_init_task(sch, p, false) when scx_enable_task() never
 * ran. The task state has not been transitioned, so this mirrors the
 * SCX_TASK_INIT branch in __scx_disable_and_exit_task().
 */
ਖਾਲੀ scx_sub_init_cancel_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ scx_exit_task_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = { .cancelled = true };

	lockdep_assert_held(&p->pi_ਤਾਲਾ);
	lockdep_assert_rq_held(task_rq(p));

	/* @p was never associated with @sch, dispatch on the explicit @sch */
	ਜੇ (SCX_HAS_OP(sch, exit_task))
		__SCX_CALL_OP_TASK(sch, ops, exit_task, task_rq(p), p, &ਦਲੀਲਾਂ);
}

ਖਾਲੀ scx_disable_and_exit_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p)
{
	__scx_disable_and_exit_task(sch, p);

	/*
	 * If set, @p exited between __scx_init_task() and scx_enable_task() in
	 * scx_sub_enable() and is initialized for both the associated sched and
	 * its parent. Exit for the child too - scx_enable_task() never ran for
	 * it, so undo only init_task. The flag is only set on the sub-enable
	 * path, so it's always clear when @p arrives here in %SCX_TASK_NONE.
	 */
	ਜੇ (p->scx.flags & SCX_TASK_SUB_INIT) {
		ਜੇ (!WARN_ON_ONCE(!scx_enabling_sub_sched))
			scx_sub_init_cancel_task(scx_enabling_sub_sched, p);
		p->scx.flags &= ~SCX_TASK_SUB_INIT;
	}

	scx_ਸੈੱਟ_task_sched(p, ਨਿਰਾ);
	scx_ਸੈੱਟ_task_state(p, SCX_TASK_NONE);
}

ਖਾਲੀ init_scx_entity(ਬਣਤਰ sched_ext_entity *scx)
{
	memset(scx, 0, ਆਕਾਰ(*scx));
	INIT_LIST_ਸਿਰ(&scx->dsq_list.ਗੰਢ);
	RB_CLEAR_ਗੰਢ(&scx->dsq_priq);
	scx->sticky_cpu = -1;
	scx->holding_cpu = -1;
	scx->runnable_cpu = -1;
	INIT_LIST_ਸਿਰ(&scx->runnable_ਗੰਢ);
	scx->runnable_at = jiffies;
	scx->ddsp_dsq_id = SCX_DSQ_INVALID;
	scx->ਟੁਕੜਾ = SCX_ਟੁਕੜਾ_DFL;
}

/* See scx_tid_alloc / scx_tid_cursor. */
ਸਥਿਰ u64 scx_alloc_tid(ਖਾਲੀ)
{
	ਬਣਤਰ scx_tid_alloc *ta;

	guard(preempt)();
	ta = this_cpu_ptr(&scx_tid_alloc);

	ਜੇ (unlikely(ta->ਅਗਲਾ >= ta->ਸਿਰਾ)) {
		ta->ਅਗਲਾ = atomic64_fetch_ਜੋੜੋ(SCX_TID_CHUNK, &scx_tid_cursor);
		ta->ਸਿਰਾ = ta->ਅਗਲਾ + SCX_TID_CHUNK;
	}
	ਵਾਪਸ ta->ਅਗਲਾ++;
}

ਸਥਿਰ ਖਾਲੀ scx_tid_hash_ਪਾਓ(ਬਣਤਰ task_struct *p)
{
	ਪੂਰਨ ret;

	lockdep_assert_held(&scx_tasks_ਤਾਲਾ);

	ret = rhashtable_lookup_ਪਾਓ_fast(&scx_tid_hash,
					    &p->scx.tid_hash_ਗੰਢ,
					    scx_tid_hash_params);
	WARN_ON_ONCE(ret);
}

ਖਾਲੀ scx_pre_fork(ਬਣਤਰ task_struct *p)
{
	/*
	 * BPF scheduler enable/disable paths want to be able to iterate and
	 * update all tasks which can become complex when racing forks. As
	 * enable/disable are very cold paths, let's use a percpu_rwsem to
	 * exclude forks.
	 */
	percpu_ਥੱਲੇ_read(&scx_fork_rwsem);
}

ਪੂਰਨ scx_fork(ਬਣਤਰ task_struct *p, ਬਣਤਰ kernel_clone_ਦਲੀਲਾਂ *kargs)
{
	s32 ret;

	percpu_rwsem_assert_held(&scx_fork_rwsem);

	p->scx.tid = scx_alloc_tid();

	ਜੇ (scx_init_task_enabled) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
		ਬਣਤਰ scx_sched *sch = scx_cgroup_sched(kargs->cset->dfl_cgrp);
#ਨਹੀਂਤਾਂ
		ਬਣਤਰ scx_sched *sch = scx_ਜੜ੍ਹ_protected_ਜੀਵਤ();
#ਜੇ_ਅੰਤ
		scx_ਸੈੱਟ_task_state(p, SCX_TASK_INIT_BEGIN);
		ret = __scx_init_task(sch, p, ਨਿਰਾ, true);
		ਜੇ (unlikely(ret)) {
			scx_ਸੈੱਟ_task_state(p, SCX_TASK_NONE);
			ਵਾਪਸ ret;
		}
		scx_ਸੈੱਟ_task_state(p, SCX_TASK_INIT);
		scx_ਸੈੱਟ_task_sched(p, sch);
	}

	ਵਾਪਸ 0;
}

ਖਾਲੀ scx_post_fork(ਬਣਤਰ task_struct *p)
{
	ਜੇ (scx_init_task_enabled) {
		scx_ਸੈੱਟ_task_state(p, SCX_TASK_READY);

		/*
		 * Enable the task immediately if it's running on sched_ext.
		 * Otherwise, it'll be enabled in switching_to_scx() if and
		 * when it's ever configured to run with a SCHED_EXT policy.
		 */
		ਜੇ (p->sched_class == &ext_sched_class) {
			ਬਣਤਰ rq_flags rf;
			ਬਣਤਰ rq *rq;

			rq = task_rq_ਤਾਲਾ_2(p, &rf);
			scx_enable_task(scx_task_sched(p), p);
			task_rq_unlock(rq, p, &rf);
		}
	}

	scoped_guard(raw_spinlock_irq, &scx_tasks_ਤਾਲਾ) {
		list_ਜੋੜੋ_ਪੂਛ(&p->scx.tasks_ਗੰਢ, &scx_tasks);
		ਜੇ (scx_tid_ਮੰਜ਼ਿਲ_task_enabled())
			scx_tid_hash_ਪਾਓ(p);
	}

	percpu_ਉੱਪਰ_read(&scx_fork_rwsem);
}

ਖਾਲੀ scx_cancel_fork(ਬਣਤਰ task_struct *p)
{
	ਜੇ (scx_init_task_enabled) {
		ਬਣਤਰ rq *rq;
		ਬਣਤਰ rq_flags rf;

		rq = task_rq_ਤਾਲਾ_2(p, &rf);
		WARN_ON_ONCE(scx_get_task_state(p) >= SCX_TASK_READY);
		scx_disable_and_exit_task(scx_task_sched(p), p);
		task_rq_unlock(rq, p, &rf);
	}

	percpu_ਉੱਪਰ_read(&scx_fork_rwsem);
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
ਸਥਿਰ bool task_dead_and_ਪੂਰਾ(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * In do_task_dead(), a dying task sets %TASK_DEAD with preemption
	 * disabled and __schedule(). If @p has %TASK_DEAD set and off CPU, @p
	 * won't ever run again.
	 */
	ਵਾਪਸ unlikely(READ_ONCE(p->__state) == TASK_DEAD) &&
		!task_on_cpu(rq, p);
}

ਖਾਲੀ sched_ext_dead(ਬਣਤਰ task_struct *p)
{
	/*
	 * By the time control reaches here, @p has %TASK_DEAD set, switched out
	 * for the last time and then dropped the rq lock - task_dead_and_done()
	 * should be returning %true nullifying the straggling sched_class ops.
	 * Remove from scx_tasks and exit @p.
	 */
	scoped_guard(raw_spinlock_irqsave, &scx_tasks_ਤਾਲਾ) {
		list_del_init(&p->scx.tasks_ਗੰਢ);
		ਜੇ (scx_tid_ਮੰਜ਼ਿਲ_task_enabled())
			rhashtable_remove_fast(&scx_tid_hash,
					       &p->scx.tid_hash_ਗੰਢ,
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
	ਜੇ (scx_get_task_state(p) != SCX_TASK_NONE) {
		ਬਣਤਰ rq_flags rf;
		ਬਣਤਰ rq *rq;

		rq = task_rq_ਤਾਲਾ_2(p, &rf);
		ਜੇ (scx_get_task_state(p) != SCX_TASK_INIT_BEGIN)
			scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_ਸੈੱਟ_task_state(p, SCX_TASK_DEAD);
		task_rq_unlock(rq, p, &rf);
	}
}

ਸਥਿਰ ਖਾਲੀ reweight_task_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			      ਅਚਲ ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);

	lockdep_assert_rq_held(task_rq(p));

	ਜੇ (task_dead_and_ਪੂਰਾ(p))
		ਵਾਪਸ;

	/*
	 * When switching sched_class away from SCX, reweight_task_scx()
	 * is called _after_ scx_disable_task(). Skip calling ops.set_weight()
	 * since the BPF scheduler may have already forgotten the task in
	 * ops.disable().
	 * p->scx.weight will be recalculated in scx_enable_task() if the task
	 * ever returns to SCX class.
	 */
	ਜੇ (scx_get_task_state(p) != SCX_TASK_ENABLED)
		ਵਾਪਸ;

	p->scx.ਭਾਰ = sched_ਭਾਰ_ਮੰਜ਼ਿਲ_cgroup(scale_ਲੱਦੋ_ਥੱਲੇ(lw->ਭਾਰ));
	ਜੇ (SCX_HAS_OP(sch, ਸੈੱਟ_ਭਾਰ))
		SCX_CALL_OP_TASK(sch, ਸੈੱਟ_ਭਾਰ, rq, p, p->scx.ਭਾਰ);
}

ਸਥਿਰ ਖਾਲੀ prio_ਬਦਲਿਆ_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 oldprio)
{
}

ਸਥਿਰ ਖਾਲੀ switching_ਮੰਜ਼ਿਲ_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);

	ਜੇ (task_dead_and_ਪੂਰਾ(p))
		ਵਾਪਸ;

	scx_enable_task(sch, p);

	/*
	 * set_cpus_allowed_scx() is not called while @p is associated with a
	 * different scheduler class. Keep the BPF scheduler up-to-date. The cid
	 * form gets its mask from scx_enable_task().
	 */
	ਜੇ (!scx_is_cid_type() && SCX_HAS_OP(sch, ਸੈੱਟ_cpumask))
		scx_call_op_ਸੈੱਟ_cpumask(sch, rq, p, (ਬਣਤਰ cpumask *)p->cpus_ptr);
}

ਸਥਿਰ ਖਾਲੀ switched_from_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (task_dead_and_ਪੂਰਾ(p))
		ਵਾਪਸ;

	/*
	 * %NONE means SCX is no longer tracking @p at the task level (e.g.
	 * scx_fail_parent() handed @p back to the parent at NONE pending the
	 * parent's own teardown). There is nothing to disable; calling
	 * scx_disable_task() would WARN on the non-%ENABLED state and trigger a
	 * NONE -> READY validation failure.
	 */
	ਜੇ (scx_get_task_state(p) == SCX_TASK_NONE)
		ਵਾਪਸ;

	scx_disable_task(scx_task_sched(p), p);
}

ਸਥਿਰ ਖਾਲੀ switched_ਮੰਜ਼ਿਲ_scx(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) {}

ਪੂਰਨ scx_check_setscheduler(ਬਣਤਰ task_struct *p, ਪੂਰਨ policy)
{
	lockdep_assert_rq_held(task_rq(p));

	/* if disallow, reject transitioning into SCX */
	ਜੇ (scx_enabled() && READ_ONCE(p->scx.disallow) &&
	    p->policy != policy && policy == SCHED_EXT)
		ਵਾਪਸ -EACCES;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ process_ddsp_deferred_locals(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *p;

	lockdep_assert_rq_held(rq);

	/*
	 * Now that @rq can be unlocked, execute the deferred enqueueing of
	 * tasks directly dispatched to the local DSQs of other CPUs. See
	 * direct_dispatch(). Keep popping from the head instead of using
	 * list_for_each_entry_safe() as dispatch_local_dsq() may unlock @rq
	 * temporarily.
	 */
	ਜਦੋਂ ((p = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ_or_null(&rq->scx.ddsp_deferred_locals,
				ਬਣਤਰ task_struct, scx.dsq_list.ਗੰਢ))) {
		ਬਣਤਰ scx_sched *sch = scx_task_sched(p);
		ਬਣਤਰ scx_dispatch_q *dsq;
		u64 dsq_id = p->scx.ddsp_dsq_id;
		u64 enq_flags = p->scx.ddsp_enq_flags;
		u64 ਟੁਕੜਾ = p->scx.ddsp_ਟੁਕੜਾ;
		u64 vtime = p->scx.ddsp_vtime;

		list_del_init(&p->scx.dsq_list.ਗੰਢ);
		clear_ਸਿੱਧਾ_dispatch(p);

		dsq = ਲੱਭੋ_dsq_for_dispatch(sch, rq, dsq_id, task_cpu(p));
		ਜੇ (!WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL))
			dispatch_ਮੰਜ਼ਿਲ_local_dsq(sch, rq, dsq, p, ਟੁਕੜਾ, vtime, enq_flags);
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
ਸਥਿਰ bool local_task_should_reenq(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
				    u64 *reenq_flags, u32 *reason)
{
	bool ਪਹਿਲਾ;

	ਪਹਿਲਾ = !(*reenq_flags & SCX_REENQ_TSR_NOT_ਪਹਿਲਾ);
	*reenq_flags |= SCX_REENQ_TSR_NOT_ਪਹਿਲਾ;

	ਜੇ (unlikely((p->scx.flags & SCX_TASK_PROTECTED) || p == scx_rescuee(rq)))
		ਵਾਪਸ false;

	*reason = SCX_TASK_REENQ_KFUNC;

	ਜੇ ((p->scx.flags & SCX_TASK_IMMED) &&
	    (!ਪਹਿਲਾ || !(*reenq_flags & SCX_REENQ_TSR_RQ_OPEN))) {
		__scx_add_event(scx_task_sched(p), SCX_EV_REENQ_IMMED, 1);
		*reason = SCX_TASK_REENQ_IMMED;
		ਵਾਪਸ true;
	}

	ਜੇ ((*reenq_flags & SCX_REENQ_CAP_REVOKE) &&
	    scx_task_reenq_on_cap_revoke(rq, p)) {
		*reason = SCX_TASK_REENQ_CAP;
		ਵਾਪਸ true;
	}

	ਵਾਪਸ *reenq_flags & SCX_REENQ_ANY;
}

/*
 * The dispatcher runs ops.dequeue() and stores the final ops_state after
 * dropping the DSQ lock, so @p can be found on a DSQ while still
 * %SCX_OPSS_DISPATCHING. Wait for the dispatch to finish before operating on
 * @p.
 */
ਖਾਲੀ scx_wait_dispatching(ਬਣਤਰ task_struct *p)
{
	ਜੇ (unlikely(atomic_long_read_acquire(&p->scx.ops_state) == SCX_OPSS_DISPATCHING))
		wait_ops_state(p, SCX_OPSS_DISPATCHING);
}

ਸਥਿਰ u32 reenq_local(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ rq *rq, u64 reenq_flags)
{
	LIST_ਸਿਰ(tasks);
	u32 nr_enqueued = 0;
	ਬਣਤਰ task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	ਜੇ (WARN_ON_ONCE(reenq_flags & __SCX_REENQ_TSR_MASK))
		reenq_flags &= ~__SCX_REENQ_TSR_MASK;
	ਜੇ (rq_is_open(rq, 0))
		reenq_flags |= SCX_REENQ_TSR_RQ_OPEN;

	/*
	 * The BPF scheduler may choose to dispatch tasks back to
	 * @rq->scx.local_dsq. Move all candidate tasks off to a private list
	 * first to avoid processing the same tasks repeatedly.
	 */
	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(p, n, &rq->scx.local_dsq.list,
				 scx.dsq_list.ਗੰਢ) {
		ਬਣਤਰ scx_sched *task_sch = scx_task_sched(p);
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
		ਜੇ (p->migration_pending)
			ਜਾਰੀ;

		ਜੇ (!scx_is_descendant(task_sch, sch))
			ਜਾਰੀ;

		ਜੇ (!local_task_should_reenq(rq, p, &reenq_flags, &reason))
			ਜਾਰੀ;

		scx_wait_dispatching(p);
		scx_dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p);

		ਜੇ (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_ਨਕਾਬ))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_ਨਕਾਬ;
		p->scx.flags |= reason;

		list_ਜੋੜੋ_ਪੂਛ(&p->scx.dsq_list.ਗੰਢ, &tasks);
	}

	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(p, n, &tasks, scx.dsq_list.ਗੰਢ) {
		list_del_init(&p->scx.dsq_list.ਗੰਢ);

		scx_do_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_ਨਕਾਬ;
		nr_enqueued++;
	}

	/*
	 * The revoke that scheduled this scan may have raced the pick: curr
	 * may be a now-capless task, either one that kept running or one
	 * promoted off the local DSQ between the ecaps sync and this scan.
	 * Zero the slice to evict it. The enqueue gate blocks new capless
	 * inserts, so no later pick can slip through after the scan.
	 */
	ਜੇ ((reenq_flags & SCX_REENQ_CAP_REVOKE) &&
	    rq->curr->sched_class == &ext_sched_class &&
	    scx_task_reenq_on_cap_revoke(rq, rq->curr)) {
		scx_ਸੈੱਟ_task_ਟੁਕੜਾ(rq->curr, 0);
		resched_curr(rq);
	}

	ਵਾਪਸ nr_enqueued;
}

ਸਥਿਰ ਖਾਲੀ process_deferred_reenq_locals(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);

	/*
	 * A task can be re-queued within this loop when a reenqueued task
	 * bounces straight back to the local DSQ. That recursion is bounded by
	 * the per-task reenqueue cap in scx_do_enqueue_task().
	 */
	ਜਦੋਂ (true) {
		ਬਣਤਰ scx_sched *sch;
		u64 reenq_flags;

		scoped_guard (raw_spinlock, &rq->scx.deferred_reenq_ਤਾਲਾ) {
			ਬਣਤਰ scx_deferred_reenq_local *drl =
				list_ਪਹਿਲਾ_ਇੰਦਰਾਜ_or_null(&rq->scx.deferred_reenq_locals,
							 ਬਣਤਰ scx_deferred_reenq_local,
							 ਗੰਢ);
			ਬਣਤਰ scx_sched_pcpu *sch_pcpu;

			ਜੇ (!drl)
				ਵਾਪਸ;

			sch_pcpu = container_ਦਾ(drl, ਬਣਤਰ scx_sched_pcpu,
						deferred_reenq_local);
			sch = sch_pcpu->sch;

			reenq_flags = drl->flags;
			WRITE_ONCE(drl->flags, 0);
			list_del_init(&drl->ਗੰਢ);
		}

		/* see schedule_dsq_reenq() */
		smp_mb();

		reenq_local(sch, rq, reenq_flags);
	}
}

ਸਥਿਰ bool user_task_should_reenq(ਬਣਤਰ task_struct *p, u64 reenq_flags, u32 *reason)
{
	*reason = SCX_TASK_REENQ_KFUNC;
	ਵਾਪਸ reenq_flags & SCX_REENQ_ANY;
}

ਸਥਿਰ ਖਾਲੀ reenq_user(ਬਣਤਰ rq *rq, ਬਣਤਰ scx_dispatch_q *dsq, u64 reenq_flags)
{
	ਬਣਤਰ rq *locked_rq = rq;
	ਬਣਤਰ scx_sched *sch = dsq->sched;
	ਬਣਤਰ scx_dsq_list_ਗੰਢ cursor = INIT_DSQ_LIST_CURSOR(cursor, dsq, 0);
	ਬਣਤਰ task_struct *p;
	s32 nr_enqueued = 0;

	lockdep_assert_rq_held(rq);

	raw_spin_ਤਾਲਾ(&dsq->ਤਾਲਾ);

	ਜਦੋਂ (likely(!READ_ONCE(sch->bypass_ਡੂੰਘਾਈ))) {
		ਬਣਤਰ rq *task_rq;
		u32 reason;

		p = nldsq_cursor_ਅਗਲਾ_task(&cursor, dsq);
		ਜੇ (!p)
			ਰੋਕੋ;

		ਜੇ (!user_task_should_reenq(p, reenq_flags, &reason))
			ਜਾਰੀ;

		task_rq = task_rq(p);

		ਜੇ (locked_rq != task_rq) {
			ਜੇ (locked_rq) {
				scx_rq_ਤਾਲਾ_drop(locked_rq);
				raw_spin_rq_unlock(locked_rq);
			}
			ਜੇ (unlikely(!raw_spin_rq_trylock(task_rq))) {
				raw_spin_unlock(&dsq->ਤਾਲਾ);
				raw_spin_rq_ਤਾਲਾ(task_rq);
				raw_spin_ਤਾਲਾ(&dsq->ਤਾਲਾ);
			}
			locked_rq = task_rq;

			/* did we lose @p while switching locks? */
			ਜੇ (nldsq_cursor_ਗੁਆਚਾ_task(&cursor, task_rq, dsq, p))
				ਜਾਰੀ;
		}

		/* @p is on @dsq, its rq and @dsq are locked */
		scx_wait_dispatching(p);
		dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_locked(p, dsq);
		raw_spin_unlock(&dsq->ਤਾਲਾ);

		ਜੇ (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_ਨਕਾਬ))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_ਨਕਾਬ;
		p->scx.flags |= reason;

		scx_do_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(task_rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_ਨਕਾਬ;

		ਜੇ (!(++nr_enqueued % SCX_TASK_ITER_BATCH)) {
			scx_rq_ਤਾਲਾ_drop(locked_rq);
			raw_spin_rq_unlock(locked_rq);
			locked_rq = ਨਿਰਾ;
			cpu_relax();
		}

		raw_spin_ਤਾਲਾ(&dsq->ਤਾਲਾ);
	}

	list_del_init(&cursor.ਗੰਢ);
	raw_spin_unlock(&dsq->ਤਾਲਾ);

	ਜੇ (locked_rq != rq) {
		ਜੇ (locked_rq) {
			scx_rq_ਤਾਲਾ_drop(locked_rq);
			raw_spin_rq_unlock(locked_rq);
		}
		raw_spin_rq_ਤਾਲਾ(rq);
	}
}

ਸਥਿਰ ਖਾਲੀ process_deferred_reenq_users(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);

	ਜਦੋਂ (true) {
		ਬਣਤਰ scx_dispatch_q *dsq;
		u64 dsq_id, reenq_flags;

		scoped_guard (raw_spinlock, &rq->scx.deferred_reenq_ਤਾਲਾ) {
			ਬਣਤਰ scx_deferred_reenq_user *dru =
				list_ਪਹਿਲਾ_ਇੰਦਰਾਜ_or_null(&rq->scx.deferred_reenq_users,
							 ਬਣਤਰ scx_deferred_reenq_user,
							 ਗੰਢ);
			ਬਣਤਰ scx_dsq_pcpu *dsq_pcpu;

			ਜੇ (!dru)
				ਵਾਪਸ;

			dsq_pcpu = container_ਦਾ(dru, ਬਣਤਰ scx_dsq_pcpu,
						deferred_reenq_user);
			dsq = dsq_pcpu->dsq;
			reenq_flags = dru->flags;
			WRITE_ONCE(dru->flags, 0);
			list_del_init(&dru->ਗੰਢ);
		}

		/* see schedule_dsq_reenq() */
		smp_mb();

		/* destroy_dsq() may have raced and invalidated @dsq, nothing to reenq */
		dsq_id = READ_ONCE(dsq->id);
		ਜੇ (unlikely(dsq_id == SCX_DSQ_INVALID))
			ਜਾਰੀ;

		BUG_ON(dsq_id & SCX_DSQ_FLAG_BUILTIN);
		reenq_user(rq, dsq, reenq_flags);
	}
}

ਸਥਿਰ ਖਾਲੀ ਲੜੀਵਾਰ_deferred(ਬਣਤਰ rq *rq)
{
	process_ddsp_deferred_locals(rq);

	ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&rq->scx.deferred_reenq_locals))
		process_deferred_reenq_locals(rq);

	ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&rq->scx.deferred_reenq_users))
		process_deferred_reenq_users(rq);

	scx_reenq_reject(rq);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_ਭਰਿਆ
bool scx_can_stop_tick(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *p = rq->curr;
	ਬਣਤਰ scx_sched *sch = scx_task_sched(p);

	ਜੇ (p->sched_class != &ext_sched_class)
		ਵਾਪਸ true;

	/*
	 * @rq->curr may still reference an outgoing EXT task after it has been
	 * dequeued. If no EXT tasks are accounted on @rq, ignore its stale
	 * slice state. If another task is dispatched from a DSQ,
	 * set_next_task_scx() will update the dependency for the incoming task.
	 */
	ਜੇ (!rq->scx.nr_running)
		ਵਾਪਸ true;

	ਜੇ (scx_bypassing(sch, cpu_ਦਾ(rq)))
		ਵਾਪਸ false;

	/*
	 * A running rescuee's charging and expiry are tick-driven, see
	 * scx_rescue_charge(). Keep the tick while rescue is in progress.
	 */
	ਜੇ (unlikely(p == scx_rescuee(rq)))
		ਵਾਪਸ false;

	/*
	 * @rq can dispatch from different DSQs, so we can't tell whether it
	 * needs the tick or not by looking at nr_running. Allow stopping ticks
	 * iff the BPF scheduler indicated so. See set_next_task_scx().
	 */
	ਵਾਪਸ rq->scx.flags & SCX_RQ_CAN_STOP_TICK;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED

DEFINE_STATIC_PERCPU_RWSEM(scx_cgroup_ops_rwsem);

ਖਾਲੀ scx_tg_init(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	tg->scx.ਭਾਰ = CGROUP_ਭਾਰ_DFL;
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
ਸਥਿਰ ਬਣਤਰ scx_sched *scx_tg_sched(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	lockdep_assert(lockdep_is_held(&cgroup_mutex) ||
		       lockdep_is_held(&scx_cgroup_ops_rwsem));

	ਜੇ (!tg->css.cgroup)
		tg = &ਜੜ੍ਹ_task_ਸਮੂਹ;
	/* INITED means ops.cgroup_init() succeeded on @tg->scx.sched */
	ਵਾਪਸ (tg->scx.flags & SCX_TG_INITED) ? tg->scx.sched : ਨਿਰਾ;
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
ਸਥਿਰ ਬਣਤਰ scx_sched *scx_tg_knob_sched(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	lockdep_assert(lockdep_is_held(&cgroup_mutex) ||
		       lockdep_is_held(&scx_cgroup_ops_rwsem));

	ਜੇ (!tg->css.cgroup || !tg->css.ਮਾਪਾ)
		ਵਾਪਸ scx_tg_sched(&ਜੜ੍ਹ_task_ਸਮੂਹ);
	ਵਾਪਸ scx_tg_sched(css_tg(tg->css.ਮਾਪਾ));
}

ਪੂਰਨ scx_tg_online(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਪੂਰਨ ret = 0;

	WARN_ON_ONCE(tg->scx.flags & (SCX_TG_ONLINE | SCX_TG_INITED));

	ਜੇ (scx_cgroup_enabled) {
		ਬਣਤਰ scx_sched *sch;

		/*
		 * The cgroup lifetime notifier populates cgrp->scx_sched before
		 * css_online, but only on the default hierarchy. Sub-scheds are
		 * attached to the cgroup2 hierarchy, so a cgroup1 task_group
		 * always belongs to the root sched.
		 */
		ਜੇ (cgroup_on_dfl(tg->css.cgroup))
			sch = scx_cgroup_sched(tg->css.cgroup);
		ਨਹੀਂਤਾਂ
			sch = scx_tg_sched(&ਜੜ੍ਹ_task_ਸਮੂਹ);

		ਜੇ (SCX_HAS_OP(sch, cgroup_init)) {
			ਬਣਤਰ scx_cgroup_init_ਦਲੀਲਾਂ ਦਲੀਲਾਂ =
				{ .ਭਾਰ = tg->scx.ਭਾਰ,
				  .bw_period_us = tg->scx.bw_period_us,
				  .bw_quota_us = tg->scx.bw_quota_us,
				  .bw_ਮਿਆਦ_us = tg->scx.bw_ਮਿਆਦ_us,
				  .sched_idle = tg->scx.idle };

			ret = SCX_CALL_OP_RET(sch, cgroup_init,
					      ਨਿਰਾ, tg->css.cgroup, &ਦਲੀਲਾਂ);
			ਜੇ (ret)
				ret = scx_ops_sanitize_err(sch, "cgroup_init", ret);
		}
		ਜੇ (ret == 0) {
			tg->scx.sched = sch;
			tg->scx.flags |= SCX_TG_ONLINE | SCX_TG_INITED;
		}
	} ਨਹੀਂਤਾਂ {
		tg->scx.flags |= SCX_TG_ONLINE;
	}

	ਵਾਪਸ ret;
}

ਖਾਲੀ scx_tg_offline(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਬਣਤਰ scx_sched *sch = tg->scx.sched;

	WARN_ON_ONCE(!(tg->scx.flags & SCX_TG_ONLINE));

	/* INITED implies non-NULL @sch, test before SCX_HAS_OP() derefs */
	ਜੇ (scx_cgroup_enabled && (tg->scx.flags & SCX_TG_INITED) &&
	    SCX_HAS_OP(sch, cgroup_exit))
		SCX_CALL_OP(sch, cgroup_exit, ਨਿਰਾ, tg->css.cgroup);
	tg->scx.sched = ਨਿਰਾ;
	tg->scx.flags &= ~(SCX_TG_ONLINE | SCX_TG_INITED);
}

/*
 * @p's sched for the cgroup migration paths. Stable as re-homes happen either
 * at CGROUP_TASK_MIGRATED of the same migration or under scx_cgroup_lock(),
 * both while holding cgroup_mutex.
 */
ਸਥਿਰ ਬਣਤਰ scx_sched *scx_cgroup_task_sched(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ rcu_dereference_protected(p->scx.sched, lockdep_is_held(&cgroup_mutex));
}

ਪੂਰਨ scx_cgroup_can_attach(ਬਣਤਰ cgroup_taskset *tset)
{
	ਬਣਤਰ cgroup_subsys_state *css;
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ ret;

	ਜੇ (!scx_cgroup_enabled)
		ਵਾਪਸ 0;

	cgroup_taskset_for_each(p, css, tset) {
		ਬਣਤਰ scx_sched *sch = scx_cgroup_task_sched(p);
		ਬਣਤਰ cgroup *from = tg_cgrp(task_ਸਮੂਹ(p));
		ਬਣਤਰ cgroup *ਮੰਜ਼ਿਲ = tg_cgrp(css_tg(css));

		WARN_ON_ONCE(p->scx.cgrp_moving_from);

		/*
		 * sched_move_task() omits identity migrations. Let's match the
		 * behavior so that ops.cgroup_prep_move() and ops.cgroup_move()
		 * always match one-to-one.
		 */
		ਜੇ (from == ਮੰਜ਼ਿਲ)
			ਜਾਰੀ;

		/*
		 * The cgroup_move ops are delivered to @p's sched, and only for
		 * moves that don't re-home @p. A re-homing move changes the dfl
		 * cgroup's sched and is reported through the
		 * exit_task/init_task pair that the re-homing generates.
		 */
		ਜੇ (!sch || sch != scx_cgroup_sched(task_css_ਸੈੱਟ(p)->mg_dst_cset->dfl_cgrp))
			ਜਾਰੀ;

		ਜੇ (SCX_HAS_OP(sch, cgroup_prep_move)) {
			ret = SCX_CALL_OP_RET(sch, cgroup_prep_move, ਨਿਰਾ,
					      p, from, css->cgroup);
			ਜੇ (ret) {
				ret = scx_ops_sanitize_err(sch, "cgroup_prep_move", ret);
				ਜਾਓ err;
			}
		}

		p->scx.cgrp_moving_from = from;
	}

	ਵਾਪਸ 0;

err:
	cgroup_taskset_for_each(p, css, tset) {
		ਬਣਤਰ scx_sched *sch = scx_cgroup_task_sched(p);

		/* cgrp_moving_from implies non-NULL @sch, test it first */
		ਜੇ (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_cancel_move))
			SCX_CALL_OP(sch, cgroup_cancel_move, ਨਿਰਾ,
				    p, p->scx.cgrp_moving_from, css->cgroup);
		p->scx.cgrp_moving_from = ਨਿਰਾ;
	}

	ਵਾਪਸ ret;
}

ਖਾਲੀ scx_cgroup_move_task(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ scx_sched *sch;

	ਜੇ (!scx_cgroup_enabled)
		ਵਾਪਸ;

	/*
	 * Migration keys off css rather than cgroup identity, so it can hand an
	 * unchanged-cgroup task here with cgrp_moving_from NULL. Nothing to
	 * report to the BPF scheduler then, so skip it and keep prep_move and
	 * move paired.
	 */
	sch = scx_cgroup_task_sched(p);
	ਜੇ (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_move))
		SCX_CALL_OP_TASK(sch, cgroup_move, task_rq(p),
				 p, p->scx.cgrp_moving_from,
				 tg_cgrp(task_ਸਮੂਹ(p)));
	p->scx.cgrp_moving_from = ਨਿਰਾ;
}

ਖਾਲੀ scx_cgroup_cancel_attach(ਬਣਤਰ cgroup_taskset *tset)
{
	ਬਣਤਰ cgroup_subsys_state *css;
	ਬਣਤਰ task_struct *p;

	ਜੇ (!scx_cgroup_enabled)
		ਵਾਪਸ;

	cgroup_taskset_for_each(p, css, tset) {
		ਬਣਤਰ scx_sched *sch = scx_cgroup_task_sched(p);

		/* cgrp_moving_from implies non-NULL @sch, test it first */
		ਜੇ (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_cancel_move))
			SCX_CALL_OP(sch, cgroup_cancel_move, ਨਿਰਾ,
				    p, p->scx.cgrp_moving_from, css->cgroup);
		p->scx.cgrp_moving_from = ਨਿਰਾ;
	}
}

ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_ਭਾਰ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ)
{
	ਬਣਤਰ scx_sched *sch;

	percpu_ਥੱਲੇ_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	ਜੇ (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_ਸੈੱਟ_ਭਾਰ) &&
	    tg->scx.ਭਾਰ != ਭਾਰ)
		SCX_CALL_OP(sch, cgroup_ਸੈੱਟ_ਭਾਰ, ਨਿਰਾ, tg_cgrp(tg), ਭਾਰ);

	tg->scx.ਭਾਰ = ਭਾਰ;

	percpu_ਉੱਪਰ_read(&scx_cgroup_ops_rwsem);
}

ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_idle(ਬਣਤਰ task_ਸਮੂਹ *tg, bool idle)
{
	ਬਣਤਰ scx_sched *sch;

	percpu_ਥੱਲੇ_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	ਜੇ (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_ਸੈੱਟ_idle) &&
	    tg->scx.idle != idle)
		SCX_CALL_OP(sch, cgroup_ਸੈੱਟ_idle, ਨਿਰਾ, tg_cgrp(tg), idle);

	/* Update the task group's idle state */
	tg->scx.idle = idle;

	percpu_ਉੱਪਰ_read(&scx_cgroup_ops_rwsem);
}

ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg,
			     u64 period_us, u64 quota_us, u64 ਮਿਆਦ_us)
{
	ਬਣਤਰ scx_sched *sch;

	percpu_ਥੱਲੇ_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	ਜੇ (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_ਸੈੱਟ_bandwidth) &&
	    (tg->scx.bw_period_us != period_us ||
	     tg->scx.bw_quota_us != quota_us ||
	     tg->scx.bw_ਮਿਆਦ_us != ਮਿਆਦ_us))
		SCX_CALL_OP(sch, cgroup_ਸੈੱਟ_bandwidth, ਨਿਰਾ,
			    tg_cgrp(tg), period_us, quota_us, ਮਿਆਦ_us);

	tg->scx.bw_period_us = period_us;
	tg->scx.bw_quota_us = quota_us;
	tg->scx.bw_ਮਿਆਦ_us = ਮਿਆਦ_us;

	percpu_ਉੱਪਰ_read(&scx_cgroup_ops_rwsem);
}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_GROUP_SCHED */

#ਜੇ defined(CONFIG_EXT_ਸਮੂਹ_SCHED) || defined(CONFIG_EXT_SUB_SCHED)
ਸਥਿਰ ਬਣਤਰ cgroup *ਜੜ੍ਹ_cgroup(ਖਾਲੀ)
{
	ਵਾਪਸ &cgrp_dfl_ਜੜ੍ਹ.cgrp;
}

/*
 * cgroup_lock() must nest outside the rwsem write side: a writer waiting
 * for cgroup_mutex deadlocks with cgroup teardown, which holds it while
 * draining a set_* file write blocked on the rwsem behind the writer.
 */
ਖਾਲੀ scx_cgroup_ਤਾਲਾ(ਖਾਲੀ)
{
	cgroup_ਤਾਲਾ();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	percpu_ਥੱਲੇ_write(&scx_cgroup_ops_rwsem);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ scx_cgroup_unlock(ਖਾਲੀ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	percpu_ਉੱਪਰ_write(&scx_cgroup_ops_rwsem);
#ਜੇ_ਅੰਤ
	cgroup_unlock();
}
#ਨਹੀਂਤਾਂ	/* CONFIG_EXT_GROUP_SCHED || CONFIG_EXT_SUB_SCHED */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cgroup *ਜੜ੍ਹ_cgroup(ਖਾਲੀ) { ਵਾਪਸ ਨਿਰਾ; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_cgroup_ਤਾਲਾ(ਖਾਲੀ) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_cgroup_unlock(ਖਾਲੀ) {}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_GROUP_SCHED || CONFIG_EXT_SUB_SCHED */

/*
 * Omitted operations:
 *
 * - migrate_task_rq: Unnecessary as task to cpu mapping is transient.
 *
 * - task_fork/dead: We need fork/dead notifications for all tasks regardless of
 *   their current sched_class. Call them directly from sched core instead.
 */
DEFINE_SCHED_CLASS(ext) = {
	.ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task		= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_scx,
	.ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task		= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_scx,
	.yield_task		= yield_task_scx,
	.yield_ਮੰਜ਼ਿਲ_task		= yield_ਮੰਜ਼ਿਲ_task_scx,

	.wakeup_preempt		= wakeup_preempt_scx,

	.pick_task		= pick_task_scx,

	.put_prev_task		= put_prev_task_scx,
	.ਸੈੱਟ_ਅਗਲਾ_task		= ਸੈੱਟ_ਅਗਲਾ_task_scx,

	.select_task_rq		= select_task_rq_scx,
	.task_woken		= task_woken_scx,
	.ਸੈੱਟ_cpus_allowed	= ਸੈੱਟ_cpus_allowed_scx,

	.rq_online		= rq_online_scx,
	.rq_offline		= rq_offline_scx,

	.task_tick		= task_tick_scx,

	.switching_ਮੰਜ਼ਿਲ		= switching_ਮੰਜ਼ਿਲ_scx,
	.switched_from		= switched_from_scx,
	.switched_ਮੰਜ਼ਿਲ		= switched_ਮੰਜ਼ਿਲ_scx,
	.reweight_task		= reweight_task_scx,
	.prio_ਬਦਲਿਆ		= prio_ਬਦਲਿਆ_scx,

	.update_curr		= update_curr_scx,

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#ਜੇ_ਅੰਤ
};

s32 scx_init_dsq(ਬਣਤਰ scx_dispatch_q *dsq, u64 dsq_id, ਬਣਤਰ scx_sched *sch)
{
	s32 cpu;

	memset(dsq, 0, ਆਕਾਰ(*dsq));

	raw_spin_ਤਾਲਾ_init(&dsq->ਤਾਲਾ);
	INIT_LIST_ਸਿਰ(&dsq->list);
	dsq->id = dsq_id;
	dsq->sched = sch;

	dsq->pcpu = alloc_percpu(ਬਣਤਰ scx_dsq_pcpu);
	ਜੇ (!dsq->pcpu)
		ਵਾਪਸ -ENOMEM;

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_dsq_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(dsq->pcpu, cpu);

		pcpu->dsq = dsq;
		INIT_LIST_ਸਿਰ(&pcpu->deferred_reenq_user.ਗੰਢ);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ exit_dsq(ਬਣਤਰ scx_dispatch_q *dsq)
{
	s32 cpu;

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_dsq_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(dsq->pcpu, cpu);
		ਬਣਤਰ scx_deferred_reenq_user *dru = &pcpu->deferred_reenq_user;
		ਬਣਤਰ rq *rq = cpu_rq(cpu);

		/*
		 * free_dsq_irq_workfn() cancelled the reenqs before the grace
		 * period and schedule_dsq_reenq() queues nothing after that.
		 */
		ਜੇ (WARN_ON_ONCE(!list_ਖਾਲੀ_ਥਾਵਾਂ(&dru->ਗੰਢ))) {
			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ਤਾਲਾ);
			list_del_init(&dru->ਗੰਢ);
		}
	}

	free_percpu(dsq->pcpu);
}

ਸਥਿਰ ਖਾਲੀ free_dsq_rcufn(ਬਣਤਰ rcu_ਸਿਰ *rcu)
{
	ਬਣਤਰ scx_dispatch_q *dsq = container_ਦਾ(rcu, ਬਣਤਰ scx_dispatch_q, rcu);

	exit_dsq(dsq);
	kfree(dsq);
}

ਸਥਿਰ ਖਾਲੀ free_dsq_irq_workfn(ਬਣਤਰ irq_work *irq_work)
{
	ਬਣਤਰ llist_ਗੰਢ *ਮੰਜ਼ਿਲ_free = llist_del_all(&dsqs_ਮੰਜ਼ਿਲ_free);
	ਬਣਤਰ scx_dispatch_q *dsq, *tmp_dsq;

	llist_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(dsq, tmp_dsq, ਮੰਜ਼ਿਲ_free, free_ਗੰਢ) {
		s32 cpu;

		/*
		 * Cancel pending reenqs. Only a run_deferred() that started
		 * before the grace period can hold one, and it keeps IRQs off,
		 * so the grace period waits for it. After this sweep,
		 * schedule_dsq_reenq() sees the invalid id under the same lock
		 * and queues nothing.
		 */
		for_each_possible_cpu(cpu) {
			ਬਣਤਰ scx_dsq_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(dsq->pcpu, cpu);
			ਬਣਤਰ rq *rq = cpu_rq(cpu);

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_ਤਾਲਾ);
			list_del_init(&pcpu->deferred_reenq_user.ਗੰਢ);
		}

		call_rcu(&dsq->rcu, free_dsq_rcufn);
	}
}

ਸਥਿਰ DEFINE_IRQ_WORK(free_dsq_irq_work, free_dsq_irq_workfn);

ਸਥਿਰ ਖਾਲੀ destroy_dsq(ਬਣਤਰ scx_sched *sch, u64 dsq_id)
{
	ਬਣਤਰ scx_dispatch_q *dsq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	rcu_read_ਤਾਲਾ();

	dsq = ਲੱਭੋ_user_dsq(sch, dsq_id);
	ਜੇ (!dsq)
		ਜਾਓ ਬਾਹਰਲਾ_unlock_rcu;

	raw_spin_ਤਾਲਾ_irqsave(&dsq->ਤਾਲਾ, flags);

	ਜੇ (dsq->nr) {
		scx_error(sch, "attempting to destroy in-use dsq 0x%016llx (nr=%u)",
			  dsq->id, dsq->nr);
		ਜਾਓ ਬਾਹਰਲਾ_unlock_dsq;
	}

	ਜੇ (rhashtable_remove_fast(&sch->dsq_hash, &dsq->hash_ਗੰਢ,
				   dsq_hash_params))
		ਜਾਓ ਬਾਹਰਲਾ_unlock_dsq;

	/*
	 * Mark dead by invalidating ->id to prevent scx_dispatch_enqueue() from
	 * queueing more tasks. As this function can be called from anywhere,
	 * freeing is bounced through an irq work to avoid nesting RCU
	 * operations inside scheduler locks.
	 */
	dsq->id = SCX_DSQ_INVALID;
	ਜੇ (llist_ਜੋੜੋ(&dsq->free_ਗੰਢ, &dsqs_ਮੰਜ਼ਿਲ_free))
		irq_work_ਕਤਾਰ(&free_dsq_irq_work);

ਬਾਹਰਲਾ_unlock_dsq:
	raw_spin_unlock_irqrestore(&dsq->ਤਾਲਾ, flags);
ਬਾਹਰਲਾ_unlock_rcu:
	rcu_read_unlock();
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
ਸਥਿਰ ਖਾਲੀ scx_cgroup_exit(ਬਣਤਰ scx_sched *sch)
{
	ਬਣਤਰ cgroup_subsys_state *css;

	/*
	 * scx_tg_on/offline() are excluded through cgroup_lock(). If we walk
	 * cgroups and exit all the inited ones, all online cgroups are exited.
	 */
	css_for_each_descendant_post(css, &ਜੜ੍ਹ_task_ਸਮੂਹ.css) {
		ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);

		/* also clear the sched of tgs whose ops.cgroup_init() failed */
		tg->scx.sched = ਨਿਰਾ;
		ਜੇ (tg->scx.flags & SCX_TG_INITED) {
			tg->scx.flags &= ~SCX_TG_INITED;
			ਜੇ (sch->ops.cgroup_exit)
				SCX_CALL_OP(sch, cgroup_exit, ਨਿਰਾ, css->cgroup);
		}
	}
}

ਸਥਿਰ ਪੂਰਨ scx_cgroup_init(ਬਣਤਰ scx_sched *sch)
{
	ਬਣਤਰ cgroup_subsys_state *css;
	ਪੂਰਨ ret;

	/*
	 * scx_tg_on/offline() are excluded through cgroup_lock(). If we walk
	 * cgroups and init, all online cgroups are initialized.
	 */
	css_for_each_descendant_pre(css, &ਜੜ੍ਹ_task_ਸਮੂਹ.css) {
		ਬਣਤਰ task_ਸਮੂਹ *tg = css_tg(css);

		ਜੇ ((tg->scx.flags & (SCX_TG_ONLINE | SCX_TG_INITED)) != SCX_TG_ONLINE)
			ਜਾਰੀ;

		ਜੇ (sch->ops.cgroup_init) {
			ਬਣਤਰ scx_cgroup_init_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
				.ਭਾਰ = tg->scx.ਭਾਰ,
				.bw_period_us = tg->scx.bw_period_us,
				.bw_quota_us = tg->scx.bw_quota_us,
				.bw_ਮਿਆਦ_us = tg->scx.bw_ਮਿਆਦ_us,
				.sched_idle = tg->scx.idle,
			};

			ret = SCX_CALL_OP_RET(sch, cgroup_init, ਨਿਰਾ, css->cgroup, &ਦਲੀਲਾਂ);
			ਜੇ (ret) {
				scx_error(sch, "ops.cgroup_init() failed (%d)", ret);
				ਵਾਪਸ ret;
			}
		}

		tg->scx.sched = sch;
		tg->scx.flags |= SCX_TG_INITED;
	}

	ਵਾਪਸ 0;
}

#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਖਾਲੀ scx_cgroup_exit(ਬਣਤਰ scx_sched *sch) {}
ਸਥਿਰ ਪੂਰਨ scx_cgroup_init(ਬਣਤਰ scx_sched *sch) { ਵਾਪਸ 0; }
#ਜੇ_ਅੰਤ


/********************************************************************************
 * Sysfs interface and ops enable/disable.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCX_ATTR(_name)								\
	ਸਥਿਰ ਬਣਤਰ kobj_attribute scx_attr_##_name = {			\
		.attr = { .name = __stringify(_name), .ਬਹੁਲਕ = 0444 },		\
		.show = scx_attr_##_name##_show,				\
	}

ਸਥਿਰ ssize_t scx_attr_state_show(ਬਣਤਰ kobject *kobj,
				   ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਵਾਪਸ sysfs_emit(buf, "%s\n", scx_enable_state_str[scx_enable_state()]);
}
SCX_ATTR(state);

ਸਥਿਰ ssize_t scx_attr_switch_all_show(ਬਣਤਰ kobject *kobj,
					ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਵਾਪਸ sysfs_emit(buf, "%d\n", READ_ONCE(scx_switching_all));
}
SCX_ATTR(switch_all);

ਸਥਿਰ ssize_t scx_attr_nr_rejected_show(ਬਣਤਰ kobject *kobj,
					 ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਵਾਪਸ sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_nr_rejected));
}
SCX_ATTR(nr_rejected);

ਸਥਿਰ ssize_t scx_attr_hotplug_seq_show(ਬਣਤਰ kobject *kobj,
					 ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਵਾਪਸ sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_hotplug_seq));
}
SCX_ATTR(hotplug_seq);

ਸਥਿਰ ssize_t scx_attr_enable_seq_show(ਬਣਤਰ kobject *kobj,
					ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਵਾਪਸ sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_enable_seq));
}
SCX_ATTR(enable_seq);

ਸਥਿਰ ਬਣਤਰ attribute *scx_global_attrs[] = {
	&scx_attr_state.attr,
	&scx_attr_switch_all.attr,
	&scx_attr_nr_rejected.attr,
	&scx_attr_hotplug_seq.attr,
	&scx_attr_enable_seq.attr,
	ਨਿਰਾ,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ attribute_ਸਮੂਹ scx_global_attr_ਸਮੂਹ = {
	.attrs = scx_global_attrs,
};

ਸਥਿਰ ਖਾਲੀ free_pnode(ਬਣਤਰ scx_sched_pnode *pnode);
ਸਥਿਰ ਖਾਲੀ free_exit_info(ਬਣਤਰ scx_exit_info *ei);
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *scx_exit_reason(ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ);
ਸਥਿਰ bool scx_claim_exit(ਬਣਤਰ scx_sched *sch, ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ);

s32 scx_alloc_kern_arena_objs(ਬਣਤਰ scx_sched *sch)
{
	ਮਾਪ_t ਮਾਪ = struct_ਮਾਪ_t(ਬਣਤਰ scx_cmask, ਬਿੱਟ,
				    SCX_CMASK_NR_ਸ਼ਬਦ(ਅੰਕ_possible_cpus()));
	ਬਣਤਰ scx_cmask *online;
	ਬਣਤਰ scx_cmask_ref ref;
	ਪੂਰਨ cpu;

	/* hotplug stays excluded until the online mask is published */
	lockdep_assert_cpus_held();

	ਜੇ (!sch->is_cid_type || !sch->arena_pool)
		ਵਾਪਸ 0;

	sch->ਸੈੱਟ_cmask_scratch = alloc_percpu(ਬਣਤਰ scx_cmask *);
	ਜੇ (!sch->ਸੈੱਟ_cmask_scratch)
		ਵਾਪਸ -ENOMEM;

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_cmask **ਥਾਂ = ਪ੍ਰਤੀ_cpu_ptr(sch->ਸੈੱਟ_cmask_scratch, cpu);

		*ਥਾਂ = scx_arena_alloc(sch, ਮਾਪ);
		ਜੇ (!*ਥਾਂ)
			ਵਾਪਸ -ENOMEM;
		scx_cmask_init(*ਥਾਂ, 0, ਅੰਕ_possible_cpus());
	}

	/* pack the online mask alongside the scratch masks */
	online = scx_arena_alloc(sch, ਮਾਪ);
	ਜੇ (!online)
		ਵਾਪਸ -ENOMEM;

	scoped_guard(rcu) {
		scx_cmask_ref_init_kern(sch, online, 0, ਅੰਕ_possible_cpus(), &ref);
		scx_cmask_ref_from_cpumask(&ref, cpu_ਸਰਗਰਮ_ਨਕਾਬ);
	}
	sch->online_cmask = online;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ scx_free_kern_arena_objs(ਬਣਤਰ scx_sched *sch)
{
	ਮਾਪ_t ਮਾਪ = struct_ਮਾਪ_t(ਬਣਤਰ scx_cmask, ਬਿੱਟ,
				    SCX_CMASK_NR_ਸ਼ਬਦ(ਅੰਕ_possible_cpus()));
	ਪੂਰਨ cpu;

	scx_arena_free(sch, sch->online_cmask, ਮਾਪ);
	ਜੇ (!sch->ਸੈੱਟ_cmask_scratch)
		ਵਾਪਸ;

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_cmask **ਥਾਂ = ਪ੍ਰਤੀ_cpu_ptr(sch->ਸੈੱਟ_cmask_scratch, cpu);

		scx_arena_free(sch, *ਥਾਂ, ਮਾਪ);
	}
	free_percpu(sch->ਸੈੱਟ_cmask_scratch);
	sch->ਸੈੱਟ_cmask_scratch = ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ scx_sched_free_rcu_work(ਬਣਤਰ work_struct *work)
{
	ਬਣਤਰ rcu_work *rcu_work = ਮੰਜ਼ਿਲ_rcu_work(work);
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(rcu_work, ਬਣਤਰ scx_sched, rcu_work);
	ਬਣਤਰ rhashtable_iter rht_iter;
	ਬਣਤਰ scx_dispatch_q *dsq;
	ਪੂਰਨ cpu, ਗੰਢ;

	irq_work_sync(&sch->propagate_exit_irq_work);
	irq_work_sync(&sch->disable_irq_work);
	kthread_destroy_ਕਾਮਾ(sch->helper);
	timer_shutdown_sync(&sch->bypass_lb_timer);
	free_cpumask_var(sch->bypass_lb_donee_cpumask);
	free_cpumask_var(sch->bypass_lb_resched_cpumask);
	free_cpumask_var(sch->stall_cpus);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	kfree(sch->cgrp_ਰਸਤਾ);
	ਜੇ (sch_cgroup(sch))
		cgroup_put(sch_cgroup(sch));
	ਜੇ (sch->sub_kset)
		kobject_put(&sch->sub_kset->kobj);
	ਜੇ (scx_ਮਾਪਾ(sch))
		kobject_put(&scx_ਮਾਪਾ(sch)->kobj);
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_sched_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(sch->pcpu, cpu);

		/*
		 * $sch would have entered bypass mode before the RCU grace
		 * period. As that blocks new deferrals, all
		 * deferred_reenq_local_node's must be off-list by now.
		 */
		WARN_ON_ONCE(!list_ਖਾਲੀ_ਥਾਵਾਂ(&pcpu->deferred_reenq_local.ਗੰਢ));

		/* remove the queued ecaps sync so the pcpu can be freed */
		scx_discard_ecaps_ਮੰਜ਼ਿਲ_sync(cpu, pcpu);

		/*
		 * Bypass blocks new kicks. Flush the kick irq_work so this
		 * pcpu's to_kick_node is off the list before it is freed.
		 */
		irq_work_sync(&cpu_rq(cpu)->scx.kick_cpus_irq_work);
		WARN_ON_ONCE(!list_ਖਾਲੀ_ਥਾਵਾਂ(&pcpu->ਮੰਜ਼ਿਲ_kick_ਗੰਢ));
		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_kick);
		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle);
		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_preempt);
		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_wait);

		exit_dsq(scx_bypass_dsq(sch, cpu));
	}

	free_percpu(sch->pcpu);

	for_each_ਗੰਢ_state(ਗੰਢ, N_POSSIBLE)
		free_pnode(sch->pnode[ਗੰਢ]);
	kfree(sch->pnode);

	scx_free_pshards(sch);

	rhashtable_walk_enter(&sch->dsq_hash, &rht_iter);
	ਕਰੋ {
		rhashtable_walk_ਸ਼ੁਰੂ(&rht_iter);

		ਜਦੋਂ (!IS_ERR_OR_NULL((dsq = rhashtable_walk_ਅਗਲਾ(&rht_iter))))
			destroy_dsq(sch, dsq->id);

		rhashtable_walk_stop(&rht_iter);
	} ਜਦੋਂ (dsq == ERR_PTR(-EAGAIN));
	rhashtable_walk_exit(&rht_iter);

	rhashtable_free_and_destroy(&sch->dsq_hash, ਨਿਰਾ, ਨਿਰਾ);
	free_exit_info(sch->exit_info);
	scx_free_kern_arena_objs(sch);
	scx_arena_pool_destroy(sch);
	ਜੇ (sch->arena_map)
		bpf_map_put(sch->arena_map);

	/* @sch is completely inactive by now */
	scx_dec_has_subs(sch);

	kfree(sch);
}

ਸਥਿਰ ਖਾਲੀ scx_kobj_release(ਬਣਤਰ kobject *kobj)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(kobj, ਬਣਤਰ scx_sched, kobj);

	INIT_RCU_WORK(&sch->rcu_work, scx_sched_free_rcu_work);
	ਕਤਾਰ_rcu_work(system_dfl_wq, &sch->rcu_work);
}

ਸਥਿਰ ssize_t scx_attr_ops_show(ਬਣਤਰ kobject *kobj,
				 ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(kobj, ਬਣਤਰ scx_sched, kobj);

	ਵਾਪਸ sysfs_emit(buf, "%s\n", sch->ops.name);
}
SCX_ATTR(ops);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_attr_event_show(buf, at, events, ਕਿਸਮ) ({				\
	sysfs_emit_at(buf, at, "%s %llu\n", #kind, (events)->ਕਿਸਮ);		\
})

ਸਥਿਰ ssize_t scx_attr_events_show(ਬਣਤਰ kobject *kobj,
				    ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(kobj, ਬਣਤਰ scx_sched, kobj);
	ਬਣਤਰ scx_event_stats events;
	ਪੂਰਨ at = 0;

	scx_read_events(sch, &events);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCX_EVENT(name)	(at += scx_attr_event_show(buf, at, &events, name))
	SCX_EVENTS_LIST(SCX_EVENT);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCX_EVENT
	ਵਾਪਸ at;
}
SCX_ATTR(events);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *scx_cap_ਨਾਂ[__SCX_NR_CAPS] = {
	[__SCX_CAP_ENQ_IMMED]	= "enq_immed",
	[__SCX_CAP_ENQ]		= "enq",
	[__SCX_CAP_PREEMPT]	= "preempt",
	[__SCX_CAP_PERF]	= "perf",
};

ਸਥਿਰ ssize_t scx_attr_caps_show(ਬਣਤਰ kobject *kobj,
				  ਬਣਤਰ kobj_attribute *ka, ਅੱਖਰ *buf)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(kobj, ਬਣਤਰ scx_sched, kobj);
	u32 npossible = ਅੰਕ_possible_cpus();
	ਬਣਤਰ scx_cmask *agg __free(kfree) =
		kzalloc_flex(*agg, ਬਿੱਟ, SCX_CMASK_NR_ਸ਼ਬਦ(npossible));
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *agg_bm __free(bitmap) = bitmap_zalloc(npossible, GFP_KERNEL);
	ssize_t ਗਿਣਤੀ_2 = 0;
	s32 cap, si;

	ਜੇ (!agg || !agg_bm)
		ਵਾਪਸ -ENOMEM;

	ਲਈ (cap = 0; cap < __SCX_NR_CAPS; cap++) {
		SCX_CMASK_DEFINE(snap, 0, SCX_CID_SHARD_MAX_CPUS);

		scx_cmask_init(agg, 0, npossible);
		ਲਈ (si = 0; si < sch->nr_pshards; si++) {
			ਬਣਤਰ scx_cmask *cm = &sch->pshard[si]->caps[cap].cmask;

			scx_cmask_reframe(snap, cm->ਅਧਾਰ, cm->nr_cids);
			scx_cmask_copy(snap, cm);
			scx_cmask_or(agg, snap);
		}
		/* %*pbl takes unsigned long bitmap layout, convert from u64 */
		bitmap_from_arr64(agg_bm, agg->ਬਿੱਟ, npossible);
		ਗਿਣਤੀ_2 += sysfs_emit_at(buf, ਗਿਣਤੀ_2, "%s: %*pbl\n",
				       scx_cap_ਨਾਂ[cap], npossible, agg_bm);
	}
	ਵਾਪਸ ਗਿਣਤੀ_2;
}
SCX_ATTR(caps);
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

ਸਥਿਰ ਬਣਤਰ attribute *scx_sched_attrs[] = {
	&scx_attr_ops.attr,
	&scx_attr_events.attr,
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	&scx_attr_caps.attr,
#ਜੇ_ਅੰਤ
	ਨਿਰਾ,
};
ATTRIBUTE_GROUPS(scx_sched);

ਸਥਿਰ ਅਚਲ ਬਣਤਰ kobj_type scx_ktype = {
	.release = scx_kobj_release,
	.sysfs_ops = &kobj_sysfs_ops,
	.default_groups = scx_sched_groups,
};

ਸਥਿਰ ਪੂਰਨ scx_uevent(ਅਚਲ ਬਣਤਰ kobject *kobj, ਬਣਤਰ kobj_uevent_env *env)
{
	ਅਚਲ ਬਣਤਰ scx_sched *sch;

	/*
	 * scx_uevent() can be reached by both scx_sched kobjects (scx_ktype)
	 * and sub-scheduler kset kobjects (kset_ktype) through the parent
	 * chain walk. Filter out the latter to avoid invalid casts.
	 */
	ਜੇ (kobj->ktype != &scx_ktype)
		ਵਾਪਸ 0;

	sch = container_ਦਾ(kobj, ਬਣਤਰ scx_sched, kobj);

	ਵਾਪਸ ਜੋੜੋ_uevent_var(env, "SCXOPS=%s", sch->ops.name);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ kset_uevent_ops scx_uevent_ops = {
	.uevent = scx_uevent,
};

/*
 * Used by sched_fork() and __setscheduler_class() to pick the matching
 * sched_class. dl/rt are already handled.
 */
bool task_should_scx(ਪੂਰਨ policy)
{
	/* if disabled, nothing should be on it */
	ਜੇ (!scx_enabled())
		ਵਾਪਸ false;

	/* scx is taking over all SCHED_OTHER and SCHED_EXT tasks */
	ਜੇ (READ_ONCE(scx_switching_all))
		ਵਾਪਸ true;

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
	ਜੇ (unlikely(scx_enable_state() == SCX_DISABLING))
		ਵਾਪਸ false;

	ਵਾਪਸ policy == SCHED_EXT;
}

bool scx_allow_ttwu_ਕਤਾਰ(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ scx_sched *sch;

	ਜੇ (!scx_enabled())
		ਵਾਪਸ true;

	sch = scx_task_sched(p);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ true;

	ਜੇ (sch->ops.flags & SCX_OPS_ALLOW_QUEUED_WAKEUP)
		ਵਾਪਸ true;

	ਜੇ (unlikely(p->sched_class != &ext_sched_class))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
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
ਸਥਿਰ __printf(2, 3) bool handle_lockup(ਪੂਰਨ exit_cpu, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
	ਬਣਤਰ scx_sched *sch;
	va_list ਦਲੀਲਾਂ;
	bool ret;

	guard(rcu)();

	sch = rcu_dereference(scx_ਜੜ੍ਹ);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ false;

	ਚੋਣ (scx_enable_state()) {
	ਹਾਲਤ SCX_ENABLING:
	ਹਾਲਤ SCX_ENABLED:
		va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
		ret = scx_vexit(sch, SCX_EXIT_ERROR, 0, exit_cpu, fmt, ਦਲੀਲਾਂ);
		va_ਸਿਰਾ(ਦਲੀਲਾਂ);
		ਵਾਪਸ ret;
	ਮੂਲ:
		ਵਾਪਸ false;
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
bool scx_rcu_cpu_stall(ਅਚਲ ਬਣਤਰ cpumask *stalled_ਨਕਾਬ)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_exit_info *ei;
	ਪੂਰਨ exit_cpu;

	guard(rcu)();

	sch = rcu_dereference(scx_ਜੜ੍ਹ);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ false;

	ਚੋਣ (scx_enable_state()) {
	ਹਾਲਤ SCX_ENABLING:
	ਹਾਲਤ SCX_ENABLED:
		ਰੋਕੋ;
	ਮੂਲ:
		ਵਾਪਸ false;
	}

	exit_cpu = cpumask_ਖਾਲੀ_ਥਾਵਾਂ(stalled_ਨਕਾਬ) ? -1 : (ਪੂਰਨ)cpumask_ਪਹਿਲਾ(stalled_ਨਕਾਬ);
	ei = sch->exit_info;

	guard(preempt)();

	ਜੇ (!scx_claim_exit(sch, SCX_EXIT_ERROR))
		ਵਾਪਸ false;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_STACKTRACE
	ei->bt_len = ਢੇਰ_trace_save(ei->bt, SCX_EXIT_BT_LEN, 1);
#ਜੇ_ਅੰਤ
	scnprintf(ei->ਸੁਨੇਹਾ, SCX_EXIT_ਸੁਨੇਹਾ_LEN, "RCU CPU stall on CPUs (%*pbl)",
		  cpumask_pr_ਦਲੀਲਾਂ(stalled_ਨਕਾਬ));
	ei->ਕਿਸਮ = SCX_EXIT_ERROR;
	ei->reason = scx_exit_reason(SCX_EXIT_ERROR);
	ei->exit_cpu = exit_cpu;
	cpumask_copy(sch->stall_cpus, stalled_ਨਕਾਬ);

	irq_work_ਕਤਾਰ(&sch->disable_irq_work);
	ਵਾਪਸ true;
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
ਖਾਲੀ scx_softlockup(u32 dur_s)
{
	ਪੂਰਨ cpu = smp_processor_id();

	ਜੇ (!handle_lockup(cpu, "soft lockup - CPU %d stuck for %us", cpu, dur_s))
		ਵਾਪਸ;

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
bool scx_hardlockup(ਪੂਰਨ cpu)
{
	ਜੇ (!handle_lockup(cpu, "hard lockup - CPU %d", cpu))
		ਵਾਪਸ false;

	printk_deferred(KERN_ERR "sched_ext: Hard lockup - CPU %d, disabling BPF scheduler\n",
			cpu);
	ਵਾਪਸ true;
}

ਸਥਿਰ u32 bypass_lb_cpu(ਬਣਤਰ scx_sched *sch, s32 donor,
			 ਬਣਤਰ cpumask *donee_ਨਕਾਬ, ਬਣਤਰ cpumask *resched_ਨਕਾਬ,
			 u32 nr_donor_ਨਿਸ਼ਾਨਾ, u32 nr_donee_ਨਿਸ਼ਾਨਾ)
{
	ਬਣਤਰ rq *donor_rq = cpu_rq(donor);
	ਬਣਤਰ scx_dispatch_q *donor_dsq = scx_bypass_dsq(sch, donor);
	ਬਣਤਰ task_struct *p, *n;
	ਬਣਤਰ scx_dsq_list_ਗੰਢ cursor = INIT_DSQ_LIST_CURSOR(cursor, donor_dsq, 0);
	s32 delta = READ_ONCE(donor_dsq->nr) - nr_donor_ਨਿਸ਼ਾਨਾ;
	u32 nr_ਸੰਤੁਲਿਤ = 0, min_delta_us;

	/*
	 * All we want to guarantee is reasonable forward progress. No reason to
	 * fine tune. Assuming every task on @donor_dsq runs their full slice,
	 * consider offloading iff the total queued duration is over the
	 * threshold.
	 */
	min_delta_us = READ_ONCE(scx_bypass_lb_intv_us) / SCX_BYPASS_LB_MIN_DELTA_DIV;
	ਜੇ (delta < DIV_ਗੇੜਾ_ਉੱਪਰ(min_delta_us, READ_ONCE(scx_ਟੁਕੜਾ_bypass_us)))
		ਵਾਪਸ 0;

	raw_spin_rq_ਤਾਲਾ_irq(donor_rq);
	raw_spin_ਤਾਲਾ(&donor_dsq->ਤਾਲਾ);
	list_ਜੋੜੋ(&cursor.ਗੰਢ, &donor_dsq->list);
resume:
	n = container_ਦਾ(&cursor, ਬਣਤਰ task_struct, scx.dsq_list);
	n = nldsq_ਅਗਲਾ_task(donor_dsq, n, false);

	ਜਦੋਂ ((p = n)) {
		ਬਣਤਰ scx_dispatch_q *donee_dsq;
		ਪੂਰਨ donee;

		n = nldsq_ਅਗਲਾ_task(donor_dsq, n, false);

		ਜੇ (donor_dsq->nr <= nr_donor_ਨਿਸ਼ਾਨਾ)
			ਰੋਕੋ;

		ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(donee_ਨਕਾਬ))
			ਰੋਕੋ;

		/*
		 * If an earlier pass placed @p on @donor_dsq from a different
		 * CPU and the donee hasn't consumed it yet, @p is still on the
		 * previous CPU and task_rq(@p) != @donor_rq. @p can't be moved
		 * without its rq locked. Skip.
		 */
		ਜੇ (task_rq(p) != donor_rq)
			ਜਾਰੀ;

		donee = cpumask_any_and_distribute(donee_ਨਕਾਬ, p->cpus_ptr);
		ਜੇ (donee >= nr_cpu_ids)
			ਜਾਰੀ;

		donee_dsq = scx_bypass_dsq(sch, donee);

		/*
		 * $p's rq is not locked but $p's DSQ lock protects its
		 * scheduling properties making this test safe.
		 */
		ਜੇ (!task_can_ਲੜੀਵਾਰ_on_remote_rq(sch, p, cpu_rq(donee), false))
			ਜਾਰੀ;

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
		dispatch_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_locked(p, donor_dsq);
		scx_dispatch_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(sch, cpu_rq(donee), donee_dsq, p, 0, 0, SCX_ENQ_NESTED);

		/*
		 * $donee might have been idle and need to be woken up. No need
		 * to be clever. Kick every CPU that receives tasks.
		 */
		cpumask_ਸੈੱਟ_cpu(donee, resched_ਨਕਾਬ);

		ਜੇ (READ_ONCE(donee_dsq->nr) >= nr_donee_ਨਿਸ਼ਾਨਾ)
			cpumask_clear_cpu(donee, donee_ਨਕਾਬ);

		nr_ਸੰਤੁਲਿਤ++;
		ਜੇ (!(nr_ਸੰਤੁਲਿਤ % SCX_BYPASS_LB_BATCH) && n) {
			list_move_ਪੂਛ(&cursor.ਗੰਢ, &n->scx.dsq_list.ਗੰਢ);
			raw_spin_unlock(&donor_dsq->ਤਾਲਾ);
			scx_rq_ਤਾਲਾ_drop(donor_rq);
			raw_spin_rq_unlock_irq(donor_rq);
			cpu_relax();
			raw_spin_rq_ਤਾਲਾ_irq(donor_rq);
			raw_spin_ਤਾਲਾ(&donor_dsq->ਤਾਲਾ);
			ਜਾਓ resume;
		}
	}

	list_del_init(&cursor.ਗੰਢ);
	raw_spin_unlock(&donor_dsq->ਤਾਲਾ);
	scx_rq_ਤਾਲਾ_drop(donor_rq);
	raw_spin_rq_unlock_irq(donor_rq);

	ਵਾਪਸ nr_ਸੰਤੁਲਿਤ;
}

ਸਥਿਰ ਖਾਲੀ bypass_lb_ਗੰਢ(ਬਣਤਰ scx_sched *sch, ਪੂਰਨ ਗੰਢ)
{
	ਅਚਲ ਬਣਤਰ cpumask *ਗੰਢ_ਨਕਾਬ = cpumask_ਦਾ_ਗੰਢ(ਗੰਢ);
	ਬਣਤਰ cpumask *donee_ਨਕਾਬ = sch->bypass_lb_donee_cpumask;
	ਬਣਤਰ cpumask *resched_ਨਕਾਬ = sch->bypass_lb_resched_cpumask;
	u32 nr_tasks = 0, nr_cpus = 0, nr_ਸੰਤੁਲਿਤ = 0;
	u32 nr_ਨਿਸ਼ਾਨਾ, nr_donor_ਨਿਸ਼ਾਨਾ;
	u32 before_min = U32_MAX, before_max = 0;
	u32 after_min = U32_MAX, after_max = 0;
	ਪੂਰਨ cpu;

	/* count the target tasks and CPUs */
	for_each_cpu_and(cpu, cpu_online_ਨਕਾਬ, ਗੰਢ_ਨਕਾਬ) {
		u32 nr = READ_ONCE(scx_bypass_dsq(sch, cpu)->nr);

		nr_tasks += nr;
		nr_cpus++;

		before_min = min(nr, before_min);
		before_max = max(nr, before_max);
	}

	ਜੇ (!nr_cpus)
		ਵਾਪਸ;

	/*
	 * We don't want CPUs to have more than $nr_donor_target tasks and
	 * balancing to fill donee CPUs upto $nr_target. Once targets are
	 * calculated, find the donee CPUs.
	 */
	nr_ਨਿਸ਼ਾਨਾ = DIV_ਗੇੜਾ_ਉੱਪਰ(nr_tasks, nr_cpus);
	nr_donor_ਨਿਸ਼ਾਨਾ = DIV_ਗੇੜਾ_ਉੱਪਰ(nr_ਨਿਸ਼ਾਨਾ * SCX_BYPASS_LB_DONOR_PCT, 100);

	cpumask_clear(donee_ਨਕਾਬ);
	for_each_cpu_and(cpu, cpu_online_ਨਕਾਬ, ਗੰਢ_ਨਕਾਬ) {
		ਜੇ (READ_ONCE(scx_bypass_dsq(sch, cpu)->nr) < nr_ਨਿਸ਼ਾਨਾ)
			cpumask_ਸੈੱਟ_cpu(cpu, donee_ਨਕਾਬ);
	}

	/* iterate !donee CPUs and see if they should be offloaded */
	cpumask_clear(resched_ਨਕਾਬ);
	for_each_cpu_and(cpu, cpu_online_ਨਕਾਬ, ਗੰਢ_ਨਕਾਬ) {
		ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(donee_ਨਕਾਬ))
			ਰੋਕੋ;
		ਜੇ (cpumask_test_cpu(cpu, donee_ਨਕਾਬ))
			ਜਾਰੀ;
		ਜੇ (READ_ONCE(scx_bypass_dsq(sch, cpu)->nr) <= nr_donor_ਨਿਸ਼ਾਨਾ)
			ਜਾਰੀ;

		nr_ਸੰਤੁਲਿਤ += bypass_lb_cpu(sch, cpu, donee_ਨਕਾਬ, resched_ਨਕਾਬ,
					     nr_donor_ਨਿਸ਼ਾਨਾ, nr_ਨਿਸ਼ਾਨਾ);
	}

	for_each_cpu(cpu, resched_ਨਕਾਬ)
		resched_cpu(cpu);

	for_each_cpu_and(cpu, cpu_online_ਨਕਾਬ, ਗੰਢ_ਨਕਾਬ) {
		u32 nr = READ_ONCE(scx_bypass_dsq(sch, cpu)->nr);

		after_min = min(nr, after_min);
		after_max = max(nr, after_max);

	}

	trace_sched_ext_bypass_lb(ਗੰਢ, nr_cpus, nr_tasks, nr_ਸੰਤੁਲਿਤ,
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
ਸਥਿਰ ਖਾਲੀ scx_bypass_lb_timerfn(ਬਣਤਰ timer_list *timer)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(timer, ਬਣਤਰ scx_sched, bypass_lb_timer);
	ਪੂਰਨ ਗੰਢ;
	u32 intv_us;

	ਜੇ (!scx_bypass_dsp_enabled(sch))
		ਵਾਪਸ;

	for_each_ਗੰਢ_with_cpus(ਗੰਢ)
		bypass_lb_ਗੰਢ(sch, ਗੰਢ);

	intv_us = READ_ONCE(scx_bypass_lb_intv_us);
	ਜੇ (intv_us)
		mod_timer(timer, jiffies + usecs_ਮੰਜ਼ਿਲ_jiffies(intv_us));
}

ਸਥਿਰ bool inc_bypass_ਡੂੰਘਾਈ(ਬਣਤਰ scx_sched *sch)
{
	lockdep_assert_held(&scx_bypass_ਤਾਲਾ);

	WARN_ON_ONCE(sch->bypass_ਡੂੰਘਾਈ < 0);
	WRITE_ONCE(sch->bypass_ਡੂੰਘਾਈ, sch->bypass_ਡੂੰਘਾਈ + 1);
	ਜੇ (sch->bypass_ਡੂੰਘਾਈ != 1)
		ਵਾਪਸ false;

	WRITE_ONCE(sch->ਟੁਕੜਾ_dfl, READ_ONCE(scx_ਟੁਕੜਾ_bypass_us) * NSEC_ਪ੍ਰਤੀ_USEC);
	sch->bypass_timestamp = ktime_get_ns();
	scx_ਜੋੜੋ_event(sch, SCX_EV_BYPASS_ACTIVATE, 1);
	ਵਾਪਸ true;
}

ਸਥਿਰ bool dec_bypass_ਡੂੰਘਾਈ(ਬਣਤਰ scx_sched *sch)
{
	lockdep_assert_held(&scx_bypass_ਤਾਲਾ);

	WARN_ON_ONCE(sch->bypass_ਡੂੰਘਾਈ < 1);
	WRITE_ONCE(sch->bypass_ਡੂੰਘਾਈ, sch->bypass_ਡੂੰਘਾਈ - 1);
	ਜੇ (sch->bypass_ਡੂੰਘਾਈ != 0)
		ਵਾਪਸ false;

	WRITE_ONCE(sch->ਟੁਕੜਾ_dfl, SCX_ਟੁਕੜਾ_DFL);
	scx_ਜੋੜੋ_event(sch, SCX_EV_BYPASS_DURATION,
		      ktime_get_ns() - sch->bypass_timestamp);
	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ enable_bypass_dsp(ਬਣਤਰ scx_sched *sch)
{
	ਬਣਤਰ scx_sched *host = scx_ਮਾਪਾ(sch) ?: sch;
	u32 intv_us = READ_ONCE(scx_bypass_lb_intv_us);
	s32 ret;

	/*
	 * @sch->bypass_depth transitioning from 0 to 1 triggers enabling.
	 * Shouldn't stagger.
	 */
	ਜੇ (WARN_ON_ONCE(test_and_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(0, &sch->bypass_dsp_claim)))
		ਵਾਪਸ;

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
	ret = atomic_inc_return(&sch->bypass_dsp_enable_ਡੂੰਘਾਈ);
	WARN_ON_ONCE(ret <= 0);

	ਜੇ (host != sch) {
		ret = atomic_inc_return(&host->bypass_dsp_enable_ਡੂੰਘਾਈ);
		WARN_ON_ONCE(ret <= 0);
	}

	/*
	 * The LB timer will stop running if bypass dispatch is disabled. Start
	 * after enabling bypass dispatch.
	 */
	ਜੇ (intv_us && !timer_pending(&host->bypass_lb_timer))
		mod_timer(&host->bypass_lb_timer,
			  jiffies + usecs_ਮੰਜ਼ਿਲ_jiffies(intv_us));
}

/* may be called without holding scx_bypass_lock */
ਖਾਲੀ scx_disable_bypass_dsp(ਬਣਤਰ scx_sched *sch)
{
	s32 ret;

	ਜੇ (!test_and_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(0, &sch->bypass_dsp_claim))
		ਵਾਪਸ;

	ret = atomic_dec_return(&sch->bypass_dsp_enable_ਡੂੰਘਾਈ);
	WARN_ON_ONCE(ret < 0);

	ਜੇ (scx_ਮਾਪਾ(sch)) {
		ret = atomic_dec_return(&scx_ਮਾਪਾ(sch)->bypass_dsp_enable_ਡੂੰਘਾਈ);
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
ਸਥਿਰ ਖਾਲੀ unbypass_renotify_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ scx_sched *pos,
				   ਬਣਤਰ scx_sched_pcpu *pcpu)
{
	ਜੇ (!pos->level) {
		rq->scx.flags |= SCX_RQ_ਜੜ੍ਹ_IDLE_RENOTIFY;
		ਵਾਪਸ;
	}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	pcpu->idle_renotify = true;
	rq->scx.flags |= SCX_RQ_SUB_IDLE_RENOTIFY;
#ਜੇ_ਅੰਤ
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
ਖਾਲੀ scx_bypass(ਬਣਤਰ scx_sched *sch, bool bypass)
{
	ਬਣਤਰ scx_sched *pos;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	ਪੂਰਨ cpu;
	bool ਬਦਲਿਆ;

	raw_spin_ਤਾਲਾ_irqsave(&scx_bypass_ਤਾਲਾ, flags);

	ਜੇ (bypass) {
		ਬਦਲਿਆ = inc_bypass_ਡੂੰਘਾਈ(sch);
		ਜੇ (ਬਦਲਿਆ)
			enable_bypass_dsp(sch);
	} ਨਹੀਂਤਾਂ {
		ਬਦਲਿਆ = dec_bypass_ਡੂੰਘਾਈ(sch);
	}

	/*
	 * Bypass state is propagated to all descendants - an scx_sched bypasses
	 * if itself or any of its ancestors are in bypass mode. A descendant's
	 * depth is the sum of its own bypasses and its ancestors', so every
	 * call propagates whether or not @sch changed state. The descendants'
	 * claims are released below with @sch's, after their tasks have moved.
	 */
	raw_spin_ਤਾਲਾ(&scx_sched_ਤਾਲਾ);
	scx_for_each_descendant_pre(pos, sch) {
		ਜੇ (pos == sch)
			ਜਾਰੀ;
		ਜੇ (bypass) {
			ਜੇ (inc_bypass_ਡੂੰਘਾਈ(pos))
				enable_bypass_dsp(pos);
		} ਨਹੀਂਤਾਂ {
			dec_bypass_ਡੂੰਘਾਈ(pos);
		}
	}
	raw_spin_unlock(&scx_sched_ਤਾਲਾ);

	/*
	 * A descendant's depth is at least @sch's, so no bypass state changed
	 * unless @sch's did.
	 */
	ਜੇ (!ਬਦਲਿਆ)
		ਜਾਓ unlock;

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
		ਬਣਤਰ rq *rq = cpu_rq(cpu);
		ਬਣਤਰ task_struct *p, *n;

		raw_spin_rq_ਤਾਲਾ(rq);
		raw_spin_ਤਾਲਾ(&scx_sched_ਤਾਲਾ);

		scx_for_each_descendant_pre(pos, sch) {
			ਬਣਤਰ scx_sched_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(pos->pcpu, cpu);
			bool was_bypassing = pcpu->flags & SCX_SCHED_PCPU_BYPASSING;

			ਜੇ (pos->bypass_ਡੂੰਘਾਈ) {
				pcpu->flags |= SCX_SCHED_PCPU_BYPASSING;
			} ਨਹੀਂਤਾਂ {
				pcpu->flags &= ~SCX_SCHED_PCPU_BYPASSING;
				ਜੇ (was_bypassing) {
					unbypass_renotify_idle(rq, pos, pcpu);
					scx_unbypass_replay_ecaps(rq, pos);
				}
			}
		}

		raw_spin_unlock(&scx_sched_ਤਾਲਾ);

		/*
		 * We need to guarantee that no tasks are on the BPF scheduler
		 * while bypassing. Either we see enabled or the enable path
		 * sees scx_bypassing() before moving tasks to SCX.
		 */
		ਜੇ (!scx_enabled()) {
			scx_rq_ਤਾਲਾ_drop(rq);
			raw_spin_rq_unlock(rq);
			ਜਾਰੀ;
		}

		/*
		 * The use of list_for_each_entry_safe_reverse() is required
		 * because each task is going to be removed from and added back
		 * to the runnable_list during iteration. Because they're added
		 * to the tail of the list, safe reverse iteration can still
		 * visit all nodes.
		 */
		list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ_ਉਲਟ(p, n, &rq->scx.runnable_list,
						 scx.runnable_ਗੰਢ) {
			ਜੇ (!scx_is_descendant(scx_task_sched(p), sch))
				ਜਾਰੀ;

			/*
			 * Bypass trumps protection. Cycling clears for queued
			 * tasks but current task needs explicit stripping.
			 */
			ਜੇ (bypass && task_current(rq, p))
				scx_task_ਟੁਕੜਾ_ended(rq, p);

			/* cycling deq/enq is enough, see the function comment */
			scoped_guard (sched_change, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE) {
				/* nothing */ ;
			}
		}

		/* resched to restore ticks and idle state */
		ਜੇ (cpu_online(cpu) || cpu == smp_processor_id())
			resched_curr(rq);

		scx_rq_ਤਾਲਾ_drop(rq);
		raw_spin_rq_unlock(rq);
	}

	/* disarming must come after moving all tasks out of the bypass DSQs */
	ਜੇ (!bypass) {
		guard(raw_spinlock)(&scx_sched_ਤਾਲਾ);
		scx_for_each_descendant_pre(pos, sch)
			ਜੇ (!pos->bypass_ਡੂੰਘਾਈ)
				scx_disable_bypass_dsp(pos);
	}
unlock:
	raw_spin_unlock_irqrestore(&scx_bypass_ਤਾਲਾ, flags);
}

ਸਥਿਰ ਖਾਲੀ free_exit_info(ਬਣਤਰ scx_exit_info *ei)
{
	kvfree(ei->dump);
	kfree(ei->ਸੁਨੇਹਾ);
	kfree(ei->bt);
	kfree(ei);
}

ਸਥਿਰ ਬਣਤਰ scx_exit_info *alloc_exit_info(ਮਾਪ_t exit_dump_len)
{
	ਬਣਤਰ scx_exit_info *ei;

	ei = kzalloc_obj(*ei);
	ਜੇ (!ei)
		ਵਾਪਸ ਨਿਰਾ;

	ei->exit_cpu = -1;
	ei->bt = kzalloc_objs(ei->bt[0], SCX_EXIT_BT_LEN);
	ei->ਸੁਨੇਹਾ = kzalloc(SCX_EXIT_ਸੁਨੇਹਾ_LEN, GFP_KERNEL);
	ei->dump = kvzalloc(exit_dump_len, GFP_KERNEL);

	ਜੇ (!ei->bt || !ei->ਸੁਨੇਹਾ || !ei->dump) {
		free_exit_info(ei);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ ei;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *scx_exit_reason(ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ)
{
	ਚੋਣ (ਕਿਸਮ) {
	ਹਾਲਤ SCX_EXIT_UNREG:
		ਵਾਪਸ "unregistered from user space";
	ਹਾਲਤ SCX_EXIT_UNREG_BPF:
		ਵਾਪਸ "unregistered from BPF";
	ਹਾਲਤ SCX_EXIT_UNREG_KERN:
		ਵਾਪਸ "unregistered from the main kernel";
	ਹਾਲਤ SCX_EXIT_SYSRQ:
		ਵਾਪਸ "disabled by sysrq-S";
	ਹਾਲਤ SCX_EXIT_ਮਾਪਾ:
		ਵਾਪਸ "parent exiting";
	ਹਾਲਤ SCX_EXIT_ਮਾਪਾ_KILL:
		ਵਾਪਸ "killed by parent scheduler";
	ਹਾਲਤ SCX_EXIT_ERROR:
		ਵਾਪਸ "runtime error";
	ਹਾਲਤ SCX_EXIT_ERROR_BPF:
		ਵਾਪਸ "scx_bpf_error";
	ਹਾਲਤ SCX_EXIT_ERROR_STALL:
		ਵਾਪਸ "runnable task stall";
	ਹਾਲਤ SCX_EXIT_ERROR_REENQ:
		ਵਾਪਸ "reenqueue limit";
	ਹਾਲਤ SCX_EXIT_ERROR_RESCUE:
		ਵਾਪਸ "rescue bandwidth overload";
	ਮੂਲ:
		ਵਾਪਸ "<UNKNOWN>";
	}
}

ਸਥਿਰ ਖਾਲੀ free_kick_syncs(ਖਾਲੀ)
{
	ਪੂਰਨ cpu;

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_kick_syncs __rcu **ksyncs = ਪ੍ਰਤੀ_cpu_ptr(&scx_kick_syncs, cpu);
		ਬਣਤਰ scx_kick_syncs *ਮੰਜ਼ਿਲ_free;

		/* flush the pending kick before freeing @ksyncs */
		irq_work_sync(&cpu_rq(cpu)->scx.kick_cpus_irq_work);
		ਮੰਜ਼ਿਲ_free = rcu_replace_ਸੂਚਕ(*ksyncs, ਨਿਰਾ, true);
		ਜੇ (ਮੰਜ਼ਿਲ_free)
			kvfree_rcu(ਮੰਜ਼ਿਲ_free, rcu);
	}
}

ਸਥਿਰ ਖਾਲੀ refresh_watchdog(ਖਾਲੀ)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ intv = ULONG_MAX;

	/* take the shortest timeout and use its half for watchdog interval */
	rcu_read_ਤਾਲਾ();
	list_for_each_ਇੰਦਰਾਜ_rcu(sch, &scx_sched_all, all)
		intv = max(min(intv, sch->watchdog_timeout / 2), 1);
	rcu_read_unlock();

	WRITE_ONCE(scx_watchdog_timestamp, jiffies);
	WRITE_ONCE(scx_watchdog_interval, intv);

	ਜੇ (intv < ULONG_MAX)
		mod_delayed_work(system_dfl_wq, &scx_watchdog_work, intv);
	ਨਹੀਂਤਾਂ
		cancel_delayed_work_sync(&scx_watchdog_work);
}

s32 scx_link_sched(ਬਣਤਰ scx_sched *sch)
{
	scoped_guard(raw_spinlock_irqsave, &scx_bypass_ਤਾਲਾ)	/* for the parent bypass check */
	scoped_guard(raw_spinlock, &scx_sched_ਤਾਲਾ) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
		ਬਣਤਰ scx_sched *ਮਾਪਾ = scx_ਮਾਪਾ(sch);

		ਜੇ (ਮਾਪਾ) {
			s32 ret;

			/*
			 * Bypass state is spread across per-cpu flags and a
			 * depth count, so inheriting it is tricky and has no
			 * valid use case. Refuse it.
			 */
			ਜੇ (READ_ONCE(ਮਾਪਾ->bypass_ਡੂੰਘਾਈ)) {
				scx_error(sch, "parent bypassing (%d)", -EBUSY);
				ਵਾਪਸ -EBUSY;
			}

			ret = rhashtable_lookup_ਪਾਓ_fast(&scx_sched_hash,
					&sch->hash_ਗੰਢ, scx_sched_hash_params);
			ਜੇ (ret) {
				scx_error(sch, "failed to insert into scx_sched_hash (%d)",
					  ret);
				ਵਾਪਸ ret;
			}

			list_ਜੋੜੋ_ਪੂਛ_rcu(&sch->sibling, &ਮਾਪਾ->children);

			/*
			 * Pairs with the mb after the ->aborting assertion in
			 * scx_claim_exit(). Either we see ->aborting and back
			 * out, or the exit path sees us and exits us.
			 */
			smp_mb();
			ਜੇ (unlikely(READ_ONCE(ਮਾਪਾ->aborting))) {
				rhashtable_remove_fast(&scx_sched_hash, &sch->hash_ਗੰਢ,
						       scx_sched_hash_params);
				list_del_rcu(&sch->sibling);
				scx_error(sch, "parent disabled (%d)", -ENOENT);
				ਵਾਪਸ -ENOENT;
			}

			sch->linked = true;
		}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

		list_ਜੋੜੋ_ਪੂਛ_rcu(&sch->all, &scx_sched_all);
	}

	refresh_watchdog();
	ਵਾਪਸ 0;
}

ਖਾਲੀ scx_unlink_sched(ਬਣਤਰ scx_sched *sch)
{
	scoped_guard(raw_spinlock_irq, &scx_sched_ਤਾਲਾ) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
		ਜੇ (sch->linked) {
			rhashtable_remove_fast(&scx_sched_hash, &sch->hash_ਗੰਢ,
					       scx_sched_hash_params);
			list_del_rcu(&sch->sibling);
			sch->linked = false;
		}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */
		list_del_rcu(&sch->all);
	}

	refresh_watchdog();
}

/*
 * Called to disable future dumps and wait for in-progress one while disabling
 * @sch. Once @sch becomes empty during disable, there's no point in dumping it.
 * This prevents calling dump ops on a dead sch.
 */
ਖਾਲੀ scx_disable_dump(ਬਣਤਰ scx_sched *sch)
{
	guard(raw_spinlock_irqsave)(&scx_dump_ਤਾਲਾ);
	sch->dump_disabled = true;
}

ਖਾਲੀ scx_log_sched_disable(ਬਣਤਰ scx_sched *sch)
{
	ਬਣਤਰ scx_exit_info *ei = sch->exit_info;
	ਅਚਲ ਅੱਖਰ *type = scx_ਮਾਪਾ(sch) ? "sub-scheduler" : "scheduler";

	ਜੇ (ei->ਕਿਸਮ >= SCX_EXIT_ERROR) {
		pr_err("sched_ext: BPF %s \"%s\" disabled (%s)\n", type,
		       sch->ops.name, ei->reason);

		ਜੇ (ei->ਸੁਨੇਹਾ[0] != '\0')
			pr_err("sched_ext: %s: %s\n", sch->ops.name, ei->ਸੁਨੇਹਾ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_STACKTRACE
		ਢੇਰ_trace_print(ei->bt, ei->bt_len, 2);
#ਜੇ_ਅੰਤ
	} ਨਹੀਂਤਾਂ {
		pr_info("sched_ext: BPF %s \"%s\" disabled (%s)\n", type,
			sch->ops.name, ei->reason);
	}
}

ਸਥਿਰ ਖਾਲੀ scx_ਜੜ੍ਹ_disable(ਬਣਤਰ scx_sched *sch)
{
	ਬਣਤਰ scx_task_iter sti;
	ਬਣਤਰ task_struct *p;
	bool was_switched_all;
	ਪੂਰਨ cpu;

	/* guarantee forward progress and wait for descendants to be disabled */
	scx_bypass(sch, true);
	drain_descendants(sch);

	ਚੋਣ (scx_ਸੈੱਟ_enable_state(SCX_DISABLING)) {
	ਹਾਲਤ SCX_DISABLING:
		WARN_ONCE(true, "sched_ext: duplicate disabling instance?");
		ਰੋਕੋ;
	ਹਾਲਤ SCX_DISABLED:
		pr_warn("sched_ext: ops error detected without ops (%s)\n",
			sch->exit_info->ਸੁਨੇਹਾ);
		WARN_ON_ONCE(scx_ਸੈੱਟ_enable_state(SCX_DISABLED) != SCX_DISABLING);
		ਜਾਓ ਪੂਰਾ;
	ਮੂਲ:
		ਰੋਕੋ;
	}

	/*
	 * Here, every runnable task is guaranteed to make forward progress and
	 * we can safely use blocking synchronization constructs. Actually
	 * disable ops.
	 */
	mutex_ਤਾਲਾ(&scx_enable_mutex);

	was_switched_all = scx_switched_all();

	static_branch_disable(&__scx_switched_all);
	WRITE_ONCE(scx_switching_all, false);

	/*
	 * Shut down cgroup support before tasks so that the cgroup attach and
	 * migration paths don't race against scx_disable_and_exit_task().
	 */
	scx_cgroup_ਤਾਲਾ();
	scx_cgroup_enabled = false;
	scx_cgroup_exit(sch);
	scx_cgroup_unlock();

	/*
	 * The BPF scheduler is going away. All tasks including %TASK_DEAD ones
	 * must be switched out and exited synchronously.
	 */
	percpu_ਥੱਲੇ_write(&scx_fork_rwsem);

	scx_init_task_enabled = false;

	scx_task_iter_ਸ਼ੁਰੂ(&sti, ਨਿਰਾ);
	ਜਦੋਂ ((p = scx_task_iter_ਅਗਲਾ_locked(&sti))) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕਤਾਰ_flags = ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK;
		ਅਚਲ ਬਣਤਰ sched_class *old_class = p->sched_class;
		ਅਚਲ ਬਣਤਰ sched_class *new_class = scx_setscheduler_class(p);

		update_rq_clock(task_rq(p));

		ਜੇ (old_class != new_class)
			ਕਤਾਰ_flags |= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS;

		scoped_guard (sched_change, p, ਕਤਾਰ_flags) {
			p->sched_class = new_class;
		}

		scx_disable_and_exit_task(scx_task_sched(p), p);
	}
	scx_task_iter_stop(&sti);

	scx_disable_dump(sch);

	scx_cgroup_ਤਾਲਾ();
	ਸੈੱਟ_cgroup_sched(sch_cgroup(sch), ਨਿਰਾ);
	scx_cgroup_unlock();

	percpu_ਉੱਪਰ_write(&scx_fork_rwsem);

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
		ਬਣਤਰ rq *rq = cpu_rq(cpu);

		scoped_guard(rq_ਤਾਲਾ_irqsave, rq) {
			update_rq_clock(rq);
			ਜੇ (was_switched_all) {
				ਜੇ (WARN_ON_ONCE(dl_server_ਅਦਲਾ_ਬਦਲੀ_bw(&rq->ext_server,
								   &rq->fair_server)))
					pr_warn("failed to re-attach fair_server on CPU %d\n", cpu);
			} ਨਹੀਂਤਾਂ {
				dl_server_detach_bw(&rq->ext_server);
			}
		}
	}

	/* no task is on scx, turn off all the switches and flush in-progress calls */
	static_branch_disable(&__scx_enabled);
	static_branch_disable(&__scx_is_cid_type);
	ਜੇ (sch->ops.flags & SCX_OPS_TID_ਮੰਜ਼ਿਲ_TASK)
		static_branch_disable(&__scx_tid_to_task_enabled);
	bitmap_zero(sch->has_op, SCX_OPI_ਸਿਰਾ);
	scx_idle_disable();
	synchronize_rcu();
	ਜੇ (sch->ops.flags & SCX_OPS_TID_ਮੰਜ਼ਿਲ_TASK)
		rhashtable_free_and_destroy(&scx_tid_hash, ਨਿਰਾ, ਨਿਰਾ);

	scx_log_sched_disable(sch);

	ਜੇ (sch->ops.ਬਾਹਰ_ਨਿਕਲੋ)
		SCX_CALL_OP(sch, ਬਾਹਰ_ਨਿਕਲੋ, ਨਿਰਾ, sch->exit_info);

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
	cpus_read_ਤਾਲਾ();
	RCU_INIT_ਸੂਚਕ(scx_ਜੜ੍ਹ, ਨਿਰਾ);
	scx_cid_retire_tables();
	cpus_read_unlock();

	/*
	 * Delete the kobject from the hierarchy synchronously. Otherwise, sysfs
	 * could observe an object of the same name still in the hierarchy when
	 * the next scheduler is loaded.
	 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਜੇ (sch->sub_kset)
		kobject_del(&sch->sub_kset->kobj);
#ਜੇ_ਅੰਤ
	/* not added if enable failed before scx_sched_sysfs_add() */
	ਜੇ (sch->kobj.state_ਵਿੱਚ_sysfs)
		kobject_del(&sch->kobj);

	free_kick_syncs();

	mutex_unlock(&scx_enable_mutex);

	WARN_ON_ONCE(scx_ਸੈੱਟ_enable_state(SCX_DISABLED) != SCX_DISABLING);
ਪੂਰਾ:
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
ਸਥਿਰ ਖਾਲੀ scx_propagate_exit_irq_workfn(ਬਣਤਰ irq_work *irq_work)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(irq_work, ਬਣਤਰ scx_sched,
					     propagate_exit_irq_work);
	ਬਣਤਰ scx_sched *pos;

	scoped_guard (raw_spinlock_irqsave, &scx_sched_ਤਾਲਾ) {
		scx_for_each_descendant_pre(pos, sch)
			scx_disable(pos, SCX_EXIT_ਮਾਪਾ);
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
ਸਥਿਰ bool scx_claim_exit(ਬਣਤਰ scx_sched *sch, ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ)
{
	ਪੂਰਨ none = SCX_EXIT_NONE;

	lockdep_assert_preemption_disabled();

	ਜੇ (WARN_ON_ONCE(ਕਿਸਮ == SCX_EXIT_NONE || ਕਿਸਮ == SCX_EXIT_ਪੂਰਾ))
		ਕਿਸਮ = SCX_EXIT_ERROR;

	ਜੇ (!atomic_try_cmpxchg(&sch->exit_ਕਿਸਮ, &none, ਕਿਸਮ))
		ਵਾਪਸ false;

	ਜੇ (ਕਿਸਮ == SCX_EXIT_ਮਾਪਾ) {
		/* an ancestor is already sweeping the subtree */
		WRITE_ONCE(sch->aborting, true);
	} ਨਹੀਂਤਾਂ {
		ਬਣਤਰ scx_sched *pos;

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

		irq_work_ਕਤਾਰ(&sch->propagate_exit_irq_work);
	}

	/* fired after ->aborting is set so callbacks can't delay recovery */
	trace_sched_ext_exit(sch, ਕਿਸਮ);

	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ scx_disable_workfn(ਬਣਤਰ kthread_work *work)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(work, ਬਣਤਰ scx_sched, disable_work);
	ਬਣਤਰ scx_exit_info *ei = sch->exit_info;
	ਪੂਰਨ ਕਿਸਮ;

	ਕਿਸਮ = atomic_read(&sch->exit_ਕਿਸਮ);
	ਜਦੋਂ (true) {
		ਜੇ (ਕਿਸਮ == SCX_EXIT_ਪੂਰਾ)	/* already disabled? */
			ਵਾਪਸ;
		WARN_ON_ONCE(ਕਿਸਮ == SCX_EXIT_NONE);
		ਜੇ (atomic_try_cmpxchg(&sch->exit_ਕਿਸਮ, &ਕਿਸਮ, SCX_EXIT_ਪੂਰਾ))
			ਰੋਕੋ;
	}
	ei->ਕਿਸਮ = ਕਿਸਮ;
	ei->reason = scx_exit_reason(ei->ਕਿਸਮ);

	ਜੇ (scx_ਮਾਪਾ(sch))
		scx_sub_disable(sch);
	ਨਹੀਂਤਾਂ
		scx_ਜੜ੍ਹ_disable(sch);
}

ਸਥਿਰ ਖਾਲੀ scx_disable(ਬਣਤਰ scx_sched *sch, ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ)
{
	guard(preempt)();
	ਜੇ (scx_claim_exit(sch, ਕਿਸਮ))
		irq_work_ਕਤਾਰ(&sch->disable_irq_work);
}

/**
 * scx_flush_disable_work - flush the disable work and wait for it to finish
 * @sch: the scheduler
 *
 * sch->disable_work might still not queued, causing kthread_flush_work()
 * as a noop. Syncing the irq_work first is required to guarantee the
 * kthread work has been queued before waiting for it.
 */
ਖਾਲੀ scx_flush_disable_work(ਬਣਤਰ scx_sched *sch)
{
	ਪੂਰਨ ਕਿਸਮ;

	ਕਰੋ {
		irq_work_sync(&sch->disable_irq_work);
		kthread_flush_work(&sch->disable_work);
		ਕਿਸਮ = atomic_read(&sch->exit_ਕਿਸਮ);
	} ਜਦੋਂ (ਕਿਸਮ != SCX_EXIT_NONE && ਕਿਸਮ != SCX_EXIT_ਪੂਰਾ);
}

ਸਥਿਰ ਖਾਲੀ dump_newline(ਬਣਤਰ seq_buf *s)
{
	trace_sched_ext_dump("");

	/* @s may be zero sized and seq_buf triggers WARN if so */
	ਜੇ (s->ਮਾਪ)
		seq_buf_putc(s, '\n');
}

__printf(2, 3) ਖਾਲੀ scx_dump_ਸਤਰ(ਬਣਤਰ seq_buf *s, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
	va_list ਦਲੀਲਾਂ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_TRACEPOINTS
	ਜੇ (trace_sched_ext_dump_enabled()) {
		/* protected by scx_dump_lock */
		ਸਥਿਰ ਅੱਖਰ ਸਤਰ_buf[SCX_EXIT_ਸੁਨੇਹਾ_LEN];

		va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
		vscnprintf(ਸਤਰ_buf, ਆਕਾਰ(ਸਤਰ_buf), fmt, ਦਲੀਲਾਂ);
		va_ਸਿਰਾ(ਦਲੀਲਾਂ);

		trace_call__sched_ext_dump(ਸਤਰ_buf);
	}
#ਜੇ_ਅੰਤ
	/* @s may be zero sized and seq_buf triggers WARN if so */
	ਜੇ (s->ਮਾਪ) {
		va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
		seq_buf_vprintf(s, fmt, ਦਲੀਲਾਂ);
		va_ਸਿਰਾ(ਦਲੀਲਾਂ);

		seq_buf_putc(s, '\n');
	}
}

ਸਥਿਰ ਖਾਲੀ dump_ਢੇਰ_trace(ਬਣਤਰ seq_buf *s, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ,
			     ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *bt, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	ਲਈ (i = 0; i < len; i++)
		scx_dump_ਸਤਰ(s, "%s%pS", ਅਗੇਤਰ, (ਖਾਲੀ *)bt[i]);
}

ਸਥਿਰ ਖਾਲੀ ops_dump_init(ਬਣਤਰ seq_buf *s, ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ)
{
	ਬਣਤਰ scx_dump_ਅੰਕੜੇ *dd = &scx_dump_ਅੰਕੜੇ;

	lockdep_assert_irqs_disabled();

	dd->cpu = smp_processor_id();		/* allow scx_bpf_dump() */
	dd->ਪਹਿਲਾ = true;
	dd->cursor = 0;
	dd->s = s;
	dd->ਅਗੇਤਰ = ਅਗੇਤਰ;
}

ਸਥਿਰ ਖਾਲੀ ops_dump_flush(ਖਾਲੀ)
{
	ਬਣਤਰ scx_dump_ਅੰਕੜੇ *dd = &scx_dump_ਅੰਕੜੇ;
	ਅੱਖਰ *ਸਤਰ = dd->buf.ਸਤਰ;

	ਜੇ (!dd->cursor)
		ਵਾਪਸ;

	/*
	 * There's something to flush and this is the first line. Insert a blank
	 * line to distinguish ops dump.
	 */
	ਜੇ (dd->ਪਹਿਲਾ) {
		dump_newline(dd->s);
		dd->ਪਹਿਲਾ = false;
	}

	/*
	 * There may be multiple lines in $line. Scan and emit each line
	 * separately.
	 */
	ਜਦੋਂ (true) {
		ਅੱਖਰ *ਸਿਰਾ = ਸਤਰ;
		ਅੱਖਰ c;

		ਜਦੋਂ (*ਸਿਰਾ != '\n' && *ਸਿਰਾ != '\0')
			ਸਿਰਾ++;

		/*
		 * If $line overflowed, it may not have newline at the end.
		 * Always emit with a newline.
		 */
		c = *ਸਿਰਾ;
		*ਸਿਰਾ = '\0';
		scx_dump_ਸਤਰ(dd->s, "%s%s", dd->ਅਗੇਤਰ, ਸਤਰ);
		ਜੇ (c == '\0')
			ਰੋਕੋ;

		/* move to the next line */
		ਸਿਰਾ++;
		ਜੇ (*ਸਿਰਾ == '\0')
			ਰੋਕੋ;
		ਸਤਰ = ਸਿਰਾ;
	}

	dd->cursor = 0;
}

ਸਥਿਰ ਖਾਲੀ ops_dump_exit(ਖਾਲੀ)
{
	ops_dump_flush();
	scx_dump_ਅੰਕੜੇ.cpu = -1;
}

ਸਥਿਰ ਖਾਲੀ scx_dump_task(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ seq_buf *s, ਬਣਤਰ scx_dump_ctx *dctx,
			  ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਅੱਖਰ marker)
{
	ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bt[SCX_EXIT_BT_LEN];
	ਬਣਤਰ scx_sched *task_sch = scx_task_sched(p);
	ਅਚਲ ਅੱਖਰ *own_marker;
	ਅੱਖਰ sch_id_buf[32];
	ਅੱਖਰ dsq_id_buf[19] = "(n/a)";
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ops_state = atomic_long_read(&p->scx.ops_state);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ bt_len = 0;

	own_marker = task_sch == sch ? "*" : "";

	ਜੇ (task_sch->level == 0)
		scnprintf(sch_id_buf, ਆਕਾਰ(sch_id_buf), "root");
	ਨਹੀਂਤਾਂ
		scnprintf(sch_id_buf, ਆਕਾਰ(sch_id_buf), "sub%d-%llu",
			  task_sch->level, task_sch->ops.sub_cgroup_id);

	ਜੇ (p->scx.dsq)
		scnprintf(dsq_id_buf, ਆਕਾਰ(dsq_id_buf), "0x%llx",
			  (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)p->scx.dsq->id);

	dump_newline(s);
	scx_dump_ਸਤਰ(s, " %c%c %s[%d] %s%s %+ldms",
		      marker, task_state_ਮੰਜ਼ਿਲ_char(p), p->comm, p->pid, own_marker, sch_id_buf,
		      jiffies_delta_msecs(p->scx.runnable_at, dctx->at_jiffies));
	scx_dump_ਸਤਰ(s, "      scx_state/flags=%u/0x%x dsq_flags=0x%x ops_state/qseq=%lu/%lu",
		      scx_get_task_state(p) >> SCX_TASK_STATE_ਖਿਸਕਾਓ,
		      p->scx.flags & ~SCX_TASK_STATE_ਨਕਾਬ, p->scx.dsq_flags,
		      ops_state & SCX_OPSS_STATE_ਨਕਾਬ, ops_state >> SCX_OPSS_QSEQ_ਖਿਸਕਾਓ);
	scx_dump_ਸਤਰ(s, "      sticky/holding_cpu=%d/%d dsq_id=%s",
		      p->scx.sticky_cpu, p->scx.holding_cpu, dsq_id_buf);
	scx_dump_ਸਤਰ(s, "      dsq_vtime=%llu slice=%llu weight=%u",
		      p->scx.dsq_vtime, p->scx.ਟੁਕੜਾ, p->scx.ਭਾਰ);
	scx_dump_ਸਤਰ(s, "      cpus=%*pb no_mig=%u", cpumask_pr_ਦਲੀਲਾਂ(p->cpus_ptr),
		      p->migration_disabled);

	ਜੇ (SCX_HAS_OP(sch, dump_task)) {
		ops_dump_init(s, "    ");
		SCX_CALL_OP(sch, dump_task, rq, dctx, p);
		ops_dump_exit();
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_STACKTRACE
	bt_len = ਢੇਰ_trace_save_tsk(p, bt, SCX_EXIT_BT_LEN, 1);
#ਜੇ_ਅੰਤ
	ਜੇ (bt_len) {
		dump_newline(s);
		dump_ਢੇਰ_trace(s, "    ", bt, bt_len);
	}
}

ਸਥਿਰ ਖਾਲੀ scx_dump_cpu(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ seq_buf *s,
			 ਬਣਤਰ scx_dump_ctx *dctx, ਪੂਰਨ cpu,
			 bool dump_all_tasks)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਣਤਰ scx_sched_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(sch->pcpu, cpu);
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ task_struct *p;
	ਬਣਤਰ seq_buf ns;
	ਮਾਪ_t avail, ਵਰਤਿਆ;
	ਅੱਖਰ *buf;
	bool idle;

	rq_ਤਾਲਾ_irqsave(rq, &rf);

	idle = list_ਖਾਲੀ_ਥਾਵਾਂ(&rq->scx.runnable_list) &&
		rq->curr->sched_class == &idle_sched_class;

	ਜੇ (idle && !SCX_HAS_OP(sch, dump_cpu))
		ਜਾਓ ਅਗਲਾ;

	/*
	 * We don't yet know whether ops.dump_cpu() will produce output
	 * and we may want to skip the default CPU dump if it doesn't.
	 * Use a nested seq_buf to generate the standard dump so that we
	 * can decide whether to commit later.
	 */
	avail = seq_buf_get_buf(s, &buf);
	seq_buf_init(&ns, buf, avail);

	dump_newline(&ns);
	scx_dump_ਸਤਰ(&ns, "CPU %-4d: nr_run=%u flags=0x%x cpu_rel=%d ksync=%lu",
		      cpu, rq->scx.nr_running, rq->scx.flags, rq->scx.cpu_released,
		      rq->scx.kick_sync);
	scx_rescue_dump(&ns, rq);
	scx_dump_ਸਤਰ(&ns, "          curr=%s[%d] class=%ps",
		      rq->curr->comm, rq->curr->pid, rq->curr->sched_class);
	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_kick))
		scx_dump_ਸਤਰ(&ns, "  cpus_to_kick   : %*pb",
			      cpumask_pr_ਦਲੀਲਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_kick));
	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle))
		scx_dump_ਸਤਰ(&ns, "  idle_to_kick   : %*pb",
			      cpumask_pr_ਦਲੀਲਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle));
	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_preempt))
		scx_dump_ਸਤਰ(&ns, "  cpus_to_preempt: %*pb",
			      cpumask_pr_ਦਲੀਲਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_preempt));
	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_wait))
		scx_dump_ਸਤਰ(&ns, "  cpus_to_wait   : %*pb",
			      cpumask_pr_ਦਲੀਲਾਂ(pcpu->cpus_ਮੰਜ਼ਿਲ_wait));
	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(rq->scx.cpus_ਮੰਜ਼ਿਲ_sync))
		scx_dump_ਸਤਰ(&ns, "  cpus_to_sync   : %*pb",
			      cpumask_pr_ਦਲੀਲਾਂ(rq->scx.cpus_ਮੰਜ਼ਿਲ_sync));

	ਵਰਤਿਆ = seq_buf_ਵਰਤਿਆ(&ns);
	ਜੇ (SCX_HAS_OP(sch, dump_cpu)) {
		ops_dump_init(&ns, "  ");
		SCX_CALL_OP(sch, dump_cpu, rq, dctx, scx_cpu_arg(cpu), idle);
		ops_dump_exit();
	}

	/*
	 * If idle && nothing generated by ops.dump_cpu(), there's
	 * nothing interesting. Skip.
	 */
	ਜੇ (idle && ਵਰਤਿਆ == seq_buf_ਵਰਤਿਆ(&ns))
		ਜਾਓ ਅਗਲਾ;

	/*
	 * $s may already have overflowed when $ns was created. If so,
	 * calling commit on it will trigger BUG.
	 */
	ਜੇ (avail) {
		seq_buf_commit(s, seq_buf_ਵਰਤਿਆ(&ns));
		ਜੇ (seq_buf_has_overflowed(&ns))
			seq_buf_ਸੈੱਟ_ਉਛਾਲ(s);
	}

	ਜੇ (rq->curr->sched_class == &ext_sched_class &&
	    (dump_all_tasks || scx_task_on_sched(sch, rq->curr)))
		scx_dump_task(sch, s, dctx, rq, rq->curr, '*');

	list_for_each_ਇੰਦਰਾਜ(p, &rq->scx.runnable_list, scx.runnable_ਗੰਢ)
		ਜੇ (dump_all_tasks || scx_task_on_sched(sch, p))
			scx_dump_task(sch, s, dctx, rq, p, ' ');
ਅਗਲਾ:
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Dump scheduler state. If @dump_all_tasks is true, dump all tasks regardless
 * of which scheduler they belong to. If false, only dump tasks owned by @sch.
 * For SysRq-D dumps, @dump_all_tasks=false since all schedulers are dumped
 * separately. For error dumps, @dump_all_tasks=true since only the failing
 * scheduler is dumped.
 */
ਸਥਿਰ ਖਾਲੀ scx_dump_state(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ scx_exit_info *ei,
			   ਮਾਪ_t dump_len, bool dump_all_tasks)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ trunc_marker[] = "\n\n~~~~ TRUNCATED ~~~~\n";
	ਬਣਤਰ scx_dump_ctx dctx = {
		.ਕਿਸਮ = ei->ਕਿਸਮ,
		.exit_ਕੋਡ = ei->exit_ਕੋਡ,
		.reason = ei->reason,
		.at_ns = ktime_get_ns(),
		.at_jiffies = jiffies,
	};
	ਬਣਤਰ seq_buf s;
	ਬਣਤਰ scx_event_stats events;
	ਪੂਰਨ cpu;

	guard(raw_spinlock_irqsave)(&scx_dump_ਤਾਲਾ);

	ਜੇ (sch->dump_disabled)
		ਵਾਪਸ;

	seq_buf_init(&s, ei->dump, dump_len);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਜੇ (sch->level == 0)
		scx_dump_ਸਤਰ(&s, "%s: root", sch->ops.name);
	ਨਹੀਂਤਾਂ
		scx_dump_ਸਤਰ(&s, "%s: sub%d-%llu %s",
			      sch->ops.name, sch->level, sch->ops.sub_cgroup_id,
			      sch->cgrp_ਰਸਤਾ);
#ਜੇ_ਅੰਤ
	ਜੇ (ei->ਕਿਸਮ == SCX_EXIT_NONE) {
		scx_dump_ਸਤਰ(&s, "Debug dump triggered by %s", ei->reason);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ei->exit_cpu >= 0)
			scx_dump_ਸਤਰ(&s, "%s[%d] triggered exit kind %d on CPU %d:",
				      current->comm, current->pid, ei->ਕਿਸਮ,
				      ei->exit_cpu);
		ਨਹੀਂਤਾਂ
			scx_dump_ਸਤਰ(&s, "%s[%d] triggered exit kind %d:",
				      current->comm, current->pid, ei->ਕਿਸਮ);
		scx_dump_ਸਤਰ(&s, "  %s (%s)", ei->reason, ei->ਸੁਨੇਹਾ);
		dump_newline(&s);
		scx_dump_ਸਤਰ(&s, "Backtrace:");
		dump_ਢੇਰ_trace(&s, "  ", ei->bt, ei->bt_len);
	}

	ਜੇ (SCX_HAS_OP(sch, dump)) {
		ops_dump_init(&s, "");
		SCX_CALL_OP(sch, dump, ਨਿਰਾ, &dctx);
		ops_dump_exit();
	}

	dump_newline(&s);
	scx_dump_ਸਤਰ(&s, "CPU states");
	scx_dump_ਸਤਰ(&s, "----------");

	/*
	 * Dump stalled CPUs first so they aren't lost to dump truncation, then
	 * walk the rest in order. Fall back to exit_cpu if no stall mask set.
	 */
	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(sch->stall_cpus)) {
		for_each_cpu(cpu, sch->stall_cpus)
			scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		for_each_possible_cpu(cpu) {
			ਜੇ (!cpumask_test_cpu(cpu, sch->stall_cpus))
				scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		}
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ei->exit_cpu >= 0)
			scx_dump_cpu(sch, &s, &dctx, ei->exit_cpu, dump_all_tasks);
		for_each_possible_cpu(cpu) {
			ਜੇ (cpu != ei->exit_cpu)
				scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		}
	}

	dump_newline(&s);
	scx_dump_ਸਤਰ(&s, "Event counters");
	scx_dump_ਸਤਰ(&s, "--------------");

	scx_read_events(sch, &events);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCX_EVENT(name)	scx_dump_event(s, &events, name)
	SCX_EVENTS_LIST(SCX_EVENT);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCX_EVENT

	ਜੇ (seq_buf_has_overflowed(&s) && dump_len >= ਆਕਾਰ(trunc_marker))
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ei->dump + dump_len - ਆਕਾਰ(trunc_marker),
		       trunc_marker, ਆਕਾਰ(trunc_marker));
}

ਸਥਿਰ ਖਾਲੀ scx_disable_irq_workfn(ਬਣਤਰ irq_work *irq_work)
{
	ਬਣਤਰ scx_sched *sch = container_ਦਾ(irq_work, ਬਣਤਰ scx_sched, disable_irq_work);
	ਬਣਤਰ scx_exit_info *ei = sch->exit_info;

	ਜੇ (ei->ਕਿਸਮ >= SCX_EXIT_ERROR)
		scx_dump_state(sch, ei, sch->ops.exit_dump_len, true);

	kthread_ਕਤਾਰ_work(sch->helper, &sch->disable_work);
}

/* finish exit_info and kick the disable work, ei->msg must already be set */
ਸਥਿਰ ਖਾਲੀ scx_ਅੰਤ_exit(ਬਣਤਰ scx_sched *sch, ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ,
			    s64 exit_ਕੋਡ, s32 exit_cpu)
{
	ਬਣਤਰ scx_exit_info *ei = sch->exit_info;

	ei->exit_ਕੋਡ = exit_ਕੋਡ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_STACKTRACE
	/*
	 * stack_trace_save()'s NMI-safety is arch-dependent and undocumented.
	 * Skip the backtrace when exiting from NMI.
	 */
	ਜੇ (ਕਿਸਮ >= SCX_EXIT_ERROR && !ਵਿੱਚ_nmi())
		ei->bt_len = ਢੇਰ_trace_save(ei->bt, SCX_EXIT_BT_LEN, 1);
#ਜੇ_ਅੰਤ
	/*
	 * Set ei->kind and ->reason for scx_dump_state(). They'll be set again
	 * in scx_disable_workfn().
	 */
	ei->ਕਿਸਮ = ਕਿਸਮ;
	ei->reason = scx_exit_reason(ei->ਕਿਸਮ);
	ei->exit_cpu = exit_cpu;

	irq_work_ਕਤਾਰ(&sch->disable_irq_work);
}

bool scx_vexit(ਬਣਤਰ scx_sched *sch,
	       ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ, s64 exit_ਕੋਡ, s32 exit_cpu,
	       ਅਚਲ ਅੱਖਰ *fmt, va_list ਦਲੀਲਾਂ)
{
	ਬਣਤਰ scx_exit_info *ei = sch->exit_info;

	guard(preempt)();

	ਜੇ (!scx_claim_exit(sch, ਕਿਸਮ))
		ਵਾਪਸ false;

	vscnprintf(ei->ਸੁਨੇਹਾ, SCX_EXIT_ਸੁਨੇਹਾ_LEN, fmt, ਦਲੀਲਾਂ);

	scx_ਅੰਤ_exit(sch, ਕਿਸਮ, exit_ਕੋਡ, exit_cpu);
	ਵਾਪਸ true;
}

ਸਥਿਰ ਪੂਰਨ alloc_kick_syncs(ਖਾਲੀ)
{
	ਪੂਰਨ cpu;

	/*
	 * Allocate per-CPU arrays sized by nr_cpu_ids. Use kvzalloc as size
	 * can exceed percpu allocator limits on large machines.
	 */
	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_kick_syncs __rcu **ksyncs = ਪ੍ਰਤੀ_cpu_ptr(&scx_kick_syncs, cpu);
		ਬਣਤਰ scx_kick_syncs *new_ksyncs;

		WARN_ON_ONCE(rcu_access_ਸੂਚਕ(*ksyncs));

		new_ksyncs = kvzalloc_ਗੰਢ(struct_ਮਾਪ(new_ksyncs, syncs, nr_cpu_ids),
					   GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu));
		ਜੇ (!new_ksyncs) {
			free_kick_syncs();
			ਵਾਪਸ -ENOMEM;
		}

		rcu_ਸੌਂਪੋ_ਸੂਚਕ(*ksyncs, new_ksyncs);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ free_pnode(ਬਣਤਰ scx_sched_pnode *pnode)
{
	ਜੇ (!pnode)
		ਵਾਪਸ;
	exit_dsq(&pnode->global_dsq);
	kfree(pnode);
}

ਸਥਿਰ ਬਣਤਰ scx_sched_pnode *alloc_pnode(ਬਣਤਰ scx_sched *sch, ਪੂਰਨ ਗੰਢ)
{
	ਬਣਤਰ scx_sched_pnode *pnode;

	pnode = kzalloc_ਗੰਢ(ਆਕਾਰ(*pnode), GFP_KERNEL, ਗੰਢ);
	ਜੇ (!pnode)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (scx_init_dsq(&pnode->global_dsq, SCX_DSQ_GLOBAL, sch)) {
		kfree(pnode);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ pnode;
}

/*
 * Allocate and initialize a new scx_sched. @cgrp's reference is always
 * consumed whether the function succeeds or fails.
 */
ਬਣਤਰ scx_sched *scx_alloc_and_ਜੋੜੋ_sched(ਬਣਤਰ scx_enable_cmd *cmd,
					  ਬਣਤਰ cgroup *cgrp,
					  ਬਣਤਰ scx_sched *ਮਾਪਾ)
{
	ਬਣਤਰ sched_ext_ops *ops = cmd->ops;
	ਬਣਤਰ scx_sched *sch;
	s32 level = ਮਾਪਾ ? ਮਾਪਾ->level + 1 : 0;
	s32 ਗੰਢ, cpu, ret, bypass_fail_cpu = nr_cpu_ids;

	sch = kzalloc_flex(*sch, ancestors, level + 1);
	ਜੇ (!sch) {
		ret = -ENOMEM;
		ਜਾਓ err_put_cgrp;
	}

	sch->exit_info = alloc_exit_info(ops->exit_dump_len);
	ਜੇ (!sch->exit_info) {
		ret = -ENOMEM;
		ਜਾਓ err_free_sch;
	}

	ret = rhashtable_init(&sch->dsq_hash, &dsq_hash_params);
	ਜੇ (ret < 0)
		ਜਾਓ err_free_ei;

	sch->pnode = kzalloc_objs(sch->pnode[0], nr_ਗੰਢ_ids);
	ਜੇ (!sch->pnode) {
		ret = -ENOMEM;
		ਜਾਓ err_free_hash;
	}

	for_each_ਗੰਢ_state(ਗੰਢ, N_POSSIBLE) {
		sch->pnode[ਗੰਢ] = alloc_pnode(sch, ਗੰਢ);
		ਜੇ (!sch->pnode[ਗੰਢ]) {
			ret = -ENOMEM;
			ਜਾਓ err_free_pnode;
		}
	}

	sch->dsp_max_batch = ops->dispatch_max_batch ?: SCX_DSP_DFL_MAX_BATCH;
	sch->pcpu = __alloc_percpu(struct_ਮਾਪ_t(ਬਣਤਰ scx_sched_pcpu,
						 dsp_ctx.buf, sch->dsp_max_batch),
				   __alignof__(ਬਣਤਰ scx_sched_pcpu));
	ਜੇ (!sch->pcpu) {
		ret = -ENOMEM;
		ਜਾਓ err_free_pnode;
	}

	for_each_possible_cpu(cpu) {
		ret = scx_init_dsq(scx_bypass_dsq(sch, cpu), SCX_DSQ_BYPASS, sch);
		ਜੇ (ret) {
			bypass_fail_cpu = cpu;
			ਜਾਓ err_free_pcpu;
		}
	}

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_sched_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(sch->pcpu, cpu);

		ਗੰਢ = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);
		pcpu->sch = sch;
		INIT_LIST_ਸਿਰ(&pcpu->deferred_reenq_local.ਗੰਢ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
		init_llist_ਗੰਢ(&pcpu->ecaps_ਮੰਜ਼ਿਲ_sync_ਗੰਢ);
#ਜੇ_ਅੰਤ
		INIT_LIST_ਸਿਰ(&pcpu->ਮੰਜ਼ਿਲ_kick_ਗੰਢ);
		ਜੇ (!zalloc_cpumask_var_ਗੰਢ(&pcpu->cpus_ਮੰਜ਼ਿਲ_kick, GFP_KERNEL, ਗੰਢ) ||
		    !zalloc_cpumask_var_ਗੰਢ(&pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle, GFP_KERNEL, ਗੰਢ) ||
		    !zalloc_cpumask_var_ਗੰਢ(&pcpu->cpus_ਮੰਜ਼ਿਲ_preempt, GFP_KERNEL, ਗੰਢ) ||
		    !zalloc_cpumask_var_ਗੰਢ(&pcpu->cpus_ਮੰਜ਼ਿਲ_wait, GFP_KERNEL, ਗੰਢ)) {
			ret = -ENOMEM;
			ਜਾਓ err_free_pcpu;
		}
	}

	sch->helper = kthread_ਲੜੀਵਾਰ_ਕਾਮਾ(0, "sched_ext_helper");
	ਜੇ (IS_ERR(sch->helper)) {
		ret = PTR_ERR(sch->helper);
		ਜਾਓ err_free_pcpu;
	}

	sched_ਸੈੱਟ_fifo(sch->helper->task);

	ਜੇ (ਮਾਪਾ)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(sch->ancestors, ਮਾਪਾ->ancestors,
		       level * ਆਕਾਰ(ਮਾਪਾ->ancestors[0]));
	sch->ancestors[level] = sch;
	sch->level = level;
	sch->id = atomic64_inc_return(&scx_sched_id_cursor);

	ਜੇ (ops->timeout_ms)
		sch->watchdog_timeout = msecs_ਮੰਜ਼ਿਲ_jiffies(ops->timeout_ms);
	ਨਹੀਂਤਾਂ
		sch->watchdog_timeout = SCX_WATCHDOG_MAX_TIMEOUT;

	sch->ਟੁਕੜਾ_dfl = SCX_ਟੁਕੜਾ_DFL;
	atomic_ਸੈੱਟ(&sch->exit_ਕਿਸਮ, SCX_EXIT_NONE);
	sch->disable_irq_work = IRQ_WORK_INIT_HARD(scx_disable_irq_workfn);
	sch->propagate_exit_irq_work = IRQ_WORK_INIT_HARD(scx_propagate_exit_irq_workfn);
	kthread_init_work(&sch->disable_work, scx_disable_workfn);
	timer_ਤਿਆਰੀ(&sch->bypass_lb_timer, scx_bypass_lb_timerfn, 0);

	ਜੇ (!alloc_cpumask_var(&sch->bypass_lb_donee_cpumask, GFP_KERNEL)) {
		ret = -ENOMEM;
		ਜਾਓ err_stop_helper;
	}
	ਜੇ (!alloc_cpumask_var(&sch->bypass_lb_resched_cpumask, GFP_KERNEL)) {
		ret = -ENOMEM;
		ਜਾਓ err_free_lb_cpumask;
	}
	ਜੇ (!zalloc_cpumask_var(&sch->stall_cpus, GFP_KERNEL)) {
		ret = -ENOMEM;
		ਜਾਓ err_free_lb_resched_cpumask;
	}
	/*
	 * Copy ops through the right union view. For cid-form the source is
	 * struct sched_ext_ops_cid which lacks the trailing cpu_acquire/
	 * cpu_release; those stay zero from kzalloc.
	 */
	ਜੇ (cmd->is_cid_type) {
		sch->ops_cid = *cmd->ops_cid;
		sch->is_cid_type = true;
	} ਨਹੀਂਤਾਂ {
		sch->ops = *cmd->ops;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਅੱਖਰ *buf = kzalloc(ਰਸਤਾ_MAX, GFP_KERNEL);
	ਜੇ (!buf) {
		ret = -ENOMEM;
		ਜਾਓ err_free_lb_resched;
	}
	cgroup_ਰਸਤਾ(cgrp, buf, ਰਸਤਾ_MAX);
	sch->cgrp_ਰਸਤਾ = kstrdup(buf, GFP_KERNEL);
	kfree(buf);
	ਜੇ (!sch->cgrp_ਰਸਤਾ) {
		ret = -ENOMEM;
		ਜਾਓ err_free_lb_resched;
	}

	sch->cgrp = cgrp;
	INIT_LIST_ਸਿਰ(&sch->children);
	INIT_LIST_ਸਿਰ(&sch->sibling);
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

	/*
	 * Publishing makes @sch visible to scx_prog_sched() readers. Failure
	 * paths after this point must free @sch through kobject_put() whose
	 * release path defers the actual freeing by an RCU grace period.
	 */
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ops->priv, sch);

	sch->kobj.kset = scx_kset;
	INIT_LIST_ਸਿਰ(&sch->all);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਜੇ (ਮਾਪਾ) {
		/*
		 * Pin @parent for @sch's lifetime. The kobject hierarchy pins
		 * it only via @parent->sub_kset, which is dropped during
		 * disable. Released in scx_sched_free_rcu_work().
		 */
		kobject_get(&ਮਾਪਾ->kobj);
	}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

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
#ਜੇ defined(CONFIG_MMU) && defined(CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ)
	ਜੇ (sch->arena_map)
		sch->arena_kern_ਅਧਾਰ = bpf_arena_map_kern_vm_ਸ਼ੁਰੂ(sch->arena_map);
#ਜੇ_ਅੰਤ
	cmd->arena_map = ਨਿਰਾ;
	ਵਾਪਸ sch;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
err_free_lb_resched:
	free_cpumask_var(sch->stall_cpus);
#ਜੇ_ਅੰਤ
err_free_lb_resched_cpumask:
	free_cpumask_var(sch->bypass_lb_resched_cpumask);
err_free_lb_cpumask:
	free_cpumask_var(sch->bypass_lb_donee_cpumask);
err_stop_helper:
	kthread_destroy_ਕਾਮਾ(sch->helper);
err_free_pcpu:
	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_sched_pcpu *pcpu = ਪ੍ਰਤੀ_cpu_ptr(sch->pcpu, cpu);

		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_kick);
		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle);
		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_preempt);
		free_cpumask_var(pcpu->cpus_ਮੰਜ਼ਿਲ_wait);
	}
	for_each_possible_cpu(cpu) {
		ਜੇ (cpu == bypass_fail_cpu)
			ਰੋਕੋ;
		exit_dsq(scx_bypass_dsq(sch, cpu));
	}
	free_percpu(sch->pcpu);
err_free_pnode:
	for_each_ਗੰਢ_state(ਗੰਢ, N_POSSIBLE)
		free_pnode(sch->pnode[ਗੰਢ]);
	kfree(sch->pnode);
err_free_hash:
	rhashtable_free_and_destroy(&sch->dsq_hash, ਨਿਰਾ, ਨਿਰਾ);
err_free_ei:
	free_exit_info(sch->exit_info);
err_free_sch:
	kfree(sch);
err_put_cgrp:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	cgroup_put(cgrp);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ ERR_PTR(ret);
}

/*
 * Add @sch's kobject to sysfs, and create its sub_kset if the scheduler
 * implements ops.sub_attach. Called by the enable workfns once @sch's
 * sysfs-visible state is initialized.
 */
ਪੂਰਨ scx_sched_sysfs_ਜੋੜੋ(ਬਣਤਰ scx_sched *sch)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਬਣਤਰ scx_sched *ਮਾਪਾ = scx_ਮਾਪਾ(sch);
	ਪੂਰਨ ret;

	ਜੇ (ਮਾਪਾ)
		ret = kobject_ਜੋੜੋ(&sch->kobj, &ਮਾਪਾ->sub_kset->kobj,
				  "sub-%llu", cgroup_id(sch_cgroup(sch)));
	ਨਹੀਂਤਾਂ
		ret = kobject_ਜੋੜੋ(&sch->kobj, ਨਿਰਾ, "root");
	ਜੇ (ret < 0)
		ਵਾਪਸ ret;

	ਜੇ (sch->ops.sub_attach) {
		sch->sub_kset = kset_create_and_ਜੋੜੋ("sub", ਨਿਰਾ, &sch->kobj);
		ਜੇ (!sch->sub_kset)
			ਵਾਪਸ -ENOMEM;
	}
	ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ kobject_ਜੋੜੋ(&sch->kobj, ਨਿਰਾ, "root");
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਪੂਰਨ check_hotplug_seq(ਬਣਤਰ scx_sched *sch,
			      ਅਚਲ ਬਣਤਰ sched_ext_ops *ops)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ global_hotplug_seq;

	/*
	 * If a hotplug event has occurred between when a scheduler was
	 * initialized, and when we were able to attach, exit and notify user
	 * space about it.
	 */
	ਜੇ (ops->hotplug_seq) {
		global_hotplug_seq = atomic_long_read(&scx_hotplug_seq);
		ਜੇ (ops->hotplug_seq != global_hotplug_seq) {
			scx_exit(sch, SCX_EXIT_UNREG_KERN,
				 SCX_ECODE_ACT_RESTART | SCX_ECODE_RSN_HOTPLUG,
				 "expected hotplug seq %llu did not match actual %llu",
				 ops->hotplug_seq, global_hotplug_seq);
			ਵਾਪਸ -EBUSY;
		}
	}

	ਵਾਪਸ 0;
}

ਪੂਰਨ scx_validate_ops(ਬਣਤਰ scx_sched *sch, ਅਚਲ ਬਣਤਰ sched_ext_ops *ops)
{
	/*
	 * It doesn't make sense to specify the SCX_OPS_ENQ_LAST flag if the
	 * ops.enqueue() callback isn't implemented.
	 */
	ਜੇ ((ops->flags & SCX_OPS_ENQ_ਆਖਰੀ) && !ops->ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ) {
		scx_error(sch, "SCX_OPS_ENQ_LAST requires ops.enqueue() to be implemented");
		ਵਾਪਸ -EINVAL;
	}

	/*
	 * SCX_OPS_TID_TO_TASK is enabled by the root scheduler. A sub-sched
	 * may set it to declare a dependency; reject if the root hasn't
	 * enabled it.
	 */
	ਜੇ ((ops->flags & SCX_OPS_TID_ਮੰਜ਼ਿਲ_TASK) && scx_ਮਾਪਾ(sch) &&
	    !(sch->ancestors[0]->ops.flags & SCX_OPS_TID_ਮੰਜ਼ਿਲ_TASK)) {
		scx_error(sch, "SCX_OPS_TID_TO_TASK requires root scheduler to enable it");
		ਵਾਪਸ -EINVAL;
	}

	/*
	 * SCX_OPS_BUILTIN_IDLE_PER_NODE requires built-in CPU idle
	 * selection policy to be enabled.
	 */
	ਜੇ ((ops->flags & SCX_OPS_BUILTIN_IDLE_ਪ੍ਰਤੀ_ਗੰਢ) &&
	    (ops->update_idle && !(ops->flags & SCX_OPS_KEEP_BUILTIN_IDLE))) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE requires CPU idle selection enabled");
		ਵਾਪਸ -EINVAL;
	}

	/*
	 * cid-form's struct is shorter and doesn't include the cpu_acquire /
	 * cpu_release tail; reading those fields off a cid-form @ops would
	 * run past the BPF allocation. Skip for cid-form.
	 */
	ਜੇ (!sch->is_cid_type && (ops->cpu_acquire || ops->cpu_release))
		pr_warn_ratelimited("ops->cpu_acquire/release() are deprecated, use sched_switch TP instead\n");

	/*
	 * Sub-scheduler support is tied to the cid-form struct_ops. A sub-sched
	 * attaches through a cid-form-only interface (sub_attach/sub_detach),
	 * and a root that accepts sub-scheds must expose cid-form state to
	 * them. Reject cpu-form schedulers on either side.
	 */
	ਜੇ (!sch->is_cid_type) {
		ਜੇ (scx_ਮਾਪਾ(sch)) {
			scx_error(sch, "sub-sched requires cid-form struct_ops");
			ਵਾਪਸ -EINVAL;
		}
		ਜੇ (ops->sub_attach || ops->sub_detach) {
			scx_error(sch, "sub_attach/sub_detach requires cid-form struct_ops");
			ਵਾਪਸ -EINVAL;
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ scx_ਜੜ੍ਹ_enable_workfn(ਬਣਤਰ kthread_work *work)
{
	ਬਣਤਰ scx_enable_cmd *cmd = container_ਦਾ(work, ਬਣਤਰ scx_enable_cmd, work);
	ਬਣਤਰ sched_ext_ops *ops = cmd->ops;
	ਬਣਤਰ cgroup *cgrp = ਜੜ੍ਹ_cgroup();
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_task_iter sti;
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ i, cpu, ret;

	mutex_ਤਾਲਾ(&scx_enable_mutex);

	ਜੇ (scx_enable_state() != SCX_DISABLED) {
		ret = -EBUSY;
		ਜਾਓ err_unlock;
	}

	/*
	 * @ops->priv binds @ops to its scx_sched instance. It is set here by
	 * scx_alloc_and_add_sched() and cleared at the tail of bpf_scx_unreg(),
	 * which runs after scx_root_disable() has dropped scx_enable_mutex. If
	 * it's still non-NULL here, a previous attachment on @ops has not
	 * finished tearing down; proceeding would let the in-flight unreg's
	 * RCU_INIT_POINTER(NULL) clobber the @ops->priv we are about to assign.
	 */
	ਜੇ (rcu_access_ਸੂਚਕ(ops->priv)) {
		ret = -EBUSY;
		ਜਾਓ err_unlock;
	}

	ret = alloc_kick_syncs();
	ਜੇ (ret)
		ਜਾਓ err_unlock;

	ਜੇ (ops->flags & SCX_OPS_TID_ਮੰਜ਼ਿਲ_TASK) {
		ret = rhashtable_init(&scx_tid_hash, &scx_tid_hash_params);
		ਜੇ (ret)
			ਜਾਓ err_free_ksyncs;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	cgroup_get(cgrp);
#ਜੇ_ਅੰਤ
	/*
	 * Transition to ENABLING to arm the disable path. Allocation failure
	 * still unwinds locally. Full disabling on failure applies only after
	 * scx_alloc_and_add_sched() succeeds.
	 */
	WARN_ON_ONCE(scx_ਸੈੱਟ_enable_state(SCX_ENABLING) != SCX_DISABLED);
	WARN_ON_ONCE(scx_ਜੜ੍ਹ);

	sch = scx_alloc_and_ਜੋੜੋ_sched(cmd, cgrp, ਨਿਰਾ);
	ਜੇ (IS_ERR(sch)) {
		ret = PTR_ERR(sch);
		WARN_ON_ONCE(scx_ਸੈੱਟ_enable_state(SCX_DISABLED) != SCX_ENABLING);
		ਜਾਓ err_free_tid_hash;
	}

	ਜੇ (sch->is_cid_type)
		static_branch_enable(&__scx_is_cid_type);

	atomic_long_ਸੈੱਟ(&scx_nr_rejected, 0);

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ rq *rq = cpu_rq(cpu);

		rq->scx.local_dsq.sched = sch;
		rq->scx.cpuperf_ਨਿਸ਼ਾਨਾ = SCX_CPUPERF_ONE;
	}

	scx_discard_stale_ecaps_syncs();
	scx_rescue_ਸੈੱਟ_knobs(sch);

	/*
	 * Keep CPUs stable during enable so that the BPF scheduler can track
	 * online CPUs by watching ->on/offline_cpu() after ->init().
	 */
	cpus_read_ਤਾਲਾ();

	/*
	 * Build the cid mapping into a private under-construction set. It
	 * becomes visible to readers only through scx_cid_publish_tables() once
	 * ops.init_cids() has finalized the layout.
	 */
	ret = scx_cid_init(sch);
	ਜੇ (ret) {
		cpus_read_unlock();
		ਜਾਓ err_disable;
	}

	/*
	 * Make the scheduler instance visible. Must be inside cpus_read_lock().
	 * See handle_hotplug().
	 */
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(scx_ਜੜ੍ਹ, sch);

	ret = scx_link_sched(sch);
	ਜੇ (ret) {
		cpus_read_unlock();
		ਜਾਓ err_disable;
	}

	scx_idle_enable(ops);

	/*
	 * A cid-form scheduler finalizes its cid layout in ops.init_cids(),
	 * which may call scx_bpf_cid_override(). Run it before the caps and
	 * shard state are built so the final layout is in effect.
	 */
	ਜੇ (sch->is_cid_type && sch->ops_cid.init_cids) {
		ret = SCX_CALL_OP_RET(sch, init_cids, ਨਿਰਾ);
		ਜੇ (ret) {
			ret = scx_ops_sanitize_err(sch, "init_cids", ret);
			cpus_read_unlock();
			scx_error(sch, "ops.init_cids() failed (%d)", ret);
			ਜਾਓ err_disable;
		}
	}

	/* the cid layout is final, expose it to readers */
	scx_cid_publish_tables();

	ret = scx_arena_pool_init(sch);
	ਜੇ (ret) {
		cpus_read_unlock();
		ਜਾਓ err_disable;
	}

	ret = scx_alloc_kern_arena_objs(sch);
	ਜੇ (ret) {
		cpus_read_unlock();
		ਜਾਓ err_disable;
	}

	ret = scx_alloc_pshards(sch);
	ਜੇ (ret) {
		cpus_read_unlock();
		ਜਾਓ err_disable;
	}

	scx_init_ਜੜ੍ਹ_caps(sch);

	/* the cid caps and shards are live now, so ops.init() can query them */
	ਜੇ (sch->ops.init) {
		ret = SCX_CALL_OP_RET(sch, init, ਨਿਰਾ);
		ਜੇ (ret) {
			ret = scx_ops_sanitize_err(sch, "init", ret);
			cpus_read_unlock();
			scx_error(sch, "ops.init() failed (%d)", ret);
			ਜਾਓ err_disable;
		}
		sch->exit_info->flags |= SCX_EFLAG_INITIALIZED;
	}

	ret = scx_sched_sysfs_ਜੋੜੋ(sch);
	ਜੇ (ret) {
		cpus_read_unlock();
		ਜਾਓ err_disable;
	}

	ਲਈ (i = SCX_OPI_CPU_HOTPLUG_BEGIN; i < SCX_OPI_CPU_HOTPLUG_ਸਿਰਾ; i++)
		ਜੇ (((ਖਾਲੀ (**)(ਖਾਲੀ))ops)[i])
			ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(i, sch->has_op);

	ret = check_hotplug_seq(sch, ops);
	ਜੇ (ret) {
		cpus_read_unlock();
		ਜਾਓ err_disable;
	}
	scx_idle_update_selcpu_topology(ops);

	cpus_read_unlock();

	ret = scx_validate_ops(sch, ops);
	ਜੇ (ret)
		ਜਾਓ err_disable;

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
		ਬਣਤਰ rq *rq = cpu_rq(cpu);

		scoped_guard(rq_ਤਾਲਾ_irqsave, rq) {
			update_rq_clock(rq);
			ret = dl_server_attach_bw(&rq->ext_server);
		}
		ਜੇ (ret) {
			pr_warn("sched_ext: failed to attach ext_server on CPU %d (%d)\n",
				cpu, ret);
			ਜਾਓ err_disable;
		}
	}

	/*
	 * Once __scx_enabled is set, %current can be switched to SCX anytime.
	 * This can lead to stalls as some BPF schedulers (e.g. userspace
	 * scheduling) may not function correctly before all tasks are switched.
	 * Init in bypass mode to guarantee forward progress.
	 */
	scx_bypass(sch, true);

	ਲਈ (i = SCX_OPI_ਸਾਧਾਰਨ_BEGIN; i < SCX_OPI_ਸਾਧਾਰਨ_ਸਿਰਾ; i++)
		ਜੇ (((ਖਾਲੀ (**)(ਖਾਲੀ))ops)[i])
			ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(i, sch->has_op);

	ਜੇ (sch->ops.cpu_acquire || sch->ops.cpu_release)
		sch->ops.flags |= SCX_OPS_HAS_CPU_PREEMPT;

	/*
	 * Lock out forks, cgroup on/offlining and moves before opening the
	 * floodgate so that they don't wander into the operations prematurely.
	 */
	percpu_ਥੱਲੇ_write(&scx_fork_rwsem);

	WARN_ON_ONCE(scx_init_task_enabled);
	scx_init_task_enabled = true;

	/* flip under fork_rwsem; the iter below covers existing tasks */
	ਜੇ (ops->flags & SCX_OPS_TID_ਮੰਜ਼ਿਲ_TASK)
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
	scx_cgroup_ਤਾਲਾ();
	ਸੈੱਟ_cgroup_sched(sch_cgroup(sch), sch);
	ret = scx_cgroup_init(sch);
	ਜੇ (ret)
		ਜਾਓ err_disable_unlock_all;

	WARN_ON_ONCE(scx_cgroup_enabled);
	scx_cgroup_enabled = true;

	scx_task_iter_ਸ਼ੁਰੂ(&sti, ਨਿਰਾ);
	ਜਦੋਂ ((p = scx_task_iter_ਅਗਲਾ_locked(&sti))) {
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
		scx_ਸੈੱਟ_task_state(p, SCX_TASK_INIT_BEGIN);
		scx_task_iter_unlock(&sti);

		ret = __scx_init_task(sch, p, ਨਿਰਾ, false);

		scx_task_iter_relock(&sti, p);

		ਜੇ (unlikely(ret)) {
			ਜੇ (scx_get_task_state(p) != SCX_TASK_DEAD)
				scx_ਸੈੱਟ_task_state(p, SCX_TASK_NONE);
			scx_task_iter_stop(&sti);
			scx_error(sch, "ops.init_task() failed (%d) for %s[%d]",
				  ret, p->comm, p->pid);
			put_task_struct(p);
			ਜਾਓ err_disable_unlock_all;
		}

		ਜੇ (scx_get_task_state(p) == SCX_TASK_DEAD) {
			/*
			 * sched_ext_dead() observed %INIT_BEGIN and set %DEAD.
			 * ops.exit_task() is owed to the sched __scx_init_task()
			 * ran against; call it now.
			 */
			scx_sub_init_cancel_task(sch, p);
		} ਨਹੀਂਤਾਂ {
			scx_ਸੈੱਟ_task_state(p, SCX_TASK_INIT);
			scx_ਸੈੱਟ_task_sched(p, sch);
			scx_ਸੈੱਟ_task_state(p, SCX_TASK_READY);
		}

		/*
		 * Insert into the tid hash. scx_tasks_lock is held by the iter;
		 * list_empty() guards against sched_ext_dead() having taken @p
		 * off the list while init ran unlocked.
		 */
		ਜੇ (scx_tid_ਮੰਜ਼ਿਲ_task_enabled() && !list_ਖਾਲੀ_ਥਾਵਾਂ(&p->scx.tasks_ਗੰਢ))
			scx_tid_hash_ਪਾਓ(p);

		put_task_struct(p);
	}
	scx_task_iter_stop(&sti);
	scx_cgroup_unlock();
	percpu_ਉੱਪਰ_write(&scx_fork_rwsem);

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
	percpu_ਥੱਲੇ_write(&scx_fork_rwsem);
	scx_task_iter_ਸ਼ੁਰੂ(&sti, ਨਿਰਾ);
	ਜਦੋਂ ((p = scx_task_iter_ਅਗਲਾ_locked(&sti))) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕਤਾਰ_flags = ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE;
		ਅਚਲ ਬਣਤਰ sched_class *old_class = p->sched_class;
		ਅਚਲ ਬਣਤਰ sched_class *new_class = scx_setscheduler_class(p);

		ਜੇ (scx_get_task_state(p) != SCX_TASK_READY)
			ਜਾਰੀ;

		ਜੇ (old_class != new_class)
			ਕਤਾਰ_flags |= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS;

		scoped_guard (sched_change, p, ਕਤਾਰ_flags) {
			scx_ਸੈੱਟ_task_ਟੁਕੜਾ(p, READ_ONCE(sch->ਟੁਕੜਾ_dfl));
			p->sched_class = new_class;
		}
	}
	scx_task_iter_stop(&sti);
	percpu_ਉੱਪਰ_write(&scx_fork_rwsem);

	scx_bypass(sch, false);

	ਜੇ (!scx_tryset_enable_state(SCX_ENABLED, SCX_ENABLING)) {
		WARN_ON_ONCE(atomic_read(&sch->exit_ਕਿਸਮ) == SCX_EXIT_NONE);
		ret = -EBUSY;
		ਜਾਓ err_disable;
	}

	ਜੇ (!(ops->flags & SCX_OPS_SWITCH_PARTIAL))
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
	ਜੇ (scx_switched_all()) {
		for_each_possible_cpu(cpu) {
			ਬਣਤਰ rq *rq = cpu_rq(cpu);

			guard(rq_ਤਾਲਾ_irqsave)(rq);
			update_rq_clock(rq);
			dl_server_detach_bw(&rq->fair_server);
		}
	}

	pr_info("sched_ext: BPF scheduler \"%s\" enabled%s\n",
		sch->ops.name, scx_switched_all() ? "" : " (partial)");
	kobject_uevent(&sch->kobj, KOBJ_ਜੋੜੋ);
	mutex_unlock(&scx_enable_mutex);

	atomic_long_inc(&scx_enable_seq);

	cmd->ret = 0;
	ਵਾਪਸ;

err_free_tid_hash:
	ਜੇ (ops->flags & SCX_OPS_TID_ਮੰਜ਼ਿਲ_TASK)
		rhashtable_free_and_destroy(&scx_tid_hash, ਨਿਰਾ, ਨਿਰਾ);
err_free_ksyncs:
	free_kick_syncs();
err_unlock:
	mutex_unlock(&scx_enable_mutex);
	cmd->ret = ret;
	ਵਾਪਸ;

err_disable_unlock_all:
	scx_cgroup_unlock();
	percpu_ਉੱਪਰ_write(&scx_fork_rwsem);
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

ਸਥਿਰ s32 scx_enable(ਬਣਤਰ scx_enable_cmd *cmd, ਬਣਤਰ bpf_link *link)
{
	ਸਥਿਰ ਬਣਤਰ kthread_ਕਾਮਾ *helper;
	ਸਥਿਰ DEFINE_MUTEX(helper_mutex);

	ਜੇ (housekeeping_enabled(HK_TYPE_DOMAIN_BOOT)) {
		pr_err("sched_ext: Not compatible with \"isolcpus=\" domain isolation\n");
		ਵਾਪਸ -EINVAL;
	}

	ਜੇ (!READ_ONCE(helper)) {
		mutex_ਤਾਲਾ(&helper_mutex);
		ਜੇ (!helper) {
			ਬਣਤਰ kthread_ਕਾਮਾ *w =
				kthread_ਲੜੀਵਾਰ_ਕਾਮਾ(0, "scx_enable_helper");
			ਜੇ (IS_ERR_OR_NULL(w)) {
				mutex_unlock(&helper_mutex);
				ਵਾਪਸ -ENOMEM;
			}
			sched_ਸੈੱਟ_fifo(w->task);
			WRITE_ONCE(helper, w);
		}
		mutex_unlock(&helper_mutex);
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਜੇ (cmd->ops->sub_cgroup_id > 1)
		kthread_init_work(&cmd->work, scx_sub_enable_workfn);
	ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */
		kthread_init_work(&cmd->work, scx_ਜੜ੍ਹ_enable_workfn);

	kthread_ਕਤਾਰ_work(READ_ONCE(helper), &cmd->work);
	kthread_flush_work(&cmd->work);
	ਵਾਪਸ cmd->ret;
}


/********************************************************************************
 * bpf_struct_ops plumbing.
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/bpf_verifier.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/bpf.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/btf.h>

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_type *task_struct_type;

ਸਥਿਰ bool bpf_scx_is_valid_access(ਪੂਰਨ off, ਪੂਰਨ ਮਾਪ,
				    ਗਿਣਤੀ bpf_access_type type,
				    ਅਚਲ ਬਣਤਰ bpf_prog *prog,
				    ਬਣਤਰ bpf_insn_access_aux *info)
{
	ਜੇ (type != BPF_READ)
		ਵਾਪਸ false;
	ਜੇ (off < 0 || off >= ਆਕਾਰ(__u64) * MAX_BPF_FUNC_ਦਲੀਲਾਂ)
		ਵਾਪਸ false;
	ਜੇ (off % ਮਾਪ != 0)
		ਵਾਪਸ false;

	ਵਾਪਸ btf_ctx_access(off, ਮਾਪ, type, prog, info);
}

/* common to both forms: only scx.disallow is writable */
ਸਥਿਰ ਪੂਰਨ bpf_scx_btf_struct_access_common(ਅਚਲ ਬਣਤਰ bpf_reg_state *reg,
					    ਪੂਰਨ off, ਪੂਰਨ ਮਾਪ)
{
	ਅਚਲ ਬਣਤਰ btf_type *t;

	t = btf_type_by_id(reg->btf, reg->btf_id);
	ਜੇ (t == task_struct_type &&
	    off >= offsetof(ਬਣਤਰ task_struct, scx.disallow) &&
	    off + ਮਾਪ <= offsetofend(ਬਣਤਰ task_struct, scx.disallow))
		ਵਾਪਸ SCALAR_ਮੁੱਲ;

	ਵਾਪਸ -EACCES;
}

ਸਥਿਰ ਪੂਰਨ bpf_scx_btf_struct_access(ਬਣਤਰ bpf_verifier_log *ਲਘੂਗਣਕ,
				     ਅਚਲ ਬਣਤਰ bpf_reg_state *reg, ਪੂਰਨ off,
				     ਪੂਰਨ ਮਾਪ)
{
	ਅਚਲ ਬਣਤਰ btf_type *t;

	t = btf_type_by_id(reg->btf, reg->btf_id);
	ਜੇ (t == task_struct_type) {
		ਜੇ ((off >= offsetof(ਬਣਤਰ task_struct, scx.ਟੁਕੜਾ) &&
		     off + ਮਾਪ <= offsetofend(ਬਣਤਰ task_struct, scx.ਟੁਕੜਾ)) ||
		    (off >= offsetof(ਬਣਤਰ task_struct, scx.dsq_vtime) &&
		     off + ਮਾਪ <= offsetofend(ਬਣਤਰ task_struct, scx.dsq_vtime)))
			ਵਾਪਸ SCALAR_ਮੁੱਲ;
	}

	ਵਾਪਸ bpf_scx_btf_struct_access_common(reg, off, ਮਾਪ);
}

/* cid-form rejects direct slice and dsq_vtime writes in favor of the kfuncs */
ਸਥਿਰ ਪੂਰਨ bpf_scx_cid_btf_struct_access(ਬਣਤਰ bpf_verifier_log *ਲਘੂਗਣਕ,
					 ਅਚਲ ਬਣਤਰ bpf_reg_state *reg, ਪੂਰਨ off,
					 ਪੂਰਨ ਮਾਪ)
{
	ਵਾਪਸ bpf_scx_btf_struct_access_common(reg, off, ਮਾਪ);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ bpf_verifier_ops bpf_scx_verifier_ops = {
	.get_func_proto = bpf_ਅਧਾਰ_func_proto,
	.is_valid_access = bpf_scx_is_valid_access,
	.btf_struct_access = bpf_scx_btf_struct_access,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ bpf_verifier_ops bpf_scx_cid_verifier_ops = {
	.get_func_proto = bpf_ਅਧਾਰ_func_proto,
	.is_valid_access = bpf_scx_is_valid_access,
	.btf_struct_access = bpf_scx_cid_btf_struct_access,
};

ਸਥਿਰ ਪੂਰਨ bpf_scx_init_member(ਅਚਲ ਬਣਤਰ btf_type *t,
			       ਅਚਲ ਬਣਤਰ btf_member *member,
			       ਖਾਲੀ *kdata, ਅਚਲ ਖਾਲੀ *udata)
{
	ਅਚਲ ਬਣਤਰ sched_ext_ops *uops = udata;
	ਬਣਤਰ sched_ext_ops *ops = kdata;
	u32 moff = __btf_member_bit_offset(t, member) / 8;
	ਪੂਰਨ ret;

	ਚੋਣ (moff) {
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, dispatch_max_batch):
		ਜੇ (*(u32 *)(udata + moff) > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
			ਵਾਪਸ -E2BIG;
		ops->dispatch_max_batch = *(u32 *)(udata + moff);
		ਵਾਪਸ 1;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, flags):
		ਜੇ (*(u64 *)(udata + moff) & ~SCX_OPS_ALL_FLAGS)
			ਵਾਪਸ -EINVAL;
		ops->flags = *(u64 *)(udata + moff);
		ਵਾਪਸ 1;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, name):
		ret = bpf_obj_name_cpy(ops->name, uops->name,
				       ਆਕਾਰ(ops->name));
		ਜੇ (ret < 0)
			ਵਾਪਸ ret;
		ਜੇ (ret == 0)
			ਵਾਪਸ -EINVAL;
		ਵਾਪਸ 1;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, timeout_ms):
		ਜੇ (msecs_ਮੰਜ਼ਿਲ_jiffies(*(u32 *)(udata + moff)) >
		    SCX_WATCHDOG_MAX_TIMEOUT)
			ਵਾਪਸ -E2BIG;
		ops->timeout_ms = *(u32 *)(udata + moff);
		ਵਾਪਸ 1;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, exit_dump_len):
		ops->exit_dump_len =
			*(u32 *)(udata + moff) ?: SCX_EXIT_DUMP_DFL_LEN;
		ਵਾਪਸ 1;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, hotplug_seq):
		ops->hotplug_seq = *(u64 *)(udata + moff);
		ਵਾਪਸ 1;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, cid_shard_ਮਾਪ):
		ops->cid_shard_ਮਾਪ = *(u32 *)(udata + moff);
		ਵਾਪਸ 1;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, rescue_bandwidth_ppt): {
		u32 bw_ppt = *(u32 *)(udata + moff);

		ਜੇ (bw_ppt > SCX_RESCUE_MAX_BW_PPT && bw_ppt != SCX_RESCUE_DISABLE)
			ਵਾਪਸ -E2BIG;
		ops->rescue_bandwidth_ppt = bw_ppt;
		ਵਾਪਸ 1;
	}
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, rescue_ਸਮਾਂ_ਹਿੱਸਾ_us): {
		u32 ਸਮਾਂ_ਹਿੱਸਾ_us = *(u32 *)(udata + moff);

		ਜੇ (ਸਮਾਂ_ਹਿੱਸਾ_us > SCX_RESCUE_MAX_ਸਮਾਂ_ਹਿੱਸਾ_US)
			ਵਾਪਸ -E2BIG;
		ਜੇ (ਸਮਾਂ_ਹਿੱਸਾ_us && ਸਮਾਂ_ਹਿੱਸਾ_us < SCX_RESCUE_MIN_ਸਮਾਂ_ਹਿੱਸਾ_US)
			ਵਾਪਸ -EINVAL;
		ops->rescue_ਸਮਾਂ_ਹਿੱਸਾ_us = ਸਮਾਂ_ਹਿੱਸਾ_us;
		ਵਾਪਸ 1;
	}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, sub_cgroup_id):
		ops->sub_cgroup_id = *(u64 *)(udata + moff);
		ਵਾਪਸ 1;
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ bpf_scx_check_member(ਅਚਲ ਬਣਤਰ btf_type *t,
				ਅਚਲ ਬਣਤਰ btf_member *member,
				ਅਚਲ ਬਣਤਰ bpf_prog *prog)
{
	u32 moff = __btf_member_bit_offset(t, member) / 8;

	ਚੋਣ (moff) {
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, init_task):
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, cgroup_init):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, cgroup_exit):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, cgroup_prep_move):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, cgroup_ਸੈੱਟ_bandwidth):
#ਜੇ_ਅੰਤ
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, cpu_online):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, cpu_offline):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, init_cids):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, init):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, ਬਾਹਰ_ਨਿਕਲੋ):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, sub_attach):
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, sub_detach):
		ਰੋਕੋ;
	ਮੂਲ:
		ਜੇ (prog->sleepable)
			ਵਾਪਸ -EINVAL;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	/*
	 * Enable private stack for operations that can nest along the
	 * hierarchy.
	 *
	 * XXX - Ideally, we should only do this for scheds that allow
	 * sub-scheds and sub-scheds themselves but I don't know how to access
	 * struct_ops from here.
	 */
	ਚੋਣ (moff) {
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, dispatch):
		prog->aux->priv_ਢੇਰ_requested = true;
		prog->aux->recursion_detected = scx_pstack_recursion_on_dispatch;
		ਰੋਕੋ;
	ਹਾਲਤ offsetof(ਬਣਤਰ sched_ext_ops, sub_caps_updated):
		prog->aux->priv_ਢੇਰ_requested = true;
		prog->aux->recursion_detected = scx_pstack_recursion_on_caps_updated;
		ਰੋਕੋ;
	}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ bpf_scx_reg(ਖਾਲੀ *kdata, ਬਣਤਰ bpf_link *link)
{
	ਬਣਤਰ scx_enable_cmd cmd = { .ops = kdata };

	ਵਾਪਸ scx_enable(&cmd, link);
}

ਬਣਤਰ scx_arena_ਜਾਂਚ {
	ਬਣਤਰ bpf_map	*arena;
	ਪੂਰਨ		err;
};

/*
 * The verifier enforces one arena per BPF program, so each struct_ops
 * member prog contributes at most one arena via bpf_prog_arena().
 * Require all non-NULL contributions to match.
 */
ਸਥਿਰ ਪੂਰਨ scx_arena_ਜਾਂਚ_prog(ਬਣਤਰ bpf_prog *prog, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ scx_arena_ਜਾਂਚ *s = ਅੰਕੜੇ;
	ਬਣਤਰ bpf_map *arena = ਨਿਰਾ;

	/* arena.o, which defines these, is built only on MMU && 64BIT */
#ਜੇ defined(CONFIG_MMU) && defined(CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ)
	arena = bpf_prog_arena(prog);
#ਜੇ_ਅੰਤ
	ਜੇ (!arena)
		ਵਾਪਸ 0;
	ਜੇ (s->arena && s->arena != arena) {
		s->err = -EINVAL;
		ਵਾਪਸ 1;
	}
	s->arena = arena;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ bpf_scx_reg_cid(ਖਾਲੀ *kdata, ਬਣਤਰ bpf_link *link)
{
	ਬਣਤਰ scx_enable_cmd cmd = { .ops_cid = kdata, .is_cid_type = true };
	ਬਣਤਰ scx_arena_ਜਾਂਚ ਜਾਂਚ = {};
	ਪੂਰਨ ret;

	bpf_struct_ops_for_each_prog(kdata, scx_arena_ਜਾਂਚ_prog, &ਜਾਂਚ);
	ਜੇ (ਜਾਂਚ.err) {
		pr_err("sched_ext: cid-form scheduler uses multiple arena maps\n");
		ਵਾਪਸ ਜਾਂਚ.err;
	}
	ਜੇ (!ਜਾਂਚ.arena) {
		pr_err("sched_ext: cid-form scheduler must use a BPF arena map\n");
		ਵਾਪਸ -EINVAL;
	}

	bpf_map_inc(ਜਾਂਚ.arena);
	cmd.arena_map = ਜਾਂਚ.arena;
	ret = scx_enable(&cmd, link);
	ਜੇ (cmd.arena_map)		/* not consumed by scx_alloc_and_add_sched() */
		bpf_map_put(cmd.arena_map);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ bpf_scx_unreg(ਖਾਲੀ *kdata, ਬਣਤਰ bpf_link *link)
{
	ਬਣਤਰ sched_ext_ops *ops = kdata;
	ਬਣਤਰ scx_sched *sch = rcu_dereference_protected(ops->priv, true);

	scx_disable(sch, SCX_EXIT_UNREG);
	scx_flush_disable_work(sch);
	RCU_INIT_ਸੂਚਕ(ops->priv, ਨਿਰਾ);
	kobject_put(&sch->kobj);
}

ਸਥਿਰ ਪੂਰਨ bpf_scx_init(ਬਣਤਰ btf *btf)
{
	task_struct_type = btf_type_by_id(btf, btf_tracing_ids[BTF_TRACING_TYPE_TASK]);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ bpf_scx_update(ਖਾਲੀ *kdata, ਖਾਲੀ *old_kdata, ਬਣਤਰ bpf_link *link)
{
	/*
	 * sched_ext does not support updating the actively-loaded BPF
	 * scheduler, as registering a BPF scheduler can always fail if the
	 * scheduler returns an error code for e.g. ops.init(), ops.init_task(),
	 * etc. Similarly, we can always race with unregistration happening
	 * elsewhere, such as with sysrq.
	 */
	ਵਾਪਸ -EOPNOTSUPP;
}

ਸਥਿਰ ਪੂਰਨ bpf_scx_validate(ਖਾਲੀ *kdata)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ s32 sched_ext_ops__select_cpu(ਬਣਤਰ task_struct *p, s32 prev_cpu, u64 wake_flags) { ਵਾਪਸ -EINVAL; }
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ task_struct *p, u64 enq_flags) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ task_struct *p, u64 enq_flags) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__dispatch(s32 prev_cpu, ਬਣਤਰ task_struct *prev__nullable) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__tick(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__runnable(ਬਣਤਰ task_struct *p, u64 enq_flags) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__running(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__stopping(ਬਣਤਰ task_struct *p, bool runnable) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__quiescent(ਬਣਤਰ task_struct *p, u64 deq_flags) {}
ਸਥਿਰ bool sched_ext_ops__yield(ਬਣਤਰ task_struct *from, ਬਣਤਰ task_struct *ਮੰਜ਼ਿਲ_nullable) { ਵਾਪਸ false; }
ਸਥਿਰ bool sched_ext_ops__core_sched_before(ਬਣਤਰ task_struct *a, ਬਣਤਰ task_struct *b) { ਵਾਪਸ false; }
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_ਸੈੱਟ_ਭਾਰ(ਬਣਤਰ task_struct *p, u32 ਭਾਰ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_ਸੈੱਟ_cpumask(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ cpumask *ਨਕਾਬ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__update_idle(s32 cpu, bool idle) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__cpu_acquire(s32 cpu, ਬਣਤਰ scx_cpu_acquire_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__cpu_release(s32 cpu, ਬਣਤਰ scx_cpu_release_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) {}
ਸਥਿਰ s32 sched_ext_ops__init_task(ਬਣਤਰ task_struct *p, ਬਣਤਰ scx_init_task_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) { ਵਾਪਸ -EINVAL; }
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__exit_task(ਬਣਤਰ task_struct *p, ਬਣਤਰ scx_exit_task_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__enable(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__disable(ਬਣਤਰ task_struct *p) {}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
ਸਥਿਰ s32 sched_ext_ops__cgroup_init(ਬਣਤਰ cgroup *cgrp, ਬਣਤਰ scx_cgroup_init_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) { ਵਾਪਸ -EINVAL; }
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__cgroup_exit(ਬਣਤਰ cgroup *cgrp) {}
ਸਥਿਰ s32 sched_ext_ops__cgroup_prep_move(ਬਣਤਰ task_struct *p, ਬਣਤਰ cgroup *from, ਬਣਤਰ cgroup *ਮੰਜ਼ਿਲ) { ਵਾਪਸ -EINVAL; }
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__cgroup_move(ਬਣਤਰ task_struct *p, ਬਣਤਰ cgroup *from, ਬਣਤਰ cgroup *ਮੰਜ਼ਿਲ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__cgroup_cancel_move(ਬਣਤਰ task_struct *p, ਬਣਤਰ cgroup *from, ਬਣਤਰ cgroup *ਮੰਜ਼ਿਲ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_cgroup_ਸੈੱਟ_ਭਾਰ(ਬਣਤਰ cgroup *cgrp, u32 ਭਾਰ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_cgroup_ਸੈੱਟ_bandwidth(ਬਣਤਰ cgroup *cgrp, u64 period_us, u64 quota_us, u64 ਮਿਆਦ_us) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_cgroup_ਸੈੱਟ_idle(ਬਣਤਰ cgroup *cgrp, bool idle) {}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_GROUP_SCHED */
ਸਥਿਰ s32 sched_ext_ops__sub_attach(ਬਣਤਰ scx_sub_attach_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) { ਵਾਪਸ -EINVAL; }
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__sub_detach(ਬਣਤਰ scx_sub_detach_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__cpu_online(s32 cpu) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__cpu_offline(s32 cpu) {}
ਸਥਿਰ s32 sched_ext_ops__init_cids(ਖਾਲੀ) { ਵਾਪਸ -EINVAL; }
ਸਥਿਰ s32 sched_ext_ops__init(ਖਾਲੀ) { ਵਾਪਸ -EINVAL; }
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__exit(ਬਣਤਰ scx_exit_info *info) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__dump(ਬਣਤਰ scx_dump_ctx *ctx) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__dump_cpu(ਬਣਤਰ scx_dump_ctx *ctx, s32 cpu, bool idle) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__dump_task(ਬਣਤਰ scx_dump_ctx *ctx, ਬਣਤਰ task_struct *p) {}

ਸਥਿਰ ਬਣਤਰ sched_ext_ops __bpf_ops_sched_ext_ops = {
	.select_cpu		= sched_ext_ops__select_cpu,
	.ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ		= sched_ext_ops_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ,
	.ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ		= sched_ext_ops_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ,
	.dispatch		= sched_ext_ops__dispatch,
	.tick			= sched_ext_ops__tick,
	.runnable		= sched_ext_ops__runnable,
	.running		= sched_ext_ops__running,
	.stopping		= sched_ext_ops__stopping,
	.quiescent		= sched_ext_ops__quiescent,
	.yield			= sched_ext_ops__yield,
	.core_sched_before	= sched_ext_ops__core_sched_before,
	.ਸੈੱਟ_ਭਾਰ		= sched_ext_ops_ਸੈੱਟ_ਭਾਰ,
	.ਸੈੱਟ_cpumask		= sched_ext_ops_ਸੈੱਟ_cpumask,
	.update_idle		= sched_ext_ops__update_idle,
	.cpu_acquire		= sched_ext_ops__cpu_acquire,
	.cpu_release		= sched_ext_ops__cpu_release,
	.init_task		= sched_ext_ops__init_task,
	.exit_task		= sched_ext_ops__exit_task,
	.enable			= sched_ext_ops__enable,
	.disable		= sched_ext_ops__disable,
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	.cgroup_init		= sched_ext_ops__cgroup_init,
	.cgroup_exit		= sched_ext_ops__cgroup_exit,
	.cgroup_prep_move	= sched_ext_ops__cgroup_prep_move,
	.cgroup_move		= sched_ext_ops__cgroup_move,
	.cgroup_cancel_move	= sched_ext_ops__cgroup_cancel_move,
	.cgroup_ਸੈੱਟ_ਭਾਰ	= sched_ext_ops_cgroup_ਸੈੱਟ_ਭਾਰ,
	.cgroup_ਸੈੱਟ_bandwidth	= sched_ext_ops_cgroup_ਸੈੱਟ_bandwidth,
	.cgroup_ਸੈੱਟ_idle	= sched_ext_ops_cgroup_ਸੈੱਟ_idle,
#ਜੇ_ਅੰਤ
	.sub_attach		= sched_ext_ops__sub_attach,
	.sub_detach		= sched_ext_ops__sub_detach,
	.cpu_online		= sched_ext_ops__cpu_online,
	.cpu_offline		= sched_ext_ops__cpu_offline,
	.init_cids		= sched_ext_ops__init_cids,
	.init			= sched_ext_ops__init,
	.ਬਾਹਰ_ਨਿਕਲੋ			= sched_ext_ops__exit,
	.dump			= sched_ext_ops__dump,
	.dump_cpu		= sched_ext_ops__dump_cpu,
	.dump_task		= sched_ext_ops__dump_task,
};

ਸਥਿਰ ਬਣਤਰ bpf_struct_ops bpf_sched_ext_ops = {
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
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_cid_ਸੈੱਟ_cmask(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ scx_cmask *cmask__arena) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops_cid__enable(ਬਣਤਰ task_struct *p, ਬਣਤਰ scx_enable_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__sub_caps_updated(ਅਚਲ ਬਣਤਰ scx_cmask *cmask__arena, u64 caps) {}
ਸਥਿਰ ਖਾਲੀ sched_ext_ops__sub_ecaps_updated(s32 cid, u64 before, u64 after) {}

ਸਥਿਰ ਬਣਤਰ sched_ext_ops_cid __bpf_ops_sched_ext_ops_cid = {
	.select_cid		= sched_ext_ops__select_cpu,
	.ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ		= sched_ext_ops_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ,
	.ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ		= sched_ext_ops_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ,
	.dispatch		= sched_ext_ops__dispatch,
	.tick			= sched_ext_ops__tick,
	.runnable		= sched_ext_ops__runnable,
	.running		= sched_ext_ops__running,
	.stopping		= sched_ext_ops__stopping,
	.quiescent		= sched_ext_ops__quiescent,
	.yield			= sched_ext_ops__yield,
	.core_sched_before	= sched_ext_ops__core_sched_before,
	.ਸੈੱਟ_ਭਾਰ		= sched_ext_ops_ਸੈੱਟ_ਭਾਰ,
	.ਸੈੱਟ_cmask		= sched_ext_ops_cid_ਸੈੱਟ_cmask,
	.update_idle		= sched_ext_ops__update_idle,
	.init_task		= sched_ext_ops__init_task,
	.exit_task		= sched_ext_ops__exit_task,
	.enable			= sched_ext_ops_cid__enable,
	.disable		= sched_ext_ops__disable,
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	.cpuctl_init		= sched_ext_ops__cgroup_init,
	.cpuctl_exit		= sched_ext_ops__cgroup_exit,
	.cpuctl_prep_move	= sched_ext_ops__cgroup_prep_move,
	.cpuctl_move		= sched_ext_ops__cgroup_move,
	.cpuctl_cancel_move	= sched_ext_ops__cgroup_cancel_move,
	.cpuctl_ਸੈੱਟ_ਭਾਰ	= sched_ext_ops_cgroup_ਸੈੱਟ_ਭਾਰ,
	.cpuctl_ਸੈੱਟ_bandwidth	= sched_ext_ops_cgroup_ਸੈੱਟ_bandwidth,
	.cpuctl_ਸੈੱਟ_idle	= sched_ext_ops_cgroup_ਸੈੱਟ_idle,
#ਜੇ_ਅੰਤ
	.sub_attach		= sched_ext_ops__sub_attach,
	.sub_detach		= sched_ext_ops__sub_detach,
	.sub_caps_updated	= sched_ext_ops__sub_caps_updated,
	.sub_ecaps_updated	= sched_ext_ops__sub_ecaps_updated,
	.cid_online		= sched_ext_ops__cpu_online,
	.cid_offline		= sched_ext_ops__cpu_offline,
	.init_cids		= sched_ext_ops__init_cids,
	.init			= sched_ext_ops__init,
	.ਬਾਹਰ_ਨਿਕਲੋ			= sched_ext_ops__exit,
	.dump			= sched_ext_ops__dump,
	.dump_cid		= sched_ext_ops__dump_cpu,
	.dump_task		= sched_ext_ops__dump_task,
};

/*
 * The cid-form struct_ops shares all bpf_struct_ops hooks with the cpu form.
 * init_member, check_member, reg, unreg, etc. process kdata as the byte block
 * verified to match by the BUILD_BUG_ON checks in scx_init().
 */
ਸਥਿਰ ਬਣਤਰ bpf_struct_ops bpf_sched_ext_ops_cid = {
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

ਸਥਿਰ ਖਾਲੀ sysrq_handle_sched_ext_reset(u8 ਕੁੰਜੀ)
{
	ਬਣਤਰ scx_sched *sch;

	sch = rcu_dereference(scx_ਜੜ੍ਹ);
	ਜੇ (likely(sch))
		scx_disable(sch, SCX_EXIT_SYSRQ);
	ਨਹੀਂਤਾਂ
		pr_info("sched_ext: BPF schedulers not loaded\n");
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ sysrq_ਕੁੰਜੀ_op sysrq_sched_ext_reset_op = {
	.handler	= sysrq_handle_sched_ext_reset,
	.help_ਸੁਨੇਹਾ	= "reset-sched-ext(S)",
	.action_ਸੁਨੇਹਾ	= "Disable sched_ext and revert all tasks to CFS",
	.enable_ਨਕਾਬ	= SYSRQ_ENABLE_RTNICE,
};

ਸਥਿਰ ਖਾਲੀ sysrq_handle_sched_ext_dump(u8 ਕੁੰਜੀ)
{
	ਬਣਤਰ scx_exit_info ei = {
		.ਕਿਸਮ		= SCX_EXIT_NONE,
		.exit_cpu	= -1,
		.reason		= "SysRq-D",
	};
	ਬਣਤਰ scx_sched *sch;

	list_for_each_ਇੰਦਰਾਜ_rcu(sch, &scx_sched_all, all)
		scx_dump_state(sch, &ei, 0, false);
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ sysrq_ਕੁੰਜੀ_op sysrq_sched_ext_dump_op = {
	.handler	= sysrq_handle_sched_ext_dump,
	.help_ਸੁਨੇਹਾ	= "dump-sched-ext(D)",
	.action_ਸੁਨੇਹਾ	= "Trigger sched_ext debug dump",
	.enable_ਨਕਾਬ	= SYSRQ_ENABLE_RTNICE,
};

ਸਥਿਰ bool can_skip_idle_kick(ਬਣਤਰ rq *rq)
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
	ਵਾਪਸ !is_idle_task(rq->curr) && !(rq->scx.flags & SCX_RQ_ਵਿੱਚ_DISPATCH);
}

ਸਥਿਰ bool kick_one_cpu(s32 cpu, ਬਣਤਰ scx_sched_pcpu *pcpu, ਬਣਤਰ rq *this_rq,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ksyncs)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਣਤਰ scx_rq *this_scx = &this_rq->scx;
	ਅਚਲ ਬਣਤਰ sched_class *cur_class;
	bool should_wait = false;
	bool kickable;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	raw_spin_rq_ਤਾਲਾ_irqsave(rq, flags);
	cur_class = rq->curr->sched_class;

	/*
	 * During CPU hotplug, a CPU may depend on kicking itself to make
	 * forward progress. Allow kicking self regardless of online state. If
	 * @cpu is running a higher class task, we have no control over @cpu.
	 * Skip kicking. A sub-sched lacking baseline access on @cid has no
	 * business forcing a reschedule there - skip. This is the authoritative
	 * cap check: ecaps is read here under @rq's lock.
	 */
	kickable = (cpu_online(cpu) || cpu == cpu_ਦਾ(this_rq)) &&
		   !sched_class_above(cur_class, &ext_sched_class);

	ਜੇ (kickable && !scx_missing_caps(pcpu->sch, cpu, SCX_CAP_ਅਧਾਰ)) {
		ਜੇ (cpumask_test_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_preempt)) {
			ਜੇ (cur_class == &ext_sched_class) {
				u64 caps = scx_caps_for_preempt(pcpu->sch, rq, 0);

				ਜੇ (unlikely(scx_missing_caps(pcpu->sch, cpu, caps)))
					__scx_add_event(pcpu->sch, SCX_EV_SUB_PREEMPT_DENIED, 1);
				ਨਹੀਂਤਾਂ ਜੇ (unlikely(!scx_ਸੈੱਟ_task_ਟੁਕੜਾ(rq->curr, 0)))
					__scx_add_event(pcpu->sch, SCX_EV_ਟੁਕੜਾ_DENIED, 1);
			}
			cpumask_clear_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_preempt);
		}

		ਜੇ (cpumask_test_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_wait)) {
			ਜੇ (cur_class == &ext_sched_class) {
				cpumask_ਸੈੱਟ_cpu(cpu, this_scx->cpus_ਮੰਜ਼ਿਲ_sync);
				ksyncs[cpu] = rq->scx.kick_sync;
				should_wait = true;
			}
			cpumask_clear_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_wait);
		}

		resched_curr(rq);
	} ਨਹੀਂਤਾਂ {
		/* a kickable cpu was skipped solely for the missing caps */
		ਜੇ (kickable)
			__scx_add_event(pcpu->sch, SCX_EV_SUB_KICK_DENIED, 1);
		cpumask_clear_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_preempt);
		cpumask_clear_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_wait);
	}

	scx_rq_ਤਾਲਾ_drop(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	ਵਾਪਸ should_wait;
}

ਸਥਿਰ ਖਾਲੀ kick_one_cpu_if_idle(s32 cpu, ਬਣਤਰ scx_sched_pcpu *pcpu,
				 ਬਣਤਰ rq *this_rq)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	raw_spin_rq_ਤਾਲਾ_irqsave(rq, flags);

	/* idle kicks need baseline access too, see kick_one_cpu() */
	ਜੇ (!can_skip_idle_kick(rq) &&
	    (cpu_online(cpu) || cpu == cpu_ਦਾ(this_rq))) {
		ਜੇ (likely(!scx_missing_caps(pcpu->sch, cpu, SCX_CAP_ਅਧਾਰ)))
			resched_curr(rq);
		ਨਹੀਂਤਾਂ
			__scx_add_event(pcpu->sch, SCX_EV_SUB_KICK_DENIED, 1);
	}

	scx_rq_ਤਾਲਾ_drop(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

ਸਥਿਰ ਖਾਲੀ kick_cpus_irq_workfn(ਬਣਤਰ irq_work *irq_work)
{
	ਬਣਤਰ rq *this_rq = this_rq();
	ਬਣਤਰ scx_rq *this_scx = &this_rq->scx;
	ਬਣਤਰ scx_kick_syncs __rcu *ksyncs_pcpu = __this_cpu_read(scx_kick_syncs);
	ਬਣਤਰ scx_sched_pcpu *pcpu, *tmp;
	bool should_wait = false;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ksyncs;
	s32 cpu;

	/* can race with free_kick_syncs() during scheduler disable */
	ਜੇ (unlikely(!ksyncs_pcpu))
		ਵਾਪਸ;

	ksyncs = rcu_dereference_bh(ksyncs_pcpu)->syncs;

	/*
	 * Walk scheds with pending kicks on this cpu. scx_kick_cpu() adds to
	 * the list under local_irq_save() and only this irq_work consumes it.
	 * A plain list without locking is sufficient.
	 */
	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(pcpu, tmp, &this_scx->sched_pcpus_ਮੰਜ਼ਿਲ_kick, ਮੰਜ਼ਿਲ_kick_ਗੰਢ) {
		list_del_init(&pcpu->ਮੰਜ਼ਿਲ_kick_ਗੰਢ);

		for_each_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_kick) {
			should_wait |= kick_one_cpu(cpu, pcpu, this_rq, ksyncs);
			cpumask_clear_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_kick);
			cpumask_clear_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle);
		}

		for_each_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle) {
			kick_one_cpu_if_idle(cpu, pcpu, this_rq);
			cpumask_clear_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle);
		}
	}

	/*
	 * Can't wait in hardirq — kick_sync can't advance, deadlocking if
	 * CPUs wait for each other. Defer to kick_sync_wait_bal_cb().
	 */
	ਜੇ (should_wait) {
		raw_spin_rq_ਤਾਲਾ(this_rq);
		this_scx->kick_sync_pending = true;
		resched_curr(this_rq);
		scx_rq_ਤਾਲਾ_drop(this_rq);
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
ਖਾਲੀ print_scx_info(ਅਚਲ ਅੱਖਰ *log_lvl, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ scx_sched *sch;
	ਗਿਣਤੀ scx_enable_state state = scx_enable_state();
	ਅਚਲ ਅੱਖਰ *all = READ_ONCE(scx_switching_all) ? "+all" : "";
	ਅੱਖਰ runnable_at_buf[22] = "?";
	ਬਣਤਰ sched_class *class;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ runnable_at;

	guard(rcu)();

	sch = scx_task_sched_rcu(p);

	ਜੇ (!sch)
		ਵਾਪਸ;

	/*
	 * Carefully check if the task was running on sched_ext, and then
	 * carefully copy the time it's been runnable, and its state.
	 */
	ਜੇ (copy_from_kernel_nofault(&class, &p->sched_class, ਆਕਾਰ(class)) ||
	    class != &ext_sched_class) {
		printk("%sSched_ext: %s (%s%s)", log_lvl, sch->ops.name,
		       scx_enable_state_str[state], all);
		ਵਾਪਸ;
	}

	ਜੇ (!copy_from_kernel_nofault(&runnable_at, &p->scx.runnable_at,
				      ਆਕਾਰ(runnable_at)))
		scnprintf(runnable_at_buf, ਆਕਾਰ(runnable_at_buf), "%+ldms",
			  jiffies_delta_msecs(runnable_at, jiffies));

	/* print everything onto one line to conserve console space */
	printk("%sSched_ext: %s (%s%s), task: runnable_at=%s",
	       log_lvl, sch->ops.name, scx_enable_state_str[state], all,
	       runnable_at_buf);
}

ਸਥਿਰ ਪੂਰਨ scx_pm_handler(ਬਣਤਰ notifier_ਖੰਡ *nb, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ event, ਖਾਲੀ *ptr)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ਜੜ੍ਹ);
	ਜੇ (!sch)
		ਵਾਪਸ NOTIFY_OK;

	/*
	 * SCX schedulers often have userspace components which are sometimes
	 * involved in critial scheduling paths. PM operations involve freezing
	 * userspace which can lead to scheduling misbehaviors including stalls.
	 * Let's bypass while PM operations are in progress.
	 */
	ਚੋਣ (event) {
	ਹਾਲਤ PM_HIBERNATION_PREPARE:
	ਹਾਲਤ PM_SUSPEND_PREPARE:
	ਹਾਲਤ PM_RESTORE_PREPARE:
		scx_bypass(sch, true);
		ਰੋਕੋ;
	ਹਾਲਤ PM_POST_HIBERNATION:
	ਹਾਲਤ PM_POST_SUSPEND:
	ਹਾਲਤ PM_POST_RESTORE:
		scx_bypass(sch, false);
		ਰੋਕੋ;
	}

	ਵਾਪਸ NOTIFY_OK;
}

ਸਥਿਰ ਬਣਤਰ notifier_ਖੰਡ scx_pm_notifier = {
	.notifier_call = scx_pm_handler,
};

ਖਾਲੀ __init init_sched_ext_class(ਖਾਲੀ)
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
		ਬਣਤਰ rq *rq = cpu_rq(cpu);
		ਪੂਰਨ  n = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);

		/* local_dsq's sch will be set during scx_root_enable() */
		BUG_ON(scx_init_dsq(&rq->scx.local_dsq, SCX_DSQ_LOCAL, ਨਿਰਾ));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
		BUG_ON(scx_init_dsq(&rq->scx.reject_dsq, SCX_DSQ_REJECT, ਨਿਰਾ));
		scx_rescue_init(rq);
#ਜੇ_ਅੰਤ

		INIT_LIST_ਸਿਰ(&rq->scx.runnable_list);
		INIT_LIST_ਸਿਰ(&rq->scx.ddsp_deferred_locals);

		BUG_ON(!zalloc_cpumask_var_ਗੰਢ(&rq->scx.cpus_ਮੰਜ਼ਿਲ_sync, GFP_KERNEL, n));
		INIT_LIST_ਸਿਰ(&rq->scx.sched_pcpus_ਮੰਜ਼ਿਲ_kick);
		raw_spin_ਤਾਲਾ_init(&rq->scx.deferred_reenq_ਤਾਲਾ);
		INIT_LIST_ਸਿਰ(&rq->scx.deferred_reenq_locals);
		INIT_LIST_ਸਿਰ(&rq->scx.deferred_reenq_users);
		rq->scx.deferred_irq_work = IRQ_WORK_INIT_HARD(deferred_irq_workfn);
		rq->scx.kick_cpus_irq_work = IRQ_WORK_INIT_HARD(kick_cpus_irq_workfn);

		ਜੇ (cpu_online(cpu))
			cpu_rq(cpu)->scx.flags |= SCX_RQ_ONLINE;
	}

	ਰਜਿਸਟਰ_sysrq_ਕੁੰਜੀ('S', &sysrq_sched_ext_reset_op);
	ਰਜਿਸਟਰ_sysrq_ਕੁੰਜੀ('D', &sysrq_sched_ext_dump_op);
	INIT_DELAYED_WORK(&scx_watchdog_work, scx_watchdog_workfn);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	BUG_ON(rhashtable_init(&scx_sched_hash, &scx_sched_hash_params));
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_SUB_SCHED */
}


/********************************************************************************
 * Helpers that can be called from the BPF scheduler.
 */
ਸਥਿਰ bool scx_vet_enq_flags(ਬਣਤਰ scx_sched *sch, u64 dsq_id, u64 *enq_flags)
{
	bool is_local = dsq_id == SCX_DSQ_LOCAL ||
		(dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON;

	ਜੇ (unlikely(*enq_flags & __SCX_ENQ_INTERNAL_MASK)) {
		scx_error(sch, "invalid enq_flags 0x%llx", *enq_flags);
		ਵਾਪਸ false;
	}

	ਜੇ (*enq_flags & SCX_ENQ_IMMED) {
		ਜੇ (unlikely(!is_local)) {
			scx_error(sch, "SCX_ENQ_IMMED on a non-local DSQ 0x%llx", dsq_id);
			ਵਾਪਸ false;
		}
	} ਨਹੀਂਤਾਂ ਜੇ ((sch->ops.flags & SCX_OPS_ALWAYS_ENQ_IMMED) && is_local) {
		*enq_flags |= SCX_ENQ_IMMED;
	}

	ਜੇ (unlikely((*enq_flags & SCX_ENQ_RESCUE) && !is_local)) {
		scx_error(sch, "SCX_ENQ_RESCUE on a non-local DSQ 0x%llx", dsq_id);
		ਵਾਪਸ false;
	}

	ਵਾਪਸ true;
}

ਸਥਿਰ bool scx_dsq_ਪਾਓ_preamble(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p,
				    u64 dsq_id, u64 *enq_flags)
{
	lockdep_assert_irqs_disabled();

	ਜੇ (unlikely(!p)) {
		scx_error(sch, "called with NULL task");
		ਵਾਪਸ false;
	}

	/* see SCX_EV_INSERT_NOT_OWNED definition */
	ਜੇ (unlikely(!scx_task_on_sched(sch, p))) {
		__scx_add_event(sch, SCX_EV_ਪਾਓ_NOT_OWNED, 1);
		ਵਾਪਸ false;
	}

	ਜੇ (!scx_vet_enq_flags(sch, dsq_id, enq_flags))
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ scx_dsq_ਪਾਓ_commit(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p,
				  u64 dsq_id, u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	ਬਣਤਰ scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	ਬਣਤਰ task_struct *ddsp_task;

	ddsp_task = __this_cpu_read(ਸਿੱਧਾ_dispatch_task);
	ਜੇ (ddsp_task) {
		mark_ਸਿੱਧਾ_dispatch(sch, ddsp_task, p, dsq_id, ਟੁਕੜਾ, vtime, enq_flags);
		ਵਾਪਸ;
	}

	ਜੇ (unlikely(dspc->cursor >= sch->dsp_max_batch)) {
		scx_error(sch, "dispatch buffer overflow");
		ਵਾਪਸ;
	}

	dspc->buf[dspc->cursor++] = (ਬਣਤਰ scx_dsp_buf_ent){
		.task = p,
		.qseq = atomic_long_read(&p->scx.ops_state) & SCX_OPSS_QSEQ_ਨਕਾਬ,
		.dsq_id = dsq_id,
		.ਟੁਕੜਾ = ਟੁਕੜਾ,
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
__bpf_kfunc bool scx_bpf_dsq_ਪਾਓ_v2(ਬਣਤਰ task_struct *p, u64 dsq_id,
					 u64 ਟੁਕੜਾ, u64 enq_flags,
					 ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ false;

	ਜੇ (!scx_dsq_ਪਾਓ_preamble(sch, p, dsq_id, &enq_flags))
		ਵਾਪਸ false;

	scx_dsq_ਪਾਓ_commit(sch, p, dsq_id, ਟੁਕੜਾ, 0, enq_flags);

	ਵਾਪਸ true;
}

/*
 * COMPAT: Will be removed in v6.23 along with the ___v2 suffix.
 */
__bpf_kfunc ਖਾਲੀ scx_bpf_dsq_ਪਾਓ(ਬਣਤਰ task_struct *p, u64 dsq_id,
				    u64 ਟੁਕੜਾ, u64 enq_flags,
				    ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	scx_bpf_dsq_ਪਾਓ_v2(p, dsq_id, ਟੁਕੜਾ, enq_flags, aux);
}

ਸਥਿਰ bool scx_dsq_ਪਾਓ_vtime(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p,
				 u64 dsq_id, u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	ਜੇ (!scx_dsq_ਪਾਓ_preamble(sch, p, dsq_id, &enq_flags))
		ਵਾਪਸ false;

	scx_dsq_ਪਾਓ_commit(sch, p, dsq_id, ਟੁਕੜਾ, vtime, enq_flags | SCX_ENQ_DSQ_PRIQ);

	ਵਾਪਸ true;
}

ਬਣਤਰ scx_bpf_dsq_ਪਾਓ_vtime_ਦਲੀਲਾਂ {
	/* @p can't be packed together as KF_RCU is not transitive */
	u64			dsq_id;
	u64			ਟੁਕੜਾ;
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
__scx_bpf_dsq_insert_vtime(ਬਣਤਰ task_struct *p,
			   ਬਣਤਰ scx_bpf_dsq_ਪਾਓ_vtime_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ,
			   ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ false;

	ਵਾਪਸ scx_dsq_ਪਾਓ_vtime(sch, p, ਦਲੀਲਾਂ->dsq_id, ਦਲੀਲਾਂ->ਟੁਕੜਾ,
				    ਦਲੀਲਾਂ->vtime, ਦਲੀਲਾਂ->enq_flags);
}

/*
 * COMPAT: Will be removed in v6.23.
 */
__bpf_kfunc ਖਾਲੀ scx_bpf_dsq_ਪਾਓ_vtime(ਬਣਤਰ task_struct *p, u64 dsq_id,
					  u64 ਟੁਕੜਾ, u64 vtime, u64 enq_flags)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ਜੜ੍ਹ);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	/*
	 * Disallow if any sub-scheds are attached. There is no way to tell
	 * which scheduler called us, so error out @p's scheduler -- read it
	 * under RCU as @p's locks aren't necessarily held here. @p may be a
	 * task past sched_ext_dead() or an idle task, in which case its
	 * scheduler can't be determined and there is nothing obviously wrong
	 * to report; just refuse the call.
	 */
	ਜੇ (unlikely(!list_ਖਾਲੀ_ਥਾਵਾਂ(&sch->children))) {
		ਬਣਤਰ scx_sched *tsch = scx_task_sched_rcu(p);

		ਜੇ (tsch)
			scx_error(tsch, "__scx_bpf_dsq_insert_vtime() must be used");
		ਵਾਪਸ;
	}
#ਜੇ_ਅੰਤ

	scx_dsq_ਪਾਓ_vtime(sch, p, dsq_id, ਟੁਕੜਾ, vtime, enq_flags);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_dispatch)
BTF_ID_FLAGS(func, scx_bpf_dsq_ਪਾਓ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_ਪਾਓ_v2, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, __scx_bpf_dsq_insert_vtime, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_ਪਾਓ_vtime, KF_RCU)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_dispatch)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_dispatch = {
	.owner			= THIS_MODULE,
	.ਸੈੱਟ			= &scx_kfunc_ids_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_dispatch,
	.filter			= scx_kfunc_context_filter,
};

ਸਥਿਰ bool scx_dsq_move(ਬਣਤਰ bpf_iter_scx_dsq_kern *kit,
			 ਬਣਤਰ task_struct *p, u64 dsq_id, u64 enq_flags,
			 bool priq)
{
	ਬਣਤਰ scx_dispatch_q *src_dsq = kit->dsq, *dst_dsq;
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ rq *p_rq, *src_rq, *locked_rq;
	bool dispatched = false;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	/*
	 * The verifier considers an iterator slot initialized on any
	 * KF_ITER_NEW return, so a BPF program may legally reach here after
	 * bpf_iter_scx_dsq_new() failed and left @kit->dsq NULL.
	 */
	ਜੇ (unlikely(!src_dsq))
		ਵਾਪਸ false;

	sch = src_dsq->sched;

	ਜੇ (!scx_vet_enq_flags(sch, dsq_id, &enq_flags))
		ਵਾਪਸ false;

	/* internal bit, can only go in after @enq_flags is vetted */
	ਜੇ (priq)
		enq_flags |= SCX_ENQ_DSQ_PRIQ;

	/*
	 * If the BPF scheduler keeps calling this function repeatedly, it can
	 * cause similar live-lock conditions as scx_consume_dispatch_q().
	 */
	ਜੇ (unlikely(READ_ONCE(sch->aborting)))
		ਵਾਪਸ false;

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

	ਜੇ (locked_rq) {
		ਜੇ (locked_rq != src_rq)
			switch_rq_ਤਾਲਾ(locked_rq, src_rq);
	} ਨਹੀਂਤਾਂ {
		raw_spin_rq_ਤਾਲਾ(src_rq);
	}

	p_rq = src_rq;
	raw_spin_ਤਾਲਾ(&src_dsq->ਤਾਲਾ);

	/* did someone else get to it while we dropped the locks? */
	ਜੇ (nldsq_cursor_ਗੁਆਚਾ_task(&kit->cursor, src_rq, src_dsq, p)) {
		raw_spin_unlock(&src_dsq->ਤਾਲਾ);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * @p has been on $src_dsq and can't move anymore. If @p is not on @sch,
	 * the caller didn't have authority over @p at the time of the call.
	 */
	ਜੇ (unlikely(!scx_task_on_sched(sch, p))) {
		scx_error(sch, "scx_bpf_dsq_move[_vtime]() on %s[%d] but the task belongs to a different scheduler",
			  p->comm, p->pid);
		raw_spin_unlock(&src_dsq->ਤਾਲਾ);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* @p is still on $src_dsq and stable, determine the destination */
	dst_dsq = ਲੱਭੋ_dsq_for_dispatch(sch, locked_rq ?: this_rq(), dsq_id, task_cpu(p));

	/*
	 * Apply vtime and slice updates before moving. @p is still on $src_dsq
	 * with both $src_dsq and its task_rq locked, satisfying the write
	 * rules, and the PRIQ insertion into $dst_dsq reads the new vtime.
	 */
	ਜੇ (kit->cursor.flags & __SCX_DSQ_ITER_HAS_VTIME)
		p->scx.dsq_vtime = kit->vtime;
	ਜੇ (kit->cursor.flags & __SCX_DSQ_ITER_HAS_SLICE)
		scx_ਸੈੱਟ_task_ਟੁਕੜਾ(p, kit->ਟੁਕੜਾ);

	/* execute move */
	p_rq = move_task_between_dsqs(sch, p, enq_flags, src_dsq, dst_dsq);
	dispatched = true;
ਬਾਹਰਲਾ:
	ਜੇ (locked_rq) {
		ਜੇ (locked_rq != p_rq)
			switch_rq_ਤਾਲਾ(p_rq, locked_rq);
	} ਨਹੀਂਤਾਂ {
		scx_rq_ਤਾਲਾ_drop(p_rq);
		raw_spin_rq_unlock_irqrestore(p_rq, flags);
	}

	kit->cursor.flags &= ~(__SCX_DSQ_ITER_HAS_SLICE |
			       __SCX_DSQ_ITER_HAS_VTIME);
	ਵਾਪਸ dispatched;
}

__bpf_kfunc_start_defs();

/**
 * scx_bpf_dispatch_nr_slots - Return the number of remaining dispatch slots
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Can only be called from ops.dispatch().
 */
__bpf_kfunc u32 scx_bpf_dispatch_nr_slots(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ 0;

	ਵਾਪਸ sch->dsp_max_batch - __this_cpu_read(sch->pcpu->dsp_ctx.cursor);
}

/**
 * scx_bpf_dispatch_cancel - Cancel the latest dispatch
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Cancel the latest dispatch. Can be called multiple times to cancel further
 * dispatches. Can only be called from ops.dispatch().
 */
__bpf_kfunc ਖਾਲੀ scx_bpf_dispatch_cancel(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_dsp_ctx *dspc;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;

	dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;

	ਜੇ (dspc->cursor > 0)
		dspc->cursor--;
	ਨਹੀਂਤਾਂ
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
__bpf_kfunc bool scx_bpf_dsq_move_ਮੰਜ਼ਿਲ_local_v2(u64 dsq_id, u64 enq_flags,
						ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_dispatch_q *dsq;
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_dsp_ctx *dspc;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ false;

	ਜੇ (!scx_vet_enq_flags(sch, SCX_DSQ_LOCAL, &enq_flags))
		ਵਾਪਸ false;

	dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;

	scx_flush_dispatch_buf(sch, dspc->rq);

	dsq = ਲੱਭੋ_user_dsq(sch, dsq_id);
	ਜੇ (unlikely(!dsq)) {
		scx_error(sch, "invalid DSQ ID 0x%016llx", dsq_id);
		ਵਾਪਸ false;
	}

	ਜੇ (scx_consume_dispatch_q(sch, dspc->rq, dsq, enq_flags)) {
		/*
		 * A successfully consumed task can be dequeued before it starts
		 * running while the CPU is trying to migrate other dispatched
		 * tasks. Bump nr_tasks to tell dispatch_one() to retry on empty
		 * local DSQ.
		 */
		dspc->nr_tasks++;
		ਵਾਪਸ true;
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ false;
	}
}

/*
 * COMPAT: ___v2 was introduced in v7.1. Remove this and ___v2 tag in the future.
 */
__bpf_kfunc bool scx_bpf_dsq_move_ਮੰਜ਼ਿਲ_local(u64 dsq_id, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਵਾਪਸ scx_bpf_dsq_move_ਮੰਜ਼ਿਲ_local_v2(dsq_id, 0, aux);
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
__bpf_kfunc ਖਾਲੀ scx_bpf_dsq_move_ਸੈੱਟ_ਟੁਕੜਾ(ਬਣਤਰ bpf_iter_scx_dsq *it__iter,
					    u64 ਟੁਕੜਾ)
{
	ਬਣਤਰ bpf_iter_scx_dsq_kern *kit = (ਖਾਲੀ *)it__iter;

	kit->ਟੁਕੜਾ = ਟੁਕੜਾ;
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
__bpf_kfunc ਖਾਲੀ scx_bpf_dsq_move_ਸੈੱਟ_vtime(ਬਣਤਰ bpf_iter_scx_dsq *it__iter,
					    u64 vtime)
{
	ਬਣਤਰ bpf_iter_scx_dsq_kern *kit = (ਖਾਲੀ *)it__iter;

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
__bpf_kfunc bool scx_bpf_dsq_move(ਬਣਤਰ bpf_iter_scx_dsq *it__iter,
				  ਬਣਤਰ task_struct *p, u64 dsq_id,
				  u64 enq_flags)
{
	ਵਾਪਸ scx_dsq_move((ਬਣਤਰ bpf_iter_scx_dsq_kern *)it__iter,
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
__bpf_kfunc bool scx_bpf_dsq_move_vtime(ਬਣਤਰ bpf_iter_scx_dsq *it__iter,
					ਬਣਤਰ task_struct *p, u64 dsq_id,
					u64 enq_flags)
{
	ਵਾਪਸ scx_dsq_move((ਬਣਤਰ bpf_iter_scx_dsq_kern *)it__iter,
			    p, dsq_id, enq_flags, true);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_dispatch)
BTF_ID_FLAGS(func, scx_bpf_dispatch_nr_slots, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_dispatch_cancel, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ਮੰਜ਼ਿਲ_local, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ਮੰਜ਼ਿਲ_local_v2, KF_IMPLICIT_ਦਲੀਲਾਂ)
/* scx_bpf_dsq_move*() also in scx_kfunc_ids_unlocked: callable from unlocked contexts */
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ਸੈੱਟ_ਟੁਕੜਾ, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ਸੈੱਟ_vtime, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_vtime, KF_RCU)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
BTF_ID_FLAGS(func, scx_bpf_sub_dispatch, KF_IMPLICIT_ਦਲੀਲਾਂ)
#ਜੇ_ਅੰਤ
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_dispatch)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_dispatch = {
	.owner			= THIS_MODULE,
	.ਸੈੱਟ			= &scx_kfunc_ids_dispatch,
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
__bpf_kfunc u32 scx_bpf_reenqueue_local(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ rq *rq;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ 0;

	rq = cpu_rq(smp_processor_id());
	lockdep_assert_rq_held(rq);

	ਵਾਪਸ reenq_local(sch, rq, SCX_REENQ_ANY);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_cpu_release)
BTF_ID_FLAGS(func, scx_bpf_reenqueue_local, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_cpu_release)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_cpu_release = {
	.owner			= THIS_MODULE,
	.ਸੈੱਟ			= &scx_kfunc_ids_cpu_release,
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
__bpf_kfunc s32 scx_bpf_create_dsq(u64 dsq_id, s32 ਗੰਢ, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_dispatch_q *dsq;
	ਬਣਤਰ scx_sched *sch;
	s32 ret;

	ਜੇ (unlikely(ਗੰਢ >= (ਪੂਰਨ)nr_ਗੰਢ_ids ||
		     (ਗੰਢ < 0 && ਗੰਢ != NUMA_NO_ਗੰਢ)))
		ਵਾਪਸ -EINVAL;

	ਜੇ (unlikely(dsq_id & SCX_DSQ_FLAG_BUILTIN))
		ਵਾਪਸ -EINVAL;

	dsq = kmalloc_ਗੰਢ(ਆਕਾਰ(*dsq), GFP_KERNEL, ਗੰਢ);
	ਜੇ (!dsq)
		ਵਾਪਸ -ENOMEM;

	/*
	 * scx_init_dsq() must be called in GFP_KERNEL context. Init it with
	 * NULL @sch and update afterwards.
	 */
	ret = scx_init_dsq(dsq, dsq_id, ਨਿਰਾ);
	ਜੇ (ret) {
		kfree(dsq);
		ਵਾਪਸ ret;
	}

	rcu_read_ਤਾਲਾ();

	sch = scx_prog_sched(aux);
	ਜੇ (sch) {
		dsq->sched = sch;
		ret = rhashtable_lookup_ਪਾਓ_fast(&sch->dsq_hash, &dsq->hash_ਗੰਢ,
						    dsq_hash_params);
	} ਨਹੀਂਤਾਂ {
		ret = -ENODEV;
	}

	rcu_read_unlock();
	ਜੇ (ret) {
		exit_dsq(dsq);
		kfree(dsq);
	}
	ਵਾਪਸ ret;
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_unlocked)
BTF_ID_FLAGS(func, scx_bpf_create_dsq, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_SLEEPABLE)
/* also in scx_kfunc_ids_dispatch: also callable from ops.dispatch() */
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ਸੈੱਟ_ਟੁਕੜਾ, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_ਸੈੱਟ_vtime, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_vtime, KF_RCU)
/* also in scx_kfunc_ids_select_cpu: also callable from ops.select_cpu()/ops.enqueue() */
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_unlocked)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_unlocked = {
	.owner			= THIS_MODULE,
	.ਸੈੱਟ			= &scx_kfunc_ids_unlocked,
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
__bpf_kfunc bool scx_bpf_task_ਸੈੱਟ_ਟੁਕੜਾ(ਬਣਤਰ task_struct *p, u64 ਟੁਕੜਾ,
					ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ rq *locked_rq;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch || !scx_task_on_sched(sch, p)))
		ਵਾਪਸ false;

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
	ਜੇ (!locked_rq ||
	    (READ_ONCE(p->scx.runnable_cpu) != cpu_ਦਾ(locked_rq) &&
	     !task_current(locked_rq, p))) {
		ਸੈੱਟ_task_ਟੁਕੜਾ_oob(sch, p, ਟੁਕੜਾ);
		ਵਾਪਸ true;
	}

	/* under the rq lock: apply now, extensions gated on baseline access */
	ਜੇ (ਟੁਕੜਾ > p->scx.ਟੁਕੜਾ &&
	    unlikely(scx_missing_caps(sch, cpu_ਦਾ(locked_rq), SCX_CAP_ਅਧਾਰ))) {
		__scx_add_event(sch, SCX_EV_ਟੁਕੜਾ_DENIED, 1);
		ਵਾਪਸ true;
	}

	ਜੇ (unlikely(!scx_ਸੈੱਟ_task_ਟੁਕੜਾ(p, ਟੁਕੜਾ)))
		__scx_add_event(sch, SCX_EV_ਟੁਕੜਾ_DENIED, 1);

	ਵਾਪਸ true;
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
__bpf_kfunc bool scx_bpf_task_ਸੈੱਟ_dsq_vtime(ਬਣਤਰ task_struct *p, u64 vtime,
					    ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch || !scx_task_on_sched(sch, p)))
		ਵਾਪਸ false;

	p->scx.dsq_vtime = vtime;
	ਵਾਪਸ true;
}

ਖਾਲੀ scx_kick_cpu(ਬਣਤਰ scx_sched *sch, s32 cpu, u64 flags)
{
	ਬਣਤਰ scx_sched_pcpu *pcpu;
	ਬਣਤਰ rq *this_rq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ irq_flags;

	/*
	 * The per-cpu kick list is guarded only by local_irq_save(), which does
	 * not mask NMIs, so kicking from NMI could corrupt it and is unsupported.
	 */
	ਜੇ (unlikely(ਵਿੱਚ_nmi())) {
		scx_error(sch, "scx_bpf_kick_cpu() called from NMI");
		ਵਾਪਸ;
	}

	local_irq_save(irq_flags);

	this_rq = this_rq();
	pcpu = this_cpu_ptr(sch->pcpu);

	/*
	 * While bypassing for PM ops, IRQ handling may not be online which can
	 * lead to irq_work_queue() malfunction such as infinite busy wait for
	 * IRQ status update. Suppress kicking.
	 */
	ਜੇ (scx_bypassing(sch, cpu_ਦਾ(this_rq)))
		ਜਾਓ ਬਾਹਰਲਾ;

	/*
	 * Actual kicking is bounced to kick_cpus_irq_workfn() to avoid nesting
	 * rq locks. We can probably be smarter and avoid bouncing if called
	 * from ops which don't hold a rq lock.
	 *
	 * The kick masks are owned by @sch->pcpu, so that a preempt kick can be
	 * attributed to @sch.
	 */
	ਜੇ (flags & SCX_KICK_IDLE) {
		ਬਣਤਰ rq *ਨਿਸ਼ਾਨਾ_rq = cpu_rq(cpu);

		ਜੇ (unlikely(flags & (SCX_KICK_PREEMPT | SCX_KICK_WAIT)))
			scx_error(sch, "PREEMPT/WAIT cannot be used with SCX_KICK_IDLE");

		ਜੇ (raw_spin_rq_trylock(ਨਿਸ਼ਾਨਾ_rq)) {
			ਜੇ (can_skip_idle_kick(ਨਿਸ਼ਾਨਾ_rq)) {
				scx_rq_ਤਾਲਾ_drop(ਨਿਸ਼ਾਨਾ_rq);
				raw_spin_rq_unlock(ਨਿਸ਼ਾਨਾ_rq);
				ਜਾਓ ਬਾਹਰਲਾ;
			}
			scx_rq_ਤਾਲਾ_drop(ਨਿਸ਼ਾਨਾ_rq);
			raw_spin_rq_unlock(ਨਿਸ਼ਾਨਾ_rq);
		}
		cpumask_ਸੈੱਟ_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_kick_if_idle);
	} ਨਹੀਂਤਾਂ {
		cpumask_ਸੈੱਟ_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_kick);

		ਜੇ (flags & SCX_KICK_PREEMPT)
			cpumask_ਸੈੱਟ_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_preempt);
		ਜੇ (flags & SCX_KICK_WAIT)
			cpumask_ਸੈੱਟ_cpu(cpu, pcpu->cpus_ਮੰਜ਼ਿਲ_wait);
	}

	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&pcpu->ਮੰਜ਼ਿਲ_kick_ਗੰਢ))
		list_ਜੋੜੋ_ਪੂਛ(&pcpu->ਮੰਜ਼ਿਲ_kick_ਗੰਢ, &this_rq->scx.sched_pcpus_ਮੰਜ਼ਿਲ_kick);
	irq_work_ਕਤਾਰ(&this_rq->scx.kick_cpus_irq_work);
ਬਾਹਰਲਾ:
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
__bpf_kfunc ਖਾਲੀ scx_bpf_kick_cpu(s32 cpu, u64 flags, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ਜੇ (likely(sch) && scx_cpu_valid(sch, cpu, ਨਿਰਾ))
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
__bpf_kfunc ਖਾਲੀ scx_bpf_kick_cid(s32 cid, u64 flags, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;
	cpu = scx_cid_ਮੰਜ਼ਿਲ_cpu(sch, cid);
	ਜੇ (cpu < 0)
		ਵਾਪਸ;
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
__bpf_kfunc s32 scx_bpf_dsq_nr_queued(u64 dsq_id, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_dispatch_q *dsq;
	s32 ret;

	preempt_disable();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch)) {
		ret = -ENODEV;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (dsq_id == SCX_DSQ_LOCAL) {
		ret = READ_ONCE((scx_locked_rq() ?: this_rq())->scx.local_dsq.nr);
		ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ ((dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON) {
		s32 cpu = scx_cpu_ret(sch, dsq_id & SCX_DSQ_LOCAL_CPU_ਨਕਾਬ);

		ਜੇ (scx_cpu_valid(sch, cpu, ਨਿਰਾ)) {
			ret = READ_ONCE(cpu_rq(cpu)->scx.local_dsq.nr);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	} ਨਹੀਂਤਾਂ {
		dsq = ਲੱਭੋ_user_dsq(sch, dsq_id);
		ਜੇ (dsq) {
			ret = READ_ONCE(dsq->nr);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}
	ret = -ENOENT;
ਬਾਹਰਲਾ:
	preempt_enable();
	ਵਾਪਸ ret;
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
__bpf_kfunc ਖਾਲੀ scx_bpf_destroy_dsq(u64 dsq_id, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	ਜੇ (sch)
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
__bpf_kfunc ਪੂਰਨ bpf_iter_scx_dsq_new(ਬਣਤਰ bpf_iter_scx_dsq *it, u64 dsq_id,
				     u64 flags, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ bpf_iter_scx_dsq_kern *kit = (ਖਾਲੀ *)it;
	ਬਣਤਰ scx_sched *sch;

	BUILD_BUG_ON(ਆਕਾਰ(ਬਣਤਰ bpf_iter_scx_dsq_kern) >
		     ਆਕਾਰ(ਬਣਤਰ bpf_iter_scx_dsq));
	BUILD_BUG_ON(__alignof__(ਬਣਤਰ bpf_iter_scx_dsq_kern) !=
		     __alignof__(ਬਣਤਰ bpf_iter_scx_dsq));
	BUILD_BUG_ON(__SCX_DSQ_ITER_ALL_FLAGS &
		     ((1U << __SCX_DSQ_LNODE_PRIV_SHIFT) - 1));

	/*
	 * next() and destroy() will be called regardless of the return value.
	 * Always clear $kit->dsq.
	 */
	kit->dsq = ਨਿਰਾ;

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

	ਜੇ (flags & ~__SCX_DSQ_ITER_USER_FLAGS)
		ਵਾਪਸ -EINVAL;

	kit->dsq = ਲੱਭੋ_user_dsq(sch, dsq_id);
	ਜੇ (!kit->dsq)
		ਵਾਪਸ -ENOENT;

	kit->cursor = INIT_DSQ_LIST_CURSOR(kit->cursor, kit->dsq, flags);

	ਵਾਪਸ 0;
}

/**
 * bpf_iter_scx_dsq_next - Progress a DSQ iterator
 * @it: iterator to progress
 *
 * Return the next task. See bpf_iter_scx_dsq_new().
 */
__bpf_kfunc ਬਣਤਰ task_struct *bpf_iter_scx_dsq_ਅਗਲਾ(ਬਣਤਰ bpf_iter_scx_dsq *it)
{
	ਬਣਤਰ bpf_iter_scx_dsq_kern *kit = (ਖਾਲੀ *)it;

	ਜੇ (!kit->dsq)
		ਵਾਪਸ ਨਿਰਾ;

	guard(raw_spinlock_irqsave)(&kit->dsq->ਤਾਲਾ);

	ਵਾਪਸ nldsq_cursor_ਅਗਲਾ_task(&kit->cursor, kit->dsq);
}

/**
 * bpf_iter_scx_dsq_destroy - Destroy a DSQ iterator
 * @it: iterator to destroy
 *
 * Undo bpf_iter_scx_dsq_new().
 */
__bpf_kfunc ਖਾਲੀ bpf_iter_scx_dsq_destroy(ਬਣਤਰ bpf_iter_scx_dsq *it)
{
	ਬਣਤਰ bpf_iter_scx_dsq_kern *kit = (ਖਾਲੀ *)it;

	ਜੇ (!kit->dsq)
		ਵਾਪਸ;

	ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&kit->cursor.ਗੰਢ)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

		raw_spin_ਤਾਲਾ_irqsave(&kit->dsq->ਤਾਲਾ, flags);
		list_del_init(&kit->cursor.ਗੰਢ);
		raw_spin_unlock_irqrestore(&kit->dsq->ਤਾਲਾ, flags);
	}
	kit->dsq = ਨਿਰਾ;
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
__bpf_kfunc ਬਣਤਰ task_struct *scx_bpf_dsq_peek(u64 dsq_id,
						 ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_dispatch_q *dsq;

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (unlikely(dsq_id & SCX_DSQ_FLAG_BUILTIN)) {
		scx_error(sch, "peek disallowed on builtin DSQ 0x%llx", dsq_id);
		ਵਾਪਸ ਨਿਰਾ;
	}

	dsq = ਲੱਭੋ_user_dsq(sch, dsq_id);
	ਜੇ (unlikely(!dsq)) {
		scx_error(sch, "peek on non-existent DSQ 0x%llx", dsq_id);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ rcu_dereference(dsq->ਪਹਿਲਾ_task);
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
__bpf_kfunc ਖਾਲੀ scx_bpf_dsq_reenq(u64 dsq_id, u64 reenq_flags,
				   ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ rq *locked_rq = scx_locked_rq();
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_dispatch_q *dsq;

	guard(preempt)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;

	ਜੇ (unlikely(reenq_flags & ~__SCX_REENQ_USER_MASK)) {
		scx_error(sch, "invalid SCX_REENQ flags 0x%llx", reenq_flags);
		ਵਾਪਸ;
	}

	/* not specifying any filter bits is the same as %SCX_REENQ_ANY */
	ਜੇ (!(reenq_flags & __SCX_REENQ_FILTER_MASK))
		reenq_flags |= SCX_REENQ_ANY;

	dsq = ਲੱਭੋ_dsq_for_dispatch(sch, locked_rq ?: this_rq(), dsq_id, smp_processor_id());
	ਸਮਾਂ_ਸਾਰਣੀ_dsq_reenq(sch, dsq, reenq_flags, locked_rq);
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
__bpf_kfunc ਖਾਲੀ scx_bpf_reenqueue_local___v2(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	scx_bpf_dsq_reenq(SCX_DSQ_LOCAL, 0, aux);
}

__bpf_kfunc_end_defs();

__printf(5, 0)
ਸਥਿਰ s32 __bstr_format(ਬਣਤਰ scx_sched *sch, u64 *ਅੰਕੜੇ_buf, ਅੱਖਰ *ਸਤਰ_buf,
			 ਮਾਪ_t ਸਤਰ_ਮਾਪ, ਅੱਖਰ *fmt, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ *ਅੰਕੜੇ,
			 u32 ਅੰਕੜੇ_sz)
{
	ਬਣਤਰ bpf_bprintf_ਅੰਕੜੇ bprintf_ਅੰਕੜੇ = { .get_bin_ਦਲੀਲਾਂ = true };
	s32 ret;

	ਜੇ (ਅੰਕੜੇ_sz % 8 || ਅੰਕੜੇ_sz > MAX_BPRINTF_VARARGS * 8 ||
	    (ਅੰਕੜੇ_sz && !ਅੰਕੜੇ)) {
		scx_error(sch, "invalid data=%p and data__sz=%u", (ਖਾਲੀ *)ਅੰਕੜੇ, ਅੰਕੜੇ_sz);
		ਵਾਪਸ -EINVAL;
	}

	ret = copy_from_kernel_nofault(ਅੰਕੜੇ_buf, ਅੰਕੜੇ, ਅੰਕੜੇ_sz);
	ਜੇ (ret < 0) {
		scx_error(sch, "failed to read data fields (%d)", ret);
		ਵਾਪਸ ret;
	}

	ret = bpf_bprintf_prepare(fmt, UINT_MAX, ਅੰਕੜੇ_buf, ਅੰਕੜੇ_sz / 8,
				  &bprintf_ਅੰਕੜੇ);
	ਜੇ (ret < 0) {
		scx_error(sch, "format preparation failed (%d)", ret);
		ਵਾਪਸ ret;
	}

	ret = bstr_printf(ਸਤਰ_buf, ਸਤਰ_ਮਾਪ, fmt,
			  bprintf_ਅੰਕੜੇ.bin_ਦਲੀਲਾਂ);
	bpf_bprintf_cleanup(&bprintf_ਅੰਕੜੇ);
	ਜੇ (ret < 0) {
		scx_error(sch, "(\"%s\", %p, %u) failed to format", fmt, ਅੰਕੜੇ, ਅੰਕੜੇ_sz);
		ਵਾਪਸ ret;
	}

	ਵਾਪਸ ret;
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
bool scx_exit_bstr(ਬਣਤਰ scx_sched *sch, ਗਿਣਤੀ scx_exit_ਕਿਸਮ ਕਿਸਮ,
		   s64 exit_ਕੋਡ, ਬਣਤਰ scx_sched *fmt_blame, ਅੱਖਰ *fmt,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ *ਅੰਕੜੇ, u32 ਅੰਕੜੇ_sz)
{
	ਬਣਤਰ scx_exit_info *ei = sch->exit_info;
	u64 ਅੰਕੜੇ_buf[MAX_BPRINTF_VARARGS];
	s32 ret;

	guard(preempt)();

	ਜੇ (!scx_claim_exit(sch, ਕਿਸਮ))
		ਵਾਪਸ false;

	ret = __bstr_format(fmt_blame, ਅੰਕੜੇ_buf, ei->ਸੁਨੇਹਾ, SCX_EXIT_ਸੁਨੇਹਾ_LEN,
			    fmt, ਅੰਕੜੇ, ਅੰਕੜੇ_sz);
	ਜੇ (ret < 0)
		scnprintf(ei->ਸੁਨੇਹਾ, SCX_EXIT_ਸੁਨੇਹਾ_LEN,
			  "exit message formatting failed (%d)", ret);

	scx_ਅੰਤ_exit(sch, ਕਿਸਮ, exit_ਕੋਡ, raw_smp_processor_id());
	ਵਾਪਸ true;
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
__bpf_kfunc ਖਾਲੀ scx_bpf_exit_bstr(s64 exit_ਕੋਡ, ਅੱਖਰ *fmt,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ *ਅੰਕੜੇ, u32 ਅੰਕੜੇ_sz,
				   ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (likely(sch))
		scx_exit_bstr(sch, SCX_EXIT_UNREG_BPF, exit_ਕੋਡ, sch, fmt,
			      ਅੰਕੜੇ, ਅੰਕੜੇ_sz);
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
__bpf_kfunc ਖਾਲੀ scx_bpf_error_bstr(ਅੱਖਰ *fmt, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ *ਅੰਕੜੇ,
				    u32 ਅੰਕੜੇ_sz, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (likely(sch))
		scx_exit_bstr(sch, SCX_EXIT_ERROR_BPF, 0, sch, fmt, ਅੰਕੜੇ,
			      ਅੰਕੜੇ_sz);
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
__bpf_kfunc ਖਾਲੀ scx_bpf_dump_bstr(ਅੱਖਰ *fmt, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ *ਅੰਕੜੇ,
				   u32 ਅੰਕੜੇ_sz, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_dump_ਅੰਕੜੇ *dd = &scx_dump_ਅੰਕੜੇ;
	ਬਣਤਰ scx_bstr_buf *buf = &dd->buf;
	s32 ret;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;

	ਜੇ (raw_smp_processor_id() != dd->cpu) {
		scx_error(sch, "scx_bpf_dump() must only be called from ops.dump() and friends");
		ਵਾਪਸ;
	}

	/* append the formatted string to the line buf */
	ret = __bstr_format(sch, buf->ਅੰਕੜੇ, buf->ਸਤਰ + dd->cursor,
			    ਆਕਾਰ(buf->ਸਤਰ) - dd->cursor, fmt, ਅੰਕੜੇ, ਅੰਕੜੇ_sz);
	ਜੇ (ret < 0) {
		scx_dump_ਸਤਰ(dd->s, "%s[!] (\"%s\", %p, %u) failed to format (%d)",
			      dd->ਅਗੇਤਰ, fmt, ਅੰਕੜੇ, ਅੰਕੜੇ_sz, ret);
		ਵਾਪਸ;
	}

	dd->cursor += ret;
	dd->cursor = min_t(s32, dd->cursor, ਆਕਾਰ(buf->ਸਤਰ));

	ਜੇ (!dd->cursor)
		ਵਾਪਸ;

	/*
	 * If the line buf overflowed or ends in a newline, flush it into the
	 * dump. This is to allow the caller to generate a single line over
	 * multiple calls. As ops_dump_flush() can also handle multiple lines in
	 * the line buf, the only case which can lead to an unexpected
	 * truncation is when the caller keeps generating newlines in the middle
	 * instead of the end consecutively. Don't do that.
	 */
	ਜੇ (dd->cursor >= ਆਕਾਰ(buf->ਸਤਰ) || buf->ਸਤਰ[dd->cursor - 1] == '\n')
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
__bpf_kfunc u32 scx_bpf_cpuperf_cap(s32 cpu, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (likely(sch) && scx_cpu_valid(sch, cpu, ਨਿਰਾ))
		ਵਾਪਸ arch_scale_cpu_ਸਮਰੱਥਾ(cpu);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ SCX_CPUPERF_ONE;
}

/**
 * scx_bpf_cidperf_cap - Query the maximum relative capacity of the CPU at @cid
 * @cid: cid of the CPU to query
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpuperf_cap().
 */
__bpf_kfunc u32 scx_bpf_cidperf_cap(s32 cid, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ SCX_CPUPERF_ONE;
	cpu = scx_cid_ਮੰਜ਼ਿਲ_cpu(sch, cid);
	ਜੇ (cpu < 0)
		ਵਾਪਸ SCX_CPUPERF_ONE;
	ਵਾਪਸ arch_scale_cpu_ਸਮਰੱਥਾ(cpu);
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
__bpf_kfunc u32 scx_bpf_cpuperf_cur(s32 cpu, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (likely(sch) && scx_cpu_valid(sch, cpu, ਨਿਰਾ))
		ਵਾਪਸ arch_scale_freq_ਸਮਰੱਥਾ(cpu);
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ SCX_CPUPERF_ONE;
}

/**
 * scx_bpf_cidperf_cur - Query the current performance of the CPU at @cid
 * @cid: cid of the CPU to query
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpuperf_cur().
 */
__bpf_kfunc u32 scx_bpf_cidperf_cur(s32 cid, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ SCX_CPUPERF_ONE;
	cpu = scx_cid_ਮੰਜ਼ਿਲ_cpu(sch, cid);
	ਜੇ (cpu < 0)
		ਵਾਪਸ SCX_CPUPERF_ONE;
	ਵਾਪਸ arch_scale_freq_ਸਮਰੱਥਾ(cpu);
}

/* validate and apply a cpuperf target, see scx_bpf_cpuperf_set() */
ਸਥਿਰ s32 scx_cpuperf_ਸੈੱਟ(ਬਣਤਰ scx_sched *sch, s32 cpu, u32 perf)
{
	ਬਣਤਰ rq *rq, *locked_rq;
	ਬਣਤਰ rq_flags rf;
	s32 ret;

	ਜੇ (unlikely(perf > SCX_CPUPERF_ONE)) {
		scx_error(sch, "Invalid cpuperf target %u for CPU %d", perf, cpu);
		ਵਾਪਸ -EINVAL;
	}

	ਜੇ (!scx_cpu_valid(sch, cpu, ਨਿਰਾ))
		ਵਾਪਸ -EINVAL;

	rq = cpu_rq(cpu);
	locked_rq = scx_locked_rq();

	/*
	 * When called with an rq lock held, restrict the operation to the
	 * corresponding CPU to prevent ABBA deadlocks.
	 */
	ਜੇ (locked_rq && rq != locked_rq) {
		scx_error(sch, "Invalid target CPU %d", cpu);
		ਵਾਪਸ -EINVAL;
	}

	/*
	 * If no rq lock is held, allow to operate on any CPU by acquiring
	 * the corresponding rq lock.
	 */
	ਜੇ (!locked_rq) {
		rq_ਤਾਲਾ_irqsave(rq, &rf);
		update_rq_clock(rq);
	}

	/*
	 * ecaps updates are folded under the rq lock, making this test
	 * authoritative: a write can never land after a revoke has taken
	 * effect on @cpu.
	 */
	ਜੇ (likely(!scx_missing_caps(sch, cpu, SCX_CAP_PERF))) {
		rq->scx.cpuperf_ਨਿਸ਼ਾਨਾ = perf;
		cpufreq_update_util(rq, 0);
		ret = 0;
	} ਨਹੀਂਤਾਂ {
		__scx_add_event(sch, SCX_EV_SUB_CIDPERF_DENIED, 1);
		ret = -EACCES;
	}

	ਜੇ (!locked_rq)
		rq_unlock_irqrestore(rq, &rf);

	ਵਾਪਸ ret;
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
__bpf_kfunc ਖਾਲੀ scx_bpf_cpuperf_ਸੈੱਟ(s32 cpu, u32 perf, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;

	scx_cpuperf_ਸੈੱਟ(sch, cpu, perf);
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
__bpf_kfunc s32 scx_bpf_cidperf_ਸੈੱਟ(s32 cid, u32 perf,
				    ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;
	cpu = scx_cid_ਮੰਜ਼ਿਲ_cpu(sch, cid);
	ਜੇ (cpu < 0)
		ਵਾਪਸ cpu;

	ਵਾਪਸ scx_cpuperf_ਸੈੱਟ(sch, cpu, perf);
}

/**
 * scx_bpf_nr_node_ids - Return the number of possible node IDs
 *
 * All valid node IDs in the system are smaller than the returned value.
 */
__bpf_kfunc u32 scx_bpf_nr_ਗੰਢ_ids(ਖਾਲੀ)
{
	ਵਾਪਸ nr_ਗੰਢ_ids;
}

/**
 * scx_bpf_nr_cpu_ids - Return the number of possible CPU IDs
 *
 * All valid CPU IDs in the system are smaller than the returned value.
 */
__bpf_kfunc u32 scx_bpf_nr_cpu_ids(ਖਾਲੀ)
{
	ਵਾਪਸ nr_cpu_ids;
}

/**
 * scx_bpf_nr_cids - Return the size of the cid space
 *
 * Equals num_possible_cpus(). All valid cids are in [0, return value).
 */
__bpf_kfunc u32 scx_bpf_nr_cids(ਖਾਲੀ)
{
	ਵਾਪਸ ਅੰਕ_possible_cpus();
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
__bpf_kfunc u32 scx_bpf_nr_online_cids(ਖਾਲੀ)
{
	ਵਾਪਸ ਅੰਕ_online_cpus();
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
__bpf_kfunc ਅਚਲ ਖਾਲੀ *scx_bpf_online_cmask(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_cmask *online;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ ਨਿਰਾ;
	online = sch->online_cmask;
	ਜੇ (unlikely(!online))
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ (ਖਾਲੀ *)scx_kaddr_ਮੰਜ਼ਿਲ_arena(sch, online);
}

/**
 * scx_bpf_this_cid - Return the cid of the CPU this program is running on
 *
 * cid-addressed equivalent of bpf_get_smp_processor_id() for scx programs.
 * The current cpu is trivially valid, so this is just a table lookup. Return
 * -EINVAL if called before any scheduler has ever published its cid tables.
 */
__bpf_kfunc s32 scx_bpf_this_cid(ਖਾਲੀ)
{
	s16 *tbl;

	guard(rcu)();

	tbl = rcu_dereference(scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl);
	ਜੇ (!tbl)
		ਵਾਪਸ -EINVAL;
	ਵਾਪਸ tbl[raw_smp_processor_id()];
}

/**
 * scx_bpf_get_possible_cpumask - Get a referenced kptr to cpu_possible_mask
 */
__bpf_kfunc ਅਚਲ ਬਣਤਰ cpumask *scx_bpf_get_possible_cpumask(ਖਾਲੀ)
{
	ਵਾਪਸ cpu_possible_ਨਕਾਬ;
}

/**
 * scx_bpf_get_online_cpumask - Get a referenced kptr to cpu_online_mask
 */
__bpf_kfunc ਅਚਲ ਬਣਤਰ cpumask *scx_bpf_get_online_cpumask(ਖਾਲੀ)
{
	ਵਾਪਸ cpu_online_ਨਕਾਬ;
}

/**
 * scx_bpf_put_cpumask - Release a possible/online cpumask
 * @cpumask: cpumask to release
 */
__bpf_kfunc ਖਾਲੀ scx_bpf_put_cpumask(ਅਚਲ ਬਣਤਰ cpumask *cpumask)
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
__bpf_kfunc bool scx_bpf_task_running(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ task_rq(p)->curr == p;
}

/**
 * scx_bpf_task_cpu - CPU a task is currently associated with
 * @p: task of interest
 */
__bpf_kfunc s32 scx_bpf_task_cpu(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ task_cpu(p);
}

/**
 * scx_bpf_task_cid - cid a task is currently associated with
 * @p: task of interest
 *
 * cid-addressed equivalent of scx_bpf_task_cpu(). task_cpu(p) is always a
 * valid cpu, so this is just a table lookup. Return -EINVAL if called before
 * any scheduler has ever published its cid tables.
 */
__bpf_kfunc s32 scx_bpf_task_cid(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	s16 *tbl;

	/* KF_RCU covers only @p - a sleepable program holds no RCU lock */
	guard(rcu)();

	tbl = rcu_dereference(scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl);
	ਜੇ (!tbl)
		ਵਾਪਸ -EINVAL;
	ਵਾਪਸ tbl[task_cpu(p)];
}

/**
 * scx_bpf_locked_rq - Return the rq currently locked by SCX
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns the rq if a rq lock is currently held by SCX.
 * Otherwise emits an error and returns NULL.
 */
__bpf_kfunc ਬਣਤਰ rq *scx_bpf_locked_rq(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ rq *rq;

	guard(preempt)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ ਨਿਰਾ;

	rq = scx_locked_rq();
	ਜੇ (!rq) {
		scx_error(sch, "accessing rq without holding rq lock");
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ rq;
}

/**
 * scx_bpf_cpu_curr - Return remote CPU's curr task
 * @cpu: CPU of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Callers must hold RCU read lock (KF_RCU).
 */
__bpf_kfunc ਬਣਤਰ task_struct *scx_bpf_cpu_curr(s32 cpu, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (!scx_cpu_valid(sch, cpu, ਨਿਰਾ))
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ rcu_dereference(cpu_rq(cpu)->curr);
}

/**
 * scx_bpf_cid_curr - Return the curr task on the CPU at @cid
 * @cid: cid of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpu_curr(). Callers must hold RCU
 * read lock (KF_RCU).
 */
__bpf_kfunc ਬਣਤਰ task_struct *scx_bpf_cid_curr(s32 cid, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ ਨਿਰਾ;
	cpu = scx_cid_ਮੰਜ਼ਿਲ_cpu(sch, cid);
	ਜੇ (cpu < 0)
		ਵਾਪਸ ਨਿਰਾ;
	ਵਾਪਸ rcu_dereference(cpu_rq(cpu)->curr);
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
__bpf_kfunc ਬਣਤਰ task_struct *scx_bpf_tid_ਮੰਜ਼ਿਲ_task(u64 tid)
{
	ਬਣਤਰ sched_ext_entity *scx;

	ਜੇ (!scx_tid_ਮੰਜ਼ਿਲ_task_enabled()) {
		ਬਣਤਰ scx_sched *sch = rcu_dereference(scx_ਜੜ੍ਹ);

		ਜੇ (sch)
			scx_error(sch, "scx_bpf_tid_to_task() called without SCX_OPS_TID_TO_TASK");
		ਵਾਪਸ ਨਿਰਾ;
	}

	scx = rhashtable_lookup(&scx_tid_hash, &tid, scx_tid_hash_params);
	ਜੇ (!scx)
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ container_ਦਾ(scx, ਬਣਤਰ task_struct, scx);
}

u64 __scx_bpf_now(ਬਣਤਰ rq *rq)
{
	/* the caller must be on @rq's cpu or hold its lock */
	lockdep_assert((rq == this_rq() && !preemptible()) ||
		       lockdep_is_held(__rq_lockp(rq)));

	ਜੇ (smp_ਲੱਦੋ_acquire(&rq->scx.flags) & SCX_RQ_CLK_VALID) {
		/* if the rq clock is valid, use the cached rq clock */
		ਵਾਪਸ READ_ONCE(rq->scx.clock);
	} ਨਹੀਂਤਾਂ {
		/*
		 * Otherwise, return a fresh rq clock.
		 *
		 * The rq clock is updated outside of the rq lock.
		 * In this case, keep the updated rq clock invalid so the next
		 * read outside the rq lock gets a fresh rq clock.
		 */
		ਵਾਪਸ sched_clock_cpu(cpu_ਦਾ(rq));
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
__bpf_kfunc u64 scx_bpf_now(ਖਾਲੀ)
{
	/*
	 * Note that scx_bpf_now() is re-entrant between a process context and
	 * an interrupt context (e.g., timer interrupt). However, we don't need
	 * to consider the race between them because such race is not observable
	 * from a caller.
	 */
	guard(preempt)();
	ਵਾਪਸ __scx_bpf_now(this_rq());
}

ਸਥਿਰ ਖਾਲੀ scx_read_events(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ scx_event_stats *events)
{
	ਪੂਰਨ cpu;

	/* Aggregate per-CPU event counters into @events. */
	memset(events, 0, ਆਕਾਰ(*events));
	for_each_possible_cpu(cpu) {
		ਬਣਤਰ scx_event_stats *e_cpu = &ਪ੍ਰਤੀ_cpu_ptr(sch->pcpu, cpu)->event_stats;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCX_EVENT(name)	(events->name += READ_ONCE(e_cpu->name))
		SCX_EVENTS_LIST(SCX_EVENT);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCX_EVENT
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
__bpf_kfunc ਖਾਲੀ scx_bpf_events(ਬਣਤਰ scx_event_stats *events, ਮਾਪ_t events__sz,
				ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	ਬਣਤਰ scx_event_stats e_sys;

	rcu_read_ਤਾਲਾ();
	sch = scx_prog_sched(aux);
	ਜੇ (sch)
		scx_read_events(sch, &e_sys);
	ਨਹੀਂਤਾਂ
		memset(&e_sys, 0, ਆਕਾਰ(e_sys));
	rcu_read_unlock();

	/*
	 * We cannot entirely trust a BPF-provided size since a BPF program
	 * might be compiled against a different vmlinux.h, of which
	 * scx_event_stats would be larger (a newer vmlinux.h) or smaller
	 * (an older vmlinux.h). Hence, we use the smaller size to avoid
	 * memory corruption.
	 */
	events__sz = min(events__sz, ਆਕਾਰ(*events));
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(events, &e_sys, events__sz);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
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
__bpf_kfunc ਬਣਤਰ cgroup *scx_bpf_task_cgroup(ਬਣਤਰ task_struct *p,
					       ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = p->sched_task_ਸਮੂਹ;
	ਬਣਤਰ cgroup *cgrp = &cgrp_dfl_ਜੜ੍ਹ.cgrp;
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!scx_kf_arg_task_ok(sch, p))
		ਜਾਓ ਬਾਹਰਲਾ;

	cgrp = tg_cgrp(tg);

ਬਾਹਰਲਾ:
	cgroup_get(cgrp);
	ਵਾਪਸ cgrp;
}
#ਜੇ_ਅੰਤ	/* CONFIG_CGROUP_SCHED */

__bpf_kfunc_end_defs();

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_any)
BTF_ID_FLAGS(func, scx_bpf_task_ਸੈੱਟ_ਟੁਕੜਾ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU);
BTF_ID_FLAGS(func, scx_bpf_task_ਸੈੱਟ_dsq_vtime, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU);
BTF_ID_FLAGS(func, scx_bpf_kick_cpu, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_kick_cid, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_dsq_nr_queued, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_destroy_dsq, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_dsq_peek, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU_PROTECTED | KF_RET_NULL)
BTF_ID_FLAGS(func, scx_bpf_dsq_reenq, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_reenqueue_local___v2, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_new, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ITER_NEW | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_ਅਗਲਾ, KF_ITER_ਅਗਲਾ | KF_RET_NULL)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_destroy, KF_ITER_DESTROY)
BTF_ID_FLAGS(func, scx_bpf_exit_bstr, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_error_bstr, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_dump_bstr, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cap, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cur, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_ਸੈੱਟ, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cidperf_cap, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cidperf_cur, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cidperf_ਸੈੱਟ, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_nr_ਗੰਢ_ids)
BTF_ID_FLAGS(func, scx_bpf_nr_cpu_ids)
BTF_ID_FLAGS(func, scx_bpf_nr_cids)
BTF_ID_FLAGS(func, scx_bpf_nr_online_cids)
BTF_ID_FLAGS(func, scx_bpf_online_cmask, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ARENA_RET)
BTF_ID_FLAGS(func, scx_bpf_this_cid)
BTF_ID_FLAGS(func, scx_bpf_get_possible_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_online_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_task_running, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_task_cpu, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_task_cid, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_locked_rq, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RET_NULL)
BTF_ID_FLAGS(func, scx_bpf_cpu_curr, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_cid_curr, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_tid_ਮੰਜ਼ਿਲ_task, KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_now)
BTF_ID_FLAGS(func, scx_bpf_events, KF_IMPLICIT_ਦਲੀਲਾਂ)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
BTF_ID_FLAGS(func, scx_bpf_task_cgroup, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU | KF_ACQUIRE)
#ਜੇ_ਅੰਤ
BTF_ID_FLAGS(func, scx_bpf_sub_grant, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_sub_revoke, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_sub_caps, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_sub_kill_bstr, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_any)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_any = {
	.owner			= THIS_MODULE,
	.ਸੈੱਟ			= &scx_kfunc_ids_any,
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
BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_cpu_only)
BTF_ID_FLAGS(func, scx_bpf_kick_cpu, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_task_cpu, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_cpu_curr, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_cpu_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cap, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cur, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_ਸੈੱਟ, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_get_possible_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_online_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_idle_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_test_and_clear_cpu_idle, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_cpu_only)

/*
 * Per-op kfunc allow flags. Each bit corresponds to a context-sensitive kfunc
 * group; an op may permit zero or more groups, with the union expressed in
 * scx_kf_allow_flags[]. The verifier-time filter (scx_kfunc_context_filter())
 * consults this table to decide whether a context-sensitive kfunc is callable
 * from a given SCX op.
 */
ਗਿਣਤੀ scx_kf_allow_flags {
	SCX_KF_ALLOW_UNLOCKED		= 1 << 0,
	SCX_KF_ALLOW_INIT_CIDS		= 1 << 1,
	SCX_KF_ALLOW_CPU_RELEASE	= 1 << 2,
	SCX_KF_ALLOW_DISPATCH		= 1 << 3,
	SCX_KF_ALLOW_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ		= 1 << 4,
	SCX_KF_ALLOW_SELECT_CPU		= 1 << 5,
};

/*
 * Map each SCX op to the union of kfunc groups it permits, indexed by
 * SCX_OP_IDX(op). Ops not listed only permit kfuncs that are not
 * context-sensitive.
 */
ਸਥਿਰ ਅਚਲ u32 scx_kf_allow_flags[] = {
	[SCX_OP_IDX(select_cpu)]	= SCX_KF_ALLOW_SELECT_CPU | SCX_KF_ALLOW_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ,
	[SCX_OP_IDX(ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ)]		= SCX_KF_ALLOW_SELECT_CPU | SCX_KF_ALLOW_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ,
	[SCX_OP_IDX(dispatch)]		= SCX_KF_ALLOW_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ | SCX_KF_ALLOW_DISPATCH,
	[SCX_OP_IDX(cpu_release)]	= SCX_KF_ALLOW_CPU_RELEASE,
	[SCX_OP_IDX(init_task)]		= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(dump)]		= SCX_KF_ALLOW_UNLOCKED,
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	[SCX_OP_IDX(cgroup_init)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_exit)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_prep_move)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_cancel_move)] = SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_ਸੈੱਟ_ਭਾਰ)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_ਸੈੱਟ_bandwidth)] = SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_ਸੈੱਟ_idle)]	= SCX_KF_ALLOW_UNLOCKED,
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_GROUP_SCHED */
	[SCX_OP_IDX(sub_attach)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(sub_detach)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(sub_ecaps_updated)]	= SCX_KF_ALLOW_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ | SCX_KF_ALLOW_DISPATCH,
	[SCX_OP_IDX(cpu_online)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cpu_offline)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(init_cids)]		= SCX_KF_ALLOW_UNLOCKED | SCX_KF_ALLOW_INIT_CIDS,
	[SCX_OP_IDX(init)]		= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(ਬਾਹਰ_ਨਿਕਲੋ)]		= SCX_KF_ALLOW_UNLOCKED,
};

/*
 * Verifier-time filter for SCX kfuncs. Registered via the .filter field on
 * each per-group btf_kfunc_id_set. The BPF core invokes this for every kfunc
 * call in the registered hook (BPF_PROG_TYPE_STRUCT_OPS or
 * BPF_PROG_TYPE_SYSCALL), regardless of which set originally introduced the
 * kfunc - so the filter must short-circuit on kfuncs it doesn't govern by
 * falling through to "allow" when none of the SCX sets contain the kfunc.
 */
ਪੂਰਨ scx_kfunc_context_filter(ਅਚਲ ਬਣਤਰ bpf_prog *prog, u32 kfunc_id)
{
	bool ਵਿੱਚ_unlocked = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_unlocked, kfunc_id);
	bool ਵਿੱਚ_init_cids = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_init_cids, kfunc_id);
	bool ਵਿੱਚ_select_cpu = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_select_cpu, kfunc_id);
	bool ਵਿੱਚ_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_dispatch, kfunc_id);
	bool ਵਿੱਚ_dispatch = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_dispatch, kfunc_id);
	bool ਵਿੱਚ_cpu_release = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_cpu_release, kfunc_id);
	bool ਵਿੱਚ_idle = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_idle, kfunc_id);
	bool ਵਿੱਚ_any = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_any, kfunc_id);
	bool ਵਿੱਚ_cpu_only = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_cpu_only, kfunc_id);
	bool ਵਿੱਚ_cid = btf_id_set8_ਸ਼ਾਮਲ_ਹੈ(&scx_kfunc_ids_cid, kfunc_id);
	u32 moff, flags;

	/* Not an SCX kfunc - allow. */
	ਜੇ (!(ਵਿੱਚ_unlocked || ਵਿੱਚ_init_cids || ਵਿੱਚ_select_cpu || ਵਿੱਚ_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ || ਵਿੱਚ_dispatch ||
	      ਵਿੱਚ_cpu_release || ਵਿੱਚ_idle || ਵਿੱਚ_any || ਵਿੱਚ_cid))
		ਵਾਪਸ 0;

	/* SYSCALL progs (e.g. BPF test_run()) may call unlocked and select_cpu kfuncs. */
	ਜੇ (prog->type == BPF_PROG_TYPE_SYSCALL)
		ਵਾਪਸ (ਵਿੱਚ_unlocked || ਵਿੱਚ_select_cpu || ਵਿੱਚ_idle || ਵਿੱਚ_any || ਵਿੱਚ_cid) ? 0 : -EACCES;

	ਜੇ (prog->type != BPF_PROG_TYPE_STRUCT_OPS)
		ਵਾਪਸ (ਵਿੱਚ_any || ਵਿੱਚ_idle || ਵਿੱਚ_cid) ? 0 : -EACCES;

	/*
	 * add_subprog_and_kfunc() collects all kfunc calls, including dead code
	 * guarded by bpf_ksym_exists(), before check_attach_btf_id() sets
	 * prog->aux->st_ops. Allow all kfuncs when st_ops is not yet set;
	 * do_check_main() re-runs the filter with st_ops set and enforces the
	 * actual restrictions.
	 */
	ਜੇ (!prog->aux->st_ops)
		ਵਾਪਸ 0;

	/*
	 * Non-SCX struct_ops: SCX kfuncs are not permitted.
	 *
	 * Both bpf_sched_ext_ops (cpu-form) and bpf_sched_ext_ops_cid
	 * (cid-form) are valid SCX struct_ops. Member offsets match between
	 * the two (verified by BUILD_BUG_ON in scx_init()), so the shared
	 * scx_kf_allow_flags[] table indexed by SCX_MOFF_IDX(moff) applies to
	 * both.
	 */
	ਜੇ (prog->aux->st_ops != &bpf_sched_ext_ops &&
	    prog->aux->st_ops != &bpf_sched_ext_ops_cid)
		ਵਾਪਸ -EACCES;

	/*
	 * cid-form schedulers must use cid/cmask kfuncs. cid and cpu are both
	 * small s32s and trivially confused, so cpu-only kfuncs are rejected at
	 * load time. The reverse (cpu-form calling cid-form kfuncs) is
	 * intentionally permissive to ease gradual cpumask -> cid migration.
	 */
	ਜੇ (prog->aux->st_ops == &bpf_sched_ext_ops_cid && ਵਿੱਚ_cpu_only)
		ਵਾਪਸ -EACCES;

	/* SCX struct_ops: check the per-op allow list. */
	ਜੇ (ਵਿੱਚ_any || ਵਿੱਚ_idle || ਵਿੱਚ_cid)
		ਵਾਪਸ 0;

	moff = prog->aux->attach_st_ops_member_off;
	flags = scx_kf_allow_flags[SCX_MOFF_IDX(moff)];

	ਜੇ ((flags & SCX_KF_ALLOW_UNLOCKED) && ਵਿੱਚ_unlocked)
		ਵਾਪਸ 0;
	ਜੇ ((flags & SCX_KF_ALLOW_INIT_CIDS) && ਵਿੱਚ_init_cids)
		ਵਾਪਸ 0;
	ਜੇ ((flags & SCX_KF_ALLOW_CPU_RELEASE) && ਵਿੱਚ_cpu_release)
		ਵਾਪਸ 0;
	ਜੇ ((flags & SCX_KF_ALLOW_DISPATCH) && ਵਿੱਚ_dispatch)
		ਵਾਪਸ 0;
	ਜੇ ((flags & SCX_KF_ALLOW_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ) && ਵਿੱਚ_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ)
		ਵਾਪਸ 0;
	ਜੇ ((flags & SCX_KF_ALLOW_SELECT_CPU) && ਵਿੱਚ_select_cpu)
		ਵਾਪਸ 0;

	ਵਾਪਸ -EACCES;
}

ਸਥਿਰ ਪੂਰਨ __init scx_init(ਖਾਲੀ)
{
	ਪੂਰਨ ret;

	/*
	 * sched_ext_ops_cid mirrors sched_ext_ops up to and including @priv.
	 * Both bpf_scx_init_member() and bpf_scx_check_member() use offsets
	 * from struct sched_ext_ops; sched_ext_ops_cid relies on those offsets
	 * matching for the shared fields. Catch any drift at boot.
	 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CID_ਵਿੱਥ_ਮੇਲ(cpu_ਖੇਤਰ, cid_ਖੇਤਰ)					\
	BUILD_BUG_ON(offsetof(ਬਣਤਰ sched_ext_ops, cpu_ਖੇਤਰ) !=		\
		     offsetof(ਬਣਤਰ sched_ext_ops_cid, cid_ਖੇਤਰ))
	/* data fields used by bpf_scx_init_member() */
	CID_ਵਿੱਥ_ਮੇਲ(dispatch_max_batch, dispatch_max_batch);
	CID_ਵਿੱਥ_ਮੇਲ(flags, flags);
	CID_ਵਿੱਥ_ਮੇਲ(name, name);
	CID_ਵਿੱਥ_ਮੇਲ(timeout_ms, timeout_ms);
	CID_ਵਿੱਥ_ਮੇਲ(exit_dump_len, exit_dump_len);
	CID_ਵਿੱਥ_ਮੇਲ(hotplug_seq, hotplug_seq);
	CID_ਵਿੱਥ_ਮੇਲ(cid_shard_ਮਾਪ, cid_shard_ਮਾਪ);
	CID_ਵਿੱਥ_ਮੇਲ(rescue_bandwidth_ppt, rescue_bandwidth_ppt);
	CID_ਵਿੱਥ_ਮੇਲ(rescue_ਸਮਾਂ_ਹਿੱਸਾ_us, rescue_ਸਮਾਂ_ਹਿੱਸਾ_us);
	CID_ਵਿੱਥ_ਮੇਲ(sub_cgroup_id, sub_cgroup_id);
	/* shared callbacks: the union view requires byte-for-byte offset match */
	CID_ਵਿੱਥ_ਮੇਲ(ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ, ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ);
	CID_ਵਿੱਥ_ਮੇਲ(ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ);
	CID_ਵਿੱਥ_ਮੇਲ(dispatch, dispatch);
	CID_ਵਿੱਥ_ਮੇਲ(tick, tick);
	CID_ਵਿੱਥ_ਮੇਲ(runnable, runnable);
	CID_ਵਿੱਥ_ਮੇਲ(running, running);
	CID_ਵਿੱਥ_ਮੇਲ(stopping, stopping);
	CID_ਵਿੱਥ_ਮੇਲ(quiescent, quiescent);
	CID_ਵਿੱਥ_ਮੇਲ(yield, yield);
	CID_ਵਿੱਥ_ਮੇਲ(core_sched_before, core_sched_before);
	CID_ਵਿੱਥ_ਮੇਲ(ਸੈੱਟ_ਭਾਰ, ਸੈੱਟ_ਭਾਰ);
	CID_ਵਿੱਥ_ਮੇਲ(update_idle, update_idle);
	CID_ਵਿੱਥ_ਮੇਲ(init_task, init_task);
	CID_ਵਿੱਥ_ਮੇਲ(exit_task, exit_task);
	CID_ਵਿੱਥ_ਮੇਲ(enable, enable);
	CID_ਵਿੱਥ_ਮੇਲ(disable, disable);
	CID_ਵਿੱਥ_ਮੇਲ(dump, dump);
	CID_ਵਿੱਥ_ਮੇਲ(dump_task, dump_task);
	CID_ਵਿੱਥ_ਮੇਲ(sub_attach, sub_attach);
	CID_ਵਿੱਥ_ਮੇਲ(sub_detach, sub_detach);
	CID_ਵਿੱਥ_ਮੇਲ(sub_caps_updated, sub_caps_updated);
	CID_ਵਿੱਥ_ਮੇਲ(sub_ecaps_updated, sub_ecaps_updated);
	CID_ਵਿੱਥ_ਮੇਲ(init_cids, init_cids);
	CID_ਵਿੱਥ_ਮੇਲ(init, init);
	CID_ਵਿੱਥ_ਮੇਲ(ਬਾਹਰ_ਨਿਕਲੋ, ਬਾਹਰ_ਨਿਕਲੋ);
	/* renamed callbacks must occupy the same slot as their cpu-form sibling */
	CID_ਵਿੱਥ_ਮੇਲ(select_cpu, select_cid);
	CID_ਵਿੱਥ_ਮੇਲ(ਸੈੱਟ_cpumask, ਸੈੱਟ_cmask);
	CID_ਵਿੱਥ_ਮੇਲ(cpu_online, cid_online);
	CID_ਵਿੱਥ_ਮੇਲ(cpu_offline, cid_offline);
	CID_ਵਿੱਥ_ਮੇਲ(dump_cpu, dump_cid);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_init, cpuctl_init);
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_exit, cpuctl_exit);
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_prep_move, cpuctl_prep_move);
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_move, cpuctl_move);
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_cancel_move, cpuctl_cancel_move);
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_ਸੈੱਟ_ਭਾਰ, cpuctl_ਸੈੱਟ_ਭਾਰ);
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_ਸੈੱਟ_bandwidth, cpuctl_ਸੈੱਟ_bandwidth);
	CID_ਵਿੱਥ_ਮੇਲ(cgroup_ਸੈੱਟ_idle, cpuctl_ਸੈੱਟ_idle);
#ਜੇ_ਅੰਤ
	/* @priv tail must align since both share the same data block */
	CID_ਵਿੱਥ_ਮੇਲ(priv, priv);
	/*
	 * cid-form must end exactly at @priv - scx_validate_ops() skips
	 * cpu_acquire/cpu_release for cid-form because reading those fields
	 * past the BPF allocation would be UB.
	 */
	BUILD_BUG_ON(offsetof(ਬਣਤਰ sched_ext_ops_cid, __end) !=
		     offsetofend(ਬਣਤਰ sched_ext_ops, priv));
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CID_ਵਿੱਥ_ਮੇਲ

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
	ਜੇ ((ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ਸੈੱਟ_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_dispatch)) ||
	    (ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ਸੈੱਟ_dispatch)) ||
	    (ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ਸੈੱਟ_cpu_release)) ||
	    (ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ਸੈੱਟ_unlocked)) ||
	    (ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_SYSCALL,
					     &scx_kfunc_ਸੈੱਟ_unlocked)) ||
	    (ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_ਸੈੱਟ_any)) ||
	    (ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_TRACING,
					     &scx_kfunc_ਸੈੱਟ_any)) ||
	    (ret = ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_SYSCALL,
					     &scx_kfunc_ਸੈੱਟ_any))) {
		pr_err("sched_ext: Failed to register kfunc sets (%d)\n", ret);
		ਵਾਪਸ ret;
	}

	ret = scx_idle_init();
	ਜੇ (ret) {
		pr_err("sched_ext: Failed to initialize idle tracking (%d)\n", ret);
		ਵਾਪਸ ret;
	}

	ret = scx_cid_kfunc_init();
	ਜੇ (ret) {
		pr_err("sched_ext: Failed to register cid kfuncs (%d)\n", ret);
		ਵਾਪਸ ret;
	}

	ret = ਰਜਿਸਟਰ_bpf_struct_ops(&bpf_sched_ext_ops, sched_ext_ops);
	ਜੇ (ret) {
		pr_err("sched_ext: Failed to register struct_ops (%d)\n", ret);
		ਵਾਪਸ ret;
	}

	ret = ਰਜਿਸਟਰ_bpf_struct_ops(&bpf_sched_ext_ops_cid, sched_ext_ops_cid);
	ਜੇ (ret) {
		pr_err("sched_ext: Failed to register cid struct_ops (%d)\n", ret);
		ਵਾਪਸ ret;
	}

	ret = ਰਜਿਸਟਰ_pm_notifier(&scx_pm_notifier);
	ਜੇ (ret) {
		pr_err("sched_ext: Failed to register PM notifier (%d)\n", ret);
		ਵਾਪਸ ret;
	}

	scx_kset = kset_create_and_ਜੋੜੋ("sched_ext", &scx_uevent_ops, kernel_kobj);
	ਜੇ (!scx_kset) {
		pr_err("sched_ext: Failed to create /sys/kernel/sched_ext\n");
		ਵਾਪਸ -ENOMEM;
	}

	ret = sysfs_create_ਸਮੂਹ(&scx_kset->kobj, &scx_global_attr_ਸਮੂਹ);
	ਜੇ (ret < 0) {
		pr_err("sched_ext: Failed to add global attributes\n");
		ਵਾਪਸ ret;
	}

	ਵਾਪਸ 0;
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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
DEFINE_SCX_COMPAT_MARKER(cgroup_ਸੈੱਟ_bandwidth_may_sleep);
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_GROUP_SCHED */
