// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler code and data structures related to cpufreq.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ update_util_ਅੰਕੜੇ __rcu *, cpufreq_update_util_ਅੰਕੜੇ);

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
ਖਾਲੀ cpufreq_ਜੋੜੋ_update_util_hook(ਪੂਰਨ cpu, ਬਣਤਰ update_util_ਅੰਕੜੇ *ਅੰਕੜੇ,
			ਖਾਲੀ (*func)(ਬਣਤਰ update_util_ਅੰਕੜੇ *ਅੰਕੜੇ, u64 ਸਮਾਂ,
				     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags))
{
	ਜੇ (WARN_ON(!ਅੰਕੜੇ || !func))
		ਵਾਪਸ;

	ਜੇ (WARN_ON(ਪ੍ਰਤੀ_cpu(cpufreq_update_util_ਅੰਕੜੇ, cpu)))
		ਵਾਪਸ;

	ਅੰਕੜੇ->func = func;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(cpufreq_update_util_ਅੰਕੜੇ, cpu), ਅੰਕੜੇ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(cpufreq_ਜੋੜੋ_update_util_hook);

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
ਖਾਲੀ cpufreq_remove_update_util_hook(ਪੂਰਨ cpu)
{
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(cpufreq_update_util_ਅੰਕੜੇ, cpu), ਨਿਰਾ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(cpufreq_remove_update_util_hook);

/**
 * cpufreq_this_cpu_can_update - Check if cpufreq policy can be updated.
 * @policy: cpufreq policy to check.
 *
 * Return 'true' if:
 * - the local and remote CPUs share @policy,
 * - dvfs_possible_from_any_cpu is set in @policy and the local CPU is not going
 *   offline (in which case it is not expected to run cpufreq updates any more).
 */
bool cpufreq_this_cpu_can_update(ਬਣਤਰ cpufreq_policy *policy)
{
	ਵਾਪਸ cpumask_test_cpu(smp_processor_id(), policy->cpus) ||
		(policy->dvfs_possible_from_any_cpu &&
		 rcu_dereference_sched(*this_cpu_ptr(&cpufreq_update_util_ਅੰਕੜੇ)));
}
