/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT

فراغ scx_tick(بنية rq *rq);
فراغ init_scx_entity(بنية sched_ext_entity *scx);
فراغ scx_pre_fork(بنية task_struct *p);
صحيح scx_fork(بنية task_struct *p, بنية kernel_clone_دليلون *kargs);
فراغ scx_post_fork(بنية task_struct *p);
فراغ scx_cancel_fork(بنية task_struct *p);
bool scx_can_stop_tick(بنية rq *rq);
فراغ scx_rq_activate(بنية rq *rq);
فراغ scx_rq_deactivate(بنية rq *rq);
صحيح scx_check_setscheduler(بنية task_struct *p, صحيح policy);
bool task_should_scx(صحيح policy);
bool scx_allow_ttwu_قطار(ثابت بنية task_struct *p);
bool scx_allow_proxy_exec(ثابت بنية task_struct *p);
فراغ scx_proxy_donor_شروعات(بنية rq *rq);
فراغ scx_proxy_reenqueue_retry(بنية rq *rq, بنية task_struct *ايندڙ);
فراغ init_sched_ext_class(فراغ);

ساكن مضمن u32 scx_cpuperf_هدف(s32 cpu)
{
	إذا (scx_enabled())
		أرجع cpu_rq(cpu)->scx.cpuperf_هدف;
	وإلا
		أرجع 0;
}

ساكن مضمن bool task_on_scx(ثابت بنية task_struct *p)
{
	أرجع scx_enabled() && p->sched_class == &ext_sched_class;
}

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
bool scx_prio_less(ثابت بنية task_struct *a, ثابت بنية task_struct *b,
		   bool اندريون_fi);
#جيڪڏهن_پورو

#وإلا	/* CONFIG_SCHED_CLASS_EXT */

ساكن مضمن فراغ scx_tick(بنية rq *rq) {}
ساكن مضمن فراغ scx_pre_fork(بنية task_struct *p) {}
ساكن مضمن صحيح scx_fork(بنية task_struct *p, بنية kernel_clone_دليلون *kargs) { أرجع 0; }
ساكن مضمن فراغ scx_post_fork(بنية task_struct *p) {}
ساكن مضمن فراغ scx_cancel_fork(بنية task_struct *p) {}
ساكن مضمن u32 scx_cpuperf_هدف(s32 cpu) { أرجع 0; }
ساكن مضمن bool scx_can_stop_tick(بنية rq *rq) { أرجع true; }
ساكن مضمن فراغ scx_rq_activate(بنية rq *rq) {}
ساكن مضمن فراغ scx_rq_deactivate(بنية rq *rq) {}
ساكن مضمن صحيح scx_check_setscheduler(بنية task_struct *p, صحيح policy) { أرجع 0; }
ساكن مضمن bool task_on_scx(ثابت بنية task_struct *p) { أرجع false; }
ساكن مضمن bool scx_allow_ttwu_قطار(ثابت بنية task_struct *p) { أرجع true; }
ساكن مضمن bool scx_allow_proxy_exec(ثابت بنية task_struct *p) { أرجع true; }
ساكن مضمن فراغ scx_proxy_donor_شروعات(بنية rq *rq) {}
ساكن مضمن فراغ scx_proxy_reenqueue_retry(بنية rq *rq,
					     بنية task_struct *ايندڙ) {}
ساكن مضمن فراغ init_sched_ext_class(فراغ) {}

#جيڪڏهن_پورو	/* CONFIG_SCHED_CLASS_EXT */

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
فراغ __scx_update_idle(بنية rq *rq, bool idle, bool do_notify);

ساكن مضمن فراغ scx_update_idle(بنية rq *rq, bool idle, bool do_notify)
{
	إذا (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#وإلا
ساكن مضمن فراغ scx_update_idle(بنية rq *rq, bool idle, bool do_notify) {}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED
#جيڪڏهن_وصف_ٿيل CONFIG_EXT_گروهه_SCHED
فراغ scx_tg_init(بنية task_گروهه *tg);
صحيح scx_tg_online(بنية task_گروهه *tg);
فراغ scx_tg_offline(بنية task_گروهه *tg);
صحيح scx_cgroup_can_attach(بنية cgroup_taskset *tset);
فراغ scx_cgroup_move_task(بنية task_struct *p);
فراغ scx_cgroup_cancel_attach(بنية cgroup_taskset *tset);
فراغ scx_گروهه_سيٽ_وزن(بنية task_گروهه *tg, بنانشان ڊگهو cgrp_وزن);
فراغ scx_گروهه_سيٽ_idle(بنية task_گروهه *tg, bool idle);
فراغ scx_گروهه_سيٽ_bandwidth(بنية task_گروهه *tg, u64 period_us, u64 quota_us, u64 مدو_us);
#وإلا	/* CONFIG_EXT_GROUP_SCHED */
ساكن مضمن فراغ scx_tg_init(بنية task_گروهه *tg) {}
ساكن مضمن صحيح scx_tg_online(بنية task_گروهه *tg) { أرجع 0; }
ساكن مضمن فراغ scx_tg_offline(بنية task_گروهه *tg) {}
ساكن مضمن صحيح scx_cgroup_can_attach(بنية cgroup_taskset *tset) { أرجع 0; }
ساكن مضمن فراغ scx_cgroup_move_task(بنية task_struct *p) {}
ساكن مضمن فراغ scx_cgroup_cancel_attach(بنية cgroup_taskset *tset) {}
ساكن مضمن فراغ scx_گروهه_سيٽ_وزن(بنية task_گروهه *tg, بنانشان ڊگهو cgrp_وزن) {}
ساكن مضمن فراغ scx_گروهه_سيٽ_idle(بنية task_گروهه *tg, bool idle) {}
ساكن مضمن فراغ scx_گروهه_سيٽ_bandwidth(بنية task_گروهه *tg, u64 period_us, u64 quota_us, u64 مدو_us) {}
#جيڪڏهن_پورو	/* CONFIG_EXT_GROUP_SCHED */
#جيڪڏهن_پورو	/* CONFIG_CGROUP_SCHED */
