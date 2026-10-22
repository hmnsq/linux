/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#अन्तर्भाव <linux/cacheinfo.h>

#अन्तर्भाव "internal.h"
#अन्तर्भाव "cid.h"

/*
 * cid tables. The cid kfuncs are available whether the root scheduler is
 * cid-form or cpu-form, the latter to allow gradual migration to cids, so every
 * root builds a default mapping. Each root enable allocates a fresh set, builds
 * it privately and publishes the __rcu globals below once the layout is final.
 * Root disable unpublishes and RCU-frees the set. kfuncs may run before the
 * tables are published and must check for NULL.
 */
u32 scx_nr_cid_shards;
s16 __rcu *scx_cid_गन्तव्यम्_cpu_tbl;
s16 __rcu *scx_cpu_गन्तव्यम्_cid_tbl;
s32 __rcu *scx_cid_गन्तव्यम्_shard;
s32 __rcu *scx_shard_ग्रन्थिः;
संरचना scx_cid_shard __rcu *scx_cid_shard_ranges;
संरचना scx_cid_topo __rcu *scx_cid_topo;

स्थिर संरचना scx_cid_tables *scx_cid_tables;	/* used only during alloc/free */

#परिभाषय SCX_CID_TOPO_NEG	(संरचना scx_cid_topo) {				\
	.core_cid = -1, .core_idx = -1, .llc_cid = -1, .llc_idx = -1,		\
	.ग्रन्थिः_cid = -1, .ग्रन्थिः_idx = -1, .shard_cid = -1, .shard_idx = -1,	\
}

/*
 * Return @cpu's LLC shared_cpu_map. If cacheinfo isn't populated (offline or
 * !present), record @cpu in @fallbacks and return its node mask instead - the
 * worst that can happen is that the cpu's LLC becomes coarser than reality.
 */
स्थिर ध्रुव संरचना cpumask *cpu_llc_आवरणम्(पूर्णाङ्क cpu, संरचना cpumask *fallbacks)
{
	संरचना cpu_cacheinfo *ci = get_cpu_cacheinfo(cpu);

	यदि (!ci || !ci->info_list || !ci->अङ्कः_leaves) {
		cpumask_समुच्चयः_cpu(cpu, fallbacks);
		प्रत्यागम cpumask_सम्बन्धि_ग्रन्थिः(cpu_गन्तव्यम्_ग्रन्थिः(cpu));
	}
	प्रत्यागम &ci->info_list[ci->अङ्कः_leaves - 1].shared_cpu_map;
}

/*
 * Compute per-LLC shard layout. Each shard holds at most @shard_size cids, and
 * in any case no more than SCX_CID_SHARD_MAX_CPUS. Cores are spread as evenly
 * as possible across shards so cpu count is balanced: the first *@nr_large_p
 * shards get (*@cores_per_shard_p + 1) cores, the rest get *@cores_per_shard_p.
 */
स्थिर शून्य calc_shard_layout(ध्रुव संरचना cpumask *llc_cpus, u32 shard_आकार,
			      u32 *cores_प्रत्येकम्_shard_p, u32 *nr_large_p)
{
	u32 nr_cores = 0, nr_cpus = 0, nr_shards;
	पूर्णाङ्क cpu;

	for_each_cpu(cpu, llc_cpus) {
		nr_cpus++;
		यदि (cpumask_first(topology_sibling_cpumask(cpu)) == cpu)
			nr_cores++;
	}

	nr_shards = max_t(u32, 1, DIV_आवर्तनम्_ऊर्ध्वम्(nr_cpus, shard_आकार));
	nr_shards = max_t(u32, nr_shards,
			  DIV_आवर्तनम्_ऊर्ध्वम्(nr_cpus, SCX_CID_SHARD_MAX_CPUS));

	*cores_प्रत्येकम्_shard_p = nr_cores / nr_shards;
	*nr_large_p = nr_cores % nr_shards;
}

स्थिर शून्य scx_cid_tables_free(संरचना scx_cid_tables *tbls)
{
	यदि (!tbls)
		प्रत्यागम;
	kvfree(tbls->cid_गन्तव्यम्_cpu);
	kvfree(tbls->cpu_गन्तव्यम्_cid);
	kvfree(tbls->cid_गन्तव्यम्_shard);
	kvfree(tbls->shard_ग्रन्थिः);
	kvfree(tbls->shard_ranges);
	kvfree(tbls->topo);
	kfree(tbls);
}

स्थिर शून्य scx_cid_tables_free_rcufn(संरचना rcu_शीर्ष *rcu)
{
	scx_cid_tables_free(container_सम्बन्धि(rcu, संरचना scx_cid_tables, rcu));
}

स्थिर संरचना scx_cid_tables *scx_cid_alloc_tables(शून्य)
{
	u32 npossible = अङ्कः_possible_cpus();
	संरचना scx_cid_tables *tbls;

	tbls = kzalloc_obj(*tbls);
	यदि (!tbls)
		प्रत्यागम रिक्त;

	tbls->cid_गन्तव्यम्_cpu = kvzalloc_objs(*tbls->cid_गन्तव्यम्_cpu, npossible);
	tbls->cpu_गन्तव्यम्_cid = kvzalloc_objs(*tbls->cpu_गन्तव्यम्_cid, nr_cpu_ids);
	tbls->cid_गन्तव्यम्_shard = kvzalloc_objs(*tbls->cid_गन्तव्यम्_shard, npossible);
	tbls->shard_ग्रन्थिः = kvzalloc_objs(*tbls->shard_ग्रन्थिः, npossible);
	tbls->shard_ranges = kvzalloc_objs(*tbls->shard_ranges, npossible);
	tbls->topo = kvzalloc_objs(*tbls->topo, npossible);

	यदि (!tbls->cid_गन्तव्यम्_cpu || !tbls->cpu_गन्तव्यम्_cid || !tbls->cid_गन्तव्यम्_shard ||
	    !tbls->shard_ग्रन्थिः || !tbls->shard_ranges || !tbls->topo) {
		scx_cid_tables_free(tbls);
		प्रत्यागम रिक्त;
	}

	प्रत्यागम tbls;
}

/**
 * scx_cid_publish_tables - Publish the tables scx_cid_init() built
 *
 * Called after ops.init_cids() where the layout is final.
 */
शून्य scx_cid_publish_tables(शून्य)
{
	संरचना scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);

	scx_nr_cid_shards = tbls->nr_shards;
	rcu_नियोजय_pointer(scx_cid_गन्तव्यम्_cpu_tbl, tbls->cid_गन्तव्यम्_cpu);
	rcu_नियोजय_pointer(scx_cpu_गन्तव्यम्_cid_tbl, tbls->cpu_गन्तव्यम्_cid);
	rcu_नियोजय_pointer(scx_cid_गन्तव्यम्_shard, tbls->cid_गन्तव्यम्_shard);
	rcu_नियोजय_pointer(scx_shard_ग्रन्थिः, tbls->shard_ग्रन्थिः);
	rcu_नियोजय_pointer(scx_cid_shard_ranges, tbls->shard_ranges);
	rcu_नियोजय_pointer(scx_cid_topo, tbls->topo);
}

/**
 * scx_cid_retire_tables - Unpublish and retire the cid tables
 *
 * Called by root disable after the readers which dereference without NULL
 * checks are drained, inside cpus_read_lock() to exclude the hotplug path.
 */
शून्य scx_cid_retire_tables(शून्य)
{
	संरचना scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);
	lockdep_assert_cpus_held();

	यदि (!tbls)
		प्रत्यागम;

	scx_cid_tables = रिक्त;
	RCU_INIT_POINTER(scx_cid_गन्तव्यम्_cpu_tbl, रिक्त);
	RCU_INIT_POINTER(scx_cpu_गन्तव्यम्_cid_tbl, रिक्त);
	RCU_INIT_POINTER(scx_cid_गन्तव्यम्_shard, रिक्त);
	RCU_INIT_POINTER(scx_shard_ग्रन्थिः, रिक्त);
	RCU_INIT_POINTER(scx_cid_shard_ranges, रिक्त);
	RCU_INIT_POINTER(scx_cid_topo, रिक्त);
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
s32 scx_cid_init(संरचना scx_sched *sch)
{
	cpumask_var_t गन्तव्यम्_walk __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t ग्रन्थिः_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t core_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_fallback __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t online_no_topo __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	संरचना scx_cid_tables *tbls;
	u32 अग्र_cid = 0;
	s32 अग्र_ग्रन्थिः_idx = 0, अग्र_llc_idx = 0, अग्र_core_idx = 0;
	s32 अग्र_shard_idx = 0;
	u32 shard_आकार, max_cids;
	u32 notopo_अन्तर्_shard;
	s32 notopo_shard_cid, notopo_shard_idx;
	s32 cpu, cid, si;

	/* CMASK_MAX_WORDS in cid.bpf.h covers NR_CPUS up to 8192 */
	BUILD_BUG_ON(NR_CPUS > 8192);

	lockdep_assert_cpus_held();
	lockdep_assert_held(&scx_enable_mutex);

	shard_आकार = sch->ops.cid_shard_आकार ?: SCX_CID_SHARD_आकार_DFL;
	max_cids = min_t(u32, shard_आकार, SCX_CID_SHARD_MAX_CPUS);

	tbls = scx_cid_alloc_tables();
	यदि (!tbls)
		प्रत्यागम -ENOMEM;

	scx_cid_tables = tbls;

	प्रति (si = 0; si < अङ्कः_possible_cpus(); si++)
		tbls->shard_ग्रन्थिः[si] = NUMA_NO_ग्रन्थिः;

	यदि (!zalloc_cpumask_var(&गन्तव्यम्_walk, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&ग्रन्थिः_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&core_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_fallback, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&online_no_topo, GFP_KERNEL))
		प्रत्यागम -ENOMEM;

	/* -1 sentinels for sparse-possible cpu id holes (0 is a valid cid) */
	प्रति (cpu = 0; cpu < nr_cpu_ids; cpu++)
		tbls->cpu_गन्तव्यम्_cid[cpu] = -1;

	cpumask_copy(गन्तव्यम्_walk, cpu_online_आवरणम्);

	यावत् (!cpumask_रिक्तानि(गन्तव्यम्_walk)) {
		s32 अग्र_cpu = cpumask_first(गन्तव्यम्_walk);
		s32 nid = cpu_गन्तव्यम्_ग्रन्थिः(अग्र_cpu);
		s32 ग्रन्थिः_cid = अग्र_cid;
		s32 ग्रन्थिः_idx;

		/*
		 * No NUMA info: skip and let the tail loop assign a no-topo
		 * cid. cpumask_of_node(-1) is undefined.
		 */
		यदि (nid < 0) {
			cpumask_clear_cpu(अग्र_cpu, गन्तव्यम्_walk);
			अनुवर्त;
		}

		ग्रन्थिः_idx = अग्र_ग्रन्थिः_idx++;

		/* node_scratch = to_walk & this node */
		cpumask_and(ग्रन्थिः_scratch, गन्तव्यम्_walk, cpumask_सम्बन्धि_ग्रन्थिः(nid));
		यदि (WARN_ON_ONCE(!cpumask_test_cpu(अग्र_cpu, ग्रन्थिः_scratch)))
			प्रत्यागम -EINVAL;

		यावत् (!cpumask_रिक्तानि(ग्रन्थिः_scratch)) {
			s32 ncpu = cpumask_first(ग्रन्थिः_scratch);
			ध्रुव संरचना cpumask *llc_आवरणम् = cpu_llc_आवरणम्(ncpu, llc_fallback);
			s32 llc_cid = अग्र_cid;
			s32 llc_idx = अग्र_llc_idx++;
			u32 cores_प्रत्येकम्_shard, nr_large;
			u32 shard_local = 0, cores_अन्तर्_shard = 0, cids_अन्तर्_shard = 0;
			s32 shard_cid, shard_idx;

			/* llc_scratch = node_scratch & this llc */
			cpumask_and(llc_scratch, ग्रन्थिः_scratch, llc_आवरणम्);
			यदि (WARN_ON_ONCE(!cpumask_test_cpu(ncpu, llc_scratch)))
				प्रत्यागम -EINVAL;

			calc_shard_layout(llc_scratch, shard_आकार, &cores_प्रत्येकम्_shard, &nr_large);
			shard_cid = अग्र_cid;
			shard_idx = अग्र_shard_idx++;
			tbls->shard_ग्रन्थिः[shard_idx] = nid;

			यावत् (!cpumask_रिक्तानि(llc_scratch)) {
				s32 lcpu = cpumask_first(llc_scratch);
				ध्रुव संरचना cpumask *sib = topology_sibling_cpumask(lcpu);
				s32 core_cid = अग्र_cid;
				s32 core_idx = अग्र_core_idx++;
				s32 ccpu;
				u32 max_cores, cids_अन्तर्_core;

				/* core_scratch = llc_scratch & this core */
				cpumask_and(core_scratch, llc_scratch, sib);
				यदि (WARN_ON_ONCE(!cpumask_test_cpu(lcpu, core_scratch)))
					प्रत्यागम -EINVAL;

				/*
				 * Advance to a new shard when either core or
				 * cid count reaches max. The latter bounds
				 * shard sizes under uneven SMT. Never start an
				 * empty shard.
				 */
				cids_अन्तर्_core = cpumask_weight(core_scratch);
				max_cores = cores_प्रत्येकम्_shard + (shard_local < nr_large ? 1 : 0);
				यदि (cores_अन्तर्_shard &&
				    (cores_अन्तर्_shard >= max_cores ||
				     cids_अन्तर्_shard + cids_अन्तर्_core > max_cids)) {
					shard_local++;
					cores_अन्तर्_shard = 0;
					cids_अन्तर्_shard = 0;
					shard_cid = अग्र_cid;
					shard_idx = अग्र_shard_idx++;
					tbls->shard_ग्रन्थिः[shard_idx] = nid;
				}
				cores_अन्तर्_shard++;
				cids_अन्तर्_shard += cids_अन्तर्_core;

				for_each_cpu(ccpu, core_scratch) {
					s32 cid = अग्र_cid++;

					tbls->cid_गन्तव्यम्_cpu[cid] = ccpu;
					tbls->cpu_गन्तव्यम्_cid[ccpu] = cid;
					tbls->cid_गन्तव्यम्_shard[cid] = shard_idx;
					tbls->topo[cid] = (संरचना scx_cid_topo){
						.core_cid = core_cid,
						.core_idx = core_idx,
						.llc_cid = llc_cid,
						.llc_idx = llc_idx,
						.ग्रन्थिः_cid = ग्रन्थिः_cid,
						.ग्रन्थिः_idx = ग्रन्थिः_idx,
						.shard_cid = shard_cid,
						.shard_idx = shard_idx,
					};

					cpumask_clear_cpu(ccpu, llc_scratch);
					cpumask_clear_cpu(ccpu, ग्रन्थिः_scratch);
					cpumask_clear_cpu(ccpu, गन्तव्यम्_walk);
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
	notopo_अन्तर्_shard = min_t(u32, shard_आकार, SCX_CID_SHARD_MAX_CPUS);
	notopo_shard_cid = -1;
	notopo_shard_idx = -1;

	for_each_cpu(cpu, cpu_possible_आवरणम्) {
		यदि (tbls->cpu_गन्तव्यम्_cid[cpu] != -1)
			अनुवर्त;
		यदि (cpu_online(cpu))
			cpumask_समुच्चयः_cpu(cpu, online_no_topo);

		cid = अग्र_cid++;
		tbls->cid_गन्तव्यम्_cpu[cid] = cpu;
		tbls->cpu_गन्तव्यम्_cid[cpu] = cid;

		यदि (notopo_अन्तर्_shard >= min_t(u32, shard_आकार, SCX_CID_SHARD_MAX_CPUS)) {
			notopo_shard_cid = cid;
			notopo_shard_idx = अग्र_shard_idx++;
			notopo_अन्तर्_shard = 0;
		}
		notopo_अन्तर्_shard++;

		tbls->cid_गन्तव्यम्_shard[cid] = notopo_shard_idx;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = notopo_shard_cid;
		tbls->topo[cid].shard_idx = notopo_shard_idx;
	}

	यदि (!cpumask_रिक्तानि(llc_fallback))
		pr_warn("scx_cid: cpus without cacheinfo, using node mask as llc: %*pbl\n",
			cpumask_pr_तर्काः(llc_fallback));
	यदि (!cpumask_रिक्तानि(online_no_topo))
		pr_warn("scx_cid: online cpus with no usable topology: %*pbl\n",
			cpumask_pr_तर्काः(online_no_topo));

	/*
	 * Fill cid_shard_ranges[] from cid_to_shard[]. Shards are contiguous
	 * cid ranges by construction: base_cid is the first cid landing in a
	 * shard, nr_cids is the count.
	 */
	प्रति (cid = 0; cid < अग्र_cid; cid++) {
		s32 sidx = tbls->cid_गन्तव्यम्_shard[cid];

		यदि (tbls->shard_ranges[sidx].nr_cids == 0)
			tbls->shard_ranges[sidx].आधारः_cid = cid;
		tbls->shard_ranges[sidx].nr_cids++;
	}

	tbls->nr_shards = अग्र_shard_idx;
	प्रत्यागम 0;
}

/**
 * scx_cmask_clear - Zero every bit in @m's active range
 * @m: cmask to clear
 *
 * Storage past the active range is left as is.
 */
शून्य scx_cmask_clear(संरचना scx_cmask *m)
{
	u32 nr_शब्दाः;

	यदि (!m->nr_cids)
		प्रत्यागम;
	nr_शब्दाः = (m->आधारः + m->nr_cids - 1) / 64 - m->आधारः / 64 + 1;
	स्मृतिं_पूरय(m->द्वयङ्काः, 0, nr_शब्दाः * परिमाण(u64));
}

/**
 * scx_cmask_fill - Set every bit in @m's active range
 * @m: cmask to fill
 *
 * Counterpart to scx_cmask_clear(). Storage past the active range is left as is.
 */
शून्य scx_cmask_fill(संरचना scx_cmask *m)
{
	u32 nr_शब्दाः, शीर्ष_द्वयङ्काः, पुच्छ_द्वयङ्काः;

	यदि (!m->nr_cids)
		प्रत्यागम;
	nr_शब्दाः = (m->आधारः + m->nr_cids - 1) / 64 - m->आधारः / 64 + 1;
	स्मृतिं_पूरय(m->द्वयङ्काः, 0xff, nr_शब्दाः * परिमाण(u64));

	/* clear word-0 bits below base */
	शीर्ष_द्वयङ्काः = m->आधारः & 63;
	यदि (शीर्ष_द्वयङ्काः)
		m->द्वयङ्काः[0] &= ~((1ULL << शीर्ष_द्वयङ्काः) - 1);

	/* clear last-word bits at or past base + nr_cids */
	पुच्छ_द्वयङ्काः = (m->आधारः + m->nr_cids) & 63;
	यदि (पुच्छ_द्वयङ्काः)
		m->द्वयङ्काः[nr_शब्दाः - 1] &= (1ULL << पुच्छ_द्वयङ्काः) - 1;
}

/*
 * Return the index of the largest entry in @counts, or NUMA_NO_NODE if all
 * entries are zero. Ties resolve to the lowest index.
 */
स्थिर s32 pick_max_ग्रन्थिः(ध्रुव u32 *counts, u32 n)
{
	s32 best = NUMA_NO_ग्रन्थिः;
	u32 best_गणनम् = 0, i;

	प्रति (i = 0; i < n; i++) {
		यदि (counts[i] > best_गणनम्) {
			best_गणनम् = counts[i];
			best = i;
		}
	}
	प्रत्यागम best;
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
__bpf_kfunc शून्य scx_bpf_cid_override(ध्रुव s32 *cpu_गन्तव्यम्_cid_arena, u32 cpu_गन्तव्यम्_cid_cnt,
				      ध्रुव s32 *shard_start__arena, u32 shard_start_cnt,
				      ध्रुव संरचना bpf_prog_aux *aux)
{
	cpumask_var_t आलोकितम् __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	u32 *ग्रन्थिः_counts __free(kfree) = रिक्त;
	s32 *cpu_गन्तव्यम्_cid __free(kfree) = रिक्त;
	s32 *shard_start __free(kfree) = रिक्त;
	u32 npossible = अङ्कः_possible_cpus();
	संरचना scx_cid_tables *tbls;
	संरचना scx_sched *sch;
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
	alloced = zalloc_cpumask_var(&आलोकितम्, GFP_KERNEL);
	ग्रन्थिः_counts = kzalloc_objs(*ग्रन्थिः_counts, nr_ग्रन्थिः_ids);
	यदि (cpu_गन्तव्यम्_cid_cnt == nr_cpu_ids)
		cpu_गन्तव्यम्_cid = kmemdup(cpu_गन्तव्यम्_cid_arena, cpu_गन्तव्यम्_cid_cnt * परिमाण(s32),
				     GFP_KERNEL);
	यदि (nr_shards && nr_shards <= npossible)
		shard_start = kmemdup(shard_start__arena, nr_shards * परिमाण(s32),
				      GFP_KERNEL);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	यदि (unlikely(!sch))
		प्रत्यागम;

	/* called from ops.init_cids(), so the tables exist and are unpublished */
	lockdep_assert_held(&scx_enable_mutex);
	tbls = scx_cid_tables;

	यदि (cpu_गन्तव्यम्_cid_cnt != nr_cpu_ids) {
		scx_error(sch, "scx_bpf_cid_override: cpu_to_cid expected %u entries, got %u",
			  nr_cpu_ids, cpu_गन्तव्यम्_cid_cnt);
		प्रत्यागम;
	}

	यदि (!nr_shards || nr_shards > npossible) {
		scx_error(sch, "scx_bpf_cid_override: invalid shard_start count %u",
			  nr_shards);
		प्रत्यागम;
	}

	यदि (!alloced || !ग्रन्थिः_counts || !cpu_गन्तव्यम्_cid || !shard_start) {
		scx_error(sch, "scx_bpf_cid_override: allocation failed");
		प्रत्यागम;
	}

	/* validate shard_start[]: starts at 0, strictly increasing, in range */
	यदि (shard_start[0] != 0) {
		scx_error(sch, "scx_bpf_cid_override: shard_start[0] must be 0, got %d",
			  shard_start[0]);
		प्रत्यागम;
	}
	प्रति (si = 1; si < nr_shards; si++) {
		यदि (shard_start[si] <= shard_start[si - 1]) {
			scx_error(sch, "scx_bpf_cid_override: shard_start not increasing at [%d]",
				  si);
			प्रत्यागम;
		}
		यदि (shard_start[si] >= npossible) {
			scx_error(sch, "scx_bpf_cid_override: shard_start[%d]=%d >= %u",
				  si, shard_start[si], npossible);
			प्रत्यागम;
		}
		यदि (shard_start[si] - shard_start[si - 1] > SCX_CID_SHARD_MAX_CPUS) {
			scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
				  si - 1, shard_start[si] - shard_start[si - 1],
				  SCX_CID_SHARD_MAX_CPUS);
			प्रत्यागम;
		}
	}
	यदि (npossible - shard_start[nr_shards - 1] > SCX_CID_SHARD_MAX_CPUS) {
		scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
			  nr_shards - 1, npossible - shard_start[nr_shards - 1],
			  SCX_CID_SHARD_MAX_CPUS);
		प्रत्यागम;
	}

	/* validate first so that invalid input leaves the tables untouched */
	for_each_possible_cpu(cpu) {
		s32 c = cpu_गन्तव्यम्_cid[cpu];

		यदि (!cid_valid(sch, c))
			प्रत्यागम;
		यदि (cpumask_test_and_समुच्चयः_cpu(c, आलोकितम्)) {
			scx_error(sch, "cid %d assigned to multiple cpus", c);
			प्रत्यागम;
		}
	}

	for_each_possible_cpu(cpu) {
		s32 c = cpu_गन्तव्यम्_cid[cpu];

		tbls->cpu_गन्तव्यम्_cid[cpu] = c;
		tbls->cid_गन्तव्यम्_cpu[c] = cpu;
	}

	/*
	 * Derive shard_node[] by majority count: an overridden shard may
	 * span NUMA nodes, so assign each to the node that owns the most cpus.
	 */
	प्रति (si = 0; si < nr_shards; si++) {
		u32 अन्तः = (si + 1 < nr_shards) ? shard_start[si + 1] : npossible;

		स्मृतिं_पूरय(ग्रन्थिः_counts, 0, nr_ग्रन्थिः_ids * परिमाण(*ग्रन्थिः_counts));
		प्रति (cid = shard_start[si]; cid < अन्तः; cid++) {
			s32 ग्रन्थिः = cpu_गन्तव्यम्_ग्रन्थिः(tbls->cid_गन्तव्यम्_cpu[cid]);

			यदि (numa_valid_ग्रन्थिः(ग्रन्थिः))
				ग्रन्थिः_counts[ग्रन्थिः]++;
		}
		tbls->shard_ग्रन्थिः[si] = pick_max_ग्रन्थिः(ग्रन्थिः_counts, nr_ग्रन्थिः_ids);
	}

	/*
	 * Invalidate stale topo info and install shard layout from
	 * @shard_start. Walk shards to derive shard_cid/shard_idx for each cid.
	 */
	si = 0;
	प्रति (cid = 0; cid < npossible; cid++) {
		यदि (si + 1 < nr_shards && cid >= shard_start[si + 1])
			si++;
		tbls->cid_गन्तव्यम्_shard[cid] = si;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = shard_start[si];
		tbls->topo[cid].shard_idx = si;
	}

	/* Rebuild shard_ranges[] for the new layout. */
	स्मृतिं_पूरय(tbls->shard_ranges, 0, npossible * परिमाण(*tbls->shard_ranges));
	प्रति (si = 0; si < nr_shards; si++) {
		u32 अन्तः = (si + 1 < nr_shards) ? shard_start[si + 1] : npossible;

		tbls->shard_ranges[si].आधारः_cid = shard_start[si];
		tbls->shard_ranges[si].nr_cids = अन्तः - shard_start[si];
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
__bpf_kfunc s32 scx_bpf_cid_गन्तव्यम्_cpu(s32 cid, ध्रुव संरचना bpf_prog_aux *aux)
{
	संरचना scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	यदि (unlikely(!sch))
		प्रत्यागम -EINVAL;
	प्रत्यागम scx_cid_गन्तव्यम्_cpu(sch, cid);
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
__bpf_kfunc s32 scx_bpf_cpu_गन्तव्यम्_cid(s32 cpu, ध्रुव संरचना bpf_prog_aux *aux)
{
	संरचना scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	यदि (unlikely(!sch))
		प्रत्यागम -EINVAL;
	प्रत्यागम scx_cpu_गन्तव्यम्_cid(sch, cpu);
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
गणना cmask_op2 {
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

स्थिर __always_inline bool cmask_op2_is_pred(ध्रुव गणना cmask_op2 op)
{
	प्रत्यागम op == CMASK_OP2_SUBSET || op == CMASK_OP2_INTERSECTS;
}

स्थिर __always_inline bool cmask_word_op2(u64 *av, ध्रुव u64 *bp, u64 आवरणम्,
					   ध्रुव गणना cmask_op2 op)
{
	विकल्प (op) {
	पक्ष CMASK_OP2_AND:
		WRITE_ONCE(*av, *av & (~आवरणम् | READ_ONCE(*bp)));
		प्रत्यागम false;
	पक्ष CMASK_OP2_OR:
		WRITE_ONCE(*av, *av | (READ_ONCE(*bp) & आवरणम्));
		प्रत्यागम false;
	पक्ष CMASK_OP2_COPY:
		WRITE_ONCE(*av, (*av & ~आवरणम्) | (READ_ONCE(*bp) & आवरणम्));
		प्रत्यागम false;
	पक्ष CMASK_OP2_ANDNOT:
		WRITE_ONCE(*av, *av & ~(READ_ONCE(*bp) & आवरणम्));
		प्रत्यागम false;
	पक्ष CMASK_OP2_SUBSET:
		/* stop on the first bit in @sub not set in @super */
		प्रत्यागम (READ_ONCE(*bp) & ~READ_ONCE(*av)) & आवरणम्;
	पक्ष CMASK_OP2_INTERSECTS:
		प्रत्यागम (READ_ONCE(*av) & READ_ONCE(*bp)) & आवरणम्;
	पक्ष CMASK_OP2_REF_OR:
		WRITE_ONCE(*av, READ_ONCE(*av) | (READ_ONCE(*bp) & आवरणम्));
		प्रत्यागम false;
	पक्ष CMASK_OP2_REF_COPY:
		WRITE_ONCE(*av, (READ_ONCE(*av) & ~आवरणम्) | (READ_ONCE(*bp) & आवरणम्));
		प्रत्यागम false;
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
स्थिर __always_inline bool cmask_walk_op2(u64 *a_द्वयङ्काः, u32 a_आधारः, u32 a_nr_cids,
					   ध्रुव u64 *b_द्वयङ्काः, u32 b_आधारः, u32 b_nr_cids,
					   ध्रुव गणना cmask_op2 op)
{
	u32 lo = max(a_आधारः, b_आधारः);
	u32 hi = min(a_आधारः + a_nr_cids, b_आधारः + b_nr_cids);
	u32 a_word_off = a_आधारः / 64;
	u32 b_word_off = b_आधारः / 64;
	u32 lo_word = lo / 64;
	u32 hi_word = (hi - 1) / 64;
	u64 शीर्ष_आवरणम् = GENMASK_U64(63, lo & 63);
	u64 पुच्छ_आवरणम् = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	यदि (lo >= hi)
		प्रत्यागम false;

	यदि (lo_word == hi_word)
		प्रत्यागम cmask_word_op2(&a_द्वयङ्काः[lo_word - a_word_off],
				      &b_द्वयङ्काः[lo_word - b_word_off],
				      शीर्ष_आवरणम् & पुच्छ_आवरणम्, op);

	यदि (cmask_word_op2(&a_द्वयङ्काः[lo_word - a_word_off],
			   &b_द्वयङ्काः[lo_word - b_word_off], शीर्ष_आवरणम्, op) &&
	    cmask_op2_is_pred(op))
		प्रत्यागम true;

	प्रति (w = lo_word + 1; w < hi_word; w++)
		यदि (cmask_word_op2(&a_द्वयङ्काः[w - a_word_off],
				   &b_द्वयङ्काः[w - b_word_off], ~0ULL, op) &&
		    cmask_op2_is_pred(op))
			प्रत्यागम true;

	प्रत्यागम cmask_word_op2(&a_द्वयङ्काः[hi_word - a_word_off],
			      &b_द्वयङ्काः[hi_word - b_word_off], पुच्छ_आवरणम्, op);
}

गणना cmask_op1 {
	CMASK_OP1_ANY_समुच्चयः,
};

स्थिर __always_inline bool cmask_word_op1(ध्रुव u64 *ap, u64 आवरणम्,
					   ध्रुव गणना cmask_op1 op)
{
	विकल्प (op) {
	पक्ष CMASK_OP1_ANY_समुच्चयः:
		प्रत्यागम READ_ONCE(*ap) & आवरणम्;
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
स्थिर __always_inline bool cmask_walk_op1(ध्रुव u64 *a_द्वयङ्काः, u32 a_आधारः,
					   u32 a_nr_cids,
					   ध्रुव गणना cmask_op1 op)
{
	u32 lo = a_आधारः;
	u32 hi = a_आधारः + a_nr_cids;
	u32 a_word_off = a_आधारः / 64;
	u32 lo_word = lo / 64;
	u32 hi_word = (hi - 1) / 64;
	u64 शीर्ष_आवरणम् = GENMASK_U64(63, lo & 63);
	u64 पुच्छ_आवरणम् = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	यदि (lo >= hi)
		प्रत्यागम false;

	यदि (lo_word == hi_word)
		प्रत्यागम cmask_word_op1(&a_द्वयङ्काः[lo_word - a_word_off],
				      शीर्ष_आवरणम् & पुच्छ_आवरणम्, op);

	यदि (cmask_word_op1(&a_द्वयङ्काः[lo_word - a_word_off], शीर्ष_आवरणम्, op))
		प्रत्यागम true;
	प्रति (w = lo_word + 1; w < hi_word; w++)
		यदि (cmask_word_op1(&a_द्वयङ्काः[w - a_word_off], ~0ULL, op))
			प्रत्यागम true;
	प्रत्यागम cmask_word_op1(&a_द्वयङ्काः[hi_word - a_word_off], पुच्छ_आवरणम्, op);
}

शून्य scx_cmask_and(संरचना scx_cmask *dst, ध्रुव संरचना scx_cmask *src)
{
	cmask_walk_op2(dst->द्वयङ्काः, dst->आधारः, dst->nr_cids,
		       src->द्वयङ्काः, src->आधारः, src->nr_cids, CMASK_OP2_AND);
}

शून्य scx_cmask_or(संरचना scx_cmask *dst, ध्रुव संरचना scx_cmask *src)
{
	cmask_walk_op2(dst->द्वयङ्काः, dst->आधारः, dst->nr_cids,
		       src->द्वयङ्काः, src->आधारः, src->nr_cids, CMASK_OP2_OR);
}

शून्य scx_cmask_copy(संरचना scx_cmask *dst, ध्रुव संरचना scx_cmask *src)
{
	cmask_walk_op2(dst->द्वयङ्काः, dst->आधारः, dst->nr_cids,
		       src->द्वयङ्काः, src->आधारः, src->nr_cids, CMASK_OP2_COPY);
}

शून्य scx_cmask_andnot(संरचना scx_cmask *dst, ध्रुव संरचना scx_cmask *src)
{
	cmask_walk_op2(dst->द्वयङ्काः, dst->आधारः, dst->nr_cids,
		       src->द्वयङ्काः, src->आधारः, src->nr_cids, CMASK_OP2_ANDNOT);
}

/*
 * Return true if @cm has any bit set in [@lo, @hi). Caller must ensure
 * [@lo, @hi) is contained in @cm's range.
 */
स्थिर bool cmask_any_समुच्चयः_अन्तर्_range(ध्रुव संरचना scx_cmask *cm, u32 lo, u32 hi)
{
	यदि (lo >= hi)
		प्रत्यागम false;
	प्रत्यागम cmask_walk_op1(&cm->द्वयङ्काः[lo / 64 - cm->आधारः / 64], lo, hi - lo,
			      CMASK_OP1_ANY_समुच्चयः);
}

/**
 * scx_cmask_subset - test whether @sub is a subset of @super
 * @sub: cmask to test
 * @super: cmask to test against
 *
 * Return true iff every set bit of @sub is also set in @super.
 */
bool scx_cmask_subset(ध्रुव संरचना scx_cmask *sub, ध्रुव संरचना scx_cmask *super)
{
	u32 super_अन्तः = super->आधारः + super->nr_cids;
	u32 sub_अन्तः = sub->आधारः + sub->nr_cids;

	/*
	 * Set bits in @sub outside @super's range can't be in @super, so any
	 * such bit means not a subset. The walk below only visits words
	 * common to both ranges, so these need a separate scan.
	 */
	यदि (sub->आधारः < super->आधारः &&
	    cmask_any_समुच्चयः_अन्तर्_range(sub, sub->आधारः, min(super->आधारः, sub_अन्तः)))
		प्रत्यागम false;
	यदि (sub_अन्तः > super_अन्तः &&
	    cmask_any_समुच्चयः_अन्तर्_range(sub, max(sub->आधारः, super_अन्तः), sub_अन्तः))
		प्रत्यागम false;

	प्रत्यागम !cmask_walk_op2((u64 *)super->द्वयङ्काः, super->आधारः, super->nr_cids,
			       sub->द्वयङ्काः, sub->आधारः, sub->nr_cids, CMASK_OP2_SUBSET);
}

bool scx_cmask_intersects(ध्रुव संरचना scx_cmask *a, ध्रुव संरचना scx_cmask *b)
{
	प्रत्यागम cmask_walk_op2((u64 *)a->द्वयङ्काः, a->आधारः, a->nr_cids,
			      b->द्वयङ्काः, b->आधारः, b->nr_cids, CMASK_OP2_INTERSECTS);
}

/**
 * scx_cmask_empty - Test whether @m has no bits set
 * @m: cmask to test
 *
 * Return true iff @m's active range has no bits set.
 */
bool scx_cmask_रिक्तानि(ध्रुव संरचना scx_cmask *m)
{
	प्रत्यागम !cmask_any_समुच्चयः_अन्तर्_range(m, m->आधारः, m->आधारः + m->nr_cids);
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
__bpf_kfunc शून्य scx_bpf_cid_topo(s32 cid, संरचना scx_cid_topo *बहिर्गमनम्, आकार_t बहिर्गमनम्_sz,
				  ध्रुव संरचना bpf_prog_aux *aux)
{
	आकार_t len = min(बहिर्गमनम्_sz, परिमाण(*बहिर्गमनम्));
	संरचना scx_cid_topo *topo;
	संरचना scx_sched *sch;

	/* the error cases and fields the kernel lacks read as -1 */
	स्मृतिं_पूरय(बहिर्गमनम्, 0xff, बहिर्गमनम्_sz);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	topo = rcu_dereference(scx_cid_topo);
	यदि (unlikely(!sch) || !cid_valid(sch, cid) || unlikely(!topo))
		प्रत्यागम;

	स्मृतिं_प्रतिलिख(बहिर्गमनम्, &topo[cid], len);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_START(scx_kfunc_ids_init_cids)
BTF_ID_FLAGS(func, scx_bpf_cid_override, KF_IMPLICIT_तर्काः | KF_SLEEPABLE)
BTF_KFUNCS_अन्तः(scx_kfunc_ids_init_cids)

स्थिर ध्रुव संरचना btf_kfunc_id_समुच्चयः scx_kfunc_समुच्चयः_init_cids = {
	.owner	= THIS_MODULE,
	.समुच्चयः	= &scx_kfunc_ids_init_cids,
	.filter	= scx_kfunc_context_filter,
};

BTF_KFUNCS_START(scx_kfunc_ids_cid)
BTF_ID_FLAGS(func, scx_bpf_cid_गन्तव्यम्_cpu, KF_IMPLICIT_तर्काः)
BTF_ID_FLAGS(func, scx_bpf_cpu_गन्तव्यम्_cid, KF_IMPLICIT_तर्काः)
BTF_ID_FLAGS(func, scx_bpf_cid_topo, KF_IMPLICIT_तर्काः)
BTF_KFUNCS_अन्तः(scx_kfunc_ids_cid)

स्थिर ध्रुव संरचना btf_kfunc_id_समुच्चयः scx_kfunc_समुच्चयः_cid = {
	.owner	= THIS_MODULE,
	.समुच्चयः	= &scx_kfunc_ids_cid,
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
पूर्णाङ्क scx_cmask_ref_init(संरचना scx_sched *sch, ध्रुव संरचना scx_cmask *src,
		       संरचना scx_cmask_ref *ref)
{
	u32 आधारः, nr_cids, alloc_शब्दाः, npossible = अङ्कः_possible_cpus();
	s32 *cid_गन्तव्यम्_shard;

	आधारः = READ_ONCE(src->आधारः);
	nr_cids = READ_ONCE(src->nr_cids);
	alloc_शब्दाः = READ_ONCE(src->alloc_शब्दाः);

	यदि (unlikely(आधारः >= npossible || nr_cids > npossible - आधारः ||
		     SCX_CMASK_NR_शब्दाः(nr_cids) > alloc_शब्दाः))
		प्रत्यागम -EINVAL;

	ref->sch = sch;
	ref->src = (संरचना scx_cmask *)src;
	ref->आधारः = आधारः;
	ref->nr_cids = nr_cids;

	cid_गन्तव्यम्_shard = rcu_dereference_all(scx_cid_गन्तव्यम्_shard);
	ref->shard_first = cid_गन्तव्यम्_shard[आधारः];
	यदि (likely(nr_cids))
		ref->shard_अन्तः = cid_गन्तव्यम्_shard[आधारः + nr_cids - 1] + 1;
	अन्यथा
		ref->shard_अन्तः = ref->shard_first;

	प्रत्यागम 0;
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
शून्य scx_cmask_ref_init_kern(संरचना scx_sched *sch, संरचना scx_cmask *m,
			     u32 आधारः, u32 nr_cids, संरचना scx_cmask_ref *ref)
{
	s32 *cid_गन्तव्यम्_shard;

	WRITE_ONCE(m->आधारः, आधारः);
	WRITE_ONCE(m->nr_cids, nr_cids);
	WRITE_ONCE(m->alloc_शब्दाः, SCX_CMASK_NR_शब्दाः(nr_cids));

	ref->sch = sch;
	ref->src = m;
	ref->आधारः = आधारः;
	ref->nr_cids = nr_cids;

	cid_गन्तव्यम्_shard = rcu_dereference_all(scx_cid_गन्तव्यम्_shard);
	ref->shard_first = cid_गन्तव्यम्_shard[आधारः];
	यदि (likely(nr_cids))
		ref->shard_अन्तः = cid_गन्तव्यम्_shard[आधारः + nr_cids - 1] + 1;
	अन्यथा
		ref->shard_अन्तः = ref->shard_first;
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
शून्य scx_cmask_ref_shard(ध्रुव संरचना scx_cmask_ref *ref, s32 shard_idx,
			 संरचना scx_cmask *बहिर्गमनम्)
{
	ध्रुव संरचना scx_cid_shard *shard =
		&rcu_dereference_all(scx_cid_shard_ranges)[shard_idx];
	u32 shard_आधारः = shard->आधारः_cid;
	u32 shard_अन्तः = shard_आधारः + shard->nr_cids;
	u32 isect_आधारः, isect_अन्तः, nr_शब्दाः, src_off, wi;
	u64 शीर्ष_आवरणम्, पुच्छ_आवरणम्;

	isect_आधारः = max(ref->आधारः, shard_आधारः);
	isect_अन्तः = min(ref->आधारः + ref->nr_cids, shard_अन्तः);

	यदि (isect_आधारः >= isect_अन्तः) {
		बहिर्गमनम्->आधारः = shard_आधारः;
		बहिर्गमनम्->nr_cids = 0;
		प्रत्यागम;
	}

	nr_शब्दाः = ((isect_अन्तः - 1) / 64) - (isect_आधारः / 64) + 1;
	यदि (nr_शब्दाः > बहिर्गमनम्->alloc_शब्दाः) {
		scx_error(ref->sch, "scx_cmask_ref_shard: out alloc_words=%u < %u for shard %d",
			  बहिर्गमनम्->alloc_शब्दाः, nr_शब्दाः, shard_idx);
		बहिर्गमनम्->आधारः = shard_आधारः;
		बहिर्गमनम्->nr_cids = 0;
		प्रत्यागम;
	}

	बहिर्गमनम्->आधारः = isect_आधारः;
	बहिर्गमनम्->nr_cids = isect_अन्तः - isect_आधारः;
	src_off = (isect_आधारः / 64) - (ref->आधारः / 64);

	प्रति (wi = 0; wi < nr_शब्दाः; wi++)
		बहिर्गमनम्->द्वयङ्काः[wi] = READ_ONCE(ref->src->द्वयङ्काः[src_off + wi]);

	शीर्ष_आवरणम् = GENMASK_U64(63, isect_आधारः & 63);
	बहिर्गमनम्->द्वयङ्काः[0] &= शीर्ष_आवरणम्;
	पुच्छ_आवरणम् = GENMASK_U64((isect_अन्तः - 1) & 63, 0);
	बहिर्गमनम्->द्वयङ्काः[nr_शब्दाः - 1] &= पुच्छ_आवरणम्;
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
शून्य scx_cmask_ref_or(ध्रुव संरचना scx_cmask_ref *ref, ध्रुव संरचना scx_cmask *src)
{
	cmask_walk_op2(ref->src->द्वयङ्काः, ref->आधारः, ref->nr_cids,
		       src->द्वयङ्काः, src->आधारः, src->nr_cids, CMASK_OP2_REF_OR);
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
शून्य scx_cmask_ref_copy(ध्रुव संरचना scx_cmask_ref *ref, ध्रुव संरचना scx_cmask *src)
{
	cmask_walk_op2(ref->src->द्वयङ्काः, ref->आधारः, ref->nr_cids,
		       src->द्वयङ्काः, src->आधारः, src->nr_cids, CMASK_OP2_REF_COPY);
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
शून्य scx_cmask_ref_from_cpumask(ध्रुव संरचना scx_cmask_ref *ref,
				ध्रुव संरचना cpumask *cpumask)
{
	संरचना scx_cmask *m = ref->src;
	u32 आधारः = ref->आधारः, nr_cids = ref->nr_cids;
	u32 wi, nr_शब्दाः;

	यदि (!nr_cids)
		प्रत्यागम;

	nr_शब्दाः = (आधारः + nr_cids - 1) / 64 - आधारः / 64 + 1;
	प्रति (wi = 0; wi < nr_शब्दाः; wi++) {
		u32 word_first_cid = (आधारः / 64 + wi) * 64;
		u64 word = 0;
		u32 bit;

		प्रति (bit = 0; bit < 64; bit++) {
			u32 cid = word_first_cid + bit;

			यदि (cid < आधारः || cid >= आधारः + nr_cids)
				अनुवर्त;
			यदि (cpumask_test_cpu(__scx_cid_to_cpu(cid), cpumask))
				word |= BIT_U64(bit);
		}
		WRITE_ONCE(m->द्वयङ्काः[wi], word);
	}
}

पूर्णाङ्क scx_cid_kfunc_init(शून्य)
{
	प्रत्यागम पञ्जिकाकोशः_btf_kfunc_id_समुच्चयः(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_समुच्चयः_init_cids) ?:
		पञ्जिकाकोशः_btf_kfunc_id_समुच्चयः(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_समुच्चयः_cid) ?:
		पञ्जिकाकोशः_btf_kfunc_id_समुच्चयः(BPF_PROG_TYPE_TRACING, &scx_kfunc_समुच्चयः_cid) ?:
		पञ्जिकाकोशः_btf_kfunc_id_समुच्चयः(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_समुच्चयः_cid);
}
