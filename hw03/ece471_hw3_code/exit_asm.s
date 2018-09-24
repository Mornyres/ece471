@ Syscall defines
.equ SYSCALL_EXIT,     1


        .globl _start
_start:

        @================================
        @ Exit
        @================================
exit:

	mov r0, #42 		@ move value 42 to register 0 (for return)
	mov r7, #SYSCALL_EXIT   @ move exit syscall to r7
	swi 0x0			@ make system call (r7)

