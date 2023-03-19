 @*****************************************************************************
 @ amplify.S : ARM NEON software amplification
 @*****************************************************************************
 @ Copyright (C) 2012 Rémi Denis-Courmont
 @
 @ SPDX-License-Identifier: LGPL-2.1-or-later
 @****************************************************************************/

#include "../asm.S"

	.syntax	unified
#if HAVE_AS_FPU_DIRECTIVE
	.fpu	neon
#endif
	.text

#define	DST	r0
#define	SRC	r1
#define	SIZE	r2
	.align 2
function amplify_float_arm_neon
	cmp		SIZE,	#0
	it		eq
	bxeq		lr
#ifdef __ARM_PCS
	vmov		s0,	r3	@ softfp
#endif
	pld		[SRC,	#64]
	vld1.f32	{d16-d17},	[SRC,:128]!
	subs		SIZE,	SIZE,	#16
	vmul.f32	d16,	d16,	d0[0]
	vmul.f32	d17,	d17,	d0[0]
	blo		5f
	pld		[SRC,	#64]
	vld1.f32	{d18-d19},	[SRC,:128]!
	subs		SIZE,	SIZE,	#16
	vmul.f32	d18,	d18,	d0[0]
	vmul.f32	d19,	d19,	d0[0]
	blo		2f
1:	@ main loop starts
	pld		[SRC,	#64]
	vld1.f32	{d20-d21},	[SRC,:128]!
	subs		SIZE,	SIZE,	#16
	vmul.f32	d20,	d20,	d0[0]
	vmul.f32	d21,	d21,	d0[0]
	vst1.f32	{d16-d17},	[DST,:128]!
	blo		3f
	pld		[SRC,	#64]
	vld1.f32	{d16-d17},	[SRC,:128]!
	subs		SIZE,	SIZE,	#16
	vmul.f32	d16,	d16,	d0[0]
	vmul.f32	d17,	d17,	d0[0]
	vst1.f32	{d18-d19},	[DST,:128]!
	blo		4f
	pld		[SRC,	#64]
	vld1.f32	{d18-d19},	[SRC,:128]!
	subs		SIZE,	SIZE,	#16
	vmul.f32	d18,	d18,	d0[0]
	vmul.f32	d19,	d19,	d0[0]
	vst1.f32	{d20-d21},	[DST,:128]!
	bhi		1b
	@ main loop ends
2:	vst1.f32	{d16-d17},	[DST,:128]!
	vst1.f32	{d18-d19},	[DST,:128]!
	bx		lr
3:	vst1.f32	{d18-d19},	[DST,:128]!
	vst1.f32	{d20-d21},	[DST,:128]!
	bx		lr
4:	vst1.f32	{d20-d21},	[DST,:128]!
5:	vst1.f32	{d16-d17},	[DST,:128]!
	bx		lr
