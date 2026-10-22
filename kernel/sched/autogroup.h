/* SPDX-License-Identifier: GPL-2.0 */
#నిర్వచితం_కాకపోతే _KERNEL_SCHED_AUTOGROUP_H
#నిర్వచించు _KERNEL_SCHED_AUTOGROUP_H

#అన్తర్భావ "sched.h"

#నిర్వచితమైతే CONFIG_SCHED_AUTOGROUP

నిర్మాణం autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	నిర్మాణం kref		kref;
	నిర్మాణం task_సమూహం	*tg;
	నిర్మాణం rw_semaphore	తాళం;
	గుర్తులేని దీర్ఘ		id;
	పూర్ణాంకం			nice;
};

బాహ్య శూన్య autogroup_init(నిర్మాణం task_struct *init_task);
బాహ్య శూన్య autogroup_free(నిర్మాణం task_సమూహం *tg);

స్థిర అంతఃవరుస bool task_సమూహం_is_autogroup(నిర్మాణం task_సమూహం *tg)
{
	తిరిగి !!tg->autogroup;
}

బాహ్య bool task_కోరుతుంది_autogroup(నిర్మాణం task_struct *p, నిర్మాణం task_సమూహం *tg);

స్థిర అంతఃవరుస నిర్మాణం task_సమూహం *
autogroup_task_సమూహం(నిర్మాణం task_struct *p, నిర్మాణం task_సమూహం *tg)
{
	బాహ్య గుర్తులేని పూర్ణాంకం sysctl_sched_autogroup_enabled;
	పూర్ణాంకం enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ఒకవేళ (enabled && task_కోరుతుంది_autogroup(p, tg))
		తిరిగి p->సంకేతం->autogroup->tg;

	తిరిగి tg;
}

బాహ్య పూర్ణాంకం autogroup_మార్గం(నిర్మాణం task_సమూహం *tg, అక్షరం *buf, పూర్ణాంకం buflen);

#లేకపోతే /* !CONFIG_SCHED_AUTOGROUP: */

స్థిర అంతఃవరుస శూన్య autogroup_init(నిర్మాణం task_struct *init_task) {  }
స్థిర అంతఃవరుస శూన్య autogroup_free(నిర్మాణం task_సమూహం *tg) { }
స్థిర అంతఃవరుస bool task_సమూహం_is_autogroup(నిర్మాణం task_సమూహం *tg)
{
	తిరిగి 0;
}

స్థిర అంతఃవరుస నిర్మాణం task_సమూహం *
autogroup_task_సమూహం(నిర్మాణం task_struct *p, నిర్మాణం task_సమూహం *tg)
{
	తిరిగి tg;
}

స్థిర అంతఃవరుస పూర్ణాంకం autogroup_మార్గం(నిర్మాణం task_సమూహం *tg, అక్షరం *buf, పూర్ణాంకం buflen)
{
	తిరిగి 0;
}

#అయితే_ముగింపు /* !CONFIG_SCHED_AUTOGROUP */

#అయితే_ముగింపు /* _KERNEL_SCHED_AUTOGROUP_H */
