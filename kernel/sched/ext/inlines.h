/* SPDX-License-Identifier: GPL-2.0 */
/*
 * BPF extensible scheduler class: Documentation/scheduler/sched-ext.rst
 *
 * Inline definitions layered on top of internal.h and cid.h.
 *
 * Copyright (c) 2026 Meta Platforms, Inc. and affiliates.
 * Copyright (c) 2026 Tejun Heo <tj@kernel.org>
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_EXT_INLINES_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_EXT_INLINES_H

#ಅನ್ತರ್ಭಾವ "internal.h"
#ಅನ್ತರ್ಭಾವ "cid.h"

/* what dispatch concluded, consumed by the pick that follows */
ಎಣಿಕೆ scx_dsp_verdict {
	SCX_DSP_NONE,		/* nothing to run */
	SCX_DSP_LOCAL,		/* local DSQ has tasks */
	SCX_DSP_PREV,		/* keep running @prev */
	SCX_DSP_RETRY,		/* pick helpers only: restart the pick */
};

/*
 * One user of this function is scx_bpf_sub_dispatch() which can be called
 * recursively as sub-sched dispatches nest. Always inline to reduce stack usage
 * from the call frame.
 */
ಸ್ಥಿರ __always_inline ಎಣಿಕೆ scx_dsp_verdict
scx_dispatch_sched(ರಚನೆ scx_sched *sch, ರಚನೆ rq *rq,
		   ರಚನೆ task_struct *prev, bool nested)
{
	ರಚನೆ scx_dsp_ctx *dspc = &this_cpu_ptr(sch->pcpu)->dsp_ctx;
	ಪೂರ್ಣಾಂಕ nr_loops = SCX_DSP_MAX_LOOPS;
	s32 cpu = cpu_ದ(rq);
	bool prev_on_sch = (prev->sched_class == &ext_sched_class) &&
		scx_task_on_sched(sch, prev);

	ಆದರೆ (scx_consume_global_dsq(sch, rq))
		ಹಿಂತಿರುಗಿ SCX_DSP_LOCAL;

	ಆದರೆ (scx_bypass_dsp_enabled(sch)) {
		/* if @sch is bypassing, only the bypass DSQs are active */
		ಆದರೆ (scx_bypassing(sch, cpu)) {
			ಆದರೆ (scx_consume_dispatch_q(sch, rq, scx_bypass_dsq(sch, cpu), 0))
				ಹಿಂತಿರುಗಿ SCX_DSP_LOCAL;
			ಹಿಂತಿರುಗಿ SCX_DSP_NONE;
		}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		/*
		 * If @sch isn't bypassing but its children are, @sch is
		 * responsible for making forward progress for both its own
		 * tasks that aren't bypassing and the bypassing descendants'
		 * tasks. The following implements a simple built-in behavior -
		 * let each CPU try to run the bypass DSQ every Nth time.
		 *
		 * Later, if necessary, we can add an ops flag to suppress the
		 * auto-consumption and a kfunc to consume the bypass DSQ and,
		 * so that the BPF scheduler can fully control scheduling of
		 * bypassed tasks.
		 */
		ರಚನೆ scx_sched_pcpu *pcpu = ಪ್ರತಿ_cpu_ptr(sch->pcpu, cpu);

		ಆದರೆ (!(pcpu->bypass_host_seq++ % SCX_BYPASS_HOST_NTH) &&
		    scx_consume_dispatch_q(sch, rq, scx_bypass_dsq(sch, cpu), 0)) {
			__scx_add_event(sch, SCX_EV_SUB_BYPASS_DISPATCH, 1);
			ಹಿಂತಿರುಗಿ SCX_DSP_LOCAL;
		}
#ಆದರೆ_ಅಂತ್ಯ	/* CONFIG_EXT_SUB_SCHED */
	}

	/*
	 * scx_rq_online() can't be used. Its cpu_active() test goes false
	 * before CPU hotplug waits for an RCU grace period, and
	 * rq_offline_scx() moves this CPU's tasks to the local DSQ only after
	 * the wait. The grace period can depend on those tasks running.
	 */
	ಆದರೆ (unlikely(!SCX_HAS_OP(sch, dispatch)) || !(rq->scx.flags & SCX_RQ_ONLINE))
		ಹಿಂತಿರುಗಿ SCX_DSP_NONE;

	dspc->rq = rq;

	/*
	 * The dispatch loop. Because scx_flush_dispatch_buf() may drop the rq
	 * lock, the local DSQ might still end up empty after a successful
	 * ops.dispatch(). If the local DSQ is empty even after ops.dispatch()
	 * produced some tasks, retry. The BPF scheduler may depend on this
	 * looping behavior to simplify its implementation.
	 */
	ಮಾಡು {
		dspc->nr_tasks = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		/* stash @prev so that nested invocations can access it */
		ಆದರೆ (!nested)
			rq->scx.sub_dispatch_prev = prev;
#ಆದರೆ_ಅಂತ್ಯ

		SCX_CALL_OP(sch, dispatch, rq, scx_cpu_arg(cpu),
			    prev_on_sch ? prev : ಖಾಲಿ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
		ಆದರೆ (!nested)
			rq->scx.sub_dispatch_prev = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ

		scx_flush_dispatch_buf(sch, rq);

		ಆದರೆ ((prev->scx.flags & SCX_TASK_QUEUED) && prev->scx.ತುಂಡು)
			ಹಿಂತಿರುಗಿ SCX_DSP_PREV;
		ಆದರೆ (rq->scx.local_dsq.nr)
			ಹಿಂತಿರುಗಿ SCX_DSP_LOCAL;
		ಆದರೆ (scx_consume_global_dsq(sch, rq))
			ಹಿಂತಿರುಗಿ SCX_DSP_LOCAL;

		/*
		 * ops.dispatch() can trap us in this loop by repeatedly
		 * dispatching ineligible tasks. Break out once in a while to
		 * allow the watchdog to run. As IRQ can't be enabled in
		 * dispatch, we want to complete this scheduling cycle and then
		 * start a new one. IOW, we want to call resched_curr() on the
		 * next, most likely idle, task, not the current one. Use
		 * __scx_bpf_kick_cpu() for deferred kicking.
		 */
		ಆದರೆ (unlikely(!--nr_loops)) {
			scx_kick_cpu(sch, cpu, 0);
			ನಿಲ್ಲಿಸು;
		}
	} ಆಗ (dspc->nr_tasks);

	/*
	 * Prevent the CPU from going idle while bypassed descendants have tasks
	 * queued. Without this fallback, bypassed tasks could stall if the host
	 * scheduler's ops.dispatch() doesn't yield any tasks.
	 */
	ಆದರೆ (scx_bypass_dsp_enabled(sch) &&
	    scx_consume_dispatch_q(sch, rq, scx_bypass_dsq(sch, cpu), 0)) {
		__scx_add_event(sch, SCX_EV_SUB_BYPASS_DISPATCH, 1);
		ಹಿಂತಿರುಗಿ SCX_DSP_LOCAL;
	}

	ಹಿಂತಿರುಗಿ SCX_DSP_NONE;
}

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_EXT_INLINES_H */
