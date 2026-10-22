/* SPDX-License-Identifier: GPL-2.0 */

#جے_بنا_تعریف _KERNEL_SCHED_SMP_H
#تعریف_کرو _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ضمن <linux/types.h>

خارجي فراغ sched_ttwu_pending(فراغ *arg);

خارجي bool call_function_single_prep_ipi(صحيح cpu);

#جے_تعریف_شدہ CONFIG_SMP
خارجي فراغ flush_smp_call_function_قطار(فراغ);
#وإلا
ساكن مضمن فراغ flush_smp_call_function_قطار(فراغ) { }
#جے_انت

#جے_انت /* _KERNEL_SCHED_SMP_H */
