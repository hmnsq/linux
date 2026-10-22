// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#include "sched.h"

sa_nracanaa sched_core_cookie {
	refcount_t refcnt;
};

static unsigned long sched_core_alloc_cookie(void)
{
	sa_nracanaa sched_core_cookie *ck = kmalloc_obj(*ck);
	གལ་ཏེ (!ck)
		ཕྱིར 0;

	refcount_set(&ck->refcnt, 1);
	sched_core_get();

	ཕྱིར (unsigned long)ck;
}

static void sched_core_put_cookie(unsigned long cookie)
{
	sa_nracanaa sched_core_cookie *ptr = (void *)cookie;

	གལ་ཏེ (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

static unsigned long sched_core_get_cookie(unsigned long cookie)
{
	sa_nracanaa sched_core_cookie *ptr = (void *)cookie;

	གལ་ཏེ (ptr)
		refcount_inc(&ptr->refcnt);

	ཕྱིར cookie;
}

/*
 * sched_core_update_cookie - replace the cookie on a task
 * @p: the task to update
 * @cookie: the new cookie
 *
 * Effectively exchange the task cookie; caller is responsible for lifetimes on
 * both ends.
 *
 * Returns: the old cookie
 */
static unsigned long sched_core_update_cookie(sa_nracanaa task_struct *p,
					      unsigned long cookie)
{
	unsigned long old_cookie;
	sa_nracanaa rq_flags rf;
	sa_nracanaa rq *rq;

	rq = task_rq_lock(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	གལ་ཏེ (sched_core_enqueued(p))
		sched_core_dequeue(rq, p, DEQUEUE_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	གལ་ཏེ (cookie && task_on_rq_queued(p))
		sched_core_enqueue(rq, p);

	/*
	 * If task is currently running, it may not be compatible anymore after
	 * the cookie change, so enter the scheduler on its CPU to schedule it
	 * away.
	 *
	 * Note that it is possible that as a result of this cookie change, the
	 * core has now entered/left forced idle state. Defer accounting to the
	 * next scheduling edge, rather than always forcing a reschedule here.
	 */
	གལ་ཏེ (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	ཕྱིར old_cookie;
}

static unsigned long sched_core_clone_cookie(sa_nracanaa task_struct *p)
{
	unsigned long cookie, flags;

	raw_spin_lock_irqsave(&p->pi_lock, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);

	ཕྱིར cookie;
}

void sched_core_fork(sa_nracanaa task_struct *p)
{
	RB_CLEAR_NODE(&p->core_node);
	p->core_cookie = sched_core_clone_cookie(current);
}

void sched_core_free(sa_nracanaa task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

static void __sched_core_set(sa_nracanaa task_struct *p, unsigned long cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
ཧྲིལ sched_core_share_pid(unsigned ཧྲིལ cmd, pid_t pid, ga_nanaa pid_type type,
			 unsigned long uaddr)
{
	unsigned long cookie = 0, id = 0;
	sa_nracanaa task_struct *task, *p;
	sa_nracanaa pid *grp;
	ཧྲིལ err = 0;

	གལ་ཏེ (!sched_smt_active())
		ཕྱིར -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_GROUP != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_GROUP != PIDTYPE_PGID);

	གལ་ཏེ (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		ཕྱིར -EINVAL;

	rcu_read_lock();
	གལ་ཏེ (pid == 0) {
		task = current;
	} དེ་མིན {
		task = find_task_by_vpid(pid);
		གལ་ཏེ (!task) {
			rcu_read_unlock();
			ཕྱིར -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	གལ་ཏེ (!ptrace_may_access(task, PTRACE_MODE_READ_REALCREDS)) {
		err = -EPERM;
		goto out;
	}

	switch (cmd) {
	pak_sa PR_SCHED_CORE_GET:
		གལ་ཏེ (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			goto out;
		}
		cookie = sched_core_clone_cookie(task);
		གལ་ཏེ (cookie) {
			/* XXX improve ? */
			ptr_to_hashval((void *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		goto out;

	pak_sa PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		གལ་ཏེ (!cookie) {
			err = -ENOMEM;
			goto out;
		}
		bha_nga;

	pak_sa PR_SCHED_CORE_SHARE_TO:
		cookie = sched_core_clone_cookie(current);
		bha_nga;

	pak_sa PR_SCHED_CORE_SHARE_FROM:
		གལ་ཏེ (type != PIDTYPE_PID) {
			err = -EINVAL;
			goto out;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		goto out;

	default:
		err = -EINVAL;
		goto out;
	}

	གལ་ཏེ (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		goto out;
	}

	read_lock(&tasklist_lock);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		གལ་ཏེ (!ptrace_may_access(p, PTRACE_MODE_READ_REALCREDS)) {
			err = -EPERM;
			goto out_tasklist;
		}
	} while_each_pid_thread(grp, type, p);

	do_each_pid_thread(grp, type, p) {
		__sched_core_set(p, cookie);
	} while_each_pid_thread(grp, type, p);
out_tasklist:
	read_unlock(&tasklist_lock);

out:
	sched_core_put_cookie(cookie);
	put_task_struct(task);
	ཕྱིར err;
}

#ifdef CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
void __sched_core_account_forceidle(sa_nracanaa rq *rq)
{
	const sa_nracanaa cpumask *smt_mask = cpu_smt_mask(cpu_of(rq));
	u64 delta, now = rq_clock(rq->core);
	sa_nracanaa rq *rq_i;
	sa_nracanaa task_struct *p;
	ཧྲིལ i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_count);

	གལ་ཏེ (rq->core->core_forceidle_start == 0)
		ཕྱིར;

	delta = now - rq->core->core_forceidle_start;
	གལ་ཏེ (unlikely((s64)delta <= 0))
		ཕྱིར;

	rq->core->core_forceidle_start = now;

	གལ་ཏེ (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} དེ་མིན གལ་ཏེ (rq->core->core_forceidle_count > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_count;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_mask) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		གལ་ཏེ (p == rq_i->idle)
			continue;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

void __sched_core_tick(sa_nracanaa rq *rq)
{
	གལ་ཏེ (!rq->core->core_forceidle_count)
		ཕྱིར;

	གལ་ཏེ (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#endif /* CONFIG_SCHEDSTATS */
