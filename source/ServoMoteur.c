#include "ServoMoteur.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_tim.h" 

//PWM output 
//emmision entre 
//PA8 et PB8 + choix de la clock par cavalier



void initServoMoteur () {
	
//angle des voiles (0 -> 90°) en PWM sur PA8 TIM1_CH1
	
//TIM4
	RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_TIM1EN;	//TIM4 enable
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
	TIM1->CCMR1 |=  TIM_CCMR1_OC1M_1;
	TIM1->CCMR1 |=  TIM_CCMR1_OC1M_2;
	
	TIM1->CCER  |= TIM_CCER_CC1E;
	TIM1->BDTR  |= TIM_BDTR_MOE;
	TIM1->ARR=47999; 
	TIM1->PSC=0x1D; 
	
	
	
		//PB8
	RCC->APB2ENR =  RCC->APB2ENR | RCC_APB2ENR_IOPAEN;
	//configuration du BP START 1011
	GPIOA->CRH= GPIOA->CRH & ~GPIO_CRH_CNF8_0;
	GPIOA->CRH= GPIOA->CRH |  GPIO_CRH_CNF8_1;
	GPIOA->CRH= GPIOA->CRH |	GPIO_CRH_MODE8_0;
	GPIOA->CRH= GPIOA->CRH |  GPIO_CRH_MODE8_1;

	//DEMARRE CLOCK
	LL_TIM_EnableCounter(TIM1);
	TIM1->CR1 |= TIM_CR1_CEN; // demarrage de la pwm
}

void setAngle(short int angle){
	TIM1->CCR1 = 0x0C00 + ((0x1240-0x0C00)*((float)angle/90.0))	;//facteur de la resolution de la pwm de  0 ->2400 -> 4800
}
