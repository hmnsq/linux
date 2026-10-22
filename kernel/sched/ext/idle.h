/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_EXT_IDLE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_EXT_IDLE_H

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/btf_ids.h>

ਬਣਤਰ cpumask;
ਬਣਤਰ sched_ext_ops;
ਬਣਤਰ task_struct;

ਬਾਹਰੀ ਬਣਤਰ btf_id_set8 scx_kfunc_ids_idle;
ਬਾਹਰੀ ਬਣਤਰ btf_id_set8 scx_kfunc_ids_select_cpu;

ਖਾਲੀ scx_idle_update_selcpu_topology(ਬਣਤਰ sched_ext_ops *ops);
ਖਾਲੀ scx_idle_init_masks(ਖਾਲੀ);

s32 scx_select_cpu_dfl(ਬਣਤਰ task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ਅਚਲ ਬਣਤਰ cpumask *cpus_allowed, u64 flags);
ਖਾਲੀ scx_idle_enable(ਬਣਤਰ sched_ext_ops *ops);
ਖਾਲੀ scx_idle_disable(ਖਾਲੀ);
ਪੂਰਨ scx_idle_init(ਖਾਲੀ);

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_EXT_IDLE_H */
