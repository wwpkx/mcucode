#ifndef __LCD_H
#define __LCD_H

#include "fsmc.h"
#include "delay.h"
#include <math.h>

#define PI 3.14

// 宏定义
// bank4 的基地址
#define BANK4_BASE 0x6C000000

// 写命令和写数据的地址，转换成可操作的指针
#define LCD_ADDR_CMD (uint16_t *)BANK4_BASE
#define LCD_ADDR_DATA (uint16_t *)(BANK4_BASE + (1 << 11))

// 定义显示屏的最大宽和高
#define W 320
#define H 480

// 常见颜色
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
#define BROWN 0XBC40 // 棕色
#define BRRED 0XFC07 // 棕红色
#define GRAY 0X8430  // 灰色

// 函数定义
// 1. 基本操作
// 1.1 初始化
void LCD_Init(void);

// 复位
void LCD_Reset(void);
// 开关背光
void LCD_BGOn(void);
void LCD_BGOff(void);

// 写命令（发出命令）
void LCD_WriteCmd(uint16_t cmd);
// 写数据
void LCD_WriteData(uint16_t data);
// 读数据
uint16_t LCD_ReadData(void);

// 初始化LCD的寄存器配置
void LCD_RegConfig(void);

// 2. 具体命令操作
// 2.1 返回ID信息
uint32_t LCD_ReadID(void);

// 通用操作：设置显示区域，给定起始点坐标（x，y），以及区域的宽和高
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

// 2.2 将屏幕设置为一种颜色；清屏
void LCD_ClearAll(uint16_t color);

// 2.3 显示字符
// 2.3.1 显示单个英文（ASCII）字符，width = height / 2
void LCD_WriteAsciiChar(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t c, uint16_t fColor, uint16_t bColor);

// 2.3.2 显示英文（ASCII）字符串，width = height / 2
void LCD_WriteAsciiString(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t *str, uint16_t fColor, uint16_t bColor);

// 2.3.3 显示中文字符
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t index, uint16_t fColor, uint16_t bColor);

// 2.4 显示图片 - 尚硅谷LOGO
void LCD_WriteAtguiguLogo(uint16_t x, uint16_t y);

// 2.5 显示几何图形
// 2.5.1 点(方块点)
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color);

// 2.5.2 线
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color);

// 2.5.3 画长方形
void LCD_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t w, uint16_t color);

// 2.5.4 画空心圆，给定圆心坐标和半径
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color);
void LCD_DrawCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color);

// 2.5.5 画实心圆，给定圆心坐标和半径
void LCD_DrawFilledCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor);
void LCD_DrawFilledCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor);


#endif
