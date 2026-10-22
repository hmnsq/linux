// SPDX-License-Identifier: GPL-2.0
/*
 * Per Entity Load Tracking (PELT)
 *
 *  Copyright (C) 2007 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 *
 *  Interactivity improvements by Mike Galbraith
 *  (C) 2007 Mike Galbraith <efault@gmx.de>
 *
 *  Various enhancements by Dmitry Adamushko.
 *  (C) 2007 Dmitry Adamushko <dmitry.adamushko@gmail.com>
 *
 *  Group scheduling enhancements by Srivatsa Vaddagiri
 *  Copyright IBM Corporation, 2007
 *  Author: Srivatsa Vaddagiri <vatsa@linux.vnet.ibm.com>
 *
 *  Scaled math optimizations by Thomas Gleixner
 *  Copyright (C) 2007, Linutronix GmbH, Thomas Gleixner <tglx@kernel.org>
 *
 *  Adaptive scheduling granularity, math enhancements by Peter Zijlstra
 *  Copyright (C) 2007 Red Hat, Inc., Peter Zijlstra
 *
 *  Move PELT related code from fair.c into this pelt.c file
 *  Author: Vincent Guittot <vincent.guittot@linaro.org>
 */
#అన్తర్భావ "pelt.h"

/*
 * Approximate:
 *   val * y^n,    where y^32 ~= 0.5 (~1 scheduling period)
 */
స్థిర u64 decay_ఎక్కించు(u64 val, u64 n)
{
	గుర్తులేని పూర్ణాంకం local_n;

	ఒకవేళ (unlikely(n > ఎక్కించు_AVG_PERIOD * 63))
		తిరిగి 0;

	/* after bounds checking we can collapse to 32-bit */
	local_n = n;

	/*
	 * As y^PERIOD = 1/2, we can combine
	 *    y^n = 1/2^(n/PERIOD) * y^(n%PERIOD)
	 * With a look-up table which covers y^n (n<PERIOD)
	 *
	 * To achieve constant time decay_load.
	 */
	ఒకవేళ (unlikely(local_n >= ఎక్కించు_AVG_PERIOD)) {
		val >>= local_n / ఎక్కించు_AVG_PERIOD;
		local_n %= ఎక్కించు_AVG_PERIOD;
	}

	val = mul_u64_u32_shr(val, runnable_avg_yN_inv[local_n], 32);
	తిరిగి val;
}

స్థిర u32 __accumulate_pelt_segments(u64 periods, u32 d1, u32 d3)
{
	u32 c1, c2, c3 = d3; /* y^0 == 1 */

	/*
	 * c1 = d1 y^p
	 */
	c1 = decay_ఎక్కించు((u64)d1, periods);

	/*
	 *            p-1
	 * c2 = 1024 \Sum y^n
	 *            n=1
	 *
	 *              inf        inf
	 *    = 1024 ( \Sum y^n - \Sum y^n - y^0 )
	 *              n=0        n=p
	 */
	c2 = ఎక్కించు_AVG_MAX - decay_ఎక్కించు(ఎక్కించు_AVG_MAX, periods) - 1024;

	తిరిగి c1 + c2 + c3;
}

/*
 * Accumulate the three separate parts of the sum; d1 the remainder
 * of the last (incomplete) period, d2 the span of full periods and d3
 * the remainder of the (incomplete) current period.
 *
 *           d1          d2           d3
 *           ^           ^            ^
 *           |           |            |
 *         |<->|<----------------->|<--->|
 * ... |---x---|------| ... |------|-----x (now)
 *
 *                           p-1
 * u' = (u + d1) y^p + 1024 \Sum y^n + d3 y^0
 *                           n=1
 *
 *    = u y^p +					(Step 1)
 *
 *                     p-1
 *      d1 y^p + 1024 \Sum y^n + d3 y^0		(Step 2)
 *                     n=1
 */
స్థిర __always_inline u32
accumulate_కూడిక(u64 delta, నిర్మాణం sched_avg *sa,
	       గుర్తులేని దీర్ఘ ఎక్కించు, గుర్తులేని దీర్ఘ runnable, పూర్ణాంకం running)
{
	u32 contrib = (u32)delta; /* p == 0 -> delta < 1024 */
	u64 periods;

	delta += sa->period_contrib;
	periods = delta / 1024; /* A period is 1024us (~1ms) */

	/*
	 * Step 1: decay old *_sum if we crossed period boundaries.
	 */
	ఒకవేళ (periods) {
		sa->ఎక్కించు_కూడిక = decay_ఎక్కించు(sa->ఎక్కించు_కూడిక, periods);
		sa->runnable_కూడిక =
			decay_ఎక్కించు(sa->runnable_కూడిక, periods);
		sa->util_కూడిక = decay_ఎక్కించు((u64)(sa->util_కూడిక), periods);

		/*
		 * Step 2
		 */
		delta %= 1024;
		ఒకవేళ (ఎక్కించు) {
			/*
			 * This relies on the:
			 *
			 * if (!load)
			 *	runnable = running = 0;
			 *
			 * clause from ___update_load_sum(); this results in
			 * the below usage of @contrib to disappear entirely,
			 * so no point in calculating it.
			 */
			contrib = __accumulate_pelt_segments(periods,
					1024 - sa->period_contrib, delta);
		}
	}
	sa->period_contrib = delta;

	ఒకవేళ (ఎక్కించు)
		sa->ఎక్కించు_కూడిక += ఎక్కించు * contrib;
	ఒకవేళ (runnable)
		sa->runnable_కూడిక += runnable * contrib << SCHED_సామర్థ్యం_జరుపు;
	ఒకవేళ (running)
		sa->util_కూడిక += contrib << SCHED_సామర్థ్యం_జరుపు;

	తిరిగి periods;
}

/*
 * We can represent the historical contribution to runnable average as the
 * coefficients of a geometric series.  To do this we sub-divide our runnable
 * history into segments of approximately 1ms (1024us); label the segment that
 * occurred N-ms ago p_N, with p_0 corresponding to the current period, e.g.
 *
 * [<- 1024us ->|<- 1024us ->|<- 1024us ->| ...
 *      p0            p1           p2
 *     (now)       (~1ms ago)  (~2ms ago)
 *
 * Let u_i denote the fraction of p_i that the entity was runnable.
 *
 * We then designate the fractions u_i as our co-efficients, yielding the
 * following representation of historical load:
 *   u_0 + u_1*y + u_2*y^2 + u_3*y^3 + ...
 *
 * We choose y based on the with of a reasonably scheduling period, fixing:
 *   y^32 = 0.5
 *
 * This means that the contribution to load ~32ms ago (u_32) will be weighted
 * approximately half as much as the contribution to load within the last ms
 * (u_0).
 *
 * When a period "rolls over" and we have new u_0`, multiplying the previous
 * sum again by y is sufficient to update:
 *   load_avg = u_0` + y*(u_0 + u_1*y + u_2*y^2 + ... )
 *            = u_0 + u_1*y + u_2*y^2 + ... [re-labeling u_i --> u_{i+1}]
 */
స్థిర __always_inline పూర్ణాంకం
___update_load_sum(u64 now, నిర్మాణం sched_avg *sa,
		  గుర్తులేని దీర్ఘ ఎక్కించు, గుర్తులేని దీర్ఘ runnable, పూర్ణాంకం running)
{
	u64 delta;

	delta = now - sa->చివరి_update_సమయం;
	/*
	 * This should only happen when time goes backwards, which it
	 * unfortunately does during sched clock init when we swap over to TSC.
	 */
	ఒకవేళ ((s64)delta < 0) {
		sa->చివరి_update_సమయం = now;
		తిరిగి 0;
	}

	/*
	 * Use 1024ns as the unit of measurement since it's a reasonable
	 * approximation of 1us and fast to compute.
	 */
	delta >>= 10;
	ఒకవేళ (!delta)
		తిరిగి 0;

	sa->చివరి_update_సమయం += delta << 10;

	/*
	 * running is a subset of runnable (weight) so running can't be set if
	 * runnable is clear. But there are some corner cases where the current
	 * se has been already dequeued but cfs_rq->h_curr still points to it.
	 * This means that weight will be 0 but not running for a sched_entity
	 * but also for a cfs_rq if the latter becomes idle. As an example,
	 * this happens during sched_balance_newidle() which calls
	 * sched_balance_update_blocked_averages().
	 *
	 * Also see the comment in accumulate_sum().
	 */
	ఒకవేళ (!ఎక్కించు)
		runnable = running = 0;

	/*
	 * Now we know we crossed measurement unit boundaries. The *_avg
	 * accrues by two steps:
	 *
	 * Step 1: accumulate *_sum since last_update_time. If we haven't
	 * crossed period boundaries, finish.
	 */
	ఒకవేళ (!accumulate_కూడిక(delta, sa, ఎక్కించు, runnable, running))
		తిరిగి 0;

	తిరిగి 1;
}

/*
 * When syncing *_avg with *_sum, we must take into account the current
 * position in the PELT segment otherwise the remaining part of the segment
 * will be considered as idle time whereas it's not yet elapsed and this will
 * generate unwanted oscillation in the range [1002..1024[.
 *
 * The max value of *_sum varies with the position in the time segment and is
 * equals to :
 *
 *   LOAD_AVG_MAX*y + sa->period_contrib
 *
 * which can be simplified into:
 *
 *   LOAD_AVG_MAX - 1024 + sa->period_contrib
 *
 * because LOAD_AVG_MAX*y == LOAD_AVG_MAX-1024
 *
 * The same care must be taken when a sched entity is added, updated or
 * removed from a cfs_rq and we need to update sched_avg. Scheduler entities
 * and the cfs rq, to which they are attached, have the same position in the
 * time segment because they use the same clock. This means that we can use
 * the period_contrib of cfs_rq when updating the sched_avg of a sched_entity
 * if it's more convenient.
 */
స్థిర __always_inline శూన్య
___update_load_avg(నిర్మాణం sched_avg *sa, గుర్తులేని దీర్ఘ ఎక్కించు)
{
	u32 divider = get_pelt_divider(sa);

	/*
	 * Step 2: update *_avg.
	 */
	sa->ఎక్కించు_avg = div_u64(ఎక్కించు * sa->ఎక్కించు_కూడిక, divider);
	sa->runnable_avg = div_u64(sa->runnable_కూడిక, divider);
	WRITE_ONCE(sa->util_avg, sa->util_కూడిక / divider);
}

/*
 * sched_entity:
 *
 *   task:
 *     se_weight()   = se->load.weight
 *     se_runnable() = !!on_rq
 *
 *   group: [ see update_cfs_group() ]
 *     se_weight()   = tg->weight * grq->load_avg / tg->load_avg
 *     se_runnable() = grq->h_nr_runnable
 *
 *   runnable_sum = se_runnable() * runnable = grq->runnable_sum
 *   runnable_avg = runnable_sum
 *
 *   load_sum := runnable
 *   load_avg = se_weight(se) * load_sum
 *
 * cfq_rq:
 *
 *   runnable_sum = \Sum se->avg.runnable_sum
 *   runnable_avg = \Sum se->avg.runnable_avg
 *
 *   load_sum = \Sum se_weight(se) * se->avg.load_sum
 *   load_avg = \Sum se->avg.load_avg
 */

పూర్ణాంకం __update_load_avg_blocked_se(u64 now, నిర్మాణం sched_entity *se)
{
	ఒకవేళ (___update_load_sum(now, &se->avg, 0, 0, 0)) {
		___update_load_avg(&se->avg, se_బరువు(se));
		trace_pelt_se_tp(se);
		తిరిగి 1;
	}

	తిరిగి 0;
}

పూర్ణాంకం __update_load_avg_se(u64 now, నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	ఒకవేళ (___update_load_sum(now, &se->avg, !!se->on_rq, se_runnable(se),
				cfs_rq->h_curr == se)) {

		___update_load_avg(&se->avg, se_బరువు(se));
		cfs_se_util_change(&se->avg);
		trace_pelt_se_tp(se);
		తిరిగి 1;
	}

	తిరిగి 0;
}

పూర్ణాంకం __update_load_avg_cfs_rq(u64 now, నిర్మాణం cfs_rq *cfs_rq)
{
	ఒకవేళ (___update_load_sum(now, &cfs_rq->avg,
				scale_ఎక్కించు_కిందికి(cfs_rq->ఎక్కించు.బరువు),
				cfs_rq->h_nr_runnable,
				cfs_rq->h_curr != ఖాళీ)) {

		___update_load_avg(&cfs_rq->avg, 1);
		trace_pelt_cfs_tp(cfs_rq);
		తిరిగి 1;
	}

	తిరిగి 0;
}

/*
 * rt_rq:
 *
 *   util_sum = \Sum se->avg.util_sum but se->avg.util_sum is not tracked
 *   util_sum = cpu_scale * load_sum
 *   runnable_sum = util_sum
 *
 *   load_avg and runnable_avg are not supported and meaningless.
 *
 */

పూర్ణాంకం update_rt_rq_ఎక్కించు_avg(u64 now, నిర్మాణం rq *rq, పూర్ణాంకం running)
{
	ఒకవేళ (___update_load_sum(now, &rq->avg_rt,
				running,
				running,
				running)) {

		___update_load_avg(&rq->avg_rt, 1);
		trace_pelt_rt_tp(rq);
		తిరిగి 1;
	}

	తిరిగి 0;
}

/*
 * dl_rq:
 *
 *   util_sum = \Sum se->avg.util_sum but se->avg.util_sum is not tracked
 *   util_sum = cpu_scale * load_sum
 *   runnable_sum = util_sum
 *
 *   load_avg and runnable_avg are not supported and meaningless.
 *
 */

పూర్ణాంకం update_dl_rq_ఎక్కించు_avg(u64 now, నిర్మాణం rq *rq, పూర్ణాంకం running)
{
	ఒకవేళ (___update_load_sum(now, &rq->avg_dl,
				running,
				running,
				running)) {

		___update_load_avg(&rq->avg_dl, 1);
		trace_pelt_dl_tp(rq);
		తిరిగి 1;
	}

	తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_SCHED_HW_PRESSURE
/*
 * hardware:
 *
 *   load_sum = \Sum se->avg.load_sum but se->avg.load_sum is not tracked
 *
 *   util_avg and runnable_load_avg are not supported and meaningless.
 *
 * Unlike rt/dl utilization tracking that track time spent by a cpu
 * running a rt/dl task through util_avg, the average HW pressure is
 * tracked through load_avg. This is because HW pressure signal is
 * time weighted "delta" capacity unlike util_avg which is binary.
 * "delta capacity" =  actual capacity  -
 *			capped capacity a cpu due to a HW event.
 */

పూర్ణాంకం update_hw_ఎక్కించు_avg(u64 now, నిర్మాణం rq *rq, u64 సామర్థ్యం)
{
	ఒకవేళ (___update_load_sum(now, &rq->avg_hw,
			       సామర్థ్యం,
			       సామర్థ్యం,
			       సామర్థ్యం)) {
		___update_load_avg(&rq->avg_hw, 1);
		trace_pelt_hw_tp(rq);
		తిరిగి 1;
	}

	తిరిగి 0;
}
#అయితే_ముగింపు /* CONFIG_SCHED_HW_PRESSURE */

#నిర్వచితమైతే CONFIG_HAVE_SCHED_AVG_IRQ
/*
 * IRQ:
 *
 *   util_sum = \Sum se->avg.util_sum but se->avg.util_sum is not tracked
 *   util_sum = cpu_scale * load_sum
 *   runnable_sum = util_sum
 *
 *   load_avg and runnable_avg are not supported and meaningless.
 *
 */

పూర్ణాంకం update_irq_ఎక్కించు_avg(నిర్మాణం rq *rq, u64 running)
{
	పూర్ణాంకం ret = 0;

	/*
	 * We can't use clock_pelt because IRQ time is not accounted in
	 * clock_task. Instead we directly scale the running time to
	 * reflect the real amount of computation
	 */
	running = cap_scale(running, arch_scale_freq_సామర్థ్యం(cpu_యొక్క(rq)));
	running = cap_scale(running, arch_scale_cpu_సామర్థ్యం(cpu_యొక్క(rq)));

	/*
	 * We know the time that has been used by interrupt since last update
	 * but we don't when. Let be pessimistic and assume that interrupt has
	 * happened just before the update. This is not so far from reality
	 * because interrupt will most probably wake up task and trig an update
	 * of rq clock during which the metric is updated.
	 * We start to decay with normal context time and then we add the
	 * interrupt context time.
	 * We can safely remove running from rq->clock because
	 * rq->clock += delta with delta >= running
	 */
	ret = ___update_load_sum(rq->clock - running, &rq->avg_irq,
				0,
				0,
				0);
	ret += ___update_load_sum(rq->clock, &rq->avg_irq,
				1,
				1,
				1);

	ఒకవేళ (ret) {
		___update_load_avg(&rq->avg_irq, 1);
		trace_pelt_irq_tp(rq);
	}

	తిరిగి ret;
}
#అయితే_ముగింపు /* CONFIG_HAVE_SCHED_AVG_IRQ */

/*
 * Load avg and utiliztion metrics need to be updated periodically and before
 * consumption. This function updates the metrics for all subsystems except for
 * the fair class. @rq must be locked and have its clock updated.
 */
bool update_other_ఎక్కించు_avgs(నిర్మాణం rq *rq)
{
	u64 now = rq_clock_pelt(rq);
	స్థిరరాశి నిర్మాణం sched_class *curr_class = rq->donor->sched_class;
	గుర్తులేని దీర్ఘ hw_pressure = arch_scale_hw_pressure(cpu_యొక్క(rq));

	lockdep_assert_rq_held(rq);

	/* hw_pressure doesn't care about invariance */
	తిరిగి update_rt_rq_ఎక్కించు_avg(now, rq, curr_class == &rt_sched_class) |
		update_dl_rq_ఎక్కించు_avg(now, rq, curr_class == &dl_sched_class) |
		update_hw_ఎక్కించు_avg(rq_clock_task(rq), rq, hw_pressure) |
		update_irq_ఎక్కించు_avg(rq, 0);
}
