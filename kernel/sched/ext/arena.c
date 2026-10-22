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
#అన్తర్భావ <linux/genalloc.h>

#అన్తర్భావ "internal.h"
#అన్తర్భావ "arena.h"

గణన scx_arena_consts {
	SCX_ARENA_MIN_క్రమం		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_పుటలు		= 4,	/* per growth */
};

s32 scx_arena_pool_init(నిర్మాణం scx_sched *sch)
{
	ఒకవేళ (!sch->arena_map)
		తిరిగి 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_క్రమం, NUMA_NO_కణుపు);
	ఒకవేళ (!sch->arena_pool)
		తిరిగి -ENOMEM;
	తిరిగి 0;
}

స్థిర శూన్య scx_arena_clear_chunk(నిర్మాణం gen_pool *pool, నిర్మాణం gen_pool_chunk *chunk,
				  శూన్య *దత్తాంశం)
{
	పూర్ణాంకం క్రమం = pool->min_alloc_క్రమం;
	కొలత_t chunk_sz = chunk->చివర_addr - chunk->ప్రారంభం_addr + 1;
	గుర్తులేని దీర్ఘ చివర_బిట్టు = chunk_sz >> క్రమం;
	గుర్తులేని దీర్ఘ b, e;

	for_each_సమితి_bitrange(b, e, chunk->బిట్లు, చివర_బిట్టు)
		gen_pool_free(pool, chunk->ప్రారంభం_addr + (b << క్రమం),
			      (e - b) << క్రమం);
}

/*
 * Tear down the pool. Outstanding gen_pool allocations are freed via
 * scx_arena_clear_chunk() so gen_pool_destroy() doesn't BUG. The underlying
 * arena pages are released when the arena map itself is torn down.
 */
శూన్య scx_arena_pool_destroy(నిర్మాణం scx_sched *sch)
{
	ఒకవేళ (!sch->arena_pool)
		తిరిగి;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, ఖాళీ);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = ఖాళీ;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
స్థిర పూర్ణాంకం scx_arena_grow(నిర్మాణం scx_sched *sch, u32 పుట_cnt)
{
	శూన్య *p;
	పూర్ణాంకం ret;

	ఒకవేళ (!sch->arena_map || !sch->arena_pool)
		తిరిగి -EINVAL;

	p = bpf_arena_alloc_పుటలు_sleepable(sch->arena_map, ఖాళీ,
					    పుట_cnt, NUMA_NO_కణుపు, 0);
	ఒకవేళ (!p)
		తిరిగి -ENOMEM;

	ret = gen_pool_కూడు(sch->arena_pool,
			   (గుర్తులేని దీర్ఘ)scx_arena_గమ్యం_kaddr(sch, p),
			   పుట_cnt * పుట_కొలత, NUMA_NO_కణుపు);
	ఒకవేళ (ret) {
		bpf_arena_free_పుటలు_non_sleepable(sch->arena_map, p, పుట_cnt);
		తిరిగి ret;
	}
	తిరిగి 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
శూన్య *scx_arena_alloc(నిర్మాణం scx_sched *sch, కొలత_t కొలత)
{
	గుర్తులేని దీర్ఘ kern_va;
	u32 పుట_cnt;

	might_sleep();

	ఒకవేళ (!sch->arena_pool)
		తిరిగి ఖాళీ;

	అయితే (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, కొలత);
		ఒకవేళ (kern_va)
			ఆపు;
		పుట_cnt = max_t(u32, SCX_ARENA_GROW_పుటలు,
				 (కొలత + పుట_కొలత - 1) >> పుట_జరుపు);
		ఒకవేళ (scx_arena_grow(sch, పుట_cnt))
			తిరిగి ఖాళీ;
	}

	తిరిగి (శూన్య *)kern_va;
}

శూన్య scx_arena_free(నిర్మాణం scx_sched *sch, శూన్య *kern_va, కొలత_t కొలత)
{
	ఒకవేళ (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (గుర్తులేని దీర్ఘ)kern_va, కొలత);
}
