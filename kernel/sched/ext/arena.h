/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#اگر_غیر_تعریف_شدہ _KERNEL_SCHED_EXT_ARENA_H
#تعریف_کرو _KERNEL_SCHED_EXT_ARENA_H

#شامل <linux/types.h>

ساخت scx_sched;

s32 scx_arena_pool_init(ساخت scx_sched *sch);
خالی scx_arena_pool_destroy(ساخت scx_sched *sch);
خالی *scx_arena_alloc(ساخت scx_sched *sch, سائز_t سائز);
خالی scx_arena_free(ساخت scx_sched *sch, خالی *kern_va, سائز_t سائز);

#اگر_ختم /* _KERNEL_SCHED_EXT_ARENA_H */
