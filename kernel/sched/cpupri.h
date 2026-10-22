/* SPDX-License-Identifier: GPL-2.0 */
#समावेश <linux/atomic.h>
#समावेश <linux/cpumask.h>
#समावेश <linux/sched/rt.h>

#घोषणा CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#घोषणा CPUPRI_INVALID		-1
#घोषणा CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#घोषणा CPUPRI_HIGHER		100

काष्ठा cpupri_vec {
	atomic_t		गिनती;
	cpumask_var_t		mask;
};

काष्ठा cpupri {
	काष्ठा cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	पूर्णांक			*cpu_to_pri;
};

पूर्णांक  cpupri_find(काष्ठा cpupri *cp, काष्ठा task_struct *p,
		 काष्ठा cpumask *lowest_mask);
पूर्णांक  cpupri_find_fitness(काष्ठा cpupri *cp, काष्ठा task_struct *p,
			 काष्ठा cpumask *lowest_mask,
			 bool (*fitness_fn)(काष्ठा task_struct *p, पूर्णांक cpu));
व्योम cpupri_set(काष्ठा cpupri *cp, पूर्णांक cpu, पूर्णांक pri);
पूर्णांक  cpupri_init(काष्ठा cpupri *cp);
व्योम cpupri_cleanup(काष्ठा cpupri *cp);
