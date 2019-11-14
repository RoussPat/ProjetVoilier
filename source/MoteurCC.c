#include "MoteurCC.h"

#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_tim.h" 

void initMoteurCC () {
	
//vitesse de rotation en PWM sur PA1 TIM2_CH2
//sens en pushpull sur PA2

//TIM4
	RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM2EN;	//TIM2 enable
	TIM2->CCMR1 &= ~TIM_CCMR1_OC2M_0;
	TIM2->CCMR1 |=  TIM_CCMR1_OC2M_1;
	TIM2->CCMR1 |=  TIM_CCMR1_OC2M_2;
	
	//TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
	//TIM2->CR1   |= TIM_CR1_ARPE;
	
	//TIM2->EGR   |= TIM_EGR_UG;
	
	TIM2->CCER  |= TIM_CCER_CC2E;
	
	
	
	TIM2->ARR=99;
	TIM2->PSC=2000;
	
	
	
		//PA1
	RCC->APB2ENR =  RCC->APB2ENR | RCC_APB2ENR_IOPAEN;
	//configuration du BP START 1011
	GPIOA->CRL= GPIOA->CRL & ~GPIO_CRL_CNF1_0;
	GPIOA->CRL= GPIOA->CRL |  GPIO_CRL_CNF1_1;
	GPIOA->CRL= GPIOA->CRL |	GPIO_CRL_MODE1_0;
	GPIOA->CRL= GPIOA->CRL |  GPIO_CRL_MODE1_1;
	
	//PA2 pushpull
	//configuration du BP START 0001
	GPIOA->CRL= GPIOA->CRL & ~GPIO_CRL_CNF2_0;
	GPIOA->CRL= GPIOA->CRL & ~GPIO_CRL_CNF2_1;
	GPIOA->CRL= GPIOA->CRL |	GPIO_CRL_MODE2_0;
	GPIOA->CRL= GPIOA->CRL & ~GPIO_CRL_MODE2_1;
	
	//DEMARRE CLOCK
	LL_TIM_EnableCounter(TIM2);
	TIM2->CR1 |= TIM_CR1_CEN; // demarrage de la pwm
}

void tournerPlateau(short int vitesse, short int sens){
	TIM2->CCR2 = vitesse	;//facteur de la resolution de la pwm
	if (sens) {
		GPIOA->ODR |=  GPIO_ODR_ODR2;
	} else {
		GPIOA->ODR &= ~GPIO_ODR_ODR2;
	}
}