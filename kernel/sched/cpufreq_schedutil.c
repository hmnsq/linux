// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#شامل <uapi/linux/sched/types.h>
#شامل "sched.h"

#تعریف_کرو IOWAIT_BOOST_MIN	(SCHED_استعداد_SCALE / 8)

ساخت sugov_tunables {
	ساخت gov_attr_سیٹ	attr_سیٹ;
	غیرعلامتی صحیح		شرح_حد_بندی_us;
};

ساخت sugov_policy {
	ساخت cpufreq_policy	*policy;

	ساخت sugov_tunables	*tunables;
	ساخت list_سر	tunables_hook;

	raw_spinlock_t		update_تالا;
	u64			آخری_freq_update_وقت;
	s64			freq_update_delay_ns;
	غیرعلامتی صحیح		اگلا_freq;
	غیرعلامتی صحیح		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	ساخت			irq_work irq_work;
	ساخت			kthread_work work;
	ساخت			mutex work_تالا;
	ساخت			kthread_کارکن کارکن;
	ساخت task_struct	*thread;
	bool			work_اندر_پیش_رفت;

	bool			limits_بدلا;
	bool			need_freq_update;
};

ساخت sugov_cpu {
	ساخت update_util_ڈیٹا	update_util;
	ساخت sugov_policy	*sg_policy;
	غیرعلامتی صحیح		cpu;

	bool			iowait_boost_pending;
	غیرعلامتی صحیح		iowait_boost;
	u64			آخری_update;

	غیرعلامتی طویل		util;
	غیرعلامتی طویل		bw_min;
	غیرعلامتی طویل		bw_max;

	/* The field below is for single-CPU policies only: */
#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
	غیرعلامتی طویل		saved_idle_calls;
#اگر_ختم
};

ساکن DEFINE_فی_CPU(ساخت sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

ساکن خالی sugov_update_شرح_حد_بندی_us(ساخت sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->شرح_حد_بندی_us * NSEC_فی_USEC;
}

ساکن خالی sugov_update_شرح_حد_بندی_us_sync(ساخت sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_سیٹ.update_تالا);

	sugov_update_شرح_حد_بندی_us(sg_policy);
}

ساکن bool sugov_should_update_freq(ساخت sugov_policy *sg_policy, u64 وقت)
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
	اگر (!cpufreq_this_cpu_can_update(sg_policy->policy))
		واپس false;

	اگر (unlikely(READ_ONCE(sg_policy->limits_بدلا))) {
		WRITE_ONCE(sg_policy->limits_بدلا, false);
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

		واپس true;
	} ورنہ اگر (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		واپس true;
	}

	delta_ns = وقت - sg_policy->آخری_freq_update_وقت;

	واپس delta_ns >= sg_policy->freq_update_delay_ns;
}

ساکن bool sugov_update_اگلا_freq(ساخت sugov_policy *sg_policy, u64 وقت,
				   غیرعلامتی صحیح اگلا_freq)
{
	اگر (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		اگر (sg_policy->اگلا_freq == اگلا_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			واپس false;
	} ورنہ اگر (sg_policy->اگلا_freq == اگلا_freq) {
		واپس false;
	}

	sg_policy->اگلا_freq = اگلا_freq;
	sg_policy->آخری_freq_update_وقت = وقت;

	واپس true;
}

ساکن خالی sugov_deferred_update(ساخت sugov_policy *sg_policy)
{
	اگر (!sg_policy->work_اندر_پیش_رفت) {
		sg_policy->work_اندر_پیش_رفت = true;
		irq_work_قطار(&sg_policy->irq_work);
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
ساکن __always_inline
غیرعلامتی طویل get_استعداد_ref_freq(ساخت cpufreq_policy *policy)
{
	غیرعلامتی صحیح freq = arch_scale_freq_ref(policy->cpu);

	اگر (freq)
		واپس freq;

	اگر (arch_scale_freq_invariant())
		واپس policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	واپس policy->cur + (policy->cur >> 2);
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
ساکن غیرعلامتی صحیح get_اگلا_freq(ساخت sugov_policy *sg_policy,
				  غیرعلامتی طویل util, غیرعلامتی طویل max)
{
	ساخت cpufreq_policy *policy = sg_policy->policy;
	غیرعلامتی صحیح freq;

	freq = get_استعداد_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	اگر (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		واپس sg_policy->اگلا_freq;

	sg_policy->cached_raw_freq = freq;
	واپس cpufreq_driver_resolve_freq(policy, freq);
}

غیرعلامتی طویل sugov_effective_cpu_perf(صحیح cpu, غیرعلامتی طویل actual,
				 غیرعلامتی طویل min,
				 غیرعلامتی طویل max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	اگر (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	واپس max(min, max);
}

ساکن خالی sugov_get_util(ساخت sugov_cpu *sg_cpu, غیرعلامتی طویل boost)
{
	غیرعلامتی طویل min, max, util = scx_cpuperf_ہدف(sg_cpu->cpu);

	اگر (!scx_switched_all())
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
ساکن bool sugov_iowait_reset(ساخت sugov_cpu *sg_cpu, u64 وقت,
			       bool سیٹ_iowait_boost)
{
	s64 delta_ns = وقت - sg_cpu->آخری_update;

	/* Reset boost only if a tick has elapsed since last request */
	اگر (delta_ns <= TICK_NSEC)
		واپس false;

	sg_cpu->iowait_boost = سیٹ_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = سیٹ_iowait_boost;

	واپس true;
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
ساکن خالی sugov_iowait_boost(ساخت sugov_cpu *sg_cpu, u64 وقت,
			       غیرعلامتی صحیح flags)
{
	bool سیٹ_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	اگر (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, وقت, سیٹ_iowait_boost))
		واپس;

	/* Boost only tasks waking up after IO */
	اگر (!سیٹ_iowait_boost)
		واپس;

	/* Ensure boost doubles only one time at each request */
	اگر (sg_cpu->iowait_boost_pending)
		واپس;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	اگر (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(غیرعلامتی صحیح, sg_cpu->iowait_boost << 1, SCHED_استعداد_SCALE);
		واپس;
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
ساکن غیرعلامتی طویل sugov_iowait_apply(ساخت sugov_cpu *sg_cpu, u64 وقت,
			       غیرعلامتی طویل max_cap)
{
	/* No boost currently required */
	اگر (!sg_cpu->iowait_boost)
		واپس 0;

	/* Reset boost if the CPU appears to have been idle enough */
	اگر (sugov_iowait_reset(sg_cpu, وقت, false))
		واپس 0;

	اگر (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		اگر (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			واپس 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	واپس (sg_cpu->iowait_boost * max_cap) >> SCHED_استعداد_سرکاؤ;
}

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
ساکن bool sugov_hold_freq(ساخت sugov_cpu *sg_cpu)
{
	غیرعلامتی طویل idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	اگر (scx_switched_all())
		واپس false;

	/* if capped by uclamp_max, always update to be in compliance */
	اگر (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		واپس false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	واپس ret;
}
#ورنہ /* !CONFIG_NO_HZ_COMMON: */
ساکن اندرونی bool sugov_hold_freq(ساخت sugov_cpu *sg_cpu) { واپس false; }
#اگر_ختم /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
ساکن اندرونی خالی ignore_dl_شرح_حد_بندی(ساخت sugov_cpu *sg_cpu)
{
	اگر (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

ساکن اندرونی bool sugov_update_single_common(ساخت sugov_cpu *sg_cpu,
					      u64 وقت, غیرعلامتی طویل max_cap,
					      غیرعلامتی صحیح flags)
{
	غیرعلامتی طویل boost;

	sugov_iowait_boost(sg_cpu, وقت, flags);
	sg_cpu->آخری_update = وقت;

	ignore_dl_شرح_حد_بندی(sg_cpu);

	اگر (!sugov_should_update_freq(sg_cpu->sg_policy, وقت))
		واپس false;

	boost = sugov_iowait_apply(sg_cpu, وقت, max_cap);
	sugov_get_util(sg_cpu, boost);

	واپس true;
}

ساکن خالی sugov_update_single_freq(ساخت update_util_ڈیٹا *hook, u64 وقت,
				     غیرعلامتی صحیح flags)
{
	ساخت sugov_cpu *sg_cpu = container_کا(hook, ساخت sugov_cpu, update_util);
	ساخت sugov_policy *sg_policy = sg_cpu->sg_policy;
	غیرعلامتی صحیح cached_freq = sg_policy->cached_raw_freq;
	غیرعلامتی طویل max_cap;
	غیرعلامتی صحیح اگلا_f;

	max_cap = arch_scale_cpu_استعداد(sg_cpu->cpu);

	اگر (!sugov_update_single_common(sg_cpu, وقت, max_cap, flags))
		واپس;

	اگلا_f = get_اگلا_freq(sg_policy, sg_cpu->util, max_cap);

	اگر (sugov_hold_freq(sg_cpu) && اگلا_f < sg_policy->اگلا_freq &&
	    !sg_policy->need_freq_update) {
		اگلا_f = sg_policy->اگلا_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	اگر (!sugov_update_اگلا_freq(sg_policy, وقت, اگلا_f))
		واپس;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	اگر (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, اگلا_f);
	} ورنہ {
		raw_spin_تالا(&sg_policy->update_تالا);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_تالا);
	}
}

ساکن خالی sugov_update_single_perf(ساخت update_util_ڈیٹا *hook, u64 وقت,
				     غیرعلامتی صحیح flags)
{
	ساخت sugov_cpu *sg_cpu = container_کا(hook, ساخت sugov_cpu, update_util);
	ساخت sugov_policy *sg_policy = sg_cpu->sg_policy;
	غیرعلامتی طویل پچھلا_util = sg_cpu->util;
	غیرعلامتی طویل max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	اگر (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, وقت, flags);
		واپس;
	}

	max_cap = arch_scale_cpu_استعداد(sg_cpu->cpu);

	اگر (!sugov_update_single_common(sg_cpu, وقت, max_cap, flags))
		واپس;

	اگر (sugov_hold_freq(sg_cpu) && sg_cpu->util < پچھلا_util)
		sg_cpu->util = پچھلا_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->آخری_freq_update_وقت = وقت;
}

ساکن غیرعلامتی صحیح sugov_اگلا_freq_shared(ساخت sugov_cpu *sg_cpu, u64 وقت)
{
	ساخت sugov_policy *sg_policy = sg_cpu->sg_policy;
	ساخت cpufreq_policy *policy = sg_policy->policy;
	غیرعلامتی طویل util = 0, max_cap;
	غیرعلامتی صحیح j;

	max_cap = arch_scale_cpu_استعداد(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		ساخت sugov_cpu *j_sg_cpu = &فی_cpu(sugov_cpu, j);
		غیرعلامتی طویل boost;

		boost = sugov_iowait_apply(j_sg_cpu, وقت, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	واپس get_اگلا_freq(sg_policy, util, max_cap);
}

ساکن خالی
sugov_update_shared(ساخت update_util_ڈیٹا *hook, u64 وقت, غیرعلامتی صحیح flags)
{
	ساخت sugov_cpu *sg_cpu = container_کا(hook, ساخت sugov_cpu, update_util);
	ساخت sugov_policy *sg_policy = sg_cpu->sg_policy;
	غیرعلامتی صحیح اگلا_f;

	raw_spin_تالا(&sg_policy->update_تالا);

	sugov_iowait_boost(sg_cpu, وقت, flags);
	sg_cpu->آخری_update = وقت;

	ignore_dl_شرح_حد_بندی(sg_cpu);

	اگر (sugov_should_update_freq(sg_policy, وقت)) {
		اگلا_f = sugov_اگلا_freq_shared(sg_cpu, وقت);

		اگر (!sugov_update_اگلا_freq(sg_policy, وقت, اگلا_f))
			جاؤ unlock;

		اگر (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, اگلا_f);
		ورنہ
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_تالا);
}

ساکن خالی sugov_work(ساخت kthread_work *work)
{
	ساخت sugov_policy *sg_policy = container_کا(work, ساخت sugov_policy, work);
	غیرعلامتی صحیح freq;
	غیرعلامتی طویل flags;

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
	raw_spin_تالا_irqsave(&sg_policy->update_تالا, flags);
	freq = sg_policy->اگلا_freq;
	sg_policy->work_اندر_پیش_رفت = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_تالا, flags);

	mutex_تالا(&sg_policy->work_تالا);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_رشتہ_L);
	mutex_unlock(&sg_policy->work_تالا);
}

ساکن خالی sugov_irq_work(ساخت irq_work *irq_work)
{
	ساخت sugov_policy *sg_policy;

	sg_policy = container_کا(irq_work, ساخت sugov_policy, irq_work);

	kthread_قطار_work(&sg_policy->کارکن, &sg_policy->work);
}

/************************** sysfs interface ************************/

ساکن ساخت sugov_tunables *global_tunables;
ساکن DEFINE_MUTEX(global_tunables_تالا);

ساکن اندرونی ساخت sugov_tunables *مقصد_sugov_tunables(ساخت gov_attr_سیٹ *attr_سیٹ)
{
	واپس container_کا(attr_سیٹ, ساخت sugov_tunables, attr_سیٹ);
}

ساکن ssize_t شرح_حد_بندی_us_show(ساخت gov_attr_سیٹ *attr_سیٹ, حرف *buf)
{
	ساخت sugov_tunables *tunables = مقصد_sugov_tunables(attr_سیٹ);

	واپس sysfs_emit(buf, "%u\n", tunables->شرح_حد_بندی_us);
}

ساکن ssize_t
شرح_حد_بندی_us_store(ساخت gov_attr_سیٹ *attr_سیٹ, ثابت حرف *buf, سائز_t گنتی)
{
	ساخت sugov_tunables *tunables = مقصد_sugov_tunables(attr_سیٹ);
	ساخت sugov_policy *sg_policy;
	غیرعلامتی صحیح شرح_حد_بندی_us;

	اگر (kstrtouint(buf, 10, &شرح_حد_بندی_us))
		واپس -EINVAL;

	tunables->شرح_حد_بندی_us = شرح_حد_بندی_us;

	list_for_each_اندراج(sg_policy, &attr_سیٹ->policy_list, tunables_hook)
		sugov_update_شرح_حد_بندی_us(sg_policy);

	واپس گنتی;
}

ساکن ساخت governor_attr شرح_حد_بندی_us = __ATTR_RW(شرح_حد_بندی_us);

ساکن ساخت attribute *sugov_attrs[] = {
	&شرح_حد_بندی_us.attr,
	تہی
};
ATTRIBUTE_GROUPS(sugov);

ساکن خالی sugov_tunables_free(ساخت kobject *kobj)
{
	ساخت gov_attr_سیٹ *attr_سیٹ = مقصد_gov_attr_سیٹ(kobj);

	kfree(مقصد_sugov_tunables(attr_سیٹ));
}

ساکن ثابت ساخت kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

ساکن ساخت cpufreq_governor schedutil_gov;

ساکن ساخت sugov_policy *sugov_policy_alloc(ساخت cpufreq_policy *policy)
{
	ساخت sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	اگر (!sg_policy)
		واپس تہی;

	sg_policy->policy = policy;
	raw_spin_تالا_init(&sg_policy->update_تالا);
	واپس sg_policy;
}

ساکن خالی sugov_policy_free(ساخت sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

ساکن صحیح sugov_kthread_create(ساخت sugov_policy *sg_policy)
{
	ساخت task_struct *thread;
	ساخت sched_attr attr = {
		.سائز		= حجم(ساخت sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_ترجیح	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= NSEC_فی_MSEC,
		.sched_deadline = 10 * NSEC_فی_MSEC,
		.sched_period	= 10 * NSEC_فی_MSEC,
	};
	ساخت cpufreq_policy *policy = sg_policy->policy;
	صحیح ret;

	/* kthread only required for slow path */
	اگر (policy->fast_switch_enabled)
		واپس 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_کارکن(&sg_policy->کارکن);
	thread = kthread_create(kthread_کارکن_fn, &sg_policy->کارکن,
				"sugov:%d",
				cpumask_پہلا(policy->related_cpus));
	اگر (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		واپس PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	اگر (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		واپس ret;
	}

	sg_policy->thread = thread;
	اگر (policy->dvfs_possible_from_any_cpu)
		سیٹ_cpus_allowed_ptr(thread, policy->related_cpus);
	ورنہ
		kthread_bind_نقاب(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_تالا);

	wake_اوپر_process(thread);

	واپس 0;
}

ساکن خالی sugov_kthread_stop(ساخت sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	اگر (sg_policy->policy->fast_switch_enabled)
		واپس;

	kthread_flush_کارکن(&sg_policy->کارکن);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_تالا);
}

ساکن ساخت sugov_tunables *sugov_tunables_alloc(ساخت sugov_policy *sg_policy)
{
	ساخت sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	اگر (tunables) {
		gov_attr_سیٹ_init(&tunables->attr_سیٹ, &sg_policy->tunables_hook);
		اگر (!have_governor_فی_policy())
			global_tunables = tunables;
	}
	واپس tunables;
}

ساکن خالی sugov_clear_global_tunables(خالی)
{
	اگر (!have_governor_فی_policy())
		global_tunables = تہی;
}

ساکن صحیح sugov_init(ساخت cpufreq_policy *policy)
{
	ساخت sugov_policy *sg_policy;
	ساخت sugov_tunables *tunables;
	صحیح ret = 0;

	/* State should be equivalent to EXIT */
	اگر (policy->governor_ڈیٹا)
		واپس -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	اگر (!sg_policy) {
		ret = -ENOMEM;
		جاؤ disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	اگر (ret)
		جاؤ free_sg_policy;

	mutex_تالا(&global_tunables_تالا);

	اگر (global_tunables) {
		اگر (WARN_ON(have_governor_فی_policy())) {
			ret = -EINVAL;
			جاؤ stop_kthread;
		}
		policy->governor_ڈیٹا = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_سیٹ_get(&global_tunables->attr_سیٹ, &sg_policy->tunables_hook);
		جاؤ برآمد;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	اگر (!tunables) {
		ret = -ENOMEM;
		جاؤ stop_kthread;
	}

	tunables->شرح_حد_بندی_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_ڈیٹا = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_جوڑو(&tunables->attr_سیٹ.kobj, &sugov_tunables_ktype,
				   get_governor_والد_kobj(policy), "%s",
				   schedutil_gov.name);
	اگر (ret)
		جاؤ fail;

برآمد:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_تالا);
	واپس 0;

fail:
	kobject_put(&tunables->attr_سیٹ.kobj);
	policy->governor_ڈیٹا = تہی;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_تالا);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	واپس ret;
}

ساکن خالی sugov_exit(ساخت cpufreq_policy *policy)
{
	ساخت sugov_policy *sg_policy = policy->governor_ڈیٹا;
	ساخت sugov_tunables *tunables = sg_policy->tunables;
	غیرعلامتی صحیح گنتی;

	mutex_تالا(&global_tunables_تالا);

	گنتی = gov_attr_سیٹ_put(&tunables->attr_سیٹ, &sg_policy->tunables_hook);
	policy->governor_ڈیٹا = تہی;
	اگر (!گنتی)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_تالا);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

ساکن صحیح sugov_آغاز(ساخت cpufreq_policy *policy)
{
	ساخت sugov_policy *sg_policy = policy->governor_ڈیٹا;
	خالی (*uu)(ساخت update_util_ڈیٹا *ڈیٹا, u64 وقت, غیرعلامتی صحیح flags);
	غیرعلامتی صحیح cpu;

	sugov_update_شرح_حد_بندی_us_sync(sg_policy);
	sg_policy->آخری_freq_update_وقت	= 0;
	sg_policy->اگلا_freq			= 0;
	sg_policy->work_اندر_پیش_رفت		= false;
	sg_policy->limits_بدلا		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	اگر (policy_is_shared(policy))
		uu = sugov_update_shared;
	ورنہ اگر (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	ورنہ
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		ساخت sugov_cpu *sg_cpu = &فی_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, حجم(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		ساخت sugov_cpu *sg_cpu = &فی_cpu(sugov_cpu, cpu);

		cpufreq_جوڑو_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	واپس 0;
}

ساکن خالی sugov_stop(ساخت cpufreq_policy *policy)
{
	ساخت sugov_policy *sg_policy = policy->governor_ڈیٹا;
	غیرعلامتی صحیح cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	اگر (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

ساکن خالی sugov_limits(ساخت cpufreq_policy *policy)
{
	ساخت sugov_policy *sg_policy = policy->governor_ڈیٹا;

	اگر (!policy->fast_switch_enabled) {
		mutex_تالا(&sg_policy->work_تالا);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_تالا);
	}

	/*
	 * The limits_changed update below must take place before the updates
	 * of policy limits in cpufreq_set_policy() or a policy limits update
	 * might be missed, so use a memory barrier to ensure it.
	 *
	 * This pairs with the memory barrier in sugov_should_update_freq().
	 */
	smp_wmb();

	WRITE_ONCE(sg_policy->limits_بدلا, true);
}

ساکن ساخت cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.باہر_نکلو			= sugov_exit,
	.آغاز			= sugov_آغاز,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#اگر_تعریف_شدہ CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
ساخت cpufreq_governor *cpufreq_default_governor(خالی)
{
	واپس &schedutil_gov;
}
#اگر_ختم

bool sugov_is_governor(ساخت cpufreq_policy *policy)
{
	واپس policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
