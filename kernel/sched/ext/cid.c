/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ಅನ್ತರ್ಭಾವ <linux/cacheinfo.h>

#ಅನ್ತರ್ಭಾವ "internal.h"
#ಅನ್ತರ್ಭಾವ "cid.h"

/*
 * cid tables. The cid kfuncs are available whether the root scheduler is
 * cid-form or cpu-form, the latter to allow gradual migration to cids, so every
 * root builds a default mapping. Each root enable allocates a fresh set, builds
 * it privately and publishes the __rcu globals below once the layout is final.
 * Root disable unpublishes and RCU-frees the set. kfuncs may run before the
 * tables are published and must check for NULL.
 */
u32 scx_nr_cid_shards;
s16 __rcu *scx_cid_ಗಮ್ಯ_cpu_tbl;
s16 __rcu *scx_cpu_ಗಮ್ಯ_cid_tbl;
s32 __rcu *scx_cid_ಗಮ್ಯ_shard;
s32 __rcu *scx_shard_ಗಂಟು;
ರಚನೆ scx_cid_shard __rcu *scx_cid_shard_ranges;
ರಚನೆ scx_cid_topo __rcu *scx_cid_topo;

ಸ್ಥಿರ ರಚನೆ scx_cid_tables *scx_cid_tables;	/* used only during alloc/free */

#ವ್ಯಾಖ್ಯಾನಿಸು SCX_CID_TOPO_NEG	(ರಚನೆ scx_cid_topo) {				\
	.core_cid = -1, .core_idx = -1, .llc_cid = -1, .llc_idx = -1,		\
	.ಗಂಟು_cid = -1, .ಗಂಟು_idx = -1, .shard_cid = -1, .shard_idx = -1,	\
}

/*
 * Return @cpu's LLC shared_cpu_map. If cacheinfo isn't populated (offline or
 * !present), record @cpu in @fallbacks and return its node mask instead - the
 * worst that can happen is that the cpu's LLC becomes coarser than reality.
 */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_llc_ಮುಖವಾಡ(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ cpumask *fallbacks)
{
	ರಚನೆ cpu_cacheinfo *ci = get_cpu_cacheinfo(cpu);

	ಆದರೆ (!ci || !ci->info_list || !ci->ಅಂಕೆ_leaves) {
		cpumask_ಗಣ_cpu(cpu, fallbacks);
		ಹಿಂತಿರುಗಿ cpumask_ದ_ಗಂಟು(cpu_ಗಮ್ಯ_ಗಂಟು(cpu));
	}
	ಹಿಂತಿರುಗಿ &ci->info_list[ci->ಅಂಕೆ_leaves - 1].shared_cpu_map;
}

/*
 * Compute per-LLC shard layout. Each shard holds at most @shard_size cids, and
 * in any case no more than SCX_CID_SHARD_MAX_CPUS. Cores are spread as evenly
 * as possible across shards so cpu count is balanced: the first *@nr_large_p
 * shards get (*@cores_per_shard_p + 1) cores, the rest get *@cores_per_shard_p.
 */
ಸ್ಥಿರ ಶೂನ್ಯ calc_shard_layout(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *llc_cpus, u32 shard_ಗಾತ್ರ,
			      u32 *cores_ಪ್ರತಿ_shard_p, u32 *nr_large_p)
{
	u32 nr_cores = 0, nr_cpus = 0, nr_shards;
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_cpu(cpu, llc_cpus) {
		nr_cpus++;
		ಆದರೆ (cpumask_ಮೊದಲ(topology_sibling_cpumask(cpu)) == cpu)
			nr_cores++;
	}

	nr_shards = max_t(u32, 1, DIV_ಸುತ್ತು_ಮೇಲೆ(nr_cpus, shard_ಗಾತ್ರ));
	nr_shards = max_t(u32, nr_shards,
			  DIV_ಸುತ್ತು_ಮೇಲೆ(nr_cpus, SCX_CID_SHARD_MAX_CPUS));

	*cores_ಪ್ರತಿ_shard_p = nr_cores / nr_shards;
	*nr_large_p = nr_cores % nr_shards;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_cid_tables_free(ರಚನೆ scx_cid_tables *tbls)
{
	ಆದರೆ (!tbls)
		ಹಿಂತಿರುಗಿ;
	kvfree(tbls->cid_ಗಮ್ಯ_cpu);
	kvfree(tbls->cpu_ಗಮ್ಯ_cid);
	kvfree(tbls->cid_ಗಮ್ಯ_shard);
	kvfree(tbls->shard_ಗಂಟು);
	kvfree(tbls->shard_ranges);
	kvfree(tbls->topo);
	kfree(tbls);
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_cid_tables_free_rcufn(ರಚನೆ rcu_ತಲೆ *rcu)
{
	scx_cid_tables_free(container_ದ(rcu, ರಚನೆ scx_cid_tables, rcu));
}

ಸ್ಥಿರ ರಚನೆ scx_cid_tables *scx_cid_alloc_tables(ಶೂನ್ಯ)
{
	u32 npossible = ಅಂಕೆ_possible_cpus();
	ರಚನೆ scx_cid_tables *tbls;

	tbls = kzalloc_obj(*tbls);
	ಆದರೆ (!tbls)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	tbls->cid_ಗಮ್ಯ_cpu = kvzalloc_objs(*tbls->cid_ಗಮ್ಯ_cpu, npossible);
	tbls->cpu_ಗಮ್ಯ_cid = kvzalloc_objs(*tbls->cpu_ಗಮ್ಯ_cid, nr_cpu_ids);
	tbls->cid_ಗಮ್ಯ_shard = kvzalloc_objs(*tbls->cid_ಗಮ್ಯ_shard, npossible);
	tbls->shard_ಗಂಟು = kvzalloc_objs(*tbls->shard_ಗಂಟು, npossible);
	tbls->shard_ranges = kvzalloc_objs(*tbls->shard_ranges, npossible);
	tbls->topo = kvzalloc_objs(*tbls->topo, npossible);

	ಆದರೆ (!tbls->cid_ಗಮ್ಯ_cpu || !tbls->cpu_ಗಮ್ಯ_cid || !tbls->cid_ಗಮ್ಯ_shard ||
	    !tbls->shard_ಗಂಟು || !tbls->shard_ranges || !tbls->topo) {
		scx_cid_tables_free(tbls);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ tbls;
}

/**
 * scx_cid_publish_tables - Publish the tables scx_cid_init() built
 *
 * Called after ops.init_cids() where the layout is final.
 */
ಶೂನ್ಯ scx_cid_publish_tables(ಶೂನ್ಯ)
{
	ರಚನೆ scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);

	scx_nr_cid_shards = tbls->nr_shards;
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(scx_cid_ಗಮ್ಯ_cpu_tbl, tbls->cid_ಗಮ್ಯ_cpu);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(scx_cpu_ಗಮ್ಯ_cid_tbl, tbls->cpu_ಗಮ್ಯ_cid);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(scx_cid_ಗಮ್ಯ_shard, tbls->cid_ಗಮ್ಯ_shard);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(scx_shard_ಗಂಟು, tbls->shard_ಗಂಟು);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(scx_cid_shard_ranges, tbls->shard_ranges);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(scx_cid_topo, tbls->topo);
}

/**
 * scx_cid_retire_tables - Unpublish and retire the cid tables
 *
 * Called by root disable after the readers which dereference without NULL
 * checks are drained, inside cpus_read_lock() to exclude the hotplug path.
 */
ಶೂನ್ಯ scx_cid_retire_tables(ಶೂನ್ಯ)
{
	ರಚನೆ scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);
	lockdep_assert_cpus_held();

	ಆದರೆ (!tbls)
		ಹಿಂತಿರುಗಿ;

	scx_cid_tables = ಖಾಲಿ;
	RCU_INIT_ಸೂಚಕ(scx_cid_ಗಮ್ಯ_cpu_tbl, ಖಾಲಿ);
	RCU_INIT_ಸೂಚಕ(scx_cpu_ಗಮ್ಯ_cid_tbl, ಖಾಲಿ);
	RCU_INIT_ಸೂಚಕ(scx_cid_ಗಮ್ಯ_shard, ಖಾಲಿ);
	RCU_INIT_ಸೂಚಕ(scx_shard_ಗಂಟು, ಖಾಲಿ);
	RCU_INIT_ಸೂಚಕ(scx_cid_shard_ranges, ಖಾಲಿ);
	RCU_INIT_ಸೂಚಕ(scx_cid_topo, ಖಾಲಿ);
	call_rcu(&tbls->rcu, scx_cid_tables_free_rcufn);
}

/**
 * scx_cid_init - build the cid mapping
 * @sch: the scx_sched being initialized; used as the scx_error() target
 *
 * Build a fresh table set. It becomes visible through scx_cid_publish_tables()
 * and is retired by scx_cid_retire_tables() at disable.
 *
 * See "Topological CPU IDs" in cid.h for the model. Walk online cpus by
 * intersection at each level (parent_scratch & this_level_mask), which keeps
 * containment correct by construction and naturally splits a physical LLC
 * straddling two NUMA nodes into two LLC units. The caller must hold
 * cpus_read_lock.
 */
s32 scx_cid_init(ರಚನೆ scx_sched *sch)
{
	cpumask_var_t ಗಮ್ಯ_walk __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t ಗಂಟು_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t core_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_fallback __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t online_no_topo __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	ರಚನೆ scx_cid_tables *tbls;
	u32 ಮುಂದಿನ_cid = 0;
	s32 ಮುಂದಿನ_ಗಂಟು_idx = 0, ಮುಂದಿನ_llc_idx = 0, ಮುಂದಿನ_core_idx = 0;
	s32 ಮುಂದಿನ_shard_idx = 0;
	u32 shard_ಗಾತ್ರ, max_cids;
	u32 notopo_ಒಳ_shard;
	s32 notopo_shard_cid, notopo_shard_idx;
	s32 cpu, cid, si;

	/* CMASK_MAX_WORDS in cid.bpf.h covers NR_CPUS up to 8192 */
	BUILD_BUG_ON(NR_CPUS > 8192);

	lockdep_assert_cpus_held();
	lockdep_assert_held(&scx_enable_mutex);

	shard_ಗಾತ್ರ = sch->ops.cid_shard_ಗಾತ್ರ ?: SCX_CID_SHARD_ಗಾತ್ರ_DFL;
	max_cids = min_t(u32, shard_ಗಾತ್ರ, SCX_CID_SHARD_MAX_CPUS);

	tbls = scx_cid_alloc_tables();
	ಆದರೆ (!tbls)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	scx_cid_tables = tbls;

	ಗಾಗಿ (si = 0; si < ಅಂಕೆ_possible_cpus(); si++)
		tbls->shard_ಗಂಟು[si] = NUMA_NO_ಗಂಟು;

	ಆದರೆ (!zalloc_cpumask_var(&ಗಮ್ಯ_walk, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&ಗಂಟು_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&core_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_fallback, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&online_no_topo, GFP_KERNEL))
		ಹಿಂತಿರುಗಿ -ENOMEM;

	/* -1 sentinels for sparse-possible cpu id holes (0 is a valid cid) */
	ಗಾಗಿ (cpu = 0; cpu < nr_cpu_ids; cpu++)
		tbls->cpu_ಗಮ್ಯ_cid[cpu] = -1;

	cpumask_copy(ಗಮ್ಯ_walk, cpu_online_ಮುಖವಾಡ);

	ಆಗ (!cpumask_ಖಾಲಿಗಳು(ಗಮ್ಯ_walk)) {
		s32 ಮುಂದಿನ_cpu = cpumask_ಮೊದಲ(ಗಮ್ಯ_walk);
		s32 nid = cpu_ಗಮ್ಯ_ಗಂಟು(ಮುಂದಿನ_cpu);
		s32 ಗಂಟು_cid = ಮುಂದಿನ_cid;
		s32 ಗಂಟು_idx;

		/*
		 * No NUMA info: skip and let the tail loop assign a no-topo
		 * cid. cpumask_of_node(-1) is undefined.
		 */
		ಆದರೆ (nid < 0) {
			cpumask_clear_cpu(ಮುಂದಿನ_cpu, ಗಮ್ಯ_walk);
			ಮುಂದುವರಿ;
		}

		ಗಂಟು_idx = ಮುಂದಿನ_ಗಂಟು_idx++;

		/* node_scratch = to_walk & this node */
		cpumask_and(ಗಂಟು_scratch, ಗಮ್ಯ_walk, cpumask_ದ_ಗಂಟು(nid));
		ಆದರೆ (WARN_ON_ONCE(!cpumask_test_cpu(ಮುಂದಿನ_cpu, ಗಂಟು_scratch)))
			ಹಿಂತಿರುಗಿ -EINVAL;

		ಆಗ (!cpumask_ಖಾಲಿಗಳು(ಗಂಟು_scratch)) {
			s32 ncpu = cpumask_ಮೊದಲ(ಗಂಟು_scratch);
			ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *llc_ಮುಖವಾಡ = cpu_llc_ಮುಖವಾಡ(ncpu, llc_fallback);
			s32 llc_cid = ಮುಂದಿನ_cid;
			s32 llc_idx = ಮುಂದಿನ_llc_idx++;
			u32 cores_ಪ್ರತಿ_shard, nr_large;
			u32 shard_local = 0, cores_ಒಳ_shard = 0, cids_ಒಳ_shard = 0;
			s32 shard_cid, shard_idx;

			/* llc_scratch = node_scratch & this llc */
			cpumask_and(llc_scratch, ಗಂಟು_scratch, llc_ಮುಖವಾಡ);
			ಆದರೆ (WARN_ON_ONCE(!cpumask_test_cpu(ncpu, llc_scratch)))
				ಹಿಂತಿರುಗಿ -EINVAL;

			calc_shard_layout(llc_scratch, shard_ಗಾತ್ರ, &cores_ಪ್ರತಿ_shard, &nr_large);
			shard_cid = ಮುಂದಿನ_cid;
			shard_idx = ಮುಂದಿನ_shard_idx++;
			tbls->shard_ಗಂಟು[shard_idx] = nid;

			ಆಗ (!cpumask_ಖಾಲಿಗಳು(llc_scratch)) {
				s32 lcpu = cpumask_ಮೊದಲ(llc_scratch);
				ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *sib = topology_sibling_cpumask(lcpu);
				s32 core_cid = ಮುಂದಿನ_cid;
				s32 core_idx = ಮುಂದಿನ_core_idx++;
				s32 ccpu;
				u32 max_cores, cids_ಒಳ_core;

				/* core_scratch = llc_scratch & this core */
				cpumask_and(core_scratch, llc_scratch, sib);
				ಆದರೆ (WARN_ON_ONCE(!cpumask_test_cpu(lcpu, core_scratch)))
					ಹಿಂತಿರುಗಿ -EINVAL;

				/*
				 * Advance to a new shard when either core or
				 * cid count reaches max. The latter bounds
				 * shard sizes under uneven SMT. Never start an
				 * empty shard.
				 */
				cids_ಒಳ_core = cpumask_ತೂಕ(core_scratch);
				max_cores = cores_ಪ್ರತಿ_shard + (shard_local < nr_large ? 1 : 0);
				ಆದರೆ (cores_ಒಳ_shard &&
				    (cores_ಒಳ_shard >= max_cores ||
				     cids_ಒಳ_shard + cids_ಒಳ_core > max_cids)) {
					shard_local++;
					cores_ಒಳ_shard = 0;
					cids_ಒಳ_shard = 0;
					shard_cid = ಮುಂದಿನ_cid;
					shard_idx = ಮುಂದಿನ_shard_idx++;
					tbls->shard_ಗಂಟು[shard_idx] = nid;
				}
				cores_ಒಳ_shard++;
				cids_ಒಳ_shard += cids_ಒಳ_core;

				for_each_cpu(ccpu, core_scratch) {
					s32 cid = ಮುಂದಿನ_cid++;

					tbls->cid_ಗಮ್ಯ_cpu[cid] = ccpu;
					tbls->cpu_ಗಮ್ಯ_cid[ccpu] = cid;
					tbls->cid_ಗಮ್ಯ_shard[cid] = shard_idx;
					tbls->topo[cid] = (ರಚನೆ scx_cid_topo){
						.core_cid = core_cid,
						.core_idx = core_idx,
						.llc_cid = llc_cid,
						.llc_idx = llc_idx,
						.ಗಂಟು_cid = ಗಂಟು_cid,
						.ಗಂಟು_idx = ಗಂಟು_idx,
						.shard_cid = shard_cid,
						.shard_idx = shard_idx,
					};

					cpumask_clear_cpu(ccpu, llc_scratch);
					cpumask_clear_cpu(ccpu, ಗಂಟು_scratch);
					cpumask_clear_cpu(ccpu, ಗಮ್ಯ_walk);
				}
			}
		}
	}

	/*
	 * No-topo section: any possible cpu without a cid - normally just the
	 * not-online ones. Pack into shards of up to min(@shard_size,
	 * SCX_CID_SHARD_MAX_CPUS) cids so that every cid has a valid shard
	 * assignment and the hard cap holds even with a large @shard_size.
	 * Collect any currently-online cpus that land here in @online_no_topo
	 * so we can warn about them at the end.
	 */
	notopo_ಒಳ_shard = min_t(u32, shard_ಗಾತ್ರ, SCX_CID_SHARD_MAX_CPUS);
	notopo_shard_cid = -1;
	notopo_shard_idx = -1;

	for_each_cpu(cpu, cpu_possible_ಮುಖವಾಡ) {
		ಆದರೆ (tbls->cpu_ಗಮ್ಯ_cid[cpu] != -1)
			ಮುಂದುವರಿ;
		ಆದರೆ (cpu_online(cpu))
			cpumask_ಗಣ_cpu(cpu, online_no_topo);

		cid = ಮುಂದಿನ_cid++;
		tbls->cid_ಗಮ್ಯ_cpu[cid] = cpu;
		tbls->cpu_ಗಮ್ಯ_cid[cpu] = cid;

		ಆದರೆ (notopo_ಒಳ_shard >= min_t(u32, shard_ಗಾತ್ರ, SCX_CID_SHARD_MAX_CPUS)) {
			notopo_shard_cid = cid;
			notopo_shard_idx = ಮುಂದಿನ_shard_idx++;
			notopo_ಒಳ_shard = 0;
		}
		notopo_ಒಳ_shard++;

		tbls->cid_ಗಮ್ಯ_shard[cid] = notopo_shard_idx;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = notopo_shard_cid;
		tbls->topo[cid].shard_idx = notopo_shard_idx;
	}

	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(llc_fallback))
		pr_warn("scx_cid: cpus without cacheinfo, using node mask as llc: %*pbl\n",
			cpumask_pr_ವಾದಗಳು(llc_fallback));
	ಆದರೆ (!cpumask_ಖಾಲಿಗಳು(online_no_topo))
		pr_warn("scx_cid: online cpus with no usable topology: %*pbl\n",
			cpumask_pr_ವಾದಗಳು(online_no_topo));

	/*
	 * Fill cid_shard_ranges[] from cid_to_shard[]. Shards are contiguous
	 * cid ranges by construction: base_cid is the first cid landing in a
	 * shard, nr_cids is the count.
	 */
	ಗಾಗಿ (cid = 0; cid < ಮುಂದಿನ_cid; cid++) {
		s32 sidx = tbls->cid_ಗಮ್ಯ_shard[cid];

		ಆದರೆ (tbls->shard_ranges[sidx].nr_cids == 0)
			tbls->shard_ranges[sidx].ತಳ_cid = cid;
		tbls->shard_ranges[sidx].nr_cids++;
	}

	tbls->nr_shards = ಮುಂದಿನ_shard_idx;
	ಹಿಂತಿರುಗಿ 0;
}

/**
 * scx_cmask_clear - Zero every bit in @m's active range
 * @m: cmask to clear
 *
 * Storage past the active range is left as is.
 */
ಶೂನ್ಯ scx_cmask_clear(ರಚನೆ scx_cmask *m)
{
	u32 nr_ಪದಗಳು;

	ಆದರೆ (!m->nr_cids)
		ಹಿಂತಿರುಗಿ;
	nr_ಪದಗಳು = (m->ತಳ + m->nr_cids - 1) / 64 - m->ತಳ / 64 + 1;
	memset(m->ಬಿಟ್ಟುಗಳು, 0, nr_ಪದಗಳು * ಗಾತ್ರ(u64));
}

/**
 * scx_cmask_fill - Set every bit in @m's active range
 * @m: cmask to fill
 *
 * Counterpart to scx_cmask_clear(). Storage past the active range is left as is.
 */
ಶೂನ್ಯ scx_cmask_fill(ರಚನೆ scx_cmask *m)
{
	u32 nr_ಪದಗಳು, ತಲೆ_ಬಿಟ್ಟುಗಳು, ಬಾಲ_ಬಿಟ್ಟುಗಳು;

	ಆದರೆ (!m->nr_cids)
		ಹಿಂತಿರುಗಿ;
	nr_ಪದಗಳು = (m->ತಳ + m->nr_cids - 1) / 64 - m->ತಳ / 64 + 1;
	memset(m->ಬಿಟ್ಟುಗಳು, 0xff, nr_ಪದಗಳು * ಗಾತ್ರ(u64));

	/* clear word-0 bits below base */
	ತಲೆ_ಬಿಟ್ಟುಗಳು = m->ತಳ & 63;
	ಆದರೆ (ತಲೆ_ಬಿಟ್ಟುಗಳು)
		m->ಬಿಟ್ಟುಗಳು[0] &= ~((1ULL << ತಲೆ_ಬಿಟ್ಟುಗಳು) - 1);

	/* clear last-word bits at or past base + nr_cids */
	ಬಾಲ_ಬಿಟ್ಟುಗಳು = (m->ತಳ + m->nr_cids) & 63;
	ಆದರೆ (ಬಾಲ_ಬಿಟ್ಟುಗಳು)
		m->ಬಿಟ್ಟುಗಳು[nr_ಪದಗಳು - 1] &= (1ULL << ಬಾಲ_ಬಿಟ್ಟುಗಳು) - 1;
}

/*
 * Return the index of the largest entry in @counts, or NUMA_NO_NODE if all
 * entries are zero. Ties resolve to the lowest index.
 */
ಸ್ಥಿರ s32 pick_max_ಗಂಟು(ಸ್ಥಿರಾಂಕ u32 *counts, u32 n)
{
	s32 ಉತ್ತಮ = NUMA_NO_ಗಂಟು;
	u32 ಉತ್ತಮ_ಎಣಿಕೆ = 0, i;

	ಗಾಗಿ (i = 0; i < n; i++) {
		ಆದರೆ (counts[i] > ಉತ್ತಮ_ಎಣಿಕೆ) {
			ಉತ್ತಮ_ಎಣಿಕೆ = counts[i];
			ಉತ್ತಮ = i;
		}
	}
	ಹಿಂತಿರುಗಿ ಉತ್ತಮ;
}

__bpf_kfunc_start_defs();

/**
 * scx_bpf_cid_override - Install an explicit cpu->cid mapping with shard info
 * @cpu_to_cid__arena: array of nr_cpu_ids s32 entries (cid for each cpu)
 * @cpu_to_cid_cnt: number of entries, must be nr_cpu_ids
 * @shard_start__arena: array of first-cid-of-each-shard, one entry per shard
 * @shard_start_cnt: number of shards
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * May only be called from ops.init_cids() of the root scheduler. Replace the
 * topology-probed cid mapping and shard layout with caller-provided ones. Each
 * possible cpu must map to a unique cid in [0, num_possible_cpus()). The shard
 * starts must be strictly increasing with the first entry 0 and all values <
 * num_possible_cpus(). The last shard extends to num_possible_cpus() and no
 * shard may span more than SCX_CID_SHARD_MAX_CPUS cids. Topo info
 * (core/LLC/node) is cleared and the shard layout is set from the input. On
 * invalid input, abort the scheduler.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_cid_override(ಸ್ಥಿರಾಂಕ s32 *cpu_ಗಮ್ಯ_cid_arena, u32 cpu_ಗಮ್ಯ_cid_cnt,
				      ಸ್ಥಿರಾಂಕ s32 *shard_ಆರಂಭ_arena, u32 shard_ಆರಂಭ_cnt,
				      ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	cpumask_var_t ನೋಡಿದ __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	u32 *ಗಂಟು_counts __free(kfree) = ಖಾಲಿ;
	s32 *cpu_ಗಮ್ಯ_cid __free(kfree) = ಖಾಲಿ;
	s32 *shard_ಆರಂಭ __free(kfree) = ಖಾಲಿ;
	u32 npossible = ಅಂಕೆ_possible_cpus();
	ರಚನೆ scx_cid_tables *tbls;
	ರಚನೆ scx_sched *sch;
	u32 nr_shards = shard_ಆರಂಭ_cnt;
	bool alloced;
	s32 cpu, cid, si;

	/*
	 * GFP_KERNEL allocs must happen before the rcu read section. Snapshot
	 * the BPF-supplied arrays so a concurrent arena write can't change
	 * them between validation and use.
	 *
	 * The BPF-supplied counts size the snapshots and thus the arena reads.
	 * Gate the copies on the count bounds, reported below once @sch is
	 * available. The bounded reads, at most 32KB, stay within the guard
	 * region that arena fault recovery covers.
	 */
	alloced = zalloc_cpumask_var(&ನೋಡಿದ, GFP_KERNEL);
	ಗಂಟು_counts = kzalloc_objs(*ಗಂಟು_counts, nr_ಗಂಟು_ids);
	ಆದರೆ (cpu_ಗಮ್ಯ_cid_cnt == nr_cpu_ids)
		cpu_ಗಮ್ಯ_cid = kmemdup(cpu_ಗಮ್ಯ_cid_arena, cpu_ಗಮ್ಯ_cid_cnt * ಗಾತ್ರ(s32),
				     GFP_KERNEL);
	ಆದರೆ (nr_shards && nr_shards <= npossible)
		shard_ಆರಂಭ = kmemdup(shard_ಆರಂಭ_arena, nr_shards * ಗಾತ್ರ(s32),
				      GFP_KERNEL);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ;

	/* called from ops.init_cids(), so the tables exist and are unpublished */
	lockdep_assert_held(&scx_enable_mutex);
	tbls = scx_cid_tables;

	ಆದರೆ (cpu_ಗಮ್ಯ_cid_cnt != nr_cpu_ids) {
		scx_error(sch, "scx_bpf_cid_override: cpu_to_cid expected %u entries, got %u",
			  nr_cpu_ids, cpu_ಗಮ್ಯ_cid_cnt);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!nr_shards || nr_shards > npossible) {
		scx_error(sch, "scx_bpf_cid_override: invalid shard_start count %u",
			  nr_shards);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!alloced || !ಗಂಟು_counts || !cpu_ಗಮ್ಯ_cid || !shard_ಆರಂಭ) {
		scx_error(sch, "scx_bpf_cid_override: allocation failed");
		ಹಿಂತಿರುಗಿ;
	}

	/* validate shard_start[]: starts at 0, strictly increasing, in range */
	ಆದರೆ (shard_ಆರಂಭ[0] != 0) {
		scx_error(sch, "scx_bpf_cid_override: shard_start[0] must be 0, got %d",
			  shard_ಆರಂಭ[0]);
		ಹಿಂತಿರುಗಿ;
	}
	ಗಾಗಿ (si = 1; si < nr_shards; si++) {
		ಆದರೆ (shard_ಆರಂಭ[si] <= shard_ಆರಂಭ[si - 1]) {
			scx_error(sch, "scx_bpf_cid_override: shard_start not increasing at [%d]",
				  si);
			ಹಿಂತಿರುಗಿ;
		}
		ಆದರೆ (shard_ಆರಂಭ[si] >= npossible) {
			scx_error(sch, "scx_bpf_cid_override: shard_start[%d]=%d >= %u",
				  si, shard_ಆರಂಭ[si], npossible);
			ಹಿಂತಿರುಗಿ;
		}
		ಆದರೆ (shard_ಆರಂಭ[si] - shard_ಆರಂಭ[si - 1] > SCX_CID_SHARD_MAX_CPUS) {
			scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
				  si - 1, shard_ಆರಂಭ[si] - shard_ಆರಂಭ[si - 1],
				  SCX_CID_SHARD_MAX_CPUS);
			ಹಿಂತಿರುಗಿ;
		}
	}
	ಆದರೆ (npossible - shard_ಆರಂಭ[nr_shards - 1] > SCX_CID_SHARD_MAX_CPUS) {
		scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
			  nr_shards - 1, npossible - shard_ಆರಂಭ[nr_shards - 1],
			  SCX_CID_SHARD_MAX_CPUS);
		ಹಿಂತಿರುಗಿ;
	}

	/* validate first so that invalid input leaves the tables untouched */
	for_each_possible_cpu(cpu) {
		s32 c = cpu_ಗಮ್ಯ_cid[cpu];

		ಆದರೆ (!cid_valid(sch, c))
			ಹಿಂತಿರುಗಿ;
		ಆದರೆ (cpumask_test_and_ಗಣ_cpu(c, ನೋಡಿದ)) {
			scx_error(sch, "cid %d assigned to multiple cpus", c);
			ಹಿಂತಿರುಗಿ;
		}
	}

	for_each_possible_cpu(cpu) {
		s32 c = cpu_ಗಮ್ಯ_cid[cpu];

		tbls->cpu_ಗಮ್ಯ_cid[cpu] = c;
		tbls->cid_ಗಮ್ಯ_cpu[c] = cpu;
	}

	/*
	 * Derive shard_node[] by majority count: an overridden shard may
	 * span NUMA nodes, so assign each to the node that owns the most cpus.
	 */
	ಗಾಗಿ (si = 0; si < nr_shards; si++) {
		u32 ಕೊನೆ = (si + 1 < nr_shards) ? shard_ಆರಂಭ[si + 1] : npossible;

		memset(ಗಂಟು_counts, 0, nr_ಗಂಟು_ids * ಗಾತ್ರ(*ಗಂಟು_counts));
		ಗಾಗಿ (cid = shard_ಆರಂಭ[si]; cid < ಕೊನೆ; cid++) {
			s32 ಗಂಟು = cpu_ಗಮ್ಯ_ಗಂಟು(tbls->cid_ಗಮ್ಯ_cpu[cid]);

			ಆದರೆ (numa_valid_ಗಂಟು(ಗಂಟು))
				ಗಂಟು_counts[ಗಂಟು]++;
		}
		tbls->shard_ಗಂಟು[si] = pick_max_ಗಂಟು(ಗಂಟು_counts, nr_ಗಂಟು_ids);
	}

	/*
	 * Invalidate stale topo info and install shard layout from
	 * @shard_start. Walk shards to derive shard_cid/shard_idx for each cid.
	 */
	si = 0;
	ಗಾಗಿ (cid = 0; cid < npossible; cid++) {
		ಆದರೆ (si + 1 < nr_shards && cid >= shard_ಆರಂಭ[si + 1])
			si++;
		tbls->cid_ಗಮ್ಯ_shard[cid] = si;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = shard_ಆರಂಭ[si];
		tbls->topo[cid].shard_idx = si;
	}

	/* Rebuild shard_ranges[] for the new layout. */
	memset(tbls->shard_ranges, 0, npossible * ಗಾತ್ರ(*tbls->shard_ranges));
	ಗಾಗಿ (si = 0; si < nr_shards; si++) {
		u32 ಕೊನೆ = (si + 1 < nr_shards) ? shard_ಆರಂಭ[si + 1] : npossible;

		tbls->shard_ranges[si].ತಳ_cid = shard_ಆರಂಭ[si];
		tbls->shard_ranges[si].nr_cids = ಕೊನೆ - shard_ಆರಂಭ[si];
	}

	tbls->nr_shards = nr_shards;
}

/**
 * scx_bpf_cid_to_cpu - Return the raw CPU id for @cid
 * @cid: cid to look up
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Return the raw CPU id for @cid. Trigger scx_error() and return -EINVAL if
 * @cid is invalid. The cid<->cpu mapping is static for the lifetime of the
 * loaded scheduler, so the BPF side can cache the result to avoid repeated
 * kfunc invocations.
 */
__bpf_kfunc s32 scx_bpf_cid_ಗಮ್ಯ_cpu(s32 cid, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಹಿಂತಿರುಗಿ scx_cid_ಗಮ್ಯ_cpu(sch, cid);
}

/**
 * scx_bpf_cpu_to_cid - Return the cid for @cpu
 * @cpu: cpu to look up
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Return the cid for @cpu. Trigger scx_error() and return -EINVAL if @cpu is
 * invalid. The cid<->cpu mapping is static for the lifetime of the loaded
 * scheduler, so the BPF side can cache the result to avoid repeated kfunc
 * invocations.
 */
__bpf_kfunc s32 scx_bpf_cpu_ಗಮ್ಯ_cid(s32 cpu, ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ರಚನೆ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಆದರೆ (unlikely(!sch))
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಹಿಂತಿರುಗಿ scx_cpu_ಗಮ್ಯ_cid(sch, cpu);
}

/*
 * Set ops on cmasks. cmask_walk_op2() shares one walk across mutating
 * (and/or/copy/andnot) and predicate (subset/intersects) two-cmask forms;
 * cmask_walk_op1() does the same shape over a single cmask range. Every public
 * entry passes a compile-time-constant @op; cmask_walk_op{1,2}() and
 * cmask_word_op{1,2}() are __always_inline so the inner switch collapses to the
 * selected op and cmask_op2_is_pred() folds the predicate early-exit out of
 * mutating ops.
 *
 * Two-cmask ops only touch @dst bits inside the intersection of the two ranges;
 * bits outside stay untouched. In particular, scx_cmask_copy() does NOT zero
 * @dst bits that lie outside @src's range.
 *
 * Word accesses use READ_ONCE/WRITE_ONCE so a caller may read @src
 * locklessly. Memory ordering against concurrent writers is the caller's
 * responsibility.
 */
ಎಣಿಕೆ cmask_op2 {
	/* mutating */
	CMASK_OP2_AND,
	CMASK_OP2_OR,
	CMASK_OP2_COPY,
	CMASK_OP2_ANDNOT,
	/* predicates - short-circuit when the per-word result is true */
	CMASK_OP2_SUBSET,
	CMASK_OP2_INTERSECTS,
	/*
	 * @a is a BPF-arena cmask. Words on @a use READ_ONCE/WRITE_ONCE since
	 * BPF may read/write concurrently. See scx_cmask_ref_or() / _copy().
	 */
	CMASK_OP2_REF_OR,
	CMASK_OP2_REF_COPY,
};

ಸ್ಥಿರ __always_inline bool cmask_op2_is_pred(ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ cmask_op2 op)
{
	ಹಿಂತಿರುಗಿ op == CMASK_OP2_SUBSET || op == CMASK_OP2_INTERSECTS;
}

ಸ್ಥಿರ __always_inline bool cmask_ಶಬ್ದ_op2(u64 *av, ಸ್ಥಿರಾಂಕ u64 *bp, u64 ಮುಖವಾಡ,
					   ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ cmask_op2 op)
{
	ಆಯ್ಕೆ (op) {
	ಸ್ಥಿತಿ CMASK_OP2_AND:
		WRITE_ONCE(*av, *av & (~ಮುಖವಾಡ | READ_ONCE(*bp)));
		ಹಿಂತಿರುಗಿ false;
	ಸ್ಥಿತಿ CMASK_OP2_OR:
		WRITE_ONCE(*av, *av | (READ_ONCE(*bp) & ಮುಖವಾಡ));
		ಹಿಂತಿರುಗಿ false;
	ಸ್ಥಿತಿ CMASK_OP2_COPY:
		WRITE_ONCE(*av, (*av & ~ಮುಖವಾಡ) | (READ_ONCE(*bp) & ಮುಖವಾಡ));
		ಹಿಂತಿರುಗಿ false;
	ಸ್ಥಿತಿ CMASK_OP2_ANDNOT:
		WRITE_ONCE(*av, *av & ~(READ_ONCE(*bp) & ಮುಖವಾಡ));
		ಹಿಂತಿರುಗಿ false;
	ಸ್ಥಿತಿ CMASK_OP2_SUBSET:
		/* stop on the first bit in @sub not set in @super */
		ಹಿಂತಿರುಗಿ (READ_ONCE(*bp) & ~READ_ONCE(*av)) & ಮುಖವಾಡ;
	ಸ್ಥಿತಿ CMASK_OP2_INTERSECTS:
		ಹಿಂತಿರುಗಿ (READ_ONCE(*av) & READ_ONCE(*bp)) & ಮುಖವಾಡ;
	ಸ್ಥಿತಿ CMASK_OP2_REF_OR:
		WRITE_ONCE(*av, READ_ONCE(*av) | (READ_ONCE(*bp) & ಮುಖವಾಡ));
		ಹಿಂತಿರುಗಿ false;
	ಸ್ಥಿತಿ CMASK_OP2_REF_COPY:
		WRITE_ONCE(*av, (READ_ONCE(*av) & ~ಮುಖವಾಡ) | (READ_ONCE(*bp) & ಮುಖವಾಡ));
		ಹಿಂತಿರುಗಿ false;
	}
	unreachable();
}

/*
 * Walk the intersection of [@a_base, @a_base + @a_nr_cids) with [@b_base,
 * @b_base + @b_nr_cids) word by word, applying @op. Mutating ops walk all words
 * and return false; predicates return true on the first word whose per-word
 * test is true. Empty intersection returns false (matches "no bits to consider"
 * for both mutate and predicate).
 *
 * Base/nr_cids are taken as parameters so callers with snapshotted bounds can
 * drive the walk with values independent of the cmask's header.
 */
ಸ್ಥಿರ __always_inline bool cmask_walk_op2(u64 *a_ಬಿಟ್ಟುಗಳು, u32 a_ತಳ, u32 a_nr_cids,
					   ಸ್ಥಿರಾಂಕ u64 *b_ಬಿಟ್ಟುಗಳು, u32 b_ತಳ, u32 b_nr_cids,
					   ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ cmask_op2 op)
{
	u32 lo = max(a_ತಳ, b_ತಳ);
	u32 hi = min(a_ತಳ + a_nr_cids, b_ತಳ + b_nr_cids);
	u32 a_ಶಬ್ದ_off = a_ತಳ / 64;
	u32 b_ಶಬ್ದ_off = b_ತಳ / 64;
	u32 lo_ಶಬ್ದ = lo / 64;
	u32 hi_ಶಬ್ದ = (hi - 1) / 64;
	u64 ತಲೆ_ಮುಖವಾಡ = GENMASK_U64(63, lo & 63);
	u64 ಬಾಲ_ಮುಖವಾಡ = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ಆದರೆ (lo >= hi)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (lo_ಶಬ್ದ == hi_ಶಬ್ದ)
		ಹಿಂತಿರುಗಿ cmask_ಶಬ್ದ_op2(&a_ಬಿಟ್ಟುಗಳು[lo_ಶಬ್ದ - a_ಶಬ್ದ_off],
				      &b_ಬಿಟ್ಟುಗಳು[lo_ಶಬ್ದ - b_ಶಬ್ದ_off],
				      ತಲೆ_ಮುಖವಾಡ & ಬಾಲ_ಮುಖವಾಡ, op);

	ಆದರೆ (cmask_ಶಬ್ದ_op2(&a_ಬಿಟ್ಟುಗಳು[lo_ಶಬ್ದ - a_ಶಬ್ದ_off],
			   &b_ಬಿಟ್ಟುಗಳು[lo_ಶಬ್ದ - b_ಶಬ್ದ_off], ತಲೆ_ಮುಖವಾಡ, op) &&
	    cmask_op2_is_pred(op))
		ಹಿಂತಿರುಗಿ true;

	ಗಾಗಿ (w = lo_ಶಬ್ದ + 1; w < hi_ಶಬ್ದ; w++)
		ಆದರೆ (cmask_ಶಬ್ದ_op2(&a_ಬಿಟ್ಟುಗಳು[w - a_ಶಬ್ದ_off],
				   &b_ಬಿಟ್ಟುಗಳು[w - b_ಶಬ್ದ_off], ~0ULL, op) &&
		    cmask_op2_is_pred(op))
			ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ cmask_ಶಬ್ದ_op2(&a_ಬಿಟ್ಟುಗಳು[hi_ಶಬ್ದ - a_ಶಬ್ದ_off],
			      &b_ಬಿಟ್ಟುಗಳು[hi_ಶಬ್ದ - b_ಶಬ್ದ_off], ಬಾಲ_ಮುಖವಾಡ, op);
}

ಎಣಿಕೆ cmask_op1 {
	CMASK_OP1_ANY_ಗಣ,
};

ಸ್ಥಿರ __always_inline bool cmask_ಶಬ್ದ_op1(ಸ್ಥಿರಾಂಕ u64 *ap, u64 ಮುಖವಾಡ,
					   ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ cmask_op1 op)
{
	ಆಯ್ಕೆ (op) {
	ಸ್ಥಿತಿ CMASK_OP1_ANY_ಗಣ:
		ಹಿಂತಿರುಗಿ READ_ONCE(*ap) & ಮುಖವಾಡ;
	}
	unreachable();
}

/*
 * Walk [@a_base, @a_base + @a_nr_cids) of @a_bits word by word, applying @op.
 * Returns true on the first word whose per-word test is true; returns false if
 * no word matches or the range is empty. All current op1s short-circuit on
 * per-word true; if a non-predicate op1 lands here, add a cmask_op1_is_pred()
 * guard analogous to cmask_op2_is_pred().
 */
ಸ್ಥಿರ __always_inline bool cmask_walk_op1(ಸ್ಥಿರಾಂಕ u64 *a_ಬಿಟ್ಟುಗಳು, u32 a_ತಳ,
					   u32 a_nr_cids,
					   ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ cmask_op1 op)
{
	u32 lo = a_ತಳ;
	u32 hi = a_ತಳ + a_nr_cids;
	u32 a_ಶಬ್ದ_off = a_ತಳ / 64;
	u32 lo_ಶಬ್ದ = lo / 64;
	u32 hi_ಶಬ್ದ = (hi - 1) / 64;
	u64 ತಲೆ_ಮುಖವಾಡ = GENMASK_U64(63, lo & 63);
	u64 ಬಾಲ_ಮುಖವಾಡ = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ಆದರೆ (lo >= hi)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (lo_ಶಬ್ದ == hi_ಶಬ್ದ)
		ಹಿಂತಿರುಗಿ cmask_ಶಬ್ದ_op1(&a_ಬಿಟ್ಟುಗಳು[lo_ಶಬ್ದ - a_ಶಬ್ದ_off],
				      ತಲೆ_ಮುಖವಾಡ & ಬಾಲ_ಮುಖವಾಡ, op);

	ಆದರೆ (cmask_ಶಬ್ದ_op1(&a_ಬಿಟ್ಟುಗಳು[lo_ಶಬ್ದ - a_ಶಬ್ದ_off], ತಲೆ_ಮುಖವಾಡ, op))
		ಹಿಂತಿರುಗಿ true;
	ಗಾಗಿ (w = lo_ಶಬ್ದ + 1; w < hi_ಶಬ್ದ; w++)
		ಆದರೆ (cmask_ಶಬ್ದ_op1(&a_ಬಿಟ್ಟುಗಳು[w - a_ಶಬ್ದ_off], ~0ULL, op))
			ಹಿಂತಿರುಗಿ true;
	ಹಿಂತಿರುಗಿ cmask_ಶಬ್ದ_op1(&a_ಬಿಟ್ಟುಗಳು[hi_ಶಬ್ದ - a_ಶಬ್ದ_off], ಬಾಲ_ಮುಖವಾಡ, op);
}

ಶೂನ್ಯ scx_cmask_and(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src)
{
	cmask_walk_op2(dst->ಬಿಟ್ಟುಗಳು, dst->ತಳ, dst->nr_cids,
		       src->ಬಿಟ್ಟುಗಳು, src->ತಳ, src->nr_cids, CMASK_OP2_AND);
}

ಶೂನ್ಯ scx_cmask_or(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src)
{
	cmask_walk_op2(dst->ಬಿಟ್ಟುಗಳು, dst->ತಳ, dst->nr_cids,
		       src->ಬಿಟ್ಟುಗಳು, src->ತಳ, src->nr_cids, CMASK_OP2_OR);
}

ಶೂನ್ಯ scx_cmask_copy(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src)
{
	cmask_walk_op2(dst->ಬಿಟ್ಟುಗಳು, dst->ತಳ, dst->nr_cids,
		       src->ಬಿಟ್ಟುಗಳು, src->ತಳ, src->nr_cids, CMASK_OP2_COPY);
}

ಶೂನ್ಯ scx_cmask_andnot(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src)
{
	cmask_walk_op2(dst->ಬಿಟ್ಟುಗಳು, dst->ತಳ, dst->nr_cids,
		       src->ಬಿಟ್ಟುಗಳು, src->ತಳ, src->nr_cids, CMASK_OP2_ANDNOT);
}

/*
 * Return true if @cm has any bit set in [@lo, @hi). Caller must ensure
 * [@lo, @hi) is contained in @cm's range.
 */
ಸ್ಥಿರ bool cmask_any_ಗಣ_ಒಳ_range(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *cm, u32 lo, u32 hi)
{
	ಆದರೆ (lo >= hi)
		ಹಿಂತಿರುಗಿ false;
	ಹಿಂತಿರುಗಿ cmask_walk_op1(&cm->ಬಿಟ್ಟುಗಳು[lo / 64 - cm->ತಳ / 64], lo, hi - lo,
			      CMASK_OP1_ANY_ಗಣ);
}

/**
 * scx_cmask_subset - test whether @sub is a subset of @super
 * @sub: cmask to test
 * @super: cmask to test against
 *
 * Return true iff every set bit of @sub is also set in @super.
 */
bool scx_cmask_subset(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *sub, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *super)
{
	u32 super_ಕೊನೆ = super->ತಳ + super->nr_cids;
	u32 sub_ಕೊನೆ = sub->ತಳ + sub->nr_cids;

	/*
	 * Set bits in @sub outside @super's range can't be in @super, so any
	 * such bit means not a subset. The walk below only visits words
	 * common to both ranges, so these need a separate scan.
	 */
	ಆದರೆ (sub->ತಳ < super->ತಳ &&
	    cmask_any_ಗಣ_ಒಳ_range(sub, sub->ತಳ, min(super->ತಳ, sub_ಕೊನೆ)))
		ಹಿಂತಿರುಗಿ false;
	ಆದರೆ (sub_ಕೊನೆ > super_ಕೊನೆ &&
	    cmask_any_ಗಣ_ಒಳ_range(sub, max(sub->ತಳ, super_ಕೊನೆ), sub_ಕೊನೆ))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ !cmask_walk_op2((u64 *)super->ಬಿಟ್ಟುಗಳು, super->ತಳ, super->nr_cids,
			       sub->ಬಿಟ್ಟುಗಳು, sub->ತಳ, sub->nr_cids, CMASK_OP2_SUBSET);
}

bool scx_cmask_intersects(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *a, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *b)
{
	ಹಿಂತಿರುಗಿ cmask_walk_op2((u64 *)a->ಬಿಟ್ಟುಗಳು, a->ತಳ, a->nr_cids,
			      b->ಬಿಟ್ಟುಗಳು, b->ತಳ, b->nr_cids, CMASK_OP2_INTERSECTS);
}

/**
 * scx_cmask_empty - Test whether @m has no bits set
 * @m: cmask to test
 *
 * Return true iff @m's active range has no bits set.
 */
bool scx_cmask_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *m)
{
	ಹಿಂತಿರುಗಿ !cmask_any_ಗಣ_ಒಳ_range(m, m->ತಳ, m->ತಳ + m->nr_cids);
}

/**
 * scx_bpf_cid_topo - Copy out per-cid topology info
 * @cid: cid to look up
 * @out: where to copy the topology info
 * @out__sz: size of @out, the program's sizeof(struct scx_cid_topo)
 * @aux: implicit BPF argument to access bpf_prog_aux hidden from BPF progs
 *
 * Fill @out with the topology info for @cid. Trigger scx_error() if @cid is out
 * of range. If @cid is valid but in the no-topo section, all fields are set to
 * -1. All fields are also set to -1 when no cid tables have been published yet,
 * which a program may observe while racing the root enable.
 *
 * The program's struct may be older or newer than the kernel's. The smaller of
 * @out__sz and the kernel's size is copied and the rest of @out is set to -1.
 */
__bpf_kfunc ಶೂನ್ಯ scx_bpf_cid_topo(s32 cid, ರಚನೆ scx_cid_topo *ಹೊರಹೋಗು, ಗಾತ್ರ_t ಹೊರಹೋಗು_sz,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ bpf_prog_aux *aux)
{
	ಗಾತ್ರ_t len = min(ಹೊರಹೋಗು_sz, ಗಾತ್ರ(*ಹೊರಹೋಗು));
	ರಚನೆ scx_cid_topo *topo;
	ರಚನೆ scx_sched *sch;

	/* the error cases and fields the kernel lacks read as -1 */
	memset(ಹೊರಹೋಗು, 0xff, ಹೊರಹೋಗು_sz);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	topo = rcu_dereference(scx_cid_topo);
	ಆದರೆ (unlikely(!sch) || !cid_valid(sch, cid) || unlikely(!topo))
		ಹಿಂತಿರುಗಿ;

	ಸ್ಮರಣೆ_ನಕಲು(ಹೊರಹೋಗು, &topo[cid], len);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_init_cids)
BTF_ID_FLAGS(func, scx_bpf_cid_override, KF_IMPLICIT_ವಾದಗಳು | KF_SLEEPABLE)
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_init_cids)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_kfunc_id_ಗಣ scx_kfunc_ಗಣ_init_cids = {
	.owner	= THIS_MODULE,
	.ಗಣ	= &scx_kfunc_ids_init_cids,
	.filter	= scx_kfunc_context_filter,
};

BTF_KFUNCS_ಆರಂಭ(scx_kfunc_ids_cid)
BTF_ID_FLAGS(func, scx_bpf_cid_ಗಮ್ಯ_cpu, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cpu_ಗಮ್ಯ_cid, KF_IMPLICIT_ವಾದಗಳು)
BTF_ID_FLAGS(func, scx_bpf_cid_topo, KF_IMPLICIT_ವಾದಗಳು)
BTF_KFUNCS_ಕೊನೆ(scx_kfunc_ids_cid)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ btf_kfunc_id_ಗಣ scx_kfunc_ಗಣ_cid = {
	.owner	= THIS_MODULE,
	.ಗಣ	= &scx_kfunc_ids_cid,
};

/**
 * scx_cmask_ref_init - Bind a scx_cmask_ref to a BPF-arena cmask
 * @sch: scheduler whose arena hosts @src
 * @src: BPF-supplied cmask, rebased to its kernel address
 * @ref: output ref
 *
 * Snapshot @src's @base, @nr_cids and @alloc_words. The snapshot is necessary
 * because BPF may mutate the live header asynchronously.
 *
 * Return 0 on success, -EINVAL if the range is out of bounds or @alloc_words
 * doesn't cover it.
 */
ಪೂರ್ಣಾಂಕ scx_cmask_ref_init(ರಚನೆ scx_sched *sch, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src,
		       ರಚನೆ scx_cmask_ref *ref)
{
	u32 ತಳ, nr_cids, alloc_ಪದಗಳು, npossible = ಅಂಕೆ_possible_cpus();
	s32 *cid_ಗಮ್ಯ_shard;

	ತಳ = READ_ONCE(src->ತಳ);
	nr_cids = READ_ONCE(src->nr_cids);
	alloc_ಪದಗಳು = READ_ONCE(src->alloc_ಪದಗಳು);

	ಆದರೆ (unlikely(ತಳ >= npossible || nr_cids > npossible - ತಳ ||
		     SCX_CMASK_NR_ಪದಗಳು(nr_cids) > alloc_ಪದಗಳು))
		ಹಿಂತಿರುಗಿ -EINVAL;

	ref->sch = sch;
	ref->src = (ರಚನೆ scx_cmask *)src;
	ref->ತಳ = ತಳ;
	ref->nr_cids = nr_cids;

	cid_ಗಮ್ಯ_shard = rcu_dereference_all(scx_cid_ಗಮ್ಯ_shard);
	ref->shard_ಮೊದಲ = cid_ಗಮ್ಯ_shard[ತಳ];
	ಆದರೆ (likely(nr_cids))
		ref->shard_ಕೊನೆ = cid_ಗಮ್ಯ_shard[ತಳ + nr_cids - 1] + 1;
	ಇಲ್ಲದಿದ್ದರೆ
		ref->shard_ಕೊನೆ = ref->shard_ಮೊದಲ;

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * scx_cmask_ref_init_kern - Bind a scx_cmask_ref to a kernel-owned cmask
 * @sch: scheduler the cmask belongs to
 * @m: kernel address of the target cmask, storage sized for @nr_cids at @base
 * @base: first cid of the active range
 * @nr_cids: active range length
 * @ref: output ref
 *
 * Like scx_cmask_ref_init() but the geometry is supplied by the caller, not
 * read from @m's header, so a concurrent BPF write to the header can't steer
 * later sizing or offsets. Rewrite the header from the trusted geometry and
 * bind @ref to it.
 */
ಶೂನ್ಯ scx_cmask_ref_init_kern(ರಚನೆ scx_sched *sch, ರಚನೆ scx_cmask *m,
			     u32 ತಳ, u32 nr_cids, ರಚನೆ scx_cmask_ref *ref)
{
	s32 *cid_ಗಮ್ಯ_shard;

	WRITE_ONCE(m->ತಳ, ತಳ);
	WRITE_ONCE(m->nr_cids, nr_cids);
	WRITE_ONCE(m->alloc_ಪದಗಳು, SCX_CMASK_NR_ಪದಗಳು(nr_cids));

	ref->sch = sch;
	ref->src = m;
	ref->ತಳ = ತಳ;
	ref->nr_cids = nr_cids;

	cid_ಗಮ್ಯ_shard = rcu_dereference_all(scx_cid_ಗಮ್ಯ_shard);
	ref->shard_ಮೊದಲ = cid_ಗಮ್ಯ_shard[ತಳ];
	ಆದರೆ (likely(nr_cids))
		ref->shard_ಕೊನೆ = cid_ಗಮ್ಯ_shard[ತಳ + nr_cids - 1] + 1;
	ಇಲ್ಲದಿದ್ದರೆ
		ref->shard_ಕೊನೆ = ref->shard_ಮೊದಲ;
}

/**
 * scx_cmask_ref_shard - Read one shard from @ref into @out
 * @ref: validated ref
 * @shard_idx: target shard, in [@ref->shard_first, @ref->shard_end)
 * @out: output cmask whose @out->alloc_words must hold the shard
 *
 * Set @out to the intersection of @ref's range with @shard_idx's cid range,
 * with bits[] read from @ref->src via READ_ONCE. Empty intersection sets
 * @out->nr_cids to 0. scx_error()s on @ref's sched if @out can't hold the
 * shard.
 */
ಶೂನ್ಯ scx_cmask_ref_shard(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref, s32 shard_idx,
			 ರಚನೆ scx_cmask *ಹೊರಹೋಗು)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cid_shard *shard =
		&rcu_dereference_all(scx_cid_shard_ranges)[shard_idx];
	u32 shard_ತಳ = shard->ತಳ_cid;
	u32 shard_ಕೊನೆ = shard_ತಳ + shard->nr_cids;
	u32 isect_ತಳ, isect_ಕೊನೆ, nr_ಪದಗಳು, src_off, wi;
	u64 ತಲೆ_ಮುಖವಾಡ, ಬಾಲ_ಮುಖವಾಡ;

	isect_ತಳ = max(ref->ತಳ, shard_ತಳ);
	isect_ಕೊನೆ = min(ref->ತಳ + ref->nr_cids, shard_ಕೊನೆ);

	ಆದರೆ (isect_ತಳ >= isect_ಕೊನೆ) {
		ಹೊರಹೋಗು->ತಳ = shard_ತಳ;
		ಹೊರಹೋಗು->nr_cids = 0;
		ಹಿಂತಿರುಗಿ;
	}

	nr_ಪದಗಳು = ((isect_ಕೊನೆ - 1) / 64) - (isect_ತಳ / 64) + 1;
	ಆದರೆ (nr_ಪದಗಳು > ಹೊರಹೋಗು->alloc_ಪದಗಳು) {
		scx_error(ref->sch, "scx_cmask_ref_shard: out alloc_words=%u < %u for shard %d",
			  ಹೊರಹೋಗು->alloc_ಪದಗಳು, nr_ಪದಗಳು, shard_idx);
		ಹೊರಹೋಗು->ತಳ = shard_ತಳ;
		ಹೊರಹೋಗು->nr_cids = 0;
		ಹಿಂತಿರುಗಿ;
	}

	ಹೊರಹೋಗು->ತಳ = isect_ತಳ;
	ಹೊರಹೋಗು->nr_cids = isect_ಕೊನೆ - isect_ತಳ;
	src_off = (isect_ತಳ / 64) - (ref->ತಳ / 64);

	ಗಾಗಿ (wi = 0; wi < nr_ಪದಗಳು; wi++)
		ಹೊರಹೋಗು->ಬಿಟ್ಟುಗಳು[wi] = READ_ONCE(ref->src->ಬಿಟ್ಟುಗಳು[src_off + wi]);

	ತಲೆ_ಮುಖವಾಡ = GENMASK_U64(63, isect_ತಳ & 63);
	ಹೊರಹೋಗು->ಬಿಟ್ಟುಗಳು[0] &= ತಲೆ_ಮುಖವಾಡ;
	ಬಾಲ_ಮುಖವಾಡ = GENMASK_U64((isect_ಕೊನೆ - 1) & 63, 0);
	ಹೊರಹೋಗು->ಬಿಟ್ಟುಗಳು[nr_ಪದಗಳು - 1] &= ಬಾಲ_ಮುಖವಾಡ;
}

/**
 * scx_cmask_ref_or - OR @src into the arena cmask referenced by @ref
 * @ref: validated ref
 * @src: stable kernel cmask
 *
 * Bits inside the intersection of @ref's snapshotted range with @src's range
 * are OR'd into @ref->src and bits outside are left unchanged. Stores on
 * @ref->src use WRITE_ONCE since BPF may read/write concurrently.
 */
ಶೂನ್ಯ scx_cmask_ref_or(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src)
{
	cmask_walk_op2(ref->src->ಬಿಟ್ಟುಗಳು, ref->ತಳ, ref->nr_cids,
		       src->ಬಿಟ್ಟುಗಳು, src->ತಳ, src->nr_cids, CMASK_OP2_REF_OR);
}

/**
 * scx_cmask_ref_copy - Copy @src into the arena cmask referenced by @ref
 * @ref: validated ref
 * @src: stable kernel cmask
 *
 * Bits inside the intersection of @ref's snapshotted range with @src's range
 * take @src's values and bits outside are left unchanged. Stores on @ref->src
 * use WRITE_ONCE since BPF may read/write concurrently.
 */
ಶೂನ್ಯ scx_cmask_ref_copy(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src)
{
	cmask_walk_op2(ref->src->ಬಿಟ್ಟುಗಳು, ref->ತಳ, ref->nr_cids,
		       src->ಬಿಟ್ಟುಗಳು, src->ತಳ, src->nr_cids, CMASK_OP2_REF_COPY);
}

/**
 * scx_cmask_ref_from_cpumask - Populate @ref's arena cmask from a cpumask
 * @ref: kern-bound ref, see scx_cmask_ref_init_kern()
 * @cpumask: cpus to translate into cids
 *
 * Write @ref's active range one word at a time, setting each cid's bit when
 * its cpu is in @cpumask. Offsets and length come from @ref's trusted geometry
 * and stores use WRITE_ONCE since BPF may read concurrently, so the arena
 * header is never read.
 */
ಶೂನ್ಯ scx_cmask_ref_from_cpumask(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref,
				ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpumask)
{
	ರಚನೆ scx_cmask *m = ref->src;
	u32 ತಳ = ref->ತಳ, nr_cids = ref->nr_cids;
	u32 wi, nr_ಪದಗಳು;

	ಆದರೆ (!nr_cids)
		ಹಿಂತಿರುಗಿ;

	nr_ಪದಗಳು = (ತಳ + nr_cids - 1) / 64 - ತಳ / 64 + 1;
	ಗಾಗಿ (wi = 0; wi < nr_ಪದಗಳು; wi++) {
		u32 ಶಬ್ದ_ಮೊದಲ_cid = (ತಳ / 64 + wi) * 64;
		u64 ಶಬ್ದ = 0;
		u32 ಬಿಟ್_2;

		ಗಾಗಿ (ಬಿಟ್_2 = 0; ಬಿಟ್_2 < 64; ಬಿಟ್_2++) {
			u32 cid = ಶಬ್ದ_ಮೊದಲ_cid + ಬಿಟ್_2;

			ಆದರೆ (cid < ತಳ || cid >= ತಳ + nr_cids)
				ಮುಂದುವರಿ;
			ಆದರೆ (cpumask_test_cpu(__scx_cid_to_cpu(cid), cpumask))
				ಶಬ್ದ |= ಬಿಟ್_U64(ಬಿಟ್_2);
		}
		WRITE_ONCE(m->ಬಿಟ್ಟುಗಳು[wi], ಶಬ್ದ);
	}
}

ಪೂರ್ಣಾಂಕ scx_cid_kfunc_init(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_ಗಣ_init_cids) ?:
		ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_ಗಣ_cid) ?:
		ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_TRACING, &scx_kfunc_ಗಣ_cid) ?:
		ರಿಜಿಸ್ಟರ್_btf_kfunc_id_ಗಣ(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_ಗಣ_cid);
}
