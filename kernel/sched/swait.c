// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ಶೂನ್ಯ __init_swait_queue_head(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			     ರಚನೆ ಬೀಗ_class_ಕೀಲಿ *ಕೀಲಿ)
{
	raw_spin_ಬೀಗ_init(&q->ಬೀಗ);
	lockdep_ಗಣ_class_and_name(&q->ಬೀಗ, ಕೀಲಿ, name);
	INIT_LIST_ತಲೆ(&q->task_list);
}
EXPORT_ಚಿಹ್ನೆ(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
ಶೂನ್ಯ swake_ಮೇಲೆ_locked(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ರಚನೆ swait_ಸರತಿ *curr;

	ಆದರೆ (list_ಖಾಲಿಗಳು(&q->task_list))
		ಹಿಂತಿರುಗಿ;

	curr = list_ಮೊದಲ_ನಮೂದು(&q->task_list, typeof(*curr), task_list);
	try_ಗಮ್ಯ_wake_ಮೇಲೆ(curr->task, TASK_ಸಾಮಾನ್ಯ, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_ಚಿಹ್ನೆ(swake_ಮೇಲೆ_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
ಶೂನ್ಯ swake_ಮೇಲೆ_all_locked(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q)
{
	ಆಗ (!list_ಖಾಲಿಗಳು(&q->task_list))
		swake_ಮೇಲೆ_locked(q, 0);
}

ಶೂನ್ಯ swake_ಮೇಲೆ_one(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	raw_spin_ಬೀಗ_irqsave(&q->ಬೀಗ, flags);
	swake_ಮೇಲೆ_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ(swake_ಮೇಲೆ_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
ಶೂನ್ಯ swake_ಮೇಲೆ_all(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q)
{
	ರಚನೆ swait_ಸರತಿ *curr;
	LIST_ತಲೆ(tmp);

	raw_spin_ಬೀಗ_irq(&q->ಬೀಗ);
	list_splice_init(&q->task_list, &tmp);
	ಆಗ (!list_ಖಾಲಿಗಳು(&tmp)) {
		curr = list_ಮೊದಲ_ನಮೂದು(&tmp, typeof(*curr), task_list);

		wake_ಮೇಲೆ_state(curr->task, TASK_ಸಾಮಾನ್ಯ);
		list_del_init(&curr->task_list);

		ಆದರೆ (list_ಖಾಲಿಗಳು(&tmp))
			ನಿಲ್ಲಿಸು;

		raw_spin_unlock_irq(&q->ಬೀಗ);
		raw_spin_ಬೀಗ_irq(&q->ಬೀಗ);
	}
	raw_spin_unlock_irq(&q->ಬೀಗ);
}
EXPORT_ಚಿಹ್ನೆ(swake_ಮೇಲೆ_all);

ಶೂನ್ಯ __prepare_to_swait(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ರಚನೆ swait_ಸರತಿ *wait)
{
	wait->task = current;
	ಆದರೆ (list_ಖಾಲಿಗಳು(&wait->task_list))
		list_ಕೂಡಿಸು_ಬಾಲ(&wait->task_list, &q->task_list);
}

ಶೂನ್ಯ prepare_ಗಮ್ಯ_swait_exclusive(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ರಚನೆ swait_ಸರತಿ *wait, ಪೂರ್ಣಾಂಕ state)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	raw_spin_ಬೀಗ_irqsave(&q->ಬೀಗ, flags);
	__prepare_to_swait(q, wait);
	ಗಣ_current_state(state);
	raw_spin_unlock_irqrestore(&q->ಬೀಗ, flags);
}
EXPORT_ಚಿಹ್ನೆ(prepare_ಗಮ್ಯ_swait_exclusive);

ದೀರ್ಘ prepare_ಗಮ್ಯ_swait_event(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ರಚನೆ swait_ಸರತಿ *wait, ಪೂರ್ಣಾಂಕ state)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	ದೀರ್ಘ ret = 0;

	raw_spin_ಬೀಗ_irqsave(&q->ಬೀಗ, flags);
	ಆದರೆ (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&wait->task_list);
		ret = -ERESTARTSYS;
	} ಇಲ್ಲದಿದ್ದರೆ {
		__prepare_to_swait(q, wait);
		ಗಣ_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->ಬೀಗ, flags);

	ಹಿಂತಿರುಗಿ ret;
}
EXPORT_ಚಿಹ್ನೆ(prepare_ಗಮ್ಯ_swait_event);

ಶೂನ್ಯ __finish_swait(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ರಚನೆ swait_ಸರತಿ *wait)
{
	__set_current_state(TASK_RUNNING);
	ಆದರೆ (!list_ಖಾಲಿಗಳು(&wait->task_list))
		list_del_init(&wait->task_list);
}

ಶೂನ್ಯ ಮುಕ್ತಾಯ_swait(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ರಚನೆ swait_ಸರತಿ *wait)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	__set_current_state(TASK_RUNNING);

	ಆದರೆ (!list_ಖಾಲಿಗಳು_careful(&wait->task_list)) {
		raw_spin_ಬೀಗ_irqsave(&q->ಬೀಗ, flags);
		list_del_init(&wait->task_list);
		raw_spin_unlock_irqrestore(&q->ಬೀಗ, flags);
	}
}
EXPORT_ಚಿಹ್ನೆ(ಮುಕ್ತಾಯ_swait);
