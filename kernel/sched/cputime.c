// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/sched/cputime.h>
#అన్తర్భావ <linux/tsacct_kern.h>
#అన్తర్భావ "sched.h"

#నిర్వచితమైతే CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #అన్తర్భావ <asm/cputime.h>
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING

DEFINE_STATIC_కీలకం_FALSE(sched_clock_irqtime);

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
DEFINE_చొప్పున_CPU(నిర్మాణం irqtime, cpu_irqtime);

శూన్య enable_sched_clock_irqtime(శూన్య)
{
	static_branch_enable(&sched_clock_irqtime);
}

శూన్య disable_sched_clock_irqtime(శూన్య)
{
	ఒకవేళ (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

స్థిర శూన్య irqtime_account_delta(నిర్మాణం irqtime *irqtime, u64 delta,
				  గణన cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->మొత్తం += delta;
	ఒకవేళ (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_చివర(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
శూన్య irqtime_account_irq(నిర్మాణం task_struct *curr, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
{
	నిర్మాణం irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	గుర్తులేని పూర్ణాంకం pc;
	s64 delta;
	పూర్ణాంకం cpu;

	ఒకవేళ (!irqtime_enabled())
		తిరిగి;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_ప్రారంభం_సమయం;
	irqtime->irq_ప్రారంభం_సమయం += delta;
	pc = irq_లెక్క() - స్థానభ్రంశం;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	ఒకవేళ (pc & HARDIRQ_ముసుగు)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	లేకపోతే ఒకవేళ ((pc & SOFTIRQ_స్థానభ్రంశం) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

స్థిర u64 irqtime_tick_accounted(u64 maxtime)
{
	నిర్మాణం irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	తిరిగి delta;
}

#లేకపోతే /* !CONFIG_IRQ_TIME_ACCOUNTING: */

స్థిర u64 irqtime_tick_accounted(u64 dummy)
{
	తిరిగి 0;
}

#అయితే_ముగింపు /* !CONFIG_IRQ_TIME_ACCOUNTING */

స్థిర అంతఃవరుస శూన్య task_సమూహం_account_క్షేత్రం(నిర్మాణం task_struct *p, పూర్ణాంకం సూచీ,
					    u64 tmp)
{
	/*
	 * Since all updates are sure to touch the root cgroup, we
	 * get ourselves ahead and touch it first. If the root cgroup
	 * is the only cgroup, then nothing else should be necessary.
	 *
	 */
	__this_cpu_add(kernel_cpustat.cpustat[సూచీ], tmp);

	cgroup_account_cputime_క్షేత్రం(p, సూచీ, tmp);
}

/*
 * Account user CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in user space since the last update
 */
శూన్య account_user_సమయం(నిర్మాణం task_struct *p, u64 cputime)
{
	పూర్ణాంకం సూచీ;

	/* Add user time to process. */
	p->utime += cputime;
	account_సమూహం_user_సమయం(p, cputime);

	సూచీ = (task_nice(p) > 0) ? CPUTIME_NICE : CPUTIME_USER;

	/* Add user time to cpustat. */
	task_సమూహం_account_క్షేత్రం(p, సూచీ, cputime);

	/* Account for user time used */
	acct_account_cputime(p);
}

/*
 * Account guest CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in virtual machine since the last update
 */
శూన్య account_guest_సమయం(నిర్మాణం task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_సమూహం_user_సమయం(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	ఒకవేళ (task_nice(p) > 0) {
		task_సమూహం_account_క్షేత్రం(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} లేకపోతే {
		task_సమూహం_account_క్షేత్రం(p, CPUTIME_USER, cputime);
		cpustat[CPUTIME_GUEST] += cputime;
	}
}

/*
 * Account system CPU time to a process and desired cpustat field
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in kernel space since the last update
 * @index: pointer to cpustat field that has to be updated
 */
శూన్య account_system_సూచీ_సమయం(నిర్మాణం task_struct *p,
			       u64 cputime, గణన cpu_usage_stat సూచీ)
{
	/* Add system time to process. */
	p->stime += cputime;
	account_సమూహం_system_సమయం(p, cputime);

	/* Add system time to cpustat. */
	task_సమూహం_account_క్షేత్రం(p, సూచీ, cputime);

	/* Account for system time used */
	acct_account_cputime(p);
}

/*
 * Account system CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @hardirq_offset: the offset to subtract from hardirq_count()
 * @cputime: the CPU time spent in kernel space since the last update
 */
శూన్య account_system_సమయం(నిర్మాణం task_struct *p, పూర్ణాంకం hardirq_స్థానభ్రంశం, u64 cputime)
{
	పూర్ణాంకం సూచీ;

	ఒకవేళ ((p->flags & PF_VCPU) && (irq_లెక్క() - hardirq_స్థానభ్రంశం == 0)) {
		account_guest_సమయం(p, cputime);
		తిరిగి;
	}

	ఒకవేళ (hardirq_లెక్క() - hardirq_స్థానభ్రంశం)
		సూచీ = CPUTIME_IRQ;
	లేకపోతే ఒకవేళ (లోపల_serving_softirq())
		సూచీ = CPUTIME_SOFTIRQ;
	లేకపోతే
		సూచీ = CPUTIME_SYSTEM;

	account_system_సూచీ_సమయం(p, cputime, సూచీ);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
శూన్య account_steal_సమయం(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
శూన్య account_idle_సమయం(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	ఒకవేళ (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	లేకపోతే
		cpustat[CPUTIME_IDLE] += cputime;
}


#నిర్వచితమైతే CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
శూన్య __account_forceidle_time(నిర్మాణం task_struct *p, u64 delta)
{
	__schedstat_add(p->stats.core_forceidle_కూడిక, delta);

	task_సమూహం_account_క్షేత్రం(p, CPUTIME_FORCEIDLE, delta);
}
#అయితే_ముగింపు /* CONFIG_SCHED_CORE */

/*
 * When a guest is interrupted for a longer amount of time, missed clock
 * ticks are not redelivered later. Due to that, this function may on
 * occasion account more time than the calling functions think elapsed.
 */
#నిర్వచితమైతే CONFIG_PARAVIRT
DEFINE_STATIC_కీలకం_FALSE(paravirt_steal_enabled);

#నిర్వచితమైతే CONFIG_HAVE_PV_STEAL_CLOCK_GEN
స్థిర u64 native_steal_clock(పూర్ణాంకం cpu)
{
	తిరిగి 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#అయితే_ముగింపు
#అయితే_ముగింపు

స్థిర __always_inline u64 steal_account_process_సమయం(u64 maxtime)
{
#నిర్వచితమైతే CONFIG_PARAVIRT
	ఒకవేళ (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_సమయం;
		steal = min(steal, maxtime);
		account_steal_సమయం(steal);
		this_rq()->prev_steal_సమయం += steal;

		తిరిగి steal;
	}
#అయితే_ముగింపు /* CONFIG_PARAVIRT */
	తిరిగి 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
స్థిర అంతఃవరుస u64 account_other_సమయం(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_సమయం(max);

	ఒకవేళ (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	తిరిగి accounted;
}

#నిర్వచితమైతే CONFIG_64_బిట్టు
స్థిర అంతఃవరుస u64 read_కూడిక_exec_runtime(నిర్మాణం task_struct *t)
{
	తిరిగి t->se.కూడిక_exec_runtime;
}
#లేకపోతే /* !CONFIG_64BIT: */
స్థిర u64 read_కూడిక_exec_runtime(నిర్మాణం task_struct *t)
{
	u64 ns;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	rq = task_rq_తాళం_2(t, &rf);
	ns = t->se.కూడిక_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	తిరిగి ns;
}
#అయితే_ముగింపు /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
శూన్య thread_సమూహం_cputime(నిర్మాణం task_struct *tsk, నిర్మాణం task_cputime *times)
{
	నిర్మాణం signal_struct *sig = tsk->సంకేతం;
	నిర్మాణం task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	ఒకవేళ (same_thread_సమూహం(current, tsk))
		(శూన్య) task_sched_runtime(current);

	guard(rcu)();
	scoped_seqlock_read (&sig->stats_తాళం, ss_తాళం_irqsave) {
		times->utime = sig->utime;
		times->stime = sig->stime;
		times->కూడిక_exec_runtime = sig->కూడిక_sched_runtime;

		__for_each_thread(sig, t) {
			task_cputime(t, &utime, &stime);
			times->utime += utime;
			times->stime += stime;
			times->కూడిక_exec_runtime += read_కూడిక_exec_runtime(t);
		}
	}
}

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
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
స్థిర శూన్య irqtime_account_process_tick(నిర్మాణం task_struct *p, పూర్ణాంకం user_tick,
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
	other = account_other_సమయం(ULONG_MAX);
	ఒకవేళ (other >= cputime)
		తిరిగి;

	cputime -= other;

	ఒకవేళ (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_సూచీ_సమయం(p, cputime, CPUTIME_SOFTIRQ);
	} లేకపోతే ఒకవేళ (user_tick) {
		account_user_సమయం(p, cputime);
	} లేకపోతే ఒకవేళ (p == this_rq()->idle) {
		account_idle_సమయం(cputime);
	} లేకపోతే ఒకవేళ (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_సమయం(p, cputime);
	} లేకపోతే {
		account_system_సూచీ_సమయం(p, cputime, CPUTIME_SYSTEM);
	}
}

#లేకపోతే /* !CONFIG_IRQ_TIME_ACCOUNTING: */
స్థిర అంతఃవరుస శూన్య irqtime_account_process_tick(నిర్మాణం task_struct *p, పూర్ణాంకం user_tick,
						u64 cputime) { }
#అయితే_ముగింపు /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ఒకవేళ defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
స్థిర శూన్య kcpustat_idle_stop(నిర్మాణం kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	పూర్ణాంకం iowait;

	ఒకవేళ (!kc->idle_elapse)
		తిరిగి;

	iowait = cpu_has_iowait(smp_processor_id());
	delta = now - kc->idle_entrytime;
	steal = steal_account_process_సమయం(delta);

	/*
	 * Record the idle time after substracting the steal time from
	 * previous update sequence. Don't substract the steal time from
	 * the current update sequence to avoid readers moving backward.
	 */
	write_seqcount_begin(&kc->idle_sleeptime_seq);
	steal_delta = min_t(u64, kc->idle_stealtime[iowait], delta);
	delta -= steal_delta;
	kc->idle_stealtime[iowait] -= steal_delta;

	ఒకవేళ (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	లేకపోతే
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_చివర(&kc->idle_sleeptime_seq);
}

స్థిర శూన్య kcpustat_idle_ప్రారంభం(నిర్మాణం kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	ఒకవేళ (kc->idle_elapse)
		తిరిగి;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_చివర(&kc->idle_sleeptime_seq);
}

శూన్య kcpustat_dyntick_stop(u64 now, u64 tick_ప్రారంభం)
{
	నిర్మాణం kernel_cpustat *kc = kcpustat_this_cpu;

	ఒకవేళ (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		ఒకవేళ (kc->idle_tick_period != tick_ప్రారంభం) {
			kc->idle_tick_period = tick_ప్రారంభం;
			kc->idle_tick_overlap = 0;
		}
		tick_ప్రారంభం = max(tick_ప్రారంభం, kc->idle_dyntick_నమోదు);
		ఒకవేళ (!WARN_ON_ONCE(now < tick_ప్రారంభం))
			kc->idle_tick_overlap += now - tick_ప్రారంభం;
		kcpustat_idle_stop(kc, now);
		kc->idle_dyntick = false;
		vtime_dyntick_stop();
	}
}

/*
 * The first tick after dyntick-idle covers a period that the dyntick-idle
 * accounting may already have accounted in part.
 */
స్థిర అంతఃవరుస u64 kcpustat_tick_overlap(శూన్య)
{
	నిర్మాణం kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	తిరిగి overlap;
}

శూన్య kcpustat_dyntick_ప్రారంభం(u64 now)
{
	నిర్మాణం kernel_cpustat *kc = kcpustat_this_cpu;

	ఒకవేళ (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_ప్రారంభం();
		kc->idle_dyntick = true;
		kc->idle_dyntick_నమోదు = now;
		kcpustat_idle_ప్రారంభం(kc, now);
	}
}

శూన్య kcpustat_irq_enter(u64 now)
{
	నిర్మాణం kernel_cpustat *kc = kcpustat_this_cpu;

	ఒకవేళ (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

శూన్య kcpustat_irq_exit(u64 now)
{
	నిర్మాణం kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	ఒకవేళ (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_సమయం_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_ప్రారంభం(kc, now);
}

స్థిర u64 kcpustat_క్షేత్రం_dyntick(పూర్ణాంకం cpu, గణన cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	నిర్మాణం kernel_cpustat *kc = &kcpustat_cpu(cpu);
	పూర్ణాంకం iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	గుర్తులేని పూర్ణాంకం seq;
	u64 idle;

	చేయి {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		ఒకవేళ (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} అయితే (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	తిరిగి idle;
}

u64 kcpustat_క్షేత్రం_idle(పూర్ణాంకం cpu)
{
	తిరిగి kcpustat_క్షేత్రం_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_చిహ్నం_GPL(kcpustat_క్షేత్రం_idle);

u64 kcpustat_క్షేత్రం_iowait(పూర్ణాంకం cpu)
{
	తిరిగి kcpustat_క్షేత్రం_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_చిహ్నం_GPL(kcpustat_క్షేత్రం_iowait);
#లేకపోతే
స్థిర అంతఃవరుస u64 kcpustat_tick_overlap(శూన్య)
{
	తిరిగి 0;
}

స్థిర u64 kcpustat_క్షేత్రం_dyntick(పూర్ణాంకం cpu, గణన cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	తిరిగి kcpustat_cpu(cpu).cpustat[idx];
}
#అయితే_ముగింపు /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

స్థిర u64 get_cpu_sleep_సమయం_us(పూర్ణాంకం cpu, గణన cpu_usage_stat idx,
				 bool compute_delta, u64 *చివరి_update_సమయం)
{
	ktime_t now = ktime_get();
	u64 res;

	ఒకవేళ (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_క్షేత్రం(idx, cpu);
	లేకపోతే
		res = kcpustat_క్షేత్రం_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_చొప్పున_USEC);

	ఒకవేళ (చివరి_update_సమయం)
		*చివరి_update_సమయం = ktime_గమ్యం_us(now);

	తిరిగి res;
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
u64 get_cpu_idle_సమయం_us(పూర్ణాంకం cpu, u64 *చివరి_update_సమయం)
{
	తిరిగి get_cpu_sleep_సమయం_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), చివరి_update_సమయం);
}
EXPORT_చిహ్నం_GPL(get_cpu_idle_సమయం_us);

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
u64 get_cpu_iowait_సమయం_us(పూర్ణాంకం cpu, u64 *చివరి_update_సమయం)
{
	తిరిగి get_cpu_sleep_సమయం_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), చివరి_update_సమయం);
}
EXPORT_చిహ్నం_GPL(get_cpu_iowait_సమయం_us);

/*
 * Use precise platform statistics if available:
 */
#నిర్వచితమైతే CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

శూన్య vtime_account_irq(నిర్మాణం task_struct *tsk, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
{
	గుర్తులేని పూర్ణాంకం pc = irq_లెక్క() - స్థానభ్రంశం;

	ఒకవేళ (pc & HARDIRQ_స్థానభ్రంశం) {
		vtime_account_hardirq(tsk);
	} లేకపోతే ఒకవేళ (pc & SOFTIRQ_స్థానభ్రంశం) {
		vtime_account_softirq(tsk);
	} లేకపోతే ఒకవేళ (!kcpustat_idle_dyntick()) {
		ఒకవేళ (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} లేకపోతే {
			vtime_account_kernel(tsk);
		}
	} లేకపోతే {
		vtime_reset();
	}
}

శూన్య cputime_adjust(నిర్మాణం task_cputime *curr, నిర్మాణం prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

శూన్య task_cputime_adjusted(నిర్మాణం task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_చిహ్నం_GPL(task_cputime_adjusted);

శూన్య thread_సమూహం_cputime_adjusted(నిర్మాణం task_struct *p, u64 *ut, u64 *st)
{
	నిర్మాణం task_cputime cputime;

	thread_సమూహం_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#లేకపోతే /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
శూన్య account_process_tick(నిర్మాణం task_struct *p, పూర్ణాంకం user_tick)
{
	u64 cputime, steal;

	ఒకవేళ (vtime_accounting_enabled_this_cpu())
		తిరిగి;

	ఒకవేళ (kcpustat_idle_dyntick())
		తిరిగి;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	ఒకవేళ (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		తిరిగి;
	}

	steal = steal_account_process_సమయం(ULONG_MAX);

	ఒకవేళ (steal >= cputime)
		తిరిగి;

	cputime -= steal;

	ఒకవేళ (user_tick)
		account_user_సమయం(p, cputime);
	లేకపోతే ఒకవేళ ((p != this_rq()->idle) || (irq_లెక్క() != HARDIRQ_స్థానభ్రంశం))
		account_system_సమయం(p, HARDIRQ_స్థానభ్రంశం, cputime);
	లేకపోతే
		account_idle_సమయం(cputime);
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
శూన్య cputime_adjust(నిర్మాణం task_cputime *curr, నిర్మాణం prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	u64 rtime, stime, utime;
	గుర్తులేని దీర్ఘ flags;

	/* Serialize concurrent callers such that we can honour our guarantees */
	raw_spin_తాళం_irqsave(&prev->తాళం, flags);
	rtime = curr->కూడిక_exec_runtime;

	/*
	 * This is possible under two circumstances:
	 *  - rtime isn't monotonic after all (a bug);
	 *  - we got reordered by the lock.
	 *
	 * In both cases this acts as a filter such that the rest of the code
	 * can assume it is monotonic regardless of anything else.
	 */
	ఒకవేళ (prev->stime + prev->utime >= rtime)
		వెళ్ళు బహిర్గతం;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	ఒకవేళ (stime == 0) {
		utime = rtime;
		వెళ్ళు update;
	}

	ఒకవేళ (utime == 0) {
		stime = rtime;
		వెళ్ళు update;
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
	ఒకవేళ (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	ఒకవేళ (utime < prev->utime) {
		utime = prev->utime;
		stime = rtime - utime;
	}

	prev->stime = stime;
	prev->utime = utime;
బహిర్గతం:
	*ut = prev->utime;
	*st = prev->stime;
	raw_spin_unlock_irqrestore(&prev->తాళం, flags);
}

శూన్య task_cputime_adjusted(నిర్మాణం task_struct *p, u64 *ut, u64 *st)
{
	నిర్మాణం task_cputime cputime = {
		.కూడిక_exec_runtime = p->se.కూడిక_exec_runtime,
	};

	ఒకవేళ (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.కూడిక_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_చిహ్నం_GPL(task_cputime_adjusted);

శూన్య thread_సమూహం_cputime_adjusted(నిర్మాణం task_struct *p, u64 *ut, u64 *st)
{
	నిర్మాణం task_cputime cputime;

	thread_సమూహం_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->సంకేతం->prev_cputime, ut, st);
}
#అయితే_ముగింపు /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#నిర్వచితమైతే CONFIG_VIRT_CPU_ACCOUNTING_GEN
స్థిర u64 vtime_delta(నిర్మాణం vtime *vtime)
{
	గుర్తులేని దీర్ఘ దీర్ఘ clock;

	clock = sched_clock();
	ఒకవేళ (clock < vtime->starttime)
		తిరిగి 0;

	తిరిగి clock - vtime->starttime;
}

స్థిర u64 get_vtime_delta(నిర్మాణం vtime *vtime)
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
	other = account_other_సమయం(delta);
	WARN_ON_ONCE(vtime->state == VTIME_INACTIVE);
	vtime->starttime += delta;

	తిరిగి delta - other;
}

స్థిర శూన్య vtime_account_system(నిర్మాణం task_struct *tsk,
				 నిర్మాణం vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	ఒకవేళ (vtime->stime >= TICK_NSEC) {
		account_system_సమయం(tsk, irq_లెక్క(), vtime->stime);
		vtime->stime = 0;
	}
}

స్థిర శూన్య vtime_account_guest(నిర్మాణం task_struct *tsk,
				నిర్మాణం vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	ఒకవేళ (vtime->gtime >= TICK_NSEC) {
		account_guest_సమయం(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

స్థిర శూన్య __vtime_account_kernel(నిర్మాణం task_struct *tsk,
				   నిర్మాణం vtime *vtime)
{
	/* We might have scheduled out from guest path */
	ఒకవేళ (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	లేకపోతే
		vtime_account_system(tsk, vtime);
}

శూన్య vtime_account_kernel(నిర్మాణం task_struct *tsk)
{
	నిర్మాణం vtime *vtime = &tsk->vtime;

	ఒకవేళ (!vtime_delta(vtime))
		తిరిగి;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_చివర(&vtime->seqcount);
}

శూన్య vtime_user_enter(నిర్మాణం task_struct *tsk)
{
	నిర్మాణం vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_చివర(&vtime->seqcount);
}

శూన్య vtime_user_exit(నిర్మాణం task_struct *tsk)
{
	నిర్మాణం vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	ఒకవేళ (vtime->utime >= TICK_NSEC) {
		account_user_సమయం(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_చివర(&vtime->seqcount);
}

శూన్య vtime_guest_enter(నిర్మాణం task_struct *tsk)
{
	నిర్మాణం vtime *vtime = &tsk->vtime;
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
	write_seqcount_చివర(&vtime->seqcount);
}
EXPORT_చిహ్నం_GPL(vtime_guest_enter);

శూన్య vtime_guest_exit(నిర్మాణం task_struct *tsk)
{
	నిర్మాణం vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_చివర(&vtime->seqcount);
}
EXPORT_చిహ్నం_GPL(vtime_guest_exit);

స్థిర శూన్య __vtime_account_idle(నిర్మాణం vtime *vtime)
{
	account_idle_సమయం(get_vtime_delta(vtime));
}

శూన్య vtime_task_switch_generic(నిర్మాణం task_struct *prev)
{
	నిర్మాణం vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ఒకవేళ (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	లేకపోతే
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_చివర(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ఒకవేళ (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	లేకపోతే ఒకవేళ (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	లేకపోతే
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_చివర(&vtime->seqcount);
}

శూన్య vtime_init_idle(నిర్మాణం task_struct *t, పూర్ణాంకం cpu)
{
	నిర్మాణం vtime *vtime = &t->vtime;
	గుర్తులేని దీర్ఘ flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_చివర(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(నిర్మాణం task_struct *t)
{
	నిర్మాణం vtime *vtime = &t->vtime;
	గుర్తులేని పూర్ణాంకం seq;
	u64 gtime;

	ఒకవేళ (!vtime_accounting_enabled())
		తిరిగి t->gtime;

	చేయి {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		ఒకవేళ (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} అయితే (read_seqcount_retry(&vtime->seqcount, seq));

	తిరిగి gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(నిర్మాణం task_struct *t, u64 *utime, u64 *stime)
{
	నిర్మాణం vtime *vtime = &t->vtime;
	గుర్తులేని పూర్ణాంకం seq;
	u64 delta;
	పూర్ణాంకం ret;

	ఒకవేళ (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		తిరిగి false;
	}

	చేయి {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		ఒకవేళ (vtime->state < VTIME_SYS)
			కొనసాగు;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ఒకవేళ (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		లేకపోతే
			*utime += vtime->utime + delta;
	} అయితే (read_seqcount_retry(&vtime->seqcount, seq));

	తిరిగి ret;
}

స్థిర పూర్ణాంకం vtime_state_fetch(నిర్మాణం vtime *vtime, పూర్ణాంకం cpu)
{
	పూర్ణాంకం state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	ఒకవేళ (vtime->cpu != cpu && vtime->cpu != -1)
		తిరిగి -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	ఒకవేళ (state == VTIME_INACTIVE)
		తిరిగి -EAGAIN;

	తిరిగి state;
}

స్థిర u64 kcpustat_user_vtime(నిర్మాణం vtime *vtime)
{
	ఒకవేళ (vtime->state == VTIME_USER)
		తిరిగి vtime->utime + vtime_delta(vtime);
	లేకపోతే ఒకవేళ (vtime->state == VTIME_GUEST)
		తిరిగి vtime->gtime + vtime_delta(vtime);
	తిరిగి 0;
}

స్థిర పూర్ణాంకం kcpustat_క్షేత్రం_vtime(u64 *cpustat,
				నిర్మాణం task_struct *tsk,
				గణన cpu_usage_stat usage,
				పూర్ణాంకం cpu, u64 *val)
{
	నిర్మాణం vtime *vtime = &tsk->vtime;
	గుర్తులేని పూర్ణాంకం seq;

	చేయి {
		పూర్ణాంకం state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ఒకవేళ (state < 0)
			తిరిగి state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		ఎంపిక (usage) {
		స్థితి CPUTIME_SYSTEM:
			ఒకవేళ (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			ఆపు;
		స్థితి CPUTIME_USER:
			ఒకవేళ (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			ఆపు;
		స్థితి CPUTIME_NICE:
			ఒకవేళ (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			ఆపు;
		స్థితి CPUTIME_GUEST:
			ఒకవేళ (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ఆపు;
		స్థితి CPUTIME_GUEST_NICE:
			ఒకవేళ (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			ఆపు;
		స్థితి CPUTIME_IDLE:
			ఒకవేళ (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ఆపు;
		స్థితి CPUTIME_IOWAIT:
			ఒకవేళ (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			ఆపు;
		అప్రమేయ:
			ఆపు;
		}
	} అయితే (read_seqcount_retry(&vtime->seqcount, seq));

	తిరిగి 0;
}

u64 kcpustat_క్షేత్రం(గణన cpu_usage_stat usage, పూర్ణాంకం cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	నిర్మాణం rq *rq;
	పూర్ణాంకం err;

	ఒకవేళ (!vtime_generic_enabled_cpu(cpu))
		తిరిగి kcpustat_క్షేత్రం_default(usage, cpu);

	rq = cpu_rq(cpu);

	కొరకు (;;) {
		నిర్మాణం task_struct *curr;

		rcu_read_తాళం();
		curr = rcu_dereference(rq->curr);
		ఒకవేళ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			తిరిగి cpustat[usage];
		}

		err = kcpustat_క్షేత్రం_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		ఒకవేళ (!err)
			తిరిగి val;

		cpu_relax();
	}
}
EXPORT_చిహ్నం_GPL(kcpustat_క్షేత్రం);

స్థిర పూర్ణాంకం kcpustat_cpu_fetch_vtime(నిర్మాణం kernel_cpustat *dst,
				    స్థిరరాశి నిర్మాణం kernel_cpustat *src,
				    నిర్మాణం task_struct *tsk, పూర్ణాంకం cpu)
{
	నిర్మాణం vtime *vtime = &tsk->vtime;
	గుర్తులేని పూర్ణాంకం seq;

	చేయి {
		u64 *cpustat;
		u64 delta;
		పూర్ణాంకం state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ఒకవేళ (state < 0)
			తిరిగి state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		ఒకవేళ (state < VTIME_IDLE)
			కొనసాగు;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ఎంపిక (state) {
		స్థితి VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			ఆపు;
		స్థితి VTIME_USER:
			ఒకవేళ (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			లేకపోతే
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			ఆపు;
		స్థితి VTIME_GUEST:
			ఒకవేళ (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} లేకపోతే {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			ఆపు;
		స్థితి VTIME_IDLE:
			ఒకవేళ (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			లేకపోతే
				cpustat[CPUTIME_IDLE] += delta;
			ఆపు;
		అప్రమేయ:
			WARN_ON_ONCE(1);
		}
	} అయితే (read_seqcount_retry(&vtime->seqcount, seq));

	తిరిగి 0;
}

శూన్య kcpustat_cpu_fetch(నిర్మాణం kernel_cpustat *dst, పూర్ణాంకం cpu)
{
	స్థిరరాశి నిర్మాణం kernel_cpustat *src = &kcpustat_cpu(cpu);
	నిర్మాణం rq *rq;
	పూర్ణాంకం err;

	ఒకవేళ (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		తిరిగి;
	}

	rq = cpu_rq(cpu);

	కొరకు (;;) {
		నిర్మాణం task_struct *curr;

		rcu_read_తాళం();
		curr = rcu_dereference(rq->curr);
		ఒకవేళ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			తిరిగి;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		ఒకవేళ (!err)
			తిరిగి;

		cpu_relax();
	}
}
EXPORT_చిహ్నం_GPL(kcpustat_cpu_fetch);

#అయితే_ముగింపు /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
