/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#അന്തര്ഭാവ <linux/types.h>

ഘടന scx_sched;

s32 scx_arena_pool_init(ഘടന scx_sched *sch);
ശൂന്യം scx_arena_pool_destroy(ഘടന scx_sched *sch);
ശൂന്യം *scx_arena_alloc(ഘടന scx_sched *sch, size_t size);
ശൂന്യം scx_arena_free(ഘടന scx_sched *sch, ശൂന്യം *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
