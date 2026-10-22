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
#অন্তর্ভাব <linux/genalloc.h>

#অন্তর্ভাব "internal.h"
#অন্তর্ভাব "arena.h"

গণনা scx_arena_consts {
	SCX_ARENA_MIN_ORDER		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_PAGES		= 4,	/* per growth */
};

s32 scx_arena_pool_init(সংরচনা scx_sched *sch)
{
	যদি (!sch->arena_map)
		প্রত্যাগম 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ORDER, NUMA_NO_গ্রন্থি);
	যদি (!sch->arena_pool)
		প্রত্যাগম -ENOMEM;
	প্রত্যাগম 0;
}

স্থির শূন্য scx_arena_clear_chunk(সংরচনা gen_pool *pool, সংরচনা gen_pool_chunk *chunk,
				  শূন্য *data)
{
	পূর্ণাঙ্ক order = pool->min_alloc_order;
	size_t chunk_sz = chunk->end_addr - chunk->start_addr + 1;
	unsigned long end_bit = chunk_sz >> order;
	unsigned long b, e;

	for_each_set_bitrange(b, e, chunk->bits, end_bit)
		gen_pool_free(pool, chunk->start_addr + (b << order),
			      (e - b) << order);
}

/*
 * Tear down the pool. Outstanding gen_pool allocations are freed via
 * scx_arena_clear_chunk() so gen_pool_destroy() doesn't BUG. The underlying
 * arena pages are released when the arena map itself is torn down.
 */
শূন্য scx_arena_pool_destroy(সংরচনা scx_sched *sch)
{
	যদি (!sch->arena_pool)
		প্রত্যাগম;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, রিক্ত);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = রিক্ত;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
স্থির পূর্ণাঙ্ক scx_arena_grow(সংরচনা scx_sched *sch, u32 page_cnt)
{
	শূন্য *p;
	পূর্ণাঙ্ক ret;

	যদি (!sch->arena_map || !sch->arena_pool)
		প্রত্যাগম -EINVAL;

	p = bpf_arena_alloc_pages_sleepable(sch->arena_map, রিক্ত,
					    page_cnt, NUMA_NO_গ্রন্থি, 0);
	যদি (!p)
		প্রত্যাগম -ENOMEM;

	ret = gen_pool_add(sch->arena_pool,
			   (unsigned long)scx_arena_to_kaddr(sch, p),
			   page_cnt * PAGE_SIZE, NUMA_NO_গ্রন্থি);
	যদি (ret) {
		bpf_arena_free_pages_non_sleepable(sch->arena_map, p, page_cnt);
		প্রত্যাগম ret;
	}
	প্রত্যাগম 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
শূন্য *scx_arena_alloc(সংরচনা scx_sched *sch, size_t size)
{
	unsigned long kern_va;
	u32 page_cnt;

	might_sleep();

	যদি (!sch->arena_pool)
		প্রত্যাগম রিক্ত;

	যাবত্ (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, size);
		যদি (kern_va)
			ভঙ্গ;
		page_cnt = max_t(u32, SCX_ARENA_GROW_PAGES,
				 (size + PAGE_SIZE - 1) >> PAGE_SHIFT);
		যদি (scx_arena_grow(sch, page_cnt))
			প্রত্যাগম রিক্ত;
	}

	প্রত্যাগম (শূন্য *)kern_va;
}

শূন্য scx_arena_free(সংরচনা scx_sched *sch, শূন্য *kern_va, size_t size)
{
	যদি (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (unsigned long)kern_va, size);
}
