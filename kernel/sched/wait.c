// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generic waiting primitives.
 *
 * (C) 2004 Nadia Yvette Chambers, Oracle
 */
#ضمن "sched.h"
#ضمن <linux/wait_bit.h>

فراغ __init_waitqueue_head(بنية wait_قطار_سر *wq_سر, ثابت حرف *name, بنية جندرا_class_کلید *کلید)
{
	spin_جندرا_init(&wq_سر->جندرا);
	lockdep_سیٹ_class_and_name(&wq_سر->جندرا, کلید, name);
	INIT_LIST_سر(&wq_سر->سر);
}

EXPORT_علامت(__init_waitqueue_head);

فراغ جوڑو_wait_قطار(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج)
{
	بنانشانی لما flags;

	wq_اندراج->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
	__add_wait_queue(wq_سر, wq_اندراج);
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);
}
EXPORT_علامت(جوڑو_wait_قطار);

فراغ جوڑو_wait_قطار_exclusive(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج)
{
	بنانشانی لما flags;

	wq_اندراج->flags |= WQ_FLAG_EXCLUSIVE;
	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
	__add_wait_queue_entry_tail(wq_سر, wq_اندراج);
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);
}
EXPORT_علامت(جوڑو_wait_قطار_exclusive);

فراغ جوڑو_wait_قطار_ترجیح(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج)
{
	بنانشانی لما flags;

	wq_اندراج->flags |= WQ_FLAG_ترجیح;
	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
	__add_wait_queue(wq_سر, wq_اندراج);
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);
}
EXPORT_علامت_GPL(جوڑو_wait_قطار_ترجیح);

صحيح جوڑو_wait_قطار_ترجیح_exclusive(بنية wait_قطار_سر *wq_سر,
				      بنية wait_قطار_اندراج *wq_اندراج)
{
	بنية list_سر *سر = &wq_سر->سر;

	guard(spinlock_irqsave)(&wq_سر->جندرا);

	إذا (!list_خالی_تھاواں(سر) &&
	    (list_پہلا_اندراج(سر, typeof(*wq_اندراج), اندراج)->flags & WQ_FLAG_ترجیح))
		أرجع -EBUSY;

	wq_اندراج->flags |= WQ_FLAG_EXCLUSIVE | WQ_FLAG_ترجیح;
	list_جوڑو(&wq_اندراج->اندراج, سر);
	أرجع 0;
}
EXPORT_علامت_GPL(جوڑو_wait_قطار_ترجیح_exclusive);

فراغ remove_wait_قطار(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج)
{
	بنانشانی لما flags;

	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
	__remove_wait_queue(wq_سر, wq_اندراج);
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);
}
EXPORT_علامت(remove_wait_قطار);

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
ساكن صحيح __wake_up_common(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال,
			صحيح nr_exclusive, صحيح wake_flags, فراغ *کلید)
{
	wait_قطار_اندراج_t *curr, *اگلا;

	lockdep_assert_held(&wq_سر->جندرا);

	curr = list_پہلا_اندراج(&wq_سر->سر, wait_قطار_اندراج_t, اندراج);

	إذا (&curr->اندراج == &wq_سر->سر)
		أرجع nr_exclusive;

	list_for_each_اندراج_محفوظ_from(curr, اگلا, &wq_سر->سر, اندراج) {
		بنانشانی flags = curr->flags;
		صحيح ret;

		ret = curr->func(curr, منوال, wake_flags, کلید);
		إذا (ret < 0)
			اكسر;
		إذا (ret && (flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			اكسر;
	}

	أرجع nr_exclusive;
}

ساكن صحيح __wake_up_common_lock(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال,
			صحيح nr_exclusive, صحيح wake_flags, فراغ *کلید)
{
	بنانشانی لما flags;
	صحيح بچیا;

	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
	بچیا = __wake_up_common(wq_سر, منوال, nr_exclusive, wake_flags,
			کلید);
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);

	أرجع nr_exclusive - بچیا;
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
صحيح __wake_up(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال,
	      صحيح nr_exclusive, فراغ *کلید)
{
	أرجع __wake_up_common_lock(wq_سر, منوال, nr_exclusive, 0, کلید);
}
EXPORT_علامت(__wake_up);

فراغ __wake_up_on_current_cpu(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال, فراغ *کلید)
{
	__wake_up_common_lock(wq_سر, منوال, 1, WF_CURRENT_CPU, کلید);
}

/*
 * Same as __wake_up but called with the spinlock in wait_queue_head_t held.
 */
فراغ __wake_up_locked(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال, صحيح nr)
{
	__wake_up_common(wq_سر, منوال, nr, 0, عدم);
}
EXPORT_علامت_GPL(__wake_up_locked);

فراغ __wake_up_locked_key(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال, فراغ *کلید)
{
	__wake_up_common(wq_سر, منوال, 1, 0, کلید);
}
EXPORT_علامت_GPL(__wake_up_locked_key);

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
فراغ __wake_up_sync_key(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال,
			فراغ *کلید)
{
	إذا (unlikely(!wq_سر))
		أرجع;

	__wake_up_common_lock(wq_سر, منوال, 1, WF_SYNC, کلید);
}
EXPORT_علامت_GPL(__wake_up_sync_key);

/**
 * __wake_up_locked_sync_key - wake up a thread blocked on a locked waitqueue.
 * @wq_head: the waitqueue
 * @mode: which threads
 * @key: opaque value to be passed to wakeup targets
 *
 * Same as __wake_up_sync_key(), but called with @wq_head->lock held.
 */
فراغ __wake_up_locked_sync_key(بنية wait_قطار_سر *wq_سر,
			       بنانشانی صحيح منوال, فراغ *کلید)
{
        __wake_up_common(wq_سر, منوال, 1, WF_SYNC, کلید);
}
EXPORT_علامت_GPL(__wake_up_locked_sync_key);

/*
 * __wake_up_sync - see __wake_up_sync_key()
 */
فراغ __wake_up_sync(بنية wait_قطار_سر *wq_سر, بنانشانی صحيح منوال)
{
	__wake_up_sync_key(wq_سر, منوال, عدم);
}
EXPORT_علامت_GPL(__wake_up_sync);	/* For internal use only */

فراغ __wake_up_pollfree(بنية wait_قطار_سر *wq_سر)
{
	__wake_up(wq_سر, TASK_عام, 0, poll_منزل_کلید(EPOLLHUP | POLLFREE));
	/* POLLFREE must have cleared the queue. */
	WARN_ON_ONCE(waitqueue_سرگرم(wq_سر));
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
prepare_منزل_wait(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج, صحيح state)
{
	بنانشانی لما flags;

	wq_اندراج->flags &= ~WQ_FLAG_EXCLUSIVE;
	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
	إذا (list_خالی_تھاواں(&wq_اندراج->اندراج))
		__add_wait_queue(wq_سر, wq_اندراج);
	سیٹ_current_state(state);
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);
}
EXPORT_علامت(prepare_منزل_wait);

/* Returns true if we are the first waiter in the queue, false otherwise. */
bool
prepare_منزل_wait_exclusive(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج, صحيح state)
{
	بنانشانی لما flags;
	bool was_خالی_تھاواں = false;

	wq_اندراج->flags |= WQ_FLAG_EXCLUSIVE;
	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
	إذا (list_خالی_تھاواں(&wq_اندراج->اندراج)) {
		was_خالی_تھاواں = list_خالی_تھاواں(&wq_سر->سر);
		__add_wait_queue_entry_tail(wq_سر, wq_اندراج);
	}
	سیٹ_current_state(state);
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);
	أرجع was_خالی_تھاواں;
}
EXPORT_علامت(prepare_منزل_wait_exclusive);

فراغ init_wait_اندراج(بنية wait_قطار_اندراج *wq_اندراج, صحيح flags)
{
	wq_اندراج->flags = flags;
	wq_اندراج->خاص = current;
	wq_اندراج->func = autoremove_wake_function;
	INIT_LIST_سر(&wq_اندراج->اندراج);
}
EXPORT_علامت(init_wait_اندراج);

لما prepare_منزل_wait_event(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج, صحيح state)
{
	بنانشانی لما flags;
	لما ret = 0;

	spin_جندرا_irqsave(&wq_سر->جندرا, flags);
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
		list_del_init(&wq_اندراج->اندراج);
		ret = -ERESTARTSYS;
	} وإلا {
		إذا (list_خالی_تھاواں(&wq_اندراج->اندراج)) {
			إذا (wq_اندراج->flags & WQ_FLAG_EXCLUSIVE)
				__add_wait_queue_entry_tail(wq_سر, wq_اندراج);
			وإلا
				__add_wait_queue(wq_سر, wq_اندراج);
		}
		سیٹ_current_state(state);
	}
	spin_unlock_irqrestore(&wq_سر->جندرا, flags);

	أرجع ret;
}
EXPORT_علامت(prepare_منزل_wait_event);

/*
 * Note! These two wait functions are entered with the
 * wait-queue lock held (and interrupts off in the _irq
 * case), so there is no race with testing the wakeup
 * condition in the caller before they add the wait
 * entry to the wake queue.
 */
صحيح do_wait_intr(wait_قطار_سر_t *wq, wait_قطار_اندراج_t *wait)
{
	إذا (likely(list_خالی_تھاواں(&wait->اندراج)))
		__add_wait_queue_entry_tail(wq, wait);

	سیٹ_current_state(TASK_INTERRUPTIBLE);
	إذا (signal_pending(current))
		أرجع -ERESTARTSYS;

	spin_unlock(&wq->جندرا);
	ویلا_سارنی();
	spin_جندرا(&wq->جندرا);

	أرجع 0;
}
EXPORT_علامت(do_wait_intr);

صحيح do_wait_intr_irq(wait_قطار_سر_t *wq, wait_قطار_اندراج_t *wait)
{
	إذا (likely(list_خالی_تھاواں(&wait->اندراج)))
		__add_wait_queue_entry_tail(wq, wait);

	سیٹ_current_state(TASK_INTERRUPTIBLE);
	إذا (signal_pending(current))
		أرجع -ERESTARTSYS;

	spin_unlock_irq(&wq->جندرا);
	ویلا_سارنی();
	spin_جندرا_irq(&wq->جندرا);

	أرجع 0;
}
EXPORT_علامت(do_wait_intr_irq);

/**
 * finish_wait - clean up after waiting in a queue
 * @wq_head: waitqueue waited on
 * @wq_entry: wait descriptor
 *
 * Sets current thread back to running state and removes
 * the wait descriptor from the given waitqueue if still
 * queued.
 */
فراغ انت_wait(بنية wait_قطار_سر *wq_سر, بنية wait_قطار_اندراج *wq_اندراج)
{
	بنانشانی لما flags;

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
	إذا (!list_خالی_تھاواں_careful(&wq_اندراج->اندراج)) {
		spin_جندرا_irqsave(&wq_سر->جندرا, flags);
		list_del_init(&wq_اندراج->اندراج);
		spin_unlock_irqrestore(&wq_سر->جندرا, flags);
	}
}
EXPORT_علامت(انت_wait);

صحيح autoremove_wake_function(بنية wait_قطار_اندراج *wq_اندراج, بنانشانی منوال, صحيح sync, فراغ *کلید)
{
	صحيح ret = default_wake_function(wq_اندراج, منوال, sync, کلید);

	إذا (ret)
		list_del_init_careful(&wq_اندراج->اندراج);

	أرجع ret;
}
EXPORT_علامت(autoremove_wake_function);

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
لما wait_woken(بنية wait_قطار_اندراج *wq_اندراج, بنانشانی منوال, لما timeout)
{
	/*
	 * The below executes an smp_mb(), which matches with the full barrier
	 * executed by the try_to_wake_up() in woken_wake_function() such that
	 * either we see the store to wq_entry->flags in woken_wake_function()
	 * or woken_wake_function() sees our store to current->state.
	 */
	سیٹ_current_state(منوال); /* A */
	إذا (!(wq_اندراج->flags & WQ_FLAG_WOKEN) && !kthread_should_stop_or_park())
		timeout = ویلا_سارنی_timeout(timeout);
	__set_current_state(TASK_RUNNING);

	/*
	 * The below executes an smp_mb(), which matches with the smp_mb() (C)
	 * in woken_wake_function() such that either we see the wait condition
	 * being true or the store to wq_entry->flags in woken_wake_function()
	 * follows ours in the coherence order.
	 */
	smp_store_mb(wq_اندراج->flags, wq_اندراج->flags & ~WQ_FLAG_WOKEN); /* B */

	أرجع timeout;
}
EXPORT_علامت(wait_woken);

صحيح woken_wake_function(بنية wait_قطار_اندراج *wq_اندراج, بنانشانی منوال, صحيح sync, فراغ *کلید)
{
	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_اندراج->flags |= WQ_FLAG_WOKEN;

	أرجع default_wake_function(wq_اندراج, منوال, sync, کلید);
}
EXPORT_علامت(woken_wake_function);

صحيح woken_wake_ثنائی_ہندسہ_function(بنية wait_قطار_اندراج *wq_اندراج, بنانشانی منوال, صحيح sync, فراغ *arg)
{
	بنية wait_ثنائی_ہندسہ_کلید *کلید = __var_wake_key(wq_اندراج, arg);
	إذا (!کلید)
		أرجع 0;

	/* Pairs with the smp_store_mb() in wait_woken(). */
	smp_mb(); /* C */
	wq_اندراج->flags |= WQ_FLAG_WOKEN;

	أرجع default_wake_function(wq_اندراج, منوال, sync, کلید);
}
EXPORT_علامت(woken_wake_ثنائی_ہندسہ_function);
