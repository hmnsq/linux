// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#అన్తర్భావ <uapi/linux/sched/types.h>
#అన్తర్భావ "sched.h"

#నిర్వచించు IOWAIT_BOOST_MIN	(SCHED_సామర్థ్యం_SCALE / 8)

నిర్మాణం sugov_tunables {
	నిర్మాణం gov_attr_సమితి	attr_సమితి;
	గుర్తులేని పూర్ణాంకం		రేటు_హద్దు_us;
};

నిర్మాణం sugov_policy {
	నిర్మాణం cpufreq_policy	*policy;

	నిర్మాణం sugov_tunables	*tunables;
	నిర్మాణం list_తల	tunables_hook;

	raw_spinlock_t		update_తాళం;
	u64			చివరి_freq_update_సమయం;
	s64			freq_update_delay_ns;
	గుర్తులేని పూర్ణాంకం		తదుపరి_freq;
	గుర్తులేని పూర్ణాంకం		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	నిర్మాణం			irq_work irq_work;
	నిర్మాణం			kthread_work work;
	నిర్మాణం			mutex work_తాళం;
	నిర్మాణం			kthread_కార్మికుడు కార్మికుడు;
	నిర్మాణం task_struct	*thread;
	bool			work_లోపల_పురోగతి;

	bool			limits_మారింది;
	bool			need_freq_update;
};

నిర్మాణం sugov_cpu {
	నిర్మాణం update_util_దత్తాంశం	update_util;
	నిర్మాణం sugov_policy	*sg_policy;
	గుర్తులేని పూర్ణాంకం		cpu;

	bool			iowait_boost_pending;
	గుర్తులేని పూర్ణాంకం		iowait_boost;
	u64			చివరి_update;

	గుర్తులేని దీర్ఘ		util;
	గుర్తులేని దీర్ఘ		bw_min;
	గుర్తులేని దీర్ఘ		bw_max;

	/* The field below is for single-CPU policies only: */
#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
	గుర్తులేని దీర్ఘ		saved_idle_calls;
#అయితే_ముగింపు
};

స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

స్థిర శూన్య sugov_update_రేటు_హద్దు_us(నిర్మాణం sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->రేటు_హద్దు_us * NSEC_చొప్పున_USEC;
}

స్థిర శూన్య sugov_update_రేటు_హద్దు_us_sync(నిర్మాణం sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_సమితి.update_తాళం);

	sugov_update_రేటు_హద్దు_us(sg_policy);
}

స్థిర bool sugov_should_update_freq(నిర్మాణం sugov_policy *sg_policy, u64 సమయం)
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
	ఒకవేళ (!cpufreq_this_cpu_can_update(sg_policy->policy))
		తిరిగి false;

	ఒకవేళ (unlikely(READ_ONCE(sg_policy->limits_మారింది))) {
		WRITE_ONCE(sg_policy->limits_మారింది, false);
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

		తిరిగి true;
	} లేకపోతే ఒకవేళ (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		తిరిగి true;
	}

	delta_ns = సమయం - sg_policy->చివరి_freq_update_సమయం;

	తిరిగి delta_ns >= sg_policy->freq_update_delay_ns;
}

స్థిర bool sugov_update_తదుపరి_freq(నిర్మాణం sugov_policy *sg_policy, u64 సమయం,
				   గుర్తులేని పూర్ణాంకం తదుపరి_freq)
{
	ఒకవేళ (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		ఒకవేళ (sg_policy->తదుపరి_freq == తదుపరి_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			తిరిగి false;
	} లేకపోతే ఒకవేళ (sg_policy->తదుపరి_freq == తదుపరి_freq) {
		తిరిగి false;
	}

	sg_policy->తదుపరి_freq = తదుపరి_freq;
	sg_policy->చివరి_freq_update_సమయం = సమయం;

	తిరిగి true;
}

స్థిర శూన్య sugov_deferred_update(నిర్మాణం sugov_policy *sg_policy)
{
	ఒకవేళ (!sg_policy->work_లోపల_పురోగతి) {
		sg_policy->work_లోపల_పురోగతి = true;
		irq_work_వరుస(&sg_policy->irq_work);
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
స్థిర __always_inline
గుర్తులేని దీర్ఘ get_సామర్థ్యం_ref_freq(నిర్మాణం cpufreq_policy *policy)
{
	గుర్తులేని పూర్ణాంకం freq = arch_scale_freq_ref(policy->cpu);

	ఒకవేళ (freq)
		తిరిగి freq;

	ఒకవేళ (arch_scale_freq_invariant())
		తిరిగి policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	తిరిగి policy->cur + (policy->cur >> 2);
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
స్థిర గుర్తులేని పూర్ణాంకం get_తదుపరి_freq(నిర్మాణం sugov_policy *sg_policy,
				  గుర్తులేని దీర్ఘ util, గుర్తులేని దీర్ఘ max)
{
	నిర్మాణం cpufreq_policy *policy = sg_policy->policy;
	గుర్తులేని పూర్ణాంకం freq;

	freq = get_సామర్థ్యం_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	ఒకవేళ (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		తిరిగి sg_policy->తదుపరి_freq;

	sg_policy->cached_raw_freq = freq;
	తిరిగి cpufreq_driver_resolve_freq(policy, freq);
}

గుర్తులేని దీర్ఘ sugov_effective_cpu_perf(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ actual,
				 గుర్తులేని దీర్ఘ min,
				 గుర్తులేని దీర్ఘ max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	ఒకవేళ (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	తిరిగి max(min, max);
}

స్థిర శూన్య sugov_get_util(నిర్మాణం sugov_cpu *sg_cpu, గుర్తులేని దీర్ఘ boost)
{
	గుర్తులేని దీర్ఘ min, max, util = scx_cpuperf_లక్ష్యం(sg_cpu->cpu);

	ఒకవేళ (!scx_switched_all())
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
స్థిర bool sugov_iowait_reset(నిర్మాణం sugov_cpu *sg_cpu, u64 సమయం,
			       bool సమితి_iowait_boost)
{
	s64 delta_ns = సమయం - sg_cpu->చివరి_update;

	/* Reset boost only if a tick has elapsed since last request */
	ఒకవేళ (delta_ns <= TICK_NSEC)
		తిరిగి false;

	sg_cpu->iowait_boost = సమితి_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = సమితి_iowait_boost;

	తిరిగి true;
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
స్థిర శూన్య sugov_iowait_boost(నిర్మాణం sugov_cpu *sg_cpu, u64 సమయం,
			       గుర్తులేని పూర్ణాంకం flags)
{
	bool సమితి_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	ఒకవేళ (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, సమయం, సమితి_iowait_boost))
		తిరిగి;

	/* Boost only tasks waking up after IO */
	ఒకవేళ (!సమితి_iowait_boost)
		తిరిగి;

	/* Ensure boost doubles only one time at each request */
	ఒకవేళ (sg_cpu->iowait_boost_pending)
		తిరిగి;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	ఒకవేళ (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(గుర్తులేని పూర్ణాంకం, sg_cpu->iowait_boost << 1, SCHED_సామర్థ్యం_SCALE);
		తిరిగి;
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
స్థిర గుర్తులేని దీర్ఘ sugov_iowait_apply(నిర్మాణం sugov_cpu *sg_cpu, u64 సమయం,
			       గుర్తులేని దీర్ఘ max_cap)
{
	/* No boost currently required */
	ఒకవేళ (!sg_cpu->iowait_boost)
		తిరిగి 0;

	/* Reset boost if the CPU appears to have been idle enough */
	ఒకవేళ (sugov_iowait_reset(sg_cpu, సమయం, false))
		తిరిగి 0;

	ఒకవేళ (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		ఒకవేళ (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			తిరిగి 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	తిరిగి (sg_cpu->iowait_boost * max_cap) >> SCHED_సామర్థ్యం_జరుపు;
}

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
స్థిర bool sugov_hold_freq(నిర్మాణం sugov_cpu *sg_cpu)
{
	గుర్తులేని దీర్ఘ idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	ఒకవేళ (scx_switched_all())
		తిరిగి false;

	/* if capped by uclamp_max, always update to be in compliance */
	ఒకవేళ (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		తిరిగి false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	తిరిగి ret;
}
#లేకపోతే /* !CONFIG_NO_HZ_COMMON: */
స్థిర అంతఃవరుస bool sugov_hold_freq(నిర్మాణం sugov_cpu *sg_cpu) { తిరిగి false; }
#అయితే_ముగింపు /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
స్థిర అంతఃవరుస శూన్య ignore_dl_రేటు_హద్దు(నిర్మాణం sugov_cpu *sg_cpu)
{
	ఒకవేళ (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

స్థిర అంతఃవరుస bool sugov_update_single_common(నిర్మాణం sugov_cpu *sg_cpu,
					      u64 సమయం, గుర్తులేని దీర్ఘ max_cap,
					      గుర్తులేని పూర్ణాంకం flags)
{
	గుర్తులేని దీర్ఘ boost;

	sugov_iowait_boost(sg_cpu, సమయం, flags);
	sg_cpu->చివరి_update = సమయం;

	ignore_dl_రేటు_హద్దు(sg_cpu);

	ఒకవేళ (!sugov_should_update_freq(sg_cpu->sg_policy, సమయం))
		తిరిగి false;

	boost = sugov_iowait_apply(sg_cpu, సమయం, max_cap);
	sugov_get_util(sg_cpu, boost);

	తిరిగి true;
}

స్థిర శూన్య sugov_update_single_freq(నిర్మాణం update_util_దత్తాంశం *hook, u64 సమయం,
				     గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం sugov_cpu *sg_cpu = container_యొక్క(hook, నిర్మాణం sugov_cpu, update_util);
	నిర్మాణం sugov_policy *sg_policy = sg_cpu->sg_policy;
	గుర్తులేని పూర్ణాంకం cached_freq = sg_policy->cached_raw_freq;
	గుర్తులేని దీర్ఘ max_cap;
	గుర్తులేని పూర్ణాంకం తదుపరి_f;

	max_cap = arch_scale_cpu_సామర్థ్యం(sg_cpu->cpu);

	ఒకవేళ (!sugov_update_single_common(sg_cpu, సమయం, max_cap, flags))
		తిరిగి;

	తదుపరి_f = get_తదుపరి_freq(sg_policy, sg_cpu->util, max_cap);

	ఒకవేళ (sugov_hold_freq(sg_cpu) && తదుపరి_f < sg_policy->తదుపరి_freq &&
	    !sg_policy->need_freq_update) {
		తదుపరి_f = sg_policy->తదుపరి_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	ఒకవేళ (!sugov_update_తదుపరి_freq(sg_policy, సమయం, తదుపరి_f))
		తిరిగి;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	ఒకవేళ (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, తదుపరి_f);
	} లేకపోతే {
		raw_spin_తాళం(&sg_policy->update_తాళం);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_తాళం);
	}
}

స్థిర శూన్య sugov_update_single_perf(నిర్మాణం update_util_దత్తాంశం *hook, u64 సమయం,
				     గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం sugov_cpu *sg_cpu = container_యొక్క(hook, నిర్మాణం sugov_cpu, update_util);
	నిర్మాణం sugov_policy *sg_policy = sg_cpu->sg_policy;
	గుర్తులేని దీర్ఘ prev_util = sg_cpu->util;
	గుర్తులేని దీర్ఘ max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	ఒకవేళ (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, సమయం, flags);
		తిరిగి;
	}

	max_cap = arch_scale_cpu_సామర్థ్యం(sg_cpu->cpu);

	ఒకవేళ (!sugov_update_single_common(sg_cpu, సమయం, max_cap, flags))
		తిరిగి;

	ఒకవేళ (sugov_hold_freq(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->చివరి_freq_update_సమయం = సమయం;
}

స్థిర గుర్తులేని పూర్ణాంకం sugov_తదుపరి_freq_shared(నిర్మాణం sugov_cpu *sg_cpu, u64 సమయం)
{
	నిర్మాణం sugov_policy *sg_policy = sg_cpu->sg_policy;
	నిర్మాణం cpufreq_policy *policy = sg_policy->policy;
	గుర్తులేని దీర్ఘ util = 0, max_cap;
	గుర్తులేని పూర్ణాంకం j;

	max_cap = arch_scale_cpu_సామర్థ్యం(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		నిర్మాణం sugov_cpu *j_sg_cpu = &చొప్పున_cpu(sugov_cpu, j);
		గుర్తులేని దీర్ఘ boost;

		boost = sugov_iowait_apply(j_sg_cpu, సమయం, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	తిరిగి get_తదుపరి_freq(sg_policy, util, max_cap);
}

స్థిర శూన్య
sugov_update_shared(నిర్మాణం update_util_దత్తాంశం *hook, u64 సమయం, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం sugov_cpu *sg_cpu = container_యొక్క(hook, నిర్మాణం sugov_cpu, update_util);
	నిర్మాణం sugov_policy *sg_policy = sg_cpu->sg_policy;
	గుర్తులేని పూర్ణాంకం తదుపరి_f;

	raw_spin_తాళం(&sg_policy->update_తాళం);

	sugov_iowait_boost(sg_cpu, సమయం, flags);
	sg_cpu->చివరి_update = సమయం;

	ignore_dl_రేటు_హద్దు(sg_cpu);

	ఒకవేళ (sugov_should_update_freq(sg_policy, సమయం)) {
		తదుపరి_f = sugov_తదుపరి_freq_shared(sg_cpu, సమయం);

		ఒకవేళ (!sugov_update_తదుపరి_freq(sg_policy, సమయం, తదుపరి_f))
			వెళ్ళు unlock;

		ఒకవేళ (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, తదుపరి_f);
		లేకపోతే
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_తాళం);
}

స్థిర శూన్య sugov_work(నిర్మాణం kthread_work *work)
{
	నిర్మాణం sugov_policy *sg_policy = container_యొక్క(work, నిర్మాణం sugov_policy, work);
	గుర్తులేని పూర్ణాంకం freq;
	గుర్తులేని దీర్ఘ flags;

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
	raw_spin_తాళం_irqsave(&sg_policy->update_తాళం, flags);
	freq = sg_policy->తదుపరి_freq;
	sg_policy->work_లోపల_పురోగతి = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_తాళం, flags);

	mutex_తాళం(&sg_policy->work_తాళం);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_సంబంధం_L);
	mutex_unlock(&sg_policy->work_తాళం);
}

స్థిర శూన్య sugov_irq_work(నిర్మాణం irq_work *irq_work)
{
	నిర్మాణం sugov_policy *sg_policy;

	sg_policy = container_యొక్క(irq_work, నిర్మాణం sugov_policy, irq_work);

	kthread_వరుస_work(&sg_policy->కార్మికుడు, &sg_policy->work);
}

/************************** sysfs interface ************************/

స్థిర నిర్మాణం sugov_tunables *global_tunables;
స్థిర DEFINE_MUTEX(global_tunables_తాళం);

స్థిర అంతఃవరుస నిర్మాణం sugov_tunables *గమ్యం_sugov_tunables(నిర్మాణం gov_attr_సమితి *attr_సమితి)
{
	తిరిగి container_యొక్క(attr_సమితి, నిర్మాణం sugov_tunables, attr_సమితి);
}

స్థిర ssize_t రేటు_హద్దు_us_show(నిర్మాణం gov_attr_సమితి *attr_సమితి, అక్షరం *buf)
{
	నిర్మాణం sugov_tunables *tunables = గమ్యం_sugov_tunables(attr_సమితి);

	తిరిగి sysfs_emit(buf, "%u\n", tunables->రేటు_హద్దు_us);
}

స్థిర ssize_t
రేటు_హద్దు_us_store(నిర్మాణం gov_attr_సమితి *attr_సమితి, స్థిరరాశి అక్షరం *buf, కొలత_t లెక్క)
{
	నిర్మాణం sugov_tunables *tunables = గమ్యం_sugov_tunables(attr_సమితి);
	నిర్మాణం sugov_policy *sg_policy;
	గుర్తులేని పూర్ణాంకం రేటు_హద్దు_us;

	ఒకవేళ (kstrtouint(buf, 10, &రేటు_హద్దు_us))
		తిరిగి -EINVAL;

	tunables->రేటు_హద్దు_us = రేటు_హద్దు_us;

	list_for_each_నమోదు(sg_policy, &attr_సమితి->policy_list, tunables_hook)
		sugov_update_రేటు_హద్దు_us(sg_policy);

	తిరిగి లెక్క;
}

స్థిర నిర్మాణం governor_attr రేటు_హద్దు_us = __ATTR_RW(రేటు_హద్దు_us);

స్థిర నిర్మాణం attribute *sugov_attrs[] = {
	&రేటు_హద్దు_us.attr,
	ఖాళీ
};
ATTRIBUTE_GROUPS(sugov);

స్థిర శూన్య sugov_tunables_free(నిర్మాణం kobject *kobj)
{
	నిర్మాణం gov_attr_సమితి *attr_సమితి = గమ్యం_gov_attr_సమితి(kobj);

	kfree(గమ్యం_sugov_tunables(attr_సమితి));
}

స్థిర స్థిరరాశి నిర్మాణం kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

స్థిర నిర్మాణం cpufreq_governor schedutil_gov;

స్థిర నిర్మాణం sugov_policy *sugov_policy_alloc(నిర్మాణం cpufreq_policy *policy)
{
	నిర్మాణం sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	ఒకవేళ (!sg_policy)
		తిరిగి ఖాళీ;

	sg_policy->policy = policy;
	raw_spin_తాళం_init(&sg_policy->update_తాళం);
	తిరిగి sg_policy;
}

స్థిర శూన్య sugov_policy_free(నిర్మాణం sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

స్థిర పూర్ణాంకం sugov_kthread_create(నిర్మాణం sugov_policy *sg_policy)
{
	నిర్మాణం task_struct *thread;
	నిర్మాణం sched_attr attr = {
		.కొలత		= పరిమాణం(నిర్మాణం sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_ప్రాధాన్యత	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= NSEC_చొప్పున_MSEC,
		.sched_deadline = 10 * NSEC_చొప్పున_MSEC,
		.sched_period	= 10 * NSEC_చొప్పున_MSEC,
	};
	నిర్మాణం cpufreq_policy *policy = sg_policy->policy;
	పూర్ణాంకం ret;

	/* kthread only required for slow path */
	ఒకవేళ (policy->fast_switch_enabled)
		తిరిగి 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_కార్మికుడు(&sg_policy->కార్మికుడు);
	thread = kthread_create(kthread_కార్మికుడు_fn, &sg_policy->కార్మికుడు,
				"sugov:%d",
				cpumask_మొదటి(policy->related_cpus));
	ఒకవేళ (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		తిరిగి PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	ఒకవేళ (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		తిరిగి ret;
	}

	sg_policy->thread = thread;
	ఒకవేళ (policy->dvfs_possible_from_any_cpu)
		సమితి_cpus_allowed_ptr(thread, policy->related_cpus);
	లేకపోతే
		kthread_bind_ముసుగు(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_తాళం);

	wake_పైకి_process(thread);

	తిరిగి 0;
}

స్థిర శూన్య sugov_kthread_stop(నిర్మాణం sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	ఒకవేళ (sg_policy->policy->fast_switch_enabled)
		తిరిగి;

	kthread_flush_కార్మికుడు(&sg_policy->కార్మికుడు);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_తాళం);
}

స్థిర నిర్మాణం sugov_tunables *sugov_tunables_alloc(నిర్మాణం sugov_policy *sg_policy)
{
	నిర్మాణం sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	ఒకవేళ (tunables) {
		gov_attr_సమితి_init(&tunables->attr_సమితి, &sg_policy->tunables_hook);
		ఒకవేళ (!have_governor_చొప్పున_policy())
			global_tunables = tunables;
	}
	తిరిగి tunables;
}

స్థిర శూన్య sugov_clear_global_tunables(శూన్య)
{
	ఒకవేళ (!have_governor_చొప్పున_policy())
		global_tunables = ఖాళీ;
}

స్థిర పూర్ణాంకం sugov_init(నిర్మాణం cpufreq_policy *policy)
{
	నిర్మాణం sugov_policy *sg_policy;
	నిర్మాణం sugov_tunables *tunables;
	పూర్ణాంకం ret = 0;

	/* State should be equivalent to EXIT */
	ఒకవేళ (policy->governor_దత్తాంశం)
		తిరిగి -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	ఒకవేళ (!sg_policy) {
		ret = -ENOMEM;
		వెళ్ళు disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	ఒకవేళ (ret)
		వెళ్ళు free_sg_policy;

	mutex_తాళం(&global_tunables_తాళం);

	ఒకవేళ (global_tunables) {
		ఒకవేళ (WARN_ON(have_governor_చొప్పున_policy())) {
			ret = -EINVAL;
			వెళ్ళు stop_kthread;
		}
		policy->governor_దత్తాంశం = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_సమితి_get(&global_tunables->attr_సమితి, &sg_policy->tunables_hook);
		వెళ్ళు బహిర్గతం;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	ఒకవేళ (!tunables) {
		ret = -ENOMEM;
		వెళ్ళు stop_kthread;
	}

	tunables->రేటు_హద్దు_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_దత్తాంశం = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_కూడు(&tunables->attr_సమితి.kobj, &sugov_tunables_ktype,
				   get_governor_మాతృ_kobj(policy), "%s",
				   schedutil_gov.name);
	ఒకవేళ (ret)
		వెళ్ళు fail;

బహిర్గతం:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_తాళం);
	తిరిగి 0;

fail:
	kobject_put(&tunables->attr_సమితి.kobj);
	policy->governor_దత్తాంశం = ఖాళీ;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_తాళం);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	తిరిగి ret;
}

స్థిర శూన్య sugov_exit(నిర్మాణం cpufreq_policy *policy)
{
	నిర్మాణం sugov_policy *sg_policy = policy->governor_దత్తాంశం;
	నిర్మాణం sugov_tunables *tunables = sg_policy->tunables;
	గుర్తులేని పూర్ణాంకం లెక్క;

	mutex_తాళం(&global_tunables_తాళం);

	లెక్క = gov_attr_సమితి_put(&tunables->attr_సమితి, &sg_policy->tunables_hook);
	policy->governor_దత్తాంశం = ఖాళీ;
	ఒకవేళ (!లెక్క)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_తాళం);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

స్థిర పూర్ణాంకం sugov_ప్రారంభం(నిర్మాణం cpufreq_policy *policy)
{
	నిర్మాణం sugov_policy *sg_policy = policy->governor_దత్తాంశం;
	శూన్య (*uu)(నిర్మాణం update_util_దత్తాంశం *దత్తాంశం, u64 సమయం, గుర్తులేని పూర్ణాంకం flags);
	గుర్తులేని పూర్ణాంకం cpu;

	sugov_update_రేటు_హద్దు_us_sync(sg_policy);
	sg_policy->చివరి_freq_update_సమయం	= 0;
	sg_policy->తదుపరి_freq			= 0;
	sg_policy->work_లోపల_పురోగతి		= false;
	sg_policy->limits_మారింది		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	ఒకవేళ (policy_is_shared(policy))
		uu = sugov_update_shared;
	లేకపోతే ఒకవేళ (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	లేకపోతే
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		నిర్మాణం sugov_cpu *sg_cpu = &చొప్పున_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, పరిమాణం(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		నిర్మాణం sugov_cpu *sg_cpu = &చొప్పున_cpu(sugov_cpu, cpu);

		cpufreq_కూడు_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	తిరిగి 0;
}

స్థిర శూన్య sugov_stop(నిర్మాణం cpufreq_policy *policy)
{
	నిర్మాణం sugov_policy *sg_policy = policy->governor_దత్తాంశం;
	గుర్తులేని పూర్ణాంకం cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	ఒకవేళ (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

స్థిర శూన్య sugov_limits(నిర్మాణం cpufreq_policy *policy)
{
	నిర్మాణం sugov_policy *sg_policy = policy->governor_దత్తాంశం;

	ఒకవేళ (!policy->fast_switch_enabled) {
		mutex_తాళం(&sg_policy->work_తాళం);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_తాళం);
	}

	/*
	 * The limits_changed update below must take place before the updates
	 * of policy limits in cpufreq_set_policy() or a policy limits update
	 * might be missed, so use a memory barrier to ensure it.
	 *
	 * This pairs with the memory barrier in sugov_should_update_freq().
	 */
	smp_wmb();

	WRITE_ONCE(sg_policy->limits_మారింది, true);
}

స్థిర నిర్మాణం cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.నిష్క్రమించు			= sugov_exit,
	.ప్రారంభం			= sugov_ప్రారంభం,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#నిర్వచితమైతే CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
నిర్మాణం cpufreq_governor *cpufreq_default_governor(శూన్య)
{
	తిరిగి &schedutil_gov;
}
#అయితే_ముగింపు

bool sugov_is_governor(నిర్మాణం cpufreq_policy *policy)
{
	తిరిగి policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
