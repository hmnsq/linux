// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#अन्तर्भाव "sched.h"

शून्य __init_swait_queue_head(संरचना swait_queue_head *q, अचर वर्ण *name,
			     संरचना lock_class_साँचो *साँचो)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, साँचो, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_चिह्न(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
शून्य swake_up_locked(संरचना swait_queue_head *q, पूर्णाङ्क wake_flags)
{
	संरचना swait_queue *curr;

	यदि (list_empty(&q->task_list))
		फर्काऊ;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_चिह्न(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
शून्य swake_up_all_locked(संरचना swait_queue_head *q)
{
	जबसम्म (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

शून्य swake_up_one(संरचना swait_queue_head *q)
{
	अचिन्हित दीर्घ flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_चिह्न(swake_up_one);

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
	जबसम्म (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		यदि (list_empty(&tmp))
			रोक;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_चिह्न(swake_up_all);

शून्य __prepare_to_swait(संरचना swait_queue_head *q, संरचना swait_queue *रुको)
{
	रुको->task = current;
	यदि (list_empty(&रुको->task_list))
		list_add_tail(&रुको->task_list, &q->task_list);
}

शून्य prepare_to_swait_exclusive(संरचना swait_queue_head *q, संरचना swait_queue *रुको, पूर्णाङ्क state)
{
	अचिन्हित दीर्घ flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, रुको);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_चिह्न(prepare_to_swait_exclusive);

दीर्घ prepare_to_swait_event(संरचना swait_queue_head *q, संरचना swait_queue *रुको, पूर्णाङ्क state)
{
	अचिन्हित दीर्घ flags;
	दीर्घ ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	यदि (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&रुको->task_list);
		ret = -ERESTARTSYS;
	} अन्यथा {
		__prepare_to_swait(q, रुको);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	फर्काऊ ret;
}
EXPORT_चिह्न(prepare_to_swait_event);

शून्य __finish_swait(संरचना swait_queue_head *q, संरचना swait_queue *रुको)
{
	__set_current_state(TASK_RUNNING);
	यदि (!list_empty(&रुको->task_list))
		list_del_init(&रुको->task_list);
}

शून्य finish_swait(संरचना swait_queue_head *q, संरचना swait_queue *रुको)
{
	अचिन्हित दीर्घ flags;

	__set_current_state(TASK_RUNNING);

	यदि (!list_empty_careful(&रुको->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&रुको->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_चिह्न(finish_swait);
