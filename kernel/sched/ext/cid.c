/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#అన్తర్భావ <linux/cacheinfo.h>

#అన్తర్భావ "internal.h"
#అన్తర్భావ "cid.h"

/*
 * cid tables. The cid kfuncs are available whether the root scheduler is
 * cid-form or cpu-form, the latter to allow gradual migration to cids, so every
 * root builds a default mapping. Each root enable allocates a fresh set, builds
 * it privately and publishes the __rcu globals below once the layout is final.
 * Root disable unpublishes and RCU-frees the set. kfuncs may run before the
 * tables are published and must check for NULL.
 */
u32 scx_nr_cid_shards;
s16 __rcu *scx_cid_గమ్యం_cpu_tbl;
s16 __rcu *scx_cpu_గమ్యం_cid_tbl;
s32 __rcu *scx_cid_గమ్యం_shard;
s32 __rcu *scx_shard_కణుపు;
నిర్మాణం scx_cid_shard __rcu *scx_cid_shard_ranges;
నిర్మాణం scx_cid_topo __rcu *scx_cid_topo;

స్థిర నిర్మాణం scx_cid_tables *scx_cid_tables;	/* used only during alloc/free */

#నిర్వచించు SCX_CID_TOPO_NEG	(నిర్మాణం scx_cid_topo) {				\
	.core_cid = -1, .core_idx = -1, .llc_cid = -1, .llc_idx = -1,		\
	.కణుపు_cid = -1, .కణుపు_idx = -1, .shard_cid = -1, .shard_idx = -1,	\
}

/*
 * Return @cpu's LLC shared_cpu_map. If cacheinfo isn't populated (offline or
 * !present), record @cpu in @fallbacks and return its node mask instead - the
 * worst that can happen is that the cpu's LLC becomes coarser than reality.
 */
స్థిర స్థిరరాశి నిర్మాణం cpumask *cpu_llc_ముసుగు(పూర్ణాంకం cpu, నిర్మాణం cpumask *fallbacks)
{
	నిర్మాణం cpu_cacheinfo *ci = get_cpu_cacheinfo(cpu);

	ఒకవేళ (!ci || !ci->info_list || !ci->అంకె_leaves) {
		cpumask_సమితి_cpu(cpu, fallbacks);
		తిరిగి cpumask_యొక్క_కణుపు(cpu_గమ్యం_కణుపు(cpu));
	}
	తిరిగి &ci->info_list[ci->అంకె_leaves - 1].shared_cpu_map;
}

/*
 * Compute per-LLC shard layout. Each shard holds at most @shard_size cids, and
 * in any case no more than SCX_CID_SHARD_MAX_CPUS. Cores are spread as evenly
 * as possible across shards so cpu count is balanced: the first *@nr_large_p
 * shards get (*@cores_per_shard_p + 1) cores, the rest get *@cores_per_shard_p.
 */
స్థిర శూన్య calc_shard_layout(స్థిరరాశి నిర్మాణం cpumask *llc_cpus, u32 shard_కొలత,
			      u32 *cores_చొప్పున_shard_p, u32 *nr_large_p)
{
	u32 nr_cores = 0, nr_cpus = 0, nr_shards;
	పూర్ణాంకం cpu;

	for_each_cpu(cpu, llc_cpus) {
		nr_cpus++;
		ఒకవేళ (cpumask_మొదటి(topology_sibling_cpumask(cpu)) == cpu)
			nr_cores++;
	}

	nr_shards = max_t(u32, 1, DIV_విడత_పైకి(nr_cpus, shard_కొలత));
	nr_shards = max_t(u32, nr_shards,
			  DIV_విడత_పైకి(nr_cpus, SCX_CID_SHARD_MAX_CPUS));

	*cores_చొప్పున_shard_p = nr_cores / nr_shards;
	*nr_large_p = nr_cores % nr_shards;
}

స్థిర శూన్య scx_cid_tables_free(నిర్మాణం scx_cid_tables *tbls)
{
	ఒకవేళ (!tbls)
		తిరిగి;
	kvfree(tbls->cid_గమ్యం_cpu);
	kvfree(tbls->cpu_గమ్యం_cid);
	kvfree(tbls->cid_గమ్యం_shard);
	kvfree(tbls->shard_కణుపు);
	kvfree(tbls->shard_ranges);
	kvfree(tbls->topo);
	kfree(tbls);
}

స్థిర శూన్య scx_cid_tables_free_rcufn(నిర్మాణం rcu_తల *rcu)
{
	scx_cid_tables_free(container_యొక్క(rcu, నిర్మాణం scx_cid_tables, rcu));
}

స్థిర నిర్మాణం scx_cid_tables *scx_cid_alloc_tables(శూన్య)
{
	u32 npossible = అంకె_possible_cpus();
	నిర్మాణం scx_cid_tables *tbls;

	tbls = kzalloc_obj(*tbls);
	ఒకవేళ (!tbls)
		తిరిగి ఖాళీ;

	tbls->cid_గమ్యం_cpu = kvzalloc_objs(*tbls->cid_గమ్యం_cpu, npossible);
	tbls->cpu_గమ్యం_cid = kvzalloc_objs(*tbls->cpu_గమ్యం_cid, nr_cpu_ids);
	tbls->cid_గమ్యం_shard = kvzalloc_objs(*tbls->cid_గమ్యం_shard, npossible);
	tbls->shard_కణుపు = kvzalloc_objs(*tbls->shard_కణుపు, npossible);
	tbls->shard_ranges = kvzalloc_objs(*tbls->shard_ranges, npossible);
	tbls->topo = kvzalloc_objs(*tbls->topo, npossible);

	ఒకవేళ (!tbls->cid_గమ్యం_cpu || !tbls->cpu_గమ్యం_cid || !tbls->cid_గమ్యం_shard ||
	    !tbls->shard_కణుపు || !tbls->shard_ranges || !tbls->topo) {
		scx_cid_tables_free(tbls);
		తిరిగి ఖాళీ;
	}

	తిరిగి tbls;
}

/**
 * scx_cid_publish_tables - Publish the tables scx_cid_init() built
 *
 * Called after ops.init_cids() where the layout is final.
 */
శూన్య scx_cid_publish_tables(శూన్య)
{
	నిర్మాణం scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);

	scx_nr_cid_shards = tbls->nr_shards;
	rcu_కేటాయించు_సూచిక(scx_cid_గమ్యం_cpu_tbl, tbls->cid_గమ్యం_cpu);
	rcu_కేటాయించు_సూచిక(scx_cpu_గమ్యం_cid_tbl, tbls->cpu_గమ్యం_cid);
	rcu_కేటాయించు_సూచిక(scx_cid_గమ్యం_shard, tbls->cid_గమ్యం_shard);
	rcu_కేటాయించు_సూచిక(scx_shard_కణుపు, tbls->shard_కణుపు);
	rcu_కేటాయించు_సూచిక(scx_cid_shard_ranges, tbls->shard_ranges);
	rcu_కేటాయించు_సూచిక(scx_cid_topo, tbls->topo);
}

/**
 * scx_cid_retire_tables - Unpublish and retire the cid tables
 *
 * Called by root disable after the readers which dereference without NULL
 * checks are drained, inside cpus_read_lock() to exclude the hotplug path.
 */
శూన్య scx_cid_retire_tables(శూన్య)
{
	నిర్మాణం scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);
	lockdep_assert_cpus_held();

	ఒకవేళ (!tbls)
		తిరిగి;

	scx_cid_tables = ఖాళీ;
	RCU_INIT_సూచిక(scx_cid_గమ్యం_cpu_tbl, ఖాళీ);
	RCU_INIT_సూచిక(scx_cpu_గమ్యం_cid_tbl, ఖాళీ);
	RCU_INIT_సూచిక(scx_cid_గమ్యం_shard, ఖాళీ);
	RCU_INIT_సూచిక(scx_shard_కణుపు, ఖాళీ);
	RCU_INIT_సూచిక(scx_cid_shard_ranges, ఖాళీ);
	RCU_INIT_సూచిక(scx_cid_topo, ఖాళీ);
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
s32 scx_cid_init(నిర్మాణం scx_sched *sch)
{
	cpumask_var_t గమ్యం_walk __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t కణుపు_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t core_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_fallback __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t online_no_topo __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	నిర్మాణం scx_cid_tables *tbls;
	u32 తదుపరి_cid = 0;
	s32 తదుపరి_కణుపు_idx = 0, తదుపరి_llc_idx = 0, తదుపరి_core_idx = 0;
	s32 తదుపరి_shard_idx = 0;
	u32 shard_కొలత, max_cids;
	u32 notopo_లోపల_shard;
	s32 notopo_shard_cid, notopo_shard_idx;
	s32 cpu, cid, si;

	/* CMASK_MAX_WORDS in cid.bpf.h covers NR_CPUS up to 8192 */
	BUILD_BUG_ON(NR_CPUS > 8192);

	lockdep_assert_cpus_held();
	lockdep_assert_held(&scx_enable_mutex);

	shard_కొలత = sch->ops.cid_shard_కొలత ?: SCX_CID_SHARD_కొలత_DFL;
	max_cids = min_t(u32, shard_కొలత, SCX_CID_SHARD_MAX_CPUS);

	tbls = scx_cid_alloc_tables();
	ఒకవేళ (!tbls)
		తిరిగి -ENOMEM;

	scx_cid_tables = tbls;

	కొరకు (si = 0; si < అంకె_possible_cpus(); si++)
		tbls->shard_కణుపు[si] = NUMA_NO_కణుపు;

	ఒకవేళ (!zalloc_cpumask_var(&గమ్యం_walk, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&కణుపు_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&core_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_fallback, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&online_no_topo, GFP_KERNEL))
		తిరిగి -ENOMEM;

	/* -1 sentinels for sparse-possible cpu id holes (0 is a valid cid) */
	కొరకు (cpu = 0; cpu < nr_cpu_ids; cpu++)
		tbls->cpu_గమ్యం_cid[cpu] = -1;

	cpumask_copy(గమ్యం_walk, cpu_online_ముసుగు);

	అయితే (!cpumask_ఖాళీలు(గమ్యం_walk)) {
		s32 తదుపరి_cpu = cpumask_మొదటి(గమ్యం_walk);
		s32 nid = cpu_గమ్యం_కణుపు(తదుపరి_cpu);
		s32 కణుపు_cid = తదుపరి_cid;
		s32 కణుపు_idx;

		/*
		 * No NUMA info: skip and let the tail loop assign a no-topo
		 * cid. cpumask_of_node(-1) is undefined.
		 */
		ఒకవేళ (nid < 0) {
			cpumask_clear_cpu(తదుపరి_cpu, గమ్యం_walk);
			కొనసాగు;
		}

		కణుపు_idx = తదుపరి_కణుపు_idx++;

		/* node_scratch = to_walk & this node */
		cpumask_and(కణుపు_scratch, గమ్యం_walk, cpumask_యొక్క_కణుపు(nid));
		ఒకవేళ (WARN_ON_ONCE(!cpumask_test_cpu(తదుపరి_cpu, కణుపు_scratch)))
			తిరిగి -EINVAL;

		అయితే (!cpumask_ఖాళీలు(కణుపు_scratch)) {
			s32 ncpu = cpumask_మొదటి(కణుపు_scratch);
			స్థిరరాశి నిర్మాణం cpumask *llc_ముసుగు = cpu_llc_ముసుగు(ncpu, llc_fallback);
			s32 llc_cid = తదుపరి_cid;
			s32 llc_idx = తదుపరి_llc_idx++;
			u32 cores_చొప్పున_shard, nr_large;
			u32 shard_local = 0, cores_లోపల_shard = 0, cids_లోపల_shard = 0;
			s32 shard_cid, shard_idx;

			/* llc_scratch = node_scratch & this llc */
			cpumask_and(llc_scratch, కణుపు_scratch, llc_ముసుగు);
			ఒకవేళ (WARN_ON_ONCE(!cpumask_test_cpu(ncpu, llc_scratch)))
				తిరిగి -EINVAL;

			calc_shard_layout(llc_scratch, shard_కొలత, &cores_చొప్పున_shard, &nr_large);
			shard_cid = తదుపరి_cid;
			shard_idx = తదుపరి_shard_idx++;
			tbls->shard_కణుపు[shard_idx] = nid;

			అయితే (!cpumask_ఖాళీలు(llc_scratch)) {
				s32 lcpu = cpumask_మొదటి(llc_scratch);
				స్థిరరాశి నిర్మాణం cpumask *sib = topology_sibling_cpumask(lcpu);
				s32 core_cid = తదుపరి_cid;
				s32 core_idx = తదుపరి_core_idx++;
				s32 ccpu;
				u32 max_cores, cids_లోపల_core;

				/* core_scratch = llc_scratch & this core */
				cpumask_and(core_scratch, llc_scratch, sib);
				ఒకవేళ (WARN_ON_ONCE(!cpumask_test_cpu(lcpu, core_scratch)))
					తిరిగి -EINVAL;

				/*
				 * Advance to a new shard when either core or
				 * cid count reaches max. The latter bounds
				 * shard sizes under uneven SMT. Never start an
				 * empty shard.
				 */
				cids_లోపల_core = cpumask_బరువు(core_scratch);
				max_cores = cores_చొప్పున_shard + (shard_local < nr_large ? 1 : 0);
				ఒకవేళ (cores_లోపల_shard &&
				    (cores_లోపల_shard >= max_cores ||
				     cids_లోపల_shard + cids_లోపల_core > max_cids)) {
					shard_local++;
					cores_లోపల_shard = 0;
					cids_లోపల_shard = 0;
					shard_cid = తదుపరి_cid;
					shard_idx = తదుపరి_shard_idx++;
					tbls->shard_కణుపు[shard_idx] = nid;
				}
				cores_లోపల_shard++;
				cids_లోపల_shard += cids_లోపల_core;

				for_each_cpu(ccpu, core_scratch) {
					s32 cid = తదుపరి_cid++;

					tbls->cid_గమ్యం_cpu[cid] = ccpu;
					tbls->cpu_గమ్యం_cid[ccpu] = cid;
					tbls->cid_గమ్యం_shard[cid] = shard_idx;
					tbls->topo[cid] = (నిర్మాణం scx_cid_topo){
						.core_cid = core_cid,
						.core_idx = core_idx,
						.llc_cid = llc_cid,
						.llc_idx = llc_idx,
						.కణుపు_cid = కణుపు_cid,
						.కణుపు_idx = కణుపు_idx,
						.shard_cid = shard_cid,
						.shard_idx = shard_idx,
					};

					cpumask_clear_cpu(ccpu, llc_scratch);
					cpumask_clear_cpu(ccpu, కణుపు_scratch);
					cpumask_clear_cpu(ccpu, గమ్యం_walk);
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
	notopo_లోపల_shard = min_t(u32, shard_కొలత, SCX_CID_SHARD_MAX_CPUS);
	notopo_shard_cid = -1;
	notopo_shard_idx = -1;

	for_each_cpu(cpu, cpu_possible_ముసుగు) {
		ఒకవేళ (tbls->cpu_గమ్యం_cid[cpu] != -1)
			కొనసాగు;
		ఒకవేళ (cpu_online(cpu))
			cpumask_సమితి_cpu(cpu, online_no_topo);

		cid = తదుపరి_cid++;
		tbls->cid_గమ్యం_cpu[cid] = cpu;
		tbls->cpu_గమ్యం_cid[cpu] = cid;

		ఒకవేళ (notopo_లోపల_shard >= min_t(u32, shard_కొలత, SCX_CID_SHARD_MAX_CPUS)) {
			notopo_shard_cid = cid;
			notopo_shard_idx = తదుపరి_shard_idx++;
			notopo_లోపల_shard = 0;
		}
		notopo_లోపల_shard++;

		tbls->cid_గమ్యం_shard[cid] = notopo_shard_idx;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = notopo_shard_cid;
		tbls->topo[cid].shard_idx = notopo_shard_idx;
	}

	ఒకవేళ (!cpumask_ఖాళీలు(llc_fallback))
		pr_warn("scx_cid: cpus without cacheinfo, using node mask as llc: %*pbl\n",
			cpumask_pr_వాదనలు(llc_fallback));
	ఒకవేళ (!cpumask_ఖాళీలు(online_no_topo))
		pr_warn("scx_cid: online cpus with no usable topology: %*pbl\n",
			cpumask_pr_వాదనలు(online_no_topo));

	/*
	 * Fill cid_shard_ranges[] from cid_to_shard[]. Shards are contiguous
	 * cid ranges by construction: base_cid is the first cid landing in a
	 * shard, nr_cids is the count.
	 */
	కొరకు (cid = 0; cid < తదుపరి_cid; cid++) {
		s32 sidx = tbls->cid_గమ్యం_shard[cid];

		ఒకవేళ (tbls->shard_ranges[sidx].nr_cids == 0)
			tbls->shard_ranges[sidx].పునాది_cid = cid;
		tbls->shard_ranges[sidx].nr_cids++;
	}

	tbls->nr_shards = తదుపరి_shard_idx;
	తిరిగి 0;
}

/**
 * scx_cmask_clear - Zero every bit in @m's active range
 * @m: cmask to clear
 *
 * Storage past the active range is left as is.
 */
శూన్య scx_cmask_clear(నిర్మాణం scx_cmask *m)
{
	u32 nr_పదాలు;

	ఒకవేళ (!m->nr_cids)
		తిరిగి;
	nr_పదాలు = (m->పునాది + m->nr_cids - 1) / 64 - m->పునాది / 64 + 1;
	memset(m->బిట్లు, 0, nr_పదాలు * పరిమాణం(u64));
}

/**
 * scx_cmask_fill - Set every bit in @m's active range
 * @m: cmask to fill
 *
 * Counterpart to scx_cmask_clear(). Storage past the active range is left as is.
 */
శూన్య scx_cmask_fill(నిర్మాణం scx_cmask *m)
{
	u32 nr_పదాలు, తల_బిట్లు, తోక_బిట్లు;

	ఒకవేళ (!m->nr_cids)
		తిరిగి;
	nr_పదాలు = (m->పునాది + m->nr_cids - 1) / 64 - m->పునాది / 64 + 1;
	memset(m->బిట్లు, 0xff, nr_పదాలు * పరిమాణం(u64));

	/* clear word-0 bits below base */
	తల_బిట్లు = m->పునాది & 63;
	ఒకవేళ (తల_బిట్లు)
		m->బిట్లు[0] &= ~((1ULL << తల_బిట్లు) - 1);

	/* clear last-word bits at or past base + nr_cids */
	తోక_బిట్లు = (m->పునాది + m->nr_cids) & 63;
	ఒకవేళ (తోక_బిట్లు)
		m->బిట్లు[nr_పదాలు - 1] &= (1ULL << తోక_బిట్లు) - 1;
}

/*
 * Return the index of the largest entry in @counts, or NUMA_NO_NODE if all
 * entries are zero. Ties resolve to the lowest index.
 */
స్థిర s32 pick_max_కణుపు(స్థిరరాశి u32 *counts, u32 n)
{
	s32 ఉత్తమ = NUMA_NO_కణుపు;
	u32 ఉత్తమ_లెక్క = 0, i;

	కొరకు (i = 0; i < n; i++) {
		ఒకవేళ (counts[i] > ఉత్తమ_లెక్క) {
			ఉత్తమ_లెక్క = counts[i];
			ఉత్తమ = i;
		}
	}
	తిరిగి ఉత్తమ;
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
__bpf_kfunc శూన్య scx_bpf_cid_override(స్థిరరాశి s32 *cpu_గమ్యం_cid_arena, u32 cpu_గమ్యం_cid_cnt,
				      స్థిరరాశి s32 *shard_ప్రారంభం_arena, u32 shard_ప్రారంభం_cnt,
				      స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	cpumask_var_t చూసిన __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	u32 *కణుపు_counts __free(kfree) = ఖాళీ;
	s32 *cpu_గమ్యం_cid __free(kfree) = ఖాళీ;
	s32 *shard_ప్రారంభం __free(kfree) = ఖాళీ;
	u32 npossible = అంకె_possible_cpus();
	నిర్మాణం scx_cid_tables *tbls;
	నిర్మాణం scx_sched *sch;
	u32 nr_shards = shard_ప్రారంభం_cnt;
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
	alloced = zalloc_cpumask_var(&చూసిన, GFP_KERNEL);
	కణుపు_counts = kzalloc_objs(*కణుపు_counts, nr_కణుపు_ids);
	ఒకవేళ (cpu_గమ్యం_cid_cnt == nr_cpu_ids)
		cpu_గమ్యం_cid = kmemdup(cpu_గమ్యం_cid_arena, cpu_గమ్యం_cid_cnt * పరిమాణం(s32),
				     GFP_KERNEL);
	ఒకవేళ (nr_shards && nr_shards <= npossible)
		shard_ప్రారంభం = kmemdup(shard_ప్రారంభం_arena, nr_shards * పరిమాణం(s32),
				      GFP_KERNEL);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి;

	/* called from ops.init_cids(), so the tables exist and are unpublished */
	lockdep_assert_held(&scx_enable_mutex);
	tbls = scx_cid_tables;

	ఒకవేళ (cpu_గమ్యం_cid_cnt != nr_cpu_ids) {
		scx_error(sch, "scx_bpf_cid_override: cpu_to_cid expected %u entries, got %u",
			  nr_cpu_ids, cpu_గమ్యం_cid_cnt);
		తిరిగి;
	}

	ఒకవేళ (!nr_shards || nr_shards > npossible) {
		scx_error(sch, "scx_bpf_cid_override: invalid shard_start count %u",
			  nr_shards);
		తిరిగి;
	}

	ఒకవేళ (!alloced || !కణుపు_counts || !cpu_గమ్యం_cid || !shard_ప్రారంభం) {
		scx_error(sch, "scx_bpf_cid_override: allocation failed");
		తిరిగి;
	}

	/* validate shard_start[]: starts at 0, strictly increasing, in range */
	ఒకవేళ (shard_ప్రారంభం[0] != 0) {
		scx_error(sch, "scx_bpf_cid_override: shard_start[0] must be 0, got %d",
			  shard_ప్రారంభం[0]);
		తిరిగి;
	}
	కొరకు (si = 1; si < nr_shards; si++) {
		ఒకవేళ (shard_ప్రారంభం[si] <= shard_ప్రారంభం[si - 1]) {
			scx_error(sch, "scx_bpf_cid_override: shard_start not increasing at [%d]",
				  si);
			తిరిగి;
		}
		ఒకవేళ (shard_ప్రారంభం[si] >= npossible) {
			scx_error(sch, "scx_bpf_cid_override: shard_start[%d]=%d >= %u",
				  si, shard_ప్రారంభం[si], npossible);
			తిరిగి;
		}
		ఒకవేళ (shard_ప్రారంభం[si] - shard_ప్రారంభం[si - 1] > SCX_CID_SHARD_MAX_CPUS) {
			scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
				  si - 1, shard_ప్రారంభం[si] - shard_ప్రారంభం[si - 1],
				  SCX_CID_SHARD_MAX_CPUS);
			తిరిగి;
		}
	}
	ఒకవేళ (npossible - shard_ప్రారంభం[nr_shards - 1] > SCX_CID_SHARD_MAX_CPUS) {
		scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
			  nr_shards - 1, npossible - shard_ప్రారంభం[nr_shards - 1],
			  SCX_CID_SHARD_MAX_CPUS);
		తిరిగి;
	}

	/* validate first so that invalid input leaves the tables untouched */
	for_each_possible_cpu(cpu) {
		s32 c = cpu_గమ్యం_cid[cpu];

		ఒకవేళ (!cid_valid(sch, c))
			తిరిగి;
		ఒకవేళ (cpumask_test_and_సమితి_cpu(c, చూసిన)) {
			scx_error(sch, "cid %d assigned to multiple cpus", c);
			తిరిగి;
		}
	}

	for_each_possible_cpu(cpu) {
		s32 c = cpu_గమ్యం_cid[cpu];

		tbls->cpu_గమ్యం_cid[cpu] = c;
		tbls->cid_గమ్యం_cpu[c] = cpu;
	}

	/*
	 * Derive shard_node[] by majority count: an overridden shard may
	 * span NUMA nodes, so assign each to the node that owns the most cpus.
	 */
	కొరకు (si = 0; si < nr_shards; si++) {
		u32 చివర = (si + 1 < nr_shards) ? shard_ప్రారంభం[si + 1] : npossible;

		memset(కణుపు_counts, 0, nr_కణుపు_ids * పరిమాణం(*కణుపు_counts));
		కొరకు (cid = shard_ప్రారంభం[si]; cid < చివర; cid++) {
			s32 కణుపు = cpu_గమ్యం_కణుపు(tbls->cid_గమ్యం_cpu[cid]);

			ఒకవేళ (numa_valid_కణుపు(కణుపు))
				కణుపు_counts[కణుపు]++;
		}
		tbls->shard_కణుపు[si] = pick_max_కణుపు(కణుపు_counts, nr_కణుపు_ids);
	}

	/*
	 * Invalidate stale topo info and install shard layout from
	 * @shard_start. Walk shards to derive shard_cid/shard_idx for each cid.
	 */
	si = 0;
	కొరకు (cid = 0; cid < npossible; cid++) {
		ఒకవేళ (si + 1 < nr_shards && cid >= shard_ప్రారంభం[si + 1])
			si++;
		tbls->cid_గమ్యం_shard[cid] = si;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = shard_ప్రారంభం[si];
		tbls->topo[cid].shard_idx = si;
	}

	/* Rebuild shard_ranges[] for the new layout. */
	memset(tbls->shard_ranges, 0, npossible * పరిమాణం(*tbls->shard_ranges));
	కొరకు (si = 0; si < nr_shards; si++) {
		u32 చివర = (si + 1 < nr_shards) ? shard_ప్రారంభం[si + 1] : npossible;

		tbls->shard_ranges[si].పునాది_cid = shard_ప్రారంభం[si];
		tbls->shard_ranges[si].nr_cids = చివర - shard_ప్రారంభం[si];
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
__bpf_kfunc s32 scx_bpf_cid_గమ్యం_cpu(s32 cid, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -EINVAL;
	తిరిగి scx_cid_గమ్యం_cpu(sch, cid);
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
__bpf_kfunc s32 scx_bpf_cpu_గమ్యం_cid(s32 cpu, స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	నిర్మాణం scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	ఒకవేళ (unlikely(!sch))
		తిరిగి -EINVAL;
	తిరిగి scx_cpu_గమ్యం_cid(sch, cpu);
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
గణన cmask_op2 {
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

స్థిర __always_inline bool cmask_op2_is_pred(స్థిరరాశి గణన cmask_op2 op)
{
	తిరిగి op == CMASK_OP2_SUBSET || op == CMASK_OP2_INTERSECTS;
}

స్థిర __always_inline bool cmask_మాట_op2(u64 *av, స్థిరరాశి u64 *bp, u64 ముసుగు,
					   స్థిరరాశి గణన cmask_op2 op)
{
	ఎంపిక (op) {
	స్థితి CMASK_OP2_AND:
		WRITE_ONCE(*av, *av & (~ముసుగు | READ_ONCE(*bp)));
		తిరిగి false;
	స్థితి CMASK_OP2_OR:
		WRITE_ONCE(*av, *av | (READ_ONCE(*bp) & ముసుగు));
		తిరిగి false;
	స్థితి CMASK_OP2_COPY:
		WRITE_ONCE(*av, (*av & ~ముసుగు) | (READ_ONCE(*bp) & ముసుగు));
		తిరిగి false;
	స్థితి CMASK_OP2_ANDNOT:
		WRITE_ONCE(*av, *av & ~(READ_ONCE(*bp) & ముసుగు));
		తిరిగి false;
	స్థితి CMASK_OP2_SUBSET:
		/* stop on the first bit in @sub not set in @super */
		తిరిగి (READ_ONCE(*bp) & ~READ_ONCE(*av)) & ముసుగు;
	స్థితి CMASK_OP2_INTERSECTS:
		తిరిగి (READ_ONCE(*av) & READ_ONCE(*bp)) & ముసుగు;
	స్థితి CMASK_OP2_REF_OR:
		WRITE_ONCE(*av, READ_ONCE(*av) | (READ_ONCE(*bp) & ముసుగు));
		తిరిగి false;
	స్థితి CMASK_OP2_REF_COPY:
		WRITE_ONCE(*av, (READ_ONCE(*av) & ~ముసుగు) | (READ_ONCE(*bp) & ముసుగు));
		తిరిగి false;
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
స్థిర __always_inline bool cmask_walk_op2(u64 *a_బిట్లు, u32 a_పునాది, u32 a_nr_cids,
					   స్థిరరాశి u64 *b_బిట్లు, u32 b_పునాది, u32 b_nr_cids,
					   స్థిరరాశి గణన cmask_op2 op)
{
	u32 lo = max(a_పునాది, b_పునాది);
	u32 hi = min(a_పునాది + a_nr_cids, b_పునాది + b_nr_cids);
	u32 a_మాట_off = a_పునాది / 64;
	u32 b_మాట_off = b_పునాది / 64;
	u32 lo_మాట = lo / 64;
	u32 hi_మాట = (hi - 1) / 64;
	u64 తల_ముసుగు = GENMASK_U64(63, lo & 63);
	u64 తోక_ముసుగు = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ఒకవేళ (lo >= hi)
		తిరిగి false;

	ఒకవేళ (lo_మాట == hi_మాట)
		తిరిగి cmask_మాట_op2(&a_బిట్లు[lo_మాట - a_మాట_off],
				      &b_బిట్లు[lo_మాట - b_మాట_off],
				      తల_ముసుగు & తోక_ముసుగు, op);

	ఒకవేళ (cmask_మాట_op2(&a_బిట్లు[lo_మాట - a_మాట_off],
			   &b_బిట్లు[lo_మాట - b_మాట_off], తల_ముసుగు, op) &&
	    cmask_op2_is_pred(op))
		తిరిగి true;

	కొరకు (w = lo_మాట + 1; w < hi_మాట; w++)
		ఒకవేళ (cmask_మాట_op2(&a_బిట్లు[w - a_మాట_off],
				   &b_బిట్లు[w - b_మాట_off], ~0ULL, op) &&
		    cmask_op2_is_pred(op))
			తిరిగి true;

	తిరిగి cmask_మాట_op2(&a_బిట్లు[hi_మాట - a_మాట_off],
			      &b_బిట్లు[hi_మాట - b_మాట_off], తోక_ముసుగు, op);
}

గణన cmask_op1 {
	CMASK_OP1_ANY_సమితి,
};

స్థిర __always_inline bool cmask_మాట_op1(స్థిరరాశి u64 *ap, u64 ముసుగు,
					   స్థిరరాశి గణన cmask_op1 op)
{
	ఎంపిక (op) {
	స్థితి CMASK_OP1_ANY_సమితి:
		తిరిగి READ_ONCE(*ap) & ముసుగు;
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
స్థిర __always_inline bool cmask_walk_op1(స్థిరరాశి u64 *a_బిట్లు, u32 a_పునాది,
					   u32 a_nr_cids,
					   స్థిరరాశి గణన cmask_op1 op)
{
	u32 lo = a_పునాది;
	u32 hi = a_పునాది + a_nr_cids;
	u32 a_మాట_off = a_పునాది / 64;
	u32 lo_మాట = lo / 64;
	u32 hi_మాట = (hi - 1) / 64;
	u64 తల_ముసుగు = GENMASK_U64(63, lo & 63);
	u64 తోక_ముసుగు = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	ఒకవేళ (lo >= hi)
		తిరిగి false;

	ఒకవేళ (lo_మాట == hi_మాట)
		తిరిగి cmask_మాట_op1(&a_బిట్లు[lo_మాట - a_మాట_off],
				      తల_ముసుగు & తోక_ముసుగు, op);

	ఒకవేళ (cmask_మాట_op1(&a_బిట్లు[lo_మాట - a_మాట_off], తల_ముసుగు, op))
		తిరిగి true;
	కొరకు (w = lo_మాట + 1; w < hi_మాట; w++)
		ఒకవేళ (cmask_మాట_op1(&a_బిట్లు[w - a_మాట_off], ~0ULL, op))
			తిరిగి true;
	తిరిగి cmask_మాట_op1(&a_బిట్లు[hi_మాట - a_మాట_off], తోక_ముసుగు, op);
}

శూన్య scx_cmask_and(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src)
{
	cmask_walk_op2(dst->బిట్లు, dst->పునాది, dst->nr_cids,
		       src->బిట్లు, src->పునాది, src->nr_cids, CMASK_OP2_AND);
}

శూన్య scx_cmask_or(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src)
{
	cmask_walk_op2(dst->బిట్లు, dst->పునాది, dst->nr_cids,
		       src->బిట్లు, src->పునాది, src->nr_cids, CMASK_OP2_OR);
}

శూన్య scx_cmask_copy(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src)
{
	cmask_walk_op2(dst->బిట్లు, dst->పునాది, dst->nr_cids,
		       src->బిట్లు, src->పునాది, src->nr_cids, CMASK_OP2_COPY);
}

శూన్య scx_cmask_andnot(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src)
{
	cmask_walk_op2(dst->బిట్లు, dst->పునాది, dst->nr_cids,
		       src->బిట్లు, src->పునాది, src->nr_cids, CMASK_OP2_ANDNOT);
}

/*
 * Return true if @cm has any bit set in [@lo, @hi). Caller must ensure
 * [@lo, @hi) is contained in @cm's range.
 */
స్థిర bool cmask_any_సమితి_లోపల_range(స్థిరరాశి నిర్మాణం scx_cmask *cm, u32 lo, u32 hi)
{
	ఒకవేళ (lo >= hi)
		తిరిగి false;
	తిరిగి cmask_walk_op1(&cm->బిట్లు[lo / 64 - cm->పునాది / 64], lo, hi - lo,
			      CMASK_OP1_ANY_సమితి);
}

/**
 * scx_cmask_subset - test whether @sub is a subset of @super
 * @sub: cmask to test
 * @super: cmask to test against
 *
 * Return true iff every set bit of @sub is also set in @super.
 */
bool scx_cmask_subset(స్థిరరాశి నిర్మాణం scx_cmask *sub, స్థిరరాశి నిర్మాణం scx_cmask *super)
{
	u32 super_చివర = super->పునాది + super->nr_cids;
	u32 sub_చివర = sub->పునాది + sub->nr_cids;

	/*
	 * Set bits in @sub outside @super's range can't be in @super, so any
	 * such bit means not a subset. The walk below only visits words
	 * common to both ranges, so these need a separate scan.
	 */
	ఒకవేళ (sub->పునాది < super->పునాది &&
	    cmask_any_సమితి_లోపల_range(sub, sub->పునాది, min(super->పునాది, sub_చివర)))
		తిరిగి false;
	ఒకవేళ (sub_చివర > super_చివర &&
	    cmask_any_సమితి_లోపల_range(sub, max(sub->పునాది, super_చివర), sub_చివర))
		తిరిగి false;

	తిరిగి !cmask_walk_op2((u64 *)super->బిట్లు, super->పునాది, super->nr_cids,
			       sub->బిట్లు, sub->పునాది, sub->nr_cids, CMASK_OP2_SUBSET);
}

bool scx_cmask_intersects(స్థిరరాశి నిర్మాణం scx_cmask *a, స్థిరరాశి నిర్మాణం scx_cmask *b)
{
	తిరిగి cmask_walk_op2((u64 *)a->బిట్లు, a->పునాది, a->nr_cids,
			      b->బిట్లు, b->పునాది, b->nr_cids, CMASK_OP2_INTERSECTS);
}

/**
 * scx_cmask_empty - Test whether @m has no bits set
 * @m: cmask to test
 *
 * Return true iff @m's active range has no bits set.
 */
bool scx_cmask_ఖాళీలు(స్థిరరాశి నిర్మాణం scx_cmask *m)
{
	తిరిగి !cmask_any_సమితి_లోపల_range(m, m->పునాది, m->పునాది + m->nr_cids);
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
__bpf_kfunc శూన్య scx_bpf_cid_topo(s32 cid, నిర్మాణం scx_cid_topo *బహిర్గతం, కొలత_t బహిర్గతం_sz,
				  స్థిరరాశి నిర్మాణం bpf_prog_aux *aux)
{
	కొలత_t len = min(బహిర్గతం_sz, పరిమాణం(*బహిర్గతం));
	నిర్మాణం scx_cid_topo *topo;
	నిర్మాణం scx_sched *sch;

	/* the error cases and fields the kernel lacks read as -1 */
	memset(బహిర్గతం, 0xff, బహిర్గతం_sz);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	topo = rcu_dereference(scx_cid_topo);
	ఒకవేళ (unlikely(!sch) || !cid_valid(sch, cid) || unlikely(!topo))
		తిరిగి;

	జ్ఞాపకం_నకలు(బహిర్గతం, &topo[cid], len);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_ప్రారంభం(scx_kfunc_ids_init_cids)
BTF_ID_FLAGS(func, scx_bpf_cid_override, KF_IMPLICIT_వాదనలు | KF_SLEEPABLE)
BTF_KFUNCS_చివర(scx_kfunc_ids_init_cids)

స్థిర స్థిరరాశి నిర్మాణం btf_kfunc_id_సమితి scx_kfunc_సమితి_init_cids = {
	.owner	= THIS_MODULE,
	.సమితి	= &scx_kfunc_ids_init_cids,
	.filter	= scx_kfunc_context_filter,
};

BTF_KFUNCS_ప్రారంభం(scx_kfunc_ids_cid)
BTF_ID_FLAGS(func, scx_bpf_cid_గమ్యం_cpu, KF_IMPLICIT_వాదనలు)
BTF_ID_FLAGS(func, scx_bpf_cpu_గమ్యం_cid, KF_IMPLICIT_వాదనలు)
BTF_ID_FLAGS(func, scx_bpf_cid_topo, KF_IMPLICIT_వాదనలు)
BTF_KFUNCS_చివర(scx_kfunc_ids_cid)

స్థిర స్థిరరాశి నిర్మాణం btf_kfunc_id_సమితి scx_kfunc_సమితి_cid = {
	.owner	= THIS_MODULE,
	.సమితి	= &scx_kfunc_ids_cid,
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
పూర్ణాంకం scx_cmask_ref_init(నిర్మాణం scx_sched *sch, స్థిరరాశి నిర్మాణం scx_cmask *src,
		       నిర్మాణం scx_cmask_ref *ref)
{
	u32 పునాది, nr_cids, alloc_పదాలు, npossible = అంకె_possible_cpus();
	s32 *cid_గమ్యం_shard;

	పునాది = READ_ONCE(src->పునాది);
	nr_cids = READ_ONCE(src->nr_cids);
	alloc_పదాలు = READ_ONCE(src->alloc_పదాలు);

	ఒకవేళ (unlikely(పునాది >= npossible || nr_cids > npossible - పునాది ||
		     SCX_CMASK_NR_పదాలు(nr_cids) > alloc_పదాలు))
		తిరిగి -EINVAL;

	ref->sch = sch;
	ref->src = (నిర్మాణం scx_cmask *)src;
	ref->పునాది = పునాది;
	ref->nr_cids = nr_cids;

	cid_గమ్యం_shard = rcu_dereference_all(scx_cid_గమ్యం_shard);
	ref->shard_మొదటి = cid_గమ్యం_shard[పునాది];
	ఒకవేళ (likely(nr_cids))
		ref->shard_చివర = cid_గమ్యం_shard[పునాది + nr_cids - 1] + 1;
	లేకపోతే
		ref->shard_చివర = ref->shard_మొదటి;

	తిరిగి 0;
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
శూన్య scx_cmask_ref_init_kern(నిర్మాణం scx_sched *sch, నిర్మాణం scx_cmask *m,
			     u32 పునాది, u32 nr_cids, నిర్మాణం scx_cmask_ref *ref)
{
	s32 *cid_గమ్యం_shard;

	WRITE_ONCE(m->పునాది, పునాది);
	WRITE_ONCE(m->nr_cids, nr_cids);
	WRITE_ONCE(m->alloc_పదాలు, SCX_CMASK_NR_పదాలు(nr_cids));

	ref->sch = sch;
	ref->src = m;
	ref->పునాది = పునాది;
	ref->nr_cids = nr_cids;

	cid_గమ్యం_shard = rcu_dereference_all(scx_cid_గమ్యం_shard);
	ref->shard_మొదటి = cid_గమ్యం_shard[పునాది];
	ఒకవేళ (likely(nr_cids))
		ref->shard_చివర = cid_గమ్యం_shard[పునాది + nr_cids - 1] + 1;
	లేకపోతే
		ref->shard_చివర = ref->shard_మొదటి;
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
శూన్య scx_cmask_ref_shard(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref, s32 shard_idx,
			 నిర్మాణం scx_cmask *బహిర్గతం)
{
	స్థిరరాశి నిర్మాణం scx_cid_shard *shard =
		&rcu_dereference_all(scx_cid_shard_ranges)[shard_idx];
	u32 shard_పునాది = shard->పునాది_cid;
	u32 shard_చివర = shard_పునాది + shard->nr_cids;
	u32 isect_పునాది, isect_చివర, nr_పదాలు, src_off, wi;
	u64 తల_ముసుగు, తోక_ముసుగు;

	isect_పునాది = max(ref->పునాది, shard_పునాది);
	isect_చివర = min(ref->పునాది + ref->nr_cids, shard_చివర);

	ఒకవేళ (isect_పునాది >= isect_చివర) {
		బహిర్గతం->పునాది = shard_పునాది;
		బహిర్గతం->nr_cids = 0;
		తిరిగి;
	}

	nr_పదాలు = ((isect_చివర - 1) / 64) - (isect_పునాది / 64) + 1;
	ఒకవేళ (nr_పదాలు > బహిర్గతం->alloc_పదాలు) {
		scx_error(ref->sch, "scx_cmask_ref_shard: out alloc_words=%u < %u for shard %d",
			  బహిర్గతం->alloc_పదాలు, nr_పదాలు, shard_idx);
		బహిర్గతం->పునాది = shard_పునాది;
		బహిర్గతం->nr_cids = 0;
		తిరిగి;
	}

	బహిర్గతం->పునాది = isect_పునాది;
	బహిర్గతం->nr_cids = isect_చివర - isect_పునాది;
	src_off = (isect_పునాది / 64) - (ref->పునాది / 64);

	కొరకు (wi = 0; wi < nr_పదాలు; wi++)
		బహిర్గతం->బిట్లు[wi] = READ_ONCE(ref->src->బిట్లు[src_off + wi]);

	తల_ముసుగు = GENMASK_U64(63, isect_పునాది & 63);
	బహిర్గతం->బిట్లు[0] &= తల_ముసుగు;
	తోక_ముసుగు = GENMASK_U64((isect_చివర - 1) & 63, 0);
	బహిర్గతం->బిట్లు[nr_పదాలు - 1] &= తోక_ముసుగు;
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
శూన్య scx_cmask_ref_or(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref, స్థిరరాశి నిర్మాణం scx_cmask *src)
{
	cmask_walk_op2(ref->src->బిట్లు, ref->పునాది, ref->nr_cids,
		       src->బిట్లు, src->పునాది, src->nr_cids, CMASK_OP2_REF_OR);
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
శూన్య scx_cmask_ref_copy(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref, స్థిరరాశి నిర్మాణం scx_cmask *src)
{
	cmask_walk_op2(ref->src->బిట్లు, ref->పునాది, ref->nr_cids,
		       src->బిట్లు, src->పునాది, src->nr_cids, CMASK_OP2_REF_COPY);
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
శూన్య scx_cmask_ref_from_cpumask(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref,
				స్థిరరాశి నిర్మాణం cpumask *cpumask)
{
	నిర్మాణం scx_cmask *m = ref->src;
	u32 పునాది = ref->పునాది, nr_cids = ref->nr_cids;
	u32 wi, nr_పదాలు;

	ఒకవేళ (!nr_cids)
		తిరిగి;

	nr_పదాలు = (పునాది + nr_cids - 1) / 64 - పునాది / 64 + 1;
	కొరకు (wi = 0; wi < nr_పదాలు; wi++) {
		u32 మాట_మొదటి_cid = (పునాది / 64 + wi) * 64;
		u64 మాట = 0;
		u32 బిట్టు_2;

		కొరకు (బిట్టు_2 = 0; బిట్టు_2 < 64; బిట్టు_2++) {
			u32 cid = మాట_మొదటి_cid + బిట్టు_2;

			ఒకవేళ (cid < పునాది || cid >= పునాది + nr_cids)
				కొనసాగు;
			ఒకవేళ (cpumask_test_cpu(__scx_cid_to_cpu(cid), cpumask))
				మాట |= బిట్టు_U64(బిట్టు_2);
		}
		WRITE_ONCE(m->బిట్లు[wi], మాట);
	}
}

పూర్ణాంకం scx_cid_kfunc_init(శూన్య)
{
	తిరిగి రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_సమితి_init_cids) ?:
		రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_సమితి_cid) ?:
		రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_TRACING, &scx_kfunc_సమితి_cid) ?:
		రిజిస్టరు_btf_kfunc_id_సమితి(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_సమితి_cid);
}
