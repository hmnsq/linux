/* SPDX-License-Identifier: GPL-2.0 */
#અન્તર્ભાવ <linux/atomic.h>
#અન્તર્ભાવ <linux/cpumask.h>
#અન્તર્ભાવ <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

સંરચના cpupri_vec {
	atomic_t		ગણતરી;
	cpumask_var_t		mask;
};

સંરચના cpupri {
	સંરચના cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	પૂર્ણાંક			*cpu_to_pri;
};

પૂર્ણાંક  cpupri_find(સંરચના cpupri *cp, સંરચના task_struct *p,
		 સંરચના cpumask *lowest_mask);
પૂર્ણાંક  cpupri_find_fitness(સંરચના cpupri *cp, સંરચના task_struct *p,
			 સંરચના cpumask *lowest_mask,
			 bool (*fitness_fn)(સંરચના task_struct *p, પૂર્ણાંક cpu));
શૂન્ય cpupri_set(સંરચના cpupri *cp, પૂર્ણાંક cpu, પૂર્ણાંક pri);
પૂર્ણાંક  cpupri_init(સંરચના cpupri *cp);
શૂન્ય cpupri_cleanup(સંરચના cpupri *cp);
