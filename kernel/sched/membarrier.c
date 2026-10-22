// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2010-2017 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * membarrier system call
 */
#అన్తర్భావ <uapi/linux/membarrier.h>
#అన్తర్భావ "sched.h"

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
#నిర్వచితమైతే CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE
#నిర్వచించు MEMBARRIER_వ్యక్తిగత_EXPEDITED_SYNC_CORE_BITMASK			\
	(MEMBARRIER_CMD_వ్యక్తిగత_EXPEDITED_SYNC_CORE			\
	| MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED_SYNC_CORE)
#లేకపోతే
#నిర్వచించు MEMBARRIER_వ్యక్తిగత_EXPEDITED_SYNC_CORE_BITMASK	0
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_RSEQ
#నిర్వచించు MEMBARRIER_వ్యక్తిగత_EXPEDITED_RSEQ_BITMASK		\
	(MEMBARRIER_CMD_వ్యక్తిగత_EXPEDITED_RSEQ			\
	| MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED_RSEQ)
#లేకపోతే
#నిర్వచించు MEMBARRIER_వ్యక్తిగత_EXPEDITED_RSEQ_BITMASK	0
#అయితే_ముగింపు

#నిర్వచించు MEMBARRIER_CMD_BITMASK						\
	(MEMBARRIER_CMD_GLOBAL | MEMBARRIER_CMD_GLOBAL_EXPEDITED	\
	| MEMBARRIER_CMD_రిజిస్టరు_GLOBAL_EXPEDITED			\
	| MEMBARRIER_CMD_వ్యక్తిగత_EXPEDITED				\
	| MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED			\
	| MEMBARRIER_వ్యక్తిగత_EXPEDITED_SYNC_CORE_BITMASK		\
	| MEMBARRIER_వ్యక్తిగత_EXPEDITED_RSEQ_BITMASK			\
	| MEMBARRIER_CMD_GET_REGISTRATIONS)

/*
 * Scoped guard for memory barriers on entry and exit.
 * Matches memory barriers before & after rq->curr modification in scheduler.
 */
DEFINE_తాళం_GUARD_0(mb, smp_mb(), smp_mb())
స్థిర DEFINE_MUTEX(membarrier_ipi_mutex);
స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం mutex, membarrier_cpu_mutexes);

#నిర్వచించు SERIALIZE_IPI() guard(mutex)(&membarrier_ipi_mutex)
#నిర్వచించు SERIALIZE_IPI_CPU(cpu_id) guard(mutex)(&చొప్పున_cpu(membarrier_cpu_mutexes, cpu_id))

స్థిర పూర్ణాంకం __init membarrier_init(శూన్య)
{
	పూర్ణాంకం i;

	for_each_possible_cpu(i)
		mutex_init(&చొప్పున_cpu(membarrier_cpu_mutexes, i));
	తిరిగి 0;
}
core_initcall(membarrier_init);

స్థిర శూన్య ipi_mb(శూన్య *info)
{
	smp_mb();	/* IPIs should be serializing but paranoid. */
}

స్థిర శూన్య ipi_sync_core(శూన్య *info)
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

స్థిర శూన్య ipi_rseq(శూన్య *info)
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
	ఒకవేళ (rseq_v2(current))
		rseq_sched_switch_event(current);
	లేకపోతే
		rseq_force_update();
}

స్థిర శూన్య ipi_sync_rq_state(శూన్య *info)
{
	నిర్మాణం mm_struct *mm = (నిర్మాణం mm_struct *) info;

	ఒకవేళ (current->mm != mm)
		తిరిగి;
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

శూన్య membarrier_exec_mmap(నిర్మాణం mm_struct *mm)
{
	/*
	 * Issue a memory barrier before clearing membarrier_state to
	 * guarantee that no memory access prior to exec is reordered after
	 * clearing this state.
	 */
	smp_mb();
	atomic_సమితి(&mm->membarrier_state, 0);
	/*
	 * Keep the runqueue membarrier_state in sync with this mm
	 * membarrier_state.
	 */
	this_cpu_write(runqueues.membarrier_state, 0);
}

శూన్య membarrier_update_current_mm(నిర్మాణం mm_struct *తదుపరి_mm)
{
	నిర్మాణం rq *rq = this_rq();
	పూర్ణాంకం membarrier_state = 0;

	ఒకవేళ (తదుపరి_mm)
		membarrier_state = atomic_read(&తదుపరి_mm->membarrier_state);
	ఒకవేళ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		తిరిగి;
	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

స్థిర పూర్ణాంకం membarrier_global_expedited(శూన్య)
{
	cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
	పూర్ణాంకం cpu;

	ఒకవేళ (అంకె_online_cpus() == 1)
		తిరిగి 0;

	ఒకవేళ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		తిరిగి -ENOMEM;

	guard(mb)();
	SERIALIZE_IPI();
	guard(cpus_read_తాళం)();

	rcu_read_తాళం();
	for_each_online_cpu(cpu) {
		నిర్మాణం task_struct *p;

		/*
		 * Skipping the current CPU is OK even through we can be
		 * migrated at any point. The current CPU, at the point
		 * where we read raw_smp_processor_id(), is ensured to
		 * be in program order with respect to the caller
		 * thread. Therefore, we can skip this CPU from the
		 * iteration.
		 */
		ఒకవేళ (cpu == raw_smp_processor_id())
			కొనసాగు;

		ఒకవేళ (!(READ_ONCE(cpu_rq(cpu)->membarrier_state) &
		    MEMBARRIER_STATE_GLOBAL_EXPEDITED))
			కొనసాగు;

		/*
		 * Skip the CPU if it runs a kernel thread which is not using
		 * a task mm.
		 */
		p = rcu_dereference(cpu_rq(cpu)->curr);
		ఒకవేళ (!p->mm)
			కొనసాగు;

		__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	preempt_disable();
	smp_call_function_many(tmpmask, ipi_mb, ఖాళీ, 1);
	preempt_enable();

	తిరిగి 0;
}

స్థిర పూర్ణాంకం membarrier_వ్యక్తిగత_expedited(పూర్ణాంకం flags, పూర్ణాంకం cpu_id)
{
	నిర్మాణం mm_struct *mm = current->mm;
	smp_call_func_t ipi_func = ipi_mb;

	ఒకవేళ (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		ఒకవేళ (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			తిరిగి -EINVAL;
		ఒకవేళ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_SYNC_CORE_READY))
			తిరిగి -EPERM;
		ipi_func = ipi_sync_core;
		prepare_sync_core_cmd(mm);
	} లేకపోతే ఒకవేళ (flags == MEMBARRIER_FLAG_RSEQ) {
		ఒకవేళ (!IS_ENABLED(CONFIG_RSEQ))
			తిరిగి -EINVAL;
		ఒకవేళ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_RSEQ_READY))
			తిరిగి -EPERM;
		ipi_func = ipi_rseq;
	} లేకపోతే {
		WARN_ON_ONCE(flags);
		ఒకవేళ (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_READY))
			తిరిగి -EPERM;
	}

	ఒకవేళ (flags != MEMBARRIER_FLAG_SYNC_CORE &&
	    (atomic_read(&mm->mm_users) == 1 || అంకె_online_cpus() == 1))
		తిరిగి 0;

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
	ఒకవేళ (cpu_id >= 0) {
		ఒకవేళ (cpu_id >= nr_cpu_ids || !cpu_possible(cpu_id))
			తిరిగి 0;

		SERIALIZE_IPI_CPU(cpu_id);
		guard(cpus_read_తాళం)();
		నిర్మాణం task_struct *p;

		ఒకవేళ (!cpu_online(cpu_id))
			తిరిగి 0;

		rcu_read_తాళం();
		p = rcu_dereference(cpu_rq(cpu_id)->curr);
		ఒకవేళ (!p || p->mm != mm) {
			rcu_read_unlock();
			తిరిగి 0;
		}
		rcu_read_unlock();
		/*
		 * smp_call_function_single() will call ipi_func() if cpu_id
		 * is the calling CPU.
		 */
		smp_call_function_single(cpu_id, ipi_func, ఖాళీ, 1);
	} లేకపోతే {
		cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
		పూర్ణాంకం cpu;

		ఒకవేళ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
			తిరిగి -ENOMEM;

		SERIALIZE_IPI();
		guard(cpus_read_తాళం)();

		rcu_read_తాళం();
		for_each_online_cpu(cpu) {
			నిర్మాణం task_struct *p;

			p = rcu_dereference(cpu_rq(cpu)->curr);
			ఒకవేళ (p && p->mm == mm)
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
		ఒకవేళ (flags != MEMBARRIER_FLAG_SYNC_CORE) {
			preempt_disable();
			smp_call_function_many(tmpmask, ipi_func, ఖాళీ, true);
			preempt_enable();
		} లేకపోతే {
			on_each_cpu_ముసుగు(tmpmask, ipi_func, ఖాళీ, true);
		}
	}

	తిరిగి 0;
}

స్థిర పూర్ణాంకం sync_runqueues_membarrier_state(నిర్మాణం mm_struct *mm)
{
	పూర్ణాంకం membarrier_state = atomic_read(&mm->membarrier_state);
	cpumask_var_t tmpmask;
	పూర్ణాంకం cpu;

	ఒకవేళ (atomic_read(&mm->mm_users) == 1 || అంకె_online_cpus() == 1) {
		this_cpu_write(runqueues.membarrier_state, membarrier_state);

		/*
		 * For single mm user, we can simply issue a memory barrier
		 * after setting MEMBARRIER_STATE_GLOBAL_EXPEDITED in the
		 * mm and in the current runqueue to guarantee that no memory
		 * access following registration is reordered before
		 * registration.
		 */
		smp_mb();
		తిరిగి 0;
	}

	ఒకవేళ (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		తిరిగి -ENOMEM;

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
	cpus_read_తాళం();
	rcu_read_తాళం();
	for_each_online_cpu(cpu) {
		నిర్మాణం rq *rq = cpu_rq(cpu);
		నిర్మాణం task_struct *p;

		p = rcu_dereference(rq->curr);
		ఒకవేళ (p && p->mm == mm)
			__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	on_each_cpu_ముసుగు(tmpmask, ipi_sync_rq_state, mm, true);

	free_cpumask_var(tmpmask);
	cpus_read_unlock();

	తిరిగి 0;
}

స్థిర పూర్ణాంకం membarrier_రిజిస్టరు_global_expedited(శూన్య)
{
	నిర్మాణం task_struct *p = current;
	నిర్మాణం mm_struct *mm = p->mm;
	పూర్ణాంకం ret;

	ఒకవేళ (atomic_read(&mm->membarrier_state) &
	    MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY)
		తిరిగి 0;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	ఒకవేళ (ret)
		తిరిగి ret;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		  &mm->membarrier_state);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం membarrier_రిజిస్టరు_వ్యక్తిగత_expedited(పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *p = current;
	నిర్మాణం mm_struct *mm = p->mm;
	పూర్ణాంకం ready_state = MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_READY,
	    సమితి_state = MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED,
	    ret;

	ఒకవేళ (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		ఒకవేళ (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			తిరిగి -EINVAL;
		ready_state =
			MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_SYNC_CORE_READY;
	} లేకపోతే ఒకవేళ (flags == MEMBARRIER_FLAG_RSEQ) {
		ఒకవేళ (!IS_ENABLED(CONFIG_RSEQ))
			తిరిగి -EINVAL;
		ready_state =
			MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_RSEQ_READY;
	} లేకపోతే {
		WARN_ON_ONCE(flags);
	}

	/*
	 * We need to consider threads belonging to different thread
	 * groups, which use the same mm. (CLONE_VM but not
	 * CLONE_THREAD).
	 */
	ఒకవేళ ((atomic_read(&mm->membarrier_state) & ready_state) == ready_state)
		తిరిగి 0;
	ఒకవేళ (flags & MEMBARRIER_FLAG_SYNC_CORE)
		సమితి_state |= MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_SYNC_CORE;
	ఒకవేళ (flags & MEMBARRIER_FLAG_RSEQ)
		సమితి_state |= MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_RSEQ;
	atomic_or(సమితి_state, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	ఒకవేళ (ret)
		తిరిగి ret;
	atomic_or(ready_state, &mm->membarrier_state);

	తిరిగి 0;
}

స్థిర పూర్ణాంకం membarrier_get_registrations(శూన్య)
{
	నిర్మాణం task_struct *p = current;
	నిర్మాణం mm_struct *mm = p->mm;
	పూర్ణాంకం registrations_ముసుగు = 0, membarrier_state, i;
	స్థిర స్థిరరాశి పూర్ణాంకం స్థితులు[] = {
		MEMBARRIER_STATE_GLOBAL_EXPEDITED |
			MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED |
			MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_READY,
		MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_SYNC_CORE |
			MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_SYNC_CORE_READY,
		MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_RSEQ |
			MEMBARRIER_STATE_వ్యక్తిగత_EXPEDITED_RSEQ_READY
	};
	స్థిర స్థిరరాశి పూర్ణాంకం registration_cmds[] = {
		MEMBARRIER_CMD_రిజిస్టరు_GLOBAL_EXPEDITED,
		MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED,
		MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED_SYNC_CORE,
		MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED_RSEQ
	};
	BUILD_BUG_ON(ARRAY_కొలత(స్థితులు) != ARRAY_కొలత(registration_cmds));

	membarrier_state = atomic_read(&mm->membarrier_state);
	కొరకు (i = 0; i < ARRAY_కొలత(స్థితులు); ++i) {
		ఒకవేళ (membarrier_state & స్థితులు[i]) {
			registrations_ముసుగు |= registration_cmds[i];
			membarrier_state &= ~స్థితులు[i];
		}
	}
	WARN_ON_ONCE(membarrier_state != 0);
	తిరిగి registrations_ముసుగు;
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
SYSCALL_DEFINE3(membarrier, పూర్ణాంకం, cmd, గుర్తులేని పూర్ణాంకం, flags, పూర్ణాంకం, cpu_id)
{
	ఎంపిక (cmd) {
	స్థితి MEMBARRIER_CMD_వ్యక్తిగత_EXPEDITED_RSEQ:
		ఒకవేళ (unlikely(flags && flags != MEMBARRIER_CMD_FLAG_CPU))
			తిరిగి -EINVAL;
		ఆపు;
	అప్రమేయ:
		ఒకవేళ (unlikely(flags))
			తిరిగి -EINVAL;
	}

	ఒకవేళ (!(flags & MEMBARRIER_CMD_FLAG_CPU))
		cpu_id = -1;

	ఎంపిక (cmd) {
	స్థితి MEMBARRIER_CMD_QUERY:
	{
		పూర్ణాంకం cmd_ముసుగు = MEMBARRIER_CMD_BITMASK;

		ఒకవేళ (tick_nohz_నిండిన_enabled())
			cmd_ముసుగు &= ~MEMBARRIER_CMD_GLOBAL;
		తిరిగి cmd_ముసుగు;
	}
	స్థితి MEMBARRIER_CMD_GLOBAL:
		/* MEMBARRIER_CMD_GLOBAL is not compatible with nohz_full. */
		ఒకవేళ (tick_nohz_నిండిన_enabled())
			తిరిగి -EINVAL;
		ఒకవేళ (అంకె_online_cpus() > 1)
			synchronize_rcu();
		తిరిగి 0;
	స్థితి MEMBARRIER_CMD_GLOBAL_EXPEDITED:
		తిరిగి membarrier_global_expedited();
	స్థితి MEMBARRIER_CMD_రిజిస్టరు_GLOBAL_EXPEDITED:
		తిరిగి membarrier_రిజిస్టరు_global_expedited();
	స్థితి MEMBARRIER_CMD_వ్యక్తిగత_EXPEDITED:
		తిరిగి membarrier_వ్యక్తిగత_expedited(0, cpu_id);
	స్థితి MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED:
		తిరిగి membarrier_రిజిస్టరు_వ్యక్తిగత_expedited(0);
	స్థితి MEMBARRIER_CMD_వ్యక్తిగత_EXPEDITED_SYNC_CORE:
		తిరిగి membarrier_వ్యక్తిగత_expedited(MEMBARRIER_FLAG_SYNC_CORE, cpu_id);
	స్థితి MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED_SYNC_CORE:
		తిరిగి membarrier_రిజిస్టరు_వ్యక్తిగత_expedited(MEMBARRIER_FLAG_SYNC_CORE);
	స్థితి MEMBARRIER_CMD_వ్యక్తిగత_EXPEDITED_RSEQ:
		తిరిగి membarrier_వ్యక్తిగత_expedited(MEMBARRIER_FLAG_RSEQ, cpu_id);
	స్థితి MEMBARRIER_CMD_రిజిస్టరు_వ్యక్తిగత_EXPEDITED_RSEQ:
		తిరిగి membarrier_రిజిస్టరు_వ్యక్తిగత_expedited(MEMBARRIER_FLAG_RSEQ);
	స్థితి MEMBARRIER_CMD_GET_REGISTRATIONS:
		తిరిగి membarrier_get_registrations();
	అప్రమేయ:
		తిరిగి -EINVAL;
	}
}
