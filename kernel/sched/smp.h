/* SPDX-License-Identifier: GPL-2.0 */

#جيڪڏهن_وصف_نه_ٿيل _KERNEL_SCHED_SMP_H
#وصف_ڪريو _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ضمن <linux/types.h>

خارجي فراغ sched_ttwu_pending(فراغ *arg);

خارجي bool call_function_single_prep_ipi(صحيح cpu);

#جيڪڏهن_وصف_ٿيل CONFIG_SMP
خارجي فراغ flush_smp_call_function_قطار(فراغ);
#وإلا
ساكن مضمن فراغ flush_smp_call_function_قطار(فراغ) { }
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* _KERNEL_SCHED_SMP_H */
