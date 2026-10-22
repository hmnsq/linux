/* SPDX-License-Identifier: GPL-2.0 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_AUTOGROUP_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_AUTOGROUP_H

#ಅನ್ತರ್ಭಾವ "sched.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_AUTOGROUP

ರಚನೆ autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	ರಚನೆ kref		kref;
	ರಚನೆ task_ಗುಂಪು	*tg;
	ರಚನೆ rw_semaphore	ಬೀಗ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		id;
	ಪೂರ್ಣಾಂಕ			nice;
};

ಬಾಹ್ಯ ಶೂನ್ಯ autogroup_init(ರಚನೆ task_struct *init_task);
ಬಾಹ್ಯ ಶೂನ್ಯ autogroup_free(ರಚನೆ task_ಗುಂಪು *tg);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_ಗುಂಪು_is_autogroup(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ !!tg->autogroup;
}

ಬಾಹ್ಯ bool task_ಬಯಸುತ್ತದೆ_autogroup(ರಚನೆ task_struct *p, ರಚನೆ task_ಗುಂಪು *tg);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_ಗುಂಪು *
autogroup_task_ಗುಂಪು(ರಚನೆ task_struct *p, ರಚನೆ task_ಗುಂಪು *tg)
{
	ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_autogroup_enabled;
	ಪೂರ್ಣಾಂಕ enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ಆದರೆ (enabled && task_ಬಯಸುತ್ತದೆ_autogroup(p, tg))
		ಹಿಂತಿರುಗಿ p->ಸಂಕೇತ->autogroup->tg;

	ಹಿಂತಿರುಗಿ tg;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ autogroup_ಮಾರ್ಗ(ರಚನೆ task_ಗುಂಪು *tg, ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ buflen);

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_AUTOGROUP: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ autogroup_init(ರಚನೆ task_struct *init_task) {  }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ autogroup_free(ರಚನೆ task_ಗುಂಪು *tg) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_ಗುಂಪು_is_autogroup(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_ಗುಂಪು *
autogroup_task_ಗುಂಪು(ರಚನೆ task_struct *p, ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ tg;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ autogroup_ಮಾರ್ಗ(ರಚನೆ task_ಗುಂಪು *tg, ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ buflen)
{
	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_AUTOGROUP */

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_AUTOGROUP_H */
