/* SPDX-License-Identifier: GPL-2.0 */
#اگر_غیر_تعریف_شدہ _KERNEL_SCHED_AUTOGROUP_H
#تعریف_کرو _KERNEL_SCHED_AUTOGROUP_H

#شامل "sched.h"

#اگر_تعریف_شدہ CONFIG_SCHED_AUTOGROUP

ساخت autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	ساخت kref		kref;
	ساخت task_گروہ	*tg;
	ساخت rw_semaphore	تالا;
	غیرعلامتی طویل		id;
	صحیح			nice;
};

خارجی خالی autogroup_init(ساخت task_struct *init_task);
خارجی خالی autogroup_free(ساخت task_گروہ *tg);

ساکن اندرونی bool task_گروہ_is_autogroup(ساخت task_گروہ *tg)
{
	واپس !!tg->autogroup;
}

خارجی bool task_چاہتا_autogroup(ساخت task_struct *p, ساخت task_گروہ *tg);

ساکن اندرونی ساخت task_گروہ *
autogroup_task_گروہ(ساخت task_struct *p, ساخت task_گروہ *tg)
{
	خارجی غیرعلامتی صحیح sysctl_sched_autogroup_enabled;
	صحیح enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	اگر (enabled && task_چاہتا_autogroup(p, tg))
		واپس p->اشارہ->autogroup->tg;

	واپس tg;
}

خارجی صحیح autogroup_راستہ(ساخت task_گروہ *tg, حرف *buf, صحیح buflen);

#ورنہ /* !CONFIG_SCHED_AUTOGROUP: */

ساکن اندرونی خالی autogroup_init(ساخت task_struct *init_task) {  }
ساکن اندرونی خالی autogroup_free(ساخت task_گروہ *tg) { }
ساکن اندرونی bool task_گروہ_is_autogroup(ساخت task_گروہ *tg)
{
	واپس 0;
}

ساکن اندرونی ساخت task_گروہ *
autogroup_task_گروہ(ساخت task_struct *p, ساخت task_گروہ *tg)
{
	واپس tg;
}

ساکن اندرونی صحیح autogroup_راستہ(ساخت task_گروہ *tg, حرف *buf, صحیح buflen)
{
	واپس 0;
}

#اگر_ختم /* !CONFIG_SCHED_AUTOGROUP */

#اگر_ختم /* _KERNEL_SCHED_AUTOGROUP_H */
