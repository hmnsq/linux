/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#include "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

struct autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	struct kref		kref;
	struct task_group	*tg;
	struct rw_semaphore	lock;
	unsigned long		id;
	ціле			nice;
};

extern void autogroup_init(struct task_struct *init_task);
extern void autogroup_free(struct task_group *tg);

static inline bool task_group_is_autogroup(struct task_group *tg)
{
	повернути !!tg->autogroup;
}

extern bool task_wants_autogroup(struct task_struct *p, struct task_group *tg);

static inline struct task_group *
autogroup_task_group(struct task_struct *p, struct task_group *tg)
{
	extern unsigned ціле sysctl_sched_autogroup_enabled;
	ціле enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	якщо (enabled && task_wants_autogroup(p, tg))
		повернути p->signal->autogroup->tg;

	повернути tg;
}

extern ціле autogroup_path(struct task_group *tg, char *buf, ціле buflen);

#інакше /* !CONFIG_SCHED_AUTOGROUP: */

static inline void autogroup_init(struct task_struct *init_task) {  }
static inline void autogroup_free(struct task_group *tg) { }
static inline bool task_group_is_autogroup(struct task_group *tg)
{
	повернути 0;
}

static inline struct task_group *
autogroup_task_group(struct task_struct *p, struct task_group *tg)
{
	повернути tg;
}

static inline ціле autogroup_path(struct task_group *tg, char *buf, ціле buflen)
{
	повернути 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
