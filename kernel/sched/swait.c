// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#અન્તર્ભાવ "sched.h"

શૂન્ય __init_swait_queue_head(સંરચના swait_queue_head *q, અચલ અક્ષર *name,
			     સંરચના lock_class_ચાવી *ચાવી)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, ચાવી, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_પ્રતીક(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
શૂન્ય swake_up_locked(સંરચના swait_queue_head *q, પૂર્ણાંક wake_flags)
{
	સંરચના swait_queue *curr;

	જો (list_empty(&q->task_list))
		પાછા;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_પ્રતીક(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
શૂન્ય swake_up_all_locked(સંરચના swait_queue_head *q)
{
	જ્યારે (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

શૂન્ય swake_up_one(સંરચના swait_queue_head *q)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_પ્રતીક(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
શૂન્ય swake_up_all(સંરચના swait_queue_head *q)
{
	સંરચના swait_queue *curr;
	LIST_HEAD(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	જ્યારે (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		જો (list_empty(&tmp))
			તોડો;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_પ્રતીક(swake_up_all);

શૂન્ય __prepare_to_swait(સંરચના swait_queue_head *q, સંરચના swait_queue *wait)
{
	wait->task = current;
	જો (list_empty(&wait->task_list))
		list_add_tail(&wait->task_list, &q->task_list);
}

શૂન્ય prepare_to_swait_exclusive(સંરચના swait_queue_head *q, સંરચના swait_queue *wait, પૂર્ણાંક state)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, wait);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_પ્રતીક(prepare_to_swait_exclusive);

long prepare_to_swait_event(સંરચના swait_queue_head *q, સંરચના swait_queue *wait, પૂર્ણાંક state)
{
	unsigned long flags;
	long ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	જો (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} નહીંતર {
		__prepare_to_swait(q, wait);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	પાછા ret;
}
EXPORT_પ્રતીક(prepare_to_swait_event);

શૂન્ય __finish_swait(સંરચના swait_queue_head *q, સંરચના swait_queue *wait)
{
	__set_current_state(TASK_RUNNING);
	જો (!list_empty(&wait->task_list))
		list_del_init(&wait->task_list);
}

શૂન્ય finish_swait(સંરચના swait_queue_head *q, સંરચના swait_queue *wait)
{
	unsigned long flags;

	__set_current_state(TASK_RUNNING);

	જો (!list_empty_careful(&wait->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_પ્રતીક(finish_swait);
