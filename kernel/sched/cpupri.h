/* SPDX-License-Identifier: GPL-2.0 */
#अन्तर्भाव <linux/atomic.h>
#अन्तर्भाव <linux/cpumask.h>
#अन्तर्भाव <linux/sched/rt.h>

#परिभाषय CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#परिभाषय CPUPRI_INVALID		-1
#परिभाषय CPUPRI_प्रसामान्यम्		 0
/* values 1-99 are for RT1-RT99 priorities */
#परिभाषय CPUPRI_HIGHER		100

संरचना cpupri_vec {
	atomic_t		गणनम्;
	cpumask_var_t		आवरणम्;
};

संरचना cpupri {
	संरचना cpupri_vec	pri_गन्तव्यम्_cpu[CPUPRI_NR_PRIORITIES];
	पूर्णाङ्क			*cpu_गन्तव्यम्_pri;
};

पूर्णाङ्क  cpupri_find(संरचना cpupri *cp, संरचना task_struct *p,
		 संरचना cpumask *निम्नतमम्_आवरणम्);
पूर्णाङ्क  cpupri_find_fitness(संरचना cpupri *cp, संरचना task_struct *p,
			 संरचना cpumask *निम्नतमम्_आवरणम्,
			 bool (*fitness_fn)(संरचना task_struct *p, पूर्णाङ्क cpu));
शून्य cpupri_समुच्चयः(संरचना cpupri *cp, पूर्णाङ्क cpu, पूर्णाङ्क pri);
पूर्णाङ्क  cpupri_init(संरचना cpupri *cp);
शून्य cpupri_cleanup(संरचना cpupri *cp);
