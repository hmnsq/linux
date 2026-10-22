// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic entry points for the idle threads and
 * implementation of the idle task scheduling class.
 *
 * (NOTE: these are not related to SCHED_IDLE batch scheduled
 *        tasks which are handled in sched/fair.c )
 */
#அந்தர்பாவ <linux/cpuidle.h>
#அந்தர்பாவ <linux/suspend.h>
#அந்தர்பாவ <linux/livepatch.h>
#அந்தர்பாவ "sched.h"
#அந்தர்பாவ "smp.h"

/* Linker adds these: start and end of __cpuidle functions */
வெளி எழுத்து __cpuidle_text_start[], __cpuidle_text_end[];

/**
 * sched_idle_set_state - Record idle state for the current CPU.
 * @idle_state: State to record.
 */
வெறுமை sched_idle_set_state(கட்டமைப்பு cpuidle_state *idle_state)
{
	idle_set_state(this_rq(), idle_state);
}

நிலையான முழு __read_mostly cpu_idle_force_poll;

வெறுமை cpu_idle_poll_ctrl(bool enable)
{
	எனில் (enable) {
		cpu_idle_force_poll++;
	} இல்லையெனில் {
		cpu_idle_force_poll--;
		WARN_ON_ONCE(cpu_idle_force_poll < 0);
	}
}

#ifdef CONFIG_GENERIC_IDLE_POLL_SETUP
நிலையான முழு __init cpu_idle_poll_setup(எழுத்து *__unused)
{
	cpu_idle_force_poll = 1;

	திரும்பு 1;
}
__setup("nohlt", cpu_idle_poll_setup);

நிலையான முழு __init cpu_idle_nopoll_setup(எழுத்து *__unused)
{
	cpu_idle_force_poll = 0;

	திரும்பு 1;
}
__setup("hlt", cpu_idle_nopoll_setup);
#endif /* CONFIG_GENERIC_IDLE_POLL_SETUP */

நிலையான noinline முழு __cpuidle cpu_idle_poll(வெறுமை)
{
	instrumentation_begin();
	trace_cpu_idle(0, smp_processor_id());
	stop_critical_timings();
	ct_cpuidle_enter();

	raw_local_irq_enable();
	போது (!tif_need_resched() &&
	       (cpu_idle_force_poll || tick_check_broadcast_expired()))
		cpu_relax();
	raw_local_irq_disable();

	ct_cpuidle_exit();
	start_critical_timings();
	trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
	local_irq_enable();
	instrumentation_end();

	திரும்பு 1;
}

/* Weak implementations for optional arch specific functions */
வெறுமை __weak arch_cpu_idle_prepare(வெறுமை) { }
வெறுமை __weak arch_cpu_idle_enter(வெறுமை) { }
வெறுமை __weak arch_cpu_idle_exit(வெறுமை) { }
வெறுமை __weak __noreturn arch_cpu_idle_dead(வெறுமை) { போது (1); }
வெறுமை __weak arch_cpu_idle(வெறுமை)
{
	cpu_idle_force_poll = 1;
}

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE
DEFINE_STATIC_சாவி_FALSE(arch_needs_tick_broadcast);

நிலையான உள்வரி வெறுமை cond_tick_broadcast_enter(வெறுமை)
{
	எனில் (static_branch_unlikely(&arch_needs_tick_broadcast))
		tick_broadcast_enter();
}

நிலையான உள்வரி வெறுமை cond_tick_broadcast_exit(வெறுமை)
{
	எனில் (static_branch_unlikely(&arch_needs_tick_broadcast))
		tick_broadcast_exit();
}
#இல்லையெனில் /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE: */
நிலையான உள்வரி வெறுமை cond_tick_broadcast_enter(வெறுமை) { }
நிலையான உள்வரி வெறுமை cond_tick_broadcast_exit(வெறுமை) { }
#endif /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE */

/**
 * default_idle_call - Default CPU idle routine.
 *
 * To use when the cpuidle framework cannot be used.
 */
வெறுமை __cpuidle default_idle_call(வெறுமை)
{
	instrumentation_begin();
	எனில் (!current_clr_polling_and_test()) {
		cond_tick_broadcast_enter();
		trace_cpu_idle(1, smp_processor_id());
		stop_critical_timings();

		ct_cpuidle_enter();
		arch_cpu_idle();
		ct_cpuidle_exit();

		start_critical_timings();
		trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
		cond_tick_broadcast_exit();
	}
	local_irq_enable();
	instrumentation_end();
}

நிலையான முழு call_cpuidle_s2idle(கட்டமைப்பு cpuidle_driver *drv,
			       கட்டமைப்பு cpuidle_device *dev,
			       u64 max_latency_ns)
{
	எனில் (current_clr_polling_and_test())
		திரும்பு -EBUSY;

	திரும்பு cpuidle_enter_s2idle(drv, dev, max_latency_ns);
}

நிலையான முழு call_cpuidle(கட்டமைப்பு cpuidle_driver *drv, கட்டமைப்பு cpuidle_device *dev,
		      முழு next_state)
{
	/*
	 * The idle task must be scheduled, it is pointless to go to idle, just
	 * update no idle residency and return.
	 */
	எனில் (current_clr_polling_and_test()) {
		dev->last_residency_ns = 0;
		local_irq_enable();
		திரும்பு -EBUSY;
	}

	/*
	 * Enter the idle state previously returned by the governor decision.
	 * This function will block until an interrupt occurs and will take
	 * care of re-enabling the local interrupts
	 */
	திரும்பு cpuidle_enter(drv, dev, next_state);
}

நிலையான வெறுமை idle_call_stop_or_retain_tick(bool stop_tick)
{
	எனில் (stop_tick || tick_nohz_tick_stopped())
		tick_nohz_idle_stop_tick();
	இல்லையெனில்
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
நிலையான வெறுமை cpuidle_idle_call(bool stop_tick)
{
	கட்டமைப்பு cpuidle_device *dev = cpuidle_get_device();
	கட்டமைப்பு cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	முழு next_state, entered_state;

	/*
	 * Check if the idle task must be rescheduled. If it is the
	 * case, exit the function after re-enabling the local IRQ.
	 */
	எனில் (need_resched()) {
		local_irq_enable();
		திரும்பு;
	}

	எனில் (cpuidle_not_available(drv, dev)) {
		idle_call_stop_or_retain_tick(stop_tick);

		default_idle_call();
		செல் exit_idle;
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

	எனில் (idle_should_enter_s2idle() || dev->forced_idle_latency_limit_ns) {
		u64 max_latency_ns;

		எனில் (idle_should_enter_s2idle()) {
			max_latency_ns = cpu_wakeup_latency_qos_limit() *
					 NSEC_PER_USEC;

			entered_state = call_cpuidle_s2idle(drv, dev,
							    max_latency_ns);
			எனில் (entered_state > 0)
				செல் exit_idle;
		} இல்லையெனில் {
			max_latency_ns = dev->forced_idle_latency_limit_ns;
		}

		tick_nohz_idle_stop_tick();

		next_state = cpuidle_find_deepest_state(drv, dev, max_latency_ns);
		call_cpuidle(drv, dev, next_state);
	} இல்லையெனில் எனில் (drv->state_மொத்தம் > 1) {
		/*
		 * stop_tick is expected to be true by default by cpuidle
		 * governors, which allows them to select idle states with
		 * target residency above the tick period length.
		 */
		stop_tick = true;

		/*
		 * Ask the cpuidle framework to choose a convenient idle state.
		 */
		next_state = cpuidle_select(drv, dev, &stop_tick);

		idle_call_stop_or_retain_tick(stop_tick);

		entered_state = call_cpuidle(drv, dev, next_state);
		/*
		 * Give the governor an opportunity to reflect on the outcome
		 */
		cpuidle_reflect(dev, entered_state);
	} இல்லையெனில் {
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
	எனில் (WARN_ON_ONCE(irqs_disabled()))
		local_irq_enable();
}

/*
 * Generic idle loop implementation
 *
 * Called with polling cleared.
 */
நிலையான வெறுமை do_idle(வெறுமை)
{
	முழு cpu = smp_processor_id();
	bool got_tick = false;

	எனில் (cpu_is_offline(cpu)) {
		local_irq_disable();
		/* All per-CPU kernel threads should be done by now. */
		WARN_ON_ONCE(need_resched());
		cpuhp_report_idle_dead();
		arch_cpu_idle_dead();
	}

	/*
	 * Check if we need to update blocked load
	 */
	nohz_run_idle_balance(cpu);

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

	போது (!need_resched()) {

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
		எனில் (cpu_idle_force_poll || tick_check_broadcast_expired()) {
			tick_nohz_idle_restart_tick();
			cpu_idle_poll();
		} இல்லையெனில் {
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
	preempt_set_need_resched();
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
	flush_smp_call_function_queue();
	schedule_idle();

	எனில் (unlikely(klp_patch_pending(current)))
		klp_update_patch_state(current);
}

bool cpu_in_idle(unsigned long pc)
{
	திரும்பு pc >= (unsigned long)__cpuidle_text_start &&
		pc < (unsigned long)__cpuidle_text_end;
}

கட்டமைப்பு idle_timer {
	கட்டமைப்பு hrtimer timer;
	முழு done;
};

நிலையான எண்ணிக்கை hrtimer_restart idle_inject_timer_fn(கட்டமைப்பு hrtimer *timer)
{
	கட்டமைப்பு idle_timer *it = container_of(timer, கட்டமைப்பு idle_timer, timer);

	WRITE_ONCE(it->done, 1);
	set_tsk_need_resched(current);

	திரும்பு HRTIMER_NORESTART;
}

வெறுமை play_idle_precise(u64 duration_ns, u64 latency_ns)
{
	கட்டமைப்பு idle_timer it;

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
	cpuidle_use_deepest_state(latency_ns);

	it.done = 0;
	hrtimer_setup_on_அடுக்கு(&it.timer, idle_inject_timer_fn, CLOCK_MONOTONIC,
			       HRTIMER_MODE_REL_HARD);
	hrtimer_start(&it.timer, ns_to_ktime(duration_ns),
		      HRTIMER_MODE_REL_PINNED_HARD);

	போது (!READ_ONCE(it.done))
		do_idle();

	cpuidle_use_deepest_state(0);
	current->flags &= ~PF_IDLE;

	preempt_fold_need_resched();
	preempt_enable();
}
EXPORT_குறியீடு_GPL(play_idle_precise);

வெறுமை cpu_startup_entry(எண்ணிக்கை cpuhp_state state)
{
	current->flags |= PF_IDLE;
	arch_cpu_idle_prepare();
	cpuhp_online_idle(state);
	போது (1)
		do_idle();
}

/*
 * idle-task scheduling class.
 */

நிலையான முழு
select_task_rq_idle(கட்டமைப்பு task_struct *p, முழு cpu, முழு flags)
{
	திரும்பு task_cpu(p); /* IDLE tasks as never migrated */
}

நிலையான முழு
balance_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	திரும்பு WARN_ON_ONCE(1);
}

/*
 * Idle tasks are unconditionally rescheduled:
 */
நிலையான வெறுமை wakeup_preempt_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	resched_curr(rq);
}

நிலையான வெறுமை update_curr_idle(கட்டமைப்பு rq *rq);

நிலையான வெறுமை put_prev_task_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev, கட்டமைப்பு task_struct *next)
{
	update_curr_idle(rq);
	scx_update_idle(rq, false, true);
	update_rq_avg_idle(rq);
}

நிலையான வெறுமை set_next_task_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *next, எண்ணிக்கை snt_e type)
{
	எனில் (type == SNT_REPICK)
		திரும்பு;

	update_idle_core(rq);
	scx_update_idle(rq, true, true);
	schedstat_inc(rq->sched_goidle);
	next->se.exec_start = rq_clock_task(rq);

	/*
	 * rq is about to be idle, check if we need to update the
	 * lost_idle_time of clock_pelt
	 */
	update_idle_rq_clock_pelt(rq);
}

கட்டமைப்பு task_struct *pick_task_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	/*
	 * Notify scx only on an idle-to-idle re-pick (the cpu was already idle).
	 * A real task->idle transition is delivered by set_next_task_idle(), so
	 * calling here too would duplicate it.
	 */
	எனில் (scx_enabled() && is_idle_task(rq->curr))
		scx_update_idle(rq, true, false);
	திரும்பு rq->idle;
}

/*
 * It is not legal to sleep in the idle task - print a warning
 * message if some code attempts to do it:
 */
நிலையான bool
dequeue_task_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	raw_spin_rq_unlock_irq(rq);
	printk(KERN_ERR "bad: scheduling from the idle thread!\n");
	dump_அடுக்கு();
	raw_spin_rq_lock_irq(rq);
	திரும்பு true;
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
நிலையான வெறுமை task_tick_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *curr, முழு queued)
{
	update_curr_idle(rq);
}

நிலையான வெறுமை switching_to_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	BUG();
}

நிலையான வெறுமை
prio_changed_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, u64 oldprio)
{
	எனில் (p->prio == oldprio)
		திரும்பு;

	BUG();
}

நிலையான வெறுமை update_curr_idle(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு sched_entity *se = &rq->idle->se;
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_start;
	எனில் (unlikely(delta_exec <= 0))
		திரும்பு;

	se->exec_start = now;

	dl_server_update_idle(&rq->fair_server, delta_exec);
#ifdef CONFIG_SCHED_CLASS_EXT
	dl_server_update_idle(&rq->ext_server, delta_exec);
#endif
}

/*
 * Simple, special scheduling class for the per-CPU idle tasks:
 */
DEFINE_SCHED_CLASS(idle) = {
	/* no enqueue/yield_task for idle tasks */

	/* dequeue is not valid, we print a debug message there: */
	.dequeue_task		= dequeue_task_idle,

	.wakeup_preempt		= wakeup_preempt_idle,

	.pick_task		= pick_task_idle,
	.put_prev_task		= put_prev_task_idle,
	.set_next_task          = set_next_task_idle,

	.balance		= balance_idle,
	.select_task_rq		= select_task_rq_idle,
	.set_cpus_allowed	= set_cpus_allowed_common,

	.task_tick		= task_tick_idle,

	.prio_changed		= prio_changed_idle,
	.switching_to		= switching_to_idle,
	.update_curr		= update_curr_idle,
};
