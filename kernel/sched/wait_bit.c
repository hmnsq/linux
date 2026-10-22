// SPDX-License-Identifier: GPL-2.0-only

#ଅନ୍ତର୍ଭାଵ <linux/sched/debug.h>
#ଅନ୍ତର୍ଭାଵ "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#define WAIT_TABLE_BITS 8
#define WAIT_TABLE_SIZE (1 << WAIT_TABLE_BITS)

ସ୍ଥିର wait_queue_head_t bit_wait_table[WAIT_TABLE_SIZE] __cacheline_aligned;

wait_queue_head_t *bit_waitqueue(unsigned long *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit)
{
	ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift = BITS_PER_LONG == 32 ? 5 : 6;
	unsigned long val = (unsigned long)word << shift | bit;

	ଫେରନ୍ତୁ bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_ପ୍ରତୀକ(bit_waitqueue);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_bit_function(ସଂରଚନା wait_queue_entry *wq_entry, unsigned mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync, ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା wait_bit_ଚାବି *ଚାବି = arg;
	ସଂରଚନା wait_bit_queue_entry *wait_bit = container_of(wq_entry, ସଂରଚନା wait_bit_queue_entry, wq_entry);

	ଯଦି (wait_bit->ଚାବି.flags != ଚାବି->flags ||
			wait_bit->ଚାବି.bit_nr != ଚାବି->bit_nr ||
			test_bit(ଚାବି->bit_nr, ଚାବି->flags))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ autoremove_wake_function(wq_entry, mode, sync, ଚାବି);
}
EXPORT_ପ୍ରତୀକ(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched
__wait_on_bit(ସଂରଚନା wait_queue_head *wq_head, ସଂରଚନା wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, unsigned mode)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	କର {
		prepare_to_wait(wq_head, &wbq_entry->wq_entry, mode);
		ଯଦି (test_bit(wbq_entry->ଚାବି.bit_nr, wbq_entry->ଚାବି.flags))
			ret = (*action)(&wbq_entry->ଚାବି, mode);
	} ଯେତେବେଳେ (test_bit_acquire(wbq_entry->ଚାବି.bit_nr, wbq_entry->ଚାବି.flags) && !ret);

	finish_wait(wq_head, &wbq_entry->wq_entry);

	ଫେରନ୍ତୁ ret;
}
EXPORT_ପ୍ରତୀକ(__wait_on_bit);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched out_of_ଧାଡ଼ି_wait_on_bit(unsigned long *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit,
				    wait_bit_action_f *action, unsigned mode)
{
	ସଂରଚନା wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	ଫେରନ୍ତୁ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_ପ୍ରତୀକ(out_of_ଧାଡ଼ି_wait_on_bit);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched out_of_ଧାଡ଼ି_wait_on_bit_timeout(
	unsigned long *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit, wait_bit_action_f *action,
	unsigned mode, unsigned long timeout)
{
	ସଂରଚନା wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.ଚାବି.timeout = jiffies + timeout;

	ଫେରନ୍ତୁ __wait_on_bit(wq_head, &wq_entry, action, mode);
}
EXPORT_ପ୍ରତୀକ_GPL(out_of_ଧାଡ଼ି_wait_on_bit_timeout);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched
__wait_on_bit_lock(ସଂରଚନା wait_queue_head *wq_head, ସଂରଚନା wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, unsigned mode)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ପାଇଁ (;;) {
		prepare_to_wait_exclusive(wq_head, &wbq_entry->wq_entry, mode);
		ଯଦି (test_bit(wbq_entry->ଚାବି.bit_nr, wbq_entry->ଚାବି.flags)) {
			ret = action(&wbq_entry->ଚାବି, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			ଯଦି (ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
		}
		ଯଦି (!test_and_set_bit(wbq_entry->ଚାବି.bit_nr, wbq_entry->ଚାବି.flags)) {
			ଯଦି (!ret)
				finish_wait(wq_head, &wbq_entry->wq_entry);
			ଫେରନ୍ତୁ 0;
		} ଅନ୍ୟଥା ଯଦି (ret) {
			ଫେରନ୍ତୁ ret;
		}
	}
}
EXPORT_ପ୍ରତୀକ(__wait_on_bit_lock);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched out_of_ଧାଡ଼ି_wait_on_bit_lock(unsigned long *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit,
					 wait_bit_action_f *action, unsigned mode)
{
	ସଂରଚନା wait_queue_head *wq_head = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	ଫେରନ୍ତୁ __wait_on_bit_lock(wq_head, &wq_entry, action, mode);
}
EXPORT_ପ୍ରତୀକ(out_of_ଧାଡ଼ି_wait_on_bit_lock);

ଶୂନ୍ୟ __wake_up_bit(ସଂରଚନା wait_queue_head *wq_head, unsigned long *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit)
{
	ସଂରଚନା wait_bit_ଚାବି ଚାବି = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	ଯଦି (waitqueue_active(wq_head))
		__wake_up(wq_head, TASK_NORMAL, 1, &ଚାବି);
}
EXPORT_ପ୍ରତୀକ(__wake_up_bit);

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
ଶୂନ୍ୟ wake_up_bit(unsigned long *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_ପ୍ରତୀକ(wake_up_bit);

wait_queue_head_t *__var_waitqueue(ଶୂନ୍ୟ *p)
{
	ଫେରନ୍ତୁ bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_ପ୍ରତୀକ(__var_waitqueue);

ସଂରଚନା wait_bit_ଚାବି *__var_wake_key(ସଂରଚନା wait_queue_entry *wq_entry, ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା wait_bit_ଚାବି *ଚାବି = arg;
	ସଂରଚନା wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, ସଂରଚନା wait_bit_queue_entry, wq_entry);

	ଯଦି (wbq_entry->ଚାବି.flags != ଚାବି->flags ||
	    wbq_entry->ଚାବି.bit_nr != ଚାବି->bit_nr)
		ଫେରନ୍ତୁ ଖାଲି;

	ଫେରନ୍ତୁ ଚାବି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା var_wake_function(ସଂରଚନା wait_queue_entry *wq_entry, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync, ଶୂନ୍ୟ *arg)
{
	ସଂରଚନା wait_bit_ଚାବି *ଚାବି = __var_wake_key(wq_entry, arg);
	ଯଦି (!ଚାବି)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ autoremove_wake_function(wq_entry, mode, sync, ଚାବି);
}

ଶୂନ୍ୟ init_wait_var_entry(ସଂରଚନା wait_bit_queue_entry *wbq_entry, ଶୂନ୍ୟ *var, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	*wbq_entry = (ସଂରଚନା wait_bit_queue_entry){
		.ଚାବି = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.ବ୍ୟକ୍ତିଗତ = current,
			.func	 = var_wake_function,
			.entry	 = LIST_HEAD_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_ପ୍ରତୀକ(init_wait_var_entry);

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
ଶୂନ୍ୟ wake_up_var(ଶୂନ୍ୟ *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_ପ୍ରତୀକ(wake_up_var);

__sched ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_wait(ସଂରଚନା wait_bit_ଚାବି *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	schedule();
	ଯଦି (signal_pending_state(mode, current))
		ଫେରନ୍ତୁ -EINTR;

	ଫେରନ୍ତୁ 0;
}
EXPORT_ପ୍ରତୀକ(bit_wait);

__sched ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_wait_io(ସଂରଚନା wait_bit_ଚାବି *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	io_schedule();
	ଯଦି (signal_pending_state(mode, current))
		ଫେରନ୍ତୁ -EINTR;

	ଫେରନ୍ତୁ 0;
}
EXPORT_ପ୍ରତୀକ(bit_wait_io);

__sched ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_wait_timeout(ସଂରଚନା wait_bit_ଚାବି *word, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	unsigned long now = READ_ONCE(jiffies);

	ଯଦି (time_after_eq(now, word->timeout))
		ଫେରନ୍ତୁ -EAGAIN;
	schedule_timeout(word->timeout - now);
	ଯଦି (signal_pending_state(mode, current))
		ଫେରନ୍ତୁ -EINTR;

	ଫେରନ୍ତୁ 0;
}
EXPORT_ପ୍ରତୀକ_GPL(bit_wait_timeout);

ଶୂନ୍ୟ __init wait_bit_init(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < WAIT_TABLE_SIZE; i++)
		init_waitqueue_head(bit_wait_table + i);
}
