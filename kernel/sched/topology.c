// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/isolation.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/bsearch.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
ਖਾਲੀ sched_domains_mutex_ਤਾਲਾ(ਖਾਲੀ)
{
	mutex_ਤਾਲਾ(&sched_domains_mutex);
}
ਖਾਲੀ sched_domains_mutex_unlock(ਖਾਲੀ)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
ਸਥਿਰ cpumask_var_t sched_domains_llc_id_allocmask;
ਸਥਿਰ cpumask_var_t sched_domains_tmpmask;
ਸਥਿਰ cpumask_var_t sched_domains_tmpmask2;
ਪੂਰਨ max_lid;

ਸਥਿਰ ਪੂਰਨ __init sched_debug_ਤਿਆਰੀ(ਅੱਖਰ *str)
{
	sched_debug_verbose = true;

	ਵਾਪਸ 0;
}
early_param("sched_verbose", sched_debug_ਤਿਆਰੀ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_SMT
ਸਥਿਰ bool sched_smt_asym_packing __read_mostly;

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_sched_smt_asym_packing(ਅੱਖਰ *str)
{
	ਜੇ (strcmp(str, "on"))
		ਵਾਪਸ 0;

	sched_smt_asym_packing = true;
	ਵਾਪਸ 1;
}
__setup("sched_smt_asym_packing=", ਤਿਆਰੀ_sched_smt_asym_packing);
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_debug(ਖਾਲੀ)
{
	ਵਾਪਸ sched_debug_verbose;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ਅਚਲ ਬਣਤਰ sd_flag_debug sd_flag_debug[] = {
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/sd_flags.h>
};
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SD_FLAG

ਸਥਿਰ ਪੂਰਨ sched_domain_debug_one(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu, ਪੂਰਨ level,
				  ਬਣਤਰ cpumask *groupmask)
{
	ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ = sd->groups;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = sd->flags;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_ਦਲੀਲਾਂ(sched_domain_ਵਿਸਤਾਰ(sd)), sd->name);

	ਜੇ (!cpumask_test_cpu(cpu, sched_domain_ਵਿਸਤਾਰ(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	ਜੇ (ਸਮੂਹ && !cpumask_test_cpu(cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(idx, &flags, __SD_FLAG_CNT) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flag = ਦੋਆਧਾਰੀ_ਅੰਕ(idx);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ meta_flags = sd_flag_debug[idx].meta_flags;

		ਜੇ ((meta_flags & SDF_SHARED_ਬੱਚਾ) && sd->ਬੱਚਾ &&
		    !(sd->ਬੱਚਾ->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		ਜੇ ((meta_flags & SDF_SHARED_ਮਾਪਾ) && sd->ਮਾਪਾ &&
		    !(sd->ਮਾਪਾ->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	ਕਰੋ {
		ਜੇ (!ਸਮੂਹ) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			ਰੋਕੋ;
		}

		ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			ਰੋਕੋ;
		}

		ਜੇ (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			ਰੋਕੋ;
		}

		cpumask_or(groupmask, groupmask, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ));

		printk(KERN_CONT " %d:{ span=%*pbl",
				ਸਮੂਹ->sgc->id,
				cpumask_pr_ਦਲੀਲਾਂ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ)));

		ਜੇ ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(ਸਮੂਹ_balance_ਨਕਾਬ(ਸਮੂਹ), sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_ਦਲੀਲਾਂ(ਸਮੂਹ_balance_ਨਕਾਬ(ਸਮੂਹ)));
		}

		ਜੇ (ਸਮੂਹ->sgc->ਸਮਰੱਥਾ != SCHED_ਸਮਰੱਥਾ_SCALE)
			printk(KERN_CONT " cap=%lu", ਸਮੂਹ->sgc->ਸਮਰੱਥਾ);

		ਜੇ (ਸਮੂਹ == sd->groups && sd->ਬੱਚਾ &&
		    !cpumask_equal(sched_domain_ਵਿਸਤਾਰ(sd->ਬੱਚਾ),
				   sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		ਸਮੂਹ = ਸਮੂਹ->ਅਗਲਾ;

		ਜੇ (ਸਮੂਹ != sd->groups)
			printk(KERN_CONT ",");

	} ਜਦੋਂ (ਸਮੂਹ != sd->groups);
	printk(KERN_CONT "\n");

	ਜੇ (!cpumask_equal(sched_domain_ਵਿਸਤਾਰ(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	ਜੇ (sd->ਮਾਪਾ &&
	    !cpumask_subset(groupmask, sched_domain_ਵਿਸਤਾਰ(sd->ਮਾਪਾ)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ sched_domain_debug(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu)
{
	ਪੂਰਨ level = 0;

	ਜੇ (!sched_debug_verbose)
		ਵਾਪਸ;

	ਜੇ (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		ਵਾਪਸ;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	ਲਈ (;;) {
		ਜੇ (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			ਰੋਕੋ;
		level++;
		sd = sd->ਮਾਪਾ;
		ਜੇ (!sd)
			ਰੋਕੋ;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ SD_DEGENERATE_GROUPS_ਨਕਾਬ =
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/sd_flags.h>
0;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SD_FLAG

ਸਥਿਰ ਪੂਰਨ sd_degenerate(ਬਣਤਰ sched_domain *sd)
{
	ਜੇ (cpumask_ਭਾਰ(sched_domain_ਵਿਸਤਾਰ(sd)) == 1)
		ਵਾਪਸ 1;

	/* Following flags need at least 2 groups */
	ਜੇ ((sd->flags & SD_DEGENERATE_GROUPS_ਨਕਾਬ) &&
	    (sd->groups != sd->groups->ਅਗਲਾ))
		ਵਾਪਸ 0;

	/* Following flags don't use groups */
	ਜੇ (sd->flags & (SD_WAKE_AFFINE))
		ਵਾਪਸ 0;

	ਵਾਪਸ 1;
}

ਸਥਿਰ ਪੂਰਨ
sd_ਮਾਪਾ_degenerate(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ sched_domain *ਮਾਪਾ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cflags = sd->flags, pflags = ਮਾਪਾ->flags;

	ਜੇ (sd_degenerate(ਮਾਪਾ))
		ਵਾਪਸ 1;

	ਜੇ (!cpumask_equal(sched_domain_ਵਿਸਤਾਰ(sd), sched_domain_ਵਿਸਤਾਰ(ਮਾਪਾ)))
		ਵਾਪਸ 0;

	/* Flags needing groups don't count if only 1 group in parent */
	ਜੇ (ਮਾਪਾ->groups == ਮਾਪਾ->groups->ਅਗਲਾ)
		pflags &= ~SD_DEGENERATE_GROUPS_ਨਕਾਬ;

	ਜੇ (~cflags & pflags)
		ਵਾਪਸ 0;

	ਵਾਪਸ 1;
}

#ਜੇ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_energy_present);
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_energy_aware = 1;
ਸਥਿਰ DEFINE_MUTEX(sched_energy_mutex);
ਸਥਿਰ bool sched_energy_update;

ਸਥਿਰ bool sched_is_eas_possible(ਅਚਲ ਬਣਤਰ cpumask *cpu_ਨਕਾਬ)
{
	bool any_asym_ਸਮਰੱਥਾ = false;
	ਪੂਰਨ i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_ਨਕਾਬ) {
		ਜੇ (rcu_access_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_asym_cpucapacity, i))) {
			any_asym_ਸਮਰੱਥਾ = true;
			ਰੋਕੋ;
		}
	}
	ਜੇ (!any_asym_ਸਮਰੱਥਾ) {
		ਜੇ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_ਦਲੀਲਾਂ(cpu_ਨਕਾਬ));
		}
		ਵਾਪਸ false;
	}

	/* EAS definitely does *not* handle SMT */
	ਜੇ (sched_smt_ਸਰਗਰਮ()) {
		ਜੇ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_ਦਲੀਲਾਂ(cpu_ਨਕਾਬ));
		}
		ਵਾਪਸ false;
	}

	ਜੇ (!arch_scale_freq_invariant()) {
		ਜੇ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_ਦਲੀਲਾਂ(cpu_ਨਕਾਬ));
		}
		ਵਾਪਸ false;
	}

	ਜੇ (!cpufreq_ready_for_eas(cpu_ਨਕਾਬ)) {
		ਜੇ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_ਦਲੀਲਾਂ(cpu_ਨਕਾਬ));
		}
		ਵਾਪਸ false;
	}

	ਵਾਪਸ true;
}

ਖਾਲੀ rebuild_sched_domains_energy(ਖਾਲੀ)
{
	mutex_ਤਾਲਾ(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਪੂਰਨ sched_energy_aware_handler(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ,
		ਖਾਲੀ *ਬਫਰ, ਮਾਪ_t *lenp, loff_t *ppos)
{
	ਪੂਰਨ ret;

	ਜੇ (ਲਿਖ && !capable(CAP_SYS_ADMIN))
		ਵਾਪਸ -EPERM;

	ਜੇ (!sched_is_eas_possible(cpu_ਸਰਗਰਮ_ਨਕਾਬ)) {
		ਜੇ (ਲਿਖ) {
			ਵਾਪਸ -EOPNOTSUPP;
		} ਨਹੀਂਤਾਂ {
			*lenp = 0;
			ਵਾਪਸ 0;
		}
	}

	ret = proc_dointvec_minmax(ਸਾਰਣੀ, ਲਿਖ, ਬਫਰ, lenp, ppos);
	ਜੇ (!ret && ਲਿਖ) {
		ਜੇ (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.ਅੰਕੜੇ           = &sysctl_sched_energy_aware,
		.maxlen         = ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ਸਥਿਰ ਪੂਰਨ __init sched_energy_aware_sysctl_init(ਖਾਲੀ)
{
	ਰਜਿਸਟਰ_sysctl_init("kernel", sched_energy_aware_sysctls);
	ਵਾਪਸ 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */

ਸਥਿਰ ਖਾਲੀ free_pd(ਬਣਤਰ perf_domain *pd)
{
	ਬਣਤਰ perf_domain *tmp;

	ਜਦੋਂ (pd) {
		tmp = pd->ਅਗਲਾ;
		kfree(pd);
		pd = tmp;
	}
}

ਸਥਿਰ ਬਣਤਰ perf_domain *ਲੱਭੋ_pd(ਬਣਤਰ perf_domain *pd, ਪੂਰਨ cpu)
{
	ਜਦੋਂ (pd) {
		ਜੇ (cpumask_test_cpu(cpu, perf_domain_ਵਿਸਤਾਰ(pd)))
			ਵਾਪਸ pd;
		pd = pd->ਅਗਲਾ;
	}

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਬਣਤਰ perf_domain *pd_init(ਪੂਰਨ cpu)
{
	ਬਣਤਰ em_perf_domain *obj = em_cpu_get(cpu);
	ਬਣਤਰ perf_domain *pd;

	ਜੇ (!obj) {
		ਜੇ (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		ਵਾਪਸ ਨਿਰਾ;
	}

	pd = kzalloc_obj(*pd);
	ਜੇ (!pd)
		ਵਾਪਸ ਨਿਰਾ;
	pd->em_pd = obj;

	ਵਾਪਸ pd;
}

ਸਥਿਰ ਖਾਲੀ perf_domain_debug(ਅਚਲ ਬਣਤਰ cpumask *cpu_map,
						ਬਣਤਰ perf_domain *pd)
{
	ਜੇ (!sched_debug() || !pd)
		ਵਾਪਸ;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_ਦਲੀਲਾਂ(cpu_map));

	ਜਦੋਂ (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_ਪਹਿਲਾ(perf_domain_ਵਿਸਤਾਰ(pd)),
				cpumask_pr_ਦਲੀਲਾਂ(perf_domain_ਵਿਸਤਾਰ(pd)),
				em_pd_nr_perf_ਹਾਲਤਾਂ(pd->em_pd));
		pd = pd->ਅਗਲਾ;
	}

	printk(KERN_CONT "\n");
}

ਸਥਿਰ ਖਾਲੀ destroy_perf_domain_rcu(ਬਣਤਰ rcu_ਸਿਰ *rp)
{
	ਬਣਤਰ perf_domain *pd;

	pd = container_ਦਾ(rp, ਬਣਤਰ perf_domain, rcu);
	free_pd(pd);
}

ਸਥਿਰ ਖਾਲੀ sched_energy_ਸੈੱਟ(bool has_eas)
{
	ਜੇ (!has_eas && sched_energy_enabled()) {
		ਜੇ (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} ਨਹੀਂਤਾਂ ਜੇ (has_eas && !sched_energy_enabled()) {
		ਜੇ (sched_debug())
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
ਸਥਿਰ bool build_perf_domains(ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਪੂਰਨ i;
	ਬਣਤਰ perf_domain *pd = ਨਿਰਾ, *tmp;
	ਪੂਰਨ cpu = cpumask_ਪਹਿਲਾ(cpu_map);
	ਬਣਤਰ ਜੜ੍ਹ_domain *rd = cpu_rq(cpu)->rd;

	ਜੇ (!sysctl_sched_energy_aware)
		ਜਾਓ ਮੁਕਤ;

	ਜੇ (!sched_is_eas_possible(cpu_map))
		ਜਾਓ ਮੁਕਤ;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		ਜੇ (ਲੱਭੋ_pd(pd, i))
			ਜਾਰੀ;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		ਜੇ (!tmp)
			ਜਾਓ ਮੁਕਤ;
		tmp->ਅਗਲਾ = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(rd->pd, pd);
	ਜੇ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ਵਾਪਸ !!pd;

ਮੁਕਤ:
	free_pd(pd);
	tmp = rd->pd;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(rd->pd, ਨਿਰਾ);
	ਜੇ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ਵਾਪਸ false;
}
#ਨਹੀਂਤਾਂ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
ਸਥਿਰ ਖਾਲੀ free_pd(ਬਣਤਰ perf_domain *pd) { }
#ਜੇ_ਅੰਤ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

ਸਥਿਰ ਖਾਲੀ free_rootdomain(ਬਣਤਰ rcu_ਸਿਰ *rcu)
{
	ਬਣਤਰ ਜੜ੍ਹ_domain *rd = container_ਦਾ(rcu, ਬਣਤਰ ਜੜ੍ਹ_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_ਨਕਾਬ);
	free_cpumask_var(rd->rto_ਨਕਾਬ);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->ਵਿਸਤਾਰ);
	free_pd(rd->pd);
	kfree(rd);
}

ਖਾਲੀ rq_attach_ਜੜ੍ਹ(ਬਣਤਰ rq *rq, ਬਣਤਰ ਜੜ੍ਹ_domain *rd)
{
	ਬਣਤਰ ਜੜ੍ਹ_domain *old_rd = ਨਿਰਾ;
	ਬਣਤਰ rq_flags rf;

	rq_ਤਾਲਾ_irqsave(rq, &rf);

	ਜੇ (rq->rd) {
		old_rd = rq->rd;

		ਜੇ (cpumask_test_cpu(rq->cpu, old_rd->online))
			ਸੈੱਟ_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->ਵਿਸਤਾਰ);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		ਜੇ (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = ਨਿਰਾ;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_ਸੈੱਟ_cpu(rq->cpu, rd->ਵਿਸਤਾਰ);
	ਜੇ (cpumask_test_cpu(rq->cpu, cpu_ਸਰਗਰਮ_ਨਕਾਬ))
		ਸੈੱਟ_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	ਜੇ (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	ਜੇ (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#ਜੇ_ਅੰਤ

	rq_unlock_irqrestore(rq, &rf);

	ਜੇ (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

ਖਾਲੀ sched_get_rd(ਬਣਤਰ ਜੜ੍ਹ_domain *rd)
{
	atomic_inc(&rd->refcount);
}

ਖਾਲੀ sched_put_rd(ਬਣਤਰ ਜੜ੍ਹ_domain *rd)
{
	ਜੇ (!atomic_dec_and_test(&rd->refcount))
		ਵਾਪਸ;

	call_rcu(&rd->rcu, free_rootdomain);
}

ਸਥਿਰ ਪੂਰਨ init_rootdomain(ਬਣਤਰ ਜੜ੍ਹ_domain *rd)
{
	ਜੇ (!zalloc_cpumask_var(&rd->ਵਿਸਤਾਰ, GFP_KERNEL))
		ਜਾਓ ਬਾਹਰਲਾ;
	ਜੇ (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		ਜਾਓ free_ਵਿਸਤਾਰ;
	ਜੇ (!zalloc_cpumask_var(&rd->dlo_ਨਕਾਬ, GFP_KERNEL))
		ਜਾਓ free_online;
	ਜੇ (!zalloc_cpumask_var(&rd->rto_ਨਕਾਬ, GFP_KERNEL))
		ਜਾਓ free_dlo_ਨਕਾਬ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_RT_ਧੱਕੋ_IPI
	rd->rto_cpu = -1;
	raw_spin_ਤਾਲਾ_init(&rd->rto_ਤਾਲਾ);
	rd->rto_ਧੱਕੋ_work = IRQ_WORK_INIT_HARD(rto_ਧੱਕੋ_irq_work_func);
#ਜੇ_ਅੰਤ

	rd->ਦੇਖੋ_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	ਜੇ (cpudl_init(&rd->cpudl) != 0)
		ਜਾਓ free_rto_ਨਕਾਬ;

	ਜੇ (cpupri_init(&rd->cpupri) != 0)
		ਜਾਓ free_cpudl;
	ਵਾਪਸ 0;

free_cpudl:
	cpudl_cleanup(&rd->cpudl);
free_rto_ਨਕਾਬ:
	free_cpumask_var(rd->rto_ਨਕਾਬ);
free_dlo_ਨਕਾਬ:
	free_cpumask_var(rd->dlo_ਨਕਾਬ);
free_online:
	free_cpumask_var(rd->online);
free_ਵਿਸਤਾਰ:
	free_cpumask_var(rd->ਵਿਸਤਾਰ);
ਬਾਹਰਲਾ:
	ਵਾਪਸ -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
ਬਣਤਰ ਜੜ੍ਹ_domain def_ਜੜ੍ਹ_domain;

ਖਾਲੀ __init init_defrootdomain(ਖਾਲੀ)
{
	init_rootdomain(&def_ਜੜ੍ਹ_domain);

	atomic_ਸੈੱਟ(&def_ਜੜ੍ਹ_domain.refcount, 1);
}

ਸਥਿਰ ਬਣਤਰ ਜੜ੍ਹ_domain *alloc_rootdomain(ਖਾਲੀ)
{
	ਬਣਤਰ ਜੜ੍ਹ_domain *rd;

	rd = kzalloc_obj(*rd);
	ਜੇ (!rd)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (init_rootdomain(rd) != 0) {
		kfree(rd);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਵਾਪਸ rd;
}

ਸਥਿਰ ਖਾਲੀ free_sched_groups(ਬਣਤਰ sched_ਸਮੂਹ *sg, ਪੂਰਨ free_sgc)
{
	ਬਣਤਰ sched_ਸਮੂਹ *tmp, *ਪਹਿਲਾ;

	ਜੇ (!sg)
		ਵਾਪਸ;

	ਪਹਿਲਾ = sg;
	ਕਰੋ {
		tmp = sg->ਅਗਲਾ;

		ਜੇ (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		ਜੇ (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} ਜਦੋਂ (sg != ਪਹਿਲਾ);
}

ਸਥਿਰ ਖਾਲੀ free_sched_domain_shared(ਬਣਤਰ sched_domain_shared *sds)
{
	ਜੇ (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

ਸਥਿਰ ਖਾਲੀ destroy_sched_domain(ਬਣਤਰ sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#ਜੇ_ਅੰਤ
	kfree(sd);
}

ਸਥਿਰ ਖਾਲੀ destroy_sched_domains_rcu(ਬਣਤਰ rcu_ਸਿਰ *rcu)
{
	ਬਣਤਰ sched_domain *sd = container_ਦਾ(rcu, ਬਣਤਰ sched_domain, rcu);

	ਜਦੋਂ (sd) {
		ਬਣਤਰ sched_domain *ਮਾਪਾ = sd->ਮਾਪਾ;
		destroy_sched_domain(sd);
		sd = ਮਾਪਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ destroy_sched_domains(ਬਣਤਰ sched_domain *sd)
{
	ਜੇ (sd)
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
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_llc);
DEFINE_ਪ੍ਰਤੀ_CPU(ਪੂਰਨ, sd_llc_ਮਾਪ);
DEFINE_ਪ੍ਰਤੀ_CPU(ਪੂਰਨ, sd_llc_id) = -1;
DEFINE_ਪ੍ਰਤੀ_CPU(ਪੂਰਨ, sd_share_id);
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_numa);
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_asym_packing);
DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_cluster_ਸਰਗਰਮ);

ਸਥਿਰ ਖਾਲੀ update_ਸਿਖਰ_cache_domain(ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain_shared *sds = ਨਿਰਾ;
	ਬਣਤਰ sched_domain *sd;
	ਪੂਰਨ id = cpu;
	ਪੂਰਨ ਮਾਪ = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	ਜੇ (sd) {
		id = cpumask_ਪਹਿਲਾ(sched_domain_ਵਿਸਤਾਰ(sd));
		ਮਾਪ = cpumask_ਭਾਰ(sched_domain_ਵਿਸਤਾਰ(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_llc, cpu), sd);
	ਪ੍ਰਤੀ_cpu(sd_llc_ਮਾਪ, cpu) = ਮਾਪ;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_llc_shared, cpu), sds);

	sd = ਸਭ_ਤੋਂ_ਨੀਵਾਂ_flag_domain(cpu, SD_CLUSTER);
	ਜੇ (sd)
		id = cpumask_ਪਹਿਲਾ(sched_domain_ਵਿਸਤਾਰ(sd));

	/*
	 * This assignment should be placed after the sd_llc_id as
	 * we want this id equals to cluster id on cluster machines
	 * but equals to LLC id on non-Cluster machines.
	 */
	ਪ੍ਰਤੀ_cpu(sd_share_id, cpu) = id;

	sd = ਸਭ_ਤੋਂ_ਨੀਵਾਂ_flag_domain(cpu, SD_NUMA);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_asym_packing, cpu), sd);

	sd = ਸਭ_ਤੋਂ_ਨੀਵਾਂ_flag_domain(cpu, SD_ASYM_CPUCAPACITY_ਭਰਿਆ);
	/*
	 * The shared object is attached to sd_asym_cpucapacity only when the
	 * asym domain is non-overlapping (i.e., not built from SD_NUMA).
	 * On overlapping (NUMA) asym domains we fall back to letting the
	 * SD_SHARE_LLC path own the shared object, so sd->shared may be NULL
	 * here.
	 */
	ਜੇ (sd && sd->shared)
		sds = sd->shared;

	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
ਸਥਿਰ ਖਾਲੀ
cpu_attach_domain(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ ਜੜ੍ਹ_domain *rd, ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਣਤਰ sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	ਲਈ (tmp = sd; tmp; ) {
		ਬਣਤਰ sched_domain *ਮਾਪਾ = tmp->ਮਾਪਾ;
		ਜੇ (!ਮਾਪਾ)
			ਰੋਕੋ;

		ਜੇ (sd_ਮਾਪਾ_degenerate(tmp, ਮਾਪਾ)) {
			tmp->ਮਾਪਾ = ਮਾਪਾ->ਮਾਪਾ;

			/* Pick reference to parent->shared. */
			ਜੇ (ਮਾਪਾ->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = ਮਾਪਾ->shared;
				ਮਾਪਾ->shared = ਨਿਰਾ;
			}

			ਜੇ (ਮਾਪਾ->ਮਾਪਾ) {
				ਮਾਪਾ->ਮਾਪਾ->ਬੱਚਾ = tmp;
				ਮਾਪਾ->ਮਾਪਾ->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			ਜੇ (ਮਾਪਾ->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(ਮਾਪਾ);
		} ਨਹੀਂਤਾਂ
			tmp = tmp->ਮਾਪਾ;
	}

	ਜੇ (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->ਮਾਪਾ;

		ਜੇ (sd) {
			ਬਣਤਰ sched_ਸਮੂਹ *sg = sd->groups;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_ਬਾਈਟਾਂ = tmp->llc_ਬਾਈਟਾਂ;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = ਨਿਰਾ;
			tmp->llc_max = 0;
			tmp->llc_ਬਾਈਟਾਂ = 0;
#ਜੇ_ਅੰਤ
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			ਕਰੋ {
				sg->flags = 0;
			} ਜਦੋਂ (sg != sd->groups);

			sd->ਬੱਚਾ = ਨਿਰਾ;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_ਜੜ੍ਹ(rq, rd);
	tmp = rq->sd;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_ਸਿਖਰ_cache_domain(cpu);
}

ਬਣਤਰ s_ਅੰਕੜੇ {
	ਬਣਤਰ sched_domain_shared * __percpu *sds;
	ਬਣਤਰ sched_domain * __percpu *sd;
	ਬਣਤਰ ਜੜ੍ਹ_domain	*rd;
};

ਗਿਣਤੀ s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_cache_ਸਰਗਰਮ);
/* user wants cache aware scheduling [0 or 1] */
ਪੂਰਨ sysctl_sched_cache_user = 1;

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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ get_effective_llc_ਬਾਈਟਾਂ(ਪੂਰਨ cpu,
					     ਬਣਤਰ sched_domain *sd)
{
	ਬਣਤਰ cacheinfo *ci;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hw_ਭਾਰ;

	ci = get_cpu_cacheinfo_llc(cpu);
	ਜੇ (!ci)
		ਵਾਪਸ 0;

	hw_ਭਾਰ = cpumask_ਭਾਰ(&ci->shared_cpu_map);
	ਜੇ (!hw_ਭਾਰ)
		ਵਾਪਸ 0;

	ਵਾਪਸ div_u64((u64)ci->ਮਾਪ * sd->ਵਿਸਤਾਰ_ਭਾਰ, hw_ਭਾਰ);
}

ਸਥਿਰ bool alloc_sd_llc(ਅਚਲ ਬਣਤਰ cpumask *cpu_map,
			 ਬਣਤਰ s_ਅੰਕੜੇ *d)
{
	ਬਣਤਰ sched_domain *sd, *ਸਿਖਰ_llc, *ਮਾਪਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *p;
	ਪੂਰਨ i;

	for_each_cpu(i, cpu_map) {
		sd = *ਪ੍ਰਤੀ_cpu_ptr(d->sd, i);
		ਜੇ (!sd)
			ਜਾਓ err;

		p = kcalloc_ਗੰਢ(max_lid + 1, ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
				 GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));
		ਜੇ (!p)
			ਜਾਓ err;

		ਸਿਖਰ_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		ਜਦੋਂ ((ਮਾਪਾ = rcu_dereference_protected(ਸਿਖਰ_llc->ਮਾਪਾ, true)) &&
		       (ਮਾਪਾ->flags & SD_SHARE_LLC))
			ਸਿਖਰ_llc = ਮਾਪਾ;

		ਜੇ (ਸਿਖਰ_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_ਬਾਈਟਾਂ = get_effective_llc_ਬਾਈਟਾਂ(i, ਸਿਖਰ_llc);
		} ਨਹੀਂਤਾਂ {
			/* avoid memory leak */
			kfree(p);
		}
	}

	ਵਾਪਸ true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *ਪ੍ਰਤੀ_cpu_ptr(d->sd, i);
		ਜੇ (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = ਨਿਰਾ;
			sd->llc_max = 0;
			sd->llc_ਬਾਈਟਾਂ = 0;
		}
	}

	ਵਾਪਸ false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
ਸਥਿਰ ਖਾਲੀ sched_cache_ਸਰਗਰਮ_ਸੈੱਟ_2(ਖਾਲੀ)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	ਜੇ (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_ਸਰਗਰਮ);
		ਜੇ (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		ਵਾਪਸ;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	ਜੇ (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_ਸਰਗਰਮ);
		ਜੇ (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} ਨਹੀਂਤਾਂ {
		static_branch_disable_cpuslocked(&sched_cache_ਸਰਗਰਮ);
		ਜੇ (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
ਖਾਲੀ sched_cache_ਸਰਗਰਮ_ਸੈੱਟ(ਖਾਲੀ)
{
	cpus_read_ਤਾਲਾ();
	sched_domains_mutex_ਤਾਲਾ();
	sched_cache_ਸਰਗਰਮ_ਸੈੱਟ_2();
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
ਖਾਲੀ sched_update_llc_ਬਾਈਟਾਂ(ਅਚਲ ਬਣਤਰ cpumask *cpus)
{
	ਬਣਤਰ sched_domain *sd, *sdp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	sched_domains_mutex_ਤਾਲਾ();

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
		sdp = rcu_dereference_sched_domain(ਪ੍ਰਤੀ_cpu(sd_llc, i));
		ਜੇ (!sdp)
			ਜਾਰੀ;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		ਜੇ (sd)
			sd->llc_ਬਾਈਟਾਂ = get_effective_llc_ਬਾਈਟਾਂ(i, sdp);
	}

	sched_domains_mutex_unlock();
}

ਸਥਿਰ ਖਾਲੀ sched_cache_ਸੈੱਟ(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	ਜੇ (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	ਨਹੀਂਤਾਂ
		static_branch_disable_cpuslocked(&sched_cache_present);

	sched_cache_ਸਰਗਰਮ_ਸੈੱਟ_2();
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ bool alloc_sd_llc(ਅਚਲ ਬਣਤਰ cpumask *cpu_map,
			 ਬਣਤਰ s_ਅੰਕੜੇ *d)
{
	ਵਾਪਸ false;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_cache_ਸੈੱਟ(bool has_multi_llcs) { }
#ਜੇ_ਅੰਤ

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
ਸਥਿਰ bool sd_ਵਿੱਚ_multi_llcs(ਬਣਤਰ sched_domain *sd)
{
	ਬਣਤਰ sched_domain *sdp = sd->ਮਾਪਾ;

	/* it does not make sense to aggregate to 1 CPU */
	ਜੇ (sd->ਵਿਸਤਾਰ_ਭਾਰ == 1)
		ਵਾਪਸ false;

	ਜਦੋਂ (sdp && sdp->ਵਿਸਤਾਰ_ਭਾਰ == sd->ਵਿਸਤਾਰ_ਭਾਰ)
		sdp = sdp->ਮਾਪਾ;

	ਵਾਪਸ !!sdp;
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
ਪੂਰਨ ਸਮੂਹ_balance_cpu(ਬਣਤਰ sched_ਸਮੂਹ *sg)
{
	ਵਾਪਸ cpumask_ਪਹਿਲਾ(ਸਮੂਹ_balance_ਨਕਾਬ(sg));
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
ਸਥਿਰ ਖਾਲੀ
build_balance_ਨਕਾਬ(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ sched_ਸਮੂਹ *sg, ਬਣਤਰ cpumask *ਨਕਾਬ)
{
	ਅਚਲ ਬਣਤਰ cpumask *sg_ਵਿਸਤਾਰ = sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg);
	ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = sd->ਨਿਜੀ;
	ਬਣਤਰ sched_domain *sibling;
	ਪੂਰਨ i;

	cpumask_clear(ਨਕਾਬ);

	for_each_cpu(i, sg_ਵਿਸਤਾਰ) {
		sibling = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		ਜੇ (!sibling->ਬੱਚਾ)
			ਜਾਰੀ;

		/* If we would not end up here, we can't continue from here */
		ਜੇ (!cpumask_equal(sg_ਵਿਸਤਾਰ, sched_domain_ਵਿਸਤਾਰ(sibling->ਬੱਚਾ)))
			ਜਾਰੀ;

		cpumask_ਸੈੱਟ_cpu(i, ਨਕਾਬ);
	}

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_ਖਾਲੀ_ਥਾਵਾਂ(ਨਕਾਬ));
}

/*
 * XXX: This creates per-node group entries; since the load-balancer will
 * immediately access remote memory to construct this group's load-balance
 * statistics having the groups node local is of dubious benefit.
 */
ਸਥਿਰ ਬਣਤਰ sched_ਸਮੂਹ *
build_ਸਮੂਹ_from_ਬੱਚਾ_sched_domain(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_ਸਮੂਹ *sg;
	ਬਣਤਰ cpumask *sg_ਵਿਸਤਾਰ;

	sg = kzalloc_ਗੰਢ(ਆਕਾਰ(ਬਣਤਰ sched_ਸਮੂਹ) + cpumask_ਮਾਪ(),
			GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu));

	ਜੇ (!sg)
		ਵਾਪਸ ਨਿਰਾ;

	sg_ਵਿਸਤਾਰ = sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg);
	ਜੇ (sd->ਬੱਚਾ) {
		cpumask_copy(sg_ਵਿਸਤਾਰ, sched_domain_ਵਿਸਤਾਰ(sd->ਬੱਚਾ));
		sg->flags = sd->ਬੱਚਾ->flags;
	} ਨਹੀਂਤਾਂ {
		cpumask_copy(sg_ਵਿਸਤਾਰ, sched_domain_ਵਿਸਤਾਰ(sd));
	}

	atomic_inc(&sg->ref);
	ਵਾਪਸ sg;
}

ਸਥਿਰ ਖਾਲੀ init_overlap_sched_ਸਮੂਹ(ਬਣਤਰ sched_domain *sd,
				     ਬਣਤਰ sched_ਸਮੂਹ *sg)
{
	ਬਣਤਰ cpumask *ਨਕਾਬ = sched_domains_tmpmask2;
	ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = sd->ਨਿਜੀ;
	ਬਣਤਰ cpumask *sg_ਵਿਸਤਾਰ;
	ਪੂਰਨ cpu;

	build_balance_ਨਕਾਬ(sd, sg, ਨਕਾਬ);
	cpu = cpumask_ਪਹਿਲਾ(ਨਕਾਬ);

	sg->sgc = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sgc, cpu);
	ਜੇ (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(ਸਮੂਹ_balance_ਨਕਾਬ(sg), ਨਕਾਬ);
	ਨਹੀਂਤਾਂ
		WARN_ON_ONCE(!cpumask_equal(ਸਮੂਹ_balance_ਨਕਾਬ(sg), ਨਕਾਬ));

	/*
	 * Initialize sgc->capacity such that even if we mess up the
	 * domains and no possible iteration will get us here, we won't
	 * die on a /0 trap.
	 */
	sg_ਵਿਸਤਾਰ = sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg);
	sg->sgc->ਸਮਰੱਥਾ = SCHED_ਸਮਰੱਥਾ_SCALE * cpumask_ਭਾਰ(sg_ਵਿਸਤਾਰ);
	sg->sgc->min_ਸਮਰੱਥਾ = SCHED_ਸਮਰੱਥਾ_SCALE;
	sg->sgc->max_ਸਮਰੱਥਾ = SCHED_ਸਮਰੱਥਾ_SCALE;
}

ਸਥਿਰ ਬਣਤਰ sched_domain *
ਲੱਭੋ_descended_sibling(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	ਜਦੋਂ (sibling->ਬੱਚਾ &&
	       !cpumask_subset(sched_domain_ਵਿਸਤਾਰ(sibling->ਬੱਚਾ),
			       sched_domain_ਵਿਸਤਾਰ(sd)))
		sibling = sibling->ਬੱਚਾ;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	ਜਦੋਂ (sibling->ਬੱਚਾ &&
	       cpumask_equal(sched_domain_ਵਿਸਤਾਰ(sibling->ਬੱਚਾ),
			     sched_domain_ਵਿਸਤਾਰ(sibling)))
		sibling = sibling->ਬੱਚਾ;

	ਵਾਪਸ sibling;
}

ਸਥਿਰ ਪੂਰਨ
build_overlap_sched_groups(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_ਸਮੂਹ *ਪਹਿਲਾ = ਨਿਰਾ, *ਆਖਰੀ = ਨਿਰਾ, *sg;
	ਅਚਲ ਬਣਤਰ cpumask *ਵਿਸਤਾਰ = sched_domain_ਵਿਸਤਾਰ(sd);
	ਬਣਤਰ cpumask *covered = sched_domains_tmpmask;
	ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = sd->ਨਿਜੀ;
	ਬਣਤਰ sched_domain *sibling;
	ਪੂਰਨ i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, ਵਿਸਤਾਰ, cpu) {
		ਬਣਤਰ cpumask *sg_ਵਿਸਤਾਰ;

		ਜੇ (cpumask_test_cpu(i, covered))
			ਜਾਰੀ;

		sibling = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, i);

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
		ਜੇ (!cpumask_test_cpu(i, sched_domain_ਵਿਸਤਾਰ(sibling)))
			ਜਾਰੀ;

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
		ਜੇ (sibling->ਬੱਚਾ &&
		    !cpumask_subset(sched_domain_ਵਿਸਤਾਰ(sibling->ਬੱਚਾ), ਵਿਸਤਾਰ))
			sibling = ਲੱਭੋ_descended_sibling(sd, sibling);

		sg = build_ਸਮੂਹ_from_ਬੱਚਾ_sched_domain(sibling, cpu);
		ਜੇ (!sg)
			ਜਾਓ fail;

		sg_ਵਿਸਤਾਰ = sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg);
		cpumask_or(covered, covered, sg_ਵਿਸਤਾਰ);

		init_overlap_sched_ਸਮੂਹ(sibling, sg);

		ਜੇ (!ਪਹਿਲਾ)
			ਪਹਿਲਾ = sg;
		ਜੇ (ਆਖਰੀ)
			ਆਖਰੀ->ਅਗਲਾ = sg;
		ਆਖਰੀ = sg;
		ਆਖਰੀ->ਅਗਲਾ = ਪਹਿਲਾ;
	}
	sd->groups = ਪਹਿਲਾ;

	ਵਾਪਸ 0;

fail:
	free_sched_groups(ਪਹਿਲਾ, 0);

	ਵਾਪਸ -ENOMEM;
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

ਸਥਿਰ ਬਣਤਰ sched_ਸਮੂਹ *get_ਸਮੂਹ(ਪੂਰਨ cpu, ਬਣਤਰ sd_ਅੰਕੜੇ *sdd)
{
	ਬਣਤਰ sched_domain *sd = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, cpu);
	ਬਣਤਰ sched_domain *ਬੱਚਾ = sd->ਬੱਚਾ;
	ਬਣਤਰ sched_ਸਮੂਹ *sg;
	bool already_ਦੇਖਿਆ;

	ਜੇ (ਬੱਚਾ)
		cpu = cpumask_ਪਹਿਲਾ(sched_domain_ਵਿਸਤਾਰ(ਬੱਚਾ));

	sg = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_ਦੇਖਿਆ = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_ਦੇਖਿਆ != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	ਜੇ (already_ਦੇਖਿਆ)
		ਵਾਪਸ sg;

	ਜੇ (ਬੱਚਾ) {
		cpumask_copy(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg), sched_domain_ਵਿਸਤਾਰ(ਬੱਚਾ));
		cpumask_copy(ਸਮੂਹ_balance_ਨਕਾਬ(sg), sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));
		sg->flags = ਬੱਚਾ->flags;
	} ਨਹੀਂਤਾਂ {
		cpumask_ਸੈੱਟ_cpu(cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));
		cpumask_ਸੈੱਟ_cpu(cpu, ਸਮੂਹ_balance_ਨਕਾਬ(sg));
	}

	sg->sgc->ਸਮਰੱਥਾ = SCHED_ਸਮਰੱਥਾ_SCALE * cpumask_ਭਾਰ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));
	sg->sgc->min_ਸਮਰੱਥਾ = SCHED_ਸਮਰੱਥਾ_SCALE;
	sg->sgc->max_ਸਮਰੱਥਾ = SCHED_ਸਮਰੱਥਾ_SCALE;

	ਵਾਪਸ sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
ਸਥਿਰ ਪੂਰਨ
build_sched_groups(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_ਸਮੂਹ *ਪਹਿਲਾ = ਨਿਰਾ, *ਆਖਰੀ = ਨਿਰਾ;
	ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = sd->ਨਿਜੀ;
	ਅਚਲ ਬਣਤਰ cpumask *ਵਿਸਤਾਰ = sched_domain_ਵਿਸਤਾਰ(sd);
	ਬਣਤਰ cpumask *covered;
	ਪੂਰਨ i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, ਵਿਸਤਾਰ, cpu) {
		ਬਣਤਰ sched_ਸਮੂਹ *sg;

		ਜੇ (cpumask_test_cpu(i, covered))
			ਜਾਰੀ;

		sg = get_ਸਮੂਹ(i, sdd);

		cpumask_or(covered, covered, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));

		ਜੇ (!ਪਹਿਲਾ)
			ਪਹਿਲਾ = sg;
		ਜੇ (ਆਖਰੀ)
			ਆਖਰੀ->ਅਗਲਾ = sg;
		ਆਖਰੀ = sg;
	}
	ਆਖਰੀ->ਅਗਲਾ = ਪਹਿਲਾ;
	sd->groups = ਪਹਿਲਾ;

	ਵਾਪਸ 0;
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
ਸਥਿਰ ਖਾਲੀ init_sched_groups_ਸਮਰੱਥਾ(ਪੂਰਨ cpu, ਬਣਤਰ sched_domain *sd)
{
	ਬਣਤਰ sched_ਸਮੂਹ *sg = sd->groups;
	ਬਣਤਰ cpumask *ਨਕਾਬ = sched_domains_tmpmask2;

	WARN_ON(!sg);

	ਕਰੋ {
		ਪੂਰਨ cpu, cores = 0, max_cpu = -1;

		sg->ਸਮੂਹ_ਭਾਰ = cpumask_ਭਾਰ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));

		cpumask_copy(ਨਕਾਬ, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));
		for_each_cpu(cpu, ਨਕਾਬ) {
			cores++;
			cpumask_andnot(ਨਕਾਬ, ਨਕਾਬ, cpu_smt_ਨਕਾਬ(cpu));
		}
		sg->cores = cores;

		ਜੇ (!(sd->flags & SD_ASYM_PACKING))
			ਜਾਓ ਅਗਲਾ;

		for_each_cpu(cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg)) {
			ਜੇ (max_cpu < 0)
				max_cpu = cpu;
			ਨਹੀਂਤਾਂ ਜੇ (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

ਅਗਲਾ:
		sg = sg->ਅਗਲਾ;
	} ਜਦੋਂ (sg != sd->groups);

	ਜੇ (cpu != ਸਮੂਹ_balance_cpu(sg))
		ਵਾਪਸ;

	update_ਸਮੂਹ_ਸਮਰੱਥਾ(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
ਖਾਲੀ sched_update_asym_prefer_cpu(ਪੂਰਨ cpu, ਪੂਰਨ old_prio, ਪੂਰਨ new_prio)
{
	ਪੂਰਨ asym_prefer_cpu = cpu;
	ਬਣਤਰ sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		ਬਣਤਰ sched_ਸਮੂਹ *sg;
		ਪੂਰਨ ਸਮੂਹ_cpu;

		ਜੇ (!(sd->flags & SD_ASYM_PACKING))
			ਜਾਰੀ;

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
		ਜੇ (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			ਜੇ (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				ਵਾਪਸ;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			ਜਾਰੀ;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		ਜੇ (new_prio >= old_prio)
			ਜਾਰੀ;

		for_each_cpu(ਸਮੂਹ_cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg)) {
			ਜੇ (sched_asym_prefer(ਸਮੂਹ_cpu, asym_prefer_cpu))
				asym_prefer_cpu = ਸਮੂਹ_cpu;
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
LIST_ਸਿਰ(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
asym_cpu_ਸਮਰੱਥਾ_classify(ਅਚਲ ਬਣਤਰ cpumask *sd_ਵਿਸਤਾਰ,
			   ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਬਣਤਰ asym_cap_ਅੰਕੜੇ *ਇੰਦਰਾਜ;
	ਪੂਰਨ ਗਿਣਤੀ_2 = 0, ਖੁੰਝ = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ, &asym_cap_list, link) {
		ਜੇ (cpumask_intersects(sd_ਵਿਸਤਾਰ, cpu_ਸਮਰੱਥਾ_ਵਿਸਤਾਰ(ਇੰਦਰਾਜ)))
			++ਗਿਣਤੀ_2;
		ਨਹੀਂਤਾਂ ਜੇ (cpumask_intersects(cpu_map, cpu_ਸਮਰੱਥਾ_ਵਿਸਤਾਰ(ਇੰਦਰਾਜ)))
			++ਖੁੰਝ;
	}

	WARN_ON_ONCE(!ਗਿਣਤੀ_2 && !list_ਖਾਲੀ_ਥਾਵਾਂ(&asym_cap_list));

	/* No asymmetry detected */
	ਜੇ (ਗਿਣਤੀ_2 < 2)
		ਵਾਪਸ 0;
	/* Some of the available CPU capacity values have not been detected */
	ਜੇ (ਖੁੰਝ)
		ਵਾਪਸ SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	ਵਾਪਸ SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_ਭਰਿਆ;

}

ਸਥਿਰ ਖਾਲੀ free_asym_cap_ਇੰਦਰਾਜ(ਬਣਤਰ rcu_ਸਿਰ *ਸਿਰ)
{
	ਬਣਤਰ asym_cap_ਅੰਕੜੇ *ਇੰਦਰਾਜ = container_ਦਾ(ਸਿਰ, ਬਣਤਰ asym_cap_ਅੰਕੜੇ, rcu);
	kfree(ਇੰਦਰਾਜ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ asym_cpu_ਸਮਰੱਥਾ_update_ਅੰਕੜੇ(ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ = arch_scale_cpu_ਸਮਰੱਥਾ(cpu);
	ਬਣਤਰ asym_cap_ਅੰਕੜੇ *ਪਾਓ_ਇੰਦਰਾਜ = ਨਿਰਾ;
	ਬਣਤਰ asym_cap_ਅੰਕੜੇ *ਇੰਦਰਾਜ;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ, &asym_cap_list, link) {
		ਜੇ (ਸਮਰੱਥਾ == ਇੰਦਰਾਜ->ਸਮਰੱਥਾ)
			ਜਾਓ ਪੂਰਾ;
		ਨਹੀਂਤਾਂ ਜੇ (!ਪਾਓ_ਇੰਦਰਾਜ && ਸਮਰੱਥਾ > ਇੰਦਰਾਜ->ਸਮਰੱਥਾ)
			ਪਾਓ_ਇੰਦਰਾਜ = list_prev_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ, link);
	}

	ਇੰਦਰਾਜ = kzalloc(ਆਕਾਰ(*ਇੰਦਰਾਜ) + cpumask_ਮਾਪ(), GFP_KERNEL);
	ਜੇ (WARN_ONCE(!ਇੰਦਰਾਜ, "Failed to allocate memory for asymmetry data\n"))
		ਵਾਪਸ;
	ਇੰਦਰਾਜ->ਸਮਰੱਥਾ = ਸਮਰੱਥਾ;

	/* If NULL then the new capacity is the smallest, add last. */
	ਜੇ (!ਪਾਓ_ਇੰਦਰਾਜ)
		list_ਜੋੜੋ_ਪੂਛ_rcu(&ਇੰਦਰਾਜ->link, &asym_cap_list);
	ਨਹੀਂਤਾਂ
		list_ਜੋੜੋ_rcu(&ਇੰਦਰਾਜ->link, &ਪਾਓ_ਇੰਦਰਾਜ->link);
ਪੂਰਾ:
	__cpumask_set_cpu(cpu, cpu_ਸਮਰੱਥਾ_ਵਿਸਤਾਰ(ਇੰਦਰਾਜ));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
ਸਥਿਰ ਖਾਲੀ asym_cpu_ਸਮਰੱਥਾ_ਜਾਂਚ(ਖਾਲੀ)
{
	ਬਣਤਰ asym_cap_ਅੰਕੜੇ *ਇੰਦਰਾਜ, *ਅਗਲਾ;
	ਪੂਰਨ cpu;

	list_for_each_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ, &asym_cap_list, link)
		cpumask_clear(cpu_ਸਮਰੱਥਾ_ਵਿਸਤਾਰ(ਇੰਦਰਾਜ));

	for_each_cpu_and(cpu, cpu_possible_ਨਕਾਬ, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_ਸਮਰੱਥਾ_update_ਅੰਕੜੇ(cpu);

	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(ਇੰਦਰਾਜ, ਅਗਲਾ, &asym_cap_list, link) {
		ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(cpu_ਸਮਰੱਥਾ_ਵਿਸਤਾਰ(ਇੰਦਰਾਜ))) {
			list_del_rcu(&ਇੰਦਰਾਜ->link);
			call_rcu(&ਇੰਦਰਾਜ->rcu, free_asym_cap_ਇੰਦਰਾਜ);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	ਜੇ (list_is_singular(&asym_cap_list)) {
		ਇੰਦਰਾਜ = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&asym_cap_list, typeof(*ਇੰਦਰਾਜ), link);
		list_del_rcu(&ਇੰਦਰਾਜ->link);
		call_rcu(&ਇੰਦਰਾਜ->rcu, free_asym_cap_ਇੰਦਰਾਜ);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

ਸਥਿਰ ਪੂਰਨ default_relax_domain_level = -1;
ਪੂਰਨ sched_domain_level_max;

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_relax_domain_level(ਅੱਖਰ *str)
{
	ਜੇ (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	ਵਾਪਸ 1;
}
__setup("relax_domain_level=", ਤਿਆਰੀ_relax_domain_level);

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_domain_attribute(ਬਣਤਰ sched_domain *sd,
				 ਬਣਤਰ sched_domain_attr *attr)
{
	ਪੂਰਨ request;

	ਜੇ (!attr || attr->relax_domain_level < 0) {
		ਜੇ (default_relax_domain_level < 0)
			ਵਾਪਸ;
		request = default_relax_domain_level;
	} ਨਹੀਂਤਾਂ
		request = attr->relax_domain_level;

	ਜੇ (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

ਸਥਿਰ ਖਾਲੀ __sdt_free(ਅਚਲ ਬਣਤਰ cpumask *cpu_map);
ਸਥਿਰ ਪੂਰਨ __sdt_alloc(ਅਚਲ ਬਣਤਰ cpumask *cpu_map);

ਸਥਿਰ ਖਾਲੀ __sds_free(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਅਚਲ ਬਣਤਰ cpumask *cpu_map);
ਸਥਿਰ ਪੂਰਨ __sds_alloc(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਅਚਲ ਬਣਤਰ cpumask *cpu_map);

ਸਥਿਰ ਖਾਲੀ __free_domain_allocs(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਗਿਣਤੀ s_alloc what,
				 ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਚੋਣ (what) {
	ਹਾਲਤ sa_rootdomain:
		ਜੇ (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	ਹਾਲਤ sa_sd:
		free_percpu(d->sd);
		fallthrough;
	ਹਾਲਤ sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	ਹਾਲਤ sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	ਹਾਲਤ sa_none:
		ਰੋਕੋ;
	}
}

ਸਥਿਰ ਗਿਣਤੀ s_alloc
__visit_domain_allocation_hell(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	memset(d, 0, ਆਕਾਰ(*d));

	ਜੇ (__sdt_alloc(cpu_map))
		ਵਾਪਸ sa_sd_storage;
	ਜੇ (__sds_alloc(d, cpu_map))
		ਵਾਪਸ sa_sd_shared;
	d->sd = alloc_percpu(ਬਣਤਰ sched_domain *);
	ਜੇ (!d->sd)
		ਵਾਪਸ sa_sd_shared;
	d->rd = alloc_rootdomain();
	ਜੇ (!d->rd)
		ਵਾਪਸ sa_sd;

	ਵਾਪਸ sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
ਸਥਿਰ ਖਾਲੀ claim_allocations(ਪੂਰਨ cpu, ਬਣਤਰ s_ਅੰਕੜੇ *d)
{
	ਬਣਤਰ sched_domain *sd;

	ਜੇ (atomic_read(&(*ਪ੍ਰਤੀ_cpu_ptr(d->sds, cpu))->ref))
		*ਪ੍ਰਤੀ_cpu_ptr(d->sds, cpu) = ਨਿਰਾ;

	ਲਈ (sd = *ਪ੍ਰਤੀ_cpu_ptr(d->sd, cpu); sd; sd = sd->ਮਾਪਾ) {
		ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = sd->ਨਿਜੀ;

		WARN_ON_ONCE(*ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, cpu) != sd);
		*ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, cpu) = ਨਿਰਾ;

		ਜੇ (atomic_read(&(*ਪ੍ਰਤੀ_cpu_ptr(sdd->sg, cpu))->ref))
			*ਪ੍ਰਤੀ_cpu_ptr(sdd->sg, cpu) = ਨਿਰਾ;

		ਜੇ (atomic_read(&(*ਪ੍ਰਤੀ_cpu_ptr(sdd->sgc, cpu))->ref))
			*ਪ੍ਰਤੀ_cpu_ptr(sdd->sgc, cpu) = ਨਿਰਾ;
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA
ਗਿਣਤੀ numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
ਸਥਿਰ ਪੂਰਨ			sched_domains_numa_ਪੱਧਰ;
ਸਥਿਰ ਪੂਰਨ			sched_numa_ਗੰਢ_ਪੱਧਰ;

ਪੂਰਨ				sched_max_numa_ਦੂਰੀ;
ਸਥਿਰ ਪੂਰਨ			*sched_domains_numa_ਦੂਰੀ;
ਸਥਿਰ ਪੂਰਨ			*sched_numa_ਗੰਢ_ਦੂਰੀ;
ਸਥਿਰ ਬਣਤਰ cpumask		***sched_domains_numa_masks;
#ਜੇ_ਅੰਤ /* CONFIG_NUMA */

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

ਸਥਿਰ ਬਣਤਰ sched_domain *
sd_init(ਬਣਤਰ sched_domain_topology_level *tl,
	ਅਚਲ ਬਣਤਰ cpumask *cpu_map,
	ਬਣਤਰ sched_domain *ਬੱਚਾ, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = &tl->ਅੰਕੜੇ;
	ਬਣਤਰ sched_domain *sd = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, cpu);
	ਪੂਰਨ sd_id, sd_ਭਾਰ, sd_flags = 0;
	ਬਣਤਰ cpumask *sd_ਵਿਸਤਾਰ;
	u64 now = sched_clock();

	sd_ਵਿਸਤਾਰ = sched_domain_ਵਿਸਤਾਰ(sd);
	cpumask_and(sd_ਵਿਸਤਾਰ, cpu_map, tl->ਨਕਾਬ(tl, cpu));
	sd_ਭਾਰ = cpumask_ਭਾਰ(sd_ਵਿਸਤਾਰ);
	sd_id = cpumask_ਪਹਿਲਾ(sd_ਵਿਸਤਾਰ);

	ਜੇ (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	ਜੇ (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_SMT
	ਜੇ (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#ਜੇ_ਅੰਤ
	sd_flags |= asym_cpu_ਸਮਰੱਥਾ_classify(sd_ਵਿਸਤਾਰ, cpu_map);

	*sd = (ਬਣਤਰ sched_domain){
		.min_interval		= sd_ਭਾਰ,
		.max_interval		= 2*sd_ਭਾਰ,
		.busy_ਗੁਣਕ		= 16,
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

		.ਆਖਰੀ_balance		= jiffies,
		.balance_interval	= sd_ਭਾਰ,

		/* 50% success rate */
		.newidle_call		= 512,
		.newidle_success	= 256,
		.newidle_ਅਨੁਪਾਤ		= 512,
		.newidle_stamp		= now,

		.max_newidle_lb_ਲਾਗਤ	= 0,
		.ਆਖਰੀ_decay_max_lb_ਲਾਗਤ	= jiffies,
		.ਬੱਚਾ			= ਬੱਚਾ,
		.name			= tl->name,
	};

	WARN_ONCE((sd->flags & (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY)) ==
		  (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY),
		  "CPU capacity asymmetry not supported on SMT\n");

	/*
	 * Convert topological properties into behaviour.
	 */
	ਜੇ (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} ਨਹੀਂਤਾਂ ਜੇ (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA
	} ਨਹੀਂਤਾਂ ਜੇ (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		ਜੇ (sched_domains_numa_ਦੂਰੀ[tl->numa_level] > ਗੰਢ_reclaim_ਦੂਰੀ) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#ਜੇ_ਅੰਤ /* CONFIG_NUMA */
	} ਨਹੀਂਤਾਂ {
		sd->cache_nice_tries = 1;
	}

	sd->ਨਿਜੀ = sdd;

	ਵਾਪਸ sd;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_SMT
ਪੂਰਨ cpu_smt_flags(ਖਾਲੀ)
{
	ਵਾਪਸ SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ਅਚਲ ਬਣਤਰ cpumask *tl_smt_ਨਕਾਬ(ਬਣਤਰ sched_domain_topology_level *tl, ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_smt_ਨਕਾਬ(cpu);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLUSTER
ਪੂਰਨ cpu_cluster_flags(ਖਾਲੀ)
{
	ਵਾਪਸ SD_CLUSTER | SD_SHARE_LLC;
}

ਅਚਲ ਬਣਤਰ cpumask *tl_cls_ਨਕਾਬ(ਬਣਤਰ sched_domain_topology_level *tl, ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_clustergroup_ਨਕਾਬ(cpu);
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_MC
ਪੂਰਨ cpu_core_flags(ਖਾਲੀ)
{
	ਵਾਪਸ SD_SHARE_LLC;
}

ਅਚਲ ਬਣਤਰ cpumask *tl_mc_ਨਕਾਬ(ਬਣਤਰ sched_domain_topology_level *tl, ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_coregroup_ਨਕਾਬ(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_ਨਕਾਬ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arch_llc_ਨਕਾਬ(cpu) cpu_coregroup_ਨਕਾਬ(cpu)
# endif

#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ arch_llc_ਨਕਾਬ(cpu) cpumask_ਦਾ(cpu)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ llc_ਨਕਾਬ(cpu) arch_llc_ਨਕਾਬ(cpu)

ਅਚਲ ਬਣਤਰ cpumask *tl_pkg_ਨਕਾਬ(ਬਣਤਰ sched_domain_topology_level *tl, ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_ਗੰਢ_ਨਕਾਬ(cpu);
}

/*
 * Topology list, bottom-up.
 */
ਸਥਿਰ ਬਣਤਰ sched_domain_topology_level default_topology[] = {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_ਨਕਾਬ, cpu_smt_flags, SMT),
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_ਨਕਾਬ, cpu_cluster_flags, CLS),
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_ਨਕਾਬ, cpu_core_flags, MC),
#ਜੇ_ਅੰਤ
	SDTL_INIT(tl_pkg_ਨਕਾਬ, ਨਿਰਾ, PKG),
	{ ਨਿਰਾ, },
};

ਸਥਿਰ ਬਣਤਰ sched_domain_topology_level *sched_domain_topology =
	default_topology;
ਸਥਿਰ ਬਣਤਰ sched_domain_topology_level *sched_domain_topology_saved;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_sd_topology(tl)			\
	ਲਈ (tl = sched_domain_topology; tl->ਨਕਾਬ; tl++)

ਖਾਲੀ __init ਸੈੱਟ_sched_topology(ਬਣਤਰ sched_domain_topology_level *tl)
{
	ਜੇ (WARN_ON_ONCE(sched_smp_initialized))
		ਵਾਪਸ;

	sched_domain_topology = tl;
	sched_domain_topology_saved = ਨਿਰਾ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA
ਸਥਿਰ ਪੂਰਨ cpu_numa_flags(ਖਾਲੀ)
{
	ਵਾਪਸ SD_NUMA;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ cpumask *sd_numa_ਨਕਾਬ(ਬਣਤਰ sched_domain_topology_level *tl, ਪੂਰਨ cpu)
{
	ਵਾਪਸ sched_domains_numa_masks[tl->numa_level][cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu)];
}

ਸਥਿਰ ਖਾਲੀ sched_numa_warn(ਅਚਲ ਅੱਖਰ *str)
{
	ਸਥਿਰ ਪੂਰਨ ਪੂਰਾ = false;
	ਪੂਰਨ i,j;

	ਜੇ (ਪੂਰਾ)
		ਵਾਪਸ;

	ਪੂਰਾ = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	ਲਈ (i = 0; i < nr_ਗੰਢ_ids; i++) {
		printk(KERN_WARNING "  ");
		ਲਈ (j = 0; j < nr_ਗੰਢ_ids; j++) {
			ਜੇ (!ਗੰਢ_state(i, N_CPU) || !ਗੰਢ_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", ਗੰਢ_ਦੂਰੀ(i,j));
			ਨਹੀਂਤਾਂ
				printk(KERN_CONT " %02d  ", ਗੰਢ_ਦੂਰੀ(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool ਲੱਭੋ_numa_ਦੂਰੀ(ਪੂਰਨ ਦੂਰੀ)
{
	bool ਮਿਲਿਆ = false;
	ਪੂਰਨ i, *distances;

	ਜੇ (ਦੂਰੀ == ਗੰਢ_ਦੂਰੀ(0, 0))
		ਵਾਪਸ true;

	rcu_read_ਤਾਲਾ();
	distances = rcu_dereference(sched_numa_ਗੰਢ_ਦੂਰੀ);
	ਜੇ (!distances)
		ਜਾਓ unlock;
	ਲਈ (i = 0; i < sched_numa_ਗੰਢ_ਪੱਧਰ; i++) {
		ਜੇ (distances[i] == ਦੂਰੀ) {
			ਮਿਲਿਆ = true;
			ਰੋਕੋ;
		}
	}
unlock:
	rcu_read_unlock();

	ਵਾਪਸ ਮਿਲਿਆ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_cpu_ਗੰਢ_but(n, nbut)		\
	for_each_ਗੰਢ_state(n, N_CPU)		\
		ਜੇ (n == nbut)			\
			ਜਾਰੀ;		\
		ਨਹੀਂਤਾਂ

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
ਸਥਿਰ ਖਾਲੀ init_numa_topology_type(ਪੂਰਨ offline_ਗੰਢ)
{
	ਪੂਰਨ a, b, c, n;

	n = sched_max_numa_ਦੂਰੀ;

	ਜੇ (sched_domains_numa_ਪੱਧਰ <= 2) {
		sched_numa_topology_type = NUMA_ਸਿੱਧਾ;
		ਵਾਪਸ;
	}

	for_each_cpu_ਗੰਢ_but(a, offline_ਗੰਢ) {
		for_each_cpu_ਗੰਢ_but(b, offline_ਗੰਢ) {
			/* Find two nodes furthest removed from each other. */
			ਜੇ (ਗੰਢ_ਦੂਰੀ(a, b) < n)
				ਜਾਰੀ;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_ਗੰਢ_but(c, offline_ਗੰਢ) {
				ਜੇ (ਗੰਢ_ਦੂਰੀ(a, c) < n &&
				    ਗੰਢ_ਦੂਰੀ(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					ਵਾਪਸ;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			ਵਾਪਸ;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_ਸਿੱਧਾ;
}


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NR_ਦੂਰੀ_ਮੁੱਲਾਂ (1 << ਦੂਰੀ_ਬਿੱਟ)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
ਸਥਿਰ ਪੂਰਨ numa_ਗੰਢ_dist(ਪੂਰਨ i, ਪੂਰਨ j)
{
	ਵਾਪਸ ਗੰਢ_ਦੂਰੀ(i, j);
}

ਪੂਰਨ arch_sched_ਗੰਢ_ਦੂਰੀ(ਪੂਰਨ from, ਪੂਰਨ ਮੰਜ਼ਿਲ)
			     __weak __alias(numa_ਗੰਢ_dist);

ਸਥਿਰ bool modified_sched_ਗੰਢ_ਦੂਰੀ(ਖਾਲੀ)
{
	ਵਾਪਸ numa_ਗੰਢ_dist != arch_sched_ਗੰਢ_ਦੂਰੀ;
}

ਸਥਿਰ ਪੂਰਨ sched_ਰਿਕਾਰਡ_numa_dist(ਪੂਰਨ offline_ਗੰਢ, ਪੂਰਨ (*n_dist)(ਪੂਰਨ, ਪੂਰਨ),
				  ਪੂਰਨ **dist, ਪੂਰਨ *ਪੱਧਰ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਦੂਰੀ_map __free(bitmap) = ਨਿਰਾ;
	ਪੂਰਨ nr_ਪੱਧਰ = 0;
	ਪੂਰਨ i, j;
	ਪੂਰਨ *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	ਦੂਰੀ_map = bitmap_alloc(NR_ਦੂਰੀ_ਮੁੱਲਾਂ, GFP_KERNEL);
	ਜੇ (!ਦੂਰੀ_map)
		ਵਾਪਸ -ENOMEM;

	bitmap_zero(ਦੂਰੀ_map, NR_ਦੂਰੀ_ਮੁੱਲਾਂ);
	for_each_cpu_ਗੰਢ_but(i, offline_ਗੰਢ) {
		for_each_cpu_ਗੰਢ_but(j, offline_ਗੰਢ) {
			ਪੂਰਨ ਦੂਰੀ = n_dist(i, j);

			ਜੇ (ਦੂਰੀ < LOCAL_ਦੂਰੀ || ਦੂਰੀ >= NR_ਦੂਰੀ_ਮੁੱਲਾਂ) {
				sched_numa_warn("Invalid distance value range");
				ਵਾਪਸ -EINVAL;
			}

			bitmap_ਸੈੱਟ(ਦੂਰੀ_map, ਦੂਰੀ, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_ਪੱਧਰ = bitmap_ਭਾਰ(ਦੂਰੀ_map, NR_ਦੂਰੀ_ਮੁੱਲਾਂ);

	distances = kzalloc_objs(ਪੂਰਨ, nr_ਪੱਧਰ);
	ਜੇ (!distances)
		ਵਾਪਸ -ENOMEM;

	ਲਈ (i = 0, j = 0; i < nr_ਪੱਧਰ; i++, j++) {
		j = ਲੱਭੋ_ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਦੂਰੀ_map, NR_ਦੂਰੀ_ਮੁੱਲਾਂ, j);
		distances[i] = j;
	}
	*dist = distances;
	*ਪੱਧਰ = nr_ਪੱਧਰ;

	ਵਾਪਸ 0;
}

ਖਾਲੀ sched_init_numa(ਪੂਰਨ offline_ਗੰਢ)
{
	ਬਣਤਰ sched_domain_topology_level *tl;
	ਪੂਰਨ nr_ਪੱਧਰ, nr_ਗੰਢ_ਪੱਧਰ;
	ਪੂਰਨ i, j;
	ਪੂਰਨ *distances, *domain_distances;
	ਬਣਤਰ cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	ਜੇ (sched_ਰਿਕਾਰਡ_numa_dist(offline_ਗੰਢ, numa_ਗੰਢ_dist, &distances,
				   &nr_ਗੰਢ_ਪੱਧਰ))
		ਵਾਪਸ;

	/* Record modified NUMA distances for building sched domains */
	ਜੇ (modified_sched_ਗੰਢ_ਦੂਰੀ()) {
		ਜੇ (sched_ਰਿਕਾਰਡ_numa_dist(offline_ਗੰਢ, arch_sched_ਗੰਢ_ਦੂਰੀ,
					   &domain_distances, &nr_ਪੱਧਰ)) {
			kfree(distances);
			ਵਾਪਸ;
		}
	} ਨਹੀਂਤਾਂ {
		domain_distances = distances;
		nr_ਪੱਧਰ = nr_ਗੰਢ_ਪੱਧਰ;
	}
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(sched_numa_ਗੰਢ_ਦੂਰੀ, distances);
	WRITE_ONCE(sched_max_numa_ਦੂਰੀ, distances[nr_ਗੰਢ_ਪੱਧਰ - 1]);
	WRITE_ONCE(sched_numa_ਗੰਢ_ਪੱਧਰ, nr_ਗੰਢ_ਪੱਧਰ);

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
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(sched_domains_numa_ਦੂਰੀ, domain_distances);

	sched_domains_numa_ਪੱਧਰ = 0;

	masks = kzalloc(ਆਕਾਰ(ਖਾਲੀ *) * nr_ਪੱਧਰ, GFP_KERNEL);
	ਜੇ (!masks)
		ਵਾਪਸ;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	ਲਈ (i = 0; i < nr_ਪੱਧਰ; i++) {
		masks[i] = kzalloc(nr_ਗੰਢ_ids * ਆਕਾਰ(ਖਾਲੀ *), GFP_KERNEL);
		ਜੇ (!masks[i])
			ਵਾਪਸ;

		for_each_cpu_ਗੰਢ_but(j, offline_ਗੰਢ) {
			ਬਣਤਰ cpumask *ਨਕਾਬ = kzalloc(cpumask_ਮਾਪ(), GFP_KERNEL);
			ਪੂਰਨ k;

			ਜੇ (!ਨਕਾਬ)
				ਵਾਪਸ;

			masks[i][j] = ਨਕਾਬ;

			for_each_cpu_ਗੰਢ_but(k, offline_ਗੰਢ) {
				ਜੇ (sched_debug() &&
				    (arch_sched_ਗੰਢ_ਦੂਰੀ(j, k) !=
				     arch_sched_ਗੰਢ_ਦੂਰੀ(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				ਜੇ (arch_sched_ਗੰਢ_ਦੂਰੀ(j, k) >
				    sched_domains_numa_ਦੂਰੀ[i])
					ਜਾਰੀ;

				cpumask_or(ਨਕਾਬ, ਨਕਾਬ, cpumask_ਦਾ_ਗੰਢ(k));
			}
		}
	}
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	ਲਈ (i = 0; sched_domain_topology[i].ਨਕਾਬ; i++);

	tl = kzalloc((i + nr_ਪੱਧਰ + 1) *
			ਆਕਾਰ(ਬਣਤਰ sched_domain_topology_level), GFP_KERNEL);
	ਜੇ (!tl)
		ਵਾਪਸ;

	/*
	 * Copy the default topology bits..
	 */
	ਲਈ (i = 0; sched_domain_topology[i].ਨਕਾਬ; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_ਨਕਾਬ, ਨਿਰਾ, ਗੰਢ_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	ਲਈ (j = 1; j < nr_ਪੱਧਰ; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_ਨਕਾਬ, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_ਪੱਧਰ = nr_ਪੱਧਰ;

	init_numa_topology_type(offline_ਗੰਢ);
}


ਸਥਿਰ ਖਾਲੀ sched_reset_numa(ਖਾਲੀ)
{
	ਪੂਰਨ nr_ਪੱਧਰ, *distances, *dom_distances = ਨਿਰਾ;
	ਬਣਤਰ cpumask ***masks;

	nr_ਪੱਧਰ = sched_domains_numa_ਪੱਧਰ;
	sched_numa_ਗੰਢ_ਪੱਧਰ = 0;
	sched_domains_numa_ਪੱਧਰ = 0;
	sched_max_numa_ਦੂਰੀ = 0;
	sched_numa_topology_type = NUMA_ਸਿੱਧਾ;
	distances = sched_numa_ਗੰਢ_ਦੂਰੀ;
	ਜੇ (sched_numa_ਗੰਢ_ਦੂਰੀ != sched_domains_numa_ਦੂਰੀ)
		dom_distances = sched_domains_numa_ਦੂਰੀ;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(sched_numa_ਗੰਢ_ਦੂਰੀ, ਨਿਰਾ);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(sched_domains_numa_ਦੂਰੀ, ਨਿਰਾ);
	masks = sched_domains_numa_masks;
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(sched_domains_numa_masks, ਨਿਰਾ);
	ਜੇ (distances || masks) {
		ਪੂਰਨ i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		ਲਈ (i = 0; i < nr_ਪੱਧਰ && masks; i++) {
			ਜੇ (!masks[i])
				ਜਾਰੀ;
			for_each_ਗੰਢ(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	ਜੇ (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = ਨਿਰਾ;
	}
}

/*
 * Call with hotplug lock held
 */
ਖਾਲੀ sched_update_numa(ਪੂਰਨ cpu, bool online)
{
	ਪੂਰਨ ਗੰਢ;

	ਗੰਢ = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	ਜੇ (cpumask_ਭਾਰ(cpumask_ਦਾ_ਗੰਢ(ਗੰਢ)) != 1)
		ਵਾਪਸ;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_ਗੰਢ : ਗੰਢ);
}

ਖਾਲੀ sched_domains_numa_masks_ਸੈੱਟ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਪੂਰਨ ਗੰਢ = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);
	ਪੂਰਨ i, j;

	ਲਈ (i = 0; i < sched_domains_numa_ਪੱਧਰ; i++) {
		ਲਈ (j = 0; j < nr_ਗੰਢ_ids; j++) {
			ਜੇ (!ਗੰਢ_state(j, N_CPU))
				ਜਾਰੀ;

			/* Set ourselves in the remote node's masks */
			ਜੇ (arch_sched_ਗੰਢ_ਦੂਰੀ(j, ਗੰਢ) <=
			    sched_domains_numa_ਦੂਰੀ[i])
				cpumask_ਸੈੱਟ_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

ਖਾਲੀ sched_domains_numa_masks_clear(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਪੂਰਨ i, j;

	ਲਈ (i = 0; i < sched_domains_numa_ਪੱਧਰ; i++) {
		ਲਈ (j = 0; j < nr_ਗੰਢ_ids; j++) {
			ਜੇ (sched_domains_numa_masks[i][j])
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
ਪੂਰਨ sched_numa_ਲੱਭੋ_closest(ਅਚਲ ਬਣਤਰ cpumask *cpus, ਪੂਰਨ cpu)
{
	ਪੂਰਨ i, j = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu), ਮਿਲਿਆ = nr_cpu_ids;
	ਬਣਤਰ cpumask ***masks;

	rcu_read_ਤਾਲਾ();
	masks = rcu_dereference(sched_domains_numa_masks);
	ਜੇ (!masks)
		ਜਾਓ unlock;
	ਲਈ (i = 0; i < sched_domains_numa_ਪੱਧਰ; i++) {
		ਜੇ (!masks[i][j])
			ਰੋਕੋ;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		ਜੇ (cpu < nr_cpu_ids) {
			ਮਿਲਿਆ = cpu;
			ਰੋਕੋ;
		}
	}
unlock:
	rcu_read_unlock();

	ਵਾਪਸ ਮਿਲਿਆ;
}

ਬਣਤਰ __cmp_key {
	ਅਚਲ ਬਣਤਰ cpumask *cpus;
	ਬਣਤਰ cpumask ***masks;
	ਪੂਰਨ ਗੰਢ;
	ਪੂਰਨ cpu;
	ਪੂਰਨ w;
};

ਸਥਿਰ ਪੂਰਨ hop_cmp(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
	ਬਣਤਰ cpumask **prev_hop, **cur_hop = *(ਬਣਤਰ cpumask ***)b;
	ਬਣਤਰ __cmp_key *k = (ਬਣਤਰ __cmp_key *)a;

	ਜੇ (cpumask_ਭਾਰ_and(k->cpus, cur_hop[k->ਗੰਢ]) <= k->cpu)
		ਵਾਪਸ 1;

	ਜੇ (b == k->masks) {
		k->w = 0;
		ਵਾਪਸ 0;
	}

	prev_hop = *((ਬਣਤਰ cpumask ***)b - 1);
	k->w = cpumask_ਭਾਰ_and(k->cpus, prev_hop[k->ਗੰਢ]);
	ਜੇ (k->w <= k->cpu)
		ਵਾਪਸ 0;

	ਵਾਪਸ -1;
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
ਪੂਰਨ sched_numa_ਲੱਭੋ_nth_cpu(ਅਚਲ ਬਣਤਰ cpumask *cpus, ਪੂਰਨ cpu, ਪੂਰਨ ਗੰਢ)
{
	ਬਣਤਰ __cmp_key k = { .cpus = cpus, .cpu = cpu };
	ਬਣਤਰ cpumask ***hop_masks;
	ਪੂਰਨ hop, ret = nr_cpu_ids;

	ਜੇ (ਗੰਢ == NUMA_NO_ਗੰਢ)
		ਵਾਪਸ cpumask_nth_and(cpu, cpus, cpu_online_ਨਕਾਬ);

	rcu_read_ਤਾਲਾ();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	ਗੰਢ = numa_ਸਭ_ਤੋਂ_ਨੇੜੇ_ਗੰਢ(ਗੰਢ, N_CPU);
	k.ਗੰਢ = ਗੰਢ;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	ਜੇ (!k.masks)
		ਜਾਓ unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_ਪੱਧਰ, ਆਕਾਰ(k.masks[0]), hop_cmp);
	ਜੇ (!hop_masks)
		ਜਾਓ unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][ਗੰਢ], k.masks[hop-1][ਗੰਢ]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][ਗੰਢ]);
unlock:
	rcu_read_unlock();
	ਵਾਪਸ ret;
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_numa_ਲੱਭੋ_nth_cpu);

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
ਅਚਲ ਬਣਤਰ cpumask *sched_numa_hop_ਨਕਾਬ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗੰਢ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hops)
{
	ਬਣਤਰ cpumask ***masks;

	ਜੇ (ਗੰਢ >= nr_ਗੰਢ_ids || hops >= sched_domains_numa_ਪੱਧਰ)
		ਵਾਪਸ ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	ਜੇ (!masks)
		ਵਾਪਸ ERR_PTR(-EBUSY);

	ਵਾਪਸ masks[hops][ਗੰਢ];
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_numa_hop_ਨਕਾਬ);

#ਜੇ_ਅੰਤ /* CONFIG_NUMA */

ਸਥਿਰ ਪੂਰਨ __sdt_alloc(ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਬਣਤਰ sched_domain_topology_level *tl;
	ਪੂਰਨ j;

	for_each_sd_topology(tl) {
		ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = &tl->ਅੰਕੜੇ;

		sdd->sd = alloc_percpu(ਬਣਤਰ sched_domain *);
		ਜੇ (!sdd->sd)
			ਵਾਪਸ -ENOMEM;

		sdd->sg = alloc_percpu(ਬਣਤਰ sched_ਸਮੂਹ *);
		ਜੇ (!sdd->sg)
			ਵਾਪਸ -ENOMEM;

		sdd->sgc = alloc_percpu(ਬਣਤਰ sched_ਸਮੂਹ_ਸਮਰੱਥਾ *);
		ਜੇ (!sdd->sgc)
			ਵਾਪਸ -ENOMEM;

		for_each_cpu(j, cpu_map) {
			ਬਣਤਰ sched_domain *sd;
			ਬਣਤਰ sched_ਸਮੂਹ *sg;
			ਬਣਤਰ sched_ਸਮੂਹ_ਸਮਰੱਥਾ *sgc;

			sd = kzalloc_ਗੰਢ(ਆਕਾਰ(ਬਣਤਰ sched_domain) + cpumask_ਮਾਪ(),
					GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(j));
			ਜੇ (!sd)
				ਵਾਪਸ -ENOMEM;

			*ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_ਗੰਢ(ਆਕਾਰ(ਬਣਤਰ sched_ਸਮੂਹ) + cpumask_ਮਾਪ(),
					GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(j));
			ਜੇ (!sg)
				ਵਾਪਸ -ENOMEM;

			sg->ਅਗਲਾ = sg;

			*ਪ੍ਰਤੀ_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_ਗੰਢ(ਆਕਾਰ(ਬਣਤਰ sched_ਸਮੂਹ_ਸਮਰੱਥਾ) + cpumask_ਮਾਪ(),
					GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(j));
			ਜੇ (!sgc)
				ਵਾਪਸ -ENOMEM;

			sgc->id = j;

			*ਪ੍ਰਤੀ_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ __sdt_free(ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਬਣਤਰ sched_domain_topology_level *tl;
	ਪੂਰਨ j;

	for_each_sd_topology(tl) {
		ਬਣਤਰ sd_ਅੰਕੜੇ *sdd = &tl->ਅੰਕੜੇ;

		for_each_cpu(j, cpu_map) {
			ਬਣਤਰ sched_domain *sd;

			ਜੇ (sdd->sd) {
				sd = *ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, j);
				ਜੇ (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*ਪ੍ਰਤੀ_cpu_ptr(sdd->sd, j));
			}

			ਜੇ (sdd->sg)
				kfree(*ਪ੍ਰਤੀ_cpu_ptr(sdd->sg, j));
			ਜੇ (sdd->sgc)
				kfree(*ਪ੍ਰਤੀ_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = ਨਿਰਾ;
		free_percpu(sdd->sg);
		sdd->sg = ਨਿਰਾ;
		free_percpu(sdd->sgc);
		sdd->sgc = ਨਿਰਾ;
	}
}

ਸਥਿਰ ਪੂਰਨ __sds_alloc(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਪੂਰਨ j;

	d->sds = alloc_percpu(ਬਣਤਰ sched_domain_shared *);
	ਜੇ (!d->sds)
		ਵਾਪਸ -ENOMEM;

	for_each_cpu(j, cpu_map) {
		ਬਣਤਰ sched_domain_shared *sds;

		sds = kzalloc_ਗੰਢ(ਆਕਾਰ(ਬਣਤਰ sched_domain_shared),
				GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(j));
		ਜੇ (!sds)
			ਵਾਪਸ -ENOMEM;

		*ਪ੍ਰਤੀ_cpu_ptr(d->sds, j) = sds;
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ __sds_free(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਪੂਰਨ j;

	ਜੇ (!d->sds)
		ਵਾਪਸ;

	for_each_cpu(j, cpu_map)
		kfree(*ਪ੍ਰਤੀ_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = ਨਿਰਾ;
}

ਸਥਿਰ ਬਣਤਰ sched_domain *build_sched_domain(ਬਣਤਰ sched_domain_topology_level *tl,
		ਅਚਲ ਬਣਤਰ cpumask *cpu_map, ਬਣਤਰ sched_domain_attr *attr,
		ਬਣਤਰ sched_domain *ਬੱਚਾ, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain *sd = sd_init(tl, cpu_map, ਬੱਚਾ, cpu);

	ਜੇ (ਬੱਚਾ) {
		sd->level = ਬੱਚਾ->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		ਬੱਚਾ->ਮਾਪਾ = sd;

		ਜੇ (!cpumask_subset(sched_domain_ਵਿਸਤਾਰ(ਬੱਚਾ),
				    sched_domain_ਵਿਸਤਾਰ(sd))) {
			pr_err("BUG: arch topology borken\n");
			pr_err("     the %s domain not a subset of the %s domain\n",
					ਬੱਚਾ->name, sd->name);
			/* Fixup, ensure @sd has at least @child CPUs. */
			cpumask_or(sched_domain_ਵਿਸਤਾਰ(sd),
				   sched_domain_ਵਿਸਤਾਰ(sd),
				   sched_domain_ਵਿਸਤਾਰ(ਬੱਚਾ));
		}

	}
	ਸੈੱਟ_domain_attribute(sd, attr);

	ਵਾਪਸ sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
ਸਥਿਰ bool topology_ਵਿਸਤਾਰ_sane(ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਬਣਤਰ sched_domain_topology_level *tl;
	ਬਣਤਰ cpumask *covered, *id_ਵੇਖਿਆ;
	ਪੂਰਨ cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_ਵੇਖਿਆ = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		ਪੂਰਨ tl_common_flags = 0;

		ਜੇ (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		ਜੇ (tl_common_flags & SD_NUMA)
			ਜਾਰੀ;

		cpumask_clear(covered);
		cpumask_clear(id_ਵੇਖਿਆ);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ਅਚਲ ਬਣਤਰ cpumask *tl_cpu_ਨਕਾਬ = tl->ਨਕਾਬ(tl, cpu);
			ਪੂਰਨ id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_ਪਹਿਲਾ(tl_cpu_ਨਕਾਬ);

			ਜੇ (cpumask_test_cpu(id, id_ਵੇਖਿਆ)) {
				/* First CPU has already been seen, ensure identical spans */
				ਜੇ (!cpumask_equal(tl->ਨਕਾਬ(tl, id), tl_cpu_ਨਕਾਬ))
					ਵਾਪਸ false;
			} ਨਹੀਂਤਾਂ {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				ਜੇ (cpumask_intersects(tl_cpu_ਨਕਾਬ, covered))
					ਵਾਪਸ false;

				cpumask_or(covered, covered, tl_cpu_ਨਕਾਬ);
				cpumask_ਸੈੱਟ_cpu(id, id_ਵੇਖਿਆ);
			}
		}
	}
	ਵਾਪਸ true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
ਸਥਿਰ ਖਾਲੀ adjust_numa_imbalance(ਬਣਤਰ sched_domain *sd_llc)
{
	ਬਣਤਰ sched_domain *ਮਾਪਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ imb_ਵਿਸਤਾਰ = 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ imb = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_llcs;

	WARN_ON(!(sd_llc->flags & SD_SHARE_LLC));
	WARN_ON(!sd_llc->ਮਾਪਾ);

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
	nr_llcs = sd_llc->ਮਾਪਾ->ਵਿਸਤਾਰ_ਭਾਰ / sd_llc->ਵਿਸਤਾਰ_ਭਾਰ;
	ਜੇ (nr_llcs == 1)
		imb = sd_llc->ਮਾਪਾ->ਵਿਸਤਾਰ_ਭਾਰ >> 3;
	ਨਹੀਂਤਾਂ
		imb = nr_llcs;

	imb = max(1U, imb);
	sd_llc->ਮਾਪਾ->imb_numa_nr = imb;

	/*
	 * Set span based on the first NUMA domain.
	 *
	 * NUMA systems always add a NODE domain before
	 * iterating the NUMA domains. Since this is before
	 * degeneration, start from sd_llc's parent's
	 * parent which is the lowest an SD_NUMA domain can
	 * be relative to sd_llc.
	 */
	ਮਾਪਾ = sd_llc->ਮਾਪਾ->ਮਾਪਾ;
	ਜਦੋਂ (ਮਾਪਾ && !(ਮਾਪਾ->flags & SD_NUMA))
		ਮਾਪਾ = ਮਾਪਾ->ਮਾਪਾ;

	imb_ਵਿਸਤਾਰ = ਮਾਪਾ ? ਮਾਪਾ->ਵਿਸਤਾਰ_ਭਾਰ : sd_llc->ਮਾਪਾ->ਵਿਸਤਾਰ_ਭਾਰ;

	/* Update the upper remainder of the topology */
	ਮਾਪਾ = sd_llc->ਮਾਪਾ;
	ਜਦੋਂ (ਮਾਪਾ) {
		ਪੂਰਨ ਗੁਣਕ = max(1U, (ਮਾਪਾ->ਵਿਸਤਾਰ_ਭਾਰ / imb_ਵਿਸਤਾਰ));

		ਮਾਪਾ->imb_numa_nr = imb * ਗੁਣਕ;
		ਮਾਪਾ = ਮਾਪਾ->ਮਾਪਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ
init_sched_domain_shared(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਬਣਤਰ sched_domain *sd, ਪੂਰਨ flags)
{
	ਬਣਤਰ sched_domain_shared *sds = ਨਿਰਾ;
	ਪੂਰਨ cpu;

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
	for_each_cpu(cpu, sched_domain_ਵਿਸਤਾਰ(sd)) {
		sds = *ਪ੍ਰਤੀ_cpu_ptr(d->sds, cpu);

		ਜੇ (!sds->alloc_flags ||
		    sd->ਵਿਸਤਾਰ_ਭਾਰ == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			ਰੋਕੋ;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	ਜੇ (WARN_ON_ONCE(!sd->shared))
		sd->shared = sds;

	/*
	 * nr_busy_cpus is consumed only by the NOHZ kick path via
	 * sd_balance_shared; on the asym-capacity path it is initialized but
	 * never read.
	 */
	atomic_ਸੈੱਟ(&sd->shared->nr_busy_cpus, sd->ਵਿਸਤਾਰ_ਭਾਰ);
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
ਸਥਿਰ bool claim_asym_sched_domain_shared(ਬਣਤਰ s_ਅੰਕੜੇ *d, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain *sd = *ਪ੍ਰਤੀ_cpu_ptr(d->sd, cpu);
	ਬਣਤਰ sched_domain *sd_asym;

	ਜੇ (!sd)
		ਵਾਪਸ false;

	sd_asym = sd;
	ਜਦੋਂ (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_ਭਰਿਆ))
		sd_asym = sd_asym->ਮਾਪਾ;

	ਜੇ (!sd_asym || (sd_asym->flags & SD_NUMA))
		ਵਾਪਸ false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	ਵਾਪਸ true;
}

ਸਥਿਰ ਪੂਰਨ __sched_domains_alloc_llc_id(ਖਾਲੀ)
{
	ਪੂਰਨ lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_ਪਹਿਲਾ_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	ਜੇ (lid >= nr_cpu_ids)
		ਵਾਪਸ -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_ਆਖਰੀ(sched_domains_llc_id_allocmask);
	ਜੇ (max > max_lid)
		max_lid = max;

	ਵਾਪਸ lid;
}

ਸਥਿਰ ਖਾਲੀ __sched_domains_free_llc_id(ਪੂਰਨ cpu)
{
	ਪੂਰਨ i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = ਪ੍ਰਤੀ_cpu(sd_llc_id, cpu);
	ਜੇ (lid == -1 || lid >= nr_cpu_ids)
		ਵਾਪਸ;

	ਪ੍ਰਤੀ_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_ਨਕਾਬ(cpu)) {
		/* An online CPU owns the llc_id. */
		ਜੇ (ਪ੍ਰਤੀ_cpu(sd_llc_id, i) == lid)
			ਵਾਪਸ;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_ਆਖਰੀ(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	ਜੇ (max < max_lid)
		max_lid = max;
}

ਖਾਲੀ sched_domains_free_llc_id(ਪੂਰਨ cpu)
{
	sched_domains_mutex_ਤਾਲਾ();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
ਸਥਿਰ ਪੂਰਨ
build_sched_domains(ਅਚਲ ਬਣਤਰ cpumask *cpu_map, ਬਣਤਰ sched_domain_attr *attr,
		    bool *multi_llcs)
{
	ਗਿਣਤੀ s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	ਬਣਤਰ sched_domain *sd;
	ਬਣਤਰ s_ਅੰਕੜੇ d;
	ਬਣਤਰ rq *rq = ਨਿਰਾ;
	ਪੂਰਨ i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	ਜੇ (WARN_ON(cpumask_ਖਾਲੀ_ਥਾਵਾਂ(cpu_map)))
		ਜਾਓ error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	ਜੇ (alloc_state != sa_rootdomain)
		ਜਾਓ error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		ਬਣਤਰ sched_domain_topology_level *tl;
		ਪੂਰਨ lid;

		sd = ਨਿਰਾ;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			ਜੇ (tl == sched_domain_topology)
				*ਪ੍ਰਤੀ_cpu_ptr(d.sd, i) = sd;
			ਜੇ (cpumask_equal(cpu_map, sched_domain_ਵਿਸਤਾਰ(sd)))
				ਰੋਕੋ;
		}

		lid = ਪ੍ਰਤੀ_cpu(sd_llc_id, i);
		ਜੇ (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			ਲਈ (ਪੂਰਨ j = cpumask_ਪਹਿਲਾ(llc_ਨਕਾਬ(i));
			     j < nr_cpu_ids;
			     j = cpumask_ਅਗਲਾ(j, llc_ਨਕਾਬ(i))) {
				ਜੇ (i == j)
					ਜਾਰੀ;

				lid = ਪ੍ਰਤੀ_cpu(sd_llc_id, j);

				ਜੇ (lid != -1) {
					ਪ੍ਰਤੀ_cpu(sd_llc_id, i) = lid;

					ਰੋਕੋ;
				}
			}

			/* a new LLC is detected */
			ਜੇ (lid == -1)
				ਪ੍ਰਤੀ_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	ਜੇ (WARN_ON(!topology_ਵਿਸਤਾਰ_sane(cpu_map)))
		ਜਾਓ error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		ਲਈ (sd = *ਪ੍ਰਤੀ_cpu_ptr(d.sd, i); sd; sd = sd->ਮਾਪਾ) {
			sd->ਵਿਸਤਾਰ_ਭਾਰ = cpumask_ਭਾਰ(sched_domain_ਵਿਸਤਾਰ(sd));
			ਜੇ (sd->flags & SD_NUMA) {
				ਜੇ (build_overlap_sched_groups(sd, i))
					ਜਾਓ error;
			} ਨਹੀਂਤਾਂ {
				ਜੇ (build_sched_groups(sd, i))
					ਜਾਓ error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *ਪ੍ਰਤੀ_cpu_ptr(d.sd, i);
		ਜੇ (!sd)
			ਜਾਰੀ;

		ਜੇ (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		ਜਦੋਂ (sd->ਮਾਪਾ && (sd->ਮਾਪਾ->flags & SD_SHARE_LLC))
			sd = sd->ਮਾਪਾ;

		ਜੇ (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			ਜੇ (sd->ਮਾਪਾ) {
				ਜੇ (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				ਜੇ (sd_ਵਿੱਚ_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	ਲਈ (i = nr_cpumask_ਬਿੱਟ-1; i >= 0; i--) {
		ਜੇ (!cpumask_test_cpu(i, cpu_map))
			ਜਾਰੀ;

		claim_allocations(i, &d);

		ਲਈ (sd = *ਪ੍ਰਤੀ_cpu_ptr(d.sd, i); sd; sd = sd->ਮਾਪਾ)
			init_sched_groups_ਸਮਰੱਥਾ(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_ਤਾਲਾ();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *ਪ੍ਰਤੀ_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		ਜੇ (ਸਭ_ਤੋਂ_ਨੀਵਾਂ_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	ਜੇ (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	ਜੇ (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_ਸਰਗਰਮ);

	ਜੇ (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_ਦਲੀਲਾਂ(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	ਵਾਪਸ ret;
}

/* Current sched domains: */
ਸਥਿਰ cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
ਸਥਿਰ ਪੂਰਨ				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
ਸਥਿਰ ਬਣਤਰ sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
ਸਥਿਰ cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
ਪੂਰਨ __weak arch_update_cpu_topology(ਖਾਲੀ)
{
	ਵਾਪਸ 0;
}

cpumask_var_t *alloc_sched_domains(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ndoms)
{
	ਪੂਰਨ i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	ਜੇ (!doms)
		ਵਾਪਸ ਨਿਰਾ;
	ਲਈ (i = 0; i < ndoms; i++) {
		ਜੇ (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			ਵਾਪਸ ਨਿਰਾ;
		}
	}
	ਵਾਪਸ doms;
}

ਖਾਲੀ free_sched_domains(cpumask_var_t doms[], ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ndoms)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	ਲਈ (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
ਪੂਰਨ __init sched_init_domains(ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	bool multi_llcs;
	ਪੂਰਨ err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_ਸਮਰੱਥਾ_ਜਾਂਚ();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	ਜੇ (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], ਨਿਰਾ, &multi_llcs);
	ਜੇ (!err)
		sched_cache_ਸੈੱਟ(multi_llcs);

	ਵਾਪਸ err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
ਸਥਿਰ ਖਾਲੀ detach_destroy_domains(ਅਚਲ ਬਣਤਰ cpumask *cpu_map)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu = cpumask_any(cpu_map);
	ਪੂਰਨ i;

	ਜੇ (rcu_access_ਸੂਚਕ(ਪ੍ਰਤੀ_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	ਜੇ (static_branch_unlikely(&sched_cluster_ਸਰਗਰਮ))
		static_branch_dec_cpuslocked(&sched_cluster_ਸਰਗਰਮ);

	rcu_read_ਤਾਲਾ();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(ਨਿਰਾ, &def_ਜੜ੍ਹ_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
ਸਥਿਰ ਪੂਰਨ dattrs_equal(ਬਣਤਰ sched_domain_attr *cur, ਪੂਰਨ idx_cur,
			ਬਣਤਰ sched_domain_attr *new, ਪੂਰਨ idx_new)
{
	ਬਣਤਰ sched_domain_attr tmp;

	/* Fast path: */
	ਜੇ (!new && !cur)
		ਵਾਪਸ 1;

	tmp = SD_ATTR_INIT;

	ਵਾਪਸ !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			ਆਕਾਰ(ਬਣਤਰ sched_domain_attr));
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
ਸਥਿਰ ਖਾਲੀ ਹਿੱਸੇ_ਕਰੋ_sched_domains_locked(ਪੂਰਨ ndoms_new, cpumask_var_t doms_new[],
				    ਬਣਤਰ sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	ਪੂਰਨ i, j, n;
	ਪੂਰਨ new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	ਜੇ (new_topology)
		asym_cpu_ਸਮਰੱਥਾ_ਜਾਂਚ();

	ਜੇ (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		ਜੇ (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_ਸਰਗਰਮ_ਨਕਾਬ,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} ਨਹੀਂਤਾਂ {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	ਲਈ (i = 0; i < ndoms_cur; i++) {
		ਲਈ (j = 0; j < n && !new_topology; j++) {
			ਜੇ (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				ਜਾਓ match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	ਜੇ (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_ਸਰਗਰਮ_ਨਕਾਬ,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	ਲਈ (i = 0; i < ndoms_new; i++) {
		ਲਈ (j = 0; j < n && !new_topology; j++) {
			ਜੇ (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				ਬਣਤਰ sched_domain *sd;
				ਪੂਰਨ cpu = cpumask_ਪਹਿਲਾ(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				ਜਦੋਂ (sd && sd->ਮਾਪਾ && (sd->ਮਾਪਾ->flags & SD_SHARE_LLC))
					sd = sd->ਮਾਪਾ;
				ਜੇ (sd && (sd->flags & SD_SHARE_LLC) && sd->ਮਾਪਾ &&
				    sd_ਵਿੱਚ_multi_llcs(sd))
					has_multi_llcs = true;
				ਜਾਓ match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : ਨਿਰਾ,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_ਸੈੱਟ(has_multi_llcs);

#ਜੇ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	ਲਈ (i = 0; i < ndoms_new; i++) {
		ਲਈ (j = 0; j < n && !sched_energy_update; j++) {
			ਜੇ (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_ਪਹਿਲਾ(doms_cur[j]))->rd->pd) {
				has_eas = true;
				ਜਾਓ match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_ਸੈੱਟ(has_eas);
#ਜੇ_ਅੰਤ

	/* Remember the new sched domains: */
	ਜੇ (doms_cur != &fallback_doms)
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
ਖਾਲੀ ਹਿੱਸੇ_ਕਰੋ_sched_domains(ਪੂਰਨ ndoms_new, cpumask_var_t doms_new[],
			     ਬਣਤਰ sched_domain_attr *dattr_new)
{
	sched_domains_mutex_ਤਾਲਾ();
	ਹਿੱਸੇ_ਕਰੋ_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
