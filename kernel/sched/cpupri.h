/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/atomic.h>
#ضمن <linux/cpumask.h>
#ضمن <linux/sched/rt.h>

#تعریف_کرو CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#تعریف_کرو CPUPRI_INVALID		-1
#تعریف_کرو CPUPRI_عام		 0
/* values 1-99 are for RT1-RT99 priorities */
#تعریف_کرو CPUPRI_HIGHER		100

بنية cpupri_vec {
	atomic_t		گنتی;
	cpumask_var_t		نقاب;
};

بنية cpupri {
	بنية cpupri_vec	pri_منزل_cpu[CPUPRI_NR_PRIORITIES];
	صحيح			*cpu_منزل_pri;
};

صحيح  cpupri_لبھو(بنية cpupri *cp, بنية task_struct *p,
		 بنية cpumask *سبھ_توں_نیواں_نقاب);
صحيح  cpupri_لبھو_fitness(بنية cpupri *cp, بنية task_struct *p,
			 بنية cpumask *سبھ_توں_نیواں_نقاب,
			 bool (*fitness_fn)(بنية task_struct *p, صحيح cpu));
فراغ cpupri_سیٹ(بنية cpupri *cp, صحيح cpu, صحيح pri);
صحيح  cpupri_init(بنية cpupri *cp);
فراغ cpupri_cleanup(بنية cpupri *cp);
