// SPDX-License-Identifier: GPL-2.0
#که_نه_تعریف_شوی _KERNEL_SCHED_PELT_H
#تعریف_کړه _KERNEL_SCHED_PELT_H
#ضمن "sched.h"

#ضمن "sched-pelt.h"

صحيح __update_load_avg_blocked_se(u64 now, بنية sched_entity *se);
صحيح __update_load_avg_se(u64 now, بنية cfs_rq *cfs_rq, بنية sched_entity *se);
صحيح __update_load_avg_cfs_rq(u64 now, بنية cfs_rq *cfs_rq);
صحيح update_rt_rq_بارول_avg(u64 now, بنية rq *rq, صحيح running);
صحيح update_dl_rq_بارول_avg(u64 now, بنية rq *rq, صحيح running);
bool update_other_بارول_avgs(بنية rq *rq);

#که_تعریف_شوی CONFIG_SCHED_HW_PRESSURE
صحيح update_hw_بارول_avg(u64 now, بنية rq *rq, u64 ظرفیت);

ساكن مضمن u64 hw_بارول_avg(بنية rq *rq)
{
	أرجع READ_ONCE(rq->avg_hw.بارول_avg);
}
#وإلا /* !CONFIG_SCHED_HW_PRESSURE: */
ساكن مضمن صحيح
update_hw_بارول_avg(u64 now, بنية rq *rq, u64 ظرفیت)
{
	أرجع 0;
}

ساكن مضمن u64 hw_بارول_avg(بنية rq *rq)
{
	أرجع 0;
}
#د_که_پای /* !CONFIG_SCHED_HW_PRESSURE */

#که_تعریف_شوی CONFIG_HAVE_SCHED_AVG_IRQ
صحيح update_irq_بارول_avg(بنية rq *rq, u64 running);
#وإلا
ساكن مضمن صحيح
update_irq_بارول_avg(بنية rq *rq, u64 running)
{
	أرجع 0;
}
#د_که_پای

#تعریف_کړه PELT_MIN_DIVIDER	(بارول_AVG_MAX - 1024)

ساكن مضمن u32 get_pelt_divider(بنية sched_avg *avg)
{
	أرجع PELT_MIN_DIVIDER + avg->period_contrib;
}

ساكن مضمن فراغ cfs_se_util_change(بنية sched_avg *avg)
{
	بینښه صحيح enqueued;

	إذا (!sched_feat(UTIL_EST))
		أرجع;

	/* Avoid store if the flag has been already reset */
	enqueued = avg->util_est;
	إذا (!(enqueued & UTIL_AVG_UNCHANGED))
		أرجع;

	/* Reset flag to report util_avg has been updated */
	enqueued &= ~UTIL_AVG_UNCHANGED;
	WRITE_ONCE(avg->util_est, enqueued);
}

ساكن مضمن u64 rq_clock_pelt(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	أرجع rq->clock_pelt - rq->ورک_idle_وخت;
}

/* The rq is idle, we can sync to clock_task */
ساكن مضمن فراغ _update_idle_rq_clock_pelt(بنية rq *rq)
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
ساكن مضمن فراغ update_rq_clock_pelt(بنية rq *rq, s64 delta)
{
	إذا (unlikely(is_idle_task(rq->curr))) {
		_update_idle_rq_clock_pelt(rq);
		أرجع;
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
	delta = cap_scale(delta, arch_scale_cpu_ظرفیت(cpu_د(rq)));
	delta = cap_scale(delta, arch_scale_freq_ظرفیت(cpu_د(rq)));

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
ساكن مضمن فراغ update_idle_rq_clock_pelt(بنية rq *rq)
{
	u32 divider = ((بارول_AVG_MAX - 1024) << SCHED_ظرفیت_ښويدنه) - بارول_AVG_MAX;
	u32 util_مجموعه = rq->cfs.avg.util_مجموعه;
	util_مجموعه += rq->avg_rt.util_مجموعه;
	util_مجموعه += rq->avg_dl.util_مجموعه;

	/*
	 * Reflecting stolen time makes sense only if the idle
	 * phase would be present at max capacity. As soon as the
	 * utilization of a rq has reached the maximum value, it is
	 * considered as an always running rq without idle time to
	 * steal. This potential idle time is considered as lost in
	 * this case. We keep track of this lost idle time compare to
	 * rq's clock_task.
	 */
	إذا (util_مجموعه >= divider)
		rq->ورک_idle_وخت += rq_clock_task(rq) - rq->clock_pelt;

	_update_idle_rq_clock_pelt(rq);
}

#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
ساكن مضمن فراغ update_idle_cfs_rq_clock_pelt(بنية cfs_rq *cfs_rq)
{
	u64 throttled;

	إذا (unlikely(cfs_rq->pelt_clock_throttled))
		throttled = U64_MAX;
	وإلا
		throttled = cfs_rq->throttled_clock_pelt_وخت;

	u64_u32_store(cfs_rq->throttled_pelt_idle, throttled);
}

/* rq->task_clock normalized against any time this cfs_rq has spent throttled */
ساكن مضمن u64 cfs_rq_clock_pelt(بنية cfs_rq *cfs_rq)
{
	إذا (unlikely(cfs_rq->pelt_clock_throttled))
		أرجع cfs_rq->throttled_clock_pelt - cfs_rq->throttled_clock_pelt_وخت;

	أرجع rq_clock_pelt(rq_د(cfs_rq)) - cfs_rq->throttled_clock_pelt_وخت;
}
#وإلا /* !CONFIG_CFS_BANDWIDTH: */
ساكن مضمن فراغ update_idle_cfs_rq_clock_pelt(بنية cfs_rq *cfs_rq) { }
ساكن مضمن u64 cfs_rq_clock_pelt(بنية cfs_rq *cfs_rq)
{
	أرجع rq_clock_pelt(rq_د(cfs_rq));
}
#د_که_پای /* !CONFIG_CFS_BANDWIDTH */

#د_که_پای /* _KERNEL_SCHED_PELT_H */
