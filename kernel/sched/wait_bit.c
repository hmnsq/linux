// SPDX-License-Identifier: GPL-2.0-only

#অন্তর্ভাব <linux/sched/debug.h>
#অন্তর্ভাব "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

স্থির wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, পূর্ণাঙ্ক bit)
{
	ধ্রুব পূর্ণাঙ্ক shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	প্রত্যাগম bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_প্রতীক(bit_waitqueue);

পূর্ণাঙ্ক wake_bit_function(সংরচনা wait_queue_entry *wq_entry, unsigned mode, পূর্ণাঙ্ক sync, শূন্য *arg)
{
	সংরচনা wait_bit_চাবি *চাবি = arg;
	সংরচনা wait_bit_queue_entry *wait_bit = container_of(wq_entry, সংরচনা wait_bit_queue_entry, wq_entry);

	যদি (wait_bit->চাবি.flags != চাবি->flags ||
			wait_bit->চাবি.bit_nr != চাবি->bit_nr ||
			test_bit(চাবি->bit_nr, চাবি->flags))
		প্রত্যাগম 0;

	প্রত্যাগম autoremove_wake_function(wq_entry, mode, sync, চাবি);
}
EXPORT_প্রতীক(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
পূর্ণাঙ্ক __sched
__wait_on_bit(সংরচনা wait_queue_head *wq_head, সংরচনা wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	পূর্ণাঙ্ক ret = 0;

	কুরু {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		যদি (test_bit(wbq_entry->চাবি.bit_nr, wbq_entry->চাবি.flags))
			ret = (*action)(&wbq_entry->চাবি, mode);
	} যাবত্ (test_bit_acquire(wbq_entry->চাবি.bit_nr, wbq_entry->চাবি.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	প্রত্যাগম ret;
}
EXPORT_প্রতীক(__wait_on_bit);

পূর্ণাঙ্ক __sched out_of_সারি_wait_on_bit(unsigned long *word, পূর্ণাঙ্ক bit,
				    wait_bit_action_f *action, unsigned mode)
{
	সংরচনা wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	প্রত্যাগম __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_প্রতীক(out_of_সারি_wait_on_bit);

পূর্ণাঙ্ক __sched out_of_সারি_wait_on_bit_timeout(
	unsigned long *word, পূর্ণাঙ্ক bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	সংরচনা wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.চাবি.timeout = jiffies + timeout;

	প্রত্যাগম __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_প্রতীক_GPL(out_of_সারি_wait_on_bit_timeout);

পূর্ণাঙ্ক __sched
__wait_on_bit_lock(সংরচনা wait_queue_head *wq_head, সংরচনা wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	পূর্ণাঙ্ক ret = 0;

	প্রতি (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		যদি (test_bit(wbq_entry->চাবি.bit_nr, wbq_entry->চাবি.flags)) {
			ret = action(&wbq_entry->চাবি, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			যদি (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		যদি (!test_and_set_bit(wbq_entry->চাবি.bit_nr, wbq_entry->চাবি.flags)) {
			যদি (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			প্রত্যাগম 0;
		} অন্যথা যদি (ret) {
			প্রত্যাগম ret;
		}
	}
}
EXPORT_প্রতীক(__wait_on_bit_lock);

পূর্ণাঙ্ক __sched out_of_সারি_wait_on_bit_lock(unsigned long *word, পূর্ণাঙ্ক bit,
					 wait_bit_action_f *action, unsigned mode)
{
	সংরচনা wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	প্রত্যাগম __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_প্রতীক(out_of_সারি_wait_on_bit_lock);

শূন্য __wake_up_bit(সংরচনা wait_queue_head *wq_head, unsigned long *word, পূর্ণাঙ্ক bit)
{
	সংরচনা wait_bit_চাবি চাবি = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	যদি (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &চাবি);
}
EXPORT_প্রতীক(__wake_up_bit);

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
শূন্য wake_up_bit(unsigned long *word, পূর্ণাঙ্ক bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_প্রতীক(wake_up_bit);

wait_queue_head_t *__var_waitqueue(শূন্য *p)
{
	প্রত্যাগম bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_প্রতীক(__var_waitqueue);

সংরচনা wait_bit_চাবি *__var_wake_key(সংরচনা wait_queue_entry *wq_entry, শূন্য *arg)
{
	সংরচনা wait_bit_চাবি *চাবি = arg;
	সংরচনা wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, সংরচনা wait_bit_queue_entry, wq_entry);

	যদি (wbq_entry->চাবি.flags != চাবি->flags ||
	    wbq_entry->চাবি.bit_nr != চাবি->bit_nr)
		প্রত্যাগম রিক্ত;

	প্রত্যাগম চাবি;
}

স্থির পূর্ণাঙ্ক var_wake_function(সংরচনা wait_queue_entry *wq_entry, unsigned পূর্ণাঙ্ক mode,
			     পূর্ণাঙ্ক sync, শূন্য *arg)
{
	সংরচনা wait_bit_চাবি *চাবি = __var_wake_key(wq_entry, arg);
	যদি (!চাবি)
		প্রত্যাগম 0;

	প্রত্যাগম autoremove_wake_function(wq_entry, mode, sync, চাবি);
}

শূন্য init_wait_var_entry(সংরচনা wait_bit_queue_entry *wbq_entry, শূন্য *var, পূর্ণাঙ্ক flags)
{
	*wbq_entry = (সংরচনা wait_bit_queue_entry){
		.চাবি = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.গুপ্ত = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_প্রতীক(init_wait_var_entry);

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
শূন্য wake_up_var(শূন্য *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_প্রতীক(wake_up_var);

__sched পূর্ণাঙ্ক bit_wait(সংরচনা wait_bit_চাবি *word, পূর্ণাঙ্ক mode)
{
	schedule();
	যদি (signal_pending_state(mode, current))
		প্রত্যাগম -EINTR;

	প্রত্যাগম 0;
}
EXPORT_প্রতীক(bit_wait);

__sched পূর্ণাঙ্ক bit_wait_io(সংরচনা wait_bit_চাবি *word, পূর্ণাঙ্ক mode)
{
	io_schedule();
	যদি (signal_pending_state(mode, current))
		প্রত্যাগম -EINTR;

	প্রত্যাগম 0;
}
EXPORT_প্রতীক(bit_wait_io);

__sched পূর্ণাঙ্ক bit_wait_timeout(সংরচনা wait_bit_চাবি *word, পূর্ণাঙ্ক mode)
{
	unsigned long now = READ_ONCE(jiffies);

	যদি (time_after_eq(now, word->timeout))
		প্রত্যাগম -EAGAIN;
	schedule_timeout(word->timeout - now);
	যদি (signal_pending_state(mode, current))
		প্রত্যাগম -EINTR;

	প্রত্যাগম 0;
}
EXPORT_প্রতীক_GPL(bit_wait_timeout);

শূন্য __init wait_bit_init(শূন্য)
{
	পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
