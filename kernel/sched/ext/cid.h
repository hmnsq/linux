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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_EXT_CID_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_EXT_CID_H

#ਅਨ੍ਤਰ੍ਭਾਵ "internal.h"

ਬਣਤਰ scx_sched;

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
ਬਣਤਰ scx_cid_tables {
	u32			nr_shards;
	s16			*cid_ਮੰਜ਼ਿਲ_cpu;	/* [num_possible_cpus()] */
	s16			*cpu_ਮੰਜ਼ਿਲ_cid;	/* [nr_cpu_ids] */
	s32			*cid_ਮੰਜ਼ਿਲ_shard;	/* [num_possible_cpus()] */
	s32			*shard_ਗੰਢ;	/* [num_possible_cpus()] */
	ਬਣਤਰ scx_cid_shard	*shard_ranges;	/* [num_possible_cpus()] */
	ਬਣਤਰ scx_cid_topo	*topo;		/* [num_possible_cpus()] */
	ਬਣਤਰ rcu_ਸਿਰ		rcu;
};

ਬਾਹਰੀ u32 scx_nr_cid_shards;
ਬਾਹਰੀ s16 __rcu *scx_cid_ਮੰਜ਼ਿਲ_cpu_tbl;
ਬਾਹਰੀ s16 __rcu *scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl;
ਬਾਹਰੀ s32 __rcu *scx_cid_ਮੰਜ਼ਿਲ_shard;
ਬਾਹਰੀ s32 __rcu *scx_shard_ਗੰਢ;
ਬਾਹਰੀ ਬਣਤਰ scx_cid_shard __rcu *scx_cid_shard_ranges;
ਬਾਹਰੀ ਬਣਤਰ scx_cid_topo __rcu *scx_cid_topo;
ਬਾਹਰੀ ਬਣਤਰ btf_id_set8 scx_kfunc_ids_init_cids;
ਬਾਹਰੀ ਬਣਤਰ btf_id_set8 scx_kfunc_ids_cid;

ਖਾਲੀ scx_cmask_clear(ਬਣਤਰ scx_cmask *m);
ਖਾਲੀ scx_cmask_fill(ਬਣਤਰ scx_cmask *m);
ਖਾਲੀ scx_cmask_and(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src);
ਖਾਲੀ scx_cmask_or(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src);
ਖਾਲੀ scx_cmask_copy(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src);
ਖਾਲੀ scx_cmask_andnot(ਬਣਤਰ scx_cmask *dst, ਅਚਲ ਬਣਤਰ scx_cmask *src);
bool scx_cmask_subset(ਅਚਲ ਬਣਤਰ scx_cmask *sub, ਅਚਲ ਬਣਤਰ scx_cmask *super);
bool scx_cmask_intersects(ਅਚਲ ਬਣਤਰ scx_cmask *a, ਅਚਲ ਬਣਤਰ scx_cmask *b);
bool scx_cmask_ਖਾਲੀ_ਥਾਵਾਂ(ਅਚਲ ਬਣਤਰ scx_cmask *m);
s32 scx_cid_init(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_cid_publish_tables(ਖਾਲੀ);
ਖਾਲੀ scx_cid_retire_tables(ਖਾਲੀ);
ਪੂਰਨ scx_cid_kfunc_init(ਖਾਲੀ);

/**
 * cid_valid - Verify a cid value, to be used on ops input args
 * @sch: scx_sched to abort on error
 * @cid: cid which came from a BPF ops
 *
 * Return true if @cid is in [0, num_possible_cpus()). On failure, trigger
 * scx_error() and return false.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool cid_valid(ਬਣਤਰ scx_sched *sch, s32 cid)
{
	ਜੇ (likely(cid >= 0 && cid < ਅੰਕ_possible_cpus()))
		ਵਾਪਸ true;
	scx_error(sch, "invalid cid %d", cid);
	ਵਾਪਸ false;
}

/**
 * __scx_cid_to_cpu - Unchecked cid->cpu table lookup
 * @cid: cid to look up. Must be in [0, num_possible_cpus()).
 *
 * Intended for callsites that have already validated @cid and that run on a
 * live scheduler, which guarantees the tables are published and stable.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ s32 __scx_cid_to_cpu(s32 cid)
{
	ਵਾਪਸ rcu_dereference_all(scx_cid_ਮੰਜ਼ਿਲ_cpu_tbl)[cid];
}

/**
 * __scx_cpu_to_cid - Unchecked cpu->cid table lookup
 * @cpu: cpu to look up. Must be a valid possible cpu id.
 *
 * Same usage constraints as __scx_cid_to_cpu().
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ s32 __scx_cpu_to_cid(s32 cpu)
{
	ਵਾਪਸ rcu_dereference_all(scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl)[cpu];
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
ਸਥਿਰ ਸਤਰਅੰਦਰ s32 scx_cid_ਮੰਜ਼ਿਲ_cpu(ਬਣਤਰ scx_sched *sch, s32 cid)
{
	s16 *tbl = rcu_dereference_all(scx_cid_ਮੰਜ਼ਿਲ_cpu_tbl);

	ਜੇ (!cid_valid(sch, cid) || unlikely(!tbl))
		ਵਾਪਸ -EINVAL;
	ਵਾਪਸ tbl[cid];
}

/**
 * scx_cpu_to_cid - Translate @cpu to its cid
 * @sch: scx_sched for error reporting
 * @cpu: cpu to look up
 *
 * Return the cid for @cpu or a negative errno on failure. Invalid cpu triggers
 * scx_error() on @sch. Same usage rules as scx_cid_to_cpu().
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ s32 scx_cpu_ਮੰਜ਼ਿਲ_cid(ਬਣਤਰ scx_sched *sch, s32 cpu)
{
	s16 *tbl = rcu_dereference_all(scx_cpu_ਮੰਜ਼ਿਲ_cid_tbl);

	ਜੇ (!scx_cpu_valid(sch, cpu, ਨਿਰਾ) || unlikely(!tbl))
		ਵਾਪਸ -EINVAL;
	ਵਾਪਸ tbl[cpu];
}

/**
 * scx_is_cid_type - Test whether the active scheduler hierarchy is cid-form
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_is_cid_type(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_unlikely(&__scx_is_cid_type);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool __scx_cmask_contains(u32 cid, ਅਚਲ ਬਣਤਰ scx_cmask *m)
{
	ਵਾਪਸ likely(cid >= m->ਅਧਾਰ && cid < m->ਅਧਾਰ + m->nr_cids);
}

/* Word in bits[] covering @cid. @cid must satisfy __scx_cmask_contains(). */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 *__scx_cmask_word(u32 cid, ਅਚਲ ਬਣਤਰ scx_cmask *m)
{
	ਵਾਪਸ (u64 *)&m->ਬਿੱਟ[cid / 64 - m->ਅਧਾਰ / 64];
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __scx_cmask_init(ਬਣਤਰ scx_cmask *m, u32 ਅਧਾਰ, u32 nr_cids,
				    u32 alloc_cids)
{
	ਜੇ (WARN_ON_ONCE(alloc_cids < nr_cids))
		nr_cids = alloc_cids;

	m->ਅਧਾਰ = ਅਧਾਰ;
	m->nr_cids = nr_cids;
	m->alloc_ਸ਼ਬਦ = SCX_CMASK_NR_ਸ਼ਬਦ(alloc_cids);
	memset(m->ਬਿੱਟ, 0, m->alloc_ਸ਼ਬਦ * ਆਕਾਰ(u64));
}

/**
 * scx_cmask_init - Initialize @m on tight storage
 * @m: cmask to initialize
 * @base: first cid of the active range
 * @nr_cids: number of cids in the active range
 *
 * All of bits[] is zeroed.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_cmask_init(ਬਣਤਰ scx_cmask *m, u32 ਅਧਾਰ, u32 nr_cids)
{
	__scx_cmask_init(m, ਅਧਾਰ, nr_cids, nr_cids);
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_cmask_reframe(ਬਣਤਰ scx_cmask *m, u32 ਅਧਾਰ, u32 nr_cids)
{
	ਜੇ (WARN_ON_ONCE(SCX_CMASK_NR_ਸ਼ਬਦ(nr_cids) > m->alloc_ਸ਼ਬਦ))
		ਵਾਪਸ;

	ਜੇ (nr_cids) {
		u32 ਆਖਰੀ_ਸ਼ਬਦ = ((ਅਧਾਰ & 63) + nr_cids - 1) / 64;

		m->ਬਿੱਟ[0] = 0;
		m->ਬਿੱਟ[ਆਖਰੀ_ਸ਼ਬਦ] = 0;
	}

	m->ਅਧਾਰ = ਅਧਾਰ;
	m->nr_cids = nr_cids;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __scx_cmask_set(u32 cid, ਬਣਤਰ scx_cmask *m)
{
	ਜੇ (!__scx_cmask_contains(cid, m))
		ਵਾਪਸ;
	*__scx_cmask_word(cid, m) |= ਦੋਆਧਾਰੀ_ਅੰਕ_U64(cid & 63);
}

/**
 * scx_cmask_test - test whether @cid is set in @m
 * @cid: cid to test
 * @m: cmask to test
 *
 * Return %false if @cid is outside @m's active range. Otherwise return the
 * bit's value. Read via READ_ONCE so callers can race set/clear writers.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_cmask_test(u32 cid, ਅਚਲ ਬਣਤਰ scx_cmask *m)
{
	ਜੇ (!__scx_cmask_contains(cid, m))
		ਵਾਪਸ false;
	ਵਾਪਸ READ_ONCE(*__scx_cmask_word(cid, m)) & ਦੋਆਧਾਰੀ_ਅੰਕ_U64(cid & 63);
}

/*
 * Words of bits[] the active range spans, 0 if empty. Tighter than the storage
 * SCX_CMASK_NR_WORDS() sizes for the worst-case base alignment.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ u32 scx_cmask_nr_ਵਰਤਿਆ_ਸ਼ਬਦ(ਅਚਲ ਬਣਤਰ scx_cmask *m)
{
	ਜੇ (!m->nr_cids)
		ਵਾਪਸ 0;
	ਵਾਪਸ ((m->ਅਧਾਰ & 63) + m->nr_cids - 1) / 64 + 1;
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_cmask_for_each_cid(cid, m)						\
	ਲਈ (u64 __bs = (m)->ਅਧਾਰ & ~63u, __wi = 0,				\
		     __nw = scx_cmask_nr_ਵਰਤਿਆ_ਸ਼ਬਦ(m);				\
	     __wi < __nw; __wi++)						\
		ਲਈ (u64 __w = READ_ONCE((m)->ਬਿੱਟ[__wi]);			\
		     __w && ((cid) = __bs + __wi * 64 + __ffs64(__w), true);	\
		     __w &= __w - 1)

/*
 * scx_cpu_arg() wraps a cpu arg being handed to an SCX op. For cid-form
 * schedulers it resolves to the matching cid; for cpu-form it passes @cpu
 * through. scx_cpu_ret() is the inverse for a cpu/cid returned from an op
 * (currently only ops.select_cpu); it validates the BPF-supplied cid and
 * triggers scx_error() on @sch if invalid.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ s32 scx_cpu_arg(s32 cpu)
{
	ਜੇ (scx_is_cid_type())
		ਵਾਪਸ __scx_cpu_to_cid(cpu);
	ਵਾਪਸ cpu;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ s32 scx_cpu_ret(ਬਣਤਰ scx_sched *sch, s32 cpu_or_cid)
{
	ਜੇ (cpu_or_cid < 0 || !scx_is_cid_type())
		ਵਾਪਸ cpu_or_cid;
	ਵਾਪਸ scx_cid_ਮੰਜ਼ਿਲ_cpu(sch, cpu_or_cid);
}

ਪੂਰਨ scx_cmask_ref_init(ਬਣਤਰ scx_sched *sch, ਅਚਲ ਬਣਤਰ scx_cmask *src,
		       ਬਣਤਰ scx_cmask_ref *ref);
ਖਾਲੀ scx_cmask_ref_init_kern(ਬਣਤਰ scx_sched *sch, ਬਣਤਰ scx_cmask *m,
			     u32 ਅਧਾਰ, u32 nr_cids, ਬਣਤਰ scx_cmask_ref *ref);
ਖਾਲੀ scx_cmask_ref_shard(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref, s32 shard_idx,
			 ਬਣਤਰ scx_cmask *ਬਾਹਰਲਾ);
ਖਾਲੀ scx_cmask_ref_from_cpumask(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref,
				ਅਚਲ ਬਣਤਰ cpumask *cpumask);
ਖਾਲੀ scx_cmask_ref_or(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref, ਅਚਲ ਬਣਤਰ scx_cmask *src);
ਖਾਲੀ scx_cmask_ref_copy(ਅਚਲ ਬਣਤਰ scx_cmask_ref *ref, ਅਚਲ ਬਣਤਰ scx_cmask *src);

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_EXT_CID_H */
