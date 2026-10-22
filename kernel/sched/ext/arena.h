/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#अगर_अघोषित _KERNEL_SCHED_EXT_ARENA_H
#घोषणा _KERNEL_SCHED_EXT_ARENA_H

#समावेश <linux/types.h>

काष्ठा scx_sched;

s32 scx_arena_pool_init(काष्ठा scx_sched *sch);
व्योम scx_arena_pool_destroy(काष्ठा scx_sched *sch);
व्योम *scx_arena_alloc(काष्ठा scx_sched *sch, माप_प्रकार आकार);
व्योम scx_arena_free(काष्ठा scx_sched *sch, व्योम *kern_va, माप_प्रकार आकार);

#पूर्ण_अगर /* _KERNEL_SCHED_EXT_ARENA_H */
