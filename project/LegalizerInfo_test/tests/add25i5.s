	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_add25i5                ## -- Begin function add25i5
	.p2align	4, 0x90
_add25i5:                               ## @add25i5
	.cfi_startproc
## %bb.0:                               ## %entry
	movd	288(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	280(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm1    ## xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3],xmm1[4],xmm0[4],xmm1[5],xmm0[5],xmm1[6],xmm0[6],xmm1[7],xmm0[7]
	movd	272(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	264(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3],xmm2[4],xmm0[4],xmm2[5],xmm0[5],xmm2[6],xmm0[6],xmm2[7],xmm0[7]
	punpcklwd	%xmm1, %xmm2    ## xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1],xmm2[2],xmm1[2],xmm2[3],xmm1[3]
	movd	256(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	248(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm1    ## xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3],xmm1[4],xmm0[4],xmm1[5],xmm0[5],xmm1[6],xmm0[6],xmm1[7],xmm0[7]
	movd	240(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	232(%rsp), %xmm3        ## xmm3 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm3    ## xmm3 = xmm3[0],xmm0[0],xmm3[1],xmm0[1],xmm3[2],xmm0[2],xmm3[3],xmm0[3],xmm3[4],xmm0[4],xmm3[5],xmm0[5],xmm3[6],xmm0[6],xmm3[7],xmm0[7]
	punpcklwd	%xmm1, %xmm3    ## xmm3 = xmm3[0],xmm1[0],xmm3[1],xmm1[1],xmm3[2],xmm1[2],xmm3[3],xmm1[3]
	punpckldq	%xmm2, %xmm3    ## xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1]
	movd	224(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	216(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm1    ## xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3],xmm1[4],xmm0[4],xmm1[5],xmm0[5],xmm1[6],xmm0[6],xmm1[7],xmm0[7]
	movd	208(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	200(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3],xmm2[4],xmm0[4],xmm2[5],xmm0[5],xmm2[6],xmm0[6],xmm2[7],xmm0[7]
	punpcklwd	%xmm1, %xmm2    ## xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1],xmm2[2],xmm1[2],xmm2[3],xmm1[3]
	movd	192(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	184(%rsp), %xmm4        ## xmm4 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm4    ## xmm4 = xmm4[0],xmm0[0],xmm4[1],xmm0[1],xmm4[2],xmm0[2],xmm4[3],xmm0[3],xmm4[4],xmm0[4],xmm4[5],xmm0[5],xmm4[6],xmm0[6],xmm4[7],xmm0[7]
	movd	176(%rsp), %xmm0        ## xmm0 = mem[0],zero,zero,zero
	movd	168(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm1    ## xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1],xmm1[2],xmm0[2],xmm1[3],xmm0[3],xmm1[4],xmm0[4],xmm1[5],xmm0[5],xmm1[6],xmm0[6],xmm1[7],xmm0[7]
	punpcklwd	%xmm4, %xmm1    ## xmm1 = xmm1[0],xmm4[0],xmm1[1],xmm4[1],xmm1[2],xmm4[2],xmm1[3],xmm4[3]
	punpckldq	%xmm2, %xmm1    ## xmm1 = xmm1[0],xmm2[0],xmm1[1],xmm2[1]
	punpcklqdq	%xmm3, %xmm1    ## xmm1 = xmm1[0],xmm3[0]
	movd	88(%rsp), %xmm0         ## xmm0 = mem[0],zero,zero,zero
	movd	80(%rsp), %xmm2         ## xmm2 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3],xmm2[4],xmm0[4],xmm2[5],xmm0[5],xmm2[6],xmm0[6],xmm2[7],xmm0[7]
	movd	72(%rsp), %xmm0         ## xmm0 = mem[0],zero,zero,zero
	movd	64(%rsp), %xmm3         ## xmm3 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm3    ## xmm3 = xmm3[0],xmm0[0],xmm3[1],xmm0[1],xmm3[2],xmm0[2],xmm3[3],xmm0[3],xmm3[4],xmm0[4],xmm3[5],xmm0[5],xmm3[6],xmm0[6],xmm3[7],xmm0[7]
	punpcklwd	%xmm2, %xmm3    ## xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3]
	movd	56(%rsp), %xmm0         ## xmm0 = mem[0],zero,zero,zero
	movd	48(%rsp), %xmm2         ## xmm2 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3],xmm2[4],xmm0[4],xmm2[5],xmm0[5],xmm2[6],xmm0[6],xmm2[7],xmm0[7]
	movd	40(%rsp), %xmm0         ## xmm0 = mem[0],zero,zero,zero
	movd	32(%rsp), %xmm4         ## xmm4 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm4    ## xmm4 = xmm4[0],xmm0[0],xmm4[1],xmm0[1],xmm4[2],xmm0[2],xmm4[3],xmm0[3],xmm4[4],xmm0[4],xmm4[5],xmm0[5],xmm4[6],xmm0[6],xmm4[7],xmm0[7]
	punpcklwd	%xmm2, %xmm4    ## xmm4 = xmm4[0],xmm2[0],xmm4[1],xmm2[1],xmm4[2],xmm2[2],xmm4[3],xmm2[3]
	punpckldq	%xmm3, %xmm4    ## xmm4 = xmm4[0],xmm3[0],xmm4[1],xmm3[1]
	movd	24(%rsp), %xmm0         ## xmm0 = mem[0],zero,zero,zero
	movd	16(%rsp), %xmm2         ## xmm2 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3],xmm2[4],xmm0[4],xmm2[5],xmm0[5],xmm2[6],xmm0[6],xmm2[7],xmm0[7]
	movd	%r9d, %xmm3
	movd	8(%rsp), %xmm0          ## xmm0 = mem[0],zero,zero,zero
	punpcklbw	%xmm0, %xmm3    ## xmm3 = xmm3[0],xmm0[0],xmm3[1],xmm0[1],xmm3[2],xmm0[2],xmm3[3],xmm0[3],xmm3[4],xmm0[4],xmm3[5],xmm0[5],xmm3[6],xmm0[6],xmm3[7],xmm0[7]
	punpcklwd	%xmm2, %xmm3    ## xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3]
	movd	%r8d, %xmm0
	movd	%ecx, %xmm2
	punpcklbw	%xmm0, %xmm2    ## xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1],xmm2[2],xmm0[2],xmm2[3],xmm0[3],xmm2[4],xmm0[4],xmm2[5],xmm0[5],xmm2[6],xmm0[6],xmm2[7],xmm0[7]
	movd	%edx, %xmm5
	movd	%esi, %xmm0
	punpcklbw	%xmm5, %xmm0    ## xmm0 = xmm0[0],xmm5[0],xmm0[1],xmm5[1],xmm0[2],xmm5[2],xmm0[3],xmm5[3],xmm0[4],xmm5[4],xmm0[5],xmm5[5],xmm0[6],xmm5[6],xmm0[7],xmm5[7]
	punpcklwd	%xmm2, %xmm0    ## xmm0 = xmm0[0],xmm2[0],xmm0[1],xmm2[1],xmm0[2],xmm2[2],xmm0[3],xmm2[3]
	punpckldq	%xmm3, %xmm0    ## xmm0 = xmm0[0],xmm3[0],xmm0[1],xmm3[1]
	punpcklqdq	%xmm4, %xmm0    ## xmm0 = xmm0[0],xmm4[0]
	paddb	%xmm1, %xmm0
	movd	352(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	movd	344(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	punpcklbw	%xmm1, %xmm2    ## xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1],xmm2[2],xmm1[2],xmm2[3],xmm1[3],xmm2[4],xmm1[4],xmm2[5],xmm1[5],xmm2[6],xmm1[6],xmm2[7],xmm1[7]
	movd	336(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	movd	328(%rsp), %xmm3        ## xmm3 = mem[0],zero,zero,zero
	punpcklbw	%xmm1, %xmm3    ## xmm3 = xmm3[0],xmm1[0],xmm3[1],xmm1[1],xmm3[2],xmm1[2],xmm3[3],xmm1[3],xmm3[4],xmm1[4],xmm3[5],xmm1[5],xmm3[6],xmm1[6],xmm3[7],xmm1[7]
	punpcklwd	%xmm2, %xmm3    ## xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3]
	movd	320(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	movd	312(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	punpcklbw	%xmm1, %xmm2    ## xmm2 = xmm2[0],xmm1[0],xmm2[1],xmm1[1],xmm2[2],xmm1[2],xmm2[3],xmm1[3],xmm2[4],xmm1[4],xmm2[5],xmm1[5],xmm2[6],xmm1[6],xmm2[7],xmm1[7]
	movd	304(%rsp), %xmm4        ## xmm4 = mem[0],zero,zero,zero
	movd	296(%rsp), %xmm1        ## xmm1 = mem[0],zero,zero,zero
	punpcklbw	%xmm4, %xmm1    ## xmm1 = xmm1[0],xmm4[0],xmm1[1],xmm4[1],xmm1[2],xmm4[2],xmm1[3],xmm4[3],xmm1[4],xmm4[4],xmm1[5],xmm4[5],xmm1[6],xmm4[6],xmm1[7],xmm4[7]
	punpcklwd	%xmm2, %xmm1    ## xmm1 = xmm1[0],xmm2[0],xmm1[1],xmm2[1],xmm1[2],xmm2[2],xmm1[3],xmm2[3]
	punpckldq	%xmm3, %xmm1    ## xmm1 = xmm1[0],xmm3[0],xmm1[1],xmm3[1]
	movd	360(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	punpcklqdq	%xmm2, %xmm1    ## xmm1 = xmm1[0],xmm2[0]
	movd	152(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	movd	144(%rsp), %xmm3        ## xmm3 = mem[0],zero,zero,zero
	punpcklbw	%xmm2, %xmm3    ## xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3],xmm3[4],xmm2[4],xmm3[5],xmm2[5],xmm3[6],xmm2[6],xmm3[7],xmm2[7]
	movd	136(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	movd	128(%rsp), %xmm4        ## xmm4 = mem[0],zero,zero,zero
	punpcklbw	%xmm2, %xmm4    ## xmm4 = xmm4[0],xmm2[0],xmm4[1],xmm2[1],xmm4[2],xmm2[2],xmm4[3],xmm2[3],xmm4[4],xmm2[4],xmm4[5],xmm2[5],xmm4[6],xmm2[6],xmm4[7],xmm2[7]
	punpcklwd	%xmm3, %xmm4    ## xmm4 = xmm4[0],xmm3[0],xmm4[1],xmm3[1],xmm4[2],xmm3[2],xmm4[3],xmm3[3]
	movd	120(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	movd	112(%rsp), %xmm3        ## xmm3 = mem[0],zero,zero,zero
	punpcklbw	%xmm2, %xmm3    ## xmm3 = xmm3[0],xmm2[0],xmm3[1],xmm2[1],xmm3[2],xmm2[2],xmm3[3],xmm2[3],xmm3[4],xmm2[4],xmm3[5],xmm2[5],xmm3[6],xmm2[6],xmm3[7],xmm2[7]
	movd	104(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	movd	96(%rsp), %xmm5         ## xmm5 = mem[0],zero,zero,zero
	punpcklbw	%xmm2, %xmm5    ## xmm5 = xmm5[0],xmm2[0],xmm5[1],xmm2[1],xmm5[2],xmm2[2],xmm5[3],xmm2[3],xmm5[4],xmm2[4],xmm5[5],xmm2[5],xmm5[6],xmm2[6],xmm5[7],xmm2[7]
	punpcklwd	%xmm3, %xmm5    ## xmm5 = xmm5[0],xmm3[0],xmm5[1],xmm3[1],xmm5[2],xmm3[2],xmm5[3],xmm3[3]
	punpckldq	%xmm4, %xmm5    ## xmm5 = xmm5[0],xmm4[0],xmm5[1],xmm4[1]
	movd	160(%rsp), %xmm2        ## xmm2 = mem[0],zero,zero,zero
	punpcklqdq	%xmm2, %xmm5    ## xmm5 = xmm5[0],xmm2[0]
	paddb	%xmm1, %xmm5
	movdqa	%xmm5, -24(%rsp)
	movb	-16(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-17(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-18(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-19(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-20(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-21(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-22(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-24(%rsp), %al
	movb	-23(%rsp), %cl
	andb	$31, %cl
	movb	%cl, (%rdi)
	andb	$31, %al
	movb	%al, (%rdi)
	movdqa	%xmm0, -40(%rsp)
	movb	-25(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-26(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-27(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-28(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-29(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-30(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-31(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-32(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-33(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-34(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-35(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-36(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-37(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-38(%rsp), %al
	andb	$31, %al
	movb	%al, (%rdi)
	movb	-40(%rsp), %al
	movb	-39(%rsp), %cl
	andb	$31, %cl
	movb	%cl, (%rdi)
	andb	$31, %al
	movb	%al, (%rdi)
	movq	%rdi, %rax
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
