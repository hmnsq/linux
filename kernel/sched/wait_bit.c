// SPDX-License-Identifier: GPL-2.0-only

#अन्तर्भाव <linux/sched/debug.h>
#अन्तर्भाव "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

स्थिर wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(अचिन्हित दीर्घ *word, पूर्णांक bit)
{
	स्थिरांक पूर्णांक shift = BITS_PER_LONG == 32 ? 5 : 6;
	अचिन्हित दीर्घ val = (अचिन्हित दीर्घ)word << shift | bit;

	परत bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_चिन्ह(bit_waitqueue);

पूर्णांक wake_bit_function(संरचना wait_queue_entry *wq_entry, अचिन्हित mode, पूर्णांक sync, शून्य *arg)
{
	संरचना wait_bit_किल्ली *किल्ली = arg;
	संरचना wait_bit_queue_entry *wait_bit = container_of(wq_entry, संरचना wait_bit_queue_entry, wq_entry);

	जर (wait_bit->किल्ली.flags != किल्ली->flags ||
			wait_bit->किल्ली.bit_nr != किल्ली->bit_nr ||
			test_bit(किल्ली->bit_nr, किल्ली->flags))
		परत 0;

	परत autoremove_wake_function(wq_entry, mode, sync, किल्ली);
}
EXPORT_चिन्ह(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
पूर्णांक __sched
__wait_on_bit(संरचना wait_queue_head *wq_head, संरचना wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, अचिन्हित mode)
{
	पूर्णांक ret = 0;

	करा {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		जर (test_bit(wbq_entry->किल्ली.bit_nr, wbq_entry->किल्ली.flags))
			ret = (*action)(&wbq_entry->किल्ली, mode);
	} पर्यंत (test_bit_acquire(wbq_entry->किल्ली.bit_nr, wbq_entry->किल्ली.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	परत ret;
}
EXPORT_चिन्ह(__wait_on_bit);

पूर्णांक __sched out_of_ओळ_wait_on_bit(अचिन्हित दीर्घ *word, पूर्णांक bit,
				    wait_bit_action_f *action, अचिन्हित mode)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	परत __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_चिन्ह(out_of_ओळ_wait_on_bit);

पूर्णांक __sched out_of_ओळ_wait_on_bit_timeout(
	अचिन्हित दीर्घ *word, पूर्णांक bit, wait_bit_action_f *action,
	अचिन्हित mode, अचिन्हित दीर्घ timeout)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.किल्ली.timeout = jiffies + timeout;

	परत __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_चिन्ह_GPL(out_of_ओळ_wait_on_bit_timeout);

पूर्णांक __sched
__wait_on_bit_lock(संरचना wait_queue_head *wq_head, संरचना wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, अचिन्हित mode)
{
	पूर्णांक ret = 0;

	साठी (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		जर (test_bit(wbq_entry->किल्ली.bit_nr, wbq_entry->किल्ली.flags)) {
			ret = action(&wbq_entry->किल्ली, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			जर (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		जर (!test_and_set_bit(wbq_entry->किल्ली.bit_nr, wbq_entry->किल्ली.flags)) {
			जर (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			परत 0;
		} अन्यथा जर (ret) {
			परत ret;
		}
	}
}
EXPORT_चिन्ह(__wait_on_bit_lock);

पूर्णांक __sched out_of_ओळ_wait_on_bit_lock(अचिन्हित दीर्घ *word, पूर्णांक bit,
					 wait_bit_action_f *action, अचिन्हित mode)
{
	संरचना wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	परत __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_चिन्ह(out_of_ओळ_wait_on_bit_lock);

शून्य __wake_up_bit(संरचना wait_queue_head *wq_head, अचिन्हित दीर्घ *word, पूर्णांक bit)
{
	संरचना wait_bit_किल्ली किल्ली = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	जर (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &किल्ली);
}
EXPORT_चिन्ह(__wake_up_bit);

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
शून्य wake_up_bit(अचिन्हित दीर्घ *word, पूर्णांक bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_चिन्ह(wake_up_bit);

wait_queue_head_t *__var_waitqueue(शून्य *p)
{
	परत bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_चिन्ह(__var_waitqueue);

संरचना wait_bit_किल्ली *__var_wake_key(संरचना wait_queue_entry *wq_entry, शून्य *arg)
{
	संरचना wait_bit_किल्ली *किल्ली = arg;
	संरचना wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, संरचना wait_bit_queue_entry, wq_entry);

	जर (wbq_entry->किल्ली.flags != किल्ली->flags ||
	    wbq_entry->किल्ली.bit_nr != किल्ली->bit_nr)
		परत रिक्त;

	परत किल्ली;
}

स्थिर पूर्णांक var_wake_function(संरचना wait_queue_entry *wq_entry, अचिन्हित पूर्णांक mode,
			     पूर्णांक sync, शून्य *arg)
{
	संरचना wait_bit_किल्ली *किल्ली = __var_wake_key(wq_entry, arg);
	जर (!किल्ली)
		परत 0;

	परत autoremove_wake_function(wq_entry, mode, sync, किल्ली);
}

शून्य init_wait_var_entry(संरचना wait_bit_queue_entry *wbq_entry, शून्य *var, पूर्णांक flags)
{
	*wbq_entry = (संरचना wait_bit_queue_entry){
		.किल्ली = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.खाजगी = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_चिन्ह(init_wait_var_entry);

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
EXPORT_चिन्ह(wake_up_var);

__sched पूर्णांक bit_wait(संरचना wait_bit_किल्ली *word, पूर्णांक mode)
{
	schedule();
	जर (signal_pending_state(mode, current))
		परत -EINTR;

	परत 0;
}
EXPORT_चिन्ह(bit_wait);

__sched पूर्णांक bit_wait_io(संरचना wait_bit_किल्ली *word, पूर्णांक mode)
{
	io_schedule();
	जर (signal_pending_state(mode, current))
		परत -EINTR;

	परत 0;
}
EXPORT_चिन्ह(bit_wait_io);

__sched पूर्णांक bit_wait_timeout(संरचना wait_bit_किल्ली *word, पूर्णांक mode)
{
	अचिन्हित दीर्घ now = READ_ONCE(jiffies);

	जर (time_after_eq(now, word->timeout))
		परत -EAGAIN;
	schedule_timeout(word->timeout - now);
	जर (signal_pending_state(mode, current))
		परत -EINTR;

	परत 0;
}
EXPORT_चिन्ह_GPL(bit_wait_timeout);

शून्य __init wait_bit_init(शून्य)
{
	पूर्णांक i;

	साठी (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
