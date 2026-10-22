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

#അന്തര്ഭാവ <linux/linkage.h>
#അന്തര്ഭാവ <linux/sched/debug.h>
#അന്തര്ഭാവ <linux/completion.h>
#അന്തര്ഭാവ "sched.h"

സ്ഥിതം ശൂന്യം complete_with_flags(ഘടന completion *x, പൂർണ്ണസംഖ്യ wake_flags)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&x->wait.lock, flags);

	എങ്കിൽ (x->done != UINT_MAX)
		x->done++;
	swake_up_locked(&x->wait, wake_flags);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}

ശൂന്യം complete_on_current_cpu(ഘടന completion *x)
{
	തിരികെ complete_with_flags(x, WF_CURRENT_CPU);
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
ശൂന്യം complete(ഘടന completion *x)
{
	complete_with_flags(x, 0);
}
EXPORT_ചിഹ്നം(complete);

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
ശൂന്യം complete_all(ഘടന completion *x)
{
	unsigned long flags;

	lockdep_assert_RT_in_threaded_ctx();

	raw_spin_lock_irqsave(&x->wait.lock, flags);
	x->done = UINT_MAX;
	swake_up_all_locked(&x->wait);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
}
EXPORT_ചിഹ്നം(complete_all);

സ്ഥിതം വരിയിൽ long __sched
do_wait_for_common(ഘടന completion *x,
		   long (*action)(long), long timeout, പൂർണ്ണസംഖ്യ state)
{
	എങ്കിൽ (!x->done) {
		DECLARE_SWAITQUEUE(wait);

		ചെയ്യുക {
			എങ്കിൽ (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				നിർത്തുക;
			}
			__prepare_to_swait(&x->wait, &wait);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->wait.lock);
			timeout = action(timeout);
			raw_spin_lock_irq(&x->wait.lock);
		} ആകുമ്പോൾ (!x->done && timeout);
		__finish_swait(&x->wait, &wait);
		എങ്കിൽ (!x->done)
			തിരികെ timeout;
	}
	എങ്കിൽ (x->done != UINT_MAX)
		x->done--;
	തിരികെ timeout ?: 1;
}

സ്ഥിതം വരിയിൽ long __sched
__wait_for_common(ഘടന completion *x,
		  long (*action)(long), long timeout, പൂർണ്ണസംഖ്യ state)
{
	might_sleep();

	complete_acquire(x);

	raw_spin_lock_irq(&x->wait.lock);
	timeout = do_wait_for_common(x, action, timeout, state);
	raw_spin_unlock_irq(&x->wait.lock);

	complete_release(x);

	തിരികെ timeout;
}

സ്ഥിതം long __sched
wait_for_common(ഘടന completion *x, long timeout, പൂർണ്ണസംഖ്യ state)
{
	തിരികെ __wait_for_common(x, schedule_timeout, timeout, state);
}

സ്ഥിതം long __sched
wait_for_common_io(ഘടന completion *x, long timeout, പൂർണ്ണസംഖ്യ state)
{
	തിരികെ __wait_for_common(x, io_schedule_timeout, timeout, state);
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
ശൂന്യം __sched wait_for_completion(ഘടന completion *x)
{
	wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_ചിഹ്നം(wait_for_completion);

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
wait_for_completion_timeout(ഘടന completion *x, unsigned long timeout)
{
	തിരികെ wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_ചിഹ്നം(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO (which traditionally means blkio only).
 */
ശൂന്യം __sched wait_for_completion_io(ഘടന completion *x)
{
	wait_for_common_io(x, MAX_SCHEDULE_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_ചിഹ്നം(wait_for_completion_io);

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
wait_for_completion_io_timeout(ഘടന completion *x, unsigned long timeout)
{
	തിരികെ wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_ചിഹ്നം(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
പൂർണ്ണസംഖ്യ __sched wait_for_completion_interruptible(ഘടന completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_INTERRUPTIBLE);

	എങ്കിൽ (t == -ERESTARTSYS)
		തിരികെ t;
	തിരികെ 0;
}
EXPORT_ചിഹ്നം(wait_for_completion_interruptible);

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
wait_for_completion_interruptible_timeout(ഘടന completion *x,
					  unsigned long timeout)
{
	തിരികെ wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_ചിഹ്നം(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
പൂർണ്ണസംഖ്യ __sched wait_for_completion_killable(ഘടന completion *x)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, TASK_KILLABLE);

	എങ്കിൽ (t == -ERESTARTSYS)
		തിരികെ t;
	തിരികെ 0;
}
EXPORT_ചിഹ്നം(wait_for_completion_killable);

പൂർണ്ണസംഖ്യ __sched wait_for_completion_state(ഘടന completion *x, unsigned പൂർണ്ണസംഖ്യ state)
{
	long t = wait_for_common(x, MAX_SCHEDULE_TIMEOUT, state);

	എങ്കിൽ (t == -ERESTARTSYS)
		തിരികെ t;
	തിരികെ 0;
}
EXPORT_ചിഹ്നം(wait_for_completion_state);

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
wait_for_completion_killable_timeout(ഘടന completion *x,
				     unsigned long timeout)
{
	തിരികെ wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_ചിഹ്നം(wait_for_completion_killable_timeout);

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
bool try_wait_for_completion(ഘടന completion *x)
{
	unsigned long flags;
	bool ret = true;

	/*
	 * Since x->done will need to be locked only
	 * in the non-blocking case, we check x->done
	 * first without taking the lock so we can
	 * return early in the blocking case.
	 */
	എങ്കിൽ (!READ_ONCE(x->done))
		തിരികെ false;

	raw_spin_lock_irqsave(&x->wait.lock, flags);
	എങ്കിൽ (!x->done)
		ret = false;
	അല്ലെങ്കിൽ എങ്കിൽ (x->done != UINT_MAX)
		x->done--;
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
	തിരികെ ret;
}
EXPORT_ചിഹ്നം(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Return: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 *	Note, this will always return true if complete_all() was called on @X.
 */
bool completion_done(ഘടന completion *x)
{
	unsigned long flags;

	എങ്കിൽ (!READ_ONCE(x->done))
		തിരികെ false;

	/*
	 * If ->done, we need to wait for complete() to release ->wait.lock
	 * otherwise we can end up freeing the completion before complete()
	 * is done referencing it.
	 */
	raw_spin_lock_irqsave(&x->wait.lock, flags);
	raw_spin_unlock_irqrestore(&x->wait.lock, flags);
	തിരികെ true;
}
EXPORT_ചിഹ്നം(completion_done);
