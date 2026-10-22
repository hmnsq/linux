/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#അന്തര്ഭാവ "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

ഘടന autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	ഘടന kref		kref;
	ഘടന task_group	*tg;
	ഘടന rw_semaphore	lock;
	unsigned long		id;
	പൂർണ്ണസംഖ്യ			nice;
};

ബാഹ്യ ശൂന്യം autogroup_init(ഘടന task_struct *init_task);
ബാഹ്യ ശൂന്യം autogroup_free(ഘടന task_group *tg);

സ്ഥിതം വരിയിൽ bool task_group_is_autogroup(ഘടന task_group *tg)
{
	തിരികെ !!tg->autogroup;
}

ബാഹ്യ bool task_wants_autogroup(ഘടന task_struct *p, ഘടന task_group *tg);

സ്ഥിതം വരിയിൽ ഘടന task_group *
autogroup_task_group(ഘടന task_struct *p, ഘടന task_group *tg)
{
	ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_sched_autogroup_enabled;
	പൂർണ്ണസംഖ്യ enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	എങ്കിൽ (enabled && task_wants_autogroup(p, tg))
		തിരികെ p->സൂചന->autogroup->tg;

	തിരികെ tg;
}

ബാഹ്യ പൂർണ്ണസംഖ്യ autogroup_path(ഘടന task_group *tg, അക്ഷരം *buf, പൂർണ്ണസംഖ്യ buflen);

#അല്ലെങ്കിൽ /* !CONFIG_SCHED_AUTOGROUP: */

സ്ഥിതം വരിയിൽ ശൂന്യം autogroup_init(ഘടന task_struct *init_task) {  }
സ്ഥിതം വരിയിൽ ശൂന്യം autogroup_free(ഘടന task_group *tg) { }
സ്ഥിതം വരിയിൽ bool task_group_is_autogroup(ഘടന task_group *tg)
{
	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ ഘടന task_group *
autogroup_task_group(ഘടന task_struct *p, ഘടന task_group *tg)
{
	തിരികെ tg;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ autogroup_path(ഘടന task_group *tg, അക്ഷരം *buf, പൂർണ്ണസംഖ്യ buflen)
{
	തിരികെ 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
