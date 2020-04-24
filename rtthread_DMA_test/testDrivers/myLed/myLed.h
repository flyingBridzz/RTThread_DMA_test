#ifndef _MY_LED_H_
#define _MY_LED_H_

#include "main.h"

#define		PORT_LED				GPIOF
#define		PIN_LED0				GPIO_PIN_9
#define		PIN_LED1				GPIO_PIN_10

#define   LED_OFF					GPIO_PIN_SET
#define		LED_ON					GPIO_PIN_RESET

#define		LED0(x)					do{ \
														if(x==LED_OFF){ \
															HAL_GPIO_WritePin(PORT_LED, PIN_LED0, LED_OFF); \
														}else{ \
															HAL_GPIO_WritePin(PORT_LED, PIN_LED0, LED_ON); \
														} \
													}while(0)

#define		LED1(x)					do{ \
														if(x==LED_OFF){ \
															HAL_GPIO_WritePin(PORT_LED, PIN_LED1, LED_OFF); \
														}else{ \
															HAL_GPIO_WritePin(PORT_LED, PIN_LED1, LED_ON); \
														} \
													}while(0)

#define		TOG_LED(x)			do{ \
														if(x==0){ \
															HAL_GPIO_TogglePin(PORT_LED,PIN_LED0); \
														}else{ \
															HAL_GPIO_TogglePin(PORT_LED,PIN_LED1); \
														} \
													}while(0)
#endif

