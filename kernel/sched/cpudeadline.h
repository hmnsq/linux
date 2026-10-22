/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/types.h>
#ضمن <linux/spinlock.h>

#define IDX_INVALID		-1

بنية cpudl_item {
	u64			dl;
	صحيح			cpu;
	صحيح			idx;
};

بنية cpudl {
	raw_spinlock_t		lock;
	صحيح			size;
	cpumask_var_t		free_cpus;
	بنية cpudl_item	*elements;
};

صحيح  cpudl_find(بنية cpudl *cp, بنية task_struct *p, بنية cpumask *later_mask);
فراغ cpudl_set(بنية cpudl *cp, صحيح cpu, u64 dl);
فراغ cpudl_clear(بنية cpudl *cp, صحيح cpu, bool online);
صحيح  cpudl_init(بنية cpudl *cp);
فراغ cpudl_cleanup(بنية cpudl *cp);
