/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT

ਖਾਲੀ scx_tick(ਬਣਤਰ rq *rq);
ਖਾਲੀ init_scx_entity(ਬਣਤਰ sched_ext_entity *scx);
ਖਾਲੀ scx_pre_fork(ਬਣਤਰ task_struct *p);
ਪੂਰਨ scx_fork(ਬਣਤਰ task_struct *p, ਬਣਤਰ kernel_clone_ਦਲੀਲਾਂ *kargs);
ਖਾਲੀ scx_post_fork(ਬਣਤਰ task_struct *p);
ਖਾਲੀ scx_cancel_fork(ਬਣਤਰ task_struct *p);
bool scx_can_stop_tick(ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_rq_activate(ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_rq_deactivate(ਬਣਤਰ rq *rq);
ਪੂਰਨ scx_check_setscheduler(ਬਣਤਰ task_struct *p, ਪੂਰਨ policy);
bool task_should_scx(ਪੂਰਨ policy);
bool scx_allow_ttwu_ਕਤਾਰ(ਅਚਲ ਬਣਤਰ task_struct *p);
bool scx_allow_proxy_exec(ਅਚਲ ਬਣਤਰ task_struct *p);
ਖਾਲੀ scx_proxy_donor_ਸ਼ੁਰੂ(ਬਣਤਰ rq *rq);
ਖਾਲੀ scx_proxy_reenqueue_retry(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *ਅਗਲਾ);
ਖਾਲੀ init_sched_ext_class(ਖਾਲੀ);

ਸਥਿਰ ਸਤਰਅੰਦਰ u32 scx_cpuperf_ਨਿਸ਼ਾਨਾ(s32 cpu)
{
	ਜੇ (scx_enabled())
		ਵਾਪਸ cpu_rq(cpu)->scx.cpuperf_ਨਿਸ਼ਾਨਾ;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_on_scx(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ scx_enabled() && p->sched_class == &ext_sched_class;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
bool scx_prio_less(ਅਚਲ ਬਣਤਰ task_struct *a, ਅਚਲ ਬਣਤਰ task_struct *b,
		   bool ਵਿੱਚ_fi);
#ਜੇ_ਅੰਤ

#ਨਹੀਂਤਾਂ	/* CONFIG_SCHED_CLASS_EXT */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_tick(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_pre_fork(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ scx_fork(ਬਣਤਰ task_struct *p, ਬਣਤਰ kernel_clone_ਦਲੀਲਾਂ *kargs) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_post_fork(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_cancel_fork(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ u32 scx_cpuperf_ਨਿਸ਼ਾਨਾ(s32 cpu) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_can_stop_tick(ਬਣਤਰ rq *rq) { ਵਾਪਸ true; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rq_activate(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rq_deactivate(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ scx_check_setscheduler(ਬਣਤਰ task_struct *p, ਪੂਰਨ policy) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_on_scx(ਅਚਲ ਬਣਤਰ task_struct *p) { ਵਾਪਸ false; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_allow_ttwu_ਕਤਾਰ(ਅਚਲ ਬਣਤਰ task_struct *p) { ਵਾਪਸ true; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool scx_allow_proxy_exec(ਅਚਲ ਬਣਤਰ task_struct *p) { ਵਾਪਸ true; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_proxy_donor_ਸ਼ੁਰੂ(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_proxy_reenqueue_retry(ਬਣਤਰ rq *rq,
					     ਬਣਤਰ task_struct *ਅਗਲਾ) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ init_sched_ext_class(ਖਾਲੀ) {}

#ਜੇ_ਅੰਤ	/* CONFIG_SCHED_CLASS_EXT */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
ਖਾਲੀ __scx_update_idle(ਬਣਤਰ rq *rq, bool idle, bool do_notify);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_update_idle(ਬਣਤਰ rq *rq, bool idle, bool do_notify)
{
	ਜੇ (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_update_idle(ਬਣਤਰ rq *rq, bool idle, bool do_notify) {}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_ਸਮੂਹ_SCHED
ਖਾਲੀ scx_tg_init(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਪੂਰਨ scx_tg_online(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਖਾਲੀ scx_tg_offline(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਪੂਰਨ scx_cgroup_can_attach(ਬਣਤਰ cgroup_taskset *tset);
ਖਾਲੀ scx_cgroup_move_task(ਬਣਤਰ task_struct *p);
ਖਾਲੀ scx_cgroup_cancel_attach(ਬਣਤਰ cgroup_taskset *tset);
ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_ਭਾਰ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cgrp_ਭਾਰ);
ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_idle(ਬਣਤਰ task_ਸਮੂਹ *tg, bool idle);
ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg, u64 period_us, u64 quota_us, u64 ਮਿਆਦ_us);
#ਨਹੀਂਤਾਂ	/* CONFIG_EXT_GROUP_SCHED */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_tg_init(ਬਣਤਰ task_ਸਮੂਹ *tg) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ scx_tg_online(ਬਣਤਰ task_ਸਮੂਹ *tg) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_tg_offline(ਬਣਤਰ task_ਸਮੂਹ *tg) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ scx_cgroup_can_attach(ਬਣਤਰ cgroup_taskset *tset) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_cgroup_move_task(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_cgroup_cancel_attach(ਬਣਤਰ cgroup_taskset *tset) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_ਭਾਰ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cgrp_ਭਾਰ) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_idle(ਬਣਤਰ task_ਸਮੂਹ *tg, bool idle) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_ਸਮੂਹ_ਸੈੱਟ_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg, u64 period_us, u64 quota_us, u64 ਮਿਆਦ_us) {}
#ਜੇ_ਅੰਤ	/* CONFIG_EXT_GROUP_SCHED */
#ਜੇ_ਅੰਤ	/* CONFIG_CGROUP_SCHED */
