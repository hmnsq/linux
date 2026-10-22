// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler code and data structures related to cpufreq.
 *
 * Copyright (C) 2016, Intel Corporation
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ update_util_ದತ್ತಾಂಶ __rcu *, cpufreq_update_util_ದತ್ತಾಂಶ);

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
ಶೂನ್ಯ cpufreq_ಕೂಡಿಸು_update_util_hook(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ update_util_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ,
			ಶೂನ್ಯ (*func)(ರಚನೆ update_util_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ, u64 ಸಮಯ,
				     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags))
{
	ಆದರೆ (WARN_ON(!ದತ್ತಾಂಶ || !func))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (WARN_ON(ಪ್ರತಿ_cpu(cpufreq_update_util_ದತ್ತಾಂಶ, cpu)))
		ಹಿಂತಿರುಗಿ;

	ದತ್ತಾಂಶ->func = func;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(cpufreq_update_util_ದತ್ತಾಂಶ, cpu), ದತ್ತಾಂಶ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(cpufreq_ಕೂಡಿಸು_update_util_hook);

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
ಶೂನ್ಯ cpufreq_remove_update_util_hook(ಪೂರ್ಣಾಂಕ cpu)
{
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(ಪ್ರತಿ_cpu(cpufreq_update_util_ದತ್ತಾಂಶ, cpu), ಖಾಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(cpufreq_remove_update_util_hook);

/**
 * cpufreq_this_cpu_can_update - Check if cpufreq policy can be updated.
 * @policy: cpufreq policy to check.
 *
 * Return 'true' if:
 * - the local and remote CPUs share @policy,
 * - dvfs_possible_from_any_cpu is set in @policy and the local CPU is not going
 *   offline (in which case it is not expected to run cpufreq updates any more).
 */
bool cpufreq_this_cpu_can_update(ರಚನೆ cpufreq_policy *policy)
{
	ಹಿಂತಿರುಗಿ cpumask_test_cpu(smp_processor_id(), policy->cpus) ||
		(policy->dvfs_possible_from_any_cpu &&
		 rcu_dereference_sched(*this_cpu_ptr(&cpufreq_update_util_ದತ್ತಾಂಶ)));
}
