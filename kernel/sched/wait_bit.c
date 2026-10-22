// SPDX-License-Identifier: GPL-2.0-only

#ضمن <linux/sched/debug.h>
#ضمن "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#تعریف_کړه WAIT_جدول_بټونه 8
#تعریف_کړه WAIT_جدول_اندازه (1 << WAIT_جدول_بټونه)

ساكن wait_کتار_سر_t بټ_wait_جدول[WAIT_جدول_اندازه] __cacheline_aligned;

wait_کتار_سر_t *بټ_waitqueue(بینښه اوږد *کلمه, صحيح بټ_2)
{
	ثابت صحيح ښويدنه = بټونه_فی_LONG == 32 ? 5 : 6;
	بینښه اوږد val = (بینښه اوږد)کلمه << ښويدنه | بټ_2;

	أرجع بټ_wait_جدول + hash_long(val, WAIT_جدول_بټونه);
}
EXPORT_نښه(بټ_waitqueue);

صحيح wake_بټ_function(بنية wait_کتار_ثبت *wq_ثبت, بینښه ډېرپېښ, صحيح sync, فراغ *arg)
{
	بنية wait_بټ_کلی *کلی = arg;
	بنية wait_بټ_کتار_ثبت *wait_بټ = container_د(wq_ثبت, بنية wait_بټ_کتار_ثبت, wq_ثبت);

	إذا (wait_بټ->کلی.flags != کلی->flags ||
			wait_بټ->کلی.بټ_nr != کلی->بټ_nr ||
			test_بټ(کلی->بټ_nr, کلی->flags))
		أرجع 0;

	أرجع autoremove_wake_function(wq_ثبت, ډېرپېښ, sync, کلی);
}
EXPORT_نښه(wake_بټ_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
صحيح __sched
__wait_on_bit(بنية wait_کتار_سر *wq_سر, بنية wait_بټ_کتار_ثبت *wbq_ثبت,
	      wait_بټ_action_f *action, بینښه ډېرپېښ)
{
	صحيح ret = 0;

	افعل {
		prepare_منزل_wait(wq_سر, &wbq_ثبت->wq_ثبت, ډېرپېښ);
		إذا (test_بټ(wbq_ثبت->کلی.بټ_nr, wbq_ثبت->کلی.flags))
			ret = (*action)(&wbq_ثبت->کلی, ډېرپېښ);
	} طالما (test_بټ_acquire(wbq_ثبت->کلی.بټ_nr, wbq_ثبت->کلی.flags) && !ret);

	پای_wait(wq_سر, &wbq_ثبت->wq_ثبت);

	أرجع ret;
}
EXPORT_نښه(__wait_on_bit);

صحيح __sched بهر_د_کرښه_wait_on_بټ(بینښه اوږد *کلمه, صحيح بټ_2,
				    wait_بټ_action_f *action, بینښه ډېرپېښ)
{
	بنية wait_کتار_سر *wq_سر = بټ_waitqueue(کلمه, بټ_2);
	DEFINE_WAIT_بټ(wq_ثبت, کلمه, بټ_2);

	أرجع __wait_on_bit(wq_سر, &wq_ثبت, action, ډېرپېښ);
}
EXPORT_نښه(بهر_د_کرښه_wait_on_بټ);

صحيح __sched بهر_د_کرښه_wait_on_بټ_timeout(
	بینښه اوږد *کلمه, صحيح بټ_2, wait_بټ_action_f *action,
	بینښه ډېرپېښ, بینښه اوږد timeout)
{
	بنية wait_کتار_سر *wq_سر = بټ_waitqueue(کلمه, بټ_2);
	DEFINE_WAIT_بټ(wq_ثبت, کلمه, بټ_2);

	wq_ثبت.کلی.timeout = jiffies + timeout;

	أرجع __wait_on_bit(wq_سر, &wq_ثبت, action, ډېرپېښ);
}
EXPORT_نښه_GPL(بهر_د_کرښه_wait_on_بټ_timeout);

صحيح __sched
__wait_on_bit_lock(بنية wait_کتار_سر *wq_سر, بنية wait_بټ_کتار_ثبت *wbq_ثبت,
			wait_بټ_action_f *action, بینښه ډېرپېښ)
{
	صحيح ret = 0;

	لكل (;;) {
		prepare_منزل_wait_exclusive(wq_سر, &wbq_ثبت->wq_ثبت, ډېرپېښ);
		إذا (test_بټ(wbq_ثبت->کلی.بټ_nr, wbq_ثبت->کلی.flags)) {
			ret = action(&wbq_ثبت->کلی, ډېرپېښ);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			إذا (ret)
				پای_wait(wq_سر, &wbq_ثبت->wq_ثبت);
		}
		إذا (!test_and_سېټ_بټ(wbq_ثبت->کلی.بټ_nr, wbq_ثبت->کلی.flags)) {
			إذا (!ret)
				پای_wait(wq_سر, &wbq_ثبت->wq_ثبت);
			أرجع 0;
		} وإلا إذا (ret) {
			أرجع ret;
		}
	}
}
EXPORT_نښه(__wait_on_bit_lock);

صحيح __sched بهر_د_کرښه_wait_on_بټ_قلف(بینښه اوږد *کلمه, صحيح بټ_2,
					 wait_بټ_action_f *action, بینښه ډېرپېښ)
{
	بنية wait_کتار_سر *wq_سر = بټ_waitqueue(کلمه, بټ_2);
	DEFINE_WAIT_بټ(wq_ثبت, کلمه, بټ_2);

	أرجع __wait_on_bit_lock(wq_سر, &wq_ثبت, action, ډېرپېښ);
}
EXPORT_نښه(بهر_د_کرښه_wait_on_بټ_قلف);

فراغ __wake_up_bit(بنية wait_کتار_سر *wq_سر, بینښه اوږد *کلمه, صحيح بټ_2)
{
	بنية wait_بټ_کلی کلی = __WAIT_BIT_KEY_INITIALIZER(کلمه, بټ_2);

	إذا (waitqueue_فعال(wq_سر))
		__wake_up(wq_سر, TASK_نورمال, 1, &کلی);
}
EXPORT_نښه(__wake_up_bit);

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
فراغ wake_پورته_بټ(بینښه اوږد *کلمه, صحيح بټ_2)
{
	__wake_up_bit(بټ_waitqueue(کلمه, بټ_2), کلمه, بټ_2);
}
EXPORT_نښه(wake_پورته_بټ);

wait_کتار_سر_t *__var_waitqueue(فراغ *p)
{
	أرجع بټ_wait_جدول + hash_ptr(p, WAIT_جدول_بټونه);
}
EXPORT_نښه(__var_waitqueue);

بنية wait_بټ_کلی *__var_wake_key(بنية wait_کتار_ثبت *wq_ثبت, فراغ *arg)
{
	بنية wait_بټ_کلی *کلی = arg;
	بنية wait_بټ_کتار_ثبت *wbq_ثبت =
		container_د(wq_ثبت, بنية wait_بټ_کتار_ثبت, wq_ثبت);

	إذا (wbq_ثبت->کلی.flags != کلی->flags ||
	    wbq_ثبت->کلی.بټ_nr != کلی->بټ_nr)
		أرجع عدم;

	أرجع کلی;
}

ساكن صحيح var_wake_function(بنية wait_کتار_ثبت *wq_ثبت, بینښه صحيح ډېرپېښ,
			     صحيح sync, فراغ *arg)
{
	بنية wait_بټ_کلی *کلی = __var_wake_key(wq_ثبت, arg);
	إذا (!کلی)
		أرجع 0;

	أرجع autoremove_wake_function(wq_ثبت, ډېرپېښ, sync, کلی);
}

فراغ init_wait_var_ثبت(بنية wait_بټ_کتار_ثبت *wbq_ثبت, فراغ *var, صحيح flags)
{
	*wbq_ثبت = (بنية wait_بټ_کتار_ثبت){
		.کلی = {
			.flags	= (var),
			.بټ_nr = -1,
		},
		.wq_ثبت = {
			.flags	 = flags,
			.خاص = current,
			.func	 = var_wake_function,
			.ثبت	 = LIST_سر_INIT(wbq_ثبت->wq_ثبت.ثبت),
		},
	};
}
EXPORT_نښه(init_wait_var_ثبت);

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
فراغ wake_پورته_var(فراغ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_نښه(wake_پورته_var);

__sched صحيح بټ_wait(بنية wait_بټ_کلی *کلمه, صحيح ډېرپېښ)
{
	مهالوېش();
	إذا (signal_pending_state(ډېرپېښ, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نښه(بټ_wait);

__sched صحيح بټ_wait_io(بنية wait_بټ_کلی *کلمه, صحيح ډېرپېښ)
{
	io_مهالوېش();
	إذا (signal_pending_state(ډېرپېښ, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نښه(بټ_wait_io);

__sched صحيح بټ_wait_timeout(بنية wait_بټ_کلی *کلمه, صحيح ډېرپېښ)
{
	بینښه اوږد now = READ_ONCE(jiffies);

	إذا (وخت_after_eq(now, کلمه->timeout))
		أرجع -EAGAIN;
	مهالوېش_timeout(کلمه->timeout - now);
	إذا (signal_pending_state(ډېرپېښ, current))
		أرجع -EINTR;

	أرجع 0;
}
EXPORT_نښه_GPL(بټ_wait_timeout);

فراغ __init wait_بټ_init(فراغ)
{
	صحيح i;

	لكل (i = 0; i < WAIT_جدول_اندازه; i++)
		init_waitqueue_سر(بټ_wait_جدول + i);
}
