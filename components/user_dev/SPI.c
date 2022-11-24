/**
 * @FilePath     : /lvgl_example/components/user_dev/SPI.c
 * @Description  : spi device configration.
 * @Author       : Yana
 * @Date         : 2022-11-22 21:22:56
 * @LastEditors  : Yana
 * @LastEditTime : 2022-11-23 19:17:11
 * @Copyright    : G AUTOMOBILE RESEARCH INSTITUTE CO.,LTD Copyright (c) 2022.
 */

/* Include ------------------------------------------------------------------------------------- */
#include "SPI.h"

/* Private function ---------------------------------------------------------------------------- */
static spi_device_handle_t spi; // handle of the spi by which device send data.

/**
 * @brief  spi bus config.
 * @param  None
 * @return None
 */
void spi_io_config()
{
    esp_err_t err;
    spi_bus_config_t spi_cfg = {
        .sclk_io_num = spi_io_clk,
        .mosi_io_num = spi_io_mosi,
        .miso_io_num = spi_io_miso,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4094,
    };
    err = spi_bus_initialize(SPI3_HOST, &spi_cfg, SPI_DMA_CH2);
    ESP_ERROR_CHECK(err);
}

/**
 * @brief  callback function of dc select.
 * @param  t:
 * @return None
 */
void lcd_spi_pre_transfer_callback(spi_transaction_t *t)
{
    int dc = (int)t->user;
    gpio_set_level(spi_io_dc, dc);
}

/**
 * @brief  spi device config.
 * @param  None
 * @return None
 */
void spi_drive_config()
{
    esp_err_t err;
    spi_device_interface_config_t spi_drv;
    memset(&spi_drv, 0, sizeof(spi_drv));
    spi_drv.mode = 0;
    spi_drv.clock_speed_hz = spi_speed;
    spi_drv.spics_io_num = spi_io_cs;
    spi_drv.queue_size = 7;
    spi_drv.pre_cb = lcd_spi_pre_transfer_callback;
    err = spi_bus_add_device(SPI3_HOST, &spi_drv, &spi);
    ESP_ERROR_CHECK(err);
}

/**
 * @brief  send one byte data of reg.
 * @param  cmd:
 * @return None
 */
void lcd_write_reg(uint8_t cmd)
{
    esp_err_t err;
    spi_transaction_t sd_cmd;
    memset(&sd_cmd, 0, sizeof(sd_cmd));
    sd_cmd.length = 8;
    sd_cmd.tx_buffer = &cmd;
    sd_cmd.user = (void *)0;
    err = spi_device_polling_transmit(spi, &sd_cmd);
    ESP_ERROR_CHECK(err);
}

/**
 * @brief  send one byte data.
 * @param  data:
 * @return None
 */
void lcd_write_data(uint8_t data)
{
    esp_err_t err;
    spi_transaction_t sd_dat;
    memset(&sd_dat, 0, sizeof(sd_dat));
    sd_dat.length = 8;
    sd_dat.tx_buffer = &data;
    sd_dat.user = (void *)1;
    err = spi_device_polling_transmit(spi, &sd_dat);
    ESP_ERROR_CHECK(err);
}

/**
 * @brief  send the data of color.
 * @param  data:
 * @return None
 */
void lcd_write_color(lcd_color_t data)
{
    esp_err_t err;
    spi_transaction_t sd_dat;
    memset(&sd_dat, 0, sizeof(sd_dat));
    sd_dat.length = 3 * 8;
    sd_dat.tx_buffer = &data;
    sd_dat.user = (void *)1;
    err = spi_device_polling_transmit(spi, &sd_dat);
    ESP_ERROR_CHECK(err);
}

/**
 * @brief  transmit data by spi.
 * @param  data:
 * @param  size:
 * @return None
 */
void lcd_write_color_x(lcd_color_t *data, uint32_t size)
{
    esp_err_t err;
    spi_transaction_t sd_datx;
    memset(&sd_datx, 0, sizeof(sd_datx));
    sd_datx.length = 24 * size;
    sd_datx.tx_buffer = data;
    sd_datx.user = (void *)1;
    err = spi_device_polling_transmit(spi, &sd_datx);
    ESP_ERROR_CHECK(err);
}

