/*
 * @FilePath     : /lvgl_example/components/Hardware/ili9844
 * @Description  :
 * @Author       : Yana
 * @Date         : 2022-11-22 21:27:28
 * @LastEditors  : Yana
 * @LastEditTime : 2022-11-22 21:39:59
 * Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2022.
 */

/* Include ------------------------------------------------------------------------------------- */
#include "ili9488.h"

/* Private function ---------------------------------------------------------------------------- */
static void lcd_pin_config()
{
    gpio_pad_select_gpio(spi_io_rst);
    gpio_pad_select_gpio(spi_io_led);
    gpio_pad_select_gpio(spi_io_dc);
    /* Set the orientation of the pin. */
    gpio_set_direction(spi_io_rst, GPIO_MODE_OUTPUT);
    gpio_set_direction(spi_io_led, GPIO_MODE_OUTPUT);
    gpio_set_direction(spi_io_dc, GPIO_MODE_OUTPUT);
}

/**
 * @brief  return the color data changed to 16 bit.
 * @param  data:
 * @return None
 */
uint16_t color_return(uint16_t data)
{
    uint16_t red, gre, blu, color;
    red = (data >> 11) & 0x1f;
    gre = (data >> 5) & 0x3f;
    blu = data & 0x1f;
    color = red + gre + blu;
    return color;
}

/**
 * @brief  set cursor position.
 * @return None
 */
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    /* Sets the column orientation */
    lcd_write_reg(0x2a);
    lcd_write_data(xStar >> 8);
    lcd_write_data(0x00FF & xStar);
    lcd_write_data(xEnd >> 8);
    lcd_write_data(0x00FF & xEnd);
    /* Sets the row orientation */
    lcd_write_reg(0x2b);
    lcd_write_data(yStar >> 8);
    lcd_write_data(0x00FF & yStar);
    lcd_write_data(yEnd >> 8);
    lcd_write_data(0x00FF & yEnd);
    /* Start write data */
    lcd_write_reg(0x2c);
}

/**
 * @brief  change the display direction of lcd.
 * @param  direction:
 * @return None
 */
void LCD_Direction(uint8_t direction)
{
    switch (direction)
    {
    case 0:
        lcd_write_reg(0x36);
        lcd_write_data((1 << 3) | (0 << 6) | (0 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 1:
        lcd_write_reg(0x36);
        lcd_write_data((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    case 2:
        lcd_write_reg(0x36);
        lcd_write_data((1 << 3) | (1 << 6) | (1 << 7)); // BGR==1,MY==0,MX==0,MV==0
        break;
    case 3:
        lcd_write_reg(0x36);
        lcd_write_data((1 << 3) | (1 << 7) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
        break;
    default:
        break;
    }
}

/**
 * @brief  return data in three bytes format.
 * @param  color:
 * @return None
 */
lcd_color_t color_format(uint16_t color)
{
    lcd_color_t lcd_color;
    lcd_color.color[0] = (color >> 8) & 0xF8; // RED
    lcd_color.color[1] = (color >> 3) & 0xFC; // GREEN
    lcd_color.color[2] = color << 3;
    return lcd_color;
}

/**
 * @brief  fill the area with color.
 * @param  color:
 * @return None
 */
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color)
{
    uint16_t i, j;
    uint16_t width = ex - sx + 1;  //得到填充的宽度
    uint16_t height = ey - sy + 1; //高度

    LCD_SetWindows(sx, sy, ex, ey); //设置显示窗口
    for (i = 0; i < height; i++)
    {
        lcd_color_t lcd_color[width];
        for (j = 0; j < width; j++)
        {
            lcd_color[j] = color_format(color[i * width + j]);
        }
        lcd_write_color_x(lcd_color, width);
    }
    LCD_SetWindows(0, 0, 320 - 1, 480 - 1); //恢复窗口设置为全屏
}

/**
 * @brief  lcd reset.
 * @param  None
 * @return None
 */
static void lcd_reset()
{
    gpio_set_level(spi_io_rst, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(spi_io_rst, 1);
    vTaskDelay(50 / portTICK_RATE_MS);
}

/**
 * @brief  Initialize config for the lcd reg.
 * @param  None
 * @return None
 */
void lcd_init()
{
    /* spi serise init and device config. */
    lcd_pin_config();
    spi_io_config();
    spi_drive_config();
    /* reset lcd to clear config. */
    lcd_reset();
    /* Adjust Control 3 (F7h)  */
    lcd_write_reg(0XF7);
    lcd_write_data(0xA9);
    lcd_write_data(0x51);
    lcd_write_data(0x2C);
    lcd_write_data(0x82);
    /* Power Control 1 (C0h)  */
    lcd_write_reg(0xC0);
    lcd_write_data(0x11);
    lcd_write_data(0x09);
    /* Power Control 2 (C1h) */
    lcd_write_reg(0xC1);
    lcd_write_data(0x41);
    /* VCOM Control (C5h)  */
    lcd_write_reg(0XC5);
    lcd_write_data(0x00);
    lcd_write_data(0x0A);
    lcd_write_data(0x80);
    /* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
    lcd_write_reg(0xB1);
    lcd_write_data(0xB0);
    lcd_write_data(0x11);
    /* Display Inversion Control (B4h) */
    lcd_write_reg(0xB4);
    lcd_write_data(0x02);
    /* Display Function Control (B6h)  */
    lcd_write_reg(0xB6);
    lcd_write_data(0x02);
    lcd_write_data(0x42);
    // lcd_write_data(0x22);
    /* Entry Mode Set (B7h)  */
    lcd_write_reg(0xB7);
    lcd_write_data(0xc6);
    /* HS Lanes Control (BEh) */
    lcd_write_reg(0xBE);
    lcd_write_data(0x00);
    lcd_write_data(0x04);
    /* Set Image Function (E9h)  */
    lcd_write_reg(0xE9);
    lcd_write_data(0x00);

    lcd_write_reg(0x36);
    // lcd_write_data(0x08);
    lcd_write_data((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));
    /* Interface Pixel Format (3Ah) */
    lcd_write_reg(0x3A);
    lcd_write_data(0x66);
    /* PGAMCTRL (Positive Gamma Control) (E0h) */
    lcd_write_reg(0xE0);
    lcd_write_data(0x00);
    lcd_write_data(0x07);
    lcd_write_data(0x10);
    lcd_write_data(0x09);
    lcd_write_data(0x17);
    lcd_write_data(0x0B);
    lcd_write_data(0x41);
    lcd_write_data(0x89);
    lcd_write_data(0x4B);
    lcd_write_data(0x0A);
    lcd_write_data(0x0C);
    lcd_write_data(0x0E);
    lcd_write_data(0x18);
    lcd_write_data(0x1B);
    lcd_write_data(0x0F);
    /* NGAMCTRL (Negative Gamma Control) (E1h)  */
    lcd_write_reg(0XE1);
    lcd_write_data(0x00);
    lcd_write_data(0x17);
    lcd_write_data(0x1A);
    lcd_write_data(0x04);
    lcd_write_data(0x0E);
    lcd_write_data(0x06);
    lcd_write_data(0x2F);
    lcd_write_data(0x45);
    lcd_write_data(0x43);
    lcd_write_data(0x02);
    lcd_write_data(0x0A);
    lcd_write_data(0x09);
    lcd_write_data(0x32);
    lcd_write_data(0x36);
    lcd_write_data(0x0F);
    /* Sleep Out (11h) */
    lcd_write_reg(0x11);
    vTaskDelay(120 / portTICK_RATE_MS);
    lcd_write_reg(0x29); // start display.
    /* Setting the Display Direction. */
    LCD_Direction(0);
    /* Turn on the backlight. */
    gpio_set_level(spi_io_led, 1);
}
