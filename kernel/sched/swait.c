// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#شامل "sched.h"

خالی __init_swait_queue_head(ساخت swait_قطار_سر *q, ثابت حرف *name,
			     ساخت تالا_class_کلید *کلید)
{
	raw_spin_تالا_init(&q->تالا);
	lockdep_سیٹ_class_and_name(&q->تالا, کلید, name);
	INIT_LIST_سر(&q->task_list);
}
EXPORT_علامت(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
خالی swake_اوپر_locked(ساخت swait_قطار_سر *q, صحیح wake_flags)
{
	ساخت swait_قطار *curr;

	اگر (list_خالی_خانے(&q->task_list))
		واپس;

	curr = list_پہلا_اندراج(&q->task_list, typeof(*curr), task_list);
	try_مقصد_wake_اوپر(curr->task, TASK_معمول, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_علامت(swake_اوپر_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
خالی swake_اوپر_all_locked(ساخت swait_قطار_سر *q)
{
	جب (!list_خالی_خانے(&q->task_list))
		swake_اوپر_locked(q, 0);
}

خالی swake_اوپر_one(ساخت swait_قطار_سر *q)
{
	غیرعلامتی طویل flags;

	raw_spin_تالا_irqsave(&q->تالا, flags);
	swake_اوپر_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->تالا, flags);
}
EXPORT_علامت(swake_اوپر_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
خالی swake_اوپر_all(ساخت swait_قطار_سر *q)
{
	ساخت swait_قطار *curr;
	LIST_سر(tmp);

	raw_spin_تالا_irq(&q->تالا);
	list_splice_init(&q->task_list, &tmp);
	جب (!list_خالی_خانے(&tmp)) {
		curr = list_پہلا_اندراج(&tmp, typeof(*curr), task_list);

		wake_اوپر_state(curr->task, TASK_معمول);
		list_del_init(&curr->task_list);

		اگر (list_خالی_خانے(&tmp))
			توڑو;

		raw_spin_unlock_irq(&q->تالا);
		raw_spin_تالا_irq(&q->تالا);
	}
	raw_spin_unlock_irq(&q->تالا);
}
EXPORT_علامت(swake_اوپر_all);

خالی __prepare_to_swait(ساخت swait_قطار_سر *q, ساخت swait_قطار *wait)
{
	wait->task = current;
	اگر (list_خالی_خانے(&wait->task_list))
		list_جوڑو_دم(&wait->task_list, &q->task_list);
}

خالی prepare_مقصد_swait_exclusive(ساخت swait_قطار_سر *q, ساخت swait_قطار *wait, صحیح state)
{
	غیرعلامتی طویل flags;

	raw_spin_تالا_irqsave(&q->تالا, flags);
	__prepare_to_swait(q, wait);
	سیٹ_current_state(state);
	raw_spin_unlock_irqrestore(&q->تالا, flags);
}
EXPORT_علامت(prepare_مقصد_swait_exclusive);

طویل prepare_مقصد_swait_event(ساخت swait_قطار_سر *q, ساخت swait_قطار *wait, صحیح state)
{
	غیرعلامتی طویل flags;
	طویل ret = 0;

	raw_spin_تالا_irqsave(&q->تالا, flags);
	اگر (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} ورنہ {
		__prepare_to_swait(q, wait);
		سیٹ_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->تالا, flags);

	واپس ret;
}
EXPORT_علامت(prepare_مقصد_swait_event);

خالی __finish_swait(ساخت swait_قطار_سر *q, ساخت swait_قطار *wait)
{
	__set_current_state(TASK_RUNNING);
	اگر (!list_خالی_خانے(&wait->task_list))
		list_del_init(&wait->task_list);
}

خالی اختتام_swait(ساخت swait_قطار_سر *q, ساخت swait_قطار *wait)
{
	غیرعلامتی طویل flags;

	__set_current_state(TASK_RUNNING);

	اگر (!list_خالی_خانے_careful(&wait->task_list)) {
		raw_spin_تالا_irqsave(&q->تالا, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->تالا, flags);
	}
}
EXPORT_علامت(اختتام_swait);
