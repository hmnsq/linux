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
#അന്തര്ഭാവ <linux/genalloc.h>

#അന്തര്ഭാവ "internal.h"
#അന്തര്ഭാവ "arena.h"

എണ്ണൽ scx_arena_consts {
	SCX_ARENA_MIN_ORDER		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_PAGES		= 4,	/* per growth */
};

s32 scx_arena_pool_init(ഘടന scx_sched *sch)
{
	എങ്കിൽ (!sch->arena_map)
		തിരികെ 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ORDER, NUMA_NO_കണ്ണി);
	എങ്കിൽ (!sch->arena_pool)
		തിരികെ -ENOMEM;
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം scx_arena_clear_chunk(ഘടന gen_pool *pool, ഘടന gen_pool_chunk *chunk,
				  ശൂന്യം *data)
{
	പൂർണ്ണസംഖ്യ order = pool->min_alloc_order;
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
ശൂന്യം scx_arena_pool_destroy(ഘടന scx_sched *sch)
{
	എങ്കിൽ (!sch->arena_pool)
		തിരികെ;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, ശൂന്യ);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = ശൂന്യ;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ scx_arena_grow(ഘടന scx_sched *sch, u32 page_cnt)
{
	ശൂന്യം *p;
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (!sch->arena_map || !sch->arena_pool)
		തിരികെ -EINVAL;

	p = bpf_arena_alloc_pages_sleepable(sch->arena_map, ശൂന്യ,
					    page_cnt, NUMA_NO_കണ്ണി, 0);
	എങ്കിൽ (!p)
		തിരികെ -ENOMEM;

	ret = gen_pool_add(sch->arena_pool,
			   (unsigned long)scx_arena_to_kaddr(sch, p),
			   page_cnt * PAGE_SIZE, NUMA_NO_കണ്ണി);
	എങ്കിൽ (ret) {
		bpf_arena_free_pages_non_sleepable(sch->arena_map, p, page_cnt);
		തിരികെ ret;
	}
	തിരികെ 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
ശൂന്യം *scx_arena_alloc(ഘടന scx_sched *sch, size_t size)
{
	unsigned long kern_va;
	u32 page_cnt;

	might_sleep();

	എങ്കിൽ (!sch->arena_pool)
		തിരികെ ശൂന്യ;

	ആകുമ്പോൾ (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, size);
		എങ്കിൽ (kern_va)
			നിർത്തുക;
		page_cnt = max_t(u32, SCX_ARENA_GROW_PAGES,
				 (size + PAGE_SIZE - 1) >> PAGE_SHIFT);
		എങ്കിൽ (scx_arena_grow(sch, page_cnt))
			തിരികെ ശൂന്യ;
	}

	തിരികെ (ശൂന്യം *)kern_va;
}

ശൂന്യം scx_arena_free(ഘടന scx_sched *sch, ശൂന്യം *kern_va, size_t size)
{
	എങ്കിൽ (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (unsigned long)kern_va, size);
}
