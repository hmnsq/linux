/* SPDX-License-Identifier: GPL-2.0 */
#অন্তর্ভাৱ <linux/atomic.h>
#অন্তর্ভাৱ <linux/cpumask.h>
#অন্তর্ভাৱ <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

সংরচনা cpupri_vec {
	atomic_t		গুনতি;
	cpumask_var_t		mask;
};

সংরচনা cpupri {
	সংরচনা cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	পূর্ণাঙ্ক			*cpu_to_pri;
};

পূর্ণাঙ্ক  cpupri_find(সংরচনা cpupri *cp, সংরচনা task_struct *p,
		 সংরচনা cpumask *lowest_mask);
পূর্ণাঙ্ক  cpupri_find_fitness(সংরচনা cpupri *cp, সংরচনা task_struct *p,
			 সংরচনা cpumask *lowest_mask,
			 bool (*fitness_fn)(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu));
শূন্য cpupri_set(সংরচনা cpupri *cp, পূর্ণাঙ্ক cpu, পূর্ণাঙ্ক pri);
পূর্ণাঙ্ক  cpupri_init(সংরচনা cpupri *cp);
শূন্য cpupri_cleanup(সংরচনা cpupri *cp);
