	.file	"syscall.c"
	.text
	.globl	linux_syscall0
	.type	linux_syscall0, @function
linux_syscall0:
.LFB0:
#	.cfi_startproc
	pushq	%rbp
#	.cfi_def_cfa_offset 16
#	.cfi_offset 6, -16
	movq	%rsp, %rbp
#	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
#APP
# 3 "syscall.c" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
#	.cfi_def_cfa 7, 8
	ret
#	.cfi_endproc
.LFE0:
	.size	linux_syscall0, .-linux_syscall0
	.globl	linux_syscall1
	.type	linux_syscall1, @function
linux_syscall1:
.LFB1:
#	.cfi_startproc
	pushq	%rbp
#	.cfi_def_cfa_offset 16
#	.cfi_offset 6, -16
	movq	%rsp, %rbp
#	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rdi
#APP
# 9 "syscall.c" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
#	.cfi_def_cfa 7, 8
	ret
#	.cfi_endproc
.LFE1:
	.size	linux_syscall1, .-linux_syscall1
	.globl	linux_syscall2
	.type	linux_syscall2, @function
linux_syscall2:
.LFB2:
#	.cfi_startproc
	pushq	%rbp
#	.cfi_def_cfa_offset 16
#	.cfi_offset 6, -16
	movq	%rsp, %rbp
#	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdx
	movq	-40(%rbp), %rsi
	movq	%rdx, %rdi
#APP
# 15 "syscall.c" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
#	.cfi_def_cfa 7, 8
	ret
#	.cfi_endproc
.LFE2:
	.size	linux_syscall2, .-linux_syscall2
	.globl	linux_syscall3
	.type	linux_syscall3, @function
linux_syscall3:
.LFB3:
#	.cfi_startproc
	pushq	%rbp
#	.cfi_def_cfa_offset 16
#	.cfi_offset 6, -16
	movq	%rsp, %rbp
#	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdi
	movq	-40(%rbp), %rsi
	movq	-48(%rbp), %rdx
#APP
# 22 "syscall.c" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
#	.cfi_def_cfa 7, 8
	ret
#	.cfi_endproc
.LFE3:
	.size	linux_syscall3, .-linux_syscall3
	.globl	linux_syscall4
	.type	linux_syscall4, @function
linux_syscall4:
.LFB4:
#	.cfi_startproc
	pushq	%rbp
#	.cfi_def_cfa_offset 16
#	.cfi_offset 6, -16
	movq	%rsp, %rbp
#	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	%r8, -56(%rbp)
	movq	-56(%rbp), %r10
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdi
	movq	-40(%rbp), %rsi
	movq	-48(%rbp), %rdx
#APP
# 30 "syscall.c" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
#	.cfi_def_cfa 7, 8
	ret
#	.cfi_endproc
.LFE4:
	.size	linux_syscall4, .-linux_syscall4
	.globl	linux_syscall5
	.type	linux_syscall5, @function
linux_syscall5:
.LFB5:
#	.cfi_startproc
	pushq	%rbp
#	.cfi_def_cfa_offset 16
#	.cfi_offset 6, -16
	movq	%rsp, %rbp
#	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	%r8, -56(%rbp)
	movq	%r9, -64(%rbp)
	movq	-56(%rbp), %r10
	movq	-64(%rbp), %r8
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdi
	movq	-40(%rbp), %rsi
	movq	-48(%rbp), %rdx
#APP
# 39 "syscall.c" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
#	.cfi_def_cfa 7, 8
	ret
#	.cfi_endproc
.LFE5:
	.size	linux_syscall5, .-linux_syscall5
	.globl	linux_syscall6
	.type	linux_syscall6, @function
linux_syscall6:
.LFB6:
#	.cfi_startproc
	pushq	%rbp
#	.cfi_def_cfa_offset 16
#	.cfi_offset 6, -16
	movq	%rsp, %rbp
#	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movq	%rcx, -48(%rbp)
	movq	%r8, -56(%rbp)
	movq	%r9, -64(%rbp)
	movq	-56(%rbp), %r10
	movq	-64(%rbp), %r8
	movq	16(%rbp), %r9
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rdi
	movq	-40(%rbp), %rsi
	movq	-48(%rbp), %rdx
#APP
# 49 "syscall.c" 1
	syscall
# 0 "" 2
#NO_APP
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
#	.cfi_def_cfa 7, 8
	ret
#	.cfi_endproc
.LFE6:
	.size	linux_syscall6, .-linux_syscall6
	.ident	"GCC: (Debian 8.2.0-14) 8.2.0"
	.section	.note.GNU-stack,"",@progbits
