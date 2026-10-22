/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT

خالی scx_tick(ساخت rq *rq);
خالی init_scx_entity(ساخت sched_ext_entity *scx);
خالی scx_pre_fork(ساخت task_struct *p);
صحیح scx_fork(ساخت task_struct *p, ساخت kernel_clone_دلائل *kargs);
خالی scx_post_fork(ساخت task_struct *p);
خالی scx_cancel_fork(ساخت task_struct *p);
bool scx_can_stop_tick(ساخت rq *rq);
خالی scx_rq_activate(ساخت rq *rq);
خالی scx_rq_deactivate(ساخت rq *rq);
صحیح scx_check_setscheduler(ساخت task_struct *p, صحیح policy);
bool task_should_scx(صحیح policy);
bool scx_allow_ttwu_قطار(ثابت ساخت task_struct *p);
bool scx_allow_proxy_exec(ثابت ساخت task_struct *p);
خالی scx_proxy_donor_آغاز(ساخت rq *rq);
خالی scx_proxy_reenqueue_retry(ساخت rq *rq, ساخت task_struct *اگلا);
خالی init_sched_ext_class(خالی);

ساکن اندرونی u32 scx_cpuperf_ہدف(s32 cpu)
{
	اگر (scx_enabled())
		واپس cpu_rq(cpu)->scx.cpuperf_ہدف;
	ورنہ
		واپس 0;
}

ساکن اندرونی bool task_on_scx(ثابت ساخت task_struct *p)
{
	واپس scx_enabled() && p->sched_class == &ext_sched_class;
}

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
bool scx_prio_less(ثابت ساخت task_struct *a, ثابت ساخت task_struct *b,
		   bool اندر_fi);
#اگر_ختم

#ورنہ	/* CONFIG_SCHED_CLASS_EXT */

ساکن اندرونی خالی scx_tick(ساخت rq *rq) {}
ساکن اندرونی خالی scx_pre_fork(ساخت task_struct *p) {}
ساکن اندرونی صحیح scx_fork(ساخت task_struct *p, ساخت kernel_clone_دلائل *kargs) { واپس 0; }
ساکن اندرونی خالی scx_post_fork(ساخت task_struct *p) {}
ساکن اندرونی خالی scx_cancel_fork(ساخت task_struct *p) {}
ساکن اندرونی u32 scx_cpuperf_ہدف(s32 cpu) { واپس 0; }
ساکن اندرونی bool scx_can_stop_tick(ساخت rq *rq) { واپس true; }
ساکن اندرونی خالی scx_rq_activate(ساخت rq *rq) {}
ساکن اندرونی خالی scx_rq_deactivate(ساخت rq *rq) {}
ساکن اندرونی صحیح scx_check_setscheduler(ساخت task_struct *p, صحیح policy) { واپس 0; }
ساکن اندرونی bool task_on_scx(ثابت ساخت task_struct *p) { واپس false; }
ساکن اندرونی bool scx_allow_ttwu_قطار(ثابت ساخت task_struct *p) { واپس true; }
ساکن اندرونی bool scx_allow_proxy_exec(ثابت ساخت task_struct *p) { واپس true; }
ساکن اندرونی خالی scx_proxy_donor_آغاز(ساخت rq *rq) {}
ساکن اندرونی خالی scx_proxy_reenqueue_retry(ساخت rq *rq,
					     ساخت task_struct *اگلا) {}
ساکن اندرونی خالی init_sched_ext_class(خالی) {}

#اگر_ختم	/* CONFIG_SCHED_CLASS_EXT */

#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
خالی __scx_update_idle(ساخت rq *rq, bool idle, bool do_notify);

ساکن اندرونی خالی scx_update_idle(ساخت rq *rq, bool idle, bool do_notify)
{
	اگر (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#ورنہ
ساکن اندرونی خالی scx_update_idle(ساخت rq *rq, bool idle, bool do_notify) {}
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED
#اگر_تعریف_شدہ CONFIG_EXT_گروہ_SCHED
خالی scx_tg_init(ساخت task_گروہ *tg);
صحیح scx_tg_online(ساخت task_گروہ *tg);
خالی scx_tg_offline(ساخت task_گروہ *tg);
صحیح scx_cgroup_can_attach(ساخت cgroup_taskset *tset);
خالی scx_cgroup_move_task(ساخت task_struct *p);
خالی scx_cgroup_cancel_attach(ساخت cgroup_taskset *tset);
خالی scx_گروہ_سیٹ_وزن(ساخت task_گروہ *tg, غیرعلامتی طویل cgrp_وزن);
خالی scx_گروہ_سیٹ_idle(ساخت task_گروہ *tg, bool idle);
خالی scx_گروہ_سیٹ_bandwidth(ساخت task_گروہ *tg, u64 period_us, u64 quota_us, u64 مدت_us);
#ورنہ	/* CONFIG_EXT_GROUP_SCHED */
ساکن اندرونی خالی scx_tg_init(ساخت task_گروہ *tg) {}
ساکن اندرونی صحیح scx_tg_online(ساخت task_گروہ *tg) { واپس 0; }
ساکن اندرونی خالی scx_tg_offline(ساخت task_گروہ *tg) {}
ساکن اندرونی صحیح scx_cgroup_can_attach(ساخت cgroup_taskset *tset) { واپس 0; }
ساکن اندرونی خالی scx_cgroup_move_task(ساخت task_struct *p) {}
ساکن اندرونی خالی scx_cgroup_cancel_attach(ساخت cgroup_taskset *tset) {}
ساکن اندرونی خالی scx_گروہ_سیٹ_وزن(ساخت task_گروہ *tg, غیرعلامتی طویل cgrp_وزن) {}
ساکن اندرونی خالی scx_گروہ_سیٹ_idle(ساخت task_گروہ *tg, bool idle) {}
ساکن اندرونی خالی scx_گروہ_سیٹ_bandwidth(ساخت task_گروہ *tg, u64 period_us, u64 quota_us, u64 مدت_us) {}
#اگر_ختم	/* CONFIG_EXT_GROUP_SCHED */
#اگر_ختم	/* CONFIG_CGROUP_SCHED */
