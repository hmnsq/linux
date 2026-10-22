// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#ಅನ್ತರ್ಭಾವ <uapi/linux/sched/types.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

#define IOWAIT_BOOST_MIN	(SCHED_CAPACITY_SCALE / 8)

ಸಂರಚನಾ sugov_tunables {
	ಸಂರಚನಾ gov_attr_set	attr_set;
	unsigned ಪೂರ್ಣಾಙ್ಕ		rate_limit_us;
};

ಸಂರಚನಾ sugov_policy {
	ಸಂರಚನಾ cpufreq_policy	*policy;

	ಸಂರಚನಾ sugov_tunables	*tunables;
	ಸಂರಚನಾ list_head	tunables_hook;

	raw_spinlock_t		update_lock;
	u64			last_freq_update_time;
	s64			freq_update_delay_ns;
	unsigned ಪೂರ್ಣಾಙ್ಕ		next_freq;
	unsigned ಪೂರ್ಣಾಙ್ಕ		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	ಸಂರಚನಾ			irq_work irq_work;
	ಸಂರಚನಾ			kthread_work work;
	ಸಂರಚನಾ			mutex work_lock;
	ಸಂರಚನಾ			kthread_worker worker;
	ಸಂರಚನಾ task_struct	*thread;
	bool			work_in_progress;

	bool			limits_changed;
	bool			need_freq_update;
};

ಸಂರಚನಾ sugov_cpu {
	ಸಂರಚನಾ update_util_data	update_util;
	ಸಂರಚನಾ sugov_policy	*sg_policy;
	unsigned ಪೂರ್ಣಾಙ್ಕ		cpu;

	bool			iowait_boost_pending;
	unsigned ಪೂರ್ಣಾಙ್ಕ		iowait_boost;
	u64			last_update;

	unsigned long		util;
	unsigned long		bw_min;
	unsigned long		bw_max;

	/* The field below is for single-CPU policies only: */
#ifdef CONFIG_NO_HZ_COMMON
	unsigned long		saved_idle_calls;
#endif
};

ಸ್ಥಿರ DEFINE_PER_CPU(ಸಂರಚನಾ sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_rate_limit_us(ಸಂರಚನಾ sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->rate_limit_us * NSEC_PER_USEC;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_rate_limit_us_sync(ಸಂರಚನಾ sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_set.update_lock);

	sugov_update_rate_limit_us(sg_policy);
}

ಸ್ಥಿರ bool sugov_should_update_freq(ಸಂರಚನಾ sugov_policy *sg_policy, u64 time)
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
	ಯದಿ (!cpufreq_this_cpu_can_update(sg_policy->policy))
		ಪ್ರತ್ಯಾಗಮ false;

	ಯದಿ (unlikely(READ_ONCE(sg_policy->limits_changed))) {
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

		ಪ್ರತ್ಯಾಗಮ true;
	} ಅನ್ಯಥಾ ಯದಿ (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		ಪ್ರತ್ಯಾಗಮ true;
	}

	delta_ns = time - sg_policy->last_freq_update_time;

	ಪ್ರತ್ಯಾಗಮ delta_ns >= sg_policy->freq_update_delay_ns;
}

ಸ್ಥಿರ bool sugov_update_next_freq(ಸಂರಚನಾ sugov_policy *sg_policy, u64 time,
				   unsigned ಪೂರ್ಣಾಙ್ಕ next_freq)
{
	ಯದಿ (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		ಯದಿ (sg_policy->next_freq == next_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			ಪ್ರತ್ಯಾಗಮ false;
	} ಅನ್ಯಥಾ ಯದಿ (sg_policy->next_freq == next_freq) {
		ಪ್ರತ್ಯಾಗಮ false;
	}

	sg_policy->next_freq = next_freq;
	sg_policy->last_freq_update_time = time;

	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_deferred_update(ಸಂರಚನಾ sugov_policy *sg_policy)
{
	ಯದಿ (!sg_policy->work_in_progress) {
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
ಸ್ಥಿರ __always_inline
unsigned long get_capacity_ref_freq(ಸಂರಚನಾ cpufreq_policy *policy)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ freq = arch_scale_freq_ref(policy->cpu);

	ಯದಿ (freq)
		ಪ್ರತ್ಯಾಗಮ freq;

	ಯದಿ (arch_scale_freq_invariant())
		ಪ್ರತ್ಯಾಗಮ policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	ಪ್ರತ್ಯಾಗಮ policy->cur + (policy->cur >> 2);
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
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ get_next_freq(ಸಂರಚನಾ sugov_policy *sg_policy,
				  unsigned long util, unsigned long max)
{
	ಸಂರಚನಾ cpufreq_policy *policy = sg_policy->policy;
	unsigned ಪೂರ್ಣಾಙ್ಕ freq;

	freq = get_capacity_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	ಯದಿ (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		ಪ್ರತ್ಯಾಗಮ sg_policy->next_freq;

	sg_policy->cached_raw_freq = freq;
	ಪ್ರತ್ಯಾಗಮ cpufreq_driver_resolve_freq(policy, freq);
}

unsigned long sugov_effective_cpu_perf(ಪೂರ್ಣಾಙ್ಕ cpu, unsigned long actual,
				 unsigned long min,
				 unsigned long max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	ಯದಿ (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	ಪ್ರತ್ಯಾಗಮ max(min, max);
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_get_util(ಸಂರಚನಾ sugov_cpu *sg_cpu, unsigned long boost)
{
	unsigned long min, max, util = scx_cpuperf_target(sg_cpu->cpu);

	ಯದಿ (!scx_switched_all())
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
ಸ್ಥಿರ bool sugov_iowait_reset(ಸಂರಚನಾ sugov_cpu *sg_cpu, u64 time,
			       bool set_iowait_boost)
{
	s64 delta_ns = time - sg_cpu->last_update;

	/* Reset boost only if a tick has elapsed since last request */
	ಯದಿ (delta_ns <= TICK_NSEC)
		ಪ್ರತ್ಯಾಗಮ false;

	sg_cpu->iowait_boost = set_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = set_iowait_boost;

	ಪ್ರತ್ಯಾಗಮ true;
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
ಸ್ಥಿರ ಶೂನ್ಯ sugov_iowait_boost(ಸಂರಚನಾ sugov_cpu *sg_cpu, u64 time,
			       unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	bool set_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	ಯದಿ (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, time, set_iowait_boost))
		ಪ್ರತ್ಯಾಗಮ;

	/* Boost only tasks waking up after IO */
	ಯದಿ (!set_iowait_boost)
		ಪ್ರತ್ಯಾಗಮ;

	/* Ensure boost doubles only one time at each request */
	ಯದಿ (sg_cpu->iowait_boost_pending)
		ಪ್ರತ್ಯಾಗಮ;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	ಯದಿ (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(unsigned ಪೂರ್ಣಾಙ್ಕ, sg_cpu->iowait_boost << 1, SCHED_CAPACITY_SCALE);
		ಪ್ರತ್ಯಾಗಮ;
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
ಸ್ಥಿರ unsigned long sugov_iowait_apply(ಸಂರಚನಾ sugov_cpu *sg_cpu, u64 time,
			       unsigned long max_cap)
{
	/* No boost currently required */
	ಯದಿ (!sg_cpu->iowait_boost)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Reset boost if the CPU appears to have been idle enough */
	ಯದಿ (sugov_iowait_reset(sg_cpu, time, false))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		ಯದಿ (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	ಪ್ರತ್ಯಾಗಮ (sg_cpu->iowait_boost * max_cap) >> SCHED_CAPACITY_SHIFT;
}

#ifdef CONFIG_NO_HZ_COMMON
ಸ್ಥಿರ bool sugov_hold_freq(ಸಂರಚನಾ sugov_cpu *sg_cpu)
{
	unsigned long idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	ಯದಿ (scx_switched_all())
		ಪ್ರತ್ಯಾಗಮ false;

	/* if capped by uclamp_max, always update to be in compliance */
	ಯದಿ (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		ಪ್ರತ್ಯಾಗಮ false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	ಪ್ರತ್ಯಾಗಮ ret;
}
#ಅನ್ಯಥಾ /* !CONFIG_NO_HZ_COMMON: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sugov_hold_freq(ಸಂರಚನಾ sugov_cpu *sg_cpu) { ಪ್ರತ್ಯಾಗಮ false; }
#endif /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ ignore_dl_rate_limit(ಸಂರಚನಾ sugov_cpu *sg_cpu)
{
	ಯದಿ (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sugov_update_single_common(ಸಂರಚನಾ sugov_cpu *sg_cpu,
					      u64 time, unsigned long max_cap,
					      unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	unsigned long boost;

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu);

	ಯದಿ (!sugov_should_update_freq(sg_cpu->sg_policy, time))
		ಪ್ರತ್ಯಾಗಮ false;

	boost = sugov_iowait_apply(sg_cpu, time, max_cap);
	sugov_get_util(sg_cpu, boost);

	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_single_freq(ಸಂರಚನಾ update_util_data *hook, u64 time,
				     unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ sugov_cpu *sg_cpu = container_of(hook, ಸಂರಚನಾ sugov_cpu, update_util);
	ಸಂರಚನಾ sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned ಪೂರ್ಣಾಙ್ಕ cached_freq = sg_policy->cached_raw_freq;
	unsigned long max_cap;
	unsigned ಪೂರ್ಣಾಙ್ಕ next_f;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	ಯದಿ (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		ಪ್ರತ್ಯಾಗಮ;

	next_f = get_next_freq(sg_policy, sg_cpu->util, max_cap);

	ಯದಿ (sugov_hold_freq(sg_cpu) && next_f < sg_policy->next_freq &&
	    !sg_policy->need_freq_update) {
		next_f = sg_policy->next_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	ಯದಿ (!sugov_update_next_freq(sg_policy, time, next_f))
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	ಯದಿ (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, next_f);
	} ಅನ್ಯಥಾ {
		raw_spin_lock(&sg_policy->update_lock);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_lock);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_single_perf(ಸಂರಚನಾ update_util_data *hook, u64 time,
				     unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ sugov_cpu *sg_cpu = container_of(hook, ಸಂರಚನಾ sugov_cpu, update_util);
	ಸಂರಚನಾ sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned long prev_util = sg_cpu->util;
	unsigned long max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	ಯದಿ (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, time, flags);
		ಪ್ರತ್ಯಾಗಮ;
	}

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	ಯದಿ (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (sugov_hold_freq(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->last_freq_update_time = time;
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ sugov_next_freq_shared(ಸಂರಚನಾ sugov_cpu *sg_cpu, u64 time)
{
	ಸಂರಚನಾ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ಸಂರಚನಾ cpufreq_policy *policy = sg_policy->policy;
	unsigned long util = 0, max_cap;
	unsigned ಪೂರ್ಣಾಙ್ಕ j;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		ಸಂರಚನಾ sugov_cpu *j_sg_cpu = &per_cpu(sugov_cpu, j);
		unsigned long boost;

		boost = sugov_iowait_apply(j_sg_cpu, time, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	ಪ್ರತ್ಯಾಗಮ get_next_freq(sg_policy, util, max_cap);
}

ಸ್ಥಿರ ಶೂನ್ಯ
sugov_update_shared(ಸಂರಚನಾ update_util_data *hook, u64 time, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ sugov_cpu *sg_cpu = container_of(hook, ಸಂರಚನಾ sugov_cpu, update_util);
	ಸಂರಚನಾ sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned ಪೂರ್ಣಾಙ್ಕ next_f;

	raw_spin_lock(&sg_policy->update_lock);

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu);

	ಯದಿ (sugov_should_update_freq(sg_policy, time)) {
		next_f = sugov_next_freq_shared(sg_cpu, time);

		ಯದಿ (!sugov_update_next_freq(sg_policy, time, next_f))
			ಗಚ್ಛ unlock;

		ಯದಿ (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, next_f);
		ಅನ್ಯಥಾ
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_lock);
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_work(ಸಂರಚನಾ kthread_work *work)
{
	ಸಂರಚನಾ sugov_policy *sg_policy = container_of(work, ಸಂರಚನಾ sugov_policy, work);
	unsigned ಪೂರ್ಣಾಙ್ಕ freq;
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

ಸ್ಥಿರ ಶೂನ್ಯ sugov_irq_work(ಸಂರಚನಾ irq_work *irq_work)
{
	ಸಂರಚನಾ sugov_policy *sg_policy;

	sg_policy = container_of(irq_work, ಸಂರಚನಾ sugov_policy, irq_work);

	kthread_queue_work(&sg_policy->worker, &sg_policy->work);
}

/************************** sysfs interface ************************/

ಸ್ಥಿರ ಸಂರಚನಾ sugov_tunables *global_tunables;
ಸ್ಥಿರ DEFINE_MUTEX(global_tunables_lock);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ sugov_tunables *to_sugov_tunables(ಸಂರಚನಾ gov_attr_set *attr_set)
{
	ಪ್ರತ್ಯಾಗಮ container_of(attr_set, ಸಂರಚನಾ sugov_tunables, attr_set);
}

ಸ್ಥಿರ ssize_t rate_limit_us_show(ಸಂರಚನಾ gov_attr_set *attr_set, ವರ್ಣ *buf)
{
	ಸಂರಚನಾ sugov_tunables *tunables = to_sugov_tunables(attr_set);

	ಪ್ರತ್ಯಾಗಮ sysfs_emit(buf, "%u\n", tunables->rate_limit_us);
}

ಸ್ಥಿರ ssize_t
rate_limit_us_store(ಸಂರಚನಾ gov_attr_set *attr_set, ಧ್ರುವ ವರ್ಣ *buf, size_t ಎಣಿಕೆ)
{
	ಸಂರಚನಾ sugov_tunables *tunables = to_sugov_tunables(attr_set);
	ಸಂರಚನಾ sugov_policy *sg_policy;
	unsigned ಪೂರ್ಣಾಙ್ಕ rate_limit_us;

	ಯದಿ (kstrtouint(buf, 10, &rate_limit_us))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	tunables->rate_limit_us = rate_limit_us;

	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook)
		sugov_update_rate_limit_us(sg_policy);

	ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಸ್ಥಿರ ಸಂರಚನಾ governor_attr rate_limit_us = __ATTR_RW(rate_limit_us);

ಸ್ಥಿರ ಸಂರಚನಾ attribute *sugov_attrs[] = {
	&rate_limit_us.attr,
	ರಿಕ್ತ
};
ATTRIBUTE_GROUPS(sugov);

ಸ್ಥಿರ ಶೂನ್ಯ sugov_tunables_free(ಸಂರಚನಾ kobject *kobj)
{
	ಸಂರಚನಾ gov_attr_set *attr_set = to_gov_attr_set(kobj);

	kfree(to_sugov_tunables(attr_set));
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

ಸ್ಥಿರ ಸಂರಚನಾ cpufreq_governor schedutil_gov;

ಸ್ಥಿರ ಸಂರಚನಾ sugov_policy *sugov_policy_alloc(ಸಂರಚನಾ cpufreq_policy *policy)
{
	ಸಂರಚನಾ sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	ಯದಿ (!sg_policy)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	sg_policy->policy = policy;
	raw_spin_lock_init(&sg_policy->update_lock);
	ಪ್ರತ್ಯಾಗಮ sg_policy;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_policy_free(ಸಂರಚನಾ sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sugov_kthread_create(ಸಂರಚನಾ sugov_policy *sg_policy)
{
	ಸಂರಚನಾ task_struct *thread;
	ಸಂರಚನಾ sched_attr attr = {
		.size		= ಪರಿಮಾಣ(ಸಂರಚನಾ sched_attr),
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
	ಸಂರಚನಾ cpufreq_policy *policy = sg_policy->policy;
	ಪೂರ್ಣಾಙ್ಕ ret;

	/* kthread only required for slow path */
	ಯದಿ (policy->fast_switch_enabled)
		ಪ್ರತ್ಯಾಗಮ 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_worker(&sg_policy->worker);
	thread = kthread_create(kthread_worker_fn, &sg_policy->worker,
				"sugov:%d",
				cpumask_first(policy->related_cpus));
	ಯದಿ (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		ಪ್ರತ್ಯಾಗಮ PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	ಯದಿ (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	sg_policy->thread = thread;
	ಯದಿ (policy->dvfs_possible_from_any_cpu)
		set_cpus_allowed_ptr(thread, policy->related_cpus);
	ಅನ್ಯಥಾ
		kthread_bind_mask(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_lock);

	wake_up_process(thread);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_kthread_stop(ಸಂರಚನಾ sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	ಯದಿ (sg_policy->policy->fast_switch_enabled)
		ಪ್ರತ್ಯಾಗಮ;

	kthread_flush_worker(&sg_policy->worker);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_lock);
}

ಸ್ಥಿರ ಸಂರಚನಾ sugov_tunables *sugov_tunables_alloc(ಸಂರಚನಾ sugov_policy *sg_policy)
{
	ಸಂರಚನಾ sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	ಯದಿ (tunables) {
		gov_attr_set_init(&tunables->attr_set, &sg_policy->tunables_hook);
		ಯದಿ (!have_governor_per_policy())
			global_tunables = tunables;
	}
	ಪ್ರತ್ಯಾಗಮ tunables;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_clear_global_tunables(ಶೂನ್ಯ)
{
	ಯದಿ (!have_governor_per_policy())
		global_tunables = ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sugov_init(ಸಂರಚನಾ cpufreq_policy *policy)
{
	ಸಂರಚನಾ sugov_policy *sg_policy;
	ಸಂರಚನಾ sugov_tunables *tunables;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	/* State should be equivalent to EXIT */
	ಯದಿ (policy->governor_data)
		ಪ್ರತ್ಯಾಗಮ -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	ಯದಿ (!sg_policy) {
		ret = -ENOMEM;
		ಗಚ್ಛ disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	ಯದಿ (ret)
		ಗಚ್ಛ free_sg_policy;

	mutex_lock(&global_tunables_lock);

	ಯದಿ (global_tunables) {
		ಯದಿ (WARN_ON(have_governor_per_policy())) {
			ret = -EINVAL;
			ಗಚ್ಛ stop_kthread;
		}
		policy->governor_data = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_set_get(&global_tunables->attr_set, &sg_policy->tunables_hook);
		ಗಚ್ಛ out;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	ಯದಿ (!tunables) {
		ret = -ENOMEM;
		ಗಚ್ಛ stop_kthread;
	}

	tunables->rate_limit_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_data = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_add(&tunables->attr_set.kobj, &sugov_tunables_ktype,
				   get_governor_parent_kobj(policy), "%s",
				   schedutil_gov.name);
	ಯದಿ (ret)
		ಗಚ್ಛ fail;

out:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_lock);
	ಪ್ರತ್ಯಾಗಮ 0;

fail:
	kobject_put(&tunables->attr_set.kobj);
	policy->governor_data = ರಿಕ್ತ;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_lock);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_exit(ಸಂರಚನಾ cpufreq_policy *policy)
{
	ಸಂರಚನಾ sugov_policy *sg_policy = policy->governor_data;
	ಸಂರಚನಾ sugov_tunables *tunables = sg_policy->tunables;
	unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;

	mutex_lock(&global_tunables_lock);

	ಎಣಿಕೆ = gov_attr_set_put(&tunables->attr_set, &sg_policy->tunables_hook);
	policy->governor_data = ರಿಕ್ತ;
	ಯದಿ (!ಎಣಿಕೆ)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_lock);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sugov_start(ಸಂರಚನಾ cpufreq_policy *policy)
{
	ಸಂರಚನಾ sugov_policy *sg_policy = policy->governor_data;
	ಶೂನ್ಯ (*uu)(ಸಂರಚನಾ update_util_data *data, u64 time, unsigned ಪೂರ್ಣಾಙ್ಕ flags);
	unsigned ಪೂರ್ಣಾಙ್ಕ cpu;

	sugov_update_rate_limit_us_sync(sg_policy);
	sg_policy->last_freq_update_time	= 0;
	sg_policy->next_freq			= 0;
	sg_policy->work_in_progress		= false;
	sg_policy->limits_changed		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	ಯದಿ (policy_is_shared(policy))
		uu = sugov_update_shared;
	ಅನ್ಯಥಾ ಯದಿ (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	ಅನ್ಯಥಾ
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		ಸಂರಚನಾ sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, ಪರಿಮಾಣ(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		ಸಂರಚನಾ sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		cpufreq_add_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_stop(ಸಂರಚನಾ cpufreq_policy *policy)
{
	ಸಂರಚನಾ sugov_policy *sg_policy = policy->governor_data;
	unsigned ಪೂರ್ಣಾಙ್ಕ cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	ಯದಿ (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_limits(ಸಂರಚನಾ cpufreq_policy *policy)
{
	ಸಂರಚನಾ sugov_policy *sg_policy = policy->governor_data;

	ಯದಿ (!policy->fast_switch_enabled) {
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

ಸ್ಥಿರ ಸಂರಚನಾ cpufreq_governor schedutil_gov = {
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
ಸಂರಚನಾ cpufreq_governor *cpufreq_default_governor(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ &schedutil_gov;
}
#endif

bool sugov_is_governor(ಸಂರಚನಾ cpufreq_policy *policy)
{
	ಪ್ರತ್ಯಾಗಮ policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
