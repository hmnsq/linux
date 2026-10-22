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
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/genalloc.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "internal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "arena.h"

ਗਿਣਤੀ scx_arena_consts {
	SCX_ARENA_MIN_ਕ੍ਰਮ		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_ਪੰਨੇ		= 4,	/* per growth */
};

s32 scx_arena_pool_init(ਬਣਤਰ scx_sched *sch)
{
	ਜੇ (!sch->arena_map)
		ਵਾਪਸ 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ਕ੍ਰਮ, NUMA_NO_ਗੰਢ);
	ਜੇ (!sch->arena_pool)
		ਵਾਪਸ -ENOMEM;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ scx_arena_clear_chunk(ਬਣਤਰ gen_pool *pool, ਬਣਤਰ gen_pool_chunk *chunk,
				  ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਪੂਰਨ ਕ੍ਰਮ = pool->min_alloc_ਕ੍ਰਮ;
	ਮਾਪ_t chunk_sz = chunk->ਸਿਰਾ_addr - chunk->ਸ਼ੁਰੂ_addr + 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਿਰਾ_ਦੋਆਧਾਰੀ_ਅੰਕ = chunk_sz >> ਕ੍ਰਮ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ b, e;

	for_each_ਸੈੱਟ_bitrange(b, e, chunk->ਬਿੱਟ, ਸਿਰਾ_ਦੋਆਧਾਰੀ_ਅੰਕ)
		gen_pool_free(pool, chunk->ਸ਼ੁਰੂ_addr + (b << ਕ੍ਰਮ),
			      (e - b) << ਕ੍ਰਮ);
}

/*
 * Tear down the pool. Outstanding gen_pool allocations are freed via
 * scx_arena_clear_chunk() so gen_pool_destroy() doesn't BUG. The underlying
 * arena pages are released when the arena map itself is torn down.
 */
ਖਾਲੀ scx_arena_pool_destroy(ਬਣਤਰ scx_sched *sch)
{
	ਜੇ (!sch->arena_pool)
		ਵਾਪਸ;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, ਨਿਰਾ);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = ਨਿਰਾ;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
ਸਥਿਰ ਪੂਰਨ scx_arena_grow(ਬਣਤਰ scx_sched *sch, u32 ਪੰਨਾ_cnt)
{
	ਖਾਲੀ *p;
	ਪੂਰਨ ret;

	ਜੇ (!sch->arena_map || !sch->arena_pool)
		ਵਾਪਸ -EINVAL;

	p = bpf_arena_alloc_ਪੰਨੇ_sleepable(sch->arena_map, ਨਿਰਾ,
					    ਪੰਨਾ_cnt, NUMA_NO_ਗੰਢ, 0);
	ਜੇ (!p)
		ਵਾਪਸ -ENOMEM;

	ret = gen_pool_ਜੋੜੋ(sch->arena_pool,
			   (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)scx_arena_ਮੰਜ਼ਿਲ_kaddr(sch, p),
			   ਪੰਨਾ_cnt * ਪੰਨਾ_ਮਾਪ, NUMA_NO_ਗੰਢ);
	ਜੇ (ret) {
		bpf_arena_free_ਪੰਨੇ_non_sleepable(sch->arena_map, p, ਪੰਨਾ_cnt);
		ਵਾਪਸ ret;
	}
	ਵਾਪਸ 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
ਖਾਲੀ *scx_arena_alloc(ਬਣਤਰ scx_sched *sch, ਮਾਪ_t ਮਾਪ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ kern_va;
	u32 ਪੰਨਾ_cnt;

	might_sleep();

	ਜੇ (!sch->arena_pool)
		ਵਾਪਸ ਨਿਰਾ;

	ਜਦੋਂ (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, ਮਾਪ);
		ਜੇ (kern_va)
			ਰੋਕੋ;
		ਪੰਨਾ_cnt = max_t(u32, SCX_ARENA_GROW_ਪੰਨੇ,
				 (ਮਾਪ + ਪੰਨਾ_ਮਾਪ - 1) >> ਪੰਨਾ_ਖਿਸਕਾਓ);
		ਜੇ (scx_arena_grow(sch, ਪੰਨਾ_cnt))
			ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ (ਖਾਲੀ *)kern_va;
}

ਖਾਲੀ scx_arena_free(ਬਣਤਰ scx_sched *sch, ਖਾਲੀ *kern_va, ਮਾਪ_t ਮਾਪ)
{
	ਜੇ (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)kern_va, ਮਾਪ);
}
