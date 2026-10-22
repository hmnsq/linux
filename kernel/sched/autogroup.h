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
	puur_naa_nka			nice;
};

extern void autogroup_init(sa_nracanaa task_struct *init_task);
extern void autogroup_free(sa_nracanaa task_group *tg);

static anta_hstha bool task_group_is_autogroup(sa_nracanaa task_group *tg)
{
	ᰀᰧᰌ !!tg->autogroup;
}

extern bool task_wants_autogroup(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg);

static anta_hstha sa_nracanaa task_group *
autogroup_task_group(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg)
{
	extern unsigned puur_naa_nka sysctl_sched_autogroup_enabled;
	puur_naa_nka enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ᰀᰧ (enabled && task_wants_autogroup(p, tg))
		ᰀᰧᰌ p->sa_nkeeta->autogroup->tg;

	ᰀᰧᰌ tg;
}

extern puur_naa_nka autogroup_path(sa_nracanaa task_group *tg, var_na *buf, puur_naa_nka buflen);

#ᰀᰧᰀ /* !CONFIG_SCHED_AUTOGROUP: */

static anta_hstha void autogroup_init(sa_nracanaa task_struct *init_task) {  }
static anta_hstha void autogroup_free(sa_nracanaa task_group *tg) { }
static anta_hstha bool task_group_is_autogroup(sa_nracanaa task_group *tg)
{
	ᰀᰧᰌ 0;
}

static anta_hstha sa_nracanaa task_group *
autogroup_task_group(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg)
{
	ᰀᰧᰌ tg;
}

static anta_hstha puur_naa_nka autogroup_path(sa_nracanaa task_group *tg, var_na *buf, puur_naa_nka buflen)
{
	ᰀᰧᰌ 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
