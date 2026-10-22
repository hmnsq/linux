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
#ಅನ್ತರ್ಭಾವ <linux/genalloc.h>

#ಅನ್ತರ್ಭಾವ "internal.h"
#ಅನ್ತರ್ಭಾವ "arena.h"

ಗಣನಾ scx_arena_consts {
	SCX_ARENA_MIN_ORDER		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_PAGES		= 4,	/* per growth */
};

s32 scx_arena_pool_init(ಸಂರಚನಾ scx_sched *sch)
{
	ಯದಿ (!sch->arena_map)
		ಪ್ರತ್ಯಾಗಮ 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ORDER, NUMA_NO_ಗಂಟು);
	ಯದಿ (!sch->arena_pool)
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_arena_clear_chunk(ಸಂರಚನಾ gen_pool *pool, ಸಂರಚನಾ gen_pool_chunk *chunk,
				  ಶೂನ್ಯ *data)
{
	ಪೂರ್ಣಾಙ್ಕ order = pool->min_alloc_order;
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
ಶೂನ್ಯ scx_arena_pool_destroy(ಸಂರಚನಾ scx_sched *sch)
{
	ಯದಿ (!sch->arena_pool)
		ಪ್ರತ್ಯಾಗಮ;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, ರಿಕ್ತ);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = ರಿಕ್ತ;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ scx_arena_grow(ಸಂರಚನಾ scx_sched *sch, u32 page_cnt)
{
	ಶೂನ್ಯ *p;
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಯದಿ (!sch->arena_map || !sch->arena_pool)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	p = bpf_arena_alloc_pages_sleepable(sch->arena_map, ರಿಕ್ತ,
					    page_cnt, NUMA_NO_ಗಂಟು, 0);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	ret = gen_pool_add(sch->arena_pool,
			   (unsigned long)scx_arena_to_kaddr(sch, p),
			   page_cnt * PAGE_SIZE, NUMA_NO_ಗಂಟು);
	ಯದಿ (ret) {
		bpf_arena_free_pages_non_sleepable(sch->arena_map, p, page_cnt);
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
ಶೂನ್ಯ *scx_arena_alloc(ಸಂರಚನಾ scx_sched *sch, size_t size)
{
	unsigned long kern_va;
	u32 page_cnt;

	might_sleep();

	ಯದಿ (!sch->arena_pool)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯಾವತ್ (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, size);
		ಯದಿ (kern_va)
			ಭಙ್ಗ;
		page_cnt = max_t(u32, SCX_ARENA_GROW_PAGES,
				 (size + PAGE_SIZE - 1) >> PAGE_SHIFT);
		ಯದಿ (scx_arena_grow(sch, page_cnt))
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ *)kern_va;
}

ಶೂನ್ಯ scx_arena_free(ಸಂರಚನಾ scx_sched *sch, ಶೂನ್ಯ *kern_va, size_t size)
{
	ಯದಿ (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (unsigned long)kern_va, size);
}
