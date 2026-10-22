// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2010-2017 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * membarrier system call
 */
#ಅನ್ತರ್ಭಾವ <uapi/linux/membarrier.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE
#ವ್ಯಾಖ್ಯಾನಿಸು MEMBARRIER_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE_BITMASK			\
	(MEMBARRIER_CMD_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE			\
	| MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು MEMBARRIER_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE_BITMASK	0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RSEQ
#ವ್ಯಾಖ್ಯಾನಿಸು MEMBARRIER_ಖಾಸಗಿ_EXPEDITED_RSEQ_BITMASK		\
	(MEMBARRIER_CMD_ಖಾಸಗಿ_EXPEDITED_RSEQ			\
	| MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED_RSEQ)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು MEMBARRIER_ಖಾಸಗಿ_EXPEDITED_RSEQ_BITMASK	0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು MEMBARRIER_CMD_BITMASK						\
	(MEMBARRIER_CMD_GLOBAL | MEMBARRIER_CMD_GLOBAL_EXPEDITED	\
	| MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_GLOBAL_EXPEDITED			\
	| MEMBARRIER_CMD_ಖಾಸಗಿ_EXPEDITED				\
	| MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED			\
	| MEMBARRIER_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE_BITMASK		\
	| MEMBARRIER_ಖಾಸಗಿ_EXPEDITED_RSEQ_BITMASK			\
	| MEMBARRIER_CMD_GET_REGISTRATIONS)

/*
 * Scoped guard for memory barriers on entry and exit.
 * Matches memory barriers before & after rq->curr modification in scheduler.
 */
DEFINE_ಬೀಗ_GUARD_0(mb, smp_mb(), smp_mb())
ಸ್ಥಿರ DEFINE_MUTEX(membarrier_ipi_mutex);
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ mutex, membarrier_cpu_mutexes);

#ವ್ಯಾಖ್ಯಾನಿಸು SERIALIZE_IPI() guard(mutex)(&membarrier_ipi_mutex)
#ವ್ಯಾಖ್ಯಾನಿಸು SERIALIZE_IPI_CPU(cpu_id) guard(mutex)(&ಪ್ರತಿ_cpu(membarrier_cpu_mutexes, cpu_id))

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init membarrier_init(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;

	for_each_possible_cpu(i)
		mutex_init(&ಪ್ರತಿ_cpu(membarrier_cpu_mutexes, i));
	ಹಿಂತಿರುಗಿ 0;
}
core_initcall(membarrier_init);

ಸ್ಥಿರ ಶೂನ್ಯ ipi_mb(ಶೂನ್ಯ *info)
{
	smp_mb();	/* IPIs should be serializing but paranoid. */
}

ಸ್ಥಿರ ಶೂನ್ಯ ipi_sync_core(ಶೂನ್ಯ *info)
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

ಸ್ಥಿರ ಶೂನ್ಯ ipi_rseq(ಶೂನ್ಯ *info)
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
	ಆದರೆ (rseq_v2(current))
		rseq_sched_switch_event(current);
	ಇಲ್ಲದಿದ್ದರೆ
		rseq_force_update();
}

ಸ್ಥಿರ ಶೂನ್ಯ ipi_sync_rq_state(ಶೂನ್ಯ *info)
{
	ರಚನೆ mm_struct *mm = (ರಚನೆ mm_struct *) info;

	ಆದರೆ (current->mm != mm)
		ಹಿಂತಿರುಗಿ;
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

ಶೂನ್ಯ membarrier_exec_mmap(ರಚನೆ mm_struct *mm)
{
	/*
	 * Issue a memory barrier before clearing membarrier_state to
	 * guarantee that no memory access prior to exec is reordered after
	 * clearing this state.
	 */
	smp_mb();
	atomic_ಗಣ(&mm->membarrier_state, 0);
	/*
	 * Keep the runqueue membarrier_state in sync with this mm
	 * membarrier_state.
	 */
	this_cpu_write(runqueues.membarrier_state, 0);
}

ಶೂನ್ಯ membarrier_update_current_mm(ರಚನೆ mm_struct *ಮುಂದಿನ_mm)
{
	ರಚನೆ rq *rq = this_rq();
	ಪೂರ್ಣಾಂಕ membarrier_state = 0;

	ಆದರೆ (ಮುಂದಿನ_mm)
		membarrier_state = atomic_read(&ಮುಂದಿನ_mm->membarrier_state);
	ಆದರೆ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ಹಿಂತಿರುಗಿ;
	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ membarrier_global_expedited(ಶೂನ್ಯ)
{
	cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
	ಪೂರ್ಣಾಂಕ cpu;

	ಆದರೆ (ಅಂಕೆ_online_cpus() == 1)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		ಹಿಂತಿರುಗಿ -ENOMEM;

	guard(mb)();
	SERIALIZE_IPI();
	guard(cpus_read_ಬೀಗ)();

	rcu_read_ಬೀಗ();
	for_each_online_cpu(cpu) {
		ರಚನೆ task_struct *p;

		/*
		 * Skipping the current CPU is OK even through we can be
		 * migrated at any point. The current CPU, at the point
		 * where we read raw_smp_processor_id(), is ensured to
		 * be in program order with respect to the caller
		 * thread. Therefore, we can skip this CPU from the
		 * iteration.
		 */
		ಆದರೆ (cpu == raw_smp_processor_id())
			ಮುಂದುವರಿ;

		ಆದರೆ (!(READ_ONCE(cpu_rq(cpu)->membarrier_state) &
		    MEMBARRIER_STATE_GLOBAL_EXPEDITED))
			ಮುಂದುವರಿ;

		/*
		 * Skip the CPU if it runs a kernel thread which is not using
		 * a task mm.
		 */
		p = rcu_dereference(cpu_rq(cpu)->curr);
		ಆದರೆ (!p->mm)
			ಮುಂದುವರಿ;

		__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	preempt_disable();
	smp_call_function_many(tmpmask, ipi_mb, ಖಾಲಿ, 1);
	preempt_enable();

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ membarrier_ಖಾಸಗಿ_expedited(ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ cpu_id)
{
	ರಚನೆ mm_struct *mm = current->mm;
	smp_call_func_t ipi_func = ipi_mb;

	ಆದರೆ (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		ಆದರೆ (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			ಹಿಂತಿರುಗಿ -EINVAL;
		ಆದರೆ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE_READY))
			ಹಿಂತಿರುಗಿ -EPERM;
		ipi_func = ipi_sync_core;
		prepare_sync_core_cmd(mm);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags == MEMBARRIER_FLAG_RSEQ) {
		ಆದರೆ (!IS_ENABLED(CONFIG_RSEQ))
			ಹಿಂತಿರುಗಿ -EINVAL;
		ಆದರೆ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_RSEQ_READY))
			ಹಿಂತಿರುಗಿ -EPERM;
		ipi_func = ipi_rseq;
	} ಇಲ್ಲದಿದ್ದರೆ {
		WARN_ON_ONCE(flags);
		ಆದರೆ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_READY))
			ಹಿಂತಿರುಗಿ -EPERM;
	}

	ಆದರೆ (flags != MEMBARRIER_FLAG_SYNC_CORE &&
	    (atomic_read(&mm->mm_users) == 1 || ಅಂಕೆ_online_cpus() == 1))
		ಹಿಂತಿರುಗಿ 0;

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
	ಆದರೆ (cpu_id >= 0) {
		ಆದರೆ (cpu_id >= nr_cpu_ids || !cpu_possible(cpu_id))
			ಹಿಂತಿರುಗಿ 0;

		SERIALIZE_IPI_CPU(cpu_id);
		guard(cpus_read_ಬೀಗ)();
		ರಚನೆ task_struct *p;

		ಆದರೆ (!cpu_online(cpu_id))
			ಹಿಂತಿರುಗಿ 0;

		rcu_read_ಬೀಗ();
		p = rcu_dereference(cpu_rq(cpu_id)->curr);
		ಆದರೆ (!p || p->mm != mm) {
			rcu_read_unlock();
			ಹಿಂತಿರುಗಿ 0;
		}
		rcu_read_unlock();
		/*
		 * smp_call_function_single() will call ipi_func() if cpu_id
		 * is the calling CPU.
		 */
		smp_call_function_single(cpu_id, ipi_func, ಖಾಲಿ, 1);
	} ಇಲ್ಲದಿದ್ದರೆ {
		cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
		ಪೂರ್ಣಾಂಕ cpu;

		ಆದರೆ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
			ಹಿಂತಿರುಗಿ -ENOMEM;

		SERIALIZE_IPI();
		guard(cpus_read_ಬೀಗ)();

		rcu_read_ಬೀಗ();
		for_each_online_cpu(cpu) {
			ರಚನೆ task_struct *p;

			p = rcu_dereference(cpu_rq(cpu)->curr);
			ಆದರೆ (p && p->mm == mm)
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
		ಆದರೆ (flags != MEMBARRIER_FLAG_SYNC_CORE) {
			preempt_disable();
			smp_call_function_many(tmpmask, ipi_func, ಖಾಲಿ, true);
			preempt_enable();
		} ಇಲ್ಲದಿದ್ದರೆ {
			on_each_cpu_ಮುಖವಾಡ(tmpmask, ipi_func, ಖಾಲಿ, true);
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sync_runqueues_membarrier_state(ರಚನೆ mm_struct *mm)
{
	ಪೂರ್ಣಾಂಕ membarrier_state = atomic_read(&mm->membarrier_state);
	cpumask_var_t tmpmask;
	ಪೂರ್ಣಾಂಕ cpu;

	ಆದರೆ (atomic_read(&mm->mm_users) == 1 || ಅಂಕೆ_online_cpus() == 1) {
		this_cpu_write(runqueues.membarrier_state, membarrier_state);

		/*
		 * For single mm user, we can simply issue a memory barrier
		 * after setting MEMBARRIER_STATE_GLOBAL_EXPEDITED in the
		 * mm and in the current runqueue to guarantee that no memory
		 * access following registration is reordered before
		 * registration.
		 */
		smp_mb();
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		ಹಿಂತಿರುಗಿ -ENOMEM;

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
	cpus_read_ಬೀಗ();
	rcu_read_ಬೀಗ();
	for_each_online_cpu(cpu) {
		ರಚನೆ rq *rq = cpu_rq(cpu);
		ರಚನೆ task_struct *p;

		p = rcu_dereference(rq->curr);
		ಆದರೆ (p && p->mm == mm)
			__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	on_each_cpu_ಮುಖವಾಡ(tmpmask, ipi_sync_rq_state, mm, true);

	free_cpumask_var(tmpmask);
	cpus_read_unlock();

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ membarrier_ರಿಜಿಸ್ಟರ್_global_expedited(ಶೂನ್ಯ)
{
	ರಚನೆ task_struct *p = current;
	ರಚನೆ mm_struct *mm = p->mm;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (atomic_read(&mm->membarrier_state) &
	    MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY)
		ಹಿಂತಿರುಗಿ 0;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		  &mm->membarrier_state);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ membarrier_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_expedited(ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ task_struct *p = current;
	ರಚನೆ mm_struct *mm = p->mm;
	ಪೂರ್ಣಾಂಕ ready_state = MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_READY,
	    ಗಣ_state = MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED,
	    ret;

	ಆದರೆ (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		ಆದರೆ (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			ಹಿಂತಿರುಗಿ -EINVAL;
		ready_state =
			MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE_READY;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags == MEMBARRIER_FLAG_RSEQ) {
		ಆದರೆ (!IS_ENABLED(CONFIG_RSEQ))
			ಹಿಂತಿರುಗಿ -EINVAL;
		ready_state =
			MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_RSEQ_READY;
	} ಇಲ್ಲದಿದ್ದರೆ {
		WARN_ON_ONCE(flags);
	}

	/*
	 * We need to consider threads belonging to different thread
	 * groups, which use the same mm. (CLONE_VM but not
	 * CLONE_THREAD).
	 */
	ಆದರೆ ((atomic_read(&mm->membarrier_state) & ready_state) == ready_state)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (flags & MEMBARRIER_FLAG_SYNC_CORE)
		ಗಣ_state |= MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE;
	ಆದರೆ (flags & MEMBARRIER_FLAG_RSEQ)
		ಗಣ_state |= MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_RSEQ;
	atomic_or(ಗಣ_state, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;
	atomic_or(ready_state, &mm->membarrier_state);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ membarrier_get_registrations(ಶೂನ್ಯ)
{
	ರಚನೆ task_struct *p = current;
	ರಚನೆ mm_struct *mm = p->mm;
	ಪೂರ್ಣಾಂಕ registrations_ಮುಖವಾಡ = 0, membarrier_state, i;
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿಗಳು[] = {
		MEMBARRIER_STATE_GLOBAL_EXPEDITED |
			MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED |
			MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_READY,
		MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE |
			MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE_READY,
		MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_RSEQ |
			MEMBARRIER_STATE_ಖಾಸಗಿ_EXPEDITED_RSEQ_READY
	};
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ registration_cmds[] = {
		MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_GLOBAL_EXPEDITED,
		MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED,
		MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE,
		MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED_RSEQ
	};
	BUILD_BUG_ON(ARRAY_ಗಾತ್ರ(ಸ್ಥಿತಿಗಳು) != ARRAY_ಗಾತ್ರ(registration_cmds));

	membarrier_state = atomic_read(&mm->membarrier_state);
	ಗಾಗಿ (i = 0; i < ARRAY_ಗಾತ್ರ(ಸ್ಥಿತಿಗಳು); ++i) {
		ಆದರೆ (membarrier_state & ಸ್ಥಿತಿಗಳು[i]) {
			registrations_ಮುಖವಾಡ |= registration_cmds[i];
			membarrier_state &= ~ಸ್ಥಿತಿಗಳು[i];
		}
	}
	WARN_ON_ONCE(membarrier_state != 0);
	ಹಿಂತಿರುಗಿ registrations_ಮುಖವಾಡ;
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
SYSCALL_DEFINE3(membarrier, ಪೂರ್ಣಾಂಕ, cmd, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, flags, ಪೂರ್ಣಾಂಕ, cpu_id)
{
	ಆಯ್ಕೆ (cmd) {
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ಖಾಸಗಿ_EXPEDITED_RSEQ:
		ಆದರೆ (unlikely(flags && flags != MEMBARRIER_CMD_FLAG_CPU))
			ಹಿಂತಿರುಗಿ -EINVAL;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಆದರೆ (unlikely(flags))
			ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (!(flags & MEMBARRIER_CMD_FLAG_CPU))
		cpu_id = -1;

	ಆಯ್ಕೆ (cmd) {
	ಸ್ಥಿತಿ MEMBARRIER_CMD_QUERY:
	{
		ಪೂರ್ಣಾಂಕ cmd_ಮುಖವಾಡ = MEMBARRIER_CMD_BITMASK;

		ಆದರೆ (tick_nohz_ತುಂಬಿದ_enabled())
			cmd_ಮುಖವಾಡ &= ~MEMBARRIER_CMD_GLOBAL;
		ಹಿಂತಿರುಗಿ cmd_ಮುಖವಾಡ;
	}
	ಸ್ಥಿತಿ MEMBARRIER_CMD_GLOBAL:
		/* MEMBARRIER_CMD_GLOBAL is not compatible with nohz_full. */
		ಆದರೆ (tick_nohz_ತುಂಬಿದ_enabled())
			ಹಿಂತಿರುಗಿ -EINVAL;
		ಆದರೆ (ಅಂಕೆ_online_cpus() > 1)
			synchronize_rcu();
		ಹಿಂತಿರುಗಿ 0;
	ಸ್ಥಿತಿ MEMBARRIER_CMD_GLOBAL_EXPEDITED:
		ಹಿಂತಿರುಗಿ membarrier_global_expedited();
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_GLOBAL_EXPEDITED:
		ಹಿಂತಿರುಗಿ membarrier_ರಿಜಿಸ್ಟರ್_global_expedited();
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ಖಾಸಗಿ_EXPEDITED:
		ಹಿಂತಿರುಗಿ membarrier_ಖಾಸಗಿ_expedited(0, cpu_id);
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED:
		ಹಿಂತಿರುಗಿ membarrier_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_expedited(0);
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE:
		ಹಿಂತಿರುಗಿ membarrier_ಖಾಸಗಿ_expedited(MEMBARRIER_FLAG_SYNC_CORE, cpu_id);
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED_SYNC_CORE:
		ಹಿಂತಿರುಗಿ membarrier_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_expedited(MEMBARRIER_FLAG_SYNC_CORE);
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ಖಾಸಗಿ_EXPEDITED_RSEQ:
		ಹಿಂತಿರುಗಿ membarrier_ಖಾಸಗಿ_expedited(MEMBARRIER_FLAG_RSEQ, cpu_id);
	ಸ್ಥಿತಿ MEMBARRIER_CMD_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_EXPEDITED_RSEQ:
		ಹಿಂತಿರುಗಿ membarrier_ರಿಜಿಸ್ಟರ್_ಖಾಸಗಿ_expedited(MEMBARRIER_FLAG_RSEQ);
	ಸ್ಥಿತಿ MEMBARRIER_CMD_GET_REGISTRATIONS:
		ಹಿಂತಿರುಗಿ membarrier_get_registrations();
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಹಿಂತಿರುಗಿ -EINVAL;
	}
}
