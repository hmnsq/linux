/* SPDX-License-Identifier: GPL-2.0 */
#ಅನ್ತರ್ಭಾವ <linux/types.h>
#ಅನ್ತರ್ಭಾವ <linux/spinlock.h>

#define IDX_INVALID		-1

ಸಂರಚನಾ cpudl_item {
	u64			dl;
	ಪೂರ್ಣಾಙ್ಕ			cpu;
	ಪೂರ್ಣಾಙ್ಕ			idx;
};

ಸಂರಚನಾ cpudl {
	raw_spinlock_t		lock;
	ಪೂರ್ಣಾಙ್ಕ			size;
	cpumask_var_t		free_cpus;
	ಸಂರಚನಾ cpudl_item	*elements;
};

ಪೂರ್ಣಾಙ್ಕ  cpudl_find(ಸಂರಚನಾ cpudl *cp, ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ cpumask *later_mask);
ಶೂನ್ಯ cpudl_set(ಸಂರಚನಾ cpudl *cp, ಪೂರ್ಣಾಙ್ಕ cpu, u64 dl);
ಶೂನ್ಯ cpudl_clear(ಸಂರಚನಾ cpudl *cp, ಪೂರ್ಣಾಙ್ಕ cpu, bool online);
ಪೂರ್ಣಾಙ್ಕ  cpudl_init(ಸಂರಚನಾ cpudl *cp);
ಶೂನ್ಯ cpudl_cleanup(ಸಂರಚನಾ cpudl *cp);
