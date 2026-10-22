// SPDX-License-Identifier: GPL-2.0-only

#شامل <linux/sched/debug.h>
#شامل "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#تعریف_کرو WAIT_جدول_بٹ 8
#تعریف_کرو WAIT_جدول_سائز (1 << WAIT_جدول_بٹ)

ساکن wait_قطار_سر_t ثنائی_ہندسہ_wait_جدول[WAIT_جدول_سائز] __cacheline_aligned;

wait_قطار_سر_t *ثنائی_ہندسہ_waitqueue(غیرعلامتی طویل *لفظ, صحیح ثنائی_ہندسہ_2)
{
	ثابت صحیح سرکاؤ = بٹ_فی_LONG == 32 ? 5 : 6;
	غیرعلامتی طویل val = (غیرعلامتی طویل)لفظ << سرکاؤ | ثنائی_ہندسہ_2;

	واپس ثنائی_ہندسہ_wait_جدول + hash_long(val, WAIT_جدول_بٹ);
}
EXPORT_علامت(ثنائی_ہندسہ_waitqueue);

صحیح wake_ثنائی_ہندسہ_function(ساخت wait_قطار_اندراج *wq_اندراج, غیرعلامتی منوال, صحیح sync, خالی *arg)
{
	ساخت wait_ثنائی_ہندسہ_کلید *کلید = arg;
	ساخت wait_ثنائی_ہندسہ_قطار_اندراج *wait_ثنائی_ہندسہ = container_کا(wq_اندراج, ساخت wait_ثنائی_ہندسہ_قطار_اندراج, wq_اندراج);

	اگر (wait_ثنائی_ہندسہ->کلید.flags != کلید->flags ||
			wait_ثنائی_ہندسہ->کلید.ثنائی_ہندسہ_nr != کلید->ثنائی_ہندسہ_nr ||
			test_ثنائی_ہندسہ(کلید->ثنائی_ہندسہ_nr, کلید->flags))
		واپس 0;

	واپس autoremove_wake_function(wq_اندراج, منوال, sync, کلید);
}
EXPORT_علامت(wake_ثنائی_ہندسہ_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
صحیح __sched
__wait_on_bit(ساخت wait_قطار_سر *wq_سر, ساخت wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج,
	      wait_ثنائی_ہندسہ_action_f *action, غیرعلامتی منوال)
{
	صحیح ret = 0;

	کرو {
		prepare_مقصد_wait(wq_سر, &wbq_اندراج->wq_اندراج, منوال);
		اگر (test_ثنائی_ہندسہ(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags))
			ret = (*action)(&wbq_اندراج->کلید, منوال);
	} جب (test_ثنائی_ہندسہ_acquire(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags) && !ret);

	اختتام_wait(wq_سر, &wbq_اندراج->wq_اندراج);

	واپس ret;
}
EXPORT_علامت(__wait_on_bit);

صحیح __sched برآمد_کا_سطر_wait_on_ثنائی_ہندسہ(غیرعلامتی طویل *لفظ, صحیح ثنائی_ہندسہ_2,
				    wait_ثنائی_ہندسہ_action_f *action, غیرعلامتی منوال)
{
	ساخت wait_قطار_سر *wq_سر = ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2);
	DEFINE_WAIT_ثنائی_ہندسہ(wq_اندراج, لفظ, ثنائی_ہندسہ_2);

	واپس __wait_on_bit(wq_سر, &wq_اندراج, action, منوال);
}
EXPORT_علامت(برآمد_کا_سطر_wait_on_ثنائی_ہندسہ);

صحیح __sched برآمد_کا_سطر_wait_on_ثنائی_ہندسہ_timeout(
	غیرعلامتی طویل *لفظ, صحیح ثنائی_ہندسہ_2, wait_ثنائی_ہندسہ_action_f *action,
	غیرعلامتی منوال, غیرعلامتی طویل timeout)
{
	ساخت wait_قطار_سر *wq_سر = ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2);
	DEFINE_WAIT_ثنائی_ہندسہ(wq_اندراج, لفظ, ثنائی_ہندسہ_2);

	wq_اندراج.کلید.timeout = jiffies + timeout;

	واپس __wait_on_bit(wq_سر, &wq_اندراج, action, منوال);
}
EXPORT_علامت_GPL(برآمد_کا_سطر_wait_on_ثنائی_ہندسہ_timeout);

صحیح __sched
__wait_on_bit_lock(ساخت wait_قطار_سر *wq_سر, ساخت wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج,
			wait_ثنائی_ہندسہ_action_f *action, غیرعلامتی منوال)
{
	صحیح ret = 0;

	برائے (;;) {
		prepare_مقصد_wait_exclusive(wq_سر, &wbq_اندراج->wq_اندراج, منوال);
		اگر (test_ثنائی_ہندسہ(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags)) {
			ret = action(&wbq_اندراج->کلید, منوال);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			اگر (ret)
				اختتام_wait(wq_سر, &wbq_اندراج->wq_اندراج);
		}
		اگر (!test_and_سیٹ_ثنائی_ہندسہ(wbq_اندراج->کلید.ثنائی_ہندسہ_nr, wbq_اندراج->کلید.flags)) {
			اگر (!ret)
				اختتام_wait(wq_سر, &wbq_اندراج->wq_اندراج);
			واپس 0;
		} ورنہ اگر (ret) {
			واپس ret;
		}
	}
}
EXPORT_علامت(__wait_on_bit_lock);

صحیح __sched برآمد_کا_سطر_wait_on_ثنائی_ہندسہ_تالا(غیرعلامتی طویل *لفظ, صحیح ثنائی_ہندسہ_2,
					 wait_ثنائی_ہندسہ_action_f *action, غیرعلامتی منوال)
{
	ساخت wait_قطار_سر *wq_سر = ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2);
	DEFINE_WAIT_ثنائی_ہندسہ(wq_اندراج, لفظ, ثنائی_ہندسہ_2);

	واپس __wait_on_bit_lock(wq_سر, &wq_اندراج, action, منوال);
}
EXPORT_علامت(برآمد_کا_سطر_wait_on_ثنائی_ہندسہ_تالا);

خالی __wake_up_bit(ساخت wait_قطار_سر *wq_سر, غیرعلامتی طویل *لفظ, صحیح ثنائی_ہندسہ_2)
{
	ساخت wait_ثنائی_ہندسہ_کلید کلید = __WAIT_BIT_KEY_INITIALIZER(لفظ, ثنائی_ہندسہ_2);

	اگر (waitqueue_فعال(wq_سر))
		__wake_up(wq_سر, TASK_معمول, 1, &کلید);
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
خالی wake_اوپر_ثنائی_ہندسہ(غیرعلامتی طویل *لفظ, صحیح ثنائی_ہندسہ_2)
{
	__wake_up_bit(ثنائی_ہندسہ_waitqueue(لفظ, ثنائی_ہندسہ_2), لفظ, ثنائی_ہندسہ_2);
}
EXPORT_علامت(wake_اوپر_ثنائی_ہندسہ);

wait_قطار_سر_t *__var_waitqueue(خالی *p)
{
	واپس ثنائی_ہندسہ_wait_جدول + hash_ptr(p, WAIT_جدول_بٹ);
}
EXPORT_علامت(__var_waitqueue);

ساخت wait_ثنائی_ہندسہ_کلید *__var_wake_key(ساخت wait_قطار_اندراج *wq_اندراج, خالی *arg)
{
	ساخت wait_ثنائی_ہندسہ_کلید *کلید = arg;
	ساخت wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج =
		container_کا(wq_اندراج, ساخت wait_ثنائی_ہندسہ_قطار_اندراج, wq_اندراج);

	اگر (wbq_اندراج->کلید.flags != کلید->flags ||
	    wbq_اندراج->کلید.ثنائی_ہندسہ_nr != کلید->ثنائی_ہندسہ_nr)
		واپس تہی;

	واپس کلید;
}

ساکن صحیح var_wake_function(ساخت wait_قطار_اندراج *wq_اندراج, غیرعلامتی صحیح منوال,
			     صحیح sync, خالی *arg)
{
	ساخت wait_ثنائی_ہندسہ_کلید *کلید = __var_wake_key(wq_اندراج, arg);
	اگر (!کلید)
		واپس 0;

	واپس autoremove_wake_function(wq_اندراج, منوال, sync, کلید);
}

خالی init_wait_var_اندراج(ساخت wait_ثنائی_ہندسہ_قطار_اندراج *wbq_اندراج, خالی *var, صحیح flags)
{
	*wbq_اندراج = (ساخت wait_ثنائی_ہندسہ_قطار_اندراج){
		.کلید = {
			.flags	= (var),
			.ثنائی_ہندسہ_nr = -1,
		},
		.wq_اندراج = {
			.flags	 = flags,
			.نجی = current,
			.func	 = var_wake_function,
			.اندراج_2	 = LIST_سر_INIT(wbq_اندراج->wq_اندراج.اندراج_2),
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
خالی wake_اوپر_var(خالی *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_علامت(wake_اوپر_var);

__sched صحیح ثنائی_ہندسہ_wait(ساخت wait_ثنائی_ہندسہ_کلید *لفظ, صحیح منوال)
{
	جدول_وقت();
	اگر (signal_pending_state(منوال, current))
		واپس -EINTR;

	واپس 0;
}
EXPORT_علامت(ثنائی_ہندسہ_wait);

__sched صحیح ثنائی_ہندسہ_wait_io(ساخت wait_ثنائی_ہندسہ_کلید *لفظ, صحیح منوال)
{
	io_جدول_وقت();
	اگر (signal_pending_state(منوال, current))
		واپس -EINTR;

	واپس 0;
}
EXPORT_علامت(ثنائی_ہندسہ_wait_io);

__sched صحیح ثنائی_ہندسہ_wait_timeout(ساخت wait_ثنائی_ہندسہ_کلید *لفظ, صحیح منوال)
{
	غیرعلامتی طویل now = READ_ONCE(jiffies);

	اگر (وقت_after_eq(now, لفظ->timeout))
		واپس -EAGAIN;
	جدول_وقت_timeout(لفظ->timeout - now);
	اگر (signal_pending_state(منوال, current))
		واپس -EINTR;

	واپس 0;
}
EXPORT_علامت_GPL(ثنائی_ہندسہ_wait_timeout);

خالی __init wait_ثنائی_ہندسہ_init(خالی)
{
	صحیح i;

	برائے (i = 0; i < WAIT_جدول_سائز; i++)
		init_waitqueue_سر(ثنائی_ہندسہ_wait_جدول + i);
}
