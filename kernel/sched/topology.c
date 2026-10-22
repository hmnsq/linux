// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#ضمن <linux/sched/isolation.h>
#ضمن <linux/sched/clock.h>
#ضمن <linux/bsearch.h>
#ضمن "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
فراغ sched_domains_mutex_قفل(فراغ)
{
	mutex_قفل(&sched_domains_mutex);
}
فراغ sched_domains_mutex_unlock(فراغ)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
ساكن cpumask_var_t sched_domains_llc_id_allocmask;
ساكن cpumask_var_t sched_domains_tmpmask;
ساكن cpumask_var_t sched_domains_tmpmask2;
صحيح max_lid;

ساكن صحيح __init sched_debug_راهاندازی(حرف *str)
{
	sched_debug_verbose = true;

	أرجع 0;
}
early_param("sched_verbose", sched_debug_راهاندازی);

#اگر_تعریف_شده CONFIG_SCHED_SMT
ساكن bool sched_smt_asym_packing __read_mostly;

ساكن صحيح __init راهاندازی_sched_smt_asym_packing(حرف *str)
{
	إذا (strcmp(str, "on"))
		أرجع 0;

	sched_smt_asym_packing = true;
	أرجع 1;
}
__setup("sched_smt_asym_packing=", راهاندازی_sched_smt_asym_packing);
#پایان_اگر

ساكن مضمن bool sched_debug(فراغ)
{
	أرجع sched_debug_verbose;
}

#تعریف_کن SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ثابت بنية sd_flag_debug sd_flag_debug[] = {
#ضمن <linux/sched/sd_flags.h>
};
#لغو_تعریف SD_FLAG

ساكن صحيح sched_domain_debug_one(بنية sched_domain *sd, صحيح cpu, صحيح level,
				  بنية cpumask *groupmask)
{
	بنية sched_گروه *گروه = sd->groups;
	بیعلامت دراز flags = sd->flags;
	بیعلامت صحيح idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_آرگومانها(sched_domain_بازه(sd)), sd->name);

	إذا (!cpumask_test_cpu(cpu, sched_domain_بازه(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	إذا (گروه && !cpumask_test_cpu(cpu, sched_گروه_بازه(گروه))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_مجموعه_بیت(idx, &flags, __SD_FLAG_CNT) {
		بیعلامت صحيح flag = بیت(idx);
		بیعلامت صحيح meta_flags = sd_flag_debug[idx].meta_flags;

		إذا ((meta_flags & SDF_SHARED_فرزند) && sd->فرزند &&
		    !(sd->فرزند->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		إذا ((meta_flags & SDF_SHARED_والد) && sd->والد &&
		    !(sd->والد->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	افعل {
		إذا (!گروه) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			اكسر;
		}

		إذا (cpumask_خالی(sched_گروه_بازه(گروه))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			اكسر;
		}

		إذا (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_گروه_بازه(گروه))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			اكسر;
		}

		cpumask_or(groupmask, groupmask, sched_گروه_بازه(گروه));

		printk(KERN_CONT " %d:{ span=%*pbl",
				گروه->sgc->id,
				cpumask_pr_آرگومانها(sched_گروه_بازه(گروه)));

		إذا ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(گروه_balance_ماسک(گروه), sched_گروه_بازه(گروه))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_آرگومانها(گروه_balance_ماسک(گروه)));
		}

		إذا (گروه->sgc->ظرفیت != SCHED_ظرفیت_SCALE)
			printk(KERN_CONT " cap=%lu", گروه->sgc->ظرفیت);

		إذا (گروه == sd->groups && sd->فرزند &&
		    !cpumask_equal(sched_domain_بازه(sd->فرزند),
				   sched_گروه_بازه(گروه))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		گروه = گروه->بعدی;

		إذا (گروه != sd->groups)
			printk(KERN_CONT ",");

	} طالما (گروه != sd->groups);
	printk(KERN_CONT "\n");

	إذا (!cpumask_equal(sched_domain_بازه(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	إذا (sd->والد &&
	    !cpumask_subset(groupmask, sched_domain_بازه(sd->والد)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	أرجع 0;
}

ساكن فراغ sched_domain_debug(بنية sched_domain *sd, صحيح cpu)
{
	صحيح level = 0;

	إذا (!sched_debug_verbose)
		أرجع;

	إذا (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		أرجع;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	لكل (;;) {
		إذا (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			اكسر;
		level++;
		sd = sd->والد;
		إذا (!sd)
			اكسر;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#تعریف_کن SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
ساكن ثابت بیعلامت صحيح SD_DEGENERATE_GROUPS_ماسک =
#ضمن <linux/sched/sd_flags.h>
0;
#لغو_تعریف SD_FLAG

ساكن صحيح sd_degenerate(بنية sched_domain *sd)
{
	إذا (cpumask_وزن(sched_domain_بازه(sd)) == 1)
		أرجع 1;

	/* Following flags need at least 2 groups */
	إذا ((sd->flags & SD_DEGENERATE_GROUPS_ماسک) &&
	    (sd->groups != sd->groups->بعدی))
		أرجع 0;

	/* Following flags don't use groups */
	إذا (sd->flags & (SD_WAKE_AFFINE))
		أرجع 0;

	أرجع 1;
}

ساكن صحيح
sd_والد_degenerate(بنية sched_domain *sd, بنية sched_domain *والد)
{
	بیعلامت دراز cflags = sd->flags, pflags = والد->flags;

	إذا (sd_degenerate(والد))
		أرجع 1;

	إذا (!cpumask_equal(sched_domain_بازه(sd), sched_domain_بازه(والد)))
		أرجع 0;

	/* Flags needing groups don't count if only 1 group in parent */
	إذا (والد->groups == والد->groups->بعدی)
		pflags &= ~SD_DEGENERATE_GROUPS_ماسک;

	إذا (~cflags & pflags)
		أرجع 0;

	أرجع 1;
}

#إذا defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_کلید_FALSE(sched_energy_present);
ساكن بیعلامت صحيح sysctl_sched_energy_aware = 1;
ساكن DEFINE_MUTEX(sched_energy_mutex);
ساكن bool sched_energy_update;

ساكن bool sched_is_eas_possible(ثابت بنية cpumask *cpu_ماسک)
{
	bool any_asym_ظرفیت = false;
	صحيح i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_ماسک) {
		إذا (rcu_access_اشاره_گر(در_هر_cpu(sd_asym_cpucapacity, i))) {
			any_asym_ظرفیت = true;
			اكسر;
		}
	}
	إذا (!any_asym_ظرفیت) {
		إذا (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_آرگومانها(cpu_ماسک));
		}
		أرجع false;
	}

	/* EAS definitely does *not* handle SMT */
	إذا (sched_smt_فعال()) {
		إذا (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_آرگومانها(cpu_ماسک));
		}
		أرجع false;
	}

	إذا (!arch_scale_freq_invariant()) {
		إذا (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_آرگومانها(cpu_ماسک));
		}
		أرجع false;
	}

	إذا (!cpufreq_ready_for_eas(cpu_ماسک)) {
		إذا (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_آرگومانها(cpu_ماسک));
		}
		أرجع false;
	}

	أرجع true;
}

فراغ rebuild_sched_domains_energy(فراغ)
{
	mutex_قفل(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#اگر_تعریف_شده CONFIG_SYSCTL
ساكن صحيح sched_energy_aware_handler(ثابت بنية ctl_جدول *جدول, صحيح اكتب,
		فراغ *میانگیر, اندازه_t *lenp, loff_t *ppos)
{
	صحيح ret;

	إذا (اكتب && !capable(CAP_SYS_ADMIN))
		أرجع -EPERM;

	إذا (!sched_is_eas_possible(cpu_فعال_ماسک)) {
		إذا (اكتب) {
			أرجع -EOPNOTSUPP;
		} وإلا {
			*lenp = 0;
			أرجع 0;
		}
	}

	ret = proc_dointvec_minmax(جدول, اكتب, میانگیر, lenp, ppos);
	إذا (!ret && اكتب) {
		إذا (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	أرجع ret;
}

ساكن ثابت بنية ctl_جدول sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.داده           = &sysctl_sched_energy_aware,
		.maxlen         = حجم(بیعلامت صحيح),
		.نما           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ساكن صحيح __init sched_energy_aware_sysctl_init(فراغ)
{
	ثبات_sysctl_init("kernel", sched_energy_aware_sysctls);
	أرجع 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#پایان_اگر /* CONFIG_SYSCTL */

ساكن فراغ free_pd(بنية perf_domain *pd)
{
	بنية perf_domain *tmp;

	طالما (pd) {
		tmp = pd->بعدی;
		kfree(pd);
		pd = tmp;
	}
}

ساكن بنية perf_domain *پیدا_کن_pd(بنية perf_domain *pd, صحيح cpu)
{
	طالما (pd) {
		إذا (cpumask_test_cpu(cpu, perf_domain_بازه(pd)))
			أرجع pd;
		pd = pd->بعدی;
	}

	أرجع عدم;
}

ساكن بنية perf_domain *pd_init(صحيح cpu)
{
	بنية em_perf_domain *obj = em_cpu_get(cpu);
	بنية perf_domain *pd;

	إذا (!obj) {
		إذا (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		أرجع عدم;
	}

	pd = kzalloc_obj(*pd);
	إذا (!pd)
		أرجع عدم;
	pd->em_pd = obj;

	أرجع pd;
}

ساكن فراغ perf_domain_debug(ثابت بنية cpumask *cpu_map,
						بنية perf_domain *pd)
{
	إذا (!sched_debug() || !pd)
		أرجع;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_آرگومانها(cpu_map));

	طالما (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_اولی(perf_domain_بازه(pd)),
				cpumask_pr_آرگومانها(perf_domain_بازه(pd)),
				em_pd_nr_perf_حالتها(pd->em_pd));
		pd = pd->بعدی;
	}

	printk(KERN_CONT "\n");
}

ساكن فراغ destroy_perf_domain_rcu(بنية rcu_سر *rp)
{
	بنية perf_domain *pd;

	pd = container_از(rp, بنية perf_domain, rcu);
	free_pd(pd);
}

ساكن فراغ sched_energy_مجموعه(bool has_eas)
{
	إذا (!has_eas && sched_energy_enabled()) {
		إذا (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} وإلا إذا (has_eas && !sched_energy_enabled()) {
		إذا (sched_debug())
			pr_info("%s: starting EAS\n", __func__);
		static_branch_enable_cpuslocked(&sched_energy_present);
	}
}

/*
 * EAS can be used on a root domain if it meets all the following conditions:
 *    1. an Energy Model (EM) is available;
 *    2. the SD_ASYM_CPUCAPACITY flag is set in the sched_domain hierarchy.
 *    3. no SMT is detected.
 *    4. schedutil is driving the frequency of all CPUs of the rd;
 *    5. frequency invariance support is present;
 */
ساكن bool build_perf_domains(ثابت بنية cpumask *cpu_map)
{
	صحيح i;
	بنية perf_domain *pd = عدم, *tmp;
	صحيح cpu = cpumask_اولی(cpu_map);
	بنية ریشه_domain *rd = cpu_rq(cpu)->rd;

	إذا (!sysctl_sched_energy_aware)
		اذهب حرر;

	إذا (!sched_is_eas_possible(cpu_map))
		اذهب حرر;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		إذا (پیدا_کن_pd(pd, i))
			استمر;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		إذا (!tmp)
			اذهب حرر;
		tmp->بعدی = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_تخصیص_بده_اشاره_گر(rd->pd, pd);
	إذا (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	أرجع !!pd;

حرر:
	free_pd(pd);
	tmp = rd->pd;
	rcu_تخصیص_بده_اشاره_گر(rd->pd, عدم);
	إذا (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	أرجع false;
}
#وإلا /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
ساكن فراغ free_pd(بنية perf_domain *pd) { }
#پایان_اگر /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

ساكن فراغ free_rootdomain(بنية rcu_سر *rcu)
{
	بنية ریشه_domain *rd = container_از(rcu, بنية ریشه_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_ماسک);
	free_cpumask_var(rd->rto_ماسک);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->بازه);
	free_pd(rd->pd);
	kfree(rd);
}

فراغ rq_attach_ریشه(بنية rq *rq, بنية ریشه_domain *rd)
{
	بنية ریشه_domain *old_rd = عدم;
	بنية rq_flags rf;

	rq_قفل_irqsave(rq, &rf);

	إذا (rq->rd) {
		old_rd = rq->rd;

		إذا (cpumask_test_cpu(rq->cpu, old_rd->online))
			مجموعه_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->بازه);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		إذا (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = عدم;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_مجموعه_cpu(rq->cpu, rd->بازه);
	إذا (cpumask_test_cpu(rq->cpu, cpu_فعال_ماسک))
		مجموعه_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	إذا (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#اگر_تعریف_شده CONFIG_SCHED_CLASS_EXT
	إذا (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#پایان_اگر

	rq_unlock_irqrestore(rq, &rf);

	إذا (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

فراغ sched_get_rd(بنية ریشه_domain *rd)
{
	atomic_inc(&rd->refcount);
}

فراغ sched_put_rd(بنية ریشه_domain *rd)
{
	إذا (!atomic_dec_and_test(&rd->refcount))
		أرجع;

	call_rcu(&rd->rcu, free_rootdomain);
}

ساكن صحيح init_rootdomain(بنية ریشه_domain *rd)
{
	إذا (!zalloc_cpumask_var(&rd->بازه, GFP_KERNEL))
		اذهب بیرون;
	إذا (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		اذهب free_بازه;
	إذا (!zalloc_cpumask_var(&rd->dlo_ماسک, GFP_KERNEL))
		اذهب free_online;
	إذا (!zalloc_cpumask_var(&rd->rto_ماسک, GFP_KERNEL))
		اذهب free_dlo_ماسک;

#اگر_تعریف_شده HAVE_RT_افزودن_IPI
	rd->rto_cpu = -1;
	raw_spin_قفل_init(&rd->rto_قفل);
	rd->rto_افزودن_work = IRQ_WORK_INIT_HARD(rto_افزودن_irq_work_func);
#پایان_اگر

	rd->بازدید_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	إذا (cpudl_init(&rd->cpudl) != 0)
		اذهب free_rto_ماسک;

	إذا (cpupri_init(&rd->cpupri) != 0)
		اذهب free_cpudl;
	أرجع 0;

free_cpudl:
	cpudl_cleanup(&rd->cpudl);
free_rto_ماسک:
	free_cpumask_var(rd->rto_ماسک);
free_dlo_ماسک:
	free_cpumask_var(rd->dlo_ماسک);
free_online:
	free_cpumask_var(rd->online);
free_بازه:
	free_cpumask_var(rd->بازه);
بیرون:
	أرجع -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
بنية ریشه_domain def_ریشه_domain;

فراغ __init init_defrootdomain(فراغ)
{
	init_rootdomain(&def_ریشه_domain);

	atomic_مجموعه(&def_ریشه_domain.refcount, 1);
}

ساكن بنية ریشه_domain *alloc_rootdomain(فراغ)
{
	بنية ریشه_domain *rd;

	rd = kzalloc_obj(*rd);
	إذا (!rd)
		أرجع عدم;

	إذا (init_rootdomain(rd) != 0) {
		kfree(rd);
		أرجع عدم;
	}

	أرجع rd;
}

ساكن فراغ free_sched_groups(بنية sched_گروه *sg, صحيح free_sgc)
{
	بنية sched_گروه *tmp, *اولی;

	إذا (!sg)
		أرجع;

	اولی = sg;
	افعل {
		tmp = sg->بعدی;

		إذا (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		إذا (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} طالما (sg != اولی);
}

ساكن فراغ free_sched_domain_shared(بنية sched_domain_shared *sds)
{
	إذا (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

ساكن فراغ destroy_sched_domain(بنية sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#اگر_تعریف_شده CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#پایان_اگر
	kfree(sd);
}

ساكن فراغ destroy_sched_domains_rcu(بنية rcu_سر *rcu)
{
	بنية sched_domain *sd = container_از(rcu, بنية sched_domain, rcu);

	طالما (sd) {
		بنية sched_domain *والد = sd->والد;
		destroy_sched_domain(sd);
		sd = والد;
	}
}

ساكن فراغ destroy_sched_domains(بنية sched_domain *sd)
{
	إذا (sd)
		call_rcu(&sd->rcu, destroy_sched_domains_rcu);
}

/*
 * Keep a special pointer to the highest sched_domain that has SD_SHARE_LLC set
 * (Last Level Cache Domain) for this allows us to avoid some pointer chasing
 * select_idle_sibling().
 *
 * Also keep a unique ID per domain (we use the first CPU number in the cpumask
 * of the domain), this allows us to quickly tell if two CPUs are in the same
 * cache domain, see cpus_share_cache().
 */
DEFINE_در_هر_CPU(بنية sched_domain __rcu *, sd_llc);
DEFINE_در_هر_CPU(صحيح, sd_llc_اندازه);
DEFINE_در_هر_CPU(صحيح, sd_llc_id) = -1;
DEFINE_در_هر_CPU(صحيح, sd_share_id);
DEFINE_در_هر_CPU(بنية sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_در_هر_CPU(بنية sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_در_هر_CPU(بنية sched_domain __rcu *, sd_numa);
DEFINE_در_هر_CPU(بنية sched_domain __rcu *, sd_asym_packing);
DEFINE_در_هر_CPU(بنية sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_کلید_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_کلید_FALSE(sched_cluster_فعال);

ساكن فراغ update_بالاترین_cache_domain(صحيح cpu)
{
	بنية sched_domain_shared *sds = عدم;
	بنية sched_domain *sd;
	صحيح id = cpu;
	صحيح اندازه = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	إذا (sd) {
		id = cpumask_اولی(sched_domain_بازه(sd));
		اندازه = cpumask_وزن(sched_domain_بازه(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_تخصیص_بده_اشاره_گر(در_هر_cpu(sd_llc, cpu), sd);
	در_هر_cpu(sd_llc_اندازه, cpu) = اندازه;
	rcu_تخصیص_بده_اشاره_گر(در_هر_cpu(sd_llc_shared, cpu), sds);

	sd = پایینترین_flag_domain(cpu, SD_CLUSTER);
	إذا (sd)
		id = cpumask_اولی(sched_domain_بازه(sd));

	/*
	 * This assignment should be placed after the sd_llc_id as
	 * we want this id equals to cluster id on cluster machines
	 * but equals to LLC id on non-Cluster machines.
	 */
	در_هر_cpu(sd_share_id, cpu) = id;

	sd = پایینترین_flag_domain(cpu, SD_NUMA);
	rcu_تخصیص_بده_اشاره_گر(در_هر_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_تخصیص_بده_اشاره_گر(در_هر_cpu(sd_asym_packing, cpu), sd);

	sd = پایینترین_flag_domain(cpu, SD_ASYM_CPUCAPACITY_پر);
	/*
	 * The shared object is attached to sd_asym_cpucapacity only when the
	 * asym domain is non-overlapping (i.e., not built from SD_NUMA).
	 * On overlapping (NUMA) asym domains we fall back to letting the
	 * SD_SHARE_LLC path own the shared object, so sd->shared may be NULL
	 * here.
	 */
	إذا (sd && sd->shared)
		sds = sd->shared;

	rcu_تخصیص_بده_اشاره_گر(در_هر_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_تخصیص_بده_اشاره_گر(در_هر_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
ساكن فراغ
cpu_attach_domain(بنية sched_domain *sd, بنية ریشه_domain *rd, صحيح cpu)
{
	بنية rq *rq = cpu_rq(cpu);
	بنية sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	لكل (tmp = sd; tmp; ) {
		بنية sched_domain *والد = tmp->والد;
		إذا (!والد)
			اكسر;

		إذا (sd_والد_degenerate(tmp, والد)) {
			tmp->والد = والد->والد;

			/* Pick reference to parent->shared. */
			إذا (والد->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = والد->shared;
				والد->shared = عدم;
			}

			إذا (والد->والد) {
				والد->والد->فرزند = tmp;
				والد->والد->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			إذا (والد->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(والد);
		} وإلا
			tmp = tmp->والد;
	}

	إذا (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->والد;

		إذا (sd) {
			بنية sched_گروه *sg = sd->groups;

#اگر_تعریف_شده CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_بایتها = tmp->llc_بایتها;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = عدم;
			tmp->llc_max = 0;
			tmp->llc_بایتها = 0;
#پایان_اگر
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			افعل {
				sg->flags = 0;
			} طالما (sg != sd->groups);

			sd->فرزند = عدم;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_ریشه(rq, rd);
	tmp = rq->sd;
	rcu_تخصیص_بده_اشاره_گر(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_بالاترین_cache_domain(cpu);
}

بنية s_داده {
	بنية sched_domain_shared * __percpu *sds;
	بنية sched_domain * __percpu *sd;
	بنية ریشه_domain	*rd;
};

تعداد s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#اگر_تعریف_شده CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_کلید_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_کلید_FALSE(sched_cache_فعال);
/* user wants cache aware scheduling [0 or 1] */
صحيح sysctl_sched_cache_user = 1;

/*
 * Get the effective LLC size in bytes that @cpu's bottom sched_domain
 * can use. A CPU within a cpuset partition can only use a proportion
 * of the physical LLC, scaled by the ratio of the partition's span
 * weight to the hardware LLC sharing weight. @sd should be the
 * topmost domain with SD_SHARE_LLC.
 *
 * Returns 0 if cacheinfo is not yet populated. This happens during
 * early boot when build_sched_domains() runs before the generic
 * cacheinfo framework has been initialized (cacheinfo_cpu_online()
 * is a device_initcall cpuhp callback). In that case,
 * cacheinfo_cpu_online() will later call sched_update_llc_bytes()
 * to fill in the bottom domain's llc_bytes once the cache attributes
 * are available.
 */
ساكن بیعلامت دراز get_effective_llc_بایتها(صحيح cpu,
					     بنية sched_domain *sd)
{
	بنية cacheinfo *ci;
	بیعلامت صحيح hw_وزن;

	ci = get_cpu_cacheinfo_llc(cpu);
	إذا (!ci)
		أرجع 0;

	hw_وزن = cpumask_وزن(&ci->shared_cpu_map);
	إذا (!hw_وزن)
		أرجع 0;

	أرجع div_u64((u64)ci->اندازه * sd->بازه_وزن, hw_وزن);
}

ساكن bool alloc_sd_llc(ثابت بنية cpumask *cpu_map,
			 بنية s_داده *d)
{
	بنية sched_domain *sd, *بالاترین_llc, *والد;
	بیعلامت صحيح *p;
	صحيح i;

	for_each_cpu(i, cpu_map) {
		sd = *در_هر_cpu_ptr(d->sd, i);
		إذا (!sd)
			اذهب err;

		p = kcalloc_گره(max_lid + 1, حجم(بیعلامت صحيح),
				 GFP_KERNEL, cpu_مقصد_گره(i));
		إذا (!p)
			اذهب err;

		بالاترین_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		طالما ((والد = rcu_dereference_protected(بالاترین_llc->والد, true)) &&
		       (والد->flags & SD_SHARE_LLC))
			بالاترین_llc = والد;

		إذا (بالاترین_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_بایتها = get_effective_llc_بایتها(i, بالاترین_llc);
		} وإلا {
			/* avoid memory leak */
			kfree(p);
		}
	}

	أرجع true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *در_هر_cpu_ptr(d->sd, i);
		إذا (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = عدم;
			sd->llc_max = 0;
			sd->llc_بایتها = 0;
		}
	}

	أرجع false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
ساكن فراغ sched_cache_فعال_مجموعه_2(فراغ)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	إذا (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_فعال);
		إذا (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		أرجع;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	إذا (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_فعال);
		إذا (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} وإلا {
		static_branch_disable_cpuslocked(&sched_cache_فعال);
		إذا (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
فراغ sched_cache_فعال_مجموعه(فراغ)
{
	cpus_read_قفل();
	sched_domains_mutex_قفل();
	sched_cache_فعال_مجموعه_2();
	sched_domains_mutex_unlock();
	cpus_read_unlock();
}

/*
 * Update the bottom sched_domain's llc_bytes for @cpus sharing a physical
 * LLC. Called from cacheinfo_cpu_online() or
 * cacheinfo_cpu_pre_down() with cpu hotplug lock held.
 *
 * Note: get_effective_llc_bytes() returns 0 on PowerPC.
 * thus cache aware scheduling is disabled on PowerPC for
 * now. PowerPC does not use the generic cacheinfo framework --
 * it has its own cacheinfo with a separate struct cache hierarchy
 * and does not populates the per-CPU struct cpu_cacheinfo array
 * that get_cpu_cacheinfo_llc() reads.
 */
فراغ sched_update_llc_بایتها(ثابت بنية cpumask *cpus)
{
	بنية sched_domain *sd, *sdp;
	بیعلامت صحيح i;

	sched_domains_mutex_قفل();

	/*
	 * ci->shared_cpu_map is built incrementally as CPUs come
	 * online, so the first CPU in an LLC initially sees
	 * hw_weight == 1 and computes an inflated llc_bytes in
	 * get_effective_llc_bytes().  Re-evaluating every LLC
	 * sibling on each online event corrects this once the full
	 * shared_cpu_map is known.
	 *
	 * The departing CPU's domains have already been detached when
	 * cacheinfo removes it. Use the surviving cache siblings instead.
	 * They may belong to different cpuset partitions, so use each CPU's
	 * own LLC domain to scale its share of the physical cache.
	 */
	for_each_cpu(i, cpus) {
		sdp = rcu_dereference_sched_domain(در_هر_cpu(sd_llc, i));
		إذا (!sdp)
			استمر;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		إذا (sd)
			sd->llc_بایتها = get_effective_llc_بایتها(i, sdp);
	}

	sched_domains_mutex_unlock();
}

ساكن فراغ sched_cache_مجموعه(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	إذا (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	وإلا
		static_branch_disable_cpuslocked(&sched_cache_present);

	sched_cache_فعال_مجموعه_2();
}
#وإلا
ساكن bool alloc_sd_llc(ثابت بنية cpumask *cpu_map,
			 بنية s_داده *d)
{
	أرجع false;
}
ساكن مضمن فراغ sched_cache_مجموعه(bool has_multi_llcs) { }
#پایان_اگر

/*
 * Return true if @sd belongs to an LLC group whose enclosing
 * partition spans more than one LLC. @sd must be the topmost
 * SD_SHARE_LLC domain.
 *
 * Any duplicated parent domains with the same span as @sd are
 * skipped: before cpu_attach_domain() degeneration these still
 * exist, after degeneration the loop is a no-op. This makes the
 * helper usable both during sched domain build and against an
 * already-attached domain tree.
 *
 * Note: For systems with a single LLC per node, cache-aware
 * scheduling is still enabled when multiple nodes exist.
 * However, NUMA balancing decisions take precedence over
 * cache-aware scheduling. Conversely, if there is only one
 * LLC per partition, cache-aware scheduling should be disabled.
 */
ساكن bool sd_درون_multi_llcs(بنية sched_domain *sd)
{
	بنية sched_domain *sdp = sd->والد;

	/* it does not make sense to aggregate to 1 CPU */
	إذا (sd->بازه_وزن == 1)
		أرجع false;

	طالما (sdp && sdp->بازه_وزن == sd->بازه_وزن)
		sdp = sdp->والد;

	أرجع !!sdp;
}

/*
 * Return the canonical balance CPU for this group, this is the first CPU
 * of this group that's also in the balance mask.
 *
 * The balance mask are all those CPUs that could actually end up at this
 * group. See build_balance_mask().
 *
 * Also see should_we_balance().
 */
صحيح گروه_balance_cpu(بنية sched_گروه *sg)
{
	أرجع cpumask_اولی(گروه_balance_ماسک(sg));
}


/*
 * NUMA topology (first read the regular topology blurb below)
 *
 * Given a node-distance table, for example:
 *
 *   node   0   1   2   3
 *     0:  10  20  30  20
 *     1:  20  10  20  30
 *     2:  30  20  10  20
 *     3:  20  30  20  10
 *
 * which represents a 4 node ring topology like:
 *
 *   0 ----- 1
 *   |       |
 *   |       |
 *   |       |
 *   3 ----- 2
 *
 * We want to construct domains and groups to represent this. The way we go
 * about doing this is to build the domains on 'hops'. For each NUMA level we
 * construct the mask of all nodes reachable in @level hops.
 *
 * For the above NUMA topology that gives 3 levels:
 *
 * NUMA-2	0-3		0-3		0-3		0-3
 *  groups:	{0-1,3},{1-3}	{0-2},{0,2-3}	{1-3},{0-1,3}	{0,2-3},{0-2}
 *
 * NUMA-1	0-1,3		0-2		1-3		0,2-3
 *  groups:	{0},{1},{3}	{0},{1},{2}	{1},{2},{3}	{0},{2},{3}
 *
 * NUMA-0	0		1		2		3
 *
 *
 * As can be seen; things don't nicely line up as with the regular topology.
 * When we iterate a domain in child domain chunks some nodes can be
 * represented multiple times -- hence the "overlap" naming for this part of
 * the topology.
 *
 * In order to minimize this overlap, we only build enough groups to cover the
 * domain. For instance Node-0 NUMA-2 would only get groups: 0-1,3 and 1-3.
 *
 * Because:
 *
 *  - the first group of each domain is its child domain; this
 *    gets us the first 0-1,3
 *  - the only uncovered node is 2, who's child domain is 1-3.
 *
 * However, because of the overlap, computing a unique CPU for each group is
 * more complicated. Consider for instance the groups of NODE-1 NUMA-2, both
 * groups include the CPUs of Node-0, while those CPUs would not in fact ever
 * end up at those groups (they would end up in group: 0-1,3).
 *
 * To correct this we have to introduce the group balance mask. This mask
 * will contain those CPUs in the group that can reach this group given the
 * (child) domain tree.
 *
 * With this we can once again compute balance_cpu and sched_group_capacity
 * relations.
 *
 * XXX include words on how balance_cpu is unique and therefore can be
 * used for sched_group_capacity links.
 *
 *
 * Another 'interesting' topology is:
 *
 *   node   0   1   2   3
 *     0:  10  20  20  30
 *     1:  20  10  20  20
 *     2:  20  20  10  20
 *     3:  30  20  20  10
 *
 * Which looks a little like:
 *
 *   0 ----- 1
 *   |     / |
 *   |   /   |
 *   | /     |
 *   2 ----- 3
 *
 * This topology is asymmetric, nodes 1,2 are fully connected, but nodes 0,3
 * are not.
 *
 * This leads to a few particularly weird cases where the sched_domain's are
 * not of the same number for each CPU. Consider:
 *
 * NUMA-2	0-3						0-3
 *  groups:	{0-2},{1-3}					{1-3},{0-2}
 *
 * NUMA-1	0-2		0-3		0-3		1-3
 *
 * NUMA-0	0		1		2		3
 *
 */


/*
 * Build the balance mask; it contains only those CPUs that can arrive at this
 * group and should be considered to continue balancing.
 *
 * We do this during the group creation pass, therefore the group information
 * isn't complete yet, however since each group represents a (child) domain we
 * can fully construct this using the sched_domain bits (which are already
 * complete).
 */
ساكن فراغ
build_balance_ماسک(بنية sched_domain *sd, بنية sched_گروه *sg, بنية cpumask *ماسک)
{
	ثابت بنية cpumask *sg_بازه = sched_گروه_بازه(sg);
	بنية sd_داده *sdd = sd->خاص;
	بنية sched_domain *sibling;
	صحيح i;

	cpumask_clear(ماسک);

	for_each_cpu(i, sg_بازه) {
		sibling = *در_هر_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		إذا (!sibling->فرزند)
			استمر;

		/* If we would not end up here, we can't continue from here */
		إذا (!cpumask_equal(sg_بازه, sched_domain_بازه(sibling->فرزند)))
			استمر;

		cpumask_مجموعه_cpu(i, ماسک);
	}

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_خالی(ماسک));
}

/*
 * XXX: This creates per-node group entries; since the load-balancer will
 * immediately access remote memory to construct this group's load-balance
 * statistics having the groups node local is of dubious benefit.
 */
ساكن بنية sched_گروه *
build_گروه_from_فرزند_sched_domain(بنية sched_domain *sd, صحيح cpu)
{
	بنية sched_گروه *sg;
	بنية cpumask *sg_بازه;

	sg = kzalloc_گره(حجم(بنية sched_گروه) + cpumask_اندازه(),
			GFP_KERNEL, cpu_مقصد_گره(cpu));

	إذا (!sg)
		أرجع عدم;

	sg_بازه = sched_گروه_بازه(sg);
	إذا (sd->فرزند) {
		cpumask_copy(sg_بازه, sched_domain_بازه(sd->فرزند));
		sg->flags = sd->فرزند->flags;
	} وإلا {
		cpumask_copy(sg_بازه, sched_domain_بازه(sd));
	}

	atomic_inc(&sg->ref);
	أرجع sg;
}

ساكن فراغ init_overlap_sched_گروه(بنية sched_domain *sd,
				     بنية sched_گروه *sg)
{
	بنية cpumask *ماسک = sched_domains_tmpmask2;
	بنية sd_داده *sdd = sd->خاص;
	بنية cpumask *sg_بازه;
	صحيح cpu;

	build_balance_ماسک(sd, sg, ماسک);
	cpu = cpumask_اولی(ماسک);

	sg->sgc = *در_هر_cpu_ptr(sdd->sgc, cpu);
	إذا (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(گروه_balance_ماسک(sg), ماسک);
	وإلا
		WARN_ON_ONCE(!cpumask_equal(گروه_balance_ماسک(sg), ماسک));

	/*
	 * Initialize sgc->capacity such that even if we mess up the
	 * domains and no possible iteration will get us here, we won't
	 * die on a /0 trap.
	 */
	sg_بازه = sched_گروه_بازه(sg);
	sg->sgc->ظرفیت = SCHED_ظرفیت_SCALE * cpumask_وزن(sg_بازه);
	sg->sgc->min_ظرفیت = SCHED_ظرفیت_SCALE;
	sg->sgc->max_ظرفیت = SCHED_ظرفیت_SCALE;
}

ساكن بنية sched_domain *
پیدا_کن_descended_sibling(بنية sched_domain *sd, بنية sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	طالما (sibling->فرزند &&
	       !cpumask_subset(sched_domain_بازه(sibling->فرزند),
			       sched_domain_بازه(sd)))
		sibling = sibling->فرزند;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	طالما (sibling->فرزند &&
	       cpumask_equal(sched_domain_بازه(sibling->فرزند),
			     sched_domain_بازه(sibling)))
		sibling = sibling->فرزند;

	أرجع sibling;
}

ساكن صحيح
build_overlap_sched_groups(بنية sched_domain *sd, صحيح cpu)
{
	بنية sched_گروه *اولی = عدم, *آخرین = عدم, *sg;
	ثابت بنية cpumask *بازه = sched_domain_بازه(sd);
	بنية cpumask *covered = sched_domains_tmpmask;
	بنية sd_داده *sdd = sd->خاص;
	بنية sched_domain *sibling;
	صحيح i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, بازه, cpu) {
		بنية cpumask *sg_بازه;

		إذا (cpumask_test_cpu(i, covered))
			استمر;

		sibling = *در_هر_cpu_ptr(sdd->sd, i);

		/*
		 * Asymmetric node setups can result in situations where the
		 * domain tree is of unequal depth, make sure to skip domains
		 * that already cover the entire range.
		 *
		 * In that case build_sched_domains() will have terminated the
		 * iteration early and our sibling sd spans will be empty.
		 * Domains should always include the CPU they're built on, so
		 * check that.
		 */
		إذا (!cpumask_test_cpu(i, sched_domain_بازه(sibling)))
			استمر;

		/*
		 * Usually we build sched_group by sibling's child sched_domain
		 * But for machines whose NUMA diameter are 3 or above, we move
		 * to build sched_group by sibling's proper descendant's child
		 * domain because sibling's child sched_domain will span out of
		 * the sched_domain being built as below.
		 *
		 * Smallest diameter=3 topology is:
		 *
		 *   node   0   1   2   3
		 *     0:  10  20  30  40
		 *     1:  20  10  20  30
		 *     2:  30  20  10  20
		 *     3:  40  30  20  10
		 *
		 *   0 --- 1 --- 2 --- 3
		 *
		 * NUMA-3       0-3             N/A             N/A             0-3
		 *  groups:     {0-2},{1-3}                                     {1-3},{0-2}
		 *
		 * NUMA-2       0-2             0-3             0-3             1-3
		 *  groups:     {0-1},{1-3}     {0-2},{2-3}     {1-3},{0-1}     {2-3},{0-2}
		 *
		 * NUMA-1       0-1             0-2             1-3             2-3
		 *  groups:     {0},{1}         {1},{2},{0}     {2},{3},{1}     {3},{2}
		 *
		 * NUMA-0       0               1               2               3
		 *
		 * The NUMA-2 groups for nodes 0 and 3 are obviously buggered, as the
		 * group span isn't a subset of the domain span.
		 */
		إذا (sibling->فرزند &&
		    !cpumask_subset(sched_domain_بازه(sibling->فرزند), بازه))
			sibling = پیدا_کن_descended_sibling(sd, sibling);

		sg = build_گروه_from_فرزند_sched_domain(sibling, cpu);
		إذا (!sg)
			اذهب fail;

		sg_بازه = sched_گروه_بازه(sg);
		cpumask_or(covered, covered, sg_بازه);

		init_overlap_sched_گروه(sibling, sg);

		إذا (!اولی)
			اولی = sg;
		إذا (آخرین)
			آخرین->بعدی = sg;
		آخرین = sg;
		آخرین->بعدی = اولی;
	}
	sd->groups = اولی;

	أرجع 0;

fail:
	free_sched_groups(اولی, 0);

	أرجع -ENOMEM;
}


/*
 * Package topology (also see the load-balance blurb in fair.c)
 *
 * The scheduler builds a tree structure to represent a number of important
 * topology features. By default (default_topology[]) these include:
 *
 *  - Simultaneous multithreading (SMT)
 *  - Multi-Core Cache (MC)
 *  - Package (PKG)
 *
 * Where the last one more or less denotes everything up to a NUMA node.
 *
 * The tree consists of 3 primary data structures:
 *
 *	sched_domain -> sched_group -> sched_group_capacity
 *	    ^ ^             ^ ^
 *          `-'             `-'
 *
 * The sched_domains are per-CPU and have a two way link (parent & child) and
 * denote the ever growing mask of CPUs belonging to that level of topology.
 *
 * Each sched_domain has a circular (double) linked list of sched_group's, each
 * denoting the domains of the level below (or individual CPUs in case of the
 * first domain level). The sched_group linked by a sched_domain includes the
 * CPU of that sched_domain [*].
 *
 * Take for instance a 2 threaded, 2 core, 2 cache cluster part:
 *
 * CPU   0   1   2   3   4   5   6   7
 *
 * PKG  [                             ]
 * MC   [             ] [             ]
 * SMT  [     ] [     ] [     ] [     ]
 *
 *  - or -
 *
 * PKG  0-7 0-7 0-7 0-7 0-7 0-7 0-7 0-7
 * MC	0-3 0-3 0-3 0-3 4-7 4-7 4-7 4-7
 * SMT  0-1 0-1 2-3 2-3 4-5 4-5 6-7 6-7
 *
 * CPU   0   1   2   3   4   5   6   7
 *
 * One way to think about it is: sched_domain moves you up and down among these
 * topology levels, while sched_group moves you sideways through it, at child
 * domain granularity.
 *
 * sched_group_capacity ensures each unique sched_group has shared storage.
 *
 * There are two related construction problems, both require a CPU that
 * uniquely identify each group (for a given domain):
 *
 *  - The first is the balance_cpu (see should_we_balance() and the
 *    load-balance blurb in fair.c); for each group we only want 1 CPU to
 *    continue balancing at a higher domain.
 *
 *  - The second is the sched_group_capacity; we want all identical groups
 *    to share a single sched_group_capacity.
 *
 * Since these topologies are exclusive by construction. That is, its
 * impossible for an SMT thread to belong to multiple cores, and cores to
 * be part of multiple caches. There is a very clear and unique location
 * for each CPU in the hierarchy.
 *
 * Therefore computing a unique CPU for each group is trivial (the iteration
 * mask is redundant and set all 1s; all CPUs in a group will end up at _that_
 * group), we can simply pick the first CPU in each group.
 *
 *
 * [*] in other words, the first group of each domain is its child domain.
 */

ساكن بنية sched_گروه *get_گروه(صحيح cpu, بنية sd_داده *sdd)
{
	بنية sched_domain *sd = *در_هر_cpu_ptr(sdd->sd, cpu);
	بنية sched_domain *فرزند = sd->فرزند;
	بنية sched_گروه *sg;
	bool already_دیدهشده;

	إذا (فرزند)
		cpu = cpumask_اولی(sched_domain_بازه(فرزند));

	sg = *در_هر_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *در_هر_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_دیدهشده = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_دیدهشده != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	إذا (already_دیدهشده)
		أرجع sg;

	إذا (فرزند) {
		cpumask_copy(sched_گروه_بازه(sg), sched_domain_بازه(فرزند));
		cpumask_copy(گروه_balance_ماسک(sg), sched_گروه_بازه(sg));
		sg->flags = فرزند->flags;
	} وإلا {
		cpumask_مجموعه_cpu(cpu, sched_گروه_بازه(sg));
		cpumask_مجموعه_cpu(cpu, گروه_balance_ماسک(sg));
	}

	sg->sgc->ظرفیت = SCHED_ظرفیت_SCALE * cpumask_وزن(sched_گروه_بازه(sg));
	sg->sgc->min_ظرفیت = SCHED_ظرفیت_SCALE;
	sg->sgc->max_ظرفیت = SCHED_ظرفیت_SCALE;

	أرجع sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
ساكن صحيح
build_sched_groups(بنية sched_domain *sd, صحيح cpu)
{
	بنية sched_گروه *اولی = عدم, *آخرین = عدم;
	بنية sd_داده *sdd = sd->خاص;
	ثابت بنية cpumask *بازه = sched_domain_بازه(sd);
	بنية cpumask *covered;
	صحيح i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, بازه, cpu) {
		بنية sched_گروه *sg;

		إذا (cpumask_test_cpu(i, covered))
			استمر;

		sg = get_گروه(i, sdd);

		cpumask_or(covered, covered, sched_گروه_بازه(sg));

		إذا (!اولی)
			اولی = sg;
		إذا (آخرین)
			آخرین->بعدی = sg;
		آخرین = sg;
	}
	آخرین->بعدی = اولی;
	sd->groups = اولی;

	أرجع 0;
}

/*
 * Initialize sched groups cpu_capacity.
 *
 * cpu_capacity indicates the capacity of sched group, which is used while
 * distributing the load between different sched groups in a sched domain.
 * Typically cpu_capacity for all the groups in a sched domain will be same
 * unless there are asymmetries in the topology. If there are asymmetries,
 * group having more cpu_capacity will pickup more load compared to the
 * group having less cpu_capacity.
 */
ساكن فراغ init_sched_groups_ظرفیت(صحيح cpu, بنية sched_domain *sd)
{
	بنية sched_گروه *sg = sd->groups;
	بنية cpumask *ماسک = sched_domains_tmpmask2;

	WARN_ON(!sg);

	افعل {
		صحيح cpu, cores = 0, max_cpu = -1;

		sg->گروه_وزن = cpumask_وزن(sched_گروه_بازه(sg));

		cpumask_copy(ماسک, sched_گروه_بازه(sg));
		for_each_cpu(cpu, ماسک) {
			cores++;
			cpumask_andnot(ماسک, ماسک, cpu_smt_ماسک(cpu));
		}
		sg->cores = cores;

		إذا (!(sd->flags & SD_ASYM_PACKING))
			اذهب بعدی;

		for_each_cpu(cpu, sched_گروه_بازه(sg)) {
			إذا (max_cpu < 0)
				max_cpu = cpu;
			وإلا إذا (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

بعدی:
		sg = sg->بعدی;
	} طالما (sg != sd->groups);

	إذا (cpu != گروه_balance_cpu(sg))
		أرجع;

	update_گروه_ظرفیت(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
فراغ sched_update_asym_prefer_cpu(صحيح cpu, صحيح old_prio, صحيح new_prio)
{
	صحيح asym_prefer_cpu = cpu;
	بنية sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		بنية sched_گروه *sg;
		صحيح گروه_cpu;

		إذا (!(sd->flags & SD_ASYM_PACKING))
			استمر;

		/*
		 * Groups of overlapping domain are replicated per NUMA
		 * node and will require updating "asym_prefer_cpu" on
		 * each local copy.
		 *
		 * If you are hitting this warning, consider moving
		 * "sg->asym_prefer_cpu" to "sg->sgc->asym_prefer_cpu"
		 * which is shared by all the overlapping groups.
		 */
		WARN_ON_ONCE(sd->flags & SD_NUMA);

		sg = sd->groups;
		إذا (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			إذا (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				أرجع;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			استمر;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		إذا (new_prio >= old_prio)
			استمر;

		for_each_cpu(گروه_cpu, sched_گروه_بازه(sg)) {
			إذا (sched_asym_prefer(گروه_cpu, asym_prefer_cpu))
				asym_prefer_cpu = گروه_cpu;
		}

		WRITE_ONCE(sg->asym_prefer_cpu, asym_prefer_cpu);
	}
}

/*
 * Set of available CPUs grouped by their corresponding capacities
 * Each list entry contains a CPU mask reflecting CPUs that share the same
 * capacity.
 * The lifespan of data is unlimited.
 */
LIST_سر(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
ساكن مضمن صحيح
asym_cpu_ظرفیت_classify(ثابت بنية cpumask *sd_بازه,
			   ثابت بنية cpumask *cpu_map)
{
	بنية asym_cap_داده *مدخل;
	صحيح شمار = 0, نیافتن = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_مدخل(مدخل, &asym_cap_list, link) {
		إذا (cpumask_intersects(sd_بازه, cpu_ظرفیت_بازه(مدخل)))
			++شمار;
		وإلا إذا (cpumask_intersects(cpu_map, cpu_ظرفیت_بازه(مدخل)))
			++نیافتن;
	}

	WARN_ON_ONCE(!شمار && !list_خالی(&asym_cap_list));

	/* No asymmetry detected */
	إذا (شمار < 2)
		أرجع 0;
	/* Some of the available CPU capacity values have not been detected */
	إذا (نیافتن)
		أرجع SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	أرجع SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_پر;

}

ساكن فراغ free_asym_cap_مدخل(بنية rcu_سر *سر)
{
	بنية asym_cap_داده *مدخل = container_از(سر, بنية asym_cap_داده, rcu);
	kfree(مدخل);
}

ساكن مضمن فراغ asym_cpu_ظرفیت_update_داده(صحيح cpu)
{
	بیعلامت دراز ظرفیت = arch_scale_cpu_ظرفیت(cpu);
	بنية asym_cap_داده *درج_مدخل = عدم;
	بنية asym_cap_داده *مدخل;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_مدخل(مدخل, &asym_cap_list, link) {
		إذا (ظرفیت == مدخل->ظرفیت)
			اذهب انجامشده;
		وإلا إذا (!درج_مدخل && ظرفیت > مدخل->ظرفیت)
			درج_مدخل = list_prev_مدخل(مدخل, link);
	}

	مدخل = kzalloc(حجم(*مدخل) + cpumask_اندازه(), GFP_KERNEL);
	إذا (WARN_ONCE(!مدخل, "Failed to allocate memory for asymmetry data\n"))
		أرجع;
	مدخل->ظرفیت = ظرفیت;

	/* If NULL then the new capacity is the smallest, add last. */
	إذا (!درج_مدخل)
		list_جمع_کن_دم_rcu(&مدخل->link, &asym_cap_list);
	وإلا
		list_جمع_کن_rcu(&مدخل->link, &درج_مدخل->link);
انجامشده:
	__cpumask_set_cpu(cpu, cpu_ظرفیت_بازه(مدخل));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
ساكن فراغ asym_cpu_ظرفیت_پیمایش(فراغ)
{
	بنية asym_cap_داده *مدخل, *بعدی;
	صحيح cpu;

	list_for_each_مدخل(مدخل, &asym_cap_list, link)
		cpumask_clear(cpu_ظرفیت_بازه(مدخل));

	for_each_cpu_and(cpu, cpu_possible_ماسک, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_ظرفیت_update_داده(cpu);

	list_for_each_مدخل_امن(مدخل, بعدی, &asym_cap_list, link) {
		إذا (cpumask_خالی(cpu_ظرفیت_بازه(مدخل))) {
			list_del_rcu(&مدخل->link);
			call_rcu(&مدخل->rcu, free_asym_cap_مدخل);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	إذا (list_is_singular(&asym_cap_list)) {
		مدخل = list_اولی_مدخل(&asym_cap_list, typeof(*مدخل), link);
		list_del_rcu(&مدخل->link);
		call_rcu(&مدخل->rcu, free_asym_cap_مدخل);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

ساكن صحيح default_relax_domain_level = -1;
صحيح sched_domain_level_max;

ساكن صحيح __init راهاندازی_relax_domain_level(حرف *str)
{
	إذا (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	أرجع 1;
}
__setup("relax_domain_level=", راهاندازی_relax_domain_level);

ساكن فراغ مجموعه_domain_attribute(بنية sched_domain *sd,
				 بنية sched_domain_attr *attr)
{
	صحيح request;

	إذا (!attr || attr->relax_domain_level < 0) {
		إذا (default_relax_domain_level < 0)
			أرجع;
		request = default_relax_domain_level;
	} وإلا
		request = attr->relax_domain_level;

	إذا (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

ساكن فراغ __sdt_free(ثابت بنية cpumask *cpu_map);
ساكن صحيح __sdt_alloc(ثابت بنية cpumask *cpu_map);

ساكن فراغ __sds_free(بنية s_داده *d, ثابت بنية cpumask *cpu_map);
ساكن صحيح __sds_alloc(بنية s_داده *d, ثابت بنية cpumask *cpu_map);

ساكن فراغ __free_domain_allocs(بنية s_داده *d, تعداد s_alloc what,
				 ثابت بنية cpumask *cpu_map)
{
	بدل (what) {
	حالة sa_rootdomain:
		إذا (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	حالة sa_sd:
		free_percpu(d->sd);
		fallthrough;
	حالة sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	حالة sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	حالة sa_none:
		اكسر;
	}
}

ساكن تعداد s_alloc
__visit_domain_allocation_hell(بنية s_داده *d, ثابت بنية cpumask *cpu_map)
{
	memset(d, 0, حجم(*d));

	إذا (__sdt_alloc(cpu_map))
		أرجع sa_sd_storage;
	إذا (__sds_alloc(d, cpu_map))
		أرجع sa_sd_shared;
	d->sd = alloc_percpu(بنية sched_domain *);
	إذا (!d->sd)
		أرجع sa_sd_shared;
	d->rd = alloc_rootdomain();
	إذا (!d->rd)
		أرجع sa_sd;

	أرجع sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
ساكن فراغ claim_allocations(صحيح cpu, بنية s_داده *d)
{
	بنية sched_domain *sd;

	إذا (atomic_read(&(*در_هر_cpu_ptr(d->sds, cpu))->ref))
		*در_هر_cpu_ptr(d->sds, cpu) = عدم;

	لكل (sd = *در_هر_cpu_ptr(d->sd, cpu); sd; sd = sd->والد) {
		بنية sd_داده *sdd = sd->خاص;

		WARN_ON_ONCE(*در_هر_cpu_ptr(sdd->sd, cpu) != sd);
		*در_هر_cpu_ptr(sdd->sd, cpu) = عدم;

		إذا (atomic_read(&(*در_هر_cpu_ptr(sdd->sg, cpu))->ref))
			*در_هر_cpu_ptr(sdd->sg, cpu) = عدم;

		إذا (atomic_read(&(*در_هر_cpu_ptr(sdd->sgc, cpu))->ref))
			*در_هر_cpu_ptr(sdd->sgc, cpu) = عدم;
	}
}

#اگر_تعریف_شده CONFIG_NUMA
تعداد numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
ساكن صحيح			sched_domains_numa_سطوح;
ساكن صحيح			sched_numa_گره_سطوح;

صحيح				sched_max_numa_فاصله;
ساكن صحيح			*sched_domains_numa_فاصله;
ساكن صحيح			*sched_numa_گره_فاصله;
ساكن بنية cpumask		***sched_domains_numa_masks;
#پایان_اگر /* CONFIG_NUMA */

/*
 * SD_flags allowed in topology descriptions.
 *
 * These flags are purely descriptive of the topology and do not prescribe
 * behaviour. Behaviour is artificial and mapped in the below sd_init()
 * function. For details, see include/linux/sched/sd_flags.h.
 *
 *   SD_SHARE_CPUCAPACITY
 *   SD_SHARE_LLC
 *   SD_CLUSTER
 *   SD_NUMA
 *
 * Odd one out, which beside describing the topology has a quirk also
 * prescribes the desired behaviour that goes along with it:
 *
 *   SD_ASYM_PACKING        - describes SMT quirks
 */
#تعریف_کن TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

ساكن بنية sched_domain *
sd_init(بنية sched_domain_topology_level *tl,
	ثابت بنية cpumask *cpu_map,
	بنية sched_domain *فرزند, صحيح cpu)
{
	بنية sd_داده *sdd = &tl->داده;
	بنية sched_domain *sd = *در_هر_cpu_ptr(sdd->sd, cpu);
	صحيح sd_id, sd_وزن, sd_flags = 0;
	بنية cpumask *sd_بازه;
	u64 now = sched_clock();

	sd_بازه = sched_domain_بازه(sd);
	cpumask_and(sd_بازه, cpu_map, tl->ماسک(tl, cpu));
	sd_وزن = cpumask_وزن(sd_بازه);
	sd_id = cpumask_اولی(sd_بازه);

	إذا (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	إذا (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#اگر_تعریف_شده CONFIG_SCHED_SMT
	إذا (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#پایان_اگر
	sd_flags |= asym_cpu_ظرفیت_classify(sd_بازه, cpu_map);

	*sd = (بنية sched_domain){
		.min_interval		= sd_وزن,
		.max_interval		= 2*sd_وزن,
		.busy_ضریب		= 16,
		.imbalance_pct		= 117,

		.cache_nice_tries	= 0,

		.flags			= 1*SD_BALANCE_NEWIDLE
					| 1*SD_BALANCE_EXEC
					| 1*SD_BALANCE_FORK
					| 0*SD_BALANCE_WAKE
					| 1*SD_WAKE_AFFINE
					| 0*SD_SHARE_CPUCAPACITY
					| 0*SD_SHARE_LLC
					| 0*SD_SERIALIZE
					| 1*SD_PREFER_SIBLING
					| 0*SD_NUMA
					| sd_flags
					,

		.آخرین_balance		= jiffies,
		.balance_interval	= sd_وزن,

		/* 50% success rate */
		.newidle_call		= 512,
		.newidle_success	= 256,
		.newidle_نسبت		= 512,
		.newidle_stamp		= now,

		.max_newidle_lb_هزینه	= 0,
		.آخرین_decay_max_lb_هزینه	= jiffies,
		.فرزند			= فرزند,
		.name			= tl->name,
	};

	WARN_ONCE((sd->flags & (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY)) ==
		  (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY),
		  "CPU capacity asymmetry not supported on SMT\n");

	/*
	 * Convert topological properties into behaviour.
	 */
	إذا (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} وإلا إذا (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#اگر_تعریف_شده CONFIG_NUMA
	} وإلا إذا (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		إذا (sched_domains_numa_فاصله[tl->numa_level] > گره_reclaim_فاصله) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#پایان_اگر /* CONFIG_NUMA */
	} وإلا {
		sd->cache_nice_tries = 1;
	}

	sd->خاص = sdd;

	أرجع sd;
}

#اگر_تعریف_شده CONFIG_SCHED_SMT
صحيح cpu_smt_flags(فراغ)
{
	أرجع SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ثابت بنية cpumask *tl_smt_ماسک(بنية sched_domain_topology_level *tl, صحيح cpu)
{
	أرجع cpu_smt_ماسک(cpu);
}
#پایان_اگر

#اگر_تعریف_شده CONFIG_SCHED_CLUSTER
صحيح cpu_cluster_flags(فراغ)
{
	أرجع SD_CLUSTER | SD_SHARE_LLC;
}

ثابت بنية cpumask *tl_cls_ماسک(بنية sched_domain_topology_level *tl, صحيح cpu)
{
	أرجع cpu_clustergroup_ماسک(cpu);
}
#پایان_اگر

#اگر_تعریف_شده CONFIG_SCHED_MC
صحيح cpu_core_flags(فراغ)
{
	أرجع SD_SHARE_LLC;
}

ثابت بنية cpumask *tl_mc_ماسک(بنية sched_domain_topology_level *tl, صحيح cpu)
{
	أرجع cpu_coregroup_ماسک(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_ماسک
#تعریف_کن arch_llc_ماسک(cpu) cpu_coregroup_ماسک(cpu)
# endif

#وإلا
#تعریف_کن arch_llc_ماسک(cpu) cpumask_از(cpu)
#پایان_اگر

#تعریف_کن llc_ماسک(cpu) arch_llc_ماسک(cpu)

ثابت بنية cpumask *tl_pkg_ماسک(بنية sched_domain_topology_level *tl, صحيح cpu)
{
	أرجع cpu_گره_ماسک(cpu);
}

/*
 * Topology list, bottom-up.
 */
ساكن بنية sched_domain_topology_level default_topology[] = {
#اگر_تعریف_شده CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_ماسک, cpu_smt_flags, SMT),
#پایان_اگر

#اگر_تعریف_شده CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_ماسک, cpu_cluster_flags, CLS),
#پایان_اگر

#اگر_تعریف_شده CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_ماسک, cpu_core_flags, MC),
#پایان_اگر
	SDTL_INIT(tl_pkg_ماسک, عدم, PKG),
	{ عدم, },
};

ساكن بنية sched_domain_topology_level *sched_domain_topology =
	default_topology;
ساكن بنية sched_domain_topology_level *sched_domain_topology_saved;

#تعریف_کن for_each_sd_topology(tl)			\
	لكل (tl = sched_domain_topology; tl->ماسک; tl++)

فراغ __init مجموعه_sched_topology(بنية sched_domain_topology_level *tl)
{
	إذا (WARN_ON_ONCE(sched_smp_initialized))
		أرجع;

	sched_domain_topology = tl;
	sched_domain_topology_saved = عدم;
}

#اگر_تعریف_شده CONFIG_NUMA
ساكن صحيح cpu_numa_flags(فراغ)
{
	أرجع SD_NUMA;
}

ساكن ثابت بنية cpumask *sd_numa_ماسک(بنية sched_domain_topology_level *tl, صحيح cpu)
{
	أرجع sched_domains_numa_masks[tl->numa_level][cpu_مقصد_گره(cpu)];
}

ساكن فراغ sched_numa_warn(ثابت حرف *str)
{
	ساكن صحيح انجامشده = false;
	صحيح i,j;

	إذا (انجامشده)
		أرجع;

	انجامشده = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	لكل (i = 0; i < nr_گره_ids; i++) {
		printk(KERN_WARNING "  ");
		لكل (j = 0; j < nr_گره_ids; j++) {
			إذا (!گره_state(i, N_CPU) || !گره_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", گره_فاصله(i,j));
			وإلا
				printk(KERN_CONT " %02d  ", گره_فاصله(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool پیدا_کن_numa_فاصله(صحيح فاصله)
{
	bool یافت = false;
	صحيح i, *distances;

	إذا (فاصله == گره_فاصله(0, 0))
		أرجع true;

	rcu_read_قفل();
	distances = rcu_dereference(sched_numa_گره_فاصله);
	إذا (!distances)
		اذهب unlock;
	لكل (i = 0; i < sched_numa_گره_سطوح; i++) {
		إذا (distances[i] == فاصله) {
			یافت = true;
			اكسر;
		}
	}
unlock:
	rcu_read_unlock();

	أرجع یافت;
}

#تعریف_کن for_each_cpu_گره_but(n, nbut)		\
	for_each_گره_state(n, N_CPU)		\
		إذا (n == nbut)			\
			استمر;		\
		وإلا

/*
 * A system can have three types of NUMA topology:
 * NUMA_DIRECT: all nodes are directly connected, or not a NUMA system
 * NUMA_GLUELESS_MESH: some nodes reachable through intermediary nodes
 * NUMA_BACKPLANE: nodes can reach other nodes through a backplane
 *
 * The difference between a glueless mesh topology and a backplane
 * topology lies in whether communication between not directly
 * connected nodes goes through intermediary nodes (where programs
 * could run), or through backplane controllers. This affects
 * placement of programs.
 *
 * The type of topology can be discerned with the following tests:
 * - If the maximum distance between any nodes is 1 hop, the system
 *   is directly connected.
 * - If for two nodes A and B, located N > 1 hops away from each other,
 *   there is an intermediary node C, which is < N hops away from both
 *   nodes A and B, the system is a glueless mesh.
 */
ساكن فراغ init_numa_topology_type(صحيح offline_گره)
{
	صحيح a, b, c, n;

	n = sched_max_numa_فاصله;

	إذا (sched_domains_numa_سطوح <= 2) {
		sched_numa_topology_type = NUMA_مستقیم;
		أرجع;
	}

	for_each_cpu_گره_but(a, offline_گره) {
		for_each_cpu_گره_but(b, offline_گره) {
			/* Find two nodes furthest removed from each other. */
			إذا (گره_فاصله(a, b) < n)
				استمر;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_گره_but(c, offline_گره) {
				إذا (گره_فاصله(a, c) < n &&
				    گره_فاصله(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					أرجع;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			أرجع;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_مستقیم;
}


#تعریف_کن NR_فاصله_مقادیر (1 << فاصله_بیتها)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
ساكن صحيح numa_گره_dist(صحيح i, صحيح j)
{
	أرجع گره_فاصله(i, j);
}

صحيح arch_sched_گره_فاصله(صحيح from, صحيح مقصد)
			     __weak __alias(numa_گره_dist);

ساكن bool modified_sched_گره_فاصله(فراغ)
{
	أرجع numa_گره_dist != arch_sched_گره_فاصله;
}

ساكن صحيح sched_رکورد_numa_dist(صحيح offline_گره, صحيح (*n_dist)(صحيح, صحيح),
				  صحيح **dist, صحيح *سطوح)
{
	بیعلامت دراز *فاصله_map __free(bitmap) = عدم;
	صحيح nr_سطوح = 0;
	صحيح i, j;
	صحيح *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	فاصله_map = bitmap_alloc(NR_فاصله_مقادیر, GFP_KERNEL);
	إذا (!فاصله_map)
		أرجع -ENOMEM;

	bitmap_zero(فاصله_map, NR_فاصله_مقادیر);
	for_each_cpu_گره_but(i, offline_گره) {
		for_each_cpu_گره_but(j, offline_گره) {
			صحيح فاصله = n_dist(i, j);

			إذا (فاصله < LOCAL_فاصله || فاصله >= NR_فاصله_مقادیر) {
				sched_numa_warn("Invalid distance value range");
				أرجع -EINVAL;
			}

			bitmap_مجموعه(فاصله_map, فاصله, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_سطوح = bitmap_وزن(فاصله_map, NR_فاصله_مقادیر);

	distances = kzalloc_objs(صحيح, nr_سطوح);
	إذا (!distances)
		أرجع -ENOMEM;

	لكل (i = 0, j = 0; i < nr_سطوح; i++, j++) {
		j = پیدا_کن_بعدی_بیت(فاصله_map, NR_فاصله_مقادیر, j);
		distances[i] = j;
	}
	*dist = distances;
	*سطوح = nr_سطوح;

	أرجع 0;
}

فراغ sched_init_numa(صحيح offline_گره)
{
	بنية sched_domain_topology_level *tl;
	صحيح nr_سطوح, nr_گره_سطوح;
	صحيح i, j;
	صحيح *distances, *domain_distances;
	بنية cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	إذا (sched_رکورد_numa_dist(offline_گره, numa_گره_dist, &distances,
				   &nr_گره_سطوح))
		أرجع;

	/* Record modified NUMA distances for building sched domains */
	إذا (modified_sched_گره_فاصله()) {
		إذا (sched_رکورد_numa_dist(offline_گره, arch_sched_گره_فاصله,
					   &domain_distances, &nr_سطوح)) {
			kfree(distances);
			أرجع;
		}
	} وإلا {
		domain_distances = distances;
		nr_سطوح = nr_گره_سطوح;
	}
	rcu_تخصیص_بده_اشاره_گر(sched_numa_گره_فاصله, distances);
	WRITE_ONCE(sched_max_numa_فاصله, distances[nr_گره_سطوح - 1]);
	WRITE_ONCE(sched_numa_گره_سطوح, nr_گره_سطوح);

	/*
	 * 'nr_levels' contains the number of unique distances
	 *
	 * The sched_domains_numa_distance[] array includes the actual distance
	 * numbers.
	 */

	/*
	 * Here, we should temporarily reset sched_domains_numa_levels to 0.
	 * If it fails to allocate memory for array sched_domains_numa_masks[][],
	 * the array will contain less then 'nr_levels' members. This could be
	 * dangerous when we use it to iterate array sched_domains_numa_masks[][]
	 * in other functions.
	 *
	 * We reset it to 'nr_levels' at the end of this function.
	 */
	rcu_تخصیص_بده_اشاره_گر(sched_domains_numa_فاصله, domain_distances);

	sched_domains_numa_سطوح = 0;

	masks = kzalloc(حجم(فراغ *) * nr_سطوح, GFP_KERNEL);
	إذا (!masks)
		أرجع;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	لكل (i = 0; i < nr_سطوح; i++) {
		masks[i] = kzalloc(nr_گره_ids * حجم(فراغ *), GFP_KERNEL);
		إذا (!masks[i])
			أرجع;

		for_each_cpu_گره_but(j, offline_گره) {
			بنية cpumask *ماسک = kzalloc(cpumask_اندازه(), GFP_KERNEL);
			صحيح k;

			إذا (!ماسک)
				أرجع;

			masks[i][j] = ماسک;

			for_each_cpu_گره_but(k, offline_گره) {
				إذا (sched_debug() &&
				    (arch_sched_گره_فاصله(j, k) !=
				     arch_sched_گره_فاصله(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				إذا (arch_sched_گره_فاصله(j, k) >
				    sched_domains_numa_فاصله[i])
					استمر;

				cpumask_or(ماسک, ماسک, cpumask_از_گره(k));
			}
		}
	}
	rcu_تخصیص_بده_اشاره_گر(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	لكل (i = 0; sched_domain_topology[i].ماسک; i++);

	tl = kzalloc((i + nr_سطوح + 1) *
			حجم(بنية sched_domain_topology_level), GFP_KERNEL);
	إذا (!tl)
		أرجع;

	/*
	 * Copy the default topology bits..
	 */
	لكل (i = 0; sched_domain_topology[i].ماسک; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_ماسک, عدم, گره_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	لكل (j = 1; j < nr_سطوح; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_ماسک, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_سطوح = nr_سطوح;

	init_numa_topology_type(offline_گره);
}


ساكن فراغ sched_reset_numa(فراغ)
{
	صحيح nr_سطوح, *distances, *dom_distances = عدم;
	بنية cpumask ***masks;

	nr_سطوح = sched_domains_numa_سطوح;
	sched_numa_گره_سطوح = 0;
	sched_domains_numa_سطوح = 0;
	sched_max_numa_فاصله = 0;
	sched_numa_topology_type = NUMA_مستقیم;
	distances = sched_numa_گره_فاصله;
	إذا (sched_numa_گره_فاصله != sched_domains_numa_فاصله)
		dom_distances = sched_domains_numa_فاصله;
	rcu_تخصیص_بده_اشاره_گر(sched_numa_گره_فاصله, عدم);
	rcu_تخصیص_بده_اشاره_گر(sched_domains_numa_فاصله, عدم);
	masks = sched_domains_numa_masks;
	rcu_تخصیص_بده_اشاره_گر(sched_domains_numa_masks, عدم);
	إذا (distances || masks) {
		صحيح i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		لكل (i = 0; i < nr_سطوح && masks; i++) {
			إذا (!masks[i])
				استمر;
			for_each_گره(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	إذا (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = عدم;
	}
}

/*
 * Call with hotplug lock held
 */
فراغ sched_update_numa(صحيح cpu, bool online)
{
	صحيح گره;

	گره = cpu_مقصد_گره(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	إذا (cpumask_وزن(cpumask_از_گره(گره)) != 1)
		أرجع;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_گره : گره);
}

فراغ sched_domains_numa_masks_مجموعه(بیعلامت صحيح cpu)
{
	صحيح گره = cpu_مقصد_گره(cpu);
	صحيح i, j;

	لكل (i = 0; i < sched_domains_numa_سطوح; i++) {
		لكل (j = 0; j < nr_گره_ids; j++) {
			إذا (!گره_state(j, N_CPU))
				استمر;

			/* Set ourselves in the remote node's masks */
			إذا (arch_sched_گره_فاصله(j, گره) <=
			    sched_domains_numa_فاصله[i])
				cpumask_مجموعه_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

فراغ sched_domains_numa_masks_clear(بیعلامت صحيح cpu)
{
	صحيح i, j;

	لكل (i = 0; i < sched_domains_numa_سطوح; i++) {
		لكل (j = 0; j < nr_گره_ids; j++) {
			إذا (sched_domains_numa_masks[i][j])
				cpumask_clear_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

/*
 * sched_numa_find_closest() - given the NUMA topology, find the cpu
 *                             closest to @cpu from @cpumask.
 * cpumask: cpumask to find a cpu from
 * cpu: cpu to be close to
 *
 * returns: cpu, or nr_cpu_ids when nothing found.
 */
صحيح sched_numa_پیدا_کن_closest(ثابت بنية cpumask *cpus, صحيح cpu)
{
	صحيح i, j = cpu_مقصد_گره(cpu), یافت = nr_cpu_ids;
	بنية cpumask ***masks;

	rcu_read_قفل();
	masks = rcu_dereference(sched_domains_numa_masks);
	إذا (!masks)
		اذهب unlock;
	لكل (i = 0; i < sched_domains_numa_سطوح; i++) {
		إذا (!masks[i][j])
			اكسر;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		إذا (cpu < nr_cpu_ids) {
			یافت = cpu;
			اكسر;
		}
	}
unlock:
	rcu_read_unlock();

	أرجع یافت;
}

بنية __cmp_key {
	ثابت بنية cpumask *cpus;
	بنية cpumask ***masks;
	صحيح گره;
	صحيح cpu;
	صحيح w;
};

ساكن صحيح hop_cmp(ثابت فراغ *a, ثابت فراغ *b)
{
	بنية cpumask **prev_hop, **cur_hop = *(بنية cpumask ***)b;
	بنية __cmp_key *k = (بنية __cmp_key *)a;

	إذا (cpumask_وزن_and(k->cpus, cur_hop[k->گره]) <= k->cpu)
		أرجع 1;

	إذا (b == k->masks) {
		k->w = 0;
		أرجع 0;
	}

	prev_hop = *((بنية cpumask ***)b - 1);
	k->w = cpumask_وزن_and(k->cpus, prev_hop[k->گره]);
	إذا (k->w <= k->cpu)
		أرجع 0;

	أرجع -1;
}

/**
 * sched_numa_find_nth_cpu() - given the NUMA topology, find the Nth closest CPU
 *                             from @cpus to @cpu, taking into account distance
 *                             from a given @node.
 * @cpus: cpumask to find a cpu from
 * @cpu: CPU to start searching
 * @node: NUMA node to order CPUs by distance
 *
 * Return: cpu, or nr_cpu_ids when nothing found.
 */
صحيح sched_numa_پیدا_کن_nth_cpu(ثابت بنية cpumask *cpus, صحيح cpu, صحيح گره)
{
	بنية __cmp_key k = { .cpus = cpus, .cpu = cpu };
	بنية cpumask ***hop_masks;
	صحيح hop, ret = nr_cpu_ids;

	إذا (گره == NUMA_NO_گره)
		أرجع cpumask_nth_and(cpu, cpus, cpu_online_ماسک);

	rcu_read_قفل();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	گره = numa_نزدیکترین_گره(گره, N_CPU);
	k.گره = گره;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	إذا (!k.masks)
		اذهب unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_سطوح, حجم(k.masks[0]), hop_cmp);
	إذا (!hop_masks)
		اذهب unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][گره], k.masks[hop-1][گره]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][گره]);
unlock:
	rcu_read_unlock();
	أرجع ret;
}
EXPORT_نماد_GPL(sched_numa_پیدا_کن_nth_cpu);

/**
 * sched_numa_hop_mask() - Get the cpumask of CPUs at most @hops hops away from
 *                         @node
 * @node: The node to count hops from.
 * @hops: Include CPUs up to that many hops away. 0 means local node.
 *
 * Return: On success, a pointer to a cpumask of CPUs at most @hops away from
 * @node, an error value otherwise.
 *
 * Requires rcu_lock to be held. Returned cpumask is only valid within that
 * read-side section, copy it if required beyond that.
 *
 * Note that not all hops are equal in distance; see sched_init_numa() for how
 * distances and masks are handled.
 * Also note that this is a reflection of sched_domains_numa_masks, which may change
 * during the lifetime of the system (offline nodes are taken out of the masks).
 */
ثابت بنية cpumask *sched_numa_hop_ماسک(بیعلامت صحيح گره, بیعلامت صحيح hops)
{
	بنية cpumask ***masks;

	إذا (گره >= nr_گره_ids || hops >= sched_domains_numa_سطوح)
		أرجع ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	إذا (!masks)
		أرجع ERR_PTR(-EBUSY);

	أرجع masks[hops][گره];
}
EXPORT_نماد_GPL(sched_numa_hop_ماسک);

#پایان_اگر /* CONFIG_NUMA */

ساكن صحيح __sdt_alloc(ثابت بنية cpumask *cpu_map)
{
	بنية sched_domain_topology_level *tl;
	صحيح j;

	for_each_sd_topology(tl) {
		بنية sd_داده *sdd = &tl->داده;

		sdd->sd = alloc_percpu(بنية sched_domain *);
		إذا (!sdd->sd)
			أرجع -ENOMEM;

		sdd->sg = alloc_percpu(بنية sched_گروه *);
		إذا (!sdd->sg)
			أرجع -ENOMEM;

		sdd->sgc = alloc_percpu(بنية sched_گروه_ظرفیت *);
		إذا (!sdd->sgc)
			أرجع -ENOMEM;

		for_each_cpu(j, cpu_map) {
			بنية sched_domain *sd;
			بنية sched_گروه *sg;
			بنية sched_گروه_ظرفیت *sgc;

			sd = kzalloc_گره(حجم(بنية sched_domain) + cpumask_اندازه(),
					GFP_KERNEL, cpu_مقصد_گره(j));
			إذا (!sd)
				أرجع -ENOMEM;

			*در_هر_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_گره(حجم(بنية sched_گروه) + cpumask_اندازه(),
					GFP_KERNEL, cpu_مقصد_گره(j));
			إذا (!sg)
				أرجع -ENOMEM;

			sg->بعدی = sg;

			*در_هر_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_گره(حجم(بنية sched_گروه_ظرفیت) + cpumask_اندازه(),
					GFP_KERNEL, cpu_مقصد_گره(j));
			إذا (!sgc)
				أرجع -ENOMEM;

			sgc->id = j;

			*در_هر_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	أرجع 0;
}

ساكن فراغ __sdt_free(ثابت بنية cpumask *cpu_map)
{
	بنية sched_domain_topology_level *tl;
	صحيح j;

	for_each_sd_topology(tl) {
		بنية sd_داده *sdd = &tl->داده;

		for_each_cpu(j, cpu_map) {
			بنية sched_domain *sd;

			إذا (sdd->sd) {
				sd = *در_هر_cpu_ptr(sdd->sd, j);
				إذا (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*در_هر_cpu_ptr(sdd->sd, j));
			}

			إذا (sdd->sg)
				kfree(*در_هر_cpu_ptr(sdd->sg, j));
			إذا (sdd->sgc)
				kfree(*در_هر_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = عدم;
		free_percpu(sdd->sg);
		sdd->sg = عدم;
		free_percpu(sdd->sgc);
		sdd->sgc = عدم;
	}
}

ساكن صحيح __sds_alloc(بنية s_داده *d, ثابت بنية cpumask *cpu_map)
{
	صحيح j;

	d->sds = alloc_percpu(بنية sched_domain_shared *);
	إذا (!d->sds)
		أرجع -ENOMEM;

	for_each_cpu(j, cpu_map) {
		بنية sched_domain_shared *sds;

		sds = kzalloc_گره(حجم(بنية sched_domain_shared),
				GFP_KERNEL, cpu_مقصد_گره(j));
		إذا (!sds)
			أرجع -ENOMEM;

		*در_هر_cpu_ptr(d->sds, j) = sds;
	}

	أرجع 0;
}

ساكن فراغ __sds_free(بنية s_داده *d, ثابت بنية cpumask *cpu_map)
{
	صحيح j;

	إذا (!d->sds)
		أرجع;

	for_each_cpu(j, cpu_map)
		kfree(*در_هر_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = عدم;
}

ساكن بنية sched_domain *build_sched_domain(بنية sched_domain_topology_level *tl,
		ثابت بنية cpumask *cpu_map, بنية sched_domain_attr *attr,
		بنية sched_domain *فرزند, صحيح cpu)
{
	بنية sched_domain *sd = sd_init(tl, cpu_map, فرزند, cpu);

	إذا (فرزند) {
		sd->level = فرزند->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		فرزند->والد = sd;

		إذا (!cpumask_subset(sched_domain_بازه(فرزند),
				    sched_domain_بازه(sd))) {
			pr_err("BUG: arch topology borken\n");
			pr_err("     the %s domain not a subset of the %s domain\n",
					فرزند->name, sd->name);
			/* Fixup, ensure @sd has at least @child CPUs. */
			cpumask_or(sched_domain_بازه(sd),
				   sched_domain_بازه(sd),
				   sched_domain_بازه(فرزند));
		}

	}
	مجموعه_domain_attribute(sd, attr);

	أرجع sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
ساكن bool topology_بازه_sane(ثابت بنية cpumask *cpu_map)
{
	بنية sched_domain_topology_level *tl;
	بنية cpumask *covered, *id_دیده;
	صحيح cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_دیده = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		صحيح tl_common_flags = 0;

		إذا (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		إذا (tl_common_flags & SD_NUMA)
			استمر;

		cpumask_clear(covered);
		cpumask_clear(id_دیده);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ثابت بنية cpumask *tl_cpu_ماسک = tl->ماسک(tl, cpu);
			صحيح id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_اولی(tl_cpu_ماسک);

			إذا (cpumask_test_cpu(id, id_دیده)) {
				/* First CPU has already been seen, ensure identical spans */
				إذا (!cpumask_equal(tl->ماسک(tl, id), tl_cpu_ماسک))
					أرجع false;
			} وإلا {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				إذا (cpumask_intersects(tl_cpu_ماسک, covered))
					أرجع false;

				cpumask_or(covered, covered, tl_cpu_ماسک);
				cpumask_مجموعه_cpu(id, id_دیده);
			}
		}
	}
	أرجع true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
ساكن فراغ adjust_numa_imbalance(بنية sched_domain *sd_llc)
{
	بنية sched_domain *والد;
	بیعلامت صحيح imb_بازه = 1;
	بیعلامت صحيح imb = 0;
	بیعلامت صحيح nr_llcs;

	WARN_ON(!(sd_llc->flags & SD_SHARE_LLC));
	WARN_ON(!sd_llc->والد);

	/*
	 * For a single LLC per node, allow an
	 * imbalance up to 12.5% of the node. This is
	 * arbitrary cutoff based two factors -- SMT and
	 * memory channels. For SMT-2, the intent is to
	 * avoid premature sharing of HT resources but
	 * SMT-4 or SMT-8 *may* benefit from a different
	 * cutoff. For memory channels, this is a very
	 * rough estimate of how many channels may be
	 * active and is based on recent CPUs with
	 * many cores.
	 *
	 * For multiple LLCs, allow an imbalance
	 * until multiple tasks would share an LLC
	 * on one node while LLCs on another node
	 * remain idle. This assumes that there are
	 * enough logical CPUs per LLC to avoid SMT
	 * factors and that there is a correlation
	 * between LLCs and memory channels.
	 */
	nr_llcs = sd_llc->والد->بازه_وزن / sd_llc->بازه_وزن;
	إذا (nr_llcs == 1)
		imb = sd_llc->والد->بازه_وزن >> 3;
	وإلا
		imb = nr_llcs;

	imb = max(1U, imb);
	sd_llc->والد->imb_numa_nr = imb;

	/*
	 * Set span based on the first NUMA domain.
	 *
	 * NUMA systems always add a NODE domain before
	 * iterating the NUMA domains. Since this is before
	 * degeneration, start from sd_llc's parent's
	 * parent which is the lowest an SD_NUMA domain can
	 * be relative to sd_llc.
	 */
	والد = sd_llc->والد->والد;
	طالما (والد && !(والد->flags & SD_NUMA))
		والد = والد->والد;

	imb_بازه = والد ? والد->بازه_وزن : sd_llc->والد->بازه_وزن;

	/* Update the upper remainder of the topology */
	والد = sd_llc->والد;
	طالما (والد) {
		صحيح ضریب = max(1U, (والد->بازه_وزن / imb_بازه));

		والد->imb_numa_nr = imb * ضریب;
		والد = والد->والد;
	}
}

ساكن فراغ
init_sched_domain_shared(بنية s_داده *d, بنية sched_domain *sd, صحيح flags)
{
	بنية sched_domain_shared *sds = عدم;
	صحيح cpu;

	/*
	 * Multiple domains can try to claim a shared object like
	 * SD_ASYM_CPUCAPACITY and SD_SHARE_LLC which can alias to
	 * same cpumask_first(sched_domain_span(sd)) CPU and can
	 * cause "nr_idle_scan" to be populated incorrectly during
	 * load balancing.
	 *
	 * Find the first CPU in sched_domain_span(sd) with an
	 * unclaimed domain (!alloc_flags) or where the alloc_flag
	 * matches the requested flag (SD_* flag)
	 *
	 * If the domain only has single CPU, allow temporary overlap
	 * in allocation since the domains will be degenerated later.
	 */
	for_each_cpu(cpu, sched_domain_بازه(sd)) {
		sds = *در_هر_cpu_ptr(d->sds, cpu);

		إذا (!sds->alloc_flags ||
		    sd->بازه_وزن == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			اكسر;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	إذا (WARN_ON_ONCE(!sd->shared))
		sd->shared = sds;

	/*
	 * nr_busy_cpus is consumed only by the NOHZ kick path via
	 * sd_balance_shared; on the asym-capacity path it is initialized but
	 * never read.
	 */
	atomic_مجموعه(&sd->shared->nr_busy_cpus, sd->بازه_وزن);
	atomic_inc(&sd->shared->ref);
}

/*
 * For asymmetric CPU capacity, attach sched_domain_shared on the innermost
 * SD_ASYM_CPUCAPACITY_FULL ancestor of @cpu's base domain when that ancestor is
 * not an overlapping NUMA-built domain (then LLC should claim shared).
 *
 * A CPU may lack any FULL ancestor (e.g., exclusive cpuset symmetric island),
 * then LLC must claim shared instead.
 *
 * Note: SD_ASYM_CPUCAPACITY_FULL is only set when all CPU capacity values
 * are present in the domain span, so the asym domain we attach to cannot
 * degenerate into a single-capacity group. The relevant edge cases are instead
 * covered by the caveats above.
 *
 * Return true if this CPU's asym path claimed sd->shared, false otherwise.
 */
ساكن bool claim_asym_sched_domain_shared(بنية s_داده *d, صحيح cpu)
{
	بنية sched_domain *sd = *در_هر_cpu_ptr(d->sd, cpu);
	بنية sched_domain *sd_asym;

	إذا (!sd)
		أرجع false;

	sd_asym = sd;
	طالما (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_پر))
		sd_asym = sd_asym->والد;

	إذا (!sd_asym || (sd_asym->flags & SD_NUMA))
		أرجع false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	أرجع true;
}

ساكن صحيح __sched_domains_alloc_llc_id(فراغ)
{
	صحيح lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_اولی_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	إذا (lid >= nr_cpu_ids)
		أرجع -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_آخرین(sched_domains_llc_id_allocmask);
	إذا (max > max_lid)
		max_lid = max;

	أرجع lid;
}

ساكن فراغ __sched_domains_free_llc_id(صحيح cpu)
{
	صحيح i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = در_هر_cpu(sd_llc_id, cpu);
	إذا (lid == -1 || lid >= nr_cpu_ids)
		أرجع;

	در_هر_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_ماسک(cpu)) {
		/* An online CPU owns the llc_id. */
		إذا (در_هر_cpu(sd_llc_id, i) == lid)
			أرجع;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_آخرین(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	إذا (max < max_lid)
		max_lid = max;
}

فراغ sched_domains_free_llc_id(صحيح cpu)
{
	sched_domains_mutex_قفل();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
ساكن صحيح
build_sched_domains(ثابت بنية cpumask *cpu_map, بنية sched_domain_attr *attr,
		    bool *multi_llcs)
{
	تعداد s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	بنية sched_domain *sd;
	بنية s_داده d;
	بنية rq *rq = عدم;
	صحيح i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	إذا (WARN_ON(cpumask_خالی(cpu_map)))
		اذهب error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	إذا (alloc_state != sa_rootdomain)
		اذهب error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		بنية sched_domain_topology_level *tl;
		صحيح lid;

		sd = عدم;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			إذا (tl == sched_domain_topology)
				*در_هر_cpu_ptr(d.sd, i) = sd;
			إذا (cpumask_equal(cpu_map, sched_domain_بازه(sd)))
				اكسر;
		}

		lid = در_هر_cpu(sd_llc_id, i);
		إذا (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			لكل (صحيح j = cpumask_اولی(llc_ماسک(i));
			     j < nr_cpu_ids;
			     j = cpumask_بعدی(j, llc_ماسک(i))) {
				إذا (i == j)
					استمر;

				lid = در_هر_cpu(sd_llc_id, j);

				إذا (lid != -1) {
					در_هر_cpu(sd_llc_id, i) = lid;

					اكسر;
				}
			}

			/* a new LLC is detected */
			إذا (lid == -1)
				در_هر_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	إذا (WARN_ON(!topology_بازه_sane(cpu_map)))
		اذهب error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		لكل (sd = *در_هر_cpu_ptr(d.sd, i); sd; sd = sd->والد) {
			sd->بازه_وزن = cpumask_وزن(sched_domain_بازه(sd));
			إذا (sd->flags & SD_NUMA) {
				إذا (build_overlap_sched_groups(sd, i))
					اذهب error;
			} وإلا {
				إذا (build_sched_groups(sd, i))
					اذهب error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *در_هر_cpu_ptr(d.sd, i);
		إذا (!sd)
			استمر;

		إذا (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		طالما (sd->والد && (sd->والد->flags & SD_SHARE_LLC))
			sd = sd->والد;

		إذا (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			إذا (sd->والد) {
				إذا (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				إذا (sd_درون_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	لكل (i = nr_cpumask_بیتها-1; i >= 0; i--) {
		إذا (!cpumask_test_cpu(i, cpu_map))
			استمر;

		claim_allocations(i, &d);

		لكل (sd = *در_هر_cpu_ptr(d.sd, i); sd; sd = sd->والد)
			init_sched_groups_ظرفیت(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_قفل();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *در_هر_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		إذا (پایینترین_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	إذا (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	إذا (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_فعال);

	إذا (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_آرگومانها(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	أرجع ret;
}

/* Current sched domains: */
ساكن cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
ساكن صحيح				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
ساكن بنية sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
ساكن cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
صحيح __weak arch_update_cpu_topology(فراغ)
{
	أرجع 0;
}

cpumask_var_t *alloc_sched_domains(بیعلامت صحيح ndoms)
{
	صحيح i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	إذا (!doms)
		أرجع عدم;
	لكل (i = 0; i < ndoms; i++) {
		إذا (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			أرجع عدم;
		}
	}
	أرجع doms;
}

فراغ free_sched_domains(cpumask_var_t doms[], بیعلامت صحيح ndoms)
{
	بیعلامت صحيح i;
	لكل (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
صحيح __init sched_init_domains(ثابت بنية cpumask *cpu_map)
{
	bool multi_llcs;
	صحيح err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_ظرفیت_پیمایش();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	إذا (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], عدم, &multi_llcs);
	إذا (!err)
		sched_cache_مجموعه(multi_llcs);

	أرجع err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
ساكن فراغ detach_destroy_domains(ثابت بنية cpumask *cpu_map)
{
	بیعلامت صحيح cpu = cpumask_any(cpu_map);
	صحيح i;

	إذا (rcu_access_اشاره_گر(در_هر_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	إذا (static_branch_unlikely(&sched_cluster_فعال))
		static_branch_dec_cpuslocked(&sched_cluster_فعال);

	rcu_read_قفل();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(عدم, &def_ریشه_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
ساكن صحيح dattrs_equal(بنية sched_domain_attr *cur, صحيح idx_cur,
			بنية sched_domain_attr *new, صحيح idx_new)
{
	بنية sched_domain_attr tmp;

	/* Fast path: */
	إذا (!new && !cur)
		أرجع 1;

	tmp = SD_ATTR_INIT;

	أرجع !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			حجم(بنية sched_domain_attr));
}

/*
 * Partition sched domains as specified by the 'ndoms_new'
 * cpumasks in the array doms_new[] of cpumasks. This compares
 * doms_new[] to the current sched domain partitioning, doms_cur[].
 * It destroys each deleted domain and builds each new domain.
 *
 * 'doms_new' is an array of cpumask_var_t's of length 'ndoms_new'.
 * The masks don't intersect (don't overlap.) We should setup one
 * sched domain for each mask. CPUs not in any of the cpumasks will
 * not be load balanced. If the same cpumask appears both in the
 * current 'doms_cur' domains and in the new 'doms_new', we can leave
 * it as it is.
 *
 * The passed in 'doms_new' should be allocated using
 * alloc_sched_domains.  This routine takes ownership of it and will
 * free_sched_domains it when done with it. If the caller failed the
 * alloc call, then it can pass in doms_new == NULL && ndoms_new == 1,
 * and partition_sched_domains() will fallback to the single partition
 * 'fallback_doms', it also forces the domains to be rebuilt.
 *
 * If doms_new == NULL it will be replaced with cpu_online_mask.
 * ndoms_new == 0 is a special case for destroying existing domains,
 * and it will not create the default domain.
 *
 * Call with hotplug lock and sched_domains_mutex held
 */
ساكن فراغ افراز_sched_domains_locked(صحيح ndoms_new, cpumask_var_t doms_new[],
				    بنية sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	صحيح i, j, n;
	صحيح new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	إذا (new_topology)
		asym_cpu_ظرفیت_پیمایش();

	إذا (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		إذا (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_فعال_ماسک,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} وإلا {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	لكل (i = 0; i < ndoms_cur; i++) {
		لكل (j = 0; j < n && !new_topology; j++) {
			إذا (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				اذهب match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	إذا (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_فعال_ماسک,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	لكل (i = 0; i < ndoms_new; i++) {
		لكل (j = 0; j < n && !new_topology; j++) {
			إذا (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    dattrs_equal(dattr_new, i, dattr_cur, j)) {
				/*
				 * Reused partition has to be taken care
				 * of here, because there could be a corner
				 * case that if the reused partition is skipped
				 * and only new partition is considered, an
				 * incorrect has_multi_llcs would be set. For
				 * example:
				 * If the only multi-LLC partition is reused
				 * and a new single-LLC partition is built,
				 * sched_cache_set(false) disables cache-aware
				 * scheduling globally despite the reused
				 * multi-LLC partition still being active.
				 */
				بنية sched_domain *sd;
				صحيح cpu = cpumask_اولی(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				طالما (sd && sd->والد && (sd->والد->flags & SD_SHARE_LLC))
					sd = sd->والد;
				إذا (sd && (sd->flags & SD_SHARE_LLC) && sd->والد &&
				    sd_درون_multi_llcs(sd))
					has_multi_llcs = true;
				اذهب match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : عدم,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_مجموعه(has_multi_llcs);

#إذا defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	لكل (i = 0; i < ndoms_new; i++) {
		لكل (j = 0; j < n && !sched_energy_update; j++) {
			إذا (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_اولی(doms_cur[j]))->rd->pd) {
				has_eas = true;
				اذهب match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_مجموعه(has_eas);
#پایان_اگر

	/* Remember the new sched domains: */
	إذا (doms_cur != &fallback_doms)
		free_sched_domains(doms_cur, ndoms_cur);

	kfree(dattr_cur);
	doms_cur = doms_new;
	dattr_cur = dattr_new;
	ndoms_cur = ndoms_new;

	update_sched_domain_debugfs();
	dl_rebuild_rd_accounting();
}

/*
 * Call with hotplug lock held
 */
فراغ افراز_sched_domains(صحيح ndoms_new, cpumask_var_t doms_new[],
			     بنية sched_domain_attr *dattr_new)
{
	sched_domains_mutex_قفل();
	افراز_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
