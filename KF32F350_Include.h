/**
  *********************************************************************
  * 文件名  KF32F350_Include.h
  * 作  者  KungFu 编译工具组
  * 版  本  V0.1
  * 日  期  2019-10-19
  * 描  述  本文提供外设库文件的头文件引用
  *********************************************************************
  */

#ifndef _KF32F350_INCLUDE_H
#define _KF32F350_INCLUDE_H

#include <string.h>
#include <stdint.h>
#include "KF32F350.h"
#include "kf32f350_osc.h"
#include "kf32f350_int.h"
#include "kf32f350_gpio.h"
#include "kf32f350_usart.h"
#include "kf32f350_rst.h"
#include "kf32f350_pclk.h"

#include "Usart.h"
extern volatile uint8_t Receive_flag;
#endif /* _KF32F350_INCLUDE_H */

