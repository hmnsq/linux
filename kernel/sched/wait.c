// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#അന്തര്ഭാവ "sched.h"
#അന്തര്ഭാവ <linux/wait_bit.h>

ശൂന്യം __init_waitqueue_head(ഘടന wait_queue_head *wq_head, സ്ഥിരം അക്ഷരം *name, ഘടന lock_class_താക്കോൽ *താക്കോൽ)
{
	spin_lock_init(&wq_head->lock);
	lockdep_set_class_and_name(&wq_head->lock, താക്കോൽ, name);
	INIT_LIST_HEAD(&wq_head->head);
}

EXPORT_ചിഹ്നം(__init_waitqueue_head);

ശൂന്യം add_wait_queue(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry)
{
	unsigned long flags;

	wq_entry->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	__add_wait_queue(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_ചിഹ്നം(add_wait_queue);

ശൂന്യം add_wait_queue_exclusive(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry)
{
	unsigned long flags;

	wq_entry->flags |= WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	__add_wait_queue_entry_tail(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_ചിഹ്നം(add_wait_queue_exclusive);

ശൂന്യം add_wait_queue_priority(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry)
{
	unsigned long flags;

	wq_entry->flags |= WQ_FLAG_PRIORITY;
	spin_lock_irqsave(&wq_head->lock, flags);
	__add_wait_queue(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_ചിഹ്നം_GPL(add_wait_queue_priority);

പൂർണ്ണസംഖ്യ add_wait_queue_priority_exclusive(ഘടന wait_queue_head *wq_head,
				      ഘടന wait_queue_entry *wq_entry)
{
	ഘടന list_head *head = &wq_head->head;

	guard(spinlock_irqsave)(&wq_head->lock);

	എങ്കിൽ (!list_empty(head) &&
	    (list_first_entry(head, typeof(*wq_entry), entry)->flags & WQ_FLAG_PRIORITY))
		തിരികെ -EBUSY;

	wq_entry->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_PRIORITY;
	list_add(&wq_entry->entry, head);
	തിരികെ 0;
}
EXPORT_ചിഹ്നം_GPL(add_wait_queue_priority_exclusive);

ശൂന്യം remove_wait_queue(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry)
{
	unsigned long flags;

	spin_lock_irqsave(&wq_head->lock, flags);
	__remove_wait_queue(wq_head, wq_entry);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_ചിഹ്നം(remove_wait_queue);

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
സ്ഥിതം പൂർണ്ണസംഖ്യ __wake_up_common(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode,
			പൂർണ്ണസംഖ്യ nr_exclusive, പൂർണ്ണസംഖ്യ wake_flags, ശൂന്യം *താക്കോൽ)
{
	wait_queue_entry_t *curr, *next;

	lockdep_assert_held(&wq_head->lock);

	curr = list_first_entry(&wq_head->head, wait_queue_entry_t, entry);

	എങ്കിൽ (&curr->entry == &wq_head->head)
		തിരികെ nr_exclusive;

	list_for_each_entry_safe_from(curr, next, &wq_head->head, entry) {
		unsigned flags = curr->flags;
		പൂർണ്ണസംഖ്യ ret;

		ret = curr->func(curr, mode, wake_flags, താക്കോൽ);
		എങ്കിൽ (ret < 0)
			നിർത്തുക;
		എങ്കിൽ (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			നിർത്തുക;
	}

	തിരികെ nr_exclusive;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __wake_up_common_lock(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode,
			പൂർണ്ണസംഖ്യ nr_exclusive, പൂർണ്ണസംഖ്യ wake_flags, ശൂന്യം *താക്കോൽ)
{
	unsigned long flags;
	പൂർണ്ണസംഖ്യ remaining;

	spin_lock_irqsave(&wq_head->lock, flags);
	remaining = __wake_up_common(wq_head, mode, nr_exclusive, wake_flags,
			താക്കോൽ);
	spin_unlock_irqrestore(&wq_head->lock, flags);

	തിരികെ nr_exclusive - remaining;
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
പൂർണ്ണസംഖ്യ __wake_up(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode,
	      പൂർണ്ണസംഖ്യ nr_exclusive, ശൂന്യം *താക്കോൽ)
{
	തിരികെ __wake_up_common_lock(wq_head, mode, nr_exclusive, 0, താക്കോൽ);
}
EXPORT_ചിഹ്നം(__wake_up);

ശൂന്യം __wake_up_on_current_cpu(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode, ശൂന്യം *താക്കോൽ)
{
	__wake_up_common_lock(wq_head, mode, 1, WF_CURRENT_CPU, താക്കോൽ);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
ശൂന്യം __wake_up_locked(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode, പൂർണ്ണസംഖ്യ nr)
{
	__wake_up_common(wq_head, mode, nr, 0, ശൂന്യ);
}
EXPORT_ചിഹ്നം_GPL(__wake_up_locked);

ശൂന്യം __wake_up_locked_key(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode, ശൂന്യം *താക്കോൽ)
{
	__wake_up_common(wq_head, mode, 1, 0, താക്കോൽ);
}
EXPORT_ചിഹ്നം_GPL(__wake_up_locked_key);

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
ശൂന്യം __wake_up_sync_key(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode,
			ശൂന്യം *താക്കോൽ)
{
	എങ്കിൽ (unlikely(!wq_head))
		തിരികെ;

	__wake_up_common_lock(wq_head, mode, 1, WF_SYNC, താക്കോൽ);
}
EXPORT_ചിഹ്നം_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
ശൂന്യം __wake_up_locked_sync_key(ഘടന wait_queue_head *wq_head,
			       unsigned പൂർണ്ണസംഖ്യ mode, ശൂന്യം *താക്കോൽ)
{
        __wake_up_common(wq_head, mode, 1, WF_SYNC, താക്കോൽ);
}
EXPORT_ചിഹ്നം_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
ശൂന്യം __wake_up_sync(ഘടന wait_queue_head *wq_head, unsigned പൂർണ്ണസംഖ്യ mode)
{
	__wake_up_sync_key(wq_head, mode, ശൂന്യ);
}
EXPORT_ചിഹ്നം_GPL(__wake_up_sync);	/* For internal use only */

ശൂന്യം __wake_up_pollfree(ഘടന wait_queue_head *wq_head)
{
	__wake_up(wq_head, TASK_NORMAL, 0, poll_to_താക്കോൽ(EPOLLHUP | POLLFREE));
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
ശൂന്യം
prepare_to_wait(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry, പൂർണ്ണസംഖ്യ state)
{
	unsigned long flags;

	wq_entry->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	എങ്കിൽ (list_empty(&wq_entry->entry))
		__add_wait_queue(wq_head, wq_entry);
	set_current_state(state);
	spin_unlock_irqrestore(&wq_head->lock, flags);
}
EXPORT_ചിഹ്നം(prepare_to_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_to_wait_exclusive(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry, പൂർണ്ണസംഖ്യ state)
{
	unsigned long flags;
	bool was_empty = false;

	wq_entry->flags |= WQ_FLAG_EXCLUSIVE;
	spin_lock_irqsave(&wq_head->lock, flags);
	എങ്കിൽ (list_empty(&wq_entry->entry)) {
		was_empty = list_empty(&wq_head->head);
		__add_wait_queue_entry_tail(wq_head, wq_entry);
	}
	set_current_state(state);
	spin_unlock_irqrestore(&wq_head->lock, flags);
	തിരികെ was_empty;
}
EXPORT_ചിഹ്നം(prepare_to_wait_exclusive);

ശൂന്യം init_wait_entry(ഘടന wait_queue_entry *wq_entry, പൂർണ്ണസംഖ്യ flags)
{
	wq_entry->flags = flags;
	wq_entry->സ്വകാര്യ = current;
	wq_entry->func = autoremove_wake_function;
	INIT_LIST_HEAD(&wq_entry->entry);
}
EXPORT_ചിഹ്നം(init_wait_entry);

long prepare_to_wait_event(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry, പൂർണ്ണസംഖ്യ state)
{
	unsigned long flags;
	long ret = 0;

	spin_lock_irqsave(&wq_head->lock, flags);
	എങ്കിൽ (signal_pending_state(state, current)) {
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
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (list_empty(&wq_entry->entry)) {
			എങ്കിൽ (wq_entry->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_head, wq_entry);
			അല്ലെങ്കിൽ
				__add_wait_queue(wq_head, wq_entry);
		}
		set_current_state(state);
	}
	spin_unlock_irqrestore(&wq_head->lock, flags);

	തിരികെ ret;
}
EXPORT_ചിഹ്നം(prepare_to_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
പൂർണ്ണസംഖ്യ do_wait_intr(wait_queue_head_t *wq, wait_queue_entry_t *wait)
{
	എങ്കിൽ (likely(list_empty(&wait->entry)))
		__add_wait_queue_entry_tail(wq, wait);

	set_current_state(TASK_INTERRUPTIBLE);
	എങ്കിൽ (signal_pending(current))
		തിരികെ -ERESTARTSYS;

	spin_unlock(&wq->lock);
	schedule();
	spin_lock(&wq->lock);

	തിരികെ 0;
}
EXPORT_ചിഹ്നം(do_wait_intr);

പൂർണ്ണസംഖ്യ do_wait_intr_irq(wait_queue_head_t *wq, wait_queue_entry_t *wait)
{
	എങ്കിൽ (likely(list_empty(&wait->entry)))
		__add_wait_queue_entry_tail(wq, wait);

	set_current_state(TASK_INTERRUPTIBLE);
	എങ്കിൽ (signal_pending(current))
		തിരികെ -ERESTARTSYS;

	spin_unlock_irq(&wq->lock);
	schedule();
	spin_lock_irq(&wq->lock);

	തിരികെ 0;
}
EXPORT_ചിഹ്നം(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
ശൂന്യം finish_wait(ഘടന wait_queue_head *wq_head, ഘടന wait_queue_entry *wq_entry)
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
	എങ്കിൽ (!list_empty_careful(&wq_entry->entry)) {
		spin_lock_irqsave(&wq_head->lock, flags);
		list_del_init(&wq_entry->entry);
		spin_unlock_irqrestore(&wq_head->lock, flags);
	}
}
EXPORT_ചിഹ്നം(finish_wait);

പൂർണ്ണസംഖ്യ autoremove_wake_function(ഘടന wait_queue_entry *wq_entry, unsigned mode, പൂർണ്ണസംഖ്യ sync, ശൂന്യം *താക്കോൽ)
{
	പൂർണ്ണസംഖ്യ ret = default_wake_function(wq_entry, mode, sync, താക്കോൽ);

	എങ്കിൽ (ret)
		list_del_init_careful(&wq_entry->entry);

	തിരികെ ret;
}
EXPORT_ചിഹ്നം(autoremove_wake_function);

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
long wait_woken(ഘടന wait_queue_entry *wq_entry, unsigned mode, long timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	set_current_state(mode); /* A */
	എങ്കിൽ (!(wq_entry->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = schedule_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_entry->flags, wq_entry->flags & ~WQ_FLAG_WOKEN); /* B */

	തിരികെ timeout;
}
EXPORT_ചിഹ്നം(wait_woken);

പൂർണ്ണസംഖ്യ woken_wake_function(ഘടന wait_queue_entry *wq_entry, unsigned mode, പൂർണ്ണസംഖ്യ sync, ശൂന്യം *താക്കോൽ)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_entry->flags |= WQ_FLAG_WOKEN;

	തിരികെ default_wake_function(wq_entry, mode, sync, താക്കോൽ);
}
EXPORT_ചിഹ്നം(woken_wake_function);

പൂർണ്ണസംഖ്യ woken_wake_bit_function(ഘടന wait_queue_entry *wq_entry, unsigned mode, പൂർണ്ണസംഖ്യ sync, ശൂന്യം *arg)
{
	ഘടന wait_bit_താക്കോൽ *താക്കോൽ = __var_wake_key(wq_entry, arg);
	എങ്കിൽ (!താക്കോൽ)
		തിരികെ 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_entry->flags |= WQ_FLAG_WOKEN;

	തിരികെ default_wake_function(wq_entry, mode, sync, താക്കോൽ);
}
EXPORT_ചിഹ്നം(woken_wake_bit_function);
