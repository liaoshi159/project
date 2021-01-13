/**
  ******************************************************************************
  * 文件名  Usart.c
  * 作  者  ChipON_AE/FAE_Group
  * 日  期  2019-10-19
  * 描  述  该文件提供了异步串口相关配置函数，包括
  *          + 串口发送
  *          + 串口异步配置
  *          + 串口同步配置
  *          + 串口接收中断使能
  ******************************************************************************/
#include "KF32F350_Include.h"

void USART_Async_config(USART_SFRmap *USARTx);//串口异步全双工配置
void USART_Sync_config(USART_SFRmap* USARTx); //串口半双工同步配置
void USART_ReceiveInt_config(USART_SFRmap *USARTx,InterruptIndex Peripheral);//串口接收中断使能
void USART_Send(USART_SFRmap* USARTx, uint8_t* Databuf, uint32_t length);//串口发送函数

/**
  * 描述   串口发送
  * 输入   USARTx:   指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR
  *      Databuf：   指向发送数据的指针
  *      length：      发送的长度
  * 返回   无
  */
void USART_Send(USART_SFRmap* USARTx, uint8_t* Databuf, uint32_t length)
{
	uint32_t i;
	for(i=0;i<length;i++)
	{
		//串口发送
		USART_SendData(USARTx,Databuf[i]);
		//发送完成标志
		while(!USART_Get_Transmitter_Empty_Flag(USARTx));
	}
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
	USART_Cmd(USARTx,TRUE);                                    //USARTx使能
}

/**
  * 描述   串口半双工同步配置(默认主模式，9bit发送，9600波特率)
  * 输入   指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR
  * 返回   无
  */
void USART_Sync_config(USART_SFRmap* USARTx)
{
	USART_InitTypeDef USART_InitStructure;

	USART_Struct_Init(&USART_InitStructure);
    USART_InitStructure.m_Mode=USART_MODE_HALFDUPLEXSYN;                        //半双工
    USART_InitStructure.m_HalfDuplexClkSource=USART_MASTER_CLOCKSOURCE_INTER;   //主模式
    USART_InitStructure.m_TransferDir=USART_DIRECTION_TRANSMIT;                 //传输方向"发送"
    USART_InitStructure.m_WordLength=USART_WORDLENGTH_9B;                       //9位数据
    USART_InitStructure.m_Parity=USART_PARITY_ODD;                              //奇校验
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
	USART_Cmd(USARTx,TRUE);                                    //USARTx使能
}


/**
  * 描述   串口接收中断配置
  * 输入   USARTx:指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR
  *      Peripheral:外设或内核中断向量编号，取值范围为：
  *                 枚举类型InterruptIndex中的外设中断向量编号
  * 返回   无
  */
void USART_ReceiveInt_config(USART_SFRmap *USARTx,InterruptIndex Peripheral)
{
	USART_RDR_INT_Enable(USARTx,TRUE);
	INT_Interrupt_Enable(Peripheral,TRUE);
	INT_All_Enable(TRUE);
}
