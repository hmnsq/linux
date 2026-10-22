// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#ضمن <linux/sched/clock.h>
#ضمن <linux/sched/cputime.h>
#ضمن <linux/tsacct_kern.h>
#ضمن "sched.h"

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #ضمن <asm/cputime.h>
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

DEFINE_STATIC_کلید_FALSE(sched_clock_irqtime);

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
DEFINE_PER_CPU(بنية irqtime, cpu_irqtime);

فراغ enable_sched_clock_irqtime(فراغ)
{
	static_branch_enable(&sched_clock_irqtime);
}

فراغ disable_sched_clock_irqtime(فراغ)
{
	إذا (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

ساكن فراغ irqtime_account_delta(بنية irqtime *irqtime, u64 delta,
				  تعداد cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->total += delta;
	إذا (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_end(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
فراغ irqtime_account_irq(بنية task_struct *curr, unsigned صحيح offset)
{
	بنية irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	unsigned صحيح pc;
	s64 delta;
	صحيح cpu;

	إذا (!irqtime_enabled())
		أرجع;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_start_time;
	irqtime->irq_start_time += delta;
	pc = irq_گنتی() - offset;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	إذا (pc & HARDIRQ_MASK)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	وإلا إذا ((pc & SOFTIRQ_OFFSET) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

ساكن u64 irqtime_tick_accounted(u64 maxtime)
{
	بنية irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	أرجع delta;
}

#وإلا /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ساكن u64 irqtime_tick_accounted(u64 dummy)
{
	أرجع 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

ساكن مضمن فراغ task_group_account_field(بنية task_struct *p, صحيح index,
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
فراغ account_user_time(بنية task_struct *p, u64 cputime)
{
	صحيح index;

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
فراغ account_guest_time(بنية task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	إذا (task_nice(p) > 0) {
		task_group_account_field(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} وإلا {
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
فراغ account_system_index_time(بنية task_struct *p,
			       u64 cputime, تعداد cpu_usage_stat index)
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
فراغ account_system_time(بنية task_struct *p, صحيح hardirq_offset, u64 cputime)
{
	صحيح index;

	إذا ((p->flags & PF_VCPU) && (irq_گنتی() - hardirq_offset == 0)) {
		account_guest_time(p, cputime);
		أرجع;
	}

	إذا (hardirq_گنتی() - hardirq_offset)
		index = CPUTIME_IRQ;
	وإلا إذا (in_serving_softirq())
		index = CPUTIME_SOFTIRQ;
	وإلا
		index = CPUTIME_SYSTEM;

	account_system_index_time(p, cputime, index);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
فراغ account_steal_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
فراغ account_idle_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	إذا (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	وإلا
		cpustat[CPUTIME_IDLE] += cputime;
}


#ifdef CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
فراغ __account_forceidle_time(بنية task_struct *p, u64 delta)
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
DEFINE_STATIC_کلید_FALSE(paravirt_steal_enabled);

#ifdef CONFIG_HAVE_PV_STEAL_CLOCK_GEN
ساكن u64 native_steal_clock(صحيح cpu)
{
	أرجع 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#endif
#endif

ساكن __always_inline u64 steal_account_process_time(u64 maxtime)
{
#ifdef CONFIG_PARAVIRT
	إذا (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_time;
		steal = min(steal, maxtime);
		account_steal_time(steal);
		this_rq()->prev_steal_time += steal;

		أرجع steal;
	}
#endif /* CONFIG_PARAVIRT */
	أرجع 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
ساكن مضمن u64 account_other_time(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_time(max);

	إذا (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	أرجع accounted;
}

#ifdef CONFIG_64BIT
ساكن مضمن u64 read_sum_exec_runtime(بنية task_struct *t)
{
	أرجع t->se.sum_exec_runtime;
}
#وإلا /* !CONFIG_64BIT: */
ساكن u64 read_sum_exec_runtime(بنية task_struct *t)
{
	u64 ns;
	بنية rq_flags rf;
	بنية rq *rq;

	rq = task_rq_lock(t, &rf);
	ns = t->se.sum_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	أرجع ns;
}
#endif /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
فراغ thread_group_cputime(بنية task_struct *tsk, بنية task_cputime *times)
{
	بنية signal_struct *sig = tsk->إشارة;
	بنية task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	إذا (same_thread_group(current, tsk))
		(فراغ) task_sched_runtime(current);

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
ساكن فراغ irqtime_account_process_tick(بنية task_struct *p, صحيح user_tick,
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
	إذا (other >= cputime)
		أرجع;

	cputime -= other;

	إذا (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_index_time(p, cputime, CPUTIME_SOFTIRQ);
	} وإلا إذا (user_tick) {
		account_user_time(p, cputime);
	} وإلا إذا (p == this_rq()->idle) {
		account_idle_time(cputime);
	} وإلا إذا (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_time(p, cputime);
	} وإلا {
		account_system_index_time(p, cputime, CPUTIME_SYSTEM);
	}
}

#وإلا /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ساكن مضمن فراغ irqtime_account_process_tick(بنية task_struct *p, صحيح user_tick,
						u64 cputime) { }
#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#إذا defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
ساكن فراغ kcpustat_idle_stop(بنية kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	صحيح iowait;

	إذا (!kc->idle_elapse)
		أرجع;

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

	إذا (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	وإلا
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

ساكن فراغ kcpustat_idle_start(بنية kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	إذا (kc->idle_elapse)
		أرجع;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

فراغ kcpustat_dyntick_stop(u64 now, u64 tick_start)
{
	بنية kernel_cpustat *kc = kcpustat_this_cpu;

	إذا (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		إذا (kc->idle_tick_period != tick_start) {
			kc->idle_tick_period = tick_start;
			kc->idle_tick_overlap = 0;
		}
		tick_start = max(tick_start, kc->idle_dyntick_entry);
		إذا (!WARN_ON_ONCE(now < tick_start))
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
ساكن مضمن u64 kcpustat_tick_overlap(فراغ)
{
	بنية kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	أرجع overlap;
}

فراغ kcpustat_dyntick_start(u64 now)
{
	بنية kernel_cpustat *kc = kcpustat_this_cpu;

	إذا (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_start();
		kc->idle_dyntick = true;
		kc->idle_dyntick_entry = now;
		kcpustat_idle_start(kc, now);
	}
}

فراغ kcpustat_irq_enter(u64 now)
{
	بنية kernel_cpustat *kc = kcpustat_this_cpu;

	إذا (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

فراغ kcpustat_irq_exit(u64 now)
{
	بنية kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	إذا (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_start(kc, now);
}

ساكن u64 kcpustat_field_dyntick(صحيح cpu, تعداد cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	بنية kernel_cpustat *kc = &kcpustat_cpu(cpu);
	صحيح iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	unsigned صحيح seq;
	u64 idle;

	افعل {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		إذا (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} طالما (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	أرجع idle;
}

u64 kcpustat_field_idle(صحيح cpu)
{
	أرجع kcpustat_field_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_علامت_GPL(kcpustat_field_idle);

u64 kcpustat_field_iowait(صحيح cpu)
{
	أرجع kcpustat_field_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_علامت_GPL(kcpustat_field_iowait);
#وإلا
ساكن مضمن u64 kcpustat_tick_overlap(فراغ)
{
	أرجع 0;
}

ساكن u64 kcpustat_field_dyntick(صحيح cpu, تعداد cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	أرجع kcpustat_cpu(cpu).cpustat[idx];
}
#endif /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

ساكن u64 get_cpu_sleep_time_us(صحيح cpu, تعداد cpu_usage_stat idx,
				 bool compute_delta, u64 *last_update_time)
{
	ktime_t now = ktime_get();
	u64 res;

	إذا (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_field(idx, cpu);
	وإلا
		res = kcpustat_field_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_PER_USEC);

	إذا (last_update_time)
		*last_update_time = ktime_to_us(now);

	أرجع res;
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
u64 get_cpu_idle_time_us(صحيح cpu, u64 *last_update_time)
{
	أرجع get_cpu_sleep_time_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), last_update_time);
}
EXPORT_علامت_GPL(get_cpu_idle_time_us);

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
u64 get_cpu_iowait_time_us(صحيح cpu, u64 *last_update_time)
{
	أرجع get_cpu_sleep_time_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), last_update_time);
}
EXPORT_علامت_GPL(get_cpu_iowait_time_us);

/*
 * Use precise platform statistics if available:
 */
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

فراغ vtime_account_irq(بنية task_struct *tsk, unsigned صحيح offset)
{
	unsigned صحيح pc = irq_گنتی() - offset;

	إذا (pc & HARDIRQ_OFFSET) {
		vtime_account_hardirq(tsk);
	} وإلا إذا (pc & SOFTIRQ_OFFSET) {
		vtime_account_softirq(tsk);
	} وإلا إذا (!kcpustat_idle_dyntick()) {
		إذا (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} وإلا {
			vtime_account_kernel(tsk);
		}
	} وإلا {
		vtime_reset();
	}
}

فراغ cputime_adjust(بنية task_cputime *curr, بنية prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

فراغ task_cputime_adjusted(بنية task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_علامت_GPL(task_cputime_adjusted);

فراغ thread_group_cputime_adjusted(بنية task_struct *p, u64 *ut, u64 *st)
{
	بنية task_cputime cputime;

	thread_group_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#وإلا /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
فراغ account_process_tick(بنية task_struct *p, صحيح user_tick)
{
	u64 cputime, steal;

	إذا (vtime_accounting_enabled_this_cpu())
		أرجع;

	إذا (kcpustat_idle_dyntick())
		أرجع;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	إذا (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		أرجع;
	}

	steal = steal_account_process_time(ULONG_MAX);

	إذا (steal >= cputime)
		أرجع;

	cputime -= steal;

	إذا (user_tick)
		account_user_time(p, cputime);
	وإلا إذا ((p != this_rq()->idle) || (irq_گنتی() != HARDIRQ_OFFSET))
		account_system_time(p, HARDIRQ_OFFSET, cputime);
	وإلا
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
فراغ cputime_adjust(بنية task_cputime *curr, بنية prev_cputime *prev,
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
	إذا (prev->stime + prev->utime >= rtime)
		اذهب out;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	إذا (stime == 0) {
		utime = rtime;
		اذهب update;
	}

	إذا (utime == 0) {
		stime = rtime;
		اذهب update;
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
	إذا (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	إذا (utime < prev->utime) {
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

فراغ task_cputime_adjusted(بنية task_struct *p, u64 *ut, u64 *st)
{
	بنية task_cputime cputime = {
		.sum_exec_runtime = p->se.sum_exec_runtime,
	};

	إذا (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.sum_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_علامت_GPL(task_cputime_adjusted);

فراغ thread_group_cputime_adjusted(بنية task_struct *p, u64 *ut, u64 *st)
{
	بنية task_cputime cputime;

	thread_group_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->إشارة->prev_cputime, ut, st);
}
#endif /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
ساكن u64 vtime_delta(بنية vtime *vtime)
{
	unsigned long long clock;

	clock = sched_clock();
	إذا (clock < vtime->starttime)
		أرجع 0;

	أرجع clock - vtime->starttime;
}

ساكن u64 get_vtime_delta(بنية vtime *vtime)
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

	أرجع delta - other;
}

ساكن فراغ vtime_account_system(بنية task_struct *tsk,
				 بنية vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	إذا (vtime->stime >= TICK_NSEC) {
		account_system_time(tsk, irq_گنتی(), vtime->stime);
		vtime->stime = 0;
	}
}

ساكن فراغ vtime_account_guest(بنية task_struct *tsk,
				بنية vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	إذا (vtime->gtime >= TICK_NSEC) {
		account_guest_time(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

ساكن فراغ __vtime_account_kernel(بنية task_struct *tsk,
				   بنية vtime *vtime)
{
	/* We might have scheduled out from guest path */
	إذا (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	وإلا
		vtime_account_system(tsk, vtime);
}

فراغ vtime_account_kernel(بنية task_struct *tsk)
{
	بنية vtime *vtime = &tsk->vtime;

	إذا (!vtime_delta(vtime))
		أرجع;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_end(&vtime->seqcount);
}

فراغ vtime_user_enter(بنية task_struct *tsk)
{
	بنية vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_end(&vtime->seqcount);
}

فراغ vtime_user_exit(بنية task_struct *tsk)
{
	بنية vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	إذا (vtime->utime >= TICK_NSEC) {
		account_user_time(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}

فراغ vtime_guest_enter(بنية task_struct *tsk)
{
	بنية vtime *vtime = &tsk->vtime;
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
EXPORT_علامت_GPL(vtime_guest_enter);

فراغ vtime_guest_exit(بنية task_struct *tsk)
{
	بنية vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_علامت_GPL(vtime_guest_exit);

ساكن فراغ __vtime_account_idle(بنية vtime *vtime)
{
	account_idle_time(get_vtime_delta(vtime));
}

فراغ vtime_task_switch_generic(بنية task_struct *prev)
{
	بنية vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	إذا (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	وإلا
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_end(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	إذا (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	وإلا إذا (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	وإلا
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_end(&vtime->seqcount);
}

فراغ vtime_init_idle(بنية task_struct *t, صحيح cpu)
{
	بنية vtime *vtime = &t->vtime;
	unsigned long flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_end(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(بنية task_struct *t)
{
	بنية vtime *vtime = &t->vtime;
	unsigned صحيح seq;
	u64 gtime;

	إذا (!vtime_accounting_enabled())
		أرجع t->gtime;

	افعل {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		إذا (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} طالما (read_seqcount_retry(&vtime->seqcount, seq));

	أرجع gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(بنية task_struct *t, u64 *utime, u64 *stime)
{
	بنية vtime *vtime = &t->vtime;
	unsigned صحيح seq;
	u64 delta;
	صحيح ret;

	إذا (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		أرجع false;
	}

	افعل {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		إذا (vtime->state < VTIME_SYS)
			استمر;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		إذا (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		وإلا
			*utime += vtime->utime + delta;
	} طالما (read_seqcount_retry(&vtime->seqcount, seq));

	أرجع ret;
}

ساكن صحيح vtime_state_fetch(بنية vtime *vtime, صحيح cpu)
{
	صحيح state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	إذا (vtime->cpu != cpu && vtime->cpu != -1)
		أرجع -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	إذا (state == VTIME_INACTIVE)
		أرجع -EAGAIN;

	أرجع state;
}

ساكن u64 kcpustat_user_vtime(بنية vtime *vtime)
{
	إذا (vtime->state == VTIME_USER)
		أرجع vtime->utime + vtime_delta(vtime);
	وإلا إذا (vtime->state == VTIME_GUEST)
		أرجع vtime->gtime + vtime_delta(vtime);
	أرجع 0;
}

ساكن صحيح kcpustat_field_vtime(u64 *cpustat,
				بنية task_struct *tsk,
				تعداد cpu_usage_stat usage,
				صحيح cpu, u64 *val)
{
	بنية vtime *vtime = &tsk->vtime;
	unsigned صحيح seq;

	افعل {
		صحيح state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		إذا (state < 0)
			أرجع state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		بدل (usage) {
		حالة CPUTIME_SYSTEM:
			إذا (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			اكسر;
		حالة CPUTIME_USER:
			إذا (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			اكسر;
		حالة CPUTIME_NICE:
			إذا (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			اكسر;
		حالة CPUTIME_GUEST:
			إذا (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			اكسر;
		حالة CPUTIME_GUEST_NICE:
			إذا (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			اكسر;
		حالة CPUTIME_IDLE:
			إذا (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			اكسر;
		حالة CPUTIME_IOWAIT:
			إذا (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			اكسر;
		افتراضي:
			اكسر;
		}
	} طالما (read_seqcount_retry(&vtime->seqcount, seq));

	أرجع 0;
}

u64 kcpustat_field(تعداد cpu_usage_stat usage, صحيح cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	بنية rq *rq;
	صحيح err;

	إذا (!vtime_generic_enabled_cpu(cpu))
		أرجع kcpustat_field_default(usage, cpu);

	rq = cpu_rq(cpu);

	لكل (;;) {
		بنية task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		إذا (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			أرجع cpustat[usage];
		}

		err = kcpustat_field_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		إذا (!err)
			أرجع val;

		cpu_relax();
	}
}
EXPORT_علامت_GPL(kcpustat_field);

ساكن صحيح kcpustat_cpu_fetch_vtime(بنية kernel_cpustat *dst,
				    ثابت بنية kernel_cpustat *src,
				    بنية task_struct *tsk, صحيح cpu)
{
	بنية vtime *vtime = &tsk->vtime;
	unsigned صحيح seq;

	افعل {
		u64 *cpustat;
		u64 delta;
		صحيح state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		إذا (state < 0)
			أرجع state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		إذا (state < VTIME_IDLE)
			استمر;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		بدل (state) {
		حالة VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			اكسر;
		حالة VTIME_USER:
			إذا (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			وإلا
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			اكسر;
		حالة VTIME_GUEST:
			إذا (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} وإلا {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			اكسر;
		حالة VTIME_IDLE:
			إذا (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			وإلا
				cpustat[CPUTIME_IDLE] += delta;
			اكسر;
		افتراضي:
			WARN_ON_ONCE(1);
		}
	} طالما (read_seqcount_retry(&vtime->seqcount, seq));

	أرجع 0;
}

فراغ kcpustat_cpu_fetch(بنية kernel_cpustat *dst, صحيح cpu)
{
	ثابت بنية kernel_cpustat *src = &kcpustat_cpu(cpu);
	بنية rq *rq;
	صحيح err;

	إذا (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		أرجع;
	}

	rq = cpu_rq(cpu);

	لكل (;;) {
		بنية task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		إذا (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			أرجع;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		إذا (!err)
			أرجع;

		cpu_relax();
	}
}
EXPORT_علامت_GPL(kcpustat_cpu_fetch);

#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
