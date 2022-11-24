/**
 * @FilePath     : /lvgl_example/components/user_dev/SPI.h
 * @Description  :
 * @Author       : Yana
 * @Date         : 2022-11-22 21:23:03
 * @LastEditors  : Yana
 * @LastEditTime : 2022-11-23 19:03:27
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2022.
 */
#ifndef _SPI_H_
#define _SPI_H_
/* Include ------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "sdkconfig.h"

/* Macro define -------------------------------------------------------------------------------- */
#define spi_io_mosi 23 /*!< for 1-line SPI, this also refered as MOSI */
#define spi_io_miso -1
#define spi_io_clk 18
#define spi_io_cs 5
#define spi_io_dc 21
#define spi_io_rst 4
#define spi_io_led 15
/* the transmit speed of spi device. */
#define spi_speed SPI_MASTER_FREQ_40M

typedef struct lcd_color_t
{
    uint8_t color[3];
} lcd_color_t;
/* Private function ---------------------------------------------------------------------------- */
void spi_io_config(void);
void spi_drive_config(void);
void lcd_write_reg(uint8_t cmd);
void lcd_write_data(uint8_t data);
void lcd_write_color(lcd_color_t data);
void lcd_write_color_x(lcd_color_t *data, uint32_t size);

#endif
