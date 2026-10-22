// SPDX-License-Identifier: GPL-2.0-only

#अन्तर्भाव <linux/sched/debug.h>
#अन्तर्भाव "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

स्थिर wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(अचिन्हित दीर्घ *word, पूर्णाङ्क bit)
{
	अचर पूर्णाङ्क shift = BITS_PER_LONG == 32 ? 5 : 6;
	अचिन्हित दीर्घ val = (अचिन्हित दीर्घ)word << shift | bit;

	फर्काऊ bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_चिह्न(bit_waitqueue);

पूर्णाङ्क wake_bit_function(संरचना wait_queue_entry *wq_entry, अचिन्हित mode, पूर्णाङ्क sync, शून्य *arg)
{
	संरचना wait_bit_साँचो *साँचो = arg;
	संरचना wait_bit_queue_entry *wait_bit = container_of(wq_entry, संरचना wait_bit_queue_entry, wq_entry);

	यदि (wait_bit->साँचो.flags != साँचो->flags ||
			wait_bit->साँचो.bit_nr != साँचो->bit_nr ||
			test_bit(साँचो->bit_nr, साँचो->flags))
		फर्काऊ 0;

	फर्काऊ autoremove_wake_function(wq_entry, mode, sync, साँचो);
}
EXPORT_चिह्न(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
पूर्णाङ्क __sched
__wait_on_bit(संरचना wait_queue_head *wq_head, संरचना wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, अचिन्हित mode)
{
	पूर्णाङ्क ret = 0;

	गर {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		यदि (test_bit(wbq_entry->साँचो.bit_nr, wbq_entry->साँचो.flags))
			ret = (*action)(&wbq_entry->साँचो, mode);
	} जबसम्म (test_bit_acquire(wbq_entry->साँचो.bit_nr, wbq_entry->साँचो.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	फर्काऊ ret;
}
EXPORT_चिह्न(__wait_on_bit);

पूर्णाङ्क __sched out_of_पङ्क्ति_wait_on_bit(अचिन्हित दीर्घ *word, पूर्णाङ्क bit,
				    wait_bit_action_f *action, अचिन्हित mode)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	फर्काऊ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_चिह्न(out_of_पङ्क्ति_wait_on_bit);

पूर्णाङ्क __sched out_of_पङ्क्ति_wait_on_bit_timeout(
	अचिन्हित दीर्घ *word, पूर्णाङ्क bit, wait_bit_action_f *action,
	अचिन्हित mode, अचिन्हित दीर्घ timeout)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.साँचो.timeout = jiffies + timeout;

	फर्काऊ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_चिह्न_GPL(out_of_पङ्क्ति_wait_on_bit_timeout);

पूर्णाङ्क __sched
__wait_on_bit_lock(संरचना wait_queue_head *wq_head, संरचना wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, अचिन्हित mode)
{
	पूर्णाङ्क ret = 0;

	लागि (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		यदि (test_bit(wbq_entry->साँचो.bit_nr, wbq_entry->साँचो.flags)) {
			ret = action(&wbq_entry->साँचो, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			यदि (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		यदि (!test_and_set_bit(wbq_entry->साँचो.bit_nr, wbq_entry->साँचो.flags)) {
			यदि (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			फर्काऊ 0;
		} अन्यथा यदि (ret) {
			फर्काऊ ret;
		}
	}
}
EXPORT_चिह्न(__wait_on_bit_lock);

पूर्णाङ्क __sched out_of_पङ्क्ति_wait_on_bit_lock(अचिन्हित दीर्घ *word, पूर्णाङ्क bit,
					 wait_bit_action_f *action, अचिन्हित mode)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	फर्काऊ __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_चिह्न(out_of_पङ्क्ति_wait_on_bit_lock);

शून्य __wake_up_bit(संरचना wait_queue_head *wq_head, अचिन्हित दीर्घ *word, पूर्णाङ्क bit)
{
	संरचना wait_bit_साँचो साँचो = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	यदि (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &साँचो);
}
EXPORT_चिह्न(__wake_up_bit);

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
शून्य wake_up_bit(अचिन्हित दीर्घ *word, पूर्णाङ्क bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_चिह्न(wake_up_bit);

wait_queue_head_t *__var_waitqueue(शून्य *p)
{
	फर्काऊ bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_चिह्न(__var_waitqueue);

संरचना wait_bit_साँचो *__var_wake_key(संरचना wait_queue_entry *wq_entry, शून्य *arg)
{
	संरचना wait_bit_साँचो *साँचो = arg;
	संरचना wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, संरचना wait_bit_queue_entry, wq_entry);

	यदि (wbq_entry->साँचो.flags != साँचो->flags ||
	    wbq_entry->साँचो.bit_nr != साँचो->bit_nr)
		फर्काऊ रिक्त;

	फर्काऊ साँचो;
}

स्थिर पूर्णाङ्क var_wake_function(संरचना wait_queue_entry *wq_entry, अचिन्हित पूर्णाङ्क mode,
			     पूर्णाङ्क sync, शून्य *arg)
{
	संरचना wait_bit_साँचो *साँचो = __var_wake_key(wq_entry, arg);
	यदि (!साँचो)
		फर्काऊ 0;

	फर्काऊ autoremove_wake_function(wq_entry, mode, sync, साँचो);
}

शून्य init_wait_var_entry(संरचना wait_bit_queue_entry *wbq_entry, शून्य *var, पूर्णाङ्क flags)
{
	*wbq_entry = (संरचना wait_bit_queue_entry){
		.साँचो = {
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
EXPORT_चिह्न(init_wait_var_entry);

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
EXPORT_चिह्न(wake_up_var);

__sched पूर्णाङ्क bit_wait(संरचना wait_bit_साँचो *word, पूर्णाङ्क mode)
{
	schedule();
	यदि (signal_pending_state(mode, current))
		फर्काऊ -EINTR;

	फर्काऊ 0;
}
EXPORT_चिह्न(bit_wait);

__sched पूर्णाङ्क bit_wait_io(संरचना wait_bit_साँचो *word, पूर्णाङ्क mode)
{
	io_schedule();
	यदि (signal_pending_state(mode, current))
		फर्काऊ -EINTR;

	फर्काऊ 0;
}
EXPORT_चिह्न(bit_wait_io);

__sched पूर्णाङ्क bit_wait_timeout(संरचना wait_bit_साँचो *word, पूर्णाङ्क mode)
{
	अचिन्हित दीर्घ now = READ_ONCE(jiffies);

	यदि (time_after_eq(now, word->timeout))
		फर्काऊ -EAGAIN;
	schedule_timeout(word->timeout - now);
	यदि (signal_pending_state(mode, current))
		फर्काऊ -EINTR;

	फर्काऊ 0;
}
EXPORT_चिह्न_GPL(bit_wait_timeout);

शून्य __init wait_bit_init(शून्य)
{
	पूर्णाङ्क i;

	लागि (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
