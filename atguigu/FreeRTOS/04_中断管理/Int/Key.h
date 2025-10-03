#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)   /* ��ȡKEY1����״̬��SW3�� */
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)   /* ��ȡKEY2����״̬��SW4�� */
#define KEY3        HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)   /* ��ȡKEY3����״̬��SW5�� */
#define KEY4        HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)   /* ��ȡKEY4����״̬��SW6�� */


#define KEY1_PRESS    1              
#define KEY2_PRESS    2              
#define KEY3_PRESS    3              
#define KEY4_PRESS    4              


uint8_t Key_Detect(void);


#endif
