 @*****************************************************************************
 @ deinterleave_chroma.S : ARM NEONv1 conversion of interleaved to planar chroma
 @*****************************************************************************
 @ Copyright (C) 2009-2011 Rémi Denis-Courmont
 @ Copyright (C) 2013 Martin Storsjö
 @
 @ SPDX-License-Identifier: LGPL-2.1-or-later
 @****************************************************************************/

#include "../asm.S"

	.syntax unified
#if HAVE_AS_FPU_DIRECTIVE
	.fpu	neon
#endif
	.text

#define UV	r0
#define COUNT	r1
#define WIDTH	r2
#define HEIGHT	r3
#define IPITCH	r4
#define IPAD	r4
#define U	r5
#define V	r6
#define OPITCH	lr
#define OPAD	lr

	.align 2
function deinterleave_chroma_neon
	push		{r4-r6,lr}
	ldmia		r0,	{U, V, OPITCH}
	ldmia		r1,	{UV, IPITCH}
	cmp		HEIGHT,	#0

	@ round the width up to a multiple of 8
	add		WIDTH,	WIDTH, #7
	bic		WIDTH,	WIDTH, #7

	sub		IPAD,	IPITCH,	WIDTH, lsl #1
	sub		OPAD,	OPITCH,	WIDTH
1:
	ite		gt
	movsgt		COUNT,	WIDTH
	pople		{r4-r6,pc}
2:
	pld		[UV, #64]
	vld2.u8		{d0, d1},	[UV,:128]!
	subs		COUNT,	COUNT,	#8
	vst1.u8		{d0},		[U,:64]!
	vst1.u8		{d1},		[V,:64]!
	bgt		2b

	subs		HEIGHT,	#1
	add		UV,	UV,	IPAD
	add		U,	U,	OPAD
	add		V,	V,	OPAD
	b		1b
