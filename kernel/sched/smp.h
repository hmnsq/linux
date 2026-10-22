/* SPDX-License-Identifier: GPL-2.0 */

#اگر_غیر_تعریف_شدہ _KERNEL_SCHED_SMP_H
#تعریف_کرو _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#شامل <linux/types.h>

خارجی خالی sched_ttwu_pending(خالی *arg);

خارجی bool call_function_single_prep_ipi(صحیح cpu);

#اگر_تعریف_شدہ CONFIG_SMP
خارجی خالی flush_smp_call_function_قطار(خالی);
#ورنہ
ساکن اندرونی خالی flush_smp_call_function_قطار(خالی) { }
#اگر_ختم

#اگر_ختم /* _KERNEL_SCHED_SMP_H */
