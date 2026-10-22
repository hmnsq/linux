/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#অন্তর্ভাৱ <linux/types.h>

সংরচনা scx_sched;

s32 scx_arena_pool_init(সংরচনা scx_sched *sch);
শূন্য scx_arena_pool_destroy(সংরচনা scx_sched *sch);
শূন্য *scx_arena_alloc(সংরচনা scx_sched *sch, size_t size);
শূন্য scx_arena_free(সংরচনা scx_sched *sch, শূন্য *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
