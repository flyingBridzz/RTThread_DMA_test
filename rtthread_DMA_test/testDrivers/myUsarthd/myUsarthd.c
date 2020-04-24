#include "myUsarthd.h"

static volatile uint8_t sprayUartRxBuffer[BUFFER_SIZE] = {0};
static volatile uint8_t sprayUartTxBuffer[BUFFER_SIZE] = {0};
static uint8_t sprayUartAddr[6] = {0x00,0x00,0x00,0x00,0x00,0x00};

void Start_UART_Receive(void)
{
	memset((void *)sprayUartRxBuffer, 0, BUFFER_SIZE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_IT(&huart1,(uint8_t *)sprayUartRxBuffer,BUFFER_SIZE);
}

uint8_t getCS(uint8_t *pData, uint32_t dataLen)
{
	uint32_t i;
	uint8_t tmpCS = 0;
	for(i=0;i<dataLen;i++)
	{
		tmpCS += pData[i];
	}
	return tmpCS;
}

void Ack_UartPing(void)
{
	sprayUartTxBuffer[0] = 0x68;
	memcpy((uint8_t *)&sprayUartTxBuffer[1], sprayUartAddr, 6);//��ַ��(Ĭ��: 00 00 00 00 00 00)
	sprayUartTxBuffer[7] = 0x68;
	sprayUartTxBuffer[8] = ackReadCode;//��������
	sprayUartTxBuffer[9] = 0x04;//�����򳤶�(L=4+m (m=0))
	
	/* ���ݱ�ʶ */
	sprayUartTxBuffer[10] = 0x00;
	sprayUartTxBuffer[11] = 0x02;
	sprayUartTxBuffer[12] = 0x00;
	sprayUartTxBuffer[13] = 0x84;

	sprayUartTxBuffer[14] = getCS((uint8_t *)sprayUartTxBuffer, 14);//CSУ����
	sprayUartTxBuffer[15] = 0x16;
	SendFrame((uint8_t *)sprayUartTxBuffer, 16);
}

void Ack_SprayCtrl(ctrl_code eCode, sprayer_sw eSw, move_mode eMove)
{
	sprayUartTxBuffer[0] = 0x68;
	memcpy((uint8_t *)&sprayUartTxBuffer[1], sprayUartAddr, 6);
	sprayUartTxBuffer[7] = 0x68;
	sprayUartTxBuffer[8] = eCode;
	sprayUartTxBuffer[9] = 0x06;
	sprayUartTxBuffer[10] = 0x00;
	sprayUartTxBuffer[11] = 0x02;
	sprayUartTxBuffer[12] = 0x0A;
	sprayUartTxBuffer[13] = 0x84;
	sprayUartTxBuffer[14] = eSw;
	sprayUartTxBuffer[15] = eMove;
	sprayUartTxBuffer[16] = getCS((uint8_t *)sprayUartTxBuffer,16);
	sprayUartTxBuffer[17] = 0x16;
	SendFrame((uint8_t *)sprayUartTxBuffer, 18);
}

void Ack_SprayState(fault_state eAlarm, sprayer_sw eSwState, platform_limit eLimit, platform_dir eDir)
{
	sprayUartTxBuffer[0] = 0x68;
	memcpy((uint8_t *)&sprayUartTxBuffer[1], sprayUartAddr, 6);
	sprayUartTxBuffer[7] = 0x68;
	sprayUartTxBuffer[8] = ackReadCode;
	sprayUartTxBuffer[9] = 0x07;
	sprayUartTxBuffer[10] = 0x00;
	sprayUartTxBuffer[11] = 0x02;
	sprayUartTxBuffer[12] = 0x0A;
	sprayUartTxBuffer[13] = 0x84;
	sprayUartTxBuffer[14] = eSwState;
	sprayUartTxBuffer[15] = eLimit;
	sprayUartTxBuffer[16] = eDir;
	sprayUartTxBuffer[17] = eAlarm;
	sprayUartTxBuffer[18] = getCS((uint8_t *)sprayUartTxBuffer,18);
	sprayUartTxBuffer[19] = 0x16;
	SendFrame((uint8_t *)sprayUartTxBuffer, 20);
}

bool Check_UartPing(void)
{
	if(sprayUartRxBuffer[0] != 0x68)
		return false;
	if(0 != memcmp((const char *)sprayUartAddr,(const char *)&sprayUartRxBuffer[1],6))
		return false;
	if(sprayUartRxBuffer[7] != 0x68)
		return false;
	if(sprayUartRxBuffer[8] != 0x11)
		return false;
	if(sprayUartRxBuffer[9] != 0x04)
		return false;
	if(sprayUartRxBuffer[10] != 0x00)
		return false;
	if(sprayUartRxBuffer[11] != 0x02)
		return false;
	if(sprayUartRxBuffer[12] != 0x00)
		return false;
	if(sprayUartRxBuffer[13] != 0x84)
		return false;
	if(sprayUartRxBuffer[14] != getCS((uint8_t *)sprayUartRxBuffer, 14))
		return false;
	if(sprayUartRxBuffer[15] != 0x16)
		return false;
	
	return true;
}

bool Check_ReadSprayState(void)
{
	if(sprayUartRxBuffer[0] != 0x68)
		return false;
	if(0 != memcmp((const char *)sprayUartAddr,(const char *)&sprayUartRxBuffer[1],6))
		return false;
	if(sprayUartRxBuffer[7] != 0x68)
		return false;
	if(sprayUartRxBuffer[8] != 0x11)
		return false;
	if(sprayUartRxBuffer[9] != 0x04)
		return false;
	if(sprayUartRxBuffer[10] != 0x00)
		return false;
	if(sprayUartRxBuffer[11] != 0x02)
		return false;
	if(sprayUartRxBuffer[12] != 0x0A)
		return false;
	if(sprayUartRxBuffer[13] != 0x84)
		return false;
	if(sprayUartRxBuffer[14] != getCS((uint8_t *)sprayUartRxBuffer, 14))
		return false;
	if(sprayUartRxBuffer[15] != 0x16)
		return false;
	
	return true;
}

bool Check_WriteSprayCtrl(sprayer_sw *eSw, move_mode *eMove)
{
	if(sprayUartRxBuffer[0] != 0x68)
		return false;
	if(0 != memcmp((const char *)sprayUartAddr,(const char *)&sprayUartRxBuffer[1],6))
		return false;
	if(sprayUartRxBuffer[7] != 0x68)
		return false;
	if(sprayUartRxBuffer[8] != 0x14)
		return false;
	if(sprayUartRxBuffer[9] != 0x06)
		return false;
	if(sprayUartRxBuffer[10] != 0x00)
		return false;
	if(sprayUartRxBuffer[11] != 0x02)
		return false;
	if(sprayUartRxBuffer[12] != 0x0A)
		return false;
	if(sprayUartRxBuffer[13] != 0x84)
		return false;
	if((sprayUartRxBuffer[14] != sprayOpen)
		&&(sprayUartRxBuffer[14] != sprayClose))
		return false;
	if((sprayUartRxBuffer[15] != upToLimit)
		&&(sprayUartRxBuffer[15] != stopLift)
		&&(sprayUartRxBuffer[15] != downToLimit)
		&&(sprayUartRxBuffer[15] != lift))
		return false;
	if(sprayUartRxBuffer[16] != getCS((uint8_t *)sprayUartRxBuffer, 16))
		return false;
	if(sprayUartRxBuffer[17] != 0x16)
		return false;
	
	*eSw = (sprayer_sw)sprayUartRxBuffer[14];
	*eMove = (move_mode)sprayUartRxBuffer[15];
	return true;
}




/*
**************************************************
*��������@IDLE_UART_IRQHandler
*���ܣ�	 ���ڿ����жϷ�����
*ע�ͣ�  ����ʹ�ô��ڿ����ж���Ҫ��stm32f4xx_it.c�ļ����µ�@USART1_IRQHandler@��������;
*        Ĭ��ʹ�õ��Ǵ���һ����ʹ���������������и���;
**************************************************
*/
void IDLE_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	uint32_t tmp_flag = 0, tmp_it_source = 0;
	if(huart->Instance == USART1)
	{
		rt_interrupt_enter();
		tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE);
		tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE);
		/* ��⴮�ڿ����ж�---------------------------------------------------------*/
		if((tmp_flag != RESET) && (tmp_it_source != RESET))
		{
			rt_sem_release(UartRxSem);
			
			//���IDLE�жϱ�־λ
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			__HAL_UART_DISABLE_IT(huart, UART_IT_IDLE);
			
			HAL_UART_AbortReceive_IT(huart);//�رմ��ڽ����ж�
		}
		rt_interrupt_leave();
	}
}


/*
**************************************************
*��������@SendFrame
*���ܣ�  ֱ�ӷ���ָ�����ȵ��ַ���
*������  uint8_t *pData	@Ҫ���͵�����(ÿ����Ԫ��һ���ֽ�)
*        uint16_t Len		@Ҫ���͵����ݳ���(��λ���ֽ�)
**************************************************
*/
void SendFrame(uint8_t *pData, uint16_t Len)
{
	rt_enter_critical();
	HAL_UART_Transmit(&huart1, pData, Len, 1000);
	rt_exit_critical();
}



