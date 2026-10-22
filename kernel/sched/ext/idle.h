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

#ಅನ್ತರ್ಭಾವ <linux/btf_ids.h>

ಸಂರಚನಾ cpumask;
ಸಂರಚನಾ sched_ext_ops;
ಸಂರಚನಾ task_struct;

ಬಾಹ್ಯ ಸಂರಚನಾ btf_id_set8 scx_kfunc_ids_idle;
ಬಾಹ್ಯ ಸಂರಚನಾ btf_id_set8 scx_kfunc_ids_select_cpu;

ಶೂನ್ಯ scx_idle_update_selcpu_topology(ಸಂರಚನಾ sched_ext_ops *ops);
ಶೂನ್ಯ scx_idle_init_masks(ಶೂನ್ಯ);

s32 scx_select_cpu_dfl(ಸಂರಚನಾ task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ಧ್ರುವ ಸಂರಚನಾ cpumask *cpus_allowed, u64 flags);
ಶೂನ್ಯ scx_idle_enable(ಸಂರಚನಾ sched_ext_ops *ops);
ಶೂನ್ಯ scx_idle_disable(ಶೂನ್ಯ);
ಪೂರ್ಣಾಙ್ಕ scx_idle_init(ಶೂನ್ಯ);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
