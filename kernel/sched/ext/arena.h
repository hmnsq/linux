/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#شامل <linux/types.h>

ساختار scx_sched;

s32 scx_arena_pool_init(ساختار scx_sched *sch);
تهی scx_arena_pool_destroy(ساختار scx_sched *sch);
تهی *scx_arena_alloc(ساختار scx_sched *sch, size_t size);
تهی scx_arena_free(ساختار scx_sched *sch, تهی *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
