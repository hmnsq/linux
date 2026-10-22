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

#அந்தர்பாவ <linux/btf_ids.h>

கட்டமைப்பு cpumask;
கட்டமைப்பு sched_ext_ops;
கட்டமைப்பு task_struct;

வெளி கட்டமைப்பு btf_id_set8 scx_kfunc_ids_idle;
வெளி கட்டமைப்பு btf_id_set8 scx_kfunc_ids_select_cpu;

வெறுமை scx_idle_update_selcpu_topology(கட்டமைப்பு sched_ext_ops *ops);
வெறுமை scx_idle_init_masks(வெறுமை);

s32 scx_select_cpu_dfl(கட்டமைப்பு task_struct *p, s32 prev_cpu, u64 wake_flags,
		       மாறிலி கட்டமைப்பு cpumask *cpus_allowed, u64 flags);
வெறுமை scx_idle_enable(கட்டமைப்பு sched_ext_ops *ops);
வெறுமை scx_idle_disable(வெறுமை);
முழு scx_idle_init(வெறுமை);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
