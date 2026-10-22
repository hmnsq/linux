/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ifndef _KERNEL_SCHED_EXT_ARENA_H
#define _KERNEL_SCHED_EXT_ARENA_H

#અન્તર્ભાવ <linux/types.h>

સંરચના scx_sched;

s32 scx_arena_pool_init(સંરચના scx_sched *sch);
શૂન્ય scx_arena_pool_destroy(સંરચના scx_sched *sch);
શૂન્ય *scx_arena_alloc(સંરચના scx_sched *sch, size_t size);
શૂન્ય scx_arena_free(સંરચના scx_sched *sch, શૂન્ય *kern_va, size_t size);

#endif /* _KERNEL_SCHED_EXT_ARENA_H */
