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
/* Private function prototypes -----------------------------------------------------------------------------------------*/
void Lvgl_task(void *arg);
void Gui_task(void *arg);

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
    err = esp_timer_start_periodic(lvgl_timer_handle, 1000); // 1����ص�
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "lvgl��ʱ�������ɹ�\r\n");
    }
}

void app_main(void)
{
    lcd_init();
    lv_init();           // lvgl core init
    lv_port_disp_init(); // lvgl display windos init
    _lv_timer_create();

    /*create task.*/
    xTaskCreate(Lvgl_task, "Lvgl_task", 4096, NULL, 2, NULL);
    vTaskDelay((1000) / portTICK_PERIOD_MS);
    xTaskCreate(Gui_task, "Gui_task", 4096, NULL, 3, NULL);
}

void Lvgl_task(void *arg)
{
    while (1)
    {
        vTaskDelay(((10) / portTICK_PERIOD_MS));
        lv_task_handler();
    }
}

void Gui_task(void *arg)
{
    static lv_obj_t *default_src;
    default_src = lv_scr_act();                     //��ȡĬ����Ļ
    lv_obj_t *label = lv_label_create(default_src); /*Add a label to the button*/
    lv_label_set_text(label, "hello_world,bitch."); /*Set the labels text*/
    lv_obj_center(label);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
    while (1)
    {
        vTaskDelay((1000) / portTICK_PERIOD_MS);
    }
}