#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "mqtt.h"
#include "wifi.h"
#include "mqtt.h"
#include "dht11.h"
#include "button.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "mqtt.h"
#include "interruptor.h"

#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO 17
#define LEDC_HS_CH1_GPIO 18
#define LEDC_HS_CH0_CHANNEL LEDC_CHANNEL_0
#define LEDC_HS_CH1_CHANNEL LEDC_CHANNEL_1
#define LEDC_TEST_DUTY (4000)
#define LEDC_TEST_FADE_TIME (3000)

#define PIN_PHOTO_INTERRUPTOR 15  // GPIO15
#define BUTTON 0

int interrompe = 0;
int red_duty = 0;
int green_duty = 4000;
int direction = 1;



void button_routine()
{
	esp_rom_gpio_pad_select_gpio(PIN_PHOTO_INTERRUPTOR);
    gpio_set_direction(PIN_PHOTO_INTERRUPTOR, GPIO_MODE_INPUT);

	ledc_timer_config_t ledc_timer = {
    .duty_resolution = LEDC_TIMER_13_BIT,
    .freq_hz = 5000,
    .speed_mode = LEDC_HS_MODE,
    .timer_num = LEDC_HS_TIMER
	};

	ledc_timer_config(&ledc_timer);

	// configure ledc channels
	ledc_channel_config_t ledc_red = {
		.channel = LEDC_HS_CH0_CHANNEL,
		.duty = 0,
		.gpio_num = LEDC_HS_CH0_GPIO,
		.speed_mode = LEDC_HS_MODE,
		.timer_sel = LEDC_HS_TIMER
	};
	ledc_channel_config_t ledc_green = {
		.channel = LEDC_HS_CH1_CHANNEL,
		.duty = 4000,
		.gpio_num = LEDC_HS_CH1_GPIO,
		.speed_mode = LEDC_HS_MODE,
		.timer_sel = LEDC_HS_TIMER
	};

	ledc_channel_config(&ledc_red);
	ledc_channel_config(&ledc_green);

	char mensagem[50];
    gpio_config_t  io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << BUTTON);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

	int intensidade = 0;


	while (true)
	{
		int state = gpio_get_level(PIN_PHOTO_INTERRUPTOR);
		int estado = gpio_get_level(BUTTON);

		if (red_duty == 4000) {
					direction = -1;
				}
				else if (red_duty == 0) {
					direction = 1;
				}
				red_duty += direction * 200;
				green_duty -= direction * 200;


		if (state == 0)
		{
			if(estado)
			{	
				
				
				// liga verde
				ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL, green_duty);
				ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL);
				ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, 0);
				ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL);
				sprintf(mensagem, "{\"yellow\": %d}", 1);
        		mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
				sprintf(mensagem, "{\"red\": %d}", 0);
        		mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);

			}
			else{
				// desliga os dois
				ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, 0);
				ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL);
				ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL, 0);
				ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL);
				sprintf(mensagem, "{\"yellow\": %d}", 0);
        		mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
				sprintf(mensagem, "{\"red\": %d}", 0);
        		mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
				
				}
		}
		else
		{
			
			
			// liga vermelho
			
			ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL, red_duty);
			ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH0_CHANNEL);
			ledc_set_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL, 0);
			ledc_update_duty(LEDC_HS_MODE, LEDC_HS_CH1_CHANNEL);
			sprintf(mensagem, "{\"yellow\": %d}", 0);
			mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
			sprintf(mensagem, "{\"red\": %d}", 1);
			mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
		}
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}
