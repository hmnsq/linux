// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic entry points for the idle threads and
 * implementation of the idle task scheduling class.
 *
 * (NOTE: these are not related to SCHED_IDLE batch scheduled
 *        tasks which are handled in sched/fair.c )
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpuidle.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/suspend.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/livepatch.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "smp.h"

/* Linker adds these: start and end of __cpuidle functions */
ਬਾਹਰੀ ਅੱਖਰ __cpuidle_text_start[], __cpuidle_text_end[];

/**
 * sched_idle_set_state - Record idle state for the current CPU.
 * @idle_state: State to record.
 */
ਖਾਲੀ sched_idle_ਸੈੱਟ_state(ਬਣਤਰ cpuidle_state *idle_state)
{
	idle_ਸੈੱਟ_state(this_rq(), idle_state);
}

ਸਥਿਰ ਪੂਰਨ __read_mostly cpu_idle_force_poll;

ਖਾਲੀ cpu_idle_poll_ctrl(bool enable)
{
	ਜੇ (enable) {
		cpu_idle_force_poll++;
	} ਨਹੀਂਤਾਂ {
		cpu_idle_force_poll--;
		WARN_ON_ONCE(cpu_idle_force_poll < 0);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_GENERIC_IDLE_POLL_ਤਿਆਰੀ
ਸਥਿਰ ਪੂਰਨ __init cpu_idle_poll_ਤਿਆਰੀ(ਅੱਖਰ *__unused)
{
	cpu_idle_force_poll = 1;

	ਵਾਪਸ 1;
}
__setup("nohlt", cpu_idle_poll_ਤਿਆਰੀ);

ਸਥਿਰ ਪੂਰਨ __init cpu_idle_nopoll_ਤਿਆਰੀ(ਅੱਖਰ *__unused)
{
	cpu_idle_force_poll = 0;

	ਵਾਪਸ 1;
}
__setup("hlt", cpu_idle_nopoll_ਤਿਆਰੀ);
#ਜੇ_ਅੰਤ /* CONFIG_GENERIC_IDLE_POLL_SETUP */

ਸਥਿਰ noinline ਪੂਰਨ __cpuidle cpu_idle_poll(ਖਾਲੀ)
{
	instrumentation_begin();
	trace_cpu_idle(0, smp_processor_id());
	stop_critical_timings();
	ct_cpuidle_enter();

	raw_local_irq_enable();
	ਜਦੋਂ (!tif_need_resched() &&
	       (cpu_idle_force_poll || tick_check_ਪ੍ਰਸਾਰਣ_expired()))
		cpu_relax();
	raw_local_irq_disable();

	ct_cpuidle_exit();
	ਸ਼ੁਰੂ_critical_timings();
	trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
	local_irq_enable();
	instrumentation_ਸਿਰਾ();

	ਵਾਪਸ 1;
}

/* Weak implementations for optional arch specific functions */
ਖਾਲੀ __weak arch_cpu_idle_prepare(ਖਾਲੀ) { }
ਖਾਲੀ __weak arch_cpu_idle_enter(ਖਾਲੀ) { }
ਖਾਲੀ __weak arch_cpu_idle_exit(ਖਾਲੀ) { }
ਖਾਲੀ __weak __noreturn arch_cpu_idle_dead(ਖਾਲੀ) { ਜਦੋਂ (1); }
ਖਾਲੀ __weak arch_cpu_idle(ਖਾਲੀ)
{
	cpu_idle_force_poll = 1;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_GENERIC_CLOCKEVENTS_ਪ੍ਰਸਾਰਣ_IDLE
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(arch_needs_tick_ਪ੍ਰਸਾਰਣ);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cond_tick_ਪ੍ਰਸਾਰਣ_enter(ਖਾਲੀ)
{
	ਜੇ (static_branch_unlikely(&arch_needs_tick_ਪ੍ਰਸਾਰਣ))
		tick_ਪ੍ਰਸਾਰਣ_enter();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cond_tick_ਪ੍ਰਸਾਰਣ_exit(ਖਾਲੀ)
{
	ਜੇ (static_branch_unlikely(&arch_needs_tick_ਪ੍ਰਸਾਰਣ))
		tick_ਪ੍ਰਸਾਰਣ_exit();
}
#ਨਹੀਂਤਾਂ /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cond_tick_ਪ੍ਰਸਾਰਣ_enter(ਖਾਲੀ) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cond_tick_ਪ੍ਰਸਾਰਣ_exit(ਖਾਲੀ) { }
#ਜੇ_ਅੰਤ /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE */

/**
 * default_idle_call - Default CPU idle routine.
 *
 * To use when the cpuidle framework cannot be used.
 */
ਖਾਲੀ __cpuidle default_idle_call(ਖਾਲੀ)
{
	instrumentation_begin();
	ਜੇ (!current_clr_polling_and_test()) {
		cond_tick_ਪ੍ਰਸਾਰਣ_enter();
		trace_cpu_idle(1, smp_processor_id());
		stop_critical_timings();

		ct_cpuidle_enter();
		arch_cpu_idle();
		ct_cpuidle_exit();

		ਸ਼ੁਰੂ_critical_timings();
		trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
		cond_tick_ਪ੍ਰਸਾਰਣ_exit();
	}
	local_irq_enable();
	instrumentation_ਸਿਰਾ();
}

ਸਥਿਰ ਪੂਰਨ call_cpuidle_s2idle(ਬਣਤਰ cpuidle_driver *drv,
			       ਬਣਤਰ cpuidle_device *dev,
			       u64 max_latency_ns)
{
	ਜੇ (current_clr_polling_and_test())
		ਵਾਪਸ -EBUSY;

	ਵਾਪਸ cpuidle_enter_s2idle(drv, dev, max_latency_ns);
}

ਸਥਿਰ ਪੂਰਨ call_cpuidle(ਬਣਤਰ cpuidle_driver *drv, ਬਣਤਰ cpuidle_device *dev,
		      ਪੂਰਨ ਅਗਲਾ_state)
{
	/*
	 * The idle task must be scheduled, it is pointless to go to idle, just
	 * update no idle residency and return.
	 */
	ਜੇ (current_clr_polling_and_test()) {
		dev->ਆਖਰੀ_residency_ns = 0;
		local_irq_enable();
		ਵਾਪਸ -EBUSY;
	}

	/*
	 * Enter the idle state previously returned by the governor decision.
	 * This function will block until an interrupt occurs and will take
	 * care of re-enabling the local interrupts
	 */
	ਵਾਪਸ cpuidle_enter(drv, dev, ਅਗਲਾ_state);
}

ਸਥਿਰ ਖਾਲੀ idle_call_stop_or_retain_tick(bool stop_tick)
{
	ਜੇ (stop_tick || tick_nohz_tick_stopped())
		tick_nohz_idle_stop_tick();
	ਨਹੀਂਤਾਂ
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
ਸਥਿਰ ਖਾਲੀ cpuidle_idle_call(bool stop_tick)
{
	ਬਣਤਰ cpuidle_device *dev = cpuidle_get_device();
	ਬਣਤਰ cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	ਪੂਰਨ ਅਗਲਾ_state, entered_state;

	/*
	 * Check if the idle task must be rescheduled. If it is the
	 * case, exit the function after re-enabling the local IRQ.
	 */
	ਜੇ (need_resched()) {
		local_irq_enable();
		ਵਾਪਸ;
	}

	ਜੇ (cpuidle_not_ਉਪਲਬਧ(drv, dev)) {
		idle_call_stop_or_retain_tick(stop_tick);

		default_idle_call();
		ਜਾਓ exit_idle;
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

	ਜੇ (idle_should_enter_s2idle() || dev->forced_idle_latency_ਹੱਦਬੰਦੀ_ns) {
		u64 max_latency_ns;

		ਜੇ (idle_should_enter_s2idle()) {
			max_latency_ns = cpu_wakeup_latency_qos_ਹੱਦਬੰਦੀ() *
					 NSEC_ਪ੍ਰਤੀ_USEC;

			entered_state = call_cpuidle_s2idle(drv, dev,
							    max_latency_ns);
			ਜੇ (entered_state > 0)
				ਜਾਓ exit_idle;
		} ਨਹੀਂਤਾਂ {
			max_latency_ns = dev->forced_idle_latency_ਹੱਦਬੰਦੀ_ns;
		}

		tick_nohz_idle_stop_tick();

		ਅਗਲਾ_state = cpuidle_ਲੱਭੋ_ਸਭ_ਤੋਂ_ਡੂੰਘਾ_state(drv, dev, max_latency_ns);
		call_cpuidle(drv, dev, ਅਗਲਾ_state);
	} ਨਹੀਂਤਾਂ ਜੇ (drv->state_ਗਿਣਤੀ > 1) {
		/*
		 * stop_tick is expected to be true by default by cpuidle
		 * governors, which allows them to select idle states with
		 * target residency above the tick period length.
		 */
		stop_tick = true;

		/*
		 * Ask the cpuidle framework to choose a convenient idle state.
		 */
		ਅਗਲਾ_state = cpuidle_select(drv, dev, &stop_tick);

		idle_call_stop_or_retain_tick(stop_tick);

		entered_state = call_cpuidle(drv, dev, ਅਗਲਾ_state);
		/*
		 * Give the governor an opportunity to reflect on the outcome
		 */
		cpuidle_reflect(dev, entered_state);
	} ਨਹੀਂਤਾਂ {
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
	ਜੇ (WARN_ON_ONCE(irqs_disabled()))
		local_irq_enable();
}

/*
 * Generic idle loop implementation
 *
 * Called with polling cleared.
 */
ਸਥਿਰ ਖਾਲੀ do_idle(ਖਾਲੀ)
{
	ਪੂਰਨ cpu = smp_processor_id();
	bool got_tick = false;

	ਜੇ (cpu_is_offline(cpu)) {
		local_irq_disable();
		/* All per-CPU kernel threads should be done by now. */
		WARN_ON_ONCE(need_resched());
		cpuhp_report_idle_dead();
		arch_cpu_idle_dead();
	}

	/*
	 * Check if we need to update blocked load
	 */
	nohz_ਲੜੀਵਾਰ_idle_balance(cpu);

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

	ਜਦੋਂ (!need_resched()) {

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
		ਜੇ (cpu_idle_force_poll || tick_check_ਪ੍ਰਸਾਰਣ_expired()) {
			tick_nohz_idle_restart_tick();
			cpu_idle_poll();
		} ਨਹੀਂਤਾਂ {
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
	preempt_ਸੈੱਟ_need_resched();
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
	flush_smp_call_function_ਕਤਾਰ();
	ਸਮਾਂ_ਸਾਰਣੀ_idle();

	ਜੇ (unlikely(klp_patch_pending(current)))
		klp_update_patch_state(current);
}

bool cpu_ਵਿੱਚ_idle(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ pc)
{
	ਵਾਪਸ pc >= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)__cpuidle_text_start &&
		pc < (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)__cpuidle_text_end;
}

ਬਣਤਰ idle_timer {
	ਬਣਤਰ hrtimer timer;
	ਪੂਰਨ ਪੂਰਾ;
};

ਸਥਿਰ ਗਿਣਤੀ hrtimer_restart idle_inject_timer_fn(ਬਣਤਰ hrtimer *timer)
{
	ਬਣਤਰ idle_timer *it = container_ਦਾ(timer, ਬਣਤਰ idle_timer, timer);

	WRITE_ONCE(it->ਪੂਰਾ, 1);
	ਸੈੱਟ_tsk_need_resched(current);

	ਵਾਪਸ HRTIMER_NORESTART;
}

ਖਾਲੀ play_idle_precise(u64 duration_ns, u64 latency_ns)
{
	ਬਣਤਰ idle_timer it;

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
	cpuidle_use_ਸਭ_ਤੋਂ_ਡੂੰਘਾ_state(latency_ns);

	it.ਪੂਰਾ = 0;
	hrtimer_ਤਿਆਰੀ_on_ਢੇਰ(&it.timer, idle_inject_timer_fn, CLOCK_MONOTONIC,
			       HRTIMER_ਬਹੁਲਕ_REL_HARD);
	hrtimer_ਸ਼ੁਰੂ(&it.timer, ns_ਮੰਜ਼ਿਲ_ktime(duration_ns),
		      HRTIMER_ਬਹੁਲਕ_REL_PINNED_HARD);

	ਜਦੋਂ (!READ_ONCE(it.ਪੂਰਾ))
		do_idle();

	cpuidle_use_ਸਭ_ਤੋਂ_ਡੂੰਘਾ_state(0);
	current->flags &= ~PF_IDLE;

	preempt_fold_need_resched();
	preempt_enable();
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(play_idle_precise);

ਖਾਲੀ cpu_startup_ਇੰਦਰਾਜ(ਗਿਣਤੀ cpuhp_state state)
{
	current->flags |= PF_IDLE;
	arch_cpu_idle_prepare();
	cpuhp_online_idle(state);
	ਜਦੋਂ (1)
		do_idle();
}

/*
 * idle-task scheduling class.
 */

ਸਥਿਰ ਪੂਰਨ
select_task_rq_idle(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ flags)
{
	ਵਾਪਸ task_cpu(p); /* IDLE tasks as never migrated */
}

ਸਥਿਰ ਪੂਰਨ
balance_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਵਾਪਸ WARN_ON_ONCE(1);
}

/*
 * Idle tasks are unconditionally rescheduled:
 */
ਸਥਿਰ ਖਾਲੀ wakeup_preempt_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	resched_curr(rq);
}

ਸਥਿਰ ਖਾਲੀ update_curr_idle(ਬਣਤਰ rq *rq);

ਸਥਿਰ ਖਾਲੀ put_prev_task_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	update_curr_idle(rq);
	scx_update_idle(rq, false, true);
	update_rq_avg_idle(rq);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਅਗਲਾ_task_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *ਅਗਲਾ, ਗਿਣਤੀ snt_e type)
{
	ਜੇ (type == SNT_REPICK)
		ਵਾਪਸ;

	update_idle_core(rq);
	scx_update_idle(rq, true, true);
	schedstat_inc(rq->sched_goidle);
	ਅਗਲਾ->se.exec_ਸ਼ੁਰੂ = rq_clock_task(rq);

	/*
	 * rq is about to be idle, check if we need to update the
	 * lost_idle_time of clock_pelt
	 */
	update_idle_rq_clock_pelt(rq);
}

ਬਣਤਰ task_struct *pick_task_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	/*
	 * Notify scx only on an idle-to-idle re-pick (the cpu was already idle).
	 * A real task->idle transition is delivered by set_next_task_idle(), so
	 * calling here too would duplicate it.
	 */
	ਜੇ (scx_enabled() && is_idle_task(rq->curr))
		scx_update_idle(rq, true, false);
	ਵਾਪਸ rq->idle;
}

/*
 * It is not legal to sleep in the idle task - print a warning
 * message if some code attempts to do it:
 */
ਸਥਿਰ bool
ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	raw_spin_rq_unlock_irq(rq);
	printk(KERN_ERR "bad: scheduling from the idle thread!\n");
	dump_ਢੇਰ();
	raw_spin_rq_ਤਾਲਾ_irq(rq);
	ਵਾਪਸ true;
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ਸਥਿਰ ਖਾਲੀ task_tick_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr, ਪੂਰਨ queued)
{
	update_curr_idle(rq);
}

ਸਥਿਰ ਖਾਲੀ switching_ਮੰਜ਼ਿਲ_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	BUG();
}

ਸਥਿਰ ਖਾਲੀ
prio_ਬਦਲਿਆ_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 oldprio)
{
	ਜੇ (p->prio == oldprio)
		ਵਾਪਸ;

	BUG();
}

ਸਥਿਰ ਖਾਲੀ update_curr_idle(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ sched_entity *se = &rq->idle->se;
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_ਸ਼ੁਰੂ;
	ਜੇ (unlikely(delta_exec <= 0))
		ਵਾਪਸ;

	se->exec_ਸ਼ੁਰੂ = now;

	dl_server_update_idle(&rq->fair_server, delta_exec);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	dl_server_update_idle(&rq->ext_server, delta_exec);
#ਜੇ_ਅੰਤ
}

/*
 * Simple, special scheduling class for the per-CPU idle tasks:
 */
DEFINE_SCHED_CLASS(idle) = {
	/* no enqueue/yield_task for idle tasks */

	/* dequeue is not valid, we print a debug message there: */
	.ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task		= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_idle,

	.wakeup_preempt		= wakeup_preempt_idle,

	.pick_task		= pick_task_idle,
	.put_prev_task		= put_prev_task_idle,
	.ਸੈੱਟ_ਅਗਲਾ_task          = ਸੈੱਟ_ਅਗਲਾ_task_idle,

	.balance		= balance_idle,
	.select_task_rq		= select_task_rq_idle,
	.ਸੈੱਟ_cpus_allowed	= ਸੈੱਟ_cpus_allowed_common,

	.task_tick		= task_tick_idle,

	.prio_ਬਦਲਿਆ		= prio_ਬਦਲਿਆ_idle,
	.switching_ਮੰਜ਼ਿਲ		= switching_ਮੰਜ਼ਿਲ_idle,
	.update_curr		= update_curr_idle,
};
