;****************************************
;     SALAMA
; (C) Elektroniikkakerho
;     Ilari & Timps
;     1998
;****************************************

indf    equ     00h
tmr0    equ     01h
pcl     equ     02h
status  equ     03h
fsr     equ     04h
porta   equ     05h
portb   equ     06h
eedata  equ     08h		;eeprom data
eeadr   equ     09h		;eeprom osoite
pclatch equ     0ah
intcon  equ     0bh
opt_f   equ     081h
trisa   equ     085h
trisb   equ     086h
eecon1  equ     088h
eecon2  equ     089h
rp0     equ     5
rd      equ     0


col1    	equ     0ch             ;ryhma 1 (plane0)
col2    	equ     0dh             ;      2
col3    	equ     0eh             ;      3
col4    	equ     0fh             ;      4
count1  	equ     010h            ;viiveille
count2  	equ     011h
d_updt  	equ     012h            ;palava ryhma
mask    	equ     013h            ;'1110'
	                                ;'1101'
        	                        ;'1011'
                	                ;'0111'

status_temp     equ     017h   		;status-rekisteri taltteen keskeytyksessa
w_temp  	equ     018h            ;w-rekisteri taltteen ...
bcol1   	equ     019h
bcol2   	equ     01ah
bcol3   	equ     01bh
bcol4   	equ     01ch
contrast 	equ     01dh
count3  	equ     01eh
status_temp2	equ	01fh
lask1   	equ     020h		;laskuri primitiivi aliohjelmille
lask2   	equ     021h		;laskuri tavallisille aliohjelmille
temp    	equ     022h		;temp-rekisteri	aliohjelmille
rivi    	equ     023h		;eeprommin osoite
d_count 	equ	024h		;nayton vaihe
status_temp3	equ	025h
col5		equ	026h		;plane1
col6		equ	027h
col7		equ	028h
col8		equ	029h
bcol5		equ	02ah
bcol6		equ	02bh
bcol7		equ	02ch
bcol8		equ	02dh
d_temp		equ	02eh		;temp-rekisteri keskeytys rutiinille
d_temp1		equ	02fh		;- -
status_t1       equ     030h
status_t2       equ     031h
function	equ	032h		;hae aliohjelman toimintamoodi
temp2		equ	033h		;temp-rekisteri primitiivi aliohjelmille
nopeus          equ     034h
tauko           equ     035h
vieritys_moodi  equ     036h

        LIST P = 16F84

        org     0
        goto    initialize
        org     4

disp_handler:
        movwf   w_temp          ;talleta w-rekisteri
        swapf   status,w
        movwf   status_temp     ;status-rekisteri talteen
	incf    d_updt,f
	movlw   b'11'
        andwf   d_updt,f
        btfss   status,2        ;oliko ryhma 1 
        goto    skip_copy
	incf	d_count,f
	movlw	b'11111' 
        andwf	d_count,f
	movfw   col1            ;oli...
        movwf   bcol1
        movfw   col2
        movwf   bcol2
        movfw   col3
        movwf   bcol3
        movfw   col4
        movwf   bcol4
	movfw	col5
	movwf	bcol5
	movfw	col6
	movwf	bcol6
	movfw	col7
	movwf	bcol7
	movfw	col8
	movwf	bcol8
skip_copy:
        movlw   0fh             ;sammuta naytto
        movwf   porta
	movfw	contrast
	subwf	d_count,w
	btfsc	status,0
	goto	ohita 	
	movwf	d_temp
	comf	d_temp,f
	incf	d_temp,f
 	rrf	contrast,w
        movwf   d_temp1
        bcf     status,0
        rrf     d_temp1,f
        bcf     status,0
        rrf     d_temp1,w
 	subwf	d_temp,w
 	btfss	status,0
	goto	cols58 	
        movfw   d_updt
        addlw   bcol1           ;naytettavan ryhman osoite
	goto	ohita2
cols58:
	movfw   d_updt
        addlw   bcol5           ;naytettavan ryhman osoite
ohita2:
        movwf   fsr             ;epasuoranosoituksen osoiterekisteri
        movfw   indf            ;osoiterikisterin osoittaman muistipaikan sisalto
        movwf   portb
        movfw   d_updt
        addlw   mask
        movwf   fsr
        movfw   indf
        movwf   porta
ohita:
        bcf     intcon,2
        swapf   status_temp,w
        movwf   status
        swapf   w_temp,f
        swapf   w_temp,w
        retfie


viive:
        swapf   status,w
        movwf   status_temp2

        movfw   nopeus
        movwf   count2
odota2:
        movlw   0ffh
        movwf   count3
odota3:
        movfw   d_updt
        andlw   0f
        btfss   status,2
        goto    odota3
        decfsz  count3,f
        goto    odota3
        decfsz  count2,f
        goto    odota2
        swapf   status_temp2,w
        movwf   status
        return

pitka_viive:
        movwf   count2
        swapf   status,w
        movwf   status_temp2
        goto    odota2

vali_viive:
        movfw   tauko
        movwf   lask1
vali2:
        movlw   0ff
        call    pitka_viive
        decfsz  lask1,f
        goto    vali2
        return

initialize:						;************** init ****************
       
        clrf    porta
        clrf    portb
        bsf     status,rp0      ;valitaan pankki 1
        movlw   00h
        movwf   trisa           ;set direction for port a
        movlw   0h
        movwf   trisb           ;set direction for port b
        bcf     status,rp0      ;valitaan pankki 0

        movlw   b'1110'         ;tehdaan taulukko ryhman valinnoille
        movwf   mask
        movlw   b'1101'
        movwf   mask+1
        movlw   b'1011'
        movwf   mask+2
        movlw   b'0111'
        movwf   mask+3

        movlw   3h             
        movwf   d_updt    
        
        clrwdt                  ;prescaler assigned to tmr0
        bsf     status,rp0
        movlw   b'01000000'        
        movwf   opt_f
        bcf     status,rp0
	movlw	0
	movwf	d_count
        movlw   b'10100000'
        movwf   intcon          ;enable tmr0-interrupt

        
; ***********************************************************************************paa ohjelma
; pieni selostus aliohjelmista

; viive
; pitka_viive		w=miten pitk‰
; vilkutus		w=montako kertaa        
; taytto
; tyhjays		p‰‰lt‰->pois kiert‰en
; kierratys_v   
; kierratys_o
; pois
; paalle
; vieritys_taytto	scrolli j‰tt‰‰ palamaan ylh‰‰lt‰ alas
; kierto_o		pyˆritt‰‰ ledej‰ oikealle vaatii ledien sytytyksen
; kierto_v
; vieritys		scrolli ylh‰‰lt‰ alas
; rapytys		lask2=montako kertaa
;**************************************************************************************************
        movlw   10
        movwf   nopeus
        movlw   1
        movwf   tauko
main:
        call    pois
        call    pois_1

        call    vali_viive

       

        movlw   b'100000'
        movwf   contrast

        call    salama
        
        call    vali_viive

        call    salama

        call    vali_viive
                
        movlw   20
        movwf   nopeus

        movlw   10
        call    strobo

        movlw   10
        movwf   nopeus

        call    vali_viive

        call    pois_1
        call    pois

        movlw   0
        movwf   contrast

        movlw   b'10001111'
        movwf   col1
        movlw   b'10010001'
        movwf   col5

        movlw   8
        call    kirkasta

        movlw   9
        call    kierto_sama_suunta_01
        call    pois_1

        movlw   10
        call    tyhjays

        call    pois

        call    vali_viive
               
        movlw   10
        call    vieritys_taytto_1
        movlw   10
        call    vieritys_taytto_a_0
        movlw   10
        call    vieritys_tyhjays_0
        movlw   10
        call    vieritys_tyhjays_a_1
                
        call    vali_viive

        movlw   b'11110000'
        movwf   col1
        movwf   col5
        movlw   8
        call    kierto_01
        call    pois_1
        movlw   10
        call    tyhjays
         
        call    pois

        movlw   7
        movwf   nopeus

        movlw   1
        call    vieritys_0
        movlw   1
        call    vieritys_a_0
        movlw   1
        call    vieritys_0

        movlw   10
        movwf   nopeus

        call    pois
        movlw   10
        call    vieritys_taytto_1
        movlw   1
        call    vieritys_0
        movlw   1
        call    vieritys_a_0
        call    pois

        movlw   10
        call    vieritys_tyhjays_a_1
        call    pois_1

        call    vali_viive
        
        movlw   20
        call    vieritys_taytto_a_1

        movlw   1
        call    vieritys_a_0
        call    pois

        movlw   20
        call    vieritys_taytto_a_0

       	movlw	20
        call    vieritys_tyhjays_0

        movlw	20
        call    vieritys_tyhjays_1

        call    vali_viive

        movlw   8
        call    taytto_1

        movlw   8
        call    taytto

        movlw   5
        call    vilkutus2

        movlw   20
        call    tyhjays
        movlw   20
        call    tyhjays_1

        call    vali_viive

        call    salama

        call    vali_viive

        call    salama
        movlw   10
       
        call    vali_viive
        
        call    vieritys_taytto_0
        movlw   10
        call    taytto
        movlw   10
        call    rapytys

        movlw   5
        call    tyhjays
        movlw   5
        call    tyhjays_1

        call    vali_viive
        
        movlw   10
        call    vieritys_taytto_1
        movlw   b'11111110'
        movwf   col1
        movlw   3
        call    kierto_o
        movlw   2
        call    kierto_v
        movlw   3
        call    kierto_o
        movlw   10
        call    taytto
        movlw   3
        call    vilkutus
        movlw   10
        call    vieritys_tyhjays_0
        movlw   10
        call    tyhjays_1
       

        call    vali_viive

        movlw   20
        call    taytto_1
        movlw   6
        call    vieritys_0
        movlw   10
        call    vieritys_taytto_0

        movlw   10
        call    himmenna
        movlw   10
        movwf   nopeus

        movlw   80
        call    pitka_viive
        call    pois
        movlw   b'11111000'
        movwf   col1
        movlw   b'11001100'
        movwf   col5
        movwf   col6
        movwf   col7
        movwf   col8
        movlw   10
        call    kirkasta
        movlw   6
        call    kierto_01
        movlw   10
        call    taytto_1
        movlw   10
        call    taytto
        movlw   7
        call    vilkutus2
        movlw   10
        call    vieritys_tyhjays_0
        movlw   10
        call    tyhjays_1
                
        call    vali_viive

        call    salama

        call    vali_viive

        movlw   6
        movwf   nopeus

        call    vieritys_taytto_0
        movlw   6
        call    vieritys_tyhjays_0

        call    vali_viive

        movlw   30
        movwf   nopeus

        movlw   10
        call    strobo

        movlw   6
        movwf   nopeus

        movlw   0
        movwf   contrast
        call    paalle_1
        movlw   10
        call    kirkasta

        movlw   50
        call    pitka_viive

        movlw   1
        call    vieritys_0
        movlw   1
        call    vieritys_a_0
        movlw   1
        call    vieritys_0
        movlw   1
        call    vieritys_a_0
        movlw   b'11111110'
        movwf   col1
        movlw   2
        call    kierto_o
        movlw   2
        call    kierto_v
        call    pois
        movlw   10
        call    himmenna
        call    pois_1

        movlw   b'100000'
        movwf   contrast

        call    vali_viive

        call    salama

        
        call    pitka_viive


        goto    main



	
        
;*************************************************** pi‰ ohjelma loppu ********************************

vieritys_0:
        movwf   temp
        movlw   b'110010'               ;kirkas + skrolli + alas
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return

vieritys_taytto_0:        
        movwf   temp
        movlw   b'100110'                   ;kirkas + taytto + alas
        movwf   vieritys_moodi
        movfw   temp  
        call    vieritys
        return

vieritys_tyhjays_0:
        movwf   temp
        movlw   b'101010'               ;kirkas + tyhjays + alas
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return

vieritys_1:        
        movwf   temp
        movlw   b'1010010'              ;himmea + skrolli + alas
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return

vieritys_taytto_1:
        movwf   temp    
        movlw   b'1000110'               ;himmea + taytto + alas
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return

vieritys_tyhjays_1:
        movwf   temp
        movlw   b'1001010'
        movwf   vieritys_moodi          ;himmea + tyhjays + alas
        movfw   temp
        call    vieritys
        return

vieritys_a_1:
        movwf   temp
        movlw   b'1010001'              ;himmea + skrolli + ylos
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return
        
vieritys_taytto_a_1:
        movwf   temp
        movlw   b'1000101'              ;himmea + taytto + ylos
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return

vieritys_tyhjays_a_1:
        movwf   temp
        movlw   b'1001001'              ;himmea + tyhjays + ylos
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return

vieritys_a_0:
        movwf   temp
        movlw   b'110001'              ;kirkas + skrolli + ylos
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return
                
vieritys_taytto_a_0:
        movwf   temp
        movlw   b'100101'              ;kirkas + taytto + ylos
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return
        
vieritys_tyhjays_a_0:
        movwf   temp
        movlw   b'101001'              ;kirkas + tyhjays + ylos
        movwf   vieritys_moodi
        movfw   temp
        call    vieritys
        return

salama:
        movlw   5
        movwf   nopeus
        movlw   10
        call    pitka_viive
        movlw   1
        call    vieritys_0
        call    pois
        call    pois_1
        movlw   15
        call    pitka_viive
        call    paalle
        call    paalle_1
        movlw   10
        call    pitka_viive
        call    pois
        call    pois_1

        return

himmenna:
	movwf	temp
        swapf   status,w
        movwf   status_temp3

h_1:	
	movfw	temp
	call	pitka_viive	
	decfsz	contrast,f
	goto	h_1
        swapf   status_temp3,w
        movwf   status
	return

kirkasta:
	movwf	temp
        swapf   status,w
        movwf   status_temp3

k_1:
	movfw	temp
	call	pitka_viive
	incf	contrast
	movlw	b'100000'
	subwf	contrast,w
	btfss	status,0
	goto	k_1
        swapf   status_temp3,w
        movwf   status
	return

vilkutus2:
        movwf   lask1
v2_2:
        movlw   05
        call    himmenna

        movlw	0a0
	call	pitka_viive

        movlw   05
        call    kirkasta
        decfsz  lask1,f
        goto    v2_2
        return


rapytys:			;vilist‰‰ kaikkia ledeja
	movwf	lask2
	movlw   b'00111100'
        movwf   col1
        movlw   b'11001111'
        movwf   col2
        movlw   b'11110011'
        movwf   col3
        movlw   b'00111100'
        movwf   col4
	movfw	lask2
        call    kierto_o
	return

vilkutus:
        movwf   lask1
v_2:
        
        call    viive
        call    pois
        
        call    viive
        call    paalle
        decfsz  lask1,f
        goto    v_2
        return

strobo:
        movwf   lask1
strobo_2:
        call    paalle
        call    paalle_1
        movlw   3    
        call    pitka_viive
        call    pois
        call    pois_1
        
        call    viive
       
        decfsz  lask1,f
        goto    strobo_2
        return

taytto:
	movwf	temp
        movlw   021
        movwf   lask1
taytto2:
    	movfw	temp
	call    pitka_viive
        bcf     status,0
        call    kierratys_v
        decfsz  lask1,f
        goto    taytto2
        return

taytto_1:
	movwf	temp
        movlw   021
        movwf   lask1
taytto_1_2:
    	movfw	temp
	call    pitka_viive
        bcf     status,0
        call    kierratys_v_1
        decfsz  lask1,f
        goto    taytto_1_2
        return

tyhjays:
	movwf	temp
        movlw   020
        movwf   lask1
tyhjays2:
	movfw	temp
        call    pitka_viive
        bsf     status,0
        call    kierratys_v
        decfsz  lask1,f
        goto    tyhjays2
        return

tyhjays_1:
	movwf	temp
        movlw   020
        movwf   lask1
tyhjays_1_2:
	movfw	temp
        call    pitka_viive
        bsf     status,0
        call    kierratys_v_1
        decfsz  lask1,f
        goto    tyhjays_1_2
        return

kierratys_v:
        bcf     intcon,7          ;disable all interrupts
        btfsc   intcon,7
        goto    kierratys_v
        rlf     col1,f
        rlf     col2,f
        rlf     col3,f
        rlf     col4,f
        bsf     intcon,7         ;enable tmr0-interrupt
        return

kierratys_v_1:
        bcf     intcon,7          ;disable all interrupts
        btfsc   intcon,7
        goto    kierratys_v_1
        rlf     col5,f
        rlf     col6,f
        rlf     col7,f
        rlf     col8,f
        bsf     intcon,7         ;enable tmr0-interrupt
        return

kierratys_o:
        bcf     intcon,7          ;disable all interrupts
        btfsc   intcon,7
        goto    kierratys_o
        rrf     col4,f
        rrf     col3,f
        rrf     col2,f
        rrf     col1,f
        bsf     intcon,7         ;enable tmr0-interrupt
        return

kierratys_o_1:
        bcf     intcon,7          ;disable all interrupts
        btfsc   intcon,7
        goto    kierratys_o_1
        rrf     col8,f
        rrf     col7,f
        rrf     col6,f
        rrf     col5,f
        bsf     intcon,7         ;enable tmr0-interrupt
        return

kierto_01:
	movwf	lask2
        movlw   b'1'
        movwf   status_t1
        movwf   status_t2
kierto_01_2:        
        movlw   021
        movwf   lask1
kierto_01_3:
        movlw   10
        call    pitka_viive
        movfw   status_t1
        movwf   status
        call    kierratys_v
        movfw   status
        movwf   status_t1
        movfw   status_t2
        movwf   status
        btfsc   lask1,0
        call    kierratys_o_1
        movfw   status
        movwf   status_t2
        decfsz  lask1,f
        goto    kierto_01_3
        decfsz  lask2,f
        goto    kierto_01_2
	return

kierto_sama_suunta_01:

	movwf	lask2
        movlw   b'1'
        movwf   status_t1
        movwf   status_t2
kierto_s_01_2:        
        movlw   021
        movwf   lask1
kierto_s_01_3:
        movlw   10
        call    pitka_viive
        movfw   status_t1
        movwf   status
        call    kierratys_v
        movfw   status
        movwf   status_t1
        movfw   status_t2
        movwf   status
        call    kierratys_v_1
        movfw   status
        movwf   status_t2
        decfsz  lask1,f
        goto    kierto_s_01_3
        decfsz  lask2,f
        goto    kierto_s_01_2
	return


        

pois:
        movlw   b'11111111'
        movwf   col1
        movwf   col2
        movwf   col3
        movwf   col4
        return
pois_1:
        movlw   b'11111111'
        movwf   col5
        movwf   col6
        movwf   col7
        movwf   col8
        return

paalle:
        movlw   b'0'
        movwf   col1
        movwf   col2
        movwf   col3
        movwf   col4
        return

paalle_1:
        movlw   b'0'
        movwf   col5
        movwf   col6
        movwf   col7
        movwf   col8
        return

hae_0:
        bcf     status,rp0	;function 0=and 1=mov 2=comf+ior
	movwf	temp2
	movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col1,f
	btfsc	function,1
	movwf	col1
	btfss	function,2
	goto	ohi1
        comf    eedata,w
        iorwf   col1,f
ohi1:	
        incf    temp2,f
        movfw   temp2
        movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col2,f
	btfsc	function,1
	movwf	col2
	btfss	function,2
	goto	ohi2
        comf    eedata,w
        iorwf   col2,f
ohi2:	
        incf    temp2,f
        movfw   temp2
        movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col3,f
	btfsc	function,1
	movwf	col3
	btfss	function,2
	goto	ohi3
        comf    eedata,w
        iorwf   col3,f
ohi3:	
        incf    temp2,f
        movfw   temp2
        movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col4,f
	btfsc	function,1
	movwf	col4
	btfss	function,2
	goto	ohi4
        comf    eedata,w
        iorwf   col4,f
ohi4:	

	return
hae_1:
        bcf     status,rp0
	movwf	temp2
	movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col5,f
	btfsc	function,1
	movwf	col5
	btfss	function,2
	goto	ohi_1_1
        comf    eedata,w
        iorwf   col5,f
ohi_1_1:	
        incf    temp2,f
        movfw   temp2
        movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col6,f
	btfsc	function,1
	movwf	col6
	btfss	function,2
	goto	ohi_1_2
        comf    eedata,w
        iorwf   col6,f
ohi_1_2:	
        incf    temp2,f
        movfw   temp2
        movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col7,f
	btfsc	function,1
	movwf	col7
	btfss	function,2
	goto	ohi_1_3
        comf    eedata,w
        iorwf   col7,f
ohi_1_3:	
        incf    temp2,f
        movfw   temp2
        movwf   eeadr
        bsf     status,rp0
        bsf     eecon1,rd
        bcf     status,rp0
        movfw   eedata
	btfsc	function,0
	andwf   col8,f
	btfsc	function,1
	movwf	col8
	btfss	function,2
	goto	ohi_1_4
        comf    eedata,w
        iorwf   col8,f
ohi_1_4:	
	return


kierto_v:
	movwf	lask2
kierto2:        
        movlw   021
        movwf   lask1
        bsf     status,0
kierto3:
        call    viive
        call    kierratys_v
        decfsz  lask1,f
        goto    kierto3
        decfsz  lask2,f
	goto    kierto2
	return

kierto_o:
	movwf	lask2
kierto2b:        
        movlw   021
        movwf   lask1
        bsf     status,0
kierto3b:
        call    viive
        call    kierratys_o
        decfsz  lask1,f
        goto    kierto3b
        decfsz  lask2,f
	goto    kierto2b
	return

kierto_v2:
	movwf	lask2
ki2_2:        
        movlw   021
        movwf   lask1
        bsf     status,0
ki2_3:
	movlw	01
        call    himmenna
        call    kierratys_v
	movlw	01
	call	kirkasta
        decfsz  lask1,f
        goto    ki2_3
        decfsz  lask2,f
	goto    ki2_2
	return


vieritys:
        movwf   temp
        movlw   1
        movwf   lask2
        btfsc   vieritys_moodi,0
        movwf   lask2
        btfsc   vieritys_moodi,1
        movwf   lask2
vieritys3:
        btfsc   vieritys_moodi,0
        movlw   3c
        btfsc   vieritys_moodi,1
        movlw   0

        movwf   rivi
        movlw   010
        movwf   lask1
vieritys2:
        bcf     intcon,7          ;disable all interrupts
        btfsc   intcon,7
	goto	vieritys2

        btfsc   vieritys_moodi,2
        movlw   b'1'
        btfsc   vieritys_moodi,3
        movlw   b'100'
        btfsc   vieritys_moodi,4
        movlw   b'10'

        movwf   function
        movfw   rivi
        btfsc   vieritys_moodi,5
	call	hae_0
        btfsc   vieritys_moodi,6
        call    hae_1
        btfsc   vieritys_moodi,0
        movlw   -4
        btfsc   vieritys_moodi,1
	movlw	4
        addwf   rivi,f
	bsf	intcon,7
        btfsc   vieritys_moodi,0
        call    viive
        btfsc   vieritys_moodi,1
        call    viive
        decfsz  lask1,f
        goto    vieritys2
	decfsz	lask2,f
	goto	vieritys3
        return


        org     2100h

        de      b'11111110',b'11111111',b'11111111',b'11111111'
        de      b'11111101',b'11111111',b'11111111',b'01111111'
        de      b'11111011',b'11111111',b'11111111',b'10111111'
        de      b'11110111',b'11111111',b'11111111',b'11011111'
        de      b'11101111',b'11111111',b'11111111',b'11101111'
        de      b'11011111',b'11111111',b'11111111',b'11110111'
        de      b'10111111',b'11111111',b'11111111',b'11111011'
        de      b'11111111',b'11111111',b'00001111',b'11111100'
        de      b'01111111',b'11111111',b'11110111',b'11111111'
        de      b'11111111',b'11111110',b'11111011',b'11111111'
        de      b'11111111',b'11111101',b'11111101',b'11111111'
        de      b'11111111',b'11111011',b'11111110',b'11111111'
        de      b'11111111',b'01110111',b'11111111',b'11111111'
        de      b'11111111',b'10101111',b'11111111',b'11111111'
        de      b'11111111',b'11011111',b'11111111',b'11111111'
        de      b'11111111',b'11111111',b'11111111',b'11111111'

        end


