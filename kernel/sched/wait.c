// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#అన్తర్భావ "sched.h"
#అన్తర్భావ <linux/wait_bit.h>

శూన్య __init_waitqueue_head(నిర్మాణం wait_వరుస_తల *wq_తల, స్థిరరాశి అక్షరం *name, నిర్మాణం తాళం_class_కీలకం *కీలకం)
{
	spin_తాళం_init(&wq_తల->తాళం);
	lockdep_సమితి_class_and_name(&wq_తల->తాళం, కీలకం, name);
	INIT_LIST_తల(&wq_తల->తల);
}

EXPORT_చిహ్నం(__init_waitqueue_head);

శూన్య కూడు_wait_వరుస(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు)
{
	గుర్తులేని దీర్ఘ flags;

	wq_నమోదు->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	__add_wait_queue(wq_తల, wq_నమోదు);
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);
}
EXPORT_చిహ్నం(కూడు_wait_వరుస);

శూన్య కూడు_wait_వరుస_exclusive(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు)
{
	గుర్తులేని దీర్ఘ flags;

	wq_నమోదు->flags |= WQ_FLAG_EXCLUSIVE;
	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	__add_wait_queue_entry_tail(wq_తల, wq_నమోదు);
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);
}
EXPORT_చిహ్నం(కూడు_wait_వరుస_exclusive);

శూన్య కూడు_wait_వరుస_ప్రాధాన్యత(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు)
{
	గుర్తులేని దీర్ఘ flags;

	wq_నమోదు->flags |= WQ_FLAG_ప్రాధాన్యత;
	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	__add_wait_queue(wq_తల, wq_నమోదు);
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);
}
EXPORT_చిహ్నం_GPL(కూడు_wait_వరుస_ప్రాధాన్యత);

పూర్ణాంకం కూడు_wait_వరుస_ప్రాధాన్యత_exclusive(నిర్మాణం wait_వరుస_తల *wq_తల,
				      నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు)
{
	నిర్మాణం list_తల *తల = &wq_తల->తల;

	guard(spinlock_irqsave)(&wq_తల->తాళం);

	ఒకవేళ (!list_ఖాళీలు(తల) &&
	    (list_మొదటి_నమోదు(తల, typeof(*wq_నమోదు), నమోదు_2)->flags & WQ_FLAG_ప్రాధాన్యత))
		తిరిగి -EBUSY;

	wq_నమోదు->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_ప్రాధాన్యత;
	list_కూడు(&wq_నమోదు->నమోదు_2, తల);
	తిరిగి 0;
}
EXPORT_చిహ్నం_GPL(కూడు_wait_వరుస_ప్రాధాన్యత_exclusive);

శూన్య remove_wait_వరుస(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు)
{
	గుర్తులేని దీర్ఘ flags;

	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	__remove_wait_queue(wq_తల, wq_నమోదు);
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);
}
EXPORT_చిహ్నం(remove_wait_వరుస);

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
స్థిర పూర్ణాంకం __wake_up_common(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం,
			పూర్ణాంకం nr_exclusive, పూర్ణాంకం wake_flags, శూన్య *కీలకం)
{
	wait_వరుస_నమోదు_t *curr, *తదుపరి;

	lockdep_assert_held(&wq_తల->తాళం);

	curr = list_మొదటి_నమోదు(&wq_తల->తల, wait_వరుస_నమోదు_t, నమోదు_2);

	ఒకవేళ (&curr->నమోదు_2 == &wq_తల->తల)
		తిరిగి nr_exclusive;

	list_for_each_నమోదు_సురక్షిత_from(curr, తదుపరి, &wq_తల->తల, నమోదు_2) {
		గుర్తులేని flags = curr->flags;
		పూర్ణాంకం ret;

		ret = curr->func(curr, బహుళకం, wake_flags, కీలకం);
		ఒకవేళ (ret < 0)
			ఆపు;
		ఒకవేళ (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			ఆపు;
	}

	తిరిగి nr_exclusive;
}

స్థిర పూర్ణాంకం __wake_up_common_lock(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం,
			పూర్ణాంకం nr_exclusive, పూర్ణాంకం wake_flags, శూన్య *కీలకం)
{
	గుర్తులేని దీర్ఘ flags;
	పూర్ణాంకం మిగిలిన;

	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	మిగిలిన = __wake_up_common(wq_తల, బహుళకం, nr_exclusive, wake_flags,
			కీలకం);
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);

	తిరిగి nr_exclusive - మిగిలిన;
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
పూర్ణాంకం __wake_up(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం,
	      పూర్ణాంకం nr_exclusive, శూన్య *కీలకం)
{
	తిరిగి __wake_up_common_lock(wq_తల, బహుళకం, nr_exclusive, 0, కీలకం);
}
EXPORT_చిహ్నం(__wake_up);

శూన్య __wake_up_on_current_cpu(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం, శూన్య *కీలకం)
{
	__wake_up_common_lock(wq_తల, బహుళకం, 1, WF_CURRENT_CPU, కీలకం);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
శూన్య __wake_up_locked(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం, పూర్ణాంకం nr)
{
	__wake_up_common(wq_తల, బహుళకం, nr, 0, ఖాళీ);
}
EXPORT_చిహ్నం_GPL(__wake_up_locked);

శూన్య __wake_up_locked_key(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం, శూన్య *కీలకం)
{
	__wake_up_common(wq_తల, బహుళకం, 1, 0, కీలకం);
}
EXPORT_చిహ్నం_GPL(__wake_up_locked_key);

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
శూన్య __wake_up_sync_key(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం,
			శూన్య *కీలకం)
{
	ఒకవేళ (unlikely(!wq_తల))
		తిరిగి;

	__wake_up_common_lock(wq_తల, బహుళకం, 1, WF_SYNC, కీలకం);
}
EXPORT_చిహ్నం_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
శూన్య __wake_up_locked_sync_key(నిర్మాణం wait_వరుస_తల *wq_తల,
			       గుర్తులేని పూర్ణాంకం బహుళకం, శూన్య *కీలకం)
{
        __wake_up_common(wq_తల, బహుళకం, 1, WF_SYNC, కీలకం);
}
EXPORT_చిహ్నం_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
శూన్య __wake_up_sync(నిర్మాణం wait_వరుస_తల *wq_తల, గుర్తులేని పూర్ణాంకం బహుళకం)
{
	__wake_up_sync_key(wq_తల, బహుళకం, ఖాళీ);
}
EXPORT_చిహ్నం_GPL(__wake_up_sync);	/* For internal use only */

శూన్య __wake_up_pollfree(నిర్మాణం wait_వరుస_తల *wq_తల)
{
	__wake_up(wq_తల, TASK_సాధారణ, 0, poll_గమ్యం_కీలకం(EPOLLHUP | POLLFREE));
	/* POLLFREE must have cleared the queue. */
	WARN_ON_ONCE(waitqueue_చురుకైన(wq_తల));
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
శూన్య
prepare_గమ్యం_wait(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, పూర్ణాంకం state)
{
	గుర్తులేని దీర్ఘ flags;

	wq_నమోదు->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	ఒకవేళ (list_ఖాళీలు(&wq_నమోదు->నమోదు_2))
		__add_wait_queue(wq_తల, wq_నమోదు);
	సమితి_current_state(state);
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);
}
EXPORT_చిహ్నం(prepare_గమ్యం_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_గమ్యం_wait_exclusive(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, పూర్ణాంకం state)
{
	గుర్తులేని దీర్ఘ flags;
	bool was_ఖాళీలు = false;

	wq_నమోదు->flags |= WQ_FLAG_EXCLUSIVE;
	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	ఒకవేళ (list_ఖాళీలు(&wq_నమోదు->నమోదు_2)) {
		was_ఖాళీలు = list_ఖాళీలు(&wq_తల->తల);
		__add_wait_queue_entry_tail(wq_తల, wq_నమోదు);
	}
	సమితి_current_state(state);
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);
	తిరిగి was_ఖాళీలు;
}
EXPORT_చిహ్నం(prepare_గమ్యం_wait_exclusive);

శూన్య init_wait_నమోదు(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, పూర్ణాంకం flags)
{
	wq_నమోదు->flags = flags;
	wq_నమోదు->అంతరంగ = current;
	wq_నమోదు->func = autoremove_wake_function;
	INIT_LIST_తల(&wq_నమోదు->నమోదు_2);
}
EXPORT_చిహ్నం(init_wait_నమోదు);

దీర్ఘ prepare_గమ్యం_wait_event(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, పూర్ణాంకం state)
{
	గుర్తులేని దీర్ఘ flags;
	దీర్ఘ ret = 0;

	spin_తాళం_irqsave(&wq_తల->తాళం, flags);
	ఒకవేళ (signal_pending_state(state, current)) {
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
		list_del_init(&wq_నమోదు->నమోదు_2);
		ret = -ERESTARTSYS;
	} లేకపోతే {
		ఒకవేళ (list_ఖాళీలు(&wq_నమోదు->నమోదు_2)) {
			ఒకవేళ (wq_నమోదు->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_తల, wq_నమోదు);
			లేకపోతే
				__add_wait_queue(wq_తల, wq_నమోదు);
		}
		సమితి_current_state(state);
	}
	spin_unlock_irqrestore(&wq_తల->తాళం, flags);

	తిరిగి ret;
}
EXPORT_చిహ్నం(prepare_గమ్యం_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
పూర్ణాంకం do_wait_intr(wait_వరుస_తల_t *wq, wait_వరుస_నమోదు_t *wait)
{
	ఒకవేళ (likely(list_ఖాళీలు(&wait->నమోదు_2)))
		__add_wait_queue_entry_tail(wq, wait);

	సమితి_current_state(TASK_INTERRUPTIBLE);
	ఒకవేళ (signal_pending(current))
		తిరిగి -ERESTARTSYS;

	spin_unlock(&wq->తాళం);
	కాలపట్టిక();
	spin_తాళం(&wq->తాళం);

	తిరిగి 0;
}
EXPORT_చిహ్నం(do_wait_intr);

పూర్ణాంకం do_wait_intr_irq(wait_వరుస_తల_t *wq, wait_వరుస_నమోదు_t *wait)
{
	ఒకవేళ (likely(list_ఖాళీలు(&wait->నమోదు_2)))
		__add_wait_queue_entry_tail(wq, wait);

	సమితి_current_state(TASK_INTERRUPTIBLE);
	ఒకవేళ (signal_pending(current))
		తిరిగి -ERESTARTSYS;

	spin_unlock_irq(&wq->తాళం);
	కాలపట్టిక();
	spin_తాళం_irq(&wq->తాళం);

	తిరిగి 0;
}
EXPORT_చిహ్నం(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
శూన్య ముగింపు_wait(నిర్మాణం wait_వరుస_తల *wq_తల, నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు)
{
	గుర్తులేని దీర్ఘ flags;

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
	ఒకవేళ (!list_ఖాళీలు_careful(&wq_నమోదు->నమోదు_2)) {
		spin_తాళం_irqsave(&wq_తల->తాళం, flags);
		list_del_init(&wq_నమోదు->నమోదు_2);
		spin_unlock_irqrestore(&wq_తల->తాళం, flags);
	}
}
EXPORT_చిహ్నం(ముగింపు_wait);

పూర్ణాంకం autoremove_wake_function(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, గుర్తులేని బహుళకం, పూర్ణాంకం sync, శూన్య *కీలకం)
{
	పూర్ణాంకం ret = default_wake_function(wq_నమోదు, బహుళకం, sync, కీలకం);

	ఒకవేళ (ret)
		list_del_init_careful(&wq_నమోదు->నమోదు_2);

	తిరిగి ret;
}
EXPORT_చిహ్నం(autoremove_wake_function);

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
దీర్ఘ wait_woken(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, గుర్తులేని బహుళకం, దీర్ఘ timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	సమితి_current_state(బహుళకం); /* A */
	ఒకవేళ (!(wq_నమోదు->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = కాలపట్టిక_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_నమోదు->flags, wq_నమోదు->flags & ~WQ_FLAG_WOKEN); /* B */

	తిరిగి timeout;
}
EXPORT_చిహ్నం(wait_woken);

పూర్ణాంకం woken_wake_function(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, గుర్తులేని బహుళకం, పూర్ణాంకం sync, శూన్య *కీలకం)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_నమోదు->flags |= WQ_FLAG_WOKEN;

	తిరిగి default_wake_function(wq_నమోదు, బహుళకం, sync, కీలకం);
}
EXPORT_చిహ్నం(woken_wake_function);

పూర్ణాంకం woken_wake_బిట్టు_function(నిర్మాణం wait_వరుస_నమోదు *wq_నమోదు, గుర్తులేని బహుళకం, పూర్ణాంకం sync, శూన్య *arg)
{
	నిర్మాణం wait_బిట్టు_కీలకం *కీలకం = __var_wake_key(wq_నమోదు, arg);
	ఒకవేళ (!కీలకం)
		తిరిగి 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_నమోదు->flags |= WQ_FLAG_WOKEN;

	తిరిగి default_wake_function(wq_నమోదు, బహుళకం, sync, కీలకం);
}
EXPORT_చిహ్నం(woken_wake_బిట్టు_function);
