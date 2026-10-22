/* SPDX-License-Identifier: GPL-2.0 */
#شامل <linux/atomic.h>
#شامل <linux/cpumask.h>
#شامل <linux/sched/rt.h>

#تعریف_کرو CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#تعریف_کرو CPUPRI_INVALID		-1
#تعریف_کرو CPUPRI_معمول		 0
/* values 1-99 are for RT1-RT99 priorities */
#تعریف_کرو CPUPRI_HIGHER		100

ساخت cpupri_vec {
	atomic_t		گنتی;
	cpumask_var_t		نقاب;
};

ساخت cpupri {
	ساخت cpupri_vec	pri_مقصد_cpu[CPUPRI_NR_PRIORITIES];
	صحیح			*cpu_مقصد_pri;
};

صحیح  cpupri_ڈھونڈو(ساخت cpupri *cp, ساخت task_struct *p,
		 ساخت cpumask *سب_سے_نیچا_نقاب);
صحیح  cpupri_ڈھونڈو_fitness(ساخت cpupri *cp, ساخت task_struct *p,
			 ساخت cpumask *سب_سے_نیچا_نقاب,
			 bool (*fitness_fn)(ساخت task_struct *p, صحیح cpu));
خالی cpupri_سیٹ(ساخت cpupri *cp, صحیح cpu, صحیح pri);
صحیح  cpupri_init(ساخت cpupri *cp);
خالی cpupri_cleanup(ساخت cpupri *cp);
