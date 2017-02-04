#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//ALIENTEK STM32F407开发板
//LED驱动代码	   
////////////////////////////////////////////////////////////////////////////////// 	

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //使能GPIOD的时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //上拉
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10); //GPIOF9,10高电平
}

void Set_Led(u8 LED,u8 state)
{
	if(LED==0)
	{
		switch(state)
		{
			case 0: LED0=1;
							break;
			case 1: LED0=0;
							break;
		}
	}else if(LED==1)
	{
		switch(state)
		{
			case 0: LED1=1;
							break;
			case 1: LED1=0;
							break;
		}
	}
}

