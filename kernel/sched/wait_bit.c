// SPDX-License-Identifier: GPL-2.0-only

#ضمن <linux/sched/debug.h>
#ضمن "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

ساكن wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, صحيح bit)
{
	ثابت صحيح shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	إرجاع bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_رمز(bit_waitqueue);

صحيح wake_bit_function(بنية wait_queue_entry *wq_entry, unsigned mode, صحيح sync, فراغ *arg)
{
	بنية wait_bit_مفتاح *مفتاح = arg;
	بنية wait_bit_queue_entry *wait_bit = container_of(wq_entry, بنية wait_bit_queue_entry, wq_entry);

	إذا (wait_bit->مفتاح.flags != مفتاح->flags ||
			wait_bit->مفتاح.bit_nr != مفتاح->bit_nr ||
			test_bit(مفتاح->bit_nr, مفتاح->flags))
		إرجاع 0;

	إرجاع autoremove_wake_function(wq_entry, mode, sync, مفتاح);
}
EXPORT_رمز(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
صحيح __sched
__wait_on_bit(بنية wait_queue_head *wq_head, بنية wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	صحيح ret = 0;

	افعل {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		إذا (test_bit(wbq_entry->مفتاح.bit_nr, wbq_entry->مفتاح.flags))
			ret = (*action)(&wbq_entry->مفتاح, mode);
	} بينما (test_bit_acquire(wbq_entry->مفتاح.bit_nr, wbq_entry->مفتاح.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	إرجاع ret;
}
EXPORT_رمز(__wait_on_bit);

صحيح __sched out_of_سطر_wait_on_bit(unsigned long *word, صحيح bit,
				    wait_bit_action_f *action, unsigned mode)
{
	بنية wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	إرجاع __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_رمز(out_of_سطر_wait_on_bit);

صحيح __sched out_of_سطر_wait_on_bit_timeout(
	unsigned long *word, صحيح bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	بنية wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.مفتاح.timeout = jiffies + timeout;

	إرجاع __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_رمز_GPL(out_of_سطر_wait_on_bit_timeout);

صحيح __sched
__wait_on_bit_lock(بنية wait_queue_head *wq_head, بنية wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	صحيح ret = 0;

	لكل (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		إذا (test_bit(wbq_entry->مفتاح.bit_nr, wbq_entry->مفتاح.flags)) {
			ret = action(&wbq_entry->مفتاح, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			إذا (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		إذا (!test_and_set_bit(wbq_entry->مفتاح.bit_nr, wbq_entry->مفتاح.flags)) {
			إذا (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			إرجاع 0;
		} وإلا إذا (ret) {
			إرجاع ret;
		}
	}
}
EXPORT_رمز(__wait_on_bit_lock);

صحيح __sched out_of_سطر_wait_on_bit_lock(unsigned long *word, صحيح bit,
					 wait_bit_action_f *action, unsigned mode)
{
	بنية wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	إرجاع __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_رمز(out_of_سطر_wait_on_bit_lock);

فراغ __wake_up_bit(بنية wait_queue_head *wq_head, unsigned long *word, صحيح bit)
{
	بنية wait_bit_مفتاح مفتاح = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	إذا (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &مفتاح);
}
EXPORT_رمز(__wake_up_bit);

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
فراغ wake_up_bit(unsigned long *word, صحيح bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_رمز(wake_up_bit);

wait_queue_head_t *__var_waitqueue(فراغ *p)
{
	إرجاع bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_رمز(__var_waitqueue);

بنية wait_bit_مفتاح *__var_wake_key(بنية wait_queue_entry *wq_entry, فراغ *arg)
{
	بنية wait_bit_مفتاح *مفتاح = arg;
	بنية wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, بنية wait_bit_queue_entry, wq_entry);

	إذا (wbq_entry->مفتاح.flags != مفتاح->flags ||
	    wbq_entry->مفتاح.bit_nr != مفتاح->bit_nr)
		إرجاع عدم;

	إرجاع مفتاح;
}

ساكن صحيح var_wake_function(بنية wait_queue_entry *wq_entry, unsigned صحيح mode,
			     صحيح sync, فراغ *arg)
{
	بنية wait_bit_مفتاح *مفتاح = __var_wake_key(wq_entry, arg);
	إذا (!مفتاح)
		إرجاع 0;

	إرجاع autoremove_wake_function(wq_entry, mode, sync, مفتاح);
}

فراغ init_wait_var_entry(بنية wait_bit_queue_entry *wbq_entry, فراغ *var, صحيح flags)
{
	*wbq_entry = (بنية wait_bit_queue_entry){
		.مفتاح = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.خاص = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_رمز(init_wait_var_entry);

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
فراغ wake_up_var(فراغ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_رمز(wake_up_var);

__sched صحيح bit_wait(بنية wait_bit_مفتاح *word, صحيح mode)
{
	schedule();
	إذا (signal_pending_state(mode, current))
		إرجاع -EINTR;

	إرجاع 0;
}
EXPORT_رمز(bit_wait);

__sched صحيح bit_wait_io(بنية wait_bit_مفتاح *word, صحيح mode)
{
	io_schedule();
	إذا (signal_pending_state(mode, current))
		إرجاع -EINTR;

	إرجاع 0;
}
EXPORT_رمز(bit_wait_io);

__sched صحيح bit_wait_timeout(بنية wait_bit_مفتاح *word, صحيح mode)
{
	unsigned long now = READ_ONCE(jiffies);

	إذا (time_after_eq(now, word->timeout))
		إرجاع -EAGAIN;
	schedule_timeout(word->timeout - now);
	إذا (signal_pending_state(mode, current))
		إرجاع -EINTR;

	إرجاع 0;
}
EXPORT_رمز_GPL(bit_wait_timeout);

فراغ __init wait_bit_init(فراغ)
{
	صحيح i;

	لكل (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
