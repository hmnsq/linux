/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#ifdef CONFIG_SCHED_CLASS_EXT

ശൂന്യം scx_tick(ഘടന rq *rq);
ശൂന്യം init_scx_entity(ഘടന sched_ext_entity *scx);
ശൂന്യം scx_pre_fork(ഘടന task_struct *p);
പൂർണ്ണസംഖ്യ scx_fork(ഘടന task_struct *p, ഘടന kernel_clone_args *kargs);
ശൂന്യം scx_post_fork(ഘടന task_struct *p);
ശൂന്യം scx_cancel_fork(ഘടന task_struct *p);
bool scx_can_stop_tick(ഘടന rq *rq);
ശൂന്യം scx_rq_activate(ഘടന rq *rq);
ശൂന്യം scx_rq_deactivate(ഘടന rq *rq);
പൂർണ്ണസംഖ്യ scx_check_setscheduler(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ policy);
bool task_should_scx(പൂർണ്ണസംഖ്യ policy);
bool scx_allow_ttwu_queue(സ്ഥിരം ഘടന task_struct *p);
bool scx_allow_proxy_exec(സ്ഥിരം ഘടന task_struct *p);
ശൂന്യം scx_proxy_donor_start(ഘടന rq *rq);
ശൂന്യം scx_proxy_reenqueue_retry(ഘടന rq *rq, ഘടന task_struct *next);
ശൂന്യം init_sched_ext_class(ശൂന്യം);

സ്ഥിതം വരിയിൽ u32 scx_cpuperf_target(s32 cpu)
{
	എങ്കിൽ (scx_enabled())
		തിരികെ cpu_rq(cpu)->scx.cpuperf_target;
	അല്ലെങ്കിൽ
		തിരികെ 0;
}

സ്ഥിതം വരിയിൽ bool task_on_scx(സ്ഥിരം ഘടന task_struct *p)
{
	തിരികെ scx_enabled() && p->sched_class == &ext_sched_class;
}

#ifdef CONFIG_SCHED_CORE
bool scx_prio_less(സ്ഥിരം ഘടന task_struct *a, സ്ഥിരം ഘടന task_struct *b,
		   bool in_fi);
#endif

#അല്ലെങ്കിൽ	/* CONFIG_SCHED_CLASS_EXT */

സ്ഥിതം വരിയിൽ ശൂന്യം scx_tick(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_pre_fork(ഘടന task_struct *p) {}
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ scx_fork(ഘടന task_struct *p, ഘടന kernel_clone_args *kargs) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_post_fork(ഘടന task_struct *p) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_cancel_fork(ഘടന task_struct *p) {}
സ്ഥിതം വരിയിൽ u32 scx_cpuperf_target(s32 cpu) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ bool scx_can_stop_tick(ഘടന rq *rq) { തിരികെ true; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rq_activate(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rq_deactivate(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ scx_check_setscheduler(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ policy) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ bool task_on_scx(സ്ഥിരം ഘടന task_struct *p) { തിരികെ false; }
സ്ഥിതം വരിയിൽ bool scx_allow_ttwu_queue(സ്ഥിരം ഘടന task_struct *p) { തിരികെ true; }
സ്ഥിതം വരിയിൽ bool scx_allow_proxy_exec(സ്ഥിരം ഘടന task_struct *p) { തിരികെ true; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_proxy_donor_start(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_proxy_reenqueue_retry(ഘടന rq *rq,
					     ഘടന task_struct *next) {}
സ്ഥിതം വരിയിൽ ശൂന്യം init_sched_ext_class(ശൂന്യം) {}

#endif	/* CONFIG_SCHED_CLASS_EXT */

#ifdef CONFIG_SCHED_CLASS_EXT
ശൂന്യം __scx_update_idle(ഘടന rq *rq, bool idle, bool do_notify);

സ്ഥിതം വരിയിൽ ശൂന്യം scx_update_idle(ഘടന rq *rq, bool idle, bool do_notify)
{
	എങ്കിൽ (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ശൂന്യം scx_update_idle(ഘടന rq *rq, bool idle, bool do_notify) {}
#endif

#ifdef CONFIG_CGROUP_SCHED
#ifdef CONFIG_EXT_GROUP_SCHED
ശൂന്യം scx_tg_init(ഘടന task_group *tg);
പൂർണ്ണസംഖ്യ scx_tg_online(ഘടന task_group *tg);
ശൂന്യം scx_tg_offline(ഘടന task_group *tg);
പൂർണ്ണസംഖ്യ scx_cgroup_can_attach(ഘടന cgroup_taskset *tset);
ശൂന്യം scx_cgroup_move_task(ഘടന task_struct *p);
ശൂന്യം scx_cgroup_cancel_attach(ഘടന cgroup_taskset *tset);
ശൂന്യം scx_group_set_weight(ഘടന task_group *tg, unsigned long cgrp_weight);
ശൂന്യം scx_group_set_idle(ഘടന task_group *tg, bool idle);
ശൂന്യം scx_group_set_bandwidth(ഘടന task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#അല്ലെങ്കിൽ	/* CONFIG_EXT_GROUP_SCHED */
സ്ഥിതം വരിയിൽ ശൂന്യം scx_tg_init(ഘടന task_group *tg) {}
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ scx_tg_online(ഘടന task_group *tg) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_tg_offline(ഘടന task_group *tg) {}
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ scx_cgroup_can_attach(ഘടന cgroup_taskset *tset) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ ശൂന്യം scx_cgroup_move_task(ഘടന task_struct *p) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_cgroup_cancel_attach(ഘടന cgroup_taskset *tset) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_group_set_weight(ഘടന task_group *tg, unsigned long cgrp_weight) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_group_set_idle(ഘടന task_group *tg, bool idle) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_group_set_bandwidth(ഘടന task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#endif	/* CONFIG_EXT_GROUP_SCHED */
#endif	/* CONFIG_CGROUP_SCHED */
