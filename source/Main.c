#include "Accelerometre.h" //TODO je sais pas si ça marche
#include "Accu7V2.h"
#include "CodeurIncremental.h"
#include "EmetteurHF.h"
#include "MoteurCC.h"
#include "RecepteurHF.h"
#include "ServoMoteur.h"
#include <stdio.h>

#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config

void SystemClock_Config(void);

int main (int argc, char * argv[]){
	SystemClock_Config();
	/* ----- TEST DE L'EMETTEUR HF -----
	InitEmeteur();
	EnvoyerMessage("test",4);
	EnvoyerMessage("test",4);*/
	
	/* ----- TEST DU RECEPTEUR HF ----- */
	static int a=0;
	InitRecepteurHF();
	while(1){
		a = GetCommande();
		//printf("%d\n",a);
	}
	
	
	/* ----- TEST DU MOTEURCC ----- 
	initMoteurCC();
	tournerPlateau(80, 1);
	
	while(1);
	*/

	// ------ CONFIGURATION -----
	InitRecepteurHF();
	initMoteurCC();
	initCodeurIncremental();
	waitInitGirouette();
	
	// ----- ANGLES DES VOILES -----
	// DOC CORTEX PAGE 151
	// faire nouveau fichier myTimer 
	/* fonction MyTimerConf :
		~ comme RCC->APB2ENR |= RCC_APB1ENR_TIM4EN;
		Timer->ARR = ???;
		Timer->PSC = ???;
	*/
	/* fonction Handler :
		void SysTick_QHandler(void) {
			SetAngle(getAngle()/2);
		}
	*/
	while(1) {
		if (getAngle() != 0) {
			SetAngle(getAngle()/2); // Utilisation de SysTick et f° d'interruption
		}
		else { //face au vent -> fermer les voilesSetAngle
			SetAngle(0);
			//option : enoyer message
		}
	}
// La fonction affine pour la variable angle de SetAngle : 
// fonction affine getAngle(alpha) -> return Angle_Teta
// alpha = 180 -> Teta = 90
// alpha = 90  -> Teta = 45
// alpha = 0   -> On ferme les voiles
// DONC : f(alpha)= alpha/2=teta SAUF pour alpha=0

}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
	
	
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucléo*********************************
  //LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000); // utile lorsqu'on utilise la fonction LL_mDelay

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}





/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */




/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
		
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
