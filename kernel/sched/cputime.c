// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cputime.h>
#ಅನ್ತರ್ಭಾವ <linux/tsacct_kern.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #ಅನ್ತರ್ಭಾವ <asm/cputime.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING

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
DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ irqtime, cpu_irqtime);

ಶೂನ್ಯ enable_sched_clock_irqtime(ಶೂನ್ಯ)
{
	static_branch_enable(&sched_clock_irqtime);
}

ಶೂನ್ಯ disable_sched_clock_irqtime(ಶೂನ್ಯ)
{
	ಆದರೆ (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

ಸ್ಥಿರ ಶೂನ್ಯ irqtime_account_delta(ರಚನೆ irqtime *irqtime, u64 delta,
				  ಎಣಿಕೆ cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->ಒಟ್ಟು += delta;
	ಆದರೆ (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_ಕೊನೆ(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
ಶೂನ್ಯ irqtime_account_irq(ರಚನೆ task_struct *curr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ)
{
	ರಚನೆ irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pc;
	s64 delta;
	ಪೂರ್ಣಾಂಕ cpu;

	ಆದರೆ (!irqtime_enabled())
		ಹಿಂತಿರುಗಿ;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_ಆರಂಭ_ಸಮಯ;
	irqtime->irq_ಆರಂಭ_ಸಮಯ += delta;
	pc = irq_ಎಣಿಕೆ() - ಸ್ಥಾನಾಂತರ;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	ಆದರೆ (pc & HARDIRQ_ಮುಖವಾಡ)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((pc & SOFTIRQ_ಸ್ಥಾನಾಂತರ) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

ಸ್ಥಿರ u64 irqtime_tick_accounted(u64 maxtime)
{
	ರಚನೆ irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	ಹಿಂತಿರುಗಿ delta;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ಸ್ಥಿರ u64 irqtime_tick_accounted(u64 dummy)
{
	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_IRQ_TIME_ACCOUNTING */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ task_ಗುಂಪು_account_ಕ್ಷೇತ್ರ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ,
					    u64 tmp)
{
	/*
	 * Since all updates are sure to touch the root cgroup, we
	 * get ourselves ahead and touch it first. If the root cgroup
	 * is the only cgroup, then nothing else should be necessary.
	 *
	 */
	__this_cpu_add(kernel_cpustat.cpustat[ಸೂಚ್ಯಂಕ], tmp);

	cgroup_account_cputime_ಕ್ಷೇತ್ರ(p, ಸೂಚ್ಯಂಕ, tmp);
}

/*
 * Account user CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in user space since the last update
 */
ಶೂನ್ಯ account_user_ಸಮಯ(ರಚನೆ task_struct *p, u64 cputime)
{
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ;

	/* Add user time to process. */
	p->utime += cputime;
	account_ಗುಂಪು_user_ಸಮಯ(p, cputime);

	ಸೂಚ್ಯಂಕ = (task_nice(p) > 0) ? CPUTIME_NICE : CPUTIME_USER;

	/* Add user time to cpustat. */
	task_ಗುಂಪು_account_ಕ್ಷೇತ್ರ(p, ಸೂಚ್ಯಂಕ, cputime);

	/* Account for user time used */
	acct_account_cputime(p);
}

/*
 * Account guest CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in virtual machine since the last update
 */
ಶೂನ್ಯ account_guest_ಸಮಯ(ರಚನೆ task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_ಗುಂಪು_user_ಸಮಯ(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	ಆದರೆ (task_nice(p) > 0) {
		task_ಗುಂಪು_account_ಕ್ಷೇತ್ರ(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} ಇಲ್ಲದಿದ್ದರೆ {
		task_ಗುಂಪು_account_ಕ್ಷೇತ್ರ(p, CPUTIME_USER, cputime);
		cpustat[CPUTIME_GUEST] += cputime;
	}
}

/*
 * Account system CPU time to a process and desired cpustat field
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in kernel space since the last update
 * @index: pointer to cpustat field that has to be updated
 */
ಶೂನ್ಯ account_system_ಸೂಚ್ಯಂಕ_ಸಮಯ(ರಚನೆ task_struct *p,
			       u64 cputime, ಎಣಿಕೆ cpu_usage_stat ಸೂಚ್ಯಂಕ)
{
	/* Add system time to process. */
	p->stime += cputime;
	account_ಗುಂಪು_system_ಸಮಯ(p, cputime);

	/* Add system time to cpustat. */
	task_ಗುಂಪು_account_ಕ್ಷೇತ್ರ(p, ಸೂಚ್ಯಂಕ, cputime);

	/* Account for system time used */
	acct_account_cputime(p);
}

/*
 * Account system CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @hardirq_offset: the offset to subtract from hardirq_count()
 * @cputime: the CPU time spent in kernel space since the last update
 */
ಶೂನ್ಯ account_system_ಸಮಯ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ hardirq_ಸ್ಥಾನಾಂತರ, u64 cputime)
{
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ;

	ಆದರೆ ((p->flags & PF_VCPU) && (irq_ಎಣಿಕೆ() - hardirq_ಸ್ಥಾನಾಂತರ == 0)) {
		account_guest_ಸಮಯ(p, cputime);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (hardirq_ಎಣಿಕೆ() - hardirq_ಸ್ಥಾನಾಂತರ)
		ಸೂಚ್ಯಂಕ = CPUTIME_IRQ;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಳ_serving_softirq())
		ಸೂಚ್ಯಂಕ = CPUTIME_SOFTIRQ;
	ಇಲ್ಲದಿದ್ದರೆ
		ಸೂಚ್ಯಂಕ = CPUTIME_SYSTEM;

	account_system_ಸೂಚ್ಯಂಕ_ಸಮಯ(p, cputime, ಸೂಚ್ಯಂಕ);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
ಶೂನ್ಯ account_steal_ಸಮಯ(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
ಶೂನ್ಯ account_idle_ಸಮಯ(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	ಆದರೆ (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	ಇಲ್ಲದಿದ್ದರೆ
		cpustat[CPUTIME_IDLE] += cputime;
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
ಶೂನ್ಯ __account_forceidle_time(ರಚನೆ task_struct *p, u64 delta)
{
	__schedstat_add(p->stats.core_forceidle_ಮೊತ್ತ, delta);

	task_ಗುಂಪು_account_ಕ್ಷೇತ್ರ(p, CPUTIME_FORCEIDLE, delta);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CORE */

/*
 * When a guest is interrupted for a longer amount of time, missed clock
 * ticks are not redelivered later. Due to that, this function may on
 * occasion account more time than the calling functions think elapsed.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PARAVIRT
DEFINE_STATIC_ಕೀಲಿ_FALSE(paravirt_steal_enabled);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HAVE_PV_STEAL_CLOCK_GEN
ಸ್ಥಿರ u64 native_steal_clock(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ __always_inline u64 steal_account_process_ಸಮಯ(u64 maxtime)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PARAVIRT
	ಆದರೆ (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_ಸಮಯ;
		steal = min(steal, maxtime);
		account_steal_ಸಮಯ(steal);
		this_rq()->prev_steal_ಸಮಯ += steal;

		ಹಿಂತಿರುಗಿ steal;
	}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_PARAVIRT */
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 account_other_ಸಮಯ(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_ಸಮಯ(max);

	ಆದರೆ (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	ಹಿಂತಿರುಗಿ accounted;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_64_ಬಿಟ್
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 read_ಮೊತ್ತ_exec_runtime(ರಚನೆ task_struct *t)
{
	ಹಿಂತಿರುಗಿ t->se.ಮೊತ್ತ_exec_runtime;
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_64BIT: */
ಸ್ಥಿರ u64 read_ಮೊತ್ತ_exec_runtime(ರಚನೆ task_struct *t)
{
	u64 ns;
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	rq = task_rq_ಬೀಗ_2(t, &rf);
	ns = t->se.ಮೊತ್ತ_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	ಹಿಂತಿರುಗಿ ns;
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
ಶೂನ್ಯ thread_ಗುಂಪು_cputime(ರಚನೆ task_struct *tsk, ರಚನೆ task_cputime *times)
{
	ರಚನೆ signal_struct *sig = tsk->ಸಂಕೇತ;
	ರಚನೆ task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	ಆದರೆ (same_thread_ಗುಂಪು(current, tsk))
		(ಶೂನ್ಯ) task_sched_runtime(current);

	guard(rcu)();
	scoped_seqlock_read (&sig->stats_ಬೀಗ, ss_ಬೀಗ_irqsave) {
		times->utime = sig->utime;
		times->stime = sig->stime;
		times->ಮೊತ್ತ_exec_runtime = sig->ಮೊತ್ತ_sched_runtime;

		__for_each_thread(sig, t) {
			task_cputime(t, &utime, &stime);
			times->utime += utime;
			times->stime += stime;
			times->ಮೊತ್ತ_exec_runtime += read_ಮೊತ್ತ_exec_runtime(t);
		}
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
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
ಸ್ಥಿರ ಶೂನ್ಯ irqtime_account_process_tick(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ user_tick,
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
	other = account_other_ಸಮಯ(ULONG_MAX);
	ಆದರೆ (other >= cputime)
		ಹಿಂತಿರುಗಿ;

	cputime -= other;

	ಆದರೆ (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_ಸೂಚ್ಯಂಕ_ಸಮಯ(p, cputime, CPUTIME_SOFTIRQ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (user_tick) {
		account_user_ಸಮಯ(p, cputime);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p == this_rq()->idle) {
		account_idle_ಸಮಯ(cputime);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_ಸಮಯ(p, cputime);
	} ಇಲ್ಲದಿದ್ದರೆ {
		account_system_ಸೂಚ್ಯಂಕ_ಸಮಯ(p, cputime, CPUTIME_SYSTEM);
	}
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ irqtime_account_process_tick(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ user_tick,
						u64 cputime) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ಆದರೆ defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
ಸ್ಥಿರ ಶೂನ್ಯ kcpustat_idle_stop(ರಚನೆ kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	ಪೂರ್ಣಾಂಕ iowait;

	ಆದರೆ (!kc->idle_elapse)
		ಹಿಂತಿರುಗಿ;

	iowait = cpu_has_iowait(smp_processor_id());
	delta = now - kc->idle_entrytime;
	steal = steal_account_process_ಸಮಯ(delta);

	/*
	 * Record the idle time after substracting the steal time from
	 * previous update sequence. Don't substract the steal time from
	 * the current update sequence to avoid readers moving backward.
	 */
	write_seqcount_begin(&kc->idle_sleeptime_seq);
	steal_delta = min_t(u64, kc->idle_stealtime[iowait], delta);
	delta -= steal_delta;
	kc->idle_stealtime[iowait] -= steal_delta;

	ಆದರೆ (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	ಇಲ್ಲದಿದ್ದರೆ
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_ಕೊನೆ(&kc->idle_sleeptime_seq);
}

ಸ್ಥಿರ ಶೂನ್ಯ kcpustat_idle_ಆರಂಭ(ರಚನೆ kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	ಆದರೆ (kc->idle_elapse)
		ಹಿಂತಿರುಗಿ;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_ಕೊನೆ(&kc->idle_sleeptime_seq);
}

ಶೂನ್ಯ kcpustat_dyntick_stop(u64 now, u64 tick_ಆರಂಭ)
{
	ರಚನೆ kernel_cpustat *kc = kcpustat_this_cpu;

	ಆದರೆ (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		ಆದರೆ (kc->idle_tick_period != tick_ಆರಂಭ) {
			kc->idle_tick_period = tick_ಆರಂಭ;
			kc->idle_tick_overlap = 0;
		}
		tick_ಆರಂಭ = max(tick_ಆರಂಭ, kc->idle_dyntick_ನಮೂದು);
		ಆದರೆ (!WARN_ON_ONCE(now < tick_ಆರಂಭ))
			kc->idle_tick_overlap += now - tick_ಆರಂಭ;
		kcpustat_idle_stop(kc, now);
		kc->idle_dyntick = false;
		vtime_dyntick_stop();
	}
}

/*
 * The first tick after dyntick-idle covers a period that the dyntick-idle
 * accounting may already have accounted in part.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 kcpustat_tick_overlap(ಶೂನ್ಯ)
{
	ರಚನೆ kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	ಹಿಂತಿರುಗಿ overlap;
}

ಶೂನ್ಯ kcpustat_dyntick_ಆರಂಭ(u64 now)
{
	ರಚನೆ kernel_cpustat *kc = kcpustat_this_cpu;

	ಆದರೆ (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_ಆರಂಭ();
		kc->idle_dyntick = true;
		kc->idle_dyntick_ನಮೂದು = now;
		kcpustat_idle_ಆರಂಭ(kc, now);
	}
}

ಶೂನ್ಯ kcpustat_irq_enter(u64 now)
{
	ರಚನೆ kernel_cpustat *kc = kcpustat_this_cpu;

	ಆದರೆ (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

ಶೂನ್ಯ kcpustat_irq_exit(u64 now)
{
	ರಚನೆ kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	ಆದರೆ (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_ಸಮಯ_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_ಆರಂಭ(kc, now);
}

ಸ್ಥಿರ u64 kcpustat_ಕ್ಷೇತ್ರ_dyntick(ಪೂರ್ಣಾಂಕ cpu, ಎಣಿಕೆ cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	ರಚನೆ kernel_cpustat *kc = &kcpustat_cpu(cpu);
	ಪೂರ್ಣಾಂಕ iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seq;
	u64 idle;

	ಮಾಡು {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		ಆದರೆ (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} ಆಗ (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	ಹಿಂತಿರುಗಿ idle;
}

u64 kcpustat_ಕ್ಷೇತ್ರ_idle(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ kcpustat_ಕ್ಷೇತ್ರ_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_ಕ್ಷೇತ್ರ_idle);

u64 kcpustat_ಕ್ಷೇತ್ರ_iowait(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ kcpustat_ಕ್ಷೇತ್ರ_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_ಕ್ಷೇತ್ರ_iowait);
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 kcpustat_tick_overlap(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ u64 kcpustat_ಕ್ಷೇತ್ರ_dyntick(ಪೂರ್ಣಾಂಕ cpu, ಎಣಿಕೆ cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	ಹಿಂತಿರುಗಿ kcpustat_cpu(cpu).cpustat[idx];
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

ಸ್ಥಿರ u64 get_cpu_sleep_ಸಮಯ_us(ಪೂರ್ಣಾಂಕ cpu, ಎಣಿಕೆ cpu_usage_stat idx,
				 bool compute_delta, u64 *ಕೊನೆಯ_update_ಸಮಯ)
{
	ktime_t now = ktime_get();
	u64 res;

	ಆದರೆ (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_ಕ್ಷೇತ್ರ(idx, cpu);
	ಇಲ್ಲದಿದ್ದರೆ
		res = kcpustat_ಕ್ಷೇತ್ರ_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_ಪ್ರತಿ_USEC);

	ಆದರೆ (ಕೊನೆಯ_update_ಸಮಯ)
		*ಕೊನೆಯ_update_ಸಮಯ = ktime_ಗಮ್ಯ_us(now);

	ಹಿಂತಿರುಗಿ res;
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
u64 get_cpu_idle_ಸಮಯ_us(ಪೂರ್ಣಾಂಕ cpu, u64 *ಕೊನೆಯ_update_ಸಮಯ)
{
	ಹಿಂತಿರುಗಿ get_cpu_sleep_ಸಮಯ_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), ಕೊನೆಯ_update_ಸಮಯ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(get_cpu_idle_ಸಮಯ_us);

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
u64 get_cpu_iowait_ಸಮಯ_us(ಪೂರ್ಣಾಂಕ cpu, u64 *ಕೊನೆಯ_update_ಸಮಯ)
{
	ಹಿಂತಿರುಗಿ get_cpu_sleep_ಸಮಯ_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), ಕೊನೆಯ_update_ಸಮಯ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(get_cpu_iowait_ಸಮಯ_us);

/*
 * Use precise platform statistics if available:
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

ಶೂನ್ಯ vtime_account_irq(ರಚನೆ task_struct *tsk, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ pc = irq_ಎಣಿಕೆ() - ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (pc & HARDIRQ_ಸ್ಥಾನಾಂತರ) {
		vtime_account_hardirq(tsk);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pc & SOFTIRQ_ಸ್ಥಾನಾಂತರ) {
		vtime_account_softirq(tsk);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!kcpustat_idle_dyntick()) {
		ಆದರೆ (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} ಇಲ್ಲದಿದ್ದರೆ {
			vtime_account_kernel(tsk);
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		vtime_reset();
	}
}

ಶೂನ್ಯ cputime_adjust(ರಚನೆ task_cputime *curr, ರಚನೆ prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

ಶೂನ್ಯ task_cputime_adjusted(ರಚನೆ task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_ಚಿಹ್ನೆ_GPL(task_cputime_adjusted);

ಶೂನ್ಯ thread_ಗುಂಪು_cputime_adjusted(ರಚನೆ task_struct *p, u64 *ut, u64 *st)
{
	ರಚನೆ task_cputime cputime;

	thread_ಗುಂಪು_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
ಶೂನ್ಯ account_process_tick(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ user_tick)
{
	u64 cputime, steal;

	ಆದರೆ (vtime_accounting_enabled_this_cpu())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (kcpustat_idle_dyntick())
		ಹಿಂತಿರುಗಿ;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	ಆದರೆ (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		ಹಿಂತಿರುಗಿ;
	}

	steal = steal_account_process_ಸಮಯ(ULONG_MAX);

	ಆದರೆ (steal >= cputime)
		ಹಿಂತಿರುಗಿ;

	cputime -= steal;

	ಆದರೆ (user_tick)
		account_user_ಸಮಯ(p, cputime);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((p != this_rq()->idle) || (irq_ಎಣಿಕೆ() != HARDIRQ_ಸ್ಥಾನಾಂತರ))
		account_system_ಸಮಯ(p, HARDIRQ_ಸ್ಥಾನಾಂತರ, cputime);
	ಇಲ್ಲದಿದ್ದರೆ
		account_idle_ಸಮಯ(cputime);
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
ಶೂನ್ಯ cputime_adjust(ರಚನೆ task_cputime *curr, ರಚನೆ prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	u64 rtime, stime, utime;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	/* Serialize concurrent callers such that we can honour our guarantees */
	raw_spin_ಬೀಗ_irqsave(&prev->ಬೀಗ, flags);
	rtime = curr->ಮೊತ್ತ_exec_runtime;

	/*
	 * This is possible under two circumstances:
	 *  - rtime isn't monotonic after all (a bug);
	 *  - we got reordered by the lock.
	 *
	 * In both cases this acts as a filter such that the rest of the code
	 * can assume it is monotonic regardless of anything else.
	 */
	ಆದರೆ (prev->stime + prev->utime >= rtime)
		ಹೋಗು ಹೊರಹೋಗು;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	ಆದರೆ (stime == 0) {
		utime = rtime;
		ಹೋಗು update;
	}

	ಆದರೆ (utime == 0) {
		stime = rtime;
		ಹೋಗು update;
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
	ಆದರೆ (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	ಆದರೆ (utime < prev->utime) {
		utime = prev->utime;
		stime = rtime - utime;
	}

	prev->stime = stime;
	prev->utime = utime;
ಹೊರಹೋಗು:
	*ut = prev->utime;
	*st = prev->stime;
	raw_spin_unlock_irqrestore(&prev->ಬೀಗ, flags);
}

ಶೂನ್ಯ task_cputime_adjusted(ರಚನೆ task_struct *p, u64 *ut, u64 *st)
{
	ರಚನೆ task_cputime cputime = {
		.ಮೊತ್ತ_exec_runtime = p->se.ಮೊತ್ತ_exec_runtime,
	};

	ಆದರೆ (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.ಮೊತ್ತ_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_ಚಿಹ್ನೆ_GPL(task_cputime_adjusted);

ಶೂನ್ಯ thread_ಗುಂಪು_cputime_adjusted(ರಚನೆ task_struct *p, u64 *ut, u64 *st)
{
	ರಚನೆ task_cputime cputime;

	thread_ಗುಂಪು_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->ಸಂಕೇತ->prev_cputime, ut, st);
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_VIRT_CPU_ACCOUNTING_GEN
ಸ್ಥಿರ u64 vtime_delta(ರಚನೆ vtime *vtime)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ clock;

	clock = sched_clock();
	ಆದರೆ (clock < vtime->starttime)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ clock - vtime->starttime;
}

ಸ್ಥಿರ u64 get_vtime_delta(ರಚನೆ vtime *vtime)
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
	other = account_other_ಸಮಯ(delta);
	WARN_ON_ONCE(vtime->state == VTIME_INACTIVE);
	vtime->starttime += delta;

	ಹಿಂತಿರುಗಿ delta - other;
}

ಸ್ಥಿರ ಶೂನ್ಯ vtime_account_system(ರಚನೆ task_struct *tsk,
				 ರಚನೆ vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	ಆದರೆ (vtime->stime >= TICK_NSEC) {
		account_system_ಸಮಯ(tsk, irq_ಎಣಿಕೆ(), vtime->stime);
		vtime->stime = 0;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ vtime_account_guest(ರಚನೆ task_struct *tsk,
				ರಚನೆ vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	ಆದರೆ (vtime->gtime >= TICK_NSEC) {
		account_guest_ಸಮಯ(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __vtime_account_kernel(ರಚನೆ task_struct *tsk,
				   ರಚನೆ vtime *vtime)
{
	/* We might have scheduled out from guest path */
	ಆದರೆ (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	ಇಲ್ಲದಿದ್ದರೆ
		vtime_account_system(tsk, vtime);
}

ಶೂನ್ಯ vtime_account_kernel(ರಚನೆ task_struct *tsk)
{
	ರಚನೆ vtime *vtime = &tsk->vtime;

	ಆದರೆ (!vtime_delta(vtime))
		ಹಿಂತಿರುಗಿ;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_ಕೊನೆ(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_user_enter(ರಚನೆ task_struct *tsk)
{
	ರಚನೆ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_ಕೊನೆ(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_user_exit(ರಚನೆ task_struct *tsk)
{
	ರಚನೆ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	ಆದರೆ (vtime->utime >= TICK_NSEC) {
		account_user_ಸಮಯ(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_ಕೊನೆ(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_guest_enter(ರಚನೆ task_struct *tsk)
{
	ರಚನೆ vtime *vtime = &tsk->vtime;
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
	write_seqcount_ಕೊನೆ(&vtime->seqcount);
}
EXPORT_ಚಿಹ್ನೆ_GPL(vtime_guest_enter);

ಶೂನ್ಯ vtime_guest_exit(ರಚನೆ task_struct *tsk)
{
	ರಚನೆ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_ಕೊನೆ(&vtime->seqcount);
}
EXPORT_ಚಿಹ್ನೆ_GPL(vtime_guest_exit);

ಸ್ಥಿರ ಶೂನ್ಯ __vtime_account_idle(ರಚನೆ vtime *vtime)
{
	account_idle_ಸಮಯ(get_vtime_delta(vtime));
}

ಶೂನ್ಯ vtime_task_switch_generic(ರಚನೆ task_struct *prev)
{
	ರಚನೆ vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ಆದರೆ (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	ಇಲ್ಲದಿದ್ದರೆ
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_ಕೊನೆ(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ಆದರೆ (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	ಇಲ್ಲದಿದ್ದರೆ
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_ಕೊನೆ(&vtime->seqcount);
}

ಶೂನ್ಯ vtime_init_idle(ರಚನೆ task_struct *t, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ vtime *vtime = &t->vtime;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_ಕೊನೆ(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(ರಚನೆ task_struct *t)
{
	ರಚನೆ vtime *vtime = &t->vtime;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seq;
	u64 gtime;

	ಆದರೆ (!vtime_accounting_enabled())
		ಹಿಂತಿರುಗಿ t->gtime;

	ಮಾಡು {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		ಆದರೆ (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} ಆಗ (read_seqcount_retry(&vtime->seqcount, seq));

	ಹಿಂತಿರುಗಿ gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(ರಚನೆ task_struct *t, u64 *utime, u64 *stime)
{
	ರಚನೆ vtime *vtime = &t->vtime;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seq;
	u64 delta;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		ಹಿಂತಿರುಗಿ false;
	}

	ಮಾಡು {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		ಆದರೆ (vtime->state < VTIME_SYS)
			ಮುಂದುವರಿ;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ಆದರೆ (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		ಇಲ್ಲದಿದ್ದರೆ
			*utime += vtime->utime + delta;
	} ಆಗ (read_seqcount_retry(&vtime->seqcount, seq));

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ vtime_state_fetch(ರಚನೆ vtime *vtime, ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	ಆದರೆ (vtime->cpu != cpu && vtime->cpu != -1)
		ಹಿಂತಿರುಗಿ -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	ಆದರೆ (state == VTIME_INACTIVE)
		ಹಿಂತಿರುಗಿ -EAGAIN;

	ಹಿಂತಿರುಗಿ state;
}

ಸ್ಥಿರ u64 kcpustat_user_vtime(ರಚನೆ vtime *vtime)
{
	ಆದರೆ (vtime->state == VTIME_USER)
		ಹಿಂತಿರುಗಿ vtime->utime + vtime_delta(vtime);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (vtime->state == VTIME_GUEST)
		ಹಿಂತಿರುಗಿ vtime->gtime + vtime_delta(vtime);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ kcpustat_ಕ್ಷೇತ್ರ_vtime(u64 *cpustat,
				ರಚನೆ task_struct *tsk,
				ಎಣಿಕೆ cpu_usage_stat usage,
				ಪೂರ್ಣಾಂಕ cpu, u64 *val)
{
	ರಚನೆ vtime *vtime = &tsk->vtime;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seq;

	ಮಾಡು {
		ಪೂರ್ಣಾಂಕ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ಆದರೆ (state < 0)
			ಹಿಂತಿರುಗಿ state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		ಆಯ್ಕೆ (usage) {
		ಸ್ಥಿತಿ CPUTIME_SYSTEM:
			ಆದರೆ (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ CPUTIME_USER:
			ಆದರೆ (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ CPUTIME_NICE:
			ಆದರೆ (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ CPUTIME_GUEST:
			ಆದರೆ (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ CPUTIME_GUEST_NICE:
			ಆದರೆ (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ CPUTIME_IDLE:
			ಆದರೆ (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ CPUTIME_IOWAIT:
			ಆದರೆ (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			ನಿಲ್ಲಿಸು;
		}
	} ಆಗ (read_seqcount_retry(&vtime->seqcount, seq));

	ಹಿಂತಿರುಗಿ 0;
}

u64 kcpustat_ಕ್ಷೇತ್ರ(ಎಣಿಕೆ cpu_usage_stat usage, ಪೂರ್ಣಾಂಕ cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	ರಚನೆ rq *rq;
	ಪೂರ್ಣಾಂಕ err;

	ಆದರೆ (!vtime_generic_enabled_cpu(cpu))
		ಹಿಂತಿರುಗಿ kcpustat_ಕ್ಷೇತ್ರ_default(usage, cpu);

	rq = cpu_rq(cpu);

	ಗಾಗಿ (;;) {
		ರಚನೆ task_struct *curr;

		rcu_read_ಬೀಗ();
		curr = rcu_dereference(rq->curr);
		ಆದರೆ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			ಹಿಂತಿರುಗಿ cpustat[usage];
		}

		err = kcpustat_ಕ್ಷೇತ್ರ_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		ಆದರೆ (!err)
			ಹಿಂತಿರುಗಿ val;

		cpu_relax();
	}
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_ಕ್ಷೇತ್ರ);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ kcpustat_cpu_fetch_vtime(ರಚನೆ kernel_cpustat *dst,
				    ಸ್ಥಿರಾಂಕ ರಚನೆ kernel_cpustat *src,
				    ರಚನೆ task_struct *tsk, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ vtime *vtime = &tsk->vtime;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seq;

	ಮಾಡು {
		u64 *cpustat;
		u64 delta;
		ಪೂರ್ಣಾಂಕ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ಆದರೆ (state < 0)
			ಹಿಂತಿರುಗಿ state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		ಆದರೆ (state < VTIME_IDLE)
			ಮುಂದುವರಿ;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ಆಯ್ಕೆ (state) {
		ಸ್ಥಿತಿ VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ VTIME_USER:
			ಆದರೆ (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			ಇಲ್ಲದಿದ್ದರೆ
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ VTIME_GUEST:
			ಆದರೆ (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} ಇಲ್ಲದಿದ್ದರೆ {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ VTIME_IDLE:
			ಆದರೆ (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			ಇಲ್ಲದಿದ್ದರೆ
				cpustat[CPUTIME_IDLE] += delta;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			WARN_ON_ONCE(1);
		}
	} ಆಗ (read_seqcount_retry(&vtime->seqcount, seq));

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ kcpustat_cpu_fetch(ರಚನೆ kernel_cpustat *dst, ಪೂರ್ಣಾಂಕ cpu)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ kernel_cpustat *src = &kcpustat_cpu(cpu);
	ರಚನೆ rq *rq;
	ಪೂರ್ಣಾಂಕ err;

	ಆದರೆ (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		ಹಿಂತಿರುಗಿ;
	}

	rq = cpu_rq(cpu);

	ಗಾಗಿ (;;) {
		ರಚನೆ task_struct *curr;

		rcu_read_ಬೀಗ();
		curr = rcu_dereference(rq->curr);
		ಆದರೆ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			ಹಿಂತಿರುಗಿ;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		ಆದರೆ (!err)
			ಹಿಂತಿರುಗಿ;

		cpu_relax();
	}
}
EXPORT_ಚಿಹ್ನೆ_GPL(kcpustat_cpu_fetch);

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
