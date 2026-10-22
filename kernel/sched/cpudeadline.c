// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಪೋಷಕ(ಪೂರ್ಣಾಂಕ i)
{
	ಹಿಂತಿರುಗಿ (i - 1) >> 1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಎಡ_ಮಗು(ಪೂರ್ಣಾಂಕ i)
{
	ಹಿಂತಿರುಗಿ (i << 1) + 1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಬಲ_ಮಗು(ಪೂರ್ಣಾಂಕ i)
{
	ಹಿಂತಿರುಗಿ (i << 1) + 2;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpudl_heapify_ಕೆಳಕ್ಕೆ(ರಚನೆ cpudl *cp, ಪೂರ್ಣಾಂಕ idx)
{
	ಪೂರ್ಣಾಂಕ l, r, ಅತಿದೊಡ್ಡ;

	ಪೂರ್ಣಾಂಕ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ಆದರೆ (ಎಡ_ಮಗು(idx) >= cp->ಗಾತ್ರ_2)
		ಹಿಂತಿರುಗಿ;

	/* adapted from lib/prio_heap.c */
	ಆಗ (1) {
		u64 ಅತಿದೊಡ್ಡ_dl;

		l = ಎಡ_ಮಗು(idx);
		r = ಬಲ_ಮಗು(idx);
		ಅತಿದೊಡ್ಡ = idx;
		ಅತಿದೊಡ್ಡ_dl = orig_dl;

		ಆದರೆ ((l < cp->ಗಾತ್ರ_2) && dl_ಸಮಯ_before(orig_dl,
						cp->elements[l].dl)) {
			ಅತಿದೊಡ್ಡ = l;
			ಅತಿದೊಡ್ಡ_dl = cp->elements[l].dl;
		}
		ಆದರೆ ((r < cp->ಗಾತ್ರ_2) && dl_ಸಮಯ_before(ಅತಿದೊಡ್ಡ_dl,
						cp->elements[r].dl))
			ಅತಿದೊಡ್ಡ = r;

		ಆದರೆ (ಅತಿದೊಡ್ಡ == idx)
			ನಿಲ್ಲಿಸು;

		/* pull largest child onto idx */
		cp->elements[idx].cpu = cp->elements[ಅತಿದೊಡ್ಡ].cpu;
		cp->elements[idx].dl = cp->elements[ಅತಿದೊಡ್ಡ].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = ಅತಿದೊಡ್ಡ;
	}
	/* actual push down of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpudl_heapify_ಮೇಲೆ(ರಚನೆ cpudl *cp, ಪೂರ್ಣಾಂಕ idx)
{
	ಪೂರ್ಣಾಂಕ p;

	ಪೂರ್ಣಾಂಕ orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	ಆದರೆ (idx == 0)
		ಹಿಂತಿರುಗಿ;

	ಮಾಡು {
		p = ಪೋಷಕ(idx);
		ಆದರೆ (dl_ಸಮಯ_before(orig_dl, cp->elements[p].dl))
			ನಿಲ್ಲಿಸು;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} ಆಗ (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ಸ್ಥಿರ ಶೂನ್ಯ cpudl_heapify(ರಚನೆ cpudl *cp, ಪೂರ್ಣಾಂಕ idx)
{
	ಆದರೆ (idx > 0 && dl_ಸಮಯ_before(cp->elements[ಪೋಷಕ(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_ಮೇಲೆ(cp, idx);
	ಇಲ್ಲದಿದ್ದರೆ
		cpudl_heapify_ಕೆಳಕ್ಕೆ(cp, idx);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cpudl_ಗರಿಷ್ಠ(ರಚನೆ cpudl *cp)
{
	ಹಿಂತಿರುಗಿ cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
ಪೂರ್ಣಾಂಕ cpudl_ಕಂಡುಹಿಡಿ(ರಚನೆ cpudl *cp, ರಚನೆ task_struct *p,
	       ರಚನೆ cpumask *later_ಮುಖವಾಡ)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_dl_entity *dl_se = &p->dl;

	ಆದರೆ (later_ಮುಖವಾಡ &&
	    cpumask_and(later_ಮುಖವಾಡ, cp->free_cpus, &p->cpus_ಮುಖವಾಡ)) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cap, max_cap = 0;
		ಪೂರ್ಣಾಂಕ cpu, max_cpu = -1;

		ಆದರೆ (!sched_asym_cpucap_ಸಕ್ರಿಯ())
			ಹಿಂತಿರುಗಿ 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_ಮುಖವಾಡ) {
			ಆದರೆ (!dl_task_ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(p, cpu)) {
				cpumask_clear_cpu(cpu, later_ಮುಖವಾಡ);

				cap = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);

				ಆದರೆ (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		ಆದರೆ (cpumask_ಖಾಲಿಗಳು(later_ಮುಖವಾಡ))
			cpumask_ಗಣ_cpu(max_cpu, later_ಮುಖವಾಡ);

		ಹಿಂತಿರುಗಿ 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_cpu = cpudl_ಗರಿಷ್ಠ(cp);

		WARN_ON(ಉತ್ತಮ_cpu != -1 && !cpu_present(ಉತ್ತಮ_cpu));

		ಆದರೆ (cpumask_test_cpu(ಉತ್ತಮ_cpu, &p->cpus_ಮುಖವಾಡ) &&
		    dl_ಸಮಯ_before(dl_se->deadline, cp->elements[0].dl)) {
			ಆದರೆ (later_ಮುಖವಾಡ)
				cpumask_ಗಣ_cpu(ಉತ್ತಮ_cpu, later_ಮುಖವಾಡ);

			ಹಿಂತಿರುಗಿ 1;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
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
ಶೂನ್ಯ cpudl_clear(ರಚನೆ cpudl *cp, ಪೂರ್ಣಾಂಕ cpu, bool online)
{
	ಪೂರ್ಣಾಂಕ old_idx, new_cpu;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_ಬೀಗ_irqsave(&cp->ಬೀಗ, flags);

	old_idx = cp->elements[cpu].idx;
	ಆದರೆ (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} ಇಲ್ಲದಿದ್ದರೆ {
		new_cpu = cp->elements[cp->ಗಾತ್ರ_2 - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->ಗಾತ್ರ_2 - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->ಗಾತ್ರ_2--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	ಆದರೆ (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	ಇಲ್ಲದಿದ್ದರೆ
		__cpumask_clear_cpu(cpu, cp->free_cpus);

	raw_spin_unlock_irqrestore(&cp->ಬೀಗ, flags);
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
ಶೂನ್ಯ cpudl_ಗಣ(ರಚನೆ cpudl *cp, ಪೂರ್ಣಾಂಕ cpu, u64 dl)
{
	ಪೂರ್ಣಾಂಕ old_idx;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_ಬೀಗ_irqsave(&cp->ಬೀಗ, flags);

	old_idx = cp->elements[cpu].idx;
	ಆದರೆ (old_idx == IDX_INVALID) {
		ಪೂರ್ಣಾಂಕ new_idx = cp->ಗಾತ್ರ_2++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_ಮೇಲೆ(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} ಇಲ್ಲದಿದ್ದರೆ {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->ಬೀಗ, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
ಪೂರ್ಣಾಂಕ cpudl_init(ರಚನೆ cpudl *cp)
{
	ಪೂರ್ಣಾಂಕ i;

	raw_spin_ಬೀಗ_init(&cp->ಬೀಗ);
	cp->ಗಾತ್ರ_2 = 0;

	cp->elements = kzalloc_objs(ರಚನೆ cpudl_ಅಂಶ, nr_cpu_ids);
	ಆದರೆ (!cp->elements)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	ಆದರೆ (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		ಹಿಂತಿರುಗಿ -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	ಹಿಂತಿರುಗಿ 0;
}

/*
 * cpudl_cleanup - clean up the cpudl structure
 * @cp: the cpudl max-heap context
 */
ಶೂನ್ಯ cpudl_cleanup(ರಚನೆ cpudl *cp)
{
	free_cpumask_var(cp->free_cpus);
	kfree(cp->elements);
}
