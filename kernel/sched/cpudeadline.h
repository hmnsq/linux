/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/types.h>
#include <linux/spinlock.h>

#define IDX_INVALID		-1

sa_nracanaa cpudl_item {
	u64			dl;
	ཧྲིལ			cpu;
	ཧྲིལ			idx;
};

sa_nracanaa cpudl {
	raw_spinlock_t		lock;
	ཧྲིལ			size;
	cpumask_var_t		free_cpus;
	sa_nracanaa cpudl_item	*elements;
};

ཧྲིལ  cpudl_find(sa_nracanaa cpudl *cp, sa_nracanaa task_struct *p, sa_nracanaa cpumask *later_mask);
void cpudl_set(sa_nracanaa cpudl *cp, ཧྲིལ cpu, u64 dl);
void cpudl_clear(sa_nracanaa cpudl *cp, ཧྲིལ cpu, bool online);
ཧྲིལ  cpudl_init(sa_nracanaa cpudl *cp);
void cpudl_cleanup(sa_nracanaa cpudl *cp);
