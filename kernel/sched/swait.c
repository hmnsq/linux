// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#अन्तर्भाव "sched.h"

शून्य __init_swait_queue_head(संरचना swait_queue_head *q, ध्रुव वर्ण *name,
			     संरचना lock_class_कुञ्चिका *कुञ्चिका)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, कुञ्चिका, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_चिह्नम्(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
शून्य swake_up_locked(संरचना swait_queue_head *q, पूर्णाङ्क wake_flags)
{
	संरचना swait_queue *curr;

	सचे (list_empty(&q->task_list))
		पटिनिवत्त;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_चिह्नम्(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
शून्य swake_up_all_locked(संरचना swait_queue_head *q)
{
	याव (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

शून्य swake_up_one(संरचना swait_queue_head *q)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_चिह्नम्(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
शून्य swake_up_all(संरचना swait_queue_head *q)
{
	संरचना swait_queue *curr;
	LIST_HEAD(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	याव (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		सचे (list_empty(&tmp))
			भङ्ग;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_चिह्नम्(swake_up_all);

शून्य __prepare_to_swait(संरचना swait_queue_head *q, संरचना swait_queue *wait)
{
	wait->task = current;
	सचे (list_empty(&wait->task_list))
		list_add_tail(&wait->task_list, &q->task_list);
}

शून्य prepare_to_swait_exclusive(संरचना swait_queue_head *q, संरचना swait_queue *wait, पूर्णाङ्क state)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, wait);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_चिह्नम्(prepare_to_swait_exclusive);

long prepare_to_swait_event(संरचना swait_queue_head *q, संरचना swait_queue *wait, पूर्णाङ्क state)
{
	unsigned long flags;
	long ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	सचे (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} अञ्ञथा {
		__prepare_to_swait(q, wait);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	पटिनिवत्त ret;
}
EXPORT_चिह्नम्(prepare_to_swait_event);

शून्य __finish_swait(संरचना swait_queue_head *q, संरचना swait_queue *wait)
{
	__set_current_state(TASK_RUNNING);
	सचे (!list_empty(&wait->task_list))
		list_del_init(&wait->task_list);
}

शून्य finish_swait(संरचना swait_queue_head *q, संरचना swait_queue *wait)
{
	unsigned long flags;

	__set_current_state(TASK_RUNNING);

	सचे (!list_empty_careful(&wait->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_चिह्नम्(finish_swait);
