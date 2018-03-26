	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	movb	$5, (%rdi)
	movb	$6, (%rdi)
	movb	$0, (%rdi)
	movb	$7, (%rdi)
	movq	%rdi, %rax
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__const
l_.str:                                 ## @.str
	.asciz	"<6 x i3><%i, %i, %i, %i, %i, %i>\n"


.subsections_via_symbols
