// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic entry points for the idle threads and
 * implementation of the idle task scheduling class.
 *
 * (NOTE: these are not related to SCHED_IDLE batch scheduled
 *        tasks which are handled in sched/fair.c )
 */
#అన్తర్భావ <linux/cpuidle.h>
#అన్తర్భావ <linux/suspend.h>
#అన్తర్భావ <linux/livepatch.h>
#అన్తర్భావ "sched.h"
#అన్తర్భావ "smp.h"

/* Linker adds these: start and end of __cpuidle functions */
బాహ్య అక్షరం __cpuidle_text_start[], __cpuidle_text_end[];

/**
 * sched_idle_set_state - Record idle state for the current CPU.
 * @idle_state: State to record.
 */
శూన్య sched_idle_సమితి_state(నిర్మాణం cpuidle_state *idle_state)
{
	idle_సమితి_state(this_rq(), idle_state);
}

స్థిర పూర్ణాంకం __read_mostly cpu_idle_force_poll;

శూన్య cpu_idle_poll_ctrl(bool enable)
{
	ఒకవేళ (enable) {
		cpu_idle_force_poll++;
	} లేకపోతే {
		cpu_idle_force_poll--;
		WARN_ON_ONCE(cpu_idle_force_poll < 0);
	}
}

#నిర్వచితమైతే CONFIG_GENERIC_IDLE_POLL_అమరిక
స్థిర పూర్ణాంకం __init cpu_idle_poll_అమరిక(అక్షరం *__unused)
{
	cpu_idle_force_poll = 1;

	తిరిగి 1;
}
__setup("nohlt", cpu_idle_poll_అమరిక);

స్థిర పూర్ణాంకం __init cpu_idle_nopoll_అమరిక(అక్షరం *__unused)
{
	cpu_idle_force_poll = 0;

	తిరిగి 1;
}
__setup("hlt", cpu_idle_nopoll_అమరిక);
#అయితే_ముగింపు /* CONFIG_GENERIC_IDLE_POLL_SETUP */

స్థిర noinline పూర్ణాంకం __cpuidle cpu_idle_poll(శూన్య)
{
	instrumentation_begin();
	trace_cpu_idle(0, smp_processor_id());
	stop_critical_timings();
	ct_cpuidle_enter();

	raw_local_irq_enable();
	అయితే (!tif_need_resched() &&
	       (cpu_idle_force_poll || tick_check_ప్రసారం_expired()))
		cpu_relax();
	raw_local_irq_disable();

	ct_cpuidle_exit();
	ప్రారంభం_critical_timings();
	trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
	local_irq_enable();
	instrumentation_చివర();

	తిరిగి 1;
}

/* Weak implementations for optional arch specific functions */
శూన్య __weak arch_cpu_idle_prepare(శూన్య) { }
శూన్య __weak arch_cpu_idle_enter(శూన్య) { }
శూన్య __weak arch_cpu_idle_exit(శూన్య) { }
శూన్య __weak __noreturn arch_cpu_idle_dead(శూన్య) { అయితే (1); }
శూన్య __weak arch_cpu_idle(శూన్య)
{
	cpu_idle_force_poll = 1;
}

#నిర్వచితమైతే CONFIG_GENERIC_CLOCKEVENTS_ప్రసారం_IDLE
DEFINE_STATIC_కీలకం_FALSE(arch_needs_tick_ప్రసారం);

స్థిర అంతఃవరుస శూన్య cond_tick_ప్రసారం_enter(శూన్య)
{
	ఒకవేళ (static_branch_unlikely(&arch_needs_tick_ప్రసారం))
		tick_ప్రసారం_enter();
}

స్థిర అంతఃవరుస శూన్య cond_tick_ప్రసారం_exit(శూన్య)
{
	ఒకవేళ (static_branch_unlikely(&arch_needs_tick_ప్రసారం))
		tick_ప్రసారం_exit();
}
#లేకపోతే /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE: */
స్థిర అంతఃవరుస శూన్య cond_tick_ప్రసారం_enter(శూన్య) { }
స్థిర అంతఃవరుస శూన్య cond_tick_ప్రసారం_exit(శూన్య) { }
#అయితే_ముగింపు /* !CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE */

/**
 * default_idle_call - Default CPU idle routine.
 *
 * To use when the cpuidle framework cannot be used.
 */
శూన్య __cpuidle default_idle_call(శూన్య)
{
	instrumentation_begin();
	ఒకవేళ (!current_clr_polling_and_test()) {
		cond_tick_ప్రసారం_enter();
		trace_cpu_idle(1, smp_processor_id());
		stop_critical_timings();

		ct_cpuidle_enter();
		arch_cpu_idle();
		ct_cpuidle_exit();

		ప్రారంభం_critical_timings();
		trace_cpu_idle(PWR_EVENT_EXIT, smp_processor_id());
		cond_tick_ప్రసారం_exit();
	}
	local_irq_enable();
	instrumentation_చివర();
}

స్థిర పూర్ణాంకం call_cpuidle_s2idle(నిర్మాణం cpuidle_driver *drv,
			       నిర్మాణం cpuidle_device *dev,
			       u64 max_latency_ns)
{
	ఒకవేళ (current_clr_polling_and_test())
		తిరిగి -EBUSY;

	తిరిగి cpuidle_enter_s2idle(drv, dev, max_latency_ns);
}

స్థిర పూర్ణాంకం call_cpuidle(నిర్మాణం cpuidle_driver *drv, నిర్మాణం cpuidle_device *dev,
		      పూర్ణాంకం తదుపరి_state)
{
	/*
	 * The idle task must be scheduled, it is pointless to go to idle, just
	 * update no idle residency and return.
	 */
	ఒకవేళ (current_clr_polling_and_test()) {
		dev->చివరి_residency_ns = 0;
		local_irq_enable();
		తిరిగి -EBUSY;
	}

	/*
	 * Enter the idle state previously returned by the governor decision.
	 * This function will block until an interrupt occurs and will take
	 * care of re-enabling the local interrupts
	 */
	తిరిగి cpuidle_enter(drv, dev, తదుపరి_state);
}

స్థిర శూన్య idle_call_stop_or_retain_tick(bool stop_tick)
{
	ఒకవేళ (stop_tick || tick_nohz_tick_stopped())
		tick_nohz_idle_stop_tick();
	లేకపోతే
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
స్థిర శూన్య cpuidle_idle_call(bool stop_tick)
{
	నిర్మాణం cpuidle_device *dev = cpuidle_get_device();
	నిర్మాణం cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	పూర్ణాంకం తదుపరి_state, entered_state;

	/*
	 * Check if the idle task must be rescheduled. If it is the
	 * case, exit the function after re-enabling the local IRQ.
	 */
	ఒకవేళ (need_resched()) {
		local_irq_enable();
		తిరిగి;
	}

	ఒకవేళ (cpuidle_not_అందుబాటు(drv, dev)) {
		idle_call_stop_or_retain_tick(stop_tick);

		default_idle_call();
		వెళ్ళు exit_idle;
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

	ఒకవేళ (idle_should_enter_s2idle() || dev->forced_idle_latency_హద్దు_ns) {
		u64 max_latency_ns;

		ఒకవేళ (idle_should_enter_s2idle()) {
			max_latency_ns = cpu_wakeup_latency_qos_హద్దు() *
					 NSEC_చొప్పున_USEC;

			entered_state = call_cpuidle_s2idle(drv, dev,
							    max_latency_ns);
			ఒకవేళ (entered_state > 0)
				వెళ్ళు exit_idle;
		} లేకపోతే {
			max_latency_ns = dev->forced_idle_latency_హద్దు_ns;
		}

		tick_nohz_idle_stop_tick();

		తదుపరి_state = cpuidle_కనుగొను_అత్యంతలోతు_state(drv, dev, max_latency_ns);
		call_cpuidle(drv, dev, తదుపరి_state);
	} లేకపోతే ఒకవేళ (drv->state_లెక్క > 1) {
		/*
		 * stop_tick is expected to be true by default by cpuidle
		 * governors, which allows them to select idle states with
		 * target residency above the tick period length.
		 */
		stop_tick = true;

		/*
		 * Ask the cpuidle framework to choose a convenient idle state.
		 */
		తదుపరి_state = cpuidle_select(drv, dev, &stop_tick);

		idle_call_stop_or_retain_tick(stop_tick);

		entered_state = call_cpuidle(drv, dev, తదుపరి_state);
		/*
		 * Give the governor an opportunity to reflect on the outcome
		 */
		cpuidle_reflect(dev, entered_state);
	} లేకపోతే {
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
	ఒకవేళ (WARN_ON_ONCE(irqs_disabled()))
		local_irq_enable();
}

/*
 * Generic idle loop implementation
 *
 * Called with polling cleared.
 */
స్థిర శూన్య do_idle(శూన్య)
{
	పూర్ణాంకం cpu = smp_processor_id();
	bool got_tick = false;

	ఒకవేళ (cpu_is_offline(cpu)) {
		local_irq_disable();
		/* All per-CPU kernel threads should be done by now. */
		WARN_ON_ONCE(need_resched());
		cpuhp_report_idle_dead();
		arch_cpu_idle_dead();
	}

	/*
	 * Check if we need to update blocked load
	 */
	nohz_నిరంతరం_idle_balance(cpu);

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

	అయితే (!need_resched()) {

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
		ఒకవేళ (cpu_idle_force_poll || tick_check_ప్రసారం_expired()) {
			tick_nohz_idle_restart_tick();
			cpu_idle_poll();
		} లేకపోతే {
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
	preempt_సమితి_need_resched();
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
	flush_smp_call_function_వరుస();
	కాలపట్టిక_idle();

	ఒకవేళ (unlikely(klp_patch_pending(current)))
		klp_update_patch_state(current);
}

bool cpu_లోపల_idle(గుర్తులేని దీర్ఘ pc)
{
	తిరిగి pc >= (గుర్తులేని దీర్ఘ)__cpuidle_text_start &&
		pc < (గుర్తులేని దీర్ఘ)__cpuidle_text_end;
}

నిర్మాణం idle_timer {
	నిర్మాణం hrtimer timer;
	పూర్ణాంకం పూర్తి;
};

స్థిర గణన hrtimer_restart idle_inject_timer_fn(నిర్మాణం hrtimer *timer)
{
	నిర్మాణం idle_timer *it = container_యొక్క(timer, నిర్మాణం idle_timer, timer);

	WRITE_ONCE(it->పూర్తి, 1);
	సమితి_tsk_need_resched(current);

	తిరిగి HRTIMER_NORESTART;
}

శూన్య play_idle_precise(u64 duration_ns, u64 latency_ns)
{
	నిర్మాణం idle_timer it;

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
	cpuidle_use_అత్యంతలోతు_state(latency_ns);

	it.పూర్తి = 0;
	hrtimer_అమరిక_on_దొంతర(&it.timer, idle_inject_timer_fn, CLOCK_MONOTONIC,
			       HRTIMER_బహుళకం_REL_HARD);
	hrtimer_ప్రారంభం(&it.timer, ns_గమ్యం_ktime(duration_ns),
		      HRTIMER_బహుళకం_REL_PINNED_HARD);

	అయితే (!READ_ONCE(it.పూర్తి))
		do_idle();

	cpuidle_use_అత్యంతలోతు_state(0);
	current->flags &= ~PF_IDLE;

	preempt_fold_need_resched();
	preempt_enable();
}
EXPORT_చిహ్నం_GPL(play_idle_precise);

శూన్య cpu_startup_నమోదు(గణన cpuhp_state state)
{
	current->flags |= PF_IDLE;
	arch_cpu_idle_prepare();
	cpuhp_online_idle(state);
	అయితే (1)
		do_idle();
}

/*
 * idle-task scheduling class.
 */

స్థిర పూర్ణాంకం
select_task_rq_idle(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం flags)
{
	తిరిగి task_cpu(p); /* IDLE tasks as never migrated */
}

స్థిర పూర్ణాంకం
balance_idle(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	తిరిగి WARN_ON_ONCE(1);
}

/*
 * Idle tasks are unconditionally rescheduled:
 */
స్థిర శూన్య wakeup_preempt_idle(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	resched_curr(rq);
}

స్థిర శూన్య update_curr_idle(నిర్మాణం rq *rq);

స్థిర శూన్య put_prev_task_idle(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి)
{
	update_curr_idle(rq);
	scx_update_idle(rq, false, true);
	update_rq_avg_idle(rq);
}

స్థిర శూన్య సమితి_తదుపరి_task_idle(నిర్మాణం rq *rq, నిర్మాణం task_struct *తదుపరి, గణన snt_e type)
{
	ఒకవేళ (type == SNT_REPICK)
		తిరిగి;

	update_idle_core(rq);
	scx_update_idle(rq, true, true);
	schedstat_inc(rq->sched_goidle);
	తదుపరి->se.exec_ప్రారంభం = rq_clock_task(rq);

	/*
	 * rq is about to be idle, check if we need to update the
	 * lost_idle_time of clock_pelt
	 */
	update_idle_rq_clock_pelt(rq);
}

నిర్మాణం task_struct *pick_task_idle(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	/*
	 * Notify scx only on an idle-to-idle re-pick (the cpu was already idle).
	 * A real task->idle transition is delivered by set_next_task_idle(), so
	 * calling here too would duplicate it.
	 */
	ఒకవేళ (scx_enabled() && is_idle_task(rq->curr))
		scx_update_idle(rq, true, false);
	తిరిగి rq->idle;
}

/*
 * It is not legal to sleep in the idle task - print a warning
 * message if some code attempts to do it:
 */
స్థిర bool
వరుస_నుండి_తీయి_task_idle(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	raw_spin_rq_unlock_irq(rq);
	printk(KERN_ERR "bad: scheduling from the idle thread!\n");
	dump_దొంతర();
	raw_spin_rq_తాళం_irq(rq);
	తిరిగి true;
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
స్థిర శూన్య task_tick_idle(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr, పూర్ణాంకం queued)
{
	update_curr_idle(rq);
}

స్థిర శూన్య switching_గమ్యం_idle(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	BUG();
}

స్థిర శూన్య
prio_మారింది_idle(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, u64 oldprio)
{
	ఒకవేళ (p->prio == oldprio)
		తిరిగి;

	BUG();
}

స్థిర శూన్య update_curr_idle(నిర్మాణం rq *rq)
{
	నిర్మాణం sched_entity *se = &rq->idle->se;
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_ప్రారంభం;
	ఒకవేళ (unlikely(delta_exec <= 0))
		తిరిగి;

	se->exec_ప్రారంభం = now;

	dl_server_update_idle(&rq->fair_server, delta_exec);
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	dl_server_update_idle(&rq->ext_server, delta_exec);
#అయితే_ముగింపు
}

/*
 * Simple, special scheduling class for the per-CPU idle tasks:
 */
DEFINE_SCHED_CLASS(idle) = {
	/* no enqueue/yield_task for idle tasks */

	/* dequeue is not valid, we print a debug message there: */
	.వరుస_నుండి_తీయి_task		= వరుస_నుండి_తీయి_task_idle,

	.wakeup_preempt		= wakeup_preempt_idle,

	.pick_task		= pick_task_idle,
	.put_prev_task		= put_prev_task_idle,
	.సమితి_తదుపరి_task          = సమితి_తదుపరి_task_idle,

	.balance		= balance_idle,
	.select_task_rq		= select_task_rq_idle,
	.సమితి_cpus_allowed	= సమితి_cpus_allowed_common,

	.task_tick		= task_tick_idle,

	.prio_మారింది		= prio_మారింది_idle,
	.switching_గమ్యం		= switching_గమ్యం_idle,
	.update_curr		= update_curr_idle,
};
