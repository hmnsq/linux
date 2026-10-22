// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#समावेश "sched.h"

व्योम __init_swait_queue_head(काष्ठा swait_queue_शीर्ष *q, स्थिर अक्षर *name,
			     काष्ठा lock_class_कुंजी *कुंजी)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, कुंजी, name);
	INIT_LIST_शीर्ष(&q->task_list);
}
EXPORT_प्रतीक(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
व्योम swake_up_locked(काष्ठा swait_queue_शीर्ष *q, पूर्णांक wake_flags)
{
	काष्ठा swait_queue *curr;

	अगर (list_empty(&q->task_list))
		वापस;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_प्रतीक(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
व्योम swake_up_all_locked(काष्ठा swait_queue_शीर्ष *q)
{
	जबतक (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

व्योम swake_up_one(काष्ठा swait_queue_शीर्ष *q)
{
	अचिन्हित दीर्घ flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_प्रतीक(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
व्योम swake_up_all(काष्ठा swait_queue_शीर्ष *q)
{
	काष्ठा swait_queue *curr;
	LIST_शीर्ष(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	जबतक (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		अगर (list_empty(&tmp))
			अवरोध;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_प्रतीक(swake_up_all);

व्योम __prepare_to_swait(काष्ठा swait_queue_शीर्ष *q, काष्ठा swait_queue *रुको)
{
	रुको->task = current;
	अगर (list_empty(&रुको->task_list))
		list_add_पुच्छ(&रुको->task_list, &q->task_list);
}

व्योम prepare_to_swait_exclusive(काष्ठा swait_queue_शीर्ष *q, काष्ठा swait_queue *रुको, पूर्णांक state)
{
	अचिन्हित दीर्घ flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, रुको);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_प्रतीक(prepare_to_swait_exclusive);

दीर्घ prepare_to_swait_event(काष्ठा swait_queue_शीर्ष *q, काष्ठा swait_queue *रुको, पूर्णांक state)
{
	अचिन्हित दीर्घ flags;
	दीर्घ ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	अगर (signal_pending_state(state, current)) {
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

	वापस ret;
}
EXPORT_प्रतीक(prepare_to_swait_event);

व्योम __finish_swait(काष्ठा swait_queue_शीर्ष *q, काष्ठा swait_queue *रुको)
{
	__set_current_state(TASK_RUNNING);
	अगर (!list_empty(&रुको->task_list))
		list_del_init(&रुको->task_list);
}

व्योम finish_swait(काष्ठा swait_queue_शीर्ष *q, काष्ठा swait_queue *रुको)
{
	अचिन्हित दीर्घ flags;

	__set_current_state(TASK_RUNNING);

	अगर (!list_empty_careful(&रुको->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&रुको->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_प्रतीक(finish_swait);
