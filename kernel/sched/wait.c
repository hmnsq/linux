// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#ضمن "sched.h"
#ضمن <linux/wait_bit.h>

فراغ __init_waitqueue_head(بنية wait_کتار_سر *wq_سر, ثابت حرف *name, بنية قلف_class_کلی *کلی)
{
	spin_قلف_init(&wq_سر->قلف);
	lockdep_سېټ_class_and_name(&wq_سر->قلف, کلی, name);
	INIT_LIST_سر(&wq_سر->سر);
}

EXPORT_نښه(__init_waitqueue_head);

فراغ جمع_کړه_wait_کتار(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت)
{
	بینښه اوږد flags;

	wq_ثبت->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_قلف_irqsave(&wq_سر->قلف, flags);
	__add_wait_queue(wq_سر, wq_ثبت);
	spin_unlock_irqrestore(&wq_سر->قلف, flags);
}
EXPORT_نښه(جمع_کړه_wait_کتار);

فراغ جمع_کړه_wait_کتار_exclusive(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت)
{
	بینښه اوږد flags;

	wq_ثبت->flags |= WQ_FLAG_EXCLUSIVE;
	spin_قلف_irqsave(&wq_سر->قلف, flags);
	__add_wait_queue_entry_tail(wq_سر, wq_ثبت);
	spin_unlock_irqrestore(&wq_سر->قلف, flags);
}
EXPORT_نښه(جمع_کړه_wait_کتار_exclusive);

فراغ جمع_کړه_wait_کتار_لومړیتوب(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت)
{
	بینښه اوږد flags;

	wq_ثبت->flags |= WQ_FLAG_لومړیتوب;
	spin_قلف_irqsave(&wq_سر->قلف, flags);
	__add_wait_queue(wq_سر, wq_ثبت);
	spin_unlock_irqrestore(&wq_سر->قلف, flags);
}
EXPORT_نښه_GPL(جمع_کړه_wait_کتار_لومړیتوب);

صحيح جمع_کړه_wait_کتار_لومړیتوب_exclusive(بنية wait_کتار_سر *wq_سر,
				      بنية wait_کتار_ثبت *wq_ثبت)
{
	بنية list_سر *سر = &wq_سر->سر;

	guard(spinlock_irqsave)(&wq_سر->قلف);

	إذا (!list_تشه(سر) &&
	    (list_لومړی_ثبت(سر, typeof(*wq_ثبت), ثبت)->flags & WQ_FLAG_لومړیتوب))
		أرجع -EBUSY;

	wq_ثبت->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_لومړیتوب;
	list_جمع_کړه(&wq_ثبت->ثبت, سر);
	أرجع 0;
}
EXPORT_نښه_GPL(جمع_کړه_wait_کتار_لومړیتوب_exclusive);

فراغ remove_wait_کتار(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت)
{
	بینښه اوږد flags;

	spin_قلف_irqsave(&wq_سر->قلف, flags);
	__remove_wait_queue(wq_سر, wq_ثبت);
	spin_unlock_irqrestore(&wq_سر->قلف, flags);
}
EXPORT_نښه(remove_wait_کتار);

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
ساكن صحيح __wake_up_common(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ,
			صحيح nr_exclusive, صحيح wake_flags, فراغ *کلی)
{
	wait_کتار_ثبت_t *curr, *بل;

	lockdep_assert_held(&wq_سر->قلف);

	curr = list_لومړی_ثبت(&wq_سر->سر, wait_کتار_ثبت_t, ثبت);

	إذا (&curr->ثبت == &wq_سر->سر)
		أرجع nr_exclusive;

	list_for_each_ثبت_خوندي_from(curr, بل, &wq_سر->سر, ثبت) {
		بینښه flags = curr->flags;
		صحيح ret;

		ret = curr->func(curr, ډېرپېښ, wake_flags, کلی);
		إذا (ret < 0)
			اكسر;
		إذا (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			اكسر;
	}

	أرجع nr_exclusive;
}

ساكن صحيح __wake_up_common_lock(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ,
			صحيح nr_exclusive, صحيح wake_flags, فراغ *کلی)
{
	بینښه اوږد flags;
	صحيح پاتې_شوی;

	spin_قلف_irqsave(&wq_سر->قلف, flags);
	پاتې_شوی = __wake_up_common(wq_سر, ډېرپېښ, nr_exclusive, wake_flags,
			کلی);
	spin_unlock_irqrestore(&wq_سر->قلف, flags);

	أرجع nr_exclusive - پاتې_شوی;
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
صحيح __wake_up(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ,
	      صحيح nr_exclusive, فراغ *کلی)
{
	أرجع __wake_up_common_lock(wq_سر, ډېرپېښ, nr_exclusive, 0, کلی);
}
EXPORT_نښه(__wake_up);

فراغ __wake_up_on_current_cpu(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ, فراغ *کلی)
{
	__wake_up_common_lock(wq_سر, ډېرپېښ, 1, WF_CURRENT_CPU, کلی);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
فراغ __wake_up_locked(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ, صحيح nr)
{
	__wake_up_common(wq_سر, ډېرپېښ, nr, 0, عدم);
}
EXPORT_نښه_GPL(__wake_up_locked);

فراغ __wake_up_locked_key(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ, فراغ *کلی)
{
	__wake_up_common(wq_سر, ډېرپېښ, 1, 0, کلی);
}
EXPORT_نښه_GPL(__wake_up_locked_key);

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
فراغ __wake_up_sync_key(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ,
			فراغ *کلی)
{
	إذا (unlikely(!wq_سر))
		أرجع;

	__wake_up_common_lock(wq_سر, ډېرپېښ, 1, WF_SYNC, کلی);
}
EXPORT_نښه_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
فراغ __wake_up_locked_sync_key(بنية wait_کتار_سر *wq_سر,
			       بینښه صحيح ډېرپېښ, فراغ *کلی)
{
        __wake_up_common(wq_سر, ډېرپېښ, 1, WF_SYNC, کلی);
}
EXPORT_نښه_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
فراغ __wake_up_sync(بنية wait_کتار_سر *wq_سر, بینښه صحيح ډېرپېښ)
{
	__wake_up_sync_key(wq_سر, ډېرپېښ, عدم);
}
EXPORT_نښه_GPL(__wake_up_sync);	/* For internal use only */

فراغ __wake_up_pollfree(بنية wait_کتار_سر *wq_سر)
{
	__wake_up(wq_سر, TASK_نورمال, 0, poll_منزل_کلی(EPOLLHUP | POLLFREE));
	/* POLLFREE must have cleared the queue. */
	WARN_ON_ONCE(waitqueue_فعال(wq_سر));
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
فراغ
prepare_منزل_wait(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت, صحيح state)
{
	بینښه اوږد flags;

	wq_ثبت->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_قلف_irqsave(&wq_سر->قلف, flags);
	إذا (list_تشه(&wq_ثبت->ثبت))
		__add_wait_queue(wq_سر, wq_ثبت);
	سېټ_current_state(state);
	spin_unlock_irqrestore(&wq_سر->قلف, flags);
}
EXPORT_نښه(prepare_منزل_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_منزل_wait_exclusive(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت, صحيح state)
{
	بینښه اوږد flags;
	bool was_تشه = false;

	wq_ثبت->flags |= WQ_FLAG_EXCLUSIVE;
	spin_قلف_irqsave(&wq_سر->قلف, flags);
	إذا (list_تشه(&wq_ثبت->ثبت)) {
		was_تشه = list_تشه(&wq_سر->سر);
		__add_wait_queue_entry_tail(wq_سر, wq_ثبت);
	}
	سېټ_current_state(state);
	spin_unlock_irqrestore(&wq_سر->قلف, flags);
	أرجع was_تشه;
}
EXPORT_نښه(prepare_منزل_wait_exclusive);

فراغ init_wait_ثبت(بنية wait_کتار_ثبت *wq_ثبت, صحيح flags)
{
	wq_ثبت->flags = flags;
	wq_ثبت->خاص = current;
	wq_ثبت->func = autoremove_wake_function;
	INIT_LIST_سر(&wq_ثبت->ثبت);
}
EXPORT_نښه(init_wait_ثبت);

اوږد prepare_منزل_wait_event(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت, صحيح state)
{
	بینښه اوږد flags;
	اوږد ret = 0;

	spin_قلف_irqsave(&wq_سر->قلف, flags);
	إذا (signal_pending_state(state, current)) {
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
		list_del_init(&wq_ثبت->ثبت);
		ret = -ERESTARTSYS;
	} وإلا {
		إذا (list_تشه(&wq_ثبت->ثبت)) {
			إذا (wq_ثبت->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_سر, wq_ثبت);
			وإلا
				__add_wait_queue(wq_سر, wq_ثبت);
		}
		سېټ_current_state(state);
	}
	spin_unlock_irqrestore(&wq_سر->قلف, flags);

	أرجع ret;
}
EXPORT_نښه(prepare_منزل_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
صحيح do_wait_intr(wait_کتار_سر_t *wq, wait_کتار_ثبت_t *wait)
{
	إذا (likely(list_تشه(&wait->ثبت)))
		__add_wait_queue_entry_tail(wq, wait);

	سېټ_current_state(TASK_INTERRUPTIBLE);
	إذا (signal_pending(current))
		أرجع -ERESTARTSYS;

	spin_unlock(&wq->قلف);
	مهالوېش();
	spin_قلف(&wq->قلف);

	أرجع 0;
}
EXPORT_نښه(do_wait_intr);

صحيح do_wait_intr_irq(wait_کتار_سر_t *wq, wait_کتار_ثبت_t *wait)
{
	إذا (likely(list_تشه(&wait->ثبت)))
		__add_wait_queue_entry_tail(wq, wait);

	سېټ_current_state(TASK_INTERRUPTIBLE);
	إذا (signal_pending(current))
		أرجع -ERESTARTSYS;

	spin_unlock_irq(&wq->قلف);
	مهالوېش();
	spin_قلف_irq(&wq->قلف);

	أرجع 0;
}
EXPORT_نښه(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
فراغ پای_wait(بنية wait_کتار_سر *wq_سر, بنية wait_کتار_ثبت *wq_ثبت)
{
	بینښه اوږد flags;

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
	إذا (!list_تشه_careful(&wq_ثبت->ثبت)) {
		spin_قلف_irqsave(&wq_سر->قلف, flags);
		list_del_init(&wq_ثبت->ثبت);
		spin_unlock_irqrestore(&wq_سر->قلف, flags);
	}
}
EXPORT_نښه(پای_wait);

صحيح autoremove_wake_function(بنية wait_کتار_ثبت *wq_ثبت, بینښه ډېرپېښ, صحيح sync, فراغ *کلی)
{
	صحيح ret = default_wake_function(wq_ثبت, ډېرپېښ, sync, کلی);

	إذا (ret)
		list_del_init_careful(&wq_ثبت->ثبت);

	أرجع ret;
}
EXPORT_نښه(autoremove_wake_function);

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
اوږد wait_woken(بنية wait_کتار_ثبت *wq_ثبت, بینښه ډېرپېښ, اوږد timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	سېټ_current_state(ډېرپېښ); /* A */
	إذا (!(wq_ثبت->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = مهالوېش_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_ثبت->flags, wq_ثبت->flags & ~WQ_FLAG_WOKEN); /* B */

	أرجع timeout;
}
EXPORT_نښه(wait_woken);

صحيح woken_wake_function(بنية wait_کتار_ثبت *wq_ثبت, بینښه ډېرپېښ, صحيح sync, فراغ *کلی)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_ثبت->flags |= WQ_FLAG_WOKEN;

	أرجع default_wake_function(wq_ثبت, ډېرپېښ, sync, کلی);
}
EXPORT_نښه(woken_wake_function);

صحيح woken_wake_بټ_function(بنية wait_کتار_ثبت *wq_ثبت, بینښه ډېرپېښ, صحيح sync, فراغ *arg)
{
	بنية wait_بټ_کلی *کلی = __var_wake_key(wq_ثبت, arg);
	إذا (!کلی)
		أرجع 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_ثبت->flags |= WQ_FLAG_WOKEN;

	أرجع default_wake_function(wq_ثبت, ډېرپېښ, sync, کلی);
}
EXPORT_نښه(woken_wake_بټ_function);
