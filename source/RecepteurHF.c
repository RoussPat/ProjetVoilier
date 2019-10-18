#include "RecepteurHF.h"
#include "stm32f1xx_ll_usart.h"

// RF teleco 
// PB6 -> ch1 DONC TIM4 [ou PB7 -> ch2]
//PWM input
//largeur d'impultion entre 1ms et 2ms + impultion toute les 20 ms

void InitRecepteur() {
	//TIM4
	
	//PWM
}
	
	
void RecevoirMessage() {
//série asynchrone, sans parité, 1 seul bit de stop, 9600 bauds
}