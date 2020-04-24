#ifndef _MY_USART_HD_H_
#define _MY_USART_HD_H_

#include "includes.h"

#define BUFFER_SIZE					(20)  /* ���ڽ������ݻ������ĳ��� */
#define TIMEOUT_LIMIT				(200) //������ҩ���ڳ�ʱʱ��


extern rt_sem_t UartRxSem;

/* 
**************************************************
*                   ��������
**************************************************
*/
void Start_UART_Receive(void);
uint8_t getCS(uint8_t *pData, uint32_t dataLen);

void Ack_UartPing(void);
void Ack_SprayCtrl(ctrl_code eCode, sprayer_sw eSw, move_mode eMove);
void Ack_SprayState(fault_state eAlarm, sprayer_sw eSwState, platform_limit eLimit, platform_dir eDir);
bool Check_UartPing(void);
bool Check_ReadSprayState(void);
bool Check_WriteSprayCtrl(sprayer_sw *eSw, move_mode *eMove);

/* ���ڷ��ͺ��� */
void SendFrame(uint8_t *pData, uint16_t Len);

/* ���ڽ��պ��� */
void IDLE_UART_IRQHandler(UART_HandleTypeDef *huart);


#endif /* _MY_USART_HD_H_ */

