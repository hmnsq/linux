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

#అన్తర్భావ <linux/linkage.h>
#అన్తర్భావ <linux/sched/debug.h>
#అన్తర్భావ <linux/completion.h>
#అన్తర్భావ "sched.h"

స్థిర శూన్య complete_with_flags(నిర్మాణం completion *x, పూర్ణాంకం wake_flags)
{
	గుర్తులేని దీర్ఘ flags;

	raw_spin_తాళం_irqsave(&x->wait.తాళం, flags);

	ఒకవేళ (x->పూర్తి != UINT_MAX)
		x->పూర్తి++;
	swake_పైకి_locked(&x->wait, wake_flags);
	raw_spin_unlock_irqrestore(&x->wait.తాళం, flags);
}

శూన్య complete_on_current_cpu(నిర్మాణం completion *x)
{
	తిరిగి complete_with_flags(x, WF_CURRENT_CPU);
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
శూన్య complete(నిర్మాణం completion *x)
{
	complete_with_flags(x, 0);
}
EXPORT_చిహ్నం(complete);

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
శూన్య complete_all(నిర్మాణం completion *x)
{
	గుర్తులేని దీర్ఘ flags;

	lockdep_assert_RT_లోపల_threaded_ctx();

	raw_spin_తాళం_irqsave(&x->wait.తాళం, flags);
	x->పూర్తి = UINT_MAX;
	swake_పైకి_all_locked(&x->wait);
	raw_spin_unlock_irqrestore(&x->wait.తాళం, flags);
}
EXPORT_చిహ్నం(complete_all);

స్థిర అంతఃవరుస దీర్ఘ __sched
do_wait_for_common(నిర్మాణం completion *x,
		   దీర్ఘ (*action)(దీర్ఘ), దీర్ఘ timeout, పూర్ణాంకం state)
{
	ఒకవేళ (!x->పూర్తి) {
		DECLARE_SWAITQUEUE(wait);

		చేయి {
			ఒకవేళ (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				ఆపు;
			}
			__prepare_to_swait(&x->wait, &wait);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->wait.తాళం);
			timeout = action(timeout);
			raw_spin_తాళం_irq(&x->wait.తాళం);
		} అయితే (!x->పూర్తి && timeout);
		__finish_swait(&x->wait, &wait);
		ఒకవేళ (!x->పూర్తి)
			తిరిగి timeout;
	}
	ఒకవేళ (x->పూర్తి != UINT_MAX)
		x->పూర్తి--;
	తిరిగి timeout ?: 1;
}

స్థిర అంతఃవరుస దీర్ఘ __sched
__wait_for_common(నిర్మాణం completion *x,
		  దీర్ఘ (*action)(దీర్ఘ), దీర్ఘ timeout, పూర్ణాంకం state)
{
	might_sleep();

	complete_acquire(x);

	raw_spin_తాళం_irq(&x->wait.తాళం);
	timeout = do_wait_for_common(x, action, timeout, state);
	raw_spin_unlock_irq(&x->wait.తాళం);

	complete_release(x);

	తిరిగి timeout;
}

స్థిర దీర్ఘ __sched
wait_for_common(నిర్మాణం completion *x, దీర్ఘ timeout, పూర్ణాంకం state)
{
	తిరిగి __wait_for_common(x, కాలపట్టిక_timeout, timeout, state);
}

స్థిర దీర్ఘ __sched
wait_for_common_io(నిర్మాణం completion *x, దీర్ఘ timeout, పూర్ణాంకం state)
{
	తిరిగి __wait_for_common(x, io_కాలపట్టిక_timeout, timeout, state);
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
శూన్య __sched wait_for_completion(నిర్మాణం completion *x)
{
	wait_for_common(x, MAX_కాలపట్టిక_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_చిహ్నం(wait_for_completion);

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
గుర్తులేని దీర్ఘ __sched
wait_for_completion_timeout(నిర్మాణం completion *x, గుర్తులేని దీర్ఘ timeout)
{
	తిరిగి wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_చిహ్నం(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO (which traditionally means blkio only).
 */
శూన్య __sched wait_for_completion_io(నిర్మాణం completion *x)
{
	wait_for_common_io(x, MAX_కాలపట్టిక_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_చిహ్నం(wait_for_completion_io);

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
గుర్తులేని దీర్ఘ __sched
wait_for_completion_io_timeout(నిర్మాణం completion *x, గుర్తులేని దీర్ఘ timeout)
{
	తిరిగి wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_చిహ్నం(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
పూర్ణాంకం __sched wait_for_completion_interruptible(నిర్మాణం completion *x)
{
	దీర్ఘ t = wait_for_common(x, MAX_కాలపట్టిక_TIMEOUT, TASK_INTERRUPTIBLE);

	ఒకవేళ (t == -ERESTARTSYS)
		తిరిగి t;
	తిరిగి 0;
}
EXPORT_చిహ్నం(wait_for_completion_interruptible);

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
దీర్ఘ __sched
wait_for_completion_interruptible_timeout(నిర్మాణం completion *x,
					  గుర్తులేని దీర్ఘ timeout)
{
	తిరిగి wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_చిహ్నం(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
పూర్ణాంకం __sched wait_for_completion_killable(నిర్మాణం completion *x)
{
	దీర్ఘ t = wait_for_common(x, MAX_కాలపట్టిక_TIMEOUT, TASK_KILLABLE);

	ఒకవేళ (t == -ERESTARTSYS)
		తిరిగి t;
	తిరిగి 0;
}
EXPORT_చిహ్నం(wait_for_completion_killable);

పూర్ణాంకం __sched wait_for_completion_state(నిర్మాణం completion *x, గుర్తులేని పూర్ణాంకం state)
{
	దీర్ఘ t = wait_for_common(x, MAX_కాలపట్టిక_TIMEOUT, state);

	ఒకవేళ (t == -ERESTARTSYS)
		తిరిగి t;
	తిరిగి 0;
}
EXPORT_చిహ్నం(wait_for_completion_state);

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
దీర్ఘ __sched
wait_for_completion_killable_timeout(నిర్మాణం completion *x,
				     గుర్తులేని దీర్ఘ timeout)
{
	తిరిగి wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_చిహ్నం(wait_for_completion_killable_timeout);

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
bool try_wait_for_completion(నిర్మాణం completion *x)
{
	గుర్తులేని దీర్ఘ flags;
	bool ret = true;

	/*
	 * Since x->done will need to be locked only
	 * in the non-blocking case, we check x->done
	 * first without taking the lock so we can
	 * return early in the blocking case.
	 */
	ఒకవేళ (!READ_ONCE(x->పూర్తి))
		తిరిగి false;

	raw_spin_తాళం_irqsave(&x->wait.తాళం, flags);
	ఒకవేళ (!x->పూర్తి)
		ret = false;
	లేకపోతే ఒకవేళ (x->పూర్తి != UINT_MAX)
		x->పూర్తి--;
	raw_spin_unlock_irqrestore(&x->wait.తాళం, flags);
	తిరిగి ret;
}
EXPORT_చిహ్నం(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Return: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 *	Note, this will always return true if complete_all() was called on @X.
 */
bool completion_పూర్తి(నిర్మాణం completion *x)
{
	గుర్తులేని దీర్ఘ flags;

	ఒకవేళ (!READ_ONCE(x->పూర్తి))
		తిరిగి false;

	/*
	 * If ->done, we need to wait for complete() to release ->wait.lock
	 * otherwise we can end up freeing the completion before complete()
	 * is done referencing it.
	 */
	raw_spin_తాళం_irqsave(&x->wait.తాళం, flags);
	raw_spin_unlock_irqrestore(&x->wait.తాళం, flags);
	తిరిగి true;
}
EXPORT_చిహ్నం(completion_పూర్తి);
