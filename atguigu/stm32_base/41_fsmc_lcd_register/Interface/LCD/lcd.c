/*
 * @Author: wushengran
 * @Date: 2024-10-13 15:15:12
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
/*
 * @Author: wushengran
 * @Date: 2024-10-13 15:15:12
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */
#include "lcd.h"
#include "lcd_font.h"

// 1. 基本操作
// 1.1 初始化
void LCD_Init(void)
{
    // 1. 底层硬件FSMC初始化
    FSMC_Init();

    // 2. 复位
    LCD_Reset();

    // 3. 开启背光
    LCD_BGOn();

    // 4. 初始化LCD的寄存器配置
    LCD_RegConfig();
}

// 复位 - PG15
void LCD_Reset(void)
{
    GPIOG->ODR &= ~GPIO_ODR_ODR15;
    Delay_ms(100);
    GPIOG->ODR |= GPIO_ODR_ODR15;
    Delay_ms(100);
}
// 开关背光 - PB0
void LCD_BGOn(void)
{
    GPIOB->ODR |= GPIO_ODR_ODR0;
}
void LCD_BGOff(void)
{
    GPIOB->ODR &= ~GPIO_ODR_ODR0;
}

// 写命令（发出命令）
void LCD_WriteCmd(uint16_t cmd)
{
    *LCD_ADDR_CMD = cmd;
}
// 写数据
void LCD_WriteData(uint16_t data)
{
    *LCD_ADDR_DATA = data;
}
// 读数据
uint16_t LCD_ReadData(void)
{
    return *LCD_ADDR_DATA;
}

// 初始化LCD的寄存器配置
void LCD_RegConfig(void)
{
    /* 1. 设置灰阶电压以调整TFT面板的伽马特性， 正校准。一般出厂就设置好了 */
    LCD_WriteCmd(0xE0);
    LCD_WriteData(0x00);
    LCD_WriteData(0x07);
    LCD_WriteData(0x10);
    LCD_WriteData(0x09);
    LCD_WriteData(0x17);
    LCD_WriteData(0x0B);
    LCD_WriteData(0x41);
    LCD_WriteData(0x89);
    LCD_WriteData(0x4B);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x18);
    LCD_WriteData(0x1B);
    LCD_WriteData(0x0F);

    /* 2. 设置灰阶电压以调整TFT面板的伽马特性，负校准 */
    LCD_WriteCmd(0XE1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x17);
    LCD_WriteData(0x1A);
    LCD_WriteData(0x04);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x06);
    LCD_WriteData(0x2F);
    LCD_WriteData(0x45);
    LCD_WriteData(0x43);
    LCD_WriteData(0x02);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x09);
    LCD_WriteData(0x32);
    LCD_WriteData(0x36);
    LCD_WriteData(0x0F);

    /* 3.  Adjust Control 3 (F7h)  */
    /*LCD_WriteCmd(0XF7);
   LCD_WriteData(0xA9);
   LCD_WriteData(0x51);
   LCD_WriteData(0x2C);
   LCD_WriteData(0x82);*/
    /* DSI write DCS command, use loose packet RGB 666 */

    /* 4. 电源控制1*/
    LCD_WriteCmd(0xC0);
    LCD_WriteData(0x11); /* 正伽马电压 */
    LCD_WriteData(0x09); /* 负伽马电压 */

    /* 5. 电源控制2 */
    LCD_WriteCmd(0xC1);
    LCD_WriteData(0x02);
    LCD_WriteData(0x03);

    /* 6. VCOM控制 */
    LCD_WriteCmd(0XC5);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x80);

    /* 7. Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    LCD_WriteCmd(0xB1);
    LCD_WriteData(0xB0);
    LCD_WriteData(0x11);

    /* 8.  Display Inversion Control (B4h) （正负电压反转，减少电磁干扰）*/
    LCD_WriteCmd(0xB4);
    LCD_WriteData(0x02);

    /* 9.  Display Function Control (B6h)  */
    LCD_WriteCmd(0xB6);
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);

    /* 10. Entry Mode Set (B7h)  */
    LCD_WriteCmd(0xB7);
    LCD_WriteData(0xc6);

    /* 11. HS Lanes Control (BEh) */
    LCD_WriteCmd(0xBE);
    LCD_WriteData(0x00);
    LCD_WriteData(0x04);

    /* 12.  Interface Pixel Format (3Ah) */
    LCD_WriteCmd(0x3A);
    LCD_WriteData(0x55); /* 0x55 : 16 bits/pixel  */

    /* 13. Sleep Out (11h) 关闭休眠模式 */
    LCD_WriteCmd(0x11);

    /* 14. 设置屏幕方向和RGB */
    LCD_WriteCmd(0x36);
    LCD_WriteData(0x08);

    Delay_ms(120);

    /* 14. display on */
    LCD_WriteCmd(0x29);
}

// 2. 具体命令操作
// 2.1 返回ID信息
uint32_t LCD_ReadID(void)
{
    // 1. 发送命令04h
    LCD_WriteCmd(0x04);

    // 2. 读取一个空数据
    LCD_ReadData();

    // 3. 读取三次，拼成完整ID返回
    uint32_t id = 0;
    id |= (LCD_ReadData() & 0xff) << 16; // 第一个字节ID
    id |= (LCD_ReadData() & 0xff) << 8;  // 第二个字节ID
    id |= (LCD_ReadData() & 0xff) << 0;  // 第三个字节ID

    return id;
}

// 通用操作：设置显示区域，给定起始点坐标（x，y），以及区域的宽和高
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // 1. 设置列
    LCD_WriteCmd(0x2a);
    // 1.1 开始列
    LCD_WriteData((x >> 8) & 0xff); // 起始列高位字节
    LCD_WriteData((x >> 0) & 0xff); // 起始列低位字节

    // 1.2 结束列
    LCD_WriteData(((x + w - 1) >> 8) & 0xff);
    LCD_WriteData(((x + w - 1) >> 0) & 0xff);

    // 2. 设置行
    LCD_WriteCmd(0x2b);
    // 1.1 开始行
    LCD_WriteData((y >> 8) & 0xff); // 起始列高位字节
    LCD_WriteData((y >> 0) & 0xff); // 起始列低位字节

    // 1.2 结束行
    LCD_WriteData(((y + h - 1) >> 8) & 0xff);
    LCD_WriteData(((y + h - 1) >> 0) & 0xff);
}

// 2.2 将屏幕设置为一种颜色；清屏
void LCD_ClearAll(uint16_t color)
{
    // 1. 设置显示区域为全屏
    LCD_SetArea(0, 0, W, H);

    // 2. 发送写显存命令
    LCD_WriteCmd(0x2c);

    // 3. 循环发送数据
    for (uint32_t i = 0; i < W * H; i++)
    {
        LCD_WriteData(color);
    }
}

// 2.3 显示字符
// 2.3.1 显示单个英文（ASCII）字符，width = height / 2
void LCD_WriteAsciiChar(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t c, uint16_t fColor, uint16_t bColor)
{
    // 1. 设置要显示的区域
    LCD_SetArea(x, y, height / 2, height);

    // 2. 发送写显存指令
    LCD_WriteCmd(0x2c);

    // 3. 找到字符点阵表达中的每一个像素点，发送对应的颜色
    // 3.1 计算字符在二维数组中的索引位置
    uint8_t index = c - ' ';

    // 3.2 判断字号大小，选择对应的二维数组，遍历每一个字节
    if (height == 16 || height == 12)
    {
        // 每个字节就是点阵中的一行，一共height行
        for (uint8_t i = 0; i < height; i++)
        {
            // 遍历每一行，先从二维数组中提取这一行的点阵表达
            uint8_t row = (height == 16) ? ascii_1608[index][i] : ascii_1206[index][i];

            // 逆向遍历一行中的每个点（从低位开始判断字节中的每一位）
            for (uint8_t j = 0; j < height / 2; j++)
            {
                if (row & 0x01)
                {
                    // 如果是1，发送字体颜色
                    LCD_WriteData(fColor);
                }
                else
                {
                    LCD_WriteData(bColor);
                }
                row >>= 1;
            }
        }
    }
    else if (height == 24)
    {
        // 每两个字节就是点阵中的一行，一共 height 行，height * 2 个字节
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 遍历每一个字节，先从二维数组中提取这一个字节（半行）的点阵表达
            uint8_t byte = ascii_2412[index][i];

            // 对于24 * 12的字号，需要判断是一行的第一个还是第二个字节，决定遍历的位数
            uint8_t bits_count = i % 2 ? 4 : 8;

            // 逆向遍历一个字节中的每个点（从低位开始判断字节中的每一位）
            for (uint8_t j = 0; j < bits_count; j++)
            {
                if (byte & 0x01)
                {
                    // 如果是1，发送字体颜色
                    LCD_WriteData(fColor);
                }
                else
                {
                    LCD_WriteData(bColor);
                }
                byte >>= 1;
            }
        }
    }
    else if (height == 32)
    {
        // 每两个字节就是点阵中的一行，一共 height 行，height * 2 个字节
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // 遍历每一个字节，先从二维数组中提取这一个字节（半行）的点阵表达
            uint8_t byte = ascii_3216[index][i];

            // 逆向遍历一个字节中的每个点（从低位开始判断字节中的每一位）
            for (uint8_t j = 0; j < 8; j++)
            {
                if (byte & 0x01)
                {
                    // 如果是1，发送字体颜色
                    LCD_WriteData(fColor);
                }
                else
                {
                    LCD_WriteData(bColor);
                }
                byte >>= 1;
            }
        }
    }
}

// 2.3.2 显示英文（ASCII）字符串，width = height / 2
void LCD_WriteAsciiString(uint16_t x, uint16_t y, uint16_t height,
                          uint8_t *str, uint16_t fColor, uint16_t bColor)
{
    uint16_t width = height / 2;

    // 定义循环变量，字符串中的索引位置
    uint8_t i = 0;

    // 以'\0'作为字符串结束标志
    while (str[i] != '\0')
    {
        // 判断是否是'\n'
        if (str[i] != '\n')
        {
            // 如果不是'\n'，还需要判断是否到达屏幕显示右边界
            if (x + width > W)
            {
                // 如果超出，就回车换行
                x = 0;
                y += height;
            }
            // 显示当前字符
            LCD_WriteAsciiChar(x, y, height, str[i], fColor, bColor);
            // 将 x 后移
            x += width;
        }
        else
        {
            // 如果遇到'\n'，就回车换行
            x = 0;
            y += height;
        }
        i++;
    }
}

// 2.3.3 显示中文字符
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t height,
                          uint8_t index, uint16_t fColor, uint16_t bColor)
{
    // 1. 设置显示区域
    LCD_SetArea(x, y, height, height);

    // 2. 发送命令
    LCD_WriteCmd(0x2c);

    // 3. 循环遍历字符点阵的每一行，4 个字节表示一行
    for (uint8_t i = 0; i < height * 4; i++)
    {
        // 遍历每一个字节，先从二维数组中提取这一个字节（半行）的点阵表达
        uint8_t byte = chinese[index][i];

        // 逆向遍历一个字节中的每个点（从低位开始判断字节中的每一位）
        for (uint8_t j = 0; j < 8; j++)
        {
            if (byte & 0x01)
            {
                // 如果是1，发送字体颜色
                LCD_WriteData(fColor);
            }
            else
            {
                LCD_WriteData(bColor);
            }
            byte >>= 1;
        }
    }
}

// 2.4 显示图片 - 尚硅谷LOGO
void LCD_WriteAtguiguLogo(uint16_t x, uint16_t y)
{
    // 1. 设置显示区域
    LCD_SetArea(x, y, 227, 68);

    // 2. 发送命令
    LCD_WriteCmd(0x2c);

    // 3. 遍历图像对应的字节数组，每2个字节表示一个像素点的RGB数据
    uint16_t len = sizeof(gImage_logo);
    for (uint16_t i = 0; i < len; i += 2)
    {
        // 拼接两个字节，得到要写入的16位RGB数据，低位在前
        uint16_t pColor = gImage_logo[i] + (gImage_logo[i + 1] << 8);

        // 发送数据
        LCD_WriteData(pColor);
    }
}

// 2.5 显示几何图形
// 2.5.1 点(方块点)
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
    LCD_SetArea(x, y, w, w);

    LCD_WriteCmd(0x2c);

    // 遍历区域中所有点，发送color数据
    for (uint16_t i = 0; i < w * w; i++)
    {
        LCD_WriteData(color);
    }
}

// 2.5.2 线，默认 x1 <= x2, 当 x1 == x2 时，y1 <= y2
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color)
{
    // 判断 x1 == x2，如果相等，直接画一条竖线
    if (x1 == x2)
    {
        for (uint16_t y = y1; y <= y2; y++)
        {
            LCD_DrawPoint(x1, y, w, color);
        }
        return;
    }

    // 按直线方程求k和b
    // y = kx + b
    // k = (y1 - y2)/(x1 - x2)
    // b = y1 - k * x1
    double k = 1.0 * (y1 - y2) / (x1 - x2);
    double b = y1 - k * x1;

    for (uint16_t x = x1; x <= x2; x++)
    {
        uint16_t y = (uint16_t)(k * x + b);
        LCD_DrawPoint(x, y, w, color);
    }
}

// 2.5.3 画长方形（空心）
void LCD_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t w, uint16_t color)
{
    uint16_t x_end = x + width - 1;
    uint16_t y_end = y + height - 1;

    // 画出4条线
    LCD_DrawLine(x, y, x_end, y, w, color);
    LCD_DrawLine(x, y, x, y_end, w, color);
    LCD_DrawLine(x_end, y, x_end, y_end, w, color);
    LCD_DrawLine(x, y_end, x_end, y_end, w, color);
}

// 2.5.4 画圆，给定圆心坐标和半径
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    // 利用极坐标方程，算出圆上每一点的横纵坐标
    for (uint16_t theta = 0; theta < 360; theta++)
    {
        uint16_t x = xCenter + r * cos(theta * PI / 180);
        uint16_t y = yCenter + r * sin(theta * PI / 180);

        LCD_DrawPoint(x, y, w, color);
    }
}

// 改进画法：同时在四象限找点
void LCD_DrawCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    // 利用极坐标方程，算出圆上每一点的横纵坐标
    for (uint16_t theta = 0; theta < 90; theta++)
    {
        uint16_t delta_x = r * cos(theta * PI / 180);
        uint16_t delta_y = r * sin(theta * PI / 180);

        // 第一象限点
        uint16_t x = xCenter + delta_x;
        uint16_t y = yCenter + delta_y;
        LCD_DrawPoint(x, y, w, color);

        // 第二象限点
        x = xCenter - delta_x;
        y = yCenter + delta_y;
        LCD_DrawPoint(x, y, w, color);

        // 第三象限点
        x = xCenter - delta_x;
        y = yCenter - delta_y;
        LCD_DrawPoint(x, y, w, color);

        // 第四象限点
        x = xCenter + delta_x;
        y = yCenter - delta_y;
        LCD_DrawPoint(x, y, w, color);
    }
}

// 2.5.5 画实心圆，给定圆心坐标和半径
void LCD_DrawFilledCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor)
{
    // 用一个循环，不停改变r，从内向外画同心圆
    for (uint16_t ri = 0; ri < r; ri++)
    {
        LCD_DrawCircle(xCenter, yCenter, ri, w, iColor);
    }

    // 最外层用单独颜色画出边界
    LCD_DrawCircle(xCenter, yCenter, r, w, color);
}
void LCD_DrawFilledCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor)
{
    // 利用极坐标方程，算出圆上每一点的横纵坐标
    for (uint16_t theta = 0; theta < 90; theta++)
    {
        uint16_t delta_x = r * cos(theta * PI / 180);
        uint16_t delta_y = r * sin(theta * PI / 180);

        // 第一象限点
        uint16_t x1 = xCenter + delta_x;
        uint16_t y1 = yCenter + delta_y;
        LCD_DrawPoint(x1, y1, w, color);

        // 第二象限点
        uint16_t x2 = xCenter - delta_x;
        uint16_t y2 = yCenter + delta_y;
        LCD_DrawPoint(x2, y2, w, color);

        // 两点连成一条横向直线
        LCD_DrawLine(x2 + w, y2, x1 - w, y1, w, iColor);

        // 第三象限点
        x1 = xCenter - delta_x;
        y1 = yCenter - delta_y;
        LCD_DrawPoint(x1, y1, w, color);

        // 第四象限点
        x2 = xCenter + delta_x;
        y2 = yCenter - delta_y;
        LCD_DrawPoint(x2, y2, w, color);

        // 两点连成一条横向直线
        LCD_DrawLine(x1 + w, y1, x2 - w, y2, w, iColor);
    }
}
