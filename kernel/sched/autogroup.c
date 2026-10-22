// SPDX-License-Identifier: GPL-2.0

/*
 * Auto-group scheduling implementation:
 */

#include "autogroup.h"
#include "sched.h"

unsigned целое __read_mostly sysctl_sched_autogroup_enabled = 1;
static struct autogroup autogroup_default;
static atomic_t autogroup_seq_nr;

#ifdef CONFIG_SYSCTL
static const struct ctl_table sched_autogroup_sysctls[] = {
	{
		.procname       = "sched_autogroup_enabled",
		.data           = &sysctl_sched_autogroup_enabled,
		.maxlen         = sizeof(unsigned целое),
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
#иначе /* !CONFIG_SYSCTL: */
#define sched_autogroup_sysctl_init() do { } пока (0)
#endif /* !CONFIG_SYSCTL */

void __init autogroup_init(struct task_struct *init_task)
{
	autogroup_default.tg = &root_task_group;
	kref_init(&autogroup_default.kref);
	init_rwsem(&autogroup_default.lock);
	init_task->signal->autogroup = &autogroup_default;
	sched_autogroup_sysctl_init();
}

void autogroup_free(struct task_group *tg)
{
	kfree(tg->autogroup);
}

static inline void autogroup_destroy(struct kref *kref)
{
	struct autogroup *ag = container_of(kref, struct autogroup, kref);

#ifdef CONFIG_RT_GROUP_SCHED
	/* We've redirected RT tasks to the root task group... */
	ag->tg->rt_se = NULL;
	ag->tg->rt_rq = NULL;
#endif
	sched_release_group(ag->tg);
	sched_destroy_group(ag->tg);
}

static inline void autogroup_kref_put(struct autogroup *ag)
{
	kref_put(&ag->kref, autogroup_destroy);
}

static inline struct autogroup *autogroup_kref_get(struct autogroup *ag)
{
	kref_get(&ag->kref);
	вернуть ag;
}

static inline struct autogroup *autogroup_task_get(struct task_struct *p)
{
	struct autogroup *ag;
	unsigned long flags;

	если (!lock_task_sighand(p, &flags))
		вернуть autogroup_kref_get(&autogroup_default);

	ag = autogroup_kref_get(p->signal->autogroup);
	unlock_task_sighand(p, &flags);

	вернуть ag;
}

static inline struct autogroup *autogroup_create(void)
{
	struct autogroup *ag = kzalloc_obj(*ag);
	struct task_group *tg;

	если (!ag)
		goto out_fail;

	tg = sched_create_group(&root_task_group);
	если (IS_ERR(tg))
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
	вернуть ag;

out_free:
	kfree(ag);
out_fail:
	если (printk_ratelimit()) {
		printk(KERN_WARNING "autogroup_create: %s failure.\n",
			ag ? "sched_create_group()" : "kzalloc()");
	}

	вернуть autogroup_kref_get(&autogroup_default);
}

bool task_wants_autogroup(struct task_struct *p, struct task_group *tg)
{
	если (tg != &root_task_group)
		вернуть false;
	/*
	 * If we race with autogroup_move_group() the caller can use the old
	 * value of signal->autogroup but in this case sched_move_task() will
	 * be called again before autogroup_kref_put().
	 *
	 * However, there is no way sched_autogroup_exit_task() could tell us
	 * to avoid autogroup->tg, so we abuse PF_EXITING flag for this case.
	 */
	если (p->flags & PF_EXITING)
		вернуть false;

	вернуть true;
}

void sched_autogroup_exit_task(struct task_struct *p)
{
	/*
	 * We are going to call exit_notify() and autogroup_move_group() can't
	 * see this thread after that: we can no longer use signal->autogroup.
	 * See the PF_EXITING check in task_wants_autogroup().
	 */
	sched_move_task(p, true);
}

static void
autogroup_move_group(struct task_struct *p, struct autogroup *ag)
{
	struct autogroup *prev;
	struct task_struct *t;
	unsigned long flags;

	если (WARN_ON_ONCE(!lock_task_sighand(p, &flags)))
		вернуть;

	prev = p->signal->autogroup;
	если (prev == ag) {
		unlock_task_sighand(p, &flags);
		вернуть;
	}

	p->signal->autogroup = autogroup_kref_get(ag);
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
void sched_autogroup_create_attach(struct task_struct *p)
{
	struct autogroup *ag = autogroup_create();

	autogroup_move_group(p, ag);

	/* Drop extra reference added by autogroup_create(): */
	autogroup_kref_put(ag);
}
EXPORT_SYMBOL(sched_autogroup_create_attach);

/* Cannot be called under siglock. Currently has no users: */
void sched_autogroup_detach(struct task_struct *p)
{
	autogroup_move_group(p, &autogroup_default);
}
EXPORT_SYMBOL(sched_autogroup_detach);

void sched_autogroup_fork(struct signal_struct *sig)
{
	sig->autogroup = autogroup_task_get(current);
}

void sched_autogroup_exit(struct signal_struct *sig)
{
	autogroup_kref_put(sig->autogroup);
}

static целое __init setup_autogroup(char *str)
{
	sysctl_sched_autogroup_enabled = 0;

	вернуть 1;
}
__setup("noautogroup", setup_autogroup);

#ifdef CONFIG_PROC_FS

целое proc_sched_autogroup_set_nice(struct task_struct *p, целое nice)
{
	static unsigned long next = INITIAL_JIFFIES;
	struct autogroup *ag;
	unsigned long shares;
	целое err, idx;

	если (nice < MIN_NICE || nice > MAX_NICE)
		вернуть -EINVAL;

	err = security_task_setnice(current, nice);
	если (err)
		вернуть err;

	если (nice < 0 && !can_nice(current, nice))
		вернуть -EPERM;

	/* This is a heavy operation, taking global locks.. */
	если (!capable(CAP_SYS_ADMIN) && time_before(jiffies, next))
		вернуть -EAGAIN;

	next = HZ / 10 + jiffies;
	ag = autogroup_task_get(p);

	idx = array_index_nospec(nice + 20, 40);
	shares = scale_load(sched_prio_to_weight[idx]);

	down_write(&ag->lock);
	err = sched_group_set_shares(ag->tg, shares);
	если (!err)
		ag->nice = nice;
	up_write(&ag->lock);

	autogroup_kref_put(ag);

	вернуть err;
}

void proc_sched_autogroup_show_task(struct task_struct *p, struct seq_file *m)
{
	struct autogroup *ag = autogroup_task_get(p);

	если (!task_group_is_autogroup(ag->tg))
		goto out;

	down_read(&ag->lock);
	seq_printf(m, "/autogroup-%ld nice %d\n", ag->id, ag->nice);
	up_read(&ag->lock);

out:
	autogroup_kref_put(ag);
}
#endif /* CONFIG_PROC_FS */

целое autogroup_path(struct task_group *tg, char *buf, целое buflen)
{
	если (!task_group_is_autogroup(tg))
		вернуть 0;

	вернуть snprintf(buf, buflen, "%s-%ld", "/autogroup", tg->autogroup->id);
}
