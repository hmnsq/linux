/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/types.h>
#include <linux/spinlock.h>

#define IDX_INVALID		-1

struct cpudl_item {
	u64			dl;
	整数			cpu;
	整数			idx;
};

struct cpudl {
	raw_spinlock_t		lock;
	整数			size;
	cpumask_var_t		free_cpus;
	struct cpudl_item	*elements;
};

整数  cpudl_find(struct cpudl *cp, struct task_struct *p, struct cpumask *later_mask);
void cpudl_set(struct cpudl *cp, 整数 cpu, u64 dl);
void cpudl_clear(struct cpudl *cp, 整数 cpu, bool online);
整数  cpudl_init(struct cpudl *cp);
void cpudl_cleanup(struct cpudl *cp);
