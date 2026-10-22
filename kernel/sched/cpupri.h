/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/atomic.h>
#include <linux/cpumask.h>
#include <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

struct cpupri_vec {
	atomic_t		count;
	cpumask_var_t		mask;
};

struct cpupri {
	struct cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	정수			*cpu_to_pri;
};

정수  cpupri_find(struct cpupri *cp, struct task_struct *p,
		 struct cpumask *lowest_mask);
정수  cpupri_find_fitness(struct cpupri *cp, struct task_struct *p,
			 struct cpumask *lowest_mask,
			 bool (*fitness_fn)(struct task_struct *p, 정수 cpu));
void cpupri_set(struct cpupri *cp, 정수 cpu, 정수 pri);
정수  cpupri_init(struct cpupri *cp);
void cpupri_cleanup(struct cpupri *cp);
