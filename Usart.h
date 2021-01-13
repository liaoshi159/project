/*
 * Usart.h
 *
 *  Created on: 2019-10-19
 *      Author: ChipON_AE/FAE_Group
 */

#ifndef _USART_H_
#define _USART_H_

extern void USART_Async_config(USART_SFRmap *USARTx);//�����첽ȫ˫������
extern void USART_Sync_config(USART_SFRmap* USARTx);//���ڰ�˫��ͬ������
extern void USART_ReceiveInt_config(USART_SFRmap *USARTx,InterruptIndex Peripheral);//���ڽ����ж�ʹ��
extern void USART_Send(USART_SFRmap* USARTx, uint8_t* Databuf, uint32_t length);//���ڷ��ͺ���

#endif /* USART_H_ */
