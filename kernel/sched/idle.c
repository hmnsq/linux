// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic entry points for the idle threads and
 * implementation of the idle task scheduling class.
 *
 * (NOTE: these are not related to SCHED_IDLE batch scheduled
 *        tasks which are handled in sched/fair.c )
 */
#अन्तर्भाव <linux/cpuidle.h>
#अन्तर्भाव <linux/suspend.h>
#अन्तर्भाव <linux/livepatch.h>
#अन्तर्भाव "sched.h"
#अन्तर्भाव "smp.h"

/* Linker adds these: start and end of __cpuidle functions */
बाह्य वर्ण __cpuidle_text_start[], __cpuidle_text_end[];

/**
 * sched_idle_set_state - Record idle state for the current CPU.
 * @idle_state: State to record.
 */
शून्य sched_idle_set_state(संरचना cpuidle_state *idle_state)
{
	idle_set_state(this_rq(), idle_state);
}

स्थिर पूर्णाङ्क __read_mostly cpu_idle_force_poll;

शून्य cpu_idle_poll_ctrl(bool enable)
{
	यदि (enable) {
		cpu_idle_force_poll++;
	} अन्यथा {
		cpu_idle_force_poll--;
		WARN_ON_ONCE(cpu_idle_force_poll < 0);
	}
}

#ifdef CONFIG_GENERIC_IDLE_POLL_SETUP
स्थिर पूर्णाङ्क __init cpu_idle_poll_setup(वर्ण *__unused)
{
	cpu_idle_force_poll = 1;

	प्रत्यागम 1;
}
__setup("nohlt", cpu_idle_poll_setup);

स्थिर पूर्णाङ्क __init cpu_idle_nopoll_setup(वर्ण *__unused)
{
	cpu_idle_force_poll = 0;

	प्रत्यागम 1;
}
__setup("hlt", cpu_idle_nopoll_setup);
#endif /* CONFIG_GENERIC_IDLE_POLL_SETUP */

स्थिर noinline पूर्णाङ्क __cpuidle cpu_idle_poll(शून्य)
{
	instrumentation_begin();
	trace_cpu_idle(0, smp_processor_id());
	stop_critical_timings();
	ct_cpuidle_enter();

	raw_local_irq_enable();
	यावत् (!tif_need_resched() &&
	       (cpu_idle_force_poll || tick_check_broadcast_expired()))
		cpu_relax();
	raw_local_irq_disable();

	ct_cpuidle_exit();
	start_critical_timings();
	trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
	local_irq_enable();
	instrumentation_end();

	प्रत्यागम 1;
}

/* Weak implementations for optional arch specific functions */
शून्य __weak arch_cpu_idle_prepare(शून्य) { }
शून्य __weak arch_cpu_idle_enter(शून्य) { }
शून्य __weak arch_cpu_idle_exit(शून्य) { }
शून्य __weak __noreturn arch_cpu_idle_dead(शून्य) { यावत् (1); }
शून्य __weak arch_cpu_idle(शून्य)
{
	cpu_idle_force_poll = 1;
}

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE
DEFINE_STATIC_कुंजी_FALSE(arch_needs_tick_broadcast);

स्थिर अन्तःस्थ शून्य cond_tick_broadcast_enter(शून्य)
{
	यदि (static_branch_unlikely(&arch_needs_tick_broadcast))
		tick_broadcast_enter();
}

स्थिर अन्तःस्थ शून्य cond_tick_broadcast_exit(शून्य)
{
	यदि (static_branch_unlikely(&arch_needs_tick_broadcast))
		tick_broadcast_exit();
}
#अन्यथा /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE: */
स्थिर अन्तःस्थ शून्य cond_tick_broadcast_enter(शून्य) { }
स्थिर अन्तःस्थ शून्य cond_tick_broadcast_exit(शून्य) { }
#endif /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE */

/**
 * default_idle_call - Default CPU idle routine.
 *
 * To use when the cpuidle framework cannot be used.
 */
शून्य __cpuidle default_idle_call(शून्य)
{
	instrumentation_begin();
	यदि (!current_clr_polling_and_test()) {
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

स्थिर पूर्णाङ्क call_cpuidle_s2idle(संरचना cpuidle_driver *drv,
			       संरचना cpuidle_device *dev,
			       u64 max_latency_ns)
{
	यदि (current_clr_polling_and_test())
		प्रत्यागम -EBUSY;

	प्रत्यागम cpuidle_enter_s2idle(drv, dev, max_latency_ns);
}

स्थिर पूर्णाङ्क call_cpuidle(संरचना cpuidle_driver *drv, संरचना cpuidle_device *dev,
		      पूर्णाङ्क next_state)
{
	/*
	 * The idle task must be scheduled, it is pointless to go to idle, just
	 * update no idle residency and return.
	 */
	यदि (current_clr_polling_and_test()) {
		dev->last_residency_ns = 0;
		local_irq_enable();
		प्रत्यागम -EBUSY;
	}

	/*
	 * Enter the idle state previously returned by the governor decision.
	 * This function will block until an interrupt occurs and will take
	 * care of re-enabling the local interrupts
	 */
	प्रत्यागम cpuidle_enter(drv, dev, next_state);
}

स्थिर शून्य idle_call_stop_or_retain_tick(bool stop_tick)
{
	यदि (stop_tick || tick_nohz_tick_stopped())
		tick_nohz_idle_stop_tick();
	अन्यथा
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
स्थिर शून्य cpuidle_idle_call(bool stop_tick)
{
	संरचना cpuidle_device *dev = cpuidle_get_device();
	संरचना cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	पूर्णाङ्क next_state, entered_state;

	/*
	 * Check if the idle task must be rescheduled. If it is the
	 * case, exit the function after re-enabling the local IRQ.
	 */
	यदि (need_resched()) {
		local_irq_enable();
		प्रत्यागम;
	}

	यदि (cpuidle_not_available(drv, dev)) {
		idle_call_stop_or_retain_tick(stop_tick);

		default_idle_call();
		गच्छ exit_idle;
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

	यदि (idle_should_enter_s2idle() || dev->forced_idle_latency_limit_ns) {
		u64 max_latency_ns;

		यदि (idle_should_enter_s2idle()) {
			max_latency_ns = cpu_wakeup_latency_qos_limit() *
					 NSEC_PER_USEC;

			entered_state = call_cpuidle_s2idle(drv, dev,
							    max_latency_ns);
			यदि (entered_state > 0)
				गच्छ exit_idle;
		} अन्यथा {
			max_latency_ns = dev->forced_idle_latency_limit_ns;
		}

		tick_nohz_idle_stop_tick();

		next_state = cpuidle_find_deepest_state(drv, dev, max_latency_ns);
		call_cpuidle(drv, dev, next_state);
	} अन्यथा यदि (drv->state_गिनती > 1) {
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
	} अन्यथा {
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
	यदि (WARN_ON_ONCE(irqs_disabled()))
		local_irq_enable();
}

/*
 * Generic idle loop implementation
 *
 * Called with polling cleared.
 */
स्थिर शून्य do_idle(शून्य)
{
	पूर्णाङ्क cpu = smp_processor_id();
	bool got_tick = false;

	यदि (cpu_is_offline(cpu)) {
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

	यावत् (!need_resched()) {

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
		यदि (cpu_idle_force_poll || tick_check_broadcast_expired()) {
			tick_nohz_idle_restart_tick();
			cpu_idle_poll();
		} अन्यथा {
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

	यदि (unlikely(klp_patch_pending(current)))
		klp_update_patch_state(current);
}

bool cpu_in_idle(unsigned long pc)
{
	प्रत्यागम pc >= (unsigned long)__cpuidle_text_start &&
		pc < (unsigned long)__cpuidle_text_end;
}

संरचना idle_timer {
	संरचना hrtimer timer;
	पूर्णाङ्क done;
};

स्थिर गणना hrtimer_restart idle_inject_timer_fn(संरचना hrtimer *timer)
{
	संरचना idle_timer *it = container_of(timer, संरचना idle_timer, timer);

	WRITE_ONCE(it->done, 1);
	set_tsk_need_resched(current);

	प्रत्यागम HRTIMER_NORESTART;
}

शून्य play_idle_precise(u64 duration_ns, u64 latency_ns)
{
	संरचना idle_timer it;

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
	hrtimer_setup_on_ढेर(&it.timer, idle_inject_timer_fn, CLOCK_MONOTONIC,
			       HRTIMER_MODE_REL_HARD);
	hrtimer_start(&it.timer, ns_to_ktime(duration_ns),
		      HRTIMER_MODE_REL_PINNED_HARD);

	यावत् (!READ_ONCE(it.done))
		do_idle();

	cpuidle_use_deepest_state(0);
	current->flags &= ~PF_IDLE;

	preempt_fold_need_resched();
	preempt_enable();
}
EXPORT_प्रतीक_GPL(play_idle_precise);

शून्य cpu_startup_entry(गणना cpuhp_state state)
{
	current->flags |= PF_IDLE;
	arch_cpu_idle_prepare();
	cpuhp_online_idle(state);
	यावत् (1)
		do_idle();
}

/*
 * idle-task scheduling class.
 */

स्थिर पूर्णाङ्क
select_task_rq_idle(संरचना task_struct *p, पूर्णाङ्क cpu, पूर्णाङ्क flags)
{
	प्रत्यागम task_cpu(p); /* IDLE tasks as never migrated */
}

स्थिर पूर्णाङ्क
balance_idle(संरचना rq *rq, संरचना rq_flags *rf)
{
	प्रत्यागम WARN_ON_ONCE(1);
}

/*
 * Idle tasks are unconditionally rescheduled:
 */
स्थिर शून्य wakeup_preempt_idle(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	resched_curr(rq);
}

स्थिर शून्य update_curr_idle(संरचना rq *rq);

स्थिर शून्य put_prev_task_idle(संरचना rq *rq, संरचना task_struct *prev, संरचना task_struct *next)
{
	update_curr_idle(rq);
	scx_update_idle(rq, false, true);
	update_rq_avg_idle(rq);
}

स्थिर शून्य set_next_task_idle(संरचना rq *rq, संरचना task_struct *next, गणना snt_e type)
{
	यदि (type == SNT_REPICK)
		प्रत्यागम;

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

संरचना task_struct *pick_task_idle(संरचना rq *rq, संरचना rq_flags *rf)
{
	/*
	 * Notify scx only on an idle-to-idle re-pick (the cpu was already idle).
	 * A real task->idle transition is delivered by set_next_task_idle(), so
	 * calling here too would duplicate it.
	 */
	यदि (scx_enabled() && is_idle_task(rq->curr))
		scx_update_idle(rq, true, false);
	प्रत्यागम rq->idle;
}

/*
 * It is not legal to sleep in the idle task - print a warning
 * message if some code attempts to do it:
 */
स्थिर bool
dequeue_task_idle(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	raw_spin_rq_unlock_irq(rq);
	printk(KERN_ERR "bad: scheduling from the idle thread!\n");
	dump_ढेर();
	raw_spin_rq_lock_irq(rq);
	प्रत्यागम true;
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
स्थिर शून्य task_tick_idle(संरचना rq *rq, संरचना task_struct *curr, पूर्णाङ्क queued)
{
	update_curr_idle(rq);
}

स्थिर शून्य switching_to_idle(संरचना rq *rq, संरचना task_struct *p)
{
	BUG();
}

स्थिर शून्य
prio_changed_idle(संरचना rq *rq, संरचना task_struct *p, u64 oldprio)
{
	यदि (p->prio == oldprio)
		प्रत्यागम;

	BUG();
}

स्थिर शून्य update_curr_idle(संरचना rq *rq)
{
	संरचना sched_entity *se = &rq->idle->se;
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_start;
	यदि (unlikely(delta_exec <= 0))
		प्रत्यागम;

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
