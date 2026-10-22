// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#समावेश <linux/sched/clock.h>
#समावेश <linux/sched/cputime.h>
#समावेश <linux/tsacct_kern.h>
#समावेश "sched.h"

#अगर_घोषित CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #समावेश <asm/cputime.h>
#पूर्ण_अगर

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING

DEFINE_STATIC_कुंजी_FALSE(sched_clock_irqtime);

/*
 * There are no locks covering percpu hardirq/softirq time.
 * They are only modified in vtime_account, on corresponding CPU
 * with interrupts disabled. So, writes are safe.
 * They are read and saved off onto struct rq in update_rq_clock().
 * This may result in other CPU reading this CPU's IRQ time and can
 * race with irq/vtime_account on this CPU. We would either get old
 * or new value with a side effect of accounting a slice of IRQ time to wrong
 * task when IRQ is in progress while we read rq->clock. That is a worthy
 * compromise in place of having locks on each IRQ in account_system_time.
 */
DEFINE_PER_CPU(काष्ठा irqtime, cpu_irqtime);

व्योम enable_sched_clock_irqtime(व्योम)
{
	static_branch_enable(&sched_clock_irqtime);
}

व्योम disable_sched_clock_irqtime(व्योम)
{
	अगर (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

अटल व्योम irqtime_account_delta(काष्ठा irqtime *irqtime, u64 delta,
				  क्रमागत cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->योग_2 += delta;
	अगर (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_end(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
व्योम irqtime_account_irq(काष्ठा task_struct *curr, अचिन्हित पूर्णांक offset)
{
	काष्ठा irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	अचिन्हित पूर्णांक pc;
	s64 delta;
	पूर्णांक cpu;

	अगर (!irqtime_enabled())
		वापस;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_start_time;
	irqtime->irq_start_time += delta;
	pc = irq_गिनती() - offset;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	अगर (pc & HARDIRQ_MASK)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	अन्यथा अगर ((pc & SOFTIRQ_OFFSET) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

अटल u64 irqtime_tick_accounted(u64 maxtime)
{
	काष्ठा irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	वापस delta;
}

#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */

अटल u64 irqtime_tick_accounted(u64 dummy)
{
	वापस 0;
}

#पूर्ण_अगर /* !CONFIG_IRQ_TIME_ACCOUNTING */

अटल अंतरभूत व्योम task_group_account_field(काष्ठा task_struct *p, पूर्णांक सूचकांक,
					    u64 tmp)
{
	/*
	 * Since all updates are sure to touch the root cgroup, we
	 * get ourselves ahead and touch it first. If the root cgroup
	 * is the only cgroup, then nothing else should be necessary.
	 *
	 */
	__this_cpu_add(kernel_cpustat.cpustat[सूचकांक], tmp);

	cgroup_account_cputime_field(p, सूचकांक, tmp);
}

/*
 * Account user CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in user space since the last update
 */
व्योम account_user_time(काष्ठा task_struct *p, u64 cputime)
{
	पूर्णांक सूचकांक;

	/* Add user time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);

	सूचकांक = (task_nice(p) > 0) ? CPUTIME_NICE : CPUTIME_USER;

	/* Add user time to cpustat. */
	task_group_account_field(p, सूचकांक, cputime);

	/* Account for user time used */
	acct_account_cputime(p);
}

/*
 * Account guest CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in virtual machine since the last update
 */
व्योम account_guest_time(काष्ठा task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	अगर (task_nice(p) > 0) {
		task_group_account_field(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} अन्यथा {
		task_group_account_field(p, CPUTIME_USER, cputime);
		cpustat[CPUTIME_GUEST] += cputime;
	}
}

/*
 * Account system CPU time to a process and desired cpustat field
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in kernel space since the last update
 * @index: pointer to cpustat field that has to be updated
 */
व्योम account_system_सूचकांक_time(काष्ठा task_struct *p,
			       u64 cputime, क्रमागत cpu_usage_stat सूचकांक)
{
	/* Add system time to process. */
	p->stime += cputime;
	account_group_system_time(p, cputime);

	/* Add system time to cpustat. */
	task_group_account_field(p, सूचकांक, cputime);

	/* Account for system time used */
	acct_account_cputime(p);
}

/*
 * Account system CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @hardirq_offset: the offset to subtract from hardirq_count()
 * @cputime: the CPU time spent in kernel space since the last update
 */
व्योम account_system_time(काष्ठा task_struct *p, पूर्णांक hardirq_offset, u64 cputime)
{
	पूर्णांक सूचकांक;

	अगर ((p->flags & PF_VCPU) && (irq_गिनती() - hardirq_offset == 0)) {
		account_guest_time(p, cputime);
		वापस;
	}

	अगर (hardirq_गिनती() - hardirq_offset)
		सूचकांक = CPUTIME_IRQ;
	अन्यथा अगर (in_serving_softirq())
		सूचकांक = CPUTIME_SOFTIRQ;
	अन्यथा
		सूचकांक = CPUTIME_SYSTEM;

	account_system_सूचकांक_time(p, cputime, सूचकांक);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
व्योम account_steal_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
व्योम account_idle_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	अगर (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	अन्यथा
		cpustat[CPUTIME_IDLE] += cputime;
}


#अगर_घोषित CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
व्योम __account_forceidle_time(काष्ठा task_struct *p, u64 delta)
{
	__schedstat_add(p->stats.core_forceidle_योग, delta);

	task_group_account_field(p, CPUTIME_FORCEIDLE, delta);
}
#पूर्ण_अगर /* CONFIG_SCHED_CORE */

/*
 * When a guest is interrupted for a longer amount of time, missed clock
 * ticks are not redelivered later. Due to that, this function may on
 * occasion account more time than the calling functions think elapsed.
 */
#अगर_घोषित CONFIG_PARAVIRT
DEFINE_STATIC_कुंजी_FALSE(paravirt_steal_enabled);

#अगर_घोषित CONFIG_HAVE_PV_STEAL_CLOCK_GEN
अटल u64 native_steal_clock(पूर्णांक cpu)
{
	वापस 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#पूर्ण_अगर
#पूर्ण_अगर

अटल __always_inline u64 steal_account_process_time(u64 maxtime)
{
#अगर_घोषित CONFIG_PARAVIRT
	अगर (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->पूर्व_steal_time;
		steal = min(steal, maxtime);
		account_steal_time(steal);
		this_rq()->पूर्व_steal_time += steal;

		वापस steal;
	}
#पूर्ण_अगर /* CONFIG_PARAVIRT */
	वापस 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
अटल अंतरभूत u64 account_other_time(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_time(max);

	अगर (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	वापस accounted;
}

#अगर_घोषित CONFIG_64BIT
अटल अंतरभूत u64 read_योग_exec_runtime(काष्ठा task_struct *t)
{
	वापस t->se.योग_exec_runtime;
}
#अन्यथा /* !CONFIG_64BIT: */
अटल u64 read_योग_exec_runtime(काष्ठा task_struct *t)
{
	u64 ns;
	काष्ठा rq_flags rf;
	काष्ठा rq *rq;

	rq = task_rq_lock(t, &rf);
	ns = t->se.योग_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	वापस ns;
}
#पूर्ण_अगर /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
व्योम thread_group_cputime(काष्ठा task_struct *tsk, काष्ठा task_cputime *बार)
{
	काष्ठा signal_struct *sig = tsk->संकेत;
	काष्ठा task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	अगर (same_thread_group(current, tsk))
		(व्योम) task_sched_runtime(current);

	guard(rcu)();
	scoped_seqlock_read (&sig->stats_lock, ss_lock_irqsave) {
		बार->utime = sig->utime;
		बार->stime = sig->stime;
		बार->योग_exec_runtime = sig->योग_sched_runtime;

		__for_each_thread(sig, t) {
			task_cputime(t, &utime, &stime);
			बार->utime += utime;
			बार->stime += stime;
			बार->योग_exec_runtime += read_योग_exec_runtime(t);
		}
	}
}

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
/*
 * Account a tick to a process and cpustat
 * @p: the process that the CPU time gets accounted to
 * @user_tick: is the tick from userspace
 * @rq: the pointer to rq
 *
 * Tick demultiplexing follows the order
 * - pending hardirq update
 * - pending softirq update
 * - user_time
 * - idle_time
 * - system time
 *   - check for guest_time
 *   - else account as system_time
 *
 * Check for hardirq is done both for system and user time as there is
 * no timer going off while we are on hardirq and hence we may never get an
 * opportunity to update it solely in system time.
 * p->stime and friends are only updated on system time and not on IRQ
 * softirq as those do not count in task exec_runtime any more.
 */
अटल व्योम irqtime_account_process_tick(काष्ठा task_struct *p, पूर्णांक user_tick,
					 u64 cputime)
{
	u64 other;

	/*
	 * When returning from idle, many ticks can get accounted at
	 * once, including some ticks of steal, IRQ, and softirq time.
	 * Subtract those ticks from the amount of time accounted to
	 * idle, or potentially user or system time. Due to rounding,
	 * other time can exceed ticks occasionally.
	 */
	other = account_other_time(अच_दीर्घ_उच्च);
	अगर (other >= cputime)
		वापस;

	cputime -= other;

	अगर (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_सूचकांक_time(p, cputime, CPUTIME_SOFTIRQ);
	} अन्यथा अगर (user_tick) {
		account_user_time(p, cputime);
	} अन्यथा अगर (p == this_rq()->idle) {
		account_idle_time(cputime);
	} अन्यथा अगर (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_time(p, cputime);
	} अन्यथा {
		account_system_सूचकांक_time(p, cputime, CPUTIME_SYSTEM);
	}
}

#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */
अटल अंतरभूत व्योम irqtime_account_process_tick(काष्ठा task_struct *p, पूर्णांक user_tick,
						u64 cputime) { }
#पूर्ण_अगर /* !CONFIG_IRQ_TIME_ACCOUNTING */

#अगर defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
अटल व्योम kcpustat_idle_stop(काष्ठा kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	पूर्णांक iowait;

	अगर (!kc->idle_elapse)
		वापस;

	iowait = cpu_has_iowait(smp_processor_id());
	delta = now - kc->idle_entrytime;
	steal = steal_account_process_time(delta);

	/*
	 * Record the idle time after substracting the steal time from
	 * previous update sequence. Don't substract the steal time from
	 * the current update sequence to avoid readers moving backward.
	 */
	write_seqcount_begin(&kc->idle_sleeptime_seq);
	steal_delta = min_t(u64, kc->idle_stealtime[iowait], delta);
	delta -= steal_delta;
	kc->idle_stealtime[iowait] -= steal_delta;

	अगर (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	अन्यथा
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

अटल व्योम kcpustat_idle_start(काष्ठा kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	अगर (kc->idle_elapse)
		वापस;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

व्योम kcpustat_dyntick_stop(u64 now, u64 tick_start)
{
	काष्ठा kernel_cpustat *kc = kcpustat_this_cpu;

	अगर (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		अगर (kc->idle_tick_period != tick_start) {
			kc->idle_tick_period = tick_start;
			kc->idle_tick_overlap = 0;
		}
		tick_start = max(tick_start, kc->idle_dyntick_entry);
		अगर (!WARN_ON_ONCE(now < tick_start))
			kc->idle_tick_overlap += now - tick_start;
		kcpustat_idle_stop(kc, now);
		kc->idle_dyntick = false;
		vtime_dyntick_stop();
	}
}

/*
 * The first tick after dyntick-idle covers a period that the dyntick-idle
 * accounting may already have accounted in part.
 */
अटल अंतरभूत u64 kcpustat_tick_overlap(व्योम)
{
	काष्ठा kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	वापस overlap;
}

व्योम kcpustat_dyntick_start(u64 now)
{
	काष्ठा kernel_cpustat *kc = kcpustat_this_cpu;

	अगर (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_start();
		kc->idle_dyntick = true;
		kc->idle_dyntick_entry = now;
		kcpustat_idle_start(kc, now);
	}
}

व्योम kcpustat_irq_enter(u64 now)
{
	काष्ठा kernel_cpustat *kc = kcpustat_this_cpu;

	अगर (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

व्योम kcpustat_irq_exit(u64 now)
{
	काष्ठा kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	अगर (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_start(kc, now);
}

अटल u64 kcpustat_field_dyntick(पूर्णांक cpu, क्रमागत cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	काष्ठा kernel_cpustat *kc = &kcpustat_cpu(cpu);
	पूर्णांक iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	अचिन्हित पूर्णांक seq;
	u64 idle;

	करो {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		अगर (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} जबतक (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	वापस idle;
}

u64 kcpustat_field_idle(पूर्णांक cpu)
{
	वापस kcpustat_field_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_प्रतीक_GPL(kcpustat_field_idle);

u64 kcpustat_field_iowait(पूर्णांक cpu)
{
	वापस kcpustat_field_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_प्रतीक_GPL(kcpustat_field_iowait);
#अन्यथा
अटल अंतरभूत u64 kcpustat_tick_overlap(व्योम)
{
	वापस 0;
}

अटल u64 kcpustat_field_dyntick(पूर्णांक cpu, क्रमागत cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	वापस kcpustat_cpu(cpu).cpustat[idx];
}
#पूर्ण_अगर /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

अटल u64 get_cpu_sleep_time_us(पूर्णांक cpu, क्रमागत cpu_usage_stat idx,
				 bool compute_delta, u64 *last_update_time)
{
	ktime_t now = ktime_get();
	u64 res;

	अगर (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_field(idx, cpu);
	अन्यथा
		res = kcpustat_field_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_PER_USEC);

	अगर (last_update_time)
		*last_update_time = ktime_to_us(now);

	वापस res;
}

/**
 * get_cpu_idle_time_us - get the total idle time of a CPU
 * @cpu: CPU number to query
 * @last_update_time: variable to store update time in. Do not update
 * counters if NULL.
 *
 * Return the cumulative idle time (since boot) for a given
 * CPU, in microseconds. Note that this is partially broken due to
 * the counter of iowait tasks that can be remotely updated without
 * any synchronization. Therefore it is possible to observe backward
 * values within two consecutive reads.
 *
 * This time is measured via accounting rather than sampling,
 * and is as accurate as ktime_get() is.
 *
 * Return: total idle time of the @cpu
 */
u64 get_cpu_idle_time_us(पूर्णांक cpu, u64 *last_update_time)
{
	वापस get_cpu_sleep_time_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), last_update_time);
}
EXPORT_प्रतीक_GPL(get_cpu_idle_time_us);

/**
 * get_cpu_iowait_time_us - get the total iowait time of a CPU
 * @cpu: CPU number to query
 * @last_update_time: variable to store update time in. Do not update
 * counters if NULL.
 *
 * Return the cumulative iowait time (since boot) for a given
 * CPU, in microseconds. Note this is partially broken due to
 * the counter of iowait tasks that can be remotely updated without
 * any synchronization. Therefore it is possible to observe backward
 * values within two consecutive reads.
 *
 * This time is measured via accounting rather than sampling,
 * and is as accurate as ktime_get() is.
 *
 * Return: total iowait time of @cpu
 */
u64 get_cpu_iowait_time_us(पूर्णांक cpu, u64 *last_update_time)
{
	वापस get_cpu_sleep_time_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), last_update_time);
}
EXPORT_प्रतीक_GPL(get_cpu_iowait_time_us);

/*
 * Use precise platform statistics if available:
 */
#अगर_घोषित CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

व्योम vtime_account_irq(काष्ठा task_struct *tsk, अचिन्हित पूर्णांक offset)
{
	अचिन्हित पूर्णांक pc = irq_गिनती() - offset;

	अगर (pc & HARDIRQ_OFFSET) {
		vtime_account_hardirq(tsk);
	} अन्यथा अगर (pc & SOFTIRQ_OFFSET) {
		vtime_account_softirq(tsk);
	} अन्यथा अगर (!kcpustat_idle_dyntick()) {
		अगर (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} अन्यथा {
			vtime_account_kernel(tsk);
		}
	} अन्यथा {
		vtime_reset();
	}
}

व्योम cputime_adjust(काष्ठा task_cputime *curr, काष्ठा पूर्व_cputime *पूर्व,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

व्योम task_cputime_adjusted(काष्ठा task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_प्रतीक_GPL(task_cputime_adjusted);

व्योम thread_group_cputime_adjusted(काष्ठा task_struct *p, u64 *ut, u64 *st)
{
	काष्ठा task_cputime cputime;

	thread_group_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#अन्यथा /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
व्योम account_process_tick(काष्ठा task_struct *p, पूर्णांक user_tick)
{
	u64 cputime, steal;

	अगर (vtime_accounting_enabled_this_cpu())
		वापस;

	अगर (kcpustat_idle_dyntick())
		वापस;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	अगर (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		वापस;
	}

	steal = steal_account_process_time(अच_दीर्घ_उच्च);

	अगर (steal >= cputime)
		वापस;

	cputime -= steal;

	अगर (user_tick)
		account_user_time(p, cputime);
	अन्यथा अगर ((p != this_rq()->idle) || (irq_गिनती() != HARDIRQ_OFFSET))
		account_system_time(p, HARDIRQ_OFFSET, cputime);
	अन्यथा
		account_idle_time(cputime);
}

/*
 * Adjust tick based cputime random precision against scheduler runtime
 * accounting.
 *
 * Tick based cputime accounting depend on random scheduling timeslices of a
 * task to be interrupted or not by the timer.  Depending on these
 * circumstances, the number of these interrupts may be over or
 * under-optimistic, matching the real user and system cputime with a variable
 * precision.
 *
 * Fix this by scaling these tick based values against the total runtime
 * accounted by the CFS scheduler.
 *
 * This code provides the following guarantees:
 *
 *   stime + utime == rtime
 *   stime_i+1 >= stime_i, utime_i+1 >= utime_i
 *
 * Assuming that rtime_i+1 >= rtime_i.
 */
व्योम cputime_adjust(काष्ठा task_cputime *curr, काष्ठा पूर्व_cputime *पूर्व,
		    u64 *ut, u64 *st)
{
	u64 rtime, stime, utime;
	अचिन्हित दीर्घ flags;

	/* Serialize concurrent callers such that we can honour our guarantees */
	raw_spin_lock_irqsave(&पूर्व->lock, flags);
	rtime = curr->योग_exec_runtime;

	/*
	 * This is possible under two circumstances:
	 *  - rtime isn't monotonic after all (a bug);
	 *  - we got reordered by the lock.
	 *
	 * In both cases this acts as a filter such that the rest of the code
	 * can assume it is monotonic regardless of anything else.
	 */
	अगर (पूर्व->stime + पूर्व->utime >= rtime)
		जाओ out;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	अगर (stime == 0) {
		utime = rtime;
		जाओ update;
	}

	अगर (utime == 0) {
		stime = rtime;
		जाओ update;
	}

	stime = mul_u64_u64_div_u64(stime, rtime, stime + utime);

update:
	/*
	 * Make sure stime doesn't go backwards; this preserves monotonicity
	 * for utime because rtime is monotonic.
	 *
	 *  utime_i+1 = rtime_i+1 - stime_i
	 *            = rtime_i+1 - (rtime_i - utime_i)
	 *            = (rtime_i+1 - rtime_i) + utime_i
	 *            >= utime_i
	 */
	अगर (stime < पूर्व->stime)
		stime = पूर्व->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	अगर (utime < पूर्व->utime) {
		utime = पूर्व->utime;
		stime = rtime - utime;
	}

	पूर्व->stime = stime;
	पूर्व->utime = utime;
out:
	*ut = पूर्व->utime;
	*st = पूर्व->stime;
	raw_spin_unlock_irqrestore(&पूर्व->lock, flags);
}

व्योम task_cputime_adjusted(काष्ठा task_struct *p, u64 *ut, u64 *st)
{
	काष्ठा task_cputime cputime = {
		.योग_exec_runtime = p->se.योग_exec_runtime,
	};

	अगर (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.योग_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->पूर्व_cputime, ut, st);
}
EXPORT_प्रतीक_GPL(task_cputime_adjusted);

व्योम thread_group_cputime_adjusted(काष्ठा task_struct *p, u64 *ut, u64 *st)
{
	काष्ठा task_cputime cputime;

	thread_group_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->संकेत->पूर्व_cputime, ut, st);
}
#पूर्ण_अगर /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#अगर_घोषित CONFIG_VIRT_CPU_ACCOUNTING_GEN
अटल u64 vtime_delta(काष्ठा vtime *vtime)
{
	अचिन्हित दीर्घ दीर्घ घड़ी;

	घड़ी = sched_clock();
	अगर (घड़ी < vtime->starttime)
		वापस 0;

	वापस घड़ी - vtime->starttime;
}

अटल u64 get_vtime_delta(काष्ठा vtime *vtime)
{
	u64 delta = vtime_delta(vtime);
	u64 other;

	/*
	 * Unlike tick based timing, vtime based timing never has lost
	 * ticks, and no need for steal time accounting to make up for
	 * lost ticks. Vtime accounts a rounded version of actual
	 * elapsed time. Limit account_other_time to prevent rounding
	 * errors from causing elapsed vtime to go negative.
	 */
	other = account_other_time(delta);
	WARN_ON_ONCE(vtime->state == VTIME_INACTIVE);
	vtime->starttime += delta;

	वापस delta - other;
}

अटल व्योम vtime_account_system(काष्ठा task_struct *tsk,
				 काष्ठा vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	अगर (vtime->stime >= TICK_NSEC) {
		account_system_time(tsk, irq_गिनती(), vtime->stime);
		vtime->stime = 0;
	}
}

अटल व्योम vtime_account_guest(काष्ठा task_struct *tsk,
				काष्ठा vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	अगर (vtime->gtime >= TICK_NSEC) {
		account_guest_time(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

अटल व्योम __vtime_account_kernel(काष्ठा task_struct *tsk,
				   काष्ठा vtime *vtime)
{
	/* We might have scheduled out from guest path */
	अगर (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	अन्यथा
		vtime_account_system(tsk, vtime);
}

व्योम vtime_account_kernel(काष्ठा task_struct *tsk)
{
	काष्ठा vtime *vtime = &tsk->vtime;

	अगर (!vtime_delta(vtime))
		वापस;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_end(&vtime->seqcount);
}

व्योम vtime_user_enter(काष्ठा task_struct *tsk)
{
	काष्ठा vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_end(&vtime->seqcount);
}

व्योम vtime_user_exit(काष्ठा task_struct *tsk)
{
	काष्ठा vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	अगर (vtime->utime >= TICK_NSEC) {
		account_user_time(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}

व्योम vtime_guest_enter(काष्ठा task_struct *tsk)
{
	काष्ठा vtime *vtime = &tsk->vtime;
	/*
	 * The flags must be updated under the lock with
	 * the vtime_starttime flush and update.
	 * That enforces a right ordering and update sequence
	 * synchronization against the reader (task_gtime())
	 * that can thus safely catch up with a tickless delta.
	 */
	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	tsk->flags |= PF_VCPU;
	vtime->state = VTIME_GUEST;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_प्रतीक_GPL(vtime_guest_enter);

व्योम vtime_guest_exit(काष्ठा task_struct *tsk)
{
	काष्ठा vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_प्रतीक_GPL(vtime_guest_exit);

अटल व्योम __vtime_account_idle(काष्ठा vtime *vtime)
{
	account_idle_time(get_vtime_delta(vtime));
}

व्योम vtime_task_switch_generic(काष्ठा task_struct *पूर्व)
{
	काष्ठा vtime *vtime = &पूर्व->vtime;

	write_seqcount_begin(&vtime->seqcount);
	अगर (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	अन्यथा
		__vtime_account_kernel(पूर्व, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_end(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	अगर (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	अन्यथा अगर (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	अन्यथा
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_end(&vtime->seqcount);
}

व्योम vtime_init_idle(काष्ठा task_struct *t, पूर्णांक cpu)
{
	काष्ठा vtime *vtime = &t->vtime;
	अचिन्हित दीर्घ flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_end(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(काष्ठा task_struct *t)
{
	काष्ठा vtime *vtime = &t->vtime;
	अचिन्हित पूर्णांक seq;
	u64 gtime;

	अगर (!vtime_accounting_enabled())
		वापस t->gtime;

	करो {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		अगर (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} जबतक (read_seqcount_retry(&vtime->seqcount, seq));

	वापस gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(काष्ठा task_struct *t, u64 *utime, u64 *stime)
{
	काष्ठा vtime *vtime = &t->vtime;
	अचिन्हित पूर्णांक seq;
	u64 delta;
	पूर्णांक ret;

	अगर (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		वापस false;
	}

	करो {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		अगर (vtime->state < VTIME_SYS)
			जारी;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		अगर (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		अन्यथा
			*utime += vtime->utime + delta;
	} जबतक (read_seqcount_retry(&vtime->seqcount, seq));

	वापस ret;
}

अटल पूर्णांक vtime_state_fetch(काष्ठा vtime *vtime, पूर्णांक cpu)
{
	पूर्णांक state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	अगर (vtime->cpu != cpu && vtime->cpu != -1)
		वापस -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	अगर (state == VTIME_INACTIVE)
		वापस -EAGAIN;

	वापस state;
}

अटल u64 kcpustat_user_vtime(काष्ठा vtime *vtime)
{
	अगर (vtime->state == VTIME_USER)
		वापस vtime->utime + vtime_delta(vtime);
	अन्यथा अगर (vtime->state == VTIME_GUEST)
		वापस vtime->gtime + vtime_delta(vtime);
	वापस 0;
}

अटल पूर्णांक kcpustat_field_vtime(u64 *cpustat,
				काष्ठा task_struct *tsk,
				क्रमागत cpu_usage_stat usage,
				पूर्णांक cpu, u64 *val)
{
	काष्ठा vtime *vtime = &tsk->vtime;
	अचिन्हित पूर्णांक seq;

	करो {
		पूर्णांक state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		अगर (state < 0)
			वापस state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		चयन (usage) {
		हाल CPUTIME_SYSTEM:
			अगर (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			अवरोध;
		हाल CPUTIME_USER:
			अगर (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			अवरोध;
		हाल CPUTIME_NICE:
			अगर (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			अवरोध;
		हाल CPUTIME_GUEST:
			अगर (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			अवरोध;
		हाल CPUTIME_GUEST_NICE:
			अगर (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			अवरोध;
		हाल CPUTIME_IDLE:
			अगर (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			अवरोध;
		हाल CPUTIME_IOWAIT:
			अगर (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			अवरोध;
		शेष:
			अवरोध;
		}
	} जबतक (read_seqcount_retry(&vtime->seqcount, seq));

	वापस 0;
}

u64 kcpustat_field(क्रमागत cpu_usage_stat usage, पूर्णांक cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	काष्ठा rq *rq;
	पूर्णांक err;

	अगर (!vtime_generic_enabled_cpu(cpu))
		वापस kcpustat_field_default(usage, cpu);

	rq = cpu_rq(cpu);

	क्रम (;;) {
		काष्ठा task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		अगर (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			वापस cpustat[usage];
		}

		err = kcpustat_field_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		अगर (!err)
			वापस val;

		cpu_relax();
	}
}
EXPORT_प्रतीक_GPL(kcpustat_field);

अटल पूर्णांक kcpustat_cpu_fetch_vtime(काष्ठा kernel_cpustat *dst,
				    स्थिर काष्ठा kernel_cpustat *src,
				    काष्ठा task_struct *tsk, पूर्णांक cpu)
{
	काष्ठा vtime *vtime = &tsk->vtime;
	अचिन्हित पूर्णांक seq;

	करो {
		u64 *cpustat;
		u64 delta;
		पूर्णांक state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		अगर (state < 0)
			वापस state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		अगर (state < VTIME_IDLE)
			जारी;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		चयन (state) {
		हाल VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			अवरोध;
		हाल VTIME_USER:
			अगर (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			अन्यथा
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			अवरोध;
		हाल VTIME_GUEST:
			अगर (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} अन्यथा {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			अवरोध;
		हाल VTIME_IDLE:
			अगर (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			अन्यथा
				cpustat[CPUTIME_IDLE] += delta;
			अवरोध;
		शेष:
			WARN_ON_ONCE(1);
		}
	} जबतक (read_seqcount_retry(&vtime->seqcount, seq));

	वापस 0;
}

व्योम kcpustat_cpu_fetch(काष्ठा kernel_cpustat *dst, पूर्णांक cpu)
{
	स्थिर काष्ठा kernel_cpustat *src = &kcpustat_cpu(cpu);
	काष्ठा rq *rq;
	पूर्णांक err;

	अगर (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		वापस;
	}

	rq = cpu_rq(cpu);

	क्रम (;;) {
		काष्ठा task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		अगर (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			वापस;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		अगर (!err)
			वापस;

		cpu_relax();
	}
}
EXPORT_प्रतीक_GPL(kcpustat_cpu_fetch);

#पूर्ण_अगर /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
