 @*****************************************************************************
 @ i420_yuyv.S : ARM NEONv1 I420 to YUYV chroma conversion
 @*****************************************************************************
 @ Copyright (C) 2009-2011 Rémi Denis-Courmont
 @
 @ SPDX-License-Identifier: LGPL-2.1-or-later
 @****************************************************************************/

#include "../asm.S"

	.syntax unified
#if HAVE_AS_FPU_DIRECTIVE
	.fpu	neon
#endif
	.text

#define O1	r0
#define O2	r1
#define WIDTH	r2
#define HEIGHT	r3
#define Y1	r4
#define Y2	r5
#define U	r6
#define V	r7
#define YPITCH	r8
#define OPAD	r10
#define YPAD	r11
#define COUNT	ip
#define OPITCH	lr

	.align 2
function i420_yuyv_neon
	push		{r4-r8,r10-r11,lr}
	ldmia		r0,	{O1, OPITCH}
	ldmia		r1,	{Y1, U, V, YPITCH}
	cmp		HEIGHT,	#0
	sub		OPAD,	OPITCH,	WIDTH,	lsl #1
	sub		YPAD,	YPITCH,	WIDTH
1:
	it		gt
	movsgt		COUNT,	WIDTH
	add		O2,	O1,	OPITCH
	add		Y2,	Y1,	YPITCH
	it		le
	pople		{r4-r8,r10-r11,pc}
2:
	pld		[U, #64]
	vld1.u8		{d2},		[U,:64]!
	pld		[V, #64]
	vld1.u8		{d3},		[V,:64]!
	pld		[Y1, #64]
	vzip.u8		d2,	d3
	subs		COUNT,	COUNT,	#16
	vld1.u8		{q0},		[Y1,:128]!
	pld		[Y2, #64]
	vmov		q3,	q1
	vzip.u8		q0,	q1
	vld1.u8		{q2},		[Y2,:128]!
	vzip.u8		q2,	q3
	vst1.u8		{q0-q1},	[O1,:128]!
	vst1.u8		{q2-q3},	[O2,:128]!
	bgt		2b

	subs		HEIGHT,	#2
	add		O1,	O2,	OPAD
	add		Y1,	Y2,	YPAD
	add		U,	U,	YPAD,	lsr #1
	add		V,	V,	YPAD,	lsr #1
	b		1b

function i420_uyvy_neon
	push		{r4-r8,r10-r11,lr}
	ldmia		r0,	{O1, OPITCH}
	ldmia		r1,	{Y1, U, V, YPITCH}
	cmp		HEIGHT,	#0
	sub		OPAD,	OPITCH,	WIDTH,	lsl #1
	sub		YPAD,	YPITCH,	WIDTH
1:
	it		gt
	movsgt		COUNT,	WIDTH
	add		O2,	O1,	OPITCH
	add		Y2,	Y1,	YPITCH
	it		le
	pople		{r4-r8,r10-r11,pc}
2:
	pld		[U, #64]
	vld1.u8		{d0},		[U,:64]!
	pld		[V, #64]
	vld1.u8		{d1},		[V,:64]!
	pld		[Y1, #64]
	vzip.u8		d0,	d1
	subs		COUNT,	COUNT,	#16
	vld1.u8		{q1},		[Y1,:128]!
	pld		[Y2, #64]
	vmov		q2,	q0
	vzip.u8		q0,	q1
	vld1.u8		{q3},		[Y2,:128]!
	vzip.u8		q2,	q3
	vst1.u8		{q0-q1},	[O1,:128]!
	vst1.u8		{q2-q3},	[O2,:128]!
	bgt		2b

	subs		HEIGHT,	#2
	add		O1,	O2,	OPAD
	add		Y1,	Y2,	YPAD
	add		U,	U,	YPAD,	lsr #1
	add		V,	V,	YPAD,	lsr #1
	b		1b
