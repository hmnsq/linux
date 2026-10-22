// SPDX-License-Identifier: GPL-2.0-only

#అన్తర్భావ <linux/sched/debug.h>
#అన్తర్భావ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#నిర్వచించు WAIT_పట్టిక_బిట్లు 8
#నిర్వచించు WAIT_పట్టిక_కొలత (1 << WAIT_పట్టిక_బిట్లు)

స్థిర wait_వరుస_తల_t బిట్టు_wait_పట్టిక[WAIT_పట్టిక_కొలత] __cacheline_aligned;

wait_వరుస_తల_t *బిట్టు_waitqueue(గుర్తులేని దీర్ఘ *మాట, పూర్ణాంకం బిట్టు_2)
{
	స్థిరరాశి పూర్ణాంకం జరుపు = బిట్లు_చొప్పున_LONG == 32 ? 5 : 6;
	గుర్తులేని దీర్ఘ val = (గుర్తులేని దీర్ఘ)మాట << జరుపు | బిట్టు_2;

	తిరిగి బిట్టు_wait_పట్టిక + hash_long(val, WAIT_పట్టిక_బిట్లు);
}
EXPORT_చిహ్నం(బిట్టు_waitqueue);

పూర్ణాంకం wake_బిట్టు_function(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, గుర్తులేని బహుళకం, పూర్ణాంకం sync, శూన్య *arg)
{
	నిర్మాణం wait_బిట్టు_కీలకం *కీలకం = arg;
	నిర్మాణం wait_బిట్టు_వరుస_నమోదు *wait_బిట్టు = container_యొక్క(wq_నమోదు, నిర్మాణం wait_బిట్టు_వరుస_నమోదు, wq_నమోదు);

	ఒకవేళ (wait_బిట్టు->కీలకం.flags != కీలకం->flags ||
			wait_బిట్టు->కీలకం.బిట్టు_nr != కీలకం->బిట్టు_nr ||
			test_బిట్టు(కీలకం->బిట్టు_nr, కీలకం->flags))
		తిరిగి 0;

	తిరిగి autoremove_wake_function(wq_నమోదు, బహుళకం, sync, కీలకం);
}
EXPORT_చిహ్నం(wake_బిట్టు_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
పూర్ణాంకం __sched
__wait_on_bit(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_బిట్టు_వరుస_నమోదు *wbq_నమోదు,
	      wait_బిట్టు_action_f *action, గుర్తులేని బహుళకం)
{
	పూర్ణాంకం ret = 0;

	చేయి {
		prepare_గమ్యం_wait(wq_తల, &wbq_నమోదు->wq_నమోదు, బహుళకం);
		ఒకవేళ (test_బిట్టు(wbq_నమోదు->కీలకం.బిట్టు_nr, wbq_నమోదు->కీలకం.flags))
			ret = (*action)(&wbq_నమోదు->కీలకం, బహుళకం);
	} అయితే (test_బిట్టు_acquire(wbq_నమోదు->కీలకం.బిట్టు_nr, wbq_నమోదు->కీలకం.flags) && !ret);

	ముగింపు_wait(wq_తల, &wbq_నమోదు->wq_నమోదు);

	తిరిగి ret;
}
EXPORT_చిహ్నం(__wait_on_bit);

పూర్ణాంకం __sched బహిర్గతం_యొక్క_పంక్తి_wait_on_బిట్టు(గుర్తులేని దీర్ఘ *మాట, పూర్ణాంకం బిట్టు_2,
				    wait_బిట్టు_action_f *action, గుర్తులేని బహుళకం)
{
	నిర్మాణం wait_వరుస_తల *wq_తల = బిట్టు_waitqueue(మాట, బిట్టు_2);
	DEFINE_WAIT_బిట్టు(wq_నమోదు, మాట, బిట్టు_2);

	తిరిగి __wait_on_bit(wq_తల, &wq_నమోదు, action, బహుళకం);
}
EXPORT_చిహ్నం(బహిర్గతం_యొక్క_పంక్తి_wait_on_బిట్టు);

పూర్ణాంకం __sched బహిర్గతం_యొక్క_పంక్తి_wait_on_బిట్టు_timeout(
	గుర్తులేని దీర్ఘ *మాట, పూర్ణాంకం బిట్టు_2, wait_బిట్టు_action_f *action,
	గుర్తులేని బహుళకం, గుర్తులేని దీర్ఘ timeout)
{
	నిర్మాణం wait_వరుస_తల *wq_తల = బిట్టు_waitqueue(మాట, బిట్టు_2);
	DEFINE_WAIT_బిట్టు(wq_నమోదు, మాట, బిట్టు_2);

	wq_నమోదు.కీలకం.timeout = jiffies + timeout;

	తిరిగి __wait_on_bit(wq_తల, &wq_నమోదు, action, బహుళకం);
}
EXPORT_చిహ్నం_GPL(బహిర్గతం_యొక్క_పంక్తి_wait_on_బిట్టు_timeout);

పూర్ణాంకం __sched
__wait_on_bit_lock(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_బిట్టు_వరుస_నమోదు *wbq_నమోదు,
			wait_బిట్టు_action_f *action, గుర్తులేని బహుళకం)
{
	పూర్ణాంకం ret = 0;

	కొరకు (;;) {
		prepare_గమ్యం_wait_exclusive(wq_తల, &wbq_నమోదు->wq_నమోదు, బహుళకం);
		ఒకవేళ (test_బిట్టు(wbq_నమోదు->కీలకం.బిట్టు_nr, wbq_నమోదు->కీలకం.flags)) {
			ret = action(&wbq_నమోదు->కీలకం, బహుళకం);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			ఒకవేళ (ret)
				ముగింపు_wait(wq_తల, &wbq_నమోదు->wq_నమోదు);
		}
		ఒకవేళ (!test_and_సమితి_బిట్టు(wbq_నమోదు->కీలకం.బిట్టు_nr, wbq_నమోదు->కీలకం.flags)) {
			ఒకవేళ (!ret)
				ముగింపు_wait(wq_తల, &wbq_నమోదు->wq_నమోదు);
			తిరిగి 0;
		} లేకపోతే ఒకవేళ (ret) {
			తిరిగి ret;
		}
	}
}
EXPORT_చిహ్నం(__wait_on_bit_lock);

పూర్ణాంకం __sched బహిర్గతం_యొక్క_పంక్తి_wait_on_బిట్టు_తాళం(గుర్తులేని దీర్ఘ *మాట, పూర్ణాంకం బిట్టు_2,
					 wait_బిట్టు_action_f *action, గుర్తులేని బహుళకం)
{
	నిర్మాణం wait_వరుస_తల *wq_తల = బిట్టు_waitqueue(మాట, బిట్టు_2);
	DEFINE_WAIT_బిట్టు(wq_నమోదు, మాట, బిట్టు_2);

	తిరిగి __wait_on_bit_lock(wq_తల, &wq_నమోదు, action, బహుళకం);
}
EXPORT_చిహ్నం(బహిర్గతం_యొక్క_పంక్తి_wait_on_బిట్టు_తాళం);

శూన్య __wake_up_bit(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని దీర్ఘ *మాట, పూర్ణాంకం బిట్టు_2)
{
	నిర్మాణం wait_బిట్టు_కీలకం కీలకం = __WAIT_BIT_KEY_INITIALIZER(మాట, బిట్టు_2);

	ఒకవేళ (waitqueue_చురుకైన(wq_తల))
		__wake_up(wq_తల, TASK_సాధారణ, 1, &కీలకం);
}
EXPORT_చిహ్నం(__wake_up_bit);

/**
 * wake_up_bit - wake up waiters on a bit
 * @word: the address containing the bit being waited on
 * @bit: the bit at that address being waited on
 *
 * Wake up any process waiting in wait_on_bit() or similar for the
 * given bit to be cleared.
 *
 * The wake-up is sent to tasks in a waitqueue selected by hash from a
 * shared pool.  Only those tasks on that queue which have requested
 * wake_up on this specific address and bit will be woken, and only if the
 * bit is clear.
 *
 * In order for this to function properly there must be a full memory
 * barrier after the bit is cleared and before this function is called.
 * If the bit was cleared atomically, such as a by clear_bit() then
 * smb_mb__after_atomic() can be used, othwewise smb_mb() is needed.
 * If the bit was cleared with a fully-ordered operation, no further
 * barrier is required.
 *
 * Normally the bit should be cleared by an operation with RELEASE
 * semantics so that any changes to memory made before the bit is
 * cleared are guaranteed to be visible after the matching wait_on_bit()
 * completes.
 */
శూన్య wake_పైకి_బిట్టు(గుర్తులేని దీర్ఘ *మాట, పూర్ణాంకం బిట్టు_2)
{
	__wake_up_bit(బిట్టు_waitqueue(మాట, బిట్టు_2), మాట, బిట్టు_2);
}
EXPORT_చిహ్నం(wake_పైకి_బిట్టు);

wait_వరుస_తల_t *__var_waitqueue(శూన్య *p)
{
	తిరిగి బిట్టు_wait_పట్టిక + hash_ptr(p, WAIT_పట్టిక_బిట్లు);
}
EXPORT_చిహ్నం(__var_waitqueue);

నిర్మాణం wait_బిట్టు_కీలకం *__var_wake_key(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, శూన్య *arg)
{
	నిర్మాణం wait_బిట్టు_కీలకం *కీలకం = arg;
	నిర్మాణం wait_బిట్టు_వరుస_నమోదు *wbq_నమోదు =
		container_యొక్క(wq_నమోదు, నిర్మాణం wait_బిట్టు_వరుస_నమోదు, wq_నమోదు);

	ఒకవేళ (wbq_నమోదు->కీలకం.flags != కీలకం->flags ||
	    wbq_నమోదు->కీలకం.బిట్టు_nr != కీలకం->బిట్టు_nr)
		తిరిగి ఖాళీ;

	తిరిగి కీలకం;
}

స్థిర పూర్ణాంకం var_wake_function(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, గుర్తులేని పూర్ణాంకం బహుళకం,
			     పూర్ణాంకం sync, శూన్య *arg)
{
	నిర్మాణం wait_బిట్టు_కీలకం *కీలకం = __var_wake_key(wq_నమోదు, arg);
	ఒకవేళ (!కీలకం)
		తిరిగి 0;

	తిరిగి autoremove_wake_function(wq_నమోదు, బహుళకం, sync, కీలకం);
}

శూన్య init_wait_var_నమోదు(నిర్మాణం wait_బిట్టు_వరుస_నమోదు *wbq_నమోదు, శూన్య *var, పూర్ణాంకం flags)
{
	*wbq_నమోదు = (నిర్మాణం wait_బిట్టు_వరుస_నమోదు){
		.కీలకం = {
			.flags	= (var),
			.బిట్టు_nr = -1,
		},
		.wq_నమోదు = {
			.flags	 = flags,
			.అంతరంగ = current,
			.func	 = var_wake_function,
			.నమోదు_2	 = LIST_తల_INIT(wbq_నమోదు->wq_నమోదు.నమోదు_2),
		},
	};
}
EXPORT_చిహ్నం(init_wait_var_నమోదు);

/**
 * wake_up_var - wake up waiters on a variable (kernel address)
 * @var: the address of the variable being waited on
 *
 * Wake up any process waiting in wait_var_event() or similar for the
 * given variable to change.  wait_var_event() can be waiting for an
 * arbitrary condition to be true and associates that condition with an
 * address.  Calling wake_up_var() suggests that the condition has been
 * made true, but does not strictly require the condtion to use the
 * address given.
 *
 * The wake-up is sent to tasks in a waitqueue selected by hash from a
 * shared pool.  Only those tasks on that queue which have requested
 * wake_up on this specific address will be woken.
 *
 * In order for this to function properly there must be a full memory
 * barrier after the variable is updated (or more accurately, after the
 * condition waited on has been made to be true) and before this function
 * is called.  If the variable was updated atomically, such as a by
 * atomic_dec() then smb_mb__after_atomic() can be used.  If the
 * variable was updated by a fully ordered operation such as
 * atomic_dec_and_test() then no extra barrier is required.  Otherwise
 * smb_mb() is needed.
 *
 * Normally the variable should be updated (the condition should be made
 * to be true) by an operation with RELEASE semantics such as
 * smp_store_release() so that any changes to memory made before the
 * variable was updated are guaranteed to be visible after the matching
 * wait_var_event() completes.
 */
శూన్య wake_పైకి_var(శూన్య *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_చిహ్నం(wake_పైకి_var);

__sched పూర్ణాంకం బిట్టు_wait(నిర్మాణం wait_బిట్టు_కీలకం *మాట, పూర్ణాంకం బహుళకం)
{
	కాలపట్టిక();
	ఒకవేళ (signal_pending_state(బహుళకం, current))
		తిరిగి -EINTR;

	తిరిగి 0;
}
EXPORT_చిహ్నం(బిట్టు_wait);

__sched పూర్ణాంకం బిట్టు_wait_io(నిర్మాణం wait_బిట్టు_కీలకం *మాట, పూర్ణాంకం బహుళకం)
{
	io_కాలపట్టిక();
	ఒకవేళ (signal_pending_state(బహుళకం, current))
		తిరిగి -EINTR;

	తిరిగి 0;
}
EXPORT_చిహ్నం(బిట్టు_wait_io);

__sched పూర్ణాంకం బిట్టు_wait_timeout(నిర్మాణం wait_బిట్టు_కీలకం *మాట, పూర్ణాంకం బహుళకం)
{
	గుర్తులేని దీర్ఘ now = READ_ONCE(jiffies);

	ఒకవేళ (సమయం_after_eq(now, మాట->timeout))
		తిరిగి -EAGAIN;
	కాలపట్టిక_timeout(మాట->timeout - now);
	ఒకవేళ (signal_pending_state(బహుళకం, current))
		తిరిగి -EINTR;

	తిరిగి 0;
}
EXPORT_చిహ్నం_GPL(బిట్టు_wait_timeout);

శూన్య __init wait_బిట్టు_init(శూన్య)
{
	పూర్ణాంకం i;

	కొరకు (i = 0; i < WAIT_పట్టిక_కొలత; i++)
		init_waitqueue_తల(బిట్టు_wait_పట్టిక + i);
}
