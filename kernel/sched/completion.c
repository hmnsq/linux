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

#अन्तर्भाव <linux/linkage.h>
#अन्तर्भाव <linux/sched/debug.h>
#अन्तर्भाव <linux/completion.h>
#अन्तर्भाव "sched.h"

स्थिर शून्य complete_with_flags(संरचना completion *x, पूर्णाङ्क wake_flags)
{
	अचिन्हित दीर्घ flags;

	raw_spin_lock_irqsave(&x->रुको.lock, flags);

	यदि (x->done != अच_पूर्णांक_उच्च)
		x->done++;
	swake_up_locked(&x->रुको, wake_flags);
	raw_spin_unlock_irqrestore(&x->रुको.lock, flags);
}

शून्य complete_on_current_cpu(संरचना completion *x)
{
	फर्काऊ complete_with_flags(x, WF_CURRENT_CPU);
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
शून्य complete(संरचना completion *x)
{
	complete_with_flags(x, 0);
}
EXPORT_चिह्न(complete);

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
शून्य complete_all(संरचना completion *x)
{
	अचिन्हित दीर्घ flags;

	lockdep_assert_RT_in_threaded_ctx();

	raw_spin_lock_irqsave(&x->रुको.lock, flags);
	x->done = अच_पूर्णांक_उच्च;
	swake_up_all_locked(&x->रुको);
	raw_spin_unlock_irqrestore(&x->रुको.lock, flags);
}
EXPORT_चिह्न(complete_all);

स्थिर अन्तःस्थ दीर्घ __sched
do_wait_for_common(संरचना completion *x,
		   दीर्घ (*action)(दीर्घ), दीर्घ timeout, पूर्णाङ्क state)
{
	यदि (!x->done) {
		DECLARE_SWAITQUEUE(रुको);

		गर {
			यदि (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				रोक;
			}
			__prepare_to_swait(&x->रुको, &रुको);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->रुको.lock);
			timeout = action(timeout);
			raw_spin_lock_irq(&x->रुको.lock);
		} जबसम्म (!x->done && timeout);
		__finish_swait(&x->रुको, &रुको);
		यदि (!x->done)
			फर्काऊ timeout;
	}
	यदि (x->done != अच_पूर्णांक_उच्च)
		x->done--;
	फर्काऊ timeout ?: 1;
}

स्थिर अन्तःस्थ दीर्घ __sched
__wait_for_common(संरचना completion *x,
		  दीर्घ (*action)(दीर्घ), दीर्घ timeout, पूर्णाङ्क state)
{
	might_sleep();

	complete_acquire(x);

	raw_spin_lock_irq(&x->रुको.lock);
	timeout = do_wait_for_common(x, action, timeout, state);
	raw_spin_unlock_irq(&x->रुको.lock);

	complete_release(x);

	फर्काऊ timeout;
}

स्थिर दीर्घ __sched
wait_for_common(संरचना completion *x, दीर्घ timeout, पूर्णाङ्क state)
{
	फर्काऊ __wait_for_common(x, schedule_timeout, timeout, state);
}

स्थिर दीर्घ __sched
wait_for_common_io(संरचना completion *x, दीर्घ timeout, पूर्णाङ्क state)
{
	फर्काऊ __wait_for_common(x, io_schedule_timeout, timeout, state);
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
शून्य __sched wait_for_completion(संरचना completion *x)
{
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_चिह्न(wait_for_completion);

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
अचिन्हित दीर्घ __sched
wait_for_completion_timeout(संरचना completion *x, अचिन्हित दीर्घ timeout)
{
	फर्काऊ wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_चिह्न(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO (which traditionally means blkio only).
 */
शून्य __sched wait_for_completion_io(संरचना completion *x)
{
	wait_for_common_io(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_चिह्न(wait_for_completion_io);

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
अचिन्हित दीर्घ __sched
wait_for_completion_io_timeout(संरचना completion *x, अचिन्हित दीर्घ timeout)
{
	फर्काऊ wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_चिह्न(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
पूर्णाङ्क __sched wait_for_completion_interruptible(संरचना completion *x)
{
	दीर्घ t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_INTERRUPTIBLE);

	यदि (t == -ERESTARTSYS)
		फर्काऊ t;
	फर्काऊ 0;
}
EXPORT_चिह्न(wait_for_completion_interruptible);

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
दीर्घ __sched
wait_for_completion_interruptible_timeout(संरचना completion *x,
					  अचिन्हित दीर्घ timeout)
{
	फर्काऊ wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_चिह्न(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
पूर्णाङ्क __sched wait_for_completion_killable(संरचना completion *x)
{
	दीर्घ t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_KILLABLE);

	यदि (t == -ERESTARTSYS)
		फर्काऊ t;
	फर्काऊ 0;
}
EXPORT_चिह्न(wait_for_completion_killable);

पूर्णाङ्क __sched wait_for_completion_state(संरचना completion *x, अचिन्हित पूर्णाङ्क state)
{
	दीर्घ t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, state);

	यदि (t == -ERESTARTSYS)
		फर्काऊ t;
	फर्काऊ 0;
}
EXPORT_चिह्न(wait_for_completion_state);

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
दीर्घ __sched
wait_for_completion_killable_timeout(संरचना completion *x,
				     अचिन्हित दीर्घ timeout)
{
	फर्काऊ wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_चिह्न(wait_for_completion_killable_timeout);

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
bool try_wait_for_completion(संरचना completion *x)
{
	अचिन्हित दीर्घ flags;
	bool ret = true;

	/*
	 * Since x->done will need to be locked only
	 * in the non-blocking case, we check x->done
	 * first without taking the lock so we can
	 * return early in the blocking case.
	 */
	यदि (!READ_ONCE(x->done))
		फर्काऊ false;

	raw_spin_lock_irqsave(&x->रुको.lock, flags);
	यदि (!x->done)
		ret = false;
	अन्यथा यदि (x->done != अच_पूर्णांक_उच्च)
		x->done--;
	raw_spin_unlock_irqrestore(&x->रुको.lock, flags);
	फर्काऊ ret;
}
EXPORT_चिह्न(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Return: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 *	Note, this will always return true if complete_all() was called on @X.
 */
bool completion_done(संरचना completion *x)
{
	अचिन्हित दीर्घ flags;

	यदि (!READ_ONCE(x->done))
		फर्काऊ false;

	/*
	 * If ->done, we need to wait for complete() to release ->wait.lock
	 * otherwise we can end up freeing the completion before complete()
	 * is done referencing it.
	 */
	raw_spin_lock_irqsave(&x->रुको.lock, flags);
	raw_spin_unlock_irqrestore(&x->रुको.lock, flags);
	फर्काऊ true;
}
EXPORT_चिह्न(completion_done);
