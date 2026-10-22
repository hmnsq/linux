/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#ಅನ್ತರ್ಭಾವ "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

ಸಂರಚನಾ autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	ಸಂರಚನಾ kref		kref;
	ಸಂರಚನಾ task_group	*tg;
	ಸಂರಚನಾ rw_semaphore	lock;
	unsigned long		id;
	ಪೂರ್ಣಾಙ್ಕ			nice;
};

ಬಾಹ್ಯ ಶೂನ್ಯ autogroup_init(ಸಂರಚನಾ task_struct *init_task);
ಬಾಹ್ಯ ಶೂನ್ಯ autogroup_free(ಸಂರಚನಾ task_group *tg);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_group_is_autogroup(ಸಂರಚನಾ task_group *tg)
{
	ಪ್ರತ್ಯಾಗಮ !!tg->autogroup;
}

ಬಾಹ್ಯ bool task_wants_autogroup(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ task_group *tg);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_group *
autogroup_task_group(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ task_group *tg)
{
	ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_sched_autogroup_enabled;
	ಪೂರ್ಣಾಙ್ಕ enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ಯದಿ (enabled && task_wants_autogroup(p, tg))
		ಪ್ರತ್ಯಾಗಮ p->ಸಙ್ಕೇತ->autogroup->tg;

	ಪ್ರತ್ಯಾಗಮ tg;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ autogroup_path(ಸಂರಚನಾ task_group *tg, ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ buflen);

#ಅನ್ಯಥಾ /* !CONFIG_SCHED_AUTOGROUP: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ autogroup_init(ಸಂರಚನಾ task_struct *init_task) {  }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ autogroup_free(ಸಂರಚನಾ task_group *tg) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_group_is_autogroup(ಸಂರಚನಾ task_group *tg)
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_group *
autogroup_task_group(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ task_group *tg)
{
	ಪ್ರತ್ಯಾಗಮ tg;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ autogroup_path(ಸಂರಚನಾ task_group *tg, ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ buflen)
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
