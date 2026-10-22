/* SPDX-License-Identifier: GPL-2.0 */
#ଅନ୍ତର୍ଭାଵ <linux/types.h>
#ଅନ୍ତର୍ଭାଵ <linux/spinlock.h>

#define IDX_INVALID		-1

ସଂରଚନା cpudl_item {
	u64			dl;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			idx;
};

ସଂରଚନା cpudl {
	raw_spinlock_t		lock;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			size;
	cpumask_var_t		free_cpus;
	ସଂରଚନା cpudl_item	*elements;
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା  cpudl_find(ସଂରଚନା cpudl *cp, ସଂରଚନା task_struct *p, ସଂରଚନା cpumask *later_mask);
ଶୂନ୍ୟ cpudl_set(ସଂରଚନା cpudl *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, u64 dl);
ଶୂନ୍ୟ cpudl_clear(ସଂରଚନା cpudl *cp, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool online);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା  cpudl_init(ସଂରଚନା cpudl *cp);
ଶୂନ୍ୟ cpudl_cleanup(ସଂରଚନା cpudl *cp);
