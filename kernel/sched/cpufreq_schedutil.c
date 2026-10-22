// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/sched/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IOWAIT_BOOST_MIN	(SCHED_ਸਮਰੱਥਾ_SCALE / 8)

ਬਣਤਰ sugov_tunables {
	ਬਣਤਰ gov_attr_ਸੈੱਟ	attr_ਸੈੱਟ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਦਰ_ਹੱਦਬੰਦੀ_us;
};

ਬਣਤਰ sugov_policy {
	ਬਣਤਰ cpufreq_policy	*policy;

	ਬਣਤਰ sugov_tunables	*tunables;
	ਬਣਤਰ list_ਸਿਰ	tunables_hook;

	raw_spinlock_t		update_ਤਾਲਾ;
	u64			ਆਖਰੀ_freq_update_ਸਮਾਂ;
	s64			freq_update_delay_ns;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਅਗਲਾ_freq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	ਬਣਤਰ			irq_work irq_work;
	ਬਣਤਰ			kthread_work work;
	ਬਣਤਰ			mutex work_ਤਾਲਾ;
	ਬਣਤਰ			kthread_ਕਾਮਾ ਕਾਮਾ;
	ਬਣਤਰ task_struct	*thread;
	bool			work_ਵਿੱਚ_ਤਰੱਕੀ;

	bool			limits_ਬਦਲਿਆ;
	bool			need_freq_update;
};

ਬਣਤਰ sugov_cpu {
	ਬਣਤਰ update_util_ਅੰਕੜੇ	update_util;
	ਬਣਤਰ sugov_policy	*sg_policy;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		cpu;

	bool			iowait_boost_pending;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		iowait_boost;
	u64			ਆਖਰੀ_update;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		util;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		bw_min;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		bw_max;

	/* The field below is for single-CPU policies only: */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		saved_idle_calls;
#ਜੇ_ਅੰਤ
};

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

ਸਥਿਰ ਖਾਲੀ sugov_update_ਦਰ_ਹੱਦਬੰਦੀ_us(ਬਣਤਰ sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->ਦਰ_ਹੱਦਬੰਦੀ_us * NSEC_ਪ੍ਰਤੀ_USEC;
}

ਸਥਿਰ ਖਾਲੀ sugov_update_ਦਰ_ਹੱਦਬੰਦੀ_us_sync(ਬਣਤਰ sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_ਸੈੱਟ.update_ਤਾਲਾ);

	sugov_update_ਦਰ_ਹੱਦਬੰਦੀ_us(sg_policy);
}

ਸਥਿਰ bool sugov_should_update_freq(ਬਣਤਰ sugov_policy *sg_policy, u64 ਸਮਾਂ)
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
	ਜੇ (!cpufreq_this_cpu_can_update(sg_policy->policy))
		ਵਾਪਸ false;

	ਜੇ (unlikely(READ_ONCE(sg_policy->limits_ਬਦਲਿਆ))) {
		WRITE_ONCE(sg_policy->limits_ਬਦਲਿਆ, false);
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

		ਵਾਪਸ true;
	} ਨਹੀਂਤਾਂ ਜੇ (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		ਵਾਪਸ true;
	}

	delta_ns = ਸਮਾਂ - sg_policy->ਆਖਰੀ_freq_update_ਸਮਾਂ;

	ਵਾਪਸ delta_ns >= sg_policy->freq_update_delay_ns;
}

ਸਥਿਰ bool sugov_update_ਅਗਲਾ_freq(ਬਣਤਰ sugov_policy *sg_policy, u64 ਸਮਾਂ,
				   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_freq)
{
	ਜੇ (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		ਜੇ (sg_policy->ਅਗਲਾ_freq == ਅਗਲਾ_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			ਵਾਪਸ false;
	} ਨਹੀਂਤਾਂ ਜੇ (sg_policy->ਅਗਲਾ_freq == ਅਗਲਾ_freq) {
		ਵਾਪਸ false;
	}

	sg_policy->ਅਗਲਾ_freq = ਅਗਲਾ_freq;
	sg_policy->ਆਖਰੀ_freq_update_ਸਮਾਂ = ਸਮਾਂ;

	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ sugov_deferred_update(ਬਣਤਰ sugov_policy *sg_policy)
{
	ਜੇ (!sg_policy->work_ਵਿੱਚ_ਤਰੱਕੀ) {
		sg_policy->work_ਵਿੱਚ_ਤਰੱਕੀ = true;
		irq_work_ਕਤਾਰ(&sg_policy->irq_work);
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
ਸਥਿਰ __always_inline
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ get_ਸਮਰੱਥਾ_ref_freq(ਬਣਤਰ cpufreq_policy *policy)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ freq = arch_scale_freq_ref(policy->cpu);

	ਜੇ (freq)
		ਵਾਪਸ freq;

	ਜੇ (arch_scale_freq_invariant())
		ਵਾਪਸ policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	ਵਾਪਸ policy->cur + (policy->cur >> 2);
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਅਗਲਾ_freq(ਬਣਤਰ sugov_policy *sg_policy,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max)
{
	ਬਣਤਰ cpufreq_policy *policy = sg_policy->policy;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ freq;

	freq = get_ਸਮਰੱਥਾ_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	ਜੇ (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		ਵਾਪਸ sg_policy->ਅਗਲਾ_freq;

	sg_policy->cached_raw_freq = freq;
	ਵਾਪਸ cpufreq_driver_resolve_freq(policy, freq);
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sugov_effective_cpu_perf(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ actual,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ min,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	ਜੇ (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	ਵਾਪਸ max(min, max);
}

ਸਥਿਰ ਖਾਲੀ sugov_get_util(ਬਣਤਰ sugov_cpu *sg_cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ boost)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ min, max, util = scx_cpuperf_ਨਿਸ਼ਾਨਾ(sg_cpu->cpu);

	ਜੇ (!scx_switched_all())
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
ਸਥਿਰ bool sugov_iowait_reset(ਬਣਤਰ sugov_cpu *sg_cpu, u64 ਸਮਾਂ,
			       bool ਸੈੱਟ_iowait_boost)
{
	s64 delta_ns = ਸਮਾਂ - sg_cpu->ਆਖਰੀ_update;

	/* Reset boost only if a tick has elapsed since last request */
	ਜੇ (delta_ns <= TICK_NSEC)
		ਵਾਪਸ false;

	sg_cpu->iowait_boost = ਸੈੱਟ_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = ਸੈੱਟ_iowait_boost;

	ਵਾਪਸ true;
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
ਸਥਿਰ ਖਾਲੀ sugov_iowait_boost(ਬਣਤਰ sugov_cpu *sg_cpu, u64 ਸਮਾਂ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	bool ਸੈੱਟ_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	ਜੇ (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, ਸਮਾਂ, ਸੈੱਟ_iowait_boost))
		ਵਾਪਸ;

	/* Boost only tasks waking up after IO */
	ਜੇ (!ਸੈੱਟ_iowait_boost)
		ਵਾਪਸ;

	/* Ensure boost doubles only one time at each request */
	ਜੇ (sg_cpu->iowait_boost_pending)
		ਵਾਪਸ;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	ਜੇ (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, sg_cpu->iowait_boost << 1, SCHED_ਸਮਰੱਥਾ_SCALE);
		ਵਾਪਸ;
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sugov_iowait_apply(ਬਣਤਰ sugov_cpu *sg_cpu, u64 ਸਮਾਂ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_cap)
{
	/* No boost currently required */
	ਜੇ (!sg_cpu->iowait_boost)
		ਵਾਪਸ 0;

	/* Reset boost if the CPU appears to have been idle enough */
	ਜੇ (sugov_iowait_reset(sg_cpu, ਸਮਾਂ, false))
		ਵਾਪਸ 0;

	ਜੇ (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		ਜੇ (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			ਵਾਪਸ 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	ਵਾਪਸ (sg_cpu->iowait_boost * max_cap) >> SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
ਸਥਿਰ bool sugov_hold_freq(ਬਣਤਰ sugov_cpu *sg_cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	ਜੇ (scx_switched_all())
		ਵਾਪਸ false;

	/* if capped by uclamp_max, always update to be in compliance */
	ਜੇ (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		ਵਾਪਸ false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	ਵਾਪਸ ret;
}
#ਨਹੀਂਤਾਂ /* !CONFIG_NO_HZ_COMMON: */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool sugov_hold_freq(ਬਣਤਰ sugov_cpu *sg_cpu) { ਵਾਪਸ false; }
#ਜੇ_ਅੰਤ /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ignore_dl_ਦਰ_ਹੱਦਬੰਦੀ(ਬਣਤਰ sugov_cpu *sg_cpu)
{
	ਜੇ (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sugov_update_single_common(ਬਣਤਰ sugov_cpu *sg_cpu,
					      u64 ਸਮਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_cap,
					      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ boost;

	sugov_iowait_boost(sg_cpu, ਸਮਾਂ, flags);
	sg_cpu->ਆਖਰੀ_update = ਸਮਾਂ;

	ignore_dl_ਦਰ_ਹੱਦਬੰਦੀ(sg_cpu);

	ਜੇ (!sugov_should_update_freq(sg_cpu->sg_policy, ਸਮਾਂ))
		ਵਾਪਸ false;

	boost = sugov_iowait_apply(sg_cpu, ਸਮਾਂ, max_cap);
	sugov_get_util(sg_cpu, boost);

	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ sugov_update_single_freq(ਬਣਤਰ update_util_ਅੰਕੜੇ *hook, u64 ਸਮਾਂ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ sugov_cpu *sg_cpu = container_ਦਾ(hook, ਬਣਤਰ sugov_cpu, update_util);
	ਬਣਤਰ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cached_freq = sg_policy->cached_raw_freq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_cap;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_f;

	max_cap = arch_scale_cpu_ਸਮਰੱਥਾ(sg_cpu->cpu);

	ਜੇ (!sugov_update_single_common(sg_cpu, ਸਮਾਂ, max_cap, flags))
		ਵਾਪਸ;

	ਅਗਲਾ_f = get_ਅਗਲਾ_freq(sg_policy, sg_cpu->util, max_cap);

	ਜੇ (sugov_hold_freq(sg_cpu) && ਅਗਲਾ_f < sg_policy->ਅਗਲਾ_freq &&
	    !sg_policy->need_freq_update) {
		ਅਗਲਾ_f = sg_policy->ਅਗਲਾ_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	ਜੇ (!sugov_update_ਅਗਲਾ_freq(sg_policy, ਸਮਾਂ, ਅਗਲਾ_f))
		ਵਾਪਸ;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	ਜੇ (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, ਅਗਲਾ_f);
	} ਨਹੀਂਤਾਂ {
		raw_spin_ਤਾਲਾ(&sg_policy->update_ਤਾਲਾ);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_ਤਾਲਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ sugov_update_single_perf(ਬਣਤਰ update_util_ਅੰਕੜੇ *hook, u64 ਸਮਾਂ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ sugov_cpu *sg_cpu = container_ਦਾ(hook, ਬਣਤਰ sugov_cpu, update_util);
	ਬਣਤਰ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ prev_util = sg_cpu->util;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	ਜੇ (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, ਸਮਾਂ, flags);
		ਵਾਪਸ;
	}

	max_cap = arch_scale_cpu_ਸਮਰੱਥਾ(sg_cpu->cpu);

	ਜੇ (!sugov_update_single_common(sg_cpu, ਸਮਾਂ, max_cap, flags))
		ਵਾਪਸ;

	ਜੇ (sugov_hold_freq(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->ਆਖਰੀ_freq_update_ਸਮਾਂ = ਸਮਾਂ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sugov_ਅਗਲਾ_freq_shared(ਬਣਤਰ sugov_cpu *sg_cpu, u64 ਸਮਾਂ)
{
	ਬਣਤਰ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ਬਣਤਰ cpufreq_policy *policy = sg_policy->policy;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util = 0, max_cap;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j;

	max_cap = arch_scale_cpu_ਸਮਰੱਥਾ(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		ਬਣਤਰ sugov_cpu *j_sg_cpu = &ਪ੍ਰਤੀ_cpu(sugov_cpu, j);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ boost;

		boost = sugov_iowait_apply(j_sg_cpu, ਸਮਾਂ, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	ਵਾਪਸ get_ਅਗਲਾ_freq(sg_policy, util, max_cap);
}

ਸਥਿਰ ਖਾਲੀ
sugov_update_shared(ਬਣਤਰ update_util_ਅੰਕੜੇ *hook, u64 ਸਮਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ sugov_cpu *sg_cpu = container_ਦਾ(hook, ਬਣਤਰ sugov_cpu, update_util);
	ਬਣਤਰ sugov_policy *sg_policy = sg_cpu->sg_policy;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_f;

	raw_spin_ਤਾਲਾ(&sg_policy->update_ਤਾਲਾ);

	sugov_iowait_boost(sg_cpu, ਸਮਾਂ, flags);
	sg_cpu->ਆਖਰੀ_update = ਸਮਾਂ;

	ignore_dl_ਦਰ_ਹੱਦਬੰਦੀ(sg_cpu);

	ਜੇ (sugov_should_update_freq(sg_policy, ਸਮਾਂ)) {
		ਅਗਲਾ_f = sugov_ਅਗਲਾ_freq_shared(sg_cpu, ਸਮਾਂ);

		ਜੇ (!sugov_update_ਅਗਲਾ_freq(sg_policy, ਸਮਾਂ, ਅਗਲਾ_f))
			ਜਾਓ unlock;

		ਜੇ (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, ਅਗਲਾ_f);
		ਨਹੀਂਤਾਂ
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_ਤਾਲਾ);
}

ਸਥਿਰ ਖਾਲੀ sugov_work(ਬਣਤਰ kthread_work *work)
{
	ਬਣਤਰ sugov_policy *sg_policy = container_ਦਾ(work, ਬਣਤਰ sugov_policy, work);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ freq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

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
	raw_spin_ਤਾਲਾ_irqsave(&sg_policy->update_ਤਾਲਾ, flags);
	freq = sg_policy->ਅਗਲਾ_freq;
	sg_policy->work_ਵਿੱਚ_ਤਰੱਕੀ = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_ਤਾਲਾ, flags);

	mutex_ਤਾਲਾ(&sg_policy->work_ਤਾਲਾ);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_ਸਬੰਧ_L);
	mutex_unlock(&sg_policy->work_ਤਾਲਾ);
}

ਸਥਿਰ ਖਾਲੀ sugov_irq_work(ਬਣਤਰ irq_work *irq_work)
{
	ਬਣਤਰ sugov_policy *sg_policy;

	sg_policy = container_ਦਾ(irq_work, ਬਣਤਰ sugov_policy, irq_work);

	kthread_ਕਤਾਰ_work(&sg_policy->ਕਾਮਾ, &sg_policy->work);
}

/************************** sysfs interface ************************/

ਸਥਿਰ ਬਣਤਰ sugov_tunables *global_tunables;
ਸਥਿਰ DEFINE_MUTEX(global_tunables_ਤਾਲਾ);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sugov_tunables *ਮੰਜ਼ਿਲ_sugov_tunables(ਬਣਤਰ gov_attr_ਸੈੱਟ *attr_ਸੈੱਟ)
{
	ਵਾਪਸ container_ਦਾ(attr_ਸੈੱਟ, ਬਣਤਰ sugov_tunables, attr_ਸੈੱਟ);
}

ਸਥਿਰ ssize_t ਦਰ_ਹੱਦਬੰਦੀ_us_show(ਬਣਤਰ gov_attr_ਸੈੱਟ *attr_ਸੈੱਟ, ਅੱਖਰ *buf)
{
	ਬਣਤਰ sugov_tunables *tunables = ਮੰਜ਼ਿਲ_sugov_tunables(attr_ਸੈੱਟ);

	ਵਾਪਸ sysfs_emit(buf, "%u\n", tunables->ਦਰ_ਹੱਦਬੰਦੀ_us);
}

ਸਥਿਰ ssize_t
ਦਰ_ਹੱਦਬੰਦੀ_us_store(ਬਣਤਰ gov_attr_ਸੈੱਟ *attr_ਸੈੱਟ, ਅਚਲ ਅੱਖਰ *buf, ਮਾਪ_t ਗਿਣਤੀ_2)
{
	ਬਣਤਰ sugov_tunables *tunables = ਮੰਜ਼ਿਲ_sugov_tunables(attr_ਸੈੱਟ);
	ਬਣਤਰ sugov_policy *sg_policy;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਦਰ_ਹੱਦਬੰਦੀ_us;

	ਜੇ (kstrtouint(buf, 10, &ਦਰ_ਹੱਦਬੰਦੀ_us))
		ਵਾਪਸ -EINVAL;

	tunables->ਦਰ_ਹੱਦਬੰਦੀ_us = ਦਰ_ਹੱਦਬੰਦੀ_us;

	list_for_each_ਇੰਦਰਾਜ(sg_policy, &attr_ਸੈੱਟ->policy_list, tunables_hook)
		sugov_update_ਦਰ_ਹੱਦਬੰਦੀ_us(sg_policy);

	ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਸਥਿਰ ਬਣਤਰ governor_attr ਦਰ_ਹੱਦਬੰਦੀ_us = __ATTR_RW(ਦਰ_ਹੱਦਬੰਦੀ_us);

ਸਥਿਰ ਬਣਤਰ attribute *sugov_attrs[] = {
	&ਦਰ_ਹੱਦਬੰਦੀ_us.attr,
	ਨਿਰਾ
};
ATTRIBUTE_GROUPS(sugov);

ਸਥਿਰ ਖਾਲੀ sugov_tunables_free(ਬਣਤਰ kobject *kobj)
{
	ਬਣਤਰ gov_attr_ਸੈੱਟ *attr_ਸੈੱਟ = ਮੰਜ਼ਿਲ_gov_attr_ਸੈੱਟ(kobj);

	kfree(ਮੰਜ਼ਿਲ_sugov_tunables(attr_ਸੈੱਟ));
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

ਸਥਿਰ ਬਣਤਰ cpufreq_governor schedutil_gov;

ਸਥਿਰ ਬਣਤਰ sugov_policy *sugov_policy_alloc(ਬਣਤਰ cpufreq_policy *policy)
{
	ਬਣਤਰ sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	ਜੇ (!sg_policy)
		ਵਾਪਸ ਨਿਰਾ;

	sg_policy->policy = policy;
	raw_spin_ਤਾਲਾ_init(&sg_policy->update_ਤਾਲਾ);
	ਵਾਪਸ sg_policy;
}

ਸਥਿਰ ਖਾਲੀ sugov_policy_free(ਬਣਤਰ sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

ਸਥਿਰ ਪੂਰਨ sugov_kthread_create(ਬਣਤਰ sugov_policy *sg_policy)
{
	ਬਣਤਰ task_struct *thread;
	ਬਣਤਰ sched_attr attr = {
		.ਮਾਪ		= ਆਕਾਰ(ਬਣਤਰ sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_ਤਰਜੀਹ	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= NSEC_ਪ੍ਰਤੀ_MSEC,
		.sched_deadline = 10 * NSEC_ਪ੍ਰਤੀ_MSEC,
		.sched_period	= 10 * NSEC_ਪ੍ਰਤੀ_MSEC,
	};
	ਬਣਤਰ cpufreq_policy *policy = sg_policy->policy;
	ਪੂਰਨ ret;

	/* kthread only required for slow path */
	ਜੇ (policy->fast_switch_enabled)
		ਵਾਪਸ 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_ਕਾਮਾ(&sg_policy->ਕਾਮਾ);
	thread = kthread_create(kthread_ਕਾਮਾ_fn, &sg_policy->ਕਾਮਾ,
				"sugov:%d",
				cpumask_ਪਹਿਲਾ(policy->related_cpus));
	ਜੇ (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		ਵਾਪਸ PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	ਜੇ (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		ਵਾਪਸ ret;
	}

	sg_policy->thread = thread;
	ਜੇ (policy->dvfs_possible_from_any_cpu)
		ਸੈੱਟ_cpus_allowed_ptr(thread, policy->related_cpus);
	ਨਹੀਂਤਾਂ
		kthread_bind_ਨਕਾਬ(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_ਤਾਲਾ);

	wake_ਉੱਪਰ_process(thread);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ sugov_kthread_stop(ਬਣਤਰ sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	ਜੇ (sg_policy->policy->fast_switch_enabled)
		ਵਾਪਸ;

	kthread_flush_ਕਾਮਾ(&sg_policy->ਕਾਮਾ);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_ਤਾਲਾ);
}

ਸਥਿਰ ਬਣਤਰ sugov_tunables *sugov_tunables_alloc(ਬਣਤਰ sugov_policy *sg_policy)
{
	ਬਣਤਰ sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	ਜੇ (tunables) {
		gov_attr_ਸੈੱਟ_init(&tunables->attr_ਸੈੱਟ, &sg_policy->tunables_hook);
		ਜੇ (!have_governor_ਪ੍ਰਤੀ_policy())
			global_tunables = tunables;
	}
	ਵਾਪਸ tunables;
}

ਸਥਿਰ ਖਾਲੀ sugov_clear_global_tunables(ਖਾਲੀ)
{
	ਜੇ (!have_governor_ਪ੍ਰਤੀ_policy())
		global_tunables = ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ sugov_init(ਬਣਤਰ cpufreq_policy *policy)
{
	ਬਣਤਰ sugov_policy *sg_policy;
	ਬਣਤਰ sugov_tunables *tunables;
	ਪੂਰਨ ret = 0;

	/* State should be equivalent to EXIT */
	ਜੇ (policy->governor_ਅੰਕੜੇ)
		ਵਾਪਸ -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	ਜੇ (!sg_policy) {
		ret = -ENOMEM;
		ਜਾਓ disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	ਜੇ (ret)
		ਜਾਓ free_sg_policy;

	mutex_ਤਾਲਾ(&global_tunables_ਤਾਲਾ);

	ਜੇ (global_tunables) {
		ਜੇ (WARN_ON(have_governor_ਪ੍ਰਤੀ_policy())) {
			ret = -EINVAL;
			ਜਾਓ stop_kthread;
		}
		policy->governor_ਅੰਕੜੇ = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_ਸੈੱਟ_get(&global_tunables->attr_ਸੈੱਟ, &sg_policy->tunables_hook);
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	ਜੇ (!tunables) {
		ret = -ENOMEM;
		ਜਾਓ stop_kthread;
	}

	tunables->ਦਰ_ਹੱਦਬੰਦੀ_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_ਅੰਕੜੇ = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_ਜੋੜੋ(&tunables->attr_ਸੈੱਟ.kobj, &sugov_tunables_ktype,
				   get_governor_ਮਾਪਾ_kobj(policy), "%s",
				   schedutil_gov.name);
	ਜੇ (ret)
		ਜਾਓ fail;

ਬਾਹਰਲਾ:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_ਤਾਲਾ);
	ਵਾਪਸ 0;

fail:
	kobject_put(&tunables->attr_ਸੈੱਟ.kobj);
	policy->governor_ਅੰਕੜੇ = ਨਿਰਾ;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_ਤਾਲਾ);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ sugov_exit(ਬਣਤਰ cpufreq_policy *policy)
{
	ਬਣਤਰ sugov_policy *sg_policy = policy->governor_ਅੰਕੜੇ;
	ਬਣਤਰ sugov_tunables *tunables = sg_policy->tunables;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2;

	mutex_ਤਾਲਾ(&global_tunables_ਤਾਲਾ);

	ਗਿਣਤੀ_2 = gov_attr_ਸੈੱਟ_put(&tunables->attr_ਸੈੱਟ, &sg_policy->tunables_hook);
	policy->governor_ਅੰਕੜੇ = ਨਿਰਾ;
	ਜੇ (!ਗਿਣਤੀ_2)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_ਤਾਲਾ);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

ਸਥਿਰ ਪੂਰਨ sugov_ਸ਼ੁਰੂ(ਬਣਤਰ cpufreq_policy *policy)
{
	ਬਣਤਰ sugov_policy *sg_policy = policy->governor_ਅੰਕੜੇ;
	ਖਾਲੀ (*uu)(ਬਣਤਰ update_util_ਅੰਕੜੇ *ਅੰਕੜੇ, u64 ਸਮਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu;

	sugov_update_ਦਰ_ਹੱਦਬੰਦੀ_us_sync(sg_policy);
	sg_policy->ਆਖਰੀ_freq_update_ਸਮਾਂ	= 0;
	sg_policy->ਅਗਲਾ_freq			= 0;
	sg_policy->work_ਵਿੱਚ_ਤਰੱਕੀ		= false;
	sg_policy->limits_ਬਦਲਿਆ		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	ਜੇ (policy_is_shared(policy))
		uu = sugov_update_shared;
	ਨਹੀਂਤਾਂ ਜੇ (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	ਨਹੀਂਤਾਂ
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		ਬਣਤਰ sugov_cpu *sg_cpu = &ਪ੍ਰਤੀ_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, ਆਕਾਰ(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		ਬਣਤਰ sugov_cpu *sg_cpu = &ਪ੍ਰਤੀ_cpu(sugov_cpu, cpu);

		cpufreq_ਜੋੜੋ_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ sugov_stop(ਬਣਤਰ cpufreq_policy *policy)
{
	ਬਣਤਰ sugov_policy *sg_policy = policy->governor_ਅੰਕੜੇ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	ਜੇ (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

ਸਥਿਰ ਖਾਲੀ sugov_limits(ਬਣਤਰ cpufreq_policy *policy)
{
	ਬਣਤਰ sugov_policy *sg_policy = policy->governor_ਅੰਕੜੇ;

	ਜੇ (!policy->fast_switch_enabled) {
		mutex_ਤਾਲਾ(&sg_policy->work_ਤਾਲਾ);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_ਤਾਲਾ);
	}

	/*
	 * The limits_changed update below must take place before the updates
	 * of policy limits in cpufreq_set_policy() or a policy limits update
	 * might be missed, so use a memory barrier to ensure it.
	 *
	 * This pairs with the memory barrier in sugov_should_update_freq().
	 */
	smp_wmb();

	WRITE_ONCE(sg_policy->limits_ਬਦਲਿਆ, true);
}

ਸਥਿਰ ਬਣਤਰ cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.ਬਾਹਰ_ਨਿਕਲੋ			= sugov_exit,
	.ਸ਼ੁਰੂ			= sugov_ਸ਼ੁਰੂ,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
ਬਣਤਰ cpufreq_governor *cpufreq_default_governor(ਖਾਲੀ)
{
	ਵਾਪਸ &schedutil_gov;
}
#ਜੇ_ਅੰਤ

bool sugov_is_governor(ਬਣਤਰ cpufreq_policy *policy)
{
	ਵਾਪਸ policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
