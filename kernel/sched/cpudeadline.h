/* SPDX-License-Identifier: GPL-2.0 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/spinlock.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDX_INVALID		-1

ਬਣਤਰ cpudl_ਵਸਤੂ {
	u64			dl;
	ਪੂਰਨ			cpu;
	ਪੂਰਨ			idx;
};

ਬਣਤਰ cpudl {
	raw_spinlock_t		ਤਾਲਾ;
	ਪੂਰਨ			ਮਾਪ;
	cpumask_var_t		free_cpus;
	ਬਣਤਰ cpudl_ਵਸਤੂ	*elements;
};

ਪੂਰਨ  cpudl_ਲੱਭੋ(ਬਣਤਰ cpudl *cp, ਬਣਤਰ task_struct *p, ਬਣਤਰ cpumask *later_ਨਕਾਬ);
ਖਾਲੀ cpudl_ਸੈੱਟ(ਬਣਤਰ cpudl *cp, ਪੂਰਨ cpu, u64 dl);
ਖਾਲੀ cpudl_clear(ਬਣਤਰ cpudl *cp, ਪੂਰਨ cpu, bool online);
ਪੂਰਨ  cpudl_init(ਬਣਤਰ cpudl *cp);
ਖਾਲੀ cpudl_cleanup(ਬਣਤਰ cpudl *cp);
