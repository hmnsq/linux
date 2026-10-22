/* SPDX-License-Identifier: GPL-2.0 */
#समावेश <linux/types.h>
#समावेश <linux/spinlock.h>

#घोषणा IDX_INVALID		-1

काष्ठा cpudl_item {
	u64			dl;
	पूर्णांक			cpu;
	पूर्णांक			idx;
};

काष्ठा cpudl {
	raw_spinlock_t		lock;
	पूर्णांक			आकार;
	cpumask_var_t		free_cpus;
	काष्ठा cpudl_item	*elements;
};

पूर्णांक  cpudl_find(काष्ठा cpudl *cp, काष्ठा task_struct *p, काष्ठा cpumask *later_mask);
व्योम cpudl_set(काष्ठा cpudl *cp, पूर्णांक cpu, u64 dl);
व्योम cpudl_clear(काष्ठा cpudl *cp, पूर्णांक cpu, bool online);
पूर्णांक  cpudl_init(काष्ठा cpudl *cp);
व्योम cpudl_cleanup(काष्ठा cpudl *cp);
