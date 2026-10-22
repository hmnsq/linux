/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/types.h>
#include <linux/spinlock.h>

#define IDX_INVALID		-1

sa_nracanaa cpudl_item {
	u64			dl;
	පූර්ණ_සංඛ්‍යා			cpu;
	පූර්ණ_සංඛ්‍යා			idx;
};

sa_nracanaa cpudl {
	raw_spinlock_t		lock;
	පූර්ණ_සංඛ්‍යා			size;
	cpumask_var_t		free_cpus;
	sa_nracanaa cpudl_item	*elements;
};

පූර්ණ_සංඛ්‍යා  cpudl_find(sa_nracanaa cpudl *cp, sa_nracanaa task_struct *p, sa_nracanaa cpumask *later_mask);
void cpudl_set(sa_nracanaa cpudl *cp, පූර්ණ_සංඛ්‍යා cpu, u64 dl);
void cpudl_clear(sa_nracanaa cpudl *cp, පූර්ණ_සංඛ්‍යා cpu, bool online);
පූර්ණ_සංඛ්‍යා  cpudl_init(sa_nracanaa cpudl *cp);
void cpudl_cleanup(sa_nracanaa cpudl *cp);
