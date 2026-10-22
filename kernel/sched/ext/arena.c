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

ಎಣಿಕೆ scx_arena_consts {
	SCX_ARENA_MIN_ಕ್ರಮ		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_ಪುಟಗಳು		= 4,	/* per growth */
};

s32 scx_arena_pool_init(ರಚನೆ scx_sched *sch)
{
	ಆದರೆ (!sch->arena_map)
		ಹಿಂತಿರುಗಿ 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ಕ್ರಮ, NUMA_NO_ಗಂಟು);
	ಆದರೆ (!sch->arena_pool)
		ಹಿಂತಿರುಗಿ -ENOMEM;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ scx_arena_clear_chunk(ರಚನೆ gen_pool *pool, ರಚನೆ gen_pool_chunk *chunk,
				  ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಪೂರ್ಣಾಂಕ ಕ್ರಮ = pool->min_alloc_ಕ್ರಮ;
	ಗಾತ್ರ_t chunk_sz = chunk->ಕೊನೆ_addr - chunk->ಆರಂಭ_addr + 1;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಕೊನೆ_ಬಿಟ್ = chunk_sz >> ಕ್ರಮ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ b, e;

	for_each_ಗಣ_bitrange(b, e, chunk->ಬಿಟ್ಟುಗಳು, ಕೊನೆ_ಬಿಟ್)
		gen_pool_free(pool, chunk->ಆರಂಭ_addr + (b << ಕ್ರಮ),
			      (e - b) << ಕ್ರಮ);
}

/*
 * Tear down the pool. Outstanding gen_pool allocations are freed via
 * scx_arena_clear_chunk() so gen_pool_destroy() doesn't BUG. The underlying
 * arena pages are released when the arena map itself is torn down.
 */
ಶೂನ್ಯ scx_arena_pool_destroy(ರಚನೆ scx_sched *sch)
{
	ಆದರೆ (!sch->arena_pool)
		ಹಿಂತಿರುಗಿ;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, ಖಾಲಿ);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = ಖಾಲಿ;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ scx_arena_grow(ರಚನೆ scx_sched *sch, u32 ಪುಟ_cnt)
{
	ಶೂನ್ಯ *p;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (!sch->arena_map || !sch->arena_pool)
		ಹಿಂತಿರುಗಿ -EINVAL;

	p = bpf_arena_alloc_ಪುಟಗಳು_sleepable(sch->arena_map, ಖಾಲಿ,
					    ಪುಟ_cnt, NUMA_NO_ಗಂಟು, 0);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	ret = gen_pool_ಕೂಡಿಸು(sch->arena_pool,
			   (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)scx_arena_ಗಮ್ಯ_kaddr(sch, p),
			   ಪುಟ_cnt * ಪುಟ_ಗಾತ್ರ, NUMA_NO_ಗಂಟು);
	ಆದರೆ (ret) {
		bpf_arena_free_ಪುಟಗಳು_non_sleepable(sch->arena_map, p, ಪುಟ_cnt);
		ಹಿಂತಿರುಗಿ ret;
	}
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
ಶೂನ್ಯ *scx_arena_alloc(ರಚನೆ scx_sched *sch, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ kern_va;
	u32 ಪುಟ_cnt;

	might_sleep();

	ಆದರೆ (!sch->arena_pool)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆಗ (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, ಗಾತ್ರ_2);
		ಆದರೆ (kern_va)
			ನಿಲ್ಲಿಸು;
		ಪುಟ_cnt = max_t(u32, SCX_ARENA_GROW_ಪುಟಗಳು,
				 (ಗಾತ್ರ_2 + ಪುಟ_ಗಾತ್ರ - 1) >> ಪುಟ_ಸರಿಸು);
		ಆದರೆ (scx_arena_grow(sch, ಪುಟ_cnt))
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ (ಶೂನ್ಯ *)kern_va;
}

ಶೂನ್ಯ scx_arena_free(ರಚನೆ scx_sched *sch, ಶೂನ್ಯ *kern_va, ಗಾತ್ರ_t ಗಾತ್ರ_2)
{
	ಆದರೆ (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)kern_va, ಗಾತ್ರ_2);
}
