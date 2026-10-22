// SPDX-License-Identifier: GPL-2.0
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Built-in idle CPU tracking policy.
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cid.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "idle.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sub.h"

/* Enable/disable built-in idle CPU selection policy */
ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_FALSE(scx_builtin_idle_enabled);

/* Enable/disable per-node idle cpumasks */
ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_FALSE(scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ);

/* Enable/disable LLC aware optimizations */
ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_FALSE(scx_selcpu_topo_llc);

/* Enable/disable NUMA aware optimizations */
ਸਥਿਰ DEFINE_STATIC_ਕੁੰਜੀ_FALSE(scx_selcpu_topo_numa);

/*
 * cpumasks to track idle CPUs within each NUMA node.
 *
 * If SCX_OPS_BUILTIN_IDLE_PER_NODE is not enabled, a single global cpumask
 * from is used to track all the idle CPUs in the system.
 */
ਬਣਤਰ scx_idle_cpus {
	cpumask_var_t cpu;
	cpumask_var_t smt;
};

/*
 * Global host-wide idle cpumasks (used when SCX_OPS_BUILTIN_IDLE_PER_NODE
 * is not enabled).
 */
ਸਥਿਰ ਬਣਤਰ scx_idle_cpus scx_idle_global_masks;

/*
 * Per-node idle cpumasks.
 */
ਸਥਿਰ ਬਣਤਰ scx_idle_cpus **scx_idle_ਗੰਢ_masks;

/*
 * Local per-CPU cpumasks (used to generate temporary idle cpumasks).
 */
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(cpumask_var_t, local_idle_cpumask);
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(cpumask_var_t, local_llc_idle_cpumask);
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(cpumask_var_t, local_numa_idle_cpumask);

/*
 * Return the idle masks associated to a target @node.
 *
 * NUMA_NO_NODE identifies the global idle cpumask.
 */
ਸਥਿਰ ਬਣਤਰ scx_idle_cpus *idle_cpumask(ਪੂਰਨ ਗੰਢ)
{
	ਵਾਪਸ ਗੰਢ == NUMA_NO_ਗੰਢ ? &scx_idle_global_masks : scx_idle_ਗੰਢ_masks[ਗੰਢ];
}

/*
 * Returns the NUMA node ID associated with a @cpu, or NUMA_NO_NODE if
 * per-node idle cpumasks are disabled.
 */
ਸਥਿਰ ਪੂਰਨ scx_cpu_ਗੰਢ_if_enabled(ਪੂਰਨ cpu)
{
	ਜੇ (!static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ))
		ਵਾਪਸ NUMA_NO_ਗੰਢ;

	ਵਾਪਸ cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);
}

ਸਥਿਰ bool scx_idle_test_and_clear_cpu(ਪੂਰਨ cpu)
{
	ਪੂਰਨ ਗੰਢ = scx_cpu_ਗੰਢ_if_enabled(cpu);
	ਬਣਤਰ cpumask *idle_cpus = idle_cpumask(ਗੰਢ)->cpu;

	/*
	 * SMT mask should be cleared whether we can claim @cpu or not. The SMT
	 * cluster is not wholly idle either way. This also prevents
	 * scx_pick_idle_cpu() from getting caught in an infinite loop.
	 */
	ਜੇ (sched_smt_ਸਰਗਰਮ()) {
		ਅਚਲ ਬਣਤਰ cpumask *smt = cpu_smt_ਨਕਾਬ(cpu);
		ਬਣਤਰ cpumask *idle_smts = idle_cpumask(ਗੰਢ)->smt;

		/*
		 * If offline, @cpu is not its own sibling and
		 * scx_pick_idle_cpu() can get caught in an infinite loop as
		 * @cpu is never cleared from the idle SMT mask. Ensure that
		 * @cpu is eventually cleared.
		 *
		 * NOTE: Use cpumask_intersects() and cpumask_test_cpu() to
		 * reduce memory writes, which may help alleviate cache
		 * coherence pressure.
		 */
		ਜੇ (cpumask_intersects(smt, idle_smts))
			cpumask_andnot(idle_smts, idle_smts, smt);
		ਨਹੀਂਤਾਂ ਜੇ (cpumask_test_cpu(cpu, idle_smts))
			__cpumask_clear_cpu(cpu, idle_smts);
	}

	ਵਾਪਸ cpumask_test_and_clear_cpu(cpu, idle_cpus);
}

/*
 * Pick an idle CPU in a specific NUMA node.
 */
ਸਥਿਰ s32 pick_idle_cpu_ਵਿੱਚ_ਗੰਢ(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed, ਪੂਰਨ ਗੰਢ, u64 flags)
{
	ਪੂਰਨ cpu;

retry:
	ਜੇ (sched_smt_ਸਰਗਰਮ()) {
		cpu = cpumask_any_and_distribute(idle_cpumask(ਗੰਢ)->smt, cpus_allowed);
		ਜੇ (cpu < nr_cpu_ids)
			ਜਾਓ ਮਿਲਿਆ;

		ਜੇ (flags & SCX_PICK_IDLE_CORE)
			ਵਾਪਸ -EBUSY;
	}

	cpu = cpumask_any_and_distribute(idle_cpumask(ਗੰਢ)->cpu, cpus_allowed);
	ਜੇ (cpu >= nr_cpu_ids)
		ਵਾਪਸ -EBUSY;

ਮਿਲਿਆ:
	ਜੇ (scx_idle_test_and_clear_cpu(cpu))
		ਵਾਪਸ cpu;
	ਨਹੀਂਤਾਂ
		ਜਾਓ retry;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA
/*
 * Tracks nodes that have not yet been visited when searching for an idle
 * CPU across all available nodes.
 */
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(nodemask_t, ਪ੍ਰਤੀ_cpu_unvisited);

/*
 * Search for an idle CPU across all nodes, excluding @node.
 */
ਸਥਿਰ s32 pick_idle_cpu_from_online_nodes(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed, ਪੂਰਨ ਗੰਢ, u64 flags)
{
	nodemask_t *unvisited;
	s32 cpu = -EBUSY;

	preempt_disable();
	unvisited = this_cpu_ptr(&ਪ੍ਰਤੀ_cpu_unvisited);

	/*
	 * Restrict the search to the online nodes (excluding the current
	 * node that has been visited already).
	 */
	nodes_copy(*unvisited, ਗੰਢ_ਹਾਲਤਾਂ[N_ONLINE]);
	ਗੰਢ_clear(ਗੰਢ, *unvisited);

	/*
	 * Traverse all nodes in order of increasing distance, starting
	 * from @node.
	 *
	 * This loop is O(N^2), with N being the amount of NUMA nodes,
	 * which might be quite expensive in large NUMA systems. However,
	 * this complexity comes into play only when a scheduler enables
	 * SCX_OPS_BUILTIN_IDLE_PER_NODE and it's requesting an idle CPU
	 * without specifying a target NUMA node, so it shouldn't be a
	 * bottleneck is most cases.
	 *
	 * As a future optimization we may want to cache the list of nodes
	 * in a per-node array, instead of actually traversing them every
	 * time.
	 */
	for_each_ਗੰਢ_numadist(ਗੰਢ, *unvisited) {
		cpu = pick_idle_cpu_ਵਿੱਚ_ਗੰਢ(cpus_allowed, ਗੰਢ, flags);
		ਜੇ (cpu >= 0)
			ਰੋਕੋ;
	}
	preempt_enable();

	ਵਾਪਸ cpu;
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ s32
pick_idle_cpu_from_online_nodes(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed, ਪੂਰਨ ਗੰਢ, u64 flags)
{
	ਵਾਪਸ -EBUSY;
}
#ਜੇ_ਅੰਤ

/*
 * Find an idle CPU in the system, starting from @node.
 */
ਸਥਿਰ s32 scx_pick_idle_cpu(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed, ਪੂਰਨ ਗੰਢ, u64 flags)
{
	s32 cpu;

	/*
	 * Always search in the starting node first (this is an
	 * optimization that can save some cycles even when the search is
	 * not limited to a single node).
	 */
	cpu = pick_idle_cpu_ਵਿੱਚ_ਗੰਢ(cpus_allowed, ਗੰਢ, flags);
	ਜੇ (cpu >= 0)
		ਵਾਪਸ cpu;

	/*
	 * Stop the search if we are using only a single global cpumask
	 * (NUMA_NO_NODE) or if the search is restricted to the first node
	 * only.
	 */
	ਜੇ (ਗੰਢ == NUMA_NO_ਗੰਢ || flags & SCX_PICK_IDLE_ਵਿੱਚ_ਗੰਢ)
		ਵਾਪਸ -EBUSY;

	/*
	 * Extend the search to the other online nodes.
	 */
	ਵਾਪਸ pick_idle_cpu_from_online_nodes(cpus_allowed, ਗੰਢ, flags);
}

/*
 * Return the amount of CPUs in the same LLC domain of @cpu (or zero if the LLC
 * domain is not defined).
 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_ਭਾਰ(s32 cpu)
{
	ਬਣਤਰ sched_domain *sd;

	sd = rcu_dereference(ਪ੍ਰਤੀ_cpu(sd_llc, cpu));
	ਜੇ (!sd)
		ਵਾਪਸ 0;

	ਵਾਪਸ sd->ਵਿਸਤਾਰ_ਭਾਰ;
}

/*
 * Return the cpumask representing the LLC domain of @cpu (or NULL if the LLC
 * domain is not defined).
 */
ਸਥਿਰ ਬਣਤਰ cpumask *llc_ਵਿਸਤਾਰ(s32 cpu)
{
	ਬਣਤਰ sched_domain *sd;

	sd = rcu_dereference(ਪ੍ਰਤੀ_cpu(sd_llc, cpu));
	ਜੇ (!sd)
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ sched_domain_ਵਿਸਤਾਰ(sd);
}

/*
 * Return the amount of CPUs in the same NUMA domain of @cpu (or zero if the
 * NUMA domain is not defined).
 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ numa_ਭਾਰ(s32 cpu)
{
	ਬਣਤਰ sched_domain *sd;
	ਬਣਤਰ sched_ਸਮੂਹ *sg;

	sd = rcu_dereference(ਪ੍ਰਤੀ_cpu(sd_numa, cpu));
	ਜੇ (!sd)
		ਵਾਪਸ 0;
	sg = sd->groups;
	ਜੇ (!sg)
		ਵਾਪਸ 0;

	ਵਾਪਸ sg->ਸਮੂਹ_ਭਾਰ;
}

/*
 * Return the cpumask representing the NUMA domain of @cpu (or NULL if the NUMA
 * domain is not defined).
 */
ਸਥਿਰ ਬਣਤਰ cpumask *numa_ਵਿਸਤਾਰ(s32 cpu)
{
	ਬਣਤਰ sched_domain *sd;
	ਬਣਤਰ sched_ਸਮੂਹ *sg;

	sd = rcu_dereference(ਪ੍ਰਤੀ_cpu(sd_numa, cpu));
	ਜੇ (!sd)
		ਵਾਪਸ ਨਿਰਾ;
	sg = sd->groups;
	ਜੇ (!sg)
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg);
}

/*
 * Return true if the LLC domains do not perfectly overlap with the NUMA
 * domains, false otherwise.
 */
ਸਥਿਰ bool llc_numa_mismatch(ਖਾਲੀ)
{
	ਪੂਰਨ cpu;

	/*
	 * We need to scan all online CPUs to verify whether their scheduling
	 * domains overlap.
	 *
	 * While it is rare to encounter architectures with asymmetric NUMA
	 * topologies, CPU hotplugging or virtualized environments can result
	 * in asymmetric configurations.
	 *
	 * For example:
	 *
	 *  NUMA 0:
	 *    - LLC 0: cpu0..cpu7
	 *    - LLC 1: cpu8..cpu15 [offline]
	 *
	 *  NUMA 1:
	 *    - LLC 0: cpu16..cpu23
	 *    - LLC 1: cpu24..cpu31
	 *
	 * In this case, if we only check the first online CPU (cpu0), we might
	 * incorrectly assume that the LLC and NUMA domains are fully
	 * overlapping, which is incorrect (as NUMA 1 has two distinct LLC
	 * domains).
	 */
	for_each_online_cpu(cpu)
		ਜੇ (llc_ਭਾਰ(cpu) != numa_ਭਾਰ(cpu))
			ਵਾਪਸ true;

	ਵਾਪਸ false;
}

/*
 * Initialize topology-aware scheduling.
 *
 * Detect if the system has multiple LLC or multiple NUMA domains and enable
 * cache-aware / NUMA-aware scheduling optimizations in the default CPU idle
 * selection policy.
 *
 * Assumption: the kernel's internal topology representation assumes that each
 * CPU belongs to a single LLC domain, and that each LLC domain is entirely
 * contained within a single NUMA node.
 */
ਖਾਲੀ scx_idle_update_selcpu_topology(ਬਣਤਰ sched_ext_ops *ops)
{
	bool enable_llc = false, enable_numa = false;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_cpus;
	s32 cpu = cpumask_ਪਹਿਲਾ(cpu_online_ਨਕਾਬ);

	/*
	 * Enable LLC domain optimization only when there are multiple LLC
	 * domains among the online CPUs. If all online CPUs are part of a
	 * single LLC domain, the idle CPU selection logic can choose any
	 * online CPU without bias.
	 *
	 * Note that it is sufficient to check the LLC domain of the first
	 * online CPU to determine whether a single LLC domain includes all
	 * CPUs.
	 */
	rcu_read_ਤਾਲਾ();
	nr_cpus = llc_ਭਾਰ(cpu);
	ਜੇ (nr_cpus > 0) {
		ਜੇ (nr_cpus < ਅੰਕ_online_cpus())
			enable_llc = true;
		pr_debug("sched_ext: LLC=%*pb weight=%u\n",
			 cpumask_pr_ਦਲੀਲਾਂ(llc_ਵਿਸਤਾਰ(cpu)), llc_ਭਾਰ(cpu));
	}

	/*
	 * Enable NUMA optimization only when there are multiple NUMA domains
	 * among the online CPUs and the NUMA domains don't perfectly overlap
	 * with the LLC domains.
	 *
	 * If all CPUs belong to the same NUMA node and the same LLC domain,
	 * enabling both NUMA and LLC optimizations is unnecessary, as checking
	 * for an idle CPU in the same domain twice is redundant.
	 *
	 * If SCX_OPS_BUILTIN_IDLE_PER_NODE is enabled ignore the NUMA
	 * optimization, as we would naturally select idle CPUs within
	 * specific NUMA nodes querying the corresponding per-node cpumask.
	 */
	ਜੇ (!(ops->flags & SCX_OPS_BUILTIN_IDLE_ਪ੍ਰਤੀ_ਗੰਢ)) {
		nr_cpus = numa_ਭਾਰ(cpu);
		ਜੇ (nr_cpus > 0) {
			ਜੇ (nr_cpus < ਅੰਕ_online_cpus() && llc_numa_mismatch())
				enable_numa = true;
			pr_debug("sched_ext: NUMA=%*pb weight=%u\n",
				 cpumask_pr_ਦਲੀਲਾਂ(numa_ਵਿਸਤਾਰ(cpu)), nr_cpus);
		}
	}
	rcu_read_unlock();

	pr_debug("sched_ext: LLC idle selection %s\n",
		 str_enabled_disabled(enable_llc));
	pr_debug("sched_ext: NUMA idle selection %s\n",
		 str_enabled_disabled(enable_numa));

	ਜੇ (enable_llc)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_llc);
	ਨਹੀਂਤਾਂ
		static_branch_disable_cpuslocked(&scx_selcpu_topo_llc);
	ਜੇ (enable_numa)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_numa);
	ਨਹੀਂਤਾਂ
		static_branch_disable_cpuslocked(&scx_selcpu_topo_numa);
}

/*
 * Return true if @p can run on all possible CPUs, false otherwise.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_affinity_all(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->nr_cpus_allowed >= ਅੰਕ_possible_cpus();
}

/*
 * Built-in CPU idle selection policy:
 *
 * 1. Prioritize full-idle cores:
 *   - always prioritize CPUs from fully idle cores (both logical CPUs are
 *     idle) to avoid interference caused by SMT.
 *
 * 2. Reuse the same CPU:
 *   - prefer the last used CPU to take advantage of cached data (L1, L2) and
 *     branch prediction optimizations.
 *
 * 3. Prefer @prev_cpu's SMT sibling:
 *   - if @prev_cpu is busy and no fully idle core is available, try to
 *     place the task on an idle SMT sibling of @prev_cpu; keeping the
 *     task on the same core makes migration cheaper, preserves L1 cache
 *     locality and reduces wakeup latency.
 *
 * 4. Pick a CPU within the same LLC (Last-Level Cache):
 *   - if the above conditions aren't met, pick a CPU that shares the same
 *     LLC, if the LLC domain is a subset of @cpus_allowed, to maintain
 *     cache locality.
 *
 * 5. Pick a CPU within the same NUMA node, if enabled:
 *   - choose a CPU from the same NUMA node, if the node cpumask is a
 *     subset of @cpus_allowed, to reduce memory access latency.
 *
 * 6. Pick any idle CPU within the @cpus_allowed domain.
 *
 * Step 4 and 5 are performed only if the system has, respectively,
 * multiple LLCs / multiple NUMA nodes (see scx_selcpu_topo_llc and
 * scx_selcpu_topo_numa) and they don't contain the same subset of CPUs.
 *
 * If %SCX_OPS_BUILTIN_IDLE_PER_NODE is enabled, the search will always
 * begin in @prev_cpu's node and proceed to other nodes in order of
 * increasing distance.
 *
 * Return the picked CPU if idle, or a negative value otherwise.
 *
 * NOTE: tasks that can only run on 1 CPU are excluded by this logic, because
 * we never call ops.select_cpu() for them, see select_task_rq().
 */
s32 scx_select_cpu_dfl(ਬਣਤਰ task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed, u64 flags)
{
	ਅਚਲ ਬਣਤਰ cpumask *llc_cpus = ਨਿਰਾ, *numa_cpus = ਨਿਰਾ;
	ਅਚਲ ਬਣਤਰ cpumask *allowed = cpus_allowed ?: p->cpus_ptr;
	ਪੂਰਨ ਗੰਢ = scx_cpu_ਗੰਢ_if_enabled(prev_cpu);
	bool is_prev_allowed;
	s32 cpu;

	preempt_disable();

	/*
	 * Determine the subset of CPUs usable by @p within @cpus_allowed.
	 */
	ਜੇ (allowed != p->cpus_ptr) {
		ਬਣਤਰ cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_idle_cpumask);

		ਜੇ (task_affinity_all(p)) {
			allowed = cpus_allowed;
		} ਨਹੀਂਤਾਂ ਜੇ (cpumask_and(local_cpus, cpus_allowed, p->cpus_ptr)) {
			allowed = local_cpus;
		} ਨਹੀਂਤਾਂ {
			cpu = -EBUSY;
			ਜਾਓ ਬਾਹਰਲਾ_enable;
		}
	}

	/*
	 * Check whether @prev_cpu is still within the allowed set. If not,
	 * we can still try selecting a nearby CPU.
	 */
	is_prev_allowed = cpumask_test_cpu(prev_cpu, allowed);

	/*
	 * This is necessary to protect llc_cpus.
	 */
	rcu_read_ਤਾਲਾ();

	/*
	 * Determine the subset of CPUs that the task can use in its
	 * current LLC and node.
	 *
	 * If the task can run on all CPUs, use the node and LLC cpumasks
	 * directly.
	 */
	ਜੇ (static_branch_maybe(CONFIG_NUMA, &scx_selcpu_topo_numa)) {
		ਬਣਤਰ cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_numa_idle_cpumask);
		ਅਚਲ ਬਣਤਰ cpumask *cpus = numa_ਵਿਸਤਾਰ(prev_cpu);

		ਜੇ (allowed == p->cpus_ptr && task_affinity_all(p))
			numa_cpus = cpus;
		ਨਹੀਂਤਾਂ ਜੇ (cpus && cpumask_and(local_cpus, allowed, cpus))
			numa_cpus = local_cpus;
	}

	ਜੇ (static_branch_maybe(CONFIG_SCHED_MC, &scx_selcpu_topo_llc)) {
		ਬਣਤਰ cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_llc_idle_cpumask);
		ਅਚਲ ਬਣਤਰ cpumask *cpus = llc_ਵਿਸਤਾਰ(prev_cpu);

		ਜੇ (allowed == p->cpus_ptr && task_affinity_all(p))
			llc_cpus = cpus;
		ਨਹੀਂਤਾਂ ਜੇ (cpus && cpumask_and(local_cpus, allowed, cpus))
			llc_cpus = local_cpus;
	}

	/*
	 * If WAKE_SYNC, try to migrate the wakee to the waker's CPU.
	 */
	ਜੇ (wake_flags & SCX_WAKE_SYNC) {
		ਪੂਰਨ waker_ਗੰਢ;

		/*
		 * If the waker's CPU is cache affine and prev_cpu is idle,
		 * then avoid a migration.
		 */
		cpu = smp_processor_id();
		ਜੇ (is_prev_allowed && cpus_share_cache(cpu, prev_cpu) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			ਜਾਓ ਬਾਹਰਲਾ_unlock;
		}

		/*
		 * If the waker's local DSQ is empty, and the system is under
		 * utilized, try to wake up @p to the local DSQ of the waker.
		 *
		 * Checking only for an empty local DSQ is insufficient as it
		 * could give the wakee an unfair advantage when the system is
		 * oversaturated.
		 *
		 * Checking only for the presence of idle CPUs is also
		 * insufficient as the local DSQ of the waker could have tasks
		 * piled up on it even if there is an idle core elsewhere on
		 * the system.
		 */
		waker_ਗੰਢ = scx_cpu_ਗੰਢ_if_enabled(cpu);
		ਜੇ (!(current->flags & PF_EXITING) &&
		    cpu_rq(cpu)->scx.local_dsq.nr == 0 &&
		    (!(flags & SCX_PICK_IDLE_ਵਿੱਚ_ਗੰਢ) || (waker_ਗੰਢ == ਗੰਢ)) &&
		    !cpumask_ਖਾਲੀ_ਥਾਵਾਂ(idle_cpumask(waker_ਗੰਢ)->cpu)) {
			ਜੇ (cpumask_test_cpu(cpu, allowed)) {
				scx_idle_test_and_clear_cpu(cpu);
				ਜਾਓ ਬਾਹਰਲਾ_unlock;
			}
		}
	}

	/*
	 * If CPU has SMT, any wholly idle CPU is likely a better pick than
	 * partially idle @prev_cpu.
	 */
	ਜੇ (sched_smt_ਸਰਗਰਮ()) {
		/*
		 * Keep using @prev_cpu if it's part of a fully idle core.
		 */
		ਜੇ (is_prev_allowed &&
		    cpumask_test_cpu(prev_cpu, idle_cpumask(ਗੰਢ)->smt) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			ਜਾਓ ਬਾਹਰਲਾ_unlock;
		}

		/*
		 * Search for any fully idle core in the same LLC domain.
		 */
		ਜੇ (llc_cpus) {
			cpu = pick_idle_cpu_ਵਿੱਚ_ਗੰਢ(llc_cpus, ਗੰਢ, SCX_PICK_IDLE_CORE);
			ਜੇ (cpu >= 0)
				ਜਾਓ ਬਾਹਰਲਾ_unlock;
		}

		/*
		 * Search for any fully idle core in the same NUMA node.
		 */
		ਜੇ (numa_cpus) {
			cpu = pick_idle_cpu_ਵਿੱਚ_ਗੰਢ(numa_cpus, ਗੰਢ, SCX_PICK_IDLE_CORE);
			ਜੇ (cpu >= 0)
				ਜਾਓ ਬਾਹਰਲਾ_unlock;
		}

		/*
		 * Search for any full-idle core usable by the task.
		 *
		 * If the node-aware idle CPU selection policy is enabled
		 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always
		 * begin in prev_cpu's node and proceed to other nodes in
		 * order of increasing distance.
		 */
		cpu = scx_pick_idle_cpu(allowed, ਗੰਢ, flags | SCX_PICK_IDLE_CORE);
		ਜੇ (cpu >= 0)
			ਜਾਓ ਬਾਹਰਲਾ_unlock;

		/*
		 * Give up if we're strictly looking for a full-idle SMT
		 * core.
		 */
		ਜੇ (flags & SCX_PICK_IDLE_CORE) {
			cpu = -EBUSY;
			ਜਾਓ ਬਾਹਰਲਾ_unlock;
		}
	}

	/*
	 * Use @prev_cpu if it's idle.
	 */
	ਜੇ (is_prev_allowed && scx_idle_test_and_clear_cpu(prev_cpu)) {
		cpu = prev_cpu;
		ਜਾਓ ਬਾਹਰਲਾ_unlock;
	}

	/*
	 * Use @prev_cpu's sibling if it's idle.
	 */
	ਜੇ (sched_smt_ਸਰਗਰਮ()) {
		for_each_cpu_and(cpu, cpu_smt_ਨਕਾਬ(prev_cpu), allowed) {
			ਜੇ (cpu == prev_cpu)
				ਜਾਰੀ;
			ਜੇ (scx_idle_test_and_clear_cpu(cpu))
				ਜਾਓ ਬਾਹਰਲਾ_unlock;
		}
	}

	/*
	 * Search for any idle CPU in the same LLC domain.
	 */
	ਜੇ (llc_cpus) {
		cpu = pick_idle_cpu_ਵਿੱਚ_ਗੰਢ(llc_cpus, ਗੰਢ, 0);
		ਜੇ (cpu >= 0)
			ਜਾਓ ਬਾਹਰਲਾ_unlock;
	}

	/*
	 * Search for any idle CPU in the same NUMA node.
	 */
	ਜੇ (numa_cpus) {
		cpu = pick_idle_cpu_ਵਿੱਚ_ਗੰਢ(numa_cpus, ਗੰਢ, 0);
		ਜੇ (cpu >= 0)
			ਜਾਓ ਬਾਹਰਲਾ_unlock;
	}

	/*
	 * Search for any idle CPU usable by the task.
	 *
	 * If the node-aware idle CPU selection policy is enabled
	 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always begin
	 * in prev_cpu's node and proceed to other nodes in order of
	 * increasing distance.
	 */
	cpu = scx_pick_idle_cpu(allowed, ਗੰਢ, flags);

ਬਾਹਰਲਾ_unlock:
	rcu_read_unlock();
ਬਾਹਰਲਾ_enable:
	preempt_enable();

	ਵਾਪਸ cpu;
}

/*
 * Initialize global and per-node idle cpumasks.
 */
ਖਾਲੀ scx_idle_init_masks(ਖਾਲੀ)
{
	ਪੂਰਨ i;

	/* Allocate global idle cpumasks */
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.cpu, GFP_KERNEL));
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.smt, GFP_KERNEL));

	/* Allocate per-node idle cpumasks (use nr_node_ids for non-contiguous NUMA nodes) */
	scx_idle_ਗੰਢ_masks = kzalloc_objs(*scx_idle_ਗੰਢ_masks, nr_ਗੰਢ_ids);
	BUG_ON(!scx_idle_ਗੰਢ_masks);

	for_each_ਗੰਢ(i) {
		scx_idle_ਗੰਢ_masks[i] = kzalloc_ਗੰਢ(ਆਕਾਰ(**scx_idle_ਗੰਢ_masks),
							 GFP_KERNEL, i);
		BUG_ON(!scx_idle_ਗੰਢ_masks[i]);

		BUG_ON(!alloc_cpumask_var_ਗੰਢ(&scx_idle_ਗੰਢ_masks[i]->cpu, GFP_KERNEL, i));
		BUG_ON(!alloc_cpumask_var_ਗੰਢ(&scx_idle_ਗੰਢ_masks[i]->smt, GFP_KERNEL, i));
	}

	/* Allocate local per-cpu idle cpumasks */
	for_each_possible_cpu(i) {
		BUG_ON(!alloc_cpumask_var_ਗੰਢ(&ਪ੍ਰਤੀ_cpu(local_idle_cpumask, i),
					       GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i)));
		BUG_ON(!alloc_cpumask_var_ਗੰਢ(&ਪ੍ਰਤੀ_cpu(local_llc_idle_cpumask, i),
					       GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i)));
		BUG_ON(!alloc_cpumask_var_ਗੰਢ(&ਪ੍ਰਤੀ_cpu(local_numa_idle_cpumask, i),
					       GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i)));
	}
}

ਸਥਿਰ ਖਾਲੀ update_builtin_idle(ਪੂਰਨ cpu, bool idle)
{
	ਪੂਰਨ ਗੰਢ = scx_cpu_ਗੰਢ_if_enabled(cpu);
	ਬਣਤਰ cpumask *idle_cpus = idle_cpumask(ਗੰਢ)->cpu;

	ਸੌਂਪੋ_cpu(cpu, idle_cpus, idle);

	ਜੇ (sched_smt_ਸਰਗਰਮ()) {
		ਅਚਲ ਬਣਤਰ cpumask *smt = cpu_smt_ਨਕਾਬ(cpu);
		ਬਣਤਰ cpumask *idle_smts = idle_cpumask(ਗੰਢ)->smt;

		ਜੇ (idle) {
			/*
			 * idle_smt handling is racy but that's fine as it's
			 * only for optimization and self-correcting.
			 */
			ਜੇ (!cpumask_subset(smt, idle_cpus))
				ਵਾਪਸ;
			cpumask_or(idle_smts, idle_smts, smt);
		} ਨਹੀਂਤਾਂ {
			cpumask_andnot(idle_smts, idle_smts, smt);
		}
	}
}

/*
 * Notify schedulers of an idle transition on @cpu's cid, delivering to every
 * sched that holds %SCX_CAP_BASE on the cid (the root holds every cap). A real
 * transition (@do_notify) reaches all holders. A forced one (@root_renotify for
 * the root, a sub-sched's idle_renotify marker for a sub) reaches only the owed
 * scheds.
 */
ਸਥਿਰ ਖਾਲੀ scx_idle_notify(ਬਣਤਰ rq *rq, bool idle, bool do_notify, bool ਜੜ੍ਹ_renotify)
{
	s32 cpu = cpu_ਦਾ(rq);
	s32 cid = scx_cpu_arg(cpu);
	ਬਣਤਰ scx_sched *ਜੜ੍ਹ = scx_ਜੜ੍ਹ_protected_ਜੀਵਤ();
	ਬਣਤਰ scx_sched *pos;

	lockdep_assert_rq_held(rq);

	/* with no sub-sched, only the root can be owed a notification */
	ਜੇ (!scx_has_subs()) {
		ਜੇ ((do_notify || ਜੜ੍ਹ_renotify) &&
		    SCX_HAS_OP(ਜੜ੍ਹ, update_idle) && !scx_bypassing(ਜੜ੍ਹ, cpu))
			SCX_CALL_OP(ਜੜ੍ਹ, update_idle, rq, cid, idle);
		ਵਾਪਸ;
	}

	pos = scx_ਅਗਲਾ_descendant_pre(ਨਿਰਾ, ਜੜ੍ਹ);
	ਜਦੋਂ (pos) {
		bool forced = false;

		ਜੇ (unlikely(scx_missing_caps(pos, cpu, SCX_CAP_ਅਧਾਰ))) {
			pos = scx_skip_subtree_pre(pos, ਜੜ੍ਹ);
			ਜਾਰੀ;
		}

		ਜੇ (!pos->level) {
			forced = ਜੜ੍ਹ_renotify;
		}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
		ਨਹੀਂਤਾਂ ਜੇ (ਪ੍ਰਤੀ_cpu_ptr(pos->pcpu, cpu)->idle_renotify) {
			ਪ੍ਰਤੀ_cpu_ptr(pos->pcpu, cpu)->idle_renotify = false;
			forced = true;
		}
#ਜੇ_ਅੰਤ
		ਜੇ ((do_notify || forced) && SCX_HAS_OP(pos, update_idle) &&
		    !scx_bypassing(pos, cpu))
			SCX_CALL_OP(pos, update_idle, rq, cid, idle);
		pos = scx_ਅਗਲਾ_descendant_pre(pos, ਜੜ੍ਹ);
	}
}

/*
 * Update the idle state of a CPU to @idle.
 *
 * If @do_notify is true, ops.update_idle() is invoked to notify the scx
 * scheduler of an actual idle state transition (idle to busy or vice
 * versa). If @do_notify is false, only the idle state in the idle masks is
 * refreshed without invoking ops.update_idle().
 *
 * This distinction is necessary, because an idle CPU can be "reserved" and
 * awakened via scx_bpf_pick_idle_cpu() + scx_bpf_kick_cpu(), marking it as
 * busy even if no tasks are dispatched. In this case, the CPU may return
 * to idle without a true state transition. Refreshing the idle masks
 * without invoking ops.update_idle() ensures accurate idle state tracking
 * while avoiding unnecessary updates and maintaining balanced state
 * transitions.
 */
ਖਾਲੀ __scx_update_idle(ਬਣਤਰ rq *rq, bool idle, bool do_notify)
{
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	lockdep_assert_rq_held(rq);

	/*
	 * pick_task_idle() calls here only on an idle-to-idle re-pick and the
	 * transitions call with @do_notify, so every reaching call updates the
	 * masks.
	 */
	ਜੇ (static_branch_likely(&scx_builtin_idle_enabled))
		update_builtin_idle(cpu, idle);

	/*
	 * ops.update_idle() fires on real idle transitions, indicated by
	 * @do_notify and managed by put_prev_task_idle()/set_next_task_idle().
	 * An idle pick also fires it to flush a forced notify owed to a sched
	 * that missed transitions while bypassed or on a cid it just gained.
	 * unbypass_renotify_idle() and scx_process_sync_ecaps() arm the per-rq
	 * gates, and scx_idle_notify() targets the owed scheds.
	 *
	 * This must come after the builtin idle update so that BPF schedulers
	 * can create interlocking between ops.update_idle() and ops.enqueue() -
	 * either enqueue() sees the idle bit or update_idle() sees the task
	 * that enqueue() queued.
	 */
	ਜੇ (do_notify ||
	    (idle && (rq->scx.flags &
		      (SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_ਜੜ੍ਹ_IDLE_RENOTIFY)))) {
		bool ਜੜ੍ਹ_renotify = rq->scx.flags & SCX_RQ_ਜੜ੍ਹ_IDLE_RENOTIFY;

		rq->scx.flags &= ~(SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_ਜੜ੍ਹ_IDLE_RENOTIFY);
		scx_idle_notify(rq, idle, do_notify, ਜੜ੍ਹ_renotify);
	}
}

ਸਥਿਰ ਖਾਲੀ reset_idle_masks(ਬਣਤਰ sched_ext_ops *ops)
{
	ਪੂਰਨ ਗੰਢ;

	/*
	 * Start with all CPUs marked busy. The idle masks are populated when
	 * bypass is lifted and each idle CPU is forced through an idle re-pick.
	 * This may temporarily omit idle CPUs but never advertises a busy CPU as
	 * idle.
	 */
	ਜੇ (!(ops->flags & SCX_OPS_BUILTIN_IDLE_ਪ੍ਰਤੀ_ਗੰਢ)) {
		cpumask_clear(idle_cpumask(NUMA_NO_ਗੰਢ)->cpu);
		cpumask_clear(idle_cpumask(NUMA_NO_ਗੰਢ)->smt);
		ਵਾਪਸ;
	}

	for_each_ਗੰਢ(ਗੰਢ) {
		cpumask_clear(idle_cpumask(ਗੰਢ)->cpu);
		cpumask_clear(idle_cpumask(ਗੰਢ)->smt);
	}
}

ਖਾਲੀ scx_idle_enable(ਬਣਤਰ sched_ext_ops *ops)
{
	ਜੇ (!ops->update_idle || (ops->flags & SCX_OPS_KEEP_BUILTIN_IDLE))
		static_branch_enable_cpuslocked(&scx_builtin_idle_enabled);
	ਨਹੀਂਤਾਂ
		static_branch_disable_cpuslocked(&scx_builtin_idle_enabled);

	ਜੇ (ops->flags & SCX_OPS_BUILTIN_IDLE_ਪ੍ਰਤੀ_ਗੰਢ)
		static_branch_enable_cpuslocked(&scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ);
	ਨਹੀਂਤਾਂ
		static_branch_disable_cpuslocked(&scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ);

	reset_idle_masks(ops);
}

ਖਾਲੀ scx_idle_disable(ਖਾਲੀ)
{
	static_branch_disable(&scx_builtin_idle_enabled);
	static_branch_disable(&scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ);
}

/********************************************************************************
 * Helpers that can be called from the BPF scheduler.
 */

ਸਥਿਰ ਪੂਰਨ validate_ਗੰਢ(ਬਣਤਰ scx_sched *sch, ਪੂਰਨ ਗੰਢ)
{
	ਜੇ (!static_branch_likely(&scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ)) {
		scx_error(sch, "per-node idle tracking is disabled");
		ਵਾਪਸ -EOPNOTSUPP;
	}

	/* Return no entry for NUMA_NO_NODE (not a critical scx error) */
	ਜੇ (ਗੰਢ == NUMA_NO_ਗੰਢ)
		ਵਾਪਸ -ENOENT;

	/* Make sure node is in a valid range */
	ਜੇ (ਗੰਢ < 0 || ਗੰਢ >= nr_ਗੰਢ_ids) {
		scx_error(sch, "invalid node %d", ਗੰਢ);
		ਵਾਪਸ -EINVAL;
	}

	/* Make sure the node is part of the set of possible nodes */
	ਜੇ (!ਗੰਢ_possible(ਗੰਢ)) {
		scx_error(sch, "unavailable node %d", ਗੰਢ);
		ਵਾਪਸ -EINVAL;
	}

	ਵਾਪਸ ਗੰਢ;
}

__bpf_kfunc_start_defs();

ਸਥਿਰ bool check_builtin_idle_enabled(ਬਣਤਰ scx_sched *sch)
{
	ਜੇ (static_branch_likely(&scx_builtin_idle_enabled))
		ਵਾਪਸ true;

	scx_error(sch, "built-in idle tracking is disabled");
	ਵਾਪਸ false;
}

/*
 * Determine whether @p is a migration-disabled task in the context of BPF
 * code.
 *
 * We can't simply check whether @p->migration_disabled is set in a
 * sched_ext callback, because the BPF prolog (__bpf_prog_enter) may disable
 * migration for the current task while running BPF code.
 *
 * Since the BPF prolog calls migrate_disable() only when CONFIG_PREEMPT_RCU
 * is enabled (via rcu_read_lock_dont_migrate()), migration_disabled == 1 for
 * the current task is ambiguous only in that case: it could be from the BPF
 * prolog rather than a real migrate_disable() call.
 *
 * Without CONFIG_PREEMPT_RCU, the BPF prolog never calls migrate_disable(),
 * so migration_disabled == 1 always means the task is truly
 * migration-disabled.
 *
 * Therefore, when migration_disabled == 1 and CONFIG_PREEMPT_RCU is enabled,
 * check whether @p is the current task or not: if it is, then migration was
 * not disabled before entering the callback, otherwise migration was disabled.
 *
 * Returns true if @p is migration-disabled, false otherwise.
 */
ਸਥਿਰ bool is_bpf_migration_disabled(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਜੇ (p->migration_disabled == 1) {
		ਜੇ (IS_ENABLED(CONFIG_PREEMPT_RCU))
			ਵਾਪਸ p != current;
		ਵਾਪਸ true;
	}
	ਵਾਪਸ p->migration_disabled;
}

ਸਥਿਰ s32 select_cpu_restricted(ਬਣਤਰ task_struct *p, s32 prev_cpu,
				 ਅਚਲ ਬਣਤਰ cpumask *allowed, u64 flags,
				 bool migration_disabled)
{
	s32 cpu;

	ਜੇ (migration_disabled)
		cpu = task_cpu(p);
	ਨਹੀਂਤਾਂ
		cpu = cpumask_ਪਹਿਲਾ(p->cpus_ptr);

	ਜੇ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ਵਾਪਸ -EBUSY;

	ਜੇ (allowed && !cpumask_test_cpu(cpu, allowed))
		ਵਾਪਸ -EBUSY;

	ਜੇ (flags & SCX_PICK_IDLE_ਵਿੱਚ_ਗੰਢ) {
		ਪੂਰਨ ਗੰਢ = scx_cpu_ਗੰਢ_if_enabled(prev_cpu);

		ਜੇ (scx_cpu_ਗੰਢ_if_enabled(cpu) != ਗੰਢ)
			ਵਾਪਸ -EBUSY;
	}

	ਜੇ (!scx_idle_test_and_clear_cpu(cpu))
		ਵਾਪਸ -EBUSY;

	ਵਾਪਸ cpu;
}

ਸਥਿਰ s32 select_cpu_from_kfunc(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ task_struct *p,
				 s32 prev_cpu, u64 wake_flags,
				 ਅਚਲ ਬਣਤਰ cpumask *allowed, u64 flags)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ irq_flags;
	bool we_locked = false;
	bool migration_disabled;
	s32 cpu;

	ਜੇ (!scx_cpu_valid(sch, prev_cpu, ਨਿਰਾ))
		ਵਾਪਸ -EINVAL;

	ਜੇ (!check_builtin_idle_enabled(sch))
		ਵਾਪਸ -EBUSY;

	/*
	 * Accessing p->cpus_ptr / p->nr_cpus_allowed needs either @p's rq
	 * lock or @p's pi_lock. Three cases:
	 *
	 *  - inside ops.select_cpu(): try_to_wake_up() holds the wake-up
	 *    task's pi_lock; the wake-up task is recorded in kf_tasks[0]
	 *    by SCX_CALL_OP_TASK_RET().
	 *  - other rq-locked SCX op: scx_locked_rq() points at the held rq.
	 *  - truly unlocked (UNLOCKED ops, SYSCALL, non-SCX struct_ops):
	 *    nothing held, take pi_lock ourselves.
	 *
	 * In the first two cases, BPF schedulers may pass an arbitrary task
	 * that the held lock doesn't cover. Refuse those.
	 */
	ਜੇ (this_rq()->scx.ਵਿੱਚ_select_cpu) {
		ਜੇ (!scx_kf_arg_task_ok(sch, p))
			ਵਾਪਸ -EINVAL;
		lockdep_assert_held(&p->pi_ਤਾਲਾ);
	} ਨਹੀਂਤਾਂ ਜੇ (scx_locked_rq()) {
		ਜੇ (task_rq(p) != scx_locked_rq())
			ਜਾਓ cross_task;
	} ਨਹੀਂਤਾਂ {
		raw_spin_ਤਾਲਾ_irqsave(&p->pi_ਤਾਲਾ, irq_flags);
		we_locked = true;
	}

	/*
	 * This may also be called from ops.enqueue(), so we need to handle
	 * per-CPU tasks as well. For these tasks, skip general idle CPU
	 * selection and validate the task's only usable CPU directly.
	 */
	migration_disabled = is_bpf_migration_disabled(p);

	ਜੇ (p->nr_cpus_allowed == 1 || migration_disabled)
		cpu = select_cpu_restricted(p, prev_cpu, allowed, flags,
					    migration_disabled);
	ਨਹੀਂਤਾਂ
		cpu = scx_select_cpu_dfl(p, prev_cpu, wake_flags,
					 allowed ?: p->cpus_ptr, flags);

	ਜੇ (we_locked)
		raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, irq_flags);

	ਵਾਪਸ cpu;

cross_task:
	scx_error(sch, "select_cpu kfunc called cross-task on %s[%d]",
		  p->comm, p->pid);
	ਵਾਪਸ -EINVAL;
}

/**
 * scx_bpf_cpu_node - Return the NUMA node the given @cpu belongs to, or
 *		      trigger an error if @cpu is invalid
 * @cpu: target CPU
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 */
__bpf_kfunc s32 scx_bpf_cpu_ਗੰਢ(s32 cpu, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch) || !scx_cpu_valid(sch, cpu, ਨਿਰਾ))
		ਵਾਪਸ NUMA_NO_ਗੰਢ;
	ਵਾਪਸ cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);
}

/**
 * scx_bpf_select_cpu_dfl - The default implementation of ops.select_cpu()
 * @p: task_struct to select a CPU for
 * @prev_cpu: CPU @p was on previously
 * @wake_flags: %SCX_WAKE_* flags
 * @is_idle: out parameter indicating whether the returned CPU is idle
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Can be called from ops.select_cpu(), ops.enqueue(), or from an unlocked
 * context such as a BPF test_run() call, as long as built-in CPU selection
 * is enabled: ops.update_idle() is missing or %SCX_OPS_KEEP_BUILTIN_IDLE
 * is set.
 *
 * Returns the picked CPU with *@is_idle indicating whether the picked CPU is
 * currently idle and thus a good candidate for direct dispatching.
 */
__bpf_kfunc s32 scx_bpf_select_cpu_dfl(ਬਣਤਰ task_struct *p, s32 prev_cpu,
				       u64 wake_flags, bool *is_idle,
				       ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

	cpu = select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags, ਨਿਰਾ, 0);
	ਜੇ (cpu >= 0) {
		*is_idle = true;
		ਵਾਪਸ cpu;
	}
	*is_idle = false;
	ਵਾਪਸ prev_cpu;
}

ਬਣਤਰ scx_bpf_select_cpu_and_ਦਲੀਲਾਂ {
	/* @p and @cpus_allowed can't be packed together as KF_RCU is not transitive */
	s32			prev_cpu;
	u64			wake_flags;
	u64			flags;
};

/**
 * __scx_bpf_select_cpu_and - Arg-wrapped CPU selection with cpumask
 * @p: task_struct to select a CPU for
 * @cpus_allowed: cpumask of allowed CPUs
 * @args: struct containing the rest of the arguments
 *       @args->prev_cpu: CPU @p was on previously
 *       @args->wake_flags: %SCX_WAKE_* flags
 *       @args->flags: %SCX_PICK_IDLE* flags
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Wrapper kfunc that takes arguments via struct to work around BPF's 5 argument
 * limit. BPF programs should use scx_bpf_select_cpu_and() which is provided
 * as an inline wrapper in common.bpf.h.
 *
 * Can be called from ops.select_cpu(), ops.enqueue(), or from an unlocked
 * context such as a BPF test_run() call, as long as built-in CPU selection
 * is enabled: ops.update_idle() is missing or %SCX_OPS_KEEP_BUILTIN_IDLE
 * is set.
 *
 * @p, @args->prev_cpu and @args->wake_flags match ops.select_cpu().
 *
 * Returns the selected idle CPU, which will be automatically awakened upon
 * returning from ops.select_cpu() and can be used for direct dispatch, or
 * a negative value if no idle CPU is available.
 */
__bpf_kfunc s32
__scx_bpf_select_cpu_and(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed,
			 ਬਣਤਰ scx_bpf_select_cpu_and_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ,
			 ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

	ਵਾਪਸ select_cpu_from_kfunc(sch, p, ਦਲੀਲਾਂ->prev_cpu, ਦਲੀਲਾਂ->wake_flags,
				     cpus_allowed, ਦਲੀਲਾਂ->flags);
}

/*
 * COMPAT: Will be removed in v6.22.
 */
__bpf_kfunc s32 scx_bpf_select_cpu_and(ਬਣਤਰ task_struct *p, s32 prev_cpu, u64 wake_flags,
				       ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed, u64 flags)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ਜੜ੍ਹ);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	/*
	 * Disallow if any sub-scheds are attached. There is no way to tell
	 * which scheduler called us, so error out @p's scheduler -- read it
	 * under RCU as @p's locks aren't necessarily held here. @p may be a
	 * task past sched_ext_dead() or an idle task, in which case its
	 * scheduler can't be determined and there is nothing obviously wrong
	 * to report; just refuse the call.
	 */
	ਜੇ (unlikely(!list_ਖਾਲੀ_ਥਾਵਾਂ(&sch->children))) {
		ਬਣਤਰ scx_sched *tsch = scx_task_sched_rcu(p);

		ਜੇ (tsch)
			scx_error(tsch, "__scx_bpf_select_cpu_and() must be used");
		ਵਾਪਸ -EINVAL;
	}
#ਜੇ_ਅੰਤ

	ਵਾਪਸ select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags,
				     cpus_allowed, flags);
}

/**
 * scx_bpf_get_idle_cpumask_node - Get a referenced kptr to the
 * idle-tracking per-CPU cpumask of a target NUMA node.
 * @node: target NUMA node
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns an empty cpumask if idle tracking is not enabled, if @node is
 * not valid, or running on a UP kernel. In this case the actual error will
 * be reported to the BPF scheduler via scx_error().
 */
__bpf_kfunc ਅਚਲ ਬਣਤਰ cpumask *
scx_bpf_get_idle_cpumask_ਗੰਢ(s32 ਗੰਢ, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਗੰਢ = validate_ਗੰਢ(sch, ਗੰਢ);
	ਜੇ (ਗੰਢ < 0)
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਵਾਪਸ idle_cpumask(ਗੰਢ)->cpu;
}

/**
 * scx_bpf_get_idle_cpumask - Get a referenced kptr to the idle-tracking
 * per-CPU cpumask.
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns an empty mask if idle tracking is not enabled, or running on a
 * UP kernel.
 */
__bpf_kfunc ਅਚਲ ਬਣਤਰ cpumask *scx_bpf_get_idle_cpumask(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਜੇ (static_branch_unlikely(&scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		ਵਾਪਸ cpu_none_ਨਕਾਬ;
	}

	ਜੇ (!check_builtin_idle_enabled(sch))
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਵਾਪਸ idle_cpumask(NUMA_NO_ਗੰਢ)->cpu;
}

/**
 * scx_bpf_get_idle_smtmask_node - Get a referenced kptr to the
 * idle-tracking, per-physical-core cpumask of a target NUMA node. Can be
 * used to determine if an entire physical core is free.
 * @node: target NUMA node
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns an empty cpumask if idle tracking is not enabled, if @node is
 * not valid, or running on a UP kernel. In this case the actual error will
 * be reported to the BPF scheduler via scx_error().
 */
__bpf_kfunc ਅਚਲ ਬਣਤਰ cpumask *
scx_bpf_get_idle_smtmask_ਗੰਢ(s32 ਗੰਢ, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਗੰਢ = validate_ਗੰਢ(sch, ਗੰਢ);
	ਜੇ (ਗੰਢ < 0)
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਜੇ (sched_smt_ਸਰਗਰਮ())
		ਵਾਪਸ idle_cpumask(ਗੰਢ)->smt;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ idle_cpumask(ਗੰਢ)->cpu;
}

/**
 * scx_bpf_get_idle_smtmask - Get a referenced kptr to the idle-tracking,
 * per-physical-core cpumask. Can be used to determine if an entire physical
 * core is free.
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns an empty mask if idle tracking is not enabled, or running on a
 * UP kernel.
 */
__bpf_kfunc ਅਚਲ ਬਣਤਰ cpumask *scx_bpf_get_idle_smtmask(ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਜੇ (static_branch_unlikely(&scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		ਵਾਪਸ cpu_none_ਨਕਾਬ;
	}

	ਜੇ (!check_builtin_idle_enabled(sch))
		ਵਾਪਸ cpu_none_ਨਕਾਬ;

	ਜੇ (sched_smt_ਸਰਗਰਮ())
		ਵਾਪਸ idle_cpumask(NUMA_NO_ਗੰਢ)->smt;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ idle_cpumask(NUMA_NO_ਗੰਢ)->cpu;
}

/**
 * scx_bpf_put_idle_cpumask - Release a previously acquired referenced kptr to
 * either the percpu, or SMT idle-tracking cpumask.
 * @idle_mask: &cpumask to use
 */
__bpf_kfunc ਖਾਲੀ scx_bpf_put_idle_cpumask(ਅਚਲ ਬਣਤਰ cpumask *idle_ਨਕਾਬ)
{
	/*
	 * Empty function body because we aren't actually acquiring or releasing
	 * a reference to a global idle cpumask, which is read-only in the
	 * caller and is never released. The acquire / release semantics here
	 * are just used to make the cpumask a trusted pointer in the caller.
	 */
}

/**
 * scx_bpf_test_and_clear_cpu_idle - Test and clear @cpu's idle state
 * @cpu: cpu to test and clear idle for
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns %true if @cpu was idle and its idle state was successfully cleared.
 * %false otherwise.
 *
 * Unavailable if ops.update_idle() is implemented and
 * %SCX_OPS_KEEP_BUILTIN_IDLE is not set.
 */
__bpf_kfunc bool scx_bpf_test_and_clear_cpu_idle(s32 cpu, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ false;

	ਜੇ (!check_builtin_idle_enabled(sch))
		ਵਾਪਸ false;

	ਜੇ (!scx_cpu_valid(sch, cpu, ਨਿਰਾ))
		ਵਾਪਸ false;

	ਵਾਪਸ scx_idle_test_and_clear_cpu(cpu);
}

/**
 * scx_bpf_pick_idle_cpu_node - Pick and claim an idle cpu from @node
 * @cpus_allowed: Allowed cpumask
 * @node: target NUMA node
 * @flags: %SCX_PICK_IDLE_* flags
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Pick and claim an idle cpu in @cpus_allowed from the NUMA node @node.
 *
 * Returns the picked idle cpu number on success, or -%EBUSY if no matching
 * cpu was found.
 *
 * The search starts from @node and proceeds to other online NUMA nodes in
 * order of increasing distance (unless SCX_PICK_IDLE_IN_NODE is specified,
 * in which case the search is limited to the target @node).
 *
 * Always returns an error if ops.update_idle() is implemented and
 * %SCX_OPS_KEEP_BUILTIN_IDLE is not set, or if
 * %SCX_OPS_BUILTIN_IDLE_PER_NODE is not set.
 */
__bpf_kfunc s32 scx_bpf_pick_idle_cpu_ਗੰਢ(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed,
					   s32 ਗੰਢ, u64 flags,
					   ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

	ਗੰਢ = validate_ਗੰਢ(sch, ਗੰਢ);
	ਜੇ (ਗੰਢ < 0)
		ਵਾਪਸ ਗੰਢ;

	ਵਾਪਸ scx_pick_idle_cpu(cpus_allowed, ਗੰਢ, flags);
}

/**
 * scx_bpf_pick_idle_cpu - Pick and claim an idle cpu
 * @cpus_allowed: Allowed cpumask
 * @flags: %SCX_PICK_IDLE_* flags
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Pick and claim an idle cpu in @cpus_allowed. Returns the picked idle cpu
 * number on success. -%EBUSY if no matching cpu was found.
 *
 * Idle CPU tracking may race against CPU scheduling state transitions. For
 * example, this function may return -%EBUSY as CPUs are transitioning into the
 * idle state. If the caller then assumes that there will be dispatch events on
 * the CPUs as they were all busy, the scheduler may end up stalling with CPUs
 * idling while there are pending tasks. Use scx_bpf_pick_any_cpu() and
 * scx_bpf_kick_cpu() to guarantee that there will be at least one dispatch
 * event in the near future.
 *
 * Unavailable if ops.update_idle() is implemented and
 * %SCX_OPS_KEEP_BUILTIN_IDLE is not set.
 *
 * Always returns an error if %SCX_OPS_BUILTIN_IDLE_PER_NODE is set, use
 * scx_bpf_pick_idle_cpu_node() instead.
 */
__bpf_kfunc s32 scx_bpf_pick_idle_cpu(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed,
				      u64 flags, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

	ਜੇ (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ)) {
		scx_error(sch, "per-node idle tracking is enabled");
		ਵਾਪਸ -EBUSY;
	}

	ਜੇ (!check_builtin_idle_enabled(sch))
		ਵਾਪਸ -EBUSY;

	ਵਾਪਸ scx_pick_idle_cpu(cpus_allowed, NUMA_NO_ਗੰਢ, flags);
}

/**
 * scx_bpf_pick_any_cpu_node - Pick and claim an idle cpu if available
 *			       or pick any CPU from @node
 * @cpus_allowed: Allowed cpumask
 * @node: target NUMA node
 * @flags: %SCX_PICK_IDLE_* flags
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Pick and claim an idle cpu in @cpus_allowed. If none is available, pick any
 * CPU in @cpus_allowed. Guaranteed to succeed and returns the picked idle cpu
 * number if @cpus_allowed is not empty. -%EBUSY is returned if @cpus_allowed is
 * empty.
 *
 * The search starts from @node and proceeds to other online NUMA nodes in
 * order of increasing distance (unless %SCX_PICK_IDLE_IN_NODE is specified,
 * in which case the search is limited to the target @node, regardless of
 * the CPU idle state).
 *
 * If ops.update_idle() is implemented and %SCX_OPS_KEEP_BUILTIN_IDLE is not
 * set, this function can't tell which CPUs are idle and will always pick any
 * CPU.
 */
__bpf_kfunc s32 scx_bpf_pick_any_cpu_ਗੰਢ(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed,
					  s32 ਗੰਢ, u64 flags,
					  ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

	ਗੰਢ = validate_ਗੰਢ(sch, ਗੰਢ);
	ਜੇ (ਗੰਢ < 0)
		ਵਾਪਸ ਗੰਢ;

	cpu = scx_pick_idle_cpu(cpus_allowed, ਗੰਢ, flags);
	ਜੇ (cpu >= 0)
		ਵਾਪਸ cpu;

	ਜੇ (flags & SCX_PICK_IDLE_ਵਿੱਚ_ਗੰਢ)
		cpu = cpumask_any_and_distribute(cpumask_ਦਾ_ਗੰਢ(ਗੰਢ), cpus_allowed);
	ਨਹੀਂਤਾਂ
		cpu = cpumask_any_distribute(cpus_allowed);
	ਜੇ (cpu < nr_cpu_ids)
		ਵਾਪਸ cpu;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ -EBUSY;
}

/**
 * scx_bpf_pick_any_cpu - Pick and claim an idle cpu if available or pick any CPU
 * @cpus_allowed: Allowed cpumask
 * @flags: %SCX_PICK_IDLE_* flags
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Pick and claim an idle cpu in @cpus_allowed. If none is available, pick any
 * CPU in @cpus_allowed. Guaranteed to succeed and returns the picked idle cpu
 * number if @cpus_allowed is not empty. -%EBUSY is returned if @cpus_allowed is
 * empty.
 *
 * If ops.update_idle() is implemented and %SCX_OPS_KEEP_BUILTIN_IDLE is not
 * set, this function can't tell which CPUs are idle and will always pick any
 * CPU.
 *
 * Always returns an error if %SCX_OPS_BUILTIN_IDLE_PER_NODE is set, use
 * scx_bpf_pick_any_cpu_node() instead.
 */
__bpf_kfunc s32 scx_bpf_pick_any_cpu(ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed,
				     u64 flags, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -ENODEV;

	ਜੇ (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_ਪ੍ਰਤੀ_ਗੰਢ)) {
		scx_error(sch, "per-node idle tracking is enabled");
		ਵਾਪਸ -EBUSY;
	}

	ਜੇ (static_branch_likely(&scx_builtin_idle_enabled)) {
		cpu = scx_pick_idle_cpu(cpus_allowed, NUMA_NO_ਗੰਢ, flags);
		ਜੇ (cpu >= 0)
			ਵਾਪਸ cpu;
	}

	cpu = cpumask_any_distribute(cpus_allowed);
	ਜੇ (cpu < nr_cpu_ids)
		ਵਾਪਸ cpu;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ -EBUSY;
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_idle)
BTF_ID_FLAGS(func, scx_bpf_cpu_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_idle_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_test_and_clear_cpu_idle, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu_ਗੰਢ, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_idle)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_idle = {
	.owner			= THIS_MODULE,
	.ਸੈੱਟ			= &scx_kfunc_ids_idle,
	.filter			= scx_kfunc_context_filter,
};

/*
 * The select_cpu kfuncs internally call task_rq_lock() when invoked from an
 * rq-unlocked context, and thus cannot be safely called from arbitrary tracing
 * contexts where @p's pi_lock state is unknown. Keep them out of
 * BPF_PROG_TYPE_TRACING by registering them in their own set which is exposed
 * only to STRUCT_OPS and SYSCALL programs.
 *
 * These kfuncs are also members of scx_kfunc_ids_unlocked (see ext.c) because
 * they're callable from unlocked contexts in addition to ops.select_cpu() and
 * ops.enqueue().
 */
BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_select_cpu)
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_RCU)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_select_cpu)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_select_cpu = {
	.owner			= THIS_MODULE,
	.ਸੈੱਟ			= &scx_kfunc_ids_select_cpu,
	.filter			= scx_kfunc_context_filter,
};

ਪੂਰਨ scx_idle_init(ਖਾਲੀ)
{
	ਵਾਪਸ ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_ਸੈੱਟ_idle) ?:
	       ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_TRACING, &scx_kfunc_ਸੈੱਟ_idle) ?:
	       ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_ਸੈੱਟ_idle) ?:
	       ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_ਸੈੱਟ_select_cpu) ?:
	       ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_ਸੈੱਟ_select_cpu);
}
