/* SPDX-License-Identifier: GPL-2.0 */
#شامل <linux/atomic.h>
#شامل <linux/cpumask.h>
#شامل <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

ساختار cpupri_vec {
	atomic_t		شمار;
	cpumask_var_t		mask;
};

ساختار cpupri {
	ساختار cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	صحیح			*cpu_to_pri;
};

صحیح  cpupri_find(ساختار cpupri *cp, ساختار task_struct *p,
		 ساختار cpumask *lowest_mask);
صحیح  cpupri_find_fitness(ساختار cpupri *cp, ساختار task_struct *p,
			 ساختار cpumask *lowest_mask,
			 bool (*fitness_fn)(ساختار task_struct *p, صحیح cpu));
تهی cpupri_set(ساختار cpupri *cp, صحیح cpu, صحیح pri);
صحیح  cpupri_init(ساختار cpupri *cp);
تهی cpupri_cleanup(ساختار cpupri *cp);
