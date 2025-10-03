#ifndef __LCD_H
#define __LCD_H

#include "fsmc.h"
#include "delay.h"
#include <math.h>

#define PI 3.14

// �궨��
// bank4 �Ļ���ַ
#define BANK4_BASE 0x6C000000

// д�����д���ݵĵ�ַ��ת���ɿɲ�����ָ��
#define LCD_ADDR_CMD (uint16_t *)BANK4_BASE
#define LCD_ADDR_DATA (uint16_t *)(BANK4_BASE + (1 << 11))

// ������ʾ��������͸�
#define W 320
#define H 480

// ������ɫ
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 // ��ɫ
#define BRRED 0XFC07 // �غ�ɫ
#define GRAY 0X8430  // ��ɫ

// ��������
// 1. ��������
// 1.1 ��ʼ��
void LCD_Init(void);

// ��λ
void LCD_Reset(void);
// ���ر���
void LCD_BGOn(void);
void LCD_BGOff(void);

// д����������
void LCD_WriteCmd(uint16_t cmd);
// д����
void LCD_WriteData(uint16_t data);
// ������
uint16_t LCD_ReadData(void);

// ��ʼ��LCD�ļĴ�������
void LCD_RegConfig(void);

// 2. �����������
// 2.1 ����ID��Ϣ
uint32_t LCD_ReadID(void);

// ͨ�ò�����������ʾ���򣬸�����ʼ�����꣨x��y�����Լ�����Ŀ�͸�
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

// 2.2 ����Ļ����Ϊһ����ɫ������
void LCD_ClearAll(uint16_t color);

// 2.3 ��ʾ�ַ�
// 2.3.1 ��ʾ����Ӣ�ģ�ASCII���ַ���width = height / 2
void LCD_WriteAsciiChar(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t c, uint16_t fColor, uint16_t bColor);

// 2.3.2 ��ʾӢ�ģ�ASCII���ַ�����width = height / 2
void LCD_WriteAsciiString(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t *str, uint16_t fColor, uint16_t bColor);

// 2.3.3 ��ʾ�����ַ�
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t index, uint16_t fColor, uint16_t bColor);

// 2.4 ��ʾͼƬ - �й��LOGO
void LCD_WriteAtguiguLogo(uint16_t x, uint16_t y);

// 2.5 ��ʾ����ͼ��
// 2.5.1 ��(�����)
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

// 2.5.2 ��
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color);

// 2.5.3 ��������
void LCD_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t w, uint16_t color);

// 2.5.4 ������Բ������Բ������Ͱ뾶
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color);
void LCD_DrawCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color);

// 2.5.5 ��ʵ��Բ������Բ������Ͱ뾶
void LCD_DrawFilledCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor);
void LCD_DrawFilledCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor);


#endif
