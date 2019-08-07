#include "main.h"

int null_temp_value;

void io_reset (void)
{	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
													RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
													RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
													RCC_APB2Periph_GPIOG, ENABLE); 
	GPIO_DeInit (GPIOA);
	GPIO_DeInit (GPIOB);
	GPIO_DeInit (GPIOC);
	GPIO_DeInit (GPIOD);
	GPIO_DeInit (GPIOE);
	GPIO_DeInit (GPIOF);
	GPIO_DeInit (GPIOG);
}

void drive_io_init (void)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	/*开启GPIO外设时钟*/
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB |
													RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
													RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
													RCC_APB2Periph_GPIOG, ENABLE); 
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;				 //
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	//////////////////////////////////////////////////////////////////////
	//GPIO_DeInit (GPIOB);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;				 //
	//GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);		
//	GPIO_SetBits(GPIOB, GPIO_Pin_3);		
//	GPIO_SetBits(GPIOB, GPIO_Pin_4);		
//	GPIO_SetBits(GPIOB, GPIO_Pin_5);		
//	GPIO_SetBits(GPIOB, GPIO_Pin_6);		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	//////////////////////////////////////////////////////////////////////
	//GPIO (GPIOC);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;				 //
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	//////////////////////////////////////////////////////////////////////
	//GPIO_DeInit (GPIOC);
	PWR_BackupAccessCmd( ENABLE );/* 允许修改RTC和后备寄存器*/ 
	RCC_LSEConfig( RCC_LSE_OFF ); /* 关闭外部低速时钟,PC14+PC15可以用作普通IO*/ 
	BKP_TamperPinCmd(DISABLE); /* 关闭入侵检测功能,PC13可以用作普通IO*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;				 //
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PWR_BackupAccessCmd(DISABLE);/* 禁止修改RTC和后备寄存器*/
	//////////////////////////////////////////////////////////////////////
	GPIO_DeInit (GPIOD);
	GPIO_DeInit (GPIOE);
	GPIO_DeInit (GPIOF);
	GPIO_DeInit (GPIOG);
	//////////////////////////////////////////////////////////////////////
	/*设置引脚模式为内部上拉输入*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	/*选择要配置的引脚*/																				   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	/*调用库函数，初始化GPIO*/
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	//////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*选择要配置的引脚*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	//////////////////////////////////////////////////////////////////////
	/* 关闭所有输出	*/	 
	GPIO_SetBits(GPIOF, GPIO_Pin_All);	
	GPIO_SetBits(GPIOG, GPIO_Pin_All);	
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	GPIO_Init(GPIOG, &GPIO_InitStructure);		
	
}

//外部中断线0的中断服务函数
void EXTI0_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line0); //清除中断线0挂起标志位
	OSQPost(io_msg, (void *) 0x55);//发送消息
	OSIntExit();
}
//外部中断线1的中断服务函数
void EXTI1_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line1); //清除中断线1挂起标志位
	OSQPost(io_msg, (void *) 0x55);//发送消息
	OSIntExit();
}
//外部中断线2的中断服务函数
void EXTI2_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line2); //清除中断线2挂起标志位
	OSQPost(io_msg, (void *) 0x55);//发送消息
	OSIntExit();
}
//外部中断线3的中断服务函数
void EXTI3_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line3); //清除中断线3挂起标志位
	OSQPost(io_msg, (void *) 0x55);//发送消息
	OSIntExit();
}

//外部中断线6 7 8的中断服务函数
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line6 | EXTI_Line7 | EXTI_Line8); //清除中断线6挂起标志位
	OSQPost(io_msg, (void *) 0x55);//发送消息
	OSIntExit();
}
//外部中断线11的中断服务函数
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line11); //清除中断线11挂起标志位
	OSQPost(io_msg, (void *) 0x55);//发送消息
	OSIntExit();
}

























