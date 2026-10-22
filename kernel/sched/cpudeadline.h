/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/types.h>
#include <linux/spinlock.h>

#define IDX_INVALID		-1

struct cpudl_item {
	u64			dl;
	정수			cpu;
	정수			idx;
};

struct cpudl {
	raw_spinlock_t		lock;
	정수			size;
	cpumask_var_t		free_cpus;
	struct cpudl_item	*elements;
};

정수  cpudl_find(struct cpudl *cp, struct task_struct *p, struct cpumask *later_mask);
void cpudl_set(struct cpudl *cp, 정수 cpu, u64 dl);
void cpudl_clear(struct cpudl *cp, 정수 cpu, bool online);
정수  cpudl_init(struct cpudl *cp);
void cpudl_cleanup(struct cpudl *cp);
