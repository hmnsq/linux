/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#അന്തര്ഭാവ <linux/types.h>

ബാഹ്യ ശൂന്യം sched_ttwu_pending(ശൂന്യം *arg);

ബാഹ്യ bool call_function_single_prep_ipi(പൂർണ്ണസംഖ്യ cpu);

#ifdef CONFIG_SMP
ബാഹ്യ ശൂന്യം flush_smp_call_function_queue(ശൂന്യം);
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ശൂന്യം flush_smp_call_function_queue(ശൂന്യം) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
