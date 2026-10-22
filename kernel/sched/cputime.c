// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#અન્તર્ભાવ <linux/sched/clock.h>
#અન્તર્ભાવ <linux/sched/cputime.h>
#અન્તર્ભાવ <linux/tsacct_kern.h>
#અન્તર્ભાવ "sched.h"

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #અન્તર્ભાવ <asm/cputime.h>
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

DEFINE_STATIC_ચાવી_FALSE(sched_clock_irqtime);

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
DEFINE_PER_CPU(સંરચના irqtime, cpu_irqtime);

શૂન્ય enable_sched_clock_irqtime(શૂન્ય)
{
	static_branch_enable(&sched_clock_irqtime);
}

શૂન્ય disable_sched_clock_irqtime(શૂન્ય)
{
	જો (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

સ્થિર શૂન્ય irqtime_account_delta(સંરચના irqtime *irqtime, u64 delta,
				  ગણના cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->total += delta;
	જો (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_end(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
શૂન્ય irqtime_account_irq(સંરચના task_struct *curr, unsigned પૂર્ણાંક offset)
{
	સંરચના irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	unsigned પૂર્ણાંક pc;
	s64 delta;
	પૂર્ણાંક cpu;

	જો (!irqtime_enabled())
		પાછા;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_start_time;
	irqtime->irq_start_time += delta;
	pc = irq_ગણતરી() - offset;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	જો (pc & HARDIRQ_MASK)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	નહીંતર જો ((pc & SOFTIRQ_OFFSET) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

સ્થિર u64 irqtime_tick_accounted(u64 maxtime)
{
	સંરચના irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	પાછા delta;
}

#નહીંતર /* !CONFIG_IRQ_TIME_ACCOUNTING: */

સ્થિર u64 irqtime_tick_accounted(u64 dummy)
{
	પાછા 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

સ્થિર અંતઃરેખ શૂન્ય task_group_account_field(સંરચના task_struct *p, પૂર્ણાંક index,
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
શૂન્ય account_user_time(સંરચના task_struct *p, u64 cputime)
{
	પૂર્ણાંક index;

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
શૂન્ય account_guest_time(સંરચના task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	જો (task_nice(p) > 0) {
		task_group_account_field(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} નહીંતર {
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
શૂન્ય account_system_index_time(સંરચના task_struct *p,
			       u64 cputime, ગણના cpu_usage_stat index)
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
શૂન્ય account_system_time(સંરચના task_struct *p, પૂર્ણાંક hardirq_offset, u64 cputime)
{
	પૂર્ણાંક index;

	જો ((p->flags & PF_VCPU) && (irq_ગણતરી() - hardirq_offset == 0)) {
		account_guest_time(p, cputime);
		પાછા;
	}

	જો (hardirq_ગણતરી() - hardirq_offset)
		index = CPUTIME_IRQ;
	નહીંતર જો (in_serving_softirq())
		index = CPUTIME_SOFTIRQ;
	નહીંતર
		index = CPUTIME_SYSTEM;

	account_system_index_time(p, cputime, index);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
શૂન્ય account_steal_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
શૂન્ય account_idle_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	જો (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	નહીંતર
		cpustat[CPUTIME_IDLE] += cputime;
}


#ifdef CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
શૂન્ય __account_forceidle_time(સંરચના task_struct *p, u64 delta)
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
DEFINE_STATIC_ચાવી_FALSE(paravirt_steal_enabled);

#ifdef CONFIG_HAVE_PV_STEAL_CLOCK_GEN
સ્થિર u64 native_steal_clock(પૂર્ણાંક cpu)
{
	પાછા 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#endif
#endif

સ્થિર __always_inline u64 steal_account_process_time(u64 maxtime)
{
#ifdef CONFIG_PARAVIRT
	જો (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_time;
		steal = min(steal, maxtime);
		account_steal_time(steal);
		this_rq()->prev_steal_time += steal;

		પાછા steal;
	}
#endif /* CONFIG_PARAVIRT */
	પાછા 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
સ્થિર અંતઃરેખ u64 account_other_time(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_time(max);

	જો (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	પાછા accounted;
}

#ifdef CONFIG_64BIT
સ્થિર અંતઃરેખ u64 read_sum_exec_runtime(સંરચના task_struct *t)
{
	પાછા t->se.sum_exec_runtime;
}
#નહીંતર /* !CONFIG_64BIT: */
સ્થિર u64 read_sum_exec_runtime(સંરચના task_struct *t)
{
	u64 ns;
	સંરચના rq_flags rf;
	સંરચના rq *rq;

	rq = task_rq_lock(t, &rf);
	ns = t->se.sum_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	પાછા ns;
}
#endif /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
શૂન્ય thread_group_cputime(સંરચના task_struct *tsk, સંરચના task_cputime *times)
{
	સંરચના signal_struct *sig = tsk->સંકેત;
	સંરચના task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	જો (same_thread_group(current, tsk))
		(શૂન્ય) task_sched_runtime(current);

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
સ્થિર શૂન્ય irqtime_account_process_tick(સંરચના task_struct *p, પૂર્ણાંક user_tick,
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
	જો (other >= cputime)
		પાછા;

	cputime -= other;

	જો (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_index_time(p, cputime, CPUTIME_SOFTIRQ);
	} નહીંતર જો (user_tick) {
		account_user_time(p, cputime);
	} નહીંતર જો (p == this_rq()->idle) {
		account_idle_time(cputime);
	} નહીંતર જો (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_time(p, cputime);
	} નહીંતર {
		account_system_index_time(p, cputime, CPUTIME_SYSTEM);
	}
}

#નહીંતર /* !CONFIG_IRQ_TIME_ACCOUNTING: */
સ્થિર અંતઃરેખ શૂન્ય irqtime_account_process_tick(સંરચના task_struct *p, પૂર્ણાંક user_tick,
						u64 cputime) { }
#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#જો defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
સ્થિર શૂન્ય kcpustat_idle_stop(સંરચના kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	પૂર્ણાંક iowait;

	જો (!kc->idle_elapse)
		પાછા;

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

	જો (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	નહીંતર
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

સ્થિર શૂન્ય kcpustat_idle_start(સંરચના kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	જો (kc->idle_elapse)
		પાછા;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

શૂન્ય kcpustat_dyntick_stop(u64 now, u64 tick_start)
{
	સંરચના kernel_cpustat *kc = kcpustat_this_cpu;

	જો (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		જો (kc->idle_tick_period != tick_start) {
			kc->idle_tick_period = tick_start;
			kc->idle_tick_overlap = 0;
		}
		tick_start = max(tick_start, kc->idle_dyntick_entry);
		જો (!WARN_ON_ONCE(now < tick_start))
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
સ્થિર અંતઃરેખ u64 kcpustat_tick_overlap(શૂન્ય)
{
	સંરચના kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	પાછા overlap;
}

શૂન્ય kcpustat_dyntick_start(u64 now)
{
	સંરચના kernel_cpustat *kc = kcpustat_this_cpu;

	જો (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_start();
		kc->idle_dyntick = true;
		kc->idle_dyntick_entry = now;
		kcpustat_idle_start(kc, now);
	}
}

શૂન્ય kcpustat_irq_enter(u64 now)
{
	સંરચના kernel_cpustat *kc = kcpustat_this_cpu;

	જો (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

શૂન્ય kcpustat_irq_exit(u64 now)
{
	સંરચના kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	જો (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_start(kc, now);
}

સ્થિર u64 kcpustat_field_dyntick(પૂર્ણાંક cpu, ગણના cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	સંરચના kernel_cpustat *kc = &kcpustat_cpu(cpu);
	પૂર્ણાંક iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	unsigned પૂર્ણાંક seq;
	u64 idle;

	કરો {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		જો (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} જ્યારે (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	પાછા idle;
}

u64 kcpustat_field_idle(પૂર્ણાંક cpu)
{
	પાછા kcpustat_field_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_પ્રતીક_GPL(kcpustat_field_idle);

u64 kcpustat_field_iowait(પૂર્ણાંક cpu)
{
	પાછા kcpustat_field_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_પ્રતીક_GPL(kcpustat_field_iowait);
#નહીંતર
સ્થિર અંતઃરેખ u64 kcpustat_tick_overlap(શૂન્ય)
{
	પાછા 0;
}

સ્થિર u64 kcpustat_field_dyntick(પૂર્ણાંક cpu, ગણના cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	પાછા kcpustat_cpu(cpu).cpustat[idx];
}
#endif /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

સ્થિર u64 get_cpu_sleep_time_us(પૂર્ણાંક cpu, ગણના cpu_usage_stat idx,
				 bool compute_delta, u64 *last_update_time)
{
	ktime_t now = ktime_get();
	u64 res;

	જો (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_field(idx, cpu);
	નહીંતર
		res = kcpustat_field_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_PER_USEC);

	જો (last_update_time)
		*last_update_time = ktime_to_us(now);

	પાછા res;
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
u64 get_cpu_idle_time_us(પૂર્ણાંક cpu, u64 *last_update_time)
{
	પાછા get_cpu_sleep_time_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), last_update_time);
}
EXPORT_પ્રતીક_GPL(get_cpu_idle_time_us);

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
u64 get_cpu_iowait_time_us(પૂર્ણાંક cpu, u64 *last_update_time)
{
	પાછા get_cpu_sleep_time_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), last_update_time);
}
EXPORT_પ્રતીક_GPL(get_cpu_iowait_time_us);

/*
 * Use precise platform statistics if available:
 */
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

શૂન્ય vtime_account_irq(સંરચના task_struct *tsk, unsigned પૂર્ણાંક offset)
{
	unsigned પૂર્ણાંક pc = irq_ગણતરી() - offset;

	જો (pc & HARDIRQ_OFFSET) {
		vtime_account_hardirq(tsk);
	} નહીંતર જો (pc & SOFTIRQ_OFFSET) {
		vtime_account_softirq(tsk);
	} નહીંતર જો (!kcpustat_idle_dyntick()) {
		જો (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} નહીંતર {
			vtime_account_kernel(tsk);
		}
	} નહીંતર {
		vtime_reset();
	}
}

શૂન્ય cputime_adjust(સંરચના task_cputime *curr, સંરચના prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

શૂન્ય task_cputime_adjusted(સંરચના task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_પ્રતીક_GPL(task_cputime_adjusted);

શૂન્ય thread_group_cputime_adjusted(સંરચના task_struct *p, u64 *ut, u64 *st)
{
	સંરચના task_cputime cputime;

	thread_group_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#નહીંતર /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
શૂન્ય account_process_tick(સંરચના task_struct *p, પૂર્ણાંક user_tick)
{
	u64 cputime, steal;

	જો (vtime_accounting_enabled_this_cpu())
		પાછા;

	જો (kcpustat_idle_dyntick())
		પાછા;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	જો (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		પાછા;
	}

	steal = steal_account_process_time(ULONG_MAX);

	જો (steal >= cputime)
		પાછા;

	cputime -= steal;

	જો (user_tick)
		account_user_time(p, cputime);
	નહીંતર જો ((p != this_rq()->idle) || (irq_ગણતરી() != HARDIRQ_OFFSET))
		account_system_time(p, HARDIRQ_OFFSET, cputime);
	નહીંતર
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
શૂન્ય cputime_adjust(સંરચના task_cputime *curr, સંરચના prev_cputime *prev,
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
	જો (prev->stime + prev->utime >= rtime)
		જાઓ out;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	જો (stime == 0) {
		utime = rtime;
		જાઓ update;
	}

	જો (utime == 0) {
		stime = rtime;
		જાઓ update;
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
	જો (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	જો (utime < prev->utime) {
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

શૂન્ય task_cputime_adjusted(સંરચના task_struct *p, u64 *ut, u64 *st)
{
	સંરચના task_cputime cputime = {
		.sum_exec_runtime = p->se.sum_exec_runtime,
	};

	જો (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.sum_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_પ્રતીક_GPL(task_cputime_adjusted);

શૂન્ય thread_group_cputime_adjusted(સંરચના task_struct *p, u64 *ut, u64 *st)
{
	સંરચના task_cputime cputime;

	thread_group_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->સંકેત->prev_cputime, ut, st);
}
#endif /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
સ્થિર u64 vtime_delta(સંરચના vtime *vtime)
{
	unsigned long long clock;

	clock = sched_clock();
	જો (clock < vtime->starttime)
		પાછા 0;

	પાછા clock - vtime->starttime;
}

સ્થિર u64 get_vtime_delta(સંરચના vtime *vtime)
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

	પાછા delta - other;
}

સ્થિર શૂન્ય vtime_account_system(સંરચના task_struct *tsk,
				 સંરચના vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	જો (vtime->stime >= TICK_NSEC) {
		account_system_time(tsk, irq_ગણતરી(), vtime->stime);
		vtime->stime = 0;
	}
}

સ્થિર શૂન્ય vtime_account_guest(સંરચના task_struct *tsk,
				સંરચના vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	જો (vtime->gtime >= TICK_NSEC) {
		account_guest_time(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

સ્થિર શૂન્ય __vtime_account_kernel(સંરચના task_struct *tsk,
				   સંરચના vtime *vtime)
{
	/* We might have scheduled out from guest path */
	જો (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	નહીંતર
		vtime_account_system(tsk, vtime);
}

શૂન્ય vtime_account_kernel(સંરચના task_struct *tsk)
{
	સંરચના vtime *vtime = &tsk->vtime;

	જો (!vtime_delta(vtime))
		પાછા;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_end(&vtime->seqcount);
}

શૂન્ય vtime_user_enter(સંરચના task_struct *tsk)
{
	સંરચના vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_end(&vtime->seqcount);
}

શૂન્ય vtime_user_exit(સંરચના task_struct *tsk)
{
	સંરચના vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	જો (vtime->utime >= TICK_NSEC) {
		account_user_time(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}

શૂન્ય vtime_guest_enter(સંરચના task_struct *tsk)
{
	સંરચના vtime *vtime = &tsk->vtime;
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
EXPORT_પ્રતીક_GPL(vtime_guest_enter);

શૂન્ય vtime_guest_exit(સંરચના task_struct *tsk)
{
	સંરચના vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_પ્રતીક_GPL(vtime_guest_exit);

સ્થિર શૂન્ય __vtime_account_idle(સંરચના vtime *vtime)
{
	account_idle_time(get_vtime_delta(vtime));
}

શૂન્ય vtime_task_switch_generic(સંરચના task_struct *prev)
{
	સંરચના vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	જો (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	નહીંતર
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_end(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	જો (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	નહીંતર જો (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	નહીંતર
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_end(&vtime->seqcount);
}

શૂન્ય vtime_init_idle(સંરચના task_struct *t, પૂર્ણાંક cpu)
{
	સંરચના vtime *vtime = &t->vtime;
	unsigned long flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_end(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(સંરચના task_struct *t)
{
	સંરચના vtime *vtime = &t->vtime;
	unsigned પૂર્ણાંક seq;
	u64 gtime;

	જો (!vtime_accounting_enabled())
		પાછા t->gtime;

	કરો {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		જો (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} જ્યારે (read_seqcount_retry(&vtime->seqcount, seq));

	પાછા gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(સંરચના task_struct *t, u64 *utime, u64 *stime)
{
	સંરચના vtime *vtime = &t->vtime;
	unsigned પૂર્ણાંક seq;
	u64 delta;
	પૂર્ણાંક ret;

	જો (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		પાછા false;
	}

	કરો {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		જો (vtime->state < VTIME_SYS)
			ચાલુ;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		જો (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		નહીંતર
			*utime += vtime->utime + delta;
	} જ્યારે (read_seqcount_retry(&vtime->seqcount, seq));

	પાછા ret;
}

સ્થિર પૂર્ણાંક vtime_state_fetch(સંરચના vtime *vtime, પૂર્ણાંક cpu)
{
	પૂર્ણાંક state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	જો (vtime->cpu != cpu && vtime->cpu != -1)
		પાછા -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	જો (state == VTIME_INACTIVE)
		પાછા -EAGAIN;

	પાછા state;
}

સ્થિર u64 kcpustat_user_vtime(સંરચના vtime *vtime)
{
	જો (vtime->state == VTIME_USER)
		પાછા vtime->utime + vtime_delta(vtime);
	નહીંતર જો (vtime->state == VTIME_GUEST)
		પાછા vtime->gtime + vtime_delta(vtime);
	પાછા 0;
}

સ્થિર પૂર્ણાંક kcpustat_field_vtime(u64 *cpustat,
				સંરચના task_struct *tsk,
				ગણના cpu_usage_stat usage,
				પૂર્ણાંક cpu, u64 *val)
{
	સંરચના vtime *vtime = &tsk->vtime;
	unsigned પૂર્ણાંક seq;

	કરો {
		પૂર્ણાંક state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		જો (state < 0)
			પાછા state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		પસંદગી (usage) {
		સ્થિતિ CPUTIME_SYSTEM:
			જો (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			તોડો;
		સ્થિતિ CPUTIME_USER:
			જો (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			તોડો;
		સ્થિતિ CPUTIME_NICE:
			જો (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			તોડો;
		સ્થિતિ CPUTIME_GUEST:
			જો (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			તોડો;
		સ્થિતિ CPUTIME_GUEST_NICE:
			જો (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			તોડો;
		સ્થિતિ CPUTIME_IDLE:
			જો (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			તોડો;
		સ્થિતિ CPUTIME_IOWAIT:
			જો (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			તોડો;
		મૂળભૂત:
			તોડો;
		}
	} જ્યારે (read_seqcount_retry(&vtime->seqcount, seq));

	પાછા 0;
}

u64 kcpustat_field(ગણના cpu_usage_stat usage, પૂર્ણાંક cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	સંરચના rq *rq;
	પૂર્ણાંક err;

	જો (!vtime_generic_enabled_cpu(cpu))
		પાછા kcpustat_field_default(usage, cpu);

	rq = cpu_rq(cpu);

	માટે (;;) {
		સંરચના task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		જો (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			પાછા cpustat[usage];
		}

		err = kcpustat_field_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		જો (!err)
			પાછા val;

		cpu_relax();
	}
}
EXPORT_પ્રતીક_GPL(kcpustat_field);

સ્થિર પૂર્ણાંક kcpustat_cpu_fetch_vtime(સંરચના kernel_cpustat *dst,
				    અચલ સંરચના kernel_cpustat *src,
				    સંરચના task_struct *tsk, પૂર્ણાંક cpu)
{
	સંરચના vtime *vtime = &tsk->vtime;
	unsigned પૂર્ણાંક seq;

	કરો {
		u64 *cpustat;
		u64 delta;
		પૂર્ણાંક state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		જો (state < 0)
			પાછા state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		જો (state < VTIME_IDLE)
			ચાલુ;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		પસંદગી (state) {
		સ્થિતિ VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			તોડો;
		સ્થિતિ VTIME_USER:
			જો (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			નહીંતર
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			તોડો;
		સ્થિતિ VTIME_GUEST:
			જો (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} નહીંતર {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			તોડો;
		સ્થિતિ VTIME_IDLE:
			જો (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			નહીંતર
				cpustat[CPUTIME_IDLE] += delta;
			તોડો;
		મૂળભૂત:
			WARN_ON_ONCE(1);
		}
	} જ્યારે (read_seqcount_retry(&vtime->seqcount, seq));

	પાછા 0;
}

શૂન્ય kcpustat_cpu_fetch(સંરચના kernel_cpustat *dst, પૂર્ણાંક cpu)
{
	અચલ સંરચના kernel_cpustat *src = &kcpustat_cpu(cpu);
	સંરચના rq *rq;
	પૂર્ણાંક err;

	જો (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		પાછા;
	}

	rq = cpu_rq(cpu);

	માટે (;;) {
		સંરચના task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		જો (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			પાછા;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		જો (!err)
			પાછા;

		cpu_relax();
	}
}
EXPORT_પ્રતીક_GPL(kcpustat_cpu_fetch);

#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
