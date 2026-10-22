/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#جے_بنا_تعریف _KERNEL_SCHED_EXT_IDLE_H
#تعریف_کرو _KERNEL_SCHED_EXT_IDLE_H

#ضمن <linux/btf_ids.h>

بنية cpumask;
بنية sched_ext_ops;
بنية task_struct;

خارجي بنية btf_id_set8 scx_kfunc_ids_idle;
خارجي بنية btf_id_set8 scx_kfunc_ids_select_cpu;

فراغ scx_idle_update_selcpu_topology(بنية sched_ext_ops *ops);
فراغ scx_idle_init_masks(فراغ);

s32 scx_select_cpu_dfl(بنية task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ثابت بنية cpumask *cpus_allowed, u64 flags);
فراغ scx_idle_enable(بنية sched_ext_ops *ops);
فراغ scx_idle_disable(فراغ);
صحيح scx_idle_init(فراغ);

#جے_انت /* _KERNEL_SCHED_EXT_IDLE_H */
