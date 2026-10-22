/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/types.h>
#ضمن <linux/spinlock.h>

#وصف_ڪريو IDX_INVALID		-1

بنية cpudl_شي {
	u64			dl;
	صحيح			cpu;
	صحيح			idx;
};

بنية cpudl {
	raw_spinlock_t		تالو;
	صحيح			سائيز;
	cpumask_var_t		free_cpus;
	بنية cpudl_شي	*elements;
};

صحيح  cpudl_ڳوليو(بنية cpudl *cp, بنية task_struct *p, بنية cpumask *later_نقاب);
فراغ cpudl_سيٽ(بنية cpudl *cp, صحيح cpu, u64 dl);
فراغ cpudl_clear(بنية cpudl *cp, صحيح cpu, bool online);
صحيح  cpudl_init(بنية cpudl *cp);
فراغ cpudl_cleanup(بنية cpudl *cp);
