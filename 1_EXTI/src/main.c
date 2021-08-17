/**
  ******************************************************************************
  * 
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;
uint16_t debug = 0;

/* Private function prototypes -----------------------------------------------*/
void EXTI0_Config(void);			

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	SysTick_Config(SystemCoreClock/1000);			// cau hinh systick ngat moi 1ms
  
  /* Configure PA.01 in interrupt mode */
	/* debug++ when call interrupt */
  EXTI0_Config();
	
  while (1)
  {
  }
}

/**
  * @brief  Configure PA.01 in interrupt mode
	* doi trang thai dieu khien khi nhan nut (co dieu khien/khong dieu khien)
  * @param  None
  * @retval None
  */
void EXTI0_Config(void)
{
  /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);								// GPIOA in PA01
  
  /* Configure PA.01 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;														// Pin_1 in PA01
	// GPIO_InitStructure.GPIO_Speed = ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;								// floating vi co tro keo len o ngoai roi
  GPIO_Init(GPIOA, &GPIO_InitStructure);															// GPIOA in PA01

  /* Connect EXTI0 Line to PA.01 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

  /* Configure EXTI1 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
