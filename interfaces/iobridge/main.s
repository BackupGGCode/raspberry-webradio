	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	Rotary_Read
	.type	Rotary_Read, @function
Rotary_Read:
/* prologue: function */
/* frame size = 0 */
	lds r24,last.1360
	lsl r24
	lsl r24
	andi r24,lo8(12)
	sts last.1360,r24
	sbis 48-32,5
	rjmp .L2
	ori r24,lo8(2)
	sts last.1360,r24
.L2:
	sbis 48-32,6
	rjmp .L3
	ori r24,lo8(1)
	sts last.1360,r24
.L3:
	mov r30,r24
	clr r31
	sbrc r30,7
	com r31
	subi r30,lo8(-(table))
	sbci r31,hi8(-(table))
/* #APP */
 ;  41 "main.c" 1
	lpm r30, Z
	
 ;  0 "" 2
/* #NOAPP */
	lds r24,enc_delta
	add r30,r24
	sts enc_delta,r30
/* epilogue start */
	ret
	.size	Rotary_Read, .-Rotary_Read
.global	Rotary_Get
	.type	Rotary_Get, @function
Rotary_Get:
/* prologue: function */
/* frame size = 0 */
	in r24,89-32
	andi r24,lo8(-3)
	out 89-32,r24
	lds r24,enc_delta
	sts enc_delta,__zero_reg__
	in r25,89-32
	ori r25,lo8(2)
	out 89-32,r25
/* epilogue start */
	ret
	.size	Rotary_Get, .-Rotary_Get
.global	Buttons_Read
	.type	Buttons_Read, @function
Buttons_Read:
/* prologue: function */
/* frame size = 0 */
	ldi r20,lo8(1)
	ldi r18,lo8(0)
	ldi r19,hi8(0)
	rjmp .L15
.L19:
	lds r24,button_must_release
	or r24,r20
	sts button_must_release,r24
	movw r30,r18
	subi r30,lo8(-(button_time))
	sbci r31,hi8(-(button_time))
	ld r24,Z
	cpi r24,lo8(80)
	brlo .L17
	ld r24,Z
	cpi r24,lo8(80)
	breq .L18
.L11:
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	cpi r18,8
	cpc r19,__zero_reg__
	breq .L16
.L20:
	lsl r20
.L15:
	in r24,54-32
	and r24,r20
	brne .L19
	lds r24,button_must_release
	and r24,r20
	breq .L11
	movw r30,r18
	subi r30,lo8(-(button_time))
	sbci r31,hi8(-(button_time))
	ld r24,Z
	tst r24
	brne .L12
	lds r24,buttons
	or r24,r20
	sts buttons,r24
.L13:
	lds r25,button_must_release
	mov r24,r20
	com r24
	and r24,r25
	sts button_must_release,r24
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	cpi r18,8
	cpc r19,__zero_reg__
	brne .L20
.L16:
	ret
.L18:
	lds r24,buttons_long
	or r24,r20
	sts buttons_long,r24
.L17:
	ld r24,Z
	subi r24,lo8(-(1))
	st Z,r24
	rjmp .L11
.L12:
	st Z,__zero_reg__
	rjmp .L13
	.size	Buttons_Read, .-Buttons_Read
.global	Buttons_Get
	.type	Buttons_Get, @function
Buttons_Get:
/* prologue: function */
/* frame size = 0 */
	mov r21,r24
	in r24,89-32
	andi r24,lo8(-3)
	out 89-32,r24
	lds r24,buttons
	lds r20,buttons
	ldi r18,lo8(1)
	ldi r19,hi8(1)
	mov r0,r21
	rjmp 2f
1:	lsl r18
	rol r19
2:	dec r0
	brpl 1b
	com r18
	and r18,r20
	sts buttons,r18
	in r18,89-32
	ori r18,lo8(2)
	out 89-32,r18
	ldi r25,lo8(0)
	rjmp 2f
1:	asr r25
	ror r24
2:	dec r21
	brpl 1b
	andi r24,lo8(1)
/* epilogue start */
	ret
	.size	Buttons_Get, .-Buttons_Get
.global	Buttons_GetLong
	.type	Buttons_GetLong, @function
Buttons_GetLong:
/* prologue: function */
/* frame size = 0 */
	mov r21,r24
	in r24,89-32
	andi r24,lo8(-3)
	out 89-32,r24
	lds r24,buttons_long
	lds r20,buttons_long
	ldi r18,lo8(1)
	ldi r19,hi8(1)
	mov r0,r21
	rjmp 2f
1:	lsl r18
	rol r19
2:	dec r0
	brpl 1b
	com r18
	and r18,r20
	sts buttons_long,r18
	in r18,89-32
	ori r18,lo8(2)
	out 89-32,r18
	ldi r25,lo8(0)
	rjmp 2f
1:	asr r25
	ror r24
2:	dec r21
	brpl 1b
	andi r24,lo8(1)
/* epilogue start */
	ret
	.size	Buttons_GetLong, .-Buttons_GetLong
.global	__vector_6
	.type	__vector_6, @function
__vector_6:
	sei
	push __zero_reg__
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Interrupt */
/* frame size = 0 */
	rcall Rotary_Read
	rcall Buttons_Read
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop __zero_reg__
	reti
	.size	__vector_6, .-__vector_6
.global	__vector_2
	.type	__vector_2, @function
__vector_2:
	push __zero_reg__
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
	lds r24,bit_count
	cpi r24,lo8(8)
	brlo .L37
	lds r24,bit_count
	cpi r24,lo8(16)
	brlo .L38
	lds r24,bit_count
	cpi r24,lo8(24)
	brsh .L33
	in r24,89-32
	andi r24,lo8(-3)
	out 89-32,r24
	lds r18,enc_delta
	sts enc_delta,__zero_reg__
	in r24,89-32
	ori r24,lo8(2)
	out 89-32,r24
	lds r24,bit_count
	ldi r19,lo8(0)
	ldi r25,lo8(0)
	sbiw r24,16
	rjmp 2f
1:	asr r19
	ror r18
2:	dec r24
	brpl 1b
	sbrc r18,0
	rjmp .L36
.L34:
	cbi 50-32,4
.L35:
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop __zero_reg__
	reti
.L38:
	lds r24,bit_count
	subi r24,lo8(-(-8))
	rcall Buttons_GetLong
	tst r24
	breq .L34
.L36:
	sbi 50-32,4
	rjmp .L35
.L37:
	lds r24,bit_count
	rcall Buttons_Get
	tst r24
	breq .L34
	sbi 50-32,4
	rjmp .L35
.L33:
	sts bit_count,__zero_reg__
	rjmp .L35
	.size	__vector_2, .-__vector_2
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	in r24,85-32
	ori r24,lo8(12)
	out 85-32,r24
	in r24,91-32
	ori r24,lo8(-128)
	out 91-32,r24
	ldi r24,lo8(16)
	out 49-32,r24
	ldi r24,lo8(-25)
	out 50-32,r24
	out 55-32,__zero_reg__
	ldi r24,lo8(-1)
	out 56-32,r24
	in r24,83-32
	ori r24,lo8(4)
	out 83-32,r24
	in r24,89-32
	ori r24,lo8(2)
	out 89-32,r24
/* #APP */
 ;  141 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L40:
	rjmp .L40
	.size	main, .-main
.global	buttons
.global	buttons
	.section .bss
	.type	buttons, @object
	.size	buttons, 1
buttons:
	.skip 1,0
.global	buttons_long
.global	buttons_long
	.type	buttons_long, @object
	.size	buttons_long, 1
buttons_long:
	.skip 1,0
.global	button_must_release
.global	button_must_release
	.type	button_must_release, @object
	.size	button_must_release, 1
button_must_release:
	.skip 1,0
.global	table
	.section	.progmem.data,"a",@progbits
	.type	table, @object
	.size	table, 16
table:
	.byte	0
	.byte	0
	.byte	-1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	1
	.byte	1
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	-1
	.byte	0
	.byte	0
.global	bit_count
.global	bit_count
	.section .bss
	.type	bit_count, @object
	.size	bit_count, 1
bit_count:
	.skip 1,0
	.lcomm last.1360,1
	.comm enc_delta,1,1
	.comm button_time,8,1
.global __do_clear_bss
