/*
 * uTask.c
 *
 *  Created on: 2020-4-4
 *      Author: ½����
 */
#include "system_init.h"
#include "malloc.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

#include "uTask.h"

/*------------------LED������--------------------*/
static void UART_DMA_task(void *pvParameters);


uint8_t USART_Array_Tansmit[]="ChipON_KF32LS200";

/**
  * ����   ����IO��ӳ�䣬PB2��uart3_rx,PA3��uart3_tx
  * ����   ��
  * ����   ��
  */
void GPIO_USART()
{
	GPIO_Write_Mode_Bits(GPIOB_SFR ,GPIO_PIN_MASK_14, GPIO_MODE_RMP);           //��ӳ��IO�ڹ���ģʽ
	GPIO_Write_Mode_Bits(GPIOB_SFR ,GPIO_PIN_MASK_15, GPIO_MODE_RMP);           //��ӳ��IO�ڹ���ģʽ
	GPIO_Pin_RMP_Config (GPIOB_SFR, GPIO_Pin_Num_14, GPIO_RMP_AF5_USART2);	   //��ӳ��ΪUSART3
	GPIO_Pin_RMP_Config (GPIOB_SFR,	GPIO_Pin_Num_15, GPIO_RMP_AF5_USART2);       //��ӳ��ΪUSART3
	GPIO_Pin_Lock_Config (GPIOB_SFR ,GPIO_PIN_MASK_14, TRUE);                   //��������
	GPIO_Pin_Lock_Config (GPIOB_SFR ,GPIO_PIN_MASK_15, TRUE);                   //��������
}

/**
  * ����  �����첽ȫ˫������(Ĭ��8bit�շ�ʹ��  ȫ˫�� 9600)
  * ����   ָ��USART�ڴ�ṹ��ָ�룬ȡֵΪUSART0_SFR~USART8_SFR
  * ����   ��
  */
void USART_Async_config(USART_SFRmap *USARTx)
{
	USART_InitTypeDef USART_InitStructure;

	USART_Struct_Init(&USART_InitStructure);
    USART_InitStructure.m_Mode=USART_MODE_FULLDUPLEXASY;                        //ȫ˫��
    USART_InitStructure.m_TransferDir=USART_DIRECTION_FULL_DUPLEX;              //���䷽��
    USART_InitStructure.m_WordLength=USART_WORDLENGTH_8B;                       //8λ����
    USART_InitStructure.m_StopBits=USART_STOPBITS_1;                            //1λֹͣλ
    USART_InitStructure.m_BaudRateBRCKS=USART_CLK_HFCLK;                        //�ڲ���Ƶʱ����Ϊ USART�����ʷ�����ʱ��

    /* ������ =Fck/(16*z��1+x/y)) ����ʱ���ڲ���Ƶ16M*/
    //4800    z:208    x:0    y:0
    //9600    z:104    x:0    y:0
    //19200   z:52     x:0    y:0
    //115200  z:8      x:1    y:15
    //������9600
    USART_InitStructure.m_BaudRateInteger=104;         //USART��������������z��ȡֵΪ0~65535
    USART_InitStructure.m_BaudRateNumerator=0;         //USART������С�����Ӳ���x��ȡֵΪ0~0xF
    USART_InitStructure.m_BaudRateDenominator=0;       //USART������С����ĸ����y��ȡֵΪ0~0xF

	USART_Reset(USARTx);                                       //USARTx��λ
	USART_Configuration(USARTx,&USART_InitStructure);          //USARTx����
	USART_Clear_Transmit_BUFR_INT_Flag(USARTx);                //USARTx����BUF����
	USART2_CTLR |= 0x01 <<30; //ȫ˫������ֵ1
}

//������
TaskHandle_t UARTDMATask_Handler;

void taskInit()
{
	//��������
	xTaskCreate((TaskFunction_t )UART_DMA_task,
				(const char*    )"UART_DMA_task",
				(uint16_t       )256,
				(void*          )NULL,
				(UBaseType_t    )2,
				(TaskHandle_t*  )&UARTDMATask_Handler);
}






/*������--------------------------------------*/
void UART_DMA_task(void *pvParameters)
{

	/*��������ʾuart���DMA�������շ���ʹ�÷�����ע���ڷ��͵ĵ�һ���ֽ������룬Ӧ�ó�����Ҫע����һ�㡣
	 *PC���Ĵ������ַ���10���ַ���uart2�Ľ���DMA����жϽ��ᴥ����������+1��ͨ��uart����DMA�ٷ��ͳ�����
	 *main������תPF0��״̬
	 */
	GPIO_Write_Mode_Bits(GPIOB_SFR, GPIO_PIN_MASK_14,GPIO_MODE_OUT);  //STATUS������Ϊ���ģʽ
	GPIO_Write_Mode_Bits(GPIOB_SFR, GPIO_PIN_MASK_15,GPIO_MODE_OUT);  //STATUS������Ϊ���ģʽ
	unsigned char i;

	Init_dma(); //dma����
	cfg_dma_intrupt(); //����dma���ж�
//	USART_Transmit_Data_Enable(USART2_SFR,ENABLE);
	USART_Receive_Data_Enable(USART2_SFR,ENABLE);
	USART_Cmd(USART2_SFR,TRUE);   //USART2ʹ��
	USART_Cmd(USART3_SFR,TRUE);   //USART2ʹ��

	USART_Transmit_Data_Enable(USART3_SFR,ENABLE);
	INT_All_Enable (TRUE);//ȫ�ֿ������ж�ʹ��,���ж�ʹ�ܿ��Ʋ�������λ/NMI/Ӳ�������ж�
	USART_SendData(USART3_SFR,0XAA); //ʹ��uart��Ӧ��DMA���͹��ܣ�����ͨ������һ���ַ��ķ�ʽ����uart-DMAͨ��


	while(1)
	{
       if(Buf_full_flag)
       {
    	   Buf_full_flag =0;
			for(i =0; i<10 ;i++)
			{
				buf_2[i] =  buf_3[i]+1;

			}


//			DMA_Clear_INT_Flag(DMA0_SFR, DMA_CHANNEL_7, DMA_INT_FINISH_TRANSFER);
//			USART_Transmit_DMA_INT_Enable(USART3_SFR,TRUE); //ʹ��UART��DMA����
    	   DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_7, TRUE); //DMA0_ch5 ������uart2����tx
//			Init_dma();
    	   GPIO_Toggle_Output_Data_Config(GPIOB_SFR,GPIO_PIN_MASK_15);
//    	   UART2_config_RXDMA(5,buf_4);
       }
//		DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_5, TRUE); //DMA0_ch5 ������uart2����tx
//		MY_MSG("�������\r\n");
		vTaskDelay(100);
    	GPIO_Toggle_Output_Data_Config(GPIOB_SFR,GPIO_PIN_MASK_14);
    }
}


//****************************************************************************
//						OS Hook
//****************************************************************************
//����ջ�������
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
//	MY_MSG("vApplicationStackOverflowHook: %s over: %d \r\n", pcTaskName, uxTaskGetStackHighWaterMark(xTask));
	while (1)
		;
}

//�����ڴ���������
void vApplicationMallocFailedHook(void)
{
	MY_MSG("vApplicationMallocFailedHook\r\n");
	while (1)
		;
}
