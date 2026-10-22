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
#समावेश <linux/genalloc.h>

#समावेश "internal.h"
#समावेश "arena.h"

क्रमागत scx_arena_consts {
	SCX_ARENA_MIN_ORDER		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_PAGES		= 4,	/* per growth */
};

s32 scx_arena_pool_init(काष्ठा scx_sched *sch)
{
	अगर (!sch->arena_map)
		वापस 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ORDER, NUMA_NO_गाँठ);
	अगर (!sch->arena_pool)
		वापस -ENOMEM;
	वापस 0;
}

अटल व्योम scx_arena_clear_chunk(काष्ठा gen_pool *pool, काष्ठा gen_pool_chunk *chunk,
				  व्योम *तथ्य)
{
	पूर्णांक order = pool->min_alloc_order;
	माप_प्रकार chunk_sz = chunk->end_addr - chunk->start_addr + 1;
	अचिन्हित दीर्घ end_bit = chunk_sz >> order;
	अचिन्हित दीर्घ b, e;

	for_each_set_bitrange(b, e, chunk->bits, end_bit)
		gen_pool_free(pool, chunk->start_addr + (b << order),
			      (e - b) << order);
}

/*
 * Tear down the pool. Outstanding gen_pool allocations are freed via
 * scx_arena_clear_chunk() so gen_pool_destroy() doesn't BUG. The underlying
 * arena pages are released when the arena map itself is torn down.
 */
व्योम scx_arena_pool_destroy(काष्ठा scx_sched *sch)
{
	अगर (!sch->arena_pool)
		वापस;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, शून्य);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = शून्य;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
अटल पूर्णांक scx_arena_grow(काष्ठा scx_sched *sch, u32 page_cnt)
{
	व्योम *p;
	पूर्णांक ret;

	अगर (!sch->arena_map || !sch->arena_pool)
		वापस -EINVAL;

	p = bpf_arena_alloc_pages_sleepable(sch->arena_map, शून्य,
					    page_cnt, NUMA_NO_गाँठ, 0);
	अगर (!p)
		वापस -ENOMEM;

	ret = gen_pool_add(sch->arena_pool,
			   (अचिन्हित दीर्घ)scx_arena_to_kaddr(sch, p),
			   page_cnt * PAGE_आकार, NUMA_NO_गाँठ);
	अगर (ret) {
		bpf_arena_free_pages_non_sleepable(sch->arena_map, p, page_cnt);
		वापस ret;
	}
	वापस 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
व्योम *scx_arena_alloc(काष्ठा scx_sched *sch, माप_प्रकार आकार)
{
	अचिन्हित दीर्घ kern_va;
	u32 page_cnt;

	might_sleep();

	अगर (!sch->arena_pool)
		वापस शून्य;

	जबतक (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, आकार);
		अगर (kern_va)
			अवरोध;
		page_cnt = max_t(u32, SCX_ARENA_GROW_PAGES,
				 (आकार + PAGE_आकार - 1) >> PAGE_SHIFT);
		अगर (scx_arena_grow(sch, page_cnt))
			वापस शून्य;
	}

	वापस (व्योम *)kern_va;
}

व्योम scx_arena_free(काष्ठा scx_sched *sch, व्योम *kern_va, माप_प्रकार आकार)
{
	अगर (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (अचिन्हित दीर्घ)kern_va, आकार);
}
