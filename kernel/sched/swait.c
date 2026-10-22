// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ضمن "sched.h"

فراغ __init_swait_queue_head(بنية swait_قطار_سر *q, ثابت حرف *name,
			     بنية جندرا_class_کلید *کلید)
{
	raw_spin_جندرا_init(&q->جندرا);
	lockdep_سیٹ_class_and_name(&q->جندرا, کلید, name);
	INIT_LIST_سر(&q->task_list);
}
EXPORT_علامت(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
فراغ swake_اتے_locked(بنية swait_قطار_سر *q, صحيح wake_flags)
{
	بنية swait_قطار *curr;

	إذا (list_خالی_تھاواں(&q->task_list))
		أرجع;

	curr = list_پہلا_اندراج(&q->task_list, typeof(*curr), task_list);
	try_منزل_wake_اتے(curr->task, TASK_عام, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_علامت(swake_اتے_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
فراغ swake_اتے_all_locked(بنية swait_قطار_سر *q)
{
	طالما (!list_خالی_تھاواں(&q->task_list))
		swake_اتے_locked(q, 0);
}

فراغ swake_اتے_one(بنية swait_قطار_سر *q)
{
	بنانشانی لما flags;

	raw_spin_جندرا_irqsave(&q->جندرا, flags);
	swake_اتے_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->جندرا, flags);
}
EXPORT_علامت(swake_اتے_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
فراغ swake_اتے_all(بنية swait_قطار_سر *q)
{
	بنية swait_قطار *curr;
	LIST_سر(tmp);

	raw_spin_جندرا_irq(&q->جندرا);
	list_splice_init(&q->task_list, &tmp);
	طالما (!list_خالی_تھاواں(&tmp)) {
		curr = list_پہلا_اندراج(&tmp, typeof(*curr), task_list);

		wake_اتے_state(curr->task, TASK_عام);
		list_del_init(&curr->task_list);

		إذا (list_خالی_تھاواں(&tmp))
			اكسر;

		raw_spin_unlock_irq(&q->جندرا);
		raw_spin_جندرا_irq(&q->جندرا);
	}
	raw_spin_unlock_irq(&q->جندرا);
}
EXPORT_علامت(swake_اتے_all);

فراغ __prepare_to_swait(بنية swait_قطار_سر *q, بنية swait_قطار *wait)
{
	wait->task = current;
	إذا (list_خالی_تھاواں(&wait->task_list))
		list_جوڑو_پوچھ(&wait->task_list, &q->task_list);
}

فراغ prepare_منزل_swait_exclusive(بنية swait_قطار_سر *q, بنية swait_قطار *wait, صحيح state)
{
	بنانشانی لما flags;

	raw_spin_جندرا_irqsave(&q->جندرا, flags);
	__prepare_to_swait(q, wait);
	سیٹ_current_state(state);
	raw_spin_unlock_irqrestore(&q->جندرا, flags);
}
EXPORT_علامت(prepare_منزل_swait_exclusive);

لما prepare_منزل_swait_event(بنية swait_قطار_سر *q, بنية swait_قطار *wait, صحيح state)
{
	بنانشانی لما flags;
	لما ret = 0;

	raw_spin_جندرا_irqsave(&q->جندرا, flags);
	إذا (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} وإلا {
		__prepare_to_swait(q, wait);
		سیٹ_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->جندرا, flags);

	أرجع ret;
}
EXPORT_علامت(prepare_منزل_swait_event);

فراغ __finish_swait(بنية swait_قطار_سر *q, بنية swait_قطار *wait)
{
	__set_current_state(TASK_RUNNING);
	إذا (!list_خالی_تھاواں(&wait->task_list))
		list_del_init(&wait->task_list);
}

فراغ انت_swait(بنية swait_قطار_سر *q, بنية swait_قطار *wait)
{
	بنانشانی لما flags;

	__set_current_state(TASK_RUNNING);

	إذا (!list_خالی_تھاواں_careful(&wait->task_list)) {
		raw_spin_جندرا_irqsave(&q->جندرا, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->جندرا, flags);
	}
}
EXPORT_علامت(انت_swait);
