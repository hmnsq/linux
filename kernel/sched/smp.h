/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _KERNEL_SCHED_SMP_H
#define _KERNEL_SCHED_SMP_H

/*
 * Scheduler internal SMP callback types and methods between the scheduler
 * and other internal parts of the core kernel:
 */
#அந்தர்பாவ <linux/types.h>

வெளி வெறுமை sched_ttwu_pending(வெறுமை *arg);

வெளி bool call_function_single_prep_ipi(முழு cpu);

#ifdef CONFIG_SMP
வெளி வெறுமை flush_smp_call_function_queue(வெறுமை);
#இல்லையெனில்
நிலையான உள்வரி வெறுமை flush_smp_call_function_queue(வெறுமை) { }
#endif

#endif /* _KERNEL_SCHED_SMP_H */
