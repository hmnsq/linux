/* SPDX-License-Identifier: GPL-2.0 */
#અન્તર્ભાવ <linux/types.h>
#અન્તર્ભાવ <linux/spinlock.h>

#define IDX_INVALID		-1

સંરચના cpudl_item {
	u64			dl;
	પૂર્ણાંક			cpu;
	પૂર્ણાંક			idx;
};

સંરચના cpudl {
	raw_spinlock_t		lock;
	પૂર્ણાંક			size;
	cpumask_var_t		free_cpus;
	સંરચના cpudl_item	*elements;
};

પૂર્ણાંક  cpudl_find(સંરચના cpudl *cp, સંરચના task_struct *p, સંરચના cpumask *later_mask);
શૂન્ય cpudl_set(સંરચના cpudl *cp, પૂર્ણાંક cpu, u64 dl);
શૂન્ય cpudl_clear(સંરચના cpudl *cp, પૂર્ણાંક cpu, bool online);
પૂર્ણાંક  cpudl_init(સંરચના cpudl *cp);
શૂન્ય cpudl_cleanup(સંરચના cpudl *cp);
