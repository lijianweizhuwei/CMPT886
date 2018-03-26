	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_add25i5                ## -- Begin function add25i5
	.p2align	4, 0x90
_add25i5:                               ## @add25i5
	.cfi_startproc
## %bb.0:                               ## %entry
	movd	%r8d, %xmm0
	movd	%ecx, %xmm1
	punpckldq	%xmm0, %xmm1    ## xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1]
	movd	%r9d, %xmm0
	punpcklqdq	%xmm0, %xmm1    ## xmm1 = xmm1[0],xmm0[0]
	movd	%esi, %xmm0
	movd	%edi, %xmm2
	punpckldq	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1]
	movd	%edx, %xmm0
	punpcklqdq	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0]
	paddd	%xmm1, %xmm2
	movdqa	%xmm2, -24(%rsp)
	movb	-24(%rsp), %al
	movb	-20(%rsp), %dl
	movb	-16(%rsp), %cl
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
