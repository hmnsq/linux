/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#অন্তর্ভাব "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

কাঠামো autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	কাঠামো kref		kref;
	কাঠামো task_group	*tg;
	কাঠামো rw_semaphore	lock;
	unsigned long		id;
	পূর্ণসংখ্যা			nice;
};

বাহ্যিক শূন্য autogroup_init(কাঠামো task_struct *init_task);
বাহ্যিক শূন্য autogroup_free(কাঠামো task_group *tg);

স্থির অন্তঃরেখ bool task_group_is_autogroup(কাঠামো task_group *tg)
{
	ফেরত !!tg->autogroup;
}

বাহ্যিক bool task_wants_autogroup(কাঠামো task_struct *p, কাঠামো task_group *tg);

স্থির অন্তঃরেখ কাঠামো task_group *
autogroup_task_group(কাঠামো task_struct *p, কাঠামো task_group *tg)
{
	বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_sched_autogroup_enabled;
	পূর্ণসংখ্যা enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	যদি (enabled && task_wants_autogroup(p, tg))
		ফেরত p->সংকেত->autogroup->tg;

	ফেরত tg;
}

বাহ্যিক পূর্ণসংখ্যা autogroup_path(কাঠামো task_group *tg, অক্ষর *buf, পূর্ণসংখ্যা buflen);

#অন্যথা /* !CONFIG_SCHED_AUTOGROUP: */

স্থির অন্তঃরেখ শূন্য autogroup_init(কাঠামো task_struct *init_task) {  }
স্থির অন্তঃরেখ শূন্য autogroup_free(কাঠামো task_group *tg) { }
স্থির অন্তঃরেখ bool task_group_is_autogroup(কাঠামো task_group *tg)
{
	ফেরত 0;
}

স্থির অন্তঃরেখ কাঠামো task_group *
autogroup_task_group(কাঠামো task_struct *p, কাঠামো task_group *tg)
{
	ফেরত tg;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা autogroup_path(কাঠামো task_group *tg, অক্ষর *buf, পূর্ণসংখ্যা buflen)
{
	ফেরত 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
