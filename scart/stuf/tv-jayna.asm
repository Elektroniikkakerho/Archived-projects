; Toimiva tekstillinen versio

LIST P=16F84A, F=INHX8M
__CONFIG _CP_OFF & _WDT_ON & _HS_OSC & _PWRTE_ON

include <p16f84a.inc>

dataindex		EQU		0x0C
delaycounter		EQU		0x0D
synccounter		EQU		0x0E
videostuff		EQU		0x0F
linecount		EQU		0x10
sleepcounter		EQU		0x11
seccounter		EQU		0x12
mincounter		EQU		0x13
framecounter		EQU		0x14

data1			EQU		0x15
data2			EQU		0x16
data3			EQU		0x17
data4			EQU		0x18
data5			EQU		0x19
data6			EQU		0x1A
data7			EQU		0x1B
data8			EQU		0x1C
data9			EQU		0x1D
dataA			EQU		0x1E
dataB			EQU		0x1F

mins_off		EQU		1
sec_init		EQU		40
fps			EQU		50
show_secs		EQU		8

enable_port		EQU		PORTA
enable_pin		EQU		1

ORG 0x000
	goto	start


; Hypp‰‰ akun osoittamaan kohtaan ja palaa sinne mist‰ t‰t‰ kutsuttiin
ReadROM:
	movwf	PCL

Text1Line1		DT			Brackets_1, K_1, II_1, N_1, N_1, O_1, S_1, T_1, A_1, A_1
Text1Line2		DT			Brackets_2, K_2, II_1, N_2, N_2, O_2, S_2, T_2, A_2, A_2
Text1Line3		DT			Brackets_2, K_3, II_1, N_3, N_3, O_2, S_3, T_2, A_3, A_3
Text1Line4		DT			Brackets_2, K_4, II_1, N_4, N_4, O_2, S_4, T_2, A_4, A_4
Text1Line5		DT			Brackets_2, K_4, II_1, N_5, N_5, O_2, S_5, T_2, A_5, A_5
Text1Line6		DT			Brackets_2, K_3, II_1, N_6, N_6, O_2, S_6, T_2, A_6, A_6
Text1Line7		DT			Brackets_2, K_2, II_1, N_7, N_7, O_2, S_2, T_2, A_7, A_7
Text1Line8		DT			Brackets_1, K_1, II_1, N_8, N_8, O_1, S_1, T_2, A_8, A_8

Text2Line1		DT			Brackets_1, E_1, I_1, K_1, II_1, N_1, N_1, O_1, S_1, T_1, A_1
Text2Line2		DT			Brackets_2, E_2, I_1, K_2, II_1, N_2, N_2, O_2, S_2, T_2, A_2
Text2Line3		DT			Brackets_2, E_3, I_1, K_3, II_1, N_3, N_3, O_2, S_3, T_2, A_3
Text2Line4		DT			Brackets_2, E_4, I_1, K_4, II_1, N_4, N_4, O_2, S_4, T_2, A_4
Text2Line5		DT			Brackets_2, E_5, I_1, K_4, II_1, N_5, N_5, O_2, S_5, T_2, A_5
Text2Line6		DT			Brackets_2, E_6, I_1, K_3, II_1, N_6, N_6, O_2, S_6, T_2, A_6
Text2Line7		DT			Brackets_2, E_7, I_1, K_2, II_1, N_7, N_7, O_2, S_2, T_2, A_7
Text2Line8		DT			Brackets_1, E_8, I_1, K_1, II_1, N_8, N_8, O_1, S_1, T_2, A_8

DummyLine		DT			0, 0, 0, 0, 0, 0, 0, 0, 0


dnop		macro
	nop
	nop
endm

tnop		macro	
	nop
	nop
	nop
endm

setbank0	macro
	bcf	STATUS, 5
endm

setbank1	macro
	bsf	STATUS, 5
endm

start:
	; Kaikki IO-pinnit outputeiksi
	setbank1
	clrw
	movwf	TRISA
	movwf	TRISB
	movlw	b'11111111'
	movwf	OPTION_REG					; Prescaler 1:128 => WDT
	setbank0

	clrf	PORTA
	clrf	PORTB
	clrf	videostuff
	movlw	sec_init
	movwf	seccounter
	movlw	mins_off
	movwf	mincounter

sleeploop:
	bcf	enable_port, enable_pin
	bcf	PORTA, 0
	sleep
	decfsz	seccounter
	goto	sleeploop
	
	decfsz	mincounter
	goto	seczero
	goto	startshow	
seczero:
	movlw	sec_init
	movwf	seccounter
	goto	sleeploop

startshow:
	movlw	fps
	movwf	framecounter
	movlw	show_secs
	movwf	seccounter
	
	bsf	enable_port, enable_pin

mainloop:
	call	VertSync

	movlw	d'17'					; 17 piiloon j‰‰v‰‰ juovaa 
	movwf	linecount
	nop
blankloop:
	call	BlackLine
	decfsz	linecount
	goto	blankloop
	nop
	
	call	BlackLine				; 91 mustaa juovaa
	movlw	d'98'
	movwf	linecount
	nop
emptyloop1:
	call	BlackLine
	decfsz	linecount
	goto	emptyloop1
	clrwdt
	call	BlackLine

	nop
	movlw	Text1Line1				; 8 tekstijuovaa
	movwf	dataindex
	call	PrepareString1
	call	DrawString1Line
	call	DrawString1Line
	call	DrawString1Line
	call	DrawString1Line
	call	DrawString1Line
	call	DrawString1Line
	call	DrawString1Line
	call	DrawString1Line

	call	BlackLine				; 90 mustaa juovaa
	movlw	d'67'
	movwf	linecount
	clrwdt
emptyloop2:
	call	BlackLine
	decfsz	linecount
	goto	emptyloop2
	nop
	call	BlackLine

	nop
	movlw	Text2Line1				; 8 tekstijuovaa
	movwf	dataindex
	call	PrepareString2
	call	DrawString2Line
	call	DrawString2Line
	call	DrawString2Line
	call	DrawString2Line
	call	DrawString2Line
	call	DrawString2Line
	call	DrawString2Line
	call	DrawString2Line

	call	BlackLine				; 102 mustaa juovaa
	movlw	d'96'
	movwf	linecount
	nop
emptyloop3:
	call	BlackLine
	decfsz	linecount
	goto	emptyloop3
	clrwdt
	call	BlackLine

	decfsz	framecounter
	goto	noframeoverflow
	nop
	call	BlackLine
	
	decfsz	seccounter
	goto	nosecoverflow
	nop
	call	BlackLine

	movlw	sec_init
	movwf	seccounter
	nop
	call	BlackLine

	movlw	mins_off
	movwf	mincounter
	nop
	call	BlackLine

	nop
	goto	sleeploop

noframeoverflow:
	call	BlackLine
	tnop
	call	BlackLine
	tnop
	call	BlackLine
	tnop
	call	BlackLine
	nop
	goto	mainloop

nosecoverflow:
	call	BlackLine
	tnop	
	call	BlackLine
	movlw	fps
	movwf	framecounter
	nop
	call	BlackLine
	nop
	goto	mainloop

; Odottaa kutsuineen (delaycounter + 1) * 3 syklin ajan eli (delaycounter + 1) us
delay:
	decfsz	delaycounter
	goto	delay
	return


; Generoi mustan viivan, kesto kutsuineen 52 us
BlackLine:
	; Aluksi 4 us synkkaustasolla
	bcf 	PORTB, 7			; set level to synclevel (bit 0)
	bcf 	PORTA, 0			; set level to synclevel (bit 1)	
	tnop
	movlw	1
	movwf	delaycounter
	call	delay				; 13
	
	; Sitten 8 us mustaa (j‰‰ lopuksi mustaksi)
	bsf	PORTA, 0
	movlw	d'55'
	movwf	delaycounter
	call	delay				; 24 (37)
	nop

	return

PrepareString1:
	; Aluksi 4 us synkkaustasolla
	bcf 	PORTB, 7			; set level to synclevel (bit 0)
	bcf 	PORTA, 0			; set level to synclevel (bit 1)	
	tnop
	movlw	1
	movwf	delaycounter
	call	delay				; 13
	
	; Sitten 8 us mustaa (j‰‰ lopuksi mustaksi)
	bsf	PORTA, 0
	movlw	d'32'
	movwf	delaycounter
	call	delay				; 24 (37)
	nop

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data1

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data2

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data3

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data4

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data5

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data6

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data7

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data8

	return

; Piirt‰‰ yhden pisterivin, jonka osoite on akussa
DrawString1Line:
	; Aluksi 4 us synkkaustasolla
	bcf 	PORTB, 7			; set level to synclevel (bit 0)
	bcf 	PORTA, 0			; set level to synclevel (bit 1)	

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data9
	dnop						; 14
	
	; Sitten 8 us mustaa (j‰‰ lopuksi mustaksi)
	bsf	PORTA, 0
	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	dataA

	movlw	2
	movwf	delaycounter
	call	delay				; 17 (31)
	; T‰st‰ alkaa varsinainen tekstin piirto
	tnop
	tnop
	dnop					; 22 (190)

	movfw	data1			; 10 (41)	
	movwf 	PORTB			; []
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	clrf	PORTB
	
	tnop					; 3 (44)
	nop

	movfw	data2			; 7 (51)	
	movwf 	PORTB			; K
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data3			; 3 (54)
	movwf 	PORTB			; I I
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	

	movfw	data4			; 7 (64)
	movwf 	PORTB			; N
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data5			; 7 (71)
	movwf 	PORTB			; N
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data6			; 6 (77)
	movwf 	PORTB			; O
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data7			; 6 (83)
	movwf 	PORTB			; S
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data8			; 7 (90)
	movwf 	PORTB			; T
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data9			; 7 (97)
	movwf 	PORTB			; A
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	dataA			; 7 (104)
	movwf 	PORTB			; A
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data1

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data2

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data3

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data4

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data5

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data6

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data7

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data8			; 64 (168)

	return				; 2 (192)


PrepareString2:
	; Aluksi 4 us synkkaustasolla
	bcf 	PORTB, 7			; set level to synclevel (bit 0)
	bcf 	PORTA, 0			; set level to synclevel (bit 1)	
	tnop
	movlw	1
	movwf	delaycounter
	call	delay				; 13
	
	; Sitten 8 us mustaa (j‰‰ lopuksi mustaksi)
	bsf		PORTA, 0
	movlw	d'35'
	movwf	delaycounter
	call	delay				; 24 (37)
	nop

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data1

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data2

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data3

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data4

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data5

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data6

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data7

	return


; Piirt‰‰ yhden pisterivin, jonka osoite on akussa
DrawString2Line:
	; Aluksi 4 us synkkaustasolla
	bcf 	PORTB, 7			; set level to synclevel (bit 0)
	bcf 	PORTA, 0			; set level to synclevel (bit 1)	

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data8
	nop

	movfw	dataindex			; 15
	
	; Sitten 8 us mustaa (j‰‰ lopuksi mustaksi)
	bsf	PORTA, 0

	incf	dataindex
	call	ReadROM
	movwf	data9

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	dataA
	
	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	dataB				; 27 (42)

	; T‰st‰ alkaa varsinainen tekstin piirto
	dnop					; 2 (44)

	movfw	data1			; 10 (54)
	movwf 	PORTB			; [
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	clrf	PORTB

	tnop					
	nop						; 4 (58)

	movfw	data2			; 6 (64)
	movwf 	PORTB			; E
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data3			; 3 (67)
	movwf 	PORTB			; I
	rlf 	PORTB, f		; second bit is shifted out (1)	

	tnop					
	nop						; 4 (71)

	movfw	data4			; 7 (78)
	movwf 	PORTB			; K
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data5			; 6 (84)
	movwf 	PORTB			; I I
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)	

	movfw	data6			; 7 (91)
	movwf 	PORTB			; N
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data7			; 7 (98)
	movwf 	PORTB			; N
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data8			; 6 (104)
	movwf 	PORTB			; O
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	data9			; 6 (110)
	movwf 	PORTB			; S
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	dataA			; 7 (117)
	movwf 	PORTB			; T
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	dataB			; 7 (124)
	movwf 	PORTB			; A
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)	
	rlf 	PORTB, f		; second bit is shifted out (1)
	rlf 	PORTB, f		; second bit is shifted out (1)

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data1

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data2

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data3

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data4

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data5

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data6

	movfw	dataindex
	incf	dataindex
	call	ReadROM
	movwf	data7			; 56 (180)

	tnop

 	return				; 2 (192)


VertSync:
	bcf	PORTB, 7
	bcf	PORTA, 0
	btfss	videostuff, 0
	goto	passbyextra1
	tnop
	tnop

	nop
passbyextra1:
	movlw	0x17
	movwf	delaycounter			
	bsf	PORTA, 0		; 14

	movlw	4
	btfsc	videostuff, 0
	movlw	5
	movwf	synccounter
	dnop

	call	delay
	
	btfsc	videostuff, 0
	goto	passbyxdelay1
	nop
	tnop
	tnop
passbyxdelay1:				; 14 + 3 + (16 + 8 + 1) * 3 = 17 + 75 = 92
	nop
shortsync1:
	bcf	PORTB, 7
	bcf	PORTA, 0
	tnop
	movlw	0x1B
	movwf	delaycounter
	bsf	PORTA, 0
	call	delay
	nop
	decfsz	synccounter
	goto	shortsync1
	nop
longsync:
	bcf	PORTB, 7
	bcf	PORTA, 0
	nop
	movlw	0x1B
	movwf	delaycounter
	call	delay
	bsf	PORTA, 0
	movlw	4
	movwf	synccounter
	tnop				; T‰h‰n tarvitaan 4 x nop, mutta koska muualla ei ole paikkaa
;	incf	videostuff		; videostuff:n kasvattamiselle, niin se tehd‰‰n n‰in
	nop
longsync_1:
	bcf	PORTB, 7
	bcf	PORTA, 0
	nop
	movlw	0x1B
	movwf	delaycounter
	call	delay
	bsf	PORTA, 0
	tnop
	decfsz	synccounter
	goto	longsync_1
	nop
shortsync:
	bcf	PORTB, 7
	bcf	PORTA, 0
	movlw	3
	btfss	videostuff, 0
	movlw	4
	movwf	synccounter
	movlw	0x1C
	movwf	delaycounter
	bsf	PORTA, 0
	call	delay
shortsync2:
	bcf	PORTB, 7
	bcf	PORTA, 0
	tnop
	movlw	0x1B
	movwf	delaycounter
	bsf	PORTA, 0
	call	delay
	nop
	decfsz	synccounter
	goto	shortsync2
extrasync:
	nop
	bcf	PORTB, 7
	bcf	PORTA, 0
	btfsc	videostuff, 0
	goto	passbyextra2
	tnop
	tnop
	nop
passbyextra2:
	movlw	0x37
	movwf	delaycounter			
	bsf	PORTA, 0		; 9
	call	delay			; 168 (177)
	
	btfss	videostuff, 0
	goto	passbyxdelay2
	nop
	tnop
	tnop
passbyxdelay2:
;	dnop
;	incf	videostuff
	
	return				; T‰m‰ funktio tarkalleen vaaditun mittainen eli 
					; seuraavan k‰skyn on oltava juovafunktion kutsu


LBracket_1	EQU			b'11000000'		; 1 + 6* 2 + 1
LBracket_2	EQU			b'10000000'

RBracket_1	EQU			b'11000000'		; 1 + 6* 2 + 1
RBracket_2	EQU			b'01000000'

Brackets_1	EQU			b'11000011'		; 1 + 6* 2 + 1
Brackets_2	EQU			b'10000001'

A_1		EQU			b'00100000'		; 2* 1 + 2 .. 5 + 2* 6
A_2		EQU			b'01010000'
A_3		EQU			b'10001000'
A_4		EQU			b'10001000'
A_5		EQU			b'11111000'
A_6		EQU			b'10001000'
A_7		EQU			b'10001000'
A_8		EQU			b'10001000

E_1		EQU			b'11110000'
E_2		EQU			b'10000000'
E_3		EQU			b'10000000'
E_4		EQU			b'10000000'
E_5		EQU			b'11100000'
E_6		EQU			b'10000000'
E_7		EQU			b'10000000'
E_8		EQU			b'11110000'

I_1		EQU			b'10000000'		; 8* 1

II_1		EQU			b'10010000'		; 8* 1

K_1		EQU			b'10001000'		; 1 .. 4 + 4 .. 1
K_2		EQU			b'10010000'
K_3		EQU			b'10100000'
K_4		EQU			b'11000000'

N_1		EQU			b'10001000'		; 1 .. 8
N_2		EQU			b'11001000'
N_3		EQU			b'11001000'
N_4		EQU			b'10101000'
N_5		EQU			b'10101000'
N_6		EQU			b'10011000'
N_7		EQU			b'10011000'
N_8		EQU			b'10001000'

O_1		EQU			b'01100000'		; 1 + 6* 2 + 1
O_2		EQU			b'10010000'

S_1		EQU			b'01100000'		; 1 .. 6 + 2 + 1
S_2		EQU			b'10010000'
S_3		EQU			b'10000000'
S_4		EQU			b'01000000'
S_5		EQU			b'00100000'
S_6		EQU			b'00010000'

T_1		EQU			b'11111000'		; 1 + 7* 2
T_2		EQU			b'00100000'

end



