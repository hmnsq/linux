/* SPDX-License-Identifier: GPL-2.0 */

#اگر_تعریف_نشده _KERNEL_SCHED_SMP_H
#تعریف_کن _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ضمن <linux/types.h>

خارجي فراغ sched_ttwu_pending(فراغ *arg);

خارجي bool call_function_single_prep_ipi(صحيح cpu);

#اگر_تعریف_شده CONFIG_SMP
خارجي فراغ flush_smp_call_function_صف(فراغ);
#وإلا
ساكن مضمن فراغ flush_smp_call_function_صف(فراغ) { }
#پایان_اگر

#پایان_اگر /* _KERNEL_SCHED_SMP_H */
