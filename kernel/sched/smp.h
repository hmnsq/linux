/* SPDX-License-Identifier: GPL-2.0 */

#که_نه_تعریف_شوی _KERNEL_SCHED_SMP_H
#تعریف_کړه _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ضمن <linux/types.h>

خارجي فراغ sched_ttwu_pending(فراغ *arg);

خارجي bool call_function_single_prep_ipi(صحيح cpu);

#که_تعریف_شوی CONFIG_SMP
خارجي فراغ flush_smp_call_function_کتار(فراغ);
#وإلا
ساكن مضمن فراغ flush_smp_call_function_کتار(فراغ) { }
#د_که_پای

#د_که_پای /* _KERNEL_SCHED_SMP_H */
