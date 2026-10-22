// SPDX-License-Identifier: GPL-2.0
#अगर_अघोषित _KERNEL_SCHED_PELT_H
#घोषणा _KERNEL_SCHED_PELT_H
#समावेश "sched.h"

#समावेश "sched-pelt.h"

पूर्णांक __update_load_avg_blocked_se(u64 now, काष्ठा sched_entity *se);
पूर्णांक __update_load_avg_se(u64 now, काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se);
पूर्णांक __update_load_avg_cfs_rq(u64 now, काष्ठा cfs_rq *cfs_rq);
पूर्णांक update_rt_rq_load_avg(u64 now, काष्ठा rq *rq, पूर्णांक running);
पूर्णांक update_dl_rq_load_avg(u64 now, काष्ठा rq *rq, पूर्णांक running);
bool update_other_load_avgs(काष्ठा rq *rq);

#अगर_घोषित CONFIG_SCHED_HW_PRESSURE
पूर्णांक update_hw_load_avg(u64 now, काष्ठा rq *rq, u64 capacity);

अटल अंतरभूत u64 hw_load_avg(काष्ठा rq *rq)
{
	वापस READ_ONCE(rq->avg_hw.load_avg);
}
#अन्यथा /* !CONFIG_SCHED_HW_PRESSURE: */
अटल अंतरभूत पूर्णांक
update_hw_load_avg(u64 now, काष्ठा rq *rq, u64 capacity)
{
	वापस 0;
}

अटल अंतरभूत u64 hw_load_avg(काष्ठा rq *rq)
{
	वापस 0;
}
#पूर्ण_अगर /* !CONFIG_SCHED_HW_PRESSURE */

#अगर_घोषित CONFIG_HAVE_SCHED_AVG_IRQ
पूर्णांक update_irq_load_avg(काष्ठा rq *rq, u64 running);
#अन्यथा
अटल अंतरभूत पूर्णांक
update_irq_load_avg(काष्ठा rq *rq, u64 running)
{
	वापस 0;
}
#पूर्ण_अगर

#घोषणा PELT_MIN_DIVIDER	(LOAD_AVG_MAX - 1024)

अटल अंतरभूत u32 get_pelt_divider(काष्ठा sched_avg *avg)
{
	वापस PELT_MIN_DIVIDER + avg->period_contrib;
}

अटल अंतरभूत व्योम cfs_se_util_change(काष्ठा sched_avg *avg)
{
	अचिन्हित पूर्णांक enqueued;

	अगर (!sched_feat(UTIL_EST))
		वापस;

	/* Avoid store if the flag has been already reset */
	enqueued = avg->util_est;
	अगर (!(enqueued & UTIL_AVG_UNCHANGED))
		वापस;

	/* Reset flag to report util_avg has been updated */
	enqueued &= ~UTIL_AVG_UNCHANGED;
	WRITE_ONCE(avg->util_est, enqueued);
}

अटल अंतरभूत u64 rq_clock_pelt(काष्ठा rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	वापस rq->clock_pelt - rq->lost_idle_time;
}

/* The rq is idle, we can sync to clock_task */
अटल अंतरभूत व्योम _update_idle_rq_clock_pelt(काष्ठा rq *rq)
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
अटल अंतरभूत व्योम update_rq_clock_pelt(काष्ठा rq *rq, s64 delta)
{
	अगर (unlikely(is_idle_task(rq->curr))) {
		_update_idle_rq_clock_pelt(rq);
		वापस;
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
अटल अंतरभूत व्योम update_idle_rq_clock_pelt(काष्ठा rq *rq)
{
	u32 divider = ((LOAD_AVG_MAX - 1024) << SCHED_CAPACITY_SHIFT) - LOAD_AVG_MAX;
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
	अगर (util_योग >= divider)
		rq->lost_idle_time += rq_clock_task(rq) - rq->clock_pelt;

	_update_idle_rq_clock_pelt(rq);
}

#अगर_घोषित CONFIG_CFS_BANDWIDTH
अटल अंतरभूत व्योम update_idle_cfs_rq_clock_pelt(काष्ठा cfs_rq *cfs_rq)
{
	u64 throttled;

	अगर (unlikely(cfs_rq->pelt_clock_throttled))
		throttled = U64_MAX;
	अन्यथा
		throttled = cfs_rq->throttled_clock_pelt_time;

	u64_u32_store(cfs_rq->throttled_pelt_idle, throttled);
}

/* rq->task_clock normalized against any time this cfs_rq has spent throttled */
अटल अंतरभूत u64 cfs_rq_clock_pelt(काष्ठा cfs_rq *cfs_rq)
{
	अगर (unlikely(cfs_rq->pelt_clock_throttled))
		वापस cfs_rq->throttled_clock_pelt - cfs_rq->throttled_clock_pelt_time;

	वापस rq_clock_pelt(rq_of(cfs_rq)) - cfs_rq->throttled_clock_pelt_time;
}
#अन्यथा /* !CONFIG_CFS_BANDWIDTH: */
अटल अंतरभूत व्योम update_idle_cfs_rq_clock_pelt(काष्ठा cfs_rq *cfs_rq) { }
अटल अंतरभूत u64 cfs_rq_clock_pelt(काष्ठा cfs_rq *cfs_rq)
{
	वापस rq_clock_pelt(rq_of(cfs_rq));
}
#पूर्ण_अगर /* !CONFIG_CFS_BANDWIDTH */

#पूर्ण_अगर /* _KERNEL_SCHED_PELT_H */
