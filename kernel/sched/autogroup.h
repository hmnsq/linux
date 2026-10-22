/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#অন্তর্ভাব "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

সংরচনা autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	সংরচনা kref		kref;
	সংরচনা task_group	*tg;
	সংরচনা rw_semaphore	lock;
	unsigned long		id;
	পূর্ণাঙ্ক			nice;
};

বাহ্য শূন্য autogroup_init(সংরচনা task_struct *init_task);
বাহ্য শূন্য autogroup_free(সংরচনা task_group *tg);

স্থির অন্তঃস্থ bool task_group_is_autogroup(সংরচনা task_group *tg)
{
	প্রত্যাগম !!tg->autogroup;
}

বাহ্য bool task_wants_autogroup(সংরচনা task_struct *p, সংরচনা task_group *tg);

স্থির অন্তঃস্থ সংরচনা task_group *
autogroup_task_group(সংরচনা task_struct *p, সংরচনা task_group *tg)
{
	বাহ্য unsigned পূর্ণাঙ্ক sysctl_sched_autogroup_enabled;
	পূর্ণাঙ্ক enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	যদি (enabled && task_wants_autogroup(p, tg))
		প্রত্যাগম p->সঙ্কেত->autogroup->tg;

	প্রত্যাগম tg;
}

বাহ্য পূর্ণাঙ্ক autogroup_path(সংরচনা task_group *tg, বর্ণ *buf, পূর্ণাঙ্ক buflen);

#অন্যথা /* !CONFIG_SCHED_AUTOGROUP: */

স্থির অন্তঃস্থ শূন্য autogroup_init(সংরচনা task_struct *init_task) {  }
স্থির অন্তঃস্থ শূন্য autogroup_free(সংরচনা task_group *tg) { }
স্থির অন্তঃস্থ bool task_group_is_autogroup(সংরচনা task_group *tg)
{
	প্রত্যাগম 0;
}

স্থির অন্তঃস্থ সংরচনা task_group *
autogroup_task_group(সংরচনা task_struct *p, সংরচনা task_group *tg)
{
	প্রত্যাগম tg;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক autogroup_path(সংরচনা task_group *tg, বর্ণ *buf, পূর্ণাঙ্ক buflen)
{
	প্রত্যাগম 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
