/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/types.h>
#include <linux/spinlock.h>

#define IDX_INVALID		-1

struct cpudl_item {
	u64			dl;
	цео			cpu;
	цео			idx;
};

struct cpudl {
	raw_spinlock_t		lock;
	цео			size;
	cpumask_var_t		free_cpus;
	struct cpudl_item	*elements;
};

цео  cpudl_find(struct cpudl *cp, struct task_struct *p, struct cpumask *later_mask);
void cpudl_set(struct cpudl *cp, цео cpu, u64 dl);
void cpudl_clear(struct cpudl *cp, цео cpu, bool online);
цео  cpudl_init(struct cpudl *cp);
void cpudl_cleanup(struct cpudl *cp);
