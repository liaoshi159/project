/*
 * uTask.c
 *
 *  Created on: 2020-4-4
 *      Author: 陆泽霖
 */
#include "system_init.h"
#include "malloc.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"

#include "uTask.h"

/*------------------LED任务定义--------------------*/
static void UART_DMA_task(void *pvParameters);


uint8_t USART_Array_Tansmit[]="ChipON_KF32LS200";

/**
  * 描述   串口IO重映射，PB2做uart3_rx,PA3做uart3_tx
  * 输入   无
  * 返回   无
  */
void GPIO_USART()
{
	GPIO_Write_Mode_Bits(GPIOB_SFR ,GPIO_PIN_MASK_14, GPIO_MODE_RMP);           //重映射IO口功能模式
	GPIO_Write_Mode_Bits(GPIOB_SFR ,GPIO_PIN_MASK_15, GPIO_MODE_RMP);           //重映射IO口功能模式
	GPIO_Pin_RMP_Config (GPIOB_SFR, GPIO_Pin_Num_14, GPIO_RMP_AF5_USART2);	   //重映射为USART3
	GPIO_Pin_RMP_Config (GPIOB_SFR,	GPIO_Pin_Num_15, GPIO_RMP_AF5_USART2);       //重映射为USART3
	GPIO_Pin_Lock_Config (GPIOB_SFR ,GPIO_PIN_MASK_14, TRUE);                   //配置锁存
	GPIO_Pin_Lock_Config (GPIOB_SFR ,GPIO_PIN_MASK_15, TRUE);                   //配置锁存
}

/**
  * 描述  串口异步全双工配置(默认8bit收发使能  全双工 9600)
  * 输入   指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR
  * 返回   无
  */
void USART_Async_config(USART_SFRmap *USARTx)
{
	USART_InitTypeDef USART_InitStructure;

	USART_Struct_Init(&USART_InitStructure);
    USART_InitStructure.m_Mode=USART_MODE_FULLDUPLEXASY;                        //全双工
    USART_InitStructure.m_TransferDir=USART_DIRECTION_FULL_DUPLEX;              //传输方向
    USART_InitStructure.m_WordLength=USART_WORDLENGTH_8B;                       //8位数据
    USART_InitStructure.m_StopBits=USART_STOPBITS_1;                            //1位停止位
    USART_InitStructure.m_BaudRateBRCKS=USART_CLK_HFCLK;                        //内部高频时钟作为 USART波特率发生器时钟

    /* 波特率 =Fck/(16*z（1+x/y)) 外设时钟内部高频16M*/
    //4800    z:208    x:0    y:0
    //9600    z:104    x:0    y:0
    //19200   z:52     x:0    y:0
    //115200  z:8      x:1    y:15
    //波特率9600
    USART_InitStructure.m_BaudRateInteger=104;         //USART波特率整数部分z，取值为0~65535
    USART_InitStructure.m_BaudRateNumerator=0;         //USART波特率小数分子部分x，取值为0~0xF
    USART_InitStructure.m_BaudRateDenominator=0;       //USART波特率小数分母部分y，取值为0~0xF

	USART_Reset(USARTx);                                       //USARTx复位
	USART_Configuration(USARTx,&USART_InitStructure);          //USARTx配置
	USART_Clear_Transmit_BUFR_INT_Flag(USARTx);                //USARTx发送BUF清零
	USART2_CTLR |= 0x01 <<30; //全双工必须值1
}

//任务句柄
TaskHandle_t UARTDMATask_Handler;

void taskInit()
{
	//创建任务
	xTaskCreate((TaskFunction_t )UART_DMA_task,
				(const char*    )"UART_DMA_task",
				(uint16_t       )256,
				(void*          )NULL,
				(UBaseType_t    )2,
				(TaskHandle_t*  )&UARTDMATask_Handler);
}






/*任务定义--------------------------------------*/
void UART_DMA_task(void *pvParameters)
{

	/*本例程演示uart配合DMA做数据收发的使用方法，注意在发送的第一个字节是误码，应用程序需要注意这一点。
	 *PC机的串口助手发送10个字符，uart2的接收DMA完成中断将会触发，将内容+1后通过uart——DMA再发送出来。
	 *main函数翻转PF0的状态
	 */
	GPIO_Write_Mode_Bits(GPIOB_SFR, GPIO_PIN_MASK_14,GPIO_MODE_OUT);  //STATUS灯配置为输出模式
	GPIO_Write_Mode_Bits(GPIOB_SFR, GPIO_PIN_MASK_15,GPIO_MODE_OUT);  //STATUS灯配置为输出模式
	unsigned char i;

	Init_dma(); //dma配置
	cfg_dma_intrupt(); //配置dma的中断
//	USART_Transmit_Data_Enable(USART2_SFR,ENABLE);
	USART_Receive_Data_Enable(USART2_SFR,ENABLE);
	USART_Cmd(USART2_SFR,TRUE);   //USART2使能
	USART_Cmd(USART3_SFR,TRUE);   //USART2使能

	USART_Transmit_Data_Enable(USART3_SFR,ENABLE);
	INT_All_Enable (TRUE);//全局可屏蔽中断使能,该中断使能控制不包含复位/NMI/硬件错误中断
	USART_SendData(USART3_SFR,0XAA); //使用uart对应的DMA发送功能，必须通过发送一个字符的方式激活uart-DMA通道


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
//			USART_Transmit_DMA_INT_Enable(USART3_SFR,TRUE); //使能UART的DMA功能
    	   DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_7, TRUE); //DMA0_ch5 服务于uart2——tx
//			Init_dma();
    	   GPIO_Toggle_Output_Data_Config(GPIOB_SFR,GPIO_PIN_MASK_15);
//    	   UART2_config_RXDMA(5,buf_4);
       }
//		DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_5, TRUE); //DMA0_ch5 服务于uart2——tx
//		MY_MSG("任务测试\r\n");
		vTaskDelay(100);
    	GPIO_Toggle_Output_Data_Config(GPIOB_SFR,GPIO_PIN_MASK_14);
    }
}


//****************************************************************************
//						OS Hook
//****************************************************************************
//任务栈溢出钩子
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
//	MY_MSG("vApplicationStackOverflowHook: %s over: %d \r\n", pcTaskName, uxTaskGetStackHighWaterMark(xTask));
	while (1)
		;
}

//任务内存分配错误钩子
void vApplicationMallocFailedHook(void)
{
	MY_MSG("vApplicationMallocFailedHook\r\n");
	while (1)
		;
}
