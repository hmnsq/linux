// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#அந்தர்பாவ "sched.h"

வெறுமை __init_swait_queue_head(கட்டமைப்பு swait_queue_head *q, மாறிலி எழுத்து *name,
			     கட்டமைப்பு lock_class_சாவி *சாவி)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, சாவி, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_குறியீடு(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
வெறுமை swake_up_locked(கட்டமைப்பு swait_queue_head *q, முழு wake_flags)
{
	கட்டமைப்பு swait_queue *curr;

	எனில் (list_empty(&q->task_list))
		திரும்பு;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_குறியீடு(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
வெறுமை swake_up_all_locked(கட்டமைப்பு swait_queue_head *q)
{
	போது (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

வெறுமை swake_up_one(கட்டமைப்பு swait_queue_head *q)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_குறியீடு(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
வெறுமை swake_up_all(கட்டமைப்பு swait_queue_head *q)
{
	கட்டமைப்பு swait_queue *curr;
	LIST_HEAD(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	போது (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		எனில் (list_empty(&tmp))
			நிறுத்து;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_குறியீடு(swake_up_all);

வெறுமை __prepare_to_swait(கட்டமைப்பு swait_queue_head *q, கட்டமைப்பு swait_queue *wait)
{
	wait->task = current;
	எனில் (list_empty(&wait->task_list))
		list_add_tail(&wait->task_list, &q->task_list);
}

வெறுமை prepare_to_swait_exclusive(கட்டமைப்பு swait_queue_head *q, கட்டமைப்பு swait_queue *wait, முழு state)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, wait);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_குறியீடு(prepare_to_swait_exclusive);

long prepare_to_swait_event(கட்டமைப்பு swait_queue_head *q, கட்டமைப்பு swait_queue *wait, முழு state)
{
	unsigned long flags;
	long ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	எனில் (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} இல்லையெனில் {
		__prepare_to_swait(q, wait);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	திரும்பு ret;
}
EXPORT_குறியீடு(prepare_to_swait_event);

வெறுமை __finish_swait(கட்டமைப்பு swait_queue_head *q, கட்டமைப்பு swait_queue *wait)
{
	__set_current_state(TASK_RUNNING);
	எனில் (!list_empty(&wait->task_list))
		list_del_init(&wait->task_list);
}

வெறுமை finish_swait(கட்டமைப்பு swait_queue_head *q, கட்டமைப்பு swait_queue *wait)
{
	unsigned long flags;

	__set_current_state(TASK_RUNNING);

	எனில் (!list_empty_careful(&wait->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_குறியீடு(finish_swait);
