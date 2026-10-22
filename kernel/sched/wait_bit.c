// SPDX-License-Identifier: GPL-2.0-only

#അന്തര്ഭാവ <linux/sched/debug.h>
#അന്തര്ഭാവ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

സ്ഥിതം wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, പൂർണ്ണസംഖ്യ bit)
{
	സ്ഥിരം പൂർണ്ണസംഖ്യ shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	തിരികെ bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_ചിഹ്നം(bit_waitqueue);

പൂർണ്ണസംഖ്യ wake_bit_function(ഘടന wait_queue_entry *wq_entry, unsigned mode, പൂർണ്ണസംഖ്യ sync, ശൂന്യം *arg)
{
	ഘടന wait_bit_താക്കോൽ *താക്കോൽ = arg;
	ഘടന wait_bit_queue_entry *wait_bit = container_of(wq_entry, ഘടന wait_bit_queue_entry, wq_entry);

	എങ്കിൽ (wait_bit->താക്കോൽ.flags != താക്കോൽ->flags ||
			wait_bit->താക്കോൽ.bit_nr != താക്കോൽ->bit_nr ||
			test_bit(താക്കോൽ->bit_nr, താക്കോൽ->flags))
		തിരികെ 0;

	തിരികെ autoremove_wake_function(wq_entry, mode, sync, താക്കോൽ);
}
EXPORT_ചിഹ്നം(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
പൂർണ്ണസംഖ്യ __sched
__wait_on_bit(ഘടന wait_queue_head *wq_head, ഘടന wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	പൂർണ്ണസംഖ്യ ret = 0;

	ചെയ്യുക {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		എങ്കിൽ (test_bit(wbq_entry->താക്കോൽ.bit_nr, wbq_entry->താക്കോൽ.flags))
			ret = (*action)(&wbq_entry->താക്കോൽ, mode);
	} ആകുമ്പോൾ (test_bit_acquire(wbq_entry->താക്കോൽ.bit_nr, wbq_entry->താക്കോൽ.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	തിരികെ ret;
}
EXPORT_ചിഹ്നം(__wait_on_bit);

പൂർണ്ണസംഖ്യ __sched out_of_വരി_wait_on_bit(unsigned long *word, പൂർണ്ണസംഖ്യ bit,
				    wait_bit_action_f *action, unsigned mode)
{
	ഘടന wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	തിരികെ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_ചിഹ്നം(out_of_വരി_wait_on_bit);

പൂർണ്ണസംഖ്യ __sched out_of_വരി_wait_on_bit_timeout(
	unsigned long *word, പൂർണ്ണസംഖ്യ bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	ഘടന wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.താക്കോൽ.timeout = jiffies + timeout;

	തിരികെ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_ചിഹ്നം_GPL(out_of_വരി_wait_on_bit_timeout);

പൂർണ്ണസംഖ്യ __sched
__wait_on_bit_lock(ഘടന wait_queue_head *wq_head, ഘടന wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	പൂർണ്ണസംഖ്യ ret = 0;

	വേണ്ടി (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		എങ്കിൽ (test_bit(wbq_entry->താക്കോൽ.bit_nr, wbq_entry->താക്കോൽ.flags)) {
			ret = action(&wbq_entry->താക്കോൽ, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			എങ്കിൽ (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		എങ്കിൽ (!test_and_set_bit(wbq_entry->താക്കോൽ.bit_nr, wbq_entry->താക്കോൽ.flags)) {
			എങ്കിൽ (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			തിരികെ 0;
		} അല്ലെങ്കിൽ എങ്കിൽ (ret) {
			തിരികെ ret;
		}
	}
}
EXPORT_ചിഹ്നം(__wait_on_bit_lock);

പൂർണ്ണസംഖ്യ __sched out_of_വരി_wait_on_bit_lock(unsigned long *word, പൂർണ്ണസംഖ്യ bit,
					 wait_bit_action_f *action, unsigned mode)
{
	ഘടന wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	തിരികെ __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_ചിഹ്നം(out_of_വരി_wait_on_bit_lock);

ശൂന്യം __wake_up_bit(ഘടന wait_queue_head *wq_head, unsigned long *word, പൂർണ്ണസംഖ്യ bit)
{
	ഘടന wait_bit_താക്കോൽ താക്കോൽ = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	എങ്കിൽ (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &താക്കോൽ);
}
EXPORT_ചിഹ്നം(__wake_up_bit);

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
ശൂന്യം wake_up_bit(unsigned long *word, പൂർണ്ണസംഖ്യ bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_ചിഹ്നം(wake_up_bit);

wait_queue_head_t *__var_waitqueue(ശൂന്യം *p)
{
	തിരികെ bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_ചിഹ്നം(__var_waitqueue);

ഘടന wait_bit_താക്കോൽ *__var_wake_key(ഘടന wait_queue_entry *wq_entry, ശൂന്യം *arg)
{
	ഘടന wait_bit_താക്കോൽ *താക്കോൽ = arg;
	ഘടന wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, ഘടന wait_bit_queue_entry, wq_entry);

	എങ്കിൽ (wbq_entry->താക്കോൽ.flags != താക്കോൽ->flags ||
	    wbq_entry->താക്കോൽ.bit_nr != താക്കോൽ->bit_nr)
		തിരികെ ശൂന്യ;

	തിരികെ താക്കോൽ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ var_wake_function(ഘടന wait_queue_entry *wq_entry, unsigned പൂർണ്ണസംഖ്യ mode,
			     പൂർണ്ണസംഖ്യ sync, ശൂന്യം *arg)
{
	ഘടന wait_bit_താക്കോൽ *താക്കോൽ = __var_wake_key(wq_entry, arg);
	എങ്കിൽ (!താക്കോൽ)
		തിരികെ 0;

	തിരികെ autoremove_wake_function(wq_entry, mode, sync, താക്കോൽ);
}

ശൂന്യം init_wait_var_entry(ഘടന wait_bit_queue_entry *wbq_entry, ശൂന്യം *var, പൂർണ്ണസംഖ്യ flags)
{
	*wbq_entry = (ഘടന wait_bit_queue_entry){
		.താക്കോൽ = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.സ്വകാര്യ = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_ചിഹ്നം(init_wait_var_entry);

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
ശൂന്യം wake_up_var(ശൂന്യം *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_ചിഹ്നം(wake_up_var);

__sched പൂർണ്ണസംഖ്യ bit_wait(ഘടന wait_bit_താക്കോൽ *word, പൂർണ്ണസംഖ്യ mode)
{
	schedule();
	എങ്കിൽ (signal_pending_state(mode, current))
		തിരികെ -EINTR;

	തിരികെ 0;
}
EXPORT_ചിഹ്നം(bit_wait);

__sched പൂർണ്ണസംഖ്യ bit_wait_io(ഘടന wait_bit_താക്കോൽ *word, പൂർണ്ണസംഖ്യ mode)
{
	io_schedule();
	എങ്കിൽ (signal_pending_state(mode, current))
		തിരികെ -EINTR;

	തിരികെ 0;
}
EXPORT_ചിഹ്നം(bit_wait_io);

__sched പൂർണ്ണസംഖ്യ bit_wait_timeout(ഘടന wait_bit_താക്കോൽ *word, പൂർണ്ണസംഖ്യ mode)
{
	unsigned long now = READ_ONCE(jiffies);

	എങ്കിൽ (time_after_eq(now, word->timeout))
		തിരികെ -EAGAIN;
	schedule_timeout(word->timeout - now);
	എങ്കിൽ (signal_pending_state(mode, current))
		തിരികെ -EINTR;

	തിരികെ 0;
}
EXPORT_ചിഹ്നം_GPL(bit_wait_timeout);

ശൂന്യം __init wait_bit_init(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
