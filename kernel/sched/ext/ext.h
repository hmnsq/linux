/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

வெறுமை scx_tick(கட்டமைப்பு rq *rq);
வெறுமை init_scx_entity(கட்டமைப்பு sched_ext_entity *scx);
வெறுமை scx_pre_fork(கட்டமைப்பு task_struct *p);
முழு scx_fork(கட்டமைப்பு task_struct *p, கட்டமைப்பு kernel_clone_args *kargs);
வெறுமை scx_post_fork(கட்டமைப்பு task_struct *p);
வெறுமை scx_cancel_fork(கட்டமைப்பு task_struct *p);
bool scx_can_stop_tick(கட்டமைப்பு rq *rq);
வெறுமை scx_rq_activate(கட்டமைப்பு rq *rq);
வெறுமை scx_rq_deactivate(கட்டமைப்பு rq *rq);
முழு scx_check_setscheduler(கட்டமைப்பு task_struct *p, முழு policy);
bool task_should_scx(முழு policy);
bool scx_allow_ttwu_queue(மாறிலி கட்டமைப்பு task_struct *p);
bool scx_allow_proxy_exec(மாறிலி கட்டமைப்பு task_struct *p);
வெறுமை scx_proxy_donor_start(கட்டமைப்பு rq *rq);
வெறுமை scx_proxy_reenqueue_retry(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *next);
வெறுமை init_sched_ext_class(வெறுமை);

நிலையான உள்வரி u32 scx_cpuperf_target(s32 cpu)
{
	எனில் (scx_enabled())
		திரும்பு cpu_rq(cpu)->scx.cpuperf_target;
	இல்லையெனில்
		திரும்பு 0;
}

நிலையான உள்வரி bool task_on_scx(மாறிலி கட்டமைப்பு task_struct *p)
{
	திரும்பு scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(மாறிலி கட்டமைப்பு task_struct *a, மாறிலி கட்டமைப்பு task_struct *b,
		   bool in_fi);
#endif

#இல்லையெனில்	/* CONFIG_SCHED_CLASS_EXT */

நிலையான உள்வரி வெறுமை scx_tick(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_pre_fork(கட்டமைப்பு task_struct *p) {}
நிலையான உள்வரி முழு scx_fork(கட்டமைப்பு task_struct *p, கட்டமைப்பு kernel_clone_args *kargs) { திரும்பு 0; }
நிலையான உள்வரி வெறுமை scx_post_fork(கட்டமைப்பு task_struct *p) {}
நிலையான உள்வரி வெறுமை scx_cancel_fork(கட்டமைப்பு task_struct *p) {}
நிலையான உள்வரி u32 scx_cpuperf_target(s32 cpu) { திரும்பு 0; }
நிலையான உள்வரி bool scx_can_stop_tick(கட்டமைப்பு rq *rq) { திரும்பு true; }
நிலையான உள்வரி வெறுமை scx_rq_activate(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_rq_deactivate(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி முழு scx_check_setscheduler(கட்டமைப்பு task_struct *p, முழு policy) { திரும்பு 0; }
நிலையான உள்வரி bool task_on_scx(மாறிலி கட்டமைப்பு task_struct *p) { திரும்பு false; }
நிலையான உள்வரி bool scx_allow_ttwu_queue(மாறிலி கட்டமைப்பு task_struct *p) { திரும்பு true; }
நிலையான உள்வரி bool scx_allow_proxy_exec(மாறிலி கட்டமைப்பு task_struct *p) { திரும்பு true; }
நிலையான உள்வரி வெறுமை scx_proxy_donor_start(கட்டமைப்பு rq *rq) {}
நிலையான உள்வரி வெறுமை scx_proxy_reenqueue_retry(கட்டமைப்பு rq *rq,
					     கட்டமைப்பு task_struct *next) {}
நிலையான உள்வரி வெறுமை init_sched_ext_class(வெறுமை) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
வெறுமை __scx_update_idle(கட்டமைப்பு rq *rq, bool idle, bool do_notify);

நிலையான உள்வரி வெறுமை scx_update_idle(கட்டமைப்பு rq *rq, bool idle, bool do_notify)
{
	எனில் (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#இல்லையெனில்
நிலையான உள்வரி வெறுமை scx_update_idle(கட்டமைப்பு rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
வெறுமை scx_tg_init(கட்டமைப்பு task_group *tg);
முழு scx_tg_online(கட்டமைப்பு task_group *tg);
வெறுமை scx_tg_offline(கட்டமைப்பு task_group *tg);
முழு scx_cgroup_can_attach(கட்டமைப்பு cgroup_taskset *tset);
வெறுமை scx_cgroup_move_task(கட்டமைப்பு task_struct *p);
வெறுமை scx_cgroup_cancel_attach(கட்டமைப்பு cgroup_taskset *tset);
வெறுமை scx_group_set_weight(கட்டமைப்பு task_group *tg, unsigned long cgrp_weight);
வெறுமை scx_group_set_idle(கட்டமைப்பு task_group *tg, bool idle);
வெறுமை scx_group_set_bandwidth(கட்டமைப்பு task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#இல்லையெனில்	/* CONFIG_EXT_GROUP_SCHED */
நிலையான உள்வரி வெறுமை scx_tg_init(கட்டமைப்பு task_group *tg) {}
நிலையான உள்வரி முழு scx_tg_online(கட்டமைப்பு task_group *tg) { திரும்பு 0; }
நிலையான உள்வரி வெறுமை scx_tg_offline(கட்டமைப்பு task_group *tg) {}
நிலையான உள்வரி முழு scx_cgroup_can_attach(கட்டமைப்பு cgroup_taskset *tset) { திரும்பு 0; }
நிலையான உள்வரி வெறுமை scx_cgroup_move_task(கட்டமைப்பு task_struct *p) {}
நிலையான உள்வரி வெறுமை scx_cgroup_cancel_attach(கட்டமைப்பு cgroup_taskset *tset) {}
நிலையான உள்வரி வெறுமை scx_group_set_weight(கட்டமைப்பு task_group *tg, unsigned long cgrp_weight) {}
நிலையான உள்வரி வெறுமை scx_group_set_idle(கட்டமைப்பு task_group *tg, bool idle) {}
நிலையான உள்வரி வெறுமை scx_group_set_bandwidth(கட்டமைப்பு task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
