// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cputime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/tsacct_kern.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #ਅਨ੍ਤਰ੍ਭਾਵ <asm/cputime.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_clock_irqtime);

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
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ irqtime, cpu_irqtime);

ਖਾਲੀ enable_sched_clock_irqtime(ਖਾਲੀ)
{
	static_branch_enable(&sched_clock_irqtime);
}

ਖਾਲੀ disable_sched_clock_irqtime(ਖਾਲੀ)
{
	ਜੇ (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

ਸਥਿਰ ਖਾਲੀ irqtime_account_delta(ਬਣਤਰ irqtime *irqtime, u64 delta,
				  ਗਿਣਤੀ cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->ਕੁੱਲ += delta;
	ਜੇ (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_ਸਿਰਾ(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
ਖਾਲੀ irqtime_account_irq(ਬਣਤਰ task_struct *curr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
{
	ਬਣਤਰ irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pc;
	s64 delta;
	ਪੂਰਨ cpu;

	ਜੇ (!irqtime_enabled())
		ਵਾਪਸ;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_ਸ਼ੁਰੂ_ਸਮਾਂ;
	irqtime->irq_ਸ਼ੁਰੂ_ਸਮਾਂ += delta;
	pc = irq_ਗਿਣਤੀ() - ਵਿੱਥ;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	ਜੇ (pc & HARDIRQ_ਨਕਾਬ)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	ਨਹੀਂਤਾਂ ਜੇ ((pc & SOFTIRQ_ਵਿੱਥ) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

ਸਥਿਰ u64 irqtime_tick_accounted(u64 maxtime)
{
	ਬਣਤਰ irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	ਵਾਪਸ delta;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ਸਥਿਰ u64 irqtime_tick_accounted(u64 dummy)
{
	ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ /* !CONFIG_IRQ_TIME_ACCOUNTING */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ task_ਸਮੂਹ_account_ਖੇਤਰ(ਬਣਤਰ task_struct *p, ਪੂਰਨ ਸੂਚਕਾਂਕ,
					    u64 tmp)
{
	/*
	 * Since all updates are sure to touch the root cgroup, we
	 * get ourselves ahead and touch it first. If the root cgroup
	 * is the only cgroup, then nothing else should be necessary.
	 *
	 */
	__this_cpu_add(kernel_cpustat.cpustat[ਸੂਚਕਾਂਕ], tmp);

	cgroup_account_cputime_ਖੇਤਰ(p, ਸੂਚਕਾਂਕ, tmp);
}

/*
 * Account user CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in user space since the last update
 */
ਖਾਲੀ account_user_ਸਮਾਂ(ਬਣਤਰ task_struct *p, u64 cputime)
{
	ਪੂਰਨ ਸੂਚਕਾਂਕ;

	/* Add user time to process. */
	p->utime += cputime;
	account_ਸਮੂਹ_user_ਸਮਾਂ(p, cputime);

	ਸੂਚਕਾਂਕ = (task_nice(p) > 0) ? CPUTIME_NICE : CPUTIME_USER;

	/* Add user time to cpustat. */
	task_ਸਮੂਹ_account_ਖੇਤਰ(p, ਸੂਚਕਾਂਕ, cputime);

	/* Account for user time used */
	acct_account_cputime(p);
}

/*
 * Account guest CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in virtual machine since the last update
 */
ਖਾਲੀ account_guest_ਸਮਾਂ(ਬਣਤਰ task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_ਸਮੂਹ_user_ਸਮਾਂ(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	ਜੇ (task_nice(p) > 0) {
		task_ਸਮੂਹ_account_ਖੇਤਰ(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} ਨਹੀਂਤਾਂ {
		task_ਸਮੂਹ_account_ਖੇਤਰ(p, CPUTIME_USER, cputime);
		cpustat[CPUTIME_GUEST] += cputime;
	}
}

/*
 * Account system CPU time to a process and desired cpustat field
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in kernel space since the last update
 * @index: pointer to cpustat field that has to be updated
 */
ਖਾਲੀ account_system_ਸੂਚਕਾਂਕ_ਸਮਾਂ(ਬਣਤਰ task_struct *p,
			       u64 cputime, ਗਿਣਤੀ cpu_usage_stat ਸੂਚਕਾਂਕ)
{
	/* Add system time to process. */
	p->stime += cputime;
	account_ਸਮੂਹ_system_ਸਮਾਂ(p, cputime);

	/* Add system time to cpustat. */
	task_ਸਮੂਹ_account_ਖੇਤਰ(p, ਸੂਚਕਾਂਕ, cputime);

	/* Account for system time used */
	acct_account_cputime(p);
}

/*
 * Account system CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @hardirq_offset: the offset to subtract from hardirq_count()
 * @cputime: the CPU time spent in kernel space since the last update
 */
ਖਾਲੀ account_system_ਸਮਾਂ(ਬਣਤਰ task_struct *p, ਪੂਰਨ hardirq_ਵਿੱਥ, u64 cputime)
{
	ਪੂਰਨ ਸੂਚਕਾਂਕ;

	ਜੇ ((p->flags & PF_VCPU) && (irq_ਗਿਣਤੀ() - hardirq_ਵਿੱਥ == 0)) {
		account_guest_ਸਮਾਂ(p, cputime);
		ਵਾਪਸ;
	}

	ਜੇ (hardirq_ਗਿਣਤੀ() - hardirq_ਵਿੱਥ)
		ਸੂਚਕਾਂਕ = CPUTIME_IRQ;
	ਨਹੀਂਤਾਂ ਜੇ (ਵਿੱਚ_serving_softirq())
		ਸੂਚਕਾਂਕ = CPUTIME_SOFTIRQ;
	ਨਹੀਂਤਾਂ
		ਸੂਚਕਾਂਕ = CPUTIME_SYSTEM;

	account_system_ਸੂਚਕਾਂਕ_ਸਮਾਂ(p, cputime, ਸੂਚਕਾਂਕ);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
ਖਾਲੀ account_steal_ਸਮਾਂ(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
ਖਾਲੀ account_idle_ਸਮਾਂ(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	ਜੇ (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	ਨਹੀਂਤਾਂ
		cpustat[CPUTIME_IDLE] += cputime;
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
ਖਾਲੀ __account_forceidle_time(ਬਣਤਰ task_struct *p, u64 delta)
{
	__schedstat_add(p->stats.core_forceidle_ਜੋੜ, delta);

	task_ਸਮੂਹ_account_ਖੇਤਰ(p, CPUTIME_FORCEIDLE, delta);
}
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CORE */

/*
 * When a guest is interrupted for a longer amount of time, missed clock
 * ticks are not redelivered later. Due to that, this function may on
 * occasion account more time than the calling functions think elapsed.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PARAVIRT
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(paravirt_steal_enabled);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HAVE_PV_STEAL_CLOCK_GEN
ਸਥਿਰ u64 native_steal_clock(ਪੂਰਨ cpu)
{
	ਵਾਪਸ 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਸਥਿਰ __always_inline u64 steal_account_process_ਸਮਾਂ(u64 maxtime)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PARAVIRT
	ਜੇ (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_ਸਮਾਂ;
		steal = min(steal, maxtime);
		account_steal_ਸਮਾਂ(steal);
		this_rq()->prev_steal_ਸਮਾਂ += steal;

		ਵਾਪਸ steal;
	}
#ਜੇ_ਅੰਤ /* CONFIG_PARAVIRT */
	ਵਾਪਸ 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 account_other_ਸਮਾਂ(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_ਸਮਾਂ(max);

	ਜੇ (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	ਵਾਪਸ accounted;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 read_ਜੋੜ_exec_runtime(ਬਣਤਰ task_struct *t)
{
	ਵਾਪਸ t->se.ਜੋੜ_exec_runtime;
}
#ਨਹੀਂਤਾਂ /* !CONFIG_64BIT: */
ਸਥਿਰ u64 read_ਜੋੜ_exec_runtime(ਬਣਤਰ task_struct *t)
{
	u64 ns;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	rq = task_rq_ਤਾਲਾ_2(t, &rf);
	ns = t->se.ਜੋੜ_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	ਵਾਪਸ ns;
}
#ਜੇ_ਅੰਤ /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
ਖਾਲੀ thread_ਸਮੂਹ_cputime(ਬਣਤਰ task_struct *tsk, ਬਣਤਰ task_cputime *times)
{
	ਬਣਤਰ signal_struct *sig = tsk->ਸੰਕੇਤ;
	ਬਣਤਰ task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	ਜੇ (same_thread_ਸਮੂਹ(current, tsk))
		(ਖਾਲੀ) task_sched_runtime(current);

	guard(rcu)();
	scoped_seqlock_read (&sig->stats_ਤਾਲਾ, ss_ਤਾਲਾ_irqsave) {
		times->utime = sig->utime;
		times->stime = sig->stime;
		times->ਜੋੜ_exec_runtime = sig->ਜੋੜ_sched_runtime;

		__for_each_thread(sig, t) {
			task_cputime(t, &utime, &stime);
			times->utime += utime;
			times->stime += stime;
			times->ਜੋੜ_exec_runtime += read_ਜੋੜ_exec_runtime(t);
		}
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
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
ਸਥਿਰ ਖਾਲੀ irqtime_account_process_tick(ਬਣਤਰ task_struct *p, ਪੂਰਨ user_tick,
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
	other = account_other_ਸਮਾਂ(ULONG_MAX);
	ਜੇ (other >= cputime)
		ਵਾਪਸ;

	cputime -= other;

	ਜੇ (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_ਸੂਚਕਾਂਕ_ਸਮਾਂ(p, cputime, CPUTIME_SOFTIRQ);
	} ਨਹੀਂਤਾਂ ਜੇ (user_tick) {
		account_user_ਸਮਾਂ(p, cputime);
	} ਨਹੀਂਤਾਂ ਜੇ (p == this_rq()->idle) {
		account_idle_ਸਮਾਂ(cputime);
	} ਨਹੀਂਤਾਂ ਜੇ (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_ਸਮਾਂ(p, cputime);
	} ਨਹੀਂਤਾਂ {
		account_system_ਸੂਚਕਾਂਕ_ਸਮਾਂ(p, cputime, CPUTIME_SYSTEM);
	}
}

#ਨਹੀਂਤਾਂ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ irqtime_account_process_tick(ਬਣਤਰ task_struct *p, ਪੂਰਨ user_tick,
						u64 cputime) { }
#ਜੇ_ਅੰਤ /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ਜੇ defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
ਸਥਿਰ ਖਾਲੀ kcpustat_idle_stop(ਬਣਤਰ kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	ਪੂਰਨ iowait;

	ਜੇ (!kc->idle_elapse)
		ਵਾਪਸ;

	iowait = cpu_has_iowait(smp_processor_id());
	delta = now - kc->idle_entrytime;
	steal = steal_account_process_ਸਮਾਂ(delta);

	/*
	 * Record the idle time after substracting the steal time from
	 * previous update sequence. Don't substract the steal time from
	 * the current update sequence to avoid readers moving backward.
	 */
	write_seqcount_begin(&kc->idle_sleeptime_seq);
	steal_delta = min_t(u64, kc->idle_stealtime[iowait], delta);
	delta -= steal_delta;
	kc->idle_stealtime[iowait] -= steal_delta;

	ਜੇ (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	ਨਹੀਂਤਾਂ
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_ਸਿਰਾ(&kc->idle_sleeptime_seq);
}

ਸਥਿਰ ਖਾਲੀ kcpustat_idle_ਸ਼ੁਰੂ(ਬਣਤਰ kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	ਜੇ (kc->idle_elapse)
		ਵਾਪਸ;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_ਸਿਰਾ(&kc->idle_sleeptime_seq);
}

ਖਾਲੀ kcpustat_dyntick_stop(u64 now, u64 tick_ਸ਼ੁਰੂ)
{
	ਬਣਤਰ kernel_cpustat *kc = kcpustat_this_cpu;

	ਜੇ (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		ਜੇ (kc->idle_tick_period != tick_ਸ਼ੁਰੂ) {
			kc->idle_tick_period = tick_ਸ਼ੁਰੂ;
			kc->idle_tick_overlap = 0;
		}
		tick_ਸ਼ੁਰੂ = max(tick_ਸ਼ੁਰੂ, kc->idle_dyntick_ਇੰਦਰਾਜ);
		ਜੇ (!WARN_ON_ONCE(now < tick_ਸ਼ੁਰੂ))
			kc->idle_tick_overlap += now - tick_ਸ਼ੁਰੂ;
		kcpustat_idle_stop(kc, now);
		kc->idle_dyntick = false;
		vtime_dyntick_stop();
	}
}

/*
 * The first tick after dyntick-idle covers a period that the dyntick-idle
 * accounting may already have accounted in part.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 kcpustat_tick_overlap(ਖਾਲੀ)
{
	ਬਣਤਰ kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	ਵਾਪਸ overlap;
}

ਖਾਲੀ kcpustat_dyntick_ਸ਼ੁਰੂ(u64 now)
{
	ਬਣਤਰ kernel_cpustat *kc = kcpustat_this_cpu;

	ਜੇ (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_ਸ਼ੁਰੂ();
		kc->idle_dyntick = true;
		kc->idle_dyntick_ਇੰਦਰਾਜ = now;
		kcpustat_idle_ਸ਼ੁਰੂ(kc, now);
	}
}

ਖਾਲੀ kcpustat_irq_enter(u64 now)
{
	ਬਣਤਰ kernel_cpustat *kc = kcpustat_this_cpu;

	ਜੇ (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

ਖਾਲੀ kcpustat_irq_exit(u64 now)
{
	ਬਣਤਰ kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	ਜੇ (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_ਸ਼ੁਰੂ(kc, now);
}

ਸਥਿਰ u64 kcpustat_ਖੇਤਰ_dyntick(ਪੂਰਨ cpu, ਗਿਣਤੀ cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	ਬਣਤਰ kernel_cpustat *kc = &kcpustat_cpu(cpu);
	ਪੂਰਨ iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq;
	u64 idle;

	ਕਰੋ {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		ਜੇ (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} ਜਦੋਂ (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	ਵਾਪਸ idle;
}

u64 kcpustat_ਖੇਤਰ_idle(ਪੂਰਨ cpu)
{
	ਵਾਪਸ kcpustat_ਖੇਤਰ_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(kcpustat_ਖੇਤਰ_idle);

u64 kcpustat_ਖੇਤਰ_iowait(ਪੂਰਨ cpu)
{
	ਵਾਪਸ kcpustat_ਖੇਤਰ_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(kcpustat_ਖੇਤਰ_iowait);
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 kcpustat_tick_overlap(ਖਾਲੀ)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ u64 kcpustat_ਖੇਤਰ_dyntick(ਪੂਰਨ cpu, ਗਿਣਤੀ cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	ਵਾਪਸ kcpustat_cpu(cpu).cpustat[idx];
}
#ਜੇ_ਅੰਤ /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

ਸਥਿਰ u64 get_cpu_sleep_ਸਮਾਂ_us(ਪੂਰਨ cpu, ਗਿਣਤੀ cpu_usage_stat idx,
				 bool compute_delta, u64 *ਆਖਰੀ_update_ਸਮਾਂ)
{
	ktime_t now = ktime_get();
	u64 res;

	ਜੇ (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_ਖੇਤਰ(idx, cpu);
	ਨਹੀਂਤਾਂ
		res = kcpustat_ਖੇਤਰ_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_ਪ੍ਰਤੀ_USEC);

	ਜੇ (ਆਖਰੀ_update_ਸਮਾਂ)
		*ਆਖਰੀ_update_ਸਮਾਂ = ktime_ਮੰਜ਼ਿਲ_us(now);

	ਵਾਪਸ res;
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
u64 get_cpu_idle_ਸਮਾਂ_us(ਪੂਰਨ cpu, u64 *ਆਖਰੀ_update_ਸਮਾਂ)
{
	ਵਾਪਸ get_cpu_sleep_ਸਮਾਂ_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), ਆਖਰੀ_update_ਸਮਾਂ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(get_cpu_idle_ਸਮਾਂ_us);

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
u64 get_cpu_iowait_ਸਮਾਂ_us(ਪੂਰਨ cpu, u64 *ਆਖਰੀ_update_ਸਮਾਂ)
{
	ਵਾਪਸ get_cpu_sleep_ਸਮਾਂ_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), ਆਖਰੀ_update_ਸਮਾਂ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(get_cpu_iowait_ਸਮਾਂ_us);

/*
 * Use precise platform statistics if available:
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

ਖਾਲੀ vtime_account_irq(ਬਣਤਰ task_struct *tsk, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ pc = irq_ਗਿਣਤੀ() - ਵਿੱਥ;

	ਜੇ (pc & HARDIRQ_ਵਿੱਥ) {
		vtime_account_hardirq(tsk);
	} ਨਹੀਂਤਾਂ ਜੇ (pc & SOFTIRQ_ਵਿੱਥ) {
		vtime_account_softirq(tsk);
	} ਨਹੀਂਤਾਂ ਜੇ (!kcpustat_idle_dyntick()) {
		ਜੇ (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} ਨਹੀਂਤਾਂ {
			vtime_account_kernel(tsk);
		}
	} ਨਹੀਂਤਾਂ {
		vtime_reset();
	}
}

ਖਾਲੀ cputime_adjust(ਬਣਤਰ task_cputime *curr, ਬਣਤਰ prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

ਖਾਲੀ task_cputime_adjusted(ਬਣਤਰ task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(task_cputime_adjusted);

ਖਾਲੀ thread_ਸਮੂਹ_cputime_adjusted(ਬਣਤਰ task_struct *p, u64 *ut, u64 *st)
{
	ਬਣਤਰ task_cputime cputime;

	thread_ਸਮੂਹ_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
ਖਾਲੀ account_process_tick(ਬਣਤਰ task_struct *p, ਪੂਰਨ user_tick)
{
	u64 cputime, steal;

	ਜੇ (vtime_accounting_enabled_this_cpu())
		ਵਾਪਸ;

	ਜੇ (kcpustat_idle_dyntick())
		ਵਾਪਸ;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	ਜੇ (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		ਵਾਪਸ;
	}

	steal = steal_account_process_ਸਮਾਂ(ULONG_MAX);

	ਜੇ (steal >= cputime)
		ਵਾਪਸ;

	cputime -= steal;

	ਜੇ (user_tick)
		account_user_ਸਮਾਂ(p, cputime);
	ਨਹੀਂਤਾਂ ਜੇ ((p != this_rq()->idle) || (irq_ਗਿਣਤੀ() != HARDIRQ_ਵਿੱਥ))
		account_system_ਸਮਾਂ(p, HARDIRQ_ਵਿੱਥ, cputime);
	ਨਹੀਂਤਾਂ
		account_idle_ਸਮਾਂ(cputime);
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
ਖਾਲੀ cputime_adjust(ਬਣਤਰ task_cputime *curr, ਬਣਤਰ prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	u64 rtime, stime, utime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	/* Serialize concurrent callers such that we can honour our guarantees */
	raw_spin_ਤਾਲਾ_irqsave(&prev->ਤਾਲਾ, flags);
	rtime = curr->ਜੋੜ_exec_runtime;

	/*
	 * This is possible under two circumstances:
	 *  - rtime isn't monotonic after all (a bug);
	 *  - we got reordered by the lock.
	 *
	 * In both cases this acts as a filter such that the rest of the code
	 * can assume it is monotonic regardless of anything else.
	 */
	ਜੇ (prev->stime + prev->utime >= rtime)
		ਜਾਓ ਬਾਹਰਲਾ;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	ਜੇ (stime == 0) {
		utime = rtime;
		ਜਾਓ update;
	}

	ਜੇ (utime == 0) {
		stime = rtime;
		ਜਾਓ update;
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
	ਜੇ (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	ਜੇ (utime < prev->utime) {
		utime = prev->utime;
		stime = rtime - utime;
	}

	prev->stime = stime;
	prev->utime = utime;
ਬਾਹਰਲਾ:
	*ut = prev->utime;
	*st = prev->stime;
	raw_spin_unlock_irqrestore(&prev->ਤਾਲਾ, flags);
}

ਖਾਲੀ task_cputime_adjusted(ਬਣਤਰ task_struct *p, u64 *ut, u64 *st)
{
	ਬਣਤਰ task_cputime cputime = {
		.ਜੋੜ_exec_runtime = p->se.ਜੋੜ_exec_runtime,
	};

	ਜੇ (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.ਜੋੜ_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(task_cputime_adjusted);

ਖਾਲੀ thread_ਸਮੂਹ_cputime_adjusted(ਬਣਤਰ task_struct *p, u64 *ut, u64 *st)
{
	ਬਣਤਰ task_cputime cputime;

	thread_ਸਮੂਹ_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->ਸੰਕੇਤ->prev_cputime, ut, st);
}
#ਜੇ_ਅੰਤ /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_VIRT_CPU_ACCOUNTING_GEN
ਸਥਿਰ u64 vtime_delta(ਬਣਤਰ vtime *vtime)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ clock;

	clock = sched_clock();
	ਜੇ (clock < vtime->starttime)
		ਵਾਪਸ 0;

	ਵਾਪਸ clock - vtime->starttime;
}

ਸਥਿਰ u64 get_vtime_delta(ਬਣਤਰ vtime *vtime)
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
	other = account_other_ਸਮਾਂ(delta);
	WARN_ON_ONCE(vtime->state == VTIME_INACTIVE);
	vtime->starttime += delta;

	ਵਾਪਸ delta - other;
}

ਸਥਿਰ ਖਾਲੀ vtime_account_system(ਬਣਤਰ task_struct *tsk,
				 ਬਣਤਰ vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	ਜੇ (vtime->stime >= TICK_NSEC) {
		account_system_ਸਮਾਂ(tsk, irq_ਗਿਣਤੀ(), vtime->stime);
		vtime->stime = 0;
	}
}

ਸਥਿਰ ਖਾਲੀ vtime_account_guest(ਬਣਤਰ task_struct *tsk,
				ਬਣਤਰ vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	ਜੇ (vtime->gtime >= TICK_NSEC) {
		account_guest_ਸਮਾਂ(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

ਸਥਿਰ ਖਾਲੀ __vtime_account_kernel(ਬਣਤਰ task_struct *tsk,
				   ਬਣਤਰ vtime *vtime)
{
	/* We might have scheduled out from guest path */
	ਜੇ (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	ਨਹੀਂਤਾਂ
		vtime_account_system(tsk, vtime);
}

ਖਾਲੀ vtime_account_kernel(ਬਣਤਰ task_struct *tsk)
{
	ਬਣਤਰ vtime *vtime = &tsk->vtime;

	ਜੇ (!vtime_delta(vtime))
		ਵਾਪਸ;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);
}

ਖਾਲੀ vtime_user_enter(ਬਣਤਰ task_struct *tsk)
{
	ਬਣਤਰ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);
}

ਖਾਲੀ vtime_user_exit(ਬਣਤਰ task_struct *tsk)
{
	ਬਣਤਰ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	ਜੇ (vtime->utime >= TICK_NSEC) {
		account_user_ਸਮਾਂ(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);
}

ਖਾਲੀ vtime_guest_enter(ਬਣਤਰ task_struct *tsk)
{
	ਬਣਤਰ vtime *vtime = &tsk->vtime;
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
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(vtime_guest_enter);

ਖਾਲੀ vtime_guest_exit(ਬਣਤਰ task_struct *tsk)
{
	ਬਣਤਰ vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(vtime_guest_exit);

ਸਥਿਰ ਖਾਲੀ __vtime_account_idle(ਬਣਤਰ vtime *vtime)
{
	account_idle_ਸਮਾਂ(get_vtime_delta(vtime));
}

ਖਾਲੀ vtime_task_switch_generic(ਬਣਤਰ task_struct *prev)
{
	ਬਣਤਰ vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ਜੇ (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	ਨਹੀਂਤਾਂ
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ਜੇ (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	ਨਹੀਂਤਾਂ ਜੇ (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	ਨਹੀਂਤਾਂ
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);
}

ਖਾਲੀ vtime_init_idle(ਬਣਤਰ task_struct *t, ਪੂਰਨ cpu)
{
	ਬਣਤਰ vtime *vtime = &t->vtime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_ਸਿਰਾ(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(ਬਣਤਰ task_struct *t)
{
	ਬਣਤਰ vtime *vtime = &t->vtime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq;
	u64 gtime;

	ਜੇ (!vtime_accounting_enabled())
		ਵਾਪਸ t->gtime;

	ਕਰੋ {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		ਜੇ (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} ਜਦੋਂ (read_seqcount_retry(&vtime->seqcount, seq));

	ਵਾਪਸ gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(ਬਣਤਰ task_struct *t, u64 *utime, u64 *stime)
{
	ਬਣਤਰ vtime *vtime = &t->vtime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq;
	u64 delta;
	ਪੂਰਨ ret;

	ਜੇ (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		ਵਾਪਸ false;
	}

	ਕਰੋ {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		ਜੇ (vtime->state < VTIME_SYS)
			ਜਾਰੀ;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ਜੇ (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		ਨਹੀਂਤਾਂ
			*utime += vtime->utime + delta;
	} ਜਦੋਂ (read_seqcount_retry(&vtime->seqcount, seq));

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ vtime_state_fetch(ਬਣਤਰ vtime *vtime, ਪੂਰਨ cpu)
{
	ਪੂਰਨ state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	ਜੇ (vtime->cpu != cpu && vtime->cpu != -1)
		ਵਾਪਸ -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	ਜੇ (state == VTIME_INACTIVE)
		ਵਾਪਸ -EAGAIN;

	ਵਾਪਸ state;
}

ਸਥਿਰ u64 kcpustat_user_vtime(ਬਣਤਰ vtime *vtime)
{
	ਜੇ (vtime->state == VTIME_USER)
		ਵਾਪਸ vtime->utime + vtime_delta(vtime);
	ਨਹੀਂਤਾਂ ਜੇ (vtime->state == VTIME_GUEST)
		ਵਾਪਸ vtime->gtime + vtime_delta(vtime);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ kcpustat_ਖੇਤਰ_vtime(u64 *cpustat,
				ਬਣਤਰ task_struct *tsk,
				ਗਿਣਤੀ cpu_usage_stat usage,
				ਪੂਰਨ cpu, u64 *val)
{
	ਬਣਤਰ vtime *vtime = &tsk->vtime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq;

	ਕਰੋ {
		ਪੂਰਨ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ਜੇ (state < 0)
			ਵਾਪਸ state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		ਚੋਣ (usage) {
		ਹਾਲਤ CPUTIME_SYSTEM:
			ਜੇ (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			ਰੋਕੋ;
		ਹਾਲਤ CPUTIME_USER:
			ਜੇ (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			ਰੋਕੋ;
		ਹਾਲਤ CPUTIME_NICE:
			ਜੇ (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			ਰੋਕੋ;
		ਹਾਲਤ CPUTIME_GUEST:
			ਜੇ (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ਰੋਕੋ;
		ਹਾਲਤ CPUTIME_GUEST_NICE:
			ਜੇ (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ਰੋਕੋ;
		ਹਾਲਤ CPUTIME_IDLE:
			ਜੇ (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ਰੋਕੋ;
		ਹਾਲਤ CPUTIME_IOWAIT:
			ਜੇ (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ਰੋਕੋ;
		ਮੂਲ:
			ਰੋਕੋ;
		}
	} ਜਦੋਂ (read_seqcount_retry(&vtime->seqcount, seq));

	ਵਾਪਸ 0;
}

u64 kcpustat_ਖੇਤਰ(ਗਿਣਤੀ cpu_usage_stat usage, ਪੂਰਨ cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	ਬਣਤਰ rq *rq;
	ਪੂਰਨ err;

	ਜੇ (!vtime_generic_enabled_cpu(cpu))
		ਵਾਪਸ kcpustat_ਖੇਤਰ_default(usage, cpu);

	rq = cpu_rq(cpu);

	ਲਈ (;;) {
		ਬਣਤਰ task_struct *curr;

		rcu_read_ਤਾਲਾ();
		curr = rcu_dereference(rq->curr);
		ਜੇ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			ਵਾਪਸ cpustat[usage];
		}

		err = kcpustat_ਖੇਤਰ_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		ਜੇ (!err)
			ਵਾਪਸ val;

		cpu_relax();
	}
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(kcpustat_ਖੇਤਰ);

ਸਥਿਰ ਪੂਰਨ kcpustat_cpu_fetch_vtime(ਬਣਤਰ kernel_cpustat *dst,
				    ਅਚਲ ਬਣਤਰ kernel_cpustat *src,
				    ਬਣਤਰ task_struct *tsk, ਪੂਰਨ cpu)
{
	ਬਣਤਰ vtime *vtime = &tsk->vtime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq;

	ਕਰੋ {
		u64 *cpustat;
		u64 delta;
		ਪੂਰਨ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ਜੇ (state < 0)
			ਵਾਪਸ state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		ਜੇ (state < VTIME_IDLE)
			ਜਾਰੀ;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ਚੋਣ (state) {
		ਹਾਲਤ VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			ਰੋਕੋ;
		ਹਾਲਤ VTIME_USER:
			ਜੇ (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			ਨਹੀਂਤਾਂ
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			ਰੋਕੋ;
		ਹਾਲਤ VTIME_GUEST:
			ਜੇ (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} ਨਹੀਂਤਾਂ {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			ਰੋਕੋ;
		ਹਾਲਤ VTIME_IDLE:
			ਜੇ (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			ਨਹੀਂਤਾਂ
				cpustat[CPUTIME_IDLE] += delta;
			ਰੋਕੋ;
		ਮੂਲ:
			WARN_ON_ONCE(1);
		}
	} ਜਦੋਂ (read_seqcount_retry(&vtime->seqcount, seq));

	ਵਾਪਸ 0;
}

ਖਾਲੀ kcpustat_cpu_fetch(ਬਣਤਰ kernel_cpustat *dst, ਪੂਰਨ cpu)
{
	ਅਚਲ ਬਣਤਰ kernel_cpustat *src = &kcpustat_cpu(cpu);
	ਬਣਤਰ rq *rq;
	ਪੂਰਨ err;

	ਜੇ (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		ਵਾਪਸ;
	}

	rq = cpu_rq(cpu);

	ਲਈ (;;) {
		ਬਣਤਰ task_struct *curr;

		rcu_read_ਤਾਲਾ();
		curr = rcu_dereference(rq->curr);
		ਜੇ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			ਵਾਪਸ;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		ਜੇ (!err)
			ਵਾਪਸ;

		cpu_relax();
	}
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(kcpustat_cpu_fetch);

#ਜੇ_ਅੰਤ /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
