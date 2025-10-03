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

// 1. ��������
// 1.1 ��ʼ��
void LCD_Init(void)
{
    // 1. �ײ�Ӳ��FSMC��ʼ��
    FSMC_Init();

    // 2. ��λ
    LCD_Reset();

    // 3. ��������
    LCD_BGOn();

    // 4. ��ʼ��LCD�ļĴ�������
    LCD_RegConfig();
}

// ��λ - PG15
void LCD_Reset(void)
{
    GPIOG->ODR &= ~GPIO_ODR_ODR15;
    Delay_ms(100);
    GPIOG->ODR |= GPIO_ODR_ODR15;
    Delay_ms(100);
}
// ���ر��� - PB0
void LCD_BGOn(void)
{
    GPIOB->ODR |= GPIO_ODR_ODR0;
}
void LCD_BGOff(void)
{
    GPIOB->ODR &= ~GPIO_ODR_ODR0;
}

// д����������
void LCD_WriteCmd(uint16_t cmd)
{
    *LCD_ADDR_CMD = cmd;
}
// д����
void LCD_WriteData(uint16_t data)
{
    *LCD_ADDR_DATA = data;
}
// ������
uint16_t LCD_ReadData(void)
{
    return *LCD_ADDR_DATA;
}

// ��ʼ��LCD�ļĴ�������
void LCD_RegConfig(void)
{
    /* 1. ���ûҽ׵�ѹ�Ե���TFT����٤�����ԣ� ��У׼��һ����������ú��� */
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

    /* 2. ���ûҽ׵�ѹ�Ե���TFT����٤�����ԣ���У׼ */
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

    /* 4. ��Դ����1*/
    LCD_WriteCmd(0xC0);
    LCD_WriteData(0x11); /* ��٤���ѹ */
    LCD_WriteData(0x09); /* ��٤���ѹ */

    /* 5. ��Դ����2 */
    LCD_WriteCmd(0xC1);
    LCD_WriteData(0x02);
    LCD_WriteData(0x03);

    /* 6. VCOM���� */
    LCD_WriteCmd(0XC5);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0A);
    LCD_WriteData(0x80);

    /* 7. Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    LCD_WriteCmd(0xB1);
    LCD_WriteData(0xB0);
    LCD_WriteData(0x11);

    /* 8.  Display Inversion Control (B4h) ��������ѹ��ת�����ٵ�Ÿ��ţ�*/
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

    /* 13. Sleep Out (11h) �ر�����ģʽ */
    LCD_WriteCmd(0x11);

    /* 14. ������Ļ�����RGB */
    LCD_WriteCmd(0x36);
    LCD_WriteData(0x08);

    Delay_ms(120);

    /* 14. display on */
    LCD_WriteCmd(0x29);
}

// 2. �����������
// 2.1 ����ID��Ϣ
uint32_t LCD_ReadID(void)
{
    // 1. ��������04h
    LCD_WriteCmd(0x04);

    // 2. ��ȡһ��������
    LCD_ReadData();

    // 3. ��ȡ���Σ�ƴ������ID����
    uint32_t id = 0;
    id |= (LCD_ReadData() & 0xff) << 16; // ��һ���ֽ�ID
    id |= (LCD_ReadData() & 0xff) << 8;  // �ڶ����ֽ�ID
    id |= (LCD_ReadData() & 0xff) << 0;  // �������ֽ�ID

    return id;
}

// ͨ�ò�����������ʾ���򣬸�����ʼ�����꣨x��y�����Լ�����Ŀ�͸�
void LCD_SetArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    // 1. ������
    LCD_WriteCmd(0x2a);
    // 1.1 ��ʼ��
    LCD_WriteData((x >> 8) & 0xff); // ��ʼ�и�λ�ֽ�
    LCD_WriteData((x >> 0) & 0xff); // ��ʼ�е�λ�ֽ�

    // 1.2 ������
    LCD_WriteData(((x + w - 1) >> 8) & 0xff);
    LCD_WriteData(((x + w - 1) >> 0) & 0xff);

    // 2. ������
    LCD_WriteCmd(0x2b);
    // 1.1 ��ʼ��
    LCD_WriteData((y >> 8) & 0xff); // ��ʼ�и�λ�ֽ�
    LCD_WriteData((y >> 0) & 0xff); // ��ʼ�е�λ�ֽ�

    // 1.2 ������
    LCD_WriteData(((y + h - 1) >> 8) & 0xff);
    LCD_WriteData(((y + h - 1) >> 0) & 0xff);
}

// 2.2 ����Ļ����Ϊһ����ɫ������
void LCD_ClearAll(uint16_t color)
{
    // 1. ������ʾ����Ϊȫ��
    LCD_SetArea(0, 0, W, H);

    // 2. ����д�Դ�����
    LCD_WriteCmd(0x2c);

    // 3. ѭ����������
    for (uint32_t i = 0; i < W * H; i++)
    {
        LCD_WriteData(color);
    }
}

// 2.3 ��ʾ�ַ�
// 2.3.1 ��ʾ����Ӣ�ģ�ASCII���ַ���width = height / 2
void LCD_WriteAsciiChar(uint16_t x, uint16_t y, uint16_t height,
                        uint8_t c, uint16_t fColor, uint16_t bColor)
{
    // 1. ����Ҫ��ʾ������
    LCD_SetArea(x, y, height / 2, height);

    // 2. ����д�Դ�ָ��
    LCD_WriteCmd(0x2c);

    // 3. �ҵ��ַ��������е�ÿһ�����ص㣬���Ͷ�Ӧ����ɫ
    // 3.1 �����ַ��ڶ�ά�����е�����λ��
    uint8_t index = c - ' ';

    // 3.2 �ж��ֺŴ�С��ѡ���Ӧ�Ķ�ά���飬����ÿһ���ֽ�
    if (height == 16 || height == 12)
    {
        // ÿ���ֽھ��ǵ����е�һ�У�һ��height��
        for (uint8_t i = 0; i < height; i++)
        {
            // ����ÿһ�У��ȴӶ�ά��������ȡ��һ�еĵ�����
            uint8_t row = (height == 16) ? ascii_1608[index][i] : ascii_1206[index][i];

            // �������һ���е�ÿ���㣨�ӵ�λ��ʼ�ж��ֽ��е�ÿһλ��
            for (uint8_t j = 0; j < height / 2; j++)
            {
                if (row & 0x01)
                {
                    // �����1������������ɫ
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
        // ÿ�����ֽھ��ǵ����е�һ�У�һ�� height �У�height * 2 ���ֽ�
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // ����ÿһ���ֽڣ��ȴӶ�ά��������ȡ��һ���ֽڣ����У��ĵ�����
            uint8_t byte = ascii_2412[index][i];

            // ����24 * 12���ֺţ���Ҫ�ж���һ�еĵ�һ�����ǵڶ����ֽڣ�����������λ��
            uint8_t bits_count = i % 2 ? 4 : 8;

            // �������һ���ֽ��е�ÿ���㣨�ӵ�λ��ʼ�ж��ֽ��е�ÿһλ��
            for (uint8_t j = 0; j < bits_count; j++)
            {
                if (byte & 0x01)
                {
                    // �����1������������ɫ
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
        // ÿ�����ֽھ��ǵ����е�һ�У�һ�� height �У�height * 2 ���ֽ�
        for (uint8_t i = 0; i < height * 2; i++)
        {
            // ����ÿһ���ֽڣ��ȴӶ�ά��������ȡ��һ���ֽڣ����У��ĵ�����
            uint8_t byte = ascii_3216[index][i];

            // �������һ���ֽ��е�ÿ���㣨�ӵ�λ��ʼ�ж��ֽ��е�ÿһλ��
            for (uint8_t j = 0; j < 8; j++)
            {
                if (byte & 0x01)
                {
                    // �����1������������ɫ
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

// 2.3.2 ��ʾӢ�ģ�ASCII���ַ�����width = height / 2
void LCD_WriteAsciiString(uint16_t x, uint16_t y, uint16_t height,
                          uint8_t *str, uint16_t fColor, uint16_t bColor)
{
    uint16_t width = height / 2;

    // ����ѭ���������ַ����е�����λ��
    uint8_t i = 0;

    // ��'\0'��Ϊ�ַ���������־
    while (str[i] != '\0')
    {
        // �ж��Ƿ���'\n'
        if (str[i] != '\n')
        {
            // �������'\n'������Ҫ�ж��Ƿ񵽴���Ļ��ʾ�ұ߽�
            if (x + width > W)
            {
                // ����������ͻس�����
                x = 0;
                y += height;
            }
            // ��ʾ��ǰ�ַ�
            LCD_WriteAsciiChar(x, y, height, str[i], fColor, bColor);
            // �� x ����
            x += width;
        }
        else
        {
            // �������'\n'���ͻس�����
            x = 0;
            y += height;
        }
        i++;
    }
}

// 2.3.3 ��ʾ�����ַ�
void LCD_WriteChineseChar(uint16_t x, uint16_t y, uint16_t height,
                          uint8_t index, uint16_t fColor, uint16_t bColor)
{
    // 1. ������ʾ����
    LCD_SetArea(x, y, height, height);

    // 2. ��������
    LCD_WriteCmd(0x2c);

    // 3. ѭ�������ַ������ÿһ�У�4 ���ֽڱ�ʾһ��
    for (uint8_t i = 0; i < height * 4; i++)
    {
        // ����ÿһ���ֽڣ��ȴӶ�ά��������ȡ��һ���ֽڣ����У��ĵ�����
        uint8_t byte = chinese[index][i];

        // �������һ���ֽ��е�ÿ���㣨�ӵ�λ��ʼ�ж��ֽ��е�ÿһλ��
        for (uint8_t j = 0; j < 8; j++)
        {
            if (byte & 0x01)
            {
                // �����1������������ɫ
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

// 2.4 ��ʾͼƬ - �й��LOGO
void LCD_WriteAtguiguLogo(uint16_t x, uint16_t y)
{
    // 1. ������ʾ����
    LCD_SetArea(x, y, 227, 68);

    // 2. ��������
    LCD_WriteCmd(0x2c);

    // 3. ����ͼ���Ӧ���ֽ����飬ÿ2���ֽڱ�ʾһ�����ص��RGB����
    uint16_t len = sizeof(gImage_logo);
    for (uint16_t i = 0; i < len; i += 2)
    {
        // ƴ�������ֽڣ��õ�Ҫд���16λRGB���ݣ���λ��ǰ
        uint16_t pColor = gImage_logo[i] + (gImage_logo[i + 1] << 8);

        // ��������
        LCD_WriteData(pColor);
    }
}

// 2.5 ��ʾ����ͼ��
// 2.5.1 ��(�����)
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
    LCD_SetArea(x, y, w, w);

    LCD_WriteCmd(0x2c);

    // �������������е㣬����color����
    for (uint16_t i = 0; i < w * w; i++)
    {
        LCD_WriteData(color);
    }
}

// 2.5.2 �ߣ�Ĭ�� x1 <= x2, �� x1 == x2 ʱ��y1 <= y2
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t w, uint16_t color)
{
    // �ж� x1 == x2�������ȣ�ֱ�ӻ�һ������
    if (x1 == x2)
    {
        for (uint16_t y = y1; y <= y2; y++)
        {
            LCD_DrawPoint(x1, y, w, color);
        }
        return;
    }

    // ��ֱ�߷�����k��b
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

// 2.5.3 �������Σ����ģ�
void LCD_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t w, uint16_t color)
{
    uint16_t x_end = x + width - 1;
    uint16_t y_end = y + height - 1;

    // ����4����
    LCD_DrawLine(x, y, x_end, y, w, color);
    LCD_DrawLine(x, y, x, y_end, w, color);
    LCD_DrawLine(x_end, y, x_end, y_end, w, color);
    LCD_DrawLine(x, y_end, x_end, y_end, w, color);
}

// 2.5.4 ��Բ������Բ������Ͱ뾶
void LCD_DrawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    // ���ü����귽�̣����Բ��ÿһ��ĺ�������
    for (uint16_t theta = 0; theta < 360; theta++)
    {
        uint16_t x = xCenter + r * cos(theta * PI / 180);
        uint16_t y = yCenter + r * sin(theta * PI / 180);

        LCD_DrawPoint(x, y, w, color);
    }
}

// �Ľ�������ͬʱ���������ҵ�
void LCD_DrawCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color)
{
    // ���ü����귽�̣����Բ��ÿһ��ĺ�������
    for (uint16_t theta = 0; theta < 90; theta++)
    {
        uint16_t delta_x = r * cos(theta * PI / 180);
        uint16_t delta_y = r * sin(theta * PI / 180);

        // ��һ���޵�
        uint16_t x = xCenter + delta_x;
        uint16_t y = yCenter + delta_y;
        LCD_DrawPoint(x, y, w, color);

        // �ڶ����޵�
        x = xCenter - delta_x;
        y = yCenter + delta_y;
        LCD_DrawPoint(x, y, w, color);

        // �������޵�
        x = xCenter - delta_x;
        y = yCenter - delta_y;
        LCD_DrawPoint(x, y, w, color);

        // �������޵�
        x = xCenter + delta_x;
        y = yCenter - delta_y;
        LCD_DrawPoint(x, y, w, color);
    }
}

// 2.5.5 ��ʵ��Բ������Բ������Ͱ뾶
void LCD_DrawFilledCircle(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor)
{
    // ��һ��ѭ������ͣ�ı�r���������⻭ͬ��Բ
    for (uint16_t ri = 0; ri < r; ri++)
    {
        LCD_DrawCircle(xCenter, yCenter, ri, w, iColor);
    }

    // ������õ�����ɫ�����߽�
    LCD_DrawCircle(xCenter, yCenter, r, w, color);
}
void LCD_DrawFilledCircle_Pro(uint16_t xCenter, uint16_t yCenter, uint16_t r, uint16_t w, uint16_t color, uint16_t iColor)
{
    // ���ü����귽�̣����Բ��ÿһ��ĺ�������
    for (uint16_t theta = 0; theta < 90; theta++)
    {
        uint16_t delta_x = r * cos(theta * PI / 180);
        uint16_t delta_y = r * sin(theta * PI / 180);

        // ��һ���޵�
        uint16_t x1 = xCenter + delta_x;
        uint16_t y1 = yCenter + delta_y;
        LCD_DrawPoint(x1, y1, w, color);

        // �ڶ����޵�
        uint16_t x2 = xCenter - delta_x;
        uint16_t y2 = yCenter + delta_y;
        LCD_DrawPoint(x2, y2, w, color);

        // ��������һ������ֱ��
        LCD_DrawLine(x2 + w, y2, x1 - w, y1, w, iColor);

        // �������޵�
        x1 = xCenter - delta_x;
        y1 = yCenter - delta_y;
        LCD_DrawPoint(x1, y1, w, color);

        // �������޵�
        x2 = xCenter + delta_x;
        y2 = yCenter - delta_y;
        LCD_DrawPoint(x2, y2, w, color);

        // ��������һ������ֱ��
        LCD_DrawLine(x1 + w, y1, x2 - w, y2, w, iColor);
    }
}
