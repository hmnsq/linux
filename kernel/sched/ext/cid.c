/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#شامل <linux/cacheinfo.h>

#شامل "internal.h"
#شامل "cid.h"

/*
 * cid tables. The cid kfuncs are available whether the root scheduler is
 * cid-form or cpu-form, the latter to allow gradual migration to cids, so every
 * root builds a default mapping. Each root enable allocates a fresh set, builds
 * it privately and publishes the __rcu globals below once the layout is final.
 * Root disable unpublishes and RCU-frees the set. kfuncs may run before the
 * tables are published and must check for NULL.
 */
u32 scx_nr_cid_shards;
s16 __rcu *scx_cid_مقصد_cpu_tbl;
s16 __rcu *scx_cpu_مقصد_cid_tbl;
s32 __rcu *scx_cid_مقصد_shard;
s32 __rcu *scx_shard_گرہ;
ساخت scx_cid_shard __rcu *scx_cid_shard_ranges;
ساخت scx_cid_topo __rcu *scx_cid_topo;

ساکن ساخت scx_cid_tables *scx_cid_tables;	/* used only during alloc/free */

#تعریف_کرو SCX_CID_TOPO_NEG	(ساخت scx_cid_topo) {				\
	.core_cid = -1, .core_idx = -1, .llc_cid = -1, .llc_idx = -1,		\
	.گرہ_cid = -1, .گرہ_idx = -1, .shard_cid = -1, .shard_idx = -1,	\
}

/*
 * Return @cpu's LLC shared_cpu_map. If cacheinfo isn't populated (offline or
 * !present), record @cpu in @fallbacks and return its node mask instead - the
 * worst that can happen is that the cpu's LLC becomes coarser than reality.
 */
ساکن ثابت ساخت cpumask *cpu_llc_نقاب(صحیح cpu, ساخت cpumask *fallbacks)
{
	ساخت cpu_cacheinfo *ci = get_cpu_cacheinfo(cpu);

	اگر (!ci || !ci->info_list || !ci->عدد_leaves) {
		cpumask_سیٹ_cpu(cpu, fallbacks);
		واپس cpumask_کا_گرہ(cpu_مقصد_گرہ(cpu));
	}
	واپس &ci->info_list[ci->عدد_leaves - 1].shared_cpu_map;
}

/*
 * Compute per-LLC shard layout. Each shard holds at most @shard_size cids, and
 * in any case no more than SCX_CID_SHARD_MAX_CPUS. Cores are spread as evenly
 * as possible across shards so cpu count is balanced: the first *@nr_large_p
 * shards get (*@cores_per_shard_p + 1) cores, the rest get *@cores_per_shard_p.
 */
ساکن خالی calc_shard_layout(ثابت ساخت cpumask *llc_cpus, u32 shard_سائز,
			      u32 *cores_فی_shard_p, u32 *nr_large_p)
{
	u32 nr_cores = 0, nr_cpus = 0, nr_shards;
	صحیح cpu;

	for_each_cpu(cpu, llc_cpus) {
		nr_cpus++;
		اگر (cpumask_پہلا(topology_sibling_cpumask(cpu)) == cpu)
			nr_cores++;
	}

	nr_shards = max_t(u32, 1, DIV_باری_اوپر(nr_cpus, shard_سائز));
	nr_shards = max_t(u32, nr_shards,
			  DIV_باری_اوپر(nr_cpus, SCX_CID_SHARD_MAX_CPUS));

	*cores_فی_shard_p = nr_cores / nr_shards;
	*nr_large_p = nr_cores % nr_shards;
}

ساکن خالی scx_cid_tables_free(ساخت scx_cid_tables *tbls)
{
	اگر (!tbls)
		واپس;
	kvfree(tbls->cid_مقصد_cpu);
	kvfree(tbls->cpu_مقصد_cid);
	kvfree(tbls->cid_مقصد_shard);
	kvfree(tbls->shard_گرہ);
	kvfree(tbls->shard_ranges);
	kvfree(tbls->topo);
	kfree(tbls);
}

ساکن خالی scx_cid_tables_free_rcufn(ساخت rcu_سر *rcu)
{
	scx_cid_tables_free(container_کا(rcu, ساخت scx_cid_tables, rcu));
}

ساکن ساخت scx_cid_tables *scx_cid_alloc_tables(خالی)
{
	u32 npossible = عدد_possible_cpus();
	ساخت scx_cid_tables *tbls;

	tbls = kzalloc_obj(*tbls);
	اگر (!tbls)
		واپس تہی;

	tbls->cid_مقصد_cpu = kvzalloc_objs(*tbls->cid_مقصد_cpu, npossible);
	tbls->cpu_مقصد_cid = kvzalloc_objs(*tbls->cpu_مقصد_cid, nr_cpu_ids);
	tbls->cid_مقصد_shard = kvzalloc_objs(*tbls->cid_مقصد_shard, npossible);
	tbls->shard_گرہ = kvzalloc_objs(*tbls->shard_گرہ, npossible);
	tbls->shard_ranges = kvzalloc_objs(*tbls->shard_ranges, npossible);
	tbls->topo = kvzalloc_objs(*tbls->topo, npossible);

	اگر (!tbls->cid_مقصد_cpu || !tbls->cpu_مقصد_cid || !tbls->cid_مقصد_shard ||
	    !tbls->shard_گرہ || !tbls->shard_ranges || !tbls->topo) {
		scx_cid_tables_free(tbls);
		واپس تہی;
	}

	واپس tbls;
}

/**
 * scx_cid_publish_tables - Publish the tables scx_cid_init() built
 *
 * Called after ops.init_cids() where the layout is final.
 */
خالی scx_cid_publish_tables(خالی)
{
	ساخت scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);

	scx_nr_cid_shards = tbls->nr_shards;
	rcu_تفویض_اشارہ_گر(scx_cid_مقصد_cpu_tbl, tbls->cid_مقصد_cpu);
	rcu_تفویض_اشارہ_گر(scx_cpu_مقصد_cid_tbl, tbls->cpu_مقصد_cid);
	rcu_تفویض_اشارہ_گر(scx_cid_مقصد_shard, tbls->cid_مقصد_shard);
	rcu_تفویض_اشارہ_گر(scx_shard_گرہ, tbls->shard_گرہ);
	rcu_تفویض_اشارہ_گر(scx_cid_shard_ranges, tbls->shard_ranges);
	rcu_تفویض_اشارہ_گر(scx_cid_topo, tbls->topo);
}

/**
 * scx_cid_retire_tables - Unpublish and retire the cid tables
 *
 * Called by root disable after the readers which dereference without NULL
 * checks are drained, inside cpus_read_lock() to exclude the hotplug path.
 */
خالی scx_cid_retire_tables(خالی)
{
	ساخت scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);
	lockdep_assert_cpus_held();

	اگر (!tbls)
		واپس;

	scx_cid_tables = تہی;
	RCU_INIT_اشارہ_گر(scx_cid_مقصد_cpu_tbl, تہی);
	RCU_INIT_اشارہ_گر(scx_cpu_مقصد_cid_tbl, تہی);
	RCU_INIT_اشارہ_گر(scx_cid_مقصد_shard, تہی);
	RCU_INIT_اشارہ_گر(scx_shard_گرہ, تہی);
	RCU_INIT_اشارہ_گر(scx_cid_shard_ranges, تہی);
	RCU_INIT_اشارہ_گر(scx_cid_topo, تہی);
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
s32 scx_cid_init(ساخت scx_sched *sch)
{
	cpumask_var_t مقصد_walk __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t گرہ_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t core_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_fallback __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t online_no_topo __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	ساخت scx_cid_tables *tbls;
	u32 اگلا_cid = 0;
	s32 اگلا_گرہ_idx = 0, اگلا_llc_idx = 0, اگلا_core_idx = 0;
	s32 اگلا_shard_idx = 0;
	u32 shard_سائز, max_cids;
	u32 notopo_اندر_shard;
	s32 notopo_shard_cid, notopo_shard_idx;
	s32 cpu, cid, si;

	/* CMASK_MAX_WORDS in cid.bpf.h covers NR_CPUS up to 8192 */
	BUILD_BUG_ON(NR_CPUS > 8192);

	lockdep_assert_cpus_held();
	lockdep_assert_held(&scx_enable_mutex);

	shard_سائز = sch->ops.cid_shard_سائز ?: SCX_CID_SHARD_سائز_DFL;
	max_cids = min_t(u32, shard_سائز, SCX_CID_SHARD_MAX_CPUS);

	tbls = scx_cid_alloc_tables();
	اگر (!tbls)
		واپس -ENOMEM;

	scx_cid_tables = tbls;

	برائے (si = 0; si < عدد_possible_cpus(); si++)
		tbls->shard_گرہ[si] = NUMA_NO_گرہ;

	اگر (!zalloc_cpumask_var(&مقصد_walk, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&گرہ_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&core_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_fallback, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&online_no_topo, GFP_KERNEL))
		واپس -ENOMEM;

	/* -1 sentinels for sparse-possible cpu id holes (0 is a valid cid) */
	برائے (cpu = 0; cpu < nr_cpu_ids; cpu++)
		tbls->cpu_مقصد_cid[cpu] = -1;

	cpumask_copy(مقصد_walk, cpu_online_نقاب);

	جب (!cpumask_خالی_خانے(مقصد_walk)) {
		s32 اگلا_cpu = cpumask_پہلا(مقصد_walk);
		s32 nid = cpu_مقصد_گرہ(اگلا_cpu);
		s32 گرہ_cid = اگلا_cid;
		s32 گرہ_idx;

		/*
		 * No NUMA info: skip and let the tail loop assign a no-topo
		 * cid. cpumask_of_node(-1) is undefined.
		 */
		اگر (nid < 0) {
			cpumask_clear_cpu(اگلا_cpu, مقصد_walk);
			جاری;
		}

		گرہ_idx = اگلا_گرہ_idx++;

		/* node_scratch = to_walk & this node */
		cpumask_and(گرہ_scratch, مقصد_walk, cpumask_کا_گرہ(nid));
		اگر (WARN_ON_ONCE(!cpumask_test_cpu(اگلا_cpu, گرہ_scratch)))
			واپس -EINVAL;

		جب (!cpumask_خالی_خانے(گرہ_scratch)) {
			s32 ncpu = cpumask_پہلا(گرہ_scratch);
			ثابت ساخت cpumask *llc_نقاب = cpu_llc_نقاب(ncpu, llc_fallback);
			s32 llc_cid = اگلا_cid;
			s32 llc_idx = اگلا_llc_idx++;
			u32 cores_فی_shard, nr_large;
			u32 shard_local = 0, cores_اندر_shard = 0, cids_اندر_shard = 0;
			s32 shard_cid, shard_idx;

			/* llc_scratch = node_scratch & this llc */
			cpumask_and(llc_scratch, گرہ_scratch, llc_نقاب);
			اگر (WARN_ON_ONCE(!cpumask_test_cpu(ncpu, llc_scratch)))
				واپس -EINVAL;

			calc_shard_layout(llc_scratch, shard_سائز, &cores_فی_shard, &nr_large);
			shard_cid = اگلا_cid;
			shard_idx = اگلا_shard_idx++;
			tbls->shard_گرہ[shard_idx] = nid;

			جب (!cpumask_خالی_خانے(llc_scratch)) {
				s32 lcpu = cpumask_پہلا(llc_scratch);
				ثابت ساخت cpumask *sib = topology_sibling_cpumask(lcpu);
				s32 core_cid = اگلا_cid;
				s32 core_idx = اگلا_core_idx++;
				s32 ccpu;
				u32 max_cores, cids_اندر_core;

				/* core_scratch = llc_scratch & this core */
				cpumask_and(core_scratch, llc_scratch, sib);
				اگر (WARN_ON_ONCE(!cpumask_test_cpu(lcpu, core_scratch)))
					واپس -EINVAL;

				/*
				 * Advance to a new shard when either core or
				 * cid count reaches max. The latter bounds
				 * shard sizes under uneven SMT. Never start an
				 * empty shard.
				 */
				cids_اندر_core = cpumask_وزن(core_scratch);
				max_cores = cores_فی_shard + (shard_local < nr_large ? 1 : 0);
				اگر (cores_اندر_shard &&
				    (cores_اندر_shard >= max_cores ||
				     cids_اندر_shard + cids_اندر_core > max_cids)) {
					shard_local++;
					cores_اندر_shard = 0;
					cids_اندر_shard = 0;
					shard_cid = اگلا_cid;
					shard_idx = اگلا_shard_idx++;
					tbls->shard_گرہ[shard_idx] = nid;
				}
				cores_اندر_shard++;
				cids_اندر_shard += cids_اندر_core;

				for_each_cpu(ccpu, core_scratch) {
					s32 cid = اگلا_cid++;

					tbls->cid_مقصد_cpu[cid] = ccpu;
					tbls->cpu_مقصد_cid[ccpu] = cid;
					tbls->cid_مقصد_shard[cid] = shard_idx;
					tbls->topo[cid] = (ساخت scx_cid_topo){
						.core_cid = core_cid,
						.core_idx = core_idx,
						.llc_cid = llc_cid,
						.llc_idx = llc_idx,
						.گرہ_cid = گرہ_cid,
						.گرہ_idx = گرہ_idx,
						.shard_cid = shard_cid,
						.shard_idx = shard_idx,
					};

					cpumask_clear_cpu(ccpu, llc_scratch);
					cpumask_clear_cpu(ccpu, گرہ_scratch);
					cpumask_clear_cpu(ccpu, مقصد_walk);
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
	notopo_اندر_shard = min_t(u32, shard_سائز, SCX_CID_SHARD_MAX_CPUS);
	notopo_shard_cid = -1;
	notopo_shard_idx = -1;

	for_each_cpu(cpu, cpu_possible_نقاب) {
		اگر (tbls->cpu_مقصد_cid[cpu] != -1)
			جاری;
		اگر (cpu_online(cpu))
			cpumask_سیٹ_cpu(cpu, online_no_topo);

		cid = اگلا_cid++;
		tbls->cid_مقصد_cpu[cid] = cpu;
		tbls->cpu_مقصد_cid[cpu] = cid;

		اگر (notopo_اندر_shard >= min_t(u32, shard_سائز, SCX_CID_SHARD_MAX_CPUS)) {
			notopo_shard_cid = cid;
			notopo_shard_idx = اگلا_shard_idx++;
			notopo_اندر_shard = 0;
		}
		notopo_اندر_shard++;

		tbls->cid_مقصد_shard[cid] = notopo_shard_idx;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = notopo_shard_cid;
		tbls->topo[cid].shard_idx = notopo_shard_idx;
	}

	اگر (!cpumask_خالی_خانے(llc_fallback))
		pr_warn("scx_cid: cpus without cacheinfo, using node mask as llc: %*pbl\n",
			cpumask_pr_دلائل(llc_fallback));
	اگر (!cpumask_خالی_خانے(online_no_topo))
		pr_warn("scx_cid: online cpus with no usable topology: %*pbl\n",
			cpumask_pr_دلائل(online_no_topo));

	/*
	 * Fill cid_shard_ranges[] from cid_to_shard[]. Shards are contiguous
	 * cid ranges by construction: base_cid is the first cid landing in a
	 * shard, nr_cids is the count.
	 */
	برائے (cid = 0; cid < اگلا_cid; cid++) {
		s32 sidx = tbls->cid_مقصد_shard[cid];

		اگر (tbls->shard_ranges[sidx].nr_cids == 0)
			tbls->shard_ranges[sidx].بنیاد_cid = cid;
		tbls->shard_ranges[sidx].nr_cids++;
	}

	tbls->nr_shards = اگلا_shard_idx;
	واپس 0;
}

/**
 * scx_cmask_clear - Zero every bit in @m's active range
 * @m: cmask to clear
 *
 * Storage past the active range is left as is.
 */
خالی scx_cmask_clear(ساخت scx_cmask *m)
{
	u32 nr_الفاظ;

	اگر (!m->nr_cids)
		واپس;
	nr_الفاظ = (m->بنیاد + m->nr_cids - 1) / 64 - m->بنیاد / 64 + 1;
	memset(m->بٹ, 0, nr_الفاظ * حجم(u64));
}

/**
 * scx_cmask_fill - Set every bit in @m's active range
 * @m: cmask to fill
 *
 * Counterpart to scx_cmask_clear(). Storage past the active range is left as is.
 */
خالی scx_cmask_fill(ساخت scx_cmask *m)
{
	u32 nr_الفاظ, سر_بٹ, دم_بٹ;

	اگر (!m->nr_cids)
		واپس;
	nr_الفاظ = (m->بنیاد + m->nr_cids - 1) / 64 - m->بنیاد / 64 + 1;
	memset(m->بٹ, 0xff, nr_الفاظ * حجم(u64));

	/* clear word-0 bits below base */
	سر_بٹ = m->بنیاد & 63;
	اگر (سر_بٹ)
		m->بٹ[0] &= ~((1ULL << سر_بٹ) - 1);

	/* clear last-word bits at or past base + nr_cids */
	دم_بٹ = (m->بنیاد + m->nr_cids) & 63;
	اگر (دم_بٹ)
		m->بٹ[nr_الفاظ - 1] &= (1ULL << دم_بٹ) - 1;
}

/*
 * Return the index of the largest entry in @counts, or NUMA_NO_NODE if all
 * entries are zero. Ties resolve to the lowest index.
 */
ساکن s32 pick_max_گرہ(ثابت u32 *counts, u32 n)
{
	s32 بہترین = NUMA_NO_گرہ;
	u32 بہترین_گنتی = 0, i;

	برائے (i = 0; i < n; i++) {
		اگر (counts[i] > بہترین_گنتی) {
			بہترین_گنتی = counts[i];
			بہترین = i;
		}
	}
	واپس بہترین;
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
__bpf_kfunc خالی scx_bpf_cid_override(ثابت s32 *cpu_مقصد_cid_arena, u32 cpu_مقصد_cid_cnt,
				      ثابت s32 *shard_آغاز_arena, u32 shard_آغاز_cnt,
				      ثابت ساخت bpf_prog_aux *aux)
{
	cpumask_var_t نظر_آیا __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	u32 *گرہ_counts __free(kfree) = تہی;
	s32 *cpu_مقصد_cid __free(kfree) = تہی;
	s32 *shard_آغاز __free(kfree) = تہی;
	u32 npossible = عدد_possible_cpus();
	ساخت scx_cid_tables *tbls;
	ساخت scx_sched *sch;
	u32 nr_shards = shard_آغاز_cnt;
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
	alloced = zalloc_cpumask_var(&نظر_آیا, GFP_KERNEL);
	گرہ_counts = kzalloc_objs(*گرہ_counts, nr_گرہ_ids);
	اگر (cpu_مقصد_cid_cnt == nr_cpu_ids)
		cpu_مقصد_cid = kmemdup(cpu_مقصد_cid_arena, cpu_مقصد_cid_cnt * حجم(s32),
				     GFP_KERNEL);
	اگر (nr_shards && nr_shards <= npossible)
		shard_آغاز = kmemdup(shard_آغاز_arena, nr_shards * حجم(s32),
				      GFP_KERNEL);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		واپس;

	/* called from ops.init_cids(), so the tables exist and are unpublished */
	lockdep_assert_held(&scx_enable_mutex);
	tbls = scx_cid_tables;

	اگر (cpu_مقصد_cid_cnt != nr_cpu_ids) {
		scx_error(sch, "scx_bpf_cid_override: cpu_to_cid expected %u entries, got %u",
			  nr_cpu_ids, cpu_مقصد_cid_cnt);
		واپس;
	}

	اگر (!nr_shards || nr_shards > npossible) {
		scx_error(sch, "scx_bpf_cid_override: invalid shard_start count %u",
			  nr_shards);
		واپس;
	}

	اگر (!alloced || !گرہ_counts || !cpu_مقصد_cid || !shard_آغاز) {
		scx_error(sch, "scx_bpf_cid_override: allocation failed");
		واپس;
	}

	/* validate shard_start[]: starts at 0, strictly increasing, in range */
	اگر (shard_آغاز[0] != 0) {
		scx_error(sch, "scx_bpf_cid_override: shard_start[0] must be 0, got %d",
			  shard_آغاز[0]);
		واپس;
	}
	برائے (si = 1; si < nr_shards; si++) {
		اگر (shard_آغاز[si] <= shard_آغاز[si - 1]) {
			scx_error(sch, "scx_bpf_cid_override: shard_start not increasing at [%d]",
				  si);
			واپس;
		}
		اگر (shard_آغاز[si] >= npossible) {
			scx_error(sch, "scx_bpf_cid_override: shard_start[%d]=%d >= %u",
				  si, shard_آغاز[si], npossible);
			واپس;
		}
		اگر (shard_آغاز[si] - shard_آغاز[si - 1] > SCX_CID_SHARD_MAX_CPUS) {
			scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
				  si - 1, shard_آغاز[si] - shard_آغاز[si - 1],
				  SCX_CID_SHARD_MAX_CPUS);
			واپس;
		}
	}
	اگر (npossible - shard_آغاز[nr_shards - 1] > SCX_CID_SHARD_MAX_CPUS) {
		scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
			  nr_shards - 1, npossible - shard_آغاز[nr_shards - 1],
			  SCX_CID_SHARD_MAX_CPUS);
		واپس;
	}

	/* validate first so that invalid input leaves the tables untouched */
	for_each_possible_cpu(cpu) {
		s32 c = cpu_مقصد_cid[cpu];

		اگر (!cid_valid(sch, c))
			واپس;
		اگر (cpumask_test_and_سیٹ_cpu(c, نظر_آیا)) {
			scx_error(sch, "cid %d assigned to multiple cpus", c);
			واپس;
		}
	}

	for_each_possible_cpu(cpu) {
		s32 c = cpu_مقصد_cid[cpu];

		tbls->cpu_مقصد_cid[cpu] = c;
		tbls->cid_مقصد_cpu[c] = cpu;
	}

	/*
	 * Derive shard_node[] by majority count: an overridden shard may
	 * span NUMA nodes, so assign each to the node that owns the most cpus.
	 */
	برائے (si = 0; si < nr_shards; si++) {
		u32 انتہا = (si + 1 < nr_shards) ? shard_آغاز[si + 1] : npossible;

		memset(گرہ_counts, 0, nr_گرہ_ids * حجم(*گرہ_counts));
		برائے (cid = shard_آغاز[si]; cid < انتہا; cid++) {
			s32 گرہ = cpu_مقصد_گرہ(tbls->cid_مقصد_cpu[cid]);

			اگر (numa_valid_گرہ(گرہ))
				گرہ_counts[گرہ]++;
		}
		tbls->shard_گرہ[si] = pick_max_گرہ(گرہ_counts, nr_گرہ_ids);
	}

	/*
	 * Invalidate stale topo info and install shard layout from
	 * @shard_start. Walk shards to derive shard_cid/shard_idx for each cid.
	 */
	si = 0;
	برائے (cid = 0; cid < npossible; cid++) {
		اگر (si + 1 < nr_shards && cid >= shard_آغاز[si + 1])
			si++;
		tbls->cid_مقصد_shard[cid] = si;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = shard_آغاز[si];
		tbls->topo[cid].shard_idx = si;
	}

	/* Rebuild shard_ranges[] for the new layout. */
	memset(tbls->shard_ranges, 0, npossible * حجم(*tbls->shard_ranges));
	برائے (si = 0; si < nr_shards; si++) {
		u32 انتہا = (si + 1 < nr_shards) ? shard_آغاز[si + 1] : npossible;

		tbls->shard_ranges[si].بنیاد_cid = shard_آغاز[si];
		tbls->shard_ranges[si].nr_cids = انتہا - shard_آغاز[si];
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
__bpf_kfunc s32 scx_bpf_cid_مقصد_cpu(s32 cid, ثابت ساخت bpf_prog_aux *aux)
{
	ساخت scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		واپس -EINVAL;
	واپس scx_cid_مقصد_cpu(sch, cid);
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
__bpf_kfunc s32 scx_bpf_cpu_مقصد_cid(s32 cpu, ثابت ساخت bpf_prog_aux *aux)
{
	ساخت scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	اگر (unlikely(!sch))
		واپس -EINVAL;
	واپس scx_cpu_مقصد_cid(sch, cpu);
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
شمار cmask_op2 {
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

ساکن __always_inline bool cmask_op2_is_pred(ثابت شمار cmask_op2 op)
{
	واپس op == CMASK_OP2_SUBSET || op == CMASK_OP2_INTERSECTS;
}

ساکن __always_inline bool cmask_لفظ_op2(u64 *av, ثابت u64 *bp, u64 نقاب,
					   ثابت شمار cmask_op2 op)
{
	انتخاب (op) {
	حالت CMASK_OP2_AND:
		WRITE_ONCE(*av, *av & (~نقاب | READ_ONCE(*bp)));
		واپس false;
	حالت CMASK_OP2_OR:
		WRITE_ONCE(*av, *av | (READ_ONCE(*bp) & نقاب));
		واپس false;
	حالت CMASK_OP2_COPY:
		WRITE_ONCE(*av, (*av & ~نقاب) | (READ_ONCE(*bp) & نقاب));
		واپس false;
	حالت CMASK_OP2_ANDNOT:
		WRITE_ONCE(*av, *av & ~(READ_ONCE(*bp) & نقاب));
		واپس false;
	حالت CMASK_OP2_SUBSET:
		/* stop on the first bit in @sub not set in @super */
		واپس (READ_ONCE(*bp) & ~READ_ONCE(*av)) & نقاب;
	حالت CMASK_OP2_INTERSECTS:
		واپس (READ_ONCE(*av) & READ_ONCE(*bp)) & نقاب;
	حالت CMASK_OP2_REF_OR:
		WRITE_ONCE(*av, READ_ONCE(*av) | (READ_ONCE(*bp) & نقاب));
		واپس false;
	حالت CMASK_OP2_REF_COPY:
		WRITE_ONCE(*av, (READ_ONCE(*av) & ~نقاب) | (READ_ONCE(*bp) & نقاب));
		واپس false;
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
ساکن __always_inline bool cmask_walk_op2(u64 *a_بٹ, u32 a_بنیاد, u32 a_nr_cids,
					   ثابت u64 *b_بٹ, u32 b_بنیاد, u32 b_nr_cids,
					   ثابت شمار cmask_op2 op)
{
	u32 lo = max(a_بنیاد, b_بنیاد);
	u32 hi = min(a_بنیاد + a_nr_cids, b_بنیاد + b_nr_cids);
	u32 a_لفظ_off = a_بنیاد / 64;
	u32 b_لفظ_off = b_بنیاد / 64;
	u32 lo_لفظ = lo / 64;
	u32 hi_لفظ = (hi - 1) / 64;
	u64 سر_نقاب = GENMASK_U64(63, lo & 63);
	u64 دم_نقاب = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	اگر (lo >= hi)
		واپس false;

	اگر (lo_لفظ == hi_لفظ)
		واپس cmask_لفظ_op2(&a_بٹ[lo_لفظ - a_لفظ_off],
				      &b_بٹ[lo_لفظ - b_لفظ_off],
				      سر_نقاب & دم_نقاب, op);

	اگر (cmask_لفظ_op2(&a_بٹ[lo_لفظ - a_لفظ_off],
			   &b_بٹ[lo_لفظ - b_لفظ_off], سر_نقاب, op) &&
	    cmask_op2_is_pred(op))
		واپس true;

	برائے (w = lo_لفظ + 1; w < hi_لفظ; w++)
		اگر (cmask_لفظ_op2(&a_بٹ[w - a_لفظ_off],
				   &b_بٹ[w - b_لفظ_off], ~0ULL, op) &&
		    cmask_op2_is_pred(op))
			واپس true;

	واپس cmask_لفظ_op2(&a_بٹ[hi_لفظ - a_لفظ_off],
			      &b_بٹ[hi_لفظ - b_لفظ_off], دم_نقاب, op);
}

شمار cmask_op1 {
	CMASK_OP1_ANY_سیٹ,
};

ساکن __always_inline bool cmask_لفظ_op1(ثابت u64 *ap, u64 نقاب,
					   ثابت شمار cmask_op1 op)
{
	انتخاب (op) {
	حالت CMASK_OP1_ANY_سیٹ:
		واپس READ_ONCE(*ap) & نقاب;
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
ساکن __always_inline bool cmask_walk_op1(ثابت u64 *a_بٹ, u32 a_بنیاد,
					   u32 a_nr_cids,
					   ثابت شمار cmask_op1 op)
{
	u32 lo = a_بنیاد;
	u32 hi = a_بنیاد + a_nr_cids;
	u32 a_لفظ_off = a_بنیاد / 64;
	u32 lo_لفظ = lo / 64;
	u32 hi_لفظ = (hi - 1) / 64;
	u64 سر_نقاب = GENMASK_U64(63, lo & 63);
	u64 دم_نقاب = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	اگر (lo >= hi)
		واپس false;

	اگر (lo_لفظ == hi_لفظ)
		واپس cmask_لفظ_op1(&a_بٹ[lo_لفظ - a_لفظ_off],
				      سر_نقاب & دم_نقاب, op);

	اگر (cmask_لفظ_op1(&a_بٹ[lo_لفظ - a_لفظ_off], سر_نقاب, op))
		واپس true;
	برائے (w = lo_لفظ + 1; w < hi_لفظ; w++)
		اگر (cmask_لفظ_op1(&a_بٹ[w - a_لفظ_off], ~0ULL, op))
			واپس true;
	واپس cmask_لفظ_op1(&a_بٹ[hi_لفظ - a_لفظ_off], دم_نقاب, op);
}

خالی scx_cmask_and(ساخت scx_cmask *dst, ثابت ساخت scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->بنیاد, dst->nr_cids,
		       src->بٹ, src->بنیاد, src->nr_cids, CMASK_OP2_AND);
}

خالی scx_cmask_or(ساخت scx_cmask *dst, ثابت ساخت scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->بنیاد, dst->nr_cids,
		       src->بٹ, src->بنیاد, src->nr_cids, CMASK_OP2_OR);
}

خالی scx_cmask_copy(ساخت scx_cmask *dst, ثابت ساخت scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->بنیاد, dst->nr_cids,
		       src->بٹ, src->بنیاد, src->nr_cids, CMASK_OP2_COPY);
}

خالی scx_cmask_andnot(ساخت scx_cmask *dst, ثابت ساخت scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->بنیاد, dst->nr_cids,
		       src->بٹ, src->بنیاد, src->nr_cids, CMASK_OP2_ANDNOT);
}

/*
 * Return true if @cm has any bit set in [@lo, @hi). Caller must ensure
 * [@lo, @hi) is contained in @cm's range.
 */
ساکن bool cmask_any_سیٹ_اندر_range(ثابت ساخت scx_cmask *cm, u32 lo, u32 hi)
{
	اگر (lo >= hi)
		واپس false;
	واپس cmask_walk_op1(&cm->بٹ[lo / 64 - cm->بنیاد / 64], lo, hi - lo,
			      CMASK_OP1_ANY_سیٹ);
}

/**
 * scx_cmask_subset - test whether @sub is a subset of @super
 * @sub: cmask to test
 * @super: cmask to test against
 *
 * Return true iff every set bit of @sub is also set in @super.
 */
bool scx_cmask_subset(ثابت ساخت scx_cmask *sub, ثابت ساخت scx_cmask *super)
{
	u32 super_انتہا = super->بنیاد + super->nr_cids;
	u32 sub_انتہا = sub->بنیاد + sub->nr_cids;

	/*
	 * Set bits in @sub outside @super's range can't be in @super, so any
	 * such bit means not a subset. The walk below only visits words
	 * common to both ranges, so these need a separate scan.
	 */
	اگر (sub->بنیاد < super->بنیاد &&
	    cmask_any_سیٹ_اندر_range(sub, sub->بنیاد, min(super->بنیاد, sub_انتہا)))
		واپس false;
	اگر (sub_انتہا > super_انتہا &&
	    cmask_any_سیٹ_اندر_range(sub, max(sub->بنیاد, super_انتہا), sub_انتہا))
		واپس false;

	واپس !cmask_walk_op2((u64 *)super->بٹ, super->بنیاد, super->nr_cids,
			       sub->بٹ, sub->بنیاد, sub->nr_cids, CMASK_OP2_SUBSET);
}

bool scx_cmask_intersects(ثابت ساخت scx_cmask *a, ثابت ساخت scx_cmask *b)
{
	واپس cmask_walk_op2((u64 *)a->بٹ, a->بنیاد, a->nr_cids,
			      b->بٹ, b->بنیاد, b->nr_cids, CMASK_OP2_INTERSECTS);
}

/**
 * scx_cmask_empty - Test whether @m has no bits set
 * @m: cmask to test
 *
 * Return true iff @m's active range has no bits set.
 */
bool scx_cmask_خالی_خانے(ثابت ساخت scx_cmask *m)
{
	واپس !cmask_any_سیٹ_اندر_range(m, m->بنیاد, m->بنیاد + m->nr_cids);
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
__bpf_kfunc خالی scx_bpf_cid_topo(s32 cid, ساخت scx_cid_topo *برآمد, سائز_t برآمد_sz,
				  ثابت ساخت bpf_prog_aux *aux)
{
	سائز_t len = min(برآمد_sz, حجم(*برآمد));
	ساخت scx_cid_topo *topo;
	ساخت scx_sched *sch;

	/* the error cases and fields the kernel lacks read as -1 */
	memset(برآمد, 0xff, برآمد_sz);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	topo = rcu_dereference(scx_cid_topo);
	اگر (unlikely(!sch) || !cid_valid(sch, cid) || unlikely(!topo))
		واپس;

	حافظہ_نقل(برآمد, &topo[cid], len);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_آغاز(scx_kfunc_ids_init_cids)
BTF_ID_FLAGS(func, scx_bpf_cid_override, KF_IMPLICIT_دلائل | KF_SLEEPABLE)
BTF_KFUNCS_انتہا(scx_kfunc_ids_init_cids)

ساکن ثابت ساخت btf_kfunc_id_سیٹ scx_kfunc_سیٹ_init_cids = {
	.owner	= THIS_MODULE,
	.سیٹ	= &scx_kfunc_ids_init_cids,
	.filter	= scx_kfunc_context_filter,
};

BTF_KFUNCS_آغاز(scx_kfunc_ids_cid)
BTF_ID_FLAGS(func, scx_bpf_cid_مقصد_cpu, KF_IMPLICIT_دلائل)
BTF_ID_FLAGS(func, scx_bpf_cpu_مقصد_cid, KF_IMPLICIT_دلائل)
BTF_ID_FLAGS(func, scx_bpf_cid_topo, KF_IMPLICIT_دلائل)
BTF_KFUNCS_انتہا(scx_kfunc_ids_cid)

ساکن ثابت ساخت btf_kfunc_id_سیٹ scx_kfunc_سیٹ_cid = {
	.owner	= THIS_MODULE,
	.سیٹ	= &scx_kfunc_ids_cid,
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
صحیح scx_cmask_ref_init(ساخت scx_sched *sch, ثابت ساخت scx_cmask *src,
		       ساخت scx_cmask_ref *ref)
{
	u32 بنیاد, nr_cids, alloc_الفاظ, npossible = عدد_possible_cpus();
	s32 *cid_مقصد_shard;

	بنیاد = READ_ONCE(src->بنیاد);
	nr_cids = READ_ONCE(src->nr_cids);
	alloc_الفاظ = READ_ONCE(src->alloc_الفاظ);

	اگر (unlikely(بنیاد >= npossible || nr_cids > npossible - بنیاد ||
		     SCX_CMASK_NR_الفاظ(nr_cids) > alloc_الفاظ))
		واپس -EINVAL;

	ref->sch = sch;
	ref->src = (ساخت scx_cmask *)src;
	ref->بنیاد = بنیاد;
	ref->nr_cids = nr_cids;

	cid_مقصد_shard = rcu_dereference_all(scx_cid_مقصد_shard);
	ref->shard_پہلا = cid_مقصد_shard[بنیاد];
	اگر (likely(nr_cids))
		ref->shard_انتہا = cid_مقصد_shard[بنیاد + nr_cids - 1] + 1;
	ورنہ
		ref->shard_انتہا = ref->shard_پہلا;

	واپس 0;
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
خالی scx_cmask_ref_init_kern(ساخت scx_sched *sch, ساخت scx_cmask *m,
			     u32 بنیاد, u32 nr_cids, ساخت scx_cmask_ref *ref)
{
	s32 *cid_مقصد_shard;

	WRITE_ONCE(m->بنیاد, بنیاد);
	WRITE_ONCE(m->nr_cids, nr_cids);
	WRITE_ONCE(m->alloc_الفاظ, SCX_CMASK_NR_الفاظ(nr_cids));

	ref->sch = sch;
	ref->src = m;
	ref->بنیاد = بنیاد;
	ref->nr_cids = nr_cids;

	cid_مقصد_shard = rcu_dereference_all(scx_cid_مقصد_shard);
	ref->shard_پہلا = cid_مقصد_shard[بنیاد];
	اگر (likely(nr_cids))
		ref->shard_انتہا = cid_مقصد_shard[بنیاد + nr_cids - 1] + 1;
	ورنہ
		ref->shard_انتہا = ref->shard_پہلا;
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
خالی scx_cmask_ref_shard(ثابت ساخت scx_cmask_ref *ref, s32 shard_idx,
			 ساخت scx_cmask *برآمد)
{
	ثابت ساخت scx_cid_shard *shard =
		&rcu_dereference_all(scx_cid_shard_ranges)[shard_idx];
	u32 shard_بنیاد = shard->بنیاد_cid;
	u32 shard_انتہا = shard_بنیاد + shard->nr_cids;
	u32 isect_بنیاد, isect_انتہا, nr_الفاظ, src_off, wi;
	u64 سر_نقاب, دم_نقاب;

	isect_بنیاد = max(ref->بنیاد, shard_بنیاد);
	isect_انتہا = min(ref->بنیاد + ref->nr_cids, shard_انتہا);

	اگر (isect_بنیاد >= isect_انتہا) {
		برآمد->بنیاد = shard_بنیاد;
		برآمد->nr_cids = 0;
		واپس;
	}

	nr_الفاظ = ((isect_انتہا - 1) / 64) - (isect_بنیاد / 64) + 1;
	اگر (nr_الفاظ > برآمد->alloc_الفاظ) {
		scx_error(ref->sch, "scx_cmask_ref_shard: out alloc_words=%u < %u for shard %d",
			  برآمد->alloc_الفاظ, nr_الفاظ, shard_idx);
		برآمد->بنیاد = shard_بنیاد;
		برآمد->nr_cids = 0;
		واپس;
	}

	برآمد->بنیاد = isect_بنیاد;
	برآمد->nr_cids = isect_انتہا - isect_بنیاد;
	src_off = (isect_بنیاد / 64) - (ref->بنیاد / 64);

	برائے (wi = 0; wi < nr_الفاظ; wi++)
		برآمد->بٹ[wi] = READ_ONCE(ref->src->بٹ[src_off + wi]);

	سر_نقاب = GENMASK_U64(63, isect_بنیاد & 63);
	برآمد->بٹ[0] &= سر_نقاب;
	دم_نقاب = GENMASK_U64((isect_انتہا - 1) & 63, 0);
	برآمد->بٹ[nr_الفاظ - 1] &= دم_نقاب;
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
خالی scx_cmask_ref_or(ثابت ساخت scx_cmask_ref *ref, ثابت ساخت scx_cmask *src)
{
	cmask_walk_op2(ref->src->بٹ, ref->بنیاد, ref->nr_cids,
		       src->بٹ, src->بنیاد, src->nr_cids, CMASK_OP2_REF_OR);
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
خالی scx_cmask_ref_copy(ثابت ساخت scx_cmask_ref *ref, ثابت ساخت scx_cmask *src)
{
	cmask_walk_op2(ref->src->بٹ, ref->بنیاد, ref->nr_cids,
		       src->بٹ, src->بنیاد, src->nr_cids, CMASK_OP2_REF_COPY);
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
خالی scx_cmask_ref_from_cpumask(ثابت ساخت scx_cmask_ref *ref,
				ثابت ساخت cpumask *cpumask)
{
	ساخت scx_cmask *m = ref->src;
	u32 بنیاد = ref->بنیاد, nr_cids = ref->nr_cids;
	u32 wi, nr_الفاظ;

	اگر (!nr_cids)
		واپس;

	nr_الفاظ = (بنیاد + nr_cids - 1) / 64 - بنیاد / 64 + 1;
	برائے (wi = 0; wi < nr_الفاظ; wi++) {
		u32 لفظ_پہلا_cid = (بنیاد / 64 + wi) * 64;
		u64 لفظ = 0;
		u32 ثنائی_ہندسہ_2;

		برائے (ثنائی_ہندسہ_2 = 0; ثنائی_ہندسہ_2 < 64; ثنائی_ہندسہ_2++) {
			u32 cid = لفظ_پہلا_cid + ثنائی_ہندسہ_2;

			اگر (cid < بنیاد || cid >= بنیاد + nr_cids)
				جاری;
			اگر (cpumask_test_cpu(__scx_cid_to_cpu(cid), cpumask))
				لفظ |= ثنائی_ہندسہ_U64(ثنائی_ہندسہ_2);
		}
		WRITE_ONCE(m->بٹ[wi], لفظ);
	}
}

صحیح scx_cid_kfunc_init(خالی)
{
	واپس رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_سیٹ_init_cids) ?:
		رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_سیٹ_cid) ?:
		رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_TRACING, &scx_kfunc_سیٹ_cid) ?:
		رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_سیٹ_cid);
}
