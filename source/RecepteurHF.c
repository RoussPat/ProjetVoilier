#include "RecepteurHF.h"
#include "stm32f1xx_ll_usart.h"

// RF teleco 
// PB6 -> ch1 DONC TIM4 [ou PB7 -> ch2]
//PWM input
//largeur d'impultion entre 1ms et 2ms + impultion toute les 20 ms
//envoie d'un signed short int en retour vers le main pour 
void InitRecepteur(int Arr, int Psc) {
	
	//TIM4
	RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM4EN;	//TIM4 enable
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
	TIM4->CCMR1 |= ~TIM_CCMR1_CC1S_0;
	
	TIM4->CCER  &= ~TIM_CCER_CC1P;
	
	TIM4->CCMR1 |=  TIM_CCMR1_CC2S_1;
	TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
	
	TIM4->CCER  |=  TIM_CCER_CC2P;

	TIM4->SMCR  |=  TIM_SMCR_TS_2;
	TIM4->SMCR  &= ~TIM_SMCR_TS_1;
	TIM4->SMCR  |=  TIM_SMCR_TS_0;
	
	TIM4->SMCR  |=  TIM_SMCR_SMS_2;
	TIM4->SMCR  &= ~TIM_SMCR_SMS_1;
	TIM4->SMCR  &= ~TIM_SMCR_SMS_0;
	
	TIM4->CCER  |=  TIM_CCER_CC1E;
	TIM4->CCER  |=  TIM_CCER_CC2E;
	
	//PWM
	TIM4->ARR=Arr;
	TIM4->PSC=Psc;
	
}
	
	
void RecevoirMessage() {
//série asynchrone, sans parité, 1 seul bit de stop, 9600 bauds
}