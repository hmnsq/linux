// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2010-2017 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * membarrier system call
 */
#समावेश <uapi/linux/membarrier.h>
#समावेश "sched.h"

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
#अगर_घोषित CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE
#घोषणा MEMBARRIER_PRIVATE_EXPEDITED_SYNC_CORE_BITMASK			\
	(MEMBARRIER_CMD_PRIVATE_EXPEDITED_SYNC_CORE			\
	| MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_SYNC_CORE)
#अन्यथा
#घोषणा MEMBARRIER_PRIVATE_EXPEDITED_SYNC_CORE_BITMASK	0
#पूर्ण_अगर

#अगर_घोषित CONFIG_RSEQ
#घोषणा MEMBARRIER_PRIVATE_EXPEDITED_RSEQ_BITMASK		\
	(MEMBARRIER_CMD_PRIVATE_EXPEDITED_RSEQ			\
	| MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_RSEQ)
#अन्यथा
#घोषणा MEMBARRIER_PRIVATE_EXPEDITED_RSEQ_BITMASK	0
#पूर्ण_अगर

#घोषणा MEMBARRIER_CMD_BITMASK						\
	(MEMBARRIER_CMD_GLOBAL | MEMBARRIER_CMD_GLOBAL_EXPEDITED	\
	| MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED			\
	| MEMBARRIER_CMD_PRIVATE_EXPEDITED				\
	| MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED			\
	| MEMBARRIER_PRIVATE_EXPEDITED_SYNC_CORE_BITMASK		\
	| MEMBARRIER_PRIVATE_EXPEDITED_RSEQ_BITMASK			\
	| MEMBARRIER_CMD_GET_REGISTRATIONS)

/*
 * Scoped guard for memory barriers on entry and exit.
 * Matches memory barriers before & after rq->curr modification in scheduler.
 */
DEFINE_LOCK_GUARD_0(mb, smp_mb(), smp_mb())
अटल DEFINE_MUTEX(membarrier_ipi_mutex);
अटल DEFINE_PER_CPU(काष्ठा mutex, membarrier_cpu_mutexes);

#घोषणा SERIALIZE_IPI() guard(mutex)(&membarrier_ipi_mutex)
#घोषणा SERIALIZE_IPI_CPU(cpu_id) guard(mutex)(&per_cpu(membarrier_cpu_mutexes, cpu_id))

अटल पूर्णांक __init membarrier_init(व्योम)
{
	पूर्णांक i;

	for_each_possible_cpu(i)
		mutex_init(&per_cpu(membarrier_cpu_mutexes, i));
	वापस 0;
}
core_initcall(membarrier_init);

अटल व्योम ipi_mb(व्योम *info)
{
	smp_mb();	/* IPIs should be serializing but paranoid. */
}

अटल व्योम ipi_sync_core(व्योम *info)
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

अटल व्योम ipi_rseq(व्योम *info)
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
	अगर (rseq_v2(current))
		rseq_sched_switch_event(current);
	अन्यथा
		rseq_force_update();
}

अटल व्योम ipi_sync_rq_state(व्योम *info)
{
	काष्ठा mm_struct *mm = (काष्ठा mm_struct *) info;

	अगर (current->mm != mm)
		वापस;
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

व्योम membarrier_exec_mmap(काष्ठा mm_struct *mm)
{
	/*
	 * Issue a memory barrier before clearing membarrier_state to
	 * guarantee that no memory access prior to exec is reordered after
	 * clearing this state.
	 */
	smp_mb();
	atomic_set(&mm->membarrier_state, 0);
	/*
	 * Keep the runqueue membarrier_state in sync with this mm
	 * membarrier_state.
	 */
	this_cpu_write(runqueues.membarrier_state, 0);
}

व्योम membarrier_update_current_mm(काष्ठा mm_struct *अग्र_mm)
{
	काष्ठा rq *rq = this_rq();
	पूर्णांक membarrier_state = 0;

	अगर (अग्र_mm)
		membarrier_state = atomic_read(&अग्र_mm->membarrier_state);
	अगर (READ_ONCE(rq->membarrier_state) == membarrier_state)
		वापस;
	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

अटल पूर्णांक membarrier_global_expedited(व्योम)
{
	cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
	पूर्णांक cpu;

	अगर (अंक_online_cpus() == 1)
		वापस 0;

	अगर (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		वापस -ENOMEM;

	guard(mb)();
	SERIALIZE_IPI();
	guard(cpus_read_lock)();

	rcu_read_lock();
	for_each_online_cpu(cpu) {
		काष्ठा task_struct *p;

		/*
		 * Skipping the current CPU is OK even through we can be
		 * migrated at any point. The current CPU, at the point
		 * where we read raw_smp_processor_id(), is ensured to
		 * be in program order with respect to the caller
		 * thread. Therefore, we can skip this CPU from the
		 * iteration.
		 */
		अगर (cpu == raw_smp_processor_id())
			जारी;

		अगर (!(READ_ONCE(cpu_rq(cpu)->membarrier_state) &
		    MEMBARRIER_STATE_GLOBAL_EXPEDITED))
			जारी;

		/*
		 * Skip the CPU if it runs a kernel thread which is not using
		 * a task mm.
		 */
		p = rcu_dereference(cpu_rq(cpu)->curr);
		अगर (!p->mm)
			जारी;

		__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	preempt_disable();
	smp_call_function_many(tmpmask, ipi_mb, शून्य, 1);
	preempt_enable();

	वापस 0;
}

अटल पूर्णांक membarrier_private_expedited(पूर्णांक flags, पूर्णांक cpu_id)
{
	काष्ठा mm_struct *mm = current->mm;
	smp_call_func_t ipi_func = ipi_mb;

	अगर (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		अगर (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			वापस -EINVAL;
		अगर (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_PRIVATE_EXPEDITED_SYNC_CORE_READY))
			वापस -EPERM;
		ipi_func = ipi_sync_core;
		prepare_sync_core_cmd(mm);
	} अन्यथा अगर (flags == MEMBARRIER_FLAG_RSEQ) {
		अगर (!IS_ENABLED(CONFIG_RSEQ))
			वापस -EINVAL;
		अगर (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_PRIVATE_EXPEDITED_RSEQ_READY))
			वापस -EPERM;
		ipi_func = ipi_rseq;
	} अन्यथा {
		WARN_ON_ONCE(flags);
		अगर (!(atomic_read(&mm->membarrier_state) &
		      MEMBARRIER_STATE_PRIVATE_EXPEDITED_READY))
			वापस -EPERM;
	}

	अगर (flags != MEMBARRIER_FLAG_SYNC_CORE &&
	    (atomic_read(&mm->mm_users) == 1 || अंक_online_cpus() == 1))
		वापस 0;

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
	अगर (cpu_id >= 0) {
		अगर (cpu_id >= nr_cpu_ids || !cpu_possible(cpu_id))
			वापस 0;

		SERIALIZE_IPI_CPU(cpu_id);
		guard(cpus_read_lock)();
		काष्ठा task_struct *p;

		अगर (!cpu_online(cpu_id))
			वापस 0;

		rcu_read_lock();
		p = rcu_dereference(cpu_rq(cpu_id)->curr);
		अगर (!p || p->mm != mm) {
			rcu_read_unlock();
			वापस 0;
		}
		rcu_read_unlock();
		/*
		 * smp_call_function_single() will call ipi_func() if cpu_id
		 * is the calling CPU.
		 */
		smp_call_function_single(cpu_id, ipi_func, शून्य, 1);
	} अन्यथा {
		cpumask_var_t __free(free_cpumask_var) tmpmask = CPUMASK_VAR_NULL;
		पूर्णांक cpu;

		अगर (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
			वापस -ENOMEM;

		SERIALIZE_IPI();
		guard(cpus_read_lock)();

		rcu_read_lock();
		for_each_online_cpu(cpu) {
			काष्ठा task_struct *p;

			p = rcu_dereference(cpu_rq(cpu)->curr);
			अगर (p && p->mm == mm)
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
		अगर (flags != MEMBARRIER_FLAG_SYNC_CORE) {
			preempt_disable();
			smp_call_function_many(tmpmask, ipi_func, शून्य, true);
			preempt_enable();
		} अन्यथा {
			on_each_cpu_mask(tmpmask, ipi_func, शून्य, true);
		}
	}

	वापस 0;
}

अटल पूर्णांक sync_runqueues_membarrier_state(काष्ठा mm_struct *mm)
{
	पूर्णांक membarrier_state = atomic_read(&mm->membarrier_state);
	cpumask_var_t tmpmask;
	पूर्णांक cpu;

	अगर (atomic_read(&mm->mm_users) == 1 || अंक_online_cpus() == 1) {
		this_cpu_write(runqueues.membarrier_state, membarrier_state);

		/*
		 * For single mm user, we can simply issue a memory barrier
		 * after setting MEMBARRIER_STATE_GLOBAL_EXPEDITED in the
		 * mm and in the current runqueue to guarantee that no memory
		 * access following registration is reordered before
		 * registration.
		 */
		smp_mb();
		वापस 0;
	}

	अगर (!zalloc_cpumask_var(&tmpmask, GFP_KERNEL))
		वापस -ENOMEM;

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
	cpus_read_lock();
	rcu_read_lock();
	for_each_online_cpu(cpu) {
		काष्ठा rq *rq = cpu_rq(cpu);
		काष्ठा task_struct *p;

		p = rcu_dereference(rq->curr);
		अगर (p && p->mm == mm)
			__cpumask_set_cpu(cpu, tmpmask);
	}
	rcu_read_unlock();

	on_each_cpu_mask(tmpmask, ipi_sync_rq_state, mm, true);

	free_cpumask_var(tmpmask);
	cpus_read_unlock();

	वापस 0;
}

अटल पूर्णांक membarrier_register_global_expedited(व्योम)
{
	काष्ठा task_struct *p = current;
	काष्ठा mm_struct *mm = p->mm;
	पूर्णांक ret;

	अगर (atomic_read(&mm->membarrier_state) &
	    MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY)
		वापस 0;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	अगर (ret)
		वापस ret;
	atomic_or(MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		  &mm->membarrier_state);

	वापस 0;
}

अटल पूर्णांक membarrier_register_private_expedited(पूर्णांक flags)
{
	काष्ठा task_struct *p = current;
	काष्ठा mm_struct *mm = p->mm;
	पूर्णांक ready_state = MEMBARRIER_STATE_PRIVATE_EXPEDITED_READY,
	    set_state = MEMBARRIER_STATE_PRIVATE_EXPEDITED,
	    ret;

	अगर (flags == MEMBARRIER_FLAG_SYNC_CORE) {
		अगर (!IS_ENABLED(CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE))
			वापस -EINVAL;
		ready_state =
			MEMBARRIER_STATE_PRIVATE_EXPEDITED_SYNC_CORE_READY;
	} अन्यथा अगर (flags == MEMBARRIER_FLAG_RSEQ) {
		अगर (!IS_ENABLED(CONFIG_RSEQ))
			वापस -EINVAL;
		ready_state =
			MEMBARRIER_STATE_PRIVATE_EXPEDITED_RSEQ_READY;
	} अन्यथा {
		WARN_ON_ONCE(flags);
	}

	/*
	 * We need to consider threads belonging to different thread
	 * groups, which use the same mm. (CLONE_VM but not
	 * CLONE_THREAD).
	 */
	अगर ((atomic_read(&mm->membarrier_state) & ready_state) == ready_state)
		वापस 0;
	अगर (flags & MEMBARRIER_FLAG_SYNC_CORE)
		set_state |= MEMBARRIER_STATE_PRIVATE_EXPEDITED_SYNC_CORE;
	अगर (flags & MEMBARRIER_FLAG_RSEQ)
		set_state |= MEMBARRIER_STATE_PRIVATE_EXPEDITED_RSEQ;
	atomic_or(set_state, &mm->membarrier_state);
	ret = sync_runqueues_membarrier_state(mm);
	अगर (ret)
		वापस ret;
	atomic_or(ready_state, &mm->membarrier_state);

	वापस 0;
}

अटल पूर्णांक membarrier_get_registrations(व्योम)
{
	काष्ठा task_struct *p = current;
	काष्ठा mm_struct *mm = p->mm;
	पूर्णांक registrations_mask = 0, membarrier_state, i;
	अटल स्थिर पूर्णांक states[] = {
		MEMBARRIER_STATE_GLOBAL_EXPEDITED |
			MEMBARRIER_STATE_GLOBAL_EXPEDITED_READY,
		MEMBARRIER_STATE_PRIVATE_EXPEDITED |
			MEMBARRIER_STATE_PRIVATE_EXPEDITED_READY,
		MEMBARRIER_STATE_PRIVATE_EXPEDITED_SYNC_CORE |
			MEMBARRIER_STATE_PRIVATE_EXPEDITED_SYNC_CORE_READY,
		MEMBARRIER_STATE_PRIVATE_EXPEDITED_RSEQ |
			MEMBARRIER_STATE_PRIVATE_EXPEDITED_RSEQ_READY
	};
	अटल स्थिर पूर्णांक registration_cmds[] = {
		MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED,
		MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED,
		MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_SYNC_CORE,
		MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_RSEQ
	};
	BUILD_BUG_ON(ARRAY_आकार(states) != ARRAY_आकार(registration_cmds));

	membarrier_state = atomic_read(&mm->membarrier_state);
	क्रम (i = 0; i < ARRAY_आकार(states); ++i) {
		अगर (membarrier_state & states[i]) {
			registrations_mask |= registration_cmds[i];
			membarrier_state &= ~states[i];
		}
	}
	WARN_ON_ONCE(membarrier_state != 0);
	वापस registrations_mask;
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
SYSCALL_DEFINE3(membarrier, पूर्णांक, cmd, अचिन्हित पूर्णांक, flags, पूर्णांक, cpu_id)
{
	चयन (cmd) {
	हाल MEMBARRIER_CMD_PRIVATE_EXPEDITED_RSEQ:
		अगर (unlikely(flags && flags != MEMBARRIER_CMD_FLAG_CPU))
			वापस -EINVAL;
		अवरोध;
	शेष:
		अगर (unlikely(flags))
			वापस -EINVAL;
	}

	अगर (!(flags & MEMBARRIER_CMD_FLAG_CPU))
		cpu_id = -1;

	चयन (cmd) {
	हाल MEMBARRIER_CMD_QUERY:
	{
		पूर्णांक cmd_mask = MEMBARRIER_CMD_BITMASK;

		अगर (tick_nohz_full_enabled())
			cmd_mask &= ~MEMBARRIER_CMD_GLOBAL;
		वापस cmd_mask;
	}
	हाल MEMBARRIER_CMD_GLOBAL:
		/* MEMBARRIER_CMD_GLOBAL is not compatible with nohz_full. */
		अगर (tick_nohz_full_enabled())
			वापस -EINVAL;
		अगर (अंक_online_cpus() > 1)
			synchronize_rcu();
		वापस 0;
	हाल MEMBARRIER_CMD_GLOBAL_EXPEDITED:
		वापस membarrier_global_expedited();
	हाल MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED:
		वापस membarrier_register_global_expedited();
	हाल MEMBARRIER_CMD_PRIVATE_EXPEDITED:
		वापस membarrier_private_expedited(0, cpu_id);
	हाल MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED:
		वापस membarrier_register_private_expedited(0);
	हाल MEMBARRIER_CMD_PRIVATE_EXPEDITED_SYNC_CORE:
		वापस membarrier_private_expedited(MEMBARRIER_FLAG_SYNC_CORE, cpu_id);
	हाल MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_SYNC_CORE:
		वापस membarrier_register_private_expedited(MEMBARRIER_FLAG_SYNC_CORE);
	हाल MEMBARRIER_CMD_PRIVATE_EXPEDITED_RSEQ:
		वापस membarrier_private_expedited(MEMBARRIER_FLAG_RSEQ, cpu_id);
	हाल MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_RSEQ:
		वापस membarrier_register_private_expedited(MEMBARRIER_FLAG_RSEQ);
	हाल MEMBARRIER_CMD_GET_REGISTRATIONS:
		वापस membarrier_get_registrations();
	शेष:
		वापस -EINVAL;
	}
}
