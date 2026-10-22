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
	ចំនួនគត់			nice;
};

extern void autogroup_init(sa_nracanaa task_struct *init_task);
extern void autogroup_free(sa_nracanaa task_group *tg);

static anta_hstha bool task_group_is_autogroup(sa_nracanaa task_group *tg)
{
	ត្រឡប់ !!tg->autogroup;
}

extern bool task_wants_autogroup(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg);

static anta_hstha sa_nracanaa task_group *
autogroup_task_group(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg)
{
	extern unsigned ចំនួនគត់ sysctl_sched_autogroup_enabled;
	ចំនួនគត់ enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ប្រសិនបើ (enabled && task_wants_autogroup(p, tg))
		ត្រឡប់ p->sa_nkeeta->autogroup->tg;

	ត្រឡប់ tg;
}

extern ចំនួនគត់ autogroup_path(sa_nracanaa task_group *tg, var_na *buf, ចំនួនគត់ buflen);

#បើមិនដូច្នេះ /* !CONFIG_SCHED_AUTOGROUP: */

static anta_hstha void autogroup_init(sa_nracanaa task_struct *init_task) {  }
static anta_hstha void autogroup_free(sa_nracanaa task_group *tg) { }
static anta_hstha bool task_group_is_autogroup(sa_nracanaa task_group *tg)
{
	ត្រឡប់ 0;
}

static anta_hstha sa_nracanaa task_group *
autogroup_task_group(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg)
{
	ត្រឡប់ tg;
}

static anta_hstha ចំនួនគត់ autogroup_path(sa_nracanaa task_group *tg, var_na *buf, ចំនួនគត់ buflen)
{
	ត្រឡប់ 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
