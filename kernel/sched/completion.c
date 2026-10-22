// SPDX-License-Identifier: GPL-2.0

/*
 * Generic wait-for-completion handler;
 *
 * It differs from semaphores in that their default case is the opposite,
 * wait_for_completion default blocks whereas semaphore default non-block. The
 * interface also makes it easy to 'complete' multiple waiting threads,
 * something which isn't entirely natural for semaphores.
 *
 * But more importantly, the primitive documents the usage. Semaphores would
 * typically be used for exclusion which gives rise to priority inversion.
 * Waiting for completion is a typically sync point, but not an exclusion point.
 */

#অন্তর্ভাব <linux/linkage.h>
#অন্তর্ভাব <linux/sched/debug.h>
#অন্তর্ভাব <linux/completion.h>
#অন্তর্ভাব "sched.h"

স্থির শূন্য complete_with_flags(কাঠামো completion *x, পূর্ণসংখ্যা wake_flags)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&x->wait.lock, flags);

	যদি (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, wake_flags);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}

শূন্য complete_on_current_cpu(কাঠামো completion *x)
{
	ফেরত complete_with_flags(x, WF_CURRENT_CPU);
}

/**
 * complete: - signals a single thread waiting on this completion
 * @x:  holds the state of this particular completion
 *
 * This will wake up a single thread waiting on this completion. Threads will be
 * awakened in the same order in which they were queued.
 *
 * See also complete_all(), wait_for_completion() and related routines.
 *
 * If this function wakes up a task, it executes a full memory barrier before
 * accessing the task state.
 */
শূন্য complete(কাঠামো completion *x)
{
	complete_with_flags(x, 0);
}
EXPORT_প্রতীক(complete);

/**
 * complete_all: - signals all threads waiting on this completion
 * @x:  holds the state of this particular completion
 *
 * This will wake up all threads waiting on this particular completion event.
 *
 * If this function wakes up a task, it executes a full memory barrier before
 * accessing the task state.
 *
 * Since complete_all() sets the completion of @x permanently to done
 * to allow multiple waiters to finish, a call to reinit_completion()
 * must be used on @x if @x is to be used again. The code must make
 * sure that all waiters have woken and finished before reinitializing
 * @x. Also note that the function completion_done() can not be used
 * to know if there are still waiters after complete_all() has been called.
 */
শূন্য complete_all(কাঠামো completion *x)
{
	unsigned long flags;

	lockdep_assert_RT_in_threaded_ctx();

	raw_spin_lock_irqsave(&x->wait.lock, flags);
	x->done = UINT_MAX;
	swake_up_all_locked(&x->wait);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}
EXPORT_প্রতীক(complete_all);

স্থির অন্তঃরেখ long __sched
do_wait_for_common(কাঠামো completion *x,
		   long (*action)(long), long timeout, পূর্ণসংখ্যা state)
{
	যদি (!x->done) {
		DECLARE_SWAITQUEUE(wait);

		করো {
			যদি (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				বিরতি;
			}
			__prepare_to_swait(&x->wait, &wait);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->wait.lock);
			timeout = action(timeout);
			raw_spin_lock_irq(&x->wait.lock);
		} যখন (!x->done && timeout);
		__finish_swait(&x->wait, &wait);
		যদি (!x->done)
			ফেরত timeout;
	}
	যদি (x->done != UINT_MAX)
		x->done--;
	ফেরত timeout ?: 1;
}

স্থির অন্তঃরেখ long __sched
__wait_for_common(কাঠামো completion *x,
		  long (*action)(long), long timeout, পূর্ণসংখ্যা state)
{
	might_sleep();

	complete_acquire(x);

	raw_spin_lock_irq(&x->wait.lock);
	timeout = do_wait_for_common(x, action, timeout, state);
	raw_spin_unlock_irq(&x->wait.lock);

	complete_release(x);

	ফেরত timeout;
}

স্থির long __sched
wait_for_common(কাঠামো completion *x, long timeout, পূর্ণসংখ্যা state)
{
	ফেরত __wait_for_common(x, schedule_timeout, timeout, state);
}

স্থির long __sched
wait_for_common_io(কাঠামো completion *x, long timeout, পূর্ণসংখ্যা state)
{
	ফেরত __wait_for_common(x, io_schedule_timeout, timeout, state);
}

/**
 * wait_for_completion: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout.
 *
 * See also similar routines (i.e. wait_for_completion_timeout()) with timeout
 * and interrupt capability. Also see complete().
 */
শূন্য __sched wait_for_completion(কাঠামো completion *x)
{
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_প্রতীক(wait_for_completion);

/**
 * wait_for_completion_timeout: - waits for completion of a task (w/timeout)
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be signaled or for a
 * specified timeout to expire. The timeout is in jiffies. It is not
 * interruptible.
 *
 * Return: 0 if timed out, and positive (at least 1, or number of jiffies left
 * till timeout) if completed.
 */
unsigned long __sched
wait_for_completion_timeout(কাঠামো completion *x, unsigned long timeout)
{
	ফেরত wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_প্রতীক(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO (which traditionally means blkio only).
 */
শূন্য __sched wait_for_completion_io(কাঠামো completion *x)
{
	wait_for_common_io(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_প্রতীক(wait_for_completion_io);

/**
 * wait_for_completion_io_timeout: - waits for completion of a task (w/timeout)
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be signaled or for a
 * specified timeout to expire. The timeout is in jiffies. It is not
 * interruptible. The caller is accounted as waiting for IO (which traditionally
 * means blkio only).
 *
 * Return: 0 if timed out, and positive (at least 1, or number of jiffies left
 * till timeout) if completed.
 */
unsigned long __sched
wait_for_completion_io_timeout(কাঠামো completion *x, unsigned long timeout)
{
	ফেরত wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_প্রতীক(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
পূর্ণসংখ্যা __sched wait_for_completion_interruptible(কাঠামো completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_INTERRUPTIBLE);

	যদি (t == -ERESTARTSYS)
		ফেরত t;
	ফেরত 0;
}
EXPORT_প্রতীক(wait_for_completion_interruptible);

/**
 * wait_for_completion_interruptible_timeout: - waits for completion (w/(to,intr))
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be signaled or for a
 * specified timeout to expire. It is interruptible. The timeout is in jiffies.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if timed out, positive (at least 1,
 * or number of jiffies left till timeout) if completed.
 */
long __sched
wait_for_completion_interruptible_timeout(কাঠামো completion *x,
					  unsigned long timeout)
{
	ফেরত wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_প্রতীক(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
পূর্ণসংখ্যা __sched wait_for_completion_killable(কাঠামো completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_KILLABLE);

	যদি (t == -ERESTARTSYS)
		ফেরত t;
	ফেরত 0;
}
EXPORT_প্রতীক(wait_for_completion_killable);

পূর্ণসংখ্যা __sched wait_for_completion_state(কাঠামো completion *x, unsigned পূর্ণসংখ্যা state)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, state);

	যদি (t == -ERESTARTSYS)
		ফেরত t;
	ফেরত 0;
}
EXPORT_প্রতীক(wait_for_completion_state);

/**
 * wait_for_completion_killable_timeout: - waits for completion of a task (w/(to,killable))
 * @x:  holds the state of this particular completion
 * @timeout:  timeout value in jiffies
 *
 * This waits for either a completion of a specific task to be
 * signaled or for a specified timeout to expire. It can be
 * interrupted by a kill signal. The timeout is in jiffies.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if timed out, positive (at least 1,
 * or number of jiffies left till timeout) if completed.
 */
long __sched
wait_for_completion_killable_timeout(কাঠামো completion *x,
				     unsigned long timeout)
{
	ফেরত wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_প্রতীক(wait_for_completion_killable_timeout);

/**
 *	try_wait_for_completion - try to decrement a completion without blocking
 *	@x:	completion structure
 *
 *	Return: 0 if a decrement cannot be done without blocking
 *		 1 if a decrement succeeded.
 *
 *	If a completion is being used as a counting completion,
 *	attempt to decrement the counter without blocking. This
 *	enables us to avoid waiting if the resource the completion
 *	is protecting is not available.
 */
bool try_wait_for_completion(কাঠামো completion *x)
{
	unsigned long flags;
	bool ret = true;

	/*
	 * Since x->done will need to be locked only
	 * in the non-blocking case, we check x->done
	 * first without taking the lock so we can
	 * return early in the blocking case.
	 */
	যদি (!READ_ONCE(x->done))
		ফেরত false;

	raw_spin_lock_irqsave(&x->wait.lock, flags);
	যদি (!x->done)
		ret = false;
	অন্যথা যদি (x->done != UINT_MAX)
		x->done--;
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
	ফেরত ret;
}
EXPORT_প্রতীক(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Return: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 *	Note, this will always return true if complete_all() was called on @X.
 */
bool completion_done(কাঠামো completion *x)
{
	unsigned long flags;

	যদি (!READ_ONCE(x->done))
		ফেরত false;

	/*
	 * If ->done, we need to wait for complete() to release ->wait.lock
	 * otherwise we can end up freeing the completion before complete()
	 * is done referencing it.
	 */
	raw_spin_lock_irqsave(&x->wait.lock, flags);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
	ফেরত true;
}
EXPORT_প্রতীক(completion_done);
