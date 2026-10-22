/* SPDX-License-Identifier: GPL-2.0 */
#अन्तर्भाव <linux/types.h>
#अन्तर्भाव <linux/spinlock.h>

#define IDX_INVALID		-1

संरचना cpudl_item {
	u64			dl;
	पूर्णांक			cpu;
	पूर्णांक			idx;
};

संरचना cpudl {
	raw_spinlock_t		lock;
	पूर्णांक			size;
	cpumask_var_t		free_cpus;
	संरचना cpudl_item	*elements;
};

पूर्णांक  cpudl_find(संरचना cpudl *cp, संरचना task_struct *p, संरचना cpumask *later_mask);
शून्य cpudl_set(संरचना cpudl *cp, पूर्णांक cpu, u64 dl);
शून्य cpudl_clear(संरचना cpudl *cp, पूर्णांक cpu, bool online);
पूर्णांक  cpudl_init(संरचना cpudl *cp);
शून्य cpudl_cleanup(संरचना cpudl *cp);
