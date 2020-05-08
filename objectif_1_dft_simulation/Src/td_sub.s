; ce programme est pour l'assembleur RealView (Keil)
	thumb
	area  moncode, code, readonly
	export	module
	import 	TabCos
	import	TabSin


module	proc
	push	{lr}
	;r0 : tabSig    r1: k
	
	;appel de calculk avec cos
	
	ldr		r2,=TabCos
	ldr 	r3,=TabSin
	BL		calculk

;r0 : resultat avec cos    r1 : resultat avec sin

	
	smull	r2,r3,r0,r0
	SMLAL	r2,r3,r1,r1
	mov		r0,r3
	pop		{LR}
	BX		LR
	endp
	

calculk	proc
	
	; r0 = tabSig    r1 : k   r2 : tabCos r3 : TaSin
	push	{r4-r8}
	mov		r12, #0x00  ; i=0
	b		loop
	
loop
	;calcul ik
	mul		r5,r1,r12
	AND		r5, #0x03F   ;modulo 64
	
	ldrsh	r4,[r2,r5, lsl #0x01]   ;cos(ik)
	ldrsh	r7,[r3,r5, lsl #0x01]   ;sin(ik)
	ldrsh	r5,[r0,r12, lsl #0x01]   ;x(i)
	
	mul		r4,r4,r5	;cos(ik)x(i) ou sin(ik)x(i)
	
	mul		r7,r7,r5	;sin(ik)x(i)
	
	add		r8,r7
	add		r6,r4
	
	add		r12,#0x01
	
	cmp		r12,#64
	bne		loop
	
	; on quitte la boucle
	
	mov		r0,r6
	mov		r1,r8
	b 		fin
fin
	pop		{r4-r8}
	BX		LR
	
	endp
	end