	.file	"swap.c"
	.text
	.globl	bufp0
	.data
	.align 8
bufp0:
	.quad	buf
.lcomm bufp1,8,8
	.text
	.globl	swap
	.def	swap;	.scl	2;	.type	32;	.endef
	.seh_proc	swap
swap:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movq	.refptr.buf(%rip), %rax
	addq	$4, %rax
	movq	%rax, bufp1(%rip)
	movq	bufp0(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	bufp1(%rip), %rdx
	movq	bufp0(%rip), %rax
	movl	(%rdx), %edx
	movl	%edx, (%rax)
	movq	bufp1(%rip), %rax
	movl	-4(%rbp), %edx
	movl	%edx, (%rax)
	nop
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
	.section	.rdata$.refptr.buf, "dr"
	.globl	.refptr.buf
	.linkonce	discard
.refptr.buf:
	.quad	buf
