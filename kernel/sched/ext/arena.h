/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#ଅନ୍ତର୍ଭାଵ <linux/types.h>

ସଂରଚନା scx_sched;

s32 scx_arena_pool_init(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ scx_arena_pool_destroy(ସଂରଚନା scx_sched *sch);
ଶୂନ୍ୟ *scx_arena_alloc(ସଂରଚନା scx_sched *sch, size_t size);
ଶୂନ୍ୟ scx_arena_free(ସଂରଚନା scx_sched *sch, ଶୂନ୍ୟ *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
