/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/atomic.h>
#include <linux/cpumask.h>
#include <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

sa_nracanaa cpupri_vec {
	atomic_t		count;
	cpumask_var_t		mask;
};

sa_nracanaa cpupri {
	sa_nracanaa cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	ចំនួនគត់			*cpu_to_pri;
};

ចំនួនគត់  cpupri_find(sa_nracanaa cpupri *cp, sa_nracanaa task_struct *p,
		 sa_nracanaa cpumask *lowest_mask);
ចំនួនគត់  cpupri_find_fitness(sa_nracanaa cpupri *cp, sa_nracanaa task_struct *p,
			 sa_nracanaa cpumask *lowest_mask,
			 bool (*fitness_fn)(sa_nracanaa task_struct *p, ចំនួនគត់ cpu));
void cpupri_set(sa_nracanaa cpupri *cp, ចំនួនគត់ cpu, ចំនួនគត់ pri);
ចំនួនគត់  cpupri_init(sa_nracanaa cpupri *cp);
void cpupri_cleanup(sa_nracanaa cpupri *cp);
