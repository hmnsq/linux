/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT

శూన్య scx_tick(నిర్మాణం rq *rq);
శూన్య init_scx_entity(నిర్మాణం sched_ext_entity *scx);
శూన్య scx_pre_fork(నిర్మాణం task_struct *p);
పూర్ణాంకం scx_fork(నిర్మాణం task_struct *p, నిర్మాణం kernel_clone_వాదనలు *kargs);
శూన్య scx_post_fork(నిర్మాణం task_struct *p);
శూన్య scx_cancel_fork(నిర్మాణం task_struct *p);
bool scx_can_stop_tick(నిర్మాణం rq *rq);
శూన్య scx_rq_activate(నిర్మాణం rq *rq);
శూన్య scx_rq_deactivate(నిర్మాణం rq *rq);
పూర్ణాంకం scx_check_setscheduler(నిర్మాణం task_struct *p, పూర్ణాంకం policy);
bool task_should_scx(పూర్ణాంకం policy);
bool scx_allow_ttwu_వరుస(స్థిరరాశి నిర్మాణం task_struct *p);
bool scx_allow_proxy_exec(స్థిరరాశి నిర్మాణం task_struct *p);
శూన్య scx_proxy_donor_ప్రారంభం(నిర్మాణం rq *rq);
శూన్య scx_proxy_reenqueue_retry(నిర్మాణం rq *rq, నిర్మాణం task_struct *తదుపరి);
శూన్య init_sched_ext_class(శూన్య);

స్థిర అంతఃవరుస u32 scx_cpuperf_లక్ష్యం(s32 cpu)
{
	ఒకవేళ (scx_enabled())
		తిరిగి cpu_rq(cpu)->scx.cpuperf_లక్ష్యం;
	లేకపోతే
		తిరిగి 0;
}

స్థిర అంతఃవరుస bool task_on_scx(స్థిరరాశి నిర్మాణం task_struct *p)
{
	తిరిగి scx_enabled() && p->sched_class == &ext_sched_class;
}

#నిర్వచితమైతే CONFIG_SCHED_CORE
bool scx_prio_less(స్థిరరాశి నిర్మాణం task_struct *a, స్థిరరాశి నిర్మాణం task_struct *b,
		   bool లోపల_fi);
#అయితే_ముగింపు

#లేకపోతే	/* CONFIG_SCHED_CLASS_EXT */

స్థిర అంతఃవరుస శూన్య scx_tick(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_pre_fork(నిర్మాణం task_struct *p) {}
స్థిర అంతఃవరుస పూర్ణాంకం scx_fork(నిర్మాణం task_struct *p, నిర్మాణం kernel_clone_వాదనలు *kargs) { తిరిగి 0; }
స్థిర అంతఃవరుస శూన్య scx_post_fork(నిర్మాణం task_struct *p) {}
స్థిర అంతఃవరుస శూన్య scx_cancel_fork(నిర్మాణం task_struct *p) {}
స్థిర అంతఃవరుస u32 scx_cpuperf_లక్ష్యం(s32 cpu) { తిరిగి 0; }
స్థిర అంతఃవరుస bool scx_can_stop_tick(నిర్మాణం rq *rq) { తిరిగి true; }
స్థిర అంతఃవరుస శూన్య scx_rq_activate(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_rq_deactivate(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస పూర్ణాంకం scx_check_setscheduler(నిర్మాణం task_struct *p, పూర్ణాంకం policy) { తిరిగి 0; }
స్థిర అంతఃవరుస bool task_on_scx(స్థిరరాశి నిర్మాణం task_struct *p) { తిరిగి false; }
స్థిర అంతఃవరుస bool scx_allow_ttwu_వరుస(స్థిరరాశి నిర్మాణం task_struct *p) { తిరిగి true; }
స్థిర అంతఃవరుస bool scx_allow_proxy_exec(స్థిరరాశి నిర్మాణం task_struct *p) { తిరిగి true; }
స్థిర అంతఃవరుస శూన్య scx_proxy_donor_ప్రారంభం(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య scx_proxy_reenqueue_retry(నిర్మాణం rq *rq,
					     నిర్మాణం task_struct *తదుపరి) {}
స్థిర అంతఃవరుస శూన్య init_sched_ext_class(శూన్య) {}

#అయితే_ముగింపు	/* CONFIG_SCHED_CLASS_EXT */

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
శూన్య __scx_update_idle(నిర్మాణం rq *rq, bool idle, bool do_notify);

స్థిర అంతఃవరుస శూన్య scx_update_idle(నిర్మాణం rq *rq, bool idle, bool do_notify)
{
	ఒకవేళ (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#లేకపోతే
స్థిర అంతఃవరుస శూన్య scx_update_idle(నిర్మాణం rq *rq, bool idle, bool do_notify) {}
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_CGROUP_SCHED
#నిర్వచితమైతే CONFIG_EXT_సమూహం_SCHED
శూన్య scx_tg_init(నిర్మాణం task_సమూహం *tg);
పూర్ణాంకం scx_tg_online(నిర్మాణం task_సమూహం *tg);
శూన్య scx_tg_offline(నిర్మాణం task_సమూహం *tg);
పూర్ణాంకం scx_cgroup_can_attach(నిర్మాణం cgroup_taskset *tset);
శూన్య scx_cgroup_move_task(నిర్మాణం task_struct *p);
శూన్య scx_cgroup_cancel_attach(నిర్మాణం cgroup_taskset *tset);
శూన్య scx_సమూహం_సమితి_బరువు(నిర్మాణం task_సమూహం *tg, గుర్తులేని దీర్ఘ cgrp_బరువు);
శూన్య scx_సమూహం_సమితి_idle(నిర్మాణం task_సమూహం *tg, bool idle);
శూన్య scx_సమూహం_సమితి_bandwidth(నిర్మాణం task_సమూహం *tg, u64 period_us, u64 quota_us, u64 వ్యవధి_us);
#లేకపోతే	/* CONFIG_EXT_GROUP_SCHED */
స్థిర అంతఃవరుస శూన్య scx_tg_init(నిర్మాణం task_సమూహం *tg) {}
స్థిర అంతఃవరుస పూర్ణాంకం scx_tg_online(నిర్మాణం task_సమూహం *tg) { తిరిగి 0; }
స్థిర అంతఃవరుస శూన్య scx_tg_offline(నిర్మాణం task_సమూహం *tg) {}
స్థిర అంతఃవరుస పూర్ణాంకం scx_cgroup_can_attach(నిర్మాణం cgroup_taskset *tset) { తిరిగి 0; }
స్థిర అంతఃవరుస శూన్య scx_cgroup_move_task(నిర్మాణం task_struct *p) {}
స్థిర అంతఃవరుస శూన్య scx_cgroup_cancel_attach(నిర్మాణం cgroup_taskset *tset) {}
స్థిర అంతఃవరుస శూన్య scx_సమూహం_సమితి_బరువు(నిర్మాణం task_సమూహం *tg, గుర్తులేని దీర్ఘ cgrp_బరువు) {}
స్థిర అంతఃవరుస శూన్య scx_సమూహం_సమితి_idle(నిర్మాణం task_సమూహం *tg, bool idle) {}
స్థిర అంతఃవరుస శూన్య scx_సమూహం_సమితి_bandwidth(నిర్మాణం task_సమూహం *tg, u64 period_us, u64 quota_us, u64 వ్యవధి_us) {}
#అయితే_ముగింపు	/* CONFIG_EXT_GROUP_SCHED */
#అయితే_ముగింపు	/* CONFIG_CGROUP_SCHED */
