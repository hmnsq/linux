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
	अचिन्हित दीर्घ		id;
	पूर्णांक			nice;
};

बाह्य शून्य autogroup_init(संरचना task_struct *init_task);
बाह्य शून्य autogroup_free(संरचना task_group *tg);

स्थिर ओळीत bool task_group_is_autogroup(संरचना task_group *tg)
{
	परत !!tg->autogroup;
}

बाह्य bool task_wants_autogroup(संरचना task_struct *p, संरचना task_group *tg);

स्थिर ओळीत संरचना task_group *
autogroup_task_group(संरचना task_struct *p, संरचना task_group *tg)
{
	बाह्य अचिन्हित पूर्णांक sysctl_sched_autogroup_enabled;
	पूर्णांक enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	जर (enabled && task_wants_autogroup(p, tg))
		परत p->संकेत->autogroup->tg;

	परत tg;
}

बाह्य पूर्णांक autogroup_path(संरचना task_group *tg, अक्षर *buf, पूर्णांक buflen);

#अन्यथा /* !CONFIG_SCHED_AUTOGROUP: */

स्थिर ओळीत शून्य autogroup_init(संरचना task_struct *init_task) {  }
स्थिर ओळीत शून्य autogroup_free(संरचना task_group *tg) { }
स्थिर ओळीत bool task_group_is_autogroup(संरचना task_group *tg)
{
	परत 0;
}

स्थिर ओळीत संरचना task_group *
autogroup_task_group(संरचना task_struct *p, संरचना task_group *tg)
{
	परत tg;
}

स्थिर ओळीत पूर्णांक autogroup_path(संरचना task_group *tg, अक्षर *buf, पूर्णांक buflen)
{
	परत 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
