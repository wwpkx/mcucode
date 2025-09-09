/*
 * @Author: wushengran
 * @Date: 2024-09-13 16:29:39
 * @Description:
 *
 * Copyright (c) 2024 by atguigu, All Rights Reserved.
 */

#include "usart.h"
#include "lcd.h"

int main(void)
{
	// 1. ��ʼ��
	USART_Init();
	LCD_Init();

	printf("�й��FSMC��չLCDʵ��...\n");

	// 2. ��ȡID
	uint32_t id = LCD_ReadID();
	printf("id = %#x\n", id);

	// 3. ��һ����ɫ�������豳��ɫ
	LCD_ClearAll(WHITE);

	// 4. ��ʾ����Ӣ���ַ�
	LCD_WriteAsciiChar(10, 10, 16, 'A', RED, WHITE);
	LCD_WriteAsciiChar(10, 30, 24, 'B', RED, WHITE);
	LCD_WriteAsciiChar(10, 60, 32, 'R', BLUE, YELLOW);

	// 5. ��ʾӢ���ַ���
	LCD_WriteAsciiString(200, 200, 24, "Hello, Atguigu! Hello, hello! At\nguigu! Hello, wolrd!", BLACK, WHITE);

	uint8_t str[] = {'a', 'b', '\0'};
	LCD_WriteAsciiString(200, 300, 24, str, BLACK, WHITE);

	// 6. ��ʾ�����ַ�
	LCD_WriteChineseChar(20, 330, 32, 0, RED, BLUE);
	LCD_WriteChineseChar(20, 362, 32, 1, BLUE, RED);
	LCD_WriteChineseChar(20, 394, 32, 2, GRAY, RED);

	// 7. ��ʾͼƬ - �й��LOGO
	LCD_WriteAtguiguLogo(50, 100);

	// 8. ����
	LCD_DrawPoint(300, 300, 5, RED);

	// 9. ����
	LCD_DrawLine(10, 10, 10, 300, 5, RED);
	LCD_DrawLine(10, 20, 300, 100, 3, BLUE);
	
	// 10. ��������
	LCD_DrawRectangle(15, 15, 280, 55, 3, RED);

	// 11. ������Բ
	LCD_DrawCircle(150, 400, 50, 3, BLUE);

	// 12. ��ʵ��Բ
	LCD_DrawFilledCircle(260, 400, 50, 3, BLUE, RED);
	// LCD_DrawFilledCircle_Pro(260, 400, 50, 3, BLUE, RED);

	while (1)
	{
	}
}
