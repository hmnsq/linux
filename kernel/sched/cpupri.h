/* SPDX-License-Identifier: GPL-2.0 */
#অন্তর্ভাব <linux/atomic.h>
#অন্তর্ভাব <linux/cpumask.h>
#অন্তর্ভাব <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

কাঠামো cpupri_vec {
	atomic_t		গুনতি;
	cpumask_var_t		mask;
};

কাঠামো cpupri {
	কাঠামো cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	পূর্ণসংখ্যা			*cpu_to_pri;
};

পূর্ণসংখ্যা  cpupri_find(কাঠামো cpupri *cp, কাঠামো task_struct *p,
		 কাঠামো cpumask *lowest_mask);
পূর্ণসংখ্যা  cpupri_find_fitness(কাঠামো cpupri *cp, কাঠামো task_struct *p,
			 কাঠামো cpumask *lowest_mask,
			 bool (*fitness_fn)(কাঠামো task_struct *p, পূর্ণসংখ্যা cpu));
শূন্য cpupri_set(কাঠামো cpupri *cp, পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা pri);
পূর্ণসংখ্যা  cpupri_init(কাঠামো cpupri *cp);
শূন্য cpupri_cleanup(কাঠামো cpupri *cp);
