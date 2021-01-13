#ifndef SYS_H_
#define SYS_H_

#include <string.h>
#include <stdint.h>
#include "KF32F350_Include.h"

#ifndef SYSCLK_FREQ_HSE
	#define SYSCLK_FREQ_HSI
#endif

/* 系统时钟选择 */
//#define SYSCLK_FREQ_48MHz	48000000
//#define SYSCLK_FREQ_72MHz	72000000
//#define SYSCLK_FREQ_96MHz	96000000
#define SYSCLK_FREQ_120MHz	120000000
//#define SYSCLK_FREQ_150MHz	150000000

static void SetSysClock(void);
void SystemInit(void);
#endif /* SYS_H_ */
