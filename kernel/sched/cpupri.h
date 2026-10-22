/* SPDX-License-Identifier: GPL-2.0 */
#ضمن <linux/atomic.h>
#ضمن <linux/cpumask.h>
#ضمن <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

بنية cpupri_vec {
	atomic_t		تعداد_2;
	cpumask_var_t		mask;
};

بنية cpupri {
	بنية cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	صحيح			*cpu_to_pri;
};

صحيح  cpupri_find(بنية cpupri *cp, بنية task_struct *p,
		 بنية cpumask *lowest_mask);
صحيح  cpupri_find_fitness(بنية cpupri *cp, بنية task_struct *p,
			 بنية cpumask *lowest_mask,
			 bool (*fitness_fn)(بنية task_struct *p, صحيح cpu));
فراغ cpupri_set(بنية cpupri *cp, صحيح cpu, صحيح pri);
صحيح  cpupri_init(بنية cpupri *cp);
فراغ cpupri_cleanup(بنية cpupri *cp);
