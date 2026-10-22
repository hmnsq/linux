/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

ಶೂನ್ಯ scx_tick(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ init_scx_entity(ಸಂರಚನಾ sched_ext_entity *scx);
ಶೂನ್ಯ scx_pre_fork(ಸಂರಚನಾ task_struct *p);
ಪೂರ್ಣಾಙ್ಕ scx_fork(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ kernel_clone_args *kargs);
ಶೂನ್ಯ scx_post_fork(ಸಂರಚನಾ task_struct *p);
ಶೂನ್ಯ scx_cancel_fork(ಸಂರಚನಾ task_struct *p);
bool scx_can_stop_tick(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_rq_activate(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_rq_deactivate(ಸಂರಚನಾ rq *rq);
ಪೂರ್ಣಾಙ್ಕ scx_check_setscheduler(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ policy);
bool task_should_scx(ಪೂರ್ಣಾಙ್ಕ policy);
bool scx_allow_ttwu_queue(ಧ್ರುವ ಸಂರಚನಾ task_struct *p);
bool scx_allow_proxy_exec(ಧ್ರುವ ಸಂರಚನಾ task_struct *p);
ಶೂನ್ಯ scx_proxy_donor_start(ಸಂರಚನಾ rq *rq);
ಶೂನ್ಯ scx_proxy_reenqueue_retry(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *next);
ಶೂನ್ಯ init_sched_ext_class(ಶೂನ್ಯ);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u32 scx_cpuperf_target(s32 cpu)
{
	ಯದಿ (scx_enabled())
		ಪ್ರತ್ಯಾಗಮ cpu_rq(cpu)->scx.cpuperf_target;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_on_scx(ಧ್ರುವ ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(ಧ್ರುವ ಸಂರಚನಾ task_struct *a, ಧ್ರುವ ಸಂರಚನಾ task_struct *b,
		   bool in_fi);
#endif

#ಅನ್ಯಥಾ	/* CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_tick(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_pre_fork(ಸಂರಚನಾ task_struct *p) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ scx_fork(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ kernel_clone_args *kargs) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_post_fork(ಸಂರಚನಾ task_struct *p) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_cancel_fork(ಸಂರಚನಾ task_struct *p) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u32 scx_cpuperf_target(s32 cpu) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool scx_can_stop_tick(ಸಂರಚನಾ rq *rq) { ಪ್ರತ್ಯಾಗಮ true; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rq_activate(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rq_deactivate(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ scx_check_setscheduler(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ policy) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_on_scx(ಧ್ರುವ ಸಂರಚನಾ task_struct *p) { ಪ್ರತ್ಯಾಗಮ false; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool scx_allow_ttwu_queue(ಧ್ರುವ ಸಂರಚನಾ task_struct *p) { ಪ್ರತ್ಯಾಗಮ true; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool scx_allow_proxy_exec(ಧ್ರುವ ಸಂರಚನಾ task_struct *p) { ಪ್ರತ್ಯಾಗಮ true; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_proxy_donor_start(ಸಂರಚನಾ rq *rq) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_proxy_reenqueue_retry(ಸಂರಚನಾ rq *rq,
					     ಸಂರಚನಾ task_struct *next) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ init_sched_ext_class(ಶೂನ್ಯ) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
ಶೂನ್ಯ __scx_update_idle(ಸಂರಚನಾ rq *rq, bool idle, bool do_notify);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_update_idle(ಸಂರಚನಾ rq *rq, bool idle, bool do_notify)
{
	ಯದಿ (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_update_idle(ಸಂರಚನಾ rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
ಶೂನ್ಯ scx_tg_init(ಸಂರಚನಾ task_group *tg);
ಪೂರ್ಣಾಙ್ಕ scx_tg_online(ಸಂರಚನಾ task_group *tg);
ಶೂನ್ಯ scx_tg_offline(ಸಂರಚನಾ task_group *tg);
ಪೂರ್ಣಾಙ್ಕ scx_cgroup_can_attach(ಸಂರಚನಾ cgroup_taskset *tset);
ಶೂನ್ಯ scx_cgroup_move_task(ಸಂರಚನಾ task_struct *p);
ಶೂನ್ಯ scx_cgroup_cancel_attach(ಸಂರಚನಾ cgroup_taskset *tset);
ಶೂನ್ಯ scx_group_set_weight(ಸಂರಚನಾ task_group *tg, unsigned long cgrp_weight);
ಶೂನ್ಯ scx_group_set_idle(ಸಂರಚನಾ task_group *tg, bool idle);
ಶೂನ್ಯ scx_group_set_bandwidth(ಸಂರಚನಾ task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#ಅನ್ಯಥಾ	/* CONFIG_EXT_GROUP_SCHED */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_tg_init(ಸಂರಚನಾ task_group *tg) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ scx_tg_online(ಸಂರಚನಾ task_group *tg) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_tg_offline(ಸಂರಚನಾ task_group *tg) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ scx_cgroup_can_attach(ಸಂರಚನಾ cgroup_taskset *tset) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_cgroup_move_task(ಸಂರಚನಾ task_struct *p) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_cgroup_cancel_attach(ಸಂರಚನಾ cgroup_taskset *tset) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_group_set_weight(ಸಂರಚನಾ task_group *tg, unsigned long cgrp_weight) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_group_set_idle(ಸಂರಚನಾ task_group *tg, bool idle) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_group_set_bandwidth(ಸಂರಚನಾ task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
