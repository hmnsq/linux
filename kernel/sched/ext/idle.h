/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#నిర్వచితం_కాకపోతే _KERNEL_SCHED_EXT_IDLE_H
#నిర్వచించు _KERNEL_SCHED_EXT_IDLE_H

#అన్తర్భావ <linux/btf_ids.h>

నిర్మాణం cpumask;
నిర్మాణం sched_ext_ops;
నిర్మాణం task_struct;

బాహ్య నిర్మాణం btf_id_set8 scx_kfunc_ids_idle;
బాహ్య నిర్మాణం btf_id_set8 scx_kfunc_ids_select_cpu;

శూన్య scx_idle_update_selcpu_topology(నిర్మాణం sched_ext_ops *ops);
శూన్య scx_idle_init_masks(శూన్య);

s32 scx_select_cpu_dfl(నిర్మాణం task_struct *p, s32 prev_cpu, u64 wake_flags,
		       స్థిరరాశి నిర్మాణం cpumask *cpus_allowed, u64 flags);
శూన్య scx_idle_enable(నిర్మాణం sched_ext_ops *ops);
శూన్య scx_idle_disable(శూన్య);
పూర్ణాంకం scx_idle_init(శూన్య);

#అయితే_ముగింపు /* _KERNEL_SCHED_EXT_IDLE_H */
