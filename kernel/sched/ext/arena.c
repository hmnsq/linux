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
#अन्तर्भाव <linux/genalloc.h>

#अन्तर्भाव "internal.h"
#अन्तर्भाव "arena.h"

गणना scx_arena_consts {
	SCX_ARENA_MIN_ORDER		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_PAGES		= 4,	/* per growth */
};

s32 scx_arena_pool_init(संरचना scx_sched *sch)
{
	यदि (!sch->arena_map)
		प्रत्यागम 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ORDER, NUMA_NO_गाँठ);
	यदि (!sch->arena_pool)
		प्रत्यागम -ENOMEM;
	प्रत्यागम 0;
}

स्थिर शून्य scx_arena_clear_chunk(संरचना gen_pool *pool, संरचना gen_pool_chunk *chunk,
				  शून्य *data)
{
	पूर्णाङ्क order = pool->min_alloc_order;
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
शून्य scx_arena_pool_destroy(संरचना scx_sched *sch)
{
	यदि (!sch->arena_pool)
		प्रत्यागम;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, रिक्त);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = रिक्त;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
स्थिर पूर्णाङ्क scx_arena_grow(संरचना scx_sched *sch, u32 page_cnt)
{
	शून्य *p;
	पूर्णाङ्क ret;

	यदि (!sch->arena_map || !sch->arena_pool)
		प्रत्यागम -EINVAL;

	p = bpf_arena_alloc_pages_sleepable(sch->arena_map, रिक्त,
					    page_cnt, NUMA_NO_गाँठ, 0);
	यदि (!p)
		प्रत्यागम -ENOMEM;

	ret = gen_pool_add(sch->arena_pool,
			   (अचिन्हित दीर्घ)scx_arena_to_kaddr(sch, p),
			   page_cnt * PAGE_SIZE, NUMA_NO_गाँठ);
	यदि (ret) {
		bpf_arena_free_pages_non_sleepable(sch->arena_map, p, page_cnt);
		प्रत्यागम ret;
	}
	प्रत्यागम 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
शून्य *scx_arena_alloc(संरचना scx_sched *sch, माप_प्रकार size)
{
	अचिन्हित दीर्घ kern_va;
	u32 page_cnt;

	might_sleep();

	यदि (!sch->arena_pool)
		प्रत्यागम रिक्त;

	यावत् (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, size);
		यदि (kern_va)
			भङ्ग;
		page_cnt = max_t(u32, SCX_ARENA_GROW_PAGES,
				 (size + PAGE_SIZE - 1) >> PAGE_SHIFT);
		यदि (scx_arena_grow(sch, page_cnt))
			प्रत्यागम रिक्त;
	}

	प्रत्यागम (शून्य *)kern_va;
}

शून्य scx_arena_free(संरचना scx_sched *sch, शून्य *kern_va, माप_प्रकार size)
{
	यदि (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (अचिन्हित दीर्घ)kern_va, size);
}
