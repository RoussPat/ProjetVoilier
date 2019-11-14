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
	
	/*
	To select Encoder Interface mode write SMS=‘001 in the TIMx_SMCR register if the counter
is counting on TI2 edges only, SMS=010 if it is counting on TI1 edges only and SMS=011 if
it is counting on both TI1 and TI2 edges.*/
	//compter sur TI1 et TI2 (donc division par 4 nécessaire)
	TIM3->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
	TIM3->SMCR &= ~TIM_SMCR_SMS_2;

	
	/*
Select the TI1 and TI2 polarity by programming the CC1P and CC2P bits in the TIMx_CCER
register. When needed, you can program the input filter as well.*/
	//polarité (non nversé [valeur à 0] et sur front montant)
	TIM3->CCER ^= TIM_CCER_CC1P;
	TIM3->CCER ^= TIM_CCER_CC2P;


	TIM3->CCMR1 ^= TIM_CCMR1_IC1F;
	TIM3->CCMR2 ^= TIM_CCMR1_IC2F;
/*	
The two inputs TI1 and TI2 are used to interface to an incremental encoder. Refer to
Table 85. The counter is clocked by each valid transition on TI1FP1 or TI2FP2 (TI1 and TI2
after input filter and polarity selection, TI1FP1=TI1 if not filtered and not inverted,
TI2FP2=TI2 if not filtered and not inverted) assuming that it is enabled (CEN bit in
TIMx_CR1 register written to ‘1). 
The sequence of transitions of the two inputs is evaluated
and generates count pulses as well as the direction signal. Depending on the sequence the
counter counts up or down, the DIR bit in the TIMx_CR1 register is modified by hardware
accordingly. The DIR bit is calculated at each transition on any input (TI1 or TI2), whatever
the counter is counting on TI1 only, TI2 only or both TI1 and TI2.

*/	
	//enable le counter
	TIM3->CR1 |= TIM_CR1_CEN;
	
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM3->CCMR1 ^= TIM_CCMR1_CC1S_1;
	
	TIM3->CCMR2 |= TIM_CCMR1_CC2S_0;
	TIM3->CCMR2 ^= TIM_CCMR1_CC2S_1;
	

/*
Encoder interface mode acts simply as an external clock with direction selection. This
means that the counter just counts continuously between 0 and the auto-reload value in the
TIMx_ARR register (0 to ARR or ARR down to 0 depending on the direction). So you must
configure TIMx_ARR before starting. In the same way, the capture, compare, prescaler,
trigger output features continue to work as normal
	*/	
	//Autoreload (359)
	TIM3->ARR = 0x59f;
	

	
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
	return (TIM3->CNT / 4);
} 



