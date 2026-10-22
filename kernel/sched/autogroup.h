/* SPDX-License-Identifier: GPL-2.0 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_AUTOGROUP_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_AUTOGROUP_H

#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_AUTOGROUP

ਬਣਤਰ autogroup {
	/*
	 * Reference doesn't mean how many threads attach to this
	 * autogroup now. It just stands for the number of tasks
	 * which could use this autogroup.
	 */
	ਬਣਤਰ kref		kref;
	ਬਣਤਰ task_ਸਮੂਹ	*tg;
	ਬਣਤਰ rw_semaphore	ਤਾਲਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		id;
	ਪੂਰਨ			nice;
};

ਬਾਹਰੀ ਖਾਲੀ autogroup_init(ਬਣਤਰ task_struct *init_task);
ਬਾਹਰੀ ਖਾਲੀ autogroup_free(ਬਣਤਰ task_ਸਮੂਹ *tg);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_ਸਮੂਹ_is_autogroup(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ !!tg->autogroup;
}

ਬਾਹਰੀ bool task_ਚਾਹੁੰਦਾ_autogroup(ਬਣਤਰ task_struct *p, ਬਣਤਰ task_ਸਮੂਹ *tg);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_ਸਮੂਹ *
autogroup_task_ਸਮੂਹ(ਬਣਤਰ task_struct *p, ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_autogroup_enabled;
	ਪੂਰਨ enabled = READ_ONCE(sysctl_sched_autogroup_enabled);

	ਜੇ (enabled && task_ਚਾਹੁੰਦਾ_autogroup(p, tg))
		ਵਾਪਸ p->ਸੰਕੇਤ->autogroup->tg;

	ਵਾਪਸ tg;
}

ਬਾਹਰੀ ਪੂਰਨ autogroup_ਰਸਤਾ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਅੱਖਰ *buf, ਪੂਰਨ buflen);

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_AUTOGROUP: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ autogroup_init(ਬਣਤਰ task_struct *init_task) {  }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ autogroup_free(ਬਣਤਰ task_ਸਮੂਹ *tg) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_ਸਮੂਹ_is_autogroup(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_ਸਮੂਹ *
autogroup_task_ਸਮੂਹ(ਬਣਤਰ task_struct *p, ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ tg;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ autogroup_ਰਸਤਾ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਅੱਖਰ *buf, ਪੂਰਨ buflen)
{
	ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_AUTOGROUP */

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_AUTOGROUP_H */
