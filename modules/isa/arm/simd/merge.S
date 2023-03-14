 @*****************************************************************************
 @ merge.S: ARMv6 SIMD mean
 @*****************************************************************************
 @ Copyright (C) 2009-2012 Rémi Denis-Courmont
 @
 @ SPDX-License-Identifier: LGPL-2.1-or-later
 @****************************************************************************/

#include "../asm.S"

	.syntax	unified
#if HAVE_AS_ARCH_DIRECTIVE
	.arch	armv6
#endif
	.text

#define	DEST	r0
#define	SRC1	r1
#define	SRC2	r2
#define	SIZE	r3

	.align 2
function merge8_armv6
	push		{r4-r9,lr}
1:
	pld		[SRC1, #64]
	ldm		SRC1!,	{r4-r5}
	pld		[SRC2, #64]
	ldm		SRC2!,	{r8-r9}
	subs		SIZE,	SIZE,	#16
	uhadd8		r4,	r4,	r8
	ldm		SRC1!,	{r6-r7}
	uhadd8		r5,	r5,	r9
	ldm		SRC2!,	{ip,lr}
	uhadd8		r6,	r6,	ip
	stm		DEST!,	{r4-r5}
	uhadd8		r7,	r7,	lr
	stm		DEST!,	{r6-r7}
	it		eq
	popeq		{r4-r9,pc}
	b		1b

	.align 2
function merge16_armv6
	push		{r4-r9,lr}
1:
	pld		[SRC1, #64]
	ldm		SRC1!,	{r4-r5}
	pld		[SRC2, #64]
	ldm		SRC2!,	{r8-r9}
	subs		SIZE,	SIZE,	#16
	uhadd16		r4,	r4,	r8
	ldm		SRC1!,	{r6-r7}
	uhadd16		r5,	r5,	r9
	ldm		SRC2!,	{ip,lr}
	uhadd16		r6,	r6,	ip
	stm		DEST!,	{r4-r5}
	uhadd16		r7,	r7,	lr
	stm		DEST!,	{r6-r7}
	it		eq
	popeq		{r4-r9,pc}
	b		1b
