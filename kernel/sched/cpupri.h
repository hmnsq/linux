/* SPDX-License-Identifier: GPL-2.0 */
#അന്തര്ഭാവ <linux/atomic.h>
#അന്തര്ഭാവ <linux/cpumask.h>
#അന്തര്ഭാവ <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

ഘടന cpupri_vec {
	atomic_t		എണ്ണം;
	cpumask_var_t		mask;
};

ഘടന cpupri {
	ഘടന cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	പൂർണ്ണസംഖ്യ			*cpu_to_pri;
};

പൂർണ്ണസംഖ്യ  cpupri_find(ഘടന cpupri *cp, ഘടന task_struct *p,
		 ഘടന cpumask *lowest_mask);
പൂർണ്ണസംഖ്യ  cpupri_find_fitness(ഘടന cpupri *cp, ഘടന task_struct *p,
			 ഘടന cpumask *lowest_mask,
			 bool (*fitness_fn)(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu));
ശൂന്യം cpupri_set(ഘടന cpupri *cp, പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ pri);
പൂർണ്ണസംഖ്യ  cpupri_init(ഘടന cpupri *cp);
ശൂന്യം cpupri_cleanup(ഘടന cpupri *cp);
