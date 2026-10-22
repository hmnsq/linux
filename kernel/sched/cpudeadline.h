/* SPDX-License-Identifier: GPL-2.0 */
#अन्तर्भाव <linux/types.h>
#अन्तर्भाव <linux/spinlock.h>

#define IDX_INVALID		-1

संरचना cpudl_item {
	u64			dl;
	पूर्णाङ्क			cpu;
	पूर्णाङ्क			idx;
};

संरचना cpudl {
	raw_spinlock_t		lock;
	पूर्णाङ्क			size;
	cpumask_var_t		free_cpus;
	संरचना cpudl_item	*elements;
};

पूर्णाङ्क  cpudl_find(संरचना cpudl *cp, संरचना task_struct *p, संरचना cpumask *later_mask);
शून्य cpudl_set(संरचना cpudl *cp, पूर्णाङ्क cpu, u64 dl);
शून्य cpudl_clear(संरचना cpudl *cp, पूर्णाङ्क cpu, bool online);
पूर्णाङ्क  cpudl_init(संरचना cpudl *cp);
शून्य cpudl_cleanup(संरचना cpudl *cp);
