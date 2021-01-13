/*
 * Usart.h
 *
 *  Created on: 2019-10-19
 *      Author: ChipON_AE/FAE_Group
 */

#ifndef _USART_H_
#define _USART_H_

extern void USART_Async_config(USART_SFRmap *USARTx);//串口异步全双工配置
extern void USART_Sync_config(USART_SFRmap* USARTx);//串口半双工同步配置
extern void USART_ReceiveInt_config(USART_SFRmap *USARTx,InterruptIndex Peripheral);//串口接收中断使能
extern void USART_Send(USART_SFRmap* USARTx, uint8_t* Databuf, uint32_t length);//串口发送函数

#endif /* USART_H_ */
