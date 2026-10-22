// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#अन्तर्भाव "sched.h"
#अन्तर्भाव <linux/wait_bit.h>

शून्य __init_waitqueue_head(संरचना wait_queue_शीर्ष *wq_शीर्ष, ध्रुव वर्ण *name, संरचना तालकम्_class_कुञ्चिका *कुञ्चिका)
{
	spin_तालकम्_init(&wq_शीर्ष->तालकम्);
	lockdep_समुच्चयः_class_and_name(&wq_शीर्ष->तालकम्, कुञ्चिका, name);
	INIT_LIST_शीर्ष(&wq_शीर्ष->शीर्ष);
}

EXPORT_चिह्नम्(__init_waitqueue_head);

शून्य सङ्कलय_wait_queue(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः)
{
	अचिह्नित दीर्घ flags;

	wq_प्रविष्टिः->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	__add_wait_queue(wq_शीर्ष, wq_प्रविष्टिः);
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);
}
EXPORT_चिह्नम्(सङ्कलय_wait_queue);

शून्य सङ्कलय_wait_queue_exclusive(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः)
{
	अचिह्नित दीर्घ flags;

	wq_प्रविष्टिः->flags |= WQ_FLAG_EXCLUSIVE;
	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	__add_wait_queue_entry_tail(wq_शीर्ष, wq_प्रविष्टिः);
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);
}
EXPORT_चिह्नम्(सङ्कलय_wait_queue_exclusive);

शून्य सङ्कलय_wait_queue_प्राथमिकता(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः)
{
	अचिह्नित दीर्घ flags;

	wq_प्रविष्टिः->flags |= WQ_FLAG_प्राथमिकता;
	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	__add_wait_queue(wq_शीर्ष, wq_प्रविष्टिः);
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);
}
EXPORT_चिह्नम्_GPL(सङ्कलय_wait_queue_प्राथमिकता);

पूर्णाङ्क सङ्कलय_wait_queue_प्राथमिकता_exclusive(संरचना wait_queue_शीर्ष *wq_शीर्ष,
				      संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः)
{
	संरचना list_शीर्ष *शीर्ष = &wq_शीर्ष->शीर्ष;

	guard(spinlock_irqsave)(&wq_शीर्ष->तालकम्);

	यदि (!list_रिक्तानि(शीर्ष) &&
	    (list_first_प्रविष्टिः(शीर्ष, typeof(*wq_प्रविष्टिः), प्रविष्टिः)->flags & WQ_FLAG_प्राथमिकता))
		प्रत्यागम -EBUSY;

	wq_प्रविष्टिः->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_प्राथमिकता;
	list_सङ्कलय(&wq_प्रविष्टिः->प्रविष्टिः, शीर्ष);
	प्रत्यागम 0;
}
EXPORT_चिह्नम्_GPL(सङ्कलय_wait_queue_प्राथमिकता_exclusive);

शून्य remove_wait_queue(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः)
{
	अचिह्नित दीर्घ flags;

	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	__remove_wait_queue(wq_शीर्ष, wq_प्रविष्टिः);
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);
}
EXPORT_चिह्नम्(remove_wait_queue);

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
स्थिर पूर्णाङ्क __wake_up_common(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्,
			पूर्णाङ्क nr_exclusive, पूर्णाङ्क wake_flags, शून्य *कुञ्चिका)
{
	wait_queue_प्रविष्टिः_t *curr, *अग्र;

	lockdep_assert_held(&wq_शीर्ष->तालकम्);

	curr = list_first_प्रविष्टिः(&wq_शीर्ष->शीर्ष, wait_queue_प्रविष्टिः_t, प्रविष्टिः);

	यदि (&curr->प्रविष्टिः == &wq_शीर्ष->शीर्ष)
		प्रत्यागम nr_exclusive;

	list_for_each_प्रविष्टिः_safe_from(curr, अग्र, &wq_शीर्ष->शीर्ष, प्रविष्टिः) {
		अचिह्नित flags = curr->flags;
		पूर्णाङ्क ret;

		ret = curr->func(curr, बहुलकम्, wake_flags, कुञ्चिका);
		यदि (ret < 0)
			भङ्ग;
		यदि (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			भङ्ग;
	}

	प्रत्यागम nr_exclusive;
}

स्थिर पूर्णाङ्क __wake_up_common_lock(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्,
			पूर्णाङ्क nr_exclusive, पूर्णाङ्क wake_flags, शून्य *कुञ्चिका)
{
	अचिह्नित दीर्घ flags;
	पूर्णाङ्क remaining;

	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	remaining = __wake_up_common(wq_शीर्ष, बहुलकम्, nr_exclusive, wake_flags,
			कुञ्चिका);
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);

	प्रत्यागम nr_exclusive - remaining;
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
पूर्णाङ्क __wake_up(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्,
	      पूर्णाङ्क nr_exclusive, शून्य *कुञ्चिका)
{
	प्रत्यागम __wake_up_common_lock(wq_शीर्ष, बहुलकम्, nr_exclusive, 0, कुञ्चिका);
}
EXPORT_चिह्नम्(__wake_up);

शून्य __wake_up_on_current_cpu(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्, शून्य *कुञ्चिका)
{
	__wake_up_common_lock(wq_शीर्ष, बहुलकम्, 1, WF_CURRENT_CPU, कुञ्चिका);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
शून्य __wake_up_locked(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्, पूर्णाङ्क nr)
{
	__wake_up_common(wq_शीर्ष, बहुलकम्, nr, 0, रिक्त);
}
EXPORT_चिह्नम्_GPL(__wake_up_locked);

शून्य __wake_up_locked_key(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्, शून्य *कुञ्चिका)
{
	__wake_up_common(wq_शीर्ष, बहुलकम्, 1, 0, कुञ्चिका);
}
EXPORT_चिह्नम्_GPL(__wake_up_locked_key);

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
शून्य __wake_up_sync_key(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्,
			शून्य *कुञ्चिका)
{
	यदि (unlikely(!wq_शीर्ष))
		प्रत्यागम;

	__wake_up_common_lock(wq_शीर्ष, बहुलकम्, 1, WF_SYNC, कुञ्चिका);
}
EXPORT_चिह्नम्_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
शून्य __wake_up_locked_sync_key(संरचना wait_queue_शीर्ष *wq_शीर्ष,
			       अचिह्नित पूर्णाङ्क बहुलकम्, शून्य *कुञ्चिका)
{
        __wake_up_common(wq_शीर्ष, बहुलकम्, 1, WF_SYNC, कुञ्चिका);
}
EXPORT_चिह्नम्_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
शून्य __wake_up_sync(संरचना wait_queue_शीर्ष *wq_शीर्ष, अचिह्नित पूर्णाङ्क बहुलकम्)
{
	__wake_up_sync_key(wq_शीर्ष, बहुलकम्, रिक्त);
}
EXPORT_चिह्नम्_GPL(__wake_up_sync);	/* For internal use only */

शून्य __wake_up_pollfree(संरचना wait_queue_शीर्ष *wq_शीर्ष)
{
	__wake_up(wq_शीर्ष, TASK_प्रसामान्यम्, 0, poll_गन्तव्यम्_कुञ्चिका(EPOLLHUP | POLLFREE));
	/* POLLFREE must have cleared the queue. */
	WARN_ON_ONCE(waitqueue_सक्रिय(wq_शीर्ष));
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
शून्य
prepare_गन्तव्यम्_wait(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, पूर्णाङ्क state)
{
	अचिह्नित दीर्घ flags;

	wq_प्रविष्टिः->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	यदि (list_रिक्तानि(&wq_प्रविष्टिः->प्रविष्टिः))
		__add_wait_queue(wq_शीर्ष, wq_प्रविष्टिः);
	समुच्चयः_current_state(state);
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);
}
EXPORT_चिह्नम्(prepare_गन्तव्यम्_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_गन्तव्यम्_wait_exclusive(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, पूर्णाङ्क state)
{
	अचिह्नित दीर्घ flags;
	bool was_रिक्तानि = false;

	wq_प्रविष्टिः->flags |= WQ_FLAG_EXCLUSIVE;
	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	यदि (list_रिक्तानि(&wq_प्रविष्टिः->प्रविष्टिः)) {
		was_रिक्तानि = list_रिक्तानि(&wq_शीर्ष->शीर्ष);
		__add_wait_queue_entry_tail(wq_शीर्ष, wq_प्रविष्टिः);
	}
	समुच्चयः_current_state(state);
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);
	प्रत्यागम was_रिक्तानि;
}
EXPORT_चिह्नम्(prepare_गन्तव्यम्_wait_exclusive);

शून्य init_wait_प्रविष्टिः(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, पूर्णाङ्क flags)
{
	wq_प्रविष्टिः->flags = flags;
	wq_प्रविष्टिः->गुप्त = current;
	wq_प्रविष्टिः->func = autoremove_wake_function;
	INIT_LIST_शीर्ष(&wq_प्रविष्टिः->प्रविष्टिः);
}
EXPORT_चिह्नम्(init_wait_प्रविष्टिः);

दीर्घ prepare_गन्तव्यम्_wait_event(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, पूर्णाङ्क state)
{
	अचिह्नित दीर्घ flags;
	दीर्घ ret = 0;

	spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
	यदि (signal_pending_state(state, current)) {
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
		list_del_init(&wq_प्रविष्टिः->प्रविष्टिः);
		ret = -ERESTARTSYS;
	} अन्यथा {
		यदि (list_रिक्तानि(&wq_प्रविष्टिः->प्रविष्टिः)) {
			यदि (wq_प्रविष्टिः->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_शीर्ष, wq_प्रविष्टिः);
			अन्यथा
				__add_wait_queue(wq_शीर्ष, wq_प्रविष्टिः);
		}
		समुच्चयः_current_state(state);
	}
	spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);

	प्रत्यागम ret;
}
EXPORT_चिह्नम्(prepare_गन्तव्यम्_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
पूर्णाङ्क do_wait_intr(wait_queue_शीर्ष_t *wq, wait_queue_प्रविष्टिः_t *प्रतीक्षस्व)
{
	यदि (likely(list_रिक्तानि(&प्रतीक्षस्व->प्रविष्टिः)))
		__add_wait_queue_entry_tail(wq, प्रतीक्षस्व);

	समुच्चयः_current_state(TASK_INTERRUPTIBLE);
	यदि (signal_pending(current))
		प्रत्यागम -ERESTARTSYS;

	spin_unlock(&wq->तालकम्);
	कालसारणी();
	spin_तालकम्(&wq->तालकम्);

	प्रत्यागम 0;
}
EXPORT_चिह्नम्(do_wait_intr);

पूर्णाङ्क do_wait_intr_irq(wait_queue_शीर्ष_t *wq, wait_queue_प्रविष्टिः_t *प्रतीक्षस्व)
{
	यदि (likely(list_रिक्तानि(&प्रतीक्षस्व->प्रविष्टिः)))
		__add_wait_queue_entry_tail(wq, प्रतीक्षस्व);

	समुच्चयः_current_state(TASK_INTERRUPTIBLE);
	यदि (signal_pending(current))
		प्रत्यागम -ERESTARTSYS;

	spin_unlock_irq(&wq->तालकम्);
	कालसारणी();
	spin_तालकम्_irq(&wq->तालकम्);

	प्रत्यागम 0;
}
EXPORT_चिह्नम्(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
शून्य finish_wait(संरचना wait_queue_शीर्ष *wq_शीर्ष, संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः)
{
	अचिह्नित दीर्घ flags;

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
	यदि (!list_रिक्तानि_careful(&wq_प्रविष्टिः->प्रविष्टिः)) {
		spin_तालकम्_irqsave(&wq_शीर्ष->तालकम्, flags);
		list_del_init(&wq_प्रविष्टिः->प्रविष्टिः);
		spin_unlock_irqrestore(&wq_शीर्ष->तालकम्, flags);
	}
}
EXPORT_चिह्नम्(finish_wait);

पूर्णाङ्क autoremove_wake_function(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, अचिह्नित बहुलकम्, पूर्णाङ्क sync, शून्य *कुञ्चिका)
{
	पूर्णाङ्क ret = default_wake_function(wq_प्रविष्टिः, बहुलकम्, sync, कुञ्चिका);

	यदि (ret)
		list_del_init_careful(&wq_प्रविष्टिः->प्रविष्टिः);

	प्रत्यागम ret;
}
EXPORT_चिह्नम्(autoremove_wake_function);

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
दीर्घ wait_woken(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, अचिह्नित बहुलकम्, दीर्घ timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	समुच्चयः_current_state(बहुलकम्); /* A */
	यदि (!(wq_प्रविष्टिः->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = कालसारणी_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_प्रविष्टिः->flags, wq_प्रविष्टिः->flags & ~WQ_FLAG_WOKEN); /* B */

	प्रत्यागम timeout;
}
EXPORT_चिह्नम्(wait_woken);

पूर्णाङ्क woken_wake_function(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, अचिह्नित बहुलकम्, पूर्णाङ्क sync, शून्य *कुञ्चिका)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_प्रविष्टिः->flags |= WQ_FLAG_WOKEN;

	प्रत्यागम default_wake_function(wq_प्रविष्टिः, बहुलकम्, sync, कुञ्चिका);
}
EXPORT_चिह्नम्(woken_wake_function);

पूर्णाङ्क woken_wake_bit_function(संरचना wait_queue_प्रविष्टिः *wq_प्रविष्टिः, अचिह्नित बहुलकम्, पूर्णाङ्क sync, शून्य *arg)
{
	संरचना wait_bit_कुञ्चिका *कुञ्चिका = __var_wake_key(wq_प्रविष्टिः, arg);
	यदि (!कुञ्चिका)
		प्रत्यागम 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_प्रविष्टिः->flags |= WQ_FLAG_WOKEN;

	प्रत्यागम default_wake_function(wq_प्रविष्टिः, बहुलकम्, sync, कुञ्चिका);
}
EXPORT_चिह्नम्(woken_wake_bit_function);
