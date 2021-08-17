/*-------------------------------------------------------------
 - Su dung LED có san trên Blue Pill - PC13. Lap trinh nhay LED
 - Theo Schematic. Ghi 1 lên chân PC13 là LED sáng.
 - Thêm vào lib thu viên rcc và gpio



-------------------------------------------------------------*/

#include "main.h"

static __IO uint32_t TimingDelay;

void delay(void);
void Delay_us(__IO uint32_t nTime);
GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
	{
		/* SysTick Config */
		if (SysTick_Config(SystemCoreClock / 1000000))
		{ 
			/* Capture error */ 
			while (1);
		}
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);
		
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		while (1)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
			GPIO_ResetBits(GPIOC, GPIO_Pin_14);
			
			Delay_us(1000000);
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
			
			Delay_us(1000000);
		}
	}
	
	void delay(void)
	{
		uint32_t i;
		for(i = 0; i < 0xfffff ; i++)
		{
		}
	}
	
	/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in us.
  * @retval None
  */
void Delay_us(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
//
