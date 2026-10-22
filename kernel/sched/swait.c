// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ضمن "sched.h"

فراغ __init_swait_queue_head(بنية swait_queue_head *q, ثابت حرف *name,
			     بنية lock_class_key *key)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, key, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_SYMBOL(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
فراغ swake_up_locked(بنية swait_queue_head *q, صحيح wake_flags)
{
	بنية swait_queue *curr;

	ܐܢ (list_empty(&q->task_list))
		ܦܢܐ;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_SYMBOL(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
فراغ swake_up_all_locked(بنية swait_queue_head *q)
{
	ܟܡܐ (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

فراغ swake_up_one(بنية swait_queue_head *q)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
فراغ swake_up_all(بنية swait_queue_head *q)
{
	بنية swait_queue *curr;
	LIST_HEAD(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	ܟܡܐ (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		ܐܢ (list_empty(&tmp))
			اكسر;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_SYMBOL(swake_up_all);

فراغ __prepare_to_swait(بنية swait_queue_head *q, بنية swait_queue *wait)
{
	wait->task = current;
	ܐܢ (list_empty(&wait->task_list))
		list_add_tail(&wait->task_list, &q->task_list);
}

فراغ prepare_to_swait_exclusive(بنية swait_queue_head *q, بنية swait_queue *wait, صحيح state)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, wait);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_SYMBOL(prepare_to_swait_exclusive);

long prepare_to_swait_event(بنية swait_queue_head *q, بنية swait_queue *wait, صحيح state)
{
	unsigned long flags;
	long ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	ܐܢ (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} ܐܚܪܢܐ {
		__prepare_to_swait(q, wait);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	ܦܢܐ ret;
}
EXPORT_SYMBOL(prepare_to_swait_event);

فراغ __finish_swait(بنية swait_queue_head *q, بنية swait_queue *wait)
{
	__set_current_state(TASK_RUNNING);
	ܐܢ (!list_empty(&wait->task_list))
		list_del_init(&wait->task_list);
}

فراغ finish_swait(بنية swait_queue_head *q, بنية swait_queue *wait)
{
	unsigned long flags;

	__set_current_state(TASK_RUNNING);

	ܐܢ (!list_empty_careful(&wait->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_SYMBOL(finish_swait);
