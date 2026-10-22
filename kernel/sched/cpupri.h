/* SPDX-License-Identifier: GPL-2.0 */
#అన్తర్భావ <linux/atomic.h>
#అన్తర్భావ <linux/cpumask.h>
#అన్తర్భావ <linux/sched/rt.h>

#నిర్వచించు CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#నిర్వచించు CPUPRI_INVALID		-1
#నిర్వచించు CPUPRI_సాధారణ		 0
/* values 1-99 are for RT1-RT99 priorities */
#నిర్వచించు CPUPRI_HIGHER		100

నిర్మాణం cpupri_vec {
	atomic_t		లెక్క;
	cpumask_var_t		ముసుగు;
};

నిర్మాణం cpupri {
	నిర్మాణం cpupri_vec	pri_గమ్యం_cpu[CPUPRI_NR_PRIORITIES];
	పూర్ణాంకం			*cpu_గమ్యం_pri;
};

పూర్ణాంకం  cpupri_కనుగొను(నిర్మాణం cpupri *cp, నిర్మాణం task_struct *p,
		 నిర్మాణం cpumask *అత్యల్ప_ముసుగు);
పూర్ణాంకం  cpupri_కనుగొను_fitness(నిర్మాణం cpupri *cp, నిర్మాణం task_struct *p,
			 నిర్మాణం cpumask *అత్యల్ప_ముసుగు,
			 bool (*fitness_fn)(నిర్మాణం task_struct *p, పూర్ణాంకం cpu));
శూన్య cpupri_సమితి(నిర్మాణం cpupri *cp, పూర్ణాంకం cpu, పూర్ణాంకం pri);
పూర్ణాంకం  cpupri_init(నిర్మాణం cpupri *cp);
శూన్య cpupri_cleanup(నిర్మాణం cpupri *cp);
