// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ಶೂನ್ಯ __init_swait_queue_head(ಸಂರಚನಾ swait_queue_head *q, ಧ್ರುವ ವರ್ಣ *name,
			     ಸಂರಚನಾ lock_class_ಕೀಲಿ *ಕೀಲಿ)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, ಕೀಲಿ, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_ಚಿಹ್ನೆ(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
ಶೂನ್ಯ swake_up_locked(ಸಂರಚನಾ swait_queue_head *q, ಪೂರ್ಣಾಙ್ಕ wake_flags)
{
	ಸಂರಚನಾ swait_queue *curr;

	ಯದಿ (list_empty(&q->task_list))
		ಪ್ರತ್ಯಾಗಮ;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_ಚಿಹ್ನೆ(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
ಶೂನ್ಯ swake_up_all_locked(ಸಂರಚನಾ swait_queue_head *q)
{
	ಯಾವತ್ (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

ಶೂನ್ಯ swake_up_one(ಸಂರಚನಾ swait_queue_head *q)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_ಚಿಹ್ನೆ(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
ಶೂನ್ಯ swake_up_all(ಸಂರಚನಾ swait_queue_head *q)
{
	ಸಂರಚನಾ swait_queue *curr;
	LIST_HEAD(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	ಯಾವತ್ (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		ಯದಿ (list_empty(&tmp))
			ಭಙ್ಗ;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_ಚಿಹ್ನೆ(swake_up_all);

ಶೂನ್ಯ __prepare_to_swait(ಸಂರಚನಾ swait_queue_head *q, ಸಂರಚನಾ swait_queue *wait)
{
	wait->task = current;
	ಯದಿ (list_empty(&wait->task_list))
		list_add_tail(&wait->task_list, &q->task_list);
}

ಶೂನ್ಯ prepare_to_swait_exclusive(ಸಂರಚನಾ swait_queue_head *q, ಸಂರಚನಾ swait_queue *wait, ಪೂರ್ಣಾಙ್ಕ state)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, wait);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_ಚಿಹ್ನೆ(prepare_to_swait_exclusive);

long prepare_to_swait_event(ಸಂರಚನಾ swait_queue_head *q, ಸಂರಚನಾ swait_queue *wait, ಪೂರ್ಣಾಙ್ಕ state)
{
	unsigned long flags;
	long ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	ಯದಿ (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} ಅನ್ಯಥಾ {
		__prepare_to_swait(q, wait);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	ಪ್ರತ್ಯಾಗಮ ret;
}
EXPORT_ಚಿಹ್ನೆ(prepare_to_swait_event);

ಶೂನ್ಯ __finish_swait(ಸಂರಚನಾ swait_queue_head *q, ಸಂರಚನಾ swait_queue *wait)
{
	__set_current_state(TASK_RUNNING);
	ಯದಿ (!list_empty(&wait->task_list))
		list_del_init(&wait->task_list);
}

ಶೂನ್ಯ finish_swait(ಸಂರಚನಾ swait_queue_head *q, ಸಂರಚನಾ swait_queue *wait)
{
	unsigned long flags;

	__set_current_state(TASK_RUNNING);

	ಯದಿ (!list_empty_careful(&wait->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_ಚಿಹ್ನೆ(finish_swait);
