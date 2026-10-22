/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#شامل "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

ساختار autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	ساختار kref		kref;
	ساختار task_group	*tg;
	ساختار rw_semaphore	lock;
	unsigned long		id;
	صحیح			nice;
};

بیرونی تهی autogroup_init(ساختار task_struct *init_task);
بیرونی تهی autogroup_free(ساختار task_group *tg);

ایستا درونخطی bool task_group_is_autogroup(ساختار task_group *tg)
{
	بازگشت !!tg->autogroup;
}

بیرونی bool task_wants_autogroup(ساختار task_struct *p, ساختار task_group *tg);

ایستا درونخطی ساختار task_group *
autogroup_task_group(ساختار task_struct *p, ساختار task_group *tg)
{
	بیرونی unsigned صحیح sysctl_sched_autogroup_enabled;
	صحیح enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	اگر (enabled && task_wants_autogroup(p, tg))
		بازگشت p->سیگنال->autogroup->tg;

	بازگشت tg;
}

بیرونی صحیح autogroup_path(ساختار task_group *tg, نویسه *buf, صحیح buflen);

#وگرنه /* !CONFIG_SCHED_AUTOGROUP: */

ایستا درونخطی تهی autogroup_init(ساختار task_struct *init_task) {  }
ایستا درونخطی تهی autogroup_free(ساختار task_group *tg) { }
ایستا درونخطی bool task_group_is_autogroup(ساختار task_group *tg)
{
	بازگشت 0;
}

ایستا درونخطی ساختار task_group *
autogroup_task_group(ساختار task_struct *p, ساختار task_group *tg)
{
	بازگشت tg;
}

ایستا درونخطی صحیح autogroup_path(ساختار task_group *tg, نویسه *buf, صحیح buflen)
{
	بازگشت 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
