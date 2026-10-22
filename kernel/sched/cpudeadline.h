/* SPDX-License-Identifier: GPL-2.0 */
#അന്തര്ഭാവ <linux/types.h>
#അന്തര്ഭാവ <linux/spinlock.h>

#define IDX_INVALID		-1

ഘടന cpudl_item {
	u64			dl;
	പൂർണ്ണസംഖ്യ			cpu;
	പൂർണ്ണസംഖ്യ			idx;
};

ഘടന cpudl {
	raw_spinlock_t		lock;
	പൂർണ്ണസംഖ്യ			size;
	cpumask_var_t		free_cpus;
	ഘടന cpudl_item	*elements;
};

പൂർണ്ണസംഖ്യ  cpudl_find(ഘടന cpudl *cp, ഘടന task_struct *p, ഘടന cpumask *later_mask);
ശൂന്യം cpudl_set(ഘടന cpudl *cp, പൂർണ്ണസംഖ്യ cpu, u64 dl);
ശൂന്യം cpudl_clear(ഘടന cpudl *cp, പൂർണ്ണസംഖ്യ cpu, bool online);
പൂർണ്ണസംഖ്യ  cpudl_init(ഘടന cpudl *cp);
ശൂന്യം cpudl_cleanup(ഘടന cpudl *cp);
