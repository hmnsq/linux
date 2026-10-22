// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic entry points for the idle threads and
 * implementation of the idle task scheduling class.
 *
 * (NOTE: these are not related to SCHED_IDLE batch scheduled
 *        tasks which are handled in sched/fair.c )
 */
#ಅನ್ತರ್ಭಾವ <linux/cpuidle.h>
#ಅನ್ತರ್ಭಾವ <linux/suspend.h>
#ಅನ್ತರ್ಭಾವ <linux/livepatch.h>
#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "smp.h"

/* Linker adds these: start and end of __cpuidle functions */
ಬಾಹ್ಯ ಅಕ್ಷರ __cpuidle_text_start[], __cpuidle_text_end[];

/**
 * sched_idle_set_state - Record idle state for the current CPU.
 * @idle_state: State to record.
 */
ಶೂನ್ಯ sched_idle_ಗಣ_state(ರಚನೆ cpuidle_state *idle_state)
{
	idle_ಗಣ_state(this_rq(), idle_state);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __read_mostly cpu_idle_force_poll;

ಶೂನ್ಯ cpu_idle_poll_ctrl(bool enable)
{
	ಆದರೆ (enable) {
		cpu_idle_force_poll++;
	} ಇಲ್ಲದಿದ್ದರೆ {
		cpu_idle_force_poll--;
		WARN_ON_ONCE(cpu_idle_force_poll < 0);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_GENERIC_IDLE_POLL_ಸಿದ್ಧತೆ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init cpu_idle_poll_ಸಿದ್ಧತೆ(ಅಕ್ಷರ *__unused)
{
	cpu_idle_force_poll = 1;

	ಹಿಂತಿರುಗಿ 1;
}
__setup("nohlt", cpu_idle_poll_ಸಿದ್ಧತೆ);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init cpu_idle_nopoll_ಸಿದ್ಧತೆ(ಅಕ್ಷರ *__unused)
{
	cpu_idle_force_poll = 0;

	ಹಿಂತಿರುಗಿ 1;
}
__setup("hlt", cpu_idle_nopoll_ಸಿದ್ಧತೆ);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_GENERIC_IDLE_POLL_SETUP */

ಸ್ಥಿರ noinline ಪೂರ್ಣಾಂಕ __cpuidle cpu_idle_poll(ಶೂನ್ಯ)
{
	instrumentation_begin();
	trace_cpu_idle(0, smp_processor_id());
	stop_critical_timings();
	ct_cpuidle_enter();

	raw_local_irq_enable();
	ಆಗ (!tif_need_resched() &&
	       (cpu_idle_force_poll || tick_check_ಪ್ರಸಾರ_expired()))
		cpu_relax();
	raw_local_irq_disable();

	ct_cpuidle_exit();
	ಆರಂಭ_critical_timings();
	trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
	local_irq_enable();
	instrumentation_ಕೊನೆ();

	ಹಿಂತಿರುಗಿ 1;
}

/* Weak implementations for optional arch specific functions */
ಶೂನ್ಯ __weak arch_cpu_idle_prepare(ಶೂನ್ಯ) { }
ಶೂನ್ಯ __weak arch_cpu_idle_enter(ಶೂನ್ಯ) { }
ಶೂನ್ಯ __weak arch_cpu_idle_exit(ಶೂನ್ಯ) { }
ಶೂನ್ಯ __weak __noreturn arch_cpu_idle_dead(ಶೂನ್ಯ) { ಆಗ (1); }
ಶೂನ್ಯ __weak arch_cpu_idle(ಶೂನ್ಯ)
{
	cpu_idle_force_poll = 1;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_GENERIC_CLOCKEVENTS_ಪ್ರಸಾರ_IDLE
DEFINE_STATIC_ಕೀಲಿ_FALSE(arch_needs_tick_ಪ್ರಸಾರ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cond_tick_ಪ್ರಸಾರ_enter(ಶೂನ್ಯ)
{
	ಆದರೆ (static_branch_unlikely(&arch_needs_tick_ಪ್ರಸಾರ))
		tick_ಪ್ರಸಾರ_enter();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cond_tick_ಪ್ರಸಾರ_exit(ಶೂನ್ಯ)
{
	ಆದರೆ (static_branch_unlikely(&arch_needs_tick_ಪ್ರಸಾರ))
		tick_ಪ್ರಸಾರ_exit();
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cond_tick_ಪ್ರಸಾರ_enter(ಶೂನ್ಯ) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cond_tick_ಪ್ರಸಾರ_exit(ಶೂನ್ಯ) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE */

/**
 * default_idle_call - Default CPU idle routine.
 *
 * To use when the cpuidle framework cannot be used.
 */
ಶೂನ್ಯ __cpuidle default_idle_call(ಶೂನ್ಯ)
{
	instrumentation_begin();
	ಆದರೆ (!current_clr_polling_and_test()) {
		cond_tick_ಪ್ರಸಾರ_enter();
		trace_cpu_idle(1, smp_processor_id());
		stop_critical_timings();

		ct_cpuidle_enter();
		arch_cpu_idle();
		ct_cpuidle_exit();

		ಆರಂಭ_critical_timings();
		trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
		cond_tick_ಪ್ರಸಾರ_exit();
	}
	local_irq_enable();
	instrumentation_ಕೊನೆ();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ call_cpuidle_s2idle(ರಚನೆ cpuidle_driver *drv,
			       ರಚನೆ cpuidle_device *dev,
			       u64 max_latency_ns)
{
	ಆದರೆ (current_clr_polling_and_test())
		ಹಿಂತಿರುಗಿ -EBUSY;

	ಹಿಂತಿರುಗಿ cpuidle_enter_s2idle(drv, dev, max_latency_ns);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ call_cpuidle(ರಚನೆ cpuidle_driver *drv, ರಚನೆ cpuidle_device *dev,
		      ಪೂರ್ಣಾಂಕ ಮುಂದಿನ_state)
{
	/*
	 * The idle task must be scheduled, it is pointless to go to idle, just
	 * update no idle residency and return.
	 */
	ಆದರೆ (current_clr_polling_and_test()) {
		dev->ಕೊನೆಯ_residency_ns = 0;
		local_irq_enable();
		ಹಿಂತಿರುಗಿ -EBUSY;
	}

	/*
	 * Enter the idle state previously returned by the governor decision.
	 * This function will block until an interrupt occurs and will take
	 * care of re-enabling the local interrupts
	 */
	ಹಿಂತಿರುಗಿ cpuidle_enter(drv, dev, ಮುಂದಿನ_state);
}

ಸ್ಥಿರ ಶೂನ್ಯ idle_call_stop_or_retain_tick(bool stop_tick)
{
	ಆದರೆ (stop_tick || tick_nohz_tick_stopped())
		tick_nohz_idle_stop_tick();
	ಇಲ್ಲದಿದ್ದರೆ
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
ಸ್ಥಿರ ಶೂನ್ಯ cpuidle_idle_call(bool stop_tick)
{
	ರಚನೆ cpuidle_device *dev = cpuidle_get_device();
	ರಚನೆ cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	ಪೂರ್ಣಾಂಕ ಮುಂದಿನ_state, entered_state;

	/*
	 * Check if the idle task must be rescheduled. If it is the
	 * case, exit the function after re-enabling the local IRQ.
	 */
	ಆದರೆ (need_resched()) {
		local_irq_enable();
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (cpuidle_not_ಲಭ್ಯ(drv, dev)) {
		idle_call_stop_or_retain_tick(stop_tick);

		default_idle_call();
		ಹೋಗು exit_idle;
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

	ಆದರೆ (idle_should_enter_s2idle() || dev->forced_idle_latency_ಎಲ್ಲೆ_ns) {
		u64 max_latency_ns;

		ಆದರೆ (idle_should_enter_s2idle()) {
			max_latency_ns = cpu_wakeup_latency_qos_ಎಲ್ಲೆ() *
					 NSEC_ಪ್ರತಿ_USEC;

			entered_state = call_cpuidle_s2idle(drv, dev,
							    max_latency_ns);
			ಆದರೆ (entered_state > 0)
				ಹೋಗು exit_idle;
		} ಇಲ್ಲದಿದ್ದರೆ {
			max_latency_ns = dev->forced_idle_latency_ಎಲ್ಲೆ_ns;
		}

		tick_nohz_idle_stop_tick();

		ಮುಂದಿನ_state = cpuidle_ಕಂಡುಹಿಡಿ_ಅತ್ಯಂತಆಳ_state(drv, dev, max_latency_ns);
		call_cpuidle(drv, dev, ಮುಂದಿನ_state);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (drv->state_ಎಣಿಕೆ > 1) {
		/*
		 * stop_tick is expected to be true by default by cpuidle
		 * governors, which allows them to select idle states with
		 * target residency above the tick period length.
		 */
		stop_tick = true;

		/*
		 * Ask the cpuidle framework to choose a convenient idle state.
		 */
		ಮುಂದಿನ_state = cpuidle_select(drv, dev, &stop_tick);

		idle_call_stop_or_retain_tick(stop_tick);

		entered_state = call_cpuidle(drv, dev, ಮುಂದಿನ_state);
		/*
		 * Give the governor an opportunity to reflect on the outcome
		 */
		cpuidle_reflect(dev, entered_state);
	} ಇಲ್ಲದಿದ್ದರೆ {
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
	ಆದರೆ (WARN_ON_ONCE(irqs_disabled()))
		local_irq_enable();
}

/*
 * Generic idle loop implementation
 *
 * Called with polling cleared.
 */
ಸ್ಥಿರ ಶೂನ್ಯ do_idle(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ cpu = smp_processor_id();
	bool got_tick = false;

	ಆದರೆ (cpu_is_offline(cpu)) {
		local_irq_disable();
		/* All per-CPU kernel threads should be done by now. */
		WARN_ON_ONCE(need_resched());
		cpuhp_report_idle_dead();
		arch_cpu_idle_dead();
	}

	/*
	 * Check if we need to update blocked load
	 */
	nohz_ಸತತ_idle_balance(cpu);

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

	ಆಗ (!need_resched()) {

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
		ಆದರೆ (cpu_idle_force_poll || tick_check_ಪ್ರಸಾರ_expired()) {
			tick_nohz_idle_restart_tick();
			cpu_idle_poll();
		} ಇಲ್ಲದಿದ್ದರೆ {
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
	preempt_ಗಣ_need_resched();
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
	flush_smp_call_function_ಸರತಿ();
	ವೇಳಾಪಟ್ಟಿ_idle();

	ಆದರೆ (unlikely(klp_patch_pending(current)))
		klp_update_patch_state(current);
}

bool cpu_ಒಳ_idle(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ pc)
{
	ಹಿಂತಿರುಗಿ pc >= (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)__cpuidle_text_start &&
		pc < (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)__cpuidle_text_end;
}

ರಚನೆ idle_timer {
	ರಚನೆ hrtimer timer;
	ಪೂರ್ಣಾಂಕ ಮುಗಿದ;
};

ಸ್ಥಿರ ಎಣಿಕೆ hrtimer_restart idle_inject_timer_fn(ರಚನೆ hrtimer *timer)
{
	ರಚನೆ idle_timer *it = container_ದ(timer, ರಚನೆ idle_timer, timer);

	WRITE_ONCE(it->ಮುಗಿದ, 1);
	ಗಣ_tsk_need_resched(current);

	ಹಿಂತಿರುಗಿ HRTIMER_NORESTART;
}

ಶೂನ್ಯ play_idle_precise(u64 duration_ns, u64 latency_ns)
{
	ರಚನೆ idle_timer it;

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
	cpuidle_use_ಅತ್ಯಂತಆಳ_state(latency_ns);

	it.ಮುಗಿದ = 0;
	hrtimer_ಸಿದ್ಧತೆ_on_ರಾಶಿ(&it.timer, idle_inject_timer_fn, CLOCK_MONOTONIC,
			       HRTIMER_ಬಹುಲಕ_REL_HARD);
	hrtimer_ಆರಂಭ(&it.timer, ns_ಗಮ್ಯ_ktime(duration_ns),
		      HRTIMER_ಬಹುಲಕ_REL_PINNED_HARD);

	ಆಗ (!READ_ONCE(it.ಮುಗಿದ))
		do_idle();

	cpuidle_use_ಅತ್ಯಂತಆಳ_state(0);
	current->flags &= ~PF_IDLE;

	preempt_fold_need_resched();
	preempt_enable();
}
EXPORT_ಚಿಹ್ನೆ_GPL(play_idle_precise);

ಶೂನ್ಯ cpu_startup_ನಮೂದು(ಎಣಿಕೆ cpuhp_state state)
{
	current->flags |= PF_IDLE;
	arch_cpu_idle_prepare();
	cpuhp_online_idle(state);
	ಆಗ (1)
		do_idle();
}

/*
 * idle-task scheduling class.
 */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
select_task_rq_idle(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ flags)
{
	ಹಿಂತಿರುಗಿ task_cpu(p); /* IDLE tasks as never migrated */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
balance_idle(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಹಿಂತಿರುಗಿ WARN_ON_ONCE(1);
}

/*
 * Idle tasks are unconditionally rescheduled:
 */
ಸ್ಥಿರ ಶೂನ್ಯ wakeup_preempt_idle(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	resched_curr(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_curr_idle(ರಚನೆ rq *rq);

ಸ್ಥಿರ ಶೂನ್ಯ put_prev_task_idle(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ)
{
	update_curr_idle(rq);
	scx_update_idle(rq, false, true);
	update_rq_avg_idle(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಮುಂದಿನ_task_idle(ರಚನೆ rq *rq, ರಚನೆ task_struct *ಮುಂದಿನ, ಎಣಿಕೆ snt_e type)
{
	ಆದರೆ (type == SNT_REPICK)
		ಹಿಂತಿರುಗಿ;

	update_idle_core(rq);
	scx_update_idle(rq, true, true);
	schedstat_inc(rq->sched_goidle);
	ಮುಂದಿನ->se.exec_ಆರಂಭ = rq_clock_task(rq);

	/*
	 * rq is about to be idle, check if we need to update the
	 * lost_idle_time of clock_pelt
	 */
	update_idle_rq_clock_pelt(rq);
}

ರಚನೆ task_struct *pick_task_idle(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	/*
	 * Notify scx only on an idle-to-idle re-pick (the cpu was already idle).
	 * A real task->idle transition is delivered by set_next_task_idle(), so
	 * calling here too would duplicate it.
	 */
	ಆದರೆ (scx_enabled() && is_idle_task(rq->curr))
		scx_update_idle(rq, true, false);
	ಹಿಂತಿರುಗಿ rq->idle;
}

/*
 * It is not legal to sleep in the idle task - print a warning
 * message if some code attempts to do it:
 */
ಸ್ಥಿರ bool
ಸರತಿಯಿಂದ_ತೆಗೆ_task_idle(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	raw_spin_rq_unlock_irq(rq);
	printk(KERN_ERR "bad: scheduling from the idle thread!\n");
	dump_ರಾಶಿ();
	raw_spin_rq_ಬೀಗ_irq(rq);
	ಹಿಂತಿರುಗಿ true;
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_tick_idle(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr, ಪೂರ್ಣಾಂಕ queued)
{
	update_curr_idle(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ switching_ಗಮ್ಯ_idle(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	BUG();
}

ಸ್ಥಿರ ಶೂನ್ಯ
prio_ಬದಲಾಯಿತು_idle(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 oldprio)
{
	ಆದರೆ (p->prio == oldprio)
		ಹಿಂತಿರುಗಿ;

	BUG();
}

ಸ್ಥಿರ ಶೂನ್ಯ update_curr_idle(ರಚನೆ rq *rq)
{
	ರಚನೆ sched_entity *se = &rq->idle->se;
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_ಆರಂಭ;
	ಆದರೆ (unlikely(delta_exec <= 0))
		ಹಿಂತಿರುಗಿ;

	se->exec_ಆರಂಭ = now;

	dl_server_update_idle(&rq->fair_server, delta_exec);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	dl_server_update_idle(&rq->ext_server, delta_exec);
#ಆದರೆ_ಅಂತ್ಯ
}

/*
 * Simple, special scheduling class for the per-CPU idle tasks:
 */
DEFINE_SCHED_CLASS(idle) = {
	/* no enqueue/yield_task for idle tasks */

	/* dequeue is not valid, we print a debug message there: */
	.ಸರತಿಯಿಂದ_ತೆಗೆ_task		= ಸರತಿಯಿಂದ_ತೆಗೆ_task_idle,

	.wakeup_preempt		= wakeup_preempt_idle,

	.pick_task		= pick_task_idle,
	.put_prev_task		= put_prev_task_idle,
	.ಗಣ_ಮುಂದಿನ_task          = ಗಣ_ಮುಂದಿನ_task_idle,

	.balance		= balance_idle,
	.select_task_rq		= select_task_rq_idle,
	.ಗಣ_cpus_allowed	= ಗಣ_cpus_allowed_common,

	.task_tick		= task_tick_idle,

	.prio_ಬದಲಾಯಿತು		= prio_ಬದಲಾಯಿತು_idle,
	.switching_ಗಮ್ಯ		= switching_ಗಮ್ಯ_idle,
	.update_curr		= update_curr_idle,
};
