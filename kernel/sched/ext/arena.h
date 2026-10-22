/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#அந்தர்பாவ <linux/types.h>

கட்டமைப்பு scx_sched;

s32 scx_arena_pool_init(கட்டமைப்பு scx_sched *sch);
வெறுமை scx_arena_pool_destroy(கட்டமைப்பு scx_sched *sch);
வெறுமை *scx_arena_alloc(கட்டமைப்பு scx_sched *sch, size_t size);
வெறுமை scx_arena_free(கட்டமைப்பு scx_sched *sch, வெறுமை *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
