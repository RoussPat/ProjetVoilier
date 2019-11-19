#include "MyTimer.h"
#include "ServoMoteur.h"
#include "CodeurIncremental.h"
#include "stm32f1xx_ll_tim.h" 
	

// fonction Handler 
void SysTick_Handler(void) {
		if (getAngle() != 0) {
			SetAngle(getAngle()/2);
		}
		else { //face au vent -> fermer les voilesSetAngle
			SetAngle(0);
			//option : enoyer message
		}
}

void MyTimerConf() {
	//enable clock -- par défaut c'est ok
	//configuration de la fréquence
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	//set à 0
	SysTick->VAL   = 0UL;
	//arr -- reload -- toutes les 500ms
	SysTick->LOAD |= 0x2255100; // freq entrée = 7210^6 -> 36*10^6 ->
	// enable interruption
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	//priority (très important, active le SysTick_TickInt)
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
	//enable == lancer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

	
// La fonction affine pour la variable angle de SetAngle : 
// fonction affine getAngle(alpha) -> return Angle_Teta
// alpha = 180 -> Teta = 90
// alpha = 90  -> Teta = 45
// alpha = 0   -> On ferme les voiles
// DONC : f(alpha)= alpha/2=teta SAUF pour alpha=0