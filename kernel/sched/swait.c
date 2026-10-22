// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ضمن "sched.h"

فراغ __init_swait_queue_head(بنية swait_کتار_سر *q, ثابت حرف *name,
			     بنية قلف_class_کلی *کلی)
{
	raw_spin_قلف_init(&q->قلف);
	lockdep_سېټ_class_and_name(&q->قلف, کلی, name);
	INIT_LIST_سر(&q->task_list);
}
EXPORT_نښه(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
فراغ swake_پورته_locked(بنية swait_کتار_سر *q, صحيح wake_flags)
{
	بنية swait_کتار *curr;

	إذا (list_تشه(&q->task_list))
		أرجع;

	curr = list_لومړی_ثبت(&q->task_list, typeof(*curr), task_list);
	try_منزل_wake_پورته(curr->task, TASK_نورمال, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_نښه(swake_پورته_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
فراغ swake_پورته_all_locked(بنية swait_کتار_سر *q)
{
	طالما (!list_تشه(&q->task_list))
		swake_پورته_locked(q, 0);
}

فراغ swake_پورته_one(بنية swait_کتار_سر *q)
{
	بینښه اوږد flags;

	raw_spin_قلف_irqsave(&q->قلف, flags);
	swake_پورته_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->قلف, flags);
}
EXPORT_نښه(swake_پورته_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
فراغ swake_پورته_all(بنية swait_کتار_سر *q)
{
	بنية swait_کتار *curr;
	LIST_سر(tmp);

	raw_spin_قلف_irq(&q->قلف);
	list_splice_init(&q->task_list, &tmp);
	طالما (!list_تشه(&tmp)) {
		curr = list_لومړی_ثبت(&tmp, typeof(*curr), task_list);

		wake_پورته_state(curr->task, TASK_نورمال);
		list_del_init(&curr->task_list);

		إذا (list_تشه(&tmp))
			اكسر;

		raw_spin_unlock_irq(&q->قلف);
		raw_spin_قلف_irq(&q->قلف);
	}
	raw_spin_unlock_irq(&q->قلف);
}
EXPORT_نښه(swake_پورته_all);

فراغ __prepare_to_swait(بنية swait_کتار_سر *q, بنية swait_کتار *wait)
{
	wait->task = current;
	إذا (list_تشه(&wait->task_list))
		list_جمع_کړه_لکۍ(&wait->task_list, &q->task_list);
}

فراغ prepare_منزل_swait_exclusive(بنية swait_کتار_سر *q, بنية swait_کتار *wait, صحيح state)
{
	بینښه اوږد flags;

	raw_spin_قلف_irqsave(&q->قلف, flags);
	__prepare_to_swait(q, wait);
	سېټ_current_state(state);
	raw_spin_unlock_irqrestore(&q->قلف, flags);
}
EXPORT_نښه(prepare_منزل_swait_exclusive);

اوږد prepare_منزل_swait_event(بنية swait_کتار_سر *q, بنية swait_کتار *wait, صحيح state)
{
	بینښه اوږد flags;
	اوږد ret = 0;

	raw_spin_قلف_irqsave(&q->قلف, flags);
	إذا (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} وإلا {
		__prepare_to_swait(q, wait);
		سېټ_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->قلف, flags);

	أرجع ret;
}
EXPORT_نښه(prepare_منزل_swait_event);

فراغ __finish_swait(بنية swait_کتار_سر *q, بنية swait_کتار *wait)
{
	__set_current_state(TASK_RUNNING);
	إذا (!list_تشه(&wait->task_list))
		list_del_init(&wait->task_list);
}

فراغ پای_swait(بنية swait_کتار_سر *q, بنية swait_کتار *wait)
{
	بینښه اوږد flags;

	__set_current_state(TASK_RUNNING);

	إذا (!list_تشه_careful(&wait->task_list)) {
		raw_spin_قلف_irqsave(&q->قلف, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->قلف, flags);
	}
}
EXPORT_نښه(پای_swait);
