// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ <linux/wait_bit.h>

ಶೂನ್ಯ __init_waitqueue_head(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ರಚನೆ ಬೀಗ_class_ಕೀಲಿ *ಕೀಲಿ)
{
	spin_ಬೀಗ_init(&wq_ತಲೆ->ಬೀಗ);
	lockdep_ಗಣ_class_and_name(&wq_ತಲೆ->ಬೀಗ, ಕೀಲಿ, name);
	INIT_LIST_ತಲೆ(&wq_ತಲೆ->ತಲೆ);
}

EXPORT_ಚಿಹ್ನೆ(__init_waitqueue_head);

ಶೂನ್ಯ ಕೂಡಿಸು_wait_ಸರತಿ(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	wq_ನಮೂದು->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	__add_wait_queue(wq_ತಲೆ, wq_ನಮೂದು);
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ(ಕೂಡಿಸು_wait_ಸರತಿ);

ಶೂನ್ಯ ಕೂಡಿಸು_wait_ಸರತಿ_exclusive(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	wq_ನಮೂದು->flags |= WQ_FLAG_EXCLUSIVE;
	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	__add_wait_queue_entry_tail(wq_ತಲೆ, wq_ನಮೂದು);
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ(ಕೂಡಿಸು_wait_ಸರತಿ_exclusive);

ಶೂನ್ಯ ಕೂಡಿಸು_wait_ಸರತಿ_ಆದ್ಯತೆ(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	wq_ನಮೂದು->flags |= WQ_FLAG_ಆದ್ಯತೆ;
	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	__add_wait_queue(wq_ತಲೆ, wq_ನಮೂದು);
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ_GPL(ಕೂಡಿಸು_wait_ಸರತಿ_ಆದ್ಯತೆ);

ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_wait_ಸರತಿ_ಆದ್ಯತೆ_exclusive(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ,
				      ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು)
{
	ರಚನೆ list_ತಲೆ *ತಲೆ = &wq_ತಲೆ->ತಲೆ;

	guard(spinlock_irqsave)(&wq_ತಲೆ->ಬೀಗ);

	ಆದರೆ (!list_ಖಾಲಿಗಳು(ತಲೆ) &&
	    (list_ಮೊದಲ_ನಮೂದು(ತಲೆ, typeof(*wq_ನಮೂದು), ನಮೂದು)->flags & WQ_FLAG_ಆದ್ಯತೆ))
		ಹಿಂತಿರುಗಿ -EBUSY;

	wq_ನಮೂದು->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_ಆದ್ಯತೆ;
	list_ಕೂಡಿಸು(&wq_ನಮೂದು->ನಮೂದು, ತಲೆ);
	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ_GPL(ಕೂಡಿಸು_wait_ಸರತಿ_ಆದ್ಯತೆ_exclusive);

ಶೂನ್ಯ remove_wait_ಸರತಿ(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	__remove_wait_queue(wq_ತಲೆ, wq_ನಮೂದು);
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ(remove_wait_ಸರತಿ);

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __wake_up_common(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
			ಪೂರ್ಣಾಂಕ nr_exclusive, ಪೂರ್ಣಾಂಕ wake_flags, ಶೂನ್ಯ *ಕೀಲಿ)
{
	wait_ಸರತಿ_ನಮೂದು_t *curr, *ಮುಂದಿನ;

	lockdep_assert_held(&wq_ತಲೆ->ಬೀಗ);

	curr = list_ಮೊದಲ_ನಮೂದು(&wq_ತಲೆ->ತಲೆ, wait_ಸರತಿ_ನಮೂದು_t, ನಮೂದು);

	ಆದರೆ (&curr->ನಮೂದು == &wq_ತಲೆ->ತಲೆ)
		ಹಿಂತಿರುಗಿ nr_exclusive;

	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ_from(curr, ಮುಂದಿನ, &wq_ತಲೆ->ತಲೆ, ನಮೂದು) {
		ಚಿಹ್ನೆರಹಿತ flags = curr->flags;
		ಪೂರ್ಣಾಂಕ ret;

		ret = curr->func(curr, ಬಹುಲಕ, wake_flags, ಕೀಲಿ);
		ಆದರೆ (ret < 0)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ nr_exclusive;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __wake_up_common_lock(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
			ಪೂರ್ಣಾಂಕ nr_exclusive, ಪೂರ್ಣಾಂಕ wake_flags, ಶೂನ್ಯ *ಕೀಲಿ)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	ಪೂರ್ಣಾಂಕ ಉಳಿದ;

	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	ಉಳಿದ = __wake_up_common(wq_ತಲೆ, ಬಹುಲಕ, nr_exclusive, wake_flags,
			ಕೀಲಿ);
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);

	ಹಿಂತಿರುಗಿ nr_exclusive - ಉಳಿದ;
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
ಪೂರ್ಣಾಂಕ __wake_up(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
	      ಪೂರ್ಣಾಂಕ nr_exclusive, ಶೂನ್ಯ *ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ __wake_up_common_lock(wq_ತಲೆ, ಬಹುಲಕ, nr_exclusive, 0, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ(__wake_up);

ಶೂನ್ಯ __wake_up_on_current_cpu(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಶೂನ್ಯ *ಕೀಲಿ)
{
	__wake_up_common_lock(wq_ತಲೆ, ಬಹುಲಕ, 1, WF_CURRENT_CPU, ಕೀಲಿ);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
ಶೂನ್ಯ __wake_up_locked(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ nr)
{
	__wake_up_common(wq_ತಲೆ, ಬಹುಲಕ, nr, 0, ಖಾಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__wake_up_locked);

ಶೂನ್ಯ __wake_up_locked_key(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಶೂನ್ಯ *ಕೀಲಿ)
{
	__wake_up_common(wq_ತಲೆ, ಬಹುಲಕ, 1, 0, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__wake_up_locked_key);

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
ಶೂನ್ಯ __wake_up_sync_key(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ,
			ಶೂನ್ಯ *ಕೀಲಿ)
{
	ಆದರೆ (unlikely(!wq_ತಲೆ))
		ಹಿಂತಿರುಗಿ;

	__wake_up_common_lock(wq_ತಲೆ, ಬಹುಲಕ, 1, WF_SYNC, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
ಶೂನ್ಯ __wake_up_locked_sync_key(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ,
			       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಶೂನ್ಯ *ಕೀಲಿ)
{
        __wake_up_common(wq_ತಲೆ, ಬಹುಲಕ, 1, WF_SYNC, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
ಶೂನ್ಯ __wake_up_sync(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	__wake_up_sync_key(wq_ತಲೆ, ಬಹುಲಕ, ಖಾಲಿ);
}
EXPORT_ಚಿಹ್ನೆ_GPL(__wake_up_sync);	/* For internal use only */

ಶೂನ್ಯ __wake_up_pollfree(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ)
{
	__wake_up(wq_ತಲೆ, TASK_ಸಾಮಾನ್ಯ, 0, poll_ಗಮ್ಯ_ಕೀಲಿ(EPOLLHUP | POLLFREE));
	/* POLLFREE must have cleared the queue. */
	WARN_ON_ONCE(waitqueue_ಸಕ್ರಿಯ(wq_ತಲೆ));
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
ಶೂನ್ಯ
prepare_ಗಮ್ಯ_wait(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಪೂರ್ಣಾಂಕ state)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	wq_ನಮೂದು->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	ಆದರೆ (list_ಖಾಲಿಗಳು(&wq_ನಮೂದು->ನಮೂದು))
		__add_wait_queue(wq_ತಲೆ, wq_ನಮೂದು);
	ಗಣ_current_state(state);
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ(prepare_ಗಮ್ಯ_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_ಗಮ್ಯ_wait_exclusive(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಪೂರ್ಣಾಂಕ state)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	bool was_ಖಾಲಿಗಳು = false;

	wq_ನಮೂದು->flags |= WQ_FLAG_EXCLUSIVE;
	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	ಆದರೆ (list_ಖಾಲಿಗಳು(&wq_ನಮೂದು->ನಮೂದು)) {
		was_ಖಾಲಿಗಳು = list_ಖಾಲಿಗಳು(&wq_ತಲೆ->ತಲೆ);
		__add_wait_queue_entry_tail(wq_ತಲೆ, wq_ನಮೂದು);
	}
	ಗಣ_current_state(state);
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);
	ಹಿಂತಿರುಗಿ was_ಖಾಲಿಗಳು;
}
EXPORT_ಚಿಹ್ನೆ(prepare_ಗಮ್ಯ_wait_exclusive);

ಶೂನ್ಯ init_wait_ನಮೂದು(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಪೂರ್ಣಾಂಕ flags)
{
	wq_ನಮೂದು->flags = flags;
	wq_ನಮೂದು->ಖಾಸಗಿ = current;
	wq_ನಮೂದು->func = autoremove_wake_function;
	INIT_LIST_ತಲೆ(&wq_ನಮೂದು->ನಮೂದು);
}
EXPORT_ಚಿಹ್ನೆ(init_wait_ನಮೂದು);

ದೀರ್ಘ prepare_ಗಮ್ಯ_wait_event(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಪೂರ್ಣಾಂಕ state)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	ದೀರ್ಘ ret = 0;

	spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
	ಆದರೆ (signal_pending_state(state, current)) {
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
		list_del_init(&wq_ನಮೂದು->ನಮೂದು);
		ret = -ERESTARTSYS;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (list_ಖಾಲಿಗಳು(&wq_ನಮೂದು->ನಮೂದು)) {
			ಆದರೆ (wq_ನಮೂದು->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_ತಲೆ, wq_ನಮೂದು);
			ಇಲ್ಲದಿದ್ದರೆ
				__add_wait_queue(wq_ತಲೆ, wq_ನಮೂದು);
		}
		ಗಣ_current_state(state);
	}
	spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);

	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(prepare_ಗಮ್ಯ_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
ಪೂರ್ಣಾಂಕ do_wait_intr(wait_ಸರತಿ_ತಲೆ_t *wq, wait_ಸರತಿ_ನಮೂದು_t *wait)
{
	ಆದರೆ (likely(list_ಖಾಲಿಗಳು(&wait->ನಮೂದು)))
		__add_wait_queue_entry_tail(wq, wait);

	ಗಣ_current_state(TASK_INTERRUPTIBLE);
	ಆದರೆ (signal_pending(current))
		ಹಿಂತಿರುಗಿ -ERESTARTSYS;

	spin_unlock(&wq->ಬೀಗ);
	ವೇಳಾಪಟ್ಟಿ();
	spin_ಬೀಗ(&wq->ಬೀಗ);

	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(do_wait_intr);

ಪೂರ್ಣಾಂಕ do_wait_intr_irq(wait_ಸರತಿ_ತಲೆ_t *wq, wait_ಸರತಿ_ನಮೂದು_t *wait)
{
	ಆದರೆ (likely(list_ಖಾಲಿಗಳು(&wait->ನಮೂದು)))
		__add_wait_queue_entry_tail(wq, wait);

	ಗಣ_current_state(TASK_INTERRUPTIBLE);
	ಆದರೆ (signal_pending(current))
		ಹಿಂತಿರುಗಿ -ERESTARTSYS;

	spin_unlock_irq(&wq->ಬೀಗ);
	ವೇಳಾಪಟ್ಟಿ();
	spin_ಬೀಗ_irq(&wq->ಬೀಗ);

	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
ಶೂನ್ಯ ಮುಕ್ತಾಯ_wait(ರಚನೆ wait_ಸರತಿ_ತಲೆ *wq_ತಲೆ, ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

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
	ಆದರೆ (!list_ಖಾಲಿಗಳು_careful(&wq_ನಮೂದು->ನಮೂದು)) {
		spin_ಬೀಗ_irqsave(&wq_ತಲೆ->ಬೀಗ, flags);
		list_del_init(&wq_ನಮೂದು->ನಮೂದು);
		spin_unlock_irqrestore(&wq_ತಲೆ->ಬೀಗ, flags);
	}
}
EXPORT_ಚಿಹ್ನೆ(ಮುಕ್ತಾಯ_wait);

ಪೂರ್ಣಾಂಕ autoremove_wake_function(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ sync, ಶೂನ್ಯ *ಕೀಲಿ)
{
	ಪೂರ್ಣಾಂಕ ret = default_wake_function(wq_ನಮೂದು, ಬಹುಲಕ, sync, ಕೀಲಿ);

	ಆದರೆ (ret)
		list_del_init_careful(&wq_ನಮೂದು->ನಮೂದು);

	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(autoremove_wake_function);

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
ದೀರ್ಘ wait_woken(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ದೀರ್ಘ timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	ಗಣ_current_state(ಬಹುಲಕ); /* A */
	ಆದರೆ (!(wq_ನಮೂದು->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = ವೇಳಾಪಟ್ಟಿ_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_ನಮೂದು->flags, wq_ನಮೂದು->flags & ~WQ_FLAG_WOKEN); /* B */

	ಹಿಂತಿರುಗಿ timeout;
}
EXPORT_ಚಿಹ್ನೆ(wait_woken);

ಪೂರ್ಣಾಂಕ woken_wake_function(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ sync, ಶೂನ್ಯ *ಕೀಲಿ)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_ನಮೂದು->flags |= WQ_FLAG_WOKEN;

	ಹಿಂತಿರುಗಿ default_wake_function(wq_ನಮೂದು, ಬಹುಲಕ, sync, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ(woken_wake_function);

ಪೂರ್ಣಾಂಕ woken_wake_ಬಿಟ್_function(ರಚನೆ wait_ಸರತಿ_ನಮೂದು *wq_ನಮೂದು, ಚಿಹ್ನೆರಹಿತ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ sync, ಶೂನ್ಯ *arg)
{
	ರಚನೆ wait_ಬಿಟ್_ಕೀಲಿ *ಕೀಲಿ = __var_wake_key(wq_ನಮೂದು, arg);
	ಆದರೆ (!ಕೀಲಿ)
		ಹಿಂತಿರುಗಿ 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_ನಮೂದು->flags |= WQ_FLAG_WOKEN;

	ಹಿಂತಿರುಗಿ default_wake_function(wq_ನಮೂದು, ಬಹುಲಕ, sync, ಕೀಲಿ);
}
EXPORT_ಚಿಹ್ನೆ(woken_wake_ಬಿಟ್_function);
