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

#ضمن <linux/linkage.h>
#ضمن <linux/sched/debug.h>
#ضمن <linux/completion.h>
#ضمن "sched.h"

ساكن فراغ complete_with_flags(بنية completion *x, صحيح wake_flags)
{
	بنانشانی لما flags;

	raw_spin_جندرا_irqsave(&x->wait.جندرا, flags);

	إذا (x->پورا != UINT_MAX)
		x->پورا++;
	swake_اتے_locked(&x->wait, wake_flags);
	raw_spin_unlock_irqrestore(&x->wait.جندرا, flags);
}

فراغ complete_on_current_cpu(بنية completion *x)
{
	أرجع complete_with_flags(x, WF_CURRENT_CPU);
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
فراغ complete(بنية completion *x)
{
	complete_with_flags(x, 0);
}
EXPORT_علامت(complete);

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
فراغ complete_all(بنية completion *x)
{
	بنانشانی لما flags;

	lockdep_assert_RT_وچ_threaded_ctx();

	raw_spin_جندرا_irqsave(&x->wait.جندرا, flags);
	x->پورا = UINT_MAX;
	swake_اتے_all_locked(&x->wait);
	raw_spin_unlock_irqrestore(&x->wait.جندرا, flags);
}
EXPORT_علامت(complete_all);

ساكن مضمن لما __sched
do_wait_for_common(بنية completion *x,
		   لما (*action)(لما), لما timeout, صحيح state)
{
	إذا (!x->پورا) {
		DECLARE_SWAITQUEUE(wait);

		افعل {
			إذا (signal_pending_state(state, current)) {
				timeout = -ERESTARTSYS;
				اكسر;
			}
			__prepare_to_swait(&x->wait, &wait);
			__set_current_state(state);
			raw_spin_unlock_irq(&x->wait.جندرا);
			timeout = action(timeout);
			raw_spin_جندرا_irq(&x->wait.جندرا);
		} طالما (!x->پورا && timeout);
		__finish_swait(&x->wait, &wait);
		إذا (!x->پورا)
			أرجع timeout;
	}
	إذا (x->پورا != UINT_MAX)
		x->پورا--;
	أرجع timeout ?: 1;
}

ساكن مضمن لما __sched
__wait_for_common(بنية completion *x,
		  لما (*action)(لما), لما timeout, صحيح state)
{
	might_sleep();

	complete_acquire(x);

	raw_spin_جندرا_irq(&x->wait.جندرا);
	timeout = do_wait_for_common(x, action, timeout, state);
	raw_spin_unlock_irq(&x->wait.جندرا);

	complete_release(x);

	أرجع timeout;
}

ساكن لما __sched
wait_for_common(بنية completion *x, لما timeout, صحيح state)
{
	أرجع __wait_for_common(x, ویلا_سارنی_timeout, timeout, state);
}

ساكن لما __sched
wait_for_common_io(بنية completion *x, لما timeout, صحيح state)
{
	أرجع __wait_for_common(x, io_ویلا_سارنی_timeout, timeout, state);
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
فراغ __sched wait_for_completion(بنية completion *x)
{
	wait_for_common(x, MAX_ویلا_سارنی_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_علامت(wait_for_completion);

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
بنانشانی لما __sched
wait_for_completion_timeout(بنية completion *x, بنانشانی لما timeout)
{
	أرجع wait_for_common(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_علامت(wait_for_completion_timeout);

/**
 * wait_for_completion_io: - waits for completion of a task
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It is NOT
 * interruptible and there is no timeout. The caller is accounted as waiting
 * for IO (which traditionally means blkio only).
 */
فراغ __sched wait_for_completion_io(بنية completion *x)
{
	wait_for_common_io(x, MAX_ویلا_سارنی_TIMEOUT, TASK_UNINTERRUPTIBLE);
}
EXPORT_علامت(wait_for_completion_io);

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
بنانشانی لما __sched
wait_for_completion_io_timeout(بنية completion *x, بنانشانی لما timeout)
{
	أرجع wait_for_common_io(x, timeout, TASK_UNINTERRUPTIBLE);
}
EXPORT_علامت(wait_for_completion_io_timeout);

/**
 * wait_for_completion_interruptible: - waits for completion of a task (w/intr)
 * @x:  holds the state of this particular completion
 *
 * This waits for completion of a specific task to be signaled. It is
 * interruptible.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
صحيح __sched wait_for_completion_interruptible(بنية completion *x)
{
	لما t = wait_for_common(x, MAX_ویلا_سارنی_TIMEOUT, TASK_INTERRUPTIBLE);

	إذا (t == -ERESTARTSYS)
		أرجع t;
	أرجع 0;
}
EXPORT_علامت(wait_for_completion_interruptible);

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
لما __sched
wait_for_completion_interruptible_timeout(بنية completion *x,
					  بنانشانی لما timeout)
{
	أرجع wait_for_common(x, timeout, TASK_INTERRUPTIBLE);
}
EXPORT_علامت(wait_for_completion_interruptible_timeout);

/**
 * wait_for_completion_killable: - waits for completion of a task (killable)
 * @x:  holds the state of this particular completion
 *
 * This waits to be signaled for completion of a specific task. It can be
 * interrupted by a kill signal.
 *
 * Return: -ERESTARTSYS if interrupted, 0 if completed.
 */
صحيح __sched wait_for_completion_killable(بنية completion *x)
{
	لما t = wait_for_common(x, MAX_ویلا_سارنی_TIMEOUT, TASK_KILLABLE);

	إذا (t == -ERESTARTSYS)
		أرجع t;
	أرجع 0;
}
EXPORT_علامت(wait_for_completion_killable);

صحيح __sched wait_for_completion_state(بنية completion *x, بنانشانی صحيح state)
{
	لما t = wait_for_common(x, MAX_ویلا_سارنی_TIMEOUT, state);

	إذا (t == -ERESTARTSYS)
		أرجع t;
	أرجع 0;
}
EXPORT_علامت(wait_for_completion_state);

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
لما __sched
wait_for_completion_killable_timeout(بنية completion *x,
				     بنانشانی لما timeout)
{
	أرجع wait_for_common(x, timeout, TASK_KILLABLE);
}
EXPORT_علامت(wait_for_completion_killable_timeout);

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
bool try_wait_for_completion(بنية completion *x)
{
	بنانشانی لما flags;
	bool ret = true;

	/*
	 * Since x->done will need to be locked only
	 * in the non-blocking case, we check x->done
	 * first without taking the lock so we can
	 * return early in the blocking case.
	 */
	إذا (!READ_ONCE(x->پورا))
		أرجع false;

	raw_spin_جندرا_irqsave(&x->wait.جندرا, flags);
	إذا (!x->پورا)
		ret = false;
	وإلا إذا (x->پورا != UINT_MAX)
		x->پورا--;
	raw_spin_unlock_irqrestore(&x->wait.جندرا, flags);
	أرجع ret;
}
EXPORT_علامت(try_wait_for_completion);

/**
 *	completion_done - Test to see if a completion has any waiters
 *	@x:	completion structure
 *
 *	Return: 0 if there are waiters (wait_for_completion() in progress)
 *		 1 if there are no waiters.
 *
 *	Note, this will always return true if complete_all() was called on @X.
 */
bool completion_پورا(بنية completion *x)
{
	بنانشانی لما flags;

	إذا (!READ_ONCE(x->پورا))
		أرجع false;

	/*
	 * If ->done, we need to wait for complete() to release ->wait.lock
	 * otherwise we can end up freeing the completion before complete()
	 * is done referencing it.
	 */
	raw_spin_جندرا_irqsave(&x->wait.جندرا, flags);
	raw_spin_unlock_irqrestore(&x->wait.جندرا, flags);
	أرجع true;
}
EXPORT_علامت(completion_پورا);
