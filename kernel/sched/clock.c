// SPDX-License-Identifier: GPL-2.0-only
/*
 * sched_clock() for unstable CPU clocks
 *
 *  Copyright (C) 2008 Red Hat, Inc., Peter Zijlstra
 *
 *  Updates and enhancements:
 *    Copyright (C) 2008 Red Hat, Inc. Steven Rostedt <srostedt@redhat.com>
 *
 * Based on code by:
 *   Ingo Molnar <mingo@redhat.com>
 *   Guillaume Chazarain <guichaz@gmail.com>
 *
 *
 * What this file implements:
 *
 * cpu_clock(i) provides a fast (execution time) high resolution
 * clock with bounded drift between CPUs. The value of cpu_clock(i)
 * is monotonic for constant i. The timestamp returned is in nanoseconds.
 *
 * ######################### BIG FAT WARNING ##########################
 * # when comparing cpu_clock(i) to cpu_clock(j) for i != j, time can #
 * # go backwards !!                                                  #
 * ####################################################################
 *
 * There is no strict promise about the base, although it tends to start
 * at 0 on boot (but people really shouldn't rely on that).
 *
 * cpu_clock(i)       -- can be used from any context, including NMI.
 * local_clock()      -- is cpu_clock() on the current CPU.
 *
 * sched_clock_cpu(i)
 *
 * How it is implemented:
 *
 * The implementation either uses sched_clock() when
 * !CONFIG_HAVE_UNSTABLE_SCHED_CLOCK, which means in that case the
 * sched_clock() is assumed to provide these properties (mostly it means
 * the architecture provides a globally synchronized highres time source).
 *
 * Otherwise it tries to create a semi stable clock from a mixture of other
 * clocks, including:
 *
 *  - GTOD (clock monotonic)
 *  - sched_clock()
 *  - explicit idle events
 *
 * We use GTOD as base and use sched_clock() deltas to improve resolution. The
 * deltas are filtered to provide monotonicity and keeping it within an
 * expected window.
 *
 * Furthermore, explicit sleep and wakeup hooks allow us to account for time
 * that is otherwise invisible (TSC gets stopped).
 *
 */

#ضمن <linux/sched/clock.h>
#ضمن "sched.h"

/*
 * Scheduler clock - returns current time in nanosec units.
 * This is default implementation.
 * Architectures and sub-architectures can override this.
 */
notrace unsigned long long __weak sched_clock(فراغ)
{
	أرجع (unsigned long long)(jiffies - INITIAL_JIFFIES)
					* (NSEC_PER_SEC / HZ);
}
EXPORT_علام_GPL(sched_clock);

ساكن DEFINE_STATIC_مبودي_FALSE(sched_clock_running);

#ifdef CONFIG_HAVE_UNSTABLE_SCHED_CLOCK
/*
 * We must start with !__sched_clock_stable because the unstable -> stable
 * transition is accurate, while the stable -> unstable transition is not.
 *
 * Similarly we start with __sched_clock_stable_early, thereby assuming we
 * will become stable, such that there's only a single 1 -> 0 transition.
 */
ساكن DEFINE_STATIC_مبودي_FALSE(__sched_clock_stable);
ساكن صحيح __sched_clock_stable_early = 1;

/*
 * We want: ktime_get_ns() + __gtod_offset == sched_clock() + __sched_clock_offset
 */
__read_mostly u64 __sched_clock_offset;
ساكن __read_mostly u64 __gtod_offset;

بنية sched_clock_data {
	u64			tick_raw;
	u64			tick_gtod;
	u64			clock;
};

ساكن DEFINE_PER_CPU_SHARED_ALIGNED(بنية sched_clock_data, sched_clock_data);

ساكن __always_inline بنية sched_clock_data *this_scd(فراغ)
{
	أرجع this_cpu_ptr(&sched_clock_data);
}

notrace ساكن مضمن بنية sched_clock_data *cpu_sdc(صحيح cpu)
{
	أرجع &per_cpu(sched_clock_data, cpu);
}

notrace صحيح sched_clock_stable(فراغ)
{
	أرجع static_branch_likely(&__sched_clock_stable);
}

notrace ساكن فراغ __scd_stamp(بنية sched_clock_data *scd)
{
	scd->tick_gtod = ktime_get_ns();
	scd->tick_raw = sched_clock();
}

notrace ساكن فراغ __set_sched_clock_stable(فراغ)
{
	بنية sched_clock_data *scd;

	/*
	 * Since we're still unstable and the tick is already running, we have
	 * to disable IRQs in order to get a consistent scd->tick* reading.
	 */
	local_irq_disable();
	scd = this_scd();
	/*
	 * Attempt to make the (initial) unstable->stable transition continuous.
	 */
	__sched_clock_offset = (scd->tick_gtod + __gtod_offset) - (scd->tick_raw);
	local_irq_enable();

	printk(KERN_INFO "sched_clock: Marking stable (%lld, %lld)->(%lld, %lld)\n",
			scd->tick_gtod, __gtod_offset,
			scd->tick_raw,  __sched_clock_offset);

	static_branch_enable(&__sched_clock_stable);
	tick_dep_clear(TICK_DEP_BIT_CLOCK_UNSTABLE);
}

/*
 * If we ever get here, we're screwed, because we found out -- typically after
 * the fact -- that TSC wasn't good. This means all our clocksources (including
 * ktime) could have reported wrong values.
 *
 * What we do here is an attempt to fix up and continue sort of where we left
 * off in a coherent manner.
 *
 * The only way to fully avoid random clock jumps is to boot with:
 * "tsc=unstable".
 */
notrace ساكن فراغ __sched_clock_work(بنية work_struct *work)
{
	بنية sched_clock_data *scd;
	صحيح cpu;

	/* take a current timestamp and set 'now' */
	preempt_disable();
	scd = this_scd();
	__scd_stamp(scd);
	scd->clock = scd->tick_gtod + __gtod_offset;
	preempt_enable();

	/* clone to all CPUs */
	for_each_possible_cpu(cpu)
		per_cpu(sched_clock_data, cpu) = *scd;

	printk(KERN_WARNING "TSC found unstable after boot, most likely due to broken BIOS. Use 'tsc=unstable'.\n");
	printk(KERN_INFO "sched_clock: Marking unstable (%lld, %lld)<-(%lld, %lld)\n",
			scd->tick_gtod, __gtod_offset,
			scd->tick_raw,  __sched_clock_offset);

	disable_sched_clock_irqtime();
	static_branch_disable(&__sched_clock_stable);
}

ساكن DECLARE_WORK(sched_clock_work, __sched_clock_work);

notrace ساكن فراغ __clear_sched_clock_stable(فراغ)
{
	إذا (!sched_clock_stable())
		أرجع;

	tick_dep_set(TICK_DEP_BIT_CLOCK_UNSTABLE);
	schedule_work(&sched_clock_work);
}

notrace فراغ clear_sched_clock_stable(فراغ)
{
	__sched_clock_stable_early = 0;

	smp_mb(); /* matches sched_clock_init_late() */

	إذا (static_مبودي_كدي(&sched_clock_running.مبودي) == 2)
		__clear_sched_clock_stable();
}

notrace ساكن فراغ __sched_clock_gtod_offset(فراغ)
{
	بنية sched_clock_data *scd = this_scd();

	__scd_stamp(scd);
	__gtod_offset = (scd->tick_raw + __sched_clock_offset) - scd->tick_gtod;
}

فراغ __init sched_clock_init(فراغ)
{
	/*
	 * Set __gtod_offset such that once we mark sched_clock_running,
	 * sched_clock_tick() continues where sched_clock() left off.
	 *
	 * Even if TSC is buggered, we're still UP at this point so it
	 * can't really be out of sync.
	 */
	local_irq_disable();
	__sched_clock_gtod_offset();
	local_irq_enable();

	static_branch_inc(&sched_clock_running);
}
/*
 * We run this as late_initcall() such that it runs after all built-in drivers,
 * notably: acpi_processor and intel_idle, which can mark the TSC as unstable.
 */
ساكن صحيح __init sched_clock_init_late(فراغ)
{
	static_branch_inc(&sched_clock_running);
	/*
	 * Ensure that it is impossible to not do a static_key update.
	 *
	 * Either {set,clear}_sched_clock_stable() must see sched_clock_running
	 * and do the update, or we must see their __sched_clock_stable_early
	 * and do the update, or both.
	 */
	smp_mb(); /* matches {set,clear}_sched_clock_stable() */

	إذا (__sched_clock_stable_early)
		__set_sched_clock_stable();
	وإلا
		disable_sched_clock_irqtime();  /* disable if clock unstable. */

	أرجع 0;
}
late_initcall(sched_clock_init_late);

/*
 * min, max except they take wrapping into account
 */

ساكن __always_inline u64 wrap_min(u64 x, u64 y)
{
	أرجع (s64)(x - y) < 0 ? x : y;
}

ساكن __always_inline u64 wrap_max(u64 x, u64 y)
{
	أرجع (s64)(x - y) > 0 ? x : y;
}

/*
 * update the percpu scd from the raw @now value
 *
 *  - filter out backward motion
 *  - use the GTOD tick value to create a window to filter crazy TSC values
 */
ساكن __always_inline u64 sched_clock_local(بنية sched_clock_data *scd)
{
	u64 now, clock, old_clock, min_clock, max_clock, gtod;
	s64 delta;

again:
	now = sched_clock_noinstr();
	delta = now - scd->tick_raw;
	إذا (unlikely(delta < 0))
		delta = 0;

	old_clock = scd->clock;

	/*
	 * scd->clock = clamp(scd->tick_gtod + delta,
	 *		      max(scd->tick_gtod, scd->clock),
	 *		      scd->tick_gtod + TICK_NSEC);
	 */

	gtod = scd->tick_gtod + __gtod_offset;
	clock = gtod + delta;
	min_clock = wrap_max(gtod, old_clock);
	max_clock = wrap_max(old_clock, gtod + TICK_NSEC);

	clock = wrap_max(clock, min_clock);
	clock = wrap_min(clock, max_clock);

	إذا (!raw_try_cmpxchg64(&scd->clock, &old_clock, clock))
		اذهب again;

	أرجع clock;
}

noinstr u64 local_clock_noinstr(فراغ)
{
	u64 clock;

	إذا (static_branch_likely(&__sched_clock_stable))
		أرجع sched_clock_noinstr() + __sched_clock_offset;

	إذا (!static_branch_likely(&sched_clock_running))
		أرجع sched_clock_noinstr();

	clock = sched_clock_local(this_scd());

	أرجع clock;
}

u64 local_clock(فراغ)
{
	u64 now;
	preempt_disable_notrace();
	now = local_clock_noinstr();
	preempt_enable_notrace();
	أرجع now;
}
EXPORT_علام_GPL(local_clock);

ساكن notrace u64 sched_clock_remote(بنية sched_clock_data *scd)
{
	بنية sched_clock_data *my_scd = this_scd();
	u64 this_clock, remote_clock;
	u64 *ptr, old_val, val;

#إذا BITS_PER_LONG != 64
again:
	/*
	 * Careful here: The local and the remote clock values need to
	 * be read out atomic as we need to compare the values and
	 * then update either the local or the remote side. So the
	 * cmpxchg64 below only protects one readout.
	 *
	 * We must reread via sched_clock_local() in the retry case on
	 * 32-bit kernels as an NMI could use sched_clock_local() via the
	 * tracer and hit between the readout of
	 * the low 32-bit and the high 32-bit portion.
	 */
	this_clock = sched_clock_local(my_scd);
	/*
	 * We must enforce atomic readout on 32-bit, otherwise the
	 * update on the remote CPU can hit in between the readout of
	 * the low 32-bit and the high 32-bit portion.
	 */
	remote_clock = cmpxchg64(&scd->clock, 0, 0);
#وإلا
	/*
	 * On 64-bit kernels the read of [my]scd->clock is atomic versus the
	 * update, so we can avoid the above 32-bit dance.
	 */
	sched_clock_local(my_scd);
again:
	this_clock = my_scd->clock;
	remote_clock = scd->clock;
#endif

	/*
	 * Use the opportunity that we have both locks
	 * taken to couple the two clocks: we take the
	 * larger time as the latest time for both
	 * runqueues. (this creates monotonic movement)
	 */
	إذا (likely((s64)(remote_clock - this_clock) < 0)) {
		ptr = &scd->clock;
		old_val = remote_clock;
		val = this_clock;
	} وإلا {
		/*
		 * Should be rare, but possible:
		 */
		ptr = &my_scd->clock;
		old_val = this_clock;
		val = remote_clock;
	}

	إذا (!try_cmpxchg64(ptr, &old_val, val))
		اذهب again;

	أرجع val;
}

/*
 * Similar to cpu_clock(), but requires local IRQs to be disabled.
 *
 * See cpu_clock().
 */
notrace u64 sched_clock_cpu(صحيح cpu)
{
	بنية sched_clock_data *scd;
	u64 clock;

	إذا (sched_clock_stable())
		أرجع sched_clock() + __sched_clock_offset;

	إذا (!static_branch_likely(&sched_clock_running))
		أرجع sched_clock();

	preempt_disable_notrace();
	scd = cpu_sdc(cpu);

	إذا (cpu != smp_processor_id())
		clock = sched_clock_remote(scd);
	وإلا
		clock = sched_clock_local(scd);
	preempt_enable_notrace();

	أرجع clock;
}
EXPORT_علام_GPL(sched_clock_cpu);

notrace فراغ sched_clock_tick(فراغ)
{
	بنية sched_clock_data *scd;

	إذا (sched_clock_stable())
		أرجع;

	إذا (!static_branch_likely(&sched_clock_running))
		أرجع;

	lockdep_assert_irqs_disabled();

	scd = this_scd();
	__scd_stamp(scd);
	sched_clock_local(scd);
}

notrace فراغ sched_clock_tick_stable(فراغ)
{
	إذا (!sched_clock_stable())
		أرجع;

	/*
	 * Called under watchdog_lock.
	 *
	 * The watchdog just found this TSC to (still) be stable, so now is a
	 * good moment to update our __gtod_offset. Because once we find the
	 * TSC to be unstable, any computation will be computing crap.
	 */
	local_irq_disable();
	__sched_clock_gtod_offset();
	local_irq_enable();
}

/*
 * We are going deep-idle (IRQs are disabled):
 */
notrace فراغ sched_clock_idle_sleep_event(فراغ)
{
	sched_clock_cpu(smp_processor_id());
}
EXPORT_علام_GPL(sched_clock_idle_sleep_event);

/*
 * We just idled; resync with ktime.
 */
notrace فراغ sched_clock_idle_wakeup_event(فراغ)
{
	unsigned long flags;

	إذا (sched_clock_stable())
		أرجع;

	إذا (unlikely(timekeeping_suspended))
		أرجع;

	local_irq_save(flags);
	sched_clock_tick();
	local_irq_restore(flags);
}
EXPORT_علام_GPL(sched_clock_idle_wakeup_event);

#وإلا /* !CONFIG_HAVE_UNSTABLE_SCHED_CLOCK: */

فراغ __init sched_clock_init(فراغ)
{
	static_branch_inc(&sched_clock_running);
	local_irq_disable();
	generic_sched_clock_init();
	local_irq_enable();
}

notrace u64 sched_clock_cpu(صحيح cpu)
{
	إذا (!static_branch_likely(&sched_clock_running))
		أرجع 0;

	أرجع sched_clock();
}

#endif /* !CONFIG_HAVE_UNSTABLE_SCHED_CLOCK */

/*
 * Running clock - returns the time that has elapsed while a guest has been
 * running.
 * On a guest this value should be local_clock minus the time the guest was
 * suspended by the hypervisor (for any reason).
 * On bare metal this function should return the same as local_clock.
 * Architectures and sub-architectures can override this.
 */
notrace u64 __weak running_clock(فراغ)
{
	أرجع local_clock();
}
