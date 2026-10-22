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

#ಅನ್ತರ್ಭಾವ <linux/linkage.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/debug.h>
#ಅನ್ತರ್ಭಾವ <linux/completion.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

ಸ್ಥಿರ ಶೂನ್ಯ complete_with_flags(ರಚನೆ completion *x, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	raw_spin_ಬೀಗ_irqsave(&x->wait.ಬೀಗ, flags);

	ಆದರೆ (x->ಮುಗಿದ != UINT_MAX)
		x->ಮುಗಿದ++;
	swake_ಮೇಲೆ_locked(&x->wait, wake_flags);
	raw_spin_unlock_irqrestore(&x->wait.ಬೀಗ, flags);
}

ಶೂನ್ಯ complete_on_current_cpu(ರಚನೆ completion *x)
{
	ಹಿಂತಿರುಗಿ complete_with_flags(x, WF_CURRENT_CPU);
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
ಶೂನ್ಯ complete(ರಚನೆ completion *x)
{
	complete_with_flags(x, 0);
}
EXPORT_ಚಿಹ್ನೆ(complete);

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
ಶೂನ್ಯ complete_all(ರಚನೆ completion *x)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	lockdep_assert_RT_ಒಳ_threaded_ctx();

	raw_spin_ಬೀಗ_irqsave(&x->wait.ಬೀಗ, flags);
	x->ಮುಗಿದ = UINT_MAX;
	swake_ಮೇಲೆ_all_locked(&x->wait);
	raw_spin_unlock_irqrestore(&x->wait.ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ(complete_all);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ __sched
do_wait_for_common(ರಚನೆ completion *x,
		   ದೀರ್ಘ (*action)(ದೀರ್ಘ), ದೀರ್ಘ timeout, ಪೂರ್ಣಾಂಕ state)
{
	ಆದರೆ (!x->ಮುಗಿದ) {
		DECLARE_SWAITQUEUE(wait);

		ಮಾಡು {
			ಆದರೆ (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				ನಿಲ್ಲಿಸು;
			}
			__prepare_to_swait(&x->wait, &wait);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->wait.ಬೀಗ);
			timeout = action(timeout);
			raw_spin_ಬೀಗ_irq(&x->wait.ಬೀಗ);
		} ಆಗ (!x->ಮುಗಿದ && timeout);
		__finish_swait(&x->wait, &wait);
		ಆದರೆ (!x->ಮುಗಿದ)
			ಹಿಂತಿರುಗಿ timeout;
	}
	ಆದರೆ (x->ಮುಗಿದ != UINT_MAX)
		x->ಮುಗಿದ--;
	ಹಿಂತಿರುಗಿ timeout ?: 1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ __sched
__wait_for_common(ರಚನೆ completion *x,
		  ದೀರ್ಘ (*action)(ದೀರ್ಘ), ದೀರ್ಘ timeout, ಪೂರ್ಣಾಂಕ state)
{
	might_sleep();

	complete_acquire(x);

	raw_spin_ಬೀಗ_irq(&x->wait.ಬೀಗ);
	timeout = do_wait_for_common(x, action, timeout, state);
	raw_spin_unlock_irq(&x->wait.ಬೀಗ);

	complete_release(x);

	ಹಿಂತಿರುಗಿ timeout;
}

ಸ್ಥಿರ ದೀರ್ಘ __sched
wait_for_common(ರಚನೆ completion *x, ದೀರ್ಘ timeout, ಪೂರ್ಣಾಂಕ state)
{
	ಹಿಂತಿರುಗಿ __wait_for_common(x, ವೇಳಾಪಟ್ಟಿ_timeout, timeout, state);
}

ಸ್ಥಿರ ದೀರ್ಘ __sched
wait_for_common_io(ರಚನೆ completion *x, ದೀರ್ಘ timeout, ಪೂರ್ಣಾಂಕ state)
{
	ಹಿಂತಿರುಗಿ __wait_for_common(x, io_ವೇಳಾಪಟ್ಟಿ_timeout, timeout, state);
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
ಶೂನ್ಯ __sched wait_for_completion(ರಚನೆ completion *x)
{
	wait_for_common(x, MAX_ವೇಳಾಪಟ್ಟಿ_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion);

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
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __sched
wait_for_completion_timeout(ರಚನೆ completion *x, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ timeout)
{
	ಹಿಂತಿರುಗಿ wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO (which traditionally means blkio only).
 */
ಶೂನ್ಯ __sched wait_for_completion_io(ರಚನೆ completion *x)
{
	wait_for_common_io(x, MAX_ವೇಳಾಪಟ್ಟಿ_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_io);

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
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __sched
wait_for_completion_io_timeout(ರಚನೆ completion *x, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ timeout)
{
	ಹಿಂತಿರುಗಿ wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
ಪೂರ್ಣಾಂಕ __sched wait_for_completion_interruptible(ರಚನೆ completion *x)
{
	ದೀರ್ಘ t = wait_for_common(x, MAX_ವೇಳಾಪಟ್ಟಿ_TIMEOUT, TASK_INTERRUPTIBLE);

	ಆದರೆ (t == -ERESTARTSYS)
		ಹಿಂತಿರುಗಿ t;
	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_interruptible);

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
ದೀರ್ಘ __sched
wait_for_completion_interruptible_timeout(ರಚನೆ completion *x,
					  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ timeout)
{
	ಹಿಂತಿರುಗಿ wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
ಪೂರ್ಣಾಂಕ __sched wait_for_completion_killable(ರಚನೆ completion *x)
{
	ದೀರ್ಘ t = wait_for_common(x, MAX_ವೇಳಾಪಟ್ಟಿ_TIMEOUT, TASK_KILLABLE);

	ಆದರೆ (t == -ERESTARTSYS)
		ಹಿಂತಿರುಗಿ t;
	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_killable);

ಪೂರ್ಣಾಂಕ __sched wait_for_completion_state(ರಚನೆ completion *x, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state)
{
	ದೀರ್ಘ t = wait_for_common(x, MAX_ವೇಳಾಪಟ್ಟಿ_TIMEOUT, state);

	ಆದರೆ (t == -ERESTARTSYS)
		ಹಿಂತಿರುಗಿ t;
	ಹಿಂತಿರುಗಿ 0;
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_state);

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
ದೀರ್ಘ __sched
wait_for_completion_killable_timeout(ರಚನೆ completion *x,
				     ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ timeout)
{
	ಹಿಂತಿರುಗಿ wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_ಚಿಹ್ನೆ(wait_for_completion_killable_timeout);

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
bool try_wait_for_completion(ರಚನೆ completion *x)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	bool ret = true;

	/*
	 * Since x->done will need to be locked only
	 * in the non-blocking case, we check x->done
	 * first without taking the lock so we can
	 * return early in the blocking case.
	 */
	ಆದರೆ (!READ_ONCE(x->ಮುಗಿದ))
		ಹಿಂತಿರುಗಿ false;

	raw_spin_ಬೀಗ_irqsave(&x->wait.ಬೀಗ, flags);
	ಆದರೆ (!x->ಮುಗಿದ)
		ret = false;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (x->ಮುಗಿದ != UINT_MAX)
		x->ಮುಗಿದ--;
	raw_spin_unlock_irqrestore(&x->wait.ಬೀಗ, flags);
	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Return: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 *	Note, this will always return true if complete_all() was called on @X.
 */
bool completion_ಮುಗಿದ(ರಚನೆ completion *x)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	ಆದರೆ (!READ_ONCE(x->ಮುಗಿದ))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * If ->done, we need to wait for complete() to release ->wait.lock
	 * otherwise we can end up freeing the completion before complete()
	 * is done referencing it.
	 */
	raw_spin_ಬೀಗ_irqsave(&x->wait.ಬೀಗ, flags);
	raw_spin_unlock_irqrestore(&x->wait.ಬೀಗ, flags);
	ಹಿಂತಿರುಗಿ true;
}
EXPORT_ಚಿಹ್ನೆ(completion_ಮುಗಿದ);
