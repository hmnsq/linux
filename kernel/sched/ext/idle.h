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

#અન્તર્ભાવ <linux/btf_ids.h>

સંરચના cpumask;
સંરચના sched_ext_ops;
સંરચના task_struct;

બાહ્ય સંરચના btf_id_set8 scx_kfunc_ids_idle;
બાહ્ય સંરચના btf_id_set8 scx_kfunc_ids_select_cpu;

શૂન્ય scx_idle_update_selcpu_topology(સંરચના sched_ext_ops *ops);
શૂન્ય scx_idle_init_masks(શૂન્ય);

s32 scx_select_cpu_dfl(સંરચના task_struct *p, s32 prev_cpu, u64 wake_flags,
		       અચલ સંરચના cpumask *cpus_allowed, u64 flags);
શૂન્ય scx_idle_enable(સંરચના sched_ext_ops *ops);
શૂન્ય scx_idle_disable(શૂન્ય);
પૂર્ણાંક scx_idle_init(શૂન્ય);

#endif /* _KERNEL_SCHED_EXT_IDLE_H */
