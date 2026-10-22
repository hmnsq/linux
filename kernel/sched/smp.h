/* SPDX-License-Identifier: GPL-2.0 */

#अगर_अघोषित _KERNEL_SCHED_SMP_H
#घोषणा _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#समावेश <linux/types.h>

बाह्य व्योम sched_ttwu_pending(व्योम *arg);

बाह्य bool call_function_single_prep_ipi(पूर्णांक cpu);

#अगर_घोषित CONFIG_SMP
बाह्य व्योम flush_smp_call_function_queue(व्योम);
#अन्यथा
अटल अंतरभूत व्योम flush_smp_call_function_queue(व्योम) { }
#पूर्ण_अगर

#पूर्ण_अगर /* _KERNEL_SCHED_SMP_H */
