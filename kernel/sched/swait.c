// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#অন্তর্ভাব "sched.h"

শূন্য __init_swait_queue_head(সংরচনা swait_queue_head *q, ধ্রুব বর্ণ *name,
			     সংরচনা lock_class_চাবি *চাবি)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, চাবি, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_প্রতীক(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
শূন্য swake_up_locked(সংরচনা swait_queue_head *q, পূর্ণাঙ্ক wake_flags)
{
	সংরচনা swait_queue *curr;

	যদি (list_empty(&q->task_list))
		প্রত্যাগম;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_প্রতীক(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
শূন্য swake_up_all_locked(সংরচনা swait_queue_head *q)
{
	যাবত্ (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

শূন্য swake_up_one(সংরচনা swait_queue_head *q)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_প্রতীক(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
শূন্য swake_up_all(সংরচনা swait_queue_head *q)
{
	সংরচনা swait_queue *curr;
	LIST_HEAD(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	যাবত্ (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		যদি (list_empty(&tmp))
			ভঙ্গ;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_প্রতীক(swake_up_all);

শূন্য __prepare_to_swait(সংরচনা swait_queue_head *q, সংরচনা swait_queue *wait)
{
	wait->task = current;
	যদি (list_empty(&wait->task_list))
		list_add_tail(&wait->task_list, &q->task_list);
}

শূন্য prepare_to_swait_exclusive(সংরচনা swait_queue_head *q, সংরচনা swait_queue *wait, পূর্ণাঙ্ক state)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, wait);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_প্রতীক(prepare_to_swait_exclusive);

long prepare_to_swait_event(সংরচনা swait_queue_head *q, সংরচনা swait_queue *wait, পূর্ণাঙ্ক state)
{
	unsigned long flags;
	long ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	যদি (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} অন্যথা {
		__prepare_to_swait(q, wait);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	প্রত্যাগম ret;
}
EXPORT_প্রতীক(prepare_to_swait_event);

শূন্য __finish_swait(সংরচনা swait_queue_head *q, সংরচনা swait_queue *wait)
{
	__set_current_state(TASK_RUNNING);
	যদি (!list_empty(&wait->task_list))
		list_del_init(&wait->task_list);
}

শূন্য finish_swait(সংরচনা swait_queue_head *q, সংরচনা swait_queue *wait)
{
	unsigned long flags;

	__set_current_state(TASK_RUNNING);

	যদি (!list_empty_careful(&wait->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_প্রতীক(finish_swait);
