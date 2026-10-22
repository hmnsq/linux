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
#ಅನ್ತರ್ಭಾವ "pelt.h"

/*
 * Approximate:
 *   val * y^n,    where y^32 ~= 0.5 (~1 scheduling period)
 */
ಸ್ಥಿರ u64 decay_ತುಂಬು(u64 val, u64 n)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ local_n;

	ಆದರೆ (unlikely(n > ತುಂಬು_AVG_PERIOD * 63))
		ಹಿಂತಿರುಗಿ 0;

	/* after bounds checking we can collapse to 32-bit */
	local_n = n;

	/*
	 * As y^PERIOD = 1/2, we can combine
	 *    y^n = 1/2^(n/PERIOD) * y^(n%PERIOD)
	 * With a look-up table which covers y^n (n<PERIOD)
	 *
	 * To achieve constant time decay_load.
	 */
	ಆದರೆ (unlikely(local_n >= ತುಂಬು_AVG_PERIOD)) {
		val >>= local_n / ತುಂಬು_AVG_PERIOD;
		local_n %= ತುಂಬು_AVG_PERIOD;
	}

	val = mul_u64_u32_shr(val, runnable_avg_yN_inv[local_n], 32);
	ಹಿಂತಿರುಗಿ val;
}

ಸ್ಥಿರ u32 __accumulate_pelt_segments(u64 periods, u32 d1, u32 d3)
{
	u32 c1, c2, c3 = d3; /* y^0 == 1 */

	/*
	 * c1 = d1 y^p
	 */
	c1 = decay_ತುಂಬು((u64)d1, periods);

	/*
	 *            p-1
	 * c2 = 1024 \Sum y^n
	 *            n=1
	 *
	 *              inf        inf
	 *    = 1024 ( \Sum y^n - \Sum y^n - y^0 )
	 *              n=0        n=p
	 */
	c2 = ತುಂಬು_AVG_MAX - decay_ತುಂಬು(ತುಂಬು_AVG_MAX, periods) - 1024;

	ಹಿಂತಿರುಗಿ c1 + c2 + c3;
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
ಸ್ಥಿರ __always_inline u32
accumulate_ಮೊತ್ತ(u64 delta, ರಚನೆ sched_avg *sa,
	       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ runnable, ಪೂರ್ಣಾಂಕ running)
{
	u32 contrib = (u32)delta; /* p == 0 -> delta < 1024 */
	u64 periods;

	delta += sa->period_contrib;
	periods = delta / 1024; /* A period is 1024us (~1ms) */

	/*
	 * Step 1: decay old *_sum if we crossed period boundaries.
	 */
	ಆದರೆ (periods) {
		sa->ತುಂಬು_ಮೊತ್ತ = decay_ತುಂಬು(sa->ತುಂಬು_ಮೊತ್ತ, periods);
		sa->runnable_ಮೊತ್ತ =
			decay_ತುಂಬು(sa->runnable_ಮೊತ್ತ, periods);
		sa->util_ಮೊತ್ತ = decay_ತುಂಬು((u64)(sa->util_ಮೊತ್ತ), periods);

		/*
		 * Step 2
		 */
		delta %= 1024;
		ಆದರೆ (ತುಂಬು) {
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

	ಆದರೆ (ತುಂಬು)
		sa->ತುಂಬು_ಮೊತ್ತ += ತುಂಬು * contrib;
	ಆದರೆ (runnable)
		sa->runnable_ಮೊತ್ತ += runnable * contrib << SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು;
	ಆದರೆ (running)
		sa->util_ಮೊತ್ತ += contrib << SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು;

	ಹಿಂತಿರುಗಿ periods;
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
ಸ್ಥಿರ __always_inline ಪೂರ್ಣಾಂಕ
___update_load_sum(u64 now, ರಚನೆ sched_avg *sa,
		  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ runnable, ಪೂರ್ಣಾಂಕ running)
{
	u64 delta;

	delta = now - sa->ಕೊನೆಯ_update_ಸಮಯ;
	/*
	 * This should only happen when time goes backwards, which it
	 * unfortunately does during sched clock init when we swap over to TSC.
	 */
	ಆದರೆ ((s64)delta < 0) {
		sa->ಕೊನೆಯ_update_ಸಮಯ = now;
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * Use 1024ns as the unit of measurement since it's a reasonable
	 * approximation of 1us and fast to compute.
	 */
	delta >>= 10;
	ಆದರೆ (!delta)
		ಹಿಂತಿರುಗಿ 0;

	sa->ಕೊನೆಯ_update_ಸಮಯ += delta << 10;

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
	ಆದರೆ (!ತುಂಬು)
		runnable = running = 0;

	/*
	 * Now we know we crossed measurement unit boundaries. The *_avg
	 * accrues by two steps:
	 *
	 * Step 1: accumulate *_sum since last_update_time. If we haven't
	 * crossed period boundaries, finish.
	 */
	ಆದರೆ (!accumulate_ಮೊತ್ತ(delta, sa, ತುಂಬು, runnable, running))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
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
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ
___update_load_avg(ರಚನೆ sched_avg *sa, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು)
{
	u32 divider = get_pelt_divider(sa);

	/*
	 * Step 2: update *_avg.
	 */
	sa->ತುಂಬು_avg = div_u64(ತುಂಬು * sa->ತುಂಬು_ಮೊತ್ತ, divider);
	sa->runnable_avg = div_u64(sa->runnable_ಮೊತ್ತ, divider);
	WRITE_ONCE(sa->util_avg, sa->util_ಮೊತ್ತ / divider);
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

ಪೂರ್ಣಾಂಕ __update_load_avg_blocked_se(u64 now, ರಚನೆ sched_entity *se)
{
	ಆದರೆ (___update_load_sum(now, &se->avg, 0, 0, 0)) {
		___update_load_avg(&se->avg, se_ತೂಕ(se));
		trace_pelt_se_tp(se);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ __update_load_avg_se(u64 now, ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಆದರೆ (___update_load_sum(now, &se->avg, !!se->on_rq, se_runnable(se),
				cfs_rq->h_curr == se)) {

		___update_load_avg(&se->avg, se_ತೂಕ(se));
		cfs_se_util_change(&se->avg);
		trace_pelt_se_tp(se);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ __update_load_avg_cfs_rq(u64 now, ರಚನೆ cfs_rq *cfs_rq)
{
	ಆದರೆ (___update_load_sum(now, &cfs_rq->avg,
				scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(cfs_rq->ತುಂಬು.ತೂಕ),
				cfs_rq->h_nr_runnable,
				cfs_rq->h_curr != ಖಾಲಿ)) {

		___update_load_avg(&cfs_rq->avg, 1);
		trace_pelt_cfs_tp(cfs_rq);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
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

ಪೂರ್ಣಾಂಕ update_rt_rq_ತುಂಬು_avg(u64 now, ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ running)
{
	ಆದರೆ (___update_load_sum(now, &rq->avg_rt,
				running,
				running,
				running)) {

		___update_load_avg(&rq->avg_rt, 1);
		trace_pelt_rt_tp(rq);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
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

ಪೂರ್ಣಾಂಕ update_dl_rq_ತುಂಬು_avg(u64 now, ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ running)
{
	ಆದರೆ (___update_load_sum(now, &rq->avg_dl,
				running,
				running,
				running)) {

		___update_load_avg(&rq->avg_dl, 1);
		trace_pelt_dl_tp(rq);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_HW_PRESSURE
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

ಪೂರ್ಣಾಂಕ update_hw_ತುಂಬು_avg(u64 now, ರಚನೆ rq *rq, u64 ಸಾಮರ್ಥ್ಯ)
{
	ಆದರೆ (___update_load_sum(now, &rq->avg_hw,
			       ಸಾಮರ್ಥ್ಯ,
			       ಸಾಮರ್ಥ್ಯ,
			       ಸಾಮರ್ಥ್ಯ)) {
		___update_load_avg(&rq->avg_hw, 1);
		trace_pelt_hw_tp(rq);
		ಹಿಂತಿರುಗಿ 1;
	}

	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_HW_PRESSURE */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HAVE_SCHED_AVG_IRQ
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

ಪೂರ್ಣಾಂಕ update_irq_ತುಂಬು_avg(ರಚನೆ rq *rq, u64 running)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	/*
	 * We can't use clock_pelt because IRQ time is not accounted in
	 * clock_task. Instead we directly scale the running time to
	 * reflect the real amount of computation
	 */
	running = cap_scale(running, arch_scale_freq_ಸಾಮರ್ಥ್ಯ(cpu_ದ(rq)));
	running = cap_scale(running, arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu_ದ(rq)));

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

	ಆದರೆ (ret) {
		___update_load_avg(&rq->avg_irq, 1);
		trace_pelt_irq_tp(rq);
	}

	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_HAVE_SCHED_AVG_IRQ */

/*
 * Load avg and utiliztion metrics need to be updated periodically and before
 * consumption. This function updates the metrics for all subsystems except for
 * the fair class. @rq must be locked and have its clock updated.
 */
bool update_other_ತುಂಬು_avgs(ರಚನೆ rq *rq)
{
	u64 now = rq_clock_pelt(rq);
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *curr_class = rq->donor->sched_class;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ hw_pressure = arch_scale_hw_pressure(cpu_ದ(rq));

	lockdep_assert_rq_held(rq);

	/* hw_pressure doesn't care about invariance */
	ಹಿಂತಿರುಗಿ update_rt_rq_ತುಂಬು_avg(now, rq, curr_class == &rt_sched_class) |
		update_dl_rq_ತುಂಬು_avg(now, rq, curr_class == &dl_sched_class) |
		update_hw_ತುಂಬು_avg(rq_clock_task(rq), rq, hw_pressure) |
		update_irq_ತುಂಬು_avg(rq, 0);
}
