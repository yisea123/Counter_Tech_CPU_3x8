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
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	/*����GPIO����ʱ��*/
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
	PWR_BackupAccessCmd( ENABLE );/* �����޸�RTC�ͺ󱸼Ĵ���*/ 
	RCC_LSEConfig( RCC_LSE_OFF ); /* �ر��ⲿ����ʱ��,PC14+PC15����������ͨIO*/ 
	BKP_TamperPinCmd(DISABLE); /* �ر����ּ�⹦��,PC13����������ͨIO*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;				 //
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PWR_BackupAccessCmd(DISABLE);/* ��ֹ�޸�RTC�ͺ󱸼Ĵ���*/
	//////////////////////////////////////////////////////////////////////
	GPIO_DeInit (GPIOD);
	GPIO_DeInit (GPIOE);
	GPIO_DeInit (GPIOF);
	GPIO_DeInit (GPIOG);
	//////////////////////////////////////////////////////////////////////
	/*��������ģʽΪ�ڲ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	/*ѡ��Ҫ���õ�����*/																				   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	//////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*ѡ��Ҫ���õ�����*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	//////////////////////////////////////////////////////////////////////
	/* �ر��������	*/	 
	GPIO_SetBits(GPIOF, GPIO_Pin_All);	
	GPIO_SetBits(GPIOG, GPIO_Pin_All);	
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	GPIO_Init(GPIOG, &GPIO_InitStructure);		
	
}

//�ⲿ�ж���0���жϷ�����
void EXTI0_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line0); //����ж���0�����־λ
	OSQPost(io_msg, (void *) 0x55);//������Ϣ
	OSIntExit();
}
//�ⲿ�ж���1���жϷ�����
void EXTI1_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line1); //����ж���1�����־λ
	OSQPost(io_msg, (void *) 0x55);//������Ϣ
	OSIntExit();
}
//�ⲿ�ж���2���жϷ�����
void EXTI2_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line2); //����ж���2�����־λ
	OSQPost(io_msg, (void *) 0x55);//������Ϣ
	OSIntExit();
}
//�ⲿ�ж���3���жϷ�����
void EXTI3_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line3); //����ж���3�����־λ
	OSQPost(io_msg, (void *) 0x55);//������Ϣ
	OSIntExit();
}

//�ⲿ�ж���6 7 8���жϷ�����
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line6 | EXTI_Line7 | EXTI_Line8); //����ж���6�����־λ
	OSQPost(io_msg, (void *) 0x55);//������Ϣ
	OSIntExit();
}
//�ⲿ�ж���11���жϷ�����
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
	EXTI_ClearITPendingBit(EXTI_Line11); //����ж���11�����־λ
	OSQPost(io_msg, (void *) 0x55);//������Ϣ
	OSIntExit();
}

























