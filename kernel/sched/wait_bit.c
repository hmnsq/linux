// SPDX-License-Identifier: GPL-2.0-only

#ضمن <linux/sched/debug.h>
#ضمن "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#وصف_ڪريو WAIT_جدول_بٽ 8
#وصف_ڪريو WAIT_جدول_سائيز (1 << WAIT_جدول_بٽ)

ساكن wait_قطار_سر_t ٻٽو_انگ_wait_جدول[WAIT_جدول_سائيز] __cacheline_aligned;

wait_قطار_سر_t *ٻٽو_انگ_waitqueue(بنانشان ڊگهو *لفظ, صحيح ٻٽو_انگ_2)
{
	ثابت صحيح سرڪايو = بٽ_في_LONG == 32 ? 5 : 6;
	بنانشان ڊگهو val = (بنانشان ڊگهو)لفظ << سرڪايو | ٻٽو_انگ_2;

	أرجع ٻٽو_انگ_wait_جدول + hash_long(val, WAIT_جدول_بٽ);
}
EXPORT_نشان(ٻٽو_انگ_waitqueue);

صحيح wake_ٻٽو_انگ_function(بنية wait_قطار_داخلا *wq_داخلا, بنانشان گهڻائي, صحيح sync, فراغ *arg)
{
	بنية wait_ٻٽو_انگ_ڪنجي *ڪنجي = arg;
	بنية wait_ٻٽو_انگ_قطار_داخلا *wait_ٻٽو_انگ = container_جو(wq_داخلا, بنية wait_ٻٽو_انگ_قطار_داخلا, wq_داخلا);

	إذا (wait_ٻٽو_انگ->ڪنجي.flags != ڪنجي->flags ||
			wait_ٻٽو_انگ->ڪنجي.ٻٽو_انگ_nr != ڪنجي->ٻٽو_انگ_nr ||
			test_ٻٽو_انگ(ڪنجي->ٻٽو_انگ_nr, ڪنجي->flags))
		أرجع 0;

	أرجع autoremove_wake_function(wq_داخلا, گهڻائي, sync, ڪنجي);
}
EXPORT_نشان(wake_ٻٽو_انگ_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
صحيح __sched
__wait_on_bit(بنية wait_قطار_سر *wq_سر, بنية wait_ٻٽو_انگ_قطار_داخلا *wbq_داخلا,
	      wait_ٻٽو_انگ_action_f *action, بنانشان گهڻائي)
{
	صحيح ret = 0;

	افعل {
		prepare_منزل_wait(wq_سر, &wbq_داخلا->wq_داخلا, گهڻائي);
		إذا (test_ٻٽو_انگ(wbq_داخلا->ڪنجي.ٻٽو_انگ_nr, wbq_داخلا->ڪنجي.flags))
			ret = (*action)(&wbq_داخلا->ڪنجي, گهڻائي);
	} طالما (test_ٻٽو_انگ_acquire(wbq_داخلا->ڪنجي.ٻٽو_انگ_nr, wbq_داخلا->ڪنجي.flags) && !ret);

	پڄاڻي_wait(wq_سر, &wbq_داخلا->wq_داخلا);

	أرجع ret;
}
EXPORT_نشان(__wait_on_bit);

صحيح __sched ٻاهريون_جو_سٽ_wait_on_ٻٽو_انگ(بنانشان ڊگهو *لفظ, صحيح ٻٽو_انگ_2,
				    wait_ٻٽو_انگ_action_f *action, بنانشان گهڻائي)
{
	بنية wait_قطار_سر *wq_سر = ٻٽو_انگ_waitqueue(لفظ, ٻٽو_انگ_2);
	DEFINE_WAIT_ٻٽو_انگ(wq_داخلا, لفظ, ٻٽو_انگ_2);

	أرجع __wait_on_bit(wq_سر, &wq_داخلا, action, گهڻائي);
}
EXPORT_نشان(ٻاهريون_جو_سٽ_wait_on_ٻٽو_انگ);

صحيح __sched ٻاهريون_جو_سٽ_wait_on_ٻٽو_انگ_timeout(
	بنانشان ڊگهو *لفظ, صحيح ٻٽو_انگ_2, wait_ٻٽو_انگ_action_f *action,
	بنانشان گهڻائي, بنانشان ڊگهو timeout)
{
	بنية wait_قطار_سر *wq_سر = ٻٽو_انگ_waitqueue(لفظ, ٻٽو_انگ_2);
	DEFINE_WAIT_ٻٽو_انگ(wq_داخلا, لفظ, ٻٽو_انگ_2);

	wq_داخلا.ڪنجي.timeout = jiffies + timeout;

	أرجع __wait_on_bit(wq_سر, &wq_داخلا, action, گهڻائي);
}
EXPORT_نشان_GPL(ٻاهريون_جو_سٽ_wait_on_ٻٽو_انگ_timeout);

صحيح __sched
__wait_on_bit_lock(بنية wait_قطار_سر *wq_سر, بنية wait_ٻٽو_انگ_قطار_داخلا *wbq_داخلا,
			wait_ٻٽو_انگ_action_f *action, بنانشان گهڻائي)
{
	صحيح ret = 0;

	لكل (;;) {
		prepare_منزل_wait_exclusive(wq_سر, &wbq_داخلا->wq_داخلا, گهڻائي);
		إذا (test_ٻٽو_انگ(wbq_داخلا->ڪنجي.ٻٽو_انگ_nr, wbq_داخلا->ڪنجي.flags)) {
			ret = action(&wbq_داخلا->ڪنجي, گهڻائي);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			إذا (ret)
				پڄاڻي_wait(wq_سر, &wbq_داخلا->wq_داخلا);
		}
		إذا (!test_and_سيٽ_ٻٽو_انگ(wbq_داخلا->ڪنجي.ٻٽو_انگ_nr, wbq_داخلا->ڪنجي.flags)) {
			إذا (!ret)
				پڄاڻي_wait(wq_سر, &wbq_داخلا->wq_داخلا);
			أرجع 0;
		} وإلا إذا (ret) {
			أرجع ret;
		}
	}
}
EXPORT_نشان(__wait_on_bit_lock);

صحيح __sched ٻاهريون_جو_سٽ_wait_on_ٻٽو_انگ_تالو(بنانشان ڊگهو *لفظ, صحيح ٻٽو_انگ_2,
					 wait_ٻٽو_انگ_action_f *action, بنانشان گهڻائي)
{
	بنية wait_قطار_سر *wq_سر = ٻٽو_انگ_waitqueue(لفظ, ٻٽو_انگ_2);
	DEFINE_WAIT_ٻٽو_انگ(wq_داخلا, لفظ, ٻٽو_انگ_2);

	أرجع __wait_on_bit_lock(wq_سر, &wq_داخلا, action, گهڻائي);
}
EXPORT_نشان(ٻاهريون_جو_سٽ_wait_on_ٻٽو_انگ_تالو);

فراغ __wake_up_bit(بنية wait_قطار_سر *wq_سر, بنانشان ڊگهو *لفظ, صحيح ٻٽو_انگ_2)
{
	بنية wait_ٻٽو_انگ_ڪنجي ڪنجي = __WAIT_BIT_KEY_INITIALIZER(لفظ, ٻٽو_انگ_2);

	إذا (waitqueue_سرگرم(wq_سر))
		__wake_up(wq_سر, TASK_عام, 1, &ڪنجي);
}
EXPORT_نشان(__wake_up_bit);

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
فراغ wake_مٿي_ٻٽو_انگ(بنانشان ڊگهو *لفظ, صحيح ٻٽو_انگ_2)
{
	__wake_up_bit(ٻٽو_انگ_waitqueue(لفظ, ٻٽو_انگ_2), لفظ, ٻٽو_انگ_2);
}
EXPORT_نشان(wake_مٿي_ٻٽو_انگ);

wait_قطار_سر_t *__var_waitqueue(فراغ *p)
{
	أرجع ٻٽو_انگ_wait_جدول + hash_ptr(p, WAIT_جدول_بٽ);
}
EXPORT_نشان(__var_waitqueue);

بنية wait_ٻٽو_انگ_ڪنجي *__var_wake_key(بنية wait_قطار_داخلا *wq_داخلا, فراغ *arg)
{
	بنية wait_ٻٽو_انگ_ڪنجي *ڪنجي = arg;
	بنية wait_ٻٽو_انگ_قطار_داخلا *wbq_داخلا =
		container_جو(wq_داخلا, بنية wait_ٻٽو_انگ_قطار_داخلا, wq_داخلا);

	إذا (wbq_داخلا->ڪنجي.flags != ڪنجي->flags ||
	    wbq_داخلا->ڪنجي.ٻٽو_انگ_nr != ڪنجي->ٻٽو_انگ_nr)
		أرجع عدم;

	أرجع ڪنجي;
}

ساكن صحيح var_wake_function(بنية wait_قطار_داخلا *wq_داخلا, بنانشان صحيح گهڻائي,
			     صحيح sync, فراغ *arg)
{
	بنية wait_ٻٽو_انگ_ڪنجي *ڪنجي = __var_wake_key(wq_داخلا, arg);
	إذا (!ڪنجي)
		أرجع 0;

	أرجع autoremove_wake_function(wq_داخلا, گهڻائي, sync, ڪنجي);
}

فراغ init_wait_var_داخلا(بنية wait_ٻٽو_انگ_قطار_داخلا *wbq_داخلا, فراغ *var, صحيح flags)
{
	*wbq_داخلا = (بنية wait_ٻٽو_انگ_قطار_داخلا){
		.ڪنجي = {
			.flags	= (var),
			.ٻٽو_انگ_nr = -1,
		},
		.wq_داخلا = {
			.flags	 = flags,
			.خاص = current,
			.func	 = var_wake_function,
			.داخلا	 = LIST_سر_INIT(wbq_داخلا->wq_داخلا.داخلا),
		},
	};
}
EXPORT_نشان(init_wait_var_داخلا);

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
فراغ wake_مٿي_var(فراغ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_نشان(wake_مٿي_var);

__sched صحيح ٻٽو_انگ_wait(بنية wait_ٻٽو_انگ_ڪنجي *لفظ, صحيح گهڻائي)
{
	وقت_جدول();
	إذا (signal_pending_state(گهڻائي, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نشان(ٻٽو_انگ_wait);

__sched صحيح ٻٽو_انگ_wait_io(بنية wait_ٻٽو_انگ_ڪنجي *لفظ, صحيح گهڻائي)
{
	io_وقت_جدول();
	إذا (signal_pending_state(گهڻائي, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نشان(ٻٽو_انگ_wait_io);

__sched صحيح ٻٽو_انگ_wait_timeout(بنية wait_ٻٽو_انگ_ڪنجي *لفظ, صحيح گهڻائي)
{
	بنانشان ڊگهو now = READ_ONCE(jiffies);

	إذا (وقت_after_eq(now, لفظ->timeout))
		أرجع -EAGAIN;
	وقت_جدول_timeout(لفظ->timeout - now);
	إذا (signal_pending_state(گهڻائي, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نشان_GPL(ٻٽو_انگ_wait_timeout);

فراغ __init wait_ٻٽو_انگ_init(فراغ)
{
	صحيح i;

	لكل (i = 0; i < WAIT_جدول_سائيز; i++)
		init_waitqueue_سر(ٻٽو_انگ_wait_جدول + i);
}
