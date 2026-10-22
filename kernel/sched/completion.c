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

#ଅନ୍ତର୍ଭାଵ <linux/linkage.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/debug.h>
#ଅନ୍ତର୍ଭାଵ <linux/completion.h>
#ଅନ୍ତର୍ଭାଵ "sched.h"

ସ୍ଥିର ଶୂନ୍ୟ complete_with_flags(ସଂରଚନା completion *x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&x->wait.lock, flags);

	ଯଦି (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, wake_flags);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}

ଶୂନ୍ୟ complete_on_current_cpu(ସଂରଚନା completion *x)
{
	ଫେରନ୍ତୁ complete_with_flags(x, WF_CURRENT_CPU);
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
ଶୂନ୍ୟ complete(ସଂରଚନା completion *x)
{
	complete_with_flags(x, 0);
}
EXPORT_ପ୍ରତୀକ(complete);

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
ଶୂନ୍ୟ complete_all(ସଂରଚନା completion *x)
{
	unsigned long flags;

	lockdep_assert_RT_in_threaded_ctx();

	raw_spin_lock_irqsave(&x->wait.lock, flags);
	x->done = UINT_MAX;
	swake_up_all_locked(&x->wait);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}
EXPORT_ପ୍ରତୀକ(complete_all);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ long __sched
do_wait_for_common(ସଂରଚନା completion *x,
		   long (*action)(long), long timeout, ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	ଯଦି (!x->done) {
		DECLARE_SWAITQUEUE(wait);

		କର {
			ଯଦି (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				ଭାଙ୍ଗ;
			}
			__prepare_to_swait(&x->wait, &wait);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->wait.lock);
			timeout = action(timeout);
			raw_spin_lock_irq(&x->wait.lock);
		} ଯେତେବେଳେ (!x->done && timeout);
		__finish_swait(&x->wait, &wait);
		ଯଦି (!x->done)
			ଫେରନ୍ତୁ timeout;
	}
	ଯଦି (x->done != UINT_MAX)
		x->done--;
	ଫେରନ୍ତୁ timeout ?: 1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ long __sched
__wait_for_common(ସଂରଚନା completion *x,
		  long (*action)(long), long timeout, ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	might_sleep();

	complete_acquire(x);

	raw_spin_lock_irq(&x->wait.lock);
	timeout = do_wait_for_common(x, action, timeout, state);
	raw_spin_unlock_irq(&x->wait.lock);

	complete_release(x);

	ଫେରନ୍ତୁ timeout;
}

ସ୍ଥିର long __sched
wait_for_common(ସଂରଚନା completion *x, long timeout, ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	ଫେରନ୍ତୁ __wait_for_common(x, schedule_timeout, timeout, state);
}

ସ୍ଥିର long __sched
wait_for_common_io(ସଂରଚନା completion *x, long timeout, ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	ଫେରନ୍ତୁ __wait_for_common(x, io_schedule_timeout, timeout, state);
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
ଶୂନ୍ୟ __sched wait_for_completion(ସଂରଚନା completion *x)
{
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_ପ୍ରତୀକ(wait_for_completion);

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
wait_for_completion_timeout(ସଂରଚନା completion *x, unsigned long timeout)
{
	ଫେରନ୍ତୁ wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO (which traditionally means blkio only).
 */
ଶୂନ୍ୟ __sched wait_for_completion_io(ସଂରଚନା completion *x)
{
	wait_for_common_io(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_io);

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
wait_for_completion_io_timeout(ସଂରଚନା completion *x, unsigned long timeout)
{
	ଫେରନ୍ତୁ wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched wait_for_completion_interruptible(ସଂରଚନା completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_INTERRUPTIBLE);

	ଯଦି (t == -ERESTARTSYS)
		ଫେରନ୍ତୁ t;
	ଫେରନ୍ତୁ 0;
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_interruptible);

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
wait_for_completion_interruptible_timeout(ସଂରଚନା completion *x,
					  unsigned long timeout)
{
	ଫେରନ୍ତୁ wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched wait_for_completion_killable(ସଂରଚନା completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_KILLABLE);

	ଯଦି (t == -ERESTARTSYS)
		ଫେରନ୍ତୁ t;
	ଫେରନ୍ତୁ 0;
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_killable);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched wait_for_completion_state(ସଂରଚନା completion *x, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, state);

	ଯଦି (t == -ERESTARTSYS)
		ଫେରନ୍ତୁ t;
	ଫେରନ୍ତୁ 0;
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_state);

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
wait_for_completion_killable_timeout(ସଂରଚନା completion *x,
				     unsigned long timeout)
{
	ଫେରନ୍ତୁ wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_ପ୍ରତୀକ(wait_for_completion_killable_timeout);

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
bool try_wait_for_completion(ସଂରଚନା completion *x)
{
	unsigned long flags;
	bool ret = true;

	/*
	 * Since x->done will need to be locked only
	 * in the non-blocking case, we check x->done
	 * first without taking the lock so we can
	 * return early in the blocking case.
	 */
	ଯଦି (!READ_ONCE(x->done))
		ଫେରନ୍ତୁ false;

	raw_spin_lock_irqsave(&x->wait.lock, flags);
	ଯଦି (!x->done)
		ret = false;
	ଅନ୍ୟଥା ଯଦି (x->done != UINT_MAX)
		x->done--;
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
	ଫେରନ୍ତୁ ret;
}
EXPORT_ପ୍ରତୀକ(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Return: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 *	Note, this will always return true if complete_all() was called on @X.
 */
bool completion_done(ସଂରଚନା completion *x)
{
	unsigned long flags;

	ଯଦି (!READ_ONCE(x->done))
		ଫେରନ୍ତୁ false;

	/*
	 * If ->done, we need to wait for complete() to release ->wait.lock
	 * otherwise we can end up freeing the completion before complete()
	 * is done referencing it.
	 */
	raw_spin_lock_irqsave(&x->wait.lock, flags);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
	ଫେରନ୍ତୁ true;
}
EXPORT_ପ୍ରତୀକ(completion_done);
