// SPDX-License-Identifier: GPL-2.0
/*
 * kernel/sched/loadavg.c
 *
 * This file contains the magic bits required to compute the global loadavg
 * figure. Its a silly number but people think its important. We go through
 * great pains to make it work on big machines and tickless kernels.
 */
#شامل <linux/sched/nohz.h>
#شامل "sched.h"

/*
 * Global load-average calculations
 *
 * We take a distributed and async approach to calculating the global load-avg
 * in order to minimize overhead.
 *
 * The global load average is an exponentially decaying average of nr_running +
 * nr_uninterruptible.
 *
 * Once every LOAD_FREQ:
 *
 *   nr_active = 0;
 *   for_each_possible_cpu(cpu)
 *	nr_active += cpu_of(cpu)->nr_running + cpu_of(cpu)->nr_uninterruptible;
 *
 *   avenrun[n] = avenrun[0] * exp_n + nr_active * (1 - exp_n)
 *
 * Due to a number of reasons the above turns in the mess below:
 *
 *  - for_each_possible_cpu() is prohibitively expensive on machines with
 *    serious number of CPUs, therefore we need to take a distributed approach
 *    to calculating nr_active.
 *
 *        \Sum_i x_i(t) = \Sum_i x_i(t) - x_i(t_0) | x_i(t_0) := 0
 *                      = \Sum_i { \Sum_j=1 x_i(t_j) - x_i(t_j-1) }
 *
 *    So assuming nr_active := 0 when we start out -- true per definition, we
 *    can simply take per-CPU deltas and fold those into a global accumulate
 *    to obtain the same result. See calc_load_fold_active().
 *
 *    Furthermore, in order to avoid synchronizing all per-CPU delta folding
 *    across the machine, we assume 10 ticks is sufficient time for every
 *    CPU to have completed this task.
 *
 *    This places an upper-bound on the IRQ-off latency of the machine. Then
 *    again, being late doesn't loose the delta, just wrecks the sample.
 *
 *  - cpu_rq()->nr_uninterruptible isn't accurately tracked per-CPU because
 *    this would add another cross-CPU cache-line miss and atomic operation
 *    to the wakeup path. Instead we increment on whatever CPU the task ran
 *    when it went into uninterruptible state and decrement on whatever CPU
 *    did the wakeup. This means that only the sum of nr_uninterruptible over
 *    all CPUs yields the correct result.
 *
 *  This covers the NO_HZ=n code, for extra head-aches, see the comment below.
 */

/* Variables and functions for calc_load */
atomic_long_t calc_بھرو_tasks;
غیرعلامتی طویل calc_بھرو_update;
غیرعلامتی طویل avenrun[3];
EXPORT_علامت(avenrun); /* should be removed */

/**
 * get_avenrun - get the load average array
 * @loads:	pointer to destination load array
 * @offset:	offset to add
 * @shift:	shift count to shift the result left
 *
 * These values are estimates at best, so no need for locking.
 */
خالی get_avenrun(غیرعلامتی طویل *loads, غیرعلامتی طویل انحراف, صحیح سرکاؤ)
{
	loads[0] = (avenrun[0] + انحراف) << سرکاؤ;
	loads[1] = (avenrun[1] + انحراف) << سرکاؤ;
	loads[2] = (avenrun[2] + انحراف) << سرکاؤ;
}

طویل calc_بھرو_fold_فعال(ساخت rq *this_rq, طویل adjust)
{
	طویل nr_فعال, delta = 0;

	nr_فعال = this_rq->nr_running - adjust;
	nr_فعال += (طویل)this_rq->nr_uninterruptible;

	اگر (nr_فعال != this_rq->calc_بھرو_فعال) {
		delta = nr_فعال - this_rq->calc_بھرو_فعال;
		this_rq->calc_بھرو_فعال = nr_فعال;
	}

	واپس delta;
}

/**
 * fixed_power_int - compute: x^n, in O(log n) time
 *
 * @x:         base of the power
 * @frac_bits: fractional bits of @x
 * @n:         power to raise @x to.
 *
 * By exploiting the relation between the definition of the natural power
 * function: x^n := x*x*...*x (x multiplied by itself for n times), and
 * the binary encoding of numbers used by computers: n := \Sum n_i * 2^i,
 * (where: n_i \elem {0, 1}, the binary vector representing n),
 * we find: x^n := x^(\Sum n_i * 2^i) := \Prod x^(n_i * 2^i), which is
 * of course trivially computable in O(log_2 n), the length of our binary
 * vector.
 */
ساکن غیرعلامتی طویل
fixed_power_int(غیرعلامتی طویل x, غیرعلامتی صحیح frac_بٹ, غیرعلامتی صحیح n)
{
	غیرعلامتی طویل نتیجہ = 1UL << frac_بٹ;

	اگر (n) {
		برائے (;;) {
			اگر (n & 1) {
				نتیجہ *= x;
				نتیجہ += 1UL << (frac_بٹ - 1);
				نتیجہ >>= frac_بٹ;
			}
			n >>= 1;
			اگر (!n)
				توڑو;
			x *= x;
			x += 1UL << (frac_بٹ - 1);
			x >>= frac_بٹ;
		}
	}

	واپس نتیجہ;
}

/*
 * a1 = a0 * e + a * (1 - e)
 *
 * a2 = a1 * e + a * (1 - e)
 *    = (a0 * e + a * (1 - e)) * e + a * (1 - e)
 *    = a0 * e^2 + a * (1 - e) * (1 + e)
 *
 * a3 = a2 * e + a * (1 - e)
 *    = (a0 * e^2 + a * (1 - e) * (1 + e)) * e + a * (1 - e)
 *    = a0 * e^3 + a * (1 - e) * (1 + e + e^2)
 *
 *  ...
 *
 * an = a0 * e^n + a * (1 - e) * (1 + e + ... + e^n-1) [1]
 *    = a0 * e^n + a * (1 - e) * (1 - e^n)/(1 - e)
 *    = a0 * e^n + a * (1 - e^n)
 *
 * [1] application of the geometric series:
 *
 *              n         1 - x^(n+1)
 *     S_n := \Sum x^i = -------------
 *             i=0          1 - x
 */
غیرعلامتی طویل
calc_بھرو_n(غیرعلامتی طویل بھرو, غیرعلامتی طویل قوت_نما,
	    غیرعلامتی طویل فعال, غیرعلامتی صحیح n)
{
	واپس calc_بھرو(بھرو, fixed_power_int(قوت_نما, FSHIFT, n), فعال);
}

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
/*
 * Handle NO_HZ for the global load-average.
 *
 * Since the above described distributed algorithm to compute the global
 * load-average relies on per-CPU sampling from the tick, it is affected by
 * NO_HZ.
 *
 * The basic idea is to fold the nr_active delta into a global NO_HZ-delta upon
 * entering NO_HZ state such that we can include this as an 'extra' CPU delta
 * when we read the global state.
 *
 * Obviously reality has to ruin such a delightfully simple scheme:
 *
 *  - When we go NO_HZ idle during the window, we can negate our sample
 *    contribution, causing under-accounting.
 *
 *    We avoid this by keeping two NO_HZ-delta counters and flipping them
 *    when the window starts, thus separating old and new NO_HZ load.
 *
 *    The only trick is the slight shift in index flip for read vs write.
 *
 *        0s            5s            10s           15s
 *          +10           +10           +10           +10
 *        |-|-----------|-|-----------|-|-----------|-|
 *    r:0 0 1           1 0           0 1           1 0
 *    w:0 1 1           0 0           1 1           0 0
 *
 *    This ensures we'll fold the old NO_HZ contribution in this window while
 *    accumulating the new one.
 *
 *  - When we wake up from NO_HZ during the window, we push up our
 *    contribution, since we effectively move our sample point to a known
 *    busy state.
 *
 *    This is solved by pushing the window forward, and thus skipping the
 *    sample, for this CPU (effectively using the NO_HZ-delta for this CPU which
 *    was in effect at the time the window opened). This also solves the issue
 *    of having to deal with a CPU having been in NO_HZ for multiple LOAD_FREQ
 *    intervals.
 *
 * When making the ILB scale, we should try to pull this in as well.
 */
ساکن atomic_long_t calc_بھرو_nohz[2];
ساکن صحیح calc_بھرو_idx;

ساکن اندرونی صحیح calc_بھرو_write_idx(خالی)
{
	صحیح idx = calc_بھرو_idx;

	/*
	 * See calc_global_nohz(), if we observe the new index, we also
	 * need to observe the new update time.
	 */
	smp_rmb();

	/*
	 * If the folding window started, make sure we start writing in the
	 * next NO_HZ-delta.
	 */
	اگر (!وقت_before(jiffies, READ_ONCE(calc_بھرو_update)))
		idx++;

	واپس idx & 1;
}

ساکن اندرونی صحیح calc_بھرو_read_idx(خالی)
{
	واپس calc_بھرو_idx & 1;
}

ساکن خالی calc_بھرو_nohz_fold(ساخت rq *rq)
{
	طویل delta;

	delta = calc_بھرو_fold_فعال(rq, 0);
	اگر (delta) {
		صحیح idx = calc_بھرو_write_idx();

		atomic_long_جوڑو(delta, &calc_بھرو_nohz[idx]);
	}
}

خالی calc_بھرو_nohz_آغاز(خالی)
{
	/*
	 * We're going into NO_HZ mode, if there's any pending delta, fold it
	 * into the pending NO_HZ delta.
	 */
	calc_بھرو_nohz_fold(this_rq());
}

/*
 * Keep track of the load for NOHZ_FULL, must be called between
 * calc_load_nohz_{start,stop}().
 */
خالی calc_بھرو_nohz_remote(ساخت rq *rq)
{
	calc_بھرو_nohz_fold(rq);
}

خالی calc_بھرو_nohz_stop(خالی)
{
	ساخت rq *this_rq = this_rq();

	/*
	 * If we're still before the pending sample window, we're done.
	 */
	this_rq->calc_بھرو_update = READ_ONCE(calc_بھرو_update);
	اگر (وقت_before(jiffies, this_rq->calc_بھرو_update))
		واپس;

	/*
	 * We woke inside or after the sample window, this means we're already
	 * accounted through the nohz accounting, so skip the entire deal and
	 * sync up for the next window.
	 */
	اگر (وقت_before(jiffies, this_rq->calc_بھرو_update + 10))
		this_rq->calc_بھرو_update += بھرو_FREQ;
}

ساکن طویل calc_بھرو_nohz_read(خالی)
{
	صحیح idx = calc_بھرو_read_idx();
	طویل delta = 0;

	اگر (atomic_long_read(&calc_بھرو_nohz[idx]))
		delta = atomic_long_xchg(&calc_بھرو_nohz[idx], 0);

	واپس delta;
}

/*
 * NO_HZ can leave us missing all per-CPU ticks calling
 * calc_load_fold_active(), but since a NO_HZ CPU folds its delta into
 * calc_load_nohz per calc_load_nohz_start(), all we need to do is fold
 * in the pending NO_HZ delta if our NO_HZ period crossed a load cycle boundary.
 *
 * Once we've updated the global active value, we need to apply the exponential
 * weights adjusted to the number of cycles missed.
 */
ساکن خالی calc_global_nohz(خالی)
{
	غیرعلامتی طویل sample_کھڑکی;
	طویل delta, فعال, n;

	sample_کھڑکی = READ_ONCE(calc_بھرو_update);
	اگر (!وقت_before(jiffies, sample_کھڑکی + 10)) {
		/*
		 * Catch-up, fold however many we are behind still
		 */
		delta = jiffies - sample_کھڑکی - 10;
		n = 1 + (delta / بھرو_FREQ);

		فعال = atomic_long_read(&calc_بھرو_tasks);
		فعال = فعال > 0 ? فعال * FIXED_1 : 0;

		avenrun[0] = calc_بھرو_n(avenrun[0], EXP_1, فعال, n);
		avenrun[1] = calc_بھرو_n(avenrun[1], EXP_5, فعال, n);
		avenrun[2] = calc_بھرو_n(avenrun[2], EXP_15, فعال, n);

		WRITE_ONCE(calc_بھرو_update, sample_کھڑکی + n * بھرو_FREQ);
	}

	/*
	 * Flip the NO_HZ index...
	 *
	 * Make sure we first write the new time then flip the index, so that
	 * calc_load_write_idx() will see the new time when it reads the new
	 * index, this avoids a double flip messing things up.
	 */
	smp_wmb();
	calc_بھرو_idx++;
}
#ورنہ /* !CONFIG_NO_HZ_COMMON: */

ساکن اندرونی طویل calc_بھرو_nohz_read(خالی) { واپس 0; }
ساکن اندرونی خالی calc_global_nohz(خالی) { }

#اگر_ختم /* !CONFIG_NO_HZ_COMMON */

/*
 * calc_load - update the avenrun load estimates 10 ticks after the
 * CPUs have updated calc_load_tasks.
 *
 * Called from the global timer code.
 */
خالی calc_global_بھرو(خالی)
{
	غیرعلامتی طویل sample_کھڑکی;
	طویل فعال, delta;

	sample_کھڑکی = READ_ONCE(calc_بھرو_update);
	اگر (وقت_before(jiffies, sample_کھڑکی + 10))
		واپس;

	/*
	 * Fold the 'old' NO_HZ-delta to include all NO_HZ CPUs.
	 */
	delta = calc_بھرو_nohz_read();
	اگر (delta)
		atomic_long_جوڑو(delta, &calc_بھرو_tasks);

	فعال = atomic_long_read(&calc_بھرو_tasks);
	فعال = فعال > 0 ? فعال * FIXED_1 : 0;

	avenrun[0] = calc_بھرو(avenrun[0], EXP_1, فعال);
	avenrun[1] = calc_بھرو(avenrun[1], EXP_5, فعال);
	avenrun[2] = calc_بھرو(avenrun[2], EXP_15, فعال);

	WRITE_ONCE(calc_بھرو_update, sample_کھڑکی + بھرو_FREQ);

	/*
	 * In case we went to NO_HZ for multiple LOAD_FREQ intervals
	 * catch up in bulk.
	 */
	calc_global_nohz();
}

/*
 * Called from sched_tick() to periodically update this CPU's
 * active count.
 */
خالی calc_global_بھرو_tick(ساخت rq *this_rq)
{
	طویل delta;

	اگر (وقت_before(jiffies, this_rq->calc_بھرو_update))
		واپس;

	delta  = calc_بھرو_fold_فعال(this_rq, 0);
	اگر (delta)
		atomic_long_جوڑو(delta, &calc_بھرو_tasks);

	this_rq->calc_بھرو_update += بھرو_FREQ;
}
