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
#ifndef _KERNEL_SCHED_EXT_CID_H
#define _KERNEL_SCHED_EXT_CID_H

#অন্তর্ভাব "internal.h"

কাঠামো scx_sched;

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
কাঠামো scx_cid_tables {
	u32			nr_shards;
	s16			*cid_to_cpu;	/* [num_possible_cpus()] */
	s16			*cpu_to_cid;	/* [nr_cpu_ids] */
	s32			*cid_to_shard;	/* [num_possible_cpus()] */
	s32			*shard_গ্রন্থি;	/* [num_possible_cpus()] */
	কাঠামো scx_cid_shard	*shard_ranges;	/* [num_possible_cpus()] */
	কাঠামো scx_cid_topo	*topo;		/* [num_possible_cpus()] */
	কাঠামো rcu_শীর্ষ		rcu;
};

বাহ্যিক u32 scx_nr_cid_shards;
বাহ্যিক s16 __rcu *scx_cid_to_cpu_tbl;
বাহ্যিক s16 __rcu *scx_cpu_to_cid_tbl;
বাহ্যিক s32 __rcu *scx_cid_to_shard;
বাহ্যিক s32 __rcu *scx_shard_গ্রন্থি;
বাহ্যিক কাঠামো scx_cid_shard __rcu *scx_cid_shard_ranges;
বাহ্যিক কাঠামো scx_cid_topo __rcu *scx_cid_topo;
বাহ্যিক কাঠামো btf_id_set8 scx_kfunc_ids_init_cids;
বাহ্যিক কাঠামো btf_id_set8 scx_kfunc_ids_cid;

শূন্য scx_cmask_clear(কাঠামো scx_cmask *m);
শূন্য scx_cmask_fill(কাঠামো scx_cmask *m);
শূন্য scx_cmask_and(কাঠামো scx_cmask *dst, ধ্রুবক কাঠামো scx_cmask *src);
শূন্য scx_cmask_or(কাঠামো scx_cmask *dst, ধ্রুবক কাঠামো scx_cmask *src);
শূন্য scx_cmask_copy(কাঠামো scx_cmask *dst, ধ্রুবক কাঠামো scx_cmask *src);
শূন্য scx_cmask_andnot(কাঠামো scx_cmask *dst, ধ্রুবক কাঠামো scx_cmask *src);
bool scx_cmask_subset(ধ্রুবক কাঠামো scx_cmask *sub, ধ্রুবক কাঠামো scx_cmask *super);
bool scx_cmask_intersects(ধ্রুবক কাঠামো scx_cmask *a, ধ্রুবক কাঠামো scx_cmask *b);
bool scx_cmask_empty(ধ্রুবক কাঠামো scx_cmask *m);
s32 scx_cid_init(কাঠামো scx_sched *sch);
শূন্য scx_cid_publish_tables(শূন্য);
শূন্য scx_cid_retire_tables(শূন্য);
পূর্ণসংখ্যা scx_cid_kfunc_init(শূন্য);

/**
 * cid_valid - Verify a cid value, to be used on ops input args
 * @sch: scx_sched to abort on error
 * @cid: cid which came from a BPF ops
 *
 * Return true if @cid is in [0, num_possible_cpus()). On failure, trigger
 * scx_error() and return false.
 */
স্থির অন্তঃরেখ bool cid_valid(কাঠামো scx_sched *sch, s32 cid)
{
	যদি (likely(cid >= 0 && cid < অঙ্ক_possible_cpus()))
		ফেরত true;
	scx_error(sch, "invalid cid %d", cid);
	ফেরত false;
}

/**
 * __scx_cid_to_cpu - Unchecked cid->cpu table lookup
 * @cid: cid to look up. Must be in [0, num_possible_cpus()).
 *
 * Intended for callsites that have already validated @cid and that run on a
 * live scheduler, which guarantees the tables are published and stable.
 */
স্থির অন্তঃরেখ s32 __scx_cid_to_cpu(s32 cid)
{
	ফেরত rcu_dereference_all(scx_cid_to_cpu_tbl)[cid];
}

/**
 * __scx_cpu_to_cid - Unchecked cpu->cid table lookup
 * @cpu: cpu to look up. Must be a valid possible cpu id.
 *
 * Same usage constraints as __scx_cid_to_cpu().
 */
স্থির অন্তঃরেখ s32 __scx_cpu_to_cid(s32 cpu)
{
	ফেরত rcu_dereference_all(scx_cpu_to_cid_tbl)[cpu];
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
স্থির অন্তঃরেখ s32 scx_cid_to_cpu(কাঠামো scx_sched *sch, s32 cid)
{
	s16 *tbl = rcu_dereference_all(scx_cid_to_cpu_tbl);

	যদি (!cid_valid(sch, cid) || unlikely(!tbl))
		ফেরত -EINVAL;
	ফেরত tbl[cid];
}

/**
 * scx_cpu_to_cid - Translate @cpu to its cid
 * @sch: scx_sched for error reporting
 * @cpu: cpu to look up
 *
 * Return the cid for @cpu or a negative errno on failure. Invalid cpu triggers
 * scx_error() on @sch. Same usage rules as scx_cid_to_cpu().
 */
স্থির অন্তঃরেখ s32 scx_cpu_to_cid(কাঠামো scx_sched *sch, s32 cpu)
{
	s16 *tbl = rcu_dereference_all(scx_cpu_to_cid_tbl);

	যদি (!scx_cpu_valid(sch, cpu, ফাঁকা) || unlikely(!tbl))
		ফেরত -EINVAL;
	ফেরত tbl[cpu];
}

/**
 * scx_is_cid_type - Test whether the active scheduler hierarchy is cid-form
 */
স্থির অন্তঃরেখ bool scx_is_cid_type(শূন্য)
{
	ফেরত static_branch_unlikely(&__scx_is_cid_type);
}

স্থির অন্তঃরেখ bool __scx_cmask_contains(u32 cid, ধ্রুবক কাঠামো scx_cmask *m)
{
	ফেরত likely(cid >= m->base && cid < m->base + m->nr_cids);
}

/* Word in bits[] covering @cid. @cid must satisfy __scx_cmask_contains(). */
স্থির অন্তঃরেখ u64 *__scx_cmask_word(u32 cid, ধ্রুবক কাঠামো scx_cmask *m)
{
	ফেরত (u64 *)&m->bits[cid / 64 - m->base / 64];
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
স্থির অন্তঃরেখ শূন্য __scx_cmask_init(কাঠামো scx_cmask *m, u32 base, u32 nr_cids,
				    u32 alloc_cids)
{
	যদি (WARN_ON_ONCE(alloc_cids < nr_cids))
		nr_cids = alloc_cids;

	m->base = base;
	m->nr_cids = nr_cids;
	m->alloc_শব্দগুলি = SCX_CMASK_NR_শব্দগুলি(alloc_cids);
	memset(m->bits, 0, m->alloc_শব্দগুলি * আকার(u64));
}

/**
 * scx_cmask_init - Initialize @m on tight storage
 * @m: cmask to initialize
 * @base: first cid of the active range
 * @nr_cids: number of cids in the active range
 *
 * All of bits[] is zeroed.
 */
স্থির অন্তঃরেখ শূন্য scx_cmask_init(কাঠামো scx_cmask *m, u32 base, u32 nr_cids)
{
	__scx_cmask_init(m, base, nr_cids, nr_cids);
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
স্থির অন্তঃরেখ শূন্য scx_cmask_reframe(কাঠামো scx_cmask *m, u32 base, u32 nr_cids)
{
	যদি (WARN_ON_ONCE(SCX_CMASK_NR_শব্দগুলি(nr_cids) > m->alloc_শব্দগুলি))
		ফেরত;

	যদি (nr_cids) {
		u32 last_word = ((base & 63) + nr_cids - 1) / 64;

		m->bits[0] = 0;
		m->bits[last_word] = 0;
	}

	m->base = base;
	m->nr_cids = nr_cids;
}

স্থির অন্তঃরেখ শূন্য __scx_cmask_set(u32 cid, কাঠামো scx_cmask *m)
{
	যদি (!__scx_cmask_contains(cid, m))
		ফেরত;
	*__scx_cmask_word(cid, m) |= BIT_U64(cid & 63);
}

/**
 * scx_cmask_test - test whether @cid is set in @m
 * @cid: cid to test
 * @m: cmask to test
 *
 * Return %false if @cid is outside @m's active range. Otherwise return the
 * bit's value. Read via READ_ONCE so callers can race set/clear writers.
 */
স্থির অন্তঃরেখ bool scx_cmask_test(u32 cid, ধ্রুবক কাঠামো scx_cmask *m)
{
	যদি (!__scx_cmask_contains(cid, m))
		ফেরত false;
	ফেরত READ_ONCE(*__scx_cmask_word(cid, m)) & BIT_U64(cid & 63);
}

/*
 * Words of bits[] the active range spans, 0 if empty. Tighter than the storage
 * SCX_CMASK_NR_WORDS() sizes for the worst-case base alignment.
 */
স্থির অন্তঃরেখ u32 scx_cmask_nr_used_শব্দগুলি(ধ্রুবক কাঠামো scx_cmask *m)
{
	যদি (!m->nr_cids)
		ফেরত 0;
	ফেরত ((m->base & 63) + m->nr_cids - 1) / 64 + 1;
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
#define scx_cmask_for_each_cid(cid, m)						\
	জন্য (u64 __bs = (m)->base & ~63u, __wi = 0,				\
		     __nw = scx_cmask_nr_used_শব্দগুলি(m);				\
	     __wi < __nw; __wi++)						\
		জন্য (u64 __w = READ_ONCE((m)->bits[__wi]);			\
		     __w && ((cid) = __bs + __wi * 64 + __ffs64(__w), true);	\
		     __w &= __w - 1)

/*
 * scx_cpu_arg() wraps a cpu arg being handed to an SCX op. For cid-form
 * schedulers it resolves to the matching cid; for cpu-form it passes @cpu
 * through. scx_cpu_ret() is the inverse for a cpu/cid returned from an op
 * (currently only ops.select_cpu); it validates the BPF-supplied cid and
 * triggers scx_error() on @sch if invalid.
 */
স্থির অন্তঃরেখ s32 scx_cpu_arg(s32 cpu)
{
	যদি (scx_is_cid_type())
		ফেরত __scx_cpu_to_cid(cpu);
	ফেরত cpu;
}

স্থির অন্তঃরেখ s32 scx_cpu_ret(কাঠামো scx_sched *sch, s32 cpu_or_cid)
{
	যদি (cpu_or_cid < 0 || !scx_is_cid_type())
		ফেরত cpu_or_cid;
	ফেরত scx_cid_to_cpu(sch, cpu_or_cid);
}

পূর্ণসংখ্যা scx_cmask_ref_init(কাঠামো scx_sched *sch, ধ্রুবক কাঠামো scx_cmask *src,
		       কাঠামো scx_cmask_ref *ref);
শূন্য scx_cmask_ref_init_kern(কাঠামো scx_sched *sch, কাঠামো scx_cmask *m,
			     u32 base, u32 nr_cids, কাঠামো scx_cmask_ref *ref);
শূন্য scx_cmask_ref_shard(ধ্রুবক কাঠামো scx_cmask_ref *ref, s32 shard_idx,
			 কাঠামো scx_cmask *out);
শূন্য scx_cmask_ref_from_cpumask(ধ্রুবক কাঠামো scx_cmask_ref *ref,
				ধ্রুবক কাঠামো cpumask *cpumask);
শূন্য scx_cmask_ref_or(ধ্রুবক কাঠামো scx_cmask_ref *ref, ধ্রুবক কাঠামো scx_cmask *src);
শূন্য scx_cmask_ref_copy(ধ্রুবক কাঠামো scx_cmask_ref *ref, ধ্রুবক কাঠামো scx_cmask *src);

#endif /* _KERNEL_SCHED_EXT_CID_H */
