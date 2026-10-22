/* SPDX-License-Identifier: GPL-2.0 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/atomic.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpumask.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/rt.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CPUPRI_INVALID		-1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CPUPRI_ਸਾਧਾਰਨ		 0
/* values 1-99 are for RT1-RT99 priorities */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CPUPRI_HIGHER		100

ਬਣਤਰ cpupri_vec {
	atomic_t		ਗਿਣਤੀ_2;
	cpumask_var_t		ਨਕਾਬ;
};

ਬਣਤਰ cpupri {
	ਬਣਤਰ cpupri_vec	pri_ਮੰਜ਼ਿਲ_cpu[CPUPRI_NR_PRIORITIES];
	ਪੂਰਨ			*cpu_ਮੰਜ਼ਿਲ_pri;
};

ਪੂਰਨ  cpupri_ਲੱਭੋ(ਬਣਤਰ cpupri *cp, ਬਣਤਰ task_struct *p,
		 ਬਣਤਰ cpumask *ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ);
ਪੂਰਨ  cpupri_ਲੱਭੋ_fitness(ਬਣਤਰ cpupri *cp, ਬਣਤਰ task_struct *p,
			 ਬਣਤਰ cpumask *ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ,
			 bool (*fitness_fn)(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu));
ਖਾਲੀ cpupri_ਸੈੱਟ(ਬਣਤਰ cpupri *cp, ਪੂਰਨ cpu, ਪੂਰਨ pri);
ਪੂਰਨ  cpupri_init(ਬਣਤਰ cpupri *cp);
ਖਾਲੀ cpupri_cleanup(ਬਣਤਰ cpupri *cp);
