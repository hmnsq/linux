/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#جيڪڏهن_وصف_نه_ٿيل _KERNEL_SCHED_EXT_ARENA_H
#وصف_ڪريو _KERNEL_SCHED_EXT_ARENA_H

#ضمن <linux/types.h>

بنية scx_sched;

s32 scx_arena_pool_init(بنية scx_sched *sch);
فراغ scx_arena_pool_destroy(بنية scx_sched *sch);
فراغ *scx_arena_alloc(بنية scx_sched *sch, سائيز_t سائيز);
فراغ scx_arena_free(بنية scx_sched *sch, فراغ *kern_va, سائيز_t سائيز);

#جيڪڏهن_پورو /* _KERNEL_SCHED_EXT_ARENA_H */
