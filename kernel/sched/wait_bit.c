// SPDX-License-Identifier: GPL-2.0-only

#அந்தர்பாவ <linux/sched/debug.h>
#அந்தர்பாவ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

நிலையான wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, முழு bit)
{
	மாறிலி முழு shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	திரும்பு bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_குறியீடு(bit_waitqueue);

முழு wake_bit_function(கட்டமைப்பு wait_queue_entry *wq_entry, unsigned mode, முழு sync, வெறுமை *arg)
{
	கட்டமைப்பு wait_bit_சாவி *சாவி = arg;
	கட்டமைப்பு wait_bit_queue_entry *wait_bit = container_of(wq_entry, கட்டமைப்பு wait_bit_queue_entry, wq_entry);

	எனில் (wait_bit->சாவி.flags != சாவி->flags ||
			wait_bit->சாவி.bit_nr != சாவி->bit_nr ||
			test_bit(சாவி->bit_nr, சாவி->flags))
		திரும்பு 0;

	திரும்பு autoremove_wake_function(wq_entry, mode, sync, சாவி);
}
EXPORT_குறியீடு(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
முழு __sched
__wait_on_bit(கட்டமைப்பு wait_queue_head *wq_head, கட்டமைப்பு wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	முழு ret = 0;

	செய் {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		எனில் (test_bit(wbq_entry->சாவி.bit_nr, wbq_entry->சாவி.flags))
			ret = (*action)(&wbq_entry->சாவி, mode);
	} போது (test_bit_acquire(wbq_entry->சாவி.bit_nr, wbq_entry->சாவி.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	திரும்பு ret;
}
EXPORT_குறியீடு(__wait_on_bit);

முழு __sched out_of_வரி_wait_on_bit(unsigned long *word, முழு bit,
				    wait_bit_action_f *action, unsigned mode)
{
	கட்டமைப்பு wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	திரும்பு __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_குறியீடு(out_of_வரி_wait_on_bit);

முழு __sched out_of_வரி_wait_on_bit_timeout(
	unsigned long *word, முழு bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	கட்டமைப்பு wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.சாவி.timeout = jiffies + timeout;

	திரும்பு __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_குறியீடு_GPL(out_of_வரி_wait_on_bit_timeout);

முழு __sched
__wait_on_bit_lock(கட்டமைப்பு wait_queue_head *wq_head, கட்டமைப்பு wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	முழு ret = 0;

	க்கு (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		எனில் (test_bit(wbq_entry->சாவி.bit_nr, wbq_entry->சாவி.flags)) {
			ret = action(&wbq_entry->சாவி, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			எனில் (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		எனில் (!test_and_set_bit(wbq_entry->சாவி.bit_nr, wbq_entry->சாவி.flags)) {
			எனில் (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			திரும்பு 0;
		} இல்லையெனில் எனில் (ret) {
			திரும்பு ret;
		}
	}
}
EXPORT_குறியீடு(__wait_on_bit_lock);

முழு __sched out_of_வரி_wait_on_bit_lock(unsigned long *word, முழு bit,
					 wait_bit_action_f *action, unsigned mode)
{
	கட்டமைப்பு wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	திரும்பு __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_குறியீடு(out_of_வரி_wait_on_bit_lock);

வெறுமை __wake_up_bit(கட்டமைப்பு wait_queue_head *wq_head, unsigned long *word, முழு bit)
{
	கட்டமைப்பு wait_bit_சாவி சாவி = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	எனில் (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &சாவி);
}
EXPORT_குறியீடு(__wake_up_bit);

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
வெறுமை wake_up_bit(unsigned long *word, முழு bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_குறியீடு(wake_up_bit);

wait_queue_head_t *__var_waitqueue(வெறுமை *p)
{
	திரும்பு bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_குறியீடு(__var_waitqueue);

கட்டமைப்பு wait_bit_சாவி *__var_wake_key(கட்டமைப்பு wait_queue_entry *wq_entry, வெறுமை *arg)
{
	கட்டமைப்பு wait_bit_சாவி *சாவி = arg;
	கட்டமைப்பு wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, கட்டமைப்பு wait_bit_queue_entry, wq_entry);

	எனில் (wbq_entry->சாவி.flags != சாவி->flags ||
	    wbq_entry->சாவி.bit_nr != சாவி->bit_nr)
		திரும்பு வெற்று;

	திரும்பு சாவி;
}

நிலையான முழு var_wake_function(கட்டமைப்பு wait_queue_entry *wq_entry, unsigned முழு mode,
			     முழு sync, வெறுமை *arg)
{
	கட்டமைப்பு wait_bit_சாவி *சாவி = __var_wake_key(wq_entry, arg);
	எனில் (!சாவி)
		திரும்பு 0;

	திரும்பு autoremove_wake_function(wq_entry, mode, sync, சாவி);
}

வெறுமை init_wait_var_entry(கட்டமைப்பு wait_bit_queue_entry *wbq_entry, வெறுமை *var, முழு flags)
{
	*wbq_entry = (கட்டமைப்பு wait_bit_queue_entry){
		.சாவி = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.தனிப்பட்ட = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_குறியீடு(init_wait_var_entry);

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
வெறுமை wake_up_var(வெறுமை *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_குறியீடு(wake_up_var);

__sched முழு bit_wait(கட்டமைப்பு wait_bit_சாவி *word, முழு mode)
{
	schedule();
	எனில் (signal_pending_state(mode, current))
		திரும்பு -EINTR;

	திரும்பு 0;
}
EXPORT_குறியீடு(bit_wait);

__sched முழு bit_wait_io(கட்டமைப்பு wait_bit_சாவி *word, முழு mode)
{
	io_schedule();
	எனில் (signal_pending_state(mode, current))
		திரும்பு -EINTR;

	திரும்பு 0;
}
EXPORT_குறியீடு(bit_wait_io);

__sched முழு bit_wait_timeout(கட்டமைப்பு wait_bit_சாவி *word, முழு mode)
{
	unsigned long now = READ_ONCE(jiffies);

	எனில் (time_after_eq(now, word->timeout))
		திரும்பு -EAGAIN;
	schedule_timeout(word->timeout - now);
	எனில் (signal_pending_state(mode, current))
		திரும்பு -EINTR;

	திரும்பு 0;
}
EXPORT_குறியீடு_GPL(bit_wait_timeout);

வெறுமை __init wait_bit_init(வெறுமை)
{
	முழு i;

	க்கு (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
