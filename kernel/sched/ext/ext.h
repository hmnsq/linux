/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

শূন্য scx_tick(সংরচনা rq *rq);
শূন্য init_scx_entity(সংরচনা sched_ext_entity *scx);
শূন্য scx_pre_fork(সংরচনা task_struct *p);
পূর্ণাঙ্ক scx_fork(সংরচনা task_struct *p, সংরচনা kernel_clone_args *kargs);
শূন্য scx_post_fork(সংরচনা task_struct *p);
শূন্য scx_cancel_fork(সংরচনা task_struct *p);
bool scx_can_stop_tick(সংরচনা rq *rq);
শূন্য scx_rq_activate(সংরচনা rq *rq);
শূন্য scx_rq_deactivate(সংরচনা rq *rq);
পূর্ণাঙ্ক scx_check_setscheduler(সংরচনা task_struct *p, পূর্ণাঙ্ক policy);
bool task_should_scx(পূর্ণাঙ্ক policy);
bool scx_allow_ttwu_queue(ধ্রুব সংরচনা task_struct *p);
bool scx_allow_proxy_exec(ধ্রুব সংরচনা task_struct *p);
শূন্য scx_proxy_donor_start(সংরচনা rq *rq);
শূন্য scx_proxy_reenqueue_retry(সংরচনা rq *rq, সংরচনা task_struct *next);
শূন্য init_sched_ext_class(শূন্য);

স্থির অন্তঃস্থ u32 scx_cpuperf_target(s32 cpu)
{
	যদি (scx_enabled())
		প্রত্যাগম cpu_rq(cpu)->scx.cpuperf_target;
	অন্যথা
		প্রত্যাগম 0;
}

স্থির অন্তঃস্থ bool task_on_scx(ধ্রুব সংরচনা task_struct *p)
{
	প্রত্যাগম scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(ধ্রুব সংরচনা task_struct *a, ধ্রুব সংরচনা task_struct *b,
		   bool in_fi);
#endif

#অন্যথা	/* CONFIG_SCHED_CLASS_EXT */

স্থির অন্তঃস্থ শূন্য scx_tick(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_pre_fork(সংরচনা task_struct *p) {}
স্থির অন্তঃস্থ পূর্ণাঙ্ক scx_fork(সংরচনা task_struct *p, সংরচনা kernel_clone_args *kargs) { প্রত্যাগম 0; }
স্থির অন্তঃস্থ শূন্য scx_post_fork(সংরচনা task_struct *p) {}
স্থির অন্তঃস্থ শূন্য scx_cancel_fork(সংরচনা task_struct *p) {}
স্থির অন্তঃস্থ u32 scx_cpuperf_target(s32 cpu) { প্রত্যাগম 0; }
স্থির অন্তঃস্থ bool scx_can_stop_tick(সংরচনা rq *rq) { প্রত্যাগম true; }
স্থির অন্তঃস্থ শূন্য scx_rq_activate(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_rq_deactivate(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ পূর্ণাঙ্ক scx_check_setscheduler(সংরচনা task_struct *p, পূর্ণাঙ্ক policy) { প্রত্যাগম 0; }
স্থির অন্তঃস্থ bool task_on_scx(ধ্রুব সংরচনা task_struct *p) { প্রত্যাগম false; }
স্থির অন্তঃস্থ bool scx_allow_ttwu_queue(ধ্রুব সংরচনা task_struct *p) { প্রত্যাগম true; }
স্থির অন্তঃস্থ bool scx_allow_proxy_exec(ধ্রুব সংরচনা task_struct *p) { প্রত্যাগম true; }
স্থির অন্তঃস্থ শূন্য scx_proxy_donor_start(সংরচনা rq *rq) {}
স্থির অন্তঃস্থ শূন্য scx_proxy_reenqueue_retry(সংরচনা rq *rq,
					     সংরচনা task_struct *next) {}
স্থির অন্তঃস্থ শূন্য init_sched_ext_class(শূন্য) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
শূন্য __scx_update_idle(সংরচনা rq *rq, bool idle, bool do_notify);

স্থির অন্তঃস্থ শূন্য scx_update_idle(সংরচনা rq *rq, bool idle, bool do_notify)
{
	যদি (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#অন্যথা
স্থির অন্তঃস্থ শূন্য scx_update_idle(সংরচনা rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
শূন্য scx_tg_init(সংরচনা task_group *tg);
পূর্ণাঙ্ক scx_tg_online(সংরচনা task_group *tg);
শূন্য scx_tg_offline(সংরচনা task_group *tg);
পূর্ণাঙ্ক scx_cgroup_can_attach(সংরচনা cgroup_taskset *tset);
শূন্য scx_cgroup_move_task(সংরচনা task_struct *p);
শূন্য scx_cgroup_cancel_attach(সংরচনা cgroup_taskset *tset);
শূন্য scx_group_set_weight(সংরচনা task_group *tg, unsigned long cgrp_weight);
শূন্য scx_group_set_idle(সংরচনা task_group *tg, bool idle);
শূন্য scx_group_set_bandwidth(সংরচনা task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#অন্যথা	/* CONFIG_EXT_GROUP_SCHED */
স্থির অন্তঃস্থ শূন্য scx_tg_init(সংরচনা task_group *tg) {}
স্থির অন্তঃস্থ পূর্ণাঙ্ক scx_tg_online(সংরচনা task_group *tg) { প্রত্যাগম 0; }
স্থির অন্তঃস্থ শূন্য scx_tg_offline(সংরচনা task_group *tg) {}
স্থির অন্তঃস্থ পূর্ণাঙ্ক scx_cgroup_can_attach(সংরচনা cgroup_taskset *tset) { প্রত্যাগম 0; }
স্থির অন্তঃস্থ শূন্য scx_cgroup_move_task(সংরচনা task_struct *p) {}
স্থির অন্তঃস্থ শূন্য scx_cgroup_cancel_attach(সংরচনা cgroup_taskset *tset) {}
স্থির অন্তঃস্থ শূন্য scx_group_set_weight(সংরচনা task_group *tg, unsigned long cgrp_weight) {}
স্থির অন্তঃস্থ শূন্য scx_group_set_idle(সংরচনা task_group *tg, bool idle) {}
স্থির অন্তঃস্থ শূন্য scx_group_set_bandwidth(সংরচনা task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
