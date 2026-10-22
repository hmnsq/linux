/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/atomic.h>
#ضمن <linux/cpumask.h>
#ضمن <linux/sched/rt.h>

#تعریف_کړه CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#تعریف_کړه CPUPRI_INVALID		-1
#تعریف_کړه CPUPRI_نورمال		 0
/* values 1-99 are for RT1-RT99 priorities */
#تعریف_کړه CPUPRI_HIGHER		100

بنية cpupri_vec {
	atomic_t		شمېر;
	cpumask_var_t		ماسک;
};

بنية cpupri {
	بنية cpupri_vec	pri_منزل_cpu[CPUPRI_NR_PRIORITIES];
	صحيح			*cpu_منزل_pri;
};

صحيح  cpupri_پیدا_کړه(بنية cpupri *cp, بنية task_struct *p,
		 بنية cpumask *ټیټ_ترین_ماسک);
صحيح  cpupri_پیدا_کړه_fitness(بنية cpupri *cp, بنية task_struct *p,
			 بنية cpumask *ټیټ_ترین_ماسک,
			 bool (*fitness_fn)(بنية task_struct *p, صحيح cpu));
فراغ cpupri_سېټ(بنية cpupri *cp, صحيح cpu, صحيح pri);
صحيح  cpupri_init(بنية cpupri *cp);
فراغ cpupri_cleanup(بنية cpupri *cp);
