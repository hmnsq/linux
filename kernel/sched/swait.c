// SPDX-License-Identifier: GPL-2.0
/*
 * <linux/swait.h> (simple wait queues ) implementation:
 */
#अन्तर्भाव "sched.h"

शून्य __init_swait_queue_head(संरचना swait_queue_शीर्ष *q, ध्रुव वर्ण *name,
			     संरचना तालकम्_class_कुञ्चिका *कुञ्चिका)
{
	raw_spin_तालकम्_init(&q->तालकम्);
	lockdep_समुच्चयः_class_and_name(&q->तालकम्, कुञ्चिका, name);
	INIT_LIST_शीर्ष(&q->task_list);
}
EXPORT_चिह्नम्(__init_swait_queue_head);

/*
 * The thing about the wake_up_state() return value; I think we can ignore it.
 *
 * If for some reason it would return 0, that means the previously waiting
 * task is already running, so it will observe condition true (or has already).
 */
शून्य swake_ऊर्ध्वम्_locked(संरचना swait_queue_शीर्ष *q, पूर्णाङ्क wake_flags)
{
	संरचना swait_queue *curr;

	यदि (list_रिक्तानि(&q->task_list))
		प्रत्यागम;

	curr = list_first_प्रविष्टिः(&q->task_list, typeof(*curr), task_list);
	try_गन्तव्यम्_wake_ऊर्ध्वम्(curr->task, TASK_प्रसामान्यम्, wake_flags);
	list_del_init(&curr->task_list);
}
EXPORT_चिह्नम्(swake_ऊर्ध्वम्_locked);

/*
 * Wake up all waiters. This is an interface which is solely exposed for
 * completions and not for general usage.
 *
 * It is intentionally different from swake_up_all() to allow usage from
 * hard interrupt context and interrupt disabled regions.
 */
शून्य swake_ऊर्ध्वम्_all_locked(संरचना swait_queue_शीर्ष *q)
{
	यावत् (!list_रिक्तानि(&q->task_list))
		swake_ऊर्ध्वम्_locked(q, 0);
}

शून्य swake_ऊर्ध्वम्_one(संरचना swait_queue_शीर्ष *q)
{
	अचिह्नित दीर्घ flags;

	raw_spin_तालकम्_irqsave(&q->तालकम्, flags);
	swake_ऊर्ध्वम्_locked(q, 0);
	raw_spin_unlock_irqrestore(&q->तालकम्, flags);
}
EXPORT_चिह्नम्(swake_ऊर्ध्वम्_one);

/*
 * Does not allow usage from IRQ disabled, since we must be able to
 * release IRQs to guarantee bounded hold time.
 */
शून्य swake_ऊर्ध्वम्_all(संरचना swait_queue_शीर्ष *q)
{
	संरचना swait_queue *curr;
	LIST_शीर्ष(tmp);

	raw_spin_तालकम्_irq(&q->तालकम्);
	list_splice_init(&q->task_list, &tmp);
	यावत् (!list_रिक्तानि(&tmp)) {
		curr = list_first_प्रविष्टिः(&tmp, typeof(*curr), task_list);

		wake_ऊर्ध्वम्_state(curr->task, TASK_प्रसामान्यम्);
		list_del_init(&curr->task_list);

		यदि (list_रिक्तानि(&tmp))
			भङ्ग;

		raw_spin_unlock_irq(&q->तालकम्);
		raw_spin_तालकम्_irq(&q->तालकम्);
	}
	raw_spin_unlock_irq(&q->तालकम्);
}
EXPORT_चिह्नम्(swake_ऊर्ध्वम्_all);

शून्य __prepare_to_swait(संरचना swait_queue_शीर्ष *q, संरचना swait_queue *प्रतीक्षस्व)
{
	प्रतीक्षस्व->task = current;
	यदि (list_रिक्तानि(&प्रतीक्षस्व->task_list))
		list_सङ्कलय_पुच्छ(&प्रतीक्षस्व->task_list, &q->task_list);
}

शून्य prepare_गन्तव्यम्_swait_exclusive(संरचना swait_queue_शीर्ष *q, संरचना swait_queue *प्रतीक्षस्व, पूर्णाङ्क state)
{
	अचिह्नित दीर्घ flags;

	raw_spin_तालकम्_irqsave(&q->तालकम्, flags);
	__prepare_to_swait(q, प्रतीक्षस्व);
	समुच्चयः_current_state(state);
	raw_spin_unlock_irqrestore(&q->तालकम्, flags);
}
EXPORT_चिह्नम्(prepare_गन्तव्यम्_swait_exclusive);

दीर्घ prepare_गन्तव्यम्_swait_event(संरचना swait_queue_शीर्ष *q, संरचना swait_queue *प्रतीक्षस्व, पूर्णाङ्क state)
{
	अचिह्नित दीर्घ flags;
	दीर्घ ret = 0;

	raw_spin_तालकम्_irqsave(&q->तालकम्, flags);
	यदि (signal_pending_state(state, current)) {
		/*
		 * See prepare_to_wait_event(). TL;DR, subsequent swake_up_one()
		 * must not see us.
		 */
		list_del_init(&प्रतीक्षस्व->task_list);
		ret = -ERESTARTSYS;
	} अन्यथा {
		__prepare_to_swait(q, प्रतीक्षस्व);
		समुच्चयः_current_state(state);
	}
	raw_spin_unlock_irqrestore(&q->तालकम्, flags);

	प्रत्यागम ret;
}
EXPORT_चिह्नम्(prepare_गन्तव्यम्_swait_event);

शून्य __finish_swait(संरचना swait_queue_शीर्ष *q, संरचना swait_queue *प्रतीक्षस्व)
{
	__set_current_state(TASK_RUNNING);
	यदि (!list_रिक्तानि(&प्रतीक्षस्व->task_list))
		list_del_init(&प्रतीक्षस्व->task_list);
}

शून्य finish_swait(संरचना swait_queue_शीर्ष *q, संरचना swait_queue *प्रतीक्षस्व)
{
	अचिह्नित दीर्घ flags;

	__set_current_state(TASK_RUNNING);

	यदि (!list_रिक्तानि_careful(&प्रतीक्षस्व->task_list)) {
		raw_spin_तालकम्_irqsave(&q->तालकम्, flags);
		list_del_init(&प्रतीक्षस्व->task_list);
		raw_spin_unlock_irqrestore(&q->तालकम्, flags);
	}
}
EXPORT_चिह्नम्(finish_swait);
