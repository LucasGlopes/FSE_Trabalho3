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

xSemaphoreHandle conexaoWifiSemaphore;
xSemaphoreHandle conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      mqtt_start();
      start_led();
    }
  }
}

void trataComunicacaoComServidor(void * params)
{
  char mensagem[50];
  char JSONAtributos[200];

  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
      struct dht11_reading dados = DHT11_read();
      if(dados.temperature != -1 && dados.temperature != -1){
        sprintf(mensagem, "{\"temperatura\":%d, \n\"umidade\": %d}", dados.temperature,dados.humidity);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
      }

      sprintf(JSONAtributos, "{\"quantidade de pinos\": 5, \n\"umidade\": 20}");
      mqtt_envia_mensagem("v1/devices/me/attributes", JSONAtributos);

      vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
  }
}




void app_main(void)
{

    init_nvs();
    DHT11_init(GPIO_NUM_4);
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Processa HTTP", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);


    
}
