// SPDX-License-Identifier: GPL-2.0-only

#ಅನ್ತರ್ಭಾವ <linux/sched/debug.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#ವ್ಯಾಖ್ಯಾನಿಸು WAIT_ಕೋಷ್ಟಕ_ಬಿಟ್ಟುಗಳು 8
#ವ್ಯಾಖ್ಯಾನಿಸು WAIT_ಕೋಷ್ಟಕ_ಗಾತ್ರ (1 << WAIT_ಕೋಷ್ಟಕ_ಬಿಟ್ಟುಗಳು)

ಸ್ಥಿರ wait_ಸರತಿ_ತಲೆ_t ಬಿಟ್_wait_ಕೋಷ್ಟಕ[WAIT_ಕೋಷ್ಟಕ_ಗಾತ್ರ] __cacheline_aligned;

wait_ಸರತಿ_ತಲೆ_t *ಬಿಟ್_waitqueue(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಿಟ್_2)
{
	ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಸರಿಸು = ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_LONG == 32 ? 5 : 6;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ val = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)ಶಬ್ದ << ಸರಿಸು | ಬಿಟ್_2;

	ಹಿಂತಿರುಗಿ ಬಿಟ್_wait_ಕೋಷ್ಟಕ + hash_long(val, WAIT_ಕೋಷ್ಟಕ_ಬಿಟ್ಟುಗಳು);
}
EXPORT_ಚಿಹ್ನೆ(ಬಿಟ್_waitqueue);

ಪೂರ್ಣಾಂಕ wake_ಬಿಟ್_function(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ sync, ಶೂನ್ಯ *arg)
{
	ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *ಕೀಲಿ = arg;
	ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು *wait_ಬಿಟ್ = container_ದ(wq_ನಮೂದು, ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು, wq_ನಮೂದು);

	ಆದರೆ (wait_ಬಿಟ್->ಕೀಲಿ.flags != ಕೀಲಿ->flags ||
			wait_ಬಿಟ್->ಕೀಲಿ.ಬಿಟ್_nr != ಕೀಲಿ->ಬಿಟ್_nr ||
			test_ಬಿಟ್(ಕೀಲಿ->ಬಿಟ್_nr, ಕೀಲಿ->flags))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ autoremove_wake_function(wq_ನಮೂದು, ಬಹುಲಕ, sync, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ(wake_ಬಿಟ್_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
ಪೂರ್ಣಾಂಕ __sched
__wait_on_bit(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು *wbq_ನಮೂದು,
	      wait_ಬಿಟ್_action_f *action, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಮಾಡು {
		prepare_ಗಮ್ಯ_wait(wq_ತಲೆ, &wbq_ನಮೂದು->wq_ನಮೂದು, ಬಹುಲಕ);
		ಆದರೆ (test_ಬಿಟ್(wbq_ನಮೂದು->ಕೀಲಿ.ಬಿಟ್_nr, wbq_ನಮೂದು->ಕೀಲಿ.flags))
			ret = (*action)(&wbq_ನಮೂದು->ಕೀಲಿ, ಬಹುಲಕ);
	} ಆಗ (test_ಬಿಟ್_acquire(wbq_ನಮೂದು->ಕೀಲಿ.ಬಿಟ್_nr, wbq_ನಮೂದು->ಕೀಲಿ.flags) && !ret);

	ಮುಕ್ತಾಯ_wait(wq_ತಲೆ, &wbq_ನಮೂದು->wq_ನಮೂದು);

	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(__wait_on_bit);

ಪೂರ್ಣಾಂಕ __sched ಹೊರಹೋಗು_ದ_ಸಾಲು_wait_on_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಿಟ್_2,
				    wait_ಬಿಟ್_action_f *action, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ)
{
	ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ = ಬಿಟ್_waitqueue(ಶಬ್ದ, ಬಿಟ್_2);
	DEFINE_WAIT_ಬಿಟ್(wq_ನಮೂದು, ಶಬ್ದ, ಬಿಟ್_2);

	ಹಿಂತಿರುಗಿ __wait_on_bit(wq_ತಲೆ, &wq_ನಮೂದು, action, ಬಹುಲಕ);
}
EXPORT_ಚಿಹ್ನೆ(ಹೊರಹೋಗು_ದ_ಸಾಲು_wait_on_ಬಿಟ್);

ಪೂರ್ಣಾಂಕ __sched ಹೊರಹೋಗು_ದ_ಸಾಲು_wait_on_ಬಿಟ್_timeout(
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಿಟ್_2, wait_ಬಿಟ್_action_f *action,
	ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ timeout)
{
	ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ = ಬಿಟ್_waitqueue(ಶಬ್ದ, ಬಿಟ್_2);
	DEFINE_WAIT_ಬಿಟ್(wq_ನಮೂದು, ಶಬ್ದ, ಬಿಟ್_2);

	wq_ನಮೂದು.ಕೀಲಿ.timeout = jiffies + timeout;

	ಹಿಂತಿರುಗಿ __wait_on_bit(wq_ತಲೆ, &wq_ನಮೂದು, action, ಬಹುಲಕ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(ಹೊರಹೋಗು_ದ_ಸಾಲು_wait_on_ಬಿಟ್_timeout);

ಪೂರ್ಣಾಂಕ __sched
__wait_on_bit_lock(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು *wbq_ನಮೂದು,
			wait_ಬಿಟ್_action_f *action, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ)
{
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಗಾಗಿ (;;) {
		prepare_ಗಮ್ಯ_wait_exclusive(wq_ತಲೆ, &wbq_ನಮೂದು->wq_ನಮೂದು, ಬಹುಲಕ);
		ಆದರೆ (test_ಬಿಟ್(wbq_ನಮೂದು->ಕೀಲಿ.ಬಿಟ್_nr, wbq_ನಮೂದು->ಕೀಲಿ.flags)) {
			ret = action(&wbq_ನಮೂದು->ಕೀಲಿ, ಬಹುಲಕ);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			ಆದರೆ (ret)
				ಮುಕ್ತಾಯ_wait(wq_ತಲೆ, &wbq_ನಮೂದು->wq_ನಮೂದು);
		}
		ಆದರೆ (!test_and_ಗಣ_ಬಿಟ್(wbq_ನಮೂದು->ಕೀಲಿ.ಬಿಟ್_nr, wbq_ನಮೂದು->ಕೀಲಿ.flags)) {
			ಆದರೆ (!ret)
				ಮುಕ್ತಾಯ_wait(wq_ತಲೆ, &wbq_ನಮೂದು->wq_ನಮೂದು);
			ಹಿಂತಿರುಗಿ 0;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret) {
			ಹಿಂತಿರುಗಿ ret;
		}
	}
}
EXPORT_ಚಿಹ್ನೆ(__wait_on_bit_lock);

ಪೂರ್ಣಾಂಕ __sched ಹೊರಹೋಗು_ದ_ಸಾಲು_wait_on_ಬಿಟ್_ಬೀಗ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಿಟ್_2,
					 wait_ಬಿಟ್_action_f *action, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ)
{
	ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ = ಬಿಟ್_waitqueue(ಶಬ್ದ, ಬಿಟ್_2);
	DEFINE_WAIT_ಬಿಟ್(wq_ನಮೂದು, ಶಬ್ದ, ಬಿಟ್_2);

	ಹಿಂತಿರುಗಿ __wait_on_bit_lock(wq_ತಲೆ, &wq_ನಮೂದು, action, ಬಹುಲಕ);
}
EXPORT_ಚಿಹ್ನೆ(ಹೊರಹೋಗು_ದ_ಸಾಲು_wait_on_ಬಿಟ್_ಬೀಗ);

ಶೂನ್ಯ __wake_up_bit(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಿಟ್_2)
{
	ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ ಕೀಲಿ = __WAIT_BIT_KEY_INITIALIZER(ಶಬ್ದ, ಬಿಟ್_2);

	ಆದರೆ (waitqueue_ಸಕ್ರಿಯ(wq_ತಲೆ))
		__wake_up(wq_ತಲೆ, TASK_ಸಾಮಾನ್ಯ, 1, &ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ(__wake_up_bit);

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
ಶೂನ್ಯ wake_ಮೇಲೆ_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಿಟ್_2)
{
	__wake_up_bit(ಬಿಟ್_waitqueue(ಶಬ್ದ, ಬಿಟ್_2), ಶಬ್ದ, ಬಿಟ್_2);
}
EXPORT_ಚಿಹ್ನೆ(wake_ಮೇಲೆ_ಬಿಟ್);

wait_ಸರತಿ_ತಲೆ_t *__var_waitqueue(ಶೂನ್ಯ *p)
{
	ಹಿಂತಿರುಗಿ ಬಿಟ್_wait_ಕೋಷ್ಟಕ + hash_ptr(p, WAIT_ಕೋಷ್ಟಕ_ಬಿಟ್ಟುಗಳು);
}
EXPORT_ಚಿಹ್ನೆ(__var_waitqueue);

ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *__var_wake_key(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಶೂನ್ಯ *arg)
{
	ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *ಕೀಲಿ = arg;
	ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು *wbq_ನಮೂದು =
		container_ದ(wq_ನಮೂದು, ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು, wq_ನಮೂದು);

	ಆದರೆ (wbq_ನಮೂದು->ಕೀಲಿ.flags != ಕೀಲಿ->flags ||
	    wbq_ನಮೂದು->ಕೀಲಿ.ಬಿಟ್_nr != ಕೀಲಿ->ಬಿಟ್_nr)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ var_wake_function(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
			     ಪೂರ್ಣಾಂಕ sync, ಶೂನ್ಯ *arg)
{
	ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *ಕೀಲಿ = __var_wake_key(wq_ನಮೂದು, arg);
	ಆದರೆ (!ಕೀಲಿ)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ autoremove_wake_function(wq_ನಮೂದು, ಬಹುಲಕ, sync, ಕೀಲಿ);
}

ಶೂನ್ಯ init_wait_var_ನಮೂದು(ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು *wbq_ನಮೂದು, ಶೂನ್ಯ *var, ಪೂರ್ಣಾಂಕ flags)
{
	*wbq_ನಮೂದು = (ರಚನೆ wait_ಬಿಟ್_ಸರತಿ_ನಮೂದು){
		.ಕೀಲಿ = {
			.flags	= (var),
			.ಬಿಟ್_nr = -1,
		},
		.wq_ನಮೂದು = {
			.flags	 = flags,
			.ಖಾಸಗಿ = current,
			.func	 = var_wake_function,
			.ನಮೂದು	 = LIST_ತಲೆ_INIT(wbq_ನಮೂದು->wq_ನಮೂದು.ನಮೂದು),
		},
	};
}
EXPORT_ಚಿಹ್ನೆ(init_wait_var_ನಮೂದು);

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
ಶೂನ್ಯ wake_ಮೇಲೆ_var(ಶೂನ್ಯ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_ಚಿಹ್ನೆ(wake_ಮೇಲೆ_var);

__sched ಪೂರ್ಣಾಂಕ ಬಿಟ್_wait(ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	ವೇಳಾಪಟ್ಟಿ();
	ಆದರೆ (signal_pending_state(ಬಹುಲಕ, current))
		ಹಿಂತಿರುಗಿ -EINTR;

	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(ಬಿಟ್_wait);

__sched ಪೂರ್ಣಾಂಕ ಬಿಟ್_wait_io(ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	io_ವೇಳಾಪಟ್ಟಿ();
	ಆದರೆ (signal_pending_state(ಬಹುಲಕ, current))
		ಹಿಂತಿರುಗಿ -EINTR;

	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(ಬಿಟ್_wait_io);

__sched ಪೂರ್ಣಾಂಕ ಬಿಟ್_wait_timeout(ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *ಶಬ್ದ, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ now = READ_ONCE(jiffies);

	ಆದರೆ (ಸಮಯ_after_eq(now, ಶಬ್ದ->timeout))
		ಹಿಂತಿರುಗಿ -EAGAIN;
	ವೇಳಾಪಟ್ಟಿ_timeout(ಶಬ್ದ->timeout - now);
	ಆದರೆ (signal_pending_state(ಬಹುಲಕ, current))
		ಹಿಂತಿರುಗಿ -EINTR;

	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ_GPL(ಬಿಟ್_wait_timeout);

ಶೂನ್ಯ __init wait_ಬಿಟ್_init(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < WAIT_ಕೋಷ್ಟಕ_ಗಾತ್ರ; i++)
		init_waitqueue_ತಲೆ(ಬಿಟ್_wait_ಕೋಷ್ಟಕ + i);
}
