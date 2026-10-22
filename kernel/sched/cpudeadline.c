// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/cpudeadline.c
 *
 *  Global CPU deadline management
 *
 *  Author: Juri Lelli <j.lelli@sssup.it>
 */
#ضمن "sched.h"

ساكن مضمن صحيح پلار(صحيح i)
{
	أرجع (i - 1) >> 1;
}

ساكن مضمن صحيح کیڼ_ماشوم(صحيح i)
{
	أرجع (i << 1) + 1;
}

ساكن مضمن صحيح ښی_ماشوم(صحيح i)
{
	أرجع (i << 1) + 2;
}

ساكن فراغ cpudl_heapify_ښکته(بنية cpudl *cp, صحيح idx)
{
	صحيح l, r, تر_ټولو_لوی;

	صحيح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	إذا (کیڼ_ماشوم(idx) >= cp->اندازه)
		أرجع;

	/* adapted from lib/prio_heap.c */
	طالما (1) {
		u64 تر_ټولو_لوی_dl;

		l = کیڼ_ماشوم(idx);
		r = ښی_ماشوم(idx);
		تر_ټولو_لوی = idx;
		تر_ټولو_لوی_dl = orig_dl;

		إذا ((l < cp->اندازه) && dl_وخت_before(orig_dl,
						cp->elements[l].dl)) {
			تر_ټولو_لوی = l;
			تر_ټولو_لوی_dl = cp->elements[l].dl;
		}
		إذا ((r < cp->اندازه) && dl_وخت_before(تر_ټولو_لوی_dl,
						cp->elements[r].dl))
			تر_ټولو_لوی = r;

		إذا (تر_ټولو_لوی == idx)
			اكسر;

		/* pull largest child onto idx */
		cp->elements[idx].cpu = cp->elements[تر_ټولو_لوی].cpu;
		cp->elements[idx].dl = cp->elements[تر_ټولو_لوی].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = تر_ټولو_لوی;
	}
	/* actual push down of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ساكن فراغ cpudl_heapify_پورته(بنية cpudl *cp, صحيح idx)
{
	صحيح p;

	صحيح orig_cpu = cp->elements[idx].cpu;
	u64 orig_dl = cp->elements[idx].dl;

	إذا (idx == 0)
		أرجع;

	افعل {
		p = پلار(idx);
		إذا (dl_وخت_before(orig_dl, cp->elements[p].dl))
			اكسر;
		/* pull parent onto idx */
		cp->elements[idx].cpu = cp->elements[p].cpu;
		cp->elements[idx].dl = cp->elements[p].dl;
		cp->elements[cp->elements[idx].cpu].idx = idx;
		idx = p;
	} طالما (idx != 0);
	/* actual push up of saved original values orig_* */
	cp->elements[idx].cpu = orig_cpu;
	cp->elements[idx].dl = orig_dl;
	cp->elements[cp->elements[idx].cpu].idx = idx;
}

ساكن فراغ cpudl_heapify(بنية cpudl *cp, صحيح idx)
{
	إذا (idx > 0 && dl_وخت_before(cp->elements[پلار(idx)].dl,
				cp->elements[idx].dl))
		cpudl_heapify_پورته(cp, idx);
	وإلا
		cpudl_heapify_ښکته(cp, idx);
}

ساكن مضمن صحيح cpudl_اعظمي(بنية cpudl *cp)
{
	أرجع cp->elements[0].cpu;
}

/*
 * cpudl_find - find the best (later-dl) CPU in the system
 * @cp: the cpudl max-heap context
 * @p: the task
 * @later_mask: a mask to fill in with the selected CPUs (or NULL)
 *
 * Returns: int - CPUs were found
 */
صحيح cpudl_پیدا_کړه(بنية cpudl *cp, بنية task_struct *p,
	       بنية cpumask *later_ماسک)
{
	ثابت بنية sched_dl_entity *dl_se = &p->dl;

	إذا (later_ماسک &&
	    cpumask_and(later_ماسک, cp->free_cpus, &p->cpus_ماسک)) {
		بینښه اوږد cap, max_cap = 0;
		صحيح cpu, max_cpu = -1;

		إذا (!sched_asym_cpucap_فعال())
			أرجع 1;

		/* Ensure the capacity of the CPUs fits the task. */
		for_each_cpu(cpu, later_ماسک) {
			إذا (!dl_task_ځاییږي_ظرفیت(p, cpu)) {
				cpumask_clear_cpu(cpu, later_ماسک);

				cap = arch_scale_cpu_ظرفیت(cpu);

				إذا (cap > max_cap ||
				    (cpu == task_cpu(p) && cap == max_cap)) {
					max_cap = cap;
					max_cpu = cpu;
				}
			}
		}

		إذا (cpumask_تشه(later_ماسک))
			cpumask_سېټ_cpu(max_cpu, later_ماسک);

		أرجع 1;
	} وإلا {
		صحيح غوره_cpu = cpudl_اعظمي(cp);

		WARN_ON(غوره_cpu != -1 && !cpu_present(غوره_cpu));

		إذا (cpumask_test_cpu(غوره_cpu, &p->cpus_ماسک) &&
		    dl_وخت_before(dl_se->deadline, cp->elements[0].dl)) {
			إذا (later_ماسک)
				cpumask_سېټ_cpu(غوره_cpu, later_ماسک);

			أرجع 1;
		}
	}
	أرجع 0;
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
	بینښه اوږد flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_قلف_irqsave(&cp->قلف, flags);

	old_idx = cp->elements[cpu].idx;
	إذا (old_idx == IDX_INVALID) {
		/*
		 * Nothing to remove if old_idx was invalid.
		 * This could happen if rq_online_dl or rq_offline_dl is
		 * called for a CPU without -dl tasks running.
		 */
	} وإلا {
		new_cpu = cp->elements[cp->اندازه - 1].cpu;
		cp->elements[old_idx].dl = cp->elements[cp->اندازه - 1].dl;
		cp->elements[old_idx].cpu = new_cpu;
		cp->اندازه--;
		cp->elements[new_cpu].idx = old_idx;
		cp->elements[cpu].idx = IDX_INVALID;
		cpudl_heapify(cp, old_idx);
	}
	إذا (likely(online))
		__cpumask_set_cpu(cpu, cp->free_cpus);
	وإلا
		__cpumask_clear_cpu(cpu, cp->free_cpus);

	raw_spin_unlock_irqrestore(&cp->قلف, flags);
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
فراغ cpudl_سېټ(بنية cpudl *cp, صحيح cpu, u64 dl)
{
	صحيح old_idx;
	بینښه اوږد flags;

	WARN_ON(!cpu_present(cpu));

	raw_spin_قلف_irqsave(&cp->قلف, flags);

	old_idx = cp->elements[cpu].idx;
	إذا (old_idx == IDX_INVALID) {
		صحيح new_idx = cp->اندازه++;

		cp->elements[new_idx].dl = dl;
		cp->elements[new_idx].cpu = cpu;
		cp->elements[cpu].idx = new_idx;
		cpudl_heapify_پورته(cp, new_idx);
		__cpumask_clear_cpu(cpu, cp->free_cpus);
	} وإلا {
		cp->elements[old_idx].dl = dl;
		cpudl_heapify(cp, old_idx);
	}

	raw_spin_unlock_irqrestore(&cp->قلف, flags);
}

/*
 * cpudl_init - initialize the cpudl structure
 * @cp: the cpudl max-heap context
 */
صحيح cpudl_init(بنية cpudl *cp)
{
	صحيح i;

	raw_spin_قلف_init(&cp->قلف);
	cp->اندازه = 0;

	cp->elements = kzalloc_objs(بنية cpudl_توکی, nr_cpu_ids);
	إذا (!cp->elements)
		أرجع -ENOMEM;

	إذا (!zalloc_cpumask_var(&cp->free_cpus, GFP_KERNEL)) {
		kfree(cp->elements);
		أرجع -ENOMEM;
	}

	for_each_possible_cpu(i)
		cp->elements[i].idx = IDX_INVALID;

	أرجع 0;
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
