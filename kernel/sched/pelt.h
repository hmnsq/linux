// SPDX-License-Identifier: GPL-2.0
#यदि_अपरिभाषितम् _KERNEL_SCHED_PELT_H
#परिभाषय _KERNEL_SCHED_PELT_H
#अन्तर्भाव "sched.h"

#अन्तर्भाव "sched-pelt.h"

पूर्णाङ्क __update_load_avg_blocked_se(u64 now, संरचना sched_entity *se);
पूर्णाङ्क __update_load_avg_se(u64 now, संरचना cfs_rq *cfs_rq, संरचना sched_entity *se);
पूर्णाङ्क __update_load_avg_cfs_rq(u64 now, संरचना cfs_rq *cfs_rq);
पूर्णाङ्क update_rt_rq_आरोपय_avg(u64 now, संरचना rq *rq, पूर्णाङ्क running);
पूर्णाङ्क update_dl_rq_आरोपय_avg(u64 now, संरचना rq *rq, पूर्णाङ्क running);
bool update_other_आरोपय_avgs(संरचना rq *rq);

#यदि_परिभाषितम् CONFIG_SCHED_HW_PRESSURE
पूर्णाङ्क update_hw_आरोपय_avg(u64 now, संरचना rq *rq, u64 क्षमता);

स्थिर अन्तःस्थ u64 hw_आरोपय_avg(संरचना rq *rq)
{
	प्रत्यागम READ_ONCE(rq->avg_hw.आरोपय_avg);
}
#अन्यथा /* !CONFIG_SCHED_HW_PRESSURE: */
स्थिर अन्तःस्थ पूर्णाङ्क
update_hw_आरोपय_avg(u64 now, संरचना rq *rq, u64 क्षमता)
{
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ u64 hw_आरोपय_avg(संरचना rq *rq)
{
	प्रत्यागम 0;
}
#यदि_अन्तः /* !CONFIG_SCHED_HW_PRESSURE */

#यदि_परिभाषितम् CONFIG_HAVE_SCHED_AVG_IRQ
पूर्णाङ्क update_irq_आरोपय_avg(संरचना rq *rq, u64 running);
#अन्यथा
स्थिर अन्तःस्थ पूर्णाङ्क
update_irq_आरोपय_avg(संरचना rq *rq, u64 running)
{
	प्रत्यागम 0;
}
#यदि_अन्तः

#परिभाषय PELT_MIN_DIVIDER	(आरोपय_AVG_MAX - 1024)

स्थिर अन्तःस्थ u32 get_pelt_divider(संरचना sched_avg *avg)
{
	प्रत्यागम PELT_MIN_DIVIDER + avg->period_contrib;
}

स्थिर अन्तःस्थ शून्य cfs_se_util_change(संरचना sched_avg *avg)
{
	अचिह्नित पूर्णाङ्क enqueued;

	यदि (!sched_feat(UTIL_EST))
		प्रत्यागम;

	/* Avoid store if the flag has been already reset */
	enqueued = avg->util_est;
	यदि (!(enqueued & UTIL_AVG_UNCHANGED))
		प्रत्यागम;

	/* Reset flag to report util_avg has been updated */
	enqueued &= ~UTIL_AVG_UNCHANGED;
	WRITE_ONCE(avg->util_est, enqueued);
}

स्थिर अन्तःस्थ u64 rq_clock_pelt(संरचना rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	प्रत्यागम rq->clock_pelt - rq->नष्टम्_idle_time;
}

/* The rq is idle, we can sync to clock_task */
स्थिर अन्तःस्थ शून्य _update_idle_rq_clock_pelt(संरचना rq *rq)
{
	rq->clock_pelt  = rq_clock_task(rq);

	u64_u32_store(rq->clock_idle, rq_clock(rq));
	/* Paired with smp_rmb in migrate_se_pelt_lag() */
	smp_wmb();
	u64_u32_store(rq->clock_pelt_idle, rq_clock_pelt(rq));
}

/*
 * The clock_pelt scales the time to reflect the effective amount of
 * computation done during the running delta time but then sync back to
 * clock_task when rq is idle.
 *
 *
 * absolute time   | 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16
 * @ max capacity  ------******---------------******---------------
 * @ half capacity ------************---------************---------
 * clock pelt      | 1| 2|    3|    4| 7| 8| 9|   10|   11|14|15|16
 *
 */
स्थिर अन्तःस्थ शून्य update_rq_clock_pelt(संरचना rq *rq, s64 delta)
{
	यदि (unlikely(is_idle_task(rq->curr))) {
		_update_idle_rq_clock_pelt(rq);
		प्रत्यागम;
	}

	/*
	 * When a rq runs at a lower compute capacity, it will need
	 * more time to do the same amount of work than at max
	 * capacity. In order to be invariant, we scale the delta to
	 * reflect how much work has been really done.
	 * Running longer results in stealing idle time that will
	 * disturb the load signal compared to max capacity. This
	 * stolen idle time will be automatically reflected when the
	 * rq will be idle and the clock will be synced with
	 * rq_clock_task.
	 */

	/*
	 * Scale the elapsed time to reflect the real amount of
	 * computation
	 */
	delta = cap_scale(delta, arch_scale_cpu_क्षमता(cpu_सम्बन्धि(rq)));
	delta = cap_scale(delta, arch_scale_freq_क्षमता(cpu_सम्बन्धि(rq)));

	rq->clock_pelt += delta;
}

/*
 * When rq becomes idle, we have to check if it has lost idle time
 * because it was fully busy. A rq is fully used when the /Sum util_sum
 * is greater or equal to:
 * (LOAD_AVG_MAX - 1024 + rq->cfs.avg.period_contrib) << SCHED_CAPACITY_SHIFT;
 * For optimization and computing rounding purpose, we don't take into account
 * the position in the current window (period_contrib) and we use the higher
 * bound of util_sum to decide.
 */
स्थिर अन्तःस्थ शून्य update_idle_rq_clock_pelt(संरचना rq *rq)
{
	u32 divider = ((आरोपय_AVG_MAX - 1024) << SCHED_क्षमता_सरणम्) - आरोपय_AVG_MAX;
	u32 util_योग = rq->cfs.avg.util_योग;
	util_योग += rq->avg_rt.util_योग;
	util_योग += rq->avg_dl.util_योग;

	/*
	 * Reflecting stolen time makes sense only if the idle
	 * phase would be present at max capacity. As soon as the
	 * utilization of a rq has reached the maximum value, it is
	 * considered as an always running rq without idle time to
	 * steal. This potential idle time is considered as lost in
	 * this case. We keep track of this lost idle time compare to
	 * rq's clock_task.
	 */
	यदि (util_योग >= divider)
		rq->नष्टम्_idle_time += rq_clock_task(rq) - rq->clock_pelt;

	_update_idle_rq_clock_pelt(rq);
}

#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
स्थिर अन्तःस्थ शून्य update_idle_cfs_rq_clock_pelt(संरचना cfs_rq *cfs_rq)
{
	u64 throttled;

	यदि (unlikely(cfs_rq->pelt_clock_throttled))
		throttled = U64_MAX;
	अन्यथा
		throttled = cfs_rq->throttled_clock_pelt_time;

	u64_u32_store(cfs_rq->throttled_pelt_idle, throttled);
}

/* rq->task_clock normalized against any time this cfs_rq has spent throttled */
स्थिर अन्तःस्थ u64 cfs_rq_clock_pelt(संरचना cfs_rq *cfs_rq)
{
	यदि (unlikely(cfs_rq->pelt_clock_throttled))
		प्रत्यागम cfs_rq->throttled_clock_pelt - cfs_rq->throttled_clock_pelt_time;

	प्रत्यागम rq_clock_pelt(rq_सम्बन्धि(cfs_rq)) - cfs_rq->throttled_clock_pelt_time;
}
#अन्यथा /* !CONFIG_CFS_BANDWIDTH: */
स्थिर अन्तःस्थ शून्य update_idle_cfs_rq_clock_pelt(संरचना cfs_rq *cfs_rq) { }
स्थिर अन्तःस्थ u64 cfs_rq_clock_pelt(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम rq_clock_pelt(rq_सम्बन्धि(cfs_rq));
}
#यदि_अन्तः /* !CONFIG_CFS_BANDWIDTH */

#यदि_अन्तः /* _KERNEL_SCHED_PELT_H */
