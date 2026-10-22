// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#شامل <linux/sched/isolation.h>
#شامل <linux/sched/clock.h>
#شامل <linux/bsearch.h>
#شامل "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
خالی sched_domains_mutex_تالا(خالی)
{
	mutex_تالا(&sched_domains_mutex);
}
خالی sched_domains_mutex_unlock(خالی)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
ساکن cpumask_var_t sched_domains_llc_id_allocmask;
ساکن cpumask_var_t sched_domains_tmpmask;
ساکن cpumask_var_t sched_domains_tmpmask2;
صحیح max_lid;

ساکن صحیح __init sched_debug_تیاری(حرف *str)
{
	sched_debug_verbose = true;

	واپس 0;
}
early_param("sched_verbose", sched_debug_تیاری);

#اگر_تعریف_شدہ CONFIG_SCHED_SMT
ساکن bool sched_smt_asym_packing __read_mostly;

ساکن صحیح __init تیاری_sched_smt_asym_packing(حرف *str)
{
	اگر (strcmp(str, "on"))
		واپس 0;

	sched_smt_asym_packing = true;
	واپس 1;
}
__setup("sched_smt_asym_packing=", تیاری_sched_smt_asym_packing);
#اگر_ختم

ساکن اندرونی bool sched_debug(خالی)
{
	واپس sched_debug_verbose;
}

#تعریف_کرو SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ثابت ساخت sd_flag_debug sd_flag_debug[] = {
#شامل <linux/sched/sd_flags.h>
};
#تعریف_منسوخ SD_FLAG

ساکن صحیح sched_domain_debug_one(ساخت sched_domain *sd, صحیح cpu, صحیح level,
				  ساخت cpumask *groupmask)
{
	ساخت sched_گروہ *گروہ = sd->groups;
	غیرعلامتی طویل flags = sd->flags;
	غیرعلامتی صحیح idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_دلائل(sched_domain_وسعت(sd)), sd->name);

	اگر (!cpumask_test_cpu(cpu, sched_domain_وسعت(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	اگر (گروہ && !cpumask_test_cpu(cpu, sched_گروہ_وسعت(گروہ))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_سیٹ_ثنائی_ہندسہ(idx, &flags, __SD_FLAG_CNT) {
		غیرعلامتی صحیح flag = ثنائی_ہندسہ(idx);
		غیرعلامتی صحیح meta_flags = sd_flag_debug[idx].meta_flags;

		اگر ((meta_flags & SDF_SHARED_بچہ) && sd->بچہ &&
		    !(sd->بچہ->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		اگر ((meta_flags & SDF_SHARED_والد) && sd->والد &&
		    !(sd->والد->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	کرو {
		اگر (!گروہ) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			توڑو;
		}

		اگر (cpumask_خالی_خانے(sched_گروہ_وسعت(گروہ))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			توڑو;
		}

		اگر (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_گروہ_وسعت(گروہ))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			توڑو;
		}

		cpumask_or(groupmask, groupmask, sched_گروہ_وسعت(گروہ));

		printk(KERN_CONT " %d:{ span=%*pbl",
				گروہ->sgc->id,
				cpumask_pr_دلائل(sched_گروہ_وسعت(گروہ)));

		اگر ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(گروہ_balance_نقاب(گروہ), sched_گروہ_وسعت(گروہ))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_دلائل(گروہ_balance_نقاب(گروہ)));
		}

		اگر (گروہ->sgc->استعداد != SCHED_استعداد_SCALE)
			printk(KERN_CONT " cap=%lu", گروہ->sgc->استعداد);

		اگر (گروہ == sd->groups && sd->بچہ &&
		    !cpumask_equal(sched_domain_وسعت(sd->بچہ),
				   sched_گروہ_وسعت(گروہ))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		گروہ = گروہ->اگلا;

		اگر (گروہ != sd->groups)
			printk(KERN_CONT ",");

	} جب (گروہ != sd->groups);
	printk(KERN_CONT "\n");

	اگر (!cpumask_equal(sched_domain_وسعت(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	اگر (sd->والد &&
	    !cpumask_subset(groupmask, sched_domain_وسعت(sd->والد)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	واپس 0;
}

ساکن خالی sched_domain_debug(ساخت sched_domain *sd, صحیح cpu)
{
	صحیح level = 0;

	اگر (!sched_debug_verbose)
		واپس;

	اگر (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		واپس;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	برائے (;;) {
		اگر (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			توڑو;
		level++;
		sd = sd->والد;
		اگر (!sd)
			توڑو;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#تعریف_کرو SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
ساکن ثابت غیرعلامتی صحیح SD_DEGENERATE_GROUPS_نقاب =
#شامل <linux/sched/sd_flags.h>
0;
#تعریف_منسوخ SD_FLAG

ساکن صحیح sd_degenerate(ساخت sched_domain *sd)
{
	اگر (cpumask_وزن(sched_domain_وسعت(sd)) == 1)
		واپس 1;

	/* Following flags need at least 2 groups */
	اگر ((sd->flags & SD_DEGENERATE_GROUPS_نقاب) &&
	    (sd->groups != sd->groups->اگلا))
		واپس 0;

	/* Following flags don't use groups */
	اگر (sd->flags & (SD_WAKE_AFFINE))
		واپس 0;

	واپس 1;
}

ساکن صحیح
sd_والد_degenerate(ساخت sched_domain *sd, ساخت sched_domain *والد)
{
	غیرعلامتی طویل cflags = sd->flags, pflags = والد->flags;

	اگر (sd_degenerate(والد))
		واپس 1;

	اگر (!cpumask_equal(sched_domain_وسعت(sd), sched_domain_وسعت(والد)))
		واپس 0;

	/* Flags needing groups don't count if only 1 group in parent */
	اگر (والد->groups == والد->groups->اگلا)
		pflags &= ~SD_DEGENERATE_GROUPS_نقاب;

	اگر (~cflags & pflags)
		واپس 0;

	واپس 1;
}

#اگر defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_کلید_FALSE(sched_energy_present);
ساکن غیرعلامتی صحیح sysctl_sched_energy_aware = 1;
ساکن DEFINE_MUTEX(sched_energy_mutex);
ساکن bool sched_energy_update;

ساکن bool sched_is_eas_possible(ثابت ساخت cpumask *cpu_نقاب)
{
	bool any_asym_استعداد = false;
	صحیح i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_نقاب) {
		اگر (rcu_access_اشارہ_گر(فی_cpu(sd_asym_cpucapacity, i))) {
			any_asym_استعداد = true;
			توڑو;
		}
	}
	اگر (!any_asym_استعداد) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_دلائل(cpu_نقاب));
		}
		واپس false;
	}

	/* EAS definitely does *not* handle SMT */
	اگر (sched_smt_فعال()) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_دلائل(cpu_نقاب));
		}
		واپس false;
	}

	اگر (!arch_scale_freq_invariant()) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_دلائل(cpu_نقاب));
		}
		واپس false;
	}

	اگر (!cpufreq_ready_for_eas(cpu_نقاب)) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_دلائل(cpu_نقاب));
		}
		واپس false;
	}

	واپس true;
}

خالی rebuild_sched_domains_energy(خالی)
{
	mutex_تالا(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#اگر_تعریف_شدہ CONFIG_SYSCTL
ساکن صحیح sched_energy_aware_handler(ثابت ساخت ctl_جدول *جدول, صحیح اكتب,
		خالی *بفر, سائز_t *lenp, loff_t *ppos)
{
	صحیح ret;

	اگر (اكتب && !capable(CAP_SYS_ADMIN))
		واپس -EPERM;

	اگر (!sched_is_eas_possible(cpu_فعال_نقاب)) {
		اگر (اكتب) {
			واپس -EOPNOTSUPP;
		} ورنہ {
			*lenp = 0;
			واپس 0;
		}
	}

	ret = proc_dointvec_minmax(جدول, اكتب, بفر, lenp, ppos);
	اگر (!ret && اكتب) {
		اگر (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	واپس ret;
}

ساکن ثابت ساخت ctl_جدول sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.ڈیٹا           = &sysctl_sched_energy_aware,
		.maxlen         = حجم(غیرعلامتی صحیح),
		.منوال           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ساکن صحیح __init sched_energy_aware_sysctl_init(خالی)
{
	رجسٹر_sysctl_init("kernel", sched_energy_aware_sysctls);
	واپس 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#اگر_ختم /* CONFIG_SYSCTL */

ساکن خالی free_pd(ساخت perf_domain *pd)
{
	ساخت perf_domain *tmp;

	جب (pd) {
		tmp = pd->اگلا;
		kfree(pd);
		pd = tmp;
	}
}

ساکن ساخت perf_domain *ڈھونڈو_pd(ساخت perf_domain *pd, صحیح cpu)
{
	جب (pd) {
		اگر (cpumask_test_cpu(cpu, perf_domain_وسعت(pd)))
			واپس pd;
		pd = pd->اگلا;
	}

	واپس تہی;
}

ساکن ساخت perf_domain *pd_init(صحیح cpu)
{
	ساخت em_perf_domain *obj = em_cpu_get(cpu);
	ساخت perf_domain *pd;

	اگر (!obj) {
		اگر (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		واپس تہی;
	}

	pd = kzalloc_obj(*pd);
	اگر (!pd)
		واپس تہی;
	pd->em_pd = obj;

	واپس pd;
}

ساکن خالی perf_domain_debug(ثابت ساخت cpumask *cpu_map,
						ساخت perf_domain *pd)
{
	اگر (!sched_debug() || !pd)
		واپس;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_دلائل(cpu_map));

	جب (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_پہلا(perf_domain_وسعت(pd)),
				cpumask_pr_دلائل(perf_domain_وسعت(pd)),
				em_pd_nr_perf_حالتیں(pd->em_pd));
		pd = pd->اگلا;
	}

	printk(KERN_CONT "\n");
}

ساکن خالی destroy_perf_domain_rcu(ساخت rcu_سر *rp)
{
	ساخت perf_domain *pd;

	pd = container_کا(rp, ساخت perf_domain, rcu);
	free_pd(pd);
}

ساکن خالی sched_energy_سیٹ(bool has_eas)
{
	اگر (!has_eas && sched_energy_enabled()) {
		اگر (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} ورنہ اگر (has_eas && !sched_energy_enabled()) {
		اگر (sched_debug())
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
ساکن bool build_perf_domains(ثابت ساخت cpumask *cpu_map)
{
	صحیح i;
	ساخت perf_domain *pd = تہی, *tmp;
	صحیح cpu = cpumask_پہلا(cpu_map);
	ساخت جڑ_domain *rd = cpu_rq(cpu)->rd;

	اگر (!sysctl_sched_energy_aware)
		جاؤ آزاد;

	اگر (!sched_is_eas_possible(cpu_map))
		جاؤ آزاد;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		اگر (ڈھونڈو_pd(pd, i))
			جاری;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		اگر (!tmp)
			جاؤ آزاد;
		tmp->اگلا = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_تفویض_اشارہ_گر(rd->pd, pd);
	اگر (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	واپس !!pd;

آزاد:
	free_pd(pd);
	tmp = rd->pd;
	rcu_تفویض_اشارہ_گر(rd->pd, تہی);
	اگر (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	واپس false;
}
#ورنہ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
ساکن خالی free_pd(ساخت perf_domain *pd) { }
#اگر_ختم /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

ساکن خالی free_rootdomain(ساخت rcu_سر *rcu)
{
	ساخت جڑ_domain *rd = container_کا(rcu, ساخت جڑ_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_نقاب);
	free_cpumask_var(rd->rto_نقاب);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->وسعت);
	free_pd(rd->pd);
	kfree(rd);
}

خالی rq_attach_جڑ(ساخت rq *rq, ساخت جڑ_domain *rd)
{
	ساخت جڑ_domain *old_rd = تہی;
	ساخت rq_flags rf;

	rq_تالا_irqsave(rq, &rf);

	اگر (rq->rd) {
		old_rd = rq->rd;

		اگر (cpumask_test_cpu(rq->cpu, old_rd->online))
			سیٹ_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->وسعت);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		اگر (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = تہی;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_سیٹ_cpu(rq->cpu, rd->وسعت);
	اگر (cpumask_test_cpu(rq->cpu, cpu_فعال_نقاب))
		سیٹ_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	اگر (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
	اگر (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#اگر_ختم

	rq_unlock_irqrestore(rq, &rf);

	اگر (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

خالی sched_get_rd(ساخت جڑ_domain *rd)
{
	atomic_inc(&rd->refcount);
}

خالی sched_put_rd(ساخت جڑ_domain *rd)
{
	اگر (!atomic_dec_and_test(&rd->refcount))
		واپس;

	call_rcu(&rd->rcu, free_rootdomain);
}

ساکن صحیح init_rootdomain(ساخت جڑ_domain *rd)
{
	اگر (!zalloc_cpumask_var(&rd->وسعت, GFP_KERNEL))
		جاؤ برآمد;
	اگر (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		جاؤ free_وسعت;
	اگر (!zalloc_cpumask_var(&rd->dlo_نقاب, GFP_KERNEL))
		جاؤ free_online;
	اگر (!zalloc_cpumask_var(&rd->rto_نقاب, GFP_KERNEL))
		جاؤ free_dlo_نقاب;

#اگر_تعریف_شدہ HAVE_RT_ڈالو_IPI
	rd->rto_cpu = -1;
	raw_spin_تالا_init(&rd->rto_تالا);
	rd->rto_ڈالو_work = IRQ_WORK_INIT_HARD(rto_ڈالو_irq_work_func);
#اگر_ختم

	rd->دورہ_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	اگر (cpudl_init(&rd->cpudl) != 0)
		جاؤ free_rto_نقاب;

	اگر (cpupri_init(&rd->cpupri) != 0)
		جاؤ free_cpudl;
	واپس 0;

free_cpudl:
	cpudl_cleanup(&rd->cpudl);
free_rto_نقاب:
	free_cpumask_var(rd->rto_نقاب);
free_dlo_نقاب:
	free_cpumask_var(rd->dlo_نقاب);
free_online:
	free_cpumask_var(rd->online);
free_وسعت:
	free_cpumask_var(rd->وسعت);
برآمد:
	واپس -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
ساخت جڑ_domain def_جڑ_domain;

خالی __init init_defrootdomain(خالی)
{
	init_rootdomain(&def_جڑ_domain);

	atomic_سیٹ(&def_جڑ_domain.refcount, 1);
}

ساکن ساخت جڑ_domain *alloc_rootdomain(خالی)
{
	ساخت جڑ_domain *rd;

	rd = kzalloc_obj(*rd);
	اگر (!rd)
		واپس تہی;

	اگر (init_rootdomain(rd) != 0) {
		kfree(rd);
		واپس تہی;
	}

	واپس rd;
}

ساکن خالی free_sched_groups(ساخت sched_گروہ *sg, صحیح free_sgc)
{
	ساخت sched_گروہ *tmp, *پہلا;

	اگر (!sg)
		واپس;

	پہلا = sg;
	کرو {
		tmp = sg->اگلا;

		اگر (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		اگر (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} جب (sg != پہلا);
}

ساکن خالی free_sched_domain_shared(ساخت sched_domain_shared *sds)
{
	اگر (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

ساکن خالی destroy_sched_domain(ساخت sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#اگر_ختم
	kfree(sd);
}

ساکن خالی destroy_sched_domains_rcu(ساخت rcu_سر *rcu)
{
	ساخت sched_domain *sd = container_کا(rcu, ساخت sched_domain, rcu);

	جب (sd) {
		ساخت sched_domain *والد = sd->والد;
		destroy_sched_domain(sd);
		sd = والد;
	}
}

ساکن خالی destroy_sched_domains(ساخت sched_domain *sd)
{
	اگر (sd)
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
DEFINE_فی_CPU(ساخت sched_domain __rcu *, sd_llc);
DEFINE_فی_CPU(صحیح, sd_llc_سائز);
DEFINE_فی_CPU(صحیح, sd_llc_id) = -1;
DEFINE_فی_CPU(صحیح, sd_share_id);
DEFINE_فی_CPU(ساخت sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_فی_CPU(ساخت sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_فی_CPU(ساخت sched_domain __rcu *, sd_numa);
DEFINE_فی_CPU(ساخت sched_domain __rcu *, sd_asym_packing);
DEFINE_فی_CPU(ساخت sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_کلید_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_کلید_FALSE(sched_cluster_فعال);

ساکن خالی update_چوٹی_cache_domain(صحیح cpu)
{
	ساخت sched_domain_shared *sds = تہی;
	ساخت sched_domain *sd;
	صحیح id = cpu;
	صحیح سائز = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	اگر (sd) {
		id = cpumask_پہلا(sched_domain_وسعت(sd));
		سائز = cpumask_وزن(sched_domain_وسعت(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_تفویض_اشارہ_گر(فی_cpu(sd_llc, cpu), sd);
	فی_cpu(sd_llc_سائز, cpu) = سائز;
	rcu_تفویض_اشارہ_گر(فی_cpu(sd_llc_shared, cpu), sds);

	sd = سب_سے_نیچا_flag_domain(cpu, SD_CLUSTER);
	اگر (sd)
		id = cpumask_پہلا(sched_domain_وسعت(sd));

	/*
	 * This assignment should be placed after the sd_llc_id as
	 * we want this id equals to cluster id on cluster machines
	 * but equals to LLC id on non-Cluster machines.
	 */
	فی_cpu(sd_share_id, cpu) = id;

	sd = سب_سے_نیچا_flag_domain(cpu, SD_NUMA);
	rcu_تفویض_اشارہ_گر(فی_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_تفویض_اشارہ_گر(فی_cpu(sd_asym_packing, cpu), sd);

	sd = سب_سے_نیچا_flag_domain(cpu, SD_ASYM_CPUCAPACITY_بھرا);
	/*
	 * The shared object is attached to sd_asym_cpucapacity only when the
	 * asym domain is non-overlapping (i.e., not built from SD_NUMA).
	 * On overlapping (NUMA) asym domains we fall back to letting the
	 * SD_SHARE_LLC path own the shared object, so sd->shared may be NULL
	 * here.
	 */
	اگر (sd && sd->shared)
		sds = sd->shared;

	rcu_تفویض_اشارہ_گر(فی_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_تفویض_اشارہ_گر(فی_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
ساکن خالی
cpu_attach_domain(ساخت sched_domain *sd, ساخت جڑ_domain *rd, صحیح cpu)
{
	ساخت rq *rq = cpu_rq(cpu);
	ساخت sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	برائے (tmp = sd; tmp; ) {
		ساخت sched_domain *والد = tmp->والد;
		اگر (!والد)
			توڑو;

		اگر (sd_والد_degenerate(tmp, والد)) {
			tmp->والد = والد->والد;

			/* Pick reference to parent->shared. */
			اگر (والد->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = والد->shared;
				والد->shared = تہی;
			}

			اگر (والد->والد) {
				والد->والد->بچہ = tmp;
				والد->والد->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			اگر (والد->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(والد);
		} ورنہ
			tmp = tmp->والد;
	}

	اگر (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->والد;

		اگر (sd) {
			ساخت sched_گروہ *sg = sd->groups;

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_بائٹیں = tmp->llc_بائٹیں;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = تہی;
			tmp->llc_max = 0;
			tmp->llc_بائٹیں = 0;
#اگر_ختم
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			کرو {
				sg->flags = 0;
			} جب (sg != sd->groups);

			sd->بچہ = تہی;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_جڑ(rq, rd);
	tmp = rq->sd;
	rcu_تفویض_اشارہ_گر(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_چوٹی_cache_domain(cpu);
}

ساخت s_ڈیٹا {
	ساخت sched_domain_shared * __percpu *sds;
	ساخت sched_domain * __percpu *sd;
	ساخت جڑ_domain	*rd;
};

شمار s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_کلید_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_کلید_FALSE(sched_cache_فعال);
/* user wants cache aware scheduling [0 or 1] */
صحیح sysctl_sched_cache_user = 1;

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
ساکن غیرعلامتی طویل get_effective_llc_بائٹیں(صحیح cpu,
					     ساخت sched_domain *sd)
{
	ساخت cacheinfo *ci;
	غیرعلامتی صحیح hw_وزن;

	ci = get_cpu_cacheinfo_llc(cpu);
	اگر (!ci)
		واپس 0;

	hw_وزن = cpumask_وزن(&ci->shared_cpu_map);
	اگر (!hw_وزن)
		واپس 0;

	واپس div_u64((u64)ci->سائز * sd->وسعت_وزن, hw_وزن);
}

ساکن bool alloc_sd_llc(ثابت ساخت cpumask *cpu_map,
			 ساخت s_ڈیٹا *d)
{
	ساخت sched_domain *sd, *چوٹی_llc, *والد;
	غیرعلامتی صحیح *p;
	صحیح i;

	for_each_cpu(i, cpu_map) {
		sd = *فی_cpu_ptr(d->sd, i);
		اگر (!sd)
			جاؤ err;

		p = kcalloc_گرہ(max_lid + 1, حجم(غیرعلامتی صحیح),
				 GFP_KERNEL, cpu_مقصد_گرہ(i));
		اگر (!p)
			جاؤ err;

		چوٹی_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		جب ((والد = rcu_dereference_protected(چوٹی_llc->والد, true)) &&
		       (والد->flags & SD_SHARE_LLC))
			چوٹی_llc = والد;

		اگر (چوٹی_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_بائٹیں = get_effective_llc_بائٹیں(i, چوٹی_llc);
		} ورنہ {
			/* avoid memory leak */
			kfree(p);
		}
	}

	واپس true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *فی_cpu_ptr(d->sd, i);
		اگر (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = تہی;
			sd->llc_max = 0;
			sd->llc_بائٹیں = 0;
		}
	}

	واپس false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
ساکن خالی sched_cache_فعال_سیٹ_2(خالی)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	اگر (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_فعال);
		اگر (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		واپس;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	اگر (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_فعال);
		اگر (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} ورنہ {
		static_branch_disable_cpuslocked(&sched_cache_فعال);
		اگر (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
خالی sched_cache_فعال_سیٹ(خالی)
{
	cpus_read_تالا();
	sched_domains_mutex_تالا();
	sched_cache_فعال_سیٹ_2();
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
خالی sched_update_llc_بائٹیں(ثابت ساخت cpumask *cpus)
{
	ساخت sched_domain *sd, *sdp;
	غیرعلامتی صحیح i;

	sched_domains_mutex_تالا();

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
		sdp = rcu_dereference_sched_domain(فی_cpu(sd_llc, i));
		اگر (!sdp)
			جاری;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		اگر (sd)
			sd->llc_بائٹیں = get_effective_llc_بائٹیں(i, sdp);
	}

	sched_domains_mutex_unlock();
}

ساکن خالی sched_cache_سیٹ(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	اگر (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	ورنہ
		static_branch_disable_cpuslocked(&sched_cache_present);

	sched_cache_فعال_سیٹ_2();
}
#ورنہ
ساکن bool alloc_sd_llc(ثابت ساخت cpumask *cpu_map,
			 ساخت s_ڈیٹا *d)
{
	واپس false;
}
ساکن اندرونی خالی sched_cache_سیٹ(bool has_multi_llcs) { }
#اگر_ختم

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
ساکن bool sd_اندر_multi_llcs(ساخت sched_domain *sd)
{
	ساخت sched_domain *sdp = sd->والد;

	/* it does not make sense to aggregate to 1 CPU */
	اگر (sd->وسعت_وزن == 1)
		واپس false;

	جب (sdp && sdp->وسعت_وزن == sd->وسعت_وزن)
		sdp = sdp->والد;

	واپس !!sdp;
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
صحیح گروہ_balance_cpu(ساخت sched_گروہ *sg)
{
	واپس cpumask_پہلا(گروہ_balance_نقاب(sg));
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
ساکن خالی
build_balance_نقاب(ساخت sched_domain *sd, ساخت sched_گروہ *sg, ساخت cpumask *نقاب)
{
	ثابت ساخت cpumask *sg_وسعت = sched_گروہ_وسعت(sg);
	ساخت sd_ڈیٹا *sdd = sd->نجی;
	ساخت sched_domain *sibling;
	صحیح i;

	cpumask_clear(نقاب);

	for_each_cpu(i, sg_وسعت) {
		sibling = *فی_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		اگر (!sibling->بچہ)
			جاری;

		/* If we would not end up here, we can't continue from here */
		اگر (!cpumask_equal(sg_وسعت, sched_domain_وسعت(sibling->بچہ)))
			جاری;

		cpumask_سیٹ_cpu(i, نقاب);
	}

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_خالی_خانے(نقاب));
}

/*
 * XXX: This creates per-node group entries; since the load-balancer will
 * immediately access remote memory to construct this group's load-balance
 * statistics having the groups node local is of dubious benefit.
 */
ساکن ساخت sched_گروہ *
build_گروہ_from_بچہ_sched_domain(ساخت sched_domain *sd, صحیح cpu)
{
	ساخت sched_گروہ *sg;
	ساخت cpumask *sg_وسعت;

	sg = kzalloc_گرہ(حجم(ساخت sched_گروہ) + cpumask_سائز(),
			GFP_KERNEL, cpu_مقصد_گرہ(cpu));

	اگر (!sg)
		واپس تہی;

	sg_وسعت = sched_گروہ_وسعت(sg);
	اگر (sd->بچہ) {
		cpumask_copy(sg_وسعت, sched_domain_وسعت(sd->بچہ));
		sg->flags = sd->بچہ->flags;
	} ورنہ {
		cpumask_copy(sg_وسعت, sched_domain_وسعت(sd));
	}

	atomic_inc(&sg->ref);
	واپس sg;
}

ساکن خالی init_overlap_sched_گروہ(ساخت sched_domain *sd,
				     ساخت sched_گروہ *sg)
{
	ساخت cpumask *نقاب = sched_domains_tmpmask2;
	ساخت sd_ڈیٹا *sdd = sd->نجی;
	ساخت cpumask *sg_وسعت;
	صحیح cpu;

	build_balance_نقاب(sd, sg, نقاب);
	cpu = cpumask_پہلا(نقاب);

	sg->sgc = *فی_cpu_ptr(sdd->sgc, cpu);
	اگر (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(گروہ_balance_نقاب(sg), نقاب);
	ورنہ
		WARN_ON_ONCE(!cpumask_equal(گروہ_balance_نقاب(sg), نقاب));

	/*
	 * Initialize sgc->capacity such that even if we mess up the
	 * domains and no possible iteration will get us here, we won't
	 * die on a /0 trap.
	 */
	sg_وسعت = sched_گروہ_وسعت(sg);
	sg->sgc->استعداد = SCHED_استعداد_SCALE * cpumask_وزن(sg_وسعت);
	sg->sgc->min_استعداد = SCHED_استعداد_SCALE;
	sg->sgc->max_استعداد = SCHED_استعداد_SCALE;
}

ساکن ساخت sched_domain *
ڈھونڈو_descended_sibling(ساخت sched_domain *sd, ساخت sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	جب (sibling->بچہ &&
	       !cpumask_subset(sched_domain_وسعت(sibling->بچہ),
			       sched_domain_وسعت(sd)))
		sibling = sibling->بچہ;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	جب (sibling->بچہ &&
	       cpumask_equal(sched_domain_وسعت(sibling->بچہ),
			     sched_domain_وسعت(sibling)))
		sibling = sibling->بچہ;

	واپس sibling;
}

ساکن صحیح
build_overlap_sched_groups(ساخت sched_domain *sd, صحیح cpu)
{
	ساخت sched_گروہ *پہلا = تہی, *آخری = تہی, *sg;
	ثابت ساخت cpumask *وسعت = sched_domain_وسعت(sd);
	ساخت cpumask *covered = sched_domains_tmpmask;
	ساخت sd_ڈیٹا *sdd = sd->نجی;
	ساخت sched_domain *sibling;
	صحیح i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, وسعت, cpu) {
		ساخت cpumask *sg_وسعت;

		اگر (cpumask_test_cpu(i, covered))
			جاری;

		sibling = *فی_cpu_ptr(sdd->sd, i);

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
		اگر (!cpumask_test_cpu(i, sched_domain_وسعت(sibling)))
			جاری;

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
		اگر (sibling->بچہ &&
		    !cpumask_subset(sched_domain_وسعت(sibling->بچہ), وسعت))
			sibling = ڈھونڈو_descended_sibling(sd, sibling);

		sg = build_گروہ_from_بچہ_sched_domain(sibling, cpu);
		اگر (!sg)
			جاؤ fail;

		sg_وسعت = sched_گروہ_وسعت(sg);
		cpumask_or(covered, covered, sg_وسعت);

		init_overlap_sched_گروہ(sibling, sg);

		اگر (!پہلا)
			پہلا = sg;
		اگر (آخری)
			آخری->اگلا = sg;
		آخری = sg;
		آخری->اگلا = پہلا;
	}
	sd->groups = پہلا;

	واپس 0;

fail:
	free_sched_groups(پہلا, 0);

	واپس -ENOMEM;
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

ساکن ساخت sched_گروہ *get_گروہ(صحیح cpu, ساخت sd_ڈیٹا *sdd)
{
	ساخت sched_domain *sd = *فی_cpu_ptr(sdd->sd, cpu);
	ساخت sched_domain *بچہ = sd->بچہ;
	ساخت sched_گروہ *sg;
	bool already_دیکھا;

	اگر (بچہ)
		cpu = cpumask_پہلا(sched_domain_وسعت(بچہ));

	sg = *فی_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *فی_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_دیکھا = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_دیکھا != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	اگر (already_دیکھا)
		واپس sg;

	اگر (بچہ) {
		cpumask_copy(sched_گروہ_وسعت(sg), sched_domain_وسعت(بچہ));
		cpumask_copy(گروہ_balance_نقاب(sg), sched_گروہ_وسعت(sg));
		sg->flags = بچہ->flags;
	} ورنہ {
		cpumask_سیٹ_cpu(cpu, sched_گروہ_وسعت(sg));
		cpumask_سیٹ_cpu(cpu, گروہ_balance_نقاب(sg));
	}

	sg->sgc->استعداد = SCHED_استعداد_SCALE * cpumask_وزن(sched_گروہ_وسعت(sg));
	sg->sgc->min_استعداد = SCHED_استعداد_SCALE;
	sg->sgc->max_استعداد = SCHED_استعداد_SCALE;

	واپس sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
ساکن صحیح
build_sched_groups(ساخت sched_domain *sd, صحیح cpu)
{
	ساخت sched_گروہ *پہلا = تہی, *آخری = تہی;
	ساخت sd_ڈیٹا *sdd = sd->نجی;
	ثابت ساخت cpumask *وسعت = sched_domain_وسعت(sd);
	ساخت cpumask *covered;
	صحیح i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, وسعت, cpu) {
		ساخت sched_گروہ *sg;

		اگر (cpumask_test_cpu(i, covered))
			جاری;

		sg = get_گروہ(i, sdd);

		cpumask_or(covered, covered, sched_گروہ_وسعت(sg));

		اگر (!پہلا)
			پہلا = sg;
		اگر (آخری)
			آخری->اگلا = sg;
		آخری = sg;
	}
	آخری->اگلا = پہلا;
	sd->groups = پہلا;

	واپس 0;
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
ساکن خالی init_sched_groups_استعداد(صحیح cpu, ساخت sched_domain *sd)
{
	ساخت sched_گروہ *sg = sd->groups;
	ساخت cpumask *نقاب = sched_domains_tmpmask2;

	WARN_ON(!sg);

	کرو {
		صحیح cpu, cores = 0, max_cpu = -1;

		sg->گروہ_وزن = cpumask_وزن(sched_گروہ_وسعت(sg));

		cpumask_copy(نقاب, sched_گروہ_وسعت(sg));
		for_each_cpu(cpu, نقاب) {
			cores++;
			cpumask_andnot(نقاب, نقاب, cpu_smt_نقاب(cpu));
		}
		sg->cores = cores;

		اگر (!(sd->flags & SD_ASYM_PACKING))
			جاؤ اگلا;

		for_each_cpu(cpu, sched_گروہ_وسعت(sg)) {
			اگر (max_cpu < 0)
				max_cpu = cpu;
			ورنہ اگر (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

اگلا:
		sg = sg->اگلا;
	} جب (sg != sd->groups);

	اگر (cpu != گروہ_balance_cpu(sg))
		واپس;

	update_گروہ_استعداد(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
خالی sched_update_asym_prefer_cpu(صحیح cpu, صحیح old_prio, صحیح new_prio)
{
	صحیح asym_prefer_cpu = cpu;
	ساخت sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		ساخت sched_گروہ *sg;
		صحیح گروہ_cpu;

		اگر (!(sd->flags & SD_ASYM_PACKING))
			جاری;

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
		اگر (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			اگر (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				واپس;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			جاری;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		اگر (new_prio >= old_prio)
			جاری;

		for_each_cpu(گروہ_cpu, sched_گروہ_وسعت(sg)) {
			اگر (sched_asym_prefer(گروہ_cpu, asym_prefer_cpu))
				asym_prefer_cpu = گروہ_cpu;
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
ساکن اندرونی صحیح
asym_cpu_استعداد_classify(ثابت ساخت cpumask *sd_وسعت,
			   ثابت ساخت cpumask *cpu_map)
{
	ساخت asym_cap_ڈیٹا *اندراج_2;
	صحیح گنتی = 0, ناکامی = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_اندراج(اندراج_2, &asym_cap_list, link) {
		اگر (cpumask_intersects(sd_وسعت, cpu_استعداد_وسعت(اندراج_2)))
			++گنتی;
		ورنہ اگر (cpumask_intersects(cpu_map, cpu_استعداد_وسعت(اندراج_2)))
			++ناکامی;
	}

	WARN_ON_ONCE(!گنتی && !list_خالی_خانے(&asym_cap_list));

	/* No asymmetry detected */
	اگر (گنتی < 2)
		واپس 0;
	/* Some of the available CPU capacity values have not been detected */
	اگر (ناکامی)
		واپس SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	واپس SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_بھرا;

}

ساکن خالی free_asym_cap_اندراج(ساخت rcu_سر *سر)
{
	ساخت asym_cap_ڈیٹا *اندراج_2 = container_کا(سر, ساخت asym_cap_ڈیٹا, rcu);
	kfree(اندراج_2);
}

ساکن اندرونی خالی asym_cpu_استعداد_update_ڈیٹا(صحیح cpu)
{
	غیرعلامتی طویل استعداد = arch_scale_cpu_استعداد(cpu);
	ساخت asym_cap_ڈیٹا *داخل_کرو_اندراج = تہی;
	ساخت asym_cap_ڈیٹا *اندراج_2;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_اندراج(اندراج_2, &asym_cap_list, link) {
		اگر (استعداد == اندراج_2->استعداد)
			جاؤ مکمل;
		ورنہ اگر (!داخل_کرو_اندراج && استعداد > اندراج_2->استعداد)
			داخل_کرو_اندراج = list_پچھلا_اندراج(اندراج_2, link);
	}

	اندراج_2 = kzalloc(حجم(*اندراج_2) + cpumask_سائز(), GFP_KERNEL);
	اگر (WARN_ONCE(!اندراج_2, "Failed to allocate memory for asymmetry data\n"))
		واپس;
	اندراج_2->استعداد = استعداد;

	/* If NULL then the new capacity is the smallest, add last. */
	اگر (!داخل_کرو_اندراج)
		list_جوڑو_دم_rcu(&اندراج_2->link, &asym_cap_list);
	ورنہ
		list_جوڑو_rcu(&اندراج_2->link, &داخل_کرو_اندراج->link);
مکمل:
	__cpumask_set_cpu(cpu, cpu_استعداد_وسعت(اندراج_2));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
ساکن خالی asym_cpu_استعداد_جائزہ(خالی)
{
	ساخت asym_cap_ڈیٹا *اندراج_2, *اگلا;
	صحیح cpu;

	list_for_each_اندراج(اندراج_2, &asym_cap_list, link)
		cpumask_clear(cpu_استعداد_وسعت(اندراج_2));

	for_each_cpu_and(cpu, cpu_possible_نقاب, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_استعداد_update_ڈیٹا(cpu);

	list_for_each_اندراج_محفوظ(اندراج_2, اگلا, &asym_cap_list, link) {
		اگر (cpumask_خالی_خانے(cpu_استعداد_وسعت(اندراج_2))) {
			list_del_rcu(&اندراج_2->link);
			call_rcu(&اندراج_2->rcu, free_asym_cap_اندراج);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	اگر (list_is_singular(&asym_cap_list)) {
		اندراج_2 = list_پہلا_اندراج(&asym_cap_list, typeof(*اندراج_2), link);
		list_del_rcu(&اندراج_2->link);
		call_rcu(&اندراج_2->rcu, free_asym_cap_اندراج);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

ساکن صحیح default_relax_domain_level = -1;
صحیح sched_domain_level_max;

ساکن صحیح __init تیاری_relax_domain_level(حرف *str)
{
	اگر (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	واپس 1;
}
__setup("relax_domain_level=", تیاری_relax_domain_level);

ساکن خالی سیٹ_domain_attribute(ساخت sched_domain *sd,
				 ساخت sched_domain_attr *attr)
{
	صحیح request;

	اگر (!attr || attr->relax_domain_level < 0) {
		اگر (default_relax_domain_level < 0)
			واپس;
		request = default_relax_domain_level;
	} ورنہ
		request = attr->relax_domain_level;

	اگر (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

ساکن خالی __sdt_free(ثابت ساخت cpumask *cpu_map);
ساکن صحیح __sdt_alloc(ثابت ساخت cpumask *cpu_map);

ساکن خالی __sds_free(ساخت s_ڈیٹا *d, ثابت ساخت cpumask *cpu_map);
ساکن صحیح __sds_alloc(ساخت s_ڈیٹا *d, ثابت ساخت cpumask *cpu_map);

ساکن خالی __free_domain_allocs(ساخت s_ڈیٹا *d, شمار s_alloc what,
				 ثابت ساخت cpumask *cpu_map)
{
	انتخاب (what) {
	حالت sa_rootdomain:
		اگر (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	حالت sa_sd:
		free_percpu(d->sd);
		fallthrough;
	حالت sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	حالت sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	حالت sa_none:
		توڑو;
	}
}

ساکن شمار s_alloc
__visit_domain_allocation_hell(ساخت s_ڈیٹا *d, ثابت ساخت cpumask *cpu_map)
{
	memset(d, 0, حجم(*d));

	اگر (__sdt_alloc(cpu_map))
		واپس sa_sd_storage;
	اگر (__sds_alloc(d, cpu_map))
		واپس sa_sd_shared;
	d->sd = alloc_percpu(ساخت sched_domain *);
	اگر (!d->sd)
		واپس sa_sd_shared;
	d->rd = alloc_rootdomain();
	اگر (!d->rd)
		واپس sa_sd;

	واپس sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
ساکن خالی claim_allocations(صحیح cpu, ساخت s_ڈیٹا *d)
{
	ساخت sched_domain *sd;

	اگر (atomic_read(&(*فی_cpu_ptr(d->sds, cpu))->ref))
		*فی_cpu_ptr(d->sds, cpu) = تہی;

	برائے (sd = *فی_cpu_ptr(d->sd, cpu); sd; sd = sd->والد) {
		ساخت sd_ڈیٹا *sdd = sd->نجی;

		WARN_ON_ONCE(*فی_cpu_ptr(sdd->sd, cpu) != sd);
		*فی_cpu_ptr(sdd->sd, cpu) = تہی;

		اگر (atomic_read(&(*فی_cpu_ptr(sdd->sg, cpu))->ref))
			*فی_cpu_ptr(sdd->sg, cpu) = تہی;

		اگر (atomic_read(&(*فی_cpu_ptr(sdd->sgc, cpu))->ref))
			*فی_cpu_ptr(sdd->sgc, cpu) = تہی;
	}
}

#اگر_تعریف_شدہ CONFIG_NUMA
شمار numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
ساکن صحیح			sched_domains_numa_سطحیں;
ساکن صحیح			sched_numa_گرہ_سطحیں;

صحیح				sched_max_numa_فاصلہ;
ساکن صحیح			*sched_domains_numa_فاصلہ;
ساکن صحیح			*sched_numa_گرہ_فاصلہ;
ساکن ساخت cpumask		***sched_domains_numa_masks;
#اگر_ختم /* CONFIG_NUMA */

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
#تعریف_کرو TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

ساکن ساخت sched_domain *
sd_init(ساخت sched_domain_topology_level *tl,
	ثابت ساخت cpumask *cpu_map,
	ساخت sched_domain *بچہ, صحیح cpu)
{
	ساخت sd_ڈیٹا *sdd = &tl->ڈیٹا;
	ساخت sched_domain *sd = *فی_cpu_ptr(sdd->sd, cpu);
	صحیح sd_id, sd_وزن, sd_flags = 0;
	ساخت cpumask *sd_وسعت;
	u64 now = sched_clock();

	sd_وسعت = sched_domain_وسعت(sd);
	cpumask_and(sd_وسعت, cpu_map, tl->نقاب(tl, cpu));
	sd_وزن = cpumask_وزن(sd_وسعت);
	sd_id = cpumask_پہلا(sd_وسعت);

	اگر (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	اگر (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#اگر_تعریف_شدہ CONFIG_SCHED_SMT
	اگر (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#اگر_ختم
	sd_flags |= asym_cpu_استعداد_classify(sd_وسعت, cpu_map);

	*sd = (ساخت sched_domain){
		.min_interval		= sd_وزن,
		.max_interval		= 2*sd_وزن,
		.busy_ضربی_عدد		= 16,
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

		.آخری_balance		= jiffies,
		.balance_interval	= sd_وزن,

		/* 50% success rate */
		.newidle_call		= 512,
		.newidle_success	= 256,
		.newidle_تناسب		= 512,
		.newidle_stamp		= now,

		.max_newidle_lb_لاگت	= 0,
		.آخری_decay_max_lb_لاگت	= jiffies,
		.بچہ			= بچہ,
		.name			= tl->name,
	};

	WARN_ONCE((sd->flags & (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY)) ==
		  (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY),
		  "CPU capacity asymmetry not supported on SMT\n");

	/*
	 * Convert topological properties into behaviour.
	 */
	اگر (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} ورنہ اگر (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#اگر_تعریف_شدہ CONFIG_NUMA
	} ورنہ اگر (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		اگر (sched_domains_numa_فاصلہ[tl->numa_level] > گرہ_reclaim_فاصلہ) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#اگر_ختم /* CONFIG_NUMA */
	} ورنہ {
		sd->cache_nice_tries = 1;
	}

	sd->نجی = sdd;

	واپس sd;
}

#اگر_تعریف_شدہ CONFIG_SCHED_SMT
صحیح cpu_smt_flags(خالی)
{
	واپس SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ثابت ساخت cpumask *tl_smt_نقاب(ساخت sched_domain_topology_level *tl, صحیح cpu)
{
	واپس cpu_smt_نقاب(cpu);
}
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_CLUSTER
صحیح cpu_cluster_flags(خالی)
{
	واپس SD_CLUSTER | SD_SHARE_LLC;
}

ثابت ساخت cpumask *tl_cls_نقاب(ساخت sched_domain_topology_level *tl, صحیح cpu)
{
	واپس cpu_clustergroup_نقاب(cpu);
}
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_MC
صحیح cpu_core_flags(خالی)
{
	واپس SD_SHARE_LLC;
}

ثابت ساخت cpumask *tl_mc_نقاب(ساخت sched_domain_topology_level *tl, صحیح cpu)
{
	واپس cpu_coregroup_نقاب(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_نقاب
#تعریف_کرو arch_llc_نقاب(cpu) cpu_coregroup_نقاب(cpu)
# endif

#ورنہ
#تعریف_کرو arch_llc_نقاب(cpu) cpumask_کا(cpu)
#اگر_ختم

#تعریف_کرو llc_نقاب(cpu) arch_llc_نقاب(cpu)

ثابت ساخت cpumask *tl_pkg_نقاب(ساخت sched_domain_topology_level *tl, صحیح cpu)
{
	واپس cpu_گرہ_نقاب(cpu);
}

/*
 * Topology list, bottom-up.
 */
ساکن ساخت sched_domain_topology_level default_topology[] = {
#اگر_تعریف_شدہ CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_نقاب, cpu_smt_flags, SMT),
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_نقاب, cpu_cluster_flags, CLS),
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_نقاب, cpu_core_flags, MC),
#اگر_ختم
	SDTL_INIT(tl_pkg_نقاب, تہی, PKG),
	{ تہی, },
};

ساکن ساخت sched_domain_topology_level *sched_domain_topology =
	default_topology;
ساکن ساخت sched_domain_topology_level *sched_domain_topology_saved;

#تعریف_کرو for_each_sd_topology(tl)			\
	برائے (tl = sched_domain_topology; tl->نقاب; tl++)

خالی __init سیٹ_sched_topology(ساخت sched_domain_topology_level *tl)
{
	اگر (WARN_ON_ONCE(sched_smp_initialized))
		واپس;

	sched_domain_topology = tl;
	sched_domain_topology_saved = تہی;
}

#اگر_تعریف_شدہ CONFIG_NUMA
ساکن صحیح cpu_numa_flags(خالی)
{
	واپس SD_NUMA;
}

ساکن ثابت ساخت cpumask *sd_numa_نقاب(ساخت sched_domain_topology_level *tl, صحیح cpu)
{
	واپس sched_domains_numa_masks[tl->numa_level][cpu_مقصد_گرہ(cpu)];
}

ساکن خالی sched_numa_warn(ثابت حرف *str)
{
	ساکن صحیح مکمل = false;
	صحیح i,j;

	اگر (مکمل)
		واپس;

	مکمل = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	برائے (i = 0; i < nr_گرہ_ids; i++) {
		printk(KERN_WARNING "  ");
		برائے (j = 0; j < nr_گرہ_ids; j++) {
			اگر (!گرہ_state(i, N_CPU) || !گرہ_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", گرہ_فاصلہ(i,j));
			ورنہ
				printk(KERN_CONT " %02d  ", گرہ_فاصلہ(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool ڈھونڈو_numa_فاصلہ(صحیح فاصلہ)
{
	bool ملا = false;
	صحیح i, *distances;

	اگر (فاصلہ == گرہ_فاصلہ(0, 0))
		واپس true;

	rcu_read_تالا();
	distances = rcu_dereference(sched_numa_گرہ_فاصلہ);
	اگر (!distances)
		جاؤ unlock;
	برائے (i = 0; i < sched_numa_گرہ_سطحیں; i++) {
		اگر (distances[i] == فاصلہ) {
			ملا = true;
			توڑو;
		}
	}
unlock:
	rcu_read_unlock();

	واپس ملا;
}

#تعریف_کرو for_each_cpu_گرہ_but(n, nbut)		\
	for_each_گرہ_state(n, N_CPU)		\
		اگر (n == nbut)			\
			جاری;		\
		ورنہ

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
ساکن خالی init_numa_topology_type(صحیح offline_گرہ)
{
	صحیح a, b, c, n;

	n = sched_max_numa_فاصلہ;

	اگر (sched_domains_numa_سطحیں <= 2) {
		sched_numa_topology_type = NUMA_براہ_راست;
		واپس;
	}

	for_each_cpu_گرہ_but(a, offline_گرہ) {
		for_each_cpu_گرہ_but(b, offline_گرہ) {
			/* Find two nodes furthest removed from each other. */
			اگر (گرہ_فاصلہ(a, b) < n)
				جاری;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_گرہ_but(c, offline_گرہ) {
				اگر (گرہ_فاصلہ(a, c) < n &&
				    گرہ_فاصلہ(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					واپس;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			واپس;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_براہ_راست;
}


#تعریف_کرو NR_فاصلہ_اقدار (1 << فاصلہ_بٹ)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
ساکن صحیح numa_گرہ_dist(صحیح i, صحیح j)
{
	واپس گرہ_فاصلہ(i, j);
}

صحیح arch_sched_گرہ_فاصلہ(صحیح from, صحیح مقصد)
			     __weak __alias(numa_گرہ_dist);

ساکن bool modified_sched_گرہ_فاصلہ(خالی)
{
	واپس numa_گرہ_dist != arch_sched_گرہ_فاصلہ;
}

ساکن صحیح sched_ریکارڈ_numa_dist(صحیح offline_گرہ, صحیح (*n_dist)(صحیح, صحیح),
				  صحیح **dist, صحیح *سطحیں)
{
	غیرعلامتی طویل *فاصلہ_map __free(bitmap) = تہی;
	صحیح nr_سطحیں = 0;
	صحیح i, j;
	صحیح *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	فاصلہ_map = bitmap_alloc(NR_فاصلہ_اقدار, GFP_KERNEL);
	اگر (!فاصلہ_map)
		واپس -ENOMEM;

	bitmap_zero(فاصلہ_map, NR_فاصلہ_اقدار);
	for_each_cpu_گرہ_but(i, offline_گرہ) {
		for_each_cpu_گرہ_but(j, offline_گرہ) {
			صحیح فاصلہ = n_dist(i, j);

			اگر (فاصلہ < LOCAL_فاصلہ || فاصلہ >= NR_فاصلہ_اقدار) {
				sched_numa_warn("Invalid distance value range");
				واپس -EINVAL;
			}

			bitmap_سیٹ(فاصلہ_map, فاصلہ, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_سطحیں = bitmap_وزن(فاصلہ_map, NR_فاصلہ_اقدار);

	distances = kzalloc_objs(صحیح, nr_سطحیں);
	اگر (!distances)
		واپس -ENOMEM;

	برائے (i = 0, j = 0; i < nr_سطحیں; i++, j++) {
		j = ڈھونڈو_اگلا_ثنائی_ہندسہ(فاصلہ_map, NR_فاصلہ_اقدار, j);
		distances[i] = j;
	}
	*dist = distances;
	*سطحیں = nr_سطحیں;

	واپس 0;
}

خالی sched_init_numa(صحیح offline_گرہ)
{
	ساخت sched_domain_topology_level *tl;
	صحیح nr_سطحیں, nr_گرہ_سطحیں;
	صحیح i, j;
	صحیح *distances, *domain_distances;
	ساخت cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	اگر (sched_ریکارڈ_numa_dist(offline_گرہ, numa_گرہ_dist, &distances,
				   &nr_گرہ_سطحیں))
		واپس;

	/* Record modified NUMA distances for building sched domains */
	اگر (modified_sched_گرہ_فاصلہ()) {
		اگر (sched_ریکارڈ_numa_dist(offline_گرہ, arch_sched_گرہ_فاصلہ,
					   &domain_distances, &nr_سطحیں)) {
			kfree(distances);
			واپس;
		}
	} ورنہ {
		domain_distances = distances;
		nr_سطحیں = nr_گرہ_سطحیں;
	}
	rcu_تفویض_اشارہ_گر(sched_numa_گرہ_فاصلہ, distances);
	WRITE_ONCE(sched_max_numa_فاصلہ, distances[nr_گرہ_سطحیں - 1]);
	WRITE_ONCE(sched_numa_گرہ_سطحیں, nr_گرہ_سطحیں);

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
	rcu_تفویض_اشارہ_گر(sched_domains_numa_فاصلہ, domain_distances);

	sched_domains_numa_سطحیں = 0;

	masks = kzalloc(حجم(خالی *) * nr_سطحیں, GFP_KERNEL);
	اگر (!masks)
		واپس;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	برائے (i = 0; i < nr_سطحیں; i++) {
		masks[i] = kzalloc(nr_گرہ_ids * حجم(خالی *), GFP_KERNEL);
		اگر (!masks[i])
			واپس;

		for_each_cpu_گرہ_but(j, offline_گرہ) {
			ساخت cpumask *نقاب = kzalloc(cpumask_سائز(), GFP_KERNEL);
			صحیح k;

			اگر (!نقاب)
				واپس;

			masks[i][j] = نقاب;

			for_each_cpu_گرہ_but(k, offline_گرہ) {
				اگر (sched_debug() &&
				    (arch_sched_گرہ_فاصلہ(j, k) !=
				     arch_sched_گرہ_فاصلہ(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				اگر (arch_sched_گرہ_فاصلہ(j, k) >
				    sched_domains_numa_فاصلہ[i])
					جاری;

				cpumask_or(نقاب, نقاب, cpumask_کا_گرہ(k));
			}
		}
	}
	rcu_تفویض_اشارہ_گر(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	برائے (i = 0; sched_domain_topology[i].نقاب; i++);

	tl = kzalloc((i + nr_سطحیں + 1) *
			حجم(ساخت sched_domain_topology_level), GFP_KERNEL);
	اگر (!tl)
		واپس;

	/*
	 * Copy the default topology bits..
	 */
	برائے (i = 0; sched_domain_topology[i].نقاب; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_نقاب, تہی, گرہ_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	برائے (j = 1; j < nr_سطحیں; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_نقاب, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_سطحیں = nr_سطحیں;

	init_numa_topology_type(offline_گرہ);
}


ساکن خالی sched_reset_numa(خالی)
{
	صحیح nr_سطحیں, *distances, *dom_distances = تہی;
	ساخت cpumask ***masks;

	nr_سطحیں = sched_domains_numa_سطحیں;
	sched_numa_گرہ_سطحیں = 0;
	sched_domains_numa_سطحیں = 0;
	sched_max_numa_فاصلہ = 0;
	sched_numa_topology_type = NUMA_براہ_راست;
	distances = sched_numa_گرہ_فاصلہ;
	اگر (sched_numa_گرہ_فاصلہ != sched_domains_numa_فاصلہ)
		dom_distances = sched_domains_numa_فاصلہ;
	rcu_تفویض_اشارہ_گر(sched_numa_گرہ_فاصلہ, تہی);
	rcu_تفویض_اشارہ_گر(sched_domains_numa_فاصلہ, تہی);
	masks = sched_domains_numa_masks;
	rcu_تفویض_اشارہ_گر(sched_domains_numa_masks, تہی);
	اگر (distances || masks) {
		صحیح i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		برائے (i = 0; i < nr_سطحیں && masks; i++) {
			اگر (!masks[i])
				جاری;
			for_each_گرہ(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	اگر (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = تہی;
	}
}

/*
 * Call with hotplug lock held
 */
خالی sched_update_numa(صحیح cpu, bool online)
{
	صحیح گرہ;

	گرہ = cpu_مقصد_گرہ(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	اگر (cpumask_وزن(cpumask_کا_گرہ(گرہ)) != 1)
		واپس;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_گرہ : گرہ);
}

خالی sched_domains_numa_masks_سیٹ(غیرعلامتی صحیح cpu)
{
	صحیح گرہ = cpu_مقصد_گرہ(cpu);
	صحیح i, j;

	برائے (i = 0; i < sched_domains_numa_سطحیں; i++) {
		برائے (j = 0; j < nr_گرہ_ids; j++) {
			اگر (!گرہ_state(j, N_CPU))
				جاری;

			/* Set ourselves in the remote node's masks */
			اگر (arch_sched_گرہ_فاصلہ(j, گرہ) <=
			    sched_domains_numa_فاصلہ[i])
				cpumask_سیٹ_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

خالی sched_domains_numa_masks_clear(غیرعلامتی صحیح cpu)
{
	صحیح i, j;

	برائے (i = 0; i < sched_domains_numa_سطحیں; i++) {
		برائے (j = 0; j < nr_گرہ_ids; j++) {
			اگر (sched_domains_numa_masks[i][j])
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
صحیح sched_numa_ڈھونڈو_closest(ثابت ساخت cpumask *cpus, صحیح cpu)
{
	صحیح i, j = cpu_مقصد_گرہ(cpu), ملا = nr_cpu_ids;
	ساخت cpumask ***masks;

	rcu_read_تالا();
	masks = rcu_dereference(sched_domains_numa_masks);
	اگر (!masks)
		جاؤ unlock;
	برائے (i = 0; i < sched_domains_numa_سطحیں; i++) {
		اگر (!masks[i][j])
			توڑو;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		اگر (cpu < nr_cpu_ids) {
			ملا = cpu;
			توڑو;
		}
	}
unlock:
	rcu_read_unlock();

	واپس ملا;
}

ساخت __cmp_key {
	ثابت ساخت cpumask *cpus;
	ساخت cpumask ***masks;
	صحیح گرہ;
	صحیح cpu;
	صحیح w;
};

ساکن صحیح hop_cmp(ثابت خالی *a, ثابت خالی *b)
{
	ساخت cpumask **پچھلا_hop, **cur_hop = *(ساخت cpumask ***)b;
	ساخت __cmp_key *k = (ساخت __cmp_key *)a;

	اگر (cpumask_وزن_and(k->cpus, cur_hop[k->گرہ]) <= k->cpu)
		واپس 1;

	اگر (b == k->masks) {
		k->w = 0;
		واپس 0;
	}

	پچھلا_hop = *((ساخت cpumask ***)b - 1);
	k->w = cpumask_وزن_and(k->cpus, پچھلا_hop[k->گرہ]);
	اگر (k->w <= k->cpu)
		واپس 0;

	واپس -1;
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
صحیح sched_numa_ڈھونڈو_nth_cpu(ثابت ساخت cpumask *cpus, صحیح cpu, صحیح گرہ)
{
	ساخت __cmp_key k = { .cpus = cpus, .cpu = cpu };
	ساخت cpumask ***hop_masks;
	صحیح hop, ret = nr_cpu_ids;

	اگر (گرہ == NUMA_NO_گرہ)
		واپس cpumask_nth_and(cpu, cpus, cpu_online_نقاب);

	rcu_read_تالا();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	گرہ = numa_قریب_ترین_گرہ(گرہ, N_CPU);
	k.گرہ = گرہ;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	اگر (!k.masks)
		جاؤ unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_سطحیں, حجم(k.masks[0]), hop_cmp);
	اگر (!hop_masks)
		جاؤ unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][گرہ], k.masks[hop-1][گرہ]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][گرہ]);
unlock:
	rcu_read_unlock();
	واپس ret;
}
EXPORT_علامت_GPL(sched_numa_ڈھونڈو_nth_cpu);

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
ثابت ساخت cpumask *sched_numa_hop_نقاب(غیرعلامتی صحیح گرہ, غیرعلامتی صحیح hops)
{
	ساخت cpumask ***masks;

	اگر (گرہ >= nr_گرہ_ids || hops >= sched_domains_numa_سطحیں)
		واپس ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	اگر (!masks)
		واپس ERR_PTR(-EBUSY);

	واپس masks[hops][گرہ];
}
EXPORT_علامت_GPL(sched_numa_hop_نقاب);

#اگر_ختم /* CONFIG_NUMA */

ساکن صحیح __sdt_alloc(ثابت ساخت cpumask *cpu_map)
{
	ساخت sched_domain_topology_level *tl;
	صحیح j;

	for_each_sd_topology(tl) {
		ساخت sd_ڈیٹا *sdd = &tl->ڈیٹا;

		sdd->sd = alloc_percpu(ساخت sched_domain *);
		اگر (!sdd->sd)
			واپس -ENOMEM;

		sdd->sg = alloc_percpu(ساخت sched_گروہ *);
		اگر (!sdd->sg)
			واپس -ENOMEM;

		sdd->sgc = alloc_percpu(ساخت sched_گروہ_استعداد *);
		اگر (!sdd->sgc)
			واپس -ENOMEM;

		for_each_cpu(j, cpu_map) {
			ساخت sched_domain *sd;
			ساخت sched_گروہ *sg;
			ساخت sched_گروہ_استعداد *sgc;

			sd = kzalloc_گرہ(حجم(ساخت sched_domain) + cpumask_سائز(),
					GFP_KERNEL, cpu_مقصد_گرہ(j));
			اگر (!sd)
				واپس -ENOMEM;

			*فی_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_گرہ(حجم(ساخت sched_گروہ) + cpumask_سائز(),
					GFP_KERNEL, cpu_مقصد_گرہ(j));
			اگر (!sg)
				واپس -ENOMEM;

			sg->اگلا = sg;

			*فی_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_گرہ(حجم(ساخت sched_گروہ_استعداد) + cpumask_سائز(),
					GFP_KERNEL, cpu_مقصد_گرہ(j));
			اگر (!sgc)
				واپس -ENOMEM;

			sgc->id = j;

			*فی_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	واپس 0;
}

ساکن خالی __sdt_free(ثابت ساخت cpumask *cpu_map)
{
	ساخت sched_domain_topology_level *tl;
	صحیح j;

	for_each_sd_topology(tl) {
		ساخت sd_ڈیٹا *sdd = &tl->ڈیٹا;

		for_each_cpu(j, cpu_map) {
			ساخت sched_domain *sd;

			اگر (sdd->sd) {
				sd = *فی_cpu_ptr(sdd->sd, j);
				اگر (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*فی_cpu_ptr(sdd->sd, j));
			}

			اگر (sdd->sg)
				kfree(*فی_cpu_ptr(sdd->sg, j));
			اگر (sdd->sgc)
				kfree(*فی_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = تہی;
		free_percpu(sdd->sg);
		sdd->sg = تہی;
		free_percpu(sdd->sgc);
		sdd->sgc = تہی;
	}
}

ساکن صحیح __sds_alloc(ساخت s_ڈیٹا *d, ثابت ساخت cpumask *cpu_map)
{
	صحیح j;

	d->sds = alloc_percpu(ساخت sched_domain_shared *);
	اگر (!d->sds)
		واپس -ENOMEM;

	for_each_cpu(j, cpu_map) {
		ساخت sched_domain_shared *sds;

		sds = kzalloc_گرہ(حجم(ساخت sched_domain_shared),
				GFP_KERNEL, cpu_مقصد_گرہ(j));
		اگر (!sds)
			واپس -ENOMEM;

		*فی_cpu_ptr(d->sds, j) = sds;
	}

	واپس 0;
}

ساکن خالی __sds_free(ساخت s_ڈیٹا *d, ثابت ساخت cpumask *cpu_map)
{
	صحیح j;

	اگر (!d->sds)
		واپس;

	for_each_cpu(j, cpu_map)
		kfree(*فی_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = تہی;
}

ساکن ساخت sched_domain *build_sched_domain(ساخت sched_domain_topology_level *tl,
		ثابت ساخت cpumask *cpu_map, ساخت sched_domain_attr *attr,
		ساخت sched_domain *بچہ, صحیح cpu)
{
	ساخت sched_domain *sd = sd_init(tl, cpu_map, بچہ, cpu);

	اگر (بچہ) {
		sd->level = بچہ->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		بچہ->والد = sd;

		اگر (!cpumask_subset(sched_domain_وسعت(بچہ),
				    sched_domain_وسعت(sd))) {
			pr_err("BUG: arch topology borken\n");
			pr_err("     the %s domain not a subset of the %s domain\n",
					بچہ->name, sd->name);
			/* Fixup, ensure @sd has at least @child CPUs. */
			cpumask_or(sched_domain_وسعت(sd),
				   sched_domain_وسعت(sd),
				   sched_domain_وسعت(بچہ));
		}

	}
	سیٹ_domain_attribute(sd, attr);

	واپس sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
ساکن bool topology_وسعت_sane(ثابت ساخت cpumask *cpu_map)
{
	ساخت sched_domain_topology_level *tl;
	ساخت cpumask *covered, *id_نظر_آیا;
	صحیح cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_نظر_آیا = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		صحیح tl_common_flags = 0;

		اگر (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		اگر (tl_common_flags & SD_NUMA)
			جاری;

		cpumask_clear(covered);
		cpumask_clear(id_نظر_آیا);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ثابت ساخت cpumask *tl_cpu_نقاب = tl->نقاب(tl, cpu);
			صحیح id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_پہلا(tl_cpu_نقاب);

			اگر (cpumask_test_cpu(id, id_نظر_آیا)) {
				/* First CPU has already been seen, ensure identical spans */
				اگر (!cpumask_equal(tl->نقاب(tl, id), tl_cpu_نقاب))
					واپس false;
			} ورنہ {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				اگر (cpumask_intersects(tl_cpu_نقاب, covered))
					واپس false;

				cpumask_or(covered, covered, tl_cpu_نقاب);
				cpumask_سیٹ_cpu(id, id_نظر_آیا);
			}
		}
	}
	واپس true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
ساکن خالی adjust_numa_imbalance(ساخت sched_domain *sd_llc)
{
	ساخت sched_domain *والد;
	غیرعلامتی صحیح imb_وسعت = 1;
	غیرعلامتی صحیح imb = 0;
	غیرعلامتی صحیح nr_llcs;

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
	nr_llcs = sd_llc->والد->وسعت_وزن / sd_llc->وسعت_وزن;
	اگر (nr_llcs == 1)
		imb = sd_llc->والد->وسعت_وزن >> 3;
	ورنہ
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
	جب (والد && !(والد->flags & SD_NUMA))
		والد = والد->والد;

	imb_وسعت = والد ? والد->وسعت_وزن : sd_llc->والد->وسعت_وزن;

	/* Update the upper remainder of the topology */
	والد = sd_llc->والد;
	جب (والد) {
		صحیح ضربی_عدد = max(1U, (والد->وسعت_وزن / imb_وسعت));

		والد->imb_numa_nr = imb * ضربی_عدد;
		والد = والد->والد;
	}
}

ساکن خالی
init_sched_domain_shared(ساخت s_ڈیٹا *d, ساخت sched_domain *sd, صحیح flags)
{
	ساخت sched_domain_shared *sds = تہی;
	صحیح cpu;

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
	for_each_cpu(cpu, sched_domain_وسعت(sd)) {
		sds = *فی_cpu_ptr(d->sds, cpu);

		اگر (!sds->alloc_flags ||
		    sd->وسعت_وزن == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			توڑو;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	اگر (WARN_ON_ONCE(!sd->shared))
		sd->shared = sds;

	/*
	 * nr_busy_cpus is consumed only by the NOHZ kick path via
	 * sd_balance_shared; on the asym-capacity path it is initialized but
	 * never read.
	 */
	atomic_سیٹ(&sd->shared->nr_busy_cpus, sd->وسعت_وزن);
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
ساکن bool claim_asym_sched_domain_shared(ساخت s_ڈیٹا *d, صحیح cpu)
{
	ساخت sched_domain *sd = *فی_cpu_ptr(d->sd, cpu);
	ساخت sched_domain *sd_asym;

	اگر (!sd)
		واپس false;

	sd_asym = sd;
	جب (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_بھرا))
		sd_asym = sd_asym->والد;

	اگر (!sd_asym || (sd_asym->flags & SD_NUMA))
		واپس false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	واپس true;
}

ساکن صحیح __sched_domains_alloc_llc_id(خالی)
{
	صحیح lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_پہلا_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	اگر (lid >= nr_cpu_ids)
		واپس -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_آخری(sched_domains_llc_id_allocmask);
	اگر (max > max_lid)
		max_lid = max;

	واپس lid;
}

ساکن خالی __sched_domains_free_llc_id(صحیح cpu)
{
	صحیح i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = فی_cpu(sd_llc_id, cpu);
	اگر (lid == -1 || lid >= nr_cpu_ids)
		واپس;

	فی_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_نقاب(cpu)) {
		/* An online CPU owns the llc_id. */
		اگر (فی_cpu(sd_llc_id, i) == lid)
			واپس;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_آخری(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	اگر (max < max_lid)
		max_lid = max;
}

خالی sched_domains_free_llc_id(صحیح cpu)
{
	sched_domains_mutex_تالا();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
ساکن صحیح
build_sched_domains(ثابت ساخت cpumask *cpu_map, ساخت sched_domain_attr *attr,
		    bool *multi_llcs)
{
	شمار s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	ساخت sched_domain *sd;
	ساخت s_ڈیٹا d;
	ساخت rq *rq = تہی;
	صحیح i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	اگر (WARN_ON(cpumask_خالی_خانے(cpu_map)))
		جاؤ error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	اگر (alloc_state != sa_rootdomain)
		جاؤ error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		ساخت sched_domain_topology_level *tl;
		صحیح lid;

		sd = تہی;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			اگر (tl == sched_domain_topology)
				*فی_cpu_ptr(d.sd, i) = sd;
			اگر (cpumask_equal(cpu_map, sched_domain_وسعت(sd)))
				توڑو;
		}

		lid = فی_cpu(sd_llc_id, i);
		اگر (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			برائے (صحیح j = cpumask_پہلا(llc_نقاب(i));
			     j < nr_cpu_ids;
			     j = cpumask_اگلا(j, llc_نقاب(i))) {
				اگر (i == j)
					جاری;

				lid = فی_cpu(sd_llc_id, j);

				اگر (lid != -1) {
					فی_cpu(sd_llc_id, i) = lid;

					توڑو;
				}
			}

			/* a new LLC is detected */
			اگر (lid == -1)
				فی_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	اگر (WARN_ON(!topology_وسعت_sane(cpu_map)))
		جاؤ error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		برائے (sd = *فی_cpu_ptr(d.sd, i); sd; sd = sd->والد) {
			sd->وسعت_وزن = cpumask_وزن(sched_domain_وسعت(sd));
			اگر (sd->flags & SD_NUMA) {
				اگر (build_overlap_sched_groups(sd, i))
					جاؤ error;
			} ورنہ {
				اگر (build_sched_groups(sd, i))
					جاؤ error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *فی_cpu_ptr(d.sd, i);
		اگر (!sd)
			جاری;

		اگر (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		جب (sd->والد && (sd->والد->flags & SD_SHARE_LLC))
			sd = sd->والد;

		اگر (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			اگر (sd->والد) {
				اگر (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				اگر (sd_اندر_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	برائے (i = nr_cpumask_بٹ-1; i >= 0; i--) {
		اگر (!cpumask_test_cpu(i, cpu_map))
			جاری;

		claim_allocations(i, &d);

		برائے (sd = *فی_cpu_ptr(d.sd, i); sd; sd = sd->والد)
			init_sched_groups_استعداد(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_تالا();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *فی_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		اگر (سب_سے_نیچا_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	اگر (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	اگر (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_فعال);

	اگر (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_دلائل(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	واپس ret;
}

/* Current sched domains: */
ساکن cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
ساکن صحیح				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
ساکن ساخت sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
ساکن cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
صحیح __weak arch_update_cpu_topology(خالی)
{
	واپس 0;
}

cpumask_var_t *alloc_sched_domains(غیرعلامتی صحیح ndoms)
{
	صحیح i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	اگر (!doms)
		واپس تہی;
	برائے (i = 0; i < ndoms; i++) {
		اگر (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			واپس تہی;
		}
	}
	واپس doms;
}

خالی free_sched_domains(cpumask_var_t doms[], غیرعلامتی صحیح ndoms)
{
	غیرعلامتی صحیح i;
	برائے (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
صحیح __init sched_init_domains(ثابت ساخت cpumask *cpu_map)
{
	bool multi_llcs;
	صحیح err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_استعداد_جائزہ();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	اگر (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], تہی, &multi_llcs);
	اگر (!err)
		sched_cache_سیٹ(multi_llcs);

	واپس err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
ساکن خالی detach_destroy_domains(ثابت ساخت cpumask *cpu_map)
{
	غیرعلامتی صحیح cpu = cpumask_any(cpu_map);
	صحیح i;

	اگر (rcu_access_اشارہ_گر(فی_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	اگر (static_branch_unlikely(&sched_cluster_فعال))
		static_branch_dec_cpuslocked(&sched_cluster_فعال);

	rcu_read_تالا();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(تہی, &def_جڑ_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
ساکن صحیح dattrs_equal(ساخت sched_domain_attr *cur, صحیح idx_cur,
			ساخت sched_domain_attr *new, صحیح idx_new)
{
	ساخت sched_domain_attr tmp;

	/* Fast path: */
	اگر (!new && !cur)
		واپس 1;

	tmp = SD_ATTR_INIT;

	واپس !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			حجم(ساخت sched_domain_attr));
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
ساکن خالی تقسیم_sched_domains_locked(صحیح ndoms_new, cpumask_var_t doms_new[],
				    ساخت sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	صحیح i, j, n;
	صحیح new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	اگر (new_topology)
		asym_cpu_استعداد_جائزہ();

	اگر (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		اگر (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_فعال_نقاب,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} ورنہ {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	برائے (i = 0; i < ndoms_cur; i++) {
		برائے (j = 0; j < n && !new_topology; j++) {
			اگر (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				جاؤ match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	اگر (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_فعال_نقاب,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	برائے (i = 0; i < ndoms_new; i++) {
		برائے (j = 0; j < n && !new_topology; j++) {
			اگر (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				ساخت sched_domain *sd;
				صحیح cpu = cpumask_پہلا(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				جب (sd && sd->والد && (sd->والد->flags & SD_SHARE_LLC))
					sd = sd->والد;
				اگر (sd && (sd->flags & SD_SHARE_LLC) && sd->والد &&
				    sd_اندر_multi_llcs(sd))
					has_multi_llcs = true;
				جاؤ match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : تہی,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_سیٹ(has_multi_llcs);

#اگر defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	برائے (i = 0; i < ndoms_new; i++) {
		برائے (j = 0; j < n && !sched_energy_update; j++) {
			اگر (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_پہلا(doms_cur[j]))->rd->pd) {
				has_eas = true;
				جاؤ match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_سیٹ(has_eas);
#اگر_ختم

	/* Remember the new sched domains: */
	اگر (doms_cur != &fallback_doms)
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
خالی تقسیم_sched_domains(صحیح ndoms_new, cpumask_var_t doms_new[],
			     ساخت sched_domain_attr *dattr_new)
{
	sched_domains_mutex_تالا();
	تقسیم_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
