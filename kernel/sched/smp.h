/* SPDX-License-Identifier: GPL-2.0 */

#నిర్వచితం_కాకపోతే _KERNEL_SCHED_SMP_H
#నిర్వచించు _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#అన్తర్భావ <linux/types.h>

బాహ్య శూన్య sched_ttwu_pending(శూన్య *arg);

బాహ్య bool call_function_single_prep_ipi(పూర్ణాంకం cpu);

#నిర్వచితమైతే CONFIG_SMP
బాహ్య శూన్య flush_smp_call_function_వరుస(శూన్య);
#లేకపోతే
స్థిర అంతఃవరుస శూన్య flush_smp_call_function_వరుస(శూన్య) { }
#అయితే_ముగింపు

#అయితే_ముగింపు /* _KERNEL_SCHED_SMP_H */
