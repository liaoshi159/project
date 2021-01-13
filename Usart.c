/**
  ******************************************************************************
  * �ļ���  Usart.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  2019-10-19
  * ��  ��  ���ļ��ṩ���첽����������ú���������
  *          + ���ڷ���
  *          + �����첽����
  *          + ����ͬ������
  *          + ���ڽ����ж�ʹ��
  ******************************************************************************/
#include "KF32F350_Include.h"

void USART_Async_config(USART_SFRmap *USARTx);//�����첽ȫ˫������
void USART_Sync_config(USART_SFRmap* USARTx); //���ڰ�˫��ͬ������
void USART_ReceiveInt_config(USART_SFRmap *USARTx,InterruptIndex Peripheral);//���ڽ����ж�ʹ��
void USART_Send(USART_SFRmap* USARTx, uint8_t* Databuf, uint32_t length);//���ڷ��ͺ���

/**
  * ����   ���ڷ���
  * ����   USARTx:   ָ��USART�ڴ�ṹ��ָ�룬ȡֵΪUSART0_SFR~USART8_SFR
  *      Databuf��   ָ�������ݵ�ָ��
  *      length��      ���͵ĳ���
  * ����   ��
  */
void USART_Send(USART_SFRmap* USARTx, uint8_t* Databuf, uint32_t length)
{
	uint32_t i;
	for(i=0;i<length;i++)
	{
		//���ڷ���
		USART_SendData(USARTx,Databuf[i]);
		//������ɱ�־
		while(!USART_Get_Transmitter_Empty_Flag(USARTx));
	}
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
	USART_Cmd(USARTx,TRUE);                                    //USARTxʹ��
}

/**
  * ����   ���ڰ�˫��ͬ������(Ĭ����ģʽ��9bit���ͣ�9600������)
  * ����   ָ��USART�ڴ�ṹ��ָ�룬ȡֵΪUSART0_SFR~USART8_SFR
  * ����   ��
  */
void USART_Sync_config(USART_SFRmap* USARTx)
{
	USART_InitTypeDef USART_InitStructure;

	USART_Struct_Init(&USART_InitStructure);
    USART_InitStructure.m_Mode=USART_MODE_HALFDUPLEXSYN;                        //��˫��
    USART_InitStructure.m_HalfDuplexClkSource=USART_MASTER_CLOCKSOURCE_INTER;   //��ģʽ
    USART_InitStructure.m_TransferDir=USART_DIRECTION_TRANSMIT;                 //���䷽��"����"
    USART_InitStructure.m_WordLength=USART_WORDLENGTH_9B;                       //9λ����
    USART_InitStructure.m_Parity=USART_PARITY_ODD;                              //��У��
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
	USART_Cmd(USARTx,TRUE);                                    //USARTxʹ��
}


/**
  * ����   ���ڽ����ж�����
  * ����   USARTx:ָ��USART�ڴ�ṹ��ָ�룬ȡֵΪUSART0_SFR~USART8_SFR
  *      Peripheral:������ں��ж�������ţ�ȡֵ��ΧΪ��
  *                 ö������InterruptIndex�е������ж��������
  * ����   ��
  */
void USART_ReceiveInt_config(USART_SFRmap *USARTx,InterruptIndex Peripheral)
{
	USART_RDR_INT_Enable(USARTx,TRUE);
	INT_Interrupt_Enable(Peripheral,TRUE);
	INT_All_Enable(TRUE);
}
