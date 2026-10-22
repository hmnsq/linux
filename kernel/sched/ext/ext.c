/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#شامل <linux/bitmap.h>
#شامل <linux/btf_ids.h>
#شامل <linux/rhashtable.h>
#شامل <linux/sched/clock.h>
#شامل <linux/sched/isolation.h>
#شامل <linux/suspend.h>
#شامل <linux/sysrq.h>

#شامل "../pelt.h"
#شامل "internal.h"
#شامل "cid.h"
#شامل "arena.h"
#شامل "idle.h"
#شامل "sub.h"
#شامل "inlines.h"

DEFINE_RAW_SPINLOCK(scx_sched_lock);

bool scx_allow_proxy_exec(ثابت ساختار task_struct *p)
{
	بازگشت true;
}

/*
 * NOTE: sched_ext is in the process of growing multiple scheduler support and
 * scx_root usage is in a transitional state. Naked dereferences are safe if the
 * caller is one of the tasks attached to SCX and explicit RCU dereference is
 * necessary otherwise. Naked scx_root dereferences trigger sparse warnings but
 * are used as temporary markers to indicate that the dereferences need to be
 * updated to point to the associated scheduler instances rather than scx_root.
 */
ساختار scx_sched __rcu *scx_ریشه;

/*
 * All scheds, writers must hold both scx_enable_mutex and scx_sched_lock.
 * Readers can hold either or rcu_read_lock().
 */
LIST_HEAD(scx_sched_all);

#ifdef CONFIG_EXT_SUB_SCHED
ثابت ساختار rhashtable_params scx_sched_hash_params = {
	.کلید_len		= sizeof_field(ساختار scx_sched, ops.sub_cgroup_id),
	.کلید_offset		= offsetof(ساختار scx_sched, ops.sub_cgroup_id),
	.head_offset		= offsetof(ساختار scx_sched, hash_گره),
	.insecure_elasticity	= true,	/* inserted under scx_sched_lock */
};

ساختار rhashtable scx_sched_hash;
#endif

/* see SCX_OPS_TID_TO_TASK */
ایستا ثابت ساختار rhashtable_params scx_tid_hash_params = {
	.کلید_len		= sizeof_field(ساختار sched_ext_entity, tid),
	.کلید_offset		= offsetof(ساختار sched_ext_entity, tid),
	.head_offset		= offsetof(ساختار sched_ext_entity, tid_hash_گره),
	.insecure_elasticity	= true,	/* inserted/removed under scx_tasks_lock */
};
ایستا ساختار rhashtable scx_tid_hash;

/*
 * During exit, a task may schedule after losing its PIDs. When disabling the
 * BPF scheduler, we need to be able to iterate tasks in every state to
 * guarantee system safety. Maintain a dedicated task list which contains every
 * task between its fork and eventual free.
 */
ایستا DEFINE_RAW_SPINLOCK(scx_tasks_lock);
ایستا LIST_HEAD(scx_tasks);

/* ops enable/disable */
DEFINE_MUTEX(scx_enable_mutex);
DEFINE_STATIC_کلید_FALSE(__scx_enabled);
DEFINE_PERCPU_RWSEM(scx_fork_rwsem);
ایستا atomic_t scx_enable_state_var = ATOMIC_INIT(SCX_DISABLED);
ایستا DEFINE_RAW_SPINLOCK(scx_bypass_lock);
ایستا bool scx_init_task_enabled;
ایستا bool scx_switching_all;
DEFINE_STATIC_کلید_FALSE(__scx_switched_all);
ایستا DEFINE_STATIC_کلید_FALSE(__scx_tid_to_task_enabled);

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
ایستا درونخطی bool scx_tid_to_task_enabled(تهی)
{
	بازگشت static_branch_likely(&__scx_tid_to_task_enabled);
}

ایستا atomic_long_t scx_nr_rejected = ATOMIC_LONG_INIT(0);
ایستا atomic_long_t scx_hotplug_seq = ATOMIC_LONG_INIT(0);

/* Global cursor for the per-CPU tid allocator. Starts at 1; tid 0 is reserved. */
ایستا atomic64_t scx_tid_cursor = ATOMIC64_INIT(1);

/* is @dsq synchronized by the containing rq lock instead of dsq->lock? */
ایستا bool dsq_is_rq_owned(ساختار scx_dispatch_q *dsq)
{
	انتخاب (dsq->id) {
	حالت SCX_DSQ_LOCAL:
	حالت SCX_DSQ_REJECT:
	حالت SCX_DSQ_RESCUE:
		بازگشت true;
	پیشفرض:
		بازگشت false;
	}
}

/* Cursor for unique scx_sched instance ids. id 0 is reserved. */
ایستا atomic64_t scx_sched_id_cursor = ATOMIC64_INIT(0);

#ifdef CONFIG_EXT_SUB_SCHED
/*
 * The sub sched being enabled. Used by scx_disable_and_exit_task() to exit
 * tasks for the sub-sched being enabled. Use a global variable instead of a
 * per-task field as all enables are serialized.
 */
ساختار scx_sched *scx_enabling_sub_sched;
#وگرنه
#define scx_enabling_sub_sched	(ساختار scx_sched *)پوچ
#endif	/* CONFIG_EXT_SUB_SCHED */

/*
 * A monotonically increasing sequence number that is incremented every time a
 * scheduler is enabled. This can be used to check if any custom sched_ext
 * scheduler has ever been used in the system.
 */
ایستا atomic_long_t scx_enable_seq = ATOMIC_LONG_INIT(0);

/*
 * Watchdog interval. All scx_sched's share a single watchdog timer and the
 * interval is half of the shortest sch->watchdog_timeout.
 */
ایستا unsigned long scx_watchdog_interval;

/*
 * The last time the delayed work was run. This delayed work relies on
 * ksoftirqd being able to run to service timer interrupts, so it's possible
 * that this work itself could get wedged. To account for this, we check that
 * it's not stalled in the timer tick, and trigger an error if it is.
 */
ایستا unsigned long scx_watchdog_timestamp = INITIAL_JIFFIES;

ایستا ساختار delayed_work scx_watchdog_work;

/*
 * For %SCX_KICK_WAIT: Each CPU has a pointer to an array of kick_sync sequence
 * numbers. The arrays are allocated with kvzalloc() as size can exceed percpu
 * allocator limits on large machines. O(nr_cpu_ids^2) allocation, allocated
 * lazily when enabling and freed when disabling to avoid waste when sched_ext
 * isn't active.
 */
ساختار scx_kick_syncs {
	ساختار rcu_head		rcu;
	unsigned long		syncs[];
};

ایستا DEFINE_PER_CPU(ساختار scx_kick_syncs __rcu *, scx_kick_syncs);

/*
 * Per-CPU buffered allocator state for p->scx.tid. Each CPU pulls a chunk of
 * SCX_TID_CHUNK ids from scx_tid_cursor and hands them out locally without
 * further synchronization. See scx_alloc_tid().
 */
ساختار scx_tid_alloc {
	u64	next;
	u64	end;
};
ایستا DEFINE_PER_CPU(ساختار scx_tid_alloc, scx_tid_alloc);

/*
 * Direct dispatch marker.
 *
 * Non-NULL values are used for direct dispatch from enqueue path. A valid
 * pointer points to the task currently being enqueued. An ERR_PTR value is used
 * to indicate that direct dispatch has already happened.
 */
ایستا DEFINE_PER_CPU(ساختار task_struct *, direct_dispatch_task);

ایستا ثابت ساختار rhashtable_params dsq_hash_params = {
	.کلید_len		= sizeof_field(ساختار scx_dispatch_q, id),
	.کلید_offset		= offsetof(ساختار scx_dispatch_q, id),
	.head_offset		= offsetof(ساختار scx_dispatch_q, hash_گره),
};

ایستا LLIST_HEAD(dsqs_to_free);

/* ops debug dump */
ایستا DEFINE_RAW_SPINLOCK(scx_dump_lock);

ساختار scx_dump_data {
	s32			cpu;
	bool			first;
	s32			cursor;
	ساختار seq_buf		*s;
	ثابت نویسه		*prefix;
	ساختار scx_bstr_buf	buf;
};

ایستا ساختار scx_dump_data scx_dump_data = {
	.cpu			= -1,
};

/* /sys/kernel/sched_ext interface */
ایستا ساختار kset *scx_kset;

/*
 * Parameters that can be adjusted through /sys/module/sched_ext/parameters.
 * There usually is no reason to modify these as normal scheduler operation
 * shouldn't be affected by them. The knobs are primarily for debugging.
 */
ایستا unsigned صحیح scx_slice_bypass_us = SCX_SLICE_BYPASS / NSEC_PER_USEC;
ایستا unsigned صحیح scx_bypass_lb_intv_us = SCX_BYPASS_LB_DFL_INTV_US;

ایستا صحیح set_slice_us(ثابت نویسه *val, ثابت ساختار kernel_param *kp)
{
	بازگشت param_set_uint_minmax(val, kp, 100, 100 * USEC_PER_MSEC);
}

ایستا ثابت ساختار kernel_param_ops slice_us_param_ops = {
	.set = set_slice_us,
	.get = param_get_uint,
};

ایستا صحیح set_bypass_lb_intv_us(ثابت نویسه *val, ثابت ساختار kernel_param *kp)
{
	بازگشت param_set_uint_minmax(val, kp, 0, 10 * USEC_PER_SEC);
}

ایستا ثابت ساختار kernel_param_ops bypass_lb_intv_us_param_ops = {
	.set = set_bypass_lb_intv_us,
	.get = param_get_uint,
};

#undef MODULE_PARAM_PREFIX
#define MODULE_PARAM_PREFIX	"sched_ext."

module_param_cb(slice_bypass_us, &slice_us_param_ops, &scx_slice_bypass_us, 0600);
MODULE_PARM_DESC(slice_bypass_us, "bypass slice in microseconds, applied on [un]load (100us to 100ms)");
module_param_cb(bypass_lb_intv_us, &bypass_lb_intv_us_param_ops, &scx_bypass_lb_intv_us, 0600);
MODULE_PARM_DESC(bypass_lb_intv_us, "bypass load balance interval in microseconds (0 (disable) to 10s)");

#undef MODULE_PARAM_PREFIX

#define CREATE_TRACE_POINTS
#شامل <trace/events/sched_ext.h>

ایستا تهی run_deferred(ساختار rq *rq);
ایستا bool task_dead_and_done(ساختار task_struct *p);
ایستا تهی scx_disable(ساختار scx_sched *sch, شمارش scx_exit_kind kind);

__printf(5, 6) bool __scx_exit(ساختار scx_sched *sch,
			       شمارش scx_exit_kind kind, s64 exit_code,
			       s32 exit_cpu, ثابت نویسه *fmt, ...)
{
	va_list args;
	bool ret;

	va_start(args, fmt);
	ret = scx_vexit(sch, kind, exit_code, exit_cpu, fmt, args);
	va_end(args);

	بازگشت ret;
}

ایستا long jiffies_delta_msecs(unsigned long at, unsigned long now)
{
	اگر (time_after(at, now))
		بازگشت jiffies_to_msecs(at - now);
	وگرنه
		بازگشت -(long)jiffies_to_msecs(now - at);
}

ایستا bool u32_before(u32 a, u32 b)
{
	بازگشت (s32)(a - b) < 0;
}

/**
 * scx_is_descendant - Test whether sched is a descendant
 * @sch: sched to test
 * @ancestor: ancestor sched to test against
 *
 * Test whether @sch is a descendant of @ancestor.
 */
bool scx_is_descendant(ساختار scx_sched *sch, ساختار scx_sched *ancestor)
{
	اگر (sch->level < ancestor->level)
		بازگشت false;
	بازگشت sch->ancestors[ancestor->level] == ancestor;
}

ایستا ساختار scx_dispatch_q *find_global_dsq(ساختار scx_sched *sch, s32 cpu)
{
	بازگشت &sch->pnode[cpu_to_گره(cpu)]->global_dsq;
}

ایستا ساختار scx_dispatch_q *find_user_dsq(ساختار scx_sched *sch, u64 dsq_id)
{
	بازگشت rhashtable_lookup(&sch->dsq_hash, &dsq_id, dsq_hash_params);
}

ایستا ثابت ساختار sched_class *scx_setscheduler_class(ساختار task_struct *p)
{
	اگر (p->sched_class == &stop_sched_class)
		بازگشت &stop_sched_class;

	بازگشت __setscheduler_class(p->policy, p->prio);
}

ایستا ساختار scx_dispatch_q *bypass_enq_target_dsq(ساختار scx_sched *sch, s32 cpu)
{
#ifdef CONFIG_EXT_SUB_SCHED
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
	وقتی (scx_parent(sch) && scx_bypassing(sch, cpu))
		sch = scx_parent(sch);
#endif	/* CONFIG_EXT_SUB_SCHED */

	بازگشت scx_bypass_dsq(sch, cpu);
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
ایستا bool rq_is_open(ساختار rq *rq, u64 enq_flags)
{
	lockdep_assert_rq_held(rq);

	/*
	 * A higher-priority class task is either running or in the process of
	 * waking up on @rq.
	 */
	اگر (sched_class_above(rq->next_class, &ext_sched_class))
		بازگشت false;

	/*
	 * @rq is either in transition to or in idle and there is no
	 * higher-priority class task waking up on it.
	 */
	اگر (sched_class_above(&ext_sched_class, rq->next_class))
		بازگشت true;

	/*
	 * @rq is either picking, in transition to, or running an SCX task.
	 */

	/*
	 * If we're in the dispatch path holding rq lock, $curr may or may not
	 * be ready depending on whether the on-going dispatch decides to extend
	 * $curr's slice. We say yes here and resolve it at the end of dispatch.
	 * See dispatch_one().
	 */
	اگر (rq->scx.flags & SCX_RQ_IN_DISPATCH)
		بازگشت true;

	/*
	 * %SCX_ENQ_PREEMPT clears $curr's slice if on SCX and kicks dispatch,
	 * so allow it to avoid spuriously triggering reenq on a combined
	 * PREEMPT|IMMED insertion.
	 */
	اگر (enq_flags & SCX_ENQ_PREEMPT) {
		ساختار task_struct *curr = rq->curr;

		/*
		 * A protected slice refuses the preemption and the cpu stays
		 * occupied. See rq_owned_post_enq().
		 */
		بازگشت curr->sched_class != &ext_sched_class ||
			likely(!(curr->scx.flags & SCX_TASK_PROTECTED));
	}

	/*
	 * @rq is either in transition to or running an SCX task and can't go
	 * idle without another SCX dispatch cycle.
	 */
	بازگشت false;
}

/*
 * Track the rq currently locked.
 *
 * This allows kfuncs to safely operate on rq from any scx ops callback,
 * knowing which rq is already locked.
 */
DEFINE_PER_CPU(ساختار rq *, scx_locked_rq_state);

/*
 * Under core scheduling, a pick that releases the rq lock invalidates the
 * core-wide selection it is part of. Count the releases so that the core-sched
 * pick can tell whether one happened across dispatch.
 */
ایستا تهی scx_rq_lock_drop(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);
#ifdef CONFIG_SCHED_CORE
	اگر (sched_core_enabled(rq))
		rq->scx.lock_drop_seq++;
#endif
}

ایستا تهی switch_rq_lock(ساختار rq *from, ساختار rq *to)
{
	bool tracked = scx_locked_rq() == from;

	اگر (tracked)
		update_locked_rq(پوچ);
	scx_rq_lock_drop(from);
	raw_spin_rq_unlock(from);
	raw_spin_rq_lock(to);
	اگر (tracked)
		update_locked_rq(to);
}

/*
 * Flipped on enable per sch->is_cid_type. Declared in internal.h so
 * subsystem inlines can read it.
 */
DEFINE_STATIC_کلید_FALSE(__scx_is_cid_type);

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
ایستا ساختار scx_cmask *scx_fill_cmask_scratch(ساختار scx_sched *sch,
						ثابت ساختار cpumask *cpumask)
{
	ساختار scx_cmask *kern_va = *this_cpu_ptr(sch->set_cmask_scratch);
	ساختار scx_cmask_ref ref;

	scx_cmask_ref_init_kern(sch, kern_va, 0, رقم_possible_cpus(), &ref);
	scx_cmask_ref_from_cpumask(&ref, cpumask);
	بازگشت kern_va;
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
ایستا درونخطی تهی scx_call_op_set_cpumask(ساختار scx_sched *sch, ساختار rq *rq,
					   ساختار task_struct *task,
					   ثابت ساختار cpumask *cpumask)
{
	اگر (scx_is_cid_type())
		SCX_CALL_CID_OP_TASK(sch, set_cmask, rq, task,
				     scx_fill_cmask_scratch(sch, cpumask));
	وگرنه
		SCX_CALL_OP_TASK(sch, set_cpumask, rq, task, cpumask);
}

شمارش scx_dsq_iter_flags {
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
ایستا ساختار task_struct *nldsq_next_task(ساختار scx_dispatch_q *dsq,
					   ساختار task_struct *cur, bool rev)
{
	ساختار list_head *list_گره;
	ساختار scx_dsq_list_گره *dsq_lnode;

	lockdep_assert_held(&dsq->lock);

	اگر (cur)
		list_گره = &cur->scx.dsq_list.گره;
	وگرنه
		list_گره = &dsq->list;

	/* find the next task, need to skip BPF iteration cursors */
	انجامبده {
		اگر (rev)
			list_گره = list_گره->prev;
		وگرنه
			list_گره = list_گره->next;

		اگر (list_گره == &dsq->list)
			بازگشت پوچ;

		dsq_lnode = container_of(list_گره, ساختار scx_dsq_list_گره,
					 گره);
	} وقتی (dsq_lnode->flags & SCX_DSQ_LNODE_ITER_CURSOR);

	بازگشت container_of(dsq_lnode, ساختار task_struct, scx.dsq_list);
}

#define nldsq_for_each_task(p, dsq)						\
	برای ((p) = nldsq_next_task((dsq), پوچ, false); (p);			\
	     (p) = nldsq_next_task((dsq), (p), false))

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
ایستا ساختار task_struct *nldsq_cursor_next_task(ساختار scx_dsq_list_گره *cursor,
						  ساختار scx_dispatch_q *dsq)
{
	bool rev = cursor->flags & SCX_DSQ_ITER_REV;
	ساختار task_struct *p;

	lockdep_assert_held(&dsq->lock);
	BUG_ON(!(cursor->flags & SCX_DSQ_LNODE_ITER_CURSOR));

	اگر (list_empty(&cursor->گره))
		p = پوچ;
	وگرنه
		p = container_of(cursor, ساختار task_struct, scx.dsq_list);

	/* skip cursors and tasks that were queued after @cursor init */
	انجامبده {
		p = nldsq_next_task(dsq, p, rev);
	} وقتی (p && unlikely(u32_before(cursor->priv, p->scx.dsq_seq)));

	اگر (p) {
		اگر (rev)
			list_move_tail(&cursor->گره, &p->scx.dsq_list.گره);
		وگرنه
			list_move(&cursor->گره, &p->scx.dsq_list.گره);
	} وگرنه {
		list_del_init(&cursor->گره);
	}

	بازگشت p;
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
ایستا bool nldsq_cursor_lost_task(ساختار scx_dsq_list_گره *cursor,
				   ساختار rq *rq, ساختار scx_dispatch_q *dsq,
				   ساختار task_struct *p)
{
	lockdep_assert_rq_held(rq);
	lockdep_assert_held(&dsq->lock);

	/*
	 * @p could have already left $src_dsq, got re-enqueud, or be in the
	 * process of being consumed by someone else.
	 */
	اگر (unlikely(p->scx.dsq != dsq ||
		     u32_before(cursor->priv, p->scx.dsq_seq) ||
		     p->scx.holding_cpu >= 0))
		بازگشت true;

	/* if @p has stayed on @dsq, its rq couldn't have changed */
	اگر (WARN_ON_ONCE(rq != task_rq(p)))
		بازگشت true;

	بازگشت false;
}

/*
 * BPF DSQ iterator. Tasks in a non-local DSQ can be iterated in [reverse]
 * dispatch order. BPF-visible iterator is opaque and larger to allow future
 * changes without breaking backward compatibility. Can be used with
 * bpf_for_each(). See bpf_iter_scx_dsq_*().
 */
ساختار bpf_iter_scx_dsq_kern {
	ساختار scx_dsq_list_گره	cursor;
	ساختار scx_dispatch_q		*dsq;
	u64				slice;
	u64				vtime;
} __attribute__((aligned(8)));

ساختار bpf_iter_scx_dsq {
	u64				__opaque[6];
} __attribute__((aligned(8)));


u32 scx_get_task_state(ثابت ساختار task_struct *p)
{
	بازگشت p->scx.flags & SCX_TASK_STATE_MASK;
}

تهی scx_set_task_state(ساختار task_struct *p, u32 state)
{
	u32 prev_state = scx_get_task_state(p);
	bool warn = false;

	انتخاب (state) {
	حالت SCX_TASK_NONE:
		warn = prev_state == SCX_TASK_DEAD;
		بشکن;
	حالت SCX_TASK_INIT_BEGIN:
		warn = prev_state != SCX_TASK_NONE;
		بشکن;
	حالت SCX_TASK_INIT:
		warn = prev_state != SCX_TASK_INIT_BEGIN;
		p->scx.flags |= SCX_TASK_RESET_RUNNABLE_AT;
		بشکن;
	حالت SCX_TASK_READY:
		warn = !(prev_state == SCX_TASK_INIT ||
			 prev_state == SCX_TASK_ENABLED);
		بشکن;
	حالت SCX_TASK_ENABLED:
		warn = prev_state != SCX_TASK_READY;
		بشکن;
	حالت SCX_TASK_DEAD:
		warn = !(prev_state == SCX_TASK_NONE ||
			 prev_state == SCX_TASK_INIT_BEGIN);
		بشکن;
	پیشفرض:
		WARN_ONCE(1, "sched_ext: Invalid task state %d -> %d for %s[%d]",
			  prev_state, state, p->comm, p->pid);
		بازگشت;
	}

	WARN_ONCE(warn, "sched_ext: Invalid task state transition 0x%x -> 0x%x for %s[%d]",
		  prev_state, state, p->comm, p->pid);

	p->scx.flags &= ~SCX_TASK_STATE_MASK;
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
تهی scx_task_iter_start(ساختار scx_task_iter *iter, ساختار cgroup *cgrp)
{
	memset(iter, 0, اندازه(*iter));

#ifdef CONFIG_EXT_SUB_SCHED
	اگر (cgrp) {
		lockdep_assert_held(&cgroup_mutex);
		iter->cgrp = cgrp;
		iter->css_pos = css_next_descendant_pre(پوچ, &iter->cgrp->self);
		css_task_iter_start(iter->css_pos, CSS_TASK_ITER_WITH_DEAD,
				    &iter->css_iter);
		بازگشت;
	}
#endif
	raw_spin_lock_irq(&scx_tasks_lock);

	iter->cursor = (ساختار sched_ext_entity){ .flags = SCX_TASK_CURSOR };
	list_add(&iter->cursor.tasks_گره, &scx_tasks);
	iter->list_locked = true;
}

ایستا تهی __scx_task_iter_rq_unlock(ساختار scx_task_iter *iter)
{
	اگر (iter->locked_task) {
		__balance_callbacks(iter->rq, &iter->rf);
		task_rq_unlock(iter->rq, iter->locked_task, &iter->rf);
		iter->locked_task = پوچ;
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
تهی scx_task_iter_unlock(ساختار scx_task_iter *iter)
{
	__scx_task_iter_rq_unlock(iter);
	اگر (iter->list_locked) {
		iter->list_locked = false;
		raw_spin_unlock_irq(&scx_tasks_lock);
	}
}

ایستا تهی __scx_task_iter_maybe_relock(ساختار scx_task_iter *iter)
{
	اگر (!iter->list_locked) {
		raw_spin_lock_irq(&scx_tasks_lock);
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
ایستا تهی scx_task_iter_relock(ساختار scx_task_iter *iter,
				 ساختار task_struct *p)
{
	__scx_task_iter_maybe_relock(iter);
	اگر (p) {
		iter->rq = task_rq_lock(p, &iter->rf);
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
تهی scx_task_iter_stop(ساختار scx_task_iter *iter)
{
#ifdef CONFIG_EXT_SUB_SCHED
	اگر (iter->cgrp) {
		اگر (iter->css_pos)
			css_task_iter_end(&iter->css_iter);
		__scx_task_iter_rq_unlock(iter);
		بازگشت;
	}
#endif
	__scx_task_iter_maybe_relock(iter);
	list_del_init(&iter->cursor.tasks_گره);
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
ایستا ساختار task_struct *scx_task_iter_next(ساختار scx_task_iter *iter)
{
	ساختار list_head *cursor = &iter->cursor.tasks_گره;
	ساختار sched_ext_entity *pos;

	اگر (!(++iter->cnt % SCX_TASK_ITER_BATCH)) {
		scx_task_iter_unlock(iter);
		cond_resched();
	}

#ifdef CONFIG_EXT_SUB_SCHED
	اگر (iter->cgrp) {
		وقتی (iter->css_pos) {
			ساختار task_struct *p;

			p = css_task_iter_next(&iter->css_iter);
			اگر (p)
				بازگشت p;

			css_task_iter_end(&iter->css_iter);
			iter->css_pos = css_next_descendant_pre(iter->css_pos,
								&iter->cgrp->self);
			اگر (iter->css_pos)
				css_task_iter_start(iter->css_pos, CSS_TASK_ITER_WITH_DEAD,
						    &iter->css_iter);
		}
		بازگشت پوچ;
	}
#endif
	__scx_task_iter_maybe_relock(iter);

	list_for_each_entry(pos, cursor, tasks_گره) {
		اگر (&pos->tasks_گره == &scx_tasks)
			بازگشت پوچ;
		اگر (!(pos->flags & SCX_TASK_CURSOR)) {
			list_move(cursor, &pos->tasks_گره);
			بازگشت container_of(pos, ساختار task_struct, scx);
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
ساختار task_struct *scx_task_iter_next_locked(ساختار scx_task_iter *iter)
{
	ساختار task_struct *p;

	__scx_task_iter_rq_unlock(iter);

	وقتی ((p = scx_task_iter_next(iter))) {
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
		اگر (p->sched_class == &idle_sched_class)
			ادامه;

		iter->rq = task_rq_lock(p, &iter->rf);
		iter->locked_task = p;

		/*
		 * cgroup_task_dead() removes the dead tasks from cset->tasks
		 * after sched_ext_dead() and cgroup iteration may see tasks
		 * which already finished sched_ext_dead(). %SCX_TASK_DEAD is
		 * set by sched_ext_dead() under @p's rq lock. Test it to
		 * avoid visiting tasks which are already dead from SCX POV.
		 */
		اگر (scx_get_task_state(p) == SCX_TASK_DEAD) {
			__scx_task_iter_rq_unlock(iter);
			ادامه;
		}

		بازگشت p;
	}
	بازگشت پوچ;
}

/**
 * scx_dump_event - Dump an event 'kind' in 'events' to 's'
 * @s: output seq_buf
 * @events: event stats
 * @kind: a kind of event to dump
 */
#define scx_dump_event(s, events, kind) انجامبده {					\
	scx_dump_سطر(&(s), "%40s: %16lld", #kind, (events)->kind);		\
} وقتی (0)


ایستا تهی scx_read_events(ساختار scx_sched *sch,
			    ساختار scx_event_stats *events);

ایستا شمارش scx_enable_state scx_enable_state(تهی)
{
	بازگشت atomic_read(&scx_enable_state_var);
}

ایستا شمارش scx_enable_state scx_set_enable_state(شمارش scx_enable_state to)
{
	بازگشت atomic_xchg(&scx_enable_state_var, to);
}

ایستا bool scx_tryset_enable_state(شمارش scx_enable_state to,
				    شمارش scx_enable_state from)
{
	صحیح from_v = from;

	بازگشت atomic_try_cmpxchg(&scx_enable_state_var, &from_v, to);
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
ایستا تهی wait_ops_state(ساختار task_struct *p, unsigned long opss)
{
	انجامبده {
		cpu_relax();
	} وقتی (atomic_long_read_acquire(&p->scx.ops_state) == opss);
}

ایستا درونخطی bool __cpu_valid(s32 cpu)
{
	بازگشت likely(cpu >= 0 && cpu < nr_cpu_ids && cpu_possible(cpu));
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
bool scx_cpu_valid(ساختار scx_sched *sch, s32 cpu, ثابت نویسه *where)
{
	اگر (__cpu_valid(cpu)) {
		بازگشت true;
	} وگرنه {
		scx_error(sch, "invalid CPU %d%s%s", cpu, where ? " " : "", where ?: "");
		بازگشت false;
	}
}

ایستا تهی deferred_bal_cb_workfn(ساختار rq *rq)
{
	run_deferred(rq);
}

ایستا تهی deferred_irq_workfn(ساختار irq_work *irq_work)
{
	ساختار rq *rq = container_of(irq_work, ساختار rq, scx.deferred_irq_work);

	raw_spin_rq_lock(rq);
	run_deferred(rq);
	scx_rq_lock_drop(rq);
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
ایستا تهی schedule_deferred(ساختار rq *rq)
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
	irq_work_queue_on(&rq->scx.deferred_irq_work, cpu_of(rq));
}

/**
 * schedule_deferred_locked - Schedule execution of deferred actions on an rq
 * @rq: target rq
 *
 * Schedule execution of deferred actions on @rq. Equivalent to
 * schedule_deferred() but requires @rq to be locked and can be more efficient.
 */
ایستا تهی schedule_deferred_locked(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);

	/*
	 * If in the middle of waking up a task, task_woken_scx() will be called
	 * afterwards which will then run the deferred actions, no need to
	 * schedule anything.
	 */
	اگر (rq->scx.flags & SCX_RQ_IN_WAKEUP)
		بازگشت;

	/* Don't do anything if there already is a deferred operation. */
	اگر (rq->scx.flags & SCX_RQ_BAL_CB_PENDING)
		بازگشت;

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
	اگر (rq->scx.flags & SCX_RQ_IN_DISPATCH) {
		rq->scx.flags |= SCX_RQ_BAL_CB_PENDING;
		بازگشت;
	}

	/*
	 * No scheduler hooks available. Use the generic irq_work path. The
	 * above WAKEUP and DISPATCH paths should cover most of the cases and
	 * the time to IRQ re-enable shouldn't be long.
	 */
	schedule_deferred(rq);
}

تهی scx_proxy_reenqueue_retry(ساختار rq *rq, ساختار task_struct *next)
{
}

تهی schedule_dsq_reenq(ساختار scx_sched *sch, ساختار scx_dispatch_q *dsq,
			u64 reenq_flags, ساختار rq *locked_rq)
{
	ساختار rq *rq;

	/*
	 * Allowing reenqueues doesn't make sense while bypassing. This also
	 * blocks from new reenqueues to be scheduled on dead scheds.
	 */
	اگر (unlikely(READ_ONCE(sch->bypass_depth)))
		بازگشت;

	اگر (dsq->id == SCX_DSQ_LOCAL) {
		rq = container_of(dsq, ساختار rq, scx.local_dsq);

		/*
		 * A sub-sched lacking baseline access on the target cid has no
		 * business triggering IPIs. The lockless test is fine: slipping
		 * through right after a revoke is harmless and a wrong denial
		 * can't happen - if the caller has seen its ownership, so does
		 * this test.
		 */
		اگر (unlikely(scx_missing_caps(sch, cpu_of(rq), SCX_CAP_BASE))) {
			__scx_add_event(sch, SCX_EV_SUB_REENQ_DENIED, 1);
			بازگشت;
		}

		ساختار scx_sched_pcpu *sch_pcpu = per_cpu_ptr(sch->pcpu, cpu_of(rq));
		ساختار scx_deferred_reenq_local *drl = &sch_pcpu->deferred_reenq_local;

		/*
		 * Pairs with smp_mb() in process_deferred_reenq_locals() and
		 * guarantees that there is a reenq_local() afterwards.
		 */
		smp_mb();

		اگر (list_empty(&drl->گره) ||
		    (READ_ONCE(drl->flags) & reenq_flags) != reenq_flags) {

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_lock);

			اگر (list_empty(&drl->گره))
				list_move_tail(&drl->گره, &rq->scx.deferred_reenq_locals);
			WRITE_ONCE(drl->flags, drl->flags | reenq_flags);
		}
	} وگرنه اگر (!(dsq->id & SCX_DSQ_FLAG_BUILTIN)) {
		rq = this_rq();

		ساختار scx_dsq_pcpu *dsq_pcpu = per_cpu_ptr(dsq->pcpu, cpu_of(rq));
		ساختار scx_deferred_reenq_user *dru = &dsq_pcpu->deferred_reenq_user;

		/*
		 * Pairs with smp_mb() in process_deferred_reenq_users() and
		 * guarantees that there is a reenq_user() afterwards.
		 */
		smp_mb();

		اگر (list_empty(&dru->گره) ||
		    (READ_ONCE(dru->flags) & reenq_flags) != reenq_flags) {

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_lock);

			/* @dsq is being destroyed, see free_dsq_irq_workfn() */
			اگر (unlikely(READ_ONCE(dsq->id) == SCX_DSQ_INVALID))
				بازگشت;

			اگر (list_empty(&dru->گره))
				list_move_tail(&dru->گره, &rq->scx.deferred_reenq_users);
			WRITE_ONCE(dru->flags, dru->flags | reenq_flags);
		}
	} وگرنه {
		scx_error(sch, "DSQ 0x%llx not allowed for reenq", dsq->id);
		بازگشت;
	}

	اگر (rq == locked_rq)
		schedule_deferred_locked(rq);
	وگرنه
		schedule_deferred(rq);
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
شمارش scx_slice_oob_consts {
	SCX_SLICE_OOB_DUR_BITS	= 43,
	SCX_SLICE_OOB_ID_BITS	= 64 - SCX_SLICE_OOB_DUR_BITS - 1,

	SCX_SLICE_OOB_DUR_MASK	= (1LLU << SCX_SLICE_OOB_DUR_BITS) - 1,
	SCX_SLICE_OOB_ID_SHIFT	= SCX_SLICE_OOB_DUR_BITS,
	SCX_SLICE_OOB_ID_MASK	= (1LLU << SCX_SLICE_OOB_ID_BITS) - 1,
	SCX_SLICE_OOB_PENDING	= 1LLU << 63,
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
ایستا تهی clear_task_slice_oob(ساختار task_struct *p)
{
	اگر (unlikely(atomic64_read(&p->scx.slice_oob)))
		atomic64_set(&p->scx.slice_oob, 0);
}

/**
 * dsq_insert_head - FIFO head insertion honoring %SCX_TASK_PROTECTED
 * @dsq: DSQ to insert into
 * @p: task being inserted
 *
 * A HEAD insert should land behind any leading protected tasks. Return %true
 * indicates whether @p became the first entry.
 */
ایستا bool dsq_درج_head(ساختار scx_dispatch_q *dsq, ساختار task_struct *p)
{
	ساختار list_head *pos = &dsq->list;
	ساختار scx_dsq_list_گره *گره;

	/*
	 * Only rq-owned DSQs can hold protected tasks and the associated rq
	 * lock keeps their flags stable.
	 */
	اگر (!dsq_is_rq_owned(dsq)) {
		list_add(&p->scx.dsq_list.گره, &dsq->list);
		بازگشت true;
	}

	list_for_each_entry(گره, &dsq->list, گره) {
		ساختار task_struct *q;

		اگر (WARN_ON_ONCE(گره->flags & SCX_DSQ_LNODE_ITER_CURSOR))
			ادامه;

		q = container_of(گره, ساختار task_struct, scx.dsq_list);
		اگر (!(q->scx.flags & SCX_TASK_PROTECTED))
			بشکن;

		pos = &گره->گره;
	}

	list_add(&p->scx.dsq_list.گره, pos);

	بازگشت pos == &dsq->list;
}

/**
 * set_task_slice_keep_oob - Set @p's slice, leaving any pending oob request
 * @p: task of interest
 * @slice: slice to set
 *
 * While %SCX_TASK_PROTECTED is set, BPF schedulers may not modify the slice.
 * Refuse and return %false.
 */
ایستا bool set_task_slice_keep_oob(ساختار task_struct *p, u64 slice)
{
	lockdep_assert_rq_held(task_rq(p));

	اگر (unlikely(p->scx.flags & SCX_TASK_PROTECTED))
		بازگشت false;

	p->scx.slice = slice;
	بازگشت true;
}

/* set @p's slice, superseding any pending out-of-band request */
bool scx_set_task_slice(ساختار task_struct *p, u64 slice)
{
	اگر (!set_task_slice_keep_oob(p, slice))
		بازگشت false;
	clear_task_slice_oob(p);
	بازگشت true;
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
تهی scx_task_slice_ended(ساختار rq *rq, ساختار task_struct *p)
{
	lockdep_assert_rq_held(rq);

	p->scx.flags &= ~SCX_TASK_PROTECTED;
	اگر (unlikely(p == scx_rescuee(rq)))
		scx_rescue_end(rq);
}

/* request @p's slice to be set to @slice, see the write rules above */
ایستا تهی set_task_slice_oob(ساختار scx_sched *sch, ساختار task_struct *p, u64 slice)
{
	u64 dur;

	اگر (slice == SCX_SLICE_INF) {
		dur = SCX_SLICE_OOB_DUR_MASK;
	} وگرنه اگر (unlikely(slice >= SCX_SLICE_OOB_DUR_MASK)) {
		dur = SCX_SLICE_OOB_DUR_MASK - 1;
		scx_add_event(sch, SCX_EV_SLICE_CLAMPED, 1);
	} وگرنه {
		dur = slice;
	}

	atomic64_set(&p->scx.slice_oob, SCX_SLICE_OOB_PENDING |
		     ((sch->id & SCX_SLICE_OOB_ID_MASK) << SCX_SLICE_OOB_ID_SHIFT) | dur);
}

/*
 * Apply a pending out-of-band slice request under @rq's lock. A request whose
 * packed id no longer matches @p's current owner is dropped. An extension needs
 * baseline cpu access on @p's cid, shortening is always allowed, and a
 * protected slice refuses both. %SCX_EV_SLICE_DENIED counts the denials. See
 * the write rules above.
 */
ایستا تهی apply_task_slice_oob(ساختار rq *rq, ساختار task_struct *p)
{
	u64 oob, dur, slice;

	lockdep_assert_rq_held(rq);

	اگر (likely(!atomic64_read(&p->scx.slice_oob)))
		بازگشت;

	oob = atomic64_xchg(&p->scx.slice_oob, 0);
	اگر (unlikely(!oob))
		بازگشت;

	/* the issuing scheduler no longer owns @p, drop the request */
	اگر (unlikely(((oob >> SCX_SLICE_OOB_ID_SHIFT) & SCX_SLICE_OOB_ID_MASK) !=
		     (scx_task_sched(p)->id & SCX_SLICE_OOB_ID_MASK)))
		بازگشت;

	dur = oob & SCX_SLICE_OOB_DUR_MASK;
	slice = dur == SCX_SLICE_OOB_DUR_MASK ? SCX_SLICE_INF : dur;

	اگر (slice > p->scx.slice &&
	    unlikely(scx_missing_caps(scx_task_sched(p), cpu_of(rq), SCX_CAP_BASE))) {
		__scx_add_event(scx_task_sched(p), SCX_EV_SLICE_DENIED, 1);
		بازگشت;
	}

	اگر (unlikely(!set_task_slice_keep_oob(p, slice)))
		__scx_add_event(scx_task_sched(p), SCX_EV_SLICE_DENIED, 1);
}

/*
 * A dsq insert kfunc doesn't write slice or dsq_vtime. The verdict carries them
 * and they are committed here, at the insertion. A zero @slice keeps the
 * current value, floored at 1 so the task isn't treated as expired.
 */
ایستا تهی apply_slice_vtime(ساختار task_struct *p, u64 slice, u64 vtime, u64 enq_flags)
{
	اگر (slice) {
		p->scx.slice = slice;
		/*
		 * An explicit slice supersedes a pending oob request. A carried
		 * default refill is not an explicit request and must keep it.
		 */
		اگر (!(enq_flags & SCX_ENQ_SLICE_DFL))
			clear_task_slice_oob(p);
	} وگرنه اگر (!p->scx.slice) {
		p->scx.slice = 1;
	}

	اگر (enq_flags & SCX_ENQ_DSQ_PRIQ)
		p->scx.dsq_vtime = vtime;
}

ایستا تهی update_curr_scx(ساختار rq *rq)
{
	ساختار task_struct *curr = rq->curr;
	s64 delta_exec;

	/* apply even on 0 delta_exec, callers may still act on the slice */
	apply_task_slice_oob(rq, curr);

	delta_exec = update_curr_common(rq);
	اگر (unlikely(delta_exec <= 0))
		بازگشت;

	اگر (curr->scx.slice != SCX_SLICE_INF)
		curr->scx.slice -= min_t(u64, curr->scx.slice, delta_exec);

	اگر (unlikely(curr == scx_rescuee(rq)))
		scx_rescue_charge(rq, delta_exec);

	dl_server_update(&rq->ext_server, delta_exec);
}

ایستا bool scx_dsq_priq_less(ساختار rb_گره *گره_a,
			      ثابت ساختار rb_گره *گره_b)
{
	ثابت ساختار task_struct *a =
		container_of(گره_a, ساختار task_struct, scx.dsq_priq);
	ثابت ساختار task_struct *b =
		container_of(گره_b, ساختار task_struct, scx.dsq_priq);

	بازگشت time_before64(a->scx.dsq_vtime, b->scx.dsq_vtime);
}

ایستا تهی dsq_inc_nr(ساختار scx_dispatch_q *dsq, ساختار task_struct *p, u64 enq_flags)
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
	اگر (enq_flags & SCX_ENQ_IMMED) {
		اگر (unlikely(dsq->id != SCX_DSQ_LOCAL)) {
			WARN_ON_ONCE(!(enq_flags & SCX_ENQ_GDSQ_FALLBACK));
			بازگشت;
		}
		p->scx.flags |= SCX_TASK_IMMED;
	}

	اگر (p->scx.flags & SCX_TASK_IMMED) {
		ساختار rq *rq = container_of(dsq, ساختار rq, scx.local_dsq);

		اگر (WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL))
			بازگشت;

		rq->scx.nr_immed++;

		/*
		 * If @rq already had other tasks or the current task is not
		 * done yet, @p can't go on the CPU immediately. Re-enqueue.
		 */
		اگر (unlikely(dsq->nr > 1 || !rq_is_open(rq, enq_flags)))
			scx_schedule_reenq_local(rq, 0);
	}
}

ایستا تهی dsq_dec_nr(ساختار scx_dispatch_q *dsq, ساختار task_struct *p)
{
	/* see dsq_inc_nr() */
	WRITE_ONCE(dsq->nr, dsq->nr - 1);

	اگر (p->scx.flags & SCX_TASK_IMMED) {
		ساختار rq *rq = container_of(dsq, ساختار rq, scx.local_dsq);

		اگر (WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL) ||
		    WARN_ON_ONCE(rq->scx.nr_immed <= 0))
			بازگشت;

		rq->scx.nr_immed--;
	}
}

ایستا تهی refill_task_slice_dfl(ساختار scx_sched *sch, ساختار task_struct *p)
{
	/*
	 * A default refill is not an explicit request, so it must not drop a
	 * pending out-of-band one, which is applied when @p next runs.
	 */
	set_task_slice_keep_oob(p, READ_ONCE(sch->slice_dfl));
	__scx_add_event(sch, SCX_EV_REFILL_SLICE_DFL, 1);
}

/*
 * Return true if @p is moving due to an internal SCX migration, false
 * otherwise.
 */
ایستا درونخطی bool task_scx_migrating(ساختار task_struct *p)
{
	/*
	 * We only need to check sticky_cpu: it is set to the destination
	 * CPU in move_remote_task_to_local_dsq() before deactivate_task()
	 * and cleared when the task is enqueued on the destination, so it
	 * is only non-negative during an internal SCX migration.
	 */
	بازگشت p->scx.sticky_cpu >= 0;
}

/* Must be called under the lock serializing @p's custody transfers. */
ایستا bool task_leave_custody(ساختار task_struct *p)
{
	اگر (!(p->scx.flags & SCX_TASK_IN_CUSTODY) || task_scx_migrating(p))
		بازگشت false;

	p->scx.flags &= ~SCX_TASK_IN_CUSTODY;
	بازگشت true;
}

ایستا تهی rq_owned_post_enq(ساختار scx_sched *sch, ساختار rq *rq,
			      ساختار scx_dispatch_q *dsq, ساختار task_struct *p,
			      u64 enq_flags)
{
	اگر (task_leave_custody(p) && SCX_HAS_OP(sch, dequeue))
		SCX_CALL_OP_TASK(sch, dequeue, rq, p, 0);

	/*
	 * Only local inserts get the wakeup treatment below. Rejects kick the
	 * deferred reenq and rescue parks are paced by the rescue timer.
	 */
	اگر (unlikely(dsq->id != SCX_DSQ_LOCAL)) {
		اگر (dsq->id == SCX_DSQ_REJECT)
			schedule_deferred_locked(rq);
		بازگشت;
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
	اگر (sched_class_above(p->sched_class, rq->next_class))
		wakeup_preempt(rq, p, 0);

	/*
	 * If @rq is in dispatch, the CPU is already vacant and looking for the
	 * next task to run. No need to preempt or trigger resched after moving
	 * @p into its local DSQ.
	 * Note that the wakeup_preempt() above may have already triggered
	 * a resched if @rq->next_class was idle. It's harmless, since
	 * need_resched is cleared immediately after task pick.
	 */
	اگر (rq->scx.flags & SCX_RQ_IN_DISPATCH)
		بازگشت;

	اگر ((enq_flags & SCX_ENQ_PREEMPT) && p != rq->curr &&
	    rq->curr->sched_class == &ext_sched_class) {
		اگر (likely(scx_set_task_slice(rq->curr, 0)))
			resched_curr(rq);
		وگرنه
			__scx_add_event(sch, SCX_EV_SLICE_DENIED, 1);
	}
}

ایستا تهی scx_dispatch_enqueue(ساختار scx_sched *sch, ساختار rq *rq,
				 ساختار scx_dispatch_q *dsq, ساختار task_struct *p,
				 u64 slice, u64 vtime, u64 enq_flags)
{
	bool is_rq_owned = false;

	اگر (dsq->id == SCX_DSQ_LOCAL) {
		dsq = scx_resolve_local_dsq(sch, rq, p, &enq_flags);
		is_rq_owned = true;
	}

	WARN_ON_ONCE(p->scx.dsq || !list_empty(&p->scx.dsq_list.گره));
	WARN_ON_ONCE((p->scx.dsq_flags & SCX_TASK_DSQ_ON_PRIQ) ||
		     !RB_EMPTY_گره(&p->scx.dsq_priq));

	اگر (!is_rq_owned) {
		raw_spin_lock_nested(&dsq->lock,
			(enq_flags & SCX_ENQ_NESTED) ? SINGLE_DEPTH_NESTING : 0);

		اگر (unlikely(dsq->id == SCX_DSQ_INVALID)) {
			scx_error(sch, "attempting to dispatch to a destroyed dsq");
			/* fall back to the global dsq */
			raw_spin_unlock(&dsq->lock);
			dsq = find_global_dsq(sch, task_cpu(p));
			raw_spin_lock(&dsq->lock);
		}
	}

	اگر (unlikely((dsq->id & SCX_DSQ_FLAG_BUILTIN) &&
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
	اگر (enq_flags & SCX_ENQ_APPLY_SLICE)
		apply_slice_vtime(p, slice, vtime, enq_flags);

	اگر (enq_flags & SCX_ENQ_DSQ_PRIQ) {
		ساختار rb_گره *rbp;

		/*
		 * A PRIQ DSQ shouldn't be using FIFO enqueueing. As tasks are
		 * linked to both the rbtree and list on PRIQs, this can only be
		 * tested easily when adding the first task.
		 */
		اگر (unlikely(RB_EMPTY_ریشه(&dsq->priq) &&
			     nldsq_next_task(dsq, پوچ, false)))
			scx_error(sch, "DSQ ID 0x%016llx already had FIFO-enqueued tasks",
				  dsq->id);

		p->scx.dsq_flags |= SCX_TASK_DSQ_ON_PRIQ;
		rb_add(&p->scx.dsq_priq, &dsq->priq, scx_dsq_priq_less);

		/*
		 * Find the previous task and insert after it on the list so
		 * that @dsq->list is vtime ordered.
		 */
		rbp = rb_prev(&p->scx.dsq_priq);
		اگر (rbp) {
			ساختار task_struct *prev =
				container_of(rbp, ساختار task_struct,
					     scx.dsq_priq);
			list_add(&p->scx.dsq_list.گره, &prev->scx.dsq_list.گره);
			/* first task unchanged - no update needed */
		} وگرنه {
			list_add(&p->scx.dsq_list.گره, &dsq->list);
			/* not builtin and new task is at head - use fastpath */
			rcu_assign_pointer(dsq->first_task, p);
		}
	} وگرنه {
		/* a FIFO DSQ shouldn't be using PRIQ enqueuing */
		اگر (unlikely(!RB_EMPTY_ریشه(&dsq->priq)))
			scx_error(sch, "DSQ ID 0x%016llx already had PRIQ-enqueued tasks",
				  dsq->id);

		اگر (enq_flags & (SCX_ENQ_HEAD | SCX_ENQ_PREEMPT)) {
			/* new task inserted at head - use fastpath */
			اگر (dsq_درج_head(dsq, p) && !(dsq->id & SCX_DSQ_FLAG_BUILTIN))
				rcu_assign_pointer(dsq->first_task, p);
		} وگرنه {
			/*
			 * dsq->list can contain parked BPF iterator cursors, so
			 * list_empty() here isn't a reliable proxy for "no real
			 * task in the DSQ". Test dsq->first_task directly.
			 */
			list_add_tail(&p->scx.dsq_list.گره, &dsq->list);
			اگر (!dsq->first_task && !(dsq->id & SCX_DSQ_FLAG_BUILTIN))
				rcu_assign_pointer(dsq->first_task, p);
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
	اگر (is_rq_owned) {
		rq_owned_post_enq(sch, rq, dsq, p, enq_flags);
	} وگرنه {
		bool call_dequeue = false;

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
		اگر (dsq->id == SCX_DSQ_GLOBAL || dsq->id == SCX_DSQ_BYPASS)
			call_dequeue = task_leave_custody(p);
		وگرنه
			p->scx.flags |= SCX_TASK_IN_CUSTODY;

		raw_spin_unlock(&dsq->lock);

		اگر (call_dequeue && SCX_HAS_OP(sch, dequeue))
			SCX_CALL_OP_TASK(sch, dequeue, rq, p, 0);
	}

	/*
	 * We're transitioning out of QUEUEING or DISPATCHING. store_release to
	 * match waiters' load_acquire.
	 */
	اگر (enq_flags & SCX_ENQ_CLEAR_OPSS)
		atomic_long_set_release(&p->scx.ops_state, SCX_OPSS_NONE);
}

تهی scx_task_unlink_from_dsq(ساختار task_struct *p, ساختار scx_dispatch_q *dsq)
{
	WARN_ON_ONCE(list_empty(&p->scx.dsq_list.گره));

	اگر (p->scx.dsq_flags & SCX_TASK_DSQ_ON_PRIQ) {
		rb_erase(&p->scx.dsq_priq, &dsq->priq);
		RB_CLEAR_گره(&p->scx.dsq_priq);
		p->scx.dsq_flags &= ~SCX_TASK_DSQ_ON_PRIQ;
	}

	list_del_init(&p->scx.dsq_list.گره);
	dsq_dec_nr(dsq, p);

	اگر (!(dsq->id & SCX_DSQ_FLAG_BUILTIN) && rcu_access_pointer(dsq->first_task) == p) {
		ساختار task_struct *first_task;

		first_task = nldsq_next_task(dsq, پوچ, false);
		rcu_assign_pointer(dsq->first_task, first_task);
	}
}

تهی scx_dispatch_dequeue(ساختار rq *rq, ساختار task_struct *p)
{
	ساختار scx_dispatch_q *dsq = p->scx.dsq;
	bool is_rq_owned = dsq && dsq_is_rq_owned(dsq);

	lockdep_assert_rq_held(rq);

	اگر (!dsq) {
		/*
		 * If !dsq && on-list, @p is on @rq's ddsp_deferred_locals.
		 * Unlinking is all that's needed to cancel.
		 */
		اگر (unlikely(!list_empty(&p->scx.dsq_list.گره)))
			list_del_init(&p->scx.dsq_list.گره);

		/*
		 * When dispatching directly from the BPF scheduler to a local
		 * DSQ, the task isn't associated with any DSQ but
		 * @p->scx.holding_cpu may be set under the protection of
		 * %SCX_OPSS_DISPATCHING.
		 */
		اگر (p->scx.holding_cpu >= 0)
			p->scx.holding_cpu = -1;

		بازگشت;
	}

	اگر (!is_rq_owned)
		raw_spin_lock(&dsq->lock);

	/*
	 * Now that we hold @dsq->lock, @p->holding_cpu and @p->scx.dsq_* can't
	 * change underneath us.
	*/
	اگر (p->scx.holding_cpu < 0) {
		/* @p must still be on @dsq, dequeue */
		scx_task_unlink_from_dsq(p, dsq);
	} وگرنه {
		/*
		 * We're racing against dispatch_to_local_dsq() which already
		 * removed @p from @dsq and set @p->scx.holding_cpu. Clear the
		 * holding_cpu which tells dispatch_to_local_dsq() that it lost
		 * the race.
		 */
		WARN_ON_ONCE(!list_empty(&p->scx.dsq_list.گره));
		p->scx.holding_cpu = -1;
	}
	p->scx.dsq = پوچ;

	اگر (!is_rq_owned)
		raw_spin_unlock(&dsq->lock);
}

/*
 * Abbreviated version of scx_dispatch_dequeue() that can be used when both
 * @p's rq and dsq are locked.
 */
ایستا تهی dispatch_dequeue_locked(ساختار task_struct *p,
				    ساختار scx_dispatch_q *dsq)
{
	lockdep_assert_rq_held(task_rq(p));
	lockdep_assert_held(&dsq->lock);

	scx_task_unlink_from_dsq(p, dsq);
	p->scx.dsq = پوچ;
}

ایستا ساختار scx_dispatch_q *find_dsq_for_dispatch(ساختار scx_sched *sch,
						    ساختار rq *rq, u64 dsq_id,
						    s32 tcpu)
{
	ساختار scx_dispatch_q *dsq;

	اگر (dsq_id == SCX_DSQ_LOCAL)
		بازگشت &rq->scx.local_dsq;

	اگر ((dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON) {
		s32 cpu = scx_cpu_ret(sch, dsq_id & SCX_DSQ_LOCAL_CPU_MASK);

		اگر (!scx_cpu_valid(sch, cpu, "in SCX_DSQ_LOCAL_ON dispatch verdict"))
			بازگشت find_global_dsq(sch, tcpu);

		بازگشت &cpu_rq(cpu)->scx.local_dsq;
	}

	اگر (dsq_id == SCX_DSQ_GLOBAL)
		dsq = find_global_dsq(sch, tcpu);
	وگرنه
		dsq = find_user_dsq(sch, dsq_id);

	/*
	 * Built-in DSQs are never inserted into dsq_hash, so REJECT and RESCUE
	 * hit the error below. They cannot be reached with an ID.
	 */
	اگر (unlikely(!dsq)) {
		scx_error(sch, "non-existent DSQ 0x%llx", dsq_id);
		بازگشت find_global_dsq(sch, tcpu);
	}

	بازگشت dsq;
}

ایستا تهی mark_direct_dispatch(ساختار scx_sched *sch,
				 ساختار task_struct *ddsp_task,
				 ساختار task_struct *p, u64 dsq_id,
				 u64 slice, u64 vtime, u64 enq_flags)
{
	/*
	 * Mark that dispatch already happened from ops.select_cpu() or
	 * ops.enqueue() by spoiling direct_dispatch_task with a non-NULL value
	 * which can never match a valid task pointer.
	 */
	__this_cpu_write(direct_dispatch_task, ERR_PTR(-ESRCH));

	/* @p must match the task on the enqueue path */
	اگر (unlikely(p != ddsp_task)) {
		اگر (IS_ERR(ddsp_task))
			scx_error(sch, "%s[%d] already direct-dispatched",
				  p->comm, p->pid);
		وگرنه
			scx_error(sch, "scheduling for %s[%d] but trying to direct-dispatch %s[%d]",
				  ddsp_task->comm, ddsp_task->pid,
				  p->comm, p->pid);
		بازگشت;
	}

	WARN_ON_ONCE(p->scx.ddsp_dsq_id != SCX_DSQ_INVALID);
	WARN_ON_ONCE(p->scx.ddsp_enq_flags);

	p->scx.ddsp_slice = slice;
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
ایستا درونخطی تهی clear_direct_dispatch(ساختار task_struct *p)
{
	p->scx.ddsp_dsq_id = SCX_DSQ_INVALID;
	p->scx.ddsp_enq_flags = 0;
}

ایستا تهی direct_dispatch(ساختار scx_sched *sch, ساختار task_struct *p,
			    u64 enq_flags)
{
	ساختار rq *rq = task_rq(p);
	ساختار scx_dispatch_q *dsq =
		find_dsq_for_dispatch(sch, rq, p->scx.ddsp_dsq_id, task_cpu(p));
	u64 ddsp_enq_flags, slice, vtime;

	p->scx.ddsp_enq_flags |= enq_flags;

	/*
	 * We are in the enqueue path with @rq locked and pinned, and thus can't
	 * double lock a remote rq and enqueue to its local DSQ. For
	 * DSQ_LOCAL_ON verdicts targeting the local DSQ of a remote CPU, defer
	 * the enqueue so that it's executed when @rq can be unlocked.
	 */
	اگر (dsq->id == SCX_DSQ_LOCAL && dsq != &rq->scx.local_dsq) {
		unsigned long opss;

		opss = atomic_long_read(&p->scx.ops_state) & SCX_OPSS_STATE_MASK;

		انتخاب (opss & SCX_OPSS_STATE_MASK) {
		حالت SCX_OPSS_NONE:
			بشکن;
		حالت SCX_OPSS_QUEUEING:
			/*
			 * As @p was never passed to the BPF side, _release is
			 * not strictly necessary. Still do it for consistency.
			 */
			atomic_long_set_release(&p->scx.ops_state, SCX_OPSS_NONE);
			بشکن;
		پیشفرض:
			WARN_ONCE(true, "sched_ext: %s[%d] has invalid ops state 0x%lx in direct_dispatch()",
				  p->comm, p->pid, opss);
			atomic_long_set_release(&p->scx.ops_state, SCX_OPSS_NONE);
			بشکن;
		}

		WARN_ON_ONCE(p->scx.dsq || !list_empty(&p->scx.dsq_list.گره));
		list_add_tail(&p->scx.dsq_list.گره,
			      &rq->scx.ddsp_deferred_locals);
		schedule_deferred_locked(rq);
		بازگشت;
	}

	ddsp_enq_flags = p->scx.ddsp_enq_flags;
	slice = p->scx.ddsp_slice;
	vtime = p->scx.ddsp_vtime;
	clear_direct_dispatch(p);

	scx_dispatch_enqueue(sch, rq, dsq, p, slice, vtime,
			     ddsp_enq_flags | SCX_ENQ_APPLY_SLICE | SCX_ENQ_CLEAR_OPSS);
}

bool scx_rq_online(ساختار rq *rq)
{
	/*
	 * Test both cpu_active() and %SCX_RQ_ONLINE. %SCX_RQ_ONLINE indicates
	 * the online state as seen from the BPF scheduler. cpu_active() test
	 * guarantees that, if this function returns %true, %SCX_RQ_ONLINE will
	 * stay set until the current scheduling operation is complete even if
	 * we aren't locking @rq.
	 */
	بازگشت likely((rq->scx.flags & SCX_RQ_ONLINE) && cpu_active(cpu_of(rq)));
}

تهی scx_do_enqueue_task(ساختار rq *rq, ساختار task_struct *p, u64 enq_flags,
			 صحیح sticky_cpu)
{
	ساختار scx_sched *sch = scx_task_sched(p);
	ساختار task_struct **ddsp_taskp;
	ساختار scx_dispatch_q *dsq;
	unsigned long qseq;

	WARN_ON_ONCE(!(p->scx.flags & SCX_TASK_QUEUED));

	/* internal movements - rq migration / RESTORE */
	اگر (sticky_cpu == cpu_of(rq))
		برو local_norefill;

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
	اگر (enq_flags & SCX_ENQ_REENQ) {
		اگر (++p->scx.reenq_cnt > 1)
			__scx_add_event(sch, SCX_EV_REENQ_REPEAT, 1);

		اگر (unlikely(p->scx.reenq_cnt > SCX_REENQ_MAX_REPEAT)) {
			__scx_exit(sch, SCX_EXIT_ERROR_REENQ, 0, cpu_of(rq),
				   "%s[%d] reenqueued %u times without running",
				   p->comm, p->pid, p->scx.reenq_cnt);
			بازگشت;
		}
	}

	/*
	 * If !scx_rq_online(), we already told the BPF scheduler that the CPU
	 * is offline and are just running the hotplug path. Don't bother the
	 * BPF scheduler.
	 */
	اگر (!scx_rq_online(rq))
		برو local;

	اگر (scx_bypassing(sch, cpu_of(rq))) {
		__scx_add_event(sch, SCX_EV_BYPASS_DISPATCH, 1);
		برو bypass;
	}

	اگر (p->scx.ddsp_dsq_id != SCX_DSQ_INVALID)
		برو direct;

	/* see %SCX_OPS_ENQ_EXITING */
	اگر (!(sch->ops.flags & SCX_OPS_ENQ_EXITING) &&
	    unlikely(p->flags & PF_EXITING)) {
		__scx_add_event(sch, SCX_EV_ENQ_SKIP_EXITING, 1);
		enq_flags |= SCX_ENQ_RESCUE;	/* avoid looping on cap rejection */
		برو local;
	}

	/* see %SCX_OPS_ENQ_MIGRATION_DISABLED */
	اگر (!(sch->ops.flags & SCX_OPS_ENQ_MIGRATION_DISABLED) &&
	    is_migration_disabled(p)) {
		__scx_add_event(sch, SCX_EV_ENQ_SKIP_MIGRATION_DISABLED, 1);
		برو local;
	}

	اگر (unlikely(!SCX_HAS_OP(sch, enqueue)))
		برو global;

	/*
	 * DSQ bypass didn't trigger, enqueue on the BPF scheduler. Wrap the
	 * per-task qseq counter where the QSEQ field wraps and skip 0, which is
	 * what scx_bpf_dsq_insert() records for a task in NONE or DISPATCHING.
	 */
	p->scx.ops_qseq = ((p->scx.ops_qseq + 1) &
			   (SCX_OPSS_QSEQ_MASK >> SCX_OPSS_QSEQ_SHIFT)) ?: 1;
	qseq = (unsigned long)p->scx.ops_qseq << SCX_OPSS_QSEQ_SHIFT;

	WARN_ON_ONCE(atomic_long_read(&p->scx.ops_state) != SCX_OPSS_NONE);
	atomic_long_set(&p->scx.ops_state, SCX_OPSS_QUEUEING | qseq);

	ddsp_taskp = this_cpu_ptr(&direct_dispatch_task);
	WARN_ON_ONCE(*ddsp_taskp);
	*ddsp_taskp = p;

	SCX_CALL_OP_TASK(sch, enqueue, rq, p, enq_flags);

	*ddsp_taskp = پوچ;
	اگر (p->scx.ddsp_dsq_id != SCX_DSQ_INVALID)
		برو direct;

	/*
	 * Task is now in BPF scheduler's custody. Set %SCX_TASK_IN_CUSTODY
	 * so ops.dequeue() is called when it leaves custody.
	 */
	p->scx.flags |= SCX_TASK_IN_CUSTODY;

	/*
	 * If not directly dispatched, QUEUEING isn't clear yet and dispatch or
	 * dequeue may be waiting. The store_release matches their load_acquire.
	 */
	atomic_long_set_release(&p->scx.ops_state, SCX_OPSS_QUEUED | qseq);
	بازگشت;

direct:
	direct_dispatch(sch, p, enq_flags);
	بازگشت;
local_norefill:
	scx_dispatch_enqueue(sch, rq, &rq->scx.local_dsq, p, 0, 0, enq_flags);
	بازگشت;
local:
	dsq = &rq->scx.local_dsq;
	برو enqueue;
global:
	dsq = find_global_dsq(sch, task_cpu(p));
	برو enqueue;
bypass:
	dsq = bypass_enq_target_dsq(sch, task_cpu(p));
	برو enqueue;

enqueue:
	refill_task_slice_dfl(sch, p);
	clear_direct_dispatch(p);
	scx_dispatch_enqueue(sch, rq, dsq, p, 0, 0, enq_flags);
}

ایستا bool task_runnable(ثابت ساختار task_struct *p)
{
	بازگشت !list_empty(&p->scx.runnable_گره);
}

ایستا تهی set_task_runnable(ساختار rq *rq, ساختار task_struct *p)
{
	lockdep_assert_rq_held(rq);

	اگر (p->scx.flags & SCX_TASK_RESET_RUNNABLE_AT) {
		p->scx.runnable_at = jiffies;
		p->scx.flags &= ~SCX_TASK_RESET_RUNNABLE_AT;
	}

	/*
	 * list_add_tail() must be used. scx_bypass() and rq_offline_scx()
	 * depend on tasks being appended to the runnable_list.
	 */
	list_add_tail(&p->scx.runnable_گره, &rq->scx.runnable_list);

	/*
	 * Record the rq @p is runnable on, maintained under the rq lock so it
	 * stays valid unlike task_cpu(), which a remote wakeup can move under
	 * pi_lock alone.
	 */
	WRITE_ONCE(p->scx.runnable_cpu, cpu_of(rq));
}

ایستا تهی clr_task_runnable(ساختار task_struct *p, bool reset_runnable_at)
{
	list_del_init(&p->scx.runnable_گره);
	WRITE_ONCE(p->scx.runnable_cpu, -1);
	اگر (reset_runnable_at) {
		p->scx.flags |= SCX_TASK_RESET_RUNNABLE_AT;
		p->scx.reenq_cnt = 0;
	}
}

ایستا تهی enqueue_task_scx(ساختار rq *rq, ساختار task_struct *p, صحیح core_enq_flags)
{
	ساختار scx_sched *sch = scx_task_sched(p);
	صحیح sticky_cpu = p->scx.sticky_cpu;
	u64 enq_flags = core_enq_flags | rq->scx.remote_activate_enq_flags;

	/*
	 * An SCX-internal migration ends on arrival. Clear sticky_cpu so @p can
	 * leave custody when inserted into the destination DSQ.
	 */
	اگر (sticky_cpu >= 0)
		p->scx.sticky_cpu = -1;

	/*
	 * SCX_RQ_IN_WAKEUP promises a task_woken_scx() call once this enqueue
	 * returns. Only the core's wakeup path delivers one. The flags stashed
	 * for a remote activation may carry the wakeup bit without it.
	 */
	اگر (core_enq_flags & ENQUEUE_WAKEUP)
		rq->scx.flags |= SCX_RQ_IN_WAKEUP;

	/*
	 * Restoring a running task will be immediately followed by
	 * set_next_task_scx() which expects the task to not be on the BPF
	 * scheduler as tasks can only start running through local DSQs. Force
	 * direct-dispatch into the local DSQ by setting the sticky_cpu. Mark
	 * IGNORE_CAPS to force entry into the local DSQ.
	 */
	اگر (unlikely(enq_flags & ENQUEUE_RESTORE) && task_current(rq, p)) {
		sticky_cpu = cpu_of(rq);
		enq_flags |= SCX_ENQ_IGNORE_CAPS;
	}

	اگر (p->scx.flags & SCX_TASK_QUEUED) {
		WARN_ON_ONCE(!task_runnable(p));
		برو out;
	}

	set_task_runnable(rq, p);
	p->scx.flags |= SCX_TASK_QUEUED;
	rq->scx.nr_running++;
	add_nr_running(rq, 1);

	اگر (SCX_HAS_OP(sch, runnable) && !task_on_rq_migrating(p))
		SCX_CALL_OP_TASK(sch, runnable, rq, p, enq_flags);

	/* Start dl_server if this is the first task being enqueued */
	اگر (rq->scx.nr_running == 1)
		dl_server_start(&rq->ext_server);

	scx_do_enqueue_task(rq, p, enq_flags, sticky_cpu);
out:
	rq->scx.flags &= ~SCX_RQ_IN_WAKEUP;

	اگر ((enq_flags & SCX_ENQ_CPU_SELECTED) &&
	    unlikely(cpu_of(rq) != p->scx.selected_cpu))
		__scx_add_event(sch, SCX_EV_SELECT_CPU_FALLBACK, 1);
}

ایستا تهی ops_dequeue(ساختار rq *rq, ساختار task_struct *p, u64 deq_flags)
{
	ساختار scx_sched *sch = scx_task_sched(p);
	unsigned long opss;

	/* dequeue is always temporary, don't reset runnable_at */
	clr_task_runnable(p, false);

retry:
	/* acquire ensures that we see the preceding updates on QUEUED */
	opss = atomic_long_read_acquire(&p->scx.ops_state);

	انتخاب (opss & SCX_OPSS_STATE_MASK) {
	حالت SCX_OPSS_NONE:
		بشکن;
	حالت SCX_OPSS_QUEUEING:
		/*
		 * QUEUEING is started and finished while holding @p's rq lock.
		 * As we're holding the rq lock now, we shouldn't see QUEUEING.
		 */
		BUG();
	حالت SCX_OPSS_QUEUED:
		/*
		 * A queued task must always be in BPF scheduler's custody. If
		 * SCX_TASK_IN_CUSTODY is clear, finish_dispatch() on another
		 * CPU has already passed task_leave_custody() (which clears the
		 * flag), but has not yet written SCX_OPSS_NONE. That final
		 * store does not require this rq's lock, so retrying with
		 * cpu_relax() is bounded: we will observe NONE (or DISPATCHING,
		 * handled by the fallthrough) on a subsequent iteration.
		 */
		اگر (unlikely(!(READ_ONCE(p->scx.flags) & SCX_TASK_IN_CUSTODY))) {
			cpu_relax();
			برو retry;
		}

		اگر (atomic_long_try_cmpxchg(&p->scx.ops_state, &opss,
					    SCX_OPSS_NONE))
			بشکن;
		fallthrough;
	حالت SCX_OPSS_DISPATCHING:
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
		بشکن;
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
	اگر (task_leave_custody(p) && SCX_HAS_OP(sch, dequeue))
		SCX_CALL_OP_TASK(sch, dequeue, rq, p, deq_flags);
}

ایستا bool dequeue_task_scx(ساختار rq *rq, ساختار task_struct *p, صحیح core_deq_flags)
{
	ساختار scx_sched *sch = scx_task_sched(p);
	u64 deq_flags = core_deq_flags;

	/*
	 * Set %SCX_DEQ_SCHED_CHANGE when the dequeue is due to a property
	 * change (not sleep).
	 */
	اگر (!(deq_flags & DEQUEUE_SLEEP))
		deq_flags |= SCX_DEQ_SCHED_CHANGE;

	اگر (!(p->scx.flags & SCX_TASK_QUEUED)) {
		WARN_ON_ONCE(task_runnable(p));
		بازگشت true;
	}

	ops_dequeue(rq, p, deq_flags);

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
	اگر (task_current(rq, p) &&
	    (SCX_HAS_OP(sch, stopping) || unlikely(p == scx_rescuee(rq)))) {
		update_curr_scx(rq);
		اگر (SCX_HAS_OP(sch, stopping))
			SCX_CALL_OP_TASK(sch, stopping, rq, p, false);
	}

	اگر (SCX_HAS_OP(sch, quiescent) && !task_on_rq_migrating(p))
		SCX_CALL_OP_TASK(sch, quiescent, rq, p, deq_flags);

	اگر (deq_flags & SCX_DEQ_SLEEP)
		p->scx.flags |= SCX_TASK_DEQD_FOR_SLEEP;
	وگرنه
		p->scx.flags &= ~SCX_TASK_DEQD_FOR_SLEEP;

	p->scx.flags &= ~SCX_TASK_QUEUED;
	rq->scx.nr_running--;
	sub_nr_running(rq, 1);

	scx_dispatch_dequeue(rq, p);

	/* see scx_task_slice_ended() for the save/restore exception */
	اگر (!((deq_flags & DEQUEUE_SAVE) && task_current(rq, p)))
		scx_task_slice_ended(rq, p);

	clear_direct_dispatch(p);
	بازگشت true;
}

ایستا تهی yield_task_scx(ساختار rq *rq)
{
	ساختار task_struct *p = rq->donor;
	ساختار scx_sched *sch = scx_task_sched(p);

	/* a yield gives the slice up */
	scx_task_slice_ended(rq, p);

	اگر (SCX_HAS_OP(sch, yield))
		SCX_CALL_OP_2TASKS_RET(sch, yield, rq, p, پوچ);
	وگرنه
		scx_set_task_slice(p, 0);
}

ایستا bool yield_to_task_scx(ساختار rq *rq, ساختار task_struct *to)
{
	ساختار task_struct *from = rq->donor;
	ساختار scx_sched *sch = scx_task_sched(from);

	/* like a plain yield, giving the slice up ends the protection */
	scx_task_slice_ended(rq, from);

	اگر (SCX_HAS_OP(sch, yield) && sch == scx_task_sched(to))
		بازگشت SCX_CALL_OP_2TASKS_RET(sch, yield, rq, from, to);
	وگرنه
		بازگشت false;
}

ایستا تهی wakeup_preempt_scx(ساختار rq *rq, ساختار task_struct *p, صحیح wake_flags)
{
	/*
	 * Preemption between SCX tasks is implemented by resetting the victim
	 * task's slice to 0 and triggering reschedule on the target CPU.
	 * Nothing to do.
	 */
	اگر (p->sched_class == &ext_sched_class)
		بازگشت;

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
	اگر (rq->scx.nr_immed)
		scx_schedule_reenq_local(rq, 0);
}

تهی scx_move_local_task_to_local_dsq(ساختار scx_sched *sch, ساختار task_struct *p,
				      u64 enq_flags, ساختار rq *dst_rq)
{
	ساختار scx_dispatch_q *dst_dsq;

	lockdep_assert_rq_held(dst_rq);

	WARN_ON_ONCE(p->scx.holding_cpu >= 0);
	scx_wait_dispatching(p);

	dst_dsq = scx_resolve_local_dsq(sch, dst_rq, p, &enq_flags);

	اگر (enq_flags & (SCX_ENQ_HEAD | SCX_ENQ_PREEMPT))
		dsq_درج_head(dst_dsq, p);
	وگرنه
		list_add_tail(&p->scx.dsq_list.گره, &dst_dsq->list);

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
ایستا تهی move_remote_task_to_local_dsq(ساختار scx_sched *sch,
					  ساختار task_struct *p, u64 enq_flags,
					  ساختار rq *src_rq, ساختار rq *dst_rq)
{
	lockdep_assert_rq_held(src_rq);

	/*
	 * Set sticky_cpu before deactivate_task() to properly mark the
	 * beginning of an SCX-internal migration.
	 */
	p->scx.sticky_cpu = cpu_of(dst_rq);
	deactivate_task(src_rq, p, 0);
	set_task_cpu(p, cpu_of(dst_rq));

	switch_rq_lock(src_rq, dst_rq);

	/*
	 * activate_task() below truncates enq_flags to 32 bits and re-derives
	 * @p's owner, dropping our scx flags and the placing @sch. We own @rq,
	 * so stash both across the call. The enqueue reads them back, keeping
	 * the scx flags and checking caps against the placer, not the owner.
	 */
	WARN_ON_ONCE(!cpumask_test_cpu(cpu_of(dst_rq), p->cpus_ptr));
	WARN_ON_ONCE(dst_rq->scx.remote_activate_enq_flags ||
		     dst_rq->scx.remote_activate_sch);
	dst_rq->scx.remote_activate_enq_flags = enq_flags;
	dst_rq->scx.remote_activate_sch = sch;
	activate_task(dst_rq, p, 0);
	dst_rq->scx.remote_activate_enq_flags = 0;
	dst_rq->scx.remote_activate_sch = پوچ;
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
ایستا bool task_can_run_on_remote_rq(ساختار scx_sched *sch,
				      ساختار task_struct *p, ساختار rq *rq,
				      bool enforce)
{
	s32 cpu = cpu_of(rq);

	/*
	 * To prevent races with @p still running on its old CPU while switching
	 * out, make sure we're holding @p's rq lock so as not to risk
	 * erroneously killing the BPF scheduler.
	 */
	اگر (enforce)
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
	اگر (unlikely(is_migration_disabled(p))) {
		اگر (enforce)
			scx_error(sch, "SCX_DSQ_LOCAL[_ON] cannot move migration disabled %s[%d] from CPU %d to %d",
				  p->comm, p->pid, task_cpu(p), cpu);
		بازگشت false;
	}

	/*
	 * We don't require the BPF scheduler to avoid dispatching to offline
	 * CPUs mostly for convenience but also because CPUs can go offline
	 * between scx_bpf_dsq_insert() calls and here. Trigger error iff the
	 * picked CPU is outside the allowed mask.
	 */
	اگر (!task_allowed_on_cpu(p, cpu)) {
		اگر (enforce)
			scx_error(sch, "SCX_DSQ_LOCAL[_ON] target CPU %d not allowed for %s[%d]",
				  cpu, p->comm, p->pid);
		بازگشت false;
	}

	اگر (!scx_rq_online(rq)) {
		اگر (enforce)
			__scx_add_event(sch, SCX_EV_DISPATCH_LOCAL_DSQ_OFFLINE, 1);
		بازگشت false;
	}

	بازگشت true;
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
ایستا bool unlink_dsq_and_switch_rq_lock(ساختار task_struct *p,
					  ساختار scx_dispatch_q *dsq,
					  ساختار rq *locked_rq,
					  ساختار rq *src_rq)
{
	s32 cpu = raw_smp_processor_id();

	lockdep_assert_held(&dsq->lock);
	lockdep_assert_rq_held(locked_rq);

	WARN_ON_ONCE(p->scx.holding_cpu >= 0);
	scx_task_unlink_from_dsq(p, dsq);
	p->scx.holding_cpu = cpu;

	raw_spin_unlock(&dsq->lock);
	switch_rq_lock(locked_rq, src_rq);

	/* task_rq couldn't have changed if we're still the holding cpu */
	اگر (likely(p->scx.holding_cpu == cpu) && !WARN_ON_ONCE(src_rq != task_rq(p))) {
		scx_wait_dispatching(p);
		بازگشت true;
	}

	بازگشت false;
}

ایستا bool consume_remote_task(ساختار scx_sched *sch, ساختار rq *this_rq,
				ساختار task_struct *p, u64 enq_flags,
				ساختار scx_dispatch_q *dsq, ساختار rq *src_rq)
{
	اگر (unlink_dsq_and_switch_rq_lock(p, dsq, this_rq, src_rq)) {
		move_remote_task_to_local_dsq(sch, p, enq_flags, src_rq, this_rq);
		بازگشت true;
	} وگرنه {
		switch_rq_lock(src_rq, this_rq);
		بازگشت false;
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
ایستا ساختار rq *move_task_between_dsqs(ساختار scx_sched *sch,
					 ساختار task_struct *p, u64 enq_flags,
					 ساختار scx_dispatch_q *src_dsq,
					 ساختار scx_dispatch_q *dst_dsq)
{
	ساختار rq *src_rq = task_rq(p), *dst_rq;

	BUG_ON(src_dsq->id == SCX_DSQ_LOCAL);
	lockdep_assert_held(&src_dsq->lock);
	lockdep_assert_rq_held(src_rq);

	scx_wait_dispatching(p);

	اگر (dst_dsq->id == SCX_DSQ_LOCAL) {
		dst_rq = container_of(dst_dsq, ساختار rq, scx.local_dsq);
		اگر (src_rq != dst_rq &&
		    unlikely(!task_can_run_on_remote_rq(sch, p, dst_rq, true))) {
			dst_dsq = find_global_dsq(sch, task_cpu(p));
			dst_rq = src_rq;
			enq_flags |= SCX_ENQ_GDSQ_FALLBACK;
		}
	} وگرنه {
		/* no need to migrate if destination is a non-local DSQ */
		dst_rq = src_rq;
	}

	/*
	 * Move @p into $dst_dsq. If $dst_dsq is the local DSQ of a different
	 * CPU, @p will be migrated.
	 */
	اگر (dst_dsq->id == SCX_DSQ_LOCAL) {
		/* @p is going from a non-local DSQ to a local DSQ */
		اگر (src_rq == dst_rq) {
			scx_task_unlink_from_dsq(p, src_dsq);
			raw_spin_unlock(&src_dsq->lock);
			scx_move_local_task_to_local_dsq(sch, p, enq_flags, dst_rq);
		} وگرنه {
			raw_spin_unlock(&src_dsq->lock);
			move_remote_task_to_local_dsq(sch, p, enq_flags, src_rq, dst_rq);
		}
	} وگرنه {
		/*
		 * @p is going from a non-local DSQ to a non-local DSQ. As
		 * $src_dsq is already locked, do an abbreviated dequeue.
		 */
		dispatch_dequeue_locked(p, src_dsq);
		raw_spin_unlock(&src_dsq->lock);

		scx_dispatch_enqueue(sch, dst_rq, dst_dsq, p, 0, 0, enq_flags);
	}

	بازگشت dst_rq;
}

bool scx_consume_dispatch_q(ساختار scx_sched *sch, ساختار rq *rq,
			    ساختار scx_dispatch_q *dsq, u64 enq_flags)
{
	ساختار task_struct *p;
retry:
	/*
	 * The caller can't expect to successfully consume a task if the task's
	 * addition to @dsq isn't guaranteed to be visible somehow. Test
	 * @dsq->list without locking and skip if it seems empty.
	 */
	اگر (list_empty(&dsq->list))
		بازگشت false;

	raw_spin_lock(&dsq->lock);

	nldsq_for_each_task(p, dsq) {
		ساختار rq *task_rq = task_rq(p);

		/*
		 * This loop can lead to multiple lockup scenarios, e.g. the BPF
		 * scheduler can put an enormous number of affinitized tasks into
		 * a contended DSQ, or the outer retry loop can repeatedly race
		 * against scx_bypass() dequeueing tasks from @dsq trying to put
		 * the system into the bypass mode. This can easily live-lock the
		 * machine. If aborting, exit from all non-bypass DSQs.
		 */
		اگر (unlikely(READ_ONCE(sch->aborting)) && dsq->id != SCX_DSQ_BYPASS)
			بشکن;

		اگر (rq == task_rq) {
			scx_task_unlink_from_dsq(p, dsq);
			raw_spin_unlock(&dsq->lock);
			scx_move_local_task_to_local_dsq(sch, p, enq_flags, rq);
			بازگشت true;
		}

		اگر (task_can_run_on_remote_rq(sch, p, rq, false)) {
			اگر (likely(consume_remote_task(sch, rq, p, enq_flags, dsq, task_rq)))
				بازگشت true;
			برو retry;
		}
	}

	raw_spin_unlock(&dsq->lock);
	بازگشت false;
}

bool scx_consume_global_dsq(ساختار scx_sched *sch, ساختار rq *rq)
{
	صحیح گره = cpu_to_گره(cpu_of(rq));

	بازگشت scx_consume_dispatch_q(sch, rq, &sch->pnode[گره]->global_dsq, 0);
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
ایستا تهی dispatch_to_local_dsq(ساختار scx_sched *sch, ساختار rq *rq,
				  ساختار scx_dispatch_q *dst_dsq, ساختار task_struct *p,
				  u64 slice, u64 vtime, u64 enq_flags)
{
	ساختار rq *src_rq = task_rq(p);
	ساختار rq *dst_rq = container_of(dst_dsq, ساختار rq, scx.local_dsq);
	ساختار rq *locked_rq = rq;

	/*
	 * We're synchronized against dequeue through DISPATCHING. As @p can't
	 * be dequeued, its task_rq and cpus_allowed are stable too.
	 *
	 * If dispatching to @rq that @p is already on, no lock dancing needed.
	 */
	اگر (rq == src_rq && rq == dst_rq) {
		scx_dispatch_enqueue(sch, rq, dst_dsq, p, slice, vtime,
				     enq_flags | SCX_ENQ_APPLY_SLICE | SCX_ENQ_CLEAR_OPSS);
		بازگشت;
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
	atomic_long_set_release(&p->scx.ops_state, SCX_OPSS_NONE);

	/* switch to @src_rq lock */
	اگر (locked_rq != src_rq) {
		switch_rq_lock(locked_rq, src_rq);
		locked_rq = src_rq;
	}

	/* task_rq couldn't have changed if we're still the holding cpu */
	اگر (likely(p->scx.holding_cpu == raw_smp_processor_id()) &&
	    !WARN_ON_ONCE(src_rq != task_rq(p))) {
		bool fallback = false;
		/*
		 * If @p is staying on the same rq, there's no need to go
		 * through the full deactivate/activate cycle. Optimize by
		 * abbreviating move_remote_task_to_local_dsq().
		 */
		اگر (src_rq == dst_rq) {
			p->scx.holding_cpu = -1;
			scx_dispatch_enqueue(sch, dst_rq, &dst_rq->scx.local_dsq, p,
					     slice, vtime, enq_flags | SCX_ENQ_APPLY_SLICE);
		} وگرنه اگر (unlikely(!task_can_run_on_remote_rq(sch, p, dst_rq, true))) {
			p->scx.holding_cpu = -1;
			fallback = true;
			scx_dispatch_enqueue(sch, src_rq, find_global_dsq(sch, task_cpu(p)),
					     p, slice, vtime,
					     enq_flags | SCX_ENQ_APPLY_SLICE |
					     SCX_ENQ_GDSQ_FALLBACK);
		} وگرنه {
			apply_slice_vtime(p, slice, vtime, enq_flags);
			move_remote_task_to_local_dsq(sch, p, enq_flags, src_rq, dst_rq);
			/* task has been moved to dst_rq, which is now locked */
			locked_rq = dst_rq;
		}

		/* if the destination CPU is idle, wake it up */
		اگر (!fallback && sched_class_above(p->sched_class, dst_rq->curr->sched_class))
			resched_curr(dst_rq);
	}

	/* switch back to @rq lock */
	اگر (locked_rq != rq)
		switch_rq_lock(locked_rq, rq);
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
ایستا تهی finish_dispatch(ساختار scx_sched *sch, ساختار rq *rq, ساختار task_struct *p,
			    unsigned long qseq_at_dispatch, u64 dsq_id,
			    u64 slice, u64 vtime, u64 enq_flags)
{
	ساختار scx_dispatch_q *dsq;
	unsigned long opss;

retry:
	/*
	 * No need for _acquire here. @p is accessed only after a successful
	 * try_cmpxchg to DISPATCHING.
	 */
	opss = atomic_long_read(&p->scx.ops_state);

	انتخاب (opss & SCX_OPSS_STATE_MASK) {
	حالت SCX_OPSS_DISPATCHING:
	حالت SCX_OPSS_NONE:
		/* someone else already got to it */
		بازگشت;
	حالت SCX_OPSS_QUEUED:
		/*
		 * If qseq doesn't match, @p has gone through at least one
		 * dispatch/dequeue and re-enqueue cycle between
		 * scx_bpf_dsq_insert() and here and we have no claim on it.
		 */
		اگر ((opss & SCX_OPSS_QSEQ_MASK) != qseq_at_dispatch)
			بازگشت;

		/* see SCX_EV_INSERT_NOT_OWNED definition */
		اگر (unlikely(!scx_task_on_sched(sch, p))) {
			__scx_add_event(sch, SCX_EV_درج_NOT_OWNED, 1);
			بازگشت;
		}

		/*
		 * While we know @p is accessible, we don't yet have a claim on
		 * it - the BPF scheduler is allowed to dispatch tasks
		 * spuriously and there can be a racing dequeue attempt. Let's
		 * claim @p by atomically transitioning it from QUEUED to
		 * DISPATCHING.
		 */
		اگر (likely(atomic_long_try_cmpxchg(&p->scx.ops_state, &opss,
						   SCX_OPSS_DISPATCHING)))
			بشکن;
		برو retry;
	حالت SCX_OPSS_QUEUEING:
		/*
		 * scx_do_enqueue_task() is in the process of transferring the
		 * task to the BPF scheduler while holding @p's rq lock. As we
		 * aren't holding any kernel or BPF resource that the enqueue
		 * path may depend upon, it's safe to wait.
		 */
		wait_ops_state(p, opss);
		برو retry;
	}

	BUG_ON(!(p->scx.flags & SCX_TASK_QUEUED));

	dsq = find_dsq_for_dispatch(sch, rq, dsq_id, task_cpu(p));

	اگر (dsq->id == SCX_DSQ_LOCAL)
		dispatch_to_local_dsq(sch, rq, dsq, p, slice, vtime, enq_flags);
	وگرنه
		scx_dispatch_enqueue(sch, rq, dsq, p, slice, vtime,
				     enq_flags | SCX_ENQ_APPLY_SLICE | SCX_ENQ_CLEAR_OPSS);
}

تهی scx_flush_dispatch_buf(ساختار scx_sched *sch, ساختار rq *rq)
{
	ساختار scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	u32 u;

	برای (u = 0; u < dspc->cursor; u++) {
		ساختار scx_dsp_buf_ent *ent = &dspc->buf[u];

		finish_dispatch(sch, rq, ent->task, ent->qseq, ent->dsq_id,
				ent->slice, ent->vtime, ent->enq_flags);
	}

	dspc->nr_tasks += dspc->cursor;
	dspc->cursor = 0;
}

ایستا درونخطی تهی maybe_queue_balance_callback(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);

	اگر (!(rq->scx.flags & SCX_RQ_BAL_CB_PENDING))
		بازگشت;

	queue_balance_callback(rq, &rq->scx.deferred_bal_cb,
				deferred_bal_cb_workfn);

	rq->scx.flags &= ~SCX_RQ_BAL_CB_PENDING;
}

ایستا شمارش scx_dsp_verdict dispatch_one(ساختار rq *rq, ساختار task_struct *prev)
{
	ساختار scx_sched *ریشه_sch = scx_ریشه_protected_live();
	شمارش scx_dsp_verdict verdict;
	s32 cpu = cpu_of(rq);

	lockdep_assert_rq_held(rq);
	rq->scx.flags |= SCX_RQ_IN_DISPATCH;

	scx_process_sync_ecaps(rq, prev);

	اگر ((ریشه_sch->ops.flags & SCX_OPS_HAS_CPU_PREEMPT) &&
	    unlikely(rq->scx.cpu_released)) {
		/*
		 * If the previous sched_class for the current CPU was not SCX,
		 * notify the BPF scheduler that it again has control of the
		 * core. This callback complements ->cpu_release(), which is
		 * emitted in switch_class().
		 */
		اگر (ریشه_sch->ops.cpu_acquire)
			SCX_CALL_OP(ریشه_sch, cpu_acquire, rq, cpu, پوچ);
		rq->scx.cpu_released = false;
	}

	اگر (prev->sched_class == &ext_sched_class) {
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
		اگر ((prev->scx.flags & SCX_TASK_QUEUED) && prev->scx.slice &&
		    !scx_bypassing(scx_task_sched(prev), cpu)) {
			verdict = SCX_DSP_PREV;
			برو has_tasks;
		}
	}

	/* if there already are tasks to run, nothing to do */
	اگر (rq->scx.local_dsq.nr) {
		verdict = SCX_DSP_LOCAL;
		برو has_tasks;
	}

	verdict = scx_dispatch_sched(ریشه_sch, rq, prev, false);
	اگر (verdict != SCX_DSP_NONE)
		برو has_tasks;

	/*
	 * Didn't find another task to run. Keep running @prev unless its own
	 * scheduler set %SCX_OPS_ENQ_LAST and takes the enqueue instead, see
	 * put_prev_task_scx(). Read the scheduler here as the dispatch above
	 * may have dropped the rq lock while @prev changed class or scheduler.
	 */
	اگر (prev->scx.flags & SCX_TASK_QUEUED) {
		ساختار scx_sched *prev_sch = scx_task_sched(prev);

		اگر ((!(prev_sch->ops.flags & SCX_OPS_ENQ_LAST) ||
		     scx_bypassing(prev_sch, cpu)) && scx_task_can_stay_on_cpu(rq, prev)) {
			__scx_add_event(prev_sch, SCX_EV_DISPATCH_KEEP_LAST, 1);
			verdict = SCX_DSP_PREV;
			برو has_tasks;
		}
	}
	rq->scx.flags &= ~SCX_RQ_IN_DISPATCH;
	بازگشت SCX_DSP_NONE;

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
	اگر (unlikely(rq->scx.local_dsq.nr > 1 && rq->scx.nr_immed))
		scx_schedule_reenq_local(rq, 0);

	rq->scx.flags &= ~SCX_RQ_IN_DISPATCH;
	بازگشت verdict;
}

ایستا تهی set_next_task_scx(ساختار rq *rq, ساختار task_struct *p, شمارش snt_e type)
{
	ساختار scx_sched *sch = scx_task_sched(p);

	اگر (type == SNT_REPICK)
		بازگشت;

	اگر (p->scx.flags & SCX_TASK_QUEUED) {
		/*
		 * Core-sched might decide to execute @p before it is
		 * dispatched. Call ops_dequeue() to notify the BPF scheduler.
		 */
		ops_dequeue(rq, p, SCX_DEQ_CORE_SCHED_EXEC);
		scx_dispatch_dequeue(rq, p);
	}

	p->se.exec_start = rq_clock_task(rq);

	/* see dequeue_task_scx() on why we skip when !QUEUED */
	اگر (SCX_HAS_OP(sch, running) && (p->scx.flags & SCX_TASK_QUEUED))
		SCX_CALL_OP_TASK(sch, running, rq, p);

	clr_task_runnable(p, true);

	/* apply any pending out-of-band slice request before the tick decision */
	apply_task_slice_oob(rq, p);

	/*
	 * @p is getting newly scheduled or got kicked after someone updated its
	 * slice. Update SCX_RQ_CAN_STOP_TICK to reflect whether the tick can be
	 * stopped. See scx_can_stop_tick().
	 *
	 * Moreover, refresh the load_avgs just when transitioning in and out of
	 * nohz. In the future, we might want to add a mechanism to update
	 * load_avgs periodically on tick-stopped CPUs.
	 */
	اگر (p->scx.slice == SCX_SLICE_INF) {
		اگر (!(rq->scx.flags & SCX_RQ_CAN_STOP_TICK)) {
			/*
			 * Bypass mode always assigns finite slices, so @p
			 * can't have an infinite slice while bypassing.
			 * Therefore, sched_update_tick_dependency() can safely
			 * evaluate the outgoing task.
			 */
			rq->scx.flags |= SCX_RQ_CAN_STOP_TICK;
			sched_update_tick_dependency(rq);

			update_other_load_avgs(rq);
		}
	} وگرنه {
		اگر (rq->scx.flags & SCX_RQ_CAN_STOP_TICK) {
			rq->scx.flags &= ~SCX_RQ_CAN_STOP_TICK;
			update_other_load_avgs(rq);
		}

		/*
		 * @rq still references the outgoing scheduling context. A finite
		 * slice is sufficient by itself to require the tick.
		 */
		اگر (tick_nohz_full_cpu(cpu_of(rq)))
			tick_nohz_dep_set_cpu(cpu_of(rq), TICK_DEP_BIT_SCHED);
	}
}

تهی scx_proxy_donor_start(ساختار rq *rq)
{
}

ایستا شمارش scx_cpu_preempt_reason
preempt_reason_from_class(ثابت ساختار sched_class *class)
{
	اگر (class == &stop_sched_class)
		بازگشت SCX_CPU_PREEMPT_STOP;
	اگر (class == &dl_sched_class)
		بازگشت SCX_CPU_PREEMPT_DL;
	اگر (class == &rt_sched_class)
		بازگشت SCX_CPU_PREEMPT_RT;
	بازگشت SCX_CPU_PREEMPT_UNKNOWN;
}

ایستا تهی switch_class(ساختار rq *rq, ساختار task_struct *next)
{
	ساختار scx_sched *sch = scx_ریشه_protected_live();
	ثابت ساختار sched_class *next_class = next->sched_class;

	اگر (!(sch->ops.flags & SCX_OPS_HAS_CPU_PREEMPT))
		بازگشت;

	/*
	 * The callback is conceptually meant to convey that the CPU is no
	 * longer under the control of SCX. Therefore, don't invoke the callback
	 * if the next class is below SCX (in which case the BPF scheduler has
	 * actively decided not to schedule any tasks on the CPU).
	 */
	اگر (sched_class_above(&ext_sched_class, next_class))
		بازگشت;

	/*
	 * At this point we know that SCX was preempted by a higher priority
	 * sched_class, so invoke the ->cpu_release() callback if we have not
	 * done so already. We only send the callback once between SCX being
	 * preempted, and it regaining control of the CPU.
	 *
	 * ->cpu_release() complements ->cpu_acquire(), which is emitted the
	 *  next time that dispatch_one() is invoked.
	 */
	اگر (!rq->scx.cpu_released) {
		اگر (sch->ops.cpu_release) {
			ساختار scx_cpu_release_args args = {
				.reason = preempt_reason_from_class(next_class),
				.task = next,
			};

			SCX_CALL_OP(sch, cpu_release, rq, cpu_of(rq), &args);
		}
		rq->scx.cpu_released = true;
	}
}

ایستا تهی put_prev_task_scx(ساختار rq *rq, ساختار task_struct *p,
			      ساختار task_struct *next)
{
	ساختار scx_sched *sch = scx_task_sched(p);
	bool rescue_keep = false;

	/* see kick_sync_wait_bal_cb() */
	smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);

	update_curr_scx(rq);

	/*
	 * If the slice is consumed, protection ends with it. A rescuee
	 * preempted beforehand keeps going, see scx_rescue_keep().
	 */
	اگر (!p->scx.slice) {
		اگر (unlikely(p == scx_rescuee(rq)))
			rescue_keep = scx_rescue_keep(rq, p);
		اگر (!rescue_keep)
			scx_task_slice_ended(rq, p);
	}

	/* see dequeue_task_scx() on why we skip when !QUEUED */
	اگر (SCX_HAS_OP(sch, stopping) && (p->scx.flags & SCX_TASK_QUEUED))
		SCX_CALL_OP_TASK(sch, stopping, rq, p, true);

	اگر (p->scx.flags & SCX_TASK_QUEUED) {
		set_task_runnable(rq, p);

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
		اگر ((p->scx.slice || unlikely(p == scx_rescuee(rq))) &&
		    !scx_bypassing(sch, cpu_of(rq))) {
			اگر (p->scx.flags & SCX_TASK_IMMED) {
				p->scx.flags |= SCX_TASK_REENQ_PREEMPTED;
				scx_do_enqueue_task(rq, p, SCX_ENQ_REENQ, -1);
				p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
			} وگرنه {
				u64 enq_flags = 0;

				/*
				 * Keep a preempted rescue going. If preempted
				 * by another SCX task, append to the local DSQ,
				 * see scx_rescue_keep().
				 */
				اگر (unlikely(p == scx_rescuee(rq))) {
					enq_flags |= SCX_ENQ_IGNORE_CAPS;
					اگر (!rescue_keep)
						enq_flags |= SCX_ENQ_HEAD;
				} وگرنه {
					enq_flags |= SCX_ENQ_HEAD;
				}

				scx_dispatch_enqueue(sch, rq, &rq->scx.local_dsq, p, 0, 0,
						     enq_flags);
			}
			برو switch_class;
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
		اگر (next && sched_class_above(&ext_sched_class, next->sched_class) &&
		    scx_task_can_stay_on_cpu(rq, p)) {
			WARN_ON_ONCE(!sched_core_enabled(rq) &&
				     !(sch->ops.flags & SCX_OPS_ENQ_LAST));
			scx_do_enqueue_task(rq, p, SCX_ENQ_LAST, -1);

			/*
			 * A task put on the local DSQ runs, as anywhere else.
			 * Here the insert can't reschedule on its own: the pick
			 * has settled on @next and @p is still curr, so
			 * resched_curr() would flag @p and __schedule() clears
			 * that right after the pick. Flag @next instead.
			 */
			اگر (p->scx.dsq == &rq->scx.local_dsq)
				set_tsk_need_resched(next);
		} وگرنه {
			scx_do_enqueue_task(rq, p, 0, -1);
		}
	}

switch_class:
	اگر (next && next->sched_class != &ext_sched_class)
		switch_class(rq, next);
}

ایستا تهی kick_sync_wait_bal_cb(ساختار rq *rq)
{
	ساختار scx_kick_syncs __rcu *ks;
	unsigned long *ksyncs;
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
	اگر (unlikely(cpu_of(rq) != smp_processor_id()))
		بازگشت;

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
	for_each_cpu(cpu, rq->scx.cpus_to_sync) {
		/*
		 * smp_load_acquire() pairs with smp_store_release() on
		 * kick_sync updates on the target CPUs.
		 */
		اگر (cpu == cpu_of(rq) ||
		    smp_load_acquire(&cpu_rq(cpu)->scx.kick_sync) != ksyncs[cpu]) {
			cpumask_clear_cpu(cpu, rq->scx.cpus_to_sync);
			ادامه;
		}

		scx_rq_lock_drop(rq);
		raw_spin_rq_unlock_irq(rq);
		وقتی (READ_ONCE(cpu_rq(cpu)->scx.kick_sync) == ksyncs[cpu]) {
			smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);
			cpu_relax();
		}
		raw_spin_rq_lock_irq(rq);
		waited = true;
	}

	اگر (waited)
		برو retry;
}

ایستا ساختار task_struct *first_local_task(ساختار rq *rq)
{
	بازگشت list_first_entry_or_null(&rq->scx.local_dsq.list,
					ساختار task_struct, scx.dsq_list.گره);
}

/*
 * Run dispatch and queue the follow-up work for a pick.
 */
ایستا شمارش scx_dsp_verdict dispatch_pick(ساختار rq *rq, ساختار rq_flags *rf,
					  ساختار task_struct *prev)
{
	شمارش scx_dsp_verdict verdict;

	rq_unpin_lock(rq, rf);
	verdict = dispatch_one(rq, prev);
	rq_repin_lock(rq, rf);
	maybe_queue_balance_callback(rq);

	/*
	 * Defer to a balance callback which can drop rq lock and enable IRQs.
	 * Waiting directly in the pick path would deadlock against CPUs sending
	 * us IPIs (e.g. TLB flushes) while we wait for them.
	 */
	اگر (unlikely(rq->scx.kick_sync_pending)) {
		rq->scx.kick_sync_pending = false;
		queue_balance_callback(rq, &rq->scx.kick_sync_bal_cb,
				       kick_sync_wait_bal_cb);
	}

	بازگشت verdict;
}

#ifdef CONFIG_SCHED_CORE
/*
 * Dispatch for a pick when core scheduling is enabled. The selection picks for
 * all SMT siblings and the rq_i->core_pick state it builds must stay atomic
 * throughout. If the dispatch released the rq lock, anything can have happened
 * in between - return %SCX_DSP_RETRY to restart the selection against current
 * state.
 */
ایستا شمارش scx_dsp_verdict dispatch_core_pick(ساختار rq *rq, ساختار rq_flags *rf,
					       ساختار task_struct *prev)
{
	شمارش scx_dsp_verdict verdict;
	u32 seq = rq->scx.lock_drop_seq;

	/* another dispatch is in flight on @rq, let that handle it */
	اگر (rq->scx.flags & SCX_RQ_IN_DISPATCH)
		بازگشت SCX_DSP_NONE;

	rq_unpin_lock(rq, rf);

	verdict = dispatch_one(rq, prev);

	اگر (cpu_of(rq) == smp_processor_id()) {
		maybe_queue_balance_callback(rq);

		/* see dispatch_pick() */
		اگر (unlikely(rq->scx.kick_sync_pending)) {
			rq->scx.kick_sync_pending = false;
			queue_balance_callback(rq, &rq->scx.kick_sync_bal_cb,
					       kick_sync_wait_bal_cb);
		}
	} وگرنه اگر (unlikely(rq->scx.flags & SCX_RQ_BAL_CB_PENDING)) {
		/*
		 * Balance callbacks must run in the context that queued them,
		 * so they can't be queued on another CPU's rq. Run the deferred
		 * work directly instead.
		 */
		rq->scx.flags &= ~SCX_RQ_BAL_CB_PENDING;
		run_deferred(rq);
	}

	rq_repin_lock(rq, rf);

	/* if dispatch_one() released the rq lock, restart the selection */
	اگر (rq->scx.lock_drop_seq != seq)
		بازگشت SCX_DSP_RETRY;

	بازگشت verdict;
}
#وگرنه	/* CONFIG_SCHED_CORE */
ایستا شمارش scx_dsp_verdict dispatch_core_pick(ساختار rq *rq, ساختار rq_flags *rf,
					       ساختار task_struct *prev)
{
	بازگشت SCX_DSP_NONE;
}
#endif	/* CONFIG_SCHED_CORE */

ایستا ساختار task_struct *
do_pick_task_scx(ساختار rq *rq, ساختار rq_flags *rf, bool force_scx)
{
	ساختار task_struct *prev = rq->curr;
	شمارش scx_dsp_verdict verdict;
	ساختار task_struct *p;

	/* see kick_sync_wait_bal_cb() */
	smp_store_release(&rq->scx.kick_sync, rq->scx.kick_sync + 1);

	rq_modified_begin(rq, &ext_sched_class);

	اگر (sched_core_enabled(rq))
		verdict = dispatch_core_pick(rq, rf, prev);
	وگرنه
		verdict = dispatch_pick(rq, rf, prev);

	اگر (verdict == SCX_DSP_RETRY)
		بازگشت RETRY_TASK;

	/*
	 * If any higher-priority sched class enqueued a runnable task on this
	 * rq during dispatch_one(), abort and return RETRY_TASK, so that the
	 * scheduler loop can restart.
	 *
	 * If @force_scx is true, always try to pick a SCHED_EXT task,
	 * regardless of any higher-priority sched classes activity.
	 */
	اگر (!force_scx && rq_modified_above(rq, &ext_sched_class))
		بازگشت RETRY_TASK;

	/*
	 * If we're keeping @prev, replenish slice if necessary and keep running
	 * @prev. Otherwise, pop the first one from the local DSQ.
	 */
	اگر (verdict == SCX_DSP_PREV) {
		p = prev;
		اگر (!p->scx.slice) {
			/* the slice is consumed, protection ends */
			scx_task_slice_ended(rq, p);
			refill_task_slice_dfl(scx_task_sched(p), p);
		}
	} وگرنه {
		p = first_local_task(rq);
		اگر (!p)
			بازگشت پوچ;

		اگر (unlikely(!p->scx.slice) && scx_task_can_stay_on_cpu(rq, p)) {
			ساختار scx_sched *sch = scx_task_sched(p);

			اگر (!scx_bypassing(sch, cpu_of(rq)) &&
			    !sch->warned_zero_slice) {
				printk_deferred(KERN_WARNING "sched_ext: %s[%d] has zero slice in %s()\n",
						p->comm, p->pid, __func__);
				sch->warned_zero_slice = true;
			}
			refill_task_slice_dfl(sch, p);
		}
	}

	بازگشت p;
}

ایستا ساختار task_struct *pick_task_scx(ساختار rq *rq, ساختار rq_flags *rf)
{
	بازگشت do_pick_task_scx(rq, rf, false);
}

/*
 * Select the next task to run from the ext scheduling class.
 *
 * Use do_pick_task_scx() directly with @force_scx enabled, since the
 * dl_server must always select a sched_ext task.
 */
ایستا ساختار task_struct *
ext_server_pick_task(ساختار sched_dl_entity *dl_se, ساختار rq_flags *rf)
{
	اگر (!scx_enabled())
		بازگشت پوچ;

	بازگشت do_pick_task_scx(dl_se->rq, rf, true);
}

/*
 * Initialize the ext server deadline entity.
 */
تهی ext_server_init(ساختار rq *rq)
{
	ساختار sched_dl_entity *dl_se = &rq->ext_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, ext_server_pick_task);
}

#ifdef CONFIG_SCHED_CORE
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
bool scx_prio_less(ثابت ساختار task_struct *a, ثابت ساختار task_struct *b,
		   bool in_fi)
{
	ساختار scx_sched *sch_a = scx_task_sched(a);
	ساختار scx_sched *sch_b = scx_task_sched(b);
	ساختار scx_sched *sch = پوچ;
	bool a_running, b_running;

	اگر (sch_a == sch_b) {
		اگر (SCX_HAS_OP(sch_a, core_sched_before))
			sch = sch_a;
	} وگرنه {
		s32 level;

		برای (level = min(sch_a->level, sch_b->level); level >= 0; level--) {
			ساختار scx_sched *anc = sch_a->ancestors[level];

			اگر (anc == sch_b->ancestors[level] &&
			    SCX_HAS_OP(anc, core_sched_before)) {
				sch = anc;
				بشکن;
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
	اگر (sch && !scx_bypassing(sch, task_cpu(a)) && !scx_bypassing(sch, task_cpu(b)))
		بازگشت SCX_CALL_OP_2TASKS_RET(sch, core_sched_before, task_rq(a),
					      (ساختار task_struct *)b,
					      (ساختار task_struct *)a);

	/*
	 * runnable_at is refreshed only on enqueue, so a task which keeps
	 * occupying its CPU carries a stale stamp. A running task is the most
	 * recently serviced whatever its stamp says. Order it after every
	 * waiting task.
	 */
	a_running = a->on_cpu;
	b_running = b->on_cpu;
	اگر (a_running != b_running)
		بازگشت a_running;

	بازگشت time_after(a->scx.runnable_at, b->scx.runnable_at);
}
#endif	/* CONFIG_SCHED_CORE */

ایستا صحیح select_task_rq_scx(ساختار task_struct *p, صحیح prev_cpu, صحیح wake_flags)
{
	ساختار scx_sched *sch = scx_task_sched(p);
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
	اگر (unlikely(wake_flags & WF_EXEC))
		بازگشت prev_cpu;

	bypassing = scx_bypassing(sch, task_cpu(p));
	اگر (likely(SCX_HAS_OP(sch, select_cpu)) && !bypassing) {
		s32 cpu;
		ساختار task_struct **ddsp_taskp;

		ddsp_taskp = this_cpu_ptr(&direct_dispatch_task);
		WARN_ON_ONCE(*ddsp_taskp);
		*ddsp_taskp = p;

		this_rq()->scx.in_select_cpu = true;
		cpu = SCX_CALL_OP_TASK_RET(sch, select_cpu, پوچ, p,
					   scx_cpu_arg(prev_cpu), wake_flags);
		cpu = scx_cpu_ret(sch, cpu);
		this_rq()->scx.in_select_cpu = false;
		p->scx.selected_cpu = cpu;
		*ddsp_taskp = پوچ;
		اگر (scx_cpu_valid(sch, cpu, "from ops.select_cpu()"))
			بازگشت cpu;
		وگرنه
			بازگشت prev_cpu;
	} وگرنه {
		s32 cpu;

		/*
		 * While bypassing, the enqueue path routes @p to a bypass DSQ
		 * without consulting the direct-dispatch target, making the
		 * default selection pointless. It doesn't work anyway when the
		 * scheduler does its own idle tracking and the built-in idle
		 * cpumasks are not updated. Leave @p on @prev_cpu.
		 */
		اگر (bypassing) {
			__scx_add_event(sch, SCX_EV_BYPASS_DISPATCH, 1);
			p->scx.selected_cpu = prev_cpu;
			بازگشت prev_cpu;
		}

		cpu = scx_select_cpu_dfl(p, prev_cpu, wake_flags, پوچ, 0);
		اگر (cpu >= 0) {
			/*
			 * Carry the slice refill and let the insertion commit
			 * it under rq lock. See the write rules.
			 */
			__scx_add_event(sch, SCX_EV_REFILL_SLICE_DFL, 1);
			p->scx.ddsp_slice = READ_ONCE(sch->slice_dfl);
			p->scx.ddsp_enq_flags = SCX_ENQ_SLICE_DFL;
			p->scx.ddsp_dsq_id = SCX_DSQ_LOCAL;
		} وگرنه {
			cpu = prev_cpu;
		}
		p->scx.selected_cpu = cpu;

		بازگشت cpu;
	}
}

ایستا تهی task_woken_scx(ساختار rq *rq, ساختار task_struct *p)
{
	run_deferred(rq);
}

ایستا تهی set_cpus_allowed_scx(ساختار task_struct *p,
				 ساختار affinity_context *ac)
{
	ساختار scx_sched *sch = scx_task_sched(p);

	set_cpus_allowed_common(p, ac);

	اگر (task_dead_and_done(p))
		بازگشت;

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
	اگر (SCX_HAS_OP(sch, set_cpumask) &&
	    (!scx_is_cid_type() || scx_get_task_state(p) == SCX_TASK_ENABLED))
		scx_call_op_set_cpumask(sch, task_rq(p), p, (ساختار cpumask *)p->cpus_ptr);
}

ایستا تهی handle_hotplug(ساختار rq *rq, bool online)
{
	ساختار scx_sched *sch = scx_ریشه_protected();
	s32 cpu = cpu_of(rq);
	s32 cpu_or_cid = cpu;

	atomic_long_inc(&scx_hotplug_seq);

	/*
	 * scx_root updates are protected by cpus_read_lock() and will stay
	 * stable here. Note that we can't depend on scx_enabled() test as the
	 * hotplug ops need to be enabled before __scx_enabled is set.
	 */
	اگر (unlikely(!sch))
		بازگشت;

	اگر (scx_enabled())
		scx_idle_update_selcpu_topology(&sch->ops);

	اگر (online)
		scx_online_ecaps(rq);
	وگرنه
		scx_offline_ecaps(rq);

	/*
	 * The tables can't be retired while this function is running as the
	 * retirement is inside cpus_read_lock. However, scx_cpu_arg() is
	 * awkward here as the tables can be NULL after root enable failure and
	 * lockdep would trigger without surrounding rcu_read_lock(). Open code
	 * the translation. If the table is NULL, the ops are also cleared and
	 * @cpu_or_cid goes unused.
	 */
	اگر (scx_is_cid_type()) {
		s16 *tbl = rcu_dereference_check(scx_cpu_to_cid_tbl,
						 lockdep_is_cpus_held());

		اگر (tbl) {
			ساختار scx_sched *pos;

			cpu_or_cid = tbl[cpu];

			guard(raw_spinlock_irqsave)(&scx_sched_lock);
			list_for_each_entry(pos, &scx_sched_all, all) {
				ساختار scx_cmask *mask = pos->online_cmask;

				اگر (mask)
					__assign_bit(cpu_or_cid, (unsigned long *)mask->bits,
						     online);
			}
		}
	}

	اگر (online && SCX_HAS_OP(sch, cpu_online))
		SCX_CALL_OP(sch, cpu_online, پوچ, cpu_or_cid);
	وگرنه اگر (!online && SCX_HAS_OP(sch, cpu_offline))
		SCX_CALL_OP(sch, cpu_offline, پوچ, cpu_or_cid);
	وگرنه
		scx_exit(sch, SCX_EXIT_UNREG_KERN,
			 SCX_ECODE_ACT_RESTART | SCX_ECODE_RSN_HOTPLUG,
			 "cpu %d going %s, exiting scheduler", cpu,
			 online ? "online" : "offline");
}

تهی scx_rq_activate(ساختار rq *rq)
{
	handle_hotplug(rq, true);
}

تهی scx_rq_deactivate(ساختار rq *rq)
{
	handle_hotplug(rq, false);
}

ایستا تهی rq_online_scx(ساختار rq *rq)
{
	rq->scx.flags |= SCX_RQ_ONLINE;
}

ایستا تهی rq_offline_scx(ساختار rq *rq)
{
	ساختار task_struct *p, *n;

	rq->scx.flags &= ~SCX_RQ_ONLINE;

	/* sched domain rebuilds call rq_offline with the CPU staying alive */
	اگر (cpu_active(cpu_of(rq)))
		بازگشت;

	scx_rescue_flush(rq);

	/*
	 * An offline CPU no longer calls ops.dispatch(). Re-enqueue its tasks
	 * onto the local DSQ so that they run here and balance_push() moves
	 * them off.
	 */
	list_for_each_entry_safe_reverse(p, n, &rq->scx.runnable_list, scx.runnable_گره) {
		اگر (p->scx.dsq == &rq->scx.local_dsq)
			ادامه;
		guard(sched_change)(p, DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK);
	}
}

ایستا bool check_rq_for_timeouts(ساختار rq *rq)
{
	ساختار scx_sched *sch;
	ساختار task_struct *p;
	ساختار rq_flags rf;
	bool timed_out = false;

	rq_lock_irqsave(rq, &rf);
	sch = rcu_dereference_bh(scx_ریشه);
	اگر (unlikely(!sch))
		برو out_unlock;

	list_for_each_entry(p, &rq->scx.runnable_list, scx.runnable_گره) {
		ساختار scx_sched *sch = scx_task_sched(p);
		unsigned long last_runnable = p->scx.runnable_at;

		اگر (unlikely(time_after(jiffies,
					last_runnable + READ_ONCE(sch->watchdog_timeout)))) {
			ساختار scx_dispatch_q *dsq = READ_ONCE(p->scx.dsq);
			u32 dur_ms = jiffies_to_msecs(jiffies - last_runnable);

			/*
			 * A task can be stuck on a DSQ that a sched other than
			 * its owner is responsible for draining, e.g. an
			 * ancestor's bypass DSQ while the owner is bypassing.
			 * Blame the drainer. The local DSQ is consumed by the
			 * cpu itself and keeps blame on the owner.
			 */
			اگر (dsq && dsq->sched && dsq->id != SCX_DSQ_LOCAL)
				sch = dsq->sched;

			__scx_exit(sch, SCX_EXIT_ERROR_STALL, 0, cpu_of(rq),
				   "%s[%d] failed to run for %u.%03us",
				   p->comm, p->pid, dur_ms / 1000,
				   dur_ms % 1000);
			timed_out = true;
			بشکن;
		}
	}
out_unlock:
	rq_unlock_irqrestore(rq, &rf);
	بازگشت timed_out;
}

ایستا تهی scx_watchdog_workfn(ساختار work_struct *work)
{
	unsigned long intv;
	صحیح cpu;

	WRITE_ONCE(scx_watchdog_timestamp, jiffies);

	for_each_online_cpu(cpu) {
		اگر (unlikely(check_rq_for_timeouts(cpu_rq(cpu))))
			بشکن;

		cond_resched();
	}

	intv = READ_ONCE(scx_watchdog_interval);
	اگر (intv < ULONG_MAX)
		queue_delayed_work(system_dfl_wq, to_delayed_work(work), intv);
}

تهی scx_tick(ساختار rq *rq)
{
	ساختار scx_sched *ریشه;
	unsigned long last_check;

	اگر (!scx_enabled())
		بازگشت;

	ریشه = rcu_dereference_bh(scx_ریشه);
	اگر (unlikely(!ریشه))
		بازگشت;

	last_check = READ_ONCE(scx_watchdog_timestamp);
	اگر (unlikely(time_after(jiffies,
				last_check + READ_ONCE(ریشه->watchdog_timeout)))) {
		u32 dur_ms = jiffies_to_msecs(jiffies - last_check);

		scx_exit(ریشه, SCX_EXIT_ERROR_STALL, 0,
			 "watchdog failed to check in for %u.%03us",
			 dur_ms / 1000, dur_ms % 1000);
	}

	update_other_load_avgs(rq);
}

ایستا تهی task_tick_scx(ساختار rq *rq, ساختار task_struct *curr, صحیح queued)
{
	ساختار scx_sched *sch = scx_task_sched(curr);

	update_curr_scx(rq);

	/*
	 * While disabling, always resched as we can't trust the slice
	 * management.
	 */
	اگر (scx_bypassing(sch, cpu_of(rq)))
		scx_set_task_slice(curr, 0);
	وگرنه اگر (SCX_HAS_OP(sch, tick))
		SCX_CALL_OP_TASK(sch, tick, rq, curr);

	اگر (!curr->scx.slice)
		resched_curr(rq);
}

#ifdef CONFIG_EXT_GROUP_SCHED
ایستا ساختار cgroup *tg_cgrp(ساختار task_group *tg)
{
	/*
	 * If CGROUP_SCHED is disabled, @tg is NULL. If @tg is an autogroup,
	 * @tg->css.cgroup is NULL. In both cases, @tg can be treated as the
	 * root cgroup.
	 */
	اگر (tg && tg->css.cgroup)
		بازگشت tg->css.cgroup;
	وگرنه
		بازگشت &cgrp_dfl_ریشه.cgrp;
}

#define SCX_INIT_TASK_ARGS_CGROUP(cgrp)		.cgroup = (cgrp),

#وگرنه	/* CONFIG_EXT_GROUP_SCHED */

#define SCX_INIT_TASK_ARGS_CGROUP(cgrp)

#endif	/* CONFIG_EXT_GROUP_SCHED */

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
صحیح __scx_init_task(ساختار scx_sched *sch, ساختار task_struct *p,
		    ساختار cgroup *cgrp, bool fork)
{
	صحیح ret;

	p->scx.disallow = false;

	اگر (SCX_HAS_OP(sch, init_task)) {
		ساختار scx_init_task_args args = {
			SCX_INIT_TASK_ARGS_CGROUP(cgrp ?: tg_cgrp(task_group(p)))
			.fork = fork,
		};

		ret = SCX_CALL_OP_RET(sch, init_task, پوچ, p, &args);
		اگر (unlikely(ret)) {
			ret = scx_ops_sanitize_err(sch, "init_task", ret);
			بازگشت ret;
		}
	}

	اگر (p->scx.disallow) {
		اگر (unlikely(scx_parent(sch))) {
			scx_error(sch, "non-root ops.init_task() set task->scx.disallow for %s[%d]",
				  p->comm, p->pid);
		} وگرنه اگر (unlikely(fork)) {
			scx_error(sch, "ops.init_task() set task->scx.disallow for %s[%d] during fork",
				  p->comm, p->pid);
		} وگرنه اگر (unlikely(scx_enable_state() != SCX_ENABLING)) {
			scx_error(sch, "ops.init_task() set task->scx.disallow for %s[%d] outside the enable path",
				  p->comm, p->pid);
		} وگرنه {
			ساختار rq *rq;
			ساختار rq_flags rf;

			rq = task_rq_lock(p, &rf);

			/*
			 * We're in the load path and @p->policy will be applied
			 * right after. Reverting @p->policy here and rejecting
			 * %SCHED_EXT transitions from scx_check_setscheduler()
			 * guarantees that if ops.init_task() sets @p->disallow,
			 * @p can never be in SCX.
			 */
			اگر (p->policy == SCHED_EXT) {
				p->policy = SCHED_NORMAL;
				atomic_long_inc(&scx_nr_rejected);
			}

			task_rq_unlock(rq, p, &rf);
		}
	}

	بازگشت 0;
}

ایستا تهی __scx_enable_task(ساختار scx_sched *sch, ساختار task_struct *p)
{
	ساختار rq *rq = task_rq(p);
	u32 weight;

	lockdep_assert_rq_held(rq);

	/*
	 * Verify the task is not in BPF scheduler's custody. If flag
	 * transitions are consistent, the flag should always be clear
	 * here.
	 */
	WARN_ON_ONCE(p->scx.flags & SCX_TASK_IN_CUSTODY);

	/*
	 * Set the weight before calling ops.enable() so that the scheduler
	 * doesn't see a stale value if they inspect the task struct.
	 */
	اگر (task_has_idle_policy(p))
		weight = WEIGHT_IDLEPRIO;
	وگرنه
		weight = sched_prio_to_weight[p->static_prio - MAX_RT_PRIO];

	p->scx.weight = sched_weight_to_cgroup(weight);

	اگر (SCX_HAS_OP(sch, enable)) {
		اگر (scx_is_cid_type()) {
			ساختار scx_cmask *cmask = scx_fill_cmask_scratch(sch, p->cpus_ptr);
			ساختار scx_enable_args args = {
				.cmask_arena_addr = scx_kaddr_to_arena(sch, cmask),
			};

			SCX_CALL_CID_OP_TASK(sch, enable, rq, p, &args);
		} وگرنه {
			SCX_CALL_OP_TASK(sch, enable, rq, p);
		}
	}

	/*
	 * The initial mask also goes out through set_cmask() so a scheduler can
	 * track affinity there alone, and before set_weight() so that the mask
	 * is in place when weight-dependent state is derived, see struct
	 * scx_enable_args.
	 */
	اگر (scx_is_cid_type() && SCX_HAS_OP(sch, set_cpumask))
		scx_call_op_set_cpumask(sch, rq, p, p->cpus_ptr);

	اگر (SCX_HAS_OP(sch, set_weight))
		SCX_CALL_OP_TASK(sch, set_weight, rq, p, p->scx.weight);
}

تهی scx_enable_task(ساختار scx_sched *sch, ساختار task_struct *p)
{
	__scx_enable_task(sch, p);
	scx_set_task_state(p, SCX_TASK_ENABLED);
}

ایستا تهی scx_disable_task(ساختار scx_sched *sch, ساختار task_struct *p)
{
	ساختار rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(scx_get_task_state(p) != SCX_TASK_ENABLED);

	clear_direct_dispatch(p);

	اگر (SCX_HAS_OP(sch, disable))
		SCX_CALL_OP_TASK(sch, disable, rq, p);
	scx_set_task_state(p, SCX_TASK_READY);

	/*
	 * Reset the SCX-managed fields when @p leaves the BPF scheduler's
	 * control, after ops.disable() has observed their final values.
	 */
	p->scx.dsq_vtime = 0;
	scx_task_slice_ended(rq, p);
	scx_set_task_slice(p, 0);
	p->scx.reenq_cnt = 0;

	/*
	 * Verify the task is not in BPF scheduler's custody. If flag
	 * transitions are consistent, the flag should always be clear
	 * here.
	 */
	WARN_ON_ONCE(p->scx.flags & SCX_TASK_IN_CUSTODY);
}

تهی __scx_disable_and_exit_task(ساختار scx_sched *sch, ساختار task_struct *p)
{
	ساختار scx_exit_task_args args = {
		.cancelled = false,
	};

	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(task_rq(p));

	انتخاب (scx_get_task_state(p)) {
	حالت SCX_TASK_NONE:
		بازگشت;
	حالت SCX_TASK_INIT:
		args.cancelled = true;
		بشکن;
	حالت SCX_TASK_READY:
		بشکن;
	حالت SCX_TASK_ENABLED:
		scx_disable_task(sch, p);
		بشکن;
	پیشفرض:
		WARN_ON_ONCE(true);
		بازگشت;
	}

	اگر (SCX_HAS_OP(sch, exit_task))
		SCX_CALL_OP_TASK(sch, exit_task, task_rq(p), p, &args);
}

/*
 * Undo a completed __scx_init_task(sch, p, false) when scx_enable_task() never
 * ran. The task state has not been transitioned, so this mirrors the
 * SCX_TASK_INIT branch in __scx_disable_and_exit_task().
 */
تهی scx_sub_init_cancel_task(ساختار scx_sched *sch, ساختار task_struct *p)
{
	ساختار scx_exit_task_args args = { .cancelled = true };

	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(task_rq(p));

	/* @p was never associated with @sch, dispatch on the explicit @sch */
	اگر (SCX_HAS_OP(sch, exit_task))
		__SCX_CALL_OP_TASK(sch, ops, exit_task, task_rq(p), p, &args);
}

تهی scx_disable_and_exit_task(ساختار scx_sched *sch, ساختار task_struct *p)
{
	__scx_disable_and_exit_task(sch, p);

	/*
	 * If set, @p exited between __scx_init_task() and scx_enable_task() in
	 * scx_sub_enable() and is initialized for both the associated sched and
	 * its parent. Exit for the child too - scx_enable_task() never ran for
	 * it, so undo only init_task. The flag is only set on the sub-enable
	 * path, so it's always clear when @p arrives here in %SCX_TASK_NONE.
	 */
	اگر (p->scx.flags & SCX_TASK_SUB_INIT) {
		اگر (!WARN_ON_ONCE(!scx_enabling_sub_sched))
			scx_sub_init_cancel_task(scx_enabling_sub_sched, p);
		p->scx.flags &= ~SCX_TASK_SUB_INIT;
	}

	scx_set_task_sched(p, پوچ);
	scx_set_task_state(p, SCX_TASK_NONE);
}

تهی init_scx_entity(ساختار sched_ext_entity *scx)
{
	memset(scx, 0, اندازه(*scx));
	INIT_LIST_HEAD(&scx->dsq_list.گره);
	RB_CLEAR_گره(&scx->dsq_priq);
	scx->sticky_cpu = -1;
	scx->holding_cpu = -1;
	scx->runnable_cpu = -1;
	INIT_LIST_HEAD(&scx->runnable_گره);
	scx->runnable_at = jiffies;
	scx->ddsp_dsq_id = SCX_DSQ_INVALID;
	scx->slice = SCX_SLICE_DFL;
}

/* See scx_tid_alloc / scx_tid_cursor. */
ایستا u64 scx_alloc_tid(تهی)
{
	ساختار scx_tid_alloc *ta;

	guard(preempt)();
	ta = this_cpu_ptr(&scx_tid_alloc);

	اگر (unlikely(ta->next >= ta->end)) {
		ta->next = atomic64_fetch_add(SCX_TID_CHUNK, &scx_tid_cursor);
		ta->end = ta->next + SCX_TID_CHUNK;
	}
	بازگشت ta->next++;
}

ایستا تهی scx_tid_hash_درج(ساختار task_struct *p)
{
	صحیح ret;

	lockdep_assert_held(&scx_tasks_lock);

	ret = rhashtable_lookup_درج_fast(&scx_tid_hash,
					    &p->scx.tid_hash_گره,
					    scx_tid_hash_params);
	WARN_ON_ONCE(ret);
}

تهی scx_pre_fork(ساختار task_struct *p)
{
	/*
	 * BPF scheduler enable/disable paths want to be able to iterate and
	 * update all tasks which can become complex when racing forks. As
	 * enable/disable are very cold paths, let's use a percpu_rwsem to
	 * exclude forks.
	 */
	percpu_down_read(&scx_fork_rwsem);
}

صحیح scx_fork(ساختار task_struct *p, ساختار kernel_clone_args *kargs)
{
	s32 ret;

	percpu_rwsem_assert_held(&scx_fork_rwsem);

	p->scx.tid = scx_alloc_tid();

	اگر (scx_init_task_enabled) {
#ifdef CONFIG_EXT_SUB_SCHED
		ساختار scx_sched *sch = scx_cgroup_sched(kargs->cset->dfl_cgrp);
#وگرنه
		ساختار scx_sched *sch = scx_ریشه_protected_live();
#endif
		scx_set_task_state(p, SCX_TASK_INIT_BEGIN);
		ret = __scx_init_task(sch, p, پوچ, true);
		اگر (unlikely(ret)) {
			scx_set_task_state(p, SCX_TASK_NONE);
			بازگشت ret;
		}
		scx_set_task_state(p, SCX_TASK_INIT);
		scx_set_task_sched(p, sch);
	}

	بازگشت 0;
}

تهی scx_post_fork(ساختار task_struct *p)
{
	اگر (scx_init_task_enabled) {
		scx_set_task_state(p, SCX_TASK_READY);

		/*
		 * Enable the task immediately if it's running on sched_ext.
		 * Otherwise, it'll be enabled in switching_to_scx() if and
		 * when it's ever configured to run with a SCHED_EXT policy.
		 */
		اگر (p->sched_class == &ext_sched_class) {
			ساختار rq_flags rf;
			ساختار rq *rq;

			rq = task_rq_lock(p, &rf);
			scx_enable_task(scx_task_sched(p), p);
			task_rq_unlock(rq, p, &rf);
		}
	}

	scoped_guard(raw_spinlock_irq, &scx_tasks_lock) {
		list_add_tail(&p->scx.tasks_گره, &scx_tasks);
		اگر (scx_tid_to_task_enabled())
			scx_tid_hash_درج(p);
	}

	percpu_up_read(&scx_fork_rwsem);
}

تهی scx_cancel_fork(ساختار task_struct *p)
{
	اگر (scx_init_task_enabled) {
		ساختار rq *rq;
		ساختار rq_flags rf;

		rq = task_rq_lock(p, &rf);
		WARN_ON_ONCE(scx_get_task_state(p) >= SCX_TASK_READY);
		scx_disable_and_exit_task(scx_task_sched(p), p);
		task_rq_unlock(rq, p, &rf);
	}

	percpu_up_read(&scx_fork_rwsem);
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
ایستا bool task_dead_and_done(ساختار task_struct *p)
{
	ساختار rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * In do_task_dead(), a dying task sets %TASK_DEAD with preemption
	 * disabled and __schedule(). If @p has %TASK_DEAD set and off CPU, @p
	 * won't ever run again.
	 */
	بازگشت unlikely(READ_ONCE(p->__state) == TASK_DEAD) &&
		!task_on_cpu(rq, p);
}

تهی sched_ext_dead(ساختار task_struct *p)
{
	/*
	 * By the time control reaches here, @p has %TASK_DEAD set, switched out
	 * for the last time and then dropped the rq lock - task_dead_and_done()
	 * should be returning %true nullifying the straggling sched_class ops.
	 * Remove from scx_tasks and exit @p.
	 */
	scoped_guard(raw_spinlock_irqsave, &scx_tasks_lock) {
		list_del_init(&p->scx.tasks_گره);
		اگر (scx_tid_to_task_enabled())
			rhashtable_remove_fast(&scx_tid_hash,
					       &p->scx.tid_hash_گره,
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
	اگر (scx_get_task_state(p) != SCX_TASK_NONE) {
		ساختار rq_flags rf;
		ساختار rq *rq;

		rq = task_rq_lock(p, &rf);
		اگر (scx_get_task_state(p) != SCX_TASK_INIT_BEGIN)
			scx_disable_and_exit_task(scx_task_sched(p), p);
		scx_set_task_state(p, SCX_TASK_DEAD);
		task_rq_unlock(rq, p, &rf);
	}
}

ایستا تهی reweight_task_scx(ساختار rq *rq, ساختار task_struct *p,
			      ثابت ساختار load_weight *lw)
{
	ساختار scx_sched *sch = scx_task_sched(p);

	lockdep_assert_rq_held(task_rq(p));

	اگر (task_dead_and_done(p))
		بازگشت;

	/*
	 * When switching sched_class away from SCX, reweight_task_scx()
	 * is called _after_ scx_disable_task(). Skip calling ops.set_weight()
	 * since the BPF scheduler may have already forgotten the task in
	 * ops.disable().
	 * p->scx.weight will be recalculated in scx_enable_task() if the task
	 * ever returns to SCX class.
	 */
	اگر (scx_get_task_state(p) != SCX_TASK_ENABLED)
		بازگشت;

	p->scx.weight = sched_weight_to_cgroup(scale_load_down(lw->weight));
	اگر (SCX_HAS_OP(sch, set_weight))
		SCX_CALL_OP_TASK(sch, set_weight, rq, p, p->scx.weight);
}

ایستا تهی prio_changed_scx(ساختار rq *rq, ساختار task_struct *p, u64 oldprio)
{
}

ایستا تهی switching_to_scx(ساختار rq *rq, ساختار task_struct *p)
{
	ساختار scx_sched *sch = scx_task_sched(p);

	اگر (task_dead_and_done(p))
		بازگشت;

	scx_enable_task(sch, p);

	/*
	 * set_cpus_allowed_scx() is not called while @p is associated with a
	 * different scheduler class. Keep the BPF scheduler up-to-date. The cid
	 * form gets its mask from scx_enable_task().
	 */
	اگر (!scx_is_cid_type() && SCX_HAS_OP(sch, set_cpumask))
		scx_call_op_set_cpumask(sch, rq, p, (ساختار cpumask *)p->cpus_ptr);
}

ایستا تهی switched_from_scx(ساختار rq *rq, ساختار task_struct *p)
{
	اگر (task_dead_and_done(p))
		بازگشت;

	/*
	 * %NONE means SCX is no longer tracking @p at the task level (e.g.
	 * scx_fail_parent() handed @p back to the parent at NONE pending the
	 * parent's own teardown). There is nothing to disable; calling
	 * scx_disable_task() would WARN on the non-%ENABLED state and trigger a
	 * NONE -> READY validation failure.
	 */
	اگر (scx_get_task_state(p) == SCX_TASK_NONE)
		بازگشت;

	scx_disable_task(scx_task_sched(p), p);
}

ایستا تهی switched_to_scx(ساختار rq *rq, ساختار task_struct *p) {}

صحیح scx_check_setscheduler(ساختار task_struct *p, صحیح policy)
{
	lockdep_assert_rq_held(task_rq(p));

	/* if disallow, reject transitioning into SCX */
	اگر (scx_enabled() && READ_ONCE(p->scx.disallow) &&
	    p->policy != policy && policy == SCHED_EXT)
		بازگشت -EACCES;

	بازگشت 0;
}

ایستا تهی process_ddsp_deferred_locals(ساختار rq *rq)
{
	ساختار task_struct *p;

	lockdep_assert_rq_held(rq);

	/*
	 * Now that @rq can be unlocked, execute the deferred enqueueing of
	 * tasks directly dispatched to the local DSQs of other CPUs. See
	 * direct_dispatch(). Keep popping from the head instead of using
	 * list_for_each_entry_safe() as dispatch_local_dsq() may unlock @rq
	 * temporarily.
	 */
	وقتی ((p = list_first_entry_or_null(&rq->scx.ddsp_deferred_locals,
				ساختار task_struct, scx.dsq_list.گره))) {
		ساختار scx_sched *sch = scx_task_sched(p);
		ساختار scx_dispatch_q *dsq;
		u64 dsq_id = p->scx.ddsp_dsq_id;
		u64 enq_flags = p->scx.ddsp_enq_flags;
		u64 slice = p->scx.ddsp_slice;
		u64 vtime = p->scx.ddsp_vtime;

		list_del_init(&p->scx.dsq_list.گره);
		clear_direct_dispatch(p);

		dsq = find_dsq_for_dispatch(sch, rq, dsq_id, task_cpu(p));
		اگر (!WARN_ON_ONCE(dsq->id != SCX_DSQ_LOCAL))
			dispatch_to_local_dsq(sch, rq, dsq, p, slice, vtime, enq_flags);
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
ایستا bool local_task_should_reenq(ساختار rq *rq, ساختار task_struct *p,
				    u64 *reenq_flags, u32 *reason)
{
	bool first;

	first = !(*reenq_flags & SCX_REENQ_TSR_NOT_FIRST);
	*reenq_flags |= SCX_REENQ_TSR_NOT_FIRST;

	اگر (unlikely((p->scx.flags & SCX_TASK_PROTECTED) || p == scx_rescuee(rq)))
		بازگشت false;

	*reason = SCX_TASK_REENQ_KFUNC;

	اگر ((p->scx.flags & SCX_TASK_IMMED) &&
	    (!first || !(*reenq_flags & SCX_REENQ_TSR_RQ_OPEN))) {
		__scx_add_event(scx_task_sched(p), SCX_EV_REENQ_IMMED, 1);
		*reason = SCX_TASK_REENQ_IMMED;
		بازگشت true;
	}

	اگر ((*reenq_flags & SCX_REENQ_CAP_REVOKE) &&
	    scx_task_reenq_on_cap_revoke(rq, p)) {
		*reason = SCX_TASK_REENQ_CAP;
		بازگشت true;
	}

	بازگشت *reenq_flags & SCX_REENQ_ANY;
}

/*
 * The dispatcher runs ops.dequeue() and stores the final ops_state after
 * dropping the DSQ lock, so @p can be found on a DSQ while still
 * %SCX_OPSS_DISPATCHING. Wait for the dispatch to finish before operating on
 * @p.
 */
تهی scx_wait_dispatching(ساختار task_struct *p)
{
	اگر (unlikely(atomic_long_read_acquire(&p->scx.ops_state) == SCX_OPSS_DISPATCHING))
		wait_ops_state(p, SCX_OPSS_DISPATCHING);
}

ایستا u32 reenq_local(ساختار scx_sched *sch, ساختار rq *rq, u64 reenq_flags)
{
	LIST_HEAD(tasks);
	u32 nr_enqueued = 0;
	ساختار task_struct *p, *n;

	lockdep_assert_rq_held(rq);

	اگر (WARN_ON_ONCE(reenq_flags & __SCX_REENQ_TSR_MASK))
		reenq_flags &= ~__SCX_REENQ_TSR_MASK;
	اگر (rq_is_open(rq, 0))
		reenq_flags |= SCX_REENQ_TSR_RQ_OPEN;

	/*
	 * The BPF scheduler may choose to dispatch tasks back to
	 * @rq->scx.local_dsq. Move all candidate tasks off to a private list
	 * first to avoid processing the same tasks repeatedly.
	 */
	list_for_each_entry_safe(p, n, &rq->scx.local_dsq.list,
				 scx.dsq_list.گره) {
		ساختار scx_sched *task_sch = scx_task_sched(p);
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
		اگر (p->migration_pending)
			ادامه;

		اگر (!scx_is_descendant(task_sch, sch))
			ادامه;

		اگر (!local_task_should_reenq(rq, p, &reenq_flags, &reason))
			ادامه;

		scx_wait_dispatching(p);
		scx_dispatch_dequeue(rq, p);

		اگر (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_MASK))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
		p->scx.flags |= reason;

		list_add_tail(&p->scx.dsq_list.گره, &tasks);
	}

	list_for_each_entry_safe(p, n, &tasks, scx.dsq_list.گره) {
		list_del_init(&p->scx.dsq_list.گره);

		scx_do_enqueue_task(rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
		nr_enqueued++;
	}

	/*
	 * The revoke that scheduled this scan may have raced the pick: curr
	 * may be a now-capless task, either one that kept running or one
	 * promoted off the local DSQ between the ecaps sync and this scan.
	 * Zero the slice to evict it. The enqueue gate blocks new capless
	 * inserts, so no later pick can slip through after the scan.
	 */
	اگر ((reenq_flags & SCX_REENQ_CAP_REVOKE) &&
	    rq->curr->sched_class == &ext_sched_class &&
	    scx_task_reenq_on_cap_revoke(rq, rq->curr)) {
		scx_set_task_slice(rq->curr, 0);
		resched_curr(rq);
	}

	بازگشت nr_enqueued;
}

ایستا تهی process_deferred_reenq_locals(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);

	/*
	 * A task can be re-queued within this loop when a reenqueued task
	 * bounces straight back to the local DSQ. That recursion is bounded by
	 * the per-task reenqueue cap in scx_do_enqueue_task().
	 */
	وقتی (true) {
		ساختار scx_sched *sch;
		u64 reenq_flags;

		scoped_guard (raw_spinlock, &rq->scx.deferred_reenq_lock) {
			ساختار scx_deferred_reenq_local *drl =
				list_first_entry_or_null(&rq->scx.deferred_reenq_locals,
							 ساختار scx_deferred_reenq_local,
							 گره);
			ساختار scx_sched_pcpu *sch_pcpu;

			اگر (!drl)
				بازگشت;

			sch_pcpu = container_of(drl, ساختار scx_sched_pcpu,
						deferred_reenq_local);
			sch = sch_pcpu->sch;

			reenq_flags = drl->flags;
			WRITE_ONCE(drl->flags, 0);
			list_del_init(&drl->گره);
		}

		/* see schedule_dsq_reenq() */
		smp_mb();

		reenq_local(sch, rq, reenq_flags);
	}
}

ایستا bool user_task_should_reenq(ساختار task_struct *p, u64 reenq_flags, u32 *reason)
{
	*reason = SCX_TASK_REENQ_KFUNC;
	بازگشت reenq_flags & SCX_REENQ_ANY;
}

ایستا تهی reenq_user(ساختار rq *rq, ساختار scx_dispatch_q *dsq, u64 reenq_flags)
{
	ساختار rq *locked_rq = rq;
	ساختار scx_sched *sch = dsq->sched;
	ساختار scx_dsq_list_گره cursor = INIT_DSQ_LIST_CURSOR(cursor, dsq, 0);
	ساختار task_struct *p;
	s32 nr_enqueued = 0;

	lockdep_assert_rq_held(rq);

	raw_spin_lock(&dsq->lock);

	وقتی (likely(!READ_ONCE(sch->bypass_depth))) {
		ساختار rq *task_rq;
		u32 reason;

		p = nldsq_cursor_next_task(&cursor, dsq);
		اگر (!p)
			بشکن;

		اگر (!user_task_should_reenq(p, reenq_flags, &reason))
			ادامه;

		task_rq = task_rq(p);

		اگر (locked_rq != task_rq) {
			اگر (locked_rq) {
				scx_rq_lock_drop(locked_rq);
				raw_spin_rq_unlock(locked_rq);
			}
			اگر (unlikely(!raw_spin_rq_trylock(task_rq))) {
				raw_spin_unlock(&dsq->lock);
				raw_spin_rq_lock(task_rq);
				raw_spin_lock(&dsq->lock);
			}
			locked_rq = task_rq;

			/* did we lose @p while switching locks? */
			اگر (nldsq_cursor_lost_task(&cursor, task_rq, dsq, p))
				ادامه;
		}

		/* @p is on @dsq, its rq and @dsq are locked */
		scx_wait_dispatching(p);
		dispatch_dequeue_locked(p, dsq);
		raw_spin_unlock(&dsq->lock);

		اگر (WARN_ON_ONCE(p->scx.flags & SCX_TASK_REENQ_REASON_MASK))
			p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;
		p->scx.flags |= reason;

		scx_do_enqueue_task(task_rq, p, SCX_ENQ_REENQ, -1);

		p->scx.flags &= ~SCX_TASK_REENQ_REASON_MASK;

		اگر (!(++nr_enqueued % SCX_TASK_ITER_BATCH)) {
			scx_rq_lock_drop(locked_rq);
			raw_spin_rq_unlock(locked_rq);
			locked_rq = پوچ;
			cpu_relax();
		}

		raw_spin_lock(&dsq->lock);
	}

	list_del_init(&cursor.گره);
	raw_spin_unlock(&dsq->lock);

	اگر (locked_rq != rq) {
		اگر (locked_rq) {
			scx_rq_lock_drop(locked_rq);
			raw_spin_rq_unlock(locked_rq);
		}
		raw_spin_rq_lock(rq);
	}
}

ایستا تهی process_deferred_reenq_users(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);

	وقتی (true) {
		ساختار scx_dispatch_q *dsq;
		u64 dsq_id, reenq_flags;

		scoped_guard (raw_spinlock, &rq->scx.deferred_reenq_lock) {
			ساختار scx_deferred_reenq_user *dru =
				list_first_entry_or_null(&rq->scx.deferred_reenq_users,
							 ساختار scx_deferred_reenq_user,
							 گره);
			ساختار scx_dsq_pcpu *dsq_pcpu;

			اگر (!dru)
				بازگشت;

			dsq_pcpu = container_of(dru, ساختار scx_dsq_pcpu,
						deferred_reenq_user);
			dsq = dsq_pcpu->dsq;
			reenq_flags = dru->flags;
			WRITE_ONCE(dru->flags, 0);
			list_del_init(&dru->گره);
		}

		/* see schedule_dsq_reenq() */
		smp_mb();

		/* destroy_dsq() may have raced and invalidated @dsq, nothing to reenq */
		dsq_id = READ_ONCE(dsq->id);
		اگر (unlikely(dsq_id == SCX_DSQ_INVALID))
			ادامه;

		BUG_ON(dsq_id & SCX_DSQ_FLAG_BUILTIN);
		reenq_user(rq, dsq, reenq_flags);
	}
}

ایستا تهی run_deferred(ساختار rq *rq)
{
	process_ddsp_deferred_locals(rq);

	اگر (!list_empty(&rq->scx.deferred_reenq_locals))
		process_deferred_reenq_locals(rq);

	اگر (!list_empty(&rq->scx.deferred_reenq_users))
		process_deferred_reenq_users(rq);

	scx_reenq_reject(rq);
}

#ifdef CONFIG_NO_HZ_FULL
bool scx_can_stop_tick(ساختار rq *rq)
{
	ساختار task_struct *p = rq->curr;
	ساختار scx_sched *sch = scx_task_sched(p);

	اگر (p->sched_class != &ext_sched_class)
		بازگشت true;

	/*
	 * @rq->curr may still reference an outgoing EXT task after it has been
	 * dequeued. If no EXT tasks are accounted on @rq, ignore its stale
	 * slice state. If another task is dispatched from a DSQ,
	 * set_next_task_scx() will update the dependency for the incoming task.
	 */
	اگر (!rq->scx.nr_running)
		بازگشت true;

	اگر (scx_bypassing(sch, cpu_of(rq)))
		بازگشت false;

	/*
	 * A running rescuee's charging and expiry are tick-driven, see
	 * scx_rescue_charge(). Keep the tick while rescue is in progress.
	 */
	اگر (unlikely(p == scx_rescuee(rq)))
		بازگشت false;

	/*
	 * @rq can dispatch from different DSQs, so we can't tell whether it
	 * needs the tick or not by looking at nr_running. Allow stopping ticks
	 * iff the BPF scheduler indicated so. See set_next_task_scx().
	 */
	بازگشت rq->scx.flags & SCX_RQ_CAN_STOP_TICK;
}
#endif

#ifdef CONFIG_EXT_GROUP_SCHED

DEFINE_STATIC_PERCPU_RWSEM(scx_cgroup_ops_rwsem);

تهی scx_tg_init(ساختار task_group *tg)
{
	tg->scx.weight = CGROUP_WEIGHT_DFL;
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
ایستا ساختار scx_sched *scx_tg_sched(ساختار task_group *tg)
{
	lockdep_assert(lockdep_is_held(&cgroup_mutex) ||
		       lockdep_is_held(&scx_cgroup_ops_rwsem));

	اگر (!tg->css.cgroup)
		tg = &ریشه_task_group;
	/* INITED means ops.cgroup_init() succeeded on @tg->scx.sched */
	بازگشت (tg->scx.flags & SCX_TG_INITED) ? tg->scx.sched : پوچ;
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
ایستا ساختار scx_sched *scx_tg_knob_sched(ساختار task_group *tg)
{
	lockdep_assert(lockdep_is_held(&cgroup_mutex) ||
		       lockdep_is_held(&scx_cgroup_ops_rwsem));

	اگر (!tg->css.cgroup || !tg->css.parent)
		بازگشت scx_tg_sched(&ریشه_task_group);
	بازگشت scx_tg_sched(css_tg(tg->css.parent));
}

صحیح scx_tg_online(ساختار task_group *tg)
{
	صحیح ret = 0;

	WARN_ON_ONCE(tg->scx.flags & (SCX_TG_ONLINE | SCX_TG_INITED));

	اگر (scx_cgroup_enabled) {
		ساختار scx_sched *sch;

		/*
		 * The cgroup lifetime notifier populates cgrp->scx_sched before
		 * css_online, but only on the default hierarchy. Sub-scheds are
		 * attached to the cgroup2 hierarchy, so a cgroup1 task_group
		 * always belongs to the root sched.
		 */
		اگر (cgroup_on_dfl(tg->css.cgroup))
			sch = scx_cgroup_sched(tg->css.cgroup);
		وگرنه
			sch = scx_tg_sched(&ریشه_task_group);

		اگر (SCX_HAS_OP(sch, cgroup_init)) {
			ساختار scx_cgroup_init_args args =
				{ .weight = tg->scx.weight,
				  .bw_period_us = tg->scx.bw_period_us,
				  .bw_quota_us = tg->scx.bw_quota_us,
				  .bw_burst_us = tg->scx.bw_burst_us,
				  .sched_idle = tg->scx.idle };

			ret = SCX_CALL_OP_RET(sch, cgroup_init,
					      پوچ, tg->css.cgroup, &args);
			اگر (ret)
				ret = scx_ops_sanitize_err(sch, "cgroup_init", ret);
		}
		اگر (ret == 0) {
			tg->scx.sched = sch;
			tg->scx.flags |= SCX_TG_ONLINE | SCX_TG_INITED;
		}
	} وگرنه {
		tg->scx.flags |= SCX_TG_ONLINE;
	}

	بازگشت ret;
}

تهی scx_tg_offline(ساختار task_group *tg)
{
	ساختار scx_sched *sch = tg->scx.sched;

	WARN_ON_ONCE(!(tg->scx.flags & SCX_TG_ONLINE));

	/* INITED implies non-NULL @sch, test before SCX_HAS_OP() derefs */
	اگر (scx_cgroup_enabled && (tg->scx.flags & SCX_TG_INITED) &&
	    SCX_HAS_OP(sch, cgroup_exit))
		SCX_CALL_OP(sch, cgroup_exit, پوچ, tg->css.cgroup);
	tg->scx.sched = پوچ;
	tg->scx.flags &= ~(SCX_TG_ONLINE | SCX_TG_INITED);
}

/*
 * @p's sched for the cgroup migration paths. Stable as re-homes happen either
 * at CGROUP_TASK_MIGRATED of the same migration or under scx_cgroup_lock(),
 * both while holding cgroup_mutex.
 */
ایستا ساختار scx_sched *scx_cgroup_task_sched(ساختار task_struct *p)
{
	بازگشت rcu_dereference_protected(p->scx.sched, lockdep_is_held(&cgroup_mutex));
}

صحیح scx_cgroup_can_attach(ساختار cgroup_taskset *tset)
{
	ساختار cgroup_subsys_state *css;
	ساختار task_struct *p;
	صحیح ret;

	اگر (!scx_cgroup_enabled)
		بازگشت 0;

	cgroup_taskset_for_each(p, css, tset) {
		ساختار scx_sched *sch = scx_cgroup_task_sched(p);
		ساختار cgroup *from = tg_cgrp(task_group(p));
		ساختار cgroup *to = tg_cgrp(css_tg(css));

		WARN_ON_ONCE(p->scx.cgrp_moving_from);

		/*
		 * sched_move_task() omits identity migrations. Let's match the
		 * behavior so that ops.cgroup_prep_move() and ops.cgroup_move()
		 * always match one-to-one.
		 */
		اگر (from == to)
			ادامه;

		/*
		 * The cgroup_move ops are delivered to @p's sched, and only for
		 * moves that don't re-home @p. A re-homing move changes the dfl
		 * cgroup's sched and is reported through the
		 * exit_task/init_task pair that the re-homing generates.
		 */
		اگر (!sch || sch != scx_cgroup_sched(task_css_set(p)->mg_dst_cset->dfl_cgrp))
			ادامه;

		اگر (SCX_HAS_OP(sch, cgroup_prep_move)) {
			ret = SCX_CALL_OP_RET(sch, cgroup_prep_move, پوچ,
					      p, from, css->cgroup);
			اگر (ret) {
				ret = scx_ops_sanitize_err(sch, "cgroup_prep_move", ret);
				برو err;
			}
		}

		p->scx.cgrp_moving_from = from;
	}

	بازگشت 0;

err:
	cgroup_taskset_for_each(p, css, tset) {
		ساختار scx_sched *sch = scx_cgroup_task_sched(p);

		/* cgrp_moving_from implies non-NULL @sch, test it first */
		اگر (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_cancel_move))
			SCX_CALL_OP(sch, cgroup_cancel_move, پوچ,
				    p, p->scx.cgrp_moving_from, css->cgroup);
		p->scx.cgrp_moving_from = پوچ;
	}

	بازگشت ret;
}

تهی scx_cgroup_move_task(ساختار task_struct *p)
{
	ساختار scx_sched *sch;

	اگر (!scx_cgroup_enabled)
		بازگشت;

	/*
	 * Migration keys off css rather than cgroup identity, so it can hand an
	 * unchanged-cgroup task here with cgrp_moving_from NULL. Nothing to
	 * report to the BPF scheduler then, so skip it and keep prep_move and
	 * move paired.
	 */
	sch = scx_cgroup_task_sched(p);
	اگر (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_move))
		SCX_CALL_OP_TASK(sch, cgroup_move, task_rq(p),
				 p, p->scx.cgrp_moving_from,
				 tg_cgrp(task_group(p)));
	p->scx.cgrp_moving_from = پوچ;
}

تهی scx_cgroup_cancel_attach(ساختار cgroup_taskset *tset)
{
	ساختار cgroup_subsys_state *css;
	ساختار task_struct *p;

	اگر (!scx_cgroup_enabled)
		بازگشت;

	cgroup_taskset_for_each(p, css, tset) {
		ساختار scx_sched *sch = scx_cgroup_task_sched(p);

		/* cgrp_moving_from implies non-NULL @sch, test it first */
		اگر (p->scx.cgrp_moving_from && SCX_HAS_OP(sch, cgroup_cancel_move))
			SCX_CALL_OP(sch, cgroup_cancel_move, پوچ,
				    p, p->scx.cgrp_moving_from, css->cgroup);
		p->scx.cgrp_moving_from = پوچ;
	}
}

تهی scx_group_set_weight(ساختار task_group *tg, unsigned long weight)
{
	ساختار scx_sched *sch;

	percpu_down_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	اگر (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_set_weight) &&
	    tg->scx.weight != weight)
		SCX_CALL_OP(sch, cgroup_set_weight, پوچ, tg_cgrp(tg), weight);

	tg->scx.weight = weight;

	percpu_up_read(&scx_cgroup_ops_rwsem);
}

تهی scx_group_set_idle(ساختار task_group *tg, bool idle)
{
	ساختار scx_sched *sch;

	percpu_down_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	اگر (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_set_idle) &&
	    tg->scx.idle != idle)
		SCX_CALL_OP(sch, cgroup_set_idle, پوچ, tg_cgrp(tg), idle);

	/* Update the task group's idle state */
	tg->scx.idle = idle;

	percpu_up_read(&scx_cgroup_ops_rwsem);
}

تهی scx_group_set_bandwidth(ساختار task_group *tg,
			     u64 period_us, u64 quota_us, u64 burst_us)
{
	ساختار scx_sched *sch;

	percpu_down_read(&scx_cgroup_ops_rwsem);
	sch = scx_tg_knob_sched(tg);

	اگر (scx_cgroup_enabled && sch && SCX_HAS_OP(sch, cgroup_set_bandwidth) &&
	    (tg->scx.bw_period_us != period_us ||
	     tg->scx.bw_quota_us != quota_us ||
	     tg->scx.bw_burst_us != burst_us))
		SCX_CALL_OP(sch, cgroup_set_bandwidth, پوچ,
			    tg_cgrp(tg), period_us, quota_us, burst_us);

	tg->scx.bw_period_us = period_us;
	tg->scx.bw_quota_us = quota_us;
	tg->scx.bw_burst_us = burst_us;

	percpu_up_read(&scx_cgroup_ops_rwsem);
}
#endif	/* CONFIG_EXT_GROUP_SCHED */

#اگر defined(CONFIG_EXT_GROUP_SCHED) || defined(CONFIG_EXT_SUB_SCHED)
ایستا ساختار cgroup *ریشه_cgroup(تهی)
{
	بازگشت &cgrp_dfl_ریشه.cgrp;
}

/*
 * cgroup_lock() must nest outside the rwsem write side: a writer waiting
 * for cgroup_mutex deadlocks with cgroup teardown, which holds it while
 * draining a set_* file write blocked on the rwsem behind the writer.
 */
تهی scx_cgroup_lock(تهی)
{
	cgroup_lock();
#ifdef CONFIG_EXT_GROUP_SCHED
	percpu_down_write(&scx_cgroup_ops_rwsem);
#endif
}

تهی scx_cgroup_unlock(تهی)
{
#ifdef CONFIG_EXT_GROUP_SCHED
	percpu_up_write(&scx_cgroup_ops_rwsem);
#endif
	cgroup_unlock();
}
#وگرنه	/* CONFIG_EXT_GROUP_SCHED || CONFIG_EXT_SUB_SCHED */
ایستا درونخطی ساختار cgroup *ریشه_cgroup(تهی) { بازگشت پوچ; }
ایستا درونخطی تهی scx_cgroup_lock(تهی) {}
ایستا درونخطی تهی scx_cgroup_unlock(تهی) {}
#endif	/* CONFIG_EXT_GROUP_SCHED || CONFIG_EXT_SUB_SCHED */

/*
 * Omitted operations:
 *
 * - migrate_task_rq: Unnecessary as task to cpu mapping is transient.
 *
 * - task_fork/dead: We need fork/dead notifications for all tasks regardless of
 *   their current sched_class. Call them directly from sched core instead.
 */
DEFINE_SCHED_CLASS(ext) = {
	.enqueue_task		= enqueue_task_scx,
	.dequeue_task		= dequeue_task_scx,
	.yield_task		= yield_task_scx,
	.yield_to_task		= yield_to_task_scx,

	.wakeup_preempt		= wakeup_preempt_scx,

	.pick_task		= pick_task_scx,

	.put_prev_task		= put_prev_task_scx,
	.set_next_task		= set_next_task_scx,

	.select_task_rq		= select_task_rq_scx,
	.task_woken		= task_woken_scx,
	.set_cpus_allowed	= set_cpus_allowed_scx,

	.rq_online		= rq_online_scx,
	.rq_offline		= rq_offline_scx,

	.task_tick		= task_tick_scx,

	.switching_to		= switching_to_scx,
	.switched_from		= switched_from_scx,
	.switched_to		= switched_to_scx,
	.reweight_task		= reweight_task_scx,
	.prio_changed		= prio_changed_scx,

	.update_curr		= update_curr_scx,

#ifdef CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#endif
};

s32 scx_init_dsq(ساختار scx_dispatch_q *dsq, u64 dsq_id, ساختار scx_sched *sch)
{
	s32 cpu;

	memset(dsq, 0, اندازه(*dsq));

	raw_spin_lock_init(&dsq->lock);
	INIT_LIST_HEAD(&dsq->list);
	dsq->id = dsq_id;
	dsq->sched = sch;

	dsq->pcpu = alloc_percpu(ساختار scx_dsq_pcpu);
	اگر (!dsq->pcpu)
		بازگشت -ENOMEM;

	for_each_possible_cpu(cpu) {
		ساختار scx_dsq_pcpu *pcpu = per_cpu_ptr(dsq->pcpu, cpu);

		pcpu->dsq = dsq;
		INIT_LIST_HEAD(&pcpu->deferred_reenq_user.گره);
	}

	بازگشت 0;
}

ایستا تهی exit_dsq(ساختار scx_dispatch_q *dsq)
{
	s32 cpu;

	for_each_possible_cpu(cpu) {
		ساختار scx_dsq_pcpu *pcpu = per_cpu_ptr(dsq->pcpu, cpu);
		ساختار scx_deferred_reenq_user *dru = &pcpu->deferred_reenq_user;
		ساختار rq *rq = cpu_rq(cpu);

		/*
		 * free_dsq_irq_workfn() cancelled the reenqs before the grace
		 * period and schedule_dsq_reenq() queues nothing after that.
		 */
		اگر (WARN_ON_ONCE(!list_empty(&dru->گره))) {
			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_lock);
			list_del_init(&dru->گره);
		}
	}

	free_percpu(dsq->pcpu);
}

ایستا تهی free_dsq_rcufn(ساختار rcu_head *rcu)
{
	ساختار scx_dispatch_q *dsq = container_of(rcu, ساختار scx_dispatch_q, rcu);

	exit_dsq(dsq);
	kfree(dsq);
}

ایستا تهی free_dsq_irq_workfn(ساختار irq_work *irq_work)
{
	ساختار llist_گره *to_free = llist_del_all(&dsqs_to_free);
	ساختار scx_dispatch_q *dsq, *tmp_dsq;

	llist_for_each_entry_safe(dsq, tmp_dsq, to_free, free_گره) {
		s32 cpu;

		/*
		 * Cancel pending reenqs. Only a run_deferred() that started
		 * before the grace period can hold one, and it keeps IRQs off,
		 * so the grace period waits for it. After this sweep,
		 * schedule_dsq_reenq() sees the invalid id under the same lock
		 * and queues nothing.
		 */
		for_each_possible_cpu(cpu) {
			ساختار scx_dsq_pcpu *pcpu = per_cpu_ptr(dsq->pcpu, cpu);
			ساختار rq *rq = cpu_rq(cpu);

			guard(raw_spinlock_irqsave)(&rq->scx.deferred_reenq_lock);
			list_del_init(&pcpu->deferred_reenq_user.گره);
		}

		call_rcu(&dsq->rcu, free_dsq_rcufn);
	}
}

ایستا DEFINE_IRQ_WORK(free_dsq_irq_work, free_dsq_irq_workfn);

ایستا تهی destroy_dsq(ساختار scx_sched *sch, u64 dsq_id)
{
	ساختار scx_dispatch_q *dsq;
	unsigned long flags;

	rcu_read_lock();

	dsq = find_user_dsq(sch, dsq_id);
	اگر (!dsq)
		برو out_unlock_rcu;

	raw_spin_lock_irqsave(&dsq->lock, flags);

	اگر (dsq->nr) {
		scx_error(sch, "attempting to destroy in-use dsq 0x%016llx (nr=%u)",
			  dsq->id, dsq->nr);
		برو out_unlock_dsq;
	}

	اگر (rhashtable_remove_fast(&sch->dsq_hash, &dsq->hash_گره,
				   dsq_hash_params))
		برو out_unlock_dsq;

	/*
	 * Mark dead by invalidating ->id to prevent scx_dispatch_enqueue() from
	 * queueing more tasks. As this function can be called from anywhere,
	 * freeing is bounced through an irq work to avoid nesting RCU
	 * operations inside scheduler locks.
	 */
	dsq->id = SCX_DSQ_INVALID;
	اگر (llist_add(&dsq->free_گره, &dsqs_to_free))
		irq_work_queue(&free_dsq_irq_work);

out_unlock_dsq:
	raw_spin_unlock_irqrestore(&dsq->lock, flags);
out_unlock_rcu:
	rcu_read_unlock();
}

#ifdef CONFIG_EXT_GROUP_SCHED
ایستا تهی scx_cgroup_exit(ساختار scx_sched *sch)
{
	ساختار cgroup_subsys_state *css;

	/*
	 * scx_tg_on/offline() are excluded through cgroup_lock(). If we walk
	 * cgroups and exit all the inited ones, all online cgroups are exited.
	 */
	css_for_each_descendant_post(css, &ریشه_task_group.css) {
		ساختار task_group *tg = css_tg(css);

		/* also clear the sched of tgs whose ops.cgroup_init() failed */
		tg->scx.sched = پوچ;
		اگر (tg->scx.flags & SCX_TG_INITED) {
			tg->scx.flags &= ~SCX_TG_INITED;
			اگر (sch->ops.cgroup_exit)
				SCX_CALL_OP(sch, cgroup_exit, پوچ, css->cgroup);
		}
	}
}

ایستا صحیح scx_cgroup_init(ساختار scx_sched *sch)
{
	ساختار cgroup_subsys_state *css;
	صحیح ret;

	/*
	 * scx_tg_on/offline() are excluded through cgroup_lock(). If we walk
	 * cgroups and init, all online cgroups are initialized.
	 */
	css_for_each_descendant_pre(css, &ریشه_task_group.css) {
		ساختار task_group *tg = css_tg(css);

		اگر ((tg->scx.flags & (SCX_TG_ONLINE | SCX_TG_INITED)) != SCX_TG_ONLINE)
			ادامه;

		اگر (sch->ops.cgroup_init) {
			ساختار scx_cgroup_init_args args = {
				.weight = tg->scx.weight,
				.bw_period_us = tg->scx.bw_period_us,
				.bw_quota_us = tg->scx.bw_quota_us,
				.bw_burst_us = tg->scx.bw_burst_us,
				.sched_idle = tg->scx.idle,
			};

			ret = SCX_CALL_OP_RET(sch, cgroup_init, پوچ, css->cgroup, &args);
			اگر (ret) {
				scx_error(sch, "ops.cgroup_init() failed (%d)", ret);
				بازگشت ret;
			}
		}

		tg->scx.sched = sch;
		tg->scx.flags |= SCX_TG_INITED;
	}

	بازگشت 0;
}

#وگرنه
ایستا تهی scx_cgroup_exit(ساختار scx_sched *sch) {}
ایستا صحیح scx_cgroup_init(ساختار scx_sched *sch) { بازگشت 0; }
#endif


/********************************************************************************
 * Sysfs interface and ops enable/disable.
 */

#define SCX_ATTR(_name)								\
	ایستا ساختار kobj_attribute scx_attr_##_name = {			\
		.attr = { .name = __stringify(_name), .mode = 0444 },		\
		.show = scx_attr_##_name##_show,				\
	}

ایستا ssize_t scx_attr_state_show(ساختار kobject *kobj,
				   ساختار kobj_attribute *ka, نویسه *buf)
{
	بازگشت sysfs_emit(buf, "%s\n", scx_enable_state_str[scx_enable_state()]);
}
SCX_ATTR(state);

ایستا ssize_t scx_attr_switch_all_show(ساختار kobject *kobj,
					ساختار kobj_attribute *ka, نویسه *buf)
{
	بازگشت sysfs_emit(buf, "%d\n", READ_ONCE(scx_switching_all));
}
SCX_ATTR(switch_all);

ایستا ssize_t scx_attr_nr_rejected_show(ساختار kobject *kobj,
					 ساختار kobj_attribute *ka, نویسه *buf)
{
	بازگشت sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_nr_rejected));
}
SCX_ATTR(nr_rejected);

ایستا ssize_t scx_attr_hotplug_seq_show(ساختار kobject *kobj,
					 ساختار kobj_attribute *ka, نویسه *buf)
{
	بازگشت sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_hotplug_seq));
}
SCX_ATTR(hotplug_seq);

ایستا ssize_t scx_attr_enable_seq_show(ساختار kobject *kobj,
					ساختار kobj_attribute *ka, نویسه *buf)
{
	بازگشت sysfs_emit(buf, "%ld\n", atomic_long_read(&scx_enable_seq));
}
SCX_ATTR(enable_seq);

ایستا ساختار attribute *scx_global_attrs[] = {
	&scx_attr_state.attr,
	&scx_attr_switch_all.attr,
	&scx_attr_nr_rejected.attr,
	&scx_attr_hotplug_seq.attr,
	&scx_attr_enable_seq.attr,
	پوچ,
};

ایستا ثابت ساختار attribute_group scx_global_attr_group = {
	.attrs = scx_global_attrs,
};

ایستا تهی free_pnode(ساختار scx_sched_pnode *pnode);
ایستا تهی free_exit_info(ساختار scx_exit_info *ei);
ایستا ثابت نویسه *scx_exit_reason(شمارش scx_exit_kind kind);
ایستا bool scx_claim_exit(ساختار scx_sched *sch, شمارش scx_exit_kind kind);

s32 scx_alloc_kern_arena_objs(ساختار scx_sched *sch)
{
	size_t size = struct_size_t(ساختار scx_cmask, bits,
				    SCX_CMASK_NR_واژگان(رقم_possible_cpus()));
	ساختار scx_cmask *online;
	ساختار scx_cmask_ref ref;
	صحیح cpu;

	/* hotplug stays excluded until the online mask is published */
	lockdep_assert_cpus_held();

	اگر (!sch->is_cid_type || !sch->arena_pool)
		بازگشت 0;

	sch->set_cmask_scratch = alloc_percpu(ساختار scx_cmask *);
	اگر (!sch->set_cmask_scratch)
		بازگشت -ENOMEM;

	for_each_possible_cpu(cpu) {
		ساختار scx_cmask **slot = per_cpu_ptr(sch->set_cmask_scratch, cpu);

		*slot = scx_arena_alloc(sch, size);
		اگر (!*slot)
			بازگشت -ENOMEM;
		scx_cmask_init(*slot, 0, رقم_possible_cpus());
	}

	/* pack the online mask alongside the scratch masks */
	online = scx_arena_alloc(sch, size);
	اگر (!online)
		بازگشت -ENOMEM;

	scoped_guard(rcu) {
		scx_cmask_ref_init_kern(sch, online, 0, رقم_possible_cpus(), &ref);
		scx_cmask_ref_from_cpumask(&ref, cpu_active_mask);
	}
	sch->online_cmask = online;

	بازگشت 0;
}

ایستا تهی scx_free_kern_arena_objs(ساختار scx_sched *sch)
{
	size_t size = struct_size_t(ساختار scx_cmask, bits,
				    SCX_CMASK_NR_واژگان(رقم_possible_cpus()));
	صحیح cpu;

	scx_arena_free(sch, sch->online_cmask, size);
	اگر (!sch->set_cmask_scratch)
		بازگشت;

	for_each_possible_cpu(cpu) {
		ساختار scx_cmask **slot = per_cpu_ptr(sch->set_cmask_scratch, cpu);

		scx_arena_free(sch, *slot, size);
	}
	free_percpu(sch->set_cmask_scratch);
	sch->set_cmask_scratch = پوچ;
}

ایستا تهی scx_sched_free_rcu_work(ساختار work_struct *work)
{
	ساختار rcu_work *rcu_work = to_rcu_work(work);
	ساختار scx_sched *sch = container_of(rcu_work, ساختار scx_sched, rcu_work);
	ساختار rhashtable_iter rht_iter;
	ساختار scx_dispatch_q *dsq;
	صحیح cpu, گره;

	irq_work_sync(&sch->propagate_exit_irq_work);
	irq_work_sync(&sch->disable_irq_work);
	kthread_destroy_worker(sch->helper);
	timer_shutdown_sync(&sch->bypass_lb_timer);
	free_cpumask_var(sch->bypass_lb_donee_cpumask);
	free_cpumask_var(sch->bypass_lb_resched_cpumask);
	free_cpumask_var(sch->stall_cpus);

#ifdef CONFIG_EXT_SUB_SCHED
	kfree(sch->cgrp_path);
	اگر (sch_cgroup(sch))
		cgroup_put(sch_cgroup(sch));
	اگر (sch->sub_kset)
		kobject_put(&sch->sub_kset->kobj);
	اگر (scx_parent(sch))
		kobject_put(&scx_parent(sch)->kobj);
#endif	/* CONFIG_EXT_SUB_SCHED */

	for_each_possible_cpu(cpu) {
		ساختار scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);

		/*
		 * $sch would have entered bypass mode before the RCU grace
		 * period. As that blocks new deferrals, all
		 * deferred_reenq_local_node's must be off-list by now.
		 */
		WARN_ON_ONCE(!list_empty(&pcpu->deferred_reenq_local.گره));

		/* remove the queued ecaps sync so the pcpu can be freed */
		scx_discard_ecaps_to_sync(cpu, pcpu);

		/*
		 * Bypass blocks new kicks. Flush the kick irq_work so this
		 * pcpu's to_kick_node is off the list before it is freed.
		 */
		irq_work_sync(&cpu_rq(cpu)->scx.kick_cpus_irq_work);
		WARN_ON_ONCE(!list_empty(&pcpu->to_kick_گره));
		free_cpumask_var(pcpu->cpus_to_kick);
		free_cpumask_var(pcpu->cpus_to_kick_if_idle);
		free_cpumask_var(pcpu->cpus_to_preempt);
		free_cpumask_var(pcpu->cpus_to_wait);

		exit_dsq(scx_bypass_dsq(sch, cpu));
	}

	free_percpu(sch->pcpu);

	for_each_گره_state(گره, N_POSSIBLE)
		free_pnode(sch->pnode[گره]);
	kfree(sch->pnode);

	scx_free_pshards(sch);

	rhashtable_walk_enter(&sch->dsq_hash, &rht_iter);
	انجامبده {
		rhashtable_walk_start(&rht_iter);

		وقتی (!IS_ERR_OR_NULL((dsq = rhashtable_walk_next(&rht_iter))))
			destroy_dsq(sch, dsq->id);

		rhashtable_walk_stop(&rht_iter);
	} وقتی (dsq == ERR_PTR(-EAGAIN));
	rhashtable_walk_exit(&rht_iter);

	rhashtable_free_and_destroy(&sch->dsq_hash, پوچ, پوچ);
	free_exit_info(sch->exit_info);
	scx_free_kern_arena_objs(sch);
	scx_arena_pool_destroy(sch);
	اگر (sch->arena_map)
		bpf_map_put(sch->arena_map);

	/* @sch is completely inactive by now */
	scx_dec_has_subs(sch);

	kfree(sch);
}

ایستا تهی scx_kobj_release(ساختار kobject *kobj)
{
	ساختار scx_sched *sch = container_of(kobj, ساختار scx_sched, kobj);

	INIT_RCU_WORK(&sch->rcu_work, scx_sched_free_rcu_work);
	queue_rcu_work(system_dfl_wq, &sch->rcu_work);
}

ایستا ssize_t scx_attr_ops_show(ساختار kobject *kobj,
				 ساختار kobj_attribute *ka, نویسه *buf)
{
	ساختار scx_sched *sch = container_of(kobj, ساختار scx_sched, kobj);

	بازگشت sysfs_emit(buf, "%s\n", sch->ops.name);
}
SCX_ATTR(ops);

#define scx_attr_event_show(buf, at, events, kind) ({				\
	sysfs_emit_at(buf, at, "%s %llu\n", #kind, (events)->kind);		\
})

ایستا ssize_t scx_attr_events_show(ساختار kobject *kobj,
				    ساختار kobj_attribute *ka, نویسه *buf)
{
	ساختار scx_sched *sch = container_of(kobj, ساختار scx_sched, kobj);
	ساختار scx_event_stats events;
	صحیح at = 0;

	scx_read_events(sch, &events);
#define SCX_EVENT(name)	(at += scx_attr_event_show(buf, at, &events, name))
	SCX_EVENTS_LIST(SCX_EVENT);
#undef SCX_EVENT
	بازگشت at;
}
SCX_ATTR(events);

#ifdef CONFIG_EXT_SUB_SCHED
ایستا ثابت نویسه *scx_cap_names[__SCX_NR_CAPS] = {
	[__SCX_CAP_ENQ_IMMED]	= "enq_immed",
	[__SCX_CAP_ENQ]		= "enq",
	[__SCX_CAP_PREEMPT]	= "preempt",
	[__SCX_CAP_PERF]	= "perf",
};

ایستا ssize_t scx_attr_caps_show(ساختار kobject *kobj,
				  ساختار kobj_attribute *ka, نویسه *buf)
{
	ساختار scx_sched *sch = container_of(kobj, ساختار scx_sched, kobj);
	u32 npossible = رقم_possible_cpus();
	ساختار scx_cmask *agg __free(kfree) =
		kzalloc_flex(*agg, bits, SCX_CMASK_NR_واژگان(npossible));
	unsigned long *agg_bm __free(bitmap) = bitmap_zalloc(npossible, GFP_KERNEL);
	ssize_t شمار = 0;
	s32 cap, si;

	اگر (!agg || !agg_bm)
		بازگشت -ENOMEM;

	برای (cap = 0; cap < __SCX_NR_CAPS; cap++) {
		SCX_CMASK_DEFINE(snap, 0, SCX_CID_SHARD_MAX_CPUS);

		scx_cmask_init(agg, 0, npossible);
		برای (si = 0; si < sch->nr_pshards; si++) {
			ساختار scx_cmask *cm = &sch->pshard[si]->caps[cap].cmask;

			scx_cmask_reframe(snap, cm->base, cm->nr_cids);
			scx_cmask_copy(snap, cm);
			scx_cmask_or(agg, snap);
		}
		/* %*pbl takes unsigned long bitmap layout, convert from u64 */
		bitmap_from_arr64(agg_bm, agg->bits, npossible);
		شمار += sysfs_emit_at(buf, شمار, "%s: %*pbl\n",
				       scx_cap_names[cap], npossible, agg_bm);
	}
	بازگشت شمار;
}
SCX_ATTR(caps);
#endif	/* CONFIG_EXT_SUB_SCHED */

ایستا ساختار attribute *scx_sched_attrs[] = {
	&scx_attr_ops.attr,
	&scx_attr_events.attr,
#ifdef CONFIG_EXT_SUB_SCHED
	&scx_attr_caps.attr,
#endif
	پوچ,
};
ATTRIBUTE_GROUPS(scx_sched);

ایستا ثابت ساختار kobj_type scx_ktype = {
	.release = scx_kobj_release,
	.sysfs_ops = &kobj_sysfs_ops,
	.default_groups = scx_sched_groups,
};

ایستا صحیح scx_uevent(ثابت ساختار kobject *kobj, ساختار kobj_uevent_env *env)
{
	ثابت ساختار scx_sched *sch;

	/*
	 * scx_uevent() can be reached by both scx_sched kobjects (scx_ktype)
	 * and sub-scheduler kset kobjects (kset_ktype) through the parent
	 * chain walk. Filter out the latter to avoid invalid casts.
	 */
	اگر (kobj->ktype != &scx_ktype)
		بازگشت 0;

	sch = container_of(kobj, ساختار scx_sched, kobj);

	بازگشت add_uevent_var(env, "SCXOPS=%s", sch->ops.name);
}

ایستا ثابت ساختار kset_uevent_ops scx_uevent_ops = {
	.uevent = scx_uevent,
};

/*
 * Used by sched_fork() and __setscheduler_class() to pick the matching
 * sched_class. dl/rt are already handled.
 */
bool task_should_scx(صحیح policy)
{
	/* if disabled, nothing should be on it */
	اگر (!scx_enabled())
		بازگشت false;

	/* scx is taking over all SCHED_OTHER and SCHED_EXT tasks */
	اگر (READ_ONCE(scx_switching_all))
		بازگشت true;

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
	اگر (unlikely(scx_enable_state() == SCX_DISABLING))
		بازگشت false;

	بازگشت policy == SCHED_EXT;
}

bool scx_allow_ttwu_queue(ثابت ساختار task_struct *p)
{
	ساختار scx_sched *sch;

	اگر (!scx_enabled())
		بازگشت true;

	sch = scx_task_sched(p);
	اگر (unlikely(!sch))
		بازگشت true;

	اگر (sch->ops.flags & SCX_OPS_ALLOW_QUEUED_WAKEUP)
		بازگشت true;

	اگر (unlikely(p->sched_class != &ext_sched_class))
		بازگشت true;

	بازگشت false;
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
ایستا __printf(2, 3) bool handle_lockup(صحیح exit_cpu, ثابت نویسه *fmt, ...)
{
	ساختار scx_sched *sch;
	va_list args;
	bool ret;

	guard(rcu)();

	sch = rcu_dereference(scx_ریشه);
	اگر (unlikely(!sch))
		بازگشت false;

	انتخاب (scx_enable_state()) {
	حالت SCX_ENABLING:
	حالت SCX_ENABLED:
		va_start(args, fmt);
		ret = scx_vexit(sch, SCX_EXIT_ERROR, 0, exit_cpu, fmt, args);
		va_end(args);
		بازگشت ret;
	پیشفرض:
		بازگشت false;
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
bool scx_rcu_cpu_stall(ثابت ساختار cpumask *stalled_mask)
{
	ساختار scx_sched *sch;
	ساختار scx_exit_info *ei;
	صحیح exit_cpu;

	guard(rcu)();

	sch = rcu_dereference(scx_ریشه);
	اگر (unlikely(!sch))
		بازگشت false;

	انتخاب (scx_enable_state()) {
	حالت SCX_ENABLING:
	حالت SCX_ENABLED:
		بشکن;
	پیشفرض:
		بازگشت false;
	}

	exit_cpu = cpumask_empty(stalled_mask) ? -1 : (صحیح)cpumask_first(stalled_mask);
	ei = sch->exit_info;

	guard(preempt)();

	اگر (!scx_claim_exit(sch, SCX_EXIT_ERROR))
		بازگشت false;

#ifdef CONFIG_STACKTRACE
	ei->bt_len = پشته_trace_save(ei->bt, SCX_EXIT_BT_LEN, 1);
#endif
	scnprintf(ei->پیام, SCX_EXIT_پیام_LEN, "RCU CPU stall on CPUs (%*pbl)",
		  cpumask_pr_args(stalled_mask));
	ei->kind = SCX_EXIT_ERROR;
	ei->reason = scx_exit_reason(SCX_EXIT_ERROR);
	ei->exit_cpu = exit_cpu;
	cpumask_copy(sch->stall_cpus, stalled_mask);

	irq_work_queue(&sch->disable_irq_work);
	بازگشت true;
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
تهی scx_softlockup(u32 dur_s)
{
	صحیح cpu = smp_processor_id();

	اگر (!handle_lockup(cpu, "soft lockup - CPU %d stuck for %us", cpu, dur_s))
		بازگشت;

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
bool scx_hardlockup(صحیح cpu)
{
	اگر (!handle_lockup(cpu, "hard lockup - CPU %d", cpu))
		بازگشت false;

	printk_deferred(KERN_ERR "sched_ext: Hard lockup - CPU %d, disabling BPF scheduler\n",
			cpu);
	بازگشت true;
}

ایستا u32 bypass_lb_cpu(ساختار scx_sched *sch, s32 donor,
			 ساختار cpumask *donee_mask, ساختار cpumask *resched_mask,
			 u32 nr_donor_target, u32 nr_donee_target)
{
	ساختار rq *donor_rq = cpu_rq(donor);
	ساختار scx_dispatch_q *donor_dsq = scx_bypass_dsq(sch, donor);
	ساختار task_struct *p, *n;
	ساختار scx_dsq_list_گره cursor = INIT_DSQ_LIST_CURSOR(cursor, donor_dsq, 0);
	s32 delta = READ_ONCE(donor_dsq->nr) - nr_donor_target;
	u32 nr_balanced = 0, min_delta_us;

	/*
	 * All we want to guarantee is reasonable forward progress. No reason to
	 * fine tune. Assuming every task on @donor_dsq runs their full slice,
	 * consider offloading iff the total queued duration is over the
	 * threshold.
	 */
	min_delta_us = READ_ONCE(scx_bypass_lb_intv_us) / SCX_BYPASS_LB_MIN_DELTA_DIV;
	اگر (delta < DIV_ROUND_UP(min_delta_us, READ_ONCE(scx_slice_bypass_us)))
		بازگشت 0;

	raw_spin_rq_lock_irq(donor_rq);
	raw_spin_lock(&donor_dsq->lock);
	list_add(&cursor.گره, &donor_dsq->list);
resume:
	n = container_of(&cursor, ساختار task_struct, scx.dsq_list);
	n = nldsq_next_task(donor_dsq, n, false);

	وقتی ((p = n)) {
		ساختار scx_dispatch_q *donee_dsq;
		صحیح donee;

		n = nldsq_next_task(donor_dsq, n, false);

		اگر (donor_dsq->nr <= nr_donor_target)
			بشکن;

		اگر (cpumask_empty(donee_mask))
			بشکن;

		/*
		 * If an earlier pass placed @p on @donor_dsq from a different
		 * CPU and the donee hasn't consumed it yet, @p is still on the
		 * previous CPU and task_rq(@p) != @donor_rq. @p can't be moved
		 * without its rq locked. Skip.
		 */
		اگر (task_rq(p) != donor_rq)
			ادامه;

		donee = cpumask_any_and_distribute(donee_mask, p->cpus_ptr);
		اگر (donee >= nr_cpu_ids)
			ادامه;

		donee_dsq = scx_bypass_dsq(sch, donee);

		/*
		 * $p's rq is not locked but $p's DSQ lock protects its
		 * scheduling properties making this test safe.
		 */
		اگر (!task_can_run_on_remote_rq(sch, p, cpu_rq(donee), false))
			ادامه;

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
		dispatch_dequeue_locked(p, donor_dsq);
		scx_dispatch_enqueue(sch, cpu_rq(donee), donee_dsq, p, 0, 0, SCX_ENQ_NESTED);

		/*
		 * $donee might have been idle and need to be woken up. No need
		 * to be clever. Kick every CPU that receives tasks.
		 */
		cpumask_set_cpu(donee, resched_mask);

		اگر (READ_ONCE(donee_dsq->nr) >= nr_donee_target)
			cpumask_clear_cpu(donee, donee_mask);

		nr_balanced++;
		اگر (!(nr_balanced % SCX_BYPASS_LB_BATCH) && n) {
			list_move_tail(&cursor.گره, &n->scx.dsq_list.گره);
			raw_spin_unlock(&donor_dsq->lock);
			scx_rq_lock_drop(donor_rq);
			raw_spin_rq_unlock_irq(donor_rq);
			cpu_relax();
			raw_spin_rq_lock_irq(donor_rq);
			raw_spin_lock(&donor_dsq->lock);
			برو resume;
		}
	}

	list_del_init(&cursor.گره);
	raw_spin_unlock(&donor_dsq->lock);
	scx_rq_lock_drop(donor_rq);
	raw_spin_rq_unlock_irq(donor_rq);

	بازگشت nr_balanced;
}

ایستا تهی bypass_lb_گره(ساختار scx_sched *sch, صحیح گره)
{
	ثابت ساختار cpumask *گره_mask = cpumask_of_گره(گره);
	ساختار cpumask *donee_mask = sch->bypass_lb_donee_cpumask;
	ساختار cpumask *resched_mask = sch->bypass_lb_resched_cpumask;
	u32 nr_tasks = 0, nr_cpus = 0, nr_balanced = 0;
	u32 nr_target, nr_donor_target;
	u32 before_min = U32_MAX, before_max = 0;
	u32 after_min = U32_MAX, after_max = 0;
	صحیح cpu;

	/* count the target tasks and CPUs */
	for_each_cpu_and(cpu, cpu_online_mask, گره_mask) {
		u32 nr = READ_ONCE(scx_bypass_dsq(sch, cpu)->nr);

		nr_tasks += nr;
		nr_cpus++;

		before_min = min(nr, before_min);
		before_max = max(nr, before_max);
	}

	اگر (!nr_cpus)
		بازگشت;

	/*
	 * We don't want CPUs to have more than $nr_donor_target tasks and
	 * balancing to fill donee CPUs upto $nr_target. Once targets are
	 * calculated, find the donee CPUs.
	 */
	nr_target = DIV_ROUND_UP(nr_tasks, nr_cpus);
	nr_donor_target = DIV_ROUND_UP(nr_target * SCX_BYPASS_LB_DONOR_PCT, 100);

	cpumask_clear(donee_mask);
	for_each_cpu_and(cpu, cpu_online_mask, گره_mask) {
		اگر (READ_ONCE(scx_bypass_dsq(sch, cpu)->nr) < nr_target)
			cpumask_set_cpu(cpu, donee_mask);
	}

	/* iterate !donee CPUs and see if they should be offloaded */
	cpumask_clear(resched_mask);
	for_each_cpu_and(cpu, cpu_online_mask, گره_mask) {
		اگر (cpumask_empty(donee_mask))
			بشکن;
		اگر (cpumask_test_cpu(cpu, donee_mask))
			ادامه;
		اگر (READ_ONCE(scx_bypass_dsq(sch, cpu)->nr) <= nr_donor_target)
			ادامه;

		nr_balanced += bypass_lb_cpu(sch, cpu, donee_mask, resched_mask,
					     nr_donor_target, nr_target);
	}

	for_each_cpu(cpu, resched_mask)
		resched_cpu(cpu);

	for_each_cpu_and(cpu, cpu_online_mask, گره_mask) {
		u32 nr = READ_ONCE(scx_bypass_dsq(sch, cpu)->nr);

		after_min = min(nr, after_min);
		after_max = max(nr, after_max);

	}

	trace_sched_ext_bypass_lb(گره, nr_cpus, nr_tasks, nr_balanced,
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
ایستا تهی scx_bypass_lb_timerfn(ساختار timer_list *timer)
{
	ساختار scx_sched *sch = container_of(timer, ساختار scx_sched, bypass_lb_timer);
	صحیح گره;
	u32 intv_us;

	اگر (!scx_bypass_dsp_enabled(sch))
		بازگشت;

	for_each_گره_with_cpus(گره)
		bypass_lb_گره(sch, گره);

	intv_us = READ_ONCE(scx_bypass_lb_intv_us);
	اگر (intv_us)
		mod_timer(timer, jiffies + usecs_to_jiffies(intv_us));
}

ایستا bool inc_bypass_depth(ساختار scx_sched *sch)
{
	lockdep_assert_held(&scx_bypass_lock);

	WARN_ON_ONCE(sch->bypass_depth < 0);
	WRITE_ONCE(sch->bypass_depth, sch->bypass_depth + 1);
	اگر (sch->bypass_depth != 1)
		بازگشت false;

	WRITE_ONCE(sch->slice_dfl, READ_ONCE(scx_slice_bypass_us) * NSEC_PER_USEC);
	sch->bypass_timestamp = ktime_get_ns();
	scx_add_event(sch, SCX_EV_BYPASS_ACTIVATE, 1);
	بازگشت true;
}

ایستا bool dec_bypass_depth(ساختار scx_sched *sch)
{
	lockdep_assert_held(&scx_bypass_lock);

	WARN_ON_ONCE(sch->bypass_depth < 1);
	WRITE_ONCE(sch->bypass_depth, sch->bypass_depth - 1);
	اگر (sch->bypass_depth != 0)
		بازگشت false;

	WRITE_ONCE(sch->slice_dfl, SCX_SLICE_DFL);
	scx_add_event(sch, SCX_EV_BYPASS_DURATION,
		      ktime_get_ns() - sch->bypass_timestamp);
	بازگشت true;
}

ایستا تهی enable_bypass_dsp(ساختار scx_sched *sch)
{
	ساختار scx_sched *host = scx_parent(sch) ?: sch;
	u32 intv_us = READ_ONCE(scx_bypass_lb_intv_us);
	s32 ret;

	/*
	 * @sch->bypass_depth transitioning from 0 to 1 triggers enabling.
	 * Shouldn't stagger.
	 */
	اگر (WARN_ON_ONCE(test_and_set_bit(0, &sch->bypass_dsp_claim)))
		بازگشت;

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
	ret = atomic_inc_return(&sch->bypass_dsp_enable_depth);
	WARN_ON_ONCE(ret <= 0);

	اگر (host != sch) {
		ret = atomic_inc_return(&host->bypass_dsp_enable_depth);
		WARN_ON_ONCE(ret <= 0);
	}

	/*
	 * The LB timer will stop running if bypass dispatch is disabled. Start
	 * after enabling bypass dispatch.
	 */
	اگر (intv_us && !timer_pending(&host->bypass_lb_timer))
		mod_timer(&host->bypass_lb_timer,
			  jiffies + usecs_to_jiffies(intv_us));
}

/* may be called without holding scx_bypass_lock */
تهی scx_disable_bypass_dsp(ساختار scx_sched *sch)
{
	s32 ret;

	اگر (!test_and_clear_bit(0, &sch->bypass_dsp_claim))
		بازگشت;

	ret = atomic_dec_return(&sch->bypass_dsp_enable_depth);
	WARN_ON_ONCE(ret < 0);

	اگر (scx_parent(sch)) {
		ret = atomic_dec_return(&scx_parent(sch)->bypass_dsp_enable_depth);
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
ایستا تهی unbypass_renotify_idle(ساختار rq *rq, ساختار scx_sched *pos,
				   ساختار scx_sched_pcpu *pcpu)
{
	اگر (!pos->level) {
		rq->scx.flags |= SCX_RQ_ریشه_IDLE_RENOTIFY;
		بازگشت;
	}
#ifdef CONFIG_EXT_SUB_SCHED
	pcpu->idle_renotify = true;
	rq->scx.flags |= SCX_RQ_SUB_IDLE_RENOTIFY;
#endif
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
تهی scx_bypass(ساختار scx_sched *sch, bool bypass)
{
	ساختار scx_sched *pos;
	unsigned long flags;
	صحیح cpu;
	bool changed;

	raw_spin_lock_irqsave(&scx_bypass_lock, flags);

	اگر (bypass) {
		changed = inc_bypass_depth(sch);
		اگر (changed)
			enable_bypass_dsp(sch);
	} وگرنه {
		changed = dec_bypass_depth(sch);
	}

	/*
	 * Bypass state is propagated to all descendants - an scx_sched bypasses
	 * if itself or any of its ancestors are in bypass mode. A descendant's
	 * depth is the sum of its own bypasses and its ancestors', so every
	 * call propagates whether or not @sch changed state. The descendants'
	 * claims are released below with @sch's, after their tasks have moved.
	 */
	raw_spin_lock(&scx_sched_lock);
	scx_for_each_descendant_pre(pos, sch) {
		اگر (pos == sch)
			ادامه;
		اگر (bypass) {
			اگر (inc_bypass_depth(pos))
				enable_bypass_dsp(pos);
		} وگرنه {
			dec_bypass_depth(pos);
		}
	}
	raw_spin_unlock(&scx_sched_lock);

	/*
	 * A descendant's depth is at least @sch's, so no bypass state changed
	 * unless @sch's did.
	 */
	اگر (!changed)
		برو unlock;

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
		ساختار rq *rq = cpu_rq(cpu);
		ساختار task_struct *p, *n;

		raw_spin_rq_lock(rq);
		raw_spin_lock(&scx_sched_lock);

		scx_for_each_descendant_pre(pos, sch) {
			ساختار scx_sched_pcpu *pcpu = per_cpu_ptr(pos->pcpu, cpu);
			bool was_bypassing = pcpu->flags & SCX_SCHED_PCPU_BYPASSING;

			اگر (pos->bypass_depth) {
				pcpu->flags |= SCX_SCHED_PCPU_BYPASSING;
			} وگرنه {
				pcpu->flags &= ~SCX_SCHED_PCPU_BYPASSING;
				اگر (was_bypassing) {
					unbypass_renotify_idle(rq, pos, pcpu);
					scx_unbypass_replay_ecaps(rq, pos);
				}
			}
		}

		raw_spin_unlock(&scx_sched_lock);

		/*
		 * We need to guarantee that no tasks are on the BPF scheduler
		 * while bypassing. Either we see enabled or the enable path
		 * sees scx_bypassing() before moving tasks to SCX.
		 */
		اگر (!scx_enabled()) {
			scx_rq_lock_drop(rq);
			raw_spin_rq_unlock(rq);
			ادامه;
		}

		/*
		 * The use of list_for_each_entry_safe_reverse() is required
		 * because each task is going to be removed from and added back
		 * to the runnable_list during iteration. Because they're added
		 * to the tail of the list, safe reverse iteration can still
		 * visit all nodes.
		 */
		list_for_each_entry_safe_reverse(p, n, &rq->scx.runnable_list,
						 scx.runnable_گره) {
			اگر (!scx_is_descendant(scx_task_sched(p), sch))
				ادامه;

			/*
			 * Bypass trumps protection. Cycling clears for queued
			 * tasks but current task needs explicit stripping.
			 */
			اگر (bypass && task_current(rq, p))
				scx_task_slice_ended(rq, p);

			/* cycling deq/enq is enough, see the function comment */
			scoped_guard (sched_change, p, DEQUEUE_SAVE | DEQUEUE_MOVE) {
				/* nothing */ ;
			}
		}

		/* resched to restore ticks and idle state */
		اگر (cpu_online(cpu) || cpu == smp_processor_id())
			resched_curr(rq);

		scx_rq_lock_drop(rq);
		raw_spin_rq_unlock(rq);
	}

	/* disarming must come after moving all tasks out of the bypass DSQs */
	اگر (!bypass) {
		guard(raw_spinlock)(&scx_sched_lock);
		scx_for_each_descendant_pre(pos, sch)
			اگر (!pos->bypass_depth)
				scx_disable_bypass_dsp(pos);
	}
unlock:
	raw_spin_unlock_irqrestore(&scx_bypass_lock, flags);
}

ایستا تهی free_exit_info(ساختار scx_exit_info *ei)
{
	kvfree(ei->dump);
	kfree(ei->پیام);
	kfree(ei->bt);
	kfree(ei);
}

ایستا ساختار scx_exit_info *alloc_exit_info(size_t exit_dump_len)
{
	ساختار scx_exit_info *ei;

	ei = kzalloc_obj(*ei);
	اگر (!ei)
		بازگشت پوچ;

	ei->exit_cpu = -1;
	ei->bt = kzalloc_objs(ei->bt[0], SCX_EXIT_BT_LEN);
	ei->پیام = kzalloc(SCX_EXIT_پیام_LEN, GFP_KERNEL);
	ei->dump = kvzalloc(exit_dump_len, GFP_KERNEL);

	اگر (!ei->bt || !ei->پیام || !ei->dump) {
		free_exit_info(ei);
		بازگشت پوچ;
	}

	بازگشت ei;
}

ایستا ثابت نویسه *scx_exit_reason(شمارش scx_exit_kind kind)
{
	انتخاب (kind) {
	حالت SCX_EXIT_UNREG:
		بازگشت "unregistered from user space";
	حالت SCX_EXIT_UNREG_BPF:
		بازگشت "unregistered from BPF";
	حالت SCX_EXIT_UNREG_KERN:
		بازگشت "unregistered from the main kernel";
	حالت SCX_EXIT_SYSRQ:
		بازگشت "disabled by sysrq-S";
	حالت SCX_EXIT_PARENT:
		بازگشت "parent exiting";
	حالت SCX_EXIT_PARENT_KILL:
		بازگشت "killed by parent scheduler";
	حالت SCX_EXIT_ERROR:
		بازگشت "runtime error";
	حالت SCX_EXIT_ERROR_BPF:
		بازگشت "scx_bpf_error";
	حالت SCX_EXIT_ERROR_STALL:
		بازگشت "runnable task stall";
	حالت SCX_EXIT_ERROR_REENQ:
		بازگشت "reenqueue limit";
	حالت SCX_EXIT_ERROR_RESCUE:
		بازگشت "rescue bandwidth overload";
	پیشفرض:
		بازگشت "<UNKNOWN>";
	}
}

ایستا تهی free_kick_syncs(تهی)
{
	صحیح cpu;

	for_each_possible_cpu(cpu) {
		ساختار scx_kick_syncs __rcu **ksyncs = per_cpu_ptr(&scx_kick_syncs, cpu);
		ساختار scx_kick_syncs *to_free;

		/* flush the pending kick before freeing @ksyncs */
		irq_work_sync(&cpu_rq(cpu)->scx.kick_cpus_irq_work);
		to_free = rcu_replace_pointer(*ksyncs, پوچ, true);
		اگر (to_free)
			kvfree_rcu(to_free, rcu);
	}
}

ایستا تهی refresh_watchdog(تهی)
{
	ساختار scx_sched *sch;
	unsigned long intv = ULONG_MAX;

	/* take the shortest timeout and use its half for watchdog interval */
	rcu_read_lock();
	list_for_each_entry_rcu(sch, &scx_sched_all, all)
		intv = max(min(intv, sch->watchdog_timeout / 2), 1);
	rcu_read_unlock();

	WRITE_ONCE(scx_watchdog_timestamp, jiffies);
	WRITE_ONCE(scx_watchdog_interval, intv);

	اگر (intv < ULONG_MAX)
		mod_delayed_work(system_dfl_wq, &scx_watchdog_work, intv);
	وگرنه
		cancel_delayed_work_sync(&scx_watchdog_work);
}

s32 scx_link_sched(ساختار scx_sched *sch)
{
	scoped_guard(raw_spinlock_irqsave, &scx_bypass_lock)	/* for the parent bypass check */
	scoped_guard(raw_spinlock, &scx_sched_lock) {
#ifdef CONFIG_EXT_SUB_SCHED
		ساختار scx_sched *parent = scx_parent(sch);

		اگر (parent) {
			s32 ret;

			/*
			 * Bypass state is spread across per-cpu flags and a
			 * depth count, so inheriting it is tricky and has no
			 * valid use case. Refuse it.
			 */
			اگر (READ_ONCE(parent->bypass_depth)) {
				scx_error(sch, "parent bypassing (%d)", -EBUSY);
				بازگشت -EBUSY;
			}

			ret = rhashtable_lookup_درج_fast(&scx_sched_hash,
					&sch->hash_گره, scx_sched_hash_params);
			اگر (ret) {
				scx_error(sch, "failed to insert into scx_sched_hash (%d)",
					  ret);
				بازگشت ret;
			}

			list_add_tail_rcu(&sch->sibling, &parent->children);

			/*
			 * Pairs with the mb after the ->aborting assertion in
			 * scx_claim_exit(). Either we see ->aborting and back
			 * out, or the exit path sees us and exits us.
			 */
			smp_mb();
			اگر (unlikely(READ_ONCE(parent->aborting))) {
				rhashtable_remove_fast(&scx_sched_hash, &sch->hash_گره,
						       scx_sched_hash_params);
				list_del_rcu(&sch->sibling);
				scx_error(sch, "parent disabled (%d)", -ENOENT);
				بازگشت -ENOENT;
			}

			sch->linked = true;
		}
#endif	/* CONFIG_EXT_SUB_SCHED */

		list_add_tail_rcu(&sch->all, &scx_sched_all);
	}

	refresh_watchdog();
	بازگشت 0;
}

تهی scx_unlink_sched(ساختار scx_sched *sch)
{
	scoped_guard(raw_spinlock_irq, &scx_sched_lock) {
#ifdef CONFIG_EXT_SUB_SCHED
		اگر (sch->linked) {
			rhashtable_remove_fast(&scx_sched_hash, &sch->hash_گره,
					       scx_sched_hash_params);
			list_del_rcu(&sch->sibling);
			sch->linked = false;
		}
#endif	/* CONFIG_EXT_SUB_SCHED */
		list_del_rcu(&sch->all);
	}

	refresh_watchdog();
}

/*
 * Called to disable future dumps and wait for in-progress one while disabling
 * @sch. Once @sch becomes empty during disable, there's no point in dumping it.
 * This prevents calling dump ops on a dead sch.
 */
تهی scx_disable_dump(ساختار scx_sched *sch)
{
	guard(raw_spinlock_irqsave)(&scx_dump_lock);
	sch->dump_disabled = true;
}

تهی scx_log_sched_disable(ساختار scx_sched *sch)
{
	ساختار scx_exit_info *ei = sch->exit_info;
	ثابت نویسه *type = scx_parent(sch) ? "sub-scheduler" : "scheduler";

	اگر (ei->kind >= SCX_EXIT_ERROR) {
		pr_err("sched_ext: BPF %s \"%s\" disabled (%s)\n", type,
		       sch->ops.name, ei->reason);

		اگر (ei->پیام[0] != '\0')
			pr_err("sched_ext: %s: %s\n", sch->ops.name, ei->پیام);
#ifdef CONFIG_STACKTRACE
		پشته_trace_print(ei->bt, ei->bt_len, 2);
#endif
	} وگرنه {
		pr_info("sched_ext: BPF %s \"%s\" disabled (%s)\n", type,
			sch->ops.name, ei->reason);
	}
}

ایستا تهی scx_ریشه_disable(ساختار scx_sched *sch)
{
	ساختار scx_task_iter sti;
	ساختار task_struct *p;
	bool was_switched_all;
	صحیح cpu;

	/* guarantee forward progress and wait for descendants to be disabled */
	scx_bypass(sch, true);
	drain_descendants(sch);

	انتخاب (scx_set_enable_state(SCX_DISABLING)) {
	حالت SCX_DISABLING:
		WARN_ONCE(true, "sched_ext: duplicate disabling instance?");
		بشکن;
	حالت SCX_DISABLED:
		pr_warn("sched_ext: ops error detected without ops (%s)\n",
			sch->exit_info->پیام);
		WARN_ON_ONCE(scx_set_enable_state(SCX_DISABLED) != SCX_DISABLING);
		برو done;
	پیشفرض:
		بشکن;
	}

	/*
	 * Here, every runnable task is guaranteed to make forward progress and
	 * we can safely use blocking synchronization constructs. Actually
	 * disable ops.
	 */
	mutex_lock(&scx_enable_mutex);

	was_switched_all = scx_switched_all();

	static_branch_disable(&__scx_switched_all);
	WRITE_ONCE(scx_switching_all, false);

	/*
	 * Shut down cgroup support before tasks so that the cgroup attach and
	 * migration paths don't race against scx_disable_and_exit_task().
	 */
	scx_cgroup_lock();
	scx_cgroup_enabled = false;
	scx_cgroup_exit(sch);
	scx_cgroup_unlock();

	/*
	 * The BPF scheduler is going away. All tasks including %TASK_DEAD ones
	 * must be switched out and exited synchronously.
	 */
	percpu_down_write(&scx_fork_rwsem);

	scx_init_task_enabled = false;

	scx_task_iter_start(&sti, پوچ);
	وقتی ((p = scx_task_iter_next_locked(&sti))) {
		unsigned صحیح queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
		ثابت ساختار sched_class *old_class = p->sched_class;
		ثابت ساختار sched_class *new_class = scx_setscheduler_class(p);

		update_rq_clock(task_rq(p));

		اگر (old_class != new_class)
			queue_flags |= DEQUEUE_CLASS;

		scoped_guard (sched_change, p, queue_flags) {
			p->sched_class = new_class;
		}

		scx_disable_and_exit_task(scx_task_sched(p), p);
	}
	scx_task_iter_stop(&sti);

	scx_disable_dump(sch);

	scx_cgroup_lock();
	set_cgroup_sched(sch_cgroup(sch), پوچ);
	scx_cgroup_unlock();

	percpu_up_write(&scx_fork_rwsem);

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
		ساختار rq *rq = cpu_rq(cpu);

		scoped_guard(rq_lock_irqsave, rq) {
			update_rq_clock(rq);
			اگر (was_switched_all) {
				اگر (WARN_ON_ONCE(dl_server_swap_bw(&rq->ext_server,
								   &rq->fair_server)))
					pr_warn("failed to re-attach fair_server on CPU %d\n", cpu);
			} وگرنه {
				dl_server_detach_bw(&rq->ext_server);
			}
		}
	}

	/* no task is on scx, turn off all the switches and flush in-progress calls */
	static_branch_disable(&__scx_enabled);
	static_branch_disable(&__scx_is_cid_type);
	اگر (sch->ops.flags & SCX_OPS_TID_TO_TASK)
		static_branch_disable(&__scx_tid_to_task_enabled);
	bitmap_zero(sch->has_op, SCX_OPI_END);
	scx_idle_disable();
	synchronize_rcu();
	اگر (sch->ops.flags & SCX_OPS_TID_TO_TASK)
		rhashtable_free_and_destroy(&scx_tid_hash, پوچ, پوچ);

	scx_log_sched_disable(sch);

	اگر (sch->ops.exit)
		SCX_CALL_OP(sch, exit, پوچ, sch->exit_info);

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
	cpus_read_lock();
	RCU_INIT_POINTER(scx_ریشه, پوچ);
	scx_cid_retire_tables();
	cpus_read_unlock();

	/*
	 * Delete the kobject from the hierarchy synchronously. Otherwise, sysfs
	 * could observe an object of the same name still in the hierarchy when
	 * the next scheduler is loaded.
	 */
#ifdef CONFIG_EXT_SUB_SCHED
	اگر (sch->sub_kset)
		kobject_del(&sch->sub_kset->kobj);
#endif
	/* not added if enable failed before scx_sched_sysfs_add() */
	اگر (sch->kobj.state_in_sysfs)
		kobject_del(&sch->kobj);

	free_kick_syncs();

	mutex_unlock(&scx_enable_mutex);

	WARN_ON_ONCE(scx_set_enable_state(SCX_DISABLED) != SCX_DISABLING);
done:
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
ایستا تهی scx_propagate_exit_irq_workfn(ساختار irq_work *irq_work)
{
	ساختار scx_sched *sch = container_of(irq_work, ساختار scx_sched,
					     propagate_exit_irq_work);
	ساختار scx_sched *pos;

	scoped_guard (raw_spinlock_irqsave, &scx_sched_lock) {
		scx_for_each_descendant_pre(pos, sch)
			scx_disable(pos, SCX_EXIT_PARENT);
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
ایستا bool scx_claim_exit(ساختار scx_sched *sch, شمارش scx_exit_kind kind)
{
	صحیح none = SCX_EXIT_NONE;

	lockdep_assert_preemption_disabled();

	اگر (WARN_ON_ONCE(kind == SCX_EXIT_NONE || kind == SCX_EXIT_DONE))
		kind = SCX_EXIT_ERROR;

	اگر (!atomic_try_cmpxchg(&sch->exit_kind, &none, kind))
		بازگشت false;

	اگر (kind == SCX_EXIT_PARENT) {
		/* an ancestor is already sweeping the subtree */
		WRITE_ONCE(sch->aborting, true);
	} وگرنه {
		ساختار scx_sched *pos;

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

		irq_work_queue(&sch->propagate_exit_irq_work);
	}

	/* fired after ->aborting is set so callbacks can't delay recovery */
	trace_sched_ext_exit(sch, kind);

	بازگشت true;
}

ایستا تهی scx_disable_workfn(ساختار kthread_work *work)
{
	ساختار scx_sched *sch = container_of(work, ساختار scx_sched, disable_work);
	ساختار scx_exit_info *ei = sch->exit_info;
	صحیح kind;

	kind = atomic_read(&sch->exit_kind);
	وقتی (true) {
		اگر (kind == SCX_EXIT_DONE)	/* already disabled? */
			بازگشت;
		WARN_ON_ONCE(kind == SCX_EXIT_NONE);
		اگر (atomic_try_cmpxchg(&sch->exit_kind, &kind, SCX_EXIT_DONE))
			بشکن;
	}
	ei->kind = kind;
	ei->reason = scx_exit_reason(ei->kind);

	اگر (scx_parent(sch))
		scx_sub_disable(sch);
	وگرنه
		scx_ریشه_disable(sch);
}

ایستا تهی scx_disable(ساختار scx_sched *sch, شمارش scx_exit_kind kind)
{
	guard(preempt)();
	اگر (scx_claim_exit(sch, kind))
		irq_work_queue(&sch->disable_irq_work);
}

/**
 * scx_flush_disable_work - flush the disable work and wait for it to finish
 * @sch: the scheduler
 *
 * sch->disable_work might still not queued, causing kthread_flush_work()
 * as a noop. Syncing the irq_work first is required to guarantee the
 * kthread work has been queued before waiting for it.
 */
تهی scx_flush_disable_work(ساختار scx_sched *sch)
{
	صحیح kind;

	انجامبده {
		irq_work_sync(&sch->disable_irq_work);
		kthread_flush_work(&sch->disable_work);
		kind = atomic_read(&sch->exit_kind);
	} وقتی (kind != SCX_EXIT_NONE && kind != SCX_EXIT_DONE);
}

ایستا تهی dump_newline(ساختار seq_buf *s)
{
	trace_sched_ext_dump("");

	/* @s may be zero sized and seq_buf triggers WARN if so */
	اگر (s->size)
		seq_buf_putc(s, '\n');
}

__printf(2, 3) تهی scx_dump_سطر(ساختار seq_buf *s, ثابت نویسه *fmt, ...)
{
	va_list args;

#ifdef CONFIG_TRACEPOINTS
	اگر (trace_sched_ext_dump_enabled()) {
		/* protected by scx_dump_lock */
		ایستا نویسه سطر_buf[SCX_EXIT_پیام_LEN];

		va_start(args, fmt);
		vscnprintf(سطر_buf, اندازه(سطر_buf), fmt, args);
		va_end(args);

		trace_call__sched_ext_dump(سطر_buf);
	}
#endif
	/* @s may be zero sized and seq_buf triggers WARN if so */
	اگر (s->size) {
		va_start(args, fmt);
		seq_buf_vprintf(s, fmt, args);
		va_end(args);

		seq_buf_putc(s, '\n');
	}
}

ایستا تهی dump_پشته_trace(ساختار seq_buf *s, ثابت نویسه *prefix,
			     ثابت unsigned long *bt, unsigned صحیح len)
{
	unsigned صحیح i;

	برای (i = 0; i < len; i++)
		scx_dump_سطر(s, "%s%pS", prefix, (تهی *)bt[i]);
}

ایستا تهی ops_dump_init(ساختار seq_buf *s, ثابت نویسه *prefix)
{
	ساختار scx_dump_data *dd = &scx_dump_data;

	lockdep_assert_irqs_disabled();

	dd->cpu = smp_processor_id();		/* allow scx_bpf_dump() */
	dd->first = true;
	dd->cursor = 0;
	dd->s = s;
	dd->prefix = prefix;
}

ایستا تهی ops_dump_flush(تهی)
{
	ساختار scx_dump_data *dd = &scx_dump_data;
	نویسه *سطر = dd->buf.سطر;

	اگر (!dd->cursor)
		بازگشت;

	/*
	 * There's something to flush and this is the first line. Insert a blank
	 * line to distinguish ops dump.
	 */
	اگر (dd->first) {
		dump_newline(dd->s);
		dd->first = false;
	}

	/*
	 * There may be multiple lines in $line. Scan and emit each line
	 * separately.
	 */
	وقتی (true) {
		نویسه *end = سطر;
		نویسه c;

		وقتی (*end != '\n' && *end != '\0')
			end++;

		/*
		 * If $line overflowed, it may not have newline at the end.
		 * Always emit with a newline.
		 */
		c = *end;
		*end = '\0';
		scx_dump_سطر(dd->s, "%s%s", dd->prefix, سطر);
		اگر (c == '\0')
			بشکن;

		/* move to the next line */
		end++;
		اگر (*end == '\0')
			بشکن;
		سطر = end;
	}

	dd->cursor = 0;
}

ایستا تهی ops_dump_exit(تهی)
{
	ops_dump_flush();
	scx_dump_data.cpu = -1;
}

ایستا تهی scx_dump_task(ساختار scx_sched *sch, ساختار seq_buf *s, ساختار scx_dump_ctx *dctx,
			  ساختار rq *rq, ساختار task_struct *p, نویسه marker)
{
	ایستا unsigned long bt[SCX_EXIT_BT_LEN];
	ساختار scx_sched *task_sch = scx_task_sched(p);
	ثابت نویسه *own_marker;
	نویسه sch_id_buf[32];
	نویسه dsq_id_buf[19] = "(n/a)";
	unsigned long ops_state = atomic_long_read(&p->scx.ops_state);
	unsigned صحیح bt_len = 0;

	own_marker = task_sch == sch ? "*" : "";

	اگر (task_sch->level == 0)
		scnprintf(sch_id_buf, اندازه(sch_id_buf), "root");
	وگرنه
		scnprintf(sch_id_buf, اندازه(sch_id_buf), "sub%d-%llu",
			  task_sch->level, task_sch->ops.sub_cgroup_id);

	اگر (p->scx.dsq)
		scnprintf(dsq_id_buf, اندازه(dsq_id_buf), "0x%llx",
			  (unsigned long long)p->scx.dsq->id);

	dump_newline(s);
	scx_dump_سطر(s, " %c%c %s[%d] %s%s %+ldms",
		      marker, task_state_to_char(p), p->comm, p->pid, own_marker, sch_id_buf,
		      jiffies_delta_msecs(p->scx.runnable_at, dctx->at_jiffies));
	scx_dump_سطر(s, "      scx_state/flags=%u/0x%x dsq_flags=0x%x ops_state/qseq=%lu/%lu",
		      scx_get_task_state(p) >> SCX_TASK_STATE_SHIFT,
		      p->scx.flags & ~SCX_TASK_STATE_MASK, p->scx.dsq_flags,
		      ops_state & SCX_OPSS_STATE_MASK, ops_state >> SCX_OPSS_QSEQ_SHIFT);
	scx_dump_سطر(s, "      sticky/holding_cpu=%d/%d dsq_id=%s",
		      p->scx.sticky_cpu, p->scx.holding_cpu, dsq_id_buf);
	scx_dump_سطر(s, "      dsq_vtime=%llu slice=%llu weight=%u",
		      p->scx.dsq_vtime, p->scx.slice, p->scx.weight);
	scx_dump_سطر(s, "      cpus=%*pb no_mig=%u", cpumask_pr_args(p->cpus_ptr),
		      p->migration_disabled);

	اگر (SCX_HAS_OP(sch, dump_task)) {
		ops_dump_init(s, "    ");
		SCX_CALL_OP(sch, dump_task, rq, dctx, p);
		ops_dump_exit();
	}

#ifdef CONFIG_STACKTRACE
	bt_len = پشته_trace_save_tsk(p, bt, SCX_EXIT_BT_LEN, 1);
#endif
	اگر (bt_len) {
		dump_newline(s);
		dump_پشته_trace(s, "    ", bt, bt_len);
	}
}

ایستا تهی scx_dump_cpu(ساختار scx_sched *sch, ساختار seq_buf *s,
			 ساختار scx_dump_ctx *dctx, صحیح cpu,
			 bool dump_all_tasks)
{
	ساختار rq *rq = cpu_rq(cpu);
	ساختار scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);
	ساختار rq_flags rf;
	ساختار task_struct *p;
	ساختار seq_buf ns;
	size_t avail, used;
	نویسه *buf;
	bool idle;

	rq_lock_irqsave(rq, &rf);

	idle = list_empty(&rq->scx.runnable_list) &&
		rq->curr->sched_class == &idle_sched_class;

	اگر (idle && !SCX_HAS_OP(sch, dump_cpu))
		برو next;

	/*
	 * We don't yet know whether ops.dump_cpu() will produce output
	 * and we may want to skip the default CPU dump if it doesn't.
	 * Use a nested seq_buf to generate the standard dump so that we
	 * can decide whether to commit later.
	 */
	avail = seq_buf_get_buf(s, &buf);
	seq_buf_init(&ns, buf, avail);

	dump_newline(&ns);
	scx_dump_سطر(&ns, "CPU %-4d: nr_run=%u flags=0x%x cpu_rel=%d ksync=%lu",
		      cpu, rq->scx.nr_running, rq->scx.flags, rq->scx.cpu_released,
		      rq->scx.kick_sync);
	scx_rescue_dump(&ns, rq);
	scx_dump_سطر(&ns, "          curr=%s[%d] class=%ps",
		      rq->curr->comm, rq->curr->pid, rq->curr->sched_class);
	اگر (!cpumask_empty(pcpu->cpus_to_kick))
		scx_dump_سطر(&ns, "  cpus_to_kick   : %*pb",
			      cpumask_pr_args(pcpu->cpus_to_kick));
	اگر (!cpumask_empty(pcpu->cpus_to_kick_if_idle))
		scx_dump_سطر(&ns, "  idle_to_kick   : %*pb",
			      cpumask_pr_args(pcpu->cpus_to_kick_if_idle));
	اگر (!cpumask_empty(pcpu->cpus_to_preempt))
		scx_dump_سطر(&ns, "  cpus_to_preempt: %*pb",
			      cpumask_pr_args(pcpu->cpus_to_preempt));
	اگر (!cpumask_empty(pcpu->cpus_to_wait))
		scx_dump_سطر(&ns, "  cpus_to_wait   : %*pb",
			      cpumask_pr_args(pcpu->cpus_to_wait));
	اگر (!cpumask_empty(rq->scx.cpus_to_sync))
		scx_dump_سطر(&ns, "  cpus_to_sync   : %*pb",
			      cpumask_pr_args(rq->scx.cpus_to_sync));

	used = seq_buf_used(&ns);
	اگر (SCX_HAS_OP(sch, dump_cpu)) {
		ops_dump_init(&ns, "  ");
		SCX_CALL_OP(sch, dump_cpu, rq, dctx, scx_cpu_arg(cpu), idle);
		ops_dump_exit();
	}

	/*
	 * If idle && nothing generated by ops.dump_cpu(), there's
	 * nothing interesting. Skip.
	 */
	اگر (idle && used == seq_buf_used(&ns))
		برو next;

	/*
	 * $s may already have overflowed when $ns was created. If so,
	 * calling commit on it will trigger BUG.
	 */
	اگر (avail) {
		seq_buf_commit(s, seq_buf_used(&ns));
		اگر (seq_buf_has_overflowed(&ns))
			seq_buf_set_overflow(s);
	}

	اگر (rq->curr->sched_class == &ext_sched_class &&
	    (dump_all_tasks || scx_task_on_sched(sch, rq->curr)))
		scx_dump_task(sch, s, dctx, rq, rq->curr, '*');

	list_for_each_entry(p, &rq->scx.runnable_list, scx.runnable_گره)
		اگر (dump_all_tasks || scx_task_on_sched(sch, p))
			scx_dump_task(sch, s, dctx, rq, p, ' ');
next:
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Dump scheduler state. If @dump_all_tasks is true, dump all tasks regardless
 * of which scheduler they belong to. If false, only dump tasks owned by @sch.
 * For SysRq-D dumps, @dump_all_tasks=false since all schedulers are dumped
 * separately. For error dumps, @dump_all_tasks=true since only the failing
 * scheduler is dumped.
 */
ایستا تهی scx_dump_state(ساختار scx_sched *sch, ساختار scx_exit_info *ei,
			   size_t dump_len, bool dump_all_tasks)
{
	ایستا ثابت نویسه trunc_marker[] = "\n\n~~~~ TRUNCATED ~~~~\n";
	ساختار scx_dump_ctx dctx = {
		.kind = ei->kind,
		.exit_code = ei->exit_code,
		.reason = ei->reason,
		.at_ns = ktime_get_ns(),
		.at_jiffies = jiffies,
	};
	ساختار seq_buf s;
	ساختار scx_event_stats events;
	صحیح cpu;

	guard(raw_spinlock_irqsave)(&scx_dump_lock);

	اگر (sch->dump_disabled)
		بازگشت;

	seq_buf_init(&s, ei->dump, dump_len);

#ifdef CONFIG_EXT_SUB_SCHED
	اگر (sch->level == 0)
		scx_dump_سطر(&s, "%s: root", sch->ops.name);
	وگرنه
		scx_dump_سطر(&s, "%s: sub%d-%llu %s",
			      sch->ops.name, sch->level, sch->ops.sub_cgroup_id,
			      sch->cgrp_path);
#endif
	اگر (ei->kind == SCX_EXIT_NONE) {
		scx_dump_سطر(&s, "Debug dump triggered by %s", ei->reason);
	} وگرنه {
		اگر (ei->exit_cpu >= 0)
			scx_dump_سطر(&s, "%s[%d] triggered exit kind %d on CPU %d:",
				      current->comm, current->pid, ei->kind,
				      ei->exit_cpu);
		وگرنه
			scx_dump_سطر(&s, "%s[%d] triggered exit kind %d:",
				      current->comm, current->pid, ei->kind);
		scx_dump_سطر(&s, "  %s (%s)", ei->reason, ei->پیام);
		dump_newline(&s);
		scx_dump_سطر(&s, "Backtrace:");
		dump_پشته_trace(&s, "  ", ei->bt, ei->bt_len);
	}

	اگر (SCX_HAS_OP(sch, dump)) {
		ops_dump_init(&s, "");
		SCX_CALL_OP(sch, dump, پوچ, &dctx);
		ops_dump_exit();
	}

	dump_newline(&s);
	scx_dump_سطر(&s, "CPU states");
	scx_dump_سطر(&s, "----------");

	/*
	 * Dump stalled CPUs first so they aren't lost to dump truncation, then
	 * walk the rest in order. Fall back to exit_cpu if no stall mask set.
	 */
	اگر (!cpumask_empty(sch->stall_cpus)) {
		for_each_cpu(cpu, sch->stall_cpus)
			scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		for_each_possible_cpu(cpu) {
			اگر (!cpumask_test_cpu(cpu, sch->stall_cpus))
				scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		}
	} وگرنه {
		اگر (ei->exit_cpu >= 0)
			scx_dump_cpu(sch, &s, &dctx, ei->exit_cpu, dump_all_tasks);
		for_each_possible_cpu(cpu) {
			اگر (cpu != ei->exit_cpu)
				scx_dump_cpu(sch, &s, &dctx, cpu, dump_all_tasks);
		}
	}

	dump_newline(&s);
	scx_dump_سطر(&s, "Event counters");
	scx_dump_سطر(&s, "--------------");

	scx_read_events(sch, &events);
#define SCX_EVENT(name)	scx_dump_event(s, &events, name)
	SCX_EVENTS_LIST(SCX_EVENT);
#undef SCX_EVENT

	اگر (seq_buf_has_overflowed(&s) && dump_len >= اندازه(trunc_marker))
		memcpy(ei->dump + dump_len - اندازه(trunc_marker),
		       trunc_marker, اندازه(trunc_marker));
}

ایستا تهی scx_disable_irq_workfn(ساختار irq_work *irq_work)
{
	ساختار scx_sched *sch = container_of(irq_work, ساختار scx_sched, disable_irq_work);
	ساختار scx_exit_info *ei = sch->exit_info;

	اگر (ei->kind >= SCX_EXIT_ERROR)
		scx_dump_state(sch, ei, sch->ops.exit_dump_len, true);

	kthread_queue_work(sch->helper, &sch->disable_work);
}

/* finish exit_info and kick the disable work, ei->msg must already be set */
ایستا تهی scx_finish_exit(ساختار scx_sched *sch, شمارش scx_exit_kind kind,
			    s64 exit_code, s32 exit_cpu)
{
	ساختار scx_exit_info *ei = sch->exit_info;

	ei->exit_code = exit_code;
#ifdef CONFIG_STACKTRACE
	/*
	 * stack_trace_save()'s NMI-safety is arch-dependent and undocumented.
	 * Skip the backtrace when exiting from NMI.
	 */
	اگر (kind >= SCX_EXIT_ERROR && !in_nmi())
		ei->bt_len = پشته_trace_save(ei->bt, SCX_EXIT_BT_LEN, 1);
#endif
	/*
	 * Set ei->kind and ->reason for scx_dump_state(). They'll be set again
	 * in scx_disable_workfn().
	 */
	ei->kind = kind;
	ei->reason = scx_exit_reason(ei->kind);
	ei->exit_cpu = exit_cpu;

	irq_work_queue(&sch->disable_irq_work);
}

bool scx_vexit(ساختار scx_sched *sch,
	       شمارش scx_exit_kind kind, s64 exit_code, s32 exit_cpu,
	       ثابت نویسه *fmt, va_list args)
{
	ساختار scx_exit_info *ei = sch->exit_info;

	guard(preempt)();

	اگر (!scx_claim_exit(sch, kind))
		بازگشت false;

	vscnprintf(ei->پیام, SCX_EXIT_پیام_LEN, fmt, args);

	scx_finish_exit(sch, kind, exit_code, exit_cpu);
	بازگشت true;
}

ایستا صحیح alloc_kick_syncs(تهی)
{
	صحیح cpu;

	/*
	 * Allocate per-CPU arrays sized by nr_cpu_ids. Use kvzalloc as size
	 * can exceed percpu allocator limits on large machines.
	 */
	for_each_possible_cpu(cpu) {
		ساختار scx_kick_syncs __rcu **ksyncs = per_cpu_ptr(&scx_kick_syncs, cpu);
		ساختار scx_kick_syncs *new_ksyncs;

		WARN_ON_ONCE(rcu_access_pointer(*ksyncs));

		new_ksyncs = kvzalloc_گره(struct_size(new_ksyncs, syncs, nr_cpu_ids),
					   GFP_KERNEL, cpu_to_گره(cpu));
		اگر (!new_ksyncs) {
			free_kick_syncs();
			بازگشت -ENOMEM;
		}

		rcu_assign_pointer(*ksyncs, new_ksyncs);
	}

	بازگشت 0;
}

ایستا تهی free_pnode(ساختار scx_sched_pnode *pnode)
{
	اگر (!pnode)
		بازگشت;
	exit_dsq(&pnode->global_dsq);
	kfree(pnode);
}

ایستا ساختار scx_sched_pnode *alloc_pnode(ساختار scx_sched *sch, صحیح گره)
{
	ساختار scx_sched_pnode *pnode;

	pnode = kzalloc_گره(اندازه(*pnode), GFP_KERNEL, گره);
	اگر (!pnode)
		بازگشت پوچ;

	اگر (scx_init_dsq(&pnode->global_dsq, SCX_DSQ_GLOBAL, sch)) {
		kfree(pnode);
		بازگشت پوچ;
	}

	بازگشت pnode;
}

/*
 * Allocate and initialize a new scx_sched. @cgrp's reference is always
 * consumed whether the function succeeds or fails.
 */
ساختار scx_sched *scx_alloc_and_add_sched(ساختار scx_enable_cmd *cmd,
					  ساختار cgroup *cgrp,
					  ساختار scx_sched *parent)
{
	ساختار sched_ext_ops *ops = cmd->ops;
	ساختار scx_sched *sch;
	s32 level = parent ? parent->level + 1 : 0;
	s32 گره, cpu, ret, bypass_fail_cpu = nr_cpu_ids;

	sch = kzalloc_flex(*sch, ancestors, level + 1);
	اگر (!sch) {
		ret = -ENOMEM;
		برو err_put_cgrp;
	}

	sch->exit_info = alloc_exit_info(ops->exit_dump_len);
	اگر (!sch->exit_info) {
		ret = -ENOMEM;
		برو err_free_sch;
	}

	ret = rhashtable_init(&sch->dsq_hash, &dsq_hash_params);
	اگر (ret < 0)
		برو err_free_ei;

	sch->pnode = kzalloc_objs(sch->pnode[0], nr_گره_ids);
	اگر (!sch->pnode) {
		ret = -ENOMEM;
		برو err_free_hash;
	}

	for_each_گره_state(گره, N_POSSIBLE) {
		sch->pnode[گره] = alloc_pnode(sch, گره);
		اگر (!sch->pnode[گره]) {
			ret = -ENOMEM;
			برو err_free_pnode;
		}
	}

	sch->dsp_max_batch = ops->dispatch_max_batch ?: SCX_DSP_DFL_MAX_BATCH;
	sch->pcpu = __alloc_percpu(struct_size_t(ساختار scx_sched_pcpu,
						 dsp_ctx.buf, sch->dsp_max_batch),
				   __alignof__(ساختار scx_sched_pcpu));
	اگر (!sch->pcpu) {
		ret = -ENOMEM;
		برو err_free_pnode;
	}

	for_each_possible_cpu(cpu) {
		ret = scx_init_dsq(scx_bypass_dsq(sch, cpu), SCX_DSQ_BYPASS, sch);
		اگر (ret) {
			bypass_fail_cpu = cpu;
			برو err_free_pcpu;
		}
	}

	for_each_possible_cpu(cpu) {
		ساختار scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);

		گره = cpu_to_گره(cpu);
		pcpu->sch = sch;
		INIT_LIST_HEAD(&pcpu->deferred_reenq_local.گره);
#ifdef CONFIG_EXT_SUB_SCHED
		init_llist_گره(&pcpu->ecaps_to_sync_گره);
#endif
		INIT_LIST_HEAD(&pcpu->to_kick_گره);
		اگر (!zalloc_cpumask_var_گره(&pcpu->cpus_to_kick, GFP_KERNEL, گره) ||
		    !zalloc_cpumask_var_گره(&pcpu->cpus_to_kick_if_idle, GFP_KERNEL, گره) ||
		    !zalloc_cpumask_var_گره(&pcpu->cpus_to_preempt, GFP_KERNEL, گره) ||
		    !zalloc_cpumask_var_گره(&pcpu->cpus_to_wait, GFP_KERNEL, گره)) {
			ret = -ENOMEM;
			برو err_free_pcpu;
		}
	}

	sch->helper = kthread_run_worker(0, "sched_ext_helper");
	اگر (IS_ERR(sch->helper)) {
		ret = PTR_ERR(sch->helper);
		برو err_free_pcpu;
	}

	sched_set_fifo(sch->helper->task);

	اگر (parent)
		memcpy(sch->ancestors, parent->ancestors,
		       level * اندازه(parent->ancestors[0]));
	sch->ancestors[level] = sch;
	sch->level = level;
	sch->id = atomic64_inc_return(&scx_sched_id_cursor);

	اگر (ops->timeout_ms)
		sch->watchdog_timeout = msecs_to_jiffies(ops->timeout_ms);
	وگرنه
		sch->watchdog_timeout = SCX_WATCHDOG_MAX_TIMEOUT;

	sch->slice_dfl = SCX_SLICE_DFL;
	atomic_set(&sch->exit_kind, SCX_EXIT_NONE);
	sch->disable_irq_work = IRQ_WORK_INIT_HARD(scx_disable_irq_workfn);
	sch->propagate_exit_irq_work = IRQ_WORK_INIT_HARD(scx_propagate_exit_irq_workfn);
	kthread_init_work(&sch->disable_work, scx_disable_workfn);
	timer_setup(&sch->bypass_lb_timer, scx_bypass_lb_timerfn, 0);

	اگر (!alloc_cpumask_var(&sch->bypass_lb_donee_cpumask, GFP_KERNEL)) {
		ret = -ENOMEM;
		برو err_stop_helper;
	}
	اگر (!alloc_cpumask_var(&sch->bypass_lb_resched_cpumask, GFP_KERNEL)) {
		ret = -ENOMEM;
		برو err_free_lb_cpumask;
	}
	اگر (!zalloc_cpumask_var(&sch->stall_cpus, GFP_KERNEL)) {
		ret = -ENOMEM;
		برو err_free_lb_resched_cpumask;
	}
	/*
	 * Copy ops through the right union view. For cid-form the source is
	 * struct sched_ext_ops_cid which lacks the trailing cpu_acquire/
	 * cpu_release; those stay zero from kzalloc.
	 */
	اگر (cmd->is_cid_type) {
		sch->ops_cid = *cmd->ops_cid;
		sch->is_cid_type = true;
	} وگرنه {
		sch->ops = *cmd->ops;
	}

#ifdef CONFIG_EXT_SUB_SCHED
	نویسه *buf = kzalloc(PATH_MAX, GFP_KERNEL);
	اگر (!buf) {
		ret = -ENOMEM;
		برو err_free_lb_resched;
	}
	cgroup_path(cgrp, buf, PATH_MAX);
	sch->cgrp_path = kstrdup(buf, GFP_KERNEL);
	kfree(buf);
	اگر (!sch->cgrp_path) {
		ret = -ENOMEM;
		برو err_free_lb_resched;
	}

	sch->cgrp = cgrp;
	INIT_LIST_HEAD(&sch->children);
	INIT_LIST_HEAD(&sch->sibling);
#endif	/* CONFIG_EXT_SUB_SCHED */

	/*
	 * Publishing makes @sch visible to scx_prog_sched() readers. Failure
	 * paths after this point must free @sch through kobject_put() whose
	 * release path defers the actual freeing by an RCU grace period.
	 */
	rcu_assign_pointer(ops->priv, sch);

	sch->kobj.kset = scx_kset;
	INIT_LIST_HEAD(&sch->all);

#ifdef CONFIG_EXT_SUB_SCHED
	اگر (parent) {
		/*
		 * Pin @parent for @sch's lifetime. The kobject hierarchy pins
		 * it only via @parent->sub_kset, which is dropped during
		 * disable. Released in scx_sched_free_rcu_work().
		 */
		kobject_get(&parent->kobj);
	}
#endif	/* CONFIG_EXT_SUB_SCHED */

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
#اگر defined(CONFIG_MMU) && defined(CONFIG_64BIT)
	اگر (sch->arena_map)
		sch->arena_kern_base = bpf_arena_map_kern_vm_start(sch->arena_map);
#endif
	cmd->arena_map = پوچ;
	بازگشت sch;

#ifdef CONFIG_EXT_SUB_SCHED
err_free_lb_resched:
	free_cpumask_var(sch->stall_cpus);
#endif
err_free_lb_resched_cpumask:
	free_cpumask_var(sch->bypass_lb_resched_cpumask);
err_free_lb_cpumask:
	free_cpumask_var(sch->bypass_lb_donee_cpumask);
err_stop_helper:
	kthread_destroy_worker(sch->helper);
err_free_pcpu:
	for_each_possible_cpu(cpu) {
		ساختار scx_sched_pcpu *pcpu = per_cpu_ptr(sch->pcpu, cpu);

		free_cpumask_var(pcpu->cpus_to_kick);
		free_cpumask_var(pcpu->cpus_to_kick_if_idle);
		free_cpumask_var(pcpu->cpus_to_preempt);
		free_cpumask_var(pcpu->cpus_to_wait);
	}
	for_each_possible_cpu(cpu) {
		اگر (cpu == bypass_fail_cpu)
			بشکن;
		exit_dsq(scx_bypass_dsq(sch, cpu));
	}
	free_percpu(sch->pcpu);
err_free_pnode:
	for_each_گره_state(گره, N_POSSIBLE)
		free_pnode(sch->pnode[گره]);
	kfree(sch->pnode);
err_free_hash:
	rhashtable_free_and_destroy(&sch->dsq_hash, پوچ, پوچ);
err_free_ei:
	free_exit_info(sch->exit_info);
err_free_sch:
	kfree(sch);
err_put_cgrp:
#ifdef CONFIG_EXT_SUB_SCHED
	cgroup_put(cgrp);
#endif
	بازگشت ERR_PTR(ret);
}

/*
 * Add @sch's kobject to sysfs, and create its sub_kset if the scheduler
 * implements ops.sub_attach. Called by the enable workfns once @sch's
 * sysfs-visible state is initialized.
 */
صحیح scx_sched_sysfs_add(ساختار scx_sched *sch)
{
#ifdef CONFIG_EXT_SUB_SCHED
	ساختار scx_sched *parent = scx_parent(sch);
	صحیح ret;

	اگر (parent)
		ret = kobject_add(&sch->kobj, &parent->sub_kset->kobj,
				  "sub-%llu", cgroup_id(sch_cgroup(sch)));
	وگرنه
		ret = kobject_add(&sch->kobj, پوچ, "root");
	اگر (ret < 0)
		بازگشت ret;

	اگر (sch->ops.sub_attach) {
		sch->sub_kset = kset_create_and_add("sub", پوچ, &sch->kobj);
		اگر (!sch->sub_kset)
			بازگشت -ENOMEM;
	}
	بازگشت 0;
#وگرنه
	بازگشت kobject_add(&sch->kobj, پوچ, "root");
#endif
}

ایستا صحیح check_hotplug_seq(ساختار scx_sched *sch,
			      ثابت ساختار sched_ext_ops *ops)
{
	unsigned long long global_hotplug_seq;

	/*
	 * If a hotplug event has occurred between when a scheduler was
	 * initialized, and when we were able to attach, exit and notify user
	 * space about it.
	 */
	اگر (ops->hotplug_seq) {
		global_hotplug_seq = atomic_long_read(&scx_hotplug_seq);
		اگر (ops->hotplug_seq != global_hotplug_seq) {
			scx_exit(sch, SCX_EXIT_UNREG_KERN,
				 SCX_ECODE_ACT_RESTART | SCX_ECODE_RSN_HOTPLUG,
				 "expected hotplug seq %llu did not match actual %llu",
				 ops->hotplug_seq, global_hotplug_seq);
			بازگشت -EBUSY;
		}
	}

	بازگشت 0;
}

صحیح scx_validate_ops(ساختار scx_sched *sch, ثابت ساختار sched_ext_ops *ops)
{
	/*
	 * It doesn't make sense to specify the SCX_OPS_ENQ_LAST flag if the
	 * ops.enqueue() callback isn't implemented.
	 */
	اگر ((ops->flags & SCX_OPS_ENQ_LAST) && !ops->enqueue) {
		scx_error(sch, "SCX_OPS_ENQ_LAST requires ops.enqueue() to be implemented");
		بازگشت -EINVAL;
	}

	/*
	 * SCX_OPS_TID_TO_TASK is enabled by the root scheduler. A sub-sched
	 * may set it to declare a dependency; reject if the root hasn't
	 * enabled it.
	 */
	اگر ((ops->flags & SCX_OPS_TID_TO_TASK) && scx_parent(sch) &&
	    !(sch->ancestors[0]->ops.flags & SCX_OPS_TID_TO_TASK)) {
		scx_error(sch, "SCX_OPS_TID_TO_TASK requires root scheduler to enable it");
		بازگشت -EINVAL;
	}

	/*
	 * SCX_OPS_BUILTIN_IDLE_PER_NODE requires built-in CPU idle
	 * selection policy to be enabled.
	 */
	اگر ((ops->flags & SCX_OPS_BUILTIN_IDLE_PER_گره) &&
	    (ops->update_idle && !(ops->flags & SCX_OPS_KEEP_BUILTIN_IDLE))) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE requires CPU idle selection enabled");
		بازگشت -EINVAL;
	}

	/*
	 * cid-form's struct is shorter and doesn't include the cpu_acquire /
	 * cpu_release tail; reading those fields off a cid-form @ops would
	 * run past the BPF allocation. Skip for cid-form.
	 */
	اگر (!sch->is_cid_type && (ops->cpu_acquire || ops->cpu_release))
		pr_warn_ratelimited("ops->cpu_acquire/release() are deprecated, use sched_switch TP instead\n");

	/*
	 * Sub-scheduler support is tied to the cid-form struct_ops. A sub-sched
	 * attaches through a cid-form-only interface (sub_attach/sub_detach),
	 * and a root that accepts sub-scheds must expose cid-form state to
	 * them. Reject cpu-form schedulers on either side.
	 */
	اگر (!sch->is_cid_type) {
		اگر (scx_parent(sch)) {
			scx_error(sch, "sub-sched requires cid-form struct_ops");
			بازگشت -EINVAL;
		}
		اگر (ops->sub_attach || ops->sub_detach) {
			scx_error(sch, "sub_attach/sub_detach requires cid-form struct_ops");
			بازگشت -EINVAL;
		}
	}

	بازگشت 0;
}

ایستا تهی scx_ریشه_enable_workfn(ساختار kthread_work *work)
{
	ساختار scx_enable_cmd *cmd = container_of(work, ساختار scx_enable_cmd, work);
	ساختار sched_ext_ops *ops = cmd->ops;
	ساختار cgroup *cgrp = ریشه_cgroup();
	ساختار scx_sched *sch;
	ساختار scx_task_iter sti;
	ساختار task_struct *p;
	صحیح i, cpu, ret;

	mutex_lock(&scx_enable_mutex);

	اگر (scx_enable_state() != SCX_DISABLED) {
		ret = -EBUSY;
		برو err_unlock;
	}

	/*
	 * @ops->priv binds @ops to its scx_sched instance. It is set here by
	 * scx_alloc_and_add_sched() and cleared at the tail of bpf_scx_unreg(),
	 * which runs after scx_root_disable() has dropped scx_enable_mutex. If
	 * it's still non-NULL here, a previous attachment on @ops has not
	 * finished tearing down; proceeding would let the in-flight unreg's
	 * RCU_INIT_POINTER(NULL) clobber the @ops->priv we are about to assign.
	 */
	اگر (rcu_access_pointer(ops->priv)) {
		ret = -EBUSY;
		برو err_unlock;
	}

	ret = alloc_kick_syncs();
	اگر (ret)
		برو err_unlock;

	اگر (ops->flags & SCX_OPS_TID_TO_TASK) {
		ret = rhashtable_init(&scx_tid_hash, &scx_tid_hash_params);
		اگر (ret)
			برو err_free_ksyncs;
	}

#ifdef CONFIG_EXT_SUB_SCHED
	cgroup_get(cgrp);
#endif
	/*
	 * Transition to ENABLING to arm the disable path. Allocation failure
	 * still unwinds locally. Full disabling on failure applies only after
	 * scx_alloc_and_add_sched() succeeds.
	 */
	WARN_ON_ONCE(scx_set_enable_state(SCX_ENABLING) != SCX_DISABLED);
	WARN_ON_ONCE(scx_ریشه);

	sch = scx_alloc_and_add_sched(cmd, cgrp, پوچ);
	اگر (IS_ERR(sch)) {
		ret = PTR_ERR(sch);
		WARN_ON_ONCE(scx_set_enable_state(SCX_DISABLED) != SCX_ENABLING);
		برو err_free_tid_hash;
	}

	اگر (sch->is_cid_type)
		static_branch_enable(&__scx_is_cid_type);

	atomic_long_set(&scx_nr_rejected, 0);

	for_each_possible_cpu(cpu) {
		ساختار rq *rq = cpu_rq(cpu);

		rq->scx.local_dsq.sched = sch;
		rq->scx.cpuperf_target = SCX_CPUPERF_ONE;
	}

	scx_discard_stale_ecaps_syncs();
	scx_rescue_set_knobs(sch);

	/*
	 * Keep CPUs stable during enable so that the BPF scheduler can track
	 * online CPUs by watching ->on/offline_cpu() after ->init().
	 */
	cpus_read_lock();

	/*
	 * Build the cid mapping into a private under-construction set. It
	 * becomes visible to readers only through scx_cid_publish_tables() once
	 * ops.init_cids() has finalized the layout.
	 */
	ret = scx_cid_init(sch);
	اگر (ret) {
		cpus_read_unlock();
		برو err_disable;
	}

	/*
	 * Make the scheduler instance visible. Must be inside cpus_read_lock().
	 * See handle_hotplug().
	 */
	rcu_assign_pointer(scx_ریشه, sch);

	ret = scx_link_sched(sch);
	اگر (ret) {
		cpus_read_unlock();
		برو err_disable;
	}

	scx_idle_enable(ops);

	/*
	 * A cid-form scheduler finalizes its cid layout in ops.init_cids(),
	 * which may call scx_bpf_cid_override(). Run it before the caps and
	 * shard state are built so the final layout is in effect.
	 */
	اگر (sch->is_cid_type && sch->ops_cid.init_cids) {
		ret = SCX_CALL_OP_RET(sch, init_cids, پوچ);
		اگر (ret) {
			ret = scx_ops_sanitize_err(sch, "init_cids", ret);
			cpus_read_unlock();
			scx_error(sch, "ops.init_cids() failed (%d)", ret);
			برو err_disable;
		}
	}

	/* the cid layout is final, expose it to readers */
	scx_cid_publish_tables();

	ret = scx_arena_pool_init(sch);
	اگر (ret) {
		cpus_read_unlock();
		برو err_disable;
	}

	ret = scx_alloc_kern_arena_objs(sch);
	اگر (ret) {
		cpus_read_unlock();
		برو err_disable;
	}

	ret = scx_alloc_pshards(sch);
	اگر (ret) {
		cpus_read_unlock();
		برو err_disable;
	}

	scx_init_ریشه_caps(sch);

	/* the cid caps and shards are live now, so ops.init() can query them */
	اگر (sch->ops.init) {
		ret = SCX_CALL_OP_RET(sch, init, پوچ);
		اگر (ret) {
			ret = scx_ops_sanitize_err(sch, "init", ret);
			cpus_read_unlock();
			scx_error(sch, "ops.init() failed (%d)", ret);
			برو err_disable;
		}
		sch->exit_info->flags |= SCX_EFLAG_INITIALIZED;
	}

	ret = scx_sched_sysfs_add(sch);
	اگر (ret) {
		cpus_read_unlock();
		برو err_disable;
	}

	برای (i = SCX_OPI_CPU_HOTPLUG_BEGIN; i < SCX_OPI_CPU_HOTPLUG_END; i++)
		اگر (((تهی (**)(تهی))ops)[i])
			set_bit(i, sch->has_op);

	ret = check_hotplug_seq(sch, ops);
	اگر (ret) {
		cpus_read_unlock();
		برو err_disable;
	}
	scx_idle_update_selcpu_topology(ops);

	cpus_read_unlock();

	ret = scx_validate_ops(sch, ops);
	اگر (ret)
		برو err_disable;

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
		ساختار rq *rq = cpu_rq(cpu);

		scoped_guard(rq_lock_irqsave, rq) {
			update_rq_clock(rq);
			ret = dl_server_attach_bw(&rq->ext_server);
		}
		اگر (ret) {
			pr_warn("sched_ext: failed to attach ext_server on CPU %d (%d)\n",
				cpu, ret);
			برو err_disable;
		}
	}

	/*
	 * Once __scx_enabled is set, %current can be switched to SCX anytime.
	 * This can lead to stalls as some BPF schedulers (e.g. userspace
	 * scheduling) may not function correctly before all tasks are switched.
	 * Init in bypass mode to guarantee forward progress.
	 */
	scx_bypass(sch, true);

	برای (i = SCX_OPI_NORMAL_BEGIN; i < SCX_OPI_NORMAL_END; i++)
		اگر (((تهی (**)(تهی))ops)[i])
			set_bit(i, sch->has_op);

	اگر (sch->ops.cpu_acquire || sch->ops.cpu_release)
		sch->ops.flags |= SCX_OPS_HAS_CPU_PREEMPT;

	/*
	 * Lock out forks, cgroup on/offlining and moves before opening the
	 * floodgate so that they don't wander into the operations prematurely.
	 */
	percpu_down_write(&scx_fork_rwsem);

	WARN_ON_ONCE(scx_init_task_enabled);
	scx_init_task_enabled = true;

	/* flip under fork_rwsem; the iter below covers existing tasks */
	اگر (ops->flags & SCX_OPS_TID_TO_TASK)
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
	scx_cgroup_lock();
	set_cgroup_sched(sch_cgroup(sch), sch);
	ret = scx_cgroup_init(sch);
	اگر (ret)
		برو err_disable_unlock_all;

	WARN_ON_ONCE(scx_cgroup_enabled);
	scx_cgroup_enabled = true;

	scx_task_iter_start(&sti, پوچ);
	وقتی ((p = scx_task_iter_next_locked(&sti))) {
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
		scx_set_task_state(p, SCX_TASK_INIT_BEGIN);
		scx_task_iter_unlock(&sti);

		ret = __scx_init_task(sch, p, پوچ, false);

		scx_task_iter_relock(&sti, p);

		اگر (unlikely(ret)) {
			اگر (scx_get_task_state(p) != SCX_TASK_DEAD)
				scx_set_task_state(p, SCX_TASK_NONE);
			scx_task_iter_stop(&sti);
			scx_error(sch, "ops.init_task() failed (%d) for %s[%d]",
				  ret, p->comm, p->pid);
			put_task_struct(p);
			برو err_disable_unlock_all;
		}

		اگر (scx_get_task_state(p) == SCX_TASK_DEAD) {
			/*
			 * sched_ext_dead() observed %INIT_BEGIN and set %DEAD.
			 * ops.exit_task() is owed to the sched __scx_init_task()
			 * ran against; call it now.
			 */
			scx_sub_init_cancel_task(sch, p);
		} وگرنه {
			scx_set_task_state(p, SCX_TASK_INIT);
			scx_set_task_sched(p, sch);
			scx_set_task_state(p, SCX_TASK_READY);
		}

		/*
		 * Insert into the tid hash. scx_tasks_lock is held by the iter;
		 * list_empty() guards against sched_ext_dead() having taken @p
		 * off the list while init ran unlocked.
		 */
		اگر (scx_tid_to_task_enabled() && !list_empty(&p->scx.tasks_گره))
			scx_tid_hash_درج(p);

		put_task_struct(p);
	}
	scx_task_iter_stop(&sti);
	scx_cgroup_unlock();
	percpu_up_write(&scx_fork_rwsem);

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
	percpu_down_write(&scx_fork_rwsem);
	scx_task_iter_start(&sti, پوچ);
	وقتی ((p = scx_task_iter_next_locked(&sti))) {
		unsigned صحیح queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE;
		ثابت ساختار sched_class *old_class = p->sched_class;
		ثابت ساختار sched_class *new_class = scx_setscheduler_class(p);

		اگر (scx_get_task_state(p) != SCX_TASK_READY)
			ادامه;

		اگر (old_class != new_class)
			queue_flags |= DEQUEUE_CLASS;

		scoped_guard (sched_change, p, queue_flags) {
			scx_set_task_slice(p, READ_ONCE(sch->slice_dfl));
			p->sched_class = new_class;
		}
	}
	scx_task_iter_stop(&sti);
	percpu_up_write(&scx_fork_rwsem);

	scx_bypass(sch, false);

	اگر (!scx_tryset_enable_state(SCX_ENABLED, SCX_ENABLING)) {
		WARN_ON_ONCE(atomic_read(&sch->exit_kind) == SCX_EXIT_NONE);
		ret = -EBUSY;
		برو err_disable;
	}

	اگر (!(ops->flags & SCX_OPS_SWITCH_PARTIAL))
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
	اگر (scx_switched_all()) {
		for_each_possible_cpu(cpu) {
			ساختار rq *rq = cpu_rq(cpu);

			guard(rq_lock_irqsave)(rq);
			update_rq_clock(rq);
			dl_server_detach_bw(&rq->fair_server);
		}
	}

	pr_info("sched_ext: BPF scheduler \"%s\" enabled%s\n",
		sch->ops.name, scx_switched_all() ? "" : " (partial)");
	kobject_uevent(&sch->kobj, KOBJ_ADD);
	mutex_unlock(&scx_enable_mutex);

	atomic_long_inc(&scx_enable_seq);

	cmd->ret = 0;
	بازگشت;

err_free_tid_hash:
	اگر (ops->flags & SCX_OPS_TID_TO_TASK)
		rhashtable_free_and_destroy(&scx_tid_hash, پوچ, پوچ);
err_free_ksyncs:
	free_kick_syncs();
err_unlock:
	mutex_unlock(&scx_enable_mutex);
	cmd->ret = ret;
	بازگشت;

err_disable_unlock_all:
	scx_cgroup_unlock();
	percpu_up_write(&scx_fork_rwsem);
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

ایستا s32 scx_enable(ساختار scx_enable_cmd *cmd, ساختار bpf_link *link)
{
	ایستا ساختار kthread_worker *helper;
	ایستا DEFINE_MUTEX(helper_mutex);

	اگر (housekeeping_enabled(HK_TYPE_DOMAIN_BOOT)) {
		pr_err("sched_ext: Not compatible with \"isolcpus=\" domain isolation\n");
		بازگشت -EINVAL;
	}

	اگر (!READ_ONCE(helper)) {
		mutex_lock(&helper_mutex);
		اگر (!helper) {
			ساختار kthread_worker *w =
				kthread_run_worker(0, "scx_enable_helper");
			اگر (IS_ERR_OR_NULL(w)) {
				mutex_unlock(&helper_mutex);
				بازگشت -ENOMEM;
			}
			sched_set_fifo(w->task);
			WRITE_ONCE(helper, w);
		}
		mutex_unlock(&helper_mutex);
	}

#ifdef CONFIG_EXT_SUB_SCHED
	اگر (cmd->ops->sub_cgroup_id > 1)
		kthread_init_work(&cmd->work, scx_sub_enable_workfn);
	وگرنه
#endif	/* CONFIG_EXT_SUB_SCHED */
		kthread_init_work(&cmd->work, scx_ریشه_enable_workfn);

	kthread_queue_work(READ_ONCE(helper), &cmd->work);
	kthread_flush_work(&cmd->work);
	بازگشت cmd->ret;
}


/********************************************************************************
 * bpf_struct_ops plumbing.
 */
#شامل <linux/bpf_verifier.h>
#شامل <linux/bpf.h>
#شامل <linux/btf.h>

ایستا ثابت ساختار btf_type *task_struct_type;

ایستا bool bpf_scx_is_valid_access(صحیح off, صحیح size,
				    شمارش bpf_access_type type,
				    ثابت ساختار bpf_prog *prog,
				    ساختار bpf_insn_access_aux *info)
{
	اگر (type != BPF_READ)
		بازگشت false;
	اگر (off < 0 || off >= اندازه(__u64) * MAX_BPF_FUNC_ARGS)
		بازگشت false;
	اگر (off % size != 0)
		بازگشت false;

	بازگشت btf_ctx_access(off, size, type, prog, info);
}

/* common to both forms: only scx.disallow is writable */
ایستا صحیح bpf_scx_btf_struct_access_common(ثابت ساختار bpf_reg_state *reg,
					    صحیح off, صحیح size)
{
	ثابت ساختار btf_type *t;

	t = btf_type_by_id(reg->btf, reg->btf_id);
	اگر (t == task_struct_type &&
	    off >= offsetof(ساختار task_struct, scx.disallow) &&
	    off + size <= offsetofend(ساختار task_struct, scx.disallow))
		بازگشت SCALAR_مقدار;

	بازگشت -EACCES;
}

ایستا صحیح bpf_scx_btf_struct_access(ساختار bpf_verifier_log *log,
				     ثابت ساختار bpf_reg_state *reg, صحیح off,
				     صحیح size)
{
	ثابت ساختار btf_type *t;

	t = btf_type_by_id(reg->btf, reg->btf_id);
	اگر (t == task_struct_type) {
		اگر ((off >= offsetof(ساختار task_struct, scx.slice) &&
		     off + size <= offsetofend(ساختار task_struct, scx.slice)) ||
		    (off >= offsetof(ساختار task_struct, scx.dsq_vtime) &&
		     off + size <= offsetofend(ساختار task_struct, scx.dsq_vtime)))
			بازگشت SCALAR_مقدار;
	}

	بازگشت bpf_scx_btf_struct_access_common(reg, off, size);
}

/* cid-form rejects direct slice and dsq_vtime writes in favor of the kfuncs */
ایستا صحیح bpf_scx_cid_btf_struct_access(ساختار bpf_verifier_log *log,
					 ثابت ساختار bpf_reg_state *reg, صحیح off,
					 صحیح size)
{
	بازگشت bpf_scx_btf_struct_access_common(reg, off, size);
}

ایستا ثابت ساختار bpf_verifier_ops bpf_scx_verifier_ops = {
	.get_func_proto = bpf_base_func_proto,
	.is_valid_access = bpf_scx_is_valid_access,
	.btf_struct_access = bpf_scx_btf_struct_access,
};

ایستا ثابت ساختار bpf_verifier_ops bpf_scx_cid_verifier_ops = {
	.get_func_proto = bpf_base_func_proto,
	.is_valid_access = bpf_scx_is_valid_access,
	.btf_struct_access = bpf_scx_cid_btf_struct_access,
};

ایستا صحیح bpf_scx_init_member(ثابت ساختار btf_type *t,
			       ثابت ساختار btf_member *member,
			       تهی *kdata, ثابت تهی *udata)
{
	ثابت ساختار sched_ext_ops *uops = udata;
	ساختار sched_ext_ops *ops = kdata;
	u32 moff = __btf_member_bit_offset(t, member) / 8;
	صحیح ret;

	انتخاب (moff) {
	حالت offsetof(ساختار sched_ext_ops, dispatch_max_batch):
		اگر (*(u32 *)(udata + moff) > INT_MAX)
			بازگشت -E2BIG;
		ops->dispatch_max_batch = *(u32 *)(udata + moff);
		بازگشت 1;
	حالت offsetof(ساختار sched_ext_ops, flags):
		اگر (*(u64 *)(udata + moff) & ~SCX_OPS_ALL_FLAGS)
			بازگشت -EINVAL;
		ops->flags = *(u64 *)(udata + moff);
		بازگشت 1;
	حالت offsetof(ساختار sched_ext_ops, name):
		ret = bpf_obj_name_cpy(ops->name, uops->name,
				       اندازه(ops->name));
		اگر (ret < 0)
			بازگشت ret;
		اگر (ret == 0)
			بازگشت -EINVAL;
		بازگشت 1;
	حالت offsetof(ساختار sched_ext_ops, timeout_ms):
		اگر (msecs_to_jiffies(*(u32 *)(udata + moff)) >
		    SCX_WATCHDOG_MAX_TIMEOUT)
			بازگشت -E2BIG;
		ops->timeout_ms = *(u32 *)(udata + moff);
		بازگشت 1;
	حالت offsetof(ساختار sched_ext_ops, exit_dump_len):
		ops->exit_dump_len =
			*(u32 *)(udata + moff) ?: SCX_EXIT_DUMP_DFL_LEN;
		بازگشت 1;
	حالت offsetof(ساختار sched_ext_ops, hotplug_seq):
		ops->hotplug_seq = *(u64 *)(udata + moff);
		بازگشت 1;
	حالت offsetof(ساختار sched_ext_ops, cid_shard_size):
		ops->cid_shard_size = *(u32 *)(udata + moff);
		بازگشت 1;
	حالت offsetof(ساختار sched_ext_ops, rescue_bandwidth_ppt): {
		u32 bw_ppt = *(u32 *)(udata + moff);

		اگر (bw_ppt > SCX_RESCUE_MAX_BW_PPT && bw_ppt != SCX_RESCUE_DISABLE)
			بازگشت -E2BIG;
		ops->rescue_bandwidth_ppt = bw_ppt;
		بازگشت 1;
	}
	حالت offsetof(ساختار sched_ext_ops, rescue_quantum_us): {
		u32 quantum_us = *(u32 *)(udata + moff);

		اگر (quantum_us > SCX_RESCUE_MAX_QUANTUM_US)
			بازگشت -E2BIG;
		اگر (quantum_us && quantum_us < SCX_RESCUE_MIN_QUANTUM_US)
			بازگشت -EINVAL;
		ops->rescue_quantum_us = quantum_us;
		بازگشت 1;
	}
#ifdef CONFIG_EXT_SUB_SCHED
	حالت offsetof(ساختار sched_ext_ops, sub_cgroup_id):
		ops->sub_cgroup_id = *(u64 *)(udata + moff);
		بازگشت 1;
#endif	/* CONFIG_EXT_SUB_SCHED */
	}

	بازگشت 0;
}

ایستا صحیح bpf_scx_check_member(ثابت ساختار btf_type *t,
				ثابت ساختار btf_member *member,
				ثابت ساختار bpf_prog *prog)
{
	u32 moff = __btf_member_bit_offset(t, member) / 8;

	انتخاب (moff) {
	حالت offsetof(ساختار sched_ext_ops, init_task):
#ifdef CONFIG_EXT_GROUP_SCHED
	حالت offsetof(ساختار sched_ext_ops, cgroup_init):
	حالت offsetof(ساختار sched_ext_ops, cgroup_exit):
	حالت offsetof(ساختار sched_ext_ops, cgroup_prep_move):
	حالت offsetof(ساختار sched_ext_ops, cgroup_set_bandwidth):
#endif
	حالت offsetof(ساختار sched_ext_ops, cpu_online):
	حالت offsetof(ساختار sched_ext_ops, cpu_offline):
	حالت offsetof(ساختار sched_ext_ops, init_cids):
	حالت offsetof(ساختار sched_ext_ops, init):
	حالت offsetof(ساختار sched_ext_ops, exit):
	حالت offsetof(ساختار sched_ext_ops, sub_attach):
	حالت offsetof(ساختار sched_ext_ops, sub_detach):
		بشکن;
	پیشفرض:
		اگر (prog->sleepable)
			بازگشت -EINVAL;
	}

#ifdef CONFIG_EXT_SUB_SCHED
	/*
	 * Enable private stack for operations that can nest along the
	 * hierarchy.
	 *
	 * XXX - Ideally, we should only do this for scheds that allow
	 * sub-scheds and sub-scheds themselves but I don't know how to access
	 * struct_ops from here.
	 */
	انتخاب (moff) {
	حالت offsetof(ساختار sched_ext_ops, dispatch):
		prog->aux->priv_پشته_requested = true;
		prog->aux->recursion_detected = scx_pstack_recursion_on_dispatch;
		بشکن;
	حالت offsetof(ساختار sched_ext_ops, sub_caps_updated):
		prog->aux->priv_پشته_requested = true;
		prog->aux->recursion_detected = scx_pstack_recursion_on_caps_updated;
		بشکن;
	}
#endif	/* CONFIG_EXT_SUB_SCHED */

	بازگشت 0;
}

ایستا صحیح bpf_scx_reg(تهی *kdata, ساختار bpf_link *link)
{
	ساختار scx_enable_cmd cmd = { .ops = kdata };

	بازگشت scx_enable(&cmd, link);
}

ساختار scx_arena_scan {
	ساختار bpf_map	*arena;
	صحیح		err;
};

/*
 * The verifier enforces one arena per BPF program, so each struct_ops
 * member prog contributes at most one arena via bpf_prog_arena().
 * Require all non-NULL contributions to match.
 */
ایستا صحیح scx_arena_scan_prog(ساختار bpf_prog *prog, تهی *data)
{
	ساختار scx_arena_scan *s = data;
	ساختار bpf_map *arena = پوچ;

	/* arena.o, which defines these, is built only on MMU && 64BIT */
#اگر defined(CONFIG_MMU) && defined(CONFIG_64BIT)
	arena = bpf_prog_arena(prog);
#endif
	اگر (!arena)
		بازگشت 0;
	اگر (s->arena && s->arena != arena) {
		s->err = -EINVAL;
		بازگشت 1;
	}
	s->arena = arena;
	بازگشت 0;
}

ایستا صحیح bpf_scx_reg_cid(تهی *kdata, ساختار bpf_link *link)
{
	ساختار scx_enable_cmd cmd = { .ops_cid = kdata, .is_cid_type = true };
	ساختار scx_arena_scan scan = {};
	صحیح ret;

	bpf_struct_ops_for_each_prog(kdata, scx_arena_scan_prog, &scan);
	اگر (scan.err) {
		pr_err("sched_ext: cid-form scheduler uses multiple arena maps\n");
		بازگشت scan.err;
	}
	اگر (!scan.arena) {
		pr_err("sched_ext: cid-form scheduler must use a BPF arena map\n");
		بازگشت -EINVAL;
	}

	bpf_map_inc(scan.arena);
	cmd.arena_map = scan.arena;
	ret = scx_enable(&cmd, link);
	اگر (cmd.arena_map)		/* not consumed by scx_alloc_and_add_sched() */
		bpf_map_put(cmd.arena_map);
	بازگشت ret;
}

ایستا تهی bpf_scx_unreg(تهی *kdata, ساختار bpf_link *link)
{
	ساختار sched_ext_ops *ops = kdata;
	ساختار scx_sched *sch = rcu_dereference_protected(ops->priv, true);

	scx_disable(sch, SCX_EXIT_UNREG);
	scx_flush_disable_work(sch);
	RCU_INIT_POINTER(ops->priv, پوچ);
	kobject_put(&sch->kobj);
}

ایستا صحیح bpf_scx_init(ساختار btf *btf)
{
	task_struct_type = btf_type_by_id(btf, btf_tracing_ids[BTF_TRACING_TYPE_TASK]);

	بازگشت 0;
}

ایستا صحیح bpf_scx_update(تهی *kdata, تهی *old_kdata, ساختار bpf_link *link)
{
	/*
	 * sched_ext does not support updating the actively-loaded BPF
	 * scheduler, as registering a BPF scheduler can always fail if the
	 * scheduler returns an error code for e.g. ops.init(), ops.init_task(),
	 * etc. Similarly, we can always race with unregistration happening
	 * elsewhere, such as with sysrq.
	 */
	بازگشت -EOPNOTSUPP;
}

ایستا صحیح bpf_scx_validate(تهی *kdata)
{
	بازگشت 0;
}

ایستا s32 sched_ext_ops__select_cpu(ساختار task_struct *p, s32 prev_cpu, u64 wake_flags) { بازگشت -EINVAL; }
ایستا تهی sched_ext_ops__enqueue(ساختار task_struct *p, u64 enq_flags) {}
ایستا تهی sched_ext_ops__dequeue(ساختار task_struct *p, u64 enq_flags) {}
ایستا تهی sched_ext_ops__dispatch(s32 prev_cpu, ساختار task_struct *prev__nullable) {}
ایستا تهی sched_ext_ops__tick(ساختار task_struct *p) {}
ایستا تهی sched_ext_ops__runnable(ساختار task_struct *p, u64 enq_flags) {}
ایستا تهی sched_ext_ops__running(ساختار task_struct *p) {}
ایستا تهی sched_ext_ops__stopping(ساختار task_struct *p, bool runnable) {}
ایستا تهی sched_ext_ops__quiescent(ساختار task_struct *p, u64 deq_flags) {}
ایستا bool sched_ext_ops__yield(ساختار task_struct *from, ساختار task_struct *to__nullable) { بازگشت false; }
ایستا bool sched_ext_ops__core_sched_before(ساختار task_struct *a, ساختار task_struct *b) { بازگشت false; }
ایستا تهی sched_ext_ops__set_weight(ساختار task_struct *p, u32 weight) {}
ایستا تهی sched_ext_ops__set_cpumask(ساختار task_struct *p, ثابت ساختار cpumask *mask) {}
ایستا تهی sched_ext_ops__update_idle(s32 cpu, bool idle) {}
ایستا تهی sched_ext_ops__cpu_acquire(s32 cpu, ساختار scx_cpu_acquire_args *args) {}
ایستا تهی sched_ext_ops__cpu_release(s32 cpu, ساختار scx_cpu_release_args *args) {}
ایستا s32 sched_ext_ops__init_task(ساختار task_struct *p, ساختار scx_init_task_args *args) { بازگشت -EINVAL; }
ایستا تهی sched_ext_ops__exit_task(ساختار task_struct *p, ساختار scx_exit_task_args *args) {}
ایستا تهی sched_ext_ops__enable(ساختار task_struct *p) {}
ایستا تهی sched_ext_ops__disable(ساختار task_struct *p) {}
#ifdef CONFIG_EXT_GROUP_SCHED
ایستا s32 sched_ext_ops__cgroup_init(ساختار cgroup *cgrp, ساختار scx_cgroup_init_args *args) { بازگشت -EINVAL; }
ایستا تهی sched_ext_ops__cgroup_exit(ساختار cgroup *cgrp) {}
ایستا s32 sched_ext_ops__cgroup_prep_move(ساختار task_struct *p, ساختار cgroup *from, ساختار cgroup *to) { بازگشت -EINVAL; }
ایستا تهی sched_ext_ops__cgroup_move(ساختار task_struct *p, ساختار cgroup *from, ساختار cgroup *to) {}
ایستا تهی sched_ext_ops__cgroup_cancel_move(ساختار task_struct *p, ساختار cgroup *from, ساختار cgroup *to) {}
ایستا تهی sched_ext_ops__cgroup_set_weight(ساختار cgroup *cgrp, u32 weight) {}
ایستا تهی sched_ext_ops__cgroup_set_bandwidth(ساختار cgroup *cgrp, u64 period_us, u64 quota_us, u64 burst_us) {}
ایستا تهی sched_ext_ops__cgroup_set_idle(ساختار cgroup *cgrp, bool idle) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
ایستا s32 sched_ext_ops__sub_attach(ساختار scx_sub_attach_args *args) { بازگشت -EINVAL; }
ایستا تهی sched_ext_ops__sub_detach(ساختار scx_sub_detach_args *args) {}
ایستا تهی sched_ext_ops__cpu_online(s32 cpu) {}
ایستا تهی sched_ext_ops__cpu_offline(s32 cpu) {}
ایستا s32 sched_ext_ops__init_cids(تهی) { بازگشت -EINVAL; }
ایستا s32 sched_ext_ops__init(تهی) { بازگشت -EINVAL; }
ایستا تهی sched_ext_ops__exit(ساختار scx_exit_info *info) {}
ایستا تهی sched_ext_ops__dump(ساختار scx_dump_ctx *ctx) {}
ایستا تهی sched_ext_ops__dump_cpu(ساختار scx_dump_ctx *ctx, s32 cpu, bool idle) {}
ایستا تهی sched_ext_ops__dump_task(ساختار scx_dump_ctx *ctx, ساختار task_struct *p) {}

ایستا ساختار sched_ext_ops __bpf_ops_sched_ext_ops = {
	.select_cpu		= sched_ext_ops__select_cpu,
	.enqueue		= sched_ext_ops__enqueue,
	.dequeue		= sched_ext_ops__dequeue,
	.dispatch		= sched_ext_ops__dispatch,
	.tick			= sched_ext_ops__tick,
	.runnable		= sched_ext_ops__runnable,
	.running		= sched_ext_ops__running,
	.stopping		= sched_ext_ops__stopping,
	.quiescent		= sched_ext_ops__quiescent,
	.yield			= sched_ext_ops__yield,
	.core_sched_before	= sched_ext_ops__core_sched_before,
	.set_weight		= sched_ext_ops__set_weight,
	.set_cpumask		= sched_ext_ops__set_cpumask,
	.update_idle		= sched_ext_ops__update_idle,
	.cpu_acquire		= sched_ext_ops__cpu_acquire,
	.cpu_release		= sched_ext_ops__cpu_release,
	.init_task		= sched_ext_ops__init_task,
	.exit_task		= sched_ext_ops__exit_task,
	.enable			= sched_ext_ops__enable,
	.disable		= sched_ext_ops__disable,
#ifdef CONFIG_EXT_GROUP_SCHED
	.cgroup_init		= sched_ext_ops__cgroup_init,
	.cgroup_exit		= sched_ext_ops__cgroup_exit,
	.cgroup_prep_move	= sched_ext_ops__cgroup_prep_move,
	.cgroup_move		= sched_ext_ops__cgroup_move,
	.cgroup_cancel_move	= sched_ext_ops__cgroup_cancel_move,
	.cgroup_set_weight	= sched_ext_ops__cgroup_set_weight,
	.cgroup_set_bandwidth	= sched_ext_ops__cgroup_set_bandwidth,
	.cgroup_set_idle	= sched_ext_ops__cgroup_set_idle,
#endif
	.sub_attach		= sched_ext_ops__sub_attach,
	.sub_detach		= sched_ext_ops__sub_detach,
	.cpu_online		= sched_ext_ops__cpu_online,
	.cpu_offline		= sched_ext_ops__cpu_offline,
	.init_cids		= sched_ext_ops__init_cids,
	.init			= sched_ext_ops__init,
	.exit			= sched_ext_ops__exit,
	.dump			= sched_ext_ops__dump,
	.dump_cpu		= sched_ext_ops__dump_cpu,
	.dump_task		= sched_ext_ops__dump_task,
};

ایستا ساختار bpf_struct_ops bpf_sched_ext_ops = {
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
ایستا تهی sched_ext_ops_cid__set_cmask(ساختار task_struct *p, ثابت ساختار scx_cmask *cmask__arena) {}
ایستا تهی sched_ext_ops_cid__enable(ساختار task_struct *p, ساختار scx_enable_args *args) {}
ایستا تهی sched_ext_ops__sub_caps_updated(ثابت ساختار scx_cmask *cmask__arena, u64 caps) {}
ایستا تهی sched_ext_ops__sub_ecaps_updated(s32 cid, u64 before, u64 after) {}

ایستا ساختار sched_ext_ops_cid __bpf_ops_sched_ext_ops_cid = {
	.select_cid		= sched_ext_ops__select_cpu,
	.enqueue		= sched_ext_ops__enqueue,
	.dequeue		= sched_ext_ops__dequeue,
	.dispatch		= sched_ext_ops__dispatch,
	.tick			= sched_ext_ops__tick,
	.runnable		= sched_ext_ops__runnable,
	.running		= sched_ext_ops__running,
	.stopping		= sched_ext_ops__stopping,
	.quiescent		= sched_ext_ops__quiescent,
	.yield			= sched_ext_ops__yield,
	.core_sched_before	= sched_ext_ops__core_sched_before,
	.set_weight		= sched_ext_ops__set_weight,
	.set_cmask		= sched_ext_ops_cid__set_cmask,
	.update_idle		= sched_ext_ops__update_idle,
	.init_task		= sched_ext_ops__init_task,
	.exit_task		= sched_ext_ops__exit_task,
	.enable			= sched_ext_ops_cid__enable,
	.disable		= sched_ext_ops__disable,
#ifdef CONFIG_EXT_GROUP_SCHED
	.cpuctl_init		= sched_ext_ops__cgroup_init,
	.cpuctl_exit		= sched_ext_ops__cgroup_exit,
	.cpuctl_prep_move	= sched_ext_ops__cgroup_prep_move,
	.cpuctl_move		= sched_ext_ops__cgroup_move,
	.cpuctl_cancel_move	= sched_ext_ops__cgroup_cancel_move,
	.cpuctl_set_weight	= sched_ext_ops__cgroup_set_weight,
	.cpuctl_set_bandwidth	= sched_ext_ops__cgroup_set_bandwidth,
	.cpuctl_set_idle	= sched_ext_ops__cgroup_set_idle,
#endif
	.sub_attach		= sched_ext_ops__sub_attach,
	.sub_detach		= sched_ext_ops__sub_detach,
	.sub_caps_updated	= sched_ext_ops__sub_caps_updated,
	.sub_ecaps_updated	= sched_ext_ops__sub_ecaps_updated,
	.cid_online		= sched_ext_ops__cpu_online,
	.cid_offline		= sched_ext_ops__cpu_offline,
	.init_cids		= sched_ext_ops__init_cids,
	.init			= sched_ext_ops__init,
	.exit			= sched_ext_ops__exit,
	.dump			= sched_ext_ops__dump,
	.dump_cid		= sched_ext_ops__dump_cpu,
	.dump_task		= sched_ext_ops__dump_task,
};

/*
 * The cid-form struct_ops shares all bpf_struct_ops hooks with the cpu form.
 * init_member, check_member, reg, unreg, etc. process kdata as the byte block
 * verified to match by the BUILD_BUG_ON checks in scx_init().
 */
ایستا ساختار bpf_struct_ops bpf_sched_ext_ops_cid = {
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

ایستا تهی sysrq_handle_sched_ext_reset(u8 کلید)
{
	ساختار scx_sched *sch;

	sch = rcu_dereference(scx_ریشه);
	اگر (likely(sch))
		scx_disable(sch, SCX_EXIT_SYSRQ);
	وگرنه
		pr_info("sched_ext: BPF schedulers not loaded\n");
}

ایستا ثابت ساختار sysrq_کلید_op sysrq_sched_ext_reset_op = {
	.handler	= sysrq_handle_sched_ext_reset,
	.help_پیام	= "reset-sched-ext(S)",
	.action_پیام	= "Disable sched_ext and revert all tasks to CFS",
	.enable_mask	= SYSRQ_ENABLE_RTNICE,
};

ایستا تهی sysrq_handle_sched_ext_dump(u8 کلید)
{
	ساختار scx_exit_info ei = {
		.kind		= SCX_EXIT_NONE,
		.exit_cpu	= -1,
		.reason		= "SysRq-D",
	};
	ساختار scx_sched *sch;

	list_for_each_entry_rcu(sch, &scx_sched_all, all)
		scx_dump_state(sch, &ei, 0, false);
}

ایستا ثابت ساختار sysrq_کلید_op sysrq_sched_ext_dump_op = {
	.handler	= sysrq_handle_sched_ext_dump,
	.help_پیام	= "dump-sched-ext(D)",
	.action_پیام	= "Trigger sched_ext debug dump",
	.enable_mask	= SYSRQ_ENABLE_RTNICE,
};

ایستا bool can_skip_idle_kick(ساختار rq *rq)
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
	بازگشت !is_idle_task(rq->curr) && !(rq->scx.flags & SCX_RQ_IN_DISPATCH);
}

ایستا bool kick_one_cpu(s32 cpu, ساختار scx_sched_pcpu *pcpu, ساختار rq *this_rq,
			 unsigned long *ksyncs)
{
	ساختار rq *rq = cpu_rq(cpu);
	ساختار scx_rq *this_scx = &this_rq->scx;
	ثابت ساختار sched_class *cur_class;
	bool should_wait = false;
	bool kickable;
	unsigned long flags;

	raw_spin_rq_lock_irqsave(rq, flags);
	cur_class = rq->curr->sched_class;

	/*
	 * During CPU hotplug, a CPU may depend on kicking itself to make
	 * forward progress. Allow kicking self regardless of online state. If
	 * @cpu is running a higher class task, we have no control over @cpu.
	 * Skip kicking. A sub-sched lacking baseline access on @cid has no
	 * business forcing a reschedule there - skip. This is the authoritative
	 * cap check: ecaps is read here under @rq's lock.
	 */
	kickable = (cpu_online(cpu) || cpu == cpu_of(this_rq)) &&
		   !sched_class_above(cur_class, &ext_sched_class);

	اگر (kickable && !scx_missing_caps(pcpu->sch, cpu, SCX_CAP_BASE)) {
		اگر (cpumask_test_cpu(cpu, pcpu->cpus_to_preempt)) {
			اگر (cur_class == &ext_sched_class) {
				u64 caps = scx_caps_for_preempt(pcpu->sch, rq, 0);

				اگر (unlikely(scx_missing_caps(pcpu->sch, cpu, caps)))
					__scx_add_event(pcpu->sch, SCX_EV_SUB_PREEMPT_DENIED, 1);
				وگرنه اگر (unlikely(!scx_set_task_slice(rq->curr, 0)))
					__scx_add_event(pcpu->sch, SCX_EV_SLICE_DENIED, 1);
			}
			cpumask_clear_cpu(cpu, pcpu->cpus_to_preempt);
		}

		اگر (cpumask_test_cpu(cpu, pcpu->cpus_to_wait)) {
			اگر (cur_class == &ext_sched_class) {
				cpumask_set_cpu(cpu, this_scx->cpus_to_sync);
				ksyncs[cpu] = rq->scx.kick_sync;
				should_wait = true;
			}
			cpumask_clear_cpu(cpu, pcpu->cpus_to_wait);
		}

		resched_curr(rq);
	} وگرنه {
		/* a kickable cpu was skipped solely for the missing caps */
		اگر (kickable)
			__scx_add_event(pcpu->sch, SCX_EV_SUB_KICK_DENIED, 1);
		cpumask_clear_cpu(cpu, pcpu->cpus_to_preempt);
		cpumask_clear_cpu(cpu, pcpu->cpus_to_wait);
	}

	scx_rq_lock_drop(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);

	بازگشت should_wait;
}

ایستا تهی kick_one_cpu_if_idle(s32 cpu, ساختار scx_sched_pcpu *pcpu,
				 ساختار rq *this_rq)
{
	ساختار rq *rq = cpu_rq(cpu);
	unsigned long flags;

	raw_spin_rq_lock_irqsave(rq, flags);

	/* idle kicks need baseline access too, see kick_one_cpu() */
	اگر (!can_skip_idle_kick(rq) &&
	    (cpu_online(cpu) || cpu == cpu_of(this_rq))) {
		اگر (likely(!scx_missing_caps(pcpu->sch, cpu, SCX_CAP_BASE)))
			resched_curr(rq);
		وگرنه
			__scx_add_event(pcpu->sch, SCX_EV_SUB_KICK_DENIED, 1);
	}

	scx_rq_lock_drop(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

ایستا تهی kick_cpus_irq_workfn(ساختار irq_work *irq_work)
{
	ساختار rq *this_rq = this_rq();
	ساختار scx_rq *this_scx = &this_rq->scx;
	ساختار scx_kick_syncs __rcu *ksyncs_pcpu = __this_cpu_read(scx_kick_syncs);
	ساختار scx_sched_pcpu *pcpu, *tmp;
	bool should_wait = false;
	unsigned long *ksyncs;
	s32 cpu;

	/* can race with free_kick_syncs() during scheduler disable */
	اگر (unlikely(!ksyncs_pcpu))
		بازگشت;

	ksyncs = rcu_dereference_bh(ksyncs_pcpu)->syncs;

	/*
	 * Walk scheds with pending kicks on this cpu. scx_kick_cpu() adds to
	 * the list under local_irq_save() and only this irq_work consumes it.
	 * A plain list without locking is sufficient.
	 */
	list_for_each_entry_safe(pcpu, tmp, &this_scx->sched_pcpus_to_kick, to_kick_گره) {
		list_del_init(&pcpu->to_kick_گره);

		for_each_cpu(cpu, pcpu->cpus_to_kick) {
			should_wait |= kick_one_cpu(cpu, pcpu, this_rq, ksyncs);
			cpumask_clear_cpu(cpu, pcpu->cpus_to_kick);
			cpumask_clear_cpu(cpu, pcpu->cpus_to_kick_if_idle);
		}

		for_each_cpu(cpu, pcpu->cpus_to_kick_if_idle) {
			kick_one_cpu_if_idle(cpu, pcpu, this_rq);
			cpumask_clear_cpu(cpu, pcpu->cpus_to_kick_if_idle);
		}
	}

	/*
	 * Can't wait in hardirq — kick_sync can't advance, deadlocking if
	 * CPUs wait for each other. Defer to kick_sync_wait_bal_cb().
	 */
	اگر (should_wait) {
		raw_spin_rq_lock(this_rq);
		this_scx->kick_sync_pending = true;
		resched_curr(this_rq);
		scx_rq_lock_drop(this_rq);
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
تهی print_scx_info(ثابت نویسه *log_lvl, ساختار task_struct *p)
{
	ساختار scx_sched *sch;
	شمارش scx_enable_state state = scx_enable_state();
	ثابت نویسه *all = READ_ONCE(scx_switching_all) ? "+all" : "";
	نویسه runnable_at_buf[22] = "?";
	ساختار sched_class *class;
	unsigned long runnable_at;

	guard(rcu)();

	sch = scx_task_sched_rcu(p);

	اگر (!sch)
		بازگشت;

	/*
	 * Carefully check if the task was running on sched_ext, and then
	 * carefully copy the time it's been runnable, and its state.
	 */
	اگر (copy_from_kernel_nofault(&class, &p->sched_class, اندازه(class)) ||
	    class != &ext_sched_class) {
		printk("%sSched_ext: %s (%s%s)", log_lvl, sch->ops.name,
		       scx_enable_state_str[state], all);
		بازگشت;
	}

	اگر (!copy_from_kernel_nofault(&runnable_at, &p->scx.runnable_at,
				      اندازه(runnable_at)))
		scnprintf(runnable_at_buf, اندازه(runnable_at_buf), "%+ldms",
			  jiffies_delta_msecs(runnable_at, jiffies));

	/* print everything onto one line to conserve console space */
	printk("%sSched_ext: %s (%s%s), task: runnable_at=%s",
	       log_lvl, sch->ops.name, scx_enable_state_str[state], all,
	       runnable_at_buf);
}

ایستا صحیح scx_pm_handler(ساختار notifier_block *nb, unsigned long event, تهی *ptr)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ریشه);
	اگر (!sch)
		بازگشت NOTIFY_OK;

	/*
	 * SCX schedulers often have userspace components which are sometimes
	 * involved in critial scheduling paths. PM operations involve freezing
	 * userspace which can lead to scheduling misbehaviors including stalls.
	 * Let's bypass while PM operations are in progress.
	 */
	انتخاب (event) {
	حالت PM_HIBERNATION_PREPARE:
	حالت PM_SUSPEND_PREPARE:
	حالت PM_RESTORE_PREPARE:
		scx_bypass(sch, true);
		بشکن;
	حالت PM_POST_HIBERNATION:
	حالت PM_POST_SUSPEND:
	حالت PM_POST_RESTORE:
		scx_bypass(sch, false);
		بشکن;
	}

	بازگشت NOTIFY_OK;
}

ایستا ساختار notifier_block scx_pm_notifier = {
	.notifier_call = scx_pm_handler,
};

تهی __init init_sched_ext_class(تهی)
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
		ساختار rq *rq = cpu_rq(cpu);
		صحیح  n = cpu_to_گره(cpu);

		/* local_dsq's sch will be set during scx_root_enable() */
		BUG_ON(scx_init_dsq(&rq->scx.local_dsq, SCX_DSQ_LOCAL, پوچ));
#ifdef CONFIG_EXT_SUB_SCHED
		BUG_ON(scx_init_dsq(&rq->scx.reject_dsq, SCX_DSQ_REJECT, پوچ));
		scx_rescue_init(rq);
#endif

		INIT_LIST_HEAD(&rq->scx.runnable_list);
		INIT_LIST_HEAD(&rq->scx.ddsp_deferred_locals);

		BUG_ON(!zalloc_cpumask_var_گره(&rq->scx.cpus_to_sync, GFP_KERNEL, n));
		INIT_LIST_HEAD(&rq->scx.sched_pcpus_to_kick);
		raw_spin_lock_init(&rq->scx.deferred_reenq_lock);
		INIT_LIST_HEAD(&rq->scx.deferred_reenq_locals);
		INIT_LIST_HEAD(&rq->scx.deferred_reenq_users);
		rq->scx.deferred_irq_work = IRQ_WORK_INIT_HARD(deferred_irq_workfn);
		rq->scx.kick_cpus_irq_work = IRQ_WORK_INIT_HARD(kick_cpus_irq_workfn);

		اگر (cpu_online(cpu))
			cpu_rq(cpu)->scx.flags |= SCX_RQ_ONLINE;
	}

	register_sysrq_کلید('S', &sysrq_sched_ext_reset_op);
	register_sysrq_کلید('D', &sysrq_sched_ext_dump_op);
	INIT_DELAYED_WORK(&scx_watchdog_work, scx_watchdog_workfn);

#ifdef CONFIG_EXT_SUB_SCHED
	BUG_ON(rhashtable_init(&scx_sched_hash, &scx_sched_hash_params));
#endif	/* CONFIG_EXT_SUB_SCHED */
}


/********************************************************************************
 * Helpers that can be called from the BPF scheduler.
 */
ایستا bool scx_vet_enq_flags(ساختار scx_sched *sch, u64 dsq_id, u64 *enq_flags)
{
	bool is_local = dsq_id == SCX_DSQ_LOCAL ||
		(dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON;

	اگر (unlikely(*enq_flags & __SCX_ENQ_INTERNAL_MASK)) {
		scx_error(sch, "invalid enq_flags 0x%llx", *enq_flags);
		بازگشت false;
	}

	اگر (*enq_flags & SCX_ENQ_IMMED) {
		اگر (unlikely(!is_local)) {
			scx_error(sch, "SCX_ENQ_IMMED on a non-local DSQ 0x%llx", dsq_id);
			بازگشت false;
		}
	} وگرنه اگر ((sch->ops.flags & SCX_OPS_ALWAYS_ENQ_IMMED) && is_local) {
		*enq_flags |= SCX_ENQ_IMMED;
	}

	اگر (unlikely((*enq_flags & SCX_ENQ_RESCUE) && !is_local)) {
		scx_error(sch, "SCX_ENQ_RESCUE on a non-local DSQ 0x%llx", dsq_id);
		بازگشت false;
	}

	بازگشت true;
}

ایستا bool scx_dsq_درج_preamble(ساختار scx_sched *sch, ساختار task_struct *p,
				    u64 dsq_id, u64 *enq_flags)
{
	lockdep_assert_irqs_disabled();

	اگر (unlikely(!p)) {
		scx_error(sch, "called with NULL task");
		بازگشت false;
	}

	/* see SCX_EV_INSERT_NOT_OWNED definition */
	اگر (unlikely(!scx_task_on_sched(sch, p))) {
		__scx_add_event(sch, SCX_EV_درج_NOT_OWNED, 1);
		بازگشت false;
	}

	اگر (!scx_vet_enq_flags(sch, dsq_id, enq_flags))
		بازگشت false;

	بازگشت true;
}

ایستا تهی scx_dsq_درج_commit(ساختار scx_sched *sch, ساختار task_struct *p,
				  u64 dsq_id, u64 slice, u64 vtime, u64 enq_flags)
{
	ساختار scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	ساختار task_struct *ddsp_task;

	ddsp_task = __this_cpu_read(direct_dispatch_task);
	اگر (ddsp_task) {
		mark_direct_dispatch(sch, ddsp_task, p, dsq_id, slice, vtime, enq_flags);
		بازگشت;
	}

	اگر (unlikely(dspc->cursor >= sch->dsp_max_batch)) {
		scx_error(sch, "dispatch buffer overflow");
		بازگشت;
	}

	dspc->buf[dspc->cursor++] = (ساختار scx_dsp_buf_ent){
		.task = p,
		.qseq = atomic_long_read(&p->scx.ops_state) & SCX_OPSS_QSEQ_MASK,
		.dsq_id = dsq_id,
		.slice = slice,
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
__bpf_kfunc bool scx_bpf_dsq_درج_v2(ساختار task_struct *p, u64 dsq_id,
					 u64 slice, u64 enq_flags,
					 ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت false;

	اگر (!scx_dsq_درج_preamble(sch, p, dsq_id, &enq_flags))
		بازگشت false;

	scx_dsq_درج_commit(sch, p, dsq_id, slice, 0, enq_flags);

	بازگشت true;
}

/*
 * COMPAT: Will be removed in v6.23 along with the ___v2 suffix.
 */
__bpf_kfunc تهی scx_bpf_dsq_درج(ساختار task_struct *p, u64 dsq_id,
				    u64 slice, u64 enq_flags,
				    ثابت ساختار bpf_prog_aux *aux)
{
	scx_bpf_dsq_درج_v2(p, dsq_id, slice, enq_flags, aux);
}

ایستا bool scx_dsq_درج_vtime(ساختار scx_sched *sch, ساختار task_struct *p,
				 u64 dsq_id, u64 slice, u64 vtime, u64 enq_flags)
{
	اگر (!scx_dsq_درج_preamble(sch, p, dsq_id, &enq_flags))
		بازگشت false;

	scx_dsq_درج_commit(sch, p, dsq_id, slice, vtime, enq_flags | SCX_ENQ_DSQ_PRIQ);

	بازگشت true;
}

ساختار scx_bpf_dsq_درج_vtime_args {
	/* @p can't be packed together as KF_RCU is not transitive */
	u64			dsq_id;
	u64			slice;
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
__scx_bpf_dsq_insert_vtime(ساختار task_struct *p,
			   ساختار scx_bpf_dsq_درج_vtime_args *args,
			   ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت false;

	بازگشت scx_dsq_درج_vtime(sch, p, args->dsq_id, args->slice,
				    args->vtime, args->enq_flags);
}

/*
 * COMPAT: Will be removed in v6.23.
 */
__bpf_kfunc تهی scx_bpf_dsq_درج_vtime(ساختار task_struct *p, u64 dsq_id,
					  u64 slice, u64 vtime, u64 enq_flags)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ریشه);
	اگر (unlikely(!sch))
		بازگشت;

#ifdef CONFIG_EXT_SUB_SCHED
	/*
	 * Disallow if any sub-scheds are attached. There is no way to tell
	 * which scheduler called us, so error out @p's scheduler -- read it
	 * under RCU as @p's locks aren't necessarily held here. @p may be a
	 * task past sched_ext_dead() or an idle task, in which case its
	 * scheduler can't be determined and there is nothing obviously wrong
	 * to report; just refuse the call.
	 */
	اگر (unlikely(!list_empty(&sch->children))) {
		ساختار scx_sched *tsch = scx_task_sched_rcu(p);

		اگر (tsch)
			scx_error(tsch, "__scx_bpf_dsq_insert_vtime() must be used");
		بازگشت;
	}
#endif

	scx_dsq_درج_vtime(sch, p, dsq_id, slice, vtime, enq_flags);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_enqueue_dispatch)
BTF_ID_FLAGS(func, scx_bpf_dsq_درج, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_درج_v2, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, __scx_bpf_dsq_insert_vtime, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_درج_vtime, KF_RCU)
BTF_KFUNCS_END(scx_kfunc_ids_enqueue_dispatch)

ایستا ثابت ساختار btf_kfunc_id_set scx_kfunc_set_enqueue_dispatch = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_enqueue_dispatch,
	.filter			= scx_kfunc_context_filter,
};

ایستا bool scx_dsq_move(ساختار bpf_iter_scx_dsq_kern *kit,
			 ساختار task_struct *p, u64 dsq_id, u64 enq_flags,
			 bool priq)
{
	ساختار scx_dispatch_q *src_dsq = kit->dsq, *dst_dsq;
	ساختار scx_sched *sch;
	ساختار rq *p_rq, *src_rq, *locked_rq;
	bool dispatched = false;
	unsigned long flags;

	/*
	 * The verifier considers an iterator slot initialized on any
	 * KF_ITER_NEW return, so a BPF program may legally reach here after
	 * bpf_iter_scx_dsq_new() failed and left @kit->dsq NULL.
	 */
	اگر (unlikely(!src_dsq))
		بازگشت false;

	sch = src_dsq->sched;

	اگر (!scx_vet_enq_flags(sch, dsq_id, &enq_flags))
		بازگشت false;

	/* internal bit, can only go in after @enq_flags is vetted */
	اگر (priq)
		enq_flags |= SCX_ENQ_DSQ_PRIQ;

	/*
	 * If the BPF scheduler keeps calling this function repeatedly, it can
	 * cause similar live-lock conditions as scx_consume_dispatch_q().
	 */
	اگر (unlikely(READ_ONCE(sch->aborting)))
		بازگشت false;

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

	اگر (locked_rq) {
		اگر (locked_rq != src_rq)
			switch_rq_lock(locked_rq, src_rq);
	} وگرنه {
		raw_spin_rq_lock(src_rq);
	}

	p_rq = src_rq;
	raw_spin_lock(&src_dsq->lock);

	/* did someone else get to it while we dropped the locks? */
	اگر (nldsq_cursor_lost_task(&kit->cursor, src_rq, src_dsq, p)) {
		raw_spin_unlock(&src_dsq->lock);
		برو out;
	}

	/*
	 * @p has been on $src_dsq and can't move anymore. If @p is not on @sch,
	 * the caller didn't have authority over @p at the time of the call.
	 */
	اگر (unlikely(!scx_task_on_sched(sch, p))) {
		scx_error(sch, "scx_bpf_dsq_move[_vtime]() on %s[%d] but the task belongs to a different scheduler",
			  p->comm, p->pid);
		raw_spin_unlock(&src_dsq->lock);
		برو out;
	}

	/* @p is still on $src_dsq and stable, determine the destination */
	dst_dsq = find_dsq_for_dispatch(sch, locked_rq ?: this_rq(), dsq_id, task_cpu(p));

	/*
	 * Apply vtime and slice updates before moving. @p is still on $src_dsq
	 * with both $src_dsq and its task_rq locked, satisfying the write
	 * rules, and the PRIQ insertion into $dst_dsq reads the new vtime.
	 */
	اگر (kit->cursor.flags & __SCX_DSQ_ITER_HAS_VTIME)
		p->scx.dsq_vtime = kit->vtime;
	اگر (kit->cursor.flags & __SCX_DSQ_ITER_HAS_SLICE)
		scx_set_task_slice(p, kit->slice);

	/* execute move */
	p_rq = move_task_between_dsqs(sch, p, enq_flags, src_dsq, dst_dsq);
	dispatched = true;
out:
	اگر (locked_rq) {
		اگر (locked_rq != p_rq)
			switch_rq_lock(p_rq, locked_rq);
	} وگرنه {
		scx_rq_lock_drop(p_rq);
		raw_spin_rq_unlock_irqrestore(p_rq, flags);
	}

	kit->cursor.flags &= ~(__SCX_DSQ_ITER_HAS_SLICE |
			       __SCX_DSQ_ITER_HAS_VTIME);
	بازگشت dispatched;
}

__bpf_kfunc_start_defs();

/**
 * scx_bpf_dispatch_nr_slots - Return the number of remaining dispatch slots
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Can only be called from ops.dispatch().
 */
__bpf_kfunc u32 scx_bpf_dispatch_nr_slots(ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت 0;

	بازگشت sch->dsp_max_batch - __this_cpu_read(sch->pcpu->dsp_ctx.cursor);
}

/**
 * scx_bpf_dispatch_cancel - Cancel the latest dispatch
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Cancel the latest dispatch. Can be called multiple times to cancel further
 * dispatches. Can only be called from ops.dispatch().
 */
__bpf_kfunc تهی scx_bpf_dispatch_cancel(ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار scx_dsp_ctx *dspc;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت;

	dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;

	اگر (dspc->cursor > 0)
		dspc->cursor--;
	وگرنه
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
__bpf_kfunc bool scx_bpf_dsq_move_to_local___v2(u64 dsq_id, u64 enq_flags,
						ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_dispatch_q *dsq;
	ساختار scx_sched *sch;
	ساختار scx_dsp_ctx *dspc;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت false;

	اگر (!scx_vet_enq_flags(sch, SCX_DSQ_LOCAL, &enq_flags))
		بازگشت false;

	dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;

	scx_flush_dispatch_buf(sch, dspc->rq);

	dsq = find_user_dsq(sch, dsq_id);
	اگر (unlikely(!dsq)) {
		scx_error(sch, "invalid DSQ ID 0x%016llx", dsq_id);
		بازگشت false;
	}

	اگر (scx_consume_dispatch_q(sch, dspc->rq, dsq, enq_flags)) {
		/*
		 * A successfully consumed task can be dequeued before it starts
		 * running while the CPU is trying to migrate other dispatched
		 * tasks. Bump nr_tasks to tell dispatch_one() to retry on empty
		 * local DSQ.
		 */
		dspc->nr_tasks++;
		بازگشت true;
	} وگرنه {
		بازگشت false;
	}
}

/*
 * COMPAT: ___v2 was introduced in v7.1. Remove this and ___v2 tag in the future.
 */
__bpf_kfunc bool scx_bpf_dsq_move_to_local(u64 dsq_id, ثابت ساختار bpf_prog_aux *aux)
{
	بازگشت scx_bpf_dsq_move_to_local___v2(dsq_id, 0, aux);
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
__bpf_kfunc تهی scx_bpf_dsq_move_set_slice(ساختار bpf_iter_scx_dsq *it__iter,
					    u64 slice)
{
	ساختار bpf_iter_scx_dsq_kern *kit = (تهی *)it__iter;

	kit->slice = slice;
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
__bpf_kfunc تهی scx_bpf_dsq_move_set_vtime(ساختار bpf_iter_scx_dsq *it__iter,
					    u64 vtime)
{
	ساختار bpf_iter_scx_dsq_kern *kit = (تهی *)it__iter;

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
__bpf_kfunc bool scx_bpf_dsq_move(ساختار bpf_iter_scx_dsq *it__iter,
				  ساختار task_struct *p, u64 dsq_id,
				  u64 enq_flags)
{
	بازگشت scx_dsq_move((ساختار bpf_iter_scx_dsq_kern *)it__iter,
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
__bpf_kfunc bool scx_bpf_dsq_move_vtime(ساختار bpf_iter_scx_dsq *it__iter,
					ساختار task_struct *p, u64 dsq_id,
					u64 enq_flags)
{
	بازگشت scx_dsq_move((ساختار bpf_iter_scx_dsq_kern *)it__iter,
			    p, dsq_id, enq_flags, true);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_dispatch)
BTF_ID_FLAGS(func, scx_bpf_dispatch_nr_slots, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_dispatch_cancel, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_to_local, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_to_local___v2, KF_IMPLICIT_ARGS)
/* scx_bpf_dsq_move*() also in scx_kfunc_ids_unlocked: callable from unlocked contexts */
BTF_ID_FLAGS(func, scx_bpf_dsq_move_set_slice, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_set_vtime, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_vtime, KF_RCU)
#ifdef CONFIG_EXT_SUB_SCHED
BTF_ID_FLAGS(func, scx_bpf_sub_dispatch, KF_IMPLICIT_ARGS)
#endif
BTF_KFUNCS_END(scx_kfunc_ids_dispatch)

ایستا ثابت ساختار btf_kfunc_id_set scx_kfunc_set_dispatch = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_dispatch,
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
__bpf_kfunc u32 scx_bpf_reenqueue_local(ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار rq *rq;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت 0;

	rq = cpu_rq(smp_processor_id());
	lockdep_assert_rq_held(rq);

	بازگشت reenq_local(sch, rq, SCX_REENQ_ANY);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_cpu_release)
BTF_ID_FLAGS(func, scx_bpf_reenqueue_local, KF_IMPLICIT_ARGS)
BTF_KFUNCS_END(scx_kfunc_ids_cpu_release)

ایستا ثابت ساختار btf_kfunc_id_set scx_kfunc_set_cpu_release = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_cpu_release,
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
__bpf_kfunc s32 scx_bpf_create_dsq(u64 dsq_id, s32 گره, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_dispatch_q *dsq;
	ساختار scx_sched *sch;
	s32 ret;

	اگر (unlikely(گره >= (صحیح)nr_گره_ids ||
		     (گره < 0 && گره != NUMA_NO_گره)))
		بازگشت -EINVAL;

	اگر (unlikely(dsq_id & SCX_DSQ_FLAG_BUILTIN))
		بازگشت -EINVAL;

	dsq = kmalloc_گره(اندازه(*dsq), GFP_KERNEL, گره);
	اگر (!dsq)
		بازگشت -ENOMEM;

	/*
	 * scx_init_dsq() must be called in GFP_KERNEL context. Init it with
	 * NULL @sch and update afterwards.
	 */
	ret = scx_init_dsq(dsq, dsq_id, پوچ);
	اگر (ret) {
		kfree(dsq);
		بازگشت ret;
	}

	rcu_read_lock();

	sch = scx_prog_sched(aux);
	اگر (sch) {
		dsq->sched = sch;
		ret = rhashtable_lookup_درج_fast(&sch->dsq_hash, &dsq->hash_گره,
						    dsq_hash_params);
	} وگرنه {
		ret = -ENODEV;
	}

	rcu_read_unlock();
	اگر (ret) {
		exit_dsq(dsq);
		kfree(dsq);
	}
	بازگشت ret;
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_unlocked)
BTF_ID_FLAGS(func, scx_bpf_create_dsq, KF_IMPLICIT_ARGS | KF_SLEEPABLE)
/* also in scx_kfunc_ids_dispatch: also callable from ops.dispatch() */
BTF_ID_FLAGS(func, scx_bpf_dsq_move_set_slice, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_set_vtime, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_dsq_move_vtime, KF_RCU)
/* also in scx_kfunc_ids_select_cpu: also callable from ops.select_cpu()/ops.enqueue() */
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ARGS | KF_RCU)
BTF_KFUNCS_END(scx_kfunc_ids_unlocked)

ایستا ثابت ساختار btf_kfunc_id_set scx_kfunc_set_unlocked = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_unlocked,
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
__bpf_kfunc bool scx_bpf_task_set_slice(ساختار task_struct *p, u64 slice,
					ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار rq *locked_rq;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch || !scx_task_on_sched(sch, p)))
		بازگشت false;

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
	اگر (!locked_rq ||
	    (READ_ONCE(p->scx.runnable_cpu) != cpu_of(locked_rq) &&
	     !task_current(locked_rq, p))) {
		set_task_slice_oob(sch, p, slice);
		بازگشت true;
	}

	/* under the rq lock: apply now, extensions gated on baseline access */
	اگر (slice > p->scx.slice &&
	    unlikely(scx_missing_caps(sch, cpu_of(locked_rq), SCX_CAP_BASE))) {
		__scx_add_event(sch, SCX_EV_SLICE_DENIED, 1);
		بازگشت true;
	}

	اگر (unlikely(!scx_set_task_slice(p, slice)))
		__scx_add_event(sch, SCX_EV_SLICE_DENIED, 1);

	بازگشت true;
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
__bpf_kfunc bool scx_bpf_task_set_dsq_vtime(ساختار task_struct *p, u64 vtime,
					    ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch || !scx_task_on_sched(sch, p)))
		بازگشت false;

	p->scx.dsq_vtime = vtime;
	بازگشت true;
}

تهی scx_kick_cpu(ساختار scx_sched *sch, s32 cpu, u64 flags)
{
	ساختار scx_sched_pcpu *pcpu;
	ساختار rq *this_rq;
	unsigned long irq_flags;

	/*
	 * The per-cpu kick list is guarded only by local_irq_save(), which does
	 * not mask NMIs, so kicking from NMI could corrupt it and is unsupported.
	 */
	اگر (unlikely(in_nmi())) {
		scx_error(sch, "scx_bpf_kick_cpu() called from NMI");
		بازگشت;
	}

	local_irq_save(irq_flags);

	this_rq = this_rq();
	pcpu = this_cpu_ptr(sch->pcpu);

	/*
	 * While bypassing for PM ops, IRQ handling may not be online which can
	 * lead to irq_work_queue() malfunction such as infinite busy wait for
	 * IRQ status update. Suppress kicking.
	 */
	اگر (scx_bypassing(sch, cpu_of(this_rq)))
		برو out;

	/*
	 * Actual kicking is bounced to kick_cpus_irq_workfn() to avoid nesting
	 * rq locks. We can probably be smarter and avoid bouncing if called
	 * from ops which don't hold a rq lock.
	 *
	 * The kick masks are owned by @sch->pcpu, so that a preempt kick can be
	 * attributed to @sch.
	 */
	اگر (flags & SCX_KICK_IDLE) {
		ساختار rq *target_rq = cpu_rq(cpu);

		اگر (unlikely(flags & (SCX_KICK_PREEMPT | SCX_KICK_WAIT)))
			scx_error(sch, "PREEMPT/WAIT cannot be used with SCX_KICK_IDLE");

		اگر (raw_spin_rq_trylock(target_rq)) {
			اگر (can_skip_idle_kick(target_rq)) {
				scx_rq_lock_drop(target_rq);
				raw_spin_rq_unlock(target_rq);
				برو out;
			}
			scx_rq_lock_drop(target_rq);
			raw_spin_rq_unlock(target_rq);
		}
		cpumask_set_cpu(cpu, pcpu->cpus_to_kick_if_idle);
	} وگرنه {
		cpumask_set_cpu(cpu, pcpu->cpus_to_kick);

		اگر (flags & SCX_KICK_PREEMPT)
			cpumask_set_cpu(cpu, pcpu->cpus_to_preempt);
		اگر (flags & SCX_KICK_WAIT)
			cpumask_set_cpu(cpu, pcpu->cpus_to_wait);
	}

	اگر (list_empty(&pcpu->to_kick_گره))
		list_add_tail(&pcpu->to_kick_گره, &this_rq->scx.sched_pcpus_to_kick);
	irq_work_queue(&this_rq->scx.kick_cpus_irq_work);
out:
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
__bpf_kfunc تهی scx_bpf_kick_cpu(s32 cpu, u64 flags, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	اگر (likely(sch) && scx_cpu_valid(sch, cpu, پوچ))
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
__bpf_kfunc تهی scx_bpf_kick_cid(s32 cid, u64 flags, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	s32 cpu;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت;
	cpu = scx_cid_to_cpu(sch, cid);
	اگر (cpu < 0)
		بازگشت;
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
__bpf_kfunc s32 scx_bpf_dsq_nr_queued(u64 dsq_id, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار scx_dispatch_q *dsq;
	s32 ret;

	preempt_disable();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch)) {
		ret = -ENODEV;
		برو out;
	}

	اگر (dsq_id == SCX_DSQ_LOCAL) {
		ret = READ_ONCE((scx_locked_rq() ?: this_rq())->scx.local_dsq.nr);
		برو out;
	} وگرنه اگر ((dsq_id & SCX_DSQ_LOCAL_ON) == SCX_DSQ_LOCAL_ON) {
		s32 cpu = scx_cpu_ret(sch, dsq_id & SCX_DSQ_LOCAL_CPU_MASK);

		اگر (scx_cpu_valid(sch, cpu, پوچ)) {
			ret = READ_ONCE(cpu_rq(cpu)->scx.local_dsq.nr);
			برو out;
		}
	} وگرنه {
		dsq = find_user_dsq(sch, dsq_id);
		اگر (dsq) {
			ret = READ_ONCE(dsq->nr);
			برو out;
		}
	}
	ret = -ENOENT;
out:
	preempt_enable();
	بازگشت ret;
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
__bpf_kfunc تهی scx_bpf_destroy_dsq(u64 dsq_id, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();
	sch = scx_prog_sched(aux);
	اگر (sch)
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
__bpf_kfunc صحیح bpf_iter_scx_dsq_new(ساختار bpf_iter_scx_dsq *it, u64 dsq_id,
				     u64 flags, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار bpf_iter_scx_dsq_kern *kit = (تهی *)it;
	ساختار scx_sched *sch;

	BUILD_BUG_ON(اندازه(ساختار bpf_iter_scx_dsq_kern) >
		     اندازه(ساختار bpf_iter_scx_dsq));
	BUILD_BUG_ON(__alignof__(ساختار bpf_iter_scx_dsq_kern) !=
		     __alignof__(ساختار bpf_iter_scx_dsq));
	BUILD_BUG_ON(__SCX_DSQ_ITER_ALL_FLAGS &
		     ((1U << __SCX_DSQ_LNODE_PRIV_SHIFT) - 1));

	/*
	 * next() and destroy() will be called regardless of the return value.
	 * Always clear $kit->dsq.
	 */
	kit->dsq = پوچ;

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت -ENODEV;

	اگر (flags & ~__SCX_DSQ_ITER_USER_FLAGS)
		بازگشت -EINVAL;

	kit->dsq = find_user_dsq(sch, dsq_id);
	اگر (!kit->dsq)
		بازگشت -ENOENT;

	kit->cursor = INIT_DSQ_LIST_CURSOR(kit->cursor, kit->dsq, flags);

	بازگشت 0;
}

/**
 * bpf_iter_scx_dsq_next - Progress a DSQ iterator
 * @it: iterator to progress
 *
 * Return the next task. See bpf_iter_scx_dsq_new().
 */
__bpf_kfunc ساختار task_struct *bpf_iter_scx_dsq_next(ساختار bpf_iter_scx_dsq *it)
{
	ساختار bpf_iter_scx_dsq_kern *kit = (تهی *)it;

	اگر (!kit->dsq)
		بازگشت پوچ;

	guard(raw_spinlock_irqsave)(&kit->dsq->lock);

	بازگشت nldsq_cursor_next_task(&kit->cursor, kit->dsq);
}

/**
 * bpf_iter_scx_dsq_destroy - Destroy a DSQ iterator
 * @it: iterator to destroy
 *
 * Undo bpf_iter_scx_dsq_new().
 */
__bpf_kfunc تهی bpf_iter_scx_dsq_destroy(ساختار bpf_iter_scx_dsq *it)
{
	ساختار bpf_iter_scx_dsq_kern *kit = (تهی *)it;

	اگر (!kit->dsq)
		بازگشت;

	اگر (!list_empty(&kit->cursor.گره)) {
		unsigned long flags;

		raw_spin_lock_irqsave(&kit->dsq->lock, flags);
		list_del_init(&kit->cursor.گره);
		raw_spin_unlock_irqrestore(&kit->dsq->lock, flags);
	}
	kit->dsq = پوچ;
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
__bpf_kfunc ساختار task_struct *scx_bpf_dsq_peek(u64 dsq_id,
						 ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار scx_dispatch_q *dsq;

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت پوچ;

	اگر (unlikely(dsq_id & SCX_DSQ_FLAG_BUILTIN)) {
		scx_error(sch, "peek disallowed on builtin DSQ 0x%llx", dsq_id);
		بازگشت پوچ;
	}

	dsq = find_user_dsq(sch, dsq_id);
	اگر (unlikely(!dsq)) {
		scx_error(sch, "peek on non-existent DSQ 0x%llx", dsq_id);
		بازگشت پوچ;
	}

	بازگشت rcu_dereference(dsq->first_task);
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
__bpf_kfunc تهی scx_bpf_dsq_reenq(u64 dsq_id, u64 reenq_flags,
				   ثابت ساختار bpf_prog_aux *aux)
{
	ساختار rq *locked_rq = scx_locked_rq();
	ساختار scx_sched *sch;
	ساختار scx_dispatch_q *dsq;

	guard(preempt)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت;

	اگر (unlikely(reenq_flags & ~__SCX_REENQ_USER_MASK)) {
		scx_error(sch, "invalid SCX_REENQ flags 0x%llx", reenq_flags);
		بازگشت;
	}

	/* not specifying any filter bits is the same as %SCX_REENQ_ANY */
	اگر (!(reenq_flags & __SCX_REENQ_FILTER_MASK))
		reenq_flags |= SCX_REENQ_ANY;

	dsq = find_dsq_for_dispatch(sch, locked_rq ?: this_rq(), dsq_id, smp_processor_id());
	schedule_dsq_reenq(sch, dsq, reenq_flags, locked_rq);
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
__bpf_kfunc تهی scx_bpf_reenqueue_local___v2(ثابت ساختار bpf_prog_aux *aux)
{
	scx_bpf_dsq_reenq(SCX_DSQ_LOCAL, 0, aux);
}

__bpf_kfunc_end_defs();

__printf(5, 0)
ایستا s32 __bstr_format(ساختار scx_sched *sch, u64 *data_buf, نویسه *سطر_buf,
			 size_t سطر_size, نویسه *fmt, unsigned long long *data,
			 u32 data__sz)
{
	ساختار bpf_bprintf_data bprintf_data = { .get_bin_args = true };
	s32 ret;

	اگر (data__sz % 8 || data__sz > MAX_BPRINTF_VARARGS * 8 ||
	    (data__sz && !data)) {
		scx_error(sch, "invalid data=%p and data__sz=%u", (تهی *)data, data__sz);
		بازگشت -EINVAL;
	}

	ret = copy_from_kernel_nofault(data_buf, data, data__sz);
	اگر (ret < 0) {
		scx_error(sch, "failed to read data fields (%d)", ret);
		بازگشت ret;
	}

	ret = bpf_bprintf_prepare(fmt, UINT_MAX, data_buf, data__sz / 8,
				  &bprintf_data);
	اگر (ret < 0) {
		scx_error(sch, "format preparation failed (%d)", ret);
		بازگشت ret;
	}

	ret = bstr_printf(سطر_buf, سطر_size, fmt,
			  bprintf_data.bin_args);
	bpf_bprintf_cleanup(&bprintf_data);
	اگر (ret < 0) {
		scx_error(sch, "(\"%s\", %p, %u) failed to format", fmt, data, data__sz);
		بازگشت ret;
	}

	بازگشت ret;
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
bool scx_exit_bstr(ساختار scx_sched *sch, شمارش scx_exit_kind kind,
		   s64 exit_code, ساختار scx_sched *fmt_blame, نویسه *fmt,
		   unsigned long long *data, u32 data__sz)
{
	ساختار scx_exit_info *ei = sch->exit_info;
	u64 data_buf[MAX_BPRINTF_VARARGS];
	s32 ret;

	guard(preempt)();

	اگر (!scx_claim_exit(sch, kind))
		بازگشت false;

	ret = __bstr_format(fmt_blame, data_buf, ei->پیام, SCX_EXIT_پیام_LEN,
			    fmt, data, data__sz);
	اگر (ret < 0)
		scnprintf(ei->پیام, SCX_EXIT_پیام_LEN,
			  "exit message formatting failed (%d)", ret);

	scx_finish_exit(sch, kind, exit_code, raw_smp_processor_id());
	بازگشت true;
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
__bpf_kfunc تهی scx_bpf_exit_bstr(s64 exit_code, نویسه *fmt,
				   unsigned long long *data, u32 data__sz,
				   ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (likely(sch))
		scx_exit_bstr(sch, SCX_EXIT_UNREG_BPF, exit_code, sch, fmt,
			      data, data__sz);
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
__bpf_kfunc تهی scx_bpf_error_bstr(نویسه *fmt, unsigned long long *data,
				    u32 data__sz, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (likely(sch))
		scx_exit_bstr(sch, SCX_EXIT_ERROR_BPF, 0, sch, fmt, data,
			      data__sz);
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
__bpf_kfunc تهی scx_bpf_dump_bstr(نویسه *fmt, unsigned long long *data,
				   u32 data__sz, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار scx_dump_data *dd = &scx_dump_data;
	ساختار scx_bstr_buf *buf = &dd->buf;
	s32 ret;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت;

	اگر (raw_smp_processor_id() != dd->cpu) {
		scx_error(sch, "scx_bpf_dump() must only be called from ops.dump() and friends");
		بازگشت;
	}

	/* append the formatted string to the line buf */
	ret = __bstr_format(sch, buf->data, buf->سطر + dd->cursor,
			    اندازه(buf->سطر) - dd->cursor, fmt, data, data__sz);
	اگر (ret < 0) {
		scx_dump_سطر(dd->s, "%s[!] (\"%s\", %p, %u) failed to format (%d)",
			      dd->prefix, fmt, data, data__sz, ret);
		بازگشت;
	}

	dd->cursor += ret;
	dd->cursor = min_t(s32, dd->cursor, اندازه(buf->سطر));

	اگر (!dd->cursor)
		بازگشت;

	/*
	 * If the line buf overflowed or ends in a newline, flush it into the
	 * dump. This is to allow the caller to generate a single line over
	 * multiple calls. As ops_dump_flush() can also handle multiple lines in
	 * the line buf, the only case which can lead to an unexpected
	 * truncation is when the caller keeps generating newlines in the middle
	 * instead of the end consecutively. Don't do that.
	 */
	اگر (dd->cursor >= اندازه(buf->سطر) || buf->سطر[dd->cursor - 1] == '\n')
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
__bpf_kfunc u32 scx_bpf_cpuperf_cap(s32 cpu, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (likely(sch) && scx_cpu_valid(sch, cpu, پوچ))
		بازگشت arch_scale_cpu_capacity(cpu);
	وگرنه
		بازگشت SCX_CPUPERF_ONE;
}

/**
 * scx_bpf_cidperf_cap - Query the maximum relative capacity of the CPU at @cid
 * @cid: cid of the CPU to query
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpuperf_cap().
 */
__bpf_kfunc u32 scx_bpf_cidperf_cap(s32 cid, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت SCX_CPUPERF_ONE;
	cpu = scx_cid_to_cpu(sch, cid);
	اگر (cpu < 0)
		بازگشت SCX_CPUPERF_ONE;
	بازگشت arch_scale_cpu_capacity(cpu);
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
__bpf_kfunc u32 scx_bpf_cpuperf_cur(s32 cpu, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (likely(sch) && scx_cpu_valid(sch, cpu, پوچ))
		بازگشت arch_scale_freq_capacity(cpu);
	وگرنه
		بازگشت SCX_CPUPERF_ONE;
}

/**
 * scx_bpf_cidperf_cur - Query the current performance of the CPU at @cid
 * @cid: cid of the CPU to query
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpuperf_cur().
 */
__bpf_kfunc u32 scx_bpf_cidperf_cur(s32 cid, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت SCX_CPUPERF_ONE;
	cpu = scx_cid_to_cpu(sch, cid);
	اگر (cpu < 0)
		بازگشت SCX_CPUPERF_ONE;
	بازگشت arch_scale_freq_capacity(cpu);
}

/* validate and apply a cpuperf target, see scx_bpf_cpuperf_set() */
ایستا s32 scx_cpuperf_set(ساختار scx_sched *sch, s32 cpu, u32 perf)
{
	ساختار rq *rq, *locked_rq;
	ساختار rq_flags rf;
	s32 ret;

	اگر (unlikely(perf > SCX_CPUPERF_ONE)) {
		scx_error(sch, "Invalid cpuperf target %u for CPU %d", perf, cpu);
		بازگشت -EINVAL;
	}

	اگر (!scx_cpu_valid(sch, cpu, پوچ))
		بازگشت -EINVAL;

	rq = cpu_rq(cpu);
	locked_rq = scx_locked_rq();

	/*
	 * When called with an rq lock held, restrict the operation to the
	 * corresponding CPU to prevent ABBA deadlocks.
	 */
	اگر (locked_rq && rq != locked_rq) {
		scx_error(sch, "Invalid target CPU %d", cpu);
		بازگشت -EINVAL;
	}

	/*
	 * If no rq lock is held, allow to operate on any CPU by acquiring
	 * the corresponding rq lock.
	 */
	اگر (!locked_rq) {
		rq_lock_irqsave(rq, &rf);
		update_rq_clock(rq);
	}

	/*
	 * ecaps updates are folded under the rq lock, making this test
	 * authoritative: a write can never land after a revoke has taken
	 * effect on @cpu.
	 */
	اگر (likely(!scx_missing_caps(sch, cpu, SCX_CAP_PERF))) {
		rq->scx.cpuperf_target = perf;
		cpufreq_update_util(rq, 0);
		ret = 0;
	} وگرنه {
		__scx_add_event(sch, SCX_EV_SUB_CIDPERF_DENIED, 1);
		ret = -EACCES;
	}

	اگر (!locked_rq)
		rq_unlock_irqrestore(rq, &rf);

	بازگشت ret;
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
__bpf_kfunc تهی scx_bpf_cpuperf_set(s32 cpu, u32 perf, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت;

	scx_cpuperf_set(sch, cpu, perf);
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
__bpf_kfunc s32 scx_bpf_cidperf_set(s32 cid, u32 perf,
				    ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت -ENODEV;
	cpu = scx_cid_to_cpu(sch, cid);
	اگر (cpu < 0)
		بازگشت cpu;

	بازگشت scx_cpuperf_set(sch, cpu, perf);
}

/**
 * scx_bpf_nr_node_ids - Return the number of possible node IDs
 *
 * All valid node IDs in the system are smaller than the returned value.
 */
__bpf_kfunc u32 scx_bpf_nr_گره_ids(تهی)
{
	بازگشت nr_گره_ids;
}

/**
 * scx_bpf_nr_cpu_ids - Return the number of possible CPU IDs
 *
 * All valid CPU IDs in the system are smaller than the returned value.
 */
__bpf_kfunc u32 scx_bpf_nr_cpu_ids(تهی)
{
	بازگشت nr_cpu_ids;
}

/**
 * scx_bpf_nr_cids - Return the size of the cid space
 *
 * Equals num_possible_cpus(). All valid cids are in [0, return value).
 */
__bpf_kfunc u32 scx_bpf_nr_cids(تهی)
{
	بازگشت رقم_possible_cpus();
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
__bpf_kfunc u32 scx_bpf_nr_online_cids(تهی)
{
	بازگشت رقم_online_cpus();
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
__bpf_kfunc ثابت تهی *scx_bpf_online_cmask(ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار scx_cmask *online;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت پوچ;
	online = sch->online_cmask;
	اگر (unlikely(!online))
		بازگشت پوچ;

	بازگشت (تهی *)scx_kaddr_to_arena(sch, online);
}

/**
 * scx_bpf_this_cid - Return the cid of the CPU this program is running on
 *
 * cid-addressed equivalent of bpf_get_smp_processor_id() for scx programs.
 * The current cpu is trivially valid, so this is just a table lookup. Return
 * -EINVAL if called before any scheduler has ever published its cid tables.
 */
__bpf_kfunc s32 scx_bpf_this_cid(تهی)
{
	s16 *tbl;

	guard(rcu)();

	tbl = rcu_dereference(scx_cpu_to_cid_tbl);
	اگر (!tbl)
		بازگشت -EINVAL;
	بازگشت tbl[raw_smp_processor_id()];
}

/**
 * scx_bpf_get_possible_cpumask - Get a referenced kptr to cpu_possible_mask
 */
__bpf_kfunc ثابت ساختار cpumask *scx_bpf_get_possible_cpumask(تهی)
{
	بازگشت cpu_possible_mask;
}

/**
 * scx_bpf_get_online_cpumask - Get a referenced kptr to cpu_online_mask
 */
__bpf_kfunc ثابت ساختار cpumask *scx_bpf_get_online_cpumask(تهی)
{
	بازگشت cpu_online_mask;
}

/**
 * scx_bpf_put_cpumask - Release a possible/online cpumask
 * @cpumask: cpumask to release
 */
__bpf_kfunc تهی scx_bpf_put_cpumask(ثابت ساختار cpumask *cpumask)
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
__bpf_kfunc bool scx_bpf_task_running(ثابت ساختار task_struct *p)
{
	بازگشت task_rq(p)->curr == p;
}

/**
 * scx_bpf_task_cpu - CPU a task is currently associated with
 * @p: task of interest
 */
__bpf_kfunc s32 scx_bpf_task_cpu(ثابت ساختار task_struct *p)
{
	بازگشت task_cpu(p);
}

/**
 * scx_bpf_task_cid - cid a task is currently associated with
 * @p: task of interest
 *
 * cid-addressed equivalent of scx_bpf_task_cpu(). task_cpu(p) is always a
 * valid cpu, so this is just a table lookup. Return -EINVAL if called before
 * any scheduler has ever published its cid tables.
 */
__bpf_kfunc s32 scx_bpf_task_cid(ثابت ساختار task_struct *p)
{
	s16 *tbl;

	/* KF_RCU covers only @p - a sleepable program holds no RCU lock */
	guard(rcu)();

	tbl = rcu_dereference(scx_cpu_to_cid_tbl);
	اگر (!tbl)
		بازگشت -EINVAL;
	بازگشت tbl[task_cpu(p)];
}

/**
 * scx_bpf_locked_rq - Return the rq currently locked by SCX
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns the rq if a rq lock is currently held by SCX.
 * Otherwise emits an error and returns NULL.
 */
__bpf_kfunc ساختار rq *scx_bpf_locked_rq(ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار rq *rq;

	guard(preempt)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت پوچ;

	rq = scx_locked_rq();
	اگر (!rq) {
		scx_error(sch, "accessing rq without holding rq lock");
		بازگشت پوچ;
	}

	بازگشت rq;
}

/**
 * scx_bpf_cpu_curr - Return remote CPU's curr task
 * @cpu: CPU of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Callers must hold RCU read lock (KF_RCU).
 */
__bpf_kfunc ساختار task_struct *scx_bpf_cpu_curr(s32 cpu, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت پوچ;

	اگر (!scx_cpu_valid(sch, cpu, پوچ))
		بازگشت پوچ;

	بازگشت rcu_dereference(cpu_rq(cpu)->curr);
}

/**
 * scx_bpf_cid_curr - Return the curr task on the CPU at @cid
 * @cid: cid of interest
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * cid-addressed equivalent of scx_bpf_cpu_curr(). Callers must hold RCU
 * read lock (KF_RCU).
 */
__bpf_kfunc ساختار task_struct *scx_bpf_cid_curr(s32 cid, ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		بازگشت پوچ;
	cpu = scx_cid_to_cpu(sch, cid);
	اگر (cpu < 0)
		بازگشت پوچ;
	بازگشت rcu_dereference(cpu_rq(cpu)->curr);
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
__bpf_kfunc ساختار task_struct *scx_bpf_tid_to_task(u64 tid)
{
	ساختار sched_ext_entity *scx;

	اگر (!scx_tid_to_task_enabled()) {
		ساختار scx_sched *sch = rcu_dereference(scx_ریشه);

		اگر (sch)
			scx_error(sch, "scx_bpf_tid_to_task() called without SCX_OPS_TID_TO_TASK");
		بازگشت پوچ;
	}

	scx = rhashtable_lookup(&scx_tid_hash, &tid, scx_tid_hash_params);
	اگر (!scx)
		بازگشت پوچ;

	بازگشت container_of(scx, ساختار task_struct, scx);
}

u64 __scx_bpf_now(ساختار rq *rq)
{
	/* the caller must be on @rq's cpu or hold its lock */
	lockdep_assert((rq == this_rq() && !preemptible()) ||
		       lockdep_is_held(__rq_lockp(rq)));

	اگر (smp_load_acquire(&rq->scx.flags) & SCX_RQ_CLK_VALID) {
		/* if the rq clock is valid, use the cached rq clock */
		بازگشت READ_ONCE(rq->scx.clock);
	} وگرنه {
		/*
		 * Otherwise, return a fresh rq clock.
		 *
		 * The rq clock is updated outside of the rq lock.
		 * In this case, keep the updated rq clock invalid so the next
		 * read outside the rq lock gets a fresh rq clock.
		 */
		بازگشت sched_clock_cpu(cpu_of(rq));
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
__bpf_kfunc u64 scx_bpf_now(تهی)
{
	/*
	 * Note that scx_bpf_now() is re-entrant between a process context and
	 * an interrupt context (e.g., timer interrupt). However, we don't need
	 * to consider the race between them because such race is not observable
	 * from a caller.
	 */
	guard(preempt)();
	بازگشت __scx_bpf_now(this_rq());
}

ایستا تهی scx_read_events(ساختار scx_sched *sch, ساختار scx_event_stats *events)
{
	صحیح cpu;

	/* Aggregate per-CPU event counters into @events. */
	memset(events, 0, اندازه(*events));
	for_each_possible_cpu(cpu) {
		ساختار scx_event_stats *e_cpu = &per_cpu_ptr(sch->pcpu, cpu)->event_stats;
#define SCX_EVENT(name)	(events->name += READ_ONCE(e_cpu->name))
		SCX_EVENTS_LIST(SCX_EVENT);
#undef SCX_EVENT
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
__bpf_kfunc تهی scx_bpf_events(ساختار scx_event_stats *events, size_t events__sz,
				ثابت ساختار bpf_prog_aux *aux)
{
	ساختار scx_sched *sch;
	ساختار scx_event_stats e_sys;

	rcu_read_lock();
	sch = scx_prog_sched(aux);
	اگر (sch)
		scx_read_events(sch, &e_sys);
	وگرنه
		memset(&e_sys, 0, اندازه(e_sys));
	rcu_read_unlock();

	/*
	 * We cannot entirely trust a BPF-provided size since a BPF program
	 * might be compiled against a different vmlinux.h, of which
	 * scx_event_stats would be larger (a newer vmlinux.h) or smaller
	 * (an older vmlinux.h). Hence, we use the smaller size to avoid
	 * memory corruption.
	 */
	events__sz = min(events__sz, اندازه(*events));
	memcpy(events, &e_sys, events__sz);
}

#ifdef CONFIG_CGROUP_SCHED
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
__bpf_kfunc ساختار cgroup *scx_bpf_task_cgroup(ساختار task_struct *p,
					       ثابت ساختار bpf_prog_aux *aux)
{
	ساختار task_group *tg = p->sched_task_group;
	ساختار cgroup *cgrp = &cgrp_dfl_ریشه.cgrp;
	ساختار scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		برو out;

	اگر (!scx_kf_arg_task_ok(sch, p))
		برو out;

	cgrp = tg_cgrp(tg);

out:
	cgroup_get(cgrp);
	بازگشت cgrp;
}
#endif	/* CONFIG_CGROUP_SCHED */

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_any)
BTF_ID_FLAGS(func, scx_bpf_task_set_slice, KF_IMPLICIT_ARGS | KF_RCU);
BTF_ID_FLAGS(func, scx_bpf_task_set_dsq_vtime, KF_IMPLICIT_ARGS | KF_RCU);
BTF_ID_FLAGS(func, scx_bpf_kick_cpu, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_kick_cid, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_dsq_nr_queued, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_destroy_dsq, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_dsq_peek, KF_IMPLICIT_ARGS | KF_RCU_PROTECTED | KF_RET_NULL)
BTF_ID_FLAGS(func, scx_bpf_dsq_reenq, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_reenqueue_local___v2, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_new, KF_IMPLICIT_ARGS | KF_ITER_NEW | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_next, KF_ITER_NEXT | KF_RET_NULL)
BTF_ID_FLAGS(func, bpf_iter_scx_dsq_destroy, KF_ITER_DESTROY)
BTF_ID_FLAGS(func, scx_bpf_exit_bstr, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_error_bstr, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_dump_bstr, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cap, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cur, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_set, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cidperf_cap, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cidperf_cur, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cidperf_set, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_nr_گره_ids)
BTF_ID_FLAGS(func, scx_bpf_nr_cpu_ids)
BTF_ID_FLAGS(func, scx_bpf_nr_cids)
BTF_ID_FLAGS(func, scx_bpf_nr_online_cids)
BTF_ID_FLAGS(func, scx_bpf_online_cmask, KF_IMPLICIT_ARGS | KF_ARENA_RET)
BTF_ID_FLAGS(func, scx_bpf_this_cid)
BTF_ID_FLAGS(func, scx_bpf_get_possible_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_online_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_task_running, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_task_cpu, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_task_cid, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_locked_rq, KF_IMPLICIT_ARGS | KF_RET_NULL)
BTF_ID_FLAGS(func, scx_bpf_cpu_curr, KF_IMPLICIT_ARGS | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_cid_curr, KF_IMPLICIT_ARGS | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_tid_to_task, KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_now)
BTF_ID_FLAGS(func, scx_bpf_events, KF_IMPLICIT_ARGS)
#ifdef CONFIG_CGROUP_SCHED
BTF_ID_FLAGS(func, scx_bpf_task_cgroup, KF_IMPLICIT_ARGS | KF_RCU | KF_ACQUIRE)
#endif
BTF_ID_FLAGS(func, scx_bpf_sub_grant, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_sub_revoke, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_sub_caps, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_sub_kill_bstr, KF_IMPLICIT_ARGS)
BTF_KFUNCS_END(scx_kfunc_ids_any)

ایستا ثابت ساختار btf_kfunc_id_set scx_kfunc_set_any = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_any,
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
BTF_KFUNCS_START(scx_kfunc_ids_cpu_only)
BTF_ID_FLAGS(func, scx_bpf_kick_cpu, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_task_cpu, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_cpu_curr, KF_IMPLICIT_ARGS | KF_RET_NULL | KF_RCU_PROTECTED)
BTF_ID_FLAGS(func, scx_bpf_cpu_گره, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cap, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_cur, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cpuperf_set, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_get_possible_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_online_cpumask, KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask_گره, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask_گره, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_idle_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_test_and_clear_cpu_idle, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu_گره, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu_گره, KF_IMPLICIT_ARGS | KF_RCU)
BTF_KFUNCS_END(scx_kfunc_ids_cpu_only)

/*
 * Per-op kfunc allow flags. Each bit corresponds to a context-sensitive kfunc
 * group; an op may permit zero or more groups, with the union expressed in
 * scx_kf_allow_flags[]. The verifier-time filter (scx_kfunc_context_filter())
 * consults this table to decide whether a context-sensitive kfunc is callable
 * from a given SCX op.
 */
شمارش scx_kf_allow_flags {
	SCX_KF_ALLOW_UNLOCKED		= 1 << 0,
	SCX_KF_ALLOW_INIT_CIDS		= 1 << 1,
	SCX_KF_ALLOW_CPU_RELEASE	= 1 << 2,
	SCX_KF_ALLOW_DISPATCH		= 1 << 3,
	SCX_KF_ALLOW_ENQUEUE		= 1 << 4,
	SCX_KF_ALLOW_SELECT_CPU		= 1 << 5,
};

/*
 * Map each SCX op to the union of kfunc groups it permits, indexed by
 * SCX_OP_IDX(op). Ops not listed only permit kfuncs that are not
 * context-sensitive.
 */
ایستا ثابت u32 scx_kf_allow_flags[] = {
	[SCX_OP_IDX(select_cpu)]	= SCX_KF_ALLOW_SELECT_CPU | SCX_KF_ALLOW_ENQUEUE,
	[SCX_OP_IDX(enqueue)]		= SCX_KF_ALLOW_SELECT_CPU | SCX_KF_ALLOW_ENQUEUE,
	[SCX_OP_IDX(dispatch)]		= SCX_KF_ALLOW_ENQUEUE | SCX_KF_ALLOW_DISPATCH,
	[SCX_OP_IDX(cpu_release)]	= SCX_KF_ALLOW_CPU_RELEASE,
	[SCX_OP_IDX(init_task)]		= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(dump)]		= SCX_KF_ALLOW_UNLOCKED,
#ifdef CONFIG_EXT_GROUP_SCHED
	[SCX_OP_IDX(cgroup_init)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_exit)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_prep_move)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_cancel_move)] = SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_set_weight)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_set_bandwidth)] = SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cgroup_set_idle)]	= SCX_KF_ALLOW_UNLOCKED,
#endif	/* CONFIG_EXT_GROUP_SCHED */
	[SCX_OP_IDX(sub_attach)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(sub_detach)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(sub_ecaps_updated)]	= SCX_KF_ALLOW_ENQUEUE | SCX_KF_ALLOW_DISPATCH,
	[SCX_OP_IDX(cpu_online)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(cpu_offline)]	= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(init_cids)]		= SCX_KF_ALLOW_UNLOCKED | SCX_KF_ALLOW_INIT_CIDS,
	[SCX_OP_IDX(init)]		= SCX_KF_ALLOW_UNLOCKED,
	[SCX_OP_IDX(exit)]		= SCX_KF_ALLOW_UNLOCKED,
};

/*
 * Verifier-time filter for SCX kfuncs. Registered via the .filter field on
 * each per-group btf_kfunc_id_set. The BPF core invokes this for every kfunc
 * call in the registered hook (BPF_PROG_TYPE_STRUCT_OPS or
 * BPF_PROG_TYPE_SYSCALL), regardless of which set originally introduced the
 * kfunc - so the filter must short-circuit on kfuncs it doesn't govern by
 * falling through to "allow" when none of the SCX sets contain the kfunc.
 */
صحیح scx_kfunc_context_filter(ثابت ساختار bpf_prog *prog, u32 kfunc_id)
{
	bool in_unlocked = btf_id_set8_contains(&scx_kfunc_ids_unlocked, kfunc_id);
	bool in_init_cids = btf_id_set8_contains(&scx_kfunc_ids_init_cids, kfunc_id);
	bool in_select_cpu = btf_id_set8_contains(&scx_kfunc_ids_select_cpu, kfunc_id);
	bool in_enqueue = btf_id_set8_contains(&scx_kfunc_ids_enqueue_dispatch, kfunc_id);
	bool in_dispatch = btf_id_set8_contains(&scx_kfunc_ids_dispatch, kfunc_id);
	bool in_cpu_release = btf_id_set8_contains(&scx_kfunc_ids_cpu_release, kfunc_id);
	bool in_idle = btf_id_set8_contains(&scx_kfunc_ids_idle, kfunc_id);
	bool in_any = btf_id_set8_contains(&scx_kfunc_ids_any, kfunc_id);
	bool in_cpu_only = btf_id_set8_contains(&scx_kfunc_ids_cpu_only, kfunc_id);
	bool in_cid = btf_id_set8_contains(&scx_kfunc_ids_cid, kfunc_id);
	u32 moff, flags;

	/* Not an SCX kfunc - allow. */
	اگر (!(in_unlocked || in_init_cids || in_select_cpu || in_enqueue || in_dispatch ||
	      in_cpu_release || in_idle || in_any || in_cid))
		بازگشت 0;

	/* SYSCALL progs (e.g. BPF test_run()) may call unlocked and select_cpu kfuncs. */
	اگر (prog->type == BPF_PROG_TYPE_SYSCALL)
		بازگشت (in_unlocked || in_select_cpu || in_idle || in_any || in_cid) ? 0 : -EACCES;

	اگر (prog->type != BPF_PROG_TYPE_STRUCT_OPS)
		بازگشت (in_any || in_idle || in_cid) ? 0 : -EACCES;

	/*
	 * add_subprog_and_kfunc() collects all kfunc calls, including dead code
	 * guarded by bpf_ksym_exists(), before check_attach_btf_id() sets
	 * prog->aux->st_ops. Allow all kfuncs when st_ops is not yet set;
	 * do_check_main() re-runs the filter with st_ops set and enforces the
	 * actual restrictions.
	 */
	اگر (!prog->aux->st_ops)
		بازگشت 0;

	/*
	 * Non-SCX struct_ops: SCX kfuncs are not permitted.
	 *
	 * Both bpf_sched_ext_ops (cpu-form) and bpf_sched_ext_ops_cid
	 * (cid-form) are valid SCX struct_ops. Member offsets match between
	 * the two (verified by BUILD_BUG_ON in scx_init()), so the shared
	 * scx_kf_allow_flags[] table indexed by SCX_MOFF_IDX(moff) applies to
	 * both.
	 */
	اگر (prog->aux->st_ops != &bpf_sched_ext_ops &&
	    prog->aux->st_ops != &bpf_sched_ext_ops_cid)
		بازگشت -EACCES;

	/*
	 * cid-form schedulers must use cid/cmask kfuncs. cid and cpu are both
	 * small s32s and trivially confused, so cpu-only kfuncs are rejected at
	 * load time. The reverse (cpu-form calling cid-form kfuncs) is
	 * intentionally permissive to ease gradual cpumask -> cid migration.
	 */
	اگر (prog->aux->st_ops == &bpf_sched_ext_ops_cid && in_cpu_only)
		بازگشت -EACCES;

	/* SCX struct_ops: check the per-op allow list. */
	اگر (in_any || in_idle || in_cid)
		بازگشت 0;

	moff = prog->aux->attach_st_ops_member_off;
	flags = scx_kf_allow_flags[SCX_MOFF_IDX(moff)];

	اگر ((flags & SCX_KF_ALLOW_UNLOCKED) && in_unlocked)
		بازگشت 0;
	اگر ((flags & SCX_KF_ALLOW_INIT_CIDS) && in_init_cids)
		بازگشت 0;
	اگر ((flags & SCX_KF_ALLOW_CPU_RELEASE) && in_cpu_release)
		بازگشت 0;
	اگر ((flags & SCX_KF_ALLOW_DISPATCH) && in_dispatch)
		بازگشت 0;
	اگر ((flags & SCX_KF_ALLOW_ENQUEUE) && in_enqueue)
		بازگشت 0;
	اگر ((flags & SCX_KF_ALLOW_SELECT_CPU) && in_select_cpu)
		بازگشت 0;

	بازگشت -EACCES;
}

ایستا صحیح __init scx_init(تهی)
{
	صحیح ret;

	/*
	 * sched_ext_ops_cid mirrors sched_ext_ops up to and including @priv.
	 * Both bpf_scx_init_member() and bpf_scx_check_member() use offsets
	 * from struct sched_ext_ops; sched_ext_ops_cid relies on those offsets
	 * matching for the shared fields. Catch any drift at boot.
	 */
#define CID_OFFSET_MATCH(cpu_field, cid_field)					\
	BUILD_BUG_ON(offsetof(ساختار sched_ext_ops, cpu_field) !=		\
		     offsetof(ساختار sched_ext_ops_cid, cid_field))
	/* data fields used by bpf_scx_init_member() */
	CID_OFFSET_MATCH(dispatch_max_batch, dispatch_max_batch);
	CID_OFFSET_MATCH(flags, flags);
	CID_OFFSET_MATCH(name, name);
	CID_OFFSET_MATCH(timeout_ms, timeout_ms);
	CID_OFFSET_MATCH(exit_dump_len, exit_dump_len);
	CID_OFFSET_MATCH(hotplug_seq, hotplug_seq);
	CID_OFFSET_MATCH(cid_shard_size, cid_shard_size);
	CID_OFFSET_MATCH(rescue_bandwidth_ppt, rescue_bandwidth_ppt);
	CID_OFFSET_MATCH(rescue_quantum_us, rescue_quantum_us);
	CID_OFFSET_MATCH(sub_cgroup_id, sub_cgroup_id);
	/* shared callbacks: the union view requires byte-for-byte offset match */
	CID_OFFSET_MATCH(enqueue, enqueue);
	CID_OFFSET_MATCH(dequeue, dequeue);
	CID_OFFSET_MATCH(dispatch, dispatch);
	CID_OFFSET_MATCH(tick, tick);
	CID_OFFSET_MATCH(runnable, runnable);
	CID_OFFSET_MATCH(running, running);
	CID_OFFSET_MATCH(stopping, stopping);
	CID_OFFSET_MATCH(quiescent, quiescent);
	CID_OFFSET_MATCH(yield, yield);
	CID_OFFSET_MATCH(core_sched_before, core_sched_before);
	CID_OFFSET_MATCH(set_weight, set_weight);
	CID_OFFSET_MATCH(update_idle, update_idle);
	CID_OFFSET_MATCH(init_task, init_task);
	CID_OFFSET_MATCH(exit_task, exit_task);
	CID_OFFSET_MATCH(enable, enable);
	CID_OFFSET_MATCH(disable, disable);
	CID_OFFSET_MATCH(dump, dump);
	CID_OFFSET_MATCH(dump_task, dump_task);
	CID_OFFSET_MATCH(sub_attach, sub_attach);
	CID_OFFSET_MATCH(sub_detach, sub_detach);
	CID_OFFSET_MATCH(sub_caps_updated, sub_caps_updated);
	CID_OFFSET_MATCH(sub_ecaps_updated, sub_ecaps_updated);
	CID_OFFSET_MATCH(init_cids, init_cids);
	CID_OFFSET_MATCH(init, init);
	CID_OFFSET_MATCH(exit, exit);
	/* renamed callbacks must occupy the same slot as their cpu-form sibling */
	CID_OFFSET_MATCH(select_cpu, select_cid);
	CID_OFFSET_MATCH(set_cpumask, set_cmask);
	CID_OFFSET_MATCH(cpu_online, cid_online);
	CID_OFFSET_MATCH(cpu_offline, cid_offline);
	CID_OFFSET_MATCH(dump_cpu, dump_cid);
#ifdef CONFIG_EXT_GROUP_SCHED
	CID_OFFSET_MATCH(cgroup_init, cpuctl_init);
	CID_OFFSET_MATCH(cgroup_exit, cpuctl_exit);
	CID_OFFSET_MATCH(cgroup_prep_move, cpuctl_prep_move);
	CID_OFFSET_MATCH(cgroup_move, cpuctl_move);
	CID_OFFSET_MATCH(cgroup_cancel_move, cpuctl_cancel_move);
	CID_OFFSET_MATCH(cgroup_set_weight, cpuctl_set_weight);
	CID_OFFSET_MATCH(cgroup_set_bandwidth, cpuctl_set_bandwidth);
	CID_OFFSET_MATCH(cgroup_set_idle, cpuctl_set_idle);
#endif
	/* @priv tail must align since both share the same data block */
	CID_OFFSET_MATCH(priv, priv);
	/*
	 * cid-form must end exactly at @priv - scx_validate_ops() skips
	 * cpu_acquire/cpu_release for cid-form because reading those fields
	 * past the BPF allocation would be UB.
	 */
	BUILD_BUG_ON(offsetof(ساختار sched_ext_ops_cid, __end) !=
		     offsetofend(ساختار sched_ext_ops, priv));
#undef CID_OFFSET_MATCH

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
	اگر ((ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_set_enqueue_dispatch)) ||
	    (ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_set_dispatch)) ||
	    (ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_set_cpu_release)) ||
	    (ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_set_unlocked)) ||
	    (ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_SYSCALL,
					     &scx_kfunc_set_unlocked)) ||
	    (ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS,
					     &scx_kfunc_set_any)) ||
	    (ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_TRACING,
					     &scx_kfunc_set_any)) ||
	    (ret = register_btf_kfunc_id_set(BPF_PROG_TYPE_SYSCALL,
					     &scx_kfunc_set_any))) {
		pr_err("sched_ext: Failed to register kfunc sets (%d)\n", ret);
		بازگشت ret;
	}

	ret = scx_idle_init();
	اگر (ret) {
		pr_err("sched_ext: Failed to initialize idle tracking (%d)\n", ret);
		بازگشت ret;
	}

	ret = scx_cid_kfunc_init();
	اگر (ret) {
		pr_err("sched_ext: Failed to register cid kfuncs (%d)\n", ret);
		بازگشت ret;
	}

	ret = register_bpf_struct_ops(&bpf_sched_ext_ops, sched_ext_ops);
	اگر (ret) {
		pr_err("sched_ext: Failed to register struct_ops (%d)\n", ret);
		بازگشت ret;
	}

	ret = register_bpf_struct_ops(&bpf_sched_ext_ops_cid, sched_ext_ops_cid);
	اگر (ret) {
		pr_err("sched_ext: Failed to register cid struct_ops (%d)\n", ret);
		بازگشت ret;
	}

	ret = register_pm_notifier(&scx_pm_notifier);
	اگر (ret) {
		pr_err("sched_ext: Failed to register PM notifier (%d)\n", ret);
		بازگشت ret;
	}

	scx_kset = kset_create_and_add("sched_ext", &scx_uevent_ops, kernel_kobj);
	اگر (!scx_kset) {
		pr_err("sched_ext: Failed to create /sys/kernel/sched_ext\n");
		بازگشت -ENOMEM;
	}

	ret = sysfs_create_group(&scx_kset->kobj, &scx_global_attr_group);
	اگر (ret < 0) {
		pr_err("sched_ext: Failed to add global attributes\n");
		بازگشت ret;
	}

	بازگشت 0;
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
#ifdef CONFIG_EXT_GROUP_SCHED
DEFINE_SCX_COMPAT_MARKER(cgroup_set_bandwidth_may_sleep);
#endif	/* CONFIG_EXT_GROUP_SCHED */
