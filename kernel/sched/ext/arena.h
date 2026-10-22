/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_EXT_ARENA_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_EXT_ARENA_H

#ಅನ್ತರ್ಭಾವ <linux/types.h>

ರಚನೆ scx_sched;

s32 scx_arena_pool_init(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ scx_arena_pool_destroy(ರಚನೆ scx_sched *sch);
ಶೂನ್ಯ *scx_arena_alloc(ರಚನೆ scx_sched *sch, ಗಾತ್ರ_t ಗಾತ್ರ_2);
ಶೂನ್ಯ scx_arena_free(ರಚನೆ scx_sched *sch, ಶೂನ್ಯ *kern_va, ಗಾತ್ರ_t ಗಾತ್ರ_2);

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_EXT_ARENA_H */
