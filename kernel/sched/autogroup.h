/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#अन्तर्भाव "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

संरचना autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	संरचना kref		kref;
	संरचना task_group	*tg;
	संरचना rw_semaphore	lock;
	unsigned long		id;
	पूर्णाङ्क			nice;
};

बाह्य शून्य autogroup_init(संरचना task_struct *init_task);
बाह्य शून्य autogroup_free(संरचना task_group *tg);

स्थिर अन्तःस्थ bool task_group_is_autogroup(संरचना task_group *tg)
{
	पटिनिवत्त !!tg->autogroup;
}

बाह्य bool task_wants_autogroup(संरचना task_struct *p, संरचना task_group *tg);

स्थिर अन्तःस्थ संरचना task_group *
autogroup_task_group(संरचना task_struct *p, संरचना task_group *tg)
{
	बाह्य unsigned पूर्णाङ्क sysctl_sched_autogroup_enabled;
	पूर्णाङ्क enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	सचे (enabled && task_wants_autogroup(p, tg))
		पटिनिवत्त p->सङ्केत->autogroup->tg;

	पटिनिवत्त tg;
}

बाह्य पूर्णाङ्क autogroup_path(संरचना task_group *tg, वर्ण *buf, पूर्णाङ्क buflen);

#अञ्ञथा /* !CONFIG_SCHED_AUTOGROUP: */

स्थिर अन्तःस्थ शून्य autogroup_init(संरचना task_struct *init_task) {  }
स्थिर अन्तःस्थ शून्य autogroup_free(संरचना task_group *tg) { }
स्थिर अन्तःस्थ bool task_group_is_autogroup(संरचना task_group *tg)
{
	पटिनिवत्त 0;
}

स्थिर अन्तःस्थ संरचना task_group *
autogroup_task_group(संरचना task_struct *p, संरचना task_group *tg)
{
	पटिनिवत्त tg;
}

स्थिर अन्तःस्थ पूर्णाङ्क autogroup_path(संरचना task_group *tg, वर्ण *buf, पूर्णाङ्क buflen)
{
	पटिनिवत्त 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
