// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#अन्तर्भाव <linux/sched/clock.h>
#अन्तर्भाव <linux/sched/cputime.h>
#अन्तर्भाव <linux/tsacct_kern.h>
#अन्तर्भाव "sched.h"

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #अन्तर्भाव <asm/cputime.h>
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

DEFINE_STATIC_साँचो_FALSE(sched_clock_irqtime);

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
DEFINE_PER_CPU(संरचना irqtime, cpu_irqtime);

शून्य enable_sched_clock_irqtime(शून्य)
{
	static_branch_enable(&sched_clock_irqtime);
}

शून्य disable_sched_clock_irqtime(शून्य)
{
	यदि (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

स्थिर शून्य irqtime_account_delta(संरचना irqtime *irqtime, u64 delta,
				  गणना cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->total += delta;
	यदि (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_end(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
शून्य irqtime_account_irq(संरचना task_struct *curr, अचिन्हित पूर्णाङ्क offset)
{
	संरचना irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	अचिन्हित पूर्णाङ्क pc;
	s64 delta;
	पूर्णाङ्क cpu;

	यदि (!irqtime_enabled())
		फर्काऊ;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_start_time;
	irqtime->irq_start_time += delta;
	pc = irq_गन्ती() - offset;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	यदि (pc & HARDIRQ_MASK)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	अन्यथा यदि ((pc & SOFTIRQ_OFFSET) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

स्थिर u64 irqtime_tick_accounted(u64 maxtime)
{
	संरचना irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	फर्काऊ delta;
}

#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */

स्थिर u64 irqtime_tick_accounted(u64 dummy)
{
	फर्काऊ 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

स्थिर अन्तःस्थ शून्य task_group_account_field(संरचना task_struct *p, पूर्णाङ्क index,
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
शून्य account_user_time(संरचना task_struct *p, u64 cputime)
{
	पूर्णाङ्क index;

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
शून्य account_guest_time(संरचना task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	यदि (task_nice(p) > 0) {
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
शून्य account_system_index_time(संरचना task_struct *p,
			       u64 cputime, गणना cpu_usage_stat index)
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
शून्य account_system_time(संरचना task_struct *p, पूर्णाङ्क hardirq_offset, u64 cputime)
{
	पूर्णाङ्क index;

	यदि ((p->flags & PF_VCPU) && (irq_गन्ती() - hardirq_offset == 0)) {
		account_guest_time(p, cputime);
		फर्काऊ;
	}

	यदि (hardirq_गन्ती() - hardirq_offset)
		index = CPUTIME_IRQ;
	अन्यथा यदि (in_serving_softirq())
		index = CPUTIME_SOFTIRQ;
	अन्यथा
		index = CPUTIME_SYSTEM;

	account_system_index_time(p, cputime, index);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
शून्य account_steal_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
शून्य account_idle_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	यदि (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	अन्यथा
		cpustat[CPUTIME_IDLE] += cputime;
}


#ifdef CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
शून्य __account_forceidle_time(संरचना task_struct *p, u64 delta)
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
DEFINE_STATIC_साँचो_FALSE(paravirt_steal_enabled);

#ifdef CONFIG_HAVE_PV_STEAL_CLOCK_GEN
स्थिर u64 native_steal_clock(पूर्णाङ्क cpu)
{
	फर्काऊ 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#endif
#endif

स्थिर __always_inline u64 steal_account_process_time(u64 maxtime)
{
#ifdef CONFIG_PARAVIRT
	यदि (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_time;
		steal = min(steal, maxtime);
		account_steal_time(steal);
		this_rq()->prev_steal_time += steal;

		फर्काऊ steal;
	}
#endif /* CONFIG_PARAVIRT */
	फर्काऊ 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
स्थिर अन्तःस्थ u64 account_other_time(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_time(max);

	यदि (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	फर्काऊ accounted;
}

#ifdef CONFIG_64BIT
स्थिर अन्तःस्थ u64 read_sum_exec_runtime(संरचना task_struct *t)
{
	फर्काऊ t->se.sum_exec_runtime;
}
#अन्यथा /* !CONFIG_64BIT: */
स्थिर u64 read_sum_exec_runtime(संरचना task_struct *t)
{
	u64 ns;
	संरचना rq_flags rf;
	संरचना rq *rq;

	rq = task_rq_lock(t, &rf);
	ns = t->se.sum_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	फर्काऊ ns;
}
#endif /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
शून्य thread_group_cputime(संरचना task_struct *tsk, संरचना task_cputime *बार)
{
	संरचना signal_struct *sig = tsk->सङ्केत;
	संरचना task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	यदि (same_thread_group(current, tsk))
		(शून्य) task_sched_runtime(current);

	guard(rcu)();
	scoped_seqlock_read (&sig->stats_lock, ss_lock_irqsave) {
		बार->utime = sig->utime;
		बार->stime = sig->stime;
		बार->sum_exec_runtime = sig->sum_sched_runtime;

		__for_each_thread(sig, t) {
			task_cputime(t, &utime, &stime);
			बार->utime += utime;
			बार->stime += stime;
			बार->sum_exec_runtime += read_sum_exec_runtime(t);
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
स्थिर शून्य irqtime_account_process_tick(संरचना task_struct *p, पूर्णाङ्क user_tick,
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
	यदि (other >= cputime)
		फर्काऊ;

	cputime -= other;

	यदि (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_index_time(p, cputime, CPUTIME_SOFTIRQ);
	} अन्यथा यदि (user_tick) {
		account_user_time(p, cputime);
	} अन्यथा यदि (p == this_rq()->idle) {
		account_idle_time(cputime);
	} अन्यथा यदि (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_time(p, cputime);
	} अन्यथा {
		account_system_index_time(p, cputime, CPUTIME_SYSTEM);
	}
}

#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */
स्थिर अन्तःस्थ शून्य irqtime_account_process_tick(संरचना task_struct *p, पूर्णाङ्क user_tick,
						u64 cputime) { }
#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#यदि defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
स्थिर शून्य kcpustat_idle_stop(संरचना kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	पूर्णाङ्क iowait;

	यदि (!kc->idle_elapse)
		फर्काऊ;

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

	यदि (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	अन्यथा
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

स्थिर शून्य kcpustat_idle_start(संरचना kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	यदि (kc->idle_elapse)
		फर्काऊ;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

शून्य kcpustat_dyntick_stop(u64 now, u64 tick_start)
{
	संरचना kernel_cpustat *kc = kcpustat_this_cpu;

	यदि (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		यदि (kc->idle_tick_period != tick_start) {
			kc->idle_tick_period = tick_start;
			kc->idle_tick_overlap = 0;
		}
		tick_start = max(tick_start, kc->idle_dyntick_entry);
		यदि (!WARN_ON_ONCE(now < tick_start))
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
स्थिर अन्तःस्थ u64 kcpustat_tick_overlap(शून्य)
{
	संरचना kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	फर्काऊ overlap;
}

शून्य kcpustat_dyntick_start(u64 now)
{
	संरचना kernel_cpustat *kc = kcpustat_this_cpu;

	यदि (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_start();
		kc->idle_dyntick = true;
		kc->idle_dyntick_entry = now;
		kcpustat_idle_start(kc, now);
	}
}

शून्य kcpustat_irq_enter(u64 now)
{
	संरचना kernel_cpustat *kc = kcpustat_this_cpu;

	यदि (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

शून्य kcpustat_irq_exit(u64 now)
{
	संरचना kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	यदि (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_start(kc, now);
}

स्थिर u64 kcpustat_field_dyntick(पूर्णाङ्क cpu, गणना cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	संरचना kernel_cpustat *kc = &kcpustat_cpu(cpu);
	पूर्णाङ्क iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	अचिन्हित पूर्णाङ्क seq;
	u64 idle;

	गर {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		यदि (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} जबसम्म (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	फर्काऊ idle;
}

u64 kcpustat_field_idle(पूर्णाङ्क cpu)
{
	फर्काऊ kcpustat_field_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_चिह्न_GPL(kcpustat_field_idle);

u64 kcpustat_field_iowait(पूर्णाङ्क cpu)
{
	फर्काऊ kcpustat_field_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_चिह्न_GPL(kcpustat_field_iowait);
#अन्यथा
स्थिर अन्तःस्थ u64 kcpustat_tick_overlap(शून्य)
{
	फर्काऊ 0;
}

स्थिर u64 kcpustat_field_dyntick(पूर्णाङ्क cpu, गणना cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	फर्काऊ kcpustat_cpu(cpu).cpustat[idx];
}
#endif /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

स्थिर u64 get_cpu_sleep_time_us(पूर्णाङ्क cpu, गणना cpu_usage_stat idx,
				 bool compute_delta, u64 *last_update_time)
{
	ktime_t now = ktime_get();
	u64 res;

	यदि (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_field(idx, cpu);
	अन्यथा
		res = kcpustat_field_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_PER_USEC);

	यदि (last_update_time)
		*last_update_time = ktime_to_us(now);

	फर्काऊ res;
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
u64 get_cpu_idle_time_us(पूर्णाङ्क cpu, u64 *last_update_time)
{
	फर्काऊ get_cpu_sleep_time_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), last_update_time);
}
EXPORT_चिह्न_GPL(get_cpu_idle_time_us);

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
u64 get_cpu_iowait_time_us(पूर्णाङ्क cpu, u64 *last_update_time)
{
	फर्काऊ get_cpu_sleep_time_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), last_update_time);
}
EXPORT_चिह्न_GPL(get_cpu_iowait_time_us);

/*
 * Use precise platform statistics if available:
 */
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

शून्य vtime_account_irq(संरचना task_struct *tsk, अचिन्हित पूर्णाङ्क offset)
{
	अचिन्हित पूर्णाङ्क pc = irq_गन्ती() - offset;

	यदि (pc & HARDIRQ_OFFSET) {
		vtime_account_hardirq(tsk);
	} अन्यथा यदि (pc & SOFTIRQ_OFFSET) {
		vtime_account_softirq(tsk);
	} अन्यथा यदि (!kcpustat_idle_dyntick()) {
		यदि (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} अन्यथा {
			vtime_account_kernel(tsk);
		}
	} अन्यथा {
		vtime_reset();
	}
}

शून्य cputime_adjust(संरचना task_cputime *curr, संरचना prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

शून्य task_cputime_adjusted(संरचना task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_चिह्न_GPL(task_cputime_adjusted);

शून्य thread_group_cputime_adjusted(संरचना task_struct *p, u64 *ut, u64 *st)
{
	संरचना task_cputime cputime;

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
शून्य account_process_tick(संरचना task_struct *p, पूर्णाङ्क user_tick)
{
	u64 cputime, steal;

	यदि (vtime_accounting_enabled_this_cpu())
		फर्काऊ;

	यदि (kcpustat_idle_dyntick())
		फर्काऊ;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	यदि (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		फर्काऊ;
	}

	steal = steal_account_process_time(अच_दीर्घ_उच्च);

	यदि (steal >= cputime)
		फर्काऊ;

	cputime -= steal;

	यदि (user_tick)
		account_user_time(p, cputime);
	अन्यथा यदि ((p != this_rq()->idle) || (irq_गन्ती() != HARDIRQ_OFFSET))
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
शून्य cputime_adjust(संरचना task_cputime *curr, संरचना prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	u64 rtime, stime, utime;
	अचिन्हित दीर्घ flags;

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
	यदि (prev->stime + prev->utime >= rtime)
		गच्छ out;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	यदि (stime == 0) {
		utime = rtime;
		गच्छ update;
	}

	यदि (utime == 0) {
		stime = rtime;
		गच्छ update;
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
	यदि (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	यदि (utime < prev->utime) {
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

शून्य task_cputime_adjusted(संरचना task_struct *p, u64 *ut, u64 *st)
{
	संरचना task_cputime cputime = {
		.sum_exec_runtime = p->se.sum_exec_runtime,
	};

	यदि (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.sum_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_चिह्न_GPL(task_cputime_adjusted);

शून्य thread_group_cputime_adjusted(संरचना task_struct *p, u64 *ut, u64 *st)
{
	संरचना task_cputime cputime;

	thread_group_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->सङ्केत->prev_cputime, ut, st);
}
#endif /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
स्थिर u64 vtime_delta(संरचना vtime *vtime)
{
	अचिन्हित दीर्घ दीर्घ घड़ी;

	घड़ी = sched_clock();
	यदि (घड़ी < vtime->starttime)
		फर्काऊ 0;

	फर्काऊ घड़ी - vtime->starttime;
}

स्थिर u64 get_vtime_delta(संरचना vtime *vtime)
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

	फर्काऊ delta - other;
}

स्थिर शून्य vtime_account_system(संरचना task_struct *tsk,
				 संरचना vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	यदि (vtime->stime >= TICK_NSEC) {
		account_system_time(tsk, irq_गन्ती(), vtime->stime);
		vtime->stime = 0;
	}
}

स्थिर शून्य vtime_account_guest(संरचना task_struct *tsk,
				संरचना vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	यदि (vtime->gtime >= TICK_NSEC) {
		account_guest_time(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

स्थिर शून्य __vtime_account_kernel(संरचना task_struct *tsk,
				   संरचना vtime *vtime)
{
	/* We might have scheduled out from guest path */
	यदि (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	अन्यथा
		vtime_account_system(tsk, vtime);
}

शून्य vtime_account_kernel(संरचना task_struct *tsk)
{
	संरचना vtime *vtime = &tsk->vtime;

	यदि (!vtime_delta(vtime))
		फर्काऊ;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_end(&vtime->seqcount);
}

शून्य vtime_user_enter(संरचना task_struct *tsk)
{
	संरचना vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_end(&vtime->seqcount);
}

शून्य vtime_user_exit(संरचना task_struct *tsk)
{
	संरचना vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	यदि (vtime->utime >= TICK_NSEC) {
		account_user_time(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}

शून्य vtime_guest_enter(संरचना task_struct *tsk)
{
	संरचना vtime *vtime = &tsk->vtime;
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
EXPORT_चिह्न_GPL(vtime_guest_enter);

शून्य vtime_guest_exit(संरचना task_struct *tsk)
{
	संरचना vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_चिह्न_GPL(vtime_guest_exit);

स्थिर शून्य __vtime_account_idle(संरचना vtime *vtime)
{
	account_idle_time(get_vtime_delta(vtime));
}

शून्य vtime_task_switch_generic(संरचना task_struct *prev)
{
	संरचना vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	यदि (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	अन्यथा
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_end(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	यदि (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	अन्यथा यदि (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	अन्यथा
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_end(&vtime->seqcount);
}

शून्य vtime_init_idle(संरचना task_struct *t, पूर्णाङ्क cpu)
{
	संरचना vtime *vtime = &t->vtime;
	अचिन्हित दीर्घ flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_end(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(संरचना task_struct *t)
{
	संरचना vtime *vtime = &t->vtime;
	अचिन्हित पूर्णाङ्क seq;
	u64 gtime;

	यदि (!vtime_accounting_enabled())
		फर्काऊ t->gtime;

	गर {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		यदि (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} जबसम्म (read_seqcount_retry(&vtime->seqcount, seq));

	फर्काऊ gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(संरचना task_struct *t, u64 *utime, u64 *stime)
{
	संरचना vtime *vtime = &t->vtime;
	अचिन्हित पूर्णाङ्क seq;
	u64 delta;
	पूर्णाङ्क ret;

	यदि (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		फर्काऊ false;
	}

	गर {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		यदि (vtime->state < VTIME_SYS)
			जारी;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		यदि (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		अन्यथा
			*utime += vtime->utime + delta;
	} जबसम्म (read_seqcount_retry(&vtime->seqcount, seq));

	फर्काऊ ret;
}

स्थिर पूर्णाङ्क vtime_state_fetch(संरचना vtime *vtime, पूर्णाङ्क cpu)
{
	पूर्णाङ्क state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	यदि (vtime->cpu != cpu && vtime->cpu != -1)
		फर्काऊ -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	यदि (state == VTIME_INACTIVE)
		फर्काऊ -EAGAIN;

	फर्काऊ state;
}

स्थिर u64 kcpustat_user_vtime(संरचना vtime *vtime)
{
	यदि (vtime->state == VTIME_USER)
		फर्काऊ vtime->utime + vtime_delta(vtime);
	अन्यथा यदि (vtime->state == VTIME_GUEST)
		फर्काऊ vtime->gtime + vtime_delta(vtime);
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क kcpustat_field_vtime(u64 *cpustat,
				संरचना task_struct *tsk,
				गणना cpu_usage_stat usage,
				पूर्णाङ्क cpu, u64 *val)
{
	संरचना vtime *vtime = &tsk->vtime;
	अचिन्हित पूर्णाङ्क seq;

	गर {
		पूर्णाङ्क state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		यदि (state < 0)
			फर्काऊ state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		विकल्प (usage) {
		पक्ष CPUTIME_SYSTEM:
			यदि (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			रोक;
		पक्ष CPUTIME_USER:
			यदि (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			रोक;
		पक्ष CPUTIME_NICE:
			यदि (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			रोक;
		पक्ष CPUTIME_GUEST:
			यदि (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			रोक;
		पक्ष CPUTIME_GUEST_NICE:
			यदि (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			रोक;
		पक्ष CPUTIME_IDLE:
			यदि (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			रोक;
		पक्ष CPUTIME_IOWAIT:
			यदि (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			रोक;
		स्वभाव:
			रोक;
		}
	} जबसम्म (read_seqcount_retry(&vtime->seqcount, seq));

	फर्काऊ 0;
}

u64 kcpustat_field(गणना cpu_usage_stat usage, पूर्णाङ्क cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	संरचना rq *rq;
	पूर्णाङ्क err;

	यदि (!vtime_generic_enabled_cpu(cpu))
		फर्काऊ kcpustat_field_default(usage, cpu);

	rq = cpu_rq(cpu);

	लागि (;;) {
		संरचना task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		यदि (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			फर्काऊ cpustat[usage];
		}

		err = kcpustat_field_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		यदि (!err)
			फर्काऊ val;

		cpu_relax();
	}
}
EXPORT_चिह्न_GPL(kcpustat_field);

स्थिर पूर्णाङ्क kcpustat_cpu_fetch_vtime(संरचना kernel_cpustat *dst,
				    अचर संरचना kernel_cpustat *src,
				    संरचना task_struct *tsk, पूर्णाङ्क cpu)
{
	संरचना vtime *vtime = &tsk->vtime;
	अचिन्हित पूर्णाङ्क seq;

	गर {
		u64 *cpustat;
		u64 delta;
		पूर्णाङ्क state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		यदि (state < 0)
			फर्काऊ state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		यदि (state < VTIME_IDLE)
			जारी;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		विकल्प (state) {
		पक्ष VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			रोक;
		पक्ष VTIME_USER:
			यदि (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			अन्यथा
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			रोक;
		पक्ष VTIME_GUEST:
			यदि (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} अन्यथा {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			रोक;
		पक्ष VTIME_IDLE:
			यदि (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			अन्यथा
				cpustat[CPUTIME_IDLE] += delta;
			रोक;
		स्वभाव:
			WARN_ON_ONCE(1);
		}
	} जबसम्म (read_seqcount_retry(&vtime->seqcount, seq));

	फर्काऊ 0;
}

शून्य kcpustat_cpu_fetch(संरचना kernel_cpustat *dst, पूर्णाङ्क cpu)
{
	अचर संरचना kernel_cpustat *src = &kcpustat_cpu(cpu);
	संरचना rq *rq;
	पूर्णाङ्क err;

	यदि (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		फर्काऊ;
	}

	rq = cpu_rq(cpu);

	लागि (;;) {
		संरचना task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		यदि (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			फर्काऊ;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		यदि (!err)
			फर्काऊ;

		cpu_relax();
	}
}
EXPORT_चिह्न_GPL(kcpustat_cpu_fetch);

#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
