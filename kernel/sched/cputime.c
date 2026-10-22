// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cputime.h>
#ಅನ್ತರ್ಭಾವ <linux/tsacct_kern.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #ಅನ್ತರ್ಭಾವ <asm/cputime.h>
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_clock_irqtime);

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
DEFINE_PER_CPU(ಸಂರಚನಾ irqtime, cpu_irqtime);

ಶೂನ್ಯ enable_sched_clock_irqtime(ಶೂನ್ಯ)
{
	static_branch_enable(&sched_clock_irqtime);
}

ಶೂನ್ಯ disable_sched_clock_irqtime(ಶೂನ್ಯ)
{
	ಯದಿ (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

ಸ್ಥಿರ ಶೂನ್ಯ irqtime_account_delta(ಸಂರಚನಾ irqtime *irqtime, u64 delta,
				  ಗಣನಾ cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->total += delta;
	ಯದಿ (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_end(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
ಶೂನ್ಯ irqtime_account_irq(ಸಂರಚನಾ task_struct *curr, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
{
	ಸಂರಚನಾ irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	unsigned ಪೂರ್ಣಾಙ್ಕ pc;
	s64 delta;
	ಪೂರ್ಣಾಙ್ಕ cpu;

	ಯದಿ (!irqtime_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_start_time;
	irqtime->irq_start_time += delta;
	pc = irq_ಎಣಿಕೆ() - offset;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	ಯದಿ (pc & HARDIRQ_MASK)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	ಅನ್ಯಥಾ ಯದಿ ((pc & SOFTIRQ_OFFSET) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

ಸ್ಥಿರ u64 irqtime_tick_accounted(u64 maxtime)
{
	ಸಂರಚನಾ irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	ಪ್ರತ್ಯಾಗಮ delta;
}

#ಅನ್ಯಥಾ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ಸ್ಥಿರ u64 irqtime_tick_accounted(u64 dummy)
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ task_group_account_field(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ index,
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
ಶೂನ್ಯ account_user_time(ಸಂರಚನಾ task_struct *p, u64 cputime)
{
	ಪೂರ್ಣಾಙ್ಕ index;

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
ಶೂನ್ಯ account_guest_time(ಸಂರಚನಾ task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	ಯದಿ (task_nice(p) > 0) {
		task_group_account_field(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} ಅನ್ಯಥಾ {
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
ಶೂನ್ಯ account_system_index_time(ಸಂರಚನಾ task_struct *p,
			       u64 cputime, ಗಣನಾ cpu_usage_stat index)
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
ಶೂನ್ಯ account_system_time(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ hardirq_offset, u64 cputime)
{
	ಪೂರ್ಣಾಙ್ಕ index;

	ಯದಿ ((p->flags & PF_VCPU) && (irq_ಎಣಿಕೆ() - hardirq_offset == 0)) {
		account_guest_time(p, cputime);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (hardirq_ಎಣಿಕೆ() - hardirq_offset)
		index = CPUTIME_IRQ;
	ಅನ್ಯಥಾ ಯದಿ (in_serving_softirq())
		index = CPUTIME_SOFTIRQ;
	ಅನ್ಯಥಾ
		index = CPUTIME_SYSTEM;

	account_system_index_time(p, cputime, index);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
ಶೂನ್ಯ account_steal_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
ಶೂನ್ಯ account_idle_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	ಯದಿ (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	ಅನ್ಯಥಾ
		cpustat[CPUTIME_IDLE] += cputime;
}


#ifdef CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
ಶೂನ್ಯ __account_forceidle_time(ಸಂರಚನಾ task_struct *p, u64 delta)
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
DEFINE_STATIC_ಕೀಲಿ_FALSE(paravirt_steal_enabled);

#ifdef CONFIG_HAVE_PV_STEAL_CLOCK_GEN
ಸ್ಥಿರ u64 native_steal_clock(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#endif
#endif

ಸ್ಥಿರ __always_inline u64 steal_account_process_time(u64 maxtime)
{
#ifdef CONFIG_PARAVIRT
	ಯದಿ (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_time;
		steal = min(steal, maxtime);
		account_steal_time(steal);
		this_rq()->prev_steal_time += steal;

		ಪ್ರತ್ಯಾಗಮ steal;
	}
#endif /* CONFIG_PARAVIRT */
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 account_other_time(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_time(max);

	ಯದಿ (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	ಪ್ರತ್ಯಾಗಮ accounted;
}

#ifdef CONFIG_64BIT
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 read_sum_exec_runtime(ಸಂರಚನಾ task_struct *t)
{
	ಪ್ರತ್ಯಾಗಮ t->se.sum_exec_runtime;
}
#ಅನ್ಯಥಾ /* !CONFIG_64BIT: */
ಸ್ಥಿರ u64 read_sum_exec_runtime(ಸಂರಚನಾ task_struct *t)
{
	u64 ns;
	ಸಂರಚನಾ rq_flags rf;
	ಸಂರಚನಾ rq *rq;

	rq = task_rq_lock(t, &rf);
	ns = t->se.sum_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	ಪ್ರತ್ಯಾಗಮ ns;
}
#endif /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
ಶೂನ್ಯ thread_group_cputime(ಸಂರಚನಾ task_struct *tsk, ಸಂರಚನಾ task_cputime *times)
{
	ಸಂರಚನಾ signal_struct *sig = tsk->ಸಙ್ಕೇತ;
	ಸಂರಚನಾ task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	ಯದಿ (same_thread_group(current, tsk))
		(ಶೂನ್ಯ) task_sched_runtime(current);

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
ಸ್ಥಿರ ಶೂನ್ಯ irqtime_account_process_tick(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ user_tick,
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
	ಯದಿ (other >= cputime)
		ಪ್ರತ್ಯಾಗಮ;

	cputime -= other;

	ಯದಿ (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_index_time(p, cputime, CPUTIME_SOFTIRQ);
	} ಅನ್ಯಥಾ ಯದಿ (user_tick) {
		account_user_time(p, cputime);
	} ಅನ್ಯಥಾ ಯದಿ (p == this_rq()->idle) {
		account_idle_time(cputime);
	} ಅನ್ಯಥಾ ಯದಿ (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_time(p, cputime);
	} ಅನ್ಯಥಾ {
		account_system_index_time(p, cputime, CPUTIME_SYSTEM);
	}
}

#ಅನ್ಯಥಾ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ irqtime_account_process_tick(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ user_tick,
						u64 cputime) { }
#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ಯದಿ defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
ಸ್ಥಿರ ಶೂನ್ಯ kcpustat_idle_stop(ಸಂರಚನಾ kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	ಪೂರ್ಣಾಙ್ಕ iowait;

	ಯದಿ (!kc->idle_elapse)
		ಪ್ರತ್ಯಾಗಮ;

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

	ಯದಿ (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	ಅನ್ಯಥಾ
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

ಸ್ಥಿರ ಶೂನ್ಯ kcpustat_idle_start(ಸಂರಚನಾ kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	ಯದಿ (kc->idle_elapse)
		ಪ್ರತ್ಯಾಗಮ;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

ಶೂನ್ಯ kcpustat_dyntick_stop(u64 now, u64 tick_start)
{
	ಸಂರಚನಾ kernel_cpustat *kc = kcpustat_this_cpu;

	ಯದಿ (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		ಯದಿ (kc->idle_tick_period != tick_start) {
			kc->idle_tick_period = tick_start;
			kc->idle_tick_overlap = 0;
		}
		tick_start = max(tick_start, kc->idle_dyntick_entry);
		ಯದಿ (!WARN_ON_ONCE(now < tick_start))
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 kcpustat_tick_overlap(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	ಪ್ರತ್ಯಾಗಮ overlap;
}

ಶೂನ್ಯ kcpustat_dyntick_start(u64 now)
{
	ಸಂರಚನಾ kernel_cpustat *kc = kcpustat_this_cpu;

	ಯದಿ (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_start();
		kc->idle_dyntick = true;
		kc->idle_dyntick_entry = now;
		kcpustat_idle_start(kc, now);
	}
}

ಶೂನ್ಯ kcpustat_irq_enter(u64 now)
{
	ಸಂರಚನಾ kernel_cpustat *kc = kcpustat_this_cpu;

	ಯದಿ (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

ಶೂನ್ಯ kcpustat_irq_exit(u64 now)
{
	ಸಂರಚನಾ kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	ಯದಿ (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_start(kc, now);
}

ಸ್ಥಿರ u64 kcpustat_field_dyntick(ಪೂರ್ಣಾಙ್ಕ cpu, ಗಣನಾ cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	ಸಂರಚನಾ kernel_cpustat *kc = &kcpustat_cpu(cpu);
	ಪೂರ್ಣಾಙ್ಕ iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	unsigned ಪೂರ್ಣಾಙ್ಕ seq;
	u64 idle;

	ಕುರು {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		ಯದಿ (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} ಯಾವತ್ (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	ಪ್ರತ್ಯಾಗಮ idle;
}

u64 kcpustat_field_idle(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ kcpustat_field_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_field_idle);

u64 kcpustat_field_iowait(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ kcpustat_field_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_field_iowait);
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 kcpustat_tick_overlap(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ u64 kcpustat_field_dyntick(ಪೂರ್ಣಾಙ್ಕ cpu, ಗಣನಾ cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	ಪ್ರತ್ಯಾಗಮ kcpustat_cpu(cpu).cpustat[idx];
}
#endif /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

ಸ್ಥಿರ u64 get_cpu_sleep_time_us(ಪೂರ್ಣಾಙ್ಕ cpu, ಗಣನಾ cpu_usage_stat idx,
				 bool compute_delta, u64 *last_update_time)
{
	ktime_t now = ktime_get();
	u64 res;

	ಯದಿ (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_field(idx, cpu);
	ಅನ್ಯಥಾ
		res = kcpustat_field_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_PER_USEC);

	ಯದಿ (last_update_time)
		*last_update_time = ktime_to_us(now);

	ಪ್ರತ್ಯಾಗಮ res;
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
u64 get_cpu_idle_time_us(ಪೂರ್ಣಾಙ್ಕ cpu, u64 *last_update_time)
{
	ಪ್ರತ್ಯಾಗಮ get_cpu_sleep_time_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), last_update_time);
}
EXPORT_ಚಿಹ್ನೆ_GPL(get_cpu_idle_time_us);

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
u64 get_cpu_iowait_time_us(ಪೂರ್ಣಾಙ್ಕ cpu, u64 *last_update_time)
{
	ಪ್ರತ್ಯಾಗಮ get_cpu_sleep_time_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), last_update_time);
}
EXPORT_ಚಿಹ್ನೆ_GPL(get_cpu_iowait_time_us);

/*
 * Use precise platform statistics if available:
 */
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

ಶೂನ್ಯ vtime_account_irq(ಸಂರಚನಾ task_struct *tsk, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ pc = irq_ಎಣಿಕೆ() - offset;

	ಯದಿ (pc & HARDIRQ_OFFSET) {
		vtime_account_hardirq(tsk);
	} ಅನ್ಯಥಾ ಯದಿ (pc & SOFTIRQ_OFFSET) {
		vtime_account_softirq(tsk);
	} ಅನ್ಯಥಾ ಯದಿ (!kcpustat_idle_dyntick()) {
		ಯದಿ (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} ಅನ್ಯಥಾ {
			vtime_account_kernel(tsk);
		}
	} ಅನ್ಯಥಾ {
		vtime_reset();
	}
}

ಶೂನ್ಯ cputime_adjust(ಸಂರಚನಾ task_cputime *curr, ಸಂರಚನಾ prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

ಶೂನ್ಯ task_cputime_adjusted(ಸಂರಚನಾ task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_ಚಿಹ್ನೆ_GPL(task_cputime_adjusted);

ಶೂನ್ಯ thread_group_cputime_adjusted(ಸಂರಚನಾ task_struct *p, u64 *ut, u64 *st)
{
	ಸಂರಚನಾ task_cputime cputime;

	thread_group_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#ಅನ್ಯಥಾ /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
ಶೂನ್ಯ account_process_tick(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ user_tick)
{
	u64 cputime, steal;

	ಯದಿ (vtime_accounting_enabled_this_cpu())
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (kcpustat_idle_dyntick())
		ಪ್ರತ್ಯಾಗಮ;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	ಯದಿ (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		ಪ್ರತ್ಯಾಗಮ;
	}

	steal = steal_account_process_time(ULONG_MAX);

	ಯದಿ (steal >= cputime)
		ಪ್ರತ್ಯಾಗಮ;

	cputime -= steal;

	ಯದಿ (user_tick)
		account_user_time(p, cputime);
	ಅನ್ಯಥಾ ಯದಿ ((p != this_rq()->idle) || (irq_ಎಣಿಕೆ() != HARDIRQ_OFFSET))
		account_system_time(p, HARDIRQ_OFFSET, cputime);
	ಅನ್ಯಥಾ
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
ಶೂನ್ಯ cputime_adjust(ಸಂರಚನಾ task_cputime *curr, ಸಂರಚನಾ prev_cputime *prev,
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
	ಯದಿ (prev->stime + prev->utime >= rtime)
		ಗಚ್ಛ out;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	ಯದಿ (stime == 0) {
		utime = rtime;
		ಗಚ್ಛ update;
	}

	ಯದಿ (utime == 0) {
		stime = rtime;
		ಗಚ್ಛ update;
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
	ಯದಿ (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	ಯದಿ (utime < prev->utime) {
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

ಶೂನ್ಯ task_cputime_adjusted(ಸಂರಚನಾ task_struct *p, u64 *ut, u64 *st)
{
	ಸಂರಚನಾ task_cputime cputime = {
		.sum_exec_runtime = p->se.sum_exec_runtime,
	};

	ಯದಿ (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.sum_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_ಚಿಹ್ನೆ_GPL(task_cputime_adjusted);

ಶೂನ್ಯ thread_group_cputime_adjusted(ಸಂರಚನಾ task_struct *p, u64 *ut, u64 *st)
{
	ಸಂರಚನಾ task_cputime cputime;

	thread_group_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->ಸಙ್ಕೇತ->prev_cputime, ut, st);
}
#endif /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
ಸ್ಥಿರ u64 vtime_delta(ಸಂರಚನಾ vtime *vtime)
{
	unsigned long long clock;

	clock = sched_clock();
	ಯದಿ (clock < vtime->starttime)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ clock - vtime->starttime;
}

ಸ್ಥಿರ u64 get_vtime_delta(ಸಂರಚನಾ vtime *vtime)
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

	ಪ್ರತ್ಯಾಗಮ delta - other;
}

ಸ್ಥಿರ ಶೂನ್ಯ vtime_account_system(ಸಂರಚನಾ task_struct *tsk,
				 ಸಂರಚನಾ vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	ಯದಿ (vtime->stime >= TICK_NSEC) {
		account_system_time(tsk, irq_ಎಣಿಕೆ(), vtime->stime);
		vtime->stime = 0;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ vtime_account_guest(ಸಂರಚನಾ task_struct *tsk,
				ಸಂರಚನಾ vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	ಯದಿ (vtime->gtime >= TICK_NSEC) {
		account_guest_time(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __vtime_account_kernel(ಸಂರಚನಾ task_struct *tsk,
				   ಸಂರಚನಾ vtime *vtime)
{
	/* We might have scheduled out from guest path */
	ಯದಿ (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	ಅನ್ಯಥಾ
		vtime_account_system(tsk, vtime);
}

ಶೂನ್ಯ vtime_account_kernel(ಸಂರಚನಾ task_struct *tsk)
{
	ಸಂರಚನಾ vtime *vtime = &tsk->vtime;

	ಯದಿ (!vtime_delta(vtime))
		ಪ್ರತ್ಯಾಗಮ;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_end(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_user_enter(ಸಂರಚನಾ task_struct *tsk)
{
	ಸಂರಚನಾ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_end(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_user_exit(ಸಂರಚನಾ task_struct *tsk)
{
	ಸಂರಚನಾ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	ಯದಿ (vtime->utime >= TICK_NSEC) {
		account_user_time(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_guest_enter(ಸಂರಚನಾ task_struct *tsk)
{
	ಸಂರಚನಾ vtime *vtime = &tsk->vtime;
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
EXPORT_ಚಿಹ್ನೆ_GPL(vtime_guest_enter);

ಶೂನ್ಯ vtime_guest_exit(ಸಂರಚನಾ task_struct *tsk)
{
	ಸಂರಚನಾ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_ಚಿಹ್ನೆ_GPL(vtime_guest_exit);

ಸ್ಥಿರ ಶೂನ್ಯ __vtime_account_idle(ಸಂರಚನಾ vtime *vtime)
{
	account_idle_time(get_vtime_delta(vtime));
}

ಶೂನ್ಯ vtime_task_switch_generic(ಸಂರಚನಾ task_struct *prev)
{
	ಸಂರಚನಾ vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ಯದಿ (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	ಅನ್ಯಥಾ
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_end(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ಯದಿ (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	ಅನ್ಯಥಾ ಯದಿ (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	ಅನ್ಯಥಾ
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_end(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_init_idle(ಸಂರಚನಾ task_struct *t, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ vtime *vtime = &t->vtime;
	unsigned long flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_end(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(ಸಂರಚನಾ task_struct *t)
{
	ಸಂರಚನಾ vtime *vtime = &t->vtime;
	unsigned ಪೂರ್ಣಾಙ್ಕ seq;
	u64 gtime;

	ಯದಿ (!vtime_accounting_enabled())
		ಪ್ರತ್ಯಾಗಮ t->gtime;

	ಕುರು {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		ಯದಿ (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} ಯಾವತ್ (read_seqcount_retry(&vtime->seqcount, seq));

	ಪ್ರತ್ಯಾಗಮ gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(ಸಂರಚನಾ task_struct *t, u64 *utime, u64 *stime)
{
	ಸಂರಚನಾ vtime *vtime = &t->vtime;
	unsigned ಪೂರ್ಣಾಙ್ಕ seq;
	u64 delta;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		ಪ್ರತ್ಯಾಗಮ false;
	}

	ಕುರು {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		ಯದಿ (vtime->state < VTIME_SYS)
			ಅನುವರ್ತ;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ಯದಿ (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		ಅನ್ಯಥಾ
			*utime += vtime->utime + delta;
	} ಯಾವತ್ (read_seqcount_retry(&vtime->seqcount, seq));

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ vtime_state_fetch(ಸಂರಚನಾ vtime *vtime, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪೂರ್ಣಾಙ್ಕ state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	ಯದಿ (vtime->cpu != cpu && vtime->cpu != -1)
		ಪ್ರತ್ಯಾಗಮ -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	ಯದಿ (state == VTIME_INACTIVE)
		ಪ್ರತ್ಯಾಗಮ -EAGAIN;

	ಪ್ರತ್ಯಾಗಮ state;
}

ಸ್ಥಿರ u64 kcpustat_user_vtime(ಸಂರಚನಾ vtime *vtime)
{
	ಯದಿ (vtime->state == VTIME_USER)
		ಪ್ರತ್ಯಾಗಮ vtime->utime + vtime_delta(vtime);
	ಅನ್ಯಥಾ ಯದಿ (vtime->state == VTIME_GUEST)
		ಪ್ರತ್ಯಾಗಮ vtime->gtime + vtime_delta(vtime);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ kcpustat_field_vtime(u64 *cpustat,
				ಸಂರಚನಾ task_struct *tsk,
				ಗಣನಾ cpu_usage_stat usage,
				ಪೂರ್ಣಾಙ್ಕ cpu, u64 *val)
{
	ಸಂರಚನಾ vtime *vtime = &tsk->vtime;
	unsigned ಪೂರ್ಣಾಙ್ಕ seq;

	ಕುರು {
		ಪೂರ್ಣಾಙ್ಕ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ಯದಿ (state < 0)
			ಪ್ರತ್ಯಾಗಮ state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		ವಿಕಲ್ಪ (usage) {
		ಪಕ್ಷ CPUTIME_SYSTEM:
			ಯದಿ (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			ಭಙ್ಗ;
		ಪಕ್ಷ CPUTIME_USER:
			ಯದಿ (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			ಭಙ್ಗ;
		ಪಕ್ಷ CPUTIME_NICE:
			ಯದಿ (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			ಭಙ್ಗ;
		ಪಕ್ಷ CPUTIME_GUEST:
			ಯದಿ (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ಭಙ್ಗ;
		ಪಕ್ಷ CPUTIME_GUEST_NICE:
			ಯದಿ (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ಭಙ್ಗ;
		ಪಕ್ಷ CPUTIME_IDLE:
			ಯದಿ (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ಭಙ್ಗ;
		ಪಕ್ಷ CPUTIME_IOWAIT:
			ಯದಿ (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			ಭಙ್ಗ;
		}
	} ಯಾವತ್ (read_seqcount_retry(&vtime->seqcount, seq));

	ಪ್ರತ್ಯಾಗಮ 0;
}

u64 kcpustat_field(ಗಣನಾ cpu_usage_stat usage, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	ಸಂರಚನಾ rq *rq;
	ಪೂರ್ಣಾಙ್ಕ err;

	ಯದಿ (!vtime_generic_enabled_cpu(cpu))
		ಪ್ರತ್ಯಾಗಮ kcpustat_field_default(usage, cpu);

	rq = cpu_rq(cpu);

	ಪ್ರತಿ (;;) {
		ಸಂರಚನಾ task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		ಯದಿ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			ಪ್ರತ್ಯಾಗಮ cpustat[usage];
		}

		err = kcpustat_field_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		ಯದಿ (!err)
			ಪ್ರತ್ಯಾಗಮ val;

		cpu_relax();
	}
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_field);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ kcpustat_cpu_fetch_vtime(ಸಂರಚನಾ kernel_cpustat *dst,
				    ಧ್ರುವ ಸಂರಚನಾ kernel_cpustat *src,
				    ಸಂರಚನಾ task_struct *tsk, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ vtime *vtime = &tsk->vtime;
	unsigned ಪೂರ್ಣಾಙ್ಕ seq;

	ಕುರು {
		u64 *cpustat;
		u64 delta;
		ಪೂರ್ಣಾಙ್ಕ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ಯದಿ (state < 0)
			ಪ್ರತ್ಯಾಗಮ state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		ಯದಿ (state < VTIME_IDLE)
			ಅನುವರ್ತ;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ವಿಕಲ್ಪ (state) {
		ಪಕ್ಷ VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			ಭಙ್ಗ;
		ಪಕ್ಷ VTIME_USER:
			ಯದಿ (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			ಅನ್ಯಥಾ
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			ಭಙ್ಗ;
		ಪಕ್ಷ VTIME_GUEST:
			ಯದಿ (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} ಅನ್ಯಥಾ {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			ಭಙ್ಗ;
		ಪಕ್ಷ VTIME_IDLE:
			ಯದಿ (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			ಅನ್ಯಥಾ
				cpustat[CPUTIME_IDLE] += delta;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			WARN_ON_ONCE(1);
		}
	} ಯಾವತ್ (read_seqcount_retry(&vtime->seqcount, seq));

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ kcpustat_cpu_fetch(ಸಂರಚನಾ kernel_cpustat *dst, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಧ್ರುವ ಸಂರಚನಾ kernel_cpustat *src = &kcpustat_cpu(cpu);
	ಸಂರಚನಾ rq *rq;
	ಪೂರ್ಣಾಙ್ಕ err;

	ಯದಿ (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		ಪ್ರತ್ಯಾಗಮ;
	}

	rq = cpu_rq(cpu);

	ಪ್ರತಿ (;;) {
		ಸಂರಚನಾ task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		ಯದಿ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			ಪ್ರತ್ಯಾಗಮ;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		ಯದಿ (!err)
			ಪ್ರತ್ಯಾಗಮ;

		cpu_relax();
	}
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_cpu_fetch);

#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
