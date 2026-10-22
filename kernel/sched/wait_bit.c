// SPDX-License-Identifier: GPL-2.0-only

#include <linux/sched/debug.h>
#include "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

static wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, ຈຳນວນເຕັມ bit)
{
	const ຈຳນວນເຕັມ shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	ສົ່ງຄືນ bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_SYMBOL(bit_waitqueue);

ຈຳນວນເຕັມ wake_bit_function(sa_nracanaa wait_queue_entry *wq_entry, unsigned mode, ຈຳນວນເຕັມ sync, void *arg)
{
	sa_nracanaa wait_bit_key *key = arg;
	sa_nracanaa wait_bit_queue_entry *wait_bit = container_of(wq_entry, sa_nracanaa wait_bit_queue_entry, wq_entry);

	ຖ້າ (wait_bit->key.flags != key->flags ||
			wait_bit->key.bit_nr != key->bit_nr ||
			test_bit(key->bit_nr, key->flags))
		ສົ່ງຄືນ 0;

	ສົ່ງຄືນ autoremove_wake_function(wq_entry, mode, sync, key);
}
EXPORT_SYMBOL(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
ຈຳນວນເຕັມ __sched
__wait_on_bit(sa_nracanaa wait_queue_head *wq_head, sa_nracanaa wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	ຈຳນວນເຕັມ ret = 0;

	do {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		ຖ້າ (test_bit(wbq_entry->key.bit_nr, wbq_entry->key.flags))
			ret = (*action)(&wbq_entry->key, mode);
	} ໃນຂະນະທີ່ (test_bit_acquire(wbq_entry->key.bit_nr, wbq_entry->key.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	ສົ່ງຄືນ ret;
}
EXPORT_SYMBOL(__wait_on_bit);

ຈຳນວນເຕັມ __sched out_of_line_wait_on_bit(unsigned long *word, ຈຳນວນເຕັມ bit,
				    wait_bit_action_f *action, unsigned mode)
{
	sa_nracanaa wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	ສົ່ງຄືນ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_SYMBOL(out_of_line_wait_on_bit);

ຈຳນວນເຕັມ __sched out_of_line_wait_on_bit_timeout(
	unsigned long *word, ຈຳນວນເຕັມ bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	sa_nracanaa wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.key.timeout = jiffies + timeout;

	ສົ່ງຄືນ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_SYMBOL_GPL(out_of_line_wait_on_bit_timeout);

ຈຳນວນເຕັມ __sched
__wait_on_bit_lock(sa_nracanaa wait_queue_head *wq_head, sa_nracanaa wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	ຈຳນວນເຕັມ ret = 0;

	ສຳລັບ (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		ຖ້າ (test_bit(wbq_entry->key.bit_nr, wbq_entry->key.flags)) {
			ret = action(&wbq_entry->key, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			ຖ້າ (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		ຖ້າ (!test_and_set_bit(wbq_entry->key.bit_nr, wbq_entry->key.flags)) {
			ຖ້າ (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			ສົ່ງຄືນ 0;
		} ບໍ່ດັ່ງນັ້ນ ຖ້າ (ret) {
			ສົ່ງຄືນ ret;
		}
	}
}
EXPORT_SYMBOL(__wait_on_bit_lock);

ຈຳນວນເຕັມ __sched out_of_line_wait_on_bit_lock(unsigned long *word, ຈຳນວນເຕັມ bit,
					 wait_bit_action_f *action, unsigned mode)
{
	sa_nracanaa wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	ສົ່ງຄືນ __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_SYMBOL(out_of_line_wait_on_bit_lock);

void __wake_up_bit(sa_nracanaa wait_queue_head *wq_head, unsigned long *word, ຈຳນວນເຕັມ bit)
{
	sa_nracanaa wait_bit_key key = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	ຖ້າ (waitqueue_active(wq_head))
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
void wake_up_bit(unsigned long *word, ຈຳນວນເຕັມ bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_SYMBOL(wake_up_bit);

wait_queue_head_t *__var_waitqueue(void *p)
{
	ສົ່ງຄືນ bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_SYMBOL(__var_waitqueue);

sa_nracanaa wait_bit_key *__var_wake_key(sa_nracanaa wait_queue_entry *wq_entry, void *arg)
{
	sa_nracanaa wait_bit_key *key = arg;
	sa_nracanaa wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, sa_nracanaa wait_bit_queue_entry, wq_entry);

	ຖ້າ (wbq_entry->key.flags != key->flags ||
	    wbq_entry->key.bit_nr != key->bit_nr)
		ສົ່ງຄືນ NULL;

	ສົ່ງຄືນ key;
}

static ຈຳນວນເຕັມ var_wake_function(sa_nracanaa wait_queue_entry *wq_entry, unsigned ຈຳນວນເຕັມ mode,
			     ຈຳນວນເຕັມ sync, void *arg)
{
	sa_nracanaa wait_bit_key *key = __var_wake_key(wq_entry, arg);
	ຖ້າ (!key)
		ສົ່ງຄືນ 0;

	ສົ່ງຄືນ autoremove_wake_function(wq_entry, mode, sync, key);
}

void init_wait_var_entry(sa_nracanaa wait_bit_queue_entry *wbq_entry, void *var, ຈຳນວນເຕັມ flags)
{
	*wbq_entry = (sa_nracanaa wait_bit_queue_entry){
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

__sched ຈຳນວນເຕັມ bit_wait(sa_nracanaa wait_bit_key *word, ຈຳນວນເຕັມ mode)
{
	schedule();
	ຖ້າ (signal_pending_state(mode, current))
		ສົ່ງຄືນ -EINTR;

	ສົ່ງຄືນ 0;
}
EXPORT_SYMBOL(bit_wait);

__sched ຈຳນວນເຕັມ bit_wait_io(sa_nracanaa wait_bit_key *word, ຈຳນວນເຕັມ mode)
{
	io_schedule();
	ຖ້າ (signal_pending_state(mode, current))
		ສົ່ງຄືນ -EINTR;

	ສົ່ງຄືນ 0;
}
EXPORT_SYMBOL(bit_wait_io);

__sched ຈຳນວນເຕັມ bit_wait_timeout(sa_nracanaa wait_bit_key *word, ຈຳນວນເຕັມ mode)
{
	unsigned long now = READ_ONCE(jiffies);

	ຖ້າ (time_after_eq(now, word->timeout))
		ສົ່ງຄືນ -EAGAIN;
	schedule_timeout(word->timeout - now);
	ຖ້າ (signal_pending_state(mode, current))
		ສົ່ງຄືນ -EINTR;

	ສົ່ງຄືນ 0;
}
EXPORT_SYMBOL_GPL(bit_wait_timeout);

void __init wait_bit_init(void)
{
	ຈຳນວນເຕັມ i;

	ສຳລັບ (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
