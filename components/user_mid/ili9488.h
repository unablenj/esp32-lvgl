/**
 * @FilePath     : /lvgl_example/components/user_mid/ili9488.h
 * @Description  : 
 * @Author       : Yana
 * @Date         : 2022-11-22 21:34:56
 * @LastEditors  : Yana
 * @LastEditTime : 2022-11-24 20:17:41
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2022.
 */

#ifndef _ili9488_H_
#define _ili9488_H_
/* Include ------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "SPI.h"
#include "sdkconfig.h"

/* Macro define -------------------------------------------------------------------------------- */
/* size of screen. */
#define lcd_width 320
#define lcd_hight 480

/* color of the brush. */ 
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
#define BROWN 0XBC40 //棕色
#define BRRED 0XFC07 //棕红色
#define GRAY 0X8430  //灰色

/* the color of the GUI */ 
#define DARKBLUE 0X01CF  //深蓝色
#define LIGHTBLUE 0X7D7C //浅蓝色
#define GRAYBLUE 0X5458  //灰蓝色

/* the color of the pannel */
#define LIGHTGREEN 0X841F //浅绿色
#define LIGHTGRAY 0XEF5B  //浅灰色(PANNEL)
#define LGRAY 0XC618      //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE 0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12    //浅棕蓝色(选择条目的反色)

/* Private function ---------------------------------------------------------------------------- */
uint16_t color_return(uint16_t data);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);
void LCD_Direction(uint8_t direction);
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);

void lcd_init(void);

#endif
