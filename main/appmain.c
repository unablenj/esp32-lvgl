#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <string.h>
#include "esp_log.h"
#include "freertos/queue.h"
#include "SPI.h"
#include "ili9488.h"
#include "lvgl.h"
#include "../components/lvgl/lv_driver/lv_port_disp.h"

static const char *TAG = "main";

static esp_timer_handle_t lvgl_timer_handle = NULL;

static IRAM_ATTR void lv_timer_cb(void *arg)
{
    lv_tick_inc(1);
}

static esp_timer_create_args_t lvgl_timer = {
    .callback = &lv_timer_cb,
    .arg = NULL,
    .name = "lvgl_timer",
    .dispatch_method = ESP_TIMER_TASK};

void _lv_timer_create(void)
{
    esp_err_t err = esp_timer_create(&lvgl_timer, &lvgl_timer_handle);
    err = esp_timer_start_periodic(lvgl_timer_handle, 1000); // 1毫秒回调
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "lvgl定时器创建成功\r\n");
    }
}

void app_main(void)
{
    lcd_init();
    lv_init();           // lvgl内核初始化
    lv_port_disp_init(); // lvgl显示接口初始化
    _lv_timer_create();

    static lv_obj_t *default_src;
    default_src = lv_scr_act();                     //获取默认屏幕
    lv_obj_t *label = lv_label_create(default_src); /*Add a label to the button*/
    lv_label_set_text(label, "hello_world");        /*Set the labels text*/
    lv_obj_center(label);

    while (1)
    {
        vTaskDelay(((10) / portTICK_PERIOD_MS));
        lv_task_handler();
    }
}
