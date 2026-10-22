/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#جے_بنا_تعریف _KERNEL_SCHED_EXT_ARENA_H
#تعریف_کرو _KERNEL_SCHED_EXT_ARENA_H

#ضمن <linux/types.h>

بنية scx_sched;

s32 scx_arena_pool_init(بنية scx_sched *sch);
فراغ scx_arena_pool_destroy(بنية scx_sched *sch);
فراغ *scx_arena_alloc(بنية scx_sched *sch, ناپ_t ناپ);
فراغ scx_arena_free(بنية scx_sched *sch, فراغ *kern_va, ناپ_t ناپ);

#جے_انت /* _KERNEL_SCHED_EXT_ARENA_H */
