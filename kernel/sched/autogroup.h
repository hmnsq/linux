/* SPDX-License-Identifier: GPL-2.0 */
#यदि_अपरिभाषितम् _KERNEL_SCHED_AUTOGROUP_H
#परिभाषय _KERNEL_SCHED_AUTOGROUP_H

#अन्तर्भाव "sched.h"

#यदि_परिभाषितम् CONFIG_SCHED_AUTOGROUP

संरचना autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	संरचना kref		kref;
	संरचना task_गणः	*tg;
	संरचना rw_semaphore	तालकम्;
	अचिह्नित दीर्घ		id;
	पूर्णाङ्क			nice;
};

बाह्य शून्य autogroup_init(संरचना task_struct *init_task);
बाह्य शून्य autogroup_free(संरचना task_गणः *tg);

स्थिर अन्तःस्थ bool task_गणः_is_autogroup(संरचना task_गणः *tg)
{
	प्रत्यागम !!tg->autogroup;
}

बाह्य bool task_इच्छति_autogroup(संरचना task_struct *p, संरचना task_गणः *tg);

स्थिर अन्तःस्थ संरचना task_गणः *
autogroup_task_गणः(संरचना task_struct *p, संरचना task_गणः *tg)
{
	बाह्य अचिह्नित पूर्णाङ्क sysctl_sched_autogroup_enabled;
	पूर्णाङ्क enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	यदि (enabled && task_इच्छति_autogroup(p, tg))
		प्रत्यागम p->सङ्केत->autogroup->tg;

	प्रत्यागम tg;
}

बाह्य पूर्णाङ्क autogroup_मार्गः(संरचना task_गणः *tg, वर्ण *buf, पूर्णाङ्क buflen);

#अन्यथा /* !CONFIG_SCHED_AUTOGROUP: */

स्थिर अन्तःस्थ शून्य autogroup_init(संरचना task_struct *init_task) {  }
स्थिर अन्तःस्थ शून्य autogroup_free(संरचना task_गणः *tg) { }
स्थिर अन्तःस्थ bool task_गणः_is_autogroup(संरचना task_गणः *tg)
{
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ संरचना task_गणः *
autogroup_task_गणः(संरचना task_struct *p, संरचना task_गणः *tg)
{
	प्रत्यागम tg;
}

स्थिर अन्तःस्थ पूर्णाङ्क autogroup_मार्गः(संरचना task_गणः *tg, वर्ण *buf, पूर्णाङ्क buflen)
{
	प्रत्यागम 0;
}

#यदि_अन्तः /* !CONFIG_SCHED_AUTOGROUP */

#यदि_अन्तः /* _KERNEL_SCHED_AUTOGROUP_H */
