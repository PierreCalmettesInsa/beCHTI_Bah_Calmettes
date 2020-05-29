#include "gassp72.h"
#include "etat.h"

//dft
int module(unsigned short * tabSig, int k);
unsigned short int dma_buf[64];
int compteurOccurence[6];	
int M2TIR = 985507 ;
//son
void son_callback(void);
extern short Son;
extern int LongueurSon;
type_etat etat ;


void sys_callback(void){
	int k ;
	GPIO_Set(GPIOB,1);
	
	// Démarrage DMA pour 64 points
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	
	for (int i = 0; i<6;i++){
		
		k = i+17 ;
		if (i>3){
			k+=2 ;
		}
		
		if (module(dma_buf,k)>= M2TIR){
			compteurOccurence[i] += 1 ;
		} else {
			compteurOccurence[i] = 0 ;
		}
	}	
	
	for (int i = 0; i<6;i++){
		if ((compteurOccurence[i]>3) && (etat.position >= 1000)){
			etat.position = 0 ;
		}
	}
	
	GPIO_Clear(GPIOB,1);
	
}


int main(void){
	
	
	// activation de la PLL qui multiplie la fréquence du quartz par 9
	CLOCK_Configure();
	// PA2 (ADC voie 2) = entrée analog
	GPIO_Configure(GPIOA, 2, INPUT, ANALOG);
	// PB1 = sortie pour profilage à l'oscillo
	GPIO_Configure(GPIOB, 1, OUTPUT, OUTPUT_PPULL);
	// PB14 = sortie pour LED
	GPIO_Configure(GPIOB, 14, OUTPUT, OUTPUT_PPULL);

	// activation ADC, sampling time 1us
	Init_TimingADC_ActiveADC_ff( ADC1, 0x52 );
	Single_Channel_ADC( ADC1, 2 );
	// Déclenchement ADC par timer2, periode (72MHz/320kHz)ticks
	Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
	// Config DMA pour utilisation du buffer dma_buf (a créér)
	Init_ADC1_DMA1( 0, dma_buf );

	// Config Timer, période exprimée en périodes horloge CPU (72 MHz)
	Systick_Period_ff( 5000*72 );
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 3 est la priorité, sys_callback est l'adresse de cette fonction, a créér en C
	Systick_Prio_IT( 3, sys_callback );
	SysTick_On;
	SysTick_Enable_IT;
	
	//son
	//init du son
	etat.position = 1001 ;
	// config TIM3-CH3 en mode PWM
	etat.resolution = PWM_Init_ff( TIM3, 3, 655 );	
	etat.periode_ticks = 6550 ;
	etat.son = &Son;
	etat.taille = LongueurSon ;	
	// initialisation du timer 4
	// Periode_en_Tck doit fournir la durée entre interruptions,
	// exprimée en périodes Tck de l'horloge principale du STM32 (72 MHz)
	u32 Periode_en_Tck = 6550 ;
	Timer_1234_Init_ff( TIM4, Periode_en_Tck );
	// enregistrement de la fonction de traitement de l'interruption timer
	// ici le 2 est la priorité, timer_callback est l'adresse de cette fonction, a créér en asm,
	// cette fonction doit être conforme à l'AAPCS
	Active_IT_Debordement_Timer( TIM4, 2, son_callback );
	// lancement des timer
	Run_Timer( TIM4 );
	Run_Timer( TIM3);
	// config port PB0 pour être utilisé par TIM3-CH3
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
	
	

	while (1) {}


}
