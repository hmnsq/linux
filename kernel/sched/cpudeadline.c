// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#ضمن "sched.h"

ساكن مضمن صحيح parent(صحيح i)
{
	ܦܢܐ (i - 1) >> 1;
}

ساكن مضمن صحيح left_child(صحيح i)
{
	ܦܢܐ (i << 1) + 1;
}

ساكن مضمن صحيح right_child(صحيح i)
{
	ܦܢܐ (i << 1) + 2;
}

ساكن فراغ cpudl_heapify_down(بنية cpudl *cp, صحيح idx)
{
	صحيح l, r, largest;

	صحيح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ܐܢ (left_child(idx) >= cp->size)
		ܦܢܐ;

	/* adapted from lib/prio_heap.c */
	ܟܡܐ (1) {
		u64 largest_dl;

		l = left_child(idx);
		r = right_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		ܐܢ ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		ܐܢ ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		ܐܢ (largest == idx)
			اكسر;

		/* pull largest child onto idx */
		cp->elements[idx].cpu = cp->elements[largest].cpu;
		cp->elements[idx].dl = cp->elements[largest].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = largest;
	}
	/* actual push down of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ساكن فراغ cpudl_heapify_up(بنية cpudl *cp, صحيح idx)
{
	صحيح p;

	صحيح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ܐܢ (idx == 0)
		ܦܢܐ;

	ܥܒܕ {
		p = parent(idx);
		ܐܢ (dl_time_before(orig_dl, cp->elements[p].dl))
			اكسر;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} ܟܡܐ (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ساكن فراغ cpudl_heapify(بنية cpudl *cp, صحيح idx)
{
	ܐܢ (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	ܐܚܪܢܐ
		cpudl_heapify_down(cp, idx);
}

ساكن مضمن صحيح cpudl_maximum(بنية cpudl *cp)
{
	ܦܢܐ cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
صحيح cpudl_find(بنية cpudl *cp, بنية task_struct *p,
	       بنية cpumask *later_mask)
{
	ثابت بنية sched_dl_entity *dl_se = &p->dl;

	ܐܢ (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		صحيح cpu, max_cpu = -1;

		ܐܢ (!sched_asym_cpucap_active())
			ܦܢܐ 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			ܐܢ (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				ܐܢ (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		ܐܢ (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		ܦܢܐ 1;
	} ܐܚܪܢܐ {
		صحيح best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		ܐܢ (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			ܐܢ (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			ܦܢܐ 1;
		}
	}
	ܦܢܐ 0;
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
فراغ cpudl_clear(بنية cpudl *cp, صحيح cpu, bool online)
{
	صحيح old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	ܐܢ (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} ܐܚܪܢܐ {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	ܐܢ (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	ܐܚܪܢܐ
		__cpumask_clear_cpu(cpu, cp->free_cpus);

	raw_spin_unlock_irqrestore(&cp->lock, flags);
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
فراغ cpudl_set(بنية cpudl *cp, صحيح cpu, u64 dl)
{
	صحيح old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	ܐܢ (old_idx == IDX_INVALID) {
		صحيح new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} ܐܚܪܢܐ {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
صحيح cpudl_init(بنية cpudl *cp)
{
	صحيح i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(بنية cpudl_item, nr_cpu_ids);
	ܐܢ (!cp->elements)
		ܦܢܐ -ENOMEM;

	ܐܢ (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		ܦܢܐ -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	ܦܢܐ 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
فراغ cpudl_cleanup(بنية cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
