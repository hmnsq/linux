// SPDX-License-Identifier: GPL-2.0-only

#include <linux/sched/debug.h>
#include "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

static wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, ціле bit)
{
	const ціле shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	повернути bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_SYMBOL(bit_waitqueue);

ціле wake_bit_function(struct wait_queue_entry *wq_entry, unsigned mode, ціле sync, void *arg)
{
	struct wait_bit_key *key = arg;
	struct wait_bit_queue_entry *wait_bit = container_of(wq_entry, struct wait_bit_queue_entry, wq_entry);

	якщо (wait_bit->key.flags != key->flags ||
			wait_bit->key.bit_nr != key->bit_nr ||
			test_bit(key->bit_nr, key->flags))
		повернути 0;

	повернути autoremove_wake_function(wq_entry, mode, sync, key);
}
EXPORT_SYMBOL(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
ціле __sched
__wait_on_bit(struct wait_queue_head *wq_head, struct wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	ціле ret = 0;

	do {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		якщо (test_bit(wbq_entry->key.bit_nr, wbq_entry->key.flags))
			ret = (*action)(&wbq_entry->key, mode);
	} поки (test_bit_acquire(wbq_entry->key.bit_nr, wbq_entry->key.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	повернути ret;
}
EXPORT_SYMBOL(__wait_on_bit);

ціле __sched out_of_line_wait_on_bit(unsigned long *word, ціле bit,
				    wait_bit_action_f *action, unsigned mode)
{
	struct wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	повернути __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_SYMBOL(out_of_line_wait_on_bit);

ціле __sched out_of_line_wait_on_bit_timeout(
	unsigned long *word, ціле bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	struct wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.key.timeout = jiffies + timeout;

	повернути __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_SYMBOL_GPL(out_of_line_wait_on_bit_timeout);

ціле __sched
__wait_on_bit_lock(struct wait_queue_head *wq_head, struct wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	ціле ret = 0;

	для (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		якщо (test_bit(wbq_entry->key.bit_nr, wbq_entry->key.flags)) {
			ret = action(&wbq_entry->key, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			якщо (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		якщо (!test_and_set_bit(wbq_entry->key.bit_nr, wbq_entry->key.flags)) {
			якщо (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			повернути 0;
		} інакше якщо (ret) {
			повернути ret;
		}
	}
}
EXPORT_SYMBOL(__wait_on_bit_lock);

ціле __sched out_of_line_wait_on_bit_lock(unsigned long *word, ціле bit,
					 wait_bit_action_f *action, unsigned mode)
{
	struct wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	повернути __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_SYMBOL(out_of_line_wait_on_bit_lock);

void __wake_up_bit(struct wait_queue_head *wq_head, unsigned long *word, ціле bit)
{
	struct wait_bit_key key = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	якщо (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &key);
}
EXPORT_SYMBOL(__wake_up_bit);

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
void wake_up_bit(unsigned long *word, ціле bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_SYMBOL(wake_up_bit);

wait_queue_head_t *__var_waitqueue(void *p)
{
	повернути bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_SYMBOL(__var_waitqueue);

struct wait_bit_key *__var_wake_key(struct wait_queue_entry *wq_entry, void *arg)
{
	struct wait_bit_key *key = arg;
	struct wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, struct wait_bit_queue_entry, wq_entry);

	якщо (wbq_entry->key.flags != key->flags ||
	    wbq_entry->key.bit_nr != key->bit_nr)
		повернути NULL;

	повернути key;
}

static ціле var_wake_function(struct wait_queue_entry *wq_entry, unsigned ціле mode,
			     ціле sync, void *arg)
{
	struct wait_bit_key *key = __var_wake_key(wq_entry, arg);
	якщо (!key)
		повернути 0;

	повернути autoremove_wake_function(wq_entry, mode, sync, key);
}

void init_wait_var_entry(struct wait_bit_queue_entry *wbq_entry, void *var, ціле flags)
{
	*wbq_entry = (struct wait_bit_queue_entry){
		.key = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.private = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_SYMBOL(init_wait_var_entry);

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
void wake_up_var(void *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_SYMBOL(wake_up_var);

__sched ціле bit_wait(struct wait_bit_key *word, ціле mode)
{
	schedule();
	якщо (signal_pending_state(mode, current))
		повернути -EINTR;

	повернути 0;
}
EXPORT_SYMBOL(bit_wait);

__sched ціле bit_wait_io(struct wait_bit_key *word, ціле mode)
{
	io_schedule();
	якщо (signal_pending_state(mode, current))
		повернути -EINTR;

	повернути 0;
}
EXPORT_SYMBOL(bit_wait_io);

__sched ціле bit_wait_timeout(struct wait_bit_key *word, ціле mode)
{
	unsigned long now = READ_ONCE(jiffies);

	якщо (time_after_eq(now, word->timeout))
		повернути -EAGAIN;
	schedule_timeout(word->timeout - now);
	якщо (signal_pending_state(mode, current))
		повернути -EINTR;

	повернути 0;
}
EXPORT_SYMBOL_GPL(bit_wait_timeout);

void __init wait_bit_init(void)
{
	ціле i;

	для (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
