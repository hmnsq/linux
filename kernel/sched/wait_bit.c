// SPDX-License-Identifier: GPL-2.0-only

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/debug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WAIT_ਸਾਰਣੀ_ਬਿੱਟ 8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WAIT_ਸਾਰਣੀ_ਮਾਪ (1 << WAIT_ਸਾਰਣੀ_ਬਿੱਟ)

ਸਥਿਰ wait_ਕਤਾਰ_ਸਿਰ_t ਦੋਆਧਾਰੀ_ਅੰਕ_wait_ਸਾਰਣੀ[WAIT_ਸਾਰਣੀ_ਮਾਪ] __cacheline_aligned;

wait_ਕਤਾਰ_ਸਿਰ_t *ਦੋਆਧਾਰੀ_ਅੰਕ_waitqueue(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਸ਼ਬਦ, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_2)
{
	ਅਚਲ ਪੂਰਨ ਖਿਸਕਾਓ = ਬਿੱਟ_ਪ੍ਰਤੀ_LONG == 32 ? 5 : 6;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ val = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)ਸ਼ਬਦ << ਖਿਸਕਾਓ | ਦੋਆਧਾਰੀ_ਅੰਕ_2;

	ਵਾਪਸ ਦੋਆਧਾਰੀ_ਅੰਕ_wait_ਸਾਰਣੀ + hash_long(val, WAIT_ਸਾਰਣੀ_ਬਿੱਟ);
}
EXPORT_ਚਿੰਨ੍ਹ(ਦੋਆਧਾਰੀ_ਅੰਕ_waitqueue);

ਪੂਰਨ wake_ਦੋਆਧਾਰੀ_ਅੰਕ_function(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਪੂਰਨ sync, ਖਾਲੀ *arg)
{
	ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *ਕੁੰਜੀ = arg;
	ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ *wait_ਦੋਆਧਾਰੀ_ਅੰਕ = container_ਦਾ(wq_ਇੰਦਰਾਜ, ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ, wq_ਇੰਦਰਾਜ);

	ਜੇ (wait_ਦੋਆਧਾਰੀ_ਅੰਕ->ਕੁੰਜੀ.flags != ਕੁੰਜੀ->flags ||
			wait_ਦੋਆਧਾਰੀ_ਅੰਕ->ਕੁੰਜੀ.ਦੋਆਧਾਰੀ_ਅੰਕ_nr != ਕੁੰਜੀ->ਦੋਆਧਾਰੀ_ਅੰਕ_nr ||
			test_ਦੋਆਧਾਰੀ_ਅੰਕ(ਕੁੰਜੀ->ਦੋਆਧਾਰੀ_ਅੰਕ_nr, ਕੁੰਜੀ->flags))
		ਵਾਪਸ 0;

	ਵਾਪਸ autoremove_wake_function(wq_ਇੰਦਰਾਜ, ਬਹੁਲਕ, sync, ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ(wake_ਦੋਆਧਾਰੀ_ਅੰਕ_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
ਪੂਰਨ __sched
__wait_on_bit(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ *wbq_ਇੰਦਰਾਜ,
	      wait_ਦੋਆਧਾਰੀ_ਅੰਕ_action_f *action, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ)
{
	ਪੂਰਨ ret = 0;

	ਕਰੋ {
		prepare_ਮੰਜ਼ਿਲ_wait(wq_ਸਿਰ, &wbq_ਇੰਦਰਾਜ->wq_ਇੰਦਰਾਜ, ਬਹੁਲਕ);
		ਜੇ (test_ਦੋਆਧਾਰੀ_ਅੰਕ(wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਦੋਆਧਾਰੀ_ਅੰਕ_nr, wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.flags))
			ret = (*action)(&wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ, ਬਹੁਲਕ);
	} ਜਦੋਂ (test_ਦੋਆਧਾਰੀ_ਅੰਕ_acquire(wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਦੋਆਧਾਰੀ_ਅੰਕ_nr, wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.flags) && !ret);

	ਅੰਤ_wait(wq_ਸਿਰ, &wbq_ਇੰਦਰਾਜ->wq_ਇੰਦਰਾਜ);

	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(__wait_on_bit);

ਪੂਰਨ __sched ਬਾਹਰਲਾ_ਦਾ_ਸਤਰ_wait_on_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਸ਼ਬਦ, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_2,
				    wait_ਦੋਆਧਾਰੀ_ਅੰਕ_action_f *action, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ)
{
	ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ = ਦੋਆਧਾਰੀ_ਅੰਕ_waitqueue(ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);
	DEFINE_WAIT_ਦੋਆਧਾਰੀ_ਅੰਕ(wq_ਇੰਦਰਾਜ, ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);

	ਵਾਪਸ __wait_on_bit(wq_ਸਿਰ, &wq_ਇੰਦਰਾਜ, action, ਬਹੁਲਕ);
}
EXPORT_ਚਿੰਨ੍ਹ(ਬਾਹਰਲਾ_ਦਾ_ਸਤਰ_wait_on_ਦੋਆਧਾਰੀ_ਅੰਕ);

ਪੂਰਨ __sched ਬਾਹਰਲਾ_ਦਾ_ਸਤਰ_wait_on_ਦੋਆਧਾਰੀ_ਅੰਕ_timeout(
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਸ਼ਬਦ, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_2, wait_ਦੋਆਧਾਰੀ_ਅੰਕ_action_f *action,
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ timeout)
{
	ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ = ਦੋਆਧਾਰੀ_ਅੰਕ_waitqueue(ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);
	DEFINE_WAIT_ਦੋਆਧਾਰੀ_ਅੰਕ(wq_ਇੰਦਰਾਜ, ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);

	wq_ਇੰਦਰਾਜ.ਕੁੰਜੀ.timeout = jiffies + timeout;

	ਵਾਪਸ __wait_on_bit(wq_ਸਿਰ, &wq_ਇੰਦਰਾਜ, action, ਬਹੁਲਕ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(ਬਾਹਰਲਾ_ਦਾ_ਸਤਰ_wait_on_ਦੋਆਧਾਰੀ_ਅੰਕ_timeout);

ਪੂਰਨ __sched
__wait_on_bit_lock(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ *wbq_ਇੰਦਰਾਜ,
			wait_ਦੋਆਧਾਰੀ_ਅੰਕ_action_f *action, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ)
{
	ਪੂਰਨ ret = 0;

	ਲਈ (;;) {
		prepare_ਮੰਜ਼ਿਲ_wait_exclusive(wq_ਸਿਰ, &wbq_ਇੰਦਰਾਜ->wq_ਇੰਦਰਾਜ, ਬਹੁਲਕ);
		ਜੇ (test_ਦੋਆਧਾਰੀ_ਅੰਕ(wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਦੋਆਧਾਰੀ_ਅੰਕ_nr, wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.flags)) {
			ret = action(&wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ, ਬਹੁਲਕ);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			ਜੇ (ret)
				ਅੰਤ_wait(wq_ਸਿਰ, &wbq_ਇੰਦਰਾਜ->wq_ਇੰਦਰਾਜ);
		}
		ਜੇ (!test_and_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਦੋਆਧਾਰੀ_ਅੰਕ_nr, wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.flags)) {
			ਜੇ (!ret)
				ਅੰਤ_wait(wq_ਸਿਰ, &wbq_ਇੰਦਰਾਜ->wq_ਇੰਦਰਾਜ);
			ਵਾਪਸ 0;
		} ਨਹੀਂਤਾਂ ਜੇ (ret) {
			ਵਾਪਸ ret;
		}
	}
}
EXPORT_ਚਿੰਨ੍ਹ(__wait_on_bit_lock);

ਪੂਰਨ __sched ਬਾਹਰਲਾ_ਦਾ_ਸਤਰ_wait_on_ਦੋਆਧਾਰੀ_ਅੰਕ_ਤਾਲਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਸ਼ਬਦ, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_2,
					 wait_ਦੋਆਧਾਰੀ_ਅੰਕ_action_f *action, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ)
{
	ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ = ਦੋਆਧਾਰੀ_ਅੰਕ_waitqueue(ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);
	DEFINE_WAIT_ਦੋਆਧਾਰੀ_ਅੰਕ(wq_ਇੰਦਰਾਜ, ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);

	ਵਾਪਸ __wait_on_bit_lock(wq_ਸਿਰ, &wq_ਇੰਦਰਾਜ, action, ਬਹੁਲਕ);
}
EXPORT_ਚਿੰਨ੍ਹ(ਬਾਹਰਲਾ_ਦਾ_ਸਤਰ_wait_on_ਦੋਆਧਾਰੀ_ਅੰਕ_ਤਾਲਾ);

ਖਾਲੀ __wake_up_bit(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਸ਼ਬਦ, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_2)
{
	ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ ਕੁੰਜੀ = __WAIT_BIT_KEY_INITIALIZER(ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);

	ਜੇ (waitqueue_ਸਰਗਰਮ(wq_ਸਿਰ))
		__wake_up(wq_ਸਿਰ, TASK_ਸਾਧਾਰਨ, 1, &ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ(__wake_up_bit);

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
ਖਾਲੀ wake_ਉੱਪਰ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਸ਼ਬਦ, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_2)
{
	__wake_up_bit(ਦੋਆਧਾਰੀ_ਅੰਕ_waitqueue(ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2), ਸ਼ਬਦ, ਦੋਆਧਾਰੀ_ਅੰਕ_2);
}
EXPORT_ਚਿੰਨ੍ਹ(wake_ਉੱਪਰ_ਦੋਆਧਾਰੀ_ਅੰਕ);

wait_ਕਤਾਰ_ਸਿਰ_t *__var_waitqueue(ਖਾਲੀ *p)
{
	ਵਾਪਸ ਦੋਆਧਾਰੀ_ਅੰਕ_wait_ਸਾਰਣੀ + hash_ptr(p, WAIT_ਸਾਰਣੀ_ਬਿੱਟ);
}
EXPORT_ਚਿੰਨ੍ਹ(__var_waitqueue);

ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *__var_wake_key(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਖਾਲੀ *arg)
{
	ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *ਕੁੰਜੀ = arg;
	ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ *wbq_ਇੰਦਰਾਜ =
		container_ਦਾ(wq_ਇੰਦਰਾਜ, ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ, wq_ਇੰਦਰਾਜ);

	ਜੇ (wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.flags != ਕੁੰਜੀ->flags ||
	    wbq_ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਦੋਆਧਾਰੀ_ਅੰਕ_nr != ਕੁੰਜੀ->ਦੋਆਧਾਰੀ_ਅੰਕ_nr)
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ ਕੁੰਜੀ;
}

ਸਥਿਰ ਪੂਰਨ var_wake_function(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ,
			     ਪੂਰਨ sync, ਖਾਲੀ *arg)
{
	ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *ਕੁੰਜੀ = __var_wake_key(wq_ਇੰਦਰਾਜ, arg);
	ਜੇ (!ਕੁੰਜੀ)
		ਵਾਪਸ 0;

	ਵਾਪਸ autoremove_wake_function(wq_ਇੰਦਰਾਜ, ਬਹੁਲਕ, sync, ਕੁੰਜੀ);
}

ਖਾਲੀ init_wait_var_ਇੰਦਰਾਜ(ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ *wbq_ਇੰਦਰਾਜ, ਖਾਲੀ *var, ਪੂਰਨ flags)
{
	*wbq_ਇੰਦਰਾਜ = (ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕਤਾਰ_ਇੰਦਰਾਜ){
		.ਕੁੰਜੀ = {
			.flags	= (var),
			.ਦੋਆਧਾਰੀ_ਅੰਕ_nr = -1,
		},
		.wq_ਇੰਦਰਾਜ = {
			.flags	 = flags,
			.ਨਿਜੀ = current,
			.func	 = var_wake_function,
			.ਇੰਦਰਾਜ	 = LIST_ਸਿਰ_INIT(wbq_ਇੰਦਰਾਜ->wq_ਇੰਦਰਾਜ.ਇੰਦਰਾਜ),
		},
	};
}
EXPORT_ਚਿੰਨ੍ਹ(init_wait_var_ਇੰਦਰਾਜ);

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
ਖਾਲੀ wake_ਉੱਪਰ_var(ਖਾਲੀ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_ਚਿੰਨ੍ਹ(wake_ਉੱਪਰ_var);

__sched ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_wait(ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *ਸ਼ਬਦ, ਪੂਰਨ ਬਹੁਲਕ)
{
	ਸਮਾਂ_ਸਾਰਣੀ();
	ਜੇ (signal_pending_state(ਬਹੁਲਕ, current))
		ਵਾਪਸ -EINTR;

	ਵਾਪਸ 0;
}
EXPORT_ਚਿੰਨ੍ਹ(ਦੋਆਧਾਰੀ_ਅੰਕ_wait);

__sched ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_wait_io(ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *ਸ਼ਬਦ, ਪੂਰਨ ਬਹੁਲਕ)
{
	io_ਸਮਾਂ_ਸਾਰਣੀ();
	ਜੇ (signal_pending_state(ਬਹੁਲਕ, current))
		ਵਾਪਸ -EINTR;

	ਵਾਪਸ 0;
}
EXPORT_ਚਿੰਨ੍ਹ(ਦੋਆਧਾਰੀ_ਅੰਕ_wait_io);

__sched ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_wait_timeout(ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *ਸ਼ਬਦ, ਪੂਰਨ ਬਹੁਲਕ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ now = READ_ONCE(jiffies);

	ਜੇ (ਸਮਾਂ_after_eq(now, ਸ਼ਬਦ->timeout))
		ਵਾਪਸ -EAGAIN;
	ਸਮਾਂ_ਸਾਰਣੀ_timeout(ਸ਼ਬਦ->timeout - now);
	ਜੇ (signal_pending_state(ਬਹੁਲਕ, current))
		ਵਾਪਸ -EINTR;

	ਵਾਪਸ 0;
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(ਦੋਆਧਾਰੀ_ਅੰਕ_wait_timeout);

ਖਾਲੀ __init wait_ਦੋਆਧਾਰੀ_ਅੰਕ_init(ਖਾਲੀ)
{
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < WAIT_ਸਾਰਣੀ_ਮਾਪ; i++)
		init_waitqueue_ਸਿਰ(ਦੋਆਧਾਰੀ_ਅੰਕ_wait_ਸਾਰਣੀ + i);
}
