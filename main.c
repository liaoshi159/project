/****************************************************************************************
 *
 * 文件名: main.c
 * 项目名: USART_Async
 * 版 本:  v1.0
 * 日 期:  2019年10月12日 18时49分13秒
 * 作 者:  ChipON_AE/FAE_Group
 * 
 ****************************************************************************************/
#include "sys.h"  
#include "KF32F350_Include.h"
#include "stdio.h"
//ChipON_KF32F350 ASCII
uint8_t USART_Array_Tansmit[100]="ChipON_KF32F350";
volatile uint8_t Receive_flag; //接收标志位

/**
  * 描述  GPIOx 输出初始化配置。
  * 输入 : GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的一个或多个组合。
  * 返回  无。
  */
void GPIOInit_Output_Config(GPIO_SFRmap* GPIOx,uint16_t GpioPin)
{
		/*初始化复位GPIOx外设，使能GPIOx外设时钟*/
			GPIO_Reset(GPIOx);

		/* 配置 Pxy作为输出模式参数 */
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_Struct_Init(&GPIO_InitStructure);
		GPIO_InitStructure.m_Pin = GpioPin;
		GPIO_InitStructure.m_Speed = GPIO_LOW_SPEED;          //初始化 GPIO输出速度
		GPIO_InitStructure.m_Mode = GPIO_MODE_OUT;            //初始化 GPIO方向为输出
		GPIO_InitStructure.m_PullUp = GPIO_NOPULL;            //初始化 GPIO是否上拉
		GPIO_InitStructure.m_PullDown = GPIO_NOPULL;          //初始化 GPIO是否下拉
		GPIO_Configuration(GPIOx,&GPIO_InitStructure);

		GPIO_Set_Output_Data_Bits(GPIOx,GpioPin,Bit_SET);					//先设置为高电平
}


void GPIO_USART0();//USART 0引脚重映射
void Usart_line_feed(USART_SFRmap *USARTx);//换行
//###############################################################################
//###############################################################################
/**
  * 描述   主函数
  * 输入   无
  * 返回   无
  */
void main()
{
	volatile float f_test=4.1415925025940;
   /* 用户可参考该例程实现USART0的全双工异步收发功能,波特率9600，串口接收
	* 配置为中断方式，接收到0x5A后，主函数发送“ChipON_KF32F350”的ASCII码*/
	volatile double d_test=4.1415925025940;

//	asm("MOV SP,#__bss_end__+0x400");
	//系统时钟120M,外设高频时钟16M
	SystemInit();

	//初始化PB8/PB9/PB10输出高电平
	GPIOInit_Output_Config(GPIOB_SFR,GPIO_PIN_MASK_8|GPIO_PIN_MASK_9|GPIO_PIN_MASK_10);

	//配置USART0引脚重映射，PA0，PA1
	GPIO_USART0();
	//全双工异步8bit 9600波特率
	USART_Async_config(USART0_SFR);
	//串口接收中断使能
//	USART_ReceiveInt_config(USART0_SFR,INT_USART0);

	Receive_flag=1;
	while(1)
	{
//       if(Receive_flag)
       {
//    	    Receive_flag=1;
    	    //发送"ChipON_KF32F350"
            GPIO_Toggle_Output_Data_Config (GPIOB_SFR,GPIO_PIN_MASK_9);
    	    USART_Send(USART0_SFR,USART_Array_Tansmit,(sizeof(USART_Array_Tansmit)-1));
    	    Usart_line_feed(USART0_SFR);

    	    printf("1文件%s:%d:%.6f\r\n",__FUNCTION__,__LINE__,(float)f_test);
    	    fprintf(USART0_STREAM,"文件\r\n");
    	    printf("2文件%s:%d:%8.2f\r\n",__FUNCTION__,__LINE__,(float)d_test);

    	    fprintf(USART0_STREAM,"abcdefghijklmnopqrst1234567890\r\nC1CC\r\n");
    	    printf("文件AAAAAAAAAAAAAA\r\n");
    	    puts("ABBB\t\r\n");
    	    fputs("ABBB\t\r\n",USART0_STREAM);

    	    sprintf(USART_Array_Tansmit,"3文%c件%s:%d:%x:%.10f\r\n",'A',__FUNCTION__,__LINE__,__LINE__,d_test);
    	    fprintf(USART0_STREAM,USART_Array_Tansmit);
       }
	}
	while(1);
}

/**
  * 描述   USART0引脚重映射
  * 输入   无
  * 返回   无
  */
void GPIO_USART0()
{
	/* 端口重映射AF5 */
	//USART0_RX		PA0
	//USART0_TX0	PA1
	GPIO_Write_Mode_Bits(GPIOA_SFR ,GPIO_PIN_MASK_0, GPIO_MODE_RMP);           //重映射IO口功能模式
	GPIO_Write_Mode_Bits(GPIOA_SFR ,GPIO_PIN_MASK_1, GPIO_MODE_RMP);           //重映射IO口功能模式
	GPIO_Pin_RMP_Config (GPIOA_SFR, GPIO_Pin_Num_0, GPIO_RMP_AF5_USART0);	   //重映射为USART0
	GPIO_Pin_RMP_Config (GPIOA_SFR,GPIO_Pin_Num_1, GPIO_RMP_AF5_USART0);       //重映射为USART0
	GPIO_Pin_Lock_Config (GPIOA_SFR ,GPIO_PIN_MASK_0, TRUE);                   //配置锁存
	GPIO_Pin_Lock_Config (GPIOA_SFR ,GPIO_PIN_MASK_1, TRUE);                   //配置锁存
}
/**
  * 描述   串口发闪送换行符
  * 输入   指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR
  * 返回   无
  */
void Usart_line_feed(USART_SFRmap *USARTx)
{
	USART_SendData(USARTx,0x0D);
	while(USART_Get_Transmitter_Empty_Flag(USARTx));
	while(!USART_Get_Transmitter_Empty_Flag(USARTx));
	USART_SendData(USARTx,0x0A);
	while(USART_Get_Transmitter_Empty_Flag(USARTx));
	while(!USART_Get_Transmitter_Empty_Flag(USARTx));
}

/**
  * 描述   报告校验发生错误的文件和行
  * 输入   file: 指向文件名称的指针
  *      line： 校验错误的行
  * 返回  无。
  */
void check_failed(uint8_t* file, uint32_t line)
{
	  /* 用户可以添加自己的代码实现报告文件名和行号,
	             例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	  while(1)
	  {
		  ;
	  }
}



