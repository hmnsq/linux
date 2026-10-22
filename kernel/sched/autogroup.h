/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#ضمن "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

بنية autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	بنية kref		kref;
	بنية task_group	*tg;
	بنية rw_semaphore	lock;
	unsigned long		id;
	صحيح			nice;
};

خارجي فراغ autogroup_init(بنية task_struct *init_task);
خارجي فراغ autogroup_free(بنية task_group *tg);

ساكن مضمن bool task_group_is_autogroup(بنية task_group *tg)
{
	ܦܢܐ !!tg->autogroup;
}

خارجي bool task_wants_autogroup(بنية task_struct *p, بنية task_group *tg);

ساكن مضمن بنية task_group *
autogroup_task_group(بنية task_struct *p, بنية task_group *tg)
{
	خارجي unsigned صحيح sysctl_sched_autogroup_enabled;
	صحيح enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ܐܢ (enabled && task_wants_autogroup(p, tg))
		ܦܢܐ p->إشارة->autogroup->tg;

	ܦܢܐ tg;
}

خارجي صحيح autogroup_path(بنية task_group *tg, حرف *buf, صحيح buflen);

#ܐܚܪܢܐ /* !CONFIG_SCHED_AUTOGROUP: */

ساكن مضمن فراغ autogroup_init(بنية task_struct *init_task) {  }
ساكن مضمن فراغ autogroup_free(بنية task_group *tg) { }
ساكن مضمن bool task_group_is_autogroup(بنية task_group *tg)
{
	ܦܢܐ 0;
}

ساكن مضمن بنية task_group *
autogroup_task_group(بنية task_struct *p, بنية task_group *tg)
{
	ܦܢܐ tg;
}

ساكن مضمن صحيح autogroup_path(بنية task_group *tg, حرف *buf, صحيح buflen)
{
	ܦܢܐ 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
