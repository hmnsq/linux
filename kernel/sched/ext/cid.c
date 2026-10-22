/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ضمن <linux/cacheinfo.h>

#ضمن "internal.h"
#ضمن "cid.h"

/*
 * cid tables. The cid kfuncs are available whether the root scheduler is
 * cid-form or cpu-form, the latter to allow gradual migration to cids, so every
 * root builds a default mapping. Each root enable allocates a fresh set, builds
 * it privately and publishes the __rcu globals below once the layout is final.
 * Root disable unpublishes and RCU-frees the set. kfuncs may run before the
 * tables are published and must check for NULL.
 */
u32 scx_nr_cid_shards;
s16 __rcu *scx_cid_منزل_cpu_tbl;
s16 __rcu *scx_cpu_منزل_cid_tbl;
s32 __rcu *scx_cid_منزل_shard;
s32 __rcu *scx_shard_گرہ;
بنية scx_cid_shard __rcu *scx_cid_shard_ranges;
بنية scx_cid_topo __rcu *scx_cid_topo;

ساكن بنية scx_cid_tables *scx_cid_tables;	/* used only during alloc/free */

#تعریف_کرو SCX_CID_TOPO_NEG	(بنية scx_cid_topo) {				\
	.core_cid = -1, .core_idx = -1, .llc_cid = -1, .llc_idx = -1,		\
	.گرہ_cid = -1, .گرہ_idx = -1, .shard_cid = -1, .shard_idx = -1,	\
}

/*
 * Return @cpu's LLC shared_cpu_map. If cacheinfo isn't populated (offline or
 * !present), record @cpu in @fallbacks and return its node mask instead - the
 * worst that can happen is that the cpu's LLC becomes coarser than reality.
 */
ساكن ثابت بنية cpumask *cpu_llc_نقاب(صحيح cpu, بنية cpumask *fallbacks)
{
	بنية cpu_cacheinfo *ci = get_cpu_cacheinfo(cpu);

	إذا (!ci || !ci->info_list || !ci->عدد_leaves) {
		cpumask_سیٹ_cpu(cpu, fallbacks);
		أرجع cpumask_دا_گرہ(cpu_منزل_گرہ(cpu));
	}
	أرجع &ci->info_list[ci->عدد_leaves - 1].shared_cpu_map;
}

/*
 * Compute per-LLC shard layout. Each shard holds at most @shard_size cids, and
 * in any case no more than SCX_CID_SHARD_MAX_CPUS. Cores are spread as evenly
 * as possible across shards so cpu count is balanced: the first *@nr_large_p
 * shards get (*@cores_per_shard_p + 1) cores, the rest get *@cores_per_shard_p.
 */
ساكن فراغ calc_shard_layout(ثابت بنية cpumask *llc_cpus, u32 shard_ناپ,
			      u32 *cores_فی_shard_p, u32 *nr_large_p)
{
	u32 nr_cores = 0, nr_cpus = 0, nr_shards;
	صحيح cpu;

	for_each_cpu(cpu, llc_cpus) {
		nr_cpus++;
		إذا (cpumask_پہلا(topology_sibling_cpumask(cpu)) == cpu)
			nr_cores++;
	}

	nr_shards = max_t(u32, 1, DIV_گیڑا_اتے(nr_cpus, shard_ناپ));
	nr_shards = max_t(u32, nr_shards,
			  DIV_گیڑا_اتے(nr_cpus, SCX_CID_SHARD_MAX_CPUS));

	*cores_فی_shard_p = nr_cores / nr_shards;
	*nr_large_p = nr_cores % nr_shards;
}

ساكن فراغ scx_cid_tables_free(بنية scx_cid_tables *tbls)
{
	إذا (!tbls)
		أرجع;
	kvfree(tbls->cid_منزل_cpu);
	kvfree(tbls->cpu_منزل_cid);
	kvfree(tbls->cid_منزل_shard);
	kvfree(tbls->shard_گرہ);
	kvfree(tbls->shard_ranges);
	kvfree(tbls->topo);
	kfree(tbls);
}

ساكن فراغ scx_cid_tables_free_rcufn(بنية rcu_سر *rcu)
{
	scx_cid_tables_free(container_دا(rcu, بنية scx_cid_tables, rcu));
}

ساكن بنية scx_cid_tables *scx_cid_alloc_tables(فراغ)
{
	u32 npossible = عدد_possible_cpus();
	بنية scx_cid_tables *tbls;

	tbls = kzalloc_obj(*tbls);
	إذا (!tbls)
		أرجع عدم;

	tbls->cid_منزل_cpu = kvzalloc_objs(*tbls->cid_منزل_cpu, npossible);
	tbls->cpu_منزل_cid = kvzalloc_objs(*tbls->cpu_منزل_cid, nr_cpu_ids);
	tbls->cid_منزل_shard = kvzalloc_objs(*tbls->cid_منزل_shard, npossible);
	tbls->shard_گرہ = kvzalloc_objs(*tbls->shard_گرہ, npossible);
	tbls->shard_ranges = kvzalloc_objs(*tbls->shard_ranges, npossible);
	tbls->topo = kvzalloc_objs(*tbls->topo, npossible);

	إذا (!tbls->cid_منزل_cpu || !tbls->cpu_منزل_cid || !tbls->cid_منزل_shard ||
	    !tbls->shard_گرہ || !tbls->shard_ranges || !tbls->topo) {
		scx_cid_tables_free(tbls);
		أرجع عدم;
	}

	أرجع tbls;
}

/**
 * scx_cid_publish_tables - Publish the tables scx_cid_init() built
 *
 * Called after ops.init_cids() where the layout is final.
 */
فراغ scx_cid_publish_tables(فراغ)
{
	بنية scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);

	scx_nr_cid_shards = tbls->nr_shards;
	rcu_سونپو_اشارہ_گر(scx_cid_منزل_cpu_tbl, tbls->cid_منزل_cpu);
	rcu_سونپو_اشارہ_گر(scx_cpu_منزل_cid_tbl, tbls->cpu_منزل_cid);
	rcu_سونپو_اشارہ_گر(scx_cid_منزل_shard, tbls->cid_منزل_shard);
	rcu_سونپو_اشارہ_گر(scx_shard_گرہ, tbls->shard_گرہ);
	rcu_سونپو_اشارہ_گر(scx_cid_shard_ranges, tbls->shard_ranges);
	rcu_سونپو_اشارہ_گر(scx_cid_topo, tbls->topo);
}

/**
 * scx_cid_retire_tables - Unpublish and retire the cid tables
 *
 * Called by root disable after the readers which dereference without NULL
 * checks are drained, inside cpus_read_lock() to exclude the hotplug path.
 */
فراغ scx_cid_retire_tables(فراغ)
{
	بنية scx_cid_tables *tbls = scx_cid_tables;

	lockdep_assert_held(&scx_enable_mutex);
	lockdep_assert_cpus_held();

	إذا (!tbls)
		أرجع;

	scx_cid_tables = عدم;
	RCU_INIT_اشارہ_گر(scx_cid_منزل_cpu_tbl, عدم);
	RCU_INIT_اشارہ_گر(scx_cpu_منزل_cid_tbl, عدم);
	RCU_INIT_اشارہ_گر(scx_cid_منزل_shard, عدم);
	RCU_INIT_اشارہ_گر(scx_shard_گرہ, عدم);
	RCU_INIT_اشارہ_گر(scx_cid_shard_ranges, عدم);
	RCU_INIT_اشارہ_گر(scx_cid_topo, عدم);
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
s32 scx_cid_init(بنية scx_sched *sch)
{
	cpumask_var_t منزل_walk __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t گرہ_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t core_scratch __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t llc_fallback __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	cpumask_var_t online_no_topo __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	بنية scx_cid_tables *tbls;
	u32 اگلا_cid = 0;
	s32 اگلا_گرہ_idx = 0, اگلا_llc_idx = 0, اگلا_core_idx = 0;
	s32 اگلا_shard_idx = 0;
	u32 shard_ناپ, max_cids;
	u32 notopo_وچ_shard;
	s32 notopo_shard_cid, notopo_shard_idx;
	s32 cpu, cid, si;

	/* CMASK_MAX_WORDS in cid.bpf.h covers NR_CPUS up to 8192 */
	BUILD_BUG_ON(NR_CPUS > 8192);

	lockdep_assert_cpus_held();
	lockdep_assert_held(&scx_enable_mutex);

	shard_ناپ = sch->ops.cid_shard_ناپ ?: SCX_CID_SHARD_ناپ_DFL;
	max_cids = min_t(u32, shard_ناپ, SCX_CID_SHARD_MAX_CPUS);

	tbls = scx_cid_alloc_tables();
	إذا (!tbls)
		أرجع -ENOMEM;

	scx_cid_tables = tbls;

	لكل (si = 0; si < عدد_possible_cpus(); si++)
		tbls->shard_گرہ[si] = NUMA_NO_گرہ;

	إذا (!zalloc_cpumask_var(&منزل_walk, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&گرہ_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&core_scratch, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&llc_fallback, GFP_KERNEL) ||
	    !zalloc_cpumask_var(&online_no_topo, GFP_KERNEL))
		أرجع -ENOMEM;

	/* -1 sentinels for sparse-possible cpu id holes (0 is a valid cid) */
	لكل (cpu = 0; cpu < nr_cpu_ids; cpu++)
		tbls->cpu_منزل_cid[cpu] = -1;

	cpumask_copy(منزل_walk, cpu_online_نقاب);

	طالما (!cpumask_خالی_تھاواں(منزل_walk)) {
		s32 اگلا_cpu = cpumask_پہلا(منزل_walk);
		s32 nid = cpu_منزل_گرہ(اگلا_cpu);
		s32 گرہ_cid = اگلا_cid;
		s32 گرہ_idx;

		/*
		 * No NUMA info: skip and let the tail loop assign a no-topo
		 * cid. cpumask_of_node(-1) is undefined.
		 */
		إذا (nid < 0) {
			cpumask_clear_cpu(اگلا_cpu, منزل_walk);
			استمر;
		}

		گرہ_idx = اگلا_گرہ_idx++;

		/* node_scratch = to_walk & this node */
		cpumask_and(گرہ_scratch, منزل_walk, cpumask_دا_گرہ(nid));
		إذا (WARN_ON_ONCE(!cpumask_test_cpu(اگلا_cpu, گرہ_scratch)))
			أرجع -EINVAL;

		طالما (!cpumask_خالی_تھاواں(گرہ_scratch)) {
			s32 ncpu = cpumask_پہلا(گرہ_scratch);
			ثابت بنية cpumask *llc_نقاب = cpu_llc_نقاب(ncpu, llc_fallback);
			s32 llc_cid = اگلا_cid;
			s32 llc_idx = اگلا_llc_idx++;
			u32 cores_فی_shard, nr_large;
			u32 shard_local = 0, cores_وچ_shard = 0, cids_وچ_shard = 0;
			s32 shard_cid, shard_idx;

			/* llc_scratch = node_scratch & this llc */
			cpumask_and(llc_scratch, گرہ_scratch, llc_نقاب);
			إذا (WARN_ON_ONCE(!cpumask_test_cpu(ncpu, llc_scratch)))
				أرجع -EINVAL;

			calc_shard_layout(llc_scratch, shard_ناپ, &cores_فی_shard, &nr_large);
			shard_cid = اگلا_cid;
			shard_idx = اگلا_shard_idx++;
			tbls->shard_گرہ[shard_idx] = nid;

			طالما (!cpumask_خالی_تھاواں(llc_scratch)) {
				s32 lcpu = cpumask_پہلا(llc_scratch);
				ثابت بنية cpumask *sib = topology_sibling_cpumask(lcpu);
				s32 core_cid = اگلا_cid;
				s32 core_idx = اگلا_core_idx++;
				s32 ccpu;
				u32 max_cores, cids_وچ_core;

				/* core_scratch = llc_scratch & this core */
				cpumask_and(core_scratch, llc_scratch, sib);
				إذا (WARN_ON_ONCE(!cpumask_test_cpu(lcpu, core_scratch)))
					أرجع -EINVAL;

				/*
				 * Advance to a new shard when either core or
				 * cid count reaches max. The latter bounds
				 * shard sizes under uneven SMT. Never start an
				 * empty shard.
				 */
				cids_وچ_core = cpumask_بھار(core_scratch);
				max_cores = cores_فی_shard + (shard_local < nr_large ? 1 : 0);
				إذا (cores_وچ_shard &&
				    (cores_وچ_shard >= max_cores ||
				     cids_وچ_shard + cids_وچ_core > max_cids)) {
					shard_local++;
					cores_وچ_shard = 0;
					cids_وچ_shard = 0;
					shard_cid = اگلا_cid;
					shard_idx = اگلا_shard_idx++;
					tbls->shard_گرہ[shard_idx] = nid;
				}
				cores_وچ_shard++;
				cids_وچ_shard += cids_وچ_core;

				for_each_cpu(ccpu, core_scratch) {
					s32 cid = اگلا_cid++;

					tbls->cid_منزل_cpu[cid] = ccpu;
					tbls->cpu_منزل_cid[ccpu] = cid;
					tbls->cid_منزل_shard[cid] = shard_idx;
					tbls->topo[cid] = (بنية scx_cid_topo){
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
					cpumask_clear_cpu(ccpu, منزل_walk);
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
	notopo_وچ_shard = min_t(u32, shard_ناپ, SCX_CID_SHARD_MAX_CPUS);
	notopo_shard_cid = -1;
	notopo_shard_idx = -1;

	for_each_cpu(cpu, cpu_possible_نقاب) {
		إذا (tbls->cpu_منزل_cid[cpu] != -1)
			استمر;
		إذا (cpu_online(cpu))
			cpumask_سیٹ_cpu(cpu, online_no_topo);

		cid = اگلا_cid++;
		tbls->cid_منزل_cpu[cid] = cpu;
		tbls->cpu_منزل_cid[cpu] = cid;

		إذا (notopo_وچ_shard >= min_t(u32, shard_ناپ, SCX_CID_SHARD_MAX_CPUS)) {
			notopo_shard_cid = cid;
			notopo_shard_idx = اگلا_shard_idx++;
			notopo_وچ_shard = 0;
		}
		notopo_وچ_shard++;

		tbls->cid_منزل_shard[cid] = notopo_shard_idx;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = notopo_shard_cid;
		tbls->topo[cid].shard_idx = notopo_shard_idx;
	}

	إذا (!cpumask_خالی_تھاواں(llc_fallback))
		pr_warn("scx_cid: cpus without cacheinfo, using node mask as llc: %*pbl\n",
			cpumask_pr_دلیلاں(llc_fallback));
	إذا (!cpumask_خالی_تھاواں(online_no_topo))
		pr_warn("scx_cid: online cpus with no usable topology: %*pbl\n",
			cpumask_pr_دلیلاں(online_no_topo));

	/*
	 * Fill cid_shard_ranges[] from cid_to_shard[]. Shards are contiguous
	 * cid ranges by construction: base_cid is the first cid landing in a
	 * shard, nr_cids is the count.
	 */
	لكل (cid = 0; cid < اگلا_cid; cid++) {
		s32 sidx = tbls->cid_منزل_shard[cid];

		إذا (tbls->shard_ranges[sidx].nr_cids == 0)
			tbls->shard_ranges[sidx].ادھار_cid = cid;
		tbls->shard_ranges[sidx].nr_cids++;
	}

	tbls->nr_shards = اگلا_shard_idx;
	أرجع 0;
}

/**
 * scx_cmask_clear - Zero every bit in @m's active range
 * @m: cmask to clear
 *
 * Storage past the active range is left as is.
 */
فراغ scx_cmask_clear(بنية scx_cmask *m)
{
	u32 nr_الفاظ;

	إذا (!m->nr_cids)
		أرجع;
	nr_الفاظ = (m->ادھار + m->nr_cids - 1) / 64 - m->ادھار / 64 + 1;
	memset(m->بٹ, 0, nr_الفاظ * حجم(u64));
}

/**
 * scx_cmask_fill - Set every bit in @m's active range
 * @m: cmask to fill
 *
 * Counterpart to scx_cmask_clear(). Storage past the active range is left as is.
 */
فراغ scx_cmask_fill(بنية scx_cmask *m)
{
	u32 nr_الفاظ, سر_بٹ, پوچھ_بٹ;

	إذا (!m->nr_cids)
		أرجع;
	nr_الفاظ = (m->ادھار + m->nr_cids - 1) / 64 - m->ادھار / 64 + 1;
	memset(m->بٹ, 0xff, nr_الفاظ * حجم(u64));

	/* clear word-0 bits below base */
	سر_بٹ = m->ادھار & 63;
	إذا (سر_بٹ)
		m->بٹ[0] &= ~((1ULL << سر_بٹ) - 1);

	/* clear last-word bits at or past base + nr_cids */
	پوچھ_بٹ = (m->ادھار + m->nr_cids) & 63;
	إذا (پوچھ_بٹ)
		m->بٹ[nr_الفاظ - 1] &= (1ULL << پوچھ_بٹ) - 1;
}

/*
 * Return the index of the largest entry in @counts, or NUMA_NO_NODE if all
 * entries are zero. Ties resolve to the lowest index.
 */
ساكن s32 pick_max_گرہ(ثابت u32 *counts, u32 n)
{
	s32 بہترین = NUMA_NO_گرہ;
	u32 بہترین_گنتی = 0, i;

	لكل (i = 0; i < n; i++) {
		إذا (counts[i] > بہترین_گنتی) {
			بہترین_گنتی = counts[i];
			بہترین = i;
		}
	}
	أرجع بہترین;
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
__bpf_kfunc فراغ scx_bpf_cid_override(ثابت s32 *cpu_منزل_cid_arena, u32 cpu_منزل_cid_cnt,
				      ثابت s32 *shard_شروع_arena, u32 shard_شروع_cnt,
				      ثابت بنية bpf_prog_aux *aux)
{
	cpumask_var_t تکیا __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	u32 *گرہ_counts __free(kfree) = عدم;
	s32 *cpu_منزل_cid __free(kfree) = عدم;
	s32 *shard_شروع __free(kfree) = عدم;
	u32 npossible = عدد_possible_cpus();
	بنية scx_cid_tables *tbls;
	بنية scx_sched *sch;
	u32 nr_shards = shard_شروع_cnt;
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
	alloced = zalloc_cpumask_var(&تکیا, GFP_KERNEL);
	گرہ_counts = kzalloc_objs(*گرہ_counts, nr_گرہ_ids);
	إذا (cpu_منزل_cid_cnt == nr_cpu_ids)
		cpu_منزل_cid = kmemdup(cpu_منزل_cid_arena, cpu_منزل_cid_cnt * حجم(s32),
				     GFP_KERNEL);
	إذا (nr_shards && nr_shards <= npossible)
		shard_شروع = kmemdup(shard_شروع_arena, nr_shards * حجم(s32),
				      GFP_KERNEL);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع;

	/* called from ops.init_cids(), so the tables exist and are unpublished */
	lockdep_assert_held(&scx_enable_mutex);
	tbls = scx_cid_tables;

	إذا (cpu_منزل_cid_cnt != nr_cpu_ids) {
		scx_error(sch, "scx_bpf_cid_override: cpu_to_cid expected %u entries, got %u",
			  nr_cpu_ids, cpu_منزل_cid_cnt);
		أرجع;
	}

	إذا (!nr_shards || nr_shards > npossible) {
		scx_error(sch, "scx_bpf_cid_override: invalid shard_start count %u",
			  nr_shards);
		أرجع;
	}

	إذا (!alloced || !گرہ_counts || !cpu_منزل_cid || !shard_شروع) {
		scx_error(sch, "scx_bpf_cid_override: allocation failed");
		أرجع;
	}

	/* validate shard_start[]: starts at 0, strictly increasing, in range */
	إذا (shard_شروع[0] != 0) {
		scx_error(sch, "scx_bpf_cid_override: shard_start[0] must be 0, got %d",
			  shard_شروع[0]);
		أرجع;
	}
	لكل (si = 1; si < nr_shards; si++) {
		إذا (shard_شروع[si] <= shard_شروع[si - 1]) {
			scx_error(sch, "scx_bpf_cid_override: shard_start not increasing at [%d]",
				  si);
			أرجع;
		}
		إذا (shard_شروع[si] >= npossible) {
			scx_error(sch, "scx_bpf_cid_override: shard_start[%d]=%d >= %u",
				  si, shard_شروع[si], npossible);
			أرجع;
		}
		إذا (shard_شروع[si] - shard_شروع[si - 1] > SCX_CID_SHARD_MAX_CPUS) {
			scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
				  si - 1, shard_شروع[si] - shard_شروع[si - 1],
				  SCX_CID_SHARD_MAX_CPUS);
			أرجع;
		}
	}
	إذا (npossible - shard_شروع[nr_shards - 1] > SCX_CID_SHARD_MAX_CPUS) {
		scx_error(sch, "scx_bpf_cid_override: shard[%d] span %d exceeds max %d",
			  nr_shards - 1, npossible - shard_شروع[nr_shards - 1],
			  SCX_CID_SHARD_MAX_CPUS);
		أرجع;
	}

	/* validate first so that invalid input leaves the tables untouched */
	for_each_possible_cpu(cpu) {
		s32 c = cpu_منزل_cid[cpu];

		إذا (!cid_valid(sch, c))
			أرجع;
		إذا (cpumask_test_and_سیٹ_cpu(c, تکیا)) {
			scx_error(sch, "cid %d assigned to multiple cpus", c);
			أرجع;
		}
	}

	for_each_possible_cpu(cpu) {
		s32 c = cpu_منزل_cid[cpu];

		tbls->cpu_منزل_cid[cpu] = c;
		tbls->cid_منزل_cpu[c] = cpu;
	}

	/*
	 * Derive shard_node[] by majority count: an overridden shard may
	 * span NUMA nodes, so assign each to the node that owns the most cpus.
	 */
	لكل (si = 0; si < nr_shards; si++) {
		u32 سرا = (si + 1 < nr_shards) ? shard_شروع[si + 1] : npossible;

		memset(گرہ_counts, 0, nr_گرہ_ids * حجم(*گرہ_counts));
		لكل (cid = shard_شروع[si]; cid < سرا; cid++) {
			s32 گرہ = cpu_منزل_گرہ(tbls->cid_منزل_cpu[cid]);

			إذا (numa_valid_گرہ(گرہ))
				گرہ_counts[گرہ]++;
		}
		tbls->shard_گرہ[si] = pick_max_گرہ(گرہ_counts, nr_گرہ_ids);
	}

	/*
	 * Invalidate stale topo info and install shard layout from
	 * @shard_start. Walk shards to derive shard_cid/shard_idx for each cid.
	 */
	si = 0;
	لكل (cid = 0; cid < npossible; cid++) {
		إذا (si + 1 < nr_shards && cid >= shard_شروع[si + 1])
			si++;
		tbls->cid_منزل_shard[cid] = si;
		tbls->topo[cid] = SCX_CID_TOPO_NEG;
		tbls->topo[cid].shard_cid = shard_شروع[si];
		tbls->topo[cid].shard_idx = si;
	}

	/* Rebuild shard_ranges[] for the new layout. */
	memset(tbls->shard_ranges, 0, npossible * حجم(*tbls->shard_ranges));
	لكل (si = 0; si < nr_shards; si++) {
		u32 سرا = (si + 1 < nr_shards) ? shard_شروع[si + 1] : npossible;

		tbls->shard_ranges[si].ادھار_cid = shard_شروع[si];
		tbls->shard_ranges[si].nr_cids = سرا - shard_شروع[si];
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
__bpf_kfunc s32 scx_bpf_cid_منزل_cpu(s32 cid, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -EINVAL;
	أرجع scx_cid_منزل_cpu(sch, cid);
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
__bpf_kfunc s32 scx_bpf_cpu_منزل_cid(s32 cpu, ثابت بنية bpf_prog_aux *aux)
{
	بنية scx_sched *sch;

	guard(rcu)();

	sch = scx_prog_sched(aux);
	إذا (unlikely(!sch))
		أرجع -EINVAL;
	أرجع scx_cpu_منزل_cid(sch, cpu);
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
تعداد cmask_op2 {
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

ساكن __always_inline bool cmask_op2_is_pred(ثابت تعداد cmask_op2 op)
{
	أرجع op == CMASK_OP2_SUBSET || op == CMASK_OP2_INTERSECTS;
}

ساكن __always_inline bool cmask_لفظ_op2(u64 *av, ثابت u64 *bp, u64 نقاب,
					   ثابت تعداد cmask_op2 op)
{
	بدل (op) {
	حالة CMASK_OP2_AND:
		WRITE_ONCE(*av, *av & (~نقاب | READ_ONCE(*bp)));
		أرجع false;
	حالة CMASK_OP2_OR:
		WRITE_ONCE(*av, *av | (READ_ONCE(*bp) & نقاب));
		أرجع false;
	حالة CMASK_OP2_COPY:
		WRITE_ONCE(*av, (*av & ~نقاب) | (READ_ONCE(*bp) & نقاب));
		أرجع false;
	حالة CMASK_OP2_ANDNOT:
		WRITE_ONCE(*av, *av & ~(READ_ONCE(*bp) & نقاب));
		أرجع false;
	حالة CMASK_OP2_SUBSET:
		/* stop on the first bit in @sub not set in @super */
		أرجع (READ_ONCE(*bp) & ~READ_ONCE(*av)) & نقاب;
	حالة CMASK_OP2_INTERSECTS:
		أرجع (READ_ONCE(*av) & READ_ONCE(*bp)) & نقاب;
	حالة CMASK_OP2_REF_OR:
		WRITE_ONCE(*av, READ_ONCE(*av) | (READ_ONCE(*bp) & نقاب));
		أرجع false;
	حالة CMASK_OP2_REF_COPY:
		WRITE_ONCE(*av, (READ_ONCE(*av) & ~نقاب) | (READ_ONCE(*bp) & نقاب));
		أرجع false;
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
ساكن __always_inline bool cmask_walk_op2(u64 *a_بٹ, u32 a_ادھار, u32 a_nr_cids,
					   ثابت u64 *b_بٹ, u32 b_ادھار, u32 b_nr_cids,
					   ثابت تعداد cmask_op2 op)
{
	u32 lo = max(a_ادھار, b_ادھار);
	u32 hi = min(a_ادھار + a_nr_cids, b_ادھار + b_nr_cids);
	u32 a_لفظ_off = a_ادھار / 64;
	u32 b_لفظ_off = b_ادھار / 64;
	u32 lo_لفظ = lo / 64;
	u32 hi_لفظ = (hi - 1) / 64;
	u64 سر_نقاب = GENMASK_U64(63, lo & 63);
	u64 پوچھ_نقاب = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	إذا (lo >= hi)
		أرجع false;

	إذا (lo_لفظ == hi_لفظ)
		أرجع cmask_لفظ_op2(&a_بٹ[lo_لفظ - a_لفظ_off],
				      &b_بٹ[lo_لفظ - b_لفظ_off],
				      سر_نقاب & پوچھ_نقاب, op);

	إذا (cmask_لفظ_op2(&a_بٹ[lo_لفظ - a_لفظ_off],
			   &b_بٹ[lo_لفظ - b_لفظ_off], سر_نقاب, op) &&
	    cmask_op2_is_pred(op))
		أرجع true;

	لكل (w = lo_لفظ + 1; w < hi_لفظ; w++)
		إذا (cmask_لفظ_op2(&a_بٹ[w - a_لفظ_off],
				   &b_بٹ[w - b_لفظ_off], ~0ULL, op) &&
		    cmask_op2_is_pred(op))
			أرجع true;

	أرجع cmask_لفظ_op2(&a_بٹ[hi_لفظ - a_لفظ_off],
			      &b_بٹ[hi_لفظ - b_لفظ_off], پوچھ_نقاب, op);
}

تعداد cmask_op1 {
	CMASK_OP1_ANY_سیٹ,
};

ساكن __always_inline bool cmask_لفظ_op1(ثابت u64 *ap, u64 نقاب,
					   ثابت تعداد cmask_op1 op)
{
	بدل (op) {
	حالة CMASK_OP1_ANY_سیٹ:
		أرجع READ_ONCE(*ap) & نقاب;
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
ساكن __always_inline bool cmask_walk_op1(ثابت u64 *a_بٹ, u32 a_ادھار,
					   u32 a_nr_cids,
					   ثابت تعداد cmask_op1 op)
{
	u32 lo = a_ادھار;
	u32 hi = a_ادھار + a_nr_cids;
	u32 a_لفظ_off = a_ادھار / 64;
	u32 lo_لفظ = lo / 64;
	u32 hi_لفظ = (hi - 1) / 64;
	u64 سر_نقاب = GENMASK_U64(63, lo & 63);
	u64 پوچھ_نقاب = GENMASK_U64((hi - 1) & 63, 0);
	u32 w;

	إذا (lo >= hi)
		أرجع false;

	إذا (lo_لفظ == hi_لفظ)
		أرجع cmask_لفظ_op1(&a_بٹ[lo_لفظ - a_لفظ_off],
				      سر_نقاب & پوچھ_نقاب, op);

	إذا (cmask_لفظ_op1(&a_بٹ[lo_لفظ - a_لفظ_off], سر_نقاب, op))
		أرجع true;
	لكل (w = lo_لفظ + 1; w < hi_لفظ; w++)
		إذا (cmask_لفظ_op1(&a_بٹ[w - a_لفظ_off], ~0ULL, op))
			أرجع true;
	أرجع cmask_لفظ_op1(&a_بٹ[hi_لفظ - a_لفظ_off], پوچھ_نقاب, op);
}

فراغ scx_cmask_and(بنية scx_cmask *dst, ثابت بنية scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->ادھار, dst->nr_cids,
		       src->بٹ, src->ادھار, src->nr_cids, CMASK_OP2_AND);
}

فراغ scx_cmask_or(بنية scx_cmask *dst, ثابت بنية scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->ادھار, dst->nr_cids,
		       src->بٹ, src->ادھار, src->nr_cids, CMASK_OP2_OR);
}

فراغ scx_cmask_copy(بنية scx_cmask *dst, ثابت بنية scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->ادھار, dst->nr_cids,
		       src->بٹ, src->ادھار, src->nr_cids, CMASK_OP2_COPY);
}

فراغ scx_cmask_andnot(بنية scx_cmask *dst, ثابت بنية scx_cmask *src)
{
	cmask_walk_op2(dst->بٹ, dst->ادھار, dst->nr_cids,
		       src->بٹ, src->ادھار, src->nr_cids, CMASK_OP2_ANDNOT);
}

/*
 * Return true if @cm has any bit set in [@lo, @hi). Caller must ensure
 * [@lo, @hi) is contained in @cm's range.
 */
ساكن bool cmask_any_سیٹ_وچ_range(ثابت بنية scx_cmask *cm, u32 lo, u32 hi)
{
	إذا (lo >= hi)
		أرجع false;
	أرجع cmask_walk_op1(&cm->بٹ[lo / 64 - cm->ادھار / 64], lo, hi - lo,
			      CMASK_OP1_ANY_سیٹ);
}

/**
 * scx_cmask_subset - test whether @sub is a subset of @super
 * @sub: cmask to test
 * @super: cmask to test against
 *
 * Return true iff every set bit of @sub is also set in @super.
 */
bool scx_cmask_subset(ثابت بنية scx_cmask *sub, ثابت بنية scx_cmask *super)
{
	u32 super_سرا = super->ادھار + super->nr_cids;
	u32 sub_سرا = sub->ادھار + sub->nr_cids;

	/*
	 * Set bits in @sub outside @super's range can't be in @super, so any
	 * such bit means not a subset. The walk below only visits words
	 * common to both ranges, so these need a separate scan.
	 */
	إذا (sub->ادھار < super->ادھار &&
	    cmask_any_سیٹ_وچ_range(sub, sub->ادھار, min(super->ادھار, sub_سرا)))
		أرجع false;
	إذا (sub_سرا > super_سرا &&
	    cmask_any_سیٹ_وچ_range(sub, max(sub->ادھار, super_سرا), sub_سرا))
		أرجع false;

	أرجع !cmask_walk_op2((u64 *)super->بٹ, super->ادھار, super->nr_cids,
			       sub->بٹ, sub->ادھار, sub->nr_cids, CMASK_OP2_SUBSET);
}

bool scx_cmask_intersects(ثابت بنية scx_cmask *a, ثابت بنية scx_cmask *b)
{
	أرجع cmask_walk_op2((u64 *)a->بٹ, a->ادھار, a->nr_cids,
			      b->بٹ, b->ادھار, b->nr_cids, CMASK_OP2_INTERSECTS);
}

/**
 * scx_cmask_empty - Test whether @m has no bits set
 * @m: cmask to test
 *
 * Return true iff @m's active range has no bits set.
 */
bool scx_cmask_خالی_تھاواں(ثابت بنية scx_cmask *m)
{
	أرجع !cmask_any_سیٹ_وچ_range(m, m->ادھار, m->ادھار + m->nr_cids);
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
__bpf_kfunc فراغ scx_bpf_cid_topo(s32 cid, بنية scx_cid_topo *باہرلا, ناپ_t باہرلا_sz,
				  ثابت بنية bpf_prog_aux *aux)
{
	ناپ_t len = min(باہرلا_sz, حجم(*باہرلا));
	بنية scx_cid_topo *topo;
	بنية scx_sched *sch;

	/* the error cases and fields the kernel lacks read as -1 */
	memset(باہرلا, 0xff, باہرلا_sz);

	guard(rcu)();

	sch = scx_prog_sched(aux);
	topo = rcu_dereference(scx_cid_topo);
	إذا (unlikely(!sch) || !cid_valid(sch, cid) || unlikely(!topo))
		أرجع;

	حافظہ_نقل(باہرلا, &topo[cid], len);
}

__bpf_kfunc_end_defs();

BTF_KFUNCS_شروع(scx_kfunc_ids_init_cids)
BTF_ID_FLAGS(func, scx_bpf_cid_override, KF_IMPLICIT_دلیلاں | KF_SLEEPABLE)
BTF_KFUNCS_سرا(scx_kfunc_ids_init_cids)

ساكن ثابت بنية btf_kfunc_id_سیٹ scx_kfunc_سیٹ_init_cids = {
	.owner	= THIS_MODULE,
	.سیٹ	= &scx_kfunc_ids_init_cids,
	.filter	= scx_kfunc_context_filter,
};

BTF_KFUNCS_شروع(scx_kfunc_ids_cid)
BTF_ID_FLAGS(func, scx_bpf_cid_منزل_cpu, KF_IMPLICIT_دلیلاں)
BTF_ID_FLAGS(func, scx_bpf_cpu_منزل_cid, KF_IMPLICIT_دلیلاں)
BTF_ID_FLAGS(func, scx_bpf_cid_topo, KF_IMPLICIT_دلیلاں)
BTF_KFUNCS_سرا(scx_kfunc_ids_cid)

ساكن ثابت بنية btf_kfunc_id_سیٹ scx_kfunc_سیٹ_cid = {
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
صحيح scx_cmask_ref_init(بنية scx_sched *sch, ثابت بنية scx_cmask *src,
		       بنية scx_cmask_ref *ref)
{
	u32 ادھار, nr_cids, alloc_الفاظ, npossible = عدد_possible_cpus();
	s32 *cid_منزل_shard;

	ادھار = READ_ONCE(src->ادھار);
	nr_cids = READ_ONCE(src->nr_cids);
	alloc_الفاظ = READ_ONCE(src->alloc_الفاظ);

	إذا (unlikely(ادھار >= npossible || nr_cids > npossible - ادھار ||
		     SCX_CMASK_NR_الفاظ(nr_cids) > alloc_الفاظ))
		أرجع -EINVAL;

	ref->sch = sch;
	ref->src = (بنية scx_cmask *)src;
	ref->ادھار = ادھار;
	ref->nr_cids = nr_cids;

	cid_منزل_shard = rcu_dereference_all(scx_cid_منزل_shard);
	ref->shard_پہلا = cid_منزل_shard[ادھار];
	إذا (likely(nr_cids))
		ref->shard_سرا = cid_منزل_shard[ادھار + nr_cids - 1] + 1;
	وإلا
		ref->shard_سرا = ref->shard_پہلا;

	أرجع 0;
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
فراغ scx_cmask_ref_init_kern(بنية scx_sched *sch, بنية scx_cmask *m,
			     u32 ادھار, u32 nr_cids, بنية scx_cmask_ref *ref)
{
	s32 *cid_منزل_shard;

	WRITE_ONCE(m->ادھار, ادھار);
	WRITE_ONCE(m->nr_cids, nr_cids);
	WRITE_ONCE(m->alloc_الفاظ, SCX_CMASK_NR_الفاظ(nr_cids));

	ref->sch = sch;
	ref->src = m;
	ref->ادھار = ادھار;
	ref->nr_cids = nr_cids;

	cid_منزل_shard = rcu_dereference_all(scx_cid_منزل_shard);
	ref->shard_پہلا = cid_منزل_shard[ادھار];
	إذا (likely(nr_cids))
		ref->shard_سرا = cid_منزل_shard[ادھار + nr_cids - 1] + 1;
	وإلا
		ref->shard_سرا = ref->shard_پہلا;
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
فراغ scx_cmask_ref_shard(ثابت بنية scx_cmask_ref *ref, s32 shard_idx,
			 بنية scx_cmask *باہرلا)
{
	ثابت بنية scx_cid_shard *shard =
		&rcu_dereference_all(scx_cid_shard_ranges)[shard_idx];
	u32 shard_ادھار = shard->ادھار_cid;
	u32 shard_سرا = shard_ادھار + shard->nr_cids;
	u32 isect_ادھار, isect_سرا, nr_الفاظ, src_off, wi;
	u64 سر_نقاب, پوچھ_نقاب;

	isect_ادھار = max(ref->ادھار, shard_ادھار);
	isect_سرا = min(ref->ادھار + ref->nr_cids, shard_سرا);

	إذا (isect_ادھار >= isect_سرا) {
		باہرلا->ادھار = shard_ادھار;
		باہرلا->nr_cids = 0;
		أرجع;
	}

	nr_الفاظ = ((isect_سرا - 1) / 64) - (isect_ادھار / 64) + 1;
	إذا (nr_الفاظ > باہرلا->alloc_الفاظ) {
		scx_error(ref->sch, "scx_cmask_ref_shard: out alloc_words=%u < %u for shard %d",
			  باہرلا->alloc_الفاظ, nr_الفاظ, shard_idx);
		باہرلا->ادھار = shard_ادھار;
		باہرلا->nr_cids = 0;
		أرجع;
	}

	باہرلا->ادھار = isect_ادھار;
	باہرلا->nr_cids = isect_سرا - isect_ادھار;
	src_off = (isect_ادھار / 64) - (ref->ادھار / 64);

	لكل (wi = 0; wi < nr_الفاظ; wi++)
		باہرلا->بٹ[wi] = READ_ONCE(ref->src->بٹ[src_off + wi]);

	سر_نقاب = GENMASK_U64(63, isect_ادھار & 63);
	باہرلا->بٹ[0] &= سر_نقاب;
	پوچھ_نقاب = GENMASK_U64((isect_سرا - 1) & 63, 0);
	باہرلا->بٹ[nr_الفاظ - 1] &= پوچھ_نقاب;
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
فراغ scx_cmask_ref_or(ثابت بنية scx_cmask_ref *ref, ثابت بنية scx_cmask *src)
{
	cmask_walk_op2(ref->src->بٹ, ref->ادھار, ref->nr_cids,
		       src->بٹ, src->ادھار, src->nr_cids, CMASK_OP2_REF_OR);
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
فراغ scx_cmask_ref_copy(ثابت بنية scx_cmask_ref *ref, ثابت بنية scx_cmask *src)
{
	cmask_walk_op2(ref->src->بٹ, ref->ادھار, ref->nr_cids,
		       src->بٹ, src->ادھار, src->nr_cids, CMASK_OP2_REF_COPY);
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
فراغ scx_cmask_ref_from_cpumask(ثابت بنية scx_cmask_ref *ref,
				ثابت بنية cpumask *cpumask)
{
	بنية scx_cmask *m = ref->src;
	u32 ادھار = ref->ادھار, nr_cids = ref->nr_cids;
	u32 wi, nr_الفاظ;

	إذا (!nr_cids)
		أرجع;

	nr_الفاظ = (ادھار + nr_cids - 1) / 64 - ادھار / 64 + 1;
	لكل (wi = 0; wi < nr_الفاظ; wi++) {
		u32 لفظ_پہلا_cid = (ادھار / 64 + wi) * 64;
		u64 لفظ = 0;
		u32 ثنائی_ہندسہ_2;

		لكل (ثنائی_ہندسہ_2 = 0; ثنائی_ہندسہ_2 < 64; ثنائی_ہندسہ_2++) {
			u32 cid = لفظ_پہلا_cid + ثنائی_ہندسہ_2;

			إذا (cid < ادھار || cid >= ادھار + nr_cids)
				استمر;
			إذا (cpumask_test_cpu(__scx_cid_to_cpu(cid), cpumask))
				لفظ |= ثنائی_ہندسہ_U64(ثنائی_ہندسہ_2);
		}
		WRITE_ONCE(m->بٹ[wi], لفظ);
	}
}

صحيح scx_cid_kfunc_init(فراغ)
{
	أرجع رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_سیٹ_init_cids) ?:
		رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_STRUCT_OPS, &scx_kfunc_سیٹ_cid) ?:
		رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_TRACING, &scx_kfunc_سیٹ_cid) ?:
		رجسٹر_btf_kfunc_id_سیٹ(BPF_PROG_TYPE_SYSCALL, &scx_kfunc_سیٹ_cid);
}
