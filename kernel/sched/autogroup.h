/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#include "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

sa_nracanaa autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	sa_nracanaa kref		kref;
	sa_nracanaa task_group	*tg;
	sa_nracanaa rw_semaphore	lock;
	unsigned long		id;
	ཧྲིལ			nice;
};

extern void autogroup_init(sa_nracanaa task_struct *init_task);
extern void autogroup_free(sa_nracanaa task_group *tg);

static anta_hstha bool task_group_is_autogroup(sa_nracanaa task_group *tg)
{
	ཕྱིར !!tg->autogroup;
}

extern bool task_wants_autogroup(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg);

static anta_hstha sa_nracanaa task_group *
autogroup_task_group(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg)
{
	extern unsigned ཧྲིལ sysctl_sched_autogroup_enabled;
	ཧྲིལ enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	གལ་ཏེ (enabled && task_wants_autogroup(p, tg))
		ཕྱིར p->sa_nkeeta->autogroup->tg;

	ཕྱིར tg;
}

extern ཧྲིལ autogroup_path(sa_nracanaa task_group *tg, var_na *buf, ཧྲིལ buflen);

#དེ་མིན /* !CONFIG_SCHED_AUTOGROUP: */

static anta_hstha void autogroup_init(sa_nracanaa task_struct *init_task) {  }
static anta_hstha void autogroup_free(sa_nracanaa task_group *tg) { }
static anta_hstha bool task_group_is_autogroup(sa_nracanaa task_group *tg)
{
	ཕྱིར 0;
}

static anta_hstha sa_nracanaa task_group *
autogroup_task_group(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg)
{
	ཕྱིར tg;
}

static anta_hstha ཧྲིལ autogroup_path(sa_nracanaa task_group *tg, var_na *buf, ཧྲིལ buflen)
{
	ཕྱིར 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
