/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ଅନ୍ତର୍ଭାଵ <linux/types.h>

ବାହ୍ୟ ଶୂନ୍ୟ sched_ttwu_pending(ଶୂନ୍ୟ *arg);

ବାହ୍ୟ bool call_function_single_prep_ipi(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);

#ifdef CONFIG_SMP
ବାହ୍ୟ ଶୂନ୍ୟ flush_smp_call_function_queue(ଶୂନ୍ୟ);
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ flush_smp_call_function_queue(ଶୂନ୍ୟ) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
