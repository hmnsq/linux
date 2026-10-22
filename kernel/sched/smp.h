/* SPDX-License-Identifier: GPL-2.0 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_SMP_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/types.h>

ਬਾਹਰੀ ਖਾਲੀ sched_ttwu_pending(ਖਾਲੀ *arg);

ਬਾਹਰੀ bool call_function_single_prep_ipi(ਪੂਰਨ cpu);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SMP
ਬਾਹਰੀ ਖਾਲੀ flush_smp_call_function_ਕਤਾਰ(ਖਾਲੀ);
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ flush_smp_call_function_ਕਤਾਰ(ਖਾਲੀ) { }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_SMP_H */
