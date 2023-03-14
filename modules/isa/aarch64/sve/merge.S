/******************************************************************************
 * merge.S : ARM SVE mean
 ******************************************************************************
 * Copyright (C) 2018 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#include "../../arm/asm.S"

	.arch	armv8-a+sve

	/* TODO: prefetch, unroll */

	.text
	.align	2
	bti_advertise
function merge8_arm_sve
	bti	c
	mov	x4, #0
	b	2f
1:	ld1b	{z0.h}, p0/z, [x1, x4]
	ld1b	{z1.h}, p0/z, [x2, x4]
	add	z0.h, z0.h, z1.h
	lsr	z0.h, z0.h, #1
	st1b	{z0.h}, p0, [x0, x4]
	inch	x4
2:	whilelt	p0.h, x4, x3
	b.first	1b
	ret

function merge16_arm_sve
	bti	c
	mov	x4, #0
	b	2f
1:	ld1h	{z0.s}, p0/z, [x1, x4, lsl #1]
	ld1h	{z1.s}, p0/z, [x2, x4, lsl #1]
	add	z0.s, z0.s, z1.s
	lsr	z0.s, z0.s, #1
	st1h	{z0.s}, p0, [x0, x4, lsl #1]
	incw	x4
2:	whilelt	p0.s, x4, x3
	b.first	1b
	ret
