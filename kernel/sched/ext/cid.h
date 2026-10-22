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
#నిర్వచితం_కాకపోతే _KERNEL_SCHED_EXT_CID_H
#నిర్వచించు _KERNEL_SCHED_EXT_CID_H

#అన్తర్భావ "internal.h"

నిర్మాణం scx_sched;

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
నిర్మాణం scx_cid_tables {
	u32			nr_shards;
	s16			*cid_గమ్యం_cpu;	/* [num_possible_cpus()] */
	s16			*cpu_గమ్యం_cid;	/* [nr_cpu_ids] */
	s32			*cid_గమ్యం_shard;	/* [num_possible_cpus()] */
	s32			*shard_కణుపు;	/* [num_possible_cpus()] */
	నిర్మాణం scx_cid_shard	*shard_ranges;	/* [num_possible_cpus()] */
	నిర్మాణం scx_cid_topo	*topo;		/* [num_possible_cpus()] */
	నిర్మాణం rcu_తల		rcu;
};

బాహ్య u32 scx_nr_cid_shards;
బాహ్య s16 __rcu *scx_cid_గమ్యం_cpu_tbl;
బాహ్య s16 __rcu *scx_cpu_గమ్యం_cid_tbl;
బాహ్య s32 __rcu *scx_cid_గమ్యం_shard;
బాహ్య s32 __rcu *scx_shard_కణుపు;
బాహ్య నిర్మాణం scx_cid_shard __rcu *scx_cid_shard_ranges;
బాహ్య నిర్మాణం scx_cid_topo __rcu *scx_cid_topo;
బాహ్య నిర్మాణం btf_id_set8 scx_kfunc_ids_init_cids;
బాహ్య నిర్మాణం btf_id_set8 scx_kfunc_ids_cid;

శూన్య scx_cmask_clear(నిర్మాణం scx_cmask *m);
శూన్య scx_cmask_fill(నిర్మాణం scx_cmask *m);
శూన్య scx_cmask_and(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src);
శూన్య scx_cmask_or(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src);
శూన్య scx_cmask_copy(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src);
శూన్య scx_cmask_andnot(నిర్మాణం scx_cmask *dst, స్థిరరాశి నిర్మాణం scx_cmask *src);
bool scx_cmask_subset(స్థిరరాశి నిర్మాణం scx_cmask *sub, స్థిరరాశి నిర్మాణం scx_cmask *super);
bool scx_cmask_intersects(స్థిరరాశి నిర్మాణం scx_cmask *a, స్థిరరాశి నిర్మాణం scx_cmask *b);
bool scx_cmask_ఖాళీలు(స్థిరరాశి నిర్మాణం scx_cmask *m);
s32 scx_cid_init(నిర్మాణం scx_sched *sch);
శూన్య scx_cid_publish_tables(శూన్య);
శూన్య scx_cid_retire_tables(శూన్య);
పూర్ణాంకం scx_cid_kfunc_init(శూన్య);

/**
 * cid_valid - Verify a cid value, to be used on ops input args
 * @sch: scx_sched to abort on error
 * @cid: cid which came from a BPF ops
 *
 * Return true if @cid is in [0, num_possible_cpus()). On failure, trigger
 * scx_error() and return false.
 */
స్థిర అంతఃవరుస bool cid_valid(నిర్మాణం scx_sched *sch, s32 cid)
{
	ఒకవేళ (likely(cid >= 0 && cid < అంకె_possible_cpus()))
		తిరిగి true;
	scx_error(sch, "invalid cid %d", cid);
	తిరిగి false;
}

/**
 * __scx_cid_to_cpu - Unchecked cid->cpu table lookup
 * @cid: cid to look up. Must be in [0, num_possible_cpus()).
 *
 * Intended for callsites that have already validated @cid and that run on a
 * live scheduler, which guarantees the tables are published and stable.
 */
స్థిర అంతఃవరుస s32 __scx_cid_to_cpu(s32 cid)
{
	తిరిగి rcu_dereference_all(scx_cid_గమ్యం_cpu_tbl)[cid];
}

/**
 * __scx_cpu_to_cid - Unchecked cpu->cid table lookup
 * @cpu: cpu to look up. Must be a valid possible cpu id.
 *
 * Same usage constraints as __scx_cid_to_cpu().
 */
స్థిర అంతఃవరుస s32 __scx_cpu_to_cid(s32 cpu)
{
	తిరిగి rcu_dereference_all(scx_cpu_గమ్యం_cid_tbl)[cpu];
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
స్థిర అంతఃవరుస s32 scx_cid_గమ్యం_cpu(నిర్మాణం scx_sched *sch, s32 cid)
{
	s16 *tbl = rcu_dereference_all(scx_cid_గమ్యం_cpu_tbl);

	ఒకవేళ (!cid_valid(sch, cid) || unlikely(!tbl))
		తిరిగి -EINVAL;
	తిరిగి tbl[cid];
}

/**
 * scx_cpu_to_cid - Translate @cpu to its cid
 * @sch: scx_sched for error reporting
 * @cpu: cpu to look up
 *
 * Return the cid for @cpu or a negative errno on failure. Invalid cpu triggers
 * scx_error() on @sch. Same usage rules as scx_cid_to_cpu().
 */
స్థిర అంతఃవరుస s32 scx_cpu_గమ్యం_cid(నిర్మాణం scx_sched *sch, s32 cpu)
{
	s16 *tbl = rcu_dereference_all(scx_cpu_గమ్యం_cid_tbl);

	ఒకవేళ (!scx_cpu_valid(sch, cpu, ఖాళీ) || unlikely(!tbl))
		తిరిగి -EINVAL;
	తిరిగి tbl[cpu];
}

/**
 * scx_is_cid_type - Test whether the active scheduler hierarchy is cid-form
 */
స్థిర అంతఃవరుస bool scx_is_cid_type(శూన్య)
{
	తిరిగి static_branch_unlikely(&__scx_is_cid_type);
}

స్థిర అంతఃవరుస bool __scx_cmask_contains(u32 cid, స్థిరరాశి నిర్మాణం scx_cmask *m)
{
	తిరిగి likely(cid >= m->పునాది && cid < m->పునాది + m->nr_cids);
}

/* Word in bits[] covering @cid. @cid must satisfy __scx_cmask_contains(). */
స్థిర అంతఃవరుస u64 *__scx_cmask_word(u32 cid, స్థిరరాశి నిర్మాణం scx_cmask *m)
{
	తిరిగి (u64 *)&m->బిట్లు[cid / 64 - m->పునాది / 64];
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
స్థిర అంతఃవరుస శూన్య __scx_cmask_init(నిర్మాణం scx_cmask *m, u32 పునాది, u32 nr_cids,
				    u32 alloc_cids)
{
	ఒకవేళ (WARN_ON_ONCE(alloc_cids < nr_cids))
		nr_cids = alloc_cids;

	m->పునాది = పునాది;
	m->nr_cids = nr_cids;
	m->alloc_పదాలు = SCX_CMASK_NR_పదాలు(alloc_cids);
	memset(m->బిట్లు, 0, m->alloc_పదాలు * పరిమాణం(u64));
}

/**
 * scx_cmask_init - Initialize @m on tight storage
 * @m: cmask to initialize
 * @base: first cid of the active range
 * @nr_cids: number of cids in the active range
 *
 * All of bits[] is zeroed.
 */
స్థిర అంతఃవరుస శూన్య scx_cmask_init(నిర్మాణం scx_cmask *m, u32 పునాది, u32 nr_cids)
{
	__scx_cmask_init(m, పునాది, nr_cids, nr_cids);
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
స్థిర అంతఃవరుస శూన్య scx_cmask_reframe(నిర్మాణం scx_cmask *m, u32 పునాది, u32 nr_cids)
{
	ఒకవేళ (WARN_ON_ONCE(SCX_CMASK_NR_పదాలు(nr_cids) > m->alloc_పదాలు))
		తిరిగి;

	ఒకవేళ (nr_cids) {
		u32 చివరి_మాట = ((పునాది & 63) + nr_cids - 1) / 64;

		m->బిట్లు[0] = 0;
		m->బిట్లు[చివరి_మాట] = 0;
	}

	m->పునాది = పునాది;
	m->nr_cids = nr_cids;
}

స్థిర అంతఃవరుస శూన్య __scx_cmask_set(u32 cid, నిర్మాణం scx_cmask *m)
{
	ఒకవేళ (!__scx_cmask_contains(cid, m))
		తిరిగి;
	*__scx_cmask_word(cid, m) |= బిట్టు_U64(cid & 63);
}

/**
 * scx_cmask_test - test whether @cid is set in @m
 * @cid: cid to test
 * @m: cmask to test
 *
 * Return %false if @cid is outside @m's active range. Otherwise return the
 * bit's value. Read via READ_ONCE so callers can race set/clear writers.
 */
స్థిర అంతఃవరుస bool scx_cmask_test(u32 cid, స్థిరరాశి నిర్మాణం scx_cmask *m)
{
	ఒకవేళ (!__scx_cmask_contains(cid, m))
		తిరిగి false;
	తిరిగి READ_ONCE(*__scx_cmask_word(cid, m)) & బిట్టు_U64(cid & 63);
}

/*
 * Words of bits[] the active range spans, 0 if empty. Tighter than the storage
 * SCX_CMASK_NR_WORDS() sizes for the worst-case base alignment.
 */
స్థిర అంతఃవరుస u32 scx_cmask_nr_వాడిన_పదాలు(స్థిరరాశి నిర్మాణం scx_cmask *m)
{
	ఒకవేళ (!m->nr_cids)
		తిరిగి 0;
	తిరిగి ((m->పునాది & 63) + m->nr_cids - 1) / 64 + 1;
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
#నిర్వచించు scx_cmask_for_each_cid(cid, m)						\
	కొరకు (u64 __bs = (m)->పునాది & ~63u, __wi = 0,				\
		     __nw = scx_cmask_nr_వాడిన_పదాలు(m);				\
	     __wi < __nw; __wi++)						\
		కొరకు (u64 __w = READ_ONCE((m)->బిట్లు[__wi]);			\
		     __w && ((cid) = __bs + __wi * 64 + __ffs64(__w), true);	\
		     __w &= __w - 1)

/*
 * scx_cpu_arg() wraps a cpu arg being handed to an SCX op. For cid-form
 * schedulers it resolves to the matching cid; for cpu-form it passes @cpu
 * through. scx_cpu_ret() is the inverse for a cpu/cid returned from an op
 * (currently only ops.select_cpu); it validates the BPF-supplied cid and
 * triggers scx_error() on @sch if invalid.
 */
స్థిర అంతఃవరుస s32 scx_cpu_arg(s32 cpu)
{
	ఒకవేళ (scx_is_cid_type())
		తిరిగి __scx_cpu_to_cid(cpu);
	తిరిగి cpu;
}

స్థిర అంతఃవరుస s32 scx_cpu_ret(నిర్మాణం scx_sched *sch, s32 cpu_or_cid)
{
	ఒకవేళ (cpu_or_cid < 0 || !scx_is_cid_type())
		తిరిగి cpu_or_cid;
	తిరిగి scx_cid_గమ్యం_cpu(sch, cpu_or_cid);
}

పూర్ణాంకం scx_cmask_ref_init(నిర్మాణం scx_sched *sch, స్థిరరాశి నిర్మాణం scx_cmask *src,
		       నిర్మాణం scx_cmask_ref *ref);
శూన్య scx_cmask_ref_init_kern(నిర్మాణం scx_sched *sch, నిర్మాణం scx_cmask *m,
			     u32 పునాది, u32 nr_cids, నిర్మాణం scx_cmask_ref *ref);
శూన్య scx_cmask_ref_shard(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref, s32 shard_idx,
			 నిర్మాణం scx_cmask *బహిర్గతం);
శూన్య scx_cmask_ref_from_cpumask(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref,
				స్థిరరాశి నిర్మాణం cpumask *cpumask);
శూన్య scx_cmask_ref_or(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref, స్థిరరాశి నిర్మాణం scx_cmask *src);
శూన్య scx_cmask_ref_copy(స్థిరరాశి నిర్మాణం scx_cmask_ref *ref, స్థిరరాశి నిర్మాణం scx_cmask *src);

#అయితే_ముగింపు /* _KERNEL_SCHED_EXT_CID_H */
