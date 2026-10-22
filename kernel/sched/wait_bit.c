// SPDX-License-Identifier: GPL-2.0-only

#ضمن <linux/sched/debug.h>
#ضمن "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#تعریف_کرو WAIT_جدول_بٹ 8
#تعریف_کرو WAIT_جدول_ناپ (1 << WAIT_جدول_بٹ)

ساكن wait_قطار_سر_t ثنائی_ہندسہ_wait_جدول[WAIT_جدول_ناپ] __cacheline_aligned;

wait_قطار_سر_t *ثنائی_ہندسہ_waitqueue(بنانشانی لما *لفظ, صحيح ثنائی_ہندسہ_2)
{
	ثابت صحيح کھسکاؤ = بٹ_فی_LONG == 32 ? 5 : 6;
	بنانشانی لما val = (بنانشانی لما)لفظ << کھسکاؤ | ثنائی_ہندسہ_2;

	أرجع ثنائی_ہندسہ_wait_جدول + hash_long(val, WAIT_جدول_بٹ);
}
EXPORT_علامت(ثنائی_ہندسہ_waitqueue);

صحيح wake_ثنائی_ہندسہ_function(بنية wait_قطار_اندراج *wq_اندراج, بنانشانی منوال, صحيح sync, فراغ *arg)
{
	بنية wait_ثنائی_ہندسہ_کلید *کلید = arg;
	بنية wait_ثنائی_ہندسہ_قطار_اندراج *wait_ثنائی_ہندسہ = container_دا(wq_اندراج, بنية wait_ثنائی_ہندسہ_قطار_اندراج, wq_اندراج);

	إذا (wait_ثنائی_ہندسہ->کلید.flags != کلید->flags ||
			wait_ثنائی_ہندسہ->کلید.ثنائی_ہندسہ_nr != کلید->ثنائی_ہندسہ_nr ||
			test_ثنائی_ہندسہ(کلید->ثنائی_ہندسہ_nr, کلید->flags))
		أرجع 0;

	أرجع autoremove_wake_function(wq_اندراج, منوال, sync, کلید);
}
EXPORT_علامت(wake_ثنائی_ہندسہ_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
صحيح __sched
__wait_on_bit(بنية wait_قطار_سر *wq_سر, بنية wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج,
	      wait_ثنائی_ہندسہ_action_f *action, بنانشانی منوال)
{
	صحيح ret = 0;

	افعل {
		prepare_منزل_wait(wq_سر, &wbq_اندراج->wq_اندراج, منوال);
		إذا (test_ثنائی_ہندسہ(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags))
			ret = (*action)(&wbq_اندراج->کلید, منوال);
	} طالما (test_ثنائی_ہندسہ_acquire(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags) && !ret);

	انت_wait(wq_سر, &wbq_اندراج->wq_اندراج);

	أرجع ret;
}
EXPORT_علامت(__wait_on_bit);

صحيح __sched باہرلا_دا_سطر_wait_on_ثنائی_ہندسہ(بنانشانی لما *لفظ, صحيح ثنائی_ہندسہ_2,
				    wait_ثنائی_ہندسہ_action_f *action, بنانشانی منوال)
{
	بنية wait_قطار_سر *wq_سر = ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2);
	DEFINE_WAIT_ثنائی_ہندسہ(wq_اندراج, لفظ, ثنائی_ہندسہ_2);

	أرجع __wait_on_bit(wq_سر, &wq_اندراج, action, منوال);
}
EXPORT_علامت(باہرلا_دا_سطر_wait_on_ثنائی_ہندسہ);

صحيح __sched باہرلا_دا_سطر_wait_on_ثنائی_ہندسہ_timeout(
	بنانشانی لما *لفظ, صحيح ثنائی_ہندسہ_2, wait_ثنائی_ہندسہ_action_f *action,
	بنانشانی منوال, بنانشانی لما timeout)
{
	بنية wait_قطار_سر *wq_سر = ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2);
	DEFINE_WAIT_ثنائی_ہندسہ(wq_اندراج, لفظ, ثنائی_ہندسہ_2);

	wq_اندراج.کلید.timeout = jiffies + timeout;

	أرجع __wait_on_bit(wq_سر, &wq_اندراج, action, منوال);
}
EXPORT_علامت_GPL(باہرلا_دا_سطر_wait_on_ثنائی_ہندسہ_timeout);

صحيح __sched
__wait_on_bit_lock(بنية wait_قطار_سر *wq_سر, بنية wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج,
			wait_ثنائی_ہندسہ_action_f *action, بنانشانی منوال)
{
	صحيح ret = 0;

	لكل (;;) {
		prepare_منزل_wait_exclusive(wq_سر, &wbq_اندراج->wq_اندراج, منوال);
		إذا (test_ثنائی_ہندسہ(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags)) {
			ret = action(&wbq_اندراج->کلید, منوال);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			إذا (ret)
				انت_wait(wq_سر, &wbq_اندراج->wq_اندراج);
		}
		إذا (!test_and_سیٹ_ثنائی_ہندسہ(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags)) {
			إذا (!ret)
				انت_wait(wq_سر, &wbq_اندراج->wq_اندراج);
			أرجع 0;
		} وإلا إذا (ret) {
			أرجع ret;
		}
	}
}
EXPORT_علامت(__wait_on_bit_lock);

صحيح __sched باہرلا_دا_سطر_wait_on_ثنائی_ہندسہ_جندرا(بنانشانی لما *لفظ, صحيح ثنائی_ہندسہ_2,
					 wait_ثنائی_ہندسہ_action_f *action, بنانشانی منوال)
{
	بنية wait_قطار_سر *wq_سر = ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2);
	DEFINE_WAIT_ثنائی_ہندسہ(wq_اندراج, لفظ, ثنائی_ہندسہ_2);

	أرجع __wait_on_bit_lock(wq_سر, &wq_اندراج, action, منوال);
}
EXPORT_علامت(باہرلا_دا_سطر_wait_on_ثنائی_ہندسہ_جندرا);

فراغ __wake_up_bit(بنية wait_قطار_سر *wq_سر, بنانشانی لما *لفظ, صحيح ثنائی_ہندسہ_2)
{
	بنية wait_ثنائی_ہندسہ_کلید کلید = __WAIT_BIT_KEY_INITIALIZER(لفظ, ثنائی_ہندسہ_2);

	إذا (waitqueue_سرگرم(wq_سر))
		__wake_up(wq_سر, TASK_عام, 1, &کلید);
}
EXPORT_علامت(__wake_up_bit);

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
فراغ wake_اتے_ثنائی_ہندسہ(بنانشانی لما *لفظ, صحيح ثنائی_ہندسہ_2)
{
	__wake_up_bit(ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2), لفظ, ثنائی_ہندسہ_2);
}
EXPORT_علامت(wake_اتے_ثنائی_ہندسہ);

wait_قطار_سر_t *__var_waitqueue(فراغ *p)
{
	أرجع ثنائی_ہندسہ_wait_جدول + hash_ptr(p, WAIT_جدول_بٹ);
}
EXPORT_علامت(__var_waitqueue);

بنية wait_ثنائی_ہندسہ_کلید *__var_wake_key(بنية wait_قطار_اندراج *wq_اندراج, فراغ *arg)
{
	بنية wait_ثنائی_ہندسہ_کلید *کلید = arg;
	بنية wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج =
		container_دا(wq_اندراج, بنية wait_ثنائی_ہندسہ_قطار_اندراج, wq_اندراج);

	إذا (wbq_اندراج->کلید.flags != کلید->flags ||
	    wbq_اندراج->کلید.ثنائی_ہندسہ_nr != کلید->ثنائی_ہندسہ_nr)
		أرجع عدم;

	أرجع کلید;
}

ساكن صحيح var_wake_function(بنية wait_قطار_اندراج *wq_اندراج, بنانشانی صحيح منوال,
			     صحيح sync, فراغ *arg)
{
	بنية wait_ثنائی_ہندسہ_کلید *کلید = __var_wake_key(wq_اندراج, arg);
	إذا (!کلید)
		أرجع 0;

	أرجع autoremove_wake_function(wq_اندراج, منوال, sync, کلید);
}

فراغ init_wait_var_اندراج(بنية wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج, فراغ *var, صحيح flags)
{
	*wbq_اندراج = (بنية wait_ثنائی_ہندسہ_قطار_اندراج){
		.کلید = {
			.flags	= (var),
			.ثنائی_ہندسہ_nr = -1,
		},
		.wq_اندراج = {
			.flags	 = flags,
			.خاص = current,
			.func	 = var_wake_function,
			.اندراج	 = LIST_سر_INIT(wbq_اندراج->wq_اندراج.اندراج),
		},
	};
}
EXPORT_علامت(init_wait_var_اندراج);

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
فراغ wake_اتے_var(فراغ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_علامت(wake_اتے_var);

__sched صحيح ثنائی_ہندسہ_wait(بنية wait_ثنائی_ہندسہ_کلید *لفظ, صحيح منوال)
{
	ویلا_سارنی();
	إذا (signal_pending_state(منوال, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_علامت(ثنائی_ہندسہ_wait);

__sched صحيح ثنائی_ہندسہ_wait_io(بنية wait_ثنائی_ہندسہ_کلید *لفظ, صحيح منوال)
{
	io_ویلا_سارنی();
	إذا (signal_pending_state(منوال, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_علامت(ثنائی_ہندسہ_wait_io);

__sched صحيح ثنائی_ہندسہ_wait_timeout(بنية wait_ثنائی_ہندسہ_کلید *لفظ, صحيح منوال)
{
	بنانشانی لما now = READ_ONCE(jiffies);

	إذا (ویلا_after_eq(now, لفظ->timeout))
		أرجع -EAGAIN;
	ویلا_سارنی_timeout(لفظ->timeout - now);
	إذا (signal_pending_state(منوال, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_علامت_GPL(ثنائی_ہندسہ_wait_timeout);

فراغ __init wait_ثنائی_ہندسہ_init(فراغ)
{
	صحيح i;

	لكل (i = 0; i < WAIT_جدول_ناپ; i++)
		init_waitqueue_سر(ثنائی_ہندسہ_wait_جدول + i);
}
