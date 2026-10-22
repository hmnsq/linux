// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#अन्तर्भाव <uapi/linux/sched/types.h>
#अन्तर्भाव "sched.h"

#परिभाषय IOWAIT_BOOST_MIN	(SCHED_क्षमता_SCALE / 8)

संरचना sugov_tunables {
	संरचना gov_attr_समुच्चयः	attr_समुच्चयः;
	अचिह्नित पूर्णाङ्क		दरः_सीमा_us;
};

संरचना sugov_policy {
	संरचना cpufreq_policy	*policy;

	संरचना sugov_tunables	*tunables;
	संरचना list_शीर्ष	tunables_hook;

	raw_spinlock_t		update_तालकम्;
	u64			चरमः_freq_update_time;
	s64			freq_update_delay_ns;
	अचिह्नित पूर्णाङ्क		अग्र_freq;
	अचिह्नित पूर्णाङ्क		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	संरचना			irq_work irq_work;
	संरचना			kthread_work work;
	संरचना			mutex work_तालकम्;
	संरचना			kthread_कार्यकर्ता कार्यकर्ता;
	संरचना task_struct	*thread;
	bool			work_अन्तर्_प्रगतिः;

	bool			limits_परिवर्तितम्;
	bool			need_freq_update;
};

संरचना sugov_cpu {
	संरचना update_util_दत्त	update_util;
	संरचना sugov_policy	*sg_policy;
	अचिह्नित पूर्णाङ्क		cpu;

	bool			iowait_boost_pending;
	अचिह्नित पूर्णाङ्क		iowait_boost;
	u64			चरमः_update;

	अचिह्नित दीर्घ		util;
	अचिह्नित दीर्घ		bw_min;
	अचिह्नित दीर्घ		bw_max;

	/* The field below is for single-CPU policies only: */
#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
	अचिह्नित दीर्घ		saved_idle_calls;
#यदि_अन्तः
};

स्थिर DEFINE_प्रत्येकम्_CPU(संरचना sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

स्थिर शून्य sugov_update_दरः_सीमा_us(संरचना sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->दरः_सीमा_us * NSEC_प्रत्येकम्_USEC;
}

स्थिर शून्य sugov_update_दरः_सीमा_us_sync(संरचना sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_समुच्चयः.update_तालकम्);

	sugov_update_दरः_सीमा_us(sg_policy);
}

स्थिर bool sugov_should_update_freq(संरचना sugov_policy *sg_policy, u64 time)
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
	यदि (!cpufreq_this_cpu_can_update(sg_policy->policy))
		प्रत्यागम false;

	यदि (unlikely(READ_ONCE(sg_policy->limits_परिवर्तितम्))) {
		WRITE_ONCE(sg_policy->limits_परिवर्तितम्, false);
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

		प्रत्यागम true;
	} अन्यथा यदि (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		प्रत्यागम true;
	}

	delta_ns = time - sg_policy->चरमः_freq_update_time;

	प्रत्यागम delta_ns >= sg_policy->freq_update_delay_ns;
}

स्थिर bool sugov_update_अग्र_freq(संरचना sugov_policy *sg_policy, u64 time,
				   अचिह्नित पूर्णाङ्क अग्र_freq)
{
	यदि (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		यदि (sg_policy->अग्र_freq == अग्र_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			प्रत्यागम false;
	} अन्यथा यदि (sg_policy->अग्र_freq == अग्र_freq) {
		प्रत्यागम false;
	}

	sg_policy->अग्र_freq = अग्र_freq;
	sg_policy->चरमः_freq_update_time = time;

	प्रत्यागम true;
}

स्थिर शून्य sugov_deferred_update(संरचना sugov_policy *sg_policy)
{
	यदि (!sg_policy->work_अन्तर्_प्रगतिः) {
		sg_policy->work_अन्तर्_प्रगतिः = true;
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
स्थिर __always_inline
अचिह्नित दीर्घ get_क्षमता_ref_freq(संरचना cpufreq_policy *policy)
{
	अचिह्नित पूर्णाङ्क freq = arch_scale_freq_ref(policy->cpu);

	यदि (freq)
		प्रत्यागम freq;

	यदि (arch_scale_freq_invariant())
		प्रत्यागम policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	प्रत्यागम policy->cur + (policy->cur >> 2);
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
स्थिर अचिह्नित पूर्णाङ्क get_अग्र_freq(संरचना sugov_policy *sg_policy,
				  अचिह्नित दीर्घ util, अचिह्नित दीर्घ max)
{
	संरचना cpufreq_policy *policy = sg_policy->policy;
	अचिह्नित पूर्णाङ्क freq;

	freq = get_क्षमता_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	यदि (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		प्रत्यागम sg_policy->अग्र_freq;

	sg_policy->cached_raw_freq = freq;
	प्रत्यागम cpufreq_driver_resolve_freq(policy, freq);
}

अचिह्नित दीर्घ sugov_effective_cpu_perf(पूर्णाङ्क cpu, अचिह्नित दीर्घ actual,
				 अचिह्नित दीर्घ min,
				 अचिह्नित दीर्घ max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	यदि (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	प्रत्यागम max(min, max);
}

स्थिर शून्य sugov_get_util(संरचना sugov_cpu *sg_cpu, अचिह्नित दीर्घ boost)
{
	अचिह्नित दीर्घ min, max, util = scx_cpuperf_target(sg_cpu->cpu);

	यदि (!scx_switched_all())
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
स्थिर bool sugov_iowait_reset(संरचना sugov_cpu *sg_cpu, u64 time,
			       bool समुच्चयः_iowait_boost)
{
	s64 delta_ns = time - sg_cpu->चरमः_update;

	/* Reset boost only if a tick has elapsed since last request */
	यदि (delta_ns <= TICK_NSEC)
		प्रत्यागम false;

	sg_cpu->iowait_boost = समुच्चयः_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = समुच्चयः_iowait_boost;

	प्रत्यागम true;
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
स्थिर शून्य sugov_iowait_boost(संरचना sugov_cpu *sg_cpu, u64 time,
			       अचिह्नित पूर्णाङ्क flags)
{
	bool समुच्चयः_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	यदि (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, time, समुच्चयः_iowait_boost))
		प्रत्यागम;

	/* Boost only tasks waking up after IO */
	यदि (!समुच्चयः_iowait_boost)
		प्रत्यागम;

	/* Ensure boost doubles only one time at each request */
	यदि (sg_cpu->iowait_boost_pending)
		प्रत्यागम;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	यदि (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(अचिह्नित पूर्णाङ्क, sg_cpu->iowait_boost << 1, SCHED_क्षमता_SCALE);
		प्रत्यागम;
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
स्थिर अचिह्नित दीर्घ sugov_iowait_apply(संरचना sugov_cpu *sg_cpu, u64 time,
			       अचिह्नित दीर्घ max_cap)
{
	/* No boost currently required */
	यदि (!sg_cpu->iowait_boost)
		प्रत्यागम 0;

	/* Reset boost if the CPU appears to have been idle enough */
	यदि (sugov_iowait_reset(sg_cpu, time, false))
		प्रत्यागम 0;

	यदि (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		यदि (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			प्रत्यागम 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	प्रत्यागम (sg_cpu->iowait_boost * max_cap) >> SCHED_क्षमता_सरणम्;
}

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
स्थिर bool sugov_hold_freq(संरचना sugov_cpu *sg_cpu)
{
	अचिह्नित दीर्घ idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	यदि (scx_switched_all())
		प्रत्यागम false;

	/* if capped by uclamp_max, always update to be in compliance */
	यदि (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		प्रत्यागम false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	प्रत्यागम ret;
}
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
स्थिर अन्तःस्थ bool sugov_hold_freq(संरचना sugov_cpu *sg_cpu) { प्रत्यागम false; }
#यदि_अन्तः /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
स्थिर अन्तःस्थ शून्य ignore_dl_दरः_सीमा(संरचना sugov_cpu *sg_cpu)
{
	यदि (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

स्थिर अन्तःस्थ bool sugov_update_single_common(संरचना sugov_cpu *sg_cpu,
					      u64 time, अचिह्नित दीर्घ max_cap,
					      अचिह्नित पूर्णाङ्क flags)
{
	अचिह्नित दीर्घ boost;

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->चरमः_update = time;

	ignore_dl_दरः_सीमा(sg_cpu);

	यदि (!sugov_should_update_freq(sg_cpu->sg_policy, time))
		प्रत्यागम false;

	boost = sugov_iowait_apply(sg_cpu, time, max_cap);
	sugov_get_util(sg_cpu, boost);

	प्रत्यागम true;
}

स्थिर शून्य sugov_update_single_freq(संरचना update_util_दत्त *hook, u64 time,
				     अचिह्नित पूर्णाङ्क flags)
{
	संरचना sugov_cpu *sg_cpu = container_सम्बन्धि(hook, संरचना sugov_cpu, update_util);
	संरचना sugov_policy *sg_policy = sg_cpu->sg_policy;
	अचिह्नित पूर्णाङ्क cached_freq = sg_policy->cached_raw_freq;
	अचिह्नित दीर्घ max_cap;
	अचिह्नित पूर्णाङ्क अग्र_f;

	max_cap = arch_scale_cpu_क्षमता(sg_cpu->cpu);

	यदि (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		प्रत्यागम;

	अग्र_f = get_अग्र_freq(sg_policy, sg_cpu->util, max_cap);

	यदि (sugov_hold_freq(sg_cpu) && अग्र_f < sg_policy->अग्र_freq &&
	    !sg_policy->need_freq_update) {
		अग्र_f = sg_policy->अग्र_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	यदि (!sugov_update_अग्र_freq(sg_policy, time, अग्र_f))
		प्रत्यागम;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	यदि (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, अग्र_f);
	} अन्यथा {
		raw_spin_तालकम्(&sg_policy->update_तालकम्);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_तालकम्);
	}
}

स्थिर शून्य sugov_update_single_perf(संरचना update_util_दत्त *hook, u64 time,
				     अचिह्नित पूर्णाङ्क flags)
{
	संरचना sugov_cpu *sg_cpu = container_सम्बन्धि(hook, संरचना sugov_cpu, update_util);
	संरचना sugov_policy *sg_policy = sg_cpu->sg_policy;
	अचिह्नित दीर्घ पूर्व_util = sg_cpu->util;
	अचिह्नित दीर्घ max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	यदि (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, time, flags);
		प्रत्यागम;
	}

	max_cap = arch_scale_cpu_क्षमता(sg_cpu->cpu);

	यदि (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		प्रत्यागम;

	यदि (sugov_hold_freq(sg_cpu) && sg_cpu->util < पूर्व_util)
		sg_cpu->util = पूर्व_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->चरमः_freq_update_time = time;
}

स्थिर अचिह्नित पूर्णाङ्क sugov_अग्र_freq_shared(संरचना sugov_cpu *sg_cpu, u64 time)
{
	संरचना sugov_policy *sg_policy = sg_cpu->sg_policy;
	संरचना cpufreq_policy *policy = sg_policy->policy;
	अचिह्नित दीर्घ util = 0, max_cap;
	अचिह्नित पूर्णाङ्क j;

	max_cap = arch_scale_cpu_क्षमता(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		संरचना sugov_cpu *j_sg_cpu = &प्रत्येकम्_cpu(sugov_cpu, j);
		अचिह्नित दीर्घ boost;

		boost = sugov_iowait_apply(j_sg_cpu, time, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	प्रत्यागम get_अग्र_freq(sg_policy, util, max_cap);
}

स्थिर शून्य
sugov_update_shared(संरचना update_util_दत्त *hook, u64 time, अचिह्नित पूर्णाङ्क flags)
{
	संरचना sugov_cpu *sg_cpu = container_सम्बन्धि(hook, संरचना sugov_cpu, update_util);
	संरचना sugov_policy *sg_policy = sg_cpu->sg_policy;
	अचिह्नित पूर्णाङ्क अग्र_f;

	raw_spin_तालकम्(&sg_policy->update_तालकम्);

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->चरमः_update = time;

	ignore_dl_दरः_सीमा(sg_cpu);

	यदि (sugov_should_update_freq(sg_policy, time)) {
		अग्र_f = sugov_अग्र_freq_shared(sg_cpu, time);

		यदि (!sugov_update_अग्र_freq(sg_policy, time, अग्र_f))
			गच्छ unlock;

		यदि (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, अग्र_f);
		अन्यथा
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_तालकम्);
}

स्थिर शून्य sugov_work(संरचना kthread_work *work)
{
	संरचना sugov_policy *sg_policy = container_सम्बन्धि(work, संरचना sugov_policy, work);
	अचिह्नित पूर्णाङ्क freq;
	अचिह्नित दीर्घ flags;

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
	raw_spin_तालकम्_irqsave(&sg_policy->update_तालकम्, flags);
	freq = sg_policy->अग्र_freq;
	sg_policy->work_अन्तर्_प्रगतिः = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_तालकम्, flags);

	mutex_तालकम्(&sg_policy->work_तालकम्);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_सम्बन्धः_L);
	mutex_unlock(&sg_policy->work_तालकम्);
}

स्थिर शून्य sugov_irq_work(संरचना irq_work *irq_work)
{
	संरचना sugov_policy *sg_policy;

	sg_policy = container_सम्बन्धि(irq_work, संरचना sugov_policy, irq_work);

	kthread_queue_work(&sg_policy->कार्यकर्ता, &sg_policy->work);
}

/************************** sysfs interface ************************/

स्थिर संरचना sugov_tunables *global_tunables;
स्थिर DEFINE_MUTEX(global_tunables_तालकम्);

स्थिर अन्तःस्थ संरचना sugov_tunables *गन्तव्यम्_sugov_tunables(संरचना gov_attr_समुच्चयः *attr_समुच्चयः)
{
	प्रत्यागम container_सम्बन्धि(attr_समुच्चयः, संरचना sugov_tunables, attr_समुच्चयः);
}

स्थिर ssize_t दरः_सीमा_us_show(संरचना gov_attr_समुच्चयः *attr_समुच्चयः, वर्ण *buf)
{
	संरचना sugov_tunables *tunables = गन्तव्यम्_sugov_tunables(attr_समुच्चयः);

	प्रत्यागम sysfs_emit(buf, "%u\n", tunables->दरः_सीमा_us);
}

स्थिर ssize_t
दरः_सीमा_us_store(संरचना gov_attr_समुच्चयः *attr_समुच्चयः, ध्रुव वर्ण *buf, आकार_t गणनम्)
{
	संरचना sugov_tunables *tunables = गन्तव्यम्_sugov_tunables(attr_समुच्चयः);
	संरचना sugov_policy *sg_policy;
	अचिह्नित पूर्णाङ्क दरः_सीमा_us;

	यदि (kstrtouint(buf, 10, &दरः_सीमा_us))
		प्रत्यागम -EINVAL;

	tunables->दरः_सीमा_us = दरः_सीमा_us;

	list_for_each_प्रविष्टिः(sg_policy, &attr_समुच्चयः->policy_list, tunables_hook)
		sugov_update_दरः_सीमा_us(sg_policy);

	प्रत्यागम गणनम्;
}

स्थिर संरचना governor_attr दरः_सीमा_us = __ATTR_RW(दरः_सीमा_us);

स्थिर संरचना attribute *sugov_attrs[] = {
	&दरः_सीमा_us.attr,
	रिक्त
};
ATTRIBUTE_GROUPS(sugov);

स्थिर शून्य sugov_tunables_free(संरचना kobject *kobj)
{
	संरचना gov_attr_समुच्चयः *attr_समुच्चयः = गन्तव्यम्_gov_attr_समुच्चयः(kobj);

	kfree(गन्तव्यम्_sugov_tunables(attr_समुच्चयः));
}

स्थिर ध्रुव संरचना kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

स्थिर संरचना cpufreq_governor schedutil_gov;

स्थिर संरचना sugov_policy *sugov_policy_alloc(संरचना cpufreq_policy *policy)
{
	संरचना sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	यदि (!sg_policy)
		प्रत्यागम रिक्त;

	sg_policy->policy = policy;
	raw_spin_तालकम्_init(&sg_policy->update_तालकम्);
	प्रत्यागम sg_policy;
}

स्थिर शून्य sugov_policy_free(संरचना sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

स्थिर पूर्णाङ्क sugov_kthread_create(संरचना sugov_policy *sg_policy)
{
	संरचना task_struct *thread;
	संरचना sched_attr attr = {
		.आकार		= परिमाण(संरचना sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_प्राथमिकता	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= NSEC_प्रत्येकम्_MSEC,
		.sched_deadline = 10 * NSEC_प्रत्येकम्_MSEC,
		.sched_period	= 10 * NSEC_प्रत्येकम्_MSEC,
	};
	संरचना cpufreq_policy *policy = sg_policy->policy;
	पूर्णाङ्क ret;

	/* kthread only required for slow path */
	यदि (policy->fast_switch_enabled)
		प्रत्यागम 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_कार्यकर्ता(&sg_policy->कार्यकर्ता);
	thread = kthread_create(kthread_कार्यकर्ता_fn, &sg_policy->कार्यकर्ता,
				"sugov:%d",
				cpumask_first(policy->related_cpus));
	यदि (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		प्रत्यागम PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	यदि (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		प्रत्यागम ret;
	}

	sg_policy->thread = thread;
	यदि (policy->dvfs_possible_from_any_cpu)
		समुच्चयः_cpus_allowed_ptr(thread, policy->related_cpus);
	अन्यथा
		kthread_bind_आवरणम्(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_तालकम्);

	wake_ऊर्ध्वम्_process(thread);

	प्रत्यागम 0;
}

स्थिर शून्य sugov_kthread_stop(संरचना sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	यदि (sg_policy->policy->fast_switch_enabled)
		प्रत्यागम;

	kthread_flush_कार्यकर्ता(&sg_policy->कार्यकर्ता);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_तालकम्);
}

स्थिर संरचना sugov_tunables *sugov_tunables_alloc(संरचना sugov_policy *sg_policy)
{
	संरचना sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	यदि (tunables) {
		gov_attr_समुच्चयः_init(&tunables->attr_समुच्चयः, &sg_policy->tunables_hook);
		यदि (!have_governor_प्रत्येकम्_policy())
			global_tunables = tunables;
	}
	प्रत्यागम tunables;
}

स्थिर शून्य sugov_clear_global_tunables(शून्य)
{
	यदि (!have_governor_प्रत्येकम्_policy())
		global_tunables = रिक्त;
}

स्थिर पूर्णाङ्क sugov_init(संरचना cpufreq_policy *policy)
{
	संरचना sugov_policy *sg_policy;
	संरचना sugov_tunables *tunables;
	पूर्णाङ्क ret = 0;

	/* State should be equivalent to EXIT */
	यदि (policy->governor_दत्त)
		प्रत्यागम -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	यदि (!sg_policy) {
		ret = -ENOMEM;
		गच्छ disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	यदि (ret)
		गच्छ free_sg_policy;

	mutex_तालकम्(&global_tunables_तालकम्);

	यदि (global_tunables) {
		यदि (WARN_ON(have_governor_प्रत्येकम्_policy())) {
			ret = -EINVAL;
			गच्छ stop_kthread;
		}
		policy->governor_दत्त = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_समुच्चयः_get(&global_tunables->attr_समुच्चयः, &sg_policy->tunables_hook);
		गच्छ बहिर्गमनम्;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	यदि (!tunables) {
		ret = -ENOMEM;
		गच्छ stop_kthread;
	}

	tunables->दरः_सीमा_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_दत्त = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_सङ्कलय(&tunables->attr_समुच्चयः.kobj, &sugov_tunables_ktype,
				   get_governor_parent_kobj(policy), "%s",
				   schedutil_gov.name);
	यदि (ret)
		गच्छ fail;

बहिर्गमनम्:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_तालकम्);
	प्रत्यागम 0;

fail:
	kobject_put(&tunables->attr_समुच्चयः.kobj);
	policy->governor_दत्त = रिक्त;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_तालकम्);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	प्रत्यागम ret;
}

स्थिर शून्य sugov_exit(संरचना cpufreq_policy *policy)
{
	संरचना sugov_policy *sg_policy = policy->governor_दत्त;
	संरचना sugov_tunables *tunables = sg_policy->tunables;
	अचिह्नित पूर्णाङ्क गणनम्;

	mutex_तालकम्(&global_tunables_तालकम्);

	गणनम् = gov_attr_समुच्चयः_put(&tunables->attr_समुच्चयः, &sg_policy->tunables_hook);
	policy->governor_दत्त = रिक्त;
	यदि (!गणनम्)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_तालकम्);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

स्थिर पूर्णाङ्क sugov_start(संरचना cpufreq_policy *policy)
{
	संरचना sugov_policy *sg_policy = policy->governor_दत्त;
	शून्य (*uu)(संरचना update_util_दत्त *दत्त, u64 time, अचिह्नित पूर्णाङ्क flags);
	अचिह्नित पूर्णाङ्क cpu;

	sugov_update_दरः_सीमा_us_sync(sg_policy);
	sg_policy->चरमः_freq_update_time	= 0;
	sg_policy->अग्र_freq			= 0;
	sg_policy->work_अन्तर्_प्रगतिः		= false;
	sg_policy->limits_परिवर्तितम्		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	यदि (policy_is_shared(policy))
		uu = sugov_update_shared;
	अन्यथा यदि (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	अन्यथा
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		संरचना sugov_cpu *sg_cpu = &प्रत्येकम्_cpu(sugov_cpu, cpu);

		स्मृतिं_पूरय(sg_cpu, 0, परिमाण(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		संरचना sugov_cpu *sg_cpu = &प्रत्येकम्_cpu(sugov_cpu, cpu);

		cpufreq_सङ्कलय_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	प्रत्यागम 0;
}

स्थिर शून्य sugov_stop(संरचना cpufreq_policy *policy)
{
	संरचना sugov_policy *sg_policy = policy->governor_दत्त;
	अचिह्नित पूर्णाङ्क cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	यदि (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

स्थिर शून्य sugov_limits(संरचना cpufreq_policy *policy)
{
	संरचना sugov_policy *sg_policy = policy->governor_दत्त;

	यदि (!policy->fast_switch_enabled) {
		mutex_तालकम्(&sg_policy->work_तालकम्);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_तालकम्);
	}

	/*
	 * The limits_changed update below must take place before the updates
	 * of policy limits in cpufreq_set_policy() or a policy limits update
	 * might be missed, so use a memory barrier to ensure it.
	 *
	 * This pairs with the memory barrier in sugov_should_update_freq().
	 */
	smp_wmb();

	WRITE_ONCE(sg_policy->limits_परिवर्तितम्, true);
}

स्थिर संरचना cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.निर्गच्छ			= sugov_exit,
	.start			= sugov_start,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#यदि_परिभाषितम् CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
संरचना cpufreq_governor *cpufreq_default_governor(शून्य)
{
	प्रत्यागम &schedutil_gov;
}
#यदि_अन्तः

bool sugov_is_governor(संरचना cpufreq_policy *policy)
{
	प्रत्यागम policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
