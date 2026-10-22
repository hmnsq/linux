/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#அந்தர்பாவ "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

கட்டமைப்பு autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	கட்டமைப்பு kref		kref;
	கட்டமைப்பு task_group	*tg;
	கட்டமைப்பு rw_semaphore	lock;
	unsigned long		id;
	முழு			nice;
};

வெளி வெறுமை autogroup_init(கட்டமைப்பு task_struct *init_task);
வெளி வெறுமை autogroup_free(கட்டமைப்பு task_group *tg);

நிலையான உள்வரி bool task_group_is_autogroup(கட்டமைப்பு task_group *tg)
{
	திரும்பு !!tg->autogroup;
}

வெளி bool task_wants_autogroup(கட்டமைப்பு task_struct *p, கட்டமைப்பு task_group *tg);

நிலையான உள்வரி கட்டமைப்பு task_group *
autogroup_task_group(கட்டமைப்பு task_struct *p, கட்டமைப்பு task_group *tg)
{
	வெளி unsigned முழு sysctl_sched_autogroup_enabled;
	முழு enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	எனில் (enabled && task_wants_autogroup(p, tg))
		திரும்பு p->சமிக்ஞை->autogroup->tg;

	திரும்பு tg;
}

வெளி முழு autogroup_path(கட்டமைப்பு task_group *tg, எழுத்து *buf, முழு buflen);

#இல்லையெனில் /* !CONFIG_SCHED_AUTOGROUP: */

நிலையான உள்வரி வெறுமை autogroup_init(கட்டமைப்பு task_struct *init_task) {  }
நிலையான உள்வரி வெறுமை autogroup_free(கட்டமைப்பு task_group *tg) { }
நிலையான உள்வரி bool task_group_is_autogroup(கட்டமைப்பு task_group *tg)
{
	திரும்பு 0;
}

நிலையான உள்வரி கட்டமைப்பு task_group *
autogroup_task_group(கட்டமைப்பு task_struct *p, கட்டமைப்பு task_group *tg)
{
	திரும்பு tg;
}

நிலையான உள்வரி முழு autogroup_path(கட்டமைப்பு task_group *tg, எழுத்து *buf, முழு buflen)
{
	திரும்பு 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
