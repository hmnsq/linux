// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2010-2017 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * membarrier system call
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/membarrier.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

/*
 * For documentation purposes, here are some membarrier ordering
 * scenarios to keep in mind:
 *
 * A) Userspace thread execution after IPI vs membarrier's memory
 *    barrier before sending the IPI
 *
 * Userspace variables:
 *
 * int x = 0, y = 0;
 *
 * The memory barrier at the start of membarrier() on CPU0 is necessary in
 * order to enforce the guarantee that any writes occurring on CPU0 before
 * the membarrier() is executed will be visible to any code executing on
 * CPU1 after the IPI-induced memory barrier:
 *
 *         CPU0                              CPU1
 *
 *         x = 1
 *         membarrier():
 *           a: smp_mb()
 *           b: send IPI                       IPI-induced mb
 *           c: smp_mb()
 *         r2 = y
 *                                           y = 1
 *                                           barrier()
 *                                           r1 = x
 *
 *                     BUG_ON(r1 == 0 && r2 == 0)
 *
 * The write to y and load from x by CPU1 are unordered by the hardware,
 * so it's possible to have "r1 = x" reordered before "y = 1" at any
 * point after (b).  If the memory barrier at (a) is omitted, then "x = 1"
 * can be reordered after (a) (although not after (c)), so we get r1 == 0
 * and r2 == 0.  This violates the guarantee that membarrier() is
 * supposed by provide.
 *
 * The timing of the memory barrier at (a) has to ensure that it executes
 * before the IPI-induced memory barrier on CPU1.
 *
 * B) Userspace thread execution before IPI vs membarrier's memory
 *    barrier after completing the IPI
 *
 * Userspace variables:
 *
 * int x = 0, y = 0;
 *
 * The memory barrier at the end of membarrier() on CPU0 is necessary in
 * order to enforce the guarantee that any writes occurring on CPU1 before
 * the membarrier() is executed will be visible to any code executing on
 * CPU0 after the membarrier():
 *
 *         CPU0                              CPU1
 *
 *                                           x = 1
 *                                           barrier()
 *                                           y = 1
 *         r2 = y
 *         membarrier():
 *           a: smp_mb()
 *           b: send IPI                       IPI-induced mb
 *           c: smp_mb()
 *         r1 = x
 *         BUG_ON(r1 == 0 && r2 == 1)
 *
 * The writes to x and y are unordered by the hardware, so it's possible to
 * have "r2 = 1" even though the write to x doesn't execute until (b).  If
 * the memory barrier at (c) is omitted then "r1 = x" can be reordered
 * before (b) (although not before (a)), so we get "r1 = 0".  This violates
 * the guarantee that membarrier() is supposed to provide.
 *
 * The timing of the memory barrier at (c) has to ensure that it executes
 * after the IPI-induced memory barrier on CPU1.
 *
 * C) Scheduling userspace thread -> kthread -> userspace thread vs membarrier
 *
 *           CPU0                            CPU1
 *
 *           membarrier():
 *           a: smp_mb()
 *                                           d: switch to kthread (includes mb)
 *           b: read rq->curr->mm == NULL
 *                                           e: switch to user (includes mb)
 *           c: smp_mb()
 *
 * Using the scenario from (A), we can show that (a) needs to be paired
 * with (e). Using the scenario from (B), we can show that (c) needs to
 * be paired with (d).
 *
 * D) exit_mm vs membarrier
 *
 * Two thread groups are created, A and B.  Thread group B is created by
 * issuing clone from group A with flag CLONE_VM set, but not CLONE_THREAD.
 * Let's assume we have a single thread within each thread group (Thread A
 * and Thread B).  Thread A runs on CPU0, Thread B runs on CPU1.
 *
 *           CPU0                            CPU1
 *
 *           membarrier():
 *             a: smp_mb()
 *                                           exit_mm():
 *                                             d: smp_mb()
 *                                             e: current->mm = NULL
 *             b: read rq->curr->mm == NULL
 *             c: smp_mb()
 *
 * Using scenario (B), we can show that (c) needs to be paired with (d).
 *
 * E) kthread_{use,unuse}_mm vs membarrier
 *
 *           CPU0                            CPU1
 *
 *           membarrier():
 *           a: smp_mb()
 *                                           kthread_unuse_mm()
 *                                             d: smp_mb()
 *                                             e: current->mm = NULL
 *           b: read rq->curr->mm == NULL
 *                                           kthread_use_mm()
 *                                             f: current->mm = mm
 *                                             g: smp_mb()
 *           c: smp_mb()
 *
 * Using the scenario from (A), we can show that (a) needs to be paired
 * with (g). Using the scenario from (B), we can show that (c) needs to
 * be paired with (d).
 */

/*
 * Bitmask made from a "or" of all commands within enum membarrier_cmd,
 * except MEMBARRIER_CMD_QUERY.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MEMBARRIER_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE_BITMASK			\
	(MEMBARRIER_CMD_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE			\
	| MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MEMBARRIER_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE_BITMASK	0
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RSEQ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MEMBARRIER_ਨਿੱਜੀ_EXPEDITED_RSEQ_BITMASK		\
	(MEMBARRIER_CMD_ਨਿੱਜੀ_EXPEDITED_RSEQ			\
	| MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED_RSEQ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MEMBARRIER_ਨਿੱਜੀ_EXPEDITED_RSEQ_BITMASK	0
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MEMBARRIER_CMD_BITMASK						\
	(MEMBARRIER_CMD_GLOBAL | MEMBARRIER_CMD_GLOBAL_EXPEDITED	\
	| MEMBARRIER_CMD_ਰਜਿਸਟਰ_GLOBAL_EXPEDITED			\
	| MEMBARRIER_CMD_ਨਿੱਜੀ_EXPEDITED				\
	| MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED			\
	| MEMBARRIER_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE_BITMASK		\
	| MEMBARRIER_ਨਿੱਜੀ_EXPEDITED_RSEQ_BITMASK			\
	| MEMBARRIER_CMD_GET_REGISTRATIONS)

/*
 * Scoped guard for memory barriers on entry and exit.
 * Matches memory barriers before & after rq->curr modification in scheduler.
 */
DEFINE_ਤਾਲਾ_GUARD_0(mb, smp_mb(), smp_mb())
ਸਥਿਰ DEFINE_MUTEX(membarrier_ipi_mutex);
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ mutex, membarrier_cpu_mutexes);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SERIALIZE_IPI() guard(mutex)(&membarrier_ipi_mutex)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SERIALIZE_IPI_CPU(cpu_id) guard(mutex)(&ਪ੍ਰਤੀ_cpu(membarrier_cpu_mutexes, cpu_id))

ਸਥਿਰ ਪੂਰਨ __init membarrier_init(ਖਾਲੀ)
{
	ਪੂਰਨ i;

	for_each_possible_cpu(i)
		mutex_init(&ਪ੍ਰਤੀ_cpu(membarrier_cpu_mutexes, i));
	ਵਾਪਸ 0;
}
core_initcall(membarrier_init);

ਸਥਿਰ ਖਾਲੀ ipi_mb(ਖਾਲੀ *info)
{
	smp_mb();	/* IPIs should be serializing but paranoid. */
}

ਸਥਿਰ ਖਾਲੀ ipi_sync_core(ਖਾਲੀ *info)
{
	/*
	 * The smp_mb() in membarrier after all the IPIs is supposed to
	 * ensure that memory on remote CPUs that occur before the IPI
	 * become visible to membarrier()'s caller -- see scenario B in
	 * the big comment at the top of this file.
	 *
	 * A sync_core() would provide this guarantee, but
	 * sync_core_before_usermode() might end up being deferred until
	 * after membarrier()'s smp_mb().
	 */
	smp_mb();	/* IPIs should be serializing but paranoid. */

	sync_core_before_usermode();
}

ਸਥਿਰ ਖਾਲੀ ipi_rseq(ਖਾਲੀ *info)
{
	/*
	 * Ensure that all stores done by the calling thread are visible
	 * to the current task before the current task resumes.  We could
	 * probably optimize this away on most architectures, but by the
	 * time we've already sent an IPI, the cost of the extra smp_mb()
	 * is negligible.
	 */
	smp_mb();
	/*
	 * Legacy mode requires that IDs are written and the critical section is
	 * evaluated. V2 optimized mode handles the critical section and IDs are
	 * only updated if they change as a consequence of preemption after
	 * return from this IPI.
	 */
	ਜੇ (rseq_v2(current))
		rseq_sched_switch_event(current);
	ਨਹੀਂਤਾਂ
		rseq_force_update();
}

ਸਥਿਰ ਖਾਲੀ ipi_sync_rq_state(ਖਾਲੀ *info)
{
	ਬਣਤਰ mm_struct *mm = (ਬਣਤਰ mm_struct *) info;

	ਜੇ (current->mm != mm)
		ਵਾਪਸ;
	this_cpu_write(runqueues.membarrier_state,
		       atomic_read(&mm->membarrier_state));
	/*
	 * Issue a memory barrier after setting
	 * MEMBARRIER_STATE_GLOBAL_EXPEDITED in the current runqueue to
	 * guarantee that no memory access following registration is reordered
	 * before registration.
	 */
	smp_mb();
}

ਖਾਲੀ membarrier_exec_mmap(ਬਣਤਰ mm_struct *mm)
{
	/*
	 * Issue a memory barrier before clearing membarrier_state to
	 * guarantee that no memory access prior to exec is reordered after
	 * clearing this state.
	 */
	smp_mb();
	atomic_ਸੈੱਟ(&mm->membarrier_state, 0);
	/*
	 * Keep the runqueue membarrier_state in sync with this mm
	 * membarrier_state.
	 */
	this_cpu_write(runqueues.membarrier_state, 0);
}

ਖਾਲੀ membarrier_update_current_mm(ਬਣਤਰ mm_struct *ਅਗਲਾ_mm)
{
	ਬਣਤਰ rq *rq = this_rq();
	ਪੂਰਨ membarrier_state = 0;

	ਜੇ (ਅਗਲਾ_mm)
		membarrier_state = atomic_read(&ਅਗਲਾ_mm->membarrier_state);
	ਜੇ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ਵਾਪਸ;
	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

ਸਥਿਰ ਪੂਰਨ membarrier_global_expedited(ਖਾਲੀ)
{
	cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
	ਪੂਰਨ cpu;

	ਜੇ (ਅੰਕ_online_cpus() == 1)
		ਵਾਪਸ 0;

	ਜੇ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		ਵਾਪਸ -ENOMEM;

	guard(mb)();
	SERIALIZE_IPI();
	guard(cpus_read_ਤਾਲਾ)();

	rcu_read_ਤਾਲਾ();
	for_each_online_cpu(cpu) {
		ਬਣਤਰ task_struct *p;

		/*
		 * Skipping the current CPU is OK even through we can be
		 * migrated at any point. The current CPU, at the point
		 * where we read raw_smp_processor_id(), is ensured to
		 * be in program order with respect to the caller
		 * thread. Therefore, we can skip this CPU from the
		 * iteration.
		 */
		ਜੇ (cpu == raw_smp_processor_id())
			ਜਾਰੀ;

		ਜੇ (!(READ_ONCE(cpu_rq(cpu)->membarrier_state) &
		    MEMBARRIER_STATE_GLOBAL_EXPEDITED))
			ਜਾਰੀ;

		/*
		 * Skip the CPU if it runs a kernel thread which is not using
		 * a task mm.
		 */
		p = rcu_dereference(cpu_rq(cpu)->curr);
		ਜੇ (!p->mm)
			ਜਾਰੀ;

		__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	preempt_disable();
	smp_call_function_many(tmpmask, ipi_mb, ਨਿਰਾ, 1);
	preempt_enable();

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ membarrier_ਨਿੱਜੀ_expedited(ਪੂਰਨ flags, ਪੂਰਨ cpu_id)
{
	ਬਣਤਰ mm_struct *mm = current->mm;
	smp_call_func_t ipi_func = ipi_mb;

	ਜੇ (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		ਜੇ (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			ਵਾਪਸ -EINVAL;
		ਜੇ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE_READY))
			ਵਾਪਸ -EPERM;
		ipi_func = ipi_sync_core;
		prepare_sync_core_cmd(mm);
	} ਨਹੀਂਤਾਂ ਜੇ (flags == MEMBARRIER_FLAG_RSEQ) {
		ਜੇ (!IS_ENABLED(CONFIG_RSEQ))
			ਵਾਪਸ -EINVAL;
		ਜੇ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_RSEQ_READY))
			ਵਾਪਸ -EPERM;
		ipi_func = ipi_rseq;
	} ਨਹੀਂਤਾਂ {
		WARN_ON_ONCE(flags);
		ਜੇ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_READY))
			ਵਾਪਸ -EPERM;
	}

	ਜੇ (flags != MEMBARRIER_FLAG_SYNC_CORE &&
	    (atomic_read(&mm->mm_users) == 1 || ਅੰਕ_online_cpus() == 1))
		ਵਾਪਸ 0;

	/*
	 * Matches memory barriers after rq->curr modification in
	 * scheduler.
	 *
	 * On RISC-V, this barrier pairing is also needed for the
	 * SYNC_CORE command when switching between processes, cf.
	 * the inline comments in membarrier_arch_switch_mm().
	 *
	 * Memory barrier on the caller thread _after_ we finished
	 * waiting for the last IPI. Matches memory barriers before
	 * rq->curr modification in scheduler.
	 */
	guard(mb)();
	ਜੇ (cpu_id >= 0) {
		ਜੇ (cpu_id >= nr_cpu_ids || !cpu_possible(cpu_id))
			ਵਾਪਸ 0;

		SERIALIZE_IPI_CPU(cpu_id);
		guard(cpus_read_ਤਾਲਾ)();
		ਬਣਤਰ task_struct *p;

		ਜੇ (!cpu_online(cpu_id))
			ਵਾਪਸ 0;

		rcu_read_ਤਾਲਾ();
		p = rcu_dereference(cpu_rq(cpu_id)->curr);
		ਜੇ (!p || p->mm != mm) {
			rcu_read_unlock();
			ਵਾਪਸ 0;
		}
		rcu_read_unlock();
		/*
		 * smp_call_function_single() will call ipi_func() if cpu_id
		 * is the calling CPU.
		 */
		smp_call_function_single(cpu_id, ipi_func, ਨਿਰਾ, 1);
	} ਨਹੀਂਤਾਂ {
		cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
		ਪੂਰਨ cpu;

		ਜੇ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
			ਵਾਪਸ -ENOMEM;

		SERIALIZE_IPI();
		guard(cpus_read_ਤਾਲਾ)();

		rcu_read_ਤਾਲਾ();
		for_each_online_cpu(cpu) {
			ਬਣਤਰ task_struct *p;

			p = rcu_dereference(cpu_rq(cpu)->curr);
			ਜੇ (p && p->mm == mm)
				__cpumask_set_cpu(cpu, tmpmask);
		}
		rcu_read_unlock();
		/*
		 * For regular membarrier, we can save a few cycles by
		 * skipping the current cpu -- we're about to do smp_mb()
		 * below, and if we migrate to a different cpu, this cpu
		 * and the new cpu will execute a full barrier in the
		 * scheduler.
		 *
		 * For SYNC_CORE, we do need a barrier on the current cpu --
		 * otherwise, if we are migrated and replaced by a different
		 * task in the same mm just before, during, or after
		 * membarrier, we will end up with some thread in the mm
		 * running without a core sync.
		 *
		 * For RSEQ, don't invoke rseq_sched_switch_event() on the
		 * caller.  User code is not supposed to issue syscalls at
		 * all from inside an rseq critical section.
		 */
		ਜੇ (flags != MEMBARRIER_FLAG_SYNC_CORE) {
			preempt_disable();
			smp_call_function_many(tmpmask, ipi_func, ਨਿਰਾ, true);
			preempt_enable();
		} ਨਹੀਂਤਾਂ {
			on_each_cpu_ਨਕਾਬ(tmpmask, ipi_func, ਨਿਰਾ, true);
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sync_runqueues_membarrier_state(ਬਣਤਰ mm_struct *mm)
{
	ਪੂਰਨ membarrier_state = atomic_read(&mm->membarrier_state);
	cpumask_var_t tmpmask;
	ਪੂਰਨ cpu;

	ਜੇ (atomic_read(&mm->mm_users) == 1 || ਅੰਕ_online_cpus() == 1) {
		this_cpu_write(runqueues.membarrier_state, membarrier_state);

		/*
		 * For single mm user, we can simply issue a memory barrier
		 * after setting MEMBARRIER_STATE_GLOBAL_EXPEDITED in the
		 * mm and in the current runqueue to guarantee that no memory
		 * access following registration is reordered before
		 * registration.
		 */
		smp_mb();
		ਵਾਪਸ 0;
	}

	ਜੇ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		ਵਾਪਸ -ENOMEM;

	/*
	 * For mm with multiple users, we need to ensure all future
	 * scheduler executions will observe @mm's new membarrier
	 * state.
	 */
	synchronize_rcu();

	/*
	 * For each cpu runqueue, if the task's mm match @mm, ensure that all
	 * @mm's membarrier state set bits are also set in the runqueue's
	 * membarrier state. This ensures that a runqueue scheduling
	 * between threads which are users of @mm has its membarrier state
	 * updated.
	 */
	SERIALIZE_IPI();
	cpus_read_ਤਾਲਾ();
	rcu_read_ਤਾਲਾ();
	for_each_online_cpu(cpu) {
		ਬਣਤਰ rq *rq = cpu_rq(cpu);
		ਬਣਤਰ task_struct *p;

		p = rcu_dereference(rq->curr);
		ਜੇ (p && p->mm == mm)
			__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	on_each_cpu_ਨਕਾਬ(tmpmask, ipi_sync_rq_state, mm, true);

	free_cpumask_var(tmpmask);
	cpus_read_unlock();

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ membarrier_ਰਜਿਸਟਰ_global_expedited(ਖਾਲੀ)
{
	ਬਣਤਰ task_struct *p = current;
	ਬਣਤਰ mm_struct *mm = p->mm;
	ਪੂਰਨ ret;

	ਜੇ (atomic_read(&mm->membarrier_state) &
	    MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY)
		ਵਾਪਸ 0;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	ਜੇ (ret)
		ਵਾਪਸ ret;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		  &mm->membarrier_state);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ membarrier_ਰਜਿਸਟਰ_ਨਿੱਜੀ_expedited(ਪੂਰਨ flags)
{
	ਬਣਤਰ task_struct *p = current;
	ਬਣਤਰ mm_struct *mm = p->mm;
	ਪੂਰਨ ready_state = MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_READY,
	    ਸੈੱਟ_state = MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED,
	    ret;

	ਜੇ (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		ਜੇ (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			ਵਾਪਸ -EINVAL;
		ready_state =
			MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE_READY;
	} ਨਹੀਂਤਾਂ ਜੇ (flags == MEMBARRIER_FLAG_RSEQ) {
		ਜੇ (!IS_ENABLED(CONFIG_RSEQ))
			ਵਾਪਸ -EINVAL;
		ready_state =
			MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_RSEQ_READY;
	} ਨਹੀਂਤਾਂ {
		WARN_ON_ONCE(flags);
	}

	/*
	 * We need to consider threads belonging to different thread
	 * groups, which use the same mm. (CLONE_VM but not
	 * CLONE_THREAD).
	 */
	ਜੇ ((atomic_read(&mm->membarrier_state) & ready_state) == ready_state)
		ਵਾਪਸ 0;
	ਜੇ (flags & MEMBARRIER_FLAG_SYNC_CORE)
		ਸੈੱਟ_state |= MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE;
	ਜੇ (flags & MEMBARRIER_FLAG_RSEQ)
		ਸੈੱਟ_state |= MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_RSEQ;
	atomic_or(ਸੈੱਟ_state, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	ਜੇ (ret)
		ਵਾਪਸ ret;
	atomic_or(ready_state, &mm->membarrier_state);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ membarrier_get_registrations(ਖਾਲੀ)
{
	ਬਣਤਰ task_struct *p = current;
	ਬਣਤਰ mm_struct *mm = p->mm;
	ਪੂਰਨ registrations_ਨਕਾਬ = 0, membarrier_state, i;
	ਸਥਿਰ ਅਚਲ ਪੂਰਨ ਹਾਲਤਾਂ[] = {
		MEMBARRIER_STATE_GLOBAL_EXPEDITED |
			MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED |
			MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_READY,
		MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE |
			MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE_READY,
		MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_RSEQ |
			MEMBARRIER_STATE_ਨਿੱਜੀ_EXPEDITED_RSEQ_READY
	};
	ਸਥਿਰ ਅਚਲ ਪੂਰਨ registration_cmds[] = {
		MEMBARRIER_CMD_ਰਜਿਸਟਰ_GLOBAL_EXPEDITED,
		MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED,
		MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE,
		MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED_RSEQ
	};
	BUILD_BUG_ON(ARRAY_ਮਾਪ(ਹਾਲਤਾਂ) != ARRAY_ਮਾਪ(registration_cmds));

	membarrier_state = atomic_read(&mm->membarrier_state);
	ਲਈ (i = 0; i < ARRAY_ਮਾਪ(ਹਾਲਤਾਂ); ++i) {
		ਜੇ (membarrier_state & ਹਾਲਤਾਂ[i]) {
			registrations_ਨਕਾਬ |= registration_cmds[i];
			membarrier_state &= ~ਹਾਲਤਾਂ[i];
		}
	}
	WARN_ON_ONCE(membarrier_state != 0);
	ਵਾਪਸ registrations_ਨਕਾਬ;
}

/**
 * sys_membarrier - issue memory barriers on a set of threads
 * @cmd:    Takes command values defined in enum membarrier_cmd.
 * @flags:  Currently needs to be 0 for all commands other than
 *          MEMBARRIER_CMD_PRIVATE_EXPEDITED_RSEQ: in the latter
 *          case it can be MEMBARRIER_CMD_FLAG_CPU, indicating that @cpu_id
 *          contains the CPU on which to interrupt (= restart)
 *          the RSEQ critical section.
 * @cpu_id: if @flags == MEMBARRIER_CMD_FLAG_CPU, indicates the cpu on which
 *          RSEQ CS should be interrupted (@cmd must be
 *          MEMBARRIER_CMD_PRIVATE_EXPEDITED_RSEQ).
 *
 * If this system call is not implemented, -ENOSYS is returned. If the
 * command specified does not exist, not available on the running
 * kernel, or if the command argument is invalid, this system call
 * returns -EINVAL. For a given command, with flags argument set to 0,
 * if this system call returns -ENOSYS or -EINVAL, it is guaranteed to
 * always return the same value until reboot. In addition, it can return
 * -ENOMEM if there is not enough memory available to perform the system
 * call.
 *
 * All memory accesses performed in program order from each targeted thread
 * is guaranteed to be ordered with respect to sys_membarrier(). If we use
 * the semantic "barrier()" to represent a compiler barrier forcing memory
 * accesses to be performed in program order across the barrier, and
 * smp_mb() to represent explicit memory barriers forcing full memory
 * ordering across the barrier, we have the following ordering table for
 * each pair of barrier(), sys_membarrier() and smp_mb():
 *
 * The pair ordering is detailed as (O: ordered, X: not ordered):
 *
 *                        barrier()   smp_mb() sys_membarrier()
 *        barrier()          X           X            O
 *        smp_mb()           X           O            O
 *        sys_membarrier()   O           O            O
 */
SYSCALL_DEFINE3(membarrier, ਪੂਰਨ, cmd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, flags, ਪੂਰਨ, cpu_id)
{
	ਚੋਣ (cmd) {
	ਹਾਲਤ MEMBARRIER_CMD_ਨਿੱਜੀ_EXPEDITED_RSEQ:
		ਜੇ (unlikely(flags && flags != MEMBARRIER_CMD_FLAG_CPU))
			ਵਾਪਸ -EINVAL;
		ਰੋਕੋ;
	ਮੂਲ:
		ਜੇ (unlikely(flags))
			ਵਾਪਸ -EINVAL;
	}

	ਜੇ (!(flags & MEMBARRIER_CMD_FLAG_CPU))
		cpu_id = -1;

	ਚੋਣ (cmd) {
	ਹਾਲਤ MEMBARRIER_CMD_QUERY:
	{
		ਪੂਰਨ cmd_ਨਕਾਬ = MEMBARRIER_CMD_BITMASK;

		ਜੇ (tick_nohz_ਭਰਿਆ_enabled())
			cmd_ਨਕਾਬ &= ~MEMBARRIER_CMD_GLOBAL;
		ਵਾਪਸ cmd_ਨਕਾਬ;
	}
	ਹਾਲਤ MEMBARRIER_CMD_GLOBAL:
		/* MEMBARRIER_CMD_GLOBAL is not compatible with nohz_full. */
		ਜੇ (tick_nohz_ਭਰਿਆ_enabled())
			ਵਾਪਸ -EINVAL;
		ਜੇ (ਅੰਕ_online_cpus() > 1)
			synchronize_rcu();
		ਵਾਪਸ 0;
	ਹਾਲਤ MEMBARRIER_CMD_GLOBAL_EXPEDITED:
		ਵਾਪਸ membarrier_global_expedited();
	ਹਾਲਤ MEMBARRIER_CMD_ਰਜਿਸਟਰ_GLOBAL_EXPEDITED:
		ਵਾਪਸ membarrier_ਰਜਿਸਟਰ_global_expedited();
	ਹਾਲਤ MEMBARRIER_CMD_ਨਿੱਜੀ_EXPEDITED:
		ਵਾਪਸ membarrier_ਨਿੱਜੀ_expedited(0, cpu_id);
	ਹਾਲਤ MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED:
		ਵਾਪਸ membarrier_ਰਜਿਸਟਰ_ਨਿੱਜੀ_expedited(0);
	ਹਾਲਤ MEMBARRIER_CMD_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE:
		ਵਾਪਸ membarrier_ਨਿੱਜੀ_expedited(MEMBARRIER_FLAG_SYNC_CORE, cpu_id);
	ਹਾਲਤ MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED_SYNC_CORE:
		ਵਾਪਸ membarrier_ਰਜਿਸਟਰ_ਨਿੱਜੀ_expedited(MEMBARRIER_FLAG_SYNC_CORE);
	ਹਾਲਤ MEMBARRIER_CMD_ਨਿੱਜੀ_EXPEDITED_RSEQ:
		ਵਾਪਸ membarrier_ਨਿੱਜੀ_expedited(MEMBARRIER_FLAG_RSEQ, cpu_id);
	ਹਾਲਤ MEMBARRIER_CMD_ਰਜਿਸਟਰ_ਨਿੱਜੀ_EXPEDITED_RSEQ:
		ਵਾਪਸ membarrier_ਰਜਿਸਟਰ_ਨਿੱਜੀ_expedited(MEMBARRIER_FLAG_RSEQ);
	ਹਾਲਤ MEMBARRIER_CMD_GET_REGISTRATIONS:
		ਵਾਪਸ membarrier_get_registrations();
	ਮੂਲ:
		ਵਾਪਸ -EINVAL;
	}
}
