/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#اگر_غیر_تعریف_شدہ _KERNEL_SCHED_EXT_IDLE_H
#تعریف_کرو _KERNEL_SCHED_EXT_IDLE_H

#شامل <linux/btf_ids.h>

ساخت cpumask;
ساخت sched_ext_ops;
ساخت task_struct;

خارجی ساخت btf_id_set8 scx_kfunc_ids_idle;
خارجی ساخت btf_id_set8 scx_kfunc_ids_select_cpu;

خالی scx_idle_update_selcpu_topology(ساخت sched_ext_ops *ops);
خالی scx_idle_init_masks(خالی);

s32 scx_select_cpu_dfl(ساخت task_struct *p, s32 پچھلا_cpu, u64 wake_flags,
		       ثابت ساخت cpumask *cpus_allowed, u64 flags);
خالی scx_idle_enable(ساخت sched_ext_ops *ops);
خالی scx_idle_disable(خالی);
صحیح scx_idle_init(خالی);

#اگر_ختم /* _KERNEL_SCHED_EXT_IDLE_H */
