/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_EXT_ARENA_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_EXT_ARENA_H

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/types.h>

ਬਣਤਰ scx_sched;

s32 scx_arena_pool_init(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ scx_arena_pool_destroy(ਬਣਤਰ scx_sched *sch);
ਖਾਲੀ *scx_arena_alloc(ਬਣਤਰ scx_sched *sch, ਮਾਪ_t ਮਾਪ);
ਖਾਲੀ scx_arena_free(ਬਣਤਰ scx_sched *sch, ਖਾਲੀ *kern_va, ਮਾਪ_t ਮਾਪ);

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_EXT_ARENA_H */
