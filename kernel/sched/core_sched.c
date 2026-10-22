// SPDX-License-Identifier: GPL-2.0-only

/*
 * A simple wrapper around refcount. An allocated sched_core_cookie's
 * address is used to compute the cookie of the task.
 */
#شامل "sched.h"

ساخت sched_core_cookie {
	refcount_t refcnt;
};

ساکن غیرعلامتی طویل sched_core_alloc_cookie(خالی)
{
	ساخت sched_core_cookie *ck = kmalloc_obj(*ck);
	اگر (!ck)
		واپس 0;

	refcount_سیٹ(&ck->refcnt, 1);
	sched_core_get();

	واپس (غیرعلامتی طویل)ck;
}

ساکن خالی sched_core_put_cookie(غیرعلامتی طویل cookie)
{
	ساخت sched_core_cookie *ptr = (خالی *)cookie;

	اگر (ptr && refcount_dec_and_test(&ptr->refcnt)) {
		kfree(ptr);
		sched_core_put();
	}
}

ساکن غیرعلامتی طویل sched_core_get_cookie(غیرعلامتی طویل cookie)
{
	ساخت sched_core_cookie *ptr = (خالی *)cookie;

	اگر (ptr)
		refcount_inc(&ptr->refcnt);

	واپس cookie;
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
ساکن غیرعلامتی طویل sched_core_update_cookie(ساخت task_struct *p,
					      غیرعلامتی طویل cookie)
{
	غیرعلامتی طویل old_cookie;
	ساخت rq_flags rf;
	ساخت rq *rq;

	rq = task_rq_تالا_2(p, &rf);

	/*
	 * Since creating a cookie implies sched_core_get(), and we cannot set
	 * a cookie until after we've created it, similarly, we cannot destroy
	 * a cookie until after we've removed it, we must have core scheduling
	 * enabled here.
	 */
	WARN_ON_ONCE((p->core_cookie || cookie) && !sched_core_enabled(rq));

	اگر (sched_core_enqueued(p))
		sched_core_قطار_سے_نکالو(rq, p, قطار_سے_نکالو_SAVE);

	old_cookie = p->core_cookie;
	p->core_cookie = cookie;

	/*
	 * Consider the cases: !prev_cookie and !cookie.
	 */
	اگر (cookie && task_on_rq_queued(p))
		sched_core_قطار_میں_ڈالو(rq, p);

	/*
	 * If task is currently running, it may not be compatible anymore after
	 * the cookie change, so enter the scheduler on its CPU to schedule it
	 * away.
	 *
	 * Note that it is possible that as a result of this cookie change, the
	 * core has now entered/left forced idle state. Defer accounting to the
	 * next scheduling edge, rather than always forcing a reschedule here.
	 */
	اگر (task_on_cpu(rq, p))
		resched_curr(rq);

	task_rq_unlock(rq, p, &rf);

	واپس old_cookie;
}

ساکن غیرعلامتی طویل sched_core_clone_cookie(ساخت task_struct *p)
{
	غیرعلامتی طویل cookie, flags;

	raw_spin_تالا_irqsave(&p->pi_تالا, flags);
	cookie = sched_core_get_cookie(p->core_cookie);
	raw_spin_unlock_irqrestore(&p->pi_تالا, flags);

	واپس cookie;
}

خالی sched_core_fork(ساخت task_struct *p)
{
	RB_CLEAR_گرہ(&p->core_گرہ);
	p->core_cookie = sched_core_clone_cookie(current);
}

خالی sched_core_free(ساخت task_struct *p)
{
	sched_core_put_cookie(p->core_cookie);
}

ساکن خالی __sched_core_set(ساخت task_struct *p, غیرعلامتی طویل cookie)
{
	cookie = sched_core_get_cookie(cookie);
	cookie = sched_core_update_cookie(p, cookie);
	sched_core_put_cookie(cookie);
}

/* Called from prctl interface: PR_SCHED_CORE */
صحیح sched_core_share_pid(غیرعلامتی صحیح cmd, pid_t pid, شمار pid_type type,
			 غیرعلامتی طویل uaddr)
{
	غیرعلامتی طویل cookie = 0, id = 0;
	ساخت task_struct *task, *p;
	ساخت pid *grp;
	صحیح err = 0;

	اگر (!sched_smt_فعال())
		واپس -ENODEV;

	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD != PIDTYPE_PID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_THREAD_گروہ != PIDTYPE_TGID);
	BUILD_BUG_ON(PR_SCHED_CORE_SCOPE_PROCESS_گروہ != PIDTYPE_PGID);

	اگر (type > PIDTYPE_PGID || cmd >= PR_SCHED_CORE_MAX || pid < 0 ||
	    (cmd != PR_SCHED_CORE_GET && uaddr))
		واپس -EINVAL;

	rcu_read_تالا();
	اگر (pid == 0) {
		task = current;
	} ورنہ {
		task = ڈھونڈو_task_by_vpid(pid);
		اگر (!task) {
			rcu_read_unlock();
			واپس -ESRCH;
		}
	}
	get_task_struct(task);
	rcu_read_unlock();

	/*
	 * Check if this process has the right to modify the specified
	 * process. Use the regular "ptrace_may_access()" checks.
	 */
	اگر (!ptrace_may_access(task, PTRACE_منوال_READ_REALCREDS)) {
		err = -EPERM;
		جاؤ برآمد;
	}

	انتخاب (cmd) {
	حالت PR_SCHED_CORE_GET:
		اگر (type != PIDTYPE_PID || uaddr & 7) {
			err = -EINVAL;
			جاؤ برآمد;
		}
		cookie = sched_core_clone_cookie(task);
		اگر (cookie) {
			/* XXX improve ? */
			ptr_مقصد_hashval((خالی *)cookie, &id);
		}
		err = put_user(id, (u64 __user *)uaddr);
		جاؤ برآمد;

	حالت PR_SCHED_CORE_CREATE:
		cookie = sched_core_alloc_cookie();
		اگر (!cookie) {
			err = -ENOMEM;
			جاؤ برآمد;
		}
		توڑو;

	حالت PR_SCHED_CORE_SHARE_مقصد:
		cookie = sched_core_clone_cookie(current);
		توڑو;

	حالت PR_SCHED_CORE_SHARE_FROM:
		اگر (type != PIDTYPE_PID) {
			err = -EINVAL;
			جاؤ برآمد;
		}
		cookie = sched_core_clone_cookie(task);
		__sched_core_set(current, cookie);
		جاؤ برآمد;

	طےشدہ:
		err = -EINVAL;
		جاؤ برآمد;
	}

	اگر (type == PIDTYPE_PID) {
		__sched_core_set(task, cookie);
		جاؤ برآمد;
	}

	read_تالا(&tasklist_تالا);
	grp = task_pid_type(task, type);

	do_each_pid_thread(grp, type, p) {
		اگر (!ptrace_may_access(p, PTRACE_منوال_READ_REALCREDS)) {
			err = -EPERM;
			جاؤ برآمد_tasklist;
		}
	} while_each_pid_thread(grp, type, p);

	do_each_pid_thread(grp, type, p) {
		__sched_core_set(p, cookie);
	} while_each_pid_thread(grp, type, p);
برآمد_tasklist:
	read_unlock(&tasklist_تالا);

برآمد:
	sched_core_put_cookie(cookie);
	put_task_struct(task);
	واپس err;
}

#اگر_تعریف_شدہ CONFIG_SCHEDSTATS

/* REQUIRES: rq->core's clock recently updated. */
خالی __sched_core_account_forceidle(ساخت rq *rq)
{
	ثابت ساخت cpumask *smt_نقاب = cpu_smt_نقاب(cpu_کا(rq));
	u64 delta, now = rq_clock(rq->core);
	ساخت rq *rq_i;
	ساخت task_struct *p;
	صحیح i;

	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(!rq->core->core_forceidle_گنتی);

	اگر (rq->core->core_forceidle_آغاز == 0)
		واپس;

	delta = now - rq->core->core_forceidle_آغاز;
	اگر (unlikely((s64)delta <= 0))
		واپس;

	rq->core->core_forceidle_آغاز = now;

	اگر (WARN_ON_ONCE(!rq->core->core_forceidle_occupation)) {
		/* can't be forced idle without a running task */
	} ورنہ اگر (rq->core->core_forceidle_گنتی > 1 ||
		   rq->core->core_forceidle_occupation > 1) {
		/*
		 * For larger SMT configurations, we need to scale the charged
		 * forced idle amount since there can be more than one forced
		 * idle sibling and more than one running cookied task.
		 */
		delta *= rq->core->core_forceidle_گنتی;
		delta = div_u64(delta, rq->core->core_forceidle_occupation);
	}

	for_each_cpu(i, smt_نقاب) {
		rq_i = cpu_rq(i);
		p = rq_i->core_pick ?: rq_i->curr;

		اگر (p == rq_i->idle)
			جاری;

		/*
		 * Note: this will account forceidle to the current CPU, even
		 * if it comes from our SMT sibling.
		 */
		__account_forceidle_time(p, delta);
	}
}

خالی __sched_core_tick(ساخت rq *rq)
{
	اگر (!rq->core->core_forceidle_گنتی)
		واپس;

	اگر (rq != rq->core)
		update_rq_clock(rq->core);

	__sched_core_account_forceidle(rq);
}

#اگر_ختم /* CONFIG_SCHEDSTATS */
