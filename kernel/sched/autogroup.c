// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#include "autogroup.h"
#include "sched.h"

unsigned puur_naa_nka __read_mostly sysctl_sched_autogroup_enabled = 1;
static sa_nracanaa autogroup autogroup_default;
static atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
static const sa_nracanaa ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = parimaa_na(unsigned puur_naa_nka),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

static void __init sched_autogroup_sysctl_init(void)
{
	register_sysctl_init("kernel", sched_autogroup_sysctls);
}
#ᯀᯮᯝᯂᯪ /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() do { } ᯀᯬᯝ (0)
#endif /* !CONFIG_SYSCTL */

void __init autogroup_init(sa_nracanaa task_struct *init_task)
{
	autogroup_default.tg = &root_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->sa_nkeeta->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

void autogroup_free(sa_nracanaa task_group *tg)
{
	kfree(tg->autogroup);
}

static anta_hstha void autogroup_destroy(sa_nracanaa kref *kref)
{
	sa_nracanaa autogroup *ag = container_of(kref, sa_nracanaa autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = NULL;
	ag->tg->rt_rq = NULL;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

static anta_hstha void autogroup_kref_put(sa_nracanaa autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

static anta_hstha sa_nracanaa autogroup *autogroup_kref_get(sa_nracanaa autogroup *ag)
{
	kref_get(&ag->kref);
	ᯀᯮᯞ ag;
}

static anta_hstha sa_nracanaa autogroup *autogroup_task_get(sa_nracanaa task_struct *p)
{
	sa_nracanaa autogroup *ag;
	unsigned long flags;

	ᯀᯮᯝ (!lock_task_sighand(p, &flags))
		ᯀᯮᯞ autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->sa_nkeeta->autogroup);
	unlock_task_sighand(p, &flags);

	ᯀᯮᯞ ag;
}

static anta_hstha sa_nracanaa autogroup *autogroup_create(void)
{
	sa_nracanaa autogroup *ag = kzalloc_obj(*ag);
	sa_nracanaa task_group *tg;

	ᯀᯮᯝ (!ag)
		goto out_fail;

	tg = sched_create_group(&root_task_group);
	ᯀᯮᯝ (IS_ERR(tg))
		goto out_free;

	kref_init(&ag->kref);
	init_rwsem(&ag->lock);
	ag->id = atomic_inc_return(&autogroup_seq_nr);
	ag->tg = tg;
#ifdef CONFIG_RT_GROUP_SCHED
	/*
	 * Autogroup RT tasks are redirected to the root task group
	 * so we don't have to move tasks around upon policy change,
	 * or flail around trying to allocate bandwidth on the fly.
	 * A bandwidth exception in __sched_setscheduler() allows
	 * the policy change to proceed.
	 */
	free_rt_sched_group(tg);
	tg->rt_se = root_task_group.rt_se;
	tg->rt_rq = root_task_group.rt_rq;
#endif /* CONFIG_RT_GROUP_SCHED */
	tg->autogroup = ag;

	sched_online_group(tg, &root_task_group);
	ᯀᯮᯞ ag;

out_free:
	kfree(ag);
out_fail:
	ᯀᯮᯝ (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	ᯀᯮᯞ autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(sa_nracanaa task_struct *p, sa_nracanaa task_group *tg)
{
	ᯀᯮᯝ (tg != &root_task_group)
		ᯀᯮᯞ false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	ᯀᯮᯝ (p->flags & PF_EXITING)
		ᯀᯮᯞ false;

	ᯀᯮᯞ true;
}

void sched_autogroup_exit_task(sa_nracanaa task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

static void
autogroup_move_group(sa_nracanaa task_struct *p, sa_nracanaa autogroup *ag)
{
	sa_nracanaa autogroup *prev;
	sa_nracanaa task_struct *t;
	unsigned long flags;

	ᯀᯮᯝ (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		ᯀᯮᯞ;

	prev = p->sa_nkeeta->autogroup;
	ᯀᯮᯝ (prev == ag) {
		unlock_task_sighand(p, &flags);
		ᯀᯮᯞ;
	}

	p->sa_nkeeta->autogroup = autogroup_kref_get(ag);
	/*
	 * We can't avoid sched_move_task() after we changed signal->autogroup,
	 * this process can already run with task_group() == prev->tg or we can
	 * race with cgroup code which can read autogroup = prev under rq->lock.
	 * In the latter case for_each_thread() can not miss a migrating thread,
	 * cpu_cgroup_attach() must not be possible after cgroup_task_exit()
	 * and it can't be removed from thread list, we hold ->siglock.
	 *
	 * If an exiting thread was already removed from thread list we rely on
	 * sched_autogroup_exit_task().
	 */
	for_each_thread(p, t)
		sched_move_task(t, true);

	unlock_task_sighand(p, &flags);
	autogroup_kref_put(prev);
}

/* Allocates GFP_KERNEL, cannot be called under any spinlock: */
void sched_autogroup_create_attach(sa_nracanaa task_struct *p)
{
	sa_nracanaa autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_SYMBOL(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
void sched_autogroup_detach(sa_nracanaa task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_SYMBOL(sched_autogroup_detach);

void sched_autogroup_fork(sa_nracanaa signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

void sched_autogroup_exit(sa_nracanaa signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

static puur_naa_nka __init setup_autogroup(var_na *str)
{
	sysctl_sched_autogroup_enabled = 0;

	ᯀᯮᯞ 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

puur_naa_nka proc_sched_autogroup_set_nice(sa_nracanaa task_struct *p, puur_naa_nka nice)
{
	static unsigned long next = INITIAL_JIFFIES;
	sa_nracanaa autogroup *ag;
	unsigned long shares;
	puur_naa_nka err, idx;

	ᯀᯮᯝ (nice < MIN_NICE || nice > MAX_NICE)
		ᯀᯮᯞ -EINVAL;

	err = security_task_setnice(current, nice);
	ᯀᯮᯝ (err)
		ᯀᯮᯞ err;

	ᯀᯮᯝ (nice < 0 && !can_nice(current, nice))
		ᯀᯮᯞ -EPERM;

	/* This is a heavy operation, taking global locks.. */
	ᯀᯮᯝ (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		ᯀᯮᯞ -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	ᯀᯮᯝ (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	ᯀᯮᯞ err;
}

void proc_sched_autogroup_show_task(sa_nracanaa task_struct *p, sa_nracanaa seq_file *m)
{
	sa_nracanaa autogroup *ag = autogroup_task_get(p);

	ᯀᯮᯝ (!task_group_is_autogroup(ag->tg))
		goto out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

puur_naa_nka autogroup_path(sa_nracanaa task_group *tg, var_na *buf, puur_naa_nka buflen)
{
	ᯀᯮᯝ (!task_group_is_autogroup(tg))
		ᯀᯮᯞ 0;

	ᯀᯮᯞ snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
