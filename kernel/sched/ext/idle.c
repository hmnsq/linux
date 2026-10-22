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
#ଅନ୍ତର୍ଭାଵ "internal.h"
#ଅନ୍ତର୍ଭାଵ "cid.h"
#ଅନ୍ତର୍ଭାଵ "idle.h"
#ଅନ୍ତର୍ଭାଵ "sub.h"

/* Enable/disable built-in idle CPU selection policy */
ସ୍ଥିର DEFINE_STATIC_ଚାବି_FALSE(scx_builtin_idle_enabled);

/* Enable/disable per-node idle cpumasks */
ସ୍ଥିର DEFINE_STATIC_ଚାବି_FALSE(scx_builtin_idle_per_ଗଣ୍ଠି);

/* Enable/disable LLC aware optimizations */
ସ୍ଥିର DEFINE_STATIC_ଚାବି_FALSE(scx_selcpu_topo_llc);

/* Enable/disable NUMA aware optimizations */
ସ୍ଥିର DEFINE_STATIC_ଚାବି_FALSE(scx_selcpu_topo_numa);

/*
 * cpumasks to track idle CPUs within each NUMA node.
 *
 * If SCX_OPS_BUILTIN_IDLE_PER_NODE is not enabled, a single global cpumask
 * from is used to track all the idle CPUs in the system.
 */
ସଂରଚନା scx_idle_cpus {
	cpumask_var_t cpu;
	cpumask_var_t smt;
};

/*
 * Global host-wide idle cpumasks (used when SCX_OPS_BUILTIN_IDLE_PER_NODE
 * is not enabled).
 */
ସ୍ଥିର ସଂରଚନା scx_idle_cpus scx_idle_global_masks;

/*
 * Per-node idle cpumasks.
 */
ସ୍ଥିର ସଂରଚନା scx_idle_cpus **scx_idle_ଗଣ୍ଠି_masks;

/*
 * Local per-CPU cpumasks (used to generate temporary idle cpumasks).
 */
ସ୍ଥିର DEFINE_PER_CPU(cpumask_var_t, local_idle_cpumask);
ସ୍ଥିର DEFINE_PER_CPU(cpumask_var_t, local_llc_idle_cpumask);
ସ୍ଥିର DEFINE_PER_CPU(cpumask_var_t, local_numa_idle_cpumask);

/*
 * Return the idle masks associated to a target @node.
 *
 * NUMA_NO_NODE identifies the global idle cpumask.
 */
ସ୍ଥିର ସଂରଚନା scx_idle_cpus *idle_cpumask(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି)
{
	ଫେରନ୍ତୁ ଗଣ୍ଠି == NUMA_NO_ଗଣ୍ଠି ? &scx_idle_global_masks : scx_idle_ଗଣ୍ଠି_masks[ଗଣ୍ଠି];
}

/*
 * Returns the NUMA node ID associated with a @cpu, or NUMA_NO_NODE if
 * per-node idle cpumasks are disabled.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_cpu_ଗଣ୍ଠି_if_enabled(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (!static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_per_ଗଣ୍ଠି))
		ଫେରନ୍ତୁ NUMA_NO_ଗଣ୍ଠି;

	ଫେରନ୍ତୁ cpu_to_ଗଣ୍ଠି(cpu);
}

ସ୍ଥିର bool scx_idle_test_and_clear_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି = scx_cpu_ଗଣ୍ଠି_if_enabled(cpu);
	ସଂରଚନା cpumask *idle_cpus = idle_cpumask(ଗଣ୍ଠି)->cpu;

	/*
	 * SMT mask should be cleared whether we can claim @cpu or not. The SMT
	 * cluster is not wholly idle either way. This also prevents
	 * scx_pick_idle_cpu() from getting caught in an infinite loop.
	 */
	ଯଦି (sched_smt_active()) {
		ଅଚଳ ସଂରଚନା cpumask *smt = cpu_smt_mask(cpu);
		ସଂରଚନା cpumask *idle_smts = idle_cpumask(ଗଣ୍ଠି)->smt;

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
		ଯଦି (cpumask_intersects(smt, idle_smts))
			cpumask_andnot(idle_smts, idle_smts, smt);
		ଅନ୍ୟଥା ଯଦି (cpumask_test_cpu(cpu, idle_smts))
			__cpumask_clear_cpu(cpu, idle_smts);
	}

	ଫେରନ୍ତୁ cpumask_test_and_clear_cpu(cpu, idle_cpus);
}

/*
 * Pick an idle CPU in a specific NUMA node.
 */
ସ୍ଥିର s32 pick_idle_cpu_in_ଗଣ୍ଠି(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି, u64 flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

retry:
	ଯଦି (sched_smt_active()) {
		cpu = cpumask_any_and_distribute(idle_cpumask(ଗଣ୍ଠି)->smt, cpus_allowed);
		ଯଦି (cpu < nr_cpu_ids)
			ଯାଅ found;

		ଯଦି (flags & SCX_PICK_IDLE_CORE)
			ଫେରନ୍ତୁ -EBUSY;
	}

	cpu = cpumask_any_and_distribute(idle_cpumask(ଗଣ୍ଠି)->cpu, cpus_allowed);
	ଯଦି (cpu >= nr_cpu_ids)
		ଫେରନ୍ତୁ -EBUSY;

found:
	ଯଦି (scx_idle_test_and_clear_cpu(cpu))
		ଫେରନ୍ତୁ cpu;
	ଅନ୍ୟଥା
		ଯାଅ retry;
}

#ifdef CONFIG_NUMA
/*
 * Tracks nodes that have not yet been visited when searching for an idle
 * CPU across all available nodes.
 */
ସ୍ଥିର DEFINE_PER_CPU(nodemask_t, per_cpu_unvisited);

/*
 * Search for an idle CPU across all nodes, excluding @node.
 */
ସ୍ଥିର s32 pick_idle_cpu_from_online_nodes(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି, u64 flags)
{
	nodemask_t *unvisited;
	s32 cpu = -EBUSY;

	preempt_disable();
	unvisited = this_cpu_ptr(&per_cpu_unvisited);

	/*
	 * Restrict the search to the online nodes (excluding the current
	 * node that has been visited already).
	 */
	nodes_copy(*unvisited, ଗଣ୍ଠି_states[N_ONLINE]);
	ଗଣ୍ଠି_clear(ଗଣ୍ଠି, *unvisited);

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
	for_each_ଗଣ୍ଠି_numadist(ଗଣ୍ଠି, *unvisited) {
		cpu = pick_idle_cpu_in_ଗଣ୍ଠି(cpus_allowed, ଗଣ୍ଠି, flags);
		ଯଦି (cpu >= 0)
			ଭାଙ୍ଗ;
	}
	preempt_enable();

	ଫେରନ୍ତୁ cpu;
}
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ s32
pick_idle_cpu_from_online_nodes(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି, u64 flags)
{
	ଫେରନ୍ତୁ -EBUSY;
}
#endif

/*
 * Find an idle CPU in the system, starting from @node.
 */
ସ୍ଥିର s32 scx_pick_idle_cpu(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି, u64 flags)
{
	s32 cpu;

	/*
	 * Always search in the starting node first (this is an
	 * optimization that can save some cycles even when the search is
	 * not limited to a single node).
	 */
	cpu = pick_idle_cpu_in_ଗଣ୍ଠି(cpus_allowed, ଗଣ୍ଠି, flags);
	ଯଦି (cpu >= 0)
		ଫେରନ୍ତୁ cpu;

	/*
	 * Stop the search if we are using only a single global cpumask
	 * (NUMA_NO_NODE) or if the search is restricted to the first node
	 * only.
	 */
	ଯଦି (ଗଣ୍ଠି == NUMA_NO_ଗଣ୍ଠି || flags & SCX_PICK_IDLE_IN_ଗଣ୍ଠି)
		ଫେରନ୍ତୁ -EBUSY;

	/*
	 * Extend the search to the other online nodes.
	 */
	ଫେରନ୍ତୁ pick_idle_cpu_from_online_nodes(cpus_allowed, ଗଣ୍ଠି, flags);
}

/*
 * Return the amount of CPUs in the same LLC domain of @cpu (or zero if the LLC
 * domain is not defined).
 */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା llc_weight(s32 cpu)
{
	ସଂରଚନା sched_domain *sd;

	sd = rcu_dereference(per_cpu(sd_llc, cpu));
	ଯଦି (!sd)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ sd->span_weight;
}

/*
 * Return the cpumask representing the LLC domain of @cpu (or NULL if the LLC
 * domain is not defined).
 */
ସ୍ଥିର ସଂରଚନା cpumask *llc_span(s32 cpu)
{
	ସଂରଚନା sched_domain *sd;

	sd = rcu_dereference(per_cpu(sd_llc, cpu));
	ଯଦି (!sd)
		ଫେରନ୍ତୁ ଖାଲି;

	ଫେରନ୍ତୁ sched_domain_span(sd);
}

/*
 * Return the amount of CPUs in the same NUMA domain of @cpu (or zero if the
 * NUMA domain is not defined).
 */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା numa_weight(s32 cpu)
{
	ସଂରଚନା sched_domain *sd;
	ସଂରଚନା sched_group *sg;

	sd = rcu_dereference(per_cpu(sd_numa, cpu));
	ଯଦି (!sd)
		ଫେରନ୍ତୁ 0;
	sg = sd->groups;
	ଯଦି (!sg)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ sg->group_weight;
}

/*
 * Return the cpumask representing the NUMA domain of @cpu (or NULL if the NUMA
 * domain is not defined).
 */
ସ୍ଥିର ସଂରଚନା cpumask *numa_span(s32 cpu)
{
	ସଂରଚନା sched_domain *sd;
	ସଂରଚନା sched_group *sg;

	sd = rcu_dereference(per_cpu(sd_numa, cpu));
	ଯଦି (!sd)
		ଫେରନ୍ତୁ ଖାଲି;
	sg = sd->groups;
	ଯଦି (!sg)
		ଫେରନ୍ତୁ ଖାଲି;

	ଫେରନ୍ତୁ sched_group_span(sg);
}

/*
 * Return true if the LLC domains do not perfectly overlap with the NUMA
 * domains, false otherwise.
 */
ସ୍ଥିର bool llc_numa_mismatch(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

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
		ଯଦି (llc_weight(cpu) != numa_weight(cpu))
			ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ false;
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
ଶୂନ୍ୟ scx_idle_update_selcpu_topology(ସଂରଚନା sched_ext_ops *ops)
{
	bool enable_llc = false, enable_numa = false;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_cpus;
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
	ଯଦି (nr_cpus > 0) {
		ଯଦି (nr_cpus < ଅଙ୍କ_online_cpus())
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
	ଯଦି (!(ops->flags & SCX_OPS_BUILTIN_IDLE_PER_ଗଣ୍ଠି)) {
		nr_cpus = numa_weight(cpu);
		ଯଦି (nr_cpus > 0) {
			ଯଦି (nr_cpus < ଅଙ୍କ_online_cpus() && llc_numa_mismatch())
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

	ଯଦି (enable_llc)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_llc);
	ଅନ୍ୟଥା
		static_branch_disable_cpuslocked(&scx_selcpu_topo_llc);
	ଯଦି (enable_numa)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_numa);
	ଅନ୍ୟଥା
		static_branch_disable_cpuslocked(&scx_selcpu_topo_numa);
}

/*
 * Return true if @p can run on all possible CPUs, false otherwise.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_affinity_all(ଅଚଳ ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ p->nr_cpus_allowed >= ଅଙ୍କ_possible_cpus();
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
s32 scx_select_cpu_dfl(ସଂରଚନା task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ଅଚଳ ସଂରଚନା cpumask *cpus_allowed, u64 flags)
{
	ଅଚଳ ସଂରଚନା cpumask *llc_cpus = ଖାଲି, *numa_cpus = ଖାଲି;
	ଅଚଳ ସଂରଚନା cpumask *allowed = cpus_allowed ?: p->cpus_ptr;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି = scx_cpu_ଗଣ୍ଠି_if_enabled(prev_cpu);
	bool is_prev_allowed;
	s32 cpu;

	preempt_disable();

	/*
	 * Determine the subset of CPUs usable by @p within @cpus_allowed.
	 */
	ଯଦି (allowed != p->cpus_ptr) {
		ସଂରଚନା cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_idle_cpumask);

		ଯଦି (task_affinity_all(p)) {
			allowed = cpus_allowed;
		} ଅନ୍ୟଥା ଯଦି (cpumask_and(local_cpus, cpus_allowed, p->cpus_ptr)) {
			allowed = local_cpus;
		} ଅନ୍ୟଥା {
			cpu = -EBUSY;
			ଯାଅ out_enable;
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
	ଯଦି (static_branch_maybe(CONFIG_NUMA, &scx_selcpu_topo_numa)) {
		ସଂରଚନା cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_numa_idle_cpumask);
		ଅଚଳ ସଂରଚନା cpumask *cpus = numa_span(prev_cpu);

		ଯଦି (allowed == p->cpus_ptr && task_affinity_all(p))
			numa_cpus = cpus;
		ଅନ୍ୟଥା ଯଦି (cpus && cpumask_and(local_cpus, allowed, cpus))
			numa_cpus = local_cpus;
	}

	ଯଦି (static_branch_maybe(CONFIG_SCHED_MC, &scx_selcpu_topo_llc)) {
		ସଂରଚନା cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_llc_idle_cpumask);
		ଅଚଳ ସଂରଚନା cpumask *cpus = llc_span(prev_cpu);

		ଯଦି (allowed == p->cpus_ptr && task_affinity_all(p))
			llc_cpus = cpus;
		ଅନ୍ୟଥା ଯଦି (cpus && cpumask_and(local_cpus, allowed, cpus))
			llc_cpus = local_cpus;
	}

	/*
	 * If WAKE_SYNC, try to migrate the wakee to the waker's CPU.
	 */
	ଯଦି (wake_flags & SCX_WAKE_SYNC) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା waker_ଗଣ୍ଠି;

		/*
		 * If the waker's CPU is cache affine and prev_cpu is idle,
		 * then avoid a migration.
		 */
		cpu = smp_processor_id();
		ଯଦି (is_prev_allowed && cpus_share_cache(cpu, prev_cpu) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			ଯାଅ out_unlock;
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
		waker_ଗଣ୍ଠି = scx_cpu_ଗଣ୍ଠି_if_enabled(cpu);
		ଯଦି (!(current->flags & PF_EXITING) &&
		    cpu_rq(cpu)->scx.local_dsq.nr == 0 &&
		    (!(flags & SCX_PICK_IDLE_IN_ଗଣ୍ଠି) || (waker_ଗଣ୍ଠି == ଗଣ୍ଠି)) &&
		    !cpumask_empty(idle_cpumask(waker_ଗଣ୍ଠି)->cpu)) {
			ଯଦି (cpumask_test_cpu(cpu, allowed)) {
				scx_idle_test_and_clear_cpu(cpu);
				ଯାଅ out_unlock;
			}
		}
	}

	/*
	 * If CPU has SMT, any wholly idle CPU is likely a better pick than
	 * partially idle @prev_cpu.
	 */
	ଯଦି (sched_smt_active()) {
		/*
		 * Keep using @prev_cpu if it's part of a fully idle core.
		 */
		ଯଦି (is_prev_allowed &&
		    cpumask_test_cpu(prev_cpu, idle_cpumask(ଗଣ୍ଠି)->smt) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			ଯାଅ out_unlock;
		}

		/*
		 * Search for any fully idle core in the same LLC domain.
		 */
		ଯଦି (llc_cpus) {
			cpu = pick_idle_cpu_in_ଗଣ୍ଠି(llc_cpus, ଗଣ୍ଠି, SCX_PICK_IDLE_CORE);
			ଯଦି (cpu >= 0)
				ଯାଅ out_unlock;
		}

		/*
		 * Search for any fully idle core in the same NUMA node.
		 */
		ଯଦି (numa_cpus) {
			cpu = pick_idle_cpu_in_ଗଣ୍ଠି(numa_cpus, ଗଣ୍ଠି, SCX_PICK_IDLE_CORE);
			ଯଦି (cpu >= 0)
				ଯାଅ out_unlock;
		}

		/*
		 * Search for any full-idle core usable by the task.
		 *
		 * If the node-aware idle CPU selection policy is enabled
		 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always
		 * begin in prev_cpu's node and proceed to other nodes in
		 * order of increasing distance.
		 */
		cpu = scx_pick_idle_cpu(allowed, ଗଣ୍ଠି, flags | SCX_PICK_IDLE_CORE);
		ଯଦି (cpu >= 0)
			ଯାଅ out_unlock;

		/*
		 * Give up if we're strictly looking for a full-idle SMT
		 * core.
		 */
		ଯଦି (flags & SCX_PICK_IDLE_CORE) {
			cpu = -EBUSY;
			ଯାଅ out_unlock;
		}
	}

	/*
	 * Use @prev_cpu if it's idle.
	 */
	ଯଦି (is_prev_allowed && scx_idle_test_and_clear_cpu(prev_cpu)) {
		cpu = prev_cpu;
		ଯାଅ out_unlock;
	}

	/*
	 * Use @prev_cpu's sibling if it's idle.
	 */
	ଯଦି (sched_smt_active()) {
		for_each_cpu_and(cpu, cpu_smt_mask(prev_cpu), allowed) {
			ଯଦି (cpu == prev_cpu)
				ଜାରି;
			ଯଦି (scx_idle_test_and_clear_cpu(cpu))
				ଯାଅ out_unlock;
		}
	}

	/*
	 * Search for any idle CPU in the same LLC domain.
	 */
	ଯଦି (llc_cpus) {
		cpu = pick_idle_cpu_in_ଗଣ୍ଠି(llc_cpus, ଗଣ୍ଠି, 0);
		ଯଦି (cpu >= 0)
			ଯାଅ out_unlock;
	}

	/*
	 * Search for any idle CPU in the same NUMA node.
	 */
	ଯଦି (numa_cpus) {
		cpu = pick_idle_cpu_in_ଗଣ୍ଠି(numa_cpus, ଗଣ୍ଠି, 0);
		ଯଦି (cpu >= 0)
			ଯାଅ out_unlock;
	}

	/*
	 * Search for any idle CPU usable by the task.
	 *
	 * If the node-aware idle CPU selection policy is enabled
	 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always begin
	 * in prev_cpu's node and proceed to other nodes in order of
	 * increasing distance.
	 */
	cpu = scx_pick_idle_cpu(allowed, ଗଣ୍ଠି, flags);

out_unlock:
	rcu_read_unlock();
out_enable:
	preempt_enable();

	ଫେରନ୍ତୁ cpu;
}

/*
 * Initialize global and per-node idle cpumasks.
 */
ଶୂନ୍ୟ scx_idle_init_masks(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	/* Allocate global idle cpumasks */
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.cpu, GFP_KERNEL));
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.smt, GFP_KERNEL));

	/* Allocate per-node idle cpumasks (use nr_node_ids for non-contiguous NUMA nodes) */
	scx_idle_ଗଣ୍ଠି_masks = kzalloc_objs(*scx_idle_ଗଣ୍ଠି_masks, nr_ଗଣ୍ଠି_ids);
	BUG_ON(!scx_idle_ଗଣ୍ଠି_masks);

	for_each_ଗଣ୍ଠି(i) {
		scx_idle_ଗଣ୍ଠି_masks[i] = kzalloc_ଗଣ୍ଠି(ଆକାର(**scx_idle_ଗଣ୍ଠି_masks),
							 GFP_KERNEL, i);
		BUG_ON(!scx_idle_ଗଣ୍ଠି_masks[i]);

		BUG_ON(!alloc_cpumask_var_ଗଣ୍ଠି(&scx_idle_ଗଣ୍ଠି_masks[i]->cpu, GFP_KERNEL, i));
		BUG_ON(!alloc_cpumask_var_ଗଣ୍ଠି(&scx_idle_ଗଣ୍ଠି_masks[i]->smt, GFP_KERNEL, i));
	}

	/* Allocate local per-cpu idle cpumasks */
	for_each_possible_cpu(i) {
		BUG_ON(!alloc_cpumask_var_ଗଣ୍ଠି(&per_cpu(local_idle_cpumask, i),
					       GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i)));
		BUG_ON(!alloc_cpumask_var_ଗଣ୍ଠି(&per_cpu(local_llc_idle_cpumask, i),
					       GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i)));
		BUG_ON(!alloc_cpumask_var_ଗଣ୍ଠି(&per_cpu(local_numa_idle_cpumask, i),
					       GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i)));
	}
}

ସ୍ଥିର ଶୂନ୍ୟ update_builtin_idle(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool idle)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି = scx_cpu_ଗଣ୍ଠି_if_enabled(cpu);
	ସଂରଚନା cpumask *idle_cpus = idle_cpumask(ଗଣ୍ଠି)->cpu;

	assign_cpu(cpu, idle_cpus, idle);

	ଯଦି (sched_smt_active()) {
		ଅଚଳ ସଂରଚନା cpumask *smt = cpu_smt_mask(cpu);
		ସଂରଚନା cpumask *idle_smts = idle_cpumask(ଗଣ୍ଠି)->smt;

		ଯଦି (idle) {
			/*
			 * idle_smt handling is racy but that's fine as it's
			 * only for optimization and self-correcting.
			 */
			ଯଦି (!cpumask_subset(smt, idle_cpus))
				ଫେରନ୍ତୁ;
			cpumask_or(idle_smts, idle_smts, smt);
		} ଅନ୍ୟଥା {
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
ସ୍ଥିର ଶୂନ୍ୟ scx_idle_notify(ସଂରଚନା rq *rq, bool idle, bool do_notify, bool ମୂଳ_renotify)
{
	s32 cpu = cpu_of(rq);
	s32 cid = scx_cpu_arg(cpu);
	ସଂରଚନା scx_sched *ମୂଳ = scx_ମୂଳ_protected_live();
	ସଂରଚନା scx_sched *pos;

	lockdep_assert_rq_held(rq);

	/* with no sub-sched, only the root can be owed a notification */
	ଯଦି (!scx_has_subs()) {
		ଯଦି ((do_notify || ମୂଳ_renotify) &&
		    SCX_HAS_OP(ମୂଳ, update_idle) && !scx_bypassing(ମୂଳ, cpu))
			SCX_CALL_OP(ମୂଳ, update_idle, rq, cid, idle);
		ଫେରନ୍ତୁ;
	}

	pos = scx_next_descendant_pre(ଖାଲି, ମୂଳ);
	ଯେତେବେଳେ (pos) {
		bool forced = false;

		ଯଦି (unlikely(scx_missing_caps(pos, cpu, SCX_CAP_BASE))) {
			pos = scx_skip_subtree_pre(pos, ମୂଳ);
			ଜାରି;
		}

		ଯଦି (!pos->level) {
			forced = ମୂଳ_renotify;
		}
#ifdef CONFIG_EXT_SUB_SCHED
		ଅନ୍ୟଥା ଯଦି (per_cpu_ptr(pos->pcpu, cpu)->idle_renotify) {
			per_cpu_ptr(pos->pcpu, cpu)->idle_renotify = false;
			forced = true;
		}
#endif
		ଯଦି ((do_notify || forced) && SCX_HAS_OP(pos, update_idle) &&
		    !scx_bypassing(pos, cpu))
			SCX_CALL_OP(pos, update_idle, rq, cid, idle);
		pos = scx_next_descendant_pre(pos, ମୂଳ);
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
ଶୂନ୍ୟ __scx_update_idle(ସଂରଚନା rq *rq, bool idle, bool do_notify)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpu_of(rq);

	lockdep_assert_rq_held(rq);

	/*
	 * pick_task_idle() calls here only on an idle-to-idle re-pick and the
	 * transitions call with @do_notify, so every reaching call updates the
	 * masks.
	 */
	ଯଦି (static_branch_likely(&scx_builtin_idle_enabled))
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
	ଯଦି (do_notify ||
	    (idle && (rq->scx.flags &
		      (SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_ମୂଳ_IDLE_RENOTIFY)))) {
		bool ମୂଳ_renotify = rq->scx.flags & SCX_RQ_ମୂଳ_IDLE_RENOTIFY;

		rq->scx.flags &= ~(SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_ମୂଳ_IDLE_RENOTIFY);
		scx_idle_notify(rq, idle, do_notify, ମୂଳ_renotify);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ reset_idle_masks(ସଂରଚନା sched_ext_ops *ops)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି;

	/*
	 * Start with all CPUs marked busy. The idle masks are populated when
	 * bypass is lifted and each idle CPU is forced through an idle re-pick.
	 * This may temporarily omit idle CPUs but never advertises a busy CPU as
	 * idle.
	 */
	ଯଦି (!(ops->flags & SCX_OPS_BUILTIN_IDLE_PER_ଗଣ୍ଠି)) {
		cpumask_clear(idle_cpumask(NUMA_NO_ଗଣ୍ଠି)->cpu);
		cpumask_clear(idle_cpumask(NUMA_NO_ଗଣ୍ଠି)->smt);
		ଫେରନ୍ତୁ;
	}

	for_each_ଗଣ୍ଠି(ଗଣ୍ଠି) {
		cpumask_clear(idle_cpumask(ଗଣ୍ଠି)->cpu);
		cpumask_clear(idle_cpumask(ଗଣ୍ଠି)->smt);
	}
}

ଶୂନ୍ୟ scx_idle_enable(ସଂରଚନା sched_ext_ops *ops)
{
	ଯଦି (!ops->update_idle || (ops->flags & SCX_OPS_KEEP_BUILTIN_IDLE))
		static_branch_enable_cpuslocked(&scx_builtin_idle_enabled);
	ଅନ୍ୟଥା
		static_branch_disable_cpuslocked(&scx_builtin_idle_enabled);

	ଯଦି (ops->flags & SCX_OPS_BUILTIN_IDLE_PER_ଗଣ୍ଠି)
		static_branch_enable_cpuslocked(&scx_builtin_idle_per_ଗଣ୍ଠି);
	ଅନ୍ୟଥା
		static_branch_disable_cpuslocked(&scx_builtin_idle_per_ଗଣ୍ଠି);

	reset_idle_masks(ops);
}

ଶୂନ୍ୟ scx_idle_disable(ଶୂନ୍ୟ)
{
	static_branch_disable(&scx_builtin_idle_enabled);
	static_branch_disable(&scx_builtin_idle_per_ଗଣ୍ଠି);
}

/********************************************************************************
 * Helpers that can be called from the BPF scheduler.
 */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା validate_ଗଣ୍ଠି(ସଂରଚନା scx_sched *sch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି)
{
	ଯଦି (!static_branch_likely(&scx_builtin_idle_per_ଗଣ୍ଠି)) {
		scx_error(sch, "per-node idle tracking is disabled");
		ଫେରନ୍ତୁ -EOPNOTSUPP;
	}

	/* Return no entry for NUMA_NO_NODE (not a critical scx error) */
	ଯଦି (ଗଣ୍ଠି == NUMA_NO_ଗଣ୍ଠି)
		ଫେରନ୍ତୁ -ENOENT;

	/* Make sure node is in a valid range */
	ଯଦି (ଗଣ୍ଠି < 0 || ଗଣ୍ଠି >= nr_ଗଣ୍ଠି_ids) {
		scx_error(sch, "invalid node %d", ଗଣ୍ଠି);
		ଫେରନ୍ତୁ -EINVAL;
	}

	/* Make sure the node is part of the set of possible nodes */
	ଯଦି (!ଗଣ୍ଠି_possible(ଗଣ୍ଠି)) {
		scx_error(sch, "unavailable node %d", ଗଣ୍ଠି);
		ଫେରନ୍ତୁ -EINVAL;
	}

	ଫେରନ୍ତୁ ଗଣ୍ଠି;
}

__bpf_kfunc_start_defs();

ସ୍ଥିର bool check_builtin_idle_enabled(ସଂରଚନା scx_sched *sch)
{
	ଯଦି (static_branch_likely(&scx_builtin_idle_enabled))
		ଫେରନ୍ତୁ true;

	scx_error(sch, "built-in idle tracking is disabled");
	ଫେରନ୍ତୁ false;
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
ସ୍ଥିର bool is_bpf_migration_disabled(ଅଚଳ ସଂରଚନା task_struct *p)
{
	ଯଦି (p->migration_disabled == 1) {
		ଯଦି (IS_ENABLED(CONFIG_PREEMPT_RCU))
			ଫେରନ୍ତୁ p != current;
		ଫେରନ୍ତୁ true;
	}
	ଫେରନ୍ତୁ p->migration_disabled;
}

ସ୍ଥିର s32 select_cpu_restricted(ସଂରଚନା task_struct *p, s32 prev_cpu,
				 ଅଚଳ ସଂରଚନା cpumask *allowed, u64 flags,
				 bool migration_disabled)
{
	s32 cpu;

	ଯଦି (migration_disabled)
		cpu = task_cpu(p);
	ଅନ୍ୟଥା
		cpu = cpumask_first(p->cpus_ptr);

	ଯଦି (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ଫେରନ୍ତୁ -EBUSY;

	ଯଦି (allowed && !cpumask_test_cpu(cpu, allowed))
		ଫେରନ୍ତୁ -EBUSY;

	ଯଦି (flags & SCX_PICK_IDLE_IN_ଗଣ୍ଠି) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି = scx_cpu_ଗଣ୍ଠି_if_enabled(prev_cpu);

		ଯଦି (scx_cpu_ଗଣ୍ଠି_if_enabled(cpu) != ଗଣ୍ଠି)
			ଫେରନ୍ତୁ -EBUSY;
	}

	ଯଦି (!scx_idle_test_and_clear_cpu(cpu))
		ଫେରନ୍ତୁ -EBUSY;

	ଫେରନ୍ତୁ cpu;
}

ସ୍ଥିର s32 select_cpu_from_kfunc(ସଂରଚନା scx_sched *sch, ସଂରଚନା task_struct *p,
				 s32 prev_cpu, u64 wake_flags,
				 ଅଚଳ ସଂରଚନା cpumask *allowed, u64 flags)
{
	unsigned long irq_flags;
	bool we_locked = false;
	bool migration_disabled;
	s32 cpu;

	ଯଦି (!scx_cpu_valid(sch, prev_cpu, ଖାଲି))
		ଫେରନ୍ତୁ -EINVAL;

	ଯଦି (!check_builtin_idle_enabled(sch))
		ଫେରନ୍ତୁ -EBUSY;

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
	ଯଦି (this_rq()->scx.in_select_cpu) {
		ଯଦି (!scx_kf_arg_task_ok(sch, p))
			ଫେରନ୍ତୁ -EINVAL;
		lockdep_assert_held(&p->pi_lock);
	} ଅନ୍ୟଥା ଯଦି (scx_locked_rq()) {
		ଯଦି (task_rq(p) != scx_locked_rq())
			ଯାଅ cross_task;
	} ଅନ୍ୟଥା {
		raw_spin_lock_irqsave(&p->pi_lock, irq_flags);
		we_locked = true;
	}

	/*
	 * This may also be called from ops.enqueue(), so we need to handle
	 * per-CPU tasks as well. For these tasks, skip general idle CPU
	 * selection and validate the task's only usable CPU directly.
	 */
	migration_disabled = is_bpf_migration_disabled(p);

	ଯଦି (p->nr_cpus_allowed == 1 || migration_disabled)
		cpu = select_cpu_restricted(p, prev_cpu, allowed, flags,
					    migration_disabled);
	ଅନ୍ୟଥା
		cpu = scx_select_cpu_dfl(p, prev_cpu, wake_flags,
					 allowed ?: p->cpus_ptr, flags);

	ଯଦି (we_locked)
		raw_spin_unlock_irqrestore(&p->pi_lock, irq_flags);

	ଫେରନ୍ତୁ cpu;

cross_task:
	scx_error(sch, "select_cpu kfunc called cross-task on %s[%d]",
		  p->comm, p->pid);
	ଫେରନ୍ତୁ -EINVAL;
}

/**
 * scx_bpf_cpu_node - Return the NUMA node the given @cpu belongs to, or
 *		      trigger an error if @cpu is invalid
 * @cpu: target CPU
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 */
__bpf_kfunc s32 scx_bpf_cpu_ଗଣ୍ଠି(s32 cpu, ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch) || !scx_cpu_valid(sch, cpu, ଖାଲି))
		ଫେରନ୍ତୁ NUMA_NO_ଗଣ୍ଠି;
	ଫେରନ୍ତୁ cpu_to_ଗଣ୍ଠି(cpu);
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
__bpf_kfunc s32 scx_bpf_select_cpu_dfl(ସଂରଚନା task_struct *p, s32 prev_cpu,
				       u64 wake_flags, bool *is_idle,
				       ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ -ENODEV;

	cpu = select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags, ଖାଲି, 0);
	ଯଦି (cpu >= 0) {
		*is_idle = true;
		ଫେରନ୍ତୁ cpu;
	}
	*is_idle = false;
	ଫେରନ୍ତୁ prev_cpu;
}

ସଂରଚନା scx_bpf_select_cpu_and_args {
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
__scx_bpf_select_cpu_and(ସଂରଚନା task_struct *p, ଅଚଳ ସଂରଚନା cpumask *cpus_allowed,
			 ସଂରଚନା scx_bpf_select_cpu_and_args *args,
			 ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ -ENODEV;

	ଫେରନ୍ତୁ select_cpu_from_kfunc(sch, p, args->prev_cpu, args->wake_flags,
				     cpus_allowed, args->flags);
}

/*
 * COMPAT: Will be removed in v6.22.
 */
__bpf_kfunc s32 scx_bpf_select_cpu_and(ସଂରଚନା task_struct *p, s32 prev_cpu, u64 wake_flags,
				       ଅଚଳ ସଂରଚନା cpumask *cpus_allowed, u64 flags)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_ମୂଳ);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ -ENODEV;

#ifdef CONFIG_EXT_SUB_SCHED
	/*
	 * Disallow if any sub-scheds are attached. There is no way to tell
	 * which scheduler called us, so error out @p's scheduler -- read it
	 * under RCU as @p's locks aren't necessarily held here. @p may be a
	 * task past sched_ext_dead() or an idle task, in which case its
	 * scheduler can't be determined and there is nothing obviously wrong
	 * to report; just refuse the call.
	 */
	ଯଦି (unlikely(!list_empty(&sch->children))) {
		ସଂରଚନା scx_sched *tsch = scx_task_sched_rcu(p);

		ଯଦି (tsch)
			scx_error(tsch, "__scx_bpf_select_cpu_and() must be used");
		ଫେରନ୍ତୁ -EINVAL;
	}
#endif

	ଫେରନ୍ତୁ select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags,
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
__bpf_kfunc ଅଚଳ ସଂରଚନା cpumask *
scx_bpf_get_idle_cpumask_ଗଣ୍ଠି(s32 ଗଣ୍ଠି, ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ cpu_none_mask;

	ଗଣ୍ଠି = validate_ଗଣ୍ଠି(sch, ଗଣ୍ଠି);
	ଯଦି (ଗଣ୍ଠି < 0)
		ଫେରନ୍ତୁ cpu_none_mask;

	ଫେରନ୍ତୁ idle_cpumask(ଗଣ୍ଠି)->cpu;
}

/**
 * scx_bpf_get_idle_cpumask - Get a referenced kptr to the idle-tracking
 * per-CPU cpumask.
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns an empty mask if idle tracking is not enabled, or running on a
 * UP kernel.
 */
__bpf_kfunc ଅଚଳ ସଂରଚନା cpumask *scx_bpf_get_idle_cpumask(ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ cpu_none_mask;

	ଯଦି (static_branch_unlikely(&scx_builtin_idle_per_ଗଣ୍ଠି)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		ଫେରନ୍ତୁ cpu_none_mask;
	}

	ଯଦି (!check_builtin_idle_enabled(sch))
		ଫେରନ୍ତୁ cpu_none_mask;

	ଫେରନ୍ତୁ idle_cpumask(NUMA_NO_ଗଣ୍ଠି)->cpu;
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
__bpf_kfunc ଅଚଳ ସଂରଚନା cpumask *
scx_bpf_get_idle_smtmask_ଗଣ୍ଠି(s32 ଗଣ୍ଠି, ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ cpu_none_mask;

	ଗଣ୍ଠି = validate_ଗଣ୍ଠି(sch, ଗଣ୍ଠି);
	ଯଦି (ଗଣ୍ଠି < 0)
		ଫେରନ୍ତୁ cpu_none_mask;

	ଯଦି (sched_smt_active())
		ଫେରନ୍ତୁ idle_cpumask(ଗଣ୍ଠି)->smt;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ idle_cpumask(ଗଣ୍ଠି)->cpu;
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
__bpf_kfunc ଅଚଳ ସଂରଚନା cpumask *scx_bpf_get_idle_smtmask(ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ cpu_none_mask;

	ଯଦି (static_branch_unlikely(&scx_builtin_idle_per_ଗଣ୍ଠି)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		ଫେରନ୍ତୁ cpu_none_mask;
	}

	ଯଦି (!check_builtin_idle_enabled(sch))
		ଫେରନ୍ତୁ cpu_none_mask;

	ଯଦି (sched_smt_active())
		ଫେରନ୍ତୁ idle_cpumask(NUMA_NO_ଗଣ୍ଠି)->smt;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ idle_cpumask(NUMA_NO_ଗଣ୍ଠି)->cpu;
}

/**
 * scx_bpf_put_idle_cpumask - Release a previously acquired referenced kptr to
 * either the percpu, or SMT idle-tracking cpumask.
 * @idle_mask: &cpumask to use
 */
__bpf_kfunc ଶୂନ୍ୟ scx_bpf_put_idle_cpumask(ଅଚଳ ସଂରଚନା cpumask *idle_mask)
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
__bpf_kfunc bool scx_bpf_test_and_clear_cpu_idle(s32 cpu, ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ false;

	ଯଦି (!check_builtin_idle_enabled(sch))
		ଫେରନ୍ତୁ false;

	ଯଦି (!scx_cpu_valid(sch, cpu, ଖାଲି))
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ scx_idle_test_and_clear_cpu(cpu);
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
__bpf_kfunc s32 scx_bpf_pick_idle_cpu_ଗଣ୍ଠି(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed,
					   s32 ଗଣ୍ଠି, u64 flags,
					   ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ -ENODEV;

	ଗଣ୍ଠି = validate_ଗଣ୍ଠି(sch, ଗଣ୍ଠି);
	ଯଦି (ଗଣ୍ଠି < 0)
		ଫେରନ୍ତୁ ଗଣ୍ଠି;

	ଫେରନ୍ତୁ scx_pick_idle_cpu(cpus_allowed, ଗଣ୍ଠି, flags);
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
__bpf_kfunc s32 scx_bpf_pick_idle_cpu(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed,
				      u64 flags, ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ -ENODEV;

	ଯଦି (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_per_ଗଣ୍ଠି)) {
		scx_error(sch, "per-node idle tracking is enabled");
		ଫେରନ୍ତୁ -EBUSY;
	}

	ଯଦି (!check_builtin_idle_enabled(sch))
		ଫେରନ୍ତୁ -EBUSY;

	ଫେରନ୍ତୁ scx_pick_idle_cpu(cpus_allowed, NUMA_NO_ଗଣ୍ଠି, flags);
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
__bpf_kfunc s32 scx_bpf_pick_any_cpu_ଗଣ୍ଠି(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed,
					  s32 ଗଣ୍ଠି, u64 flags,
					  ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ -ENODEV;

	ଗଣ୍ଠି = validate_ଗଣ୍ଠି(sch, ଗଣ୍ଠି);
	ଯଦି (ଗଣ୍ଠି < 0)
		ଫେରନ୍ତୁ ଗଣ୍ଠି;

	cpu = scx_pick_idle_cpu(cpus_allowed, ଗଣ୍ଠି, flags);
	ଯଦି (cpu >= 0)
		ଫେରନ୍ତୁ cpu;

	ଯଦି (flags & SCX_PICK_IDLE_IN_ଗଣ୍ଠି)
		cpu = cpumask_any_and_distribute(cpumask_of_ଗଣ୍ଠି(ଗଣ୍ଠି), cpus_allowed);
	ଅନ୍ୟଥା
		cpu = cpumask_any_distribute(cpus_allowed);
	ଯଦି (cpu < nr_cpu_ids)
		ଫେରନ୍ତୁ cpu;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ -EBUSY;
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
__bpf_kfunc s32 scx_bpf_pick_any_cpu(ଅଚଳ ସଂରଚନା cpumask *cpus_allowed,
				     u64 flags, ଅଚଳ ସଂରଚନା bpf_prog_aux *aux)
{
	ସଂରଚନା scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ଯଦି (unlikely(!sch))
		ଫେରନ୍ତୁ -ENODEV;

	ଯଦି (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_per_ଗଣ୍ଠି)) {
		scx_error(sch, "per-node idle tracking is enabled");
		ଫେରନ୍ତୁ -EBUSY;
	}

	ଯଦି (static_branch_likely(&scx_builtin_idle_enabled)) {
		cpu = scx_pick_idle_cpu(cpus_allowed, NUMA_NO_ଗଣ୍ଠି, flags);
		ଯଦି (cpu >= 0)
			ଫେରନ୍ତୁ cpu;
	}

	cpu = cpumask_any_distribute(cpus_allowed);
	ଯଦି (cpu < nr_cpu_ids)
		ଫେରନ୍ତୁ cpu;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ -EBUSY;
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_idle)
BTF_ID_FLAGS(func, scx_bpf_cpu_ଗଣ୍ଠି, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask_ଗଣ୍ଠି, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask_ଗଣ୍ଠି, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask, KF_IMPLICIT_ARGS | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_idle_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_test_and_clear_cpu_idle, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu_ଗଣ୍ଠି, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu_ଗଣ୍ଠି, KF_IMPLICIT_ARGS | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu, KF_IMPLICIT_ARGS | KF_RCU)
BTF_KFUNCS_END(scx_kfunc_ids_idle)

ସ୍ଥିର ଅଚଳ ସଂରଚନା btf_kfunc_id_set scx_kfunc_set_idle = {
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

ସ୍ଥିର ଅଚଳ ସଂରଚନା btf_kfunc_id_set scx_kfunc_set_select_cpu = {
	.owner			= THIS_MODULE,
	.set			= &scx_kfunc_ids_select_cpu,
	.filter			= scx_kfunc_context_filter,
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_idle_init(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_set_idle) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_TRACING, &scx_kfunc_set_idle) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_set_idle) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_set_select_cpu) ?:
	       register_btf_kfunc_id_set(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_set_select_cpu);
}
