/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_SCHED_AUTOGROUP_H
#define _KERNEL_SCHED_AUTOGROUP_H

#ଅନ୍ତର୍ଭାଵ "sched.h"

#ifdef CONFIG_SCHED_AUTOGROUP

ସଂରଚନା autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	ସଂରଚନା kref		kref;
	ସଂରଚନା task_group	*tg;
	ସଂରଚନା rw_semaphore	lock;
	unsigned long		id;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			nice;
};

ବାହ୍ୟ ଶୂନ୍ୟ autogroup_init(ସଂରଚନା task_struct *init_task);
ବାହ୍ୟ ଶୂନ୍ୟ autogroup_free(ସଂରଚନା task_group *tg);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_group_is_autogroup(ସଂରଚନା task_group *tg)
{
	ଫେରନ୍ତୁ !!tg->autogroup;
}

ବାହ୍ୟ bool task_wants_autogroup(ସଂରଚନା task_struct *p, ସଂରଚନା task_group *tg);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_group *
autogroup_task_group(ସଂରଚନା task_struct *p, ସଂରଚନା task_group *tg)
{
	ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_autogroup_enabled;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ଯଦି (enabled && task_wants_autogroup(p, tg))
		ଫେରନ୍ତୁ p->ସଙ୍କେତ->autogroup->tg;

	ଫେରନ୍ତୁ tg;
}

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା autogroup_path(ସଂରଚନା task_group *tg, ଅକ୍ଷର *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା buflen);

#ଅନ୍ୟଥା /* !CONFIG_SCHED_AUTOGROUP: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ autogroup_init(ସଂରଚନା task_struct *init_task) {  }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ autogroup_free(ସଂରଚନା task_group *tg) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_group_is_autogroup(ସଂରଚନା task_group *tg)
{
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_group *
autogroup_task_group(ସଂରଚନା task_struct *p, ସଂରଚନା task_group *tg)
{
	ଫେରନ୍ତୁ tg;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା autogroup_path(ସଂରଚନା task_group *tg, ଅକ୍ଷର *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା buflen)
{
	ଫେରନ୍ତୁ 0;
}

#endif /* !CONFIG_SCHED_AUTOGROUP */

#endif /* _KERNEL_SCHED_AUTOGROUP_H */
