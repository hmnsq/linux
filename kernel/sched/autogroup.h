/* SPDX-License-Identifier: GPL-2.0 */
#که_نه_تعریف_شوی _KERNEL_SCHED_AUTOGROUP_H
#تعریف_کړه _KERNEL_SCHED_AUTOGROUP_H

#ضمن "sched.h"

#که_تعریف_شوی CONFIG_SCHED_AUTOGROUP

بنية autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	بنية kref		kref;
	بنية task_ډله	*tg;
	بنية rw_semaphore	قلف;
	بینښه اوږد		id;
	صحيح			nice;
};

خارجي فراغ autogroup_init(بنية task_struct *init_task);
خارجي فراغ autogroup_free(بنية task_ډله *tg);

ساكن مضمن bool task_ډله_is_autogroup(بنية task_ډله *tg)
{
	أرجع !!tg->autogroup;
}

خارجي bool task_غواړي_autogroup(بنية task_struct *p, بنية task_ډله *tg);

ساكن مضمن بنية task_ډله *
autogroup_task_ډله(بنية task_struct *p, بنية task_ډله *tg)
{
	خارجي بینښه صحيح sysctl_sched_autogroup_enabled;
	صحيح enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	إذا (enabled && task_غواړي_autogroup(p, tg))
		أرجع p->إشارة->autogroup->tg;

	أرجع tg;
}

خارجي صحيح autogroup_لار(بنية task_ډله *tg, حرف *buf, صحيح buflen);

#وإلا /* !CONFIG_SCHED_AUTOGROUP: */

ساكن مضمن فراغ autogroup_init(بنية task_struct *init_task) {  }
ساكن مضمن فراغ autogroup_free(بنية task_ډله *tg) { }
ساكن مضمن bool task_ډله_is_autogroup(بنية task_ډله *tg)
{
	أرجع 0;
}

ساكن مضمن بنية task_ډله *
autogroup_task_ډله(بنية task_struct *p, بنية task_ډله *tg)
{
	أرجع tg;
}

ساكن مضمن صحيح autogroup_لار(بنية task_ډله *tg, حرف *buf, صحيح buflen)
{
	أرجع 0;
}

#د_که_پای /* !CONFIG_SCHED_AUTOGROUP */

#د_که_پای /* _KERNEL_SCHED_AUTOGROUP_H */
