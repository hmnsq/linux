// SPDX-License-Identifier: GPL-2.0-only

#ಅನ್ತರ್ಭಾವ <linux/sched/debug.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

ಸ್ಥಿರ wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, ಪೂರ್ಣಾಙ್ಕ bit)
{
	ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	ಪ್ರತ್ಯಾಗಮ bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_ಚಿಹ್ನೆ(bit_waitqueue);

ಪೂರ್ಣಾಙ್ಕ wake_bit_function(ಸಂರಚನಾ wait_queue_entry *wq_entry, unsigned mode, ಪೂರ್ಣಾಙ್ಕ sync, ಶೂನ್ಯ *arg)
{
	ಸಂರಚನಾ wait_bit_ಕೀಲಿ *ಕೀಲಿ = arg;
	ಸಂರಚನಾ wait_bit_queue_entry *wait_bit = container_of(wq_entry, ಸಂರಚನಾ wait_bit_queue_entry, wq_entry);

	ಯದಿ (wait_bit->ಕೀಲಿ.flags != ಕೀಲಿ->flags ||
			wait_bit->ಕೀಲಿ.bit_nr != ಕೀಲಿ->bit_nr ||
			test_bit(ಕೀಲಿ->bit_nr, ಕೀಲಿ->flags))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ autoremove_wake_function(wq_entry, mode, sync, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
ಪೂರ್ಣಾಙ್ಕ __sched
__wait_on_bit(ಸಂರಚನಾ wait_queue_head *wq_head, ಸಂರಚನಾ wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಕುರು {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		ಯದಿ (test_bit(wbq_entry->ಕೀಲಿ.bit_nr, wbq_entry->ಕೀಲಿ.flags))
			ret = (*action)(&wbq_entry->ಕೀಲಿ, mode);
	} ಯಾವತ್ (test_bit_acquire(wbq_entry->ಕೀಲಿ.bit_nr, wbq_entry->ಕೀಲಿ.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	ಪ್ರತ್ಯಾಗಮ ret;
}
EXPORT_ಚಿಹ್ನೆ(__wait_on_bit);

ಪೂರ್ಣಾಙ್ಕ __sched out_of_ಸಾಲು_wait_on_bit(unsigned long *word, ಪೂರ್ಣಾಙ್ಕ bit,
				    wait_bit_action_f *action, unsigned mode)
{
	ಸಂರಚನಾ wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	ಪ್ರತ್ಯಾಗಮ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_ಚಿಹ್ನೆ(out_of_ಸಾಲು_wait_on_bit);

ಪೂರ್ಣಾಙ್ಕ __sched out_of_ಸಾಲು_wait_on_bit_timeout(
	unsigned long *word, ಪೂರ್ಣಾಙ್ಕ bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	ಸಂರಚನಾ wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.ಕೀಲಿ.timeout = jiffies + timeout;

	ಪ್ರತ್ಯಾಗಮ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_ಚಿಹ್ನೆ_GPL(out_of_ಸಾಲು_wait_on_bit_timeout);

ಪೂರ್ಣಾಙ್ಕ __sched
__wait_on_bit_lock(ಸಂರಚನಾ wait_queue_head *wq_head, ಸಂರಚನಾ wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಪ್ರತಿ (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		ಯದಿ (test_bit(wbq_entry->ಕೀಲಿ.bit_nr, wbq_entry->ಕೀಲಿ.flags)) {
			ret = action(&wbq_entry->ಕೀಲಿ, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			ಯದಿ (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		ಯದಿ (!test_and_set_bit(wbq_entry->ಕೀಲಿ.bit_nr, wbq_entry->ಕೀಲಿ.flags)) {
			ಯದಿ (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			ಪ್ರತ್ಯಾಗಮ 0;
		} ಅನ್ಯಥಾ ಯದಿ (ret) {
			ಪ್ರತ್ಯಾಗಮ ret;
		}
	}
}
EXPORT_ಚಿಹ್ನೆ(__wait_on_bit_lock);

ಪೂರ್ಣಾಙ್ಕ __sched out_of_ಸಾಲು_wait_on_bit_lock(unsigned long *word, ಪೂರ್ಣಾಙ್ಕ bit,
					 wait_bit_action_f *action, unsigned mode)
{
	ಸಂರಚನಾ wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	ಪ್ರತ್ಯಾಗಮ __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_ಚಿಹ್ನೆ(out_of_ಸಾಲು_wait_on_bit_lock);

ಶೂನ್ಯ __wake_up_bit(ಸಂರಚನಾ wait_queue_head *wq_head, unsigned long *word, ಪೂರ್ಣಾಙ್ಕ bit)
{
	ಸಂರಚನಾ wait_bit_ಕೀಲಿ ಕೀಲಿ = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	ಯದಿ (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &ಕೀಲಿ);
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
ಶೂನ್ಯ wake_up_bit(unsigned long *word, ಪೂರ್ಣಾಙ್ಕ bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_ಚಿಹ್ನೆ(wake_up_bit);

wait_queue_head_t *__var_waitqueue(ಶೂನ್ಯ *p)
{
	ಪ್ರತ್ಯಾಗಮ bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_ಚಿಹ್ನೆ(__var_waitqueue);

ಸಂರಚನಾ wait_bit_ಕೀಲಿ *__var_wake_key(ಸಂರಚನಾ wait_queue_entry *wq_entry, ಶೂನ್ಯ *arg)
{
	ಸಂರಚನಾ wait_bit_ಕೀಲಿ *ಕೀಲಿ = arg;
	ಸಂರಚನಾ wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, ಸಂರಚನಾ wait_bit_queue_entry, wq_entry);

	ಯದಿ (wbq_entry->ಕೀಲಿ.flags != ಕೀಲಿ->flags ||
	    wbq_entry->ಕೀಲಿ.bit_nr != ಕೀಲಿ->bit_nr)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ var_wake_function(ಸಂರಚನಾ wait_queue_entry *wq_entry, unsigned ಪೂರ್ಣಾಙ್ಕ mode,
			     ಪೂರ್ಣಾಙ್ಕ sync, ಶೂನ್ಯ *arg)
{
	ಸಂರಚನಾ wait_bit_ಕೀಲಿ *ಕೀಲಿ = __var_wake_key(wq_entry, arg);
	ಯದಿ (!ಕೀಲಿ)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ autoremove_wake_function(wq_entry, mode, sync, ಕೀಲಿ);
}

ಶೂನ್ಯ init_wait_var_entry(ಸಂರಚನಾ wait_bit_queue_entry *wbq_entry, ಶೂನ್ಯ *var, ಪೂರ್ಣಾಙ್ಕ flags)
{
	*wbq_entry = (ಸಂರಚನಾ wait_bit_queue_entry){
		.ಕೀಲಿ = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.ಗುಪ್ತ = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_ಚಿಹ್ನೆ(init_wait_var_entry);

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
ಶೂನ್ಯ wake_up_var(ಶೂನ್ಯ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_ಚಿಹ್ನೆ(wake_up_var);

__sched ಪೂರ್ಣಾಙ್ಕ bit_wait(ಸಂರಚನಾ wait_bit_ಕೀಲಿ *word, ಪೂರ್ಣಾಙ್ಕ mode)
{
	schedule();
	ಯದಿ (signal_pending_state(mode, current))
		ಪ್ರತ್ಯಾಗಮ -EINTR;

	ಪ್ರತ್ಯಾಗಮ 0;
}
EXPORT_ಚಿಹ್ನೆ(bit_wait);

__sched ಪೂರ್ಣಾಙ್ಕ bit_wait_io(ಸಂರಚನಾ wait_bit_ಕೀಲಿ *word, ಪೂರ್ಣಾಙ್ಕ mode)
{
	io_schedule();
	ಯದಿ (signal_pending_state(mode, current))
		ಪ್ರತ್ಯಾಗಮ -EINTR;

	ಪ್ರತ್ಯಾಗಮ 0;
}
EXPORT_ಚಿಹ್ನೆ(bit_wait_io);

__sched ಪೂರ್ಣಾಙ್ಕ bit_wait_timeout(ಸಂರಚನಾ wait_bit_ಕೀಲಿ *word, ಪೂರ್ಣಾಙ್ಕ mode)
{
	unsigned long now = READ_ONCE(jiffies);

	ಯದಿ (time_after_eq(now, word->timeout))
		ಪ್ರತ್ಯಾಗಮ -EAGAIN;
	schedule_timeout(word->timeout - now);
	ಯದಿ (signal_pending_state(mode, current))
		ಪ್ರತ್ಯಾಗಮ -EINTR;

	ಪ್ರತ್ಯಾಗಮ 0;
}
EXPORT_ಚಿಹ್ನೆ_GPL(bit_wait_timeout);

ಶೂನ್ಯ __init wait_bit_init(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
