/*
 * dma.h
 *
 *  Created on: 2019-11-5
 *      Author: charles_cai
 */

#ifndef DMA_H_
#define DMA_H_

extern uint8_t buf_2[10];
extern unsigned char buf_4[5];
void Init_dma() ;
void cfg_dma_intrupt();
void UART2_config_RXDMA(uint8_t len,uint8_t*pbuf);
void UART3_TXDMA_config(void);
#endif /* DMA_H_ */
