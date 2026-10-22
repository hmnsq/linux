// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#అన్తర్భావ "sched.h"

శూన్య __init_swait_queue_head(నిర్మాణం swait_వరుస_తల *q, స్థిరరాశి అక్షరం *name,
			     నిర్మాణం తాళం_class_కీలకం *కీలకం)
{
	raw_spin_తాళం_init(&q->తాళం);
	lockdep_సమితి_class_and_name(&q->తాళం, కీలకం, name);
	INIT_LIST_తల(&q->task_list);
}
EXPORT_చిహ్నం(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
శూన్య swake_పైకి_locked(నిర్మాణం swait_వరుస_తల *q, పూర్ణాంకం wake_flags)
{
	నిర్మాణం swait_వరుస *curr;

	ఒకవేళ (list_ఖాళీలు(&q->task_list))
		తిరిగి;

	curr = list_మొదటి_నమోదు(&q->task_list, typeof(*curr), task_list);
	try_గమ్యం_wake_పైకి(curr->task, TASK_సాధారణ, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_చిహ్నం(swake_పైకి_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
శూన్య swake_పైకి_all_locked(నిర్మాణం swait_వరుస_తల *q)
{
	అయితే (!list_ఖాళీలు(&q->task_list))
		swake_పైకి_locked(q, 0);
}

శూన్య swake_పైకి_one(నిర్మాణం swait_వరుస_తల *q)
{
	గుర్తులేని దీర్ఘ flags;

	raw_spin_తాళం_irqsave(&q->తాళం, flags);
	swake_పైకి_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->తాళం, flags);
}
EXPORT_చిహ్నం(swake_పైకి_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
శూన్య swake_పైకి_all(నిర్మాణం swait_వరుస_తల *q)
{
	నిర్మాణం swait_వరుస *curr;
	LIST_తల(tmp);

	raw_spin_తాళం_irq(&q->తాళం);
	list_splice_init(&q->task_list, &tmp);
	అయితే (!list_ఖాళీలు(&tmp)) {
		curr = list_మొదటి_నమోదు(&tmp, typeof(*curr), task_list);

		wake_పైకి_state(curr->task, TASK_సాధారణ);
		list_del_init(&curr->task_list);

		ఒకవేళ (list_ఖాళీలు(&tmp))
			ఆపు;

		raw_spin_unlock_irq(&q->తాళం);
		raw_spin_తాళం_irq(&q->తాళం);
	}
	raw_spin_unlock_irq(&q->తాళం);
}
EXPORT_చిహ్నం(swake_పైకి_all);

శూన్య __prepare_to_swait(నిర్మాణం swait_వరుస_తల *q, నిర్మాణం swait_వరుస *wait)
{
	wait->task = current;
	ఒకవేళ (list_ఖాళీలు(&wait->task_list))
		list_కూడు_తోక(&wait->task_list, &q->task_list);
}

శూన్య prepare_గమ్యం_swait_exclusive(నిర్మాణం swait_వరుస_తల *q, నిర్మాణం swait_వరుస *wait, పూర్ణాంకం state)
{
	గుర్తులేని దీర్ఘ flags;

	raw_spin_తాళం_irqsave(&q->తాళం, flags);
	__prepare_to_swait(q, wait);
	సమితి_current_state(state);
	raw_spin_unlock_irqrestore(&q->తాళం, flags);
}
EXPORT_చిహ్నం(prepare_గమ్యం_swait_exclusive);

దీర్ఘ prepare_గమ్యం_swait_event(నిర్మాణం swait_వరుస_తల *q, నిర్మాణం swait_వరుస *wait, పూర్ణాంకం state)
{
	గుర్తులేని దీర్ఘ flags;
	దీర్ఘ ret = 0;

	raw_spin_తాళం_irqsave(&q->తాళం, flags);
	ఒకవేళ (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} లేకపోతే {
		__prepare_to_swait(q, wait);
		సమితి_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->తాళం, flags);

	తిరిగి ret;
}
EXPORT_చిహ్నం(prepare_గమ్యం_swait_event);

శూన్య __finish_swait(నిర్మాణం swait_వరుస_తల *q, నిర్మాణం swait_వరుస *wait)
{
	__set_current_state(TASK_RUNNING);
	ఒకవేళ (!list_ఖాళీలు(&wait->task_list))
		list_del_init(&wait->task_list);
}

శూన్య ముగింపు_swait(నిర్మాణం swait_వరుస_తల *q, నిర్మాణం swait_వరుస *wait)
{
	గుర్తులేని దీర్ఘ flags;

	__set_current_state(TASK_RUNNING);

	ఒకవేళ (!list_ఖాళీలు_careful(&wait->task_list)) {
		raw_spin_తాళం_irqsave(&q->తాళం, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->తాళం, flags);
	}
}
EXPORT_చిహ్నం(ముగింపు_swait);
