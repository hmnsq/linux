// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#ଅନ୍ତର୍ଭାଵ <uapi/linux/sched/types.h>
#ଅନ୍ତର୍ଭାଵ "sched.h"

#define IOWAIT_BOOST_MIN	(SCHED_CAPACITY_SCALE / 8)

ସଂରଚନା sugov_tunables {
	ସଂରଚନା gov_attr_set	attr_set;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		rate_limit_us;
};

ସଂରଚନା sugov_policy {
	ସଂରଚନା cpufreq_policy	*policy;

	ସଂରଚନା sugov_tunables	*tunables;
	ସଂରଚନା list_head	tunables_hook;

	raw_spinlock_t		update_lock;
	u64			last_freq_update_time;
	s64			freq_update_delay_ns;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		next_freq;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	ସଂରଚନା			irq_work irq_work;
	ସଂରଚନା			kthread_work work;
	ସଂରଚନା			mutex work_lock;
	ସଂରଚନା			kthread_worker worker;
	ସଂରଚନା task_struct	*thread;
	bool			work_in_progress;

	bool			limits_changed;
	bool			need_freq_update;
};

ସଂରଚନା sugov_cpu {
	ସଂରଚନା update_util_data	update_util;
	ସଂରଚନା sugov_policy	*sg_policy;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		cpu;

	bool			iowait_boost_pending;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		iowait_boost;
	u64			last_update;

	unsigned long		util;
	unsigned long		bw_min;
	unsigned long		bw_max;

	/* The field below is for single-CPU policies only: */
#ifdef CONFIG_NO_HZ_COMMON
	unsigned long		saved_idle_calls;
#endif
};

ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

ସ୍ଥିର ଶୂନ୍ୟ sugov_update_rate_limit_us(ସଂରଚନା sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->rate_limit_us * NSEC_PER_USEC;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_update_rate_limit_us_sync(ସଂରଚନା sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_set.update_lock);

	sugov_update_rate_limit_us(sg_policy);
}

ସ୍ଥିର bool sugov_should_update_freq(ସଂରଚନା sugov_policy *sg_policy, u64 time)
{
	s64 delta_ns;

	/*
	 * Since cpufreq_update_util() is called with rq->lock held for
	 * the @target_cpu, our per-CPU data is fully serialized.
	 *
	 * However, drivers cannot in general deal with cross-CPU
	 * requests, so while get_next_freq() will work, our
	 * sugov_update_commit() call may not for the fast switching platforms.
	 *
	 * Hence stop here for remote requests if they aren't supported
	 * by the hardware, as calculating the frequency is pointless if
	 * we cannot in fact act on it.
	 *
	 * This is needed on the slow switching platforms too to prevent CPUs
	 * going offline from leaving stale IRQ work items behind.
	 */
	ଯଦି (!cpufreq_this_cpu_can_update(sg_policy->policy))
		ଫେରନ୍ତୁ false;

	ଯଦି (unlikely(READ_ONCE(sg_policy->limits_changed))) {
		WRITE_ONCE(sg_policy->limits_changed, false);
		sg_policy->need_freq_update = true;

		/*
		 * The above limits_changed update must occur before the reads
		 * of policy limits in cpufreq_driver_resolve_freq() or a policy
		 * limits update might be missed, so use a memory barrier to
		 * ensure it.
		 *
		 * This pairs with the write memory barrier in sugov_limits().
		 */
		smp_mb();

		ଫେରନ୍ତୁ true;
	} ଅନ୍ୟଥା ଯଦି (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		ଫେରନ୍ତୁ true;
	}

	delta_ns = time - sg_policy->last_freq_update_time;

	ଫେରନ୍ତୁ delta_ns >= sg_policy->freq_update_delay_ns;
}

ସ୍ଥିର bool sugov_update_next_freq(ସଂରଚନା sugov_policy *sg_policy, u64 time,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_freq)
{
	ଯଦି (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		ଯଦି (sg_policy->next_freq == next_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			ଫେରନ୍ତୁ false;
	} ଅନ୍ୟଥା ଯଦି (sg_policy->next_freq == next_freq) {
		ଫେରନ୍ତୁ false;
	}

	sg_policy->next_freq = next_freq;
	sg_policy->last_freq_update_time = time;

	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_deferred_update(ସଂରଚନା sugov_policy *sg_policy)
{
	ଯଦି (!sg_policy->work_in_progress) {
		sg_policy->work_in_progress = true;
		irq_work_queue(&sg_policy->irq_work);
	}
}

/**
 * get_capacity_ref_freq - get the reference frequency that has been used to
 * correlate frequency and compute capacity for a given cpufreq policy. We use
 * the CPU managing it for the arch_scale_freq_ref() call in the function.
 * @policy: the cpufreq policy of the CPU in question.
 *
 * Return: the reference CPU frequency to compute a capacity.
 */
ସ୍ଥିର __always_inline
unsigned long get_capacity_ref_freq(ସଂରଚନା cpufreq_policy *policy)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା freq = arch_scale_freq_ref(policy->cpu);

	ଯଦି (freq)
		ଫେରନ୍ତୁ freq;

	ଯଦି (arch_scale_freq_invariant())
		ଫେରନ୍ତୁ policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	ଫେରନ୍ତୁ policy->cur + (policy->cur >> 2);
}

/**
 * get_next_freq - Compute a new frequency for a given cpufreq policy.
 * @sg_policy: schedutil policy object to compute the new frequency for.
 * @util: Current CPU utilization.
 * @max: CPU capacity.
 *
 * If the utilization is frequency-invariant, choose the new frequency to be
 * proportional to it, that is
 *
 * next_freq = C * max_freq * util / max
 *
 * Otherwise, approximate the would-be frequency-invariant utilization by
 * util_raw * (curr_freq / max_freq) which leads to
 *
 * next_freq = C * curr_freq * util_raw / max
 *
 * Take C = 1.25 for the frequency tipping point at (util / max) = 0.8.
 *
 * The lowest driver-supported frequency which is equal or greater than the raw
 * next_freq (as calculated above) is returned, subject to policy min/max and
 * cpufreq driver limitations.
 */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_next_freq(ସଂରଚନା sugov_policy *sg_policy,
				  unsigned long util, unsigned long max)
{
	ସଂରଚନା cpufreq_policy *policy = sg_policy->policy;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା freq;

	freq = get_capacity_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	ଯଦି (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		ଫେରନ୍ତୁ sg_policy->next_freq;

	sg_policy->cached_raw_freq = freq;
	ଫେରନ୍ତୁ cpufreq_driver_resolve_freq(policy, freq);
}

unsigned long sugov_effective_cpu_perf(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, unsigned long actual,
				 unsigned long min,
				 unsigned long max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	ଯଦି (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	ଫେରନ୍ତୁ max(min, max);
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_get_util(ସଂରଚନା sugov_cpu *sg_cpu, unsigned long boost)
{
	unsigned long min, max, util = scx_cpuperf_target(sg_cpu->cpu);

	ଯଦି (!scx_switched_all())
		util += cpu_util_cfs_boost(sg_cpu->cpu);
	util = effective_cpu_util(sg_cpu->cpu, util, &min, &max);
	util = max(util, boost);
	sg_cpu->bw_min = min;
	sg_cpu->bw_max = max;
	sg_cpu->util = sugov_effective_cpu_perf(sg_cpu->cpu, util, min, max);
}

/**
 * sugov_iowait_reset() - Reset the IO boost status of a CPU.
 * @sg_cpu: the sugov data for the CPU to boost
 * @time: the update time from the caller
 * @set_iowait_boost: true if an IO boost has been requested
 *
 * The IO wait boost of a task is disabled after a tick since the last update
 * of a CPU. If a new IO wait boost is requested after more then a tick, then
 * we enable the boost starting from IOWAIT_BOOST_MIN, which improves energy
 * efficiency by ignoring sporadic wakeups from IO.
 */
ସ୍ଥିର bool sugov_iowait_reset(ସଂରଚନା sugov_cpu *sg_cpu, u64 time,
			       bool set_iowait_boost)
{
	s64 delta_ns = time - sg_cpu->last_update;

	/* Reset boost only if a tick has elapsed since last request */
	ଯଦି (delta_ns <= TICK_NSEC)
		ଫେରନ୍ତୁ false;

	sg_cpu->iowait_boost = set_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = set_iowait_boost;

	ଫେରନ୍ତୁ true;
}

/**
 * sugov_iowait_boost() - Updates the IO boost status of a CPU.
 * @sg_cpu: the sugov data for the CPU to boost
 * @time: the update time from the caller
 * @flags: SCHED_CPUFREQ_IOWAIT if the task is waking up after an IO wait
 *
 * Each time a task wakes up after an IO operation, the CPU utilization can be
 * boosted to a certain utilization which doubles at each "frequent and
 * successive" wakeup from IO, ranging from IOWAIT_BOOST_MIN to the utilization
 * of the maximum OPP.
 *
 * To keep doubling, an IO boost has to be requested at least once per tick,
 * otherwise we restart from the utilization of the minimum OPP.
 */
ସ୍ଥିର ଶୂନ୍ୟ sugov_iowait_boost(ସଂରଚନା sugov_cpu *sg_cpu, u64 time,
			       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	bool set_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	ଯଦି (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, time, set_iowait_boost))
		ଫେରନ୍ତୁ;

	/* Boost only tasks waking up after IO */
	ଯଦି (!set_iowait_boost)
		ଫେରନ୍ତୁ;

	/* Ensure boost doubles only one time at each request */
	ଯଦି (sg_cpu->iowait_boost_pending)
		ଫେରନ୍ତୁ;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	ଯଦି (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sg_cpu->iowait_boost << 1, SCHED_CAPACITY_SCALE);
		ଫେରନ୍ତୁ;
	}

	/* First wakeup after IO: start with minimum boost */
	sg_cpu->iowait_boost = IOWAIT_BOOST_MIN;
}

/**
 * sugov_iowait_apply() - Apply the IO boost to a CPU.
 * @sg_cpu: the sugov data for the cpu to boost
 * @time: the update time from the caller
 * @max_cap: the max CPU capacity
 *
 * A CPU running a task which woken up after an IO operation can have its
 * utilization boosted to speed up the completion of those IO operations.
 * The IO boost value is increased each time a task wakes up from IO, in
 * sugov_iowait_boost(), and it's instead decreased by this function,
 * each time an increase has not been requested (!iowait_boost_pending).
 *
 * A CPU which also appears to have been idle for at least one tick has also
 * its IO boost utilization reset.
 *
 * This mechanism is designed to boost high frequently IO waiting tasks, while
 * being more conservative on tasks which does sporadic IO operations.
 */
ସ୍ଥିର unsigned long sugov_iowait_apply(ସଂରଚନା sugov_cpu *sg_cpu, u64 time,
			       unsigned long max_cap)
{
	/* No boost currently required */
	ଯଦି (!sg_cpu->iowait_boost)
		ଫେରନ୍ତୁ 0;

	/* Reset boost if the CPU appears to have been idle enough */
	ଯଦି (sugov_iowait_reset(sg_cpu, time, false))
		ଫେରନ୍ତୁ 0;

	ଯଦି (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		ଯଦି (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			ଫେରନ୍ତୁ 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	ଫେରନ୍ତୁ (sg_cpu->iowait_boost * max_cap) >> SCHED_CAPACITY_SHIFT;
}

#ifdef CONFIG_NO_HZ_COMMON
ସ୍ଥିର bool sugov_hold_freq(ସଂରଚନା sugov_cpu *sg_cpu)
{
	unsigned long idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	ଯଦି (scx_switched_all())
		ଫେରନ୍ତୁ false;

	/* if capped by uclamp_max, always update to be in compliance */
	ଯଦି (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		ଫେରନ୍ତୁ false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	ଫେରନ୍ତୁ ret;
}
#ଅନ୍ୟଥା /* !CONFIG_NO_HZ_COMMON: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sugov_hold_freq(ସଂରଚନା sugov_cpu *sg_cpu) { ଫେରନ୍ତୁ false; }
#endif /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ ignore_dl_rate_limit(ସଂରଚନା sugov_cpu *sg_cpu)
{
	ଯଦି (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sugov_update_single_common(ସଂରଚନା sugov_cpu *sg_cpu,
					      u64 time, unsigned long max_cap,
					      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	unsigned long boost;

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu);

	ଯଦି (!sugov_should_update_freq(sg_cpu->sg_policy, time))
		ଫେରନ୍ତୁ false;

	boost = sugov_iowait_apply(sg_cpu, time, max_cap);
	sugov_get_util(sg_cpu, boost);

	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_update_single_freq(ସଂରଚନା update_util_data *hook, u64 time,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sugov_cpu *sg_cpu = container_of(hook, ସଂରଚନା sugov_cpu, update_util);
	ସଂରଚନା sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cached_freq = sg_policy->cached_raw_freq;
	unsigned long max_cap;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_f;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	ଯଦି (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		ଫେରନ୍ତୁ;

	next_f = get_next_freq(sg_policy, sg_cpu->util, max_cap);

	ଯଦି (sugov_hold_freq(sg_cpu) && next_f < sg_policy->next_freq &&
	    !sg_policy->need_freq_update) {
		next_f = sg_policy->next_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	ଯଦି (!sugov_update_next_freq(sg_policy, time, next_f))
		ଫେରନ୍ତୁ;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	ଯଦି (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, next_f);
	} ଅନ୍ୟଥା {
		raw_spin_lock(&sg_policy->update_lock);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_lock);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_update_single_perf(ସଂରଚନା update_util_data *hook, u64 time,
				     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sugov_cpu *sg_cpu = container_of(hook, ସଂରଚନା sugov_cpu, update_util);
	ସଂରଚନା sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned long prev_util = sg_cpu->util;
	unsigned long max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	ଯଦି (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, time, flags);
		ଫେରନ୍ତୁ;
	}

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	ଯଦି (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		ଫେରନ୍ତୁ;

	ଯଦି (sugov_hold_freq(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->last_freq_update_time = time;
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sugov_next_freq_shared(ସଂରଚନା sugov_cpu *sg_cpu, u64 time)
{
	ସଂରଚନା sugov_policy *sg_policy = sg_cpu->sg_policy;
	ସଂରଚନା cpufreq_policy *policy = sg_policy->policy;
	unsigned long util = 0, max_cap;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		ସଂରଚନା sugov_cpu *j_sg_cpu = &per_cpu(sugov_cpu, j);
		unsigned long boost;

		boost = sugov_iowait_apply(j_sg_cpu, time, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	ଫେରନ୍ତୁ get_next_freq(sg_policy, util, max_cap);
}

ସ୍ଥିର ଶୂନ୍ୟ
sugov_update_shared(ସଂରଚନା update_util_data *hook, u64 time, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sugov_cpu *sg_cpu = container_of(hook, ସଂରଚନା sugov_cpu, update_util);
	ସଂରଚନା sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_f;

	raw_spin_lock(&sg_policy->update_lock);

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu);

	ଯଦି (sugov_should_update_freq(sg_policy, time)) {
		next_f = sugov_next_freq_shared(sg_cpu, time);

		ଯଦି (!sugov_update_next_freq(sg_policy, time, next_f))
			ଯାଅ unlock;

		ଯଦି (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, next_f);
		ଅନ୍ୟଥା
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_lock);
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_work(ସଂରଚନା kthread_work *work)
{
	ସଂରଚନା sugov_policy *sg_policy = container_of(work, ସଂରଚନା sugov_policy, work);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା freq;
	unsigned long flags;

	/*
	 * Hold sg_policy->update_lock shortly to handle the case where:
	 * in case sg_policy->next_freq is read here, and then updated by
	 * sugov_deferred_update() just before work_in_progress is set to false
	 * here, we may miss queueing the new update.
	 *
	 * Note: If a work was queued after the update_lock is released,
	 * sugov_work() will just be called again by kthread_work code; and the
	 * request will be proceed before the sugov thread sleeps.
	 */
	raw_spin_lock_irqsave(&sg_policy->update_lock, flags);
	freq = sg_policy->next_freq;
	sg_policy->work_in_progress = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_lock, flags);

	mutex_lock(&sg_policy->work_lock);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_RELATION_L);
	mutex_unlock(&sg_policy->work_lock);
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_irq_work(ସଂରଚନା irq_work *irq_work)
{
	ସଂରଚନା sugov_policy *sg_policy;

	sg_policy = container_of(irq_work, ସଂରଚନା sugov_policy, irq_work);

	kthread_queue_work(&sg_policy->worker, &sg_policy->work);
}

/************************** sysfs interface ************************/

ସ୍ଥିର ସଂରଚନା sugov_tunables *global_tunables;
ସ୍ଥିର DEFINE_MUTEX(global_tunables_lock);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା sugov_tunables *to_sugov_tunables(ସଂରଚନା gov_attr_set *attr_set)
{
	ଫେରନ୍ତୁ container_of(attr_set, ସଂରଚନା sugov_tunables, attr_set);
}

ସ୍ଥିର ssize_t rate_limit_us_show(ସଂରଚନା gov_attr_set *attr_set, ଅକ୍ଷର *buf)
{
	ସଂରଚନା sugov_tunables *tunables = to_sugov_tunables(attr_set);

	ଫେରନ୍ତୁ sysfs_emit(buf, "%u\n", tunables->rate_limit_us);
}

ସ୍ଥିର ssize_t
rate_limit_us_store(ସଂରଚନା gov_attr_set *attr_set, ଅଚଳ ଅକ୍ଷର *buf, size_t ଗଣନା_2)
{
	ସଂରଚନା sugov_tunables *tunables = to_sugov_tunables(attr_set);
	ସଂରଚନା sugov_policy *sg_policy;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rate_limit_us;

	ଯଦି (kstrtouint(buf, 10, &rate_limit_us))
		ଫେରନ୍ତୁ -EINVAL;

	tunables->rate_limit_us = rate_limit_us;

	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook)
		sugov_update_rate_limit_us(sg_policy);

	ଫେରନ୍ତୁ ଗଣନା_2;
}

ସ୍ଥିର ସଂରଚନା governor_attr rate_limit_us = __ATTR_RW(rate_limit_us);

ସ୍ଥିର ସଂରଚନା attribute *sugov_attrs[] = {
	&rate_limit_us.attr,
	ଖାଲି
};
ATTRIBUTE_GROUPS(sugov);

ସ୍ଥିର ଶୂନ୍ୟ sugov_tunables_free(ସଂରଚନା kobject *kobj)
{
	ସଂରଚନା gov_attr_set *attr_set = to_gov_attr_set(kobj);

	kfree(to_sugov_tunables(attr_set));
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

ସ୍ଥିର ସଂରଚନା cpufreq_governor schedutil_gov;

ସ୍ଥିର ସଂରଚନା sugov_policy *sugov_policy_alloc(ସଂରଚନା cpufreq_policy *policy)
{
	ସଂରଚନା sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	ଯଦି (!sg_policy)
		ଫେରନ୍ତୁ ଖାଲି;

	sg_policy->policy = policy;
	raw_spin_lock_init(&sg_policy->update_lock);
	ଫେରନ୍ତୁ sg_policy;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_policy_free(ସଂରଚନା sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sugov_kthread_create(ସଂରଚନା sugov_policy *sg_policy)
{
	ସଂରଚନା task_struct *thread;
	ସଂରଚନା sched_attr attr = {
		.size		= ଆକାର(ସଂରଚନା sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_priority	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= NSEC_PER_MSEC,
		.sched_deadline = 10 * NSEC_PER_MSEC,
		.sched_period	= 10 * NSEC_PER_MSEC,
	};
	ସଂରଚନା cpufreq_policy *policy = sg_policy->policy;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/* kthread only required for slow path */
	ଯଦି (policy->fast_switch_enabled)
		ଫେରନ୍ତୁ 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_worker(&sg_policy->worker);
	thread = kthread_create(kthread_worker_fn, &sg_policy->worker,
				"sugov:%d",
				cpumask_first(policy->related_cpus));
	ଯଦି (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		ଫେରନ୍ତୁ PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	ଯଦି (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		ଫେରନ୍ତୁ ret;
	}

	sg_policy->thread = thread;
	ଯଦି (policy->dvfs_possible_from_any_cpu)
		set_cpus_allowed_ptr(thread, policy->related_cpus);
	ଅନ୍ୟଥା
		kthread_bind_mask(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_lock);

	wake_up_process(thread);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_kthread_stop(ସଂରଚନା sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	ଯଦି (sg_policy->policy->fast_switch_enabled)
		ଫେରନ୍ତୁ;

	kthread_flush_worker(&sg_policy->worker);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_lock);
}

ସ୍ଥିର ସଂରଚନା sugov_tunables *sugov_tunables_alloc(ସଂରଚନା sugov_policy *sg_policy)
{
	ସଂରଚନା sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	ଯଦି (tunables) {
		gov_attr_set_init(&tunables->attr_set, &sg_policy->tunables_hook);
		ଯଦି (!have_governor_per_policy())
			global_tunables = tunables;
	}
	ଫେରନ୍ତୁ tunables;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_clear_global_tunables(ଶୂନ୍ୟ)
{
	ଯଦି (!have_governor_per_policy())
		global_tunables = ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sugov_init(ସଂରଚନା cpufreq_policy *policy)
{
	ସଂରଚନା sugov_policy *sg_policy;
	ସଂରଚନା sugov_tunables *tunables;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	/* State should be equivalent to EXIT */
	ଯଦି (policy->governor_data)
		ଫେରନ୍ତୁ -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	ଯଦି (!sg_policy) {
		ret = -ENOMEM;
		ଯାଅ disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	ଯଦି (ret)
		ଯାଅ free_sg_policy;

	mutex_lock(&global_tunables_lock);

	ଯଦି (global_tunables) {
		ଯଦି (WARN_ON(have_governor_per_policy())) {
			ret = -EINVAL;
			ଯାଅ stop_kthread;
		}
		policy->governor_data = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_set_get(&global_tunables->attr_set, &sg_policy->tunables_hook);
		ଯାଅ out;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	ଯଦି (!tunables) {
		ret = -ENOMEM;
		ଯାଅ stop_kthread;
	}

	tunables->rate_limit_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_data = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_add(&tunables->attr_set.kobj, &sugov_tunables_ktype,
				   get_governor_parent_kobj(policy), "%s",
				   schedutil_gov.name);
	ଯଦି (ret)
		ଯାଅ fail;

out:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_lock);
	ଫେରନ୍ତୁ 0;

fail:
	kobject_put(&tunables->attr_set.kobj);
	policy->governor_data = ଖାଲି;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_lock);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_exit(ସଂରଚନା cpufreq_policy *policy)
{
	ସଂରଚନା sugov_policy *sg_policy = policy->governor_data;
	ସଂରଚନା sugov_tunables *tunables = sg_policy->tunables;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;

	mutex_lock(&global_tunables_lock);

	ଗଣନା_2 = gov_attr_set_put(&tunables->attr_set, &sg_policy->tunables_hook);
	policy->governor_data = ଖାଲି;
	ଯଦି (!ଗଣନା_2)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_lock);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sugov_start(ସଂରଚନା cpufreq_policy *policy)
{
	ସଂରଚନା sugov_policy *sg_policy = policy->governor_data;
	ଶୂନ୍ୟ (*uu)(ସଂରଚନା update_util_data *data, u64 time, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	sugov_update_rate_limit_us_sync(sg_policy);
	sg_policy->last_freq_update_time	= 0;
	sg_policy->next_freq			= 0;
	sg_policy->work_in_progress		= false;
	sg_policy->limits_changed		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	ଯଦି (policy_is_shared(policy))
		uu = sugov_update_shared;
	ଅନ୍ୟଥା ଯଦି (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	ଅନ୍ୟଥା
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		ସଂରଚନା sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, ଆକାର(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		ସଂରଚନା sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		cpufreq_add_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_stop(ସଂରଚନା cpufreq_policy *policy)
{
	ସଂରଚନା sugov_policy *sg_policy = policy->governor_data;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	ଯଦି (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ sugov_limits(ସଂରଚନା cpufreq_policy *policy)
{
	ସଂରଚନା sugov_policy *sg_policy = policy->governor_data;

	ଯଦି (!policy->fast_switch_enabled) {
		mutex_lock(&sg_policy->work_lock);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_lock);
	}

	/*
	 * The limits_changed update below must take place before the updates
	 * of policy limits in cpufreq_set_policy() or a policy limits update
	 * might be missed, so use a memory barrier to ensure it.
	 *
	 * This pairs with the memory barrier in sugov_should_update_freq().
	 */
	smp_wmb();

	WRITE_ONCE(sg_policy->limits_changed, true);
}

ସ୍ଥିର ସଂରଚନା cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.exit			= sugov_exit,
	.start			= sugov_start,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#ifdef CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
ସଂରଚନା cpufreq_governor *cpufreq_default_governor(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ &schedutil_gov;
}
#endif

bool sugov_is_governor(ସଂରଚନା cpufreq_policy *policy)
{
	ଫେରନ୍ତୁ policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
