@.syntax unified

@ Syscall Definitions
.equ SYSCALL_EXIT,     1
.equ SYSCALL_WRITE,    4

@ Other Definitions
.equ STDOUT,	       1

        .globl _start
_start:

	mov	r0,#0

	@==========================================
	@ Your code for part 3d goes here =\
	@                                  \/
	@==========================================
printloop:
	mov r0, r6	@ move current line to r0
	cmp r0, #15	@ continue if and only if fewer than 15 lines have been printed
	bgt exit

	@==========================================

	bl	print_number		@ print r0 as a decimal number

	ldr	r1,=message		@ load message
	blx	print_string		@ print it -- modified to allow THUMB

	@==========================================
	@ Your Code for part 3d Also goes Here  =\
	@                                        \/
	@==========================================
	add r6,r6, #1	@increment current line number
	b printloop
	@==========================================



        @================================
        @ Exit
        @================================
exit:
	mov	r0,#0			@ Return a zero
        mov	r7,#SYSCALL_EXIT	@
        swi	0x0			@ Run the system call


	@====================
	@ print_string
	@====================
	@ Null-terminated string to print pointed to by r1
	@ the value in r1 is destroyed by this routine


.thumb

print_string:

	push    {r0,r2,r3,r7,lr}	@ Save r0,r2,r7,r10,lr on stack -- modified to use r3 instead of r10

	mov	r2,#3			@ The wrong value of r2
					@ Just so the code runs
					@ Your code below will overwrite
					@ with the proper version

	@==========================================
	@ Your code for part 3b goes here =\
	@                                  \/
	@==========================================
	
	mov r4, r1	@ this is the base string address which will be offset
	mov r2, #0	@ r2 will contain the string length; starting at 0th char

charcount:
	ldrb r3,[r4]		@load register byte r4 into r3 (length = current char offset)
	cmp r3, #0		@reached NULL terminator yet?
	beq print_string_EXIT	@if yes, exit subroutine
		
	add r2, r2, #1		@increment length
	add r4,r4, #1		@increment string pointer
	b charcount

print_string_EXIT:

	@==========================================
	@ The length of the string pointed to by r1
	@ Should be put in r2 by your code above
	@==========================================

	mov	r0,#STDOUT		@ R0 Print to stdout
					@ R1 points to our string
					@ R2 is the length
	mov	r7,#SYSCALL_WRITE	@ Load syscall number
	swi	0x0			@ System call

	pop	{r0,r2,r3,r7,pc}       @ pop r0,r2,r3,pc from stack

.arm

        @#############################
	@ print_number
	@#############################
	@ r0 = value to print
	@ r0 is destroyed by this routine

print_number:
	push	{r10,LR}	@ Save r10 and return address on stack
        ldr	r10,=buffer	@ Point to beginning of buffer
        add	r10,r10,#10	@ Offset buffer address by 10

divide:
	bl	divide_by_10	@ divide by 10
	add	r8,r8,#0x30	@ Add 0x30 (which is a null char here) to the remainder of divide_by_10, which is stored in r8
	strb	r8,[r10],#-1	@ store to buffer, increment pointer
	adds	r0,r7,#0	@ move quotient to r0, update status flag
	bne	divide		@ if quotient not zero then loop

write_out:
        add	r1,r10,#1	@ adjust pointer to print to beginning

        blx	print_string	@ print the string

        pop	{r10,LR}	@ restore saved values from stack

        mov	pc,lr		@ return from function

	##############################
	# divide_by_10
	##############################
	# r0=numerator
        # r4,r5 trashed
	# r7 = quotient, r8 = remainder

divide_by_10:
	ldr	r4,=429496730		@ 1/10 * 2^32
	sub	r5,r0,r0,lsr #30
	umull	r8,r7,r4,r5		@ {r8,r7}=r4*r5

	mov	r4,#10			@ calculate remainder
	mul	r8,r7,r4
	sub	r8,r0,r8

	mov	pc,lr

.data
message:	.string ": ECE471 is cool\n"

@ BSS
.lcomm buffer,11
