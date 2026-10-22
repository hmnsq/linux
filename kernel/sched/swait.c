// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ضمن "sched.h"

فراغ __init_swait_queue_head(بنية swait_قطار_سر *q, ثابت حرف *name,
			     بنية تالو_class_ڪنجي *ڪنجي)
{
	raw_spin_تالو_init(&q->تالو);
	lockdep_سيٽ_class_and_name(&q->تالو, ڪنجي, name);
	INIT_LIST_سر(&q->task_list);
}
EXPORT_نشان(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
فراغ swake_مٿي_locked(بنية swait_قطار_سر *q, صحيح wake_flags)
{
	بنية swait_قطار *curr;

	إذا (list_خالي_جايون(&q->task_list))
		أرجع;

	curr = list_پهريون_داخلا(&q->task_list, typeof(*curr), task_list);
	try_منزل_wake_مٿي(curr->task, TASK_عام, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_نشان(swake_مٿي_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
فراغ swake_مٿي_all_locked(بنية swait_قطار_سر *q)
{
	طالما (!list_خالي_جايون(&q->task_list))
		swake_مٿي_locked(q, 0);
}

فراغ swake_مٿي_one(بنية swait_قطار_سر *q)
{
	بنانشان ڊگهو flags;

	raw_spin_تالو_irqsave(&q->تالو, flags);
	swake_مٿي_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->تالو, flags);
}
EXPORT_نشان(swake_مٿي_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
فراغ swake_مٿي_all(بنية swait_قطار_سر *q)
{
	بنية swait_قطار *curr;
	LIST_سر(tmp);

	raw_spin_تالو_irq(&q->تالو);
	list_splice_init(&q->task_list, &tmp);
	طالما (!list_خالي_جايون(&tmp)) {
		curr = list_پهريون_داخلا(&tmp, typeof(*curr), task_list);

		wake_مٿي_state(curr->task, TASK_عام);
		list_del_init(&curr->task_list);

		إذا (list_خالي_جايون(&tmp))
			اكسر;

		raw_spin_unlock_irq(&q->تالو);
		raw_spin_تالو_irq(&q->تالو);
	}
	raw_spin_unlock_irq(&q->تالو);
}
EXPORT_نشان(swake_مٿي_all);

فراغ __prepare_to_swait(بنية swait_قطار_سر *q, بنية swait_قطار *wait)
{
	wait->task = current;
	إذا (list_خالي_جايون(&wait->task_list))
		list_جوڙيو_پڇ(&wait->task_list, &q->task_list);
}

فراغ prepare_منزل_swait_exclusive(بنية swait_قطار_سر *q, بنية swait_قطار *wait, صحيح state)
{
	بنانشان ڊگهو flags;

	raw_spin_تالو_irqsave(&q->تالو, flags);
	__prepare_to_swait(q, wait);
	سيٽ_current_state(state);
	raw_spin_unlock_irqrestore(&q->تالو, flags);
}
EXPORT_نشان(prepare_منزل_swait_exclusive);

ڊگهو prepare_منزل_swait_event(بنية swait_قطار_سر *q, بنية swait_قطار *wait, صحيح state)
{
	بنانشان ڊگهو flags;
	ڊگهو ret = 0;

	raw_spin_تالو_irqsave(&q->تالو, flags);
	إذا (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} وإلا {
		__prepare_to_swait(q, wait);
		سيٽ_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->تالو, flags);

	أرجع ret;
}
EXPORT_نشان(prepare_منزل_swait_event);

فراغ __finish_swait(بنية swait_قطار_سر *q, بنية swait_قطار *wait)
{
	__set_current_state(TASK_RUNNING);
	إذا (!list_خالي_جايون(&wait->task_list))
		list_del_init(&wait->task_list);
}

فراغ پڄاڻي_swait(بنية swait_قطار_سر *q, بنية swait_قطار *wait)
{
	بنانشان ڊگهو flags;

	__set_current_state(TASK_RUNNING);

	إذا (!list_خالي_جايون_careful(&wait->task_list)) {
		raw_spin_تالو_irqsave(&q->تالو, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->تالو, flags);
	}
}
EXPORT_نشان(پڄاڻي_swait);
