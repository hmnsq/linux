/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

void scx_tick(sa_nracanaa rq *rq);
void init_scx_entity(sa_nracanaa sched_ext_entity *scx);
void scx_pre_fork(sa_nracanaa task_struct *p);
ཧྲིལ scx_fork(sa_nracanaa task_struct *p, sa_nracanaa kernel_clone_args *kargs);
void scx_post_fork(sa_nracanaa task_struct *p);
void scx_cancel_fork(sa_nracanaa task_struct *p);
bool scx_can_stop_tick(sa_nracanaa rq *rq);
void scx_rq_activate(sa_nracanaa rq *rq);
void scx_rq_deactivate(sa_nracanaa rq *rq);
ཧྲིལ scx_check_setscheduler(sa_nracanaa task_struct *p, ཧྲིལ policy);
bool task_should_scx(ཧྲིལ policy);
bool scx_allow_ttwu_queue(const sa_nracanaa task_struct *p);
bool scx_allow_proxy_exec(const sa_nracanaa task_struct *p);
void scx_proxy_donor_start(sa_nracanaa rq *rq);
void scx_proxy_reenqueue_retry(sa_nracanaa rq *rq, sa_nracanaa task_struct *next);
void init_sched_ext_class(void);

static anta_hstha u32 scx_cpuperf_target(s32 cpu)
{
	གལ་ཏེ (scx_enabled())
		ཕྱིར cpu_rq(cpu)->scx.cpuperf_target;
	དེ་མིན
		ཕྱིར 0;
}

static anta_hstha bool task_on_scx(const sa_nracanaa task_struct *p)
{
	ཕྱིར scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(const sa_nracanaa task_struct *a, const sa_nracanaa task_struct *b,
		   bool in_fi);
#endif

#དེ་མིན	/* CONFIG_SCHED_CLASS_EXT */

static anta_hstha void scx_tick(sa_nracanaa rq *rq) {}
static anta_hstha void scx_pre_fork(sa_nracanaa task_struct *p) {}
static anta_hstha ཧྲིལ scx_fork(sa_nracanaa task_struct *p, sa_nracanaa kernel_clone_args *kargs) { ཕྱིར 0; }
static anta_hstha void scx_post_fork(sa_nracanaa task_struct *p) {}
static anta_hstha void scx_cancel_fork(sa_nracanaa task_struct *p) {}
static anta_hstha u32 scx_cpuperf_target(s32 cpu) { ཕྱིར 0; }
static anta_hstha bool scx_can_stop_tick(sa_nracanaa rq *rq) { ཕྱིར true; }
static anta_hstha void scx_rq_activate(sa_nracanaa rq *rq) {}
static anta_hstha void scx_rq_deactivate(sa_nracanaa rq *rq) {}
static anta_hstha ཧྲིལ scx_check_setscheduler(sa_nracanaa task_struct *p, ཧྲིལ policy) { ཕྱིར 0; }
static anta_hstha bool task_on_scx(const sa_nracanaa task_struct *p) { ཕྱིར false; }
static anta_hstha bool scx_allow_ttwu_queue(const sa_nracanaa task_struct *p) { ཕྱིར true; }
static anta_hstha bool scx_allow_proxy_exec(const sa_nracanaa task_struct *p) { ཕྱིར true; }
static anta_hstha void scx_proxy_donor_start(sa_nracanaa rq *rq) {}
static anta_hstha void scx_proxy_reenqueue_retry(sa_nracanaa rq *rq,
					     sa_nracanaa task_struct *next) {}
static anta_hstha void init_sched_ext_class(void) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
void __scx_update_idle(sa_nracanaa rq *rq, bool idle, bool do_notify);

static anta_hstha void scx_update_idle(sa_nracanaa rq *rq, bool idle, bool do_notify)
{
	གལ་ཏེ (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#དེ་མིན
static anta_hstha void scx_update_idle(sa_nracanaa rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
void scx_tg_init(sa_nracanaa task_group *tg);
ཧྲིལ scx_tg_online(sa_nracanaa task_group *tg);
void scx_tg_offline(sa_nracanaa task_group *tg);
ཧྲིལ scx_cgroup_can_attach(sa_nracanaa cgroup_taskset *tset);
void scx_cgroup_move_task(sa_nracanaa task_struct *p);
void scx_cgroup_cancel_attach(sa_nracanaa cgroup_taskset *tset);
void scx_group_set_weight(sa_nracanaa task_group *tg, unsigned long cgrp_weight);
void scx_group_set_idle(sa_nracanaa task_group *tg, bool idle);
void scx_group_set_bandwidth(sa_nracanaa task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#དེ་མིན	/* CONFIG_EXT_GROUP_SCHED */
static anta_hstha void scx_tg_init(sa_nracanaa task_group *tg) {}
static anta_hstha ཧྲིལ scx_tg_online(sa_nracanaa task_group *tg) { ཕྱིར 0; }
static anta_hstha void scx_tg_offline(sa_nracanaa task_group *tg) {}
static anta_hstha ཧྲིལ scx_cgroup_can_attach(sa_nracanaa cgroup_taskset *tset) { ཕྱིར 0; }
static anta_hstha void scx_cgroup_move_task(sa_nracanaa task_struct *p) {}
static anta_hstha void scx_cgroup_cancel_attach(sa_nracanaa cgroup_taskset *tset) {}
static anta_hstha void scx_group_set_weight(sa_nracanaa task_group *tg, unsigned long cgrp_weight) {}
static anta_hstha void scx_group_set_idle(sa_nracanaa task_group *tg, bool idle) {}
static anta_hstha void scx_group_set_bandwidth(sa_nracanaa task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
