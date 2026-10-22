// SPDX-License-Identifier: GPL-2.0-only

#समावेश <linux/sched/debug.h>
#समावेश "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#घोषणा WAIT_TABLE_BITS 8
#घोषणा WAIT_TABLE_आकार (1 << WAIT_TABLE_BITS)

अटल wait_queue_शीर्ष_t bit_wait_table[WAIT_TABLE_आकार] __cacheline_aligned;

wait_queue_शीर्ष_t *bit_waitqueue(अचिन्हित दीर्घ *word, पूर्णांक bit)
{
	स्थिर पूर्णांक shift = BITS_PER_LONG == 32 ? 5 : 6;
	अचिन्हित दीर्घ val = (अचिन्हित दीर्घ)word << shift | bit;

	वापस bit_wait_table + hash_long(val, WAIT_TABLE_BITS);
}
EXPORT_प्रतीक(bit_waitqueue);

पूर्णांक wake_bit_function(काष्ठा wait_queue_entry *wq_entry, अचिन्हित mode, पूर्णांक sync, व्योम *arg)
{
	काष्ठा wait_bit_कुंजी *कुंजी = arg;
	काष्ठा wait_bit_queue_entry *wait_bit = container_of(wq_entry, काष्ठा wait_bit_queue_entry, wq_entry);

	अगर (wait_bit->कुंजी.flags != कुंजी->flags ||
			wait_bit->कुंजी.bit_nr != कुंजी->bit_nr ||
			test_bit(कुंजी->bit_nr, कुंजी->flags))
		वापस 0;

	वापस autoremove_wake_function(wq_entry, mode, sync, कुंजी);
}
EXPORT_प्रतीक(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
पूर्णांक __sched
__wait_on_bit(काष्ठा wait_queue_शीर्ष *wq_शीर्ष, काष्ठा wait_bit_queue_entry *wbq_entry,
	      wait_bit_action_f *action, अचिन्हित mode)
{
	पूर्णांक ret = 0;

	करो {
		prepare_to_wait(wq_शीर्ष, &wbq_entry->wq_entry, mode);
		अगर (test_bit(wbq_entry->कुंजी.bit_nr, wbq_entry->कुंजी.flags))
			ret = (*action)(&wbq_entry->कुंजी, mode);
	} जबतक (test_bit_acquire(wbq_entry->कुंजी.bit_nr, wbq_entry->कुंजी.flags) && !ret);

	finish_wait(wq_शीर्ष, &wbq_entry->wq_entry);

	वापस ret;
}
EXPORT_प्रतीक(__wait_on_bit);

पूर्णांक __sched out_of_पंक्ति_wait_on_bit(अचिन्हित दीर्घ *word, पूर्णांक bit,
				    wait_bit_action_f *action, अचिन्हित mode)
{
	काष्ठा wait_queue_शीर्ष *wq_शीर्ष = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	वापस __wait_on_bit(wq_शीर्ष, &wq_entry, action, mode);
}
EXPORT_प्रतीक(out_of_पंक्ति_wait_on_bit);

पूर्णांक __sched out_of_पंक्ति_wait_on_bit_timeout(
	अचिन्हित दीर्घ *word, पूर्णांक bit, wait_bit_action_f *action,
	अचिन्हित mode, अचिन्हित दीर्घ timeout)
{
	काष्ठा wait_queue_शीर्ष *wq_शीर्ष = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	wq_entry.कुंजी.timeout = jiffies + timeout;

	वापस __wait_on_bit(wq_शीर्ष, &wq_entry, action, mode);
}
EXPORT_प्रतीक_GPL(out_of_पंक्ति_wait_on_bit_timeout);

पूर्णांक __sched
__wait_on_bit_lock(काष्ठा wait_queue_शीर्ष *wq_शीर्ष, काष्ठा wait_bit_queue_entry *wbq_entry,
			wait_bit_action_f *action, अचिन्हित mode)
{
	पूर्णांक ret = 0;

	क्रम (;;) {
		prepare_to_wait_exclusive(wq_शीर्ष, &wbq_entry->wq_entry, mode);
		अगर (test_bit(wbq_entry->कुंजी.bit_nr, wbq_entry->कुंजी.flags)) {
			ret = action(&wbq_entry->कुंजी, mode);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			अगर (ret)
				finish_wait(wq_शीर्ष, &wbq_entry->wq_entry);
		}
		अगर (!test_and_set_bit(wbq_entry->कुंजी.bit_nr, wbq_entry->कुंजी.flags)) {
			अगर (!ret)
				finish_wait(wq_शीर्ष, &wbq_entry->wq_entry);
			वापस 0;
		} अन्यथा अगर (ret) {
			वापस ret;
		}
	}
}
EXPORT_प्रतीक(__wait_on_bit_lock);

पूर्णांक __sched out_of_पंक्ति_wait_on_bit_lock(अचिन्हित दीर्घ *word, पूर्णांक bit,
					 wait_bit_action_f *action, अचिन्हित mode)
{
	काष्ठा wait_queue_शीर्ष *wq_शीर्ष = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_entry, word, bit);

	वापस __wait_on_bit_lock(wq_शीर्ष, &wq_entry, action, mode);
}
EXPORT_प्रतीक(out_of_पंक्ति_wait_on_bit_lock);

व्योम __wake_up_bit(काष्ठा wait_queue_शीर्ष *wq_शीर्ष, अचिन्हित दीर्घ *word, पूर्णांक bit)
{
	काष्ठा wait_bit_कुंजी कुंजी = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	अगर (waitqueue_active(wq_शीर्ष))
		__wake_up(wq_शीर्ष, TASK_NORMAL, 1, &कुंजी);
}
EXPORT_प्रतीक(__wake_up_bit);

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
व्योम wake_up_bit(अचिन्हित दीर्घ *word, पूर्णांक bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_प्रतीक(wake_up_bit);

wait_queue_शीर्ष_t *__var_waitqueue(व्योम *p)
{
	वापस bit_wait_table + hash_ptr(p, WAIT_TABLE_BITS);
}
EXPORT_प्रतीक(__var_waitqueue);

काष्ठा wait_bit_कुंजी *__var_wake_key(काष्ठा wait_queue_entry *wq_entry, व्योम *arg)
{
	काष्ठा wait_bit_कुंजी *कुंजी = arg;
	काष्ठा wait_bit_queue_entry *wbq_entry =
		container_of(wq_entry, काष्ठा wait_bit_queue_entry, wq_entry);

	अगर (wbq_entry->कुंजी.flags != कुंजी->flags ||
	    wbq_entry->कुंजी.bit_nr != कुंजी->bit_nr)
		वापस शून्य;

	वापस कुंजी;
}

अटल पूर्णांक var_wake_function(काष्ठा wait_queue_entry *wq_entry, अचिन्हित पूर्णांक mode,
			     पूर्णांक sync, व्योम *arg)
{
	काष्ठा wait_bit_कुंजी *कुंजी = __var_wake_key(wq_entry, arg);
	अगर (!कुंजी)
		वापस 0;

	वापस autoremove_wake_function(wq_entry, mode, sync, कुंजी);
}

व्योम init_wait_var_entry(काष्ठा wait_bit_queue_entry *wbq_entry, व्योम *var, पूर्णांक flags)
{
	*wbq_entry = (काष्ठा wait_bit_queue_entry){
		.कुंजी = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_entry = {
			.flags	 = flags,
			.निजी = current,
			.func	 = var_wake_function,
			.entry	 = LIST_शीर्ष_INIT(wbq_entry->wq_entry.entry),
		},
	};
}
EXPORT_प्रतीक(init_wait_var_entry);

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
व्योम wake_up_var(व्योम *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_प्रतीक(wake_up_var);

__sched पूर्णांक bit_wait(काष्ठा wait_bit_कुंजी *word, पूर्णांक mode)
{
	schedule();
	अगर (signal_pending_state(mode, current))
		वापस -EINTR;

	वापस 0;
}
EXPORT_प्रतीक(bit_wait);

__sched पूर्णांक bit_wait_io(काष्ठा wait_bit_कुंजी *word, पूर्णांक mode)
{
	io_schedule();
	अगर (signal_pending_state(mode, current))
		वापस -EINTR;

	वापस 0;
}
EXPORT_प्रतीक(bit_wait_io);

__sched पूर्णांक bit_wait_timeout(काष्ठा wait_bit_कुंजी *word, पूर्णांक mode)
{
	अचिन्हित दीर्घ now = READ_ONCE(jiffies);

	अगर (time_after_eq(now, word->timeout))
		वापस -EAGAIN;
	schedule_timeout(word->timeout - now);
	अगर (signal_pending_state(mode, current))
		वापस -EINTR;

	वापस 0;
}
EXPORT_प्रतीक_GPL(bit_wait_timeout);

व्योम __init wait_bit_init(व्योम)
{
	पूर्णांक i;

	क्रम (i = 0; i < WAIT_TABLE_आकार; i++)
		init_waitqueue_शीर्ष(bit_wait_table + i);
}
