// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#شامل "sched.h"

ایستا درونخطی صحیح parent(صحیح i)
{
	بازگشت (i - 1) >> 1;
}

ایستا درونخطی صحیح چپ_child(صحیح i)
{
	بازگشت (i << 1) + 1;
}

ایستا درونخطی صحیح راست_child(صحیح i)
{
	بازگشت (i << 1) + 2;
}

ایستا تهی cpudl_heapify_down(ساختار cpudl *cp, صحیح idx)
{
	صحیح l, r, largest;

	صحیح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	اگر (چپ_child(idx) >= cp->size)
		بازگشت;

	/* adapted from lib/prio_heap.c */
	وقتی (1) {
		u64 largest_dl;

		l = چپ_child(idx);
		r = راست_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		اگر ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		اگر ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		اگر (largest == idx)
			بشکن;

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

ایستا تهی cpudl_heapify_up(ساختار cpudl *cp, صحیح idx)
{
	صحیح p;

	صحیح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	اگر (idx == 0)
		بازگشت;

	انجامبده {
		p = parent(idx);
		اگر (dl_time_before(orig_dl, cp->elements[p].dl))
			بشکن;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} وقتی (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ایستا تهی cpudl_heapify(ساختار cpudl *cp, صحیح idx)
{
	اگر (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	وگرنه
		cpudl_heapify_down(cp, idx);
}

ایستا درونخطی صحیح cpudl_maximum(ساختار cpudl *cp)
{
	بازگشت cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
صحیح cpudl_find(ساختار cpudl *cp, ساختار task_struct *p,
	       ساختار cpumask *later_mask)
{
	ثابت ساختار sched_dl_entity *dl_se = &p->dl;

	اگر (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		صحیح cpu, max_cpu = -1;

		اگر (!sched_asym_cpucap_active())
			بازگشت 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			اگر (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				اگر (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		اگر (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		بازگشت 1;
	} وگرنه {
		صحیح best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		اگر (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			اگر (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			بازگشت 1;
		}
	}
	بازگشت 0;
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
تهی cpudl_clear(ساختار cpudl *cp, صحیح cpu, bool online)
{
	صحیح old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	اگر (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} وگرنه {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	اگر (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	وگرنه
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
تهی cpudl_set(ساختار cpudl *cp, صحیح cpu, u64 dl)
{
	صحیح old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	اگر (old_idx == IDX_INVALID) {
		صحیح new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} وگرنه {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
صحیح cpudl_init(ساختار cpudl *cp)
{
	صحیح i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(ساختار cpudl_item, nr_cpu_ids);
	اگر (!cp->elements)
		بازگشت -ENOMEM;

	اگر (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		بازگشت -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	بازگشت 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
تهی cpudl_cleanup(ساختار cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
