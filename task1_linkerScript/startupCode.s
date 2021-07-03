	.cpu cortex-m4
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"startupCode.c"
	.text
	.align	1
	.global	ResetHandler
	.arch armv7e-m
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	ResetHandler, %function
ResetHandler:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	push	{r7, lr}
	sub	sp, sp, #24
	add	r7, sp, #0
	ldr	r3, .L7
	str	r3, [r7, #20]
	ldr	r3, .L7+4
	str	r3, [r7, #8]
	ldr	r3, .L7+8
	str	r3, [r7, #16]
	ldr	r3, .L7+12
	str	r3, [r7, #12]
	ldr	r3, .L7+16
	str	r3, [r7, #4]
	b	.L2
.L3:
	ldr	r3, [r7, #20]
	adds	r2, r3, #4
	str	r2, [r7, #20]
	movs	r2, #0
	str	r2, [r3]
.L2:
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #8]
	cmp	r2, r3
	bcc	.L3
	b	.L4
.L5:
	ldr	r2, [r7, #16]
	adds	r3, r2, #4
	str	r3, [r7, #16]
	ldr	r3, [r7, #12]
	adds	r1, r3, #4
	str	r1, [r7, #12]
	ldr	r2, [r2]
	str	r2, [r3]
.L4:
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #4]
	cmp	r2, r3
	bcc	.L5
	bl	main
.L6:
	b	.L6
.L8:
	.align	2
.L7:
	.word	__bss_start__
	.word	__bss_end__
	.word	__data_load__
	.word	__data_start__
	.word	__data_end__
	.size	ResetHandler, .-ResetHandler
	.ident	"GCC: (GNU Arm Embedded Toolchain 9-2020-q2-update) 9.3.1 20200408 (release)"
