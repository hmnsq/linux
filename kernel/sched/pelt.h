// SPDX-License-Identifier: GPL-2.0
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_PELT_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_PELT_H
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "sched-pelt.h"

ਪੂਰਨ __update_load_avg_blocked_se(u64 now, ਬਣਤਰ sched_entity *se);
ਪੂਰਨ __update_load_avg_se(u64 now, ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se);
ਪੂਰਨ __update_load_avg_cfs_rq(u64 now, ਬਣਤਰ cfs_rq *cfs_rq);
ਪੂਰਨ update_rt_rq_ਲੱਦੋ_avg(u64 now, ਬਣਤਰ rq *rq, ਪੂਰਨ running);
ਪੂਰਨ update_dl_rq_ਲੱਦੋ_avg(u64 now, ਬਣਤਰ rq *rq, ਪੂਰਨ running);
bool update_other_ਲੱਦੋ_avgs(ਬਣਤਰ rq *rq);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_HW_PRESSURE
ਪੂਰਨ update_hw_ਲੱਦੋ_avg(u64 now, ਬਣਤਰ rq *rq, u64 ਸਮਰੱਥਾ);

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 hw_ਲੱਦੋ_avg(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ READ_ONCE(rq->avg_hw.ਲੱਦੋ_avg);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_HW_PRESSURE: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
update_hw_ਲੱਦੋ_avg(u64 now, ਬਣਤਰ rq *rq, u64 ਸਮਰੱਥਾ)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 hw_ਲੱਦੋ_avg(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_HW_PRESSURE */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HAVE_SCHED_AVG_IRQ
ਪੂਰਨ update_irq_ਲੱਦੋ_avg(ਬਣਤਰ rq *rq, u64 running);
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
update_irq_ਲੱਦੋ_avg(ਬਣਤਰ rq *rq, u64 running)
{
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PELT_MIN_DIVIDER	(ਲੱਦੋ_AVG_MAX - 1024)

ਸਥਿਰ ਸਤਰਅੰਦਰ u32 get_pelt_divider(ਬਣਤਰ sched_avg *avg)
{
	ਵਾਪਸ PELT_MIN_DIVIDER + avg->period_contrib;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cfs_se_util_change(ਬਣਤਰ sched_avg *avg)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ enqueued;

	ਜੇ (!sched_feat(UTIL_EST))
		ਵਾਪਸ;

	/* Avoid store if the flag has been already reset */
	enqueued = avg->util_est;
	ਜੇ (!(enqueued & UTIL_AVG_UNCHANGED))
		ਵਾਪਸ;

	/* Reset flag to report util_avg has been updated */
	enqueued &= ~UTIL_AVG_UNCHANGED;
	WRITE_ONCE(avg->util_est, enqueued);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 rq_clock_pelt(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ਵਾਪਸ rq->clock_pelt - rq->ਗੁਆਚਾ_idle_ਸਮਾਂ;
}

/* The rq is idle, we can sync to clock_task */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ _update_idle_rq_clock_pelt(ਬਣਤਰ rq *rq)
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_rq_clock_pelt(ਬਣਤਰ rq *rq, s64 delta)
{
	ਜੇ (unlikely(is_idle_task(rq->curr))) {
		_update_idle_rq_clock_pelt(rq);
		ਵਾਪਸ;
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
	delta = cap_scale(delta, arch_scale_cpu_ਸਮਰੱਥਾ(cpu_ਦਾ(rq)));
	delta = cap_scale(delta, arch_scale_freq_ਸਮਰੱਥਾ(cpu_ਦਾ(rq)));

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_idle_rq_clock_pelt(ਬਣਤਰ rq *rq)
{
	u32 divider = ((ਲੱਦੋ_AVG_MAX - 1024) << SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ) - ਲੱਦੋ_AVG_MAX;
	u32 util_ਜੋੜ = rq->cfs.avg.util_ਜੋੜ;
	util_ਜੋੜ += rq->avg_rt.util_ਜੋੜ;
	util_ਜੋੜ += rq->avg_dl.util_ਜੋੜ;

	/*
	 * Reflecting stolen time makes sense only if the idle
	 * phase would be present at max capacity. As soon as the
	 * utilization of a rq has reached the maximum value, it is
	 * considered as an always running rq without idle time to
	 * steal. This potential idle time is considered as lost in
	 * this case. We keep track of this lost idle time compare to
	 * rq's clock_task.
	 */
	ਜੇ (util_ਜੋੜ >= divider)
		rq->ਗੁਆਚਾ_idle_ਸਮਾਂ += rq_clock_task(rq) - rq->clock_pelt;

	_update_idle_rq_clock_pelt(rq);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_idle_cfs_rq_clock_pelt(ਬਣਤਰ cfs_rq *cfs_rq)
{
	u64 throttled;

	ਜੇ (unlikely(cfs_rq->pelt_clock_throttled))
		throttled = U64_MAX;
	ਨਹੀਂਤਾਂ
		throttled = cfs_rq->throttled_clock_pelt_ਸਮਾਂ;

	u64_u32_store(cfs_rq->throttled_pelt_idle, throttled);
}

/* rq->task_clock normalized against any time this cfs_rq has spent throttled */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 cfs_rq_clock_pelt(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਜੇ (unlikely(cfs_rq->pelt_clock_throttled))
		ਵਾਪਸ cfs_rq->throttled_clock_pelt - cfs_rq->throttled_clock_pelt_ਸਮਾਂ;

	ਵਾਪਸ rq_clock_pelt(rq_ਦਾ(cfs_rq)) - cfs_rq->throttled_clock_pelt_ਸਮਾਂ;
}
#ਨਹੀਂਤਾਂ /* !CONFIG_CFS_BANDWIDTH: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_idle_cfs_rq_clock_pelt(ਬਣਤਰ cfs_rq *cfs_rq) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 cfs_rq_clock_pelt(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ rq_clock_pelt(rq_ਦਾ(cfs_rq));
}
#ਜੇ_ਅੰਤ /* !CONFIG_CFS_BANDWIDTH */

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_PELT_H */
