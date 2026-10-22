/* SPDX-License-Identifier: GPL-2.0 */
#ಅನ್ತರ್ಭಾವ <linux/atomic.h>
#ಅನ್ತರ್ಭಾವ <linux/cpumask.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/rt.h>

#ವ್ಯಾಖ್ಯಾನಿಸು CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#ವ್ಯಾಖ್ಯಾನಿಸು CPUPRI_INVALID		-1
#ವ್ಯಾಖ್ಯಾನಿಸು CPUPRI_ಸಾಮಾನ್ಯ		 0
/* values 1-99 are for RT1-RT99 priorities */
#ವ್ಯಾಖ್ಯಾನಿಸು CPUPRI_HIGHER		100

ರಚನೆ cpupri_vec {
	atomic_t		ಎಣಿಕೆ_2;
	cpumask_var_t		ಮುಖವಾಡ;
};

ರಚನೆ cpupri {
	ರಚನೆ cpupri_vec	pri_ಗಮ್ಯ_cpu[CPUPRI_NR_PRIORITIES];
	ಪೂರ್ಣಾಂಕ			*cpu_ಗಮ್ಯ_pri;
};

ಪೂರ್ಣಾಂಕ  cpupri_ಕಂಡುಹಿಡಿ(ರಚನೆ cpupri *cp, ರಚನೆ task_struct *p,
		 ರಚನೆ cpumask *ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ);
ಪೂರ್ಣಾಂಕ  cpupri_ಕಂಡುಹಿಡಿ_fitness(ರಚನೆ cpupri *cp, ರಚನೆ task_struct *p,
			 ರಚನೆ cpumask *ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ,
			 bool (*fitness_fn)(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu));
ಶೂನ್ಯ cpupri_ಗಣ(ರಚನೆ cpupri *cp, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ pri);
ಪೂರ್ಣಾಂಕ  cpupri_init(ರಚನೆ cpupri *cp);
ಶೂನ್ಯ cpupri_cleanup(ರಚನೆ cpupri *cp);
