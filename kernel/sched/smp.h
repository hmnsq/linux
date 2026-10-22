/* SPDX-License-Identifier: GPL-2.0 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_SMP_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ಅನ್ತರ್ಭಾವ <linux/types.h>

ಬಾಹ್ಯ ಶೂನ್ಯ sched_ttwu_pending(ಶೂನ್ಯ *arg);

ಬಾಹ್ಯ bool call_function_single_prep_ipi(ಪೂರ್ಣಾಂಕ cpu);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SMP
ಬಾಹ್ಯ ಶೂನ್ಯ flush_smp_call_function_ಸರತಿ(ಶೂನ್ಯ);
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ flush_smp_call_function_ಸರತಿ(ಶೂನ್ಯ) { }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_SMP_H */
