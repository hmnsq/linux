/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT

ಶೂನ್ಯ scx_tick(ರಚನೆ rq *rq);
ಶೂನ್ಯ init_scx_entity(ರಚನೆ sched_ext_entity *scx);
ಶೂನ್ಯ scx_pre_fork(ರಚನೆ task_struct *p);
ಪೂರ್ಣಾಂಕ scx_fork(ರಚನೆ task_struct *p, ರಚನೆ kernel_clone_ವಾದಗಳು *kargs);
ಶೂನ್ಯ scx_post_fork(ರಚನೆ task_struct *p);
ಶೂನ್ಯ scx_cancel_fork(ರಚನೆ task_struct *p);
bool scx_can_stop_tick(ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_rq_activate(ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_rq_deactivate(ರಚನೆ rq *rq);
ಪೂರ್ಣಾಂಕ scx_check_setscheduler(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ policy);
bool task_should_scx(ಪೂರ್ಣಾಂಕ policy);
bool scx_allow_ttwu_ಸರತಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p);
bool scx_allow_proxy_exec(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p);
ಶೂನ್ಯ scx_proxy_donor_ಆರಂಭ(ರಚನೆ rq *rq);
ಶೂನ್ಯ scx_proxy_reenqueue_retry(ರಚನೆ rq *rq, ರಚನೆ task_struct *ಮುಂದಿನ);
ಶೂನ್ಯ init_sched_ext_class(ಶೂನ್ಯ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u32 scx_cpuperf_ಗುರಿ(s32 cpu)
{
	ಆದರೆ (scx_enabled())
		ಹಿಂತಿರುಗಿ cpu_rq(cpu)->scx.cpuperf_ಗುರಿ;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_on_scx(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ scx_enabled() && p->sched_class == &ext_sched_class;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
bool scx_prio_less(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *a, ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *b,
		   bool ಒಳ_fi);
#ಆದರೆ_ಅಂತ್ಯ

#ಇಲ್ಲದಿದ್ದರೆ	/* CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_tick(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_pre_fork(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ scx_fork(ರಚನೆ task_struct *p, ರಚನೆ kernel_clone_ವಾದಗಳು *kargs) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_post_fork(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_cancel_fork(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u32 scx_cpuperf_ಗುರಿ(s32 cpu) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_can_stop_tick(ರಚನೆ rq *rq) { ಹಿಂತಿರುಗಿ true; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rq_activate(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rq_deactivate(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ scx_check_setscheduler(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ policy) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_on_scx(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_allow_ttwu_ಸರತಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ true; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool scx_allow_proxy_exec(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ true; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_proxy_donor_ಆರಂಭ(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_proxy_reenqueue_retry(ರಚನೆ rq *rq,
					     ರಚನೆ task_struct *ಮುಂದಿನ) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ init_sched_ext_class(ಶೂನ್ಯ) {}

#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_SCHED_CLASS_EXT */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
ಶೂನ್ಯ __scx_update_idle(ರಚನೆ rq *rq, bool idle, bool do_notify);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_update_idle(ರಚನೆ rq *rq, bool idle, bool do_notify)
{
	ಆದರೆ (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_update_idle(ರಚನೆ rq *rq, bool idle, bool do_notify) {}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_ಗುಂಪು_SCHED
ಶೂನ್ಯ scx_tg_init(ರಚನೆ task_ಗುಂಪು *tg);
ಪೂರ್ಣಾಂಕ scx_tg_online(ರಚನೆ task_ಗುಂಪು *tg);
ಶೂನ್ಯ scx_tg_offline(ರಚನೆ task_ಗುಂಪು *tg);
ಪೂರ್ಣಾಂಕ scx_cgroup_can_attach(ರಚನೆ cgroup_taskset *tset);
ಶೂನ್ಯ scx_cgroup_move_task(ರಚನೆ task_struct *p);
ಶೂನ್ಯ scx_cgroup_cancel_attach(ರಚನೆ cgroup_taskset *tset);
ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_ತೂಕ(ರಚನೆ task_ಗುಂಪು *tg, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cgrp_ತೂಕ);
ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_idle(ರಚನೆ task_ಗುಂಪು *tg, bool idle);
ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_bandwidth(ರಚನೆ task_ಗುಂಪು *tg, u64 period_us, u64 quota_us, u64 ಅವಧಿ_us);
#ಇಲ್ಲದಿದ್ದರೆ	/* CONFIG_EXT_GROUP_SCHED */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_tg_init(ರಚನೆ task_ಗುಂಪು *tg) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ scx_tg_online(ರಚನೆ task_ಗುಂಪು *tg) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_tg_offline(ರಚನೆ task_ಗುಂಪು *tg) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ scx_cgroup_can_attach(ರಚನೆ cgroup_taskset *tset) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_cgroup_move_task(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_cgroup_cancel_attach(ರಚನೆ cgroup_taskset *tset) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_ತೂಕ(ರಚನೆ task_ಗುಂಪು *tg, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cgrp_ತೂಕ) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_idle(ರಚನೆ task_ಗುಂಪು *tg, bool idle) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_ಗುಂಪು_ಗಣ_bandwidth(ರಚನೆ task_ಗುಂಪು *tg, u64 period_us, u64 quota_us, u64 ಅವಧಿ_us) {}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_GROUP_SCHED */
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_CGROUP_SCHED */
