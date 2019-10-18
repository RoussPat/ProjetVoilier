#include "EmetteurHF.h"
#include "stm32f1xx_ll_usart.h"

// data IN -> PA9 de l'emeteur
// TX ENABLE -> PA11 de l'emeteur push pull 1 ou 0 allumé ou eteint
// l'emeteur sera en connexion usart serie avec le STM32

void InitEmeteur(){

	//enable de l'usart 1
	RCC->APB1ENR |= RCC_APB1ENR_USART1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ;
	//Config de la pate PA9
	GPIOA->CRH= GPIOA->CRH &~GPIO_CRH_CNF9_0;
	GPIOA->CRH= GPIOA->CRH | GPIO_CRH_CNF9_1;
	GPIOA->CRH= GPIOA->CRH | GPIO_CRH_MODE9_0;
	GPIOA->CRH= GPIOA->CRH | GPIO_CRH_MODE9_1;
	//Config de la pate PA11 en push pull GPO
	GPIOA->CRH= GPIOA->CRH &~GPIO_CRH_CNF11_0;
	GPIOA->CRH= GPIOA->CRH &~GPIO_CRH_CNF11_1;
	GPIOA->CRH= GPIOA->CRH | GPIO_CRH_MODE11_0;
	GPIOA->CRH= GPIOA->CRH &~GPIO_CRH_MODE11_1;
	
	
	LL_USART_InitTypeDef stru ;
	stru.BaudRate = 9600;
	stru.DataWidth = LL_USART_DATAWIDTH_8B ;
	stru.HardwareFlowControl = LL_USART_HWCONTROL_NONE ;
	stru.OverSampling = LL_USART_OVERSAMPLING_16 ;
	stru.Parity = LL_USART_PARITY_NONE;
	stru.StopBits = LL_USART_STOPBITS_1 ;
	stru.TransferDirection = LL_USART_DIRECTION_TX ;
	LL_USART_Init(USART1, &stru);
	LL_USART_Enable(USART1);
}
void EnvoyerMessage(char* str, int size){
	char data;//k
	int i;
	for(i=0;i<size;i++){
		data = str[i];
		while(!LL_USART_IsActiveFlag_TXE(USART1)){
			//do nothing;
		}
		LL_USART_TransmitData8(USART1, data); 
	}
}
