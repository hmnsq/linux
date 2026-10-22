/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

تهی scx_tick(ساختار rq *rq);
تهی init_scx_entity(ساختار sched_ext_entity *scx);
تهی scx_pre_fork(ساختار task_struct *p);
صحیح scx_fork(ساختار task_struct *p, ساختار kernel_clone_args *kargs);
تهی scx_post_fork(ساختار task_struct *p);
تهی scx_cancel_fork(ساختار task_struct *p);
bool scx_can_stop_tick(ساختار rq *rq);
تهی scx_rq_activate(ساختار rq *rq);
تهی scx_rq_deactivate(ساختار rq *rq);
صحیح scx_check_setscheduler(ساختار task_struct *p, صحیح policy);
bool task_should_scx(صحیح policy);
bool scx_allow_ttwu_queue(ثابت ساختار task_struct *p);
bool scx_allow_proxy_exec(ثابت ساختار task_struct *p);
تهی scx_proxy_donor_start(ساختار rq *rq);
تهی scx_proxy_reenqueue_retry(ساختار rq *rq, ساختار task_struct *next);
تهی init_sched_ext_class(تهی);

ایستا درونخطی u32 scx_cpuperf_target(s32 cpu)
{
	اگر (scx_enabled())
		بازگشت cpu_rq(cpu)->scx.cpuperf_target;
	وگرنه
		بازگشت 0;
}

ایستا درونخطی bool task_on_scx(ثابت ساختار task_struct *p)
{
	بازگشت scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(ثابت ساختار task_struct *a, ثابت ساختار task_struct *b,
		   bool in_fi);
#endif

#وگرنه	/* CONFIG_SCHED_CLASS_EXT */

ایستا درونخطی تهی scx_tick(ساختار rq *rq) {}
ایستا درونخطی تهی scx_pre_fork(ساختار task_struct *p) {}
ایستا درونخطی صحیح scx_fork(ساختار task_struct *p, ساختار kernel_clone_args *kargs) { بازگشت 0; }
ایستا درونخطی تهی scx_post_fork(ساختار task_struct *p) {}
ایستا درونخطی تهی scx_cancel_fork(ساختار task_struct *p) {}
ایستا درونخطی u32 scx_cpuperf_target(s32 cpu) { بازگشت 0; }
ایستا درونخطی bool scx_can_stop_tick(ساختار rq *rq) { بازگشت true; }
ایستا درونخطی تهی scx_rq_activate(ساختار rq *rq) {}
ایستا درونخطی تهی scx_rq_deactivate(ساختار rq *rq) {}
ایستا درونخطی صحیح scx_check_setscheduler(ساختار task_struct *p, صحیح policy) { بازگشت 0; }
ایستا درونخطی bool task_on_scx(ثابت ساختار task_struct *p) { بازگشت false; }
ایستا درونخطی bool scx_allow_ttwu_queue(ثابت ساختار task_struct *p) { بازگشت true; }
ایستا درونخطی bool scx_allow_proxy_exec(ثابت ساختار task_struct *p) { بازگشت true; }
ایستا درونخطی تهی scx_proxy_donor_start(ساختار rq *rq) {}
ایستا درونخطی تهی scx_proxy_reenqueue_retry(ساختار rq *rq,
					     ساختار task_struct *next) {}
ایستا درونخطی تهی init_sched_ext_class(تهی) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
تهی __scx_update_idle(ساختار rq *rq, bool idle, bool do_notify);

ایستا درونخطی تهی scx_update_idle(ساختار rq *rq, bool idle, bool do_notify)
{
	اگر (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#وگرنه
ایستا درونخطی تهی scx_update_idle(ساختار rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
تهی scx_tg_init(ساختار task_group *tg);
صحیح scx_tg_online(ساختار task_group *tg);
تهی scx_tg_offline(ساختار task_group *tg);
صحیح scx_cgroup_can_attach(ساختار cgroup_taskset *tset);
تهی scx_cgroup_move_task(ساختار task_struct *p);
تهی scx_cgroup_cancel_attach(ساختار cgroup_taskset *tset);
تهی scx_group_set_weight(ساختار task_group *tg, unsigned long cgrp_weight);
تهی scx_group_set_idle(ساختار task_group *tg, bool idle);
تهی scx_group_set_bandwidth(ساختار task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#وگرنه	/* CONFIG_EXT_GROUP_SCHED */
ایستا درونخطی تهی scx_tg_init(ساختار task_group *tg) {}
ایستا درونخطی صحیح scx_tg_online(ساختار task_group *tg) { بازگشت 0; }
ایستا درونخطی تهی scx_tg_offline(ساختار task_group *tg) {}
ایستا درونخطی صحیح scx_cgroup_can_attach(ساختار cgroup_taskset *tset) { بازگشت 0; }
ایستا درونخطی تهی scx_cgroup_move_task(ساختار task_struct *p) {}
ایستا درونخطی تهی scx_cgroup_cancel_attach(ساختار cgroup_taskset *tset) {}
ایستا درونخطی تهی scx_group_set_weight(ساختار task_group *tg, unsigned long cgrp_weight) {}
ایستا درونخطی تهی scx_group_set_idle(ساختار task_group *tg, bool idle) {}
ایستا درونخطی تهی scx_group_set_bandwidth(ساختار task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
