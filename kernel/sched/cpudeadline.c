// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#அந்தர்பாவ "sched.h"

நிலையான உள்வரி முழு parent(முழு i)
{
	திரும்பு (i - 1) >> 1;
}

நிலையான உள்வரி முழு இடது_child(முழு i)
{
	திரும்பு (i << 1) + 1;
}

நிலையான உள்வரி முழு வலது_child(முழு i)
{
	திரும்பு (i << 1) + 2;
}

நிலையான வெறுமை cpudl_heapify_down(கட்டமைப்பு cpudl *cp, முழு idx)
{
	முழு l, r, largest;

	முழு orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	எனில் (இடது_child(idx) >= cp->size)
		திரும்பு;

	/* adapted from lib/prio_heap.c */
	போது (1) {
		u64 largest_dl;

		l = இடது_child(idx);
		r = வலது_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		எனில் ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		எனில் ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		எனில் (largest == idx)
			நிறுத்து;

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

நிலையான வெறுமை cpudl_heapify_up(கட்டமைப்பு cpudl *cp, முழு idx)
{
	முழு p;

	முழு orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	எனில் (idx == 0)
		திரும்பு;

	செய் {
		p = parent(idx);
		எனில் (dl_time_before(orig_dl, cp->elements[p].dl))
			நிறுத்து;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} போது (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

நிலையான வெறுமை cpudl_heapify(கட்டமைப்பு cpudl *cp, முழு idx)
{
	எனில் (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	இல்லையெனில்
		cpudl_heapify_down(cp, idx);
}

நிலையான உள்வரி முழு cpudl_maximum(கட்டமைப்பு cpudl *cp)
{
	திரும்பு cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
முழு cpudl_find(கட்டமைப்பு cpudl *cp, கட்டமைப்பு task_struct *p,
	       கட்டமைப்பு cpumask *later_mask)
{
	மாறிலி கட்டமைப்பு sched_dl_entity *dl_se = &p->dl;

	எனில் (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		முழு cpu, max_cpu = -1;

		எனில் (!sched_asym_cpucap_active())
			திரும்பு 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			எனில் (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				எனில் (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		எனில் (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		திரும்பு 1;
	} இல்லையெனில் {
		முழு best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		எனில் (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			எனில் (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			திரும்பு 1;
		}
	}
	திரும்பு 0;
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
வெறுமை cpudl_clear(கட்டமைப்பு cpudl *cp, முழு cpu, bool online)
{
	முழு old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	எனில் (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} இல்லையெனில் {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	எனில் (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	இல்லையெனில்
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
வெறுமை cpudl_set(கட்டமைப்பு cpudl *cp, முழு cpu, u64 dl)
{
	முழு old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	எனில் (old_idx == IDX_INVALID) {
		முழு new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} இல்லையெனில் {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
முழு cpudl_init(கட்டமைப்பு cpudl *cp)
{
	முழு i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(கட்டமைப்பு cpudl_item, nr_cpu_ids);
	எனில் (!cp->elements)
		திரும்பு -ENOMEM;

	எனில் (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		திரும்பு -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	திரும்பு 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
வெறுமை cpudl_cleanup(கட்டமைப்பு cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
