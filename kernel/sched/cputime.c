// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#شامل <linux/sched/clock.h>
#شامل <linux/sched/cputime.h>
#شامل <linux/tsacct_kern.h>
#شامل "sched.h"

#اگر_تعریف_شدہ CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #شامل <asm/cputime.h>
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING

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
DEFINE_فی_CPU(ساخت irqtime, cpu_irqtime);

خالی enable_sched_clock_irqtime(خالی)
{
	static_branch_enable(&sched_clock_irqtime);
}

خالی disable_sched_clock_irqtime(خالی)
{
	اگر (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

ساکن خالی irqtime_account_delta(ساخت irqtime *irqtime, u64 delta,
				  شمار cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->کل += delta;
	اگر (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_انتہا(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
خالی irqtime_account_irq(ساخت task_struct *curr, غیرعلامتی صحیح انحراف)
{
	ساخت irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	غیرعلامتی صحیح pc;
	s64 delta;
	صحیح cpu;

	اگر (!irqtime_enabled())
		واپس;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_آغاز_وقت;
	irqtime->irq_آغاز_وقت += delta;
	pc = irq_گنتی() - انحراف;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	اگر (pc & HARDIRQ_نقاب)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	ورنہ اگر ((pc & SOFTIRQ_انحراف) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

ساکن u64 irqtime_tick_accounted(u64 maxtime)
{
	ساخت irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	واپس delta;
}

#ورنہ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ساکن u64 irqtime_tick_accounted(u64 dummy)
{
	واپس 0;
}

#اگر_ختم /* !CONFIG_IRQ_TIME_ACCOUNTING */

ساکن اندرونی خالی task_گروہ_account_میدان(ساخت task_struct *p, صحیح اشاریہ,
					    u64 tmp)
{
	/*
	 * Since all updates are sure to touch the root cgroup, we
	 * get ourselves ahead and touch it first. If the root cgroup
	 * is the only cgroup, then nothing else should be necessary.
	 *
	 */
	__this_cpu_add(kernel_cpustat.cpustat[اشاریہ], tmp);

	cgroup_account_cputime_میدان(p, اشاریہ, tmp);
}

/*
 * Account user CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in user space since the last update
 */
خالی account_user_وقت(ساخت task_struct *p, u64 cputime)
{
	صحیح اشاریہ;

	/* Add user time to process. */
	p->utime += cputime;
	account_گروہ_user_وقت(p, cputime);

	اشاریہ = (task_nice(p) > 0) ? CPUTIME_NICE : CPUTIME_USER;

	/* Add user time to cpustat. */
	task_گروہ_account_میدان(p, اشاریہ, cputime);

	/* Account for user time used */
	acct_account_cputime(p);
}

/*
 * Account guest CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in virtual machine since the last update
 */
خالی account_guest_وقت(ساخت task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_گروہ_user_وقت(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	اگر (task_nice(p) > 0) {
		task_گروہ_account_میدان(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} ورنہ {
		task_گروہ_account_میدان(p, CPUTIME_USER, cputime);
		cpustat[CPUTIME_GUEST] += cputime;
	}
}

/*
 * Account system CPU time to a process and desired cpustat field
 * @p: the process that the CPU time gets accounted to
 * @cputime: the CPU time spent in kernel space since the last update
 * @index: pointer to cpustat field that has to be updated
 */
خالی account_system_اشاریہ_وقت(ساخت task_struct *p,
			       u64 cputime, شمار cpu_usage_stat اشاریہ)
{
	/* Add system time to process. */
	p->stime += cputime;
	account_گروہ_system_وقت(p, cputime);

	/* Add system time to cpustat. */
	task_گروہ_account_میدان(p, اشاریہ, cputime);

	/* Account for system time used */
	acct_account_cputime(p);
}

/*
 * Account system CPU time to a process.
 * @p: the process that the CPU time gets accounted to
 * @hardirq_offset: the offset to subtract from hardirq_count()
 * @cputime: the CPU time spent in kernel space since the last update
 */
خالی account_system_وقت(ساخت task_struct *p, صحیح hardirq_انحراف, u64 cputime)
{
	صحیح اشاریہ;

	اگر ((p->flags & PF_VCPU) && (irq_گنتی() - hardirq_انحراف == 0)) {
		account_guest_وقت(p, cputime);
		واپس;
	}

	اگر (hardirq_گنتی() - hardirq_انحراف)
		اشاریہ = CPUTIME_IRQ;
	ورنہ اگر (اندر_serving_softirq())
		اشاریہ = CPUTIME_SOFTIRQ;
	ورنہ
		اشاریہ = CPUTIME_SYSTEM;

	account_system_اشاریہ_وقت(p, cputime, اشاریہ);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
خالی account_steal_وقت(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
خالی account_idle_وقت(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	اگر (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	ورنہ
		cpustat[CPUTIME_IDLE] += cputime;
}


#اگر_تعریف_شدہ CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
خالی __account_forceidle_time(ساخت task_struct *p, u64 delta)
{
	__schedstat_add(p->stats.core_forceidle_مجموع, delta);

	task_گروہ_account_میدان(p, CPUTIME_FORCEIDLE, delta);
}
#اگر_ختم /* CONFIG_SCHED_CORE */

/*
 * When a guest is interrupted for a longer amount of time, missed clock
 * ticks are not redelivered later. Due to that, this function may on
 * occasion account more time than the calling functions think elapsed.
 */
#اگر_تعریف_شدہ CONFIG_PARAVIRT
DEFINE_STATIC_کلید_FALSE(paravirt_steal_enabled);

#اگر_تعریف_شدہ CONFIG_HAVE_PV_STEAL_CLOCK_GEN
ساکن u64 native_steal_clock(صحیح cpu)
{
	واپس 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#اگر_ختم
#اگر_ختم

ساکن __always_inline u64 steal_account_process_وقت(u64 maxtime)
{
#اگر_تعریف_شدہ CONFIG_PARAVIRT
	اگر (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->پچھلا_steal_وقت;
		steal = min(steal, maxtime);
		account_steal_وقت(steal);
		this_rq()->پچھلا_steal_وقت += steal;

		واپس steal;
	}
#اگر_ختم /* CONFIG_PARAVIRT */
	واپس 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
ساکن اندرونی u64 account_other_وقت(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_وقت(max);

	اگر (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	واپس accounted;
}

#اگر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
ساکن اندرونی u64 read_مجموع_exec_runtime(ساخت task_struct *t)
{
	واپس t->se.مجموع_exec_runtime;
}
#ورنہ /* !CONFIG_64BIT: */
ساکن u64 read_مجموع_exec_runtime(ساخت task_struct *t)
{
	u64 ns;
	ساخت rq_flags rf;
	ساخت rq *rq;

	rq = task_rq_تالا_2(t, &rf);
	ns = t->se.مجموع_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	واپس ns;
}
#اگر_ختم /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
خالی thread_گروہ_cputime(ساخت task_struct *tsk, ساخت task_cputime *times)
{
	ساخت signal_struct *sig = tsk->اشارہ;
	ساخت task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	اگر (same_thread_گروہ(current, tsk))
		(خالی) task_sched_runtime(current);

	guard(rcu)();
	scoped_seqlock_read (&sig->stats_تالا, ss_تالا_irqsave) {
		times->utime = sig->utime;
		times->stime = sig->stime;
		times->مجموع_exec_runtime = sig->مجموع_sched_runtime;

		__for_each_thread(sig, t) {
			task_cputime(t, &utime, &stime);
			times->utime += utime;
			times->stime += stime;
			times->مجموع_exec_runtime += read_مجموع_exec_runtime(t);
		}
	}
}

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
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
ساکن خالی irqtime_account_process_tick(ساخت task_struct *p, صحیح user_tick,
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
	other = account_other_وقت(ULONG_MAX);
	اگر (other >= cputime)
		واپس;

	cputime -= other;

	اگر (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_اشاریہ_وقت(p, cputime, CPUTIME_SOFTIRQ);
	} ورنہ اگر (user_tick) {
		account_user_وقت(p, cputime);
	} ورنہ اگر (p == this_rq()->idle) {
		account_idle_وقت(cputime);
	} ورنہ اگر (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_وقت(p, cputime);
	} ورنہ {
		account_system_اشاریہ_وقت(p, cputime, CPUTIME_SYSTEM);
	}
}

#ورنہ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ساکن اندرونی خالی irqtime_account_process_tick(ساخت task_struct *p, صحیح user_tick,
						u64 cputime) { }
#اگر_ختم /* !CONFIG_IRQ_TIME_ACCOUNTING */

#اگر defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
ساکن خالی kcpustat_idle_stop(ساخت kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	صحیح iowait;

	اگر (!kc->idle_elapse)
		واپس;

	iowait = cpu_has_iowait(smp_processor_id());
	delta = now - kc->idle_entrytime;
	steal = steal_account_process_وقت(delta);

	/*
	 * Record the idle time after substracting the steal time from
	 * previous update sequence. Don't substract the steal time from
	 * the current update sequence to avoid readers moving backward.
	 */
	write_seqcount_begin(&kc->idle_sleeptime_seq);
	steal_delta = min_t(u64, kc->idle_stealtime[iowait], delta);
	delta -= steal_delta;
	kc->idle_stealtime[iowait] -= steal_delta;

	اگر (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	ورنہ
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_انتہا(&kc->idle_sleeptime_seq);
}

ساکن خالی kcpustat_idle_آغاز(ساخت kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	اگر (kc->idle_elapse)
		واپس;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_انتہا(&kc->idle_sleeptime_seq);
}

خالی kcpustat_dyntick_stop(u64 now, u64 tick_آغاز)
{
	ساخت kernel_cpustat *kc = kcpustat_this_cpu;

	اگر (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		اگر (kc->idle_tick_period != tick_آغاز) {
			kc->idle_tick_period = tick_آغاز;
			kc->idle_tick_overlap = 0;
		}
		tick_آغاز = max(tick_آغاز, kc->idle_dyntick_اندراج);
		اگر (!WARN_ON_ONCE(now < tick_آغاز))
			kc->idle_tick_overlap += now - tick_آغاز;
		kcpustat_idle_stop(kc, now);
		kc->idle_dyntick = false;
		vtime_dyntick_stop();
	}
}

/*
 * The first tick after dyntick-idle covers a period that the dyntick-idle
 * accounting may already have accounted in part.
 */
ساکن اندرونی u64 kcpustat_tick_overlap(خالی)
{
	ساخت kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	واپس overlap;
}

خالی kcpustat_dyntick_آغاز(u64 now)
{
	ساخت kernel_cpustat *kc = kcpustat_this_cpu;

	اگر (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_آغاز();
		kc->idle_dyntick = true;
		kc->idle_dyntick_اندراج = now;
		kcpustat_idle_آغاز(kc, now);
	}
}

خالی kcpustat_irq_enter(u64 now)
{
	ساخت kernel_cpustat *kc = kcpustat_this_cpu;

	اگر (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

خالی kcpustat_irq_exit(u64 now)
{
	ساخت kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	اگر (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_وقت_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_آغاز(kc, now);
}

ساکن u64 kcpustat_میدان_dyntick(صحیح cpu, شمار cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	ساخت kernel_cpustat *kc = &kcpustat_cpu(cpu);
	صحیح iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	غیرعلامتی صحیح seq;
	u64 idle;

	کرو {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		اگر (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} جب (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	واپس idle;
}

u64 kcpustat_میدان_idle(صحیح cpu)
{
	واپس kcpustat_میدان_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_علامت_GPL(kcpustat_میدان_idle);

u64 kcpustat_میدان_iowait(صحیح cpu)
{
	واپس kcpustat_میدان_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_علامت_GPL(kcpustat_میدان_iowait);
#ورنہ
ساکن اندرونی u64 kcpustat_tick_overlap(خالی)
{
	واپس 0;
}

ساکن u64 kcpustat_میدان_dyntick(صحیح cpu, شمار cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	واپس kcpustat_cpu(cpu).cpustat[idx];
}
#اگر_ختم /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

ساکن u64 get_cpu_sleep_وقت_us(صحیح cpu, شمار cpu_usage_stat idx,
				 bool compute_delta, u64 *آخری_update_وقت)
{
	ktime_t now = ktime_get();
	u64 res;

	اگر (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_میدان(idx, cpu);
	ورنہ
		res = kcpustat_میدان_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_فی_USEC);

	اگر (آخری_update_وقت)
		*آخری_update_وقت = ktime_مقصد_us(now);

	واپس res;
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
u64 get_cpu_idle_وقت_us(صحیح cpu, u64 *آخری_update_وقت)
{
	واپس get_cpu_sleep_وقت_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), آخری_update_وقت);
}
EXPORT_علامت_GPL(get_cpu_idle_وقت_us);

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
u64 get_cpu_iowait_وقت_us(صحیح cpu, u64 *آخری_update_وقت)
{
	واپس get_cpu_sleep_وقت_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), آخری_update_وقت);
}
EXPORT_علامت_GPL(get_cpu_iowait_وقت_us);

/*
 * Use precise platform statistics if available:
 */
#اگر_تعریف_شدہ CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

خالی vtime_account_irq(ساخت task_struct *tsk, غیرعلامتی صحیح انحراف)
{
	غیرعلامتی صحیح pc = irq_گنتی() - انحراف;

	اگر (pc & HARDIRQ_انحراف) {
		vtime_account_hardirq(tsk);
	} ورنہ اگر (pc & SOFTIRQ_انحراف) {
		vtime_account_softirq(tsk);
	} ورنہ اگر (!kcpustat_idle_dyntick()) {
		اگر (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} ورنہ {
			vtime_account_kernel(tsk);
		}
	} ورنہ {
		vtime_reset();
	}
}

خالی cputime_adjust(ساخت task_cputime *curr, ساخت پچھلا_cputime *پچھلا,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

خالی task_cputime_adjusted(ساخت task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_علامت_GPL(task_cputime_adjusted);

خالی thread_گروہ_cputime_adjusted(ساخت task_struct *p, u64 *ut, u64 *st)
{
	ساخت task_cputime cputime;

	thread_گروہ_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#ورنہ /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
خالی account_process_tick(ساخت task_struct *p, صحیح user_tick)
{
	u64 cputime, steal;

	اگر (vtime_accounting_enabled_this_cpu())
		واپس;

	اگر (kcpustat_idle_dyntick())
		واپس;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	اگر (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		واپس;
	}

	steal = steal_account_process_وقت(ULONG_MAX);

	اگر (steal >= cputime)
		واپس;

	cputime -= steal;

	اگر (user_tick)
		account_user_وقت(p, cputime);
	ورنہ اگر ((p != this_rq()->idle) || (irq_گنتی() != HARDIRQ_انحراف))
		account_system_وقت(p, HARDIRQ_انحراف, cputime);
	ورنہ
		account_idle_وقت(cputime);
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
خالی cputime_adjust(ساخت task_cputime *curr, ساخت پچھلا_cputime *پچھلا,
		    u64 *ut, u64 *st)
{
	u64 rtime, stime, utime;
	غیرعلامتی طویل flags;

	/* Serialize concurrent callers such that we can honour our guarantees */
	raw_spin_تالا_irqsave(&پچھلا->تالا, flags);
	rtime = curr->مجموع_exec_runtime;

	/*
	 * This is possible under two circumstances:
	 *  - rtime isn't monotonic after all (a bug);
	 *  - we got reordered by the lock.
	 *
	 * In both cases this acts as a filter such that the rest of the code
	 * can assume it is monotonic regardless of anything else.
	 */
	اگر (پچھلا->stime + پچھلا->utime >= rtime)
		جاؤ برآمد;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	اگر (stime == 0) {
		utime = rtime;
		جاؤ update;
	}

	اگر (utime == 0) {
		stime = rtime;
		جاؤ update;
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
	اگر (stime < پچھلا->stime)
		stime = پچھلا->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	اگر (utime < پچھلا->utime) {
		utime = پچھلا->utime;
		stime = rtime - utime;
	}

	پچھلا->stime = stime;
	پچھلا->utime = utime;
برآمد:
	*ut = پچھلا->utime;
	*st = پچھلا->stime;
	raw_spin_unlock_irqrestore(&پچھلا->تالا, flags);
}

خالی task_cputime_adjusted(ساخت task_struct *p, u64 *ut, u64 *st)
{
	ساخت task_cputime cputime = {
		.مجموع_exec_runtime = p->se.مجموع_exec_runtime,
	};

	اگر (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.مجموع_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->پچھلا_cputime, ut, st);
}
EXPORT_علامت_GPL(task_cputime_adjusted);

خالی thread_گروہ_cputime_adjusted(ساخت task_struct *p, u64 *ut, u64 *st)
{
	ساخت task_cputime cputime;

	thread_گروہ_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->اشارہ->پچھلا_cputime, ut, st);
}
#اگر_ختم /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#اگر_تعریف_شدہ CONFIG_VIRT_CPU_ACCOUNTING_GEN
ساکن u64 vtime_delta(ساخت vtime *vtime)
{
	غیرعلامتی طویل طویل clock;

	clock = sched_clock();
	اگر (clock < vtime->starttime)
		واپس 0;

	واپس clock - vtime->starttime;
}

ساکن u64 get_vtime_delta(ساخت vtime *vtime)
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
	other = account_other_وقت(delta);
	WARN_ON_ONCE(vtime->state == VTIME_INACTIVE);
	vtime->starttime += delta;

	واپس delta - other;
}

ساکن خالی vtime_account_system(ساخت task_struct *tsk,
				 ساخت vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	اگر (vtime->stime >= TICK_NSEC) {
		account_system_وقت(tsk, irq_گنتی(), vtime->stime);
		vtime->stime = 0;
	}
}

ساکن خالی vtime_account_guest(ساخت task_struct *tsk,
				ساخت vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	اگر (vtime->gtime >= TICK_NSEC) {
		account_guest_وقت(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

ساکن خالی __vtime_account_kernel(ساخت task_struct *tsk,
				   ساخت vtime *vtime)
{
	/* We might have scheduled out from guest path */
	اگر (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	ورنہ
		vtime_account_system(tsk, vtime);
}

خالی vtime_account_kernel(ساخت task_struct *tsk)
{
	ساخت vtime *vtime = &tsk->vtime;

	اگر (!vtime_delta(vtime))
		واپس;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_انتہا(&vtime->seqcount);
}

خالی vtime_user_enter(ساخت task_struct *tsk)
{
	ساخت vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_انتہا(&vtime->seqcount);
}

خالی vtime_user_exit(ساخت task_struct *tsk)
{
	ساخت vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	اگر (vtime->utime >= TICK_NSEC) {
		account_user_وقت(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_انتہا(&vtime->seqcount);
}

خالی vtime_guest_enter(ساخت task_struct *tsk)
{
	ساخت vtime *vtime = &tsk->vtime;
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
	write_seqcount_انتہا(&vtime->seqcount);
}
EXPORT_علامت_GPL(vtime_guest_enter);

خالی vtime_guest_exit(ساخت task_struct *tsk)
{
	ساخت vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_انتہا(&vtime->seqcount);
}
EXPORT_علامت_GPL(vtime_guest_exit);

ساکن خالی __vtime_account_idle(ساخت vtime *vtime)
{
	account_idle_وقت(get_vtime_delta(vtime));
}

خالی vtime_task_switch_generic(ساخت task_struct *پچھلا)
{
	ساخت vtime *vtime = &پچھلا->vtime;

	write_seqcount_begin(&vtime->seqcount);
	اگر (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	ورنہ
		__vtime_account_kernel(پچھلا, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_انتہا(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	اگر (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	ورنہ اگر (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	ورنہ
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_انتہا(&vtime->seqcount);
}

خالی vtime_init_idle(ساخت task_struct *t, صحیح cpu)
{
	ساخت vtime *vtime = &t->vtime;
	غیرعلامتی طویل flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_انتہا(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(ساخت task_struct *t)
{
	ساخت vtime *vtime = &t->vtime;
	غیرعلامتی صحیح seq;
	u64 gtime;

	اگر (!vtime_accounting_enabled())
		واپس t->gtime;

	کرو {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		اگر (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} جب (read_seqcount_retry(&vtime->seqcount, seq));

	واپس gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(ساخت task_struct *t, u64 *utime, u64 *stime)
{
	ساخت vtime *vtime = &t->vtime;
	غیرعلامتی صحیح seq;
	u64 delta;
	صحیح ret;

	اگر (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		واپس false;
	}

	کرو {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		اگر (vtime->state < VTIME_SYS)
			جاری;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		اگر (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		ورنہ
			*utime += vtime->utime + delta;
	} جب (read_seqcount_retry(&vtime->seqcount, seq));

	واپس ret;
}

ساکن صحیح vtime_state_fetch(ساخت vtime *vtime, صحیح cpu)
{
	صحیح state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	اگر (vtime->cpu != cpu && vtime->cpu != -1)
		واپس -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	اگر (state == VTIME_INACTIVE)
		واپس -EAGAIN;

	واپس state;
}

ساکن u64 kcpustat_user_vtime(ساخت vtime *vtime)
{
	اگر (vtime->state == VTIME_USER)
		واپس vtime->utime + vtime_delta(vtime);
	ورنہ اگر (vtime->state == VTIME_GUEST)
		واپس vtime->gtime + vtime_delta(vtime);
	واپس 0;
}

ساکن صحیح kcpustat_میدان_vtime(u64 *cpustat,
				ساخت task_struct *tsk,
				شمار cpu_usage_stat usage,
				صحیح cpu, u64 *val)
{
	ساخت vtime *vtime = &tsk->vtime;
	غیرعلامتی صحیح seq;

	کرو {
		صحیح state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		اگر (state < 0)
			واپس state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		انتخاب (usage) {
		حالت CPUTIME_SYSTEM:
			اگر (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			توڑو;
		حالت CPUTIME_USER:
			اگر (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			توڑو;
		حالت CPUTIME_NICE:
			اگر (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			توڑو;
		حالت CPUTIME_GUEST:
			اگر (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			توڑو;
		حالت CPUTIME_GUEST_NICE:
			اگر (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			توڑو;
		حالت CPUTIME_IDLE:
			اگر (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			توڑو;
		حالت CPUTIME_IOWAIT:
			اگر (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			توڑو;
		طےشدہ:
			توڑو;
		}
	} جب (read_seqcount_retry(&vtime->seqcount, seq));

	واپس 0;
}

u64 kcpustat_میدان(شمار cpu_usage_stat usage, صحیح cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	ساخت rq *rq;
	صحیح err;

	اگر (!vtime_generic_enabled_cpu(cpu))
		واپس kcpustat_میدان_default(usage, cpu);

	rq = cpu_rq(cpu);

	برائے (;;) {
		ساخت task_struct *curr;

		rcu_read_تالا();
		curr = rcu_dereference(rq->curr);
		اگر (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			واپس cpustat[usage];
		}

		err = kcpustat_میدان_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		اگر (!err)
			واپس val;

		cpu_relax();
	}
}
EXPORT_علامت_GPL(kcpustat_میدان);

ساکن صحیح kcpustat_cpu_fetch_vtime(ساخت kernel_cpustat *dst,
				    ثابت ساخت kernel_cpustat *src,
				    ساخت task_struct *tsk, صحیح cpu)
{
	ساخت vtime *vtime = &tsk->vtime;
	غیرعلامتی صحیح seq;

	کرو {
		u64 *cpustat;
		u64 delta;
		صحیح state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		اگر (state < 0)
			واپس state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		اگر (state < VTIME_IDLE)
			جاری;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		انتخاب (state) {
		حالت VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			توڑو;
		حالت VTIME_USER:
			اگر (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			ورنہ
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			توڑو;
		حالت VTIME_GUEST:
			اگر (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} ورنہ {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			توڑو;
		حالت VTIME_IDLE:
			اگر (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			ورنہ
				cpustat[CPUTIME_IDLE] += delta;
			توڑو;
		طےشدہ:
			WARN_ON_ONCE(1);
		}
	} جب (read_seqcount_retry(&vtime->seqcount, seq));

	واپس 0;
}

خالی kcpustat_cpu_fetch(ساخت kernel_cpustat *dst, صحیح cpu)
{
	ثابت ساخت kernel_cpustat *src = &kcpustat_cpu(cpu);
	ساخت rq *rq;
	صحیح err;

	اگر (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		واپس;
	}

	rq = cpu_rq(cpu);

	برائے (;;) {
		ساخت task_struct *curr;

		rcu_read_تالا();
		curr = rcu_dereference(rq->curr);
		اگر (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			واپس;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		اگر (!err)
			واپس;

		cpu_relax();
	}
}
EXPORT_علامت_GPL(kcpustat_cpu_fetch);

#اگر_ختم /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
