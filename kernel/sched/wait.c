// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#ضمن "sched.h"
#ضمن <linux/wait_bit.h>

فراغ __init_waitqueue_head(بنية wait_صف_سر *wq_سر, ثابت حرف *name, بنية قفل_class_کلید *کلید)
{
	spin_قفل_init(&wq_سر->قفل);
	lockdep_مجموعه_class_and_name(&wq_سر->قفل, کلید, name);
	INIT_LIST_سر(&wq_سر->سر);
}

EXPORT_نماد(__init_waitqueue_head);

فراغ جمع_کن_wait_صف(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل)
{
	بیعلامت دراز flags;

	wq_مدخل->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_قفل_irqsave(&wq_سر->قفل, flags);
	__add_wait_queue(wq_سر, wq_مدخل);
	spin_unlock_irqrestore(&wq_سر->قفل, flags);
}
EXPORT_نماد(جمع_کن_wait_صف);

فراغ جمع_کن_wait_صف_exclusive(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل)
{
	بیعلامت دراز flags;

	wq_مدخل->flags |= WQ_FLAG_EXCLUSIVE;
	spin_قفل_irqsave(&wq_سر->قفل, flags);
	__add_wait_queue_entry_tail(wq_سر, wq_مدخل);
	spin_unlock_irqrestore(&wq_سر->قفل, flags);
}
EXPORT_نماد(جمع_کن_wait_صف_exclusive);

فراغ جمع_کن_wait_صف_اولویت(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل)
{
	بیعلامت دراز flags;

	wq_مدخل->flags |= WQ_FLAG_اولویت;
	spin_قفل_irqsave(&wq_سر->قفل, flags);
	__add_wait_queue(wq_سر, wq_مدخل);
	spin_unlock_irqrestore(&wq_سر->قفل, flags);
}
EXPORT_نماد_GPL(جمع_کن_wait_صف_اولویت);

صحيح جمع_کن_wait_صف_اولویت_exclusive(بنية wait_صف_سر *wq_سر,
				      بنية wait_صف_مدخل *wq_مدخل)
{
	بنية list_سر *سر = &wq_سر->سر;

	guard(spinlock_irqsave)(&wq_سر->قفل);

	إذا (!list_خالی(سر) &&
	    (list_اولی_مدخل(سر, typeof(*wq_مدخل), مدخل)->flags & WQ_FLAG_اولویت))
		أرجع -EBUSY;

	wq_مدخل->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_اولویت;
	list_جمع_کن(&wq_مدخل->مدخل, سر);
	أرجع 0;
}
EXPORT_نماد_GPL(جمع_کن_wait_صف_اولویت_exclusive);

فراغ remove_wait_صف(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل)
{
	بیعلامت دراز flags;

	spin_قفل_irqsave(&wq_سر->قفل, flags);
	__remove_wait_queue(wq_سر, wq_مدخل);
	spin_unlock_irqrestore(&wq_سر->قفل, flags);
}
EXPORT_نماد(remove_wait_صف);

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
ساكن صحيح __wake_up_common(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما,
			صحيح nr_exclusive, صحيح wake_flags, فراغ *کلید)
{
	wait_صف_مدخل_t *curr, *بعدی;

	lockdep_assert_held(&wq_سر->قفل);

	curr = list_اولی_مدخل(&wq_سر->سر, wait_صف_مدخل_t, مدخل);

	إذا (&curr->مدخل == &wq_سر->سر)
		أرجع nr_exclusive;

	list_for_each_مدخل_امن_from(curr, بعدی, &wq_سر->سر, مدخل) {
		بیعلامت flags = curr->flags;
		صحيح ret;

		ret = curr->func(curr, نما, wake_flags, کلید);
		إذا (ret < 0)
			اكسر;
		إذا (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			اكسر;
	}

	أرجع nr_exclusive;
}

ساكن صحيح __wake_up_common_lock(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما,
			صحيح nr_exclusive, صحيح wake_flags, فراغ *کلید)
{
	بیعلامت دراز flags;
	صحيح مانده;

	spin_قفل_irqsave(&wq_سر->قفل, flags);
	مانده = __wake_up_common(wq_سر, نما, nr_exclusive, wake_flags,
			کلید);
	spin_unlock_irqrestore(&wq_سر->قفل, flags);

	أرجع nr_exclusive - مانده;
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
صحيح __wake_up(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما,
	      صحيح nr_exclusive, فراغ *کلید)
{
	أرجع __wake_up_common_lock(wq_سر, نما, nr_exclusive, 0, کلید);
}
EXPORT_نماد(__wake_up);

فراغ __wake_up_on_current_cpu(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما, فراغ *کلید)
{
	__wake_up_common_lock(wq_سر, نما, 1, WF_CURRENT_CPU, کلید);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
فراغ __wake_up_locked(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما, صحيح nr)
{
	__wake_up_common(wq_سر, نما, nr, 0, عدم);
}
EXPORT_نماد_GPL(__wake_up_locked);

فراغ __wake_up_locked_key(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما, فراغ *کلید)
{
	__wake_up_common(wq_سر, نما, 1, 0, کلید);
}
EXPORT_نماد_GPL(__wake_up_locked_key);

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
فراغ __wake_up_sync_key(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما,
			فراغ *کلید)
{
	إذا (unlikely(!wq_سر))
		أرجع;

	__wake_up_common_lock(wq_سر, نما, 1, WF_SYNC, کلید);
}
EXPORT_نماد_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
فراغ __wake_up_locked_sync_key(بنية wait_صف_سر *wq_سر,
			       بیعلامت صحيح نما, فراغ *کلید)
{
        __wake_up_common(wq_سر, نما, 1, WF_SYNC, کلید);
}
EXPORT_نماد_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
فراغ __wake_up_sync(بنية wait_صف_سر *wq_سر, بیعلامت صحيح نما)
{
	__wake_up_sync_key(wq_سر, نما, عدم);
}
EXPORT_نماد_GPL(__wake_up_sync);	/* For internal use only */

فراغ __wake_up_pollfree(بنية wait_صف_سر *wq_سر)
{
	__wake_up(wq_سر, TASK_نرمال, 0, poll_مقصد_کلید(EPOLLHUP | POLLFREE));
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
prepare_مقصد_wait(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل, صحيح state)
{
	بیعلامت دراز flags;

	wq_مدخل->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_قفل_irqsave(&wq_سر->قفل, flags);
	إذا (list_خالی(&wq_مدخل->مدخل))
		__add_wait_queue(wq_سر, wq_مدخل);
	مجموعه_current_state(state);
	spin_unlock_irqrestore(&wq_سر->قفل, flags);
}
EXPORT_نماد(prepare_مقصد_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_مقصد_wait_exclusive(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل, صحيح state)
{
	بیعلامت دراز flags;
	bool was_خالی = false;

	wq_مدخل->flags |= WQ_FLAG_EXCLUSIVE;
	spin_قفل_irqsave(&wq_سر->قفل, flags);
	إذا (list_خالی(&wq_مدخل->مدخل)) {
		was_خالی = list_خالی(&wq_سر->سر);
		__add_wait_queue_entry_tail(wq_سر, wq_مدخل);
	}
	مجموعه_current_state(state);
	spin_unlock_irqrestore(&wq_سر->قفل, flags);
	أرجع was_خالی;
}
EXPORT_نماد(prepare_مقصد_wait_exclusive);

فراغ init_wait_مدخل(بنية wait_صف_مدخل *wq_مدخل, صحيح flags)
{
	wq_مدخل->flags = flags;
	wq_مدخل->خاص = current;
	wq_مدخل->func = autoremove_wake_function;
	INIT_LIST_سر(&wq_مدخل->مدخل);
}
EXPORT_نماد(init_wait_مدخل);

دراز prepare_مقصد_wait_event(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل, صحيح state)
{
	بیعلامت دراز flags;
	دراز ret = 0;

	spin_قفل_irqsave(&wq_سر->قفل, flags);
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
		list_del_init(&wq_مدخل->مدخل);
		ret = -ERESTARTSYS;
	} وإلا {
		إذا (list_خالی(&wq_مدخل->مدخل)) {
			إذا (wq_مدخل->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_سر, wq_مدخل);
			وإلا
				__add_wait_queue(wq_سر, wq_مدخل);
		}
		مجموعه_current_state(state);
	}
	spin_unlock_irqrestore(&wq_سر->قفل, flags);

	أرجع ret;
}
EXPORT_نماد(prepare_مقصد_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
صحيح do_wait_intr(wait_صف_سر_t *wq, wait_صف_مدخل_t *wait)
{
	إذا (likely(list_خالی(&wait->مدخل)))
		__add_wait_queue_entry_tail(wq, wait);

	مجموعه_current_state(TASK_INTERRUPTIBLE);
	إذا (signal_pending(current))
		أرجع -ERESTARTSYS;

	spin_unlock(&wq->قفل);
	زمانبندی();
	spin_قفل(&wq->قفل);

	أرجع 0;
}
EXPORT_نماد(do_wait_intr);

صحيح do_wait_intr_irq(wait_صف_سر_t *wq, wait_صف_مدخل_t *wait)
{
	إذا (likely(list_خالی(&wait->مدخل)))
		__add_wait_queue_entry_tail(wq, wait);

	مجموعه_current_state(TASK_INTERRUPTIBLE);
	إذا (signal_pending(current))
		أرجع -ERESTARTSYS;

	spin_unlock_irq(&wq->قفل);
	زمانبندی();
	spin_قفل_irq(&wq->قفل);

	أرجع 0;
}
EXPORT_نماد(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
فراغ پایان_wait(بنية wait_صف_سر *wq_سر, بنية wait_صف_مدخل *wq_مدخل)
{
	بیعلامت دراز flags;

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
	إذا (!list_خالی_careful(&wq_مدخل->مدخل)) {
		spin_قفل_irqsave(&wq_سر->قفل, flags);
		list_del_init(&wq_مدخل->مدخل);
		spin_unlock_irqrestore(&wq_سر->قفل, flags);
	}
}
EXPORT_نماد(پایان_wait);

صحيح autoremove_wake_function(بنية wait_صف_مدخل *wq_مدخل, بیعلامت نما, صحيح sync, فراغ *کلید)
{
	صحيح ret = default_wake_function(wq_مدخل, نما, sync, کلید);

	إذا (ret)
		list_del_init_careful(&wq_مدخل->مدخل);

	أرجع ret;
}
EXPORT_نماد(autoremove_wake_function);

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
دراز wait_woken(بنية wait_صف_مدخل *wq_مدخل, بیعلامت نما, دراز timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	مجموعه_current_state(نما); /* A */
	إذا (!(wq_مدخل->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = زمانبندی_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_مدخل->flags, wq_مدخل->flags & ~WQ_FLAG_WOKEN); /* B */

	أرجع timeout;
}
EXPORT_نماد(wait_woken);

صحيح woken_wake_function(بنية wait_صف_مدخل *wq_مدخل, بیعلامت نما, صحيح sync, فراغ *کلید)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_مدخل->flags |= WQ_FLAG_WOKEN;

	أرجع default_wake_function(wq_مدخل, نما, sync, کلید);
}
EXPORT_نماد(woken_wake_function);

صحيح woken_wake_بیت_function(بنية wait_صف_مدخل *wq_مدخل, بیعلامت نما, صحيح sync, فراغ *arg)
{
	بنية wait_بیت_کلید *کلید = __var_wake_key(wq_مدخل, arg);
	إذا (!کلید)
		أرجع 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_مدخل->flags |= WQ_FLAG_WOKEN;

	أرجع default_wake_function(wq_مدخل, نما, sync, کلید);
}
EXPORT_نماد(woken_wake_بیت_function);
