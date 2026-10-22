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

#അന്തര്ഭാവ <linux/btf_ids.h>

ഘടന cpumask;
ഘടന sched_ext_ops;
ഘടന task_struct;

ബാഹ്യ ഘടന btf_id_set8 scx_kfunc_ids_idle;
ബാഹ്യ ഘടന btf_id_set8 scx_kfunc_ids_select_cpu;

ശൂന്യം scx_idle_update_selcpu_topology(ഘടന sched_ext_ops *ops);
ശൂന്യം scx_idle_init_masks(ശൂന്യം);

s32 scx_select_cpu_dfl(ഘടന task_struct *p, s32 prev_cpu, u64 wake_flags,
		       സ്ഥിരം ഘടന cpumask *cpus_allowed, u64 flags);
ശൂന്യം scx_idle_enable(ഘടന sched_ext_ops *ops);
ശൂന്യം scx_idle_disable(ശൂന്യം);
പൂർണ്ണസംഖ്യ scx_idle_init(ശൂന്യം);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
