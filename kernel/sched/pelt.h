// SPDX-License-Identifier: GPL-2.0
#ifndef _KERNEL_SCHED_PELT_H
#define _KERNEL_SCHED_PELT_H
#அந்தர்பாவ "sched.h"

#அந்தர்பாவ "sched-pelt.h"

முழு __update_load_avg_blocked_se(u64 now, கட்டமைப்பு sched_entity *se);
முழு __update_load_avg_se(u64 now, கட்டமைப்பு cfs_rq *cfs_rq, கட்டமைப்பு sched_entity *se);
முழு __update_load_avg_cfs_rq(u64 now, கட்டமைப்பு cfs_rq *cfs_rq);
முழு update_rt_rq_load_avg(u64 now, கட்டமைப்பு rq *rq, முழு running);
முழு update_dl_rq_load_avg(u64 now, கட்டமைப்பு rq *rq, முழு running);
bool update_other_load_avgs(கட்டமைப்பு rq *rq);

#ifdef CONFIG_SCHED_HW_PRESSURE
முழு update_hw_load_avg(u64 now, கட்டமைப்பு rq *rq, u64 capacity);

நிலையான உள்வரி u64 hw_load_avg(கட்டமைப்பு rq *rq)
{
	திரும்பு READ_ONCE(rq->avg_hw.load_avg);
}
#இல்லையெனில் /* !CONFIG_SCHED_HW_PRESSURE: */
நிலையான உள்வரி முழு
update_hw_load_avg(u64 now, கட்டமைப்பு rq *rq, u64 capacity)
{
	திரும்பு 0;
}

நிலையான உள்வரி u64 hw_load_avg(கட்டமைப்பு rq *rq)
{
	திரும்பு 0;
}
#endif /* !CONFIG_SCHED_HW_PRESSURE */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
முழு update_irq_load_avg(கட்டமைப்பு rq *rq, u64 running);
#இல்லையெனில்
நிலையான உள்வரி முழு
update_irq_load_avg(கட்டமைப்பு rq *rq, u64 running)
{
	திரும்பு 0;
}
#endif

#define PELT_MIN_DIVIDER	(LOAD_AVG_MAX - 1024)

நிலையான உள்வரி u32 get_pelt_divider(கட்டமைப்பு sched_avg *avg)
{
	திரும்பு PELT_MIN_DIVIDER + avg->period_contrib;
}

நிலையான உள்வரி வெறுமை cfs_se_util_change(கட்டமைப்பு sched_avg *avg)
{
	unsigned முழு enqueued;

	எனில் (!sched_feat(UTIL_EST))
		திரும்பு;

	/* Avoid store if the flag has been already reset */
	enqueued = avg->util_est;
	எனில் (!(enqueued & UTIL_AVG_UNCHANGED))
		திரும்பு;

	/* Reset flag to report util_avg has been updated */
	enqueued &= ~UTIL_AVG_UNCHANGED;
	WRITE_ONCE(avg->util_est, enqueued);
}

நிலையான உள்வரி u64 rq_clock_pelt(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	திரும்பு rq->clock_pelt - rq->lost_idle_time;
}

/* The rq is idle, we can sync to clock_task */
நிலையான உள்வரி வெறுமை _update_idle_rq_clock_pelt(கட்டமைப்பு rq *rq)
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
நிலையான உள்வரி வெறுமை update_rq_clock_pelt(கட்டமைப்பு rq *rq, s64 delta)
{
	எனில் (unlikely(is_idle_task(rq->curr))) {
		_update_idle_rq_clock_pelt(rq);
		திரும்பு;
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
	delta = cap_scale(delta, arch_scale_cpu_capacity(cpu_of(rq)));
	delta = cap_scale(delta, arch_scale_freq_capacity(cpu_of(rq)));

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
நிலையான உள்வரி வெறுமை update_idle_rq_clock_pelt(கட்டமைப்பு rq *rq)
{
	u32 divider = ((LOAD_AVG_MAX - 1024) << SCHED_CAPACITY_SHIFT) - LOAD_AVG_MAX;
	u32 util_sum = rq->cfs.avg.util_sum;
	util_sum += rq->avg_rt.util_sum;
	util_sum += rq->avg_dl.util_sum;

	/*
	 * Reflecting stolen time makes sense only if the idle
	 * phase would be present at max capacity. As soon as the
	 * utilization of a rq has reached the maximum value, it is
	 * considered as an always running rq without idle time to
	 * steal. This potential idle time is considered as lost in
	 * this case. We keep track of this lost idle time compare to
	 * rq's clock_task.
	 */
	எனில் (util_sum >= divider)
		rq->lost_idle_time += rq_clock_task(rq) - rq->clock_pelt;

	_update_idle_rq_clock_pelt(rq);
}

#ifdef CONFIG_CFS_BANDWIDTH
நிலையான உள்வரி வெறுமை update_idle_cfs_rq_clock_pelt(கட்டமைப்பு cfs_rq *cfs_rq)
{
	u64 throttled;

	எனில் (unlikely(cfs_rq->pelt_clock_throttled))
		throttled = U64_MAX;
	இல்லையெனில்
		throttled = cfs_rq->throttled_clock_pelt_time;

	u64_u32_store(cfs_rq->throttled_pelt_idle, throttled);
}

/* rq->task_clock normalized against any time this cfs_rq has spent throttled */
நிலையான உள்வரி u64 cfs_rq_clock_pelt(கட்டமைப்பு cfs_rq *cfs_rq)
{
	எனில் (unlikely(cfs_rq->pelt_clock_throttled))
		திரும்பு cfs_rq->throttled_clock_pelt - cfs_rq->throttled_clock_pelt_time;

	திரும்பு rq_clock_pelt(rq_of(cfs_rq)) - cfs_rq->throttled_clock_pelt_time;
}
#இல்லையெனில் /* !CONFIG_CFS_BANDWIDTH: */
நிலையான உள்வரி வெறுமை update_idle_cfs_rq_clock_pelt(கட்டமைப்பு cfs_rq *cfs_rq) { }
நிலையான உள்வரி u64 cfs_rq_clock_pelt(கட்டமைப்பு cfs_rq *cfs_rq)
{
	திரும்பு rq_clock_pelt(rq_of(cfs_rq));
}
#endif /* !CONFIG_CFS_BANDWIDTH */

#endif /* _KERNEL_SCHED_PELT_H */
