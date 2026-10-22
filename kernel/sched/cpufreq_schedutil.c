// SPDX-License-Identifier: GPL-2.0
/*
 * CPUFreq governor based on scheduler-provided CPU utilization data.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#समावेश <uapi/linux/sched/types.h>
#समावेश "sched.h"

#घोषणा IOWAIT_BOOST_MIN	(SCHED_CAPACITY_SCALE / 8)

काष्ठा sugov_tunables {
	काष्ठा gov_attr_set	attr_set;
	अचिन्हित पूर्णांक		rate_limit_us;
};

काष्ठा sugov_policy {
	काष्ठा cpufreq_policy	*policy;

	काष्ठा sugov_tunables	*tunables;
	काष्ठा list_शीर्ष	tunables_hook;

	raw_spinlock_t		update_lock;
	u64			last_freq_update_time;
	s64			freq_update_delay_ns;
	अचिन्हित पूर्णांक		अग्र_freq;
	अचिन्हित पूर्णांक		cached_raw_freq;

	/* The next fields are only needed if fast switch cannot be used: */
	काष्ठा			irq_work irq_work;
	काष्ठा			kthread_work work;
	काष्ठा			mutex work_lock;
	काष्ठा			kthread_worker worker;
	काष्ठा task_struct	*thread;
	bool			work_in_progress;

	bool			limits_changed;
	bool			need_freq_update;
};

काष्ठा sugov_cpu {
	काष्ठा update_util_तथ्य	update_util;
	काष्ठा sugov_policy	*sg_policy;
	अचिन्हित पूर्णांक		cpu;

	bool			iowait_boost_pending;
	अचिन्हित पूर्णांक		iowait_boost;
	u64			last_update;

	अचिन्हित दीर्घ		util;
	अचिन्हित दीर्घ		bw_min;
	अचिन्हित दीर्घ		bw_max;

	/* The field below is for single-CPU policies only: */
#अगर_घोषित CONFIG_NO_HZ_COMMON
	अचिन्हित दीर्घ		saved_idle_calls;
#पूर्ण_अगर
};

अटल DEFINE_PER_CPU(काष्ठा sugov_cpu, sugov_cpu);

/************************ Governor internals ***********************/

अटल व्योम sugov_update_rate_limit_us(काष्ठा sugov_policy *sg_policy)
{
	/*
	 * Cast rate_limit_us before multiplication to force 64-bit arithmetic.
	 * Otherwise, on 32-bit platforms, both operands are converted to
	 * 32-bit unsigned long and the multiplication may overflow.
	 */
	sg_policy->freq_update_delay_ns =
		(s64)sg_policy->tunables->rate_limit_us * NSEC_PER_USEC;
}

अटल व्योम sugov_update_rate_limit_us_sync(काष्ठा sugov_policy *sg_policy)
{
	/*
	 * Serialize with rate_limit_us_store() to prevent this path from using
	 * a stale rate limit tunable value to set the limit for the given
	 * policy.
	 */
	guard(mutex)(&sg_policy->tunables->attr_set.update_lock);

	sugov_update_rate_limit_us(sg_policy);
}

अटल bool sugov_should_update_freq(काष्ठा sugov_policy *sg_policy, u64 समय)
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
	अगर (!cpufreq_this_cpu_can_update(sg_policy->policy))
		वापस false;

	अगर (unlikely(READ_ONCE(sg_policy->limits_changed))) {
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

		वापस true;
	} अन्यथा अगर (sg_policy->need_freq_update) {
		/* ignore_dl_rate_limit() wants a new frequency to be found. */
		वापस true;
	}

	delta_ns = समय - sg_policy->last_freq_update_time;

	वापस delta_ns >= sg_policy->freq_update_delay_ns;
}

अटल bool sugov_update_अग्र_freq(काष्ठा sugov_policy *sg_policy, u64 समय,
				   अचिन्हित पूर्णांक अग्र_freq)
{
	अगर (sg_policy->need_freq_update) {
		sg_policy->need_freq_update = false;
		/*
		 * The policy limits have changed, but if the return value of
		 * cpufreq_driver_resolve_freq() after applying the new limits
		 * is still equal to the previously selected frequency, the
		 * driver callback need not be invoked unless the driver
		 * specifically wants that to happen on every update of the
		 * policy limits.
		 */
		अगर (sg_policy->अग्र_freq == अग्र_freq &&
		    !cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS))
			वापस false;
	} अन्यथा अगर (sg_policy->अग्र_freq == अग्र_freq) {
		वापस false;
	}

	sg_policy->अग्र_freq = अग्र_freq;
	sg_policy->last_freq_update_time = समय;

	वापस true;
}

अटल व्योम sugov_deferred_update(काष्ठा sugov_policy *sg_policy)
{
	अगर (!sg_policy->work_in_progress) {
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
अटल __always_inline
अचिन्हित दीर्घ get_capacity_ref_freq(काष्ठा cpufreq_policy *policy)
{
	अचिन्हित पूर्णांक freq = arch_scale_freq_ref(policy->cpu);

	अगर (freq)
		वापस freq;

	अगर (arch_scale_freq_invariant())
		वापस policy->cpuinfo.max_freq;

	/*
	 * Apply a 25% margin so that we select a higher frequency than
	 * the current one before the CPU is fully busy:
	 */
	वापस policy->cur + (policy->cur >> 2);
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
अटल अचिन्हित पूर्णांक get_अग्र_freq(काष्ठा sugov_policy *sg_policy,
				  अचिन्हित दीर्घ util, अचिन्हित दीर्घ max)
{
	काष्ठा cpufreq_policy *policy = sg_policy->policy;
	अचिन्हित पूर्णांक freq;

	freq = get_capacity_ref_freq(policy);
	freq = map_util_freq(util, freq, max);

	अगर (freq == sg_policy->cached_raw_freq && !sg_policy->need_freq_update)
		वापस sg_policy->अग्र_freq;

	sg_policy->cached_raw_freq = freq;
	वापस cpufreq_driver_resolve_freq(policy, freq);
}

अचिन्हित दीर्घ sugov_effective_cpu_perf(पूर्णांक cpu, अचिन्हित दीर्घ actual,
				 अचिन्हित दीर्घ min,
				 अचिन्हित दीर्घ max)
{
	/* Add dvfs headroom to actual utilization */
	actual = map_util_perf(actual);
	/* Actually we don't need to target the max performance */
	अगर (actual < max)
		max = actual;

	/*
	 * Ensure at least minimum performance while providing more compute
	 * capacity when possible.
	 */
	वापस max(min, max);
}

अटल व्योम sugov_get_util(काष्ठा sugov_cpu *sg_cpu, अचिन्हित दीर्घ boost)
{
	अचिन्हित दीर्घ min, max, util = scx_cpuperf_target(sg_cpu->cpu);

	अगर (!scx_switched_all())
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
अटल bool sugov_iowait_reset(काष्ठा sugov_cpu *sg_cpu, u64 समय,
			       bool set_iowait_boost)
{
	s64 delta_ns = समय - sg_cpu->last_update;

	/* Reset boost only if a tick has elapsed since last request */
	अगर (delta_ns <= TICK_NSEC)
		वापस false;

	sg_cpu->iowait_boost = set_iowait_boost ? IOWAIT_BOOST_MIN : 0;
	sg_cpu->iowait_boost_pending = set_iowait_boost;

	वापस true;
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
अटल व्योम sugov_iowait_boost(काष्ठा sugov_cpu *sg_cpu, u64 समय,
			       अचिन्हित पूर्णांक flags)
{
	bool set_iowait_boost = flags & SCHED_CPUFREQ_IOWAIT;

	/* Reset boost if the CPU appears to have been idle enough */
	अगर (sg_cpu->iowait_boost &&
	    sugov_iowait_reset(sg_cpu, समय, set_iowait_boost))
		वापस;

	/* Boost only tasks waking up after IO */
	अगर (!set_iowait_boost)
		वापस;

	/* Ensure boost doubles only one time at each request */
	अगर (sg_cpu->iowait_boost_pending)
		वापस;
	sg_cpu->iowait_boost_pending = true;

	/* Double the boost at each request */
	अगर (sg_cpu->iowait_boost) {
		sg_cpu->iowait_boost =
			min_t(अचिन्हित पूर्णांक, sg_cpu->iowait_boost << 1, SCHED_CAPACITY_SCALE);
		वापस;
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
अटल अचिन्हित दीर्घ sugov_iowait_apply(काष्ठा sugov_cpu *sg_cpu, u64 समय,
			       अचिन्हित दीर्घ max_cap)
{
	/* No boost currently required */
	अगर (!sg_cpu->iowait_boost)
		वापस 0;

	/* Reset boost if the CPU appears to have been idle enough */
	अगर (sugov_iowait_reset(sg_cpu, समय, false))
		वापस 0;

	अगर (!sg_cpu->iowait_boost_pending) {
		/*
		 * No boost pending; reduce the boost value.
		 */
		sg_cpu->iowait_boost >>= 1;
		अगर (sg_cpu->iowait_boost < IOWAIT_BOOST_MIN) {
			sg_cpu->iowait_boost = 0;
			वापस 0;
		}
	}

	sg_cpu->iowait_boost_pending = false;

	/*
	 * sg_cpu->util is already in capacity scale; convert iowait_boost
	 * into the same scale so we can compare.
	 */
	वापस (sg_cpu->iowait_boost * max_cap) >> SCHED_CAPACITY_SHIFT;
}

#अगर_घोषित CONFIG_NO_HZ_COMMON
अटल bool sugov_hold_freq(काष्ठा sugov_cpu *sg_cpu)
{
	अचिन्हित दीर्घ idle_calls;
	bool ret;

	/*
	 * The heuristics in this function is for the fair class. For SCX, the
	 * performance target comes directly from the BPF scheduler. Let's just
	 * follow it.
	 */
	अगर (scx_switched_all())
		वापस false;

	/* if capped by uclamp_max, always update to be in compliance */
	अगर (uclamp_rq_is_capped(cpu_rq(sg_cpu->cpu)))
		वापस false;

	/*
	 * Maintain the frequency if the CPU has not been idle recently, as
	 * reduction is likely to be premature.
	 */
	idle_calls = tick_nohz_get_idle_calls_cpu(sg_cpu->cpu);
	ret = idle_calls == sg_cpu->saved_idle_calls;

	sg_cpu->saved_idle_calls = idle_calls;
	वापस ret;
}
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
अटल अंतरभूत bool sugov_hold_freq(काष्ठा sugov_cpu *sg_cpu) { वापस false; }
#पूर्ण_अगर /* !CONFIG_NO_HZ_COMMON */

/*
 * Make sugov_should_update_freq() ignore the rate limit when DL
 * has increased the utilization.
 */
अटल अंतरभूत व्योम ignore_dl_rate_limit(काष्ठा sugov_cpu *sg_cpu)
{
	अगर (cpu_bw_dl(cpu_rq(sg_cpu->cpu)) > sg_cpu->bw_min)
		sg_cpu->sg_policy->need_freq_update = true;
}

अटल अंतरभूत bool sugov_update_single_common(काष्ठा sugov_cpu *sg_cpu,
					      u64 समय, अचिन्हित दीर्घ max_cap,
					      अचिन्हित पूर्णांक flags)
{
	अचिन्हित दीर्घ boost;

	sugov_iowait_boost(sg_cpu, समय, flags);
	sg_cpu->last_update = समय;

	ignore_dl_rate_limit(sg_cpu);

	अगर (!sugov_should_update_freq(sg_cpu->sg_policy, समय))
		वापस false;

	boost = sugov_iowait_apply(sg_cpu, समय, max_cap);
	sugov_get_util(sg_cpu, boost);

	वापस true;
}

अटल व्योम sugov_update_single_freq(काष्ठा update_util_तथ्य *hook, u64 समय,
				     अचिन्हित पूर्णांक flags)
{
	काष्ठा sugov_cpu *sg_cpu = container_of(hook, काष्ठा sugov_cpu, update_util);
	काष्ठा sugov_policy *sg_policy = sg_cpu->sg_policy;
	अचिन्हित पूर्णांक cached_freq = sg_policy->cached_raw_freq;
	अचिन्हित दीर्घ max_cap;
	अचिन्हित पूर्णांक अग्र_f;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	अगर (!sugov_update_single_common(sg_cpu, समय, max_cap, flags))
		वापस;

	अग्र_f = get_अग्र_freq(sg_policy, sg_cpu->util, max_cap);

	अगर (sugov_hold_freq(sg_cpu) && अग्र_f < sg_policy->अग्र_freq &&
	    !sg_policy->need_freq_update) {
		अग्र_f = sg_policy->अग्र_freq;

		/* Restore cached freq as next_freq has changed */
		sg_policy->cached_raw_freq = cached_freq;
	}

	अगर (!sugov_update_अग्र_freq(sg_policy, समय, अग्र_f))
		वापस;

	/*
	 * This code runs under rq->lock for the target CPU, so it won't run
	 * concurrently on two different CPUs for the same target and it is not
	 * necessary to acquire the lock in the fast switch case.
	 */
	अगर (sg_policy->policy->fast_switch_enabled) {
		cpufreq_driver_fast_switch(sg_policy->policy, अग्र_f);
	} अन्यथा {
		raw_spin_lock(&sg_policy->update_lock);
		sugov_deferred_update(sg_policy);
		raw_spin_unlock(&sg_policy->update_lock);
	}
}

अटल व्योम sugov_update_single_perf(काष्ठा update_util_तथ्य *hook, u64 समय,
				     अचिन्हित पूर्णांक flags)
{
	काष्ठा sugov_cpu *sg_cpu = container_of(hook, काष्ठा sugov_cpu, update_util);
	काष्ठा sugov_policy *sg_policy = sg_cpu->sg_policy;
	अचिन्हित दीर्घ पूर्व_util = sg_cpu->util;
	अचिन्हित दीर्घ max_cap;

	/*
	 * Fall back to the "frequency" path if frequency invariance is not
	 * supported, because the direct mapping between the utilization and
	 * the performance levels depends on the frequency invariance.
	 */
	अगर (!arch_scale_freq_invariant()) {
		sugov_update_single_freq(hook, समय, flags);
		वापस;
	}

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	अगर (!sugov_update_single_common(sg_cpu, समय, max_cap, flags))
		वापस;

	अगर (sugov_hold_freq(sg_cpu) && sg_cpu->util < पूर्व_util)
		sg_cpu->util = पूर्व_util;

	cpufreq_driver_adjust_perf(sg_policy->policy, sg_cpu->bw_min,
				   sg_cpu->util, sg_cpu->bw_max, max_cap);

	sg_policy->need_freq_update = false;
	sg_policy->last_freq_update_time = समय;
}

अटल अचिन्हित पूर्णांक sugov_अग्र_freq_shared(काष्ठा sugov_cpu *sg_cpu, u64 समय)
{
	काष्ठा sugov_policy *sg_policy = sg_cpu->sg_policy;
	काष्ठा cpufreq_policy *policy = sg_policy->policy;
	अचिन्हित दीर्घ util = 0, max_cap;
	अचिन्हित पूर्णांक j;

	max_cap = arch_scale_cpu_capacity(sg_cpu->cpu);

	for_each_cpu(j, policy->cpus) {
		काष्ठा sugov_cpu *j_sg_cpu = &per_cpu(sugov_cpu, j);
		अचिन्हित दीर्घ boost;

		boost = sugov_iowait_apply(j_sg_cpu, समय, max_cap);
		sugov_get_util(j_sg_cpu, boost);

		util = max(j_sg_cpu->util, util);
	}

	वापस get_अग्र_freq(sg_policy, util, max_cap);
}

अटल व्योम
sugov_update_shared(काष्ठा update_util_तथ्य *hook, u64 समय, अचिन्हित पूर्णांक flags)
{
	काष्ठा sugov_cpu *sg_cpu = container_of(hook, काष्ठा sugov_cpu, update_util);
	काष्ठा sugov_policy *sg_policy = sg_cpu->sg_policy;
	अचिन्हित पूर्णांक अग्र_f;

	raw_spin_lock(&sg_policy->update_lock);

	sugov_iowait_boost(sg_cpu, समय, flags);
	sg_cpu->last_update = समय;

	ignore_dl_rate_limit(sg_cpu);

	अगर (sugov_should_update_freq(sg_policy, समय)) {
		अग्र_f = sugov_अग्र_freq_shared(sg_cpu, समय);

		अगर (!sugov_update_अग्र_freq(sg_policy, समय, अग्र_f))
			जाओ unlock;

		अगर (sg_policy->policy->fast_switch_enabled)
			cpufreq_driver_fast_switch(sg_policy->policy, अग्र_f);
		अन्यथा
			sugov_deferred_update(sg_policy);
	}
unlock:
	raw_spin_unlock(&sg_policy->update_lock);
}

अटल व्योम sugov_work(काष्ठा kthread_work *work)
{
	काष्ठा sugov_policy *sg_policy = container_of(work, काष्ठा sugov_policy, work);
	अचिन्हित पूर्णांक freq;
	अचिन्हित दीर्घ flags;

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
	freq = sg_policy->अग्र_freq;
	sg_policy->work_in_progress = false;
	raw_spin_unlock_irqrestore(&sg_policy->update_lock, flags);

	mutex_lock(&sg_policy->work_lock);
	__cpufreq_driver_target(sg_policy->policy, freq, CPUFREQ_RELATION_L);
	mutex_unlock(&sg_policy->work_lock);
}

अटल व्योम sugov_irq_work(काष्ठा irq_work *irq_work)
{
	काष्ठा sugov_policy *sg_policy;

	sg_policy = container_of(irq_work, काष्ठा sugov_policy, irq_work);

	kthread_queue_work(&sg_policy->worker, &sg_policy->work);
}

/************************** sysfs interface ************************/

अटल काष्ठा sugov_tunables *global_tunables;
अटल DEFINE_MUTEX(global_tunables_lock);

अटल अंतरभूत काष्ठा sugov_tunables *to_sugov_tunables(काष्ठा gov_attr_set *attr_set)
{
	वापस container_of(attr_set, काष्ठा sugov_tunables, attr_set);
}

अटल ssize_t rate_limit_us_show(काष्ठा gov_attr_set *attr_set, अक्षर *buf)
{
	काष्ठा sugov_tunables *tunables = to_sugov_tunables(attr_set);

	वापस sysfs_emit(buf, "%u\n", tunables->rate_limit_us);
}

अटल ssize_t
rate_limit_us_store(काष्ठा gov_attr_set *attr_set, स्थिर अक्षर *buf, माप_प्रकार गिनती)
{
	काष्ठा sugov_tunables *tunables = to_sugov_tunables(attr_set);
	काष्ठा sugov_policy *sg_policy;
	अचिन्हित पूर्णांक rate_limit_us;

	अगर (kstrtouint(buf, 10, &rate_limit_us))
		वापस -EINVAL;

	tunables->rate_limit_us = rate_limit_us;

	list_for_each_entry(sg_policy, &attr_set->policy_list, tunables_hook)
		sugov_update_rate_limit_us(sg_policy);

	वापस गिनती;
}

अटल काष्ठा governor_attr rate_limit_us = __ATTR_RW(rate_limit_us);

अटल काष्ठा attribute *sugov_attrs[] = {
	&rate_limit_us.attr,
	शून्य
};
ATTRIBUTE_GROUPS(sugov);

अटल व्योम sugov_tunables_free(काष्ठा kobject *kobj)
{
	काष्ठा gov_attr_set *attr_set = to_gov_attr_set(kobj);

	kfree(to_sugov_tunables(attr_set));
}

अटल स्थिर काष्ठा kobj_type sugov_tunables_ktype = {
	.default_groups = sugov_groups,
	.sysfs_ops = &governor_sysfs_ops,
	.release = &sugov_tunables_free,
};

/********************** cpufreq governor interface *********************/

अटल काष्ठा cpufreq_governor schedutil_gov;

अटल काष्ठा sugov_policy *sugov_policy_alloc(काष्ठा cpufreq_policy *policy)
{
	काष्ठा sugov_policy *sg_policy;

	sg_policy = kzalloc_obj(*sg_policy);
	अगर (!sg_policy)
		वापस शून्य;

	sg_policy->policy = policy;
	raw_spin_lock_init(&sg_policy->update_lock);
	वापस sg_policy;
}

अटल व्योम sugov_policy_free(काष्ठा sugov_policy *sg_policy)
{
	kfree(sg_policy);
}

अटल पूर्णांक sugov_kthread_create(काष्ठा sugov_policy *sg_policy)
{
	काष्ठा task_struct *thread;
	काष्ठा sched_attr attr = {
		.आकार		= माप(काष्ठा sched_attr),
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
	काष्ठा cpufreq_policy *policy = sg_policy->policy;
	पूर्णांक ret;

	/* kthread only required for slow path */
	अगर (policy->fast_switch_enabled)
		वापस 0;

	kthread_init_work(&sg_policy->work, sugov_work);
	kthread_init_worker(&sg_policy->worker);
	thread = kthread_create(kthread_worker_fn, &sg_policy->worker,
				"sugov:%d",
				cpumask_first(policy->related_cpus));
	अगर (IS_ERR(thread)) {
		pr_err("failed to create sugov thread: %pe\n", thread);
		वापस PTR_ERR(thread);
	}

	ret = sched_setattr_nocheck(thread, &attr);
	अगर (ret) {
		kthread_stop(thread);
		pr_warn("%s: failed to set SCHED_DEADLINE\n", __func__);
		वापस ret;
	}

	sg_policy->thread = thread;
	अगर (policy->dvfs_possible_from_any_cpu)
		set_cpus_allowed_ptr(thread, policy->related_cpus);
	अन्यथा
		kthread_bind_mask(thread, policy->related_cpus);

	init_irq_work(&sg_policy->irq_work, sugov_irq_work);
	mutex_init(&sg_policy->work_lock);

	wake_up_process(thread);

	वापस 0;
}

अटल व्योम sugov_kthread_stop(काष्ठा sugov_policy *sg_policy)
{
	/* kthread only required for slow path */
	अगर (sg_policy->policy->fast_switch_enabled)
		वापस;

	kthread_flush_worker(&sg_policy->worker);
	kthread_stop(sg_policy->thread);
	mutex_destroy(&sg_policy->work_lock);
}

अटल काष्ठा sugov_tunables *sugov_tunables_alloc(काष्ठा sugov_policy *sg_policy)
{
	काष्ठा sugov_tunables *tunables;

	tunables = kzalloc_obj(*tunables);
	अगर (tunables) {
		gov_attr_set_init(&tunables->attr_set, &sg_policy->tunables_hook);
		अगर (!have_governor_per_policy())
			global_tunables = tunables;
	}
	वापस tunables;
}

अटल व्योम sugov_clear_global_tunables(व्योम)
{
	अगर (!have_governor_per_policy())
		global_tunables = शून्य;
}

अटल पूर्णांक sugov_init(काष्ठा cpufreq_policy *policy)
{
	काष्ठा sugov_policy *sg_policy;
	काष्ठा sugov_tunables *tunables;
	पूर्णांक ret = 0;

	/* State should be equivalent to EXIT */
	अगर (policy->governor_तथ्य)
		वापस -EBUSY;

	cpufreq_enable_fast_switch(policy);

	sg_policy = sugov_policy_alloc(policy);
	अगर (!sg_policy) {
		ret = -ENOMEM;
		जाओ disable_fast_switch;
	}

	ret = sugov_kthread_create(sg_policy);
	अगर (ret)
		जाओ free_sg_policy;

	mutex_lock(&global_tunables_lock);

	अगर (global_tunables) {
		अगर (WARN_ON(have_governor_per_policy())) {
			ret = -EINVAL;
			जाओ stop_kthread;
		}
		policy->governor_तथ्य = sg_policy;
		sg_policy->tunables = global_tunables;

		gov_attr_set_get(&global_tunables->attr_set, &sg_policy->tunables_hook);
		जाओ out;
	}

	tunables = sugov_tunables_alloc(sg_policy);
	अगर (!tunables) {
		ret = -ENOMEM;
		जाओ stop_kthread;
	}

	tunables->rate_limit_us = cpufreq_policy_transition_delay_us(policy);

	policy->governor_तथ्य = sg_policy;
	sg_policy->tunables = tunables;

	ret = kobject_init_and_add(&tunables->attr_set.kobj, &sugov_tunables_ktype,
				   get_governor_parent_kobj(policy), "%s",
				   schedutil_gov.name);
	अगर (ret)
		जाओ fail;

out:
	/*
	 * Schedutil is the preferred governor for EAS, so rebuild sched domains
	 * on governor changes to make sure the scheduler knows about them.
	 */
	em_rebuild_sched_domains();
	mutex_unlock(&global_tunables_lock);
	वापस 0;

fail:
	kobject_put(&tunables->attr_set.kobj);
	policy->governor_तथ्य = शून्य;
	sugov_clear_global_tunables();

stop_kthread:
	sugov_kthread_stop(sg_policy);
	mutex_unlock(&global_tunables_lock);

free_sg_policy:
	sugov_policy_free(sg_policy);

disable_fast_switch:
	cpufreq_disable_fast_switch(policy);

	pr_err("initialization failed (error %d)\n", ret);
	वापस ret;
}

अटल व्योम sugov_exit(काष्ठा cpufreq_policy *policy)
{
	काष्ठा sugov_policy *sg_policy = policy->governor_तथ्य;
	काष्ठा sugov_tunables *tunables = sg_policy->tunables;
	अचिन्हित पूर्णांक गिनती;

	mutex_lock(&global_tunables_lock);

	गिनती = gov_attr_set_put(&tunables->attr_set, &sg_policy->tunables_hook);
	policy->governor_तथ्य = शून्य;
	अगर (!गिनती)
		sugov_clear_global_tunables();

	mutex_unlock(&global_tunables_lock);

	sugov_kthread_stop(sg_policy);
	sugov_policy_free(sg_policy);
	cpufreq_disable_fast_switch(policy);

	em_rebuild_sched_domains();
}

अटल पूर्णांक sugov_start(काष्ठा cpufreq_policy *policy)
{
	काष्ठा sugov_policy *sg_policy = policy->governor_तथ्य;
	व्योम (*uu)(काष्ठा update_util_तथ्य *तथ्य, u64 समय, अचिन्हित पूर्णांक flags);
	अचिन्हित पूर्णांक cpu;

	sugov_update_rate_limit_us_sync(sg_policy);
	sg_policy->last_freq_update_time	= 0;
	sg_policy->अग्र_freq			= 0;
	sg_policy->work_in_progress		= false;
	sg_policy->limits_changed		= false;
	sg_policy->cached_raw_freq		= 0;

	sg_policy->need_freq_update = cpufreq_driver_test_flags(CPUFREQ_NEED_UPDATE_LIMITS);

	अगर (policy_is_shared(policy))
		uu = sugov_update_shared;
	अन्यथा अगर (policy->fast_switch_enabled && cpufreq_driver_has_adjust_perf())
		uu = sugov_update_single_perf;
	अन्यथा
		uu = sugov_update_single_freq;

	for_each_cpu(cpu, policy->cpus) {
		काष्ठा sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		स_रखो(sg_cpu, 0, माप(*sg_cpu));
		sg_cpu->cpu = cpu;
		sg_cpu->sg_policy = sg_policy;
	}

	/*
	 * Publish the hooks only after all per-CPU data is initialized, so a
	 * shared policy's sugov_update_shared() never reads an uninitialized
	 * sibling sugov_cpu.
	 */
	for_each_cpu(cpu, policy->cpus) {
		काष्ठा sugov_cpu *sg_cpu = &per_cpu(sugov_cpu, cpu);

		cpufreq_add_update_util_hook(cpu, &sg_cpu->update_util, uu);
	}

	वापस 0;
}

अटल व्योम sugov_stop(काष्ठा cpufreq_policy *policy)
{
	काष्ठा sugov_policy *sg_policy = policy->governor_तथ्य;
	अचिन्हित पूर्णांक cpu;

	for_each_cpu(cpu, policy->cpus)
		cpufreq_remove_update_util_hook(cpu);

	synchronize_rcu();

	अगर (!policy->fast_switch_enabled) {
		irq_work_sync(&sg_policy->irq_work);
		kthread_cancel_work_sync(&sg_policy->work);
	}
}

अटल व्योम sugov_limits(काष्ठा cpufreq_policy *policy)
{
	काष्ठा sugov_policy *sg_policy = policy->governor_तथ्य;

	अगर (!policy->fast_switch_enabled) {
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

अटल काष्ठा cpufreq_governor schedutil_gov = {
	.name			= "schedutil",
	.owner			= THIS_MODULE,
	.flags			= CPUFREQ_GOV_DYNAMIC_SWITCHING,
	.init			= sugov_init,
	.निकास			= sugov_exit,
	.start			= sugov_start,
	.stop			= sugov_stop,
	.limits			= sugov_limits,
};

#अगर_घोषित CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL
काष्ठा cpufreq_governor *cpufreq_default_governor(व्योम)
{
	वापस &schedutil_gov;
}
#पूर्ण_अगर

bool sugov_is_governor(काष्ठा cpufreq_policy *policy)
{
	वापस policy->governor == &schedutil_gov;
}

cpufreq_governor_init(schedutil_gov);
