	.text











	.globl	GetCursorPosNoFail              # -- Begin function GetCursorPosNoFail
	.p2align	4, 0x90
GetCursorPosNoFail:                     # @GetCursorPosNoFail

	pushq	%rsi
	subq	$48, %rsp
	leaq	40(%rsp), %rsi
	.p2align	4, 0x90
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	movq	%rsi, %rcx
	callq	GetCursorPos
	testl	%eax, %eax
	je	.LBB0_1

	movq	40(%rsp), %rax
	addq	$48, %rsp
	popq	%rsi
	retq
                                        # -- End function




	.globl	GetWindowRectNoFail             # -- Begin function GetWindowRectNoFail
	.p2align	4, 0x90
GetWindowRectNoFail:                    # @GetWindowRectNoFail

	pushq	%rsi
	pushq	%rdi
	subq	$40, %rsp
	movq	%rdx, %rdi
	movq	%rcx, %rsi
	.p2align	4, 0x90
.LBB1_1:                                # =>This Inner Loop Header: Depth=1
	movq	%rdi, %rcx
	movq	%rsi, %rdx
	callq	GetWindowRect
	testl	%eax, %eax
	je	.LBB1_1

	movq	%rsi, %rax
	addq	$40, %rsp
	popq	%rdi
	popq	%rsi
	retq
                                        # -- End function




	.globl	GetCloseButtonCenter            # -- Begin function GetCloseButtonCenter
	.p2align	4, 0x90
GetCloseButtonCenter:                   # @GetCloseButtonCenter

	pushq	%rsi
	pushq	%rdi
	subq	$56, %rsp
	movq	%rcx, %rsi
	leaq	40(%rsp), %rdi
	.p2align	4, 0x90
.LBB2_1:                                # =>This Inner Loop Header: Depth=1
	movq	%rsi, %rcx
	movq	%rdi, %rdx
	callq	GetWindowRect
	testl	%eax, %eax
	je	.LBB2_1
	.p2align	4, 0x90
.LBB2_2:                                # =>This Inner Loop Header: Depth=1
	movl	$30, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB2_2

	movl	%eax, %esi
	.p2align	4, 0x90
.LBB2_4:                                # =>This Inner Loop Header: Depth=1
	movl	$32, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB2_4

	movl	%eax, %edi
	.p2align	4, 0x90
.LBB2_6:                                # =>This Inner Loop Header: Depth=1
	movl	$4, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB2_6

	movl	48(%rsp), %ecx
	movl	%esi, %edx
	shrl	$31, %edx
	addl	%esi, %edx
	sarl	%edx
	addl	%edi, %edx
	movl	%eax, %r8d
	shrl	$31, %r8d
	addl	%eax, %r8d
	sarl	%r8d
	addl	%edi, %r8d
	addl	44(%rsp), %r8d
	subl	%edx, %ecx
	shlq	$32, %r8
	orq	%r8, %rcx
	movq	%rcx, %rax
	addq	$56, %rsp
	popq	%rdi
	popq	%rsi
	retq
                                        # -- End function




	.globl	spawnThought                    # -- Begin function spawnThought
	.p2align	4, 0x90
spawnThought:                           # @spawnThought

	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rsi
	pushq	%rdi
	pushq	%rbp
	pushq	%rbx
	subq	$120, %rsp
	movl	%edx, %edi
	movq	%rcx, %rsi
	.p2align	4, 0x90
.LBB3_1:                                # =>This Inner Loop Header: Depth=1
	movl	$30, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB3_1

	movl	%eax, %ebp
	.p2align	4, 0x90
.LBB3_3:                                # =>This Inner Loop Header: Depth=1
	movl	$32, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB3_3

	movl	%eax, %ebx
	.p2align	4, 0x90
.LBB3_5:                                # =>This Inner Loop Header: Depth=1
	movl	$4, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB3_5

	movl	%eax, %r14d
	movl	24(%rsi), %eax
	addl	$1, %eax
	movl	%eax, 24(%rsi)
	imull	%ebp, %eax
	movl	%eax, 28(%rsi)
	movq	8(%rsi), %rcx
	callq	GetDC
	movq	%rax, %r15
	movl	24(%rsi), %eax
	leal	7(%rax), %ecx
	testl	%eax, %eax
	cmovnsl	%eax, %ecx
	andl	$-8, %ecx
	subl	%ecx, %eax
	cltq
	leaq	chThts(%rip), %rcx
	movq	(%rcx,%rax,8), %r12
	leaq	96(%rsp), %r13
	.p2align	4, 0x90
.LBB3_7:                                # =>This Inner Loop Header: Depth=1
	movq	%r12, %rcx
	callq	lstrlenA
	movq	%r15, %rcx
	movq	%r12, %rdx
	movl	%eax, %r8d
	movq	%r13, %r9
	callq	GetTextExtentPoint32A
	testl	%eax, %eax
	je	.LBB3_7

	movq	8(%rsi), %rcx
	movq	%r15, %rdx
	callq	ReleaseDC
	movl	100(%rsp), %eax
	leal	(%rax,%rbx,2), %ecx
	addl	%ecx, 96(%rsp)
	addl	%ebx, %r14d
	leal	(%r14,%rax,2), %eax
	movl	%eax, 100(%rsp)
	movq	(%rsi), %rcx
	leaq	104(%rsp), %rdx
	callq	GetWindowRect
	movl	112(%rsp), %eax
	subl	104(%rsp), %eax
	movl	116(%rsp), %ecx
	movl	%eax, %ebp
	shrl	$31, %ebp
	addl	%eax, %ebp
	sarl	%ebp
	subl	108(%rsp), %ecx
	movl	%ecx, %r14d
	shrl	$31, %r14d
	addl	%ecx, %r14d
	sarl	%r14d
	leaq	.str.8(%rip), %rbx
	.p2align	4, 0x90
.LBB3_9:                                # =>This Inner Loop Header: Depth=1
	movq	(%rsi), %rax
	movq	16(%rsi), %rcx
	movq	24(%rcx), %r8
	movq	64(%rcx), %rdx
	movl	96(%rsp), %ecx
	movl	100(%rsp), %r9d
	movq	%rsi, 88(%rsp)
	movq	%r8, 80(%rsp)
	movq	%rax, 64(%rsp)
	movl	%r9d, 56(%rsp)
	movl	%ecx, 48(%rsp)
	movl	%r14d, 40(%rsp)
	movl	%ebp, 32(%rsp)
	movq	$0, 72(%rsp)
	xorl	%ecx, %ecx
	movq	%rbx, %r8
	movl	$281542656, %r9d                # imm = 0x10C80000
	callq	CreateWindowExA
	movq	%rax, 8(%rsi)
	testq	%rax, %rax
	jne	.LBB3_11

	testl	%edi, %edi
	je	.LBB3_9
	jmp	.LBB3_12
.LBB3_11:
	movq	%rax, %rcx
	xorl	%edx, %edx
	movl	$1, %r8d
	callq	InvalidateRect
	movq	8(%rsi), %rcx
	callq	UpdateWindow
	movq	(%rsi), %rcx
	xorl	%edx, %edx
	movl	$1, %r8d
	callq	InvalidateRect
	movq	(%rsi), %rcx
	callq	UpdateWindow
.LBB3_12:
	addq	$120, %rsp
	popq	%rbx
	popq	%rbp
	popq	%rdi
	popq	%rsi
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	retq
                                        # -- End function




	.globl	MainWndProc                     # -- Begin function MainWndProc
	.p2align	4, 0x90
MainWndProc:                            # @MainWndProc

	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rsi
	pushq	%rdi
	pushq	%rbp
	pushq	%rbx
	subq	$216, %rsp
	movq	%r9, %rsi
	movq	%r8, %rdi
	movl	%edx, %ebx
	movq	%rcx, %r14
	cmpl	$1, %edx
	jne	.LBB4_2

	movq	(%rsi), %r8
	movq	%r14, %rcx
	movl	$-21, %edx
	callq	SetWindowLongPtrA
	jmp	.LBB4_44
.LBB4_2:
	movq	%r14, %rcx
	movl	$-21, %edx
	callq	GetWindowLongPtrA
	cmpl	$511, %ebx                      # imm = 0x1FF
	jle	.LBB4_3

	cmpl	$516, %ebx                      # imm = 0x204
	je	.LBB4_38

	cmpl	$513, %ebx                      # imm = 0x201
	je	.LBB4_40

	cmpl	$512, %ebx                      # imm = 0x200
	jne	.LBB4_44

	movq	%rax, %r15
	leaq	104(%rsp), %r12
	.p2align	4, 0x90
.LBB4_11:                               # =>This Inner Loop Header: Depth=1
	movq	%r12, %rcx
	callq	GetCursorPos
	testl	%eax, %eax
	je	.LBB4_11

	movq	104(%rsp), %rax
	movq	%rax, 96(%rsp)                  # 8-byte Spill
	shrq	$32, %rax
	movq	%rax, 88(%rsp)                  # 8-byte Spill
	movq	%r15, 72(%rsp)                  # 8-byte Spill
	movq	8(%r15), %r12
	leaq	104(%rsp), %r13
	.p2align	4, 0x90
.LBB4_13:                               # =>This Inner Loop Header: Depth=1
	movq	%r12, %rcx
	movq	%r13, %rdx
	callq	GetWindowRect
	testl	%eax, %eax
	je	.LBB4_13
	.p2align	4, 0x90
.LBB4_14:                               # =>This Inner Loop Header: Depth=1
	movl	$30, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB4_14

	movl	%eax, %ebp
	movq	72(%rsp), %r12                  # 8-byte Reload
	.p2align	4, 0x90
.LBB4_16:                               # =>This Inner Loop Header: Depth=1
	movl	$32, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB4_16

	movl	%eax, 68(%rsp)                  # 4-byte Spill
	.p2align	4, 0x90
.LBB4_18:                               # =>This Inner Loop Header: Depth=1
	movl	$4, %ecx
	callq	GetSystemMetrics
	testl	%eax, %eax
	je	.LBB4_18

	movl	108(%rsp), %ecx
	movl	%ecx, 80(%rsp)                  # 4-byte Spill
	movl	112(%rsp), %ecx
	movl	%ecx, 84(%rsp)                  # 4-byte Spill
	movl	%eax, %r15d
	shrl	$31, %r15d
	addl	%eax, %r15d
	sarl	%r15d
	addl	68(%rsp), %r15d                 # 4-byte Folded Reload
	movq	8(%r12), %r12
	leaq	104(%rsp), %r13
	.p2align	4, 0x90
.LBB4_20:                               # =>This Inner Loop Header: Depth=1
	movq	%r12, %rcx
	movq	%r13, %rdx
	callq	GetWindowRect
	testl	%eax, %eax
	je	.LBB4_20

	movl	%ebp, %eax
	shrl	$31, %ebp
	addl	%eax, %ebp
	sarl	%ebp
	negl	%ebp
	movq	72(%rsp), %rax                  # 8-byte Reload
	movq	(%rax), %r12
	leaq	176(%rsp), %r13
	.p2align	4, 0x90
.LBB4_22:                               # =>This Inner Loop Header: Depth=1
	movq	%r12, %rcx
	movq	%r13, %rdx
	callq	GetWindowRect
	testl	%eax, %eax
	je	.LBB4_22

	movq	72(%rsp), %r10                  # 8-byte Reload
	movl	28(%r10), %eax
	movl	68(%rsp), %ecx                  # 4-byte Reload
	addl	96(%rsp), %ecx                  # 4-byte Folded Reload
	subl	%ecx, %ebp
	addl	84(%rsp), %ebp                  # 4-byte Folded Reload
	movl	%eax, %ecx
	negl	%ecx
	cmpl	%ecx, %ebp
	jl	.LBB4_44

	cmpl	%eax, %ebp
	jg	.LBB4_44

	addl	80(%rsp), %r15d                 # 4-byte Folded Reload
	movl	%r15d, %r9d
	subl	88(%rsp), %r9d                  # 4-byte Folded Reload
	cmpl	%ecx, %r9d
	jl	.LBB4_44

	cmpl	%eax, %r9d
	jg	.LBB4_44

	movl	104(%rsp), %eax
	movl	108(%rsp), %ecx
	movl	%eax, %r8d
	subl	112(%rsp), %r8d
	addl	184(%rsp), %r8d
	subl	116(%rsp), %ecx
	addl	%r15d, %r9d
	addl	188(%rsp), %ecx
	addl	%eax, %ebp
	js	.LBB4_28

	cmpl	%r8d, %ebp
	jl	.LBB4_31

	movl	%ebp, %eax
	cltd
	idivl	%r8d
	movl	%edx, %ebp
	jmp	.LBB4_31
.LBB4_3:
	cmpl	$2, %ebx
	je	.LBB4_41

	cmpl	$15, %ebx
	jne	.LBB4_44

	leaq	104(%rsp), %rdx
	movq	%r14, %rcx
	movq	%rax, %r15
	callq	BeginPaint
	movq	%rax, %r12
	movq	%r15, %rax
	cmpq	%r14, (%r15)
	je	.LBB4_6

	movl	24(%rax), %eax
	leal	7(%rax), %ecx
	testl	%eax, %eax
	cmovnsl	%eax, %ecx
	andl	$-8, %ecx
	subl	%ecx, %eax
	cltq
	leaq	chThts(%rip), %rcx
	movq	(%rcx,%rax,8), %r15
	movq	%r15, %rcx
	callq	lstrlenA
	movl	%eax, 32(%rsp)
	movq	%r12, %rcx
	xorl	%edx, %edx
	xorl	%r8d, %r8d
	jmp	.LBB4_37
.LBB4_38:
	leaq	.str.10(%rip), %rdx
	leaq	.str.11(%rip), %r8
	jmp	.LBB4_39
.LBB4_40:
	leaq	.str.12(%rip), %rdx
	leaq	.str.13(%rip), %r8
.LBB4_39:
	movq	%r14, %rcx
	movl	$48, %r9d
	callq	MessageBoxA
	jmp	.LBB4_44
.LBB4_41:
	cmpq	%r14, (%rax)
	je	.LBB4_42

	movq	%rax, %rcx
	movl	$1, %edx
	callq	spawnThought
	jmp	.LBB4_44
.LBB4_42:
	xorl	%ecx, %ecx
	callq	PostQuitMessage
	xorl	%eax, %eax
	jmp	.LBB4_45
.LBB4_6:
	movl	24(%rax), %r8d
	leaq	.str.9(%rip), %rdx
	leaq	176(%rsp), %r15
	movq	%r15, %rcx
	callq	wsprintfA
	movq	%r15, %rcx
	callq	lstrlenA
	movl	%eax, 32(%rsp)
	movq	%r12, %rcx
	movl	$10, %edx
	movl	$10, %r8d
.LBB4_37:
	movq	%r15, %r9
	callq	TextOutA
	leaq	104(%rsp), %rdx
	movq	%r14, %rcx
	callq	EndPaint
	jmp	.LBB4_44
.LBB4_28:
	addl	%r8d, %ebp
.LBB4_31:
	testl	%r9d, %r9d
	js	.LBB4_32

	cmpl	%ecx, %r9d
	jl	.LBB4_35

	movl	%r9d, %eax
	cltd
	idivl	%ecx
	movl	%edx, %r9d
	jmp	.LBB4_35
.LBB4_32:
	addl	%ecx, %r9d
.LBB4_35:
	movq	8(%r10), %rcx
	movl	$65, 48(%rsp)
	movl	$-2147483648, 40(%rsp)          # imm = 0x80000000
	movl	$-2147483648, 32(%rsp)          # imm = 0x80000000
	xorl	%edx, %edx
	movl	%ebp, %r8d
	callq	SetWindowPos
.LBB4_44:
	movq	%r14, %rcx
	movl	%ebx, %edx
	movq	%rdi, %r8
	movq	%rsi, %r9
	callq	DefWindowProcA
.LBB4_45:
	addq	$216, %rsp
	popq	%rbx
	popq	%rbp
	popq	%rdi
	popq	%rsi
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	retq
                                        # -- End function




	.globl	WinMain                         # -- Begin function WinMain
	.p2align	4, 0x90
WinMain:                                # @WinMain

	pushq	%rsi
	pushq	%rdi
	subq	$280, %rsp                      # imm = 0x118
	movaps	%xmm6, 256(%rsp)                # 16-byte Spill
	xorl	%ecx, %ecx
	callq	GetModuleHandleA
	movq	%rax, %rsi
	movl	$0, 136(%rsp)
	leaq	MainWndProc(%rip), %rax
	movq	%rax, 144(%rsp)
	movq	$0, 152(%rsp)
	movq	%rsi, 160(%rsp)
	movq	$0, 168(%rsp)
	movl	$32515, %edx                    # imm = 0x7F03
	movq	%rsi, %rcx
	callq	LoadCursorA
	movq	%rax, 176(%rsp)
	movq	$5, 184(%rsp)
	movq	$0, 192(%rsp)
	leaq	.str.14(%rip), %rax
	movq	%rax, 200(%rsp)
	leaq	136(%rsp), %rdi
	.p2align	4, 0x90
.LBB5_1:                                # =>This Inner Loop Header: Depth=1
	movq	%rdi, %rcx
	callq	RegisterClassA
	testw	%ax, %ax
	je	.LBB5_1

	xorps	%xmm6, %xmm6
	movaps	%xmm6, 112(%rsp)
	movaps	%xmm6, 96(%rsp)
	movq	%rdi, 112(%rsp)
	leaq	96(%rsp), %rdi
	.p2align	4, 0x90
.LBB5_3:                                # =>This Inner Loop Header: Depth=1
	movq	200(%rsp), %rdx
	movq	%rdi, 88(%rsp)
	movq	%rsi, 80(%rsp)
	movups	%xmm6, 64(%rsp)
	movl	$-2147483648, 56(%rsp)          # imm = 0x80000000
	movl	$-2147483648, 48(%rsp)          # imm = 0x80000000
	movl	$0, 40(%rsp)
	movl	$0, 32(%rsp)
	xorl	%ecx, %ecx
	movq	%rdx, %r8
	movl	$297795584, %r9d                # imm = 0x11C00000
	callq	CreateWindowExA
	movq	%rax, 96(%rsp)
	testq	%rax, %rax
	je	.LBB5_3

	movq	%rax, %rcx
	movl	$3, %edx
	callq	ShowWindow
	leaq	96(%rsp), %rcx
	xorl	%edx, %edx
	callq	spawnThought
	leaq	208(%rsp), %rcx
	xorl	%edx, %edx
	xorl	%r8d, %r8d
	xorl	%r9d, %r9d
	callq	GetMessageA
	testl	%eax, %eax
	je	.LBB5_7

	leaq	208(%rsp), %rsi
	.p2align	4, 0x90
.LBB5_6:                                # =>This Inner Loop Header: Depth=1
	movq	%rsi, %rcx
	callq	TranslateMessage
	movq	%rsi, %rcx
	callq	DispatchMessageA
	movq	%rsi, %rcx
	xorl	%edx, %edx
	xorl	%r8d, %r8d
	xorl	%r9d, %r9d
	callq	GetMessageA
	testl	%eax, %eax
	jne	.LBB5_6
.LBB5_7:
	xorl	%ecx, %ecx
	callq	ExitProcess
	xorl	%eax, %eax
	movaps	256(%rsp), %xmm6                # 16-byte Reload
	addq	$280, %rsp                      # imm = 0x118
	popq	%rdi
	popq	%rsi
	retq
                                        # -- End function
	.section	.rdata,"dr",one_only,.str
.str:                                   # @.str
	.asciz	"Turning the alarm off and sleep."

	.section	.rdata,"dr",one_only,.str.1
.str.1:                                 # @.str.1
	.asciz	"Computer hard. Become Amish."

	.section	.rdata,"dr",one_only,.str.2
.str.2:                                 # @.str.2
	.asciz	"Do an AI startup."

	.section	.rdata,"dr",one_only,.str.3
.str.3:                                 # @.str.3
	.asciz	"Invest in Apple, Inc."

	.section	.rdata,"dr",one_only,.str.4
.str.4:                                 # @.str.4
	.asciz	"Kidnapper? I barely know 'er."

	.section	.rdata,"dr",one_only,.str.5
.str.5:                                 # @.str.5
	.asciz	"The knives are speaking to me."

	.section	.rdata,"dr",one_only,.str.6
.str.6:                                 # @.str.6
	.asciz	"Skin yourself."

	.section	.rdata,"dr",one_only,.str.7
.str.7:                                 # @.str.7
	.asciz	"TBD"

	.section	.data$chThts,"dw",one_only,chThts
	.globl	chThts                          # @chThts
	.p2align	4, 0x0
chThts:
	.quad	.str
	.quad	.str.1
	.quad	.str.2
	.quad	.str.3
	.quad	.str.4
	.quad	.str.5
	.quad	.str.6
	.quad	.str.7

	.section	.rdata$cntCnThts,"dr",one_only,cntCnThts
	.globl	cntCnThts                       # @cntCnThts
	.p2align	2, 0x0
cntCnThts:
	.long	8                               # 0x8

	.section	.rdata,"dr",one_only,.str.8
.str.8:                                 # @.str.8
	.asciz	"the voice"

	.section	.rdata,"dr",one_only,.str.9
.str.9:                                 # @.str.9
	.asciz	"MENTAL ILLNESS LEVEL: %d"

	.section	.rdata,"dr",one_only,.str.10
.str.10:                                # @.str.10
	.asciz	"Holy sheet bro. Damn."

	.section	.rdata,"dr",one_only,.str.11
.str.11:                                # @.str.11
	.asciz	"!"

	.section	.rdata,"dr",one_only,.str.12
.str.12:                                # @.str.12
	.asciz	"Woah there buddy. Calm down."

	.section	.rdata,"dr",one_only,.str.13
.str.13:                                # @.str.13
	.asciz	"?"

	.section	.rdata,"dr",one_only,.str.14
.str.14:                                # @.str.14
	.asciz	"intrusive thoughts"

	.addrsig
	.addrsig_sym MainWndProc
