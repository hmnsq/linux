/* SPDX-License-Identifier: GPL-2.0 */
#شامل <linux/types.h>
#شامل <linux/spinlock.h>

#تعریف_کرو IDX_INVALID		-1

ساخت cpudl_شے {
	u64			dl;
	صحیح			cpu;
	صحیح			idx;
};

ساخت cpudl {
	raw_spinlock_t		تالا;
	صحیح			سائز;
	cpumask_var_t		free_cpus;
	ساخت cpudl_شے	*elements;
};

صحیح  cpudl_ڈھونڈو(ساخت cpudl *cp, ساخت task_struct *p, ساخت cpumask *later_نقاب);
خالی cpudl_سیٹ(ساخت cpudl *cp, صحیح cpu, u64 dl);
خالی cpudl_clear(ساخت cpudl *cp, صحیح cpu, bool online);
صحیح  cpudl_init(ساخت cpudl *cp);
خالی cpudl_cleanup(ساخت cpudl *cp);
