#include "ServoMoteur.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_tim.h" 

//PWM output 
//emmision entre 
//PA8 et PB8 + choix de la clock par cavalier



void initServoMoteur () {
	
//angle des voiles (0 -> 90°) en PWM sur PB8 TIM4_CH3
	
//TIM4
	RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM4EN;	//TIM4 enable
	TIM4->CCMR2 &= ~TIM_CCMR2_OC3M_0;
	TIM4->CCMR2 |=  TIM_CCMR2_OC3M_1;
	TIM4->CCMR2 |=  TIM_CCMR2_OC3M_2;
	
	TIM4->CCER  |= TIM_CCER_CC3E;
	
	TIM4->ARR=47999;
	TIM4->PSC=0x1D;
	
	
	
		//PB8
	RCC->APB2ENR =  RCC->APB2ENR | RCC_APB2ENR_IOPBEN;
	//configuration du BP START 1011
	GPIOB->CRH= GPIOB->CRH & ~GPIO_CRH_CNF8_0;
	GPIOB->CRH= GPIOB->CRH |  GPIO_CRH_CNF8_1;
	GPIOB->CRH= GPIOB->CRH |	GPIO_CRH_MODE8_0;
	GPIOB->CRH= GPIOB->CRH |  GPIO_CRH_MODE8_1;

	//DEMARRE CLOCK
	LL_TIM_EnableCounter(TIM4);
	TIM4->CR1 |= TIM_CR1_CEN; // demarrage de la pwm
}

void setAngle(short int angle){
	TIM4->CCR3 = 0x0C00 + ((0x1240-0x0C00)*(angle/90))	;//facteur de la resolution de la pwm de  0 ->2400 -> 4800
}