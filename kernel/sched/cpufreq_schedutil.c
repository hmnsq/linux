// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#ضمن <uapi/linux/sched/types.h>
#ضمن "sched.h"

#تعریف_کړه IOWAIT_BOOST_MIN	(SCHED_ظرفیت_SCALE / 8)

بنية sugov_tunables {
	بنية gov_attr_سېټ	attr_سېټ;
	بینښه صحيح		شرح_حد_us;
};

بنية sugov_policy {
	بنية cpufreq_policy	*policy;

	بنية sugov_tunables	*tunables;
	بنية list_سر	tunables_hook;

	raw_spinlock_t		update_قلف;
	u64			وروستی_freq_update_وخت;
	s64			freq_update_delay_ns;
	بینښه صحيح		بل_freq;
	بینښه صحيح		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	بنية			irq_work irq_work;
	بنية			kthread_work work;
	بنية			mutex work_قلف;
	بنية			kthread_کارګر کارګر;
	بنية task_struct	*thread;
	bool			work_دننه_پرمختګ;

	bool			limits_بدل_شوی;
	bool			need_freq_update;
};

بنية sugov_cpu {
	بنية update_util_معلومات	update_util;
	بنية sugov_policy	*sg_policy;
	بینښه صحيح		cpu;

	bool			iowait_boost_pending;
	بینښه صحيح		iowait_boost;
	u64			وروستی_update;

	بینښه اوږد		util;
	بینښه اوږد		bw_min;
	بینښه اوږد		bw_max;

	/* The field below is for single-CPU policies only: */
#که_تعریف_شوی CONFIG_NO_HZ_COMMON
	بینښه اوږد		saved_idle_calls;
#د_که_پای
};

ساكن DEFINE_فی_CPU(بنية sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

ساكن فراغ sugov_update_شرح_حد_us(بنية sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->شرح_حد_us * NSEC_فی_USEC;
}

ساكن فراغ sugov_update_شرح_حد_us_sync(بنية sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_سېټ.update_قلف);

	sugov_update_شرح_حد_us(sg_policy);
}

ساكن bool sugov_should_update_freq(بنية sugov_policy *sg_policy, u64 وخت)
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
	إذا (!cpufreq_this_cpu_can_update(sg_policy->policy))
		أرجع false;

	إذا (unlikely(READ_ONCE(sg_policy->limits_بدل_شوی))) {
		WRITE_ONCE(sg_policy->limits_بدل_شوی, false);
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

		أرجع true;
	} وإلا إذا (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		أرجع true;
	}

	delta_ns = وخت - sg_policy->وروستی_freq_update_وخت;

	أرجع delta_ns >= sg_policy->freq_update_delay_ns;
}

ساكن bool sugov_update_بل_freq(بنية sugov_policy *sg_policy, u64 وخت,
				   بینښه صحيح بل_freq)
{
	إذا (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		إذا (sg_policy->بل_freq == بل_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			أرجع false;
	} وإلا إذا (sg_policy->بل_freq == بل_freq) {
		أرجع false;
	}

	sg_policy->بل_freq = بل_freq;
	sg_policy->وروستی_freq_update_وخت = وخت;

	أرجع true;
}

ساكن فراغ sugov_deferred_update(بنية sugov_policy *sg_policy)
{
	إذا (!sg_policy->work_دننه_پرمختګ) {
		sg_policy->work_دننه_پرمختګ = true;
		irq_work_کتار(&sg_policy->irq_work);
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
ساكن __always_inline
بینښه اوږد get_ظرفیت_ref_freq(بنية cpufreq_policy *policy)
{
	بینښه صحيح freq = arch_scale_freq_ref(policy->cpu);

	إذا (freq)
		أرجع freq;

	إذا (arch_scale_freq_invariant())
		أرجع policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	أرجع policy->cur + (policy->cur >> 2);
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
ساكن بینښه صحيح get_بل_freq(بنية sugov_policy *sg_policy,
				  بینښه اوږد util, بینښه اوږد max)
{
	بنية cpufreq_policy *policy = sg_policy->policy;
	بینښه صحيح freq;

	freq = get_ظرفیت_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	إذا (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		أرجع sg_policy->بل_freq;

	sg_policy->cached_raw_freq = freq;
	أرجع cpufreq_driver_resolve_freq(policy, freq);
}

بینښه اوږد sugov_effective_cpu_perf(صحيح cpu, بینښه اوږد actual,
				 بینښه اوږد min,
				 بینښه اوږد max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	إذا (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	أرجع max(min, max);
}

ساكن فراغ sugov_get_util(بنية sugov_cpu *sg_cpu, بینښه اوږد boost)
{
	بینښه اوږد min, max, util = scx_cpuperf_موخه(sg_cpu->cpu);

	إذا (!scx_switched_all())
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
ساكن bool sugov_iowait_reset(بنية sugov_cpu *sg_cpu, u64 وخت,
			       bool سېټ_iowait_boost)
{
	s64 delta_ns = وخت - sg_cpu->وروستی_update;

	/* Reset boost only if a tick has elapsed since last request */
	إذا (delta_ns <= TICK_NSEC)
		أرجع false;

	sg_cpu->iowait_boost = سېټ_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = سېټ_iowait_boost;

	أرجع true;
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
ساكن فراغ sugov_iowait_boost(بنية sugov_cpu *sg_cpu, u64 وخت,
			       بینښه صحيح flags)
{
	bool سېټ_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	إذا (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, وخت, سېټ_iowait_boost))
		أرجع;

	/* Boost only tasks waking up after IO */
	إذا (!سېټ_iowait_boost)
		أرجع;

	/* Ensure boost doubles only one time at each request */
	إذا (sg_cpu->iowait_boost_pending)
		أرجع;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	إذا (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(بینښه صحيح, sg_cpu->iowait_boost << 1, SCHED_ظرفیت_SCALE);
		أرجع;
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
ساكن بینښه اوږد sugov_iowait_apply(بنية sugov_cpu *sg_cpu, u64 وخت,
			       بینښه اوږد max_cap)
{
	/* No boost currently required */
	إذا (!sg_cpu->iowait_boost)
		أرجع 0;

	/* Reset boost if the CPU appears to have been idle enough */
	إذا (sugov_iowait_reset(sg_cpu, وخت, false))
		أرجع 0;

	إذا (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		إذا (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			أرجع 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	أرجع (sg_cpu->iowait_boost * max_cap) >> SCHED_ظرفیت_ښويدنه;
}

#که_تعریف_شوی CONFIG_NO_HZ_COMMON
ساكن bool sugov_hold_freq(بنية sugov_cpu *sg_cpu)
{
	بینښه اوږد idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	إذا (scx_switched_all())
		أرجع false;

	/* if capped by uclamp_max, always update to be in compliance */
	إذا (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		أرجع false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	أرجع ret;
}
#وإلا /* !CONFIG_NO_HZ_COMMON: */
ساكن مضمن bool sugov_hold_freq(بنية sugov_cpu *sg_cpu) { أرجع false; }
#د_که_پای /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
ساكن مضمن فراغ ignore_dl_شرح_حد(بنية sugov_cpu *sg_cpu)
{
	إذا (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

ساكن مضمن bool sugov_update_single_common(بنية sugov_cpu *sg_cpu,
					      u64 وخت, بینښه اوږد max_cap,
					      بینښه صحيح flags)
{
	بینښه اوږد boost;

	sugov_iowait_boost(sg_cpu, وخت, flags);
	sg_cpu->وروستی_update = وخت;

	ignore_dl_شرح_حد(sg_cpu);

	إذا (!sugov_should_update_freq(sg_cpu->sg_policy, وخت))
		أرجع false;

	boost = sugov_iowait_apply(sg_cpu, وخت, max_cap);
	sugov_get_util(sg_cpu, boost);

	أرجع true;
}

ساكن فراغ sugov_update_single_freq(بنية update_util_معلومات *hook, u64 وخت,
				     بینښه صحيح flags)
{
	بنية sugov_cpu *sg_cpu = container_د(hook, بنية sugov_cpu, update_util);
	بنية sugov_policy *sg_policy = sg_cpu->sg_policy;
	بینښه صحيح cached_freq = sg_policy->cached_raw_freq;
	بینښه اوږد max_cap;
	بینښه صحيح بل_f;

	max_cap = arch_scale_cpu_ظرفیت(sg_cpu->cpu);

	إذا (!sugov_update_single_common(sg_cpu, وخت, max_cap, flags))
		أرجع;

	بل_f = get_بل_freq(sg_policy, sg_cpu->util, max_cap);

	إذا (sugov_hold_freq(sg_cpu) && بل_f < sg_policy->بل_freq &&
	    !sg_policy->need_freq_update) {
		بل_f = sg_policy->بل_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	إذا (!sugov_update_بل_freq(sg_policy, وخت, بل_f))
		أرجع;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	إذا (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, بل_f);
	} وإلا {
		raw_spin_قلف(&sg_policy->update_قلف);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_قلف);
	}
}

ساكن فراغ sugov_update_single_perf(بنية update_util_معلومات *hook, u64 وخت,
				     بینښه صحيح flags)
{
	بنية sugov_cpu *sg_cpu = container_د(hook, بنية sugov_cpu, update_util);
	بنية sugov_policy *sg_policy = sg_cpu->sg_policy;
	بینښه اوږد prev_util = sg_cpu->util;
	بینښه اوږد max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	إذا (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, وخت, flags);
		أرجع;
	}

	max_cap = arch_scale_cpu_ظرفیت(sg_cpu->cpu);

	إذا (!sugov_update_single_common(sg_cpu, وخت, max_cap, flags))
		أرجع;

	إذا (sugov_hold_freq(sg_cpu) && sg_cpu->util < prev_util)
		sg_cpu->util = prev_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->وروستی_freq_update_وخت = وخت;
}

ساكن بینښه صحيح sugov_بل_freq_shared(بنية sugov_cpu *sg_cpu, u64 وخت)
{
	بنية sugov_policy *sg_policy = sg_cpu->sg_policy;
	بنية cpufreq_policy *policy = sg_policy->policy;
	بینښه اوږد util = 0, max_cap;
	بینښه صحيح j;

	max_cap = arch_scale_cpu_ظرفیت(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		بنية sugov_cpu *j_sg_cpu = &فی_cpu(sugov_cpu, j);
		بینښه اوږد boost;

		boost = sugov_iowait_apply(j_sg_cpu, وخت, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	أرجع get_بل_freq(sg_policy, util, max_cap);
}

ساكن فراغ
sugov_update_shared(بنية update_util_معلومات *hook, u64 وخت, بینښه صحيح flags)
{
	بنية sugov_cpu *sg_cpu = container_د(hook, بنية sugov_cpu, update_util);
	بنية sugov_policy *sg_policy = sg_cpu->sg_policy;
	بینښه صحيح بل_f;

	raw_spin_قلف(&sg_policy->update_قلف);

	sugov_iowait_boost(sg_cpu, وخت, flags);
	sg_cpu->وروستی_update = وخت;

	ignore_dl_شرح_حد(sg_cpu);

	إذا (sugov_should_update_freq(sg_policy, وخت)) {
		بل_f = sugov_بل_freq_shared(sg_cpu, وخت);

		إذا (!sugov_update_بل_freq(sg_policy, وخت, بل_f))
			اذهب unlock;

		إذا (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, بل_f);
		وإلا
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_قلف);
}

ساكن فراغ sugov_work(بنية kthread_work *work)
{
	بنية sugov_policy *sg_policy = container_د(work, بنية sugov_policy, work);
	بینښه صحيح freq;
	بینښه اوږد flags;

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
	raw_spin_قلف_irqsave(&sg_policy->update_قلف, flags);
	freq = sg_policy->بل_freq;
	sg_policy->work_دننه_پرمختګ = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_قلف, flags);

	mutex_قلف(&sg_policy->work_قلف);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_اړیکه_L);
	mutex_unlock(&sg_policy->work_قلف);
}

ساكن فراغ sugov_irq_work(بنية irq_work *irq_work)
{
	بنية sugov_policy *sg_policy;

	sg_policy = container_د(irq_work, بنية sugov_policy, irq_work);

	kthread_کتار_work(&sg_policy->کارګر, &sg_policy->work);
}

/************************** sysfs interface ************************/

ساكن بنية sugov_tunables *global_tunables;
ساكن DEFINE_MUTEX(global_tunables_قلف);

ساكن مضمن بنية sugov_tunables *منزل_sugov_tunables(بنية gov_attr_سېټ *attr_سېټ)
{
	أرجع container_د(attr_سېټ, بنية sugov_tunables, attr_سېټ);
}

ساكن ssize_t شرح_حد_us_show(بنية gov_attr_سېټ *attr_سېټ, حرف *buf)
{
	بنية sugov_tunables *tunables = منزل_sugov_tunables(attr_سېټ);

	أرجع sysfs_emit(buf, "%u\n", tunables->شرح_حد_us);
}

ساكن ssize_t
شرح_حد_us_store(بنية gov_attr_سېټ *attr_سېټ, ثابت حرف *buf, اندازه_t شمېر)
{
	بنية sugov_tunables *tunables = منزل_sugov_tunables(attr_سېټ);
	بنية sugov_policy *sg_policy;
	بینښه صحيح شرح_حد_us;

	إذا (kstrtouint(buf, 10, &شرح_حد_us))
		أرجع -EINVAL;

	tunables->شرح_حد_us = شرح_حد_us;

	list_for_each_ثبت(sg_policy, &attr_سېټ->policy_list, tunables_hook)
		sugov_update_شرح_حد_us(sg_policy);

	أرجع شمېر;
}

ساكن بنية governor_attr شرح_حد_us = __ATTR_RW(شرح_حد_us);

ساكن بنية attribute *sugov_attrs[] = {
	&شرح_حد_us.attr,
	عدم
};
ATTRIBUTE_GROUPS(sugov);

ساكن فراغ sugov_tunables_free(بنية kobject *kobj)
{
	بنية gov_attr_سېټ *attr_سېټ = منزل_gov_attr_سېټ(kobj);

	kfree(منزل_sugov_tunables(attr_سېټ));
}

ساكن ثابت بنية kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

ساكن بنية cpufreq_governor schedutil_gov;

ساكن بنية sugov_policy *sugov_policy_alloc(بنية cpufreq_policy *policy)
{
	بنية sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	إذا (!sg_policy)
		أرجع عدم;

	sg_policy->policy = policy;
	raw_spin_قلف_init(&sg_policy->update_قلف);
	أرجع sg_policy;
}

ساكن فراغ sugov_policy_free(بنية sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

ساكن صحيح sugov_kthread_create(بنية sugov_policy *sg_policy)
{
	بنية task_struct *thread;
	بنية sched_attr attr = {
		.اندازه		= حجم(بنية sched_attr),
		.sched_policy	= SCHED_DEADLINE,
		.sched_flags	= SCHED_FLAG_SUGOV,
		.sched_nice	= 0,
		.sched_لومړیتوب	= 0,
		/*
		 * Fake (unused) bandwidth; workaround to "fix"
		 * priority inheritance.
		 */
		.sched_runtime	= NSEC_فی_MSEC,
		.sched_deadline = 10 * NSEC_فی_MSEC,
		.sched_period	= 10 * NSEC_فی_MSEC,
	};
	بنية cpufreq_policy *policy = sg_policy->policy;
	صحيح ret;

	/* kthread only required for slow path */
	إذا (policy->fast_switch_enabled)
		أرجع 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_کارګر(&sg_policy->کارګر);
	thread = kthread_create(kthread_کارګر_fn, &sg_policy->کارګر,
				"sugov:%d",
				cpumask_لومړی(policy->related_cpus));
	إذا (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		أرجع PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	إذا (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		أرجع ret;
	}

	sg_policy->thread = thread;
	إذا (policy->dvfs_possible_from_any_cpu)
		سېټ_cpus_allowed_ptr(thread, policy->related_cpus);
	وإلا
		kthread_bind_ماسک(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_قلف);

	wake_پورته_process(thread);

	أرجع 0;
}

ساكن فراغ sugov_kthread_stop(بنية sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	إذا (sg_policy->policy->fast_switch_enabled)
		أرجع;

	kthread_flush_کارګر(&sg_policy->کارګر);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_قلف);
}

ساكن بنية sugov_tunables *sugov_tunables_alloc(بنية sugov_policy *sg_policy)
{
	بنية sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	إذا (tunables) {
		gov_attr_سېټ_init(&tunables->attr_سېټ, &sg_policy->tunables_hook);
		إذا (!have_governor_فی_policy())
			global_tunables = tunables;
	}
	أرجع tunables;
}

ساكن فراغ sugov_clear_global_tunables(فراغ)
{
	إذا (!have_governor_فی_policy())
		global_tunables = عدم;
}

ساكن صحيح sugov_init(بنية cpufreq_policy *policy)
{
	بنية sugov_policy *sg_policy;
	بنية sugov_tunables *tunables;
	صحيح ret = 0;

	/* State should be equivalent to EXIT */
	إذا (policy->governor_معلومات)
		أرجع -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	إذا (!sg_policy) {
		ret = -ENOMEM;
		اذهب disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	إذا (ret)
		اذهب free_sg_policy;

	mutex_قلف(&global_tunables_قلف);

	إذا (global_tunables) {
		إذا (WARN_ON(have_governor_فی_policy())) {
			ret = -EINVAL;
			اذهب stop_kthread;
		}
		policy->governor_معلومات = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_سېټ_get(&global_tunables->attr_سېټ, &sg_policy->tunables_hook);
		اذهب بهر;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	إذا (!tunables) {
		ret = -ENOMEM;
		اذهب stop_kthread;
	}

	tunables->شرح_حد_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_معلومات = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_جمع_کړه(&tunables->attr_سېټ.kobj, &sugov_tunables_ktype,
				   get_governor_پلار_kobj(policy), "%s",
				   schedutil_gov.name);
	إذا (ret)
		اذهب fail;

بهر:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_قلف);
	أرجع 0;

fail:
	kobject_put(&tunables->attr_سېټ.kobj);
	policy->governor_معلومات = عدم;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_قلف);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	أرجع ret;
}

ساكن فراغ sugov_exit(بنية cpufreq_policy *policy)
{
	بنية sugov_policy *sg_policy = policy->governor_معلومات;
	بنية sugov_tunables *tunables = sg_policy->tunables;
	بینښه صحيح شمېر;

	mutex_قلف(&global_tunables_قلف);

	شمېر = gov_attr_سېټ_put(&tunables->attr_سېټ, &sg_policy->tunables_hook);
	policy->governor_معلومات = عدم;
	إذا (!شمېر)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_قلف);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

ساكن صحيح sugov_پیل(بنية cpufreq_policy *policy)
{
	بنية sugov_policy *sg_policy = policy->governor_معلومات;
	فراغ (*uu)(بنية update_util_معلومات *معلومات, u64 وخت, بینښه صحيح flags);
	بینښه صحيح cpu;

	sugov_update_شرح_حد_us_sync(sg_policy);
	sg_policy->وروستی_freq_update_وخت	= 0;
	sg_policy->بل_freq			= 0;
	sg_policy->work_دننه_پرمختګ		= false;
	sg_policy->limits_بدل_شوی		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	إذا (policy_is_shared(policy))
		uu = sugov_update_shared;
	وإلا إذا (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	وإلا
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		بنية sugov_cpu *sg_cpu = &فی_cpu(sugov_cpu, cpu);

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
		بنية sugov_cpu *sg_cpu = &فی_cpu(sugov_cpu, cpu);

		cpufreq_جمع_کړه_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	أرجع 0;
}

ساكن فراغ sugov_stop(بنية cpufreq_policy *policy)
{
	بنية sugov_policy *sg_policy = policy->governor_معلومات;
	بینښه صحيح cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	إذا (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

ساكن فراغ sugov_limits(بنية cpufreq_policy *policy)
{
	بنية sugov_policy *sg_policy = policy->governor_معلومات;

	إذا (!policy->fast_switch_enabled) {
		mutex_قلف(&sg_policy->work_قلف);
		cpufreq_policy_apply_limits(policy);
		mutex_unlock(&sg_policy->work_قلف);
	}

	/*
	 * The limits_changed update below must take place before the updates
	 * of policy limits in cpufreq_set_policy() or a policy limits update
	 * might be missed, so use a memory barrier to ensure it.
	 *
	 * This pairs with the memory barrier in sugov_should_update_freq().
	 */
	smp_wmb();

	WRITE_ONCE(sg_policy->limits_بدل_شوی, true);
}

ساكن بنية cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.وتل			= sugov_exit,
	.پیل			= sugov_پیل,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#که_تعریف_شوی CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
بنية cpufreq_governor *cpufreq_default_governor(فراغ)
{
	أرجع &schedutil_gov;
}
#د_که_پای

bool sugov_is_governor(بنية cpufreq_policy *policy)
{
	أرجع policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
