// SPDX-License-Identifier: GPL-2.0-only

#ضمن <linux/sched/debug.h>
#ضمن "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#تعریف_کن WAIT_جدول_بیتها 8
#تعریف_کن WAIT_جدول_اندازه (1 << WAIT_جدول_بیتها)

ساكن wait_صف_سر_t بیت_wait_جدول[WAIT_جدول_اندازه] __cacheline_aligned;

wait_صف_سر_t *بیت_waitqueue(بیعلامت دراز *کلمه, صحيح بیت_2)
{
	ثابت صحيح لغزش = بیتها_در_هر_LONG == 32 ? 5 : 6;
	بیعلامت دراز val = (بیعلامت دراز)کلمه << لغزش | بیت_2;

	أرجع بیت_wait_جدول + hash_long(val, WAIT_جدول_بیتها);
}
EXPORT_نماد(بیت_waitqueue);

صحيح wake_بیت_function(بنية wait_صف_مدخل *wq_مدخل, بیعلامت نما, صحيح sync, فراغ *arg)
{
	بنية wait_بیت_کلید *کلید = arg;
	بنية wait_بیت_صف_مدخل *wait_بیت = container_از(wq_مدخل, بنية wait_بیت_صف_مدخل, wq_مدخل);

	إذا (wait_بیت->کلید.flags != کلید->flags ||
			wait_بیت->کلید.بیت_nr != کلید->بیت_nr ||
			test_بیت(کلید->بیت_nr, کلید->flags))
		أرجع 0;

	أرجع autoremove_wake_function(wq_مدخل, نما, sync, کلید);
}
EXPORT_نماد(wake_بیت_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
صحيح __sched
__wait_on_bit(بنية wait_صف_سر *wq_سر, بنية wait_بیت_صف_مدخل *wbq_مدخل,
	      wait_بیت_action_f *action, بیعلامت نما)
{
	صحيح ret = 0;

	افعل {
		prepare_مقصد_wait(wq_سر, &wbq_مدخل->wq_مدخل, نما);
		إذا (test_بیت(wbq_مدخل->کلید.بیت_nr, wbq_مدخل->کلید.flags))
			ret = (*action)(&wbq_مدخل->کلید, نما);
	} طالما (test_بیت_acquire(wbq_مدخل->کلید.بیت_nr, wbq_مدخل->کلید.flags) && !ret);

	پایان_wait(wq_سر, &wbq_مدخل->wq_مدخل);

	أرجع ret;
}
EXPORT_نماد(__wait_on_bit);

صحيح __sched بیرون_از_سطر_wait_on_بیت(بیعلامت دراز *کلمه, صحيح بیت_2,
				    wait_بیت_action_f *action, بیعلامت نما)
{
	بنية wait_صف_سر *wq_سر = بیت_waitqueue(کلمه, بیت_2);
	DEFINE_WAIT_بیت(wq_مدخل, کلمه, بیت_2);

	أرجع __wait_on_bit(wq_سر, &wq_مدخل, action, نما);
}
EXPORT_نماد(بیرون_از_سطر_wait_on_بیت);

صحيح __sched بیرون_از_سطر_wait_on_بیت_timeout(
	بیعلامت دراز *کلمه, صحيح بیت_2, wait_بیت_action_f *action,
	بیعلامت نما, بیعلامت دراز timeout)
{
	بنية wait_صف_سر *wq_سر = بیت_waitqueue(کلمه, بیت_2);
	DEFINE_WAIT_بیت(wq_مدخل, کلمه, بیت_2);

	wq_مدخل.کلید.timeout = jiffies + timeout;

	أرجع __wait_on_bit(wq_سر, &wq_مدخل, action, نما);
}
EXPORT_نماد_GPL(بیرون_از_سطر_wait_on_بیت_timeout);

صحيح __sched
__wait_on_bit_lock(بنية wait_صف_سر *wq_سر, بنية wait_بیت_صف_مدخل *wbq_مدخل,
			wait_بیت_action_f *action, بیعلامت نما)
{
	صحيح ret = 0;

	لكل (;;) {
		prepare_مقصد_wait_exclusive(wq_سر, &wbq_مدخل->wq_مدخل, نما);
		إذا (test_بیت(wbq_مدخل->کلید.بیت_nr, wbq_مدخل->کلید.flags)) {
			ret = action(&wbq_مدخل->کلید, نما);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			إذا (ret)
				پایان_wait(wq_سر, &wbq_مدخل->wq_مدخل);
		}
		إذا (!test_and_مجموعه_بیت(wbq_مدخل->کلید.بیت_nr, wbq_مدخل->کلید.flags)) {
			إذا (!ret)
				پایان_wait(wq_سر, &wbq_مدخل->wq_مدخل);
			أرجع 0;
		} وإلا إذا (ret) {
			أرجع ret;
		}
	}
}
EXPORT_نماد(__wait_on_bit_lock);

صحيح __sched بیرون_از_سطر_wait_on_بیت_قفل(بیعلامت دراز *کلمه, صحيح بیت_2,
					 wait_بیت_action_f *action, بیعلامت نما)
{
	بنية wait_صف_سر *wq_سر = بیت_waitqueue(کلمه, بیت_2);
	DEFINE_WAIT_بیت(wq_مدخل, کلمه, بیت_2);

	أرجع __wait_on_bit_lock(wq_سر, &wq_مدخل, action, نما);
}
EXPORT_نماد(بیرون_از_سطر_wait_on_بیت_قفل);

فراغ __wake_up_bit(بنية wait_صف_سر *wq_سر, بیعلامت دراز *کلمه, صحيح بیت_2)
{
	بنية wait_بیت_کلید کلید = __WAIT_BIT_KEY_INITIALIZER(کلمه, بیت_2);

	إذا (waitqueue_فعال(wq_سر))
		__wake_up(wq_سر, TASK_نرمال, 1, &کلید);
}
EXPORT_نماد(__wake_up_bit);

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
فراغ wake_به_بالا_بیت(بیعلامت دراز *کلمه, صحيح بیت_2)
{
	__wake_up_bit(بیت_waitqueue(کلمه, بیت_2), کلمه, بیت_2);
}
EXPORT_نماد(wake_به_بالا_بیت);

wait_صف_سر_t *__var_waitqueue(فراغ *p)
{
	أرجع بیت_wait_جدول + hash_ptr(p, WAIT_جدول_بیتها);
}
EXPORT_نماد(__var_waitqueue);

بنية wait_بیت_کلید *__var_wake_key(بنية wait_صف_مدخل *wq_مدخل, فراغ *arg)
{
	بنية wait_بیت_کلید *کلید = arg;
	بنية wait_بیت_صف_مدخل *wbq_مدخل =
		container_از(wq_مدخل, بنية wait_بیت_صف_مدخل, wq_مدخل);

	إذا (wbq_مدخل->کلید.flags != کلید->flags ||
	    wbq_مدخل->کلید.بیت_nr != کلید->بیت_nr)
		أرجع عدم;

	أرجع کلید;
}

ساكن صحيح var_wake_function(بنية wait_صف_مدخل *wq_مدخل, بیعلامت صحيح نما,
			     صحيح sync, فراغ *arg)
{
	بنية wait_بیت_کلید *کلید = __var_wake_key(wq_مدخل, arg);
	إذا (!کلید)
		أرجع 0;

	أرجع autoremove_wake_function(wq_مدخل, نما, sync, کلید);
}

فراغ init_wait_var_مدخل(بنية wait_بیت_صف_مدخل *wbq_مدخل, فراغ *var, صحيح flags)
{
	*wbq_مدخل = (بنية wait_بیت_صف_مدخل){
		.کلید = {
			.flags	= (var),
			.بیت_nr = -1,
		},
		.wq_مدخل = {
			.flags	 = flags,
			.خاص = current,
			.func	 = var_wake_function,
			.مدخل	 = LIST_سر_INIT(wbq_مدخل->wq_مدخل.مدخل),
		},
	};
}
EXPORT_نماد(init_wait_var_مدخل);

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
فراغ wake_به_بالا_var(فراغ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_نماد(wake_به_بالا_var);

__sched صحيح بیت_wait(بنية wait_بیت_کلید *کلمه, صحيح نما)
{
	زمانبندی();
	إذا (signal_pending_state(نما, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نماد(بیت_wait);

__sched صحيح بیت_wait_io(بنية wait_بیت_کلید *کلمه, صحيح نما)
{
	io_زمانبندی();
	إذا (signal_pending_state(نما, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نماد(بیت_wait_io);

__sched صحيح بیت_wait_timeout(بنية wait_بیت_کلید *کلمه, صحيح نما)
{
	بیعلامت دراز now = READ_ONCE(jiffies);

	إذا (زمان_after_eq(now, کلمه->timeout))
		أرجع -EAGAIN;
	زمانبندی_timeout(کلمه->timeout - now);
	إذا (signal_pending_state(نما, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نماد_GPL(بیت_wait_timeout);

فراغ __init wait_بیت_init(فراغ)
{
	صحيح i;

	لكل (i = 0; i < WAIT_جدول_اندازه; i++)
		init_waitqueue_سر(بیت_wait_جدول + i);
}
