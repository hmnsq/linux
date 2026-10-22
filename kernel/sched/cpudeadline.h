/* SPDX-License-Identifier: GPL-2.0 */
#అన్తర్భావ <linux/types.h>
#అన్తర్భావ <linux/spinlock.h>

#నిర్వచించు IDX_INVALID		-1

నిర్మాణం cpudl_అంశం {
	u64			dl;
	పూర్ణాంకం			cpu;
	పూర్ణాంకం			idx;
};

నిర్మాణం cpudl {
	raw_spinlock_t		తాళం;
	పూర్ణాంకం			కొలత;
	cpumask_var_t		free_cpus;
	నిర్మాణం cpudl_అంశం	*elements;
};

పూర్ణాంకం  cpudl_కనుగొను(నిర్మాణం cpudl *cp, నిర్మాణం task_struct *p, నిర్మాణం cpumask *later_ముసుగు);
శూన్య cpudl_సమితి(నిర్మాణం cpudl *cp, పూర్ణాంకం cpu, u64 dl);
శూన్య cpudl_clear(నిర్మాణం cpudl *cp, పూర్ణాంకం cpu, bool online);
పూర్ణాంకం  cpudl_init(నిర్మాణం cpudl *cp);
శూన్య cpudl_cleanup(నిర్మాణం cpudl *cp);
