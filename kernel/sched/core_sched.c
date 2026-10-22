// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#ضمن "sched.h"

بنية sched_core_cookie {
	refcount_t refcnt;
};

ساكن بنانشان ڊگهو sched_core_alloc_cookie(فراغ)
{
	بنية sched_core_cookie *ck = kmalloc_obj(*ck);
	إذا (!ck)
		أرجع 0;

	refcount_سيٽ(&ck->refcnt, 1);
	sched_core_get();

	أرجع (بنانشان ڊگهو)ck;
}

ساكن فراغ sched_core_put_cookie(بنانشان ڊگهو cookie)
{
	بنية sched_core_cookie *ptr = (فراغ *)cookie;

	إذا (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

ساكن بنانشان ڊگهو sched_core_get_cookie(بنانشان ڊگهو cookie)
{
	بنية sched_core_cookie *ptr = (فراغ *)cookie;

	إذا (ptr)
		refcount_inc(&ptr->refcnt);

	أرجع cookie;
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
ساكن بنانشان ڊگهو sched_core_update_cookie(بنية task_struct *p,
					      بنانشان ڊگهو cookie)
{
	بنانشان ڊگهو old_cookie;
	بنية rq_flags rf;
	بنية rq *rq;

	rq = task_rq_تالو_2(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	إذا (sched_core_enqueued(p))
		sched_core_قطار_مان_ڪڍو(rq, p, قطار_مان_ڪڍو_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	إذا (cookie && task_on_rq_queued(p))
		sched_core_قطار_اندر_وجهو(rq, p);

	/*
	 * If task is currently running, it may not be compatible anymore after
	 * the cookie change, so enter the scheduler on its CPU to schedule it
	 * away.
	 *
	 * Note that it is possible that as a result of this cookie change, the
	 * core has now entered/left forced idle state. Defer accounting to the
	 * next scheduling edge, rather than always forcing a reschedule here.
	 */
	إذا (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	أرجع old_cookie;
}

ساكن بنانشان ڊگهو sched_core_clone_cookie(بنية task_struct *p)
{
	بنانشان ڊگهو cookie, flags;

	raw_spin_تالو_irqsave(&p->pi_تالو, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_تالو, flags);

	أرجع cookie;
}

فراغ sched_core_fork(بنية task_struct *p)
{
	RB_CLEAR_ڳنڍ(&p->core_ڳنڍ);
	p->core_cookie = sched_core_clone_cookie(current);
}

فراغ sched_core_free(بنية task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

ساكن فراغ __sched_core_set(بنية task_struct *p, بنانشان ڊگهو cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
صحيح sched_core_share_pid(بنانشان صحيح cmd, pid_t pid, تعداد pid_type type,
			 بنانشان ڊگهو uaddr)
{
	بنانشان ڊگهو cookie = 0, id = 0;
	بنية task_struct *task, *p;
	بنية pid *grp;
	صحيح err = 0;

	إذا (!sched_smt_سرگرم())
		أرجع -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_گروهه != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_گروهه != PIDTYPE_PGID);

	إذا (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		أرجع -EINVAL;

	rcu_read_تالو();
	إذا (pid == 0) {
		task = current;
	} وإلا {
		task = ڳوليو_task_by_vpid(pid);
		إذا (!task) {
			rcu_read_unlock();
			أرجع -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	إذا (!ptrace_may_access(task, PTRACE_گهڻائي_READ_REALCREDS)) {
		err = -EPERM;
		اذهب ٻاهريون;
	}

	بدل (cmd) {
	حالة PR_SCHED_CORE_GET:
		إذا (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			اذهب ٻاهريون;
		}
		cookie = sched_core_clone_cookie(task);
		إذا (cookie) {
			/* XXX improve ? */
			ptr_منزل_hashval((فراغ *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		اذهب ٻاهريون;

	حالة PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		إذا (!cookie) {
			err = -ENOMEM;
			اذهب ٻاهريون;
		}
		اكسر;

	حالة PR_SCHED_CORE_SHARE_منزل:
		cookie = sched_core_clone_cookie(current);
		اكسر;

	حالة PR_SCHED_CORE_SHARE_FROM:
		إذا (type != PIDTYPE_PID) {
			err = -EINVAL;
			اذهب ٻاهريون;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		اذهب ٻاهريون;

	افتراضي:
		err = -EINVAL;
		اذهب ٻاهريون;
	}

	إذا (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		اذهب ٻاهريون;
	}

	read_تالو(&tasklist_تالو);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		إذا (!ptrace_may_access(p, PTRACE_گهڻائي_READ_REALCREDS)) {
			err = -EPERM;
			اذهب ٻاهريون_tasklist;
		}
	} while_each_pid_thread(grp, type, p);

	do_each_pid_thread(grp, type, p) {
		__sched_core_set(p, cookie);
	} while_each_pid_thread(grp, type, p);
ٻاهريون_tasklist:
	read_unlock(&tasklist_تالو);

ٻاهريون:
	sched_core_put_cookie(cookie);
	put_task_struct(task);
	أرجع err;
}

#جيڪڏهن_وصف_ٿيل CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
فراغ __sched_core_account_forceidle(بنية rq *rq)
{
	ثابت بنية cpumask *smt_نقاب = cpu_smt_نقاب(cpu_جو(rq));
	u64 delta, now = rq_clock(rq->core);
	بنية rq *rq_i;
	بنية task_struct *p;
	صحيح i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_ڳڻپ);

	إذا (rq->core->core_forceidle_شروعات == 0)
		أرجع;

	delta = now - rq->core->core_forceidle_شروعات;
	إذا (unlikely((s64)delta <= 0))
		أرجع;

	rq->core->core_forceidle_شروعات = now;

	إذا (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} وإلا إذا (rq->core->core_forceidle_ڳڻپ > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_ڳڻپ;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_نقاب) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		إذا (p == rq_i->idle)
			استمر;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

فراغ __sched_core_tick(بنية rq *rq)
{
	إذا (!rq->core->core_forceidle_ڳڻپ)
		أرجع;

	إذا (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#جيڪڏهن_پورو /* CONFIG_SCHEDSTATS */
