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
#అన్తర్భావ "internal.h"
#అన్తర్భావ "cid.h"
#అన్తర్భావ "idle.h"
#అన్తర్భావ "sub.h"

/* Enable/disable built-in idle CPU selection policy */
స్థిర DEFINE_STATIC_కీలకం_FALSE(scx_builtin_idle_enabled);

/* Enable/disable per-node idle cpumasks */
స్థిర DEFINE_STATIC_కీలకం_FALSE(scx_builtin_idle_చొప్పున_కణుపు);

/* Enable/disable LLC aware optimizations */
స్థిర DEFINE_STATIC_కీలకం_FALSE(scx_selcpu_topo_llc);

/* Enable/disable NUMA aware optimizations */
స్థిర DEFINE_STATIC_కీలకం_FALSE(scx_selcpu_topo_numa);

/*
 * cpumasks to track idle CPUs within each NUMA node.
 *
 * If SCX_OPS_BUILTIN_IDLE_PER_NODE is not enabled, a single global cpumask
 * from is used to track all the idle CPUs in the system.
 */
నిర్మాణం scx_idle_cpus {
	cpumask_var_t cpu;
	cpumask_var_t smt;
};

/*
 * Global host-wide idle cpumasks (used when SCX_OPS_BUILTIN_IDLE_PER_NODE
 * is not enabled).
 */
స్థిర నిర్మాణం scx_idle_cpus scx_idle_global_masks;

/*
 * Per-node idle cpumasks.
 */
స్థిర నిర్మాణం scx_idle_cpus **scx_idle_కణుపు_masks;

/*
 * Local per-CPU cpumasks (used to generate temporary idle cpumasks).
 */
స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, local_idle_cpumask);
స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, local_llc_idle_cpumask);
స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, local_numa_idle_cpumask);

/*
 * Return the idle masks associated to a target @node.
 *
 * NUMA_NO_NODE identifies the global idle cpumask.
 */
స్థిర నిర్మాణం scx_idle_cpus *idle_cpumask(పూర్ణాంకం కణుపు)
{
	తిరిగి కణుపు == NUMA_NO_కణుపు ? &scx_idle_global_masks : scx_idle_కణుపు_masks[కణుపు];
}

/*
 * Returns the NUMA node ID associated with a @cpu, or NUMA_NO_NODE if
 * per-node idle cpumasks are disabled.
 */
స్థిర పూర్ణాంకం scx_cpu_కణుపు_if_enabled(పూర్ణాంకం cpu)
{
	ఒకవేళ (!static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_చొప్పున_కణుపు))
		తిరిగి NUMA_NO_కణుపు;

	తిరిగి cpu_గమ్యం_కణుపు(cpu);
}

స్థిర bool scx_idle_test_and_clear_cpu(పూర్ణాంకం cpu)
{
	పూర్ణాంకం కణుపు = scx_cpu_కణుపు_if_enabled(cpu);
	నిర్మాణం cpumask *idle_cpus = idle_cpumask(కణుపు)->cpu;

	/*
	 * SMT mask should be cleared whether we can claim @cpu or not. The SMT
	 * cluster is not wholly idle either way. This also prevents
	 * scx_pick_idle_cpu() from getting caught in an infinite loop.
	 */
	ఒకవేళ (sched_smt_చురుకైన()) {
		స్థిరరాశి నిర్మాణం cpumask *smt = cpu_smt_ముసుగు(cpu);
		నిర్మాణం cpumask *idle_smts = idle_cpumask(కణుపు)->smt;

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
		ఒకవేళ (cpumask_intersects(smt, idle_smts))
			cpumask_andnot(idle_smts, idle_smts, smt);
		లేకపోతే ఒకవేళ (cpumask_test_cpu(cpu, idle_smts))
			__cpumask_clear_cpu(cpu, idle_smts);
	}

	తిరిగి cpumask_test_and_clear_cpu(cpu, idle_cpus);
}

/*
 * Pick an idle CPU in a specific NUMA node.
 */
స్థిర s32 pick_idle_cpu_లోపల_కణుపు(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed, పూర్ణాంకం కణుపు, u64 flags)
{
	పూర్ణాంకం cpu;

retry:
	ఒకవేళ (sched_smt_చురుకైన()) {
		cpu = cpumask_any_and_distribute(idle_cpumask(కణుపు)->smt, cpus_allowed);
		ఒకవేళ (cpu < nr_cpu_ids)
			వెళ్ళు దొరికింది;

		ఒకవేళ (flags & SCX_PICK_IDLE_CORE)
			తిరిగి -EBUSY;
	}

	cpu = cpumask_any_and_distribute(idle_cpumask(కణుపు)->cpu, cpus_allowed);
	ఒకవేళ (cpu >= nr_cpu_ids)
		తిరిగి -EBUSY;

దొరికింది:
	ఒకవేళ (scx_idle_test_and_clear_cpu(cpu))
		తిరిగి cpu;
	లేకపోతే
		వెళ్ళు retry;
}

#నిర్వచితమైతే CONFIG_NUMA
/*
 * Tracks nodes that have not yet been visited when searching for an idle
 * CPU across all available nodes.
 */
స్థిర DEFINE_చొప్పున_CPU(nodemask_t, చొప్పున_cpu_unvisited);

/*
 * Search for an idle CPU across all nodes, excluding @node.
 */
స్థిర s32 pick_idle_cpu_from_online_nodes(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed, పూర్ణాంకం కణుపు, u64 flags)
{
	nodemask_t *unvisited;
	s32 cpu = -EBUSY;

	preempt_disable();
	unvisited = this_cpu_ptr(&చొప్పున_cpu_unvisited);

	/*
	 * Restrict the search to the online nodes (excluding the current
	 * node that has been visited already).
	 */
	nodes_copy(*unvisited, కణుపు_స్థితులు[N_ONLINE]);
	కణుపు_clear(కణుపు, *unvisited);

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
	for_each_కణుపు_numadist(కణుపు, *unvisited) {
		cpu = pick_idle_cpu_లోపల_కణుపు(cpus_allowed, కణుపు, flags);
		ఒకవేళ (cpu >= 0)
			ఆపు;
	}
	preempt_enable();

	తిరిగి cpu;
}
#లేకపోతే
స్థిర అంతఃవరుస s32
pick_idle_cpu_from_online_nodes(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed, పూర్ణాంకం కణుపు, u64 flags)
{
	తిరిగి -EBUSY;
}
#అయితే_ముగింపు

/*
 * Find an idle CPU in the system, starting from @node.
 */
స్థిర s32 scx_pick_idle_cpu(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed, పూర్ణాంకం కణుపు, u64 flags)
{
	s32 cpu;

	/*
	 * Always search in the starting node first (this is an
	 * optimization that can save some cycles even when the search is
	 * not limited to a single node).
	 */
	cpu = pick_idle_cpu_లోపల_కణుపు(cpus_allowed, కణుపు, flags);
	ఒకవేళ (cpu >= 0)
		తిరిగి cpu;

	/*
	 * Stop the search if we are using only a single global cpumask
	 * (NUMA_NO_NODE) or if the search is restricted to the first node
	 * only.
	 */
	ఒకవేళ (కణుపు == NUMA_NO_కణుపు || flags & SCX_PICK_IDLE_లోపల_కణుపు)
		తిరిగి -EBUSY;

	/*
	 * Extend the search to the other online nodes.
	 */
	తిరిగి pick_idle_cpu_from_online_nodes(cpus_allowed, కణుపు, flags);
}

/*
 * Return the amount of CPUs in the same LLC domain of @cpu (or zero if the LLC
 * domain is not defined).
 */
స్థిర గుర్తులేని పూర్ణాంకం llc_బరువు(s32 cpu)
{
	నిర్మాణం sched_domain *sd;

	sd = rcu_dereference(చొప్పున_cpu(sd_llc, cpu));
	ఒకవేళ (!sd)
		తిరిగి 0;

	తిరిగి sd->పరిధి_బరువు;
}

/*
 * Return the cpumask representing the LLC domain of @cpu (or NULL if the LLC
 * domain is not defined).
 */
స్థిర నిర్మాణం cpumask *llc_పరిధి(s32 cpu)
{
	నిర్మాణం sched_domain *sd;

	sd = rcu_dereference(చొప్పున_cpu(sd_llc, cpu));
	ఒకవేళ (!sd)
		తిరిగి ఖాళీ;

	తిరిగి sched_domain_పరిధి(sd);
}

/*
 * Return the amount of CPUs in the same NUMA domain of @cpu (or zero if the
 * NUMA domain is not defined).
 */
స్థిర గుర్తులేని పూర్ణాంకం numa_బరువు(s32 cpu)
{
	నిర్మాణం sched_domain *sd;
	నిర్మాణం sched_సమూహం *sg;

	sd = rcu_dereference(చొప్పున_cpu(sd_numa, cpu));
	ఒకవేళ (!sd)
		తిరిగి 0;
	sg = sd->groups;
	ఒకవేళ (!sg)
		తిరిగి 0;

	తిరిగి sg->సమూహం_బరువు;
}

/*
 * Return the cpumask representing the NUMA domain of @cpu (or NULL if the NUMA
 * domain is not defined).
 */
స్థిర నిర్మాణం cpumask *numa_పరిధి(s32 cpu)
{
	నిర్మాణం sched_domain *sd;
	నిర్మాణం sched_సమూహం *sg;

	sd = rcu_dereference(చొప్పున_cpu(sd_numa, cpu));
	ఒకవేళ (!sd)
		తిరిగి ఖాళీ;
	sg = sd->groups;
	ఒకవేళ (!sg)
		తిరిగి ఖాళీ;

	తిరిగి sched_సమూహం_పరిధి(sg);
}

/*
 * Return true if the LLC domains do not perfectly overlap with the NUMA
 * domains, false otherwise.
 */
స్థిర bool llc_numa_mismatch(శూన్య)
{
	పూర్ణాంకం cpu;

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
		ఒకవేళ (llc_బరువు(cpu) != numa_బరువు(cpu))
			తిరిగి true;

	తిరిగి false;
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
శూన్య scx_idle_update_selcpu_topology(నిర్మాణం sched_ext_ops *ops)
{
	bool enable_llc = false, enable_numa = false;
	గుర్తులేని పూర్ణాంకం nr_cpus;
	s32 cpu = cpumask_మొదటి(cpu_online_ముసుగు);

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
	rcu_read_తాళం();
	nr_cpus = llc_బరువు(cpu);
	ఒకవేళ (nr_cpus > 0) {
		ఒకవేళ (nr_cpus < అంకె_online_cpus())
			enable_llc = true;
		pr_debug("sched_ext: LLC=%*pb weight=%u\n",
			 cpumask_pr_వాదనలు(llc_పరిధి(cpu)), llc_బరువు(cpu));
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
	ఒకవేళ (!(ops->flags & SCX_OPS_BUILTIN_IDLE_చొప్పున_కణుపు)) {
		nr_cpus = numa_బరువు(cpu);
		ఒకవేళ (nr_cpus > 0) {
			ఒకవేళ (nr_cpus < అంకె_online_cpus() && llc_numa_mismatch())
				enable_numa = true;
			pr_debug("sched_ext: NUMA=%*pb weight=%u\n",
				 cpumask_pr_వాదనలు(numa_పరిధి(cpu)), nr_cpus);
		}
	}
	rcu_read_unlock();

	pr_debug("sched_ext: LLC idle selection %s\n",
		 str_enabled_disabled(enable_llc));
	pr_debug("sched_ext: NUMA idle selection %s\n",
		 str_enabled_disabled(enable_numa));

	ఒకవేళ (enable_llc)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_llc);
	లేకపోతే
		static_branch_disable_cpuslocked(&scx_selcpu_topo_llc);
	ఒకవేళ (enable_numa)
		static_branch_enable_cpuslocked(&scx_selcpu_topo_numa);
	లేకపోతే
		static_branch_disable_cpuslocked(&scx_selcpu_topo_numa);
}

/*
 * Return true if @p can run on all possible CPUs, false otherwise.
 */
స్థిర అంతఃవరుస bool task_affinity_all(స్థిరరాశి నిర్మాణం task_struct *p)
{
	తిరిగి p->nr_cpus_allowed >= అంకె_possible_cpus();
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
s32 scx_select_cpu_dfl(నిర్మాణం task_struct *p, s32 prev_cpu, u64 wake_flags,
		       స్థిరరాశి నిర్మాణం cpumask *cpus_allowed, u64 flags)
{
	స్థిరరాశి నిర్మాణం cpumask *llc_cpus = ఖాళీ, *numa_cpus = ఖాళీ;
	స్థిరరాశి నిర్మాణం cpumask *allowed = cpus_allowed ?: p->cpus_ptr;
	పూర్ణాంకం కణుపు = scx_cpu_కణుపు_if_enabled(prev_cpu);
	bool is_prev_allowed;
	s32 cpu;

	preempt_disable();

	/*
	 * Determine the subset of CPUs usable by @p within @cpus_allowed.
	 */
	ఒకవేళ (allowed != p->cpus_ptr) {
		నిర్మాణం cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_idle_cpumask);

		ఒకవేళ (task_affinity_all(p)) {
			allowed = cpus_allowed;
		} లేకపోతే ఒకవేళ (cpumask_and(local_cpus, cpus_allowed, p->cpus_ptr)) {
			allowed = local_cpus;
		} లేకపోతే {
			cpu = -EBUSY;
			వెళ్ళు బహిర్గతం_enable;
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
	rcu_read_తాళం();

	/*
	 * Determine the subset of CPUs that the task can use in its
	 * current LLC and node.
	 *
	 * If the task can run on all CPUs, use the node and LLC cpumasks
	 * directly.
	 */
	ఒకవేళ (static_branch_maybe(CONFIG_NUMA, &scx_selcpu_topo_numa)) {
		నిర్మాణం cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_numa_idle_cpumask);
		స్థిరరాశి నిర్మాణం cpumask *cpus = numa_పరిధి(prev_cpu);

		ఒకవేళ (allowed == p->cpus_ptr && task_affinity_all(p))
			numa_cpus = cpus;
		లేకపోతే ఒకవేళ (cpus && cpumask_and(local_cpus, allowed, cpus))
			numa_cpus = local_cpus;
	}

	ఒకవేళ (static_branch_maybe(CONFIG_SCHED_MC, &scx_selcpu_topo_llc)) {
		నిర్మాణం cpumask *local_cpus = this_cpu_cpumask_var_ptr(local_llc_idle_cpumask);
		స్థిరరాశి నిర్మాణం cpumask *cpus = llc_పరిధి(prev_cpu);

		ఒకవేళ (allowed == p->cpus_ptr && task_affinity_all(p))
			llc_cpus = cpus;
		లేకపోతే ఒకవేళ (cpus && cpumask_and(local_cpus, allowed, cpus))
			llc_cpus = local_cpus;
	}

	/*
	 * If WAKE_SYNC, try to migrate the wakee to the waker's CPU.
	 */
	ఒకవేళ (wake_flags & SCX_WAKE_SYNC) {
		పూర్ణాంకం waker_కణుపు;

		/*
		 * If the waker's CPU is cache affine and prev_cpu is idle,
		 * then avoid a migration.
		 */
		cpu = smp_processor_id();
		ఒకవేళ (is_prev_allowed && cpus_share_cache(cpu, prev_cpu) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			వెళ్ళు బహిర్గతం_unlock;
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
		waker_కణుపు = scx_cpu_కణుపు_if_enabled(cpu);
		ఒకవేళ (!(current->flags & PF_EXITING) &&
		    cpu_rq(cpu)->scx.local_dsq.nr == 0 &&
		    (!(flags & SCX_PICK_IDLE_లోపల_కణుపు) || (waker_కణుపు == కణుపు)) &&
		    !cpumask_ఖాళీలు(idle_cpumask(waker_కణుపు)->cpu)) {
			ఒకవేళ (cpumask_test_cpu(cpu, allowed)) {
				scx_idle_test_and_clear_cpu(cpu);
				వెళ్ళు బహిర్గతం_unlock;
			}
		}
	}

	/*
	 * If CPU has SMT, any wholly idle CPU is likely a better pick than
	 * partially idle @prev_cpu.
	 */
	ఒకవేళ (sched_smt_చురుకైన()) {
		/*
		 * Keep using @prev_cpu if it's part of a fully idle core.
		 */
		ఒకవేళ (is_prev_allowed &&
		    cpumask_test_cpu(prev_cpu, idle_cpumask(కణుపు)->smt) &&
		    scx_idle_test_and_clear_cpu(prev_cpu)) {
			cpu = prev_cpu;
			వెళ్ళు బహిర్గతం_unlock;
		}

		/*
		 * Search for any fully idle core in the same LLC domain.
		 */
		ఒకవేళ (llc_cpus) {
			cpu = pick_idle_cpu_లోపల_కణుపు(llc_cpus, కణుపు, SCX_PICK_IDLE_CORE);
			ఒకవేళ (cpu >= 0)
				వెళ్ళు బహిర్గతం_unlock;
		}

		/*
		 * Search for any fully idle core in the same NUMA node.
		 */
		ఒకవేళ (numa_cpus) {
			cpu = pick_idle_cpu_లోపల_కణుపు(numa_cpus, కణుపు, SCX_PICK_IDLE_CORE);
			ఒకవేళ (cpu >= 0)
				వెళ్ళు బహిర్గతం_unlock;
		}

		/*
		 * Search for any full-idle core usable by the task.
		 *
		 * If the node-aware idle CPU selection policy is enabled
		 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always
		 * begin in prev_cpu's node and proceed to other nodes in
		 * order of increasing distance.
		 */
		cpu = scx_pick_idle_cpu(allowed, కణుపు, flags | SCX_PICK_IDLE_CORE);
		ఒకవేళ (cpu >= 0)
			వెళ్ళు బహిర్గతం_unlock;

		/*
		 * Give up if we're strictly looking for a full-idle SMT
		 * core.
		 */
		ఒకవేళ (flags & SCX_PICK_IDLE_CORE) {
			cpu = -EBUSY;
			వెళ్ళు బహిర్గతం_unlock;
		}
	}

	/*
	 * Use @prev_cpu if it's idle.
	 */
	ఒకవేళ (is_prev_allowed && scx_idle_test_and_clear_cpu(prev_cpu)) {
		cpu = prev_cpu;
		వెళ్ళు బహిర్గతం_unlock;
	}

	/*
	 * Use @prev_cpu's sibling if it's idle.
	 */
	ఒకవేళ (sched_smt_చురుకైన()) {
		for_each_cpu_and(cpu, cpu_smt_ముసుగు(prev_cpu), allowed) {
			ఒకవేళ (cpu == prev_cpu)
				కొనసాగు;
			ఒకవేళ (scx_idle_test_and_clear_cpu(cpu))
				వెళ్ళు బహిర్గతం_unlock;
		}
	}

	/*
	 * Search for any idle CPU in the same LLC domain.
	 */
	ఒకవేళ (llc_cpus) {
		cpu = pick_idle_cpu_లోపల_కణుపు(llc_cpus, కణుపు, 0);
		ఒకవేళ (cpu >= 0)
			వెళ్ళు బహిర్గతం_unlock;
	}

	/*
	 * Search for any idle CPU in the same NUMA node.
	 */
	ఒకవేళ (numa_cpus) {
		cpu = pick_idle_cpu_లోపల_కణుపు(numa_cpus, కణుపు, 0);
		ఒకవేళ (cpu >= 0)
			వెళ్ళు బహిర్గతం_unlock;
	}

	/*
	 * Search for any idle CPU usable by the task.
	 *
	 * If the node-aware idle CPU selection policy is enabled
	 * (%SCX_OPS_BUILTIN_IDLE_PER_NODE), the search will always begin
	 * in prev_cpu's node and proceed to other nodes in order of
	 * increasing distance.
	 */
	cpu = scx_pick_idle_cpu(allowed, కణుపు, flags);

బహిర్గతం_unlock:
	rcu_read_unlock();
బహిర్గతం_enable:
	preempt_enable();

	తిరిగి cpu;
}

/*
 * Initialize global and per-node idle cpumasks.
 */
శూన్య scx_idle_init_masks(శూన్య)
{
	పూర్ణాంకం i;

	/* Allocate global idle cpumasks */
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.cpu, GFP_KERNEL));
	BUG_ON(!alloc_cpumask_var(&scx_idle_global_masks.smt, GFP_KERNEL));

	/* Allocate per-node idle cpumasks (use nr_node_ids for non-contiguous NUMA nodes) */
	scx_idle_కణుపు_masks = kzalloc_objs(*scx_idle_కణుపు_masks, nr_కణుపు_ids);
	BUG_ON(!scx_idle_కణుపు_masks);

	for_each_కణుపు(i) {
		scx_idle_కణుపు_masks[i] = kzalloc_కణుపు(పరిమాణం(**scx_idle_కణుపు_masks),
							 GFP_KERNEL, i);
		BUG_ON(!scx_idle_కణుపు_masks[i]);

		BUG_ON(!alloc_cpumask_var_కణుపు(&scx_idle_కణుపు_masks[i]->cpu, GFP_KERNEL, i));
		BUG_ON(!alloc_cpumask_var_కణుపు(&scx_idle_కణుపు_masks[i]->smt, GFP_KERNEL, i));
	}

	/* Allocate local per-cpu idle cpumasks */
	for_each_possible_cpu(i) {
		BUG_ON(!alloc_cpumask_var_కణుపు(&చొప్పున_cpu(local_idle_cpumask, i),
					       GFP_KERNEL, cpu_గమ్యం_కణుపు(i)));
		BUG_ON(!alloc_cpumask_var_కణుపు(&చొప్పున_cpu(local_llc_idle_cpumask, i),
					       GFP_KERNEL, cpu_గమ్యం_కణుపు(i)));
		BUG_ON(!alloc_cpumask_var_కణుపు(&చొప్పున_cpu(local_numa_idle_cpumask, i),
					       GFP_KERNEL, cpu_గమ్యం_కణుపు(i)));
	}
}

స్థిర శూన్య update_builtin_idle(పూర్ణాంకం cpu, bool idle)
{
	పూర్ణాంకం కణుపు = scx_cpu_కణుపు_if_enabled(cpu);
	నిర్మాణం cpumask *idle_cpus = idle_cpumask(కణుపు)->cpu;

	కేటాయించు_cpu(cpu, idle_cpus, idle);

	ఒకవేళ (sched_smt_చురుకైన()) {
		స్థిరరాశి నిర్మాణం cpumask *smt = cpu_smt_ముసుగు(cpu);
		నిర్మాణం cpumask *idle_smts = idle_cpumask(కణుపు)->smt;

		ఒకవేళ (idle) {
			/*
			 * idle_smt handling is racy but that's fine as it's
			 * only for optimization and self-correcting.
			 */
			ఒకవేళ (!cpumask_subset(smt, idle_cpus))
				తిరిగి;
			cpumask_or(idle_smts, idle_smts, smt);
		} లేకపోతే {
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
స్థిర శూన్య scx_idle_notify(నిర్మాణం rq *rq, bool idle, bool do_notify, bool మూలం_renotify)
{
	s32 cpu = cpu_యొక్క(rq);
	s32 cid = scx_cpu_arg(cpu);
	నిర్మాణం scx_sched *మూలం = scx_మూలం_protected_సజీవ();
	నిర్మాణం scx_sched *pos;

	lockdep_assert_rq_held(rq);

	/* with no sub-sched, only the root can be owed a notification */
	ఒకవేళ (!scx_has_subs()) {
		ఒకవేళ ((do_notify || మూలం_renotify) &&
		    SCX_HAS_OP(మూలం, update_idle) && !scx_bypassing(మూలం, cpu))
			SCX_CALL_OP(మూలం, update_idle, rq, cid, idle);
		తిరిగి;
	}

	pos = scx_తదుపరి_descendant_pre(ఖాళీ, మూలం);
	అయితే (pos) {
		bool forced = false;

		ఒకవేళ (unlikely(scx_missing_caps(pos, cpu, SCX_CAP_పునాది))) {
			pos = scx_skip_subtree_pre(pos, మూలం);
			కొనసాగు;
		}

		ఒకవేళ (!pos->level) {
			forced = మూలం_renotify;
		}
#నిర్వచితమైతే CONFIG_EXT_SUB_SCHED
		లేకపోతే ఒకవేళ (చొప్పున_cpu_ptr(pos->pcpu, cpu)->idle_renotify) {
			చొప్పున_cpu_ptr(pos->pcpu, cpu)->idle_renotify = false;
			forced = true;
		}
#అయితే_ముగింపు
		ఒకవేళ ((do_notify || forced) && SCX_HAS_OP(pos, update_idle) &&
		    !scx_bypassing(pos, cpu))
			SCX_CALL_OP(pos, update_idle, rq, cid, idle);
		pos = scx_తదుపరి_descendant_pre(pos, మూలం);
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
శూన్య __scx_update_idle(నిర్మాణం rq *rq, bool idle, bool do_notify)
{
	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	lockdep_assert_rq_held(rq);

	/*
	 * pick_task_idle() calls here only on an idle-to-idle re-pick and the
	 * transitions call with @do_notify, so every reaching call updates the
	 * masks.
	 */
	ఒకవేళ (static_branch_likely(&scx_builtin_idle_enabled))
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
	ఒకవేళ (do_notify ||
	    (idle && (rq->scx.flags &
		      (SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_మూలం_IDLE_RENOTIFY)))) {
		bool మూలం_renotify = rq->scx.flags & SCX_RQ_మూలం_IDLE_RENOTIFY;

		rq->scx.flags &= ~(SCX_RQ_SUB_IDLE_RENOTIFY | SCX_RQ_మూలం_IDLE_RENOTIFY);
		scx_idle_notify(rq, idle, do_notify, మూలం_renotify);
	}
}

స్థిర శూన్య reset_idle_masks(నిర్మాణం sched_ext_ops *ops)
{
	పూర్ణాంకం కణుపు;

	/*
	 * Start with all CPUs marked busy. The idle masks are populated when
	 * bypass is lifted and each idle CPU is forced through an idle re-pick.
	 * This may temporarily omit idle CPUs but never advertises a busy CPU as
	 * idle.
	 */
	ఒకవేళ (!(ops->flags & SCX_OPS_BUILTIN_IDLE_చొప్పున_కణుపు)) {
		cpumask_clear(idle_cpumask(NUMA_NO_కణుపు)->cpu);
		cpumask_clear(idle_cpumask(NUMA_NO_కణుపు)->smt);
		తిరిగి;
	}

	for_each_కణుపు(కణుపు) {
		cpumask_clear(idle_cpumask(కణుపు)->cpu);
		cpumask_clear(idle_cpumask(కణుపు)->smt);
	}
}

శూన్య scx_idle_enable(నిర్మాణం sched_ext_ops *ops)
{
	ఒకవేళ (!ops->update_idle || (ops->flags & SCX_OPS_KEEP_BUILTIN_IDLE))
		static_branch_enable_cpuslocked(&scx_builtin_idle_enabled);
	లేకపోతే
		static_branch_disable_cpuslocked(&scx_builtin_idle_enabled);

	ఒకవేళ (ops->flags & SCX_OPS_BUILTIN_IDLE_చొప్పున_కణుపు)
		static_branch_enable_cpuslocked(&scx_builtin_idle_చొప్పున_కణుపు);
	లేకపోతే
		static_branch_disable_cpuslocked(&scx_builtin_idle_చొప్పున_కణుపు);

	reset_idle_masks(ops);
}

శూన్య scx_idle_disable(శూన్య)
{
	static_branch_disable(&scx_builtin_idle_enabled);
	static_branch_disable(&scx_builtin_idle_చొప్పున_కణుపు);
}

/********************************************************************************
 * Helpers that can be called from the BPF scheduler.
 */

స్థిర పూర్ణాంకం validate_కణుపు(నిర్మాణం scx_sched *sch, పూర్ణాంకం కణుపు)
{
	ఒకవేళ (!static_branch_likely(&scx_builtin_idle_చొప్పున_కణుపు)) {
		scx_error(sch, "per-node idle tracking is disabled");
		తిరిగి -EOPNOTSUPP;
	}

	/* Return no entry for NUMA_NO_NODE (not a critical scx error) */
	ఒకవేళ (కణుపు == NUMA_NO_కణుపు)
		తిరిగి -ENOENT;

	/* Make sure node is in a valid range */
	ఒకవేళ (కణుపు < 0 || కణుపు >= nr_కణుపు_ids) {
		scx_error(sch, "invalid node %d", కణుపు);
		తిరిగి -EINVAL;
	}

	/* Make sure the node is part of the set of possible nodes */
	ఒకవేళ (!కణుపు_possible(కణుపు)) {
		scx_error(sch, "unavailable node %d", కణుపు);
		తిరిగి -EINVAL;
	}

	తిరిగి కణుపు;
}

__bpf_kfunc_start_defs();

స్థిర bool check_builtin_idle_enabled(నిర్మాణం scx_sched *sch)
{
	ఒకవేళ (static_branch_likely(&scx_builtin_idle_enabled))
		తిరిగి true;

	scx_error(sch, "built-in idle tracking is disabled");
	తిరిగి false;
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
స్థిర bool is_bpf_migration_disabled(స్థిరరాశి నిర్మాణం task_struct *p)
{
	ఒకవేళ (p->migration_disabled == 1) {
		ఒకవేళ (IS_ENABLED(CONFIG_PREEMPT_RCU))
			తిరిగి p != current;
		తిరిగి true;
	}
	తిరిగి p->migration_disabled;
}

స్థిర s32 select_cpu_restricted(నిర్మాణం task_struct *p, s32 prev_cpu,
				 స్థిరరాశి నిర్మాణం cpumask *allowed, u64 flags,
				 bool migration_disabled)
{
	s32 cpu;

	ఒకవేళ (migration_disabled)
		cpu = task_cpu(p);
	లేకపోతే
		cpu = cpumask_మొదటి(p->cpus_ptr);

	ఒకవేళ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		తిరిగి -EBUSY;

	ఒకవేళ (allowed && !cpumask_test_cpu(cpu, allowed))
		తిరిగి -EBUSY;

	ఒకవేళ (flags & SCX_PICK_IDLE_లోపల_కణుపు) {
		పూర్ణాంకం కణుపు = scx_cpu_కణుపు_if_enabled(prev_cpu);

		ఒకవేళ (scx_cpu_కణుపు_if_enabled(cpu) != కణుపు)
			తిరిగి -EBUSY;
	}

	ఒకవేళ (!scx_idle_test_and_clear_cpu(cpu))
		తిరిగి -EBUSY;

	తిరిగి cpu;
}

స్థిర s32 select_cpu_from_kfunc(నిర్మాణం scx_sched *sch, నిర్మాణం task_struct *p,
				 s32 prev_cpu, u64 wake_flags,
				 స్థిరరాశి నిర్మాణం cpumask *allowed, u64 flags)
{
	గుర్తులేని దీర్ఘ irq_flags;
	bool we_locked = false;
	bool migration_disabled;
	s32 cpu;

	ఒకవేళ (!scx_cpu_valid(sch, prev_cpu, ఖాళీ))
		తిరిగి -EINVAL;

	ఒకవేళ (!check_builtin_idle_enabled(sch))
		తిరిగి -EBUSY;

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
	ఒకవేళ (this_rq()->scx.లోపల_select_cpu) {
		ఒకవేళ (!scx_kf_arg_task_ok(sch, p))
			తిరిగి -EINVAL;
		lockdep_assert_held(&p->pi_తాళం);
	} లేకపోతే ఒకవేళ (scx_locked_rq()) {
		ఒకవేళ (task_rq(p) != scx_locked_rq())
			వెళ్ళు cross_task;
	} లేకపోతే {
		raw_spin_తాళం_irqsave(&p->pi_తాళం, irq_flags);
		we_locked = true;
	}

	/*
	 * This may also be called from ops.enqueue(), so we need to handle
	 * per-CPU tasks as well. For these tasks, skip general idle CPU
	 * selection and validate the task's only usable CPU directly.
	 */
	migration_disabled = is_bpf_migration_disabled(p);

	ఒకవేళ (p->nr_cpus_allowed == 1 || migration_disabled)
		cpu = select_cpu_restricted(p, prev_cpu, allowed, flags,
					    migration_disabled);
	లేకపోతే
		cpu = scx_select_cpu_dfl(p, prev_cpu, wake_flags,
					 allowed ?: p->cpus_ptr, flags);

	ఒకవేళ (we_locked)
		raw_spin_unlock_irqrestore(&p->pi_తాళం, irq_flags);

	తిరిగి cpu;

cross_task:
	scx_error(sch, "select_cpu kfunc called cross-task on %s[%d]",
		  p->comm, p->pid);
	తిరిగి -EINVAL;
}

/**
 * scx_bpf_cpu_node - Return the NUMA node the given @cpu belongs to, or
 *		      trigger an error if @cpu is invalid
 * @cpu: target CPU
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 */
__bpf_kfunc s32 scx_bpf_cpu_కణుపు(s32 cpu, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch) || !scx_cpu_valid(sch, cpu, ఖాళీ))
		తిరిగి NUMA_NO_కణుపు;
	తిరిగి cpu_గమ్యం_కణుపు(cpu);
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
__bpf_kfunc s32 scx_bpf_select_cpu_dfl(నిర్మాణం task_struct *p, s32 prev_cpu,
				       u64 wake_flags, bool *is_idle,
				       స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

	cpu = select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags, ఖాళీ, 0);
	ఒకవేళ (cpu >= 0) {
		*is_idle = true;
		తిరిగి cpu;
	}
	*is_idle = false;
	తిరిగి prev_cpu;
}

నిర్మాణం scx_bpf_select_cpu_and_వాదనలు {
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
__scx_bpf_select_cpu_and(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం cpumask *cpus_allowed,
			 నిర్మాణం scx_bpf_select_cpu_and_వాదనలు *వాదనలు,
			 స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

	తిరిగి select_cpu_from_kfunc(sch, p, వాదనలు->prev_cpu, వాదనలు->wake_flags,
				     cpus_allowed, వాదనలు->flags);
}

/*
 * COMPAT: Will be removed in v6.22.
 */
__bpf_kfunc s32 scx_bpf_select_cpu_and(నిర్మాణం task_struct *p, s32 prev_cpu, u64 wake_flags,
				       స్థిరరాశి నిర్మాణం cpumask *cpus_allowed, u64 flags)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = rcu_dereference(scx_మూలం);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

#నిర్వచితమైతే CONFIG_EXT_SUB_SCHED
	/*
	 * Disallow if any sub-scheds are attached. There is no way to tell
	 * which scheduler called us, so error out @p's scheduler -- read it
	 * under RCU as @p's locks aren't necessarily held here. @p may be a
	 * task past sched_ext_dead() or an idle task, in which case its
	 * scheduler can't be determined and there is nothing obviously wrong
	 * to report; just refuse the call.
	 */
	ఒకవేళ (unlikely(!list_ఖాళీలు(&sch->children))) {
		నిర్మాణం scx_sched *tsch = scx_task_sched_rcu(p);

		ఒకవేళ (tsch)
			scx_error(tsch, "__scx_bpf_select_cpu_and() must be used");
		తిరిగి -EINVAL;
	}
#అయితే_ముగింపు

	తిరిగి select_cpu_from_kfunc(sch, p, prev_cpu, wake_flags,
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
__bpf_kfunc స్థిరరాశి నిర్మాణం cpumask *
scx_bpf_get_idle_cpumask_కణుపు(s32 కణుపు, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి cpu_none_ముసుగు;

	కణుపు = validate_కణుపు(sch, కణుపు);
	ఒకవేళ (కణుపు < 0)
		తిరిగి cpu_none_ముసుగు;

	తిరిగి idle_cpumask(కణుపు)->cpu;
}

/**
 * scx_bpf_get_idle_cpumask - Get a referenced kptr to the idle-tracking
 * per-CPU cpumask.
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Returns an empty mask if idle tracking is not enabled, or running on a
 * UP kernel.
 */
__bpf_kfunc స్థిరరాశి నిర్మాణం cpumask *scx_bpf_get_idle_cpumask(స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి cpu_none_ముసుగు;

	ఒకవేళ (static_branch_unlikely(&scx_builtin_idle_చొప్పున_కణుపు)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		తిరిగి cpu_none_ముసుగు;
	}

	ఒకవేళ (!check_builtin_idle_enabled(sch))
		తిరిగి cpu_none_ముసుగు;

	తిరిగి idle_cpumask(NUMA_NO_కణుపు)->cpu;
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
__bpf_kfunc స్థిరరాశి నిర్మాణం cpumask *
scx_bpf_get_idle_smtmask_కణుపు(s32 కణుపు, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి cpu_none_ముసుగు;

	కణుపు = validate_కణుపు(sch, కణుపు);
	ఒకవేళ (కణుపు < 0)
		తిరిగి cpu_none_ముసుగు;

	ఒకవేళ (sched_smt_చురుకైన())
		తిరిగి idle_cpumask(కణుపు)->smt;
	లేకపోతే
		తిరిగి idle_cpumask(కణుపు)->cpu;
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
__bpf_kfunc స్థిరరాశి నిర్మాణం cpumask *scx_bpf_get_idle_smtmask(స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి cpu_none_ముసుగు;

	ఒకవేళ (static_branch_unlikely(&scx_builtin_idle_చొప్పున_కణుపు)) {
		scx_error(sch, "SCX_OPS_BUILTIN_IDLE_PER_NODE enabled");
		తిరిగి cpu_none_ముసుగు;
	}

	ఒకవేళ (!check_builtin_idle_enabled(sch))
		తిరిగి cpu_none_ముసుగు;

	ఒకవేళ (sched_smt_చురుకైన())
		తిరిగి idle_cpumask(NUMA_NO_కణుపు)->smt;
	లేకపోతే
		తిరిగి idle_cpumask(NUMA_NO_కణుపు)->cpu;
}

/**
 * scx_bpf_put_idle_cpumask - Release a previously acquired referenced kptr to
 * either the percpu, or SMT idle-tracking cpumask.
 * @idle_mask: &cpumask to use
 */
__bpf_kfunc శూన్య scx_bpf_put_idle_cpumask(స్థిరరాశి నిర్మాణం cpumask *idle_ముసుగు)
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
__bpf_kfunc bool scx_bpf_test_and_clear_cpu_idle(s32 cpu, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి false;

	ఒకవేళ (!check_builtin_idle_enabled(sch))
		తిరిగి false;

	ఒకవేళ (!scx_cpu_valid(sch, cpu, ఖాళీ))
		తిరిగి false;

	తిరిగి scx_idle_test_and_clear_cpu(cpu);
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
__bpf_kfunc s32 scx_bpf_pick_idle_cpu_కణుపు(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed,
					   s32 కణుపు, u64 flags,
					   స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

	కణుపు = validate_కణుపు(sch, కణుపు);
	ఒకవేళ (కణుపు < 0)
		తిరిగి కణుపు;

	తిరిగి scx_pick_idle_cpu(cpus_allowed, కణుపు, flags);
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
__bpf_kfunc s32 scx_bpf_pick_idle_cpu(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed,
				      u64 flags, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

	ఒకవేళ (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_చొప్పున_కణుపు)) {
		scx_error(sch, "per-node idle tracking is enabled");
		తిరిగి -EBUSY;
	}

	ఒకవేళ (!check_builtin_idle_enabled(sch))
		తిరిగి -EBUSY;

	తిరిగి scx_pick_idle_cpu(cpus_allowed, NUMA_NO_కణుపు, flags);
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
__bpf_kfunc s32 scx_bpf_pick_any_cpu_కణుపు(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed,
					  s32 కణుపు, u64 flags,
					  స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

	కణుపు = validate_కణుపు(sch, కణుపు);
	ఒకవేళ (కణుపు < 0)
		తిరిగి కణుపు;

	cpu = scx_pick_idle_cpu(cpus_allowed, కణుపు, flags);
	ఒకవేళ (cpu >= 0)
		తిరిగి cpu;

	ఒకవేళ (flags & SCX_PICK_IDLE_లోపల_కణుపు)
		cpu = cpumask_any_and_distribute(cpumask_యొక్క_కణుపు(కణుపు), cpus_allowed);
	లేకపోతే
		cpu = cpumask_any_distribute(cpus_allowed);
	ఒకవేళ (cpu < nr_cpu_ids)
		తిరిగి cpu;
	లేకపోతే
		తిరిగి -EBUSY;
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
__bpf_kfunc s32 scx_bpf_pick_any_cpu(స్థిరరాశి నిర్మాణం cpumask *cpus_allowed,
				     u64 flags, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;
	s32 cpu;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -ENODEV;

	ఒకవేళ (static_branch_maybe(CONFIG_NUMA, &scx_builtin_idle_చొప్పున_కణుపు)) {
		scx_error(sch, "per-node idle tracking is enabled");
		తిరిగి -EBUSY;
	}

	ఒకవేళ (static_branch_likely(&scx_builtin_idle_enabled)) {
		cpu = scx_pick_idle_cpu(cpus_allowed, NUMA_NO_కణుపు, flags);
		ఒకవేళ (cpu >= 0)
			తిరిగి cpu;
	}

	cpu = cpumask_any_distribute(cpus_allowed);
	ఒకవేళ (cpu < nr_cpu_ids)
		తిరిగి cpu;
	లేకపోతే
		తిరిగి -EBUSY;
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ప్రారంభం(scx_kfunc_ids_idle)
BTF_ID_FLAGS(func, scx_bpf_cpu_కణుపు, KF_IMPLICIT_వాదనలు)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask_కణుపు, KF_IMPLICIT_వాదనలు | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_cpumask, KF_IMPLICIT_వాదనలు | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask_కణుపు, KF_IMPLICIT_వాదనలు | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_get_idle_smtmask, KF_IMPLICIT_వాదనలు | KF_ACQUIRE)
BTF_ID_FLAGS(func, scx_bpf_put_idle_cpumask, KF_RELEASE)
BTF_ID_FLAGS(func, scx_bpf_test_and_clear_cpu_idle, KF_IMPLICIT_వాదనలు)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu_కణుపు, KF_IMPLICIT_వాదనలు | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_idle_cpu, KF_IMPLICIT_వాదనలు | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu_కణుపు, KF_IMPLICIT_వాదనలు | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_pick_any_cpu, KF_IMPLICIT_వాదనలు | KF_RCU)
BTF_KFUNCS_చివర(scx_kfunc_ids_idle)

స్థిర స్థిరరాశి నిర్మాణం btf_kfunc_id_సమితి scx_kfunc_సమితి_idle = {
	.owner			= THIS_MODULE,
	.సమితి			= &scx_kfunc_ids_idle,
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
BTF_KFUNCS_ప్రారంభం(scx_kfunc_ids_select_cpu)
BTF_ID_FLAGS(func, __scx_bpf_select_cpu_and, KF_IMPLICIT_వాదనలు | KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_and, KF_RCU)
BTF_ID_FLAGS(func, scx_bpf_select_cpu_dfl, KF_IMPLICIT_వాదనలు | KF_RCU)
BTF_KFUNCS_చివర(scx_kfunc_ids_select_cpu)

స్థిర స్థిరరాశి నిర్మాణం btf_kfunc_id_సమితి scx_kfunc_సమితి_select_cpu = {
	.owner			= THIS_MODULE,
	.సమితి			= &scx_kfunc_ids_select_cpu,
	.filter			= scx_kfunc_context_filter,
};

పూర్ణాంకం scx_idle_init(శూన్య)
{
	తిరిగి రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_సమితి_idle) ?:
	       రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_TRACING, &scx_kfunc_సమితి_idle) ?:
	       రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_సమితి_idle) ?:
	       రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_సమితి_select_cpu) ?:
	       రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_సమితి_select_cpu);
}
