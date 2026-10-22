// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#অন্তর্ভাব "sched.h"

স্থির অন্তঃরেখ পূর্ণসংখ্যা parent(পূর্ণসংখ্যা i)
{
	ফেরত (i - 1) >> 1;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা বাম_child(পূর্ণসংখ্যা i)
{
	ফেরত (i << 1) + 1;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা ডান_child(পূর্ণসংখ্যা i)
{
	ফেরত (i << 1) + 2;
}

স্থির শূন্য cpudl_heapify_down(কাঠামো cpudl *cp, পূর্ণসংখ্যা idx)
{
	পূর্ণসংখ্যা l, r, largest;

	পূর্ণসংখ্যা orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	যদি (বাম_child(idx) >= cp->আকার_2)
		ফেরত;

	/* adapted from lib/prio_heap.c */
	যখন (1) {
		u64 largest_dl;

		l = বাম_child(idx);
		r = ডান_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		যদি ((l < cp->আকার_2) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		যদি ((r < cp->আকার_2) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		যদি (largest == idx)
			বিরতি;

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

স্থির শূন্য cpudl_heapify_up(কাঠামো cpudl *cp, পূর্ণসংখ্যা idx)
{
	পূর্ণসংখ্যা p;

	পূর্ণসংখ্যা orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	যদি (idx == 0)
		ফেরত;

	করো {
		p = parent(idx);
		যদি (dl_time_before(orig_dl, cp->elements[p].dl))
			বিরতি;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} যখন (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

স্থির শূন্য cpudl_heapify(কাঠামো cpudl *cp, পূর্ণসংখ্যা idx)
{
	যদি (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	অন্যথা
		cpudl_heapify_down(cp, idx);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা cpudl_maximum(কাঠামো cpudl *cp)
{
	ফেরত cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
পূর্ণসংখ্যা cpudl_find(কাঠামো cpudl *cp, কাঠামো task_struct *p,
	       কাঠামো cpumask *later_mask)
{
	ধ্রুবক কাঠামো sched_dl_entity *dl_se = &p->dl;

	যদি (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		পূর্ণসংখ্যা cpu, max_cpu = -1;

		যদি (!sched_asym_cpucap_active())
			ফেরত 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			যদি (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				যদি (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		যদি (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		ফেরত 1;
	} অন্যথা {
		পূর্ণসংখ্যা best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		যদি (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			যদি (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			ফেরত 1;
		}
	}
	ফেরত 0;
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
শূন্য cpudl_clear(কাঠামো cpudl *cp, পূর্ণসংখ্যা cpu, bool online)
{
	পূর্ণসংখ্যা old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	যদি (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} অন্যথা {
		new_cpu = cp->elements[cp->আকার_2 - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->আকার_2 - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->আকার_2--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	যদি (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	অন্যথা
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
শূন্য cpudl_set(কাঠামো cpudl *cp, পূর্ণসংখ্যা cpu, u64 dl)
{
	পূর্ণসংখ্যা old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	যদি (old_idx == IDX_INVALID) {
		পূর্ণসংখ্যা new_idx = cp->আকার_2++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} অন্যথা {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
পূর্ণসংখ্যা cpudl_init(কাঠামো cpudl *cp)
{
	পূর্ণসংখ্যা i;

	raw_spin_lock_init(&cp->lock);
	cp->আকার_2 = 0;

	cp->elements = kzalloc_objs(কাঠামো cpudl_item, nr_cpu_ids);
	যদি (!cp->elements)
		ফেরত -ENOMEM;

	যদি (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		ফেরত -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	ফেরত 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
শূন্য cpudl_cleanup(কাঠামো cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
