/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

शून्य scx_tick(संरचना rq *rq);
शून्य init_scx_entity(संरचना sched_ext_entity *scx);
शून्य scx_pre_fork(संरचना task_struct *p);
पूर्णाङ्क scx_fork(संरचना task_struct *p, संरचना kernel_clone_args *kargs);
शून्य scx_post_fork(संरचना task_struct *p);
शून्य scx_cancel_fork(संरचना task_struct *p);
bool scx_can_stop_tick(संरचना rq *rq);
शून्य scx_rq_activate(संरचना rq *rq);
शून्य scx_rq_deactivate(संरचना rq *rq);
पूर्णाङ्क scx_check_setscheduler(संरचना task_struct *p, पूर्णाङ्क policy);
bool task_should_scx(पूर्णाङ्क policy);
bool scx_allow_ttwu_queue(ध्रुव संरचना task_struct *p);
bool scx_allow_proxy_exec(ध्रुव संरचना task_struct *p);
शून्य scx_proxy_donor_start(संरचना rq *rq);
शून्य scx_proxy_reenqueue_retry(संरचना rq *rq, संरचना task_struct *next);
शून्य init_sched_ext_class(शून्य);

स्थिर अन्तःस्थ u32 scx_cpuperf_target(s32 cpu)
{
	सचे (scx_enabled())
		पटिनिवत्त cpu_rq(cpu)->scx.cpuperf_target;
	अञ्ञथा
		पटिनिवत्त 0;
}

स्थिर अन्तःस्थ bool task_on_scx(ध्रुव संरचना task_struct *p)
{
	पटिनिवत्त scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(ध्रुव संरचना task_struct *a, ध्रुव संरचना task_struct *b,
		   bool in_fi);
#endif

#अञ्ञथा	/* CONFIG_SCHED_CLASS_EXT */

स्थिर अन्तःस्थ शून्य scx_tick(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_pre_fork(संरचना task_struct *p) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_fork(संरचना task_struct *p, संरचना kernel_clone_args *kargs) { पटिनिवत्त 0; }
स्थिर अन्तःस्थ शून्य scx_post_fork(संरचना task_struct *p) {}
स्थिर अन्तःस्थ शून्य scx_cancel_fork(संरचना task_struct *p) {}
स्थिर अन्तःस्थ u32 scx_cpuperf_target(s32 cpu) { पटिनिवत्त 0; }
स्थिर अन्तःस्थ bool scx_can_stop_tick(संरचना rq *rq) { पटिनिवत्त true; }
स्थिर अन्तःस्थ शून्य scx_rq_activate(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_rq_deactivate(संरचना rq *rq) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_check_setscheduler(संरचना task_struct *p, पूर्णाङ्क policy) { पटिनिवत्त 0; }
स्थिर अन्तःस्थ bool task_on_scx(ध्रुव संरचना task_struct *p) { पटिनिवत्त false; }
स्थिर अन्तःस्थ bool scx_allow_ttwu_queue(ध्रुव संरचना task_struct *p) { पटिनिवत्त true; }
स्थिर अन्तःस्थ bool scx_allow_proxy_exec(ध्रुव संरचना task_struct *p) { पटिनिवत्त true; }
स्थिर अन्तःस्थ शून्य scx_proxy_donor_start(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_proxy_reenqueue_retry(संरचना rq *rq,
					     संरचना task_struct *next) {}
स्थिर अन्तःस्थ शून्य init_sched_ext_class(शून्य) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
शून्य __scx_update_idle(संरचना rq *rq, bool idle, bool do_notify);

स्थिर अन्तःस्थ शून्य scx_update_idle(संरचना rq *rq, bool idle, bool do_notify)
{
	सचे (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#अञ्ञथा
स्थिर अन्तःस्थ शून्य scx_update_idle(संरचना rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
शून्य scx_tg_init(संरचना task_group *tg);
पूर्णाङ्क scx_tg_online(संरचना task_group *tg);
शून्य scx_tg_offline(संरचना task_group *tg);
पूर्णाङ्क scx_cgroup_can_attach(संरचना cgroup_taskset *tset);
शून्य scx_cgroup_move_task(संरचना task_struct *p);
शून्य scx_cgroup_cancel_attach(संरचना cgroup_taskset *tset);
शून्य scx_group_set_weight(संरचना task_group *tg, unsigned long cgrp_weight);
शून्य scx_group_set_idle(संरचना task_group *tg, bool idle);
शून्य scx_group_set_bandwidth(संरचना task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#अञ्ञथा	/* CONFIG_EXT_GROUP_SCHED */
स्थिर अन्तःस्थ शून्य scx_tg_init(संरचना task_group *tg) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_tg_online(संरचना task_group *tg) { पटिनिवत्त 0; }
स्थिर अन्तःस्थ शून्य scx_tg_offline(संरचना task_group *tg) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_cgroup_can_attach(संरचना cgroup_taskset *tset) { पटिनिवत्त 0; }
स्थिर अन्तःस्थ शून्य scx_cgroup_move_task(संरचना task_struct *p) {}
स्थिर अन्तःस्थ शून्य scx_cgroup_cancel_attach(संरचना cgroup_taskset *tset) {}
स्थिर अन्तःस्थ शून्य scx_group_set_weight(संरचना task_group *tg, unsigned long cgrp_weight) {}
स्थिर अन्तःस्थ शून्य scx_group_set_idle(संरचना task_group *tg, bool idle) {}
स्थिर अन्तःस्थ शून्य scx_group_set_bandwidth(संरचना task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
