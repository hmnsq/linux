/* SPDX-License-Identifier: GPL-2.0 */
#অন্তর্ভাৱ <linux/types.h>
#অন্তর্ভাৱ <linux/spinlock.h>

#define IDX_INVALID		-1

সংরচনা cpudl_item {
	u64			dl;
	পূর্ণাঙ্ক			cpu;
	পূর্ণাঙ্ক			idx;
};

সংরচনা cpudl {
	raw_spinlock_t		lock;
	পূর্ণাঙ্ক			size;
	cpumask_var_t		free_cpus;
	সংরচনা cpudl_item	*elements;
};

পূর্ণাঙ্ক  cpudl_find(সংরচনা cpudl *cp, সংরচনা task_struct *p, সংরচনা cpumask *later_mask);
শূন্য cpudl_set(সংরচনা cpudl *cp, পূর্ণাঙ্ক cpu, u64 dl);
শূন্য cpudl_clear(সংরচনা cpudl *cp, পূর্ণাঙ্ক cpu, bool online);
পূর্ণাঙ্ক  cpudl_init(সংরচনা cpudl *cp);
শূন্য cpudl_cleanup(সংরচনা cpudl *cp);
