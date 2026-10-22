/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

શૂન્ય scx_tick(સંરચના rq *rq);
શૂન્ય init_scx_entity(સંરચના sched_ext_entity *scx);
શૂન્ય scx_pre_fork(સંરચના task_struct *p);
પૂર્ણાંક scx_fork(સંરચના task_struct *p, સંરચના kernel_clone_args *kargs);
શૂન્ય scx_post_fork(સંરચના task_struct *p);
શૂન્ય scx_cancel_fork(સંરચના task_struct *p);
bool scx_can_stop_tick(સંરચના rq *rq);
શૂન્ય scx_rq_activate(સંરચના rq *rq);
શૂન્ય scx_rq_deactivate(સંરચના rq *rq);
પૂર્ણાંક scx_check_setscheduler(સંરચના task_struct *p, પૂર્ણાંક policy);
bool task_should_scx(પૂર્ણાંક policy);
bool scx_allow_ttwu_queue(અચલ સંરચના task_struct *p);
bool scx_allow_proxy_exec(અચલ સંરચના task_struct *p);
શૂન્ય scx_proxy_donor_start(સંરચના rq *rq);
શૂન્ય scx_proxy_reenqueue_retry(સંરચના rq *rq, સંરચના task_struct *next);
શૂન્ય init_sched_ext_class(શૂન્ય);

સ્થિર અંતઃરેખ u32 scx_cpuperf_target(s32 cpu)
{
	જો (scx_enabled())
		પાછા cpu_rq(cpu)->scx.cpuperf_target;
	નહીંતર
		પાછા 0;
}

સ્થિર અંતઃરેખ bool task_on_scx(અચલ સંરચના task_struct *p)
{
	પાછા scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(અચલ સંરચના task_struct *a, અચલ સંરચના task_struct *b,
		   bool in_fi);
#endif

#નહીંતર	/* CONFIG_SCHED_CLASS_EXT */

સ્થિર અંતઃરેખ શૂન્ય scx_tick(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_pre_fork(સંરચના task_struct *p) {}
સ્થિર અંતઃરેખ પૂર્ણાંક scx_fork(સંરચના task_struct *p, સંરચના kernel_clone_args *kargs) { પાછા 0; }
સ્થિર અંતઃરેખ શૂન્ય scx_post_fork(સંરચના task_struct *p) {}
સ્થિર અંતઃરેખ શૂન્ય scx_cancel_fork(સંરચના task_struct *p) {}
સ્થિર અંતઃરેખ u32 scx_cpuperf_target(s32 cpu) { પાછા 0; }
સ્થિર અંતઃરેખ bool scx_can_stop_tick(સંરચના rq *rq) { પાછા true; }
સ્થિર અંતઃરેખ શૂન્ય scx_rq_activate(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_rq_deactivate(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ પૂર્ણાંક scx_check_setscheduler(સંરચના task_struct *p, પૂર્ણાંક policy) { પાછા 0; }
સ્થિર અંતઃરેખ bool task_on_scx(અચલ સંરચના task_struct *p) { પાછા false; }
સ્થિર અંતઃરેખ bool scx_allow_ttwu_queue(અચલ સંરચના task_struct *p) { પાછા true; }
સ્થિર અંતઃરેખ bool scx_allow_proxy_exec(અચલ સંરચના task_struct *p) { પાછા true; }
સ્થિર અંતઃરેખ શૂન્ય scx_proxy_donor_start(સંરચના rq *rq) {}
સ્થિર અંતઃરેખ શૂન્ય scx_proxy_reenqueue_retry(સંરચના rq *rq,
					     સંરચના task_struct *next) {}
સ્થિર અંતઃરેખ શૂન્ય init_sched_ext_class(શૂન્ય) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
શૂન્ય __scx_update_idle(સંરચના rq *rq, bool idle, bool do_notify);

સ્થિર અંતઃરેખ શૂન્ય scx_update_idle(સંરચના rq *rq, bool idle, bool do_notify)
{
	જો (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#નહીંતર
સ્થિર અંતઃરેખ શૂન્ય scx_update_idle(સંરચના rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
શૂન્ય scx_tg_init(સંરચના task_group *tg);
પૂર્ણાંક scx_tg_online(સંરચના task_group *tg);
શૂન્ય scx_tg_offline(સંરચના task_group *tg);
પૂર્ણાંક scx_cgroup_can_attach(સંરચના cgroup_taskset *tset);
શૂન્ય scx_cgroup_move_task(સંરચના task_struct *p);
શૂન્ય scx_cgroup_cancel_attach(સંરચના cgroup_taskset *tset);
શૂન્ય scx_group_set_weight(સંરચના task_group *tg, unsigned long cgrp_weight);
શૂન્ય scx_group_set_idle(સંરચના task_group *tg, bool idle);
શૂન્ય scx_group_set_bandwidth(સંરચના task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#નહીંતર	/* CONFIG_EXT_GROUP_SCHED */
સ્થિર અંતઃરેખ શૂન્ય scx_tg_init(સંરચના task_group *tg) {}
સ્થિર અંતઃરેખ પૂર્ણાંક scx_tg_online(સંરચના task_group *tg) { પાછા 0; }
સ્થિર અંતઃરેખ શૂન્ય scx_tg_offline(સંરચના task_group *tg) {}
સ્થિર અંતઃરેખ પૂર્ણાંક scx_cgroup_can_attach(સંરચના cgroup_taskset *tset) { પાછા 0; }
સ્થિર અંતઃરેખ શૂન્ય scx_cgroup_move_task(સંરચના task_struct *p) {}
સ્થિર અંતઃરેખ શૂન્ય scx_cgroup_cancel_attach(સંરચના cgroup_taskset *tset) {}
સ્થિર અંતઃરેખ શૂન્ય scx_group_set_weight(સંરચના task_group *tg, unsigned long cgrp_weight) {}
સ્થિર અંતઃરેખ શૂન્ય scx_group_set_idle(સંરચના task_group *tg, bool idle) {}
સ્થિર અંતઃરેખ શૂન્ય scx_group_set_bandwidth(સંરચના task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
