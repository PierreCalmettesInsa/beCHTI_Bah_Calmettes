	thumb
	
	
	area  moncode, code, readonly
	include	etat.inc
	export	timer_callback
	import	etat
	import	LongueurSon
	import	Son
	
TIM3_CCR3	equ	0x4000043C	; adresse registre PWM


	
	
timer_callback proc
	;il faut ajouter 32768 pour que toutes les valeurs soient poitives, soit 0x8000
	push {r4}

	ldr	r0, =etat
	ldr	r1, [r0,#E_POS]
	ldr	r2,[r0,#E_TAI]
	
	
	;si position < taille
	cmp	r1,r2 
	bge sinon
	
	
	;alors
	ldr	r3,[r0,#E_SON]
	ldrsh r4,[r3,r1,lsl #0x1]

	;ajout d'une composante continue
	add r4,#0x8000
	;multiplication par un facteur d'échelle
	ldr r12,[r0,#E_RES]
	mul r4, r12    ;on multiplie l'chantillon par la résolution
	mov r12,#0xFFFF
	udiv r4,r12     ;on divise l'échantillon par 2^16 -1
	
	
	;on envoie l'échaillon à PWM
	ldr r12, =TIM3_CCR3   ;addresse de PWM
	str r4,[r12]      ;echantillon mis à l'adresse de PWM
	;on incremente la position
	add r1,#0x1
	str	r1,[r0, #E_POS]
	b fin
	
sinon
	mov r3,#0	;echantillon à zero
	add r3, #0x8000
	ldr r12,[r0,#E_RES]
	mul r3, r12    ;on multiplie l'chantillon par la résolution
	mov r12,#0xFFFF
	udiv r3,r12     ;on divise l'échantillon par 2^16 -1
	;mov r1,#0
	;str r1,[r0,#E_POS]
	ldr r12,=TIM3_CCR3
	str r3,[r12]

	
fin

	pop {r4}
	bx lr

	endp
	end
	

