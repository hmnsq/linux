// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#অন্তর্ভাব <uapi/linux/sched/types.h>
#অন্তর্ভাব "sched.h"

#define IOWAIT_BOOST_MIN	(SCHED_CAPACITY_SCALE / 8)

সংরচনা sugov_tunables {
	সংরচনা gov_attr_set	attr_set;
	unsigned পূর্ণাঙ্ক		rate_limit_us;
};

সংরচনা sugov_policy {
	সংরচনা cpufreq_policy	*policy;

	সংরচনা sugov_tunables	*tunables;
	সংরচনা list_head	tunables_hook;

	raw_spinlock_t		update_lock;
	u64			last_freq_update_time;
	s64			freq_update_delay_ns;
	unsigned পূর্ণাঙ্ক		next_freq;
	unsigned পূর্ণাঙ্ক		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	সংরচনা			irq_work irq_work;
	সংরচনা			kthread_work work;
	সংরচনা			mutex work_lock;
	সংরচনা			kthread_worker worker;
	সংরচনা task_struct	*thread;
	bool			work_in_progress;

	bool			limits_changed;
	bool			need_freq_update;
};

সংরচনা sugov_cpu {
	সংরচনা update_util_data	update_util;
	সংরচনা sugov_policy	*sg_policy;
	unsigned পূর্ণাঙ্ক		cpu;

	bool			iowait_boost_pending;
	unsigned পূর্ণাঙ্ক		iowait_boost;
	u64			last_update;

	unsigned long		util;
	unsigned long		bw_min;
	unsigned long		bw_max;

	/* The field below is for single-CPU policies only: */
#ifdef CONFIG_NO_HZ_COMMON
	unsigned long		saved_idle_calls;
#endif
};

স্থির DEFINE_PER_CPU(সংরচনা sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

স্থির শূন্য sugov_update_rate_limit_us(সংরচনা sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->rate_limit_us * NSEC_PER_USEC;
}

স্থির শূন্য sugov_update_rate_limit_us_sync(সংরচনা sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_set.update_lock);

	sugov_update_rate_limit_us(sg_policy);
}

স্থির bool sugov_should_update_freq(সংরচনা sugov_policy *sg_policy, u64 time)
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
	যদি (!cpufreq_this_cpu_can_update(sg_policy->policy))
		প্রত্যাগম false;

	যদি (unlikely(READ_ONCE(sg_policy->limits_changed))) {
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

		প্রত্যাগম true;
	} অন্যথা যদি (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		প্রত্যাগম true;
	}

	delta_ns = time - sg_policy->last_freq_update_time;

	প্রত্যাগম delta_ns >= sg_policy->freq_update_delay_ns;
}

স্থির bool sugov_update_next_freq(সংরচনা sugov_policy *sg_policy, u64 time,
				   unsigned পূর্ণাঙ্ক next_freq)
{
	যদি (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		যদি (sg_policy->next_freq == next_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			প্রত্যাগম false;
	} অন্যথা যদি (sg_policy->next_freq == next_freq) {
		প্রত্যাগম false;
	}

	sg_policy->next_freq = next_freq;
	sg_policy->last_freq_update_time = time;

	প্রত্যাগম true;
}

স্থির শূন্য sugov_deferred_update(সংরচনা sugov_policy *sg_policy)
{
	যদি (!sg_policy->work_in_progress) {
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
স্থির __always_inline
unsigned long get_capacity_ref_freq(সংরচনা cpufreq_policy *policy)
{
	unsigned পূর্ণাঙ্ক freq = arch_scale_freq_ref(policy->cpu);

	যদি (freq)
		প্রত্যাগম freq;

	যদি (arch_scale_freq_invariant())
		প্রত্যাগম policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	প্রত্যাগম policy->cur + (policy->cur >> 2);
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
স্থির unsigned পূর্ণাঙ্ক get_next_freq(সংরচনা sugov_policy *sg_policy,
				  unsigned long util, unsigned long max)
{
	সংরচনা cpufreq_policy *policy = sg_policy->policy;
	unsigned পূর্ণাঙ্ক freq;

	freq = get_capacity_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	যদি (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		প্রত্যাগম sg_policy->next_freq;

	sg_policy->cached_raw_freq = freq;
	প্রত্যাগম cpufreq_driver_resolve_freq(policy, freq);
}

unsigned long sugov_effective_cpu_perf(পূর্ণাঙ্ক cpu, unsigned long actual,
				 unsigned long min,
				 unsigned long max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	যদি (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	প্রত্যাগম max(min, max);
}

স্থির শূন্য sugov_get_util(সংরচনা sugov_cpu *sg_cpu, unsigned long boost)
{
	unsigned long min, max, util = scx_cpuperf_target(sg_cpu->cpu);

	যদি (!scx_switched_all())
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
স্থির bool sugov_iowait_reset(সংরচনা sugov_cpu *sg_cpu, u64 time,
			       bool set_iowait_boost)
{
	s64 delta_ns = time - sg_cpu->last_update;

	/* Reset boost only if a tick has elapsed since last request */
	যদি (delta_ns <= TICK_NSEC)
		প্রত্যাগম false;

	sg_cpu->iowait_boost = set_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = set_iowait_boost;

	প্রত্যাগম true;
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
স্থির শূন্য sugov_iowait_boost(সংরচনা sugov_cpu *sg_cpu, u64 time,
			       unsigned পূর্ণাঙ্ক flags)
{
	bool set_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	যদি (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, time, set_iowait_boost))
		প্রত্যাগম;

	/* Boost only tasks waking up after IO */
	যদি (!set_iowait_boost)
		প্রত্যাগম;

	/* Ensure boost doubles only one time at each request */
	যদি (sg_cpu->iowait_boost_pending)
		প্রত্যাগম;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	যদি (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(unsigned পূর্ণাঙ্ক, sg_cpu->iowait_boost << 1, SCHED_CAPACITY_SCALE);
		প্রত্যাগম;
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
স্থির unsigned long sugov_iowait_apply(সংরচনা sugov_cpu *sg_cpu, u64 time,
			       unsigned long max_cap)
{
	/* No boost currently required */
	যদি (!sg_cpu->iowait_boost)
		প্রত্যাগম 0;

	/* Reset boost if the CPU appears to have been idle enough */
	যদি (sugov_iowait_reset(sg_cpu, time, false))
		প্রত্যাগম 0;

	যদি (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		যদি (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			প্রত্যাগম 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	প্রত্যাগম (sg_cpu->iowait_boost * max_cap) >> SCHED_CAPACITY_SHIFT;
}

#ifdef CONFIG_NO_HZ_COMMON
স্থির bool sugov_hold_freq(সংরচনা sugov_cpu *sg_cpu)
{
	unsigned long idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	যদি (scx_switched_all())
		প্রত্যাগম false;

	/* if capped by uclamp_max, always update to be in compliance */
	যদি (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		প্রত্যাগম false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	প্রত্যাগম ret;
}
#অন্যথা /* !CONFIG_NO_HZ_COMMON: */
স্থির অন্তঃস্থ bool sugov_hold_freq(সংরচনা sugov_cpu *sg_cpu) { প্রত্যাগম false; }
#endif /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
স্থির অন্তঃস্থ শূন্য ignore_dl_rate_limit(সংরচনা sugov_cpu *sg_cpu)
{
	যদি (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

স্থির অন্তঃস্থ bool sugov_update_single_common(সংরচনা sugov_cpu *sg_cpu,
					      u64 time, unsigned long max_cap,
					      unsigned পূর্ণাঙ্ক flags)
{
	unsigned long boost;

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu);

	যদি (!sugov_should_update_freq(sg_cpu->sg_policy, time))
		প্রত্যাগম false;

	boost = sugov_iowait_apply(sg_cpu, time, max_cap);
	sugov_get_util(sg_cpu, boost);

	প্রত্যাগম true;
}

স্থির শূন্য sugov_update_single_freq(সংরচনা update_util_data *hook, u64 time,
				     unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা sugov_cpu *sg_cpu = container_of(hook, সংরচনা sugov_cpu, update_util);
	সংরচনা sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned পূর্ণাঙ্ক cached_freq = sg_policy->cached_raw_freq;
	unsigned long max_cap;
	unsigned পূর্ণাঙ্ক next_f;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	যদি (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		প্রত্যাগম;

	next_f = get_next_freq(sg_policy, sg_cpu->util, max_cap);

	যদি (sugov_hold_freq(sg_cpu) && next_f < sg_policy->next_freq &&
	    !sg_policy->need_freq_update) {
		next_f = sg_policy->next_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	যদি (!sugov_update_next_freq(sg_policy, time, next_f))
		প্রত্যাগম;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	যদি (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, next_f);
	} অন্যথা {
		raw_spin_lock(&sg_policy->update_lock);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_lock);
	}
}

স্থির শূন্য sugov_update_single_perf(সংরচনা update_util_data *hook, u64 time,
				     unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা sugov_cpu *sg_cpu = container_of(hook, সংরচনা sugov_cpu, update_util);
	সংরচনা sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned long prev_util = sg_cpu->util;
	unsigned long max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	যদি (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, time, flags);
		প্রত্যাগম;
	}

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	যদি (!sugov_update_single_common(sg_cpu, time, max_cap, flags))
		প্রত্যাগম;

	যদি (sugov_hold_freq(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->last_freq_update_time = time;
}

স্থির unsigned পূর্ণাঙ্ক sugov_next_freq_shared(সংরচনা sugov_cpu *sg_cpu, u64 time)
{
	সংরচনা sugov_policy *sg_policy = sg_cpu->sg_policy;
	সংরচনা cpufreq_policy *policy = sg_policy->policy;
	unsigned long util = 0, max_cap;
	unsigned পূর্ণাঙ্ক j;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		সংরচনা sugov_cpu *j_sg_cpu = &per_cpu(sugov_cpu, j);
		unsigned long boost;

		boost = sugov_iowait_apply(j_sg_cpu, time, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	প্রত্যাগম get_next_freq(sg_policy, util, max_cap);
}

স্থির শূন্য
sugov_update_shared(সংরচনা update_util_data *hook, u64 time, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা sugov_cpu *sg_cpu = container_of(hook, সংরচনা sugov_cpu, update_util);
	সংরচনা sugov_policy *sg_policy = sg_cpu->sg_policy;
	unsigned পূর্ণাঙ্ক next_f;

	raw_spin_lock(&sg_policy->update_lock);

	sugov_iowait_boost(sg_cpu, time, flags);
	sg_cpu->last_update = time;

	ignore_dl_rate_limit(sg_cpu);

	যদি (sugov_should_update_freq(sg_policy, time)) {
		next_f = sugov_next_freq_shared(sg_cpu, time);

		যদি (!sugov_update_next_freq(sg_policy, time, next_f))
			গচ্ছ unlock;

		যদি (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, next_f);
		অন্যথা
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_lock);
}

স্থির শূন্য sugov_work(সংরচনা kthread_work *work)
{
	সংরচনা sugov_policy *sg_policy = container_of(work, সংরচনা sugov_policy, work);
	unsigned পূর্ণাঙ্ক freq;
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

স্থির শূন্য sugov_irq_work(সংরচনা irq_work *irq_work)
{
	সংরচনা sugov_policy *sg_policy;

	sg_policy = container_of(irq_work, সংরচনা sugov_policy, irq_work);

	kthread_queue_work(&sg_policy->worker, &sg_policy->work);
}

/************************** sysfs interface ************************/

স্থির সংরচনা sugov_tunables *global_tunables;
স্থির DEFINE_MUTEX(global_tunables_lock);

স্থির অন্তঃস্থ সংরচনা sugov_tunables *to_sugov_tunables(সংরচনা gov_attr_set *attr_set)
{
	প্রত্যাগম container_of(attr_set, সংরচনা sugov_tunables, attr_set);
}

স্থির ssize_t rate_limit_us_show(সংরচনা gov_attr_set *attr_set, বর্ণ *buf)
{
	সংরচনা sugov_tunables *tunables = to_sugov_tunables(attr_set);

	প্রত্যাগম sysfs_emit(buf, "%u\n", tunables->rate_limit_us);
}

স্থির ssize_t
rate_limit_us_store(সংরচনা gov_attr_set *attr_set, ধ্রুব বর্ণ *buf, size_t গুনতি)
{
	সংরচনা sugov_tunables *tunables = to_sugov_tunables(attr_set);
	সংরচনা sugov_policy *sg_policy;
	unsigned পূর্ণাঙ্ক rate_limit_us;

	যদি (kstrtouint(buf, 10, &rate_limit_us))
		প্রত্যাগম -EINVAL;

	tunables->rate_limit_us = rate_limit_us;

	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook)
		sugov_update_rate_limit_us(sg_policy);

	প্রত্যাগম গুনতি;
}

স্থির সংরচনা governor_attr rate_limit_us = __ATTR_RW(rate_limit_us);

স্থির সংরচনা attribute *sugov_attrs[] = {
	&rate_limit_us.attr,
	রিক্ত
};
ATTRIBUTE_GROUPS(sugov);

স্থির শূন্য sugov_tunables_free(সংরচনা kobject *kobj)
{
	সংরচনা gov_attr_set *attr_set = to_gov_attr_set(kobj);

	kfree(to_sugov_tunables(attr_set));
}

স্থির ধ্রুব সংরচনা kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

স্থির সংরচনা cpufreq_governor schedutil_gov;

স্থির সংরচনা sugov_policy *sugov_policy_alloc(সংরচনা cpufreq_policy *policy)
{
	সংরচনা sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	যদি (!sg_policy)
		প্রত্যাগম রিক্ত;

	sg_policy->policy = policy;
	raw_spin_lock_init(&sg_policy->update_lock);
	প্রত্যাগম sg_policy;
}

স্থির শূন্য sugov_policy_free(সংরচনা sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

স্থির পূর্ণাঙ্ক sugov_kthread_create(সংরচনা sugov_policy *sg_policy)
{
	সংরচনা task_struct *thread;
	সংরচনা sched_attr attr = {
		.size		= পরিমাণ(সংরচনা sched_attr),
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
	সংরচনা cpufreq_policy *policy = sg_policy->policy;
	পূর্ণাঙ্ক ret;

	/* kthread only required for slow path */
	যদি (policy->fast_switch_enabled)
		প্রত্যাগম 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_worker(&sg_policy->worker);
	thread = kthread_create(kthread_worker_fn, &sg_policy->worker,
				"sugov:%d",
				cpumask_first(policy->related_cpus));
	যদি (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		প্রত্যাগম PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	যদি (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		প্রত্যাগম ret;
	}

	sg_policy->thread = thread;
	যদি (policy->dvfs_possible_from_any_cpu)
		set_cpus_allowed_ptr(thread, policy->related_cpus);
	অন্যথা
		kthread_bind_mask(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_lock);

	wake_up_process(thread);

	প্রত্যাগম 0;
}

স্থির শূন্য sugov_kthread_stop(সংরচনা sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	যদি (sg_policy->policy->fast_switch_enabled)
		প্রত্যাগম;

	kthread_flush_worker(&sg_policy->worker);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_lock);
}

স্থির সংরচনা sugov_tunables *sugov_tunables_alloc(সংরচনা sugov_policy *sg_policy)
{
	সংরচনা sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	যদি (tunables) {
		gov_attr_set_init(&tunables->attr_set, &sg_policy->tunables_hook);
		যদি (!have_governor_per_policy())
			global_tunables = tunables;
	}
	প্রত্যাগম tunables;
}

স্থির শূন্য sugov_clear_global_tunables(শূন্য)
{
	যদি (!have_governor_per_policy())
		global_tunables = রিক্ত;
}

স্থির পূর্ণাঙ্ক sugov_init(সংরচনা cpufreq_policy *policy)
{
	সংরচনা sugov_policy *sg_policy;
	সংরচনা sugov_tunables *tunables;
	পূর্ণাঙ্ক ret = 0;

	/* State should be equivalent to EXIT */
	যদি (policy->governor_data)
		প্রত্যাগম -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	যদি (!sg_policy) {
		ret = -ENOMEM;
		গচ্ছ disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	যদি (ret)
		গচ্ছ free_sg_policy;

	mutex_lock(&global_tunables_lock);

	যদি (global_tunables) {
		যদি (WARN_ON(have_governor_per_policy())) {
			ret = -EINVAL;
			গচ্ছ stop_kthread;
		}
		policy->governor_data = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_set_get(&global_tunables->attr_set, &sg_policy->tunables_hook);
		গচ্ছ out;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	যদি (!tunables) {
		ret = -ENOMEM;
		গচ্ছ stop_kthread;
	}

	tunables->rate_limit_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_data = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_add(&tunables->attr_set.kobj, &sugov_tunables_ktype,
				   get_governor_parent_kobj(policy), "%s",
				   schedutil_gov.name);
	যদি (ret)
		গচ্ছ fail;

out:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_lock);
	প্রত্যাগম 0;

fail:
	kobject_put(&tunables->attr_set.kobj);
	policy->governor_data = রিক্ত;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_lock);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	প্রত্যাগম ret;
}

স্থির শূন্য sugov_exit(সংরচনা cpufreq_policy *policy)
{
	সংরচনা sugov_policy *sg_policy = policy->governor_data;
	সংরচনা sugov_tunables *tunables = sg_policy->tunables;
	unsigned পূর্ণাঙ্ক গুনতি;

	mutex_lock(&global_tunables_lock);

	গুনতি = gov_attr_set_put(&tunables->attr_set, &sg_policy->tunables_hook);
	policy->governor_data = রিক্ত;
	যদি (!গুনতি)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_lock);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

স্থির পূর্ণাঙ্ক sugov_start(সংরচনা cpufreq_policy *policy)
{
	সংরচনা sugov_policy *sg_policy = policy->governor_data;
	শূন্য (*uu)(সংরচনা update_util_data *data, u64 time, unsigned পূর্ণাঙ্ক flags);
	unsigned পূর্ণাঙ্ক cpu;

	sugov_update_rate_limit_us_sync(sg_policy);
	sg_policy->last_freq_update_time	= 0;
	sg_policy->next_freq			= 0;
	sg_policy->work_in_progress		= false;
	sg_policy->limits_changed		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	যদি (policy_is_shared(policy))
		uu = sugov_update_shared;
	অন্যথা যদি (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	অন্যথা
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		সংরচনা sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		memset(sg_cpu, 0, পরিমাণ(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		সংরচনা sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		cpufreq_add_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	প্রত্যাগম 0;
}

স্থির শূন্য sugov_stop(সংরচনা cpufreq_policy *policy)
{
	সংরচনা sugov_policy *sg_policy = policy->governor_data;
	unsigned পূর্ণাঙ্ক cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	যদি (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

স্থির শূন্য sugov_limits(সংরচনা cpufreq_policy *policy)
{
	সংরচনা sugov_policy *sg_policy = policy->governor_data;

	যদি (!policy->fast_switch_enabled) {
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

স্থির সংরচনা cpufreq_governor schedutil_gov = {
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
সংরচনা cpufreq_governor *cpufreq_default_governor(শূন্য)
{
	প্রত্যাগম &schedutil_gov;
}
#endif

bool sugov_is_governor(সংরচনা cpufreq_policy *policy)
{
	প্রত্যাগম policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
