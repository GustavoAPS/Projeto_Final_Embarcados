#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "touch.h"
#include "wifi.h"
#include "mqtt.h"
#include "lm35.h"
#include "dht11.h"
#include "interruptor.h"
#include "led.h"
#include "led_dual.h"

SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  }
}

void trataComunicacaoComServidor(void * params)
{
  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY))
  {
    xTaskCreate(led_routine, "Rotina do led", 2048, NULL, 10, NULL);
    xTaskCreate(&led_dual_routine, "Rotina do led2", 2048, NULL, 11, NULL);
    while (1)
    {
      touch_routine();
      routine_lm35c();
      rotina_interruptor();
      DHT11_routine();
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
     //xTaskCreate(&routine_lm35c, "Rotina do lm35c", 4096, NULL, 1, NULL);
     //xTaskCreate(&DHT11_routine, "Rotina do dht11", 4096, NULL, 1, NULL);
  }
}

void app_main(void)
{
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
}
