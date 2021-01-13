/**
  ******************************************************************************
  * �ļ���  sys.c
  * ��  ��  ChipON_AE/FAE_Group
  * ��  ��  2019-10-10
  * ��  ��  ���ļ��ṩ��ϵͳʱ�ӳ�ʼ����������
  *          + ʱ������
  *          + GPIO����
  ******************************************************************************
  */
#include "sys.h"
/**
  * ����  ʱ��Ĭ�����á�
  * ����  �ޡ�
  * ����  �ޡ�
*/
static void SetSysClock(void)
{
	OSC_INTHF_Software_Enable(TRUE);
	while(OSC_Get_INTHF_INT_Flag() != SET);
	/* PLLʱ��Դѡ�� */
#ifdef SYSCLK_FREQ_HSI
	OSC_PLL_Input_Source_Config(PLL_INPUT_INTHF);
#else
	OSC_PLL_Input_Source_Config(PLL_INPUT_EXTHF);
#endif
	/* PLL��Ƶʱ��ѡ�� */
#ifdef SYSCLK_FREQ_48MHz
	OSC_PLL_Multiple_Value_Select(48,2,8);
#elif defined SYSCLK_FREQ_72MHz
	OSC_PLL_Multiple_Value_Select(36,2,4);
#elif defined SYSCLK_FREQ_96MHz
	OSC_PLL_Multiple_Value_Select(12,1,2);
#elif defined SYSCLK_FREQ_120MHz
	OSC_PLL_Multiple_Value_Select(30,2,2);
#elif defined SYSCLK_FREQ_150MHz
	OSC_PLL_Multiple_Value_Select(150,8,2);
#endif
	OSC_PLL_Start_Delay_Config(PLL_START_DELAY_1024);
	OSC_PLL_Software_Enable(TRUE);
	OSC_PLL_RST();
	/* ��ʱ������ */
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	OSC_SCK_Source_Config(SCLK_SOURCE_PLL);
	/* �������ʱ�� */
	OSC_HFCK_Division_Config(HFCK_DIVISION_1);
	OSC_HFCK_Source_Config(HFCK_SOURCE_INTHF);
	OSC_HFCK_Enable(TRUE);
}

/**
  * ����  ϵͳ��ʼ����
  * ����  �ޡ�
  * ����  �ޡ�
*/
void SystemInit(void)
{
	/* ʱ������ */
	SetSysClock();

	/* ��λ״̬ */
}

