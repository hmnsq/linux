/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#ضمن <linux/types.h>

بنية scx_sched;

s32 scx_arena_pool_init(بنية scx_sched *sch);
فراغ scx_arena_pool_destroy(بنية scx_sched *sch);
فراغ *scx_arena_alloc(بنية scx_sched *sch, size_t size);
فراغ scx_arena_free(بنية scx_sched *sch, فراغ *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
