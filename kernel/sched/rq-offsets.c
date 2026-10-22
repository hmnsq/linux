// SPDX-License-Identifier: GPL-2.0
#define COMPILE_OFFSETS
#include <linux/kbuild.h>
#include <linux/types.h>
#include "sched.h"

số_nguyên main(void)
{
	DEFINE(RQ_nr_pinned, offsetof(struct rq, nr_pinned));

	trả_về 0;
}
