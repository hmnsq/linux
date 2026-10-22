/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ضمن <linux/types.h>

خارجي فراغ sched_ttwu_pending(فراغ *arg);

خارجي bool call_function_single_prep_ipi(صحيح cpu);

#ifdef CONFIG_SMP
خارجي فراغ flush_smp_call_function_queue(فراغ);
#אחרת
ساكن مضمن فراغ flush_smp_call_function_queue(فراغ) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
