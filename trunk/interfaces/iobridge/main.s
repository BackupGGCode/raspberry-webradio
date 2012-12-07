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
	sbis 48-32,6
	rjmp .L2
	ori r24,lo8(2)
	sts last.1360,r24
.L2:
	sbis 48-32,5
	rjmp .L3
	lds r24,last.1360
	ori r24,lo8(1)
	sts last.1360,r24
.L3:
	lds r30,last.1360
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
	ldi r30,lo8(89)
	ldi r31,hi8(89)
	ld r24,Z
	andi r24,lo8(-3)
	st Z,r24
	lds r24,enc_delta
	sts enc_delta,__zero_reg__
	ld r25,Z
	ori r25,lo8(2)
	st Z,r25
/* epilogue start */
	ret
	.size	Rotary_Get, .-Rotary_Get
.global	Buttons_Read
	.type	Buttons_Read, @function
Buttons_Read:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
	ldi r22,lo8(1)
	ldi r18,lo8(0)
	ldi r19,hi8(0)
	ldi r28,lo8(54)
	ldi r29,hi8(54)
	ldi r26,lo8(button_time)
	ldi r27,hi8(button_time)
.L14:
	ld r24,Y
	and r24,r22
	brne .L8
	lds r24,button_must_release
	or r24,r22
	sts button_must_release,r24
	movw r30,r26
	add r30,r18
	adc r31,r19
	ld r24,Z
	cpi r24,lo8(80)
	brsh .L9
	movw r30,r26
	add r30,r18
	adc r31,r19
	ld r24,Z
	subi r24,lo8(-(1))
	st Z,r24
	rjmp .L10
.L9:
	movw r30,r26
	add r30,r18
	adc r31,r19
	ld r24,Z
	cpi r24,lo8(80)
	brne .L10
	lds r24,buttons_long
	or r24,r22
	sts buttons_long,r24
	movw r30,r26
	add r30,r18
	adc r31,r19
	ld r24,Z
	subi r24,lo8(-(1))
	st Z,r24
	rjmp .L10
.L8:
	lds r24,button_must_release
	and r24,r22
	breq .L10
	movw r30,r26
	add r30,r18
	adc r31,r19
	ld r24,Z
	cpi r24,lo8(80)
	brsh .L11
	lds r24,buttons
	or r24,r22
	sts buttons,r24
	rjmp .L12
.L11:
	movw r30,r26
	add r30,r18
	adc r31,r19
	st Z,__zero_reg__
.L12:
	lds r25,button_must_release
	mov r24,r22
	com r24
	and r24,r25
	sts button_must_release,r24
.L10:
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	cpi r18,8
	cpc r19,__zero_reg__
	breq .L15
	lsl r22
	rjmp .L14
.L15:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	Buttons_Read, .-Buttons_Read
.global	Buttons_Get
	.type	Buttons_Get, @function
Buttons_Get:
/* prologue: function */
/* frame size = 0 */
	mov r21,r24
	ldi r30,lo8(89)
	ldi r31,hi8(89)
	ld r24,Z
	andi r24,lo8(-3)
	st Z,r24
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
	ld r18,Z
	ori r18,lo8(2)
	st Z,r18
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
	ldi r30,lo8(89)
	ldi r31,hi8(89)
	ld r24,Z
	andi r24,lo8(-3)
	st Z,r24
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
	ld r18,Z
	ori r18,lo8(2)
	st Z,r18
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
	lds r24,bit_count
	tst r24
	brne .L22
	lds r18,buttons
	lds r20,buttons_long
	lds r21,enc_delta
	mov r25,r20
	ldi r24,lo8(0)
	ldi r19,lo8(0)
	or r24,r18
	or r25,r19
	mov r19,r21
	clr r19
	ldi r18,lo8(0)
	or r24,r18
	or r25,r19
	clr r26
	sbrc r25,7
	com r26
	mov r27,r26
	sts current_val,r24
	sts (current_val)+1,r25
	sts (current_val)+2,r26
	sts (current_val)+3,r27
.L22:
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
	brsh .L24
	lds r24,bit_count
	tst r24
	brne .L25
	lds r24,buttons
	sts button_current,r24
	sts buttons,__zero_reg__
.L25:
	lds r24,button_current
	lds r18,bit_count
	ldi r25,lo8(0)
	rjmp 2f
1:	asr r25
	ror r24
2:	dec r18
	brpl 1b
	sbrs r24,0
	rjmp .L26
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ld r24,Z
	ori r24,lo8(16)
	st Z,r24
	rjmp .L27
.L26:
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ld r24,Z
	andi r24,lo8(-17)
	st Z,r24
	rjmp .L27
.L24:
	lds r24,bit_count
	cpi r24,lo8(16)
	brsh .L28
	lds r24,bit_count
	cpi r24,lo8(8)
	brne .L29
	lds r24,buttons_long
	sts button_l_current,r24
	sts buttons_long,__zero_reg__
.L29:
	lds r18,button_l_current
	lds r24,bit_count
	ldi r19,lo8(0)
	ldi r25,lo8(0)
	sbiw r24,8
	rjmp 2f
1:	asr r19
	ror r18
2:	dec r24
	brpl 1b
	sbrs r18,0
	rjmp .L30
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ld r24,Z
	ori r24,lo8(16)
	st Z,r24
	rjmp .L27
.L30:
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ld r24,Z
	andi r24,lo8(-17)
	st Z,r24
	rjmp .L27
.L28:
	lds r24,bit_count
	cpi r24,lo8(24)
	brsh .L27
	lds r24,bit_count
	cpi r24,lo8(16)
	brne .L31
	rcall Rotary_Get
	sts rotary_current,r24
.L31:
	lds r18,rotary_current
	lds r24,bit_count
	ldi r19,lo8(0)
	ldi r25,lo8(0)
	sbiw r24,16
	rjmp 2f
1:	asr r19
	ror r18
2:	dec r24
	brpl 1b
	sbrs r18,0
	rjmp .L32
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ld r24,Z
	ori r24,lo8(16)
	st Z,r24
	rjmp .L27
.L32:
	ldi r30,lo8(50)
	ldi r31,hi8(50)
	ld r24,Z
	andi r24,lo8(-17)
	st Z,r24
.L27:
	lds r24,bit_count
	subi r24,lo8(-(1))
	sts bit_count,r24
	lds r24,bit_count
	cpi r24,lo8(24)
	brne .L34
	sts bit_count,__zero_reg__
.L34:
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
	.size	__vector_2, .-__vector_2
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r30,lo8(85)
	ldi r31,hi8(85)
	ld r24,Z
	ori r24,lo8(12)
	st Z,r24
	ldi r30,lo8(91)
	ldi r31,hi8(91)
	ld r24,Z
	ori r24,lo8(-128)
	st Z,r24
	ldi r24,lo8(16)
	out 49-32,r24
	ldi r24,lo8(-25)
	out 50-32,r24
	out 55-32,__zero_reg__
	ldi r24,lo8(-1)
	out 56-32,r24
	ldi r30,lo8(83)
	ldi r31,hi8(83)
	ld r24,Z
	ori r24,lo8(4)
	st Z,r24
	ldi r30,lo8(89)
	ldi r31,hi8(89)
	ld r24,Z
	ori r24,lo8(2)
	st Z,r24
/* #APP */
 ;  160 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L36:
	rjmp .L36
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
.global	current_bit
	.data
	.type	current_bit, @object
	.size	current_bit, 4
current_bit:
	.byte	1
	.byte	0
	.byte	0
	.byte	0
.global	current_val
.global	current_val
	.section .bss
	.type	current_val, @object
	.size	current_val, 4
current_val:
	.skip 4,0
.global	button_current
.global	button_current
	.type	button_current, @object
	.size	button_current, 1
button_current:
	.skip 1,0
.global	button_l_current
.global	button_l_current
	.type	button_l_current, @object
	.size	button_l_current, 1
button_l_current:
	.skip 1,0
.global	rotary_current
.global	rotary_current
	.type	rotary_current, @object
	.size	rotary_current, 1
rotary_current:
	.skip 1,0
	.lcomm last.1360,1
	.comm enc_delta,1,1
	.comm button_time,8,1
.global __do_copy_data
.global __do_clear_bss
