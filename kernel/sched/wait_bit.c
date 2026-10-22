// SPDX-License-Identifier: GPL-2.0-only

#شامل <linux/sched/debug.h>
#شامل "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

ایستا wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, صحیح bit)
{
	ثابت صحیح shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	بازگشت bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_نماد(bit_waitqueue);

صحیح wake_bit_function(ساختار wait_queue_entry *wq_entry, unsigned mode, صحیح sync, تهی *arg)
{
	ساختار wait_bit_کلید *کلید = arg;
	ساختار wait_bit_queue_entry *wait_bit = container_of(wq_entry, ساختار wait_bit_queue_entry, wq_entry);

	اگر (wait_bit->کلید.flags != کلید->flags ||
			wait_bit->کلید.bit_nr != کلید->bit_nr ||
			test_bit(کلید->bit_nr, کلید->flags))
		بازگشت 0;

	بازگشت autoremove_wake_function(wq_entry, mode, sync, کلید);
}
EXPORT_نماد(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
صحیح __sched
__wait_on_bit(ساختار wait_queue_head *wq_head, ساختار wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	صحیح ret = 0;

	انجامبده {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		اگر (test_bit(wbq_entry->کلید.bit_nr, wbq_entry->کلید.flags))
			ret = (*action)(&wbq_entry->کلید, mode);
	} وقتی (test_bit_acquire(wbq_entry->کلید.bit_nr, wbq_entry->کلید.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	بازگشت ret;
}
EXPORT_نماد(__wait_on_bit);

صحیح __sched out_of_سطر_wait_on_bit(unsigned long *word, صحیح bit,
				    wait_bit_action_f *action, unsigned mode)
{
	ساختار wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	بازگشت __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_نماد(out_of_سطر_wait_on_bit);

صحیح __sched out_of_سطر_wait_on_bit_timeout(
	unsigned long *word, صحیح bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	ساختار wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.کلید.timeout = jiffies + timeout;

	بازگشت __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_نماد_GPL(out_of_سطر_wait_on_bit_timeout);

صحیح __sched
__wait_on_bit_lock(ساختار wait_queue_head *wq_head, ساختار wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	صحیح ret = 0;

	برای (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		اگر (test_bit(wbq_entry->کلید.bit_nr, wbq_entry->کلید.flags)) {
			ret = action(&wbq_entry->کلید, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			اگر (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		اگر (!test_and_set_bit(wbq_entry->کلید.bit_nr, wbq_entry->کلید.flags)) {
			اگر (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			بازگشت 0;
		} وگرنه اگر (ret) {
			بازگشت ret;
		}
	}
}
EXPORT_نماد(__wait_on_bit_lock);

صحیح __sched out_of_سطر_wait_on_bit_lock(unsigned long *word, صحیح bit,
					 wait_bit_action_f *action, unsigned mode)
{
	ساختار wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	بازگشت __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_نماد(out_of_سطر_wait_on_bit_lock);

تهی __wake_up_bit(ساختار wait_queue_head *wq_head, unsigned long *word, صحیح bit)
{
	ساختار wait_bit_کلید کلید = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	اگر (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &کلید);
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
تهی wake_up_bit(unsigned long *word, صحیح bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_نماد(wake_up_bit);

wait_queue_head_t *__var_waitqueue(تهی *p)
{
	بازگشت bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_نماد(__var_waitqueue);

ساختار wait_bit_کلید *__var_wake_key(ساختار wait_queue_entry *wq_entry, تهی *arg)
{
	ساختار wait_bit_کلید *کلید = arg;
	ساختار wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, ساختار wait_bit_queue_entry, wq_entry);

	اگر (wbq_entry->کلید.flags != کلید->flags ||
	    wbq_entry->کلید.bit_nr != کلید->bit_nr)
		بازگشت پوچ;

	بازگشت کلید;
}

ایستا صحیح var_wake_function(ساختار wait_queue_entry *wq_entry, unsigned صحیح mode,
			     صحیح sync, تهی *arg)
{
	ساختار wait_bit_کلید *کلید = __var_wake_key(wq_entry, arg);
	اگر (!کلید)
		بازگشت 0;

	بازگشت autoremove_wake_function(wq_entry, mode, sync, کلید);
}

تهی init_wait_var_entry(ساختار wait_bit_queue_entry *wbq_entry, تهی *var, صحیح flags)
{
	*wbq_entry = (ساختار wait_bit_queue_entry){
		.کلید = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.خصوصی = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_نماد(init_wait_var_entry);

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
تهی wake_up_var(تهی *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_نماد(wake_up_var);

__sched صحیح bit_wait(ساختار wait_bit_کلید *word, صحیح mode)
{
	schedule();
	اگر (signal_pending_state(mode, current))
		بازگشت -EINTR;

	بازگشت 0;
}
EXPORT_نماد(bit_wait);

__sched صحیح bit_wait_io(ساختار wait_bit_کلید *word, صحیح mode)
{
	io_schedule();
	اگر (signal_pending_state(mode, current))
		بازگشت -EINTR;

	بازگشت 0;
}
EXPORT_نماد(bit_wait_io);

__sched صحیح bit_wait_timeout(ساختار wait_bit_کلید *word, صحیح mode)
{
	unsigned long now = READ_ONCE(jiffies);

	اگر (time_after_eq(now, word->timeout))
		بازگشت -EAGAIN;
	schedule_timeout(word->timeout - now);
	اگر (signal_pending_state(mode, current))
		بازگشت -EINTR;

	بازگشت 0;
}
EXPORT_نماد_GPL(bit_wait_timeout);

تهی __init wait_bit_init(تهی)
{
	صحیح i;

	برای (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
