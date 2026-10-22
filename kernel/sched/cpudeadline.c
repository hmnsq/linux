// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ parent(ಪೂರ್ಣಾಙ್ಕ i)
{
	ಪ್ರತ್ಯಾಗಮ (i - 1) >> 1;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ ಎಡ_child(ಪೂರ್ಣಾಙ್ಕ i)
{
	ಪ್ರತ್ಯಾಗಮ (i << 1) + 1;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ ಬಲ_child(ಪೂರ್ಣಾಙ್ಕ i)
{
	ಪ್ರತ್ಯಾಗಮ (i << 1) + 2;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpudl_heapify_down(ಸಂರಚನಾ cpudl *cp, ಪೂರ್ಣಾಙ್ಕ idx)
{
	ಪೂರ್ಣಾಙ್ಕ l, r, largest;

	ಪೂರ್ಣಾಙ್ಕ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ಯದಿ (ಎಡ_child(idx) >= cp->size)
		ಪ್ರತ್ಯಾಗಮ;

	/* adapted from lib/prio_heap.c */
	ಯಾವತ್ (1) {
		u64 largest_dl;

		l = ಎಡ_child(idx);
		r = ಬಲ_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		ಯದಿ ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		ಯದಿ ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		ಯದಿ (largest == idx)
			ಭಙ್ಗ;

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

ಸ್ಥಿರ ಶೂನ್ಯ cpudl_heapify_up(ಸಂರಚನಾ cpudl *cp, ಪೂರ್ಣಾಙ್ಕ idx)
{
	ಪೂರ್ಣಾಙ್ಕ p;

	ಪೂರ್ಣಾಙ್ಕ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ಯದಿ (idx == 0)
		ಪ್ರತ್ಯಾಗಮ;

	ಕುರು {
		p = parent(idx);
		ಯದಿ (dl_time_before(orig_dl, cp->elements[p].dl))
			ಭಙ್ಗ;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} ಯಾವತ್ (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpudl_heapify(ಸಂರಚನಾ cpudl *cp, ಪೂರ್ಣಾಙ್ಕ idx)
{
	ಯದಿ (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	ಅನ್ಯಥಾ
		cpudl_heapify_down(cp, idx);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ cpudl_maximum(ಸಂರಚನಾ cpudl *cp)
{
	ಪ್ರತ್ಯಾಗಮ cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
ಪೂರ್ಣಾಙ್ಕ cpudl_find(ಸಂರಚನಾ cpudl *cp, ಸಂರಚನಾ task_struct *p,
	       ಸಂರಚನಾ cpumask *later_mask)
{
	ಧ್ರುವ ಸಂರಚನಾ sched_dl_entity *dl_se = &p->dl;

	ಯದಿ (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		ಪೂರ್ಣಾಙ್ಕ cpu, max_cpu = -1;

		ಯದಿ (!sched_asym_cpucap_active())
			ಪ್ರತ್ಯಾಗಮ 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			ಯದಿ (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				ಯದಿ (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		ಯದಿ (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		ಪ್ರತ್ಯಾಗಮ 1;
	} ಅನ್ಯಥಾ {
		ಪೂರ್ಣಾಙ್ಕ best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		ಯದಿ (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			ಯದಿ (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
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
ಶೂನ್ಯ cpudl_clear(ಸಂರಚನಾ cpudl *cp, ಪೂರ್ಣಾಙ್ಕ cpu, bool online)
{
	ಪೂರ್ಣಾಙ್ಕ old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	ಯದಿ (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} ಅನ್ಯಥಾ {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	ಯದಿ (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	ಅನ್ಯಥಾ
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
ಶೂನ್ಯ cpudl_set(ಸಂರಚನಾ cpudl *cp, ಪೂರ್ಣಾಙ್ಕ cpu, u64 dl)
{
	ಪೂರ್ಣಾಙ್ಕ old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	ಯದಿ (old_idx == IDX_INVALID) {
		ಪೂರ್ಣಾಙ್ಕ new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} ಅನ್ಯಥಾ {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
ಪೂರ್ಣಾಙ್ಕ cpudl_init(ಸಂರಚನಾ cpudl *cp)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(ಸಂರಚನಾ cpudl_item, nr_cpu_ids);
	ಯದಿ (!cp->elements)
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	ಯದಿ (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
ಶೂನ್ಯ cpudl_cleanup(ಸಂರಚನಾ cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
