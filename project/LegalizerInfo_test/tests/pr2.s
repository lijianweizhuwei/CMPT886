	.text
	.file	"pr2.ll"
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4               # -- Begin function signmaskd
.LCPI0_0:
	.long	2147483648              # 0x80000000
	.long	0                       # 0x0
	.long	2147483648              # 0x80000000
	.long	0                       # 0x0
	.text
	.globl	signmaskd
	.p2align	4, 0x90
	.type	signmaskd,@function
signmaskd:                              # @signmaskd
	.cfi_startproc
# %bb.0:
	movdqa	.LCPI0_0(%rip), %xmm1   # xmm1 = [2147483648,0,2147483648,0]
	pxor	%xmm1, %xmm0
	movdqa	%xmm1, %xmm2
	pcmpgtd	%xmm0, %xmm2
	pshufd	$160, %xmm2, %xmm3      # xmm3 = xmm2[0,0,2,2]
	pcmpeqd	%xmm1, %xmm0
	pshufd	$245, %xmm0, %xmm0      # xmm0 = xmm0[1,1,3,3]
	pand	%xmm3, %xmm0
	pshufd	$245, %xmm2, %xmm1      # xmm1 = xmm2[1,1,3,3]
	por	%xmm0, %xmm1
	movmskpd	%xmm1, %eax
	retq
.Lfunc_end0:
	.size	signmaskd, .Lfunc_end0-signmaskd
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4               # -- Begin function main
.LCPI1_0:
	.zero	16,51
.LCPI1_1:
	.quad	3694133962361549636     # 0x3344334433443344
	.quad	3694133962361549636     # 0x3344334433443344
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movaps	.LCPI1_0(%rip), %xmm0   # xmm0 = [3689348814741910323,3689348814741910323]
	movaps	%xmm0, 16(%rsp)
	movaps	16(%rsp), %xmm0
	movl	$.L.str, %edi
	callq	_Z14print_registerIDv2_xEvPKcT_
	movaps	.LCPI1_1(%rip), %xmm0   # xmm0 = [3694133962361549636,3694133962361549636]
	movaps	%xmm0, (%rsp)
	movaps	(%rsp), %xmm0
	movl	$.L.str.1, %edi
	callq	_Z14print_registerIDv2_xEvPKcT_
	movdqa	16(%rsp), %xmm1
	movdqa	(%rsp), %xmm0
	pcmpeqb	%xmm1, %xmm0
	movdqa	%xmm0, 32(%rsp)         # 16-byte Spill
	movl	$.L.str.2, %edi
	callq	_Z14print_registerIDv2_xEvPKcT_
	movaps	32(%rsp), %xmm0         # 16-byte Reload
	callq	signmaskd
	movzbl	%al, %esi
	movl	$.L.str.3, %edi
	xorl	%eax, %eax
	callq	printf
	xorl	%eax, %eax
	addq	$56, %rsp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	.text._Z14print_registerIDv2_xEvPKcT_,"axG",@progbits,_Z14print_registerIDv2_xEvPKcT_,comdat
	.weak	_Z14print_registerIDv2_xEvPKcT_ # -- Begin function _Z14print_registerIDv2_xEvPKcT_
	.p2align	4, 0x90
	.type	_Z14print_registerIDv2_xEvPKcT_,@function
_Z14print_registerIDv2_xEvPKcT_:        # @_Z14print_registerIDv2_xEvPKcT_
	.cfi_startproc
# %bb.0:
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movaps	%xmm0, (%rsp)           # 16-byte Spill
	movq	%rdi, %rcx
	movl	$.L.str.4, %edi
	xorl	%eax, %eax
	movq	%rcx, %rsi
	callq	printf
	movdqa	(%rsp), %xmm0           # 16-byte Reload
	movq	%xmm0, %r14
	pshufd	$78, %xmm0, %xmm0       # xmm0 = xmm0[2,3,0,1]
	movq	%xmm0, %rbx
	movq	%rbx, %rsi
	shrq	$56, %rsi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
                                        # kill: def %esi killed %esi killed %rsi
	callq	printf
	movq	%rbx, %rax
	shrq	$48, %rax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movq	%rbx, %rax
	shrq	$40, %rax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movq	%rbx, %rax
	shrq	$32, %rax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movl	%ebx, %ecx
	shrl	$24, %ecx
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	movl	%ecx, %esi
	callq	printf
	movl	%ebx, %eax
	shrl	$16, %eax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movzbl	%bh, %esi  # NOREX
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movzbl	%bl, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movq	%r14, %rsi
	shrq	$56, %rsi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
                                        # kill: def %esi killed %esi killed %rsi
	callq	printf
	movq	%r14, %rax
	movq	%r14, %rbx
	shrq	$48, %rax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movq	%rbx, %rax
	shrq	$40, %rax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movq	%rbx, %rax
	shrq	$32, %rax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movl	%ebx, %ecx
	shrl	$24, %ecx
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	movl	%ecx, %esi
	callq	printf
	movl	%ebx, %eax
	shrl	$16, %eax
	movzbl	%al, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movzbl	%bh, %esi  # NOREX
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movzbl	%bl, %esi
	movl	$.L.str.5, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$10, %edi
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	jmp	putchar                 # TAILCALL
.Lfunc_end2:
	.size	_Z14print_registerIDv2_xEvPKcT_, .Lfunc_end2-_Z14print_registerIDv2_xEvPKcT_
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"temp1"
	.size	.L.str, 6

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"temp2"
	.size	.L.str.1, 6

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"simd<8>::eq(temp1, temp2)"
	.size	.L.str.2, 26

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	"  hsimd<64>::signmask(rslt) = %u\n"
	.size	.L.str.3, 34

	.type	.L.str.4,@object        # @.str.4
.L.str.4:
	.asciz	"%40s = "
	.size	.L.str.4, 8

	.type	.L.str.5,@object        # @.str.5
.L.str.5:
	.asciz	"%02X "
	.size	.L.str.5, 6


	.ident	"clang version 3.7.1 (tags/RELEASE_371/final)"
	.section	".note.GNU-stack","",@progbits
