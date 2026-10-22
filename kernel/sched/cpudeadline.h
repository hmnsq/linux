/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/types.h>
#ضمن <linux/spinlock.h>

#تعریف_کن IDX_INVALID		-1

بنية cpudl_قلم {
	u64			dl;
	صحيح			cpu;
	صحيح			idx;
};

بنية cpudl {
	raw_spinlock_t		قفل;
	صحيح			اندازه;
	cpumask_var_t		free_cpus;
	بنية cpudl_قلم	*elements;
};

صحيح  cpudl_پیدا_کن(بنية cpudl *cp, بنية task_struct *p, بنية cpumask *later_ماسک);
فراغ cpudl_مجموعه(بنية cpudl *cp, صحيح cpu, u64 dl);
فراغ cpudl_clear(بنية cpudl *cp, صحيح cpu, bool online);
صحيح  cpudl_init(بنية cpudl *cp);
فراغ cpudl_cleanup(بنية cpudl *cp);
