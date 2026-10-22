// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#ಅನ್ತರ್ಭಾವ <uapi/linux/sched/types.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

#ವ್ಯಾಖ್ಯಾನಿಸು IOWAIT_BOOST_MIN	(SCHED_ಸಾಮರ್ಥ್ಯ_SCALE / 8)

ರಚನೆ sugov_tunables {
	ರಚನೆ gov_attr_ಗಣ	attr_ಗಣ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ದರ_ಎಲ್ಲೆ_us;
};

ರಚನೆ sugov_policy {
	ರಚನೆ cpufreq_policy	*policy;

	ರಚನೆ sugov_tunables	*tunables;
	ರಚನೆ list_ತಲೆ	tunables_hook;

	raw_spinlock_t		update_ಬೀಗ;
	u64			ಕೊನೆಯ_freq_update_ಸಮಯ;
	s64			freq_update_delay_ns;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ಮುಂದಿನ_freq;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	ರಚನೆ			irq_work irq_work;
	ರಚನೆ			kthread_work work;
	ರಚನೆ			mutex work_ಬೀಗ;
	ರಚನೆ			kthread_ಕೆಲಸಗಾರ ಕೆಲಸಗಾರ;
	ರಚನೆ task_struct	*thread;
	bool			work_ಒಳ_ಪ್ರಗತಿ;

	bool			limits_ಬದಲಾಯಿತು;
	bool			need_freq_update;
};

ರಚನೆ sugov_cpu {
	ರಚನೆ update_util_ದತ್ತಾಂಶ	update_util;
	ರಚನೆ sugov_policy	*sg_policy;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		cpu;

	bool			iowait_boost_pending;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		iowait_boost;
	u64			ಕೊನೆಯ_update;

	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		util;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		bw_min;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		bw_max;

	/* The field below is for single-CPU policies only: */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		saved_idle_calls;
#ಆದರೆ_ಅಂತ್ಯ
};

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_ದರ_ಎಲ್ಲೆ_us(ರಚನೆ sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->ದರ_ಎಲ್ಲೆ_us * NSEC_ಪ್ರತಿ_USEC;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_ದರ_ಎಲ್ಲೆ_us_sync(ರಚನೆ sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_ಗಣ.update_ಬೀಗ);

	sugov_update_ದರ_ಎಲ್ಲೆ_us(sg_policy);
}

ಸ್ಥಿರ bool sugov_should_update_freq(ರಚನೆ sugov_policy *sg_policy, u64 ಸಮಯ)
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
	ಆದರೆ (!cpufreq_this_cpu_can_update(sg_policy->policy))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (unlikely(READ_ONCE(sg_policy->limits_ಬದಲಾಯಿತು))) {
		WRITE_ONCE(sg_policy->limits_ಬದಲಾಯಿತು, false);
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

		ಹಿಂತಿರುಗಿ true;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		ಹಿಂತಿರುಗಿ true;
	}

	delta_ns = ಸಮಯ - sg_policy->ಕೊನೆಯ_freq_update_ಸಮಯ;

	ಹಿಂತಿರುಗಿ delta_ns >= sg_policy->freq_update_delay_ns;
}

ಸ್ಥಿರ bool sugov_update_ಮುಂದಿನ_freq(ರಚನೆ sugov_policy *sg_policy, u64 ಸಮಯ,
				   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮುಂದಿನ_freq)
{
	ಆದರೆ (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		ಆದರೆ (sg_policy->ಮುಂದಿನ_freq == ಮುಂದಿನ_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			ಹಿಂತಿರುಗಿ false;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sg_policy->ಮುಂದಿನ_freq == ಮುಂದಿನ_freq) {
		ಹಿಂತಿರುಗಿ false;
	}

	sg_policy->ಮುಂದಿನ_freq = ಮುಂದಿನ_freq;
	sg_policy->ಕೊನೆಯ_freq_update_ಸಮಯ = ಸಮಯ;

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_deferred_update(ರಚನೆ sugov_policy *sg_policy)
{
	ಆದರೆ (!sg_policy->work_ಒಳ_ಪ್ರಗತಿ) {
		sg_policy->work_ಒಳ_ಪ್ರಗತಿ = true;
		irq_work_ಸರತಿ(&sg_policy->irq_work);
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
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ get_ಸಾಮರ್ಥ್ಯ_ref_freq(ರಚನೆ cpufreq_policy *policy)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ freq = arch_scale_freq_ref(policy->cpu);

	ಆದರೆ (freq)
		ಹಿಂತಿರುಗಿ freq;

	ಆದರೆ (arch_scale_freq_invariant())
		ಹಿಂತಿರುಗಿ policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	ಹಿಂತಿರುಗಿ policy->cur + (policy->cur >> 2);
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ get_ಮುಂದಿನ_freq(ರಚನೆ sugov_policy *sg_policy,
				  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max)
{
	ರಚನೆ cpufreq_policy *policy = sg_policy->policy;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ freq;

	freq = get_ಸಾಮರ್ಥ್ಯ_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	ಆದರೆ (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		ಹಿಂತಿರುಗಿ sg_policy->ಮುಂದಿನ_freq;

	sg_policy->cached_raw_freq = freq;
	ಹಿಂತಿರುಗಿ cpufreq_driver_resolve_freq(policy, freq);
}

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sugov_effective_cpu_perf(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ actual,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ min,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	ಆದರೆ (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	ಹಿಂತಿರುಗಿ max(min, max);
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_get_util(ರಚನೆ sugov_cpu *sg_cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ boost)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ min, max, util = scx_cpuperf_ಗುರಿ(sg_cpu->cpu);

	ಆದರೆ (!scx_switched_all())
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
ಸ್ಥಿರ bool sugov_iowait_reset(ರಚನೆ sugov_cpu *sg_cpu, u64 ಸಮಯ,
			       bool ಗಣ_iowait_boost)
{
	s64 delta_ns = ಸಮಯ - sg_cpu->ಕೊನೆಯ_update;

	/* Reset boost only if a tick has elapsed since last request */
	ಆದರೆ (delta_ns <= TICK_NSEC)
		ಹಿಂತಿರುಗಿ false;

	sg_cpu->iowait_boost = ಗಣ_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = ಗಣ_iowait_boost;

	ಹಿಂತಿರುಗಿ true;
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
ಸ್ಥಿರ ಶೂನ್ಯ sugov_iowait_boost(ರಚನೆ sugov_cpu *sg_cpu, u64 ಸಮಯ,
			       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	bool ಗಣ_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	ಆದರೆ (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, ಸಮಯ, ಗಣ_iowait_boost))
		ಹಿಂತಿರುಗಿ;

	/* Boost only tasks waking up after IO */
	ಆದರೆ (!ಗಣ_iowait_boost)
		ಹಿಂತಿರುಗಿ;

	/* Ensure boost doubles only one time at each request */
	ಆದರೆ (sg_cpu->iowait_boost_pending)
		ಹಿಂತಿರುಗಿ;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	ಆದರೆ (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, sg_cpu->iowait_boost << 1, SCHED_ಸಾಮರ್ಥ್ಯ_SCALE);
		ಹಿಂತಿರುಗಿ;
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sugov_iowait_apply(ರಚನೆ sugov_cpu *sg_cpu, u64 ಸಮಯ,
			       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_cap)
{
	/* No boost currently required */
	ಆದರೆ (!sg_cpu->iowait_boost)
		ಹಿಂತಿರುಗಿ 0;

	/* Reset boost if the CPU appears to have been idle enough */
	ಆದರೆ (sugov_iowait_reset(sg_cpu, ಸಮಯ, false))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		ಆದರೆ (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	ಹಿಂತಿರುಗಿ (sg_cpu->iowait_boost * max_cap) >> SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
ಸ್ಥಿರ bool sugov_hold_freq(ರಚನೆ sugov_cpu *sg_cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	ಆದರೆ (scx_switched_all())
		ಹಿಂತಿರುಗಿ false;

	/* if capped by uclamp_max, always update to be in compliance */
	ಆದರೆ (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	ಹಿಂತಿರುಗಿ ret;
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NO_HZ_COMMON: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sugov_hold_freq(ರಚನೆ sugov_cpu *sg_cpu) { ಹಿಂತಿರುಗಿ false; }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ignore_dl_ದರ_ಎಲ್ಲೆ(ರಚನೆ sugov_cpu *sg_cpu)
{
	ಆದರೆ (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sugov_update_single_common(ರಚನೆ sugov_cpu *sg_cpu,
					      u64 ಸಮಯ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_cap,
					      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ boost;

	sugov_iowait_boost(sg_cpu, ಸಮಯ, flags);
	sg_cpu->ಕೊನೆಯ_update = ಸಮಯ;

	ignore_dl_ದರ_ಎಲ್ಲೆ(sg_cpu);

	ಆದರೆ (!sugov_should_update_freq(sg_cpu->sg_policy, ಸಮಯ))
		ಹಿಂತಿರುಗಿ false;

	boost = sugov_iowait_apply(sg_cpu, ಸಮಯ, max_cap);
	sugov_get_util(sg_cpu, boost);

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_single_freq(ರಚನೆ update_util_ದತ್ತಾಂಶ *hook, u64 ಸಮಯ,
				     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sugov_cpu *sg_cpu = container_ದ(hook, ರಚನೆ sugov_cpu, update_util);
	ರಚನೆ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cached_freq = sg_policy->cached_raw_freq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_cap;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮುಂದಿನ_f;

	max_cap = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(sg_cpu->cpu);

	ಆದರೆ (!sugov_update_single_common(sg_cpu, ಸಮಯ, max_cap, flags))
		ಹಿಂತಿರುಗಿ;

	ಮುಂದಿನ_f = get_ಮುಂದಿನ_freq(sg_policy, sg_cpu->util, max_cap);

	ಆದರೆ (sugov_hold_freq(sg_cpu) && ಮುಂದಿನ_f < sg_policy->ಮುಂದಿನ_freq &&
	    !sg_policy->need_freq_update) {
		ಮುಂದಿನ_f = sg_policy->ಮುಂದಿನ_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	ಆದರೆ (!sugov_update_ಮುಂದಿನ_freq(sg_policy, ಸಮಯ, ಮುಂದಿನ_f))
		ಹಿಂತಿರುಗಿ;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	ಆದರೆ (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, ಮುಂದಿನ_f);
	} ಇಲ್ಲದಿದ್ದರೆ {
		raw_spin_ಬೀಗ(&sg_policy->update_ಬೀಗ);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_ಬೀಗ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_update_single_perf(ರಚನೆ update_util_ದತ್ತಾಂಶ *hook, u64 ಸಮಯ,
				     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sugov_cpu *sg_cpu = container_ದ(hook, ರಚನೆ sugov_cpu, update_util);
	ರಚನೆ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ prev_util = sg_cpu->util;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	ಆದರೆ (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, ಸಮಯ, flags);
		ಹಿಂತಿರುಗಿ;
	}

	max_cap = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(sg_cpu->cpu);

	ಆದರೆ (!sugov_update_single_common(sg_cpu, ಸಮಯ, max_cap, flags))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (sugov_hold_freq(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->ಕೊನೆಯ_freq_update_ಸಮಯ = ಸಮಯ;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sugov_ಮುಂದಿನ_freq_shared(ರಚನೆ sugov_cpu *sg_cpu, u64 ಸಮಯ)
{
	ರಚನೆ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ರಚನೆ cpufreq_policy *policy = sg_policy->policy;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util = 0, max_cap;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ j;

	max_cap = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		ರಚನೆ sugov_cpu *j_sg_cpu = &ಪ್ರತಿ_cpu(sugov_cpu, j);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ boost;

		boost = sugov_iowait_apply(j_sg_cpu, ಸಮಯ, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	ಹಿಂತಿರುಗಿ get_ಮುಂದಿನ_freq(sg_policy, util, max_cap);
}

ಸ್ಥಿರ ಶೂನ್ಯ
sugov_update_shared(ರಚನೆ update_util_ದತ್ತಾಂಶ *hook, u64 ಸಮಯ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sugov_cpu *sg_cpu = container_ದ(hook, ರಚನೆ sugov_cpu, update_util);
	ರಚನೆ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮುಂದಿನ_f;

	raw_spin_ಬೀಗ(&sg_policy->update_ಬೀಗ);

	sugov_iowait_boost(sg_cpu, ಸಮಯ, flags);
	sg_cpu->ಕೊನೆಯ_update = ಸಮಯ;

	ignore_dl_ದರ_ಎಲ್ಲೆ(sg_cpu);

	ಆದರೆ (sugov_should_update_freq(sg_policy, ಸಮಯ)) {
		ಮುಂದಿನ_f = sugov_ಮುಂದಿನ_freq_shared(sg_cpu, ಸಮಯ);

		ಆದರೆ (!sugov_update_ಮುಂದಿನ_freq(sg_policy, ಸಮಯ, ಮುಂದಿನ_f))
			ಹೋಗು unlock;

		ಆದರೆ (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, ಮುಂದಿನ_f);
		ಇಲ್ಲದಿದ್ದರೆ
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_ಬೀಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_work(ರಚನೆ kthread_work *work)
{
	ರಚನೆ sugov_policy *sg_policy = container_ದ(work, ರಚನೆ sugov_policy, work);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ freq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

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
	raw_spin_ಬೀಗ_irqsave(&sg_policy->update_ಬೀಗ, flags);
	freq = sg_policy->ಮುಂದಿನ_freq;
	sg_policy->work_ಒಳ_ಪ್ರಗತಿ = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_ಬೀಗ, flags);

	mutex_ಬೀಗ(&sg_policy->work_ಬೀಗ);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_ಸಂಬಂಧ_L);
	mutex_unlock(&sg_policy->work_ಬೀಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_irq_work(ರಚನೆ irq_work *irq_work)
{
	ರಚನೆ sugov_policy *sg_policy;

	sg_policy = container_ದ(irq_work, ರಚನೆ sugov_policy, irq_work);

	kthread_ಸರತಿ_work(&sg_policy->ಕೆಲಸಗಾರ, &sg_policy->work);
}

/************************** sysfs interface ************************/

ಸ್ಥಿರ ರಚನೆ sugov_tunables *global_tunables;
ಸ್ಥಿರ DEFINE_MUTEX(global_tunables_ಬೀಗ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sugov_tunables *ಗಮ್ಯ_sugov_tunables(ರಚನೆ gov_attr_ಗಣ *attr_ಗಣ)
{
	ಹಿಂತಿರುಗಿ container_ದ(attr_ಗಣ, ರಚನೆ sugov_tunables, attr_ಗಣ);
}

ಸ್ಥಿರ ssize_t ದರ_ಎಲ್ಲೆ_us_show(ರಚನೆ gov_attr_ಗಣ *attr_ಗಣ, ಅಕ್ಷರ *buf)
{
	ರಚನೆ sugov_tunables *tunables = ಗಮ್ಯ_sugov_tunables(attr_ಗಣ);

	ಹಿಂತಿರುಗಿ sysfs_emit(buf, "%u\n", tunables->ದರ_ಎಲ್ಲೆ_us);
}

ಸ್ಥಿರ ssize_t
ದರ_ಎಲ್ಲೆ_us_store(ರಚನೆ gov_attr_ಗಣ *attr_ಗಣ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf, ಗಾತ್ರ_t ಎಣಿಕೆ_2)
{
	ರಚನೆ sugov_tunables *tunables = ಗಮ್ಯ_sugov_tunables(attr_ಗಣ);
	ರಚನೆ sugov_policy *sg_policy;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ದರ_ಎಲ್ಲೆ_us;

	ಆದರೆ (kstrtouint(buf, 10, &ದರ_ಎಲ್ಲೆ_us))
		ಹಿಂತಿರುಗಿ -EINVAL;

	tunables->ದರ_ಎಲ್ಲೆ_us = ದರ_ಎಲ್ಲೆ_us;

	list_for_each_ನಮೂದು(sg_policy, &attr_ಗಣ->policy_list, tunables_hook)
		sugov_update_ದರ_ಎಲ್ಲೆ_us(sg_policy);

	ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

ಸ್ಥಿರ ರಚನೆ governor_attr ದರ_ಎಲ್ಲೆ_us = __ATTR_RW(ದರ_ಎಲ್ಲೆ_us);

ಸ್ಥಿರ ರಚನೆ attribute *sugov_attrs[] = {
	&ದರ_ಎಲ್ಲೆ_us.attr,
	ಖಾಲಿ
};
ATTRIBUTE_GROUPS(sugov);

ಸ್ಥಿರ ಶೂನ್ಯ sugov_tunables_free(ರಚನೆ kobject *kobj)
{
	ರಚನೆ gov_attr_ಗಣ *attr_ಗಣ = ಗಮ್ಯ_gov_attr_ಗಣ(kobj);

	kfree(ಗಮ್ಯ_sugov_tunables(attr_ಗಣ));
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

ಸ್ಥಿರ ರಚನೆ cpufreq_governor schedutil_gov;

ಸ್ಥಿರ ರಚನೆ sugov_policy *sugov_policy_alloc(ರಚನೆ cpufreq_policy *policy)
{
	ರಚನೆ sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	ಆದರೆ (!sg_policy)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	sg_policy->policy = policy;
	raw_spin_ಬೀಗ_init(&sg_policy->update_ಬೀಗ);
	ಹಿಂತಿರುಗಿ sg_policy;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_policy_free(ರಚನೆ sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sugov_kthread_create(ರಚನೆ sugov_policy *sg_policy)
{
	ರಚನೆ task_struct *thread;
	ರಚನೆ sched_attr attr = {
		.ಗಾತ್ರ_2		= ಗಾತ್ರ(ರಚನೆ sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_ಆದ್ಯತೆ	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= NSEC_ಪ್ರತಿ_MSEC,
		.sched_deadline = 10 * NSEC_ಪ್ರತಿ_MSEC,
		.sched_period	= 10 * NSEC_ಪ್ರತಿ_MSEC,
	};
	ರಚನೆ cpufreq_policy *policy = sg_policy->policy;
	ಪೂರ್ಣಾಂಕ ret;

	/* kthread only required for slow path */
	ಆದರೆ (policy->fast_switch_enabled)
		ಹಿಂತಿರುಗಿ 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_ಕೆಲಸಗಾರ(&sg_policy->ಕೆಲಸಗಾರ);
	thread = kthread_create(kthread_ಕೆಲಸಗಾರ_fn, &sg_policy->ಕೆಲಸಗಾರ,
				"sugov:%d",
				cpumask_ಮೊದಲ(policy->related_cpus));
	ಆದರೆ (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		ಹಿಂತಿರುಗಿ PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	ಆದರೆ (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		ಹಿಂತಿರುಗಿ ret;
	}

	sg_policy->thread = thread;
	ಆದರೆ (policy->dvfs_possible_from_any_cpu)
		ಗಣ_cpus_allowed_ptr(thread, policy->related_cpus);
	ಇಲ್ಲದಿದ್ದರೆ
		kthread_bind_ಮುಖವಾಡ(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_ಬೀಗ);

	wake_ಮೇಲೆ_process(thread);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_kthread_stop(ರಚನೆ sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	ಆದರೆ (sg_policy->policy->fast_switch_enabled)
		ಹಿಂತಿರುಗಿ;

	kthread_flush_ಕೆಲಸಗಾರ(&sg_policy->ಕೆಲಸಗಾರ);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_ಬೀಗ);
}

ಸ್ಥಿರ ರಚನೆ sugov_tunables *sugov_tunables_alloc(ರಚನೆ sugov_policy *sg_policy)
{
	ರಚನೆ sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	ಆದರೆ (tunables) {
		gov_attr_ಗಣ_init(&tunables->attr_ಗಣ, &sg_policy->tunables_hook);
		ಆದರೆ (!have_governor_ಪ್ರತಿ_policy())
			global_tunables = tunables;
	}
	ಹಿಂತಿರುಗಿ tunables;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_clear_global_tunables(ಶೂನ್ಯ)
{
	ಆದರೆ (!have_governor_ಪ್ರತಿ_policy())
		global_tunables = ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sugov_init(ರಚನೆ cpufreq_policy *policy)
{
	ರಚನೆ sugov_policy *sg_policy;
	ರಚನೆ sugov_tunables *tunables;
	ಪೂರ್ಣಾಂಕ ret = 0;

	/* State should be equivalent to EXIT */
	ಆದರೆ (policy->governor_ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	ಆದರೆ (!sg_policy) {
		ret = -ENOMEM;
		ಹೋಗು disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	ಆದರೆ (ret)
		ಹೋಗು free_sg_policy;

	mutex_ಬೀಗ(&global_tunables_ಬೀಗ);

	ಆದರೆ (global_tunables) {
		ಆದರೆ (WARN_ON(have_governor_ಪ್ರತಿ_policy())) {
			ret = -EINVAL;
			ಹೋಗು stop_kthread;
		}
		policy->governor_ದತ್ತಾಂಶ = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_ಗಣ_get(&global_tunables->attr_ಗಣ, &sg_policy->tunables_hook);
		ಹೋಗು ಹೊರಹೋಗು;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	ಆದರೆ (!tunables) {
		ret = -ENOMEM;
		ಹೋಗು stop_kthread;
	}

	tunables->ದರ_ಎಲ್ಲೆ_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_ದತ್ತಾಂಶ = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_ಕೂಡಿಸು(&tunables->attr_ಗಣ.kobj, &sugov_tunables_ktype,
				   get_governor_ಪೋಷಕ_kobj(policy), "%s",
				   schedutil_gov.name);
	ಆದರೆ (ret)
		ಹೋಗು fail;

ಹೊರಹೋಗು:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_ಬೀಗ);
	ಹಿಂತಿರುಗಿ 0;

fail:
	kobject_put(&tunables->attr_ಗಣ.kobj);
	policy->governor_ದತ್ತಾಂಶ = ಖಾಲಿ;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_ಬೀಗ);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_exit(ರಚನೆ cpufreq_policy *policy)
{
	ರಚನೆ sugov_policy *sg_policy = policy->governor_ದತ್ತಾಂಶ;
	ರಚನೆ sugov_tunables *tunables = sg_policy->tunables;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;

	mutex_ಬೀಗ(&global_tunables_ಬೀಗ);

	ಎಣಿಕೆ_2 = gov_attr_ಗಣ_put(&tunables->attr_ಗಣ, &sg_policy->tunables_hook);
	policy->governor_ದತ್ತಾಂಶ = ಖಾಲಿ;
	ಆದರೆ (!ಎಣಿಕೆ_2)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_ಬೀಗ);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sugov_ಆರಂಭ(ರಚನೆ cpufreq_policy *policy)
{
	ರಚನೆ sugov_policy *sg_policy = policy->governor_ದತ್ತಾಂಶ;
	ಶೂನ್ಯ (*uu)(ರಚನೆ update_util_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, u64 ಸಮಯ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu;

	sugov_update_ದರ_ಎಲ್ಲೆ_us_sync(sg_policy);
	sg_policy->ಕೊನೆಯ_freq_update_ಸಮಯ	= 0;
	sg_policy->ಮುಂದಿನ_freq			= 0;
	sg_policy->work_ಒಳ_ಪ್ರಗತಿ		= false;
	sg_policy->limits_ಬದಲಾಯಿತು		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	ಆದರೆ (policy_is_shared(policy))
		uu = sugov_update_shared;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	ಇಲ್ಲದಿದ್ದರೆ
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		ರಚನೆ sugov_cpu *sg_cpu = &ಪ್ರತಿ_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, ಗಾತ್ರ(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		ರಚನೆ sugov_cpu *sg_cpu = &ಪ್ರತಿ_cpu(sugov_cpu, cpu);

		cpufreq_ಕೂಡಿಸು_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_stop(ರಚನೆ cpufreq_policy *policy)
{
	ರಚನೆ sugov_policy *sg_policy = policy->governor_ದತ್ತಾಂಶ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	ಆದರೆ (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sugov_limits(ರಚನೆ cpufreq_policy *policy)
{
	ರಚನೆ sugov_policy *sg_policy = policy->governor_ದತ್ತಾಂಶ;

	ಆದರೆ (!policy->fast_switch_enabled) {
		mutex_ಬೀಗ(&sg_policy->work_ಬೀಗ);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_ಬೀಗ);
	}

	/*
	 * The limits_changed update below must take place before the updates
	 * of policy limits in cpufreq_set_policy() or a policy limits update
	 * might be missed, so use a memory barrier to ensure it.
	 *
	 * This pairs with the memory barrier in sugov_should_update_freq().
	 */
	smp_wmb();

	WRITE_ONCE(sg_policy->limits_ಬದಲಾಯಿತು, true);
}

ಸ್ಥಿರ ರಚನೆ cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.ನಿರ್ಗಮಿಸು			= sugov_exit,
	.ಆರಂಭ			= sugov_ಆರಂಭ,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
ರಚನೆ cpufreq_governor *cpufreq_default_governor(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ &schedutil_gov;
}
#ಆದರೆ_ಅಂತ್ಯ

bool sugov_is_governor(ರಚನೆ cpufreq_policy *policy)
{
	ಹಿಂತಿರುಗಿ policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
