/****************************************************************************************
 *
 * �ļ���: main.c
 * ��Ŀ��: USART_Async
 * �� ��:  v1.0
 * �� ��:  2019��10��12�� 18ʱ49��13��
 * �� ��:  ChipON_AE/FAE_Group
 * 
 ****************************************************************************************/
#include "sys.h"  
#include "KF32F350_Include.h"
#include "stdio.h"
//ChipON_KF32F350 ASCII
uint8_t USART_Array_Tansmit[100]="ChipON_KF32F350";
volatile uint8_t Receive_flag; //���ձ�־λ

/**
  * ����  GPIOx �����ʼ�����á�
  * ���� : GPIOx: ָ��GPIO�ڴ�ṹ��ָ�룬ȡֵΪGPIOA_SFR~GPIOH_SFR��
  *       GpioPin: �˿��������룬ȡֵΪGPIO_PIN_MASK_0~GPIO_PIN_MASK_15�е�һ��������ϡ�
  * ����  �ޡ�
  */
void GPIOInit_Output_Config(GPIO_SFRmap* GPIOx,uint16_t GpioPin)
{
		/*��ʼ����λGPIOx���裬ʹ��GPIOx����ʱ��*/
			GPIO_Reset(GPIOx);

		/* ���� Pxy��Ϊ���ģʽ���� */
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_Struct_Init(&GPIO_InitStructure);
		GPIO_InitStructure.m_Pin = GpioPin;
		GPIO_InitStructure.m_Speed = GPIO_LOW_SPEED;          //��ʼ�� GPIO����ٶ�
		GPIO_InitStructure.m_Mode = GPIO_MODE_OUT;            //��ʼ�� GPIO����Ϊ���
		GPIO_InitStructure.m_PullUp = GPIO_NOPULL;            //��ʼ�� GPIO�Ƿ�����
		GPIO_InitStructure.m_PullDown = GPIO_NOPULL;          //��ʼ�� GPIO�Ƿ�����
		GPIO_Configuration(GPIOx,&GPIO_InitStructure);

		GPIO_Set_Output_Data_Bits(GPIOx,GpioPin,Bit_SET);					//������Ϊ�ߵ�ƽ
}


void GPIO_USART0();//USART 0������ӳ��
void Usart_line_feed(USART_SFRmap *USARTx);//����
//###############################################################################
//###############################################################################
/**
  * ����   ������
  * ����   ��
  * ����   ��
  */
void main()
{
	volatile float f_test=4.1415925025940;
   /* �û��ɲο�������ʵ��USART0��ȫ˫���첽�շ�����,������9600�����ڽ���
	* ����Ϊ�жϷ�ʽ�����յ�0x5A�����������͡�ChipON_KF32F350����ASCII��*/
	volatile double d_test=4.1415925025940;

//	asm("MOV SP,#__bss_end__+0x400");
	//ϵͳʱ��120M,�����Ƶʱ��16M
	SystemInit();

	//��ʼ��PB8/PB9/PB10����ߵ�ƽ
	GPIOInit_Output_Config(GPIOB_SFR,GPIO_PIN_MASK_8|GPIO_PIN_MASK_9|GPIO_PIN_MASK_10);

	//����USART0������ӳ�䣬PA0��PA1
	GPIO_USART0();
	//ȫ˫���첽8bit 9600������
	USART_Async_config(USART0_SFR);
	//���ڽ����ж�ʹ��
//	USART_ReceiveInt_config(USART0_SFR,INT_USART0);

	Receive_flag=1;
	while(1)
	{
//       if(Receive_flag)
       {
//    	    Receive_flag=1;
    	    //����"ChipON_KF32F350"
            GPIO_Toggle_Output_Data_Config (GPIOB_SFR,GPIO_PIN_MASK_9);
    	    USART_Send(USART0_SFR,USART_Array_Tansmit,(sizeof(USART_Array_Tansmit)-1));
    	    Usart_line_feed(USART0_SFR);

    	    printf("1�ļ�%s:%d:%.6f\r\n",__FUNCTION__,__LINE__,(float)f_test);
    	    fprintf(USART0_STREAM,"�ļ�\r\n");
    	    printf("2�ļ�%s:%d:%8.2f\r\n",__FUNCTION__,__LINE__,(float)d_test);

    	    fprintf(USART0_STREAM,"abcdefghijklmnopqrst1234567890\r\nC1CC\r\n");
    	    printf("�ļ�AAAAAAAAAAAAAA\r\n");
    	    puts("ABBB\t\r\n");
    	    fputs("ABBB\t\r\n",USART0_STREAM);

    	    sprintf(USART_Array_Tansmit,"3��%c��%s:%d:%x:%.10f\r\n",'A',__FUNCTION__,__LINE__,__LINE__,d_test);
    	    fprintf(USART0_STREAM,USART_Array_Tansmit);
       }
	}
	while(1);
}

/**
  * ����   USART0������ӳ��
  * ����   ��
  * ����   ��
  */
void GPIO_USART0()
{
	/* �˿���ӳ��AF5 */
	//USART0_RX		PA0
	//USART0_TX0	PA1
	GPIO_Write_Mode_Bits(GPIOA_SFR ,GPIO_PIN_MASK_0, GPIO_MODE_RMP);           //��ӳ��IO�ڹ���ģʽ
	GPIO_Write_Mode_Bits(GPIOA_SFR ,GPIO_PIN_MASK_1, GPIO_MODE_RMP);           //��ӳ��IO�ڹ���ģʽ
	GPIO_Pin_RMP_Config (GPIOA_SFR, GPIO_Pin_Num_0, GPIO_RMP_AF5_USART0);	   //��ӳ��ΪUSART0
	GPIO_Pin_RMP_Config (GPIOA_SFR,GPIO_Pin_Num_1, GPIO_RMP_AF5_USART0);       //��ӳ��ΪUSART0
	GPIO_Pin_Lock_Config (GPIOA_SFR ,GPIO_PIN_MASK_0, TRUE);                   //��������
	GPIO_Pin_Lock_Config (GPIOA_SFR ,GPIO_PIN_MASK_1, TRUE);                   //��������
}
/**
  * ����   ���ڷ����ͻ��з�
  * ����   ָ��USART�ڴ�ṹ��ָ�룬ȡֵΪUSART0_SFR~USART8_SFR
  * ����   ��
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
  * ����   ����У�鷢��������ļ�����
  * ����   file: ָ���ļ����Ƶ�ָ��
  *      line�� У��������
  * ����  �ޡ�
  */
void check_failed(uint8_t* file, uint32_t line)
{
	  /* �û���������Լ��Ĵ���ʵ�ֱ����ļ������к�,
	             ����: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	  while(1)
	  {
		  ;
	  }
}



