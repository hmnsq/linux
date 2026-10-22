/* SPDX-License-Identifier: GPL-2.0 */
#جے_بنا_تعریف _KERNEL_SCHED_AUTOGROUP_H
#تعریف_کرو _KERNEL_SCHED_AUTOGROUP_H

#ضمن "sched.h"

#جے_تعریف_شدہ CONFIG_SCHED_AUTOGROUP

بنية autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	بنية kref		kref;
	بنية task_گروہ	*tg;
	بنية rw_semaphore	جندرا;
	بنانشانی لما		id;
	صحيح			nice;
};

خارجي فراغ autogroup_init(بنية task_struct *init_task);
خارجي فراغ autogroup_free(بنية task_گروہ *tg);

ساكن مضمن bool task_گروہ_is_autogroup(بنية task_گروہ *tg)
{
	أرجع !!tg->autogroup;
}

خارجي bool task_چاہندا_autogroup(بنية task_struct *p, بنية task_گروہ *tg);

ساكن مضمن بنية task_گروہ *
autogroup_task_گروہ(بنية task_struct *p, بنية task_گروہ *tg)
{
	خارجي بنانشانی صحيح sysctl_sched_autogroup_enabled;
	صحيح enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	إذا (enabled && task_چاہندا_autogroup(p, tg))
		أرجع p->إشارة->autogroup->tg;

	أرجع tg;
}

خارجي صحيح autogroup_رستہ(بنية task_گروہ *tg, حرف *buf, صحيح buflen);

#وإلا /* !CONFIG_SCHED_AUTOGROUP: */

ساكن مضمن فراغ autogroup_init(بنية task_struct *init_task) {  }
ساكن مضمن فراغ autogroup_free(بنية task_گروہ *tg) { }
ساكن مضمن bool task_گروہ_is_autogroup(بنية task_گروہ *tg)
{
	أرجع 0;
}

ساكن مضمن بنية task_گروہ *
autogroup_task_گروہ(بنية task_struct *p, بنية task_گروہ *tg)
{
	أرجع tg;
}

ساكن مضمن صحيح autogroup_رستہ(بنية task_گروہ *tg, حرف *buf, صحيح buflen)
{
	أرجع 0;
}

#جے_انت /* !CONFIG_SCHED_AUTOGROUP */

#جے_انت /* _KERNEL_SCHED_AUTOGROUP_H */
