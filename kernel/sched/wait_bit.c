// SPDX-License-Identifier: GPL-2.0-only

#अन्तर्भाव <linux/sched/debug.h>
#अन्तर्भाव "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

स्थिर wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, पूर्णाङ्क bit)
{
	ध्रुव पूर्णाङ्क shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	पटिनिवत्त bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_चिह्नम्(bit_waitqueue);

पूर्णाङ्क wake_bit_function(संरचना wait_queue_entry *wq_entry, unsigned mode, पूर्णाङ्क sync, शून्य *arg)
{
	संरचना wait_bit_कुञ्चिका *कुञ्चिका = arg;
	संरचना wait_bit_queue_entry *wait_bit = container_of(wq_entry, संरचना wait_bit_queue_entry, wq_entry);

	सचे (wait_bit->कुञ्चिका.flags != कुञ्चिका->flags ||
			wait_bit->कुञ्चिका.bit_nr != कुञ्चिका->bit_nr ||
			test_bit(कुञ्चिका->bit_nr, कुञ्चिका->flags))
		पटिनिवत्त 0;

	पटिनिवत्त autoremove_wake_function(wq_entry, mode, sync, कुञ्चिका);
}
EXPORT_चिह्नम्(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
पूर्णाङ्क __sched
__wait_on_bit(संरचना wait_queue_head *wq_head, संरचना wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	पूर्णाङ्क ret = 0;

	करोहि {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		सचे (test_bit(wbq_entry->कुञ्चिका.bit_nr, wbq_entry->कुञ्चिका.flags))
			ret = (*action)(&wbq_entry->कुञ्चिका, mode);
	} याव (test_bit_acquire(wbq_entry->कुञ्चिका.bit_nr, wbq_entry->कुञ्चिका.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	पटिनिवत्त ret;
}
EXPORT_चिह्नम्(__wait_on_bit);

पूर्णाङ्क __sched out_of_पङ्क्तिः_wait_on_bit(unsigned long *word, पूर्णाङ्क bit,
				    wait_bit_action_f *action, unsigned mode)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	पटिनिवत्त __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_चिह्नम्(out_of_पङ्क्तिः_wait_on_bit);

पूर्णाङ्क __sched out_of_पङ्क्तिः_wait_on_bit_timeout(
	unsigned long *word, पूर्णाङ्क bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.कुञ्चिका.timeout = jiffies + timeout;

	पटिनिवत्त __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_चिह्नम्_GPL(out_of_पङ्क्तिः_wait_on_bit_timeout);

पूर्णाङ्क __sched
__wait_on_bit_lock(संरचना wait_queue_head *wq_head, संरचना wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	पूर्णाङ्क ret = 0;

	अत्थाय (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		सचे (test_bit(wbq_entry->कुञ्चिका.bit_nr, wbq_entry->कुञ्चिका.flags)) {
			ret = action(&wbq_entry->कुञ्चिका, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			सचे (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		सचे (!test_and_set_bit(wbq_entry->कुञ्चिका.bit_nr, wbq_entry->कुञ्चिका.flags)) {
			सचे (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			पटिनिवत्त 0;
		} अञ्ञथा सचे (ret) {
			पटिनिवत्त ret;
		}
	}
}
EXPORT_चिह्नम्(__wait_on_bit_lock);

पूर्णाङ्क __sched out_of_पङ्क्तिः_wait_on_bit_lock(unsigned long *word, पूर्णाङ्क bit,
					 wait_bit_action_f *action, unsigned mode)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	पटिनिवत्त __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_चिह्नम्(out_of_पङ्क्तिः_wait_on_bit_lock);

शून्य __wake_up_bit(संरचना wait_queue_head *wq_head, unsigned long *word, पूर्णाङ्क bit)
{
	संरचना wait_bit_कुञ्चिका कुञ्चिका = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	सचे (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &कुञ्चिका);
}
EXPORT_चिह्नम्(__wake_up_bit);

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
शून्य wake_up_bit(unsigned long *word, पूर्णाङ्क bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_चिह्नम्(wake_up_bit);

wait_queue_head_t *__var_waitqueue(शून्य *p)
{
	पटिनिवत्त bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_चिह्नम्(__var_waitqueue);

संरचना wait_bit_कुञ्चिका *__var_wake_key(संरचना wait_queue_entry *wq_entry, शून्य *arg)
{
	संरचना wait_bit_कुञ्चिका *कुञ्चिका = arg;
	संरचना wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, संरचना wait_bit_queue_entry, wq_entry);

	सचे (wbq_entry->कुञ्चिका.flags != कुञ्चिका->flags ||
	    wbq_entry->कुञ्चिका.bit_nr != कुञ्चिका->bit_nr)
		पटिनिवत्त रिक्त;

	पटिनिवत्त कुञ्चिका;
}

स्थिर पूर्णाङ्क var_wake_function(संरचना wait_queue_entry *wq_entry, unsigned पूर्णाङ्क mode,
			     पूर्णाङ्क sync, शून्य *arg)
{
	संरचना wait_bit_कुञ्चिका *कुञ्चिका = __var_wake_key(wq_entry, arg);
	सचे (!कुञ्चिका)
		पटिनिवत्त 0;

	पटिनिवत्त autoremove_wake_function(wq_entry, mode, sync, कुञ्चिका);
}

शून्य init_wait_var_entry(संरचना wait_bit_queue_entry *wbq_entry, शून्य *var, पूर्णाङ्क flags)
{
	*wbq_entry = (संरचना wait_bit_queue_entry){
		.कुञ्चिका = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.गुप्त = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_चिह्नम्(init_wait_var_entry);

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
शून्य wake_up_var(शून्य *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_चिह्नम्(wake_up_var);

__sched पूर्णाङ्क bit_wait(संरचना wait_bit_कुञ्चिका *word, पूर्णाङ्क mode)
{
	schedule();
	सचे (signal_pending_state(mode, current))
		पटिनिवत्त -EINTR;

	पटिनिवत्त 0;
}
EXPORT_चिह्नम्(bit_wait);

__sched पूर्णाङ्क bit_wait_io(संरचना wait_bit_कुञ्चिका *word, पूर्णाङ्क mode)
{
	io_schedule();
	सचे (signal_pending_state(mode, current))
		पटिनिवत्त -EINTR;

	पटिनिवत्त 0;
}
EXPORT_चिह्नम्(bit_wait_io);

__sched पूर्णाङ्क bit_wait_timeout(संरचना wait_bit_कुञ्चिका *word, पूर्णाङ्क mode)
{
	unsigned long now = READ_ONCE(jiffies);

	सचे (time_after_eq(now, word->timeout))
		पटिनिवत्त -EAGAIN;
	schedule_timeout(word->timeout - now);
	सचे (signal_pending_state(mode, current))
		पटिनिवत्त -EINTR;

	पटिनिवत्त 0;
}
EXPORT_चिह्नम्_GPL(bit_wait_timeout);

शून्य __init wait_bit_init(शून्य)
{
	पूर्णाङ्क i;

	अत्थाय (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
