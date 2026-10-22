// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/wait_bit.h>

ਖਾਲੀ __init_waitqueue_head(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਅਚਲ ਅੱਖਰ *name, ਬਣਤਰ ਤਾਲਾ_class_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
	spin_ਤਾਲਾ_init(&wq_ਸਿਰ->ਤਾਲਾ);
	lockdep_ਸੈੱਟ_class_and_name(&wq_ਸਿਰ->ਤਾਲਾ, ਕੁੰਜੀ, name);
	INIT_LIST_ਸਿਰ(&wq_ਸਿਰ->ਸਿਰ);
}

EXPORT_ਚਿੰਨ੍ਹ(__init_waitqueue_head);

ਖਾਲੀ ਜੋੜੋ_wait_ਕਤਾਰ(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	wq_ਇੰਦਰਾਜ->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	__add_wait_queue(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);
}
EXPORT_ਚਿੰਨ੍ਹ(ਜੋੜੋ_wait_ਕਤਾਰ);

ਖਾਲੀ ਜੋੜੋ_wait_ਕਤਾਰ_exclusive(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	wq_ਇੰਦਰਾਜ->flags |= WQ_FLAG_EXCLUSIVE;
	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	__add_wait_queue_entry_tail(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);
}
EXPORT_ਚਿੰਨ੍ਹ(ਜੋੜੋ_wait_ਕਤਾਰ_exclusive);

ਖਾਲੀ ਜੋੜੋ_wait_ਕਤਾਰ_ਤਰਜੀਹ(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	wq_ਇੰਦਰਾਜ->flags |= WQ_FLAG_ਤਰਜੀਹ;
	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	__add_wait_queue(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(ਜੋੜੋ_wait_ਕਤਾਰ_ਤਰਜੀਹ);

ਪੂਰਨ ਜੋੜੋ_wait_ਕਤਾਰ_ਤਰਜੀਹ_exclusive(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ,
				      ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ)
{
	ਬਣਤਰ list_ਸਿਰ *ਸਿਰ = &wq_ਸਿਰ->ਸਿਰ;

	guard(spinlock_irqsave)(&wq_ਸਿਰ->ਤਾਲਾ);

	ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(ਸਿਰ) &&
	    (list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(ਸਿਰ, typeof(*wq_ਇੰਦਰਾਜ), ਇੰਦਰਾਜ)->flags & WQ_FLAG_ਤਰਜੀਹ))
		ਵਾਪਸ -EBUSY;

	wq_ਇੰਦਰਾਜ->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_ਤਰਜੀਹ;
	list_ਜੋੜੋ(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ, ਸਿਰ);
	ਵਾਪਸ 0;
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(ਜੋੜੋ_wait_ਕਤਾਰ_ਤਰਜੀਹ_exclusive);

ਖਾਲੀ remove_wait_ਕਤਾਰ(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	__remove_wait_queue(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);
}
EXPORT_ਚਿੰਨ੍ਹ(remove_wait_ਕਤਾਰ);

/*
 * The core wakeup function. Non-exclusive wakeups (nr_exclusive == 0) just
 * wake everything up. If it's an exclusive wakeup (nr_exclusive == small +ve
 * number) then we wake that number of exclusive tasks, and potentially all
 * the non-exclusive tasks. Normally, exclusive tasks will be at the end of
 * the list and any non-exclusive tasks will be woken first. A priority task
 * may be at the head of the list, and can consume the event without any other
 * tasks being woken if it's also an exclusive task.
 *
 * There are circumstances in which we can try to wake a task which has already
 * started to run but is not in state TASK_RUNNING. try_to_wake_up() returns
 * zero in this (rare) case, and we handle it by continuing to scan the queue.
 */
ਸਥਿਰ ਪੂਰਨ __wake_up_common(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ,
			ਪੂਰਨ nr_exclusive, ਪੂਰਨ wake_flags, ਖਾਲੀ *ਕੁੰਜੀ)
{
	wait_ਕਤਾਰ_ਇੰਦਰਾਜ_t *curr, *ਅਗਲਾ;

	lockdep_assert_held(&wq_ਸਿਰ->ਤਾਲਾ);

	curr = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&wq_ਸਿਰ->ਸਿਰ, wait_ਕਤਾਰ_ਇੰਦਰਾਜ_t, ਇੰਦਰਾਜ);

	ਜੇ (&curr->ਇੰਦਰਾਜ == &wq_ਸਿਰ->ਸਿਰ)
		ਵਾਪਸ nr_exclusive;

	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ_from(curr, ਅਗਲਾ, &wq_ਸਿਰ->ਸਿਰ, ਇੰਦਰਾਜ) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ flags = curr->flags;
		ਪੂਰਨ ret;

		ret = curr->func(curr, ਬਹੁਲਕ, wake_flags, ਕੁੰਜੀ);
		ਜੇ (ret < 0)
			ਰੋਕੋ;
		ਜੇ (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			ਰੋਕੋ;
	}

	ਵਾਪਸ nr_exclusive;
}

ਸਥਿਰ ਪੂਰਨ __wake_up_common_lock(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ,
			ਪੂਰਨ nr_exclusive, ਪੂਰਨ wake_flags, ਖਾਲੀ *ਕੁੰਜੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	ਪੂਰਨ ਬਚਿਆ;

	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	ਬਚਿਆ = __wake_up_common(wq_ਸਿਰ, ਬਹੁਲਕ, nr_exclusive, wake_flags,
			ਕੁੰਜੀ);
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);

	ਵਾਪਸ nr_exclusive - ਬਚਿਆ;
}

/**
 * __wake_up - wake up threads blocked on a waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @nr_exclusive: how many wake-one or wake-many threads to wake up
 * @key: is directly passed to the wakeup function
 *
 * If this function wakes up a task, it executes a full memory barrier
 * before accessing the task state.  Returns the number of exclusive
 * tasks that were awaken.
 */
ਪੂਰਨ __wake_up(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ,
	      ਪੂਰਨ nr_exclusive, ਖਾਲੀ *ਕੁੰਜੀ)
{
	ਵਾਪਸ __wake_up_common_lock(wq_ਸਿਰ, ਬਹੁਲਕ, nr_exclusive, 0, ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ(__wake_up);

ਖਾਲੀ __wake_up_on_current_cpu(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ, ਖਾਲੀ *ਕੁੰਜੀ)
{
	__wake_up_common_lock(wq_ਸਿਰ, ਬਹੁਲਕ, 1, WF_CURRENT_CPU, ਕੁੰਜੀ);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
ਖਾਲੀ __wake_up_locked(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ, ਪੂਰਨ nr)
{
	__wake_up_common(wq_ਸਿਰ, ਬਹੁਲਕ, nr, 0, ਨਿਰਾ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__wake_up_locked);

ਖਾਲੀ __wake_up_locked_key(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ, ਖਾਲੀ *ਕੁੰਜੀ)
{
	__wake_up_common(wq_ਸਿਰ, ਬਹੁਲਕ, 1, 0, ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__wake_up_locked_key);

/**
 * __wake_up_sync_key - wake up threads blocked on a waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Passes WF_SYNC to waitqueue wake functions. The default wake function
 * forwards it to the scheduler; see WF_SYNC for the hint's semantics.
 *
 * If this function wakes up a task, it executes a full memory barrier
 * before accessing the task state.
 */
ਖਾਲੀ __wake_up_sync_key(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ,
			ਖਾਲੀ *ਕੁੰਜੀ)
{
	ਜੇ (unlikely(!wq_ਸਿਰ))
		ਵਾਪਸ;

	__wake_up_common_lock(wq_ਸਿਰ, ਬਹੁਲਕ, 1, WF_SYNC, ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
ਖਾਲੀ __wake_up_locked_sync_key(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ, ਖਾਲੀ *ਕੁੰਜੀ)
{
        __wake_up_common(wq_ਸਿਰ, ਬਹੁਲਕ, 1, WF_SYNC, ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
ਖਾਲੀ __wake_up_sync(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ)
{
	__wake_up_sync_key(wq_ਸਿਰ, ਬਹੁਲਕ, ਨਿਰਾ);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(__wake_up_sync);	/* For internal use only */

ਖਾਲੀ __wake_up_pollfree(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ)
{
	__wake_up(wq_ਸਿਰ, TASK_ਸਾਧਾਰਨ, 0, poll_ਮੰਜ਼ਿਲ_ਕੁੰਜੀ(EPOLLHUP | POLLFREE));
	/* POLLFREE must have cleared the queue. */
	WARN_ON_ONCE(waitqueue_ਸਰਗਰਮ(wq_ਸਿਰ));
}

/*
 * Note: we use "set_current_state()" _after_ the wait-queue add,
 * because we need a memory barrier there on SMP, so that any
 * wake-function that tests for the wait-queue being active
 * will be guaranteed to see waitqueue addition _or_ subsequent
 * tests in this thread will see the wakeup having taken place.
 *
 * The spin_unlock() itself is semi-permeable and only protects
 * one way (it only protects stuff inside the critical region and
 * stops them from bleeding out - it would still allow subsequent
 * loads to move into the critical region).
 */
ਖਾਲੀ
prepare_ਮੰਜ਼ਿਲ_wait(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਪੂਰਨ state)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	wq_ਇੰਦਰਾਜ->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ))
		__add_wait_queue(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
	ਸੈੱਟ_current_state(state);
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);
}
EXPORT_ਚਿੰਨ੍ਹ(prepare_ਮੰਜ਼ਿਲ_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_ਮੰਜ਼ਿਲ_wait_exclusive(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਪੂਰਨ state)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	bool was_ਖਾਲੀ_ਥਾਵਾਂ = false;

	wq_ਇੰਦਰਾਜ->flags |= WQ_FLAG_EXCLUSIVE;
	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ)) {
		was_ਖਾਲੀ_ਥਾਵਾਂ = list_ਖਾਲੀ_ਥਾਵਾਂ(&wq_ਸਿਰ->ਸਿਰ);
		__add_wait_queue_entry_tail(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
	}
	ਸੈੱਟ_current_state(state);
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	ਵਾਪਸ was_ਖਾਲੀ_ਥਾਵਾਂ;
}
EXPORT_ਚਿੰਨ੍ਹ(prepare_ਮੰਜ਼ਿਲ_wait_exclusive);

ਖਾਲੀ init_wait_ਇੰਦਰਾਜ(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਪੂਰਨ flags)
{
	wq_ਇੰਦਰਾਜ->flags = flags;
	wq_ਇੰਦਰਾਜ->ਨਿਜੀ = current;
	wq_ਇੰਦਰਾਜ->func = autoremove_wake_function;
	INIT_LIST_ਸਿਰ(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ);
}
EXPORT_ਚਿੰਨ੍ਹ(init_wait_ਇੰਦਰਾਜ);

ਲੰਮਾ prepare_ਮੰਜ਼ਿਲ_wait_event(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਪੂਰਨ state)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	ਲੰਮਾ ret = 0;

	spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	ਜੇ (signal_pending_state(state, current)) {
		/*
		 * Exclusive waiter must not fail if it was selected by wakeup,
		 * it should "consume" the condition we were waiting for.
		 *
		 * The caller will recheck the condition and return success if
		 * we were already woken up, we can not miss the event because
		 * wakeup locks/unlocks the same wq_head->lock.
		 *
		 * But we need to ensure that set-condition + wakeup after that
		 * can't see us, it should wake up another exclusive waiter if
		 * we fail.
		 */
		list_del_init(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ);
		ret = -ERESTARTSYS;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ)) {
			ਜੇ (wq_ਇੰਦਰਾਜ->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
			ਨਹੀਂਤਾਂ
				__add_wait_queue(wq_ਸਿਰ, wq_ਇੰਦਰਾਜ);
		}
		ਸੈੱਟ_current_state(state);
	}
	spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);

	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(prepare_ਮੰਜ਼ਿਲ_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
ਪੂਰਨ do_wait_intr(wait_ਕਤਾਰ_ਸਿਰ_t *wq, wait_ਕਤਾਰ_ਇੰਦਰਾਜ_t *wait)
{
	ਜੇ (likely(list_ਖਾਲੀ_ਥਾਵਾਂ(&wait->ਇੰਦਰਾਜ)))
		__add_wait_queue_entry_tail(wq, wait);

	ਸੈੱਟ_current_state(TASK_INTERRUPTIBLE);
	ਜੇ (signal_pending(current))
		ਵਾਪਸ -ERESTARTSYS;

	spin_unlock(&wq->ਤਾਲਾ);
	ਸਮਾਂ_ਸਾਰਣੀ();
	spin_ਤਾਲਾ(&wq->ਤਾਲਾ);

	ਵਾਪਸ 0;
}
EXPORT_ਚਿੰਨ੍ਹ(do_wait_intr);

ਪੂਰਨ do_wait_intr_irq(wait_ਕਤਾਰ_ਸਿਰ_t *wq, wait_ਕਤਾਰ_ਇੰਦਰਾਜ_t *wait)
{
	ਜੇ (likely(list_ਖਾਲੀ_ਥਾਵਾਂ(&wait->ਇੰਦਰਾਜ)))
		__add_wait_queue_entry_tail(wq, wait);

	ਸੈੱਟ_current_state(TASK_INTERRUPTIBLE);
	ਜੇ (signal_pending(current))
		ਵਾਪਸ -ERESTARTSYS;

	spin_unlock_irq(&wq->ਤਾਲਾ);
	ਸਮਾਂ_ਸਾਰਣੀ();
	spin_ਤਾਲਾ_irq(&wq->ਤਾਲਾ);

	ਵਾਪਸ 0;
}
EXPORT_ਚਿੰਨ੍ਹ(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
ਖਾਲੀ ਅੰਤ_wait(ਬਣਤਰ wait_ਕਤਾਰ_ਸਿਰ *wq_ਸਿਰ, ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	__set_current_state(TASK_RUNNING);
	/*
	 * We can check for list emptiness outside the lock
	 * IFF:
	 *  - we use the "careful" check that verifies both
	 *    the next and prev pointers, so that there cannot
	 *    be any half-pending updates in progress on other
	 *    CPU's that we haven't seen yet (and that might
	 *    still change the stack area.
	 * and
	 *  - all other users take the lock (ie we can only
	 *    have _one_ other CPU that looks at or modifies
	 *    the list).
	 */
	ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ_careful(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ)) {
		spin_ਤਾਲਾ_irqsave(&wq_ਸਿਰ->ਤਾਲਾ, flags);
		list_del_init(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ);
		spin_unlock_irqrestore(&wq_ਸਿਰ->ਤਾਲਾ, flags);
	}
}
EXPORT_ਚਿੰਨ੍ਹ(ਅੰਤ_wait);

ਪੂਰਨ autoremove_wake_function(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਪੂਰਨ sync, ਖਾਲੀ *ਕੁੰਜੀ)
{
	ਪੂਰਨ ret = default_wake_function(wq_ਇੰਦਰਾਜ, ਬਹੁਲਕ, sync, ਕੁੰਜੀ);

	ਜੇ (ret)
		list_del_init_careful(&wq_ਇੰਦਰਾਜ->ਇੰਦਰਾਜ);

	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(autoremove_wake_function);

/*
 * DEFINE_WAIT_FUNC(wait, woken_wake_func);
 *
 * add_wait_queue(&wq_head, &wait);
 * for (;;) {
 *     if (condition)
 *         break;
 *
 *     // in wait_woken()			// in woken_wake_function()
 *
 *     p->state = mode;				wq_entry->flags |= WQ_FLAG_WOKEN;
 *     smp_mb(); // A				try_to_wake_up():
 *     if (!(wq_entry->flags & WQ_FLAG_WOKEN))	   <full barrier>
 *         schedule()				   if (p->state & mode)
 *     p->state = TASK_RUNNING;			      p->state = TASK_RUNNING;
 *     wq_entry->flags &= ~WQ_FLAG_WOKEN;	~~~~~~~~~~~~~~~~~~
 *     smp_mb(); // B				condition = true;
 * }						smp_mb(); // C
 * remove_wait_queue(&wq_head, &wait);		wq_entry->flags |= WQ_FLAG_WOKEN;
 */
ਲੰਮਾ wait_woken(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਲੰਮਾ timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	ਸੈੱਟ_current_state(ਬਹੁਲਕ); /* A */
	ਜੇ (!(wq_ਇੰਦਰਾਜ->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = ਸਮਾਂ_ਸਾਰਣੀ_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_ਇੰਦਰਾਜ->flags, wq_ਇੰਦਰਾਜ->flags & ~WQ_FLAG_WOKEN); /* B */

	ਵਾਪਸ timeout;
}
EXPORT_ਚਿੰਨ੍ਹ(wait_woken);

ਪੂਰਨ woken_wake_function(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਪੂਰਨ sync, ਖਾਲੀ *ਕੁੰਜੀ)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_ਇੰਦਰਾਜ->flags |= WQ_FLAG_WOKEN;

	ਵਾਪਸ default_wake_function(wq_ਇੰਦਰਾਜ, ਬਹੁਲਕ, sync, ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ(woken_wake_function);

ਪੂਰਨ woken_wake_ਦੋਆਧਾਰੀ_ਅੰਕ_function(ਬਣਤਰ wait_ਕਤਾਰ_ਇੰਦਰਾਜ *wq_ਇੰਦਰਾਜ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਬਹੁਲਕ, ਪੂਰਨ sync, ਖਾਲੀ *arg)
{
	ਬਣਤਰ wait_ਦੋਆਧਾਰੀ_ਅੰਕ_ਕੁੰਜੀ *ਕੁੰਜੀ = __var_wake_key(wq_ਇੰਦਰਾਜ, arg);
	ਜੇ (!ਕੁੰਜੀ)
		ਵਾਪਸ 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_ਇੰਦਰਾਜ->flags |= WQ_FLAG_WOKEN;

	ਵਾਪਸ default_wake_function(wq_ਇੰਦਰਾਜ, ਬਹੁਲਕ, sync, ਕੁੰਜੀ);
}
EXPORT_ਚਿੰਨ੍ਹ(woken_wake_ਦੋਆਧਾਰੀ_ਅੰਕ_function);
