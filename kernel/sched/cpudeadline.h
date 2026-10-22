/* SPDX-License-Identifier: GPL-2.0 */
#شامل <linux/types.h>
#شامل <linux/spinlock.h>

#define IDX_INVALID		-1

ساختار cpudl_item {
	u64			dl;
	صحیح			cpu;
	صحیح			idx;
};

ساختار cpudl {
	raw_spinlock_t		lock;
	صحیح			size;
	cpumask_var_t		free_cpus;
	ساختار cpudl_item	*elements;
};

صحیح  cpudl_find(ساختار cpudl *cp, ساختار task_struct *p, ساختار cpumask *later_mask);
تهی cpudl_set(ساختار cpudl *cp, صحیح cpu, u64 dl);
تهی cpudl_clear(ساختار cpudl *cp, صحیح cpu, bool online);
صحیح  cpudl_init(ساختار cpudl *cp);
تهی cpudl_cleanup(ساختار cpudl *cp);
