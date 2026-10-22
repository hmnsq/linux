// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/core.c
 *
 *  Core kernel CPU scheduler code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#define INSTANTIATE_EXPORTED_MIGRATE_DISABLE
#ضمن <linux/sched.h>
#ضمن <linux/highmem.h>
#ضمن <linux/hrtimer_api.h>
#ضمن <linux/ktime_api.h>
#ضمن <linux/sched/signal.h>
#ضمن <linux/syscalls_api.h>
#ضمن <linux/debug_locks.h>
#ضمن <linux/prefetch.h>
#ضمن <linux/capability.h>
#ضمن <linux/pgtable_api.h>
#ضمن <linux/wait_bit.h>
#ضمن <linux/jiffies.h>
#ضمن <linux/spinlock_api.h>
#ضمن <linux/cpumask_api.h>
#ضمن <linux/lockdep_api.h>
#ضمن <linux/hardirq.h>
#ضمن <linux/softirq.h>
#ضمن <linux/refcount_api.h>
#ضمن <linux/topology.h>
#ضمن <linux/sched/clock.h>
#ضمن <linux/sched/cond_resched.h>
#ضمن <linux/sched/cputime.h>
#ضمن <linux/sched/debug.h>
#ضمن <linux/sched/hotplug.h>
#ضمن <linux/sched/init.h>
#ضمن <linux/sched/isolation.h>
#ضمن <linux/sched/loadavg.h>
#ضمن <linux/sched/mm.h>
#ضمن <linux/sched/nohz.h>
#ضمن <linux/sched/rseq_api.h>
#ضمن <linux/sched/rt.h>

#ضمن <linux/context_tracking.h>
#ضمن <linux/cpuset.h>
#ضمن <linux/delayacct.h>
#ضمن <linux/init_task.h>
#ضمن <linux/interrupt.h>
#ضمن <linux/ioprio.h>
#ضمن <linux/kallsyms.h>
#ضمن <linux/kcov.h>
#ضمن <linux/kprobes.h>
#ضمن <linux/llist_api.h>
#ضمن <linux/mmu_context.h>
#ضمن <linux/mmzone.h>
#ضمن <linux/mutex_api.h>
#ضمن <linux/nmi.h>
#ضمن <linux/nospec.h>
#ضمن <linux/perf_event_api.h>
#ضمن <linux/profile.h>
#ضمن <linux/psi.h>
#ضمن <linux/rcuwait_api.h>
#ضمن <linux/rseq.h>
#ضمن <linux/sched/wake_q.h>
#ضمن <linux/scs.h>
#ضمن <linux/slab.h>
#ضمن <linux/syscalls.h>
#ضمن <linux/vtime.h>
#ضمن <linux/wait_api.h>
#ضمن <linux/workqueue_api.h>
#ضمن <linux/livepatch_sched.h>

#ifdef CONFIG_PREEMPT_DYNAMIC
# ifdef CONFIG_GENERIC_IRQ_ENTRY
#  include <linux/irq-entry-common.h>
# endif
#endif

#ضمن <uapi/linux/sched/types.h>

#ضمن <asm/irq_regs.h>
#ضمن <asm/switch_to.h>
#ضمن <asm/tlb.h>

#define CREATE_TRACE_POINTS
#ضمن <linux/sched/rseq_api.h>
#ضمن <trace/events/sched.h>
#ضمن <trace/events/ipi.h>
#undef CREATE_TRACE_POINTS

#ضمن "sched.h"
#ضمن "stats.h"

#ضمن "autogroup.h"
#ضمن "pelt.h"
#ضمن "smp.h"

#ضمن "../workqueue_internal.h"
#ضمن "../../io_uring/io-wq.h"
#ضمن "../smpboot.h"
#ضمن "../locking/mutex.h"

EXPORT_TRACEPOINT_رمز_GPL(ipi_send_cpu);
EXPORT_TRACEPOINT_رمز_GPL(ipi_send_cpumask);

/*
 * Export tracepoints that act as a bare tracehook (ie: have no trace event
 * associated with them) to allow external modules to probe them.
 */
EXPORT_TRACEPOINT_رمز_GPL(pelt_cfs_tp);
EXPORT_TRACEPOINT_رمز_GPL(pelt_rt_tp);
EXPORT_TRACEPOINT_رمز_GPL(pelt_dl_tp);
EXPORT_TRACEPOINT_رمز_GPL(pelt_irq_tp);
EXPORT_TRACEPOINT_رمز_GPL(pelt_se_tp);
EXPORT_TRACEPOINT_رمز_GPL(pelt_hw_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_cpu_capacity_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_overutilized_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_util_est_cfs_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_util_est_se_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_update_nr_running_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_compute_energy_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_entry_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_exit_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_set_need_resched_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_dl_throttle_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_dl_replenish_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_dl_update_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_dl_server_start_tp);
EXPORT_TRACEPOINT_رمز_GPL(sched_dl_server_stop_tp);

DEFINE_PER_CPU_SHARED_ALIGNED(بنية rq, runqueues);
DEFINE_PER_CPU(بنية rnd_state, sched_rnd_state);

#ifdef CONFIG_SCHED_PROXY_EXEC
DEFINE_STATIC_مفتاح_TRUE(__sched_proxy_exec);
ساكن صحيح __init setup_proxy_exec(حرف *str)
{
	bool proxy_enable = true;

	إذا (*str && kstrtobool(str + 1, &proxy_enable)) {
		pr_warn("Unable to parse sched_proxy_exec=\n");
		إرجاع 0;
	}

	إذا (proxy_enable) {
		pr_info("sched_proxy_exec enabled via boot arg\n");
		static_branch_enable(&__sched_proxy_exec);
	} وإلا {
		pr_info("sched_proxy_exec disabled via boot arg\n");
		static_branch_disable(&__sched_proxy_exec);
	}
	إرجاع 1;
}
#وإلا
ساكن صحيح __init setup_proxy_exec(حرف *str)
{
	pr_warn("CONFIG_SCHED_PROXY_EXEC=n, so it cannot be enabled or disabled at boot time\n");
	إرجاع 0;
}
#endif
__setup("sched_proxy_exec", setup_proxy_exec);

/*
 * Debugging: various feature bits
 *
 * If SCHED_DEBUG is disabled, each compilation unit has its own copy of
 * sysctl_sched_features, defined in sched.h, to allow constants propagation
 * at compile time and compiler optimization based on features default.
 */
#define SCHED_FEAT(name, enabled)	\
	(1UL << __SCHED_FEAT_##name) * enabled |
__read_mostly unsigned صحيح sysctl_sched_features =
#ضمن "features.h"
	0;
#undef SCHED_FEAT

/*
 * Print a warning if need_resched is set for the given duration (if
 * LATENCY_WARN is enabled).
 *
 * If sysctl_resched_latency_warn_once is set, only one warning will be shown
 * per boot.
 */
__read_mostly صحيح sysctl_resched_latency_warn_ms = 100;
__read_mostly صحيح sysctl_resched_latency_warn_once = 1;

/*
 * Number of tasks to iterate in a single balance run.
 * Limited because this is done with IRQs disabled.
 */
__read_mostly unsigned صحيح sysctl_sched_nr_migrate = SCHED_NR_MIGRATE_BREAK;

__read_mostly صحيح scheduler_running;

#ifdef CONFIG_SCHED_CORE

DEFINE_STATIC_مفتاح_FALSE(__sched_core_enabled);

/* kernel prio, less is more */
ساكن مضمن صحيح __task_prio(ثابت بنية task_struct *p)
{
	إذا (p->sched_class == &stop_sched_class) /* trumps deadline */
		إرجاع -2;

	إذا (p->dl_server)
		إرجاع -1; /* deadline */

	إذا (rt_or_dl_prio(p->prio))
		إرجاع p->prio; /* [-1, 99] */

	إذا (p->sched_class == &idle_sched_class)
		إرجاع MAX_RT_PRIO + NICE_WIDTH; /* 140 */

	إذا (task_on_scx(p))
		إرجاع MAX_RT_PRIO + MAX_NICE + 1; /* 120, squash ext */

	إرجاع MAX_RT_PRIO + MAX_NICE; /* 119, squash fair */
}

/*
 * l(a,b)
 * le(a,b) := !l(b,a)
 * g(a,b)  := l(b,a)
 * ge(a,b) := !l(a,b)
 */

/* real prio, less is less */
ساكن مضمن bool prio_less(ثابت بنية task_struct *a,
			     ثابت بنية task_struct *b, bool in_fi)
{

	صحيح pa = __task_prio(a), pb = __task_prio(b);

	إذا (-pa < -pb)
		إرجاع true;

	إذا (-pb < -pa)
		إرجاع false;

	إذا (pa == -1) { /* dl_prio() doesn't work because of stop_class above */
		ثابت بنية sched_dl_entity *a_dl, *b_dl;

		a_dl = &a->dl;
		/*
		 * Since,'a' and 'b' can be CFS tasks served by DL server,
		 * __task_prio() can return -1 (for DL) even for those. In that
		 * case, get to the dl_server's DL entity.
		 */
		إذا (a->dl_server)
			a_dl = a->dl_server;

		b_dl = &b->dl;
		إذا (b->dl_server)
			b_dl = b->dl_server;

		إرجاع !dl_time_before(a_dl->deadline, b_dl->deadline);
	}

	إذا (pa == MAX_RT_PRIO + MAX_NICE)	/* fair */
		إرجاع cfs_prio_less(a, b, in_fi);

#ifdef CONFIG_SCHED_CLASS_EXT
	إذا (pa == MAX_RT_PRIO + MAX_NICE + 1)	/* ext */
		إرجاع scx_prio_less(a, b, in_fi);
#endif

	إرجاع false;
}

ساكن مضمن bool __sched_core_less(ثابت بنية task_struct *a,
				     ثابت بنية task_struct *b)
{
	إذا (a->core_cookie < b->core_cookie)
		إرجاع true;

	إذا (a->core_cookie > b->core_cookie)
		إرجاع false;

	/* flip prio, so high prio is leftmost */
	إذا (prio_less(b, a, !!task_rq(a)->core->core_forceidle_تعداد))
		إرجاع true;

	إرجاع false;
}

#define __node_2_sc(عقدة) rb_entry((عقدة), بنية task_struct, core_عقدة)

ساكن مضمن bool rb_sched_core_less(بنية rb_عقدة *a, ثابت بنية rb_عقدة *b)
{
	إرجاع __sched_core_less(__node_2_sc(a), __node_2_sc(b));
}

ساكن مضمن صحيح rb_sched_core_cmp(ثابت فراغ *مفتاح, ثابت بنية rb_عقدة *عقدة)
{
	ثابت بنية task_struct *p = __node_2_sc(عقدة);
	unsigned long cookie = (unsigned long)مفتاح;

	إذا (cookie < p->core_cookie)
		إرجاع -1;

	إذا (cookie > p->core_cookie)
		إرجاع 1;

	إرجاع 0;
}

فراغ sched_core_enqueue(بنية rq *rq, بنية task_struct *p)
{
	إذا (p->se.sched_delayed)
		إرجاع;

	rq->core->core_task_seq++;

	إذا (!p->core_cookie)
		إرجاع;

	rb_add(&p->core_عقدة, &rq->core_شجرة, rb_sched_core_less);
}

فراغ sched_core_dequeue(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	إذا (p->se.sched_delayed)
		إرجاع;

	rq->core->core_task_seq++;

	إذا (sched_core_enqueued(p)) {
		rb_erase(&p->core_عقدة, &rq->core_شجرة);
		RB_CLEAR_عقدة(&p->core_عقدة);
	}

	/*
	 * Migrating the last task off the cpu, with the cpu in forced idle
	 * state. Reschedule to create an accounting edge for forced idle,
	 * and re-examine whether the core is still in forced idle state.
	 */
	إذا (!(flags & DEQUEUE_SAVE) && rq->nr_running == 1 &&
	    rq->core->core_forceidle_تعداد && rq->curr == rq->idle)
		resched_curr(rq);
}

ساكن صحيح sched_task_is_throttled(بنية task_struct *p, صحيح cpu)
{
	إذا (p->sched_class->task_is_throttled)
		إرجاع p->sched_class->task_is_throttled(p, cpu);

	إرجاع 0;
}

ساكن بنية task_struct *sched_core_next(بنية task_struct *p, unsigned long cookie)
{
	بنية rb_عقدة *عقدة = &p->core_عقدة;
	صحيح cpu = task_cpu(p);

	افعل {
		عقدة = rb_next(عقدة);
		إذا (!عقدة)
			إرجاع عدم;

		p = __node_2_sc(عقدة);
		إذا (p->core_cookie != cookie)
			إرجاع عدم;

	} بينما (sched_task_is_throttled(p, cpu));

	إرجاع p;
}

/*
 * Find left-most (aka, highest priority) and unthrottled task matching @cookie.
 * If no suitable task is found, NULL will be returned.
 */
ساكن بنية task_struct *sched_core_find(بنية rq *rq, unsigned long cookie)
{
	بنية task_struct *p;
	بنية rb_عقدة *عقدة;

	عقدة = rb_find_first((فراغ *)cookie, &rq->core_شجرة, rb_sched_core_cmp);
	إذا (!عقدة)
		إرجاع عدم;

	p = __node_2_sc(عقدة);
	إذا (!sched_task_is_throttled(p, rq->cpu))
		إرجاع p;

	إرجاع sched_core_next(p, cookie);
}

/*
 * Magic required such that:
 *
 *	raw_spin_rq_lock(rq);
 *	...
 *	raw_spin_rq_unlock(rq);
 *
 * ends up locking and unlocking the _same_ lock, and all CPUs
 * always agree on what rq has what lock.
 *
 * XXX entirely possible to selectively enable cores, don't bother for now.
 */

ساكن DEFINE_MUTEX(sched_core_mutex);
ساكن atomic_t sched_core_تعداد;
ساكن بنية cpumask sched_core_mask;

ساكن فراغ sched_core_lock(صحيح cpu, unsigned long *flags)
	__context_unsafe(/* acquires multiple */)
	__acquires(&runqueues.__lock) /* overapproximation */
{
	ثابت بنية cpumask *smt_mask = cpu_smt_mask(cpu);
	صحيح t, i = 0;

	local_irq_save(*flags);
	for_each_cpu(t, smt_mask)
		raw_spin_lock_nested(&cpu_rq(t)->__lock, i++);
}

ساكن فراغ sched_core_unlock(صحيح cpu, unsigned long *flags)
	__context_unsafe(/* releases multiple */)
	__releases(&runqueues.__lock) /* overapproximation */
{
	ثابت بنية cpumask *smt_mask = cpu_smt_mask(cpu);
	صحيح t;

	for_each_cpu(t, smt_mask)
		raw_spin_unlock(&cpu_rq(t)->__lock);
	local_irq_restore(*flags);
}

ساكن فراغ __sched_core_flip(bool enabled)
{
	unsigned long flags;
	صحيح cpu, t;

	cpus_read_lock();

	/*
	 * Toggle the online cores, one by one.
	 */
	cpumask_copy(&sched_core_mask, cpu_online_mask);
	for_each_cpu(cpu, &sched_core_mask) {
		ثابت بنية cpumask *smt_mask = cpu_smt_mask(cpu);

		sched_core_lock(cpu, &flags);

		/*
		 * A core-wide selection may have the shared rq lock temporarily
		 * released by a lock-dropping ->pick_task(). Flipping would
		 * rebind rq_lockp() under it. Wait it out.
		 */
		بينما (cpu_rq(cpu)->core->core_pick_in_flight) {
			sched_core_unlock(cpu, &flags);
			cpu_relax();
			sched_core_lock(cpu, &flags);
		}

		for_each_cpu(t, smt_mask)
			cpu_rq(t)->core_enabled = enabled;

		cpu_rq(cpu)->core->core_forceidle_start = 0;

		sched_core_unlock(cpu, &flags);

		cpumask_andnot(&sched_core_mask, &sched_core_mask, smt_mask);
	}

	/*
	 * Toggle the offline CPUs.
	 */
	for_each_cpu_andnot(cpu, cpu_possible_mask, cpu_online_mask)
		cpu_rq(cpu)->core_enabled = enabled;

	cpus_read_unlock();
}

ساكن فراغ sched_core_assert_empty(فراغ)
{
	صحيح cpu;

	for_each_possible_cpu(cpu)
		WARN_ON_ONCE(!RB_EMPTY_جذر(&cpu_rq(cpu)->core_شجرة));
}

ساكن فراغ __sched_core_enable(فراغ)
{
	static_branch_enable(&__sched_core_enabled);
	/*
	 * Ensure all previous instances of raw_spin_rq_*lock() have finished
	 * and future ones will observe !sched_core_disabled().
	 */
	synchronize_rcu();
	__sched_core_flip(true);
	sched_core_assert_empty();
}

ساكن فراغ __sched_core_disable(فراغ)
{
	sched_core_assert_empty();
	__sched_core_flip(false);
	static_branch_disable(&__sched_core_enabled);
}

فراغ sched_core_get(فراغ)
{
	إذا (atomic_inc_not_zero(&sched_core_تعداد))
		إرجاع;

	mutex_lock(&sched_core_mutex);
	إذا (!atomic_read(&sched_core_تعداد))
		__sched_core_enable();

	smp_mb__before_atomic();
	atomic_inc(&sched_core_تعداد);
	mutex_unlock(&sched_core_mutex);
}

ساكن فراغ __sched_core_put(بنية work_struct *work)
{
	إذا (atomic_dec_and_mutex_lock(&sched_core_تعداد, &sched_core_mutex)) {
		__sched_core_disable();
		mutex_unlock(&sched_core_mutex);
	}
}

فراغ sched_core_put(فراغ)
{
	ساكن DECLARE_WORK(_work, __sched_core_put);

	/*
	 * "There can be only one"
	 *
	 * Either this is the last one, or we don't actually need to do any
	 * 'work'. If it is the last *again*, we rely on
	 * WORK_STRUCT_PENDING_BIT.
	 */
	إذا (!atomic_add_unless(&sched_core_تعداد, -1, 1))
		schedule_work(&_work);
}

#وإلا /* !CONFIG_SCHED_CORE: */

ساكن مضمن فراغ sched_core_enqueue(بنية rq *rq, بنية task_struct *p) { }
ساكن مضمن فراغ
sched_core_dequeue(بنية rq *rq, بنية task_struct *p, صحيح flags) { }

#endif /* !CONFIG_SCHED_CORE */

/* need a wrapper since we may need to trace from modules */
EXPORT_TRACEPOINT_رمز(sched_set_state_tp);

/*
 * Call via the helper macro trace_set_current_state.
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_state_tp)
 */
فراغ __trace_set_current_state(صحيح state_قيمة)
{
	trace_call__sched_set_state_tp(current, state_قيمة);
}
EXPORT_رمز(__trace_set_current_state);

صحيح task_llc(ثابت بنية task_struct *p)
{
	إرجاع per_cpu(sd_llc_id, task_cpu(p));
}

/*
 * Serialization rules:
 *
 * Lock order:
 *
 *   p->pi_lock
 *     rq->lock
 *       hrtimer_cpu_base->lock (hrtimer_start() for bandwidth controls)
 *
 *  rq1->lock
 *    rq2->lock  where: rq1 < rq2
 *
 * Regular state:
 *
 * Normal scheduling state is serialized by rq->lock. __schedule() takes the
 * local CPU's rq->lock, it optionally removes the task from the runqueue and
 * always looks at the local rq data structures to find the most eligible task
 * to run next.
 *
 * Task enqueue is also under rq->lock, possibly taken from another CPU.
 * Wakeups from another LLC domain might use an IPI to transfer the enqueue to
 * the local CPU to avoid bouncing the runqueue state around [ see
 * ttwu_queue_wakelist() ]
 *
 * Task wakeup, specifically wakeups that involve migration, are horribly
 * complicated to avoid having to take two rq->locks.
 *
 * Special state:
 *
 * System-calls and anything external will use task_rq_lock() which acquires
 * both p->pi_lock and rq->lock. As a consequence the state they change is
 * stable while holding either lock:
 *
 *  - sched_setaffinity()/
 *    set_cpus_allowed_ptr():	p->cpus_ptr, p->nr_cpus_allowed
 *  - set_user_nice():		p->se.load, p->*prio
 *  - __sched_setscheduler():	p->sched_class, p->policy, p->*prio,
 *				p->se.load, p->rt_priority,
 *				p->dl.dl_{runtime, deadline, period, flags, bw, density}
 *  - sched_setnuma():		p->numa_preferred_nid
 *  - sched_move_task():	p->sched_task_group
 *  - uclamp_update_active()	p->uclamp*
 *
 * p->state <- TASK_*:
 *
 *   is changed locklessly using set_current_state(), __set_current_state() or
 *   set_special_state(), see their respective comments, or by
 *   try_to_wake_up(). This latter uses p->pi_lock to serialize against
 *   concurrent self.
 *
 * p->on_rq <- { 0, 1 = TASK_ON_RQ_QUEUED, 2 = TASK_ON_RQ_MIGRATING }:
 *
 *   is set by activate_task() and cleared by deactivate_task()/block_task(),
 *   under rq->lock. Non-zero indicates the task is runnable, the special
 *   ON_RQ_MIGRATING state is used for migration without holding both
 *   rq->locks. It indicates task_cpu() is not stable, see task_rq_lock().
 *
 *   Additionally it is possible to be ->on_rq but still be considered not
 *   runnable when p->se.sched_delayed is true. These tasks are on the runqueue
 *   but will be dequeued as soon as they get picked again. See the
 *   task_is_runnable() helper.
 *
 * p->on_cpu <- { 0, 1 }:
 *
 *   is set by prepare_task() and cleared by finish_task() such that it will be
 *   set before p is scheduled-in and cleared after p is scheduled-out, both
 *   under rq->lock. Non-zero indicates the task is running on its CPU.
 *
 *   [ The astute reader will observe that it is possible for two tasks on one
 *     CPU to have ->on_cpu = 1 at the same time. ]
 *
 * p->is_blocked <- { 0, 1 }:
 *
 *   is set by try_to_block_task() and cleared by ttwu_do_wakeup() and tracks
 *   if the task is blocked. Traditionally this would mirror p->on_rq, however
 *   due things like DELAY_DEQUEUE and PROXY_EXEC, this can diverge.
 *
 * task_cpu(p): is changed by set_task_cpu(), the rules are:
 *
 *  - Don't call set_task_cpu() on a blocked task:
 *
 *    We don't care what CPU we're not running on, this simplifies hotplug,
 *    the CPU assignment of blocked tasks isn't required to be valid.
 *
 *  - for try_to_wake_up(), called under p->pi_lock:
 *
 *    This allows try_to_wake_up() to only take one rq->lock, see its comment.
 *
 *  - for migration called under rq->lock:
 *    [ see task_on_rq_migrating() in task_rq_lock() ]
 *
 *    o move_queued_task()
 *    o detach_task()
 *
 *  - for migration called under double_rq_lock():
 *
 *    o __migrate_swap_task()
 *    o push_rt_task() / pull_rt_task()
 *    o push_dl_task() / pull_dl_task()
 *    o dl_task_offline_migration()
 *
 */

فراغ raw_spin_rq_lock_nested(بنية rq *rq, صحيح subclass)
	__context_unsafe()
{
	raw_spinlock_t *lock;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	إذا (sched_core_disabled()) {
		raw_spin_lock_nested(&rq->__lock, subclass);
		/* preempt_count *MUST* be > 1 */
		preempt_enable_no_resched();
		إرجاع;
	}

	لكل (;;) {
		lock = __rq_lockp(rq);
		raw_spin_lock_nested(lock, subclass);
		إذا (likely(lock == __rq_lockp(rq))) {
			/* preempt_count *MUST* be > 1 */
			preempt_enable_no_resched();
			إرجاع;
		}
		raw_spin_unlock(lock);
	}
}

bool raw_spin_rq_trylock(بنية rq *rq)
	__context_unsafe()
{
	raw_spinlock_t *lock;
	bool ret;

	/* Matches synchronize_rcu() in __sched_core_enable() */
	preempt_disable();
	إذا (sched_core_disabled()) {
		ret = raw_spin_trylock(&rq->__lock);
		preempt_enable();
		إرجاع ret;
	}

	لكل (;;) {
		lock = __rq_lockp(rq);
		ret = raw_spin_trylock(lock);
		إذا (!ret || (likely(lock == __rq_lockp(rq)))) {
			preempt_enable();
			إرجاع ret;
		}
		raw_spin_unlock(lock);
	}
}

/*
 * double_rq_lock - safely lock two runqueues
 */
فراغ double_rq_lock(بنية rq *rq1, بنية rq *rq2)
{
	lockdep_assert_irqs_disabled();

	إذا (rq_order_less(rq2, rq1))
		swap(rq1, rq2);

	raw_spin_rq_lock(rq1);
	إذا (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_lock_nested(rq2, SINGLE_DEPTH_NESTING);
	وإلا
		__acquire_ctx_lock(__rq_lockp(rq2)); /* fake acquire */

	double_rq_clock_clear_update(rq1, rq2);
}

/*
 * ___task_rq_lock - lock the rq @p resides on.
 */
بنية rq *___task_rq_lock(بنية task_struct *p, بنية rq_flags *rf)
{
	بنية rq *rq;

	lockdep_assert_held(&p->pi_lock);

	لكل (;;) {
		rq = task_rq(p);
		raw_spin_rq_lock(rq);
		إذا (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_lock(rq, rf);
			إرجاع rq;
		}
		raw_spin_rq_unlock(rq);

		بينما (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * task_rq_lock - lock p->pi_lock and lock the rq @p resides on.
 */
بنية rq *_task_rq_lock(بنية task_struct *p, بنية rq_flags *rf)
{
	بنية rq *rq;

	لكل (;;) {
		raw_spin_lock_irqsave(&p->pi_lock, rf->flags);
		rq = task_rq(p);
		raw_spin_rq_lock(rq);
		/*
		 *	move_queued_task()		task_rq_lock()
		 *
		 *	ACQUIRE (rq->lock)
		 *	[S] ->on_rq = MIGRATING		[L] rq = task_rq()
		 *	WMB (__set_task_cpu())		ACQUIRE (rq->lock);
		 *	[S] ->cpu = new_cpu		[L] task_rq()
		 *					[L] ->on_rq
		 *	RELEASE (rq->lock)
		 *
		 * If we observe the old CPU in task_rq_lock(), the acquire of
		 * the old rq->lock will fully serialize against the stores.
		 *
		 * If we observe the new CPU in task_rq_lock(), the address
		 * dependency headed by '[L] rq = task_rq()' and the acquire
		 * will pair with the WMB to ensure we then also see migrating.
		 */
		إذا (likely(rq == task_rq(p) && !task_on_rq_migrating(p))) {
			rq_pin_lock(rq, rf);
			إرجاع rq;
		}
		raw_spin_rq_unlock(rq);
		raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);

		بينما (unlikely(task_on_rq_migrating(p)))
			cpu_relax();
	}
}

/*
 * RQ-clock updating methods:
 */

/* Use CONFIG_PARAVIRT as this will avoid more #ifdef in arch code. */
#ifdef CONFIG_PARAVIRT
DEFINE_STATIC_مفتاح_FALSE(paravirt_steal_rq_enabled);
#endif

ساكن فراغ update_rq_clock_task(بنية rq *rq, s64 delta)
{
/*
 * In theory, the compile should just see 0 here, and optimize out the call
 * to sched_rt_avg_update. But I don't trust it...
 */
	s64 __maybe_unused steal = 0, irq_delta = 0;

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	إذا (irqtime_enabled()) {
		irq_delta = irq_time_read(cpu_of(rq)) - rq->prev_irq_time;

		/*
		 * Since irq_time is only updated on {soft,}irq_exit, we might run into
		 * this case when a previous update_rq_clock() happened inside a
		 * {soft,}IRQ region.
		 *
		 * When this happens, we stop ->clock_task and only update the
		 * prev_irq_time stamp to account for the part that fit, so that a next
		 * update will consume the rest. This ensures ->clock_task is
		 * monotonic.
		 *
		 * It does however cause some slight miss-attribution of {soft,}IRQ
		 * time, a more accurate solution would be to update the irq_time using
		 * the current rq->clock timestamp, except that would require using
		 * atomic ops.
		 */
		إذا (irq_delta > delta)
			irq_delta = delta;

		rq->prev_irq_time += irq_delta;
		delta -= irq_delta;
		delayacct_irq(rq->curr, irq_delta);
	}
#endif
#ifdef CONFIG_PARAVIRT_TIME_ACCOUNTING
	إذا (static_branch_unlikely(&paravirt_steal_rq_enabled)) {
		u64 prev_steal;

		steal = prev_steal = paravirt_steal_clock(cpu_of(rq));
		steal -= rq->prev_steal_time_rq;

		إذا (unlikely(steal > delta))
			steal = delta;

		rq->prev_steal_time_rq = prev_steal;
		delta -= steal;
	}
#endif

	rq->clock_task += delta;

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	إذا ((irq_delta + steal) && sched_feat(NONTASK_CAPACITY))
		update_irq_load_avg(rq, irq_delta + steal);
#endif
	update_rq_clock_pelt(rq, delta);
}

فراغ update_rq_clock(بنية rq *rq)
{
	s64 delta;
	u64 clock;

	lockdep_assert_rq_held(rq);

	إذا (rq->clock_update_flags & RQCF_ACT_SKIP)
		إرجاع;

	إذا (sched_feat(WARN_DOUBLE_CLOCK))
		WARN_ON_ONCE(rq->clock_update_flags & RQCF_UPDATED);
	rq->clock_update_flags |= RQCF_UPDATED;

	clock = sched_clock_cpu(cpu_of(rq));
	scx_rq_clock_update(rq, clock);

	delta = clock - rq->clock;
	إذا (delta < 0)
		إرجاع;
	rq->clock += delta;

	update_rq_clock_task(rq, delta);
}

#ifdef CONFIG_SCHED_HRTICK
/*
 * Use HR-timers to deliver accurate preemption points.
 */

تعداد {
	HRTICK_SCHED_NONE		= 0,
	HRTICK_SCHED_DEFER		= BIT(1),
	HRTICK_SCHED_START		= BIT(2),
	HRTICK_SCHED_REARM_HRTIMER	= BIT(3)
};

ساكن فراغ __used hrtick_clear(بنية rq *rq)
{
	إذا (hrtimer_active(&rq->hrtick_timer))
		hrtimer_cancel(&rq->hrtick_timer);
}

/*
 * High-resolution timer tick.
 * Runs from hardirq context with interrupts disabled.
 */
ساكن تعداد hrtimer_restart hrtick(بنية hrtimer *timer)
{
	بنية rq *rq = container_of(timer, بنية rq, hrtick_timer);
	بنية rq_flags rf;

	WARN_ON_ONCE(cpu_of(rq) != smp_processor_id());

	rq_lock(rq, &rf);
	update_rq_clock(rq);
	rq->donor->sched_class->task_tick(rq, rq->donor, 1);
	rq_unlock(rq, &rf);

	إرجاع HRTIMER_NORESTART;
}

ساكن مضمن bool hrtick_needs_rearm(بنية hrtimer *timer, ktime_t expires)
{
	/*
	 * Queued is false when the timer is not started or currently
	 * running the callback. In both cases, restart. If queued check
	 * whether the expiry time actually changes substantially.
	 */
	إرجاع !hrtimer_is_queued(timer) ||
		abs(expires - hrtimer_get_expires(timer)) > 5000;
}

ساكن فراغ hrtick_cond_restart(بنية rq *rq)
{
	بنية hrtimer *timer = &rq->hrtick_timer;
	ktime_t time = rq->hrtick_time;

	إذا (hrtick_needs_rearm(timer, time))
		hrtimer_start(timer, time, HRTIMER_MODE_ABS_PINNED_HARD);
}

/*
 * called from hardirq (IPI) context
 */
ساكن فراغ __hrtick_start(فراغ *arg)
{
	بنية rq *rq = arg;
	بنية rq_flags rf;

	rq_lock(rq, &rf);
	hrtick_cond_restart(rq);
	rq_unlock(rq, &rf);
}

/*
 * Called to set the hrtick timer state.
 *
 * called with rq->lock held and IRQs disabled
 */
فراغ hrtick_start(بنية rq *rq, u64 delay)
{
	s64 delta;

	/*
	 * Don't schedule slices shorter than 10000ns, that just
	 * doesn't make sense and can cause timer DoS.
	 */
	delta = max_t(s64, delay, 10000LL);

	/*
	 * If this is in the middle of schedule() only note the delay
	 * and let hrtick_schedule_exit() deal with it.
	 */
	إذا (rq->hrtick_sched) {
		rq->hrtick_sched |= HRTICK_SCHED_START;
		rq->hrtick_delay = delta;
		إرجاع;
	}

	rq->hrtick_time = ktime_add_ns(ktime_get(), delta);
	إذا (!hrtick_needs_rearm(&rq->hrtick_timer, rq->hrtick_time))
		إرجاع;

	إذا (rq == this_rq())
		hrtimer_start(&rq->hrtick_timer, rq->hrtick_time, HRTIMER_MODE_ABS_PINNED_HARD);
	وإلا
		smp_call_function_single_async(cpu_of(rq), &rq->hrtick_csd);
}

ساكن مضمن فراغ hrtick_schedule_enter(بنية rq *rq)
{
	rq->hrtick_sched = HRTICK_SCHED_DEFER;
	إذا (hrtimer_test_and_clear_rearm_deferred())
		rq->hrtick_sched |= HRTICK_SCHED_REARM_HRTIMER;
}

ساكن مضمن فراغ hrtick_schedule_exit(بنية rq *rq)
{
	إذا (rq->hrtick_sched & HRTICK_SCHED_START) {
		rq->hrtick_time = ktime_add_ns(ktime_get(), rq->hrtick_delay);
		hrtick_cond_restart(rq);
	} وإلا إذا (idle_rq(rq)) {
		/*
		 * No need for using hrtimer_is_active(). The timer is CPU local
		 * and interrupts are disabled, so the callback cannot be
		 * running and the queued state is valid.
		 */
		إذا (hrtimer_is_queued(&rq->hrtick_timer))
			hrtimer_cancel(&rq->hrtick_timer);
	}

	إذا (rq->hrtick_sched & HRTICK_SCHED_REARM_HRTIMER)
		__hrtimer_rearm_deferred();

	rq->hrtick_sched = HRTICK_SCHED_NONE;
}

ساكن فراغ hrtick_rq_init(بنية rq *rq)
{
	INIT_CSD(&rq->hrtick_csd, __hrtick_start, rq);
	rq->hrtick_sched = HRTICK_SCHED_NONE;
	hrtimer_setup(&rq->hrtick_timer, hrtick, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD | HRTIMER_MODE_LAZY_REARM);
}
#وإلا /* !CONFIG_SCHED_HRTICK: */
ساكن مضمن فراغ hrtick_clear(بنية rq *rq) { }
ساكن مضمن فراغ hrtick_rq_init(بنية rq *rq) { }
ساكن مضمن فراغ hrtick_schedule_enter(بنية rq *rq) { }
ساكن مضمن فراغ hrtick_schedule_exit(بنية rq *rq) { }
#endif /* !CONFIG_SCHED_HRTICK */

/*
 * try_cmpxchg based fetch_or() macro so it works for different integer types:
 */
#define fetch_or(ptr, mask)						\
	({								\
		typeof(ptr) _ptr = (ptr);				\
		typeof(mask) _mask = (mask);				\
		typeof(*_ptr) _val = *_ptr;				\
									\
		افعل {							\
		} بينما (!try_cmpxchg(_ptr, &_val, _val | _mask));	\
	_val;								\
})

#ifdef TIF_POLLING_NRFLAG
/*
 * Atomically set TIF_NEED_RESCHED and test for TIF_POLLING_NRFLAG,
 * this avoids any races wrt polling state changes and thereby avoids
 * spurious IPIs.
 */
ساكن مضمن bool set_nr_and_not_polling(بنية thread_info *ti, صحيح tif)
{
	إرجاع !(fetch_or(&ti->flags, 1 << tif) & _TIF_POLLING_NRFLAG);
}

/*
 * Atomically set TIF_NEED_RESCHED if TIF_POLLING_NRFLAG is set.
 *
 * If this returns true, then the idle task promises to call
 * sched_ttwu_pending() and reschedule soon.
 */
ساكن bool set_nr_if_polling(بنية task_struct *p)
{
	بنية thread_info *ti = task_thread_info(p);
	typeof(ti->flags) val = READ_ONCE(ti->flags);

	افعل {
		إذا (!(val & _TIF_POLLING_NRFLAG))
			إرجاع false;
		إذا (val & _TIF_NEED_RESCHED)
			إرجاع true;
	} بينما (!try_cmpxchg(&ti->flags, &val, val | _TIF_NEED_RESCHED));

	إرجاع true;
}

#وإلا
ساكن مضمن bool set_nr_and_not_polling(بنية thread_info *ti, صحيح tif)
{
	set_ti_thread_flag(ti, tif);
	إرجاع true;
}

ساكن مضمن bool set_nr_if_polling(بنية task_struct *p)
{
	إرجاع false;
}
#endif

ساكن bool __wake_q_add(بنية wake_q_head *head, بنية task_struct *task)
{
	بنية wake_q_عقدة *عقدة = &task->wake_q;

	/*
	 * Atomically grab the task, if ->wake_q is !nil already it means
	 * it's already queued (either by us or someone else) and will get the
	 * wakeup due to that.
	 *
	 * In order to ensure that a pending wakeup will observe our pending
	 * state, even in the failed case, an explicit smp_mb() must be used.
	 */
	smp_mb__before_atomic();
	إذا (unlikely(cmpxchg_relaxed(&عقدة->next, عدم, WAKE_Q_TAIL)))
		إرجاع false;

	/*
	 * The head is context local, there can be no concurrency.
	 */
	*head->lastp = عقدة;
	head->lastp = &عقدة->next;
	إرجاع true;
}

/**
 * wake_q_add() - queue a wakeup for 'later' waking.
 * @head: the wake_q_head to add @task to
 * @task: the task to queue for 'later' wakeup
 *
 * Queue a task for later wakeup, most likely by the wake_up_q() call in the
 * same context, _HOWEVER_ this is not guaranteed, the wakeup can come
 * instantly.
 *
 * This function must be used as-if it were wake_up_process(); IOW the task
 * must be ready to be woken at this location.
 */
فراغ wake_q_add(بنية wake_q_head *head, بنية task_struct *task)
{
	إذا (__wake_q_add(head, task))
		get_task_struct(task);
}

/**
 * wake_q_add_safe() - safely queue a wakeup for 'later' waking.
 * @head: the wake_q_head to add @task to
 * @task: the task to queue for 'later' wakeup
 *
 * Queue a task for later wakeup, most likely by the wake_up_q() call in the
 * same context, _HOWEVER_ this is not guaranteed, the wakeup can come
 * instantly.
 *
 * This function must be used as-if it were wake_up_process(); IOW the task
 * must be ready to be woken at this location.
 *
 * This function is essentially a task-safe equivalent to wake_q_add(). Callers
 * that already hold reference to @task can call the 'safe' version and trust
 * wake_q to do the right thing depending whether or not the @task is already
 * queued for wakeup.
 */
فراغ wake_q_add_safe(بنية wake_q_head *head, بنية task_struct *task)
{
	إذا (!__wake_q_add(head, task))
		put_task_struct(task);
}

فراغ wake_up_q(بنية wake_q_head *head)
{
	بنية wake_q_عقدة *عقدة = head->first;

	بينما (عقدة != WAKE_Q_TAIL) {
		بنية task_struct *task;

		task = container_of(عقدة, بنية task_struct, wake_q);
		عقدة = عقدة->next;
		/* pairs with cmpxchg_relaxed() in __wake_q_add() */
		WRITE_ONCE(task->wake_q.next, عدم);
		/* Task can safely be re-inserted now. */

		/*
		 * wake_up_process() executes a full barrier, which pairs with
		 * the queueing in wake_q_add() so as not to miss wakeups.
		 */
		wake_up_process(task);
		put_task_struct(task);
	}
}

/*
 * resched_curr - mark rq's current task 'to be rescheduled now'.
 *
 * On UP this means the setting of the need_resched flag, on SMP it
 * might also involve a cross-CPU call to trigger the scheduler on
 * the target CPU.
 */
ساكن فراغ __resched_curr(بنية rq *rq, صحيح tif)
{
	بنية task_struct *curr = rq->curr;
	بنية thread_info *cti = task_thread_info(curr);
	bool need_ipi;
	صحيح cpu;

	lockdep_assert_rq_held(rq);

	/*
	 * Always immediately preempt the idle task; no point in delaying doing
	 * actual work.
	 */
	إذا (is_idle_task(curr) && tif == TIF_NEED_RESCHED_LAZY)
		tif = TIF_NEED_RESCHED;

	إذا (cti->flags & ((1 << tif) | _TIF_NEED_RESCHED))
		إرجاع;

	cpu = cpu_of(rq);

	إذا (cpu == smp_processor_id()) {
		set_ti_thread_flag(cti, tif);
		إذا (tif == TIF_NEED_RESCHED)
			set_preempt_need_resched();
		trace_sched_set_need_resched_tp(curr, cpu, tif);
		إرجاع;
	}

	need_ipi = set_nr_and_not_polling(cti, tif);
	trace_sched_set_need_resched_tp(curr, cpu, tif);
	إذا (need_ipi) {
		إذا (tif == TIF_NEED_RESCHED)
			smp_send_reschedule(cpu);
	} وإلا {
		trace_sched_wake_idle_without_ipi(cpu);
	}
}

/*
 * Calls to this function MUST be guarded by a
 * tracepoint_enabled(sched_set_need_resched_tp)
 */
فراغ __trace_set_need_resched(بنية task_struct *curr, صحيح tif)
{
	trace_call__sched_set_need_resched_tp(curr, smp_processor_id(), tif);
}
EXPORT_رمز_GPL(__trace_set_need_resched);

فراغ resched_curr(بنية rq *rq)
{
	__resched_curr(rq, TIF_NEED_RESCHED);
}

#ifdef CONFIG_PREEMPT_DYNAMIC
ساكن DEFINE_STATIC_مفتاح_FALSE(sk_dynamic_preempt_lazy);
ساكن __always_inline bool dynamic_preempt_lazy(فراغ)
{
	إرجاع static_branch_unlikely(&sk_dynamic_preempt_lazy);
}
#وإلا
ساكن __always_inline bool dynamic_preempt_lazy(فراغ)
{
	إرجاع IS_ENABLED(CONFIG_PREEMPT_LAZY);
}
#endif

ساكن __always_inline صحيح get_lazy_tif_bit(فراغ)
{
	إذا (dynamic_preempt_lazy())
		إرجاع TIF_NEED_RESCHED_LAZY;

	إرجاع TIF_NEED_RESCHED;
}

فراغ resched_curr_lazy(بنية rq *rq)
{
	__resched_curr(rq, get_lazy_tif_bit());
}

فراغ resched_cpu(صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);
	unsigned long flags;

	raw_spin_rq_lock_irqsave(rq, flags);
	إذا (cpu_online(cpu) || cpu == smp_processor_id())
		resched_curr(rq);
	raw_spin_rq_unlock_irqrestore(rq, flags);
}

#ifdef CONFIG_NO_HZ_COMMON
/*
 * In the semi idle case, use the nearest busy CPU for migrating timers
 * from an idle CPU.  This is good for power-savings.
 *
 * We don't do similar optimization for completely idle system, as
 * selecting an idle CPU will add more delays to the timers than intended
 * (as that CPU's timer base may not be up to date wrt jiffies etc).
 */
صحيح get_nohz_timer_target(فراغ)
{
	صحيح i, cpu = smp_processor_id(), default_cpu = -1;
	بنية sched_domain *sd;
	ثابت بنية cpumask *hk_mask;

	إذا (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE)) {
		إذا (!idle_cpu(cpu))
			إرجاع cpu;
		default_cpu = cpu;
	}

	hk_mask = housekeeping_cpumask(HK_TYPE_KERNEL_NOISE);

	guard(rcu)();

	for_each_domain(cpu, sd) {
		for_each_cpu_and(i, sched_domain_span(sd), hk_mask) {
			إذا (cpu == i)
				استمر;

			إذا (!idle_cpu(i))
				إرجاع i;
		}
	}

	إذا (default_cpu == -1)
		default_cpu = housekeeping_any_cpu(HK_TYPE_KERNEL_NOISE);

	إرجاع default_cpu;
}

/*
 * When add_timer_on() enqueues a timer into the timer wheel of an
 * idle CPU then this timer might expire before the next timer event
 * which is scheduled to wake up that CPU. In case of a completely
 * idle system the next event might even be infinite time into the
 * future. wake_up_idle_cpu() ensures that the CPU is woken up and
 * leaves the inner idle loop so the newly added timer is taken into
 * account when the CPU goes back to idle and evaluates the timer
 * wheel for the next timer event.
 */
ساكن فراغ wake_up_idle_cpu(صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

	إذا (cpu == smp_processor_id())
		إرجاع;

	/*
	 * Set TIF_NEED_RESCHED and send an IPI if in the non-polling
	 * part of the idle loop. This forces an exit from the idle loop
	 * and a round trip to schedule(). Now this could be optimized
	 * because a simple new idle loop iteration is enough to
	 * re-evaluate the next tick. Provided some re-ordering of tick
	 * nohz functions that would need to follow TIF_NR_POLLING
	 * clearing:
	 *
	 * - On most architectures, a simple fetch_or on ti::flags with a
	 *   "0" value would be enough to know if an IPI needs to be sent.
	 *
	 * - x86 needs to perform a last need_resched() check between
	 *   monitor and mwait which doesn't take timers into account.
	 *   There a dedicated TIF_TIMER flag would be required to
	 *   fetch_or here and be checked along with TIF_NEED_RESCHED
	 *   before mwait().
	 *
	 * However, remote timer enqueue is not such a frequent event
	 * and testing of the above solutions didn't appear to report
	 * much benefits.
	 */
	إذا (set_nr_and_not_polling(task_thread_info(rq->idle), TIF_NEED_RESCHED))
		smp_send_reschedule(cpu);
	وإلا
		trace_sched_wake_idle_without_ipi(cpu);
}

ساكن bool wake_up_full_nohz_cpu(صحيح cpu)
{
	/*
	 * We just need the target to call irq_exit() and re-evaluate
	 * the next tick. The nohz full kick at least implies that.
	 * If needed we can still optimize that later with an
	 * empty IRQ.
	 */
	إذا (cpu_is_offline(cpu))
		إرجاع true;  /* Don't try to wake offline CPUs. */
	إذا (tick_nohz_full_cpu(cpu)) {
		إذا (cpu != smp_processor_id() ||
		    tick_nohz_tick_stopped())
			tick_nohz_full_kick_cpu(cpu);
		إرجاع true;
	}

	إرجاع false;
}

/*
 * Wake up the specified CPU.  If the CPU is going offline, it is the
 * caller's responsibility to deal with the lost wakeup, for example,
 * by hooking into the CPU_DEAD notifier like timers and hrtimers do.
 */
فراغ wake_up_nohz_cpu(صحيح cpu)
{
	إذا (!wake_up_full_nohz_cpu(cpu))
		wake_up_idle_cpu(cpu);
}

ساكن فراغ nohz_csd_func(فراغ *info)
{
	بنية rq *rq = info;
	صحيح cpu = cpu_of(rq);
	unsigned صحيح flags;

	/*
	 * Release the rq::nohz_csd.
	 */
	flags = atomic_fetch_andnot(NOHZ_KICK_MASK | NOHZ_NEWILB_KICK, nohz_flags(cpu));
	WARN_ON(!(flags & NOHZ_KICK_MASK));

	rq->idle_balance = idle_cpu(cpu);
	إذا (rq->idle_balance) {
		rq->nohz_idle_balance = flags;
		__raise_softirq_irqoff(SCHED_SOFTIRQ);
	}
}

#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_FULL
ساكن مضمن bool __need_bw_check(بنية rq *rq, بنية task_struct *p)
{
	إذا (rq->nr_running != 1)
		إرجاع false;

	إذا (p->sched_class != &fair_sched_class)
		إرجاع false;

	إذا (!task_on_rq_queued(p))
		إرجاع false;

	إرجاع true;
}

bool sched_can_stop_tick(بنية rq *rq)
{
	صحيح fifo_nr_running;

	/* Deadline tasks, even if single, need the tick */
	إذا (rq->dl.dl_nr_running)
		إرجاع false;

	/*
	 * If there are more than one RR tasks, we need the tick to affect the
	 * actual RR behaviour.
	 */
	إذا (rq->rt.rr_nr_running) {
		إذا (rq->rt.rr_nr_running == 1)
			إرجاع true;
		وإلا
			إرجاع false;
	}

	/*
	 * If there's no RR tasks, but FIFO tasks, we can skip the tick, no
	 * forced preemption between FIFO tasks.
	 */
	fifo_nr_running = rq->rt.rt_nr_running - rq->rt.rr_nr_running;
	إذا (fifo_nr_running)
		إرجاع true;

	/*
	 * If there are no DL,RR/FIFO tasks, there must only be CFS or SCX tasks
	 * left. For CFS, if there's more than one we need the tick for
	 * involuntary preemption. For SCX, ask.
	 */
	إذا (scx_enabled() && !scx_can_stop_tick(rq))
		إرجاع false;

	إذا (rq->cfs.h_nr_queued > 1)
		إرجاع false;

	/*
	 * If there is one task and it has CFS runtime bandwidth constraints
	 * and it's on the cpu now we don't want to stop the tick.
	 * This check prevents clearing the bit if a newly enqueued task here is
	 * dequeued by migrating while the constrained task continues to run.
	 * E.g. going from 2->1 without going through pick_next_task().
	 */
	إذا (__need_bw_check(rq, rq->curr)) {
		إذا (cfs_task_bw_constrained(rq->curr))
			إرجاع false;
	}

	إرجاع true;
}
#endif /* CONFIG_NO_HZ_FULL */

#إذا defined(CONFIG_RT_GROUP_SCHED) || defined(CONFIG_FAIR_GROUP_SCHED)
/*
 * Iterate task_group tree rooted at *from, calling @down when first entering a
 * node and @up when leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
صحيح walk_tg_شجرة_from(بنية task_group *from,
			     tg_visitor down, tg_visitor up, فراغ *data)
{
	بنية task_group *parent, *child;
	صحيح ret;

	parent = from;

down:
	ret = (*down)(parent, data);
	إذا (ret)
		اذهب out;
	list_for_each_entry_rcu(child, &parent->children, siblings) {
		parent = child;
		اذهب down;

up:
		استمر;
	}
	ret = (*up)(parent, data);
	إذا (ret || parent == from)
		اذهب out;

	child = parent;
	parent = parent->parent;
	إذا (parent)
		اذهب up;
out:
	إرجاع ret;
}

صحيح tg_nop(بنية task_group *tg, فراغ *data)
{
	إرجاع 0;
}
#endif

فراغ set_load_weight(بنية task_struct *p, bool update_load)
{
	صحيح prio = p->static_prio - MAX_RT_PRIO;
	بنية load_weight lw;

	إذا (task_has_idle_policy(p)) {
		lw.weight = scale_load(WEIGHT_IDLEPRIO);
		lw.inv_weight = WMULT_IDLEPRIO;
	} وإلا {
		lw.weight = scale_load(sched_prio_to_weight[prio]);
		lw.inv_weight = sched_prio_to_wmult[prio];
	}

	/*
	 * SCHED_OTHER tasks have to update their load when changing their
	 * weight
	 */
	إذا (update_load && p->sched_class->reweight_task)
		p->sched_class->reweight_task(task_rq(p), p, &lw);
	وإلا
		p->se.load = lw;
}

#ifdef CONFIG_UCLAMP_TASK
/*
 * Serializes updates of utilization clamp values
 *
 * The (slow-path) user-space triggers utilization clamp value updates which
 * can require updates on (fast-path) scheduler's data structures used to
 * support enqueue/dequeue operations.
 * While the per-CPU rq lock protects fast-path update operations, user-space
 * requests are serialized using a mutex to reduce the risk of conflicting
 * updates or API abuses.
 */
ساكن __maybe_unused DEFINE_MUTEX(uclamp_mutex);

/* Max allowed minimum utilization */
ساكن unsigned صحيح __maybe_unused sysctl_sched_uclamp_util_min = SCHED_CAPACITY_SCALE;

/* Max allowed maximum utilization */
ساكن unsigned صحيح __maybe_unused sysctl_sched_uclamp_util_max = SCHED_CAPACITY_SCALE;

/*
 * By default RT tasks run at the maximum performance point/capacity of the
 * system. Uclamp enforces this by always setting UCLAMP_MIN of RT tasks to
 * SCHED_CAPACITY_SCALE.
 *
 * This knob allows admins to change the default behavior when uclamp is being
 * used. In battery powered devices, particularly, running at the maximum
 * capacity and frequency will increase energy consumption and shorten the
 * battery life.
 *
 * This knob only affects RT tasks that their uclamp_se->user_defined == false.
 *
 * This knob will not override the system default sched_util_clamp_min defined
 * above.
 */
unsigned صحيح sysctl_sched_uclamp_util_min_rt_default = SCHED_CAPACITY_SCALE;

/* All clamps are required to be less or equal than these values */
ساكن بنية uclamp_se uclamp_default[UCLAMP_CNT];

/*
 * This static key is used to reduce the uclamp overhead in the fast path. It
 * primarily disables the call to uclamp_rq_{inc, dec}() in
 * enqueue/dequeue_task().
 *
 * This allows users to continue to enable uclamp in their kernel config with
 * minimum uclamp overhead in the fast path.
 *
 * As soon as userspace modifies any of the uclamp knobs, the static key is
 * enabled, since we have an actual users that make use of uclamp
 * functionality.
 *
 * The knobs that would enable this static key are:
 *
 *   * A task modifying its uclamp value with sched_setattr().
 *   * An admin modifying the sysctl_sched_uclamp_{min, max} via procfs.
 *   * An admin modifying the cgroup cpu.uclamp.{min, max}
 */
DEFINE_STATIC_مفتاح_FALSE(sched_uclamp_used);

ساكن مضمن unsigned صحيح
uclamp_idle_قيمة(بنية rq *rq, تعداد uclamp_id clamp_id,
		  unsigned صحيح clamp_قيمة)
{
	/*
	 * Avoid blocked utilization pushing up the frequency when we go
	 * idle (which drops the max-clamp) by retaining the last known
	 * max-clamp.
	 */
	إذا (clamp_id == UCLAMP_MAX) {
		rq->uclamp_flags |= UCLAMP_FLAG_IDLE;
		إرجاع clamp_قيمة;
	}

	إرجاع uclamp_none(UCLAMP_MIN);
}

ساكن مضمن فراغ uclamp_idle_reset(بنية rq *rq, تعداد uclamp_id clamp_id,
				     unsigned صحيح clamp_قيمة)
{
	/* Reset max-clamp retention only on idle exit */
	إذا (!(rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		إرجاع;

	uclamp_rq_set(rq, clamp_id, clamp_قيمة);
}

ساكن مضمن
unsigned صحيح uclamp_rq_max_قيمة(بنية rq *rq, تعداد uclamp_id clamp_id,
				   unsigned صحيح clamp_قيمة)
{
	بنية uclamp_bucket *bucket = rq->uclamp[clamp_id].bucket;
	صحيح bucket_id = UCLAMP_BUCKETS - 1;

	/*
	 * Since both min and max clamps are max aggregated, find the
	 * top most bucket with tasks in.
	 */
	لكل ( ; bucket_id >= 0; bucket_id--) {
		إذا (!bucket[bucket_id].tasks)
			استمر;
		إرجاع bucket[bucket_id].قيمة;
	}

	/* No tasks -- default clamp values */
	إرجاع uclamp_idle_قيمة(rq, clamp_id, clamp_قيمة);
}

ساكن فراغ __uclamp_update_util_min_rt_default(بنية task_struct *p)
{
	unsigned صحيح default_util_min;
	بنية uclamp_se *uc_se;

	lockdep_assert_held(&p->pi_lock);

	uc_se = &p->uclamp_req[UCLAMP_MIN];

	/* Only sync if user didn't override the default */
	إذا (uc_se->user_defined)
		إرجاع;

	default_util_min = sysctl_sched_uclamp_util_min_rt_default;
	uclamp_se_set(uc_se, default_util_min, false);
}

ساكن فراغ uclamp_update_util_min_rt_default(بنية task_struct *p)
{
	إذا (!rt_task(p))
		إرجاع;

	/* Protect updates to p->uclamp_* */
	guard(task_rq_lock)(p);
	__uclamp_update_util_min_rt_default(p);
}

ساكن مضمن بنية uclamp_se
uclamp_tg_restrict(بنية task_struct *p, تعداد uclamp_id clamp_id)
{
	/* Copy by value as we could modify it */
	بنية uclamp_se uc_req = p->uclamp_req[clamp_id];
#ifdef CONFIG_UCLAMP_TASK_GROUP
	unsigned صحيح tg_min, tg_max, قيمة;

	/*
	 * Tasks in autogroups or root task group will be
	 * restricted by system defaults.
	 */
	إذا (task_group_is_autogroup(task_group(p)))
		إرجاع uc_req;
	إذا (task_group(p) == &جذر_task_group)
		إرجاع uc_req;

	tg_min = task_group(p)->uclamp[UCLAMP_MIN].قيمة;
	tg_max = task_group(p)->uclamp[UCLAMP_MAX].قيمة;
	قيمة = uc_req.قيمة;
	قيمة = clamp(قيمة, tg_min, tg_max);
	uclamp_se_set(&uc_req, قيمة, false);
#endif

	إرجاع uc_req;
}

/*
 * The effective clamp bucket index of a task depends on, by increasing
 * priority:
 * - the task specific clamp value, when explicitly requested from userspace
 * - the task group effective clamp value, for tasks not either in the root
 *   group or in an autogroup
 * - the system default clamp value, defined by the sysadmin
 */
ساكن مضمن بنية uclamp_se
uclamp_eff_get(بنية task_struct *p, تعداد uclamp_id clamp_id)
{
	بنية uclamp_se uc_req = uclamp_tg_restrict(p, clamp_id);
	بنية uclamp_se uc_max = uclamp_default[clamp_id];

	/* System default restrictions always apply */
	إذا (unlikely(uc_req.قيمة > uc_max.قيمة))
		إرجاع uc_max;

	إرجاع uc_req;
}

unsigned long uclamp_eff_قيمة(بنية task_struct *p, تعداد uclamp_id clamp_id)
{
	بنية uclamp_se uc_eff;

	/* Task currently refcounted: use back-annotated (effective) value */
	إذا (p->uclamp[clamp_id].active)
		إرجاع (unsigned long)p->uclamp[clamp_id].قيمة;

	uc_eff = uclamp_eff_get(p, clamp_id);

	إرجاع (unsigned long)uc_eff.قيمة;
}

/*
 * When a task is enqueued on a rq, the clamp bucket currently defined by the
 * task's uclamp::bucket_id is refcounted on that rq. This also immediately
 * updates the rq's clamp value if required.
 *
 * Tasks can have a task-specific value requested from user-space, track
 * within each bucket the maximum value for tasks refcounted in it.
 * This "local max aggregation" allows to track the exact "requested" value
 * for each bucket when all its RUNNABLE tasks require the same clamp.
 */
ساكن مضمن فراغ uclamp_rq_inc_id(بنية rq *rq, بنية task_struct *p,
				    تعداد uclamp_id clamp_id)
{
	بنية uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	بنية uclamp_se *uc_se = &p->uclamp[clamp_id];
	بنية uclamp_bucket *bucket;

	lockdep_assert_rq_held(rq);

	/* Update task effective clamp */
	p->uclamp[clamp_id] = uclamp_eff_get(p, clamp_id);

	bucket = &uc_rq->bucket[uc_se->bucket_id];
	bucket->tasks++;
	uc_se->active = true;

	uclamp_idle_reset(rq, clamp_id, uc_se->قيمة);

	/*
	 * Local max aggregation: rq buckets always track the max
	 * "requested" clamp value of its RUNNABLE tasks.
	 */
	إذا (bucket->tasks == 1 || uc_se->قيمة > bucket->قيمة)
		bucket->قيمة = uc_se->قيمة;

	إذا (uc_se->قيمة > uclamp_rq_get(rq, clamp_id))
		uclamp_rq_set(rq, clamp_id, uc_se->قيمة);
}

/*
 * When a task is dequeued from a rq, the clamp bucket refcounted by the task
 * is released. If this is the last task reference counting the rq's max
 * active clamp value, then the rq's clamp value is updated.
 *
 * Both refcounted tasks and rq's cached clamp values are expected to be
 * always valid. If it's detected they are not, as defensive programming,
 * enforce the expected state and warn.
 */
ساكن مضمن فراغ uclamp_rq_dec_id(بنية rq *rq, بنية task_struct *p,
				    تعداد uclamp_id clamp_id)
{
	بنية uclamp_rq *uc_rq = &rq->uclamp[clamp_id];
	بنية uclamp_se *uc_se = &p->uclamp[clamp_id];
	بنية uclamp_bucket *bucket;
	unsigned صحيح bkt_clamp;
	unsigned صحيح rq_clamp;

	lockdep_assert_rq_held(rq);

	/*
	 * If sched_uclamp_used was enabled after task @p was enqueued,
	 * we could end up with unbalanced call to uclamp_rq_dec_id().
	 *
	 * In this case the uc_se->active flag should be false since no uclamp
	 * accounting was performed at enqueue time and we can just return
	 * here.
	 *
	 * Need to be careful of the following enqueue/dequeue ordering
	 * problem too
	 *
	 *	enqueue(taskA)
	 *	// sched_uclamp_used gets enabled
	 *	enqueue(taskB)
	 *	dequeue(taskA)
	 *	// Must not decrement bucket->tasks here
	 *	dequeue(taskB)
	 *
	 * where we could end up with stale data in uc_se and
	 * bucket[uc_se->bucket_id].
	 *
	 * The following check here eliminates the possibility of such race.
	 */
	إذا (unlikely(!uc_se->active))
		إرجاع;

	bucket = &uc_rq->bucket[uc_se->bucket_id];

	WARN_ON_ONCE(!bucket->tasks);
	إذا (likely(bucket->tasks))
		bucket->tasks--;

	uc_se->active = false;

	/*
	 * Keep "local max aggregation" simple and accept to (possibly)
	 * overboost some RUNNABLE tasks in the same bucket.
	 * The rq clamp bucket value is reset to its base value whenever
	 * there are no more RUNNABLE tasks refcounting it.
	 */
	إذا (likely(bucket->tasks))
		إرجاع;

	rq_clamp = uclamp_rq_get(rq, clamp_id);
	/*
	 * Defensive programming: this should never happen. If it happens,
	 * e.g. due to future modification, warn and fix up the expected value.
	 */
	WARN_ON_ONCE(bucket->قيمة > rq_clamp);
	إذا (bucket->قيمة >= rq_clamp) {
		bkt_clamp = uclamp_rq_max_قيمة(rq, clamp_id, uc_se->قيمة);
		uclamp_rq_set(rq, clamp_id, bkt_clamp);
	}
}

ساكن مضمن فراغ uclamp_rq_inc(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	تعداد uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	إذا (!uclamp_is_used())
		إرجاع;

	إذا (unlikely(!p->sched_class->uclamp_enabled))
		إرجاع;

	/* Only inc the delayed task which being woken up. */
	إذا (p->se.sched_delayed && !(flags & ENQUEUE_DELAYED))
		إرجاع;

	for_each_clamp_id(clamp_id)
		uclamp_rq_inc_id(rq, p, clamp_id);

	/* Reset clamp idle holding when there is one RUNNABLE task */
	إذا (rq->uclamp_flags & UCLAMP_FLAG_IDLE)
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ساكن مضمن فراغ uclamp_rq_dec(بنية rq *rq, بنية task_struct *p)
{
	تعداد uclamp_id clamp_id;

	/*
	 * Avoid any overhead until uclamp is actually used by the userspace.
	 *
	 * The condition is constructed such that a NOP is generated when
	 * sched_uclamp_used is disabled.
	 */
	إذا (!uclamp_is_used())
		إرجاع;

	إذا (unlikely(!p->sched_class->uclamp_enabled))
		إرجاع;

	إذا (p->se.sched_delayed)
		إرجاع;

	for_each_clamp_id(clamp_id)
		uclamp_rq_dec_id(rq, p, clamp_id);
}

ساكن مضمن فراغ uclamp_rq_reinc_id(بنية rq *rq, بنية task_struct *p,
				      تعداد uclamp_id clamp_id)
{
	إذا (!p->uclamp[clamp_id].active)
		إرجاع;

	uclamp_rq_dec_id(rq, p, clamp_id);
	uclamp_rq_inc_id(rq, p, clamp_id);

	/*
	 * Make sure to clear the idle flag if we've transiently reached 0
	 * active tasks on rq.
	 */
	إذا (clamp_id == UCLAMP_MAX && (rq->uclamp_flags & UCLAMP_FLAG_IDLE))
		rq->uclamp_flags &= ~UCLAMP_FLAG_IDLE;
}

ساكن مضمن فراغ
uclamp_update_active(بنية task_struct *p)
{
	تعداد uclamp_id clamp_id;
	بنية rq_flags rf;
	بنية rq *rq;

	/*
	 * Lock the task and the rq where the task is (or was) queued.
	 *
	 * We might lock the (previous) rq of a !RUNNABLE task, but that's the
	 * price to pay to safely serialize util_{min,max} updates with
	 * enqueues, dequeues and migration operations.
	 * This is the same locking schema used by __set_cpus_allowed_ptr().
	 */
	rq = task_rq_lock(p, &rf);

	/*
	 * Setting the clamp bucket is serialized by task_rq_lock().
	 * If the task is not yet RUNNABLE and its task_struct is not
	 * affecting a valid clamp bucket, the next time it's enqueued,
	 * it will already see the updated clamp bucket value.
	 */
	for_each_clamp_id(clamp_id)
		uclamp_rq_reinc_id(rq, p, clamp_id);

	task_rq_unlock(rq, p, &rf);
}

#ifdef CONFIG_UCLAMP_TASK_GROUP
ساكن مضمن فراغ
uclamp_update_active_tasks(بنية cgroup_subsys_state *css)
{
	بنية css_task_iter it;
	بنية task_struct *p;

	css_task_iter_start(css, 0, &it);
	بينما ((p = css_task_iter_next(&it)))
		uclamp_update_active(p);
	css_task_iter_end(&it);
}

ساكن فراغ cpu_util_update_eff(بنية cgroup_subsys_state *css);
#endif

#ifdef CONFIG_SYSCTL
#ifdef CONFIG_UCLAMP_TASK_GROUP
ساكن فراغ uclamp_update_جذر_tg(فراغ)
{
	بنية task_group *tg = &جذر_task_group;

	uclamp_se_set(&tg->uclamp_req[UCLAMP_MIN],
		      sysctl_sched_uclamp_util_min, false);
	uclamp_se_set(&tg->uclamp_req[UCLAMP_MAX],
		      sysctl_sched_uclamp_util_max, false);

	guard(rcu)();
	cpu_util_update_eff(&جذر_task_group.css);
}
#وإلا
ساكن فراغ uclamp_update_جذر_tg(فراغ) { }
#endif

ساكن فراغ uclamp_sync_util_min_rt_default(فراغ)
{
	بنية task_struct *g, *p;

	/*
	 * copy_process()			sysctl_uclamp
	 *					  uclamp_min_rt = X;
	 *   write_lock(&tasklist_lock)		  read_lock(&tasklist_lock)
	 *   // link thread			  smp_mb__after_spinlock()
	 *   write_unlock(&tasklist_lock)	  read_unlock(&tasklist_lock);
	 *   sched_post_fork()			  for_each_process_thread()
	 *     __uclamp_sync_rt()		    __uclamp_sync_rt()
	 *
	 * Ensures that either sched_post_fork() will observe the new
	 * uclamp_min_rt or for_each_process_thread() will observe the new
	 * task.
	 */
	read_lock(&tasklist_lock);
	smp_mb__after_spinlock();
	read_unlock(&tasklist_lock);

	guard(rcu)();
	for_each_process_thread(g, p)
		uclamp_update_util_min_rt_default(p);
}

ساكن صحيح sysctl_sched_uclamp_handler(ثابت بنية ctl_table *table, صحيح اكتب,
				فراغ *buffer, size_t *lenp, loff_t *ppos)
{
	bool update_جذر_tg = false;
	صحيح old_min, old_max, old_min_rt;
	صحيح result;

	guard(mutex)(&uclamp_mutex);

	old_min = sysctl_sched_uclamp_util_min;
	old_max = sysctl_sched_uclamp_util_max;
	old_min_rt = sysctl_sched_uclamp_util_min_rt_default;

	result = proc_dointvec(table, اكتب, buffer, lenp, ppos);
	إذا (result)
		اذهب undo;
	إذا (!اكتب)
		إرجاع 0;

	إذا (sysctl_sched_uclamp_util_min > sysctl_sched_uclamp_util_max ||
	    sysctl_sched_uclamp_util_max > SCHED_CAPACITY_SCALE	||
	    sysctl_sched_uclamp_util_min_rt_default > SCHED_CAPACITY_SCALE) {

		result = -EINVAL;
		اذهب undo;
	}

	إذا (old_min != sysctl_sched_uclamp_util_min) {
		uclamp_se_set(&uclamp_default[UCLAMP_MIN],
			      sysctl_sched_uclamp_util_min, false);
		update_جذر_tg = true;
	}
	إذا (old_max != sysctl_sched_uclamp_util_max) {
		uclamp_se_set(&uclamp_default[UCLAMP_MAX],
			      sysctl_sched_uclamp_util_max, false);
		update_جذر_tg = true;
	}

	إذا (update_جذر_tg) {
		sched_uclamp_enable();
		uclamp_update_جذر_tg();
	}

	إذا (old_min_rt != sysctl_sched_uclamp_util_min_rt_default) {
		sched_uclamp_enable();
		uclamp_sync_util_min_rt_default();
	}

	/*
	 * We update all RUNNABLE tasks only when task groups are in use.
	 * Otherwise, keep it simple and do just a lazy update at each next
	 * task enqueue time.
	 */
	إرجاع 0;

undo:
	sysctl_sched_uclamp_util_min = old_min;
	sysctl_sched_uclamp_util_max = old_max;
	sysctl_sched_uclamp_util_min_rt_default = old_min_rt;
	إرجاع result;
}
#endif /* CONFIG_SYSCTL */

ساكن فراغ uclamp_fork(بنية task_struct *p)
{
	تعداد uclamp_id clamp_id;

	/*
	 * We don't need to hold task_rq_lock() when updating p->uclamp_* here
	 * as the task is still at its early fork stages.
	 */
	for_each_clamp_id(clamp_id)
		p->uclamp[clamp_id].active = false;

	إذا (likely(!p->sched_reset_on_fork))
		إرجاع;

	for_each_clamp_id(clamp_id) {
		uclamp_se_set(&p->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}
}

ساكن فراغ uclamp_post_fork(بنية task_struct *p)
{
	uclamp_update_util_min_rt_default(p);
}

ساكن فراغ __init init_uclamp_rq(بنية rq *rq)
{
	تعداد uclamp_id clamp_id;
	بنية uclamp_rq *uc_rq = rq->uclamp;

	for_each_clamp_id(clamp_id) {
		uc_rq[clamp_id] = (بنية uclamp_rq) {
			.قيمة = uclamp_none(clamp_id)
		};
	}

	rq->uclamp_flags = UCLAMP_FLAG_IDLE;
}

ساكن فراغ __init init_uclamp(فراغ)
{
	بنية uclamp_se uc_max = {};
	تعداد uclamp_id clamp_id;
	صحيح cpu;

	for_each_possible_cpu(cpu)
		init_uclamp_rq(cpu_rq(cpu));

	for_each_clamp_id(clamp_id) {
		uclamp_se_set(&init_task.uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
	}

	/* System defaults allow max clamp values for both indexes */
	uclamp_se_set(&uc_max, uclamp_none(UCLAMP_MAX), false);
	for_each_clamp_id(clamp_id) {
		uclamp_default[clamp_id] = uc_max;
#ifdef CONFIG_UCLAMP_TASK_GROUP
		جذر_task_group.uclamp_req[clamp_id] = uc_max;
		جذر_task_group.uclamp[clamp_id] = uc_max;
#endif
	}
}

#وإلا /* !CONFIG_UCLAMP_TASK: */
ساكن مضمن فراغ uclamp_rq_inc(بنية rq *rq, بنية task_struct *p, صحيح flags) { }
ساكن مضمن فراغ uclamp_rq_dec(بنية rq *rq, بنية task_struct *p) { }
ساكن مضمن فراغ uclamp_fork(بنية task_struct *p) { }
ساكن مضمن فراغ uclamp_post_fork(بنية task_struct *p) { }
ساكن مضمن فراغ init_uclamp(فراغ) { }
#endif /* !CONFIG_UCLAMP_TASK */

bool sched_task_on_rq(بنية task_struct *p)
{
	إرجاع task_on_rq_queued(p);
}

unsigned long get_wchan(بنية task_struct *p)
{
	unsigned long ip = 0;
	unsigned صحيح state;

	إذا (!p || p == current)
		إرجاع 0;

	/* Only get wchan if task is blocked and we can keep it that way. */
	raw_spin_lock_irq(&p->pi_lock);
	state = READ_ONCE(p->__state);
	smp_rmb(); /* see try_to_wake_up() */
	إذا (state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq)
		ip = __get_wchan(p);
	raw_spin_unlock_irq(&p->pi_lock);

	إرجاع ip;
}

فراغ enqueue_task(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	إذا (!(flags & ENQUEUE_NOCLOCK))
		update_rq_clock(rq);

	/*
	 * Can be before ->enqueue_task() because uclamp considers the
	 * ENQUEUE_DELAYED task before its ->sched_delayed gets cleared
	 * in ->enqueue_task().
	 */
	uclamp_rq_inc(rq, p, flags);

	p->sched_class->enqueue_task(rq, p, flags);

	psi_enqueue(p, flags);

	إذا (!(flags & ENQUEUE_RESTORE))
		sched_info_enqueue(rq, p);

	إذا (sched_core_enabled(rq))
		sched_core_enqueue(rq, p);
}

/*
 * Must only return false when DEQUEUE_SLEEP.
 */
مضمن bool dequeue_task(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	إذا (sched_core_enabled(rq))
		sched_core_dequeue(rq, p, flags);

	إذا (!(flags & DEQUEUE_NOCLOCK))
		update_rq_clock(rq);

	إذا (!(flags & DEQUEUE_SAVE))
		sched_info_dequeue(rq, p);

	psi_dequeue(p, flags);

	/*
	 * Must be before ->dequeue_task() because ->dequeue_task() can 'fail'
	 * and mark the task ->sched_delayed.
	 */
	uclamp_rq_dec(rq, p);
	إرجاع p->sched_class->dequeue_task(rq, p, flags);
}

فراغ activate_task(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	إذا (task_on_rq_migrating(p))
		flags |= ENQUEUE_MIGRATED;

	enqueue_task(rq, p, flags);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_QUEUED);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);
}

فراغ deactivate_task(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	WARN_ON_ONCE(flags & DEQUEUE_SLEEP);

	WRITE_ONCE(p->on_rq, TASK_ON_RQ_MIGRATING);
	ASSERT_EXCLUSIVE_WRITER(p->on_rq);

	/*
	 * Code explicitly relies on TASK_ON_RQ_MIGRATING begin set *before*
	 * dequeue_task() and cleared *after* enqueue_task().
	 */

	dequeue_task(rq, p, flags);
}

ساكن bool dequeue_block_task(بنية rq *rq, بنية task_struct *p,
			       unsigned long task_state)
{
	صحيح flags = DEQUEUE_NOCLOCK;

	p->sched_contributes_to_load =
		(task_state & TASK_UNINTERRUPTIBLE) &&
		!(task_state & TASK_NOLOAD) &&
		!(task_state & TASK_FROZEN);

	إذا (unlikely(is_special_task_state(task_state)))
		flags |= DEQUEUE_SPECIAL;

	/*
	 * __schedule()			ttwu()
	 *   prev_state = prev->state;    if (p->on_rq && ...)
	 *   if (prev_state)		    goto out;
	 *     p->on_rq = 0;		  smp_acquire__after_ctrl_dep();
	 *				  p->state = TASK_WAKING
	 *
	 * Where __schedule() and ttwu() have matching control dependencies.
	 *
	 * Once the caller invokes __block_task(), schedule() must not care about
	 * p->state any more.
	 */
	إرجاع dequeue_task(rq, p, DEQUEUE_SLEEP | flags);
}

ساكن فراغ block_task(بنية rq *rq, بنية task_struct *p, unsigned long task_state)
{
	إذا (dequeue_block_task(rq, p, task_state))
		__block_task(rq, p);
}

/**
 * task_curr - is this task currently executing on a CPU?
 * @p: the task in question.
 *
 * Return: 1 if the task is currently executing. 0 otherwise.
 */
مضمن صحيح task_curr(ثابت بنية task_struct *p)
{
	إرجاع cpu_curr(task_cpu(p)) == p;
}

فراغ wakeup_preempt(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	بنية task_struct *donor = rq->donor;

	إذا (p->sched_class == rq->next_class) {
		rq->next_class->wakeup_preempt(rq, p, flags);

	} وإلا إذا (sched_class_above(p->sched_class, rq->next_class)) {
		rq->next_class->wakeup_preempt(rq, p, flags);
		resched_curr(rq);
		rq->next_class = p->sched_class;
	}

	/*
	 * A queue event has occurred, and we're going to schedule.  In
	 * this case, we can save a useless back to back clock update.
	 */
	إذا (task_on_rq_queued(donor) && test_tsk_need_resched(rq->curr))
		rq_clock_skip_update(rq);
}

ساكن __always_inline
صحيح __task_state_match(بنية task_struct *p, unsigned صحيح state)
{
	إذا (READ_ONCE(p->__state) & state)
		إرجاع 1;

	إذا (READ_ONCE(p->saved_state) & state)
		إرجاع -1;

	إرجاع 0;
}

ساكن __always_inline
صحيح task_state_match(بنية task_struct *p, unsigned صحيح state)
{
	/*
	 * Serialize against current_save_and_set_rtlock_wait_state(),
	 * current_restore_rtlock_saved_state(), and __refrigerator().
	 */
	guard(raw_spinlock_irq)(&p->pi_lock);
	إرجاع __task_state_match(p, state);
}

/*
 * wait_task_inactive - wait for a thread to unschedule.
 *
 * Wait for the thread to block in any of the states set in @match_state.
 * If it changes, i.e. @p might have woken up, then return zero.  When we
 * succeed in waiting for @p to be off its CPU, we return a positive number
 * (its total switch count).  If a second call a short while later returns the
 * same number, the caller can be sure that @p has remained unscheduled the
 * whole time.
 *
 * The caller must ensure that the task *will* unschedule sometime soon,
 * else this function might spin for a *long* time. This function can't
 * be called with interrupts off, or it may introduce deadlock with
 * smp_call_function() if an IPI is sent by the same process we are
 * waiting to become inactive.
 */
unsigned long wait_task_inactive(بنية task_struct *p, unsigned صحيح match_state)
{
	صحيح running, queued, match;
	بنية rq_flags rf;
	unsigned long ncsw;
	بنية rq *rq;

	لكل (;;) {
		/*
		 * We do the initial early heuristics without holding
		 * any task-queue locks at all. We'll only try to get
		 * the runqueue lock when things look like they will
		 * work out!
		 */
		rq = task_rq(p);

		/*
		 * If the task is actively running on another CPU
		 * still, just relax and busy-wait without holding
		 * any locks.
		 *
		 * NOTE! Since we don't hold any locks, it's not
		 * even sure that "rq" stays as the right runqueue!
		 * But we don't care, since "task_on_cpu()" will
		 * return false if the runqueue has changed and p
		 * is actually now running somewhere else!
		 */
		بينما (task_on_cpu(rq, p)) {
			إذا (!task_state_match(p, match_state))
				إرجاع 0;
			cpu_relax();
		}

		/*
		 * Ok, time to look more closely! We need the rq
		 * lock now, to be *sure*. If we're wrong, we'll
		 * just go back and repeat.
		 */
		rq = task_rq_lock(p, &rf);
		/*
		 * If task is sched_delayed, force dequeue it, to avoid always
		 * hitting the tick timeout in the queued case
		 */
		إذا (p->se.sched_delayed)
			dequeue_task(rq, p, DEQUEUE_SLEEP | DEQUEUE_DELAYED);
		trace_sched_wait_task(p);
		running = task_on_cpu(rq, p);
		queued = task_on_rq_queued(p);
		ncsw = 0;
		إذا ((match = __task_state_match(p, match_state))) {
			/*
			 * When matching on p->saved_state, consider this task
			 * still queued so it will wait.
			 */
			إذا (match < 0)
				queued = 1;
			ncsw = p->nvcsw | LONG_MIN; /* sets MSB */
		}
		task_rq_unlock(rq, p, &rf);

		/*
		 * If it changed from the expected state, bail out now.
		 */
		إذا (unlikely(!ncsw))
			اكسر;

		/*
		 * Was it really running after all now that we
		 * checked with the proper locks actually held?
		 *
		 * Oops. Go back and try again..
		 */
		إذا (unlikely(running)) {
			cpu_relax();
			استمر;
		}

		/*
		 * It's not enough that it's not actively running,
		 * it must be off the runqueue _entirely_, and not
		 * preempted!
		 *
		 * So if it was still runnable (but just not actively
		 * running right now), it's preempted, and we should
		 * yield - it could be a while.
		 */
		إذا (unlikely(queued)) {
			ktime_t to = NSEC_PER_SEC / HZ;

			set_current_state(TASK_UNINTERRUPTIBLE);
			schedule_hrtimeout(&to, HRTIMER_MODE_REL_HARD);
			استمر;
		}

		/*
		 * Ahh, all good. It wasn't running, and it wasn't
		 * runnable, which means that it will never become
		 * running in the future either. We're all done!
		 */
		اكسر;
	}

	إرجاع ncsw;
}

ساكن فراغ
do_set_cpus_allowed(بنية task_struct *p, بنية affinity_context *ctx);

ساكن فراغ migrate_disable_switch(بنية rq *rq, بنية task_struct *p)
{
	بنية affinity_context ac = {
		.new_mask  = cpumask_of(rq->cpu),
		.flags     = SCA_MIGRATE_DISABLE,
	};

	إذا (likely(!p->migration_disabled))
		إرجاع;

	إذا (p->cpus_ptr != &p->cpus_mask)
		إرجاع;

	scoped_guard (task_rq_lock, p)
		do_set_cpus_allowed(p, &ac);
}

فراغ ___migrate_enable(فراغ)
{
	بنية task_struct *p = current;
	بنية affinity_context ac = {
		.new_mask  = &p->cpus_mask,
		.flags     = SCA_MIGRATE_ENABLE,
	};

	__set_cpus_allowed_ptr(p, &ac);
}
EXPORT_رمز_GPL(___migrate_enable);

فراغ migrate_disable(فراغ)
{
	__migrate_disable();
}
EXPORT_رمز_GPL(migrate_disable);

فراغ migrate_enable(فراغ)
{
	__migrate_enable();
}
EXPORT_رمز_GPL(migrate_enable);

ساكن مضمن bool rq_has_pinned_tasks(بنية rq *rq)
{
	إرجاع rq->nr_pinned;
}

ساكن مضمن bool task_can_migrate_to_preferred(بنية task_struct *p, صحيح cpu)
{
	/* No need to migrate from a preferred CPU */
	إذا (cpu_preferred(cpu))
		إرجاع false;

	/* Only FAIR tasks honor preferred CPU state */
	إذا (unlikely(p->sched_class != &fair_sched_class))
		إرجاع false;

	/* Ignore preferred state if task affinity is changing */
	إذا (unlikely(!cpumask_test_cpu(task_cpu(p), p->cpus_ptr)))
		إرجاع false;

	إرجاع cpumask_intersects_and(p->cpus_ptr, cpu_preferred_mask,
				      task_cpu_possible_mask(p));
}

/*
 * Per-CPU kthreads are allowed to run on !active && online CPUs, see
 * __set_cpus_allowed_ptr() and select_fallback_rq().
 */
ساكن مضمن bool is_cpu_allowed(بنية task_struct *p, صحيح cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	إذا (!task_allowed_on_cpu(p, cpu))
		إرجاع false;

	/* migrate_disabled() must be allowed to finish. */
	إذا (is_migration_disabled(p))
		إرجاع cpu_online(cpu);

	/* Non kernel threads are not allowed during either online or offline. */
	إذا (!(p->flags & PF_KTHREAD)) {
		/* Try to use preferred CPU if task's affinity allows */
		إذا (task_can_migrate_to_preferred(p, cpu))
			إرجاع false;
		إرجاع cpu_active(cpu);
	}

	/* KTHREAD_IS_PER_CPU is always allowed. */
	إذا (kthread_is_per_cpu(p))
		إرجاع cpu_online(cpu);

	/* Regular kernel threads don't get to stay during offline. */
	إذا (cpu_dying(cpu))
		إرجاع false;

	/* Try to keep unbound kthreads on a preferred CPU if possible. */
	إذا (task_can_migrate_to_preferred(p, cpu))
		إرجاع false;

	/* Otherwise, they are allowed to run on online CPU. */
	إرجاع cpu_online(cpu);
}

/*
 * This is how migration works:
 *
 * 1) we invoke migration_cpu_stop() on the target CPU using
 *    stop_one_cpu().
 * 2) stopper starts to run (implicitly forcing the migrated thread
 *    off the CPU)
 * 3) it checks whether the migrated task is still in the wrong runqueue.
 * 4) if it's in the wrong runqueue then the migration thread removes
 *    it and puts it into the right queue.
 * 5) stopper completes and stop_one_cpu() returns and the migration
 *    is done.
 */

/*
 * move_queued_task - move a queued task to new rq.
 *
 * Returns (locked) new rq. Old rq's lock is released.
 */
ساكن بنية rq *move_queued_task(بنية rq *rq, بنية rq_flags *rf,
				   بنية task_struct *p, صحيح new_cpu)
	__must_hold(__rq_lockp(rq))
{
	lockdep_assert_rq_held(rq);

	deactivate_task(rq, p, DEQUEUE_NOCLOCK);
	set_task_cpu(p, new_cpu);
	rq_unlock(rq, rf);

	rq = cpu_rq(new_cpu);

	rq_lock(rq, rf);
	WARN_ON_ONCE(task_cpu(p) != new_cpu);
	activate_task(rq, p, 0);
	wakeup_preempt(rq, p, 0);

	إرجاع rq;
}

بنية migration_arg {
	بنية task_struct		*task;
	صحيح				dest_cpu;
	بنية set_affinity_pending	*pending;
};

/*
 * @refs: number of wait_for_completion()
 * @stop_pending: is @stop_work in use
 */
بنية set_affinity_pending {
	refcount_t		refs;
	unsigned صحيح		stop_pending;
	بنية completion	done;
	بنية cpu_stop_work	stop_work;
	بنية migration_arg	arg;
};

/*
 * Move (not current) task off this CPU, onto the destination CPU. We're doing
 * this because either it can't run here any more (set_cpus_allowed()
 * away from this CPU, or CPU going down), or because we're
 * attempting to rebalance this task on exec (sched_exec).
 *
 * So we race with normal scheduler movements, but that's OK, as long
 * as the task is no longer on this CPU.
 */
ساكن بنية rq *__migrate_task(بنية rq *rq, بنية rq_flags *rf,
				 بنية task_struct *p, صحيح dest_cpu)
	__must_hold(__rq_lockp(rq))
{
	/* Affinity changed (again). */
	إذا (!is_cpu_allowed(p, dest_cpu))
		إرجاع rq;

	rq = move_queued_task(rq, rf, p, dest_cpu);

	إرجاع rq;
}

/*
 * migration_cpu_stop - this will be executed by a high-prio stopper thread
 * and performs thread migration by bumping thread off CPU then
 * 'pushing' onto another runqueue.
 */
ساكن صحيح migration_cpu_stop(فراغ *data)
{
	بنية migration_arg *arg = data;
	بنية set_affinity_pending *pending = arg->pending;
	بنية task_struct *p = arg->task;
	بنية rq *rq = this_rq();
	bool complete = false;
	بنية rq_flags rf;

	/*
	 * The original target CPU might have gone down and we might
	 * be on another CPU but it doesn't matter.
	 */
	local_irq_save(rf.flags);
	/*
	 * We need to explicitly wake pending tasks before running
	 * __migrate_task() such that we will not miss enforcing cpus_ptr
	 * during wakeups, see set_cpus_allowed_ptr()'s TASK_WAKING test.
	 */
	flush_smp_call_function_queue();

	/*
	 * We may change the underlying rq, but the locks held will
	 * appropriately be "transferred" when switching.
	 */
	context_unsafe_alias(rq);

	raw_spin_lock(&p->pi_lock);
	rq_lock(rq, &rf);

	/*
	 * If we were passed a pending, then ->stop_pending was set, thus
	 * p->migration_pending must have remained stable.
	 */
	WARN_ON_ONCE(pending && pending != p->migration_pending);

	/*
	 * If task_rq(p) != rq, it cannot be migrated here, because we're
	 * holding rq->lock, if p->on_rq == 0 it cannot get enqueued because
	 * we're holding p->pi_lock.
	 */
	إذا (task_rq(p) == rq) {
		إذا (is_migration_disabled(p))
			اذهب out;

		إذا (pending) {
			p->migration_pending = عدم;
			complete = true;

			إذا (cpumask_test_cpu(task_cpu(p), &p->cpus_mask))
				اذهب out;
		}

		إذا (task_on_rq_queued(p)) {
			update_rq_clock(rq);
			rq = __migrate_task(rq, &rf, p, arg->dest_cpu);
		} وإلا {
			p->wake_cpu = arg->dest_cpu;
		}

		/*
		 * XXX __migrate_task() can fail, at which point we might end
		 * up running on a dodgy CPU, AFAICT this can only happen
		 * during CPU hotplug, at which point we'll get pushed out
		 * anyway, so it's probably not a big deal.
		 */

	} وإلا إذا (pending) {
		/*
		 * This happens when we get migrated between migrate_enable()'s
		 * preempt_enable() and scheduling the stopper task. At that
		 * point we're a regular task again and not current anymore.
		 *
		 * A !PREEMPT kernel has a giant hole here, which makes it far
		 * more likely.
		 */

		/*
		 * The task moved before the stopper got to run. We're holding
		 * ->pi_lock, so the allowed mask is stable - if it got
		 * somewhere allowed, we're done.
		 */
		إذا (cpumask_test_cpu(task_cpu(p), p->cpus_ptr)) {
			p->migration_pending = عدم;
			complete = true;
			اذهب out;
		}

		/*
		 * When migrate_enable() hits a rq mis-match we can't reliably
		 * determine is_migration_disabled() and so have to chase after
		 * it.
		 */
		WARN_ON_ONCE(!pending->stop_pending);
		preempt_disable();
		rq_unlock(rq, &rf);
		raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);
		stop_one_cpu_nowait(task_cpu(p), migration_cpu_stop,
				    &pending->arg, &pending->stop_work);
		preempt_enable();
		إرجاع 0;
	}
out:
	إذا (pending)
		pending->stop_pending = false;
	rq_unlock(rq, &rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);

	إذا (complete)
		complete_all(&pending->done);

	إرجاع 0;
}

صحيح ادفع_cpu_stop(فراغ *arg)
{
	بنية rq *lowest_rq = عدم, *rq = this_rq();
	بنية task_struct *p = arg;

	raw_spin_lock_irq(&p->pi_lock);
	raw_spin_rq_lock(rq);

	إذا (task_rq(p) != rq)
		اذهب out_unlock;

	إذا (is_migration_disabled(p)) {
		p->migration_flags |= MDF_ادفع;
		اذهب out_unlock;
	}

	p->migration_flags &= ~MDF_ادفع;

	إذا (p->sched_class->find_lock_rq)
		lowest_rq = p->sched_class->find_lock_rq(p, rq);

	إذا (!lowest_rq)
		اذهب out_unlock;

	lockdep_assert_rq_held(lowest_rq);

	// XXX validate p is still the highest prio task
	إذا (task_rq(p) == rq) {
		move_queued_task_locked(rq, lowest_rq, p);
		resched_curr(lowest_rq);
	}

	double_unlock_balance(rq, lowest_rq);

out_unlock:
	rq->ادفع_busy = false;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irq(&p->pi_lock);

	put_task_struct(p);
	إرجاع 0;
}

ساكن مضمن فراغ mm_update_cpus_allowed(بنية mm_struct *mm, ثابت cpumask_t *affmask);

/*
 * sched_class::set_cpus_allowed must do the below, but is not required to
 * actually call this function.
 */
فراغ set_cpus_allowed_common(بنية task_struct *p, بنية affinity_context *ctx)
{
	إذا (ctx->flags & (SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) {
		p->cpus_ptr = ctx->new_mask;
		إرجاع;
	}

	cpumask_copy(&p->cpus_mask, ctx->new_mask);
	p->nr_cpus_allowed = cpumask_weight(ctx->new_mask);
	mm_update_cpus_allowed(p->mm, ctx->new_mask);

	/*
	 * Swap in a new user_cpus_ptr if SCA_USER flag set
	 */
	إذا (ctx->flags & SCA_USER)
		swap(p->user_cpus_ptr, ctx->user_mask);
}

ساكن فراغ
do_set_cpus_allowed(بنية task_struct *p, بنية affinity_context *ctx)
{
	scoped_guard (sched_change, p, DEQUEUE_SAVE)
		p->sched_class->set_cpus_allowed(p, ctx);
}

/*
 * Used for kthread_bind() and select_fallback_rq(), in both cases the user
 * affinity (if any) should be destroyed too.
 */
فراغ set_cpus_allowed_force(بنية task_struct *p, ثابت بنية cpumask *new_mask)
{
	بنية affinity_context ac = {
		.new_mask  = new_mask,
		.user_mask = عدم,
		.flags     = SCA_USER,	/* clear the user requested mask */
	};
	اتحاد cpumask_rcuhead {
		cpumask_t cpumask;
		بنية rcu_head rcu;
	};

	scoped_guard (__task_rq_lock, p)
		do_set_cpus_allowed(p, &ac);

	/*
	 * Because this is called with p->pi_lock held, it is not possible
	 * to use kfree() here (when PREEMPT_RT=y), therefore punt to using
	 * kfree_rcu().
	 */
	kfree_rcu((اتحاد cpumask_rcuhead *)ac.user_mask, rcu);
}

صحيح dup_user_cpus_ptr(بنية task_struct *dst, بنية task_struct *src,
		      صحيح عقدة)
{
	cpumask_t *user_mask;
	unsigned long flags;

	/*
	 * Always clear dst->user_cpus_ptr first as their user_cpus_ptr's
	 * may differ by now due to racing.
	 */
	dst->user_cpus_ptr = عدم;

	/*
	 * This check is racy and losing the race is a valid situation.
	 * It is not worth the extra overhead of taking the pi_lock on
	 * every fork/clone.
	 */
	إذا (data_race(!src->user_cpus_ptr))
		إرجاع 0;

	user_mask = alloc_user_cpus_ptr(عقدة);
	إذا (!user_mask)
		إرجاع -ENOMEM;

	/*
	 * Use pi_lock to protect content of user_cpus_ptr
	 *
	 * Though unlikely, user_cpus_ptr can be reset to NULL by a concurrent
	 * set_cpus_allowed_force().
	 */
	raw_spin_lock_irqsave(&src->pi_lock, flags);
	إذا (src->user_cpus_ptr) {
		swap(dst->user_cpus_ptr, user_mask);
		cpumask_copy(dst->user_cpus_ptr, src->user_cpus_ptr);
	}
	raw_spin_unlock_irqrestore(&src->pi_lock, flags);

	إذا (unlikely(user_mask))
		kfree(user_mask);

	إرجاع 0;
}

ساكن مضمن بنية cpumask *clear_user_cpus_ptr(بنية task_struct *p)
{
	بنية cpumask *user_mask = عدم;

	swap(p->user_cpus_ptr, user_mask);

	إرجاع user_mask;
}

فراغ release_user_cpus_ptr(بنية task_struct *p)
{
	kfree(clear_user_cpus_ptr(p));
}

/*
 * This function is wildly self concurrent; here be dragons.
 *
 *
 * When given a valid mask, __set_cpus_allowed_ptr() must block until the
 * designated task is enqueued on an allowed CPU. If that task is currently
 * running, we have to kick it out using the CPU stopper.
 *
 * Migrate-Disable comes along and tramples all over our nice sandcastle.
 * Consider:
 *
 *     Initial conditions: P0->cpus_mask = [0, 1]
 *
 *     P0@CPU0                  P1
 *
 *     migrate_disable();
 *     <preempted>
 *                              set_cpus_allowed_ptr(P0, [1]);
 *
 * P1 *cannot* return from this set_cpus_allowed_ptr() call until P0 executes
 * its outermost migrate_enable() (i.e. it exits its Migrate-Disable region).
 * This means we need the following scheme:
 *
 *     P0@CPU0                  P1
 *
 *     migrate_disable();
 *     <preempted>
 *                              set_cpus_allowed_ptr(P0, [1]);
 *                                <blocks>
 *     <resumes>
 *     migrate_enable();
 *       __set_cpus_allowed_ptr();
 *       <wakes local stopper>
 *                         `--> <woken on migration completion>
 *
 * Now the fun stuff: there may be several P1-like tasks, i.e. multiple
 * concurrent set_cpus_allowed_ptr(P0, [*]) calls. CPU affinity changes of any
 * task p are serialized by p->pi_lock, which we can leverage: the one that
 * should come into effect at the end of the Migrate-Disable region is the last
 * one. This means we only need to track a single cpumask (i.e. p->cpus_mask),
 * but we still need to properly signal those waiting tasks at the appropriate
 * moment.
 *
 * This is implemented using struct set_affinity_pending. The first
 * __set_cpus_allowed_ptr() caller within a given Migrate-Disable region will
 * setup an instance of that struct and install it on the targeted task_struct.
 * Any and all further callers will reuse that instance. Those then wait for
 * a completion signaled at the tail of the CPU stopper callback (1), triggered
 * on the end of the Migrate-Disable region (i.e. outermost migrate_enable()).
 *
 *
 * (1) In the cases covered above. There is one more where the completion is
 * signaled within affine_move_task() itself: when a subsequent affinity request
 * occurs after the stopper bailed out due to the targeted task still being
 * Migrate-Disable. Consider:
 *
 *     Initial conditions: P0->cpus_mask = [0, 1]
 *
 *     CPU0		  P1				P2
 *     <P0>
 *       migrate_disable();
 *       <preempted>
 *                        set_cpus_allowed_ptr(P0, [1]);
 *                          <blocks>
 *     <migration/0>
 *       migration_cpu_stop()
 *         is_migration_disabled()
 *           <bails>
 *                                                       set_cpus_allowed_ptr(P0, [0, 1]);
 *                                                         <signal completion>
 *                          <awakes>
 *
 * Note that the above is safe vs a concurrent migrate_enable(), as any
 * pending affinity completion is preceded by an uninstallation of
 * p->migration_pending done with p->pi_lock held.
 */
ساكن صحيح affine_move_task(بنية rq *rq, بنية task_struct *p, بنية rq_flags *rf,
			    صحيح dest_cpu, unsigned صحيح flags)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	بنية set_affinity_pending my_pending = { }, *pending = عدم;
	bool stop_pending, complete = false;

	/*
	 * Can the task run on the task's current CPU? If so, we're done
	 *
	 * We are also done if the task is the current donor, boosting a lock-
	 * holding proxy, (and potentially has been migrated outside its
	 * current or previous affinity mask)
	 */
	إذا (cpumask_test_cpu(task_cpu(p), &p->cpus_mask) ||
	    (task_current_donor(rq, p) && !task_current(rq, p))) {
		بنية task_struct *ادفع_task = عدم;

		إذا ((flags & SCA_MIGRATE_ENABLE) &&
		    (p->migration_flags & MDF_ادفع) && !rq->ادفع_busy) {
			rq->ادفع_busy = true;
			ادفع_task = get_task_struct(p);
		}

		/*
		 * If there are pending waiters, but no pending stop_work,
		 * then complete now.
		 */
		pending = p->migration_pending;
		إذا (pending && !pending->stop_pending) {
			p->migration_pending = عدم;
			complete = true;
		}

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		إذا (ادفع_task) {
			stop_one_cpu_nowait(rq->cpu, ادفع_cpu_stop,
					    p, &rq->ادفع_work);
		}
		preempt_enable();

		إذا (complete)
			complete_all(&pending->done);

		إرجاع 0;
	}

	إذا (!(flags & SCA_MIGRATE_ENABLE)) {
		/* serialized by p->pi_lock */
		إذا (!p->migration_pending) {
			/* Install the request */
			refcount_set(&my_pending.refs, 1);
			init_completion(&my_pending.done);
			my_pending.arg = (بنية migration_arg) {
				.task = p,
				.dest_cpu = dest_cpu,
				.pending = &my_pending,
			};

			p->migration_pending = &my_pending;
		} وإلا {
			pending = p->migration_pending;
			refcount_inc(&pending->refs);
			/*
			 * Affinity has changed, but we've already installed a
			 * pending. migration_cpu_stop() *must* see this, else
			 * we risk a completion of the pending despite having a
			 * task on a disallowed CPU.
			 *
			 * Serialized by p->pi_lock, so this is safe.
			 */
			pending->arg.dest_cpu = dest_cpu;
		}
	}
	pending = p->migration_pending;
	/*
	 * - !MIGRATE_ENABLE:
	 *   we'll have installed a pending if there wasn't one already.
	 *
	 * - MIGRATE_ENABLE:
	 *   we're here because the current CPU isn't matching anymore,
	 *   the only way that can happen is because of a concurrent
	 *   set_cpus_allowed_ptr() call, which should then still be
	 *   pending completion.
	 *
	 * Either way, we really should have a @pending here.
	 */
	إذا (WARN_ON_ONCE(!pending)) {
		task_rq_unlock(rq, p, rf);
		إرجاع -EINVAL;
	}

	إذا (task_on_cpu(rq, p) || READ_ONCE(p->__state) == TASK_WAKING) {
		/*
		 * MIGRATE_ENABLE gets here because 'p == current', but for
		 * anything else we cannot do is_migration_disabled(), punt
		 * and have the stopper function handle it all race-free.
		 */
		stop_pending = pending->stop_pending;
		إذا (!stop_pending)
			pending->stop_pending = true;

		إذا (flags & SCA_MIGRATE_ENABLE)
			p->migration_flags &= ~MDF_ادفع;

		preempt_disable();
		task_rq_unlock(rq, p, rf);
		إذا (!stop_pending) {
			stop_one_cpu_nowait(cpu_of(rq), migration_cpu_stop,
					    &pending->arg, &pending->stop_work);
		}
		preempt_enable();

		إذا (flags & SCA_MIGRATE_ENABLE)
			إرجاع 0;
	} وإلا {

		إذا (!is_migration_disabled(p)) {
			إذا (task_on_rq_queued(p))
				rq = move_queued_task(rq, rf, p, dest_cpu);

			إذا (!pending->stop_pending) {
				p->migration_pending = عدم;
				complete = true;
			}
		}
		task_rq_unlock(rq, p, rf);

		إذا (complete)
			complete_all(&pending->done);
	}

	wait_for_completion(&pending->done);

	إذا (refcount_dec_and_test(&pending->refs))
		wake_up_var(&pending->refs); /* No UaF, just an address */

	/*
	 * Block the original owner of &pending until all subsequent callers
	 * have seen the completion and decremented the refcount
	 */
	wait_var_event(&my_pending.refs, !refcount_read(&my_pending.refs));

	/* ARGH */
	WARN_ON_ONCE(my_pending.stop_pending);

	إرجاع 0;
}

/*
 * Called with both p->pi_lock and rq->lock held; drops both before returning.
 */
ساكن صحيح __set_cpus_allowed_ptr_locked(بنية task_struct *p,
					 بنية affinity_context *ctx,
					 بنية rq *rq,
					 بنية rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	ثابت بنية cpumask *cpu_allowed_mask = task_cpu_possible_mask(p);
	ثابت بنية cpumask *cpu_valid_mask = cpu_active_mask;
	bool kthread = p->flags & PF_KTHREAD;
	unsigned صحيح dest_cpu;
	صحيح ret = 0;

	إذا (kthread || is_migration_disabled(p)) {
		/*
		 * Kernel threads are allowed on online && !active CPUs,
		 * however, during cpu-hot-unplug, even these might get pushed
		 * away if not KTHREAD_IS_PER_CPU.
		 *
		 * Specifically, migration_disabled() tasks must not fail the
		 * cpumask_any_and_distribute() pick below, esp. so on
		 * SCA_MIGRATE_ENABLE, otherwise we'll not call
		 * set_cpus_allowed_common() and actually reset p->cpus_ptr.
		 */
		cpu_valid_mask = cpu_online_mask;
	}

	إذا (!kthread && !cpumask_subset(ctx->new_mask, cpu_allowed_mask)) {
		ret = -EINVAL;
		اذهب out;
	}

	/*
	 * Must re-check here, to close a race against __kthread_bind(),
	 * sched_setaffinity() is not guaranteed to observe the flag.
	 */
	إذا ((ctx->flags & SCA_CHECK) && (p->flags & PF_NO_SETAFFINITY)) {
		ret = -EINVAL;
		اذهب out;
	}

	إذا (!(ctx->flags & SCA_MIGRATE_ENABLE)) {
		إذا (cpumask_equal(&p->cpus_mask, ctx->new_mask)) {
			إذا (ctx->flags & SCA_USER)
				swap(p->user_cpus_ptr, ctx->user_mask);
			اذهب out;
		}

		إذا (WARN_ON_ONCE(p == current &&
				 is_migration_disabled(p) &&
				 !cpumask_test_cpu(task_cpu(p), ctx->new_mask))) {
			ret = -EBUSY;
			اذهب out;
		}
	}

	/*
	 * Picking a ~random cpu helps in cases where we are changing affinity
	 * for groups of tasks (ie. cpuset), so that load balancing is not
	 * immediately required to distribute the tasks within their new mask.
	 */
	dest_cpu = cpumask_any_and_distribute(cpu_valid_mask, ctx->new_mask);
	إذا (dest_cpu >= nr_cpu_ids) {
		ret = -EINVAL;
		اذهب out;
	}

	do_set_cpus_allowed(p, ctx);

	إرجاع affine_move_task(rq, p, rf, dest_cpu, ctx->flags);

out:
	task_rq_unlock(rq, p, rf);

	إرجاع ret;
}

/*
 * Change a given task's CPU affinity. Migrate the thread to a
 * proper CPU and schedule it away if the CPU it's executing on
 * is removed from the allowed bitmask.
 *
 * NOTE: the caller must have a valid reference to the task, the
 * task must not exit() & deallocate itself prematurely. The
 * call is not atomic; no spinlocks may be held.
 */
صحيح __set_cpus_allowed_ptr(بنية task_struct *p, بنية affinity_context *ctx)
{
	بنية rq_flags rf;
	بنية rq *rq;

	rq = task_rq_lock(p, &rf);
	/*
	 * Masking should be skipped if SCA_USER or any of the SCA_MIGRATE_*
	 * flags are set.
	 */
	إذا (p->user_cpus_ptr &&
	    !(ctx->flags & (SCA_USER | SCA_MIGRATE_ENABLE | SCA_MIGRATE_DISABLE)) &&
	    cpumask_and(rq->scratch_mask, ctx->new_mask, p->user_cpus_ptr))
		ctx->new_mask = rq->scratch_mask;

	إرجاع __set_cpus_allowed_ptr_locked(p, ctx, rq, &rf);
}

صحيح set_cpus_allowed_ptr(بنية task_struct *p, ثابت بنية cpumask *new_mask)
{
	بنية affinity_context ac = {
		.new_mask  = new_mask,
		.flags     = 0,
	};

	إرجاع __set_cpus_allowed_ptr(p, &ac);
}
EXPORT_رمز_GPL(set_cpus_allowed_ptr);

/*
 * Change a given task's CPU affinity to the intersection of its current
 * affinity mask and @subset_mask, writing the resulting mask to @new_mask.
 * If user_cpus_ptr is defined, use it as the basis for restricting CPU
 * affinity or use cpu_online_mask instead.
 *
 * If the resulting mask is empty, leave the affinity unchanged and return
 * -EINVAL.
 */
ساكن صحيح restrict_cpus_allowed_ptr(بنية task_struct *p,
				     بنية cpumask *new_mask,
				     ثابت بنية cpumask *subset_mask)
{
	بنية affinity_context ac = {
		.new_mask  = new_mask,
		.flags     = 0,
	};
	بنية rq_flags rf;
	بنية rq *rq;
	صحيح err;

	rq = task_rq_lock(p, &rf);

	/*
	 * Forcefully restricting the affinity of a deadline task is
	 * likely to cause problems, so fail and noisily override the
	 * mask entirely.
	 */
	إذا (task_has_dl_policy(p) && dl_bandwidth_enabled()) {
		err = -EPERM;
		اذهب err_unlock;
	}

	إذا (!cpumask_and(new_mask, task_user_cpus(p), subset_mask)) {
		err = -EINVAL;
		اذهب err_unlock;
	}

	إرجاع __set_cpus_allowed_ptr_locked(p, &ac, rq, &rf);

err_unlock:
	task_rq_unlock(rq, p, &rf);
	إرجاع err;
}

/*
 * Restrict the CPU affinity of task @p so that it is a subset of
 * task_cpu_possible_mask() and point @p->user_cpus_ptr to a copy of the
 * old affinity mask. If the resulting mask is empty, we warn and walk
 * up the cpuset hierarchy until we find a suitable mask.
 */
فراغ force_compatible_cpus_allowed_ptr(بنية task_struct *p)
{
	cpumask_var_t new_mask;
	ثابت بنية cpumask *override_mask = task_cpu_possible_mask(p);

	alloc_cpumask_var(&new_mask, GFP_KERNEL);

	/*
	 * __migrate_task() can fail silently in the face of concurrent
	 * offlining of the chosen destination CPU, so take the hotplug
	 * lock to ensure that the migration succeeds.
	 */
	cpus_read_lock();
	إذا (!cpumask_available(new_mask))
		اذهب out_set_mask;

	إذا (!restrict_cpus_allowed_ptr(p, new_mask, override_mask))
		اذهب out_free_mask;

	/*
	 * We failed to find a valid subset of the affinity mask for the
	 * task, so override it based on its cpuset hierarchy.
	 */
	cpuset_cpus_allowed(p, new_mask);
	override_mask = new_mask;

out_set_mask:
	إذا (printk_ratelimit()) {
		printk_deferred("Overriding affinity for process %d (%s) to CPUs %*pbl\n",
				task_pid_nr(p), p->comm,
				cpumask_pr_args(override_mask));
	}

	WARN_ON(set_cpus_allowed_ptr(p, override_mask));
out_free_mask:
	cpus_read_unlock();
	free_cpumask_var(new_mask);
}

/*
 * Restore the affinity of a task @p which was previously restricted by a
 * call to force_compatible_cpus_allowed_ptr().
 *
 * It is the caller's responsibility to serialise this with any calls to
 * force_compatible_cpus_allowed_ptr(@p).
 */
فراغ relax_compatible_cpus_allowed_ptr(بنية task_struct *p)
{
	بنية affinity_context ac = {
		.new_mask  = task_user_cpus(p),
		.flags     = 0,
	};
	صحيح ret;

	/*
	 * Try to restore the old affinity mask with __sched_setaffinity().
	 * Cpuset masking will be done there too.
	 */
	ret = __sched_setaffinity(p, &ac);
	WARN_ON_ONCE(ret);
}

#ifdef CONFIG_SMP

فراغ set_task_cpu(بنية task_struct *p, unsigned صحيح new_cpu)
{
	unsigned صحيح state = READ_ONCE(p->__state);
	bool proxy_migrated = sched_proxy_exec() && p->is_blocked &&
			      task_cpu(p) != p->wake_cpu;

	/*
	 * We should never call set_task_cpu() on a blocked task,
	 * ttwu() will sort out the placement.
	 */
	WARN_ON_ONCE(state != TASK_RUNNING && state != TASK_WAKING && !p->on_rq);

	/*
	 * Migrating fair class task must have p->on_rq = TASK_ON_RQ_MIGRATING,
	 * because schedstat_wait_{start,end} rebase migrating task's wait_start
	 * time relying on p->on_rq.
	 */
	WARN_ON_ONCE(state == TASK_RUNNING &&
		     p->sched_class == &fair_sched_class &&
		     (p->on_rq && !task_on_rq_migrating(p)));

#ifdef CONFIG_LOCKDEP
	/*
	 * The caller should hold either p->pi_lock or rq->lock, when changing
	 * a task's CPU. ->pi_lock for waking tasks, rq->lock for runnable tasks.
	 *
	 * sched_move_task() holds both and thus holding either pins the cgroup,
	 * see task_group().
	 *
	 * Furthermore, all task_rq users should acquire both locks, see
	 * task_rq_lock().
	 */
	WARN_ON_ONCE(debug_locks && !(lockdep_is_held(&p->pi_lock) ||
				      lockdep_is_held(__rq_lockp(task_rq(p)))));
#endif
	/*
	 * Clearly, migrating tasks to offline CPUs is a fairly daft thing.
	 */
	WARN_ON_ONCE(!cpu_online(new_cpu));

	/*
	 * Proxy execution can move a blocked task's scheduling context to any
	 * CPU without moving its migration-disabled execution context. The
	 * wakeup path will return the task to a CPU where it can execute.
	 */
	WARN_ON_ONCE(is_migration_disabled(p) && !proxy_migrated);

	trace_sched_migrate_task(p, new_cpu);

	إذا (task_cpu(p) != new_cpu) {
		إذا (p->sched_class->migrate_task_rq)
			p->sched_class->migrate_task_rq(p, new_cpu);
		p->se.nr_migrations++;
		perf_event_task_migrate(p);
	}

	__set_task_cpu(p, new_cpu);
}
#endif /* CONFIG_SMP */

#ifdef CONFIG_NUMA_BALANCING
ساكن فراغ __migrate_swap_task(بنية task_struct *p, صحيح cpu)
{
	إذا (task_on_rq_queued(p)) {
		بنية rq *src_rq, *dst_rq;
		بنية rq_flags srf, drf;

		src_rq = task_rq(p);
		dst_rq = cpu_rq(cpu);

		rq_pin_lock(src_rq, &srf);
		rq_pin_lock(dst_rq, &drf);

		move_queued_task_locked(src_rq, dst_rq, p);
		wakeup_preempt(dst_rq, p, 0);

		rq_unpin_lock(dst_rq, &drf);
		rq_unpin_lock(src_rq, &srf);

	} وإلا {
		/*
		 * Task isn't running anymore; make it appear like we migrated
		 * it before it went to sleep. This means on wakeup we make the
		 * previous CPU our target instead of where it really is.
		 */
		p->wake_cpu = cpu;
	}
}

بنية migration_swap_arg {
	بنية task_struct *src_task, *dst_task;
	صحيح src_cpu, dst_cpu;
};

ساكن صحيح migrate_swap_stop(فراغ *data)
{
	بنية migration_swap_arg *arg = data;
	بنية rq *src_rq, *dst_rq;

	إذا (!cpu_active(arg->src_cpu) || !cpu_active(arg->dst_cpu))
		إرجاع -EAGAIN;

	src_rq = cpu_rq(arg->src_cpu);
	dst_rq = cpu_rq(arg->dst_cpu);

	guard(double_raw_spinlock)(&arg->src_task->pi_lock, &arg->dst_task->pi_lock);
	guard(double_rq_lock)(src_rq, dst_rq);

	إذا (task_cpu(arg->dst_task) != arg->dst_cpu)
		إرجاع -EAGAIN;

	إذا (task_cpu(arg->src_task) != arg->src_cpu)
		إرجاع -EAGAIN;

	إذا (!cpumask_test_cpu(arg->dst_cpu, arg->src_task->cpus_ptr))
		إرجاع -EAGAIN;

	إذا (!cpumask_test_cpu(arg->src_cpu, arg->dst_task->cpus_ptr))
		إرجاع -EAGAIN;

	__migrate_swap_task(arg->src_task, arg->dst_cpu);
	__migrate_swap_task(arg->dst_task, arg->src_cpu);

	إرجاع 0;
}

/*
 * Cross migrate two tasks
 */
صحيح migrate_swap(بنية task_struct *cur, بنية task_struct *p,
		صحيح target_cpu, صحيح curr_cpu)
{
	بنية migration_swap_arg arg;
	صحيح ret = -EINVAL;

	arg = (بنية migration_swap_arg){
		.src_task = cur,
		.src_cpu = curr_cpu,
		.dst_task = p,
		.dst_cpu = target_cpu,
	};

	إذا (arg.src_cpu == arg.dst_cpu)
		اذهب out;

	/*
	 * These three tests are all lockless; this is OK since all of them
	 * will be re-checked with proper locks held further down the line.
	 */
	إذا (!cpu_active(arg.src_cpu) || !cpu_active(arg.dst_cpu))
		اذهب out;

	إذا (!cpumask_test_cpu(arg.dst_cpu, arg.src_task->cpus_ptr))
		اذهب out;

	إذا (!cpumask_test_cpu(arg.src_cpu, arg.dst_task->cpus_ptr))
		اذهب out;

	trace_sched_swap_numa(cur, arg.src_cpu, p, arg.dst_cpu);
	ret = stop_two_cpus(arg.dst_cpu, arg.src_cpu, migrate_swap_stop, &arg);

out:
	إرجاع ret;
}
#endif /* CONFIG_NUMA_BALANCING */

/***
 * kick_process - kick a running thread to enter/exit the kernel
 * @p: the to-be-kicked thread
 *
 * Cause a process which is running on another CPU to enter
 * kernel-mode, without any delay. (to get signals handled.)
 *
 * NOTE: this function doesn't have to take the runqueue lock,
 * because all it wants to ensure is that the remote task enters
 * the kernel. If the IPI races and the task has been migrated
 * to another CPU then no harm is done and the purpose has been
 * achieved as well.
 */
فراغ kick_process(بنية task_struct *p)
{
	guard(preempt)();
	صحيح cpu = task_cpu(p);

	إذا ((cpu != smp_processor_id()) && task_curr(p))
		smp_send_reschedule(cpu);
}
EXPORT_رمز_GPL(kick_process);

/*
 * ->cpus_ptr is protected by both rq->lock and p->pi_lock
 *
 * A few notes on cpu_active vs cpu_online:
 *
 *  - cpu_active must be a subset of cpu_online
 *
 *  - on CPU-up we allow per-CPU kthreads on the online && !active CPU,
 *    see __set_cpus_allowed_ptr(). At this point the newly online
 *    CPU isn't yet part of the sched domains, and balancing will not
 *    see it.
 *
 *  - on CPU-down we clear cpu_active() to mask the sched domains and
 *    avoid the load balancer to place new tasks on the to be removed
 *    CPU. Existing tasks will remain running there and will be taken
 *    off.
 *
 * This means that fallback selection must not select !active CPUs.
 * And can assume that any active CPU must be online. Conversely
 * select_task_rq() below may allow selection of !active CPUs in order
 * to satisfy the above rules.
 */
ساكن صحيح select_fallback_rq(صحيح cpu, بنية task_struct *p)
{
	صحيح nid = cpu_to_عقدة(cpu);
	ثابت بنية cpumask *nodemask = عدم;
	تعداد { cpuset, possible, fail } state = cpuset;
	صحيح dest_cpu;

	/*
	 * If the node that the CPU is on has been offlined, cpu_to_node()
	 * will return -1. There is no CPU on the node, and we should
	 * select the CPU on the other node.
	 */
	إذا (nid != -1) {
		nodemask = cpumask_of_عقدة(nid);

		/* Look for allowed, online CPU in same node. */
		for_each_cpu(dest_cpu, nodemask) {
			إذا (is_cpu_allowed(p, dest_cpu))
				إرجاع dest_cpu;
		}
	}

	لكل (;;) {
		/* Any allowed, online CPU? */
		for_each_cpu(dest_cpu, p->cpus_ptr) {
			إذا (!is_cpu_allowed(p, dest_cpu))
				استمر;

			اذهب out;
		}

		/* No more Mr. Nice Guy. */
		بدل (state) {
		حالة cpuset:
			إذا (cpuset_cpus_allowed_fallback(p)) {
				state = possible;
				اكسر;
			}
			fallthrough;
		حالة possible:
			set_cpus_allowed_force(p, task_cpu_fallback_mask(p));
			state = fail;
			اكسر;
		حالة fail:
			BUG();
			اكسر;
		}
	}

out:
	إذا (state != cpuset) {
		/*
		 * Don't tell them about moving exiting tasks or
		 * kernel threads (both mm NULL), since they never
		 * leave kernel.
		 */
		إذا (p->mm && printk_ratelimit()) {
			printk_deferred("process %d (%s) no longer affine to cpu%d\n",
					task_pid_nr(p), p->comm, cpu);
		}
	}

	إرجاع dest_cpu;
}

/*
 * The caller (fork, wakeup) owns p->pi_lock, ->cpus_ptr is stable.
 */
ساكن مضمن
صحيح select_task_rq(بنية task_struct *p, صحيح cpu, صحيح *wake_flags)
{
	lockdep_assert_held(&p->pi_lock);

	إذا (p->nr_cpus_allowed > 1 && !is_migration_disabled(p)) {
		cpu = p->sched_class->select_task_rq(p, cpu, *wake_flags);
		*wake_flags |= WF_RQ_SELECTED;
	} وإلا {
		cpu = cpumask_any(p->cpus_ptr);
	}

	/*
	 * In order not to call set_task_cpu() on a blocking task we need
	 * to rely on ttwu() to place the task on a valid ->cpus_ptr
	 * CPU.
	 *
	 * Since this is common to all placement strategies, this lives here.
	 *
	 * [ this allows ->select_task() to simply return task_cpu(p) and
	 *   not worry about this generic constraint ]
	 */
	إذا (unlikely(!is_cpu_allowed(p, cpu)))
		cpu = select_fallback_rq(task_cpu(p), p);

	إرجاع cpu;
}

فراغ sched_set_stop_task(صحيح cpu, بنية task_struct *stop)
{
	ساكن بنية lock_class_مفتاح stop_pi_lock;
	بنية sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };
	بنية task_struct *old_stop = cpu_rq(cpu)->stop;

	إذا (stop) {
		/*
		 * Make it appear like a SCHED_FIFO task, its something
		 * userspace knows about and won't get confused about.
		 *
		 * Also, it will make PI more or less work without too
		 * much confusion -- but then, stop work should not
		 * rely on PI working anyway.
		 */
		sched_setscheduler_nocheck(stop, SCHED_FIFO, &param);

		stop->sched_class = &stop_sched_class;

		/*
		 * The PI code calls rt_mutex_setprio() with ->pi_lock held to
		 * adjust the effective priority of a task. As a result,
		 * rt_mutex_setprio() can trigger (RT) balancing operations,
		 * which can then trigger wakeups of the stop thread to push
		 * around the current task.
		 *
		 * The stop task itself will never be part of the PI-chain, it
		 * never blocks, therefore that ->pi_lock recursion is safe.
		 * Tell lockdep about this by placing the stop->pi_lock in its
		 * own class.
		 */
		lockdep_set_class(&stop->pi_lock, &stop_pi_lock);
	}

	cpu_rq(cpu)->stop = stop;

	إذا (old_stop) {
		/*
		 * Reset it back to a normal scheduling class so that
		 * it can die in pieces.
		 */
		old_stop->sched_class = &rt_sched_class;
	}
}

ساكن فراغ
ttwu_stat(بنية task_struct *p, صحيح cpu, صحيح wake_flags)
{
	بنية rq *rq;

	إذا (!schedstat_enabled())
		إرجاع;

	rq = this_rq();

	إذا (cpu == rq->cpu) {
		__schedstat_inc(rq->ttwu_local);
		__schedstat_inc(p->stats.nr_wakeups_local);
	} وإلا {
		بنية sched_domain *sd;

		__schedstat_inc(p->stats.nr_wakeups_remote);

		guard(rcu)();
		for_each_domain(rq->cpu, sd) {
			إذا (cpumask_test_cpu(cpu, sched_domain_span(sd))) {
				__schedstat_inc(sd->ttwu_wake_remote);
				اكسر;
			}
		}
	}

	إذا (wake_flags & WF_MIGRATED)
		__schedstat_inc(p->stats.nr_wakeups_migrate);

	__schedstat_inc(rq->ttwu_تعداد);
	__schedstat_inc(p->stats.nr_wakeups);

	إذا (wake_flags & WF_SYNC)
		__schedstat_inc(p->stats.nr_wakeups_sync);
}

/*
 * Mark the task runnable.
 */
ساكن مضمن فراغ ttwu_do_wakeup(بنية task_struct *p)
{
	p->is_blocked = 0;
	WRITE_ONCE(p->__state, TASK_RUNNING);
	trace_sched_wakeup(p);
}

فراغ update_rq_avg_idle(بنية rq *rq)
{
	u64 idle_stamp = rq->idle_stamp;
	u64 delta, max;

	إذا (!idle_stamp)
		إرجاع;

	delta = rq_clock(rq) - idle_stamp;

	update_avg(&rq->avg_idle, delta);

	max = 2 * rq->max_idle_balance_cost;
	إذا (rq->avg_idle > max)
		rq->avg_idle = max;
	rq->idle_stamp = 0;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ساكن فراغ zap_balance_callbacks(بنية rq *rq);

ساكن مضمن فراغ proxy_reset_donor(بنية rq *rq)
{
	WARN_ON_ONCE(rq->donor == rq->curr);

	put_prev_set_next_task(rq, rq->donor, rq->curr);
	rq->next_class = rq->curr->sched_class;
	rq_set_donor(rq, rq->curr);
	zap_balance_callbacks(rq);
	resched_curr(rq);
}

/*
 * Checks to see if task p has been proxy-migrated to another rq
 * and needs to be returned. If so, we deactivate the task here
 * so that it can be properly woken up on the p->wake_cpu
 * (or whichever cpu select_task_rq() picks at the bottom of
 * try_to_wake_up()
 */
ساكن مضمن bool proxy_needs_return(بنية rq *rq, بنية task_struct *p)
{
	bool dequeued;

	/*
	 * Typically per __set_task_cpu(), task_cpu(p) == p->wake_cpu.
	 *
	 * However, proxy_set_task_cpu() is such that it preserves the
	 * original cpu in p->wake_cpu while migrating p for proxy reasons
	 * (possibly outside of the allowed p->cpus_ptr).
	 *
	 * Furthermore, migration_cpu_stop() / __migrate_swap_task(), will
	 * only set p->wake_cpu when !p->on_rq, and since here p->on_rq, this
	 * will not apply. But if it did, this check is the safe way around
	 * and would migrate.
	 */
	إذا (task_cpu(p) == p->wake_cpu)
		إرجاع false;

	scoped_guard(raw_spinlock, &p->blocked_lock) {
		/* Task is waking up; clear any blocked_on relationship */
		__clear_task_blocked_on(p, عدم);

		/* If already current, don't need to return migrate */
		إذا (task_current(rq, p))
			إرجاع false;
	}

	dequeued = dequeue_block_task(rq, p, TASK_WAKING);

	/*
	 * Dequeue @p from its scheduling class before resetting rq->donor.
	 * In particular, sched_ext needs to end the donor's running session
	 * and clear SCX_TASK_QUEUED before put_prev_task_scx() is called by
	 * proxy_reset_donor(); otherwise it would reenqueue the blocked donor.
	 *
	 * Keep on_rq set until all donor references have been replaced.
	 */
	إذا (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	إذا (dequeued)
		__block_task(rq, p);
	إرجاع true;
}
#وإلا /* !CONFIG_SCHED_PROXY_EXEC */
ساكن مضمن bool proxy_needs_return(بنية rq *rq, بنية task_struct *p)
{
	إرجاع false;
}
#endif /* CONFIG_SCHED_PROXY_EXEC */

ساكن فراغ
ttwu_do_activate(بنية rq *rq, بنية task_struct *p, صحيح wake_flags,
		 بنية rq_flags *rf)
{
	صحيح en_flags = ENQUEUE_WAKEUP | ENQUEUE_NOCLOCK;

	lockdep_assert_rq_held(rq);

	إذا (p->sched_contributes_to_load)
		rq->nr_uninterruptible--;

	إذا (wake_flags & WF_RQ_SELECTED)
		en_flags |= ENQUEUE_RQ_SELECTED;
	إذا (wake_flags & WF_MIGRATED)
		en_flags |= ENQUEUE_MIGRATED;
	وإلا إذا (p->in_iowait) {
		delayacct_blkio_end(p);
		atomic_dec(&task_rq(p)->nr_iowait);
	}

	activate_task(rq, p, en_flags);
	wakeup_preempt(rq, p, wake_flags);

	ttwu_do_wakeup(p);

	إذا (p->sched_class->task_woken) {
		/*
		 * Our task @p is fully woken up and running; so it's safe to
		 * drop the rq->lock, hereafter rq is only used for statistics.
		 */
		rq_unpin_lock(rq, rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_lock(rq, rf);
	}
}

/*
 * Consider @p being inside a wait loop:
 *
 *   for (;;) {
 *      set_current_state(TASK_UNINTERRUPTIBLE);
 *
 *      if (CONDITION)
 *         break;
 *
 *      schedule();
 *   }
 *   __set_current_state(TASK_RUNNING);
 *
 * between set_current_state() and schedule(). In this case @p is still
 * runnable, so all that needs doing is change p->state back to TASK_RUNNING in
 * an atomic manner.
 *
 * By taking task_rq(p)->lock we serialize against schedule(), if @p->on_rq
 * then schedule() must still happen and p->state can be changed to
 * TASK_RUNNING. Otherwise we lost the race, schedule() has happened, and we
 * need to do a full wakeup with enqueue.
 *
 * Returns: %true when the wakeup is done,
 *          %false otherwise.
 */
ساكن صحيح ttwu_runnable(بنية task_struct *p, صحيح wake_flags)
{
	ACQUIRE(__task_rq_lock, guard)(p);
	بنية rq *rq = guard.rq;

	إذا (!task_on_rq_queued(p))
		إرجاع 0;

	update_rq_clock(rq);
	إذا (p->is_blocked) {
		إذا (p->se.sched_delayed)
			enqueue_task(rq, p, ENQUEUE_NOCLOCK | ENQUEUE_DELAYED);
		إذا (proxy_needs_return(rq, p))
			إرجاع 0;
	}
	إذا (!task_on_cpu(rq, p)) {
		/*
		 * When on_rq && !on_cpu the task is preempted, see if
		 * it should preempt the task that is current now.
		 */
		wakeup_preempt(rq, p, wake_flags | WF_TTWU_RQ);
	}
	ttwu_do_wakeup(p);
	إرجاع 1;
}

فراغ sched_ttwu_pending(فراغ *arg)
{
	بنية llist_عقدة *llist = arg;
	بنية rq *rq = this_rq();
	بنية task_struct *p, *t;
	بنية rq_flags rf;

	إذا (!llist)
		إرجاع;

	rq_lock_irqsave(rq, &rf);
	update_rq_clock(rq);

	llist_for_each_entry_safe(p, t, llist, wake_entry.llist) {
		إذا (WARN_ON_ONCE(p->on_cpu))
			smp_cond_load_acquire(&p->on_cpu, !VAL);

		إذا (WARN_ON_ONCE(task_cpu(p) != cpu_of(rq)))
			set_task_cpu(p, cpu_of(rq));

		ttwu_do_activate(rq, p, p->sched_remote_wakeup ? WF_MIGRATED : 0, &rf);
	}

	/*
	 * Must be after enqueueing at least once task such that
	 * idle_cpu() does not observe a false-negative -- if it does,
	 * it is possible for select_idle_siblings() to stack a number
	 * of tasks on this CPU during that window.
	 *
	 * It is OK to clear ttwu_pending when another task pending.
	 * We will receive IPI after local IRQ enabled and then enqueue it.
	 * Since now nr_running > 0, idle_cpu() will always get correct result.
	 */
	WRITE_ONCE(rq->ttwu_pending, 0);
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Prepare the scene for sending an IPI for a remote smp_call
 *
 * Returns true if the caller can proceed with sending the IPI.
 * Returns false otherwise.
 */
bool call_function_single_prep_ipi(صحيح cpu)
{
	إذا (set_nr_if_polling(cpu_rq(cpu)->idle)) {
		trace_sched_wake_idle_without_ipi(cpu);
		إرجاع false;
	}

	إرجاع true;
}

/*
 * Queue a task on the target CPUs wake_list and wake the CPU via IPI if
 * necessary. The wakee CPU on receipt of the IPI will queue the task
 * via sched_ttwu_wakeup() for activation so the wakee incurs the cost
 * of the wakeup instead of the waker.
 */
ساكن فراغ __ttwu_queue_wakelist(بنية task_struct *p, صحيح cpu, صحيح wake_flags)
{
	بنية rq *rq = cpu_rq(cpu);

	p->sched_remote_wakeup = !!(wake_flags & WF_MIGRATED);

	WRITE_ONCE(rq->ttwu_pending, 1);
#ifdef CONFIG_SMP
	__smp_call_single_queue(cpu, &p->wake_entry.llist);
#endif
}

فراغ wake_up_if_idle(صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

	guard(rcu)();
	إذا (is_idle_task(rcu_dereference(rq->curr))) {
		guard(rq_lock_irqsave)(rq);
		إذا (is_idle_task(rq->curr))
			resched_curr(rq);
	}
}

bool cpus_equal_capacity(صحيح this_cpu, صحيح that_cpu)
{
	إذا (!sched_asym_cpucap_active())
		إرجاع true;

	إذا (this_cpu == that_cpu)
		إرجاع true;

	إرجاع arch_scale_cpu_capacity(this_cpu) == arch_scale_cpu_capacity(that_cpu);
}

bool cpus_share_cache(صحيح this_cpu, صحيح that_cpu)
{
	إذا (this_cpu == that_cpu)
		إرجاع true;

	إرجاع per_cpu(sd_llc_id, this_cpu) == per_cpu(sd_llc_id, that_cpu);
}

/*
 * Whether CPUs are share cache resources, which means LLC on non-cluster
 * machines and LLC tag or L2 on machines with clusters.
 */
bool cpus_share_resources(صحيح this_cpu, صحيح that_cpu)
{
	إذا (this_cpu == that_cpu)
		إرجاع true;

	إرجاع per_cpu(sd_share_id, this_cpu) == per_cpu(sd_share_id, that_cpu);
}

ساكن مضمن bool ttwu_queue_cond(بنية task_struct *p, صحيح cpu)
{
	صحيح this_cpu = smp_processor_id();

	/* See SCX_OPS_ALLOW_QUEUED_WAKEUP. */
	إذا (!scx_allow_ttwu_queue(p))
		إرجاع false;

#ifdef CONFIG_SMP
	إذا (p->sched_class == &stop_sched_class)
		إرجاع false;
#endif

	/*
	 * Do not complicate things with the async wake_list while the CPU is
	 * in hotplug state.
	 */
	إذا (!cpu_active(cpu))
		إرجاع false;

	/* Ensure the task will still be allowed to run on the CPU. */
	إذا (!cpumask_test_cpu(cpu, p->cpus_ptr))
		إرجاع false;

	/*
	 * If the CPU does not share cache, then queue the task on the
	 * remote rqs wakelist to avoid accessing remote data.
	 */
	إذا (!cpus_share_cache(this_cpu, cpu))
		إرجاع true;

	إذا (cpu == this_cpu)
		إرجاع false;

	/*
	 * If the wakee cpu is idle, or the task is descheduling and the
	 * only running task on the CPU, then use the wakelist to offload
	 * the task activation to the idle (or soon-to-be-idle) CPU as
	 * the current CPU is likely busy. nr_running is checked to
	 * avoid unnecessary task stacking.
	 *
	 * Note that we can only get here with (wakee) p->on_rq=0,
	 * p->on_cpu can be whatever, we've done the dequeue, so
	 * the wakee has been accounted out of ->nr_running.
	 */
	إذا (!cpu_rq(cpu)->nr_running)
		إرجاع true;

	إرجاع false;
}

ساكن bool ttwu_queue_wakelist(بنية task_struct *p, صحيح cpu, صحيح wake_flags)
{
	إذا (sched_feat(TTWU_QUEUE) && ttwu_queue_cond(p, cpu)) {
		sched_clock_cpu(cpu); /* Sync clocks across CPUs */
		__ttwu_queue_wakelist(p, cpu, wake_flags);
		إرجاع true;
	}

	إرجاع false;
}

ساكن فراغ ttwu_queue(بنية task_struct *p, صحيح cpu, صحيح wake_flags)
{
	بنية rq *rq = cpu_rq(cpu);
	بنية rq_flags rf;

	إذا (ttwu_queue_wakelist(p, cpu, wake_flags))
		إرجاع;

	rq_lock(rq, &rf);
	update_rq_clock(rq);
	ttwu_do_activate(rq, p, wake_flags, &rf);
	rq_unlock(rq, &rf);
}

/*
 * Invoked from try_to_wake_up() to check whether the task can be woken up.
 *
 * The caller holds p::pi_lock if p != current or has preemption
 * disabled when p == current.
 *
 * The rules of saved_state:
 *
 *   The related locking code always holds p::pi_lock when updating
 *   p::saved_state, which means the code is fully serialized in both cases.
 *
 *   For PREEMPT_RT, the lock wait and lock wakeups happen via TASK_RTLOCK_WAIT.
 *   No other bits set. This allows to distinguish all wakeup scenarios.
 *
 *   For FREEZER, the wakeup happens via TASK_FROZEN. No other bits set. This
 *   allows us to prevent early wakeup of tasks before they can be run on
 *   asymmetric ISA architectures (eg ARMv9).
 */
ساكن __always_inline
bool ttwu_state_match(بنية task_struct *p, unsigned صحيح state, صحيح *success)
{
	صحيح match;

	إذا (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		WARN_ON_ONCE((state & TASK_RTLOCK_WAIT) &&
			     state != TASK_RTLOCK_WAIT);
	}

	*success = !!(match = __task_state_match(p, state));

	/*
	 * Saved state preserves the task state across blocking on
	 * an RT lock or TASK_FREEZABLE tasks.  If the state matches,
	 * set p::saved_state to TASK_RUNNING, but do not wake the task
	 * because it waits for a lock wakeup or __thaw_task(). Also
	 * indicate success because from the regular waker's point of
	 * view this has succeeded.
	 *
	 * After acquiring the lock the task will restore p::__state
	 * from p::saved_state which ensures that the regular
	 * wakeup is not lost. The restore will also set
	 * p::saved_state to TASK_RUNNING so any further tests will
	 * not result in false positives vs. @success
	 */
	إذا (match < 0)
		p->saved_state = TASK_RUNNING;

	إرجاع match > 0;
}

/*
 * Notes on Program-Order guarantees on SMP systems.
 *
 *  MIGRATION
 *
 * The basic program-order guarantee on SMP systems is that when a task [t]
 * migrates, all its activity on its old CPU [c0] happens-before any subsequent
 * execution on its new CPU [c1].
 *
 * For migration (of runnable tasks) this is provided by the following means:
 *
 *  A) UNLOCK of the rq(c0)->lock scheduling out task t
 *  B) migration for t is required to synchronize *both* rq(c0)->lock and
 *     rq(c1)->lock (if not at the same time, then in that order).
 *  C) LOCK of the rq(c1)->lock scheduling in task
 *
 * Release/acquire chaining guarantees that B happens after A and C after B.
 * Note: the CPU doing B need not be c0 or c1
 *
 * Example:
 *
 *   CPU0            CPU1            CPU2
 *
 *   LOCK rq(0)->lock
 *   sched-out X
 *   sched-in Y
 *   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(0)->lock // orders against CPU0
 *                                   dequeue X
 *                                   UNLOCK rq(0)->lock
 *
 *                                   LOCK rq(1)->lock
 *                                   enqueue X
 *                                   UNLOCK rq(1)->lock
 *
 *                   LOCK rq(1)->lock // orders against CPU2
 *                   sched-out Z
 *                   sched-in X
 *                   UNLOCK rq(1)->lock
 *
 *
 *  BLOCKING -- aka. SLEEP + WAKEUP
 *
 * For blocking we (obviously) need to provide the same guarantee as for
 * migration. However the means are completely different as there is no lock
 * chain to provide order. Instead we do:
 *
 *   1) smp_store_release(X->on_cpu, 0)   -- finish_task()
 *   2) smp_cond_load_acquire(!X->on_cpu) -- try_to_wake_up()
 *
 * Example:
 *
 *   CPU0 (schedule)  CPU1 (try_to_wake_up) CPU2 (schedule)
 *
 *   LOCK rq(0)->lock LOCK X->pi_lock
 *   dequeue X
 *   sched-out X
 *   smp_store_release(X->on_cpu, 0);
 *
 *                    smp_cond_load_acquire(&X->on_cpu, !VAL);
 *                    X->state = WAKING
 *                    set_task_cpu(X,2)
 *
 *                    LOCK rq(2)->lock
 *                    enqueue X
 *                    X->state = RUNNING
 *                    UNLOCK rq(2)->lock
 *
 *                                          LOCK rq(2)->lock // orders against CPU1
 *                                          sched-out Z
 *                                          sched-in X
 *                                          UNLOCK rq(2)->lock
 *
 *                    UNLOCK X->pi_lock
 *   UNLOCK rq(0)->lock
 *
 *
 * However, for wakeups there is a second guarantee we must provide, namely we
 * must ensure that CONDITION=1 done by the caller can not be reordered with
 * accesses to the task state; see try_to_wake_up() and set_current_state().
 */

/**
 * try_to_wake_up - wake up a thread
 * @p: the thread to be awakened
 * @state: the mask of task states that can be woken
 * @wake_flags: wake modifier flags (WF_*)
 *
 * Conceptually does:
 *
 *   If (@state & @p->state) @p->state = TASK_RUNNING.
 *
 * If the task was not queued/runnable, also place it back on a runqueue.
 *
 * This function is atomic against schedule() which would dequeue the task.
 *
 * It issues a full memory barrier before accessing @p->state, see the comment
 * with set_current_state().
 *
 * Uses p->pi_lock to serialize against concurrent wake-ups.
 *
 * Relies on p->pi_lock stabilizing:
 *  - p->sched_class
 *  - p->cpus_ptr
 *  - p->sched_task_group
 * in order to do migration, see its use of select_task_rq()/set_task_cpu().
 *
 * Tries really hard to only take one task_rq(p)->lock for performance.
 * Takes rq->lock in:
 *  - ttwu_runnable()    -- old rq, unavoidable, see comment there;
 *  - ttwu_queue()       -- new rq, for enqueue of the task;
 *  - psi_ttwu_dequeue() -- much sadness :-( accounting will kill us.
 *
 * As a consequence we race really badly with just about everything. See the
 * many memory barriers and their comments for details.
 *
 * Return: %true if @p->state changes (an actual wakeup was done),
 *	   %false otherwise.
 */
صحيح try_to_wake_up(بنية task_struct *p, unsigned صحيح state, صحيح wake_flags)
{
	guard(preempt)();
	صحيح cpu, success = 0;

	wake_flags |= WF_TTWU;

	إذا (p == current) {
		/*
		 * We're waking current, this means 'p->on_rq' and 'task_cpu(p)
		 * == smp_processor_id()'. Together this means we can special
		 * case the whole 'p->on_rq && ttwu_runnable()' case below
		 * without taking any locks.
		 *
		 * Specifically, given current runs ttwu() we must be before
		 * schedule()'s block_task(), as such this must not observe
		 * sched_delayed.
		 *
		 * In particular:
		 *  - we rely on Program-Order guarantees for all the ordering,
		 *  - we're serialized against set_special_state() by virtue of
		 *    it disabling IRQs (this allows not taking ->pi_lock).
		 */
		WARN_ON_ONCE(p->se.sched_delayed);
		WARN_ON_ONCE(p->is_blocked);
		/* If p is current, we know we can run here, so clear blocked_on */
		clear_task_blocked_on(p, عدم);
		إذا (!ttwu_state_match(p, state, &success))
			اذهب out;

		trace_sched_waking(p);
		ttwu_do_wakeup(p);
		اذهب out;
	}

	/*
	 * If we are going to wake up a thread waiting for CONDITION we
	 * need to ensure that CONDITION=1 done by the caller can not be
	 * reordered with p->state check below. This pairs with smp_store_mb()
	 * in set_current_state() that the waiting thread does.
	 */
	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		smp_mb__after_spinlock();

		إذا (!ttwu_state_match(p, state, &success))
			اكسر;

		trace_sched_waking(p);

		/*
		 * Ensure we load p->on_rq _after_ p->state, otherwise it would
		 * be possible to, falsely, observe p->on_rq == 0 and get stuck
		 * in smp_cond_load_acquire() below.
		 *
		 * sched_ttwu_pending()			try_to_wake_up()
		 *   STORE p->on_rq = 1			  LOAD p->state
		 *   UNLOCK rq->lock
		 *
		 * __schedule() (switch to task 'p')
		 *   LOCK rq->lock			  smp_rmb();
		 *   smp_mb__after_spinlock();
		 *   UNLOCK rq->lock
		 *
		 * [task p]
		 *   STORE p->state = UNINTERRUPTIBLE	  LOAD p->on_rq
		 *
		 * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
		 * __schedule().  See the comment for smp_mb__after_spinlock().
		 *
		 * A similar smp_rmb() lives in __task_needs_rq_lock().
		 */
		smp_rmb();
		إذا (READ_ONCE(p->on_rq) && ttwu_runnable(p, wake_flags))
			اكسر;

		/*
		 * Ensure we load p->on_cpu _after_ p->on_rq, otherwise it would be
		 * possible to, falsely, observe p->on_cpu == 0.
		 *
		 * One must be running (->on_cpu == 1) in order to remove oneself
		 * from the runqueue.
		 *
		 * __schedule() (switch to task 'p')	try_to_wake_up()
		 *   STORE p->on_cpu = 1		  LOAD p->on_rq
		 *   UNLOCK rq->lock
		 *
		 * __schedule() (put 'p' to sleep)
		 *   LOCK rq->lock			  smp_rmb();
		 *   smp_mb__after_spinlock();
		 *   STORE p->on_rq = 0			  LOAD p->on_cpu
		 *
		 * Pairs with the LOCK+smp_mb__after_spinlock() on rq->lock in
		 * __schedule().  See the comment for smp_mb__after_spinlock().
		 *
		 * Form a control-dep-acquire with p->on_rq == 0 above, to ensure
		 * schedule()'s block_task() has 'happened' and p will no longer
		 * care about it's own p->state. See the comment in __schedule().
		 */
		smp_acquire__after_ctrl_dep();

		/*
		 * We're doing the wakeup (@success == 1), they did a dequeue (p->on_rq
		 * == 0), which means we need to do an enqueue, change p->state to
		 * TASK_WAKING such that we can unlock p->pi_lock before doing the
		 * enqueue, such as ttwu_queue_wakelist().
		 */
		WRITE_ONCE(p->__state, TASK_WAKING);

		/*
		 * If the owning (remote) CPU is still in the middle of schedule() with
		 * this task as prev, considering queueing p on the remote CPUs wake_list
		 * which potentially sends an IPI instead of spinning on p->on_cpu to
		 * let the waker make forward progress. This is safe because IRQs are
		 * disabled and the IPI will deliver after on_cpu is cleared.
		 *
		 * Ensure we load task_cpu(p) after p->on_cpu:
		 *
		 * set_task_cpu(p, cpu);
		 *   STORE p->cpu = @cpu
		 * __schedule() (switch to task 'p')
		 *   LOCK rq->lock
		 *   smp_mb__after_spin_lock()		smp_cond_load_acquire(&p->on_cpu)
		 *   STORE p->on_cpu = 1		LOAD p->cpu
		 *
		 * to ensure we observe the correct CPU on which the task is currently
		 * scheduling.
		 */
		إذا (smp_load_acquire(&p->on_cpu) &&
		    ttwu_queue_wakelist(p, task_cpu(p), wake_flags))
			اكسر;

		/*
		 * If the owning (remote) CPU is still in the middle of schedule() with
		 * this task as prev, wait until it's done referencing the task.
		 *
		 * Pairs with the smp_store_release() in finish_task().
		 *
		 * This ensures that tasks getting woken will be fully ordered against
		 * their previous state and preserve Program Order.
		 */
		smp_cond_load_acquire(&p->on_cpu, !VAL);

		cpu = select_task_rq(p, p->wake_cpu, &wake_flags);
		إذا (task_cpu(p) != cpu) {
			إذا (p->in_iowait) {
				delayacct_blkio_end(p);
				atomic_dec(&task_rq(p)->nr_iowait);
			}

			wake_flags |= WF_MIGRATED;
			psi_ttwu_dequeue(p);
			set_task_cpu(p, cpu);
		} وإلا إذا (cpu != p->wake_cpu) {
			/*
			 * If we were proxy-migrated to cpu, then
			 * select_task_rq() picks cpu instead of wake_cpu
			 * to return to, we won't call set_task_cpu(),
			 * leaving a stale wake_cpu pointing to where we
			 * proxy-migrated from. So just fixup wake_cpu here
			 * if its not correct
			 */
			p->wake_cpu = cpu;
		}

		ttwu_queue(p, cpu, wake_flags);
	}
out:
	إذا (success)
		ttwu_stat(p, task_cpu(p), wake_flags);

	إرجاع success;
}

ساكن bool __task_needs_rq_lock(بنية task_struct *p)
{
	unsigned صحيح state = READ_ONCE(p->__state);

	/*
	 * Since pi->lock blocks try_to_wake_up(), we don't need rq->lock when
	 * the task is blocked. Make sure to check @state since ttwu() can drop
	 * locks at the end, see ttwu_queue_wakelist().
	 */
	إذا (state == TASK_RUNNING || state == TASK_WAKING)
		إرجاع true;

	/*
	 * Ensure we load p->on_rq after p->__state, otherwise it would be
	 * possible to, falsely, observe p->on_rq == 0.
	 *
	 * See try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	إذا (p->on_rq)
		إرجاع true;

	/*
	 * Ensure the task has finished __schedule() and will not be referenced
	 * anymore. Again, see try_to_wake_up() for a longer comment.
	 */
	smp_rmb();
	smp_cond_load_acquire(&p->on_cpu, !VAL);

	إرجاع false;
}

/**
 * task_call_func - Invoke a function on task in fixed state
 * @p: Process for which the function is to be invoked, can be @current.
 * @func: Function to invoke.
 * @arg: Argument to function.
 *
 * Fix the task in it's current state by avoiding wakeups and or rq operations
 * and call @func(@arg) on it.  This function can use task_is_runnable() and
 * task_curr() to work out what the state is, if required.  Given that @func
 * can be invoked with a runqueue lock held, it had better be quite
 * lightweight.
 *
 * Returns:
 *   Whatever @func returns
 */
صحيح task_call_func(بنية task_struct *p, task_call_f func, فراغ *arg)
{
	بنية rq_flags rf;
	صحيح ret;

	raw_spin_lock_irqsave(&p->pi_lock, rf.flags);

	إذا (__task_needs_rq_lock(p)) {
		بنية rq *rq = __task_rq_lock(p, &rf);

		/*
		 * At this point the task is pinned; either:
		 *  - blocked and we're holding off wakeups	 (pi->lock)
		 *  - woken, and we're holding off enqueue	 (rq->lock)
		 *  - queued, and we're holding off schedule	 (rq->lock)
		 *  - running, and we're holding off de-schedule (rq->lock)
		 *
		 * The called function (@func) can use: task_curr(), p->on_rq and
		 * p->__state to differentiate between these states.
		 */
		ret = func(p, arg);

		__task_rq_unlock(rq, p, &rf);
	} وإلا {
		ret = func(p, arg);
	}

	raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);
	إرجاع ret;
}

/**
 * cpu_curr_snapshot - Return a snapshot of the currently running task
 * @cpu: The CPU on which to snapshot the task.
 *
 * Returns the task_struct pointer of the task "currently" running on
 * the specified CPU.
 *
 * If the specified CPU was offline, the return value is whatever it
 * is, perhaps a pointer to the task_struct structure of that CPU's idle
 * task, but there is no guarantee.  Callers wishing a useful return
 * value must take some action to ensure that the specified CPU remains
 * online throughout.
 *
 * This function executes full memory barriers before and after fetching
 * the pointer, which permits the caller to confine this function's fetch
 * with respect to the caller's accesses to other shared variables.
 */
بنية task_struct *cpu_curr_snapshot(صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);
	بنية task_struct *t;
	بنية rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	smp_mb__after_spinlock(); /* Pairing determined by caller's synchronization design. */
	t = rcu_dereference(cpu_curr(cpu));
	rq_unlock_irqrestore(rq, &rf);
	smp_mb(); /* Pairing determined by caller's synchronization design. */

	إرجاع t;
}

/**
 * wake_up_process - Wake up a specific process
 * @p: The process to be woken up.
 *
 * Attempt to wake up the nominated process and move it to the set of runnable
 * processes.
 *
 * Return: 1 if the process was woken up, 0 if it was already running.
 *
 * This function executes a full memory barrier before accessing the task state.
 */
صحيح wake_up_process(بنية task_struct *p)
{
	إرجاع try_to_wake_up(p, TASK_NORMAL, 0);
}
EXPORT_رمز(wake_up_process);

صحيح wake_up_state(بنية task_struct *p, unsigned صحيح state)
{
	إرجاع try_to_wake_up(p, state, 0);
}

/*
 * Perform scheduler related setup for a newly forked process p.
 * p is forked by current.
 *
 * __sched_fork() is basic setup which is also used by sched_init() to
 * initialize the boot CPU's idle task.
 */
ساكن فراغ __sched_fork(u64 clone_flags, بنية task_struct *p)
{
	p->on_rq			= 0;

	p->se.on_rq			= 0;
	p->se.exec_start		= 0;
	p->se.sum_exec_runtime		= 0;
	p->se.prev_sum_exec_runtime	= 0;
	p->se.nr_migrations		= 0;
	p->se.vruntime			= 0;
	p->se.vlag			= 0;
	p->se.rel_deadline		= 0;
	INIT_LIST_HEAD(&p->se.group_عقدة);

	/* A delayed task cannot be in clone(). */
	WARN_ON_ONCE(p->se.sched_delayed);
	WARN_ON_ONCE(p->is_blocked);

#ifdef CONFIG_FAIR_GROUP_SCHED
	p->se.cfs_rq			= عدم;
#ifdef CONFIG_CFS_BANDWIDTH
	init_cfs_throttle_work(p);
#endif
#endif

#ifdef CONFIG_SCHEDSTATS
	/* Even if schedstat is disabled, there should not be garbage */
	memset(&p->stats, 0, حجم(p->stats));
#endif

	init_dl_entity(&p->dl);

	INIT_LIST_HEAD(&p->rt.run_list);
	p->rt.timeout		= 0;
	p->rt.time_slice	= sched_rr_timeslice;
	p->rt.on_rq		= 0;
	p->rt.on_list		= 0;

#ifdef CONFIG_SCHED_CLASS_EXT
	init_scx_entity(&p->scx);
#endif

#ifdef CONFIG_PREEMPT_NOTIFIERS
	INIT_HLIST_HEAD(&p->preempt_notifiers);
#endif

#ifdef CONFIG_COMPACTION
	p->capture_control = عدم;
#endif
	init_numa_balancing(clone_flags, p);
	p->wake_entry.u_flags = CSD_TYPE_TTWU;
	p->migration_pending = عدم;
	init_sched_mm(p);
}

DEFINE_STATIC_مفتاح_FALSE(sched_numa_balancing);

#ifdef CONFIG_NUMA_BALANCING

صحيح sysctl_numa_balancing_mode;

ساكن فراغ __set_numabalancing_state(bool enabled)
{
	إذا (enabled)
		static_branch_enable(&sched_numa_balancing);
	وإلا
		static_branch_disable(&sched_numa_balancing);
}

فراغ set_numabalancing_state(bool enabled)
{
	إذا (enabled)
		sysctl_numa_balancing_mode = NUMA_BALANCING_NORMAL;
	وإلا
		sysctl_numa_balancing_mode = NUMA_BALANCING_DISABLED;
	__set_numabalancing_state(enabled);
}

#ifdef CONFIG_SYSCTL
ساكن فراغ reset_memory_tiering(فراغ)
{
	بنية pglist_data *pgdat;

	for_each_online_pgdat(pgdat) {
		pgdat->nbp_threshold = 0;
		pgdat->nbp_th_nr_cand = عقدة_page_state(pgdat, PGPROMOTE_CANDIDATE);
		pgdat->nbp_th_start = jiffies_to_msecs(jiffies);
	}
}

ساكن صحيح sysctl_numa_balancing(ثابت بنية ctl_table *table, صحيح اكتب,
			  فراغ *buffer, size_t *lenp, loff_t *ppos)
{
	بنية ctl_table t;
	صحيح err;
	صحيح state = sysctl_numa_balancing_mode;

	إذا (اكتب && !capable(CAP_SYS_ADMIN))
		إرجاع -EPERM;

	t = *table;
	t.data = &state;
	err = proc_dointvec_minmax(&t, اكتب, buffer, lenp, ppos);
	إذا (err < 0)
		إرجاع err;
	إذا (اكتب) {
		إذا (!(sysctl_numa_balancing_mode & NUMA_BALANCING_MEMORY_TIERING) &&
		    (state & NUMA_BALANCING_MEMORY_TIERING))
			reset_memory_tiering();
		sysctl_numa_balancing_mode = state;
		__set_numabalancing_state(state);
	}
	إرجاع err;
}
#endif /* CONFIG_SYSCTL */
#endif /* CONFIG_NUMA_BALANCING */

#ifdef CONFIG_SCHEDSTATS

DEFINE_STATIC_مفتاح_FALSE(sched_schedstats);

ساكن فراغ set_schedstats(bool enabled)
{
	إذا (enabled)
		static_branch_enable(&sched_schedstats);
	وإلا
		static_branch_disable(&sched_schedstats);
}

فراغ force_schedstat_enabled(فراغ)
{
	إذا (!schedstat_enabled()) {
		pr_info("kernel profiling enabled schedstats, disable via kernel.sched_schedstats.\n");
		static_branch_enable(&sched_schedstats);
	}
}

ساكن صحيح __init setup_schedstats(حرف *str)
{
	صحيح ret = 0;
	إذا (!str)
		اذهب out;

	إذا (!strcmp(str, "enable")) {
		set_schedstats(true);
		ret = 1;
	} وإلا إذا (!strcmp(str, "disable")) {
		set_schedstats(false);
		ret = 1;
	}
out:
	إذا (!ret)
		pr_warn("Unable to parse schedstats=\n");

	إرجاع ret;
}
__setup("schedstats=", setup_schedstats);

#ifdef CONFIG_SYSCTL
ساكن صحيح sysctl_schedstats(ثابت بنية ctl_table *table, صحيح اكتب, فراغ *buffer,
		size_t *lenp, loff_t *ppos)
{
	بنية ctl_table t;
	صحيح err;
	صحيح state = static_branch_likely(&sched_schedstats);

	إذا (اكتب && !capable(CAP_SYS_ADMIN))
		إرجاع -EPERM;

	t = *table;
	t.data = &state;
	err = proc_dointvec_minmax(&t, اكتب, buffer, lenp, ppos);
	إذا (err < 0)
		إرجاع err;
	إذا (اكتب)
		set_schedstats(state);
	إرجاع err;
}
#endif /* CONFIG_SYSCTL */
#endif /* CONFIG_SCHEDSTATS */

#ifdef CONFIG_SYSCTL
ساكن ثابت بنية ctl_table sched_core_sysctls[] = {
#ifdef CONFIG_SCHEDSTATS
	{
		.procname       = "sched_schedstats",
		.data           = عدم,
		.maxlen         = حجم(unsigned صحيح),
		.mode           = 0644,
		.proc_handler   = sysctl_schedstats,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
#endif /* CONFIG_SCHEDSTATS */
#ifdef CONFIG_UCLAMP_TASK
	{
		.procname       = "sched_util_clamp_min",
		.data           = &sysctl_sched_uclamp_util_min,
		.maxlen         = حجم(unsigned صحيح),
		.mode           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_max",
		.data           = &sysctl_sched_uclamp_util_max,
		.maxlen         = حجم(unsigned صحيح),
		.mode           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
	{
		.procname       = "sched_util_clamp_min_rt_default",
		.data           = &sysctl_sched_uclamp_util_min_rt_default,
		.maxlen         = حجم(unsigned صحيح),
		.mode           = 0644,
		.proc_handler   = sysctl_sched_uclamp_handler,
	},
#endif /* CONFIG_UCLAMP_TASK */
#ifdef CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing",
		.data		= عدم, /* filled in by handler */
		.maxlen		= حجم(unsigned صحيح),
		.mode		= 0644,
		.proc_handler	= sysctl_numa_balancing,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_FOUR,
	},
#endif /* CONFIG_NUMA_BALANCING */
};
ساكن صحيح __init sched_core_sysctl_init(فراغ)
{
	register_sysctl_init("kernel", sched_core_sysctls);
	إرجاع 0;
}
late_initcall(sched_core_sysctl_init);
#endif /* CONFIG_SYSCTL */

/*
 * fork()/clone()-time setup:
 */
صحيح sched_fork(u64 clone_flags, بنية task_struct *p)
{
	__sched_fork(clone_flags, p);
	/*
	 * We mark the process as NEW here. This guarantees that
	 * nobody will actually run it, and a signal or other external
	 * event cannot wake it up and insert it on the runqueue either.
	 */
	p->__state = TASK_NEW;

	/*
	 * Make sure we do not leak PI boosting priority to the child.
	 */
	p->prio = current->normal_prio;

	uclamp_fork(p);

	/*
	 * Revert to default priority/policy on fork if requested.
	 */
	إذا (unlikely(p->sched_reset_on_fork)) {
		إذا (task_has_dl_policy(p) || task_has_rt_policy(p)) {
			p->policy = SCHED_NORMAL;
			p->static_prio = NICE_TO_PRIO(0);
			p->rt_priority = 0;
			p->timer_slack_ns = p->default_timer_slack_ns;
		} وإلا إذا (PRIO_TO_NICE(p->static_prio) < 0)
			p->static_prio = NICE_TO_PRIO(0);

		p->prio = p->normal_prio = p->static_prio;
		set_load_weight(p, false);
		p->se.custom_slice = 0;
		p->se.slice = sysctl_sched_base_slice;

		/*
		 * We don't need the reset flag anymore after the fork. It has
		 * fulfilled its duty:
		 */
		p->sched_reset_on_fork = 0;
	}

	إذا (dl_prio(p->prio))
		إرجاع -EAGAIN;

	scx_pre_fork(p);

	إذا (rt_prio(p->prio)) {
		p->sched_class = &rt_sched_class;
#ifdef CONFIG_SCHED_CLASS_EXT
	} وإلا إذا (task_should_scx(p->policy)) {
		p->sched_class = &ext_sched_class;
#endif
	} وإلا {
		p->sched_class = &fair_sched_class;
	}

	init_entity_runnable_average(&p->se);


#ifdef CONFIG_SCHED_INFO
	إذا (likely(sched_info_on()))
		memset(&p->sched_info, 0, حجم(p->sched_info));
#endif
	p->on_cpu = 0;
	init_task_preempt_تعداد(p);
	plist_عقدة_init(&p->pushable_tasks, MAX_PRIO);
	RB_CLEAR_عقدة(&p->pushable_dl_tasks);

	إرجاع 0;
}

صحيح sched_cgroup_fork(بنية task_struct *p, بنية kernel_clone_args *kargs)
{
	unsigned long flags;

	/*
	 * Because we're not yet on the pid-hash, p->pi_lock isn't strictly
	 * required yet, but lockdep gets upset if rules are violated.
	 */
	raw_spin_lock_irqsave(&p->pi_lock, flags);
#ifdef CONFIG_CGROUP_SCHED
	إذا (1) {
		بنية task_group *tg;
		tg = container_of(kargs->cset->subsys[cpu_cgrp_id],
				  بنية task_group, css);
		tg = autogroup_task_group(p, tg);
		p->sched_task_group = tg;
	}
#endif
	/*
	 * We're setting the CPU for the first time, we don't migrate,
	 * so use __set_task_cpu().
	 */
	__set_task_cpu(p, smp_processor_id());
	إذا (p->sched_class->task_fork)
		p->sched_class->task_fork(p);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	إرجاع scx_fork(p, kargs);
}

فراغ sched_cancel_fork(بنية task_struct *p)
{
	scx_cancel_fork(p);
}

ساكن فراغ sched_mm_cid_fork(بنية task_struct *t);

فراغ sched_post_fork(بنية task_struct *p)
{
	sched_mm_cid_fork(p);
	uclamp_post_fork(p);
	scx_post_fork(p);
}

u64 to_ratio(u64 period, u64 runtime)
{
	إذا (runtime == RUNTIME_INF)
		إرجاع BW_UNIT;

	/*
	 * Doing this here saves a lot of checks in all
	 * the calling paths, and returning zero seems
	 * safe for them anyway.
	 */
	إذا (period == 0)
		إرجاع 0;

	إرجاع div64_u64(runtime << BW_SHIFT, period);
}

/*
 * wake_up_new_task - wake up a newly created task for the first time.
 *
 * This function will do some initial scheduler statistics housekeeping
 * that must be done for every newly created context, then puts the task
 * on the runqueue and wakes it.
 */
فراغ wake_up_new_task(بنية task_struct *p)
{
	بنية rq_flags rf;
	بنية rq *rq;
	صحيح wake_flags = WF_FORK;

	raw_spin_lock_irqsave(&p->pi_lock, rf.flags);
	WRITE_ONCE(p->__state, TASK_RUNNING);
	/*
	 * Fork balancing, do it here and not earlier because:
	 *  - cpus_ptr can change in the fork path
	 *  - any previously selected CPU might disappear through hotplug
	 *
	 * Use __set_task_cpu() to avoid calling sched_class::migrate_task_rq,
	 * as we're not fully set-up yet.
	 */
	p->recent_used_cpu = task_cpu(p);
	__set_task_cpu(p, select_task_rq(p, task_cpu(p), &wake_flags));
	rq = __task_rq_lock(p, &rf);
	update_rq_clock(rq);
	post_init_entity_util_avg(p);

	activate_task(rq, p, ENQUEUE_NOCLOCK | ENQUEUE_INITIAL);
	trace_sched_wakeup_new(p);
	wakeup_preempt(rq, p, wake_flags);
	إذا (p->sched_class->task_woken) {
		/*
		 * Nothing relies on rq->lock after this, so it's fine to
		 * drop it.
		 */
		rq_unpin_lock(rq, &rf);
		p->sched_class->task_woken(rq, p);
		rq_repin_lock(rq, &rf);
	}
	task_rq_unlock(rq, p, &rf);
}

#ifdef CONFIG_PREEMPT_NOTIFIERS

ساكن DEFINE_STATIC_مفتاح_FALSE(preempt_notifier_مفتاح);

فراغ preempt_notifier_inc(فراغ)
{
	static_branch_inc(&preempt_notifier_مفتاح);
}
EXPORT_رمز_GPL(preempt_notifier_inc);

فراغ preempt_notifier_dec(فراغ)
{
	static_branch_dec(&preempt_notifier_مفتاح);
}
EXPORT_رمز_GPL(preempt_notifier_dec);

/**
 * preempt_notifier_register - tell me when current is being preempted & rescheduled
 * @notifier: notifier struct to register
 */
فراغ preempt_notifier_register(بنية preempt_notifier *notifier)
{
	إذا (!static_branch_unlikely(&preempt_notifier_مفتاح))
		WARN(1, "registering preempt_notifier while notifiers disabled\n");

	hlist_add_head(&notifier->link, &current->preempt_notifiers);
}
EXPORT_رمز_GPL(preempt_notifier_register);

/**
 * preempt_notifier_unregister - no longer interested in preemption notifications
 * @notifier: notifier struct to unregister
 *
 * This is *not* safe to call from within a preemption notifier.
 */
فراغ preempt_notifier_unregister(بنية preempt_notifier *notifier)
{
	hlist_del(&notifier->link);
}
EXPORT_رمز_GPL(preempt_notifier_unregister);

ساكن فراغ __fire_sched_in_preempt_notifiers(بنية task_struct *curr)
{
	بنية preempt_notifier *notifier;

	hlist_for_each_entry(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_in(notifier, raw_smp_processor_id());
}

ساكن __always_inline فراغ fire_sched_in_preempt_notifiers(بنية task_struct *curr)
{
	إذا (static_branch_unlikely(&preempt_notifier_مفتاح))
		__fire_sched_in_preempt_notifiers(curr);
}

ساكن فراغ
__fire_sched_out_preempt_notifiers(بنية task_struct *curr,
				   بنية task_struct *next)
{
	بنية preempt_notifier *notifier;

	hlist_for_each_entry(notifier, &curr->preempt_notifiers, link)
		notifier->ops->sched_out(notifier, next);
}

ساكن __always_inline فراغ
fire_sched_out_preempt_notifiers(بنية task_struct *curr,
				 بنية task_struct *next)
{
	إذا (static_branch_unlikely(&preempt_notifier_مفتاح))
		__fire_sched_out_preempt_notifiers(curr, next);
}

#وإلا /* !CONFIG_PREEMPT_NOTIFIERS: */

ساكن مضمن فراغ fire_sched_in_preempt_notifiers(بنية task_struct *curr)
{
}

ساكن مضمن فراغ
fire_sched_out_preempt_notifiers(بنية task_struct *curr,
				 بنية task_struct *next)
{
}

#endif /* !CONFIG_PREEMPT_NOTIFIERS */

ساكن مضمن فراغ prepare_task(بنية task_struct *next)
{
	/*
	 * Claim the task as running, we do this before switching to it
	 * such that any running task will have this set.
	 *
	 * See the smp_load_acquire(&p->on_cpu) case in ttwu() and
	 * its ordering comment.
	 */
	WRITE_ONCE(next->on_cpu, 1);
}

ساكن مضمن فراغ finish_task(بنية task_struct *prev)
{
	/*
	 * This must be the very last reference to @prev from this CPU. After
	 * p->on_cpu is cleared, the task can be moved to a different CPU. We
	 * must ensure this doesn't happen until the switch is completely
	 * finished.
	 *
	 * In particular, the load of prev->state in finish_task_switch() must
	 * happen before this.
	 *
	 * Pairs with the smp_cond_load_acquire() in try_to_wake_up().
	 */
	smp_store_release(&prev->on_cpu, 0);
}

/*
 * Only called from __schedule context
 *
 * There are some cases where we are going to re-do the action
 * that added the balance callbacks. We may not be in a state
 * where we can run them, so just zap them so they can be
 * properly re-added on the next time around. This is similar
 * handling to running the callbacks, except we just don't call
 * them.
 */
ساكن فراغ zap_balance_callbacks(بنية rq *rq)
{
	بنية balance_callback *next, *head;
	bool found = false;

	lockdep_assert_rq_held(rq);

	head = rq->balance_callback;
	بينما (head) {
		إذا (head == &balance_ادفع_callback)
			found = true;
		next = head->next;
		head->next = عدم;
		head = next;
	}
	rq->balance_callback = found ? &balance_ادفع_callback : عدم;
}

ساكن فراغ do_balance_callbacks(بنية rq *rq, بنية balance_callback *head)
{
	فراغ (*func)(بنية rq *rq);
	بنية balance_callback *next;

	lockdep_assert_rq_held(rq);

	بينما (head) {
		func = head->func;
		next = head->next;
		head->next = عدم;
		head = next;

		func(rq);
	}
}

ساكن فراغ balance_ادفع(بنية rq *rq);

/*
 * balance_push_callback is a right abuse of the callback interface and plays
 * by significantly different rules.
 *
 * Where the normal balance_callback's purpose is to be ran in the same context
 * that queued it (only later, when it's safe to drop rq->lock again),
 * balance_push_callback is specifically targeted at __schedule().
 *
 * This abuse is tolerated because it places all the unlikely/odd cases behind
 * a single test, namely: rq->balance_callback == NULL.
 */
بنية balance_callback balance_ادفع_callback = {
	.next = عدم,
	.func = balance_ادفع,
};

ساكن مضمن بنية balance_callback *
__splice_balance_callbacks(بنية rq *rq, bool split)
{
	بنية balance_callback *head = rq->balance_callback;

	إذا (likely(!head))
		إرجاع عدم;

	lockdep_assert_rq_held(rq);
	/*
	 * Must not take balance_push_callback off the list when
	 * splice_balance_callbacks() and balance_callbacks() are not
	 * in the same rq->lock section.
	 *
	 * In that case it would be possible for __schedule() to interleave
	 * and observe the list empty.
	 */
	إذا (split && head == &balance_ادفع_callback)
		head = عدم;
	وإلا
		rq->balance_callback = عدم;

	إرجاع head;
}

بنية balance_callback *splice_balance_callbacks(بنية rq *rq)
{
	إرجاع __splice_balance_callbacks(rq, true);
}

فراغ __balance_callbacks(بنية rq *rq, بنية rq_flags *rf)
{
	إذا (rf)
		rq_unpin_lock(rq, rf);
	do_balance_callbacks(rq, __splice_balance_callbacks(rq, false));
	إذا (rf)
		rq_repin_lock(rq, rf);
}

فراغ balance_callbacks(بنية rq *rq, بنية balance_callback *head)
{
	unsigned long flags;

	إذا (unlikely(head)) {
		raw_spin_rq_lock_irqsave(rq, flags);
		do_balance_callbacks(rq, head);
		raw_spin_rq_unlock_irqrestore(rq, flags);
	}
}

ساكن مضمن فراغ
prepare_lock_switch(بنية rq *rq, بنية task_struct *next, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
	__acquires(__rq_lockp(this_rq()))
{
	/*
	 * Since the runqueue lock will be released by the next
	 * task (which is an invalid locking op but in the case
	 * of the scheduler it's an obvious special-case), so we
	 * do an early lockdep release here:
	 */
	rq_unpin_lock(rq, rf);
	spin_release(&__rq_lockp(rq)->dep_map, _THIS_IP_);
#ifdef CONFIG_DEBUG_SPINLOCK
	/* this is a valid case when another task releases the spinlock */
	rq_lockp(rq)->owner = next;
#endif
	/*
	 * Model the rq reference switcheroo.
	 */
	__release(__rq_lockp(rq));
	__acquire(__rq_lockp(this_rq()));
}

ساكن مضمن فراغ finish_lock_switch(بنية rq *rq)
	__releases(__rq_lockp(rq))
{
	/*
	 * If we are tracking spinlock dependencies then we have to
	 * fix up the runqueue lock - which gets 'carried over' from
	 * prev into current:
	 */
	spin_acquire(&__rq_lockp(rq)->dep_map, 0, 0, _THIS_IP_);
	__balance_callbacks(rq, عدم);
	hrtick_schedule_exit(rq);
	raw_spin_rq_unlock_irq(rq);
}

/*
 * NOP if the arch has not defined these:
 */

#ifndef prepare_arch_switch
# define prepare_arch_switch(next)	افعل { } بينما (0)
#endif

#ifndef finish_arch_post_lock_switch
# define finish_arch_post_lock_switch()	افعل { } بينما (0)
#endif

ساكن مضمن فراغ kmap_local_sched_out(فراغ)
{
#ifdef CONFIG_KMAP_LOCAL
	إذا (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_out();
#endif
}

ساكن مضمن فراغ kmap_local_sched_in(فراغ)
{
#ifdef CONFIG_KMAP_LOCAL
	إذا (unlikely(current->kmap_ctrl.idx))
		__kmap_local_sched_in();
#endif
}

/**
 * prepare_task_switch - prepare to switch tasks
 * @rq: the runqueue preparing to switch
 * @prev: the current task that is being switched out
 * @next: the task we are going to switch to.
 *
 * This is called with the rq lock held and interrupts off. It must
 * be paired with a subsequent finish_task_switch after the context
 * switch.
 *
 * prepare_task_switch sets up locking and calls architecture specific
 * hooks.
 */
ساكن مضمن فراغ
prepare_task_switch(بنية rq *rq, بنية task_struct *prev,
		    بنية task_struct *next)
	__must_hold(__rq_lockp(rq))
{
	kcov_prepare_switch(prev);
	sched_info_switch(rq, prev, next);
	perf_event_task_sched_out(prev, next);
	fire_sched_out_preempt_notifiers(prev, next);
	kmap_local_sched_out();
	prepare_task(next);
	prepare_arch_switch(next);
}

/**
 * finish_task_switch - clean up after a task-switch
 * @prev: the thread we just switched away from.
 *
 * finish_task_switch must be called after the context switch, paired
 * with a prepare_task_switch call before the context switch.
 * finish_task_switch will reconcile locking set up by prepare_task_switch,
 * and do any other architecture-specific cleanup actions.
 *
 * Note that we may have delayed dropping an mm in context_switch(). If
 * so, we finish that here outside of the runqueue lock. (Doing it
 * with the lock held can cause deadlocks; see schedule() for
 * details.)
 *
 * The context switch have flipped the stack from under us and restored the
 * local variables which were saved when this task called schedule() in the
 * past. 'prev == current' is still correct but we need to recalculate this_rq
 * because prev may have moved to another CPU.
 */
ساكن بنية rq *finish_task_switch(بنية task_struct *prev)
	__releases(__rq_lockp(this_rq()))
{
	بنية rq *rq = this_rq();
	بنية mm_struct *mm = rq->prev_mm;
	unsigned صحيح prev_state;

	/*
	 * The previous task will have left us with a preempt_count of 2
	 * because it left us after:
	 *
	 *	schedule()
	 *	  preempt_disable();			// 1
	 *	  __schedule()
	 *	    raw_spin_lock_irq(&rq->lock)	// 2
	 *
	 * Also, see FORK_PREEMPT_COUNT.
	 */
	إذا (WARN_ONCE(preempt_تعداد() != 2*PREEMPT_DISABLE_OFFSET,
		      "corrupted preempt_count: %s/%d/0x%x\n",
		      current->comm, current->pid, preempt_تعداد()))
		preempt_تعداد_set(FORK_PREEMPT_تعداد);

	rq->prev_mm = عدم;

	/*
	 * A task struct has one reference for the use as "current".
	 * If a task dies, then it sets TASK_DEAD in tsk->state and calls
	 * schedule one last time. The schedule call will never return, and
	 * the scheduled task must drop that reference.
	 *
	 * We must observe prev->state before clearing prev->on_cpu (in
	 * finish_task), otherwise a concurrent wakeup can get prev
	 * running on another CPU and we could rave with its RUNNING -> DEAD
	 * transition, resulting in a double drop.
	 */
	prev_state = READ_ONCE(prev->__state);
	vtime_task_switch(prev);
	perf_event_task_sched_in(prev, current);
	finish_task(prev);
	tick_nohz_task_switch();
	finish_lock_switch(rq);
	finish_arch_post_lock_switch();
	kcov_finish_switch(current);
	/*
	 * kmap_local_sched_out() is invoked with rq::lock held and
	 * interrupts disabled. There is no requirement for that, but the
	 * sched out code does not have an interrupt enabled section.
	 * Restoring the maps on sched in does not require interrupts being
	 * disabled either.
	 */
	kmap_local_sched_in();

	/*
	 * Any cached block-layer timestamp (plug->cur_ktime) is stale now,
	 * invalidate it.
	 */
	blk_plug_invalidate_ts();

	fire_sched_in_preempt_notifiers(current);
	/*
	 * When switching through a kernel thread, the loop in
	 * membarrier_{private,global}_expedited() may have observed that
	 * kernel thread and not issued an IPI. It is therefore possible to
	 * schedule between user->kernel->user threads without passing though
	 * switch_mm(). Membarrier requires a barrier after storing to
	 * rq->curr, before returning to userspace, so provide them here:
	 *
	 * - a full memory barrier for {PRIVATE,GLOBAL}_EXPEDITED, implicitly
	 *   provided by mmdrop_lazy_tlb(),
	 * - a sync_core for SYNC_CORE.
	 */
	إذا (mm) {
		membarrier_mm_sync_core_before_usermode(mm);
		mmdrop_lazy_tlb_sched(mm);
	}

	إذا (unlikely(prev_state == TASK_DEAD)) {
		إذا (prev->sched_class->task_dead)
			prev->sched_class->task_dead(prev);

		/*
		 * sched_ext_dead() must come before cgroup_task_dead() to
		 * prevent cgroups from being removed while its member tasks are
		 * visible to SCX schedulers.
		 */
		sched_ext_dead(prev);
		cgroup_task_dead(prev);

		/* Task is done with its stack. */
		put_task_مكدس(prev);

		put_task_struct_rcu_user(prev);
	}

	إرجاع rq;
}

/**
 * schedule_tail - first thing a freshly forked thread must call.
 * @prev: the thread we just switched away from.
 */
asmlinkage __visible فراغ schedule_tail(بنية task_struct *prev)
	__releases(__rq_lockp(this_rq()))
{
	/*
	 * New tasks start with FORK_PREEMPT_COUNT, see there and
	 * finish_task_switch() for details.
	 *
	 * finish_task_switch() will drop rq->lock() and lower preempt_count
	 * and the preempt_enable() will end up enabling preemption (on
	 * PREEMPT_COUNT kernels).
	 */

	finish_task_switch(prev);
	/*
	 * This is a special case: the newly created task has just
	 * switched the context for the first time. It is returning from
	 * schedule for the first time in this path.
	 */
	trace_sched_exit_tp(true);
	preempt_enable();

	إذا (current->set_child_tid)
		put_user(task_pid_vnr(current), current->set_child_tid);

	calculate_sigpending();
}

/*
 * context_switch - switch to the new MM and the new thread's register state.
 */
ساكن __always_inline بنية rq *
context_switch(بنية rq *rq, بنية task_struct *prev,
	       بنية task_struct *next, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	prepare_task_switch(rq, prev, next);

	/*
	 * For paravirt, this is coupled with an exit in switch_to to
	 * combine the page table reload and the switch backend into
	 * one hypercall.
	 */
	arch_start_context_switch(prev);

	/*
	 * kernel -> kernel   lazy + transfer active
	 *   user -> kernel   lazy + mmgrab_lazy_tlb() active
	 *
	 * kernel ->   user   switch + mmdrop_lazy_tlb() active
	 *   user ->   user   switch
	 */
	إذا (!next->mm) {				// to kernel
		enter_lazy_tlb(prev->active_mm, next);

		next->active_mm = prev->active_mm;
		إذا (prev->mm)				// from user
			mmgrab_lazy_tlb(prev->active_mm);
		وإلا
			prev->active_mm = عدم;
	} وإلا {					// to user
		membarrier_switch_mm(rq, prev->active_mm, next->mm);
		/*
		 * sys_membarrier() requires an smp_mb() between setting
		 * rq->curr / membarrier_switch_mm() and returning to userspace.
		 *
		 * The below provides this either through switch_mm(), or in
		 * case 'prev->active_mm == next->mm' through
		 * finish_task_switch()'s mmdrop().
		 */
		switch_mm_irqs_off(prev->active_mm, next->mm, next);
		lru_gen_use_mm(next->mm);

		إذا (!prev->mm) {			// from kernel
			/* will mmdrop_lazy_tlb() in finish_task_switch(). */
			rq->prev_mm = prev->active_mm;
			prev->active_mm = عدم;
		}
	}

	mm_cid_switch_to(prev, next);

	/*
	 * Tell rseq that the task was scheduled in. Must be after
	 * switch_mm_cid() to get the TIF flag set.
	 */
	rseq_sched_switch_event(next);

	prepare_lock_switch(rq, next, rf);

	/* Here we just switch the register state and the stack. */
	switch_to(prev, next, prev);
	barrier();

	إرجاع finish_task_switch(prev);
}

/*
 * nr_running and nr_context_switches:
 *
 * externally visible scheduler statistics: current number of runnable
 * threads, total number of context switches performed since bootup.
 */
unsigned صحيح nr_running(فراغ)
{
	unsigned صحيح i, sum = 0;

	for_each_online_cpu(i)
		sum += cpu_rq(i)->nr_running;

	إرجاع sum;
}

/*
 * Check if only the current task is running on the CPU.
 *
 * Caution: this function does not check that the caller has disabled
 * preemption, thus the result might have a time-of-check-to-time-of-use
 * race.  The caller is responsible to use it correctly, for example:
 *
 * - from a non-preemptible section (of course)
 *
 * - from a thread that is bound to a single CPU
 *
 * - in a loop with very short iterations (e.g. a polling loop)
 */
bool single_task_running(فراغ)
{
	إرجاع raw_rq()->nr_running == 1;
}
EXPORT_رمز(single_task_running);

unsigned long long nr_context_switches_cpu(صحيح cpu)
{
	إرجاع cpu_rq(cpu)->nr_switches;
}

unsigned long long nr_context_switches(فراغ)
{
	صحيح i;
	unsigned long long sum = 0;

	for_each_possible_cpu(i)
		sum += cpu_rq(i)->nr_switches;

	إرجاع sum;
}

/*
 * Return whether any tasks are accounted as I/O waiters on this CPU.
 * They may wake up on a different CPU.
 */
bool cpu_has_iowait(صحيح cpu)
{
	إرجاع atomic_read(&cpu_rq(cpu)->nr_iowait) != 0;
}

/*
 * IO-wait accounting, and how it's mostly bollocks (on SMP).
 *
 * The idea behind IO-wait account is to account the idle time that we could
 * have spend running if it were not for IO. That is, if we were to improve the
 * storage performance, we'd have a proportional reduction in IO-wait time.
 *
 * This all works nicely on UP, where, when a task blocks on IO, we account
 * idle time as IO-wait, because if the storage were faster, it could've been
 * running and we'd not be idle.
 *
 * This has been extended to SMP, by doing the same for each CPU. This however
 * is broken.
 *
 * Imagine for instance the case where two tasks block on one CPU, only the one
 * CPU will have IO-wait accounted, while the other has regular idle. Even
 * though, if the storage were faster, both could've ran at the same time,
 * utilising both CPUs.
 *
 * This means, that when looking globally, the current IO-wait accounting on
 * SMP is a lower bound, by reason of under accounting.
 *
 * Worse, since the numbers are provided per CPU, they are sometimes
 * interpreted per CPU, and that is nonsensical. A blocked task isn't strictly
 * associated with any one particular CPU, it can wake to another CPU than it
 * blocked on. This means the per CPU IO-wait number is meaningless.
 *
 * Task CPU affinities can make all that even more 'interesting'.
 */

unsigned صحيح nr_iowait(فراغ)
{
	unsigned صحيح i, sum = 0;

	for_each_possible_cpu(i)
		sum += atomic_read(&cpu_rq(i)->nr_iowait);

	إرجاع sum;
}

/*
 * sched_exec - execve() is a valuable balancing opportunity, because at
 * this point the task has the smallest effective memory and cache footprint.
 */
فراغ sched_exec(فراغ)
{
	بنية task_struct *p = current;
	بنية migration_arg arg;
	صحيح dest_cpu;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		dest_cpu = p->sched_class->select_task_rq(p, task_cpu(p), WF_EXEC);
		إذا (dest_cpu == smp_processor_id())
			إرجاع;

		إذا (unlikely(!cpu_active(dest_cpu)))
			إرجاع;

		arg = (بنية migration_arg){ p, dest_cpu };
	}
	stop_one_cpu(task_cpu(p), migration_cpu_stop, &arg);
}

DEFINE_PER_CPU(بنية kernel_stat, kstat);
DEFINE_PER_CPU(بنية kernel_cpustat, kernel_cpustat) = {
#ifdef CONFIG_NO_HZ_COMMON
	.idle_sleeptime_seq = SEQCNT_ZERO(kernel_cpustat.idle_sleeptime_seq)
#endif
};

EXPORT_PER_CPU_رمز(kstat);
EXPORT_PER_CPU_رمز(kernel_cpustat);

/*
 * The function fair_sched_class.update_curr accesses the struct curr
 * and its field curr->exec_start; when called from task_sched_runtime(),
 * we observe a high rate of cache misses in practice.
 * Prefetching this data results in improved performance.
 */
ساكن مضمن فراغ prefetch_curr_exec_start(بنية task_struct *p)
{
	بنية sched_entity *curr = task_rq(p)->cfs.curr;

	prefetch(curr);
	prefetch(&curr->exec_start);
}

/*
 * Return accounted runtime for the task.
 * In case the task is currently running, return the runtime plus current's
 * pending runtime that have not been accounted yet.
 */
unsigned long long task_sched_runtime(بنية task_struct *p)
{
	بنية rq_flags rf;
	بنية rq *rq;
	u64 ns;

#ifdef CONFIG_64BIT
	/*
	 * 64-bit doesn't need locks to atomically read a 64-bit value.
	 * So we have a optimization chance when the task's delta_exec is 0.
	 * Reading ->on_cpu is racy, but this is OK.
	 *
	 * If we race with it leaving CPU, we'll take a lock. So we're correct.
	 * If we race with it entering CPU, unaccounted time is 0. This is
	 * indistinguishable from the read occurring a few cycles earlier.
	 * If we see ->on_cpu without ->on_rq, the task is leaving, and has
	 * been accounted, so we're correct here as well.
	 */
	إذا (!p->on_cpu || !task_on_rq_queued(p))
		إرجاع p->se.sum_exec_runtime;
#endif

	rq = task_rq_lock(p, &rf);
	/*
	 * Must be ->curr _and_ ->on_rq.  If dequeued, we would
	 * project cycles that may never be accounted to this
	 * thread, breaking clock_gettime().
	 */
	إذا (task_current_donor(rq, p) && task_on_rq_queued(p)) {
		prefetch_curr_exec_start(p);
		update_rq_clock(rq);
		p->sched_class->update_curr(rq);
	}
	ns = p->se.sum_exec_runtime;
	task_rq_unlock(rq, p, &rf);

	إرجاع ns;
}

ساكن u64 cpu_resched_latency(بنية rq *rq)
{
	صحيح latency_warn_ms = READ_ONCE(sysctl_resched_latency_warn_ms);
	u64 resched_latency, now = rq_clock(rq);
	ساكن bool warned_once;

	إذا (sysctl_resched_latency_warn_once && warned_once)
		إرجاع 0;

	إذا (!need_resched() || !latency_warn_ms)
		إرجاع 0;

	إذا (system_state == SYSTEM_BOOTING)
		إرجاع 0;

	إذا (!rq->last_seen_need_resched_ns) {
		rq->last_seen_need_resched_ns = now;
		rq->ticks_without_resched = 0;
		إرجاع 0;
	}

	rq->ticks_without_resched++;
	resched_latency = now - rq->last_seen_need_resched_ns;
	إذا (resched_latency <= latency_warn_ms * NSEC_PER_MSEC)
		إرجاع 0;

	warned_once = true;

	إرجاع resched_latency;
}

ساكن صحيح __init setup_resched_latency_warn_ms(حرف *str)
{
	long val;

	إذا ((kstrtol(str, 0, &val))) {
		pr_warn("Unable to set resched_latency_warn_ms\n");
		إرجاع 1;
	}

	sysctl_resched_latency_warn_ms = val;
	إرجاع 1;
}
__setup("resched_latency_warn_ms=", setup_resched_latency_warn_ms);

/*
 * This function gets called by the timer code, with HZ frequency.
 * We call it with interrupts disabled.
 */
فراغ sched_tick(فراغ)
{
	صحيح cpu = smp_processor_id();
	بنية rq *rq = cpu_rq(cpu);
	/* scheduler accounting goes to the donor task */
	بنية task_struct *curr, *donor;
	بنية rq_flags rf;
	unsigned long hw_pressure;
	u64 resched_latency;

	إذا (!cpu_preferred(cpu))
		sched_ادفع_current_non_preferred_cpu(rq);

	إذا (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		arch_scale_freq_tick();

	sched_clock_tick();

	rq_lock(rq, &rf);
	curr = rq->curr;
	donor = rq->donor;

	psi_account_irqtime(rq, curr, عدم);

	update_rq_clock(rq);
	hw_pressure = arch_scale_hw_pressure(cpu_of(rq));
	update_hw_load_avg(rq_clock_task(rq), rq, hw_pressure);

	إذا (dynamic_preempt_lazy() && tif_test_bit(TIF_NEED_RESCHED_LAZY))
		resched_curr(rq);

	donor->sched_class->task_tick(rq, donor, 0);
	إذا (sched_feat(LATENCY_WARN))
		resched_latency = cpu_resched_latency(rq);
	calc_global_load_tick(rq);
	sched_core_tick(rq);
	scx_tick(rq);

	rq_unlock(rq, &rf);

	إذا (sched_feat(LATENCY_WARN) && resched_latency)
		resched_latency_warn(cpu, resched_latency);

	perf_event_task_tick();

	إذا (curr->flags & PF_WQ_WORKER)
		wq_worker_tick(curr);

	إذا (!scx_switched_all()) {
		rq->idle_balance = idle_cpu(cpu);
		sched_balance_trigger(rq);
	}
}

#ifdef CONFIG_NO_HZ_FULL

بنية tick_work {
	صحيح			cpu;
	atomic_t		state;
	بنية delayed_work	work;
};
/* Values for ->state, see diagram below. */
#define TICK_SCHED_REMOTE_OFFLINE	0
#define TICK_SCHED_REMOTE_OFFLINING	1
#define TICK_SCHED_REMOTE_RUNNING	2

/*
 * State diagram for ->state:
 *
 *
 *          TICK_SCHED_REMOTE_OFFLINE
 *                    |   ^
 *                    |   |
 *                    |   | sched_tick_remote()
 *                    |   |
 *                    |   |
 *                    +--TICK_SCHED_REMOTE_OFFLINING
 *                    |   ^
 *                    |   |
 * sched_tick_start() |   | sched_tick_stop()
 *                    |   |
 *                    V   |
 *          TICK_SCHED_REMOTE_RUNNING
 *
 *
 * Other transitions get WARN_ON_ONCE(), except that sched_tick_remote()
 * and sched_tick_start() are happy to leave the state in RUNNING.
 */

ساكن بنية tick_work __percpu *tick_work_cpu;

ساكن فراغ sched_tick_remote(بنية work_struct *work)
{
	بنية delayed_work *dwork = to_delayed_work(work);
	بنية tick_work *twork = container_of(dwork, بنية tick_work, work);
	صحيح cpu = twork->cpu;
	بنية rq *rq = cpu_rq(cpu);
	صحيح os;

	/*
	 * Handle the tick only if it appears the remote CPU is running in full
	 * dynticks mode. The check is racy by nature, but missing a tick or
	 * having one too much is no big deal because the scheduler tick updates
	 * statistics and checks timeslices in a time-independent way, regardless
	 * of when exactly it is running.
	 */
	إذا (tick_nohz_tick_stopped_cpu(cpu)) {
		guard(rq_lock_irq)(rq);
		بنية task_struct *curr = rq->curr;

		إذا (cpu_online(cpu)) {
			/*
			 * Since this is a remote tick for full dynticks mode,
			 * we are always sure that there is no proxy (only a
			 * single task is running).
			 */
			WARN_ON_ONCE(rq->curr != rq->donor);
			update_rq_clock(rq);

			إذا (!is_idle_task(curr)) {
				/*
				 * Make sure the next tick runs within a
				 * reasonable amount of time.
				 */
				u64 delta = rq_clock_task(rq) - curr->se.exec_start;
				WARN_ON_ONCE(delta > (u64)NSEC_PER_SEC * 30);
			}
			curr->sched_class->task_tick(rq, curr, 0);

			calc_load_nohz_remote(rq);
		}
	}

	/*
	 * Run the remote tick once per second (1Hz). This arbitrary
	 * frequency is large enough to avoid overload but short enough
	 * to keep scheduler internal stats reasonably up to date.  But
	 * first update state to reflect hotplug activity if required.
	 */
	os = atomic_fetch_add_unless(&twork->state, -1, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_OFFLINE);
	إذا (os == TICK_SCHED_REMOTE_RUNNING)
		queue_delayed_work(system_dfl_wq, dwork, HZ);
}

ساكن فراغ sched_tick_start(صحيح cpu)
{
	صحيح os;
	بنية tick_work *twork;

	إذا (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		إرجاع;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = per_cpu_ptr(tick_work_cpu, cpu);
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_RUNNING);
	WARN_ON_ONCE(os == TICK_SCHED_REMOTE_RUNNING);
	إذا (os == TICK_SCHED_REMOTE_OFFLINE) {
		twork->cpu = cpu;
		INIT_DELAYED_WORK(&twork->work, sched_tick_remote);
		queue_delayed_work(system_dfl_wq, &twork->work, HZ);
	}
}

#ifdef CONFIG_HOTPLUG_CPU
ساكن فراغ sched_tick_stop(صحيح cpu)
{
	بنية tick_work *twork;
	صحيح os;

	إذا (housekeeping_cpu(cpu, HK_TYPE_KERNEL_NOISE))
		إرجاع;

	WARN_ON_ONCE(!tick_work_cpu);

	twork = per_cpu_ptr(tick_work_cpu, cpu);
	/* There cannot be competing actions, but don't rely on stop-machine. */
	os = atomic_xchg(&twork->state, TICK_SCHED_REMOTE_OFFLINING);
	WARN_ON_ONCE(os != TICK_SCHED_REMOTE_RUNNING);
	/* Don't cancel, as this would mess up the state machine. */
}
#endif /* CONFIG_HOTPLUG_CPU */

صحيح __init sched_tick_offload_init(فراغ)
{
	tick_work_cpu = alloc_percpu(بنية tick_work);
	BUG_ON(!tick_work_cpu);
	إرجاع 0;
}

#وإلا /* !CONFIG_NO_HZ_FULL: */
ساكن مضمن فراغ sched_tick_start(صحيح cpu) { }
ساكن مضمن فراغ sched_tick_stop(صحيح cpu) { }
#endif /* !CONFIG_NO_HZ_FULL */

#إذا defined(CONFIG_PREEMPTION) && (defined(CONFIG_DEBUG_PREEMPT) || \
				defined(CONFIG_TRACE_PREEMPT_TOGGLE))
/*
 * If the value passed in is equal to the current preempt count
 * then we just disabled preemption. Start timing the latency.
 */
ساكن مضمن فراغ preempt_latency_start(صحيح val)
{
	إذا (preempt_تعداد() == val) {
		unsigned long ip = get_lock_parent_ip();
#ifdef CONFIG_DEBUG_PREEMPT
		current->preempt_disable_ip = ip;
#endif
		trace_preempt_off(CALLER_ADDR0, ip);
	}
}

فراغ preempt_تعداد_add(صحيح val)
{
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 *
	 * Cannot detect underflow based on the current preempt_count() value
	 * if using HAS_SEPARATE_PREEMPT_RESCHED_BITS because preempt count takes all 32
	 * bits.
	 */
	إذا (!IS_ENABLED(CONFIG_HAS_SEPARATE_PREEMPT_RESCHED_BITS) &&
	    DEBUG_LOCKS_WARN_ON((preempt_تعداد() < 0)))
		إرجاع;
#endif
	__preempt_count_add(val);
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Spinlock count overflowing soon?
	 */
	DEBUG_LOCKS_WARN_ON((preempt_تعداد() & PREEMPT_MASK) >=
				PREEMPT_MASK - 10);
#endif
	preempt_latency_start(val);
}
EXPORT_رمز(preempt_تعداد_add);
NOKPROBE_رمز(preempt_تعداد_add);

/*
 * If the value passed in equals to the current preempt count
 * then we just enabled preemption. Stop timing the latency.
 */
ساكن مضمن فراغ preempt_latency_stop(صحيح val)
{
	إذا (preempt_تعداد() == val)
		trace_preempt_on(CALLER_ADDR0, get_lock_parent_ip());
}

فراغ preempt_تعداد_sub(صحيح val)
{
#ifdef CONFIG_DEBUG_PREEMPT
	/*
	 * Underflow?
	 */
	unsigned صحيح uval = val;
	unsigned صحيح pc = preempt_تعداد();

	إذا (DEBUG_LOCKS_WARN_ON(pc - uval > pc))
		إرجاع;
	/*
	 * Is the spinlock portion underflowing?
	 */
	إذا (DEBUG_LOCKS_WARN_ON((val < PREEMPT_MASK) &&
			!(preempt_تعداد() & PREEMPT_MASK)))
		إرجاع;
#endif

	preempt_latency_stop(val);
	__preempt_count_sub(val);
}
EXPORT_رمز(preempt_تعداد_sub);
NOKPROBE_رمز(preempt_تعداد_sub);

#وإلا
ساكن مضمن فراغ preempt_latency_start(صحيح val) { }
ساكن مضمن فراغ preempt_latency_stop(صحيح val) { }
#endif

ساكن مضمن unsigned long get_preempt_disable_ip(بنية task_struct *p)
{
#ifdef CONFIG_DEBUG_PREEMPT
	إرجاع p->preempt_disable_ip;
#وإلا
	إرجاع 0;
#endif
}

/*
 * Print scheduling while atomic bug:
 */
ساكن noinline فراغ __schedule_bug(بنية task_struct *prev)
{
	/* Save this before calling printk(), since that will clobber it */
	unsigned long preempt_disable_ip = get_preempt_disable_ip(current);

	إذا (oops_in_progress)
		إرجاع;

	printk(KERN_ERR "BUG: scheduling while atomic: %s/%d/0x%08x\n",
		prev->comm, prev->pid, preempt_تعداد());

	debug_show_held_locks(prev);
	print_modules();
	إذا (irqs_disabled())
		print_irqtrace_events(prev);
	إذا (IS_ENABLED(CONFIG_DEBUG_PREEMPT)) {
		pr_err("Preemption disabled at:");
		print_ip_sym(KERN_ERR, preempt_disable_ip);
	}
	check_panic_on_warn("scheduling while atomic");

	dump_مكدس();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}

/*
 * Various schedule()-time debugging checks and statistics:
 */
ساكن مضمن فراغ schedule_debug(بنية task_struct *prev, bool preempt)
{
#ifdef CONFIG_SCHED_مكدس_END_CHECK
	إذا (task_مكدس_end_corrupted(prev))
		panic("corrupted stack end detected inside scheduler\n");

	إذا (task_scs_end_corrupted(prev))
		panic("corrupted shadow stack detected inside scheduler\n");
#endif

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP
	إذا (!preempt && READ_ONCE(prev->__state) && prev->non_block_تعداد) {
		printk(KERN_ERR "BUG: scheduling in a non-blocking section: %s/%d/%i\n",
			prev->comm, prev->pid, prev->non_block_تعداد);
		dump_مكدس();
		add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
	}
#endif

	إذا (unlikely(in_atomic_preempt_off())) {
		__schedule_bug(prev);
		preempt_تعداد_set(PREEMPT_DISABLED);
	}
	rcu_sleep_check();
	WARN_ON_ONCE(ct_state() == CT_STATE_USER);

	profile_hit(SCHED_PROFILING, __builtin_return_address(0));

	schedstat_inc(this_rq()->sched_تعداد);
}

ساكن فراغ prev_balance(بنية rq *rq, بنية rq_flags *rf)
{
	ثابت بنية sched_class *start_class = rq->donor->sched_class;
	ثابت بنية sched_class *class;

	/*
	 * We must do the balancing pass before put_prev_task(), such
	 * that when we release the rq->lock the task is in the same
	 * state as before we took rq->lock.
	 *
	 * We can terminate the balance pass as soon as we know there is
	 * a runnable task of @class priority or higher.
	 */
	for_active_class_range(class, start_class, &idle_sched_class) {
		إذا (class->balance && class->balance(rq, rf))
			اكسر;
	}
}

/*
 * Pick up the highest-prio task:
 */
ساكن مضمن بنية task_struct *
__pick_next_task(بنية rq *rq, بنية rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ثابت بنية sched_class *class;
	بنية task_struct *p;

	rq->dl_server = عدم;

	إذا (scx_enabled())
		اذهب restart;

	/*
	 * Optimization: we know that if all tasks are in the fair class we can
	 * call that function directly, but only if the @prev task wasn't of a
	 * higher scheduling class, because otherwise those lose the
	 * opportunity to pull in more work from other CPUs.
	 */
	إذا (likely(!sched_class_above(rq->donor->sched_class, &fair_sched_class) &&
		   rq->nr_running == rq->cfs.h_nr_queued)) {

		p = pick_task_fair(rq, rf);
		إذا (unlikely(p == RETRY_TASK))
			اذهب restart;

		/* Assume the next prioritized class is idle_sched_class */
		إذا (!p)
			p = pick_task_idle(rq, rf);

		put_prev_set_next_task(rq, rq->donor, p);
		إرجاع p;
	}

restart:
	prev_balance(rq, rf);

	for_each_active_class(class) {
		p = class->pick_task(rq, rf);
		إذا (unlikely(p == RETRY_TASK))
			اذهب restart;
		إذا (p) {
			put_prev_set_next_task(rq, rq->donor, p);
			إرجاع p;
		}
	}

	BUG(); /* The idle class should always have a runnable task. */
}

#ifdef CONFIG_SCHED_CORE
ساكن مضمن bool is_task_rq_idle(بنية task_struct *t)
{
	إرجاع (task_rq(t)->idle == t);
}

ساكن مضمن bool cookie_equals(بنية task_struct *a, unsigned long cookie)
{
	إرجاع is_task_rq_idle(a) || (a->core_cookie == cookie);
}

ساكن مضمن bool cookie_match(بنية task_struct *a, بنية task_struct *b)
{
	إذا (is_task_rq_idle(a) || is_task_rq_idle(b))
		إرجاع true;

	إرجاع a->core_cookie == b->core_cookie;
}

/*
 * Careful; this can return RETRY_TASK, it does not include the retry-loop
 * itself due to the whole SMT pick retry thing below.
 */
ساكن مضمن بنية task_struct *pick_task(بنية rq *rq, بنية rq_flags *rf)
{
	ثابت بنية sched_class *class;
	بنية task_struct *p;

	rq->dl_server = عدم;

	for_each_active_class(class) {
		p = class->pick_task(rq, rf);
		إذا (p)
			إرجاع p;
	}

	BUG(); /* The idle class should always have a runnable task. */
}

خارجي فراغ task_vruntime_update(بنية rq *rq, بنية task_struct *p, bool in_fi);

ساكن فراغ queue_core_balance(بنية rq *rq);

ساكن بنية task_struct *
pick_next_task(بنية rq *rq, بنية rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	بنية task_struct *next, *p, *max;
	ثابت بنية cpumask *smt_mask;
	bool fi_before = false;
	bool core_clock_updated = (rq == rq->core);
	unsigned long cookie;
	صحيح i, cpu, occ = 0;
	بنية rq *rq_i;
	bool need_sync = false;

	إذا (!sched_core_enabled(rq))
		إرجاع __pick_next_task(rq, rf);

	cpu = cpu_of(rq);

	/* Stopper task is switching into idle, no need core-wide selection. */
	إذا (cpu_is_offline(cpu)) {
		/*
		 * Reset core_pick so that we don't enter the fastpath when
		 * coming online. core_pick would already be migrated to
		 * another cpu during offline.
		 */
		rq->core_pick = عدم;
		rq->core_dl_server = عدم;
		إرجاع __pick_next_task(rq, rf);
	}

	rq->core->core_pick_in_flight++;

	/*
	 * If there were no {en,de}queues since we picked (IOW, the task
	 * pointers are all still valid), and we haven't scheduled the last
	 * pick yet, do so now.
	 *
	 * rq->core_pick can be NULL if no selection was made for a CPU because
	 * it was either offline or went offline during a sibling's core-wide
	 * selection. In this case, do a core-wide selection.
	 */
	إذا (rq->core->core_pick_seq == rq->core->core_task_seq &&
	    rq->core_pick) {
		next = rq->core_pick;
		rq->dl_server = rq->core_dl_server;
		rq->core_pick = عدم;
		rq->core_dl_server = عدم;
		اذهب out_set_next;
	}

	prev_balance(rq, rf);

	smt_mask = cpu_smt_mask(cpu);

restart:
	need_sync |= !!rq->core->core_cookie;

	/* reset state */
	rq->core->core_cookie = 0UL;
	إذا (rq->core->core_forceidle_تعداد) {
		إذا (!core_clock_updated) {
			update_rq_clock(rq->core);
			core_clock_updated = true;
		}
		sched_core_account_forceidle(rq);
		/* reset after accounting force idle */
		rq->core->core_forceidle_start = 0;
		rq->core->core_forceidle_تعداد = 0;
		rq->core->core_forceidle_occupation = 0;
		need_sync = true;
		fi_before = true;
	}

	/*
	 * core->core_task_seq, core->core_pick_seq
	 *
	 * @task_seq guards the task state ({en,de}queues)
	 * @pick_seq is the @task_seq we did a selection on
	 *
	 * Once a core-wide selection is committed, a non-NULL core_pick denotes
	 * a pick which still needs to be consumed on this CPU.
	 *
	 * However, preemptions can cause multiple picks on the same task set.
	 * 'Fix' this by also increasing @task_seq for every pick.
	 */
	rq->core->core_task_seq++;

	/*
	 * Optimize for common case where this CPU has no cookies
	 * and there are no cookied tasks running on siblings.
	 */
	إذا (!need_sync) {
		next = pick_task(rq, rf);
		إذا (unlikely(next == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			إذا (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			اذهب restart;
		}

		إذا (!next->core_cookie) {
			rq->core_pick = عدم;
			rq->core_dl_server = عدم;
			/*
			 * For robustness, update the min_vruntime_fi for
			 * unconstrained picks as well.
			 */
			WARN_ON_ONCE(fi_before);
			task_vruntime_update(rq, next, false);
			اذهب out_set_next;
		}
	}

	/*
	 * For each thread: do the regular task pick and find the max prio task
	 * amongst them.
	 *
	 * Tie-break prio towards the current CPU
	 */
	max = عدم;
	for_each_cpu_wrap(i, smt_mask, cpu) {
		rq_i = cpu_rq(i);

		/*
		 * Current cpu always has its clock updated on entrance to
		 * pick_next_task(). If the current cpu is not the core,
		 * the core may also have been updated above.
		 */
		إذا (i != cpu && (rq_i != rq->core || !core_clock_updated))
			update_rq_clock(rq_i);

		p = pick_task(rq_i, rf);
		إذا (unlikely(p == RETRY_TASK)) {
			/* rq lock may have been dropped, clocks invalidated */
			core_clock_updated = false;
			إذا (!(rq->clock_update_flags & RQCF_UPDATED))
				update_rq_clock(rq);
			اذهب restart;
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = rq_i->dl_server;

		إذا (!max || prio_less(max, p, fi_before))
			max = p;
	}

	cookie = rq->core->core_cookie = max->core_cookie;

	/*
	 * For each thread: try and find a runnable task that matches @max or
	 * force idle.
	 */
	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick;

		إذا (!cookie_equals(p, cookie)) {
			p = عدم;
			إذا (cookie)
				p = sched_core_find(rq_i, cookie);
			إذا (!p)
				p = idle_sched_class.pick_task(rq_i, rf);
		}

		rq_i->core_pick = p;
		rq_i->core_dl_server = عدم;

		إذا (p == rq_i->idle) {
			إذا (rq_i->nr_running) {
				rq->core->core_forceidle_تعداد++;
				إذا (!fi_before)
					rq->core->core_forceidle_seq++;
			}
		} وإلا {
			occ++;
		}
	}

	إذا (schedstat_enabled() && rq->core->core_forceidle_تعداد) {
		rq->core->core_forceidle_start = rq_clock(rq->core);
		rq->core->core_forceidle_occupation = occ;
	}

	rq->core->core_pick_seq = rq->core->core_task_seq;
	next = rq->core_pick;

	/* Something should have been selected for current CPU */
	WARN_ON_ONCE(!next);

	/*
	 * Reschedule siblings
	 *
	 * NOTE: L1TF -- at this point we're no longer running the old task and
	 * sending an IPI (below) ensures the sibling will no longer be running
	 * their task. This ensures there is no inter-sibling overlap between
	 * non-matching user state.
	 */
	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);

		/*
		 * An online sibling might have gone offline before a task
		 * could be picked for it, or it might be offline but later
		 * happen to come online, but its too late and nothing was
		 * picked for it.  That's Ok - it will pick tasks for itself,
		 * so ignore it.
		 */
		إذا (!rq_i->core_pick)
			استمر;

		/*
		 * Update for new !FI->FI transitions, or if continuing to be in !FI:
		 * fi_before     fi      update?
		 *  0            0       1
		 *  0            1       1
		 *  1            0       1
		 *  1            1       0
		 */
		إذا (!(fi_before && rq->core->core_forceidle_تعداد))
			task_vruntime_update(rq_i, rq_i->core_pick, !!rq->core->core_forceidle_تعداد);

		rq_i->core_pick->core_occupation = occ;

		إذا (i == cpu) {
			rq_i->core_pick = عدم;
			rq_i->core_dl_server = عدم;
			استمر;
		}

		/* Did we break L1TF mitigation requirements? */
		WARN_ON_ONCE(!cookie_match(next, rq_i->core_pick));

		إذا (rq_i->curr == rq_i->core_pick) {
			rq_i->core_pick = عدم;
			rq_i->core_dl_server = عدم;
			استمر;
		}

		resched_curr(rq_i);
	}

out_set_next:
	rq->core->core_pick_in_flight--;
	put_prev_set_next_task(rq, rq->donor, next);
	إذا (rq->core->core_forceidle_تعداد && next == rq->idle)
		queue_core_balance(rq);

	إرجاع next;
}

ساكن bool try_steal_cookie(صحيح this, صحيح that)
{
	بنية rq *dst = cpu_rq(this), *src = cpu_rq(that);
	بنية task_struct *p;
	unsigned long cookie;
	bool success = false;

	guard(irq)();
	guard(double_rq_lock)(dst, src);

	cookie = dst->core->core_cookie;
	إذا (!cookie)
		إرجاع false;

	إذا (dst->curr != dst->idle)
		إرجاع false;

	p = sched_core_find(src, cookie);
	إذا (!p)
		إرجاع false;

	افعل {
		إذا (p == src->core_pick || p == src->curr || p == src->donor)
			اذهب next;

		إذا (task_is_blocked(p))
			اذهب next;

		إذا (!is_cpu_allowed(p, this))
			اذهب next;

		إذا (p->core_occupation > dst->idle->core_occupation)
			اذهب next;
		/*
		 * sched_core_find() and sched_core_next() will ensure
		 * that task @p is not throttled now, we also need to
		 * check whether the runqueue of the destination CPU is
		 * being throttled.
		 */
		إذا (sched_task_is_throttled(p, this))
			اذهب next;

		move_queued_task_locked(src, dst, p);
		resched_curr(dst);

		success = true;
		اكسر;

next:
		p = sched_core_next(p, cookie);
	} بينما (p);

	إرجاع success;
}

ساكن bool steal_cookie_task(صحيح cpu, بنية sched_domain *sd)
{
	صحيح i;

	for_each_cpu_wrap(i, sched_domain_span(sd), cpu + 1) {
		إذا (i == cpu)
			استمر;

		إذا (need_resched())
			اكسر;

		إذا (try_steal_cookie(cpu, i))
			إرجاع true;
	}

	إرجاع false;
}

ساكن فراغ sched_core_balance(بنية rq *rq)
	__must_hold(__rq_lockp(rq))
{
	بنية sched_domain *sd;
	صحيح cpu = cpu_of(rq);

	guard(preempt)();
	guard(rcu)();

	raw_spin_rq_unlock_irq(rq);
	for_each_domain(cpu, sd) {
		إذا (need_resched())
			اكسر;

		إذا (steal_cookie_task(cpu, sd))
			اكسر;
	}
	raw_spin_rq_lock_irq(rq);
}

ساكن DEFINE_PER_CPU(بنية balance_callback, core_balance_head);

ساكن فراغ queue_core_balance(بنية rq *rq)
{
	إذا (!sched_core_enabled(rq))
		إرجاع;

	إذا (!rq->core->core_cookie)
		إرجاع;

	إذا (!rq->nr_running) /* not forced idle */
		إرجاع;

	queue_balance_callback(rq, &per_cpu(core_balance_head, rq->cpu), sched_core_balance);
}

DEFINE_LOCK_GUARD_1(core_lock, صحيح,
		    sched_core_lock(*_T->lock, &_T->flags),
		    sched_core_unlock(*_T->lock, &_T->flags),
		    unsigned long flags)

ساكن فراغ sched_core_cpu_starting(unsigned صحيح cpu)
{
	ثابت بنية cpumask *smt_mask = cpu_smt_mask(cpu);
	بنية rq *rq = cpu_rq(cpu), *core_rq = عدم;
	صحيح t;

	guard(core_lock)(&cpu);

	WARN_ON_ONCE(rq->core != rq);

	/* if we're the first, we'll be our own leader */
	إذا (cpumask_weight(smt_mask) == 1)
		إرجاع;

	/* find the leader */
	for_each_cpu(t, smt_mask) {
		إذا (t == cpu)
			استمر;
		rq = cpu_rq(t);
		إذا (rq->core == rq) {
			core_rq = rq;
			اكسر;
		}
	}

	إذا (WARN_ON_ONCE(!core_rq)) /* whoopsie */
		إرجاع;

	/* install and validate core_rq */
	for_each_cpu(t, smt_mask) {
		rq = cpu_rq(t);

		إذا (t == cpu)
			rq->core = core_rq;

		WARN_ON_ONCE(rq->core != core_rq);
	}
}

ساكن فراغ sched_core_cpu_deactivate(unsigned صحيح cpu)
{
	ثابت بنية cpumask *smt_mask = cpu_smt_mask(cpu);
	بنية rq *rq = cpu_rq(cpu), *core_rq = عدم;
	صحيح t;

	guard(core_lock)(&cpu);

	/* if we're the last man standing, nothing to do */
	إذا (cpumask_weight(smt_mask) == 1) {
		WARN_ON_ONCE(rq->core != rq);
		إرجاع;
	}

	/* if we're not the leader, nothing to do */
	إذا (rq->core != rq)
		إرجاع;

	/* find a new leader */
	for_each_cpu(t, smt_mask) {
		إذا (t == cpu)
			استمر;
		core_rq = cpu_rq(t);
		اكسر;
	}

	إذا (WARN_ON_ONCE(!core_rq)) /* impossible */
		إرجاع;

	/* copy the shared state to the new leader */
	core_rq->core_task_seq             = rq->core_task_seq;
	core_rq->core_pick_seq             = rq->core_pick_seq;
	core_rq->core_cookie               = rq->core_cookie;
	core_rq->core_forceidle_تعداد      = rq->core_forceidle_تعداد;
	core_rq->core_forceidle_seq        = rq->core_forceidle_seq;
	core_rq->core_forceidle_occupation = rq->core_forceidle_occupation;

	/*
	 * A stale leftover would bias the count forever if this CPU later
	 * returns as its own leader. Move, don't copy.
	 */
	core_rq->core_pick_in_flight       = rq->core_pick_in_flight;
	rq->core_pick_in_flight            = 0;

	/*
	 * Accounting edge for forced idle is handled in pick_next_task().
	 * Don't need another one here, since the hotplug thread shouldn't
	 * have a cookie.
	 */
	core_rq->core_forceidle_start = 0;

	/* install new leader */
	for_each_cpu(t, smt_mask) {
		rq = cpu_rq(t);
		rq->core = core_rq;
	}
}

ساكن مضمن فراغ sched_core_cpu_dying(unsigned صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

	إذا (rq->core != rq)
		rq->core = rq;
}

#وإلا /* !CONFIG_SCHED_CORE: */

ساكن مضمن فراغ sched_core_cpu_starting(unsigned صحيح cpu) {}
ساكن مضمن فراغ sched_core_cpu_deactivate(unsigned صحيح cpu) {}
ساكن مضمن فراغ sched_core_cpu_dying(unsigned صحيح cpu) {}

ساكن بنية task_struct *
pick_next_task(بنية rq *rq, بنية rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	إرجاع __pick_next_task(rq, rf);
}

#endif /* !CONFIG_SCHED_CORE */

/*
 * Constants for the sched_mode argument of __schedule().
 *
 * The mode argument allows RT enabled kernels to differentiate a
 * preemption from blocking on an 'sleeping' spin/rwlock.
 */
#define SM_IDLE			(-1)
#define SM_NONE			0
#define SM_PREEMPT		1
#define SM_RTLOCK_WAIT		2

/*
 * Helper function for __schedule()
 *
 * Tries to deactivate the task, unless the should_block arg
 * is false or if a signal is pending. In the case a signal
 * is pending, marks the task's __state as RUNNING (and clear
 * blocked_on).
 */
ساكن bool try_to_block_task(بنية rq *rq, بنية task_struct *p,
			      unsigned long *task_state_p, bool should_block)
{
	unsigned long task_state = *task_state_p;

	WARN_ON_ONCE(p->is_blocked);

	إذا (signal_pending_state(task_state, p)) {
		WRITE_ONCE(p->__state, TASK_RUNNING);
		*task_state_p = TASK_RUNNING;
		clear_task_blocked_on(p, عدم);

		إرجاع false;
	}

	p->is_blocked = 1;

	/*
	 * We check should_block after signal_pending because we
	 * will want to wake the task in that case. But if
	 * should_block is false, its likely due to the task being
	 * blocked on a mutex, and we want to keep it on the runqueue
	 * to be selectable for proxy-execution.
	 */
	إذا (!should_block)
		إرجاع false;

	block_task(rq, p, task_state);
	إرجاع true;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ساكن مضمن فراغ proxy_set_task_cpu(بنية task_struct *p, صحيح cpu)
{
	unsigned صحيح wake_cpu;

	/*
	 * Since we are enqueuing a blocked task on a cpu it may
	 * not be able to run on, preserve wake_cpu when we
	 * __set_task_cpu so we can return the task to where it
	 * was previously runnable.
	 */
	wake_cpu = p->wake_cpu;
	__set_task_cpu(p, cpu);
	p->wake_cpu = wake_cpu;
}

ساكن مضمن بنية task_struct *proxy_resched_idle(بنية rq *rq)
{
	put_prev_set_next_task(rq, rq->donor, rq->idle);
	rq->next_class = &idle_sched_class;
	rq_set_donor(rq, rq->idle);
	set_tsk_need_resched(rq->idle);
	إرجاع rq->idle;
}

ساكن فراغ proxy_deactivate(بنية rq *rq, بنية task_struct *donor)
{
	unsigned long state = READ_ONCE(donor->__state);

	WARN_ON_ONCE(state == TASK_RUNNING);
	WARN_ON_ONCE(donor->blocked_on);
	/*
	 * Because we got donor from pick_next_task(), it is *crucial*
	 * that we call proxy_resched_idle() before we deactivate it.
	 * As once we deactivate donor, donor->on_rq is set to zero,
	 * which allows ttwu() to immediately try to wake the task on
	 * another rq. So we cannot use *any* references to donor
	 * after that point. So things like cfs_rq->curr or rq->donor
	 * need to be changed from next *before* we deactivate.
	 */
	proxy_resched_idle(rq);
	block_task(rq, donor, state);
}

/*
 * Remove a retained proxy donor before changing its scheduler ownership.
 * The caller holds p->pi_lock, so p cannot wake and migrate if block_task()
 * drops it from the runqueue. If DELAY_DEQUEUE keeps a blocked fair task
 * queued, switching_from_fair() completes the dequeue in the immediately
 * following sched_change_begin().
 */
فراغ sched_proxy_block_task(بنية rq *rq, بنية task_struct *p)
{
	unsigned long state = READ_ONCE(p->__state);

	lockdep_assert_held(&p->pi_lock);
	lockdep_assert_rq_held(rq);

	إذا (!p->is_blocked || !task_on_rq_queued(p))
		إرجاع;
	إذا (WARN_ON_ONCE(state == TASK_RUNNING))
		إرجاع;

	إذا (task_current_donor(rq, p))
		proxy_reset_donor(rq);

	إذا (!p->se.sched_delayed)
		block_task(rq, p, state);

	WARN_ON_ONCE(task_on_rq_queued(p) && !p->se.sched_delayed);
}

ساكن مضمن فراغ proxy_release_rq_lock(بنية rq *rq, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	/*
	 * The class scheduler may have queued a balance callback
	 * from pick_next_task() called earlier.
	 *
	 * So here we have to zap callbacks before unlocking the rq
	 * as another CPU may jump in and call sched_balance_rq
	 * which can trip the warning in rq_pin_lock() if we
	 * leave callbacks set.
	 *
	 * After we later reaquire the rq lock, we will force __schedule()
	 * to pick_again, so the callbacks will get re-established.
	 */
	zap_balance_callbacks(rq);
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

ساكن مضمن فراغ proxy_reacquire_rq_lock(بنية rq *rq, بنية rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_repin_lock(rq, rf);
	update_rq_clock(rq);
}

/*
 * If the blocked-on relationship crosses CPUs, migrate @p to the
 * owner's CPU.
 *
 * This is because we must respect the CPU affinity of execution
 * contexts (owner) but we can ignore affinity for scheduling
 * contexts (@p). So we have to move scheduling contexts towards
 * potential execution contexts.
 *
 * Note: The owner can disappear, but simply migrate to @target_cpu
 * and leave that CPU to sort things out.
 */
ساكن فراغ proxy_migrate_task(بنية rq *rq, بنية rq_flags *rf,
			       بنية task_struct *p, صحيح target_cpu)
	__must_hold(__rq_lockp(rq))
{
	بنية rq *target_rq = cpu_rq(target_cpu);
	LIST_HEAD(migrate_list);

	lockdep_assert_rq_held(rq);
	/*
	 * Since we are migrating a blocked donor, it could be rq->donor,
	 * and we want to make sure there aren't any references from this
	 * rq to it before we drop the lock. This avoids another cpu
	 * jumping in and grabbing the rq lock and referencing rq->donor
	 * or cfs_rq->curr, etc after we have migrated it to another cpu,
	 * and before we pick_again in __schedule.
	 *
	 * So call proxy_resched_idle() to drop the rq->donor references
	 * before we release the lock.
	 */
	proxy_resched_idle(rq);
	لكل (; p; p = p->blocked_donor) {
		WARN_ON(p == rq->curr);
		deactivate_task(rq, p, DEQUEUE_NOCLOCK);
		proxy_set_task_cpu(p, target_cpu);
		/*
		 * We can re-use se.group_node to migrate the thing,
		 * because @p is deactivated (won't be balanced) and
		 * we hold the rq_lock.
		 */
		list_add(&p->se.group_عقدة, &migrate_list);
	}
	proxy_release_rq_lock(rq, rf);

	__attach_tasks(target_rq, &migrate_list);

	proxy_reacquire_rq_lock(rq, rf);
}

/*
 * Find runnable lock owner to proxy for mutex blocked donor
 *
 * Follow the blocked-on relation:
 *
 *                ,-> task
 *                |     | blocked-on
 *                |     v
 *  blocked_donor |   mutex
 *                |     | owner
 *                |     v
 *                `-- task
 *
 * and set the blocked_donor relation, this latter is used by the mutex
 * code to find which (blocked) task to hand-off to.
 *
 * Lock order:
 *
 *   p->pi_lock
 *     rq->lock
 *       mutex->wait_lock
 *         p->blocked_lock
 *
 * Returns the task that is going to be used as execution context (the one
 * that is actually going to be run on cpu_of(rq)).
 */
ساكن بنية task_struct *
find_proxy_task(بنية rq *rq, بنية task_struct *donor, بنية rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	بنية task_struct *owner = عدم;
	bool curr_in_chain = false;
	صحيح this_cpu = cpu_of(rq);
	بنية task_struct *p;
	صحيح owner_cpu;

	/* Follow blocked_on chain. */
	لكل (p = donor; p->is_blocked; p = owner) {
		/* if its PROXY_WAKING, do return migration or run if current */
		بنية mutex *mutex = p->blocked_on;
		إذا (!mutex) {
			clear_task_blocked_on(p, mutex);
			إذا (task_current(rq, p)) {
				p->is_blocked = 0;
				إرجاع p;
			}
			اذهب deactivate;
		}

		/*
		 * By taking mutex->wait_lock we hold off concurrent mutex_unlock()
		 * and ensure @owner sticks around.
		 */
		guard(raw_spinlock)(&mutex->wait_lock);
		guard(raw_spinlock)(&p->blocked_lock);

		/* Check again that p is blocked with blocked_lock held */
		إذا (mutex != __get_task_blocked_on(p)) {
			/*
			 * Something changed in the blocked_on chain and
			 * we don't know if only at this level. So, let's
			 * just bail out completely and let __schedule()
			 * figure things out (pick_again loop).
			 */
			إرجاع عدم;
		}

		إذا (task_current(rq, p))
			curr_in_chain = true;

		owner = __mutex_owner(mutex);
		إذا (!owner) {
			/*
			 * If there is no owner, either clear blocked_on
			 * and return p (if it is current and safe to
			 * just run on this rq), or return-migrate the task.
			 */
			__clear_task_blocked_on(p, عدم);
			إذا (task_current(rq, p)) {
				p->is_blocked = 0;
				إرجاع p;
			}
			اذهب deactivate;
		}

		إذا (!READ_ONCE(owner->on_rq) || owner->se.sched_delayed) {
			/* XXX Don't handle blocked owners/delayed dequeue yet */
			إذا (curr_in_chain)
				اذهب resched_idle;
			__clear_task_blocked_on(p, عدم);
			اذهب deactivate;
		}

		owner_cpu = task_cpu(owner);
		إذا (owner_cpu != this_cpu) {
			/*
			 * @owner can disappear, simply migrate to @owner_cpu
			 * and leave that CPU to sort things out.
			 */
			إذا (curr_in_chain)
				اذهب resched_idle;
			اذهب migrate_task;
		}

		إذا (task_on_rq_migrating(owner)) {
			/*
			 * One of the chain of mutex owners is currently migrating to this
			 * CPU, but has not yet been enqueued because we are holding the
			 * rq lock. As a simple solution, just schedule rq->idle to give
			 * the migration a chance to complete. Much like the migrate_task
			 * case we should end up back in find_proxy_task(), this time
			 * hopefully with all relevant tasks already enqueued.
			 */
			اذهب resched_idle;
		}

		/*
		 * Its possible to race where after we check owner->on_rq
		 * but before we check (owner_cpu != this_cpu) that the
		 * task on another cpu was migrated back to this cpu. In
		 * that case it could slip by our  checks. So double check
		 * we are still on this cpu and not migrating. If we get
		 * inconsistent results, try again.
		 */
		إذا (!task_on_rq_queued(owner) || task_cpu(owner) != this_cpu)
			إرجاع عدم;

		إذا (owner == p) {
			/*
			 * It's possible we interleave with mutex_unlock like:
			 *
			 *				lock(&rq->lock);
			 *				  find_proxy_task()
			 * mutex_unlock()
			 *   lock(&wait_lock);
			 *   donor(owner) = current->blocked_donor;
			 *   unlock(&wait_lock);
			 *
			 *   wake_up_q();
			 *     ...
			 *       ttwu_runnable()
			 *         __task_rq_lock()
			 *				  lock(&wait_lock);
			 *				  owner == p
			 *
			 * Which leaves us to finish the ttwu_runnable() and make it go.
			 *
			 * So schedule rq->idle so that ttwu_runnable() can get the rq
			 * lock and mark owner as running.
			 */
			اذهب resched_idle;
		}
		/*
		 * OK, now we're absolutely sure @owner is on this
		 * rq, therefore holding @rq->lock is sufficient to
		 * guarantee its existence, as per ttwu_remote().
		 */
		owner->blocked_donor = p;
	}
	WARN_ON_ONCE(owner && !owner->on_rq);

	إذا (owner && !sched_cpu_cookie_match(rq, owner)) {
		إذا (curr_in_chain)
			إرجاع proxy_resched_idle(rq);
		p = donor; /* Deactivate the donor, not the runnable owner */
		clear_task_blocked_on(p, عدم);
		اذهب deactivate;
	}
	إرجاع owner;

resched_idle:
	إرجاع proxy_resched_idle(rq);
deactivate:
	proxy_deactivate(rq, p);
	إرجاع عدم;
migrate_task:
	proxy_migrate_task(rq, rf, p, owner_cpu);
	إرجاع عدم;
}
#وإلا /* SCHED_PROXY_EXEC */
ساكن بنية task_struct *
find_proxy_task(بنية rq *rq, بنية task_struct *donor, بنية rq_flags *rf)
{
	WARN_ONCE(1, "This should never be called in the !SCHED_PROXY_EXEC case\n");
	إرجاع donor;
}
#endif /* SCHED_PROXY_EXEC */

/*
 * __schedule() is the main scheduler function.
 *
 * The main means of driving the scheduler and thus entering this function are:
 *
 *   1. Explicit blocking: mutex, semaphore, waitqueue, etc.
 *
 *   2. TIF_NEED_RESCHED flag is checked on interrupt and userspace return
 *      paths. For example, see arch/x86/entry_64.S.
 *
 *      To drive preemption between tasks, the scheduler sets the flag in timer
 *      interrupt handler sched_tick().
 *
 *   3. Wakeups don't really cause entry into schedule(). They add a
 *      task to the run-queue and that's it.
 *
 *      Now, if the new task added to the run-queue preempts the current
 *      task, then the wakeup sets TIF_NEED_RESCHED and schedule() gets
 *      called on the nearest possible occasion:
 *
 *       - If the kernel is preemptible (CONFIG_PREEMPTION=y):
 *
 *         - in syscall or exception context, at the next outmost
 *           preempt_enable(). (this might be as soon as the wake_up()'s
 *           spin_unlock()!)
 *
 *         - in IRQ context, return from interrupt-handler to
 *           preemptible context
 *
 *       - If the kernel is not preemptible (CONFIG_PREEMPTION is not set)
 *         then at the next:
 *
 *          - cond_resched() call
 *          - explicit schedule() call
 *          - return from syscall or exception to user-space
 *          - return from interrupt-handler to user-space
 *
 * WARNING: must be called with preemption disabled!
 */
ساكن فراغ __sched notrace __schedule(صحيح sched_mode)
{
	بنية task_struct *prev, *next;
	/*
	 * On PREEMPT_RT kernel, SM_RTLOCK_WAIT is noted
	 * as a preemption by schedule_debug() and RCU.
	 */
	bool preempt = sched_mode > SM_NONE;
	bool is_switch = false;
	unsigned long *switch_تعداد;
	unsigned long prev_state;
	بنية rq_flags rf;
	بنية rq *rq;
	صحيح cpu;

	/* Trace preemptions consistently with task switches */
	trace_sched_entry_tp(sched_mode == SM_PREEMPT);

	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	prev = rq->curr;

	schedule_debug(prev, preempt);

	klp_sched_try_switch(prev);

	local_irq_disable();
	rcu_note_context_switch(preempt);
	migrate_disable_switch(rq, prev);

	/*
	 * Make sure that signal_pending_state()->signal_pending() below
	 * can't be reordered with __set_current_state(TASK_INTERRUPTIBLE)
	 * done by the caller to avoid the race with signal_wake_up():
	 *
	 * __set_current_state(@state)		signal_wake_up()
	 * schedule()				  set_tsk_thread_flag(p, TIF_SIGPENDING)
	 *					  wake_up_state(p, state)
	 *   LOCK rq->lock			    LOCK p->pi_state
	 *   smp_mb__after_spinlock()		    smp_mb__after_spinlock()
	 *     if (signal_pending_state())	    if (p->state & @state)
	 *
	 * Also, the membarrier system call requires a full memory barrier
	 * after coming from user-space, before storing to rq->curr; this
	 * barrier matches a full barrier in the proximity of the membarrier
	 * system call exit.
	 */
	rq_lock(rq, &rf);
	smp_mb__after_spinlock();

	hrtick_schedule_enter(rq);

	/* Promote REQ to ACT */
	rq->clock_update_flags <<= 1;
	update_rq_clock(rq);
	rq->clock_update_flags = RQCF_UPDATED;

	switch_تعداد = &prev->nivcsw;

	/* Task state changes only considers SM_PREEMPT as preemption */
	preempt = sched_mode == SM_PREEMPT;

	/*
	 * We must load prev->state once (task_struct::state is volatile), such
	 * that we form a control dependency vs deactivate_task() below.
	 */
	prev_state = READ_ONCE(prev->__state);
	إذا (sched_mode == SM_IDLE) {
		/* SCX must consult the BPF scheduler to tell if rq is empty */
		إذا (!rq->nr_running && !scx_enabled()) {
			next = prev;
			rq->next_class = &idle_sched_class;
			اذهب picked;
		}
	} وإلا إذا (!preempt && prev_state) {
		/*
		 * Keep mutex-blocked tasks on the runqueue for proxy execution
		 * only when their scheduling class allows it. Without proxy
		 * execution, task_is_blocked() always returns false.
		 */
		try_to_block_task(rq, prev, &prev_state,
				  !task_is_blocked(prev) || !scx_allow_proxy_exec(prev));
		switch_تعداد = &prev->nvcsw;
	}

pick_again:
	assert_balance_callbacks_empty(rq);
	next = pick_next_task(rq, &rf);
	rq->next_class = next->sched_class;
	إذا (sched_proxy_exec()) {
		بنية task_struct *prev_donor = rq->donor;

		rq_set_donor(rq, next);
		next->blocked_donor = عدم;
		إذا (unlikely(next->is_blocked)) {
			next = find_proxy_task(rq, next, &rf);
			إذا (!next) {
				zap_balance_callbacks(rq);
				اذهب pick_again;
			}
			إذا (next == rq->idle) {
				zap_balance_callbacks(rq);
				scx_proxy_reenqueue_retry(rq, next);
				اذهب keep_resched;
			}
		}
		إذا (rq->donor == prev_donor && prev != next) {
			بنية task_struct *donor = rq->donor;
			/*
			 * When transitioning like:
			 *
			 *         prev         next
			 * donor:    B            B
			 * curr:     A          B or C
			 *
			 * then put_prev_set_next_task() will not have done
			 * anything, since B == B. However, A might have
			 * missed a RT/DL balance opportunity due to being
			 * on_cpu.
			 */
			donor->sched_class->put_prev_task(rq, donor, donor);
			donor->sched_class->set_next_task(rq, donor, SNT_PICK);
		}
		scx_proxy_donor_start(rq);
		scx_proxy_reenqueue_retry(rq, next);
	} وإلا {
		rq_set_donor(rq, next);
	}

picked:
	clear_tsk_need_resched(prev);
	clear_preempt_need_resched();
keep_resched:
	rq->last_seen_need_resched_ns = 0;

	is_switch = prev != next;
	إذا (likely(is_switch)) {
		rq->nr_switches++;
		/*
		 * RCU users of rcu_dereference(rq->curr) may not see
		 * changes to task_struct made by pick_next_task().
		 */
		RCU_INIT_POINTER(rq->curr, next);

		/*
		 * The membarrier system call requires each architecture
		 * to have a full memory barrier after updating
		 * rq->curr, before returning to user-space.
		 *
		 * Here are the schemes providing that barrier on the
		 * various architectures:
		 * - mm ? switch_mm() : mmdrop() for x86, s390, sparc, PowerPC,
		 *   RISC-V.  switch_mm() relies on membarrier_arch_switch_mm()
		 *   on PowerPC and on RISC-V.
		 * - finish_lock_switch() for weakly-ordered
		 *   architectures where spin_unlock is a full barrier,
		 * - switch_to() for arm64 (weakly-ordered, spin_unlock
		 *   is a RELEASE barrier),
		 *
		 * The barrier matches a full barrier in the proximity of
		 * the membarrier system call entry.
		 *
		 * On RISC-V, this barrier pairing is also needed for the
		 * SYNC_CORE command when switching between processes, cf.
		 * the inline comments in membarrier_arch_switch_mm().
		 */
		++*switch_تعداد;

		psi_account_irqtime(rq, prev, next);
		psi_sched_switch(prev, next, !task_on_rq_queued(prev) ||
					     prev->se.sched_delayed);

		trace_sched_switch(preempt, prev, next, prev_state);

		/* Also unlocks the rq: */
		rq = context_switch(rq, prev, next, &rf);
	} وإلا {
		rq_unpin_lock(rq, &rf);
		__balance_callbacks(rq, عدم);
		hrtick_schedule_exit(rq);
		raw_spin_rq_unlock_irq(rq);
	}
	trace_sched_exit_tp(is_switch);
}

فراغ __noreturn do_task_dead(فراغ)
{
	/* Causes final put_task_struct in finish_task_switch(): */
	set_special_state(TASK_DEAD);

	/* Tell freezer to ignore us: */
	current->flags |= PF_NOFREEZE;

	__schedule(SM_NONE);
	BUG();

	/* Avoid "noreturn function does return" - but don't continue if BUG() is a NOP: */
	لكل (;;)
		cpu_relax();
}

ساكن مضمن فراغ sched_submit_work(بنية task_struct *tsk)
{
	ساكن DEFINE_WAIT_OVERRIDE_MAP(sched_map, LD_WAIT_CONFIG);
	unsigned صحيح task_flags;

	/*
	 * Establish LD_WAIT_CONFIG context to ensure none of the code called
	 * will use a blocking primitive -- which would lead to recursion.
	 */
	lock_map_acquire_try(&sched_map);

	task_flags = tsk->flags;
	/*
	 * If a worker goes to sleep, notify and ask workqueue whether it
	 * wants to wake up a task to maintain concurrency.
	 */
	إذا (task_flags & PF_WQ_WORKER)
		wq_worker_sleeping(tsk);
	وإلا إذا (task_flags & PF_IO_WORKER)
		io_wq_worker_sleeping(tsk);

	/*
	 * spinlock and rwlock must not flush block requests.  This will
	 * deadlock if the callback attempts to acquire a lock which is
	 * already acquired.
	 */
	WARN_ON_ONCE(current->__state & TASK_RTLOCK_WAIT);

	/*
	 * If we are going to sleep and we have plugged IO queued,
	 * make sure to submit it to avoid deadlocks.
	 */
	blk_flush_plug(tsk->plug, true);

	lock_map_release(&sched_map);
}

ساكن فراغ sched_update_worker(بنية task_struct *tsk)
{
	إذا (tsk->flags & (PF_WQ_WORKER | PF_IO_WORKER)) {
		إذا (tsk->flags & PF_WQ_WORKER)
			wq_worker_running(tsk);
		وإلا
			io_wq_worker_running(tsk);
	}
}

ساكن __always_inline فراغ __schedule_loop(صحيح sched_mode)
{
	افعل {
		preempt_disable();
		__schedule(sched_mode);
		sched_preempt_enable_no_resched();
	} بينما (need_resched());
}

asmlinkage __visible فراغ __sched schedule(فراغ)
{
	بنية task_struct *tsk = current;

#ifdef CONFIG_RT_MUTEXES
	lockdep_assert(!tsk->sched_rt_mutex);
#endif

	إذا (!task_is_running(tsk))
		sched_submit_work(tsk);
	__schedule_loop(SM_NONE);
	sched_update_worker(tsk);
}
EXPORT_رمز(schedule);

/*
 * synchronize_rcu_tasks() makes sure that no task is stuck in preempted
 * state (have scheduled out non-voluntarily) by making sure that all
 * tasks have either left the run queue or have gone into user space.
 * As idle tasks do not do either, they must not ever be preempted
 * (schedule out non-voluntarily).
 *
 * schedule_idle() is similar to schedule_preempt_disable() except that it
 * never enables preemption because it does not call sched_submit_work().
 */
فراغ __sched schedule_idle(فراغ)
{
	/*
	 * As this skips calling sched_submit_work(), which the idle task does
	 * regardless because that function is a NOP when the task is in a
	 * TASK_RUNNING state, make sure this isn't used someplace that the
	 * current task can be in any other state. Note, idle is always in the
	 * TASK_RUNNING state.
	 */
	WARN_ON_ONCE(current->__state);
	افعل {
		__schedule(SM_IDLE);
	} بينما (need_resched());
}

#إذا defined(CONFIG_CONTEXT_TRACKING_USER) && !defined(CONFIG_HAVE_CONTEXT_TRACKING_USER_OFFSTACK)
asmlinkage __visible فراغ __sched schedule_user(فراغ)
{
	/*
	 * If we come here after a random call to set_need_resched(),
	 * or we have been woken up remotely but the IPI has not yet arrived,
	 * we haven't yet exited the RCU idle mode. Do it here manually until
	 * we find a better solution.
	 *
	 * NB: There are buggy callers of this function.  Ideally we
	 * should warn if prev_state != CT_STATE_USER, but that will trigger
	 * too frequently to make sense yet.
	 */
	تعداد ctx_state prev_state = exception_enter();
	schedule();
	exception_exit(prev_state);
}
#endif

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
فراغ __sched schedule_preempt_disabled(فراغ)
{
	sched_preempt_enable_no_resched();
	schedule();
	preempt_disable();
}

#ifdef CONFIG_PREEMPT_RT
فراغ __sched notrace schedule_rtlock(فراغ)
{
	__schedule_loop(SM_RTLOCK_WAIT);
}
NOKPROBE_رمز(schedule_rtlock);
#endif

ساكن فراغ __sched notrace preempt_schedule_common(فراغ)
{
	افعل {
		/*
		 * Because the function tracer can trace preempt_count_sub()
		 * and it also uses preempt_enable/disable_notrace(), if
		 * NEED_RESCHED is set, the preempt_enable_notrace() called
		 * by the function tracer will call this function again and
		 * cause infinite recursion.
		 *
		 * Preemption must be disabled here before the function
		 * tracer can trace. Break up preempt_disable() into two
		 * calls. One to disable preemption without fear of being
		 * traced. The other to still record the preemption latency,
		 * which can also be traced by the function tracer.
		 */
		preempt_disable_notrace();
		preempt_latency_start(1);
		__schedule(SM_PREEMPT);
		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		/*
		 * Check again in case we missed a preemption opportunity
		 * between schedule and now.
		 */
	} بينما (need_resched());
}

#ifdef CONFIG_PREEMPTION
/*
 * This is the entry point to schedule() from in-kernel preemption
 * off of preempt_enable.
 */
asmlinkage __visible فراغ __sched notrace preempt_schedule(فراغ)
{
	/*
	 * If there is a non-zero preempt_count or interrupts are disabled,
	 * we do not want to preempt the current task. Just return..
	 */
	إذا (likely(!preemptible()))
		إرجاع;
	preempt_schedule_common();
}
NOKPROBE_رمز(preempt_schedule);
EXPORT_رمز(preempt_schedule);

/**
 * preempt_schedule_notrace - preempt_schedule called by tracing
 *
 * The tracing infrastructure uses preempt_enable_notrace to prevent
 * recursion and tracing preempt enabling caused by the tracing
 * infrastructure itself. But as tracing can happen in areas coming
 * from userspace or just about to enter userspace, a preempt enable
 * can occur before user_exit() is called. This will cause the scheduler
 * to be called when the system is still in usermode.
 *
 * To prevent this, the preempt_enable_notrace will use this function
 * instead of preempt_schedule() to exit user context if needed before
 * calling the scheduler.
 */
asmlinkage __visible فراغ __sched notrace preempt_schedule_notrace(فراغ)
{
	تعداد ctx_state prev_ctx;

	إذا (likely(!preemptible()))
		إرجاع;

	افعل {
		/*
		 * Because the function tracer can trace preempt_count_sub()
		 * and it also uses preempt_enable/disable_notrace(), if
		 * NEED_RESCHED is set, the preempt_enable_notrace() called
		 * by the function tracer will call this function again and
		 * cause infinite recursion.
		 *
		 * Preemption must be disabled here before the function
		 * tracer can trace. Break up preempt_disable() into two
		 * calls. One to disable preemption without fear of being
		 * traced. The other to still record the preemption latency,
		 * which can also be traced by the function tracer.
		 */
		preempt_disable_notrace();
		preempt_latency_start(1);
		/*
		 * Needs preempt disabled in case user_exit() is traced
		 * and the tracer calls preempt_enable_notrace() causing
		 * an infinite recursion.
		 */
		prev_ctx = exception_enter();
		__schedule(SM_PREEMPT);
		exception_exit(prev_ctx);

		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();
	} بينما (need_resched());
}
EXPORT_رمز_GPL(preempt_schedule_notrace);

#endif /* CONFIG_PREEMPTION */

/*
 * This is the entry point to schedule() from kernel preemption
 * off of IRQ context.
 * Note, that this is called and return with IRQs disabled. This will
 * protect us against recursive calling from IRQ contexts.
 */
asmlinkage __visible فراغ __sched preempt_schedule_irq(فراغ)
{
	تعداد ctx_state prev_state;

	/* Catch callers which need to be fixed */
	BUG_ON(preempt_تعداد() || !irqs_disabled());

	prev_state = exception_enter();

	افعل {
		preempt_disable();
		local_irq_enable();
		__schedule(SM_PREEMPT);
		local_irq_disable();
		sched_preempt_enable_no_resched();
	} بينما (need_resched());

	exception_exit(prev_state);
}

صحيح default_wake_function(wait_queue_entry_t *curr, unsigned mode, صحيح wake_flags,
			  فراغ *مفتاح)
{
	WARN_ON_ONCE(wake_flags & ~(WF_SYNC|WF_CURRENT_CPU));
	إرجاع try_to_wake_up(curr->خاص, mode, wake_flags);
}
EXPORT_رمز(default_wake_function);

ثابت بنية sched_class *__setscheduler_class(صحيح policy, صحيح prio)
{
	إذا (dl_prio(prio))
		إرجاع &dl_sched_class;

	إذا (rt_prio(prio))
		إرجاع &rt_sched_class;

#ifdef CONFIG_SCHED_CLASS_EXT
	إذا (task_should_scx(policy))
		إرجاع &ext_sched_class;
#endif

	إرجاع &fair_sched_class;
}

#ifdef CONFIG_RT_MUTEXES

/*
 * Would be more useful with typeof()/auto_type but they don't mix with
 * bit-fields. Since it's a local thing, use int. Keep the generic sounding
 * name such that if someone were to implement this function we get to compare
 * notes.
 */
#define fetch_and_set(x, v) ({ صحيح _x = (x); (x) = (v); _x; })

فراغ rt_mutex_pre_schedule(فراغ)
{
	lockdep_assert(!fetch_and_set(current->sched_rt_mutex, 1));
	sched_submit_work(current);
}

/*
 * Used within the futex syscall context, skips sched_submit_work() because none
 * its work will be done. Asserts ensure that it is indeed the case.
 */
فراغ rt_mutex_futex_pre_schedule(فراغ)
{
	lockdep_assert(!(current->flags & (PF_WQ_WORKER | PF_IO_WORKER)));
	lockdep_assert(!current->plug);
	lockdep_assert(!fetch_and_set(current->sched_rt_mutex, 1));
}

فراغ rt_mutex_schedule(فراغ)
{
	lockdep_assert(current->sched_rt_mutex);
	__schedule_loop(SM_NONE);
}

فراغ rt_mutex_post_schedule(فراغ)
{
	sched_update_worker(current);
	lockdep_assert(fetch_and_set(current->sched_rt_mutex, 0));
}

فراغ rt_mutex_futex_post_schedule(فراغ)
{
	lockdep_assert(fetch_and_set(current->sched_rt_mutex, 0));
}

/*
 * rt_mutex_setprio - set the current priority of a task
 * @p: task to boost
 * @pi_task: donor task
 *
 * This function changes the 'effective' priority of a task. It does
 * not touch ->normal_prio like __setscheduler().
 *
 * Used by the rt_mutex code to implement priority inheritance
 * logic. Call site only calls if the priority of the task changed.
 */
فراغ rt_mutex_setprio(بنية task_struct *p, بنية task_struct *pi_task)
{
	صحيح prio, oldprio, queue_flag =
		DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	ثابت بنية sched_class *prev_class, *next_class;
	بنية rq_flags rf;
	بنية rq *rq;

	/* XXX used to be waiter->prio, not waiter->task->prio */
	prio = __rt_effective_prio(pi_task, p->normal_prio);

	/*
	 * If nothing changed; bail early.
	 */
	إذا (p->pi_قمة_task == pi_task && prio == p->prio && !dl_prio(prio))
		إرجاع;

	rq = __task_rq_lock(p, &rf);
	update_rq_clock(rq);
	/*
	 * Set under pi_lock && rq->lock, such that the value can be used under
	 * either lock.
	 *
	 * Note that there is loads of tricky to make this pointer cache work
	 * right. rt_mutex_slowunlock()+rt_mutex_postunlock() work together to
	 * ensure a task is de-boosted (pi_task is set to NULL) before the
	 * task is allowed to run again (and can exit). This ensures the pointer
	 * points to a blocked task -- which guarantees the task is present.
	 */
	p->pi_قمة_task = pi_task;

	/*
	 * For FIFO/RR we only need to set prio, if that matches we're done.
	 */
	إذا (prio == p->prio && !dl_prio(prio))
		اذهب out_unlock;

	/*
	 * Idle task boosting is a no-no in general. There is one
	 * exception, when PREEMPT_RT and NOHZ is active:
	 *
	 * The idle task calls get_next_timer_interrupt() and holds
	 * the timer wheel base->lock on the CPU and another CPU wants
	 * to access the timer (probably to cancel it). We can safely
	 * ignore the boosting request, as the idle CPU runs this code
	 * with interrupts disabled and will complete the lock
	 * protected section without being interrupted. So there is no
	 * real need to boost.
	 */
	إذا (unlikely(p == rq->idle)) {
		WARN_ON(p != rq->curr);
		WARN_ON(p->pi_blocked_on);
		اذهب out_unlock;
	}

	trace_sched_pi_setprio(p, pi_task);
	oldprio = p->prio;

	إذا (oldprio == prio && !dl_prio(prio))
		queue_flag &= ~DEQUEUE_MOVE;

	prev_class = p->sched_class;
	next_class = __setscheduler_class(p->policy, prio);

	إذا (prev_class != next_class)
		queue_flag |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flag) {
		/*
		 * Boosting condition are:
		 * 1. -rt task is running and holds mutex A
		 *      --> -dl task blocks on mutex A
		 *
		 * 2. -dl task is running and holds mutex A
		 *      --> -dl task blocks on mutex A and could preempt the
		 *          running task
		 */
		إذا (dl_prio(prio)) {
			إذا (!dl_prio(p->normal_prio) ||
			    (pi_task && dl_prio(pi_task->prio) &&
			     dl_entity_preempt(&pi_task->dl, &p->dl))) {
				p->dl.pi_se = pi_task->dl.pi_se;
				scope->flags |= ENQUEUE_REPLENISH;
			} وإلا {
				p->dl.pi_se = &p->dl;
			}
		} وإلا إذا (rt_prio(prio)) {
			إذا (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			إذا (oldprio < prio)
				scope->flags |= ENQUEUE_HEAD;
		} وإلا {
			إذا (dl_prio(oldprio))
				p->dl.pi_se = &p->dl;
			إذا (rt_prio(oldprio))
				p->rt.timeout = 0;
		}

		p->sched_class = next_class;
		p->prio = prio;
	}
out_unlock:
	/* Caller holds task_struct::pi_lock, IRQs are still disabled */

	__balance_callbacks(rq, &rf);
	__task_rq_unlock(rq, p, &rf);
}
#endif /* CONFIG_RT_MUTEXES */

#إذا !defined(CONFIG_PREEMPTION)
صحيح __sched __cond_resched(فراغ)
{
	إذا (should_resched(0) && !irqs_disabled()) {
		preempt_schedule_common();
		إرجاع 1;
	}
	/*
	 * In PREEMPT_RCU kernels, ->rcu_read_lock_nesting tells the tick
	 * whether the current CPU is in an RCU read-side critical section,
	 * so the tick can report quiescent states even for CPUs looping
	 * in kernel context.  In contrast, in non-preemptible kernels,
	 * RCU readers leave no in-memory hints, which means that CPU-bound
	 * processes executing in kernel context might never report an
	 * RCU quiescent state.  Therefore, the following code causes
	 * cond_resched() to report a quiescent state, but only when RCU
	 * is in urgent need of one.
	 * A third case, preemptible, but non-PREEMPT_RCU provides for
	 * urgently needed quiescent states via rcu_flavor_sched_clock_irq().
	 */
#ifndef CONFIG_PREEMPT_RCU
	rcu_all_qs();
#endif
	إرجاع 0;
}
EXPORT_رمز(__cond_resched);
#endif

/*
 * __cond_resched_lock() - if a reschedule is pending, drop the given lock,
 * call schedule, and on return reacquire the lock.
 *
 * This works OK both with and without CONFIG_PREEMPTION. We do strange low-level
 * operations here to prevent schedule() from being called twice (once via
 * spin_unlock(), once by hand).
 */
صحيح __cond_resched_lock(spinlock_t *lock)
{
	صحيح resched = should_resched(PREEMPT_LOCK_OFFSET);
	صحيح ret = 0;

	lockdep_assert_held(lock);

	إذا (spin_needbreak(lock) || resched) {
		spin_unlock(lock);
		إذا (!_cond_resched())
			cpu_relax();
		ret = 1;
		spin_lock(lock);
	}
	إرجاع ret;
}
EXPORT_رمز(__cond_resched_lock);

صحيح __cond_resched_rwlock_read(rwlock_t *lock)
{
	صحيح resched = should_resched(PREEMPT_LOCK_OFFSET);
	صحيح ret = 0;

	lockdep_assert_held_read(lock);

	إذا (rwlock_needbreak(lock) || resched) {
		read_unlock(lock);
		إذا (!_cond_resched())
			cpu_relax();
		ret = 1;
		read_lock(lock);
	}
	إرجاع ret;
}
EXPORT_رمز(__cond_resched_rwlock_read);

صحيح __cond_resched_rwlock_write(rwlock_t *lock)
{
	صحيح resched = should_resched(PREEMPT_LOCK_OFFSET);
	صحيح ret = 0;

	lockdep_assert_held_write(lock);

	إذا (rwlock_needbreak(lock) || resched) {
		write_unlock(lock);
		إذا (!_cond_resched())
			cpu_relax();
		ret = 1;
		write_lock(lock);
	}
	إرجاع ret;
}
EXPORT_رمز(__cond_resched_rwlock_write);

#ifdef CONFIG_PREEMPT_DYNAMIC

# ifdef CONFIG_GENERIC_IRQ_ENTRY
#  include <linux/irq-entry-common.h>
# endif

/*
 * NONE:
 *   (unselectable)
 *
 * VOLUNTARY:
 *   (unselectable)
 *
 * FULL:
 *   dynamic_preempt_lazy       <- false
 *
 * LAZY:
 *   dynamic_preempt_lazy       <- true
 */

تعداد {
	preempt_dynamic_undefined = -1,
	preempt_dynamic_full,
	preempt_dynamic_lazy,
};

صحيح preempt_dynamic_mode = preempt_dynamic_undefined;

صحيح sched_dynamic_mode(ثابت حرف *str)
{
	إذا (!strcmp(str, "full"))
		إرجاع preempt_dynamic_full;

	إذا (!strcmp(str, "lazy"))
		إرجاع preempt_dynamic_lazy;

	إرجاع -EINVAL;
}

# define preempt_dynamic_مفتاح_enable(f)	static_مفتاح_enable(&sk_dynamic_##f.مفتاح)
# define preempt_dynamic_مفتاح_disable(f)	static_مفتاح_disable(&sk_dynamic_##f.مفتاح)

ساكن DEFINE_MUTEX(sched_dynamic_mutex);

ساكن فراغ __sched_dynamic_update(صحيح mode)
{
	بدل (mode) {
	حالة preempt_dynamic_full:
		preempt_dynamic_مفتاح_disable(preempt_lazy);
		إذا (mode != preempt_dynamic_mode)
			pr_info("Dynamic Preempt: full\n");
		اكسر;

	حالة preempt_dynamic_lazy:
		preempt_dynamic_مفتاح_enable(preempt_lazy);
		إذا (mode != preempt_dynamic_mode)
			pr_info("Dynamic Preempt: lazy\n");
		اكسر;
	}

	WRITE_ONCE(preempt_dynamic_mode, mode);
}

فراغ sched_dynamic_update(صحيح mode)
{
	mutex_lock(&sched_dynamic_mutex);
	__sched_dynamic_update(mode);
	mutex_unlock(&sched_dynamic_mutex);
}

ساكن صحيح __init setup_preempt_mode(حرف *str)
{
	صحيح mode = sched_dynamic_mode(str);
	إذا (mode < 0) {
		pr_warn("Dynamic Preempt: unsupported mode: %s\n", str);
		إرجاع 0;
	}

	sched_dynamic_update(mode);
	إرجاع 1;
}
__setup("preempt=", setup_preempt_mode);

ساكن فراغ __init preempt_dynamic_init(فراغ)
{
	إذا (preempt_dynamic_mode == preempt_dynamic_undefined) {
		إذا (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			sched_dynamic_update(preempt_dynamic_lazy);
		} وإلا {
			/* Default static call setting, nothing to do */
			WARN_ON_ONCE(!IS_ENABLED(CONFIG_PREEMPT));
			preempt_dynamic_mode = preempt_dynamic_full;
			pr_info("Dynamic Preempt: full\n");
		}
	}
}

# define PREEMPT_MODEL_ACCESSOR(mode)					\
	bool preempt_model_##mode(فراغ)					\
	{								\
		صحيح mode = READ_ONCE(preempt_dynamic_mode);		\
		WARN_ON_ONCE(mode == preempt_dynamic_undefined);	\
		إرجاع mode == preempt_dynamic_##mode;			\
	}								\
	EXPORT_رمز_GPL(preempt_model_##mode)

PREEMPT_MODEL_ACCESSOR(full);
PREEMPT_MODEL_ACCESSOR(lazy);

#وإلا /* !CONFIG_PREEMPT_DYNAMIC: */

#define preempt_dynamic_mode -1

ساكن مضمن فراغ preempt_dynamic_init(فراغ) { }

#endif /* CONFIG_PREEMPT_DYNAMIC */

ثابت حرف *preempt_modes[] = {
	"full", "lazy", عدم,
};

ثابت حرف *preempt_model_str(فراغ)
{
	bool brace = IS_ENABLED(CONFIG_PREEMPT_RT) &&
		(IS_ENABLED(CONFIG_PREEMPT_DYNAMIC) ||
		 IS_ENABLED(CONFIG_PREEMPT_LAZY));
	ساكن حرف buf[128];

	إذا (IS_ENABLED(CONFIG_PREEMPT_BUILD)) {
		بنية seq_buf s;

		seq_buf_init(&s, buf, حجم(buf));
		seq_buf_puts(&s, "PREEMPT");

		إذا (IS_ENABLED(CONFIG_PREEMPT_RT))
			seq_buf_printf(&s, "%sRT%s",
				       brace ? "_{" : "_",
				       brace ? "," : "");

		إذا (IS_ENABLED(CONFIG_PREEMPT_DYNAMIC)) {
			seq_buf_printf(&s, "(%s)%s",
				       preempt_dynamic_mode >= 0 ?
				       preempt_modes[preempt_dynamic_mode] : "undef",
				       brace ? "}" : "");
			إرجاع seq_buf_str(&s);
		}

		إذا (IS_ENABLED(CONFIG_PREEMPT_LAZY)) {
			seq_buf_printf(&s, "LAZY%s",
				       brace ? "}" : "");
			إرجاع seq_buf_str(&s);
		}

		إرجاع seq_buf_str(&s);
	}

	إذا (IS_ENABLED(CONFIG_PREEMPT_VOLUNTARY_BUILD))
		إرجاع "VOLUNTARY";

	إرجاع "NONE";
}

صحيح io_schedule_prepare(فراغ)
{
	صحيح old_iowait = current->in_iowait;

	current->in_iowait = 1;
	blk_flush_plug(current->plug, true);
	إرجاع old_iowait;
}

فراغ io_schedule_finish(صحيح token)
{
	current->in_iowait = token;
}

/*
 * This task is about to go to sleep on IO. Increment rq->nr_iowait so
 * that process accounting knows that this is a task in IO wait state.
 */
long __sched io_schedule_timeout(long timeout)
{
	صحيح token;
	long ret;

	token = io_schedule_prepare();
	ret = schedule_timeout(timeout);
	io_schedule_finish(token);

	إرجاع ret;
}
EXPORT_رمز(io_schedule_timeout);

فراغ __sched io_schedule(فراغ)
{
	صحيح token;

	token = io_schedule_prepare();
	schedule();
	io_schedule_finish(token);
}
EXPORT_رمز(io_schedule);

فراغ sched_show_task(بنية task_struct *p)
{
	unsigned long حرر;
	صحيح ppid;

	إذا (!try_get_task_مكدس(p))
		إرجاع;

	pr_info("task:%-15.15s state:%c", p->comm, task_state_to_char(p));

	إذا (task_is_running(p))
		pr_cont("  running task    ");
	حرر = مكدس_not_used(p);
	ppid = 0;
	rcu_read_lock();
	إذا (pid_alive(p))
		ppid = task_pid_nr(rcu_dereference(p->real_parent));
	rcu_read_unlock();
	pr_cont(" stack:%-5lu pid:%-5d tgid:%-5d ppid:%-6d task_flags:0x%04x flags:0x%08lx\n",
		حرر, task_pid_nr(p), task_tgid_nr(p),
		ppid, p->flags, read_task_thread_flags(p));

	print_worker_info(KERN_INFO, p);
	print_stop_info(KERN_INFO, p);
	print_scx_info(KERN_INFO, p);
	show_مكدس(p, عدم, KERN_INFO);
	put_task_مكدس(p);
}
EXPORT_رمز_GPL(sched_show_task);

ساكن مضمن bool
state_filter_match(unsigned long state_filter, بنية task_struct *p)
{
	unsigned صحيح state = READ_ONCE(p->__state);

	/* no filter, everything matches */
	إذا (!state_filter)
		إرجاع true;

	/* filter, but doesn't match */
	إذا (!(state & state_filter))
		إرجاع false;

	/*
	 * When looking for TASK_UNINTERRUPTIBLE skip TASK_IDLE (allows
	 * TASK_KILLABLE).
	 */
	إذا (state_filter == TASK_UNINTERRUPTIBLE && (state & TASK_NOLOAD))
		إرجاع false;

	إرجاع true;
}


فراغ show_state_filter(unsigned صحيح state_filter)
{
	بنية task_struct *g, *p;

	rcu_read_lock();
	for_each_process_thread(g, p) {
		/*
		 * reset the NMI-timeout, listing all files on a slow
		 * console might take a lot of time:
		 * Also, reset softlockup watchdogs on all CPUs, because
		 * another CPU might be blocked waiting for us to process
		 * an IPI.
		 */
		touch_nmi_watchdog();
		touch_all_softlockup_watchdogs();
		إذا (state_filter_match(state_filter, p))
			sched_show_task(p);
	}

	إذا (!state_filter)
		sysrq_sched_debug_show();

	rcu_read_unlock();
	/*
	 * Only show locks if all tasks are dumped:
	 */
	إذا (!state_filter)
		debug_show_all_locks();
}

/**
 * init_idle - set up an idle thread for a given CPU
 * @idle: task in question
 * @cpu: CPU the idle task belongs to
 *
 * NOTE: this function does not set the idle thread's NEED_RESCHED
 * flag, to make booting more robust.
 */
فراغ __init init_idle(بنية task_struct *idle, صحيح cpu)
{
	بنية affinity_context ac = (بنية affinity_context) {
		.new_mask  = cpumask_of(cpu),
		.flags     = 0,
	};
	بنية rq *rq = cpu_rq(cpu);
	unsigned long flags;

	raw_spin_lock_irqsave(&idle->pi_lock, flags);
	raw_spin_rq_lock(rq);

	idle->__state = TASK_RUNNING;
	idle->se.exec_start = sched_clock();
	/*
	 * PF_KTHREAD should already be set at this point; regardless, make it
	 * look like a proper per-CPU kthread.
	 */
	idle->flags |= PF_KTHREAD | PF_NO_SETAFFINITY;
	kthread_set_per_cpu(idle, cpu);

	/*
	 * No validation and serialization required at boot time and for
	 * setting up the idle tasks of not yet online CPUs.
	 */
	set_cpus_allowed_common(idle, &ac);
	/*
	 * We're having a chicken and egg problem, even though we are
	 * holding rq->lock, the CPU isn't yet set to this CPU so the
	 * lockdep check in task_group() will fail.
	 *
	 * Similar case to sched_fork(). / Alternatively we could
	 * use task_rq_lock() here and obtain the other rq->lock.
	 *
	 * Silence PROVE_RCU
	 */
	rcu_read_lock();
	__set_task_cpu(idle, cpu);
	rcu_read_unlock();

	rq->idle = idle;
	rq_set_donor(rq, idle);
	rcu_assign_pointer(rq->curr, idle);
	idle->on_rq = TASK_ON_RQ_QUEUED;
	idle->on_cpu = 1;
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irqrestore(&idle->pi_lock, flags);

	/* Set the preempt count _outside_ the spinlocks! */
	init_idle_preempt_تعداد(idle, cpu);

	/*
	 * The idle tasks have their own, simple scheduling class:
	 */
	idle->sched_class = &idle_sched_class;
	ftrace_graph_init_idle_task(idle, cpu);
	vtime_init_idle(idle, cpu);
	sprintf(idle->comm, "%s/%d", INIT_TASK_COMM, cpu);
}

صحيح cpuset_cpumask_can_shrink(ثابت بنية cpumask *cur,
			      ثابت بنية cpumask *trial)
{
	صحيح ret = 1;

	إذا (cpumask_empty(cur))
		إرجاع ret;

	ret = dl_cpuset_cpumask_can_shrink(cur, trial);

	إرجاع ret;
}

صحيح task_can_attach(بنية task_struct *p)
{
	صحيح ret = 0;

	/*
	 * Kthreads which disallow setaffinity shouldn't be moved
	 * to a new cpuset; we don't want to change their CPU
	 * affinity and isolating such threads by their set of
	 * allowed nodes is unnecessary.  Thus, cpusets are not
	 * applicable for such threads.  This prevents checking for
	 * success of set_cpus_allowed_ptr() on all attached tasks
	 * before cpus_mask may be changed.
	 */
	إذا (p->flags & PF_NO_SETAFFINITY)
		ret = -EINVAL;

	إرجاع ret;
}

bool sched_smp_initialized __read_mostly;

#ifdef CONFIG_NUMA_BALANCING
/* Migrate current task p to target_cpu */
صحيح migrate_task_to(بنية task_struct *p, صحيح target_cpu)
{
	بنية migration_arg arg = { p, target_cpu };
	صحيح curr_cpu = task_cpu(p);

	إذا (curr_cpu == target_cpu)
		إرجاع 0;

	إذا (!cpumask_test_cpu(target_cpu, p->cpus_ptr))
		إرجاع -EINVAL;

	/* TODO: This is not properly updating schedstats */

	trace_sched_move_numa(p, curr_cpu, target_cpu);
	إرجاع stop_one_cpu(curr_cpu, migration_cpu_stop, &arg);
}

/*
 * Requeue a task on a given node and accurately track the number of NUMA
 * tasks on the runqueues
 */
فراغ sched_setnuma(بنية task_struct *p, صحيح nid)
{
	guard(task_rq_lock)(p);
	scoped_guard (sched_change, p, DEQUEUE_SAVE)
		p->numa_preferred_nid = nid;
}
#endif /* CONFIG_NUMA_BALANCING */

#ifdef CONFIG_HOTPLUG_CPU
/*
 * Invoked on the outgoing CPU in context of the CPU hotplug thread
 * after ensuring that there are no user space tasks left on the CPU.
 *
 * If there is a lazy mm in use on the hotplug thread, drop it and
 * switch to init_mm.
 *
 * The reference count on init_mm is dropped in finish_cpu().
 */
ساكن فراغ sched_force_init_mm(فراغ)
{
	بنية mm_struct *mm = current->active_mm;

	إذا (mm != &init_mm) {
		mmgrab_lazy_tlb(&init_mm);
		local_irq_disable();
		current->active_mm = &init_mm;
		switch_mm_irqs_off(mm, &init_mm, current);
		local_irq_enable();
		finish_arch_post_lock_switch();
		mmdrop_lazy_tlb(mm);
	}

	/* finish_cpu(), as ran on the BP, will clean up the active_mm state */
}

ساكن صحيح __balance_push_cpu_stop(فراغ *arg)
{
	بنية task_struct *p = arg;
	بنية rq *rq = this_rq();
	بنية rq_flags rf;
	صحيح cpu;

	scoped_guard (raw_spinlock_irq, &p->pi_lock) {
		/*
		 * We may change the underlying rq, but the locks held will
		 * appropriately be "transferred" when switching.
		 */
		context_unsafe_alias(rq);

		cpu = select_fallback_rq(rq->cpu, p);

		rq_lock(rq, &rf);
		update_rq_clock(rq);
		إذا (task_rq(p) == rq && task_on_rq_queued(p))
			rq = __migrate_task(rq, &rf, p, cpu);
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);

	إرجاع 0;
}

ساكن DEFINE_PER_CPU(بنية cpu_stop_work, ادفع_work);

/*
 * Ensure we only run per-cpu kthreads once the CPU goes !active.
 *
 * This is enabled below SCHED_AP_ACTIVE; when !cpu_active(), but only
 * effective when the hotplug motion is down.
 */
ساكن فراغ balance_ادفع(بنية rq *rq)
	__must_hold(__rq_lockp(rq))
{
	بنية task_struct *ادفع_task = rq->curr;

	lockdep_assert_rq_held(rq);

	/*
	 * Ensure the thing is persistent until balance_push_set(.on = false);
	 */
	rq->balance_callback = &balance_ادفع_callback;

	/*
	 * Only active while going offline and when invoked on the outgoing
	 * CPU.
	 */
	إذا (!cpu_dying(rq->cpu) || rq != this_rq())
		إرجاع;

	/*
	 * Both the cpu-hotplug and stop task are in this case and are
	 * required to complete the hotplug process.
	 */
	إذا (kthread_is_per_cpu(ادفع_task) ||
	    is_migration_disabled(ادفع_task)) {

		/*
		 * If this is the idle task on the outgoing CPU try to wake
		 * up the hotplug control thread which might wait for the
		 * last task to vanish. The rcuwait_active() check is
		 * accurate here because the waiter is pinned on this CPU
		 * and can't obviously be running in parallel.
		 *
		 * On RT kernels this also has to check whether there are
		 * pinned and scheduled out tasks on the runqueue. They
		 * need to leave the migrate disabled section first.
		 */
		إذا (!rq->nr_running && !rq_has_pinned_tasks(rq) &&
		    rcuwait_active(&rq->hotplug_wait)) {
			raw_spin_rq_unlock(rq);
			rcuwait_wake_up(&rq->hotplug_wait);
			raw_spin_rq_lock(rq);
		}
		إرجاع;
	}

	get_task_struct(ادفع_task);
	/*
	 * Temporarily drop rq->lock such that we can wake-up the stop task.
	 * Both preemption and IRQs are still disabled.
	 */
	preempt_disable();
	raw_spin_rq_unlock(rq);
	stop_one_cpu_nowait(rq->cpu, __balance_push_cpu_stop, ادفع_task,
			    this_cpu_ptr(&ادفع_work));
	preempt_enable();
	/*
	 * At this point need_resched() is true and we'll take the loop in
	 * schedule(). The next pick is obviously going to be the stop task
	 * which kthread_is_per_cpu() and will push this task away.
	 */
	raw_spin_rq_lock(rq);
}

ساكن فراغ balance_ادفع_set(صحيح cpu, bool on)
{
	بنية rq *rq = cpu_rq(cpu);
	بنية rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	إذا (on) {
		WARN_ON_ONCE(rq->balance_callback);
		rq->balance_callback = &balance_ادفع_callback;
	} وإلا إذا (rq->balance_callback == &balance_ادفع_callback) {
		rq->balance_callback = عدم;
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * Invoked from a CPUs hotplug control thread after the CPU has been marked
 * inactive. All tasks which are not per CPU kernel threads are either
 * pushed off this CPU now via balance_push() or placed on a different CPU
 * during wakeup. Wait until the CPU is quiescent.
 */
ساكن فراغ balance_hotplug_wait(فراغ)
{
	بنية rq *rq = this_rq();

	rcuwait_wait_event(&rq->hotplug_wait,
			   rq->nr_running == 1 && !rq_has_pinned_tasks(rq),
			   TASK_UNINTERRUPTIBLE);
}

#وإلا /* !CONFIG_HOTPLUG_CPU: */

ساكن مضمن فراغ balance_ادفع(بنية rq *rq)
{
}

ساكن مضمن فراغ balance_ادفع_set(صحيح cpu, bool on)
{
}

ساكن مضمن فراغ balance_hotplug_wait(فراغ)
{
}

#endif /* !CONFIG_HOTPLUG_CPU */

فراغ set_rq_online(بنية rq *rq)
{
	إذا (!rq->online) {
		ثابت بنية sched_class *class;

		cpumask_set_cpu(rq->cpu, rq->rd->online);
		rq->online = 1;

		for_each_class(class) {
			إذا (class->rq_online)
				class->rq_online(rq);
		}
	}
}

فراغ set_rq_offline(بنية rq *rq)
{
	إذا (rq->online) {
		ثابت بنية sched_class *class;

		update_rq_clock(rq);
		for_each_class(class) {
			إذا (class->rq_offline)
				class->rq_offline(rq);
		}

		cpumask_clear_cpu(rq->cpu, rq->rd->online);
		rq->online = 0;
	}
}

ساكن مضمن فراغ sched_set_rq_online(بنية rq *rq, صحيح cpu)
{
	بنية rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	إذا (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_online(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

ساكن مضمن فراغ sched_set_rq_offline(بنية rq *rq, صحيح cpu)
{
	بنية rq_flags rf;

	rq_lock_irqsave(rq, &rf);
	إذا (rq->rd) {
		BUG_ON(!cpumask_test_cpu(cpu, rq->rd->span));
		set_rq_offline(rq);
	}
	rq_unlock_irqrestore(rq, &rf);
}

/*
 * used to mark begin/end of suspend/resume:
 */
ساكن صحيح رقم_cpus_frozen;

/*
 * Update cpusets according to cpu_active mask.  If cpusets are
 * disabled, cpuset_update_active_cpus() becomes a simple wrapper
 * around partition_sched_domains().
 *
 * If we come here as part of a suspend/resume, don't touch cpusets because we
 * want to restore it back to its original state upon resume anyway.
 */
ساكن فراغ cpuset_cpu_active(فراغ)
{
	إذا (cpuhp_tasks_frozen) {
		/*
		 * num_cpus_frozen tracks how many CPUs are involved in suspend
		 * resume sequence. As long as this is not the last online
		 * operation in the resume sequence, just build a single sched
		 * domain, ignoring cpusets.
		 */
		cpuset_reset_sched_domains();
		إذا (--رقم_cpus_frozen)
			إرجاع;
		/*
		 * This is the last CPU online operation. So fall through and
		 * restore the original sched domains by considering the
		 * cpuset configurations.
		 */
		cpuset_force_rebuild();
	}
	cpuset_update_active_cpus();
}

ساكن فراغ cpuset_cpu_inactive(unsigned صحيح cpu)
{
	إذا (!cpuhp_tasks_frozen) {
		cpuset_update_active_cpus();
	} وإلا {
		رقم_cpus_frozen++;
		cpuset_reset_sched_domains();
	}
}

ساكن مضمن فراغ sched_smt_present_inc(صحيح cpu)
{
	إذا (cpumask_weight(cpu_smt_mask(cpu)) == 2)
		static_branch_inc_cpuslocked(&sched_smt_present);
}

ساكن مضمن فراغ sched_smt_present_dec(صحيح cpu)
{
	إذا (cpumask_weight(cpu_smt_mask(cpu)) == 2)
		static_branch_dec_cpuslocked(&sched_smt_present);
}

صحيح sched_cpu_activate(unsigned صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

	/*
	 * Clear the balance_push callback and prepare to schedule
	 * regular tasks.
	 */
	balance_ادفع_set(cpu, false);

	/*
	 * When going up, increment the number of cores with SMT present.
	 */
	sched_smt_present_inc(cpu);
	set_cpu_active(cpu, true);

	إذا (sched_smp_initialized) {
		sched_update_numa(cpu, true);
		sched_domains_numa_masks_set(cpu);
		cpuset_cpu_active();
	}

	scx_rq_activate(rq);

	/*
	 * Put the rq online, if not already. This happens:
	 *
	 * 1) In the early boot process, because we build the real domains
	 *    after all CPUs have been brought up.
	 *
	 * 2) At runtime, if cpuset_cpu_active() fails to rebuild the
	 *    domains.
	 */
	sched_set_rq_online(rq, cpu);

	/* preferred is subset of active and follows its state */
	set_cpu_preferred(cpu, true);

	إرجاع 0;
}

صحيح sched_cpu_deactivate(unsigned صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);
	صحيح ret;

	ret = dl_bw_deactivate(cpu);

	إذا (ret)
		إرجاع ret;

	set_cpu_preferred(cpu, false);

	/*
	 * Remove CPU from nohz.idle_cpus_mask to prevent participating in
	 * load balancing when not active
	 */
	scoped_guard (rcu)
		nohz_balance_exit_idle(rq);

	set_cpu_active(cpu, false);

	/*
	 * From this point forward, this CPU will refuse to run any task that
	 * is not: migrate_disable() or KTHREAD_IS_PER_CPU, and will actively
	 * push those tasks away until this gets cleared, see
	 * sched_cpu_dying().
	 */
	balance_ادفع_set(cpu, true);

	/*
	 * We've cleared cpu_active_mask / set balance_push, wait for all
	 * preempt-disabled and RCU users of this state to go away such that
	 * all new such users will observe it.
	 *
	 * Specifically, we rely on ttwu to no longer target this CPU, see
	 * ttwu_queue_cond() and is_cpu_allowed().
	 *
	 * Do sync before park smpboot threads to take care the RCU boost case.
	 */
	synchronize_rcu();

	sched_domains_free_llc_id(cpu);

	sched_set_rq_offline(rq, cpu);

	scx_rq_deactivate(rq);

	/*
	 * When going down, decrement the number of cores with SMT present.
	 */
	sched_smt_present_dec(cpu);

	sched_core_cpu_deactivate(cpu);

	إذا (!sched_smp_initialized)
		إرجاع 0;

	sched_update_numa(cpu, false);
	cpuset_cpu_inactive(cpu);
	sched_domains_numa_masks_clear(cpu);
	إرجاع 0;
}

ساكن فراغ sched_rq_cpu_starting(unsigned صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);

	rq->calc_load_update = calc_load_update;
	update_max_interval();
}

صحيح sched_cpu_starting(unsigned صحيح cpu)
{
	sched_core_cpu_starting(cpu);
	sched_rq_cpu_starting(cpu);
	sched_tick_start(cpu);
	إرجاع 0;
}

#ifdef CONFIG_HOTPLUG_CPU

/*
 * Invoked immediately before the stopper thread is invoked to bring the
 * CPU down completely. At this point all per CPU kthreads except the
 * hotplug thread (current) and the stopper thread (inactive) have been
 * either parked or have been unbound from the outgoing CPU. Ensure that
 * any of those which might be on the way out are gone.
 *
 * If after this point a bound task is being woken on this CPU then the
 * responsible hotplug callback has failed to do it's job.
 * sched_cpu_dying() will catch it with the appropriate fireworks.
 */
صحيح sched_cpu_wait_empty(unsigned صحيح cpu)
{
	balance_hotplug_wait();
	sched_force_init_mm();
	إرجاع 0;
}

/*
 * Since this CPU is going 'away' for a while, fold any nr_active delta we
 * might have. Called from the CPU stopper task after ensuring that the
 * stopper is the last running task on the CPU, so nr_active count is
 * stable. We need to take the tear-down thread which is calling this into
 * account, so we hand in adjust = 1 to the load calculation.
 *
 * Also see the comment "Global load-average calculations".
 */
ساكن فراغ calc_load_migrate(بنية rq *rq)
{
	long delta = calc_load_fold_active(rq, 1);

	إذا (delta)
		atomic_long_add(delta, &calc_load_tasks);
}

ساكن فراغ dump_rq_tasks(بنية rq *rq, ثابت حرف *loglvl)
{
	بنية task_struct *g, *p;
	صحيح cpu = cpu_of(rq);

	lockdep_assert_rq_held(rq);

	printk("%sCPU%d enqueued tasks (%u total):\n", loglvl, cpu, rq->nr_running);
	for_each_process_thread(g, p) {
		إذا (task_cpu(p) != cpu)
			استمر;

		إذا (!task_on_rq_queued(p))
			استمر;

		printk("%s\tpid: %d, name: %s\n", loglvl, p->pid, p->comm);
	}
}

صحيح sched_cpu_dying(unsigned صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);
	بنية rq_flags rf;

	/* Handle pending wakeups and then migrate everything off */
	sched_tick_stop(cpu);

	rq_lock_irqsave(rq, &rf);
	update_rq_clock(rq);
	إذا (rq->nr_running != 1 || rq_has_pinned_tasks(rq)) {
		WARN(true, "Dying CPU not properly vacated!");
		dump_rq_tasks(rq, KERN_WARNING);
	}
	dl_server_stop(&rq->fair_server);
#ifdef CONFIG_SCHED_CLASS_EXT
	dl_server_stop(&rq->ext_server);
#endif
	rq_unlock_irqrestore(rq, &rf);

	calc_load_migrate(rq);
	update_max_interval();
	hrtick_clear(rq);
	sched_core_cpu_dying(cpu);
	إرجاع 0;
}
#endif /* CONFIG_HOTPLUG_CPU */

فراغ __init sched_init_smp(فراغ)
{
	sched_init_numa(NUMA_NO_عقدة);

	prandom_init_once(&sched_rnd_state);

	/*
	 * There's no userspace yet to cause hotplug operations; hence all the
	 * CPU masks are stable and all blatant races in the below code cannot
	 * happen.
	 */
	sched_domains_mutex_lock();
	sched_init_domains(cpu_active_mask);
	sched_domains_mutex_unlock();

	/* Move init over to a non-isolated CPU */
	إذا (set_cpus_allowed_ptr(current, housekeeping_cpumask(HK_TYPE_DOMAIN)) < 0)
		BUG();
	current->flags &= ~PF_NO_SETAFFINITY;
	sched_init_granularity();

	init_sched_rt_class();
	init_sched_dl_class();

	sched_init_dl_servers();

	sched_smp_initialized = true;
}

ساكن صحيح __init migration_init(فراغ)
{
	sched_cpu_starting(smp_processor_id());
	إرجاع 0;
}
early_initcall(migration_init);

صحيح in_sched_functions(unsigned long addr)
{
	إرجاع in_lock_functions(addr) ||
		(addr >= (unsigned long)__sched_text_start
		&& addr < (unsigned long)__sched_text_end);
}

#ifdef CONFIG_CGROUP_SCHED
/*
 * Default task group.
 * Every task in system belongs to this group at bootup.
 */
بنية task_group جذر_task_group;
LIST_HEAD(task_groups);

/* Cacheline aligned slab cache for task_group */
ساكن بنية kmem_cache *task_group_cache __ro_after_init;
#endif

فراغ __init sched_init(فراغ)
{
	unsigned long __maybe_unused ptr = 0;
	صحيح i;

	/* Make sure the linker didn't screw up */
	BUG_ON(!sched_class_above(&stop_sched_class, &dl_sched_class));
	BUG_ON(!sched_class_above(&dl_sched_class, &rt_sched_class));
	BUG_ON(!sched_class_above(&rt_sched_class, &fair_sched_class));
	BUG_ON(!sched_class_above(&fair_sched_class, &idle_sched_class));
#ifdef CONFIG_SCHED_CLASS_EXT
	BUG_ON(!sched_class_above(&fair_sched_class, &ext_sched_class));
	BUG_ON(!sched_class_above(&ext_sched_class, &idle_sched_class));
#endif

	wait_bit_init();

#ifdef CONFIG_FAIR_GROUP_SCHED
	جذر_task_group.cfs_rq = &runqueues.cfs;

	جذر_task_group.shares = جذر_TASK_GROUP_LOAD;
	init_cfs_bandwidth(&جذر_task_group.cfs_bandwidth, عدم);
#endif /* CONFIG_FAIR_GROUP_SCHED */
#ifdef CONFIG_EXT_GROUP_SCHED
	scx_tg_init(&جذر_task_group);
#endif /* CONFIG_EXT_GROUP_SCHED */
#ifdef CONFIG_RT_GROUP_SCHED
	ptr += 2 * nr_cpu_ids * حجم(فراغ **);
	ptr = (unsigned long)kzalloc(ptr, GFP_NOWAIT);
	جذر_task_group.rt_se = (بنية sched_rt_entity **)ptr;
	ptr += nr_cpu_ids * حجم(فراغ **);

	جذر_task_group.rt_rq = (بنية rt_rq **)ptr;
	ptr += nr_cpu_ids * حجم(فراغ **);

#endif /* CONFIG_RT_GROUP_SCHED */

	init_defrootdomain();

#ifdef CONFIG_RT_GROUP_SCHED
	init_rt_bandwidth(&جذر_task_group.rt_bandwidth,
			global_rt_period(), global_rt_runtime());
#endif /* CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_CGROUP_SCHED
	task_group_cache = KMEM_CACHE(task_group, 0);

	list_add(&جذر_task_group.list, &task_groups);
	INIT_LIST_HEAD(&جذر_task_group.children);
	INIT_LIST_HEAD(&جذر_task_group.siblings);
	autogroup_init(&init_task);
#endif /* CONFIG_CGROUP_SCHED */

	for_each_possible_cpu(i) {
		بنية rq *rq;

		rq = cpu_rq(i);
		raw_spin_lock_init(&rq->__lock);
		rq->nr_running = 0;
		rq->calc_load_active = 0;
		rq->calc_load_update = jiffies + LOAD_FREQ;
		init_cfs_rq(&rq->cfs);
		init_rt_rq(&rq->rt);
		init_dl_rq(&rq->dl);
#ifdef CONFIG_FAIR_GROUP_SCHED
		INIT_LIST_HEAD(&rq->leaf_cfs_rq_list);
		rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
		/*
		 * How much CPU bandwidth does root_task_group get?
		 *
		 * In case of task-groups formed through the cgroup filesystem, it
		 * gets 100% of the CPU resources in the system. This overall
		 * system CPU resource is divided among the tasks of
		 * root_task_group and its child task-groups in a fair manner,
		 * based on each entity's (task or task-group's) weight
		 * (se->load.weight).
		 *
		 * In other words, if root_task_group has 10 tasks of weight
		 * 1024) and two child groups A0 and A1 (of weight 1024 each),
		 * then A0's share of the CPU resource is:
		 *
		 *	A0's bandwidth = 1024 / (10*1024 + 1024 + 1024) = 8.33%
		 *
		 * We achieve this by letting root_task_group's tasks sit
		 * directly in rq->cfs (i.e root_task_group->se[] = NULL).
		 */
		init_tg_cfs_entry(&جذر_task_group, &rq->cfs, عدم, i, عدم);
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * This is required for init cpu because rt.c:__enable_runtime()
		 * starts working after scheduler_running, which is not the case
		 * yet.
		 */
		rq->rt.rt_runtime = global_rt_runtime();
		init_tg_rt_entry(&جذر_task_group, &rq->rt, عدم, i, عدم);
#endif
		rq->next_class = &idle_sched_class;

		rq->sd = عدم;
		rq->rd = عدم;
		rq->cpu_capacity = SCHED_CAPACITY_SCALE;
		rq->balance_callback = &balance_ادفع_callback;
		rq->active_balance = 0;
		rq->next_balance = jiffies;
		rq->ادفع_cpu = 0;
		rq->cpu = i;
		rq->online = 0;
		rq->idle_stamp = 0;
		rq->avg_idle = 2*sysctl_sched_migration_cost;
		rq->max_idle_balance_cost = sysctl_sched_migration_cost;

		INIT_LIST_HEAD(&rq->cfs_tasks);

		rq_attach_جذر(rq, &def_جذر_domain);
#ifdef CONFIG_NO_HZ_COMMON
		rq->last_blocked_load_update_tick = jiffies;
		atomic_set(&rq->nohz_flags, 0);

		INIT_CSD(&rq->nohz_csd, nohz_csd_func, rq);
#endif
#ifdef CONFIG_HOTPLUG_CPU
		rcuwait_init(&rq->hotplug_wait);
#endif
		hrtick_rq_init(rq);
		atomic_set(&rq->nr_iowait, 0);
		fair_server_init(rq);
#ifdef CONFIG_SCHED_CLASS_EXT
		ext_server_init(rq);
#endif

#ifdef CONFIG_SCHED_CORE
		rq->core = rq;
		rq->core_pick = عدم;
		rq->core_dl_server = عدم;
		rq->core_enabled = 0;
		rq->core_شجرة = RB_جذر;
		rq->core_forceidle_تعداد = 0;
		rq->core_forceidle_occupation = 0;
		rq->core_forceidle_start = 0;
		rq->core_pick_in_flight = 0;

		rq->core_cookie = 0UL;
#endif
#ifdef CONFIG_SCHED_CACHE
		raw_spin_lock_init(&rq->cpu_epoch_lock);
		rq->cpu_epoch_next = jiffies;
#endif

		zalloc_cpumask_var_عقدة(&rq->scratch_mask, GFP_KERNEL, cpu_to_عقدة(i));
	}

	set_load_weight(&init_task, false);
	init_task.se.slice = sysctl_sched_base_slice,

	/*
	 * The boot idle thread does lazy MMU switching as well:
	 */
	mmgrab_lazy_tlb(&init_mm);
	enter_lazy_tlb(&init_mm, current);

	/*
	 * The idle task doesn't need the kthread struct to function, but it
	 * is dressed up as a per-CPU kthread and thus needs to play the part
	 * if we want to avoid special-casing it in code that deals with per-CPU
	 * kthreads.
	 */
	WARN_ON(!set_kthread_struct(current));

	/*
	 * Make us the idle thread. Technically, schedule() should not be
	 * called from this thread, however somewhere below it might be,
	 * but because we are the idle thread, we just pick up running again
	 * when this runqueue becomes "idle".
	 */
	__sched_fork(0, current);
	init_idle(current, smp_processor_id());

	calc_load_update = jiffies + LOAD_FREQ;

	idle_thread_set_boot_cpu();

	balance_ادفع_set(smp_processor_id(), false);
	init_sched_fair_class();
	init_sched_ext_class();

	psi_init();

	init_uclamp();

	preempt_dynamic_init();

	scheduler_running = 1;
}

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP

فراغ __might_sleep(ثابت حرف *file, صحيح سطر)
{
	unsigned صحيح state = get_current_state();
	/*
	 * Blocking primitives will set (and therefore destroy) current->state,
	 * since we will exit with TASK_RUNNING make sure we enter with it,
	 * otherwise we will destroy state.
	 */
	WARN_ONCE(state != TASK_RUNNING && current->task_state_change,
			"do not call blocking ops when !TASK_RUNNING; "
			"state=%x set at [<%p>] %pS\n", state,
			(فراغ *)current->task_state_change,
			(فراغ *)current->task_state_change);

	__might_resched(file, سطر, 0);
}
EXPORT_رمز(__might_sleep);

ساكن فراغ print_preempt_disable_ip(صحيح preempt_offset, unsigned long ip)
{
	إذا (!IS_ENABLED(CONFIG_DEBUG_PREEMPT))
		إرجاع;

	إذا (preempt_تعداد() == preempt_offset)
		إرجاع;

	pr_err("Preemption disabled at:");
	print_ip_sym(KERN_ERR, ip);
}

ساكن مضمن bool resched_offsets_ok(unsigned صحيح offsets)
{
	unsigned صحيح nested = preempt_تعداد();

	nested += rcu_preempt_depth() << MIGHT_RESCHED_RCU_SHIFT;

	إرجاع nested == offsets;
}

فراغ __might_resched(ثابت حرف *file, صحيح سطر, unsigned صحيح offsets)
{
	/* Ratelimiting timestamp: */
	ساكن unsigned long prev_jiffy;

	unsigned long preempt_disable_ip;

	/* WARN_ON_ONCE() by default, no rate limit required: */
	rcu_sleep_check();

	إذا ((resched_offsets_ok(offsets) && !irqs_disabled() &&
	     !is_idle_task(current) && !current->non_block_تعداد) ||
	    system_state == SYSTEM_BOOTING || system_state > SYSTEM_RUNNING ||
	    oops_in_progress)
		إرجاع;

	إذا (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		إرجاع;
	prev_jiffy = jiffies;

	/* Save this before calling printk(), since that will clobber it: */
	preempt_disable_ip = get_preempt_disable_ip(current);

	pr_err("BUG: sleeping function called from invalid context at %s:%d\n",
	       file, سطر);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, non_block: %d, pid: %d, name: %s\n",
	       in_atomic(), irqs_disabled(), current->non_block_تعداد,
	       current->pid, current->comm);
	pr_err("preempt_count: %x, expected: %x\n", preempt_تعداد(),
	       offsets & MIGHT_RESCHED_PREEMPT_MASK);

	إذا (IS_ENABLED(CONFIG_PREEMPT_RCU)) {
		pr_err("RCU nest depth: %d, expected: %u\n",
		       rcu_preempt_depth(), offsets >> MIGHT_RESCHED_RCU_SHIFT);
	}

	إذا (task_مكدس_end_corrupted(current))
		pr_emerg("Thread overran stack, or stack corrupted\n");

	debug_show_held_locks(current);
	إذا (irqs_disabled())
		print_irqtrace_events(current);

	print_preempt_disable_ip(offsets & MIGHT_RESCHED_PREEMPT_MASK,
				 preempt_disable_ip);

	dump_مكدس();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_رمز(__might_resched);

فراغ __cant_sleep(ثابت حرف *file, صحيح سطر)
{
	ساكن unsigned long prev_jiffy;

	إذا (irqs_disabled())
		إرجاع;

	إذا (!IS_ENABLED(CONFIG_PREEMPT_تعداد))
		إرجاع;

	إذا (preempt_تعداد())
		إرجاع;

	إذا (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		إرجاع;
	prev_jiffy = jiffies;

	printk(KERN_ERR "BUG: assuming atomic context at %s:%d\n", file, سطر);
	printk(KERN_ERR "in_atomic(): %d, irqs_disabled(): %d, pid: %d, name: %s\n",
			in_atomic(), irqs_disabled(),
			current->pid, current->comm);

	debug_show_held_locks(current);
	dump_مكدس();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_رمز_GPL(__cant_sleep);

# ifdef CONFIG_SMP
فراغ __cant_migrate(ثابت حرف *file, صحيح سطر)
{
	ساكن unsigned long prev_jiffy;

	إذا (irqs_disabled())
		إرجاع;

	إذا (is_migration_disabled(current))
		إرجاع;

	إذا (!IS_ENABLED(CONFIG_PREEMPT_تعداد))
		إرجاع;

	إذا (preempt_تعداد())
		إرجاع;

	إذا (time_before(jiffies, prev_jiffy + HZ) && prev_jiffy)
		إرجاع;
	prev_jiffy = jiffies;

	pr_err("BUG: assuming non migratable context at %s:%d\n", file, سطر);
	pr_err("in_atomic(): %d, irqs_disabled(): %d, migration_disabled() %u pid: %d, name: %s\n",
	       in_atomic(), irqs_disabled(), is_migration_disabled(current),
	       current->pid, current->comm);

	debug_show_held_locks(current);
	dump_مكدس();
	add_taint(TAINT_WARN, LOCKDEP_STILL_OK);
}
EXPORT_رمز_GPL(__cant_migrate);
# endif /* CONFIG_SMP */
#endif /* CONFIG_DEBUG_ATOMIC_SLEEP */

#ifdef CONFIG_MAGIC_SYSRQ
فراغ normalize_rt_tasks(فراغ)
{
	بنية task_struct *g, *p;
	بنية sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
	};

	read_lock(&tasklist_lock);
	for_each_process_thread(g, p) {
		/*
		 * Only normalize user tasks:
		 */
		إذا (p->flags & PF_KTHREAD)
			استمر;

		p->se.exec_start = 0;
		schedstat_set(p->stats.wait_start,  0);
		schedstat_set(p->stats.sleep_start, 0);
		schedstat_set(p->stats.block_start, 0);

		إذا (!rt_or_dl_task(p)) {
			/*
			 * Renice negative nice level userspace
			 * tasks back to 0:
			 */
			إذا (task_nice(p) < 0)
				set_user_nice(p, 0);
			استمر;
		}

		__sched_setscheduler(p, &attr, false, false);
	}
	read_unlock(&tasklist_lock);
}

#endif /* CONFIG_MAGIC_SYSRQ */

#ifdef CONFIG_KGDB_KDB
/*
 * These functions are only useful for KDB.
 *
 * They can only be called when the whole system has been
 * stopped - every CPU needs to be quiescent, and no scheduling
 * activity can take place. Using them for anything else would
 * be a serious bug, and as a result, they aren't even visible
 * under any other configuration.
 */

/**
 * curr_task - return the current task for a given CPU.
 * @cpu: the processor in question.
 *
 * ONLY VALID WHEN THE WHOLE SYSTEM IS STOPPED!
 *
 * Return: The current task for @cpu.
 */
بنية task_struct *curr_task(صحيح cpu)
{
	إرجاع cpu_curr(cpu);
}

#endif /* CONFIG_KGDB_KDB */

#ifdef CONFIG_CGROUP_SCHED
/* task_group_lock serializes the addition/removal of task groups */
ساكن DEFINE_SPINLOCK(task_group_lock);

ساكن مضمن فراغ alloc_uclamp_sched_group(بنية task_group *tg,
					    بنية task_group *parent)
{
#ifdef CONFIG_UCLAMP_TASK_GROUP
	تعداد uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		uclamp_se_set(&tg->uclamp_req[clamp_id],
			      uclamp_none(clamp_id), false);
		tg->uclamp[clamp_id] = parent->uclamp[clamp_id];
	}
#endif
}

ساكن فراغ sched_free_group(بنية task_group *tg)
{
	free_fair_sched_group(tg);
	free_rt_sched_group(tg);
	autogroup_free(tg);
	kmem_cache_free(task_group_cache, tg);
}

ساكن فراغ sched_free_group_rcu(بنية rcu_head *rcu)
{
	sched_free_group(container_of(rcu, بنية task_group, rcu));
}

ساكن فراغ sched_unregister_group(بنية task_group *tg)
{
	unregister_fair_sched_group(tg);
	unregister_rt_sched_group(tg);
	/*
	 * We have to wait for yet another RCU grace period to expire, as
	 * print_cfs_stats() might run concurrently.
	 */
	call_rcu(&tg->rcu, sched_free_group_rcu);
}

/* allocate runqueue etc for a new task group */
بنية task_group *sched_create_group(بنية task_group *parent)
{
	بنية task_group *tg;

	tg = kmem_cache_alloc(task_group_cache, GFP_KERNEL | __GFP_ZERO);
	إذا (!tg)
		إرجاع ERR_PTR(-ENOMEM);

	إذا (!alloc_fair_sched_group(tg, parent))
		اذهب err;

	إذا (!alloc_rt_sched_group(tg, parent))
		اذهب err;

	scx_tg_init(tg);
	alloc_uclamp_sched_group(tg, parent);

	إرجاع tg;

err:
	sched_free_group(tg);
	إرجاع ERR_PTR(-ENOMEM);
}

فراغ sched_online_group(بنية task_group *tg, بنية task_group *parent)
{
	unsigned long flags;

	spin_lock_irqsave(&task_group_lock, flags);
	list_add_tail_rcu(&tg->list, &task_groups);

	/* Root should already exist: */
	WARN_ON(!parent);

	tg->parent = parent;
	INIT_LIST_HEAD(&tg->children);
	list_add_rcu(&tg->siblings, &parent->children);
	spin_unlock_irqrestore(&task_group_lock, flags);

	online_fair_sched_group(tg);
}

/* RCU callback to free various structures associated with a task group */
ساكن فراغ sched_unregister_group_rcu(بنية rcu_head *rhp)
{
	/* Now it should be safe to free those cfs_rqs: */
	sched_unregister_group(container_of(rhp, بنية task_group, rcu));
}

فراغ sched_destroy_group(بنية task_group *tg)
{
	/* Wait for possible concurrent references to cfs_rqs complete: */
	call_rcu(&tg->rcu, sched_unregister_group_rcu);
}

فراغ sched_release_group(بنية task_group *tg)
{
	unsigned long flags;

	/*
	 * Unlink first, to avoid walk_tg_tree_from() from finding us (via
	 * sched_cfs_period_timer()).
	 *
	 * For this to be effective, we have to wait for all pending users of
	 * this task group to leave their RCU critical section to ensure no new
	 * user will see our dying task group any more. Specifically ensure
	 * that tg_unthrottle_up() won't add decayed cfs_rq's to it.
	 *
	 * We therefore defer calling unregister_fair_sched_group() to
	 * sched_unregister_group() which is guarantied to get called only after the
	 * current RCU grace period has expired.
	 */
	spin_lock_irqsave(&task_group_lock, flags);
	list_del_rcu(&tg->list);
	list_del_rcu(&tg->siblings);
	spin_unlock_irqrestore(&task_group_lock, flags);
}

ساكن فراغ sched_change_group(بنية task_struct *tsk)
{
	بنية task_group *tg;

	/*
	 * All callers are synchronized by task_rq_lock(); we do not use RCU
	 * which is pointless here. Thus, we pass "true" to task_css_check()
	 * to prevent lockdep warnings.
	 */
	tg = container_of(task_css_check(tsk, cpu_cgrp_id, true),
			  بنية task_group, css);
	tg = autogroup_task_group(tsk, tg);
	tsk->sched_task_group = tg;

#ifdef CONFIG_FAIR_GROUP_SCHED
	إذا (tsk->sched_class->task_change_group)
		tsk->sched_class->task_change_group(tsk);
	وإلا
#endif
		set_task_rq(tsk, task_cpu(tsk));
}

/*
 * Change task's runqueue when it moves between groups.
 *
 * The caller of this function should have put the task in its new group by
 * now. This function just updates tsk->se.cfs_rq and tsk->se.parent to reflect
 * its new group.
 */
فراغ sched_move_task(بنية task_struct *tsk, bool for_autogroup)
{
	unsigned صحيح queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE;
	bool resched = false;
	bool queued = false;
	بنية rq *rq;

	CLASS(task_rq_lock, rq_guard)(tsk);
	rq = rq_guard.rq;

	scoped_guard (sched_change, tsk, queue_flags) {
		sched_change_group(tsk);
		إذا (!for_autogroup)
			scx_cgroup_move_task(tsk);
		إذا (scope->running)
			resched = true;
		queued = scope->queued;
	}

	إذا (resched)
		resched_curr(rq);
	وإلا إذا (queued)
		wakeup_preempt(rq, tsk, 0);

	__balance_callbacks(rq, &rq_guard.rf);
}

ساكن بنية cgroup_subsys_state *
cpu_cgroup_css_alloc(بنية cgroup_subsys_state *parent_css)
{
	بنية task_group *parent = css_tg(parent_css);
	بنية task_group *tg;

	إذا (!parent) {
		/* This is early initialization for the top cgroup */
		إرجاع &جذر_task_group.css;
	}

	tg = sched_create_group(parent);
	إذا (IS_ERR(tg))
		إرجاع ERR_PTR(-ENOMEM);

	إرجاع &tg->css;
}

/* Expose task group only after completing cgroup initialization */
ساكن صحيح cpu_cgroup_css_online(بنية cgroup_subsys_state *css)
{
	بنية task_group *tg = css_tg(css);
	بنية task_group *parent = css_tg(css->parent);
	صحيح ret;

	ret = scx_tg_online(tg);
	إذا (ret)
		إرجاع ret;

	إذا (parent)
		sched_online_group(tg, parent);

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* Propagate the effective uclamp value for the new group */
	guard(mutex)(&uclamp_mutex);
	guard(rcu)();
	cpu_util_update_eff(css);
#endif

	إرجاع 0;
}

ساكن فراغ cpu_cgroup_css_offline(بنية cgroup_subsys_state *css)
{
	بنية task_group *tg = css_tg(css);

	scx_tg_offline(tg);
}

ساكن فراغ cpu_cgroup_css_released(بنية cgroup_subsys_state *css)
{
	بنية task_group *tg = css_tg(css);

	sched_release_group(tg);
}

ساكن فراغ cpu_cgroup_css_free(بنية cgroup_subsys_state *css)
{
	بنية task_group *tg = css_tg(css);

	/*
	 * Relies on the RCU grace period between css_released() and this.
	 */
	sched_unregister_group(tg);
}

ساكن صحيح cpu_cgroup_can_attach(بنية cgroup_taskset *tset)
{
#ifdef CONFIG_RT_GROUP_SCHED
	بنية task_struct *task;
	بنية cgroup_subsys_state *css;

	إذا (!rt_group_sched_enabled())
		اذهب scx_check;

	cgroup_taskset_for_each(task, css, tset) {
		إذا (!sched_rt_can_attach(css_tg(css), task))
			إرجاع -EINVAL;
	}
scx_check:
#endif /* CONFIG_RT_GROUP_SCHED */
	إرجاع scx_cgroup_can_attach(tset);
}

ساكن فراغ cpu_cgroup_attach(بنية cgroup_taskset *tset)
{
	بنية task_struct *task;
	بنية cgroup_subsys_state *css;

	cgroup_taskset_for_each(task, css, tset)
		sched_move_task(task, false);
}

ساكن فراغ cpu_cgroup_cancel_attach(بنية cgroup_taskset *tset)
{
	scx_cgroup_cancel_attach(tset);
}

#ifdef CONFIG_UCLAMP_TASK_GROUP
ساكن فراغ cpu_util_update_eff(بنية cgroup_subsys_state *css)
{
	بنية cgroup_subsys_state *قمة_css = css;
	بنية uclamp_se *uc_parent = عدم;
	بنية uclamp_se *uc_se = عدم;
	unsigned صحيح eff[UCLAMP_CNT];
	تعداد uclamp_id clamp_id;
	unsigned صحيح clamps;

	lockdep_assert_held(&uclamp_mutex);
	WARN_ON_ONCE(!rcu_read_lock_held());

	css_for_each_descendant_pre(css, قمة_css) {
		uc_parent = css_tg(css)->parent
			? css_tg(css)->parent->uclamp : عدم;

		for_each_clamp_id(clamp_id) {
			/* Assume effective clamps matches requested clamps */
			eff[clamp_id] = css_tg(css)->uclamp_req[clamp_id].قيمة;
			/* Cap effective clamps with parent's effective clamps */
			إذا (uc_parent &&
			    eff[clamp_id] > uc_parent[clamp_id].قيمة) {
				eff[clamp_id] = uc_parent[clamp_id].قيمة;
			}
		}
		/* Ensure protection is always capped by limit */
		eff[UCLAMP_MIN] = min(eff[UCLAMP_MIN], eff[UCLAMP_MAX]);

		/* Propagate most restrictive effective clamps */
		clamps = 0x0;
		uc_se = css_tg(css)->uclamp;
		for_each_clamp_id(clamp_id) {
			إذا (eff[clamp_id] == uc_se[clamp_id].قيمة)
				استمر;
			uc_se[clamp_id].قيمة = eff[clamp_id];
			uc_se[clamp_id].bucket_id = uclamp_bucket_id(eff[clamp_id]);
			clamps |= (0x1 << clamp_id);
		}
		إذا (!clamps) {
			css = css_rightmost_descendant(css);
			استمر;
		}

		/* Immediately update descendants RUNNABLE tasks */
		uclamp_update_active_tasks(css);
	}
}

/*
 * Integer 10^N with a given N exponent by casting to integer the literal "1eN"
 * C expression. Since there is no way to convert a macro argument (N) into a
 * character constant, use two levels of macros.
 */
#define _POW10(exp) ((unsigned صحيح)1e##exp)
#define POW10(exp) _POW10(exp)

بنية uclamp_request {
#define UCLAMP_PERCENT_SHIFT	2
#define UCLAMP_PERCENT_SCALE	(100 * POW10(UCLAMP_PERCENT_SHIFT))
	s64 percent;
	u64 util;
	صحيح ret;
};

ساكن مضمن بنية uclamp_request
capacity_from_percent(حرف *buf)
{
	بنية uclamp_request req = {
		.percent = UCLAMP_PERCENT_SCALE,
		.util = SCHED_CAPACITY_SCALE,
		.ret = 0,
	};

	buf = strim(buf);
	إذا (strcmp(buf, "max")) {
		req.ret = cgroup_parse_float(buf, UCLAMP_PERCENT_SHIFT,
					     &req.percent);
		إذا (req.ret)
			إرجاع req;
		إذا ((u64)req.percent > UCLAMP_PERCENT_SCALE) {
			req.ret = -ERANGE;
			إرجاع req;
		}

		req.util = req.percent << SCHED_CAPACITY_SHIFT;
		req.util = DIV_ROUND_CLOSEST_ULL(req.util, UCLAMP_PERCENT_SCALE);
	}

	إرجاع req;
}

ساكن ssize_t cpu_uclamp_write(بنية kernfs_open_file *of, حرف *buf,
				size_t nbytes, loff_t off,
				تعداد uclamp_id clamp_id)
{
	بنية uclamp_request req;
	بنية task_group *tg;

	req = capacity_from_percent(buf);
	إذا (req.ret)
		إرجاع req.ret;

	sched_uclamp_enable();

	guard(mutex)(&uclamp_mutex);
	guard(rcu)();

	tg = css_tg(of_css(of));
	إذا (tg->uclamp_req[clamp_id].قيمة != req.util)
		uclamp_se_set(&tg->uclamp_req[clamp_id], req.util, false);

	/*
	 * Because of not recoverable conversion rounding we keep track of the
	 * exact requested value
	 */
	tg->uclamp_pct[clamp_id] = req.percent;

	/* Update effective clamps to track the most restrictive value */
	cpu_util_update_eff(of_css(of));

	إرجاع nbytes;
}

ساكن ssize_t cpu_uclamp_min_write(بنية kernfs_open_file *of,
				    حرف *buf, size_t nbytes,
				    loff_t off)
{
	إرجاع cpu_uclamp_write(of, buf, nbytes, off, UCLAMP_MIN);
}

ساكن ssize_t cpu_uclamp_max_write(بنية kernfs_open_file *of,
				    حرف *buf, size_t nbytes,
				    loff_t off)
{
	إرجاع cpu_uclamp_write(of, buf, nbytes, off, UCLAMP_MAX);
}

ساكن مضمن فراغ cpu_uclamp_print(بنية seq_file *sf,
				    تعداد uclamp_id clamp_id)
{
	بنية task_group *tg;
	u64 util_clamp;
	u64 percent;
	u32 rem;

	scoped_guard (rcu) {
		tg = css_tg(seq_css(sf));
		util_clamp = tg->uclamp_req[clamp_id].قيمة;
	}

	إذا (util_clamp == SCHED_CAPACITY_SCALE) {
		seq_puts(sf, "max\n");
		إرجاع;
	}

	percent = tg->uclamp_pct[clamp_id];
	percent = div_u64_rem(percent, POW10(UCLAMP_PERCENT_SHIFT), &rem);
	seq_printf(sf, "%llu.%0*u\n", percent, UCLAMP_PERCENT_SHIFT, rem);
}

ساكن صحيح cpu_uclamp_min_show(بنية seq_file *sf, فراغ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MIN);
	إرجاع 0;
}

ساكن صحيح cpu_uclamp_max_show(بنية seq_file *sf, فراغ *v)
{
	cpu_uclamp_print(sf, UCLAMP_MAX);
	إرجاع 0;
}
#endif /* CONFIG_UCLAMP_TASK_GROUP */

#ifdef CONFIG_GROUP_SCHED_WEIGHT
ساكن unsigned long tg_weight(بنية task_group *tg)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	إرجاع scale_load_down(tg->shares);
#وإلا
	إرجاع sched_weight_from_cgroup(tg->scx.weight);
#endif
}

ساكن صحيح cpu_shares_write_u64(بنية cgroup_subsys_state *css,
				بنية cftype *cftype, u64 shareval)
{
	صحيح ret;

	إذا (shareval > scale_load_down(ULONG_MAX))
		shareval = MAX_SHARES;
	ret = sched_group_set_shares(css_tg(css), scale_load(shareval));
	إذا (!ret)
		scx_group_set_weight(css_tg(css),
				     sched_weight_to_cgroup(shareval));
	إرجاع ret;
}

ساكن u64 cpu_shares_read_u64(بنية cgroup_subsys_state *css,
			       بنية cftype *cft)
{
	إرجاع tg_weight(css_tg(css));
}
#endif /* CONFIG_GROUP_SCHED_WEIGHT */

#ifdef CONFIG_CFS_BANDWIDTH
ساكن DEFINE_MUTEX(cfs_constraints_mutex);

ساكن صحيح __cfs_schedulable(بنية task_group *tg, u64 period, u64 runtime);

ساكن صحيح tg_set_cfs_bandwidth(بنية task_group *tg,
				u64 period_us, u64 quota_us, u64 burst_us)
{
	صحيح i, ret = 0, runtime_enabled, runtime_was_enabled;
	بنية cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	u64 period, quota, burst;

	period = (u64)period_us * NSEC_PER_USEC;

	إذا (quota_us == RUNTIME_INF)
		quota = RUNTIME_INF;
	وإلا
		quota = (u64)quota_us * NSEC_PER_USEC;

	burst = (u64)burst_us * NSEC_PER_USEC;

	/*
	 * Prevent race between setting of cfs_rq->runtime_enabled and
	 * unthrottle_offline_cfs_rqs().
	 */
	guard(cpus_read_lock)();
	guard(mutex)(&cfs_constraints_mutex);

	ret = __cfs_schedulable(tg, period, quota);
	إذا (ret)
		إرجاع ret;

	runtime_enabled = quota != RUNTIME_INF;
	runtime_was_enabled = cfs_b->quota != RUNTIME_INF;
	/*
	 * If we need to toggle cfs_bandwidth_used, off->on must occur
	 * before making related changes, and on->off must occur afterwards
	 */
	إذا (runtime_enabled && !runtime_was_enabled)
		cfs_bandwidth_usage_inc();

	scoped_guard (raw_spinlock_irq, &cfs_b->lock) {
		cfs_b->period = ns_to_ktime(period);
		cfs_b->quota = quota;
		cfs_b->burst = burst;

		__refill_cfs_bandwidth_runtime(cfs_b);

		/*
		 * Restart the period timer (if active) to handle new
		 * period expiry:
		 */
		إذا (runtime_enabled)
			start_cfs_bandwidth(cfs_b);
	}

	for_each_online_cpu(i) {
		بنية cfs_rq *cfs_rq = tg_cfs_rq(tg, i);
		بنية rq *rq = cfs_rq->rq;

		guard(rq_lock_irq)(rq);

		cfs_rq->runtime_enabled = runtime_enabled;
		cfs_rq->runtime_remaining = 1;

		إذا (cfs_rq->throttled) {
			update_rq_clock(rq);
			unthrottle_cfs_rq(cfs_rq);
		}
	}

	إذا (runtime_was_enabled && !runtime_enabled)
		cfs_bandwidth_usage_dec();

	إرجاع 0;
}

ساكن u64 tg_get_cfs_period(بنية task_group *tg)
{
	u64 cfs_period_us;

	cfs_period_us = ktime_to_ns(tg->cfs_bandwidth.period);
	do_div(cfs_period_us, NSEC_PER_USEC);

	إرجاع cfs_period_us;
}

ساكن u64 tg_get_cfs_quota(بنية task_group *tg)
{
	u64 quota_us;

	إذا (tg->cfs_bandwidth.quota == RUNTIME_INF)
		إرجاع RUNTIME_INF;

	quota_us = tg->cfs_bandwidth.quota;
	do_div(quota_us, NSEC_PER_USEC);

	إرجاع quota_us;
}

ساكن u64 tg_get_cfs_burst(بنية task_group *tg)
{
	u64 burst_us;

	burst_us = tg->cfs_bandwidth.burst;
	do_div(burst_us, NSEC_PER_USEC);

	إرجاع burst_us;
}

بنية cfs_schedulable_data {
	بنية task_group *tg;
	u64 period, quota;
};

/*
 * normalize group quota/period to be quota/max_period
 * note: units are usecs
 */
ساكن u64 normalize_cfs_quota(بنية task_group *tg,
			       بنية cfs_schedulable_data *d)
{
	u64 quota, period;

	إذا (tg == d->tg) {
		period = d->period;
		quota = d->quota;
	} وإلا {
		period = tg_get_cfs_period(tg);
		quota = tg_get_cfs_quota(tg);
	}

	/* note: these should typically be equivalent */
	إذا (quota == RUNTIME_INF || quota == -1)
		إرجاع RUNTIME_INF;

	إرجاع to_ratio(period, quota);
}

ساكن صحيح tg_cfs_schedulable_down(بنية task_group *tg, فراغ *data)
{
	بنية cfs_schedulable_data *d = data;
	بنية cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
	s64 quota = 0, parent_quota = -1;

	إذا (!tg->parent) {
		quota = RUNTIME_INF;
	} وإلا {
		بنية cfs_bandwidth *parent_b = &tg->parent->cfs_bandwidth;

		quota = normalize_cfs_quota(tg, d);
		parent_quota = parent_b->hierarchical_quota;

		/*
		 * Ensure max(child_quota) <= parent_quota.  On cgroup2,
		 * always take the non-RUNTIME_INF min.  On cgroup1, only
		 * inherit when no limit is set. In both cases this is used
		 * by the scheduler to determine if a given CFS task has a
		 * bandwidth constraint at some higher level.
		 */
		إذا (cgroup_subsys_on_dfl(cpu_cgrp_subsys)) {
			إذا (quota == RUNTIME_INF)
				quota = parent_quota;
			وإلا إذا (parent_quota != RUNTIME_INF)
				quota = min(quota, parent_quota);
		} وإلا {
			إذا (quota == RUNTIME_INF)
				quota = parent_quota;
			وإلا إذا (parent_quota != RUNTIME_INF && quota > parent_quota)
				إرجاع -EINVAL;
		}
	}
	cfs_b->hierarchical_quota = quota;

	إرجاع 0;
}

ساكن صحيح __cfs_schedulable(بنية task_group *tg, u64 period, u64 quota)
{
	بنية cfs_schedulable_data data = {
		.tg = tg,
		.period = period,
		.quota = quota,
	};

	إذا (quota != RUNTIME_INF) {
		do_div(data.period, NSEC_PER_USEC);
		do_div(data.quota, NSEC_PER_USEC);
	}

	guard(rcu)();
	إرجاع walk_tg_شجرة(tg_cfs_schedulable_down, tg_nop, &data);
}

ساكن صحيح cpu_cfs_stat_show(بنية seq_file *sf, فراغ *v)
{
	بنية task_group *tg = css_tg(seq_css(sf));
	بنية cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;

	seq_printf(sf, "nr_periods %d\n", cfs_b->nr_periods);
	seq_printf(sf, "nr_throttled %d\n", cfs_b->nr_throttled);
	seq_printf(sf, "throttled_time %llu\n", cfs_b->throttled_time);

	إذا (schedstat_enabled() && tg != &جذر_task_group) {
		بنية sched_statistics *stats;
		u64 ws = 0;
		صحيح i;

		for_each_possible_cpu(i) {
			stats = __schedstats_from_se(tg_se(tg, i));
			ws += schedstat_val(stats->wait_sum);
		}

		seq_printf(sf, "wait_sum %llu\n", ws);
	}

	seq_printf(sf, "nr_bursts %d\n", cfs_b->nr_burst);
	seq_printf(sf, "burst_time %llu\n", cfs_b->burst_time);

	إرجاع 0;
}

ساكن u64 throttled_time_self(بنية task_group *tg)
{
	صحيح i;
	u64 total = 0;

	for_each_possible_cpu(i) {
		total += READ_ONCE(tg_cfs_rq(tg, i)->throttled_clock_self_time);
	}

	إرجاع total;
}

ساكن صحيح cpu_cfs_local_stat_show(بنية seq_file *sf, فراغ *v)
{
	بنية task_group *tg = css_tg(seq_css(sf));

	seq_printf(sf, "throttled_time %llu\n", throttled_time_self(tg));

	إرجاع 0;
}
#endif /* CONFIG_CFS_BANDWIDTH */

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
ثابت u64 max_bw_quota_period_us = 1 * USEC_PER_SEC; /* 1s */
ساكن ثابت u64 min_bw_quota_period_us = 1 * USEC_PER_MSEC; /* 1ms */
/* More than 203 days if BW_SHIFT equals 20. */
ساكن ثابت u64 max_bw_runtime_us = MAX_BW;

ساكن فراغ tg_bandwidth(بنية task_group *tg,
			 u64 *period_us_p, u64 *quota_us_p, u64 *burst_us_p)
{
#ifdef CONFIG_CFS_BANDWIDTH
	إذا (period_us_p)
		*period_us_p = tg_get_cfs_period(tg);
	إذا (quota_us_p)
		*quota_us_p = tg_get_cfs_quota(tg);
	إذا (burst_us_p)
		*burst_us_p = tg_get_cfs_burst(tg);
#وإلا /* !CONFIG_CFS_BANDWIDTH */
	إذا (period_us_p)
		*period_us_p = tg->scx.bw_period_us;
	إذا (quota_us_p)
		*quota_us_p = tg->scx.bw_quota_us;
	إذا (burst_us_p)
		*burst_us_p = tg->scx.bw_burst_us;
#endif /* CONFIG_CFS_BANDWIDTH */
}

ساكن u64 cpu_period_read_u64(بنية cgroup_subsys_state *css,
			       بنية cftype *cft)
{
	u64 period_us;

	tg_bandwidth(css_tg(css), &period_us, عدم, عدم);
	إرجاع period_us;
}

ساكن صحيح tg_set_bandwidth(بنية task_group *tg,
			    u64 period_us, u64 quota_us, u64 burst_us)
{
	ثابت u64 max_usec = U64_MAX / NSEC_PER_USEC;
	صحيح ret = 0;

	إذا (tg == &جذر_task_group)
		إرجاع -EINVAL;

	/* Values should survive translation to nsec */
	إذا (period_us > max_usec ||
	    (quota_us != RUNTIME_INF && quota_us > max_usec) ||
	    burst_us > max_usec)
		إرجاع -EINVAL;

	/*
	 * Ensure we have some amount of bandwidth every period. This is to
	 * prevent reaching a state of large arrears when throttled via
	 * entity_tick() resulting in prolonged exit starvation.
	 */
	إذا (quota_us < min_bw_quota_period_us ||
	    period_us < min_bw_quota_period_us)
		إرجاع -EINVAL;

	/*
	 * Likewise, bound things on the other side by preventing insane quota
	 * periods.  This also allows us to normalize in computing quota
	 * feasibility.
	 */
	إذا (period_us > max_bw_quota_period_us)
		إرجاع -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	إذا (quota_us != RUNTIME_INF && quota_us > max_bw_runtime_us)
		إرجاع -EINVAL;

	إذا (quota_us != RUNTIME_INF && (burst_us > quota_us ||
					burst_us + quota_us > max_bw_runtime_us))
		إرجاع -EINVAL;

#ifdef CONFIG_CFS_BANDWIDTH
	ret = tg_set_cfs_bandwidth(tg, period_us, quota_us, burst_us);
#endif /* CONFIG_CFS_BANDWIDTH */
	إذا (!ret)
		scx_group_set_bandwidth(tg, period_us, quota_us, burst_us);
	إرجاع ret;
}

ساكن s64 cpu_quota_read_s64(بنية cgroup_subsys_state *css,
			      بنية cftype *cft)
{
	u64 quota_us;

	tg_bandwidth(css_tg(css), عدم, &quota_us, عدم);
	إرجاع quota_us;	/* (s64)RUNTIME_INF becomes -1 */
}

ساكن u64 cpu_burst_read_u64(بنية cgroup_subsys_state *css,
			      بنية cftype *cft)
{
	u64 burst_us;

	tg_bandwidth(css_tg(css), عدم, عدم, &burst_us);
	إرجاع burst_us;
}

ساكن صحيح cpu_period_write_u64(بنية cgroup_subsys_state *css,
				بنية cftype *cftype, u64 period_us)
{
	بنية task_group *tg = css_tg(css);
	u64 quota_us, burst_us;

	tg_bandwidth(tg, عدم, &quota_us, &burst_us);
	إرجاع tg_set_bandwidth(tg, period_us, quota_us, burst_us);
}

ساكن صحيح cpu_quota_write_s64(بنية cgroup_subsys_state *css,
			       بنية cftype *cftype, s64 quota_us)
{
	بنية task_group *tg = css_tg(css);
	u64 period_us, burst_us;

	إذا (quota_us < 0)
		quota_us = RUNTIME_INF;

	tg_bandwidth(tg, &period_us, عدم, &burst_us);
	إرجاع tg_set_bandwidth(tg, period_us, quota_us, burst_us);
}

ساكن صحيح cpu_burst_write_u64(بنية cgroup_subsys_state *css,
			       بنية cftype *cftype, u64 burst_us)
{
	بنية task_group *tg = css_tg(css);
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, عدم);
	إرجاع tg_set_bandwidth(tg, period_us, quota_us, burst_us);
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

#ifdef CONFIG_RT_GROUP_SCHED
ساكن صحيح cpu_rt_runtime_write(بنية cgroup_subsys_state *css,
				بنية cftype *cft, s64 val)
{
	إرجاع sched_group_set_rt_runtime(css_tg(css), val);
}

ساكن s64 cpu_rt_runtime_read(بنية cgroup_subsys_state *css,
			       بنية cftype *cft)
{
	إرجاع sched_group_rt_runtime(css_tg(css));
}

ساكن صحيح cpu_rt_period_write_uint(بنية cgroup_subsys_state *css,
				    بنية cftype *cftype, u64 rt_period_us)
{
	إرجاع sched_group_set_rt_period(css_tg(css), rt_period_us);
}

ساكن u64 cpu_rt_period_read_uint(بنية cgroup_subsys_state *css,
				   بنية cftype *cft)
{
	إرجاع sched_group_rt_period(css_tg(css));
}
#endif /* CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_GROUP_SCHED_WEIGHT
ساكن s64 cpu_idle_read_s64(بنية cgroup_subsys_state *css,
			       بنية cftype *cft)
{
	إرجاع css_tg(css)->idle;
}

ساكن صحيح cpu_idle_write_s64(بنية cgroup_subsys_state *css,
				بنية cftype *cft, s64 idle)
{
	صحيح ret;

	ret = sched_group_set_idle(css_tg(css), idle);
	إذا (!ret)
		scx_group_set_idle(css_tg(css), idle);
	إرجاع ret;
}
#endif /* CONFIG_GROUP_SCHED_WEIGHT */

ساكن بنية cftype cpu_legacy_files[] = {
#ifdef CONFIG_GROUP_SCHED_WEIGHT
	{
		.name = "shares",
		.read_u64 = cpu_shares_read_u64,
		.write_u64 = cpu_shares_write_u64,
	},
	{
		.name = "idle",
		.read_s64 = cpu_idle_read_s64,
		.write_s64 = cpu_idle_write_s64,
	},
#endif
#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
	{
		.name = "cfs_period_us",
		.read_u64 = cpu_period_read_u64,
		.write_u64 = cpu_period_write_u64,
	},
	{
		.name = "cfs_quota_us",
		.read_s64 = cpu_quota_read_s64,
		.write_s64 = cpu_quota_write_s64,
	},
	{
		.name = "cfs_burst_us",
		.read_u64 = cpu_burst_read_u64,
		.write_u64 = cpu_burst_write_u64,
	},
#endif
#ifdef CONFIG_CFS_BANDWIDTH
	{
		.name = "stat",
		.seq_show = cpu_cfs_stat_show,
	},
	{
		.name = "stat.local",
		.seq_show = cpu_cfs_local_stat_show,
	},
#endif
#ifdef CONFIG_UCLAMP_TASK_GROUP
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_جذر,
		.seq_show = cpu_uclamp_min_show,
		.اكتب = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_جذر,
		.seq_show = cpu_uclamp_max_show,
		.اكتب = cpu_uclamp_max_write,
	},
#endif
	{ }	/* Terminate */
};

#ifdef CONFIG_RT_GROUP_SCHED
ساكن بنية cftype rt_group_files[] = {
	{
		.name = "rt_runtime_us",
		.read_s64 = cpu_rt_runtime_read,
		.write_s64 = cpu_rt_runtime_write,
	},
	{
		.name = "rt_period_us",
		.read_u64 = cpu_rt_period_read_uint,
		.write_u64 = cpu_rt_period_write_uint,
	},
	{ }	/* Terminate */
};

# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DEFINE_STATIC_مفتاح_FALSE(rt_group_sched);
# وإلا
DEFINE_STATIC_مفتاح_TRUE(rt_group_sched);
# endif

ساكن صحيح __init setup_rt_group_sched(حرف *str)
{
	long val;

	إذا (kstrtol(str, 0, &val) || val < 0 || val > 1) {
		pr_warn("Unable to set rt_group_sched\n");
		إرجاع 1;
	}
	إذا (val)
		static_branch_enable(&rt_group_sched);
	وإلا
		static_branch_disable(&rt_group_sched);

	إرجاع 1;
}
__setup("rt_group_sched=", setup_rt_group_sched);

ساكن صحيح __init cpu_rt_group_init(فراغ)
{
	إذا (!rt_group_sched_enabled())
		إرجاع 0;

	WARN_ON(cgroup_add_legacy_cftypes(&cpu_cgrp_subsys, rt_group_files));
	إرجاع 0;
}
subsys_initcall(cpu_rt_group_init);
#endif /* CONFIG_RT_GROUP_SCHED */

ساكن صحيح cpu_extra_stat_show(بنية seq_file *sf,
			       بنية cgroup_subsys_state *css)
{
#ifdef CONFIG_CFS_BANDWIDTH
	{
		بنية task_group *tg = css_tg(css);
		بنية cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		u64 throttled_usec, burst_usec;

		throttled_usec = cfs_b->throttled_time;
		do_div(throttled_usec, NSEC_PER_USEC);
		burst_usec = cfs_b->burst_time;
		do_div(burst_usec, NSEC_PER_USEC);

		seq_printf(sf, "nr_periods %d\n"
			   "nr_throttled %d\n"
			   "throttled_usec %llu\n"
			   "nr_bursts %d\n"
			   "burst_usec %llu\n",
			   cfs_b->nr_periods, cfs_b->nr_throttled,
			   throttled_usec, cfs_b->nr_burst, burst_usec);
	}
#endif /* CONFIG_CFS_BANDWIDTH */
	إرجاع 0;
}

ساكن صحيح cpu_local_stat_show(بنية seq_file *sf,
			       بنية cgroup_subsys_state *css)
{
#ifdef CONFIG_CFS_BANDWIDTH
	{
		بنية task_group *tg = css_tg(css);
		u64 throttled_self_usec;

		throttled_self_usec = throttled_time_self(tg);
		do_div(throttled_self_usec, NSEC_PER_USEC);

		seq_printf(sf, "throttled_usec %llu\n",
			   throttled_self_usec);
	}
#endif
	إرجاع 0;
}

#ifdef CONFIG_GROUP_SCHED_WEIGHT

ساكن u64 cpu_weight_read_u64(بنية cgroup_subsys_state *css,
			       بنية cftype *cft)
{
	إرجاع sched_weight_to_cgroup(tg_weight(css_tg(css)));
}

ساكن صحيح cpu_weight_write_u64(بنية cgroup_subsys_state *css,
				بنية cftype *cft, u64 cgrp_weight)
{
	unsigned long weight;
	صحيح ret;

	إذا (cgrp_weight < CGROUP_WEIGHT_MIN || cgrp_weight > CGROUP_WEIGHT_MAX)
		إرجاع -ERANGE;

	weight = sched_weight_from_cgroup(cgrp_weight);

	ret = sched_group_set_shares(css_tg(css), scale_load(weight));
	إذا (!ret)
		scx_group_set_weight(css_tg(css), cgrp_weight);
	إرجاع ret;
}

ساكن s64 cpu_weight_nice_read_s64(بنية cgroup_subsys_state *css,
				    بنية cftype *cft)
{
	unsigned long weight = tg_weight(css_tg(css));
	صحيح last_delta = INT_MAX;
	صحيح prio, delta;

	/* find the closest nice value to the current weight */
	لكل (prio = 0; prio < ARRAY_SIZE(sched_prio_to_weight); prio++) {
		delta = abs(sched_prio_to_weight[prio] - weight);
		إذا (delta >= last_delta)
			اكسر;
		last_delta = delta;
	}

	إرجاع PRIO_TO_NICE(prio - 1 + MAX_RT_PRIO);
}

ساكن صحيح cpu_weight_nice_write_s64(بنية cgroup_subsys_state *css,
				     بنية cftype *cft, s64 nice)
{
	unsigned long weight;
	صحيح idx, ret;

	إذا (nice < MIN_NICE || nice > MAX_NICE)
		إرجاع -ERANGE;

	idx = NICE_TO_PRIO(nice) - MAX_RT_PRIO;
	idx = array_index_nospec(idx, 40);
	weight = sched_prio_to_weight[idx];

	ret = sched_group_set_shares(css_tg(css), scale_load(weight));
	إذا (!ret)
		scx_group_set_weight(css_tg(css),
				     sched_weight_to_cgroup(weight));
	إرجاع ret;
}
#endif /* CONFIG_GROUP_SCHED_WEIGHT */

ساكن فراغ __maybe_unused cpu_period_quota_print(بنية seq_file *sf,
						  long period, long quota)
{
	إذا (quota < 0)
		seq_puts(sf, "max");
	وإلا
		seq_printf(sf, "%ld", quota);

	seq_printf(sf, " %ld\n", period);
}

/* caller should put the current value in *@periodp before calling */
ساكن صحيح __maybe_unused cpu_period_quota_parse(حرف *buf, u64 *period_us_p,
						 u64 *quota_us_p)
{
	حرف tok[21];	/* U64_MAX */

	إذا (sscanf(buf, "%20s %llu", tok, period_us_p) < 1)
		إرجاع -EINVAL;

	إذا (sscanf(tok, "%llu", quota_us_p) < 1) {
		إذا (!strcmp(tok, "max"))
			*quota_us_p = RUNTIME_INF;
		وإلا
			إرجاع -EINVAL;
	}

	إرجاع 0;
}

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
ساكن صحيح cpu_max_show(بنية seq_file *sf, فراغ *v)
{
	بنية task_group *tg = css_tg(seq_css(sf));
	u64 period_us, quota_us;

	tg_bandwidth(tg, &period_us, &quota_us, عدم);
	cpu_period_quota_print(sf, period_us, quota_us);
	إرجاع 0;
}

ساكن ssize_t cpu_max_write(بنية kernfs_open_file *of,
			     حرف *buf, size_t nbytes, loff_t off)
{
	بنية task_group *tg = css_tg(of_css(of));
	u64 period_us, quota_us, burst_us;
	صحيح ret;

	tg_bandwidth(tg, &period_us, عدم, &burst_us);
	ret = cpu_period_quota_parse(buf, &period_us, &quota_us);
	إذا (!ret)
		ret = tg_set_bandwidth(tg, period_us, quota_us, burst_us);
	إرجاع ret ?: nbytes;
}
#endif /* CONFIG_CFS_BANDWIDTH */

ساكن بنية cftype cpu_files[] = {
#ifdef CONFIG_GROUP_SCHED_WEIGHT
	{
		.name = "weight",
		.flags = CFTYPE_NOT_ON_جذر,
		.read_u64 = cpu_weight_read_u64,
		.write_u64 = cpu_weight_write_u64,
	},
	{
		.name = "weight.nice",
		.flags = CFTYPE_NOT_ON_جذر,
		.read_s64 = cpu_weight_nice_read_s64,
		.write_s64 = cpu_weight_nice_write_s64,
	},
	{
		.name = "idle",
		.flags = CFTYPE_NOT_ON_جذر,
		.read_s64 = cpu_idle_read_s64,
		.write_s64 = cpu_idle_write_s64,
	},
#endif
#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
	{
		.name = "max",
		.flags = CFTYPE_NOT_ON_جذر,
		.seq_show = cpu_max_show,
		.اكتب = cpu_max_write,
	},
	{
		.name = "max.burst",
		.flags = CFTYPE_NOT_ON_جذر,
		.read_u64 = cpu_burst_read_u64,
		.write_u64 = cpu_burst_write_u64,
	},
#endif /* CONFIG_CFS_BANDWIDTH */
#ifdef CONFIG_UCLAMP_TASK_GROUP
	{
		.name = "uclamp.min",
		.flags = CFTYPE_NOT_ON_جذر,
		.seq_show = cpu_uclamp_min_show,
		.اكتب = cpu_uclamp_min_write,
	},
	{
		.name = "uclamp.max",
		.flags = CFTYPE_NOT_ON_جذر,
		.seq_show = cpu_uclamp_max_show,
		.اكتب = cpu_uclamp_max_write,
	},
#endif /* CONFIG_UCLAMP_TASK_GROUP */
	{ }	/* terminate */
};

بنية cgroup_subsys cpu_cgrp_subsys = {
	.css_alloc	= cpu_cgroup_css_alloc,
	.css_online	= cpu_cgroup_css_online,
	.css_offline	= cpu_cgroup_css_offline,
	.css_released	= cpu_cgroup_css_released,
	.css_free	= cpu_cgroup_css_free,
	.css_extra_stat_show = cpu_extra_stat_show,
	.css_local_stat_show = cpu_local_stat_show,
	.can_attach	= cpu_cgroup_can_attach,
	.attach		= cpu_cgroup_attach,
	.cancel_attach	= cpu_cgroup_cancel_attach,
	.legacy_cftypes	= cpu_legacy_files,
	.dfl_cftypes	= cpu_files,
	.early_init	= true,
	.threaded	= true,
};

#endif /* CONFIG_CGROUP_SCHED */

فراغ dump_cpu_task(صحيح cpu)
{
	إذا (in_hardirq() && cpu == smp_processor_id()) {
		بنية pt_regs *regs;

		regs = get_irq_regs();
		إذا (regs) {
			show_regs(regs);
			إرجاع;
		}
	}

	إذا (trigger_single_cpu_backtrace(cpu))
		إرجاع;

	pr_info("Task dump for CPU %d:\n", cpu);
	sched_show_task(cpu_curr(cpu));
}

/*
 * Nice levels are multiplicative, with a gentle 10% change for every
 * nice level changed. I.e. when a CPU-bound task goes from nice 0 to
 * nice 1, it will get ~10% less CPU time than another CPU-bound task
 * that remained on nice 0.
 *
 * The "10% effect" is relative and cumulative: from _any_ nice level,
 * if you go up 1 level, it's -10% CPU usage, if you go down 1 level
 * it's +10% CPU usage. (to achieve that we use a multiplier of 1.25.
 * If a task goes up by ~10% and another task goes down by ~10% then
 * the relative distance between them is ~25%.)
 */
ثابت صحيح sched_prio_to_weight[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};

/*
 * Inverse (2^32/x) values of the sched_prio_to_weight[] array, pre-calculated.
 *
 * In cases where the weight does not change often, we can use the
 * pre-calculated inverse to speed up arithmetics by turning divisions
 * into multiplications:
 */
ثابت u32 sched_prio_to_wmult[40] = {
 /* -20 */     48388,     59856,     76040,     92818,    118348,
 /* -15 */    147320,    184698,    229616,    287308,    360437,
 /* -10 */    449829,    563644,    704093,    875809,   1099582,
 /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
 /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
 /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
 /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
 /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
};

فراغ call_trace_sched_update_nr_running(بنية rq *rq, صحيح تعداد_2)
{
        trace_sched_update_nr_running_tp(rq, تعداد_2);
}

#ifdef CONFIG_SCHED_MM_CID
/*
 * Concurrency IDentifier management
 *
 * Serialization rules:
 *
 * mm::mm_cid::mutex:	Serializes fork() and exit() and therefore
 *			protects mm::mm_cid::users and mode switch
 *			transitions
 *
 * mm::mm_cid::lock:	Serializes mm_update_max_cids() and
 *			mm_update_cpus_allowed(). Nests in mm_cid::mutex
 *			and runqueue lock.
 *
 * The mm_cidmask bitmap is not protected by any of the mm::mm_cid locks
 * and can only be modified with atomic operations.
 *
 * The mm::mm_cid:pcpu per CPU storage is protected by the CPUs runqueue
 * lock.
 *
 * CID ownership:
 *
 * A CID is either owned by a task (stored in task_struct::mm_cid.cid) or
 * by a CPU (stored in mm::mm_cid.pcpu::cid). CIDs owned by CPUs have the
 * MM_CID_ONCPU bit set.
 *
 * During the transition of ownership mode, the MM_CID_TRANSIT bit is set
 * on the CIDs. When this bit is set the tasks drop the CID back into the
 * pool when scheduling out.
 *
 * Both bits (ONCPU and TRANSIT) are filtered out by task_cid() when the
 * CID is actually handed over to user space in the RSEQ memory.
 *
 * Mode switching:
 *
 * The ownership mode is per process and stored in mm:mm_cid::mode with the
 * following possible states:
 *
 *	0:				Per task ownership
 *	0 | MM_CID_TRANSIT:		Transition from per CPU to per task
 *	MM_CID_ONCPU:			Per CPU ownership
 *	MM_CID_ONCPU | MM_CID_TRANSIT:	Transition from per task to per CPU
 *
 * All transitions of ownership mode happen in two phases:
 *
 *  1) mm:mm_cid::mode has the MM_CID_TRANSIT bit set. This is OR'ed on the
 *     CIDs and denotes that the CID is only temporarily owned by a
 *     task. When the task schedules out it drops the CID back into the
 *     pool if this bit is set.
 *
 *  2) The initiating context walks the per CPU space or the tasks to fixup
 *     or drop the CIDs and after completion it clears MM_CID_TRANSIT in
 *     mm:mm_cid::mode. After that point the CIDs are strictly task or CPU
 *     owned again.
 *
 * This two phase transition is required to prevent CID space exhaustion
 * during the transition as a direct transfer of ownership would fail:
 *
 *   - On task to CPU mode switch if a task is scheduled in on one CPU and
 *     then migrated to another CPU before the fixup freed enough per task
 *     CIDs.
 *
 *   - On CPU to task mode switch if two tasks are scheduled in on the same
 *     CPU before the fixup freed per CPU CIDs.
 *
 *   Both scenarios can result in a live lock because sched_in() is invoked
 *   with runqueue lock held and loops in search of a CID and the fixup
 *   thread can't make progress freeing them up because it is stuck on the
 *   same runqueue lock.
 *
 * While MM_CID_TRANSIT is active during the transition phase the MM_CID
 * bitmap can be contended, but that's a temporary contention bound to the
 * transition period. After that everything goes back into steady state and
 * nothing except fork() and exit() will touch the bitmap. This is an
 * acceptable tradeoff as it completely avoids complex serialization,
 * memory barriers and atomic operations for the common case.
 *
 * Aside of that this mechanism also ensures RT compability:
 *
 *   - The task which runs the fixup is fully preemptible except for the
 *     short runqueue lock held sections.
 *
 *   - The transient impact of the bitmap contention is only problematic
 *     when there is a thundering herd scenario of tasks scheduling in and
 *     out concurrently. There is not much which can be done about that
 *     except for avoiding mode switching by a proper overall system
 *     configuration.
 *
 * Switching to per CPU mode happens when the user count becomes greater
 * than the maximum number of CIDs, which is calculated by:
 *
 *	opt_cids = min(mm_cid::nr_cpus_allowed, mm_cid::users);
 *	max_cids = min(1.25 * opt_cids, num_possible_cpus());
 *
 * The +25% allowance is useful for tight CPU masks in scenarios where only
 * a few threads are created and destroyed to avoid frequent mode
 * switches. Though this allowance shrinks, the closer opt_cids becomes to
 * num_possible_cpus(), which is the (unfortunate) hard ABI limit.
 *
 * At the point of switching to per CPU mode the new user is not yet
 * visible in the system, so the task which initiated the fork() runs the
 * fixup function. mm_cid_fixup_tasks_to_cpu() walks the thread list and
 * either marks each task owned CID with MM_CID_TRANSIT if the task is
 * running on a CPU or drops it into the CID pool if a task is not on a
 * CPU. Tasks which schedule in before the task walk reaches them do the
 * handover in mm_cid_schedin(). When mm_cid_fixup_tasks_to_cpus()
 * completes it is guaranteed that no task related to that MM owns a CID
 * anymore.
 *
 * Switching back to task mode happens when the user count goes below the
 * threshold which was recorded on the per CPU mode switch:
 *
 *	pcpu_thrs = min(opt_cids - (opt_cids / 4), num_possible_cpus() / 2);
 *
 * This threshold is updated when a affinity change increases the number of
 * allowed CPUs for the MM, which might cause a switch back to per task
 * mode.
 *
 * If the switch back was initiated by a exiting task, then that task runs
 * the fixup function. If it was initiated by a affinity change, then it's
 * run either in the deferred update function in context of a workqueue or
 * by a task which forks a new one or by a task which exits. Whatever
 * happens first. mm_cid_fixup_cpus_to_task() walks through the possible
 * CPUs and either marks the CPU owned CIDs with MM_CID_TRANSIT if a
 * related task is running on the CPU or drops it into the pool. Tasks
 * which are scheduled in before the fixup covered them do the handover
 * themself. When mm_cid_fixup_cpus_to_tasks() completes it is guaranteed
 * that no CID related to that MM is owned by a CPU anymore.
 */

/*
 * Update the CID range properties when the constraints change. Invoked via
 * fork(), exit() and affinity changes
 */
ساكن فراغ __mm_update_max_cids(بنية mm_mm_cid *mc)
{
	unsigned صحيح opt_cids, max_cids;

	/* Calculate the new optimal constraint */
	opt_cids = min(mc->nr_cpus_allowed, mc->users);

	/* Adjust the maximum CIDs to +25% limited by the number of possible CPUs */
	max_cids = min(opt_cids + (opt_cids / 4), رقم_possible_cpus());
	WRITE_ONCE(mc->max_cids, max_cids);
}

ساكن مضمن unsigned صحيح mm_cid_calc_pcpu_thrs(بنية mm_mm_cid *mc)
{
	unsigned صحيح opt_cids;

	opt_cids = min(mc->nr_cpus_allowed, mc->users);
	/* Has to be at least 1 because 0 indicates PCPU mode off */
	إرجاع max(min(opt_cids - opt_cids / 4, رقم_possible_cpus() / 2), 1);
}

ساكن bool mm_update_max_cids(بنية mm_struct *mm)
{
	بنية mm_mm_cid *mc = &mm->mm_cid;
	bool percpu = cid_on_cpu(mc->mode);

	lockdep_assert_held(&mm->mm_cid.lock);

	/* Clear deferred mode switch flag. A change is handled by the caller */
	mc->update_deferred = false;
	__mm_update_max_cids(mc);

	/* Check whether owner mode must be changed */
	إذا (!percpu) {
		/* Enable per CPU mode when the number of users is above max_cids */
		إذا (mc->users > mc->max_cids)
			mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	} وإلا {
		/* Switch back to per task if user count under threshold */
		إذا (mc->users < mc->pcpu_thrs)
			mc->pcpu_thrs = 0;
	}

	/* Mode change required? */
	إذا (percpu == !!mc->pcpu_thrs)
		إرجاع false;

	/* Flip the mode and set the transition flag to bridge the transfer */
	WRITE_ONCE(mc->mode, mc->mode ^ (MM_CID_TRANSIT | MM_CID_ONCPU));
	/*
	 * Order the store against the subsequent fixups so that
	 * acquire(rq::lock) cannot be reordered by the CPU before the
	 * store.
	 */
	smp_mb();
	إرجاع true;
}

ساكن مضمن فراغ mm_update_cpus_allowed(بنية mm_struct *mm, ثابت بنية cpumask *affmsk)
{
	بنية cpumask *mm_allowed;
	بنية mm_mm_cid *mc;
	unsigned صحيح weight;

	إذا (!mm || !READ_ONCE(mm->mm_cid.users))
		إرجاع;
	/*
	 * mm::mm_cid::mm_cpus_allowed is the superset of each threads
	 * allowed CPUs mask which means it can only grow.
	 */
	mc = &mm->mm_cid;
	guard(raw_spinlock)(&mc->lock);
	mm_allowed = mm_cpus_allowed(mm);
	weight = cpumask_weighted_or(mm_allowed, mm_allowed, affmsk);
	إذا (weight == mc->nr_cpus_allowed)
		إرجاع;

	WRITE_ONCE(mc->nr_cpus_allowed, weight);
	__mm_update_max_cids(mc);
	إذا (!cid_on_cpu(mc->mode))
		إرجاع;

	/* Adjust the threshold to the wider set */
	mc->pcpu_thrs = mm_cid_calc_pcpu_thrs(mc);
	/* Switch back to per task mode? */
	إذا (mc->users >= mc->pcpu_thrs)
		إرجاع;

	/* Don't queue twice */
	إذا (mc->update_deferred)
		إرجاع;

	/* Queue the irq work, which schedules the real work */
	mc->update_deferred = true;
	irq_work_queue(&mc->irq_work);
}

ساكن مضمن فراغ mm_cid_complete_transit(بنية mm_struct *mm, unsigned صحيح mode)
{
	/*
	 * Ensure that the store removing the TRANSIT bit cannot be
	 * reordered by the CPU before the fixups have been completed.
	 */
	smp_mb();
	WRITE_ONCE(mm->mm_cid.mode, mode);
}

ساكن مضمن فراغ mm_cid_transit_to_task(بنية task_struct *t, بنية mm_cid_pcpu *pcp)
{
	إذا (cid_on_cpu(t->mm_cid.cid)) {
		unsigned صحيح cid = cpu_cid_to_cid(t->mm_cid.cid);

		t->mm_cid.cid = cid_to_transit_cid(cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ساكن فراغ mm_cid_fixup_cpus_to_tasks(بنية mm_struct *mm)
{
	unsigned صحيح cpu;

	/* Walk the CPUs and fixup all stale CIDs */
	for_each_possible_cpu(cpu) {
		بنية mm_cid_pcpu *pcp = per_cpu_ptr(mm->mm_cid.pcpu, cpu);
		بنية rq *rq = cpu_rq(cpu);

		/* Remote access to mm::mm_cid::pcpu requires rq_lock */
		guard(rq_lock_irq)(rq);
		/* Is the CID still owned by the CPU? */
		إذا (cid_on_cpu(pcp->cid)) {
			/*
			 * If rq->curr has @mm, transfer it with the
			 * transition bit set. Otherwise drop it.
			 */
			إذا (rq->curr->mm == mm && rq->curr->mm_cid.active)
				mm_cid_transit_to_task(rq->curr, pcp);
			وإلا
				mm_drop_cid_on_cpu(mm, pcp);

		} وإلا إذا (rq->curr->mm == mm && rq->curr->mm_cid.active) {
			unsigned صحيح cid = rq->curr->mm_cid.cid;

			/*
			 * Set the transition bit only on a genuine task-owned
			 * CID. A running active task can legitimately have
			 * MM_CID_UNSET here: in per-CPU mode CIDs are assigned
			 * lazily on schedule-in, so the fork()/execve() window
			 * leaves the task active with no owned CID. Setting the
			 * transition bit on MM_CID_UNSET would later feed
			 * clear_bit() an out-of-bounds bit number via
			 * mm_cid_schedout(), so exclude it. A CPU-owned
			 * (MM_CID_ONCPU) CID is handled by the cid_on_cpu()
			 * branch above and never reaches here.
			 */
			إذا (cid != MM_CID_UNSET && !cid_in_transit(cid)) {
				cid = cid_to_transit_cid(cid);
				rq->curr->mm_cid.cid = cid;
				pcp->cid = cid;
			}
		}
	}
	mm_cid_complete_transit(mm, 0);
}

ساكن مضمن فراغ mm_cid_transit_to_cpu(بنية task_struct *t, بنية mm_cid_pcpu *pcp)
{
	إذا (cid_on_task(t->mm_cid.cid)) {
		t->mm_cid.cid = cid_to_transit_cid(t->mm_cid.cid);
		pcp->cid = t->mm_cid.cid;
	}
}

ساكن فراغ mm_cid_fixup_task_to_cpu(بنية task_struct *t, بنية mm_struct *mm)
{
	/* Remote access to mm::mm_cid::pcpu requires rq_lock */
	guard(task_rq_lock)(t);
	إذا (cid_on_task(t->mm_cid.cid)) {
		/* If running on the CPU, put the CID in transit mode, otherwise drop it */
		إذا (task_rq(t)->curr == t)
			mm_cid_transit_to_cpu(t, per_cpu_ptr(mm->mm_cid.pcpu, task_cpu(t)));
		وإلا
			mm_unset_cid_on_task(t);
	}
}

ساكن فراغ mm_cid_fixup_tasks_to_cpus(فراغ)
{
	بنية mm_struct *mm = current->mm;
	بنية task_struct *t;

	lockdep_assert_held(&mm->mm_cid.mutex);

	hlist_for_each_entry(t, &mm->mm_cid.user_list, mm_cid.عقدة) {
		/* Current has already transferred before invoking the fixup. */
		إذا (t != current)
			mm_cid_fixup_task_to_cpu(t, mm);
	}

	mm_cid_complete_transit(mm, MM_CID_ONCPU);
}

ساكن bool sched_mm_cid_add_user(بنية task_struct *t, بنية mm_struct *mm)
{
	lockdep_assert_held(&mm->mm_cid.lock);

	t->mm_cid.active = 1;
	hlist_add_head(&t->mm_cid.عقدة, &mm->mm_cid.user_list);
	mm->mm_cid.users++;
	إرجاع mm_update_max_cids(mm);
}

ساكن فراغ sched_mm_cid_fork(بنية task_struct *t)
{
	بنية mm_struct *mm = t->mm;
	bool percpu;

	إذا (!mm)
		إرجاع;

	WARN_ON_ONCE(t->mm_cid.cid != MM_CID_UNSET);

	guard(mutex)(&mm->mm_cid.mutex);
	scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
		بنية mm_cid_pcpu *pcp = this_cpu_ptr(mm->mm_cid.pcpu);

		/* First user ? */
		إذا (!mm->mm_cid.users) {
			sched_mm_cid_add_user(t, mm);
			t->mm_cid.cid = mm_get_cid(mm);
			/* Required for execve() */
			pcp->cid = t->mm_cid.cid;
			إرجاع;
		}

		إذا (!sched_mm_cid_add_user(t, mm)) {
			إذا (!cid_on_cpu(mm->mm_cid.mode))
				t->mm_cid.cid = mm_get_cid(mm);
			إرجاع;
		}

		/* Handle the mode change and transfer current's CID */
		percpu = cid_on_cpu(mm->mm_cid.mode);
		إذا (!percpu)
			mm_cid_transit_to_task(current, pcp);
		وإلا
			mm_cid_transit_to_cpu(current, pcp);
	}

	إذا (percpu) {
		mm_cid_fixup_tasks_to_cpus();
	} وإلا {
		mm_cid_fixup_cpus_to_tasks(mm);
		t->mm_cid.cid = mm_get_cid(mm);
	}
}

ساكن bool sched_mm_cid_remove_user(بنية task_struct *t)
{
	lockdep_assert_held(&t->mm->mm_cid.lock);

	t->mm_cid.active = 0;
	/* Clear the transition bit */
	t->mm_cid.cid = cid_from_transit_cid(t->mm_cid.cid);
	mm_unset_cid_on_task(t);
	hlist_del_init(&t->mm_cid.عقدة);
	t->mm->mm_cid.users--;
	إرجاع mm_update_max_cids(t->mm);
}

ساكن bool __sched_mm_cid_exit(بنية task_struct *t)
{
	بنية mm_struct *mm = t->mm;

	إذا (!sched_mm_cid_remove_user(t))
		إرجاع false;
	/*
	 * Contrary to fork() this only deals with a switch back to per
	 * task mode either because the above decreased users or an
	 * affinity change increased the number of allowed CPUs and the
	 * deferred fixup did not run yet.
	 */
	إذا (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.mode)))
		إرجاع false;
	/*
	 * A failed fork(2) cleanup never gets here, so @current must have
	 * the same MM as @t. That's true for exit() and the failed
	 * pthread_create() cleanup case.
	 */
	إذا (WARN_ON_ONCE(current->mm != mm))
		إرجاع false;
	إرجاع true;
}

/*
 * When a task exits, the MM CID held by the task is not longer required as
 * the task cannot return to user space.
 */
فراغ sched_mm_cid_exit(بنية task_struct *t)
{
	بنية mm_struct *mm = t->mm;

	إذا (!mm || !t->mm_cid.active)
		إرجاع;
	/*
	 * Ensure that only one instance is doing MM CID operations within
	 * a MM. The common case is uncontended. The rare fixup case adds
	 * some overhead.
	 */
	scoped_guard(mutex, &mm->mm_cid.mutex) {
		/* mm_cid::mutex is sufficient to protect mm_cid::users */
		إذا (likely(mm->mm_cid.users > 1)) {
			scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
				إذا (!__sched_mm_cid_exit(t))
					إرجاع;
				/*
				 * Mode change. The task has the CID unset
				 * already and dealt with an eventually set
				 * TRANSIT bit. If the CID is owned by the CPU
				 * then drop it.
				 */
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			}
			mm_cid_fixup_cpus_to_tasks(mm);
			إرجاع;
		}
		/* Last user */
		scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
			/* Required across execve() */
			إذا (t == current)
				mm_cid_transit_to_task(t, this_cpu_ptr(mm->mm_cid.pcpu));
			/* Ignore mode change. There is nothing to do. */
			sched_mm_cid_remove_user(t);
		}
	}

	/*
	 * As this is the last user (execve(), process exit or failed
	 * fork(2)) there is no concurrency anymore.
	 *
	 * Synchronize eventually pending work to ensure that there are no
	 * dangling references left. @t->mm_cid.users is zero so nothing
	 * can queue this work anymore.
	 */
	irq_work_sync(&mm->mm_cid.irq_work);
	cancel_work_sync(&mm->mm_cid.work);
}

/* Deactivate MM CID allocation across execve() */
فراغ sched_mm_cid_before_execve(بنية task_struct *t)
{
	sched_mm_cid_exit(t);
}

/* Reactivate MM CID after execve() */
فراغ sched_mm_cid_after_execve(بنية task_struct *t)
{
	إذا (t->mm)
		sched_mm_cid_fork(t);
}

ساكن فراغ mm_cid_work_fn(بنية work_struct *work)
{
	بنية mm_struct *mm = container_of(work, بنية mm_struct, mm_cid.work);

	guard(mutex)(&mm->mm_cid.mutex);
	/* Did the last user task exit already? */
	إذا (!mm->mm_cid.users)
		إرجاع;

	scoped_guard(raw_spinlock_irq, &mm->mm_cid.lock) {
		/* Have fork() or exit() handled it already? */
		إذا (!mm->mm_cid.update_deferred)
			إرجاع;
		/* This clears mm_cid::update_deferred */
		إذا (!mm_update_max_cids(mm))
			إرجاع;
		/* Affinity changes can only switch back to task mode */
		إذا (WARN_ON_ONCE(cid_on_cpu(mm->mm_cid.mode)))
			إرجاع;
	}
	mm_cid_fixup_cpus_to_tasks(mm);
}

ساكن فراغ mm_cid_irq_work(بنية irq_work *work)
{
	بنية mm_struct *mm = container_of(work, بنية mm_struct, mm_cid.irq_work);

	/*
	 * Needs to be unconditional because mm_cid::lock cannot be held
	 * when scheduling work as mm_update_cpus_allowed() nests inside
	 * rq::lock and schedule_work() might end up in wakeup...
	 */
	schedule_work(&mm->mm_cid.work);
}

فراغ mm_init_cid(بنية mm_struct *mm, بنية task_struct *p)
{
	mm->mm_cid.max_cids = 0;
	mm->mm_cid.mode = 0;
	mm->mm_cid.nr_cpus_allowed = p->nr_cpus_allowed;
	mm->mm_cid.users = 0;
	mm->mm_cid.pcpu_thrs = 0;
	mm->mm_cid.update_deferred = 0;
	raw_spin_lock_init(&mm->mm_cid.lock);
	mutex_init(&mm->mm_cid.mutex);
	mm->mm_cid.irq_work = IRQ_WORK_INIT_HARD(mm_cid_irq_work);
	INIT_WORK(&mm->mm_cid.work, mm_cid_work_fn);
	INIT_HLIST_HEAD(&mm->mm_cid.user_list);
	cpumask_copy(mm_cpus_allowed(mm), &p->cpus_mask);
	bitmap_zero(mm_cidmask(mm), رقم_possible_cpus());
}
#وإلا /* CONFIG_SCHED_MM_CID */
ساكن مضمن فراغ mm_update_cpus_allowed(بنية mm_struct *mm, ثابت بنية cpumask *affmsk) { }
ساكن مضمن فراغ sched_mm_cid_fork(بنية task_struct *t) { }
#endif /* !CONFIG_SCHED_MM_CID */

ساكن DEFINE_PER_CPU(بنية sched_change_ctx, sched_change_ctx);

بنية sched_change_ctx *sched_change_begin(بنية task_struct *p, unsigned صحيح flags)
{
	بنية sched_change_ctx *ctx = this_cpu_ptr(&sched_change_ctx);
	بنية rq *rq = task_rq(p);

	/*
	 * Must exclusively use matched flags since this is both dequeue and
	 * enqueue.
	 */
	WARN_ON_ONCE(flags & 0xFFFF0000);

	lockdep_assert_rq_held(rq);

	إذا (!(flags & DEQUEUE_NOCLOCK)) {
		update_rq_clock(rq);
		flags |= DEQUEUE_NOCLOCK;
	}

	إذا ((flags & DEQUEUE_CLASS) && p->sched_class->switching_from)
		p->sched_class->switching_from(rq, p);

	*ctx = (بنية sched_change_ctx){
		.p = p,
		.class = p->sched_class,
		.flags = flags,
		.queued = task_on_rq_queued(p),
		.running = task_current_donor(rq, p),
	};

	إذا (!(flags & DEQUEUE_CLASS)) {
		إذا (p->sched_class->get_prio)
			ctx->prio = p->sched_class->get_prio(rq, p);
		وإلا
			ctx->prio = p->prio;
	}

	إذا (ctx->queued)
		dequeue_task(rq, p, flags);
	إذا (ctx->running)
		put_prev_task(rq, p);

	إذا ((flags & DEQUEUE_CLASS) && p->sched_class->switched_from)
		p->sched_class->switched_from(rq, p);

	إرجاع ctx;
}

فراغ sched_change_end(بنية sched_change_ctx *ctx)
{
	بنية task_struct *p = ctx->p;
	بنية rq *rq = task_rq(p);

	lockdep_assert_rq_held(rq);

	/*
	 * Changing class without *QUEUE_CLASS is bad.
	 */
	WARN_ON_ONCE(p->sched_class != ctx->class && !(ctx->flags & ENQUEUE_CLASS));

	إذا ((ctx->flags & ENQUEUE_CLASS) && p->sched_class->switching_to)
		p->sched_class->switching_to(rq, p);

	إذا (ctx->queued)
		enqueue_task(rq, p, ctx->flags);
	إذا (ctx->running)
		set_next_task(rq, p);

	إذا (ctx->flags & ENQUEUE_CLASS) {
		إذا (p->sched_class->switched_to)
			p->sched_class->switched_to(rq, p);

		/*
		 * Check if the task can preempt the current in case the class
		 * was upgraded. Note that none of the switch*_from() methods
		 * know the new class and none of the switch*_to() methods know
		 * the old class so we must resched here.
		 */
		إذا (ctx->queued)
			wakeup_preempt(rq, p, 0);

		إذا (ctx->running) {
			/*
			 * If this was a degradation in class; make sure to
			 * reschedule.
			 */
			إذا (sched_class_above(ctx->class, p->sched_class))
				resched_curr(rq);
		}
	} وإلا {
		p->sched_class->prio_changed(rq, p, ctx->prio);
	}
}

#ifdef CONFIG_PREFERRED_CPU
ساكن DEFINE_PER_CPU(بنية cpu_stop_work, npc_ادفع_task_work);

ساكن صحيح sched_non_preferred_cpu_ادفع_stop(فراغ *arg)
{
	بنية task_struct *p = arg;
	بنية rq *rq = this_rq();
	بنية rq_flags rf;
	صحيح cpu;

	إذا (cpu_preferred(rq->cpu)) {
		scoped_guard(rq_lock_irqsave, rq)
			rq->npc_ادفع_work_pending = false;
		put_task_struct(p);
		إرجاع 0;
	}

	scoped_guard (raw_spinlock_irq, &p->pi_lock) {
		/*
		 * select_fallback_rq() may acquire the rq lock in case of
		 * fallback. So call it before grabbing rq lock. If the task
		 * migrates to another CPU before the rq lock is acquired,
		 * subsequent validation of task's current rq will help to
		 * safely bail out.
		 */
		cpu = select_fallback_rq(rq->cpu, p);
		context_unsafe_alias(rq);
		rq_lock(rq, &rf);
		rq->npc_ادفع_work_pending = false;
		update_rq_clock(rq);

		إذا (task_rq(p) == rq && task_on_rq_queued(p)) {
			بنية rq *dest_rq = __migrate_task(rq, &rf, p, cpu);

			إذا (rq != dest_rq)
				schedstat_inc(p->stats.nr_migrations_cpu_non_preferred);
			rq = dest_rq;
		}
		rq_unlock(rq, &rf);
	}

	put_task_struct(p);
	إرجاع 0;
}

/*
 * Push the current task running on non-preferred CPU(npc).
 * Using this non preferred CPU will lead to more contention
 * in the host. So it is better not to use this CPU.
 *
 * Since task is running, call a stopper to push the task out. This is
 * similar to how task moves during hotplug. In select_fallback_rq() a
 * preferred CPU will be chosen and henceforth task shouldn't come back to
 * this CPU again.
 *
 * Works for FAIR class only.
 *
 * If task is affined only on non-preferred CPUs, no point in moving it out.
 */
فراغ sched_ادفع_current_non_preferred_cpu(بنية rq *rq)
{
	بنية task_struct *ادفع_task = rq->curr;

	scoped_guard(rq_lock, rq) {
		/* Push the task if its explicit affinity allows */
		إذا (!task_can_migrate_to_preferred(ادفع_task, rq->cpu))
			إرجاع;

		/* There is already a stopper thread. Don't race with it. */
		إذا (rq->npc_ادفع_work_pending)
			إرجاع;

		إذا (is_migration_disabled(ادفع_task))
			إرجاع;

		rq->npc_ادفع_work_pending = true;
	}

	/* sched_tick runs with interrupts disabled. */
	get_task_struct(ادفع_task);
	stop_one_cpu_nowait(rq->cpu, sched_non_preferred_cpu_ادفع_stop,
			    ادفع_task, this_cpu_ptr(&npc_ادفع_task_work));
}
#endif
