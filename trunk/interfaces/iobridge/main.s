	.file	"main.c"
__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	LCDSetData
	.type	LCDSetData, @function
LCDSetData:
/* prologue: function */
/* frame size = 0 */
	sbrs r24,0
	rjmp .L2
	sbi 50-32,0
	sbrs r24,1
	rjmp .L4
.L13:
	sbi 50-32,1
	sbrs r24,2
	rjmp .L6
.L14:
	sbi 50-32,2
	sbrc r24,3
	rjmp .L12
.L8:
	cbi 50-32,3
	ret
.L2:
	cbi 50-32,0
	sbrc r24,1
	rjmp .L13
.L4:
	cbi 50-32,1
	sbrc r24,2
	rjmp .L14
.L6:
	cbi 50-32,2
	sbrs r24,3
	rjmp .L8
.L12:
	sbi 50-32,3
	ret
	.size	LCDSetData, .-LCDSetData
.global	LCDClk
	.type	LCDClk, @function
LCDClk:
/* prologue: function */
/* frame size = 0 */
	sbi 50-32,5
	ldi r24,lo8(2000)
	ldi r25,hi8(2000)
	movw r30,r24
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	cbi 50-32,5
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	ret
	.size	LCDClk, .-LCDClk
.global	Rotary_Read
	.type	Rotary_Read, @function
Rotary_Read:
/* prologue: function */
/* frame size = 0 */
	lds r24,last.1542
	lsl r24
	lsl r24
	andi r24,lo8(12)
	sts last.1542,r24
	sbis 54-32,0
	rjmp .L18
	ori r24,lo8(2)
	sts last.1542,r24
.L18:
	sbis 54-32,1
	rjmp .L19
	ori r24,lo8(1)
	sts last.1542,r24
.L19:
	mov r30,r24
	clr r31
	sbrc r30,7
	com r31
	subi r30,lo8(-(table))
	sbci r31,hi8(-(table))
/* #APP */
 ;  54 "main.c" 1
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
/* #APP */
 ;  63 "main.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r24,enc_delta
	sts enc_delta,__zero_reg__
/* #APP */
 ;  66 "main.c" 1
	sei
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	ret
	.size	Rotary_Get, .-Rotary_Get
.global	Buttons_Read
	.type	Buttons_Read, @function
Buttons_Read:
/* prologue: function */
/* frame size = 0 */
	sts key_timeout,__zero_reg__
	ldi r20,lo8(1)
	ldi r18,lo8(0)
	ldi r19,hi8(0)
	rjmp .L28
.L34:
	lds r24,button_must_release
	and r24,r20
	breq .L26
	lds r24,buttons
	or r24,r20
	sts buttons,r24
	lds r25,button_must_release
	mov r24,r20
	com r24
	and r24,r25
	sts button_must_release,r24
	movw r30,r18
	subi r30,lo8(-(button_time))
	sbci r31,hi8(-(button_time))
	st Z,__zero_reg__
.L26:
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	cpi r18,8
	cpc r19,__zero_reg__
	breq .L31
.L35:
	lsl r20
.L28:
	in r24,54-32
	ldi r25,lo8(0)
	movw r30,r18
	subi r30,lo8(-(button_idx))
	sbci r31,hi8(-(button_idx))
	ld r0,Z
	rjmp 2f
1:	asr r25
	ror r24
2:	dec r0
	brpl 1b
	sbrc r24,0
	rjmp .L34
	lds r24,button_must_release
	or r24,r20
	sts button_must_release,r24
	movw r30,r18
	subi r30,lo8(-(button_time))
	sbci r31,hi8(-(button_time))
	ld r24,Z
	cpi r24,lo8(80)
	brlo .L33
	ld r24,Z
	cpi r24,lo8(80)
	brne .L26
	lds r24,buttons_long
	or r24,r20
	sts buttons_long,r24
.L33:
	ld r24,Z
	subi r24,lo8(-(1))
	st Z,r24
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))
	cpi r18,8
	cpc r19,__zero_reg__
	brne .L35
.L31:
	ret
	.size	Buttons_Read, .-Buttons_Read
.global	Buttons_Get
	.type	Buttons_Get, @function
Buttons_Get:
/* prologue: function */
/* frame size = 0 */
	mov r21,r24
/* #APP */
 ;  110 "main.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
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
/* #APP */
 ;  113 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
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
/* #APP */
 ;  118 "main.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
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
/* #APP */
 ;  121 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
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
	lds r24,key_timeout
	tst r24
	breq .L44
	lds r24,key_timeout
	subi r24,lo8(-(-1))
	sts key_timeout,r24
.L43:
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
.L44:
	rcall Buttons_Read
	rjmp .L43
	.size	__vector_6, .-__vector_6
.global	LCDCmd8
	.type	LCDCmd8, @function
LCDCmd8:
/* prologue: function */
/* frame size = 0 */
	cbi 50-32,4
	rcall LCDSetData
	sbi 50-32,5
	ldi r24,lo8(2000)
	ldi r25,hi8(2000)
	movw r30,r24
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r30,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	cbi 50-32,5
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	ret
	.size	LCDCmd8, .-LCDCmd8
.global	LCDCmd4
	.type	LCDCmd4, @function
LCDCmd4:
	push r15
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
	mov r15,r22
	cbi 50-32,4
	rcall LCDSetData
	sbi 50-32,5
	ldi r16,lo8(2000)
	ldi r17,hi8(2000)
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	cbi 50-32,5
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	mov r24,r15
	rcall LCDSetData
	sbi 50-32,5
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	cbi 50-32,5
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	pop r17
	pop r16
	pop r15
	ret
	.size	LCDCmd4, .-LCDCmd4
.global	LCDInit
	.type	LCDInit, @function
LCDInit:
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
	sbi 49-32,4
	sbi 49-32,5
	in r24,49-32
	ori r24,lo8(15)
	out 49-32,r24
	ldi r24,lo8(-25536)
	ldi r25,hi8(-25536)
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(48)
	rcall LCDCmd8
	ldi r16,lo8(10000)
	ldi r17,hi8(10000)
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(48)
	rcall LCDCmd8
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(2)
	rcall LCDCmd8
	ldi r24,lo8(2)
	ldi r22,lo8(8)
	rcall LCDCmd4
	ldi r24,lo8(0)
	ldi r22,lo8(12)
	rcall LCDCmd4
	ldi r24,lo8(0)
	ldi r22,lo8(6)
	rcall LCDCmd4
	ldi r24,lo8(0)
	ldi r22,lo8(1)
	rcall LCDCmd4
/* epilogue start */
	pop r17
	pop r16
	ret
	.size	LCDInit, .-LCDInit
.global	LCDCmd
	.type	LCDCmd, @function
LCDCmd:
/* prologue: function */
/* frame size = 0 */
	mov r22,r24
	swap r24
	andi r24,lo8(15)
	andi r22,lo8(15)
	rcall LCDCmd4
/* epilogue start */
	ret
	.size	LCDCmd, .-LCDCmd
.global	LCDClear
	.type	LCDClear, @function
LCDClear:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(0)
	ldi r22,lo8(1)
	rcall LCDCmd4
/* epilogue start */
	ret
	.size	LCDClear, .-LCDClear
.global	LCDPos
	.type	LCDPos, @function
LCDPos:
/* prologue: function */
/* frame size = 0 */
	mov r25,r22
	ori r25,lo8(-128)
	mov r22,r24
	swap r22
	lsl r22
	lsl r22
	andi r22,lo8(-64)
	or r22,r25
	mov r24,r22
	swap r24
	andi r24,lo8(15)
	andi r22,lo8(15)
	rcall LCDCmd4
/* epilogue start */
	ret
	.size	LCDPos, .-LCDPos
.global	LCDData
	.type	LCDData, @function
LCDData:
	push r15
	push r16
	push r17
/* prologue: function */
/* frame size = 0 */
	mov r15,r24
	mov r24,r22
	sbi 50-32,4
	rcall LCDSetData
	sbi 50-32,5
	ldi r16,lo8(2000)
	ldi r17,hi8(2000)
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	cbi 50-32,5
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	mov r24,r15
	rcall LCDSetData
	sbi 50-32,5
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* #NOAPP */
	cbi 50-32,5
	movw r24,r16
/* #APP */
 ;  105 "/usr/lib/gcc/avr/4.3.5/../../../avr/include/util/delay_basic.h" 1
	1: sbiw r24,1
	brne 1b
 ;  0 "" 2
/* epilogue start */
/* #NOAPP */
	pop r17
	pop r16
	pop r15
	ret
	.size	LCDData, .-LCDData
.global	LCDChar
	.type	LCDChar, @function
LCDChar:
/* prologue: function */
/* frame size = 0 */
	mov r22,r24
	andi r24,lo8(15)
	swap r22
	andi r22,lo8(15)
	rcall LCDData
/* epilogue start */
	ret
	.size	LCDChar, .-LCDChar
.global	LCDInt
	.type	LCDInt, @function
LCDInt:
	push r12
	push r13
	push r15
	push r16
	push r17
	push r29
	push r28
	rcall .
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 5 */
	movw r16,r24
	sbrc r25,7
	rjmp .L71
	sbiw r24,0
	breq .L72
.L63:
	ldi r24,lo8(4)
	mov r15,r24
	movw r12,r28
	sec
	adc r12,__zero_reg__
	adc r13,__zero_reg__
.L65:
	movw r30,r12
	add r30,r15
	adc r31,__zero_reg__
	movw r24,r16
	ldi r22,lo8(10)
	ldi r23,hi8(10)
	rcall __divmodhi4
	st Z,r24
	movw r24,r16
	ldi r22,lo8(10)
	ldi r23,hi8(10)
	rcall __divmodhi4
	movw r16,r22
	dec r15
	cp __zero_reg__,r22
	cpc __zero_reg__,r23
	brlt .L65
.L68:
	inc r15
	movw r30,r12
	add r30,r15
	adc r31,__zero_reg__
	ld r24,Z
	subi r24,lo8(-(48))
	rcall LCDChar
	ldi r24,lo8(3)
	cp r24,r15
	brsh .L68
.L67:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	pop r16
	pop r15
	pop r13
	pop r12
	ret
.L72:
	ldi r24,lo8(48)
	rcall LCDChar
	rjmp .L67
.L71:
	ldi r24,lo8(45)
	rcall LCDChar
	com r17
	neg r16
	sbci r17,lo8(-1)
	rjmp .L63
	.size	LCDInt, .-LCDInt
.global	LCDUInt
	.type	LCDUInt, @function
LCDUInt:
	push r14
	push r15
	push r17
	push r29
	push r28
	rcall .
	rcall .
	push __tmp_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 5 */
	movw r18,r24
	sbiw r24,0
	breq .L82
	ldi r17,lo8(4)
	movw r14,r28
	sec
	adc r14,__zero_reg__
	adc r15,__zero_reg__
.L76:
	movw r30,r14
	add r30,r17
	adc r31,__zero_reg__
	movw r24,r18
	ldi r22,lo8(10)
	ldi r23,hi8(10)
	rcall __udivmodhi4
	st Z,r24
	movw r24,r18
	ldi r22,lo8(10)
	ldi r23,hi8(10)
	rcall __udivmodhi4
	movw r18,r22
	subi r17,lo8(-(-1))
	cp r22,__zero_reg__
	cpc r23,__zero_reg__
	brne .L76
.L79:
	subi r17,lo8(-(1))
	movw r30,r14
	add r30,r17
	adc r31,__zero_reg__
	ld r24,Z
	subi r24,lo8(-(48))
	rcall LCDChar
	cpi r17,lo8(4)
	brlo .L79
.L78:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r28
	pop r29
	pop r17
	pop r15
	pop r14
	ret
.L82:
	ldi r24,lo8(48)
	rcall LCDChar
	rjmp .L78
	.size	LCDUInt, .-LCDUInt
.global	LCDLoadChar
	.type	LCDLoadChar, @function
LCDLoadChar:
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
	mov r16,r22
	mov r15,r23
	mov r17,r20
	lsl r24
	lsl r24
	lsl r24
	ori r24,lo8(64)
	rcall LCDCmd
	tst r17
	breq .L84
	mov r24,r16
	mov r25,r15
	movw r18,r24
	movw r28,r18
.L85:
	ld r24,Y+
	rcall LCDChar
	movw r18,r28
	sub r18,r16
	cp r18,r17
	brlo .L85
.L84:
	ldi r24,lo8(-128)
	rcall LCDCmd
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	ret
	.size	LCDLoadChar, .-LCDLoadChar
.global	init_graph
	.type	init_graph, @function
init_graph:
	push r29
	push r28
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 8 */
	movw r22,r28
	subi r22,lo8(-(1))
	sbci r23,hi8(-(1))
	movw r26,r22
	ldi r30,lo8(C.29.1646)
	ldi r31,hi8(C.29.1646)
	ldi r24,lo8(8)
.L89:
	ld r0,Z+
	st X+,r0
	subi r24,lo8(-(-1))
	brne .L89
	ldi r24,lo8(1)
	ldi r20,lo8(8)
	rcall LCDLoadChar
/* epilogue start */
	adiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r28
	pop r29
	ret
	.size	init_graph, .-init_graph
.global	draw_graph
	.type	draw_graph, @function
draw_graph:
	push r17
/* prologue: function */
/* frame size = 0 */
	mov r17,r24
	lds r24,col
	lds r22,old_width.1652
	rcall LCDPos
	ldi r24,lo8(32)
	rcall LCDChar
	lds r24,col
	mov r22,r17
	rcall LCDPos
	ldi r24,lo8(35)
	rcall LCDChar
	sts old_width.1652,r17
/* epilogue start */
	pop r17
	ret
	.size	draw_graph, .-draw_graph
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
	ldi r24,lo8(-1)
	out 49-32,r24
	out 55-32,__zero_reg__
	out 56-32,r24
	in r24,83-32
	ori r24,lo8(4)
	out 83-32,r24
	in r24,89-32
	ori r24,lo8(2)
	out 89-32,r24
/* #APP */
 ;  177 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	rcall LCDInit
	rcall LCDClear
	rcall init_graph
	rjmp .L102
.L95:
	ldi r24,lo8(1)
	rcall Buttons_Get
	tst r24
	brne .L103
.L96:
	ldi r24,lo8(2)
	rcall Buttons_Get
	tst r24
	brne .L104
.L97:
	ldi r24,lo8(0)
	rcall Buttons_GetLong
	tst r24
	brne .L105
.L102:
/* #APP */
 ;  63 "main.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r24,enc_delta
	sts enc_delta,__zero_reg__
/* #APP */
 ;  66 "main.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(0)
	rcall Buttons_Get
	tst r24
	breq .L95
	ldi r24,lo8(49)
	rcall LCDChar
	ldi r24,lo8(1)
	rcall Buttons_Get
	tst r24
	breq .L96
.L103:
	ldi r24,lo8(50)
	rcall LCDChar
	ldi r24,lo8(2)
	rcall Buttons_Get
	tst r24
	breq .L97
.L104:
	ldi r24,lo8(51)
	rcall LCDChar
	ldi r24,lo8(0)
	rcall Buttons_GetLong
	tst r24
	breq .L102
.L105:
	rcall LCDClear
	ldi r24,lo8(62)
	rcall LCDChar
	rjmp .L102
	.size	main, .-main
.global	LCDPrint
	.type	LCDPrint, @function
LCDPrint:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
	movw r28,r24
	ld r22,Y
	tst r22
	breq .L109
.L110:
	mov r24,r22
	andi r24,lo8(15)
	swap r22
	andi r22,lo8(15)
	rcall LCDData
	adiw r28,1
	ld r22,Y
	tst r22
	brne .L110
.L109:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	LCDPrint, .-LCDPrint
.global	LCDPrint_P
	.type	LCDPrint_P, @function
LCDPrint_P:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
	movw r30,r24
	movw r28,r24
/* #APP */
 ;  122 "lcd.h" 1
	lpm r24, Z
	
 ;  0 "" 2
/* #NOAPP */
	cp __zero_reg__,r24
	brge .L115
.L116:
	mov r22,r24
	clr r23
	sbrc r22,7
	com r23
	andi r22,lo8(240)
	andi r23,hi8(240)
	asr r23
	ror r22
	asr r23
	ror r22
	asr r23
	ror r22
	asr r23
	ror r22
	andi r24,lo8(15)
	rcall LCDData
	adiw r28,1
	movw r30,r28
/* #APP */
 ;  122 "lcd.h" 1
	lpm r24, Z
	
 ;  0 "" 2
/* #NOAPP */
	cp __zero_reg__,r24
	brlt .L116
.L115:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	LCDPrint_P, .-LCDPrint_P
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
.global	key_timeout
.global	key_timeout
	.type	key_timeout, @object
	.size	key_timeout, 1
key_timeout:
	.skip 1,0
.global	button_idx
	.data
	.type	button_idx, @object
	.size	button_idx, 3
button_idx:
	.byte	2
	.byte	3
	.byte	4
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
.global	col
.global	col
	.section .bss
	.type	col, @object
	.size	col, 1
col:
	.skip 1,0
	.lcomm old_width.1652,1
	.data
	.type	C.29.1646, @object
	.size	C.29.1646, 8
C.29.1646:
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.byte	31
	.lcomm last.1542,1
	.comm enc_delta,1,1
	.comm button_time,8,1
.global __do_copy_data
.global __do_clear_bss
