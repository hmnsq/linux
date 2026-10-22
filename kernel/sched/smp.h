/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#অন্তর্ভাব <linux/types.h>

বাহ্যিক শূন্য sched_ttwu_pending(শূন্য *arg);

বাহ্যিক bool call_function_single_prep_ipi(পূর্ণসংখ্যা cpu);

#ifdef CONFIG_SMP
বাহ্যিক শূন্য flush_smp_call_function_queue(শূন্য);
#অন্যথা
স্থির অন্তঃরেখ শূন্য flush_smp_call_function_queue(শূন্য) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
