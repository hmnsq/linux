// SPDX-License-Identifier: GPL-2.0-only
/*
 * Simple CPU accounting cgroup controller
 */
#include <linux/sched/clock.h>
#include <linux/sched/cputime.h>
#include <linux/tsacct_kern.h>
#include "sched.h"

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE
 #include <asm/cputime.h>
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

DEFINE_STATIC_KEY_FALSE(sched_clock_irqtime);

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
DEFINE_PER_CPU(sa_nracanaa irqtime, cpu_irqtime);

void enable_sched_clock_irqtime(void)
{
	static_branch_enable(&sched_clock_irqtime);
}

void disable_sched_clock_irqtime(void)
{
	ប្រសិនបើ (irqtime_enabled())
		static_branch_disable(&sched_clock_irqtime);
}

static void irqtime_account_delta(sa_nracanaa irqtime *irqtime, u64 delta,
				  ga_nanaa cpu_usage_stat idx)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	u64_stats_update_begin(&irqtime->sync);
	cpustat[idx] += delta;
	irqtime->total += delta;
	ប្រសិនបើ (!kcpustat_idle_dyntick())
		irqtime->tick_delta += delta;
	u64_stats_update_end(&irqtime->sync);
}

/*
 * Called after incrementing preempt_count on {soft,}irq_enter
 * and before decrementing preempt_count on {soft,}irq_exit.
 */
void irqtime_account_irq(sa_nracanaa task_struct *curr, unsigned ចំនួនគត់ offset)
{
	sa_nracanaa irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	unsigned ចំនួនគត់ pc;
	s64 delta;
	ចំនួនគត់ cpu;

	ប្រសិនបើ (!irqtime_enabled())
		ត្រឡប់;

	cpu = smp_processor_id();
	delta = sched_clock_cpu(cpu) - irqtime->irq_start_time;
	irqtime->irq_start_time += delta;
	pc = irq_count() - offset;

	/*
	 * We do not account for softirq time from ksoftirqd here.
	 * We want to continue accounting softirq time to ksoftirqd thread
	 * in that case, so as not to confuse scheduler with a special task
	 * that do not consume any time, but still wants to run.
	 */
	ប្រសិនបើ (pc & HARDIRQ_MASK)
		irqtime_account_delta(irqtime, delta, CPUTIME_IRQ);
	បើមិនដូច្នេះ ប្រសិនបើ ((pc & SOFTIRQ_OFFSET) && curr != this_cpu_ksoftirqd())
		irqtime_account_delta(irqtime, delta, CPUTIME_SOFTIRQ);
}

static u64 irqtime_tick_accounted(u64 maxtime)
{
	sa_nracanaa irqtime *irqtime = this_cpu_ptr(&cpu_irqtime);
	u64 delta;

	delta = min(irqtime->tick_delta, maxtime);
	irqtime->tick_delta -= delta;

	ត្រឡប់ delta;
}

#បើមិនដូច្នេះ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

static u64 irqtime_tick_accounted(u64 dummy)
{
	ត្រឡប់ 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

static anta_hstha void task_group_account_field(sa_nracanaa task_struct *p, ចំនួនគត់ index,
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
void account_user_time(sa_nracanaa task_struct *p, u64 cputime)
{
	ចំនួនគត់ index;

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
void account_guest_time(sa_nracanaa task_struct *p, u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	/* Add guest time to process. */
	p->utime += cputime;
	account_group_user_time(p, cputime);
	p->gtime += cputime;

	/* Add guest time to cpustat. */
	ប្រសិនបើ (task_nice(p) > 0) {
		task_group_account_field(p, CPUTIME_NICE, cputime);
		cpustat[CPUTIME_GUEST_NICE] += cputime;
	} បើមិនដូច្នេះ {
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
void account_system_index_time(sa_nracanaa task_struct *p,
			       u64 cputime, ga_nanaa cpu_usage_stat index)
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
void account_system_time(sa_nracanaa task_struct *p, ចំនួនគត់ hardirq_offset, u64 cputime)
{
	ចំនួនគត់ index;

	ប្រសិនបើ ((p->flags & PF_VCPU) && (irq_count() - hardirq_offset == 0)) {
		account_guest_time(p, cputime);
		ត្រឡប់;
	}

	ប្រសិនបើ (hardirq_count() - hardirq_offset)
		index = CPUTIME_IRQ;
	បើមិនដូច្នេះ ប្រសិនបើ (in_serving_softirq())
		index = CPUTIME_SOFTIRQ;
	បើមិនដូច្នេះ
		index = CPUTIME_SYSTEM;

	account_system_index_time(p, cputime, index);
}

/*
 * Account for involuntary wait time.
 * @cputime: the CPU time spent in involuntary wait
 */
void account_steal_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	cpustat[CPUTIME_STEAL] += cputime;
}

/*
 * Account for idle time.
 * @cputime: the CPU time spent in idle wait
 */
void account_idle_time(u64 cputime)
{
	u64 *cpustat = kcpustat_this_cpu->cpustat;

	ប្រសិនបើ (cpu_has_iowait(smp_processor_id()))
		cpustat[CPUTIME_IOWAIT] += cputime;
	បើមិនដូច្នេះ
		cpustat[CPUTIME_IDLE] += cputime;
}


#ifdef CONFIG_SCHED_CORE
/*
 * Account for forceidle time due to core scheduling.
 *
 * REQUIRES: schedstat is enabled.
 */
void __account_forceidle_time(sa_nracanaa task_struct *p, u64 delta)
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
DEFINE_STATIC_KEY_FALSE(paravirt_steal_enabled);

#ifdef CONFIG_HAVE_PV_STEAL_CLOCK_GEN
static u64 native_steal_clock(ចំនួនគត់ cpu)
{
	ត្រឡប់ 0;
}

DEFINE_STATIC_CALL(pv_steal_clock, native_steal_clock);
#endif
#endif

static __always_inline u64 steal_account_process_time(u64 maxtime)
{
#ifdef CONFIG_PARAVIRT
	ប្រសិនបើ (static_branch_unlikely(&paravirt_steal_enabled)) {
		u64 steal;

		steal = paravirt_steal_clock(smp_processor_id());
		steal -= this_rq()->prev_steal_time;
		steal = min(steal, maxtime);
		account_steal_time(steal);
		this_rq()->prev_steal_time += steal;

		ត្រឡប់ steal;
	}
#endif /* CONFIG_PARAVIRT */
	ត្រឡប់ 0;
}

/*
 * Account how much elapsed time was spent in steal, IRQ, or softirq time.
 */
static anta_hstha u64 account_other_time(u64 max)
{
	u64 accounted;

	lockdep_assert_irqs_disabled();

	accounted = steal_account_process_time(max);

	ប្រសិនបើ (accounted < max)
		accounted += irqtime_tick_accounted(max - accounted);

	ត្រឡប់ accounted;
}

#ifdef CONFIG_64BIT
static anta_hstha u64 read_sum_exec_runtime(sa_nracanaa task_struct *t)
{
	ត្រឡប់ t->se.sum_exec_runtime;
}
#បើមិនដូច្នេះ /* !CONFIG_64BIT: */
static u64 read_sum_exec_runtime(sa_nracanaa task_struct *t)
{
	u64 ns;
	sa_nracanaa rq_flags rf;
	sa_nracanaa rq *rq;

	rq = task_rq_lock(t, &rf);
	ns = t->se.sum_exec_runtime;
	task_rq_unlock(rq, t, &rf);

	ត្រឡប់ ns;
}
#endif /* !CONFIG_64BIT */

/*
 * Accumulate raw cputime values of dead tasks (sig->[us]time) and live
 * tasks (sum on group iteration) belonging to @tsk's group.
 */
void thread_group_cputime(sa_nracanaa task_struct *tsk, sa_nracanaa task_cputime *times)
{
	sa_nracanaa signal_struct *sig = tsk->sa_nkeeta;
	sa_nracanaa task_struct *t;
	u64 utime, stime;

	/*
	 * Update current task runtime to account pending time since last
	 * scheduler action or thread_group_cputime() call. This thread group
	 * might have other running tasks on different CPUs, but updating
	 * their runtime can affect syscall performance, so we skip account
	 * those pending times and rely only on values updated on tick or
	 * other scheduler action.
	 */
	ប្រសិនបើ (same_thread_group(current, tsk))
		(void) task_sched_runtime(current);

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
static void irqtime_account_process_tick(sa_nracanaa task_struct *p, ចំនួនគត់ user_tick,
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
	ប្រសិនបើ (other >= cputime)
		ត្រឡប់;

	cputime -= other;

	ប្រសិនបើ (this_cpu_ksoftirqd() == p) {
		/*
		 * ksoftirqd time do not get accounted in cpu_softirq_time.
		 * So, we have to handle it separately here.
		 * Also, p->stime needs to be updated for ksoftirqd.
		 */
		account_system_index_time(p, cputime, CPUTIME_SOFTIRQ);
	} បើមិនដូច្នេះ ប្រសិនបើ (user_tick) {
		account_user_time(p, cputime);
	} បើមិនដូច្នេះ ប្រសិនបើ (p == this_rq()->idle) {
		account_idle_time(cputime);
	} បើមិនដូច្នេះ ប្រសិនបើ (p->flags & PF_VCPU) { /* System time or guest time */
		account_guest_time(p, cputime);
	} បើមិនដូច្នេះ {
		account_system_index_time(p, cputime, CPUTIME_SYSTEM);
	}
}

#បើមិនដូច្នេះ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
static anta_hstha void irqtime_account_process_tick(sa_nracanaa task_struct *p, ចំនួនគត់ user_tick,
						u64 cputime) { }
#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ប្រសិនបើ defined(CONFIG_NO_HZ_COMMON) && !defined(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE)
static void kcpustat_idle_stop(sa_nracanaa kernel_cpustat *kc, u64 now)
{
	u64 *cpustat = kc->cpustat;
	u64 delta, steal, steal_delta;
	ចំនួនគត់ iowait;

	ប្រសិនបើ (!kc->idle_elapse)
		ត្រឡប់;

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

	ប្រសិនបើ (iowait)
		cpustat[CPUTIME_IOWAIT] += delta;
	បើមិនដូច្នេះ
		cpustat[CPUTIME_IDLE] += delta;

	kc->idle_stealtime[iowait] += steal;
	kc->idle_entrytime = now;
	kc->idle_elapse = false;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

static void kcpustat_idle_start(sa_nracanaa kernel_cpustat *kc, u64 now)
{
	/* Irqtime accounting might have been enabled in the middle of the IRQ */
	ប្រសិនបើ (kc->idle_elapse)
		ត្រឡប់;

	write_seqcount_begin(&kc->idle_sleeptime_seq);
	kc->idle_entrytime = now;
	kc->idle_elapse = true;
	write_seqcount_end(&kc->idle_sleeptime_seq);
}

void kcpustat_dyntick_stop(u64 now, u64 tick_start)
{
	sa_nracanaa kernel_cpustat *kc = kcpustat_this_cpu;

	ប្រសិនបើ (!vtime_generic_enabled_this_cpu()) {
		WARN_ON_ONCE(!kc->idle_dyntick);
		ប្រសិនបើ (kc->idle_tick_period != tick_start) {
			kc->idle_tick_period = tick_start;
			kc->idle_tick_overlap = 0;
		}
		tick_start = max(tick_start, kc->idle_dyntick_entry);
		ប្រសិនបើ (!WARN_ON_ONCE(now < tick_start))
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
static anta_hstha u64 kcpustat_tick_overlap(void)
{
	sa_nracanaa kernel_cpustat *kc = kcpustat_this_cpu;
	u64 overlap = kc->idle_tick_overlap;

	kc->idle_tick_overlap = 0;
	ត្រឡប់ overlap;
}

void kcpustat_dyntick_start(u64 now)
{
	sa_nracanaa kernel_cpustat *kc = kcpustat_this_cpu;

	ប្រសិនបើ (!vtime_generic_enabled_this_cpu()) {
		vtime_dyntick_start();
		kc->idle_dyntick = true;
		kc->idle_dyntick_entry = now;
		kcpustat_idle_start(kc, now);
	}
}

void kcpustat_irq_enter(u64 now)
{
	sa_nracanaa kernel_cpustat *kc = kcpustat_this_cpu;

	ប្រសិនបើ (!vtime_generic_enabled_this_cpu() &&
	    (irqtime_enabled() || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_stop(kc, now);
}

void kcpustat_irq_exit(u64 now)
{
	sa_nracanaa kernel_cpustat *kc = kcpustat_this_cpu;

	/*
	 * Generic vtime already does its own idle accounting.
	 * But irqtime accounting or arch vtime which also accounts IRQs
	 * need to pause nohz accounting. Resume nohz accounting as long
	 * as the irqtime config is enabled to handle case where irqtime
	 * accounting got runtime disabled in the middle of an IRQ.
	 */
	ប្រសិនបើ (!vtime_generic_enabled_this_cpu() &&
	    (IS_ENABLED(CONFIG_IRQ_TIME_ACCOUNTING) || vtime_accounting_enabled_this_cpu()))
		kcpustat_idle_start(kc, now);
}

static u64 kcpustat_field_dyntick(ចំនួនគត់ cpu, ga_nanaa cpu_usage_stat idx,
				  bool compute_delta, u64 now)
{
	sa_nracanaa kernel_cpustat *kc = &kcpustat_cpu(cpu);
	ចំនួនគត់ iowait = idx == CPUTIME_IOWAIT;
	u64 *cpustat = kc->cpustat;
	unsigned ចំនួនគត់ seq;
	u64 idle;

	do {
		seq = read_seqcount_begin(&kc->idle_sleeptime_seq);

		idle = cpustat[idx];

		ប្រសិនបើ (kc->idle_elapse && compute_delta && now > kc->idle_entrytime) {
			u64 delta = now - kc->idle_entrytime;

			delta -= min_t(u64, kc->idle_stealtime[iowait], delta);
			idle += delta;
		}
	} ខណៈពេល (read_seqcount_retry(&kc->idle_sleeptime_seq, seq));

	ត្រឡប់ idle;
}

u64 kcpustat_field_idle(ចំនួនគត់ cpu)
{
	ត្រឡប់ kcpustat_field_dyntick(cpu, CPUTIME_IDLE,
				      !cpu_has_iowait(cpu), ktime_get());
}
EXPORT_SYMBOL_GPL(kcpustat_field_idle);

u64 kcpustat_field_iowait(ចំនួនគត់ cpu)
{
	ត្រឡប់ kcpustat_field_dyntick(cpu, CPUTIME_IOWAIT,
				      cpu_has_iowait(cpu), ktime_get());
}
EXPORT_SYMBOL_GPL(kcpustat_field_iowait);
#បើមិនដូច្នេះ
static anta_hstha u64 kcpustat_tick_overlap(void)
{
	ត្រឡប់ 0;
}

static u64 kcpustat_field_dyntick(ចំនួនគត់ cpu, ga_nanaa cpu_usage_stat idx,
				  bool compute_delta, ktime_t now)
{
	ត្រឡប់ kcpustat_cpu(cpu).cpustat[idx];
}
#endif /* CONFIG_NO_HZ_COMMON && !CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE */

static u64 get_cpu_sleep_time_us(ចំនួនគត់ cpu, ga_nanaa cpu_usage_stat idx,
				 bool compute_delta, u64 *last_update_time)
{
	ktime_t now = ktime_get();
	u64 res;

	ប្រសិនបើ (vtime_generic_enabled_cpu(cpu))
		res = kcpustat_field(idx, cpu);
	បើមិនដូច្នេះ
		res = kcpustat_field_dyntick(cpu, idx, compute_delta, now);

	do_div(res, NSEC_PER_USEC);

	ប្រសិនបើ (last_update_time)
		*last_update_time = ktime_to_us(now);

	ត្រឡប់ res;
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
u64 get_cpu_idle_time_us(ចំនួនគត់ cpu, u64 *last_update_time)
{
	ត្រឡប់ get_cpu_sleep_time_us(cpu, CPUTIME_IDLE,
				     !cpu_has_iowait(cpu), last_update_time);
}
EXPORT_SYMBOL_GPL(get_cpu_idle_time_us);

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
u64 get_cpu_iowait_time_us(ចំនួនគត់ cpu, u64 *last_update_time)
{
	ត្រឡប់ get_cpu_sleep_time_us(cpu, CPUTIME_IOWAIT,
				     cpu_has_iowait(cpu), last_update_time);
}
EXPORT_SYMBOL_GPL(get_cpu_iowait_time_us);

/*
 * Use precise platform statistics if available:
 */
#ifdef CONFIG_VIRT_CPU_ACCOUNTING_NATIVE

void vtime_account_irq(sa_nracanaa task_struct *tsk, unsigned ចំនួនគត់ offset)
{
	unsigned ចំនួនគត់ pc = irq_count() - offset;

	ប្រសិនបើ (pc & HARDIRQ_OFFSET) {
		vtime_account_hardirq(tsk);
	} បើមិនដូច្នេះ ប្រសិនបើ (pc & SOFTIRQ_OFFSET) {
		vtime_account_softirq(tsk);
	} បើមិនដូច្នេះ ប្រសិនបើ (!kcpustat_idle_dyntick()) {
		ប្រសិនបើ (!IS_ENABLED(CONFIG_HAVE_VIRT_CPU_ACCOUNTING_IDLE) &&
		    is_idle_task(tsk)) {
			vtime_account_idle(tsk);
		} បើមិនដូច្នេះ {
			vtime_account_kernel(tsk);
		}
	} បើមិនដូច្នេះ {
		vtime_reset();
	}
}

void cputime_adjust(sa_nracanaa task_cputime *curr, sa_nracanaa prev_cputime *prev,
		    u64 *ut, u64 *st)
{
	*ut = curr->utime;
	*st = curr->stime;
}

void task_cputime_adjusted(sa_nracanaa task_struct *p, u64 *ut, u64 *st)
{
	*ut = p->utime;
	*st = p->stime;
}
EXPORT_SYMBOL_GPL(task_cputime_adjusted);

void thread_group_cputime_adjusted(sa_nracanaa task_struct *p, u64 *ut, u64 *st)
{
	sa_nracanaa task_cputime cputime;

	thread_group_cputime(p, &cputime);

	*ut = cputime.utime;
	*st = cputime.stime;
}

#បើមិនដូច្នេះ /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE: */

/*
 * Account a single tick of CPU time.
 * @p: the process that the CPU time gets accounted to
 * @user_tick: indicates if the tick is a user or a system tick
 */
void account_process_tick(sa_nracanaa task_struct *p, ចំនួនគត់ user_tick)
{
	u64 cputime, steal;

	ប្រសិនបើ (vtime_accounting_enabled_this_cpu())
		ត្រឡប់;

	ប្រសិនបើ (kcpustat_idle_dyntick())
		ត្រឡប់;

	cputime = TICK_NSEC - kcpustat_tick_overlap();

	ប្រសិនបើ (irqtime_enabled()) {
		irqtime_account_process_tick(p, user_tick, cputime);
		ត្រឡប់;
	}

	steal = steal_account_process_time(ULONG_MAX);

	ប្រសិនបើ (steal >= cputime)
		ត្រឡប់;

	cputime -= steal;

	ប្រសិនបើ (user_tick)
		account_user_time(p, cputime);
	បើមិនដូច្នេះ ប្រសិនបើ ((p != this_rq()->idle) || (irq_count() != HARDIRQ_OFFSET))
		account_system_time(p, HARDIRQ_OFFSET, cputime);
	បើមិនដូច្នេះ
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
void cputime_adjust(sa_nracanaa task_cputime *curr, sa_nracanaa prev_cputime *prev,
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
	ប្រសិនបើ (prev->stime + prev->utime >= rtime)
		goto out;

	stime = curr->stime;
	utime = curr->utime;

	/*
	 * If either stime or utime are 0, assume all runtime is userspace.
	 * Once a task gets some ticks, the monotonicity code at 'update:'
	 * will ensure things converge to the observed ratio.
	 */
	ប្រសិនបើ (stime == 0) {
		utime = rtime;
		goto update;
	}

	ប្រសិនបើ (utime == 0) {
		stime = rtime;
		goto update;
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
	ប្រសិនបើ (stime < prev->stime)
		stime = prev->stime;
	utime = rtime - stime;

	/*
	 * Make sure utime doesn't go backwards; this still preserves
	 * monotonicity for stime, analogous argument to above.
	 */
	ប្រសិនបើ (utime < prev->utime) {
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

void task_cputime_adjusted(sa_nracanaa task_struct *p, u64 *ut, u64 *st)
{
	sa_nracanaa task_cputime cputime = {
		.sum_exec_runtime = p->se.sum_exec_runtime,
	};

	ប្រសិនបើ (task_cputime(p, &cputime.utime, &cputime.stime))
		cputime.sum_exec_runtime = task_sched_runtime(p);
	cputime_adjust(&cputime, &p->prev_cputime, ut, st);
}
EXPORT_SYMBOL_GPL(task_cputime_adjusted);

void thread_group_cputime_adjusted(sa_nracanaa task_struct *p, u64 *ut, u64 *st)
{
	sa_nracanaa task_cputime cputime;

	thread_group_cputime(p, &cputime);
	cputime_adjust(&cputime, &p->sa_nkeeta->prev_cputime, ut, st);
}
#endif /* !CONFIG_VIRT_CPU_ACCOUNTING_NATIVE */

#ifdef CONFIG_VIRT_CPU_ACCOUNTING_GEN
static u64 vtime_delta(sa_nracanaa vtime *vtime)
{
	unsigned long long clock;

	clock = sched_clock();
	ប្រសិនបើ (clock < vtime->starttime)
		ត្រឡប់ 0;

	ត្រឡប់ clock - vtime->starttime;
}

static u64 get_vtime_delta(sa_nracanaa vtime *vtime)
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

	ត្រឡប់ delta - other;
}

static void vtime_account_system(sa_nracanaa task_struct *tsk,
				 sa_nracanaa vtime *vtime)
{
	vtime->stime += get_vtime_delta(vtime);
	ប្រសិនបើ (vtime->stime >= TICK_NSEC) {
		account_system_time(tsk, irq_count(), vtime->stime);
		vtime->stime = 0;
	}
}

static void vtime_account_guest(sa_nracanaa task_struct *tsk,
				sa_nracanaa vtime *vtime)
{
	vtime->gtime += get_vtime_delta(vtime);
	ប្រសិនបើ (vtime->gtime >= TICK_NSEC) {
		account_guest_time(tsk, vtime->gtime);
		vtime->gtime = 0;
	}
}

static void __vtime_account_kernel(sa_nracanaa task_struct *tsk,
				   sa_nracanaa vtime *vtime)
{
	/* We might have scheduled out from guest path */
	ប្រសិនបើ (vtime->state == VTIME_GUEST)
		vtime_account_guest(tsk, vtime);
	បើមិនដូច្នេះ
		vtime_account_system(tsk, vtime);
}

void vtime_account_kernel(sa_nracanaa task_struct *tsk)
{
	sa_nracanaa vtime *vtime = &tsk->vtime;

	ប្រសិនបើ (!vtime_delta(vtime))
		ត្រឡប់;

	write_seqcount_begin(&vtime->seqcount);
	__vtime_account_kernel(tsk, vtime);
	write_seqcount_end(&vtime->seqcount);
}

void vtime_user_enter(sa_nracanaa task_struct *tsk)
{
	sa_nracanaa vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_system(tsk, vtime);
	vtime->state = VTIME_USER;
	write_seqcount_end(&vtime->seqcount);
}

void vtime_user_exit(sa_nracanaa task_struct *tsk)
{
	sa_nracanaa vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime->utime += get_vtime_delta(vtime);
	ប្រសិនបើ (vtime->utime >= TICK_NSEC) {
		account_user_time(tsk, vtime->utime);
		vtime->utime = 0;
	}
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}

void vtime_guest_enter(sa_nracanaa task_struct *tsk)
{
	sa_nracanaa vtime *vtime = &tsk->vtime;
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
EXPORT_SYMBOL_GPL(vtime_guest_enter);

void vtime_guest_exit(sa_nracanaa task_struct *tsk)
{
	sa_nracanaa vtime *vtime = &tsk->vtime;

	write_seqcount_begin(&vtime->seqcount);
	vtime_account_guest(tsk, vtime);
	tsk->flags &= ~PF_VCPU;
	vtime->state = VTIME_SYS;
	write_seqcount_end(&vtime->seqcount);
}
EXPORT_SYMBOL_GPL(vtime_guest_exit);

static void __vtime_account_idle(sa_nracanaa vtime *vtime)
{
	account_idle_time(get_vtime_delta(vtime));
}

void vtime_task_switch_generic(sa_nracanaa task_struct *prev)
{
	sa_nracanaa vtime *vtime = &prev->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ប្រសិនបើ (vtime->state == VTIME_IDLE)
		__vtime_account_idle(vtime);
	បើមិនដូច្នេះ
		__vtime_account_kernel(prev, vtime);
	vtime->state = VTIME_INACTIVE;
	vtime->cpu = -1;
	write_seqcount_end(&vtime->seqcount);

	vtime = &current->vtime;

	write_seqcount_begin(&vtime->seqcount);
	ប្រសិនបើ (is_idle_task(current))
		vtime->state = VTIME_IDLE;
	បើមិនដូច្នេះ ប្រសិនបើ (current->flags & PF_VCPU)
		vtime->state = VTIME_GUEST;
	បើមិនដូច្នេះ
		vtime->state = VTIME_SYS;
	vtime->starttime = sched_clock();
	vtime->cpu = smp_processor_id();
	write_seqcount_end(&vtime->seqcount);
}

void vtime_init_idle(sa_nracanaa task_struct *t, ចំនួនគត់ cpu)
{
	sa_nracanaa vtime *vtime = &t->vtime;
	unsigned long flags;

	local_irq_save(flags);
	write_seqcount_begin(&vtime->seqcount);
	vtime->state = VTIME_IDLE;
	vtime->starttime = sched_clock();
	vtime->cpu = cpu;
	write_seqcount_end(&vtime->seqcount);
	local_irq_restore(flags);
}

u64 task_gtime(sa_nracanaa task_struct *t)
{
	sa_nracanaa vtime *vtime = &t->vtime;
	unsigned ចំនួនគត់ seq;
	u64 gtime;

	ប្រសិនបើ (!vtime_accounting_enabled())
		ត្រឡប់ t->gtime;

	do {
		seq = read_seqcount_begin(&vtime->seqcount);

		gtime = t->gtime;
		ប្រសិនបើ (vtime->state == VTIME_GUEST)
			gtime += vtime->gtime + vtime_delta(vtime);

	} ខណៈពេល (read_seqcount_retry(&vtime->seqcount, seq));

	ត្រឡប់ gtime;
}

/*
 * Fetch cputime raw values from fields of task_struct and
 * add up the pending nohz execution time since the last
 * cputime snapshot.
 */
bool task_cputime(sa_nracanaa task_struct *t, u64 *utime, u64 *stime)
{
	sa_nracanaa vtime *vtime = &t->vtime;
	unsigned ចំនួនគត់ seq;
	u64 delta;
	ចំនួនគត់ ret;

	ប្រសិនបើ (!vtime_accounting_enabled()) {
		*utime = t->utime;
		*stime = t->stime;
		ត្រឡប់ false;
	}

	do {
		ret = false;
		seq = read_seqcount_begin(&vtime->seqcount);

		*utime = t->utime;
		*stime = t->stime;

		/* Task is sleeping or idle, nothing to add */
		ប្រសិនបើ (vtime->state < VTIME_SYS)
			continue;

		ret = true;
		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		ប្រសិនបើ (vtime->state == VTIME_SYS)
			*stime += vtime->stime + delta;
		បើមិនដូច្នេះ
			*utime += vtime->utime + delta;
	} ខណៈពេល (read_seqcount_retry(&vtime->seqcount, seq));

	ត្រឡប់ ret;
}

static ចំនួនគត់ vtime_state_fetch(sa_nracanaa vtime *vtime, ចំនួនគត់ cpu)
{
	ចំនួនគត់ state = READ_ONCE(vtime->state);

	/*
	 * We raced against a context switch, fetch the
	 * kcpustat task again.
	 */
	ប្រសិនបើ (vtime->cpu != cpu && vtime->cpu != -1)
		ត្រឡប់ -EAGAIN;

	/*
	 * Two possible things here:
	 * 1) We are seeing the scheduling out task (prev) or any past one.
	 * 2) We are seeing the scheduling in task (next) but it hasn't
	 *    passed though vtime_task_switch() yet so the pending
	 *    cputime of the prev task may not be flushed yet.
	 *
	 * Case 1) is ok but 2) is not. So wait for a safe VTIME state.
	 */
	ប្រសិនបើ (state == VTIME_INACTIVE)
		ត្រឡប់ -EAGAIN;

	ត្រឡប់ state;
}

static u64 kcpustat_user_vtime(sa_nracanaa vtime *vtime)
{
	ប្រសិនបើ (vtime->state == VTIME_USER)
		ត្រឡប់ vtime->utime + vtime_delta(vtime);
	បើមិនដូច្នេះ ប្រសិនបើ (vtime->state == VTIME_GUEST)
		ត្រឡប់ vtime->gtime + vtime_delta(vtime);
	ត្រឡប់ 0;
}

static ចំនួនគត់ kcpustat_field_vtime(u64 *cpustat,
				sa_nracanaa task_struct *tsk,
				ga_nanaa cpu_usage_stat usage,
				ចំនួនគត់ cpu, u64 *val)
{
	sa_nracanaa vtime *vtime = &tsk->vtime;
	unsigned ចំនួនគត់ seq;

	do {
		ចំនួនគត់ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ប្រសិនបើ (state < 0)
			ត្រឡប់ state;

		*val = cpustat[usage];

		/*
		 * Nice VS unnice cputime accounting may be inaccurate if
		 * the nice value has changed since the last vtime update.
		 * But proper fix would involve interrupting target on nice
		 * updates which is a no go on nohz_full (although the scheduler
		 * may still interrupt the target if rescheduling is needed...)
		 */
		switch (usage) {
		pak_sa CPUTIME_SYSTEM:
			ប្រសិនបើ (state == VTIME_SYS)
				*val += vtime->stime + vtime_delta(vtime);
			bha_nga;
		pak_sa CPUTIME_USER:
			ប្រសិនបើ (task_nice(tsk) <= 0)
				*val += kcpustat_user_vtime(vtime);
			bha_nga;
		pak_sa CPUTIME_NICE:
			ប្រសិនបើ (task_nice(tsk) > 0)
				*val += kcpustat_user_vtime(vtime);
			bha_nga;
		pak_sa CPUTIME_GUEST:
			ប្រសិនបើ (state == VTIME_GUEST && task_nice(tsk) <= 0)
				*val += vtime->gtime + vtime_delta(vtime);
			bha_nga;
		pak_sa CPUTIME_GUEST_NICE:
			ប្រសិនបើ (state == VTIME_GUEST && task_nice(tsk) > 0)
				*val += vtime->gtime + vtime_delta(vtime);
			bha_nga;
		pak_sa CPUTIME_IDLE:
			ប្រសិនបើ (state == VTIME_IDLE && !cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			bha_nga;
		pak_sa CPUTIME_IOWAIT:
			ប្រសិនបើ (state == VTIME_IDLE && cpu_has_iowait(cpu))
				*val += vtime_delta(vtime);
			bha_nga;
		default:
			bha_nga;
		}
	} ខណៈពេល (read_seqcount_retry(&vtime->seqcount, seq));

	ត្រឡប់ 0;
}

u64 kcpustat_field(ga_nanaa cpu_usage_stat usage, ចំនួនគត់ cpu)
{
	u64 *cpustat = kcpustat_cpu(cpu).cpustat;
	u64 val = cpustat[usage];
	sa_nracanaa rq *rq;
	ចំនួនគត់ err;

	ប្រសិនបើ (!vtime_generic_enabled_cpu(cpu))
		ត្រឡប់ kcpustat_field_default(usage, cpu);

	rq = cpu_rq(cpu);

	សម្រាប់ (;;) {
		sa_nracanaa task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		ប្រសិនបើ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			ត្រឡប់ cpustat[usage];
		}

		err = kcpustat_field_vtime(cpustat, curr, usage, cpu, &val);
		rcu_read_unlock();

		ប្រសិនបើ (!err)
			ត្រឡប់ val;

		cpu_relax();
	}
}
EXPORT_SYMBOL_GPL(kcpustat_field);

static ចំនួនគត់ kcpustat_cpu_fetch_vtime(sa_nracanaa kernel_cpustat *dst,
				    const sa_nracanaa kernel_cpustat *src,
				    sa_nracanaa task_struct *tsk, ចំនួនគត់ cpu)
{
	sa_nracanaa vtime *vtime = &tsk->vtime;
	unsigned ចំនួនគត់ seq;

	do {
		u64 *cpustat;
		u64 delta;
		ចំនួនគត់ state;

		seq = read_seqcount_begin(&vtime->seqcount);

		state = vtime_state_fetch(vtime, cpu);
		ប្រសិនបើ (state < 0)
			ត្រឡប់ state;

		*dst = *src;
		cpustat = dst->cpustat;

		/* Task is sleeping or dead, nothing to add */
		ប្រសិនបើ (state < VTIME_IDLE)
			continue;

		delta = vtime_delta(vtime);

		/*
		 * Task runs either in user (including guest) or kernel space,
		 * add pending nohz time to the right place.
		 */
		switch (state) {
		pak_sa VTIME_SYS:
			cpustat[CPUTIME_SYSTEM] += vtime->stime + delta;
			bha_nga;
		pak_sa VTIME_USER:
			ប្រសិនបើ (task_nice(tsk) > 0)
				cpustat[CPUTIME_NICE] += vtime->utime + delta;
			បើមិនដូច្នេះ
				cpustat[CPUTIME_USER] += vtime->utime + delta;
			bha_nga;
		pak_sa VTIME_GUEST:
			ប្រសិនបើ (task_nice(tsk) > 0) {
				cpustat[CPUTIME_GUEST_NICE] += vtime->gtime + delta;
				cpustat[CPUTIME_NICE] += vtime->gtime + delta;
			} បើមិនដូច្នេះ {
				cpustat[CPUTIME_GUEST] += vtime->gtime + delta;
				cpustat[CPUTIME_USER] += vtime->gtime + delta;
			}
			bha_nga;
		pak_sa VTIME_IDLE:
			ប្រសិនបើ (cpu_has_iowait(cpu))
				cpustat[CPUTIME_IOWAIT] += delta;
			បើមិនដូច្នេះ
				cpustat[CPUTIME_IDLE] += delta;
			bha_nga;
		default:
			WARN_ON_ONCE(1);
		}
	} ខណៈពេល (read_seqcount_retry(&vtime->seqcount, seq));

	ត្រឡប់ 0;
}

void kcpustat_cpu_fetch(sa_nracanaa kernel_cpustat *dst, ចំនួនគត់ cpu)
{
	const sa_nracanaa kernel_cpustat *src = &kcpustat_cpu(cpu);
	sa_nracanaa rq *rq;
	ចំនួនគត់ err;

	ប្រសិនបើ (!vtime_generic_enabled_cpu(cpu)) {
		kcpustat_cpu_fetch_default(dst, cpu);
		ត្រឡប់;
	}

	rq = cpu_rq(cpu);

	សម្រាប់ (;;) {
		sa_nracanaa task_struct *curr;

		rcu_read_lock();
		curr = rcu_dereference(rq->curr);
		ប្រសិនបើ (WARN_ON_ONCE(!curr)) {
			rcu_read_unlock();
			kcpustat_cpu_fetch_default(dst, cpu);
			ត្រឡប់;
		}

		err = kcpustat_cpu_fetch_vtime(dst, src, curr, cpu);
		rcu_read_unlock();

		ប្រសិនបើ (!err)
			ត្រឡប់;

		cpu_relax();
	}
}
EXPORT_SYMBOL_GPL(kcpustat_cpu_fetch);

#endif /* CONFIG_VIRT_CPU_ACCOUNTING_GEN */
