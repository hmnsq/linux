/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#నిర్వచితం_కాకపోతే _KERNEL_SCHED_EXT_ARENA_H
#నిర్వచించు _KERNEL_SCHED_EXT_ARENA_H

#అన్తర్భావ <linux/types.h>

నిర్మాణం scx_sched;

s32 scx_arena_pool_init(నిర్మాణం scx_sched *sch);
శూన్య scx_arena_pool_destroy(నిర్మాణం scx_sched *sch);
శూన్య *scx_arena_alloc(నిర్మాణం scx_sched *sch, కొలత_t కొలత);
శూన్య scx_arena_free(నిర్మాణం scx_sched *sch, శూన్య *kern_va, కొలత_t కొలత);

#అయితే_ముగింపు /* _KERNEL_SCHED_EXT_ARENA_H */
