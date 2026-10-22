/* SPDX-License-Identifier: GPL-2.0 */
#اگر_تعریف_نشده _KERNEL_SCHED_AUTOGROUP_H
#تعریف_کن _KERNEL_SCHED_AUTOGROUP_H

#ضمن "sched.h"

#اگر_تعریف_شده CONFIG_SCHED_AUTOGROUP

بنية autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	بنية kref		kref;
	بنية task_گروه	*tg;
	بنية rw_semaphore	قفل;
	بیعلامت دراز		id;
	صحيح			nice;
};

خارجي فراغ autogroup_init(بنية task_struct *init_task);
خارجي فراغ autogroup_free(بنية task_گروه *tg);

ساكن مضمن bool task_گروه_is_autogroup(بنية task_گروه *tg)
{
	أرجع !!tg->autogroup;
}

خارجي bool task_میخواهد_autogroup(بنية task_struct *p, بنية task_گروه *tg);

ساكن مضمن بنية task_گروه *
autogroup_task_گروه(بنية task_struct *p, بنية task_گروه *tg)
{
	خارجي بیعلامت صحيح sysctl_sched_autogroup_enabled;
	صحيح enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	إذا (enabled && task_میخواهد_autogroup(p, tg))
		أرجع p->إشارة->autogroup->tg;

	أرجع tg;
}

خارجي صحيح autogroup_مسیر(بنية task_گروه *tg, حرف *buf, صحيح buflen);

#وإلا /* !CONFIG_SCHED_AUTOGROUP: */

ساكن مضمن فراغ autogroup_init(بنية task_struct *init_task) {  }
ساكن مضمن فراغ autogroup_free(بنية task_گروه *tg) { }
ساكن مضمن bool task_گروه_is_autogroup(بنية task_گروه *tg)
{
	أرجع 0;
}

ساكن مضمن بنية task_گروه *
autogroup_task_گروه(بنية task_struct *p, بنية task_گروه *tg)
{
	أرجع tg;
}

ساكن مضمن صحيح autogroup_مسیر(بنية task_گروه *tg, حرف *buf, صحيح buflen)
{
	أرجع 0;
}

#پایان_اگر /* !CONFIG_SCHED_AUTOGROUP */

#پایان_اگر /* _KERNEL_SCHED_AUTOGROUP_H */
