/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#ಅನ್ತರ್ಭಾವ <linux/types.h>

ಸಂರಚನಾ scx_sched;

s32 scx_arena_pool_init(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ scx_arena_pool_destroy(ಸಂರಚನಾ scx_sched *sch);
ಶೂನ್ಯ *scx_arena_alloc(ಸಂರಚನಾ scx_sched *sch, size_t size);
ಶೂನ್ಯ scx_arena_free(ಸಂರಚನಾ scx_sched *sch, ಶೂನ್ಯ *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
