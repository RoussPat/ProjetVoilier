#include "CodeurIncremental.h"
#include "stm32f103xb.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_tim.h"

//capture compare 
//CHA sur PA6 -- floating input
//CHB sur PA7 -- floating input
//l'index sur PA5 -- true axe du voilier (angle 0)
	//avant le while, mettre le compteur à 0 -- faire faire un tour à vide à la girouette


void initCodeurIncremental(){
	//RCC enable GPIOA et TIM3
	RCC->APB1ENR=RCC_APB1ENR_TIM3EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//configuration des broches PA6 et PA7 -- en floating input
	LL_GPIO_InitTypeDef CHA = {LL_GPIO_PIN_6, LL_GPIO_MODE_FLOATING, LL_GPIO_MODE_OUTPUT_2MHz, LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_DOWN };
	LL_GPIO_Init(GPIOA, &CHA);
	
	LL_GPIO_InitTypeDef CHB = {LL_GPIO_PIN_7, LL_GPIO_MODE_FLOATING, LL_GPIO_MODE_OUTPUT_2MHz, LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_DOWN };
	LL_GPIO_Init(GPIOA, &CHB);
	
	//configuration de la broche PA5
	LL_GPIO_InitTypeDef INDEX = {LL_GPIO_PIN_5, LL_GPIO_MODE_FLOATING, LL_GPIO_MODE_OUTPUT_2MHz, LL_GPIO_OUTPUT_PUSHPULL,LL_GPIO_PULL_DOWN };
	LL_GPIO_Init(GPIOA, &INDEX);
	
	//compter sur TI1 et TI2 (donc division par 4 nécessaire)
	TIM3->SMCR |= TIM_SMCR_SMS;
	
	//polarité (non nversé [valeur à 0] et sur front montant)
	TIM3->CCER ^= TIM_CCER_CC1P;
	TIM3->CCER ^= TIM_CCER_CC2P;
	
	//Autoreload (359)
	TIM2->ARR = 0x167;
	
	//enable le counter
	TIM3->CR1 |= TIM_CR1_CEN;
	
}

void waitInitGirouette() {
	int fini = 0;
	while (!fini) {
		//regarde la valeur de INDEX (donc PA5)
		if ((GPIOA->IDR & 1<<5) == 1<<5) { 
			//mettre le compteur à zéro
			TIM3->CNT=0;
			fini=1;
		}
	}
}

int getAngle(){
	return (TIM3->CNT);
} 



