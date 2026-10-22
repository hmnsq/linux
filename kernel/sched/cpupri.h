/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/atomic.h>
#ضمن <linux/cpumask.h>
#ضمن <linux/sched/rt.h>

#تعریف_کن CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#تعریف_کن CPUPRI_INVALID		-1
#تعریف_کن CPUPRI_نرمال		 0
/* values 1-99 are for RT1-RT99 priorities */
#تعریف_کن CPUPRI_HIGHER		100

بنية cpupri_vec {
	atomic_t		شمار;
	cpumask_var_t		ماسک;
};

بنية cpupri {
	بنية cpupri_vec	pri_مقصد_cpu[CPUPRI_NR_PRIORITIES];
	صحيح			*cpu_مقصد_pri;
};

صحيح  cpupri_پیدا_کن(بنية cpupri *cp, بنية task_struct *p,
		 بنية cpumask *پایینترین_ماسک);
صحيح  cpupri_پیدا_کن_fitness(بنية cpupri *cp, بنية task_struct *p,
			 بنية cpumask *پایینترین_ماسک,
			 bool (*fitness_fn)(بنية task_struct *p, صحيح cpu));
فراغ cpupri_مجموعه(بنية cpupri *cp, صحيح cpu, صحيح pri);
صحيح  cpupri_init(بنية cpupri *cp);
فراغ cpupri_cleanup(بنية cpupri *cp);
