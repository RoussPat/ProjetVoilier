#include "MyTimer.h"
#include "ServoMoteur.h"
#include "MoteurCC.h"
#include "RecepteurHF.h"
#include "CodeurIncremental.h"
#include "stm32f1xx_ll_tim.h" 
	

// fonction Handler 
void SysTick_Handler(void) {
	
	//--------------ANGLE------------
	static int angle;
	angle	= getAngle(); //angle entre 0 et 360
	// on ramène l'angle entre 0 et 180 (par rapport à la proue du voilier)
	if (angle >180) {
		angle = 360 - angle;
	}
	//on traite les deux cas : fonction affine (angle entre 45 et 90) ou renvoie 0 sinon (trop face au vent)
	if (angle >= 45) {
		setAngle(((angle-45)*2)/3);
	}
	else {
		setAngle(0);
	}
	
	//------------MOTEUR CC--------------
	int a = GetCommande();
	if(a>0) {
		tournerPlateau(a*(100/85), 1);
	}else {
		tournerPlateau(a*(-100/85), 0);
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
