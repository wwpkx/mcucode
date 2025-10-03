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
	// 1. 初始化
	USART_Init();
	LCD_Init();

	printf("尚硅谷FSMC扩展LCD实验...\n");

	// 2. 读取ID
	uint32_t id = LCD_ReadID();
	printf("id = %#x\n", id);

	// 3. 用一种颜色清屏，设背景色
	LCD_ClearAll(WHITE);

	// 4. 显示单个英文字符
	LCD_WriteAsciiChar(10, 10, 16, 'A', RED, WHITE);
	LCD_WriteAsciiChar(10, 30, 24, 'B', RED, WHITE);
	LCD_WriteAsciiChar(10, 60, 32, 'R', BLUE, YELLOW);

	// 5. 显示英文字符串
	LCD_WriteAsciiString(200, 200, 24, "Hello, Atguigu! Hello, hello! At\nguigu! Hello, wolrd!", BLACK, WHITE);

	uint8_t str[] = {'a', 'b', '\0'};
	LCD_WriteAsciiString(200, 300, 24, str, BLACK, WHITE);

	// 6. 显示中文字符
	LCD_WriteChineseChar(20, 330, 32, 0, RED, BLUE);
	LCD_WriteChineseChar(20, 362, 32, 1, BLUE, RED);
	LCD_WriteChineseChar(20, 394, 32, 2, GRAY, RED);

	// 7. 显示图片 - 尚硅谷LOGO
	LCD_WriteAtguiguLogo(50, 100);

	// 8. 画点
	LCD_DrawPoint(300, 300, 5, RED);

	// 9. 画线
	LCD_DrawLine(10, 10, 10, 300, 5, RED);
	LCD_DrawLine(10, 20, 300, 100, 3, BLUE);
	
	// 10. 画长方形
	LCD_DrawRectangle(15, 15, 280, 55, 3, RED);

	// 11. 画空心圆
	LCD_DrawCircle(150, 400, 50, 3, BLUE);

	// 12. 画实心圆
	LCD_DrawFilledCircle(260, 400, 50, 3, BLUE, RED);
	// LCD_DrawFilledCircle_Pro(260, 400, 50, 3, BLUE, RED);

	while (1)
	{
	}
}
