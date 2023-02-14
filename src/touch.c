#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "touch.h"
#include "nvs_handler.h"
#include "mqtt.h"

#define TOUCH_IO  25  // Select the input GPIO pin for touch

#define LED 2 

void touch_routine()
{
    char mensagem[50];
    gpio_config_t  io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << TOUCH_IO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    esp_rom_gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    while (1)
    {
        int touched = gpio_get_level(TOUCH_IO);
        gpio_set_level(LED, touched);
        sprintf(mensagem, "{\"touch\": %d}", touched);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        sprintf(mensagem, "{\"led\": %d}", touched);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
    
    
}