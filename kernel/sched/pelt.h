// SPDX-License-Identifier: GPL-2.0
#اگر_غیر_تعریف_شدہ _KERNEL_SCHED_PELT_H
#تعریف_کرو _KERNEL_SCHED_PELT_H
#شامل "sched.h"

#شامل "sched-pelt.h"

صحیح __update_load_avg_blocked_se(u64 now, ساخت sched_entity *se);
صحیح __update_load_avg_se(u64 now, ساخت cfs_rq *cfs_rq, ساخت sched_entity *se);
صحیح __update_load_avg_cfs_rq(u64 now, ساخت cfs_rq *cfs_rq);
صحیح update_rt_rq_بھرو_avg(u64 now, ساخت rq *rq, صحیح running);
صحیح update_dl_rq_بھرو_avg(u64 now, ساخت rq *rq, صحیح running);
bool update_other_بھرو_avgs(ساخت rq *rq);

#اگر_تعریف_شدہ CONFIG_SCHED_HW_PRESSURE
صحیح update_hw_بھرو_avg(u64 now, ساخت rq *rq, u64 استعداد);

ساکن اندرونی u64 hw_بھرو_avg(ساخت rq *rq)
{
	واپس READ_ONCE(rq->avg_hw.بھرو_avg);
}
#ورنہ /* !CONFIG_SCHED_HW_PRESSURE: */
ساکن اندرونی صحیح
update_hw_بھرو_avg(u64 now, ساخت rq *rq, u64 استعداد)
{
	واپس 0;
}

ساکن اندرونی u64 hw_بھرو_avg(ساخت rq *rq)
{
	واپس 0;
}
#اگر_ختم /* !CONFIG_SCHED_HW_PRESSURE */

#اگر_تعریف_شدہ CONFIG_HAVE_SCHED_AVG_IRQ
صحیح update_irq_بھرو_avg(ساخت rq *rq, u64 running);
#ورنہ
ساکن اندرونی صحیح
update_irq_بھرو_avg(ساخت rq *rq, u64 running)
{
	واپس 0;
}
#اگر_ختم

#تعریف_کرو PELT_MIN_DIVIDER	(بھرو_AVG_MAX - 1024)

ساکن اندرونی u32 get_pelt_divider(ساخت sched_avg *avg)
{
	واپس PELT_MIN_DIVIDER + avg->period_contrib;
}

ساکن اندرونی خالی cfs_se_util_change(ساخت sched_avg *avg)
{
	غیرعلامتی صحیح enqueued;

	اگر (!sched_feat(UTIL_EST))
		واپس;

	/* Avoid store if the flag has been already reset */
	enqueued = avg->util_est;
	اگر (!(enqueued & UTIL_AVG_UNCHANGED))
		واپس;

	/* Reset flag to report util_avg has been updated */
	enqueued &= ~UTIL_AVG_UNCHANGED;
	WRITE_ONCE(avg->util_est, enqueued);
}

ساکن اندرونی u64 rq_clock_pelt(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	واپس rq->clock_pelt - rq->گمشدہ_idle_وقت;
}

/* The rq is idle, we can sync to clock_task */
ساکن اندرونی خالی _update_idle_rq_clock_pelt(ساخت rq *rq)
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
ساکن اندرونی خالی update_rq_clock_pelt(ساخت rq *rq, s64 delta)
{
	اگر (unlikely(is_idle_task(rq->curr))) {
		_update_idle_rq_clock_pelt(rq);
		واپس;
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
	delta = cap_scale(delta, arch_scale_cpu_استعداد(cpu_کا(rq)));
	delta = cap_scale(delta, arch_scale_freq_استعداد(cpu_کا(rq)));

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
ساکن اندرونی خالی update_idle_rq_clock_pelt(ساخت rq *rq)
{
	u32 divider = ((بھرو_AVG_MAX - 1024) << SCHED_استعداد_سرکاؤ) - بھرو_AVG_MAX;
	u32 util_مجموع = rq->cfs.avg.util_مجموع;
	util_مجموع += rq->avg_rt.util_مجموع;
	util_مجموع += rq->avg_dl.util_مجموع;

	/*
	 * Reflecting stolen time makes sense only if the idle
	 * phase would be present at max capacity. As soon as the
	 * utilization of a rq has reached the maximum value, it is
	 * considered as an always running rq without idle time to
	 * steal. This potential idle time is considered as lost in
	 * this case. We keep track of this lost idle time compare to
	 * rq's clock_task.
	 */
	اگر (util_مجموع >= divider)
		rq->گمشدہ_idle_وقت += rq_clock_task(rq) - rq->clock_pelt;

	_update_idle_rq_clock_pelt(rq);
}

#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
ساکن اندرونی خالی update_idle_cfs_rq_clock_pelt(ساخت cfs_rq *cfs_rq)
{
	u64 throttled;

	اگر (unlikely(cfs_rq->pelt_clock_throttled))
		throttled = U64_MAX;
	ورنہ
		throttled = cfs_rq->throttled_clock_pelt_وقت;

	u64_u32_store(cfs_rq->throttled_pelt_idle, throttled);
}

/* rq->task_clock normalized against any time this cfs_rq has spent throttled */
ساکن اندرونی u64 cfs_rq_clock_pelt(ساخت cfs_rq *cfs_rq)
{
	اگر (unlikely(cfs_rq->pelt_clock_throttled))
		واپس cfs_rq->throttled_clock_pelt - cfs_rq->throttled_clock_pelt_وقت;

	واپس rq_clock_pelt(rq_کا(cfs_rq)) - cfs_rq->throttled_clock_pelt_وقت;
}
#ورنہ /* !CONFIG_CFS_BANDWIDTH: */
ساکن اندرونی خالی update_idle_cfs_rq_clock_pelt(ساخت cfs_rq *cfs_rq) { }
ساکن اندرونی u64 cfs_rq_clock_pelt(ساخت cfs_rq *cfs_rq)
{
	واپس rq_clock_pelt(rq_کا(cfs_rq));
}
#اگر_ختم /* !CONFIG_CFS_BANDWIDTH */

#اگر_ختم /* _KERNEL_SCHED_PELT_H */
