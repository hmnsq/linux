/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cacheinfo.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cid.h"

/*
 * cid tables. The cid kfuncs are available whether the root scheduler is
 * cid-form or cpu-form, the latter to allow gradual migration to cids, so every
 * root builds a default mapping. Each root enable allocates a fresh set, builds
 * it privately and publishes the __rcu globals below once the layout is final.
 * Root disable unpublishes and RCU-frees the set. kfuncs may run before the
 * tables are published and must check for NULL.
 */
u32 scx_nr_cid_shards;
s16 __rcu *scx_cid_ਮੰਜ਼ਿਲ_cpu_tbl;
s16 __rcu *scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl;
s32 __rcu *scx_cid_ਮੰਜ਼ਿਲ_shard;
s32 __rcu *scx_shard_ਗੰਢ;
ਬਣਤਰ scx_cid_shard __rcu *scx_cid_shard_ranges;
ਬਣਤਰ scx_cid_topo __rcu *scx_cid_topo;

ਸਥਿਰ ਬਣਤਰ scx_cid_tables *scx_cid_tables;	/* used only during alloc/free */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCX_CID_TOPO_NEG	(ਬਣਤਰ scx_cid_topo) {				\
	.core_cid = -1, .core_idx = -1, .llc_cid = -1, .llc_idx = -1,		\
	.ਗੰਢ_cid = -1, .ਗੰਢ_idx = -1, .shard_cid = -1, .shard_idx = -1,	\
}

/*
 * Return @cpu's LLC shared_cpu_map. If cacheinfo isn't populated (offline or
 * !present), record @cpu in @fallbacks and return its node mask instead - the
 * worst that can happen is that the cpu's LLC becomes coarser than reality.
 */
ਸਥਿਰ ਅਚਲ ਬਣਤਰ cpumask *cpu_llc_ਨਕਾਬ(ਪੂਰਨ cpu, ਬਣਤਰ cpumask *fallbacks)
{
	ਬਣਤਰ cpu_cacheinfo *ci = get_cpu_cacheinfo(cpu);

	ਜੇ (!ci || !ci->info_list || !ci->ਅੰਕ_leaves) {
		cpumask_ਸੈੱਟ_cpu(cpu, fallbacks);
		ਵਾਪਸ cpumask_ਦਾ_ਗੰਢ(cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu));
	}
	ਵਾਪਸ &ci->info_list[ci->ਅੰਕ_leaves - 1].shared_cpu_map;
}

/*
 * Compute per-LLC shard layout. Each shard holds at most @shard_size cids, and
 * in any case no more than SCX_CID_SHARD_MAX_CPUS. Cores are spread as evenly
 * as possible across shards so cpu count is balanced: the first *@nr_large_p
 * shards get (*@cores_per_shard_p + 1) cores, the rest get *@cores_per_shard_p.
 */
ਸਥਿਰ ਖਾਲੀ calc_shard_layout(ਅਚਲ ਬਣਤਰ cpumask *llc_cpus, u32 shard_ਮਾਪ,
			      u32 *cores_ਪ੍ਰਤੀ_shard_p, u32 *nr_large_p)
{
	u32 nr_cores = 0, nr_cpus = 0, nr_shards;
	ਪੂਰਨ cpu;

	for_each_cpu(cpu, llc_cpus) {
		nr_cpus++;
		ਜੇ (cpumask_ਪਹਿਲਾ(topology_sibling_cpumask(cpu)) == cpu)
			nr_cores++;
	}

	nr_shards = max_t(u32, 1, DIV_ਗੇੜਾ_ਉੱਪਰ(nr_cpus, shard_ਮਾਪ));
	nr_shards = max_t(u32, nr_shards,
			  DIV_ਗੇੜਾ_ਉੱਪਰ(nr_cpus, SCX_CID_SHARD_MAX_CPUS));

	*cores_ਪ੍ਰਤੀ_shard_p = nr_cores / nr_shards;
	*nr_large_p = nr_cores % nr_shards;
}

ਸਥਿਰ ਖਾਲੀ scx_cid_tables_free(ਬਣਤਰ scx_cid_tables *tbls)
{
	ਜੇ (!tbls)
		ਵਾਪਸ;
	kvfree(tbls->cid_ਮੰਜ਼ਿਲ_cpu);
	kvfree(tbls->cpu_ਮੰਜ਼ਿਲ_cid);
	kvfree(tbls->cid_ਮੰਜ਼ਿਲ_shard);
	kvfree(tbls->shard_ਗੰਢ);
	kvfree(tbls->shard_ranges);
	kvfree(tbls->topo);
	kfree(tbls);
}

ਸਥਿਰ ਖਾਲੀ scx_cid_tables_free_rcufn(ਬਣਤਰ rcu_ਸਿਰ *rcu)
{
	scx_cid_tables_free(container_ਦਾ(rcu, ਬਣਤਰ scx_cid_tables, rcu));
}

ਸਥਿਰ ਬਣਤਰ scx_cid_tables *scx_cid_alloc_tables(ਖਾਲੀ)
{
	u32 npossible = ਅੰਕ_possible_cpus();
	ਬਣਤਰ scx_cid_tables *tbls;

	tbls = kzalloc_obj(*tbls);
	ਜੇ (!tbls)
		ਵਾਪਸ ਨਿਰਾ;

	tbls->cid_ਮੰਜ਼ਿਲ_cpu = kvzalloc_objs(*tbls->cid_ਮੰਜ਼ਿਲ_cpu, npossible);
	tbls->cpu_ਮੰਜ਼ਿਲ_cid = kvzalloc_objs(*tbls->cpu_ਮੰਜ਼ਿਲ_cid, nr_cpu_ids);
	tbls->cid_ਮੰਜ਼ਿਲ_shard = kvzalloc_objs(*tbls->cid_ਮੰਜ਼ਿਲ_shard, npossible);
	tbls->shard_ਗੰਢ = kvzalloc_objs(*tbls->shard_ਗੰਢ, npossible);
	tbls->shard_ranges = kvzalloc_objs(*tbls->shard_ranges, npossible);
	tbls->topo = kvzalloc_objs(*tbls->topo, npossible);

	ਜੇ (!tbls->cid_ਮੰਜ਼ਿਲ_cpu || !tbls->cpu_ਮੰਜ਼ਿਲ_cid || !tbls->cid_ਮੰਜ਼ਿਲ_shard ||
	    !tbls->shard_ਗੰਢ || !tbls->shard_ranges || !tbls->topo) {
		scx_cid_tables_free(tbls);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ tbls;
}

/**
 * scx_cid_publish_tables - Publish the tables scx_cid_init() built
 *
 * Called after ops.init_cids() where the layout is final.
 */
ਖਾਲੀ scx_cid_publish_tables(ਖਾਲੀ)
{
	ਬਣਤਰ scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);

	scx_nr_cid_shards = tbls->nr_shards;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(scx_cid_ਮੰਜ਼ਿਲ_cpu_tbl, tbls->cid_ਮੰਜ਼ਿਲ_cpu);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl, tbls->cpu_ਮੰਜ਼ਿਲ_cid);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(scx_cid_ਮੰਜ਼ਿਲ_shard, tbls->cid_ਮੰਜ਼ਿਲ_shard);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(scx_shard_ਗੰਢ, tbls->shard_ਗੰਢ);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(scx_cid_shard_ranges, tbls->shard_ranges);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(scx_cid_topo, tbls->topo);
}

/**
 * scx_cid_retire_tables - Unpublish and retire the cid tables
 *
 * Called by root disable after the readers which dereference without NULL
 * checks are drained, inside cpus_read_lock() to exclude the hotplug path.
 */
ਖਾਲੀ scx_cid_retire_tables(ਖਾਲੀ)
{
	ਬਣਤਰ scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);
	lockdep_assert_cpus_held();

	ਜੇ (!tbls)
		ਵਾਪਸ;

	scx_cid_tables = ਨਿਰਾ;
	RCU_INIT_ਸੂਚਕ(scx_cid_ਮੰਜ਼ਿਲ_cpu_tbl, ਨਿਰਾ);
	RCU_INIT_ਸੂਚਕ(scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl, ਨਿਰਾ);
	RCU_INIT_ਸੂਚਕ(scx_cid_ਮੰਜ਼ਿਲ_shard, ਨਿਰਾ);
	RCU_INIT_ਸੂਚਕ(scx_shard_ਗੰਢ, ਨਿਰਾ);
	RCU_INIT_ਸੂਚਕ(scx_cid_shard_ranges, ਨਿਰਾ);
	RCU_INIT_ਸੂਚਕ(scx_cid_topo, ਨਿਰਾ);
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
s32 scx_cid_init(ਬਣਤਰ scx_sched *sch)
{
	cpumask_var_t ਮੰਜ਼ਿਲ_walk __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t ਗੰਢ_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t core_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_fallback __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t online_no_topo __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	ਬਣਤਰ scx_cid_tables *tbls;
	u32 ਅਗਲਾ_cid = 0;
	s32 ਅਗਲਾ_ਗੰਢ_idx = 0, ਅਗਲਾ_llc_idx = 0, ਅਗਲਾ_core_idx = 0;
	s32 ਅਗਲਾ_shard_idx = 0;
	u32 shard_ਮਾਪ, max_cids;
	u32 notopo_ਵਿੱਚ_shard;
	s32 notopo_shard_cid, notopo_shard_idx;
	s32 cpu, cid, si;

	/* CMASK_MAX_WORDS in cid.bpf.h covers NR_CPUS up to 8192 */
	BUILD_BUG_ON(NR_CPUS > 8192);

	lockdep_assert_cpus_held();
	lockdep_assert_held(&scx_enable_mutex);

	shard_ਮਾਪ = sch->ops.cid_shard_ਮਾਪ ?: SCX_CID_SHARD_ਮਾਪ_DFL;
	max_cids = min_t(u32, shard_ਮਾਪ, SCX_CID_SHARD_MAX_CPUS);

	tbls = scx_cid_alloc_tables();
	ਜੇ (!tbls)
		ਵਾਪਸ -ENOMEM;

	scx_cid_tables = tbls;

	ਲਈ (si = 0; si < ਅੰਕ_possible_cpus(); si++)
		tbls->shard_ਗੰਢ[si] = NUMA_NO_ਗੰਢ;

	ਜੇ (!zalloc_cpumask_var(&ਮੰਜ਼ਿਲ_walk, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&ਗੰਢ_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&core_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_fallback, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&online_no_topo, GFP_KERNEL))
		ਵਾਪਸ -ENOMEM;

	/* -1 sentinels for sparse-possible cpu id holes (0 is a valid cid) */
	ਲਈ (cpu = 0; cpu < nr_cpu_ids; cpu++)
		tbls->cpu_ਮੰਜ਼ਿਲ_cid[cpu] = -1;

	cpumask_copy(ਮੰਜ਼ਿਲ_walk, cpu_online_ਨਕਾਬ);

	ਜਦੋਂ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(ਮੰਜ਼ਿਲ_walk)) {
		s32 ਅਗਲਾ_cpu = cpumask_ਪਹਿਲਾ(ਮੰਜ਼ਿਲ_walk);
		s32 nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(ਅਗਲਾ_cpu);
		s32 ਗੰਢ_cid = ਅਗਲਾ_cid;
		s32 ਗੰਢ_idx;

		/*
		 * No NUMA info: skip and let the tail loop assign a no-topo
		 * cid. cpumask_of_node(-1) is undefined.
		 */
		ਜੇ (nid < 0) {
			cpumask_clear_cpu(ਅਗਲਾ_cpu, ਮੰਜ਼ਿਲ_walk);
			ਜਾਰੀ;
		}

		ਗੰਢ_idx = ਅਗਲਾ_ਗੰਢ_idx++;

		/* node_scratch = to_walk & this node */
		cpumask_and(ਗੰਢ_scratch, ਮੰਜ਼ਿਲ_walk, cpumask_ਦਾ_ਗੰਢ(nid));
		ਜੇ (WARN_ON_ONCE(!cpumask_test_cpu(ਅਗਲਾ_cpu, ਗੰਢ_scratch)))
			ਵਾਪਸ -EINVAL;

		ਜਦੋਂ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(ਗੰਢ_scratch)) {
			s32 ncpu = cpumask_ਪਹਿਲਾ(ਗੰਢ_scratch);
			ਅਚਲ ਬਣਤਰ cpumask *llc_ਨਕਾਬ = cpu_llc_ਨਕਾਬ(ncpu, llc_fallback);
			s32 llc_cid = ਅਗਲਾ_cid;
			s32 llc_idx = ਅਗਲਾ_llc_idx++;
			u32 cores_ਪ੍ਰਤੀ_shard, nr_large;
			u32 shard_local = 0, cores_ਵਿੱਚ_shard = 0, cids_ਵਿੱਚ_shard = 0;
			s32 shard_cid, shard_idx;

			/* llc_scratch = node_scratch & this llc */
			cpumask_and(llc_scratch, ਗੰਢ_scratch, llc_ਨਕਾਬ);
			ਜੇ (WARN_ON_ONCE(!cpumask_test_cpu(ncpu, llc_scratch)))
				ਵਾਪਸ -EINVAL;

			calc_shard_layout(llc_scratch, shard_ਮਾਪ, &cores_ਪ੍ਰਤੀ_shard, &nr_large);
			shard_cid = ਅਗਲਾ_cid;
			shard_idx = ਅਗਲਾ_shard_idx++;
			tbls->shard_ਗੰਢ[shard_idx] = nid;

			ਜਦੋਂ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(llc_scratch)) {
				s32 lcpu = cpumask_ਪਹਿਲਾ(llc_scratch);
				ਅਚਲ ਬਣਤਰ cpumask *sib = topology_sibling_cpumask(lcpu);
				s32 core_cid = ਅਗਲਾ_cid;
				s32 core_idx = ਅਗਲਾ_core_idx++;
				s32 ccpu;
				u32 max_cores, cids_ਵਿੱਚ_core;

				/* core_scratch = llc_scratch & this core */
				cpumask_and(core_scratch, llc_scratch, sib);
				ਜੇ (WARN_ON_ONCE(!cpumask_test_cpu(lcpu, core_scratch)))
					ਵਾਪਸ -EINVAL;

				/*
				 * Advance to a new shard when either core or
				 * cid count reaches max. The latter bounds
				 * shard sizes under uneven SMT. Never start an
				 * empty shard.
				 */
				cids_ਵਿੱਚ_core = cpumask_ਭਾਰ(core_scratch);
				max_cores = cores_ਪ੍ਰਤੀ_shard + (shard_local < nr_large ? 1 : 0);
				ਜੇ (cores_ਵਿੱਚ_shard &&
				    (cores_ਵਿੱਚ_shard >= max_cores ||
				     cids_ਵਿੱਚ_shard + cids_ਵਿੱਚ_core > max_cids)) {
					shard_local++;
					cores_ਵਿੱਚ_shard = 0;
					cids_ਵਿੱਚ_shard = 0;
					shard_cid = ਅਗਲਾ_cid;
					shard_idx = ਅਗਲਾ_shard_idx++;
					tbls->shard_ਗੰਢ[shard_idx] = nid;
				}
				cores_ਵਿੱਚ_shard++;
				cids_ਵਿੱਚ_shard += cids_ਵਿੱਚ_core;

				for_each_cpu(ccpu, core_scratch) {
					s32 cid = ਅਗਲਾ_cid++;

					tbls->cid_ਮੰਜ਼ਿਲ_cpu[cid] = ccpu;
					tbls->cpu_ਮੰਜ਼ਿਲ_cid[ccpu] = cid;
					tbls->cid_ਮੰਜ਼ਿਲ_shard[cid] = shard_idx;
					tbls->topo[cid] = (ਬਣਤਰ scx_cid_topo){
						.core_cid = core_cid,
						.core_idx = core_idx,
						.llc_cid = llc_cid,
						.llc_idx = llc_idx,
						.ਗੰਢ_cid = ਗੰਢ_cid,
						.ਗੰਢ_idx = ਗੰਢ_idx,
						.shard_cid = shard_cid,
						.shard_idx = shard_idx,
					};

					cpumask_clear_cpu(ccpu, llc_scratch);
					cpumask_clear_cpu(ccpu, ਗੰਢ_scratch);
					cpumask_clear_cpu(ccpu, ਮੰਜ਼ਿਲ_walk);
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
	notopo_ਵਿੱਚ_shard = min_t(u32, shard_ਮਾਪ, SCX_CID_SHARD_MAX_CPUS);
	notopo_shard_cid = -1;
	notopo_shard_idx = -1;

	for_each_cpu(cpu, cpu_possible_ਨਕਾਬ) {
		ਜੇ (tbls->cpu_ਮੰਜ਼ਿਲ_cid[cpu] != -1)
			ਜਾਰੀ;
		ਜੇ (cpu_online(cpu))
			cpumask_ਸੈੱਟ_cpu(cpu, online_no_topo);

		cid = ਅਗਲਾ_cid++;
		tbls->cid_ਮੰਜ਼ਿਲ_cpu[cid] = cpu;
		tbls->cpu_ਮੰਜ਼ਿਲ_cid[cpu] = cid;

		ਜੇ (notopo_ਵਿੱਚ_shard >= min_t(u32, shard_ਮਾਪ, SCX_CID_SHARD_MAX_CPUS)) {
			notopo_shard_cid = cid;
			notopo_shard_idx = ਅਗਲਾ_shard_idx++;
			notopo_ਵਿੱਚ_shard = 0;
		}
		notopo_ਵਿੱਚ_shard++;

		tbls->cid_ਮੰਜ਼ਿਲ_shard[cid] = notopo_shard_idx;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = notopo_shard_cid;
		tbls->topo[cid].shard_idx = notopo_shard_idx;
	}

	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(llc_fallback))
		pr_warn("scx_cid: cpus without cacheinfo, using node mask as llc: %*pbl\n",
			cpumask_pr_ਦਲੀਲਾਂ(llc_fallback));
	ਜੇ (!cpumask_ਖਾਲੀ_ਥਾਵਾਂ(online_no_topo))
		pr_warn("scx_cid: online cpus with no usable topology: %*pbl\n",
			cpumask_pr_ਦਲੀਲਾਂ(online_no_topo));

	/*
	 * Fill cid_shard_ranges[] from cid_to_shard[]. Shards are contiguous
	 * cid ranges by construction: base_cid is the first cid landing in a
	 * shard, nr_cids is the count.
	 */
	ਲਈ (cid = 0; cid < ਅਗਲਾ_cid; cid++) {
		s32 sidx = tbls->cid_ਮੰਜ਼ਿਲ_shard[cid];

		ਜੇ (tbls->shard_ranges[sidx].nr_cids == 0)
			tbls->shard_ranges[sidx].ਅਧਾਰ_cid = cid;
		tbls->shard_ranges[sidx].nr_cids++;
	}

	tbls->nr_shards = ਅਗਲਾ_shard_idx;
	ਵਾਪਸ 0;
}

/**
 * scx_cmask_clear - Zero every bit in @m's active range
 * @m: cmask to clear
 *
 * Storage past the active range is left as is.
 */
ਖਾਲੀ scx_cmask_clear(ਬਣਤਰ scx_cmask *m)
{
	u32 nr_ਸ਼ਬਦ;

	ਜੇ (!m->nr_cids)
		ਵਾਪਸ;
	nr_ਸ਼ਬਦ = (m->ਅਧਾਰ + m->nr_cids - 1) / 64 - m->ਅਧਾਰ / 64 + 1;
	memset(m->ਬਿੱਟ, 0, nr_ਸ਼ਬਦ * ਆਕਾਰ(u64));
}

/**
 * scx_cmask_fill - Set every bit in @m's active range
 * @m: cmask to fill
 *
 * Counterpart to scx_cmask_clear(). Storage past the active range is left as is.
 */
ਖਾਲੀ scx_cmask_fill(ਬਣਤਰ scx_cmask *m)
{
	u32 nr_ਸ਼ਬਦ, ਸਿਰ_ਬਿੱਟ, ਪੂਛ_ਬਿੱਟ;

	ਜੇ (!m->nr_cids)
		ਵਾਪਸ;
	nr_ਸ਼ਬਦ = (m->ਅਧਾਰ + m->nr_cids - 1) / 64 - m->ਅਧਾਰ / 64 + 1;
	memset(m->ਬਿੱਟ, 0xff, nr_ਸ਼ਬਦ * ਆਕਾਰ(u64));

	/* clear word-0 bits below base */
	ਸਿਰ_ਬਿੱਟ = m->ਅਧਾਰ & 63;
	ਜੇ (ਸਿਰ_ਬਿੱਟ)
		m->ਬਿੱਟ[0] &= ~((1ULL << ਸਿਰ_ਬਿੱਟ) - 1);

	/* clear last-word bits at or past base + nr_cids */
	ਪੂਛ_ਬਿੱਟ = (m->ਅਧਾਰ + m->nr_cids) & 63;
	ਜੇ (ਪੂਛ_ਬਿੱਟ)
		m->ਬਿੱਟ[nr_ਸ਼ਬਦ - 1] &= (1ULL << ਪੂਛ_ਬਿੱਟ) - 1;
}

/*
 * Return the index of the largest entry in @counts, or NUMA_NO_NODE if all
 * entries are zero. Ties resolve to the lowest index.
 */
ਸਥਿਰ s32 pick_max_ਗੰਢ(ਅਚਲ u32 *counts, u32 n)
{
	s32 ਉੱਤਮ = NUMA_NO_ਗੰਢ;
	u32 ਉੱਤਮ_ਗਿਣਤੀ = 0, i;

	ਲਈ (i = 0; i < n; i++) {
		ਜੇ (counts[i] > ਉੱਤਮ_ਗਿਣਤੀ) {
			ਉੱਤਮ_ਗਿਣਤੀ = counts[i];
			ਉੱਤਮ = i;
		}
	}
	ਵਾਪਸ ਉੱਤਮ;
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
__bpf_kfunc ਖਾਲੀ scx_bpf_cid_override(ਅਚਲ s32 *cpu_ਮੰਜ਼ਿਲ_cid_arena, u32 cpu_ਮੰਜ਼ਿਲ_cid_cnt,
				      ਅਚਲ s32 *shard_ਸ਼ੁਰੂ_arena, u32 shard_ਸ਼ੁਰੂ_cnt,
				      ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	cpumask_var_t ਵੇਖਿਆ __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	u32 *ਗੰਢ_counts __free(kfree) = ਨਿਰਾ;
	s32 *cpu_ਮੰਜ਼ਿਲ_cid __free(kfree) = ਨਿਰਾ;
	s32 *shard_ਸ਼ੁਰੂ __free(kfree) = ਨਿਰਾ;
	u32 npossible = ਅੰਕ_possible_cpus();
	ਬਣਤਰ scx_cid_tables *tbls;
	ਬਣਤਰ scx_sched *sch;
	u32 nr_shards = shard_ਸ਼ੁਰੂ_cnt;
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
	alloced = zalloc_cpumask_var(&ਵੇਖਿਆ, GFP_KERNEL);
	ਗੰਢ_counts = kzalloc_objs(*ਗੰਢ_counts, nr_ਗੰਢ_ids);
	ਜੇ (cpu_ਮੰਜ਼ਿਲ_cid_cnt == nr_cpu_ids)
		cpu_ਮੰਜ਼ਿਲ_cid = kmemdup(cpu_ਮੰਜ਼ਿਲ_cid_arena, cpu_ਮੰਜ਼ਿਲ_cid_cnt * ਆਕਾਰ(s32),
				     GFP_KERNEL);
	ਜੇ (nr_shards && nr_shards <= npossible)
		shard_ਸ਼ੁਰੂ = kmemdup(shard_ਸ਼ੁਰੂ_arena, nr_shards * ਆਕਾਰ(s32),
				      GFP_KERNEL);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ;

	/* called from ops.init_cids(), so the tables exist and are unpublished */
	lockdep_assert_held(&scx_enable_mutex);
	tbls = scx_cid_tables;

	ਜੇ (cpu_ਮੰਜ਼ਿਲ_cid_cnt != nr_cpu_ids) {
		scx_error(sch, "scx_bpf_cid_override: cpu_to_cid expected %u entries, got %u",
			  nr_cpu_ids, cpu_ਮੰਜ਼ਿਲ_cid_cnt);
		ਵਾਪਸ;
	}

	ਜੇ (!nr_shards || nr_shards > npossible) {
		scx_error(sch, "scx_bpf_cid_override: invalid shard_start count %u",
			  nr_shards);
		ਵਾਪਸ;
	}

	ਜੇ (!alloced || !ਗੰਢ_counts || !cpu_ਮੰਜ਼ਿਲ_cid || !shard_ਸ਼ੁਰੂ) {
		scx_error(sch, "scx_bpf_cid_override: allocation failed");
		ਵਾਪਸ;
	}

	/* validate shard_start[]: starts at 0, strictly increasing, in range */
	ਜੇ (shard_ਸ਼ੁਰੂ[0] != 0) {
		scx_error(sch, "scx_bpf_cid_override: shard_start[0] must be 0, got %d",
			  shard_ਸ਼ੁਰੂ[0]);
		ਵਾਪਸ;
	}
	ਲਈ (si = 1; si < nr_shards; si++) {
		ਜੇ (shard_ਸ਼ੁਰੂ[si] <= shard_ਸ਼ੁਰੂ[si - 1]) {
			scx_error(sch, "scx_bpf_cid_override: shard_start not increasing at [%d]",
				  si);
			ਵਾਪਸ;
		}
		ਜੇ (shard_ਸ਼ੁਰੂ[si] >= npossible) {
			scx_error(sch, "scx_bpf_cid_override: shard_start[%d]=%d >= %u",
				  si, shard_ਸ਼ੁਰੂ[si], npossible);
			ਵਾਪਸ;
		}
		ਜੇ (shard_ਸ਼ੁਰੂ[si] - shard_ਸ਼ੁਰੂ[si - 1] > SCX_CID_SHARD_MAX_CPUS) {
			scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
				  si - 1, shard_ਸ਼ੁਰੂ[si] - shard_ਸ਼ੁਰੂ[si - 1],
				  SCX_CID_SHARD_MAX_CPUS);
			ਵਾਪਸ;
		}
	}
	ਜੇ (npossible - shard_ਸ਼ੁਰੂ[nr_shards - 1] > SCX_CID_SHARD_MAX_CPUS) {
		scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
			  nr_shards - 1, npossible - shard_ਸ਼ੁਰੂ[nr_shards - 1],
			  SCX_CID_SHARD_MAX_CPUS);
		ਵਾਪਸ;
	}

	/* validate first so that invalid input leaves the tables untouched */
	for_each_possible_cpu(cpu) {
		s32 c = cpu_ਮੰਜ਼ਿਲ_cid[cpu];

		ਜੇ (!cid_valid(sch, c))
			ਵਾਪਸ;
		ਜੇ (cpumask_test_and_ਸੈੱਟ_cpu(c, ਵੇਖਿਆ)) {
			scx_error(sch, "cid %d assigned to multiple cpus", c);
			ਵਾਪਸ;
		}
	}

	for_each_possible_cpu(cpu) {
		s32 c = cpu_ਮੰਜ਼ਿਲ_cid[cpu];

		tbls->cpu_ਮੰਜ਼ਿਲ_cid[cpu] = c;
		tbls->cid_ਮੰਜ਼ਿਲ_cpu[c] = cpu;
	}

	/*
	 * Derive shard_node[] by majority count: an overridden shard may
	 * span NUMA nodes, so assign each to the node that owns the most cpus.
	 */
	ਲਈ (si = 0; si < nr_shards; si++) {
		u32 ਸਿਰਾ = (si + 1 < nr_shards) ? shard_ਸ਼ੁਰੂ[si + 1] : npossible;

		memset(ਗੰਢ_counts, 0, nr_ਗੰਢ_ids * ਆਕਾਰ(*ਗੰਢ_counts));
		ਲਈ (cid = shard_ਸ਼ੁਰੂ[si]; cid < ਸਿਰਾ; cid++) {
			s32 ਗੰਢ = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(tbls->cid_ਮੰਜ਼ਿਲ_cpu[cid]);

			ਜੇ (numa_valid_ਗੰਢ(ਗੰਢ))
				ਗੰਢ_counts[ਗੰਢ]++;
		}
		tbls->shard_ਗੰਢ[si] = pick_max_ਗੰਢ(ਗੰਢ_counts, nr_ਗੰਢ_ids);
	}

	/*
	 * Invalidate stale topo info and install shard layout from
	 * @shard_start. Walk shards to derive shard_cid/shard_idx for each cid.
	 */
	si = 0;
	ਲਈ (cid = 0; cid < npossible; cid++) {
		ਜੇ (si + 1 < nr_shards && cid >= shard_ਸ਼ੁਰੂ[si + 1])
			si++;
		tbls->cid_ਮੰਜ਼ਿਲ_shard[cid] = si;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = shard_ਸ਼ੁਰੂ[si];
		tbls->topo[cid].shard_idx = si;
	}

	/* Rebuild shard_ranges[] for the new layout. */
	memset(tbls->shard_ranges, 0, npossible * ਆਕਾਰ(*tbls->shard_ranges));
	ਲਈ (si = 0; si < nr_shards; si++) {
		u32 ਸਿਰਾ = (si + 1 < nr_shards) ? shard_ਸ਼ੁਰੂ[si + 1] : npossible;

		tbls->shard_ranges[si].ਅਧਾਰ_cid = shard_ਸ਼ੁਰੂ[si];
		tbls->shard_ranges[si].nr_cids = ਸਿਰਾ - shard_ਸ਼ੁਰੂ[si];
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
__bpf_kfunc s32 scx_bpf_cid_ਮੰਜ਼ਿਲ_cpu(s32 cid, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -EINVAL;
	ਵਾਪਸ scx_cid_ਮੰਜ਼ਿਲ_cpu(sch, cid);
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
__bpf_kfunc s32 scx_bpf_cpu_ਮੰਜ਼ਿਲ_cid(s32 cpu, ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਬਣਤਰ scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ਜੇ (unlikely(!sch))
		ਵਾਪਸ -EINVAL;
	ਵਾਪਸ scx_cpu_ਮੰਜ਼ਿਲ_cid(sch, cpu);
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
ਗਿਣਤੀ cmask_op2 {
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

ਸਥਿਰ __always_inline bool cmask_op2_is_pred(ਅਚਲ ਗਿਣਤੀ cmask_op2 op)
{
	ਵਾਪਸ op == CMASK_OP2_SUBSET || op == CMASK_OP2_INTERSECTS;
}

ਸਥਿਰ __always_inline bool cmask_ਸ਼ਬਦ_op2(u64 *av, ਅਚਲ u64 *bp, u64 ਨਕਾਬ,
					   ਅਚਲ ਗਿਣਤੀ cmask_op2 op)
{
	ਚੋਣ (op) {
	ਹਾਲਤ CMASK_OP2_AND:
		WRITE_ONCE(*av, *av & (~ਨਕਾਬ | READ_ONCE(*bp)));
		ਵਾਪਸ false;
	ਹਾਲਤ CMASK_OP2_OR:
		WRITE_ONCE(*av, *av | (READ_ONCE(*bp) & ਨਕਾਬ));
		ਵਾਪਸ false;
	ਹਾਲਤ CMASK_OP2_COPY:
		WRITE_ONCE(*av, (*av & ~ਨਕਾਬ) | (READ_ONCE(*bp) & ਨਕਾਬ));
		ਵਾਪਸ false;
	ਹਾਲਤ CMASK_OP2_ANDNOT:
		WRITE_ONCE(*av, *av & ~(READ_ONCE(*bp) & ਨਕਾਬ));
		ਵਾਪਸ false;
	ਹਾਲਤ CMASK_OP2_SUBSET:
		/* stop on the first bit in @sub not set in @super */
		ਵਾਪਸ (READ_ONCE(*bp) & ~READ_ONCE(*av)) & ਨਕਾਬ;
	ਹਾਲਤ CMASK_OP2_INTERSECTS:
		ਵਾਪਸ (READ_ONCE(*av) & READ_ONCE(*bp)) & ਨਕਾਬ;
	ਹਾਲਤ CMASK_OP2_REF_OR:
		WRITE_ONCE(*av, READ_ONCE(*av) | (READ_ONCE(*bp) & ਨਕਾਬ));
		ਵਾਪਸ false;
	ਹਾਲਤ CMASK_OP2_REF_COPY:
		WRITE_ONCE(*av, (READ_ONCE(*av) & ~ਨਕਾਬ) | (READ_ONCE(*bp) & ਨਕਾਬ));
		ਵਾਪਸ false;
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
ਸਥਿਰ __always_inline bool cmask_walk_op2(u64 *a_ਬਿੱਟ, u32 a_ਅਧਾਰ, u32 a_nr_cids,
					   ਅਚਲ u64 *b_ਬਿੱਟ, u32 b_ਅਧਾਰ, u32 b_nr_cids,
					   ਅਚਲ ਗਿਣਤੀ cmask_op2 op)
{
	u32 lo = max(a_ਅਧਾਰ, b_ਅਧਾਰ);
	u32 hi = min(a_ਅਧਾਰ + a_nr_cids, b_ਅਧਾਰ + b_nr_cids);
	u32 a_ਸ਼ਬਦ_off = a_ਅਧਾਰ / 64;
	u32 b_ਸ਼ਬਦ_off = b_ਅਧਾਰ / 64;
	u32 lo_ਸ਼ਬਦ = lo / 64;
	u32 hi_ਸ਼ਬਦ = (hi - 1) / 64;
	u64 ਸਿਰ_ਨਕਾਬ = GENMASK_U64(63, lo & 63);
	u64 ਪੂਛ_ਨਕਾਬ = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ਜੇ (lo >= hi)
		ਵਾਪਸ false;

	ਜੇ (lo_ਸ਼ਬਦ == hi_ਸ਼ਬਦ)
		ਵਾਪਸ cmask_ਸ਼ਬਦ_op2(&a_ਬਿੱਟ[lo_ਸ਼ਬਦ - a_ਸ਼ਬਦ_off],
				      &b_ਬਿੱਟ[lo_ਸ਼ਬਦ - b_ਸ਼ਬਦ_off],
				      ਸਿਰ_ਨਕਾਬ & ਪੂਛ_ਨਕਾਬ, op);

	ਜੇ (cmask_ਸ਼ਬਦ_op2(&a_ਬਿੱਟ[lo_ਸ਼ਬਦ - a_ਸ਼ਬਦ_off],
			   &b_ਬਿੱਟ[lo_ਸ਼ਬਦ - b_ਸ਼ਬਦ_off], ਸਿਰ_ਨਕਾਬ, op) &&
	    cmask_op2_is_pred(op))
		ਵਾਪਸ true;

	ਲਈ (w = lo_ਸ਼ਬਦ + 1; w < hi_ਸ਼ਬਦ; w++)
		ਜੇ (cmask_ਸ਼ਬਦ_op2(&a_ਬਿੱਟ[w - a_ਸ਼ਬਦ_off],
				   &b_ਬਿੱਟ[w - b_ਸ਼ਬਦ_off], ~0ULL, op) &&
		    cmask_op2_is_pred(op))
			ਵਾਪਸ true;

	ਵਾਪਸ cmask_ਸ਼ਬਦ_op2(&a_ਬਿੱਟ[hi_ਸ਼ਬਦ - a_ਸ਼ਬਦ_off],
			      &b_ਬਿੱਟ[hi_ਸ਼ਬਦ - b_ਸ਼ਬਦ_off], ਪੂਛ_ਨਕਾਬ, op);
}

ਗਿਣਤੀ cmask_op1 {
	CMASK_OP1_ANY_ਸੈੱਟ,
};

ਸਥਿਰ __always_inline bool cmask_ਸ਼ਬਦ_op1(ਅਚਲ u64 *ap, u64 ਨਕਾਬ,
					   ਅਚਲ ਗਿਣਤੀ cmask_op1 op)
{
	ਚੋਣ (op) {
	ਹਾਲਤ CMASK_OP1_ANY_ਸੈੱਟ:
		ਵਾਪਸ READ_ONCE(*ap) & ਨਕਾਬ;
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
ਸਥਿਰ __always_inline bool cmask_walk_op1(ਅਚਲ u64 *a_ਬਿੱਟ, u32 a_ਅਧਾਰ,
					   u32 a_nr_cids,
					   ਅਚਲ ਗਿਣਤੀ cmask_op1 op)
{
	u32 lo = a_ਅਧਾਰ;
	u32 hi = a_ਅਧਾਰ + a_nr_cids;
	u32 a_ਸ਼ਬਦ_off = a_ਅਧਾਰ / 64;
	u32 lo_ਸ਼ਬਦ = lo / 64;
	u32 hi_ਸ਼ਬਦ = (hi - 1) / 64;
	u64 ਸਿਰ_ਨਕਾਬ = GENMASK_U64(63, lo & 63);
	u64 ਪੂਛ_ਨਕਾਬ = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ਜੇ (lo >= hi)
		ਵਾਪਸ false;

	ਜੇ (lo_ਸ਼ਬਦ == hi_ਸ਼ਬਦ)
		ਵਾਪਸ cmask_ਸ਼ਬਦ_op1(&a_ਬਿੱਟ[lo_ਸ਼ਬਦ - a_ਸ਼ਬਦ_off],
				      ਸਿਰ_ਨਕਾਬ & ਪੂਛ_ਨਕਾਬ, op);

	ਜੇ (cmask_ਸ਼ਬਦ_op1(&a_ਬਿੱਟ[lo_ਸ਼ਬਦ - a_ਸ਼ਬਦ_off], ਸਿਰ_ਨਕਾਬ, op))
		ਵਾਪਸ true;
	ਲਈ (w = lo_ਸ਼ਬਦ + 1; w < hi_ਸ਼ਬਦ; w++)
		ਜੇ (cmask_ਸ਼ਬਦ_op1(&a_ਬਿੱਟ[w - a_ਸ਼ਬਦ_off], ~0ULL, op))
			ਵਾਪਸ true;
	ਵਾਪਸ cmask_ਸ਼ਬਦ_op1(&a_ਬਿੱਟ[hi_ਸ਼ਬਦ - a_ਸ਼ਬਦ_off], ਪੂਛ_ਨਕਾਬ, op);
}

ਖਾਲੀ scx_cmask_and(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src)
{
	cmask_walk_op2(dst->ਬਿੱਟ, dst->ਅਧਾਰ, dst->nr_cids,
		       src->ਬਿੱਟ, src->ਅਧਾਰ, src->nr_cids, CMASK_OP2_AND);
}

ਖਾਲੀ scx_cmask_or(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src)
{
	cmask_walk_op2(dst->ਬਿੱਟ, dst->ਅਧਾਰ, dst->nr_cids,
		       src->ਬਿੱਟ, src->ਅਧਾਰ, src->nr_cids, CMASK_OP2_OR);
}

ਖਾਲੀ scx_cmask_copy(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src)
{
	cmask_walk_op2(dst->ਬਿੱਟ, dst->ਅਧਾਰ, dst->nr_cids,
		       src->ਬਿੱਟ, src->ਅਧਾਰ, src->nr_cids, CMASK_OP2_COPY);
}

ਖਾਲੀ scx_cmask_andnot(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src)
{
	cmask_walk_op2(dst->ਬਿੱਟ, dst->ਅਧਾਰ, dst->nr_cids,
		       src->ਬਿੱਟ, src->ਅਧਾਰ, src->nr_cids, CMASK_OP2_ANDNOT);
}

/*
 * Return true if @cm has any bit set in [@lo, @hi). Caller must ensure
 * [@lo, @hi) is contained in @cm's range.
 */
ਸਥਿਰ bool cmask_any_ਸੈੱਟ_ਵਿੱਚ_range(ਅਚਲ ਬਣਤਰ scx_cmask *cm, u32 lo, u32 hi)
{
	ਜੇ (lo >= hi)
		ਵਾਪਸ false;
	ਵਾਪਸ cmask_walk_op1(&cm->ਬਿੱਟ[lo / 64 - cm->ਅਧਾਰ / 64], lo, hi - lo,
			      CMASK_OP1_ANY_ਸੈੱਟ);
}

/**
 * scx_cmask_subset - test whether @sub is a subset of @super
 * @sub: cmask to test
 * @super: cmask to test against
 *
 * Return true iff every set bit of @sub is also set in @super.
 */
bool scx_cmask_subset(ਅਚਲ ਬਣਤਰ scx_cmask *sub, ਅਚਲ ਬਣਤਰ scx_cmask *super)
{
	u32 super_ਸਿਰਾ = super->ਅਧਾਰ + super->nr_cids;
	u32 sub_ਸਿਰਾ = sub->ਅਧਾਰ + sub->nr_cids;

	/*
	 * Set bits in @sub outside @super's range can't be in @super, so any
	 * such bit means not a subset. The walk below only visits words
	 * common to both ranges, so these need a separate scan.
	 */
	ਜੇ (sub->ਅਧਾਰ < super->ਅਧਾਰ &&
	    cmask_any_ਸੈੱਟ_ਵਿੱਚ_range(sub, sub->ਅਧਾਰ, min(super->ਅਧਾਰ, sub_ਸਿਰਾ)))
		ਵਾਪਸ false;
	ਜੇ (sub_ਸਿਰਾ > super_ਸਿਰਾ &&
	    cmask_any_ਸੈੱਟ_ਵਿੱਚ_range(sub, max(sub->ਅਧਾਰ, super_ਸਿਰਾ), sub_ਸਿਰਾ))
		ਵਾਪਸ false;

	ਵਾਪਸ !cmask_walk_op2((u64 *)super->ਬਿੱਟ, super->ਅਧਾਰ, super->nr_cids,
			       sub->ਬਿੱਟ, sub->ਅਧਾਰ, sub->nr_cids, CMASK_OP2_SUBSET);
}

bool scx_cmask_intersects(ਅਚਲ ਬਣਤਰ scx_cmask *a, ਅਚਲ ਬਣਤਰ scx_cmask *b)
{
	ਵਾਪਸ cmask_walk_op2((u64 *)a->ਬਿੱਟ, a->ਅਧਾਰ, a->nr_cids,
			      b->ਬਿੱਟ, b->ਅਧਾਰ, b->nr_cids, CMASK_OP2_INTERSECTS);
}

/**
 * scx_cmask_empty - Test whether @m has no bits set
 * @m: cmask to test
 *
 * Return true iff @m's active range has no bits set.
 */
bool scx_cmask_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ scx_cmask *m)
{
	ਵਾਪਸ !cmask_any_ਸੈੱਟ_ਵਿੱਚ_range(m, m->ਅਧਾਰ, m->ਅਧਾਰ + m->nr_cids);
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
__bpf_kfunc ਖਾਲੀ scx_bpf_cid_topo(s32 cid, ਬਣਤਰ scx_cid_topo *ਬਾਹਰਲਾ, ਮਾਪ_t ਬਾਹਰਲਾ_sz,
				  ਅਚਲ ਬਣਤਰ bpf_prog_aux *aux)
{
	ਮਾਪ_t len = min(ਬਾਹਰਲਾ_sz, ਆਕਾਰ(*ਬਾਹਰਲਾ));
	ਬਣਤਰ scx_cid_topo *topo;
	ਬਣਤਰ scx_sched *sch;

	/* the error cases and fields the kernel lacks read as -1 */
	memset(ਬਾਹਰਲਾ, 0xff, ਬਾਹਰਲਾ_sz);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	topo = rcu_dereference(scx_cid_topo);
	ਜੇ (unlikely(!sch) || !cid_valid(sch, cid) || unlikely(!topo))
		ਵਾਪਸ;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਬਾਹਰਲਾ, &topo[cid], len);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_init_cids)
BTF_ID_FLAGS(func, scx_bpf_cid_override, KF_IMPLICIT_ਦਲੀਲਾਂ | KF_SLEEPABLE)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_init_cids)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_init_cids = {
	.owner	= THIS_MODULE,
	.ਸੈੱਟ	= &scx_kfunc_ids_init_cids,
	.filter	= scx_kfunc_context_filter,
};

BTF_KFUNCS_ਸ਼ੁਰੂ(scx_kfunc_ids_cid)
BTF_ID_FLAGS(func, scx_bpf_cid_ਮੰਜ਼ਿਲ_cpu, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cpu_ਮੰਜ਼ਿਲ_cid, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_ID_FLAGS(func, scx_bpf_cid_topo, KF_IMPLICIT_ਦਲੀਲਾਂ)
BTF_KFUNCS_ਸਿਰਾ(scx_kfunc_ids_cid)

ਸਥਿਰ ਅਚਲ ਬਣਤਰ btf_kfunc_id_ਸੈੱਟ scx_kfunc_ਸੈੱਟ_cid = {
	.owner	= THIS_MODULE,
	.ਸੈੱਟ	= &scx_kfunc_ids_cid,
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
ਪੂਰਨ scx_cmask_ref_init(ਬਣਤਰ scx_sched *sch, ਅਚਲ ਬਣਤਰ scx_cmask *src,
		       ਬਣਤਰ scx_cmask_ref *ref)
{
	u32 ਅਧਾਰ, nr_cids, alloc_ਸ਼ਬਦ, npossible = ਅੰਕ_possible_cpus();
	s32 *cid_ਮੰਜ਼ਿਲ_shard;

	ਅਧਾਰ = READ_ONCE(src->ਅਧਾਰ);
	nr_cids = READ_ONCE(src->nr_cids);
	alloc_ਸ਼ਬਦ = READ_ONCE(src->alloc_ਸ਼ਬਦ);

	ਜੇ (unlikely(ਅਧਾਰ >= npossible || nr_cids > npossible - ਅਧਾਰ ||
		     SCX_CMASK_NR_ਸ਼ਬਦ(nr_cids) > alloc_ਸ਼ਬਦ))
		ਵਾਪਸ -EINVAL;

	ref->sch = sch;
	ref->src = (ਬਣਤਰ scx_cmask *)src;
	ref->ਅਧਾਰ = ਅਧਾਰ;
	ref->nr_cids = nr_cids;

	cid_ਮੰਜ਼ਿਲ_shard = rcu_dereference_all(scx_cid_ਮੰਜ਼ਿਲ_shard);
	ref->shard_ਪਹਿਲਾ = cid_ਮੰਜ਼ਿਲ_shard[ਅਧਾਰ];
	ਜੇ (likely(nr_cids))
		ref->shard_ਸਿਰਾ = cid_ਮੰਜ਼ਿਲ_shard[ਅਧਾਰ + nr_cids - 1] + 1;
	ਨਹੀਂਤਾਂ
		ref->shard_ਸਿਰਾ = ref->shard_ਪਹਿਲਾ;

	ਵਾਪਸ 0;
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
ਖਾਲੀ scx_cmask_ref_init_kern(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ scx_cmask *m,
			     u32 ਅਧਾਰ, u32 nr_cids, ਬਣਤਰ scx_cmask_ref *ref)
{
	s32 *cid_ਮੰਜ਼ਿਲ_shard;

	WRITE_ONCE(m->ਅਧਾਰ, ਅਧਾਰ);
	WRITE_ONCE(m->nr_cids, nr_cids);
	WRITE_ONCE(m->alloc_ਸ਼ਬਦ, SCX_CMASK_NR_ਸ਼ਬਦ(nr_cids));

	ref->sch = sch;
	ref->src = m;
	ref->ਅਧਾਰ = ਅਧਾਰ;
	ref->nr_cids = nr_cids;

	cid_ਮੰਜ਼ਿਲ_shard = rcu_dereference_all(scx_cid_ਮੰਜ਼ਿਲ_shard);
	ref->shard_ਪਹਿਲਾ = cid_ਮੰਜ਼ਿਲ_shard[ਅਧਾਰ];
	ਜੇ (likely(nr_cids))
		ref->shard_ਸਿਰਾ = cid_ਮੰਜ਼ਿਲ_shard[ਅਧਾਰ + nr_cids - 1] + 1;
	ਨਹੀਂਤਾਂ
		ref->shard_ਸਿਰਾ = ref->shard_ਪਹਿਲਾ;
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
ਖਾਲੀ scx_cmask_ref_shard(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref, s32 shard_idx,
			 ਬਣਤਰ scx_cmask *ਬਾਹਰਲਾ)
{
	ਅਚਲ ਬਣਤਰ scx_cid_shard *shard =
		&rcu_dereference_all(scx_cid_shard_ranges)[shard_idx];
	u32 shard_ਅਧਾਰ = shard->ਅਧਾਰ_cid;
	u32 shard_ਸਿਰਾ = shard_ਅਧਾਰ + shard->nr_cids;
	u32 isect_ਅਧਾਰ, isect_ਸਿਰਾ, nr_ਸ਼ਬਦ, src_off, wi;
	u64 ਸਿਰ_ਨਕਾਬ, ਪੂਛ_ਨਕਾਬ;

	isect_ਅਧਾਰ = max(ref->ਅਧਾਰ, shard_ਅਧਾਰ);
	isect_ਸਿਰਾ = min(ref->ਅਧਾਰ + ref->nr_cids, shard_ਸਿਰਾ);

	ਜੇ (isect_ਅਧਾਰ >= isect_ਸਿਰਾ) {
		ਬਾਹਰਲਾ->ਅਧਾਰ = shard_ਅਧਾਰ;
		ਬਾਹਰਲਾ->nr_cids = 0;
		ਵਾਪਸ;
	}

	nr_ਸ਼ਬਦ = ((isect_ਸਿਰਾ - 1) / 64) - (isect_ਅਧਾਰ / 64) + 1;
	ਜੇ (nr_ਸ਼ਬਦ > ਬਾਹਰਲਾ->alloc_ਸ਼ਬਦ) {
		scx_error(ref->sch, "scx_cmask_ref_shard: out alloc_words=%u < %u for shard %d",
			  ਬਾਹਰਲਾ->alloc_ਸ਼ਬਦ, nr_ਸ਼ਬਦ, shard_idx);
		ਬਾਹਰਲਾ->ਅਧਾਰ = shard_ਅਧਾਰ;
		ਬਾਹਰਲਾ->nr_cids = 0;
		ਵਾਪਸ;
	}

	ਬਾਹਰਲਾ->ਅਧਾਰ = isect_ਅਧਾਰ;
	ਬਾਹਰਲਾ->nr_cids = isect_ਸਿਰਾ - isect_ਅਧਾਰ;
	src_off = (isect_ਅਧਾਰ / 64) - (ref->ਅਧਾਰ / 64);

	ਲਈ (wi = 0; wi < nr_ਸ਼ਬਦ; wi++)
		ਬਾਹਰਲਾ->ਬਿੱਟ[wi] = READ_ONCE(ref->src->ਬਿੱਟ[src_off + wi]);

	ਸਿਰ_ਨਕਾਬ = GENMASK_U64(63, isect_ਅਧਾਰ & 63);
	ਬਾਹਰਲਾ->ਬਿੱਟ[0] &= ਸਿਰ_ਨਕਾਬ;
	ਪੂਛ_ਨਕਾਬ = GENMASK_U64((isect_ਸਿਰਾ - 1) & 63, 0);
	ਬਾਹਰਲਾ->ਬਿੱਟ[nr_ਸ਼ਬਦ - 1] &= ਪੂਛ_ਨਕਾਬ;
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
ਖਾਲੀ scx_cmask_ref_or(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref, ਅਚਲ ਬਣਤਰ scx_cmask *src)
{
	cmask_walk_op2(ref->src->ਬਿੱਟ, ref->ਅਧਾਰ, ref->nr_cids,
		       src->ਬਿੱਟ, src->ਅਧਾਰ, src->nr_cids, CMASK_OP2_REF_OR);
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
ਖਾਲੀ scx_cmask_ref_copy(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref, ਅਚਲ ਬਣਤਰ scx_cmask *src)
{
	cmask_walk_op2(ref->src->ਬਿੱਟ, ref->ਅਧਾਰ, ref->nr_cids,
		       src->ਬਿੱਟ, src->ਅਧਾਰ, src->nr_cids, CMASK_OP2_REF_COPY);
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
ਖਾਲੀ scx_cmask_ref_from_cpumask(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref,
				ਅਚਲ ਬਣਤਰ cpumask *cpumask)
{
	ਬਣਤਰ scx_cmask *m = ref->src;
	u32 ਅਧਾਰ = ref->ਅਧਾਰ, nr_cids = ref->nr_cids;
	u32 wi, nr_ਸ਼ਬਦ;

	ਜੇ (!nr_cids)
		ਵਾਪਸ;

	nr_ਸ਼ਬਦ = (ਅਧਾਰ + nr_cids - 1) / 64 - ਅਧਾਰ / 64 + 1;
	ਲਈ (wi = 0; wi < nr_ਸ਼ਬਦ; wi++) {
		u32 ਸ਼ਬਦ_ਪਹਿਲਾ_cid = (ਅਧਾਰ / 64 + wi) * 64;
		u64 ਸ਼ਬਦ = 0;
		u32 ਦੋਆਧਾਰੀ_ਅੰਕ_2;

		ਲਈ (ਦੋਆਧਾਰੀ_ਅੰਕ_2 = 0; ਦੋਆਧਾਰੀ_ਅੰਕ_2 < 64; ਦੋਆਧਾਰੀ_ਅੰਕ_2++) {
			u32 cid = ਸ਼ਬਦ_ਪਹਿਲਾ_cid + ਦੋਆਧਾਰੀ_ਅੰਕ_2;

			ਜੇ (cid < ਅਧਾਰ || cid >= ਅਧਾਰ + nr_cids)
				ਜਾਰੀ;
			ਜੇ (cpumask_test_cpu(__scx_cid_to_cpu(cid), cpumask))
				ਸ਼ਬਦ |= ਦੋਆਧਾਰੀ_ਅੰਕ_U64(ਦੋਆਧਾਰੀ_ਅੰਕ_2);
		}
		WRITE_ONCE(m->ਬਿੱਟ[wi], ਸ਼ਬਦ);
	}
}

ਪੂਰਨ scx_cid_kfunc_init(ਖਾਲੀ)
{
	ਵਾਪਸ ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_ਸੈੱਟ_init_cids) ?:
		ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_ਸੈੱਟ_cid) ?:
		ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_TRACING, &scx_kfunc_ਸੈੱਟ_cid) ?:
		ਰਜਿਸਟਰ_btf_kfunc_id_ਸੈੱਟ(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_ਸੈੱਟ_cid);
}
