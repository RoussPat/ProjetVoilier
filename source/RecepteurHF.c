#include "RecepteurHF.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_tim.h" 

// page 385
// RF teleco 
// PB6 -> ch1 DONC TIM4 [ou PB7 -> ch2]
//PWM input
//largeur d'impultion entre 1ms et 2ms + impultion toute les 20 ms
//envoie d'un signed short int en retour vers le main pour 
//PSC = 0 ARR = 27 pour T = 2ms /512 (pour une precision de 128)
void InitRecepteurHF() {
	
	
	//TIM4
	RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM4EN;	//TIM4 enable
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
	TIM4->CCMR1 |=  TIM_CCMR1_CC1S_0;
	
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
	
	TIM4->ARR=0xFFFF;
	TIM4->PSC=281;
	
		//PB6
	RCC->APB2ENR =  RCC->APB2ENR | RCC_APB2ENR_IOPBEN;
	//configuration du BP START 0100
	GPIOB->CRL= GPIOB->CRL |  GPIO_CRL_CNF6_0;
	GPIOB->CRL= GPIOB->CRL &~ GPIO_CRL_CNF6_1;
	GPIOB->CRL= GPIOB->CRL &~	GPIO_CRL_MODE6_0;
	GPIOB->CRL= GPIOB->CRL &~ GPIO_CRL_MODE6_1;
	
	//DEMARRE CLOCK
	LL_TIM_EnableCounter(TIM4);
}
	
	
int GetCommande() {
	return (TIM4->CCR2 - 401);
}