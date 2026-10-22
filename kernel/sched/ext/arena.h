/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#অন্তর্ভাব <linux/types.h>

কাঠামো scx_sched;

s32 scx_arena_pool_init(কাঠামো scx_sched *sch);
শূন্য scx_arena_pool_destroy(কাঠামো scx_sched *sch);
শূন্য *scx_arena_alloc(কাঠামো scx_sched *sch, আকার_t আকার_2);
শূন্য scx_arena_free(কাঠামো scx_sched *sch, শূন্য *kern_va, আকার_t আকার_2);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
