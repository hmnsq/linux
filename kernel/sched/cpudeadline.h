/* SPDX-License-Identifier: GPL-2.0 */
#অন্তর্ভাব <linux/types.h>
#অন্তর্ভাব <linux/spinlock.h>

#define IDX_INVALID		-1

কাঠামো cpudl_item {
	u64			dl;
	পূর্ণসংখ্যা			cpu;
	পূর্ণসংখ্যা			idx;
};

কাঠামো cpudl {
	raw_spinlock_t		lock;
	পূর্ণসংখ্যা			আকার_2;
	cpumask_var_t		free_cpus;
	কাঠামো cpudl_item	*elements;
};

পূর্ণসংখ্যা  cpudl_find(কাঠামো cpudl *cp, কাঠামো task_struct *p, কাঠামো cpumask *later_mask);
শূন্য cpudl_set(কাঠামো cpudl *cp, পূর্ণসংখ্যা cpu, u64 dl);
শূন্য cpudl_clear(কাঠামো cpudl *cp, পূর্ণসংখ্যা cpu, bool online);
পূর্ণসংখ্যা  cpudl_init(কাঠামো cpudl *cp);
শূন্য cpudl_cleanup(কাঠামো cpudl *cp);
