/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#અન્તર્ભાવ <linux/types.h>

બાહ્ય શૂન્ય sched_ttwu_pending(શૂન્ય *arg);

બાહ્ય bool call_function_single_prep_ipi(પૂર્ણાંક cpu);

#ifdef CONFIG_SMP
બાહ્ય શૂન્ય flush_smp_call_function_queue(શૂન્ય);
#નહીંતર
સ્થિર અંતઃરેખ શૂન્ય flush_smp_call_function_queue(શૂન્ય) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
