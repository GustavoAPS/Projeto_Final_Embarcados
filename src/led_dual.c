#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "mqtt.h"
#include "led.h"

#define YELLOW 17
#define RED 16

void led_dual_routine()
{
    esp_rom_gpio_pad_select_gpio(YELLOW);
    gpio_set_direction(YELLOW, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(RED);
    gpio_set_direction(RED, GPIO_MODE_OUTPUT);

    int estado = 0;
    char mensagem[20];
    while (true){
        gpio_set_level(YELLOW, estado);
         gpio_set_level(RED, !estado);
        estado = !estado;
        sprintf(mensagem, "{\"yellow\": %d}", estado);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        sprintf(mensagem, "{\"red\": %d}", estado);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

  }


}