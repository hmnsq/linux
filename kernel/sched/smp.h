/* SPDX-License-Identifier: GPL-2.0 */

#यदि_अपरिभाषितम् _KERNEL_SCHED_SMP_H
#परिभाषय _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#अन्तर्भाव <linux/types.h>

बाह्य शून्य sched_ttwu_pending(शून्य *arg);

बाह्य bool call_function_single_prep_ipi(पूर्णाङ्क cpu);

#यदि_परिभाषितम् CONFIG_SMP
बाह्य शून्य flush_smp_call_function_queue(शून्य);
#अन्यथा
स्थिर अन्तःस्थ शून्य flush_smp_call_function_queue(शून्य) { }
#यदि_अन्तः

#यदि_अन्तः /* _KERNEL_SCHED_SMP_H */
