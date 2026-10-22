/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/types.h>
#include <linux/spinlock.h>

#define IDX_INVALID		-1

struct cpudl_item {
	u64			dl;
	số_nguyên			cpu;
	số_nguyên			idx;
};

struct cpudl {
	raw_spinlock_t		lock;
	số_nguyên			size;
	cpumask_var_t		free_cpus;
	struct cpudl_item	*elements;
};

số_nguyên  cpudl_find(struct cpudl *cp, struct task_struct *p, struct cpumask *later_mask);
void cpudl_set(struct cpudl *cp, số_nguyên cpu, u64 dl);
void cpudl_clear(struct cpudl *cp, số_nguyên cpu, bool online);
số_nguyên  cpudl_init(struct cpudl *cp);
void cpudl_cleanup(struct cpudl *cp);
