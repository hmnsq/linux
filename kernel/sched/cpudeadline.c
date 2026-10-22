// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#అన్తర్భావ "sched.h"

స్థిర అంతఃవరుస పూర్ణాంకం మాతృ(పూర్ణాంకం i)
{
	తిరిగి (i - 1) >> 1;
}

స్థిర అంతఃవరుస పూర్ణాంకం ఎడమ_బిడ్డ(పూర్ణాంకం i)
{
	తిరిగి (i << 1) + 1;
}

స్థిర అంతఃవరుస పూర్ణాంకం కుడి_బిడ్డ(పూర్ణాంకం i)
{
	తిరిగి (i << 1) + 2;
}

స్థిర శూన్య cpudl_heapify_కిందికి(నిర్మాణం cpudl *cp, పూర్ణాంకం idx)
{
	పూర్ణాంకం l, r, అతిపెద్ద;

	పూర్ణాంకం orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ఒకవేళ (ఎడమ_బిడ్డ(idx) >= cp->కొలత)
		తిరిగి;

	/* adapted from lib/prio_heap.c */
	అయితే (1) {
		u64 అతిపెద్ద_dl;

		l = ఎడమ_బిడ్డ(idx);
		r = కుడి_బిడ్డ(idx);
		అతిపెద్ద = idx;
		అతిపెద్ద_dl = orig_dl;

		ఒకవేళ ((l < cp->కొలత) && dl_సమయం_before(orig_dl,
						cp->elements[l].dl)) {
			అతిపెద్ద = l;
			అతిపెద్ద_dl = cp->elements[l].dl;
		}
		ఒకవేళ ((r < cp->కొలత) && dl_సమయం_before(అతిపెద్ద_dl,
						cp->elements[r].dl))
			అతిపెద్ద = r;

		ఒకవేళ (అతిపెద్ద == idx)
			ఆపు;

		/* pull largest child onto idx */
		cp->elements[idx].cpu = cp->elements[అతిపెద్ద].cpu;
		cp->elements[idx].dl = cp->elements[అతిపెద్ద].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = అతిపెద్ద;
	}
	/* actual push down of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

స్థిర శూన్య cpudl_heapify_పైకి(నిర్మాణం cpudl *cp, పూర్ణాంకం idx)
{
	పూర్ణాంకం p;

	పూర్ణాంకం orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ఒకవేళ (idx == 0)
		తిరిగి;

	చేయి {
		p = మాతృ(idx);
		ఒకవేళ (dl_సమయం_before(orig_dl, cp->elements[p].dl))
			ఆపు;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} అయితే (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

స్థిర శూన్య cpudl_heapify(నిర్మాణం cpudl *cp, పూర్ణాంకం idx)
{
	ఒకవేళ (idx > 0 && dl_సమయం_before(cp->elements[మాతృ(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_పైకి(cp, idx);
	లేకపోతే
		cpudl_heapify_కిందికి(cp, idx);
}

స్థిర అంతఃవరుస పూర్ణాంకం cpudl_గరిష్ఠ(నిర్మాణం cpudl *cp)
{
	తిరిగి cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
పూర్ణాంకం cpudl_కనుగొను(నిర్మాణం cpudl *cp, నిర్మాణం task_struct *p,
	       నిర్మాణం cpumask *later_ముసుగు)
{
	స్థిరరాశి నిర్మాణం sched_dl_entity *dl_se = &p->dl;

	ఒకవేళ (later_ముసుగు &&
	    cpumask_and(later_ముసుగు, cp->free_cpus, &p->cpus_ముసుగు)) {
		గుర్తులేని దీర్ఘ cap, max_cap = 0;
		పూర్ణాంకం cpu, max_cpu = -1;

		ఒకవేళ (!sched_asym_cpucap_చురుకైన())
			తిరిగి 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_ముసుగు) {
			ఒకవేళ (!dl_task_సరిపోతుంది_సామర్థ్యం(p, cpu)) {
				cpumask_clear_cpu(cpu, later_ముసుగు);

				cap = arch_scale_cpu_సామర్థ్యం(cpu);

				ఒకవేళ (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		ఒకవేళ (cpumask_ఖాళీలు(later_ముసుగు))
			cpumask_సమితి_cpu(max_cpu, later_ముసుగు);

		తిరిగి 1;
	} లేకపోతే {
		పూర్ణాంకం ఉత్తమ_cpu = cpudl_గరిష్ఠ(cp);

		WARN_ON(ఉత్తమ_cpu != -1 && !cpu_present(ఉత్తమ_cpu));

		ఒకవేళ (cpumask_test_cpu(ఉత్తమ_cpu, &p->cpus_ముసుగు) &&
		    dl_సమయం_before(dl_se->deadline, cp->elements[0].dl)) {
			ఒకవేళ (later_ముసుగు)
				cpumask_సమితి_cpu(ఉత్తమ_cpu, later_ముసుగు);

			తిరిగి 1;
		}
	}
	తిరిగి 0;
}

/*
 * cpudl_clear - remove a CPU from the cpudl max-heap
 * @cp: the cpudl max-heap context
 * @cpu: the target CPU
 * @online: the online state of the deadline runqueue
 *
 * Notes: assumes cpu_rq(cpu)->lock is locked
 *
 * Returns: (void)
 */
శూన్య cpudl_clear(నిర్మాణం cpudl *cp, పూర్ణాంకం cpu, bool online)
{
	పూర్ణాంకం old_idx, new_cpu;
	గుర్తులేని దీర్ఘ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_తాళం_irqsave(&cp->తాళం, flags);

	old_idx = cp->elements[cpu].idx;
	ఒకవేళ (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} లేకపోతే {
		new_cpu = cp->elements[cp->కొలత - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->కొలత - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->కొలత--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	ఒకవేళ (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	లేకపోతే
		__cpumask_clear_cpu(cpu, cp->free_cpus);

	raw_spin_unlock_irqrestore(&cp->తాళం, flags);
}

/*
 * cpudl_set - update the cpudl max-heap
 * @cp: the cpudl max-heap context
 * @cpu: the target CPU
 * @dl: the new earliest deadline for this CPU
 *
 * Notes: assumes cpu_rq(cpu)->lock is locked
 *
 * Returns: (void)
 */
శూన్య cpudl_సమితి(నిర్మాణం cpudl *cp, పూర్ణాంకం cpu, u64 dl)
{
	పూర్ణాంకం old_idx;
	గుర్తులేని దీర్ఘ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_తాళం_irqsave(&cp->తాళం, flags);

	old_idx = cp->elements[cpu].idx;
	ఒకవేళ (old_idx == IDX_INVALID) {
		పూర్ణాంకం new_idx = cp->కొలత++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_పైకి(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} లేకపోతే {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->తాళం, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
పూర్ణాంకం cpudl_init(నిర్మాణం cpudl *cp)
{
	పూర్ణాంకం i;

	raw_spin_తాళం_init(&cp->తాళం);
	cp->కొలత = 0;

	cp->elements = kzalloc_objs(నిర్మాణం cpudl_అంశం, nr_cpu_ids);
	ఒకవేళ (!cp->elements)
		తిరిగి -ENOMEM;

	ఒకవేళ (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		తిరిగి -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	తిరిగి 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
శూన్య cpudl_cleanup(నిర్మాణం cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
