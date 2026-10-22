/* SPDX-License-Identifier: GPL-2.0 */
#அந்தர்பாவ <linux/atomic.h>
#அந்தர்பாவ <linux/cpumask.h>
#அந்தர்பாவ <linux/sched/rt.h>

#define CPUPRI_NR_PRIORITIES	(MAX_RT_PRIO+1)

#define CPUPRI_INVALID		-1
#define CPUPRI_NORMAL		 0
/* values 1-99 are for RT1-RT99 priorities */
#define CPUPRI_HIGHER		100

கட்டமைப்பு cpupri_vec {
	atomic_t		மொத்தம்;
	cpumask_var_t		mask;
};

கட்டமைப்பு cpupri {
	கட்டமைப்பு cpupri_vec	pri_to_cpu[CPUPRI_NR_PRIORITIES];
	முழு			*cpu_to_pri;
};

முழு  cpupri_find(கட்டமைப்பு cpupri *cp, கட்டமைப்பு task_struct *p,
		 கட்டமைப்பு cpumask *lowest_mask);
முழு  cpupri_find_fitness(கட்டமைப்பு cpupri *cp, கட்டமைப்பு task_struct *p,
			 கட்டமைப்பு cpumask *lowest_mask,
			 bool (*fitness_fn)(கட்டமைப்பு task_struct *p, முழு cpu));
வெறுமை cpupri_set(கட்டமைப்பு cpupri *cp, முழு cpu, முழு pri);
முழு  cpupri_init(கட்டமைப்பு cpupri *cp);
வெறுமை cpupri_cleanup(கட்டமைப்பு cpupri *cp);
