/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 * Copyright (c) 2024 Andrea Righi <arighi@nvidia.com>
 */
#अगर_अघोषित _KERNEL_SCHED_EXT_IDLE_H
#घोषणा _KERNEL_SCHED_EXT_IDLE_H

#समावेश <linux/btf_ids.h>

काष्ठा cpumask;
काष्ठा sched_ext_ops;
काष्ठा task_struct;

बाह्य काष्ठा btf_id_set8 scx_kfunc_ids_idle;
बाह्य काष्ठा btf_id_set8 scx_kfunc_ids_select_cpu;

व्योम scx_idle_update_selcpu_topology(काष्ठा sched_ext_ops *ops);
व्योम scx_idle_init_masks(व्योम);

s32 scx_select_cpu_dfl(काष्ठा task_struct *p, s32 पूर्व_cpu, u64 wake_flags,
		       स्थिर काष्ठा cpumask *cpus_allowed, u64 flags);
व्योम scx_idle_enable(काष्ठा sched_ext_ops *ops);
व्योम scx_idle_disable(व्योम);
पूर्णांक scx_idle_init(व्योम);

#पूर्ण_अगर /* _KERNEL_SCHED_EXT_IDLE_H */
