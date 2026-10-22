/* SPDX-License-Identifier: GPL-2.0 */
#அந்தர்பாவ <linux/types.h>
#அந்தர்பாவ <linux/spinlock.h>

#define IDX_INVALID		-1

கட்டமைப்பு cpudl_item {
	u64			dl;
	முழு			cpu;
	முழு			idx;
};

கட்டமைப்பு cpudl {
	raw_spinlock_t		lock;
	முழு			size;
	cpumask_var_t		free_cpus;
	கட்டமைப்பு cpudl_item	*elements;
};

முழு  cpudl_find(கட்டமைப்பு cpudl *cp, கட்டமைப்பு task_struct *p, கட்டமைப்பு cpumask *later_mask);
வெறுமை cpudl_set(கட்டமைப்பு cpudl *cp, முழு cpu, u64 dl);
வெறுமை cpudl_clear(கட்டமைப்பு cpudl *cp, முழு cpu, bool online);
முழு  cpudl_init(கட்டமைப்பு cpudl *cp);
வெறுமை cpudl_cleanup(கட்டமைப்பு cpudl *cp);
