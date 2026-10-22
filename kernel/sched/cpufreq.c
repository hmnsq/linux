// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler code and data structures related to cpufreq.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#అన్తర్భావ "sched.h"

DEFINE_చొప్పున_CPU(నిర్మాణం update_util_దత్తాంశం __rcu *, cpufreq_update_util_దత్తాంశం);

/**
 * cpufreq_add_update_util_hook - Populate the CPU's update_util_data pointer.
 * @cpu: The CPU to set the pointer for.
 * @data: New pointer value.
 * @func: Callback function to set for the CPU.
 *
 * Set and publish the update_util_data pointer for the given CPU.
 *
 * The update_util_data pointer of @cpu is set to @data and the callback
 * function pointer in the target struct update_util_data is set to @func.
 * That function will be called by cpufreq_update_util() from RCU-sched
 * read-side critical sections, so it must not sleep.  @data will always be
 * passed to it as the first argument which allows the function to get to the
 * target update_util_data structure and its container.
 *
 * The update_util_data pointer of @cpu must be NULL when this function is
 * called or it will WARN() and return with no effect.
 */
శూన్య cpufreq_కూడు_update_util_hook(పూర్ణాంకం cpu, నిర్మాణం update_util_దత్తాంశం *దత్తాంశం,
			శూన్య (*func)(నిర్మాణం update_util_దత్తాంశం *దత్తాంశం, u64 సమయం,
				     గుర్తులేని పూర్ణాంకం flags))
{
	ఒకవేళ (WARN_ON(!దత్తాంశం || !func))
		తిరిగి;

	ఒకవేళ (WARN_ON(చొప్పున_cpu(cpufreq_update_util_దత్తాంశం, cpu)))
		తిరిగి;

	దత్తాంశం->func = func;
	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(cpufreq_update_util_దత్తాంశం, cpu), దత్తాంశం);
}
EXPORT_చిహ్నం_GPL(cpufreq_కూడు_update_util_hook);

/**
 * cpufreq_remove_update_util_hook - Clear the CPU's update_util_data pointer.
 * @cpu: The CPU to clear the pointer for.
 *
 * Clear the update_util_data pointer for the given CPU.
 *
 * Callers must use RCU callbacks to free any memory that might be
 * accessed via the old update_util_data pointer or invoke synchronize_rcu()
 * right after this function to avoid use-after-free.
 */
శూన్య cpufreq_remove_update_util_hook(పూర్ణాంకం cpu)
{
	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(cpufreq_update_util_దత్తాంశం, cpu), ఖాళీ);
}
EXPORT_చిహ్నం_GPL(cpufreq_remove_update_util_hook);

/**
 * cpufreq_this_cpu_can_update - Check if cpufreq policy can be updated.
 * @policy: cpufreq policy to check.
 *
 * Return 'true' if:
 * - the local and remote CPUs share @policy,
 * - dvfs_possible_from_any_cpu is set in @policy and the local CPU is not going
 *   offline (in which case it is not expected to run cpufreq updates any more).
 */
bool cpufreq_this_cpu_can_update(నిర్మాణం cpufreq_policy *policy)
{
	తిరిగి cpumask_test_cpu(smp_processor_id(), policy->cpus) ||
		(policy->dvfs_possible_from_any_cpu &&
		 rcu_dereference_sched(*this_cpu_ptr(&cpufreq_update_util_దత్తాంశం)));
}
