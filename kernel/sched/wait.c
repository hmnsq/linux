// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#অন্তর্ভাব "sched.h"
#অন্তর্ভাব <linux/wait_bit.h>

শূন্য __init_waitqueue_head(সংরচনা wait_queue_head *wq_head, ধ্রুব বর্ণ *name, সংরচনা lock_class_চাবি *চাবি)
{
	spin_lock_init(&wq_head->lock);
	lockdep_set_class_and_name(&wq_head->lock, চাবি, name);
	INIT_LIST_HEAD(&wq_head->head);
}

EXPORT_প্রতীক(__init_waitqueue_head);

শূন্য add_wait_queue(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry)
{
	unsigned long flags;

	wq_entry->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	__add_wait_queue(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_প্রতীক(add_wait_queue);

শূন্য add_wait_queue_exclusive(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry)
{
	unsigned long flags;

	wq_entry->flags |= WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	__add_wait_queue_entry_tail(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_প্রতীক(add_wait_queue_exclusive);

শূন্য add_wait_queue_priority(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry)
{
	unsigned long flags;

	wq_entry->flags |= WQ_FLAG_PRIORITY;
	spin_lock_irqsave(&wq_head->lock, flags);
	__add_wait_queue(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_প্রতীক_GPL(add_wait_queue_priority);

পূর্ণাঙ্ক add_wait_queue_priority_exclusive(সংরচনা wait_queue_head *wq_head,
				      সংরচনা wait_queue_entry *wq_entry)
{
	সংরচনা list_head *head = &wq_head->head;

	guard(spinlock_irqsave)(&wq_head->lock);

	যদি (!list_empty(head) &&
	    (list_first_entry(head, typeof(*wq_entry), entry)->flags & WQ_FLAG_PRIORITY))
		প্রত্যাগম -EBUSY;

	wq_entry->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_PRIORITY;
	list_add(&wq_entry->entry, head);
	প্রত্যাগম 0;
}
EXPORT_প্রতীক_GPL(add_wait_queue_priority_exclusive);

শূন্য remove_wait_queue(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry)
{
	unsigned long flags;

	spin_lock_irqsave(&wq_head->lock, flags);
	__remove_wait_queue(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_প্রতীক(remove_wait_queue);

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
স্থির পূর্ণাঙ্ক __wake_up_common(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode,
			পূর্ণাঙ্ক nr_exclusive, পূর্ণাঙ্ক wake_flags, শূন্য *চাবি)
{
	wait_queue_entry_t *curr, *next;

	lockdep_assert_held(&wq_head->lock);

	curr = list_first_entry(&wq_head->head, wait_queue_entry_t, entry);

	যদি (&curr->entry == &wq_head->head)
		প্রত্যাগম nr_exclusive;

	list_for_each_entry_safe_from(curr, next, &wq_head->head, entry) {
		unsigned flags = curr->flags;
		পূর্ণাঙ্ক ret;

		ret = curr->func(curr, mode, wake_flags, চাবি);
		যদি (ret < 0)
			ভঙ্গ;
		যদি (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			ভঙ্গ;
	}

	প্রত্যাগম nr_exclusive;
}

স্থির পূর্ণাঙ্ক __wake_up_common_lock(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode,
			পূর্ণাঙ্ক nr_exclusive, পূর্ণাঙ্ক wake_flags, শূন্য *চাবি)
{
	unsigned long flags;
	পূর্ণাঙ্ক remaining;

	spin_lock_irqsave(&wq_head->lock, flags);
	remaining = __wake_up_common(wq_head, mode, nr_exclusive, wake_flags,
			চাবি);
	spin_unlock_irqrestore(&wq_head->lock, flags);

	প্রত্যাগম nr_exclusive - remaining;
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
পূর্ণাঙ্ক __wake_up(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode,
	      পূর্ণাঙ্ক nr_exclusive, শূন্য *চাবি)
{
	প্রত্যাগম __wake_up_common_lock(wq_head, mode, nr_exclusive, 0, চাবি);
}
EXPORT_প্রতীক(__wake_up);

শূন্য __wake_up_on_current_cpu(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode, শূন্য *চাবি)
{
	__wake_up_common_lock(wq_head, mode, 1, WF_CURRENT_CPU, চাবি);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
শূন্য __wake_up_locked(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode, পূর্ণাঙ্ক nr)
{
	__wake_up_common(wq_head, mode, nr, 0, রিক্ত);
}
EXPORT_প্রতীক_GPL(__wake_up_locked);

শূন্য __wake_up_locked_key(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode, শূন্য *চাবি)
{
	__wake_up_common(wq_head, mode, 1, 0, চাবি);
}
EXPORT_প্রতীক_GPL(__wake_up_locked_key);

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
শূন্য __wake_up_sync_key(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode,
			শূন্য *চাবি)
{
	যদি (unlikely(!wq_head))
		প্রত্যাগম;

	__wake_up_common_lock(wq_head, mode, 1, WF_SYNC, চাবি);
}
EXPORT_প্রতীক_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
শূন্য __wake_up_locked_sync_key(সংরচনা wait_queue_head *wq_head,
			       unsigned পূর্ণাঙ্ক mode, শূন্য *চাবি)
{
        __wake_up_common(wq_head, mode, 1, WF_SYNC, চাবি);
}
EXPORT_প্রতীক_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
শূন্য __wake_up_sync(সংরচনা wait_queue_head *wq_head, unsigned পূর্ণাঙ্ক mode)
{
	__wake_up_sync_key(wq_head, mode, রিক্ত);
}
EXPORT_প্রতীক_GPL(__wake_up_sync);	/* For internal use only */

শূন্য __wake_up_pollfree(সংরচনা wait_queue_head *wq_head)
{
	__wake_up(wq_head, TASK_NORMAL, 0, poll_to_চাবি(EPOLLHUP | POLLFREE));
	/* POLLFREE must have cleared the queue. */
	WARN_ON_ONCE(waitqueue_active(wq_head));
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
শূন্য
prepare_to_wait(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry, পূর্ণাঙ্ক state)
{
	unsigned long flags;

	wq_entry->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	যদি (list_empty(&wq_entry->entry))
		__add_wait_queue(wq_head, wq_entry);
	set_current_state(state);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_প্রতীক(prepare_to_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_to_wait_exclusive(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry, পূর্ণাঙ্ক state)
{
	unsigned long flags;
	bool was_empty = false;

	wq_entry->flags |= WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	যদি (list_empty(&wq_entry->entry)) {
		was_empty = list_empty(&wq_head->head);
		__add_wait_queue_entry_tail(wq_head, wq_entry);
	}
	set_current_state(state);
	spin_unlock_irqrestore(&wq_head->lock, flags);
	প্রত্যাগম was_empty;
}
EXPORT_প্রতীক(prepare_to_wait_exclusive);

শূন্য init_wait_entry(সংরচনা wait_queue_entry *wq_entry, পূর্ণাঙ্ক flags)
{
	wq_entry->flags = flags;
	wq_entry->গুপ্ত = current;
	wq_entry->func = autoremove_wake_function;
	INIT_LIST_HEAD(&wq_entry->entry);
}
EXPORT_প্রতীক(init_wait_entry);

long prepare_to_wait_event(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry, পূর্ণাঙ্ক state)
{
	unsigned long flags;
	long ret = 0;

	spin_lock_irqsave(&wq_head->lock, flags);
	যদি (signal_pending_state(state, current)) {
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
		list_del_init(&wq_entry->entry);
		ret = -ERESTARTSYS;
	} অন্যথা {
		যদি (list_empty(&wq_entry->entry)) {
			যদি (wq_entry->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_head, wq_entry);
			অন্যথা
				__add_wait_queue(wq_head, wq_entry);
		}
		set_current_state(state);
	}
	spin_unlock_irqrestore(&wq_head->lock, flags);

	প্রত্যাগম ret;
}
EXPORT_প্রতীক(prepare_to_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
পূর্ণাঙ্ক do_wait_intr(wait_queue_head_t *wq, wait_queue_entry_t *wait)
{
	যদি (likely(list_empty(&wait->entry)))
		__add_wait_queue_entry_tail(wq, wait);

	set_current_state(TASK_INTERRUPTIBLE);
	যদি (signal_pending(current))
		প্রত্যাগম -ERESTARTSYS;

	spin_unlock(&wq->lock);
	schedule();
	spin_lock(&wq->lock);

	প্রত্যাগম 0;
}
EXPORT_প্রতীক(do_wait_intr);

পূর্ণাঙ্ক do_wait_intr_irq(wait_queue_head_t *wq, wait_queue_entry_t *wait)
{
	যদি (likely(list_empty(&wait->entry)))
		__add_wait_queue_entry_tail(wq, wait);

	set_current_state(TASK_INTERRUPTIBLE);
	যদি (signal_pending(current))
		প্রত্যাগম -ERESTARTSYS;

	spin_unlock_irq(&wq->lock);
	schedule();
	spin_lock_irq(&wq->lock);

	প্রত্যাগম 0;
}
EXPORT_প্রতীক(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
শূন্য finish_wait(সংরচনা wait_queue_head *wq_head, সংরচনা wait_queue_entry *wq_entry)
{
	unsigned long flags;

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
	যদি (!list_empty_careful(&wq_entry->entry)) {
		spin_lock_irqsave(&wq_head->lock, flags);
		list_del_init(&wq_entry->entry);
		spin_unlock_irqrestore(&wq_head->lock, flags);
	}
}
EXPORT_প্রতীক(finish_wait);

পূর্ণাঙ্ক autoremove_wake_function(সংরচনা wait_queue_entry *wq_entry, unsigned mode, পূর্ণাঙ্ক sync, শূন্য *চাবি)
{
	পূর্ণাঙ্ক ret = default_wake_function(wq_entry, mode, sync, চাবি);

	যদি (ret)
		list_del_init_careful(&wq_entry->entry);

	প্রত্যাগম ret;
}
EXPORT_প্রতীক(autoremove_wake_function);

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
long wait_woken(সংরচনা wait_queue_entry *wq_entry, unsigned mode, long timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	set_current_state(mode); /* A */
	যদি (!(wq_entry->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = schedule_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_entry->flags, wq_entry->flags & ~WQ_FLAG_WOKEN); /* B */

	প্রত্যাগম timeout;
}
EXPORT_প্রতীক(wait_woken);

পূর্ণাঙ্ক woken_wake_function(সংরচনা wait_queue_entry *wq_entry, unsigned mode, পূর্ণাঙ্ক sync, শূন্য *চাবি)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_entry->flags |= WQ_FLAG_WOKEN;

	প্রত্যাগম default_wake_function(wq_entry, mode, sync, চাবি);
}
EXPORT_প্রতীক(woken_wake_function);

পূর্ণাঙ্ক woken_wake_bit_function(সংরচনা wait_queue_entry *wq_entry, unsigned mode, পূর্ণাঙ্ক sync, শূন্য *arg)
{
	সংরচনা wait_bit_চাবি *চাবি = __var_wake_key(wq_entry, arg);
	যদি (!চাবি)
		প্রত্যাগম 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_entry->flags |= WQ_FLAG_WOKEN;

	প্রত্যাগম default_wake_function(wq_entry, mode, sync, চাবি);
}
EXPORT_প্রতীক(woken_wake_bit_function);
