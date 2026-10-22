/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#यदि_अपरिभाषितम् _KERNEL_SCHED_EXT_IDLE_H
#परिभाषय _KERNEL_SCHED_EXT_IDLE_H

#अन्तर्भाव <linux/btf_ids.h>

संरचना cpumask;
संरचना sched_ext_ops;
संरचना task_struct;

बाह्य संरचना btf_id_set8 scx_kfunc_ids_idle;
बाह्य संरचना btf_id_set8 scx_kfunc_ids_select_cpu;

शून्य scx_idle_update_selcpu_topology(संरचना sched_ext_ops *ops);
शून्य scx_idle_init_masks(शून्य);

s32 scx_select_cpu_dfl(संरचना task_struct *p, s32 पूर्व_cpu, u64 wake_flags,
		       ध्रुव संरचना cpumask *cpus_allowed, u64 flags);
शून्य scx_idle_enable(संरचना sched_ext_ops *ops);
शून्य scx_idle_disable(शून्य);
पूर्णाङ्क scx_idle_init(शून्य);

#यदि_अन्तः /* _KERNEL_SCHED_EXT_IDLE_H */
