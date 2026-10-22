// SPDX-License-Identifier: GPL-2.0-only

#અન્તર્ભાવ <linux/sched/debug.h>
#અન્તર્ભાવ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

સ્થિર wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, પૂર્ણાંક bit)
{
	અચલ પૂર્ણાંક shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	પાછા bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_પ્રતીક(bit_waitqueue);

પૂર્ણાંક wake_bit_function(સંરચના wait_queue_entry *wq_entry, unsigned mode, પૂર્ણાંક sync, શૂન્ય *arg)
{
	સંરચના wait_bit_ચાવી *ચાવી = arg;
	સંરચના wait_bit_queue_entry *wait_bit = container_of(wq_entry, સંરચના wait_bit_queue_entry, wq_entry);

	જો (wait_bit->ચાવી.flags != ચાવી->flags ||
			wait_bit->ચાવી.bit_nr != ચાવી->bit_nr ||
			test_bit(ચાવી->bit_nr, ચાવી->flags))
		પાછા 0;

	પાછા autoremove_wake_function(wq_entry, mode, sync, ચાવી);
}
EXPORT_પ્રતીક(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
પૂર્ણાંક __sched
__wait_on_bit(સંરચના wait_queue_head *wq_head, સંરચના wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	પૂર્ણાંક ret = 0;

	કરો {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		જો (test_bit(wbq_entry->ચાવી.bit_nr, wbq_entry->ચાવી.flags))
			ret = (*action)(&wbq_entry->ચાવી, mode);
	} જ્યારે (test_bit_acquire(wbq_entry->ચાવી.bit_nr, wbq_entry->ચાવી.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	પાછા ret;
}
EXPORT_પ્રતીક(__wait_on_bit);

પૂર્ણાંક __sched out_of_લીટી_wait_on_bit(unsigned long *word, પૂર્ણાંક bit,
				    wait_bit_action_f *action, unsigned mode)
{
	સંરચના wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	પાછા __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_પ્રતીક(out_of_લીટી_wait_on_bit);

પૂર્ણાંક __sched out_of_લીટી_wait_on_bit_timeout(
	unsigned long *word, પૂર્ણાંક bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	સંરચના wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.ચાવી.timeout = jiffies + timeout;

	પાછા __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_પ્રતીક_GPL(out_of_લીટી_wait_on_bit_timeout);

પૂર્ણાંક __sched
__wait_on_bit_lock(સંરચના wait_queue_head *wq_head, સંરચના wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	પૂર્ણાંક ret = 0;

	માટે (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		જો (test_bit(wbq_entry->ચાવી.bit_nr, wbq_entry->ચાવી.flags)) {
			ret = action(&wbq_entry->ચાવી, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			જો (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		જો (!test_and_set_bit(wbq_entry->ચાવી.bit_nr, wbq_entry->ચાવી.flags)) {
			જો (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			પાછા 0;
		} નહીંતર જો (ret) {
			પાછા ret;
		}
	}
}
EXPORT_પ્રતીક(__wait_on_bit_lock);

પૂર્ણાંક __sched out_of_લીટી_wait_on_bit_lock(unsigned long *word, પૂર્ણાંક bit,
					 wait_bit_action_f *action, unsigned mode)
{
	સંરચના wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	પાછા __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_પ્રતીક(out_of_લીટી_wait_on_bit_lock);

શૂન્ય __wake_up_bit(સંરચના wait_queue_head *wq_head, unsigned long *word, પૂર્ણાંક bit)
{
	સંરચના wait_bit_ચાવી ચાવી = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	જો (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &ચાવી);
}
EXPORT_પ્રતીક(__wake_up_bit);

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
શૂન્ય wake_up_bit(unsigned long *word, પૂર્ણાંક bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_પ્રતીક(wake_up_bit);

wait_queue_head_t *__var_waitqueue(શૂન્ય *p)
{
	પાછા bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_પ્રતીક(__var_waitqueue);

સંરચના wait_bit_ચાવી *__var_wake_key(સંરચના wait_queue_entry *wq_entry, શૂન્ય *arg)
{
	સંરચના wait_bit_ચાવી *ચાવી = arg;
	સંરચના wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, સંરચના wait_bit_queue_entry, wq_entry);

	જો (wbq_entry->ચાવી.flags != ચાવી->flags ||
	    wbq_entry->ચાવી.bit_nr != ચાવી->bit_nr)
		પાછા ખાલી;

	પાછા ચાવી;
}

સ્થિર પૂર્ણાંક var_wake_function(સંરચના wait_queue_entry *wq_entry, unsigned પૂર્ણાંક mode,
			     પૂર્ણાંક sync, શૂન્ય *arg)
{
	સંરચના wait_bit_ચાવી *ચાવી = __var_wake_key(wq_entry, arg);
	જો (!ચાવી)
		પાછા 0;

	પાછા autoremove_wake_function(wq_entry, mode, sync, ચાવી);
}

શૂન્ય init_wait_var_entry(સંરચના wait_bit_queue_entry *wbq_entry, શૂન્ય *var, પૂર્ણાંક flags)
{
	*wbq_entry = (સંરચના wait_bit_queue_entry){
		.ચાવી = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.ખાનગી = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_પ્રતીક(init_wait_var_entry);

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
શૂન્ય wake_up_var(શૂન્ય *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_પ્રતીક(wake_up_var);

__sched પૂર્ણાંક bit_wait(સંરચના wait_bit_ચાવી *word, પૂર્ણાંક mode)
{
	schedule();
	જો (signal_pending_state(mode, current))
		પાછા -EINTR;

	પાછા 0;
}
EXPORT_પ્રતીક(bit_wait);

__sched પૂર્ણાંક bit_wait_io(સંરચના wait_bit_ચાવી *word, પૂર્ણાંક mode)
{
	io_schedule();
	જો (signal_pending_state(mode, current))
		પાછા -EINTR;

	પાછા 0;
}
EXPORT_પ્રતીક(bit_wait_io);

__sched પૂર્ણાંક bit_wait_timeout(સંરચના wait_bit_ચાવી *word, પૂર્ણાંક mode)
{
	unsigned long now = READ_ONCE(jiffies);

	જો (time_after_eq(now, word->timeout))
		પાછા -EAGAIN;
	schedule_timeout(word->timeout - now);
	જો (signal_pending_state(mode, current))
		પાછા -EINTR;

	પાછા 0;
}
EXPORT_પ્રતીક_GPL(bit_wait_timeout);

શૂન્ય __init wait_bit_init(શૂન્ય)
{
	પૂર્ણાંક i;

	માટે (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
