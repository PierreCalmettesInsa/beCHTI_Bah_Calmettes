#include "gassp72.h"

int module(unsigned short * tabSig, int k);

unsigned short int dma_buf[64];

int compteurOccurence[6];
	
int M2TIR = 985507 ;


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
	

	
	

	while (1) {}


}
