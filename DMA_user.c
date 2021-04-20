/**
  ******************************************************************************
  * 文件名  main.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.1
  * 日  期  2019-11-16
  * 描  述  该文件提供了基于外设库的最小代码
  *
  *********************************************************************
  */
#include "system_init.h"

unsigned char buf_2[10] ={0x1,0x2 ,0x4 ,0x8 , 0x10,0x20 ,0x40 , 0x80, 0x55,0xAA};
unsigned char buf_3[10] ={0};
unsigned char buf_4[5] ={0};
unsigned char buf_1 =0x1F;
//uint8_t buf_2[64] = {
//		                          0X5A,0X22,0X33,0X44,0X66,0X77,0X88,0X99,0XAA,0XBB,
//								  0X01,0X02,0X03,0X04,0X05,0X06,0X07,0X08,0X09,0X10,
//								  0X11,0X12,0X13,0X14,0X15,0X16,0X17,0X18,0X19,0X20,
//								  0X21,0X22,0X23,0X24,0X25,0X26,0X27,0X28,0X29,0X30,
//								  0X31,0X32,0X33,0X34,0X35,0X36,0X37,0X38,0X39,0X40,
//								  0X41,0X42,0X43,0X44,0X45,0X46,0X47,0X48,0X49,0X50,
//								  0X51,0X52,0X53,0XA5};
//unsigned char buf_3[65] ={0};


#ifndef DMA_C_
#define DMA_C_



void UART2_config_RXDMA(uint8_t len,uint8_t*pbuf)
{
	DMA_InitTypeDef dmaNewStruct;
	/* DMA复位 */
	DMA_Reset (DMA0_SFR);///

//usrt接收DMA配置
	/* 配置 传输数据个数: 65 */
	dmaNewStruct.m_Number = len;
	/* 配置 DMA传输方向：外设到内存 */
	dmaNewStruct.m_Direction = DMA_PERIPHERAL_TO_MEMORY;//DMA_MEMORY_TO_PERIPHERAL;//
	/* 配置 DMA通道优先级：低优先级 */
	dmaNewStruct.m_Priority = DMA_CHANNEL_TOP;
	/* 配置 外设数据位宽：32位宽 */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 存储器数据位宽:32位宽 */
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 外设地址增量模式使能: 使能 */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* 配置 存储器地址增量模式使能: 使能 */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* 配置 DMA通道选择:通道1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_6;
	/* 配置 数据块传输模式： */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//一次触发传输一个数据，
	/* 配置 循环模式使能: 禁止 */
	dmaNewStruct.m_LoopMode = TRUE;  //循环模式，传输计数为0时，再重新从第一个地址开始
	/* 配置 外设起始地址：等待发送的数据的起始地址 */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART2_RBUFR; //UART2寄存器地址

	/* 配置 内存起始地址：接收数据的内存空间的起始地址 */
//	dmaNewStruct.m_MemoryAddr = (uint32_t ) &buf_2;//USART2_TBUFR

	/* 配置 内存起始地址：接收数据的内存空间的起始地址 */
	dmaNewStruct.m_MemoryAddr = (uint32_t )&pbuf;//USART2_TBUFR
	/* 配置DMA功能函数 */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);
	USART_Receive_DMA_INT_Enable(USART2_SFR,TRUE);
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_6, TRUE); //DMA0_ch6对应UART-RX
}

void UART3_TXDMA_config(void)
{
	DMA_InitTypeDef dmaNewStruct;
	/* DMA复位 */
	DMA_Reset (DMA0_SFR);///

	dmaNewStruct.m_Number = 10;
	/* 配置 DMA传输方向：外设到内存 */
	dmaNewStruct.m_Direction = DMA_MEMORY_TO_PERIPHERAL;//DMA_MEMORY_TO_PERIPHERAL;//
	/* 配置 DMA通道优先级：低优先级 */
	dmaNewStruct.m_Priority = DMA_CHANNEL_LOWER;
	/* 配置 外设数据位宽：32/16/8位宽 */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 存储器数据位宽:32/16/8位宽*/
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 外设地址增量模式使能: 使能 */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* 配置 存储器地址增量模式使能: 使能 */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* 配置 DMA通道选择:通道1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_7;
	/* 配置 数据块传输模式： */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//一次触发传输一个数据，
	/* 配置 循环模式使能: 禁止 */
	dmaNewStruct.m_LoopMode = FALSE;  //循环模式，传输计数为0时，再重新从第一个地址开始
	/* 配置 外设起始地址：等待发送的数据的起始地址 */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART3_TBUFR; //外设的寄存器地址

	/* 配置 内存起始地址：接收数据的内存空间的起始地址 */
	dmaNewStruct.m_MemoryAddr = (uint32_t ) buf_2;//
//	dmaNewStruct.m_MemoryAddr = 0x100015c8;//

	/* 配置DMA功能函数 */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);

	USART_Transmit_DMA_INT_Enable(USART3_SFR,TRUE); //使能UART的DMA功能
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_7, TRUE); //DMA0_ch5 对应UART-tx
}

void Init_dma()
{

	DMA_InitTypeDef dmaNewStruct;
	/* DMA功能配置 */

	/* DMA复位 */
	DMA_Reset (DMA0_SFR);///

//usrt接收DMA配置
	/* 配置 传输数据个数: 65 */
	dmaNewStruct.m_Number = 10;
	/* 配置 DMA传输方向：外设到内存 */
	dmaNewStruct.m_Direction = DMA_PERIPHERAL_TO_MEMORY;//DMA_MEMORY_TO_PERIPHERAL;//
	/* 配置 DMA通道优先级：低优先级 */
	dmaNewStruct.m_Priority = DMA_CHANNEL_TOP;
	/* 配置 外设数据位宽：32位宽 */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 存储器数据位宽:32位宽 */
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 外设地址增量模式使能: 使能 */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* 配置 存储器地址增量模式使能: 使能 */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* 配置 DMA通道选择:通道1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_6;
	/* 配置 数据块传输模式： */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//一次触发传输一个数据，
	/* 配置 循环模式使能: 禁止 */
	dmaNewStruct.m_LoopMode = TRUE;  //循环模式，传输计数为0时，再重新从第一个地址开始
	/* 配置 外设起始地址：等待发送的数据的起始地址 */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART2_RBUFR; //UART2寄存器地址

	/* 配置 内存起始地址：接收数据的内存空间的起始地址 */
//	dmaNewStruct.m_MemoryAddr = (uint32_t ) &buf_2;//USART2_TBUFR

	/* 配置 内存起始地址：接收数据的内存空间的起始地址 */
	dmaNewStruct.m_MemoryAddr = (uint32_t ) buf_3;//USART2_TBUFR
	/* 配置DMA功能函数 */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);
	USART_Receive_DMA_INT_Enable(USART2_SFR,TRUE);
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_6, TRUE); //DMA0_ch6对应UART-RX

//usrt发送DMA配置
	/* 配置 传输数据个数: 64 */
	dmaNewStruct.m_Number = 10;
	/* 配置 DMA传输方向：外设到内存 */
	dmaNewStruct.m_Direction = DMA_MEMORY_TO_PERIPHERAL;//DMA_MEMORY_TO_PERIPHERAL;//
	/* 配置 DMA通道优先级：低优先级 */
	dmaNewStruct.m_Priority = DMA_CHANNEL_LOWER;
	/* 配置 外设数据位宽：32/16/8位宽 */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 存储器数据位宽:32/16/8位宽*/
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* 配置 外设地址增量模式使能: 使能 */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* 配置 存储器地址增量模式使能: 使能 */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* 配置 DMA通道选择:通道1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_7;
	/* 配置 数据块传输模式： */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//一次触发传输一个数据，
	/* 配置 循环模式使能: 禁止 */
	dmaNewStruct.m_LoopMode = FALSE;  //循环模式，传输计数为0时，再重新从第一个地址开始
	/* 配置 外设起始地址：等待发送的数据的起始地址 */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART3_TBUFR; //外设的寄存器地址

	/* 配置 内存起始地址：接收数据的内存空间的起始地址 */
	dmaNewStruct.m_MemoryAddr = (uint32_t ) buf_2;//
//	dmaNewStruct.m_MemoryAddr = 0x100015c8;//

	/* 配置DMA功能函数 */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);
	USART_Transmit_DMA_INT_Enable(USART3_SFR,TRUE); //使能UART的DMA功能
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_7, TRUE); //DMA0_ch5 对应UART-tx
}

/**
  * 描述   配置DMA的传输过半中断和传输完成中断
  * 输入   无
  * 返回   无
  */
void cfg_dma_intrupt()
{
//	DMA_Set_INT_Enable(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_HALF_TRANSFER,TRUE); //DMA0_ch3 对应AD0
//	DMA_Clear_INT_Flag(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_HALF_TRANSFER); //DMA0_ch3 对应AD0

	DMA_Set_INT_Enable(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_FINISH_TRANSFER,TRUE); //DMA0_ch3 对应AD0
	DMA_Clear_INT_Flag(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_FINISH_TRANSFER); //DMA0_ch3 对应AD0

//	DMA_Set_INT_Enable(DMA0_SFR, DMA_CHANNEL_5, DMA_INT_FINISH_TRANSFER,TRUE); //DMA0_ch3 对应AD0
//	DMA_Clear_INT_Flag(DMA0_SFR, DMA_CHANNEL_5, DMA_INT_FINISH_TRANSFER); //DMA0_ch3 对应AD0


	INT_Interrupt_Priority_Config(INT_DMA0,7,0);//抢占优先级4,子优先级0
	INT_Interrupt_Enable(INT_DMA0,TRUE);//外设中断使能
	INT_Clear_Interrupt_Flag(INT_DMA0);//清中断标志
}


#endif /* DMA_C_ */
