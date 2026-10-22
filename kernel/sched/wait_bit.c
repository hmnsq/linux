// SPDX-License-Identifier: GPL-2.0-only

#अन्तर्भाव <linux/sched/debug.h>
#अन्तर्भाव "sched.h"

/*
 * The implementation of the wait_bit*() and related waiting APIs:
 */

#परिभाषय WAIT_सारणी_द्वयङ्काः 8
#परिभाषय WAIT_सारणी_आकार (1 << WAIT_सारणी_द्वयङ्काः)

स्थिर wait_queue_शीर्ष_t bit_wait_सारणी[WAIT_सारणी_आकार] __cacheline_aligned;

wait_queue_शीर्ष_t *bit_waitqueue(अचिह्नित दीर्घ *word, पूर्णाङ्क bit)
{
	ध्रुव पूर्णाङ्क सरणम् = द्वयङ्काः_प्रत्येकम्_LONG == 32 ? 5 : 6;
	अचिह्नित दीर्घ val = (अचिह्नित दीर्घ)word << सरणम् | bit;

	प्रत्यागम bit_wait_सारणी + hash_long(val, WAIT_सारणी_द्वयङ्काः);
}
EXPORT_चिह्नम्(bit_waitqueue);

पूर्णाङ्क wake_bit_function(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, अचिह्नित बहुलकम्, पूर्णाङ्क sync, शून्य *arg)
{
	संरचना wait_bit_कुञ्चिका *कुञ्चिका = arg;
	संरचना wait_bit_queue_प्रविष्टिः *wait_bit = container_सम्बन्धि(wq_प्रविष्टिः, संरचना wait_bit_queue_प्रविष्टिः, wq_प्रविष्टिः);

	यदि (wait_bit->कुञ्चिका.flags != कुञ्चिका->flags ||
			wait_bit->कुञ्चिका.bit_nr != कुञ्चिका->bit_nr ||
			test_bit(कुञ्चिका->bit_nr, कुञ्चिका->flags))
		प्रत्यागम 0;

	प्रत्यागम autoremove_wake_function(wq_प्रविष्टिः, बहुलकम्, sync, कुञ्चिका);
}
EXPORT_चिह्नम्(wake_bit_function);

/*
 * To allow interruptible waiting and asynchronous (i.e. non-blocking)
 * waiting, the actions of __wait_on_bit() and __wait_on_bit_lock() are
 * permitted return codes. Nonzero return codes halt waiting and return.
 */
पूर्णाङ्क __sched
__wait_on_bit(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_bit_queue_प्रविष्टिः *wbq_प्रविष्टिः,
	      wait_bit_action_f *action, अचिह्नित बहुलकम्)
{
	पूर्णाङ्क ret = 0;

	कुरु {
		prepare_गन्तव्यम्_wait(wq_शीर्ष, &wbq_प्रविष्टिः->wq_प्रविष्टिः, बहुलकम्);
		यदि (test_bit(wbq_प्रविष्टिः->कुञ्चिका.bit_nr, wbq_प्रविष्टिः->कुञ्चिका.flags))
			ret = (*action)(&wbq_प्रविष्टिः->कुञ्चिका, बहुलकम्);
	} यावत् (test_bit_acquire(wbq_प्रविष्टिः->कुञ्चिका.bit_nr, wbq_प्रविष्टिः->कुञ्चिका.flags) && !ret);

	finish_wait(wq_शीर्ष, &wbq_प्रविष्टिः->wq_प्रविष्टिः);

	प्रत्यागम ret;
}
EXPORT_चिह्नम्(__wait_on_bit);

पूर्णाङ्क __sched बहिर्गमनम्_सम्बन्धि_पङ्क्तिः_wait_on_bit(अचिह्नित दीर्घ *word, पूर्णाङ्क bit,
				    wait_bit_action_f *action, अचिह्नित बहुलकम्)
{
	संरचना wait_queue_शीर्ष *wq_शीर्ष = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_प्रविष्टिः, word, bit);

	प्रत्यागम __wait_on_bit(wq_शीर्ष, &wq_प्रविष्टिः, action, बहुलकम्);
}
EXPORT_चिह्नम्(बहिर्गमनम्_सम्बन्धि_पङ्क्तिः_wait_on_bit);

पूर्णाङ्क __sched बहिर्गमनम्_सम्बन्धि_पङ्क्तिः_wait_on_bit_timeout(
	अचिह्नित दीर्घ *word, पूर्णाङ्क bit, wait_bit_action_f *action,
	अचिह्नित बहुलकम्, अचिह्नित दीर्घ timeout)
{
	संरचना wait_queue_शीर्ष *wq_शीर्ष = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_प्रविष्टिः, word, bit);

	wq_प्रविष्टिः.कुञ्चिका.timeout = jiffies + timeout;

	प्रत्यागम __wait_on_bit(wq_शीर्ष, &wq_प्रविष्टिः, action, बहुलकम्);
}
EXPORT_चिह्नम्_GPL(बहिर्गमनम्_सम्बन्धि_पङ्क्तिः_wait_on_bit_timeout);

पूर्णाङ्क __sched
__wait_on_bit_lock(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_bit_queue_प्रविष्टिः *wbq_प्रविष्टिः,
			wait_bit_action_f *action, अचिह्नित बहुलकम्)
{
	पूर्णाङ्क ret = 0;

	प्रति (;;) {
		prepare_गन्तव्यम्_wait_exclusive(wq_शीर्ष, &wbq_प्रविष्टिः->wq_प्रविष्टिः, बहुलकम्);
		यदि (test_bit(wbq_प्रविष्टिः->कुञ्चिका.bit_nr, wbq_प्रविष्टिः->कुञ्चिका.flags)) {
			ret = action(&wbq_प्रविष्टिः->कुञ्चिका, बहुलकम्);
			/*
			 * See the comment in prepare_to_wait_event().
			 * finish_wait() does not necessarily takes wwq_head->lock,
			 * but test_and_set_bit() implies mb() which pairs with
			 * smp_mb__after_atomic() before wake_up_page().
			 */
			यदि (ret)
				finish_wait(wq_शीर्ष, &wbq_प्रविष्टिः->wq_प्रविष्टिः);
		}
		यदि (!test_and_समुच्चयः_bit(wbq_प्रविष्टिः->कुञ्चिका.bit_nr, wbq_प्रविष्टिः->कुञ्चिका.flags)) {
			यदि (!ret)
				finish_wait(wq_शीर्ष, &wbq_प्रविष्टिः->wq_प्रविष्टिः);
			प्रत्यागम 0;
		} अन्यथा यदि (ret) {
			प्रत्यागम ret;
		}
	}
}
EXPORT_चिह्नम्(__wait_on_bit_lock);

पूर्णाङ्क __sched बहिर्गमनम्_सम्बन्धि_पङ्क्तिः_wait_on_bit_तालकम्(अचिह्नित दीर्घ *word, पूर्णाङ्क bit,
					 wait_bit_action_f *action, अचिह्नित बहुलकम्)
{
	संरचना wait_queue_शीर्ष *wq_शीर्ष = bit_waitqueue(word, bit);
	DEFINE_WAIT_BIT(wq_प्रविष्टिः, word, bit);

	प्रत्यागम __wait_on_bit_lock(wq_शीर्ष, &wq_प्रविष्टिः, action, बहुलकम्);
}
EXPORT_चिह्नम्(बहिर्गमनम्_सम्बन्धि_पङ्क्तिः_wait_on_bit_तालकम्);

शून्य __wake_up_bit(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित दीर्घ *word, पूर्णाङ्क bit)
{
	संरचना wait_bit_कुञ्चिका कुञ्चिका = __WAIT_BIT_KEY_INITIALIZER(word, bit);

	यदि (waitqueue_सक्रिय(wq_शीर्ष))
		__wake_up(wq_शीर्ष, TASK_प्रसामान्यम्, 1, &कुञ्चिका);
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
शून्य wake_ऊर्ध्वम्_bit(अचिह्नित दीर्घ *word, पूर्णाङ्क bit)
{
	__wake_up_bit(bit_waitqueue(word, bit), word, bit);
}
EXPORT_चिह्नम्(wake_ऊर्ध्वम्_bit);

wait_queue_शीर्ष_t *__var_waitqueue(शून्य *p)
{
	प्रत्यागम bit_wait_सारणी + hash_ptr(p, WAIT_सारणी_द्वयङ्काः);
}
EXPORT_चिह्नम्(__var_waitqueue);

संरचना wait_bit_कुञ्चिका *__var_wake_key(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, शून्य *arg)
{
	संरचना wait_bit_कुञ्चिका *कुञ्चिका = arg;
	संरचना wait_bit_queue_प्रविष्टिः *wbq_प्रविष्टिः =
		container_सम्बन्धि(wq_प्रविष्टिः, संरचना wait_bit_queue_प्रविष्टिः, wq_प्रविष्टिः);

	यदि (wbq_प्रविष्टिः->कुञ्चिका.flags != कुञ्चिका->flags ||
	    wbq_प्रविष्टिः->कुञ्चिका.bit_nr != कुञ्चिका->bit_nr)
		प्रत्यागम रिक्त;

	प्रत्यागम कुञ्चिका;
}

स्थिर पूर्णाङ्क var_wake_function(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, अचिह्नित पूर्णाङ्क बहुलकम्,
			     पूर्णाङ्क sync, शून्य *arg)
{
	संरचना wait_bit_कुञ्चिका *कुञ्चिका = __var_wake_key(wq_प्रविष्टिः, arg);
	यदि (!कुञ्चिका)
		प्रत्यागम 0;

	प्रत्यागम autoremove_wake_function(wq_प्रविष्टिः, बहुलकम्, sync, कुञ्चिका);
}

शून्य init_wait_var_प्रविष्टिः(संरचना wait_bit_queue_प्रविष्टिः *wbq_प्रविष्टिः, शून्य *var, पूर्णाङ्क flags)
{
	*wbq_प्रविष्टिः = (संरचना wait_bit_queue_प्रविष्टिः){
		.कुञ्चिका = {
			.flags	= (var),
			.bit_nr = -1,
		},
		.wq_प्रविष्टिः = {
			.flags	 = flags,
			.गुप्त = current,
			.func	 = var_wake_function,
			.प्रविष्टिः	 = LIST_शीर्ष_INIT(wbq_प्रविष्टिः->wq_प्रविष्टिः.प्रविष्टिः),
		},
	};
}
EXPORT_चिह्नम्(init_wait_var_प्रविष्टिः);

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
शून्य wake_ऊर्ध्वम्_var(शून्य *var)
{
	__wake_up_bit(__var_waitqueue(var), var, -1);
}
EXPORT_चिह्नम्(wake_ऊर्ध्वम्_var);

__sched पूर्णाङ्क bit_wait(संरचना wait_bit_कुञ्चिका *word, पूर्णाङ्क बहुलकम्)
{
	कालसारणी();
	यदि (signal_pending_state(बहुलकम्, current))
		प्रत्यागम -EINTR;

	प्रत्यागम 0;
}
EXPORT_चिह्नम्(bit_wait);

__sched पूर्णाङ्क bit_wait_io(संरचना wait_bit_कुञ्चिका *word, पूर्णाङ्क बहुलकम्)
{
	io_कालसारणी();
	यदि (signal_pending_state(बहुलकम्, current))
		प्रत्यागम -EINTR;

	प्रत्यागम 0;
}
EXPORT_चिह्नम्(bit_wait_io);

__sched पूर्णाङ्क bit_wait_timeout(संरचना wait_bit_कुञ्चिका *word, पूर्णाङ्क बहुलकम्)
{
	अचिह्नित दीर्घ now = READ_ONCE(jiffies);

	यदि (time_after_eq(now, word->timeout))
		प्रत्यागम -EAGAIN;
	कालसारणी_timeout(word->timeout - now);
	यदि (signal_pending_state(बहुलकम्, current))
		प्रत्यागम -EINTR;

	प्रत्यागम 0;
}
EXPORT_चिह्नम्_GPL(bit_wait_timeout);

शून्य __init wait_bit_init(शून्य)
{
	पूर्णाङ्क i;

	प्रति (i = 0; i < WAIT_सारणी_आकार; i++)
		init_waitqueue_शीर्ष(bit_wait_सारणी + i);
}
