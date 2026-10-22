/* SPDX-License-Identifier: GPL-2.0 */
#अन्तर्भाव <linux/atomic.h>
#अन्तर्भाव <linux/cpumask.h>
#अन्तर्भाव <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

संरचना cpupri_vec {
	atomic_t		गन्ती;
	cpumask_var_t		mask;
};

संरचना cpupri {
	संरचना cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	पूर्णाङ्क			*cpu_to_pri;
};

पूर्णाङ्क  cpupri_find(संरचना cpupri *cp, संरचना task_struct *p,
		 संरचना cpumask *lowest_mask);
पूर्णाङ्क  cpupri_find_fitness(संरचना cpupri *cp, संरचना task_struct *p,
			 संरचना cpumask *lowest_mask,
			 bool (*fitness_fn)(संरचना task_struct *p, पूर्णाङ्क cpu));
शून्य cpupri_set(संरचना cpupri *cp, पूर्णाङ्क cpu, पूर्णाङ्क pri);
पूर्णाङ्क  cpupri_init(संरचना cpupri *cp);
शून्य cpupri_cleanup(संरचना cpupri *cp);
