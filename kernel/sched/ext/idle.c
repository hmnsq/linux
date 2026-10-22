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
#ضمن "internal.h"
#ضمن "cid.h"
#ضمن "idle.h"
#ضمن "sub.h"

/* Enable/disable built-in idle CPU selection policy */
ساكن DEFINE_STATIC_کلید_FALSE(scx_builtin_idle_enabled);

/* Enable/disable per-node idle cpumasks */
ساكن DEFINE_STATIC_کلید_FALSE(scx_builtin_idle_per_گرہ);

/* Enable/disable LLC aware optimizations */
ساكن DEFINE_STATIC_کلید_FALSE(scx_selcpu_topo_llc);

/* Enable/disable NUMA aware optimizations */
ساكن DEFINE_STATIC_کلید_FALSE(scx_selcpu_topo_numa);

/*
 * cpumasks to track idle CPUs within each NUMA node.
 *
 * If SCX_OPS_BUILTIN_IDLE_PER_NODE is not enabled, a single global cpumask
 * from is used to track all the idle CPUs in the system.
 */
بنية scx_idle_cpus {
	cpumask_var_t cpu;
	cpumask_var_t smt;
};

/*
 * Global host-wide idle cpumasks (used when SCX_OPS_BUILTIN_IDLE_PER_NODE
 * is not enabled).
 */
ساكن بنية scx_idle_cpus scx_idle_global_masks;

/*
 * Per-node idle cpumasks.
 */
ساكن بنية scx_idle_cpus **scx_idle_گرہ_masks;

/*
 * Local per-CPU cpumasks (used to generate temporary idle cpumasks).
 */
ساكن DEFINE_PER_CPU(cpumask_var_t, local_idle_cpumask);
ساكن DEFINE_PER_CPU(cpumask_var_t, local_llc_idle_cpumask);
ساكن DEFINE_PER_CPU(cpumask_var_t, local_numa_idle_cpumask);

/*
 * Return the idle masks associated to a target @node.
 *
 * NUMA_NO_NODE identifies the global idle cpumask.
 */
ساكن بنية scx_idle_cpus *idle_cpumask(صحيح گرہ)
{
	أرجع گرہ == NUMA_NO_گرہ ? &scx_idle_global_masks : scx_idle_گرہ_masks[گرہ];
}

/*
 * Returns the NUMA node ID associated with a @cpu, or NUMA_NO_NODE if
 * per-node idle cpumasks are disabled.
 */
ساكن صحيح scx_cpu_گرہ_if_enabled(صحيح cpu)
{
	إذا (!static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_per_گرہ))
		أرجع NUMA_NO_گرہ;

	أرجع cpu_to_گرہ(cpu);
}

ساكن bool scx_idle_test_and_clear_cpu(صحيح cpu)
{
	صحيح گرہ = scx_cpu_گرہ_if_enabled(cpu);
	بنية cpumask *idle_cpus = idle_cpumask(گرہ)->cpu;

	/*
	 * SMT mask should be cleared whether we can claim @cpu or not. The SMT
	 * cluster is not wholly idle either way. This also prevents
	 * scx_pick_idle_cpu() from getting caught in an infinite loop.
	 */
	إذا (sched_smt_active()) {
		ثابت بنية cpumask *smt = cpu_smt_mask(cpu);
		بنية cpumask *idle_smts = idle_cpumask(گرہ)->smt;

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
		إذا (cpumask_intersects(smt, idle_smts))
			cpumask_andnot(idle_smts, idle_smts, smt);
		وإلا إذا (cpumask_test_cpu(cpu, idle_smts))
			__cpumask_clear_cpu(cpu, idle_smts);
	}

	أرجع cpumask_test_and_clear_cpu(cpu, idle_cpus);
}

/*
 * Pick an idle CPU in a specific NUMA node.
 */
ساكن s32 pick_idle_cpu_in_گرہ(ثابت بنية cpumask *cpus_allowed, صحيح گرہ, u64 flags)
{
	صحيح cpu;

retry:
	إذا (sched_smt_active()) {
		cpu = cpumask_any_and_distribute(idle_cpumask(گرہ)->smt, cpus_allowed);
		إذا (cpu < nr_cpu_ids)
			اذهب found;

		إذا (flags & SCX_PICK_IDLE_CORE)
			أرجع -EBUSY;
	}

	cpu = cpumask_any_and_distribute(idle_cpumask(گرہ)->cpu, cpus_allowed);
	إذا (cpu >= nr_cpu_ids)
		أرجع -EBUSY;

found:
	إذا (scx_idle_test_and_clear_cpu(cpu))
		أرجع cpu;
	وإلا
		اذهب retry;
}

#ifdef CONFIG_NUMA
/*
 * Tracks nodes that have not yet been visited when searching for an idle
 * CPU across all available nodes.
 */
ساكن DEFINE_PER_CPU(nodemask_t, per_cpu_unvisited);

/*
 * Search for an idle CPU across all nodes, excluding @node.
 */
ساكن s32 pick_idle_cpu_from_online_nodes(ثابت بنية cpumask *cpus_allowed, صحيح گرہ, u64 flags)
{
	nodemask_t *unvisited;
	s32 cpu = -EBUSY;

	preempt_disable();
	unvisited = this_cpu_ptr(&per_cpu_unvisited);

	/*
	 * Restrict the search to the online nodes (excluding the current
	 * node that has been visited already).
	 */
	nodes_copy(*unvisited, گرہ_states[N_ONLINE]);
	گرہ_clear(گرہ, *unvisited);

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
	for_each_گرہ_numadist(گرہ, *unvisited) {
		cpu = pick_idle_cpu_in_گرہ(cpus_allowed, گرہ, flags);
		إذا (cpu >= 0)
			اكسر;
	}
	preempt_enable();

	أرجع cpu;
}
#وإلا
ساكن مضمن s32
pick_idle_cpu_from_online_nodes(ثابت بنية cpumask *cpus_allowed, صحيح گرہ, u64 flags)
{
	أرجع -EBUSY;
}
#endif

/*
 * Find an idle CPU in the system, starting from @node.
 */
ساكن s32 scx_pick_idle_cpu(ثابت بنية cpumask *cpus_allowed, صحيح گرہ, u64 flags)
{
	s32 cpu;

	/*
	 * Always search in the starting node first (this is an
	 * optimization that can save some cycles even when the search is
	 * not limited to a single node).
	 */
	cpu = pick_idle_cpu_in_گرہ(cpus_allowed, گرہ, flags);
	إذا (cpu >= 0)
		أرجع cpu;

	/*
	 * Stop the search if we are using only a single global cpumask
	 * (NUMA_NO_NODE) or if the search is restricted to the first node
	 * only.
	 */
	إذا (گرہ == NUMA_NO_گرہ || flags & SCX_PICK_IDLE_IN_گرہ)
		أرجع -EBUSY;

	/*
	 * Extend the search to the other online nodes.
	 */
	أرجع pick_idle_cpu_from_online_nodes(cpus_allowed, گرہ, flags);
}

/*
 * Return the amount of CPUs in the same LLC domain of @cpu (or zero if the LLC
 * domain is not defined).
 */
ساكن unsigned صحيح llc_weight(s32 cpu)
{
	بنية sched_domain *sd;

	sd = rcu_dereference(per_cpu(sd_llc, cpu));
	إذا (!sd)
		أرجع 0;

	أرجع sd->span_weight;
}

/*
 * Return the cpumask representing the LLC domain of @cpu (or NULL if the LLC
 * domain is not defined).
 */
ساكن بنية cpumask *llc_span(s32 cpu)
{
	بنية sched_domain *sd;

	sd = rcu_dereference(per_cpu(sd_llc, cpu));
	إذا (!sd)
		أرجع عدم;

	أرجع sched_domain_span(sd);
}

/*
 * Return the amount of CPUs in the same NUMA domain of @cpu (or zero if the
 * NUMA domain is not defined).
 */
ساكن unsigned صحيح numa_weight(s32 cpu)
{
	بنية sched_domain *sd;
	بنية sched_group *sg;

	sd = rcu_dereference(per_cpu(sd_numa, cpu));
	إذا (!sd)
		أرجع 0;
	sg = sd->groups;
	إذا (!sg)
		أرجع 0;

	أرجع sg->group_weight;
}

/*
 * Return the cpumask representing the NUMA domain of @cpu (or NULL if the NUMA
 * domain is not defined).
 */
ساكن بنية cpumask *numa_span(s32 cpu)
{
	بنية sched_domain *sd;
	بنية sched_group *sg;

	sd = rcu_dereference(per_cpu(sd_numa, cpu));
	إذا (!sd)
		أرجع عدم;
	sg = sd->groups;
	إذا (!sg)
		أرجع عدم;

	أرجع sched_group_span(sg);
}

/*
 * Return true if the LLC domains do not perfectly overlap with the NUMA
 * domains, false otherwise.
 */
ساكن bool llc_numa_mismatch(فراغ)
{
	صحيح cpu;

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
		إذا (llc_weight(cpu) != numa_weight(cpu))
			أرجع true;

	أرجع false;
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
فراغ scx_idle_update_selcpu_topology(بنية sched_ext_ops *ops)
{
	bool enable_llc = false, enable_numa = false;
	unsigned صحيح nr_cpus;
	s32 cpu = cpumask_first(cpu_online_mask);

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
	rcu_read_lock();
	nr_cpus = llc_weight(cpu);
	إذا (nr_cpus > 0) {
		إذا (nr_cpus < عدد_online_cpus())
			enable_llc = true;
		pr_debug("sched_ext: LLC=%*pb weight=%u\n",
			 cpumask_pr_args(llc_span(cpu)), llc_weight(cpu));
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
	إذا (!(ops->flags & SCX_OPS_BUILTIN_IDLE_PER_گرہ)) {
		nr_cpus = numa_weight(cpu);
		إذا (nr_cpus > 0) {
			إذا (nr_cpus < عدد_online_cpus() && llc_numa_mismatch())
				enable_numa = true;
			pr_debug("sched_ext: NUMA=%*pb weight=%u\n",
				 cpumask_pr_args(numa_span(cpu)), nr_cpus);
		}
	}
	rcu_read_unlock();

	pr_debug("sched_ext: LLC idle selection %s\n",
		 str_enabled_disabled(enable_llc));
	pr_debug("sched_ext: NUMA idle selection %s\n",
		 str_enabled_disabled(enable_numa));

	إذا (enable_llc)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_llc);
	وإلا
		static_branch_disable_cpuslocked(&scx_selcpu_topo_llc);
	إذا (enable_numa)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_numa);
	وإلا
		static_branch_disable_cpuslocked(&scx_selcpu_topo_numa);
}

/*
 * Return true if @p can run on all possible CPUs, false otherwise.
 */
ساكن مضمن bool task_affinity_all(ثابت بنية task_struct *p)
{
	أرجع p->nr_cpus_allowed >= عدد_possible_cpus();
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
s32 scx_select_cpu_dfl(بنية task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ثابت بنية cpumask *cpus_allowed, u64 flags)
{
	ثابت بنية cpumask *llc_cpus = عدم, *numa_cpus = عدم;
	ثابت بنية cpumask *allowed = cpus_allowed ?: p->cpus_ptr;
	صحيح گرہ = scx_cpu_گرہ_if_enabled(prev_cpu);
	bool is_prev_allowed;
	s32 cpu;

	preempt_disable();

	/*
	 * Determine the subset of CPUs usable by @p within @cpus_allowed.
	 */
	إذا (allowed != p->cpus_ptr) {
		بنية cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_idle_cpumask);

		إذا (task_affinity_all(p)) {
			allowed = cpus_allowed;
		} وإلا إذا (cpumask_and(local_cpus, cpus_allowed, p->cpus_ptr)) {
			allowed = local_cpus;
		} وإلا {
			cpu = -EBUSY;
			اذهب out_enable;
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
	rcu_read_lock();

	/*
	 * Determine the subset of CPUs that the task can use in its
	 * current LLC and node.
	 *
	 * If the task can run on all CPUs, use the node and LLC cpumasks
	 * directly.
	 */
	إذا (static_branch_maybe(CONFIG_NUMA, &scx_selcpu_topo_numa)) {
		بنية cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_numa_idle_cpumask);
		ثابت بنية cpumask *cpus = numa_span(prev_cpu);

		إذا (allowed == p->cpus_ptr && task_affinity_all(p))
			numa_cpus = cpus;
		وإلا إذا (cpus && cpumask_and(local_cpus, allowed, cpus))
			numa_cpus = local_cpus;
	}

	إذا (static_branch_maybe(CONFIG_SCHED_MC, &scx_selcpu_topo_llc)) {
		بنية cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_llc_idle_cpumask);
		ثابت بنية cpumask *cpus = llc_span(prev_cpu);

		إذا (allowed == p->cpus_ptr && task_affinity_all(p))
			llc_cpus = cpus;
		وإلا إذا (cpus && cpumask_and(local_cpus, allowed, cpus))
			llc_cpus = local_cpus;
	}

	/*
	 * If WAKE_SYNC, try to migrate the wakee to the waker's CPU.
	 */
	إذا (wake_flags & SCX_WAKE_SYNC) {
		صحيح waker_گرہ;

		/*
		 * If the waker's CPU is cache affine and prev_cpu is idle,
		 * then avoid a migration.
		 */
		cpu = smp_processor_id();
		إذا (is_prev_allowed && cpus_share_cache(cpu, prev_cpu) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			اذهب out_unlock;
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
		waker_گرہ = scx_cpu_گرہ_if_enabled(cpu);
		إذا (!(current->flags & PF_EXITING) &&
		    cpu_rq(cpu)->scx.local_dsq.nr == 0 &&
		    (!(flags & SCX_PICK_IDLE_IN_گرہ) || (waker_گرہ == گرہ)) &&
		    !cpumask_empty(idle_cpumask(waker_گرہ)->cpu)) {
			إذا (cpumask_test_cpu(cpu, allowed)) {
				scx_idle_test_and_clear_cpu(cpu);
				اذهب out_unlock;
			}
		}
	}

	/*
	 * If CPU has SMT, any wholly idle CPU is likely a better pick than
	 * partially idle @prev_cpu.
	 */
	إذا (sched_smt_active()) {
		/*
		 * Keep using @prev_cpu if it's part of a fully idle core.
		 */
		إذا (is_prev_allowed &&
		    cpumask_test_cpu(prev_cpu, idle_cpumask(گرہ)->smt) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			اذهب out_unlock;
		}

		/*
		 * Search for any fully idle core in the same LLC domain.
		 */
		إذا (llc_cpus) {
			cpu = pick_idle_cpu_in_گرہ(llc_cpus, گرہ, SCX_PICK_IDLE_CORE);
			إذا (cpu >= 0)
				اذهب out_unlock;
		}

		/*
		 * Search for any fully idle core in the same NUMA node.
		 */
		إذا (numa_cpus) {
			cpu = pick_idle_cpu_in_گرہ(numa_cpus, گرہ, SCX_PICK_IDLE_CORE);
			إذا (cpu >= 0)
				اذهب out_unlock;
		}

		/*
		 * Search for any full-idle core usable by the task.
		 *
		 * If the node-aware idle CPU selection policy is enabled
		 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always
		 * begin in prev_cpu's node and proceed to other nodes in
		 * order of increasing distance.
		 */
		cpu = scx_pick_idle_cpu(allowed, گرہ, flags | SCX_PICK_IDLE_CORE);
		إذا (cpu >= 0)
			اذهب out_unlock;

		/*
		 * Give up if we're strictly looking for a full-idle SMT
		 * core.
		 */
		إذا (flags & SCX_PICK_IDLE_CORE) {
			cpu = -EBUSY;
			اذهب out_unlock;
		}
	}

	/*
	 * Use @prev_cpu if it's idle.
	 */
	إذا (is_prev_allowed && scx_idle_test_and_clear_cpu(prev_cpu)) {
		cpu = prev_cpu;
		اذهب out_unlock;
	}

	/*
	 * Use @prev_cpu's sibling if it's idle.
	 */
	إذا (sched_smt_active()) {
		for_each_cpu_and(cpu, cpu_smt_mask(prev_cpu), allowed) {
			إذا (cpu == prev_cpu)
				استمر;
			إذا (scx_idle_test_and_clear_cpu(cpu))
				اذهب out_unlock;
		}
	}

	/*
	 * Search for any idle CPU in the same LLC domain.
	 */
	إذا (llc_cpus) {
		cpu = pick_idle_cpu_in_گرہ(llc_cpus, گرہ, 0);
		إذا (cpu >= 0)
			اذهب out_unlock;
	}

	/*
	 * Search for any idle CPU in the same NUMA node.
	 */
	إذا (numa_cpus) {
		cpu = pick_idle_cpu_in_گرہ(numa_cpus, گرہ, 0);
		إذا (cpu >= 0)
			اذهب out_unlock;
	}

	/*
	 * Search for any idle CPU usable by the task.
	 *
	 * If the node-aware idle CPU selection policy is enabled
	 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always begin
	 * in prev_cpu's node and proceed to other nodes in order of
	 * increasing distance.
	 */
	cpu = scx_pick_idle_cpu(allowed, گرہ, flags);

out_unlock:
	rcu_read_unlock();
out_enable:
	preempt_enable();

	أرجع cpu;
}

/*
 * Initialize global and per-node idle cpumasks.
 */
فراغ scx_idle_init_masks(فراغ)
{
	صحيح i;

	/* Allocate global idle cpumasks */
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.cpu, GFP_KERNEL));
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.smt, GFP_KERNEL));

	/* Allocate per-node idle cpumasks (use nr_node_ids for non-contiguous NUMA nodes) */
	scx_idle_گرہ_masks = kzalloc_objs(*scx_idle_گرہ_masks, nr_گرہ_ids);
	BUG_ON(!scx_idle_گرہ_masks);

	for_each_گرہ(i) {
		scx_idle_گرہ_masks[i] = kzalloc_گرہ(حجم(**scx_idle_گرہ_masks),
							 GFP_KERNEL, i);
		BUG_ON(!scx_idle_گرہ_masks[i]);

		BUG_ON(!alloc_cpumask_var_گرہ(&scx_idle_گرہ_masks[i]->cpu, GFP_KERNEL, i));
		BUG_ON(!alloc_cpumask_var_گرہ(&scx_idle_گرہ_masks[i]->smt, GFP_KERNEL, i));
	}

	/* Allocate local per-cpu idle cpumasks */
	for_each_possible_cpu(i) {
		BUG_ON(!alloc_cpumask_var_گرہ(&per_cpu(local_idle_cpumask, i),
					       GFP_KERNEL, cpu_to_گرہ(i)));
		BUG_ON(!alloc_cpumask_var_گرہ(&per_cpu(local_llc_idle_cpumask, i),
					       GFP_KERNEL, cpu_to_گرہ(i)));
		BUG_ON(!alloc_cpumask_var_گرہ(&per_cpu(local_numa_idle_cpumask, i),
					       GFP_KERNEL, cpu_to_گرہ(i)));
	}
}

ساكن فراغ update_builtin_idle(صحيح cpu, bool idle)
{
	صحيح گرہ = scx_cpu_گرہ_if_enabled(cpu);
	بنية cpumask *idle_cpus = idle_cpumask(گرہ)->cpu;

	assign_cpu(cpu, idle_cpus, idle);

	إذا (sched_smt_active()) {
		ثابت بنية cpumask *smt = cpu_smt_mask(cpu);
		بنية cpumask *idle_smts = idle_cpumask(گرہ)->smt;

		إذا (idle) {
			/*
			 * idle_smt handling is racy but that's fine as it's
			 * only for optimization and self-correcting.
			 */
			إذا (!cpumask_subset(smt, idle_cpus))
				أرجع;
			cpumask_or(idle_smts, idle_smts, smt);
		} وإلا {
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
ساكن فراغ scx_idle_notify(بنية rq *rq, bool idle, bool do_notify, bool جڑ_renotify)
{
	s32 cpu = cpu_of(rq);
	s32 cid = scx_cpu_arg(cpu);
	بنية scx_sched *جڑ = scx_جڑ_protected_live();
	بنية scx_sched *pos;

	lockdep_assert_rq_held(rq);

	/* with no sub-sched, only the root can be owed a notification */
	إذا (!scx_has_subs()) {
		إذا ((do_notify || جڑ_renotify) &&
		    SCX_HAS_OP(جڑ, update_idle) && !scx_bypassing(جڑ, cpu))
			SCX_CALL_OP(جڑ, update_idle, rq, cid, idle);
		أرجع;
	}

	pos = scx_next_descendant_pre(عدم, جڑ);
	طالما (pos) {
		bool forced = false;

		إذا (unlikely(scx_missing_caps(pos, cpu, SCX_CAP_BASE))) {
			pos = scx_skip_subtree_pre(pos, جڑ);
			استمر;
		}

		إذا (!pos->level) {
			forced = جڑ_renotify;
		}
#ifdef CONFIG_EXT_SUB_SCHED
		وإلا إذا (per_cpu_ptr(pos->pcpu, cpu)->idle_renotify) {
			per_cpu_ptr(pos->pcpu, cpu)->idle_renotify = false;
			forced = true;
		}
#endif
		إذا ((do_notify || forced) && SCX_HAS_OP(pos, update_idle) &&
		    !scx_bypassing(pos, cpu))
			SCX_CALL_OP(pos, update_idle, rq, cid, idle);
		pos = scx_next_descendant_pre(pos, جڑ);
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
فراغ __scx_update_idle(بنية rq *rq, bool idle, bool do_notify)
{
	صحيح cpu = cpu_of(rq);

	lockdep_assert_rq_held(rq);

	/*
	 * pick_task_idle() calls here only on an idle-to-idle re-pick and the
	 * transitions call with @do_notify, so every reaching call updates the
	 * masks.
	 */
	إذا (static_branch_likely(&scx_builtin_idle_enabled))
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
	إذا (do_notify ||
	    (idle && (rq->scx.flags &
		      (SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_جڑ_IDLE_RENOTIFY)))) {
		bool جڑ_renotify = rq->scx.flags & SCX_RQ_جڑ_IDLE_RENOTIFY;

		rq->scx.flags &= ~(SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_جڑ_IDLE_RENOTIFY);
		scx_idle_notify(rq, idle, do_notify, جڑ_renotify);
	}
}

ساكن فراغ reset_idle_masks(بنية sched_ext_ops *ops)
{
	صحيح گرہ;

	/*
	 * Start with all CPUs marked busy. The idle masks are populated when
	 * bypass is lifted and each idle CPU is forced through an idle re-pick.
	 * This may temporarily omit idle CPUs but never advertises a busy CPU as
	 * idle.
	 */
	إذا (!(ops->flags & SCX_OPS_BUILTIN_IDLE_PER_گرہ)) {
		cpumask_clear(idle_cpumask(NUMA_NO_گرہ)->cpu);
		cpumask_clear(idle_cpumask(NUMA_NO_گرہ)->smt);
		أرجع;
	}

	for_each_گرہ(گرہ) {
		cpumask_clear(idle_cpumask(گرہ)->cpu);
		cpumask_clear(idle_cpumask(گرہ)->smt);
	}
}

فراغ scx_idle_enable(بنية sched_ext_ops *ops)
{
	إذا (!ops->update_idle || (ops->flags & SCX_OPS_KEEP_BUILTIN_IDLE))
		static_branch_enable_cpuslocked(&scx_builtin_idle_enabled);
	وإلا
		static_branch_disable_cpuslocked(&scx_builtin_idle_enabled);

	إذا (ops->flags & SCX_OPS_BUILTIN_IDLE_PER_گرہ)
		static_branch_enable_cpuslocked(&scx_builtin_idle_per_گرہ);
	وإلا
		static_branch_disable_cpuslocked(&scx_builtin_idle_per_گرہ);

	reset_idle_masks(ops);
}

فراغ scx_idle_disable(فراغ)
{
	static_branch_disable(&scx_builtin_idle_enabled);
	static_branch_disable(&scx_builtin_idle_per_گرہ);
}

/********************************************************************************
 * Helpers that can be called from the BPF scheduler.
 */

ساكن صحيح validate_گرہ(بنية scx_sched *sch, صحيح گرہ)
{
	إذا (!static_branch_likely(&scx_builtin_idle_per_گرہ)) {
		scx_error(sch, "per-node idle tracking is disabled");
		أرجع -EOPNOTSUPP;
	}

	/* Return no entry for NUMA_NO_NODE (not a critical scx error) */
	إذا (گرہ == NUMA_NO_گرہ)
		أرجع -ENOENT;

	/* Make sure node is in a valid range */
	إذا (گرہ < 0 || گرہ >= nr_گرہ_ids) {
		scx_error(sch, "invalid node %d", گرہ);
		أرجع -EINVAL;
	}

	/* Make sure the node is part of the set of possible nodes */
	إذا (!گرہ_possible(گرہ)) {
		scx_error(sch, "unavailable node %d", گرہ);
		أرجع -EINVAL;
	}

	أرجع گرہ;
}

__bpf_kfunc_start_defs();

ساكن bool check_builtin_idle_enabled(بنية scx_sched *sch)
{
	إذا (static_branch_likely(&scx_builtin_idle_enabled))
		أرجع true;

	scx_error(sch, "built-in idle tracking is disabled");
	أرجع false;
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
ساكن bool is_bpf_migration_disabled(ثابت بنية task_struct *p)
{
	إذا (p->migration_disabled == 1) {
		إذا (IS_ENABLED(CONFIG_PREEMPT_RCU))
			أرجع p != current;
		أرجع true;
	}
	أرجع p->migration_disabled;
}

ساكن s32 select_cpu_restricted(بنية task_struct *p, s32 prev_cpu,
				 ثابت بنية cpumask *allowed, u64 flags,
				 bool migration_disabled)
{
	s32 cpu;

	إذا (migration_disabled)
		cpu = task_cpu(p);
	وإلا
		cpu = cpumask_first(p->cpus_ptr);

	إذا (!cpumask_test_cpu(cpu, p->cpus_ptr))
		أرجع -EBUSY;

	إذا (allowed && !cpumask_test_cpu(cpu, allowed))
		أرجع -EBUSY;

	إذا (flags & SCX_PICK_IDLE_IN_گرہ) {
		صحيح گرہ = scx_cpu_گرہ_if_enabled(prev_cpu);

		إذا (scx_cpu_گرہ_if_enabled(cpu) != گرہ)
			أرجع -EBUSY;
	}

	إذا (!scx_idle_test_and_clear_cpu(cpu))
		أرجع -EBUSY;

	أرجع cpu;
}

ساكن s32 select_cpu_from_kfunc(بنية scx_sched *sch, بنية task_struct *p,
				 s32 prev_cpu, u64 wake_flags,
				 ثابت بنية cpumask *allowed, u64 flags)
{
	unsigned long irq_flags;
	bool we_locked = false;
	bool migration_disabled;
	s32 cpu;

	إذا (!scx_cpu_valid(sch, prev_cpu, عدم))
		أرجع -EINVAL;

	إذا (!check_builtin_idle_enabled(sch))
		أرجع -EBUSY;

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
	إذا (this_rq()->scx.in_select_cpu) {
		إذا (!scx_kf_arg_task_ok(sch, p))
			أرجع -EINVAL;
		lockdep_assert_held(&p->pi_lock);
	} وإلا إذا (scx_locked_rq()) {
		إذا (task_rq(p) != scx_locked_rq())
			اذهب cross_task;
	} وإلا {
		raw_spin_lock_irqsave(&p->pi_lock, irq_flags);
		we_locked = true;
	}

	/*
	 * This may also be called from ops.enqueue(), so we need to handle
	 * per-CPU tasks as well. For these tasks, skip general idle CPU
	 * selection and validate the task's only usable CPU directly.
	 */
	migration_disabled = is_bpf_migration_disabled(p);

	إذا (p->nr_cpus_allowed == 1 || migration_disabled)
		cpu = select_cpu_restricted(p, prev_cpu, allowed, flags,
					    migration_disabled);
	وإلا
		cpu = scx_select_cpu_dfl(p, prev_cpu, wake_flags,
					 allowed ?: p->cpus_ptr, flags);

	إذا (we_locked)
		raw_spin_unlock_irqrestore(&p->pi_lock, irq_flags);

	أرجع cpu;

cross_task:
	scx_error(sch, "select_cpu kfunc called cross-task on %s[%d]",
		  p->comm, p->pid);
	أرجع -EINVAL;
}

/**
 * scx_bpf_cpu_node - Return the NUMA node the given @cpu belongs to, or
 *		      trigger an error if @cpu is invalid
 * @cpu: target CPU
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 */
__bpf_kfunc s32 scx_bpf_cpu_گرہ(s32 cpu, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch) || !scx_cpu_valid(sch, cpu, عدم))
		أرجع NUMA_NO_گرہ;
	أرجع cpu_to_گرہ(cpu);
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
__bpf_kfunc s32 scx_bpf_select_cpu_dfl(بنية task_struct *p, s32 prev_cpu,
				       u64 wake_flags, bool *is_idle,
				       ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -ENODEV;

	cpu = select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags, عدم, 0);
	إذا (cpu >= 0) {
		*is_idle = true;
		أرجع cpu;
	}
	*is_idle = false;
	أرجع prev_cpu;
}

بنية scx_bpf_select_cpu_and_args {
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
__scx_bpf_select_cpu_and(بنية task_struct *p, ثابت بنية cpumask *cpus_allowed,
			 بنية scx_bpf_select_cpu_and_args *args,
			 ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -ENODEV;

	أرجع select_cpu_from_kfunc(sch, p, args->prev_cpu, args->wake_flags,
				     cpus_allowed, args->flags);
}

/*
 * COMPAT: Will be removed in v6.22.
 */
__bpf_kfunc s32 scx_bpf_select_cpu_and(بنية task_struct *p, s32 prev_cpu, u64 wake_flags,
				       ثابت بنية cpumask *cpus_allowed, u64 flags)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_جڑ);
	إذا (unlikely(!sch))
		أرجع -ENODEV;

#ifdef CONFIG_EXT_SUB_SCHED
	/*
	 * Disallow if any sub-scheds are attached. There is no way to tell
	 * which scheduler called us, so error out @p's scheduler -- read it
	 * under RCU as @p's locks aren't necessarily held here. @p may be a
	 * task past sched_ext_dead() or an idle task, in which case its
	 * scheduler can't be determined and there is nothing obviously wrong
	 * to report; just refuse the call.
	 */
	إذا (unlikely(!list_empty(&sch->children))) {
		بنية scx_sched *tsch = scx_task_sched_rcu(p);

		إذا (tsch)
			scx_error(tsch, "__scx_bpf_select_cpu_and() must be used");
		أرجع -EINVAL;
	}
#endif

	أرجع select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags,
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
__bpf_kfunc ثابت بنية cpumask *
scx_bpf_get_idle_cpumask_گرہ(s32 گرہ, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع cpu_none_mask;

	گرہ = validate_گرہ(sch, گرہ);
	إذا (گرہ < 0)
		أرجع cpu_none_mask;

	أرجع idle_cpumask(گرہ)->cpu;
}

/**
 * scx_bpf_get_idle_cpumask - Get a referenced kptr to the idle-tracking
 * per-CPU cpumask.
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns an empty mask if idle tracking is not enabled, or running on a
 * UP kernel.
 */
__bpf_kfunc ثابت بنية cpumask *scx_bpf_get_idle_cpumask(ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع cpu_none_mask;

	إذا (static_branch_unlikely(&scx_builtin_idle_per_گرہ)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		أرجع cpu_none_mask;
	}

	إذا (!check_builtin_idle_enabled(sch))
		أرجع cpu_none_mask;

	أرجع idle_cpumask(NUMA_NO_گرہ)->cpu;
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
__bpf_kfunc ثابت بنية cpumask *
scx_bpf_get_idle_smtmask_گرہ(s32 گرہ, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع cpu_none_mask;

	گرہ = validate_گرہ(sch, گرہ);
	إذا (گرہ < 0)
		أرجع cpu_none_mask;

	إذا (sched_smt_active())
		أرجع idle_cpumask(گرہ)->smt;
	وإلا
		أرجع idle_cpumask(گرہ)->cpu;
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
__bpf_kfunc ثابت بنية cpumask *scx_bpf_get_idle_smtmask(ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع cpu_none_mask;

	إذا (static_branch_unlikely(&scx_builtin_idle_per_گرہ)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		أرجع cpu_none_mask;
	}

	إذا (!check_builtin_idle_enabled(sch))
		أرجع cpu_none_mask;

	إذا (sched_smt_active())
		أرجع idle_cpumask(NUMA_NO_گرہ)->smt;
	وإلا
		أرجع idle_cpumask(NUMA_NO_گرہ)->cpu;
}

/**
 * scx_bpf_put_idle_cpumask - Release a previously acquired referenced kptr to
 * either the percpu, or SMT idle-tracking cpumask.
 * @idle_mask: &cpumask to use
 */
__bpf_kfunc فراغ scx_bpf_put_idle_cpumask(ثابت بنية cpumask *idle_mask)
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
__bpf_kfunc bool scx_bpf_test_and_clear_cpu_idle(s32 cpu, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع false;

	إذا (!check_builtin_idle_enabled(sch))
		أرجع false;

	إذا (!scx_cpu_valid(sch, cpu, عدم))
		أرجع false;

	أرجع scx_idle_test_and_clear_cpu(cpu);
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
__bpf_kfunc s32 scx_bpf_pick_idle_cpu_گرہ(ثابت بنية cpumask *cpus_allowed,
					   s32 گرہ, u64 flags,
					   ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -ENODEV;

	گرہ = validate_گرہ(sch, گرہ);
	إذا (گرہ < 0)
		أرجع گرہ;

	أرجع scx_pick_idle_cpu(cpus_allowed, گرہ, flags);
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
__bpf_kfunc s32 scx_bpf_pick_idle_cpu(ثابت بنية cpumask *cpus_allowed,
				      u64 flags, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -ENODEV;

	إذا (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_per_گرہ)) {
		scx_error(sch, "per-node idle tracking is enabled");
		أرجع -EBUSY;
	}

	إذا (!check_builtin_idle_enabled(sch))
		أرجع -EBUSY;

	أرجع scx_pick_idle_cpu(cpus_allowed, NUMA_NO_گرہ, flags);
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
__bpf_kfunc s32 scx_bpf_pick_any_cpu_گرہ(ثابت بنية cpumask *cpus_allowed,
					  s32 گرہ, u64 flags,
					  ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -ENODEV;

	گرہ = validate_گرہ(sch, گرہ);
	إذا (گرہ < 0)
		أرجع گرہ;

	cpu = scx_pick_idle_cpu(cpus_allowed, گرہ, flags);
	إذا (cpu >= 0)
		أرجع cpu;

	إذا (flags & SCX_PICK_IDLE_IN_گرہ)
		cpu = cpumask_any_and_distribute(cpumask_of_گرہ(گرہ), cpus_allowed);
	وإلا
		cpu = cpumask_any_distribute(cpus_allowed);
	إذا (cpu < nr_cpu_ids)
		أرجع cpu;
	وإلا
		أرجع -EBUSY;
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
__bpf_kfunc s32 scx_bpf_pick_any_cpu(ثابت بنية cpumask *cpus_allowed,
				     u64 flags, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -ENODEV;

	إذا (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_per_گرہ)) {
		scx_error(sch, "per-node idle tracking is enabled");
		أرجع -EBUSY;
	}

	إذا (static_branch_likely(&scx_builtin_idle_enabled)) {
		cpu = scx_pick_idle_cpu(cpus_allowed, NUMA_NO_گرہ, flags);
		إذا (cpu >= 0)
			أرجع cpu;
	}

	cpu = cpumask_any_distribute(cpus_allowed);
	إذا (cpu < nr_cpu_ids)
		أرجع cpu;
	وإلا
		أرجع -EBUSY;
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_idle)
BTF_ID_FLAGS(func, scx_bpf_cpu_گرہ, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask_گرہ, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask_گرہ, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_idle_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_test_and_clear_cpu_idle, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu_گرہ, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu_گرہ, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu, KF_IMPLICIT_ARGS | KF_RCU)
BTF_KFUNCS_END(scx_kfunc_ids_idle)

ساكن ثابت بنية btf_kfunc_id_set scx_kfunc_set_idle = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_idle,
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
BTF_KFUNCS_START(scx_kfunc_ids_select_cpu)
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_ARGS | KF_RCU)
BTF_KFUNCS_END(scx_kfunc_ids_select_cpu)

ساكن ثابت بنية btf_kfunc_id_set scx_kfunc_set_select_cpu = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_select_cpu,
	.filter			= scx_kfunc_context_filter,
};

صحيح scx_idle_init(فراغ)
{
	أرجع register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_set_idle) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_TRACING, &scx_kfunc_set_idle) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_set_idle) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_set_select_cpu) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_set_select_cpu);
}
