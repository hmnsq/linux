/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#ifndef _KERNEL_SCHED_EXT_IDLE_H
#define _KERNEL_SCHED_EXT_IDLE_H

#شامل <linux/btf_ids.h>

ساختار cpumask;
ساختار sched_ext_ops;
ساختار task_struct;

بیرونی ساختار btf_id_set8 scx_kfunc_ids_idle;
بیرونی ساختار btf_id_set8 scx_kfunc_ids_select_cpu;

تهی scx_idle_update_selcpu_topology(ساختار sched_ext_ops *ops);
تهی scx_idle_init_masks(تهی);

s32 scx_select_cpu_dfl(ساختار task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ثابت ساختار cpumask *cpus_allowed, u64 flags);
تهی scx_idle_enable(ساختار sched_ext_ops *ops);
تهی scx_idle_disable(تهی);
صحیح scx_idle_init(تهی);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
