/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_EXT_IDLE_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_EXT_IDLE_H

#ಅನ್ತರ್ಭಾವ <linux/btf_ids.h>

ರಚನೆ cpumask;
ರಚನೆ sched_ext_ops;
ರಚನೆ task_struct;

ಬಾಹ್ಯ ರಚನೆ btf_id_set8 scx_kfunc_ids_idle;
ಬಾಹ್ಯ ರಚನೆ btf_id_set8 scx_kfunc_ids_select_cpu;

ಶೂನ್ಯ scx_idle_update_selcpu_topology(ರಚನೆ sched_ext_ops *ops);
ಶೂನ್ಯ scx_idle_init_masks(ಶೂನ್ಯ);

s32 scx_select_cpu_dfl(ರಚನೆ task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpus_allowed, u64 flags);
ಶೂನ್ಯ scx_idle_enable(ರಚನೆ sched_ext_ops *ops);
ಶೂನ್ಯ scx_idle_disable(ಶೂನ್ಯ);
ಪೂರ್ಣಾಂಕ scx_idle_init(ಶೂನ್ಯ);

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_EXT_IDLE_H */
