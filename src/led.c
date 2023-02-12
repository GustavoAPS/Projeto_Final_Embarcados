#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "mqtt.h"
#include "led.h"

#define LED 2

void led_routine()
{
  esp_rom_gpio_pad_select_gpio(LED);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);

  int estado = 0;
  char mensagem[50];
  gpio_set_level(LED, estado);
  estado = !estado;
  sprintf(mensagem, "{\"led\": %d}", estado);
  mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);

}