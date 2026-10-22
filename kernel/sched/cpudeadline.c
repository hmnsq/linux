// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#include "sched.h"

static anta_hstha ཧྲིལ parent(ཧྲིལ i)
{
	ལོག (i - 1) >> 1;
}

static anta_hstha ཧྲིལ left_child(ཧྲིལ i)
{
	ལོག (i << 1) + 1;
}

static anta_hstha ཧྲིལ right_child(ཧྲིལ i)
{
	ལོག (i << 1) + 2;
}

static void cpudl_heapify_down(sa_nracanaa cpudl *cp, ཧྲིལ idx)
{
	ཧྲིལ l, r, largest;

	ཧྲིལ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	གལ་སྲིད (left_child(idx) >= cp->size)
		ལོག;

	/* adapted from lib/prio_heap.c */
	སྐབས (1) {
		u64 largest_dl;

		l = left_child(idx);
		r = right_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		གལ་སྲིད ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		གལ་སྲིད ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		གལ་སྲིད (largest == idx)
			bha_nga;

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

static void cpudl_heapify_up(sa_nracanaa cpudl *cp, ཧྲིལ idx)
{
	ཧྲིལ p;

	ཧྲིལ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	གལ་སྲིད (idx == 0)
		ལོག;

	do {
		p = parent(idx);
		གལ་སྲིད (dl_time_before(orig_dl, cp->elements[p].dl))
			bha_nga;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} སྐབས (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

static void cpudl_heapify(sa_nracanaa cpudl *cp, ཧྲིལ idx)
{
	གལ་སྲིད (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	དེ་མིན
		cpudl_heapify_down(cp, idx);
}

static anta_hstha ཧྲིལ cpudl_maximum(sa_nracanaa cpudl *cp)
{
	ལོག cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
ཧྲིལ cpudl_find(sa_nracanaa cpudl *cp, sa_nracanaa task_struct *p,
	       sa_nracanaa cpumask *later_mask)
{
	const sa_nracanaa sched_dl_entity *dl_se = &p->dl;

	གལ་སྲིད (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		ཧྲིལ cpu, max_cpu = -1;

		གལ་སྲིད (!sched_asym_cpucap_active())
			ལོག 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			གལ་སྲིད (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				གལ་སྲིད (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		གལ་སྲིད (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		ལོག 1;
	} དེ་མིན {
		ཧྲིལ best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		གལ་སྲིད (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			གལ་སྲིད (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			ལོག 1;
		}
	}
	ལོག 0;
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
void cpudl_clear(sa_nracanaa cpudl *cp, ཧྲིལ cpu, bool online)
{
	ཧྲིལ old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	གལ་སྲིད (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} དེ་མིན {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	གལ་སྲིད (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	དེ་མིན
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
void cpudl_set(sa_nracanaa cpudl *cp, ཧྲིལ cpu, u64 dl)
{
	ཧྲིལ old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	གལ་སྲིད (old_idx == IDX_INVALID) {
		ཧྲིལ new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} དེ་མིན {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
ཧྲིལ cpudl_init(sa_nracanaa cpudl *cp)
{
	ཧྲིལ i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(sa_nracanaa cpudl_item, nr_cpu_ids);
	གལ་སྲིད (!cp->elements)
		ལོག -ENOMEM;

	གལ་སྲིད (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		ལོག -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	ལོག 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
void cpudl_cleanup(sa_nracanaa cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
