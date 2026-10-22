/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

ଶୂନ୍ୟ scx_tick(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ init_scx_entity(ସଂରଚନା sched_ext_entity *scx);
ଶୂନ୍ୟ scx_pre_fork(ସଂରଚନା task_struct *p);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_fork(ସଂରଚନା task_struct *p, ସଂରଚନା kernel_clone_args *kargs);
ଶୂନ୍ୟ scx_post_fork(ସଂରଚନା task_struct *p);
ଶୂନ୍ୟ scx_cancel_fork(ସଂରଚନା task_struct *p);
bool scx_can_stop_tick(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_rq_activate(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_rq_deactivate(ସଂରଚନା rq *rq);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_check_setscheduler(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy);
bool task_should_scx(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy);
bool scx_allow_ttwu_queue(ଅଚଳ ସଂରଚନା task_struct *p);
bool scx_allow_proxy_exec(ଅଚଳ ସଂରଚନା task_struct *p);
ଶୂନ୍ୟ scx_proxy_donor_start(ସଂରଚନା rq *rq);
ଶୂନ୍ୟ scx_proxy_reenqueue_retry(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *next);
ଶୂନ୍ୟ init_sched_ext_class(ଶୂନ୍ୟ);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u32 scx_cpuperf_target(s32 cpu)
{
	ଯଦି (scx_enabled())
		ଫେରନ୍ତୁ cpu_rq(cpu)->scx.cpuperf_target;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_on_scx(ଅଚଳ ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(ଅଚଳ ସଂରଚନା task_struct *a, ଅଚଳ ସଂରଚନା task_struct *b,
		   bool in_fi);
#endif

#ଅନ୍ୟଥା	/* CONFIG_SCHED_CLASS_EXT */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_tick(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_pre_fork(ସଂରଚନା task_struct *p) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_fork(ସଂରଚନା task_struct *p, ସଂରଚନା kernel_clone_args *kargs) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_post_fork(ସଂରଚନା task_struct *p) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_cancel_fork(ସଂରଚନା task_struct *p) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u32 scx_cpuperf_target(s32 cpu) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool scx_can_stop_tick(ସଂରଚନା rq *rq) { ଫେରନ୍ତୁ true; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rq_activate(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rq_deactivate(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_check_setscheduler(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_on_scx(ଅଚଳ ସଂରଚନା task_struct *p) { ଫେରନ୍ତୁ false; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool scx_allow_ttwu_queue(ଅଚଳ ସଂରଚନା task_struct *p) { ଫେରନ୍ତୁ true; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool scx_allow_proxy_exec(ଅଚଳ ସଂରଚନା task_struct *p) { ଫେରନ୍ତୁ true; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_proxy_donor_start(ସଂରଚନା rq *rq) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_proxy_reenqueue_retry(ସଂରଚନା rq *rq,
					     ସଂରଚନା task_struct *next) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ init_sched_ext_class(ଶୂନ୍ୟ) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
ଶୂନ୍ୟ __scx_update_idle(ସଂରଚନା rq *rq, bool idle, bool do_notify);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_update_idle(ସଂରଚନା rq *rq, bool idle, bool do_notify)
{
	ଯଦି (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_update_idle(ସଂରଚନା rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
ଶୂନ୍ୟ scx_tg_init(ସଂରଚନା task_group *tg);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_tg_online(ସଂରଚନା task_group *tg);
ଶୂନ୍ୟ scx_tg_offline(ସଂରଚନା task_group *tg);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_cgroup_can_attach(ସଂରଚନା cgroup_taskset *tset);
ଶୂନ୍ୟ scx_cgroup_move_task(ସଂରଚନା task_struct *p);
ଶୂନ୍ୟ scx_cgroup_cancel_attach(ସଂରଚନା cgroup_taskset *tset);
ଶୂନ୍ୟ scx_group_set_weight(ସଂରଚନା task_group *tg, unsigned long cgrp_weight);
ଶୂନ୍ୟ scx_group_set_idle(ସଂରଚନା task_group *tg, bool idle);
ଶୂନ୍ୟ scx_group_set_bandwidth(ସଂରଚନା task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#ଅନ୍ୟଥା	/* CONFIG_EXT_GROUP_SCHED */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_tg_init(ସଂରଚନା task_group *tg) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_tg_online(ସଂରଚନା task_group *tg) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_tg_offline(ସଂରଚନା task_group *tg) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scx_cgroup_can_attach(ସଂରଚନା cgroup_taskset *tset) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_cgroup_move_task(ସଂରଚନା task_struct *p) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_cgroup_cancel_attach(ସଂରଚନା cgroup_taskset *tset) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_group_set_weight(ସଂରଚନା task_group *tg, unsigned long cgrp_weight) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_group_set_idle(ସଂରଚନା task_group *tg, bool idle) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_group_set_bandwidth(ସଂରଚନା task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
