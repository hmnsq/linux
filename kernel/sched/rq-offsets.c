// SPDX-License-Identifier: GPL-2.0
#define COMPILE_OFFSETS
#include <linux/kbuild.h>
#include <linux/types.h>
#include "sched.h"

整数 主要(void)
{
	DEFINE(RQ_nr_pinned, offsetof(struct rq, nr_pinned));

	返回 0;
}
