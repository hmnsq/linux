/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ಅನ್ತರ್ಭಾವ <linux/types.h>

ಬಾಹ್ಯ ಶೂನ್ಯ sched_ttwu_pending(ಶೂನ್ಯ *arg);

ಬಾಹ್ಯ bool call_function_single_prep_ipi(ಪೂರ್ಣಾಙ್ಕ cpu);

#ifdef CONFIG_SMP
ಬಾಹ್ಯ ಶೂನ್ಯ flush_smp_call_function_queue(ಶೂನ್ಯ);
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ flush_smp_call_function_queue(ಶೂನ್ಯ) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
