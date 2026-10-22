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
#شامل <linux/genalloc.h>

#شامل "internal.h"
#شامل "arena.h"

شمار scx_arena_consts {
	SCX_ARENA_MIN_ترتیب		= 3,	/* 8-byte minimum sub-allocation */
	SCX_ARENA_GROW_صفحات		= 4,	/* per growth */
};

s32 scx_arena_pool_init(ساخت scx_sched *sch)
{
	اگر (!sch->arena_map)
		واپس 0;

	sch->arena_pool = gen_pool_create(SCX_ARENA_MIN_ترتیب, NUMA_NO_گرہ);
	اگر (!sch->arena_pool)
		واپس -ENOMEM;
	واپس 0;
}

ساکن خالی scx_arena_clear_chunk(ساخت gen_pool *pool, ساخت gen_pool_chunk *chunk,
				  خالی *ڈیٹا)
{
	صحیح ترتیب = pool->min_alloc_ترتیب;
	سائز_t chunk_sz = chunk->انتہا_addr - chunk->آغاز_addr + 1;
	غیرعلامتی طویل انتہا_ثنائی_ہندسہ = chunk_sz >> ترتیب;
	غیرعلامتی طویل b, e;

	for_each_سیٹ_bitrange(b, e, chunk->بٹ, انتہا_ثنائی_ہندسہ)
		gen_pool_free(pool, chunk->آغاز_addr + (b << ترتیب),
			      (e - b) << ترتیب);
}

/*
 * Tear down the pool. Outstanding gen_pool allocations are freed via
 * scx_arena_clear_chunk() so gen_pool_destroy() doesn't BUG. The underlying
 * arena pages are released when the arena map itself is torn down.
 */
خالی scx_arena_pool_destroy(ساخت scx_sched *sch)
{
	اگر (!sch->arena_pool)
		واپس;
	gen_pool_for_each_chunk(sch->arena_pool, scx_arena_clear_chunk, تہی);
	gen_pool_destroy(sch->arena_pool);
	sch->arena_pool = تہی;
}

/*
 * Grow the pool by @page_cnt pages. bpf_arena_alloc_pages_sleepable() and
 * gen_pool_add() (which calls vzalloc(GFP_KERNEL)) require a sleepable
 * context.
 */
ساکن صحیح scx_arena_grow(ساخت scx_sched *sch, u32 صفحہ_cnt)
{
	خالی *p;
	صحیح ret;

	اگر (!sch->arena_map || !sch->arena_pool)
		واپس -EINVAL;

	p = bpf_arena_alloc_صفحات_sleepable(sch->arena_map, تہی,
					    صفحہ_cnt, NUMA_NO_گرہ, 0);
	اگر (!p)
		واپس -ENOMEM;

	ret = gen_pool_جوڑو(sch->arena_pool,
			   (غیرعلامتی طویل)scx_arena_مقصد_kaddr(sch, p),
			   صفحہ_cnt * صفحہ_سائز, NUMA_NO_گرہ);
	اگر (ret) {
		bpf_arena_free_صفحات_non_sleepable(sch->arena_map, p, صفحہ_cnt);
		واپس ret;
	}
	واپس 0;
}

/*
 * Allocate @size bytes from the arena pool. Returns kernel VA on success, NULL
 * on failure. May grow the pool via scx_arena_grow() which sleeps. Caller must
 * be in a GFP_KERNEL context.
 */
خالی *scx_arena_alloc(ساخت scx_sched *sch, سائز_t سائز)
{
	غیرعلامتی طویل kern_va;
	u32 صفحہ_cnt;

	might_sleep();

	اگر (!sch->arena_pool)
		واپس تہی;

	جب (true) {
		kern_va = gen_pool_alloc(sch->arena_pool, سائز);
		اگر (kern_va)
			توڑو;
		صفحہ_cnt = max_t(u32, SCX_ARENA_GROW_صفحات,
				 (سائز + صفحہ_سائز - 1) >> صفحہ_سرکاؤ);
		اگر (scx_arena_grow(sch, صفحہ_cnt))
			واپس تہی;
	}

	واپس (خالی *)kern_va;
}

خالی scx_arena_free(ساخت scx_sched *sch, خالی *kern_va, سائز_t سائز)
{
	اگر (sch->arena_pool && kern_va)
		gen_pool_free(sch->arena_pool, (غیرعلامتی طویل)kern_va, سائز);
}
