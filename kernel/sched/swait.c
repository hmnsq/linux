// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ضمن "sched.h"

فراغ __init_swait_queue_head(بنية swait_صف_سر *q, ثابت حرف *name,
			     بنية قفل_class_کلید *کلید)
{
	raw_spin_قفل_init(&q->قفل);
	lockdep_مجموعه_class_and_name(&q->قفل, کلید, name);
	INIT_LIST_سر(&q->task_list);
}
EXPORT_نماد(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
فراغ swake_به_بالا_locked(بنية swait_صف_سر *q, صحيح wake_flags)
{
	بنية swait_صف *curr;

	إذا (list_خالی(&q->task_list))
		أرجع;

	curr = list_اولی_مدخل(&q->task_list, typeof(*curr), task_list);
	try_مقصد_wake_به_بالا(curr->task, TASK_نرمال, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_نماد(swake_به_بالا_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
فراغ swake_به_بالا_all_locked(بنية swait_صف_سر *q)
{
	طالما (!list_خالی(&q->task_list))
		swake_به_بالا_locked(q, 0);
}

فراغ swake_به_بالا_one(بنية swait_صف_سر *q)
{
	بیعلامت دراز flags;

	raw_spin_قفل_irqsave(&q->قفل, flags);
	swake_به_بالا_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->قفل, flags);
}
EXPORT_نماد(swake_به_بالا_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
فراغ swake_به_بالا_all(بنية swait_صف_سر *q)
{
	بنية swait_صف *curr;
	LIST_سر(tmp);

	raw_spin_قفل_irq(&q->قفل);
	list_splice_init(&q->task_list, &tmp);
	طالما (!list_خالی(&tmp)) {
		curr = list_اولی_مدخل(&tmp, typeof(*curr), task_list);

		wake_به_بالا_state(curr->task, TASK_نرمال);
		list_del_init(&curr->task_list);

		إذا (list_خالی(&tmp))
			اكسر;

		raw_spin_unlock_irq(&q->قفل);
		raw_spin_قفل_irq(&q->قفل);
	}
	raw_spin_unlock_irq(&q->قفل);
}
EXPORT_نماد(swake_به_بالا_all);

فراغ __prepare_to_swait(بنية swait_صف_سر *q, بنية swait_صف *wait)
{
	wait->task = current;
	إذا (list_خالی(&wait->task_list))
		list_جمع_کن_دم(&wait->task_list, &q->task_list);
}

فراغ prepare_مقصد_swait_exclusive(بنية swait_صف_سر *q, بنية swait_صف *wait, صحيح state)
{
	بیعلامت دراز flags;

	raw_spin_قفل_irqsave(&q->قفل, flags);
	__prepare_to_swait(q, wait);
	مجموعه_current_state(state);
	raw_spin_unlock_irqrestore(&q->قفل, flags);
}
EXPORT_نماد(prepare_مقصد_swait_exclusive);

دراز prepare_مقصد_swait_event(بنية swait_صف_سر *q, بنية swait_صف *wait, صحيح state)
{
	بیعلامت دراز flags;
	دراز ret = 0;

	raw_spin_قفل_irqsave(&q->قفل, flags);
	إذا (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} وإلا {
		__prepare_to_swait(q, wait);
		مجموعه_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->قفل, flags);

	أرجع ret;
}
EXPORT_نماد(prepare_مقصد_swait_event);

فراغ __finish_swait(بنية swait_صف_سر *q, بنية swait_صف *wait)
{
	__set_current_state(TASK_RUNNING);
	إذا (!list_خالی(&wait->task_list))
		list_del_init(&wait->task_list);
}

فراغ پایان_swait(بنية swait_صف_سر *q, بنية swait_صف *wait)
{
	بیعلامت دراز flags;

	__set_current_state(TASK_RUNNING);

	إذا (!list_خالی_careful(&wait->task_list)) {
		raw_spin_قفل_irqsave(&q->قفل, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->قفل, flags);
	}
}
EXPORT_نماد(پایان_swait);
