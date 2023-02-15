#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt.h"

#define PIN_PHOTO_INTERRUPTOR 15  // GPIO15

void rotina_interruptor()
{
    int state = 0;
    ESP_LOGI("Photo-interrupter", "Starting...");
    esp_rom_gpio_pad_select_gpio(PIN_PHOTO_INTERRUPTOR);
    gpio_set_direction(PIN_PHOTO_INTERRUPTOR, GPIO_MODE_INPUT);

    char mensagem[50];
    state = gpio_get_level(PIN_PHOTO_INTERRUPTOR);
    if (state == 0)
        ESP_LOGI("Photo-interrupter", "Sensor detected something");
    else
        ESP_LOGI("Photo-interrupter", "No sensor detected");
    sprintf(mensagem, "{\"interruptor\": %d}", state);
    mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
    
}