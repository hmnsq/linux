/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

শূন্য scx_tick(কাঠামো rq *rq);
শূন্য init_scx_entity(কাঠামো sched_ext_entity *scx);
শূন্য scx_pre_fork(কাঠামো task_struct *p);
পূর্ণসংখ্যা scx_fork(কাঠামো task_struct *p, কাঠামো kernel_clone_args *kargs);
শূন্য scx_post_fork(কাঠামো task_struct *p);
শূন্য scx_cancel_fork(কাঠামো task_struct *p);
bool scx_can_stop_tick(কাঠামো rq *rq);
শূন্য scx_rq_activate(কাঠামো rq *rq);
শূন্য scx_rq_deactivate(কাঠামো rq *rq);
পূর্ণসংখ্যা scx_check_setscheduler(কাঠামো task_struct *p, পূর্ণসংখ্যা policy);
bool task_should_scx(পূর্ণসংখ্যা policy);
bool scx_allow_ttwu_queue(ধ্রুবক কাঠামো task_struct *p);
bool scx_allow_proxy_exec(ধ্রুবক কাঠামো task_struct *p);
শূন্য scx_proxy_donor_start(কাঠামো rq *rq);
শূন্য scx_proxy_reenqueue_retry(কাঠামো rq *rq, কাঠামো task_struct *পরবর্তী);
শূন্য init_sched_ext_class(শূন্য);

স্থির অন্তঃরেখ u32 scx_cpuperf_target(s32 cpu)
{
	যদি (scx_enabled())
		ফেরত cpu_rq(cpu)->scx.cpuperf_target;
	অন্যথা
		ফেরত 0;
}

স্থির অন্তঃরেখ bool task_on_scx(ধ্রুবক কাঠামো task_struct *p)
{
	ফেরত scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(ধ্রুবক কাঠামো task_struct *a, ধ্রুবক কাঠামো task_struct *b,
		   bool in_fi);
#endif

#অন্যথা	/* CONFIG_SCHED_CLASS_EXT */

স্থির অন্তঃরেখ শূন্য scx_tick(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_pre_fork(কাঠামো task_struct *p) {}
স্থির অন্তঃরেখ পূর্ণসংখ্যা scx_fork(কাঠামো task_struct *p, কাঠামো kernel_clone_args *kargs) { ফেরত 0; }
স্থির অন্তঃরেখ শূন্য scx_post_fork(কাঠামো task_struct *p) {}
স্থির অন্তঃরেখ শূন্য scx_cancel_fork(কাঠামো task_struct *p) {}
স্থির অন্তঃরেখ u32 scx_cpuperf_target(s32 cpu) { ফেরত 0; }
স্থির অন্তঃরেখ bool scx_can_stop_tick(কাঠামো rq *rq) { ফেরত true; }
স্থির অন্তঃরেখ শূন্য scx_rq_activate(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_rq_deactivate(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ পূর্ণসংখ্যা scx_check_setscheduler(কাঠামো task_struct *p, পূর্ণসংখ্যা policy) { ফেরত 0; }
স্থির অন্তঃরেখ bool task_on_scx(ধ্রুবক কাঠামো task_struct *p) { ফেরত false; }
স্থির অন্তঃরেখ bool scx_allow_ttwu_queue(ধ্রুবক কাঠামো task_struct *p) { ফেরত true; }
স্থির অন্তঃরেখ bool scx_allow_proxy_exec(ধ্রুবক কাঠামো task_struct *p) { ফেরত true; }
স্থির অন্তঃরেখ শূন্য scx_proxy_donor_start(কাঠামো rq *rq) {}
স্থির অন্তঃরেখ শূন্য scx_proxy_reenqueue_retry(কাঠামো rq *rq,
					     কাঠামো task_struct *পরবর্তী) {}
স্থির অন্তঃরেখ শূন্য init_sched_ext_class(শূন্য) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
শূন্য __scx_update_idle(কাঠামো rq *rq, bool idle, bool do_notify);

স্থির অন্তঃরেখ শূন্য scx_update_idle(কাঠামো rq *rq, bool idle, bool do_notify)
{
	যদি (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#অন্যথা
স্থির অন্তঃরেখ শূন্য scx_update_idle(কাঠামো rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
শূন্য scx_tg_init(কাঠামো task_group *tg);
পূর্ণসংখ্যা scx_tg_online(কাঠামো task_group *tg);
শূন্য scx_tg_offline(কাঠামো task_group *tg);
পূর্ণসংখ্যা scx_cgroup_can_attach(কাঠামো cgroup_taskset *tset);
শূন্য scx_cgroup_move_task(কাঠামো task_struct *p);
শূন্য scx_cgroup_cancel_attach(কাঠামো cgroup_taskset *tset);
শূন্য scx_group_set_weight(কাঠামো task_group *tg, unsigned long cgrp_weight);
শূন্য scx_group_set_idle(কাঠামো task_group *tg, bool idle);
শূন্য scx_group_set_bandwidth(কাঠামো task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#অন্যথা	/* CONFIG_EXT_GROUP_SCHED */
স্থির অন্তঃরেখ শূন্য scx_tg_init(কাঠামো task_group *tg) {}
স্থির অন্তঃরেখ পূর্ণসংখ্যা scx_tg_online(কাঠামো task_group *tg) { ফেরত 0; }
স্থির অন্তঃরেখ শূন্য scx_tg_offline(কাঠামো task_group *tg) {}
স্থির অন্তঃরেখ পূর্ণসংখ্যা scx_cgroup_can_attach(কাঠামো cgroup_taskset *tset) { ফেরত 0; }
স্থির অন্তঃরেখ শূন্য scx_cgroup_move_task(কাঠামো task_struct *p) {}
স্থির অন্তঃরেখ শূন্য scx_cgroup_cancel_attach(কাঠামো cgroup_taskset *tset) {}
স্থির অন্তঃরেখ শূন্য scx_group_set_weight(কাঠামো task_group *tg, unsigned long cgrp_weight) {}
স্থির অন্তঃরেখ শূন্য scx_group_set_idle(কাঠামো task_group *tg, bool idle) {}
স্থির অন্তঃরেখ শূন্য scx_group_set_bandwidth(কাঠামো task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
