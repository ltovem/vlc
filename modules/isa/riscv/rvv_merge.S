/******************************************************************************
 * rvv_merge.S: RISC-V Vector mean
 ******************************************************************************
 * Copyright (C) 2022 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

	.option arch, +v
	.text
	.align	2

	.globl	merge8_rvv
	.type	merge8_rvv, %function
merge8_rvv:
	csrwi	vxrm, 0
1:	vsetvli	t0, a3, e8, m8, ta, ma
	vle8.v	v16, (a1)
	add	a1, a1, t0
	vle8.v	v24, (a2)
	add	a2, a2, t0
	vaaddu.vv	v16, v16, v24
	sub	a3, a3, t0
	vse8.v	v16, (a0)
	add	a0, a0, t0
	bnez	a3, 1b
	ret
	.size	merge8_rvv, . - merge8_rvv

	.globl	merge16_rvv
	.type	merge16_rvv, %function
merge16_rvv:
	csrwi	vxrm, 0
	srli	a3, a3, 1
1:	vsetvli	t0, a3, e16, m8, ta, ma
	slli	t1, t0, 1
	vle16.v	v16, (a1)
	add	a1, a1, t1
	vle16.v	v24, (a2)
	add	a2, a2, t1
	vaaddu.vv	v16, v16, v24
	sub	a3, a3, t0
	vse16.v	v16, (a0)
	add	a0, a0, t1
	bnez	a3, 1b
	ret
	.size	merge16_rvv, . - merge16_rvv

