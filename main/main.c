#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "mqtt.h"
#include "dht11.h"
#include "led.h"
#include "nvs.h"
#include "button.h"
#include "buzzer.h"

xSemaphoreHandle conexaoWifiSemaphore;
xSemaphoreHandle conexaoMQTTSemaphore;

int current_humidity = 0;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      mqtt_start();
      start_led();
      inicia_buzzer();
    }
  }
}

void trataComunicacaoComServidor(void * params)
{
  char mensagem[50];
  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
      if(CONFIG_ESP_MODE == 1)
      {
        struct dht11_reading dados = DHT11_read();
        if(dados.temperature != -1 && dados.temperature != -1){
          sprintf(mensagem, "{\"temperatura\":%d, \n\"umidade\": %d}", dados.temperature,dados.humidity);
          mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
          current_humidity = dados.humidity;
        }
      }
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}

void trataBuzzer(void * params)
{
    while(true)
    {
      if(current_humidity > 55)
      {
        aciona_buzzer();
      }
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}


void app_main(void)
{
    printf("MODO: %d\n", CONFIG_ESP_MODE);
    init_nvs();
    init_button();

    if(CONFIG_ESP_MODE == 1)
    {
      DHT11_init(GPIO_NUM_4);
    }
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Processa HTTP", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
    xTaskCreate(&trataBuzzer, "Buzzer", 4096, NULL, 1, NULL);

    
}
