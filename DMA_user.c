/**
  ******************************************************************************
  * �ļ���  main.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  V2.1
  * ��  ��  2019-11-16
  * ��  ��  ���ļ��ṩ�˻�����������С����
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
	/* DMA��λ */
	DMA_Reset (DMA0_SFR);///

//usrt����DMA����
	/* ���� �������ݸ���: 65 */
	dmaNewStruct.m_Number = len;
	/* ���� DMA���䷽�����赽�ڴ� */
	dmaNewStruct.m_Direction = DMA_PERIPHERAL_TO_MEMORY;//DMA_MEMORY_TO_PERIPHERAL;//
	/* ���� DMAͨ�����ȼ��������ȼ� */
	dmaNewStruct.m_Priority = DMA_CHANNEL_TOP;
	/* ���� ��������λ��32λ�� */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �洢������λ��:32λ�� */
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* ���� �洢����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* ���� DMAͨ��ѡ��:ͨ��1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_6;
	/* ���� ���ݿ鴫��ģʽ�� */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//һ�δ�������һ�����ݣ�
	/* ���� ѭ��ģʽʹ��: ��ֹ */
	dmaNewStruct.m_LoopMode = TRUE;  //ѭ��ģʽ���������Ϊ0ʱ�������´ӵ�һ����ַ��ʼ
	/* ���� ������ʼ��ַ���ȴ����͵����ݵ���ʼ��ַ */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART2_RBUFR; //UART2�Ĵ�����ַ

	/* ���� �ڴ���ʼ��ַ���������ݵ��ڴ�ռ����ʼ��ַ */
//	dmaNewStruct.m_MemoryAddr = (uint32_t ) &buf_2;//USART2_TBUFR

	/* ���� �ڴ���ʼ��ַ���������ݵ��ڴ�ռ����ʼ��ַ */
	dmaNewStruct.m_MemoryAddr = (uint32_t )&pbuf;//USART2_TBUFR
	/* ����DMA���ܺ��� */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);
	USART_Receive_DMA_INT_Enable(USART2_SFR,TRUE);
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_6, TRUE); //DMA0_ch6��ӦUART-RX
}

void UART3_TXDMA_config(void)
{
	DMA_InitTypeDef dmaNewStruct;
	/* DMA��λ */
	DMA_Reset (DMA0_SFR);///

	dmaNewStruct.m_Number = 10;
	/* ���� DMA���䷽�����赽�ڴ� */
	dmaNewStruct.m_Direction = DMA_MEMORY_TO_PERIPHERAL;//DMA_MEMORY_TO_PERIPHERAL;//
	/* ���� DMAͨ�����ȼ��������ȼ� */
	dmaNewStruct.m_Priority = DMA_CHANNEL_LOWER;
	/* ���� ��������λ��32/16/8λ�� */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �洢������λ��:32/16/8λ��*/
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* ���� �洢����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* ���� DMAͨ��ѡ��:ͨ��1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_7;
	/* ���� ���ݿ鴫��ģʽ�� */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//һ�δ�������һ�����ݣ�
	/* ���� ѭ��ģʽʹ��: ��ֹ */
	dmaNewStruct.m_LoopMode = FALSE;  //ѭ��ģʽ���������Ϊ0ʱ�������´ӵ�һ����ַ��ʼ
	/* ���� ������ʼ��ַ���ȴ����͵����ݵ���ʼ��ַ */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART3_TBUFR; //����ļĴ�����ַ

	/* ���� �ڴ���ʼ��ַ���������ݵ��ڴ�ռ����ʼ��ַ */
	dmaNewStruct.m_MemoryAddr = (uint32_t ) buf_2;//
//	dmaNewStruct.m_MemoryAddr = 0x100015c8;//

	/* ����DMA���ܺ��� */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);

	USART_Transmit_DMA_INT_Enable(USART3_SFR,TRUE); //ʹ��UART��DMA����
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_7, TRUE); //DMA0_ch5 ��ӦUART-tx
}

void Init_dma()
{

	DMA_InitTypeDef dmaNewStruct;
	/* DMA�������� */

	/* DMA��λ */
	DMA_Reset (DMA0_SFR);///

//usrt����DMA����
	/* ���� �������ݸ���: 65 */
	dmaNewStruct.m_Number = 10;
	/* ���� DMA���䷽�����赽�ڴ� */
	dmaNewStruct.m_Direction = DMA_PERIPHERAL_TO_MEMORY;//DMA_MEMORY_TO_PERIPHERAL;//
	/* ���� DMAͨ�����ȼ��������ȼ� */
	dmaNewStruct.m_Priority = DMA_CHANNEL_TOP;
	/* ���� ��������λ��32λ�� */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �洢������λ��:32λ�� */
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* ���� �洢����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* ���� DMAͨ��ѡ��:ͨ��1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_6;
	/* ���� ���ݿ鴫��ģʽ�� */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//һ�δ�������һ�����ݣ�
	/* ���� ѭ��ģʽʹ��: ��ֹ */
	dmaNewStruct.m_LoopMode = TRUE;  //ѭ��ģʽ���������Ϊ0ʱ�������´ӵ�һ����ַ��ʼ
	/* ���� ������ʼ��ַ���ȴ����͵����ݵ���ʼ��ַ */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART2_RBUFR; //UART2�Ĵ�����ַ

	/* ���� �ڴ���ʼ��ַ���������ݵ��ڴ�ռ����ʼ��ַ */
//	dmaNewStruct.m_MemoryAddr = (uint32_t ) &buf_2;//USART2_TBUFR

	/* ���� �ڴ���ʼ��ַ���������ݵ��ڴ�ռ����ʼ��ַ */
	dmaNewStruct.m_MemoryAddr = (uint32_t ) buf_3;//USART2_TBUFR
	/* ����DMA���ܺ��� */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);
	USART_Receive_DMA_INT_Enable(USART2_SFR,TRUE);
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_6, TRUE); //DMA0_ch6��ӦUART-RX

//usrt����DMA����
	/* ���� �������ݸ���: 64 */
	dmaNewStruct.m_Number = 10;
	/* ���� DMA���䷽�����赽�ڴ� */
	dmaNewStruct.m_Direction = DMA_MEMORY_TO_PERIPHERAL;//DMA_MEMORY_TO_PERIPHERAL;//
	/* ���� DMAͨ�����ȼ��������ȼ� */
	dmaNewStruct.m_Priority = DMA_CHANNEL_LOWER;
	/* ���� ��������λ��32/16/8λ�� */
	dmaNewStruct.m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �洢������λ��:32/16/8λ��*/
	dmaNewStruct.m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
	/* ���� �����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_PeripheralInc = FALSE;
	/* ���� �洢����ַ����ģʽʹ��: ʹ�� */
	dmaNewStruct.m_MemoryInc = TRUE;
	/* ���� DMAͨ��ѡ��:ͨ��1 */
	dmaNewStruct.m_Channel = DMA_CHANNEL_7;
	/* ���� ���ݿ鴫��ģʽ�� */
	dmaNewStruct.m_BlockMode = DMA_TRANSFER_BYTE;//һ�δ�������һ�����ݣ�
	/* ���� ѭ��ģʽʹ��: ��ֹ */
	dmaNewStruct.m_LoopMode = FALSE;  //ѭ��ģʽ���������Ϊ0ʱ�������´ӵ�һ����ַ��ʼ
	/* ���� ������ʼ��ַ���ȴ����͵����ݵ���ʼ��ַ */
	dmaNewStruct.m_PeriphAddr = (uint32_t) &USART3_TBUFR; //����ļĴ�����ַ

	/* ���� �ڴ���ʼ��ַ���������ݵ��ڴ�ռ����ʼ��ַ */
	dmaNewStruct.m_MemoryAddr = (uint32_t ) buf_2;//
//	dmaNewStruct.m_MemoryAddr = 0x100015c8;//

	/* ����DMA���ܺ��� */
	DMA_Configuration (DMA0_SFR, &dmaNewStruct);
	USART_Transmit_DMA_INT_Enable(USART3_SFR,TRUE); //ʹ��UART��DMA����
	DMA_Channel_Enable (DMA0_SFR, DMA_CHANNEL_7, TRUE); //DMA0_ch5 ��ӦUART-tx
}

/**
  * ����   ����DMA�Ĵ�������жϺʹ�������ж�
  * ����   ��
  * ����   ��
  */
void cfg_dma_intrupt()
{
//	DMA_Set_INT_Enable(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_HALF_TRANSFER,TRUE); //DMA0_ch3 ��ӦAD0
//	DMA_Clear_INT_Flag(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_HALF_TRANSFER); //DMA0_ch3 ��ӦAD0

	DMA_Set_INT_Enable(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_FINISH_TRANSFER,TRUE); //DMA0_ch3 ��ӦAD0
	DMA_Clear_INT_Flag(DMA0_SFR, DMA_CHANNEL_6, DMA_INT_FINISH_TRANSFER); //DMA0_ch3 ��ӦAD0

//	DMA_Set_INT_Enable(DMA0_SFR, DMA_CHANNEL_5, DMA_INT_FINISH_TRANSFER,TRUE); //DMA0_ch3 ��ӦAD0
//	DMA_Clear_INT_Flag(DMA0_SFR, DMA_CHANNEL_5, DMA_INT_FINISH_TRANSFER); //DMA0_ch3 ��ӦAD0


	INT_Interrupt_Priority_Config(INT_DMA0,7,0);//��ռ���ȼ�4,�����ȼ�0
	INT_Interrupt_Enable(INT_DMA0,TRUE);//�����ж�ʹ��
	INT_Clear_Interrupt_Flag(INT_DMA0);//���жϱ�־
}


#endif /* DMA_C_ */
