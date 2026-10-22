/* SPDX-License-Identifier: GPL-2.0 */
#ଅନ୍ତର୍ଭାଵ <linux/atomic.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpumask.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

ସଂରଚନା cpupri_vec {
	atomic_t		ଗଣନା_2;
	cpumask_var_t		mask;
};

ସଂରଚନା cpupri {
	ସଂରଚନା cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			*cpu_to_pri;
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା  cpupri_find(ସଂରଚନା cpupri *cp, ସଂରଚନା task_struct *p,
		 ସଂରଚନା cpumask *lowest_mask);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା  cpupri_find_fitness(ସଂରଚନା cpupri *cp, ସଂରଚନା task_struct *p,
			 ସଂରଚନା cpumask *lowest_mask,
			 bool (*fitness_fn)(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu));
ଶୂନ୍ୟ cpupri_set(ସଂରଚନା cpupri *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pri);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା  cpupri_init(ସଂରଚନା cpupri *cp);
ଶୂନ୍ୟ cpupri_cleanup(ସଂରଚନା cpupri *cp);
