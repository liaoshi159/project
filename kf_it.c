/****************************************************************************************
 *
 * �ļ���: kf_it.c
 * ��Ŀ��: USART_Async
 * �� ��: v1.0
 * �� ��: 2019��10��12�� 18ʱ49��13��
 * �� ��: ChipON_AE/FAE_Group
 * 
 ****************************************************************************************/
#include"KF32F350_Include.h"
//*****************************************************************************************
//                                 USART0�жϺ���
//*****************************************************************************************	
void __attribute__((interrupt))_USART0_exception (void)
{
	if(USART_Get_Receive_BUFR_Ready_Flag(USART0_SFR))
	{
		if(USART_ReceiveData(USART0_SFR)==0x5A)
		{
			Receive_flag=1;
		}
	}
}

