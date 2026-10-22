// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#অন্তর্ভাব <linux/sched/clock.h>
#অন্তর্ভাব <linux/sched/cputime.h>
#অন্তর্ভাব <linux/tsacct_kern.h>
#অন্তর্ভাব "sched.h"

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #অন্তর্ভাব <asm/cputime.h>
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

DEFINE_STATIC_চাবি_FALSE(sched_clock_irqtime);

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
DEFINE_PER_CPU(সংরচনা irqtime, cpu_irqtime);

শূন্য enable_sched_clock_irqtime(শূন্য)
{
	static_branch_enable(&sched_clock_irqtime);
}

শূন্য disable_sched_clock_irqtime(শূন্য)
{
	যদি (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

স্থির শূন্য irqtime_account_delta(সংরচনা irqtime *irqtime, u64 delta,
				  গণনা cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->total += delta;
	যদি (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_end(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
শূন্য irqtime_account_irq(সংরচনা task_struct *curr, unsigned পূর্ণাঙ্ক offset)
{
	সংরচনা irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	unsigned পূর্ণাঙ্ক pc;
	s64 delta;
	পূর্ণাঙ্ক cpu;

	যদি (!irqtime_enabled())
		প্রত্যাগম;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_start_time;
	irqtime->irq_start_time += delta;
	pc = irq_গুনতি() - offset;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	যদি (pc & HARDIRQ_MASK)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	অন্যথা যদি ((pc & SOFTIRQ_OFFSET) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

স্থির u64 irqtime_tick_accounted(u64 maxtime)
{
	সংরচনা irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	প্রত্যাগম delta;
}

#অন্যথা /* !CONFIG_IRQ_TIME_ACCOUNTING: */

স্থির u64 irqtime_tick_accounted(u64 dummy)
{
	প্রত্যাগম 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

স্থির অন্তঃস্থ শূন্য task_group_account_field(সংরচনা task_struct *p, পূর্ণাঙ্ক index,
					    u64 tmp)
{
	/*
	 * Since all updates are sure to touch the root cgroup, we
	 * get ourselves ahead and touch it first. If the root cgroup
	 * is the only cgroup, then nothing else should be necessary.
	 *
	 */
	__this_cpu_add(kernel_cpustat.cpustat[index], tmp);

	cgroup_account_cputime_field(p, index, tmp);
}

/*
 * Account user CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in user space since the last update
 */
শূন্য account_user_time(সংরচনা task_struct *p, u64 cputime)
{
	পূর্ণাঙ্ক index;

	/* Add user time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);

	index = (task_nice(p) > 0) ? CPUTIME_NICE : CPUTIME_USER;

	/* Add user time to cpustat. */
	task_group_account_field(p, index, cputime);

	/* Account for user time used */
	acct_account_cputime(p);
}

/*
 * Account guest CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in virtual machine since the last update
 */
শূন্য account_guest_time(সংরচনা task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	যদি (task_nice(p) > 0) {
		task_group_account_field(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} অন্যথা {
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
শূন্য account_system_index_time(সংরচনা task_struct *p,
			       u64 cputime, গণনা cpu_usage_stat index)
{
	/* Add system time to process. */
	p->stime += cputime;
	account_group_system_time(p, cputime);

	/* Add system time to cpustat. */
	task_group_account_field(p, index, cputime);

	/* Account for system time used */
	acct_account_cputime(p);
}

/*
 * Account system CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @hardirq_offset: the offset to subtract from hardirq_count()
 * @cputime: the CPU time spent in kernel space since the last update
 */
শূন্য account_system_time(সংরচনা task_struct *p, পূর্ণাঙ্ক hardirq_offset, u64 cputime)
{
	পূর্ণাঙ্ক index;

	যদি ((p->flags & PF_VCPU) && (irq_গুনতি() - hardirq_offset == 0)) {
		account_guest_time(p, cputime);
		প্রত্যাগম;
	}

	যদি (hardirq_গুনতি() - hardirq_offset)
		index = CPUTIME_IRQ;
	অন্যথা যদি (in_serving_softirq())
		index = CPUTIME_SOFTIRQ;
	অন্যথা
		index = CPUTIME_SYSTEM;

	account_system_index_time(p, cputime, index);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
শূন্য account_steal_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
শূন্য account_idle_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	যদি (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	অন্যথা
		cpustat[CPUTIME_IDLE] += cputime;
}


#ifdef CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
শূন্য __account_forceidle_time(সংরচনা task_struct *p, u64 delta)
{
	__schedstat_add(p->stats.core_forceidle_sum, delta);

	task_group_account_field(p, CPUTIME_FORCEIDLE, delta);
}
#endif /* CONFIG_SCHED_CORE */

/*
 * When a guest is interrupted for a longer amount of time, missed clock
 * ticks are not redelivered later. Due to that, this function may on
 * occasion account more time than the calling functions think elapsed.
 */
#ifdef CONFIG_PARAVIRT
DEFINE_STATIC_চাবি_FALSE(paravirt_steal_enabled);

#ifdef CONFIG_HAVE_PV_STEAL_CLOCK_GEN
স্থির u64 native_steal_clock(পূর্ণাঙ্ক cpu)
{
	প্রত্যাগম 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#endif
#endif

স্থির __always_inline u64 steal_account_process_time(u64 maxtime)
{
#ifdef CONFIG_PARAVIRT
	যদি (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_time;
		steal = min(steal, maxtime);
		account_steal_time(steal);
		this_rq()->prev_steal_time += steal;

		প্রত্যাগম steal;
	}
#endif /* CONFIG_PARAVIRT */
	প্রত্যাগম 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
স্থির অন্তঃস্থ u64 account_other_time(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_time(max);

	যদি (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	প্রত্যাগম accounted;
}

#ifdef CONFIG_64BIT
স্থির অন্তঃস্থ u64 read_sum_exec_runtime(সংরচনা task_struct *t)
{
	প্রত্যাগম t->se.sum_exec_runtime;
}
#অন্যথা /* !CONFIG_64BIT: */
স্থির u64 read_sum_exec_runtime(সংরচনা task_struct *t)
{
	u64 ns;
	সংরচনা rq_flags rf;
	সংরচনা rq *rq;

	rq = task_rq_lock(t, &rf);
	ns = t->se.sum_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	প্রত্যাগম ns;
}
#endif /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
শূন্য thread_group_cputime(সংরচনা task_struct *tsk, সংরচনা task_cputime *times)
{
	সংরচনা signal_struct *sig = tsk->সঙ্কেত;
	সংরচনা task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	যদি (same_thread_group(current, tsk))
		(শূন্য) task_sched_runtime(current);

	guard(rcu)();
	scoped_seqlock_read (&sig->stats_lock, ss_lock_irqsave) {
		times->utime = sig->utime;
		times->stime = sig->stime;
		times->sum_exec_runtime = sig->sum_sched_runtime;

		__for_each_thread(sig, t) {
			task_cputime(t, &utime, &stime);
			times->utime += utime;
			times->stime += stime;
			times->sum_exec_runtime += read_sum_exec_runtime(t);
		}
	}
}

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
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
স্থির শূন্য irqtime_account_process_tick(সংরচনা task_struct *p, পূর্ণাঙ্ক user_tick,
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
	other = account_other_time(ULONG_MAX);
	যদি (other >= cputime)
		প্রত্যাগম;

	cputime -= other;

	যদি (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_index_time(p, cputime, CPUTIME_SOFTIRQ);
	} অন্যথা যদি (user_tick) {
		account_user_time(p, cputime);
	} অন্যথা যদি (p == this_rq()->idle) {
		account_idle_time(cputime);
	} অন্যথা যদি (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_time(p, cputime);
	} অন্যথা {
		account_system_index_time(p, cputime, CPUTIME_SYSTEM);
	}
}

#অন্যথা /* !CONFIG_IRQ_TIME_ACCOUNTING: */
স্থির অন্তঃস্থ শূন্য irqtime_account_process_tick(সংরচনা task_struct *p, পূর্ণাঙ্ক user_tick,
						u64 cputime) { }
#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#যদি defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
স্থির শূন্য kcpustat_idle_stop(সংরচনা kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	পূর্ণাঙ্ক iowait;

	যদি (!kc->idle_elapse)
		প্রত্যাগম;

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

	যদি (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	অন্যথা
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

স্থির শূন্য kcpustat_idle_start(সংরচনা kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	যদি (kc->idle_elapse)
		প্রত্যাগম;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

শূন্য kcpustat_dyntick_stop(u64 now, u64 tick_start)
{
	সংরচনা kernel_cpustat *kc = kcpustat_this_cpu;

	যদি (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		যদি (kc->idle_tick_period != tick_start) {
			kc->idle_tick_period = tick_start;
			kc->idle_tick_overlap = 0;
		}
		tick_start = max(tick_start, kc->idle_dyntick_entry);
		যদি (!WARN_ON_ONCE(now < tick_start))
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
স্থির অন্তঃস্থ u64 kcpustat_tick_overlap(শূন্য)
{
	সংরচনা kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	প্রত্যাগম overlap;
}

শূন্য kcpustat_dyntick_start(u64 now)
{
	সংরচনা kernel_cpustat *kc = kcpustat_this_cpu;

	যদি (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_start();
		kc->idle_dyntick = true;
		kc->idle_dyntick_entry = now;
		kcpustat_idle_start(kc, now);
	}
}

শূন্য kcpustat_irq_enter(u64 now)
{
	সংরচনা kernel_cpustat *kc = kcpustat_this_cpu;

	যদি (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

শূন্য kcpustat_irq_exit(u64 now)
{
	সংরচনা kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	যদি (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_start(kc, now);
}

স্থির u64 kcpustat_field_dyntick(পূর্ণাঙ্ক cpu, গণনা cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	সংরচনা kernel_cpustat *kc = &kcpustat_cpu(cpu);
	পূর্ণাঙ্ক iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	unsigned পূর্ণাঙ্ক seq;
	u64 idle;

	কুরু {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		যদি (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} যাবত্ (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	প্রত্যাগম idle;
}

u64 kcpustat_field_idle(পূর্ণাঙ্ক cpu)
{
	প্রত্যাগম kcpustat_field_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_প্রতীক_GPL(kcpustat_field_idle);

u64 kcpustat_field_iowait(পূর্ণাঙ্ক cpu)
{
	প্রত্যাগম kcpustat_field_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_প্রতীক_GPL(kcpustat_field_iowait);
#অন্যথা
স্থির অন্তঃস্থ u64 kcpustat_tick_overlap(শূন্য)
{
	প্রত্যাগম 0;
}

স্থির u64 kcpustat_field_dyntick(পূর্ণাঙ্ক cpu, গণনা cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	প্রত্যাগম kcpustat_cpu(cpu).cpustat[idx];
}
#endif /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

স্থির u64 get_cpu_sleep_time_us(পূর্ণাঙ্ক cpu, গণনা cpu_usage_stat idx,
				 bool compute_delta, u64 *last_update_time)
{
	ktime_t now = ktime_get();
	u64 res;

	যদি (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_field(idx, cpu);
	অন্যথা
		res = kcpustat_field_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_PER_USEC);

	যদি (last_update_time)
		*last_update_time = ktime_to_us(now);

	প্রত্যাগম res;
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
u64 get_cpu_idle_time_us(পূর্ণাঙ্ক cpu, u64 *last_update_time)
{
	প্রত্যাগম get_cpu_sleep_time_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), last_update_time);
}
EXPORT_প্রতীক_GPL(get_cpu_idle_time_us);

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
u64 get_cpu_iowait_time_us(পূর্ণাঙ্ক cpu, u64 *last_update_time)
{
	প্রত্যাগম get_cpu_sleep_time_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), last_update_time);
}
EXPORT_প্রতীক_GPL(get_cpu_iowait_time_us);

/*
 * Use precise platform statistics if available:
 */
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

শূন্য vtime_account_irq(সংরচনা task_struct *tsk, unsigned পূর্ণাঙ্ক offset)
{
	unsigned পূর্ণাঙ্ক pc = irq_গুনতি() - offset;

	যদি (pc & HARDIRQ_OFFSET) {
		vtime_account_hardirq(tsk);
	} অন্যথা যদি (pc & SOFTIRQ_OFFSET) {
		vtime_account_softirq(tsk);
	} অন্যথা যদি (!kcpustat_idle_dyntick()) {
		যদি (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} অন্যথা {
			vtime_account_kernel(tsk);
		}
	} অন্যথা {
		vtime_reset();
	}
}

শূন্য cputime_adjust(সংরচনা task_cputime *curr, সংরচনা prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

শূন্য task_cputime_adjusted(সংরচনা task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_প্রতীক_GPL(task_cputime_adjusted);

শূন্য thread_group_cputime_adjusted(সংরচনা task_struct *p, u64 *ut, u64 *st)
{
	সংরচনা task_cputime cputime;

	thread_group_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#অন্যথা /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
শূন্য account_process_tick(সংরচনা task_struct *p, পূর্ণাঙ্ক user_tick)
{
	u64 cputime, steal;

	যদি (vtime_accounting_enabled_this_cpu())
		প্রত্যাগম;

	যদি (kcpustat_idle_dyntick())
		প্রত্যাগম;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	যদি (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		প্রত্যাগম;
	}

	steal = steal_account_process_time(ULONG_MAX);

	যদি (steal >= cputime)
		প্রত্যাগম;

	cputime -= steal;

	যদি (user_tick)
		account_user_time(p, cputime);
	অন্যথা যদি ((p != this_rq()->idle) || (irq_গুনতি() != HARDIRQ_OFFSET))
		account_system_time(p, HARDIRQ_OFFSET, cputime);
	অন্যথা
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
শূন্য cputime_adjust(সংরচনা task_cputime *curr, সংরচনা prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	u64 rtime, stime, utime;
	unsigned long flags;

	/* Serialize concurrent callers such that we can honour our guarantees */
	raw_spin_lock_irqsave(&prev->lock, flags);
	rtime = curr->sum_exec_runtime;

	/*
	 * This is possible under two circumstances:
	 *  - rtime isn't monotonic after all (a bug);
	 *  - we got reordered by the lock.
	 *
	 * In both cases this acts as a filter such that the rest of the code
	 * can assume it is monotonic regardless of anything else.
	 */
	যদি (prev->stime + prev->utime >= rtime)
		গচ্ছ out;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	যদি (stime == 0) {
		utime = rtime;
		গচ্ছ update;
	}

	যদি (utime == 0) {
		stime = rtime;
		গচ্ছ update;
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
	যদি (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	যদি (utime < prev->utime) {
		utime = prev->utime;
		stime = rtime - utime;
	}

	prev->stime = stime;
	prev->utime = utime;
out:
	*ut = prev->utime;
	*st = prev->stime;
	raw_spin_unlock_irqrestore(&prev->lock, flags);
}

শূন্য task_cputime_adjusted(সংরচনা task_struct *p, u64 *ut, u64 *st)
{
	সংরচনা task_cputime cputime = {
		.sum_exec_runtime = p->se.sum_exec_runtime,
	};

	যদি (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.sum_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_প্রতীক_GPL(task_cputime_adjusted);

শূন্য thread_group_cputime_adjusted(সংরচনা task_struct *p, u64 *ut, u64 *st)
{
	সংরচনা task_cputime cputime;

	thread_group_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->সঙ্কেত->prev_cputime, ut, st);
}
#endif /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
স্থির u64 vtime_delta(সংরচনা vtime *vtime)
{
	unsigned long long clock;

	clock = sched_clock();
	যদি (clock < vtime->starttime)
		প্রত্যাগম 0;

	প্রত্যাগম clock - vtime->starttime;
}

স্থির u64 get_vtime_delta(সংরচনা vtime *vtime)
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

	প্রত্যাগম delta - other;
}

স্থির শূন্য vtime_account_system(সংরচনা task_struct *tsk,
				 সংরচনা vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	যদি (vtime->stime >= TICK_NSEC) {
		account_system_time(tsk, irq_গুনতি(), vtime->stime);
		vtime->stime = 0;
	}
}

স্থির শূন্য vtime_account_guest(সংরচনা task_struct *tsk,
				সংরচনা vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	যদি (vtime->gtime >= TICK_NSEC) {
		account_guest_time(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

স্থির শূন্য __vtime_account_kernel(সংরচনা task_struct *tsk,
				   সংরচনা vtime *vtime)
{
	/* We might have scheduled out from guest path */
	যদি (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	অন্যথা
		vtime_account_system(tsk, vtime);
}

শূন্য vtime_account_kernel(সংরচনা task_struct *tsk)
{
	সংরচনা vtime *vtime = &tsk->vtime;

	যদি (!vtime_delta(vtime))
		প্রত্যাগম;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_end(&vtime->seqcount);
}

শূন্য vtime_user_enter(সংরচনা task_struct *tsk)
{
	সংরচনা vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_end(&vtime->seqcount);
}

শূন্য vtime_user_exit(সংরচনা task_struct *tsk)
{
	সংরচনা vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	যদি (vtime->utime >= TICK_NSEC) {
		account_user_time(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}

শূন্য vtime_guest_enter(সংরচনা task_struct *tsk)
{
	সংরচনা vtime *vtime = &tsk->vtime;
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
EXPORT_প্রতীক_GPL(vtime_guest_enter);

শূন্য vtime_guest_exit(সংরচনা task_struct *tsk)
{
	সংরচনা vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_প্রতীক_GPL(vtime_guest_exit);

স্থির শূন্য __vtime_account_idle(সংরচনা vtime *vtime)
{
	account_idle_time(get_vtime_delta(vtime));
}

শূন্য vtime_task_switch_generic(সংরচনা task_struct *prev)
{
	সংরচনা vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	যদি (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	অন্যথা
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_end(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	যদি (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	অন্যথা যদি (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	অন্যথা
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_end(&vtime->seqcount);
}

শূন্য vtime_init_idle(সংরচনা task_struct *t, পূর্ণাঙ্ক cpu)
{
	সংরচনা vtime *vtime = &t->vtime;
	unsigned long flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_end(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(সংরচনা task_struct *t)
{
	সংরচনা vtime *vtime = &t->vtime;
	unsigned পূর্ণাঙ্ক seq;
	u64 gtime;

	যদি (!vtime_accounting_enabled())
		প্রত্যাগম t->gtime;

	কুরু {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		যদি (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} যাবত্ (read_seqcount_retry(&vtime->seqcount, seq));

	প্রত্যাগম gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(সংরচনা task_struct *t, u64 *utime, u64 *stime)
{
	সংরচনা vtime *vtime = &t->vtime;
	unsigned পূর্ণাঙ্ক seq;
	u64 delta;
	পূর্ণাঙ্ক ret;

	যদি (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		প্রত্যাগম false;
	}

	কুরু {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		যদি (vtime->state < VTIME_SYS)
			অনুবর্ত;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		যদি (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		অন্যথা
			*utime += vtime->utime + delta;
	} যাবত্ (read_seqcount_retry(&vtime->seqcount, seq));

	প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক vtime_state_fetch(সংরচনা vtime *vtime, পূর্ণাঙ্ক cpu)
{
	পূর্ণাঙ্ক state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	যদি (vtime->cpu != cpu && vtime->cpu != -1)
		প্রত্যাগম -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	যদি (state == VTIME_INACTIVE)
		প্রত্যাগম -EAGAIN;

	প্রত্যাগম state;
}

স্থির u64 kcpustat_user_vtime(সংরচনা vtime *vtime)
{
	যদি (vtime->state == VTIME_USER)
		প্রত্যাগম vtime->utime + vtime_delta(vtime);
	অন্যথা যদি (vtime->state == VTIME_GUEST)
		প্রত্যাগম vtime->gtime + vtime_delta(vtime);
	প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক kcpustat_field_vtime(u64 *cpustat,
				সংরচনা task_struct *tsk,
				গণনা cpu_usage_stat usage,
				পূর্ণাঙ্ক cpu, u64 *val)
{
	সংরচনা vtime *vtime = &tsk->vtime;
	unsigned পূর্ণাঙ্ক seq;

	কুরু {
		পূর্ণাঙ্ক state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		যদি (state < 0)
			প্রত্যাগম state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		বিকল্প (usage) {
		পক্ষ CPUTIME_SYSTEM:
			যদি (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			ভঙ্গ;
		পক্ষ CPUTIME_USER:
			যদি (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			ভঙ্গ;
		পক্ষ CPUTIME_NICE:
			যদি (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			ভঙ্গ;
		পক্ষ CPUTIME_GUEST:
			যদি (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ভঙ্গ;
		পক্ষ CPUTIME_GUEST_NICE:
			যদি (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ভঙ্গ;
		পক্ষ CPUTIME_IDLE:
			যদি (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ভঙ্গ;
		পক্ষ CPUTIME_IOWAIT:
			যদি (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ভঙ্গ;
		স্বভাব:
			ভঙ্গ;
		}
	} যাবত্ (read_seqcount_retry(&vtime->seqcount, seq));

	প্রত্যাগম 0;
}

u64 kcpustat_field(গণনা cpu_usage_stat usage, পূর্ণাঙ্ক cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	সংরচনা rq *rq;
	পূর্ণাঙ্ক err;

	যদি (!vtime_generic_enabled_cpu(cpu))
		প্রত্যাগম kcpustat_field_default(usage, cpu);

	rq = cpu_rq(cpu);

	প্রতি (;;) {
		সংরচনা task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		যদি (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			প্রত্যাগম cpustat[usage];
		}

		err = kcpustat_field_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		যদি (!err)
			প্রত্যাগম val;

		cpu_relax();
	}
}
EXPORT_প্রতীক_GPL(kcpustat_field);

স্থির পূর্ণাঙ্ক kcpustat_cpu_fetch_vtime(সংরচনা kernel_cpustat *dst,
				    ধ্রুব সংরচনা kernel_cpustat *src,
				    সংরচনা task_struct *tsk, পূর্ণাঙ্ক cpu)
{
	সংরচনা vtime *vtime = &tsk->vtime;
	unsigned পূর্ণাঙ্ক seq;

	কুরু {
		u64 *cpustat;
		u64 delta;
		পূর্ণাঙ্ক state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		যদি (state < 0)
			প্রত্যাগম state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		যদি (state < VTIME_IDLE)
			অনুবর্ত;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		বিকল্প (state) {
		পক্ষ VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			ভঙ্গ;
		পক্ষ VTIME_USER:
			যদি (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			অন্যথা
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			ভঙ্গ;
		পক্ষ VTIME_GUEST:
			যদি (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} অন্যথা {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			ভঙ্গ;
		পক্ষ VTIME_IDLE:
			যদি (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			অন্যথা
				cpustat[CPUTIME_IDLE] += delta;
			ভঙ্গ;
		স্বভাব:
			WARN_ON_ONCE(1);
		}
	} যাবত্ (read_seqcount_retry(&vtime->seqcount, seq));

	প্রত্যাগম 0;
}

শূন্য kcpustat_cpu_fetch(সংরচনা kernel_cpustat *dst, পূর্ণাঙ্ক cpu)
{
	ধ্রুব সংরচনা kernel_cpustat *src = &kcpustat_cpu(cpu);
	সংরচনা rq *rq;
	পূর্ণাঙ্ক err;

	যদি (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		প্রত্যাগম;
	}

	rq = cpu_rq(cpu);

	প্রতি (;;) {
		সংরচনা task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		যদি (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			প্রত্যাগম;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		যদি (!err)
			প্রত্যাগম;

		cpu_relax();
	}
}
EXPORT_প্রতীক_GPL(kcpustat_cpu_fetch);

#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
