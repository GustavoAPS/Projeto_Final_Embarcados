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
#include "nvs_handler.h"

#include "button.h"

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
    xTaskCreate(&DHT11_routine, "Rotina do temperatura1", 2048, NULL, 11, NULL);
    xTaskCreate(&routine_lm35c, "Rotina do temperatura2", 2048, NULL, 11, NULL);
    xTaskCreate(&button_routine, "Rotina do led2", 2048, NULL, 11, NULL);
    xTaskCreate(&touch_routine, "Rotina do touch", 2048, NULL, 11, NULL);

    while (1)
    {
      rotina_interruptor();
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
     //xTaskCreate(&routine_lm35c, "Rotina do lm35c", 4096, NULL, 1, NULL);
     //xTaskCreate(&DHT11_routine, "Rotina do dht11", 4096, NULL, 1, NULL);
  }
}

void app_main(void)
{
    inicia_nvs();
    float temperatura = 0;
    float umidade = 0;
    temperatura = le_valor_nvs("Temperatura");
    umidade = le_valor_nvs("Umidade");


    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();

    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
}
