// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#include "sched.h"

static inline 整数 parent(整数 i)
{
	返回 (i - 1) >> 1;
}

static inline 整数 left_child(整数 i)
{
	返回 (i << 1) + 1;
}

static inline 整数 right_child(整数 i)
{
	返回 (i << 1) + 2;
}

static void cpudl_heapify_down(struct cpudl *cp, 整数 idx)
{
	整数 l, r, largest;

	整数 orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	如果 (left_child(idx) >= cp->size)
		返回;

	/* adapted from lib/prio_heap.c */
	当 (1) {
		u64 largest_dl;

		l = left_child(idx);
		r = right_child(idx);
		largest = idx;
		largest_dl = orig_dl;

		如果 ((l < cp->size) && dl_time_before(orig_dl,
						cp->elements[l].dl)) {
			largest = l;
			largest_dl = cp->elements[l].dl;
		}
		如果 ((r < cp->size) && dl_time_before(largest_dl,
						cp->elements[r].dl))
			largest = r;

		如果 (largest == idx)
			break;

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

static void cpudl_heapify_up(struct cpudl *cp, 整数 idx)
{
	整数 p;

	整数 orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	如果 (idx == 0)
		返回;

	do {
		p = parent(idx);
		如果 (dl_time_before(orig_dl, cp->elements[p].dl))
			break;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} 当 (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

static void cpudl_heapify(struct cpudl *cp, 整数 idx)
{
	如果 (idx > 0 && dl_time_before(cp->elements[parent(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_up(cp, idx);
	否则
		cpudl_heapify_down(cp, idx);
}

static inline 整数 cpudl_maximum(struct cpudl *cp)
{
	返回 cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
整数 cpudl_find(struct cpudl *cp, struct task_struct *p,
	       struct cpumask *later_mask)
{
	const struct sched_dl_entity *dl_se = &p->dl;

	如果 (later_mask &&
	    cpumask_and(later_mask, cp->free_cpus, &p->cpus_mask)) {
		unsigned long cap, max_cap = 0;
		整数 cpu, max_cpu = -1;

		如果 (!sched_asym_cpucap_active())
			返回 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_mask) {
			如果 (!dl_task_fits_capacity(p, cpu)) {
				cpumask_clear_cpu(cpu, later_mask);

				cap = arch_scale_cpu_capacity(cpu);

				如果 (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		如果 (cpumask_empty(later_mask))
			cpumask_set_cpu(max_cpu, later_mask);

		返回 1;
	} 否则 {
		整数 best_cpu = cpudl_maximum(cp);

		WARN_ON(best_cpu != -1 && !cpu_present(best_cpu));

		如果 (cpumask_test_cpu(best_cpu, &p->cpus_mask) &&
		    dl_time_before(dl_se->deadline, cp->elements[0].dl)) {
			如果 (later_mask)
				cpumask_set_cpu(best_cpu, later_mask);

			返回 1;
		}
	}
	返回 0;
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
void cpudl_clear(struct cpudl *cp, 整数 cpu, bool online)
{
	整数 old_idx, new_cpu;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	如果 (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} 否则 {
		new_cpu = cp->elements[cp->size - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->size - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->size--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	如果 (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	否则
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
void cpudl_set(struct cpudl *cp, 整数 cpu, u64 dl)
{
	整数 old_idx;
	unsigned long flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_lock_irqsave(&cp->lock, flags);

	old_idx = cp->elements[cpu].idx;
	如果 (old_idx == IDX_INVALID) {
		整数 new_idx = cp->size++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_up(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} 否则 {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->lock, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
整数 cpudl_init(struct cpudl *cp)
{
	整数 i;

	raw_spin_lock_init(&cp->lock);
	cp->size = 0;

	cp->elements = kzalloc_objs(struct cpudl_item, nr_cpu_ids);
	如果 (!cp->elements)
		返回 -ENOMEM;

	如果 (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		返回 -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	返回 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
void cpudl_cleanup(struct cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
