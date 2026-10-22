// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#അന്തര്ഭാവ "sched.h"

ശൂന്യം __init_swait_queue_head(ഘടന swait_queue_head *q, സ്ഥിരം അക്ഷരം *name,
			     ഘടന lock_class_താക്കോൽ *താക്കോൽ)
{
	raw_spin_lock_init(&q->lock);
	lockdep_set_class_and_name(&q->lock, താക്കോൽ, name);
	INIT_LIST_HEAD(&q->task_list);
}
EXPORT_ചിഹ്നം(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
ശൂന്യം swake_up_locked(ഘടന swait_queue_head *q, പൂർണ്ണസംഖ്യ wake_flags)
{
	ഘടന swait_queue *curr;

	എങ്കിൽ (list_empty(&q->task_list))
		തിരികെ;

	curr = list_first_entry(&q->task_list, typeof(*curr), task_list);
	try_to_wake_up(curr->task, TASK_NORMAL, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_ചിഹ്നം(swake_up_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
ശൂന്യം swake_up_all_locked(ഘടന swait_queue_head *q)
{
	ആകുമ്പോൾ (!list_empty(&q->task_list))
		swake_up_locked(q, 0);
}

ശൂന്യം swake_up_one(ഘടന swait_queue_head *q)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	swake_up_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_ചിഹ്നം(swake_up_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
ശൂന്യം swake_up_all(ഘടന swait_queue_head *q)
{
	ഘടന swait_queue *curr;
	LIST_HEAD(tmp);

	raw_spin_lock_irq(&q->lock);
	list_splice_init(&q->task_list, &tmp);
	ആകുമ്പോൾ (!list_empty(&tmp)) {
		curr = list_first_entry(&tmp, typeof(*curr), task_list);

		wake_up_state(curr->task, TASK_NORMAL);
		list_del_init(&curr->task_list);

		എങ്കിൽ (list_empty(&tmp))
			നിർത്തുക;

		raw_spin_unlock_irq(&q->lock);
		raw_spin_lock_irq(&q->lock);
	}
	raw_spin_unlock_irq(&q->lock);
}
EXPORT_ചിഹ്നം(swake_up_all);

ശൂന്യം __prepare_to_swait(ഘടന swait_queue_head *q, ഘടന swait_queue *wait)
{
	wait->task = current;
	എങ്കിൽ (list_empty(&wait->task_list))
		list_add_tail(&wait->task_list, &q->task_list);
}

ശൂന്യം prepare_to_swait_exclusive(ഘടന swait_queue_head *q, ഘടന swait_queue *wait, പൂർണ്ണസംഖ്യ state)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&q->lock, flags);
	__prepare_to_swait(q, wait);
	set_current_state(state);
	raw_spin_unlock_irqrestore(&q->lock, flags);
}
EXPORT_ചിഹ്നം(prepare_to_swait_exclusive);

long prepare_to_swait_event(ഘടന swait_queue_head *q, ഘടന swait_queue *wait, പൂർണ്ണസംഖ്യ state)
{
	unsigned long flags;
	long ret = 0;

	raw_spin_lock_irqsave(&q->lock, flags);
	എങ്കിൽ (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} അല്ലെങ്കിൽ {
		__prepare_to_swait(q, wait);
		set_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->lock, flags);

	തിരികെ ret;
}
EXPORT_ചിഹ്നം(prepare_to_swait_event);

ശൂന്യം __finish_swait(ഘടന swait_queue_head *q, ഘടന swait_queue *wait)
{
	__set_current_state(TASK_RUNNING);
	എങ്കിൽ (!list_empty(&wait->task_list))
		list_del_init(&wait->task_list);
}

ശൂന്യം finish_swait(ഘടന swait_queue_head *q, ഘടന swait_queue *wait)
{
	unsigned long flags;

	__set_current_state(TASK_RUNNING);

	എങ്കിൽ (!list_empty_careful(&wait->task_list)) {
		raw_spin_lock_irqsave(&q->lock, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->lock, flags);
	}
}
EXPORT_ചിഹ്നം(finish_swait);
