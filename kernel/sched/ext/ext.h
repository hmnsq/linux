/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Copyright (c) 2022 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2022 Tejun Heo <tj@kernel.org>
 * Copyright (c) 2022 David Vernet <dvernet@meta.com>
 */
#अगर_घोषित CONFIG_SCHED_CLASS_EXT

व्योम scx_tick(काष्ठा rq *rq);
व्योम init_scx_entity(काष्ठा sched_ext_entity *scx);
व्योम scx_pre_fork(काष्ठा task_struct *p);
पूर्णांक scx_fork(काष्ठा task_struct *p, काष्ठा kernel_clone_args *kargs);
व्योम scx_post_fork(काष्ठा task_struct *p);
व्योम scx_cancel_fork(काष्ठा task_struct *p);
bool scx_can_stop_tick(काष्ठा rq *rq);
व्योम scx_rq_activate(काष्ठा rq *rq);
व्योम scx_rq_deactivate(काष्ठा rq *rq);
पूर्णांक scx_check_setscheduler(काष्ठा task_struct *p, पूर्णांक policy);
bool task_should_scx(पूर्णांक policy);
bool scx_allow_ttwu_queue(स्थिर काष्ठा task_struct *p);
bool scx_allow_proxy_exec(स्थिर काष्ठा task_struct *p);
व्योम scx_proxy_donor_start(काष्ठा rq *rq);
व्योम scx_proxy_reenqueue_retry(काष्ठा rq *rq, काष्ठा task_struct *अग्र);
व्योम init_sched_ext_class(व्योम);

अटल अंतरभूत u32 scx_cpuperf_target(s32 cpu)
{
	अगर (scx_enabled())
		वापस cpu_rq(cpu)->scx.cpuperf_target;
	अन्यथा
		वापस 0;
}

अटल अंतरभूत bool task_on_scx(स्थिर काष्ठा task_struct *p)
{
	वापस scx_enabled() && p->sched_class == &ext_sched_class;
}

#अगर_घोषित CONFIG_SCHED_CORE
bool scx_prio_less(स्थिर काष्ठा task_struct *a, स्थिर काष्ठा task_struct *b,
		   bool in_fi);
#पूर्ण_अगर

#अन्यथा	/* CONFIG_SCHED_CLASS_EXT */

अटल अंतरभूत व्योम scx_tick(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_pre_fork(काष्ठा task_struct *p) {}
अटल अंतरभूत पूर्णांक scx_fork(काष्ठा task_struct *p, काष्ठा kernel_clone_args *kargs) { वापस 0; }
अटल अंतरभूत व्योम scx_post_fork(काष्ठा task_struct *p) {}
अटल अंतरभूत व्योम scx_cancel_fork(काष्ठा task_struct *p) {}
अटल अंतरभूत u32 scx_cpuperf_target(s32 cpu) { वापस 0; }
अटल अंतरभूत bool scx_can_stop_tick(काष्ठा rq *rq) { वापस true; }
अटल अंतरभूत व्योम scx_rq_activate(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_rq_deactivate(काष्ठा rq *rq) {}
अटल अंतरभूत पूर्णांक scx_check_setscheduler(काष्ठा task_struct *p, पूर्णांक policy) { वापस 0; }
अटल अंतरभूत bool task_on_scx(स्थिर काष्ठा task_struct *p) { वापस false; }
अटल अंतरभूत bool scx_allow_ttwu_queue(स्थिर काष्ठा task_struct *p) { वापस true; }
अटल अंतरभूत bool scx_allow_proxy_exec(स्थिर काष्ठा task_struct *p) { वापस true; }
अटल अंतरभूत व्योम scx_proxy_donor_start(काष्ठा rq *rq) {}
अटल अंतरभूत व्योम scx_proxy_reenqueue_retry(काष्ठा rq *rq,
					     काष्ठा task_struct *अग्र) {}
अटल अंतरभूत व्योम init_sched_ext_class(व्योम) {}

#पूर्ण_अगर	/* CONFIG_SCHED_CLASS_EXT */

#अगर_घोषित CONFIG_SCHED_CLASS_EXT
व्योम __scx_update_idle(काष्ठा rq *rq, bool idle, bool do_notify);

अटल अंतरभूत व्योम scx_update_idle(काष्ठा rq *rq, bool idle, bool do_notify)
{
	अगर (scx_enabled())
		__scx_update_idle(rq, idle, do_notify);
}
#अन्यथा
अटल अंतरभूत व्योम scx_update_idle(काष्ठा rq *rq, bool idle, bool do_notify) {}
#पूर्ण_अगर

#अगर_घोषित CONFIG_CGROUP_SCHED
#अगर_घोषित CONFIG_EXT_GROUP_SCHED
व्योम scx_tg_init(काष्ठा task_group *tg);
पूर्णांक scx_tg_online(काष्ठा task_group *tg);
व्योम scx_tg_offline(काष्ठा task_group *tg);
पूर्णांक scx_cgroup_can_attach(काष्ठा cgroup_taskset *tset);
व्योम scx_cgroup_move_task(काष्ठा task_struct *p);
व्योम scx_cgroup_cancel_attach(काष्ठा cgroup_taskset *tset);
व्योम scx_group_set_weight(काष्ठा task_group *tg, अचिन्हित दीर्घ cgrp_weight);
व्योम scx_group_set_idle(काष्ठा task_group *tg, bool idle);
व्योम scx_group_set_bandwidth(काष्ठा task_group *tg, u64 period_us, u64 quota_us, u64 burst_us);
#अन्यथा	/* CONFIG_EXT_GROUP_SCHED */
अटल अंतरभूत व्योम scx_tg_init(काष्ठा task_group *tg) {}
अटल अंतरभूत पूर्णांक scx_tg_online(काष्ठा task_group *tg) { वापस 0; }
अटल अंतरभूत व्योम scx_tg_offline(काष्ठा task_group *tg) {}
अटल अंतरभूत पूर्णांक scx_cgroup_can_attach(काष्ठा cgroup_taskset *tset) { वापस 0; }
अटल अंतरभूत व्योम scx_cgroup_move_task(काष्ठा task_struct *p) {}
अटल अंतरभूत व्योम scx_cgroup_cancel_attach(काष्ठा cgroup_taskset *tset) {}
अटल अंतरभूत व्योम scx_group_set_weight(काष्ठा task_group *tg, अचिन्हित दीर्घ cgrp_weight) {}
अटल अंतरभूत व्योम scx_group_set_idle(काष्ठा task_group *tg, bool idle) {}
अटल अंतरभूत व्योम scx_group_set_bandwidth(काष्ठा task_group *tg, u64 period_us, u64 quota_us, u64 burst_us) {}
#पूर्ण_अगर	/* CONFIG_EXT_GROUP_SCHED */
#पूर्ण_अगर	/* CONFIG_CGROUP_SCHED */
