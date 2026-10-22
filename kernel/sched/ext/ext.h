/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT

शून्य scx_tick(संरचना rq *rq);
शून्य init_scx_entity(संरचना sched_ext_entity *scx);
शून्य scx_pre_fork(संरचना task_struct *p);
पूर्णाङ्क scx_fork(संरचना task_struct *p, संरचना kernel_clone_तर्काः *kargs);
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
शून्य scx_proxy_reenqueue_retry(संरचना rq *rq, संरचना task_struct *अग्र);
शून्य init_sched_ext_class(शून्य);

स्थिर अन्तःस्थ u32 scx_cpuperf_target(s32 cpu)
{
	यदि (scx_enabled())
		प्रत्यागम cpu_rq(cpu)->scx.cpuperf_target;
	अन्यथा
		प्रत्यागम 0;
}

स्थिर अन्तःस्थ bool task_on_scx(ध्रुव संरचना task_struct *p)
{
	प्रत्यागम scx_enabled() && p->sched_class == &ext_sched_class;
}

#यदि_परिभाषितम् CONFIG_SCHED_CORE
bool scx_prio_less(ध्रुव संरचना task_struct *a, ध्रुव संरचना task_struct *b,
		   bool अन्तर्_fi);
#यदि_अन्तः

#अन्यथा	/* CONFIG_SCHED_CLASS_EXT */

स्थिर अन्तःस्थ शून्य scx_tick(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_pre_fork(संरचना task_struct *p) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_fork(संरचना task_struct *p, संरचना kernel_clone_तर्काः *kargs) { प्रत्यागम 0; }
स्थिर अन्तःस्थ शून्य scx_post_fork(संरचना task_struct *p) {}
स्थिर अन्तःस्थ शून्य scx_cancel_fork(संरचना task_struct *p) {}
स्थिर अन्तःस्थ u32 scx_cpuperf_target(s32 cpu) { प्रत्यागम 0; }
स्थिर अन्तःस्थ bool scx_can_stop_tick(संरचना rq *rq) { प्रत्यागम true; }
स्थिर अन्तःस्थ शून्य scx_rq_activate(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_rq_deactivate(संरचना rq *rq) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_check_setscheduler(संरचना task_struct *p, पूर्णाङ्क policy) { प्रत्यागम 0; }
स्थिर अन्तःस्थ bool task_on_scx(ध्रुव संरचना task_struct *p) { प्रत्यागम false; }
स्थिर अन्तःस्थ bool scx_allow_ttwu_queue(ध्रुव संरचना task_struct *p) { प्रत्यागम true; }
स्थिर अन्तःस्थ bool scx_allow_proxy_exec(ध्रुव संरचना task_struct *p) { प्रत्यागम true; }
स्थिर अन्तःस्थ शून्य scx_proxy_donor_start(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य scx_proxy_reenqueue_retry(संरचना rq *rq,
					     संरचना task_struct *अग्र) {}
स्थिर अन्तःस्थ शून्य init_sched_ext_class(शून्य) {}

#यदि_अन्तः	/* CONFIG_SCHED_CLASS_EXT */

#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
शून्य __scx_update_idle(संरचना rq *rq, bool idle, bool do_notify);

स्थिर अन्तःस्थ शून्य scx_update_idle(संरचना rq *rq, bool idle, bool do_notify)
{
	यदि (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#अन्यथा
स्थिर अन्तःस्थ शून्य scx_update_idle(संरचना rq *rq, bool idle, bool do_notify) {}
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_CGROUP_SCHED
#यदि_परिभाषितम् CONFIG_EXT_गणः_SCHED
शून्य scx_tg_init(संरचना task_गणः *tg);
पूर्णाङ्क scx_tg_online(संरचना task_गणः *tg);
शून्य scx_tg_offline(संरचना task_गणः *tg);
पूर्णाङ्क scx_cgroup_can_attach(संरचना cgroup_taskset *tset);
शून्य scx_cgroup_move_task(संरचना task_struct *p);
शून्य scx_cgroup_cancel_attach(संरचना cgroup_taskset *tset);
शून्य scx_गणः_समुच्चयः_weight(संरचना task_गणः *tg, अचिह्नित दीर्घ cgrp_weight);
शून्य scx_गणः_समुच्चयः_idle(संरचना task_गणः *tg, bool idle);
शून्य scx_गणः_समुच्चयः_bandwidth(संरचना task_गणः *tg, u64 period_us, u64 quota_us, u64 burst_us);
#अन्यथा	/* CONFIG_EXT_GROUP_SCHED */
स्थिर अन्तःस्थ शून्य scx_tg_init(संरचना task_गणः *tg) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_tg_online(संरचना task_गणः *tg) { प्रत्यागम 0; }
स्थिर अन्तःस्थ शून्य scx_tg_offline(संरचना task_गणः *tg) {}
स्थिर अन्तःस्थ पूर्णाङ्क scx_cgroup_can_attach(संरचना cgroup_taskset *tset) { प्रत्यागम 0; }
स्थिर अन्तःस्थ शून्य scx_cgroup_move_task(संरचना task_struct *p) {}
स्थिर अन्तःस्थ शून्य scx_cgroup_cancel_attach(संरचना cgroup_taskset *tset) {}
स्थिर अन्तःस्थ शून्य scx_गणः_समुच्चयः_weight(संरचना task_गणः *tg, अचिह्नित दीर्घ cgrp_weight) {}
स्थिर अन्तःस्थ शून्य scx_गणः_समुच्चयः_idle(संरचना task_गणः *tg, bool idle) {}
स्थिर अन्तःस्थ शून्य scx_गणः_समुच्चयः_bandwidth(संरचना task_गणः *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#यदि_अन्तः	/* CONFIG_EXT_GROUP_SCHED */
#यदि_अन्तः	/* CONFIG_CGROUP_SCHED */
