/**
  ********************************************************************
  * 文件名  system_init.h
  * 作  者   ChipON_AE/FAE_Group
  * 版  本  V2.1
  * 日  期  2018-3-15
  * 描  述  该文件提供了外设库包含的头文件与系统时钟配置相关的宏定义。
  *
  *********************************************************************
*/
#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_

#include <string.h>
#include <stdint.h>
#include "stdio.h"

#include "KF32LS_BASIC.h"
#include "kf32ls_basic_adc.h"
#include "kf32ls_basic_aes.h"
#include "kf32ls_basic_bkp.h"
#include "kf32ls_basic_can.h"
#include "kf32ls_basic_cfgl.h"
#include "kf32ls_basic_crc.h"
#include "kf32ls_basic_dma.h"
#include "kf32ls_basic_flash.h"
#include "kf32ls_basic_gpio.h"
#include "kf32ls_basic_i2c.h"
#include "kf32ls_basic_int.h"
#include "kf32ls_basic_iwdt.h"
#include "kf32ls_basic_led.h"
#include "kf32ls_basic_lcd.h"
#include "kf32ls_basic_osc.h"
#include "kf32ls_basic_pclk.h"
#include "kf32ls_basic_pm.h"
#include "kf32ls_basic_qei.h"
#include "kf32ls_basic_rst.h"
#include "kf32ls_basic_rtc.h"
#include "kf32ls_basic_spi.h"
#include "kf32ls_basic_sysctl.h"
#include "kf32ls_basic_systick.h"
#include "kf32ls_basic_tim.h"
#include "kf32ls_basic_usart.h"
#include "kf32ls_basic_usb.h"
#include "kf32ls_basic_wwdt.h"

#include "DMA_user.h"

extern volatile unsigned char Buf_full_flag;
//extern unsigned char buf_3[65],buf_2[64];
extern unsigned char buf_3[10],buf_2[10];
extern unsigned char buf_4[5];
/* 系统时钟选择 */
#ifndef SYSCLK_FREQ_HSE
	#define SYSCLK_FREQ_HSI         //内部高频
#endif

/* 系统时钟选择 */
#ifdef KF32LS500
#define SYSCLK_FREQ_48MHz	48000000
#endif
#ifdef KF32LS200
#define SYSCLK_FREQ_48MHz	48000000
#endif
#ifdef KF32LS100
#define SYSCLK_FREQ_48MHz	48000000
#endif

static void SetSysClock(void);
void SystemInit(void);
#endif /* SYS_H_ */
