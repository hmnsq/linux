/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#અન્તર્ભાવ "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

સંરચના autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	સંરચના kref		kref;
	સંરચના task_group	*tg;
	સંરચના rw_semaphore	lock;
	unsigned long		id;
	પૂર્ણાંક			nice;
};

બાહ્ય શૂન્ય autogroup_init(સંરચના task_struct *init_task);
બાહ્ય શૂન્ય autogroup_free(સંરચના task_group *tg);

સ્થિર અંતઃરેખ bool task_group_is_autogroup(સંરચના task_group *tg)
{
	પાછા !!tg->autogroup;
}

બાહ્ય bool task_wants_autogroup(સંરચના task_struct *p, સંરચના task_group *tg);

સ્થિર અંતઃરેખ સંરચના task_group *
autogroup_task_group(સંરચના task_struct *p, સંરચના task_group *tg)
{
	બાહ્ય unsigned પૂર્ણાંક sysctl_sched_autogroup_enabled;
	પૂર્ણાંક enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	જો (enabled && task_wants_autogroup(p, tg))
		પાછા p->સંકેત->autogroup->tg;

	પાછા tg;
}

બાહ્ય પૂર્ણાંક autogroup_path(સંરચના task_group *tg, અક્ષર *buf, પૂર્ણાંક buflen);

#નહીંતર /* !CONFIG_SCHED_AUTOGROUP: */

સ્થિર અંતઃરેખ શૂન્ય autogroup_init(સંરચના task_struct *init_task) {  }
સ્થિર અંતઃરેખ શૂન્ય autogroup_free(સંરચના task_group *tg) { }
સ્થિર અંતઃરેખ bool task_group_is_autogroup(સંરચના task_group *tg)
{
	પાછા 0;
}

સ્થિર અંતઃરેખ સંરચના task_group *
autogroup_task_group(સંરચના task_struct *p, સંરચના task_group *tg)
{
	પાછા tg;
}

સ્થિર અંતઃરેખ પૂર્ણાંક autogroup_path(સંરચના task_group *tg, અક્ષર *buf, પૂર્ણાંક buflen)
{
	પાછા 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
