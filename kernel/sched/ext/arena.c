/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * scx_arena_pool: kernel-side sub-allocator over BPF-arena pages.
 *
 * Each chunk added to @sch->arena_pool comes from one
 * bpf_arena_alloc_pages_sleepable() call and is registered at the
 * kernel-side mapping address.
 *
 * Allocations grow the pool on demand. Underlying arena pages are released
 * when the arena map itself is torn down.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ضمن <linux/genalloc.h>

#ضمن "internal.h"
#ضمن "arena.h"

تعداد scx_arena_consts {
	SCX_ARENA_MIN_ترتيب		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_صفحا		= 4,	/* per growth */
};

s32 scx_arena_pool_init(بنية scx_sched *sch)
{
	إذا (!sch->arena_map)
		أرجع 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ترتيب, NUMA_NO_ڳنڍ);
	إذا (!sch->arena_pool)
		أرجع -ENOMEM;
	أرجع 0;
}

ساكن فراغ scx_arena_clear_chunk(بنية gen_pool *pool, بنية gen_pool_chunk *chunk,
				  فراغ *معلومات)
{
	صحيح ترتيب = pool->min_alloc_ترتيب;
	سائيز_t chunk_sz = chunk->پڇاڙي_addr - chunk->شروعات_addr + 1;
	بنانشان ڊگهو پڇاڙي_ٻٽو_انگ = chunk_sz >> ترتيب;
	بنانشان ڊگهو b, e;

	for_each_سيٽ_bitrange(b, e, chunk->بٽ, پڇاڙي_ٻٽو_انگ)
		gen_pool_free(pool, chunk->شروعات_addr + (b << ترتيب),
			      (e - b) << ترتيب);
}

/*
 * Tear down the pool. Outstanding gen_pool allocations are freed via
 * scx_arena_clear_chunk() so gen_pool_destroy() doesn't BUG. The underlying
 * arena pages are released when the arena map itself is torn down.
 */
فراغ scx_arena_pool_destroy(بنية scx_sched *sch)
{
	إذا (!sch->arena_pool)
		أرجع;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, عدم);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = عدم;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
ساكن صحيح scx_arena_grow(بنية scx_sched *sch, u32 صفحو_cnt)
{
	فراغ *p;
	صحيح ret;

	إذا (!sch->arena_map || !sch->arena_pool)
		أرجع -EINVAL;

	p = bpf_arena_alloc_صفحا_sleepable(sch->arena_map, عدم,
					    صفحو_cnt, NUMA_NO_ڳنڍ, 0);
	إذا (!p)
		أرجع -ENOMEM;

	ret = gen_pool_جوڙيو(sch->arena_pool,
			   (بنانشان ڊگهو)scx_arena_منزل_kaddr(sch, p),
			   صفحو_cnt * صفحو_سائيز, NUMA_NO_ڳنڍ);
	إذا (ret) {
		bpf_arena_free_صفحا_non_sleepable(sch->arena_map, p, صفحو_cnt);
		أرجع ret;
	}
	أرجع 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
فراغ *scx_arena_alloc(بنية scx_sched *sch, سائيز_t سائيز)
{
	بنانشان ڊگهو kern_va;
	u32 صفحو_cnt;

	might_sleep();

	إذا (!sch->arena_pool)
		أرجع عدم;

	طالما (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, سائيز);
		إذا (kern_va)
			اكسر;
		صفحو_cnt = max_t(u32, SCX_ARENA_GROW_صفحا,
				 (سائيز + صفحو_سائيز - 1) >> صفحو_سرڪايو);
		إذا (scx_arena_grow(sch, صفحو_cnt))
			أرجع عدم;
	}

	أرجع (فراغ *)kern_va;
}

فراغ scx_arena_free(بنية scx_sched *sch, فراغ *kern_va, سائيز_t سائيز)
{
	إذا (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (بنانشان ڊگهو)kern_va, سائيز);
}
