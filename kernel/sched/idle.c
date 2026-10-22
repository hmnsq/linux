// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic entry points for the idle threads and
 * implementation of the idle task scheduling class.
 *
 * (NOTE: these are not related to SCHED_IDLE batch scheduled
 *        tasks which are handled in sched/fair.c )
 */
#شامل <linux/cpuidle.h>
#شامل <linux/suspend.h>
#شامل <linux/livepatch.h>
#شامل "sched.h"
#شامل "smp.h"

/* Linker adds these: start and end of __cpuidle functions */
خارجی حرف __cpuidle_text_start[], __cpuidle_text_end[];

/**
 * sched_idle_set_state - Record idle state for the current CPU.
 * @idle_state: State to record.
 */
خالی sched_idle_سیٹ_state(ساخت cpuidle_state *idle_state)
{
	idle_سیٹ_state(this_rq(), idle_state);
}

ساکن صحیح __read_mostly cpu_idle_force_poll;

خالی cpu_idle_poll_ctrl(bool enable)
{
	اگر (enable) {
		cpu_idle_force_poll++;
	} ورنہ {
		cpu_idle_force_poll--;
		WARN_ON_ONCE(cpu_idle_force_poll < 0);
	}
}

#اگر_تعریف_شدہ CONFIG_GENERIC_IDLE_POLL_تیاری
ساکن صحیح __init cpu_idle_poll_تیاری(حرف *__unused)
{
	cpu_idle_force_poll = 1;

	واپس 1;
}
__setup("nohlt", cpu_idle_poll_تیاری);

ساکن صحیح __init cpu_idle_nopoll_تیاری(حرف *__unused)
{
	cpu_idle_force_poll = 0;

	واپس 1;
}
__setup("hlt", cpu_idle_nopoll_تیاری);
#اگر_ختم /* CONFIG_GENERIC_IDLE_POLL_SETUP */

ساکن noinline صحیح __cpuidle cpu_idle_poll(خالی)
{
	instrumentation_begin();
	trace_cpu_idle(0, smp_processor_id());
	stop_critical_timings();
	ct_cpuidle_enter();

	raw_local_irq_enable();
	جب (!tif_need_resched() &&
	       (cpu_idle_force_poll || tick_check_نشر_expired()))
		cpu_relax();
	raw_local_irq_disable();

	ct_cpuidle_exit();
	آغاز_critical_timings();
	trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
	local_irq_enable();
	instrumentation_انتہا();

	واپس 1;
}

/* Weak implementations for optional arch specific functions */
خالی __weak arch_cpu_idle_prepare(خالی) { }
خالی __weak arch_cpu_idle_enter(خالی) { }
خالی __weak arch_cpu_idle_exit(خالی) { }
خالی __weak __noreturn arch_cpu_idle_dead(خالی) { جب (1); }
خالی __weak arch_cpu_idle(خالی)
{
	cpu_idle_force_poll = 1;
}

#اگر_تعریف_شدہ CONFIG_GENERIC_CLOCKEVENTS_نشر_IDLE
DEFINE_STATIC_کلید_FALSE(arch_needs_tick_نشر);

ساکن اندرونی خالی cond_tick_نشر_enter(خالی)
{
	اگر (static_branch_unlikely(&arch_needs_tick_نشر))
		tick_نشر_enter();
}

ساکن اندرونی خالی cond_tick_نشر_exit(خالی)
{
	اگر (static_branch_unlikely(&arch_needs_tick_نشر))
		tick_نشر_exit();
}
#ورنہ /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE: */
ساکن اندرونی خالی cond_tick_نشر_enter(خالی) { }
ساکن اندرونی خالی cond_tick_نشر_exit(خالی) { }
#اگر_ختم /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE */

/**
 * default_idle_call - Default CPU idle routine.
 *
 * To use when the cpuidle framework cannot be used.
 */
خالی __cpuidle default_idle_call(خالی)
{
	instrumentation_begin();
	اگر (!current_clr_polling_and_test()) {
		cond_tick_نشر_enter();
		trace_cpu_idle(1, smp_processor_id());
		stop_critical_timings();

		ct_cpuidle_enter();
		arch_cpu_idle();
		ct_cpuidle_exit();

		آغاز_critical_timings();
		trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
		cond_tick_نشر_exit();
	}
	local_irq_enable();
	instrumentation_انتہا();
}

ساکن صحیح call_cpuidle_s2idle(ساخت cpuidle_driver *drv,
			       ساخت cpuidle_device *dev,
			       u64 max_latency_ns)
{
	اگر (current_clr_polling_and_test())
		واپس -EBUSY;

	واپس cpuidle_enter_s2idle(drv, dev, max_latency_ns);
}

ساکن صحیح call_cpuidle(ساخت cpuidle_driver *drv, ساخت cpuidle_device *dev,
		      صحیح اگلا_state)
{
	/*
	 * The idle task must be scheduled, it is pointless to go to idle, just
	 * update no idle residency and return.
	 */
	اگر (current_clr_polling_and_test()) {
		dev->آخری_residency_ns = 0;
		local_irq_enable();
		واپس -EBUSY;
	}

	/*
	 * Enter the idle state previously returned by the governor decision.
	 * This function will block until an interrupt occurs and will take
	 * care of re-enabling the local interrupts
	 */
	واپس cpuidle_enter(drv, dev, اگلا_state);
}

ساکن خالی idle_call_stop_or_retain_tick(bool stop_tick)
{
	اگر (stop_tick || tick_nohz_tick_stopped())
		tick_nohz_idle_stop_tick();
	ورنہ
		tick_nohz_idle_retain_tick();
}

/**
 * cpuidle_idle_call - the main idle function
 *
 * NOTE: no locks or semaphores should be used here
 *
 * On architectures that support TIF_POLLING_NRFLAG, is called with polling
 * set, and it returns with polling set.  If it ever stops polling, it
 * must clear the polling bit.
 */
ساکن خالی cpuidle_idle_call(bool stop_tick)
{
	ساخت cpuidle_device *dev = cpuidle_get_device();
	ساخت cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	صحیح اگلا_state, entered_state;

	/*
	 * Check if the idle task must be rescheduled. If it is the
	 * case, exit the function after re-enabling the local IRQ.
	 */
	اگر (need_resched()) {
		local_irq_enable();
		واپس;
	}

	اگر (cpuidle_not_دستیاب(drv, dev)) {
		idle_call_stop_or_retain_tick(stop_tick);

		default_idle_call();
		جاؤ exit_idle;
	}

	/*
	 * Suspend-to-idle ("s2idle") is a system state in which all user space
	 * has been frozen, all I/O devices have been suspended and the only
	 * activity happens here and in interrupts (if any). In that case bypass
	 * the cpuidle governor and go straight for the deepest idle state
	 * available.  Possibly also suspend the local tick and the entire
	 * timekeeping to prevent timer interrupts from kicking us out of idle
	 * until a proper wakeup interrupt happens.
	 */

	اگر (idle_should_enter_s2idle() || dev->forced_idle_latency_حد_بندی_ns) {
		u64 max_latency_ns;

		اگر (idle_should_enter_s2idle()) {
			max_latency_ns = cpu_wakeup_latency_qos_حد_بندی() *
					 NSEC_فی_USEC;

			entered_state = call_cpuidle_s2idle(drv, dev,
							    max_latency_ns);
			اگر (entered_state > 0)
				جاؤ exit_idle;
		} ورنہ {
			max_latency_ns = dev->forced_idle_latency_حد_بندی_ns;
		}

		tick_nohz_idle_stop_tick();

		اگلا_state = cpuidle_ڈھونڈو_گہرا_ترین_state(drv, dev, max_latency_ns);
		call_cpuidle(drv, dev, اگلا_state);
	} ورنہ اگر (drv->state_گنتی > 1) {
		/*
		 * stop_tick is expected to be true by default by cpuidle
		 * governors, which allows them to select idle states with
		 * target residency above the tick period length.
		 */
		stop_tick = true;

		/*
		 * Ask the cpuidle framework to choose a convenient idle state.
		 */
		اگلا_state = cpuidle_select(drv, dev, &stop_tick);

		idle_call_stop_or_retain_tick(stop_tick);

		entered_state = call_cpuidle(drv, dev, اگلا_state);
		/*
		 * Give the governor an opportunity to reflect on the outcome
		 */
		cpuidle_reflect(dev, entered_state);
	} ورنہ {
		idle_call_stop_or_retain_tick(stop_tick);

		/*
		 * If there is only a single idle state (or none), there is
		 * nothing meaningful for the governor to choose.  Skip the
		 * governor and always use state 0.
		 */
		call_cpuidle(drv, dev, 0);
	}

exit_idle:
	__current_set_polling();

	/*
	 * It is up to the idle functions to re-enable local interrupts
	 */
	اگر (WARN_ON_ONCE(irqs_disabled()))
		local_irq_enable();
}

/*
 * Generic idle loop implementation
 *
 * Called with polling cleared.
 */
ساکن خالی do_idle(خالی)
{
	صحیح cpu = smp_processor_id();
	bool got_tick = false;

	اگر (cpu_is_offline(cpu)) {
		local_irq_disable();
		/* All per-CPU kernel threads should be done by now. */
		WARN_ON_ONCE(need_resched());
		cpuhp_report_idle_dead();
		arch_cpu_idle_dead();
	}

	/*
	 * Check if we need to update blocked load
	 */
	nohz_تسلسل_idle_balance(cpu);

	/*
	 * If the arch has a polling bit, we maintain an invariant:
	 *
	 * Our polling bit is clear if we're not scheduled (i.e. if rq->curr !=
	 * rq->idle). This means that, if rq->idle has the polling bit set,
	 * then setting need_resched is guaranteed to cause the CPU to
	 * reschedule.
	 */

	__current_set_polling();
	tick_nohz_idle_enter();

	جب (!need_resched()) {

		/*
		 * Interrupts shouldn't be re-enabled from that point on until
		 * the CPU sleeping instruction is reached. Otherwise an interrupt
		 * may fire and queue a timer that would be ignored until the CPU
		 * wakes from the sleeping instruction. And testing need_resched()
		 * doesn't tell about pending needed timer reprogram.
		 *
		 * Several cases to consider:
		 *
		 * - SLEEP-UNTIL-PENDING-INTERRUPT based instructions such as
		 *   "wfi" or "mwait" are fine because they can be entered with
		 *   interrupt disabled.
		 *
		 * - sti;mwait() couple is fine because the interrupts are
		 *   re-enabled only upon the execution of mwait, leaving no gap
		 *   in-between.
		 *
		 * - ROLLBACK based idle handlers with the sleeping instruction
		 *   called with interrupts enabled are NOT fine. In this scheme
		 *   when the interrupt detects it has interrupted an idle handler,
		 *   it rolls back to its beginning which performs the
		 *   need_resched() check before re-executing the sleeping
		 *   instruction. This can leak a pending needed timer reprogram.
		 *   If such a scheme is really mandatory due to the lack of an
		 *   appropriate CPU sleeping instruction, then a FAST-FORWARD
		 *   must instead be applied: when the interrupt detects it has
		 *   interrupted an idle handler, it must resume to the end of
		 *   this idle handler so that the generic idle loop is iterated
		 *   again to reprogram the tick.
		 */
		local_irq_disable();

		arch_cpu_idle_enter();
		rcu_nocb_flush_deferred_wakeup();

		/*
		 * In poll mode we re-enable interrupts and spin. Also if we
		 * detected in the wakeup from idle path that the tick
		 * broadcast device expired for us, we don't want to go deep
		 * idle as we know that the IPI is going to arrive right away.
		 */
		اگر (cpu_idle_force_poll || tick_check_نشر_expired()) {
			tick_nohz_idle_restart_tick();
			cpu_idle_poll();
		} ورنہ {
			cpuidle_idle_call(got_tick);
		}
		got_tick = tick_nohz_idle_got_tick();
		arch_cpu_idle_exit();
	}

	/*
	 * Since we fell out of the loop above, we know TIF_NEED_RESCHED must
	 * be set, propagate it into PREEMPT_NEED_RESCHED.
	 *
	 * This is required because for polling idle loops we will not have had
	 * an IPI to fold the state for us.
	 */
	preempt_سیٹ_need_resched();
	tick_nohz_idle_exit();
	__current_clr_polling();

	/*
	 * We promise to call sched_ttwu_pending() and reschedule if
	 * need_resched() is set while polling is set. That means that clearing
	 * polling needs to be visible before doing these things.
	 */
	smp_mb__after_atomic();

	/*
	 * RCU relies on this call to be done outside of an RCU read-side
	 * critical section.
	 */
	flush_smp_call_function_قطار();
	جدول_وقت_idle();

	اگر (unlikely(klp_patch_pending(current)))
		klp_update_patch_state(current);
}

bool cpu_اندر_idle(غیرعلامتی طویل pc)
{
	واپس pc >= (غیرعلامتی طویل)__cpuidle_text_start &&
		pc < (غیرعلامتی طویل)__cpuidle_text_end;
}

ساخت idle_timer {
	ساخت hrtimer timer;
	صحیح مکمل;
};

ساکن شمار hrtimer_restart idle_inject_timer_fn(ساخت hrtimer *timer)
{
	ساخت idle_timer *it = container_کا(timer, ساخت idle_timer, timer);

	WRITE_ONCE(it->مکمل, 1);
	سیٹ_tsk_need_resched(current);

	واپس HRTIMER_NORESTART;
}

خالی play_idle_precise(u64 duration_ns, u64 latency_ns)
{
	ساخت idle_timer it;

	/*
	 * Only FIFO tasks can disable the tick since they don't need the forced
	 * preemption.
	 */
	WARN_ON_ONCE(current->policy != SCHED_FIFO);
	WARN_ON_ONCE(current->nr_cpus_allowed != 1);
	WARN_ON_ONCE(!(current->flags & PF_KTHREAD));
	WARN_ON_ONCE(!(current->flags & PF_NO_SETAFFINITY));
	WARN_ON_ONCE(!duration_ns);
	WARN_ON_ONCE(current->mm);

	rcu_sleep_check();
	preempt_disable();
	current->flags |= PF_IDLE;
	cpuidle_use_گہرا_ترین_state(latency_ns);

	it.مکمل = 0;
	hrtimer_تیاری_on_ڈھیر(&it.timer, idle_inject_timer_fn, CLOCK_MONOTONIC,
			       HRTIMER_منوال_REL_HARD);
	hrtimer_آغاز(&it.timer, ns_مقصد_ktime(duration_ns),
		      HRTIMER_منوال_REL_PINNED_HARD);

	جب (!READ_ONCE(it.مکمل))
		do_idle();

	cpuidle_use_گہرا_ترین_state(0);
	current->flags &= ~PF_IDLE;

	preempt_fold_need_resched();
	preempt_enable();
}
EXPORT_علامت_GPL(play_idle_precise);

خالی cpu_startup_اندراج(شمار cpuhp_state state)
{
	current->flags |= PF_IDLE;
	arch_cpu_idle_prepare();
	cpuhp_online_idle(state);
	جب (1)
		do_idle();
}

/*
 * idle-task scheduling class.
 */

ساکن صحیح
select_task_rq_idle(ساخت task_struct *p, صحیح cpu, صحیح flags)
{
	واپس task_cpu(p); /* IDLE tasks as never migrated */
}

ساکن صحیح
balance_idle(ساخت rq *rq, ساخت rq_flags *rf)
{
	واپس WARN_ON_ONCE(1);
}

/*
 * Idle tasks are unconditionally rescheduled:
 */
ساکن خالی wakeup_preempt_idle(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	resched_curr(rq);
}

ساکن خالی update_curr_idle(ساخت rq *rq);

ساکن خالی put_پچھلا_task_idle(ساخت rq *rq, ساخت task_struct *پچھلا, ساخت task_struct *اگلا)
{
	update_curr_idle(rq);
	scx_update_idle(rq, false, true);
	update_rq_avg_idle(rq);
}

ساکن خالی سیٹ_اگلا_task_idle(ساخت rq *rq, ساخت task_struct *اگلا, شمار snt_e type)
{
	اگر (type == SNT_REPICK)
		واپس;

	update_idle_core(rq);
	scx_update_idle(rq, true, true);
	schedstat_inc(rq->sched_goidle);
	اگلا->se.exec_آغاز = rq_clock_task(rq);

	/*
	 * rq is about to be idle, check if we need to update the
	 * lost_idle_time of clock_pelt
	 */
	update_idle_rq_clock_pelt(rq);
}

ساخت task_struct *pick_task_idle(ساخت rq *rq, ساخت rq_flags *rf)
{
	/*
	 * Notify scx only on an idle-to-idle re-pick (the cpu was already idle).
	 * A real task->idle transition is delivered by set_next_task_idle(), so
	 * calling here too would duplicate it.
	 */
	اگر (scx_enabled() && is_idle_task(rq->curr))
		scx_update_idle(rq, true, false);
	واپس rq->idle;
}

/*
 * It is not legal to sleep in the idle task - print a warning
 * message if some code attempts to do it:
 */
ساکن bool
قطار_سے_نکالو_task_idle(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	raw_spin_rq_unlock_irq(rq);
	printk(KERN_ERR "bad: scheduling from the idle thread!\n");
	dump_ڈھیر();
	raw_spin_rq_تالا_irq(rq);
	واپس true;
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ساکن خالی task_tick_idle(ساخت rq *rq, ساخت task_struct *curr, صحیح queued)
{
	update_curr_idle(rq);
}

ساکن خالی switching_مقصد_idle(ساخت rq *rq, ساخت task_struct *p)
{
	BUG();
}

ساکن خالی
prio_بدلا_idle(ساخت rq *rq, ساخت task_struct *p, u64 oldprio)
{
	اگر (p->prio == oldprio)
		واپس;

	BUG();
}

ساکن خالی update_curr_idle(ساخت rq *rq)
{
	ساخت sched_entity *se = &rq->idle->se;
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_آغاز;
	اگر (unlikely(delta_exec <= 0))
		واپس;

	se->exec_آغاز = now;

	dl_server_update_idle(&rq->fair_server, delta_exec);
#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
	dl_server_update_idle(&rq->ext_server, delta_exec);
#اگر_ختم
}

/*
 * Simple, special scheduling class for the per-CPU idle tasks:
 */
DEFINE_SCHED_CLASS(idle) = {
	/* no enqueue/yield_task for idle tasks */

	/* dequeue is not valid, we print a debug message there: */
	.قطار_سے_نکالو_task		= قطار_سے_نکالو_task_idle,

	.wakeup_preempt		= wakeup_preempt_idle,

	.pick_task		= pick_task_idle,
	.put_پچھلا_task		= put_پچھلا_task_idle,
	.سیٹ_اگلا_task          = سیٹ_اگلا_task_idle,

	.balance		= balance_idle,
	.select_task_rq		= select_task_rq_idle,
	.سیٹ_cpus_allowed	= سیٹ_cpus_allowed_common,

	.task_tick		= task_tick_idle,

	.prio_بدلا		= prio_بدلا_idle,
	.switching_مقصد		= switching_مقصد_idle,
	.update_curr		= update_curr_idle,
};
