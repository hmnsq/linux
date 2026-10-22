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

#include <linux/btf_ids.h>

sa_nracanaa cpumask;
sa_nracanaa sched_ext_ops;
sa_nracanaa task_struct;

extern sa_nracanaa btf_id_set8 scx_kfunc_ids_idle;
extern sa_nracanaa btf_id_set8 scx_kfunc_ids_select_cpu;

void scx_idle_update_selcpu_topology(sa_nracanaa sched_ext_ops *ops);
void scx_idle_init_masks(void);

s32 scx_select_cpu_dfl(sa_nracanaa task_struct *p, s32 prev_cpu, u64 wake_flags,
		       const sa_nracanaa cpumask *cpus_allowed, u64 flags);
void scx_idle_enable(sa_nracanaa sched_ext_ops *ops);
void scx_idle_disable(void);
ကိန်းပြည့် scx_idle_init(void);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
