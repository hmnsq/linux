/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/types.h>
#ضمن <linux/spinlock.h>

#تعریف_کړه IDX_INVALID		-1

بنية cpudl_توکی {
	u64			dl;
	صحيح			cpu;
	صحيح			idx;
};

بنية cpudl {
	raw_spinlock_t		قلف;
	صحيح			اندازه;
	cpumask_var_t		free_cpus;
	بنية cpudl_توکی	*elements;
};

صحيح  cpudl_پیدا_کړه(بنية cpudl *cp, بنية task_struct *p, بنية cpumask *later_ماسک);
فراغ cpudl_سېټ(بنية cpudl *cp, صحيح cpu, u64 dl);
فراغ cpudl_clear(بنية cpudl *cp, صحيح cpu, bool online);
صحيح  cpudl_init(بنية cpudl *cp);
فراغ cpudl_cleanup(بنية cpudl *cp);
