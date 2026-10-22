/* SPDX-License-Identifier: GPL-2.0 */
#جيڪڏهن_وصف_نه_ٿيل _KERNEL_SCHED_AUTOGROUP_H
#وصف_ڪريو _KERNEL_SCHED_AUTOGROUP_H

#ضمن "sched.h"

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_AUTOGROUP

بنية autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	بنية kref		kref;
	بنية task_گروهه	*tg;
	بنية rw_semaphore	تالو;
	بنانشان ڊگهو		id;
	صحيح			nice;
};

خارجي فراغ autogroup_init(بنية task_struct *init_task);
خارجي فراغ autogroup_free(بنية task_گروهه *tg);

ساكن مضمن bool task_گروهه_is_autogroup(بنية task_گروهه *tg)
{
	أرجع !!tg->autogroup;
}

خارجي bool task_چاهي_autogroup(بنية task_struct *p, بنية task_گروهه *tg);

ساكن مضمن بنية task_گروهه *
autogroup_task_گروهه(بنية task_struct *p, بنية task_گروهه *tg)
{
	خارجي بنانشان صحيح sysctl_sched_autogroup_enabled;
	صحيح enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	إذا (enabled && task_چاهي_autogroup(p, tg))
		أرجع p->إشارة->autogroup->tg;

	أرجع tg;
}

خارجي صحيح autogroup_رستو(بنية task_گروهه *tg, حرف *buf, صحيح buflen);

#وإلا /* !CONFIG_SCHED_AUTOGROUP: */

ساكن مضمن فراغ autogroup_init(بنية task_struct *init_task) {  }
ساكن مضمن فراغ autogroup_free(بنية task_گروهه *tg) { }
ساكن مضمن bool task_گروهه_is_autogroup(بنية task_گروهه *tg)
{
	أرجع 0;
}

ساكن مضمن بنية task_گروهه *
autogroup_task_گروهه(بنية task_struct *p, بنية task_گروهه *tg)
{
	أرجع tg;
}

ساكن مضمن صحيح autogroup_رستو(بنية task_گروهه *tg, حرف *buf, صحيح buflen)
{
	أرجع 0;
}

#جيڪڏهن_پورو /* !CONFIG_SCHED_AUTOGROUP */

#جيڪڏهن_پورو /* _KERNEL_SCHED_AUTOGROUP_H */
