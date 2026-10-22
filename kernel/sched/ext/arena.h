/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2025 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2025 Tejun Heo <tj@kernel.org>
 */
#यदि_अपरिभाषितम् _KERNEL_SCHED_EXT_ARENA_H
#परिभाषय _KERNEL_SCHED_EXT_ARENA_H

#अन्तर्भाव <linux/types.h>

संरचना scx_sched;

s32 scx_arena_pool_init(संरचना scx_sched *sch);
शून्य scx_arena_pool_destroy(संरचना scx_sched *sch);
शून्य *scx_arena_alloc(संरचना scx_sched *sch, आकार_t आकार);
शून्य scx_arena_free(संरचना scx_sched *sch, शून्य *kern_va, आकार_t आकार);

#यदि_अन्तः /* _KERNEL_SCHED_EXT_ARENA_H */
