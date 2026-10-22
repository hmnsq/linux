/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#شامل <linux/types.h>

بیرونی تهی sched_ttwu_pending(تهی *arg);

بیرونی bool call_function_single_prep_ipi(صحیح cpu);

#ifdef CONFIG_SMP
بیرونی تهی flush_smp_call_function_queue(تهی);
#وگرنه
ایستا درونخطی تهی flush_smp_call_function_queue(تهی) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
