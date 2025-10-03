#ifndef __INT_OLED_H__
#define __INT_OLED_H__
#include "Com_Util.h"

void Int_OLED_WriteData(u8 bytes[], u8 length);

void Int_OLED_WriteCmd(u8 bytes[], u8 length);

void Int_OLED_Init();

void Int_OLED_Clear();

/**
 * @brief ��ָ��λ�ã�x,y����ʾ�ַ���
 *
 * @param x ������
 * @param y ������
 * @param str ��չʾ�ַ���
 */
void Int_OLED_ShowStr(u8 x, u8 y, u8 *str);

/**
 * @brief ��ָ��λ��(x,y)��ʾ����
 *
 * @param x ������
 * @param y ������
 * @param num ��չʾ����
 */
void Int_OLED_ShowNum(u8 x, u8 y, long int num);

#endif /* __INT_OLED_H__ */