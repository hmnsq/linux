/* SPDX-License-Identifier: GPL-2.0 */
#ಅನ್ತರ್ಭಾವ <linux/types.h>
#ಅನ್ತರ್ಭಾವ <linux/spinlock.h>

#ವ್ಯಾಖ್ಯಾನಿಸು IDX_INVALID		-1

ರಚನೆ cpudl_ಅಂಶ {
	u64			dl;
	ಪೂರ್ಣಾಂಕ			cpu;
	ಪೂರ್ಣಾಂಕ			idx;
};

ರಚನೆ cpudl {
	raw_spinlock_t		ಬೀಗ;
	ಪೂರ್ಣಾಂಕ			ಗಾತ್ರ_2;
	cpumask_var_t		free_cpus;
	ರಚನೆ cpudl_ಅಂಶ	*elements;
};

ಪೂರ್ಣಾಂಕ  cpudl_ಕಂಡುಹಿಡಿ(ರಚನೆ cpudl *cp, ರಚನೆ task_struct *p, ರಚನೆ cpumask *later_ಮುಖವಾಡ);
ಶೂನ್ಯ cpudl_ಗಣ(ರಚನೆ cpudl *cp, ಪೂರ್ಣಾಂಕ cpu, u64 dl);
ಶೂನ್ಯ cpudl_clear(ರಚನೆ cpudl *cp, ಪೂರ್ಣಾಂಕ cpu, bool online);
ಪೂರ್ಣಾಂಕ  cpudl_init(ರಚನೆ cpudl *cp);
ಶೂನ್ಯ cpudl_cleanup(ರಚನೆ cpudl *cp);
