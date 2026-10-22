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

#ଅନ୍ତର୍ଭାଵ <linux/btf_ids.h>

ସଂରଚନା cpumask;
ସଂରଚନା sched_ext_ops;
ସଂରଚନା task_struct;

ବାହ୍ୟ ସଂରଚନା btf_id_set8 scx_kfunc_ids_idle;
ବାହ୍ୟ ସଂରଚନା btf_id_set8 scx_kfunc_ids_select_cpu;

ଶୂନ୍ୟ scx_idle_update_selcpu_topology(ସଂରଚନା sched_ext_ops *ops);
ଶୂନ୍ୟ scx_idle_init_masks(ଶୂନ୍ୟ);

s32 scx_select_cpu_dfl(ସଂରଚନା task_struct *p, s32 prev_cpu, u64 wake_flags,
		       ଅଚଳ ସଂରଚନା cpumask *cpus_allowed, u64 flags);
ଶୂନ୍ୟ scx_idle_enable(ସଂରଚନା sched_ext_ops *ops);
ଶୂନ୍ୟ scx_idle_disable(ଶୂନ୍ୟ);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_idle_init(ଶୂନ୍ୟ);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
