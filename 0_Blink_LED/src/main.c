/*-------------------------------------------------------------
 - Su dung LED có san trên Blue Pill - PC13. Lap trinh nhay LED
 - Theo Schematic. Ghi 1 lên chân PC13 là LED sáng.
 - Thêm vào lib thu viên rcc và gpio



-------------------------------------------------------------*/

#include "stm32f10x.h"

void delay(void);
GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
	{
		
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		
	
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
		
		while (1)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			GPIO_ResetBits(GPIOC, GPIO_Pin_14);
			delay();
			delay();
			delay();
			delay();
			delay();
			delay();
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
			delay();
			delay();
			delay();
			delay();
			delay();
			delay();
		}
	}
	
	void delay(void)
	{
		uint32_t i;
		for(i = 0; i < 0xfffff ; i++)
		{
		}
	}
//
