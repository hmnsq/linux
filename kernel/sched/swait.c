// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਖਾਲੀ __init_swait_queue_head(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਅਚਲ ਅੱਖਰ *name,
			     ਬਣਤਰ ਤਾਲਾ_class_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
	raw_spin_ਤਾਲਾ_init(&q->ਤਾਲਾ);
	lockdep_ਸੈੱਟ_class_and_name(&q->ਤਾਲਾ, ਕੁੰਜੀ, name);
	INIT_LIST_ਸਿਰ(&q->task_list);
}
EXPORT_ਚਿੰਨ੍ਹ(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
ਖਾਲੀ swake_ਉੱਪਰ_locked(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਪੂਰਨ wake_flags)
{
	ਬਣਤਰ swait_ਕਤਾਰ *curr;

	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&q->task_list))
		ਵਾਪਸ;

	curr = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&q->task_list, typeof(*curr), task_list);
	try_ਮੰਜ਼ਿਲ_wake_ਉੱਪਰ(curr->task, TASK_ਸਾਧਾਰਨ, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_ਚਿੰਨ੍ਹ(swake_ਉੱਪਰ_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
ਖਾਲੀ swake_ਉੱਪਰ_all_locked(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q)
{
	ਜਦੋਂ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&q->task_list))
		swake_ਉੱਪਰ_locked(q, 0);
}

ਖਾਲੀ swake_ਉੱਪਰ_one(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	raw_spin_ਤਾਲਾ_irqsave(&q->ਤਾਲਾ, flags);
	swake_ਉੱਪਰ_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->ਤਾਲਾ, flags);
}
EXPORT_ਚਿੰਨ੍ਹ(swake_ਉੱਪਰ_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
ਖਾਲੀ swake_ਉੱਪਰ_all(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q)
{
	ਬਣਤਰ swait_ਕਤਾਰ *curr;
	LIST_ਸਿਰ(tmp);

	raw_spin_ਤਾਲਾ_irq(&q->ਤਾਲਾ);
	list_splice_init(&q->task_list, &tmp);
	ਜਦੋਂ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&tmp)) {
		curr = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&tmp, typeof(*curr), task_list);

		wake_ਉੱਪਰ_state(curr->task, TASK_ਸਾਧਾਰਨ);
		list_del_init(&curr->task_list);

		ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&tmp))
			ਰੋਕੋ;

		raw_spin_unlock_irq(&q->ਤਾਲਾ);
		raw_spin_ਤਾਲਾ_irq(&q->ਤਾਲਾ);
	}
	raw_spin_unlock_irq(&q->ਤਾਲਾ);
}
EXPORT_ਚਿੰਨ੍ਹ(swake_ਉੱਪਰ_all);

ਖਾਲੀ __prepare_to_swait(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਬਣਤਰ swait_ਕਤਾਰ *wait)
{
	wait->task = current;
	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&wait->task_list))
		list_ਜੋੜੋ_ਪੂਛ(&wait->task_list, &q->task_list);
}

ਖਾਲੀ prepare_ਮੰਜ਼ਿਲ_swait_exclusive(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਬਣਤਰ swait_ਕਤਾਰ *wait, ਪੂਰਨ state)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	raw_spin_ਤਾਲਾ_irqsave(&q->ਤਾਲਾ, flags);
	__prepare_to_swait(q, wait);
	ਸੈੱਟ_current_state(state);
	raw_spin_unlock_irqrestore(&q->ਤਾਲਾ, flags);
}
EXPORT_ਚਿੰਨ੍ਹ(prepare_ਮੰਜ਼ਿਲ_swait_exclusive);

ਲੰਮਾ prepare_ਮੰਜ਼ਿਲ_swait_event(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਬਣਤਰ swait_ਕਤਾਰ *wait, ਪੂਰਨ state)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	ਲੰਮਾ ret = 0;

	raw_spin_ਤਾਲਾ_irqsave(&q->ਤਾਲਾ, flags);
	ਜੇ (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} ਨਹੀਂਤਾਂ {
		__prepare_to_swait(q, wait);
		ਸੈੱਟ_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->ਤਾਲਾ, flags);

	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ(prepare_ਮੰਜ਼ਿਲ_swait_event);

ਖਾਲੀ __finish_swait(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਬਣਤਰ swait_ਕਤਾਰ *wait)
{
	__set_current_state(TASK_RUNNING);
	ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&wait->task_list))
		list_del_init(&wait->task_list);
}

ਖਾਲੀ ਅੰਤ_swait(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਬਣਤਰ swait_ਕਤਾਰ *wait)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	__set_current_state(TASK_RUNNING);

	ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ_careful(&wait->task_list)) {
		raw_spin_ਤਾਲਾ_irqsave(&q->ਤਾਲਾ, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->ਤਾਲਾ, flags);
	}
}
EXPORT_ਚਿੰਨ੍ਹ(ਅੰਤ_swait);
