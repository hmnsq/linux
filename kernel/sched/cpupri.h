/* SPDX-License-Identifier: GPL-2.0 */
#ಅನ್ತರ್ಭಾವ <linux/atomic.h>
#ಅನ್ತರ್ಭಾವ <linux/cpumask.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

ಸಂರಚನಾ cpupri_vec {
	atomic_t		ಎಣಿಕೆ;
	cpumask_var_t		mask;
};

ಸಂರಚನಾ cpupri {
	ಸಂರಚನಾ cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	ಪೂರ್ಣಾಙ್ಕ			*cpu_to_pri;
};

ಪೂರ್ಣಾಙ್ಕ  cpupri_find(ಸಂರಚನಾ cpupri *cp, ಸಂರಚನಾ task_struct *p,
		 ಸಂರಚನಾ cpumask *lowest_mask);
ಪೂರ್ಣಾಙ್ಕ  cpupri_find_fitness(ಸಂರಚನಾ cpupri *cp, ಸಂರಚನಾ task_struct *p,
			 ಸಂರಚನಾ cpumask *lowest_mask,
			 bool (*fitness_fn)(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu));
ಶೂನ್ಯ cpupri_set(ಸಂರಚನಾ cpupri *cp, ಪೂರ್ಣಾಙ್ಕ cpu, ಪೂರ್ಣಾಙ್ಕ pri);
ಪೂರ್ಣಾಙ್ಕ  cpupri_init(ಸಂರಚನಾ cpupri *cp);
ಶೂನ್ಯ cpupri_cleanup(ಸಂರಚನಾ cpupri *cp);
