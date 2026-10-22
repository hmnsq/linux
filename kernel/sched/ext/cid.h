/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Topological CPU IDs (cids)
 * --------------------------
 *
 * Raw cpu numbers are clumsy for sharding work and communication across
 * topology units, especially from BPF: the space can be sparse, numerical
 * closeness doesn't imply topological closeness (x86 hyperthreading often puts
 * SMT siblings far apart), and a range of cpu ids doesn't mean anything.
 * Sub-scheds make this acute - cpu allocation, revocation and other state are
 * constantly communicated across sub-scheds, and passing whole cpumasks scales
 * poorly with cpu count. cpumasks are also awkward in BPF: a variable-length
 * kernel type sized for the maximum NR_CPUS (4k), with verbose helper sequences
 * for every op.
 *
 * cids give every cpu a dense, topology-ordered id. CPUs sharing a core, LLC or
 * NUMA node get contiguous cid ranges, so a topology unit becomes a (start,
 * length) slice of cid space. Communication can pass a slice instead of a
 * cpumask, and BPF code can process, for example, a u64 word's worth of cids at
 * a time.
 *
 * The mapping is built once at root scheduler enable time by walking the
 * topology of online cpus only. Going by online cpus is out of necessity:
 * depending on the arch, topology info isn't reliably available for offline
 * cpus. The expected usage model is restarting the scheduler on hotplug events
 * so the mapping is rebuilt against the new online set. A scheduler that wants
 * to handle hotplug without a restart can provide its own cid and shard mapping
 * through the override interface.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_EXT_CID_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_EXT_CID_H

#ಅನ್ತರ್ಭಾವ "internal.h"

ರಚನೆ scx_sched;

/*
 * Cid space (total is always num_possible_cpus()) is laid out with
 * topology-annotated cids first, then no-topo cids at the tail. The
 * topology-annotated block covers the cpus that were online when scx_cid_init()
 * ran and remains valid even after those cpus go offline. The tail block covers
 * possible-but-not-online cpus and carries all-(-1) topo info (see
 * scx_cid_topo); callers detect it via the -1 sentinels.
 *
 * See the comment above the table definitions in cid.c for the
 * memory-ordering and visibility contract.
 */
ರಚನೆ scx_cid_tables {
	u32			nr_shards;
	s16			*cid_ಗಮ್ಯ_cpu;	/* [num_possible_cpus()] */
	s16			*cpu_ಗಮ್ಯ_cid;	/* [nr_cpu_ids] */
	s32			*cid_ಗಮ್ಯ_shard;	/* [num_possible_cpus()] */
	s32			*shard_ಗಂಟು;	/* [num_possible_cpus()] */
	ರಚನೆ scx_cid_shard	*shard_ranges;	/* [num_possible_cpus()] */
	ರಚನೆ scx_cid_topo	*topo;		/* [num_possible_cpus()] */
	ರಚನೆ rcu_ತಲೆ		rcu;
};

ಬಾಹ್ಯ u32 scx_nr_cid_shards;
ಬಾಹ್ಯ s16 __rcu *scx_cid_ಗಮ್ಯ_cpu_tbl;
ಬಾಹ್ಯ s16 __rcu *scx_cpu_ಗಮ್ಯ_cid_tbl;
ಬಾಹ್ಯ s32 __rcu *scx_cid_ಗಮ್ಯ_shard;
ಬಾಹ್ಯ s32 __rcu *scx_shard_ಗಂಟು;
ಬಾಹ್ಯ ರಚನೆ scx_cid_shard __rcu *scx_cid_shard_ranges;
ಬಾಹ್ಯ ರಚನೆ scx_cid_topo __rcu *scx_cid_topo;
ಬಾಹ್ಯ ರಚನೆ btf_id_set8 scx_kfunc_ids_init_cids;
ಬಾಹ್ಯ ರಚನೆ btf_id_set8 scx_kfunc_ids_cid;

ಶೂನ್ಯ scx_cmask_clear(ರಚನೆ scx_cmask *m);
ಶೂನ್ಯ scx_cmask_fill(ರಚನೆ scx_cmask *m);
ಶೂನ್ಯ scx_cmask_and(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src);
ಶೂನ್ಯ scx_cmask_or(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src);
ಶೂನ್ಯ scx_cmask_copy(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src);
ಶೂನ್ಯ scx_cmask_andnot(ರಚನೆ scx_cmask *dst, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src);
bool scx_cmask_subset(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *sub, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *super);
bool scx_cmask_intersects(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *a, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *b);
bool scx_cmask_ಖಾಲಿಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *m);
s32 scx_cid_init(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_cid_publish_tables(ಶೂನ್ಯ);
ಶೂನ್ಯ scx_cid_retire_tables(ಶೂನ್ಯ);
ಪೂರ್ಣಾಂಕ scx_cid_kfunc_init(ಶೂನ್ಯ);

/**
 * cid_valid - Verify a cid value, to be used on ops input args
 * @sch: scx_sched to abort on error
 * @cid: cid which came from a BPF ops
 *
 * Return true if @cid is in [0, num_possible_cpus()). On failure, trigger
 * scx_error() and return false.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cid_valid(ರಚನೆ scx_sched *sch, s32 cid)
{
	ಆದರೆ (likely(cid >= 0 && cid < ಅಂಕೆ_possible_cpus()))
		ಹಿಂತಿರುಗಿ true;
	scx_error(sch, "invalid cid %d", cid);
	ಹಿಂತಿರುಗಿ false;
}

/**
 * __scx_cid_to_cpu - Unchecked cid->cpu table lookup
 * @cid: cid to look up. Must be in [0, num_possible_cpus()).
 *
 * Intended for callsites that have already validated @cid and that run on a
 * live scheduler, which guarantees the tables are published and stable.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ s32 __scx_cid_to_cpu(s32 cid)
{
	ಹಿಂತಿರುಗಿ rcu_dereference_all(scx_cid_ಗಮ್ಯ_cpu_tbl)[cid];
}

/**
 * __scx_cpu_to_cid - Unchecked cpu->cid table lookup
 * @cpu: cpu to look up. Must be a valid possible cpu id.
 *
 * Same usage constraints as __scx_cid_to_cpu().
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ s32 __scx_cpu_to_cid(s32 cpu)
{
	ಹಿಂತಿರುಗಿ rcu_dereference_all(scx_cpu_ಗಮ್ಯ_cid_tbl)[cpu];
}

/**
 * scx_cid_to_cpu - Translate @cid to its cpu
 * @sch: scx_sched for error reporting
 * @cid: cid to look up
 *
 * Return the cpu for @cid or a negative errno on failure. Invalid cid triggers
 * scx_error() on @sch. The mapping is stable while the scheduler is live.
 *
 * Return -EINVAL without triggering scx_error() if no tables have been
 * published yet, which a prog-facing kfunc can observe while racing the root
 * scheduler enable.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ s32 scx_cid_ಗಮ್ಯ_cpu(ರಚನೆ scx_sched *sch, s32 cid)
{
	s16 *tbl = rcu_dereference_all(scx_cid_ಗಮ್ಯ_cpu_tbl);

	ಆದರೆ (!cid_valid(sch, cid) || unlikely(!tbl))
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಹಿಂತಿರುಗಿ tbl[cid];
}

/**
 * scx_cpu_to_cid - Translate @cpu to its cid
 * @sch: scx_sched for error reporting
 * @cpu: cpu to look up
 *
 * Return the cid for @cpu or a negative errno on failure. Invalid cpu triggers
 * scx_error() on @sch. Same usage rules as scx_cid_to_cpu().
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ s32 scx_cpu_ಗಮ್ಯ_cid(ರಚನೆ scx_sched *sch, s32 cpu)
{
	s16 *tbl = rcu_dereference_all(scx_cpu_ಗಮ್ಯ_cid_tbl);

	ಆದರೆ (!scx_cpu_valid(sch, cpu, ಖಾಲಿ) || unlikely(!tbl))
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಹಿಂತಿರುಗಿ tbl[cpu];
}

/**
 * scx_is_cid_type - Test whether the active scheduler hierarchy is cid-form
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_is_cid_type(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_unlikely(&__scx_is_cid_type);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool __scx_cmask_contains(u32 cid, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *m)
{
	ಹಿಂತಿರುಗಿ likely(cid >= m->ತಳ && cid < m->ತಳ + m->nr_cids);
}

/* Word in bits[] covering @cid. @cid must satisfy __scx_cmask_contains(). */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 *__scx_cmask_word(u32 cid, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *m)
{
	ಹಿಂತಿರುಗಿ (u64 *)&m->ಬಿಟ್ಟುಗಳು[cid / 64 - m->ತಳ / 64];
}

/**
 * __scx_cmask_init - Initialize @m with explicit storage capacity
 * @m: cmask to initialize
 * @base: first cid of the active range
 * @nr_cids: number of cids in the active range
 * @alloc_cids: storage capacity in cids, at least @nr_cids
 *
 * Use when storage is sized larger than the initial active range. All of
 * bits[] is zeroed.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __scx_cmask_init(ರಚನೆ scx_cmask *m, u32 ತಳ, u32 nr_cids,
				    u32 alloc_cids)
{
	ಆದರೆ (WARN_ON_ONCE(alloc_cids < nr_cids))
		nr_cids = alloc_cids;

	m->ತಳ = ತಳ;
	m->nr_cids = nr_cids;
	m->alloc_ಪದಗಳು = SCX_CMASK_NR_ಪದಗಳು(alloc_cids);
	memset(m->ಬಿಟ್ಟುಗಳು, 0, m->alloc_ಪದಗಳು * ಗಾತ್ರ(u64));
}

/**
 * scx_cmask_init - Initialize @m on tight storage
 * @m: cmask to initialize
 * @base: first cid of the active range
 * @nr_cids: number of cids in the active range
 *
 * All of bits[] is zeroed.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_cmask_init(ರಚನೆ scx_cmask *m, u32 ತಳ, u32 nr_cids)
{
	__scx_cmask_init(m, ತಳ, nr_cids, nr_cids);
}

/**
 * scx_cmask_reframe - Reshape @m's active range without resizing storage
 * @m: cmask to reframe
 * @base: new active range base
 * @nr_cids: new active range length, must fit within @m->alloc_words
 *
 * Body bits within the new range become garbage - only the head and tail
 * words are zeroed to keep the padding invariant.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_cmask_reframe(ರಚನೆ scx_cmask *m, u32 ತಳ, u32 nr_cids)
{
	ಆದರೆ (WARN_ON_ONCE(SCX_CMASK_NR_ಪದಗಳು(nr_cids) > m->alloc_ಪದಗಳು))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (nr_cids) {
		u32 ಕೊನೆಯ_ಶಬ್ದ = ((ತಳ & 63) + nr_cids - 1) / 64;

		m->ಬಿಟ್ಟುಗಳು[0] = 0;
		m->ಬಿಟ್ಟುಗಳು[ಕೊನೆಯ_ಶಬ್ದ] = 0;
	}

	m->ತಳ = ತಳ;
	m->nr_cids = nr_cids;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __scx_cmask_set(u32 cid, ರಚನೆ scx_cmask *m)
{
	ಆದರೆ (!__scx_cmask_contains(cid, m))
		ಹಿಂತಿರುಗಿ;
	*__scx_cmask_word(cid, m) |= ಬಿಟ್_U64(cid & 63);
}

/**
 * scx_cmask_test - test whether @cid is set in @m
 * @cid: cid to test
 * @m: cmask to test
 *
 * Return %false if @cid is outside @m's active range. Otherwise return the
 * bit's value. Read via READ_ONCE so callers can race set/clear writers.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_cmask_test(u32 cid, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *m)
{
	ಆದರೆ (!__scx_cmask_contains(cid, m))
		ಹಿಂತಿರುಗಿ false;
	ಹಿಂತಿರುಗಿ READ_ONCE(*__scx_cmask_word(cid, m)) & ಬಿಟ್_U64(cid & 63);
}

/*
 * Words of bits[] the active range spans, 0 if empty. Tighter than the storage
 * SCX_CMASK_NR_WORDS() sizes for the worst-case base alignment.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u32 scx_cmask_nr_ಬಳಸಿದ_ಪದಗಳು(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *m)
{
	ಆದರೆ (!m->nr_cids)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ ((m->ತಳ & 63) + m->nr_cids - 1) / 64 + 1;
}

/**
 * scx_cmask_for_each_cid - iterate set cids in @m
 * @cid: s32 loop var that receives each set cid in turn
 * @m: cmask to iterate
 *
 * Visits set bits within @m's active range in ascending order. Scans only the
 * words the active range spans, where head and tail padding is kept zero, so
 * no per-cid range check is needed.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು scx_cmask_for_each_cid(cid, m)						\
	ಗಾಗಿ (u64 __bs = (m)->ತಳ & ~63u, __wi = 0,				\
		     __nw = scx_cmask_nr_ಬಳಸಿದ_ಪದಗಳು(m);				\
	     __wi < __nw; __wi++)						\
		ಗಾಗಿ (u64 __w = READ_ONCE((m)->ಬಿಟ್ಟುಗಳು[__wi]);			\
		     __w && ((cid) = __bs + __wi * 64 + __ffs64(__w), true);	\
		     __w &= __w - 1)

/*
 * scx_cpu_arg() wraps a cpu arg being handed to an SCX op. For cid-form
 * schedulers it resolves to the matching cid; for cpu-form it passes @cpu
 * through. scx_cpu_ret() is the inverse for a cpu/cid returned from an op
 * (currently only ops.select_cpu); it validates the BPF-supplied cid and
 * triggers scx_error() on @sch if invalid.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ s32 scx_cpu_arg(s32 cpu)
{
	ಆದರೆ (scx_is_cid_type())
		ಹಿಂತಿರುಗಿ __scx_cpu_to_cid(cpu);
	ಹಿಂತಿರುಗಿ cpu;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ s32 scx_cpu_ret(ರಚನೆ scx_sched *sch, s32 cpu_or_cid)
{
	ಆದರೆ (cpu_or_cid < 0 || !scx_is_cid_type())
		ಹಿಂತಿರುಗಿ cpu_or_cid;
	ಹಿಂತಿರುಗಿ scx_cid_ಗಮ್ಯ_cpu(sch, cpu_or_cid);
}

ಪೂರ್ಣಾಂಕ scx_cmask_ref_init(ರಚನೆ scx_sched *sch, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src,
		       ರಚನೆ scx_cmask_ref *ref);
ಶೂನ್ಯ scx_cmask_ref_init_kern(ರಚನೆ scx_sched *sch, ರಚನೆ scx_cmask *m,
			     u32 ತಳ, u32 nr_cids, ರಚನೆ scx_cmask_ref *ref);
ಶೂನ್ಯ scx_cmask_ref_shard(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref, s32 shard_idx,
			 ರಚನೆ scx_cmask *ಹೊರಹೋಗು);
ಶೂನ್ಯ scx_cmask_ref_from_cpumask(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref,
				ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpumask);
ಶೂನ್ಯ scx_cmask_ref_or(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src);
ಶೂನ್ಯ scx_cmask_ref_copy(ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask_ref *ref, ಸ್ಥಿರಾಂಕ ರಚನೆ scx_cmask *src);

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_EXT_CID_H */
