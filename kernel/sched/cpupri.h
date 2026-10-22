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
	整数			*cpu_to_pri;
};

整数  cpupri_find(struct cpupri *cp, struct task_struct *p,
		 struct cpumask *lowest_mask);
整数  cpupri_find_fitness(struct cpupri *cp, struct task_struct *p,
			 struct cpumask *lowest_mask,
			 bool (*fitness_fn)(struct task_struct *p, 整数 cpu));
void cpupri_set(struct cpupri *cp, 整数 cpu, 整数 pri);
整数  cpupri_init(struct cpupri *cp);
void cpupri_cleanup(struct cpupri *cp);
