/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#اگر_غیر_تعریف_شدہ KERNEL_SCHED_EXT_اندرونی_H
#تعریف_کرو KERNEL_SCHED_EXT_اندرونی_H

#شامل "../sched.h"
#شامل "types.h"

#شامل <trace/events/sched_ext.h>

/**
 * scx_add_event - Increase an event counter for 'name' by 'cnt'
 * @sch: scx_sched to account events for
 * @name: an event name defined in struct scx_event_stats
 * @cnt: the number of the event occurred
 *
 * This can be used when preemption is not disabled.
 */
#تعریف_کرو scx_جوڑو_event(sch, name, cnt) کرو {					\
	this_cpu_جوڑو((sch)->pcpu->event_stats.name, (cnt));			\
	trace_sched_ext_event(#name, (cnt));					\
} جب(0)

/**
 * __scx_add_event - Increase an event counter for 'name' by 'cnt'
 * @sch: scx_sched to account events for
 * @name: an event name defined in struct scx_event_stats
 * @cnt: the number of the event occurred
 *
 * This should be used only when preemption is disabled.
 */
#تعریف_کرو __scx_add_event(sch, name, cnt) کرو {					\
	__this_cpu_add((sch)->pcpu->event_stats.name, (cnt));			\
	trace_sched_ext_event(#name, cnt);					\
} جب(0)

#تعریف_کرو SCX_OP_IDX(op)		(offsetof(ساخت sched_ext_ops, op) / حجم(خالی (*)(خالی)))
#تعریف_کرو SCX_MOFF_IDX(moff)	((moff) / حجم(خالی (*)(خالی)))

شمار scx_exit_قسم {
	SCX_EXIT_NONE,
	SCX_EXIT_مکمل,

	SCX_EXIT_UNREG = 64,	/* user-space initiated unregistration */
	SCX_EXIT_UNREG_BPF,	/* BPF-initiated unregistration */
	SCX_EXIT_UNREG_KERN,	/* kernel-initiated unregistration */
	SCX_EXIT_SYSRQ,		/* requested by 'S' sysrq */
	SCX_EXIT_والد,	/* parent exiting */
	SCX_EXIT_والد_KILL,	/* killed by parent scheduler */

	SCX_EXIT_ERROR = 1024,	/* runtime error, error msg contains details */
	SCX_EXIT_ERROR_BPF,	/* ERROR but triggered through scx_bpf_error() */
	SCX_EXIT_ERROR_STALL,	/* watchdog detected stalled runnable tasks */
	SCX_EXIT_ERROR_REENQ,	/* task hit reenqueue limit without running */
	SCX_EXIT_ERROR_RESCUE,	/* ejected for overloading rescue execution */
};

/*
 * An exit code can be specified when exiting with scx_bpf_exit() or scx_exit(),
 * corresponding to exit_kind UNREG_BPF and UNREG_KERN respectively. The codes
 * are 64bit of the format:
 *
 *   Bits: [63  ..  48 47   ..  32 31 .. 0]
 *         [ SYS ACT ] [ SYS RSN ] [ USR  ]
 *
 *   SYS ACT: System-defined exit actions
 *   SYS RSN: System-defined exit reasons
 *   USR    : User-defined exit codes and reasons
 *
 * Using the above, users may communicate intention and context by ORing system
 * actions and/or system reasons with a user-defined exit code.
 */
شمار scx_exit_رمز {
	/* Reasons */
	SCX_ECODE_RSN_HOTPLUG	= 1LLU << 32,
	SCX_ECODE_RSN_CGROUP_OFFLINE = 2LLU << 32,

	/* Actions */
	SCX_ECODE_ACT_RESTART	= 1LLU << 48,
};

شمار scx_exit_flags {
	/*
	 * ops.exit() may be called even if the loading failed before ops.init()
	 * finishes successfully. This is because ops.exit() allows rich exit
	 * info communication. The following flag indicates whether ops.init()
	 * finished successfully.
	 */
	SCX_EFLAG_INITIALIZED   = 1LLU << 0,
};

/*
 * scx_exit_info is passed to ops.exit() to describe why the BPF scheduler is
 * being disabled.
 */
ساخت scx_exit_info {
	/* %SCX_EXIT_* - broad category of the exit reason */
	شمار scx_exit_قسم	قسم;

	/*
	 * CPU that initiated the exit, valid once @kind has been set.
	 * Negative if the exit path didn't identify a CPU.
	 */
	s32			exit_cpu;

	/* exit code if gracefully exiting */
	s64			exit_رمز;

	/* %SCX_EFLAG_* */
	u64			flags;

	/* textual representation of the above */
	ثابت حرف		*reason;

	/* backtrace if exiting due to an error */
	غیرعلامتی طویل		*bt;
	u32			bt_len;

	/* informational message */
	حرف			*پیغام;

	/* debug dump */
	حرف			*dump;
};

/* sched_ext_ops.flags */
شمار scx_ops_flags {
	/*
	 * Keep built-in idle tracking even if ops.update_idle() is implemented.
	 */
	SCX_OPS_KEEP_BUILTIN_IDLE	= 1LLU << 0,

	/*
	 * By default, if there are no other task to run on the CPU, ext core
	 * keeps running the current task even after its slice expires. If this
	 * flag is specified, such tasks are passed to ops.enqueue() with
	 * %SCX_ENQ_LAST. See the comment above %SCX_ENQ_LAST for more info.
	 */
	SCX_OPS_ENQ_آخری		= 1LLU << 1,

	/*
	 * An exiting task may schedule after PF_EXITING is set. In such cases,
	 * bpf_task_from_pid() may not be able to find the task and if the BPF
	 * scheduler depends on pid lookup for dispatching, the task will be
	 * lost leading to various issues including RCU grace period stalls.
	 *
	 * To mask this problem, by default, unhashed tasks are automatically
	 * dispatched to the local DSQ on enqueue. If the BPF scheduler doesn't
	 * depend on pid lookups and wants to handle these tasks directly, the
	 * following flag can be used. With %SCX_OPS_TID_TO_TASK,
	 * scx_bpf_tid_to_task() can find exiting tasks reliably.
	 */
	SCX_OPS_ENQ_EXITING		= 1LLU << 2,

	/*
	 * If set, only tasks with policy set to SCHED_EXT are attached to
	 * sched_ext. If clear, SCHED_NORMAL tasks are also included.
	 */
	SCX_OPS_SWITCH_PARTIAL		= 1LLU << 3,

	/*
	 * A migration disabled task can only execute on its current CPU. By
	 * default, such tasks are automatically put on the CPU's local DSQ with
	 * the default slice on enqueue. If this ops flag is set, they also go
	 * through ops.enqueue().
	 *
	 * A migration disabled task never invokes ops.select_cpu() as it can
	 * only select the current CPU. Also, p->cpus_ptr will only contain its
	 * current CPU while p->nr_cpus_allowed keeps tracking p->user_cpus_ptr
	 * and thus may disagree with cpumask_weight(p->cpus_ptr).
	 */
	SCX_OPS_ENQ_MIGRATION_DISABLED	= 1LLU << 4,

	/*
	 * Queued wakeup (ttwu_queue) is a wakeup optimization that invokes
	 * ops.enqueue() on the ops.select_cpu() selected or the wakee's
	 * previous CPU via IPI (inter-processor interrupt) to reduce cacheline
	 * transfers. When this optimization is enabled, ops.select_cpu() is
	 * skipped in some cases (when racing against the wakee switching out).
	 * As the BPF scheduler may depend on ops.select_cpu() being invoked
	 * during wakeups, queued wakeup is disabled by default.
	 *
	 * If this ops flag is set, queued wakeup optimization is enabled and
	 * the BPF scheduler must be able to handle ops.enqueue() invoked on the
	 * wakee's CPU without preceding ops.select_cpu() even for tasks which
	 * may be executed on multiple CPUs.
	 */
	SCX_OPS_ALLOW_QUEUED_WAKEUP	= 1LLU << 5,

	/*
	 * If set, enable per-node idle cpumasks. If clear, use a single global
	 * flat idle cpumask.
	 */
	SCX_OPS_BUILTIN_IDLE_فی_گرہ	= 1LLU << 6,

	/*
	 * If set, %SCX_ENQ_IMMED is assumed to be set on all local DSQ
	 * enqueues.
	 */
	SCX_OPS_ALWAYS_ENQ_IMMED	= 1LLU << 7,

	/*
	 * Maintain a mapping from p->scx.tid to task_struct so the BPF
	 * scheduler can recover task pointers from stored tids via
	 * scx_bpf_tid_to_task().
	 *
	 * Only the root scheduler turns this on. A sub-sched may set the flag
	 * to declare a dependency on the lookup; if the root scheduler hasn't
	 * enabled it, attaching the sub-sched is rejected.
	 */
	SCX_OPS_TID_مقصد_TASK		= 1LLU << 8,

	SCX_OPS_ALL_FLAGS		= SCX_OPS_KEEP_BUILTIN_IDLE |
					  SCX_OPS_ENQ_آخری |
					  SCX_OPS_ENQ_EXITING |
					  SCX_OPS_ENQ_MIGRATION_DISABLED |
					  SCX_OPS_ALLOW_QUEUED_WAKEUP |
					  SCX_OPS_SWITCH_PARTIAL |
					  SCX_OPS_BUILTIN_IDLE_فی_گرہ |
					  SCX_OPS_ALWAYS_ENQ_IMMED |
					  SCX_OPS_TID_مقصد_TASK,

	/* high 8 bits are internal, don't include in SCX_OPS_ALL_FLAGS */
	__SCX_OPS_INTERNAL_MASK		= 0xffLLU << 56,

	SCX_OPS_HAS_CPU_PREEMPT		= 1LLU << 56,
};

/* argument container for ops.init_task() */
ساخت scx_init_task_دلائل {
	/*
	 * Set if ops.init_task() is being invoked on the fork path, as opposed
	 * to the scheduler transition path.
	 */
	bool			fork;
#اگر_تعریف_شدہ CONFIG_EXT_گروہ_SCHED
	/* the cgroup the task is joining */
	ساخت cgroup		*cgroup;
#اگر_ختم
};

/* argument container for ops.exit_task() */
ساخت scx_exit_task_دلائل {
	/* Whether the task exited before running on sched_ext. */
	bool cancelled;
};

/**
 * struct scx_enable_args - Argument container for cid-form ops.enable()
 * @cmask_arena_addr: BPF arena address of the cmask of cids the task may run on
 *
 * @cmask_arena_addr is the task's affinity as it enters the scheduler.
 * set_cmask() delivers the same mask right after enable(), before set_weight()
 * and the first enqueue, then every affinity change afterwards, and is never
 * called before enable(). A scheduler may therefore track affinity in
 * set_cmask() alone.
 *
 * The kernel builds the mask in the scheduler arena from its own geometry, so
 * the header is valid regardless of what the scheduler last wrote there. The
 * memory is per-cpu scratch reused once the callback returns: copy the bits
 * out, don't keep the address. The set_cmask() argument follows the same rules.
 *
 * The address is a plain value rather than a typed pointer because BTF can't
 * mark a struct member as an arena pointer yet and a pointer member would reach
 * the program typed as a kernel pointer. Cast it to struct scx_cmask __arena *
 * before use. Once arena members can be typed, a typed alias will join this
 * field in an anonymous union at the same offset.
 */
ساخت scx_enable_دلائل {
	u64	cmask_arena_addr;
};

/* argument container for ops.cgroup_init() */
ساخت scx_cgroup_init_دلائل {
	/* the weight of the cgroup [1..10000] */
	u32			وزن;

	/* bandwidth control parameters from cpu.max and cpu.max.burst */
	u64			bw_period_us;
	u64			bw_quota_us;
	u64			bw_مدت_us;

	/* whether the cgroup is configured SCHED_IDLE via cpu.idle */
	bool			sched_idle;
};

شمار scx_cpu_preempt_reason {
	/* next task is being scheduled by &sched_class_rt */
	SCX_CPU_PREEMPT_RT,
	/* next task is being scheduled by &sched_class_dl */
	SCX_CPU_PREEMPT_DL,
	/* next task is being scheduled by &sched_class_stop */
	SCX_CPU_PREEMPT_STOP,
	/* unknown reason for SCX being preempted */
	SCX_CPU_PREEMPT_UNKNOWN,
};

/*
 * Argument container for ops.cpu_acquire(). Currently empty, but may be
 * expanded in the future.
 */
ساخت scx_cpu_acquire_دلائل {};

/* argument container for ops.cpu_release() */
ساخت scx_cpu_release_دلائل {
	/* the reason the CPU was preempted */
	شمار scx_cpu_preempt_reason reason;

	/* the task that's going to be scheduled on the CPU */
	ساخت task_struct	*task;
};

/* informational context provided to dump operations */
ساخت scx_dump_ctx {
	شمار scx_exit_قسم	قسم;
	s64			exit_رمز;
	ثابت حرف		*reason;
	u64			at_ns;
	u64			at_jiffies;
};

/* argument container for ops.sub_attach() */
ساخت scx_sub_attach_دلائل {
	ساخت sched_ext_ops	*ops;
	حرف			*cgroup_راستہ;
};

/* argument container for ops.sub_detach() */
ساخت scx_sub_detach_دلائل {
	ساخت sched_ext_ops	*ops;
	حرف			*cgroup_راستہ;
};

/**
 * struct sched_ext_ops - Operation table for BPF scheduler implementation
 *
 * A BPF scheduler can implement an arbitrary scheduling policy by
 * implementing and loading operations in this table. Note that a userland
 * scheduling policy can also be implemented using the BPF scheduler
 * as a shim layer.
 */
ساخت sched_ext_ops {
	/**
	 * @select_cpu: Pick the target CPU for a task which is being woken up
	 * @p: task being woken up
	 * @prev_cpu: the cpu @p was on before sleeping
	 * @wake_flags: SCX_WAKE_*
	 *
	 * Decision made here isn't final. @p may be moved to any CPU while it
	 * is getting dispatched for execution later. However, as @p is not on
	 * the rq at this point, getting the eventual execution CPU right here
	 * saves a small bit of overhead down the line.
	 *
	 * If an idle CPU is returned, the CPU is kicked and will try to
	 * dispatch. While an explicit custom mechanism can be added,
	 * select_cpu() serves as the default way to wake up idle CPUs.
	 *
	 * @p may be inserted into a DSQ directly by calling
	 * scx_bpf_dsq_insert(). If so, the ops.enqueue() will be skipped.
	 * Directly inserting into %SCX_DSQ_LOCAL will put @p in the local DSQ
	 * of the CPU returned by this operation.
	 *
	 * Note that select_cpu() is never called for tasks that can only run
	 * on a single CPU or tasks with migration disabled, as they don't have
	 * the option to select a different CPU. See select_task_rq() for
	 * details.
	 */
	s32 (*select_cpu)(ساخت task_struct *p, s32 پچھلا_cpu, u64 wake_flags);

	/**
	 * @enqueue: Enqueue a task on the BPF scheduler
	 * @p: task being enqueued
	 * @enq_flags: %SCX_ENQ_*
	 *
	 * @p is ready to run. Insert directly into a DSQ by calling
	 * scx_bpf_dsq_insert() or enqueue on the BPF scheduler. If not directly
	 * inserted, the bpf scheduler owns @p and if it fails to dispatch @p,
	 * the task will stall.
	 *
	 * If @p was inserted into a DSQ from ops.select_cpu(), this callback is
	 * skipped.
	 */
	خالی (*قطار_میں_ڈالو)(ساخت task_struct *p, u64 enq_flags);

	/**
	 * @dequeue: Remove a task from the BPF scheduler
	 * @p: task being dequeued
	 * @deq_flags: %SCX_DEQ_*
	 *
	 * Remove @p from the BPF scheduler. This is usually called to isolate
	 * the task while updating its scheduling properties (e.g. priority).
	 *
	 * The ext core keeps track of whether the BPF side owns a given task or
	 * not and can gracefully ignore spurious dispatches from BPF side,
	 * which makes it safe to not implement this method. However, depending
	 * on the scheduling logic, this can lead to confusing behaviors - e.g.
	 * scheduling position not being updated across a priority change.
	 */
	خالی (*قطار_سے_نکالو)(ساخت task_struct *p, u64 deq_flags);

	/**
	 * @dispatch: Dispatch tasks from the BPF scheduler and/or user DSQs
	 * @cpu: CPU to dispatch tasks for
	 * @prev: previous task being switched out
	 *
	 * Called when a CPU's local dsq is empty. The operation should dispatch
	 * one or more tasks from the BPF scheduler into the DSQs using
	 * scx_bpf_dsq_insert() and/or move from user DSQs into the local DSQ
	 * using scx_bpf_dsq_move_to_local().
	 *
	 * The maximum number of times scx_bpf_dsq_insert() can be called
	 * without an intervening scx_bpf_dsq_move_to_local() is specified by
	 * ops.dispatch_max_batch. See the comments on top of the two functions
	 * for more details.
	 *
	 * When not %NULL, @prev is an SCX task with its slice depleted. If
	 * @prev is still runnable as indicated by set %SCX_TASK_QUEUED in
	 * @prev->scx.flags, it is not enqueued yet and will be enqueued after
	 * ops.dispatch() returns. To keep executing @prev, return without
	 * dispatching or moving any tasks. Also see %SCX_OPS_ENQ_LAST.
	 */
	خالی (*dispatch)(s32 cpu, ساخت task_struct *پچھلا);

	/**
	 * @tick: Periodic tick
	 * @p: task running currently
	 *
	 * This operation is called every 1/HZ seconds on CPUs which are
	 * executing an SCX task. Setting a slice of 0 for @p with
	 * scx_bpf_task_set_slice() will trigger an immediate dispatch cycle on
	 * the CPU.
	 */
	خالی (*tick)(ساخت task_struct *p);

	/**
	 * @runnable: A task is becoming runnable on its associated CPU
	 * @p: task becoming runnable
	 * @enq_flags: %SCX_ENQ_*
	 *
	 * This and the following three functions can be used to track a task's
	 * execution state transitions. A task becomes ->runnable() on a CPU,
	 * and then goes through one or more ->running() and ->stopping() pairs
	 * as it runs on the CPU, and eventually becomes ->quiescent() when it's
	 * done running on the CPU.
	 *
	 * @p is becoming runnable on the CPU because it's
	 *
	 * - waking up (%SCX_ENQ_WAKEUP)
	 * - being moved from another CPU
	 * - being restored after temporarily taken off the queue for an
	 *   attribute change.
	 *
	 * This and ->enqueue() are related but not coupled. This operation
	 * notifies @p's state transition and may not be followed by ->enqueue()
	 * e.g. when @p is being dispatched to a remote CPU, or when @p is
	 * being enqueued on a CPU experiencing a hotplug event. Likewise, a
	 * task may be ->enqueue()'d without being preceded by this operation
	 * e.g. after exhausting its slice.
	 */
	خالی (*runnable)(ساخت task_struct *p, u64 enq_flags);

	/**
	 * @running: A task is starting to run on its associated CPU
	 * @p: task starting to run
	 *
	 * Note that this callback may be called from a CPU other than the
	 * one the task is going to run on. This can happen when a task
	 * property is changed (i.e., affinity), since set_next_task_scx(),
	 * which triggers this callback, may run on a CPU different from
	 * the task's assigned CPU.
	 *
	 * Therefore, always use scx_bpf_task_cpu(@p) to determine the
	 * target CPU the task is going to use.
	 *
	 * See ->runnable() for explanation on the task state notifiers.
	 */
	خالی (*running)(ساخت task_struct *p);

	/**
	 * @stopping: A task is stopping execution
	 * @p: task stopping to run
	 * @runnable: is task @p still runnable?
	 *
	 * Note that this callback may be called from a CPU other than the
	 * one the task was running on. This can happen when a task
	 * property is changed (i.e., affinity), since dequeue_task_scx(),
	 * which triggers this callback, may run on a CPU different from
	 * the task's assigned CPU.
	 *
	 * Therefore, always use scx_bpf_task_cpu(@p) to retrieve the CPU
	 * the task was running on.
	 *
	 * See ->runnable() for explanation on the task state notifiers. If
	 * !@runnable, ->quiescent() will be invoked after this operation
	 * returns.
	 */
	خالی (*stopping)(ساخت task_struct *p, bool runnable);

	/**
	 * @quiescent: A task is becoming not runnable on its associated CPU
	 * @p: task becoming not runnable
	 * @deq_flags: %SCX_DEQ_*
	 *
	 * See ->runnable() for explanation on the task state notifiers.
	 *
	 * @p is becoming quiescent on the CPU because it's
	 *
	 * - sleeping (%SCX_DEQ_SLEEP)
	 * - being moved to another CPU
	 * - being temporarily taken off the queue for an attribute change
	 *   (%SCX_DEQ_SCHED_CHANGE)
	 *
	 * This and ->dequeue() are related but not coupled. This operation
	 * notifies @p's state transition and may not be preceded by ->dequeue()
	 * e.g. when @p is being dispatched to a remote CPU.
	 */
	خالی (*quiescent)(ساخت task_struct *p, u64 deq_flags);

	/**
	 * @yield: Yield CPU
	 * @from: yielding task
	 * @to: optional yield target task
	 *
	 * If @to is NULL, @from is yielding the CPU to other runnable tasks.
	 * The BPF scheduler should ensure that other available tasks are
	 * dispatched before the yielding task. Return value is ignored in this
	 * case.
	 *
	 * If @to is not-NULL, @from wants to yield the CPU to @to. If the bpf
	 * scheduler can implement the request, return %true; otherwise, %false.
	 */
	bool (*yield)(ساخت task_struct *from, ساخت task_struct *مقصد);

	/**
	 * @core_sched_before: Task ordering for core-sched
	 * @a: task A
	 * @b: task B
	 *
	 * Used by core-sched to determine the ordering between two tasks. See
	 * Documentation/admin-guide/hw-vuln/core-scheduling.rst for details on
	 * core-sched.
	 *
	 * Both @a and @b are runnable and may or may not currently be queued on
	 * the BPF scheduler. Should return %true if @a should run before @b.
	 * %false if there's no required ordering or @b should run before @a.
	 *
	 * In a scheduler hierarchy, a pair spanning two schedulers is ordered
	 * by the nearest common ancestor implementing this op, so the op may be
	 * called on tasks that the scheduler delegated to its sub-schedulers
	 * and is not scheduling anymore. See scx_prio_less().
	 *
	 * If not specified, the default is ordering them according to when they
	 * became runnable.
	 */
	bool (*core_sched_before)(ساخت task_struct *a, ساخت task_struct *b);

	/**
	 * @set_weight: Set task weight
	 * @p: task to set weight for
	 * @weight: new weight [1..10000]
	 *
	 * Update @p's weight to @weight.
	 */
	خالی (*سیٹ_وزن)(ساخت task_struct *p, u32 وزن);

	/**
	 * @set_cpumask: Set CPU affinity
	 * @p: task to set CPU affinity for
	 * @cpumask: cpumask of cpus that @p can run on
	 *
	 * Update @p's CPU affinity to @cpumask.
	 */
	خالی (*سیٹ_cpumask)(ساخت task_struct *p,
			    ثابت ساخت cpumask *cpumask);

	/**
	 * @update_idle: Update the idle state of a CPU
	 * @cpu: CPU to update the idle state for
	 * @idle: whether entering or exiting the idle state
	 *
	 * This operation is called when @rq's CPU goes or leaves the idle
	 * state. By default, implementing this operation disables the built-in
	 * idle CPU tracking and the following helpers become unavailable:
	 *
	 * - scx_bpf_select_cpu_dfl()
	 * - scx_bpf_select_cpu_and()
	 * - scx_bpf_test_and_clear_cpu_idle()
	 * - scx_bpf_pick_idle_cpu()
	 *
	 * The user also must implement ops.select_cpu() as the default
	 * implementation relies on scx_bpf_select_cpu_dfl().
	 *
	 * Specify the %SCX_OPS_KEEP_BUILTIN_IDLE flag to keep the built-in idle
	 * tracking.
	 *
	 * Only actual transitions are reported. A CPU that is claimed with an
	 * idle pick and kicked but dispatches no task returns to idle without a
	 * transition. A scheduler tracking idle CPUs itself must restore the
	 * idle state from ops.dispatch() when it returns without the next task
	 * to run.
	 */
	خالی (*update_idle)(s32 cpu, bool idle);

	/**
	 * @init_task: Initialize a task to run in a BPF scheduler
	 * @p: task to initialize for BPF scheduling
	 * @args: init arguments, see the struct definition
	 *
	 * Either we're loading a BPF scheduler or a new task is being forked.
	 * Initialize @p for BPF scheduling. This operation may block and can
	 * be used for allocations, and is called exactly once for a task.
	 *
	 * Return 0 for success, -errno for failure. An error return while
	 * loading will abort loading of the BPF scheduler. During a fork, it
	 * will abort that specific fork.
	 */
	s32 (*init_task)(ساخت task_struct *p, ساخت scx_init_task_دلائل *دلائل);

	/**
	 * @exit_task: Exit a previously-running task from the system
	 * @p: task to exit
	 * @args: exit arguments, see the struct definition
	 *
	 * @p is exiting or the BPF scheduler is being unloaded. Perform any
	 * necessary cleanup for @p.
	 */
	خالی (*exit_task)(ساخت task_struct *p, ساخت scx_exit_task_دلائل *دلائل);

	/**
	 * @enable: Enable BPF scheduling for a task
	 * @p: task to enable BPF scheduling for
	 *
	 * Enable @p for BPF scheduling. enable() is called on @p any time it
	 * enters SCX, and is always paired with a matching disable().
	 */
	خالی (*enable)(ساخت task_struct *p);

	/**
	 * @disable: Disable BPF scheduling for a task
	 * @p: task to disable BPF scheduling for
	 *
	 * @p is exiting, leaving SCX or the BPF scheduler is being unloaded.
	 * Disable BPF scheduling for @p. A disable() call is always matched
	 * with a prior enable() call.
	 */
	خالی (*disable)(ساخت task_struct *p);

	/**
	 * @dump: Dump BPF scheduler state on error
	 * @ctx: debug dump context
	 *
	 * Use scx_bpf_dump() to generate BPF scheduler specific debug dump.
	 */
	خالی (*dump)(ساخت scx_dump_ctx *ctx);

	/**
	 * @dump_cpu: Dump BPF scheduler state for a CPU on error
	 * @ctx: debug dump context
	 * @cpu: CPU to generate debug dump for
	 * @idle: @cpu is currently idle without any runnable tasks
	 *
	 * Use scx_bpf_dump() to generate BPF scheduler specific debug dump for
	 * @cpu. If @idle is %true and this operation doesn't produce any
	 * output, @cpu is skipped for dump.
	 */
	خالی (*dump_cpu)(ساخت scx_dump_ctx *ctx, s32 cpu, bool idle);

	/**
	 * @dump_task: Dump BPF scheduler state for a runnable task on error
	 * @ctx: debug dump context
	 * @p: runnable task to generate debug dump for
	 *
	 * Use scx_bpf_dump() to generate BPF scheduler specific debug dump for
	 * @p.
	 */
	خالی (*dump_task)(ساخت scx_dump_ctx *ctx, ساخت task_struct *p);

#اگر_تعریف_شدہ CONFIG_EXT_گروہ_SCHED
	/**
	 * @cgroup_init: Initialize a cgroup
	 * @cgrp: cgroup being initialized
	 * @args: init arguments, see the struct definition
	 *
	 * Initialize @cgrp for sched_ext, delivered to @cgrp's sched either
	 * when the BPF scheduler is being loaded or when @cgrp is created. This
	 * operation may block.
	 *
	 * Cgroup handovers also generate these ops: an enabling sub-scheduler
	 * receives ops.cgroup_init() for every cgroup in its subtree while the
	 * previous sched receives ops.cgroup_exit(), and disabling reverses the
	 * two.
	 *
	 * When the BPF scheduler is being loaded or cgroups are being handed
	 * over, @cgrp may already have been removed by userspace: a removed
	 * cgroup stays schedulable until its dying tasks finish their final
	 * context switches.
	 *
	 * Return 0 for success, -errno for failure. An error return while
	 * loading will abort loading of the BPF scheduler. During cgroup
	 * creation, it will abort the specific cgroup creation.
	 */
	s32 (*cgroup_init)(ساخت cgroup *cgrp,
			   ساخت scx_cgroup_init_دلائل *دلائل);

	/**
	 * @cgroup_exit: Exit a cgroup
	 * @cgrp: cgroup being exited
	 *
	 * Exit @cgrp for sched_ext, delivered to the sched whose
	 * ops.cgroup_init() it pairs with, either when the BPF scheduler is
	 * being unloaded or when @cgrp is destroyed. This operation may block.
	 *
	 * For a destroyed @cgrp, delivery follows the last scheduling event on
	 * it: a removed cgroup stays schedulable until its dying tasks finish
	 * their final context switches.
	 */
	خالی (*cgroup_exit)(ساخت cgroup *cgrp);

	/**
	 * @cgroup_prep_move: Prepare a task to be moved to a different cgroup
	 * @p: task being moved
	 * @from: cgroup @p is being moved from
	 * @to: cgroup @p is being moved to
	 *
	 * Prepare @p for move from cgroup @from to @to. This operation may
	 * block and can be used for allocations.
	 *
	 * The cgroup_move ops are delivered to @p's sched, and only for moves
	 * that don't re-home @p. A re-homing move is reported through
	 * ops.exit_task() and ops.init_task() instead. @from and @to can
	 * reference cgroups the sched never received ops.cgroup_init() for, as
	 * the cpu controller can be coarser than the sub-scheduler topology.
	 *
	 * Return 0 for success, -errno for failure. An error return aborts the
	 * migration.
	 */
	s32 (*cgroup_prep_move)(ساخت task_struct *p,
				ساخت cgroup *from, ساخت cgroup *مقصد);

	/**
	 * @cgroup_move: Commit cgroup move
	 * @p: task being moved
	 * @from: cgroup @p is being moved from
	 * @to: cgroup @p is being moved to
	 *
	 * Commit the move. @p is dequeued during this operation.
	 */
	خالی (*cgroup_move)(ساخت task_struct *p,
			    ساخت cgroup *from, ساخت cgroup *مقصد);

	/**
	 * @cgroup_cancel_move: Cancel cgroup move
	 * @p: task whose cgroup move is being canceled
	 * @from: cgroup @p was being moved from
	 * @to: cgroup @p was being moved to
	 *
	 * @p was cgroup_prep_move()'d but failed before reaching cgroup_move().
	 * Undo the preparation.
	 */
	خالی (*cgroup_cancel_move)(ساخت task_struct *p,
				   ساخت cgroup *from, ساخت cgroup *مقصد);

	/**
	 * @cgroup_set_weight: A cgroup's weight is being changed
	 * @cgrp: cgroup whose weight is being updated
	 * @weight: new weight [1..10000]
	 *
	 * Update @cgrp's weight to @weight.
	 *
	 * Knobs of a cgroup belong to the parent, so the set_* ops are
	 * delivered to @cgrp's parent's sched. That sched may never have seen
	 * ops.cgroup_init() for @cgrp - at a sub-scheduler attach point, the
	 * parent sched tracks @cgrp through ops.sub_attach() instead.
	 */
	خالی (*cgroup_سیٹ_وزن)(ساخت cgroup *cgrp, u32 وزن);

	/**
	 * @cgroup_set_bandwidth: A cgroup's bandwidth is being changed
	 * @cgrp: cgroup whose bandwidth is being updated
	 * @period_us: bandwidth control period
	 * @quota_us: bandwidth control quota
	 * @burst_us: bandwidth control burst
	 *
	 * Update @cgrp's bandwidth control parameters. This is from the cpu.max
	 * cgroup interface. This operation may block.
	 *
	 * @quota_us / @period_us determines the CPU bandwidth @cgrp is entitled
	 * to. For example, if @period_us is 1_000_000 and @quota_us is
	 * 2_500_000. @cgrp is entitled to 2.5 CPUs. @burst_us can be
	 * interpreted in the same fashion and specifies how much @cgrp can
	 * burst temporarily. The specific control mechanism and thus the
	 * interpretation of @period_us and burstiness is up to the BPF
	 * scheduler.
	 *
	 * Delivery follows the same rule as cgroup_set_weight().
	 */
	خالی (*cgroup_سیٹ_bandwidth)(ساخت cgroup *cgrp,
				     u64 period_us, u64 quota_us, u64 مدت_us);

	/**
	 * @cgroup_set_idle: A cgroup's idle state is being changed
	 * @cgrp: cgroup whose idle state is being updated
	 * @idle: whether the cgroup is entering or exiting idle state
	 *
	 * Update @cgrp's idle state to @idle. This callback is invoked when
	 * a cgroup transitions between idle and non-idle states, allowing the
	 * BPF scheduler to adjust its behavior accordingly.
	 *
	 * Delivery follows the same rule as cgroup_set_weight().
	 */
	خالی (*cgroup_سیٹ_idle)(ساخت cgroup *cgrp, bool idle);

#اگر_ختم	/* CONFIG_EXT_GROUP_SCHED */

	/**
	 * @sub_attach: Attach a sub-scheduler
	 * @args: argument container, see the struct definition
	 *
	 * Return 0 to accept the sub-scheduler. -errno to reject.
	 */
	s32 (*sub_attach)(ساخت scx_sub_attach_دلائل *دلائل);

	/**
	 * @sub_detach: Detach a sub-scheduler
	 * @args: argument container, see the struct definition
	 */
	خالی (*sub_detach)(ساخت scx_sub_detach_دلائل *دلائل);

	/**
	 * @sub_caps_updated: Caps on this sub-sched's shard changed
	 * @cmask: cids whose caps changed (cmask->base identifies the shard)
	 * @caps: SCX_CAP_* that changed
	 *
	 * Invoked after grant or revoke modifies caps on a shard. There can be
	 * only one in-flight invocation per shard. @cmask and @caps coalesce
	 * all changes since the last delivery. Direction (set vs cleared) isn't
	 * encoded. Query current state with scx_bpf_sub_caps().
	 *
	 * Delivered asynchronously after the change is recorded, and may run
	 * before it takes effect on any given cpu. Use it to track which caps
	 * the sub-sched holds and propagate to its own children, not to decide
	 * if a task can run on a cpu now. sub_ecaps_updated() reports that per
	 * cpu, once it is in effect.
	 *
	 * May call scx_bpf_sub_grant() / scx_bpf_sub_revoke() on children.
	 */
	خالی (*sub_caps_updated)(ثابت ساخت scx_cmask *cmask, u64 caps);

	/**
	 * @sub_ecaps_updated: This sub-sched's effective caps on a cid changed
	 * @cid: the cid whose effective caps changed
	 * @before: effective caps as of the last delivery
	 * @after: effective caps now
	 *
	 * Invoked when this sub-sched's effective caps on @cid change, once the
	 * change is in effect on the cpu. Runs in dispatch context with rq lock
	 * held, and can perform all operations allowed in ops.dispatch()
	 * including inserting/moving tasks.
	 */
	خالی (*sub_ecaps_updated)(s32 cid, u64 before, u64 after);

	/*
	 * All online ops must come before ops.cpu_online().
	 */

	/**
	 * @cpu_online: A CPU became online
	 * @cpu: CPU which just came up
	 *
	 * @cpu just came online. @cpu will not call ops.enqueue() or
	 * ops.dispatch(), nor run tasks associated with other CPUs beforehand.
	 */
	خالی (*cpu_online)(s32 cpu);

	/**
	 * @cpu_offline: A CPU is going offline
	 * @cpu: CPU which is going offline
	 *
	 * @cpu is going offline. @cpu will not call ops.enqueue() or
	 * ops.dispatch(), nor run tasks associated with other CPUs afterwards.
	 */
	خالی (*cpu_offline)(s32 cpu);

	/*
	 * All CPU hotplug ops must come before ops.init_cids().
	 */

	/**
	 * @init_cids: Finalize the cid layout (cid-form only)
	 *
	 * Runs after the default cid layout is built, before caps and shards
	 * are finalized. A cid-form scheduler may call scx_bpf_cid_override()
	 * here for a custom layout. Ignored for cpu-form schedulers.
	 */
	s32 (*init_cids)(خالی);

	/**
	 * @init: Initialize the BPF scheduler
	 */
	s32 (*init)(خالی);

	/**
	 * @exit: Clean up after the BPF scheduler
	 * @info: Exit info
	 *
	 * ops.exit() is also called on ops.init() failure, which is a bit
	 * unusual. This is to allow rich reporting through @info on how
	 * ops.init() failed.
	 */
	خالی (*باہر_نکلو)(ساخت scx_exit_info *info);

	/*
	 * Data fields must comes after all ops fields.
	 */

	/**
	 * @dispatch_max_batch: Max nr of tasks that dispatch() can dispatch
	 */
	u32 dispatch_max_batch;

	/**
	 * @flags: %SCX_OPS_* flags
	 */
	u64 flags;

	/**
	 * @timeout_ms: The maximum amount of time, in milliseconds, that a
	 * runnable task should be able to wait before being scheduled. The
	 * maximum timeout may not exceed the default timeout of 30 seconds.
	 *
	 * Defaults to the maximum allowed timeout value of 30 seconds.
	 */
	u32 timeout_ms;

	/**
	 * @exit_dump_len: scx_exit_info.dump buffer length. If 0, the default
	 * value of 32768 is used.
	 */
	u32 exit_dump_len;

	/**
	 * @hotplug_seq: A sequence number that may be set by the scheduler to
	 * detect when a hotplug event has occurred during the loading process.
	 * If 0, no detection occurs. Otherwise, the scheduler will fail to
	 * load if the sequence number does not match @scx_hotplug_seq on the
	 * enable path.
	 */
	u64 hotplug_seq;

	/**
	 * @cid_shard_size: Target number of CIDs per shard
	 *
	 * Shards are contiguous CID ranges used as operation and locking
	 * domains for sub-scheduling. Each LLC is divided into ceil(nr_cpus /
	 * @cid_shard_size) shards, then cores are distributed across them
	 * evenly. If one core has more logical CPUs than @cid_shard_size, its
	 * shard will become larger than @cid_shard_size. Values above
	 * SCX_CID_SHARD_MAX_CPUS are capped. 0 means use the default (24).
	 */
	u32 cid_shard_سائز;

	/**
	 * @rescue_bandwidth_ppt: Rescue execution bandwidth in parts per thousand
	 *
	 * The fraction of each CPU's time that may be consumed running tasks
	 * from its rescue DSQ. A higher bandwidth admits and escalates rescues
	 * faster, see @rescue_quantum_us.
	 *
	 * Only the root scheduler's value is used. 0 means the default of 20
	 * (2%). May not exceed 250 (25%). %SCX_RESCUE_DISABLE disables rescue -
	 * %SCX_ENQ_RESCUE inserts are then rejected like any other insert
	 * lacking the caps.
	 */
	u32 rescue_bandwidth_ppt;

	/**
	 * @rescue_quantum_us: Rescue execution quantum in microseconds
	 *
	 * How much CPU time each rescue gets. Rescues run one at a time per CPU
	 * and admissions are paced to keep rescue execution within
	 * @rescue_bandwidth_ppt - with the defaults, one 5ms rescue every
	 * 250ms. A crowded queue round-robins on the quantum divided across the
	 * waiters, floored at 1ms. A stuck rescue eventually escalates to
	 * forced execution. A larger quantum interrupts the CPU less often but
	 * for longer and spaces rescues further apart.
	 *
	 * Only the root scheduler's value is used. 0 means the default (5000).
	 * Non-zero values must be within [1000, 100000]. Values too short for
	 * the kernel to meter are lifted silently.
	 */
	u32 rescue_وقتی_حصہ_us;

	/**
	 * @sub_cgroup_id: When >1, attach the scheduler as a sub-scheduler
	 * on the specified cgroup.
	 */
	u64 sub_cgroup_id;

	/**
	 * @name: BPF scheduler's name
	 *
	 * Must be a non-zero valid BPF object name including only isalnum(),
	 * '_' and '.' chars. Exposed via the ops file in the scheduler's sysfs
	 * directory, /sys/kernel/sched_ext/root/ops for the root scheduler,
	 * while the BPF scheduler is enabled.
	 */
	حرف name[SCX_OPS_NAME_LEN];

	/* internal use only, must be NULL */
	خالی __rcu *priv;

	/*
	 * Deprecated callbacks. Kept at the end of the struct so the cid-form
	 * struct (sched_ext_ops_cid) can omit them without affecting the
	 * shared field offsets. Use SCX_ENQ_IMMED instead. Sitting past
	 * SCX_OPI_END means has_op doesn't cover them, so SCX_HAS_OP() cannot
	 * be used; callers must test sch->ops.cpu_acquire / cpu_release
	 * directly.
	 */

	/**
	 * @cpu_acquire: A CPU is becoming available to the BPF scheduler
	 * @cpu: The CPU being acquired by the BPF scheduler.
	 * @args: Acquire arguments, see the struct definition.
	 *
	 * A CPU that was previously released from the BPF scheduler is now once
	 * again under its control. Deprecated; use SCX_ENQ_IMMED instead.
	 */
	خالی (*cpu_acquire)(s32 cpu, ساخت scx_cpu_acquire_دلائل *دلائل);

	/**
	 * @cpu_release: A CPU is taken away from the BPF scheduler
	 * @cpu: The CPU being released by the BPF scheduler.
	 * @args: Release arguments, see the struct definition.
	 *
	 * The specified CPU is no longer under the control of the BPF
	 * scheduler. This could be because it was preempted by a higher
	 * priority sched_class, though there may be other reasons as well. The
	 * caller should consult @args->reason to determine the cause.
	 * Deprecated; use SCX_ENQ_IMMED instead.
	 */
	خالی (*cpu_release)(s32 cpu, ساخت scx_cpu_release_دلائل *دلائل);
};

/**
 * struct sched_ext_ops_cid - cid-form alternative to struct sched_ext_ops
 *
 * Mirrors struct sched_ext_ops with cpu/cpumask substituted with cid/cmask
 * where applicable. Layout up to and including @priv matches sched_ext_ops
 * byte-for-byte (verified by BUILD_BUG_ON checks at scx_init() time) so
 * shared field offsets work for both struct types in bpf_scx_init_member()
 * and bpf_scx_check_member(). The deprecated cpu_acquire/cpu_release
 * callbacks at the tail of sched_ext_ops are omitted here entirely.
 *
 * Differences from sched_ext_ops:
 *   - select_cpu       -> select_cid (returns cid)
 *   - dispatch         -> dispatch (cpu arg is now cid)
 *   - update_idle      -> update_idle (cpu arg is now cid)
 *   - set_cpumask      -> set_cmask (cmask instead of cpumask)
 *   - enable           -> enable (takes struct scx_enable_args)
 *   - cpu_online       -> cid_online
 *   - cpu_offline      -> cid_offline
 *   - dump_cpu         -> dump_cid
 *   - cgroup_*         -> cpuctl_* (they track the cgroup cpu controller)
 *   - cpu_acquire/cpu_release  -> not present (deprecated in sched_ext_ops)
 *
 * BPF schedulers using this type cannot call cpu-form scx_bpf_* kfuncs;
 * use the cid-form variants instead. Enforced at BPF verifier time via
 * scx_kfunc_context_filter() branching on prog->aux->st_ops.
 *
 * See sched_ext_ops for callback documentation.
 */
ساخت sched_ext_ops_cid {
	s32 (*select_cid)(ساخت task_struct *p, s32 پچھلا_cid, u64 wake_flags);
	خالی (*قطار_میں_ڈالو)(ساخت task_struct *p, u64 enq_flags);
	خالی (*قطار_سے_نکالو)(ساخت task_struct *p, u64 deq_flags);
	خالی (*dispatch)(s32 cid, ساخت task_struct *پچھلا);
	خالی (*tick)(ساخت task_struct *p);
	خالی (*runnable)(ساخت task_struct *p, u64 enq_flags);
	خالی (*running)(ساخت task_struct *p);
	خالی (*stopping)(ساخت task_struct *p, bool runnable);
	خالی (*quiescent)(ساخت task_struct *p, u64 deq_flags);
	bool (*yield)(ساخت task_struct *from, ساخت task_struct *مقصد);
	bool (*core_sched_before)(ساخت task_struct *a,
				   ساخت task_struct *b);
	خالی (*سیٹ_وزن)(ساخت task_struct *p, u32 وزن);
	خالی (*سیٹ_cmask)(ساخت task_struct *p,
			   ثابت ساخت scx_cmask *cmask__arena);
	خالی (*update_idle)(s32 cid, bool idle);
	s32 (*init_task)(ساخت task_struct *p,
			  ساخت scx_init_task_دلائل *دلائل);
	خالی (*exit_task)(ساخت task_struct *p,
			   ساخت scx_exit_task_دلائل *دلائل);
	خالی (*enable)(ساخت task_struct *p, ساخت scx_enable_دلائل *دلائل);
	خالی (*disable)(ساخت task_struct *p);
	خالی (*dump)(ساخت scx_dump_ctx *ctx);
	خالی (*dump_cid)(ساخت scx_dump_ctx *ctx, s32 cid, bool idle);
	خالی (*dump_task)(ساخت scx_dump_ctx *ctx, ساخت task_struct *p);
#اگر_تعریف_شدہ CONFIG_EXT_گروہ_SCHED
	s32 (*cpuctl_init)(ساخت cgroup *cgrp, ساخت scx_cgroup_init_دلائل *دلائل);
	خالی (*cpuctl_exit)(ساخت cgroup *cgrp);
	s32 (*cpuctl_prep_move)(ساخت task_struct *p, ساخت cgroup *from,
				ساخت cgroup *مقصد);
	خالی (*cpuctl_move)(ساخت task_struct *p, ساخت cgroup *from, ساخت cgroup *مقصد);
	خالی (*cpuctl_cancel_move)(ساخت task_struct *p, ساخت cgroup *from,
				   ساخت cgroup *مقصد);
	خالی (*cpuctl_سیٹ_وزن)(ساخت cgroup *cgrp, u32 وزن);
	خالی (*cpuctl_سیٹ_bandwidth)(ساخت cgroup *cgrp, u64 period_us, u64 quota_us,
				     u64 مدت_us);
	خالی (*cpuctl_سیٹ_idle)(ساخت cgroup *cgrp, bool idle);
#اگر_ختم	/* CONFIG_EXT_GROUP_SCHED */
	s32 (*sub_attach)(ساخت scx_sub_attach_دلائل *دلائل);
	خالی (*sub_detach)(ساخت scx_sub_detach_دلائل *دلائل);
	خالی (*sub_caps_updated)(ثابت ساخت scx_cmask *cmask__arena, u64 caps);
	خالی (*sub_ecaps_updated)(s32 cid, u64 before, u64 after);
	خالی (*cid_online)(s32 cid);
	خالی (*cid_offline)(s32 cid);
	s32 (*init_cids)(خالی);
	s32 (*init)(خالی);
	خالی (*باہر_نکلو)(ساخت scx_exit_info *info);

	/* Data fields - must match sched_ext_ops layout exactly */
	u32 dispatch_max_batch;
	u64 flags;
	u32 timeout_ms;
	u32 exit_dump_len;
	u64 hotplug_seq;
	u32 cid_shard_سائز;
	u32 rescue_bandwidth_ppt;
	u32 rescue_وقتی_حصہ_us;
	u64 sub_cgroup_id;
	حرف name[SCX_OPS_NAME_LEN];

	/* internal use only, must be NULL */
	خالی __rcu *priv;

	/* layout end anchor for the BUILD_BUG_ON in scx_init(); keep last */
	حرف __end[0];
};

شمار scx_opi {
	SCX_OPI_BEGIN			= 0,
	SCX_OPI_معمول_BEGIN		= 0,
	SCX_OPI_معمول_انتہا		= SCX_OP_IDX(cpu_online),
	SCX_OPI_CPU_HOTPLUG_BEGIN	= SCX_OP_IDX(cpu_online),
	SCX_OPI_CPU_HOTPLUG_انتہا		= SCX_OP_IDX(init_cids),
	SCX_OPI_انتہا			= SCX_OP_IDX(init_cids),
};

/*
 * Collection of event counters. Event types are placed in descending order.
 */
ساخت scx_event_stats {
	/*
	 * If ops.select_cpu() returns a CPU which can't be used by the task,
	 * the core scheduler code silently picks a fallback CPU.
	 */
	s64		SCX_EV_SELECT_CPU_FALLBACK;

	/*
	 * When dispatching to a local DSQ, the CPU may have gone offline in
	 * the meantime. In this case, the task is bounced to the global DSQ.
	 */
	s64		SCX_EV_DISPATCH_LOCAL_DSQ_OFFLINE;

	/*
	 * If SCX_OPS_ENQ_LAST is not set, the number of times that a task
	 * continued to run because there were no other tasks on the CPU.
	 */
	s64		SCX_EV_DISPATCH_KEEP_آخری;

	/*
	 * If SCX_OPS_ENQ_EXITING is not set, the number of times that a task
	 * is dispatched to a local DSQ when exiting.
	 */
	s64		SCX_EV_ENQ_SKIP_EXITING;

	/*
	 * If SCX_OPS_ENQ_MIGRATION_DISABLED is not set, the number of times a
	 * migration disabled task skips ops.enqueue() and is dispatched to its
	 * local DSQ.
	 */
	s64		SCX_EV_ENQ_SKIP_MIGRATION_DISABLED;

	/*
	 * The number of times a task, enqueued on a local DSQ with
	 * SCX_ENQ_IMMED, was re-enqueued because the CPU was not available for
	 * immediate execution.
	 */
	s64		SCX_EV_REENQ_IMMED;

	/*
	 * The number of times a reenqueue (%SCX_ENQ_REENQ) led to another
	 * reenqueue without the task running in between. This count climbing
	 * rapidly indicates that the BPF scheduler keeps re-deciding placements
	 * it can't honor. A single task reenqueued more than
	 * %SCX_REENQ_MAX_REPEAT times gets its owning scheduler ejected.
	 */
	s64		SCX_EV_REENQ_REPEAT;

	/*
	 * Total number of times a task's time slice was refilled with the
	 * default value (SCX_SLICE_DFL).
	 */
	s64		SCX_EV_REFILL_قاش_DFL;

	/*
	 * The number of times an out-of-band slice request exceeded the maximum
	 * representable value and was clamped.
	 */
	s64		SCX_EV_قاش_CLAMPED;

	/*
	 * The number of times a slice extension was denied because the
	 * scheduler lacked baseline cpu access on the task's cpu.
	 */
	s64		SCX_EV_قاش_DENIED;

	/*
	 * The total duration of bypass modes in nanoseconds.
	 */
	s64		SCX_EV_BYPASS_DURATION;

	/*
	 * The number of tasks dispatched in the bypassing mode.
	 */
	s64		SCX_EV_BYPASS_DISPATCH;

	/*
	 * The number of times the bypassing mode has been activated.
	 */
	s64		SCX_EV_BYPASS_ACTIVATE;

	/*
	 * The number of times the scheduler attempted to insert a task that it
	 * doesn't own into a DSQ. Such attempts are ignored.
	 *
	 * As BPF schedulers are allowed to ignore dequeues, it's difficult to
	 * tell whether such an attempt is from a scheduler malfunction or an
	 * ignored dequeue around sub-sched enabling. If this count keeps going
	 * up regardless of sub-sched enabling, it likely indicates a bug in the
	 * scheduler.
	 */
	s64		SCX_EV_داخل_کرو_NOT_OWNED;

	/*
	 * The number of times tasks from bypassing descendants are scheduled
	 * from sub_bypass_dsq's.
	 */
	s64		SCX_EV_SUB_BYPASS_DISPATCH;

	/*
	 * The number of times a migration-disabled task lacking the cap for its
	 * cid was allowed onto the local DSQ. It must run on its pinned CPU, so
	 * it can't be rejected. The violation is counted here.
	 */
	s64		SCX_EV_SUB_FORCED_ADMIT;

	/*
	 * The number of times a preempting kick was refused because the
	 * sub-sched lacked SCX_CAP_PREEMPT for a task outside its subtree. The
	 * kick degrades to a plain reschedule.
	 */
	s64		SCX_EV_SUB_PREEMPT_DENIED;

	/*
	 * The number of times a kick was skipped because the sub-sched lacked
	 * baseline access on the target cid. The preempt-part degradation of a
	 * delivered kick is counted in SCX_EV_SUB_PREEMPT_DENIED instead.
	 */
	s64		SCX_EV_SUB_KICK_DENIED;

	/*
	 * The number of times a local DSQ reenq was dropped because the
	 * sub-sched lacked baseline access on the target cid.
	 */
	s64		SCX_EV_SUB_REENQ_DENIED;

	/*
	 * The number of times scx_bpf_cidperf_set() was denied because the
	 * sub-sched lacked SCX_CAP_PERF on the target cid.
	 */
	s64		SCX_EV_SUB_CIDPERF_DENIED;

	/*
	 * The number of times an insert carrying %SCX_ENQ_RESCUE lacked the
	 * caps for its cid and the task entered the rescue path.
	 */
	s64		SCX_EV_SUB_RESCUE;
};

#تعریف_کرو SCX_EVENTS_LIST(SCX_EVENT)					\
	SCX_EVENT(SCX_EV_SELECT_CPU_FALLBACK);				\
	SCX_EVENT(SCX_EV_DISPATCH_LOCAL_DSQ_OFFLINE);			\
	SCX_EVENT(SCX_EV_DISPATCH_KEEP_آخری);				\
	SCX_EVENT(SCX_EV_ENQ_SKIP_EXITING);				\
	SCX_EVENT(SCX_EV_ENQ_SKIP_MIGRATION_DISABLED);			\
	SCX_EVENT(SCX_EV_REENQ_IMMED);					\
	SCX_EVENT(SCX_EV_REENQ_REPEAT);					\
	SCX_EVENT(SCX_EV_REFILL_قاش_DFL);				\
	SCX_EVENT(SCX_EV_قاش_CLAMPED);				\
	SCX_EVENT(SCX_EV_قاش_DENIED);					\
	SCX_EVENT(SCX_EV_BYPASS_DURATION);				\
	SCX_EVENT(SCX_EV_BYPASS_DISPATCH);				\
	SCX_EVENT(SCX_EV_BYPASS_ACTIVATE);				\
	SCX_EVENT(SCX_EV_داخل_کرو_NOT_OWNED);				\
	SCX_EVENT(SCX_EV_SUB_BYPASS_DISPATCH);				\
	SCX_EVENT(SCX_EV_SUB_FORCED_ADMIT);				\
	SCX_EVENT(SCX_EV_SUB_PREEMPT_DENIED);				\
	SCX_EVENT(SCX_EV_SUB_KICK_DENIED);				\
	SCX_EVENT(SCX_EV_SUB_REENQ_DENIED);				\
	SCX_EVENT(SCX_EV_SUB_CIDPERF_DENIED);				\
	SCX_EVENT(SCX_EV_SUB_RESCUE)

ساخت scx_sched;

شمار scx_sched_pcpu_flags {
	SCX_SCHED_PCPU_BYPASSING	= 1LLU << 0,
};

/* dispatch buf */
ساخت scx_dsp_buf_ent {
	ساخت task_struct	*task;
	غیرعلامتی طویل		qseq;
	u64			dsq_id;
	u64			قاش;
	u64			vtime;
	u64			enq_flags;
};

ساخت scx_dsp_ctx {
	ساخت rq		*rq;
	u32			cursor;
	u32			nr_tasks;
	ساخت scx_dsp_buf_ent	buf[];
};

ساخت scx_deferred_reenq_local {
	ساخت list_سر	گرہ;
	u64			flags;
};

ساخت scx_sched_pcpu {
	ساخت scx_sched	*sch;
	u64			flags;	/* protected by rq lock */

	/*
	 * Kick state owned by this cpu for this sched. scx_kick_cpu() records
	 * targets here and links @to_kick_node onto the cpu's
	 * rq->scx.sched_pcpus_to_kick. The cpu's single kick irq_work walks
	 * that list and kicks each sched's targets on its behalf. Per-sched so
	 * a kick stays attributed to its scheduler.
	 */
	cpumask_var_t		cpus_مقصد_kick;
	cpumask_var_t		cpus_مقصد_kick_if_idle;
	cpumask_var_t		cpus_مقصد_preempt;
	cpumask_var_t		cpus_مقصد_wait;
	ساخت list_سر	مقصد_kick_گرہ;

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	/*
	 * pshard->caps[cap_bit] is the set of cids the sched holds that one
	 * cap on. ecaps is its transpose: the set of SCX_CAP_* bits the sched
	 * effectively holds on this cpu, with implied caps folded in, so that
	 * the hot-path check is a single read.
	 *
	 * While pshard->caps[] under pshard->lock is the target configuration,
	 * ecaps is the effective copy owned by the cpu. It is written under the
	 * rq lock while processing rq->ecaps_to_sync. Can also be read with
	 * READ_ONCE() outside rq lock.
	 *
	 * See queue_sync_ecaps() and scx_process_sync_ecaps().
	 */
	u64			ecaps;
	ساخت llist_گرہ	ecaps_مقصد_sync_گرہ;
	/* owed a forced update_idle() re-notify on this cpu */
	bool			idle_renotify;
	/* effective caps as of the last sub_ecaps_updated() delivery */
	u64			reported_ecaps;

	/*
	 * Decaying rescue runtime consumed on this cpu, see
	 * scx_rescue_decay_avg(). Overload on this cpu ejects the sub with the
	 * largest value. Accessed only under this cpu's rq lock.
	 */
	u64			rescue_avg;
	u64			rescue_avg_at;	/* last decay, jiffies_64 */
#اگر_ختم

	/*
	 * The event counters are in a per-CPU variable to minimize the
	 * accounting overhead. A system-wide view on the event counter is
	 * constructed when requested by scx_bpf_events().
	 */
	ساخت scx_event_stats	event_stats;

	ساخت scx_deferred_reenq_local deferred_reenq_local;
	ساخت scx_dispatch_q	bypass_dsq;
#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	u32			bypass_host_seq;
#اگر_ختم

	/* must be the last entry - contains flex array */
	ساخت scx_dsp_ctx	dsp_ctx;
};

ساخت scx_sched_pnode {
	ساخت scx_dispatch_q	global_dsq;
};

/*
 * Sub-sched capability delegation.
 *
 * Caps are per-cid permissions parents delegate to direct children via
 * scx_bpf_sub_grant() / scx_bpf_sub_revoke(). A child's cap set is always a
 * subset of its parent's. A sub-sched checks its caps locally, and cross-sched
 * communication is needed only when the delegation set itself changes.
 *
 * Caps are used to implement sub-sched scheduling on the enqueue path. Picking
 * a cid for a task at a leaf depends on which cids the leaf is allowed to use.
 * Resolving that programmatically on every enqueue would mean a cross-sched
 * round-trip call chain, possibly retrying if the request can't be granted
 * as-is.
 *
 * The dispatch path is different - it runs as top-down recursion via
 * scx_bpf_sub_dispatch(): a sched's dispatch op invokes a child's dispatch op
 * on the local rq, and the subtree dispatches in a single pass.
 *
 * Locking is per shard. cid space is split into shards, and each sub-sched has
 * its own pshard->lock for each shard. Operations are broken up on shard
 * boundaries. Different shards never contend. Shards are expected to be
 * topology-aligned and likely to serve as the locality unit when cids are
 * allocated to schedulers, so per-shard lock granularity scales naturally with
 * the allocation pattern.
 *
 * ENQ_IMMED  insert an IMMED task onto the cid's local DSQ
 *            - kick the cid's cpu (except SCX_KICK_PREEMPT)
 *
 * ENQ        insert any task onto the cid's local DSQ (implies ENQ_IMMED)
 *
 * PREEMPT    preempt any task running on the cid regardless of the owning
 *            sched (implies ENQ). Preempting a task in the sched's own subtree
 *            doesn't require any cap.
 *            - SCX_ENQ_PREEMPT inserts
 *            - SCX_KICK_PREEMPT kicks
 *
 * PERF       control the cid's cpu power/perf management state, currently the
 *            cpufreq target set through scx_bpf_cidperf_set(). Hardware
 *            control is a separate axis from queue access: PERF neither
 *            implies nor is implied by the caps above.
 *
 * Implied caps apply to the holder's own use of a cid, not to delegation.
 * scx_bpf_sub_grant() delegates literally-held caps, so a cap held only through
 * implication is usable but cannot be re-delegated to a child. When granting a
 * cap, it usually makes sense to delegate its implied caps explicitly alongside
 * it.
 */
شمار scx_cap_flags {
	__SCX_CAP_ENQ_IMMED		= 0,
	__SCX_CAP_ENQ			= 1,
	__SCX_CAP_PREEMPT		= 2,
	__SCX_CAP_PERF			= 3,

	__SCX_NR_CAPS,
	__SCX_CAP_ALL			= ثنائی_ہندسہ_U64(__SCX_NR_CAPS) - 1,

	SCX_CAP_ENQ_IMMED		= ثنائی_ہندسہ_U64(__SCX_CAP_ENQ_IMMED),
	SCX_CAP_ENQ			= ثنائی_ہندسہ_U64(__SCX_CAP_ENQ),
	SCX_CAP_PREEMPT			= ثنائی_ہندسہ_U64(__SCX_CAP_PREEMPT),
	SCX_CAP_PERF			= ثنائی_ہندسہ_U64(__SCX_CAP_PERF),

	/* alias for minimal cap to make any use of a cpu */
	SCX_CAP_بنیاد			= SCX_CAP_ENQ_IMMED,

	/* caps whose loss strands queued tasks, see scx_process_sync_ecaps() */
	SCX_CAPS_REENQ_ON_LOSS		= SCX_CAP_ENQ_IMMED | SCX_CAP_ENQ,
};

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
/* iterate set bits in a u64 cap mask */
#تعریف_کرو scx_for_each_cap_ثنائی_ہندسہ(cap_ثنائی_ہندسہ, caps)				\
	برائے (u64 __caps = (caps);					\
	     __caps && ((cap_ثنائی_ہندسہ) = __ffs64(__caps), true);		\
	     __caps &= __caps - 1)

/*
 * Sub-cap update notifier.
 *
 * ops_cid.sub_caps_updated() notifies sub-scheds when their cap state changes
 * so they can refresh internal state without polling scx_bpf_sub_caps() per
 * enqueue.
 *
 * Three constraints shape the design:
 *
 *   1. Static memory. Deliveries use a fixed-size buffer, both for runtime
 *      efficiency and so notifications can't be lost under memory pressure.
 *
 *   2. High-frequency updates. Grant/revoke can mutate caps in bursts, and the
 *      notifier path must absorb that without amplifying it.
 *
 *   3. Recursive grant/revoke from the callback. A child receiving a
 *      notification can call grant/revoke on its own children, which can
 *      cascade recursively down its subtree.
 *
 * (1) and (2) lead to coalescing into a fixed payload. Each delivery carries a
 * single (cmask, caps) pair covering every change since the previous one.
 * Direction (set vs cleared) isn't encoded as it doesn't fit in the fixed-size
 * summary. The callback queries scx_bpf_sub_caps() for current state. Only one
 * delivery is in flight per shard. Further changes fold into the same buffer
 * and ship as the next callback, so a shard's callbacks fire in order.
 *
 * (3) leads to deferred delivery. Events accumulate during grant/revoke and are
 * delivered after the shard lock is released.
 */
ساخت scx_caps_updated {
	raw_spinlock_t		تالا;
	u64			caps;
	ساخت scx_cmask	*cmask_arena_برآمد;
	ساخت list_سر	گرہ_اندر_flight;
	/* Kernel-side accumulator. Access as &cu->cmask. */
	TRAILING_OVERLAP(ساخت scx_cmask, cmask, بٹ,
			 u64 بٹ_2[SCX_CMASK_NR_الفاظ(SCX_CID_SHARD_MAX_CPUS)];
	);
};

ساخت scx_pshard {
	raw_spinlock_t		تالا;		/* serializes caps */
	ساخت scx_sched	*sch;		/* backpointer */
	ساخت scx_caps_updated	caps_updated;

	/*
	 * Per-cap cmask, inline via TRAILING_OVERLAP so cmask.bits[] overlaps
	 * the trailing _bits[] storage. Access as &caps[i].cmask. See
	 * scx_sched_pcpu->ecaps.
	 */
	TRAILING_OVERLAP(ساخت scx_cmask, cmask, بٹ,
			 u64 بٹ_2[SCX_CMASK_NR_الفاظ(SCX_CID_SHARD_MAX_CPUS)];
	) caps[__SCX_NR_CAPS];

	/*
	 * Shard geometry captured at alloc. cmask_arena_out's own header is
	 * bpf-writable and the live shard range can change before the
	 * rcu-deferred free, so re-init and size cmask_arena_out from these
	 * trusted copies instead.
	 */
	u32			بنیاد;
	u32			nr_cids;
};
#اگر_ختم

ساخت scx_sched {
	/*
	 * cpu-form and cid-form ops share field offsets up to .priv (verified
	 * by BUILD_BUG_ON in scx_init()). The anonymous union lets the kernel
	 * access either view of the same storage without function-pointer
	 * casts: use .ops for cpu-form and shared fields, .ops_cid for the
	 * callbacks whose cid-form signature differs (set_cmask, enable,
	 * select_cid, cid_online, ...).
	 */
	اتحاد {
		ساخت sched_ext_ops		ops;
		ساخت sched_ext_ops_cid	ops_cid;
	};
	bool			is_cid_type;	/* true if registered via bpf_sched_ext_ops_cid */
	bool			dead;		/* set after ops.exit(), gates scx_prog_sched() */

	/*
	 * Arena map auto-discovered from member progs at struct_ops attach.
	 * cid-form schedulers must use exactly one arena across all member
	 * progs. NULL on cpu-form.
	 *
	 * @arena_pool sub-allocates @arena_map. Each gen_pool chunk is added
	 * at the kernel-side mapping address. @arena_kern_base is the start
	 * of the arena's kern_vm range. See scx_arena_to_kaddr().
	 */
	ساخت bpf_map		*arena_map;
	ساخت gen_pool		*arena_pool;
	uintptr_t		arena_kern_بنیاد;

	/*
	 * Per-CPU arena cmask the kernel fills from a task's cpumask and hands
	 * to ops_cid.enable() and ops_cid.set_cmask(). The stored pointers are
	 * the kernel addresses.
	 */
	ساخت scx_cmask * __percpu *سیٹ_cmask_scratch;
	ساخت scx_cmask *online_cmask;

	DECLARE_BITMAP(has_op, SCX_OPI_انتہا);

	/*
	 * Dispatch queues.
	 *
	 * The global DSQ (%SCX_DSQ_GLOBAL) is split per-node for scalability.
	 * This is to avoid live-locking in bypass mode where all tasks are
	 * dispatched to %SCX_DSQ_GLOBAL and all CPUs consume from it. If
	 * per-node split isn't sufficient, it can be further split.
	 */
	ساخت rhashtable	dsq_hash;
	ساخت scx_sched_pnode	**pnode;
#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	ساخت scx_pshard	**pshard;	/* indexed by shard_idx */
#اگر_ختم
	ساخت scx_sched_pcpu __percpu *pcpu;

	u64			قاش_dfl;
	u64			bypass_timestamp;
	s32			bypass_گہرائی;

	/* bypass dispatch path enable state, see scx_bypass_dsp_enabled() */
	غیرعلامتی طویل		bypass_dsp_claim;
	atomic_t		bypass_dsp_enable_گہرائی;

	bool			aborting;
	bool			dump_disabled;	/* protected by scx_dump_lock */
	u32			dsp_max_batch;
	s32			level;

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	/*
	 * pshard[] size captured at enable for the async RCU free path -
	 * scx_nr_cid_shards may be rewritten by a later enable's
	 * scx_cid_publish_tables() before free runs. While sch is active, use
	 * the global.
	 */
	u32			nr_pshards;
#اگر_ختم

	/*
	 * Updates to the following warned bitfields can race causing RMW issues
	 * but it doesn't really matter.
	 */
	bool			warned_zero_قاش:1;
	bool			warned_unassoc_progs:1;

	ساخت list_سر	all;

	/* unique instance id, monotonic and never reused */
	u64			id;

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	ساخت rhash_سر	hash_گرہ;

	ساخت list_سر	children;
	ساخت list_سر	sibling;
	ساخت cgroup		*cgrp;
	حرف			*cgrp_راستہ;
	ساخت kset		*sub_kset;

	bool			linked;		/* on ->children, see scx_link_sched() */
	bool			sub_attached;
#اگر_ختم	/* CONFIG_EXT_SUB_SCHED */

	/*
	 * The maximum amount of time in jiffies that a task may be runnable
	 * without being scheduled on a CPU. If this timeout is exceeded, it
	 * will trigger scx_error().
	 */
	غیرعلامتی طویل		watchdog_timeout;

	atomic_t		exit_قسم;
	ساخت scx_exit_info	*exit_info;

	ساخت kobject		kobj;

	ساخت kthread_کارکن	*helper;
	ساخت irq_work		disable_irq_work;
	ساخت kthread_work	disable_work;
	ساخت irq_work		propagate_exit_irq_work; /* see scx_claim_exit() */
	ساخت timer_list	bypass_lb_timer;
	cpumask_var_t		bypass_lb_donee_cpumask;
	cpumask_var_t		bypass_lb_resched_cpumask;
	cpumask_var_t		stall_cpus;
	ساخت rcu_work		rcu_work;

	/* all ancestors including self */
	ساخت scx_sched	*ancestors[];
};

/**
 * scx_arena_to_kaddr - Translate a BPF-arena pointer to its kernel address
 * @sch: scheduler whose arena hosts @bpf_ptr
 * @bpf_ptr: BPF-arena pointer, only the low 32 bits are used
 *
 * The (u32) cast normalizes any input into the arena's 4 GiB kern_vm range,
 * which combined with scratch-page fault recovery makes the returned pointer
 * safe to dereference up to GUARD_SZ / 2 past the intended object. Accesses
 * larger than GUARD_SZ / 2 must be explicitly bounds-checked.
 */
ساکن اندرونی خالی *scx_arena_مقصد_kaddr(ساخت scx_sched *sch, ثابت خالی *bpf_ptr)
{
	واپس (خالی *)(sch->arena_kern_بنیاد + (u32)(uintptr_t)bpf_ptr);
}

/**
 * scx_kaddr_to_arena - Translate a kernel arena address to the BPF form
 * @sch: scheduler whose arena hosts @kaddr
 * @kaddr: kernel address inside @sch's arena
 *
 * __arena callback arguments need no translation. Addresses handed to BPF any
 * other way, such as struct fields and kfunc return values, go through this.
 */
ساکن اندرونی uintptr_t scx_kaddr_مقصد_arena(ساخت scx_sched *sch, ثابت خالی *kaddr)
{
	واپس (uintptr_t)kaddr - sch->arena_kern_بنیاد;
}

شمار scx_wake_flags {
	/* expose select WF_* flags as enums */
	SCX_WAKE_FORK		= WF_FORK,
	SCX_WAKE_TTWU		= WF_TTWU,
	SCX_WAKE_SYNC		= WF_SYNC,
};

شمار scx_enq_flags {
	/* expose select ENQUEUE_* flags as enums */
	SCX_ENQ_WAKEUP		= قطار_میں_ڈالو_WAKEUP,
	SCX_ENQ_سر		= قطار_میں_ڈالو_سر,
	SCX_ENQ_CPU_SELECTED	= قطار_میں_ڈالو_RQ_SELECTED,

	/* high 32bits are SCX specific */

	/*
	 * Set the following to trigger preemption when calling
	 * scx_bpf_dsq_insert() with a local dsq as the target. The slice of the
	 * current task is cleared to zero and the CPU is kicked into the
	 * scheduling path. Implies %SCX_ENQ_HEAD.
	 */
	SCX_ENQ_PREEMPT		= 1LLU << 32,

	/*
	 * Only allowed on local DSQs. Guarantees that the task either gets
	 * on the CPU immediately and stays on it, or gets reenqueued back
	 * to the BPF scheduler. It will never linger on a local DSQ or be
	 * silently put back after preemption.
	 *
	 * The protection persists until the next fresh enqueue - it
	 * survives SAVE/RESTORE cycles, slice extensions and preemption.
	 * If the task can't stay on the CPU for any reason, it gets
	 * reenqueued back to the BPF scheduler.
	 *
	 * Exiting and migration-disabled tasks bypass ops.enqueue() and
	 * are placed directly on a local DSQ without IMMED protection
	 * unless %SCX_OPS_ENQ_EXITING and %SCX_OPS_ENQ_MIGRATION_DISABLED
	 * are set respectively.
	 */
	SCX_ENQ_IMMED		= 1LLU << 33,

	/*
	 * Only allowed on local DSQs. If the insert lacks the caps for the
	 * target cid, divert the task to the CPU's rescue path instead of
	 * rejecting and reenqueueing, e.g. when the task's affinity is
	 * restricted to cids the scheduler doesn't hold. The kernel runs
	 * rescued tasks on the target CPU. Rescue execution is guaranteed to
	 * make forward progress and is bandwidth-limited, see the
	 * rescue_bandwidth_ppt and rescue_quantum_us ops fields.
	 */
	SCX_ENQ_RESCUE		= 1LLU << 34,

	/*
	 * The task being enqueued was previously enqueued on a DSQ, but was
	 * removed and is being re-enqueued. See SCX_TASK_REENQ_* flags to find
	 * out why a given task is being reenqueued.
	 */
	SCX_ENQ_REENQ		= 1LLU << 40,

	/*
	 * The task being enqueued is the only task available for the cpu. By
	 * default, ext core keeps executing such tasks but when
	 * %SCX_OPS_ENQ_LAST is specified, they're ops.enqueue()'d with the
	 * %SCX_ENQ_LAST flag set.
	 *
	 * If the task is queued on the local DSQ of the CPU it was running on,
	 * it continues to run. Otherwise, the CPU goes idle. A scheduler that
	 * wants a full dispatch cycle on the CPU should kick it.
	 */
	SCX_ENQ_آخری		= 1LLU << 41,

	/* high 8 bits are internal */
	__SCX_ENQ_INTERNAL_MASK	= 0xffLLU << 56,

	SCX_ENQ_CLEAR_OPSS	= 1LLU << 56,
	SCX_ENQ_DSQ_PRIQ	= 1LLU << 57,
	SCX_ENQ_NESTED		= 1LLU << 58,
	SCX_ENQ_GDSQ_FALLBACK	= 1LLU << 59,	/* fell back to global DSQ */
	SCX_ENQ_IGNORE_CAPS	= 1LLU << 60,	/* admit to local DSQ ignoring caps */
	SCX_ENQ_APPLY_قاش	= 1LLU << 61,	/* apply carried slice/vtime at insertion */
	SCX_ENQ_قاش_DFL	= 1LLU << 62,	/* carried slice is a default refill */
};

شمار scx_deq_flags {
	/* expose select DEQUEUE_* flags as enums */
	SCX_DEQ_SLEEP		= قطار_سے_نکالو_SLEEP,

	/* high 32bits are SCX specific */

	/*
	 * The generic core-sched layer decided to execute the task even though
	 * it hasn't been dispatched yet. Dequeue from the BPF side.
	 */
	SCX_DEQ_CORE_SCHED_EXEC	= 1LLU << 32,

	/*
	 * The task is being dequeued due to a property change (e.g.,
	 * sched_setaffinity(), sched_setscheduler(), set_user_nice(),
	 * etc.).
	 */
	SCX_DEQ_SCHED_CHANGE	= 1LLU << 33,
};

شمار scx_reenq_flags {
	/* low 16bits determine which tasks should be reenqueued */
	SCX_REENQ_ANY		= 1LLU << 0,	/* all tasks */

	/* internal: kernel-issued on cap revoke, not accepted from BPF */
	SCX_REENQ_CAP_REVOKE	= 1LLU << 1,

	__SCX_REENQ_FILTER_MASK	= 0xffffLLU,

	__SCX_REENQ_USER_MASK	= SCX_REENQ_ANY,

	/* bits 32-35 used by task_should_reenq() */
	SCX_REENQ_TSR_RQ_OPEN	= 1LLU << 32,
	SCX_REENQ_TSR_NOT_پہلا	= 1LLU << 33,

	__SCX_REENQ_TSR_MASK	= 0xfLLU << 32,
};

شمار scx_pick_idle_cpu_flags {
	SCX_PICK_IDLE_CORE	= 1LLU << 0,	/* pick a CPU whose SMT siblings are also idle */
	SCX_PICK_IDLE_اندر_گرہ	= 1LLU << 1,	/* pick a CPU in the same target NUMA node */
};

شمار scx_kick_flags {
	/*
	 * Kick the target CPU if idle. Guarantees that the target CPU goes
	 * through at least one full scheduling cycle before going idle. If the
	 * target CPU can be determined to be currently not idle and going to go
	 * through a scheduling cycle before going idle, noop.
	 */
	SCX_KICK_IDLE		= 1LLU << 0,

	/*
	 * Preempt the current task and execute the dispatch path. If the
	 * current task of the target CPU is an SCX task, its ->scx.slice is
	 * cleared to zero before the scheduling path is invoked so that the
	 * task expires and the dispatch path is invoked.
	 */
	SCX_KICK_PREEMPT	= 1LLU << 1,

	/*
	 * The scx_bpf_kick_cpu() call will return after the current SCX task of
	 * the target CPU switches out. This can be used to implement e.g. core
	 * scheduling. This has no effect if the current task on the target CPU
	 * is not on SCX.
	 */
	SCX_KICK_WAIT		= 1LLU << 2,
};

شمار scx_tg_flags {
	SCX_TG_ONLINE		= 1U << 0,
	SCX_TG_INITED		= 1U << 1,
	SCX_TG_SUB_INIT		= 1U << 2,	/* see scx_cgroup_claim_subtree() */
};

شمار scx_enable_state {
	SCX_ENABLING,
	SCX_ENABLED,
	SCX_DISABLING,
	SCX_DISABLED,
};

ساکن ثابت حرف *scx_enable_state_str[] = {
	[SCX_ENABLING]		= "enabling",
	[SCX_ENABLED]		= "enabled",
	[SCX_DISABLING]		= "disabling",
	[SCX_DISABLED]		= "disabled",
};

/*
 * Task Ownership State Machine (sched_ext_entity->ops_state)
 *
 * The sched_ext core uses this state machine to track task ownership
 * between the SCX core and the BPF scheduler. This allows the BPF
 * scheduler to dispatch tasks without strict ordering requirements, while
 * the SCX core safely rejects invalid dispatches.
 *
 * State Transitions
 *
 *       .------------> NONE (owned by SCX core)
 *       |               |           ^
 *       |       enqueue |           | direct dispatch
 *       |               v           |
 *       |           QUEUEING -------'
 *       |               |
 *       |       enqueue |
 *       |     completes |
 *       |               v
 *       |            QUEUED (owned by BPF scheduler)
 *       |               |
 *       |      dispatch |
 *       |               |
 *       |               v
 *       |          DISPATCHING
 *       |               |
 *       |      dispatch |
 *       |     completes |
 *       `---------------'
 *
 * State Descriptions
 *
 * - %SCX_OPSS_NONE:
 *     Task is owned by the SCX core. It's either on a run queue, running,
 *     or being manipulated by the core scheduler. The BPF scheduler has no
 *     claim on this task.
 *
 * - %SCX_OPSS_QUEUEING:
 *     Transitional state while transferring a task from the SCX core to
 *     the BPF scheduler. The task's rq lock is held during this state.
 *     Since QUEUEING is both entered and exited under the rq lock, dequeue
 *     can never observe this state (it would be a BUG). When finishing a
 *     dispatch, if the task is still in %SCX_OPSS_QUEUEING the completion
 *     path busy-waits for it to leave this state (via wait_ops_state())
 *     before retrying.
 *
 * - %SCX_OPSS_QUEUED:
 *     Task is owned by the BPF scheduler. It's on a DSQ (dispatch queue)
 *     and the BPF scheduler is responsible for dispatching it. A QSEQ
 *     (queue sequence number) is embedded in this state to detect
 *     dispatch/dequeue races: if a task is dequeued and re-enqueued, the
 *     QSEQ changes and any in-flight dispatch operations targeting the old
 *     QSEQ are safely ignored.
 *
 * - %SCX_OPSS_DISPATCHING:
 *     Transitional state while transferring a task from the BPF scheduler
 *     back to the SCX core. This state indicates the BPF scheduler has
 *     selected the task for execution. When dequeue needs to take the task
 *     off a DSQ and it is still in %SCX_OPSS_DISPATCHING, the dequeue path
 *     busy-waits for it to leave this state (via wait_ops_state()) before
 *     proceeding. Exits to %SCX_OPSS_NONE when dispatch completes.
 *
 * Memory Ordering
 *
 * Transitions out of %SCX_OPSS_QUEUEING and %SCX_OPSS_DISPATCHING into
 * %SCX_OPSS_NONE or %SCX_OPSS_QUEUED must use atomic_long_set_release()
 * and waiters must use atomic_long_read_acquire(). This ensures proper
 * synchronization between concurrent operations.
 *
 * Cross-CPU Task Migration
 *
 * When moving a task in the %SCX_OPSS_DISPATCHING state, we can't simply
 * grab the target CPU's rq lock because a concurrent dequeue might be
 * waiting on %SCX_OPSS_DISPATCHING while holding the source rq lock
 * (deadlock).
 *
 * The sched_ext core uses a "lock dancing" protocol coordinated by
 * p->scx.holding_cpu. When moving a task to a different rq:
 *
 *   1. Set p->scx.holding_cpu to the current CPU
 *   2. Set task state to %SCX_OPSS_NONE; dequeue waits while DISPATCHING
 *      is set, so clearing DISPATCHING first prevents the circular wait
 *      (safe to lock the rq we need)
 *   3. Unlock the current CPU's rq
 *   4. Lock src_rq (where the task currently lives)
 *   5. Verify p->scx.holding_cpu == current CPU, if not, dequeue won the
 *      race (dequeue clears holding_cpu to -1 when it takes the task), in
 *      this case migration is aborted
 *   6. If src_rq == dst_rq: clear holding_cpu and enqueue directly
 *      into dst_rq's local DSQ (no lock swap needed)
 *   7. Otherwise, verify under src_rq lock that the task can be moved to dst_rq
 *      (CPU affinity, migration_disabled, etc.). If not, clear holding_cpu,
 *      leave the task on src_rq, and enqueue it on the fallback DSQ.
 *   8. Otherwise (i.e. if the task can be moved to dst_rq), call
 *      move_remote_task_to_local_dsq(), which releases src_rq, locks dst_rq,
 *      and performs the deactivate/activate migration cycle
 *      (dst_rq is held on return)
 *   9. Unlock dst_rq and re-lock the current CPU's rq to restore
 *      the lock state expected by the caller
 *
 * If any verification fails, abort the migration.
 *
 * This state tracking allows the BPF scheduler to try to dispatch any task
 * at any time regardless of its state. The SCX core can safely
 * reject/ignore invalid dispatches, simplifying the BPF scheduler
 * implementation.
 */
شمار scx_ops_state {
	SCX_OPSS_NONE,		/* owned by the SCX core */
	SCX_OPSS_QUEUEING,	/* in transit to the BPF scheduler */
	SCX_OPSS_QUEUED,	/* owned by the BPF scheduler */
	SCX_OPSS_DISPATCHING,	/* in transit back to the SCX core */

	/*
	 * QSEQ brands each QUEUED instance so that, when dispatch races
	 * dequeue/requeue, the dispatcher can tell whether it still has a claim
	 * on the task being dispatched.
	 *
	 * QSEQ is generated from the per-task p->scx.ops_qseq counter so that
	 * it doesn't repeat across QUEUED instances of the same task even if
	 * the task moves between rqs. 0 is never used as a valid QSEQ since
	 * NONE and DISPATCHING map to this value.
	 *
	 * As some 32bit archs can't do 64bit store_release/load_acquire,
	 * p->scx.ops_state is atomic_long_t which leaves 30 bits for QSEQ on
	 * 32bit machines. The dispatch race window QSEQ protects is very narrow
	 * and runs with IRQ disabled. 30 bits should be sufficient.
	 */
	SCX_OPSS_QSEQ_سرکاؤ	= 2,
};

/* Use macros to ensure that the type is unsigned long for the masks */
#تعریف_کرو SCX_OPSS_STATE_نقاب	((1LU << SCX_OPSS_QSEQ_سرکاؤ) - 1)
#تعریف_کرو SCX_OPSS_QSEQ_نقاب	(~SCX_OPSS_STATE_نقاب)

/*
 * SCX task iterator.
 */
ساخت scx_task_iter {
	ساخت sched_ext_entity		cursor;
	ساخت task_struct		*locked_task;
	ساخت rq			*rq;
	ساخت rq_flags			rf;
	u32				cnt;
	bool				list_locked;
#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	ساخت cgroup			*cgrp;
	ساخت cgroup_subsys_state	*css_pos;
	ساخت css_task_iter		css_iter;
#اگر_ختم
};

/*
 * scx_enable() is offloaded to a dedicated system-wide RT kthread to avoid
 * starvation. During the READY -> ENABLED task switching loop, the calling
 * thread's sched_class gets switched from fair to ext. As fair has higher
 * priority than ext, the calling thread can be indefinitely starved under
 * fair-class saturation, leading to a system hang.
 */
ساخت scx_enable_cmd {
	ساخت kthread_work	work;
	اتحاد {
		ساخت sched_ext_ops		*ops;
		ساخت sched_ext_ops_cid	*ops_cid;
	};
	bool			is_cid_type;
	ساخت bpf_map		*arena_map;	/* arena ref to transfer to sch */
	صحیح			ret;
};

/* string formatting from BPF */
ساخت scx_bstr_buf {
	u64			ڈیٹا[MAX_BPRINTF_VARARGS];
	حرف			سطر[SCX_EXIT_پیغام_LEN];
};

/* Internal helper for DEFINE_SCX_COMPAT_MARKER(). */
#تعریف_کرو DECLARE_SCX_COMPAT_MARKER(func)						\
	خارجی خالی scx_compat_marker_##func(خالی)

/**
 * DEFINE_SCX_COMPAT_MARKER() - define a userspace capability marker
 * @func: marker suffix; the defined symbol is scx_compat_marker_@func
 *
 * Emit an empty, callerless function that is retained in the kernel's BTF.
 * Its presence is part of the kernel<->userspace contract: userspace probes
 * scx_compat_marker_@func (e.g. via BTF) to detect that this kernel supports
 * the corresponding feature.
 *
 * The leading declaration suppresses the missing-prototype warning; the
 * trailing declaration consumes the semicolon at the use site.
 */
#تعریف_کرو DEFINE_SCX_COMPAT_MARKER(func)						\
	DECLARE_SCX_COMPAT_MARKER(func);					\
	__used __retain خالی scx_compat_marker_##func(خالی) {}			\
	DECLARE_SCX_COMPAT_MARKER(func)

خارجی ساخت scx_sched __rcu *scx_جڑ;
DECLARE_فی_CPU(ساخت rq *, scx_locked_rq_state);

/*
 * True when the currently loaded scheduler hierarchy is cid-form. All scheds
 * in a hierarchy share one form, so this single key tells callsites which
 * view to use without per-sch dereferences. Use scx_is_cid_type() to test.
 */
DECLARE_STATIC_کلید_FALSE(__scx_is_cid_type);

صحیح scx_kfunc_context_filter(ثابت ساخت bpf_prog *prog, u32 kfunc_id);

bool scx_cpu_valid(ساخت scx_sched *sch, s32 cpu, ثابت حرف *where);

__printf(5, 0) bool scx_vexit(ساخت scx_sched *sch, شمار scx_exit_قسم قسم,
			      s64 exit_رمز, s32 exit_cpu, ثابت حرف *fmt,
			      va_list دلائل);
__printf(5, 6) bool __scx_exit(ساخت scx_sched *sch, شمار scx_exit_قسم قسم,
			       s64 exit_رمز, s32 exit_cpu, ثابت حرف *fmt, ...);

u32 scx_get_task_state(ثابت ساخت task_struct *p);
خالی scx_سیٹ_task_state(ساخت task_struct *p, u32 state);
خالی scx_task_iter_آغاز(ساخت scx_task_iter *iter, ساخت cgroup *cgrp);
خالی scx_task_iter_unlock(ساخت scx_task_iter *iter);
خالی scx_task_iter_stop(ساخت scx_task_iter *iter);
ساخت task_struct *scx_task_iter_اگلا_locked(ساخت scx_task_iter *iter);
bool scx_سیٹ_task_قاش(ساخت task_struct *p, u64 قاش);
خالی scx_task_قاش_ended(ساخت rq *rq, ساخت task_struct *p);
خالی scx_task_unlink_from_dsq(ساخت task_struct *p, ساخت scx_dispatch_q *dsq);
خالی scx_dispatch_قطار_سے_نکالو(ساخت rq *rq, ساخت task_struct *p);
خالی scx_do_قطار_میں_ڈالو_task(ساخت rq *rq, ساخت task_struct *p, u64 enq_flags,
			 صحیح sticky_cpu);
خالی scx_move_local_task_مقصد_local_dsq(ساخت scx_sched *sch, ساخت task_struct *p,
				      u64 enq_flags, ساخت rq *dst_rq);
bool scx_consume_dispatch_q(ساخت scx_sched *sch, ساخت rq *rq,
			    ساخت scx_dispatch_q *dsq, u64 enq_flags);
bool scx_consume_global_dsq(ساخت scx_sched *sch, ساخت rq *rq);
bool scx_rq_online(ساخت rq *rq);
خالی scx_flush_dispatch_buf(ساخت scx_sched *sch, ساخت rq *rq);
s32 scx_init_dsq(ساخت scx_dispatch_q *dsq, u64 dsq_id, ساخت scx_sched *sch);
__printf(2, 3) خالی scx_dump_سطر(ساخت seq_buf *s, ثابت حرف *fmt, ...);
خالی scx_kick_cpu(ساخت scx_sched *sch, s32 cpu, u64 flags);
u64 __scx_bpf_now(ساخت rq *rq);
خالی جدول_وقت_dsq_reenq(ساخت scx_sched *sch, ساخت scx_dispatch_q *dsq,
			u64 reenq_flags, ساخت rq *locked_rq);
خالی scx_wait_dispatching(ساخت task_struct *p);
صحیح __scx_init_task(ساخت scx_sched *sch, ساخت task_struct *p,
		    ساخت cgroup *cgrp, bool fork);
خالی scx_enable_task(ساخت scx_sched *sch, ساخت task_struct *p);
خالی __scx_disable_and_exit_task(ساخت scx_sched *sch, ساخت task_struct *p);
خالی scx_sub_init_cancel_task(ساخت scx_sched *sch, ساخت task_struct *p);
خالی scx_disable_and_exit_task(ساخت scx_sched *sch, ساخت task_struct *p);
#اگر defined(CONFIG_EXT_گروہ_SCHED) || defined(CONFIG_EXT_SUB_SCHED)
خالی scx_cgroup_تالا(خالی);
خالی scx_cgroup_unlock(خالی);
#اگر_ختم
s32 scx_alloc_kern_arena_objs(ساخت scx_sched *sch);
خالی scx_disable_bypass_dsp(ساخت scx_sched *sch);
خالی scx_bypass(ساخت scx_sched *sch, bool bypass);
s32 scx_link_sched(ساخت scx_sched *sch);
خالی scx_unlink_sched(ساخت scx_sched *sch);
خالی scx_disable_dump(ساخت scx_sched *sch);
خالی scx_log_sched_disable(ساخت scx_sched *sch);
خالی scx_flush_disable_work(ساخت scx_sched *sch);
ساخت scx_sched *scx_alloc_and_جوڑو_sched(ساخت scx_enable_cmd *cmd,
					  ساخت cgroup *cgrp,
					  ساخت scx_sched *والد);
صحیح scx_validate_ops(ساخت scx_sched *sch, ثابت ساخت sched_ext_ops *ops);
صحیح scx_sched_sysfs_جوڑو(ساخت scx_sched *sch);
bool scx_is_descendant(ساخت scx_sched *sch, ساخت scx_sched *ancestor);
__printf(5, 0) bool scx_exit_bstr(ساخت scx_sched *sch, شمار scx_exit_قسم قسم,
				  s64 exit_رمز, ساخت scx_sched *fmt_blame,
				  حرف *fmt, غیرعلامتی طویل طویل *ڈیٹا, u32 ڈیٹا_sz);

خارجی raw_spinlock_t scx_sched_تالا;
خارجی ساخت mutex scx_enable_mutex;
خارجی ساخت percpu_rw_semaphore scx_fork_rwsem;
خارجی bool scx_cgroup_enabled;
خارجی ساخت list_سر scx_sched_all;
#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
خارجی ثابت ساخت rhashtable_params scx_sched_hash_params;
خارجی ساخت rhashtable scx_sched_hash;
خارجی ساخت scx_sched *scx_enabling_sub_sched;
#اگر_ختم

#تعریف_کرو scx_exit(sch, قسم, exit_رمز, fmt, دلائل...)				\
	__scx_exit(sch, قسم, exit_رمز, raw_smp_processor_id(), fmt, ##args)
#تعریف_کرو scx_error(sch, fmt, دلائل...)						\
	scx_exit((sch), SCX_EXIT_ERROR, 0, fmt, ##args)

/**
 * scx_root_protected_live - Root sched for paths that only run while live
 *
 * scx_root is published before the scheduler goes live and cleared only after
 * it is fully drained, so a path that only executes while the scheduler is live
 * can never race an update. Return the root sched with a plain load, never
 * %NULL.
 */
ساکن اندرونی ساخت scx_sched *scx_جڑ_protected_زندہ(خالی)
{
	واپس rcu_dereference_protected(scx_جڑ, true);
}

/**
 * scx_root_protected - Root sched for contexts that exclude its updates
 *
 * Both scx_root updates run under the locks checked below, so holding one
 * excludes them. Return the root sched with a plain load, %NULL if no scheduler
 * is loaded.
 */
ساکن اندرونی ساخت scx_sched *scx_جڑ_protected(خالی)
{
	واپس rcu_dereference_protected(scx_جڑ,
					 lockdep_is_cpus_held() ||
					 lockdep_is_held(&scx_enable_mutex));
}

ساکن اندرونی ساخت scx_dispatch_q *scx_bypass_dsq(ساخت scx_sched *sch, s32 cpu)
{
	واپس &فی_cpu_ptr(sch->pcpu, cpu)->bypass_dsq;
}

/**
 * scx_bypass_dsp_enabled - Check if bypass dispatch path is enabled
 * @sch: scheduler to check
 *
 * When a descendant scheduler enters bypass mode, bypassed tasks are scheduled
 * by the nearest non-bypassing ancestor, or the root scheduler if all ancestors
 * are bypassing. In the former case, the ancestor is not itself bypassing but
 * its bypass DSQs will be populated with bypassed tasks from descendants. Thus,
 * the ancestor's bypass dispatch path must be active even though its own
 * bypass_depth remains zero.
 *
 * This function checks bypass_dsp_enable_depth which is managed separately from
 * bypass_depth to enable this decoupling. See enable_bypass_dsp() and
 * scx_disable_bypass_dsp().
 */
ساکن اندرونی bool scx_bypass_dsp_enabled(ساخت scx_sched *sch)
{
	واپس unlikely(atomic_read(&sch->bypass_dsp_enable_گہرائی));
}

/**
 * scx_ops_sanitize_err - Sanitize a -errno value
 * @sch: scx_sched to error out on error
 * @ops_name: operation to blame on failure
 * @err: -errno value to sanitize
 *
 * Verify @err is a valid -errno. If not, trigger scx_error() and return
 * -%EPROTO. This is necessary because returning a rogue -errno up the chain can
 * cause misbehaviors. For an example, a large negative return from
 * ops.init_task() triggers an oops when passed up the call chain because the
 * value fails IS_ERR() test after being encoded with ERR_PTR() and then is
 * handled as a pointer.
 */
ساکن اندرونی صحیح scx_ops_sanitize_err(ساخت scx_sched *sch, ثابت حرف *ops_name, s32 err)
{
	اگر (err < 0 && err >= -MAX_ERRNO)
		واپس err;

	scx_error(sch, "ops.%s() returned an invalid errno %d", ops_name, err);
	واپس -EPROTO;
}

ساکن اندرونی خالی scx_جدول_وقت_reenq_local(ساخت rq *rq, u64 reenq_flags)
{
	ساخت scx_sched *جڑ = rcu_dereference_sched(scx_جڑ);

	اگر (WARN_ON_ONCE(!جڑ))
		واپس;

	جدول_وقت_dsq_reenq(جڑ, &rq->scx.local_dsq, reenq_flags, rq);
}

/*
 * Return the rq currently locked from an scx callback, or NULL if no rq is
 * locked.
 */
ساکن اندرونی ساخت rq *scx_locked_rq(خالی)
{
	واپس __this_cpu_read(scx_locked_rq_state);
}

ساکن اندرونی خالی update_locked_rq(ساخت rq *rq)
{
	/*
	 * Check whether @rq is actually locked. This can help expose bugs
	 * or incorrect assumptions about the context in which a kfunc or
	 * callback is executed.
	 */
	اگر (rq)
		lockdep_assert_rq_held(rq);
	__this_cpu_write(scx_locked_rq_state, rq);
}

#تعریف_کرو SCX_HAS_OP(sch, op)	test_ثنائی_ہندسہ(SCX_OP_IDX(op), (sch)->has_op)

/*
 * SCX ops can recurse via scx_bpf_sub_dispatch() - the inner call must not
 * clobber the outer's scx_locked_rq_state. Save it on entry, restore on exit.
 *
 * @ops is the ops table to dispatch through: ops for the cpu form, ops_cid
 * for the cid form.
 */
#تعریف_کرو __SCX_CALL_OP(sch, ops, op, locked_rq, دلائل...)				\
کرو {										\
	ساخت rq *__prev_locked_rq;						\
										\
	اگر (locked_rq) {							\
		__prev_locked_rq = scx_locked_rq();				\
		update_locked_rq(locked_rq);					\
	}									\
	(sch)->ops.op(دلائل);							\
	اگر (locked_rq)								\
		update_locked_rq(__prev_locked_rq);				\
} جب (0)

#تعریف_کرو SCX_CALL_OP(sch, op, locked_rq, دلائل...)				\
	__SCX_CALL_OP(sch, ops, op, locked_rq, ##args)

#تعریف_کرو SCX_CALL_OP_RET(sch, op, locked_rq, دلائل...)				\
({										\
	ساخت rq *__prev_locked_rq;						\
	__typeof__((sch)->ops.op(دلائل)) __ret;					\
										\
	اگر (locked_rq) {							\
		__prev_locked_rq = scx_locked_rq();				\
		update_locked_rq(locked_rq);					\
	}									\
	__ret = (sch)->ops.op(دلائل);						\
	اگر (locked_rq)								\
		update_locked_rq(__prev_locked_rq);				\
	__ret;									\
})

/*
 * SCX_CALL_OP_TASK*() invokes an SCX op that takes one or two task arguments
 * and records them in current->scx.kf_tasks[] for the duration of the call. A
 * kfunc invoked from inside such an op can then use
 * scx_kf_arg_task_ok() to verify that its task argument is one of
 * those subject tasks.
 *
 * Every SCX_CALL_OP_TASK*() call site invokes its op with @p's rq lock held -
 * either via the @locked_rq argument here, or (for ops.select_cpu()) via @p's
 * pi_lock held by try_to_wake_up() with rq tracking via scx_rq.in_select_cpu.
 * So if kf_tasks[] is set, @p's scheduler-protected fields are stable.
 *
 * kf_tasks[] can not stack, so task-based SCX ops must not nest. The
 * WARN_ON_ONCE() in each macro catches a re-entry of any of the three variants
 * while a previous one is still in progress.
 */
#تعریف_کرو __SCX_CALL_OP_TASK(sch, ops, op, locked_rq, task, دلائل...)		\
کرو {										\
	WARN_ON_ONCE(current->scx.kf_tasks[0]);					\
	current->scx.kf_tasks[0] = task;					\
	__SCX_CALL_OP((sch), ops, op, locked_rq, task, ##args);			\
	current->scx.kf_tasks[0] = تہی;					\
} جب (0)

/*
 * A per-task op runs on @task's owner - WARN if @sch isn't it. Sites that must
 * target a different scheduler call __SCX_CALL_OP_TASK() directly.
 */
#تعریف_کرو SCX_CALL_OP_TASK(sch, op, locked_rq, task, دلائل...)			\
کرو {										\
	WARN_ON_ONCE(scx_has_subs() && (sch) != scx_task_sched_rcu(task));	\
	__SCX_CALL_OP_TASK((sch), ops, op, locked_rq, task, ##args);		\
} جب (0)

/*
 * Dispatch a task op through the cid-form ops_cid table, for the ops whose
 * cid-form signature differs from the cpu-form slot: set_cmask() takes an arena
 * cmask instead of a cpumask and enable() takes scx_enable_args.
 */
#تعریف_کرو SCX_CALL_CID_OP_TASK(sch, op, locked_rq, task, دلائل...)			\
	__SCX_CALL_OP_TASK(sch, ops_cid, op, locked_rq, task, ##args)

#تعریف_کرو SCX_CALL_OP_TASK_RET(sch, op, locked_rq, task, دلائل...)			\
({										\
	__typeof__((sch)->ops.op(task, ##args)) __ret;				\
	WARN_ON_ONCE(scx_has_subs() && (sch) != scx_task_sched_rcu(task));	\
	WARN_ON_ONCE(current->scx.kf_tasks[0]);					\
	current->scx.kf_tasks[0] = task;					\
	__ret = SCX_CALL_OP_RET((sch), op, locked_rq, task, ##args);		\
	current->scx.kf_tasks[0] = تہی;					\
	__ret;									\
})

#تعریف_کرو SCX_CALL_OP_2TASKS_RET(sch, op, locked_rq, task0, task1, دلائل...)	\
({										\
	__typeof__((sch)->ops.op(task0, task1, ##args)) __ret;			\
	WARN_ON_ONCE(current->scx.kf_tasks[0]);					\
	current->scx.kf_tasks[0] = task0;					\
	current->scx.kf_tasks[1] = task1;					\
	__ret = SCX_CALL_OP_RET((sch), op, locked_rq, task0, task1, ##args);	\
	current->scx.kf_tasks[0] = تہی;					\
	current->scx.kf_tasks[1] = تہی;					\
	__ret;									\
})

/* see SCX_CALL_OP_TASK() */
ساکن __always_inline bool scx_kf_arg_task_ok(ساخت scx_sched *sch,
					       ساخت task_struct *p)
{
	اگر (unlikely((p != current->scx.kf_tasks[0] &&
		      p != current->scx.kf_tasks[1]))) {
		scx_error(sch, "called on a task not being operated on");
		واپس false;
	}

	واپس true;
}

ساکن اندرونی bool scx_bypassing(ساخت scx_sched *sch, s32 cpu)
{
	واپس unlikely(فی_cpu_ptr(sch->pcpu, cpu)->flags &
			SCX_SCHED_PCPU_BYPASSING);
}

#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
DECLARE_STATIC_کلید_FALSE(__scx_has_subs);

/**
 * scx_has_subs - Whether any sub-scheduler exists
 *
 * Gates the sub-sched portions of hot paths so that a root-only system doesn't
 * pay for them. See scx_sub_enable_workfn() and scx_sched_free_rcu_work().
 */
ساکن اندرونی bool scx_has_subs(خالی)
{
	واپس static_branch_unlikely(&__scx_has_subs);
}

/**
 * scx_task_sched - Find scx_sched scheduling a task
 * @p: task of interest
 *
 * Return @p's scheduler instance. Must be called with @p's pi_lock or rq lock
 * held.
 */
ساکن اندرونی ساخت scx_sched *scx_task_sched(ثابت ساخت task_struct *p)
{
	واپس rcu_dereference_protected(p->scx.sched,
					 lockdep_is_held(&p->pi_تالا) ||
					 lockdep_is_held(__rq_lockp(task_rq(p))));
}

/**
 * scx_task_sched_rcu - Find scx_sched scheduling a task
 * @p: task of interest
 *
 * Return @p's scheduler instance. The returned scx_sched is RCU protected.
 */
ساکن اندرونی ساخت scx_sched *scx_task_sched_rcu(ثابت ساخت task_struct *p)
{
	واپس rcu_dereference_all(p->scx.sched);
}

/**
 * scx_task_on_sched - Is a task on the specified sched?
 * @sch: sched to test against
 * @p: task of interest
 *
 * Returns %true if @p is on @sch, %false otherwise.
 */
ساکن اندرونی bool scx_task_on_sched(ساخت scx_sched *sch,
				     ثابت ساخت task_struct *p)
{
	واپس rcu_access_اشارہ_گر(p->scx.sched) == sch;
}

/**
 * scx_prog_sched - Find scx_sched associated with a BPF prog
 * @aux: aux passed in from BPF to a kfunc
 *
 * To be called from kfuncs. Return the scheduler instance associated with the
 * BPF program given the implicit kfunc argument aux. The returned scx_sched is
 * RCU protected.
 */
ساکن اندرونی ساخت scx_sched *scx_prog_sched(ثابت ساخت bpf_prog_aux *aux)
{
	ساخت sched_ext_ops *ops;
	ساخت scx_sched *sch, *جڑ;

	ops = bpf_prog_get_assoc_struct_ops(aux);
	اگر (likely(ops)) {
		sch = rcu_dereference_all(ops->priv);
		اگر (sch && unlikely(READ_ONCE(sch->dead)))
			واپس تہی;
		واپس sch;
	}

	جڑ = rcu_dereference_all(scx_جڑ);
	اگر (جڑ) {
		اگر (unlikely(READ_ONCE(جڑ->dead)))
			واپس تہی;
		/*
		 * COMPAT-v6.19: Schedulers built before sub-sched support was
		 * introduced may have unassociated non-struct_ops programs.
		 */
		اگر (!جڑ->ops.sub_attach)
			واپس جڑ;

		اگر (!جڑ->warned_unassoc_progs) {
			printk_deferred(KERN_WARNING "sched_ext: Unassociated program %s (id %d)\n",
					aux->name, aux->id);
			جڑ->warned_unassoc_progs = true;
		}
	}

	واپس تہی;
}

/**
 * scx_parent - Find the parent sched
 * @sch: sched to find the parent of
 *
 * Returns the parent scheduler or %NULL if @sch is root.
 */
ساکن اندرونی ساخت scx_sched *scx_والد(ساخت scx_sched *sch)
{
	اگر (sch->level)
		واپس sch->ancestors[sch->level - 1];
	ورنہ
		واپس تہی;
}

#ورنہ	/* CONFIG_EXT_SUB_SCHED */
ساکن اندرونی bool scx_has_subs(خالی) { واپس false; }

ساکن اندرونی ساخت scx_sched *scx_task_sched(ثابت ساخت task_struct *p)
{
	واپس rcu_dereference_protected(scx_جڑ,
					 lockdep_is_held(&p->pi_تالا) ||
					 lockdep_is_held(__rq_lockp(task_rq(p))));
}

ساکن اندرونی ساخت scx_sched *scx_task_sched_rcu(ثابت ساخت task_struct *p)
{
	واپس rcu_dereference_all(scx_جڑ);
}

ساکن اندرونی bool scx_task_on_sched(ساخت scx_sched *sch,
				     ثابت ساخت task_struct *p)
{
	واپس true;
}

ساکن اندرونی ساخت scx_sched *scx_prog_sched(ثابت ساخت bpf_prog_aux *aux)
{
	ساخت scx_sched *جڑ = rcu_dereference_all(scx_جڑ);

	اگر (جڑ && unlikely(READ_ONCE(جڑ->dead)))
		واپس تہی;
	واپس جڑ;
}

ساکن اندرونی ساخت scx_sched *scx_والد(ساخت scx_sched *sch) { واپس تہی; }

#اگر_ختم	/* CONFIG_EXT_SUB_SCHED */

#اگر_ختم /* _KERNEL_SCHED_EXT_INTERNAL_H */
