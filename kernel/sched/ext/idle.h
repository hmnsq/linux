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

#অন্তর্ভাব <linux/btf_ids.h>

কাঠামো cpumask;
কাঠামো sched_ext_ops;
কাঠামো task_struct;

বাহ্যিক কাঠামো btf_id_set8 scx_kfunc_ids_idle;
বাহ্যিক কাঠামো btf_id_set8 scx_kfunc_ids_select_cpu;

শূন্য scx_idle_update_selcpu_topology(কাঠামো sched_ext_ops *ops);
শূন্য scx_idle_init_masks(শূন্য);

s32 scx_select_cpu_dfl(কাঠামো task_struct *p, s32 পূর্ব_cpu, u64 wake_flags,
		       ধ্রুবক কাঠামো cpumask *cpus_allowed, u64 flags);
শূন্য scx_idle_enable(কাঠামো sched_ext_ops *ops);
শূন্য scx_idle_disable(শূন্য);
পূর্ণসংখ্যা scx_idle_init(শূন্য);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
