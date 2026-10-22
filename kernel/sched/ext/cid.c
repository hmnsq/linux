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
s16 __rcu *scx_cid_to_cpu_tbl;
s16 __rcu *scx_cpu_to_cid_tbl;
s32 __rcu *scx_cid_to_shard;
s32 __rcu *scx_shard_ಗಂಟು;
ಸಂರಚನಾ scx_cid_shard __rcu *scx_cid_shard_ranges;
ಸಂರಚನಾ scx_cid_topo __rcu *scx_cid_topo;

ಸ್ಥಿರ ಸಂರಚನಾ scx_cid_tables *scx_cid_tables;	/* used only during alloc/free */

#define SCX_CID_TOPO_NEG	(ಸಂರಚನಾ scx_cid_topo) {				\
	.core_cid = -1, .core_idx = -1, .llc_cid = -1, .llc_idx = -1,		\
	.ಗಂಟು_cid = -1, .ಗಂಟು_idx = -1, .shard_cid = -1, .shard_idx = -1,	\
}

/*
 * Return @cpu's LLC shared_cpu_map. If cacheinfo isn't populated (offline or
 * !present), record @cpu in @fallbacks and return its node mask instead - the
 * worst that can happen is that the cpu's LLC becomes coarser than reality.
 */
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ cpumask *cpu_llc_mask(ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ cpumask *fallbacks)
{
	ಸಂರಚನಾ cpu_cacheinfo *ci = get_cpu_cacheinfo(cpu);

	ಯದಿ (!ci || !ci->info_list || !ci->ಅಂಕೆ_leaves) {
		cpumask_set_cpu(cpu, fallbacks);
		ಪ್ರತ್ಯಾಗಮ cpumask_of_ಗಂಟು(cpu_to_ಗಂಟು(cpu));
	}
	ಪ್ರತ್ಯಾಗಮ &ci->info_list[ci->ಅಂಕೆ_leaves - 1].shared_cpu_map;
}

/*
 * Compute per-LLC shard layout. Each shard holds at most @shard_size cids, and
 * in any case no more than SCX_CID_SHARD_MAX_CPUS. Cores are spread as evenly
 * as possible across shards so cpu count is balanced: the first *@nr_large_p
 * shards get (*@cores_per_shard_p + 1) cores, the rest get *@cores_per_shard_p.
 */
ಸ್ಥಿರ ಶೂನ್ಯ calc_shard_layout(ಧ್ರುವ ಸಂರಚನಾ cpumask *llc_cpus, u32 shard_size,
			      u32 *cores_per_shard_p, u32 *nr_large_p)
{
	u32 nr_cores = 0, nr_cpus = 0, nr_shards;
	ಪೂರ್ಣಾಙ್ಕ cpu;

	for_each_cpu(cpu, llc_cpus) {
		nr_cpus++;
		ಯದಿ (cpumask_first(topology_sibling_cpumask(cpu)) == cpu)
			nr_cores++;
	}

	nr_shards = max_t(u32, 1, DIV_ROUND_UP(nr_cpus, shard_size));
	nr_shards = max_t(u32, nr_shards,
			  DIV_ROUND_UP(nr_cpus, SCX_CID_SHARD_MAX_CPUS));

	*cores_per_shard_p = nr_cores / nr_shards;
	*nr_large_p = nr_cores % nr_shards;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_cid_tables_free(ಸಂರಚನಾ scx_cid_tables *tbls)
{
	ಯದಿ (!tbls)
		ಪ್ರತ್ಯಾಗಮ;
	kvfree(tbls->cid_to_cpu);
	kvfree(tbls->cpu_to_cid);
	kvfree(tbls->cid_to_shard);
	kvfree(tbls->shard_ಗಂಟು);
	kvfree(tbls->shard_ranges);
	kvfree(tbls->topo);
	kfree(tbls);
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_cid_tables_free_rcufn(ಸಂರಚನಾ rcu_head *rcu)
{
	scx_cid_tables_free(container_of(rcu, ಸಂರಚನಾ scx_cid_tables, rcu));
}

ಸ್ಥಿರ ಸಂರಚನಾ scx_cid_tables *scx_cid_alloc_tables(ಶೂನ್ಯ)
{
	u32 npossible = ಅಂಕೆ_possible_cpus();
	ಸಂರಚನಾ scx_cid_tables *tbls;

	tbls = kzalloc_obj(*tbls);
	ಯದಿ (!tbls)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	tbls->cid_to_cpu = kvzalloc_objs(*tbls->cid_to_cpu, npossible);
	tbls->cpu_to_cid = kvzalloc_objs(*tbls->cpu_to_cid, nr_cpu_ids);
	tbls->cid_to_shard = kvzalloc_objs(*tbls->cid_to_shard, npossible);
	tbls->shard_ಗಂಟು = kvzalloc_objs(*tbls->shard_ಗಂಟು, npossible);
	tbls->shard_ranges = kvzalloc_objs(*tbls->shard_ranges, npossible);
	tbls->topo = kvzalloc_objs(*tbls->topo, npossible);

	ಯದಿ (!tbls->cid_to_cpu || !tbls->cpu_to_cid || !tbls->cid_to_shard ||
	    !tbls->shard_ಗಂಟು || !tbls->shard_ranges || !tbls->topo) {
		scx_cid_tables_free(tbls);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	ಪ್ರತ್ಯಾಗಮ tbls;
}

/**
 * scx_cid_publish_tables - Publish the tables scx_cid_init() built
 *
 * Called after ops.init_cids() where the layout is final.
 */
ಶೂನ್ಯ scx_cid_publish_tables(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);

	scx_nr_cid_shards = tbls->nr_shards;
	rcu_assign_pointer(scx_cid_to_cpu_tbl, tbls->cid_to_cpu);
	rcu_assign_pointer(scx_cpu_to_cid_tbl, tbls->cpu_to_cid);
	rcu_assign_pointer(scx_cid_to_shard, tbls->cid_to_shard);
	rcu_assign_pointer(scx_shard_ಗಂಟು, tbls->shard_ಗಂಟು);
	rcu_assign_pointer(scx_cid_shard_ranges, tbls->shard_ranges);
	rcu_assign_pointer(scx_cid_topo, tbls->topo);
}

/**
 * scx_cid_retire_tables - Unpublish and retire the cid tables
 *
 * Called by root disable after the readers which dereference without NULL
 * checks are drained, inside cpus_read_lock() to exclude the hotplug path.
 */
ಶೂನ್ಯ scx_cid_retire_tables(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);
	lockdep_assert_cpus_held();

	ಯದಿ (!tbls)
		ಪ್ರತ್ಯಾಗಮ;

	scx_cid_tables = ರಿಕ್ತ;
	RCU_INIT_POINTER(scx_cid_to_cpu_tbl, ರಿಕ್ತ);
	RCU_INIT_POINTER(scx_cpu_to_cid_tbl, ರಿಕ್ತ);
	RCU_INIT_POINTER(scx_cid_to_shard, ರಿಕ್ತ);
	RCU_INIT_POINTER(scx_shard_ಗಂಟು, ರಿಕ್ತ);
	RCU_INIT_POINTER(scx_cid_shard_ranges, ರಿಕ್ತ);
	RCU_INIT_POINTER(scx_cid_topo, ರಿಕ್ತ);
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
s32 scx_cid_init(ಸಂರಚನಾ scx_sched *sch)
{
	cpumask_var_t to_walk __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t ಗಂಟು_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t core_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_fallback __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t online_no_topo __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	ಸಂರಚನಾ scx_cid_tables *tbls;
	u32 next_cid = 0;
	s32 next_ಗಂಟು_idx = 0, next_llc_idx = 0, next_core_idx = 0;
	s32 next_shard_idx = 0;
	u32 shard_size, max_cids;
	u32 notopo_in_shard;
	s32 notopo_shard_cid, notopo_shard_idx;
	s32 cpu, cid, si;

	/* CMASK_MAX_WORDS in cid.bpf.h covers NR_CPUS up to 8192 */
	BUILD_BUG_ON(NR_CPUS > 8192);

	lockdep_assert_cpus_held();
	lockdep_assert_held(&scx_enable_mutex);

	shard_size = sch->ops.cid_shard_size ?: SCX_CID_SHARD_SIZE_DFL;
	max_cids = min_t(u32, shard_size, SCX_CID_SHARD_MAX_CPUS);

	tbls = scx_cid_alloc_tables();
	ಯದಿ (!tbls)
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	scx_cid_tables = tbls;

	ಪ್ರತಿ (si = 0; si < ಅಂಕೆ_possible_cpus(); si++)
		tbls->shard_ಗಂಟು[si] = NUMA_NO_ಗಂಟು;

	ಯದಿ (!zalloc_cpumask_var(&to_walk, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&ಗಂಟು_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&core_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_fallback, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&online_no_topo, GFP_KERNEL))
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	/* -1 sentinels for sparse-possible cpu id holes (0 is a valid cid) */
	ಪ್ರತಿ (cpu = 0; cpu < nr_cpu_ids; cpu++)
		tbls->cpu_to_cid[cpu] = -1;

	cpumask_copy(to_walk, cpu_online_mask);

	ಯಾವತ್ (!cpumask_empty(to_walk)) {
		s32 next_cpu = cpumask_first(to_walk);
		s32 nid = cpu_to_ಗಂಟು(next_cpu);
		s32 ಗಂಟು_cid = next_cid;
		s32 ಗಂಟು_idx;

		/*
		 * No NUMA info: skip and let the tail loop assign a no-topo
		 * cid. cpumask_of_node(-1) is undefined.
		 */
		ಯದಿ (nid < 0) {
			cpumask_clear_cpu(next_cpu, to_walk);
			ಅನುವರ್ತ;
		}

		ಗಂಟು_idx = next_ಗಂಟು_idx++;

		/* node_scratch = to_walk & this node */
		cpumask_and(ಗಂಟು_scratch, to_walk, cpumask_of_ಗಂಟು(nid));
		ಯದಿ (WARN_ON_ONCE(!cpumask_test_cpu(next_cpu, ಗಂಟು_scratch)))
			ಪ್ರತ್ಯಾಗಮ -EINVAL;

		ಯಾವತ್ (!cpumask_empty(ಗಂಟು_scratch)) {
			s32 ncpu = cpumask_first(ಗಂಟು_scratch);
			ಧ್ರುವ ಸಂರಚನಾ cpumask *llc_mask = cpu_llc_mask(ncpu, llc_fallback);
			s32 llc_cid = next_cid;
			s32 llc_idx = next_llc_idx++;
			u32 cores_per_shard, nr_large;
			u32 shard_local = 0, cores_in_shard = 0, cids_in_shard = 0;
			s32 shard_cid, shard_idx;

			/* llc_scratch = node_scratch & this llc */
			cpumask_and(llc_scratch, ಗಂಟು_scratch, llc_mask);
			ಯದಿ (WARN_ON_ONCE(!cpumask_test_cpu(ncpu, llc_scratch)))
				ಪ್ರತ್ಯಾಗಮ -EINVAL;

			calc_shard_layout(llc_scratch, shard_size, &cores_per_shard, &nr_large);
			shard_cid = next_cid;
			shard_idx = next_shard_idx++;
			tbls->shard_ಗಂಟು[shard_idx] = nid;

			ಯಾವತ್ (!cpumask_empty(llc_scratch)) {
				s32 lcpu = cpumask_first(llc_scratch);
				ಧ್ರುವ ಸಂರಚನಾ cpumask *sib = topology_sibling_cpumask(lcpu);
				s32 core_cid = next_cid;
				s32 core_idx = next_core_idx++;
				s32 ccpu;
				u32 max_cores, cids_in_core;

				/* core_scratch = llc_scratch & this core */
				cpumask_and(core_scratch, llc_scratch, sib);
				ಯದಿ (WARN_ON_ONCE(!cpumask_test_cpu(lcpu, core_scratch)))
					ಪ್ರತ್ಯಾಗಮ -EINVAL;

				/*
				 * Advance to a new shard when either core or
				 * cid count reaches max. The latter bounds
				 * shard sizes under uneven SMT. Never start an
				 * empty shard.
				 */
				cids_in_core = cpumask_weight(core_scratch);
				max_cores = cores_per_shard + (shard_local < nr_large ? 1 : 0);
				ಯದಿ (cores_in_shard &&
				    (cores_in_shard >= max_cores ||
				     cids_in_shard + cids_in_core > max_cids)) {
					shard_local++;
					cores_in_shard = 0;
					cids_in_shard = 0;
					shard_cid = next_cid;
					shard_idx = next_shard_idx++;
					tbls->shard_ಗಂಟು[shard_idx] = nid;
				}
				cores_in_shard++;
				cids_in_shard += cids_in_core;

				for_each_cpu(ccpu, core_scratch) {
					s32 cid = next_cid++;

					tbls->cid_to_cpu[cid] = ccpu;
					tbls->cpu_to_cid[ccpu] = cid;
					tbls->cid_to_shard[cid] = shard_idx;
					tbls->topo[cid] = (ಸಂರಚನಾ scx_cid_topo){
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
					cpumask_clear_cpu(ccpu, to_walk);
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
	notopo_in_shard = min_t(u32, shard_size, SCX_CID_SHARD_MAX_CPUS);
	notopo_shard_cid = -1;
	notopo_shard_idx = -1;

	for_each_cpu(cpu, cpu_possible_mask) {
		ಯದಿ (tbls->cpu_to_cid[cpu] != -1)
			ಅನುವರ್ತ;
		ಯದಿ (cpu_online(cpu))
			cpumask_set_cpu(cpu, online_no_topo);

		cid = next_cid++;
		tbls->cid_to_cpu[cid] = cpu;
		tbls->cpu_to_cid[cpu] = cid;

		ಯದಿ (notopo_in_shard >= min_t(u32, shard_size, SCX_CID_SHARD_MAX_CPUS)) {
			notopo_shard_cid = cid;
			notopo_shard_idx = next_shard_idx++;
			notopo_in_shard = 0;
		}
		notopo_in_shard++;

		tbls->cid_to_shard[cid] = notopo_shard_idx;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = notopo_shard_cid;
		tbls->topo[cid].shard_idx = notopo_shard_idx;
	}

	ಯದಿ (!cpumask_empty(llc_fallback))
		pr_warn("scx_cid: cpus without cacheinfo, using node mask as llc: %*pbl\n",
			cpumask_pr_args(llc_fallback));
	ಯದಿ (!cpumask_empty(online_no_topo))
		pr_warn("scx_cid: online cpus with no usable topology: %*pbl\n",
			cpumask_pr_args(online_no_topo));

	/*
	 * Fill cid_shard_ranges[] from cid_to_shard[]. Shards are contiguous
	 * cid ranges by construction: base_cid is the first cid landing in a
	 * shard, nr_cids is the count.
	 */
	ಪ್ರತಿ (cid = 0; cid < next_cid; cid++) {
		s32 sidx = tbls->cid_to_shard[cid];

		ಯದಿ (tbls->shard_ranges[sidx].nr_cids == 0)
			tbls->shard_ranges[sidx].base_cid = cid;
		tbls->shard_ranges[sidx].nr_cids++;
	}

	tbls->nr_shards = next_shard_idx;
	ಪ್ರತ್ಯಾಗಮ 0;
}

/**
 * scx_cmask_clear - Zero every bit in @m's active range
 * @m: cmask to clear
 *
 * Storage past the active range is left as is.
 */
ಶೂನ್ಯ scx_cmask_clear(ಸಂರಚನಾ scx_cmask *m)
{
	u32 nr_ಪದಗಳು;

	ಯದಿ (!m->nr_cids)
		ಪ್ರತ್ಯಾಗಮ;
	nr_ಪದಗಳು = (m->base + m->nr_cids - 1) / 64 - m->base / 64 + 1;
	memset(m->bits, 0, nr_ಪದಗಳು * ಪರಿಮಾಣ(u64));
}

/**
 * scx_cmask_fill - Set every bit in @m's active range
 * @m: cmask to fill
 *
 * Counterpart to scx_cmask_clear(). Storage past the active range is left as is.
 */
ಶೂನ್ಯ scx_cmask_fill(ಸಂರಚನಾ scx_cmask *m)
{
	u32 nr_ಪದಗಳು, head_bits, tail_bits;

	ಯದಿ (!m->nr_cids)
		ಪ್ರತ್ಯಾಗಮ;
	nr_ಪದಗಳು = (m->base + m->nr_cids - 1) / 64 - m->base / 64 + 1;
	memset(m->bits, 0xff, nr_ಪದಗಳು * ಪರಿಮಾಣ(u64));

	/* clear word-0 bits below base */
	head_bits = m->base & 63;
	ಯದಿ (head_bits)
		m->bits[0] &= ~((1ULL << head_bits) - 1);

	/* clear last-word bits at or past base + nr_cids */
	tail_bits = (m->base + m->nr_cids) & 63;
	ಯದಿ (tail_bits)
		m->bits[nr_ಪದಗಳು - 1] &= (1ULL << tail_bits) - 1;
}

/*
 * Return the index of the largest entry in @counts, or NUMA_NO_NODE if all
 * entries are zero. Ties resolve to the lowest index.
 */
ಸ್ಥಿರ s32 pick_max_ಗಂಟು(ಧ್ರುವ u32 *counts, u32 n)
{
	s32 best = NUMA_NO_ಗಂಟು;
	u32 best_ಎಣಿಕೆ = 0, i;

	ಪ್ರತಿ (i = 0; i < n; i++) {
		ಯದಿ (counts[i] > best_ಎಣಿಕೆ) {
			best_ಎಣಿಕೆ = counts[i];
			best = i;
		}
	}
	ಪ್ರತ್ಯಾಗಮ best;
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
__bpf_kfunc ಶೂನ್ಯ scx_bpf_cid_override(ಧ್ರುವ s32 *cpu_to_cid__arena, u32 cpu_to_cid_cnt,
				      ಧ್ರುವ s32 *shard_start__arena, u32 shard_start_cnt,
				      ಧ್ರುವ ಸಂರಚನಾ bpf_prog_aux *aux)
{
	cpumask_var_t seen __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	u32 *ಗಂಟು_counts __free(kfree) = ರಿಕ್ತ;
	s32 *cpu_to_cid __free(kfree) = ರಿಕ್ತ;
	s32 *shard_start __free(kfree) = ರಿಕ್ತ;
	u32 npossible = ಅಂಕೆ_possible_cpus();
	ಸಂರಚನಾ scx_cid_tables *tbls;
	ಸಂರಚನಾ scx_sched *sch;
	u32 nr_shards = shard_start_cnt;
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
	alloced = zalloc_cpumask_var(&seen, GFP_KERNEL);
	ಗಂಟು_counts = kzalloc_objs(*ಗಂಟು_counts, nr_ಗಂಟು_ids);
	ಯದಿ (cpu_to_cid_cnt == nr_cpu_ids)
		cpu_to_cid = kmemdup(cpu_to_cid__arena, cpu_to_cid_cnt * ಪರಿಮಾಣ(s32),
				     GFP_KERNEL);
	ಯದಿ (nr_shards && nr_shards <= npossible)
		shard_start = kmemdup(shard_start__arena, nr_shards * ಪರಿಮಾಣ(s32),
				      GFP_KERNEL);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಯದಿ (unlikely(!sch))
		ಪ್ರತ್ಯಾಗಮ;

	/* called from ops.init_cids(), so the tables exist and are unpublished */
	lockdep_assert_held(&scx_enable_mutex);
	tbls = scx_cid_tables;

	ಯದಿ (cpu_to_cid_cnt != nr_cpu_ids) {
		scx_error(sch, "scx_bpf_cid_override: cpu_to_cid expected %u entries, got %u",
			  nr_cpu_ids, cpu_to_cid_cnt);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (!nr_shards || nr_shards > npossible) {
		scx_error(sch, "scx_bpf_cid_override: invalid shard_start count %u",
			  nr_shards);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (!alloced || !ಗಂಟು_counts || !cpu_to_cid || !shard_start) {
		scx_error(sch, "scx_bpf_cid_override: allocation failed");
		ಪ್ರತ್ಯಾಗಮ;
	}

	/* validate shard_start[]: starts at 0, strictly increasing, in range */
	ಯದಿ (shard_start[0] != 0) {
		scx_error(sch, "scx_bpf_cid_override: shard_start[0] must be 0, got %d",
			  shard_start[0]);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಪ್ರತಿ (si = 1; si < nr_shards; si++) {
		ಯದಿ (shard_start[si] <= shard_start[si - 1]) {
			scx_error(sch, "scx_bpf_cid_override: shard_start not increasing at [%d]",
				  si);
			ಪ್ರತ್ಯಾಗಮ;
		}
		ಯದಿ (shard_start[si] >= npossible) {
			scx_error(sch, "scx_bpf_cid_override: shard_start[%d]=%d >= %u",
				  si, shard_start[si], npossible);
			ಪ್ರತ್ಯಾಗಮ;
		}
		ಯದಿ (shard_start[si] - shard_start[si - 1] > SCX_CID_SHARD_MAX_CPUS) {
			scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
				  si - 1, shard_start[si] - shard_start[si - 1],
				  SCX_CID_SHARD_MAX_CPUS);
			ಪ್ರತ್ಯಾಗಮ;
		}
	}
	ಯದಿ (npossible - shard_start[nr_shards - 1] > SCX_CID_SHARD_MAX_CPUS) {
		scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
			  nr_shards - 1, npossible - shard_start[nr_shards - 1],
			  SCX_CID_SHARD_MAX_CPUS);
		ಪ್ರತ್ಯಾಗಮ;
	}

	/* validate first so that invalid input leaves the tables untouched */
	for_each_possible_cpu(cpu) {
		s32 c = cpu_to_cid[cpu];

		ಯದಿ (!cid_valid(sch, c))
			ಪ್ರತ್ಯಾಗಮ;
		ಯದಿ (cpumask_test_and_set_cpu(c, seen)) {
			scx_error(sch, "cid %d assigned to multiple cpus", c);
			ಪ್ರತ್ಯಾಗಮ;
		}
	}

	for_each_possible_cpu(cpu) {
		s32 c = cpu_to_cid[cpu];

		tbls->cpu_to_cid[cpu] = c;
		tbls->cid_to_cpu[c] = cpu;
	}

	/*
	 * Derive shard_node[] by majority count: an overridden shard may
	 * span NUMA nodes, so assign each to the node that owns the most cpus.
	 */
	ಪ್ರತಿ (si = 0; si < nr_shards; si++) {
		u32 end = (si + 1 < nr_shards) ? shard_start[si + 1] : npossible;

		memset(ಗಂಟು_counts, 0, nr_ಗಂಟು_ids * ಪರಿಮಾಣ(*ಗಂಟು_counts));
		ಪ್ರತಿ (cid = shard_start[si]; cid < end; cid++) {
			s32 ಗಂಟು = cpu_to_ಗಂಟು(tbls->cid_to_cpu[cid]);

			ಯದಿ (numa_valid_ಗಂಟು(ಗಂಟು))
				ಗಂಟು_counts[ಗಂಟು]++;
		}
		tbls->shard_ಗಂಟು[si] = pick_max_ಗಂಟು(ಗಂಟು_counts, nr_ಗಂಟು_ids);
	}

	/*
	 * Invalidate stale topo info and install shard layout from
	 * @shard_start. Walk shards to derive shard_cid/shard_idx for each cid.
	 */
	si = 0;
	ಪ್ರತಿ (cid = 0; cid < npossible; cid++) {
		ಯದಿ (si + 1 < nr_shards && cid >= shard_start[si + 1])
			si++;
		tbls->cid_to_shard[cid] = si;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = shard_start[si];
		tbls->topo[cid].shard_idx = si;
	}

	/* Rebuild shard_ranges[] for the new layout. */
	memset(tbls->shard_ranges, 0, npossible * ಪರಿಮಾಣ(*tbls->shard_ranges));
	ಪ್ರತಿ (si = 0; si < nr_shards; si++) {
		u32 end = (si + 1 < nr_shards) ? shard_start[si + 1] : npossible;

		tbls->shard_ranges[si].base_cid = shard_start[si];
		tbls->shard_ranges[si].nr_cids = end - shard_start[si];
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
__bpf_kfunc s32 scx_bpf_cid_to_cpu(s32 cid, ಧ್ರುವ ಸಂರಚನಾ bpf_prog_aux *aux)
{
	ಸಂರಚನಾ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಯದಿ (unlikely(!sch))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;
	ಪ್ರತ್ಯಾಗಮ scx_cid_to_cpu(sch, cid);
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
__bpf_kfunc s32 scx_bpf_cpu_to_cid(s32 cpu, ಧ್ರುವ ಸಂರಚನಾ bpf_prog_aux *aux)
{
	ಸಂರಚನಾ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ಯದಿ (unlikely(!sch))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;
	ಪ್ರತ್ಯಾಗಮ scx_cpu_to_cid(sch, cpu);
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
ಗಣನಾ cmask_op2 {
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

ಸ್ಥಿರ __always_inline bool cmask_op2_is_pred(ಧ್ರುವ ಗಣನಾ cmask_op2 op)
{
	ಪ್ರತ್ಯಾಗಮ op == CMASK_OP2_SUBSET || op == CMASK_OP2_INTERSECTS;
}

ಸ್ಥಿರ __always_inline bool cmask_word_op2(u64 *av, ಧ್ರುವ u64 *bp, u64 mask,
					   ಧ್ರುವ ಗಣನಾ cmask_op2 op)
{
	ವಿಕಲ್ಪ (op) {
	ಪಕ್ಷ CMASK_OP2_AND:
		WRITE_ONCE(*av, *av & (~mask | READ_ONCE(*bp)));
		ಪ್ರತ್ಯಾಗಮ false;
	ಪಕ್ಷ CMASK_OP2_OR:
		WRITE_ONCE(*av, *av | (READ_ONCE(*bp) & mask));
		ಪ್ರತ್ಯಾಗಮ false;
	ಪಕ್ಷ CMASK_OP2_COPY:
		WRITE_ONCE(*av, (*av & ~mask) | (READ_ONCE(*bp) & mask));
		ಪ್ರತ್ಯಾಗಮ false;
	ಪಕ್ಷ CMASK_OP2_ANDNOT:
		WRITE_ONCE(*av, *av & ~(READ_ONCE(*bp) & mask));
		ಪ್ರತ್ಯಾಗಮ false;
	ಪಕ್ಷ CMASK_OP2_SUBSET:
		/* stop on the first bit in @sub not set in @super */
		ಪ್ರತ್ಯಾಗಮ (READ_ONCE(*bp) & ~READ_ONCE(*av)) & mask;
	ಪಕ್ಷ CMASK_OP2_INTERSECTS:
		ಪ್ರತ್ಯಾಗಮ (READ_ONCE(*av) & READ_ONCE(*bp)) & mask;
	ಪಕ್ಷ CMASK_OP2_REF_OR:
		WRITE_ONCE(*av, READ_ONCE(*av) | (READ_ONCE(*bp) & mask));
		ಪ್ರತ್ಯಾಗಮ false;
	ಪಕ್ಷ CMASK_OP2_REF_COPY:
		WRITE_ONCE(*av, (READ_ONCE(*av) & ~mask) | (READ_ONCE(*bp) & mask));
		ಪ್ರತ್ಯಾಗಮ false;
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
ಸ್ಥಿರ __always_inline bool cmask_walk_op2(u64 *a_bits, u32 a_base, u32 a_nr_cids,
					   ಧ್ರುವ u64 *b_bits, u32 b_base, u32 b_nr_cids,
					   ಧ್ರುವ ಗಣನಾ cmask_op2 op)
{
	u32 lo = max(a_base, b_base);
	u32 hi = min(a_base + a_nr_cids, b_base + b_nr_cids);
	u32 a_word_off = a_base / 64;
	u32 b_word_off = b_base / 64;
	u32 lo_word = lo / 64;
	u32 hi_word = (hi - 1) / 64;
	u64 head_mask = GENMASK_U64(63, lo & 63);
	u64 tail_mask = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ಯದಿ (lo >= hi)
		ಪ್ರತ್ಯಾಗಮ false;

	ಯದಿ (lo_word == hi_word)
		ಪ್ರತ್ಯಾಗಮ cmask_word_op2(&a_bits[lo_word - a_word_off],
				      &b_bits[lo_word - b_word_off],
				      head_mask & tail_mask, op);

	ಯದಿ (cmask_word_op2(&a_bits[lo_word - a_word_off],
			   &b_bits[lo_word - b_word_off], head_mask, op) &&
	    cmask_op2_is_pred(op))
		ಪ್ರತ್ಯಾಗಮ true;

	ಪ್ರತಿ (w = lo_word + 1; w < hi_word; w++)
		ಯದಿ (cmask_word_op2(&a_bits[w - a_word_off],
				   &b_bits[w - b_word_off], ~0ULL, op) &&
		    cmask_op2_is_pred(op))
			ಪ್ರತ್ಯಾಗಮ true;

	ಪ್ರತ್ಯಾಗಮ cmask_word_op2(&a_bits[hi_word - a_word_off],
			      &b_bits[hi_word - b_word_off], tail_mask, op);
}

ಗಣನಾ cmask_op1 {
	CMASK_OP1_ANY_SET,
};

ಸ್ಥಿರ __always_inline bool cmask_word_op1(ಧ್ರುವ u64 *ap, u64 mask,
					   ಧ್ರುವ ಗಣನಾ cmask_op1 op)
{
	ವಿಕಲ್ಪ (op) {
	ಪಕ್ಷ CMASK_OP1_ANY_SET:
		ಪ್ರತ್ಯಾಗಮ READ_ONCE(*ap) & mask;
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
ಸ್ಥಿರ __always_inline bool cmask_walk_op1(ಧ್ರುವ u64 *a_bits, u32 a_base,
					   u32 a_nr_cids,
					   ಧ್ರುವ ಗಣನಾ cmask_op1 op)
{
	u32 lo = a_base;
	u32 hi = a_base + a_nr_cids;
	u32 a_word_off = a_base / 64;
	u32 lo_word = lo / 64;
	u32 hi_word = (hi - 1) / 64;
	u64 head_mask = GENMASK_U64(63, lo & 63);
	u64 tail_mask = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ಯದಿ (lo >= hi)
		ಪ್ರತ್ಯಾಗಮ false;

	ಯದಿ (lo_word == hi_word)
		ಪ್ರತ್ಯಾಗಮ cmask_word_op1(&a_bits[lo_word - a_word_off],
				      head_mask & tail_mask, op);

	ಯದಿ (cmask_word_op1(&a_bits[lo_word - a_word_off], head_mask, op))
		ಪ್ರತ್ಯಾಗಮ true;
	ಪ್ರತಿ (w = lo_word + 1; w < hi_word; w++)
		ಯದಿ (cmask_word_op1(&a_bits[w - a_word_off], ~0ULL, op))
			ಪ್ರತ್ಯಾಗಮ true;
	ಪ್ರತ್ಯಾಗಮ cmask_word_op1(&a_bits[hi_word - a_word_off], tail_mask, op);
}

ಶೂನ್ಯ scx_cmask_and(ಸಂರಚನಾ scx_cmask *dst, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *src)
{
	cmask_walk_op2(dst->bits, dst->base, dst->nr_cids,
		       src->bits, src->base, src->nr_cids, CMASK_OP2_AND);
}

ಶೂನ್ಯ scx_cmask_or(ಸಂರಚನಾ scx_cmask *dst, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *src)
{
	cmask_walk_op2(dst->bits, dst->base, dst->nr_cids,
		       src->bits, src->base, src->nr_cids, CMASK_OP2_OR);
}

ಶೂನ್ಯ scx_cmask_copy(ಸಂರಚನಾ scx_cmask *dst, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *src)
{
	cmask_walk_op2(dst->bits, dst->base, dst->nr_cids,
		       src->bits, src->base, src->nr_cids, CMASK_OP2_COPY);
}

ಶೂನ್ಯ scx_cmask_andnot(ಸಂರಚನಾ scx_cmask *dst, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *src)
{
	cmask_walk_op2(dst->bits, dst->base, dst->nr_cids,
		       src->bits, src->base, src->nr_cids, CMASK_OP2_ANDNOT);
}

/*
 * Return true if @cm has any bit set in [@lo, @hi). Caller must ensure
 * [@lo, @hi) is contained in @cm's range.
 */
ಸ್ಥಿರ bool cmask_any_set_in_range(ಧ್ರುವ ಸಂರಚನಾ scx_cmask *cm, u32 lo, u32 hi)
{
	ಯದಿ (lo >= hi)
		ಪ್ರತ್ಯಾಗಮ false;
	ಪ್ರತ್ಯಾಗಮ cmask_walk_op1(&cm->bits[lo / 64 - cm->base / 64], lo, hi - lo,
			      CMASK_OP1_ANY_SET);
}

/**
 * scx_cmask_subset - test whether @sub is a subset of @super
 * @sub: cmask to test
 * @super: cmask to test against
 *
 * Return true iff every set bit of @sub is also set in @super.
 */
bool scx_cmask_subset(ಧ್ರುವ ಸಂರಚನಾ scx_cmask *sub, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *super)
{
	u32 super_end = super->base + super->nr_cids;
	u32 sub_end = sub->base + sub->nr_cids;

	/*
	 * Set bits in @sub outside @super's range can't be in @super, so any
	 * such bit means not a subset. The walk below only visits words
	 * common to both ranges, so these need a separate scan.
	 */
	ಯದಿ (sub->base < super->base &&
	    cmask_any_set_in_range(sub, sub->base, min(super->base, sub_end)))
		ಪ್ರತ್ಯಾಗಮ false;
	ಯದಿ (sub_end > super_end &&
	    cmask_any_set_in_range(sub, max(sub->base, super_end), sub_end))
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ !cmask_walk_op2((u64 *)super->bits, super->base, super->nr_cids,
			       sub->bits, sub->base, sub->nr_cids, CMASK_OP2_SUBSET);
}

bool scx_cmask_intersects(ಧ್ರುವ ಸಂರಚನಾ scx_cmask *a, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *b)
{
	ಪ್ರತ್ಯಾಗಮ cmask_walk_op2((u64 *)a->bits, a->base, a->nr_cids,
			      b->bits, b->base, b->nr_cids, CMASK_OP2_INTERSECTS);
}

/**
 * scx_cmask_empty - Test whether @m has no bits set
 * @m: cmask to test
 *
 * Return true iff @m's active range has no bits set.
 */
bool scx_cmask_empty(ಧ್ರುವ ಸಂರಚನಾ scx_cmask *m)
{
	ಪ್ರತ್ಯಾಗಮ !cmask_any_set_in_range(m, m->base, m->base + m->nr_cids);
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
__bpf_kfunc ಶೂನ್ಯ scx_bpf_cid_topo(s32 cid, ಸಂರಚನಾ scx_cid_topo *out, size_t out__sz,
				  ಧ್ರುವ ಸಂರಚನಾ bpf_prog_aux *aux)
{
	size_t len = min(out__sz, ಪರಿಮಾಣ(*out));
	ಸಂರಚನಾ scx_cid_topo *topo;
	ಸಂರಚನಾ scx_sched *sch;

	/* the error cases and fields the kernel lacks read as -1 */
	memset(out, 0xff, out__sz);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	topo = rcu_dereference(scx_cid_topo);
	ಯದಿ (unlikely(!sch) || !cid_valid(sch, cid) || unlikely(!topo))
		ಪ್ರತ್ಯಾಗಮ;

	memcpy(out, &topo[cid], len);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_init_cids)
BTF_ID_FLAGS(func, scx_bpf_cid_override, KF_IMPLICIT_ARGS | KF_SLEEPABLE)
BTF_KFUNCS_END(scx_kfunc_ids_init_cids)

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ btf_kfunc_id_set scx_kfunc_set_init_cids = {
	.owner	= THIS_MODULE,
	.set	= &scx_kfunc_ids_init_cids,
	.filter	= scx_kfunc_context_filter,
};

BTF_KFUNCS_START(scx_kfunc_ids_cid)
BTF_ID_FLAGS(func, scx_bpf_cid_to_cpu, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cpu_to_cid, KF_IMPLICIT_ARGS)
BTF_ID_FLAGS(func, scx_bpf_cid_topo, KF_IMPLICIT_ARGS)
BTF_KFUNCS_END(scx_kfunc_ids_cid)

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ btf_kfunc_id_set scx_kfunc_set_cid = {
	.owner	= THIS_MODULE,
	.set	= &scx_kfunc_ids_cid,
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
ಪೂರ್ಣಾಙ್ಕ scx_cmask_ref_init(ಸಂರಚನಾ scx_sched *sch, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *src,
		       ಸಂರಚನಾ scx_cmask_ref *ref)
{
	u32 base, nr_cids, alloc_ಪದಗಳು, npossible = ಅಂಕೆ_possible_cpus();
	s32 *cid_to_shard;

	base = READ_ONCE(src->base);
	nr_cids = READ_ONCE(src->nr_cids);
	alloc_ಪದಗಳು = READ_ONCE(src->alloc_ಪದಗಳು);

	ಯದಿ (unlikely(base >= npossible || nr_cids > npossible - base ||
		     SCX_CMASK_NR_ಪದಗಳು(nr_cids) > alloc_ಪದಗಳು))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	ref->sch = sch;
	ref->src = (ಸಂರಚನಾ scx_cmask *)src;
	ref->base = base;
	ref->nr_cids = nr_cids;

	cid_to_shard = rcu_dereference_all(scx_cid_to_shard);
	ref->shard_first = cid_to_shard[base];
	ಯದಿ (likely(nr_cids))
		ref->shard_end = cid_to_shard[base + nr_cids - 1] + 1;
	ಅನ್ಯಥಾ
		ref->shard_end = ref->shard_first;

	ಪ್ರತ್ಯಾಗಮ 0;
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
ಶೂನ್ಯ scx_cmask_ref_init_kern(ಸಂರಚನಾ scx_sched *sch, ಸಂರಚನಾ scx_cmask *m,
			     u32 base, u32 nr_cids, ಸಂರಚನಾ scx_cmask_ref *ref)
{
	s32 *cid_to_shard;

	WRITE_ONCE(m->base, base);
	WRITE_ONCE(m->nr_cids, nr_cids);
	WRITE_ONCE(m->alloc_ಪದಗಳು, SCX_CMASK_NR_ಪದಗಳು(nr_cids));

	ref->sch = sch;
	ref->src = m;
	ref->base = base;
	ref->nr_cids = nr_cids;

	cid_to_shard = rcu_dereference_all(scx_cid_to_shard);
	ref->shard_first = cid_to_shard[base];
	ಯದಿ (likely(nr_cids))
		ref->shard_end = cid_to_shard[base + nr_cids - 1] + 1;
	ಅನ್ಯಥಾ
		ref->shard_end = ref->shard_first;
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
ಶೂನ್ಯ scx_cmask_ref_shard(ಧ್ರುವ ಸಂರಚನಾ scx_cmask_ref *ref, s32 shard_idx,
			 ಸಂರಚನಾ scx_cmask *out)
{
	ಧ್ರುವ ಸಂರಚನಾ scx_cid_shard *shard =
		&rcu_dereference_all(scx_cid_shard_ranges)[shard_idx];
	u32 shard_base = shard->base_cid;
	u32 shard_end = shard_base + shard->nr_cids;
	u32 isect_base, isect_end, nr_ಪದಗಳು, src_off, wi;
	u64 head_mask, tail_mask;

	isect_base = max(ref->base, shard_base);
	isect_end = min(ref->base + ref->nr_cids, shard_end);

	ಯದಿ (isect_base >= isect_end) {
		out->base = shard_base;
		out->nr_cids = 0;
		ಪ್ರತ್ಯಾಗಮ;
	}

	nr_ಪದಗಳು = ((isect_end - 1) / 64) - (isect_base / 64) + 1;
	ಯದಿ (nr_ಪದಗಳು > out->alloc_ಪದಗಳು) {
		scx_error(ref->sch, "scx_cmask_ref_shard: out alloc_words=%u < %u for shard %d",
			  out->alloc_ಪದಗಳು, nr_ಪದಗಳು, shard_idx);
		out->base = shard_base;
		out->nr_cids = 0;
		ಪ್ರತ್ಯಾಗಮ;
	}

	out->base = isect_base;
	out->nr_cids = isect_end - isect_base;
	src_off = (isect_base / 64) - (ref->base / 64);

	ಪ್ರತಿ (wi = 0; wi < nr_ಪದಗಳು; wi++)
		out->bits[wi] = READ_ONCE(ref->src->bits[src_off + wi]);

	head_mask = GENMASK_U64(63, isect_base & 63);
	out->bits[0] &= head_mask;
	tail_mask = GENMASK_U64((isect_end - 1) & 63, 0);
	out->bits[nr_ಪದಗಳು - 1] &= tail_mask;
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
ಶೂನ್ಯ scx_cmask_ref_or(ಧ್ರುವ ಸಂರಚನಾ scx_cmask_ref *ref, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *src)
{
	cmask_walk_op2(ref->src->bits, ref->base, ref->nr_cids,
		       src->bits, src->base, src->nr_cids, CMASK_OP2_REF_OR);
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
ಶೂನ್ಯ scx_cmask_ref_copy(ಧ್ರುವ ಸಂರಚನಾ scx_cmask_ref *ref, ಧ್ರುವ ಸಂರಚನಾ scx_cmask *src)
{
	cmask_walk_op2(ref->src->bits, ref->base, ref->nr_cids,
		       src->bits, src->base, src->nr_cids, CMASK_OP2_REF_COPY);
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
ಶೂನ್ಯ scx_cmask_ref_from_cpumask(ಧ್ರುವ ಸಂರಚನಾ scx_cmask_ref *ref,
				ಧ್ರುವ ಸಂರಚನಾ cpumask *cpumask)
{
	ಸಂರಚನಾ scx_cmask *m = ref->src;
	u32 base = ref->base, nr_cids = ref->nr_cids;
	u32 wi, nr_ಪದಗಳು;

	ಯದಿ (!nr_cids)
		ಪ್ರತ್ಯಾಗಮ;

	nr_ಪದಗಳು = (base + nr_cids - 1) / 64 - base / 64 + 1;
	ಪ್ರತಿ (wi = 0; wi < nr_ಪದಗಳು; wi++) {
		u32 word_first_cid = (base / 64 + wi) * 64;
		u64 word = 0;
		u32 bit;

		ಪ್ರತಿ (bit = 0; bit < 64; bit++) {
			u32 cid = word_first_cid + bit;

			ಯದಿ (cid < base || cid >= base + nr_cids)
				ಅನುವರ್ತ;
			ಯದಿ (cpumask_test_cpu(__scx_cid_to_cpu(cid), cpumask))
				word |= BIT_U64(bit);
		}
		WRITE_ONCE(m->bits[wi], word);
	}
}

ಪೂರ್ಣಾಙ್ಕ scx_cid_kfunc_init(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_set_init_cids) ?:
		register_btf_kfunc_id_set(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_set_cid) ?:
		register_btf_kfunc_id_set(BPF_PROG_TYPE_TRACING, &scx_kfunc_set_cid) ?:
		register_btf_kfunc_id_set(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_set_cid);
}
