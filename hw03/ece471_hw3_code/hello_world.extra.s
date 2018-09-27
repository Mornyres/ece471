.syntax unified

@ Syscall Definitions
.equ SYSCALL_EXIT,	1
.equ SYSCALL_WRITE,	4
.equ SWI_WrBytes,	0x69
.equ SWI_RdBytes,	0x6A

@ Other Definitions
.equ STDOUT,	1
.equ STDIN,	0
.equ buflen,	10


        .globl _start
_start:

	
	@==========================================
	@ Something cool first attempt: getting STDIN input line num argument
	@                                  
	@==========================================

	/*
	If it were to work, this is how:
		It formats a template string, calls a STDIN syscall which accepts characters up to some delimiter and stores it in the template string. I think you would need to manually convert the received chars to an int. This gets passed to the loop section as the line number argument.

	add r1, =inputbuffer
	mov r2, r1, #buflen

getinput:
	swi SWI_RdBytes
	strb r0, [r1], #1
	cmp r1, r2
	blo getinput
	cmp r0, #'.'
	bne getinput

	mov r0,STDOUT
	adr r1, =inputbuffer
	mov r2,#2
	swi SWI_WrBytes	
	
	*/
	
	@==========================================
	@ Something cool 2nd attempt: descending line numbers
	@                                  
	@==========================================
	mov	r6,#15	@ starting instead at 15 for descent
printloop:
	mov r0, r6	@ move current line to r0
	cmp r0, #0	@ continue if and only if fewer than 15 lines have been printed
	blt exit	@ this becomes less than to break if line 0 is reached

	@==========================================

	bl	print_number		@ print r0 as a decimal number

	ldr	r1,=message		@ load message
	bl	print_string		@ print it

	@==========================================
	@ your code for part 3d also goes here  =\
	@                                        \/
	@==========================================
	sub r6,r6, #1	@increment current line number
	b printloop
	@==========================================



        @================================
        @ exit
        @================================
exit:
	mov	r0,#0			@ return a zero
        mov	r7,#SYSCALL_EXIT
        swi	0x0			@ run the system call


	@====================
	@ print_string
	@====================
	@ null-terminated string to print pointed to by r1
	@ the value in r1 is destroyed by this routine


@.thumb

print_string:

	push    {r0,r2,r7,r10,lr}	@ save r0,r2,r7,r10,lr on stack

	mov	r2,#3			@ the wrong value of r2
					@ just so the code runs
					@ your code below will overwrite
					@ with the proper version

	@==========================================
	@ your code for part 3b goes here =\
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

	pop	{r0,r2,r7,r10,pc}       @ pop r0,r2,r3,pc from stack

@.arm

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
	add	r8,r8,#0x30	@ Add 0x30 to the remainder of divide_by_10, which is stored in r8
	strb	r8,[r10],#-1	@ store to buffer, increment pointer
	adds	r0,r7,#0	@ move quotient to r0, update status flag
	bne	divide		@ if quotient not zero then loop

write_out:
        add	r1,r10,#1	@ adjust pointer to print to beginning

        bl	print_string	@ print the string

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

/* More stuff for first SC attempt
inputprompt:	.string "Print how many lines? 2 digits please:\n"
inputvalue:	.word 0

outputtemplate:	.string	"Printing %d lines\n"
*/

@ BSS
.lcomm buffer,11
.lcomm inputbuffer,10
